/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include "auction/auction.h"
#include <utilitieslib/network/crypt.h>
#include <utilitieslib/components/HashFunctions.h>
#include "entity/Concept.h"
#include "container/dbcontainerpack.h"
#include "entity/entity.h"
#include "varutils.h"
#include "containerloadsave.h"
#include "dbcomm/dbcontainer.h"
#include "dbcomm/dbcomm.h"
#include "gameComm/sendToClient.h"
#include "player/parseClientInput.h"
#include "entity/entVarUpdate.h"
#include "comm_game.h"
#include "map.h"
#include "gameComm/trayServer.h"
#include "gameComm/wdwbase.h"
#include "entity/costume.h"    // for costume structures
#include "gameData/BodyPart.h"
#include "gameData/costume_data.h"
#include <utilitieslib/components/earray.h>

#include "entity/character_base.h"
#include "entity/character_db.h"
#include "entity/classes.h"
#include "entity/origins.h"
#include "entity/powers.h"
#include "storyarc/contact.h"
#include "storyarc/storyarcprivate.h"
#include "storyarc/task.h"
#include "dbcomm/dbdoor.h"
#include "entity/friends.h"
#include "entity/friendCommon.h"
#include "containerEmail.h"
#include "entity/entPlayer.h"
#include <utilitieslib/components/bitfield.h>

#include "dbcomm/staticMapInfo.h"

#include "entity/keybinds.h"
#include <utilitieslib/utils/error.h>
#include "entity/SgrpServer.h"
#include "player/pl_stats_db.h"
#include <utilitieslib/utils/timing.h>
#include "team.h"
#include "entity/teamup.h"
#include "entity/costume_db.h"
#include "player/badges_server.h"
#include "entity/SgrpBadges.h"
#include "player/BadgeStats.h"
#include "mapgroup.h"
#include "cmdparse/cmdservercsr.h"
#include "reward.h"
#include "containerArena.h"
#include "gameSys/arenamap.h"
#include "gameComm/trayCommon.h"
#include "containerInventory.h"
#include "entity/character_inventory.h"
#include "entity/character_combat.h"
#include "entity/Proficiency.h"
#include "svr/svr_player.h"
#include "entity/pvp.h"
#include "cmdparse/cmdserver.h"
#include "entity/attribmod.h"
#include "entity/Supergroup.h"
#include "containerSupergroup.h"
#include "entity/RewardToken.h"
#include "gameComm/pet.h"
#include "bases/basedata.h"
#include "container/container_store.h"
#include "gameData/raidstruct.h"
#include "gameData/sgrpstatsstruct.h"
#include "bases/DetailRecipe.h"
#include "entity/mapHistory.h"
#include "gameData/miningaccumulator.h"
#include "entity/TaskforceParams.h"
#include "entity/attrib_description.h"
#include "entity/chatSettings.h"
#include <utilitieslib/language/AppLocale.h> //for locisUserSelectable and getCurrentLocale
#include "storyarc/playerCreatedStoryarcServer.h"
#include "gameData/statgroupstruct.h"
#include "entity/character_level.h"
#include "player/badges.h"
#include "baseloadsave.h"
#include "gameSys/sgraid.h"
#include "entity/power_customization.h"
#include "player/pophelp.h"
#include "entity/alignment_shift.h"
#include "auth/authUserData.h"
#include "dbcomm/dbmapxfer.h"
#include "container/containerEventHistory.h"
#include "cmdparse/autoCommands.h"
#include "account/AccountInventory.h"
#include "dbcomm/logcomm.h"
#include "MARTY.h"
#include "zowie.h"
#include "gameComm/automapServer.h"
#define MAX_BUF 16384

extern DBPowers s_dbpows;

typedef struct TestDataBaseTypes
{
    char test_byte;
    short test_short;
    int test_int;
    float test_float;
    char test_utf8[32];
    char test_ascii[32];
    char test_attr[MAX_ATTRIBNAME_LEN];
    int test_conref;
    U32 test_date;
    char * test_estr_utf8;
    char * test_estr_ascii;
    const char * test_utf8_cached;
    const char * test_ascii_cached;
    char test_bin[8];
    char * test_large_estr_binary;
    char * test_large_estr_utf8;
    char * test_large_estr_ascii;
} TestDataBaseTypes;

/////////////////////////////////////////////////////////////////////////////////
// Entity data block
/////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------
// TestDataBaseTypes description
//---------------------------------------------------------------------------------

LineDesc testdatabasetypes_line_desc[] =
{
    {{PACKTYPE_INT, SIZE_INT8, "test_byte", OFFSET(TestDataBaseTypes, test_byte),}, {"test_byte"}},
    {{PACKTYPE_INT, SIZE_INT16, "test_short", OFFSET(TestDataBaseTypes, test_short),}, {"test_short"}},
    {{PACKTYPE_INT, SIZE_INT32, "test_int", OFFSET(TestDataBaseTypes, test_int),}, {"test_int"}},
    {{PACKTYPE_FLOAT, SIZE_FLOAT32, "test_float", OFFSET(TestDataBaseTypes, test_float),}, {"test_float"}},
    {{PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN, "test_attr", OFFSET(TestDataBaseTypes, test_attr),}, {"test_attr"}},
    {{PACKTYPE_CONREF, CONTAINER_MAPS, "test_conref", OFFSET(TestDataBaseTypes, test_conref),}, {"test_conref"}},
    {{PACKTYPE_DATE, 0, "test_date", OFFSET(TestDataBaseTypes, test_date),}, {"test_date"}},
    //{{PACKTYPE_SUB, 1, "test_sub", (int)null_desc,}, {"test_sub"}},
    //{{PACKTYPE_EARRAY, (int)null_alloc, "test_earray", (int)null_desc,}, {"test_earray"}},
    {{PACKTYPE_STR_UTF8, SIZEOF2(TestDataBaseTypes, test_utf8), "test_utf8", OFFSET(TestDataBaseTypes, test_utf8),}, {"test_utf8"}},
    {{PACKTYPE_STR_ASCII, SIZEOF2(TestDataBaseTypes, test_ascii), "test_ascii", OFFSET(TestDataBaseTypes, test_ascii),}, {"test_ascii"}},
    {{PACKTYPE_ESTRING_UTF8, SIZEOF2(TestDataBaseTypes, test_estr_utf8), "test_estr_utf8", OFFSET(TestDataBaseTypes, test_estr_utf8),}, {"test_estr_utf8"}},
    {{PACKTYPE_ESTRING_ASCII, SIZEOF2(TestDataBaseTypes, test_estr_ascii), "test_estr_ascii", OFFSET(TestDataBaseTypes, test_estr_ascii),}, {"test_estr_ascii"}},
    {{PACKTYPE_STR_UTF8_CACHED, 32, "test_utf8_cached", OFFSET(TestDataBaseTypes, test_utf8_cached),}, {"test_utf8_cached"}},
    {{PACKTYPE_STR_ASCII_CACHED, 32, "test_ascii_cached", OFFSET(TestDataBaseTypes, test_ascii_cached),}, {"test_ascii_cached"}},
    {{PACKTYPE_BIN_STR, SIZEOF2(TestDataBaseTypes, test_bin)*4, "test_bin", OFFSET(TestDataBaseTypes, test_bin),}, {"test_bin"}},
    {{PACKTYPE_LARGE_ESTRING_BINARY, 0, "test_large_estr_binary", OFFSET(TestDataBaseTypes, test_large_estr_binary),}, {"test_large_estr_binary"}},
    {{PACKTYPE_LARGE_ESTRING_UTF8, 0, "test_large_estr_utf8", OFFSET(TestDataBaseTypes, test_large_estr_utf8),}, {"test_large_estr_utf8"}},
    {{PACKTYPE_LARGE_ESTRING_ASCII, 0, "test_large_estr_ascii", OFFSET(TestDataBaseTypes, test_large_estr_ascii),}, {"test_large_estr_ascii"}},
    { 0 },
};

StructDesc testdatabasetypes_desc[] =
{
    sizeof(TestDataBaseTypes), {AT_NOT_ARRAY,{0}}, testdatabasetypes_line_desc,
    "This table stores is used for testing all database types and is not used by the game."
};

LineDesc testdatabasetypes_schema_desc[] =
{
    { PACKTYPE_SUB, 1, "TestDataBaseTypes", (intptr_t)testdatabasetypes_desc},
    { 0 }
};

//---------------------------------------------------------------------------------
// Pet Name description
//---------------------------------------------------------------------------------

LineDesc petname_line_desc[] =
{
    {{ PACKTYPE_STR_ASCII,     MAX_NAME_LEN,                    "PowerName",OFFSET(PetName, DEPRECATED_pchPowerName ),    },
        {"The internal name of the power that creates the pet"}},

    {{ PACKTYPE_INT,     SIZE_INT32,                        "PetNumber",OFFSET(PetName, petNumber ),       },
        "The 0-based index of the pet. For instance, for a first level Mastermind power<br>"
        "this could be 0-2"},

    {{ PACKTYPE_STR_UTF8,        SIZEOF2(PetName, petName),        "PetName",    OFFSET(PetName, petName ),           },
        "The custom name of the pet."},

    {{ PACKTYPE_STR_ASCII,     MAX_NAME_LEN,                    "EntityDef",OFFSET(PetName, pchEntityDef ),    },
        {"The internal name of the entity def that creates the pet"}},

    { 0 },
};

StructDesc petname_desc[] =
{
    sizeof(PetName),
    {AT_EARRAY, OFFSET2_PTR(Entity, pl, EntPlayer, petNames )},
    petname_line_desc,

    "This table stores the custom names a player has for their pets. Each "
    "individual pet that has been assigned a name has their own row."
};

//---------------------------------------------------------------------------------
// Map History description
//---------------------------------------------------------------------------------

LineDesc maphistory_line_desc[] =
{
    {{ PACKTYPE_INT,     SIZE_INT32,        "MapType",            OFFSET(MapHistory, mapType ),   },
        "The type of map:<br>"
        "   0 = Static<br>"
        "   1 = Mission<br>"
        "   2 = Arena<br>"
        "   3 = Base"},

    {{ PACKTYPE_INT,     SIZE_INT32,        "MapID",            OFFSET(MapHistory, mapID ),        },
        "The unique id for the map. Map number for static and mission maps, event id for arenas, and supergroup id for bases"},

    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,            "PosX",                OFFSET(MapHistory, last_pos[0]) },
        "Last location of the character on the map."},
    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,            "PosY",                OFFSET(MapHistory, last_pos[1]) },
        "Last location of the character on the map."},
    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,            "PosZ",                OFFSET(MapHistory, last_pos[2]) },
        "Last location of the character on the map."},

    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,            "OrientP",            OFFSET(MapHistory, last_pyr[0]) },
        "Orientation of the character the last time they were on the map."},
    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,            "OrientY",            OFFSET(MapHistory, last_pyr[1]) },
        "Orientation of the character the last time they were on the map."},
    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,            "OrientR",            OFFSET(MapHistory, last_pyr[2]) },
        "Orientation of the character the last time they were on the map."},

    { 0 },
};

StructDesc maphistory_desc[] =
{
    sizeof(MapHistory),
    {AT_EARRAY, OFFSET2_PTR(Entity, pl, EntPlayer, mapHistory )},
    maphistory_line_desc,

    "This table has a list of all of the maps a player has been to since the last static map."
};


//---------------------------------------------------------------------------------
//  InvBaseDetail description
//---------------------------------------------------------------------------------

static const char* s_nameFromDetail(const Detail *item)
{
    if( verify(item) )
    {
        return item->pchName;
    }
    return "";
}

LineDesc invbasedetail_line_desc[] =
{
    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,        "name",                OFFSET(DetailInventoryItem, item), (IntFromStr*)detail_GetItem, (StrFromInt*)s_nameFromDetail} ,
        "TODO"},

    {{ PACKTYPE_INT,    SIZE_INT32,        "amount",    OFFSET(DetailInventoryItem, amount) },
        "TODO"},


    { 0 },
};

StructDesc invbasedetail_desc[] =
{
    sizeof(DetailInventoryItem),
    {AT_EARRAY, OFFSET2_PTR(Entity, pchar, Character, detailInv)},
    invbasedetail_line_desc,

    "TODO: A brief description of this table. Include if this is a 1:1 "
    "or 1:n mapping to the ents table. See stat_line_desc below for an "
    "example."
};

LineDesc certificationrecord_line_desc[] =
{
    {{ PACKTYPE_INT,     SIZE_INT32,        "claimed",    OFFSET(CertificationRecord, claimed) },
        "Number of items claimed by character"},
    {{ PACKTYPE_INT,     SIZE_INT32,        "deleted",    OFFSET(CertificationRecord, deleted) },
        "Number of deleted items, not actually deleted, just hidden on UI"},        
    {{ PACKTYPE_LARGE_ESTRING_ASCII, 0,        "Name",        OFFSET(CertificationRecord, pchRecipe) }, // estring blobs must be last
        "Unique description for this item"},
    { 0 },
};

StructDesc certificationhistory_desc[] = 
{
    sizeof(CertificationRecord),
    {AT_EARRAY, OFFSET2_PTR(Entity, pl, EntPlayer, ppCertificationHistory)},
    certificationrecord_line_desc,

    "This table holds a character history of interactions with certifications"
};

extern StructDesc tray_desc[];

//---------------------------------------------------------------------------------
// Costume Reward
//---------------------------------------------------------------------------------

//------------------------------------------------------------
//  @see MapServer/container/containerSupergroup.c:sgrp_reward_token_line_desc
//----------------------------------------------------------
static LineDesc reward_token_line_desc[] =
{
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN, "PieceName", OFFSET(RewardToken, reward) },
        "The keyname of the reward"},

    {{ PACKTYPE_INT,  SIZE_INT32, "RewardValue", OFFSET(RewardToken, val) },
        "The count (if applicable) of the reward"},
    {{ PACKTYPE_INT,  SIZE_INT32, "RewardTime", OFFSET(RewardToken, timer) },
        "reward token timer, usually last time rewarded"},
    { 0 },
};

StructDesc player_reward_token_desc[] =
{
    sizeof(RewardToken),
    {AT_EARRAY, OFFSET2_PTR(Entity, pl, EntPlayer, rewardTokens )},
    reward_token_line_desc,
    "RewardToken table is a list of all the custom player rewards.<br>"
    "Each row represents a single reward.<br>"
    "Each character can have many rewards."
};

StructDesc active_player_reward_token_desc[] =
{
    sizeof(RewardToken),
    {AT_EARRAY, OFFSET2_PTR(Entity, pl, EntPlayer, activePlayerRewardTokens )},
    reward_token_line_desc,
    "RewardToken table is a list of all the custom player rewards.<br>"
    "Each row represents a single reward.<br>"
    "Each character can have many rewards."
};



//---------------------------------------------------------------------------------
// Friend list description
//---------------------------------------------------------------------------------
LineDesc friend_list_line_desc[] =
{
    {{ PACKTYPE_CONREF, CONTAINER_ENTS, "Id",            OFFSET(Friend,dbid),        },
        "DB ID - The ContainerID of the character's friend"},

    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,        "Class",        OFFSET(Friend,arch),        },
        "The archetype (class) of the character's friend"},

    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,        "Origin",        OFFSET(Friend,origin),        },
        "The origin of the character's friend"},

    {{ PACKTYPE_STR_UTF8,  SIZEOF2(Friend,description),    "Description",    OFFSET(Friend,description), },
        "Unused at the moment."},

    { 0 },
};

StructDesc friend_list_desc[] =
{
    sizeof(Friend),
    {AT_STRUCT_ARRAY, OFFSET_PTR(Entity,friends)},
    friend_list_line_desc,

    "The Friends table is a list of all of the friends for every character.<br>"
    "Each row represents a single friend for a specific character. Each<br>"
    "character will likely have multiple entires in the table."
};

//---------------------------------------------------------------------------------
// Defeat list description
//---------------------------------------------------------------------------------
LineDesc defeat_list_line_desc[] =
{
    {{ PACKTYPE_CONREF, CONTAINER_ENTS, "VictorId",            OFFSET(DefeatRecord,victor_id),        },
        "TODO"},

    {{ PACKTYPE_INT, SIZE_INT32, "DefeatTime", OFFSET(DefeatRecord, defeat_time) },
        "TODO"},

    { 0 },
};

StructDesc defeat_list_desc[] =
{
    sizeof(DefeatRecord),
    {AT_STRUCT_ARRAY, OFFSET2_PTR(Entity, pl, EntPlayer, pvpDefeatList )},
    defeat_list_line_desc,

    "TODO: A brief description of this table. Include if this is a 1:1 "
    "or 1:n mapping to the ents table. See stat_line_desc below for an "
    "example."
};

//---------------------------------------------------------------------------------
// Chat settings description
//---------------------------------------------------------------------------------


LineDesc chat_settings_window_line_desc[] =
{
    {{ PACKTYPE_INT,     SIZE_INT32,     "TabList",        OFFSET(ChatWindowSettings, tabListBF),        },
        "Bitfield - Tabs included in window"},

    {{ PACKTYPE_INT,     SIZE_INT32,     "SelectedTab",    OFFSET(ChatWindowSettings, selectedtop),    },
        "The selected tab id in the top pane"},

    {{ PACKTYPE_INT,     SIZE_INT32,     "SelectedTabBot",    OFFSET(ChatWindowSettings, selectedbot),    },
        "The selected tab id in the bottom pane"},

    {{ PACKTYPE_FLOAT,        SIZE_INT32,     "Divider",        OFFSET(ChatWindowSettings, divider),        },
        "The height of the divider making the chat window two panes."},

    { 0 },
};

StructDesc chat_settings_window_desc[] =
{
    sizeof(ChatWindowSettings),
    {AT_STRUCT_ARRAY, { INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, chat_settings, 0), INDIRECTION(ChatSettings, windows, 0) } },
    chat_settings_window_line_desc,

    "This table includes all of the UI data for each chat window.<br>"
    "Each row describes a single window.<br>"
    "There can be multiple windows per Ent."
};


LineDesc chat_settings_tab_line_desc[] =
{
    {{ PACKTYPE_STR_UTF8,     SIZEOF2(ChatTabSettings, name),    "TabName",            OFFSET(ChatTabSettings, name),            },
        "The name of the tab"},

    // Deprecated
    {{ PACKTYPE_INT,     SIZE_INT32,                        "SystemChannels",    OFFSET(ChatTabSettings, systemBF),            },
        "Bitfield - the set of system channels to display in the tab - Deprecated"},

    {{ PACKTYPE_INT,     SIZE_INT32,                        "UserChannels",        OFFSET(ChatTabSettings, userBF),            },
        "Bitfield - the set of user channels to display in the tab"},

    {{ PACKTYPE_INT,     SIZE_INT32,                        "TabOptions",        OFFSET(ChatTabSettings, optionsBF),            },
        "Bitfield - where to display tab"},

    {{ PACKTYPE_INT,     SIZE_INT32,                        "DefaultChannel",    OFFSET(ChatTabSettings, defaultChannel),    },
        "The id of the default output channel on this tab."},

    {{ PACKTYPE_INT,     SIZE_INT32,                        "DefaultType",        OFFSET(ChatTabSettings, defaultType),        },
        "If this tab is system created or user created<br>"
        "   1 = System<br>"
        "   2 = User"},

    {{ PACKTYPE_BIN_STR, SYSTEM_CHANNEL_BITFIELD_SIZE*4,    "SystemChannelsBitField", OFFSET(ChatTabSettings, systemChannels),    },
        "Bitfield - the set of system channels to display in the tab"},

    { 0 },
};

StructDesc chat_settings_tab_desc[] =
{
    sizeof(ChatTabSettings),
    {AT_STRUCT_ARRAY, { INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, chat_settings, 0), INDIRECTION(ChatSettings, tabs, 0) } },
    chat_settings_tab_line_desc,

    "This describes all the information a chat-tab needs to save.<br>"
    "Chat tabs are used to organize communication in the chat window.<br>"
    "Each row describes a single chat tab.<br>"
    "There can be many tabs per chat window."
};

LineDesc chat_settings_channel_line_desc[] =
{
    {{ PACKTYPE_STR_UTF8,     SIZEOF2(ChatChannelSettings, name),    "ChannelName",        OFFSET(ChatChannelSettings, name),        },
        "The name of the channel"},

    {{ PACKTYPE_INT,     SIZE_INT32,                            "ChannelOptions",    OFFSET(ChatChannelSettings, optionsBF),    },
        "Unused"},
    {{ PACKTYPE_INT,     SIZE_INT32,                            "Color1",    OFFSET(ChatChannelSettings, color1),    },
            "Color for this chat channel"},
    {{ PACKTYPE_INT,     SIZE_INT32,                            "Color2",    OFFSET(ChatChannelSettings, color2),    },
            "Color for this chat channel"},
    { 0 },
};

StructDesc chat_settings_channel_desc[] =
{
    sizeof(ChatChannelSettings),
    {AT_STRUCT_ARRAY, { INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, chat_settings, 0), INDIRECTION(ChatSettings, channels, 0) } },
    chat_settings_channel_line_desc,

    "This table describes a chat channel, used by the chat window.<br>"
    "Each row is a single channel<br>"
    "Characters can have multiple channels."
};

//---------------------------------------------------------------------------------
// Combat Monitor description
//---------------------------------------------------------------------------------
static LineDesc combat_monitor_stat_line_desc[] =
{
    {{ PACKTYPE_INT,  SIZE_INT32, "iKey", OFFSET(CombatMonitorStat, iKey) },
        "Key to the attribute description"},
    {{ PACKTYPE_INT,  SIZE_INT32, "iOrder", OFFSET(CombatMonitorStat, iOrder) },
        "Order in the list"},
    { 0 },
};

StructDesc combatMonitorStat_desc[] =
{
    sizeof(CombatMonitorStat),
    {AT_STRUCT_ARRAY, OFFSET2_PTR(Entity, pl, EntPlayer, combatMonitorStats) },
    combat_monitor_stat_line_desc,

    "The Combat Monitor Stats table is a list of unique keys OF attributes the player is currently monitoring<br>"
    "Each row represents a key/index pair<br>"
    "character will likely have multiple entires in the table."
};

//---------------------------------------------------------------------------------
// Recent badges description
//---------------------------------------------------------------------------------
static LineDesc recent_badge_line_desc[] =
{
    {{ PACKTYPE_INT, SIZE_INT32, "Idx", OFFSET(RecentBadge, idx) },
        "Index of the badge" },
    {{ PACKTYPE_INT, SIZE_INT32, "TimeAwarded", OFFSET(RecentBadge, timeAwarded) },
        "When the badge was earned" },
    { 0 },
};

StructDesc recentBadge_desc[] =
{
    sizeof(RecentBadge),
    {AT_STRUCT_ARRAY, OFFSET2_PTR(Entity, pl, EntPlayer, recentBadges) },
    recent_badge_line_desc,
    "The most recent 50 badges awarded to the player<br>"
    "Each row gives the badge index and time awarded<br>"
    "Characters can have 0 to 50 of these."
};

//---------------------------------------------------------------------------------
// Badge monitor description
//---------------------------------------------------------------------------------
static LineDesc badge_monitor_info_line_desc[] =
{
    {{ PACKTYPE_INT,  SIZE_INT32, "iIdx", OFFSET(BadgeMonitorInfo, iIdx) },
        "Index of the badge"},
    { 0 },
};

StructDesc badgeMonitor_desc[] =
{
    sizeof(BadgeMonitorInfo),
    {AT_STRUCT_ARRAY,OFFSET2_PTR(Entity, pl, EntPlayer, badgeMonitorInfo) },
    badge_monitor_info_line_desc,
    "The badge monitor info is a list of badge indices"
    "in the display list for that badge's progress bar<br>"
    "Characters can have 0 to 10 of these."
};

//---------------------------------------------------------------------------------
// Window description
//---------------------------------------------------------------------------------
LineDesc window_line_desc[] =
{
    {{ PACKTYPE_INT,     SIZE_INT32,     "xp",            OFFSET(WdwBase,xp),                    },
        "X location of window"},
    {{ PACKTYPE_INT,     SIZE_INT32,     "yp",            OFFSET(WdwBase,yp),                    },
        "Y location of window"},

    {{ PACKTYPE_INT,     SIZE_INT32,     "wd",            OFFSET(WdwBase,wd),                    },
        "Width of window"},

    {{ PACKTYPE_INT,     SIZE_INT32,     "ht",            OFFSET(WdwBase,ht),                    },
        "Height of window"},

    {{ PACKTYPE_INT,     SIZE_INT32,     "dragFrame",    OFFSET(WdwBase,draggable_frame),    },
        "The window is resizable"},

    {{ PACKTYPE_INT,     SIZE_INT32,     "mode",            OFFSET(WdwBase,mode),            },
        "If the window is open or closed<br>"
        "   2 = displaying<br>"
        "   4 = docked"},

    {{ PACKTYPE_INT,     SIZE_INT32,     "locked",        OFFSET(WdwBase,locked),                },
        "If The window is attached to its parent"},

    {{ PACKTYPE_INT,     SIZE_INT32,     "Color",        OFFSET(WdwBase,color),                },
        "Color - The color of the window (used for borders and buttons)"},

    {{ PACKTYPE_INT,     SIZE_INT32,     "BackColor",    OFFSET(WdwBase,back_color),            },
        "Color - The background color of the window (used for filling)"},

    {{ PACKTYPE_FLOAT,     SIZE_FLOAT32,            "Scale",        OFFSET(WdwBase,sc),                    },
        "The scale to draw the window at."},

    {{ PACKTYPE_INT,     SIZE_INT32,    "Maximized",    OFFSET(WdwBase,maximized),            },
        "Is the window maximized."},

    { 0 },
};

StructDesc window_desc[] =

{
    sizeof(WdwBase),
    {AT_STRUCT_ARRAY, { INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, winLocs, 1) } },
    window_line_desc,

    "This table descibes UI windows.  It should only contain data if the user has changed the default settings.<br>"
    "Each row describes a single window.<br>"
    "Every character will have multiple windows."
};


//---------------------------------------------------------------------------------
// CompletedOrder description
//---------------------------------------------------------------------------------
LineDesc order_line_desc[] =
{
    {{ PACKTYPE_INT, SIZE_INT32,                            "OrderId0",    OFFSET(OrderId,    u32[0] ),    },
        "order id"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "OrderId1", OFFSET(OrderId,    u32[1] ),    },
        "order id"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "OrderId2", OFFSET(OrderId, u32[2] ),    },
        "order id"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "OrderId3", OFFSET(OrderId, u32[3] ),    },
        "order id"},

    { 0 },
};

StructDesc completed_order_desc[] =
{
    sizeof(OrderId),
    {AT_EARRAY, OFFSET2_PTR(Entity, pl, EntPlayer, completed_orders)},
    order_line_desc,

    "The CompleteOrder table specifies the list of orders that AccountServer can resolve as fully completed."
};

StructDesc pending_order_desc[] =
{
    sizeof(OrderId),
    {AT_EARRAY, OFFSET2_PTR(Entity, pl, EntPlayer, pending_orders)},
    order_line_desc,

    "The PendingOrder table specifies the list of orders that the map has rewarded, but the AccountServer cannot yet resolve as fully completed."
};


//---------------------------------------------------------------------------------
// Keybind description
//---------------------------------------------------------------------------------
LineDesc keybind_line_desc[] =
{
    {{ PACKTYPE_STR_UTF8,         SIZEOF2(ServerKeyBind, command),   "Command",  OFFSET(ServerKeyBind, command),   },
        "The string of the command to be executed when the given key is pressed"},

    {{ PACKTYPE_INT,         SIZE_INT32, "KeyCode",    OFFSET(ServerKeyBind, key),        },
        "The key code of the key"},

    {{ PACKTYPE_INT,         SIZE_INT32, "Modifier",    OFFSET(ServerKeyBind, modifier),    },
        "The modifier of the keystroke<br>"
        "  0 = NONE<br>"
        "  1 = CTRL<br>"
        "  2 = SHIFT<br>"
        "  3 = ALT"},

    { 0 },
};

StructDesc keybind_desc[] =
{
    sizeof(ServerKeyBind),
    {AT_STRUCT_ARRAY, { INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, keybinds, 1) }},
    keybind_line_desc,

    "The KeyBinds table specifies the customized keybinds for each character.<br>"
    "There is one keybind for each row, and multiple keybinds for each character."
};

//---------------------------------------------------------------------------------
// Costume description
//---------------------------------------------------------------------------------
LineDesc costume_part_line_desc[] =
{
    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,        "Name",            OFFSET(CostumePart, pchName),            },
        "Name of the bone this piece is attached to."},

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,       "Geom",            OFFSET(CostumePart, pchGeom),           },
        "Name of the geometry to attach to the bone"},

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,       "Tex1",            OFFSET(CostumePart, pchTex1),           },
        "Name of the primary texture to apply to this piece."},

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,       "Tex2",            OFFSET(CostumePart, pchTex2),           },
        "Name of the secondary texture to apply to this piece."},

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,        "DisplayName",    OFFSET(CostumePart, displayName),        },
        "The displayable name of the piece"},

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,       "Region",        OFFSET(CostumePart, regionName),        },
        "Which region of the body this is used in. (e.g. head, chest)"},

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,       "BodySet",        OFFSET(CostumePart, bodySetName),       },
        "TODO"},

    {{ PACKTYPE_INT,     SIZE_INT32,         "Color1",        OFFSET(CostumePart, color[0]),            },
        "Color - the primary color to apply to this piece"},

    {{ PACKTYPE_INT,     SIZE_INT32,         "Color2",        OFFSET(CostumePart, color[1]),            },
        "Color - the secondary color to apply to this piece"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "CostumeNum",    OFFSET(CostumePart, costumeNum),        },
        "The index of the costume that this piece belongs to"},

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,        "FxName",        OFFSET(CostumePart,    pchFxName),            },
        "The FX name to apply to this piece."},

    {{ PACKTYPE_INT,     SIZE_INT32,         "Color3",        OFFSET(CostumePart,    color[2]),            },
        "Color - the tertiary color to apply to this piece"},

    {{ PACKTYPE_INT,     SIZE_INT32,         "Color4",        OFFSET(CostumePart,    color[3]),            },
        "Color - the quaternary color to apply to this piece"},

    { 0 },
};


StructDesc costume_part_desc[] =
{
    sizeof(CostumePart),
    { AT_STRUCT_ARRAY, OFFSET(DBCostume, part) },
    costume_part_line_desc,

    "The CostumeParts table specifies the costumes for all the characters.<br>"
    "Each costume a character owns is represented with multiple rows in the table."
};

LineDesc costumes_line_desc[] =
{
    { PACKTYPE_SUB, MAX_COSTUMES*MAX_COSTUME_PARTS,  "CostumeParts", (intptr_t)costume_part_desc },
    { 0 },
};

StructDesc costumes_desc[] =
{
    sizeof(DBCostume),
    {AT_STRUCT_ARRAY,{0}},
    costumes_line_desc
};

StructDesc super_costume_part_desc =
{
    sizeof(CostumePart),
    {AT_POINTER_ARRAY,{ INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, superCostume, 1),  INDIRECTION(Costume, parts, 1)}},
    costume_part_line_desc
};

LineDesc appearance_line_desc[] =
{
    {{ PACKTYPE_INT,     SIZE_INT32,         "BodyType",        OFFSET(Appearance, bodytype),            },
        "Specifies the body type (skeleton) used by the character<br>"
        "   0 = Male<br>"
        "   1 = Female<br>"
        "   4 = Huge"},

    {{ PACKTYPE_INT,     SIZE_INT32,         "ColorSkin",    OFFSET(Appearance, colorSkin),            },
        "The color of the character's skin."},

    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,                "BodyScale",    OFFSET(Appearance, fScale),                },
        "The basic scales of the various overall body parts"},
    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,                "BoneScale",    OFFSET(Appearance, fBoneScale),            },
        "The basic scales of the various overall body parts"},
    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,                "HeadScale",    OFFSET(Appearance, fHeadScale),            },
        "The basic scales of the various overall body parts"},
    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,                "ShoulderScale",OFFSET(Appearance, fShoulderScale),        },
        "The basic scales of the various overall body parts"},
    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,                "ChestScale",    OFFSET(Appearance, fChestScale),        },
        "The basic scales of the various overall body parts"},
    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,                "WaistScale",    OFFSET(Appearance, fWaistScale),        },
        "The basic scales of the various overall body parts"},
    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,                "HipScale",        OFFSET(Appearance, fHipScale),            },
        "The basic scales of the various overall body parts"},
    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,                "LegScale",        OFFSET(Appearance, fLegScale),            },
        "The basic scales of the various overall body parts"},

    {{ PACKTYPE_INT,     SIZE_INT32,         "ConvertedScale", OFFSET(Appearance, convertedScale),    },
        "Chacters made before body scaling had to be converted to new system.  If this is set, that conversion<br>"
        "has happened."},

    {{ PACKTYPE_INT,     SIZE_INT32,         "HeadScales",        OFFSET(Appearance, compressedScales[0]),    },
        "The scales for the head shape. These are vec3's compressed into integer format."},
    {{ PACKTYPE_INT,     SIZE_INT32,         "BrowScales",        OFFSET(Appearance, compressedScales[1]),    },
        "The scales for the head shape. These are vec3's compressed into integer format."},
    {{ PACKTYPE_INT,     SIZE_INT32,         "CheekScales",        OFFSET(Appearance, compressedScales[2]),    },
        "The scales for the head shape. These are vec3's compressed into integer format."},
    {{ PACKTYPE_INT,     SIZE_INT32,         "ChinScales",        OFFSET(Appearance, compressedScales[3]),    },
        "The scales for the head shape. These are vec3's compressed into integer format."},
    {{ PACKTYPE_INT,     SIZE_INT32,         "CraniumScales",    OFFSET(Appearance, compressedScales[4]),    },
        "The scales for the head shape. These are vec3's compressed into integer format."},
    {{ PACKTYPE_INT,     SIZE_INT32,         "JawScales",        OFFSET(Appearance, compressedScales[5]),    },
        "The scales for the head shape. These are vec3's compressed into integer format."},
    {{ PACKTYPE_INT,     SIZE_INT32,         "NoseScales",        OFFSET(Appearance, compressedScales[6]),    },
        "The scales for the head shape. These are vec3's compressed into integer format."},

    // Don't alter the names and quantities of these next six without first checking what's going on for their original definitions in
    // the main ent_line_desc
    // Also make sure the total number of slots (primary and all aux's) matches the SG slot count defined as NUM_SG_COLOR_SLOTS in costume.h
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimary",            OFFSET(Appearance, superColorsPrimaryU[0].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, determines if primary color is original or one of the supergroup colors"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimaryHigh",            OFFSET(Appearance, superColorsPrimaryU[0].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, determines if primary color is original or one of the supergroup colors, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondary",        OFFSET(Appearance, superColorsSecondaryU[0].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, determines if primary color is original or one of the supergroup colors"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondaryHigh",        OFFSET(Appearance, superColorsSecondaryU[0].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, determines if secondary color is original or one of the supergroup colors, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimary2",        OFFSET(Appearance, superColorsPrimary2U[0].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, SuperPrimary was not large enough for all parts"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimary2High",        OFFSET(Appearance, superColorsPrimary2U[0].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, SuperPrimary was not large enough for all parts, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondary2",        OFFSET(Appearance, superColorsSecondary2U[0].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, SuperSecondary was not large enough for all parts"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondary2High",        OFFSET(Appearance, superColorsSecondary2U[0].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, SuperSecondary was not large enough for all parts, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperTertiary",        OFFSET(Appearance, superColorsTertiaryU[0].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperTertiaryHigh",        OFFSET(Appearance, superColorsTertiaryU[0].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperQuaternary",        OFFSET(Appearance, superColorsQuaternaryU[0].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperQuaternaryHigh",        OFFSET(Appearance, superColorsQuaternaryU[0].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode, high bits"},

    // First auxiliary slot.
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimaryAux1",        OFFSET(Appearance, superColorsPrimaryU[1].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, determines if primary color is original or one of the supergroup colors"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimaryAux1High",        OFFSET(Appearance, superColorsPrimaryU[1].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, determines if primary color is original or one of the supergroup colors, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondaryAux1",    OFFSET(Appearance, superColorsSecondaryU[1].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, determines if secondary color is original or one of the supergroup colors"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondaryAux1High",    OFFSET(Appearance, superColorsSecondaryU[1].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, determines if secondary color is original or one of the supergroup colors, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimary2Aux1",    OFFSET(Appearance, superColorsPrimary2U[1].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, SuperPrimary was not large enough for all parts"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimary2Aux1High",    OFFSET(Appearance, superColorsPrimary2U[1].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, SuperPrimary was not large enough for all parts, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondary2Aux1",    OFFSET(Appearance, superColorsSecondary2U[1].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, SuperSecondary was not large enough for all parts"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondary2Aux1High",    OFFSET(Appearance, superColorsSecondary2U[1].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, SuperSecondary was not large enough for all parts, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperTertiaryAux1",    OFFSET(Appearance, superColorsTertiaryU[1].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperTertiaryAux1High",    OFFSET(Appearance, superColorsTertiaryU[1].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperQuaternaryAux1",    OFFSET(Appearance, superColorsQuaternaryU[1].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperQuaternaryAux1High",    OFFSET(Appearance, superColorsQuaternaryU[1].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode, high bits"},

    // power colors.
    {{ PACKTYPE_INT, SIZE_INT32,                            "PowerColorPrimary1",    OFFSET_IGNORE(),                                    },
        "Unused - Power customization prototyping"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "PowerColorPrimary2",    OFFSET_IGNORE(),                                    },
        "Unused - Power customization prototyping"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "PowerColorSecondary1",    OFFSET_IGNORE(),                                    },
        "Unused - Power customization prototyping"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "PowerColorSecondary2",    OFFSET_IGNORE(),                                    },
        "Unused - Power customization prototyping"},

    // Second auxiliary slot.
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimaryAux2",        OFFSET(Appearance, superColorsPrimaryU[2].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, determines if primary color is original or one of the supergroup colors"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimaryAux2High",        OFFSET(Appearance, superColorsPrimaryU[2].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, determines if primary color is original or one of the supergroup colors, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondaryAux2",    OFFSET(Appearance, superColorsSecondaryU[2].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, determines if secondary color is original or one of the supergroup colors"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondaryAux2High",    OFFSET(Appearance, superColorsSecondaryU[2].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, determines if secondary color is original or one of the supergroup colors, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimary2Aux2",    OFFSET(Appearance, superColorsPrimary2U[2].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, SuperPrimary was not large enough for all parts"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimary2Aux2High",    OFFSET(Appearance, superColorsPrimary2U[2].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, SuperPrimary was not large enough for all parts, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondary2Aux2",    OFFSET(Appearance, superColorsSecondary2U[2].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, SuperSecondary was not large enough for all parts"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondary2Aux2High",    OFFSET(Appearance, superColorsSecondary2U[2].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, SuperSecondary was not large enough for all parts, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperTertiaryAux2",    OFFSET(Appearance, superColorsTertiaryU[2].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperTertiaryAux2High",    OFFSET(Appearance, superColorsTertiaryU[2].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperQuaternaryAux2",    OFFSET(Appearance, superColorsQuaternaryU[2].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperQuaternaryAux2High",    OFFSET(Appearance, superColorsQuaternaryU[2].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode, high bits"},
    // Third auxiliary slot.
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimaryAux3",        OFFSET(Appearance, superColorsPrimaryU[3].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, determines if primary color is original or one of the supergroup colors"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimaryAux3High",        OFFSET(Appearance, superColorsPrimaryU[3].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, determines if primary color is original or one of the supergroup colors, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondaryAux3",    OFFSET(Appearance, superColorsSecondaryU[3].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, determines if secondary color is original or one of the supergroup colors"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondaryAux3High",    OFFSET(Appearance, superColorsSecondaryU[3].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, determines if secondary color is original or one of the supergroup colors, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimary2Aux3",    OFFSET(Appearance, superColorsPrimary2U[3].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, SuperPrimary was not large enough for all parts"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimary2Aux3High",    OFFSET(Appearance, superColorsPrimary2U[3].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, SuperPrimary was not large enough for all parts, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondary2Aux3",    OFFSET(Appearance, superColorsSecondary2U[3].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, SuperSecondary was not large enough for all parts"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondary2Aux3High",    OFFSET(Appearance, superColorsSecondary2U[3].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, SuperSecondary was not large enough for all parts, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperTertiaryAux3",    OFFSET(Appearance, superColorsTertiaryU[3].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperTertiaryAux3High",    OFFSET(Appearance, superColorsTertiaryU[3].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperQuaternaryAux3",    OFFSET(Appearance, superColorsQuaternaryU[3].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperQuaternaryAux3High",    OFFSET(Appearance, superColorsQuaternaryU[3].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode, high bits"},
    // Fourth auxiliary slot.
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimaryAux4",        OFFSET(Appearance, superColorsPrimaryU[4].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, determines if primary color is original or one of the supergroup colors"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimaryAux4High",        OFFSET(Appearance, superColorsPrimaryU[4].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, determines if primary color is original or one of the supergroup colors, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondaryAux4",    OFFSET(Appearance, superColorsSecondaryU[4].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, determines if secondary color is original or one of the supergroup colors"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondaryAux4High",    OFFSET(Appearance, superColorsSecondaryU[4].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, determines if secondary color is original or one of the supergroup colors, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimary2Aux4",    OFFSET(Appearance, superColorsPrimary2U[4].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, SuperPrimary was not large enough for all parts"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimary2Aux4High",    OFFSET(Appearance, superColorsPrimary2U[4].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, SuperPrimary was not large enough for all parts, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondary2Aux4",    OFFSET(Appearance, superColorsSecondary2U[4].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, SuperSecondary was not large enough for all parts"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondary2Aux4High",    OFFSET(Appearance, superColorsSecondary2U[4].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, SuperSecondary was not large enough for all parts, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperTertiaryAux4",    OFFSET(Appearance, superColorsTertiaryU[4].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperTertiaryAux4High",    OFFSET(Appearance, superColorsTertiaryU[4].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperQuaternaryAux4",    OFFSET(Appearance, superColorsQuaternaryU[4].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperQuaternaryAux4High",    OFFSET(Appearance, superColorsQuaternaryU[4].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode, high bits"},
    // Fifth auxiliary slot.
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimaryAux5",        OFFSET(Appearance, superColorsPrimaryU[5].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, determines if primary color is original or one of the supergroup colors"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimaryAux5High",        OFFSET(Appearance, superColorsPrimaryU[5].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, determines if primary color is original or one of the supergroup colors, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondaryAux5",    OFFSET(Appearance, superColorsSecondaryU[5].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, determines if secondary color is original or one of the supergroup colors"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondaryAux5High",    OFFSET(Appearance, superColorsSecondaryU[5].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, determines if secondary color is original or one of the supergroup colors, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimary2Aux5",    OFFSET(Appearance, superColorsPrimary2U[5].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, SuperPrimary was not large enough for all parts"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimary2Aux5High",    OFFSET(Appearance, superColorsPrimary2U[5].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, SuperPrimary was not large enough for all parts, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondary2Aux5",    OFFSET(Appearance, superColorsSecondary2U[5].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, SuperSecondary was not large enough for all parts"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondary2Aux5High",    OFFSET(Appearance, superColorsSecondary2U[5].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, SuperSecondary was not large enough for all parts, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperTertiaryAux5",    OFFSET(Appearance, superColorsTertiaryU[5].SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperTertiaryAux5High",    OFFSET(Appearance, superColorsTertiaryU[5].SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode, high bits"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperQuaternaryAux5",    OFFSET(Appearance, superColorsQuaternaryU[5].SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperQuaternaryAux5High",    OFFSET(Appearance, superColorsQuaternaryU[5].SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode, high bits"},

    // Custom powers token
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                    "PrimaryPowerToken",    OFFSET_IGNORE(),                    },
        "Unused - Power customization prototyping"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                    "SecondaryPowerToken",    OFFSET_IGNORE(),                    },
        "Unused - Power customization prototyping"},

    // Currently selected supergroup color set
    {{ PACKTYPE_INT, SIZE_INT32,                        "SuperColorSet",    OFFSET(Appearance, currentSuperColorSet),                },
        "Currently selected super group color set"},

    { 0 },
};


StructDesc appearance_desc[] =
{
    sizeof(Appearance),
    { AT_STRUCT_ARRAY, OFFSET(DBAppearance, appearance) },
    appearance_line_desc,

    "The Appearance table specifies the basic structure for each character's avatar.<br>"
    "Each character has a single row in the Appearance table."
};

LineDesc appearances_line_desc[] =
{
    { PACKTYPE_SUB, MAX_COSTUMES,  "Appearance", (intptr_t)appearance_desc },
    { 0 },
};

StructDesc appearances_desc[] =
{
    sizeof(DBAppearance),
    {AT_STRUCT_ARRAY,{0}},
    appearances_line_desc,
};
LineDesc powercustomization_line_desc[] =
{

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,       "PowerCatName",    OFFSET(DBPowerCustomization, powerCatName),       },
    "Power Category Name - Category of the power"},

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,       "PowerSetName",    OFFSET(DBPowerCustomization, powerSetName),       },
    "Power Set Name - Set of the power"},

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,       "PowerName",    OFFSET(DBPowerCustomization, powerName),       },
    "Power Name - Name of the power"},

    {{ PACKTYPE_INT,     SIZE_INT32,         "Color1",        OFFSET(DBPowerCustomization, customTint.primary),            },
    "Color - the primary color to apply to this power"},

    {{ PACKTYPE_INT,     SIZE_INT32,         "Color2",        OFFSET(DBPowerCustomization, customTint.secondary),            },
    "Color - the primary color to apply to this power"},

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,       "Token",        OFFSET(DBPowerCustomization, token),       },
    "Token"},

    {{ PACKTYPE_INT,    SIZE_INT32,            "SlotId",        OFFSET(DBPowerCustomization, slotId),       },
    "SlotId of this customization"},

    { 0 },
};


StructDesc powercustomization_desc[] =
{
    sizeof(DBPowerCustomization),
    { AT_STRUCT_ARRAY, OFFSET(DBPowerCustomizationList, powerCustomization) },
    powercustomization_line_desc,

    "The powerCustomization table specifies the custom powers for all the characters.<br>"
    "Each costume a character owns is represented with multiple rows in the table."
};
LineDesc powercustomization_list_line_desc[] =
{
    { PACKTYPE_SUB, MAX_COSTUMES*MAX_POWERS,  "PowerCustomizations", (intptr_t)powercustomization_desc },
    { 0 },

};
StructDesc powercustomization_list_desc[] =
{
    sizeof(DBPowerCustomizationList),
    {AT_STRUCT_ARRAY, {0}},
    powercustomization_list_line_desc,
};

//---------------------------------------------------------------------------------
// FameString list description
//---------------------------------------------------------------------------------
LineDesc    famestring_list_line_desc[] =
{
    {{ PACKTYPE_STR_UTF8, MAX_FAME_STRING,     "String",     0                              },
        "The string the npc should say"},

    { 0 },
};

StructDesc famestring_list_desc[] =
{
    sizeof(char) * MAX_FAME_STRING,
    {AT_STRUCT_ARRAY, OFFSET2_PTR(Entity, pl,    EntPlayer, fameStrings) },
    famestring_list_line_desc,

    "Fame strings are used by random NPC's passing by to comment on somehting the player has recently done.<br>"
    "There can be many strings per player."
};

//---------------------------------------------------------------------------------
// Contact description
//---------------------------------------------------------------------------------
LineDesc contact_line_desc[] =
{
    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,            "ID",                    OFFSET(StoryContactInfo, handle),        INOUT(ContactGetHandle, ContactFileName)},
        "Persistant Handle that refers to the contact definition"},

    {{ PACKTYPE_BIN_STR,    TASK_BITFIELD_SIZE*4,    "TaskIssued",            OFFSET(StoryContactInfo, taskIssued)},
        "Bitfield - Designates which of the contact's tasks have been given to the character"},

    {{ PACKTYPE_BIN_STR,    STORYARC_BITFIELD_SIZE*4,"StoryArcIssued",        OFFSET(StoryContactInfo, storyArcIssued)},
        "Bitfield - Designates which of the contact's story arcs have been given to the character"},

    {{ PACKTYPE_INT,        SIZE_INT32,                "DialogSeed",            OFFSET(StoryContactInfo, dialogSeed)},
        "Determines how the contact will talk to a player and what randomly chosen tasks the player gets"},

    {{ PACKTYPE_INT,        SIZE_INT32,                "ContactIntroSeed",        OFFSET(StoryContactInfo, contactIntroSeed)},
        "Seed that determines how and which random contacts will be introduced to the player"},

    {{ PACKTYPE_INT,        SIZE_INT32,                "ContactPoints",        OFFSET(StoryContactInfo, contactPoints)},
        "How many contact points the character has earned for this contact"},

    {{ PACKTYPE_INT,        SIZE_INT32,                "ContactRelationship",    OFFSET(StoryContactInfo, contactRelationship)},
        "The character's current relationship with this contact.<br>"
        "   0 = NO_RELATIONSHIP<br>"
        "   1 = ACQUAINTANCE<br>"
        "   2 = FRIEND<br>"
        "   3 = CONFIDANT"},

    {{ PACKTYPE_INT,        SIZE_INT32,                "ContactsIntroduced",    OFFSET(StoryContactInfo, contactsIntroduced)},
        "Number of contacts that this contact has introduced to you"},

    {{ PACKTYPE_INT,        SIZE_INT32,                "SeenPlayer",            OFFSET(StoryContactInfo, seenPlayer)},
        "Flags the fact that the contact has said his first time string to the player"},

    {{ PACKTYPE_INT,        SIZE_INT32,                "NotifyPlayer",            OFFSET(StoryContactInfo, notifyPlayer)},
        "Flags the contact as wanting to speak to the player"},

    {{ PACKTYPE_INT,        SIZE_INT32,                "ItemsBought",            OFFSET(StoryContactInfo, itemsBought)},
        "Bitfield - Which unique items a player has bought from this contact"},

    {{ PACKTYPE_INT,        SIZE_INT8,                "RewardContact",        OFFSET(StoryContactInfo, rewardContact)},
        "Flags whether or not a contact should introduce you to a new contact as part of a story reward"},

    {{ PACKTYPE_INT,        SIZE_INT32,                "BrokerHandle",            OFFSET(StoryContactInfo, brokerHandle)},
        "Special field used by newspaper contacts that determines which Broker they are currently tied to<br>"
        "Doing newspaper missions will then give you credit towards that Broker"},

    { 0 },
};

StructDesc contact_desc[] =
{
    sizeof(StoryContactInfo),
    {AT_POINTER_ARRAY,{INDIRECTION(Entity, storyInfo, 1), INDIRECTION(StoryInfo, contactInfos, 1)}},
    contact_line_desc,

    "This table describes a players contacts. An NPC they interact with for mission and story.<br>"
    "Each row describes a single contact.<br>"
    "Each character will have multiple contacts."
};

StructDesc tf_contact_desc[] =
{
    sizeof(StoryContactInfo),
    {AT_POINTER_ARRAY,{INDIRECTION(TaskForce, storyInfo, 1), INDIRECTION(StoryInfo, contactInfos, 1)}},
    contact_line_desc,

    "This table describes a task force contact.<br>"
    "Each row describes a single contact.<br>"
    "Each task force will have one contact."
};

//---------------------------------------------------------------------------------
// Story Clue list description
// CURRENTLY UNUSED AND DEPRECATED, BUT THERE ARE ALREADY DATABASE TABLES
//---------------------------------------------------------------------------------
LineDesc    storyclue_line_desc[] =
{
    {{ PACKTYPE_INT,    SIZE_INT32,        "sahandle",        OFFSET(StoryClueInfo, sahandle.context)},
        "The ID of the story arc definition."},

    {{ PACKTYPE_INT,    SIZE_INT32,        "clueIndex",    OFFSET(StoryClueInfo, clueIndex)},
        "The index into the StoryArc's list of clues that this StoryClueInfo describes."},

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,    "varName",        OFFSET(StoryClueInfo, varName)},
        "The name of the dynamically-determined var."},

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,    "varValue",        OFFSET(StoryClueInfo, varValue)},
        "The value of the dynamically-determined var."},

    { 0 },
};

// StructDesc storyclue_desc[] =
// {
//     sizeof(StoryClueInfo),
//     {AT_EARRAY, OFFSET2_PTR(Entity, storyInfo, StoryInfo, storyArcClueInfo)},
//     storyclue_line_desc,
// 
//     "Story Clue Info contains dynamically determined var data for the clue text."
// };



//---------------------------------------------------------------------------------
// Story Arc description
//---------------------------------------------------------------------------------
LineDesc storyarc_line_desc[] =
{
    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,                "ID",        OFFSET2(StoryArcInfo, sahandle, StoryTaskHandle, context),        INOUT(StoryArcContextFromFileName, StoryArcFileName) },
        "Persistant Handle that refers to the story arc definition"},

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,                "Contact",    OFFSET(StoryArcInfo, contact),        INOUT(ContactGetHandle, ContactFileName)},
        "The contact that gave the player this story arc"},

    {{ PACKTYPE_INT,        SIZE_INT32,                "Episode",    OFFSET(StoryArcInfo, episode)},
        "Keeps track of which episode within the story arc the player is currently on"},

    {{ PACKTYPE_INT,        SIZE_INT32,                "Seed",        OFFSET(StoryArcInfo, seed)},
        "The random seed used to generate the story arc."},

    {{ PACKTYPE_INT,        SIZE_INT32,                "ClueNeedsIntro", OFFSET(StoryArcInfo, clueNeedsIntro)},
        "Marks which clue, if any, needs to be shown to the player next time they see the contact"},

    {{ PACKTYPE_BIN_STR,    CLUE_BITFIELD_SIZE*4,    "Clues",    OFFSET(StoryArcInfo, haveClues)},
        "Bitfield - Keeps track of the clues that the player has seen"},

    {{ PACKTYPE_BIN_STR,    EP_TASK_BITFIELD_SIZE*4,"TaskComplete",    OFFSET(StoryArcInfo, taskComplete)},
        "Bitfield - Keeps track of all tasks that were completed successfully within the current episode"},

    {{ PACKTYPE_BIN_STR,    EP_TASK_BITFIELD_SIZE*4,"TaskIssued",    OFFSET(StoryArcInfo, taskIssued)},
        "Bitfield - Keeps track of all tasks that issued within the current episode"},

    {{ PACKTYPE_INT,        SIZE_INT32,                "PlayerCreatedID", OFFSET(StoryArcInfo, playerCreatedID), },
        "ID of a playercreated mission"},

    { 0 },
};

StructDesc storyarc_desc[] =
{
    sizeof(StoryArcInfo),
    {AT_POINTER_ARRAY,{INDIRECTION(Entity, storyInfo, 1), INDIRECTION(StoryInfo, storyArcs, 1)}},
    storyarc_line_desc,

    "This table describes a players storyarc.  A storyline given to them by a contact.<br>"
    "Each row describes a single storyarc.<br>"
    "Each character will have multiple storyarcs."
};

StructDesc tf_storyarc_desc[] =
{
    sizeof(StoryArcInfo),
    {AT_POINTER_ARRAY,{INDIRECTION(TaskForce, storyInfo, 1), INDIRECTION(StoryInfo, storyArcs, 1)}},
    storyarc_line_desc,

    "This table describes a Task Force storyarc.  A storyline given to them by a contact.<br>"
    "Each row describes a single storyarc.<br>"
    "Each task force will have one storyarc."
};

// ***********************************************************************************
// IMPORTANT: Make sure you also update this in containersupergroup.c sg_task_line_desc
// Yes this is a terrible hack, but its the best option for now
// ***********************************************************************************

//---------------------------------------------------------------------------------
// Task description
//---------------------------------------------------------------------------------
LineDesc task_line_desc[] =
{
    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,        "ID",            OFFSET2(StoryTaskInfo, sahandle, StoryTaskHandle, context),        INOUT(TaskGetHandle, TaskFileName)},
        "Handle of the contact or storyarc from which this task came"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "SubHandle",    OFFSET2(StoryTaskInfo, sahandle, StoryTaskHandle, subhandle)},
        "Identifies which task this is within the contact or storyarc"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "CompoundPos",    OFFSET2(StoryTaskInfo, sahandle, StoryTaskHandle, compoundPos) },
        "The step of a compound that the player is currently on"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "Seed",            OFFSET(StoryTaskInfo, seed)},
        "The random seed used to generate the task."},

    {{ PACKTYPE_INT,        SIZE_INT32,            "State",        OFFSET(StoryTaskInfo, state)},    // FIXME!!! Translate to readble string or use attribs?
        "The current state of the task:<br>"
        "   0 = TASK_NONE<br>"
        "   1 = TASK_ASSIGNED<br>"
        "   2 = TASK_MARKED_SUCCESS<br>"
        "   3 = TASK_MARKED_FAILURE<br>"
        "   4 = TASK_SUCCEEDED<br>"
        "   5 = TASK_FAILED"},

    {{ PACKTYPE_BIN_STR,    CLUE_BITFIELD_SIZE*4,    "Clues",    OFFSET(StoryTaskInfo, haveClues)},
        "Bitfield - Keeps track of the clues that the player has seen"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "ClueNeedsIntro",OFFSET(StoryTaskInfo, clueNeedsIntro)},
        "Marks which clue, if any, needs to be shown to the player next time they see the contact"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "SpawnGiven",    OFFSET(StoryTaskInfo, spawnGiven)},
        "Flags whether or not the encounter associated with the task has been spawned"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "Level",        OFFSET(StoryTaskInfo, level)},
        "The 1-based level of the task."},

    {{ PACKTYPE_INT,        SIZE_INT32,            "Timeout",        OFFSET(StoryTaskInfo, timeout)},
        "The time that the task timer will expire."},

    {{ PACKTYPE_CONREF,        CONTAINER_ENTS,        "AssignedDbId",        OFFSET(StoryTaskInfo, assignedDbId) },
        "DB ID - ContainerID of the player the task was assigned to"},

    {{ PACKTYPE_INT,        SIZE_INT32,        "AssignedTime",        OFFSET(StoryTaskInfo, assignedTime) },
        "Keeps track of when the task was assigned"},

    {{ PACKTYPE_INT,        SIZE_INT32,        "MissionMapId",        OFFSET(StoryTaskInfo, missionMapId) },
        "Map ID - The map ID for the mission map"},

    {{ PACKTYPE_INT,        SIZE_INT32,        "MissionDoorMapId",    OFFSET(StoryTaskInfo, doorMapId)    },
        "Map ID - The map ID for the map which contains the door to the mission."},

    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,            "MissionDoorPosX",    OFFSET(StoryTaskInfo, doorPos[0])   },
        "The location on the MissionDoorMap of the door to the mission."},
    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,            "MissionDoorPosY",    OFFSET(StoryTaskInfo, doorPos[1])   },
        "The location on the MissionDoorMap of the door to the mission."},
    {{ PACKTYPE_FLOAT,        SIZE_FLOAT32,            "MissionDoorPosZ",    OFFSET(StoryTaskInfo, doorPos[2])   },
        "The location on the MissionDoorMap of the door to the mission."},

    {{ PACKTYPE_BIN_STR, MAX_OBJECTIVE_INFOS*4, "CompleteObjectives", OFFSET(StoryTaskInfo, completeObjectives)},
        "Keeps track of which mission objectives have been completed"},

    {{ PACKTYPE_STR_ASCII,        SIZEOF2(StoryTaskInfo, villainType),    "VillainType",    OFFSET(StoryTaskInfo, villainType)},
        "Deprecated: no longer used"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "VillainCount",    OFFSET(StoryTaskInfo, curKillCount)},
        "Current number of villains killed for kill tasks"},

    {{ PACKTYPE_STR_ASCII,        SIZEOF2(StoryTaskInfo, villainType2),    "VillainType2",    OFFSET(StoryTaskInfo, villainType2)},
        "Deprecated: no longer used"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "VillainCount2",OFFSET(StoryTaskInfo, curKillCount2)},
        "Current number of villains killed for kill tasks, a second count for tracking a second villain group"},

    {{ PACKTYPE_STR_ASCII,        SIZEOF2(StoryTaskInfo, deliveryTargetName),    "DeliveryTargetName",OFFSET(StoryTaskInfo, deliveryTargetName)},
        "Deprecated: no longer used"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "NextVisitLocation", OFFSET(StoryTaskInfo, nextLocation) },
        "Index of the next location to visit(only for visit location tasks)"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "SubtaskSuccess",OFFSET(StoryTaskInfo, subtaskSuccess) },
        "Bitfield - Marks whether each subtask within a compound task has been completed"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "Notoriety",    OFFSET(StoryTaskInfo, old_notoriety)},
        "deprecated" },

    {{ PACKTYPE_INT,        SIZE_INT32,            "SkillLevel",    OFFSET(StoryTaskInfo, skillLevel)},
        "The skill level that this task was spawned at"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "VillainGroup",    OFFSET(StoryTaskInfo, villainGroup)},
        "The randomly generated villaingroup that should be spawned within the mission"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "MapSet",        OFFSET(StoryTaskInfo, mapSet)},
        "The randomly generated mapset that is to be used for this task"},

    {{ PACKTYPE_INT,        SIZE_INT8,            "TeamCompleted",OFFSET(StoryTaskInfo, teamCompleted)},
        "An identical task has been completed with a group meaning this task can be prompted for completion<br>"
        "The value stored is the notoriety level that the task was teamcompleted on"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "SideObjectives",        OFFSET(StoryTaskInfo, completeSideObjectives)},
        "Tracks side objectives that have been completed that we dont want to allow to respawn on mission reset"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "PlayerCreated",    OFFSET2(StoryTaskInfo, sahandle, StoryTaskHandle, bPlayerCreated)},
        "Identifies a player created task"},
    {{ PACKTYPE_INT,        SIZE_INT32,            "PlayerCreatedID", OFFSET(StoryTaskInfo, playerCreatedID), },
        "ID of a playercreated mission"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "LevelAdjust",    OFFSET2(StoryTaskInfo, difficulty, StoryDifficulty, levelAdjust )},
        "Level adustment of enemies  ( -1 to +4 )"},
    {{ PACKTYPE_INT,        SIZE_INT32,            "TeamSize",    OFFSET2(StoryTaskInfo, difficulty, StoryDifficulty, teamSize )},
        "Team size this player is treated as ( 1 to 8 )"},
    {{ PACKTYPE_INT,        SIZE_INT32,            "UpgradeAV",    OFFSET2(StoryTaskInfo, difficulty, StoryDifficulty, alwaysAV )},
        "it true, don't downgrade AV to EB, otherwise always do"},
    {{ PACKTYPE_INT,        SIZE_INT32,            "DowngradeBoss",    OFFSET2(StoryTaskInfo, difficulty, StoryDifficulty, dontReduceBoss )},
        "if true, no bosses while solo"},
    {{ PACKTYPE_STR_ASCII,        SIZEOF2(StoryTaskInfo, deprecatedStr),    "MysteryInvestigation_VarType",    OFFSET(StoryTaskInfo, deprecatedStr), },
        "DEPRECATED."},
    {{ PACKTYPE_STR_ASCII,        SIZEOF2(StoryTaskInfo, deprecatedStr),    "MysteryInvestigation_VarValue",    OFFSET(StoryTaskInfo, deprecatedStr), },
        "DEPRECATED."},

    {{ PACKTYPE_INT,        SIZE_INT32,            "TimerType",    OFFSET(StoryTaskInfo, timerType)},
        "The type of timer used on the task.  1 is count up, -1 is count down."},

    {{ PACKTYPE_INT,        SIZE_INT32,            "FailOnTimeout",    OFFSET(StoryTaskInfo, failOnTimeout)},
        "Whether the task will fail when the timer expires."},

    {{ PACKTYPE_INT,        SIZE_INT32,            "TimeZero",        OFFSET(StoryTaskInfo, timezero)},
        "The time when the timer equals zero.  Differs from timeout on limited countups."},

    { 0 },
};

StructDesc task_desc[] =
{
    sizeof(StoryTaskInfo),
    {AT_POINTER_ARRAY,{INDIRECTION(Entity, storyInfo, 1), INDIRECTION(StoryInfo, tasks, 1)}},
    task_line_desc,

    "This table describes a players task.  A task is given to player by a contact.<br>"
    "Each row describes a single task.<br>"
    "Each character will have multiple tasks."
};

StructDesc tf_task_desc[] =
{
    sizeof(StoryTaskInfo),
    {AT_POINTER_ARRAY,{INDIRECTION(TaskForce, storyInfo, 1), INDIRECTION(StoryInfo, tasks, 1)}},
    task_line_desc,

    "This table describes a task force task.  A task is given to player by a contact.<br>"
    "Each row describes a single task.<br>"
    "Each task force will have multiple tasks."
};

StructDesc team_task_desc[] =
{
    sizeof(StoryTaskInfo),
    {AT_POINTER_ARRAY,{INDIRECTION(Teamup, activetask, 0)}},
    task_line_desc,

    "TODO"
};

//---------------------------------------------------------------------------------
// Newspaper history description
//---------------------------------------------------------------------------------
LineDesc    newspaper_history_line_desc[] =
{
    {{ PACKTYPE_INT, SIZE_INT32,                            "NTHIndexItem",        OFFSET(StoryInfo, newspaperTaskHistoryIndex[0]), },
        "Contains the index of the oldest ITEM_NAME that has been used part of a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem0",            OFFSET(StoryInfo, newspaperTaskHistory[0][0]),   },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem1",            OFFSET(StoryInfo, newspaperTaskHistory[0][1]),   },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem2",            OFFSET(StoryInfo, newspaperTaskHistory[0][2]),   },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem3",            OFFSET(StoryInfo, newspaperTaskHistory[0][3]),   },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem4",            OFFSET(StoryInfo, newspaperTaskHistory[0][4]),   },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem5",            OFFSET(StoryInfo, newspaperTaskHistory[0][5]),   },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem6",            OFFSET(StoryInfo, newspaperTaskHistory[0][6]),   },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem7",            OFFSET(StoryInfo, newspaperTaskHistory[0][7]),   },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem8",            OFFSET(StoryInfo, newspaperTaskHistory[0][8]),   },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem9",            OFFSET(StoryInfo, newspaperTaskHistory[0][9]),   },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem10",        OFFSET(StoryInfo, newspaperTaskHistory[0][10]),  },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem11",        OFFSET(StoryInfo, newspaperTaskHistory[0][11]),  },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem12",        OFFSET(StoryInfo, newspaperTaskHistory[0][12]),  },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem13",        OFFSET(StoryInfo, newspaperTaskHistory[0][13]),  },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem14",        OFFSET(StoryInfo, newspaperTaskHistory[0][14]),  },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem15",        OFFSET(StoryInfo, newspaperTaskHistory[0][15]),  },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem16",        OFFSET(StoryInfo, newspaperTaskHistory[0][16]),  },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem17",        OFFSET(StoryInfo, newspaperTaskHistory[0][17]),  },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem18",        OFFSET(StoryInfo, newspaperTaskHistory[0][18]),  },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHItem19",        OFFSET(StoryInfo, newspaperTaskHistory[0][19]),  },
        "Keeps track of one of the last 20 ITEM_NAMES that have been used in a players newspaper task"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "NTHIndexPerson",    OFFSET(StoryInfo, newspaperTaskHistoryIndex[1]), },
        "Contains the index of the oldest PERSON_NAME that has been used part of a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson0",        OFFSET(StoryInfo, newspaperTaskHistory[1][0]),   },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson1",        OFFSET(StoryInfo, newspaperTaskHistory[1][1]),   },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson2",        OFFSET(StoryInfo, newspaperTaskHistory[1][2]),   },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson3",        OFFSET(StoryInfo, newspaperTaskHistory[1][3]),   },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson4",        OFFSET(StoryInfo, newspaperTaskHistory[1][4]),   },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson5",        OFFSET(StoryInfo, newspaperTaskHistory[1][5]),   },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson6",        OFFSET(StoryInfo, newspaperTaskHistory[1][6]),   },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson7",        OFFSET(StoryInfo, newspaperTaskHistory[1][7]),   },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson8",        OFFSET(StoryInfo, newspaperTaskHistory[1][8]),   },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson9",        OFFSET(StoryInfo, newspaperTaskHistory[1][9]),   },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson10",        OFFSET(StoryInfo, newspaperTaskHistory[1][10]),  },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson11",        OFFSET(StoryInfo, newspaperTaskHistory[1][11]),  },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson12",        OFFSET(StoryInfo, newspaperTaskHistory[1][12]),  },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson13",        OFFSET(StoryInfo, newspaperTaskHistory[1][13]),  },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson14",        OFFSET(StoryInfo, newspaperTaskHistory[1][14]),  },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson15",        OFFSET(StoryInfo, newspaperTaskHistory[1][15]),  },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson16",        OFFSET(StoryInfo, newspaperTaskHistory[1][16]),  },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson17",        OFFSET(StoryInfo, newspaperTaskHistory[1][17]),  },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson18",        OFFSET(StoryInfo, newspaperTaskHistory[1][18]),  },
        "Keeps track of one of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,                        "NTHPerson19",        OFFSET(StoryInfo, newspaperTaskHistory[1][19]),  },
        "Keeps track of the last 20 PERSON_NAMES that have been used in a players newspaper task"},
    {0}
};

StructDesc    newspaper_history_desc[] =
{
    sizeof(StoryInfo), {AT_POINTER_ARRAY, OFFSET(Entity, storyInfo)}, newspaper_history_line_desc,

    "This table describes the history of items and people that were a part of a players completed newspaper task.<br>"
    "Each row describes a single newspaper history.<br>"
    "Each character will have one newspaper history."
};

StructDesc tf_newspaper_history_desc[] =
{
    sizeof(StoryInfo), {AT_POINTER_ARRAY, OFFSET(TaskForce, storyInfo)}, newspaper_history_line_desc,

    "TODO"
};


//---------------------------------------------------------------------------------
// Souvenir Clue description
//---------------------------------------------------------------------------------
LineDesc    souvenirClue_line_desc[] =
{
    {{PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,    "ID",    OFFSET(SouvenirClue, name)},
        "The clue key"},

    {0}
};

StructDesc    souvenirClue_desc[] =
{
    sizeof(SouvenirClue),
    {AT_POINTER_ARRAY, { INDIRECTION(Entity, storyInfo, 1), INDIRECTION(StoryInfo, souvenirClues, 1)}},
    souvenirClue_line_desc,

    "After players complete missions they sometimes get souvenir clues which display a brief re-cap of the story.<br>"
    "This table is a list of souvenir clues.<br>"
    "Each character can have multiple clues."
};

StructDesc tf_souvenirClue_desc[] =
{
    sizeof(SouvenirClue),
    {AT_POINTER_ARRAY,{INDIRECTION(TaskForce, storyInfo, 1), INDIRECTION(StoryInfo, souvenirClues, 1)}},
    souvenirClue_line_desc,

    "TODO"
};

LineDesc taskForceParameter_line_desc[] =
{
    // Note there's no offset, because we find the base address of the
    // array elements and they're just numbers so there's no offsetting
    // within a larger structure.
    {{PACKTYPE_INT, SIZE_INT32, "param", 0},
        "Parameter value"},
    {0}
};

StructDesc tf_parameters_desc[] =
{
    // This is an array of simple numbers, stored directly in the
    // TaskForce structure, so each is 4 bytes in size and they are in
    // place rather than found by following a pointer.
    sizeof(U32),
    {AT_STRUCT_ARRAY,{INDIRECTION(TaskForce, parameters, 0)}},
    taskForceParameter_line_desc,

    "Any taskforce can have parameters set on it to make it more difficult\n"
    "Each parameter has a numeric value to specify it\n"
    "Whether the parameter is active is set by the bitfield"
};

//---------------------------------------------------------------------------------
// Ignore list description
//---------------------------------------------------------------------------------

LineDesc ignore_list_line_desc[] =
{
    {{PACKTYPE_INT, SIZE_INT32, "authid", 0},
        "The auth id of an ignored player"},
    { 0 },
};

StructDesc ignore_list_desc[] =
{
    sizeof(int),
    {AT_STRUCT_ARRAY, OFFSET(Entity,ignoredAuthId)},
    ignore_list_line_desc,

    "The Ignore table is a list of player ids the character is ignoring.<br>"
    "Each row is a single person ignored.<br>"
    "Every character can ignore multiple players."
};



LineDesc    visited_map_desc[] =
{
    {{ PACKTYPE_INT,        SIZE_INT32,                "MapId",    OFFSET(VisitedMap,map_id),},
        "Map id - The map ID of the static map"},

    {{ PACKTYPE_BIN_STR,    MAX_MAPVISIT_CELLS/8,    "Cells",     OFFSET(VisitedMap,cells), },
        "Bitmap - a bitmap of the static map indicating which locations have been visited."},

    { 0 },
};

StructDesc visited_maps_desc[] =
{
    sizeof(VisitedMap),
    {AT_EARRAY, OFFSET2_PTR(Entity, pl, EntPlayer, visited_maps)},
    visited_map_desc,

    "This table stores where the character has been on every static map.<br>"
    "For every static map a character has visited, there is row in the VisitedMaps table for that character."
};

LineDesc    gmail_claim_state_line_desc[] =
{
    {{ PACKTYPE_INT,        SIZE_INT32,                "MailId",    OFFSET(GMailClaimState,mail_id),},
        "Mail id - The mail ID of the gmail"},
    {{ PACKTYPE_INT, SIZE_INT8,                        "GmailClaimState",            OFFSET(GMailClaimState, claim_state),    },
        "Saves the gmail claim state. (none->trying to claim->claimed and waiting for chat ack->none)" },

    { 0 },
};

StructDesc gmail_claim_state_desc[] =
{
    sizeof(GMailClaimState),
    {AT_STRUCT_ARRAY,{ INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, gmailClaimState, 0)}},
    gmail_claim_state_line_desc,

    "This table stores the state of characters gmail claims. <br>"
    "There can be 1 for every gmail in flight. The three states are none, trying to claim, claimed (waiting for ack)."
};

LineDesc    marty_reward_track_line_desc[] =
{

    {{ PACKTYPE_INT, SIZE_INT32,                           "MARTYPerMinuteAccum", OFFSET(MARTY_RewardData, expMinuteAccum ),    },
    "Current Exp accumulated this minute"},
    {{PACKTYPE_BIN_STR, SIZEOF2(MARTY_RewardData,expCircleBuffer),    "MARTYRingBuffer",    OFFSET(MARTY_RewardData, expCircleBuffer),        },
    "All Ring Buffer Data"    },
    {{ PACKTYPE_INT, SIZE_INT32,                           "MARTYCurrentMinute", OFFSET(MARTY_RewardData, expCurrentMinute ),    },
    "Current Minute"},
    {{ PACKTYPE_INT, SIZE_INT32,                           "MARTYRingBufferLoc", OFFSET(MARTY_RewardData, expRingBufferLocation ),    },
    "Ring Buffer Location"},
    {{ PACKTYPE_INT, SIZE_INT32,                           "MARTY5MinSum", OFFSET(MARTY_RewardData, exp5MinSum ),    },
    "5 Minute Sum"},
    {{ PACKTYPE_INT, SIZE_INT32,                           "MARTY15MinSum", OFFSET(MARTY_RewardData, exp15MinSum ),    },
    "15 Minute Sum"},
    {{ PACKTYPE_INT, SIZE_INT32,                           "MARTY30MinSum", OFFSET(MARTY_RewardData, exp30MinSum ),    },
    "30 Minute Sum"},
    {{ PACKTYPE_INT, SIZE_INT32,                           "MARTY60MinSum", OFFSET(MARTY_RewardData, exp60MinSum ),    },
    "60 Minute Sum"},
    {{ PACKTYPE_INT, SIZE_INT32,                           "MARTY120MinSum", OFFSET(MARTY_RewardData, exp120MinSum ),    },
    "120 Minute Sum"},
    {{ PACKTYPE_INT, SIZE_INT32,                           "MARTYThrottled", OFFSET(MARTY_RewardData, expThrottled ),    },
    "Amount of time left throttling"},
    { 0 },
};

StructDesc marty_reward_track_desc[] =
{
    sizeof(MARTY_RewardData),
    {AT_STRUCT_ARRAY,{ INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, MARTY_rewardData, 0)}},
        marty_reward_track_line_desc,
        "This table stores the state of the MARTY throttle data. <br>"
        "There can be multiple tracks for each type of experience gain we are tracking."
};
LineDesc    gmail_pending_line_desc[] =
{
    {{ PACKTYPE_INT,        SIZE_INT32,                "MailId",            OFFSET2_PTR(Entity, pl,    EntPlayer, gmail_pending_mail_id),  },
    "Mail id - The mail ID of the gmail"},
    {{ PACKTYPE_INT,        SIZE_INT32,                "XactID",            OFFSET2_PTR(Entity, pl,    EntPlayer, gmail_pending_xact_id),  },
    "Transaction ID of the pending transaction"},
    {{ PACKTYPE_INT,        SIZE_INT32,                "GmailXactState",    OFFSET2_PTR(Entity, pl,    EntPlayer, gmail_pending_state),    },
    "Current state of the pending transaction" },
    {{ PACKTYPE_INT,        SIZE_INT32,                "Influence",        OFFSET2_PTR(Entity, pl,    EntPlayer, gmail_pending_influence),},
    "Influence being sent"},
    {{ PACKTYPE_INT,        SIZE_INT32,                "RequestTime",        OFFSET2_PTR(Entity, pl,    EntPlayer, gmail_pending_requestTime),},
    "Time stamp when this request was made"},
    {{ PACKTYPE_STR_UTF8,        SIZEOF2(EntPlayer, gmail_pending_subject),    "Subject",    OFFSET2_PTR(Entity, pl,    EntPlayer, gmail_pending_subject),},
    "Subject of message being sent"},
    {{ PACKTYPE_STR_UTF8,        SIZEOF2(EntPlayer, gmail_pending_attachment),    "Attachment",    OFFSET2_PTR(Entity, pl,    EntPlayer, gmail_pending_attachment),},
    "Attachment of message being sent (if any)"},
    {{ PACKTYPE_STR_UTF8,        SIZEOF2(EntPlayer, gmail_pending_to),    "Recipient",            OFFSET2_PTR(Entity, pl,    EntPlayer, gmail_pending_to),},
    "Recipient of message being sent"},
    {{ PACKTYPE_STR_UTF8,        SIZEOF2(EntPlayer, gmail_pending_inventory),    "Inventory",    OFFSET2_PTR(Entity, pl,    EntPlayer, gmail_pending_inventory),},
    "Pending inventory to be given to character (if any)"},
    {{ PACKTYPE_INT,        SIZE_INT32,                "BankedInfluence",        OFFSET2_PTR(Entity, pl,    EntPlayer, gmail_pending_banked_influence),},
    "Influence that was attempted to be rewarded, but couldn't be for some reason"},

    {{ PACKTYPE_LARGE_ESTRING_UTF8, 0,                        "Body",    OFFSET2_PTR(Entity, pl,    EntPlayer, gmail_pending_body) },
    "Body of message being sent"},

    { 0 },
};

StructDesc gmail_pending_desc[] =
{
    sizeof(Entity),
    {AT_NOT_ARRAY,{0}},
    gmail_pending_line_desc,

    "This table stores the state of pending gmail transactions. <br>"
    "There can only be one in process at any time (waiting for ack)."
};

LineDesc    queued_reward_line_desc[] =
{
    {{ PACKTYPE_STR_UTF8,    SIZEOF2(RewardDBStore, pendingRewardName),    "Name",    OFFSET(RewardDBStore, pendingRewardName),},
    "Subject of message being sent"},
    {{ PACKTYPE_INT,        SIZE_INT32,                "Vgroup",    OFFSET(RewardDBStore,pendingRewardVgroup),},
    "Villain group of the reward"},
    {{ PACKTYPE_INT,        SIZE_INT32,                "Level",    OFFSET(RewardDBStore, pendingRewardLevel),    },
    "Reward level" },

    { 0 },
};

StructDesc queued_reward_desc[] =
{
    sizeof(RewardDBStore),
    {AT_STRUCT_ARRAY,{ INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, queuedRewards, 0)}},
    queued_reward_line_desc,

    "This table stores the queued rewards tables."
};
//---------------------------------------------------------------------------------
// Entity description
//---------------------------------------------------------------------------------

LineDesc ent2_line_desc[] =
{
    {{ PACKTYPE_INT, SIZE_INT32,                            "RespecTokens",            OFFSET2_PTR(Entity, pl,            EntPlayer, respecTokens),                },
        "Bitfield - the set of respec tokens the player has earned."},

    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntPlayer, pendingReward),    "PendingReward",        {INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, pendingReward.pendingRewardName, 0)}      },
        "The name of the reward table to apply to the character (used when character is offline and is granted a reward)"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "PendingRewardVillian",    {INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, pendingReward.pendingRewardVgroup, 0)}      },
        "The villain group for the pending reward (used when character is offline and is granted a reward)"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "PendingRewardLevel",    {INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, pendingReward.pendingRewardLevel, 0)}      },
        "The level for the pending reward (used when character is offline and is granted a reward)"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "TitleBadge",            OFFSET2_PTR(Entity, pl,            EntPlayer, titleBadge),                    },
        "The character's current chosen Badge title"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "ChatSettings",            {INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, chat_settings.options, 0)}  },
        "Bitfield - Chat settings"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "PrimaryChatMinimized",    {INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, chat_settings.primaryChatMinimized, 0)}  },
        "Bitfield - Chat settings"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "MousePitch",            OFFSET2_PTR(Entity, pl,            EntPlayer, mousePitchOption),            },
        "Specifies how the camera follows the character<br>"
        "   0 = FREE<br>"
        "   1 = SPRING<br>"
        "   2 = FIXED"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "UiSettings2",            OFFSET2_PTR(Entity, pl,            EntPlayer, uiSettings2),                },
        "Bitfield - Misc UI settings"},

    {{ PACKTYPE_INT, SIZE_INT8,                            "UserSendChannel",        {INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, chat_settings.userSendChannel, 0)}  },
        "The default chat channel the character is chatting to"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "FreeTailorSessions",    OFFSET2_PTR(Entity, pl,            EntPlayer, freeTailorSessions),            },
        "The count of free tailor (costume change) sessions the character has earned"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "MapOptions",            OFFSET2_PTR(Entity, pl,            EntPlayer, mapOptions),                    },
        "Bitfield - Map display options"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "Notoriety",        OFFSET2_PTR(Entity, pl,    EntPlayer,    deprecated),                    },
        "The character's notoriety (difficulty) level<br>"
        "   0 = Heroic/Villainous<br>"
        "   1 = Tenacious/Malicious<br>"
        "   2 = Rugged/Vicious<br>"
        "   3 = Unyielding/Ruthless<br>"
        "   4 = Invincible/Relentless"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "ChatBubbleTextColor",    OFFSET2_PTR(Entity, pl,            EntPlayer, chatBubbleTextColor),        },
        "Color - Color of text in character's chat bubble (set in options)"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "ChatBubbleBackColor",    OFFSET2_PTR(Entity, pl,            EntPlayer, chatBubbleBackColor),        },
        "Color - Color of backgroudn bubble for character's chat bubble (set in options)"},

    {{ PACKTYPE_STR_UTF8,    SIZEOF2(EntPlayer, titleTheText),    "TitleTheText",            OFFSET2_PTR(Entity, pl,            EntPlayer, titleTheText),                },
        "If the character has a title leading with a definite article, this is the text to use for it."},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "DividerSuperName",        OFFSET2_PTR(Entity, pl,            EntPlayer, divSuperName),        },
        "Width of  name column in supergroup window"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "DividerSuperMap",        OFFSET2_PTR(Entity, pl,            EntPlayer, divSuperMap),        },
        "Width of map column in supergroup window"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "DividerSuperTitle",    OFFSET2_PTR(Entity, pl,            EntPlayer, divSuperTitle),        },
        "Width of title column in supergroup window"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "DividerEmailFrom",        OFFSET2_PTR(Entity, pl,            EntPlayer, divEmailFrom),        },
        "Width of from column in email window"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "DividerEmailSubject",    OFFSET2_PTR(Entity, pl,            EntPlayer, divEmailSubject),    },
        "Width of subject column in email window"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "DividerFriendName",    OFFSET2_PTR(Entity, pl,            EntPlayer, divFriendName),        },
        "Width of name column in friend window"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "DividerLfgName",        OFFSET2_PTR(Entity, pl,            EntPlayer, divLfgName),            },
        "Width of name column in search window"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "DividerLfgMap",        OFFSET2_PTR(Entity, pl,            EntPlayer, divLfgMap),            },
        "Width of map column in search window"},

    {{ PACKTYPE_INT, SIZE_INT8,                            "ChatBeta",                OFFSET2_PTR(Entity, pl,            EntPlayer, chatBeta),            },
        "Obsolete, unused"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "LfgFlags",                OFFSET2_PTR(Entity, pl,            EntPlayer, lfg),                },
        "Bitfield - What kind of groups the character is looking to join<br>"
        "   1 = Any<br>"
        "   2 = Hunt<br>"
        "   4 = Missions<br>"
        "   8 = Task Force<br>"
        "  16 = Trial<br>"
        "  32 = Arena<br>"
        "  64 = None"},

    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntPlayer, comment),        "Comment",                OFFSET2_PTR(Entity, pl,            EntPlayer, comment),            },
        "The comment displayed for this character in the search window"},

    {{ PACKTYPE_FLOAT, SIZE_FLOAT32,                                "TooltipDelay",            OFFSET2_PTR(Entity, pl,            EntPlayer, tooltipDelay),        },
        "Controls how rapidly tooltips pop up"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "UltraTailor",            OFFSET2_PTR(Entity, pl,            EntPlayer, ultraTailor),        },
        "This is a GM bit that can be set granting players the ability to change gender"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "ArenaPaid",            OFFSET2_PTR(Entity, pl,            EntPlayer, arena_paid),            },
        "This has the unique id of the last arena event this player paid for"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "ArenaPaidAmount",        OFFSET2_PTR(Entity, pl,            EntPlayer, arena_paid_amount),    },
        "This has the amount this player paid for his last arena event in case it needs to be refunded"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "ArenaPrizeAmount",        OFFSET2_PTR(Entity, pl,            EntPlayer, arena_prize_amount),    },
        "The last prize this player got"},

    {{ PACKTYPE_FLOAT, SIZE_FLOAT32,                                "Insight",                {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, attrCur.fInsight, 0)}  },
        "Unused - The amount of insight the character has (Invention system)"},

     {{ PACKTYPE_INT, SIZE_INT32,                            "CurrentAlt2Tray",        OFFSET3_PTR2(Entity, pl, EntPlayer, tray, Tray, current_trays[2]),        },
        "The index of the 3rd tray"},

    {{ PACKTYPE_FLOAT,    SIZE_FLOAT32,                                "MaxHitPoints",            {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, attrMax.fHitPoints, 0)}  },
        "The character's most recent max hit points. (Takes into account current buffs.)"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "WisdomPoints",            {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, iWisdomPoints, 0)}},
        "Unused - The amount of wisdom the character has earned (Invention system)"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "WisdomLevel",            {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, iWisdomLevel, 0)}},
        "Unused - The character's current widom level (Invention system)"},

    {{ PACKTYPE_INT,    SIZE_INT8,                            "PvPSwitch",            OFFSET2_PTR(Entity, pl,            EntPlayer, pvpSwitch),            },
        "Unused - If the player has volunteered for pvp"},

    {{ PACKTYPE_FLOAT, SIZE_FLOAT32,                                "Reputation",            OFFSET2_PTR(Entity, pl,            EntPlayer, reputationPoints),    },
        "The character's current PvP reputation"},

    {{ PACKTYPE_INT, SIZE_INT16,                            "VillainGurneyMapId",    OFFSET(Entity,villain_gurney_map_id),       },
        "The last static map with a villain hospital"},

    {{ PACKTYPE_INT, SIZE_INT8,                            "SkillsUnlocked",        {INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, skillsUnlocked, 0)},       },
        "Unused - If true, then the skills system is unlocked (Invention system)"},

    {{ PACKTYPE_FLOAT, SIZE_FLOAT32,                                "Rage",                    {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, attrCur.fRage, 0)}  },
        "The character's current Rage"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "ExitMissionContext",            OFFSET3_PTR(Entity, storyInfo,    StoryInfo, exithandle, StoryTaskHandle, context),    },
        "Specifies the information about the task that was just completed for displaying the exit mission text"},
    {{ PACKTYPE_INT,    SIZE_INT32,                            "ExitMissionSubHandle",            OFFSET3_PTR(Entity, storyInfo,    StoryInfo, exithandle, StoryTaskHandle, subhandle),    },
        "Specifies the information about the task that was just completed for displaying the exit mission text"},
    {{ PACKTYPE_INT,    SIZE_INT32,                            "ExitMissionCompoundPos",        OFFSET3_PTR(Entity, storyInfo,    StoryInfo, exithandle, StoryTaskHandle, compoundPos),    },
        "Specifies the information about the task that was just completed for displaying the exit mission text"},
    {{ PACKTYPE_INT,    SIZE_INT32,                            "ExitMissionOwnerId",            OFFSET2_PTR(Entity, storyInfo,    StoryInfo, exitMissionOwnerId),            },
        "Specifies the information about the task that was just completed for displaying the exit mission text"},
    {{ PACKTYPE_INT,    SIZE_INT8,                            "ExitMissionSuccess",            OFFSET2_PTR(Entity, storyInfo,    StoryInfo, exitMissionSuccess),            },
        "Specifies the information about the task that was just completed for displaying the exit mission text"},

    {{ PACKTYPE_INT,    SIZE_INT8,                            "TeamCompleteOption",            OFFSET2_PTR(Entity, pl,    EntPlayer, teamCompleteOption),            },
        "Specifies if the character wants credit for shared missions<br>"
        "   0 = Prompt<br>"
        "   1 = Always get credit<br>"
        "   2 = never get credit"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "TimeInSGMode",                    OFFSET2_PTR(Entity, pl,            EntPlayer, timeInSGMode),            },
        "The time this character has spent in Supergroup mode"},

    {{ PACKTYPE_INT,    SIZE_INT8,                            "UpdateTeamTask",                OFFSET2_PTR(Entity, storyInfo,    StoryInfo, taskStatusChanged),            },
        "Flag that tells the player to update the rest of the team about the task status"},
        
    {{ PACKTYPE_INT,    SIZE_INT32,                            "BuffSettings",                    OFFSET2_PTR(Entity, pl,            EntPlayer, buffSettings),            },
        "Bitfield - UI settings regarding buff icon display"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "RecipeInvBonus",                OFFSET2_PTR(Entity, pchar, Character, recipeInvBonusSlots),        },
        "Number of bonus recipe inventory slots"},
    {{ PACKTYPE_INT,    SIZE_INT32,                            "RecipeInvTotal",                OFFSET2_PTR(Entity, pchar, Character, recipeInvTotalSlots),        },
        "Number of total recipe inventory slots"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "SalvageInvBonus",                OFFSET2_PTR(Entity, pchar, Character, salvageInvBonusSlots),    },
        "Number of bonus salvage inventory slots"},
    {{ PACKTYPE_INT,    SIZE_INT32,                            "SalvageInvTotal",                OFFSET2_PTR(Entity, pchar, Character, salvageInvTotalSlots),    },
        "Number of total salvage inventory slots"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "AuctionInvBonus",                OFFSET2_PTR(Entity, pchar, Character, auctionInvBonusSlots),  },
        "Number of bonus auction inventory slots"},
    {{ PACKTYPE_INT,    SIZE_INT32,                            "AuctionInvTotal",                OFFSET2_PTR(Entity, pchar, Character, auctionInvTotalSlots),  },
        "Number of total auction inventory slots"},

    {{ PACKTYPE_INT, SIZE_INT32,                                "UiSettings3",                    OFFSET2_PTR(Entity, pl,            EntPlayer, uiSettings3),                },
        "Bitfield - Misc UI settings"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "StoredSalvageInvBonus",        OFFSET2_PTR(Entity, pchar, Character, storedSalvageInvBonusSlots),  },
        "Number of bonus stored salvage inventory slots"},
    {{ PACKTYPE_INT,    SIZE_INT32,                            "StoredSalvageInvTotal",        OFFSET2_PTR(Entity, pchar, Character, storedSalvageInvTotalSlots),  },
        "Number of total stored salvage inventory slots"},

    {{ PACKTYPE_STR_ASCII,    SIZEOF2(EntPlayer,accountServerLock), "AccSvrLock",                    OFFSET2_PTR(Entity, pl, EntPlayer, accountServerLock), INOUT(0,0), LINEDESCFLAG_INDEXEDCOLUMN  },
        "Used for account server transaction recovery"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "TrayIndexes",        OFFSET3_PTR2(Entity, pl, EntPlayer, tray, Tray, trayIndexes),      },
        "Bitfield for the current Tray Index of the 8 additional trays"},

    {{ PACKTYPE_INT,    SIZE_INT32,                            "HideField",        OFFSET2_PTR(Entity, pl, EntPlayer, hidden),  },
        "Bitfield for hide options"},
    {{ PACKTYPE_STR_ASCII,    SIZEOF2(EntPlayer,primarySet),        "originalPrimary",                OFFSET2_PTR(Entity, pl, EntPlayer, primarySet),        },
        "Original primary powerset"},
    {{ PACKTYPE_STR_ASCII,    SIZEOF2(EntPlayer,secondarySet),    "originalSecondary",            OFFSET2_PTR(Entity, pl, EntPlayer, secondarySet),    },
        "Original primary powerset"},
    {{ PACKTYPE_FLOAT, SIZE_FLOAT32,                                "MouseScrollSpeed",            OFFSET2_PTR(Entity, pl,            EntPlayer, mouseScrollSpeed),    },
        "The character's mouse scrolling speed"},

    {{ PACKTYPE_INT,  SIZE_INT32,                        "ExperienceRest",            {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, iExperienceRest, 0)}     },
        "The amount of rest accrued to be used"},

    {{ PACKTYPE_INT,  SIZE_INT32,    "CurBuild",                {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, iCurBuild, 0)}             },
        "The 0-based build number currently in use"},

    // The number of BuildLevel fields should match MAX_BUILD_NUM as defined in character_base.h  If you need to add more build numbers
    // add the fields here and then go increase MAX_BUILD_NUM to match
    {{ PACKTYPE_INT,  SIZE_INT32,    "LevelBuild0",                {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, iBuildLevels[0], 0)}       },
        "The level of build 0"},
    {{ PACKTYPE_INT,  SIZE_INT32,    "LevelBuild1",                {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, iBuildLevels[1], 0)}       },
        "The level of build 1"},
    {{ PACKTYPE_INT,  SIZE_INT32,    "LevelBuild2",                {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, iBuildLevels[2], 0)}       },
        "The level of build 2"},
    {{ PACKTYPE_INT,  SIZE_INT32,    "LevelBuild3",                {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, iBuildLevels[3], 0)}       },
        "The level of build 3"},
    {{ PACKTYPE_INT,  SIZE_INT32,    "LevelBuild4",                {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, iBuildLevels[4], 0)}       },
        "The level of build 4"},
    {{ PACKTYPE_INT,  SIZE_INT32,    "LevelBuild5",                {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, iBuildLevels[5], 0)}       },
        "The level of build 5"},
    {{ PACKTYPE_INT,  SIZE_INT32,    "LevelBuild6",                {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, iBuildLevels[6], 0)}       },
        "The level of build 6"},
    {{ PACKTYPE_INT,  SIZE_INT32,    "LevelBuild7",                {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, iBuildLevels[7], 0)}       },
        "The level of build 7"},

    {{ PACKTYPE_CONREF, CONTAINER_RAIDS,            "RaidsId",            OFFSET(Entity,raid_id),            INOUT(0,0),    LINEDESCFLAG_INDEXEDCOLUMN    },
        "Internal id - Identifies which raid group the character is in."},
    {{ PACKTYPE_CONREF, CONTAINER_LEVELINGPACTS,    "LevelingPactsId",    OFFSET(Entity,levelingpact_id),    INOUT(0,0),    LINEDESCFLAG_INDEXEDCOLUMN | LINEDESCFLAG_READONLY    },
        "Internal id - Identifies which leveling pact the character is in."},

    {{ PACKTYPE_INT, SIZE_INT32,                            "PendingArchitectTickets",    OFFSET2_PTR(Entity, pl, EntPlayer, pendingArchitectTickets), },
        "The count of pending architect tickets the character has earned"},

    {{ PACKTYPE_INT,  SIZE_INT32,    "BuildChangeTime",            {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, iBuildChangeTime, 0)}      },
        "Seconds since 2000 at which we can next change builds"},

    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntPlayer,buildNames[0]),    "BuildName0",    OFFSET2_PTR(Entity, pl, EntPlayer, buildNames[0]), },
    "Name of build 0"},                                        
    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntPlayer,buildNames[0]),    "BuildName1",    OFFSET2_PTR(Entity, pl, EntPlayer, buildNames[1]), },
    "Name of build 1"},
    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntPlayer,buildNames[0]),    "BuildName2",    OFFSET2_PTR(Entity, pl, EntPlayer, buildNames[2]), },
    "Name of build 2"},
    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntPlayer,buildNames[0]),    "BuildName3",    OFFSET2_PTR(Entity, pl, EntPlayer, buildNames[3]), },
    "Name of build 3"},
    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntPlayer,buildNames[0]),    "BuildName4",    OFFSET2_PTR(Entity, pl, EntPlayer, buildNames[4]), },
    "Name of build 4"},
    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntPlayer,buildNames[0]),    "BuildName5",    OFFSET2_PTR(Entity, pl, EntPlayer, buildNames[5]), },
    "Name of build 5"},
    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntPlayer,buildNames[0]),    "BuildName6",    OFFSET2_PTR(Entity, pl, EntPlayer, buildNames[6]), },
    "Name of build 6"},
    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntPlayer,buildNames[0]),    "BuildName7",    OFFSET2_PTR(Entity, pl, EntPlayer, buildNames[7]), },
    "Name of build 7"},

    {{ PACKTYPE_INT,    SIZE_INT32,                        "ExitMissionPlayerCreated",        OFFSET3_PTR(Entity, storyInfo,    StoryInfo, exithandle, StoryTaskHandle, bPlayerCreated),    },
        "Specifies the information about the task that was just completed for displaying the exit mission text"},

    {{ PACKTYPE_DATE,0,              "LastDayJobsStart",                OFFSET(Entity,last_day_jobs_start),    INOUT(0,0),    LINEDESCFLAG_INDEXEDCOLUMN    },
        "The date and time of the beginning of accumulated day job time."},

    {{ PACKTYPE_INT, SIZE_INT32,          "ArchitectMissionsCompleted",    OFFSET2_PTR(Entity, pl, EntPlayer, architectMissionsCompleted), },
        "Bitfield tracking which missions the player was there for the completion of"},

    {{ PACKTYPE_INT, SIZE_INT8,                            "PlayerSubType",OFFSET2_PTR(Entity, pl, EntPlayer, playerSubType),                },
        "Specifies the character's standing within their PlayerType<br>"
        "   0 = Normal<br>"
        "   1 = Paragon<br>"
        "   2 = Rogue" },
    {{ PACKTYPE_INT, SIZE_INT8,                            "InfluenceType",OFFSET2_PTR(Entity, pchar, Character, playerTypeByLocation),        },
        "Specifies the player's currency type for non-Praetorians<br>"
        "   0 = Influence<br>"
        "   1 = Infamy"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "InfluenceEscrow",OFFSET2_PTR(Entity, pchar, Character, iInfluenceEscrow),        },
        "Total Influence (not Infamy or Information) available. Copied to Influence if InfluenceType is 0"},

    {{ PACKTYPE_INT, SIZE_INT8,                            "AutoAcceptAbove",OFFSET2_PTR(Entity, pl, EntPlayer, autoAcceptTeamAbove),        },
        "The how far above their current level will be auto accepted by the player"},
    {{ PACKTYPE_INT, SIZE_INT8,                            "AutoAcceptBelow",OFFSET2_PTR(Entity, pl, EntPlayer, autoAcceptTeamBelow),        },
        "The how far below their current level will be auto accepted by the player"},

    {{ PACKTYPE_INT,        SIZE_INT32,            "LevelAdjust",   OFFSET3_PTR(Entity, storyInfo, StoryInfo, difficulty, StoryDifficulty, levelAdjust),},
        "Level adustment of enemies  ( -1 to +4 )"},
    {{ PACKTYPE_INT,        SIZE_INT32,            "TeamSize",         OFFSET3_PTR(Entity, storyInfo, StoryInfo, difficulty, StoryDifficulty, teamSize), },
        "Team size this player is treated as ( 1 to 8 )"},
    {{ PACKTYPE_INT,        SIZE_INT32,            "UpgradeAV",     OFFSET3_PTR(Entity, storyInfo, StoryInfo, difficulty, StoryDifficulty, alwaysAV), },
        "it true, don't downgrade AV to EB, otherwise always do"},
    {{ PACKTYPE_INT,        SIZE_INT32,            "DowngradeBoss", OFFSET3_PTR(Entity, storyInfo, StoryInfo, difficulty, StoryDifficulty, dontReduceBoss), },
        "if true, no bosses while solo"},

    {{ PACKTYPE_INT, SIZE_INT8,                            "PraetorianProgress",OFFSET2_PTR(Entity, pl, EntPlayer, praetorianProgress),    },
        "Whether the character is from Primal Earth or Praetorian and if so what progress they've made<br>"
        "   0 = Primal Earth-born<br>"
        "   1 = Praetorian, still in the tutorial<br>"
        "   2 = Praetorian, in Praetoria<br>"
        "   3 = Praetorian, on Primal Earth<br>"
        "   4 = Praetorian transferring to Paragon City<br>"
        "   5 = Praetorian transferring to the Rogue Isles<br>"
        "   6 = Primal Earth character in tutorial before choosing a side"},

    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntPlayer,specialMapReturnData),    "SpecialMapReturnData",    OFFSET2_PTR(Entity, pl, EntPlayer, specialMapReturnData), },
        "Special map return data - used to transfer back from one static map to another"},                                        

    {{ PACKTYPE_INT, SIZE_INT32,                            "IncarnateTimer0",        OFFSET2_PTR(Entity, pchar, Character, incarnateSlotTimesSlottable[0]),    },
        "The time when incarnate slot 0 will be slottable again."},
    {{ PACKTYPE_INT, SIZE_INT32,                            "IncarnateTimer1",        OFFSET2_PTR(Entity, pchar, Character, incarnateSlotTimesSlottable[1]),    },
        "The time when incarnate slot 1 will be slottable again."},
    {{ PACKTYPE_INT, SIZE_INT32,                            "IncarnateTimer2",        OFFSET2_PTR(Entity, pchar, Character, incarnateSlotTimesSlottable[2]),    },
        "The time when incarnate slot 2 will be slottable again."},
    {{ PACKTYPE_INT, SIZE_INT32,                            "IncarnateTimer3",        OFFSET2_PTR(Entity, pchar, Character, incarnateSlotTimesSlottable[3]),    },
        "The time when incarnate slot 3 will be slottable again."},
    {{ PACKTYPE_INT, SIZE_INT32,                            "IncarnateTimer4",        OFFSET2_PTR(Entity, pchar, Character, incarnateSlotTimesSlottable[4]),    },
        "The time when incarnate slot 4 will be slottable again."},
    {{ PACKTYPE_INT, SIZE_INT32,                            "IncarnateTimer5",        OFFSET2_PTR(Entity, pchar, Character, incarnateSlotTimesSlottable[5]),    },
        "The time when incarnate slot 5 will be slottable again."},
    {{ PACKTYPE_INT, SIZE_INT32,                            "IncarnateTimer6",        OFFSET2_PTR(Entity, pchar, Character, incarnateSlotTimesSlottable[6]),    },
        "The time when incarnate slot 6 will be slottable again."},
    {{ PACKTYPE_INT, SIZE_INT32,                            "IncarnateTimer7",        OFFSET2_PTR(Entity, pchar, Character, incarnateSlotTimesSlottable[7]),    },
        "The time when incarnate slot 7 will be slottable again."},
    {{ PACKTYPE_INT, SIZE_INT32,                            "IncarnateTimer8",        OFFSET2_PTR(Entity, pchar, Character, incarnateSlotTimesSlottable[8]),    },
        "The time when incarnate slot 8 will be slottable again."},
    {{ PACKTYPE_INT, SIZE_INT32,                            "IncarnateTimer9",        OFFSET2_PTR(Entity, pchar, Character, incarnateSlotTimesSlottable[9]),    },
        "The time when incarnate slot 9 will be slottable again."},

    {{PACKTYPE_INT, SIZE_INT32,                            "PopHelpLatest0",        OFFSET2_PTR(Entity,pl, EntPlayer,popHelpLatest[0]),        },
    "The latest pop helps to have been received, in order." },
    {{PACKTYPE_INT, SIZE_INT32,                            "PopHelpLatest1",        OFFSET2_PTR(Entity,pl, EntPlayer,popHelpLatest[1]),        },
    "The latest pop helps to have been received, in order." },
    {{PACKTYPE_INT, SIZE_INT32,                            "PopHelpLatest2",        OFFSET2_PTR(Entity,pl, EntPlayer,popHelpLatest[2]),        },
    "The latest pop helps to have been received, in order." },
    {{PACKTYPE_INT, SIZE_INT32,                            "PopHelpLatest3",        OFFSET2_PTR(Entity,pl, EntPlayer,popHelpLatest[3]),        },
    "The latest pop helps to have been received, in order." },
    {{PACKTYPE_INT, SIZE_INT32,                            "PopHelpLatest4",        OFFSET2_PTR(Entity,pl, EntPlayer,popHelpLatest[4]),        },
    "The latest pop helps to have been received, in order." },
    {{PACKTYPE_BIN_STR, SIZEOF2(EntPlayer,popHelpStatus),    "PopHelpStatus",    OFFSET2_PTR(Entity,pl, EntPlayer,popHelpStatus),        },
        "The state of each pop help, untriggered, unread, unseen, or dissmissed."    },
    {{ PACKTYPE_INT, SIZE_INT32,                            "TitleColor1",            OFFSET2_PTR(Entity, pl,    EntPlayer, titleColors[0]),        },
        "Color of title for veterans."},
    {{ PACKTYPE_INT, SIZE_INT32,                            "TitleColor2",            OFFSET2_PTR(Entity, pl,    EntPlayer, titleColors[1]),        },
        "Color of title for veterans."},

    {{ PACKTYPE_BIN_STR, 1024 / 8,                        "AuthUserDataEx",        OFFSET2_PTR(Entity, pl,    EntPlayer, auth_user_data),        },
        "A copy of the auth user data from the Auth server."},

    {{ PACKTYPE_CONREF, CONTAINER_LEAGUES,                "LeaguesId",            OFFSET(Entity,league_id), INOUT(0,0), LINEDESCFLAG_INDEXEDCOLUMN | LINEDESCFLAG_READONLY    },
        "Internal id - Identifies which league the character is in."},

    {{ PACKTYPE_INT, SIZE_INT8,                            "SpecialReturnInProgress",OFFSET2_PTR(Entity, pl, EntPlayer, specialMapReturnInProgress),    },
        "Is a specialMapReturnData transfer in progress?  Used to solve race condition problems." },
     {{ PACKTYPE_INT, SIZE_INT32,                            "CurrentRazerTray",    OFFSET3_PTR2(Entity, pl, EntPlayer, tray, Tray, current_trays[3] ),        },
        "The index of the Razer tray"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "RequiresGoingRogueOrTrial",    OFFSET2_PTR(Entity, pl,    EntPlayer,    deprecated),        },
        "" },

    {{ PACKTYPE_INT, SIZE_INT32,                            "HomeDBID",                OFFSET2_PTR(Entity, pl,    EntPlayer, homeDBID),            },
        "dbid on home shard, only relevant when visiting"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "HomeShard",            OFFSET2_PTR(Entity, pl,    EntPlayer, homeShard),            },
        "Home shard number"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "RemoteDBID",            OFFSET2_PTR(Entity, pl,    EntPlayer, remoteDBID),            },
        "Remote dbid when visiting.  This is a flag to the home shard that the character is remote."},
    {{ PACKTYPE_INT, SIZE_INT32,                            "VisitStartTime",        OFFSET2_PTR(Entity, pl,    EntPlayer, visitStartTime),        },
        "secondsSince2000 at which the character started the visit.  Used to ensure they go back home eventually"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "HomeSGID",                OFFSET2_PTR(Entity, pl,    EntPlayer, homeSGID),            },
        "Supergroup ID on home shard, only relevant when visiting"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "HomeLPID",                OFFSET2_PTR(Entity, pl,    EntPlayer, homeLPID),            },
        "Leveling Pact ID on home shard, only relevant when visiting"},
    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntPlayer,shardVisitorData), "ShardVisitorData",    OFFSET2_PTR(Entity, pl, EntPlayer, shardVisitorData),    },
        "Data used during shard visitor transfer.  Includes such things as league identifier, target map, target location"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "RemoteShard",            OFFSET2_PTR(Entity, pl,    EntPlayer, remoteShard),        },
        "Remote shard when visiting."},
    {{ PACKTYPE_INT, SIZE_INT32,                            "DisplayAlignmentStatsToOthers",    OFFSET2_PTR(Entity, pl, EntPlayer, displayAlignmentStatsToOthers),    },
        "Flag to denote whether this player's alignment stats (in the Alignment tab in the Player Info window) are visible to other players."},

    {{ PACKTYPE_INT, SIZE_INT32,                            "DesiredTeamNumber",    OFFSET2_PTR(Entity, pl,    EntPlayer, desiredTeamNumber),    },
        "Team number I want to be on in end game raid league"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "LastAutoCommandRunTime",    OFFSET(Entity, lastAutoCommandRunTime),    },
        "The time that the Auto Command system last ran commands against this Entity." },

    {{ PACKTYPE_INT, SIZE_INT32,                            "IsTeamLeader",            OFFSET2_PTR(Entity, pl,    EntPlayer, isTurnstileTeamLeader),    },
        "Promote me to team leader when turnstile starts"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "LastTurnstileEventID",    OFFSET2_PTR(Entity, pl,    EntPlayer, lastTurnstileEventID),    },
        "ID of the last turnstile event I was in"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "LastTurnstileMission",    OFFSET2_PTR(Entity, pl,    EntPlayer, lastTurnstileMissionID),    },
        "ID of the last turnstile mission I was in"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "TurnstileTeamLock",    OFFSET2_PTR(Entity, pl,    EntPlayer, turnstileTeamInterlock),    },
        "Turnstile team lock"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "PendingCertification0",    OFFSET2_PTR(Entity, pl,    EntPlayer, pendingCertification.u32[0] ),    },
        "Certification order we have not heard back from yet"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "PendingCertification1", OFFSET2_PTR(Entity, pl,    EntPlayer, pendingCertification.u32[1] ),    },
        "Certification order we have not heard back from yet"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "PendingCertification2", OFFSET2_PTR(Entity, pl,    EntPlayer, pendingCertification.u32[2] ),    },
        "Certification order we have not heard back from yet"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "PendingCertification3", OFFSET2_PTR(Entity, pl,    EntPlayer, pendingCertification.u32[3] ),    },
        "Certification order we have not heard back from yet"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "HelperStatus",            OFFSET2_PTR(Entity, pl, EntPlayer, helperStatus),    },
        "flag that determines whether the player is a newbie or a vet for help system purposes."},

    {{ PACKTYPE_INT, SIZE_INT32,                            "UiSettings4",            OFFSET2_PTR(Entity, pl,    EntPlayer, uiSettings4),                },
        "Bitfield - Misc UI settings"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "MapOptionRevision",    OFFSET2_PTR(Entity, pl, EntPlayer, mapOptionRevision),            },
        "Used to initialize the MapOptions and MapOptions2 values to handle new defaults we want to set."},

    {{ PACKTYPE_INT, SIZE_INT32,                            "MapOptions2",    OFFSET2_PTR(Entity, pl, EntPlayer, mapOptions2),            },
        "Bitfield - Map display options"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "SelectedContactOnZoneEnter",    OFFSET2_PTR(Entity, storyInfo, StoryInfo, delayedSelectedContact),    },
        "This contact (stored by handle #) will be selected as soon as the player next ticks, which may be after a mapmove.  Currently used by the Contact Finder."},

    {{ PACKTYPE_INT, SIZE_INT32,                           "PendingCertificationGrant", OFFSET2_PTR(Entity, pl,    EntPlayer, deprecated ),    },
        "Deprecated"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "TeamupTimer_ActivePlayer",    OFFSET(Entity, teamupTimer_activePlayer),    },
        "Moment in time when teamup_activePlayer is set to point to teamup (internal)"},

    {{ PACKTYPE_INT, SIZE_INT32,                           "ValidateCostume", OFFSET2_PTR(Entity, pl,    EntPlayer, validateCostume ),    },
        "If set, the primary costume should be validated on receipt of account inventory"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "NumCostumeStored",        OFFSET2_PTR(Entity, pl,            EntPlayer, num_costumes_stored),            },
        "The number of costume slots stored on this character"},

    {{ PACKTYPE_INT, SIZE_INT32,                           "DoNotKick", OFFSET2_PTR(Entity, pl,    EntPlayer, doNotKick ),    },
        "if set, the character will not be kicked for invalid cosutmes, ATs and Powersets"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "LastTurnstileStartTime",    OFFSET2_PTR(Entity, pl,    EntPlayer, lastTurnstileStartTime),    },
        "Time that turnstile started"},

    {{ PACKTYPE_INT, SIZE_INT8,                                "HideOpenSalvageWarning",    OFFSET2_PTR(Entity, pl, EntPlayer, hideOpenSalvageWarning),    },
        "Option to hide the open salvage warning dialog."},    

    {{ PACKTYPE_FLOAT,  SIZE_FLOAT32,                        "Absorb",            OFFSET2_PTR(Entity, pl,    EntPlayer, deprecated ),  },
        "Deprecated"},

    {{ PACKTYPE_INT,  SIZE_INT8,                            "hideStorePiecesState",        OFFSET2_PTR(Entity, pl, EntPlayer, hideStorePiecesState),    },
        "State of Hide Store Pieces in Tailor"},

    {{ PACKTYPE_FLOAT, SIZE_FLOAT32,                        "cursorScale",                OFFSET2_PTR(Entity, pl, EntPlayer, cursorScale),    },
        "Scale of the cursor"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "NewFeaturesVersion",        OFFSET2_PTR(Entity, pl,    EntPlayer, newFeaturesVersion),    },
        "Last version that New Features window was shown"},
        
    {{ PACKTYPE_INT, SIZE_INT32,                            "Passcode",        OFFSET2_PTR(Entity, pl,    EntPlayer, passcode),    },
        "Last passcode used to enter a supergroup base."},
    // Add new single fields for entities above this line.
    // New Subs should go at the end of the original ent_line_desc below, i.e., NO SUBS GO HERE!
    { 0 },
};
StructDesc ent2_desc[] =
{
    sizeof(Entity),
    {AT_NOT_ARRAY,{0}},
    ent2_line_desc,

    "Each character (entity) has a single row in the Ents2 table.<br>"
    "Every character is given a unique identifier, the ContainerID, which is used to relate the character to data found in other tables as well."
};

LineDesc ent_line_desc[] =
{
    {{ PACKTYPE_CONREF, CONTAINER_TEAMUPS,        "TeamupsId",        OFFSET(Entity,teamup_id),        INOUT(0,0),LINEDESCFLAG_INDEXEDCOLUMN    },
        "Internal id - Identifys which 8-player team the character is on."},

    {{ PACKTYPE_CONREF, CONTAINER_SUPERGROUPS,    "SupergroupsId",    OFFSET(Entity,supergroup_id),   INOUT(0,0),LINEDESCFLAG_INDEXEDCOLUMN    },
        "Database id - Identifys which supergroup the character is in."},

    {{ PACKTYPE_CONREF,    CONTAINER_TASKFORCES,    "TaskforcesId",        OFFSET(Entity,taskforce_id),    INOUT(0,0),LINEDESCFLAG_INDEXEDCOLUMN    },
        "Internal id - Identifys which task force group the character is in."},

    {{ PACKTYPE_INT, SIZE_INT32,                    "AuthId",            OFFSET(Entity,auth_id),        INOUT(0,0),LINEDESCFLAG_INDEXEDCOLUMN                },
        "Account id (from the auth server) for the owner of the character"},

    {{ PACKTYPE_STR_ASCII, SIZEOF2(Entity,auth_name),    "AuthName",            OFFSET(Entity,auth_name),    INOUT(0,0),LINEDESCFLAG_INDEXEDCOLUMN                },
        "Account name (from the auth server) for the owner of the character"},

    {{ PACKTYPE_STR_ASCII, SIZEOF2(Entity,name),        "Name",                OFFSET(Entity,name),        INOUT(0,0),LINEDESCFLAG_INDEXEDCOLUMN, MAX_NAME_LEN    },
        "Character name"},

    {{ PACKTYPE_INT, SIZE_INT16,        "StaticMapId",        OFFSET(Entity,static_map_id),       },
        "Map id - The last static map the player was on (usually a city zone). When they exit a mission, they will be sent to this zone."},

    {{ PACKTYPE_INT, SIZE_INT16,        "MapId",            OFFSET(Entity,map_id),              },
        "Map id - The ID of the map they are currently on."},

#define ENTLINE_WRITE_IDX 8 // Above items are read-only

    {{ PACKTYPE_FLOAT, SIZE_FLOAT32,            "PosX",                OFFSET2_PTR(Entity, pl, EntPlayer, last_static_pos[0]) },
        "Current location of the character in their current map."},
    {{ PACKTYPE_FLOAT, SIZE_FLOAT32,            "PosY",                OFFSET2_PTR(Entity, pl, EntPlayer, last_static_pos[1]) },
        "Current location of the character in their current map."},
    {{ PACKTYPE_FLOAT, SIZE_FLOAT32,            "PosZ",                OFFSET2_PTR(Entity, pl, EntPlayer, last_static_pos[2]) },
        "Current location of the character in their current map."},

    {{ PACKTYPE_FLOAT,    SIZE_FLOAT32,            "OrientP",            OFFSET2_PTR(Entity, pl, EntPlayer, last_static_pyr[0]) },
        "Current orientation of the character ."},
    {{ PACKTYPE_FLOAT,    SIZE_FLOAT32,            "OrientY",            OFFSET2_PTR(Entity, pl, EntPlayer, last_static_pyr[1]) },
        "Current orientation of the character ."},
    {{ PACKTYPE_FLOAT,    SIZE_FLOAT32,            "OrientR",            OFFSET2_PTR(Entity, pl, EntPlayer, last_static_pyr[2]) },
        "Current orientation of the character ."},

    {{ PACKTYPE_INT, SIZE_INT32,        "TotalTime",        OFFSET(Entity,total_time),          },
        "The number of seconds the character has been online. Only updated on map moves and log out."},

    {{ PACKTYPE_INT, SIZE_INT32,        "LoginCount",        OFFSET(Entity,login_count),         },
        "The number of times the character has been logged in."},

    {{ PACKTYPE_DATE,0,              "LastActive",        OFFSET(Entity,last_time),           },
        "The date and time of the last log in."},

    {{ PACKTYPE_INT, SIZE_INT8,        "AccessLevel",        OFFSET(Entity,access_level),        },
        "The command access level. Normal players are always 0. GMs have higher values."},

    {{ PACKTYPE_DATE,0,              "ChatBanExpire",    OFFSET(Entity,chat_ban_expire),     },
        "The date and time when the character will be allowed to send chat again."},

    {{ PACKTYPE_INT, SIZE_INT32,    "DbFlags",            OFFSET(Entity,db_flags),            },
        "Bitfield - used for internal database communication"},

    {{ PACKTYPE_INT, SIZE_INT8,        "Locale",            OFFSET(Entity,playerLocale),        },
        "Sets the language and other locale defaults"},

    {{ PACKTYPE_INT, SIZE_INT16,    "GurneyMapId",        OFFSET(Entity,gurney_map_id),       },
        "Map id - The ID of the most recently visited static map which contains a hospital. If the character is defeated, they will get sent to this hospital."},

    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntPlayer,titleCommon), "TitleCommon",        OFFSET2_PTR(Entity, pl,    EntPlayer, titleCommon),    },
        "The common title of the character, chosen at level 15"},

    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntPlayer,titleOrigin), "TitleOrigin",        OFFSET2_PTR(Entity, pl,    EntPlayer, titleOrigin),    },
        "The origin title of the character, chosen at level 20"},

    {{ PACKTYPE_FLOAT, SIZE_FLOAT32,            "MouseSpeed",        OFFSET2_PTR(Entity, pl,    EntPlayer, mouse_speed),        },
        "A multiplier for mouse speed (sensitivity). This is set in the options screen."},

    {{ PACKTYPE_FLOAT, SIZE_FLOAT32,            "TurnSpeed",        OFFSET2_PTR(Entity, pl,    EntPlayer, turn_speed),            },
        "A multiplier for mouse turning speed (sensitivity). This is set in the options screen."},

    {{ PACKTYPE_INT, SIZE_INT32,    "TopChatFilter",    OFFSET2_PTR(Entity, pl,    EntPlayer, topChatChannels),    },
        "Bitfield - designates which types of chat appear in the top pane of the chat window."},

    {{ PACKTYPE_INT, SIZE_INT32,    "BotChatFilter",    OFFSET2_PTR(Entity, pl,    EntPlayer, botChatChannels),    },
        "Bitfield - designates which types of chat appear in the bottom pane of the chat window."},

    {{ PACKTYPE_INT, SIZE_INT32,    "ChatSendChannel",    OFFSET2_PTR(Entity, pl,    EntPlayer, chatSendChannel),    },
        "The id of the output channel in the chat window"},

    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntPlayer, keyProfile), "KeyProfile",        OFFSET2_PTR(Entity, pl,    EntPlayer, keyProfile),            },
        "The name of the keybind profile the player has chosen on the options screen."},

    {{ PACKTYPE_INT, SIZE_INT32,    "KeybindCount",        OFFSET2_PTR(Entity, pl, EntPlayer, keybind_count),        },
        "The number of keybinds currently in use"},

    {{ PACKTYPE_INT, SIZE_INT32,    "FriendCount",        OFFSET(Entity,friendCount),                                },
        "The number of local friends"},

    {{ PACKTYPE_INT, SIZE_INT32,    "Rank",                OFFSET2_PTR(Entity, superstat, SupergroupStats,    rank),                },
        "The character's supergroup rank.<br>"
        "   0 = member (lowest rank)<br>"
        "   1 = lieutenant<br>"
        "   2 = captain<br>"
        "   3 = commander<br>"
        "   4 = leader (highest rank)"},

    {{ PACKTYPE_INT,    SIZE_INT32,        "TimePlayed",        OFFSET2_PTR(Entity, superstat, SupergroupStats,    total_time_played),    },
        "Unused - The number of seconds the character has been in supergroup mode."},

    {{ PACKTYPE_DATE,0,                "MemberSince",        OFFSET2_PTR(Entity, superstat, SupergroupStats,    member_since),        },
        "The date and time when the character joined their supergroup"},

    {{ PACKTYPE_INT, SIZE_INT8,        "TaskForceMode",    OFFSET2_PTR(Entity, pl,            EntPlayer,        taskforce_mode),    },
        "If 1, the player is on a task force and in task force mode. If 0, they are not. 2 if its an architect taskforce"},

    {{ PACKTYPE_INT, SIZE_INT8,        "BodyType",     {INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, costume[0], 1), INDIRECTION(Costume, appearance.bodytype, 0),}         },
        "Gender and Body Type:<br>"
        "0 = Male<br>"
        "1 = Female<br>"
        "4 = Huge "},

    {{ PACKTYPE_FLOAT, SIZE_FLOAT32,            "BodyScale",    {INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, costume[0], 1), INDIRECTION(Costume, appearance.fScale, 0),}         },
        "Overall body scale."},

    {{ PACKTYPE_FLOAT, SIZE_FLOAT32,            "BoneScale",    {INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, costume[0], 1), INDIRECTION(Costume, appearance.fBoneScale, 0),}     },
        "Overall bone scale."},

    {{ PACKTYPE_INT, SIZE_INT32,        "ColorSkin",    {INDIRECTION(Entity, pl, 1), INDIRECTION(EntPlayer, costume[0], 1), INDIRECTION(Costume, appearance.colorSkin, 0),}        },
        "Color - skin color"},

    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntStrings, motto),                "Motto",            OFFSET2_PTR(Entity, strings,    EntStrings, motto ),            },
        "The character's battle cry (motto), entered on the ID screen"},

    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntStrings, playerDescription), "Description",        OFFSET2_PTR(Entity, strings,    EntStrings, playerDescription ),},
        "The character's description, entered on the ID screen"},

    {{ PACKTYPE_INT, SIZE_INT32,        "CurrentTray",        OFFSET3_PTR2(Entity, pl, EntPlayer, tray, Tray, current_trays[0]),        },
        "The index of the main tray"},

    {{ PACKTYPE_INT, SIZE_INT32,        "CurrentAltTray",    OFFSET3_PTR2(Entity, pl, EntPlayer, tray, Tray, current_trays[1]),        },
        "The index of the alternate (2nd) tray"},

    {{ PACKTYPE_FLOAT, SIZE_FLOAT32,            "ChatDivider",        OFFSET2_PTR(Entity, pl,            EntPlayer,    chat_divider),        },
        "The location of the chat divider in the chat window."},

    {{ PACKTYPE_STR_UTF8, SIZEOF2(Entity,spawn_target),   "SpawnTarget",        OFFSET(Entity,spawn_target),    },
        "The name of a beacon where the character will be spawned."},

    // Character stuff except for class, origin, and powers (which is done separately)
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,  "Class",                {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, pclass, 0) }, INOUT(AttrToCharacterClass, CharacterClassToAttr)        },
        "Attribute - The Archetype (class) of the character"},

    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN,  "Origin",                {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, porigin, 0) }, INOUT(AttrToCharacterOrigin, CharacterOriginToAttr)    },
        "Attribute - The Origin of the character"},

    {{ PACKTYPE_INT,  SIZE_INT32,    "Level",                {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, iLevel, 0)}              },
        "The 0-based security level of the character. (If this value is 4, then the character's level is 5)"},

    {{ PACKTYPE_INT,  SIZE_INT32,    "ExperiencePoints",        {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, iExperiencePoints, 0)}   },
        "The number of experience points earned"},

    {{ PACKTYPE_INT,  SIZE_INT32,    "ExperienceDebt",        {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, iExperienceDebt, 0)}     },
        "The amount of debt accrued to be worked off"},

    {{ PACKTYPE_INT,  SIZE_INT32,    "InfluencePoints",        {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, iInfluencePoints, 0)}    },
        "The amount of influence the character currently has"},

    {{ PACKTYPE_FLOAT,  SIZE_FLOAT32,           "HitPoints",            {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, attrCur.fHitPoints, 0)}  },
        "The character's current hit points"},

    {{ PACKTYPE_FLOAT,  SIZE_FLOAT32,           "Endurance",            {INDIRECTION(Entity, pchar, 1), INDIRECTION(Character, attrCur.fEndurance, 0)}  },
        "The character's current endurance"},

    {{ PACKTYPE_INT, SIZE_INT8,                            "ChatFontSize",            OFFSET2_PTR(Entity, pl,            EntPlayer, chat_font_size),                },
        "The size of the chat font (set in options)"},

    {{ PACKTYPE_BIN_STR,    UNIQUE_TASK_BITFIELD_SIZE*4,    "UniqueTaskIssued",        OFFSET2_PTR(Entity, storyInfo,    StoryInfo, uniqueTaskIssued),            },
        "Bitfield - Tracks which unique tasks have been given to the character"},

    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntPlayer, titleSpecial),    "TitleSpecial",            OFFSET2_PTR(Entity, pl,            EntPlayer, titleSpecial),                },
        "The special title given to the character (given by GMs)"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "TitlesChosen",            OFFSET2_PTR(Entity, pl,            EntPlayer, iTitlesChosen),                },
        "Tracks which which titles (common and origin) the character has chosen."},

    {{ PACKTYPE_INT, SIZE_INT32,                            "TitleSpecialExpires",    OFFSET2_PTR(Entity, pl,            EntPlayer, titleSpecialExpires),        },
        "The number of seconds which the character keeps the special title."},

    {{ PACKTYPE_BIN_STR, 16,                                "AuthUserData",            OFFSET2_PTR(Entity, pl,            EntPlayer, old_auth_user_data),        },
        "A copy of the auth user data from the Auth server."},

    {{ PACKTYPE_INT, SIZE_INT32,                            "UiSettings",            OFFSET2_PTR(Entity, pl,            EntPlayer, uiSettings),                    },
        "Bitfield - Misc UI settings (set in the options screen)"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "ShowSettings",            OFFSET2_PTR(Entity, pl,            EntPlayer, showSettings),                },
        "Bitfield - UI settings for showing reticles, health bars, names, etc. (set in the options screen)"},

    {{ PACKTYPE_INT, SIZE_INT16,                            "NPCCostume",            OFFSET2_PTR(Entity, pl,            EntPlayer, npc_costume),                },
        "If the character has been shape-changed, the index of the NPC Costume to use."},

    {{ PACKTYPE_INT, SIZE_INT8,                                    "Banned",                OFFSET2_PTR(Entity, pl,            EntPlayer, banned),                        },
        "If 1, the character cannot be logged in. (Set and unset by GMs.)"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "NumCostumeSlots",        OFFSET2_PTR(Entity, pl,            EntPlayer, num_costume_slots),            },
        "The number of costume slots this character has earned"},

    // If we ever go four color across the board, the six Super* values here may get more added to them.  If that happens, the mirrors for these
    // in the appearance_line_desc will need modification
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimary",            OFFSET2_PTR(Entity, pl,            EntPlayer, superColorsPrimaryU.SGBitSetLow),            },
        "Bitfield - For the Supergroup costume, determines if primary color is original or one of the supergroup colors"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimaryHigh",            OFFSET2_PTR(Entity, pl,            EntPlayer, superColorsPrimaryU.SGBitSetHigh),            },
        "Bitfield - For the Supergroup costume, determines if primary color is original or one of the supergroup colors, high bits"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondary",        OFFSET2_PTR(Entity, pl,            EntPlayer, superColorsSecondaryU.SGBitSetLow),        },
    "Bitfield - For the Supergroup costume, determines if secondary color is original or one of the supergroup colors"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondaryHigh",        OFFSET2_PTR(Entity, pl,            EntPlayer, superColorsSecondaryU.SGBitSetHigh),        },
    "Bitfield - For the Supergroup costume, determines if secondary color is original or one of the supergroup colors, high bits"},


    {{ PACKTYPE_INT, SIZE_INT32,                            "CurrentCostume",        OFFSET2_PTR(Entity, pl,            EntPlayer, current_costume),            },
        "Index of the costume the character is currently wearing."},

    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimary2",        OFFSET2_PTR(Entity, pl,            EntPlayer, superColorsPrimary2U.SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, SuperPrimary was not large enough for all parts"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperPrimary2High",        OFFSET2_PTR(Entity, pl,            EntPlayer, superColorsPrimary2U.SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, SuperPrimary was not large enough for all parts, high bits"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondary2",        OFFSET2_PTR(Entity, pl,            EntPlayer, superColorsSecondary2U.SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, SuperSecondary was not large enough for all parts"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperSecondary2High",        OFFSET2_PTR(Entity, pl,            EntPlayer, superColorsSecondary2U.SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, SuperSecondary was not large enough for all parts, high bits"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperTertiary",        OFFSET2_PTR(Entity, pl,            EntPlayer, superColorsTertiaryU.SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperTertiaryHigh",        OFFSET2_PTR(Entity, pl,            EntPlayer, superColorsTertiaryU.SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode, high bits"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperQuaternary",        OFFSET2_PTR(Entity, pl,            EntPlayer, superColorsQuaternaryU.SGBitSetLow),        },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode"},
    {{ PACKTYPE_INT, SIZE_INT32,                            "SuperQuaternaryHigh",        OFFSET2_PTR(Entity, pl,            EntPlayer, superColorsQuaternaryU.SGBitSetHigh),        },
        "Bitfield - For the Supergroup costume, some parts have 4 colors, determines which color is used for supergorup mode, high bits"},

    {{ PACKTYPE_STR_UTF8, SIZEOF2(EntPlayer, costumeFxSpecial),    "FxSpecial",        OFFSET2_PTR(Entity, pl,            EntPlayer, costumeFxSpecial),            },
        "Color - TODO"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "FxSpecialExpires",        OFFSET2_PTR(Entity, pl,            EntPlayer, costumeFxSpecialExpires),    },
        "Color - TODO"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "CsrModified",            OFFSET2_PTR(Entity, pl,            EntPlayer, csrModified),                },
        "If non-zero, then a GM has modified this character in some way. (Used to exclude the character in data mining.)"},

    {{ PACKTYPE_DATE, 0,                                    "DateCreated",            OFFSET2_PTR(Entity, pl,            EntPlayer, dateCreated),                },
        "The date that the character was created."},

    {{ PACKTYPE_INT, SIZE_INT8,                            "Gender",                OFFSET(Entity,gender),                                                    },
        "The gender of the character (used to match gender in gendered languages)<br>"
        "   0 = UNDEFINED<br>"
        "   1 = NEUTER<br>"
        "   2 = MALE<br>"
        "   3 = FEMALE"},

    {{ PACKTYPE_INT, SIZE_INT8,                            "NameGender",            OFFSET(Entity,name_gender),                                                },
        "The gender of the name of the character (used to match gender in gendered languages)<br>"
        "   0 = UNDEFINED<br>"
        "   1 = NEUTER<br>"
        "   2 = MALE<br>"
        "   3 = FEMALE"},

    {{ PACKTYPE_INT, SIZE_INT8,                            "PlayerType",            OFFSET2_PTR(Entity, pl,            EntPlayer, playerType),                    },
        "Specifies if the player is a hero or a villain<br>"
        "   0 = Hero<br>"
        "   1 = Villain"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "Prestige",                OFFSET2_PTR(Entity, pl,            EntPlayer, prestige),                    },
        "How much prestige the character has earned while a member of their current supergroup."},

    {{ PACKTYPE_INT, SIZE_INT32,                            "IsSlotLocked",        OFFSET2_PTR(Entity, pl,            EntPlayer, slotLockLevel),                },
        "Current lock state of this character.  0 is unlocked, 1 is locked, 2 is offlined (and locked)."},


    // DO NOT ADD ANY MORE SINGLE FIELDS!
    // Add new single fields to the ent2_line_desc above.
    // Continue to add all Subs below

    // Type Sub must go last
    { PACKTYPE_SUB,        1,                            "Ents2",            (intptr_t)ent2_desc,                    },
    { PACKTYPE_EARRAY,    (intptr_t)trayobj_Create,        "Tray",                (intptr_t)tray_desc,                    },
    { PACKTYPE_SUB,        MAX_FRIENDS,                "Friends",            (intptr_t)friend_list_desc,            },
    { PACKTYPE_SUB,        MAX_WINDOW_COUNT,            "Windows",            (intptr_t)window_desc,                },
    { PACKTYPE_SUB,        BIND_MAX,                    "KeyBinds",            (intptr_t)&keybind_desc,                },
    { PACKTYPE_SUB,        MAX_COSTUME_PARTS,            "SuperCostumeParts",(intptr_t)&super_costume_part_desc,    },
    { PACKTYPE_EARRAY,    (intptr_t)visitedMap_Create,        "VisitedMaps",        (intptr_t)visited_maps_desc,        },
    { PACKTYPE_SUB,        NUM_FAME_STRINGS,            "FameStrings",        (intptr_t)famestring_list_desc,        },
    { PACKTYPE_SUB,        MAX_CHAT_WINDOWS,            "ChatWindows",        (intptr_t)chat_settings_window_desc    },
    { PACKTYPE_SUB,        MAX_CHAT_TABS,                "ChatTabs",            (intptr_t)chat_settings_tab_desc        },
    { PACKTYPE_SUB,        MAX_CHAT_CHANNELS,            "ChatChannels",        (intptr_t)chat_settings_channel_desc },
    { PACKTYPE_SUB,        MAX_DEFEAT_RECORDS,            "DefeatRecord",        (intptr_t)defeat_list_desc },
    { PACKTYPE_EARRAY, (intptr_t)rewardtoken_Create,        "RewardTokens",        (intptr_t)player_reward_token_desc            },
    { PACKTYPE_EARRAY, (intptr_t)rewardtoken_Create,        "RewardTokensActive",(intptr_t)active_player_reward_token_desc            },

    // Story arc stuff
    { PACKTYPE_SUB, CONTACTS_PER_PLAYER,        "Contacts",            (intptr_t)contact_desc        },
    { PACKTYPE_SUB,    STORYARCS_PER_PLAYER,        "StoryArcs",        (intptr_t)storyarc_desc        },
//    { PACKTYPE_EARRAY, (int)StoryClueInfoCreate, "StoryClues",        (int)storyclue_desc        },
// StoryClueInfo is currently unused and deprecated, but if we resurrect it, it should go here
    { PACKTYPE_SUB,    TASKS_PER_PLAYER,            "Tasks",            (intptr_t)task_desc            },
    { PACKTYPE_SUB, SOUVENIRCLUES_PER_PLAYER,    "SouvenirClues",    (intptr_t)souvenirClue_desc    },
    { PACKTYPE_SUB, 1,                            "NewspaperHistory",    (intptr_t)newspaper_history_desc    },

    { PACKTYPE_EARRAY, (intptr_t)petname_Create,                "PetNames",            (intptr_t)petname_desc        },
    { PACKTYPE_EARRAY, (intptr_t)mapHistoryCreate,            "MapHistory",        (intptr_t)maphistory_desc    },
    { PACKTYPE_EARRAY, (intptr_t)detailinventoryitem_Create,    "InvBaseDetail",    (intptr_t)invbasedetail_desc    },

    { PACKTYPE_SUB, MAX_COMBAT_STATS,            "CombatMonitorStat",(intptr_t)combatMonitorStat_desc    },
    { PACKTYPE_SUB, BADGE_ENT_RECENT_BADGES,    "RecentBadge",        (intptr_t)recentBadge_desc    },
    { PACKTYPE_SUB, MAX_BADGE_MONITOR_ENTRIES,    "BadgeMonitor",        (intptr_t)badgeMonitor_desc    },

    { PACKTYPE_SUB,        MAX_IGNORES,            "Ignore",            (intptr_t)ignore_list_desc,            },
    { PACKTYPE_SUB,        MAX_GMAIL,                "GmailClaims",        (intptr_t)&gmail_claim_state_desc },
    { PACKTYPE_SUB,        1,                        "GmailPending",        (intptr_t)gmail_pending_desc },
    { PACKTYPE_SUB,        MAX_REWARD_TABLES,        "QueuedRewardTables",    (intptr_t)&queued_reward_desc },
    { PACKTYPE_SUB,        MARTY_ExperienceTypeCount,    "MARTYTracks",    (intptr_t)&marty_reward_track_desc },
    { PACKTYPE_EARRAY, (intptr_t)certificationRecord_Create,    "CertificationHistory",    (intptr_t)certificationhistory_desc    },
    { PACKTYPE_EARRAY, (intptr_t)orderId_Create,        "CompletedOrders",    (intptr_t)completed_order_desc    },
    { PACKTYPE_EARRAY, (intptr_t)orderId_Create,        "PendingOrders",    (intptr_t)pending_order_desc        },
    { 0 },
};

StructDesc ent_desc[] =
{
    sizeof(Entity),
    {AT_NOT_ARRAY,{0}},
    ent_line_desc,

    "Each character (entity) has a single row in the Ents table.<br>"
    "Every character is given a unique identifier, the ContainerID, which is used to relate the character to data found in other tables as well."
};

LineDesc ent_schema_desc[] =
{
    { PACKTYPE_SUB,        1,            "Ents",        (intptr_t)ent_desc },
    { 0 }
};

/////////////////////////////////////////////////////////////////////////////////
// Powers data block
/////////////////////////////////////////////////////////////////////////////////

LineDesc power_line_desc[] =
{
    {{ PACKTYPE_INT,       SIZE_INT32,   "PowerID",               OFFSET(DBPower,iID),                       },
        "An ID used to relate boosts (enhancements) to this power. All boosts which specify this power ID are slotted into this power."},

    {{ PACKTYPE_ATTR,      MAX_ATTRIBNAME_LEN, "CategoryName",          OFFSET(DBPower,dbpowBase.pchCategoryName), },
        "The power is identified by the CategoryName, the PowerSetName within the category, and the PowerName within the power set."},
    {{ PACKTYPE_ATTR,      MAX_ATTRIBNAME_LEN, "PowerSetName",          OFFSET(DBPower,dbpowBase.pchPowerSetName), },
        "The power is identified by the CategoryName, the PowerSetName within the category, and the PowerName within the power set."},
    {{ PACKTYPE_ATTR,      MAX_ATTRIBNAME_LEN, "PowerName",             OFFSET(DBPower,dbpowBase.pchPowerName),    },
        "The power is identified by the CategoryName, the PowerSetName within the category, and the PowerName within the power set."},

    {{ PACKTYPE_INT,       SIZE_INT32,   "PowerLevelBought",      OFFSET(DBPower,iLevelBought),              },
        "The 0-based character level at which the power was selected."},

    {{ PACKTYPE_INT,       SIZE_INT32,   "PowerNumBoostsBought",  OFFSET(DBPower,iNumBoostsBought),          },
        "The number of extra boost (enhancement) slots the character has assigned to this power."},

    {{ PACKTYPE_INT,       SIZE_INT32,   "PowerSetLevelBought",   OFFSET(DBPower,iPowerSetLevelBought),      },
        "The 0-based character level at which the power set was bought."},

    {{ PACKTYPE_INT,       SIZE_INT32,   "NumCharges",            OFFSET(DBPower,iNumCharges),               },
        "If the power has a limited number of charges, the number remaining before it is removed."},

    {{ PACKTYPE_FLOAT,             SIZE_FLOAT32,   "UsageTime",             OFFSET(DBPower,fUsageTime),                },
        "If the power has limited time usage, how much time remains before it is removed."},

    {{ PACKTYPE_INT,       SIZE_INT32,   "CreationTime",          OFFSET(DBPower,ulCreationTime),            },
        "SecsSince2000 - The time when the power was granted or selected"},

    {{ PACKTYPE_INT,       SIZE_INT32,   "RechargedAt",           OFFSET(DBPower,ulRechargedAt),             },
        "SecsSince2000 - The time when the power will be recharged"},

    {{ PACKTYPE_INT,                SIZE_INT8,   "Active",                OFFSET(DBPower,bActive),                   },
        "Bitfield = Specifies if the power is active and if it is the default power<br>"
        "   1 = Active<br>"
        "   2 = Default"},

    {{ PACKTYPE_FLOAT,             SIZE_FLOAT32,   "Var0",                  OFFSET(DBPower,afVars[0]),                 },
        "Unused - (Invention system)"},
    {{ PACKTYPE_FLOAT,             SIZE_FLOAT32,   "Var1",                  OFFSET(DBPower,afVars[1]),                 },
        "Unused - (Invention system)"},
    {{ PACKTYPE_FLOAT,             SIZE_FLOAT32,   "Var2",                  OFFSET(DBPower,afVars[2]),                 },
        "Unused - (Invention system)"},
    {{ PACKTYPE_FLOAT,             SIZE_FLOAT32,   "Var3",                  OFFSET(DBPower,afVars[3]),                 },
        "Unused - (Invention system)"},

    // the powerup slots
    {{ PACKTYPE_INT,    SIZE_INT32,    "SlottedPowerupsMask",    OFFSET(DBPower,iSlottedPowerupsMask), },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType0",        OFFSET(DBPower, aPowerupSlots[0].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId0",        OFFSET(DBPower, aPowerupSlots[0].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType1",        OFFSET(DBPower, aPowerupSlots[1].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId1",        OFFSET(DBPower, aPowerupSlots[1].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType2",        OFFSET(DBPower, aPowerupSlots[2].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId2",        OFFSET(DBPower, aPowerupSlots[2].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType3",        OFFSET(DBPower, aPowerupSlots[3].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId3",        OFFSET(DBPower, aPowerupSlots[3].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType4",        OFFSET(DBPower, aPowerupSlots[4].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId4",        OFFSET(DBPower, aPowerupSlots[4].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType5",        OFFSET(DBPower, aPowerupSlots[5].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId5",        OFFSET(DBPower, aPowerupSlots[5].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType6",        OFFSET(DBPower, aPowerupSlots[6].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId6",        OFFSET(DBPower, aPowerupSlots[6].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType7",        OFFSET(DBPower, aPowerupSlots[7].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId7",        OFFSET(DBPower, aPowerupSlots[7].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType8",        OFFSET(DBPower, aPowerupSlots[8].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId8",        OFFSET(DBPower, aPowerupSlots[8].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType9",        OFFSET(DBPower, aPowerupSlots[9].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId9",        OFFSET(DBPower, aPowerupSlots[9].id) },
        "Unused - (Invention system)"},

    {{ PACKTYPE_FLOAT,    SIZE_FLOAT32,       "AvailableTime",        OFFSET(DBPower, fAvailableTime),      },
        "If the power has in-game limited time usage, how much time it has been available to the player."},

    {{ PACKTYPE_INT,    SIZE_INT32,    "BuildNum",                OFFSET(DBPower, iBuildNum) },
        "Zero based build number this lives in"},

    {{ PACKTYPE_INT,    SIZE_INT8,    "Disabled",                OFFSET(DBPower, bDisabled) },
        "If this is true, the power is grayed out and unusable.  Added for Universal Enhancements."},

    {{ PACKTYPE_INT,    SIZE_INT32,    "InstanceBought",        OFFSET(DBPower, instanceBought) },
        "Instance this power was bought in"},

    {{ PACKTYPE_INT,    SIZE_INT32, "UniqueID",                OFFSET(DBPower, iUniqueID) },
        "Identifier for this power, unique per Character."},

    // fin
    { 0 },
};
// change this when adding new powerup slots
STATIC_ASSERT(POWERUP_COST_SLOTS_MAX_COUNT == 10);

StructDesc power_desc[] =
{
    sizeof(DBPower), {AT_STRUCT_ARRAY, OFFSET(DBPowers, powers)}, power_line_desc,
    "Each power a character owns has a row in the Powers table. Therefore, there will likely be multiple rows in the table for each character."
};

// Boosts

LineDesc boost_line_desc[] =
{
    {{ PACKTYPE_INT,       SIZE_INT32,   "PowerID",      OFFSET(DBBoost,iID),                       },
        "The ID of the power in the Powers table specifying where this boost is slotted."},

    {{ PACKTYPE_INT,       SIZE_INT32,   "Idx",          OFFSET(DBBoost,iIdx),                      },
        "The index of the slot where this boost is slotted, or -1 if the boost is in the character's inventory."},

    {{ PACKTYPE_ATTR,      MAX_ATTRIBNAME_LEN, "CategoryName", OFFSET(DBBoost,dbpowBase.pchCategoryName), },
        "The boost is identified by the CategoryName, the PowerSetName within the category, and the BoostName within the power set."},
    {{ PACKTYPE_ATTR,      MAX_ATTRIBNAME_LEN, "PowerSetName", OFFSET(DBBoost,dbpowBase.pchPowerSetName), },
        "The boost is identified by the CategoryName, the PowerSetName within the category, and the BoostName within the power set."},
    {{ PACKTYPE_ATTR,      MAX_ATTRIBNAME_LEN, "BoostName",    OFFSET(DBBoost,dbpowBase.pchPowerName),    },
        "The boost is identified by the CategoryName, the PowerSetName within the category, and the BoostName within the power set."},

    {{ PACKTYPE_INT,       SIZE_INT32,   "Level",        OFFSET(DBBoost,iLevel),                    },
        "The 0-based level of the boost"},

    {{ PACKTYPE_INT,       SIZE_INT32,   "NumCombines",  OFFSET(DBBoost,iNumCombines),              },
        "The number of times the boost has been combined with another."},

    {{ PACKTYPE_FLOAT,             SIZE_FLOAT32,   "Var0",         OFFSET(DBBoost,afVars[0]),                 },
        "Unused - (Invention system)"},
    {{ PACKTYPE_FLOAT,             SIZE_FLOAT32,   "Var1",         OFFSET(DBBoost,afVars[1]),                 },
        "Unused - (Invention system)"},
    {{ PACKTYPE_FLOAT,             SIZE_FLOAT32,   "Var2",         OFFSET(DBBoost,afVars[2]),                 },
        "Unused - (Invention system)"},
    {{ PACKTYPE_FLOAT,             SIZE_FLOAT32,   "Var3",         OFFSET(DBBoost,afVars[3]),                 },
        "Unused - (Invention system)"},

    // the powerup slots
    {{ PACKTYPE_INT,    SIZE_INT32,    "SlottedPowerupsMask",    OFFSET(DBBoost,iSlottedPowerupsMask), },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType0",        OFFSET(DBBoost, aPowerupSlots[0].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId0",        OFFSET(DBBoost, aPowerupSlots[0].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType1",        OFFSET(DBBoost, aPowerupSlots[1].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId1",        OFFSET(DBBoost, aPowerupSlots[1].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType2",        OFFSET(DBBoost, aPowerupSlots[2].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId2",        OFFSET(DBBoost, aPowerupSlots[2].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType3",        OFFSET(DBBoost, aPowerupSlots[3].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId3",        OFFSET(DBBoost, aPowerupSlots[3].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType4",        OFFSET(DBBoost, aPowerupSlots[4].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId4",        OFFSET(DBBoost, aPowerupSlots[4].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType5",        OFFSET(DBBoost, aPowerupSlots[5].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId5",        OFFSET(DBBoost, aPowerupSlots[5].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType6",        OFFSET(DBBoost, aPowerupSlots[6].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId6",        OFFSET(DBBoost, aPowerupSlots[6].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType7",        OFFSET(DBBoost, aPowerupSlots[7].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId7",        OFFSET(DBBoost, aPowerupSlots[7].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType8",        OFFSET(DBBoost, aPowerupSlots[8].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId8",        OFFSET(DBBoost, aPowerupSlots[8].id) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotType9",        OFFSET(DBBoost, aPowerupSlots[9].type) },
        "Unused - (Invention system)"},
    {{ PACKTYPE_INT,    SIZE_INT32,    "PowerupSlotId9",        OFFSET(DBBoost, aPowerupSlots[9].id) },
        "Unused - (Invention system)"},

    // fin
    { 0 },
};

// change this when adding new powerup slots
STATIC_ASSERT(POWERUP_COST_SLOTS_MAX_COUNT == 10);

StructDesc boost_desc[] =
{
    sizeof(DBBoost),
    {AT_STRUCT_ARRAY, OFFSET(DBPowers, boosts)},
    boost_line_desc,

    "Each boost (enhancement) a character owns has a row in the Boosts table. Therefore, there will likely be multiple rows in the table for each character."
};

// Inspirations

LineDesc insp_line_desc[] =
{
    {{ PACKTYPE_INT,       SIZE_INT32,   "Col",          OFFSET(DBInspiration,iCol),                      },
        "The column the inspiration is at in the inspiration window."},
    {{ PACKTYPE_INT,       SIZE_INT32,   "Row",          OFFSET(DBInspiration,iRow),                      },
        "The row the inspiration is at in the inspiration window."},

    {{ PACKTYPE_ATTR,      MAX_ATTRIBNAME_LEN, "CategoryName", OFFSET(DBInspiration,dbpowBase.pchCategoryName), },
        "The inspiration is identified by the CategoryName, the PowerSetName within the category, and the PowerName within the power set."},
    {{ PACKTYPE_ATTR,      MAX_ATTRIBNAME_LEN, "PowerSetName", OFFSET(DBInspiration,dbpowBase.pchPowerSetName), },
        "The inspiration is identified by the CategoryName, the PowerSetName within the category, and the PowerName within the power set."},
    {{ PACKTYPE_ATTR,      MAX_ATTRIBNAME_LEN, "PowerName",    OFFSET(DBInspiration,dbpowBase.pchPowerName),    },
        "The inspiration is identified by the CategoryName, the PowerSetName within the category, and the PowerName within the power set."},

    { 0 },
};

StructDesc insp_desc[] =
{
    sizeof(DBInspiration),
    {AT_STRUCT_ARRAY, OFFSET(DBPowers, inspirations)},
    insp_line_desc,

    "Each inspiration a character owns has a row in the Inspirations table. Therefore, there will likely be multiple rows in the table for each character."
};

// Attribmods

LineDesc attribmod_line_desc[] =
{
    {{ PACKTYPE_INT,  SIZE_INT32,   "Idx",          OFFSET(DBAttribMod,iIdxTemplate),              },
        "Specifies which part of the power this AttribMod is from."},

    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN, "CategoryName", OFFSET(DBAttribMod,dbpowBase.pchCategoryName), },
        "The power is identified by the CategoryName, the PowerSetName within the category, and the PowerName within the power set."},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN, "PowerSetName", OFFSET(DBAttribMod,dbpowBase.pchPowerSetName), },
        "The power is identified by the CategoryName, the PowerSetName within the category, and the PowerName within the power set."},
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN, "PowerName",    OFFSET(DBAttribMod,dbpowBase.pchPowerName),    },
        "The power is identified by the CategoryName, the PowerSetName within the category, and the PowerName within the power set."},

    {{ PACKTYPE_FLOAT,          SIZE_FLOAT32, "Duration",     OFFSET(DBAttribMod,fDuration),                 },
        "Seconds - how long the AttribMod affects the character overall"},

    {{ PACKTYPE_FLOAT,          SIZE_FLOAT32, "Magnitude",    OFFSET(DBAttribMod,fMagnitude),                },
        "How stringly the AttribMod affects the character."},

    {{ PACKTYPE_FLOAT,          SIZE_FLOAT32, "Timer",        OFFSET(DBAttribMod,fTimer),                    },
        "Seconds - how much longer the AttribMod will affect the character."},

    {{ PACKTYPE_FLOAT,          SIZE_FLOAT32, "Chance",        OFFSET(DBAttribMod,fChance),                    },
        "Percent - how likely it is that the attribmod will be applied."},

    {{ PACKTYPE_INT,        SIZE_INT32, "UiD",        OFFSET(DBAttribMod,UID),                    },
        "Unique ID"},

    {{ PACKTYPE_INT,        SIZE_INT32, "PetFlags",        OFFSET(DBAttribMod,petFlags),                    },
        "Unique ID"},

    {{ PACKTYPE_ATTR,    MAX_ATTRIBNAME_LEN,    "CustomFXToken",    OFFSET(DBAttribMod, customFXToken),            },
        "Custom FX Token to launch the effect when it zones."},

    {{    PACKTYPE_INT,        SIZE_INT32,    "PrimaryTint",    OFFSET(DBAttribMod, uiTint.primary.integer),    },
        "Primary Tint Color"},

    {{    PACKTYPE_INT,        SIZE_INT32,    "SecondaryTint",    OFFSET(DBAttribMod, uiTint.secondary.integer),    },
        "Secondary Tint Color"},

    {{    PACKTYPE_INT,    SIZE_INT8,    "SuppressedByStacking",    OFFSET(DBAttribMod, bSuppressedByStacking),    },
        "Is this mod suppressed by something that has kStackType_Suppress?" },

    { 0 },
};

StructDesc attribmod_desc[] =
{
    sizeof(DBAttribMod),
    {AT_STRUCT_ARRAY, OFFSET(DBPowers, attribmods)},
    attribmod_line_desc,

    "Every power which is currently affecting a character will put one or more AttribMods on them. Therefore, there will likely be multiple rows in the table for each character."

};

// The whole thing

LineDesc powers_line_desc[] =
{
    // Don't change the order or insert anything in here without checking
    // the packageEntPowers function, which evilly modifies it.
    { PACKTYPE_SUB, MAX_POWERS,             "Powers",       (intptr_t)power_desc     }, // don't change the order
    { PACKTYPE_SUB, MAX_DB_BOOSTS,       "Boosts",       (intptr_t)boost_desc     }, // don't change the order
    { PACKTYPE_SUB, MAX_DB_INSPIRATIONS, "Inspirations", (intptr_t)insp_desc      }, // don't change the order
    { PACKTYPE_SUB, MAX_DB_ATTRIB_MODS,  "AttribMods",   (intptr_t)attribmod_desc }, // don't change the order
    { 0 },
};

StructDesc powers_desc[] =
{
    sizeof(DBPowers),
    {AT_STRUCT_ARRAY,{0}},
    powers_line_desc
};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

LineDesc stat_line_desc[] =
{
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN, "Name"               , OFFSET(DBStat, pchItem),     },
        "The name of the statistic"},

    {{ PACKTYPE_INT,  SIZE_INT32,   "General_Today"      , OFFSET(DBStat, iValues[kStat_General]  [kStatPeriod_Today]    ) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "General_Yesterday"  , OFFSET(DBStat, iValues[kStat_General]  [kStatPeriod_Yesterday]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "General_ThisMonth"  , OFFSET(DBStat, iValues[kStat_General]  [kStatPeriod_ThisMonth]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "General_LastMonth"  , OFFSET(DBStat, iValues[kStat_General]  [kStatPeriod_LastMonth]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Kills_Today"        , OFFSET(DBStat, iValues[kStat_Kills]    [kStatPeriod_Today]    ) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Kills_Yesterday"    , OFFSET(DBStat, iValues[kStat_Kills]    [kStatPeriod_Yesterday]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Kills_ThisMonth"    , OFFSET(DBStat, iValues[kStat_Kills]    [kStatPeriod_ThisMonth]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Kills_LastMonth"    , OFFSET(DBStat, iValues[kStat_Kills]    [kStatPeriod_LastMonth]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Deaths_Today"       , OFFSET(DBStat, iValues[kStat_Deaths]   [kStatPeriod_Today]    ) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Deaths_Yesterday"   , OFFSET(DBStat, iValues[kStat_Deaths]   [kStatPeriod_Yesterday]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Deaths_ThisMonth"   , OFFSET(DBStat, iValues[kStat_Deaths]   [kStatPeriod_ThisMonth]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Deaths_LastMonth"   , OFFSET(DBStat, iValues[kStat_Deaths]   [kStatPeriod_LastMonth]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Time_Today"         , OFFSET(DBStat, iValues[kStat_Time]     [kStatPeriod_Today]    ) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Time_Yesterday"     , OFFSET(DBStat, iValues[kStat_Time]     [kStatPeriod_Yesterday]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Time_ThisMonth"     , OFFSET(DBStat, iValues[kStat_Time]     [kStatPeriod_ThisMonth]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Time_LastMonth"     , OFFSET(DBStat, iValues[kStat_Time]     [kStatPeriod_LastMonth]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "XP_Today"           , OFFSET(DBStat, iValues[kStat_XP]       [kStatPeriod_Today]    ) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "XP_Yesterday"       , OFFSET(DBStat, iValues[kStat_XP]       [kStatPeriod_Yesterday]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "XP_ThisMonth"       , OFFSET(DBStat, iValues[kStat_XP]       [kStatPeriod_ThisMonth]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "XP_LastMonth"       , OFFSET(DBStat, iValues[kStat_XP]       [kStatPeriod_LastMonth]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Influence_Today"    , OFFSET(DBStat, iValues[kStat_Influence][kStatPeriod_Today]    ) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Influence_Yesterday", OFFSET(DBStat, iValues[kStat_Influence][kStatPeriod_Yesterday]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Influence_ThisMonth", OFFSET(DBStat, iValues[kStat_Influence][kStatPeriod_ThisMonth]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Influence_LastMonth", OFFSET(DBStat, iValues[kStat_Influence][kStatPeriod_LastMonth]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Wisdom_Today"       , OFFSET(DBStat, iValues[kStat_Wisdom]   [kStatPeriod_Today]    ) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Wisdom_Yesterday"   , OFFSET(DBStat, iValues[kStat_Wisdom]   [kStatPeriod_Yesterday]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Wisdom_ThisMonth"   , OFFSET(DBStat, iValues[kStat_Wisdom]   [kStatPeriod_ThisMonth]) },
        "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Wisdom_LastMonth"   , OFFSET(DBStat, iValues[kStat_Wisdom]   [kStatPeriod_LastMonth]) },
        "The value of the statistic for a given aspect and period."},

    {{ PACKTYPE_INT,  SIZE_INT32,   "Architect_XP_Today"           , OFFSET(DBStat, iValues[kStat_ArchitectXP]       [kStatPeriod_Today]    ) },
    "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Architect_XP_Yesterday"       , OFFSET(DBStat, iValues[kStat_ArchitectXP]       [kStatPeriod_Yesterday]) },
    "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Architect_XP_ThisMonth"       , OFFSET(DBStat, iValues[kStat_ArchitectXP]       [kStatPeriod_ThisMonth]) },
    "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Architect_XP_LastMonth"       , OFFSET(DBStat, iValues[kStat_ArchitectXP]       [kStatPeriod_LastMonth]) },
    "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Architect_Influence_Today"    , OFFSET(DBStat, iValues[kStat_ArchitectInfluence][kStatPeriod_Today]    ) },
    "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Architect_Influence_Yesterday", OFFSET(DBStat, iValues[kStat_ArchitectInfluence][kStatPeriod_Yesterday]) },
    "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Architect_Influence_ThisMonth", OFFSET(DBStat, iValues[kStat_ArchitectInfluence][kStatPeriod_ThisMonth]) },
    "The value of the statistic for a given aspect and period."},
    {{ PACKTYPE_INT,  SIZE_INT32,   "Architect_Influence_LastMonth", OFFSET(DBStat, iValues[kStat_ArchitectInfluence][kStatPeriod_LastMonth]) },
    "The value of the statistic for a given aspect and period."},

    { 0 },
};

StructDesc stat_desc[] =
{
    sizeof(DBStat),
    {AT_STRUCT_ARRAY, OFFSET(DBStats, stats)},
    stat_line_desc,

    "The Stats table keeps records of various statistics which roll over and "
    "expire periodically. These are the statistics which are used to show "
    "the Top Ten lists on the kiosks.<br>"
    "Each row in the table records all the periodic information for all "
    "aspects of a given statistic. There will likely be several rows for each character.<br>"
    "Periods:<br>"
    "   Today      = The value for the last calendar day the character was logged in.<br>"
    "   Yesterday  = The value for the day before the last calendar day the character was logged in.<br>"
    "   This_Month = The cumulative value for all the days in the month that the character was last logged in.<br>"
    "   Last_Month = The cumulative value for all the days in the previous month that the character was last logged in.<br>"
    "Aspects:<br>"
    "   General    = General and overall value<br>"
    "   Kills      = Number of kills<br>"
    "   Deaths     = Number of deaths<br>"
    "   Time       = Time spent<br>"
    "   XP         = Experience earned<br>"
    "   Influence  = Influence earned<br>"
    "   Wisdom     = Wisdom earned<br>"
};

LineDesc stats_line_desc[] =
{
    { PACKTYPE_SUB, MAX_DB_STATS,  "Stats", (intptr_t)stat_desc },
    { 0 },
};

StructDesc stats_desc[] =
{
    sizeof(DBStats),
    {AT_STRUCT_ARRAY,{0}},
    stats_line_desc
};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

typedef struct ShardAccountInventoryItem {
    SkuId sku_id;
    int granted;
    int claimed;
    U32 expires;
    char recipe[128];
} ShardAccountInventoryItem;

typedef struct ShardAccount
{
    int slot_count;
    char account[32];
    int was_vip;
    int inventorySize;
    int showPremiumSlotLockNag;
    ShardAccountInventoryItem    **inventory;
    U8 loyaltyStatus[LOYALTY_BITS/8];
    int loyaltyPointsUnspent;
    int loyaltyPointsEarned;
    U32    accountStatusFlags;    // see AccountStatusFlags
} ShardAccount;


static LineDesc inventory_item_line_desc[] =
{
    {{ PACKTYPE_INT,  SIZE_INT32, "SkuId0", OFFSET(ShardAccountInventoryItem, sku_id.u32[0]) },
    "The SKU of the inventory item"},
    {{ PACKTYPE_INT,  SIZE_INT32, "SkuId1", OFFSET(ShardAccountInventoryItem, sku_id.u32[1]) },
    "The SKU of the inventory item"},

    {{ PACKTYPE_INT,  SIZE_INT32, "Granted", OFFSET(ShardAccountInventoryItem, granted) },
    "The count of the inventory item"},
    {{ PACKTYPE_INT,  SIZE_INT32, "Claimed", OFFSET(ShardAccountInventoryItem, claimed) },
    "The count claimed of the inventory item"},
    {{ PACKTYPE_INT,  SIZE_INT32, "Expires", OFFSET(ShardAccountInventoryItem, expires) },
    "The date the inventory item expires"},

    { 0 },
};

StructDesc account_inventory_desc[] =
{
    sizeof(ShardAccountInventoryItem),
    {AT_EARRAY, OFFSET(ShardAccount, inventory )},
    inventory_item_line_desc,
    "AccountInventory table is a list of all the account inventory items.<br>"
    "Each row represents a inventory item.<br>"
    "Each account can have many inventory items."
};

LineDesc shardaccount_line_desc[] =
{
    {{ PACKTYPE_INT, SIZE_INT32, "SlotCount", OFFSET(ShardAccount, slot_count) },
        "The number of additional slots that an account has earned on a given shard."},
    {{ PACKTYPE_STR_ASCII, SIZEOF2(ShardAccount, account), "AuthName", OFFSET(ShardAccount, account), INOUT(0,0), LINEDESCFLAG_INDEXEDCOLUMN },
        "Account name (from the auth server) for the owner of the character"},
    {{ PACKTYPE_INT, SIZE_INT32, "WasVIP", OFFSET(ShardAccount, was_vip) },
        "The previous VIP status. Used at login to check for VIP to free transitions (lock all characters on shard)."},

    {{ PACKTYPE_INT, SIZE_INT32, "LoyaltyStatus0", OFFSET(ShardAccount, loyaltyStatus[0]) },
        "Account loyalty status - part 1"},
    {{ PACKTYPE_INT, SIZE_INT32, "LoyaltyStatus1", OFFSET(ShardAccount, loyaltyStatus[4]) },
        "Account loyalty status - part 2"},
    {{ PACKTYPE_INT, SIZE_INT32, "LoyaltyStatus2", OFFSET(ShardAccount, loyaltyStatus[8]) },
        "Account loyalty status - part 3"},
    {{ PACKTYPE_INT, SIZE_INT32, "LoyaltyStatus3", OFFSET(ShardAccount, loyaltyStatus[12]) },
        "Account loyalty status - part 4"},

    {{ PACKTYPE_INT, SIZE_INT32, "LoyaltyPointsUnspent", OFFSET(ShardAccount, loyaltyPointsUnspent) },
        "Loyalty Points Unspent on this account"},
    {{ PACKTYPE_INT, SIZE_INT32, "LoyaltyPointsEarned", OFFSET(ShardAccount, loyaltyPointsEarned) },
        "Loyalty Points Earned on this account"},
    {{ PACKTYPE_INT, SIZE_INT32, "AccountStatusFlags", OFFSET(ShardAccount, accountStatusFlags) },
        "Account Status Flags on this account"},

    {{ PACKTYPE_INT, SIZE_INT32, "AccountInventorySize", OFFSET(ShardAccount, inventorySize) },
        "Max number of account inventory items"},

    {{ PACKTYPE_INT, SIZE_INT32, "ShowPremiumSlotLockNag", OFFSET(ShardAccount, inventorySize) },
        "Whether or not the player could need to have VIP -> Premium slot locking explained"},

    {{ PACKTYPE_EARRAY,    (intptr_t)rewardtoken_Create,            "AccountInventory",            (intptr_t)account_inventory_desc        },
        "Account inventory items"},

    { 0 },
};

StructDesc shardaccount_desc[] =
{
    sizeof(ShardAccount),
    {AT_NOT_ARRAY,{0}},
    shardaccount_line_desc,
    "Info about a player account on a given shard."
};


/////////////////////////////////////////////////////////////////////////////////
// SuperGroup data block
/////////////////////////////////////////////////////////////////////////////////


static StashTable    common_vars_hash;
static char         **common_vars;
static int          common_vars_count,common_vars_max;

char *entTemplate()
{
    StuffBuff   sb;
    int         i;

    initStuffBuff( &sb, 30000 );
    for(i=0;ent_line_desc[i].type;i++)
        structToLineTemplate(&sb,0,&ent_line_desc[i],1);
    for(i=0;powers_line_desc[i].type;i++)
        structToLineTemplate(&sb,0,&powers_line_desc[i],1);
    for(i=0;stats_line_desc[i].type;i++)
        structToLineTemplate(&sb,0,&stats_line_desc[i],1);
    for(i=0;costumes_line_desc[i].type;i++)
        structToLineTemplate(&sb,0,&costumes_line_desc[i],1);
    for(i=0;appearances_line_desc[i].type;i++)
        structToLineTemplate(&sb,0,&appearances_line_desc[i],1);
    for(i=0;powercustomization_list_line_desc[i].type;i++)
        structToLineTemplate(&sb,0,&powercustomization_list_line_desc[i],1);

    badge_Template(&sb);

    // and the inventory
    inventory_Template(&sb);

    return sb.buff;
}

char *entSchema()
{
    StuffBuff   sb;
    int         i;

    initStuffBuff( &sb, 30000 );
    addStringToStuffBuff(&sb, "<html><body><table cellspacing=0 cellpadding=3 >");
    for(i=0;ent_schema_desc[i].type;i++)
        structToLineSchema(&sb,0,&ent_schema_desc[i],1);
    for(i=0;powers_line_desc[i].type;i++)
        structToLineSchema(&sb,0,&powers_line_desc[i],1);
    for(i=0;stats_line_desc[i].type;i++)
        structToLineSchema(&sb,0,&stats_line_desc[i],1);
    for(i=0;costumes_line_desc[i].type;i++)
        structToLineSchema(&sb,0,&costumes_line_desc[i],1);
    for(i=0;appearances_line_desc[i].type;i++)
        structToLineSchema(&sb,0,&appearances_line_desc[i],1);
    for(i=0;powercustomization_list_line_desc[i].type;i++)
        structToLineSchema(&sb,0,&powercustomization_list_line_desc[i],1);
    addStringToStuffBuff(&sb, "</table></body></html>");

    return sb.buff;
}

typedef struct
{
    int     attr_id;
    char    str[256];
} VarEntry;

static int __cdecl cmpVarEntry(const VarEntry *a,const VarEntry *b)
{
    return a->attr_id - b->attr_id;
}

static void setMaxCostumePartCount(int count)
{
    int i;
    for(i=0;ent_line_desc[i].type;i++)
    {
        if(0 == stricmp(ent_line_desc[i].name, "CostumeParts"))
        {
            ent_line_desc[i].size = GetBodyPartCount();
            return;
        }
    }
}

void fixSuperGroupMode(Entity* e)
{
    if(e && e->pl)
    {
        sgroup_SetMode(e, e->pl->supergroup_mode, 1);
    }
}


void fixSuperGroupFields(Entity* e)
{
    if (e && e->supergroup)
    {

        e->supergroup->tithe = sgroup_TitheClamp(e->supergroup->tithe);
        e->supergroup->demoteTimeout = sgroup_DemoteClamp(e->supergroup->demoteTimeout);

    }
}

static void fixCostume(Entity* e, int clear_supergroup )
{
     if(e->pl->npc_costume && e->access_level && isProductionMode() )
    {
        e->npcIndex = e->pl->npc_costume;
    }
    else
    {
        e->npcIndex = e->pl->npc_costume = 0;
        e->costumePriority = 0;
    }

    if(e && e->pl && e->pl->costume && e->pl->superCostume)
    {
        memcpy(&e->pl->superCostume->appearance, &e->pl->costume[0]->appearance, sizeof(Appearance));
    }
}

static void fixMisc(Entity *e)
{
    //fix chat bubble color for previously existing users
    if( !e->pl->chatBubbleBackColor && !e->pl->chatBubbleTextColor )
        e->pl->chatBubbleBackColor = -1;

    if(e->pl->titleThe) // will only happen for english speakers
    {
        strcpy(e->pl->titleTheText, "The"); // database stores pre-translated string
        e->pl->titleThe = 0;
    }

    if( !e->pl->tooltipDelay )
        e->pl->tooltipDelay = .6f;

    //always force locale to english
    e->playerLocale = LOCALE_ID_ENGLISH;
}

static void fixSpecialAttribs(Entity *e)
{
    AttribMod *pmod;
    AttribModListIter iter;

    if (!e || !e->pchar) return;

    pmod = modlist_GetFirstMod(&iter, &e->pchar->listMod);

    while (pmod)
    {
        if (pmod->fTimer <= 0.0f &&
            (pmod->ptemplate->offAttrib == kSpecialAttrib_SetMode
            || pmod->ptemplate->offAttrib == kSpecialAttrib_SetCostume))
        {
            HandleSpecialAttrib(pmod, e->pchar, 0.0, NULL);
        }

        pmod = modlist_GetNextMod(&iter);
    }
}

static void growStoryInfoArrays(StoryInfo* info, int alloc_mem)
{
    int origSize;
    int i;

    origSize = eaSize(&info->contactInfos);
    eaSetSize(&info->contactInfos, MAX(CONTACTS_PER_PLAYER, CONTACTS_PER_TASKFORCE));
    for(i = origSize; i < eaSize(&info->contactInfos); i++)
    {
        info->contactInfos[i] = alloc_mem?storyContactInfoAlloc():NULL;
    }

    origSize = eaSize(&info->storyArcs);
    eaSetSize(&info->storyArcs, MAX(STORYARCS_PER_PLAYER, STORYARCS_PER_TASKFORCE));
    for(i = 0; i < eaSize(&info->storyArcs); i++)
    {
        if( i < origSize ) // this sets the placeholder ID just before saving to DB
        {
             if( info->storyArcs[i] && info->storyArcs[i]->sahandle.bPlayerCreated    )
                info->storyArcs[i]->playerCreatedID = info->storyArcs[i]->sahandle.context;
        }
        else
            info->storyArcs[i] = alloc_mem?storyArcInfoAlloc():NULL;
    }

    origSize = eaSize(&info->tasks);
    eaSetSize(&info->tasks, MAX(TASKS_PER_PLAYER, TASKS_PER_TASKFORCE));
    for(i = origSize; i < eaSize(&info->tasks); i++)
    {
        info->tasks[i] = alloc_mem?storyTaskInfoAlloc():NULL;
    }

    // MAK - because the way we store souvenir clues is insane, we have to empty array first
    // and prevent the structToLine code from overwriting shared memory
    if (alloc_mem) 
        eaSetSizeConst(&info->souvenirClues, 0);
    origSize = eaSize(&info->souvenirClues);
    eaSetSizeConst(&info->souvenirClues, SOUVENIRCLUES_PER_PLAYER);
    for(i = origSize; i < eaSize(&info->souvenirClues); i++)
    {
        SouvenirClue* clues = alloc_mem ? calloc(1, sizeof(SouvenirClue)) : NULL;
        if (alloc_mem)
            clues->uid = -1; // Dummy souvenir clue entiries has UID: -1
        info->souvenirClues[i] = clues;
    }
}

static void shrinkStoryInfoArrays(StoryInfo* info, int assignedDbId)
{
    int i;

    // Clean up the contacts array.
    //    Compact the array by removing all invalid elements.
    //    Note that this also cleans up any previous entries that has become invalid.
    //    This happens when an existing contact definition is renamed or deleted.
    for(i = eaSize(&info->contactInfos)-1; i >= 0; i--)
    {
        int ok = info->contactInfos[i] && info->contactInfos[i]->handle && storyContactInfoInit(info->contactInfos[i]);
        if (!ok)
        {
            // Either the entry is emtpy or the entry was not reconstructed correctly.
            // Reconstruction would fail if a contact was removed from the data files, for example.
            if (info->contactInfos[i])
                storyContactInfoFree(info->contactInfos[i]);
            eaRemove(&info->contactInfos, i);
        }
    }

    // Clean up the storyarcs array.
    for(i = eaSize(&info->storyArcs)-1; i >= 0; i--)
    {
        int ok;
        if(info->storyArcs[i] && info->storyArcs[i]->playerCreatedID )
        {
            info->storyArcs[i]->sahandle.context = info->storyArcs[i]->playerCreatedID;
            info->storyArcs[i]->sahandle.bPlayerCreated = 1;
        }

        ok = info->storyArcs[i] && info->storyArcs[i]->sahandle.context && info->storyArcs[i]->contact && storyArcInfoInit(info->storyArcs[i]) && ContactGetInfo(info, info->storyArcs[i]->contact);
        if (!ok)
        {
            if (info->storyArcs[i])
            {
                // Attempt to cleanly remove this story arc from the contact which may have issued it so player can get it again
                // see RemoveStoryArc
                if(info->storyArcs[i] && info->storyArcs[i]->sahandle.context && info->storyArcs[i]->contact)
                {
                    StoryContactInfo *badArcContact = ContactGetInfo(info, info->storyArcs[i]->contact);
                    if(badArcContact)
                    {
                        int j, numRefs = eaSize(&badArcContact->contact->def->storyarcrefs);
                        for (j = 0; j < numRefs; j++)
                        {
                            if (badArcContact->contact->def->storyarcrefs[j]->sahandle.context == info->storyArcs[i]->sahandle.context &&
                                badArcContact->contact->def->storyarcrefs[j]->sahandle.bPlayerCreated == info->storyArcs[i]->sahandle.bPlayerCreated )
                            {
                                BitFieldSet(badArcContact->storyArcIssued, STORYARC_BITFIELD_SIZE, j, 0);
                            }
                        }
                    }
                }
                storyArcInfoDestroy(info->storyArcs[i]);
            }
            eaRemove(&info->storyArcs, i);
        }
    }

    // Clean up the tasks array.
    for(i = eaSize(&info->tasks)-1; i >= 0; i--)
    {
        int ok = info->tasks[i] && info->tasks[i]->sahandle.context && storyTaskInfoInit(info->tasks[i]) && TaskGetContact(info, info->tasks[i]) && !TaskIsDuplicate(info, i);
        if (ok)
        {
            // MAK - this hack is to restore assigned dbid's when a character gets copied
            // from one server to another
            if (assignedDbId > 0)
                info->tasks[i]->assignedDbId = assignedDbId;
        }
        else
        {
            if (info->tasks[i])
            {
                int j;
                for(j = eaSize(&info->storyArcs)-1; j >= 0; j--)
                {
                    // If we are removing this task as invalid, we may have put the storyarc in a bad state
                    // Clear the taskComplete & taskIssued field so it can recover
                    if(info->tasks[i]->sahandle.context == info->storyArcs[j]->sahandle.context)
                    {
                        BitFieldClear(info->storyArcs[j]->taskComplete, EP_TASK_BITFIELD_SIZE);
                        BitFieldClear(info->storyArcs[j]->taskIssued, EP_TASK_BITFIELD_SIZE);
                    }
                }
                storyTaskInfoFree(info->tasks[i]);
            }
            eaRemove(&info->tasks, i);
        }
    }

    // Clean up the souvenir clues array.
    for(i = eaSize(&info->souvenirClues)-1; i >= 0; i--)
    {
        int ok = info->souvenirClues[i] && (info->souvenirClues[i]->uid != -1);
        if (!ok)
        {
            if(info->souvenirClues[i])
                freeConst(info->souvenirClues[i]);
            eaRemoveConst(&info->souvenirClues, i);
        }
    }
}

static void fixupClueReferences(StoryInfo* info)
{
    int i;

    // Go through all clues referenced by all tasks and
    // remove all clue entries that are invalid.
    for(i = eaSize(&info->tasks)-1; i >= 0; i--)
    {
        StoryTaskInfo* taskState = info->tasks[i];
        const StoryClue** clues = TaskGetClues(&taskState->sahandle);
        U32* clueStates = TaskGetClueStates(info, taskState);
        int clueCount = eaSize(&clues);
        int clueIndex;

        if (!clues || !clueStates)
        {
            BitFieldClear(taskState->haveClues, CLUE_BITFIELD_SIZE);
            LOG_OLD_ERR("fixupClueReferences: couldn't get task and had to ignore clues");
            continue;
        }

        // Iterate over all clues for the task.
        for(clueIndex = BitFieldGetFirst(clueStates, CLUE_BITFIELD_SIZE);
            clueIndex != -1;
            clueIndex = BitFieldGetNext(clueStates, CLUE_BITFIELD_SIZE, clueIndex))
        {
            // Remove all clues that are invalid.
            if(clueIndex >= clueCount)
            {
                BitFieldSet(clueStates, CLUE_BITFIELD_SIZE, clueIndex, 0);
            }
        }
    }

    for(i = eaSize(&info->storyArcs)-1; i >= 0; i--)
    {
        StoryArcInfo* storyArc = info->storyArcs[i];
        StoryClue** clues = storyArc->def->clues;
        U32* clueStates = storyArc->haveClues;

        int clueCount = eaSize(&clues);
        int clueIndex;

        // Iterate over all clues for the task.
        for(clueIndex = BitFieldGetFirst(clueStates, CLUE_BITFIELD_SIZE);
            clueIndex != -1;
            clueIndex = BitFieldGetNext(clueStates, CLUE_BITFIELD_SIZE, clueIndex))
        {
            // Remove all clues that are invalid.
            if(clueIndex >= clueCount)
            {
                BitFieldSet(clueStates, CLUE_BITFIELD_SIZE, clueIndex, 0);
            }
        }
    }
}

static void fixupSouvenirClueReferences(StoryInfo* info)
{
    int i;
    for(i = eaSize(&info->souvenirClues)-1; i >= 0; i--)
    {
        const SouvenirClue* clue = info->souvenirClues[i];

        // If we're looking at a dummy entry, but it has a name, this entries was retrieved from the database.
        if(clue->uid == -1 && clue->name)
        {
            // Find the clue definition referenced by the name.
            clue = scFindByName(clue->name);

            // If the clue does not exist, leave it alone.
            // Dummy souvenir clue entiries will be cleaned up by shrinkStoryInfoArrays
            if(!clue)
                continue;

            // Replace the dummy clue entry with the real clue definition.
            freeConst(info->souvenirClues[i]);
            info->souvenirClues[i] = clue; // info is mutable, list is mutable, clue is not
        }
    }
}

static void fixupMissionDoors(StoryInfo* info, int isVillain)
{
    int i, n=0;

    if( !dbGetTrackedDoors(&n) || !n ) // this is pointless with no door list
        return;

    n = eaSize(&info->tasks);
    for (i = 0; i < n; i++)
    {
        StoryTaskInfo* task = info->tasks[i];
        if (TaskIsMission(task) && task->doorMapId )
        {
            DoorEntry* door = dbFindGlobalMapObj(task->doorMapId, task->doorPos);
            int alliance = storyTaskInfoGetAlliance(task);
            const MapSpec *mapSpec = MapSpecFromMapId(task->doorMapId);

            if (door && door->type == DOORTYPE_MISSIONLOCATION 
                && staticMapInfo_IsZoneAllowedToHaveMission(mapSpec, alliance)) 
            {
                task->doorId = door->db_id;
            }
            else // couldn't get my door back?
            {
                char buf[2000];
                sprintf(buf, "fixing door for task %s: %i (%0.2f,%0.2f,%0.2f) ",
                    task->def->logicalname,    task->doorMapId, task->doorPos[0], task->doorPos[1], task->doorPos[2]);

                AssignMissionDoor(info, task, isVillain);
                door = dbFindGlobalMapObj(task->doorMapId, task->doorPos);

                strcatf(buf, " -> %i (%0.2f,%0.2f,%0.2f)\n", task->doorMapId, task->doorPos[0], task->doorPos[1], task->doorPos[2]);
                verbose_printf("%s", buf);
            }
        }
    }
}

static void fixupTeamDoor(Entity* e)
{
    if (e->teamup && e->teamup->activetask->doorMapId)
    {
        DoorEntry* door = dbFindGlobalMapObj(e->teamup->activetask->doorMapId, e->teamup->activetask->doorPos);
        if (door) e->teamup->activetask->doorId = door->db_id;
    }
}

void updateExpireables(Entity *e)
{
    if(e->pl)
    {
        if(e->pl->titleSpecialExpires>0
            && e->pl->titleSpecialExpires < dbSecondsSince2000())
        {
            e->pl->titleSpecial[0]='\0';
            e->pl->titleSpecialExpires = 0;
            e->send_costume = 1;
        }
        if(e->pl->costumeFxSpecialExpires>0
            && e->pl->costumeFxSpecialExpires < dbSecondsSince2000())
        {
            e->pl->costumeFxSpecial[0]='\0';
            e->pl->costumeFxSpecialExpires = 0;
            e->send_costume = 1;
        }
    }
}

typedef struct uiSetting
{
    StructIndirection indirection[MAX_INDIRECTIONS];
}uiSetting;


uiSetting uiSettings[] = {
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    mouse_invert)        }, // 0
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    options_saved)        }, // 1
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    fading_chat)        }, // 2
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    fading_nav)            }, // 3
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    titleThe)            }, // 4
    {OFFSET3_PTR2(Entity, pl,    EntPlayer,    tray,    Tray,        mode)                }, // 5
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!"
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    inspiration_mode)    }, // 6
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    hideFeePrompt)        }, // 7 was deprecated
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    first)                }, // 8
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    teambuff_display)    }, // 9
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    allowProfanity)        }, // 10
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    supergroup_mode)    }, // 11
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!"
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    hideUsefulSalvageWarning)    }, // 12 was deprecated
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    showToolTips),        }, // 13
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    showBalloon),        }, // 14
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    forcedPopHelpDisable),        }, // 15 was deprecated
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    gmailFriendOnly),    }, // 16 was deprecated
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    declineGifts),        }, // 17
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!"
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    declineTeamGifts),    }, // 18
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    promptTeamTeleport),}, // 19
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    capesUnlocked),        }, // 20
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    glowieUnlocked),    }, // 21
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    showPets),            }, // 22
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    webHideBasics),        }, // 23
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!"
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    webHideStats),        }, // 24
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    webHidePowers),        }, // 25
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    windowFade),        }, // 26
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    logChat),            }, // 27
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    attachArticle),        }, // 28
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    clicktomove),        }, // 29
    {OFFSET3_PTR2(Entity, pl,    EntPlayer,    tray,    Tray,        mode_alt2)            }, // 30
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    deprecated),        }, // 31 deprecated. if you reclaim this, you have to update the deprecated array in unpackUISettings
    // FULL!! Add new fields to uiSettings4
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!"
};

// DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!"
uiSetting uiSettings2[] = {
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    deprecated)                    }, // 0 deprecated. if you reclaim this, you have to update the deprecated array in unpackUISettings
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    deprecated),                }, // 1 deprecated. if you reclaim this, you have to update the deprecated array in unpackUISettings
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    disableDrag),                }, // 2
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    showPetBuffs),                }, // 3
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    disablePetSay),                }, // 4
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    enableTeamPetSay),            }, // 5
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!"
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    disablePetNames),            }, // 6
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    showSalvage),                }, // 7
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    hidePlacePrompt),            }, // 8
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    hideDeletePrompt),            }, // 9
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    preventPetIconDrag),        }, // 10
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    showPetControls),            }, // 11
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!"
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    advancedPetControls),        }, // 12
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    hide_supergroup_emblem),    }, // 13
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    showEnemyTells),            }, // 14
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    showEnemyBroadcast),        }, // 15
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    hideEnemyLocal),            }, // 16
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    fading_chat1),                }, // 17
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!"
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    fading_chat2),                }, // 18
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    fading_chat3),                }, // 19
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    fading_chat4),                }, // 20
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    freeCamera),                }, // 21
    {OFFSET2_PTR(Entity, pl,    EntPlayer,  helpChatAdded),                }, // 22
    {OFFSET2_PTR(Entity, pl,    EntPlayer,  hideDeleteSalvagePrompt),    }, // 23
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!"
    {OFFSET2_PTR(Entity, pl,    EntPlayer,  declineSuperGroupInvite),    }, // 24
    {OFFSET2_PTR(Entity, pl,    EntPlayer,  declineTradeInvite),        }, // 25
    {OFFSET2_PTR(Entity, pl,    EntPlayer,  hideDeleteRecipePrompt),    }, // 26
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    webHideBadges),                }, // 27
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    webHideFriends),            }, // 28
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    hideCoopPrompt),            }, // 29
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    is_a_spammer),                }, // 30
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    deprecated),                }, // 31 deprecated. if you reclaim this, you have to update the deprecated array in unpackUISettings
    // FULL!! Add new fields to uiSettings4
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!"
};

// DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!
uiSetting uiSettings3[] = {
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    deprecated)                        }, // 0 deprecated. if you reclaim this, you have to update the deprecated array in unpackUISettings
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    hideInspirationFull),            }, // 1
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    hideSalvageFull),                }, // 2
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    hideRecipeFull),                }, // 3
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    hideEnhancementFull),            }, // 4
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    contactSortByName),                }, // 5
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    contactSortByZone),                }, // 6
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    contactSortByRelationship),        }, // 7
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    contactSortByActive),            }, // 8
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    recipeHideUnOwned),                }, // 9
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    recipeHideMissingParts),        }, // 10
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    recipeHideUnOwnedBench),        }, // 11
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    recipeHideMissingPartsBench),    }, // 12
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    staticColorsPerName),            }, // 13
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    reverseMouseButtons),            }, // 14
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    disableCameraShake),            }, // 15
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    disableMouseScroll),            }, // 16
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    logPrivateMessages),            }, // 17
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    disableLoadingTips),            }, // 18
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    enableJoystick),                }, // 19
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    fading_tray),                    }, // 20
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    multiBuildsSetUp),                }, // 21
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    ArchitectNav)                    }, // 22
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    ArchitectTips)                    }, // 23
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    ArchitectAutoSave)                }, // 24
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    noXP)                            }, // 25
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    deprecated)                        }, // 26 Deprecated. if you reclaim this, you have to update the deprecated array in unpackUISettings
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    disableEmail)                    }, // 27
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    friendSgEmailOnly)                }, // 28
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    noXPExemplar)                    }, // 29
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    popHelpDisable)                    }, // 30
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    reclaim_deprecated)                }, // 31
    // FULL!! Add new fields to uiSettings4
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!
};

// DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!
uiSetting uiSettings4[] = {
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    useOldTeamUI)                 }, // 0
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    hideUnclaimableCert)          }, // 1
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    blinkCertifications)          }, // 2
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    voucherSingleCharacterPrompt) }, // 3
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    newCertificationPrompt)       }, // 4
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    hideUnslotPrompt)             }, // 5
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    hideUsefulSalvageWarning)     }, // 6
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    hideLoyaltyTreeAccessButton)  }, // 7
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    hideStoreAccessButton)        }, // 8
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    autoFlipSuperPackCards)       }, // 9
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    hideConvertConfirmPrompt)     }, // 10
    {OFFSET2_PTR(Entity, pl,    EntPlayer,    showTimer)                    }, // 11

 // 12
 // 13
 // 14
 // 15

 // 16
 // 17
 // 18
 // 19
 // 20

 // 21
 // 22
 // 23
 // 24
 // 25

 // 26
 // 27
 // 28
 // 29
 // 30
 // 31
    // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!
};
// NOTE: Currently there are 8 deprecated bits, at some point we can use a bit to recover X-1 of the deprecated bits
//       Not sure when the best time for that will be


// add new settings to end of list, each setting uses 3 bits, 10 fields max (10 used)
// DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!
uiSetting showSettings[] = {
    {OFFSET2_PTR(Entity, pl,    EntPlayer, eShowArchetype),            },
    {OFFSET2_PTR(Entity, pl,    EntPlayer, eShowSupergroup),        },
    {OFFSET2_PTR(Entity, pl,    EntPlayer, eShowPlayerName),        },
    {OFFSET2_PTR(Entity, pl,    EntPlayer, eShowPlayerBars),        },
    {OFFSET2_PTR(Entity, pl,    EntPlayer, eShowVillainName),        },
    {OFFSET2_PTR(Entity, pl,    EntPlayer, eShowVillainBars),        },
    {OFFSET2_PTR(Entity, pl,    EntPlayer, eShowPlayerReticles),    },
    {OFFSET2_PTR(Entity, pl,    EntPlayer, eShowVillainReticles),    },
    {OFFSET2_PTR(Entity, pl,    EntPlayer, eShowAssistReticles),    },
    {OFFSET2_PTR(Entity, pl,    EntPlayer, eShowOwnerName),            }, // this has always been only one bit, so I'm going to cram another field in on the end
    {OFFSET2_PTR(Entity, pl,    EntPlayer, eShowPlayerRating),        }, // 
}; // DO NOT ADD ANY MORE OPTIONS TO THIS BITFIELD!!!!!!!!!!!!!

void packageUISettings(Entity *e)
{
    int i, uifield = 0, uifield2 = 0, uifield3 = 0, uifield4 = 0, showfield = 0;
    int *iptr;

    e->pl->deprecated = 0; // reset all deprecated fields
    
    // uiSettings are all bool, 32 bits in integer cap
    for( i = MIN( 31, ARRAY_SIZE(uiSettings)-1 ); i >= 0; i-- )
    {
        iptr = siApplyMultipleIndirections((char *)e, uiSettings[i].indirection, MAX_INDIRECTIONS);
        if( (*iptr) > 0 ) // should never be greater than one, but force to one anyways to keep it from breaking bitfield
            uifield |= 1<<i;
    }
    
    // second bitfield for later
    for( i = MIN( 31, ARRAY_SIZE(uiSettings2)-1 ); i > 0; i-- ) // This is not >= because we broke the first field
    {
        iptr = siApplyMultipleIndirections((char *)e, uiSettings2[i].indirection, MAX_INDIRECTIONS);
        if( (*iptr) > 0 ) // should never be greater than one, but force to one anyways to keep it from breaking bitfield
            uifield2 |= 1<<i;
    }

    // third bitfield for ui Setttings
    for( i = MIN( 31, ARRAY_SIZE(uiSettings3)-1 ); i >= 0; i-- )
    {
        iptr = siApplyMultipleIndirections((char *)e, uiSettings3[i].indirection, MAX_INDIRECTIONS);
        if( (*iptr) > 0 ) // should never be greater than one, but force to one anyways to keep it from breaking bitfield
            uifield3 |= 1<<i;
    }

    // fourth bitfield for ui Setttings
    for( i = MIN( 31, ARRAY_SIZE(uiSettings4)-1 ); i >= 0; i-- )
    {
        iptr = siApplyMultipleIndirections((char *)e, uiSettings4[i].indirection, MAX_INDIRECTIONS);
        if( (*iptr) > 0 ) // should never be greater than one, but force to one anyways to keep it from breaking bitfield
            uifield4 |= 1<<i;
    }
    // show settings take 3 bits, 32/3 = 10.6, therefore 10 settings is most we can save
    for( i = 0; i < ARRAY_SIZE(showSettings); i++ )
    {
        iptr = siApplyMultipleIndirections((char *)e, showSettings[i].indirection, MAX_INDIRECTIONS);

        if( i == 10 ) // Evil hackery
            showfield |= (MIN(0x07,*iptr))<<((i*3)-1);
        else
        {
            if( (*iptr) > 0x07) // should never be larger than 7, but if it is for some reason, cap it so it won't interfere with rest of bitfield
                showfield |= 0x07<<(i*3);
            else
                showfield |= (*iptr)<<(i*3);
        }
    }
    
    e->pl->uiSettings = uifield;
    e->pl->uiSettings2 = uifield2;
    e->pl->uiSettings3 = uifield3;
    e->pl->uiSettings4 = uifield4;
    e->pl->showSettings = showfield;
}

void unpackUISettings(Entity *e)
{
    int i, val;
    int *iptr;
    int deprecated[4][32] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 1, },
                              { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 1, },
                              { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
                                0, 0, },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                                0, 0, }};
    
    
    for( i = MIN( 31, ARRAY_SIZE(uiSettings)-1); i >= 0; i-- )
    {
        val = ((e->pl->uiSettings>>i)&0x01);
        iptr = siApplyMultipleIndirections((char *)e, uiSettings[i].indirection, MAX_INDIRECTIONS);

        if( e->pl->reclaim_deprecated || !deprecated[0][i] )
            *iptr = val;
        else
            *iptr = 0;
    }
    
    for( i = MIN( 31, ARRAY_SIZE(uiSettings2)-1); i > 0; i-- )// This is not >= because we broke the first field
    {
        val = ((e->pl->uiSettings2>>i)&0x01);
        iptr = siApplyMultipleIndirections((char *)e, uiSettings2[i].indirection, MAX_INDIRECTIONS);
        *iptr = val;

        if( e->pl->reclaim_deprecated || !deprecated[1][i] )
            *iptr = val;
        else
            *iptr = 0;
    }

    for( i = MIN( 31, ARRAY_SIZE(uiSettings3)-1); i >= 0; i-- )
    {
        val = ((e->pl->uiSettings3>>i)&0x01);
        iptr = siApplyMultipleIndirections((char *)e, uiSettings3[i].indirection, MAX_INDIRECTIONS);
        *iptr = val;

        if( e->pl->reclaim_deprecated || !deprecated[2][i] )
            *iptr = val;
        else
            *iptr = 0;
    }

    for( i = MIN( 31, ARRAY_SIZE(uiSettings4)-1); i >= 0; i-- )
    {
        val = ((e->pl->uiSettings4>>i)&0x01);
        iptr = siApplyMultipleIndirections((char *)e, uiSettings4[i].indirection, MAX_INDIRECTIONS);
        *iptr = val;

        if( e->pl->reclaim_deprecated || !deprecated[2][i] )
            *iptr = val;
        else
            *iptr = 0;
    }

    e->pl->reclaim_deprecated = 1;

    for( i = 0; i < ARRAY_SIZE(showSettings); i++ )
    {
        if( i==10 )
            val = ( e->pl->showSettings>>((i*3)-1) )&0x07;
        else
            val = ( e->pl->showSettings>>(i*3) )&0x07;
        iptr = siApplyMultipleIndirections((char *)e, showSettings[i].indirection, MAX_INDIRECTIONS);
        *iptr = val;
    }
}

// Return 1 on success, 0 on normal failure, and -1 on FATAL failure
int validateEntPowersPackage(Entity *eOrig, char *buff)
{
    int idx;
    int bRet;
    DB_RestoreAttrib_Types eRestoreAttrib = eRAT_RestoreAll;
    char table[100],field[200],idxstr[MAX_BUF],*val;
    struct DBInv
    {
        DBGenericInventoryItem **genItems;
        DBConceptInventoryItem **conceptItems;
    } dbinv = {0};
    Entity *e;
    char   *bufforig = buff;
    
    e = playerCreate();
    strncpy(e->name,eOrig->name,MAX_NAME_LEN);
    e->db_id = eOrig->db_id;
    
    memset(&s_dbpows,            0, sizeof(DBPowers)    );
    
    growStoryInfoArrays(e->storyInfo, 1);
    
    while(decodeLine(&buff,table,field,&val,&idx,idxstr))
    {
        if (strnicmp(table,"var",3)==0)
        {
            
        }
        else if(strnicmp(table,"Powers",6)==0
                || strnicmp(table,"Boosts",6)==0
                || strnicmp(table,"Inspirations",12)==0
                || strnicmp(table,"AttribMods",10)==0)
        {
            lineToStruct((char *)&s_dbpows,powers_desc,table,field,val,idx);
        }
        else if(strnicmp(table,"Stats",5)==0)
        {
        }
        else if(strnicmp(table,"CostumeParts",12)==0)
        {
        }
        else if(strnicmp(table,"Appearance",10)==0)
        {
        }
        else if(strnicmp(table,"PowerCustomizations", 19) == 0)
        {
        }
        else if(strnicmp(table,"Badges",6)==0)
        {
        }
        else
        {
            lineToStruct((char *)e,ent_desc,table,field,val,idx);
        }
    }

    if (g_ArchitectTaskForce || e->db_flags & DBFLAG_CLEARATTRIBS)
        eRestoreAttrib = eRAT_ClearAbusiveBuffs;
            
    // don't restore attributes when entering arena or base PvP
    if (OnArenaMap() || RaidIsRunning() || server_visible_state.isPvP)
        eRestoreAttrib = eRAT_ClearAll;

    bRet = !unpackEntPowers(e, &s_dbpows, eRestoreAttrib, "Validate");
    if(!bRet)
    {
        LOG_OLD( "Unpacked for %d:\n{\n%s\n}\n", e->db_id, bufforig);
        LOG_OLD( "Validate logged for %d", e->db_id);
    }
    if (e->pchar->entParent == NULL || e->corrupted_dont_save)
    {
        // FATAL ERROR
        bRet = -1;
    }
    
    e->db_id = 0; // make sure not to free things relating to the real entity
    entFree(e);
    
    return bRet;
}

// void checkContainerBadges(Entity *e, char *container, char *ctxt)
// {
//     if( container )
//     {
//         int level = 0;
//         char badges[BADGE_BITFIELD_SIZE] = "";
//         if(containerGetField()
//         {
//         }
//     }
// }
extern bool checkContainerBadges_Internal(const char *container);
void checkContainerBadges(Entity *e, const char *container, const char *ctxt)
{
    if( container )
    {
        int l = 0;
        char *lvl = strstr(container,"\nLevel ");
        if( lvl && 1 == sscanf(lvl,"\nLevel %i", &l) && l >= 10 )
        {
            bool bad = !checkContainerBadges_Internal(container);
            if( bad )
            {
                static int savecontainer = 10;
                dbLog("CheckBadges",e,"%s: bad container level=%i, badges all zero. %s", ctxt, l, (savecontainer-- > 0) ? container : "<already printed>");
                devassertmsg(0,"bad badges container.");
            }
        }
    }
}


void checkEntBadges(Entity *e, char *ctxt)
{
    // uncomment if need badge checking again
//     static bool recurse = false;
//     if(recurse)
//         return;
//     recurse = true;
    
//     if( e && e->pl && e->pchar && e->pchar->iLevel >= 10 )
//     {
//         int i;
//         for( i = 0; i < ARRAY_SIZE( e->pl->aiBadgesOwned ); ++i ) 
//         {
//             if(e->pl->aiBadgesOwned[i])
//                 break;
//         }
//         if( i == ARRAY_SIZE( e->pl->aiBadgesOwned ))
//         {
//             static bool once = false;
//             once = true;
//             dbLog("CheckBadges",e,"%s: bad container level=%i, badges all zero.", ctxt, e->pchar->iLevel);
//             devassertmsg(0,"bad badges container.");
//         }
//     }
//     recurse = false;
}


static void s_logEntContainer(Entity *e, LogLevel logLevel, char *category, char *container)
{
    if( verify(e && container) )
    {
        int i;
        char *fieldVal;
        static char *fields[] =
            {
                "AuthUserData",
                "Badges[0].Owned"
            };

        for( i = 0; i < ARRAY_SIZE( fields ); ++i )
        {
            fieldVal = getContainerValueStatic( container, fields[i] );
            LOG_ENT( e, LOG_ENTITY, logLevel, 0, "%s %s %s", category, fields[i], fieldVal);
        }

    }
}


static char *s_packageEnt(Entity *e, int start_line_idx)
{
    int i;
    int costumeNPCOld, costumePriorityOld;
    const cCostume *pCostumeOld = NULL;
    U32 t,dt,last_time;
    StuffBuff   sb;

    if( !e || ENTTYPE(e) != ENTTYPE_PLAYER )
        return 0;

    // @testing -AB: see if we can detect corruption  :07/19/06
    checkEntBadges(e,__FUNCTION__);
    
    // save last static map position if appropriate
    if (!(e->pl->door_anim || e->dbcomm_state.in_map_xfer))
    {    // not in transfer step
        int mapType = getMapType();
        if (mapType == MAPTYPE_STATIC)
        {
            copyVec3(ENTPOS(e), e->pl->last_static_pos);
            getMat3YPR(ENTMAT(e), e->pl->last_static_pyr);
        }
        else
        { //Update the top of the stack if we're on that map
            MapHistory *hist = mapHistoryLast(e->pl);
            if (hist && hist->mapType == mapType && hist->mapID == getMapID())
            {
                copyVec3(ENTPOS(e), hist->last_pos);
                getMat3YPR(ENTMAT(e), hist->last_pyr);
            }
        }
    }// HERE

    // clear temporary flags
    e->db_flags &= ~(DBFLAG_UNTARGETABLE | DBFLAG_INVINCIBLE | DBFLAG_INVISIBLE | DBFLAG_CLEARATTRIBS |DBFLAG_ARCHITECT_EXIT );
    if (e->untargetable & UNTARGETABLE_DBFLAG)
        e->db_flags |= DBFLAG_UNTARGETABLE;
    if (e->invincible & INVINCIBLE_DBFLAG)
        e->db_flags |= DBFLAG_INVINCIBLE;
    if (ENTHIDE(e) & 2)
        e->db_flags |= DBFLAG_INVISIBLE;
    if (e->client) 
    {
        if (e->client->controls.nocoll == 2)
            e->db_flags |= DBFLAG_NOCOLL;
        else
            e->db_flags &= ~DBFLAG_NOCOLL;
    }

    if (OnArenaMap() || RaidIsRunning() || g_ArchitectTaskForce || server_visible_state.isPvP )
    {
        e->db_flags |= DBFLAG_CLEARATTRIBS;
    }
    
    t = dbSecondsSince2000();
    dt = t - e->last_time;

    if(!e->last_day_jobs_start)
    {
        //This should only happen if 
        //(1) we have cleared the day jobs field because we've hit the day jobs tick
        //        this can happen more than once in a single login--every 8 min in fact,
        //        but only the first time will award day jobs credit
        //(2) because this is the character's first time logging on.
        e->last_day_jobs_start = t;
        LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Logout location : map %d (%f %f %f)", e->map_id, e->net_pos[0], e->net_pos[1], e->net_pos[2]);
    }

    // dt related
    e->total_time += dt;
    stat_AddTimeInZone(e->pl, dt);
    badge_RecordTime(e, dt);

    last_time = e->last_time;
    e->last_time = t;
    
    playerGetReputation(e);    // Triggers update
    pvp_CleanDefeatList(e);
    fixSuperGroupMode(e);
    initStuffBuff( &sb, MAX_BUF );

    setMaxCostumePartCount(GetBodyPartCount());
    pCostumeOld = e->costume;
    costumeNPCOld = e->npcIndex;
    costumePriorityOld = e->costumePriority;
    fixCostume(e,1);
    assert(e->storyInfo);
    growStoryInfoArrays(e->storyInfo, 0);
    packageUISettings(e);

    for(i=start_line_idx;ent_line_desc[i].type;i++)
    {
        structToLine(&sb,(char*)e,0,ent_desc,&ent_line_desc[i],0);
    }

    packageEntPowers(e, &sb, powers_desc );
    packageEntStats(e, &sb, stats_desc, last_time);
    packageCostumes(e, &sb, costumes_desc );
    packageAppearances( e, &sb, appearances_desc );
    packagePowerCustomizations(e, &sb, powercustomization_list_desc);
    badge_Package( e, &sb );

    entity_packageInventory( e, &sb );

    shrinkStoryInfoArrays(e->storyInfo, e->db_id);
    setMaxCostumePartCount(MAX_COSTUME_PARTS);

    if (isDevelopmentMode())
    {
        FILE* f;
        f = fopen("c:\\temp.out.txt", "wt");
        if (f) {
            fwrite(sb.buff, 1, strlen(sb.buff), f);
            fclose(f);
        }
    }
    //checkEntCorrupted(sb.buff);

    if(validateEntPowersPackage(e, sb.buff) == -1)
    {
        freeStuffBuff(&sb);
        return 0;
    }

    // --------------------
    // log any pieces of the container

    s_logEntContainer( e, LOG_LEVEL_IMPORTANT, "FieldLog:Pak", sb.buff );
    checkContainerBadges(e,sb.buff,__FUNCTION__);


    // restoring original costume
    // @todo NEEDS_REVIEW
    // Is the save and restore of e->costume necessary any longer?
    // Doesn't seem to be as operations now happen on e->pl costumes.
    // If it was then this ad hoc save and restore of e->costume won't
    // cut it any longer, need entDetachCostume() and entRestoreDetachedCostume()
    e->costume = pCostumeOld;
    e->npcIndex = costumeNPCOld;
    e->costumePriority = costumePriorityOld;
    if(e->pl)
        e->pl->npc_costume = costumeNPCOld;

    // --------------------
    // finally

    return sb.buff;
}

char* packageEnt(Entity *e) { return s_packageEnt(e,ENTLINE_WRITE_IDX); }
char* packageEntAll(Entity *e) { return s_packageEnt(e,0); }

//----------------------------------------
//  Helper for unpacking inventory
//----------------------------------------
typedef struct DBEntInv
{
    DBGenericInventoryItem **genItems;
    DBConceptInventoryItem **conceptItems;
} DBEntInv;

static void s_unpackEntInventory(Entity *e, DBEntInv *dbinv)
{
    int i;
    int size; // note: need to add in linear order, so use 'size' var

    if(verify( e && dbinv ))
    {

        size = eaSize( &dbinv->genItems );
        for( i = 0; i < size; ++i)
        {
            if( dbinv->genItems[i] )
            {
                character_AdjustInventoryEx( e->pchar, dbinv->genItems[i]->type, dbinv->genItems[i]->id, dbinv->genItems[i]->amount, NULL, true, false );
                dbgenericinventoryitem_Destroy( dbinv->genItems[i] );
            }
        }
        eaDestroy( &dbinv->genItems );

        size = eaSize( &dbinv->conceptItems );
        for( i = 0; i < size; ++i)
        {
            if( verify( dbinv->conceptItems[i] ))
            {
                const ConceptDef *def = conceptdef_GetById( dbinv->conceptItems[i]->defId );

                if( verify( def ))
                {
                    character_AdjustConcept( e->pchar, dbinv->conceptItems[i]->defId, dbinv->conceptItems[i]->afVars, dbinv->conceptItems[i]->amount );
                }
                else
                {
                    dbLog("UnpackInventoryFatal", e, "invalid concept def id %d", dbinv->conceptItems[i]->defId);
                }


                dbconceptinventoryitem_Destroy( dbinv->conceptItems[i] );
            }
        }
        eaDestroy(&dbinv->conceptItems);
    }
}

static void fixUnpackedCostume(Entity *e)
{
    // backwards compat for capes
    if(e->pl->capesUnlocked)
        rewardtoken_Award(&e->pl->rewardTokens, "Back_Regular_Cape", 1);

    // backwards compat for auras
    if(e->pl->glowieUnlocked)
        rewardtoken_Award(&e->pl->rewardTokens, "Auras_Common", 1);

    // hacky fix for dual valentine costume keys
    if(rewardtoken_IdxFromName(&e->pl->rewardTokens, "Event_Valentine_Heart_Pattern") >= 0)
        rewardtoken_Award(&e->pl->rewardTokens, "Event_Valentine_Pattern", 1);

    // make sure they have all the appropriate keys for their powers
    costumeAwardPowersetParts(e, 1, 0);

    // validate all costumes have geometry
    costumeValidateCostumes(e);

    // make sure they don't have any empty weapons etc.
    costumeFillPowersetDefaults(e);
}

// DGNOTE 9/18/2008
// See comment above unpackEntPowers(...) over in character_db.c for details.  This variable can be eventually removed ...
int g_sanityCheckBuildNumbers = 0;

static void fixMultiBuilds(Entity *e)
{
    if (e && e->pl && e->pl->multiBuildsSetUp == 0)
    {
        // Fix iBuildLevels[0] to mirror current security level
        if (e->pchar && e->pchar->iCurBuild == 0 && e->pchar->iLevel > e->pchar->iBuildLevels[0])
        {
            e->pchar->iBuildLevels[0] = e->pchar->iLevel;
            if (isDevelopmentMode())
            {
                // Players should not need this in production
                g_sanityCheckBuildNumbers = 1;
            }
        }
        e->pl->multiBuildsSetUp = 1;
    }
    e->pchar->iActiveBuild = e->pchar->iCurBuild;
}

static void fixTraySlots(Entity *e)
{
    int k, l;
    int needFixup;
    int clearFullTray = 0;
    Tray *tray;
    TrayObj *obj;
    PowerSet *pset;
    Power *pow;
    // Detect a bad tray by looking for a primary power in slot 2.  Pre-I13, this is where primaries lived.
    // Post-I13, primaries are in slot 4, i.e, g_numSharedPowersets + 1.  +1 to skip inherents which precede primaries
    // Post-I18, primaries are in a way later slot (after Incarnate powersets), so I took out the magic number and put in g_numSharedPowersets + 1
    // Post-I19, primaries are in an even later slot (after a second Inherent powerset), so I took out the last remaining magic number and put in g_numAutomaticSpecificPowersets.

    if (!e || !e->pl || !e->pl->tray)
    {
        return;
    }

    tray = e->pl->tray;

    // I do a metric boat-load of pointer checks in here.  None of them should ever fail, but I'd much rather check and fail gracefully
    // as opposed to crashing / asserting the mapserver.  That would be a "bad thing" (tm).  Players just love it when they can crash
    // mapservers.  The net result of a failure in here is that we can't clean up the mess, so we just destroy the player's tray completely.
    if (e->pchar && eaSize(&e->pchar->ppBuildPowerSets[0]) > (g_numSharedPowersets + g_numAutomaticSpecificPowersets))
    {
        needFixup = 0;
        // Only scan the first build's worth of tray data.  If we have an error, it has to be here
        resetTrayIterator(tray, 0, kTrayCategory_PlayerControlled);
        while (getNextTrayObjFromIterator(&obj))
        {
            if (obj && obj->type == kTrayItemType_Power && obj->iset == 2)
            {
                // Got a candidate.
                if (obj->pchPowerSetName 
                    && e->pchar->ppBuildPowerSets[0][g_numSharedPowersets + g_numAutomaticSpecificPowersets]
                    && e->pchar->ppBuildPowerSets[0][g_numSharedPowersets + g_numAutomaticSpecificPowersets]->psetBase
                    && e->pchar->ppBuildPowerSets[0][g_numSharedPowersets + g_numAutomaticSpecificPowersets]->psetBase->pchName)
                {                        
                    if (strcmp(obj->pchPowerSetName, e->pchar->ppBuildPowerSets[0][g_numSharedPowersets + g_numAutomaticSpecificPowersets]->psetBase->pchName) == 0)
                    {
                        needFixup = 1;
                        break;
                    }
                }
            }
        }
        if (needFixup)
        {
            resetTrayIterator(tray, 0, kTrayCategory_PlayerControlled);
            while (getNextTrayObjFromIterator(&obj))
            {
                if (obj && obj->type == kTrayItemType_Power)
                {
                    for (k = eaSize(&e->pchar->ppBuildPowerSets[0]) - 1; k >= 0; k--)
                    {
                        pset = e->pchar->ppBuildPowerSets[0][k];
                        for (l = eaSize(&pset->ppPowers) - 1; l >= 0; l--)
                        {
                            pow = pset->ppPowers[l];
                            // Sanity check pointers
                            if (pow && pow->ppowBase && pow->ppowBase->psetParent && pow->ppowBase->psetParent->pcatParent &&
                                pow->ppowBase->pchName && pow->ppowBase->psetParent->pchName && pow->ppowBase->psetParent->pcatParent->pchName)
                            {
                                if (stricmp(obj->pchPowerSetName, pow->ppowBase->psetParent->pchName) == 0 &&
                                    stricmp(obj->pchCategory, pow->ppowBase->psetParent->pcatParent->pchName) == 0)
                                {
                                    // Given the comment immediately below, do I actually need to do this test?
                                    if (stricmp(obj->pchPowerName, pow->ppowBase->pchName) == 0)
                                    {
                                        // Only need to change the iset, since the power number within the powerset won't have changed
                                        obj->iset = k;
                                        k = -1;
                                        break;
                                    }
                                }
                                else
                                {
                                    break;
                                }
                            }
                            else
                            {
                                clearFullTray = 1;
                                k = -1;
                                // break out of the whole mess of nested loops
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        clearFullTray = 1;
    }

    if (clearFullTray)
    {
        resetTrayIterator(tray, -1, kTrayCategory_PlayerControlled);
        while (getNextTrayObjFromIterator(&obj))
        {
            destroyCurrentTrayObjViaIterator();
        }
    }
}

static void fixEpicATFlags(Entity *e)
{
    if (!e)
        return;

    // Characters might have both, neither or just one of the flags which
    // indicate which epic AT they can unlock. Praetorians should never have
    // either of them set, Primals at most one. A Primal with neither set is
    // an old character and we need to set the flag which matches their faction.
    if (!(e->db_flags & DBFLAG_UNLOCK_HERO_EPICS) && !(e->db_flags & DBFLAG_UNLOCK_VILLAIN_EPICS))
    {
        if (ENT_IS_PRIMAL(e))
        {
            if (ENT_IS_HERO(e))
            {
                e->db_flags |= DBFLAG_UNLOCK_HERO_EPICS;
                LOG_OLD( "Unpack Set DBID %d as hero for initial faction\n", e->db_id);
            }
            else
            {
                e->db_flags |= DBFLAG_UNLOCK_VILLAIN_EPICS;
                LOG_OLD( "Unpack Set DBID %d as villain for initial faction\n", e->db_id);
            }
        }
    }
    else if ((e->db_flags & DBFLAG_UNLOCK_HERO_EPICS) && (e->db_flags & DBFLAG_UNLOCK_VILLAIN_EPICS))
    {
        // Turn off the non-matching flag for their faction (or both if they
        // were not Primal-born). We can't know which faction they started on
        // so this is our best guess.
        if (ENT_IS_PRAETORIAN(e) || ENT_IS_HERO(e))
            e->db_flags &= ~DBFLAG_UNLOCK_VILLAIN_EPICS;
        if (ENT_IS_PRAETORIAN(e) || ENT_IS_VILLAIN(e))
            e->db_flags &= ~DBFLAG_UNLOCK_HERO_EPICS;

        LOG_OLD("Unpack DBID %d had both faction flags set\n", e->db_id);
    }
    else
    {
        // We expect Primals to have one flag set, but Praetorians should
        // never have either.
        if (ENT_IS_PRAETORIAN(e))
        {
            e->db_flags &= ~(DBFLAG_UNLOCK_HERO_EPICS | DBFLAG_UNLOCK_VILLAIN_EPICS);
            LOG_OLD("Unpack DBID %d had a faction flag but is Praetorian\n", e->db_id);
        }
    }
}

// MAK - an entity should be zero'ed before calling this function - essentially, entities
// can only be loaded on logins and be correct
void unpackEnt( Entity *e, char *buff )
{
    int     log = 0;
    DB_RestoreAttrib_Types eRestoreAttrib = eRAT_RestoreAll;
    int     idx;
    char    table[100],field[200],idxstr[MAX_BUF],*val;
    DBStats dbstats;
    DBCostume dbcostumes;
    DBAppearance dbappearance;
    DBPowerCustomizationList dbpowercustomizations;
    DBEntInv dbinv = {0};

    char   *bufforig = buff;
    if (isDevelopmentMode())
    {
        FILE* f;
        f = fopen("c:\\temp.in.txt", "wt");
        if (f) {
            fwrite(buff, 1, strlen(buff), f);
            fclose(f);
        }
    }

    memset(&s_dbpows,        0, sizeof(DBPowers)    );
    memset(&dbstats,        0, sizeof(DBStats)    );
    memset(&dbcostumes,        0, sizeof(DBCostume));
    memset(&dbappearance,    0, sizeof(DBAppearance)    );
    memset(&dbpowercustomizations, 0, sizeof(DBPowerCustomizationList) );

    //setMaxCostumePartCount(GetBodyPartCount());
    assert(e->storyInfo);
    growStoryInfoArrays(e->storyInfo, 1);

    // @testing -AB: trying to find badge corruption :07/19/06
    checkContainerBadges(NULL,buff,__FUNCTION__);

    while(decodeLine(&buff,table,field,&val,&idx,idxstr))
    {
        if (strnicmp(table,"var",3)==0)
        {

        }
        else if(strnicmp(table,"Powers",6)==0
                || strnicmp(table,"Boosts",6)==0
                || strnicmp(table,"Inspirations",12)==0
                || strnicmp(table,"AttribMods",10)==0)
        {
            lineToStruct((char *)&s_dbpows,powers_desc,table,field,val,idx);
        }
        else if(strnicmp(table,"Stats",5)==0)
        {
            lineToStruct((char *)&dbstats,stats_desc,table,field,val,idx);
        }
        else if(strnicmp(table,"CostumeParts",12)==0)
        {
            lineToStruct((char *)&dbcostumes,costumes_desc,table,field,val,idx);
        }
        else if(strnicmp(table,"Appearance",10)==0)
        {
            lineToStruct((char *)&dbappearance,appearances_desc,table,field,val,idx);
        }
        else if(strnicmp(table,"PowerCustomizations",19)==0)
        {
            lineToStruct((char *)&dbpowercustomizations,powercustomization_list_desc,table,field,val,idx);
        }
        else if(strnicmp(table,"Badges",6)==0)
        {
            badge_UnpackLine(e, table, field, val, idx);
        }
        else if( entity_unpackInventory(&dbinv.genItems, &dbinv.conceptItems, e, table, idx, field, val) )
        {
            // EMPTY
        }
        else
        {
            lineToStruct((char *)e,ent_desc,table,field,val,idx);
        }
    }

    e->pl->current_powerCust = e->pl->current_costume;

    // Fix up characters that only had a single build.  This must be done before we call unpackEntPowers(...), since that routine will
    // do the initial build selection based on iCurBuild from the database.
    fixMultiBuilds(e);

    // @testing -AB: see if we can detect corruption  :07/19/06
    checkEntBadges(e,"unpackEnt0");

    unpackUISettings(e);
    fixupSouvenirClueReferences(e->storyInfo);
    shrinkStoryInfoArrays(e->storyInfo, e->db_id);
    fixupClueReferences(e->storyInfo);
    setMaxCostumePartCount(MAX_COSTUME_PARTS);
    fixCostume(e,0);
    fixMisc(e);

    // @testing -AB: see if we can detect corruption  :07/19/06
    checkEntBadges(e,"unpackEnt1");

    if (g_ArchitectTaskForce || e->db_flags & DBFLAG_CLEARATTRIBS)
        eRestoreAttrib = eRAT_ClearAbusiveBuffs;

    if( OnArenaMap() || RaidIsRunning() || server_visible_state.isPvP)
        eRestoreAttrib = eRAT_ClearAll;

    // WARNING: anything unpacked into a pchar will get wiped out at this stmt
    log = unpackEntPowers(e, &s_dbpows, eRestoreAttrib, "Unpack");
    // No longer need (or want) this.

    g_sanityCheckBuildNumbers = 0;

    // @testing -AB: see if we can detect corruption  :07/19/06
    checkEntBadges(e,"unpackEnt_afterpowers");

    // This column is deprecated and nobody on Live should ever have anything
    // in it. For the Going Rogue beta this means we're deleting money for
    // some people, but that was going to happen anyway.
    e->pchar->iInfluenceEscrow = 0;

    unpackEntStats(e, &dbstats);
    unpackCostumes(e, &dbcostumes);
    unpackAppearance( e, &dbappearance );
    unpackPowerCustomizations(e, &dbpowercustomizations);
    fixupMissionDoors(e->storyInfo, ENT_IS_VILLAIN(e));
    if (e->pchar->pclass && e->pchar->porigin && !e->corrupted_dont_save)
    {
        badge_UpdateBadgesCollection(e);        //    only do this after a character is created
    }
    tray_unpack( e );
    updateExpireables(e);
    fixSpecialAttribs(e);
    e->pl->reputationUpdateTime = e->last_time ? e->last_time : dbSecondsSince2000();    // Didn't bother to duplicate this value in the db

    e->pvpZoneTimer = PVP_ZONE_TIME;
    playerAddReputation(e, 0.0);    // Refresh the reputation related information

    s_unpackEntInventory(e, &dbinv);
    unpackChatSettings(e);

    if(log)
    {
        LOG_OLD("Unpacked for %d:\n{\n%s\n}\n", e->db_id, bufforig);
        LOG_OLD( "Unpack issue logged for %d", e->db_id);
    }

    entUpdatePosInstantaneous(e,ENTPOS(e));

    fixEpicATFlags(e);

    if (e->db_flags & DBFLAG_UNTARGETABLE)
    {
        e->untargetable = UNTARGETABLE_DBFLAG;
        e->admin = 1;
    }
    if (e->db_flags & DBFLAG_INVINCIBLE)
        e->invincible = INVINCIBLE_DBFLAG;
    if (e->db_flags & DBFLAG_INVISIBLE)
        SET_ENTHIDE(e) = 2;
    if (e->db_flags & DBFLAG_NOCOLL)
        if (e->client)
            e->client->controls.nocoll = 2;

    if ( db_state.has_gurneys || db_state.has_villain_gurneys )
    {
        if( db_state.is_static_map )
        {
            StaticMapInfo    *info = staticMapInfoFind(db_state.map_id);
            if (info && !info->dontTrackGurneys)
            {
                if( db_state.has_gurneys )
                    e->gurney_map_id = db_state.map_id;
                if( db_state.has_villain_gurneys ) //Tracked separately in case you change teams
                    e->villain_gurney_map_id = db_state.map_id;
            }
        }
        //If you are in a mission with it's own gurney]
        //(per design, mission gurneys aren't hero or villain specific)
        else
        {
            e->mission_gurney_map_id = db_state.map_id;
        }
    }

    fixUnpackedCostume(e);

    // Fix up tray slots for I13 multiple builds.
    fixTraySlots(e);

    // --------------------
    // log any pieces of the container

    s_logEntContainer( e, LOG_LEVEL_IMPORTANT, "FieldLog:UnPak", bufforig );

    // @testing -AB: see if we can detect corruption  :07/19/06
    checkEntBadges(e,"unpackEnt_last");
    //set the player's locale to the same as the server
    e->playerLocale = getCurrentLocale();

}

static bool validAttributeString(const char *s)
{
    if(strHasSQLEscapeChars(s))
        return false;
    for(;s && *s;s++)
        if(*s & 0x80)
            return false;
    return true;
}

#define TEST_ATTRIBS(S) if(!validAttributeString(S)) FatalErrorf("invalid attribute %s",S);
#define TEST_ATTRIBS_CTX(CTX,S) if(!validAttributeString(S)) FatalErrorf("%s: invalid attribute %s",CTX,S);
#define TEST_ATTRIBS_FN(S,FN) if(!validAttributeString(S)) FatalErrorFilenamef(FN, "invalid attribute %s",S)

StuffBuff * gsb;
static int addPermanentRewardTokensToStuffBuff(cStashElement element)
{
    const char * rewardTableName = stashElementGetStringKey(element);

    if( rewardTableName && rewardIsPermanentRewardToken( rewardTableName ) )
    {
        TEST_ATTRIBS(rewardTableName);
        addStringToStuffBuff(gsb, "\"%s\"\n", rewardTableName);
        printf("Adding Perma Reward %s\n", rewardTableName);
    }
    return 1;
}

static void s_addStrings(StuffBuff *sb, const char * const *strs, const char *ctx)
{
    int i;
    for(i = eaSize(&strs)-1; i >= 0; --i)
    {
        TEST_ATTRIBS_CTX(ctx, strs[i]);
        addStringToStuffBuff(sb,"\"%s\"\n", strs[i]);
    }
}

AttributeNamesCallback *attributeNamesCallbacks = 0;

static char *attributeNames()
{
    int            i,j,k,l,m,n;
    StuffBuff      sb;
    ScriptVarsTable vars = {0};

    initStuffBuff( &sb, MAX_BUF );

    // Names for all the statistics
    StuffStatNames(&sb);

    // Get all the class and origin names.
    for(i=eaSize(&g_CharacterClasses.ppClasses)-1; i>=0; i--)
    {
        TEST_ATTRIBS_CTX("Classes", g_CharacterClasses.ppClasses[i]->pchName);
        addStringToStuffBuff(&sb,"\"%s\"\n", g_CharacterClasses.ppClasses[i]->pchName);
    }
    for(i=eaSize(&g_CharacterOrigins.ppOrigins)-1; i>=0; i--)
    {
        TEST_ATTRIBS_CTX("Origins", g_CharacterOrigins.ppOrigins[i]->pchName);
        addStringToStuffBuff(&sb,"\"%s\"\n", g_CharacterOrigins.ppOrigins[i]->pchName);
    }
    for(i=eaSize(&g_VillainClasses.ppClasses)-1; i>=0; i--)
    {
        TEST_ATTRIBS_CTX("Villain Classes", g_VillainClasses.ppClasses[i]->pchName);
        addStringToStuffBuff(&sb,"\"%s\"\n", g_VillainClasses.ppClasses[i]->pchName);
    }
    for(i=eaSize(&g_VillainOrigins.ppOrigins)-1; i>=0; i--)
    {
        TEST_ATTRIBS_CTX("Villain Origins", g_VillainOrigins.ppOrigins[i]->pchName);
        addStringToStuffBuff(&sb,"\"%s\"\n", g_VillainOrigins.ppOrigins[i]->pchName);
    }

    // Get all the category, power set, and power names
    for(i=eaSize(&g_PowerDictionary.ppPowerCategories)-1; i>=0; i--)
    {
        int iset;
        const PowerCategory *pcat = g_PowerDictionary.ppPowerCategories[i];

        TEST_ATTRIBS_FN(pcat->pchSourceFile,pcat->pchName);
        addStringToStuffBuff(&sb,"\"%s\"\n", pcat->pchName);

        for(iset=eaSize(&pcat->ppPowerSets)-1; iset>=0; iset--)
        {
            int ipow;
            const BasePowerSet *pset = pcat->ppPowerSets[iset];

            TEST_ATTRIBS_FN(pset->pchSourceFile,pset->pchName);
            addStringToStuffBuff(&sb,"\"%s\"\n", pset->pchName);

            for(ipow=eaSize(&pset->ppPowers)-1; ipow>=0; ipow--)
            {
                const BasePower* bp = pset->ppPowers[ipow];
                TEST_ATTRIBS_FN(bp->pchSourceName,bp->pchName);
                addStringToStuffBuff(&sb,"\"%s\"\n", bp->pchName);
            }
        }
    }

    // Get all BodyPart names.  These go into the CostumePart.Name fields.
    for(i = eaSize(&bodyPartList.bodyParts)-1; i >= 0; i--)
    {
        const BodyPart* part = bodyPartList.bodyParts[i];
        TEST_ATTRIBS_FN(part->sourceFile,part->name);
        addStringToStuffBuff(&sb,"\"%s\"\n", part->name);
    }

    // get all contact names - handles are 1 to n
    n = ContactNumDefinitions();
    for (i = 1; i <= n; i++)
    {
        TEST_ATTRIBS_FN(ContactFileName(i),ContactFileName(i));
        addStringToStuffBuff(&sb,"\"%s\"\n", ContactFileName(i));
    }

    // get all storyarc names - handles are -1 to -n
    n = StoryArcNumDefinitions();
    for (i = 1; i <= n; i++)
    {
        TEST_ATTRIBS_FN( StoryArcFileName(-i),StoryArcFileName(-i) );
        addStringToStuffBuff(&sb,"\"%s\"\n", StoryArcFileName(-i));
    }

    // all souvenir clue names
    i = 0;
    while(1)
    {
        const SouvenirClue* clue = scGetByIndex(i);
        if(!clue)
            break;

        TEST_ATTRIBS_FN(clue->filename,clue->name);
        addStringToStuffBuff(&sb,"\"%s\"\n", clue->name);
        i++;
    }

    // Walk the reward table and add all PermanentRewardTokens
    gsb = &sb; //poor man's parameters to the call back
    stashForEachElementConst(rewardTableStash(),addPermanentRewardTokensToStuffBuff);

    // get all unique task names
    n = UniqueTaskGetCount();
    for (i = 0; i < n; i++)
    {
        TEST_ATTRIBS_CTX("Tasks", UniqueTaskGetAttribute(UniqueTaskByIndex(i)));
        addStringToStuffBuff(&sb, "\"%s\"\n", UniqueTaskGetAttribute(UniqueTaskByIndex(i)));
    }

    loadCostumes();
    loadPowerCustomizations();

    for( i = eaSize(&gCostumeMaster.bodySet)-1; i >= 0; i-- )
    {
        TEST_ATTRIBS_CTX("Costume BodySet", gCostumeMaster.bodySet[i]->name );
        addStringToStuffBuff(&sb,"\"%s\"\n", gCostumeMaster.bodySet[i]->name );
        for( j = eaSize(&gCostumeMaster.bodySet[i]->originSet)-1; j >= 0; j-- )
        {
            for( k = eaSize(&gCostumeMaster.bodySet[i]->originSet[j]->regionSet)-1; k >= 0; k--)
            {
                const CostumeRegionSet * rset = gCostumeMaster.bodySet[i]->originSet[j]->regionSet[k];

                TEST_ATTRIBS_CTX("Costume RegionSet", rset->name );
                addStringToStuffBuff(&sb,"\"%s\"\n", rset->name );
                s_addStrings(&sb, rset->keys, "Costume RegionSet Key");

                for( l = eaSize( &rset->boneSet )-1; l >= 0; l--)
                {
                    TEST_ATTRIBS_CTX("Costume BoneSet", rset->boneSet[l]->name );
                    addStringToStuffBuff(&sb,"\"%s\"\n", rset->boneSet[l]->name );
                    s_addStrings(&sb, rset->boneSet[l]->keys, "Costume BoneSet Key");

                    for( m = eaSize(&rset->boneSet[l]->geo )-1; m >= 0; m--)
                    {
                        const CostumeGeoSet *gset = rset->boneSet[l]->geo[m];

                        TEST_ATTRIBS_CTX("Costume GeoSet", gset->bodyPartName );
                        addStringToStuffBuff(&sb,"\"%s\"\n", gset->bodyPartName );
                        s_addStrings(&sb, gset->keys, "Costume GeoSet Key");

                        for( n = eaSize(&gset->info)-1; n >= 0; n-- )
                        {
                            TEST_ATTRIBS_CTX("Costume Geom", gset->info[n]->geoName            );
                            addStringToStuffBuff(&sb,"\"%s\"\n", gset->info[n]->geoName            );
                            TEST_ATTRIBS_CTX("Costume Tex1", gset->info[n]->texName1        );
                            addStringToStuffBuff(&sb,"\"%s\"\n", gset->info[n]->texName1        );
                            TEST_ATTRIBS_CTX("Costume Tex2", gset->info[n]->texName2        );
                            addStringToStuffBuff(&sb,"\"%s\"\n", gset->info[n]->texName2        );
                            TEST_ATTRIBS_CTX("Costume FX", gset->info[n]->fxName            );
                            addStringToStuffBuff(&sb,"\"%s\"\n", gset->info[n]->fxName            );
                            s_addStrings(&sb, gset->info[n]->keys, "Costume Key");
                        }

                        for( n = eaSize(&gset->maskNames)-1; n >= 0; n-- )
                        {
                            TEST_ATTRIBS_CTX("Costume Mask", gset->maskNames[n] );
                            addStringToStuffBuff(&sb,"\"%s\"\n", gset->maskNames[n] );
                        }

                        for( n = eaSize(&gset->masks)-1; n >= 0; n-- )
                        {
                            TEST_ATTRIBS_CTX("Costume Mask", gset->masks[n] );
                            addStringToStuffBuff(&sb,"\"%s\"\n", gset->masks[n] );
                        }

                        for( n = eaSize(&gset->mask)-1; n >= 0; n-- )
                        {
                            TEST_ATTRIBS_CTX("Costume Mask", gset->mask[n]->name    );
                            addStringToStuffBuff(&sb,"\"%s\"\n", gset->mask[n]->name );
                            s_addStrings(&sb, gset->mask[n]->keys, "Costume Mask Key");
                        }
                    }
                }
            }
        }
    }

    for(i = eaSize(&g_tasksets.sets)-1; i >= 0; i--)
    {
        for(j = eaSize(&g_tasksets.sets[i]->tasks)-1; j >= 0; j--)
        {
            if(    strstriConst(g_tasksets.sets[i]->filename, "/BROKERS/") ||
                strstriConst(g_tasksets.sets[i]->filename, "/Police_Band/") )
            {
                ScriptVarsTablePushScope(&vars, &g_tasksets.sets[i]->tasks[j]->vs);

                for (k = eaSize(&g_tasksets.sets[i]->tasks[j]->vs.vars) - 1; k > 0; k--)
                {
                    if (!stricmp("ITEM_NAME", VarName(g_tasksets.sets[i]->tasks[j]->vs.vars[k])) ||
                        !stricmp("PERSON_NAME", VarName(g_tasksets.sets[i]->tasks[j]->vs.vars[k])))
                    {
                        char* groupname = VarGroupName(g_tasksets.sets[i]->tasks[j]->vs.vars[k]);
                        int numvalues = VarNumValues(g_tasksets.sets[i]->tasks[j]->vs.vars[k]);

                        for(l = 0; l < numvalues; l++)
                        {
                            char* brokerString = VarValue(g_tasksets.sets[i]->tasks[j]->vs.vars[k], l);
                            TEST_ATTRIBS_CTX("Brokers", brokerString);
                            addStringToStuffBuff(&sb,"\"%s\"\n", brokerString);
                        }
                        break;
                    }
                }
                ScriptVarsTablePopScope(&vars);
            }
        }
    }

    // All base detail names
    for(i=eaSize(&g_DetailDict.ppDetails)-1; i>=0; i--)
    {
        TEST_ATTRIBS_CTX("Base Detail", g_DetailDict.ppDetails[i]->pchName);
        addStringToStuffBuff(&sb,"\"%s\"\n", g_DetailDict.ppDetails[i]->pchName);
    }

    // All recipe names
    for(i=eaSize(&g_DetailRecipeDict.ppRecipes)-1; i>=0; i--)
    {
        TEST_ATTRIBS_CTX("Recipe", g_DetailRecipeDict.ppRecipes[i]->pchName);
        addStringToStuffBuff(&sb,"\"%s\"\n", g_DetailRecipeDict.ppRecipes[i]->pchName);
    }

    // All custom power tokens
    for (i=eaSize(&g_PowerDictionary.ppPowers) - 1; i >= 0; --i)
    {
        const BasePower* power = g_PowerDictionary.ppPowers[i];

        if (power->customfx == 0)
            continue;

        for (j = eaSize(&power->customfx) - 1; j >= 0; --j)
        {
            TEST_ATTRIBS_CTX("Custom FX", power->customfx[j]->pchToken);
            addStringToStuffBuff(&sb, "\"%s\"\n", power->customfx[j]->pchToken);
        }
    }

    // The compiler doesn't like EArrays of function pointers...
    for (i = eaSize((void***)&attributeNamesCallbacks) - 1; i >= 0; i--)
    {
        (*attributeNamesCallbacks[i])(&sb);
    }

    return sb.buff;
}

void AddAttributeNamesCallback(AttributeNamesCallback newCallback)
{
    eaPush((void***)&attributeNamesCallbacks, newCallback);
}

StructDesc team_active_player_reward_token_desc[] =
{
    sizeof(RewardToken),
    {AT_EARRAY, OFFSET(Teamup, activePlayerRewardTokens )},
    reward_token_line_desc,
    "RewardToken table is a list of all the custom player rewards.<br>"
    "Each row represents a single reward.<br>"
    "Each character can have many rewards."
};

LineDesc teamup_line_desc[] =
{
    {{ PACKTYPE_CONREF,            CONTAINER_ENTS,                        "LeaderId",        OFFSET2(Teamup, members, TeamMembers, leader  ) },
        "DB ID - ContainerID of the leader of the team"},

    {{ PACKTYPE_INT,            SIZE_INT32,                            "MissionMapId",    OFFSET(Teamup, map_id)                            },
        "Map Id of the current active mission"},

    {{ PACKTYPE_INT,            SIZE_INT32,                            "PlayersOnMap",    OFFSET(Teamup, map_playersonmap)            },
        "Number of players currently on the mission map"},

    {{ PACKTYPE_INT,            SIZE_INT32,                            "InstanceType",    OFFSET(Teamup, map_type)                    },
        "Describes what kind of instanced map the team is on.<br>"
        "   1 = MAPINSTANCE_MISSION<br>"
        "   2 = MAPINSTANCE_ARENA"},

    {{ PACKTYPE_ATTR,        MAX_ATTRIBNAME_LEN,                        "Contact",        OFFSET(Teamup, mission_contact),        INOUT(ContactGetHandle, ContactFileName)},
        "The contact involved with the teams current mission"},

    {{ PACKTYPE_STR_UTF8,            SIZEOF2(Teamup, mission_status),    "Status",        OFFSET(Teamup, mission_status)                    },
        "String that describes the current status of the teams mission"},

    {{ PACKTYPE_INT,            SIZE_INT32,                            "SidkickCount",    OFFSET(Teamup, deprecated)                },
        "The number of sidekick/exemplar pairs on the team"},

    {{ PACKTYPE_INT,            SIZE_INT32,                            "KeyClues",        OFFSET(Teamup, keyclues)                        },
        "Bitfield - Tracks which clues have been sent to the team"},

    {{ PACKTYPE_INT,            SIZE_INT32,                            "KheldianCount",    OFFSET(Teamup, kheldianCount)                    },
        "Number of Kheldians on the team"},

    {{ PACKTYPE_INT,            SIZE_INT32,                            "LastAmbush",    OFFSET(Teamup, lastambushtime)                    },
        "Keeps track of the last time the team was ambushed"},
    {{ PACKTYPE_INT,            SIZE_INT32,                            "TeamLevel",    OFFSET(Teamup, team_level)                    },
            "Level of the team"},
    {{ PACKTYPE_INT,            SIZE_INT32,                            "TeamMentor",    OFFSET(Teamup, team_mentor)                    },
            "Mentor of the team"},

    {{ PACKTYPE_INT,            SIZE_INT32,                            "TeamSwapLock",        OFFSET(Teamup, teamSwapLock)                    },
        "Team swap lock status"},
    
    {{ PACKTYPE_INT,            SIZE_INT32,                            "ActivePlayerDbid",    OFFSET(Teamup, activePlayerDbid)                },
        "Dbid of owner of active task (or last active task if there isn't one currently)"},
    {{ PACKTYPE_INT,            SIZE_INT32,                            "ActivePlayerRevision",    OFFSET(Teamup, activePlayerRevision)    },
        "Index used to determine when an entity has updated active player information and needs to refresh flagged powers"},
    {{ PACKTYPE_INT,            SIZE_INT32,                            "ProbationalActivePlayerDbid",    OFFSET(Teamup, probationalActivePlayerDbid)                },
        "Dbid of owner of active task (or last active task if there isn't one currently) before 10 second timer has elapsed"},
    {{ PACKTYPE_INT,            SIZE_INT32,                            "ProbationalActivePlayerDbidExpiration",    OFFSET(Teamup, probationalActivePlayerDbidExpiration)    },
        "Time at which the probational active player becomes the real active player"},

    {{ PACKTYPE_INT,            SIZE_INT32,                            "MaximumPlayerCount",    OFFSET(Teamup, maximumPlayerCount)        },
        "The biggest this Teamup's member count has ever been."},

    {{ PACKTYPE_EARRAY,            (intptr_t)rewardtoken_Create,            "TeamupRewardTokensActive",            (intptr_t)team_active_player_reward_token_desc        },
        "reward tokens the active player of this team has"},

    { PACKTYPE_SUB,            1,                                    "TeamupTask",    (intptr_t)team_task_desc                                },

    { 0 },
};

StructDesc teamup_desc[] =
{
    sizeof(Teamup),
    {AT_NOT_ARRAY,{0}},
    teamup_line_desc,

    "Describes a team that was consists of one or more players.<br>"
    "Every player can only be a member of one teamup at any given time."
};


LineDesc taskforce_line_desc[] =
{
    {{ PACKTYPE_CONREF,    CONTAINER_ENTS,                        "LeaderId",            OFFSET2(TaskForce, members, TeamMembers, leader  ) },
        "DB ID - ContainerID of the leader of the task force"},

    {{ PACKTYPE_STR_UTF8,    SIZEOF2(TaskForce,  name),            "Name",                OFFSET(TaskForce,  name),            },
        "The name assigned to the task force"},

    {{ PACKTYPE_BIN_STR,    UNIQUE_TASK_BITFIELD_SIZE*4,    "UniqueTaskIssued",        OFFSET2_PTR(TaskForce, storyInfo,    StoryInfo,    uniqueTaskIssued),    },
        "Bitfield - Keeps track of the unique tasks issued to to the taskforce so that they cannot be repeated"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "Level",            OFFSET(TaskForce, level)    },
        "The level spawns will appear"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "LevelAdjust",        OFFSET(TaskForce, levelAdjust) },
        "All spawn levels will also be boosted by this amount(specified by the contact who gave the task force)"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "DeleteMe",            OFFSET(TaskForce, deleteMe) },
        "Flags the taskforce for deletion now that it has ended"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "ExemplarLevel",    OFFSET(TaskForce, exemplar_level) },
        "The highest level of the taskforce, everyone above this level will be capped to it"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "Parameters",        OFFSET(TaskForce, parametersSet) },
        "Bitfield - tracks which taskforce parameters are enabled"},

    {{ PACKTYPE_INT, SIZE_INT32,                            "MinTeamSize",        OFFSET(TaskForce, minTeamSize) },
        "The minimum team size for this task force"},
    
    {{ PACKTYPE_INT, SIZE_INT32,                            "ArchitectId",        OFFSET(TaskForce, architect_id) },
        "Id of the mission on the mission server" },

    {{ PACKTYPE_INT, SIZE_INT32,                            "ArchitectTestMode",OFFSET(TaskForce, architect_flags) },
        "Testing a player created story arc" },

    {{ PACKTYPE_INT, SIZE_INT32,                            "ArchitectAuthId",    OFFSET(TaskForce, architect_authid) },
        "Author of this arc, if from the same auth region" },

    // estring blobs must be last
    {{ PACKTYPE_LARGE_ESTRING_BINARY, 0,                        "PlayerStoryArc",    OFFSET(TaskForce, playerCreatedArc) },
        "Serialized player-created story arc that this taskforce is running"},

    { PACKTYPE_SUB, CONTACTS_PER_TASKFORCE,        "TaskForceContacts",        (intptr_t)tf_contact_desc        },
    { PACKTYPE_SUB,    STORYARCS_PER_TASKFORCE,    "TaskForceStoryArcs",        (intptr_t)tf_storyarc_desc        },
    { PACKTYPE_SUB,    TASKS_PER_TASKFORCE,        "TaskForceTasks",            (intptr_t)tf_task_desc            },
    { PACKTYPE_SUB, SOUVENIRCLUES_PER_PLAYER,    "TaskForceSouvenirClues",    (intptr_t)tf_souvenirClue_desc    },
    { PACKTYPE_SUB, PARAMETERS_PER_TASKFORCE,    "TaskForceParameters",        (intptr_t)tf_parameters_desc        },
    { 0 },
};

StructDesc taskforce_desc[] =
{
    sizeof(TaskForce),
    {AT_NOT_ARRAY,{0}},
    taskforce_line_desc,

    "Describes a taskforce that was created by a set number of people to go on a special storyarc together.<br>"
    "Every player can only be a member of one taskforce at any given time."
};

LineDesc raid_line_desc[] =
{
    { 0 },
};

StructDesc raid_desc[] =
{
    0,
    {AT_NOT_ARRAY,{0}},
    raid_line_desc,
    "this is a stub for a future container type."
};

LineDesc eventhistory_line_desc[] =
{
    {{ PACKTYPE_INT, SIZE_INT32, "TimeStamp", OFFSET(KarmaEventHistory, timeStamp) },
        "Start date/time for the event" },
    {{ PACKTYPE_STR_UTF8, 50, "EventName", OFFSET_PTR(KarmaEventHistory, eventName) },
        "Name of the event (eg. BAF)" },
    {{ PACKTYPE_STR_UTF8, 32, "AuthName", OFFSET_PTR(KarmaEventHistory, authName) },
        "Account name for this player" },
    {{ PACKTYPE_STR_UTF8, 21, "Name", OFFSET_PTR(KarmaEventHistory, playerName) },
        "Player character name" },
    {{ PACKTYPE_INT, SIZE_INT32, "DBID", OFFSET(KarmaEventHistory, dbid) },
        "DBID for the character (lets us check deleted/renamed characters)" },
    {{ PACKTYPE_INT, SIZE_INT32, "Level", OFFSET(KarmaEventHistory, playerLevel) },
        "Character level (0-based) during the event" },
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN, "Class", OFFSET(KarmaEventHistory, playerClass) },
        "Character Archetype" },
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN, "PrimaryPowerset", OFFSET(KarmaEventHistory, playerPrimary) },
        "Character Primary powerset" },
    {{ PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN, "SecondaryPowerset", OFFSET(KarmaEventHistory, playerSecondary) },
        "Character Secondary powerset" },
    {{ PACKTYPE_STR_UTF8, 50, "BandTable", OFFSET_PTR(KarmaEventHistory, bandTable) },
        "Reward table awarded by band" },
    {{ PACKTYPE_STR_UTF8, 50, "PercentileTable", OFFSET_PTR(KarmaEventHistory, percentileTable) },
        "Reward table awarded by percentile" },
    {{ PACKTYPE_INT, SIZE_INT32, "OfferedTable", OFFSET(KarmaEventHistory, whichTable) },
        "Which reward table offered (0 = no table, 1 = band table, 2 = percentile table)" },
    {{ PACKTYPE_STR_UTF8, 50, "RewardChosen", OFFSET_PTR(KarmaEventHistory, rewardChosen) },
        "Item (if any) chosen by the character" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagesPlayed", OFFSET(KarmaEventHistory, playedStages) },
        "Number of stages of the event played by this character" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagesQualified", OFFSET(KarmaEventHistory, qualifiedStages) },
        "Number of stages of the event qualified (played successfully) by this character" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagesAvgScore", OFFSET(KarmaEventHistory, avgStagePoints) },
        "Weighted average score for all event stages for this character" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagesAvgPercentile", OFFSET(KarmaEventHistory, avgStagePercentile) },
        "Weighted average percentage rating for all event stages for this character" },
    {{ PACKTYPE_INT, SIZE_INT32, "TimePaused", OFFSET(KarmaEventHistory, pauseDuration) },
        "How long in seconds (if at all) the character's participation in the event was paused" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagesPaused", OFFSET(KarmaEventHistory, pausedStagesCredited) },
        "How many stages the character received credit for while paused (should only be 0 or 1)" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore1", OFFSET(KarmaEventHistory, stageScore[0]) },
        "Score for stage 1" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold1", OFFSET(KarmaEventHistory, stageThreshold[0]) },
        "Threshold for stage 1 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank1", OFFSET(KarmaEventHistory, stageRank[0]) },
        "Rank for stage 1" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile1", OFFSET(KarmaEventHistory, stagePercentile[0]) },
        "Percentile for stage 1" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples1", OFFSET(KarmaEventHistory, stageSamples[0]) },
        "Samples for stage 1" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore2", OFFSET(KarmaEventHistory, stageScore[1]) },
        "Score for stage 2" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold2", OFFSET(KarmaEventHistory, stageThreshold[1]) },
        "Threshold for stage 2 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank2", OFFSET(KarmaEventHistory, stageRank[1]) },
        "Rank for stage 2" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile2", OFFSET(KarmaEventHistory, stagePercentile[1]) },
        "Percentile for stage 2" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples2", OFFSET(KarmaEventHistory, stageSamples[1]) },
        "Samples for stage 2" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore3", OFFSET(KarmaEventHistory, stageScore[2]) },
        "Score for stage 3" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold3", OFFSET(KarmaEventHistory, stageThreshold[2]) },
        "Threshold for stage 3 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank3", OFFSET(KarmaEventHistory, stageRank[2]) },
        "Rank for stage 3" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile3", OFFSET(KarmaEventHistory, stagePercentile[2]) },
        "Percentile for stage 3" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples3", OFFSET(KarmaEventHistory, stageSamples[2]) },
        "Samples for stage 3" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore4", OFFSET(KarmaEventHistory, stageScore[3]) },
        "Score for stage 4" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold4", OFFSET(KarmaEventHistory, stageThreshold[3]) },
        "Threshold for stage 4 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank4", OFFSET(KarmaEventHistory, stageRank[3]) },
        "Rank for stage 4" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile4", OFFSET(KarmaEventHistory, stagePercentile[3]) },
        "Percentile for stage 4" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples4", OFFSET(KarmaEventHistory, stageSamples[3]) },
        "Samples for stage 4" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore5", OFFSET(KarmaEventHistory, stageScore[4]) },
        "Score for stage 5" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold5", OFFSET(KarmaEventHistory, stageThreshold[4]) },
        "Threshold for stage 5 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank5", OFFSET(KarmaEventHistory, stageRank[4]) },
        "Rank for stage 5" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile5", OFFSET(KarmaEventHistory, stagePercentile[4]) },
        "Percentile for stage 5" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples5", OFFSET(KarmaEventHistory, stageSamples[4]) },
        "Samples for stage 5" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore6", OFFSET(KarmaEventHistory, stageScore[5]) },
        "Score for stage 6" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold6", OFFSET(KarmaEventHistory, stageThreshold[5]) },
        "Threshold for stage 6 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank6", OFFSET(KarmaEventHistory, stageRank[5]) },
        "Rank for stage 6" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile6", OFFSET(KarmaEventHistory, stagePercentile[5]) },
        "Percentile for stage 6" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples6", OFFSET(KarmaEventHistory, stageSamples[5]) },
        "Samples for stage 6" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore7", OFFSET(KarmaEventHistory, stageScore[6]) },
        "Score for stage 7" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold7", OFFSET(KarmaEventHistory, stageThreshold[6]) },
        "Threshold for stage 7 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank7", OFFSET(KarmaEventHistory, stageRank[6]) },
        "Rank for stage 7" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile7", OFFSET(KarmaEventHistory, stagePercentile[6]) },
        "Percentile for stage 7" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples7", OFFSET(KarmaEventHistory, stageSamples[6]) },
        "Samples for stage 7" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore8", OFFSET(KarmaEventHistory, stageScore[7]) },
        "Score for stage 8" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold8", OFFSET(KarmaEventHistory, stageThreshold[7]) },
        "Threshold for stage 8 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank8", OFFSET(KarmaEventHistory, stageRank[7]) },
        "Rank for stage 8" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile8", OFFSET(KarmaEventHistory, stagePercentile[7]) },
        "Percentile for stage 8" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples8", OFFSET(KarmaEventHistory, stageSamples[7]) },
        "Samples for stage 8" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore9", OFFSET(KarmaEventHistory, stageScore[8]) },
        "Score for stage 9" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold9", OFFSET(KarmaEventHistory, stageThreshold[8]) },
        "Threshold for stage 9 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank9", OFFSET(KarmaEventHistory, stageRank[8]) },
        "Rank for stage 9" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile9", OFFSET(KarmaEventHistory, stagePercentile[8]) },
        "Percentile for stage 9" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples9", OFFSET(KarmaEventHistory, stageSamples[8]) },
        "Samples for stage 9" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore10", OFFSET(KarmaEventHistory, stageScore[9]) },
        "Score for stage 10" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold10", OFFSET(KarmaEventHistory, stageThreshold[9]) },
        "Threshold for stage 10 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank10", OFFSET(KarmaEventHistory, stageRank[9]) },
        "Rank for stage 10" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile10", OFFSET(KarmaEventHistory, stagePercentile[9]) },
        "Percentile for stage 10" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples10", OFFSET(KarmaEventHistory, stageSamples[9]) },
        "Samples for stage 10" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore11", OFFSET(KarmaEventHistory, stageScore[10]) },
        "Score for stage 11" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold11", OFFSET(KarmaEventHistory, stageThreshold[10]) },
        "Threshold for stage 11 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank11", OFFSET(KarmaEventHistory, stageRank[10]) },
        "Rank for stage 11" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile11", OFFSET(KarmaEventHistory, stagePercentile[10]) },
        "Percentile for stage 11" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples11", OFFSET(KarmaEventHistory, stageSamples[10]) },
        "Samples for stage 11" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore12", OFFSET(KarmaEventHistory, stageScore[11]) },
        "Score for stage 12" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold12", OFFSET(KarmaEventHistory, stageThreshold[11]) },
        "Threshold for stage 12 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank12", OFFSET(KarmaEventHistory, stageRank[11]) },
        "Rank for stage 12" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile12", OFFSET(KarmaEventHistory, stagePercentile[11]) },
        "Percentile for stage 12" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples12", OFFSET(KarmaEventHistory, stageSamples[11]) },
        "Samples for stage 12" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore13", OFFSET(KarmaEventHistory, stageScore[12]) },
        "Score for stage 13" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold13", OFFSET(KarmaEventHistory, stageThreshold[12]) },
        "Threshold for stage 13 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank13", OFFSET(KarmaEventHistory, stageRank[12]) },
        "Rank for stage 13" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile13", OFFSET(KarmaEventHistory, stagePercentile[12]) },
        "Percentile for stage 13" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples13", OFFSET(KarmaEventHistory, stageSamples[12]) },
        "Samples for stage 13" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore14", OFFSET(KarmaEventHistory, stageScore[13]) },
        "Score for stage 14" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold14", OFFSET(KarmaEventHistory, stageThreshold[13]) },
        "Threshold for stage 14 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank14", OFFSET(KarmaEventHistory, stageRank[13]) },
        "Rank for stage 14" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile14", OFFSET(KarmaEventHistory, stagePercentile[13]) },
        "Percentile for stage 14" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples14", OFFSET(KarmaEventHistory, stageSamples[13]) },
        "Samples for stage 14" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore15", OFFSET(KarmaEventHistory, stageScore[14]) },
        "Score for stage 15" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold15", OFFSET(KarmaEventHistory, stageThreshold[14]) },
        "Threshold for stage 15 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank15", OFFSET(KarmaEventHistory, stageRank[14]) },
        "Rank for stage 15" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile15", OFFSET(KarmaEventHistory, stagePercentile[14]) },
        "Percentile for stage 15" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples15", OFFSET(KarmaEventHistory, stageSamples[14]) },
        "Samples for stage 15" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore16", OFFSET(KarmaEventHistory, stageScore[15]) },
        "Score for stage 16" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold16", OFFSET(KarmaEventHistory, stageThreshold[15]) },
        "Threshold for stage 16 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank16", OFFSET(KarmaEventHistory, stageRank[15]) },
        "Rank for stage 16" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile16", OFFSET(KarmaEventHistory, stagePercentile[15]) },
        "Percentile for stage 16" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples16", OFFSET(KarmaEventHistory, stageSamples[15]) },
        "Samples for stage 16" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore17", OFFSET(KarmaEventHistory, stageScore[16]) },
        "Score for stage 17" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold17", OFFSET(KarmaEventHistory, stageThreshold[16]) },
        "Threshold for stage 17 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank17", OFFSET(KarmaEventHistory, stageRank[16]) },
        "Rank for stage 17" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile17", OFFSET(KarmaEventHistory, stagePercentile[16]) },
        "Percentile for stage 17" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples17", OFFSET(KarmaEventHistory, stageSamples[16]) },
        "Samples for stage 17" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore18", OFFSET(KarmaEventHistory, stageScore[17]) },
        "Score for stage 18" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold18", OFFSET(KarmaEventHistory, stageThreshold[17]) },
        "Threshold for stage 18 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank18", OFFSET(KarmaEventHistory, stageRank[17]) },
        "Rank for stage 18" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile18", OFFSET(KarmaEventHistory, stagePercentile[17]) },
        "Percentile for stage 18" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples18", OFFSET(KarmaEventHistory, stageSamples[17]) },
        "Samples for stage 18" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore19", OFFSET(KarmaEventHistory, stageScore[18]) },
        "Score for stage 19" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold19", OFFSET(KarmaEventHistory, stageThreshold[18]) },
        "Threshold for stage 19 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank19", OFFSET(KarmaEventHistory, stageRank[18]) },
        "Rank for stage 19" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile19", OFFSET(KarmaEventHistory, stagePercentile[18]) },
        "Percentile for stage 19" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples19", OFFSET(KarmaEventHistory, stageSamples[18]) },
        "Samples for stage 19" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore20", OFFSET(KarmaEventHistory, stageScore[19]) },
        "Score for stage 20" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold20", OFFSET(KarmaEventHistory, stageThreshold[19]) },
        "Threshold for stage 20 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank20", OFFSET(KarmaEventHistory, stageRank[19]) },
        "Rank for stage 20" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile20", OFFSET(KarmaEventHistory, stagePercentile[19]) },
        "Percentile for stage 20" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples20", OFFSET(KarmaEventHistory, stageSamples[19]) },
        "Samples for stage 20" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore21", OFFSET(KarmaEventHistory, stageScore[20]) },
        "Score for stage 21" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold21", OFFSET(KarmaEventHistory, stageThreshold[20]) },
        "Threshold for stage 21 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank21", OFFSET(KarmaEventHistory, stageRank[20]) },
        "Rank for stage 21" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile21", OFFSET(KarmaEventHistory, stagePercentile[20]) },
        "Percentile for stage 21" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples21", OFFSET(KarmaEventHistory, stageSamples[20]) },
        "Samples for stage 21" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore22", OFFSET(KarmaEventHistory, stageScore[21]) },
        "Score for stage 22" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold22", OFFSET(KarmaEventHistory, stageThreshold[21]) },
        "Threshold for stage 22 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank22", OFFSET(KarmaEventHistory, stageRank[21]) },
        "Rank for stage 22" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile22", OFFSET(KarmaEventHistory, stagePercentile[21]) },
        "Percentile for stage 22" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples22", OFFSET(KarmaEventHistory, stageSamples[21]) },
        "Samples for stage 22" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore23", OFFSET(KarmaEventHistory, stageScore[22]) },
        "Score for stage 23" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold23", OFFSET(KarmaEventHistory, stageThreshold[22]) },
        "Threshold for stage 23 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank23", OFFSET(KarmaEventHistory, stageRank[22]) },
        "Rank for stage 23" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile23", OFFSET(KarmaEventHistory, stagePercentile[22]) },
        "Percentile for stage 23" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples23", OFFSET(KarmaEventHistory, stageSamples[22]) },
        "Samples for stage 23" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore24", OFFSET(KarmaEventHistory, stageScore[23]) },
        "Score for stage 24" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold24", OFFSET(KarmaEventHistory, stageThreshold[23]) },
        "Threshold for stage 24 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank24", OFFSET(KarmaEventHistory, stageRank[23]) },
        "Rank for stage 24" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile24", OFFSET(KarmaEventHistory, stagePercentile[23]) },
        "Percentile for stage 24" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples24", OFFSET(KarmaEventHistory, stageSamples[23]) },
        "Samples for stage 24" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore25", OFFSET(KarmaEventHistory, stageScore[24]) },
        "Score for stage 25" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold25", OFFSET(KarmaEventHistory, stageThreshold[24]) },
        "Threshold for stage 25 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank25", OFFSET(KarmaEventHistory, stageRank[24]) },
        "Rank for stage 25" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile25", OFFSET(KarmaEventHistory, stagePercentile[24]) },
        "Percentile for stage 25" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples25", OFFSET(KarmaEventHistory, stageSamples[24]) },
        "Samples for stage 25" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore26", OFFSET(KarmaEventHistory, stageScore[25]) },
        "Score for stage 26" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold26", OFFSET(KarmaEventHistory, stageThreshold[25]) },
        "Threshold for stage 26 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank26", OFFSET(KarmaEventHistory, stageRank[25]) },
        "Rank for stage 26" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile26", OFFSET(KarmaEventHistory, stagePercentile[25]) },
        "Percentile for stage 26" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples26", OFFSET(KarmaEventHistory, stageSamples[25]) },
        "Samples for stage 26" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore27", OFFSET(KarmaEventHistory, stageScore[26]) },
        "Score for stage 27" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold27", OFFSET(KarmaEventHistory, stageThreshold[26]) },
        "Threshold for stage 27 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank27", OFFSET(KarmaEventHistory, stageRank[26]) },
        "Rank for stage 27" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile27", OFFSET(KarmaEventHistory, stagePercentile[26]) },
        "Percentile for stage 27" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples27", OFFSET(KarmaEventHistory, stageSamples[26]) },
        "Samples for stage 27" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore28", OFFSET(KarmaEventHistory, stageScore[27]) },
        "Score for stage 28" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold28", OFFSET(KarmaEventHistory, stageThreshold[27]) },
        "Threshold for stage 28 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank28", OFFSET(KarmaEventHistory, stageRank[27]) },
        "Rank for stage 28" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile28", OFFSET(KarmaEventHistory, stagePercentile[27]) },
        "Percentile for stage 28" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples28", OFFSET(KarmaEventHistory, stageSamples[27]) },
        "Samples for stage 28" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore29", OFFSET(KarmaEventHistory, stageScore[28]) },
        "Score for stage 29" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold29", OFFSET(KarmaEventHistory, stageThreshold[28]) },
        "Threshold for stage 29 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank29", OFFSET(KarmaEventHistory, stageRank[28]) },
        "Rank for stage 29" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile29", OFFSET(KarmaEventHistory, stagePercentile[28]) },
        "Percentile for stage 29" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples29", OFFSET(KarmaEventHistory, stageSamples[28]) },
        "Samples for stage 29" },

    {{ PACKTYPE_INT, SIZE_INT32, "StageScore30", OFFSET(KarmaEventHistory, stageScore[29]) },
        "Score for stage 30" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageThreshold30", OFFSET(KarmaEventHistory, stageThreshold[29]) },
        "Threshold for stage 30 (same for everybody)" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageRank30", OFFSET(KarmaEventHistory, stageRank[29]) },
        "Rank for stage 30" },
    {{ PACKTYPE_INT, SIZE_INT32, "StagePercentile30", OFFSET(KarmaEventHistory, stagePercentile[29]) },
        "Percentile for stage 30" },
    {{ PACKTYPE_INT, SIZE_INT32, "StageSamples30", OFFSET(KarmaEventHistory, stageSamples[29]) },
        "Samples for stage 30" },
    {0},
};

StructDesc eventhistory_desc[] =
{
    sizeof(KarmaEventHistory),
    {AT_NOT_ARRAY,{0}},
    eventhistory_line_desc,
    "History data for Karma zone events."
};

char *teamupTemplate()
{
    return dbContainerTemplate(teamup_desc);
}

char *teamupSchema()
{
    return dbContainerSchema(teamup_desc, "Teamups");
}

char *packageTeamup(Entity *e)
{
    char * result;

    if( e->teamup->activetask && e->teamup->activetask->sahandle.bPlayerCreated )
        e->teamup->activetask->playerCreatedID = e->teamup->activetask->sahandle.context;

    result = dbContainerPackage(teamup_desc,(char*)e->teamup);
    if (isDevelopmentMode())
    {
        FILE* f;
        f = fopen("c:\\team_todbserver.txt", "wb");
        if (f) {
            fwrite(result, 1, strlen(result), f);
            fclose(f);
        }
    }

    return result;
}

void unpackLeague(Entity *e,char *mem)
{
    if (isDevelopmentMode())
    {
        FILE* f;
        f = fopen("c:\\league_fromdbserver.txt", "wb");
        if (f) {
            fwrite(mem, 1, strlen(mem), f);
            fclose(f);
        }
    }

    if (!e->league)
    {
        e->league = createLeague();
    }
    else
    {
        clearLeague(e->league);
    }

    dbContainerUnpack(league_desc,mem,(char*)e->league);
    e->leaguelist_uptodate = 0;

}

// be very careful if you add more functionality here to guarantee that
// the new functionality CANNOT make e->teamup NULL or free the Teamup pointed to by e->teamup.
void unpackTeamup(Entity *e,char *mem)
{
    U32 now = timerSecondsSince2000();

    if (isDevelopmentMode())
    {
        FILE* f;
        f = fopen("c:\\team_fromdbserver.txt", "wb");
        if (f) {
            fwrite(mem, 1, strlen(mem), f);
            fclose(f);
        }
    }

    if (!e->teamup)
    {
        e->teamup = createTeamup();
        e->teamup->activetask = storyTaskInfoAlloc();
    }
    else if (e->teamupTimer_activePlayer >= now || !e->teamupTimer_activePlayer
                || e->teamup != e->teamup_activePlayer)
    {
        clearTeamup(e->teamup);
    }
    else
    {
        // leave the old e->teamup there, e->teamup_activePlayer will point to that teamup
        // matches with the destroyTeamup() call in character_TickPhaseOne().
        e->teamup = createTeamup();
        e->teamup->activetask = storyTaskInfoAlloc(); // this is fine, unpack clobbers the old data if clearTeamup() is called.
        memcpy(e->teamup->members.rotors, e->teamup_activePlayer->members.rotors, 3*sizeof(int));
    }

    dbContainerUnpack(teamup_desc,mem,(char*)e->teamup);

    e->teamlist_uptodate = 0;

    if (e->teamupTimer_activePlayer >= now || !e->teamupTimer_activePlayer)
    {
        e->teamup_activePlayer = e->teamup; // shallow copy on purpose
    }

    fixupTeamDoor(e);

    if( e->teamup->activetask->sahandle.bPlayerCreated )
        e->teamup->activetask->sahandle.context = e->teamup->activetask->playerCreatedID;
    else if (e->teamup->activetask->sahandle.context &&  !storyTaskInfoInit(e->teamup->activetask) )
        memset(e->teamup->activetask, 0, sizeof(StoryTaskInfo)); // silent cleanup
}

char *packageEventHistory(KarmaEventHistory *event)
{
    char *result;

    result = dbContainerPackage(eventhistory_desc,(char*)event);

    if (isDevelopmentMode())
    {
        FILE* f;
        f = fopen("c:\\eventhistory_todbserver.txt", "wb");
        if (f)
        {
            fwrite(result, 1, strlen(result), f);
            fclose(f);
        }
    }

    return result;
}

KarmaEventHistory *unpackEventHistory(char *mem)
{
    KarmaEventHistory *retval = NULL;

    if (isDevelopmentMode())
    {
        FILE* f;
        f = fopen("c:\\eventhistory_fromdbserver.txt", "wb");
        if (f)
        {
            fwrite(mem, 1, strlen(mem), f);
            fclose(f);
        }
    }

    retval = eventHistory_Create();
    dbContainerUnpack(eventhistory_desc, mem, retval);

    return retval;
}

char *taskForceTemplate()
{
    return dbContainerTemplate(taskforce_desc);
}

char *taskForceSchema()
{
    return dbContainerSchema(taskforce_desc, "TaskForces");
}

char *packageTaskForce(Entity *ent)
{
    char* result;
    growStoryInfoArrays(ent->taskforce->storyInfo, 0);
    result = dbContainerPackage(taskforce_desc,(char*)ent->taskforce);
    shrinkStoryInfoArrays(ent->taskforce->storyInfo, 0);
    fixupClueReferences(ent->taskforce->storyInfo);

    if (isDevelopmentMode())
    {
        FILE* f;
        f = fopen("c:\\taskforce_todbserver.txt", "wb");
        if (f) {
            fwrite(result, 1, strlen(result), f);
            fclose(f);
        }
    }

    return result;
}

static void s_unpackTaskForce(TaskForce **ptaskforce, int dbid, char *mem, int isVillain)
{
    TaskForce *taskforce = *ptaskforce;
    int i;

    if(taskforce)
    {
        clearTaskForce(taskforce);
    }
    else
        *ptaskforce = taskforce = createTaskForce();

    if (isDevelopmentMode())
    {
        FILE* f;
        f = fopen("c:\\taskforce_fromdbserver.txt", "wb");
        if (f) {
            fwrite(mem, 1, strlen(mem), f);
            fclose(f);
        }
    }

    taskforce->storyInfo = storyInfoCreate();
    growStoryInfoArrays(taskforce->storyInfo, 1);
    dbContainerUnpack(taskforce_desc, mem, taskforce);

     if(taskforce->playerCreatedArc && taskforce->playerCreatedArc[0])
    {
        const StoryTask* pCurrentTask=0;
        PlayerCreatedStoryArc *pPlayerArc =    playerCreatedStoryarc_Add((char*)unescapeAndUnpack(taskforce->playerCreatedArc), dbid, 0, 0, 0, 0 );

        if( taskforce->storyInfo->tasks[0]->sahandle.bPlayerCreated )
            taskforce->storyInfo->tasks[0]->sahandle.context = dbid;
        if( taskforce->storyInfo->storyArcs && taskforce->storyInfo->tasks[0] )
            pCurrentTask = TaskParentDefinition(&taskforce->storyInfo->tasks[0]->sahandle);
        if( pPlayerArc )
            playerCreatedStoryArc_SetContact( pPlayerArc, taskforce );
        if( pCurrentTask )
        {
            gArchitectMapMinLevel = pCurrentTask->minPlayerLevel;
            gArchitectMapMaxLevel = pCurrentTask->maxPlayerLevel;        
        }
    }
    fixupSouvenirClueReferences(taskforce->storyInfo);
    shrinkStoryInfoArrays(taskforce->storyInfo, 0);
    fixupMissionDoors(taskforce->storyInfo, isVillain);
    fixupClueReferences(taskforce->storyInfo);
    taskforce->storyInfo->taskforce = 1;
    
    for (i = eaSize(&taskforce->storyInfo->tasks - 1); i >= 0; i--)
    {
        StoryTaskInfo *task = taskforce->storyInfo->tasks[i];

        // Set up the zowie array
        // Note that zowie_setup checks we're on the correct map,
        // and that the task is a zowie task, so it's safe to call it for any task.
        zowie_setup(task, 0);
    }
}

void unpackTaskForce(Entity *e,char *mem,int send_to_client)
{
    s_unpackTaskForce(&e->taskforce, e->taskforce_id, mem, ENT_IS_VILLAIN(e));

    // Make sure the client gets updated to match the new challenge
    // information.
    e->tf_params_update = TRUE;
    e->teamlist_uptodate = 0;
}

void unpackMapTaskForce(int dbid, char *mem)
{
    assert(dbid == g_ArchitectTaskForce_dbid);
    s_unpackTaskForce(&g_ArchitectTaskForce, dbid, mem, 0);
}

void unpackLevelingPact(Entity *e, char *mem, int send_to_client)
{
    if(!e->levelingpact)
        e->levelingpact = calloc(1, sizeof(*e->levelingpact));
    else
        e->levelingpact->experience = 0;

    if(isDevelopmentMode())
    {
        FILE* f = fopen("c:\\levelingpact_fromdbserver.txt", "wb");
        if(f)
        {
            fwrite(mem, 1, strlen(mem), f);
            fclose(f);
        }
    }

    dbContainerUnpack(levelingpact_desc, mem, (char*)e->levelingpact);
}

#define MAX_PETITION_MSG    1024
#define MAX_SUMMARY            128

LineDesc petition_line_desc[] =
{
    {{ PACKTYPE_STR_UTF8, SIZEOF2(Entity,auth_name),             "AuthName",        OFFSET(Entity,auth_name),           },
        "Authname of player sending the petition"},

    {{ PACKTYPE_STR_UTF8, SIZEOF2(Entity,name),    "Name",            OFFSET(Entity,name), INOUT(0,0),0, MAX_NAME_LEN     },
        "Name of current character"},

    { PACKTYPE_FLOAT, SIZE_FLOAT32,            "PosX",            OFFSET(Entity, fork[3][0]) },
    { PACKTYPE_FLOAT, SIZE_FLOAT32,            "PosY",            OFFSET(Entity, fork[3][1]) },
    {{ PACKTYPE_FLOAT, SIZE_FLOAT32,            "PosZ",            OFFSET(Entity, fork[3][2]) },
        "Current location of the character in their current map."},

#define PETITION_ENT_COUNT 5
    {{ PACKTYPE_STR_UTF8, 256,            "MapName",         },
        "Name of the map the character is currently on."},

    {{ PACKTYPE_DATE,0,              "Date",            OFFSET(Entity,teamup_id),        INOUT(0,0),LINEDESCFLAG_INDEXEDCOLUMN },
        "Date the petition was entered"},

    {{ PACKTYPE_STR_UTF8, MAX_SUMMARY,    "Summary",         },
        "Brief description of the problem."},

    {{ PACKTYPE_STR_UTF8, MAX_PETITION_MSG,"Msg",             },
        "Description of the problem"},

    {{ PACKTYPE_INT, SIZE_INT8,        "Fetched",         },
        "TODO"},

    {{ PACKTYPE_INT, SIZE_INT8,        "Done",             },
        "TODO"},

    {{ PACKTYPE_INT, SIZE_INT8,        "Category",         },
        "The type of petition:<br>"
        "   0 = (NO LONGER USED) --> Bug Report<br>"
        "   1 = Stuck<br>"
        "   2 = Exploits and Cheating<br>"
        "   3 = Feedback and Suggestions<br>"
        "   4 = Harassment and Conduct<br>"
        "   5 = Technical Issues<br>"
        "   6 = General Help"},

    { 0 },
};

StructDesc petition_desc[] =
{
    sizeof(Entity),
    {AT_NOT_ARRAY,{0}},
    petition_line_desc,

    "TODO"
};

char *packagePetition(Entity *e, PetitionCategory cat, const char *summary, const char *msg)
{
    char    buf[200],*s;
    int        i;
    StuffBuff    sb;

    initStuffBuff(&sb,1000);

    for(i=0;i<PETITION_ENT_COUNT;i++)
        structToLine(&sb,(char*)e,0,petition_desc,&petition_line_desc[i],0);

    addStringToStuffBuff(&sb, "MapName \"%s\"\n",db_state.map_name);
    addStringToStuffBuff(&sb, "Date \"%s\"\n",timerMakeDateStringFromSecondsSince2000(buf,dbSecondsSince2000()));

    s = (char*)escapeString(summary);
    if (strlen(s) >= MAX_SUMMARY)
        s[MAX_SUMMARY-1] = 0;
    addStringToStuffBuff(&sb, "Summary \"%s\"\n",s);

    s = (char*)escapeString(msg);
    if (strlen(s) >= MAX_PETITION_MSG)
        s[MAX_PETITION_MSG-1] = 0;
    addStringToStuffBuff(&sb, "Msg \"%s\"\n",s);

    addStringToStuffBuff(&sb, "Category %d\n", cat);

    return sb.buff;
}


LineDesc offline_line_desc[] =
{
    // TODO: A brief description of this table. Include if this is a 1:1
    // or 1:n mapping to the ents table. See stat_line_desc below for an
    // example.

    {{ PACKTYPE_INT,            SIZE_INT32,        "AuthId",        },
        "TODO"},

    { 0 },
};

StructDesc offline_desc[] =
{
    0,
    {AT_NOT_ARRAY,{0}},
    offline_line_desc
};

StructDesc auctioninv_history_desc[] =
{
    // @todo -AB:  :09/27/06
    { 0 },
};

void containerWriteTemplates(char *dir)
{
    dbWriteAttributes(dir, "vars.attribute",attributeNames());

    // --------------------
    // badges

    // player
    dbWriteAttributes(dir, "badges.attribute",badge_Names(&g_BadgeDefs));
    dbWriteAttributes(dir, "badgestats.attribute",badge_StatNames(g_hashBadgeStatUsage));
    dbWriteAttributes(dir, "pophelp.attribute",popHelpTags());

    // supergroup
    dbWriteAttributes(dir, "supergroup_badges.attribute",badge_Names(&g_SgroupBadges));
    dbWriteAttributes(dir, "supergroup_badgestats.attribute",badge_StatNames(g_BadgeStatsSgroup.aIdxBadgesFromName));

    // --------------------
    // misc.

    dbWriteTemplate(dir,"testdatabasetypes.template",dbContainerTemplate(testdatabasetypes_desc));
    dbWriteTemplate(dir,"email.template",emailTemplate());
    dbWriteTemplate(dir,"ents.template",entTemplate()); // must be after badge_StatNames
    dbWriteTemplate(dir,"teamups.template",teamupTemplate());
    dbWriteTemplate(dir,"supergroups.template",superGroupTemplate());
    dbWriteTemplate(dir,"taskforces.template",taskForceTemplate());
    dbWriteTemplate(dir,"petitions.template",dbContainerTemplate(petition_desc));
    dbWriteTemplate(dir,"mapgroups.template",mapGroupTemplate());
    dbWriteTemplate(dir,"arenaevents.template",arenaEventTemplate());
    dbWriteTemplate(dir,"arenaplayers.template",arenaPlayerTemplate());
    dbWriteTemplate(dir,"baseraids.template",cstoreTemplate(g_ScheduledBaseRaidStore));
    dbWriteTemplate(dir,"sgraidinfos.template",cstoreTemplate(g_SupergroupRaidInfoStore));
    dbWriteTemplate(dir,"base.template",dbContainerTemplate(base_desc));
    dbWriteTemplate(dir,"statserver_supergroupstats.template",stat_SgrpStats_Template());
    dbWriteTemplate(dir,"itemofpowergames.template",cstoreTemplate(g_ItemOfPowerGameStore));
    dbWriteTemplate(dir,"itemsofpower.template",cstoreTemplate(g_ItemOfPowerStore));
    dbWriteTemplate(dir,"offline.template",dbContainerTemplate(offline_desc));
    dbWriteTemplate(dir,"miningaccumulator.template",MiningAccumulatorTemplate());
//     dbWriteTemplate(dir,"raids.template",dbContainerTemplate(raid_desc));
    dbWriteTemplate(dir,"levelingpacts.template",dbContainerTemplate(levelingpact_desc));
    dbWriteTemplate(dir,"leagues.template",dbContainerTemplate(league_desc));
    dbWriteTemplate(dir,"eventhistory.template",dbContainerTemplate(eventhistory_desc));
    dbWriteTemplate(dir,"autocommands.template",dbContainerTemplate(autocommand_desc));
    dbWriteTemplate(dir,"shardaccounts.template",dbContainerTemplate(shardaccount_desc));

    dbWriteSchema(dir,"testdatabasetypes.schema.html",dbContainerSchema(testdatabasetypes_desc, "TestDataBaseTypes"));
    dbWriteSchema(dir,"email.schema.html",emailSchema());
    dbWriteSchema(dir,"ents.schema.html",entSchema()); // must be after badge_StatNames
    dbWriteSchema(dir,"teamups.schema.html",teamupSchema());
    dbWriteSchema(dir,"supergroups.schema.html",superGroupSchema());
    dbWriteSchema(dir,"taskforces.schema.html",taskForceSchema());
    dbWriteSchema(dir,"petitions.schema.html",dbContainerSchema(petition_desc, "Petitions"));
    dbWriteSchema(dir,"mapgroups.schema.html",mapGroupSchema());
    dbWriteSchema(dir,"arenaevents.schema.html",arenaEventSchema());
    dbWriteSchema(dir,"arenaplayers.schema.html",arenaPlayerSchema());
    dbWriteSchema(dir,"baseraids.schema.html",cstoreSchema(g_ScheduledBaseRaidStore, "BaseRaids"));
    dbWriteSchema(dir,"sgraidinfos.schema.html",cstoreSchema(g_SupergroupRaidInfoStore, "SGRaidInfos"));
    dbWriteSchema(dir,"base.schema.html",dbContainerSchema(base_desc, "Base"));
//    No longer supporting schema for statserver_supergroupstats... so there.
//    dbWriteSchema(dir,"statserver_supergroupstats.schema.html", cstoreSchema(g_stat_SgrpStatsStore, "statserver_supergroupstats"));
    dbWriteSchema(dir,"itemofpowergames.schema.html",cstoreSchema(g_ItemOfPowerGameStore, "ItemOfPowerGames"));
    dbWriteSchema(dir,"itemsofpower.schema.html",cstoreSchema(g_ItemOfPowerStore, "ItemsOfPower"));
    dbWriteSchema(dir,"offline.schema.html",dbContainerSchema(offline_desc, "Offline"));
    dbWriteSchema(dir,"miningaccumulator.schema.html",MiningAccumulatorSchema());
//     dbWriteSchema(dir,"raids.schema.html",dbContainerSchema(raid_desc, "Raids"));
    dbWriteSchema(dir,"levelingpacts.schema.html",dbContainerSchema(levelingpact_desc, "LevelingPacts"));
    dbWriteSchema(dir,"leagues.schema.html",dbContainerSchema(league_desc, "Leagues"));
    dbWriteSchema(dir,"eventhistory.schema.html",dbContainerSchema(eventhistory_desc, "EventHistory"));
    dbWriteSchema(dir,"autocommands.schema.html",dbContainerSchema(autocommand_desc, "AutoCommands"));
    dbWriteSchema(dir,"shardaccounts.schema.html",dbContainerSchema(shardaccount_desc, "ShardAccounts"));

    inventory_WriteDbIds();
    proficiency_WriteDbIds();
}


void morphCharacterFile(ClientLink *client, Entity *e, char *fname)
{
    char *mem;

    mem = fileAlloc(fname, 0);
    if(!mem)
    {
        conPrintf(client, "Unable to find file: %s", fname);
        return;
    }
    else
    {
        // cleanup storyarcs
        StoryArcInfoReset(e);

        // load up new character
        unpackEnt(e, mem);

        // swap to first costume slot
        costume_change(e, 0);

        character_ActivateAllAutoPowers(e->pchar);

        // make good...
        entSetOnTeamHero(e, 1);

        // force client reload
        client->ready = CLIENTSTATE_RE_REQALLENTS;
    }

    free(mem);
}


void morphCharacter(ClientLink* client, Entity *e, char *at, char *pri_power, char *sec_power, int level)
{
    char    fname[256];

    if(strcmp(at,"common")==0)
    {
        sprintf(fname, "%s", pri_power);
    }
    else
    {
        if (!clientLinkGetDebugVar(client, "MorphLevel"))
        {
            return;
        }

        sprintf(fname, "n:/nobackup/resource/characters/%s/%s%s/%d.txt", at, pri_power, sec_power,
            (int)(intptr_t)clientLinkGetDebugVar(client, "MorphLevel"));
    }
    morphCharacterFile(client, e, fname);
}


extern StructDesc supergroup_desc[];
extern StructDesc ItemOfPower_desc[];

StructDesc *getStructDesc(int type, char* table)
{
    if (type == CONTAINER_ENTS)
    {
        if (strnicmp(table,"var",3)==0)
        {
            return NULL;
        }
        else if(strnicmp(table,"Powers",6)==0
            || strnicmp(table,"Boosts",6)==0
            || strnicmp(table,"Inspirations",12)==0
            || strnicmp(table,"AttribMods",10)==0)
        {
            return powers_desc;
        }
        else if(strnicmp(table,"Stats",5)==0)
        {
            return stats_desc;
        }
        else if(strnicmp(table,"CostumeParts",12)==0)
        {
            return costumes_desc;
        }
        else if(strnicmp(table,"Appearance",10)==0)
        {
            return appearances_desc;
        }
        else if(strnicmp(table,"PowerCustomizations", 19) == 0)
        {
            return powercustomization_list_desc;
        }
        else if(strnicmp(table,"Badges",6)==0)
        {
            // Badges and inventory don't use normal StructDescs, so this won't work
            return NULL;
        }
        else if( inventory_typeFromTable(table) != kInventoryType_Count)
        {
            return NULL;
        }
        else
        {
            return ent_desc;
        }
    }
    else if (type == CONTAINER_SUPERGROUPS)
    {
        return supergroup_desc;
    }
    else if (type == CONTAINER_BASE)
    {
        return base_desc;
    }
    else if (type == CONTAINER_ITEMSOFPOWER)
    {
        return ItemOfPower_desc;
    }
    else if (type == CONTAINER_SGRPSTATS)
    {
        // Badgesstats don't use normal structs, this won't work
        return NULL;
    }

    return NULL;
}

