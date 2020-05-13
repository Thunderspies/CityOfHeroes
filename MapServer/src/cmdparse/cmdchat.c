

#include "cmdchat.h"
#include <utilitieslib/utils/cmdoldparse.h>
#include "cmdserver.h"
#include "cmdservercsr.h"
#include "gameComm/svr_chat.h"
#include "language/langServerUtil.h"
#include "comm_backend.h"
#include "dbcomm/dbcontainer.h"
#include "cmdparse/cmdenum.h"
#include "entity/friends.h"
#include "entity/costume.h"
#include "entity/power_customization.h"
#include "entity/entserver.h"
#include "entity/entVarUpdate.h"
#include "comm_game.h"
#include "entity/entity.h"
#include "container/team.h"
#include "container/league.h"
#include "gameComm/sendToClient.h"
#include "dbcomm/dbquery.h"
#include "entity/entPlayer.h"
#include "gameComm/trading.h"
#include "dbcomm/dbcomm.h"
#include "entity/character_base.h"
#include "entity/buddy_server.h"
#include "storyarc/storyarcprivate.h"
#include "filter/validate_name.h"
#include "storyarc/mission.h"
#include "entity/character_level.h"
#include "gameComm/search.h"
#include <utilitieslib/utils/utils.h>
#include "player/badges.h"
#include "svr/svr_player.h"
#include "gameSys/arenamap.h"
#include "gameSys/arenamapserver.h"
#include "gameData/arenastruct.h"
#include "dbcomm/dbnamecache.h"
#include "dbcomm/shardcomm.h"
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include "entity/SgrpServer.h"
#include "gameSys/raidmapserver.h"
#include "gameSys/sgraid.h"
#include "reward.h"
#include "entity/entStrings.h"
#include "comm_game.h"
#include "auth/authUserData.h"
#include "cmdparse/cmdstatserver.h"
#include "player/badges_server.h"
#include "dbcomm/staticMapInfo.h"
#include "entity/teamup.h"
#include <utilitieslib/language/messagestoreutil.h>
#include "gameSys/turnstile.h"
#include "container/containerbroadcast.h"
#include "entity/character_eval.h"
#include "gamesys/endgameraid.h"
#include "door.h"


static char tmp_str_cmd[1000], tmp_str2_cmd[1000], player_name_cmd[64];
static int tmp_int, tmp_int2, tmp_int3, tmp_int4, tmp_int5, tmp_int6, tmp_int7, tmp_dbid;
static char tmp_taskdesc[100];

static void *tmp_var_list[] = { // List of tmp_* vars in order to automatically add CMDF_HIDEPRINT
    &tmp_str_cmd, &tmp_str2_cmd, &player_name_cmd,
    &tmp_int, &tmp_int2, &tmp_int3, &tmp_int4, &tmp_int5, 
    &tmp_int6, &tmp_int7,&tmp_dbid,&tmp_taskdesc,
};

Cmd chat_cmds[] =
{
    // chat channels
    //-------------------------------------------------------------------

    { 0, "tell", CMD_CHAT_PRIVATE, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send a message to only one player." },
    { 0, "t", CMD_CHAT_PRIVATE, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send a message to only one player." },
    { 0, "ttl", CMD_CHAT_PRIVATE_TEAM_LEADER, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send a message to the team leader." },
    { 0, "tll", CMD_CHAT_PRIVATE_LEAGUE_LEADER, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send a message to the league leader." },
    { 0, "private", CMD_CHAT_PRIVATE, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send a message to only one player." },
    { 0, "p", CMD_CHAT_PRIVATE, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send a message to only one player." },
    { 0, "whisper", CMD_CHAT_PRIVATE, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send a message to only one player." },
    { 0, "tellTrialMessage", CMD_CHAT_TRIAL_RESPONSE, {{ CMDSTR( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR |CMDF_HIDEPRINT,
    "Send a response from a trial explaining that they can't use tells." },

    { 0, "friendlist", CMD_FRIENDLIST, {{0}}, 0,
    "Display friend list." },
    { 0, "fl", CMD_FRIENDLIST, {{0}}, 0,
    "Display friend list." },

    { 0, "group", CMD_CHAT_TEAM, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to group channel." },
    { 0, "g", CMD_CHAT_TEAM, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to group channel." },
    { 0, "team", CMD_CHAT_TEAM, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to group channel." },

    { 0, "yell", CMD_CHAT_BROADCAST, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to entire map." },
    { 0, "y", CMD_CHAT_BROADCAST, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to entire map." },
    { 0, "broadcast", CMD_CHAT_BROADCAST, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to entire map." },
    { 0, "b", CMD_CHAT_BROADCAST, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to entire map." },

    { 0, "r", CMD_CHAT_REPLY, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Reply to last person that sent you a tell." },
    { 0, "reply", CMD_CHAT_REPLY, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Reply to last person that sent you a tell." },

    { 0, "say", CMD_CHAT_LOCAL, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to your area." },
    { 0, "local", CMD_CHAT_LOCAL, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to your area." },
    { 0, "l", CMD_CHAT_LOCAL, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to your area." },
    { 0, "s", CMD_CHAT_LOCAL, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to your area." },

    { 0, "request", CMD_CHAT_REQUEST, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to request channel." },
    { 0, "req", CMD_CHAT_REQUEST, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to request channel." },
    { 0, "sell", CMD_CHAT_REQUEST, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to request channel." },
    { 0, "auction", CMD_CHAT_REQUEST, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to request channel." },

    { 0, "lookingforgroup", CMD_CHAT_LOOKING_FOR_GROUP, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to Looking for Group channel." },
    { 0, "lfg", CMD_CHAT_LOOKING_FOR_GROUP, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to Looking for Group channel." },


    { 0, "supergroup", CMD_CHAT_SUPERGROUP, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to super group channel." },
    { 0, "sg", CMD_CHAT_SUPERGROUP, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to super group channel." },
    { 0, "lp", CMD_CHAT_LEVELINGPACT, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to leveling pact channel." },

    { 0, "coalition", CMD_CHAT_ALLIANCE, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to coalition channel." },

    { 0, "c", CMD_CHAT_ALLIANCE, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Send message to coalition channel." },

    { 0, "league_chat", CMD_CHAT_LEAGUE, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Send message to league channel." },
    { 0, "league", CMD_CHAT_LEAGUE, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Send message to league channel." },
    { 0, "lc", CMD_CHAT_LEAGUE, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Send message to league channel." },
    { 2, "a", CMD_CHAT_ADMIN, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS,
    "Admin chat command" },
    { 2, "mapadmin", CMD_CHAT_MAP_ADMIN, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS,
    "Admin chat command to just current map" },

    { 0, "arena_local", CMD_CHAT_ARENA, {{ CMDSENTENCE( tmp_str_cmd ) }}, CMDF_HIDEVARS|CMDF_RETURNONERROR|CMDF_HIDEPRINT,
    "Local arena chat channel" },

    { 0, "ac", CMD_CHAT_ARENA_GLOBAL, {{ CMDSENTENCE( tmp_str_cmd ) }}, CMDF_HIDEVARS|CMDF_RETURNONERROR,
    "Arena chat channel" },

    { 0, "arena", CMD_CHAT_ARENA_GLOBAL, {{ CMDSENTENCE( tmp_str_cmd ) }}, CMDF_HIDEVARS|CMDF_RETURNONERROR,
    "Arena chat channel" },

    { 0, "ma", CMD_CHAT_ARCHITECT_GLOBAL, {{ CMDSENTENCE( tmp_str_cmd ) }}, CMDF_HIDEVARS|CMDF_RETURNONERROR,
        "Archiect chat channel" },

    { 0, "mission_architect", CMD_CHAT_ARCHITECT_GLOBAL, {{ CMDSENTENCE( tmp_str_cmd ) }}, CMDF_HIDEVARS|CMDF_RETURNONERROR,
        "Archiect chat channel" },

    { 0, "h", CMD_CHAT_HELP_GLOBAL, {{ CMDSENTENCE( tmp_str_cmd ) }}, CMDF_HIDEVARS|CMDF_RETURNONERROR,
    "Help/guide chat channel" },

    { 0, "help", CMD_CHAT_HELP_GLOBAL, {{ CMDSENTENCE( tmp_str_cmd ) }}, CMDF_HIDEVARS|CMDF_RETURNONERROR,
    "Help/guide chat channel" },

    { 0, "hc", CMD_CHAT_HELP_GLOBAL, {{ CMDSENTENCE( tmp_str_cmd ) }}, CMDF_HIDEVARS|CMDF_RETURNONERROR,
    "Help/guide chat channel" },

    { 0, "helpchat", CMD_CHAT_HELP_GLOBAL, {{ CMDSENTENCE( tmp_str_cmd ) }}, CMDF_HIDEVARS|CMDF_RETURNONERROR,
    "Help/guide chat channel" },

    { 0, "guide", CMD_CHAT_HELP_GLOBAL, {{ CMDSENTENCE( tmp_str_cmd ) }}, CMDF_HIDEVARS|CMDF_RETURNONERROR,
    "Help/guide chat channel" },


    { 9, "sendchat", CMD_SEND_SERVER_MSG, {{CMDSTR(player_name_cmd)},{CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS,
    "Send chat message across mapservers." },

    { 0, "telllast", CMD_CHAT_TELL_LAST,  {{ CMDSENTENCE( tmp_str_cmd ) }}, CMDF_HIDEVARS|CMDF_RETURNONERROR|CMDF_HIDEPRINT,
    "Send tell to last person you sent tell to." },

    { 0, "tl", CMD_CHAT_TELL_LAST,  {{ CMDSENTENCE( tmp_str_cmd ) }}, CMDF_HIDEVARS|CMDF_RETURNONERROR|CMDF_HIDEPRINT,
    "Send tell to last person you sent tell to." },
    // emotes
    //-------------------------------------------------------------------
    { 0, "e", CMD_CHAT_EMOTE, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Emotes a text string." },
    { 0, "me", CMD_CHAT_EMOTE, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Emotes a text string." },
    { 0, "em", CMD_CHAT_EMOTE, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Emotes a text string." },
    { 0, "emote", CMD_CHAT_EMOTE, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Emotes a text string." },

    // costume change emotes
    //-------------------------------------------------------------------
    { 0, "cc_e", CMD_CHAT_EMOTE_COSTUME_CHANGE, {{ PARSETYPE_S32, &tmp_int }, { CMDSTR( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Uses an emote to change costumes." },
    { 0, "cc_emote", CMD_CHAT_EMOTE_COSTUME_CHANGE, {{ PARSETYPE_S32, &tmp_int }, { CMDSTR( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Uses an emote to change costumes." },

    // friends
    //-------------------------------------------------------------------

    { 0, "friend", CMD_ADD_FRIEND, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Add player to friend list." },
    { 0, "f", CMD_CHAT_FRIEND, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Talk to friends channel." },

    { 0, "estrange", CMD_REMOVE_FRIEND, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Remove player from friend list." },
    { 0, "unfriend", CMD_REMOVE_FRIEND, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
    "Remove player from friend list." },

    // leveling pacts
    //-------------------------------------------------------------------
    { 9, "debug_enable_levelingpack", CMD_DEBUG_LEVELINGPACT_ENABLE, {{ PARSETYPE_S32, &tmp_int    }}, CMDF_HIDEVARS|CMDF_RETURNONERROR,
        "Enable leveling pacts" },
    { 0, "levelingpact",    CMD_LEVELINGPACT_INVITE, {{ CMDSENTENCE( player_name_cmd )}}, CMDF_HIDEVARS|CMDF_RETURNONERROR,
    "Invite player to join your leveling pact." },
    { 0, "levelingpact_accept", CMD_LEVELINGPACT_ACCEPT, {{ PARSETYPE_S32, &tmp_int }}, CMDF_HIDEPRINT|CMDF_HIDEVARS|CMDF_RETURNONERROR,
    "Accept an invitation into a leveling pact." },
    { 0, "levelingpact_decline", CMD_LEVELINGPACT_DECLINE, {{ PARSETYPE_S32, &tmp_int },{ CMDSTR( tmp_str_cmd )}}, CMDF_HIDEPRINT|CMDF_HIDEVARS|CMDF_RETURNONERROR,
    "Decline an invitation into a leveling pact." },
    { 0, "unlevelingpact_real",    CMD_LEVELINGPACT_QUIT, {{ 0 }}, CMDF_HIDEPRINT|CMDF_HIDEVARS|CMDF_RETURNONERROR,
    "Leave your leveling pact." },

    { 3, "levelingpact_add_no_xp",    CMD_LEVELINGPACT_CSR_ADD_MEMBER, {{ CMDSTR( player_name_cmd )}, {CMDSTR(tmp_str_cmd)}}, CMDF_RETURNONERROR,
    "Forcibly join two players in a leveling pact by name." },
    { 3, "levelingpact_add",    CMD_LEVELINGPACT_CSR_ADD_MEMBER_XP, {{ CMDSTR( player_name_cmd )}, {CMDSTR(tmp_str_cmd)}, {PARSETYPE_S32, &tmp_int}}, CMDF_RETURNONERROR,
    "Forcibly join two players in a leveling pact by name." },
    { 4, "levelingpact_set_experience", CMD_LEVELINGPACT_CSR_SET_EXPERIENCE, {{ CMDSTR( player_name_cmd )},{ PARSETYPE_S32, &tmp_int }}, CMDF_RETURNONERROR,
    "Set the total experience that a leveling pact shares." },
    { 4, "levelingpact_set_influence",    CMD_LEVELINGPACT_CSR_SET_INFLUENCE, {{ CMDSTR( player_name_cmd )},{ PARSETYPE_S32, &tmp_int }}, CMDF_RETURNONERROR,
    "Set the total influence that a leveling pact shares." },
    { 4, "levelingpact_info",    CMD_LEVELINGPACT_CSR_INFO, {{ CMDSTR( player_name_cmd )}}, CMDF_RETURNONERROR,
    "Set the total influence that a leveling pact shares." },

    // team
    //-------------------------------------------------------------------

    { 0, "invite", CMD_TEAM_INVITE, {{ CMDSENTENCE( player_name_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Invite player to join team." },
    { 0, "i", CMD_TEAM_INVITE, {{ CMDSENTENCE( player_name_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Invite player to join team." },
    { 9, "invite_long", CMD_TEAM_INVITE_LONG, {{ CMDSTR( player_name_cmd )},{ CMDSTR( tmp_str_cmd )}, { PARSETYPE_S32, &tmp_dbid }, { CMDSTR( tmp_taskdesc )},{ PARSETYPE_S32, &tmp_int },{ PARSETYPE_S32, &tmp_int2 },{ PARSETYPE_S32, &tmp_int3 },{ PARSETYPE_S32, &tmp_int4 },{ PARSETYPE_S32, &tmp_int5 }}, CMDF_HIDEVARS,
        "Invite player to join team" },

    { 0, "team_accept", CMD_TEAM_INVITE_ACCEPT, {{ CMDSTR( player_name_cmd )},{ PARSETYPE_S32, &tmp_int },{ PARSETYPE_S32, &tmp_dbid },{ CMDSTR( tmp_str_cmd )},{ PARSETYPE_S32, &tmp_int2 }}, CMDF_HIDEPRINT|CMDF_HIDEVARS,
        "Recieves a team accept" },
    { 9, "team_accept_relay", CMD_TEAM_ACCEPT_RELAY, {{ PARSETYPE_S32, &tmp_dbid },{ PARSETYPE_S32, &tmp_int },{ PARSETYPE_S32, &tmp_int2 },{ PARSETYPE_S32, &tmp_int3 },{ PARSETYPE_S32, &tmp_int4 }}, CMDF_HIDEVARS,
        "Asks a player to add himself (player_id, team_id, invited_by, inviter_pvp)" },
    { 9, "team_accept_offer_relay", CMD_TEAM_ACCEPT_OFFER_RELAY, {{ PARSETYPE_S32, &tmp_dbid },{ PARSETYPE_S32, &tmp_int }, {PARSETYPE_S32, &tmp_int2}, {PARSETYPE_S32, &tmp_int3}, {PARSETYPE_S32, &tmp_int4}}, CMDF_HIDEVARS,
        "Asks a player to add himself (player_id, team_id, invited_by, inviter_pvp, addEvenIfNotLeader)" },
    { 0, "team_decline", CMD_TEAM_INVITE_DECLINE, {{ CMDSTR( player_name_cmd )},{ PARSETYPE_S32, &tmp_int },{ CMDSTR( tmp_str_cmd )}}, CMDF_HIDEPRINT|CMDF_HIDEVARS,
        "Recieves a team decline" },

    { 0, "kick", CMD_TEAM_KICK, {{ CMDSENTENCE( player_name_cmd )}, }, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Kick player from team." },
    { 0, "k", CMD_TEAM_KICK, {{ CMDSENTENCE( player_name_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Kick player from team." },
    { 0, "team_kick_internal", CMD_TEAM_KICK_INTERNAL, {{ PARSETYPE_S32, &tmp_int },{ CMDSENTENCE( player_name_cmd )} }, CMDF_HIDEPRINT|CMDF_HIDEVARS,
        "Kicks player without warnings" },
    { 9, "team_kick_relay", CMD_TEAM_KICK_RELAY, {{ PARSETYPE_S32, &tmp_int },{ PARSETYPE_S32, &tmp_int2 }}, CMDF_HIDEVARS,
        "Asks player to kick himself (kicked_id, kicked_by)" },
    { 9, "team_map_relay", CMD_TEAM_MAP_RELAY, {{ PARSETYPE_S32, &tmp_int }}, CMDF_HIDEVARS,
        "Asks player to send map update" },


    { 0, "leaveTeam", CMD_TEAM_QUIT, {{ 0 }}, 0,
        "Leave your current team and league." },

    { 0, "team_quit_internal", CMD_TEAM_QUIT_INTERNAL, {{0 }, }, CMDF_HIDEPRINT,
        "Quits player without warnings" },
    { 9, "team_quit_relay", CMD_TEAM_QUIT_RELAY, {{ PARSETYPE_S32, &tmp_int }}, CMDF_HIDEVARS,
        "Relays a team_quit_internal command" },
    { 9, "tf_quit_relay", CMD_TF_QUIT_RELAY, {{ PARSETYPE_S32, &tmp_int }}, CMDF_HIDEVARS,
        "Relays a task force quit internal command - synonymous with team_quit_relay" },

    { 0, "lfgtoggle", CMD_TEAM_LFG, {{ 0 }}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Toggle looking for group status." },

    { 0, "lfgset", CMD_TEAM_LFG_SET, {{ PARSETYPE_S32, &tmp_int }}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Set looking for group status" },

    { 0, "buffs", CMD_TEAM_BUFF_DISP, {{ PARSETYPE_S32, &tmp_int  }}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Toggle team buff display." },

    { 0, "makeleader", CMD_TEAM_CHANGE_LEADER, {{ CMDSENTENCE( player_name_cmd ) }}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Change the team leader." },

    { 0, "ml", CMD_TEAM_CHANGE_LEADER, {{ CMDSENTENCE( player_name_cmd ) }}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Change the team leader." },
    { 9, "makeleader_relay", CMD_TEAM_CHANGE_LEADER_RELAY, {{ PARSETYPE_S32, &tmp_dbid }, { CMDSENTENCE( player_name_cmd ) }}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Change the team leader." },

    { 0, "teamMoveToLeague", CMD_TEAM_MOVE_CREATE_LEAGUE, {{ CMDSENTENCE( player_name_cmd )    }}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Create a new league." },
    { 0, "tmtl", CMD_TEAM_MOVE_CREATE_LEAGUE, {{ CMDSENTENCE( player_name_cmd )    }}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Create a new league." },
    // supergroup
    //------------------------------------------------------------------

    { 0, "sginvite", CMD_SUPERGROUP_INVITE, {{ CMDSENTENCE( player_name_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Invite player to join supergroup." },
    { 0, "sgi", CMD_SUPERGROUP_INVITE, {{ CMDSENTENCE( player_name_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Invite player to join supergroup." },
    { 9, "sginvite_long", CMD_SUPERGROUP_INVITE_LONG, {{ CMDSTR( player_name_cmd )},{ CMDSTR( tmp_str_cmd )}, { PARSETYPE_S32, &tmp_dbid },{ PARSETYPE_S32, &tmp_int }}, 0,
        "Invite player to join supergroup" },
    { 0, "altinvite", CMD_SUPERGROUP_INVITE_ALT, {{ CMDSENTENCE( player_name_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
		"Joins a different character on your account to your supergroup." },

    { 0, "sg_accept", CMD_SUPERGROUP_INVITE_ACCEPT, {{ CMDSTR( player_name_cmd )},{ PARSETYPE_S32, &tmp_int },{ PARSETYPE_S32, &tmp_dbid },{ CMDSTR( tmp_str_cmd )}}, CMDF_HIDEPRINT,
        "Recieves a supergroup accept" },
    { 9, "sg_accept_relay", CMD_SUPERGROUP_ACCEPT_RELAY, {{ PARSETYPE_S32, &tmp_int },{ PARSETYPE_S32, &tmp_int2 },{ PARSETYPE_S32, &tmp_int3 }}, 0,
        "Asks a player to add themselves to supergroup (player_id, sg_id, inviter_id)" },
	{ 9, "sg_alt_relay", CMD_SUPERGROUP_ALT_RELAY, {{ PARSETYPE_S32, &tmp_int },{ PARSETYPE_S32, &tmp_int2 },{ PARSETYPE_S32, &tmp_int3 },{ PARSETYPE_S32, &tmp_int4 },{ PARSETYPE_S32, &tmp_int5 }}, 0,
		"Joins a player to a supergroup if they are on the same account as the inviter (player_id, sg_id, sg_type, inviter_id, inviter_authid)" },
    { 0, "sg_decline", CMD_SUPERGROUP_INVITE_DECLINE, {{ CMDSTR( player_name_cmd )},{ PARSETYPE_S32, &tmp_int },{ CMDSTR( tmp_str_cmd )}}, CMDF_HIDEPRINT,
        "Recieves a supergroup decline" },

    { 0, "sg_kick_yes", CMD_SUPERGROUP_KICK, {{ CMDSENTENCE( player_name_cmd )}, }, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Kick player from supergroup." },
    { 9, "sg_kick_relay", CMD_SUPERGROUP_KICK_RELAY, {{ PARSETYPE_S32, &tmp_int },{ PARSETYPE_S32, &tmp_int2 }}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Kick player from join supergroup" },

    { 0, "sgleave", CMD_SUPERGROUP_QUIT, {{ 0 }}, 0,
        "Leave your current supergroup." },

    { 0, "sgstats", CMD_SUPERGROUP_GET_STATS, {{0}}, 0,
        "Display supergroup info in chat window." },

    { 9, "sgstatsrelay", CMD_SUPERGROUP_GET_STATS_RELAY, {{ PARSETYPE_S32, &tmp_int }}, 0,
        "force given db_id to reload stats" },

    { 0, "promote", CMD_SUPERGROUP_PROMOTE, {{ CMDSENTENCE( player_name_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Promote supergroup member one rank."    },

    { 0, "demote", CMD_SUPERGROUP_DEMOTE, {{ CMDSENTENCE( player_name_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Demote supergroup member one rank."    },

    { 9, "promote_long", CMD_SUPERGROUP_PROMOTE_LONG, {{ PARSETYPE_S32, &tmp_int },{ PARSETYPE_S32, &tmp_int2 },{ PARSETYPE_S32, &tmp_int3 },{ PARSETYPE_S32, &tmp_dbid },{ CMDSTR( player_name_cmd )}}, 0,
        "internal promote command"    },

    { 9, "csr_promote_long", CMD_SUPERGROUP_CSR_PROMOTE_LONG, {{ PARSETYPE_S32, &tmp_int },{ PARSETYPE_S32, &tmp_int2 },{ PARSETYPE_S32, &tmp_int3 },{ PARSETYPE_S32, &tmp_dbid },{ CMDSTR( player_name_cmd )}}, 0,
        "internal promote command"    },

    { 9, "sg_initiate", CMD_SUPERGROUP_SET_DEFAULTS, {{ CMDSENTENCE( player_name_cmd )}}, 0,
        "internal initiate command"    },

    { 9, "sgcreate", CMD_SUPERGROUP_DEBUG_CREATE, {{ CMDSENTENCE( tmp_str_cmd ) }}, 0,
        "Create supergroup for development testing" },

    { 2, "sgjoin", CMD_SUPERGROUP_DEBUG_JOIN, {{ CMDSENTENCE( tmp_str_cmd ) }}, 0,
        "join a supergroup for development testing and csr" },

    { 9, "sgreg",    CMD_SUPERGROUP_REGISTRAR, {{0}}, 0,
        "Simulate click on the supergroup registar" },

    { 0, "nameLeader", CMD_SUPERGROUP_RENAME_LEADER, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Renames the 'Leader' supergroup rank."    },

    { 0, "nameOverlord", CMD_SUPERGROUP_RENAME_LEADER, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Renames the 'Overlord' supergroup rank."    },

    { 0, "nameCommander", CMD_SUPERGROUP_RENAME_COMMANDER, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Renames the 'Commander' supergroup rank."    },

    { 0, "nameRingleader", CMD_SUPERGROUP_RENAME_COMMANDER, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Renames the 'Ringleader' supergroup rank."    },

    { 0, "nameCaptain", CMD_SUPERGROUP_RENAME_CAPTAIN, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Renames the 'Captain' supergroup rank."    },

    { 0, "nameTaskmaster", CMD_SUPERGROUP_RENAME_CAPTAIN, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Renames the 'TaskMaster' supergroup rank."    },

    { 0, "nameLieutenant", CMD_SUPERGROUP_RENAME_LIEUTENANT, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Renames the 'Lieutenant' supergroup rank."    },

    { 0, "nameEnforcer", CMD_SUPERGROUP_RENAME_LIEUTENANT, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Renames the 'Enforcer' supergroup rank."    },

    { 0, "nameMember", CMD_SUPERGROUP_RENAME_MEMBER, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Renames the 'Member' supergroup rank."    },

    { 0, "nameFlunky", CMD_SUPERGROUP_RENAME_MEMBER, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Renames the 'Flunky' supergroup rank."    },

    { 0, "sgSetMOTD", CMD_SUPERGROUP_MOTD_SET, {{ CMDSENTENCE( tmp_str_cmd )}}, 0,
        "Sets supergroup MOTD."    },

    { 0, "sgSetMotto", CMD_SUPERGROUP_MOTTO_SET, {{ CMDSENTENCE( tmp_str_cmd )}}, 0,
        "Sets supergroup motto."    },

    { 0, "sgSetDescription", CMD_SUPERGROUP_DESCRIPTION_SET, {{ CMDSENTENCE( tmp_str_cmd )}}, 0,
        "Sets supergroup description."    },

//    SCMD_ARENA_CAMERA,
//    SCMD_ARENA_UNCAMERA,
    { 0, "sgSetDemoteTimeout", CMD_SUPERGROUP_DEMOTETIMEOUT_SET, {{ PARSETYPE_S32, &tmp_int }}, 0,
        "Sets supergroup demote timeout."    },

    { 0, "sgmode", CMD_SUPERGROUP_MODE, {{ 0 }}, 0,
        "Toggle supergroup mode."    },

    { 0, "sgmodeset", CMD_SUPERGROUP_MODE_SET, {{ PARSETYPE_S32, &tmp_int }}, 0,
        "Setsupergroup mode."    },

    { 1, "sgwho", CMD_SUPERGROUP_WHO, {{0}}, 0,
        "see who is in a supergroup"    },

    { 1, "sgleader", CMD_SUPERGROUP_WHO_LEADER, {{0}}, 0,
        "find leader of a supergroup."    },

    { 9, "sgrefreshrelay", CMD_SUPERGROUP_GET_REFRESH_RELAY, {{ PARSETYPE_S32, &tmp_int }}, 0,
        "force given db_id to reload supergroup" },

    { 3, "sgrankname", CMD_SUPERGROUP_RENAME_RANK, {{ PARSETYPE_S32, &tmp_int }, {CMDSENTENCE(tmp_str_cmd)}}, 0,
        "Change rank #<int> name to <str>" },
    
    { 1, "sgrankprint", CMD_SUPERGROUP_RANK_PRINT, {{0}}, 0,
        "Print rank information" },

    // alliance
    //-----------------------------------------------------------------
    { 0, "coalition_invite", CMD_ALLIANCE_INVITE, {{ CMDSENTENCE( player_name_cmd )}}, CMDF_HIDEVARS,
        "Invite player's supergroup to join coalition." },
    { 0, "ci", CMD_ALLIANCE_INVITE, {{ CMDSENTENCE( player_name_cmd )}}, CMDF_HIDEVARS,
        "Invite player's supergroup to join coalition." },
    { 9, "coalition_invite_relay", CMD_ALLIANCE_INVITE_RELAY, {{ CMDSTR( player_name_cmd )},{ CMDSTR( tmp_str_cmd )}, { PARSETYPE_S32, &tmp_dbid }, { PARSETYPE_S32, &tmp_int }, { CMDSTR( tmp_str2_cmd) }, { PARSETYPE_S32, &tmp_int2 }}, CMDF_HIDEPRINT,
        "Invite player's supergroup to join coalition." },

    { 0, "coalition_accept", CMD_ALLIANCE_ACCEPT, {{ CMDSTR( player_name_cmd )}, { PARSETYPE_S32, &tmp_dbid }, { PARSETYPE_S32, &tmp_int }}, CMDF_HIDEPRINT,
        "Receives a coalition accept" },
    { 9, "coalition_accept_relay", CMD_ALLIANCE_ACCEPT_RELAY, {{ CMDSTR( player_name_cmd )},{ CMDSTR( tmp_str_cmd )}, { PARSETYPE_S32, &tmp_dbid }, { PARSETYPE_S32, &tmp_int }, { PARSETYPE_S32, &tmp_int2 }}, CMDF_HIDEPRINT,
        "Receives a coalition accept" },

    { 0, "coalition_decline", CMD_ALLIANCE_DECLINE, {{ CMDSTR( player_name_cmd )}, { PARSETYPE_S32, &tmp_dbid }, { CMDSTR(tmp_str_cmd) }}, CMDF_HIDEPRINT,
        "Receives a coalition decline" },

    { 0, "coalition_cancel", CMD_ALLIANCE_CANCEL, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS,
        "Cancel coalition with a supergroup." },

    { 0, "coalition_sg_mintalkrank", CMD_ALLIANCE_SG_MINTALKRANK, {{PARSETYPE_S32, &tmp_int}}, CMDF_HIDEVARS,
        "Set the minimum rank of members of your Supergroup who can use coalition chat." },

    { 0, "coalition_mintalkrank", CMD_ALLIANCE_MINTALKRANK, {{PARSETYPE_S32, &tmp_dbid}, {PARSETYPE_S32, &tmp_int}}, CMDF_HIDEVARS,
        "Set the minimum rank of members of a coalition Supergroup who your Supergroup can hear." },

    { 0, "coalition_nosend", CMD_ALLIANCE_NOSEND, {{PARSETYPE_S32, &tmp_dbid}, {PARSETYPE_S32, &tmp_int}}, CMDF_HIDEVARS,
        "Stop your Supergroup from sending coalition chat to an ally Supergroup." },

    //-----------------------------------------------------------------
    { 0, "trade", CMD_TRADE_INVITE, {{ CMDSENTENCE( player_name_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Invite player to trade." },
    { 0, "trade_accept", CMD_TRADE_INVITE_ACCEPT, {{ CMDSTR( player_name_cmd )},{ PARSETYPE_S32, &tmp_int },{ PARSETYPE_S32, &tmp_dbid },{ CMDSTR( tmp_str_cmd )}}, CMDF_HIDEPRINT,
        "Recieves a trade accept" },
    { 0, "trade_decline", CMD_TRADE_INVITE_DECLINE, {{ CMDSTR( player_name_cmd )},{ PARSETYPE_S32, &tmp_int },{ CMDSTR( tmp_str_cmd )}}, CMDF_HIDEPRINT,
        "Recieves a trade decline" },

    { 0, "costume_change", CMD_COSTUME_CHANGE, {{ PARSETYPE_S32, &tmp_int }}, 0,
        "Change current costume." },
    { 0, "cc", CMD_COSTUME_CHANGE, {{ PARSETYPE_S32, &tmp_int }}, 0,
        "Change current costume." },
    { 2, "costume_add_slot", CMD_COSTUME_ADDSLOT, {{0}}, 0, 
        "Get another costume slot" },
    
    //-----------------------------------------------------------------

    { 0, "hideset", CMD_HIDE_SET, {{ PARSETYPE_S32, &tmp_int }}, CMDF_HIDEVARS | CMDF_RETURNONERROR | CMDF_HIDEPRINT,
        "Set hide options bitfield." },

    { 0, "search", CMD_SEARCH, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Find a player." },
    { 0, "sea", CMD_SEARCH, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Find a player." },
    { 0, "findmember", CMD_SEARCH, {{ CMDSENTENCE( tmp_str_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Find a player." },

    { 0, "set_title", CMD_SET_BADGE_TITLE, {{ PARSETYPE_S32, &tmp_int  }}, CMDF_HIDEVARS | CMDF_RETURNONERROR | CMDF_HIDEPRINT,
        "Set badge title." },

    { 0, "set_title_id", CMD_SET_BADGE_TITLE_ID, {{ PARSETYPE_S32, &tmp_int }}, CMDF_HIDEVARS | CMDF_RETURNONERROR | CMDF_HIDEPRINT,
        "Set badge title (uses badge ID)." },

    { 9, "badge_debug", CMD_SET_BADGE_DEBUG_LEVEL, {{ PARSETYPE_S32, &tmp_int }}, CMDF_HIDEVARS | CMDF_HIDEPRINT,
        "Set badge debug level (0 = off, 1 = show known, 2 = show visible).", },

    { 0, "get_comment", CMD_SET_COMMENT, {{CMDSENTENCE(tmp_str_cmd)}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Get search comment." },
    { 0, "comment", CMD_SET_COMMENT, {{CMDSENTENCE(tmp_str_cmd)}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Set search comment." },

    //-----------------------------------------------------------------

    { 0, "arenainvite", CMD_ARENA_INVITE, {{ CMDSENTENCE( player_name_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Invite player to join your arena event." },
    { 0, "ai", CMD_ARENA_INVITE, {{ CMDSENTENCE( player_name_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Invite player to join your arena event." },
    { 9, "arenainvite_long", CMD_ARENA_INVITE_LONG, {{ CMDSTR( player_name_cmd )},{ CMDSTR( tmp_str_cmd )}, { PARSETYPE_S32, &tmp_dbid }, { PARSETYPE_S32, &tmp_int }, { PARSETYPE_S32, &tmp_int2 }}, 0,
        "Invite player to join arena event" },

    { 0, "arena_accept", CMD_ARENA_INVITE_ACCEPT, {{ PARSETYPE_S32, &tmp_dbid },{ PARSETYPE_S32, &tmp_int },{ PARSETYPE_S32, &tmp_int2 },}, CMDF_HIDEPRINT,
        "Recieves a arena accept" },
    //{ 9, "arena_accept_relay", CMD_ARENA_ACCEPT_RELAY, {{ PARSETYPE_S32, &tmp_int },{ PARSETYPE_S32, &tmp_int2 },{ PARSETYPE_S32, &tmp_int3 }}, 0,
    //    "Asks a player to add themselves to supergroup (player_id, sg_id, inviter_id)" },
    { 0, "arena_decline", CMD_ARENA_INVITE_DECLINE, {{ PARSETYPE_S32, &tmp_dbid }}, CMDF_HIDEPRINT,
        "Recieves a arena decline" },

    // ---------------------------------------------------------------
    { 0, "sgraid_invite", CMD_RAID_INVITE, {{ CMDSENTENCE( player_name_cmd )}}, CMDF_HIDEVARS,
        "Invite player's supergroup to an instant raid." },
    { 0, "raid_invite", CMD_RAID_INVITE, {{ CMDSENTENCE( player_name_cmd )}}, CMDF_HIDEVARS,
        "Invite player's supergroup to an instant raid." },
    { 0, "league_invite", CMD_LEAGUE_INVITE, {{ CMDSENTENCE( player_name_cmd)}}, CMDF_HIDEVARS,
        "Invite player to join league." },
    { 0, "li", CMD_LEAGUE_INVITE, {{ CMDSENTENCE( player_name_cmd)}}, CMDF_HIDEVARS,
        "Invite player to join league." },
    { 9, "leagueinvite_long", CMD_LEAGUE_INVITE_LONG,    {{ CMDSTR( player_name_cmd )},{ CMDSTR( tmp_str_cmd )}, { PARSETYPE_S32, &tmp_dbid }, { CMDSTR( tmp_taskdesc )}, { PARSETYPE_S32, &tmp_int }, { PARSETYPE_S32, &tmp_int2 },{ PARSETYPE_S32, &tmp_int3 },{ PARSETYPE_S32, &tmp_int4 },{ PARSETYPE_S32, &tmp_int5 }, {PARSETYPE_S32, &tmp_int6}, {PARSETYPE_S32, &tmp_int7}}, CMDF_HIDEVARS,
        "Invite player to join league" },
    { 0, "league_accept", CMD_LEAGUE_ACCEPT, {{ CMDSTR( player_name_cmd )},{ PARSETYPE_S32, &tmp_int },{ PARSETYPE_S32, &tmp_dbid },{ CMDSTR( tmp_str_cmd )},{ PARSETYPE_S32, &tmp_int2 }}, CMDF_HIDEPRINT|CMDF_HIDEVARS,
        "Receives a league accept" },
    { 9, "league_accept_relay", CMD_LEAGUE_ACCEPT_RELAY, {{PARSETYPE_S32, &tmp_int}, {PARSETYPE_S32, &tmp_int2}, {CMDSTR(tmp_str_cmd)}, {PARSETYPE_S32, &tmp_int3}, {PARSETYPE_S32, &tmp_int4}, {PARSETYPE_S32, &tmp_int5}}, 0,
        "Inviter relay" },
    { 9, "league_accept_offer_relay", CMD_LEAGUE_ACCEPT_OFFER_RELAY, {{ PARSETYPE_S32, &tmp_dbid },{ PARSETYPE_S32, &tmp_int }, {PARSETYPE_S32, &tmp_int2}, {PARSETYPE_S32, &tmp_int3}, {PARSETYPE_S32, &tmp_int4}}, CMDF_HIDEVARS,
        "Asks a player to add himself (player_id, team_id, invited_by, inviter_pvp)" },

    { 0, "league_decline", CMD_LEAGUE_DECLINE, {{ CMDSTR( player_name_cmd )},{ PARSETYPE_S32, &tmp_int },{ CMDSTR( tmp_str_cmd )}}, CMDF_HIDEPRINT|CMDF_HIDEVARS,
        "Receives a league decline" },

    { 0, "leaveLeague", CMD_TEAM_QUIT, {{ 0 }} , 0,
        "Leave your current team and league." },        //    intentionally using the CMD_TEAM_QUIT
    { 9, "leaveLeagueRelay", CMD_LEAGUE_QUIT_RELAY, {{ PARSETYPE_S32, &tmp_int}} , 0,
        "Leave your current league." },
    { 0, "leagueWithdrawTeam", CMD_LEAGUE_TEAM_WITHDRAW, {{ 0 }}, 0,
        "Withdraw your team from current league." },
    { 9, "leagueWithdrawTeamRelay", CMD_LEAGUE_TEAM_WITHDRAW_RELAY, {{ PARSETYPE_S32, &tmp_int }}, 0,
        "Withdraw your team from current league. Internal command version" },
    { 0, "leagueToggleTeamLock",    CMD_LEAGUE_TEAM_SWAP_LOCK, {{ 0 }}, 0,
        "Lock your team from league swap"    },
    { 0, "league_kick", CMD_LEAGUE_KICK, {{ CMDSENTENCE( player_name_cmd )}, }, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Kick player from league." },
    { 0, "lk", CMD_LEAGUE_KICK, {{ CMDSENTENCE( player_name_cmd )}}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Kick player from league." },
    { 0, "league_kick_internal", CMD_LEAGUE_KICK_INTERNAL, {{ PARSETYPE_S32, &tmp_int }, {PARSETYPE_S32, &tmp_int2},{ CMDSENTENCE( player_name_cmd )} }, CMDF_HIDEPRINT|CMDF_HIDEVARS,
        "Kicks player without warnings" },
    { 9, "league_kick_relay", CMD_LEAGUE_KICK_RELAY, {{ PARSETYPE_S32, &tmp_int },{ PARSETYPE_S32, &tmp_int2 }}, CMDF_HIDEVARS,
        "Asks player to kick himself (kicked_id, kicked_by)" },
    { 9, "league_map_relay", CMD_LEAGUE_MAP_RELAY, {{ PARSETYPE_S32, &tmp_int }}, CMDF_HIDEVARS,
        "Asks player to send map update" },
    { 0, "league_make_leader", CMD_LEAGUE_CHANGE_LEADER, {{ CMDSENTENCE( player_name_cmd ) }}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Change the team leader." },
    { 0, "lml", CMD_LEAGUE_CHANGE_LEADER, {{ CMDSENTENCE( player_name_cmd ) }}, CMDF_HIDEVARS | CMDF_RETURNONERROR,
        "Change the team leader." },
    { 0, "league_teamswap", CMD_LEAGUE_TEAM_SWAP_MEMBERS, {{ PARSETYPE_S32, &tmp_int }, {PARSETYPE_S32, &tmp_int2}}, CMDF_HIDEVARS,
        "Swap the teams of 2 players" },
    { 0, "league_teammove", CMD_LEAGUE_TEAM_MOVE_MEMBER, {{ PARSETYPE_S32, &tmp_int}, { PARSETYPE_S32, &tmp_int2}, {PARSETYPE_S32, &tmp_int3}}, CMDF_HIDEVARS,
        "Move selected player to selected team"    },
    { 9, "league_remove_accept_block", CMD_LEAGUE_REMOVE_ACCEPT_BLOCK, {{ PARSETYPE_S32, &tmp_int }} , 0,
        "Remove the league block" },
    { 9, "turnstile_player_left_league", CMD_TURNSTILE_LEAGUE_QUIT, {{ PARSETYPE_S32, &tmp_int}}, CMDF_HIDEVARS,
        "Player has left league and the turnstile needs to know"    },
    { 9, "tut_invite", CMD_TURNSTILE_INVITE_PLAYER_TO_EVENT, {{ CMDSENTENCE( player_name_cmd ) }}, CMDF_HIDEVARS,
        "Invite player to the leaders instance"    },
    { 9, "turnstile_invite_player_relay", CMD_TURNSTILE_INVITE_PLAYER_TO_EVENT_RELAY, {{ PARSETYPE_S32, &tmp_dbid}, {PARSETYPE_S32, &tmp_int}, {PARSETYPE_S32, &tmp_int2}, {PARSETYPE_S32, &tmp_int3}, { CMDSENTENCE( player_name_cmd ) }}, CMDF_HIDEVARS,
        "Relay invite player to the leaders instance"    },
    { 0, "turnstile_invite_player_accept", CMD_TURNSTILE_INVITE_PLAYER_TO_EVENT_ACCEPT, {{ PARSETYPE_S32, &tmp_dbid}, { PARSETYPE_S32, &tmp_int}, { PARSETYPE_S32, &tmp_int2}}, CMDF_HIDEVARS,
        "Invite player to the leaders instance"    },
    { 9, "turnstile_invite_player_accept_relay", CMD_TURNSTILE_INVITE_PLAYER_TO_EVENT_ACCEPT_RELAY, {{PARSETYPE_S32, &tmp_dbid}, {PARSETYPE_S32, &tmp_int}}, CMDF_HIDEVARS,
        "Invite player to the leaders instance"    },
    { 9, "turnstile_join_specific_mission_instance", CMD_TURNSTILE_JOIN_SPECIFIC_MISSION_INSTANCE, {{PARSETYPE_S32, &tmp_dbid}, {PARSETYPE_S32, &tmp_int}, {PARSETYPE_S32, &tmp_int2}}, CMDF_HIDEVARS,
        "Player is joining a specific mission instance"    },
    { 9, "raid_invite_relay", CMD_RAID_INVITE_RELAY, {{ CMDSTR( player_name_cmd )},{ CMDSTR( tmp_str_cmd )}, { PARSETYPE_S32, &tmp_dbid }, { PARSETYPE_S32, &tmp_int }, { CMDSTR( tmp_str2_cmd) }, { PARSETYPE_S32, &tmp_int2 }}, CMDF_HIDEPRINT,
        "Invite player's supergroup to an instant raid. (Internal)" },

    { 0, "raid_accept", CMD_RAID_ACCEPT, {{ CMDSTR( player_name_cmd )}, { PARSETYPE_S32, &tmp_dbid }, { PARSETYPE_S32, &tmp_int }}, CMDF_HIDEPRINT,
        "Accept an offer to have an instant raid" },

    { 0, "raid_decline", CMD_RAID_DECLINE, {{ CMDSTR( player_name_cmd )}, { PARSETYPE_S32, &tmp_dbid }, { PARSETYPE_S32, &tmp_int }}, CMDF_HIDEPRINT,
        "Decline an offer to have an instant raid" },
    { 0, "clearRewardChoice", CMD_REWARDCHOICE_CLEAR, {0}, 0,
        "Choose no item in your current reward choice" },
    { 0, "tut_votekick", CMD_ENDGAMERAID_VOTE_KICK, {{ CMDSENTENCE( player_name_cmd ) }}, CMDF_HIDEVARS,
        "Start a vote kick request" },
    { 0, "tut_votekick_opinion", CMD_ENDGAMERAID_VOTE_KICK_OPINION, {{ PARSETYPE_S32, &tmp_int }}, CMDF_HIDEVARS,
        "Vote kick opinion" },
    // TO DO
    /*
    /where
    /time
    */

    { 0},
};

// returns 0 if offline
//
int player_online( char *name )
{
    return dbPlayerOnWhichMap(name) >= 0;
}

static int canInviteeAndInviterPlayTogether(int invitee_playerTypeByLocation, int invitee_praetorianProgress, int inviter_playerTypeByLocation, int inviter_praetorianProgress)
{
    int retval = 1;

    // If multiversal play isn't allowed,
    // then if their Praetorian-ness isn't the same, disallow the invite
    // emulates ENT_IS_IN_PRAETORIA(e)
    if (!server_state.allowPrimalAndPraetoriansToTeamUp &&
        ((invitee_praetorianProgress == kPraetorianProgress_Tutorial || invitee_praetorianProgress == kPraetorianProgress_Praetoria)
           != (inviter_praetorianProgress == kPraetorianProgress_Tutorial || inviter_praetorianProgress == kPraetorianProgress_Praetoria)))
    {
        retval = 0;
    }

    if (!server_state.allowHeroAndVillainPlayerTypesToTeamUp &&
        invitee_playerTypeByLocation != inviter_playerTypeByLocation) // this line allows Going Rogue cross-grays to team up
    {
        retval = 0;
    }
    
    return retval;
}

// common to teams and leagues
char *common_CheckInviterConditions(Entity *inviter, char *invitee_name, int *disallowedBitfield)
{
    int online, sgroup_id = 0;
    Entity *invitee; // NULL if not on this map!
    int invitee_id = dbPlayerIdFromName(invitee_name);
    int invitee_playerType = dbPlayerTypeFromId(invitee_id);
    OnlinePlayerInfo* invitee_opi = dbGetOnlinePlayerInfo(invitee_id);
    const MapSpec *spec = MapSpecFromMapId(inviter->static_map_id);
    int interlockCheck;

    invitee = entFromDbId(invitee_id);
    online = player_online(invitee_name);

    interlockCheck = turnstileMapserver_teamInterlockCheck(inviter, 1);
    if (interlockCheck == 2)
    {
        return localizedPrintf(inviter, "CantDoThatWhileInQueue");
    }
    if (interlockCheck != 0)
    {
        return localizedPrintf(inviter, "CantDoThatAtTheMoment");
    }

    if (OnArenaMap())
    {
        // Don't allow people to invite others when they're in the arena.
        return localizedPrintf(inviter, "CannotInviteToTeamWhileInArenaMessage");
    }

    if (db_state.base_map_id == 41) // Destroyed Galaxy City
    {
        return localizedPrintf(inviter, "CannotInviteToTeamWhileInTutorialMessage");
    }

    if (invitee_id < 0 || // Character doesn't exist or
        (!invitee && !online) || // He's not online and he's not on this mapserver or
        (invitee && invitee->logout_timer) || // He's on this mapserver, and online, but he's currently marked as "quitting"
        (invitee && invitee->pl->hidden&(1<<HIDE_INVITE)))
    {
        return localizedPrintf( inviter, "playerNotFound", invitee_name);
    }

    if (entIsTrial(inviter))
    {
        return localizedPrintf( inviter, "CannotInviteTrialAccount");
    }

    if (inviter->db_id == invitee_id)
    {
        return localizedPrintf( inviter, "CannotInviteYourself");
    }

    //////////////////////////////////////////////////////////
    // Push the following checks to the invitee's server
    // since we can't guarantee we have the invitee's info here
    // We're pushing all necessary inviter information with the invite_long.
    //////////////////////////////////////////////////////////

    if (!server_state.allowHeroAndVillainPlayerTypesToTeamUp)
    {
        (*disallowedBitfield) |= (1 << 0); // disallow alignment mix because of server state
    }

    // If you are on a Coed server and inviting the first enemy to your team, all your teammates must be on this map with you. 
    // (Because if your team isn't already coed, there isn't currently a way to know whether teammate's servers are coed )
    if (!areMembersAlignmentMixed(inviter->teamup ? &inviter->teamup->members : NULL))
    {
        if (!areAllMembersOnThisMap(inviter->teamup ? &inviter->teamup->members : NULL))
        {
            (*disallowedBitfield) |= (1 << 1); // disallow alignment mix because team isn't on the same map
        }
        else if (inviter->teamup && inviter->teamup->activetask && inviter->teamup->activetask->def && inviter->teamup->activetask->def->alliance != SA_ALLIANCE_BOTH)
        {
            (*disallowedBitfield) |= (1 << 2); // disallow alignment mix because alliance-specific mission is selected
        }
    }

    if (!server_state.allowPrimalAndPraetoriansToTeamUp)
    {
        (*disallowedBitfield) |= (1 << 3); // disallow universe mix because of server state
    }

    // If you are on an multiversal server and inviting the first cross-universal entity to your team, all your teammates must be on this map with you. 
    // (Because if your team isn't already multiversal, there isn't currently a way to know whether teammate's servers are multiversal )
    if (!areMembersUniverseMixed(inviter->teamup ? &inviter->teamup->members : NULL)
        && !areAllMembersOnThisMap(inviter->teamup ? &inviter->teamup->members : NULL))
    {
        (*disallowedBitfield) |= (1 << 4); // disallow universe mix because team isn't on the same map
    }

    if (inviter->pl->inviter_dbid)
    {
        return localizedPrintf( inviter, "SelfConsideringOffer");
    }

    if (db_state.is_endgame_raid)
    {
        return localizedPrintf( inviter, "CouldNotActionPlayerReason", "InviteString", invitee_name, "YouOnIncarnateTrial" );
    }

    return NULL;
}

char *team_CheckInviterConditions(Entity *inviter, char *invitee_name, int *disallowedBitfield)
{
    char *ret;

    if (invitee_name[0] == 0)
    {
        return localizedPrintf(inviter, "incorrectFormat", "teaminviteString", "playerNameString", "emptyString", "teaminviteSynonyms");
    }

    if (ret = common_CheckInviterConditions(inviter, invitee_name, disallowedBitfield))
    {
        return ret;
    }

    // check team-specific conditions

    if (inviter->pl->taskforce_mode == TASKFORCE_DEFAULT)
    {
        return localizedPrintf(inviter, "CannotInviteToTaskForce");
    }

    if (inviter->teamup_id && (inviter->db_id != inviter->teamup->members.leader))
    {
        return localizedPrintf(inviter, "OnlyteamLeaderCanInvite");
    }

    if (inviter->teamup && inviter->teamup->members.count >= MAX_TEAM_MEMBERS)
    {
        return localizedPrintf(inviter, "TeamIsFull");
    }

    if (inviter->teamup && inviter->teamup->activetask &&
        ((inviter->teamup->activetask->def && inviter->teamup->activetask->def->maxPlayers && inviter->teamup->members.count >= inviter->teamup->activetask->def->maxPlayers)
         || (inviter->teamup->activetask->compoundParent && inviter->teamup->activetask->compoundParent->maxPlayers && inviter->teamup->members.count >= inviter->teamup->activetask->compoundParent->maxPlayers)))
    {
        return localizedPrintf(inviter, "TeamIsFullForCurrentMission");
    }

    return NULL;
}

char *league_CheckInviterConditions(Entity *inviter, char *invitee_name, int *disallowedBitfield)
{
    int sgroup_id = 0;
    int id = dbPlayerIdFromName(invitee_name);
    char *ret = 0;

    if (invitee_name[0] == 0)
    {
        return localizedPrintf(inviter, "incorrectFormat", "leagueinviteString", "playerNameString", "emptyString", "leagueinviteSynonyms");
    }

    if (ret = common_CheckInviterConditions(inviter, invitee_name, disallowedBitfield))
    {
        return ret;
    }

    // check league-specific conditions

    if (inviter->league_id && (inviter->db_id != inviter->league->members.leader))
    {
        return localizedPrintf(inviter, "OnlyLeagueLeaderCanInvite");
    }
    else if (inviter->teamup_id && (inviter->db_id != inviter->teamup->members.leader))
    {
        return localizedPrintf(inviter, "OnlyLeagueLeaderCanInvite");
    }

    if (inviter->league && inviter->league->members.count >= MAX_LEAGUE_MEMBERS)
    {
        return localizedPrintf(inviter, "LeagueIsFull");
    }

    if (inviter->teamup)
    {
        //    don't allow league invite to your own team
        int i;
        for (i = 0; i < inviter->teamup->members.count; ++i)
        {
            if (stricmp(inviter->teamup->members.names[i],invitee_name) == 0)
            {
                return localizedPrintf(inviter, "LeagueCantInviteOwnTeam");
            }
        }
    }
    //    if the inviter recently created the league, block him
    if ((dbSecondsSince2000() - inviter->pl->league_accept_blocking_time) < BLOCK_ACCEPT_TIME)
        return localizedPrintf( inviter, "LeagueWaitToInvite" );

    return NULL;
}

// common to teams and leagues
char *common_CheckInviteeConditions(Entity *invitee, char *invitee_name, char *taskdesc, int *type, int inviter_dbid, char *inviter_name, int disallowedBitfield, int inviter_praetorianProgress, int inviter_playerTypeByLocation)
{
    int interlockCheck = turnstileMapserver_teamInterlockCheck(invitee, 1);
    if (interlockCheck == 2)
    {
        return localizedPrintf(invitee, "CantAcceptWhileInQueue");
    }
    if (interlockCheck != 0)
    {
        return localizedPrintf(invitee, "CantAcceptWhileInQueue");
    }

    if (OnArenaMap())
    {
        return localizedPrintf(invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "PlayerMustExitArena");
    }

    if (db_state.base_map_id == 41) // Destroyed Galaxy City
    {
        return localizedPrintf(invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "PlayerMustFinishTutorial");
    }

    if (OnMissionMap())
    {
        //    if the inviter is on the same map as us, this invite should be okay
        Entity *inviter = entFromDbId(inviter_dbid);
        if (!inviter)
            return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "PlayerMustExitMission" );
    }

    if (invitee->league_id)
    {
        if (invitee->league && invitee->league->members.count > 1)
        {
            return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "CantInviteLeaguePlayer" );
        }
        else
        {
            league_MemberQuit(invitee, 1);
        }
    }
    
    if (isIgnored( invitee, inviter_dbid))
    {
        return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "PlayerHasIgnoredYou" );
    }
    else if (invitee->pl->inviter_dbid) // already being invited to another team
    {
        return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "PlayerIsConsideringAnotherOffer" );
    }
    else if (invitee->pl->last_inviter_dbid == inviter_dbid && (dbSecondsSince2000() - invitee->pl->last_invite_time) < BLOCK_INVITE_TIME)
    {
        return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "MustWaitLonger" );
    }
    else if (!canInviteeAndInviterPlayTogether(invitee->pchar->playerTypeByLocation, invitee->pl->praetorianProgress, inviter_playerTypeByLocation, inviter_praetorianProgress))
    {
        return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "CantInviteEnemy" );
    }
    else if ((disallowedBitfield & (1 << 0)) && (invitee->pchar->playerTypeByLocation != inviter_playerTypeByLocation))
    {
        return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "CantInviteEnemy" );
    }
    else if ((disallowedBitfield & (1 << 1)) && (invitee->pchar->playerTypeByLocation != inviter_playerTypeByLocation))
    {
        return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "CantMakeANonCoedTeamIntoACoedTeamUnlessAllTeammatesAreOnThisMap" );
    }
    else if ((disallowedBitfield & (1 << 2)) && (invitee->pchar->playerTypeByLocation != inviter_playerTypeByLocation))
    {
        return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "CantMakeACoedTeamWhenAnAllianceSpecificMissionIsSelected" );
    }
    else if ((disallowedBitfield & (1 << 3)) && (ENT_IS_IN_PRAETORIA(invitee) != (inviter_praetorianProgress == kPraetorianProgress_Tutorial || inviter_praetorianProgress == kPraetorianProgress_Praetoria)))
    {
        return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "CantInviteEnemy" );
    }
    else if ((disallowedBitfield & (1 << 4)) && (ENT_IS_IN_PRAETORIA(invitee) != (inviter_praetorianProgress == kPraetorianProgress_Tutorial || inviter_praetorianProgress == kPraetorianProgress_Praetoria)))
    {
        return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "CantMakeAMultiversalTeamUnlessAllTeammatesAreOnThisMap" );
    }
    else if (invitee->pl->lfg & LFG_NOGROUP)
    {
        return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "PlayerNoGroup" );
    }
    else if (db_state.is_endgame_raid)
    {
        return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "PlayerOnIncarnateTrial" );
    }

    return NULL;
}

char *team_CheckInviteeConditions(Entity *invitee, char *invitee_name, char *taskdesc, int *type, int inviter_dbid, char *inviter_name, int disallowedBitfield, int inviter_praetorianProgress, int inviter_playerTypeByLocation)
{
    char *ret;

    if (ret = common_CheckInviteeConditions(invitee, invitee_name, taskdesc, type, inviter_dbid, inviter_name, disallowedBitfield, inviter_praetorianProgress, inviter_playerTypeByLocation))
    {
        return ret;
    }

    // check team-specific conditions

    if (invitee->pl->taskforce_mode == TASKFORCE_DEFAULT)
    {
        return localizedPrintf( invitee, "CannotInviteTaskForcePlayer", invitee_name);
    }

    if (invitee->teamup_id)
    {
        if( invitee->teamup->members.count > 1 ) // if player is already on a team forget it
            return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "PlayerIsAlreadyInTeam" );
        else
        {
            int task_assignedDbId;
            StoryTaskHandle sahandle;

            sscanf(taskdesc, "%i:%Ii:%i:%i:%hhi", &task_assignedDbId, SAHANDLE_SCANARGSPOS(sahandle));
            if (invitee->teamup->activetask->assignedDbId &&
                invitee->teamup->activetask->assignedDbId == task_assignedDbId &&
                isSameStoryTaskPos(&sahandle, &invitee->teamup->activetask->sahandle))
            {
                *type = 0; // combining teams assigned to same task
            }
            else if (TeamTaskValidateStoryTask(invitee, &sahandle) != TTSE_SUCCESS)
            {
                return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "PlayerCannotJoinMission" );
            }
            else if ( invitee->teamup->activetask->assignedDbId || (invitee->pl->taskforce_mode == TASKFORCE_ARCHITECT) )
            {
                *type = 1;
            }
        }
    }
    else if (invitee->pl->taskforce_mode == TASKFORCE_ARCHITECT)
    {
        if (invitee && invitee->taskforce && (invitee->taskforce->members.count > 1) )
        {
            return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "PlayerIsAlreadyInTeam" );
        }
    }

    return NULL;
}

// type : 0 - regular invite, 1 - own a mission warning, 2 - ok box because on a mission,
char *league_CheckInviteeConditions(Entity *invitee, char *invitee_name, char *taskdesc, int *type, int inviter_dbid, 
                                    char *inviter_name, int disallowedBitfield, int inviter_praetorianProgress, 
                                    int inviter_playerTypeByLocation, int remainingSpots, int numTeams)
{
    char *ret;

    if (ret = common_CheckInviteeConditions(invitee, invitee_name, taskdesc, type, inviter_dbid, inviter_name, disallowedBitfield, inviter_praetorianProgress, inviter_playerTypeByLocation))
    {
        return ret;
    }

    // check league-specific conditions
    if (invitee->teamup)
    {
        //    out of spots
        if (invitee->teamup->members.count > remainingSpots)
        {
            return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "NotEnoughSpots" );
        }

        if ((invitee->teamup->teamSwapLock || invitee->taskforce_id) && numTeams >= MAX_LEAGUE_TEAMS)
        {
            return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "LockedTeamFull" );
        }
    }

    //    recently joined/left a league, blocked
    if ((dbSecondsSince2000() - invitee->pl->league_accept_blocking_time) < BLOCK_ACCEPT_TIME)
        return localizedPrintf( invitee, "CouldNotActionPlayerReason", "InviteString", invitee_name, "BlockingAccept" );

    return NULL;
}
static int league_teamCheckAtomicJoin(Entity *leader, Entity *joiner)
{
    char taskdesc[100];
    int disallowedBitfield = 0;
    int temp_leagueid = joiner->league_id;
    int temp_teamid = joiner->teamup_id;
    int temp_incarnateTrial = db_state.is_endgame_raid;

    if (leader->teamup)
        sprintf(taskdesc, "%i:%i:%i:%i:%i", leader->teamup->activetask->assignedDbId, SAHANDLE_ARGSPOS(leader->teamup->activetask->sahandle) );
    else
        strcpy(taskdesc, "0:0:0:0:0");

    joiner->league_id = joiner->teamup_id = 0;        //    pretend they aren't in a league or team for the purpose of the join
    db_state.is_endgame_raid = 0;
    if (!team_CheckInviterConditions(leader, joiner->name, &disallowedBitfield))
    {
        int type = 0;
        if (!team_CheckInviteeConditions(joiner, joiner->name, taskdesc, &type, leader->db_id, leader->name, disallowedBitfield,
            leader->pl->praetorianProgress, leader->pchar->playerTypeByLocation))
        {
            joiner->league_id = temp_leagueid;
            joiner->teamup_id = temp_teamid;
            db_state.is_endgame_raid = temp_incarnateTrial;
            return 1;
        }
    }
    joiner->league_id = temp_leagueid;
    joiner->teamup_id = temp_teamid;
    db_state.is_endgame_raid = temp_incarnateTrial;
    return 0;
}
static int league_teamCheckSwapEnts(Entity *leader1, Entity *ent2)
{
    assert(leader1);
    assert(ent2);
    if (leader1 && ent2)
    {
        if (!ent2->teamup || (ent2->teamup && (ent2->teamup->members.leader != ent2->db_id)))    //    leader can not be traded for now
        {
            return league_teamCheckAtomicJoin(leader1, ent2);
        }
    }
    return 0;
}

static void team_leaveOrRelay(ClientLink *client, int quitId)
{
    char quitStr[200];
    Entity *teamMate;
    sprintf(quitStr, "team_quit_relay %i", quitId);
    if (teamMate = findEntOrRelayById(client, quitId, quitStr, NULL))
    {    
        team_MemberQuit(teamMate);
    }
}

static void league_leaveOrRelay(ClientLink *client, int quitId)
{
    char quitStr[200];
    Entity *e;
    sprintf(quitStr, "leaveLeagueRelay %i", quitId);
    if (e = findEntOrRelayById(client, quitId, quitStr, NULL))
    {    
        league_MemberQuit(e, 1);
    }
}

static void team_acceptOfferOrRelay(ClientLink *client, int inviterDBId, int teamLeaderId, int inviteeDBid, int pvpSwitch)
{
    Entity *teamLeader;
    char acceptStr[200];
    sprintf(acceptStr, "team_accept_offer_relay %i %i %i %i 0", teamLeaderId, inviteeDBid, inviterDBId, pvpSwitch);

    if (teamLeader = findEntOrRelayById(client, teamLeaderId, acceptStr, NULL))
    {
        int success = 0;
        //    need to verify that this person didn't just quit and these are delayed invites
        //    if they are, have the person leave
        if (dbSecondsSince2000() - teamLeader->pl->league_accept_blocking_time < BLOCK_ACCEPT_TIME)
        {
            chatSendToPlayer(inviteeDBid, localizedPrintf( teamLeader,"CouldNotActionPlayerReason", "InviteString", teamLeader->name, "AcceptBlock"), INFO_USER_ERROR, 0 );
            chatSendToPlayer(teamLeader->db_id, localizedPrintf( teamLeader,"CouldNotActionReason", "JoinString", "AcceptBlock"), INFO_USER_ERROR, 0 );
        }
        else if (team_AcceptOffer(teamLeader, inviteeDBid, 0))
        {
            success = 1;
        }
        if (!success)
        {
            league_leaveOrRelay(client, inviteeDBid);
        }
    }
}

//    helper function to relay league invites
//    assumes that league already exists and the ent just needs to make sure the leader performs the invite
//    could be modified so that if the inviter isn't in a league, it could assume the inviter would become the new leader...
static void league_acceptOfferOrRelay(ClientLink *client, Entity *inviter, int inviteeDBid, int teamLockStatus, int pvpSwitch)
{
    //    client can be NULL
    devassert(inviter);
    devassert(inviter->league);
    if (inviter && inviter->league)
    {
        char acceptStr[200];
        Entity *leagueLeader = NULL;
        int result = 0;
        sprintf(acceptStr, "league_accept_offer_relay %i %i %i %i %i", inviter->league->members.leader, inviteeDBid, inviter->db_id, teamLockStatus, pvpSwitch);
        if (leagueLeader = findEntOrRelayById( client, inviter->league->members.leader, acceptStr, &result ))
        {
            if (!league_AcceptOffer( leagueLeader, inviteeDBid, inviter->db_id, teamLockStatus))
            {
                team_leaveOrRelay(client, inviteeDBid);
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------
static int s_levelingPacts_enabled;
void LevelingPactEnable(int leveling_pact_status)
{
    s_levelingPacts_enabled = leveling_pact_status;
}
void LevelingPactInvite(Entity *e, char *invitee_name)
{
    char *msg = NULL;
    int invitee_dbid = 0;
    Entity *invitee = NULL;

    if(!e || !e->owned || !e->pl || !e->pchar)
        return;

    // this case turns on/off level pack creation
    if (!s_levelingPacts_enabled)
    {
        msg = localizedPrintf(e, "LevelingPactSuspended");
    }
    else if(!invitee_name || !invitee_name[0])
    {
        msg = localizedPrintf(e, "incorrectFormat", "levelingpactString", "playerNameString", "emptyString", "levelingpactSynonyms");
    }
    else if(e->db_id == invitee_dbid ||
            !(invitee_dbid = dbPlayerIdFromName(invitee_name)) ||
            !(invitee = entFromDbId(invitee_dbid)) ||
            !invitee->pl || !invitee->pchar ||
            invitee->logout_timer ||
            !invitee->owned ||
            (invitee->pl->hidden & (1<<HIDE_INVITE)) )
    {
        msg = localizedPrintf(e, "playerNotOnMap", invitee_name);
    }
    else if (db_state.base_map_id == 41) // Destroyed Galaxy City mapmove number
    {
        msg = localizedPrintf(e, "LevelingPactNotHere");
    }
    else if(invitee->levelingpact)
    {
        msg = localizedPrintf(e, "LevelingPactAlreadyIn", invitee_name);
    }
    else if(e->levelingpact && e->levelingpact->members.count >= MAX_LEVELINGPACT_MEMBERS) // statserver will reconfirm this
    {
        msg = localizedPrintf(e, "LevelingPactTooManyMembers");
    }
    else if(eaiFind(&invitee->pl->levelingpact_invites, e->db_id) >= 0)
    {
        msg = localizedPrintf(e, "LevelingPactAlreadyRequested", invitee_name);
    }
    else if(character_CalcExperienceLevel(e->pchar) > (LEVELINGPACT_MAXLEVEL-1) ||
            character_CalcExperienceLevel(invitee->pchar) > (LEVELINGPACT_MAXLEVEL-1) )
    {
        char buf[16];
        sprintf(buf, "%d", LEVELINGPACT_MAXLEVEL);
        msg = localizedPrintf(e, "LevelingPactLevelTooHigh", buf);
    }
    else if ( entIsTrial(e))
    {
        msg = localizedPrintf( e, "LevelingPactTrialInviter" );
    }
    else if ( entIsTrial(invitee))
    {
        msg = localizedPrintf( e, "LevelingPactTrialInvitee" );
    }
    else if(isIgnored(invitee, e->db_id))
    {
        msg = localizedPrintf(e, "CouldNotActionPlayerReason", "InviteString", invitee_name, "PlayerHasIgnoredYou");
    }

    if(msg)
    {
        // something's awry, let the player know
        chatSendToPlayer(e->db_id, msg, INFO_USER_ERROR, 0);
    }
    else
    {
        // all is well, send the invite
        eaiPush(&invitee->pl->levelingpact_invites, e->db_id);
        chatSendToPlayer(e->db_id, localizedPrintf(e, "LevelingPactInvited", invitee_name), INFO_SVR_COM, 0);

        START_PACKET(pak_out, invitee, SERVER_LEVELINGPACT_INVITE)
        pktSendBitsAuto(pak_out, e->db_id);
        pktSendString(pak_out, e->name);
        END_PACKET
    }
}

void LevelingPactBefriend(Entity *inviter, Entity *accepter)
{
    if(!inviter || !accepter)
        return;

    if(inviter->levelingpact)
    {
        int i;
        char *accepterName, *memberName;
        Entity *pactMember;
        accepterName = dbPlayerNameFromId(accepter->db_id);
        for(i = 0; i < inviter->levelingpact->count; i++)
        {
            //this will fail if the pact member isn't on the same map.  We should probably change it if we ever add more than one 
            //member per pact.
            pactMember = entFromDbId(inviter->levelingpact->members.ids[i]);
            if(pactMember)
            {
                if( pactMember->db_id != accepter->db_id )
                {
                    memberName = dbPlayerNameFromId(pactMember->db_id);
                    addFriend( accepter, memberName );
                    addFriend( pactMember, accepterName );
                }
            }
        }
    }
    else //if( !isFriend( e, pactMember->db_id ) )
    {
        char *accepterName, *inviterName;
        accepterName = dbPlayerNameFromId(accepter->db_id);
        inviterName = dbPlayerNameFromId(inviter->db_id);
        addFriend( accepter, inviterName );
        addFriend( inviter, accepterName );
    }
}

void LevelingPactAccept(Entity *e, int inviter_dbid)
{
    char *msg = NULL;
    Entity *inviter = NULL;

    if(!e || !e->owned || !e->pl || !e->pchar)
        return;

    if(eaiFindAndRemoveFast(&e->pl->levelingpact_invites, inviter_dbid) < 0)
    {
        dbLog("cheater", e, "tried to accept an invite to %d's leveling pact, but wasn't invited", inviter_dbid);
        if((inviter = entFromDbId(inviter_dbid)))
        msg = localizedPrintf(e, "playerNotOnMap", entGetName(inviter) );
    }
    else if(e->levelingpact)
    {
        msg = localizedPrintf(e, "LevelingPactAlreadyInSelf");
    }
    else if(!(inviter = entFromDbId(inviter_dbid)) ||
            !inviter->pl || !inviter->pchar ||
            inviter->logout_timer ||
            !inviter->owned ||
            (inviter->pl->hidden & (1<<HIDE_INVITE)))
    {
        if(dbPlayerNameFromId(inviter_dbid))
            msg = localizedPrintf(e, "playerNotOnMap", dbPlayerNameFromId(inviter_dbid) );
        //we have to quit if there is no inviter since we'd otherwise try to create a
        //pact with a non-existent inviter.  This will crash the mapserver.
        else
            msg = localizedPrintf(e, "PlayerNotInZone");
    }
    else if(inviter->levelingpact && inviter->levelingpact->members.count >= MAX_LEVELINGPACT_MEMBERS) // statserver will reconfirm this
    {
        msg = localizedPrintf(e, "LevelingPactTooManyMembers");
    }
    else if(character_CalcExperienceLevel(e->pchar) > (LEVELINGPACT_MAXLEVEL-1) ||
            character_CalcExperienceLevel(inviter->pchar) > (LEVELINGPACT_MAXLEVEL-1) )
    {
        char buf[16];
        sprintf(buf, "%d", LEVELINGPACT_MAXLEVEL);
        msg = localizedPrintf(e, "LevelingPactLevelTooHigh", buf);
    }
    else if(isIgnored(inviter, e->db_id))
    {
        msg = localizedPrintf(e, "CouldNotActionPlayerReason", "JoinString", inviter->name, "PlayerHasIgnoredYou");
    }
    else if ( entIsTrial(e))
    {
        msg =  localizedPrintf( e, "LevelingPactTrialInvitee" );
    }
    else if ( entIsTrial(inviter))
    {
        msg =  localizedPrintf( e, "LevelingPactTrialInviter" );
    }

    if(msg)
    {
        // something's awry, let the player know
        chatSendToPlayer(e->db_id, msg, INFO_USER_ERROR, 0);
    }
    else
    {
        SgrpStat_SendPassthru(e->db_id,0, "statserver_levelingpact_join %d %d %d %d \"%s\" 0",
            inviter->db_id, inviter->pchar->iExperiencePoints,
            e->db_id, e->pchar->iExperiencePoints, e->name);
        
        if(!inviter)
            inviter = entFromDbId(inviter_dbid);

        //invite everyone as friends.
        LevelingPactBefriend(inviter,e);
    }
}

void LevelingPactDecline(Entity *e, int inviter_dbid, char *msg)
{
    if(e && e->owned && e->pl &&
        eaiFindAndRemoveFast(&e->pl->levelingpact_invites, inviter_dbid) >= 0)
    {
        if(!eaiSize(&e->pl->levelingpact_invites))
            eaiDestroy(&e->pl->levelingpact_invites);
        chatSendToPlayer(inviter_dbid, localizedPrintf(e, "LevelingPactDeclines", msg), INFO_USER_ERROR, 0);
    }
}

void LevelingPactQuit(Entity *e)
{
    if(e && e->owned && e->pchar)
    {
        if(e->levelingpact)
        {
            int *members = &e->db_id;
            int memberCount = 1;

            // this will likely result in the player missing some xp, recent gains by the pact will be ignored
            // the statserver will correct the pact's experience and member count

            if(!dbContainerAddDelMembers(CONTAINER_LEVELINGPACTS, 0, 0, e->levelingpact_id, memberCount, members, NULL))
            {
                //this can happen if two quit requests are sent before the first one can get resolved by the dbserver.
                dbLog("levelingpact", e, "Warning: could not quit leveling pact %d", e->levelingpact_id);
                chatSendToPlayer(e->db_id, localizedPrintf(e, "LevelingPactCouldNotQuit"), INFO_USER_ERROR, 0);
            }
        }
        else
        {
            chatSendToPlayer(e->db_id, localizedPrintf(e, "LevelingPactNotAMember"), INFO_USER_ERROR, 0);
        }
    }
}


//leveling pact CSR functions
void LevelingPactAddMember(Entity *e, char *name1, char *name2, int exp)
{
    int dbid1, dbid2;
    int failure = 0;

    if(!name1 || !name2 || !e || !e->owned || exp < 0)
        failure = 1;

    if(!failure)
    {
        //this means this command won't work on entities that aren't on line.
        dbid1 = dbPlayerIdFromName(name1);
        dbid2 = dbPlayerIdFromName(name2);

        if(!dbid1 || !dbid2 || dbid1 < 0 || dbid2 < 0)
        {
            chatSendToPlayer(e->db_id, localizedPrintf(e, "LevelingPactInvalidName"), INFO_USER_ERROR, 0);
            failure = 1;
        }
        else if(dbid1 == dbid2)
        {
            chatSendToPlayer(e->db_id, localizedPrintf(e, "LevelingPactDuplicatePact"), INFO_USER_ERROR, 0);
            failure =1;
        }
    }

    if(!failure)
    {
        SgrpStat_SendPassthru(e->db_id, 0, "statserver_levelingpact_join %d %d %d %d \"%s\" %d",
                                        dbid1, exp, dbid2, exp, "", e->db_id);
    }
}
void LevelingPactSetExperience(Entity *e, char *name, int xp)
{
    int memberId;
    Entity *member;
    if(!name || !e)
    {
        return;
    }

    memberId = dbPlayerIdFromName(name);
    member = entFromDbId(memberId);

    if(!member || !member->levelingpact )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e, "LevelingPactCSRNotAMember"), INFO_USER_ERROR, 0);
        return;
    }
    xp -= member->levelingpact->experience;
    
    {
        SgrpStat_SendPassthru(e->db_id,0,"statserver_levelingpact_addxp %d %d %d",
            member->levelingpact_id,xp, stat_TimeSinceXpReceived(e));    
    }
    chatSendToPlayer(e->db_id, localizedPrintf(e, "LevelingPactCSRSetXP"), INFO_USER_ERROR, 0);
}

// void LevelingPactRemoveMember(Entity *e, char *name)
// {
//     Entity *memberToKick;
//     int dbid;
//     if(!e)
//     {
//         return;
//     }
//     dbid = dbPlayerIdFromName(name);
//     memberToKick = entFromDbId(dbid);
//     
// 
//     if(memberToKick)
//     {
//         if(memberToKick->levelingpact)
//         {
//             SgrpStat_SendPassthru(0,0, "statserver_levelingpact_quit %d", dbid);
//             chatSendToPlayer(dbid, localizedPrintf(memberToKick, "LevelingPactPactSevered"), INFO_USER_ERROR, 0);
//         }
//         else
//         {
//             chatSendToPlayer(e->db_id, localizedPrintf(e, "LevelingPactCSRNotAMember"), INFO_USER_ERROR, 0);
//         }
//     }
// }

void LevelingPactSetInfluence(Entity *e, char *name, int influence)
{
    int memberId;
    Entity *member;
    if(!name || !e)
    {
        return;
    }

    memberId = dbPlayerIdFromName(name);
    member = entFromDbId(memberId);

    if(!member || !member->levelingpact )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e, "LevelingPactCSRNotAMember"), INFO_USER_ERROR, 0);
        return;
    }

    {
        U32 isVillain = SAFE_MEMBER2(e,pchar,playerTypeByLocation) == kPlayerType_Villain;
        SgrpStat_SendPassthru(e->db_id,0, "statserver_levelingpact_csr_add_inf %d %d %d",
            member->levelingpact_id,influence, isVillain);
    }
    chatSendToPlayer(e->db_id, localizedPrintf(e, "LevelingPactCSRSetInfluence"), INFO_USER_ERROR, 0);

}

void LevelingPactInfo(Entity *e, char *name, ClientLink *client)
{
    int memberId;
    int i;
    Entity *member;
    if(!name || !e)
    {
        return;
    }

    memberId = dbPlayerIdFromName(name);
    member = entFromDbId(memberId);

    if(!member || !member->levelingpact )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e, "LevelingPactCSRNotAMember"), INFO_USER_ERROR, 0);
        return;
    }

    conPrintf(client, "LevelingPact %i\n", member->levelingpact_id);
    conPrintf(client, "    Experience: %i\n", member->levelingpact->experience);
    for(i = 0; i < member->levelingpact->members.count; i++)
    {
        conPrintf(client, "    MEMBER: %s (%i)\n", member->levelingpact->members.names[i], member->levelingpact->members.ids[i]);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------

void chatCommand( Cmd * cmd, ClientLink *client, char* str )
{
    char tmp_str[1024];
    char tmp_str2[1024];
    char player_name[64];
    char stack_tmp_str[1024];
    int stack_tmp_int = tmp_int; // Save these on the stack for ease of debugging recursive calls
    int stack_tmp_int2 = tmp_int2;
    int stack_tmp_int3 = tmp_int3;
    int stack_tmp_int4 = tmp_int4;
    int stack_tmp_int5 = tmp_int5;
    int stack_tmp_int6 = tmp_int6;
    int stack_tmp_int7 = tmp_int7;
    int stack_tmp_dbid = tmp_dbid;

    Entity *e = 0;
    int online;

    // JE: For any new commands that modify an entity's data, and *don't* go through findEntOrRelay,
    //  check e->owned first, if it's not set, then don't execute the command!  We still want chat
    //  commads to go through, though.

    if( client )
        e = client->entity;

    strncpyt(tmp_str, tmp_str_cmd,1024);
    strncpyt(tmp_str2, tmp_str2_cmd,1024);
    strncpyt(stack_tmp_str,str,1024);
    tmp_str_cmd[0]='\0';
    strncpyt(player_name, player_name_cmd, 64);
    player_name_cmd[0]='\0';

    switch (cmd->num )
    {

        // CHAT CHANNELS
        //------------------------------------------------------------------------------------------------

        case CMD_CHAT_PRIVATE:
            {
                if( e )
                    chatSendPrivateMsg( e, tmp_str, 0, 1, 0);
            } break;
        case CMD_CHAT_PRIVATE_TEAM_LEADER:
            {
                if( e )
                    chatSendPrivateMsg( e, tmp_str, CMD_CHAT_PRIVATE_TEAM_LEADER, 1, 0);
            }break;
        case CMD_CHAT_PRIVATE_LEAGUE_LEADER:
            {
                if( e )
                    chatSendPrivateMsg( e, tmp_str, CMD_CHAT_PRIVATE_LEAGUE_LEADER, 1, 0);
            }break;
        case CMD_CHAT_TRIAL_RESPONSE:
            {
                if(e)
                {
                    chatSendTrialMessage(e, tmp_str);
                }
            }break;
        case CMD_CHAT_BROADCAST:
            {
                if( e )
                    chatSendBroadCast( e, tmp_str );
            } break;

        case CMD_CHAT_MAP_ADMIN:
            {
                if( e )
                    chatSendMapAdmin( e, tmp_str );
            } break;

        case CMD_CHAT_LOCAL:
            {
                if( e )
                    chatSendLocal( e, tmp_str );
            } break;

        case CMD_CHAT_REQUEST:
            {
                if( e )
                    chatSendRequest( e, tmp_str );
            } break;

        case CMD_CHAT_LOOKING_FOR_GROUP:
            {
                if (e)
                    chatSendToLookingForGroup(e, tmp_str);
            } break;

        case CMD_CHAT_FRIEND:
            {
                if( e )
                    chatSendToFriends( e, tmp_str );
            } break;

        case CMD_CHAT_TEAM:
            {
                if( e )
                     chatSendToTeamup( e, tmp_str, INFO_TEAM_COM );
            } break;

        case CMD_CHAT_SUPERGROUP:
            {
                if( e )
                    chatSendToSupergroup( e, tmp_str, INFO_SUPERGROUP_COM );
            } break;

        case CMD_CHAT_LEVELINGPACT:
            {
                if( e )
                    chatSendToLevelingpact( e, tmp_str, INFO_LEVELINGPACT_COM );
            } break;

        case CMD_CHAT_ALLIANCE:
            {
                if (e)
                    chatSendToAlliance(e, tmp_str);
            } break;

        case CMD_CHAT_LEAGUE:
            {
                if (e)
                    chatSendToLeague( e, tmp_str, INFO_LEAGUE_COM);
            } break;

        case CMD_CHAT_ADMIN:
            {
                if (e && tmp_str[0] != '\0')
                    chatSendToAll( e, tmp_str );
            } break;

        case CMD_CHAT_ARENA:
            {
                if( e )
                    chatSendToArena( e, tmp_str, INFO_ARENA );
            } break;

        case CMD_CHAT_ARENA_GLOBAL:
            {
                if( e )
                    chatSendToArenaChannel( e, tmp_str );
            } break;

        case CMD_CHAT_ARCHITECT_GLOBAL:
            {
                if ( e )
                    chatSendToArchitectGlobal( e, tmp_str );
            }break;

        case CMD_CHAT_HELP_GLOBAL:
            {
                if( e )
                    chatSendToHelpChannel( e, tmp_str );
            } break;

        case CMD_CHAT_EMOTE:
            {
                if( e )
                    chatSendEmote( e, tmp_str );
            } break;

        case CMD_CHAT_EMOTE_COSTUME_CHANGE:
            {
                if( e && e->pl )
                {
                    chatSendEmoteCostumeChange( e, tmp_str, tmp_int );
                }
            } break;

        case CMD_CHAT_REPLY:
            {
                if( e )
                    chatSendReply( e, tmp_str );
            }break;

        case CMD_CHAT_TELL_LAST:
            {
                if( e )
                    chatSendTellLast( e, tmp_str );
            }break;

        case CMD_SEND_SERVER_MSG:
            {
                int result = 0;
                Entity *e = findEntOrRelay( NULL, player_name, stack_tmp_str, &result);

                if ( e && tmp_str[0] != '\0' )
                    sendChatMsg( e, tmp_str, INFO_SVR_COM, 0 );

            }break;

        // FRIENDS
        //------------------------------------------------------------------------------------------------

        case CMD_ADD_FRIEND:
            {
                if( e && e->owned )
                {
                    if( tmp_str[0] == 0 )
                    {
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
                            "addfriendString", "playerNameString", "emptyString", "addfriendSynonyms" ), INFO_USER_ERROR, 0);
                    }
                    else
                        addFriend( e, tmp_str );
                }
            } break;

        case CMD_REMOVE_FRIEND:
            {
                if( e && e->owned )
                {
                    if( tmp_str[0] == 0 )
                    {
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
                            "removefriendString", "playerNameString", "emptyString", "removefriendSynonyms"), INFO_USER_ERROR, 0);
                    }
                    else
                        removeFriend( e, tmp_str );
                }
            } break;

        case CMD_FRIENDLIST:
            {
                if( e )
                    displayFriends( e );
            } break;


        // LEVELING PACTS
        //----------------------------------------------------------------------
        xcase CMD_DEBUG_LEVELINGPACT_ENABLE: 
            {
                LevelingPactEnable(stack_tmp_int);
                if (e)
                {
                    char buf[30];
                    sprintf(buf, "Leveling pact status:%s", stack_tmp_int ? "on" : "off");
                    chatSendToPlayer(e->db_id, buf, INFO_SVR_COM, 0);
                }
            }
        xcase CMD_LEVELINGPACT_INVITE:    LevelingPactInvite(e, player_name);
        xcase CMD_LEVELINGPACT_ACCEPT:    LevelingPactAccept(e, stack_tmp_int);
        xcase CMD_LEVELINGPACT_DECLINE:    LevelingPactDecline(e, stack_tmp_int, tmp_str);
        xcase CMD_LEVELINGPACT_QUIT:    LevelingPactQuit(e);
        xcase CMD_LEVELINGPACT_CSR_ADD_MEMBER:        LevelingPactAddMember(e, player_name, tmp_str, 0);
        xcase CMD_LEVELINGPACT_CSR_ADD_MEMBER_XP:    LevelingPactAddMember(e, player_name, tmp_str, stack_tmp_int);
        xcase CMD_LEVELINGPACT_CSR_SET_EXPERIENCE:    LevelingPactSetExperience(e, player_name, stack_tmp_int);
        xcase CMD_LEVELINGPACT_CSR_SET_INFLUENCE:    LevelingPactSetInfluence(e, player_name, stack_tmp_int);
        xcase CMD_LEVELINGPACT_CSR_INFO:    LevelingPactInfo(e, player_name, client);
        break;

        // TEAMUPS
        //------------------------------------------------------------------------------------------------

        case CMD_TEAM_INVITE:
            {
                Entity *inviter = e; // this is for readability's sake
                char *invitee_name = player_name; // this is for readability's sake
                char *buf = 0;
                char command[512];
                char taskdesc[100] = {0};
                int disallowedBitfield = 0;
    
                if (!inviter || !inviter->pl)
                    break;

                if (db_state.is_endgame_raid)
                {
                    int eventID;
                    turnstileMapserver_getMapInfo(&eventID, NULL);
                    devassert(eventID);
                    if (eventID)
                    {
                        sprintf(command, "tut_invite %s", player_name);
                        serverParseClientEx(command, client, ACCESS_INTERNAL);
                    }
                    else
                    {
                        chatSendToPlayer(inviter->db_id, "TUTInvalidInstance", INFO_USER_ERROR, 0);
                    }
                    break;
                }

                buf = team_CheckInviterConditions(inviter, invitee_name, &disallowedBitfield);

                // Don't add checks here, add to team_CheckInviterConditions or common_CheckInviterConditions!

                if (buf)
                {
                    if (inviter)
                    {
                        chatSendToPlayer(inviter->db_id, buf, INFO_USER_ERROR, 0);
                    }
                    break;
                }

                if (inviter->teamup)
                    sprintf(taskdesc, "%i:%i:%i:%i:%i", inviter->teamup->activetask->assignedDbId, SAHANDLE_ARGSPOS(inviter->teamup->activetask->sahandle) );
                else
                    strcpy(taskdesc, "0:0:0:0:0");

                sprintf(command, "%s \"%s\" \"%s\" %i %s %i %i %i %i %i", "invite_long", invitee_name, inviter->name, inviter->db_id, taskdesc, inviter->pchar->playerTypeByLocation, disallowedBitfield, inviter->pl->praetorianProgress, RaidIsRunning(), inviter->supergroup_id );
                serverParseClient(command, NULL);
            }
            break;

        case CMD_TEAM_INVITE_LONG:
            {
                int result = 0;
                int inviter_playerTypeByLocation = stack_tmp_int;
                int disallowedBitfield = stack_tmp_int2;
                int inviter_praetorianProgress = stack_tmp_int3;
                int raid = stack_tmp_int4;
                int inviterSGID = stack_tmp_int5;
                int inviterDbID = stack_tmp_dbid;
                Entity* invitee = findEntOrRelay(NULL, player_name, stack_tmp_str, &result);
                int type = 0; // 0 - regular invite, 1 - own a mission warning, 2 - ok box because on a mission,

                // should we verify this person can actually offer here - no, assume we came from CMD_TEAM_INVITE
                if (invitee)
                {
                    char * buf = team_CheckInviteeConditions(invitee, player_name, tmp_taskdesc, &type, inviterDbID, tmp_str, disallowedBitfield, inviter_praetorianProgress, inviter_playerTypeByLocation);

                    // Is this deprecated?
                    if ((raid || RaidIsRunning()) && inviterSGID != invitee->supergroup_id)
                    {
                        buf = localizedPrintf(invitee, "CouldNotActionPlayerReason", "InviteString", player_name, "CantInviteRaidEnemy" );
                    }

                    // Don't add more checks here, add to team_CheckInviteeConditions or common_CheckInviteeConditions!

                    if (buf)
                    {
                        chatSendToPlayer(inviterDbID, buf, INFO_USER_ERROR, 0);
                        break;
                    }

                    invitee->pl->inviter_dbid = invitee->pl->last_inviter_dbid = inviterDbID; // remeber the inviter, so we can trap bad commands
                    invitee->pl->last_invite_time = dbSecondsSince2000();

                    if( type != 2 )
                        chatSendToPlayer( inviterDbID, localizedPrintf( invitee, "TeamInviteSent" ) , INFO_SVR_COM, 0 );

                    START_PACKET( pak, invitee, SERVER_TEAM_OFFER )
                        pktSendBits( pak, 32, inviterDbID );
                        pktSendString( pak, tmp_str );
                        pktSendBits( pak, 2, type );
                    END_PACKET
                }
            }
            break;

        case CMD_TEAM_INVITE_ACCEPT:
            {
                Entity *inviter;
                int result = 0;

                 if( e && e->pl ) // first make sure the inviter is the guy who actually invited
                {
                    if ( e->pl->inviter_dbid != stack_tmp_int )
                    {
                        // TODO: Mark player is suspicious...and take away their candy
                        chatSendToPlayer( stack_tmp_dbid, localizedPrintf(e,"youCouldNotJoinTeam", "Unknown") , INFO_USER_ERROR, 0 );
                        e->pl->inviter_dbid = 0;
                        break;
                    }

                    e->pl->inviter_dbid = e->pl->last_inviter_dbid = 0; // reset value so someone else can invite

                    if (db_state.is_endgame_raid)
                    {
                        chatSendToPlayer(stack_tmp_dbid, localizedPrintf( e, "youCouldNotJoinTeam", "YouOnIncarnateTrial"), INFO_USER_ERROR, 0 );
                        break;
                    }

                    // remove them from their team of one, silently
                    if( e->teamup )
                        teamDelMember( e, CONTAINER_TEAMUPS, 0, 0 );

                    //    remove them from their league if they were in one
                    if( e->league )
                        league_MemberQuit(e, 1);
                    e->pl->lfg = 0;
                    svrSendEntListToDb(&e, 1);
                }

                inviter = findEntOrRelay( client, player_name, stack_tmp_str, &result );

                if( inviter )
                {
                    // check for mismatched PvP flags
                    if (inviter->pl->pvpSwitch != stack_tmp_int2)
                    {
                        chatSendToPlayer(inviter->db_id, localizedPrintf( inviter, "CouldNotActionPlayerReason", "InviteString", tmp_str, stack_tmp_int2?"PlayerHasPvPOn":"PlayerDoesNotHavePvPOn"), INFO_USER_ERROR, 0 );
                        chatSendToPlayer(stack_tmp_dbid, localizedPrintf( inviter, "CouldNotActionReason", "JoinString", stack_tmp_int2?"YourPvPSwitchIsOn":"YourPvPSwitchIsOff"), INFO_USER_ERROR, 0 );
                        break;
                    }

                    if (dbSecondsSince2000() - inviter->pl->league_accept_blocking_time < BLOCK_ACCEPT_TIME)
                    {
                        chatSendToPlayer(inviter->db_id, localizedPrintf( inviter, "CouldNotActionPlayerReason", "InviteString", tmp_str, "AcceptBlock"), INFO_USER_ERROR, 0 );
                        chatSendToPlayer(stack_tmp_dbid, localizedPrintf( inviter, "CouldNotActionReason", "JoinString", "AcceptBlock"), INFO_USER_ERROR, 0 );
                        break;
                    }

                    if (db_state.is_endgame_raid)
                    {
                        chatSendToPlayer(inviter->db_id, localizedPrintf( inviter, "CouldNotActionPlayerReason", "InviteString", tmp_str, "YouOnIncarnateTrial"), INFO_USER_ERROR, 0 );
                        chatSendToPlayer(stack_tmp_dbid, localizedPrintf( inviter, "CouldNotActionReason", "JoinString", "PlayerOnIncarnateTrial"), INFO_USER_ERROR, 0 );
                        break;
                    }
                    // check to make sure the invitee can still do the mission that is selected
                    if (team_AcceptOffer( inviter, stack_tmp_dbid, 0))
                    {
                        //    If in a league, also accept to join the league
                        if (inviter->league_id && inviter->league)
                        {
                            if (inviter->league->members.count > 1)
                            {
                                int teamLockStatus = 0;
                                teamLockStatus |= inviter->pl && inviter->pl->taskforce_mode;
                                teamLockStatus |= inviter->teamup && inviter->teamup->teamSwapLock;
                                league_acceptOfferOrRelay(client, inviter, stack_tmp_dbid, stack_tmp_int2, teamLockStatus);
                            }
                            else
                            {
                                league_MemberQuit(inviter, 1);
                            }
                        }
                    }
                }

            }break;
        case CMD_TEAM_ACCEPT_OFFER_RELAY:
            {
                if(stack_tmp_dbid<=0)
                    return;

                if (relayCommandStart(NULL, stack_tmp_dbid, stack_tmp_str, &e, &online))
                {
                    int success = 0;
                    if (e->pl->pvpSwitch != stack_tmp_int3) // Make sure we're still of the same pvp switch as the team leader
                    {
                        chatSendToPlayer(stack_tmp_int2, localizedPrintf( e,"CouldNotActionPlayerReason", "InviteString", e->name, e->pl->pvpSwitch?"PlayerHasPvPOn":"PlayerDoesNotHavePvPOn"), INFO_USER_ERROR, 0 );
                        chatSendToPlayer(e->db_id, localizedPrintf( e,"CouldNotActionReason", "JoinString", e->pl->pvpSwitch?"YourPvPSwitchIsOn":"YourPvPSwitchIsOff"), INFO_USER_ERROR, 0 );
                    }
                    else if (dbSecondsSince2000() - e->pl->league_accept_blocking_time < BLOCK_ACCEPT_TIME)
                    {
                        //    need to verify that this person didn't just quit and these are delayed invites
                        //    if they are, have the person leave
                        chatSendToPlayer(stack_tmp_int2, localizedPrintf( e,"CouldNotActionPlayerReason", "InviteString", e->name, "AcceptBlock"), INFO_USER_ERROR, 0 );
                        chatSendToPlayer(e->db_id, localizedPrintf( e,"CouldNotActionReason", "JoinString", "AcceptBlock"), INFO_USER_ERROR, 0 );
                    }
                    else if (team_AcceptOffer( e, stack_tmp_int, stack_tmp_int4))
                    {
                        success = 1;
                    }
                    if (!success)
                        league_leaveOrRelay(client, stack_tmp_int);

                    relayCommandEnd(&e, &online);
                }
            }break;
        case CMD_TEAM_ACCEPT_RELAY:
            if(stack_tmp_dbid<=0)
                return;

            if (relayCommandStart(NULL, stack_tmp_dbid, stack_tmp_str, &e, &online))
            {
                if (e->pl->pvpSwitch != stack_tmp_int3) // Make sure we're still of the same pvp switch as the team leader
                {
                    chatSendToPlayer(stack_tmp_int2, localizedPrintf( e,"CouldNotActionPlayerReason", "InviteString", e->name, e->pl->pvpSwitch?"PlayerHasPvPOn":"PlayerDoesNotHavePvPOn"), INFO_USER_ERROR, 0 );
                    chatSendToPlayer(e->db_id, localizedPrintf( e,"CouldNotActionReason", "JoinString", e->pl->pvpSwitch?"YourPvPSwitchIsOn":"YourPvPSwitchIsOff"), INFO_USER_ERROR, 0 );
                    relayCommandEnd(&e, &online);
                    break;
                }
                team_AcceptRelay(e, stack_tmp_int, stack_tmp_int2, stack_tmp_int4 );
                relayCommandEnd(&e, &online);
            }
            break;

        case CMD_TEAM_INVITE_DECLINE:
            {
                if( e && e->pl ) // first make sure the inviter is the guy who actually invited
                {
                    if ( e->pl->inviter_dbid != stack_tmp_int )
                    {
                        // TODO: Mark player is suspicious...and take away their candy
                        e->pl->inviter_dbid = 0;
                        break;
                    }
                    e->pl->inviter_dbid = 0; // reset value so someone else can invite
                }
                chatSendToPlayer(stack_tmp_int,localizedPrintf(e,"declineTeamOffer", tmp_str), INFO_USER_ERROR, 0 );
            } break;

        case CMD_TEAM_KICK:
            {
                int kickedID = dbPlayerIdFromName( player_name );
                int interlockCheck;

                if(!e || !e->pl)
                    break;

                interlockCheck = turnstileMapserver_teamInterlockCheck(e, 1);
                if (interlockCheck == 2)
                {
                    chatSendToPlayer(e->db_id, textStd("CantDoThatWhileInQueue"), INFO_USER_ERROR, 0 );
                    break;
                }
                if (interlockCheck != 0)
                {
                    chatSendToPlayer(e->db_id, textStd("CantDoThatAtTheMoment"), INFO_USER_ERROR, 0 );
                    break;
                }

                if( player_name[0] == 0 )
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat", "teamkickString", "playerNameString", "emptyString", "teamkickSynonyms"), INFO_USER_ERROR, 0);
                    break;
                }

                if (db_state.is_endgame_raid && !turnstileMapserver_eventLockedByGroup())
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"EGVRK_PugKickPrevent"), INFO_USER_ERROR, 0);
                    break;
                }

                if( e->pl->taskforce_mode || db_state.is_endgame_raid)
                {
                    START_PACKET( pak, e, SERVER_TASKFORCE_KICK )
                    pktSendString( pak, player_name );
                    pktSendBits( pak, 32,  kickedID );
                    pktSendBits(pak, 1, db_state.is_endgame_raid ? 1 : 0);
                    END_PACKET
                }
                else
                {
                    char msg[64];
                    sprintf( msg, "team_kick_internal %i %s", kickedID, player_name );
                    serverParseClient( msg, client );
                }
            }break;

        case CMD_TEAM_KICK_INTERNAL:
            {
                if(e)
                {
                    //    kick from the league first since the league kick checks if the kicker is the team leader
                    //            which becomes invalid if the team kick is first
                    if (e->league)
                    {
                        char msg[64];
                        sprintf(msg, "league_kick_relay %i %i", stack_tmp_int, e->league->members.leader);
                        serverParseClient(msg, NULL);
                    }
                    team_KickMember( e, stack_tmp_int, player_name );
                }
            }break;

        case CMD_TEAM_KICK_RELAY:
            if(stack_tmp_int<=0)
                return;

            if (relayCommandStart(NULL, stack_tmp_int, stack_tmp_str, &e, &online))
            {
                if (e && e->teamup_id && e->teamup)
                {
                    team_KickRelay(e, stack_tmp_int2);
                }
                relayCommandEnd(&e, &online);
            }
            break;

        case CMD_TEAM_MAP_RELAY:
            e = findEntOrRelayById(NULL, stack_tmp_int, stack_tmp_str, NULL);
            if(e)
            {
                teamSendMapUpdate(e);
            }
            break;

        case CMD_TEAM_QUIT:
            {
                if( e && e->pl && e->pl->taskforce_mode )
                {
                    START_PACKET( pak, e, SERVER_TASKFORCE_QUIT )
                    END_PACKET
                }
                else if (e && e->owned)
                {
                    //    if on endgame raid
                    if (db_state.is_endgame_raid)
                    {
                        //    check for confirmation
                        START_PACKET( pak, e, SERVER_LEAGUE_ER_QUIT );
                        END_PACKET
                    }
                    else
                    {
                        serverParseClient("team_quit_internal", client);
                    }
                }
            }break;

        case CMD_TEAM_QUIT_INTERNAL:
            if( e && e->owned )
            {
                if (e->league_id)
                {
                    sendEntsMsg(CONTAINER_LEAGUES, e->league_id, INFO_SVR_COM, 0, "%s%s", DBMSG_CHAT_MSG, localizedPrintf(e,"hasQuitLeague", e->name));
                }

                league_MemberQuit(e, 1);
                team_MemberQuit(e);
            }
            break;

        case CMD_TEAM_QUIT_RELAY:
            e = findEntOrRelayById(NULL, stack_tmp_int, stack_tmp_str, NULL);
            if (e)
            {
                //    don't quit the league too
                //    we use this to do things like team swaps within a league
                team_MemberQuit(e);
            }
            break;

        case CMD_TF_QUIT_RELAY:
            {
                int online;

                if (stack_tmp_int <= 0) break;
                if (relayCommandStart(NULL, stack_tmp_int, stack_tmp_str, &e, &online))
                {
                    team_MemberQuit(e);
                    relayCommandEnd(&e, &online);
                }
            }break;

        case CMD_TEAM_LFG:
            {
                if( e && e->owned )
                    team_Lfg( e );
            }break;

        case CMD_TEAM_LFG_SET:
            {
                if( e && e->owned )
                    team_Lfg_Set( e, stack_tmp_int );
            }break;

        case CMD_TEAM_BUFF_DISP:
            {
                if( e && e->pl && e->owned )
                    e->pl->teambuff_display = stack_tmp_int;
            }break;

        case CMD_TEAM_CHANGE_LEADER:
            {
                if( e && e->pl )
                {
                    int interlockCheck = turnstileMapserver_teamInterlockCheck(e, 1);
                    if (interlockCheck == 0)
                    {
                        team_changeLeader( e, player_name );
                    }
                    else if (interlockCheck == 2)
                    {
                        chatSendToPlayer(e->db_id, textStd("CantDoThatWhileInQueue"), INFO_USER_ERROR, 0);
                    }
                    else
                    {
                        chatSendToPlayer(e->db_id, textStd("CantDoThatAtTheMoment"), INFO_USER_ERROR, 0);
                    }
                }
            }break;
        case CMD_TEAM_CHANGE_LEADER_RELAY:
            {
                e = findEntOrRelayById(client, stack_tmp_dbid, stack_tmp_str, NULL);
                if (e)
                {
                    team_changeLeader(e, player_name);
                }
            }break;
        case CMD_TEAM_MOVE_CREATE_LEAGUE:
            {
                if( e && e->pl)
                {
                    int movingEntId = dbPlayerIdFromName( player_name );
                    //    make sure ent is the leader of the player, and neither is in a league yet
                    if (team_IsLeader(e, e->db_id) && team_IsMember(e, movingEntId) && !e->league && 
                        !e->pl->inTurnstileQueue)    //    if we're in the queue (the team member is in the queue and can't be moved)
                    {
                        //    if we are creating a new league, push everyone on the team into the league
                        int i;
                        int movingSwapLock = 0;
                        //    kick this person off the team (he will still be in the league)
                        team_KickMember(e, movingEntId, player_name);
                        movingSwapLock |= e->pl && e->pl->taskforce_mode;
                        movingSwapLock |= e->teamup && e->teamup->teamSwapLock;
                        if (!league_AcceptOffer(e, movingEntId, movingEntId, movingSwapLock))
                        {
                            team_leaveOrRelay(client, movingEntId);
                        }

                        if (e->teamup)
                        {
                            for (i = (e->teamup->members.count-1); i >= 0; --i)
                            {
                                if ((e->teamup->members.ids[i] != e->teamup->members.leader) &&
                                    (e->teamup->members.ids[i] != movingEntId))

                                {
                                    if (!league_AcceptOffer(e, e->teamup->members.ids[i], e->teamup->members.leader, movingSwapLock))
                                    {
                                        team_leaveOrRelay(client, e->teamup->members.ids[i]);
                                    }
                                }
                            }
                        }
                    }
                }
            }break;
            //-------------------------------------------------------------------------------------------------
            // SUPERGROUPS ////////////////////////////////////////////////////////////////////////////////////
            //-------------------------------------------------------------------------------------------------

        case CMD_SUPERGROUP_INVITE:
            {
                char buf[256];
                int online;
                Entity * player;

                int id = dbPlayerIdFromName(player_name);

                if( !e )
                    break;

                if( player_name[0] == 0 )
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
                        "sginviteString", "playerNameString", "emptyString", "sginviteSynonyms"), INFO_USER_ERROR, 0);
                    break;
                }

                if( !e->supergroup_id )
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"CouldNotActionPlayerReason", "InviteString", player_name, "NotInSuperGroup"), INFO_USER_ERROR, 0);
                    break;
                }

                player = entFromDbId( id );
                online = player_online(player_name);

                if (id < 0 ||                            // Character doesn't exist or
                    (!player && !online) ||                // He's not online and he's not on this mapserver or
                    (player && player->logout_timer) ||    // He's on this mapserver, and online, but he's currently marked as "quitting"
                    (player && player->pl->hidden&(1<<HIDE_INVITE)) ) 
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"playerNotFound", player_name), INFO_USER_ERROR, 0);
                    break;
                }

                if (player && player->pl && player->pl->declineSuperGroupInvite)
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"AutoDeclineSgroupOffer"), INFO_USER_ERROR, 0);
                    break;
                }

                if( e->db_id == id ) // trying to invite himself
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotInviteYourselfSG"), INFO_USER_ERROR, 0);
                    break;
                }

                if( !sgroup_hasPermission( e, SG_PERM_INVITE) ) // not high enough rank to invite
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"CouldNotActionPlayerReason", "InviteString", player_name, "NotHighEnoughRank"), INFO_USER_ERROR, 0);
                    break;
                }

                if( e->supergroup->members.count >= MAX_SUPER_GROUP_MEMBERS ) // no room in supergroup
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"CouldNotActionPlayerReason", "InviteString", player_name, "SuperGroupIsFull"), INFO_USER_ERROR, 0);
                    break;
                }

                if( entIsTrial(player) ) // Trial accounts can't join SGs
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotInviteTrialAccountSG"), INFO_USER_ERROR, 0);
                    break;
                }

                // Praetorians can't join SGs until they come to Primal Earth.
                if( ENT_IS_IN_PRAETORIA(player) )
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotInvitePraetorianSG"), INFO_USER_ERROR, 0);
                    break;
                }

                sprintf( buf, "%s \"%s\" \"%s\" %i %i", "sginvite_long", player_name, e->name, e->db_id, e->supergroup->playerType);
                serverParseClient( buf, NULL );

            }break;

        case CMD_SUPERGROUP_INVITE_LONG:
            {
                int result = 0;
                Entity* ent = findEntOrRelay( NULL, player_name, stack_tmp_str, &result );

                if( ent )
                {
                    int id = dbPlayerIdFromName(tmp_str);
                    if( ent->supergroup_id ) // if player is already in a SG forget it
                    {
                        chatSendToPlayer( id, localizedPrintf(ent,"CouldNotActionPlayerReason", "InviteString", player_name, "PlayerIsAlreadyInSuperGroup"), INFO_USER_ERROR, 0 );
                        break;
                    }

                    if( isIgnored( ent, stack_tmp_dbid ) )
                    {
                        char *buf = localizedPrintf( ent, "CouldNotActionPlayerReason", "InviteString", player_name, "PlayerHasIgnoredYou" );
                        chatSendToPlayer( stack_tmp_dbid, buf , INFO_USER_ERROR, 0 );
                        break;
                    }

                    if (ent->pl && ent->pl->declineSuperGroupInvite)
                    {
                        chatSendToPlayer(id, localizedPrintf(ent,"AutoDeclineSgroupOffer"), INFO_USER_ERROR, 0);
                        break;
                    }

                    if( entIsTrial(ent) ) // Trial accounts can't join SGs
                    {
                        chatSendToPlayer(id, localizedPrintf(ent,"CannotInviteTrialAccountSG"), INFO_USER_ERROR, 0);
                        break;
                    }

                    // Praetorians can't join SGs until they come to Primal Earth.
                    if( ENT_IS_IN_PRAETORIA(ent) )
                    {
                        chatSendToPlayer(id, localizedPrintf(ent,"CannotInvitePraetorianSG"), INFO_USER_ERROR, 0);
                        break;
                    }

                    if( ent->pl->inviter_dbid ) // already being invited to another team
                    {
                        char *buf = localizedPrintf( ent, "CouldNotActionPlayerReason", "InviteString", player_name, "PlayerIsConsideringAnotherOffer" );
                        chatSendToPlayer( id, buf, INFO_USER_ERROR, 0 );
                        break;
                    }

                    if ( ent->pl->last_inviter_dbid == stack_tmp_dbid && (dbSecondsSince2000()-ent->pl->last_invite_time) < BLOCK_INVITE_TIME  )
                    {
                        char *buf = localizedPrintf( ent, "CouldNotActionPlayerReason", "InviteString", player_name, "MustWaitLonger" );
                        chatSendToPlayer( id, buf, INFO_USER_ERROR, 0 );
                        break;
                    }

                    if( ent->pl->playerType != tmp_int) // Hero SGs can't invite villains
                    {
                        chatSendToPlayer(id, localizedPrintf(ent,"CouldNotActionPlayerReason", "InviteString", player_name, "CantSGInviteEnemy"), INFO_USER_ERROR, 0);
                        break;
                    }

                    {
                        char *buf = localizedPrintf( ent, "SuperInviteSent" );
                        chatSendToPlayer( stack_tmp_dbid, buf , INFO_SVR_COM, 0 );
                    }

                    ent->pl->inviter_dbid = ent->pl->last_inviter_dbid =stack_tmp_dbid; // remeber the inviter, so we can trap bad commands
                    ent->pl->last_invite_time = dbSecondsSince2000();

                    START_PACKET( pak, ent, SERVER_SUPERGROUP_OFFER )
                    pktSendBits( pak, 32, stack_tmp_dbid );
                    pktSendString( pak, tmp_str );
                    END_PACKET
                }

            }break;

		case CMD_SUPERGROUP_INVITE_ALT:
			{
				char buf[256];
				int online;
				Entity * player;

				int id = dbPlayerIdFromName(player_name);
																					  
				if( !e )														 
					break;
					 
				if( player_name[0] == 0 )
				{
					chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
						"sginviteString", "playerNameString", "emptyString", "sginviteSynonyms"), INFO_USER_ERROR, 0);
					break;
				}

				if( !e->supergroup_id )
				{
					chatSendToPlayer(e->db_id, localizedPrintf(e,"CouldNotActionPlayerReason", "InviteString", player_name, "NotInSuperGroup"), INFO_USER_ERROR, 0);
					break;
				}

				if (id < 0)							  
				{
					chatSendToPlayer(e->db_id, localizedPrintf(e,"playerNotFound", player_name), INFO_USER_ERROR, 0);
					break;
				}

				if( e->db_id == id ) // trying to invite himself
				{
					chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotInviteYourselfSG"), INFO_USER_ERROR, 0);												
					break;
				}

				if( !sgroup_hasPermission( e, SG_PERM_INVITE) ) // not high enough rank to invite
				{
					chatSendToPlayer(e->db_id, localizedPrintf(e,"CouldNotActionPlayerReason", "InviteString", player_name, "NotHighEnoughRank"), INFO_USER_ERROR, 0);												
					break;
				}

				if( e->supergroup->members.count >= MAX_SUPER_GROUP_MEMBERS ) // no room in supergroup
				{
					chatSendToPlayer(e->db_id, localizedPrintf(e,"CouldNotActionPlayerReason", "InviteString", player_name, "SuperGroupIsFull"), INFO_USER_ERROR, 0);
					break;
				}

				player = entFromDbId( id );
				online = player_online(player_name);
																				  
				if (player || online)
				{
					chatSendToPlayer(e->db_id, "You cannot use this command to invite a character that is online!", INFO_USER_ERROR, 0);
					break;
				}

				sprintf( buf, "%s %i %i %i %i %i", "sg_alt_relay", id, e->supergroup_id, e->supergroup->playerType, e->db_id, e->auth_id);
				serverParseClient( buf, NULL );
												  
			}break;

        case CMD_SUPERGROUP_COSTUME:
            {
                int result = 0;
                Entity* ent = findEntOrRelay( NULL, player_name, stack_tmp_str, &result );

                if( ent )
                    sgroup_setSuperCostume( ent, 1 );

            }break;

        case CMD_SUPERGROUP_INVITE_ACCEPT:
            {
                Entity *inviter;
                int result = 0;

                if( e && e->pl ) // first make sure the inviter is the guy who actually invited
                {
                    if ( e->pl->inviter_dbid != stack_tmp_int )
                    {
                        // TODO: Mark player is suspicious...and take away their candy
                        e->pl->inviter_dbid = 0;
                        break;
                    }

                    // Just in case we somehow get invited, make sure trial accounts don't accept
                    if ( entIsTrial(e) )
                    {
                        chatSendToPlayer( e->db_id, localizedPrintf( e, "CannotAcceptTrialAccountSG" ), INFO_USER_ERROR, 0 );
                        break;
                    }

                    // Similarly, make sure we can't join if we're a Praetorian who hasn't yet come to Primal Earth.
                    if ( ENT_IS_IN_PRAETORIA(e) )
                    {
                        chatSendToPlayer( e->db_id, localizedPrintf( e, "CannotAcceptPraetorianSG" ), INFO_USER_ERROR, 0 );
                        break;
                    }

                    e->pl->inviter_dbid = e->pl->last_inviter_dbid = 0; // reset value so someone else can invite
                }

                inviter = findEntOrRelay( client, player_name, stack_tmp_str, &result ); // start cross mapserver search

                if( inviter )
                    sgroup_AcceptOffer( inviter, stack_tmp_dbid, tmp_str );

            }break;

        case CMD_SUPERGROUP_ACCEPT_RELAY:
            if(stack_tmp_int<=0)
                return;

            if (relayCommandStart(NULL, stack_tmp_int, stack_tmp_str, &e, &online))
            {
                sgroup_AcceptRelay(e, stack_tmp_int2, stack_tmp_int3);
                relayCommandEnd(&e, &online);
            }
            break;

		case CMD_SUPERGROUP_ALT_RELAY:
			if(stack_tmp_int<=0)
				return;

			if (relayCommandStart(NULL, stack_tmp_int, stack_tmp_str, &e, &online))
			{
				sgroup_AltRelay(e, stack_tmp_int2, stack_tmp_int3, stack_tmp_int4, stack_tmp_int5);
				relayCommandEnd(&e, &online);
			}
			break;

        case CMD_SUPERGROUP_DEBUG_JOIN:
            sgroup_JoinDebug(e, tmp_str);
            break;

        case CMD_SUPERGROUP_INVITE_DECLINE:
            {
                if( e && e->pl ) // first make sure the inviter is the guy who actually invited
                {
                    if ( e->pl->inviter_dbid != stack_tmp_int )
                    {
                        // TODO: Mark player is suspicious...and take away their candy
                        e->pl->inviter_dbid = 0;
                        break;
                    }
                    e->pl->inviter_dbid = 0; // reset value so someone else can invite
                }
                chatSendToPlayer(stack_tmp_int,localizedPrintf(e,"declineSgroupOffer", tmp_str), INFO_USER_ERROR, 0 );
            } break;

        case CMD_SUPERGROUP_KICK:
            {
                if( !e )
                    break;

                if( player_name[0] == 0 )
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
                        "sgkickString", "playerNameString", "emptyString", "sgkickSynonyms"), INFO_USER_ERROR, 0);
                    break;
                }

                sgroup_KickMember( e, player_name );
            }break;

        case CMD_SUPERGROUP_KICK_RELAY:
            if(stack_tmp_int<=0)
                return;

            if (relayCommandStart(NULL, stack_tmp_int, stack_tmp_str, &e, &online))
            {
                sgroup_KickRelay(e, stack_tmp_int2);
                relayCommandEnd(&e, &online);
            }
            break;

        case CMD_SUPERGROUP_QUIT:
            {
                if(e && e->owned)
                    sgroup_MemberQuit( e );
            }break;

        case CMD_SUPERGROUP_GET_STATS:
            {
                if(e)
                    sgroup_getStats( e );
            }break;

        case CMD_SUPERGROUP_GET_STATS_RELAY:
            {
                e = findEntOrRelayById(NULL, stack_tmp_int, stack_tmp_str, NULL);
                if (e)
                {
                    sgroup_getStats(e);
                }
            }break;

        case CMD_SUPERGROUP_PROMOTE:
        case CMD_SUPERGROUP_DEMOTE:
            {
                char buf[256];

                if(!e)
                    break;

                if( player_name[0] == 0 )
                {
                    if (cmd->num == CMD_SUPERGROUP_PROMOTE)
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
                            "sgpromoteString", "newRankString", "emptyString", "sgpromoteSynonyms" ), INFO_USER_ERROR, 0);
                    else if (cmd->num == CMD_SUPERGROUP_DEMOTE)
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat", 
                            "sgdemoteString", "newRankString", "emptyString", "sgdemoteSynonyms"), INFO_USER_ERROR, 0);
                    break;
                }

                if (cmd->num == CMD_SUPERGROUP_PROMOTE && sgroup_rank(e, e->db_id) == NUM_SG_RANKS - 2)
                {
                    int i;
                    int targetID;
                    int targetRank;
                    char *complaint;

                    targetID = dbPlayerIdFromName(player_name);
                    targetRank = sgroup_rank(e, targetID);
                    if (targetRank == NUM_SG_RANKS - 3)
                    {
                        for (i = eaSize(&e->sgStats) - 1; i >= 0; i--)
                        {
                            if (e->sgStats[i]->db_id == targetID && e->supergroup != NULL && e->sgStats[i]->playerType != e->supergroup->playerType)
                            {
                                // I only break; here, the complaint comes in a bit.  This is because C does not support a "break 2;" statement
                                // to break out of two control structures, i.e. this inner for loop and the outer switch.  The subsequent test
                                // of i >= 0 will trip if and only if this break; happened.
                                break;
                            }
                        }

                        // If the loop above detected the player was dark, complain about this and bail out.
                        if (i >= 0)
                        {
                            complaint = e->sgStats[i]->playerType == kPlayerType_Villain ? "CantPromoteDarkVillain" : "CantPromoteDarkHero";
                            chatSendToPlayer(e->db_id, textStd(complaint), INFO_USER_ERROR, 0);
                            break;
                        }
                        sgroup_startConfirmation(e, player_name);
                        break;
                    }
                }

                if ( e->access_level < 2 )
                {
                    if( !sgroup_IsMember(e, e->db_id) ) // verify the demote even has a team
                        break;

                    if (!sgroup_hasPermission(e, (cmd->num == CMD_SUPERGROUP_PROMOTE ? SG_PERM_PROMOTE : SG_PERM_DEMOTE)))
                        break;

                    sprintf( buf, "promote_long %i %i %i %i \"%s\"", e->supergroup_id, sgroup_rank(e, e->db_id), (cmd->num == CMD_SUPERGROUP_PROMOTE)?1:-1, e->db_id, player_name );
                }
                else
                    sprintf( buf, "csr_promote_long %i %i %i %i \"%s\"", e->supergroup_id, sgroup_rank(e, e->db_id), (cmd->num == CMD_SUPERGROUP_PROMOTE)?1:-1, e->db_id, player_name );

                serverParseClient( buf, NULL );
                sgroup_getStats(e);
            }break;

        // BH: i didnt want to have to duplicate code here, so these two commands share logic until they need differentiating
        case CMD_SUPERGROUP_PROMOTE_LONG:
        case CMD_SUPERGROUP_CSR_PROMOTE_LONG:
            {
                int result = 0;
                Entity *ent = findEntOrRelay( client, player_name, stack_tmp_str, &result );
            
                if( !ent && !result && client && client->entity )
                {
                    chatSendToPlayer( stack_tmp_dbid, localizedPrintf( client->entity, "CouldNotActionPlayerReason", "ChangeRankString", player_name, "PlayerIsNotOnline" ),
                                        INFO_USER_ERROR, 0);
                }

                if( ent )
                {
                    if (cmd->num == CMD_SUPERGROUP_PROMOTE_LONG)
                        sgroup_promote( ent, stack_tmp_dbid, stack_tmp_int, stack_tmp_int2, stack_tmp_int3 );
                    else
                        sgroup_promoteCsr( ent, stack_tmp_dbid, stack_tmp_int, stack_tmp_int2, stack_tmp_int3 );
                }

            }break;

        case CMD_SUPERGROUP_SET_DEFAULTS:
            {
                int result = 0;
                Entity *e = findEntOrRelay( client, player_name, stack_tmp_str, &result );

                if( e )
                    sgroup_initiate( e );
            }break;

        case CMD_SUPERGROUP_DEBUG_CREATE:
            {
                if( e && tmp_str[0] != '\0' )
                    sgroup_CreateDebug( e, tmp_str );
            }break;

        case CMD_SUPERGROUP_REGISTRAR:
            {
                if(e)
                    serverParseClient( "contactDialog supergroupregistrar", client );
            }break;

        case CMD_SUPERGROUP_RENAME_LEADER:
            {
                if( e )
                {
                    int rank = 4;
                    sgroup_setGeneric( e, SG_PERM_RANKS, sgroup_genRenameRank, &rank, tmp_str, "RenameRank" );
                }
            }break;
        case CMD_SUPERGROUP_RENAME_COMMANDER:
            {
                if( e )
                {
                    int rank = 3;
                    sgroup_setGeneric( e, SG_PERM_RANKS, sgroup_genRenameRank, &rank, tmp_str, "RenameRank" );
                }
            }break;
        case CMD_SUPERGROUP_RENAME_CAPTAIN:
            {
                if( e )
                {
                    int rank = 2;
                    sgroup_setGeneric( e, SG_PERM_RANKS, sgroup_genRenameRank, &rank, tmp_str, "RenameRank" );
                }
            }break;
        case CMD_SUPERGROUP_RENAME_LIEUTENANT:
            {
                if( e )
                {
                    int rank = 1;
                    sgroup_setGeneric( e, SG_PERM_RANKS, sgroup_genRenameRank, &rank, tmp_str, "RenameRank" );
                }
            }break;
        case CMD_SUPERGROUP_RENAME_MEMBER:
            {
                if( e )
                {
                    int rank = 0;
                    sgroup_setGeneric( e, SG_PERM_RANKS, sgroup_genRenameRank, &rank, tmp_str, "RenameRank" );
                }
            }break;

        case CMD_SUPERGROUP_MOTD_SET:
            {
                if( e && tmp_str[0] != '\0' )
                    sgroup_setGeneric( e, SG_PERM_MOTD, sgroup_genSetMOTD, tmp_str, NULL, "SetMOTD");
            }break;
        case CMD_SUPERGROUP_MOTTO_SET:
            {
                if( e && tmp_str[0] != '\0' )
                    sgroup_setGeneric( e, SG_PERM_MOTTO, sgroup_genSetMotto, tmp_str, NULL, "SetMotto");
            }break;
        case CMD_SUPERGROUP_DESCRIPTION_SET:
            {
                if( e && tmp_str[0] != '\0' )
                    sgroup_setGeneric( e, SG_PERM_DESCRIPTION, sgroup_genSetDescription, tmp_str, NULL, "SetDescription" );
            }break;
//        case CMD_SUPERGROUP_TITHE_SET:
//            {
//                if( e )
//                    sgroup_setGeneric( e, SG_PERM_TITHING, sgroup_genSetTithe, &tmp_int, NULL, "SetTithe" );
//            }break;
        case CMD_SUPERGROUP_DEMOTETIMEOUT_SET:
            {
                if( e )
                    sgroup_setGeneric( e, SG_PERM_AUTODEMOTE, sgroup_genSetDemoteTimeout, &tmp_int, NULL, "SetDemoteTimeout" );
            }break;

        case CMD_SUPERGROUP_MODE:
            {
                 if( e )
                    sgroup_ToggleMode(e);
            }break;

        case CMD_SUPERGROUP_MODE_SET:
            {
                if( e )
                    sgroup_SetMode(e, stack_tmp_int, 0);
            }break;

        case CMD_SUPERGROUP_WHO:
            {
                if( e )
                    sgroup_CsrWho(e, 0);
            }break;

        case CMD_SUPERGROUP_WHO_LEADER:
            {
                if( e )
                    sgroup_CsrWho(e, 1);
            }break;

        case CMD_SUPERGROUP_GET_REFRESH_RELAY:
            {
                e = entFromDbIdEvenSleeping(stack_tmp_int);
                if (e)
                    sgroup_RefreshClient(e);
            }break;

        case CMD_SUPERGROUP_RENAME_RANK:
            {
                if( e && e->supergroup )
                {
                    sgroup_setGeneric(e, 0, sgroup_genRenameRank, &tmp_int, tmp_str, "RenameRank");
                }
            }break;

        case CMD_SUPERGROUP_RANK_PRINT:
            if(e && e->supergroup)
            {
                int i, j;
                
                for(i = 0; i < NUM_SG_RANKS_VISIBLE; i++)
                {
                    conPrintf(client, "%i %s", i, e->supergroup->rankList[i].name);

                    for(j = 0; j < SG_PERM_COUNT; j++)
                    {
                        if(sgroupPermissions[j].used && TSTB(e->supergroup->rankList[i].permissionInt, j))
                            conPrintf(client, " %s", localizedPrintf(e,sgroupPermissions[j].name));
                    }
                }
            }break;


        //------------------------------------------------------------------------------------------------
        // ALLIANCE //////////////////////////////////////////////////////////////////////////////////////
        //------------------------------------------------------------------------------------------------
        case CMD_ALLIANCE_INVITE:
            {
                char *buf = 0;
                char buffer[512];

                if(!e || !e->pl)
                    break;

                if (!player_name[0])
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat", 
                        "/coalition_invite", "player_name", " ", "/ci") , INFO_USER_ERROR, 0);
                    break;
                }
    
                buf = alliance_CheckInviterConditions( e, player_name );

                if(buf)
                {
                    chatSendToPlayer(e->db_id, buf, INFO_USER_ERROR, 0);
                    break;
                }

                sprintf( buffer, "%s \"%s\" \"%s\" %d %d \"%s\" %d", "coalition_invite_relay", player_name, e->name, e->db_id, e->supergroup_id, e->supergroup->name, e->pl->playerType);
                serverParseClient( buffer, NULL );

            }break;

        case CMD_ALLIANCE_INVITE_RELAY:
            {
                int result = 0;
                Entity* ent = findEntOrRelay( NULL, player_name, stack_tmp_str, &result );
                char *leader_name;

                // stack_tmp_str:    original command ("coalition_invite_relay ...")
                // player_name:        invitee name
                // tmp_str:            inviter name
                // stack_tmp_dbid:    inviter dbid
                // stack_tmp_int:    inviter supergroup dbid
                // tmp_str2:        inviter supergroup name
                // stack_tmp_int2:  inviter supergroup playertype

                if( ent )
                {
                    char * buf = alliance_CheckInviteeConditions( ent, stack_tmp_dbid, stack_tmp_int, stack_tmp_int2);

                    if(buf)
                    {
                        chatSendToPlayer(stack_tmp_dbid, buf, INFO_USER_ERROR, 0);
                        break;
                    }

                    leader_name = sgroup_FindLongestWithPermission(ent, SG_PERM_ALLIANCE);
                    if (!leader_name || !leader_name[0])
                    {
                        chatSendToPlayer(stack_tmp_dbid, localizedPrintf(ent,"NoLeaderOnline"), INFO_USER_ERROR, 0);
                        break;
                    }

                    // if this is not the leader, relay the command to the leader
                    if (strcmp(leader_name, ent->name)!=0)
                    {
                        char buffer[1024];
                        sprintf( buffer, "%s \"%s\" \"%s\" %d %d \"%s\" %d", "coalition_invite_relay", leader_name, tmp_str, stack_tmp_dbid, stack_tmp_int, tmp_str2, stack_tmp_int2);
                        serverParseClient( buffer, NULL );
                        break;
                    }


                    if( isIgnored( ent, stack_tmp_dbid ) )
                    {
                        buf = localizedPrintf( ent, "CouldNotActionPlayerReason", "InviteString", player_name, "PlayerHasIgnoredYou" );
                    }
                    else if( ent->pl->inviter_dbid ) // already being invited to another team
                    {
                        buf = localizedPrintf( ent, "CouldNotActionPlayerReason", "InviteString", player_name, "PlayerIsConsideringAnotherOffer" );
                    }

                    if(buf)
                    {
                        chatSendToPlayer(stack_tmp_dbid, buf, INFO_USER_ERROR, 0);
                        break;
                    }

                    ent->pl->inviter_dbid = stack_tmp_dbid; // remeber the inviter, so we can trap bad commands

                    chatSendToPlayer( stack_tmp_dbid, localizedPrintf( ent, "AllyInviteSent" ) , INFO_SVR_COM, 0 );

                    START_PACKET( pak, ent, SERVER_ALLIANCE_OFFER )
                        pktSendBits( pak, 32, stack_tmp_dbid );    // inviter dbid
                        pktSendBits( pak, 32, stack_tmp_int );    // inviter sg dbid
                        pktSendString( pak, tmp_str );            // inviter name
                        pktSendString( pak, tmp_str2 );            // inviter sg name
                    END_PACKET
                }
                else if (!result)
                {
                    chatSendToPlayer(stack_tmp_dbid, localizedPrintf(0, "playerNotFound", player_name), INFO_USER_ERROR, 0);
                }
            }break;

        case CMD_ALLIANCE_ACCEPT:
            {
                // stack_tmp_str:    original command ("coalition_accept ...")
                // player_name:        inviter(acceptee) name
                // stack_tmp_dbid:    inviter(acceptee) dbid
                // stack_tmp_int:    inviter(acceptee) supergroup dbid

                char *buf = 0;
                char buffer[1024];

                if (!e || !e->pl)
                    break;

                buf = alliance_CheckAccepterConditions(e, stack_tmp_dbid, player_name);
                
                e->pl->inviter_dbid = e->pl->last_inviter_dbid = 0;
                svrSendEntListToDb(&e, 1);

                if (buf)
                {
                    chatSendToPlayer(e->db_id, buf, INFO_USER_ERROR, 0);
                    break;
                }

                sprintf( buffer, "%s \"%s\" \"%s\" %d %d %d", "coalition_accept_relay", player_name, e->name, e->db_id, e->supergroup_id, e->pl->playerType);
                serverParseClient( buffer, NULL );

            }break;

        case CMD_ALLIANCE_ACCEPT_RELAY:
            {
                int result = 0;
                Entity* ent = findEntOrRelay( NULL, player_name, stack_tmp_str, &result );
                // stack_tmp_str:    original command ("coalition_accept_relay ...")
                // player_name:        inviter(acceptee) name
                // tmp_str:            invitee(accepter) name
                // stack_tmp_dbid:    invitee(accepter) dbid
                // stack_tmp_int:    invitee(accepter) supergroup dbid
                // stack_tmp_int2:  invitee(accepter) supergroup playertype

                if(ent)
                {
                    char * buf = alliance_CheckAccepteeConditions( ent, stack_tmp_dbid, stack_tmp_int, stack_tmp_int2);
                    if(buf)
                    {
                        chatSendToPlayer(stack_tmp_dbid, buf, INFO_USER_ERROR, 0);
                        break;
                    }

                    alliance_FormAlliance(ent->supergroup_id, stack_tmp_int);
                    chatSendToPlayer(stack_tmp_dbid, localizedPrintf(ent,"AllianceFormed", player_name), INFO_SVR_COM, 0);
                    chatSendToPlayer(ent->db_id, localizedPrintf(ent,"AllianceFormed", tmp_str), INFO_SVR_COM, 0);
                }
                else if (!result)
                    chatSendToPlayer(stack_tmp_dbid, localizedPrintf(0,"playerNotFound", player_name), INFO_USER_ERROR, 0);

            }break;

        case CMD_ALLIANCE_DECLINE:
            {
                // stack_tmp_str:    original command ("coalition_decline ...")
                // player_name:        inviter(acceptee) name
                // stack_tmp_dbid:    inviter(acceptee) dbid
                // tmp_str:            invitee(accepter) name
                if (e && e->pl)
                {
                    if (e->pl->inviter_dbid != stack_tmp_dbid)
                    {
                        // TODO: Mark player is suspicious...and take away their candy
                        e->pl->inviter_dbid = 0;
                        break;
                    }
                    e->pl->inviter_dbid = 0; // reset value so someone else can invite
                }
                chatSendToPlayer(stack_tmp_int,localizedPrintf(e,"declineAllianceOffer", tmp_str), INFO_USER_ERROR, 0 );

            } break;

        case CMD_ALLIANCE_CANCEL:
            {
                // stack_tmp_str:    original command ("coalition_cancel ...")
                // tmp_str_cmd:        supergroup name to cancel with

                // DGNOTE 11/4/2008
                // How the hell did this ever work?  Prior to this change, it required the db_id 
                // of the SG you were cancelling with.  Which will be totally unknown to your
                // average player.

                char *buf = 0;
                int id;
                int map_num;
                int i;
                
                if (e == NULL || e->pl == NULL || e->supergroup == NULL)
                {
                    break;
                }

                id = dbSyncContainerFindByElement(CONTAINER_SUPERGROUPS,"Name",tmp_str,&map_num,0);

                if (id <= 0)
                {
                    // Didn't find the SG using normal DB lookup.  This means it's defunct, and we have a "dead" alliance.
                    // Drop back 10 and punt at this point - try and look it up in the local copy of the supergroup
                    // structure.
                    for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
                    {
                        if (stricmp(e->supergroup->allies[i].name, tmp_str) == 0)
                        {
                            // If we ever reach here, things are really badly broken.  In the name of speed and a quiet life, I'm not going to check permissions
                            // or anything, just remove the dead SG from the allies[] array.
                            alliance_CancelDefunctAlliance(e->supergroup_id, i);
                            chatSendToPlayer(e->db_id, localizedPrintf(e, "AllianceBrokenWith", tmp_str ), INFO_SVR_COM, 0);
                            break;
                        }
                    }
                }
                if (id <= 0)
                {
                    if (i >= MAX_SUPERGROUP_ALLIES)
                    {
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"NotYourAlly"), INFO_USER_ERROR, 0);
                    }
                    break;
                }

                buf = alliance_CheckCancelerConditions(e, id);
                if (buf)
                {
                    chatSendToPlayer(e->db_id, buf, INFO_USER_ERROR, 0);
                    break;
                }

                alliance_CancelAlliance(e->supergroup_id, id);
                chatSendToPlayer(e->db_id, localizedPrintf(e, "AllianceBrokenWith", tmp_str ), INFO_SVR_COM, 0);
            } break;

        case CMD_ALLIANCE_SG_MINTALKRANK:
            {
                char * buf;
                // stack_tmp_str:    original command ("coalition_sg_mintalkrank ...")
                // stack_tmp_int:    min talk rank
                if (!e || !e->pl)
                    break;

                buf = alliance_SetMinTalkRank(e, stack_tmp_int);
                if (buf)
                {
                    chatSendToPlayer(e->db_id, buf, INFO_USER_ERROR, 0);
                    break;
                }
                chatSendToPlayer(e->db_id, localizedPrintf(e,"AllianceMinTalkRankSet", stack_tmp_int), INFO_SVR_COM, 0);
            } break;

        case CMD_ALLIANCE_MINTALKRANK:
            {
                // stack_tmp_str:    original command ("coalition_mintalkrank ...")
                // stack_tmp_dbid:    ally dbid
                // stack_tmp_int:    min talk rank

                char *buf = 0;

                if (!e || !e->pl)
                    break;

                buf = alliance_SetAllyMinTalkRank(e, stack_tmp_dbid, stack_tmp_int);
                if (buf)
                {
                    chatSendToPlayer(e->db_id, buf, INFO_USER_ERROR, 0);
                    break;
                }
                chatSendToPlayer(e->db_id, localizedPrintf(e,"AllianceAllyMinTalkRankSet", stack_tmp_int) , INFO_SVR_COM, 0);

            } break;

        case CMD_ALLIANCE_NOSEND:
            {
                // stack_tmp_str:    original command ("coalition_nosend ...")
                // stack_tmp_dbid:    ally dbid
                // stack_tmp_int:    nosend

                char *buf = 0;

                if (!e || !e->pl)
                    break;

                buf = alliance_SetAllyNoSend(e, stack_tmp_dbid, stack_tmp_int);
                if (buf)
                {
                    chatSendToPlayer(e->db_id, buf, INFO_USER_ERROR, 0);
                    break;
                }
                chatSendToPlayer(e->db_id, localizedPrintf(e,stack_tmp_int?"AllianceAllyNoSendSet":"AllianceAllyNoSendUnset"), INFO_SVR_COM, 0);

            } break;

            //------------------------------------------------------------------------------------------------
            // LEAGUES ////////////////////////////////////////////////////////////////////////////////////////
            //-------------------------------------------------------------------------------------------------
        case CMD_LEAGUE_INVITE:
            {
                char *buf = 0;
                char command[512];
                char taskdesc[100] = {0};
                Entity *inviter = e; // this is for readability's sake
                char *invitee_name = player_name; // this is for readability's sake
                int disallowedBitfield = 0;
                int remainingSpots = MAX_LEAGUE_MEMBERS;
                int i;
                int numTeams = league_teamCount(inviter->league);

                if (!inviter || !inviter->pl)
                    break;

                if (db_state.is_endgame_raid)
                {
                    if (inviter->pl->lastTurnstileEventID)
                    {
                        sprintf(command, "tut_invite %s", player_name);
                        serverParseClientEx(command, client, ACCESS_INTERNAL);
                    }
                    else
                    {
                        chatSendToPlayer(inviter->db_id, "TUTInvalidInstance", INFO_USER_ERROR, 0);
                    }
                    break;
                }

                buf = league_CheckInviterConditions(inviter, invitee_name, &disallowedBitfield);

                // Don't add checks here, add to league_CheckInviterConditions or common_CheckInviterConditions!

                if (buf)
                {
                    if (inviter)
                    {
                        chatSendToPlayer(inviter->db_id, buf, INFO_USER_ERROR, 0);
                    }
                    break;
                }

                for (i = 0; i < numTeams; ++i)
                {
                    remainingSpots -= league_getTeamLockStatus(inviter->league, i) ? MAX_TEAM_MEMBERS : league_getTeamMemberCount(inviter->league, i);
                }
                devassert(remainingSpots >= 0);

                strcpy(taskdesc, "0:0:0:0:0");        //    for now this is 0:0:0, but if we ever add league missions...

                //    if the format here changes, update the relayed portion as well (that bounces to the leader)
                sprintf(command, "%s \"%s\" \"%s\" %i %s %i %i %i %i %i %i %i", "leagueinvite_long", invitee_name, 
                    inviter->name, inviter->db_id, taskdesc, inviter->pchar->playerTypeByLocation, 
                    disallowedBitfield, inviter->pl->praetorianProgress, RaidIsRunning(), inviter->supergroup_id, 
                    remainingSpots, numTeams );
                serverParseClient( command, NULL );

            }break;
        case CMD_LEAGUE_INVITE_LONG:
            {
                int result = 0;
                int inviter_playerTypeByLocation = stack_tmp_int;
                int disallowedBitfield = stack_tmp_int2;
                int inviter_praetorianProgress = stack_tmp_int3;
                int raid = stack_tmp_int4;
                int inviterSGID = stack_tmp_int5;
                int inviterDbID = stack_tmp_dbid;
                int remainingSpots = stack_tmp_int6;
                int numTeams = stack_tmp_int7;
                Entity* invitee = findEntOrRelay(NULL, player_name, stack_tmp_str, &result);
                int type = 0; // 0 - regular invite, 1 - own a mission warning, 2 - ok box because on a mission,
                char *inviteeName = player_name;

                //    If the person we invited is in a team, don't really ask them, ask the team leader instead since they represent the team
                if (invitee && invitee->teamup_id && invitee->teamup && (invitee->teamup->members.count > 1) )
                {
                    int i;
                    char *leaderName = NULL;
                    for (i = 0; i < invitee->teamup->members.count; ++i)
                    {
                        if (invitee->teamup->members.ids[i] == invitee->teamup->members.leader)
                        {
                            leaderName = invitee->teamup->members.names[i];
                            break;
                        }
                    }
                    if (leaderName)
                    {
                        //    if the format here changes, update the relayed portion as well (that bounces to the leader)
                        char *replaceStr = NULL;
                        estrConcatf(&replaceStr, "leagueinvite_long \"%s\" \"%s\" %i %s %i %i %i %i %i %i %i", leaderName, tmp_str, 
                            stack_tmp_dbid, tmp_taskdesc, stack_tmp_int, stack_tmp_int2, stack_tmp_int3, stack_tmp_int4, 
                            stack_tmp_int5,    stack_tmp_int6, stack_tmp_int7);
                        invitee = findEntOrRelay(NULL, leaderName, replaceStr, &result);
                        estrDestroy(&replaceStr);
                        if (invitee)
                        {
                            inviteeName = invitee->name;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }

                // should we verify this person can actually offer here? Does it matter?
                if (invitee)
                {
                    char * buf = league_CheckInviteeConditions(invitee, stricmp(inviteeName, player_name) ? localizedPrintf(invitee, "playerLeader", player_name, inviteeName) : player_name, tmp_taskdesc, &type, inviterDbID, tmp_str, disallowedBitfield, inviter_praetorianProgress, inviter_playerTypeByLocation, remainingSpots, numTeams);

                    // Is this deprecated?
                    if ((raid || RaidIsRunning()) && inviterSGID != invitee->supergroup_id)
                    {
                        buf = localizedPrintf(invitee, "CouldNotActionPlayerReason", "InviteString", inviteeName, "CantInviteRaidEnemy" );
                    }

                    // Don't add more checks here, add to league_CheckInviteeConditions or common_CheckInviteeConditions!

                    if (buf)
                    {
                        chatSendToPlayer(inviterDbID, buf, INFO_USER_ERROR, 0);
                        break;
                    }

                    invitee->pl->inviter_dbid = invitee->pl->last_inviter_dbid = inviterDbID; // remember the inviter, so we can trap bad commands
                    invitee->pl->last_invite_time = dbSecondsSince2000();

                    if( type != 2 )
                        chatSendToPlayer( inviterDbID, localizedPrintf( invitee, "LeagueInviteSent" ) , INFO_SVR_COM, 0 );

                    START_PACKET( pak, invitee, SERVER_LEAGUE_OFFER )
                        pktSendBits( pak, 32, inviterDbID );
                        pktSendString( pak, tmp_str );
                        pktSendBits( pak, 2, type );
                    END_PACKET
                }
            }break;
        case CMD_LEAGUE_ACCEPT:
            {
                int result = 0;
                char msg[512];
                Entity *invitee = e;
                if( invitee && invitee->pl ) // first make sure this is the person accepting
                {
                    int lockStatus = 0;
                    int teamupCount = 0;
                    int itr;
                    int teamLeaderId;
                    char *teamupMemberIds = estrTemp();
                    if ( invitee->pl->inviter_dbid != stack_tmp_int )
                    {
                        // TODO: Mark player is suspicious...and take away their candy
                        chatSendToPlayer( stack_tmp_dbid, localizedPrintf(invitee,"youCouldNotJoinLeague", "Unknown") , INFO_USER_ERROR, 0 );
                        invitee->pl->inviter_dbid = 0;
                        break;
                    }

                    invitee->pl->inviter_dbid = invitee->pl->last_inviter_dbid = 0;
                    //    between accepting a league invite, they joined another league
                    //    Scenario..
                    //    A invites B
                    //    B accepts and tells statserver to update
                    //    C invites B before statserver updates A and B and C
                    //    Statserver updates and puts B in a league
                    //    B accepts but shouldn't be able to
                    if( invitee->league )
                    {
                        chatSendToPlayer( stack_tmp_dbid, localizedPrintf(invitee,"youCouldNotJoinLeague", "CantAcceptWhileInLeague") , INFO_USER_ERROR, 0 );
                        break;                            
                    }
                    else if (invitee->teamup)
                    {
                        if (invitee->db_id != invitee->teamup->members.leader)
                        {
                            chatSendToPlayer( stack_tmp_dbid, localizedPrintf(invitee,"youCouldNotJoinLeague", "NotLeader") , INFO_USER_ERROR, 0 );
                            break;
                        }
                    }
                    else if ((dbSecondsSince2000() - invitee->pl->league_accept_blocking_time) < BLOCK_ACCEPT_TIME)
                    {
                        //    so when they quit a league of 1, they can't join immediately
                        chatSendToPlayer( stack_tmp_dbid, localizedPrintf(invitee,"youCouldNotJoinLeague", "WaitASec") , INFO_USER_ERROR, 0 );
                        break;
                    }
                    else if (db_state.is_endgame_raid)
                    {
                        //    so when they quit a league of 1, they can't join immediately
                        chatSendToPlayer( stack_tmp_dbid, localizedPrintf(invitee,"youCouldNotJoinLeague", "YouOnIncarnateTrial") , INFO_USER_ERROR, 0 );
                        break;
                    }

                    invitee->pl->league_accept_blocking_time = dbSecondsSince2000();
                    invitee->pl->lfg = 0;
                    svrSendEntListToDb(&invitee, 1);

                    lockStatus |= invitee->pl->taskforce_mode;
                    lockStatus |= invitee->teamup && invitee->teamup->teamSwapLock;

                    teamupCount = invitee->teamup ? invitee->teamup->members.count : 1;
                    teamLeaderId = invitee->teamup ? invitee->teamup->members.leader : invitee->db_id;
                    if (teamupCount > 1)
                    {
                        for (itr = 0; itr < teamupCount; ++itr)
                        {
                            estrConcatf(&teamupMemberIds, "%i,", invitee->teamup->members.ids[itr]);
                        }
                    }
                    else
                    {
                        estrConcatf(&teamupMemberIds, "%i,", invitee->db_id);
                    }
                    _snprintf_s( msg, sizeof(msg), sizeof(msg)-1, "league_accept_relay %i %i \"%s\" %i %i %i", stack_tmp_int, teamupCount, teamupMemberIds, lockStatus, teamLeaderId, stack_tmp_int2 );
                    serverParseClientEx( msg, client, ACCESS_INTERNAL );

                    estrDestroy(&teamupMemberIds);
                }
            }break;
        case CMD_LEAGUE_ACCEPT_RELAY:
            {
                int result = 0;
                int inviter_dbid = stack_tmp_int;
                char *originalCmd = stack_tmp_str;
                Entity *inviter = findEntOrRelayById( client, inviter_dbid, originalCmd, &result );

                if( inviter )
                {
                    // check for mismatched PvP flags
                    int i;
                    int memberCount = stack_tmp_int2;
                    int lockStatus = stack_tmp_int3;
                    int inviteeDbId = stack_tmp_int4;
                    int pvpSwitch = stack_tmp_int5;

                    if (inviter->pl->pvpSwitch != pvpSwitch)
                    {
                        chatSendToPlayer(inviter->db_id, localizedPrintf( inviter, "CouldNotActionPlayerReason", "InviteString", tmp_str, stack_tmp_int2?"PlayerHasPvPOn":"PlayerDoesNotHavePvPOn"), INFO_USER_ERROR, 0 );
                        chatSendToPlayer(inviteeDbId, localizedPrintf( inviter, "CouldNotActionReason", "JoinString", stack_tmp_int2?"YourPvPSwitchIsOn":"YourPvPSwitchIsOff"), INFO_USER_ERROR, 0 );
                        break;
                    }
                    else if (inviter->league && inviter->league->members.leader != inviter->db_id)
                    {
                        chatSendToPlayer(inviter->db_id, localizedPrintf( inviter, "CouldNotActionPlayerReason", "InviteString", tmp_str, "NotLeader"), INFO_USER_ERROR, 0 );
                        chatSendToPlayer(inviteeDbId, localizedPrintf( inviter, "CouldNotActionReason", "JoinString", "NotLeader"), INFO_USER_ERROR, 0 );
                        break;
                    }
                    else if (dbSecondsSince2000() - inviter->pl->league_accept_blocking_time < BLOCK_ACCEPT_TIME)
                    {
                        chatSendToPlayer(inviter->db_id, localizedPrintf( inviter, "CouldNotActionPlayerReason", "InviteString", tmp_str, "JoinedAlready"), INFO_USER_ERROR, 0 );
                        chatSendToPlayer(inviteeDbId, localizedPrintf( inviter, "CouldNotActionReason", "JoinString", "JoinedAlready"), INFO_USER_ERROR, 0 );
                        break;
                    }
                    else if (db_state.is_endgame_raid)
                    {
                        chatSendToPlayer(inviter->db_id, localizedPrintf( inviter, "CouldNotActionPlayerReason", "InviteString", tmp_str, "YouOnIncarnateTrial"), INFO_USER_ERROR, 0 );
                        chatSendToPlayer(inviteeDbId, localizedPrintf( inviter, "CouldNotActionReason", "JoinString", "PlayerOnIncarnateTrial"), INFO_USER_ERROR, 0 );
                        break;
                    }
                    else
                    {
                        int remainingSpots = MAX_LEAGUE_MEMBERS;
                        int teamCount = inviter->league ? league_teamCount(inviter->league) : 0;
                        for (i = 0; i < teamCount; ++i)
                        {
                            remainingSpots -= league_getTeamLockStatus(inviter->league, i) ? MAX_TEAM_MEMBERS : league_getTeamMemberCount(inviter->league, i);
                        }

                        if (remainingSpots < memberCount)
                        {
                            chatSendToPlayer(inviter->db_id, localizedPrintf( inviter, "CouldNotActionPlayerReason", "InviteString", tmp_str, "LeagueFull"), INFO_USER_ERROR, 0 );
                            chatSendToPlayer(inviteeDbId, localizedPrintf( inviter, "CouldNotActionReason", "JoinString", "LeagueFull"), INFO_USER_ERROR, 0 );                            
                            break;

                        }
                        if (teamCount >= MAX_LEAGUE_TEAMS && lockStatus)
                        {
                            chatSendToPlayer(inviter->db_id, localizedPrintf( inviter, "CouldNotActionPlayerReason", "InviteString", tmp_str, "LockedTeamFull"), INFO_USER_ERROR, 0 );
                            chatSendToPlayer(inviteeDbId, localizedPrintf( inviter, "CouldNotActionReason", "JoinString", "LockedTeamFull"), INFO_USER_ERROR, 0 );                            
                            break;
                        }

                        //    new league, block for the creator too
                        if (!inviter->league)
                            inviter->pl->league_accept_blocking_time = dbSecondsSince2000();

                        //    When inviter is creating a league, if this league is brand new, and inviter is part of a team
                        //    Pull everyone in the team into this league
                        if (inviter->teamup)
                        {
                            if (!inviter->league || (inviter->league && inviter->league->members.count <= 1))        //    either no exisiting league. or he was the only one in his league
                            {
                                int teamMembers;
                                int teamLockStatus = 0;
                                teamLockStatus |= inviter->pl && inviter->pl->taskforce_mode;
                                teamLockStatus |= inviter->teamup && inviter->teamup->teamSwapLock;

                                if (!league_AcceptOffer( inviter, inviter->teamup->members.leader, inviter->teamup->members.leader, teamLockStatus))
                                {
                                    team_leaveOrRelay(client, inviter->teamup->members.leader);
                                }

                                for (teamMembers = (inviter->teamup->members.count-1); teamMembers >= 0; teamMembers--)
                                {
                                    if (!league_AcceptOffer( inviter, inviter->teamup->members.ids[teamMembers], inviter->teamup->members.leader, teamLockStatus))
                                    {
                                        team_leaveOrRelay(client, inviter->teamup->members.ids[teamMembers]);
                                    }
                                }
                            }

                        }

                        //    When inviter is inviting a new person, and that person is in a team
                        //    Pull everyone in their team into the league
                        if (memberCount > 1)
                        {
                            //    TODO:
                            //    this section could be handled by the statserver
                            //    come up with a method to send all the ids
                            //    send it to the statserver and have it determine where people go
                            //    if the team count is full
                            char *memberStr = strdup(tmp_str);
                            int *memberIds = NULL;
                            eaiPush(&memberIds, atoi(strtok(memberStr, ",")));
                            for (i = 1; i < memberCount; ++i)
                            {
                                eaiPush(&memberIds, atoi(strtok(NULL, ",")));
                            }
                            SAFE_FREE(memberStr);
                            if (teamCount >= MAX_LEAGUE_TEAMS)
                            {
                                int teamMembers;
                                int numDelayedInvites[MAX_LEAGUE_TEAMS];
                                int j;
                                int *teamMemberIds = NULL;
                                int *teamLeaderIds = NULL;
                                for (j = 0; j < MAX_LEAGUE_TEAMS; j++)
                                {
                                    numDelayedInvites[j] = 0;
                                }
                                for (teamMembers = 0; teamMembers < eaiSize(&memberIds); ++teamMembers)
                                {
                                    //    add everyone into the league
                                    for (j = 0; j < MAX_LEAGUE_TEAMS; ++j)
                                    {
                                        //    should this change to a check team conditions
                                        if (!league_getTeamLockStatus(inviter->league, j) && ((league_getTeamMemberCount(inviter->league, j) + numDelayedInvites[j]) < MAX_TEAM_MEMBERS))
                                        {
                                            int teamMember_dbid = memberIds[teamMembers];
                                            if (teamMember_dbid != inviteeDbId)
                                            {
                                                numDelayedInvites[j]++;
                                                team_leaveOrRelay(client, teamMember_dbid);
                                                team_acceptOfferOrRelay(client, inviter->db_id, league_getTeamLeadId(inviter->league, j), teamMember_dbid, stack_tmp_int2);
                                                //    we don't invite to the league yet
                                                //    league accept offer destroys all lock status and member count information
                                                //    but store their id's
                                                eaiPush(&teamMemberIds, teamMember_dbid);    
                                                eaiPush(&teamLeaderIds, league_getTeamLeadId(inviter->league, j));
                                            }
                                            break;
                                        }
                                    }
                                }

                                for (j = 0; j < MAX_LEAGUE_TEAMS; ++j)
                                {
                                    //    should this change to a check team conditions
                                    if (!league_getTeamLockStatus(inviter->league, j) && (league_getTeamMemberCount(inviter->league, j) + numDelayedInvites[j]) < MAX_TEAM_MEMBERS)
                                    {
                                        numDelayedInvites[j]++;
                                        //    now do the leader
                                        team_leaveOrRelay(client, inviteeDbId);
                                        team_acceptOfferOrRelay(client, inviter->db_id, league_getTeamLeadId(inviter->league, j), inviteeDbId, stack_tmp_int2);
                                        //    we don't invite to the league yet
                                        //    league accept offer destroys all lock status and member count information
                                        //    but store their id's
                                        eaiPush(&teamMemberIds, inviteeDbId);
                                        eaiPush(&teamLeaderIds, league_getTeamLeadId(inviter->league, j));
                                        break;
                                    }
                                }
                                for (j = 0; j < eaiSize(&teamMemberIds); ++j)
                                {
                                    if (!league_AcceptOffer( inviter, teamMemberIds[j], teamLeaderIds[j], 0))    //    we know that the team is unlocked since they were accepted to this team after all
                                    {
                                        team_leaveOrRelay(client, teamMemberIds[j]);
                                    }
                                }
                                eaiDestroy(&teamMemberIds);
                                eaiDestroy(&teamLeaderIds);
                            }
                            else
                            {
                                int teamMembers;
                                league_AcceptOffer( inviter, inviteeDbId, inviteeDbId, lockStatus);
                                for (teamMembers = 0; teamMembers < eaiSize(&memberIds); ++teamMembers)
                                {
                                    if (memberIds[teamMembers] != inviteeDbId)
                                    {
                                        if (!league_AcceptOffer( inviter, memberIds[teamMembers], inviteeDbId, lockStatus))
                                        {
                                            team_leaveOrRelay(client, memberIds[teamMembers]);
                                        }
                                    }
                                }
                            }
                            eaiDestroy(&memberIds);
                        }
                        else
                        {
                            league_AcceptOffer(inviter, inviteeDbId, inviteeDbId, lockStatus);
                        }
                    }
                }
            }break;

        case CMD_LEAGUE_ACCEPT_OFFER_RELAY:
            {
                Entity *inviter;
                if(stack_tmp_dbid<=0)
                    return;

                if (relayCommandStart(NULL, stack_tmp_dbid, stack_tmp_str, &inviter, &online))
                {
                    if (inviter->pl->pvpSwitch != stack_tmp_int4) // Make sure we're still of the same pvp switch as the team leader
                    {
                        chatSendToPlayer(stack_tmp_int2, localizedPrintf( inviter,"CouldNotActionPlayerReason", "InviteString", inviter->name, inviter->pl->pvpSwitch?"PlayerHasPvPOn":"PlayerDoesNotHavePvPOn"), INFO_USER_ERROR, 0 );
                        chatSendToPlayer(inviter->db_id, localizedPrintf( inviter,"CouldNotActionReason", "JoinString", inviter->pl->pvpSwitch?"YourPvPSwitchIsOn":"YourPvPSwitchIsOff"), INFO_USER_ERROR, 0 );
                        relayCommandEnd(&inviter, &online);
                        break;
                    }

                    if (!league_AcceptOffer( inviter, stack_tmp_int, stack_tmp_int2, stack_tmp_int3))
                    {
                        team_leaveOrRelay(client, stack_tmp_int);
                    }
                    relayCommandEnd(&inviter, &online);
                }
            }break;
        case CMD_LEAGUE_DECLINE:
            {
                Entity *inviter = e;
                if( inviter && inviter->pl ) // first make sure the inviter is the guy who actually invited
                {
                    if ( inviter->pl->inviter_dbid != stack_tmp_int )
                    {
                        // TODO: Mark player is suspicious...and take away their candy
                        inviter->pl->inviter_dbid = 0; // reset value so someone else can invite
                        break;
                    }
                    inviter->pl->inviter_dbid = 0; // reset value so someone else can invite
                }
                chatSendToPlayer(stack_tmp_int,localizedPrintf(inviter,"declineLeagueOffer", tmp_str), INFO_USER_ERROR, 0 );
            }break;
        case CMD_LEAGUE_QUIT_RELAY:
            {
                Entity *leagueEnt = findEntOrRelayById(client, stack_tmp_int, stack_tmp_str, NULL);
                if (leagueEnt && leagueEnt->owned)
                {
                    if (leagueEnt->league_id)
                    {
                        sendEntsMsg(CONTAINER_LEAGUES, leagueEnt->league_id, INFO_SVR_COM, 0, "%s%s", DBMSG_CHAT_MSG, localizedPrintf(leagueEnt,"hasQuitLeague", leagueEnt->name));
                    }

                    league_MemberQuit(leagueEnt, 1);
                }
            }break;
        case CMD_LEAGUE_TEAM_WITHDRAW_RELAY:
            {
                e = findEntOrRelayById(client, stack_tmp_int, stack_tmp_str, 0);
                if (!e)
                    break;
            }        //    intentional fallthrough
        case CMD_LEAGUE_TEAM_WITHDRAW:
            {
                Entity *withdrawingTeamLeader = e;
                if (db_state.is_endgame_raid)
                {
                    chatSendToPlayer(withdrawingTeamLeader->db_id,localizedPrintf(withdrawingTeamLeader,"CantWithdrawOnIncarnateTrial"), INFO_USER_ERROR, 0 );
                }
                else
                {
                    if (withdrawingTeamLeader->teamup && withdrawingTeamLeader->db_id == withdrawingTeamLeader->teamup->members.leader)
                    {
                        int i;
                        if (league_IsLeader(withdrawingTeamLeader, withdrawingTeamLeader->db_id))
                        {
                            for (i = 0; i <withdrawingTeamLeader->league->members.count;++i)
                            {
                                if (withdrawingTeamLeader->league->members.ids[i] != withdrawingTeamLeader->db_id)
                                {
                                    if (withdrawingTeamLeader->league->members.ids[i] == withdrawingTeamLeader->league->teamLeaderIDs[i])
                                    {
                                        league_changeLeader(withdrawingTeamLeader, withdrawingTeamLeader->league->members.names[i]);
                                    }
                                }
                            }
                        }
                        for (i = 0; i < withdrawingTeamLeader->teamup->members.count; ++i)
                        {
                            league_leaveOrRelay(client, withdrawingTeamLeader->teamup->members.ids[i]);
                        }
                    }
                    else
                    {
                        league_MemberQuit(withdrawingTeamLeader, 1);
                    }
                }
            }
            break;
        case CMD_LEAGUE_KICK:
            {
                int kickedID = dbPlayerIdFromName( player_name );
                char msg[64];
                int interlockCheck;
                Entity *leader = e;
                if(!leader || !leader->pl)
                    break;

                interlockCheck = turnstileMapserver_teamInterlockCheck(leader, 1);
                if (interlockCheck == 2)
                {
                    chatSendToPlayer(leader->db_id, textStd("CantDoThatWhileInQueue"), INFO_USER_ERROR, 0 );
                    break;
                }
                if (interlockCheck != 0)
                {
                    chatSendToPlayer(leader->db_id, textStd("CantDoThatAtTheMoment"), INFO_USER_ERROR, 0 );
                    break;
                }

                if( player_name[0] == 0 )
                {
                    chatSendToPlayer(leader->db_id, localizedPrintf(leader,"incorrectFormat", "leaguekickString", "playerNameString", "emptyString", "leaguekickSynonyms"), INFO_USER_ERROR, 0);
                    break;
                }


                sprintf( msg, "league_kick_internal %i 0 %s", kickedID, player_name );
                serverParseClient( msg, client );
            }break;
        case CMD_LEAGUE_KICK_INTERNAL:
            {
                Entity *leader = e;
                if(leader && leader->league)
                {
                    int isTeamKick = league_isMemberTeamLocked(leader, stack_tmp_int);
                    int isEndgameRaidMap = db_state.is_endgame_raid;
                    int needConfirmation = isTeamKick | isEndgameRaidMap;

                    if (db_state.is_endgame_raid && !turnstileMapserver_eventLockedByGroup())
                    {
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"EGVRK_PugKickPrevent"), INFO_USER_ERROR, 0);
                        break;
                    }

                    if (needConfirmation && !stack_tmp_int2)
                    {
                        //    if person being kicked is locked, or we're on a raid map
                        //    send a confirm to kick
                        START_PACKET( pak, leader, SERVER_LEAGUE_TEAM_KICK )
                            pktSendString( pak, player_name );
                        pktSendBits( pak, 32,  stack_tmp_int );
                        pktSendBits (pak, 1, isTeamKick ? 1 : 0);
                        END_PACKET
                    }
                    else
                    {
                        if (isTeamKick)
                        {
                            int i;
                            int leaderId = 0;
                            char msg[64];
                            for (i = 0; i < leader->league->members.count; ++i)
                            {
                                if (leader->league->members.ids[i] == stack_tmp_int)
                                {
                                    leaderId = leader->league->teamLeaderIDs[i];
                                    break;
                                }
                            }
                            sprintf( msg, "leagueWithdrawTeamRelay %i", leaderId);
                            serverParseClientEx( msg , client, ACCESS_INTERNAL );
                        }
                        else
                        {
                            //    otherwise kick just the player
                            league_KickMember( leader, stack_tmp_int, player_name );
                        }
                    }
                }

            }break;
        case CMD_LEAGUE_KICK_RELAY:
            {
                Entity *leader = e;
                if(stack_tmp_int<=0)
                    return;

                if (relayCommandStart(NULL, stack_tmp_int, stack_tmp_str, &leader, &online))
                {
                    if (leader && leader->league && leader->league_id)
                    {
                        league_KickRelay(leader, stack_tmp_int2);
                    }
                    relayCommandEnd(&leader, &online);
                }
            }break;
        case CMD_LEAGUE_MAP_RELAY:
            {
                Entity *player = findEntOrRelayById(NULL, stack_tmp_int, stack_tmp_str, NULL);
                if(player)
                {
                    leagueSendMapUpdate(player);
                }
            }break;
        case CMD_LEAGUE_CHANGE_LEADER:
            {
                Entity *leader = e;
                if( leader && leader->pl )
                {
                    int interlockCheck = turnstileMapserver_teamInterlockCheck(leader, 1);
                    if (interlockCheck == 0)
                    {
                        league_changeLeader( leader, player_name );
                    }
                    else if (interlockCheck == 2)
                    {
                        chatSendToPlayer(leader->db_id, textStd("CantDoThatWhileInQueue"), INFO_USER_ERROR, 0);
                    }
                    else
                    {
                        chatSendToPlayer(leader->db_id, textStd("CantDoThatAtTheMoment"), INFO_USER_ERROR, 0);
                    }
                }
            }break;
        case CMD_LEAGUE_TEAM_SWAP_LOCK:
            {
                Entity *teamLeader = e;
                if( teamLeader )
                    league_teamswapLockToggle(teamLeader);
            }break;
        case CMD_LEAGUE_TEAM_SWAP_MEMBERS:
            {
                Entity *leader = e;
                //    TODO:    move this logic onto the statserver
                //            this may be a bit trickier since the entities need to be on this map...
                if (leader && leader->pl && !leader->pl->inTurnstileQueue)        //    can't be in the queue
                {
                    //    tmp_int1 is the first members db_id
                    //    tmp_int2 is the second members db_id
                    Entity *swapEnt1 = entFromDbId(stack_tmp_int);    //    do not relay these
                    Entity *swapEnt2 = entFromDbId(stack_tmp_int2);    //    do not relay these
                    //    Verify that both members are on the same map
                    if (swapEnt1 && swapEnt2)
                    {
                        devassert(swapEnt1->pl && !swapEnt1->pl->inTurnstileQueue
                            && swapEnt2->pl && !swapEnt2->pl->inTurnstileQueue);
                        if (swapEnt1->pl && !swapEnt1->pl->inTurnstileQueue
                            && swapEnt2->pl && !swapEnt2->pl->inTurnstileQueue)
                        {
                            //    Verify that this is the leader
                            if (league_IsLeader(swapEnt1, leader->db_id) && league_IsLeader(swapEnt2, leader->db_id))
                            {
                                //    existing problems:
                                //    taskforce_id is sometimes not zero. why?
                                //    league exists
                                //    teamup_id exists
                                if (league_isMemberTeamLocked(leader, swapEnt1->db_id))
                                {
                                    chatSendToPlayer(leader->db_id, localizedPrintf(leader, "LeagueEnt1IsTeamLocked"), INFO_SVR_COM, 0);
                                }
                                else if (league_isMemberTeamLocked(leader, swapEnt2->db_id))
                                {
                                    chatSendToPlayer(leader->db_id, localizedPrintf(leader, "LeagueEnt2IsTeamLocked"), INFO_SVR_COM, 0);
                                }
                                else
                                {
                                    int teamLead1 = swapEnt1->teamup ? swapEnt1->teamup->members.leader : swapEnt1->db_id;
                                    int teamLead2 = swapEnt2->teamup ? swapEnt2->teamup->members.leader : swapEnt2->db_id;
                                    Entity *team1Inviter = teamLead1 ? entFromDbId( teamLead1 ) : NULL;        //    do not relay this
                                    Entity *team2Inviter = teamLead2 ? entFromDbId( teamLead2 ) : NULL;        //    do not relay this

                                    //    everyone needs to be on the map to make this an atomic operation. otherwise, we have to use relays
                                    if (team1Inviter && team2Inviter)
                                    {
                                        if (league_teamCheckSwapEnts(team1Inviter, swapEnt2))
                                        {
                                            if (league_teamCheckSwapEnts(team2Inviter, swapEnt1))
                                            {
                                                team_MemberQuit(swapEnt1);
                                                team_MemberQuit(swapEnt2);

                                                if (team_AcceptOffer(team2Inviter, swapEnt1->db_id, 0))
                                                {
                                                    league_updateTeamLeaderSolo(leader, swapEnt1->db_id, team2Inviter->db_id);
                                                }
                                                else
                                                {
                                                    league_leaveOrRelay(client, swapEnt1->db_id);
                                                }
                                                if (team_AcceptOffer(team1Inviter, swapEnt2->db_id, 0))
                                                {
                                                    league_updateTeamLeaderSolo(leader, swapEnt2->db_id, team1Inviter->db_id);
                                                }
                                                else
                                                {
                                                    league_leaveOrRelay(client, swapEnt2->db_id);
                                                }
                                            }
                                            else
                                            {
                                                chatSendToPlayer(leader->db_id, localizedPrintf(leader, "LeaguePlayer1Failedswap"), INFO_SVR_COM, 0);
                                            }
                                        }
                                        else
                                        {
                                            chatSendToPlayer(leader->db_id, localizedPrintf(leader, "LeaguePlayer2Failedswap"), INFO_SVR_COM, 0);
                                        }
                                    }
                                    else
                                    {
                                        chatSendToPlayer(leader->db_id, localizedPrintf(leader, "LeagueBothLeadersMustBeOnMap"), INFO_SVR_COM, 0);
                                    }
                                }
                            }
                            else
                            {
                                chatSendToPlayer(leader->db_id, localizedPrintf(leader, "LeagueMustBeLeader"), INFO_SVR_COM, 0);
                            }
                        }
                    }
                    else
                    {
                        chatSendToPlayer(leader->db_id, localizedPrintf(leader, "LeagueBothPlayersMustBeOnMap"), INFO_SVR_COM, 0);
                    }
                }
            }break;
        case CMD_LEAGUE_TEAM_MOVE_MEMBER:
            {
                //    TODO:    move this logic onto the statserver
                //            this may be a bit trickier since the entity needs to be on this map
                int leader_dbid = stack_tmp_int;
                Entity *moving = findEntOrRelayById(client, stack_tmp_int2, stack_tmp_str, NULL);
                Entity *leader = entFromDbId(leader_dbid);
                if (leader)
                {
                    //    verify moving is on the map
                    if (moving)
                    {
                        //    verify that ent is the leader
                        if (league_IsLeader(moving, leader_dbid))
                        {
                            int teamLeaderId = stack_tmp_int3;
                            if (league_isMemberTeamLocked(moving, moving->db_id))
                            {
                                chatSendToPlayer(leader_dbid, localizedPrintf(moving, "LeagueEnt1IsTeamLocked"), INFO_SVR_COM, 0);
                            }

                            //    does the team "moving" is moving to, exist
                            else if (teamLeaderId != -1)
                            {
                                //    can moving join the team
                                //    is the team locked
                                Entity *teamLeader = entFromDbId(teamLeaderId);        //    do not relay this. the other team leader needs to be 
                                //    on the map or else the atomic join doesn't work
                                if (teamLeader)
                                {
                                    if (league_IsLeader(teamLeader, leader_dbid))
                                    {
                                        if (team_IsLeader(moving, teamLeaderId))
                                        {
                                            chatSendToPlayer(leader_dbid, localizedPrintf(moving, "LeagueAlreadyOnTeam"), INFO_SVR_COM, 0);
                                        }
                                        else if (league_isMemberTeamLocked(moving, teamLeaderId))
                                        {
                                            chatSendToPlayer(leader_dbid, localizedPrintf(moving, "LeagueEnt2IsTeamLocked"), INFO_SVR_COM, 0);
                                        }
                                        else if (!league_teamCheckAtomicJoin(teamLeader, moving))
                                        {
                                            chatSendToPlayer(leader_dbid, localizedPrintf(moving, "LeagueCouldNotMoveToTeam"), INFO_SVR_COM, 0);                                            
                                        }
                                        else
                                        {
                                            team_MemberQuit(moving);
                                            if (team_AcceptOffer(teamLeader, moving->db_id, 0))
                                            {
                                                league_updateTeamLeaderSolo(moving, moving->db_id, teamLeaderId);
                                            }
                                            else
                                            {
                                                league_leaveOrRelay(client, moving->db_id);
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    chatSendToPlayer(leader_dbid, localizedPrintf(moving, "LeagueTeamLeaderMustBeOnMap"), INFO_SVR_COM, 0);
                                }
                            }
                            else
                            {
                                if (league_teamCount(moving->league) < MAX_LEAGUE_TEAMS)
                                {
                                    //    give them a new team
                                    //    quit the team, but still in the league =]]
                                    team_MemberQuit(moving);
                                    league_updateTeamLeaderSolo(moving, moving->db_id, moving->db_id);
                                }
                                else
                                {
                                    chatSendToPlayer(leader_dbid, localizedPrintf(moving, "LeagueExceedingTeamLimit"), INFO_SVR_COM, 0);
                                }
                            }
                        }
                        else
                        {
                            chatSendToPlayer(leader_dbid, localizedPrintf(moving, "LeagueMustBeLeader"), INFO_SVR_COM, 0);
                        }
                    }
                    else
                    {
                        chatSendToPlayer(leader_dbid, localizedPrintf(moving, "LeaguePlayerMustBeOnMap"), INFO_SVR_COM, 0);
                    }
                }
                else
                {
                    chatSendToPlayer(leader_dbid, localizedPrintf(moving, "LeagueLeaderMustBeOnMap"), INFO_SVR_COM, 0);
                }
            }break;
        case CMD_LEAGUE_REMOVE_ACCEPT_BLOCK:
            {
                Entity *player = findEntOrRelayById(client, stack_tmp_int, stack_tmp_str, NULL);
                if (player && player->pl)
                {
                    player->pl->league_accept_blocking_time = 0;
                }
            }break;
        case CMD_TURNSTILE_LEAGUE_QUIT:
            {
                if (e && e->pl)
                {
                    if (e->pl->lastTurnstileEventID)
                    {
                        //    notify the turnstile server that we are leaving
                        int voluntaryLeave = stack_tmp_int;
                        turnstileMapserver_playerLeavingRaid(e, voluntaryLeave);
                    }
                }
            }break;
        case CMD_TURNSTILE_INVITE_PLAYER_TO_EVENT:
            {
                Entity *inviter = e;
                if (inviter)
                {
                    if (db_state.is_endgame_raid && !IncarnateTrialComplete())
                    {
                        //    verify i am the leader
                        if (inviter->league && league_IsLeader(inviter, inviter->db_id))
                        {
                            int invitee_dbid = dbPlayerIdFromName( player_name );
                            if (invitee_dbid)
                            {
                                //    if the format here changes, update the relayed portion as well (that bounces to the leader)
                                char msg[128];
                                sprintf( msg, "turnstile_invite_player_relay %i %i %i %i %s", invitee_dbid, inviter->db_id, inviter->pl->lastTurnstileMissionID, inviter->league->members.count, inviter->name);
                                serverParseClientEx( msg, client, ACCESS_INTERNAL );
                            }
                            else
                            {
                                //    send message that this isn't a player
                                chatSendToPlayer(inviter->db_id, localizedPrintf(inviter, "playerNotFound", player_name), INFO_SVR_COM, 0);
                            }
                        }
                        else
                        {
                            //    send message that they are not a leader
                            chatSendToPlayer(inviter->db_id, localizedPrintf(inviter, "LeagueMustBeLeader"), INFO_SVR_COM, 0);
                        }
                    }
                    else
                    {
                        //    send message that you are not in an instance
                        chatSendToPlayer(inviter->db_id, localizedPrintf(inviter, "TUTNotInInstance"), INFO_SVR_COM, 0);
                    }
                }

            }break;
        case CMD_TURNSTILE_INVITE_PLAYER_TO_EVENT_RELAY:
            {
                Entity *invitee = findEntOrRelayById(client, stack_tmp_dbid, stack_tmp_str, NULL);
                if (invitee)
                {
                    if (invitee->league_id)
                    {
                        if (!league_IsLeader(invitee, invitee->db_id))
                        {
                            //    if the format here changes, update the relayed portion as well (that bounces to the leader)
                            char *replaceStr = NULL;
                            estrConcatf(&replaceStr, "turnstile_invite_player_relay %i %i %i %i %s", invitee->league->members.leader, 
                                stack_tmp_int, stack_tmp_int2, stack_tmp_int3, player_name_cmd);
                            invitee = findEntOrRelayById(client, invitee->league->members.leader, stack_tmp_str, NULL);
                            estrDestroy(&replaceStr);
                            if (!invitee)
                            {
                                break;
                            }    
                        }
                    }
                    else if (invitee->teamup)
                    {
                        if (invitee->teamup->members.leader != invitee->db_id)
                        {
                            //    if the format here changes, update the relayed portion as well (that bounces to the leader)
                            char *replaceStr = NULL;
                            estrConcatf(&replaceStr, "turnstile_invite_player_relay %i %i %i %i %s", invitee->teamup->members.leader, 
                                stack_tmp_int, stack_tmp_int2, stack_tmp_int3, player_name_cmd);
                            invitee = findEntOrRelayById(client, invitee->teamup->members.leader, stack_tmp_str, NULL);
                            estrDestroy(&replaceStr);
                            if (!invitee)
                            {
                                break;
                            }    
                        }
                    }
                    if (invitee->pl->inviter_dbid)
                    {
                        //    player is considering another offer
                        chatSendToPlayer(stack_tmp_int, localizedPrintf(NULL,"CouldNotActionPlayerReason", "InviteString", invitee->name, "PlayerIsConsideringAnotherOffer"), INFO_SVR_COM, 0);
                        break;
                    }
                    else if ((invitee->pl->last_inviter_dbid == stack_tmp_int) && ((dbSecondsSince2000() - invitee->pl->last_invite_time) < BLOCK_INVITE_TIME))
                    {
                        //    player is considering another offer
                        chatSendToPlayer(stack_tmp_int, localizedPrintf(NULL,"CouldNotActionPlayerReason", "InviteString", invitee->name, "MustWaitLonger"), INFO_SVR_COM, 0);
                        break;
                    }
                    if (!staticMapInfoFind(invitee->map_id))
                    {
                        //    player must be on a static map
                        chatSendToPlayer(stack_tmp_int, localizedPrintf(NULL,"CouldNotActionPlayerReason", "InviteString", invitee->name, "TUTPlayerOnMissionMap"), INFO_SVR_COM, 0);
                        break;
                    }
                    if (!EAINRANGE(stack_tmp_int2, turnstileConfigDef.missions))
                    {
                        //    invalid mission
                        chatSendToPlayer(stack_tmp_int, localizedPrintf(NULL,"CouldNotActionPlayerReason", "InviteString", invitee->name, "TUTInvalidMissionForPlayer"), INFO_SVR_COM, 0);
                        break;
                    }
                    else
                    {
                        TeamMembers *members = NULL;
                        if (invitee->league)
                        {
                            members = &invitee->league->members;
                        }
                        else if (invitee->teamup)
                        {
                            members = &invitee->teamup->members;
                        }
                        if (members)
                        {
                            if (members->count + stack_tmp_int3 > turnstileConfigDef.missions[stack_tmp_int2]->maximumPlayers )
                            {
                                //    invalid mission
                                chatSendToPlayer(stack_tmp_int, localizedPrintf(NULL,"CouldNotActionPlayerReason", "InviteString", invitee->name,"TUTTooManyPlayers"), INFO_SVR_COM, 0);
                                break;
                            }
                        }
                        else
                        {
                            if (1 + stack_tmp_int3 > turnstileConfigDef.missions[stack_tmp_int2]->maximumPlayers )
                            {
                                //    invalid mission
                                chatSendToPlayer(stack_tmp_int, localizedPrintf(NULL,"CouldNotActionPlayerReason", "InviteString", invitee->name,"TUTTooManyPlayers"), INFO_SVR_COM, 0);
                                break;
                            }
                        }
                    }
                    invitee->pl->inviter_dbid = invitee->pl->last_inviter_dbid = stack_tmp_int;
                    invitee->pl->last_invite_time = dbSecondsSince2000();

                    chatSendToPlayer(stack_tmp_int, localizedPrintf(NULL,"TUTInviteSent"), INFO_SVR_COM, 0);
                    //    if person being kicked is locked, or we're on a raid map
                    //    send a confirm to kick
                    START_PACKET( pak, invitee, SERVER_EVENT_JOIN_LEADER );
                        pktSendString(pak, player_name);
                        pktSendString(pak, turnstileConfigDef.missions[stack_tmp_int2]->name);
                        pktSendBitsAuto(pak, stack_tmp_int);
                        pktSendBitsAuto(pak, stack_tmp_int2);
                    END_PACKET
                }
            }break;
        case CMD_TURNSTILE_INVITE_PLAYER_TO_EVENT_ACCEPT:
            {
                Entity *invitee = e;
                if (invitee)
                {
                    int inviter_dbid = invitee->pl->last_inviter_dbid; 
                    if (inviter_dbid == stack_tmp_dbid)
                    {
                        if (stack_tmp_int)
                        {
                            char msg[64];
                            int itr;
                            TeamMembers *members = NULL;
                            if (invitee->league)
                            {
                                members = &invitee->league->members;
                            }
                            else if (invitee->teamup)
                            {
                                members = &invitee->teamup->members;
                            }
                            if (members)
                            {
                                int allPass = 1;
                                for (itr = 0; itr < members->count; ++itr)
                                {
                                    Entity *teammate = entFromDbId(members->ids[itr]);
                                    allPass = 0;
                                    if (teammate)
                                    {
                                        if (chareval_requires(teammate->pchar, turnstileConfigDef.missions[stack_tmp_int2]->hideIf, "defs/turnstile_server.def"))
                                        {
                                            //    invalid mission
                                            chatSendToPlayer(invitee->db_id, localizedPrintf(NULL,"CouldNotActionPlayerReason", "JoinString", teammate->name,"TUTInvalidMissionForPlayerTeammate"), INFO_SVR_COM, 0);
                                            break;
                                        }
                                        if (turnstileConfigDef.missions[stack_tmp_int2]->required && !chareval_requires(teammate->pchar, turnstileConfigDef.missions[stack_tmp_int2]->required, "defs/turnstile_server.def"))
                                        {
                                            //    invalid mission
                                            chatSendToPlayer(invitee->db_id, localizedPrintf(NULL,"CouldNotActionPlayerReason", "JoinString", teammate->name,"TUTInvalidMissionForPlayerTeammate"), INFO_SVR_COM, 0);
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        chatSendToPlayer(invitee->db_id, localizedPrintf(NULL,"CouldNotActionPlayerReason", "JoinString", members->names[itr],"TUTPlayerNotOnMap"), INFO_SVR_COM, 0);
                                        break;
                                    }
                                    allPass = 1;
                                }
                                if (!allPass)
                                {
                                    chatSendToPlayer(invitee->db_id, localizedPrintf(NULL,"TUTFailedTeammate"), INFO_SVR_COM, 0);
                                    break;
                                }
                            }
                            else
                            {
                                if (chareval_requires(invitee->pchar, turnstileConfigDef.missions[stack_tmp_int2]->hideIf, "defs/turnstile_server.def"))
                                {
                                    //    invalid mission
                                    chatSendToPlayer(invitee->db_id, localizedPrintf(NULL,"CouldNotActionPlayerReason", "JoinString", player_name,"TUTInvalidMissionForPlayer"), INFO_SVR_COM, 0);
                                    break;
                                }
                                if (turnstileConfigDef.missions[stack_tmp_int2]->required && !chareval_requires(invitee->pchar, turnstileConfigDef.missions[stack_tmp_int2]->required, "defs/turnstile_server.def"))
                                {
                                    //    invalid mission
                                    chatSendToPlayer(invitee->db_id, localizedPrintf(NULL,"CouldNotActionPlayerReason", "JoinString", player_name,"TUTInvalidMissionForPlayer"), INFO_SVR_COM, 0);
                                    break;
                                }
                            }
                            sprintf( msg, "turnstile_invite_player_accept_relay %i %i", inviter_dbid, invitee->db_id );
                            serverParseClientEx( msg, client, ACCESS_INTERNAL );
                        }
                        else
                        {
                            //    send message that they are not a leader
                            chatSendToPlayer(inviter_dbid, localizedPrintf(NULL, "TUTInviteeDeclined", invitee->name), INFO_SVR_COM, 0);
                            invitee->pl->inviter_dbid = 0;
                            break;
                        }
                    }
                    else
                    {
                        //    cheater?
                        invitee->pl->inviter_dbid = 0;
                        break;
                    }
                }
            }break;
        case CMD_TURNSTILE_INVITE_PLAYER_TO_EVENT_ACCEPT_RELAY:
            {
                //    send request to player
                Entity *inviter = findEntOrRelayById(client, stack_tmp_dbid, stack_tmp_str, NULL);
                if (inviter)
                {
                    if (db_state.is_endgame_raid && !IncarnateTrialComplete())
                    {
                        //    verify i am the leader
                        if (inviter->league && league_IsLeader(inviter, inviter->db_id))
                        {
                            char msg[64];
                            int db_id = stack_tmp_int; 
                            if (db_id)
                            {
                                sprintf( msg, "turnstile_join_specific_mission_instance %i %i %i", db_id, inviter->pl->lastTurnstileMissionID, inviter->pl->lastTurnstileEventID );
                                serverParseClientEx( msg, client, ACCESS_INTERNAL );
                            }
                            else
                            {
                                //    send message that this isn't a player
                                chatSendToPlayer(inviter->db_id, localizedPrintf(inviter, "playerNotFound", player_name), INFO_SVR_COM, 0);
                            }
                        }
                        else
                        {
                            //    send message that they are not a leader
                            chatSendToPlayer(inviter->db_id, localizedPrintf(inviter, "LeagueMustBeLeader"), INFO_SVR_COM, 0);
                        }
                    }
                    else
                    {
                        //    send message that you are not in an instance
                        chatSendToPlayer(inviter->db_id, localizedPrintf(inviter, "TUTNotInInstance"), INFO_SVR_COM, 0);
                    }
                }
            }break;
        case CMD_TURNSTILE_JOIN_SPECIFIC_MISSION_INSTANCE:
            {
                Entity *invitee = findEntOrRelayById(client, stack_tmp_dbid, stack_tmp_str, NULL);
                if (invitee)
                {
                    int missionID = stack_tmp_int;
                    int instanceID = stack_tmp_int2;
                    TeamMembers *members = NULL;
                    if (invitee->league)
                    {
                        members = &invitee->league->members;
                    }
                    else if (invitee->teamup)
                    {
                        members = &invitee->teamup->members;
                    }
                    if (members)
                    {
                        if (invitee->db_id == members->leader)
                        {
                            int itr;
                            for (itr = 0; itr < members->count; ++itr)
                            {
                                Entity *groupMate = entFromDbId(members->ids[itr]);
                                if (groupMate)
                                {
                                    turnstileMapserver_QueueForSpecificMissionInstance(groupMate, missionID, instanceID);
                                }
                            }
                        }
                    }
                    else
                    {
                        turnstileMapserver_QueueForSpecificMissionInstance(invitee, missionID, instanceID);
                    }
                }
            }break;
        //------------------------------------------------------------------------------------------------
        // TRADING ////////////////////////////////////////////////////////////////////////////////////////
        //-------------------------------------------------------------------------------------------------

        case CMD_TRADE_INVITE:
            {
                if( player_name[0] == 0 )
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
                        "tradeCString", "playerNameString", "emptyString", "tradeSynonyms" ), INFO_USER_ERROR, 0);
                    break;
                }

                if(e)
                    trade_offer( e, player_name );
            }break;

        case CMD_TRADE_INVITE_ACCEPT:
            {
                Entity *inviter;
                int id = dbPlayerIdFromName( player_name );

                inviter = entFromDbId( id );

                if( e && e->pl && inviter ) // first make sure the inviter is the guy who actually invited
                {
                    if( e->pl->inviter_dbid != inviter->db_id || e->db_id == inviter->db_id )
                    {
                        // TODO: Mark player is suspicious...and take away their candy
                        chatSendToPlayer( stack_tmp_dbid, localizedPrintf(e,"TradeFailed") , INFO_USER_ERROR, 0 );
                        e->pl->inviter_dbid = 0;
                        break;
                    }
                    e->pl->inviter_dbid = e->pl->last_inviter_dbid = 0; // reset value so someone else can invite
                }

                if( inviter && e )
                    trade_init( inviter, e );

            }break;

        case CMD_TRADE_INVITE_DECLINE:
            {
                Entity *inviter;
                int id = dbPlayerIdFromName( player_name );
                inviter = entFromDbId( id );

                if( e && e->pl ) // first make sure the inviter is the guy who actually invited
                {
                    if ( e->pl->inviter_dbid != stack_tmp_int )
                    {
                        // TODO: Mark player is suspicious...and take away their candy
                        e->pl->inviter_dbid = 0;
                        break;
                    }
                    e->pl->inviter_dbid = 0; // reset value so someone else can invite
                }

                if( inviter )
                    sendChatMsg( inviter, localizedPrintf(e,"declineTradeOffer", tmp_str) , INFO_USER_ERROR, 0);

            } break;

            //------------------------------------------------------------------------------------------------
            // Costume ////////////////////////////////////////////////////////////////////////////////////////
            //-------------------------------------------------------------------------------------------------

        case CMD_COSTUME_CHANGE:
            {
                if( e && e->pl ) 
                {
                    Costume *costume;
                    
                    costume_change( e, stack_tmp_int );
                    
                    costume = costume_current(e);
                
                    if (costume)
                    {
                        int sgColorIndex = costume->appearance.currentSuperColorSet;
                        if (sgColorIndex < 0 || sgColorIndex >= NUM_SG_COLOR_SLOTS)
                        {
                            sgColorIndex = 0;
                            costume->appearance.currentSuperColorSet = 0;
                        }
                        e->pl->superColorsPrimaryU    = costume->appearance.superColorsPrimaryU[sgColorIndex];
                        e->pl->superColorsSecondaryU  = costume->appearance.superColorsSecondaryU[sgColorIndex];
                        e->pl->superColorsPrimary2U   = costume->appearance.superColorsPrimary2U[sgColorIndex];
                        e->pl->superColorsSecondary2U = costume->appearance.superColorsSecondary2U[sgColorIndex];
                        e->pl->superColorsTertiaryU   = costume->appearance.superColorsTertiaryU[sgColorIndex];
                        e->pl->superColorsQuaternaryU = costume->appearance.superColorsQuaternaryU[sgColorIndex];
                        costume_sendSGColors(e);
                        
                        PrintCostumeToLog(e);
                    }
                }
            }break;

        case CMD_COSTUME_ADDSLOT:
            {
                if( e && e->pl && e->pl->num_costume_slots < costume_get_max_earned_slots( e ) ) 
                {
                    int totalSlots;

                    e->pl->num_costume_slots++;
                    totalSlots = costume_get_num_slots(e) - 1;

                    costume_destroy( e->pl->costume[totalSlots]  );
                    e->pl->costume[totalSlots] = NULL; // destroy does not set to NULL.  NULL is checked in costume_new_slot()

                    e->pl->costume[totalSlots] = costume_new_slot( e );

                    powerCustList_destroy( e->pl->powerCustomizationLists[totalSlots] );
                    e->pl->powerCustomizationLists[totalSlots] = powerCustList_new_slot( e );

                    e->pl->num_costumes_stored = totalSlots + 1;
                    costumeFillPowersetDefaults(e);

                    e->send_all_costume = true;
                    e->send_costume = true;
                    e->updated_powerCust = true;
                    e->send_all_powerCust = true;
                }
            }break;

            //------------------------------------------------------------------------------------------------
            // Hide ////////////////////////////////////////////////////////////////////////////////////////
            //-------------------------------------------------------------------------------------------------

        case CMD_HIDE_SET:
            {
                if( e && e->pl )
                {
                    if( (tmp_int&(1<<HIDE_GLOBAL_FRIENDS)) && !(e->pl->hidden&(1<<HIDE_GLOBAL_FRIENDS)) )
                        shardCommSendf(e,true,"FriendHide");
                    else if( !(tmp_int&(1<<HIDE_GLOBAL_FRIENDS)) && (e->pl->hidden&(1<<HIDE_GLOBAL_FRIENDS)) )
                        shardCommSendf(e,true,"FriendunHide");

                    if( (tmp_int&(1<<HIDE_GLOBAL_CHANNELS)) && !(e->pl->hidden&(1<<HIDE_GLOBAL_CHANNELS)) )
                        shardCommSendf(e,true,"invisible");
                    else if( !(tmp_int&(1<<HIDE_GLOBAL_CHANNELS)) && (e->pl->hidden&(1<<HIDE_GLOBAL_CHANNELS)) )
                        shardCommSendf(e,true,"visible");

                    if( (tmp_int&(1<<HIDE_TELL)) && !(e->pl->hidden&(1<<HIDE_TELL)) )
                        shardCommSendf(e,true,"TellHide");
                    else if( !(tmp_int&(1<<HIDE_TELL)) && (e->pl->hidden&(1<<HIDE_TELL)) )
                        shardCommSendf(e,true,"TellUnHide");

                    if(!tmp_int)
                        chatSendToPlayer(e->db_id, localizedPrintf(e, "NotHiddenFromAnything"), INFO_SVR_COM, 0);
                    else if( tmp_int == (1<<HIDE_COUNT)-1 )
                        chatSendToPlayer(e->db_id, localizedPrintf(e, "HiddenFromEverything"), INFO_SVR_COM, 0);
                    else
                    {
                        char * hidden_from;
                        char * not_hidden_from;

                         estrCreate(&hidden_from);
                        estrCreate(&not_hidden_from);
                        estrConcatf(&hidden_from, "%s ", localizedPrintf(e, "YouHiddenFrom"));
                        estrConcatf(&not_hidden_from, "%s ", localizedPrintf(e, "YouAreNotHiddenFrom"));

                        estrConcatf( (tmp_int&(1<<HIDE_SEARCH))?&hidden_from:&not_hidden_from, "%s, ", localizedPrintf(e, "HiddenFromSearch") );
                        estrConcatf( (tmp_int&(1<<HIDE_SG))?&hidden_from:&not_hidden_from, "%s, ", localizedPrintf(e, "HiddenFromSg") );
                        estrConcatf( (tmp_int&(1<<HIDE_FRIENDS))?&hidden_from:&not_hidden_from, "%s, ", localizedPrintf(e, "HiddenFromFriend") );
                        estrConcatf( (tmp_int&(1<<HIDE_GLOBAL_FRIENDS))?&hidden_from:&not_hidden_from, "%s, ", localizedPrintf(e, "HiddenFromGlobalFriend") );
                        estrConcatf( (tmp_int&(1<<HIDE_GLOBAL_CHANNELS))?&hidden_from:&not_hidden_from, "%s, ", localizedPrintf(e, "HiddenFromGlobalChannel") );
                        estrConcatf( (tmp_int&(1<<HIDE_TELL))?&hidden_from:&not_hidden_from, "%s, ", localizedPrintf(e, "HiddenFromTell") );
                        estrConcatf( (tmp_int&(1<<HIDE_INVITE))?&hidden_from:&not_hidden_from, "%s, ", localizedPrintf(e, "HiddenFromInvite") );

                        hidden_from[strlen(hidden_from)-2] = '.';
                        not_hidden_from[strlen(not_hidden_from)-2] = '.';
                        chatSendToPlayer(e->db_id, hidden_from, INFO_SVR_COM, 0);
                        chatSendToPlayer(e->db_id, not_hidden_from, INFO_SVR_COM, 0);

                        estrDestroy(&hidden_from);
                        estrDestroy(&not_hidden_from);
                    }

                    e->pl->hidden = tmp_int;
                    svrSendEntListToDb(&e, 1);
                }
            }break;

        case CMD_SEARCH:
            {
                if( e && e->pl )
                {
                    search_parse( e, tmp_str );
                }
            }break;

        case CMD_SET_BADGE_TITLE:
        case CMD_SET_BADGE_TITLE_ID:
            {
                if( e && e->pl )
                {
                    char * title;

                    if ( cmd->num == CMD_SET_BADGE_TITLE )
                        title = badge_setTitle( e, stack_tmp_int );
                    else
                        title = badge_setTitleId( e, stack_tmp_int );

                    if( title )
                          chatSendToPlayer( e->db_id, localizedPrintf(e,"BadgeTitleSet", title), INFO_SVR_COM, 0);
                    else
                        chatSendToPlayer( e->db_id, localizedPrintf(e,"BadgeTitleCleared"), INFO_SVR_COM, 0);

                    e->send_costume = 1;
                }
            }break;
        case CMD_SET_BADGE_DEBUG_LEVEL:
            {
                if( e && e->pl )
                {
                    if( stack_tmp_int >= 0 && stack_tmp_int <= 2 )
                    {
                        e->pl->debugBadgeLevel = stack_tmp_int;
                        badge_ClearUpdatedAll( e );
                    }
                }
            }break;
        case CMD_SET_COMMENT:
            {
                if( e && e->pl )
                {
                    strncpyt(e->pl->comment, breakHTMLtags(tmp_str), MAX_NAME_LEN );
                    chatSendToPlayer( e->db_id, localizedPrintf(e,"CommentSetTo", e->pl->comment), INFO_SVR_COM, 0);
                }
            }break;

        case CMD_GET_COMMENT:
            {
                if( e && e->pl )
                {
                    chatSendToPlayer( e->db_id, localizedPrintf(e,"CommentSetTo", e->pl->comment), INFO_SVR_COM, 0);
                }
            }break;

            //------------------------------------------------------------------------------------------------
            // Arena ////////////////////////////////////////////////////////////////////////////////////////
            //-------------------------------------------------------------------------------------------------


        case CMD_ARENA_INVITE:
            {
                char buf[256] = {0};
                int i, online;
                Entity * player;
                ArenaEvent * event;
                int sendMessage = true;

                 int id = dbPlayerIdFromName(player_name);

                if( !e )
                {
                    sendMessage = false;
                    break;
                }

                if( player_name[0] == 0 )
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
                        "arenainviteString", "playerNameString", "emptyString", "arenainviteSynonyms" ) , INFO_ARENA_ERROR, 0);
                    sendMessage = false;
                    break;
                }

                event = FindEventDetail( e->pl->arenaActiveEvent.eventid, 0, NULL );


                //if( !event ) // pass through for context_menu to allow testing
                //    event = FindEventDetail( stack_tmp_int, 0, NULL );

                if( !event || event->uniqueid != e->pl->arenaActiveEvent.uniqueid )
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"CouldNotActionPlayerReason", 
                        "InviteString", player_name, "NotInEvent" ), INFO_ARENA_ERROR, 0);
                    sendMessage = false;
                    break;
                }

                player = entFromDbId( id );
                online = player_online(player_name);

                if (id < 0 ||                            // Character doesn't exist or
                    (!player && !online) ||                // He's not online and he's not on this mapserver or
                    (player && player->logout_timer) ||    // He's on this mapserver, and online, but he's currently marked as "quitting"
                    (player && player->pl->hidden&(1<<HIDE_TELL)) )
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"playerNotFound", player_name), INFO_ARENA_ERROR, 0);
                    sendMessage = false;
                    break;
                }

                if( e->db_id == id ) // trying to invite himself
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotInviteYourselfArena"), INFO_ARENA_ERROR, 0);
                    sendMessage = false;
                    break;
                }

                if( event->creatorid != e->db_id ) // not high enough rank to invite
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"CouldNotActionPlayerReason", 
                        "InviteString", player_name, "NotEventCreator" ), INFO_ARENA_ERROR, 0);
                    sendMessage = false;
                    break;
                }

                if( event->numplayers >= event->maxplayers ) // no room in supergroup
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"CouldNotActionPlayerReason", 
                        "InviteString", player_name, "EventIsFull"  ), INFO_ARENA_ERROR, 0);
                    sendMessage = false;
                    break;
                }

                for( i = 0; i < eaSize(&event->participants); i++ )
                {
                    if( id == event->participants[i]->dbid )
                    {
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"CouldNotActionPlayerReason", 
                            "InviteString", player_name, "AlreadyInEvent"), INFO_ARENA_ERROR, 0);
                        sendMessage = false;
                        break;
                    }
                }

                if (sendMessage)
                {
                    // ok, everyhting seems cool on this end, send it across mapservers is nessecary
                    sprintf( buf, "%s \"%s\" \"%s\" %i %i %i", "arenainvite_long", player_name, e->name, e->db_id, event->eventid, event->uniqueid );
                    serverParseClient( buf, NULL );
                }

            }break;

        case CMD_ARENA_INVITE_LONG:
            {
                int result = 0;
                Entity* ent = findEntOrRelay( NULL, player_name, stack_tmp_str, &result );

                if( ent )
                {
                    int id = dbPlayerIdFromName(tmp_str);

                    if( ent->pl->arenaActiveEvent.eventid ) // if player is already on a event forget it
                    {
                        char * buf = localizedPrintf(ent,"CouldNotActionPlayerReason", "InviteString", player_name, "PlayerIsAlreadyInEvent" );
                        chatSendToPlayer( id, buf, INFO_ARENA_ERROR, 0 );
                        break;
                    }

                    if( isIgnored( ent, stack_tmp_dbid ) )
                    {
                        char *buf = localizedPrintf( ent, "CouldNotActionPlayerReason", "InviteString", player_name, "PlayerHasIgnoredYou" );
                        chatSendToPlayer( stack_tmp_dbid, buf , INFO_ARENA_ERROR, 0 );
                        break;
                    }

                    if( ent->pl->inviter_dbid ) // already being invited to another team
                    {
                        char *buf = localizedPrintf(ent,"CouldNotActionPlayerReason", "InviteString", player_name, "PlayerIsConsideringAnotherOffer");
                        chatSendToPlayer(id, buf, INFO_ARENA_ERROR, 0 );
                        break;
                    }
                    else if ( ent->pl->last_inviter_dbid == stack_tmp_dbid && (dbSecondsSince2000()-ent->pl->last_invite_time) < BLOCK_INVITE_TIME )
                    {
                        char *buf = localizedPrintf(ent,"CouldNotActionPlayerReason", "InviteString", player_name, "MustWaitLonger");
                        chatSendToPlayer(id, buf, INFO_ARENA_ERROR, 0 );
                        break;
                    }

                    {
                        char *buf = localizedPrintf( ent, "ArenaInviteSent" );
                        chatSendToPlayer( stack_tmp_dbid, buf , INFO_ARENA, 0 );
                    }

                    ent->pl->inviter_dbid = ent->pl->last_inviter_dbid = stack_tmp_dbid; // remember the inviter, so we can trap bad commands
                    ent->pl->last_invite_time = dbSecondsSince2000();
                    //ent->pl->arenaTeleportTimer = dbSecondsSince2000() + ARENA_SCHEDULED_TELEPORT_WAIT;
                    //ent->pl->arenaTeleportEventId = stack_tmp_int;
                    //ent->pl->arenaTeleportUniqueId = stack_tmp_int2;

                    START_PACKET( pak, ent, SERVER_ARENA_OFFER )
                    pktSendBits( pak, 32, stack_tmp_dbid );
                    pktSendString( pak, tmp_str );
                    pktSendBits( pak, 32, stack_tmp_int );
                    pktSendBits( pak, 32, stack_tmp_int2);
                    END_PACKET
                }

            }break;

        case CMD_ARENA_INVITE_ACCEPT:
            {
                int result = 0;

                if( e && e->pl ) // first make sure the inviter is the guy who actually invited
                {
                    if ( e->pl->inviter_dbid != stack_tmp_dbid )
                    {
                        // TODO: Mark player is suspicious...and take away their candy
                        e->pl->inviter_dbid = 0;
                        break;
                    }

                    e->pl->inviter_dbid = e->pl->last_inviter_dbid = 0; // reset value so someone else can invite
                }

                // let the arena server handle it from here
                ArenaRequestJoin( e, tmp_int, tmp_int2, true, true );
                e->pl->arenaTeleportAccepted = 1;

                
            }break;

        case CMD_ARENA_INVITE_DECLINE:
            {
                if( e && e->pl ) // first make sure the inviter is the guy who actually invited
                {
                    if ( e->pl->inviter_dbid != stack_tmp_dbid )
                    {
                        // TODO: Mark player is suspicious...and take away their candy
                        e->pl->inviter_dbid = 0;
                        break;
                    }
                    e->pl->inviter_dbid = 0; // reset value so someone else can invite
                }

                chatSendToPlayer( stack_tmp_dbid, localizedPrintf(e, "declineArenaOffer", e->name), INFO_ARENA_ERROR, 0 );

            } break;

        //------------------------------------------------------------------------------------------------
        // Instant raids //////////////////////////////////////////////////////////////////////////////////////
        //------------------------------------------------------------------------------------------------
        case CMD_RAID_INVITE:
            {
                // player_name:        target player

                char *buf = 0;
                char buffer[512];

                if(!e || !e->pl)
                    break;

                if (!player_name[0])
                {
                    chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat", 
                        "/sgraid_invite", "player_name", " ", " ") , INFO_USER_ERROR, 0);
                    break;
                }
    
                buf = raid_CheckInviterConditions( e, player_name );

                if(buf)
                {
                    chatSendToPlayer(e->db_id, buf, INFO_USER_ERROR, 0);
                    break;
                }

                sprintf( buffer, "%s \"%s\" \"%s\" %d %d \"%s\" %d", "raid_invite_relay", player_name, e->name, e->db_id, e->supergroup_id, e->supergroup->name, e->playerLocale);
                serverParseClient( buffer, NULL );

            }break;

        case CMD_RAID_INVITE_RELAY:
            {
                int result = 0;
                Entity* ent = findEntOrRelay( NULL, player_name, stack_tmp_str, &result );
                char *leader_name;

                // stack_tmp_str:    original command ("coalition_invite_relay ...")
                // player_name:        invitee name
                // tmp_str:            inviter name
                // stack_tmp_dbid:    inviter dbid
                // stack_tmp_int:    inviter supergroup dbid
                // tmp_str2:        inviter supergroup name
                // stack_tmp_int2:    inviter playerLocale

                if( ent )
                {
                    char * buf = raid_CheckInviteeConditions( ent, stack_tmp_dbid, stack_tmp_int );
                    if(buf)
                    {
                        chatSendToPlayer(stack_tmp_dbid, buf, INFO_USER_ERROR, 0);
                        break;
                    }

                    // find the eldest-on leader
                    leader_name = sgroup_FindLongestWithPermission(ent, SG_PERM_RAID_SCHEDULE);
                    if (!leader_name || !leader_name[0])
                    {
                         chatSendToPlayer(stack_tmp_dbid, localizedPrintf(ent,"NoRaidLeaderOnline"), INFO_USER_ERROR, 0);
                        break;
                    }

                    // if this is not the leader, relay the command to the leader
                    if (strcmp(leader_name, ent->name)!=0)
                    {
                        char buffer[1024];
                        sprintf( buffer, "%s \"%s\" \"%s\" %d %d \"%s\" %d", "raid_invite_relay", leader_name, tmp_str, stack_tmp_dbid, stack_tmp_int, tmp_str2, stack_tmp_int2);
                        serverParseClient( buffer, NULL );
                        break;
                    }


                    if( isIgnored( ent, stack_tmp_dbid ) )
                    {
                        buf = localizedPrintf( ent, "CouldNotActionPlayerReason", "InviteString", player_name, "PlayerHasIgnoredYou" );
                    }
                    else if( ent->pl->inviter_dbid ) // already being invited to another team
                    {
                        buf = localizedPrintf( ent, "CouldNotActionPlayerReason", "InviteString", player_name, "PlayerIsConsideringAnotherOffer" );
                    }
                    else if ( ent->pl->last_inviter_dbid == stack_tmp_dbid && (dbSecondsSince2000()-ent->pl->last_invite_time) < BLOCK_INVITE_TIME  )
                    {
                        buf = localizedPrintf( ent, "CouldNotActionPlayerReason", "InviteString", player_name, "MustWaitLonger" );
                    }

                    if(buf)
                    {
                        chatSendToPlayer(stack_tmp_dbid, buf, INFO_USER_ERROR, 0);
                        break;
                    }

                    ent->pl->inviter_dbid = ent->pl->last_inviter_dbid = stack_tmp_dbid; // remember the inviter, so we can trap bad commands
                    ent->pl->inviter_sgid = stack_tmp_int;
                    ent->pl->last_invite_time = dbSecondsSince2000();

                    chatSendToPlayer( stack_tmp_dbid, localizedPrintf( ent, "RaidInviteSent" ) , INFO_SVR_COM, 0 );

                    START_PACKET( pak, ent, SERVER_SGRAID_OFFER )
                        pktSendBits( pak, 32, stack_tmp_dbid );    // inviter dbid
                        pktSendBits( pak, 32, stack_tmp_int );    // inviter sg dbid
                        pktSendString( pak, tmp_str );            // inviter name
                        pktSendString( pak, tmp_str2 );            // inviter sg name
                    END_PACKET
                }
                else if (!result)
                {
                    chatSendToPlayer(stack_tmp_dbid, localizedPrintf(0, "playerNotFound", player_name), INFO_USER_ERROR, 0);
                }
            }break;

        case CMD_RAID_ACCEPT:
            {
                // stack_tmp_str:    original command ("coalition_accept ...")
                // player_name:        inviter(acceptee) name
                // stack_tmp_dbid:    inviter(acceptee) dbid
                // stack_tmp_int:    inviter(acceptee) supergroup dbid

                char *buf = 0;

                if (!e || !e->pl)
                    break;

                if (stack_tmp_dbid != e->pl->inviter_dbid ||
                    stack_tmp_int != e->pl->inviter_sgid)
                {
                    dbLog("cheater", e, "Spoofed raid accept - may be unintentional if it only happens once (%i,%i)", stack_tmp_dbid, stack_tmp_int);
                    e->pl->inviter_dbid = 0;
                    e->pl->inviter_sgid = 0;
                    break;
                }

                buf = raid_CheckInviteeConditions(e, stack_tmp_dbid, stack_tmp_int);
                
                e->pl->inviter_dbid = e->pl->last_inviter_dbid = 0;
                svrSendEntListToDb(&e, 1);

                if (buf)
                {
                    chatSendToPlayer(e->db_id, buf, INFO_USER_ERROR, 0);
                    break;
                }
                RaidAcceptInstantInvite(e, stack_tmp_dbid, stack_tmp_int);
            }break;

        case CMD_RAID_DECLINE:
            {
                // stack_tmp_str:    original command ("coalition_decline ...")
                // player_name:        inviter(acceptee) name
                // stack_tmp_dbid:    inviter(acceptee) dbid
                // stack_tmp_int:    inviter(acceptee) supergroup dbid

                int result = 0;
                Entity *inviter = 0;

                if (e && e->pl)
                {
                    if (e->pl->inviter_dbid != stack_tmp_dbid)
                    {
                        dbLog("cheater", e, "Spoofed raid decline - may be unintentional if it only happens once (%i,%i)", stack_tmp_dbid, stack_tmp_int);
                        e->pl->inviter_dbid = 0;
                        e->pl->inviter_sgid = 0;
                        break;
                    }
                    e->pl->inviter_dbid = 0; // reset value so someone else can invite
                    e->pl->inviter_sgid = 0;
                }

                chatSendToPlayer(stack_tmp_dbid, localizedPrintf(inviter,"declineRaidOffer", e->name), INFO_SVR_COM, 0);

            } break;

        case CMD_REWARDCHOICE_CLEAR:
            {
                clearRewardChoice(e, 0);
            } break;
        case CMD_ENDGAMERAID_VOTE_KICK:
            {
                if (e && e->pl)
                {
                    if (db_state.is_endgame_raid)
                    {
                        if (!turnstileMapserver_eventLockedByGroup())
                        {
                            int kicked_dbid = dbPlayerIdFromName(player_name);
                            if (kicked_dbid)
                            {
                                Entity *kicked = entFromDbId(kicked_dbid);
                                if (kicked)
                                {
                                    devassert(league_IsMember(e, kicked_dbid));
                                    if (league_IsMember(e, kicked_dbid))
                                    {
                                        EndGameRaidRequestVoteKick(e, kicked);
                                    }
                                    else
                                    {
                                        leaveMission(kicked, kicked->static_map_id, 1);
                                    }
                                }
                                else
                                {
                                    //    error that player isn't on the map
                                    chatSendToPlayer(e->db_id, localizedPrintf(e, "EGRVK_NotOnMap"), INFO_USER_ERROR, 0);
                                }
                            }
                            else
                            {
                                //    error that player doesn't exist
                                chatSendToPlayer(e->db_id, localizedPrintf(e, "EGRVK_InvalidPlayer"), INFO_USER_ERROR, 0);
                            }
                        }
                        else
                        {
                            chatSendToPlayer(e->db_id, localizedPrintf(e, "EGRVK_EventNotLocked"), INFO_USER_ERROR, 0);
                        }
                    }
                    else
                    {
                        chatSendToPlayer(e->db_id, localizedPrintf(e, "EGRVK_NotEndgameRaidMap"), INFO_USER_ERROR, 0);
                    }
                }
            } break;
        case CMD_ENDGAMERAID_VOTE_KICK_OPINION:
            {
                if (e && e->pl)
                {
                    EndGameRaidReceiveVoteKickOpinion(e->db_id, stack_tmp_int);
                }
            } break;
    }

    //clear out tmp variables
    tmp_str[0] = '\0';
    player_name[0] = '\0';
    tmp_int = tmp_int2 = tmp_int3 = tmp_int4 = tmp_dbid = 0;
}

void cmdOldChatCheckCmds(void)
{
    int i, k, m;
    Cmd *cmds = chat_cmds;
    for(i=0;cmds[i].name;i++)
    {
        bool b=false;
        Cmd *cmd = &cmds[i];
        if (cmd->flags & CMDF_HIDEVARS)
            continue;
        for (k=0; k < cmd->num_args && !b; k++)
        {
            DataDesc *dd = &cmd->data[k];
            for (m=0; m<ARRAY_SIZE(tmp_var_list) && !b; m++) {
                if (dd->ptr == tmp_var_list[m]) {
/*                    Errorf("\
Found command \"%s\" in cmdChat.c using tmp_* but not marked as CMDF_HIDEVARS.\nThis means that if someone types the \
command with no parameters it will show the value of the tmp_* variable (which may have been the parameter \
to someone else's command if this is a server-side command), and will fail to display the handy usage \
instructions.", cmd->name); */
                    // Okay, so there are too many wrong to error, let's just set these smartlyish...
                    cmd->flags |= CMDF_HIDEVARS; 
                    b=true;
                }
            }
        }
    }
}
