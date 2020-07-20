/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "Reward.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/rand.h>
#include "container/team.h"
#include "bases/basedata.h"
#include "bases/detailrecipe.h"
#include "entity/SgrpRewards.h"
#include "entity/Supergroup.h"
#include "entity/RewardSlot.h"
#include "entity/Character_base.h"
#include "entity/character_inventory.h"
#include "entity/Character_animfx.h"
#include "entity/character_eval.h"
#include "entity/Entity.h"
#include "gameComm/NpcServer.h"
#include "gameComm/villainDef.h"
#include "gameComm/sendToClient.h"
#include "entity/entVarUpdate.h"    // For enum INFO_BOX_MSGS
#include "entity/powers.h"
#include "entity/character_level.h"
#include "entity/entGameActions.h"
#include "entity/origins.h"
#include "cmdparse/cmdserver.h"
#include <utilitieslib/utils/error.h>
#include "dbcomm/dbcomm.h"   // for dbLog()
#include "player/pl_stats.h"
#include "entity/PowerInfo.h"
#include "TeamReward.h"
#include <utilitieslib/utils/utils.h>
#include "storyarc/storyarcinterface.h"
#include "player/badges_server.h"
#include "entity/entPlayer.h"
#include "comm_game.h"
#include "svr/svr_base.h"
#include <utilitieslib/utils/error.h>
#include "player/badges.h"
#include "player/badges_server.h"
#include "dbghelper.h"
#include "entity/costume.h"
#include "entity/buddy_server.h"
#include "language/langServerUtil.h"
#include "auth/authUserData.h"
#include <utilitieslib/components/SharedMemory.h>
#include "gamesys/arenamapserver.h"
#include "storyarc/storyinfo.h"
#include "storyarc/contact.h"
#include "entity/Salvage.h"
#include "entity/Concept.h"
#include "entity/Proficiency.h"
#include "container/containerInventory.h"
#include "entity/SgrpServer.h"
#include "bases/bases.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/ScriptVars.h>
#include "gameData/mininghelper.h"
#include <utilitieslib/components/estring.h>
#include "storyarc/task.h"
#include "entity/character_combat.h"
#include "entity/character_mods.h"
#include "gameSys/DayJob.h"
#include "cmdparse/cmdstatserver.h"    //required for leveling pact stat server communication
#include "storyarc/playerCreatedStoryarcServer.h"
#include <stdlib.h>
#include <string.h>
#include "player/pophelp.h"
#include "dbcomm/staticMapInfo.h"
#include "dbcomm/logcomm.h"
#include "gameComm/svr_chat.h"
#include "storyarc/contactCommon.h"
#include "entity/alignment_shift.h"
#include "account/AccountTypes.h"
#include "account/AccountCatalog.h"

int g_NoRewardMap = 0;

extern StuffBuff s_sbRewardLog;

extern StaticDefine ParseIncarnateDefines[]; // defined in load_def.c

const char* gRewardSourceLabels[REWARDSOURCE_COUNT] = {
    "undefined_source",
    "drop",
    "encounter",
    "story",
    "task",
    "mission",
    "event",
    "arena",
    "script",
    "pnpc_auto",
    "npc_on_click",
    "recipe",
    "invention_recipe",
    "badge",
    "badge_button",
    "attribmod",
    "csr",
    "devchoice",
    "tf_weekly",
    "salvage_open",
    "choice",
    "debug",
};

bool rewardtokentype_Valid( RewardTokenType e )
{
    return INRANGE0(e, kRewardTokenType_Count);
}

StaticDefineInt rewardTokenFlags[] = {
    DEFINE_INT
    { "Player", kRewardTokenType_Player },
    { "Sgrp", kRewardTokenType_Sgrp },
    { "ActivePlayer", kRewardTokenType_ActivePlayer },
    DEFINE_END
};

//------------------------------------------------------------------------
// Reward parsing
//------------------------------------------------------------------------
typedef struct RewardTable
{
    const char *name;         // name of this table
    const char *filename;        // where the data came from (debugging only)
    RewardTokenType    permanentRewardToken;
    bool randomItemOfPower;
    const RewardDef** ppDefs; // rewards defs for each level
    int    verified;        // shortcut for verification purposes
} RewardTable;

typedef struct RewardDictionary
{
    const RewardTable **ppTables;
    cStashTable hashRewardTable;
} RewardDictionary;

typedef struct ItemSetDictionary
{
    const RewardItemSet **ppItemSets;
    cStashTable hashItemSets;
} ItemSetDictionary;

typedef struct RewardChoiceDictionary
{
    const RewardChoiceSet **ppChoiceSets;
    cStashTable hashRewardChoice;
}RewardChoiceDictionary;

//////////////////////////////////////////////////////////////

typedef struct MeritRewardStoryArc
{
    const char *name;
    int    merits;
    int    failedmerits;
    const char *rewardToken;
    const char *bonusOncePerEpochTable;
    const char *bonusReplayTable;
    const char *bonusToken;
    int bonusMerits;
} MeritRewardStoryArc;

typedef struct MeritRewardDictionary
{
    const MeritRewardStoryArc **ppStoryArc;
    cStashTable hashMeritReward;
} MeritRewardDictionary;


typedef struct GlobalRewardDefinition
{
    const char *name;
    const char *path;
    const char *itemSetSM;
    const char *itemSetBin;
    const char *rewardSM;
    const char *rewardBin;
    const char *rewardChoiceSM;
    const char *rewardChoiceBin;
    
    ItemSetDictionary        itemSetDictionary;
    RewardDictionary        rewardDictionary;
    RewardChoiceDictionary    rewardChoiceDictionary;
    MeritRewardDictionary    meritRewardDictionary;

} GlobalRewardDefinition;

SHARED_MEMORY GlobalRewardDefinition g_RewardDefinitions[] =
{
    {
        "Invention",
        "defs/reward",
        "SM_ItemSets",
        "ItemSets.bin",
        "SM_RewardTables",
        "RewardTables.bin",
        "SM_RewardChoiceTables",
        "RewardChoiceTables.bin",
        NULL,
        NULL,
        NULL,
        NULL
    }
};
SHARED_MEMORY GlobalRewardDefinition *g_RewardDefinition = g_RewardDefinitions;
int                        g_RewardDefinitionIdx = 0;

StaticDefineInt AddRemoveEnum[] =
{
    DEFINE_INT
    { "Power", 0 },
    { "Add", 0 },
    { "RemovePower", 1 },
    { "Remove", 1 },
    DEFINE_END
};

TokenizerParseInfo ParsePowerRewardItemChance[] =
{
    {    "Chance",            TOK_STRUCTPARAM | TOK_INT(RewardItem, chance, 0),               },
    {    "AddRemove",        TOK_STRUCTPARAM | TOK_INT(RewardItem, power.remove, 0), AddRemoveEnum},
    {    "PowerCategory",    TOK_STRUCTPARAM | TOK_STRING(RewardItem, power.powerCategory, 0)   },
    {    "PowerSet",            TOK_STRUCTPARAM | TOK_STRING(RewardItem, power.powerSet, 0)        },
    {    "Power",            TOK_STRUCTPARAM | TOK_STRING(RewardItem, power.power,   0)           },
    {    "Level",            TOK_STRUCTPARAM | TOK_INT(RewardItem, power.level,      0) },
    {    "FixedLevel",        TOK_STRUCTPARAM | TOK_INT(RewardItem, power.fixedlevel, 0) },

    // -AB: this type should always be omitted, type is implicit, put new entries above this :2005 Feb 17 03:13 PM
    {    "Type",                TOK_INT(RewardItem, type, kRewardItemType_Power) },
    // these fields are here so the tokenizer persists them, but they are not used in a power
    // reward item. This is necessary because multiple reward item definitions use the same
    // type to convey reward information
    {    "Pad1",                TOK_INT(RewardItem, padField1, 0) },
    {    "Pad2",                TOK_INT(RewardItem, padField2, 0) },
    {    "ItemSetAddRemove",    TOK_INT(RewardItem, remove, 0), AddRemoveEnum},
    {    "\n",                TOK_END,                                0 },
    {    "", 0, 0 }
};


TokenizerParseInfo ParsePowerRewardItemChanceFixedLevel[] =
{
    {    "Chance",            TOK_STRUCTPARAM | TOK_INT(RewardItem, chance, 0),               },
    {    "PowerCategory",    TOK_STRUCTPARAM | TOK_STRING(RewardItem, power.powerCategory, 0)   },
    {    "PowerSet",            TOK_STRUCTPARAM | TOK_STRING(RewardItem, power.powerSet, 0)        },
    {    "Power",            TOK_STRUCTPARAM | TOK_STRING(RewardItem, power.power, 0)           },
    {    "Level",            TOK_STRUCTPARAM | TOK_INT(RewardItem, power.level,        0) },
    {    "FixedLevel",        TOK_STRUCTPARAM | TOK_INT(RewardItem, power.fixedlevel,   1) },
    {    "AddRemove",        TOK_STRUCTPARAM | TOK_INT(RewardItem, power.remove,       0), AddRemoveEnum},

    // -AB: this type should always be omitted :2005 Feb 17 03:13 PM
    {    "Type",                TOK_INT(RewardItem, type,             kRewardItemType_Power) },
    // these fields are here so the tokenizer persists them, but they are not used in a power
    // reward item. This is necessary because multiple reward item definitions use the same
    // type to convey reward information
    {    "Pad1",                TOK_INT(RewardItem, padField1, 0) },
    {    "Pad2",                TOK_INT(RewardItem, padField2, 0) },
    {    "ItemSetAddRemove",    TOK_INT(RewardItem, remove, 0), AddRemoveEnum},
    {    "\n",                TOK_END,                            0},
    {    "", 0, 0 }
};

TokenizerParseInfo ParsePowerRewardItemAdd[] =
{
    {    "PowerCategory",    TOK_STRUCTPARAM | TOK_STRING(RewardItem, power.powerCategory, 0)   },
    {    "PowerSet",            TOK_STRUCTPARAM | TOK_STRING(RewardItem, power.powerSet, 0)        },
    {    "Power",            TOK_STRUCTPARAM | TOK_STRING(RewardItem, power.power, 0)           },
    {    "Level",            TOK_STRUCTPARAM | TOK_INT(RewardItem, power.level,        0) },
    {    "Chance",            TOK_STRUCTPARAM | TOK_INT(RewardItem, chance,             1) },
    {    "FixedLevel",        TOK_STRUCTPARAM | TOK_INT(RewardItem, power.fixedlevel,   0) },
    {    "AddRemove",        TOK_STRUCTPARAM | TOK_INT(RewardItem, power.remove,       0), AddRemoveEnum},

    // -AB: this type should always be omitted :2005 Feb 17 03:13 PM
    {    "Type",                TOK_INT(RewardItem, type,             kRewardItemType_Power) },
    // these fields are here so the tokenizer persists them, but they are not used in a power
    // reward item. This is necessary because multiple reward item definitions use the same
    // type to convey reward information
    {    "Pad1",                TOK_INT(RewardItem, padField1, 0) },
    {    "Pad2",                TOK_INT(RewardItem, padField2, 0) },
    {    "ItemSetAddRemove",    TOK_INT(RewardItem, remove, 0), AddRemoveEnum},
    {    "\n",                TOK_END,                            0},
    {    "", 0, 0 }
};

TokenizerParseInfo ParsePowerRewardItemRemove[] =
{
    {    "PowerCategory",    TOK_STRUCTPARAM | TOK_STRING(RewardItem, power.powerCategory, 0)   },
    {    "PowerSet",            TOK_STRUCTPARAM | TOK_STRING(RewardItem, power.powerSet, 0)        },
    {    "Power",            TOK_STRUCTPARAM | TOK_STRING(RewardItem, power.power, 0)           },
    {    "Level",            TOK_STRUCTPARAM | TOK_INT(RewardItem, power.level,        0) },
    {    "FixedLevel",        TOK_STRUCTPARAM | TOK_INT(RewardItem, power.fixedlevel,   0) },
    {    "Chance",            TOK_STRUCTPARAM | TOK_INT(RewardItem, chance,             1) },
    {    "AddRemove",        TOK_STRUCTPARAM | TOK_INT(RewardItem, power.remove,       1), AddRemoveEnum},
    {    "\n",                TOK_END,                            0},
    {    "", 0, 0 }
};

//------------------------------------------------------------
//  Helper for the reward items that share common characteristics:
// - Salvage 10 Scrap
// - Concept 20 Dynamite
// - Proficiency 71 Gadget
// NOTE: the 'type' element is not a STRUCTPARAM, and is there for the
//       automatic value that it takes.
//----------------------------------------------------------
#define GENERIC_REWARDITEM_PARSEINFO(TYPE) \
    {    "Chance",    TOK_STRUCTPARAM | TOK_INT(RewardItem, chance,1) }, \
    {    "Name",        TOK_STRUCTPARAM | TOK_STRING(RewardItem, itemName, 0) }, \
    {    "Type",        TOK_INT(RewardItem, type, TYPE)                        }, \
    {    "\n",        TOK_END,                            0                }

TokenizerParseInfo ParseTokenRewardItemRemove[] =
{
    GENERIC_REWARDITEM_PARSEINFO(kRewardItemType_Token),
    {    "AddRemove",    TOK_STRUCTPARAM | TOK_INT(RewardItem, remove, 1), AddRemoveEnum},
    {    "", 0, 0 }
};

static TokenizerParseInfo ParseSalvageRewardItem[] =
{
    GENERIC_REWARDITEM_PARSEINFO(kRewardItemType_Salvage),
    {    "Count", TOK_STRUCTPARAM | TOK_INT(RewardItem, count, 1) },
    {    "", 0, 0 }
};

static TokenizerParseInfo ParseConceptRewardItem[] =
{
    GENERIC_REWARDITEM_PARSEINFO(kRewardItemType_Concept),
    {    "", 0, 0 }
};

static TokenizerParseInfo ParseProficiencyRewardItem[] =
{
    GENERIC_REWARDITEM_PARSEINFO(kRewardItemType_Proficiency),
    {    "", 0, 0 }
};

StaticDefineInt rewardDetailRecipeFlags[] = {
    DEFINE_INT
    { "unlimited", 1 },
    DEFINE_END
};

StaticDefineInt rewardRewardTableFlags[] = {
    DEFINE_INT
    { "AtCritterLevel", kRewardTableFlags_AtCritterLevel },
    DEFINE_END
};

static TokenizerParseInfo ParseDetailRecipeRewardItem[] =
{
    GENERIC_REWARDITEM_PARSEINFO(kRewardItemType_DetailRecipe),
    {    "Count", TOK_STRUCTPARAM | TOK_INT(RewardItem, count, 0) },
    {    "Unlimited", TOK_STRUCTPARAM | TOK_FLAGS(RewardItem, unlimited, 0), rewardDetailRecipeFlags },
    {    "", 0, 0 }
};

static TokenizerParseInfo ParseDetailRewardItem[] =
{
    GENERIC_REWARDITEM_PARSEINFO(kRewardItemType_Detail),
    {    "", 0, 0 }
};


static TokenizerParseInfo ParseRewardTableRewardItem[] =
{
    GENERIC_REWARDITEM_PARSEINFO(kRewardItemType_RewardTable),
    {    "RewardTableFlags", TOK_STRUCTPARAM | TOK_FLAGS(RewardItem, rewardTableFlags, 0), rewardRewardTableFlags },
    {    "", 0, 0 }
};

static TokenizerParseInfo ParseRewardTokenCountRewardItem[] =
{
    GENERIC_REWARDITEM_PARSEINFO(kRewardItemType_RewardTokenCount),
    {    "Count", TOK_STRUCTPARAM | TOK_INT(RewardItem, count, 0) },
    {    "", 0, 0 }
};

static TokenizerParseInfo ParseIncarnatePointsRewardItem[] =
{
    GENERIC_REWARDITEM_PARSEINFO(kRewardItemType_IncarnatePoints),
    { "Count", TOK_STRUCTPARAM | TOK_INT(RewardItem, count, 0) },
    { "", 0, 0 }
};

static TokenizerParseInfo ParseAccountProductRewardItem[] =
{
    GENERIC_REWARDITEM_PARSEINFO(kRewardItemType_AccountProduct),
    { "Count", TOK_STRUCTPARAM | TOK_INT(RewardItem, count, 0) },
    { "Rarity", TOK_STRUCTPARAM | TOK_INT(RewardItem, rarity, 0), AccountTypes_RarityEnum },
    { "", 0, 0 }
};

TokenizerParseInfo ParseChanceRequiresItem[] =
{
    {    "Requires",                    TOK_STRINGARRAY( RewardItem, chanceRequires),             },
    {    "Chance",                    TOK_NULLSTRUCT(ParsePowerRewardItemChance)},
    {    "Power",                    TOK_REDUNDANTNAME | TOK_NULLSTRUCT(ParsePowerRewardItemAdd)},
    {    "RemovePower",                TOK_REDUNDANTNAME | TOK_NULLSTRUCT(ParsePowerRewardItemRemove)},
    {    "Salvage",                    TOK_REDUNDANTNAME | TOK_NULLSTRUCT(ParseSalvageRewardItem)},
    {    "Concept",                    TOK_REDUNDANTNAME | TOK_NULLSTRUCT(ParseConceptRewardItem)},
    {    "Proficiency",                TOK_REDUNDANTNAME | TOK_NULLSTRUCT(ParseProficiencyRewardItem)},
    {    "DetailRecipe",                TOK_REDUNDANTNAME | TOK_NULLSTRUCT(ParseDetailRecipeRewardItem)},
    {    "Recipe",                    TOK_REDUNDANTNAME | TOK_NULLSTRUCT(ParseDetailRecipeRewardItem)},
    {    "Detail",                    TOK_REDUNDANTNAME | TOK_NULLSTRUCT(ParseDetailRewardItem)},
    {    "RemoveToken",                TOK_REDUNDANTNAME | TOK_NULLSTRUCT(ParseTokenRewardItemRemove)},
    {    "RewardTable",                TOK_REDUNDANTNAME | TOK_NULLSTRUCT(ParseRewardTableRewardItem)},
    {    "TokenCount",                TOK_REDUNDANTNAME | TOK_NULLSTRUCT(ParseRewardTokenCountRewardItem)},
    {    "IncarnatePoints",            TOK_REDUNDANTNAME | TOK_NULLSTRUCT(ParseIncarnatePointsRewardItem)},
    {    "AccountProduct",            TOK_REDUNDANTNAME | TOK_NULLSTRUCT(ParseAccountProductRewardItem)},
    {    "FixedLevelEnh",            TOK_REDUNDANTNAME | TOK_NULLSTRUCT(ParsePowerRewardItemChanceFixedLevel)},
    {    "FixedLevelEnhancement",    TOK_REDUNDANTNAME | TOK_NULLSTRUCT(ParsePowerRewardItemChanceFixedLevel)},
    {    "{",                        TOK_START,        0 },
    {    "}",                        TOK_END,        0 },
    {    "", 0, 0 }
};


//------------------------------------------------------------
// parse a RewardItemSet
// see ParsePowerRewardItemChance, where it is called 'ItemSet'
// see ParseRewardItemSet, where it is called 'ItemSetDef'
// currently the 'Chance' member controls writing these members
//to memory and the bin file, i
// -AB: adding salvage :2005 Feb 17 02:55 PM
//----------------------------------------------------------
TokenizerParseInfo ParseRewardItemSet[] =
{
    {    "Name",                        TOK_STRUCTPARAM | TOK_STRING(RewardItemSet, name, 0) },
    {    "Filename",                    TOK_CURRENTFILE(RewardItemSet, filename) },
    {    "Verified",                    TOK_INT(RewardItemSet, verified, 0) },
    {    "Chance",                    TOK_STRUCT(RewardItemSet, rewardItems, ParsePowerRewardItemChance)},
    {    "Power",                    TOK_REDUNDANTNAME | TOK_STRUCT(RewardItemSet, rewardItems, ParsePowerRewardItemAdd)},
    {    "RemovePower",                TOK_REDUNDANTNAME | TOK_STRUCT(RewardItemSet, rewardItems, ParsePowerRewardItemRemove)},
    {    "Salvage",                    TOK_REDUNDANTNAME | TOK_STRUCT(RewardItemSet, rewardItems, ParseSalvageRewardItem)},
    {    "Concept",                    TOK_REDUNDANTNAME | TOK_STRUCT(RewardItemSet, rewardItems, ParseConceptRewardItem)},
    {    "Proficiency",                TOK_REDUNDANTNAME | TOK_STRUCT(RewardItemSet, rewardItems, ParseProficiencyRewardItem)},
    {    "DetailRecipe",                TOK_REDUNDANTNAME | TOK_STRUCT(RewardItemSet, rewardItems, ParseDetailRecipeRewardItem)},
    {    "Recipe",                    TOK_REDUNDANTNAME | TOK_STRUCT(RewardItemSet, rewardItems, ParseDetailRecipeRewardItem)},
    {    "Detail",                    TOK_REDUNDANTNAME | TOK_STRUCT(RewardItemSet, rewardItems, ParseDetailRewardItem)},
    {    "RemoveToken",                TOK_REDUNDANTNAME | TOK_STRUCT(RewardItemSet, rewardItems, ParseTokenRewardItemRemove)},
    {    "RewardTable",                TOK_REDUNDANTNAME | TOK_STRUCT(RewardItemSet, rewardItems, ParseRewardTableRewardItem)},
    {    "TokenCount",                TOK_REDUNDANTNAME | TOK_STRUCT(RewardItemSet, rewardItems, ParseRewardTokenCountRewardItem)},
    {    "IncarnatePoints",            TOK_REDUNDANTNAME | TOK_STRUCT(RewardItemSet, rewardItems, ParseIncarnatePointsRewardItem)},
    {    "AccountProduct",            TOK_REDUNDANTNAME | TOK_STRUCT(RewardItemSet, rewardItems, ParseAccountProductRewardItem)},
    {    "FixedLevelEnh",            TOK_REDUNDANTNAME | TOK_STRUCT(RewardItemSet, rewardItems, ParsePowerRewardItemChanceFixedLevel)},
    {    "FixedLevelEnhancement",    TOK_REDUNDANTNAME | TOK_STRUCT(RewardItemSet, rewardItems, ParsePowerRewardItemChanceFixedLevel)},
    {    "ChanceRequires",            TOK_STRUCT(RewardItemSet, rewardItems, ParseChanceRequiresItem)},
    {    "{",                        TOK_START,        0 },
    {    "}",                        TOK_END,        0 },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseRewardDropGroup[] =
{
    {    "Chance",        TOK_STRUCTPARAM | TOK_INT(RewardDropGroup, chance, 0)},
    {    "ItemSetName",    TOK_STRINGARRAY(RewardDropGroup,itemSetNames) },
    {    "ItemSet",        TOK_STRUCT(RewardDropGroup, itemSets, ParseRewardItemSet)},
    {    "{",            TOK_START,        0 },
    {    "}",            TOK_END,            0 },
    {    "", 0, 0 }
};

StaticDefineInt rewardSetFlags[] = {
    DEFINE_INT
    { "always",        RSF_ALWAYS },
    { "everyone",    RSF_EVERYONE },
    { "nolimit",    RSF_NOLIMIT },
    { "forced",        RSF_FORCED },
    DEFINE_END
};


TokenizerParseInfo ParseRewardSet[] =
{
    {    "Filename",            TOK_CURRENTFILE(RewardSet, filename) },
    {    "Chance",            TOK_STRUCTPARAM | TOK_F32(RewardSet, chance, 0)},
    {    "Flags",            TOK_STRUCTPARAM | TOK_FLAGS(RewardSet, flags, 0), rewardSetFlags},
    {    "{",                TOK_START,        0 },
    {    "DefaultOrigin",    TOK_BOOLFLAG(RewardSet, catchUnlistedOrigins, 0)},
    {    "Origin",            TOK_STRINGARRAY(RewardSet, origins)},
    {    "DefaultArchetype",    TOK_BOOLFLAG(RewardSet, catchUnlistedArchetypes, 0)},
    {    "Archetype",        TOK_STRINGARRAY(RewardSet, archetypes)},
    {    "DefaultVillainGroup",    TOK_BOOLFLAG(RewardSet, catchUnlistedVGs, 0)},
    {    "VillainGroup",        TOK_INTARRAY(RewardSet, groups), ParseVillainGroupEnum},
    {    "Requires",            TOK_STRINGARRAY(RewardSet, rewardSetRequires)},
    {    "Experience",        TOK_INT(RewardSet, experience, 0)},
    {   "BonusExperience",    TOK_F32(RewardSet, bonus_experience, 0)},
    {    "Wisdom",            TOK_INT(RewardSet, wisdom, 0)},
    {    "Influence",        TOK_INT(RewardSet, influence, 0)},
//    {    "RewardTable",        TOK_STRINGARRAY(RewardSet, rewardTable)}, // ARM NOTE: This appears to be unused, so I'm pulling it.  Revert if I'm wrong.
    {    "Prestige",            TOK_INT(RewardSet, prestige, 0)},
    {    "IncarnateSubtype", TOK_STRUCTPARAM | TOK_FLAGS(RewardSet, incarnateSubtype, 0), ParseIncarnateDefines },
    {    "IncarnateCount",    TOK_INT(RewardSet, incarnateCount, 0)},
    {    "IgnoreRewardCaps",    TOK_BOOLFLAG(RewardSet, bIgnoreRewardCaps, 0)},
    {    "LeagueOnly",        TOK_BOOLFLAG(RewardSet, bLeagueOnly, 0)},
    {    "SuperDropGroup",    TOK_STRUCT(RewardSet, sgrpDropGroups, ParseRewardDropGroup)},
    {    "DropGroup",        TOK_STRUCT(RewardSet, dropGroups, ParseRewardDropGroup)},
    {    "}",                TOK_END,            0 },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseRewardDef[] =
{

    {    "Level",        TOK_STRUCTPARAM | TOK_INT(RewardDef, level, 0)},
    {    "{",            TOK_START,        0 },
    {    "Chance",        TOK_STRUCT(RewardDef, rewardSets, ParseRewardSet)},
    {    "}",            TOK_END,            0 },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseRewardTable[] =
{
    {    "Name",                    TOK_STRUCTPARAM | TOK_STRING(RewardTable, name, 0)},
    {    "{",                    TOK_START,        0 },
    {    "PermanentRewardToken",    TOK_FLAGS(RewardTable, permanentRewardToken, 0), rewardTokenFlags },
    {    "RandomItemOfPower",    TOK_BOOLFLAG(RewardTable, randomItemOfPower, 0) },
    {   "dataFilename",            TOK_CURRENTFILE(RewardTable,filename) },
    {    "Verified",                TOK_INT(RewardTable, verified, 0)},
    {    "Level",                TOK_STRUCT(RewardTable, ppDefs, ParseRewardDef) },
    {    "}",                    TOK_END,        0 },
    { 0 }
};

TokenizerParseInfo ParseRewardChoiceRequirement[] =
{
    {    "{",            TOK_START,    0    },
    {    "Requires",        TOK_STRINGARRAY(RewardChoiceRequirement, ppchRequires)    },
    {    "Warning",        TOK_STRING(RewardChoiceRequirement, pchWarning, 0)    },
    {    "}",            TOK_END,    0    },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseRewardChoice[] =
{
    {    "{",                TOK_START,    0                                        },
    {    "Description",        TOK_STRING(RewardChoice, pchDescription, 0)    },
    {    "Watermark",        TOK_STRING(RewardChoice, pchWatermark, 0) },
    {    "RewardTable",        TOK_STRINGARRAY(RewardChoice, ppchRewardTable)    },
    {    "Requirement",        TOK_STRUCT(RewardChoice, ppRequirements, ParseRewardChoiceRequirement)    },
    {    "VisibleRequirement",        TOK_STRUCT(RewardChoice, ppVisibleRequirements, ParseRewardChoiceRequirement)    },
    {    "RewardTableFlags", TOK_STRUCTPARAM | TOK_FLAGS(RewardChoice, rewardTableFlags, 0), rewardRewardTableFlags },
    {    "}",                TOK_END,        0                                        },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseRewardChoiceSet[] =
{
    {    "Name",            TOK_STRUCTPARAM | TOK_STRING(RewardChoiceSet, pchName, 0)},
    {    "Filename",        TOK_CURRENTFILE(RewardChoiceSet, pchSourceFile)},
    {    "Desc",            TOK_STRING(RewardChoiceSet, pchDesc, 0)},
    {    "UnusedInitializer",    TOK_INT(RewardChoiceSet, isMoralChoice, 0)},
    {    "DisableChooseNothing",    TOK_BOOLFLAG(RewardChoiceSet, disableChooseNothing, 0)},
    {    "{",            TOK_START,        0 },
    {    "Choice",        TOK_STRUCT(RewardChoiceSet, ppChoices, ParseRewardChoice) },
    {    "}",            TOK_END,            0 },
    { 0 }
};

TokenizerParseInfo ParseRewardMoralChoiceSet[] =
{
    {    "Name",            TOK_STRUCTPARAM | TOK_STRING(RewardChoiceSet, pchName, 0)},
    {    "Desc",            TOK_STRING(RewardChoiceSet, pchDesc, 0)},
    {    "UnusedInitializer",    TOK_INT(RewardChoiceSet, isMoralChoice, 1)},
    {    "DisableChooseNothing",    TOK_BOOLFLAG(RewardChoiceSet, disableChooseNothing, 0)},
    {    "{",            TOK_START,        0 },
    {    "Choice",        TOK_STRUCT(RewardChoiceSet, ppChoices, ParseRewardChoice) },
    {    "}",            TOK_END,            0 },
    { 0 }
};

TokenizerParseInfo ParseRewardDictionary[] =
{
    {    "RewardTable", TOK_STRUCT(RewardDictionary, ppTables, ParseRewardTable) },
    { 0 }
};

TokenizerParseInfo ParseItemSetDictionary[] =
{
    {    "ItemSetDef", TOK_STRUCT(ItemSetDictionary, ppItemSets, ParseRewardItemSet) },
    { 0 }
};

TokenizerParseInfo ParseRewardChoiceDictionary[] =
{
    {    "RewardChoiceTable", TOK_STRUCT(RewardChoiceDictionary, ppChoiceSets, ParseRewardChoiceSet) },
    {    "RewardMoralChoiceTable", TOK_STRUCT(RewardChoiceDictionary, ppChoiceSets, ParseRewardMoralChoiceSet) },
    { 0 }
};

////////////////////////////////////////////////////////////////////////////////////////
// storyarc.merits

TokenizerParseInfo ParseMeritRewardStoryArc[] =
{
    {    "Name",                        TOK_STRUCTPARAM | TOK_STRING(MeritRewardStoryArc, name, 0)            }, 
    {    "Merits",                    TOK_STRUCTPARAM | TOK_INT(MeritRewardStoryArc, merits, 0)            }, 
    {    "FailedMerits",                TOK_STRUCTPARAM | TOK_INT(MeritRewardStoryArc, failedmerits, 0)        }, 
    {    "Token",                    TOK_STRUCTPARAM | TOK_STRING(MeritRewardStoryArc, rewardToken, 0)    }, 
    {    "BonusOncePerEpochTable",    TOK_STRUCTPARAM | TOK_STRING(MeritRewardStoryArc, bonusOncePerEpochTable, 0)    }, 
    {    "BonusReplayTable",            TOK_STRUCTPARAM | TOK_STRING(MeritRewardStoryArc, bonusReplayTable, 0)    }, 
    {    "BonusToken",                TOK_STRUCTPARAM | TOK_STRING(MeritRewardStoryArc, bonusToken, 0)    }, 
    {    "BonusMerits",                TOK_STRUCTPARAM | TOK_INT(MeritRewardStoryArc, bonusMerits, 0)    }, 
    {    "\n",                        TOK_END,                            0                                },
    { 0 }
};

TokenizerParseInfo ParseMeritRewardDictionary[] =
{
    {    "Storyarc", TOK_STRUCT(MeritRewardDictionary, ppStoryArc, ParseMeritRewardStoryArc) },
    { 0 }
};

////////////////////////////////////////////////////////////////////////////////////////


//typedef enum
//{
//    SPECIALREWARDNAME_UNLOCKCAPES,
//    SPECIALREWARDNAME_UNLOCKGLOWIE,
//    SPECIALREWARDNAME_FREECOSTUMECHANGE,
//    SPECIALREWARDNAME_COSTUMESLOT,
//    SPECIALREWARDNAME_MAX
//    SPECIALREWARDNAME_STORYARCMERIT
//    SPECIALREWARDNAME_STORYARCMERITFAILED
//    SPECIALREWARDNAME_DISABLEHALFHEALTH
//    SPECIALREWARDNAME_FINISHEDPRAETORIANTUTORIAL
//    SPECIALREWARDNAME_FINISHEDPRAETORIA
//} SpecialRewardIds;
char *specialRewardNames[SPECIALREWARDNAME_MAX] =
{
    "UnlockCapes",
    "UnlockGlowie",
    "FreeCostumeChange",
    "CostumeSlot",
    "StoryArcMerit",
    "StoryArcMeritFailed",
    "DisableHalfHealth",
    "FinishedPraetorianTutorial",
    "FinishedPraetoria",
};

static void rewardSetAward(const RewardSet* set, RewardAccumulator* reward, Entity* player, VillainGroupEnum vgroup, RewardItemSetTarget bitsTarget, int containerType, char **lootList);
static void rewardDropGroupAward(const RewardDropGroup* group, RewardAccumulator* reward, Entity *e, int deflevel, bool bEveryone, RewardItemSetTarget bitsTarget, char **lootList);
static void rewardItemSetAward(const RewardItemSet* set, RewardAccumulator* reward, Entity *e, int deflevel, RewardSetFlags bEveryone, char **lootList);
static bool rewardVerifyAndProcess(TokenizerParseInfo pti[], void* structptr);
static bool rewardGenerate(TokenizerParseInfo pti[], void* structptr);
static int rewardPointerPostProcess(TokenizerParseInfo pti[], void* structptr);
static bool rewardItemSetDictionaryVerifyAndProcess(TokenizerParseInfo pti[], void* structptr);
static bool rewardItemSetFinalProcess(TokenizerParseInfo pti[], void* structptr, bool shared_memory);
static bool rewardFinalProcess(TokenizerParseInfo pti[], void* structptr, bool shared_memory);
static bool rewardChoiceFinalProcess(TokenizerParseInfo pti[], void* structptr, bool shared_memory);
static bool meritRewardFinalProcess(TokenizerParseInfo pti[], void* structptr, bool shared_memory);
static void rewardConnectToItemSets(void);
static bool rewardChoiceVerifyAndProcess(ParseTable pti[], void* structptr);
static int s_updateSendPendingReward(Entity *e, const RewardChoiceSet *rewardChoice);
// This isn't used as a normal ParserLoadFiles postprocessor, it gets called
// explicitly after other data has been categorised as well.
static int rewardMeritVerify(void);

void rewardAddSpecialRewardNames(void);

static int rewardPrintDebugInfo = 0;

cStashTable rewardTableStash()
{
    return g_RewardDefinition->rewardDictionary.hashRewardTable;
}

void rewardReadDefFiles()
{
    int flags = PARSER_SERVERONLY;
    int j;

    for(j = 0; j < ARRAY_SIZE(g_RewardDefinitions); j++)
    {
        g_RewardDefinition = g_RewardDefinitions + j;

        if(g_RewardDefinition->itemSetDictionary.ppItemSets)
        {
            // This is a reload, we don't want to load from shared memory, so set the PARSER_FORCEREBUILD flag
            flags |= PARSER_FORCEREBUILD;

            stashTableDestroyConst(g_RewardDefinition->itemSetDictionary.hashItemSets);
            ParserUnload(ParseItemSetDictionary, &g_RewardDefinition->itemSetDictionary, sizeof(g_RewardDefinition->itemSetDictionary));
        }

        ParserLoadFilesShared(g_RewardDefinition->itemSetSM, g_RewardDefinition->path, ".itemset", g_RewardDefinition->itemSetBin, flags, ParseItemSetDictionary, &g_RewardDefinition->itemSetDictionary, sizeof(g_RewardDefinition->itemSetDictionary), NULL, NULL, rewardItemSetDictionaryVerifyAndProcess, NULL, rewardItemSetFinalProcess);

        if(g_RewardDefinition->rewardDictionary.ppTables)
        {
            // This is a reload, we don't want to load from shared memory, so set the PARSER_FORCEREBUILD flag
            flags |= PARSER_FORCEREBUILD;
            ParserUnload(ParseRewardDictionary, &g_RewardDefinition->rewardDictionary, sizeof(g_RewardDefinition->rewardDictionary));
        }

        ParserLoadFilesShared(g_RewardDefinition->rewardSM, g_RewardDefinition->path, ".reward",  g_RewardDefinition->rewardBin, flags, ParseRewardDictionary, &g_RewardDefinition->rewardDictionary, sizeof(g_RewardDefinition->rewardDictionary), NULL, NULL, rewardVerifyAndProcess, rewardGenerate, rewardFinalProcess);

        if(g_RewardDefinition->rewardChoiceDictionary.ppChoiceSets)
        {
            // This is a reload, we don't want to load from shared memory, so set the PARSER_FORCEREBUILD flag
            flags |= PARSER_FORCEREBUILD;
            ParserUnload(ParseRewardChoiceDictionary, &g_RewardDefinition->rewardChoiceDictionary, sizeof(g_RewardDefinition->rewardChoiceDictionary));
        }
        ParserLoadFilesShared(g_RewardDefinition->rewardChoiceSM, g_RewardDefinition->path, ".choice",  g_RewardDefinition->rewardChoiceBin, flags, ParseRewardChoiceDictionary, &g_RewardDefinition->rewardChoiceDictionary, sizeof(g_RewardDefinition->rewardChoiceDictionary), NULL, NULL, rewardChoiceVerifyAndProcess, NULL, rewardChoiceFinalProcess);

        if(g_RewardDefinition->meritRewardDictionary.ppStoryArc)
        {
            // This is a reload, we don't want to load from shared memory, so set the PARSER_FORCEREBUILD flag
            flags |= PARSER_FORCEREBUILD;
            ParserUnload(ParseMeritRewardDictionary, &g_RewardDefinition->meritRewardDictionary, sizeof(g_RewardDefinition->meritRewardDictionary));
        }
        ParserLoadFilesShared("SM_Merits", g_RewardDefinition->path, ".merits",  "meritsRewards.bin", flags, ParseMeritRewardDictionary, 
                                &g_RewardDefinition->meritRewardDictionary, sizeof(g_RewardDefinition->meritRewardDictionary), NULL, NULL, 
                                NULL, NULL, meritRewardFinalProcess);

        rewardAddSpecialRewardNames();
        rewardMeritVerify();
    }

    reward_UpdateWeeklyTFTokenList();
    g_RewardDefinitionIdx = 0;
    g_RewardDefinition = g_RewardDefinitions + g_RewardDefinitionIdx;
}

typedef enum
{
    REWARDTYPE_REWARDTABLE,
    REWARDTYPE_BADGE,
    REWARDTYPE_COSTUMEPART,
    REWARDTYPE_UNLOCKCAPES,
    REWARDTYPE_UNLOCKGLOWIE,
    REWARDTYPE_FREECOSTUMECHANGE,
    REWARDTYPE_COSTUMESLOT,
    REWARDTYPE_STORYARCMERIT,
    REWARDTYPE_STORYARCMERITFAILED,
    REWARDTYPE_POWERCUSTTHEME,
    REWARDTYPE_DISABLEHALFHEALTH,
    REWARDTYPE_FINISHEDPRAETORIANTUTORIAL,
    REWARDTYPE_FINISHEDPRAETORIA,
    REWARDTYPE_MAX
} RewardTypes;

// Used on startup to verify uniqueness of names
static cStashTable s_AllRewardNames = NULL;

void allRewardNamesInit()
{
    s_AllRewardNames = stashTableCreateWithStringKeys(4096, StashDefault);
}

void allRewardNamesCleanup()
{
    stashTableDestroyConst(s_AllRewardNames);
    s_AllRewardNames = NULL;
}

// this function is solely used for checking for duplicate reward names
// I.E., reward names that are the same for different types of rewards, 
// such as a badge and a reward table having the same name
static int addToAllRewardNames(const char *reward, int type)
{
    int ret;
    assert(s_AllRewardNames);

    if ( stashFindInt(s_AllRewardNames, reward, &ret) )
    {
        if ( ret != type )
            return 0;
        return 1;
    }
    return stashAddIntConst(s_AllRewardNames, reward, type, false);
}

void rewardAddSpecialRewardNames(void)
{
    if ( !addToAllRewardNames(specialRewardNames[SPECIALREWARDNAME_UNLOCKCAPES], REWARDTYPE_UNLOCKCAPES) )
        ErrorfInternal("Reward name conflicts with special reward name \"%s\"", specialRewardNames[SPECIALREWARDNAME_UNLOCKCAPES]);
    if ( !addToAllRewardNames(specialRewardNames[SPECIALREWARDNAME_UNLOCKGLOWIE], REWARDTYPE_UNLOCKGLOWIE) )
        ErrorfInternal("Reward name conflicts with special reward name \"%s\"", specialRewardNames[SPECIALREWARDNAME_UNLOCKGLOWIE]);
    if ( !addToAllRewardNames(specialRewardNames[SPECIALREWARDNAME_FREECOSTUMECHANGE], REWARDTYPE_FREECOSTUMECHANGE) )
        ErrorfInternal("Reward name conflicts with special reward name \"%s\"", specialRewardNames[SPECIALREWARDNAME_FREECOSTUMECHANGE]);
    if ( !addToAllRewardNames(specialRewardNames[SPECIALREWARDNAME_COSTUMESLOT], REWARDTYPE_COSTUMESLOT) )
        ErrorfInternal("Reward name conflicts with special reward name \"%s\"", specialRewardNames[SPECIALREWARDNAME_COSTUMESLOT]);        
    if ( !addToAllRewardNames(specialRewardNames[SPECIALREWARDNAME_STORYARCMERIT], REWARDTYPE_STORYARCMERIT) )
        ErrorfInternal("Reward name conflicts with special reward name \"%s\"", specialRewardNames[SPECIALREWARDNAME_STORYARCMERIT]);    
    if ( !addToAllRewardNames(specialRewardNames[SPECIALREWARDNAME_STORYARCMERITFAILED], REWARDTYPE_STORYARCMERITFAILED) )
        ErrorfInternal("Reward name conflicts with special reward name \"%s\"", specialRewardNames[SPECIALREWARDNAME_STORYARCMERITFAILED]);    
    if ( !addToAllRewardNames(specialRewardNames[SPECIALREWARDNAME_DISABLEHALFHEALTH], REWARDTYPE_DISABLEHALFHEALTH) )
        ErrorfInternal("Reward name conflicts with special reward name \"%s\"", specialRewardNames[SPECIALREWARDNAME_DISABLEHALFHEALTH]);
    if ( !addToAllRewardNames(specialRewardNames[SPECIALREWARDNAME_FINISHEDPRAETORIANTUTORIAL], REWARDTYPE_FINISHEDPRAETORIANTUTORIAL) )
        ErrorfInternal("Reward name conflicts with special reward name \"%s\"", specialRewardNames[SPECIALREWARDNAME_FINISHEDPRAETORIANTUTORIAL]);
    if ( !addToAllRewardNames(specialRewardNames[SPECIALREWARDNAME_FINISHEDPRAETORIA], REWARDTYPE_FINISHEDPRAETORIA) )
        ErrorfInternal("Reward name conflicts with special reward name \"%s\"", specialRewardNames[SPECIALREWARDNAME_FINISHEDPRAETORIA]);
}

static bool rewardItemSetFinalProcess(TokenizerParseInfo pti[], ItemSetDictionary *dict, bool shared_memory)
{
    bool ret = true;
    bool noRequires;
    int i, j;
    int n = eaSize(&dict->ppItemSets);

    assert(!dict->hashItemSets);
    dict->hashItemSets = stashTableCreateWithStringKeys(stashOptimalSize(n), stashShared(shared_memory));

    for(i = 0; i < n; i++ )
    {
        const RewardItemSet *set = dict->ppItemSets[i];

        if(!stashAddPointerConst(dict->hashItemSets, set->name, set, false))
        {
            ErrorFilenamef(g_RewardDefinition->path, "Duplicate ItemSet name \"%s\"", set->name);
            ret = false;
        }

        // verify that all items in set do not have requires
        noRequires = false;
        for (j = eaSize(&set->rewardItems) - 1; j >=0; j--)
        {
            if (set->rewardItems[j] && set->rewardItems[j]->chanceRequires == NULL)
            {
                noRequires = true;
                break;
            }
        }

        if (!noRequires)
        {
            ErrorFilenamef(g_RewardDefinition->path, "All itmes in ItemSet name \"%s\" have requires.", set->name);
            ret = false;            
        }
    }

    return ret;
}

static bool rewardFinalProcess(TokenizerParseInfo pti[], RewardDictionary *dict, bool shared_memory)
{
    bool ret = true;
    int i;
    int n = eaSize(&dict->ppTables);

    assert(!dict->hashRewardTable);
    dict->hashRewardTable = stashTableCreateWithStringKeys(stashOptimalSize(n), stashShared(shared_memory));

    for(i = 0; i < n; i++ )
    {
        if( !addToAllRewardNames(dict->ppTables[i]->name, REWARDTYPE_REWARDTABLE) ||
            !stashAddPointerConst(dict->hashRewardTable, dict->ppTables[i]->name, dict->ppTables[i], false) )
        {
            ErrorfInternal("Duplicate RewardTable Name: %s", g_RewardDefinition->rewardDictionary.ppTables[i]->name);
            ret = false;
        }
    }

    return ret;
}

static bool rewardChoiceFinalProcess(TokenizerParseInfo pti[], RewardChoiceDictionary *dict, bool shared_memory)
{
    bool ret = true;
    int i;
    int n = eaSize(&dict->ppChoiceSets);

    assert(!dict->hashRewardChoice);
    dict->hashRewardChoice = stashTableCreateWithStringKeys(stashOptimalSize(n), stashShared(shared_memory));

    for(i = 0; i < n; i++ )
    {
        if ( stashFindElementConst(g_RewardDefinition->rewardDictionary.hashRewardTable, dict->ppChoiceSets[i]->pchName, NULL) )
        {
            ErrorfInternal("Reward Table Name Conflicts with Choice Table name: %s", dict->ppChoiceSets[i]->pchName );
            ret = false;
        }
        else if ( !stashAddPointerConst(dict->hashRewardChoice, dict->ppChoiceSets[i]->pchName, dict->ppChoiceSets[i], false) )
        {
            ErrorfInternal("Duplicate ChoiceTable Name: %s", g_RewardDefinition->rewardChoiceDictionary.ppChoiceSets[i]->pchName);
            ret = false;
        }
    }

    return ret;
}

static bool meritRewardFinalProcess(TokenizerParseInfo pti[], MeritRewardDictionary *dict, bool shared_memory)
{
    bool ret = true;
    int i;
    int n = eaSize(&dict->ppStoryArc);

    assert(!dict->hashMeritReward);
    dict->hashMeritReward = stashTableCreateWithStringKeys(stashOptimalSize(n), stashShared(shared_memory));

    for(i = 0; i < n; i++ )
    {
        if (!stashAddPointerConst(dict->hashMeritReward, dict->ppStoryArc[i]->name, dict->ppStoryArc[i], false))
        {
            ErrorfInternal("Duplicate Merit Reward for Storyarc Name: %s", g_RewardDefinition->meritRewardDictionary.ppStoryArc[i]->name);
            ret = false;
        }
    }

    return ret;
}

void rewardAddBadgeToHashTable(const char *pchBadgeName, const char* pchFilename)
{
    if (!addToAllRewardNames(pchBadgeName, REWARDTYPE_BADGE))
        ErrorFilenamef(pchFilename, "RewardTable Name Conflicts with Badge name: %s", pchBadgeName);
}

void rewardAddCostumePartToHashTable(const char *pchCostumePart)
{
    if (!addToAllRewardNames(pchCostumePart, REWARDTYPE_COSTUMEPART))
        ErrorfInternal("RewardTable Name Conflicts with RewardToken name: %s", pchCostumePart);
}

void rewardAddPowerCustThemeToHashTable(char *pchPowerCustTheme)
{
    if (!addToAllRewardNames(pchPowerCustTheme, REWARDTYPE_POWERCUSTTHEME))
        ErrorfInternal("RewardTable Name Conflicts with RewardToken name: %s", pchPowerCustTheme);
}

static bool rewardChoiceVerifyAndProcess(TokenizerParseInfo pti[], void* structptr)
{
    bool ret = true;
    int rewardChoiceIndex;

    for (rewardChoiceIndex = eaSize(&g_RewardDefinition->rewardChoiceDictionary.ppChoiceSets) - 1; rewardChoiceIndex >= 0; rewardChoiceIndex--)
    {
        const RewardChoiceSet *rewardChoiceSet = g_RewardDefinition->rewardChoiceDictionary.ppChoiceSets[rewardChoiceIndex];
        if (rewardChoiceSet)
        {
            int choiceCount = eaSize(&rewardChoiceSet->ppChoices);
            int i;
            if (rewardChoiceSet->isMoralChoice && choiceCount != 2)
            {
                Errorf("Moral Choice Table '%s' has %i choices, requires exactly 2 choices.", rewardChoiceSet->pchName, choiceCount);
                ret = false;
            }
            if (choiceCount > (sizeof(int)*8))
            {
                Errorf("Reward Table %s has too many rewards %i", rewardChoiceSet->pchName, choiceCount);
                ret = false;
            }
            for (i = 0; i < choiceCount; ++i)
            {
                int j;
                const RewardChoice *rewardSelection = rewardChoiceSet->ppChoices[i];

                if(rewardSelection->ppRequirements)
                {
                    for(j = eaSize(&rewardSelection->ppRequirements) - 1; j >= 0; j--)
                        chareval_Validate(rewardSelection->ppRequirements[j]->ppchRequires, "defs/reward/Choices.choice");
                }
                if(rewardSelection->ppVisibleRequirements)
                {
                    for(j = eaSize(&rewardSelection->ppVisibleRequirements) - 1; j >= 0; j--)
                        chareval_Validate(rewardSelection->ppVisibleRequirements[j]->ppchRequires, "defs/reward/Choices.choice");
                }

                //    1 bit is set aside for the "TOTAL HACK" candy cane so throw an error if 31 bits are used
                if (eaSize(&rewardSelection->ppRequirements) >= (sizeof(int)*8))
                {
                    Errorf("Reward Choice %i has too many requirements %i", (i+1), eaSize(&rewardChoiceSet->ppChoices[i]->ppRequirements));
                    ret = false;
                }
            }
        }
    }

    return ret;
}


static int rewardMeritVerify(void)
{
    int returnMe = 1;
    int storyArcIndex;

    for (storyArcIndex = eaSize(&g_RewardDefinition->meritRewardDictionary.ppStoryArc) - 1; storyArcIndex >= 0; storyArcIndex--)
    {
        const char *rewardToken = g_RewardDefinition->meritRewardDictionary.ppStoryArc[storyArcIndex]->rewardToken;

        if (!stricmp(rewardToken, "none"))
        {
            continue;
        }

        if (reward_GetTokenType(rewardToken) == kRewardTokenType_None)
        {
            Errorf("Reward token \"%s\" from merit reward for storyarc \"%s\" not found in RewardTokens!", rewardToken, g_RewardDefinition->meritRewardDictionary.ppStoryArc[storyArcIndex]->name);
            returnMe = 0;
        }
    }
    return returnMe;
}


static const char* errorPrefix;
static const char* errorFilename = NULL;

static bool rewardVerifyAndProcess(TokenizerParseInfo pti[], void* structptr)
{
    char buffer[1024];
    int i;
    int iTable;    
    bool ret = true;
    int maxLevel;
    static int errorcount = 0;

    maxLevel = MAX(MAX_PLAYER_SECURITY_LEVEL, eaiSize(&g_ExperienceTables.aTables.piRequired));

    for(iTable=eaSize(&g_RewardDefinition->rewardDictionary.ppTables)-1; iTable>=0; iTable--)
    {
        RewardTable *pTable = (RewardTable*)(g_RewardDefinition->rewardDictionary.ppTables[iTable]);

        if (pTable->verified)
            continue;

        if( !pTable->permanentRewardToken && !pTable->randomItemOfPower ) //Door keys and Items of Power don't need level listings
        {
            // Walk through all possible levels for using a 0 based level number.
            for(i = 0; i < maxLevel; i++)
            {
                int found = 0;
                int j;

                for(j = eaSize(&pTable->ppDefs)-1; j >= 0; j--)
                {
                    const RewardDef* def = pTable->ppDefs[j];
                    if(def->level == i+1)
                    {
                        found = 1;
                        break;
                    }
                }

                if(!found)
                {
                    //sprintf(buffer, "defs/Reward/%s.reward", g_RewardDefinition->rewardDictionary.ppTables[iTable]->name);
                    ErrorFilenamef(pTable->filename, "Reward error %s, level %i def missing", pTable->name, i+1);
                }
            }
        }

        rewardVerifyMaxErrorCount(1);
        for(i = eaSize(&pTable->ppDefs)-1; i >= 0; i--)
        {
            int j;
            const RewardDef* def = pTable->ppDefs[i];
            sprintf(buffer, "Reward error, level %i", def->level);
            rewardVerifyErrorInfo(buffer, pTable->filename);

            for( j = eaSize(&pTable->ppDefs[i]->rewardSets)-1; j >= 0; j-- )
            {
                RewardSet * rset = pTable->ppDefs[i]->rewardSets[j];
                if(eaSize(&rset->dropGroups) > 1 || eaSize(&rset->sgrpDropGroups) > 1)
                    ErrorFilenamef(pTable->filename, "ERROR: (File %s) Table %s has a reward with multiple dropgroups! Per-line datamining assumes single dropgroups.", pTable->filename, pTable->name);
                //if( rset->influence && !rset->prestige && !rset->requires)
                //    ErrorFilenamef(pTable->filename, "WARNING: (File %s) Table %s has reward has %i influnce, but no prestige. count: %i", pTable->filename, pTable->name, rset->influence, ++errorcount );
            }

            if (!rewardSetsVerifyAndProcess(def->rewardSets))
                ret = false;

        }
        pTable->verified = true;
    }

    return ret;
}

static bool rewardGenerate(TokenizerParseInfo pti[], RewardDictionary *pdict)
{
    int iTable;
    static int errorcount = 0;

    for(iTable=eaSize(&pdict->ppTables)-1; iTable>=0; iTable--)
    {
        int i;
        const RewardTable *pTable = pdict->ppTables[iTable];

        // Walk through all possible levels for using a 0 based level number.
        for(i =eaSize(&pTable->ppDefs)-1; i>= 0; i--)
        {
            int j;
            const RewardDef *def = pTable->ppDefs[i];

            for(j=eaSize(&def->rewardSets)-1; j>=0; j--)
            {
                RewardSet *rset = def->rewardSets[j];

                //if( rset->influence && !rset->prestige && !rset->requires)
                //    ErrorFilenamef(pTable->filename, "WARNING: (File %s) Table %s has reward has %i influnce, but no prestige. count: %i", pTable->filename, pTable->name, rset->influence, ++errorcount );

                rset->level = def->level;
            }
        }
    }
    return true;
}


//------------------------------------------------------------------------
// Reward Error checking functions
//------------------------------------------------------------------------
static int rewardSetVerifyAndProcess(RewardSet* set);
static int rewardDropGroupVerifyAndProcess(RewardDropGroup* group);
static int rewardItemSetVerifyAndProcess(RewardItemSet* set);


int maxErrorCount = 50;

void rewardVerifyErrorInfo(const char* prefix, const char* filename)
{
    errorPrefix = prefix;
    errorFilename = filename;
}

void rewardVerifyMaxErrorCount(int count)
{
    maxErrorCount = count;
}

// Error checking starting from RewardSet level because RewardDef contains a bunch of Clue related stuff which
// cannot be verified here.
int rewardSetsVerifyAndProcess(RewardSet** rewardSets)
{
    int i;
    int ret = 1;

    if(!rewardSets)
        return 1;

    for(i = eaSize(&rewardSets)-1; i >= 0; i--)
    {
        RewardSet* set = cpp_const_cast(RewardSet*)(rewardSets[i]);
        if (!rewardSetVerifyAndProcess(set))
            ret = 0;

        if(eaSize(&set->dropGroups) > 1)
        {
            printf("Reward:%s\n","");
            return 0;
        }
    }
    return ret;
}

static int rewardSetVerifyAndProcess(RewardSet* set)
{
    int i;
    int ret = 1;

    if(set->rewardSetRequires)
        chareval_Validate(set->rewardSetRequires, errorFilename);

    for(i = eaSize(&set->dropGroups)-1; i >= 0; i--)
    {
        if (!rewardDropGroupVerifyAndProcess(cpp_const_cast(RewardDropGroup*)(set->dropGroups[i])))
            ret = 0;
    }
    return ret;
}

static int rewardDropGroupVerifyAndProcess(RewardDropGroup* group)
{
    int ret = 1;
    if(eaSize(&group->itemSets)>0 || eaSize(&group->itemSetNames)>0)
    {
        int i;

        for(i=eaSize(&group->itemSets)-1; i>=0; i--)
        {
             if(!rewardItemSetVerifyAndProcess(cpp_const_cast(RewardItemSet*)(group->itemSets[i])))
            {
                ret = false;
            }
        }

        for( i = eaSize(&group->itemSetNames) - 1; i >= 0; --i)
        {
            const char *isetFullName = group->itemSetNames[i];
            char *isetName;
            const RewardItemSet *iset;
            
            if(!strnicmp(isetFullName,"RT.",3)) // Ignore reward table names, at least for now
                continue;

            isetName =  strchr(isetFullName,'.') ? strchr(isetFullName,'.') + 1 : isetFullName; // get past delimiter
            iset = stashFindPointerReturnPointerConst(g_RewardDefinition->itemSetDictionary.hashItemSets, isetName );

            if( iset )
            {
                if(!rewardItemSetVerifyAndProcess(cpp_const_cast(RewardItemSet*)(iset)))
                {
                    ret = false;
                }
            }
            else
            {
                ErrorFilenamef(errorFilename, "%s: refers to item set %s that doesn't exist\n", errorFilename, isetName );
                ret = false;
            }
        }

    }
    else
    {
        if(errorFilename)
            ErrorFilenamef(errorFilename, "%s: reward drop group doesn't have any items\n", errorPrefix);
        else
            Errorf("%s: reward drop group doesn't have any items\n", errorPrefix);
    }

    return ret;
}

static bool rewardItemSetDictionaryVerifyAndProcess(TokenizerParseInfo pti[], ItemSetDictionary *dict)
{
    int i;
    bool ret = true;

    for(i = eaSize(&dict->ppItemSets)-1; i >= 0; i--)
    {
        if(!rewardItemSetVerifyAndProcess(cpp_const_cast(RewardItemSet*)(dict->ppItemSets[i])))
            ret = false;
    }
    return ret;
}

//------------------------------------------------------------
// verify a rewarditem of type kRewardItemType_Power
// -AB: created :2005 Feb 18 04:08 PM
//----------------------------------------------------------
static int rewarditem_VerifyTypePower( const RewardItem *item )
{
    int ret = 0;
    if( verify( item && item->type == kRewardItemType_Power))
    {
        const PowerNameRef* power = &item->power;
        const BasePower* basePower = powerdict_GetBasePowerByName(&g_PowerDictionary, power->powerCategory, power->powerSet, power->power);
        if(!basePower)
        {
            ret = 0;
            if(errorPrefix)
            {
                if(maxErrorCount)
                {
                    char* errorString = "%s: Bad power name %s.%s.%s found\n";
                    if(errorFilename)
                    {
                        ErrorFilenamef(errorFilename, errorString, errorPrefix, power->powerCategory, power->powerSet, power->power);
                    }
                    else
                    {
                        Errorf(errorString, errorPrefix, power->powerCategory, power->powerSet, power->power);
                    }
                    maxErrorCount--;
                }
                LOG( LOG_ERROR, LOG_LEVEL_VERBOSE, 0, "%s: Bad power name %s.%s.%s found\n", errorPrefix, power->powerCategory, power->powerSet, power->power);
            }
            else
            {
                if(maxErrorCount)
                {
                    char* errorString = "Bad power name %s.%s.%s found\n";
                    if(errorFilename)
                    {
                        ErrorFilenamef(errorFilename, errorString, power->powerCategory, power->powerSet, power->power);
                    }
                    else
                    {
                        Errorf(errorString, power->powerCategory, power->powerSet, power->power);
                    }
                    maxErrorCount--;
                }
                LOG( LOG_ERROR, LOG_LEVEL_VERBOSE, 0, "Bad power name %s.%s.%s found\n", power->powerCategory, power->powerSet, power->power);
            }
        }
        else
        {
            // success
            ret = 1;
        }
    }
    // --------------------
    // finally, return

    return ret;
}


//------------------------------------------------------------
// verify the salvage type
//----------------------------------------------------------
static int rewarditem_VerifyTypeSalvage( const RewardItem *item )
{
    int res = 0;

    if( verify( item && item->type == kRewardItemType_Salvage ))
    {
        res = (salvage_GetItem( item->itemName) != NULL);
    }
    // --------------------
    // finally, return

    if(!res)
        ErrorFilenamef(errorFilename, "%s: Salvage can't be found %s", errorPrefix, item->itemName);

    return res;

}

//------------------------------------------------------------
// verify the concept type
//----------------------------------------------------------
static int rewarditem_VerifyTypeConcept( const RewardItem *item )
{
    int res = 0;

    if( verify( item && item->type == kRewardItemType_Concept ))
    {
        res = (conceptdef_Get( item->itemName) != NULL);
    }
    // --------------------
    // finally, return

    if(!res)
        ErrorFilenamef(errorFilename, "%s: Concept can't be found %s", errorPrefix, item->itemName);

    return res;
}

//------------------------------------------------------------
// verify the proficiency type
//----------------------------------------------------------
static int rewarditem_VerifyTypeProficiency( const RewardItem *item )
{
    int res = 0;

    if( verify( item && item->type == kRewardItemType_Proficiency ))
    {
        res = (proficiency_GetItem( item->itemName) != NULL);
    }
    // --------------------
    // finally, return

    if(!res)
        ErrorFilenamef(errorFilename, "%s: Proficiency can't be found %s", errorPrefix, item->itemName);

    return res;

}

static int rewarditem_VerifyTypeDetailRecipe( const RewardItem *item )
{
    int res = 0;

    if( verify( item && item->type == kRewardItemType_DetailRecipe ))
    {
        res = detailrecipedict_CatagoryExists(item->itemName);
        if (!res)
            res = (detailrecipedict_RecipeFromName(item->itemName) != NULL);
    }

    // --------------------
    // finally, return

    if(!res)
        ErrorFilenamef(errorFilename, "%s: DetailRecipe can't be found %s", errorPrefix, item->itemName);

    return res;

}

static int rewarditem_VerifyTypeDetail( const RewardItem *item )
{
    int res = 0;

    if( verify( item && item->type == kRewardItemType_Detail ))
    {
        res = (basedata_GetDetailByName( item->itemName ) != NULL);
    }
    // --------------------
    // finally, return

    if(!res)
        ErrorFilenamef(errorFilename, "%s: Detail can't be found %s", errorPrefix, item->itemName);

    return res;

}

static int rewarditem_VerifyTypeToken( const RewardItem *item )
{
    int res = 0;

    if( verify( item && item->type == kRewardItemType_Token ))
    {
        res = 1; // is there a way to verify that the token name will actually work?
    }
    // --------------------
    // finally, return

    if(!res)
        ErrorFilenamef(errorFilename, "%s: Detail can't be found %s", errorPrefix, item->itemName);

    return res;
}

static int rewarditem_VerifyTypeRewardTable( const RewardItem *item )
{
    int res = 1;

    // TODO: These can't be checked here since they aren't loaded yet.

//    if( verify( item && item->type == kRewardItemType_RewardTable ))
//    {
//        // ab: ugly, create a static local hashtable just for this.
//        // will only happen when bin files are created, so should be okay.
//        static StashTable ht = NULL;
//        if( !ht )
//        {
//            int i;
//            ht = stashTableCreateWithStringKeys( 128, StashDeepCopyKeys );
//            for( i = eaSize( &g_RewardDefinition->rewardDictionary.ppTables ) - 1; i >= 0; --i)
//            {
//                stashAddPointer(ht, g_RewardDefinition->rewardDictionary.ppTables[i]->name, g_RewardDefinition->rewardDictionary.ppTables[i], false);
//            }
//        }
//        res = (stashFindPointerReturnPointer(ht, item->itemName ) != NULL);
//    }
//
//    // --------------------
//    // finally, return
//
//    if(!res)
//        ErrorFilenamef(errorFilename, "%s: RewardTable can't be found %s", errorPrefix, item->itemName);

    return res;

}

static int rewarditem_VerifyTypeRewardTokenCount( const RewardItem *item )
{
    int res = 1;

    // TODO: These can't be checked here since they aren't loaded yet.
/*
    if( verify( item && item->type == kRewardItemType_RewardTokenCount ))
    {
        res = rewardIsPermanentRewardToken(item->itemName);
    }

    // --------------------
    // finally, return

    if(!res)
        ErrorFilenamef(errorFilename, "%s: Reward Token can't be found %s", errorPrefix, item->itemName);
*/

    return res;

}


static int rewarditem_VerifyTypeIncarnatePoints(const RewardItem *item)
{
    int res = verify(item && item->type == kRewardItemType_IncarnatePoints);

    return res;
}


static int rewarditem_VerifyTypeAccountProduct(const RewardItem *item)
{
    int res = verify(item && item->type == kRewardItemType_AccountProduct);

    if (res)
        res = accountCatalogGetProduct(skuIdFromString(item->itemName)) != NULL;

    if (!res)
        ErrorFilenamef(errorFilename, "%s: Account Product '%s' can't be found.", errorPrefix, item->itemName);

    if (res)
        res = item->rarity != 0;

    if (!res)
        ErrorFilenamef(errorFilename, "%s: Account Product '%s' has a bad rarity in the reward file.", errorPrefix, item->itemName);

    return res;
}


typedef int (*rewarditem_verifyFuncType)( const RewardItem *);

//------------------------------------------------------------
// lookup for dispatching function verification by type.
// MUST BE IN ORDER OF RewardItemType ENUM.
// -AB: created :2005 Feb 18 04:12 PM
//----------------------------------------------------------
static rewarditem_verifyFuncType s_rewarditem_verifyFuncType[] =
    {
        rewarditem_VerifyTypePower,
        rewarditem_VerifyTypeSalvage,
        rewarditem_VerifyTypeConcept,
        rewarditem_VerifyTypeProficiency,
        rewarditem_VerifyTypeDetailRecipe,
        rewarditem_VerifyTypeDetail,
        rewarditem_VerifyTypeToken,
        rewarditem_VerifyTypeRewardTable,
        rewarditem_VerifyTypeRewardTokenCount,
        rewarditem_VerifyTypeIncarnatePoints,
        rewarditem_VerifyTypeAccountProduct,
    };
STATIC_ASSERT(ARRAY_SIZE(s_rewarditem_verifyFuncType) == kRewardItemType_Count);


// end itemset verifying functions
// ------------------------------------------------------------


//------------------------------------------------------------
// Verify an item set
//----------------------------------------------------------
static int rewardItemSetVerifyAndProcess(RewardItemSet* set)
{
    int ret = 1;
    int i;
    bool noRequires = false;
    const RewardItem **items = set->rewardItems;

    if(!errorFilename)
        errorFilename = "defs/reward/Tables.itemset";

    if (!set->verified)
    {
        if(eaSize(&items) == 0)
            ErrorFilenamef(errorFilename, "%s: no RewardItems specified in ItemSet %s. This will never reward anything.", errorPrefix, set->name);
        
        for(i = eaSize(&items)-1; i >= 0; i--)
        {
            const RewardItem *reward = items[i];

            if (reward && reward->chanceRequires == NULL)
                noRequires = true;
            else
                chareval_Validate(reward->chanceRequires, errorFilename);

            // --------------------
            // dispatch the check func by type
            if( verify( reward && INRANGE( reward->type, 0, ARRAY_SIZE( s_rewarditem_verifyFuncType ))) )
            {
                // accumulate the result of all the calls
                ret = s_rewarditem_verifyFuncType[ reward->type ](reward) && ret;
            }
        }

        if (!noRequires)
        {
            ErrorFilenamef(errorFilename, "All items in ItemSet name \"%s\" have requires.", set->name);
            ret = false;            
        }

        // Crashing on this line indicates that it is too late in the shared memory loading sequence to verify rewards
        set->verified = true;
    }

    return ret;
}
//------------------------------------------------------------------------
// Reward Loot List Functions
//------------------------------------------------------------------------
void rewardAppendToLootList(char **lootList, char *format, ...)
{
    if (lootList == NULL || *lootList == NULL)
        return;

    if (**lootList != 0)
        estrConcatStaticCharArray(lootList, ", ");

    VA_START(args, format);
    estrConcatfv(lootList, format, args);
    VA_END();
}

//------------------------------------------------------------------------
// Private Reward Functions
//------------------------------------------------------------------------
void rewardDefAward(const RewardDef* def, RewardAccumulator* reward, Entity* player, VillainGroupEnum vgroup, const char *rewardTableName, int containerType, char **lootList)
{
    assert(def && reward);

    if(rewardPrintDebugInfo)
        printf("--- Reward begin ---\n");

    rewardSetsAward(def->rewardSets, reward, player, vgroup, def->level, rewardTableName, containerType, lootList);

    if(rewardPrintDebugInfo)
        printf("--- Reward end ---\n");
}

static RewardAccumulatedIncarnatePoints* rewardAccumulatedIncarnatePoints_Create(int subtype, int count);

static void rewardSetAward(const RewardSet* set, RewardAccumulator* reward, Entity* player, VillainGroupEnum vgroup, RewardItemSetTarget bitsTarget, int containerType, char **lootList)
{
    int i;
    int totalChanceSum = 0;    // sum of all chance fields in the drop group.
    int chanceSum = 0;
    int randRoll;

    if(verify(set && reward && rewarditemsettarget_Valid(bitsTarget)))
    {
        if ( (containerType == CONTAINER_LEAGUES) ^ set->bLeagueOnly )
            return;

        reward->experience += set->experience;
        reward->wisdom += set->wisdom;
        reward->influence += set->influence;
//        reward->rewardTable = set->rewardTable; // ARM NOTE: This appears to be unused, so I'm pulling it.  Revert if I'm wrong.
        reward->prestige += set->prestige;
        reward->bonus_experience += set->bonus_experience;
        reward->bIgnoreRewardCaps |= set->bIgnoreRewardCaps;
        reward->bLeagueOnly |= set->bLeagueOnly;

        if (set->incarnateSubtype != -1)
        {
            RewardAccumulatedIncarnatePoints *p;
            p = rewardAccumulatedIncarnatePoints_Create(set->incarnateSubtype, set->incarnateCount);
            if (verify(p))
            {
                eaPush(&reward->incarnatePoints, p);
            }
        }

        // Award entity with a single drop group.
        //   The chance fields in all drop groups do not need to add up to a hundred.
        //   We always pick one using the chance field to give each group their proper weight.

        // Sum up all chance fields in drop groups.
        totalChanceSum = 0;
        for(i = eaSize(&set->dropGroups)-1; i >= 0; i--)
        {
            totalChanceSum += set->dropGroups[i]->chance;
        }

        if (totalChanceSum==0)
            return;

        // Generate a random number that is within the range of the sum.
        randRoll = rand() % totalChanceSum;
        if(rewardPrintDebugInfo)
            printf("\trandroll = %i / %i\n", randRoll, totalChanceSum);

        // Find out in which drop group the roll fell into.
        {
            int iSize = eaSize(&set->dropGroups);
            for(i = 0; i < iSize; i++)
            {
                chanceSum += set->dropGroups[i]->chance;
                if(chanceSum > randRoll)
                {
                    if(rewardPrintDebugInfo)
                        printf("\tAwarding DropGroup %d (%d (chanceSum) > %d randRoll)\n", i, chanceSum, randRoll);
                    rewardDropGroupAward(set->dropGroups[i], reward, player, set->level, set->flags, bitsTarget, lootList);
                    break;
                }
            }
        }
    }
}


bool rewarditemsettarget_Valid( RewardItemSetTarget bitfield )
{
    // no bits set outside the range
    return !(bitfield & ~kItemSetTarget_All);
}

static void rewardaccumulatedrewardtable_Accum(const RewardItem* item, RewardAccumulator* reward, int deflevel, RewardSetFlags set_flags);

void rewardItemSetAccumByName(const char* itemSetName, RewardAccumulator* reward, Entity *e, int deflevel, RewardSetFlags set_flags, RewardItemSetTarget bitsTarget, char **lootList)
{
    // try the raw name first (backwards compat)
    const RewardItemSet *iset = stashFindPointerReturnPointerConst(g_RewardDefinition->itemSetDictionary.hashItemSets, itemSetName );
    RewardItemSetTarget targetmaskIset = kItemSetTarget_Player;

    if(!strnicmp(itemSetName,"RT.",3))
    {
        RewardItem pItem; // Should be safe, itemName is shallow copied, and type is checked
        pItem.type = kRewardItemType_RewardTable;
        pItem.itemName = itemSetName + 3;
        pItem.rewardTableFlags = kRewardTableFlags_AtCritterLevel;        // do not adjust for level since we have already made the adjustment
        rewardaccumulatedrewardtable_Accum(&pItem,reward,deflevel,set_flags);
        return;
    }
    
    // find marked up itemset
    {
        //------------------------------------------------------------
        //  These targets are closely associated with
        // game/tools/scripts/xlstorewarddef.bat:OutputReward
        //----------------------------------------------------------
        static struct
        {
            char const *str;
            RewardItemSetTarget target;
        } targets[] =
        {
            { "PL.", kItemSetTarget_Player },
            { "SG.", kItemSetTarget_Sgrp},
        };
        int i;
        for( i = 0; i < ARRAY_SIZE( targets ); ++i )
        {
            int len = (int)strlen(targets[i].str);
            if( 0 == strnicmp( itemSetName, targets[i].str, len ))
            {
                itemSetName += len;
                iset = stashFindPointerReturnPointerConst(g_RewardDefinition->itemSetDictionary.hashItemSets, itemSetName );

                targetmaskIset = targets[i].target;
                break;                // BREAK IN FLOW
            }
        }
    }

    if( !iset )
    {
        ErrorfInternal("No ItemSet found named %s", itemSetName);
        return;
    }

    if(rewardPrintDebugInfo)
        printf("\t\tAwarding ItemSet %s\n", itemSetName);

    if( bitsTarget & targetmaskIset )
    {
        rewardItemSetAward(iset, reward, e, deflevel, set_flags, lootList);
    }
}

static void rewardDropGroupAward(const RewardDropGroup* group, RewardAccumulator* reward, Entity *e, int deflevel, RewardSetFlags set_flags, RewardItemSetTarget bitsTarget, char **lootList)
{
    int i;
    if(verify(group && reward && rewarditemsettarget_Valid( bitsTarget )))
    {
        // Award entity with each itemset.
        for(i = eaSize(&group->itemSetNames)-1; i >= 0; i--)
        {
            rewardItemSetAccumByName(group->itemSetNames[i], reward, e, deflevel, set_flags, bitsTarget, lootList);
        }

        // Award entity with each itemset.
        // NOTE: no target checked for inline itemsets. this is just the way it is, the tools
        // for the designers don't ever fill this out anyway.
        for(i = eaSize(&group->itemSets)-1; i >= 0; i--)
        {
            if(rewardPrintDebugInfo)
                printf("\t\tAwarding ItemSet %i\n", i);
            rewardItemSetAward(group->itemSets[i], reward, e, deflevel, set_flags, lootList);
        }
    }
}

// ------------------------------------------------------------
// Accumulated object support functionionality

// ----------------------------------------
//  RewardAccumulatedPower

MP_DEFINE(RewardAccumulatedPower);
static RewardAccumulatedPower* rewardaccumulatedpower_Create(const RewardItem *item, int defLevel, RewardSetFlags set_flags)
{
    RewardAccumulatedPower* res = NULL;

    if( verify( item && item->type == kRewardItemType_Power ))
    {
        // create the pool, arbitrary number
        MP_CREATE(RewardAccumulatedPower, 16);
        res = MP_ALLOC( RewardAccumulatedPower );
        if( verify( res ))
        {
            res->powerNameRef = &item->power;
            res->set_flags = set_flags;
            res->level = defLevel;
        }
    }
    // --------------------
    // finally, return

    return res;
}

static void rewardaccumulatedpower_Destroy( RewardAccumulatedPower *p )
{
    MP_FREE( RewardAccumulatedPower, p );
}

//------------------------------------------------------------
// helper for accumulating powers in a reward. this prototype
//  is uniform for helping simplify the dispatch of accumulating items by type.
// see Reward.c: static s_rewardaccumulated_accumFuncs[] for usage
// -AB: created :2005 Feb 18 10:10 AM
//----------------------------------------------------------
static void rewardaccumulatedpower_Accum(const RewardItem* item, RewardAccumulator* reward, int deflevel, RewardSetFlags set_flags)
{
    RewardAccumulatedPower *p;
    p = rewardaccumulatedpower_Create( item, deflevel, set_flags );
    if( verify( p ) )
        eaPush( &reward->powers, p);
}

// ============================================================
// RewardAccumulatedSalvage

MP_DEFINE(RewardAccumulatedSalvage);
static RewardAccumulatedSalvage* rewardaccumulatedsalvage_Create(const RewardItem *item, int defLevel, RewardSetFlags set_flags )
{
    RewardAccumulatedSalvage* res = NULL;

    if( verify( item && item->type == kRewardItemType_Salvage ))
    {
        // create the pool, arbitrary number
        MP_CREATE(RewardAccumulatedSalvage, 16);
        res = MP_ALLOC( RewardAccumulatedSalvage );
        if( verify( res ))
        {
            verify( res->item = salvage_GetItem( item->itemName ) );
            res->set_flags = set_flags;
            res->count = item->count;
        }
    }

    // --------------------
    // finally, return

    return res;

}

void rewardaccumulatedsalvage_Destroy( RewardAccumulatedSalvage *p )
{
    MP_FREE(RewardAccumulatedSalvage, p );
}

//------------------------------------------------------------
// helper for accumulating salvage in a reward. this prototype
//  is uniform for helping simplify the dispatch of accumulating items by type.
// see Reward.c: static s_rewardaccumulated_accumFuncs[] for usage
// -AB: created :2005 Feb 18 10:10 AM
//----------------------------------------------------------
static void rewardaccumulatedsalvage_Accum(const RewardItem* item, RewardAccumulator* reward, int deflevel, RewardSetFlags set_flags)
{
    RewardAccumulatedSalvage *p;
    p = rewardaccumulatedsalvage_Create( item, deflevel, set_flags );
    if( verify( p ) )
        eaPush( &reward->salvages, p);
}

// ============================================================
// RewardAccumulatedConcept

MP_DEFINE(RewardAccumulatedConcept);
static RewardAccumulatedConcept* rewardaccumulatedconcept_Create(const RewardItem *item, int defLevel, RewardSetFlags set_flags )
{
    RewardAccumulatedConcept* res = NULL;

    if( verify( item && item->type == kRewardItemType_Concept ))
    {
        // create the pool, arbitrary number
        MP_CREATE(RewardAccumulatedConcept, 16);
        res = MP_ALLOC( RewardAccumulatedConcept );
        if( verify( res ))
        {
            verify( res->item = conceptdef_Get( item->itemName ) );
            res->set_flags = set_flags;
        }
    }

    // --------------------
    // finally, return

    return res;

}

static void rewardaccumulatedconcept_Destroy( RewardAccumulatedConcept *p )
{
    MP_FREE(RewardAccumulatedConcept, p );
}

static void rewardaccumulatedconcept_Accum(const RewardItem* item, RewardAccumulator* reward, int deflevel, RewardSetFlags set_flags)
{
    RewardAccumulatedConcept *p;
    p = rewardaccumulatedconcept_Create( item, deflevel, set_flags );
    if( verify( p ) )
        eaPush( &reward->concepts, p);
}

// ============================================================
// RewardAccumulatedProficiency

MP_DEFINE(RewardAccumulatedProficiency);
static RewardAccumulatedProficiency* rewardaccumulatedproficiency_Create(const RewardItem *item, int defLevel, RewardSetFlags set_flags )
{
    RewardAccumulatedProficiency* res = NULL;

    if( verify( item && item->type == kRewardItemType_Proficiency ))
    {
        // create the pool, arbitrary number
        MP_CREATE(RewardAccumulatedProficiency, 16);
        res = MP_ALLOC( RewardAccumulatedProficiency );
        if( verify( res ))
        {
            verify( res->item = proficiency_GetItem( item->itemName) );
            res->set_flags = set_flags;
        }
    }

    // --------------------
    // finally, return

    return res;
}

static void rewardaccumulatedproficiency_Destroy( RewardAccumulatedProficiency *p )
{
    MP_FREE(RewardAccumulatedProficiency, p );
}

static void rewardaccumulatedproficiency_Accum(const RewardItem* item, RewardAccumulator* reward, int deflevel, RewardSetFlags set_flags)
{
    RewardAccumulatedProficiency *p;
    p = rewardaccumulatedproficiency_Create( item, deflevel, set_flags );
    if( verify( p ) )
        eaPush( &reward->proficiencies, p);
}



// ============================================================
// RewardAccumulatedDetailRecipe

MP_DEFINE(RewardAccumulatedRewardTokenCount);
static RewardAccumulatedRewardTokenCount* rewardaccumulatedrewardtokencount_Create(const RewardItem *item, int defLevel, RewardSetFlags set_flags )
{
    RewardAccumulatedRewardTokenCount* res = NULL;

    if( verify( item && item->type == kRewardItemType_RewardTokenCount ))
    {
        // create the pool, arbitrary number
        MP_CREATE(RewardAccumulatedRewardTokenCount, 16);
        res = MP_ALLOC( RewardAccumulatedRewardTokenCount );
        if( verify( res ))
        {
            res->tokenName = item->itemName;
            res->count = item->count;
        }
    }

    // --------------------
    // finally, return

    return res;
}

static void rewardaccumulatedrewardtokencount_Destroy( RewardAccumulatedDetailRecipe *p )
{
    MP_FREE(RewardAccumulatedRewardTokenCount, p );
}

static void rewardaccumulatedrewardtokencount_Accum(const RewardItem* item, RewardAccumulator* reward, int deflevel, RewardSetFlags set_flags)
{
    RewardAccumulatedRewardTokenCount *p;
    p = rewardaccumulatedrewardtokencount_Create( item, deflevel, set_flags );
    if( verify( p ) )
        eaPush( &reward->rewardTokenCount, p);
}



// ============================================================
// RewardAccumulatedDetailRecipe

MP_DEFINE(RewardAccumulatedDetailRecipe);
static RewardAccumulatedDetailRecipe* rewardaccumulateddetailrecipe_Create(const RewardItem *item, int defLevel, RewardSetFlags set_flags )
{
    RewardAccumulatedDetailRecipe* res = NULL;

    if( verify( item && item->type == kRewardItemType_DetailRecipe ))
    {
        // create the pool, arbitrary number
        MP_CREATE(RewardAccumulatedDetailRecipe, 16);
        res = MP_ALLOC( RewardAccumulatedDetailRecipe );
        if( verify( res ))
        {
            // Arbitrary cap for recipes
            if (defLevel > 50)
                defLevel = 50;

            res->item = detailrecipedict_RecipeFromName( item->itemName );
            if (!res->item)
            {
                int min = detailrecipedict_CatagoryMin(item->itemName);
                int max = detailrecipedict_CatagoryMax(item->itemName);

                // checking limits for category
                if (min > 0 && defLevel < min)
                    defLevel = min;
                if (max > 0 && defLevel > max)
                    defLevel = max;

                res->item = detailrecipedict_RecipeFromCatagoryLevel( item->itemName, defLevel);
                if (!res->item)
                {
                    if(!(res->item = detailrecipedict_RecipeFromCatagoryLevel( item->itemName, 0) ))
                    {
                        Errorf("unable to find recipe for item: %s.%d", item->itemName, defLevel);
                    }
                }
            }
            res->set_flags = set_flags;
            res->count = item->count;
            res->unlimited = item->unlimited;
        }
    }

    // --------------------
    // finally, return

    return res;
}

static void rewardaccumulateddetailrecipe_Destroy( RewardAccumulatedDetailRecipe *p )
{
    MP_FREE(RewardAccumulatedDetailRecipe, p );
}

static void rewardaccumulateddetailrecipe_Accum(const RewardItem* item, RewardAccumulator* reward, int deflevel, RewardSetFlags set_flags)
{
    RewardAccumulatedDetailRecipe *p;
    p = rewardaccumulateddetailrecipe_Create( item, deflevel, set_flags );
    if( verify( p ) )
        eaPush( &reward->detailRecipes, p);
}

// ============================================================
// RewardAccumulatedDetail

MP_DEFINE(RewardAccumulatedDetail);
static RewardAccumulatedDetail* rewardaccumulateddetail_Create(const RewardItem *item, int defLevel, RewardSetFlags set_flags )
{
    RewardAccumulatedDetail* res = NULL;

    if( verify( item && item->type == kRewardItemType_Detail ))
    {
        // create the pool, arbitrary number
        MP_CREATE(RewardAccumulatedDetail, 16);
        res = MP_ALLOC( RewardAccumulatedDetail );
        if( verify( res ))
        {
            verify( res->item = basedata_GetDetailByName( item->itemName) );
            res->set_flags = set_flags;
        }
    }

    // --------------------
    // finally, return

    return res;
}

static void rewardaccumulateddetail_Destroy( RewardAccumulatedDetail *p )
{
    MP_FREE(RewardAccumulatedDetail, p );
}

static void rewardaccumulateddetail_Accum(const RewardItem* item, RewardAccumulator* reward, int deflevel, RewardSetFlags set_flags)
{
    RewardAccumulatedDetail *p;
    p = rewardaccumulateddetail_Create( item, deflevel, set_flags );
    if( verify( p ) )
        eaPush( &reward->details, p);
}

// ============================================================
// RewardAccumulatedToken

MP_DEFINE(RewardAccumulatedToken);
static RewardAccumulatedToken* rewardaccumulatedtoken_Create( const RewardItem *item, int defLevel, RewardSetFlags set_flags )
{
    RewardAccumulatedToken* res = NULL;

    if( verify( item && item->type == kRewardItemType_Token ))
    {
        // create the pool, arbitrary number
        MP_CREATE(RewardAccumulatedToken, 16);
        res = MP_ALLOC( RewardAccumulatedToken );
        if( verify( res ))
        {
            verify( res->tokenName = item->itemName );
            res->remove = item->remove;
        }
    }

    // --------------------
    // finally, return

    return res;

}

void rewardaccumulatedtoken_Destroy( RewardAccumulatedToken *p )
{
    MP_FREE(RewardAccumulatedToken, p );
}

static void rewardaccumulatedtoken_Accum(const RewardItem* item, RewardAccumulator* reward, int deflevel, RewardSetFlags set_flags)
{
    RewardAccumulatedToken *p;
    p = rewardaccumulatedtoken_Create( item, deflevel, set_flags );
    if( verify( p ) )
        eaPush( &reward->tokens, p);
}

MP_DEFINE(RewardAccumulatedRewardTable);
static RewardAccumulatedRewardTable* rewardaccumulatedrewardtable_Create(const RewardItem *item, int defLevel, RewardSetFlags set_flags )
{
    RewardAccumulatedRewardTable* res = NULL;

    if( verify( item && item->type == kRewardItemType_RewardTable ))
    {
        // create the pool, arbitrary number
        MP_CREATE(RewardAccumulatedRewardTable, 16);
        res = MP_ALLOC( RewardAccumulatedRewardTable );
        if( verify( res ))
        {
            res->tableName = item->itemName;
            res->set_flags = set_flags;
            res->rewardTableFlags = item->rewardTableFlags;
            res->level = MINMAX(defLevel,0,MAX_PLAYER_SECURITY_LEVEL);
        }
    }

    // --------------------
    // finally, return

    return res;
}

static void rewardaccumulatedrewardtable_Destroy( RewardAccumulatedRewardTable *p )
{
    MP_FREE(RewardAccumulatedRewardTable, p );
}

static void rewardaccumulatedrewardtable_Accum(const RewardItem* item, RewardAccumulator* reward, int deflevel, RewardSetFlags set_flags)
{
    RewardAccumulatedRewardTable *p;
    p = rewardaccumulatedrewardtable_Create( item, deflevel, set_flags );
    if( verify( p ) )
        eaPush( &reward->rewardTables, p);
}

// ============================================================
// RewardAccumulatedIncarnatePoints

MP_DEFINE(RewardAccumulatedIncarnatePoints);
static RewardAccumulatedIncarnatePoints* rewardAccumulatedIncarnatePoints_Create(int subtype, int count)
{
    RewardAccumulatedIncarnatePoints* reward = NULL;

    // create the pool, arbitrary number
    MP_CREATE(RewardAccumulatedIncarnatePoints, 16);
    reward = MP_ALLOC(RewardAccumulatedIncarnatePoints);
    if (verify(reward))
    {
        reward->count = count;
        reward->subtype = subtype;
    }

    return reward;
}

static void rewardAccumulatedIncarnatePoints_Destroy(RewardAccumulatedIncarnatePoints *p)
{
    MP_FREE(RewardAccumulatedIncarnatePoints, p );
}

static void rewardAccumulatedIncarnatePoints_Accum(const RewardItem* item, RewardAccumulator* reward, int deflevel, RewardSetFlags set_flags)
{
    RewardAccumulatedIncarnatePoints *p;
    if (verify(item && item->type == kRewardItemType_IncarnatePoints))
    {
        p = rewardAccumulatedIncarnatePoints_Create(atoi(StaticDefineLookup(ParseIncarnateDefines, item->itemName)), item->count);
        if (verify(p))
        {
            eaPush(&reward->incarnatePoints, p);
        }
    }
}

// ============================================================
// RewardAccumulatedAccountProduct

MP_DEFINE(RewardAccumulatedAccountProduct);
static RewardAccumulatedAccountProduct* rewardAccumulatedAccountProduct_Create(const char *productCode, int count, int rarity)
{
    RewardAccumulatedAccountProduct* reward = NULL;

    // create the pool, arbitrary number
    MP_CREATE(RewardAccumulatedAccountProduct, 16);
    reward = MP_ALLOC(RewardAccumulatedAccountProduct);
    if (verify(reward))
    {
        strncpy(reward->productCode, productCode, 8);
        reward->productCode[8] = '\0';
        reward->count = count;
        reward->rarity = rarity;
    }

    return reward;
}

static void rewardAccumulatedAccountProduct_Destroy(RewardAccumulatedAccountProduct *p)
{
    MP_FREE(RewardAccumulatedAccountProduct, p );
}

static void rewardAccumulatedAccountProduct_Accum(const RewardItem* item, RewardAccumulator* reward, int deflevel, RewardSetFlags set_flags)
{
    RewardAccumulatedAccountProduct *p;
    if (verify(item && item->type == kRewardItemType_AccountProduct))
    {
        p = rewardAccumulatedAccountProduct_Create(item->itemName, item->count, item->rarity);
        if (verify(p))
        {
            eaPush(&reward->accountProducts, p);
        }
    }
}

// ============================================================
// accumulation helpers

typedef void (*rewardaccumulated_AccumFuncType)(const RewardItem* item, RewardAccumulator* reward, int deflevel, RewardSetFlags set_flags);

// type lookup table. MUST BE IN RewardItemType ORDER
static rewardaccumulated_AccumFuncType s_rewardaccumulated_accumFuncs[] =
    {
        rewardaccumulatedpower_Accum,
        rewardaccumulatedsalvage_Accum,
        rewardaccumulatedconcept_Accum,
        rewardaccumulatedproficiency_Accum,
        rewardaccumulateddetailrecipe_Accum,
        rewardaccumulateddetail_Accum,
        rewardaccumulatedtoken_Accum,
        rewardaccumulatedrewardtable_Accum,
        rewardaccumulatedrewardtokencount_Accum,
        rewardAccumulatedIncarnatePoints_Accum,
        rewardAccumulatedAccountProduct_Accum,
    };
STATIC_ASSERT(ARRAY_SIZE(s_rewardaccumulated_accumFuncs) == kRewardItemType_Count);


// END Accumulated helpers
// ------------------------------------------------------------

//------------------------------------------------------------
// converts the items in a RewardItemSet into RewardAccumulator
// items
//----------------------------------------------------------
static void rewardItemSetAward(const RewardItemSet* set, RewardAccumulator* reward, Entity *e, int deflevel, RewardSetFlags set_flags, char **lootList)
{
    int randRoll;
    int totalChanceSum = 0; // sum of all chance fields in the item set.
    int chanceSum = 0;
    int i;
    const RewardItem **items = set->rewardItems;
    
    const RewardItem **pickFrom = NULL;

    eaCreateConst(&pickFrom);
    if(verify(set && reward && e && e->pl ))
    {
        int iSize;

        // Sum up all chance fields in drop groups.
        totalChanceSum = 0;
        for(i = eaSize(&items)-1; i >= 0; i--)
        {
            bool addMe = true;

            if (items[i]->chanceRequires != NULL && !chareval_Eval(e->pchar, items[i]->chanceRequires, set->filename))
            {
                addMe = false;
            }

            // This lovely little ball of hack is to prevent the system from rewarding the same unique item twice
            //   in the same Super Pack, so the account transaction doesn't fail...
            // There are still edge cases where the # of product added + the # of product owned > the # of product allowed,
            //   in which case the multi transaction still fails...
            if (items[i]->type == kRewardItemType_AccountProduct && lootList)
            {
                const AccountProduct *product = accountCatalogGetProduct(skuIdFromString(items[i]->itemName));

                if (product->grantLimit == 1)
                {
                    int index;

                    for (index = eaSize(&reward->accountProducts) - 1; index >= 0; index--)
                    {
                        if (!stricmp(reward->accountProducts[index]->productCode, items[i]->itemName))
                        {
                            addMe = false;
                        }
                    }
                }
            }

            if (addMe)
            {
                totalChanceSum += items[i]->chance;
                eaPushConst(&pickFrom, items[i]);
            }
        }

        if (totalChanceSum==0)
            return;

        randRoll = rand() % totalChanceSum;
        if(rewardPrintDebugInfo)
            printf("\t\trandroll = %i / %i\n", randRoll, totalChanceSum);

        // Find out in which drop group the roll fell into.

        // accumulate the items
        iSize = eaSize(&pickFrom);
        for(i = 0; i < iSize;  i++)
        {
            chanceSum += pickFrom[i]->chance;
            if(chanceSum > randRoll)
            {
                const RewardItem *item = pickFrom[i];
                bool canReward = true;

                // --------------------
                // filter by type

//                switch ( item->type )
//                {
//                    case kRewardItemType_Salvage:
//                    {
//                    }
//                    break;
//                };

                // --------------------
                // dispatch the accum function

                if( canReward && verify( INRANGE( item->type, 0, ARRAY_SIZE(s_rewardaccumulated_accumFuncs) ) ) )
                {
                    s_rewardaccumulated_accumFuncs[item->type](item, reward, deflevel, set_flags);

                    // --------------------
                    // logging

                    if(rewardPrintDebugInfo)
                        printf("\tAwarding Item %s (%d (chanceSum) > %d (randRoll))\n", items[i]->power.power, chanceSum, randRoll);

                    break; // BREAK IN FLOW
                }
            }
        }
    }
}

RAddPowerResult rewardAddPower(Entity* e, const PowerNameRef* power, int deflevel, bool bAllowBoost, RewardSource source)
{
    int result = RAP_OK;
    const BasePower* basePower;
    int bArchitect = 0, bArchitectTest = 0, bArchitectDevChoice = 0;
    TeamRewardDebug * pDebug = 0;
    int bCheckFallback = true;

    if( source == REWARDSOURCE_DEBUG )
        pDebug = getRewardDebug(e->db_id);

    if(g_ArchitectTaskForce && !rewardSourceAllowedArchitect(source))
    {
        if( (g_ArchitectTaskForce->architect_flags&ARCHITECT_TEST) )
            bArchitectTest = 1;

        if( !(g_ArchitectTaskForce->architect_flags&ARCHITECT_DEVCHOICE) ) // dev choice is treated like a normal mission
            bArchitect = 1;
        else
            bArchitectDevChoice = 1;
    }

    // Add the thing to the player's inventory.
    basePower = powerdict_GetBasePowerByName(&g_PowerDictionary, power->powerCategory, power->powerSet, power->power);
    if(!basePower)
    {
        return RAP_NO_ROOM;
    }

    // check for reward requires
    while (bCheckFallback && e->pchar && basePower && basePower->ppchRewardRequires)
    {
        if (!chareval_Eval(e->pchar, basePower->ppchRewardRequires, basePower->pchSourceFile))
        {
            if (basePower->pchRewardFallback)
                // fallback will retain level from initially requested power 
                basePower = powerdict_GetBasePowerByString(&g_PowerDictionary, basePower->pchRewardFallback, NULL);
            else 
                basePower = NULL;
        } else {
            bCheckFallback = false;
        }

    }

    if(!basePower)
    {
        return RAP_NO_ROOM;
    }

    switch(basePower->eType)
    {
    case kPowerType_Boost:
        {
            int level;

            if(power->fixedlevel)
                level = power->level;
            else
                level = deflevel+power->level;

            DATA_MINE_ENHANCEMENT(basePower,level-1,"in",gRewardSourceLabels[source],1);

            if(!bArchitect && bAllowBoost)
            {
                if( pDebug )
                {
                    pDebug->enhancement_cnt++;
                    break;
                }

                if(level < 1)
                    level = 1;
                else if(level > MAX_PLAYER_SECURITY_LEVEL+3)
                    level = MAX_PLAYER_SECURITY_LEVEL+3;

                result = character_AddBoost(e->pchar, basePower, level-1, 0, NULL);
                if(result>=0)
                {
                    const char *pchFloater= basePower->pchDisplayFloatRewarded ? basePower->pchDisplayFloatRewarded : "FloatFoundEnhancement";

                    sendInfoBox(e, INFO_REWARD, "SpecializationYouReceived", basePower->pchDisplayName);
                    LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Enh %s", basePower->pchName);
                    serveFloater(e, e, pchFloater, 0.0f, kFloaterStyle_Attention, 0);

                    if (getPopHelpStateByTag(e, "CODEPH_Got_Enhancement") == PopHelpState_Untriggered)
                    {
                        triggerPopHelpEventHappenedByTag(e, "CODEPH_Got_Enhancement");
                    }

                    if (boost_IsOriginSpecific(basePower) && getPopHelpStateByTag(e, "CODEPH_Got_Origin_Enhancement") == PopHelpState_Untriggered)
                    {
                        triggerPopHelpEventHappenedByTag(e, "CODEPH_Got_Origin_Enhancement");
                    }
                }
                else
                    LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Fail:Enh %s (full inventory)", basePower->pchName);
            }
            else
            {
                // Currently, this means they are exemplared, but it's possible there will be other
                //   reasons in the future that they aren't allowed a boost
                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Fail:Enh %s (exemplared)", basePower->pchName);
                DATA_MINE_ENHANCEMENT(basePower,level-1,"out","handicapped",1);
            }
        }
        break;
    case kPowerType_Inspiration:
        if(!bArchitectTest)
        {
            if( pDebug )
            {
                pDebug->inspirations++;
                break;
            }

            result = character_AddInspiration(e->pchar, basePower, gRewardSourceLabels[source]);

            if(result>=0 )
            {
                const char *pchFloater= basePower->pchDisplayFloatRewarded ? basePower->pchDisplayFloatRewarded : "FloatFoundInspiration";
                if( bArchitect || bArchitectDevChoice )
                    badge_StatAddArchitectAllowed(e, "Architect.Inspirations", 1, 0 );

                sendInfoBox(e, INFO_REWARD, "InspirationYouReceived", basePower->pchDisplayName);
                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Insp %s", basePower->pchName);
                serveFloater(e, e, pchFloater, 0.0f, kFloaterStyle_Attention, 0);
            }
            else
                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Fail:Insp  %s (full inventory)", basePower->pchName);
        }
        break;
    default:
        result = RAP_BAD_REWARD_POWER;
        if(power->remove)
        {
            LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Remove:Tpow %s", dbg_BasePowerStr(basePower));
            character_RemoveBasePower(e->pchar, basePower);
            result = RAP_OK;
        }
        else
        {
            if( pDebug )
                break;

            if(character_AddRewardPower(e->pchar, basePower))
            {
                result = RAP_OK;
                if( stricmp(basePower->psetParent->pchName, "Accolades") == 0 )
                {
                    const char *pchFloater= basePower->pchDisplayFloatRewarded ? basePower->pchDisplayFloatRewarded : "FloatFoundAccoladePower";

                    sendInfoBox(e, INFO_REWARD, "TempPowerYouReceived", basePower->pchDisplayName);
                    LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Apow  %s", dbg_BasePowerStr(basePower));

                    serveFloater(e, e, pchFloater, 0.0f, kFloaterStyle_Attention, 0);
                    power_sendTrayAdd( e, basePower );
                }
                else if( stricmp(basePower->psetParent->pchName, "Temporary_Powers") == 0 )
                {
                    const char *pchFloater= basePower->pchDisplayFloatRewarded ? basePower->pchDisplayFloatRewarded : "FloatFoundTempPower";

                    sendInfoBox(e, INFO_REWARD, "TempPowerYouReceived", basePower->pchDisplayName);
                    LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Tpow %s", dbg_BasePowerStr(basePower));
                    serveFloater(e, e, pchFloater, 0.0f, kFloaterStyle_Attention, 0);
                    power_sendTrayAdd( e, basePower );
                }
                else
                {
                    const char *pchFloater= basePower->pchDisplayFloatRewarded ? basePower->pchDisplayFloatRewarded : "silent";

                    LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Tpow %s", dbg_BasePowerStr(basePower));
                    serveFloater(e, e, pchFloater, 0.0f, kFloaterStyle_Attention, 0);
                }
            }
        }
        break;
    }

    if (result > RAP_OK)
        result = RAP_OK;

    return result;
}


//------------------------------------------------------------------------
// Reward Restrictions
//------------------------------------------------------------------------


// returns -1 for default group
// returns 0 for unrestricted group (catch all)
static int numVillainGroupsAllowed(const RewardSet *set)
{
    if (set->catchUnlistedVGs)
        return -1;

    if (set->groups)
        return eaiSize(&set->groups);

    return 0;
}

static int numOriginsAllowed(const RewardSet *set)
{
    if (set->catchUnlistedOrigins)
        return -1;

    if (set->origins)
        return eaSize(&set->origins);

    return 0;
}

static int numArchetypesAllowed(const RewardSet *set)
{
    if (set->catchUnlistedArchetypes)
        return -1;

    if (set->archetypes)
        return eaSize(&set->archetypes);

    return 0;
}

// returns:
// 0 if villain group is not allowed by this reward set
// 1 if villain group is allowed by this reward set and there is a restriction
// 2 if villain group is allowed by this reward set because there is no restriction
static int isVillainGroupOK(const RewardSet *set, VillainGroupEnum vgroup)
{
    int num, i;
    if ((num = numVillainGroupsAllowed(set)) > 0)
    {
        for (i = 0; i < num; i++)
        {
            if (vgroup == set->groups[i])
                return 1;
        }

        if(rewardPrintDebugInfo)
            printf("Skipping reward set: villain group does not match reward set villain group\n");
        return 0;
    }

    return 2;
}

static int isOriginOK(const RewardSet *set, const char *playerOrigin)
{
    int num, i;
    if (playerOrigin && (num = numOriginsAllowed(set)) > 0)
    {
        for (i = 0; i < num; i++)
        {
            if (0 == stricmp(playerOrigin, set->origins[i]))
                return 1;
        }

        if(rewardPrintDebugInfo)
            printf("Skipping reward set: player origin does not match reward set origin\n");
        return 0;
    }

    return 2;
}

static int isArchetypeOK(const RewardSet *set, const char *playerArchetype)
{
    int num, i;
    if (playerArchetype && (num = numArchetypesAllowed(set)) > 0)
    {
        for (i = 0; i < num; i++)
        {
            if (0 == stricmp(playerArchetype, set->archetypes[i]))
                return 1;
        }

        if(rewardPrintDebugInfo)
            printf("Skipping reward set: player archetype does not match reward set archetype\n");
        return 0;
    }

    return 2;
}

//------------------------------------------------------------------------
// Shared Reward interface
//------------------------------------------------------------------------

static void s_rewardSetsAwardDatamine(const RewardSet *set, const char *rewardTableName, int rewardTableLevel, bool hit)
{
    if(rewardTableName && set->dropGroups && !(set->flags & RSF_ALWAYS))
    {
        int i;
        char* name = NULL;
        int chance = set->chance * 1000;

        for(i = eaSize(&set->dropGroups[0]->itemSetNames)-1; i >= 0; --i)
        {
            const char *itemSetName = set->dropGroups[0]->itemSetNames[i];
            if(itemSetName[0] && itemSetName[1] && itemSetName[2] == '.' && strnicmp(itemSetName,"RT",2))
                itemSetName += 3;
            estrPrintf(&name,"rewardhits.%s.L%i.C%i.%s",rewardTableName,rewardTableLevel,chance,itemSetName);
            MiningAccumulatorAdd(name,(hit ? "Hit" : "Miss"),1);
        }

        estrDestroy(&name);
    }
}

void rewardSetsAward(const RewardSet** rewardSets, RewardAccumulator* reward, Entity* player, VillainGroupEnum vgroup, int iLevel, const char *rewardTableName, int containerType, char **lootList)
{
    int i, check1, check2, check3, checkRequires;
    int usedRestrictedArchetype = 0;
    int usedRestrictedOrigin = 0;
    int usedRestrictedVG = 0;
    const RewardSet *set;
    F32 randRoll;
    const char* playerOrigin = NULL;
    const char* playerArchetype = NULL;

    if(player && player->pchar)
    {
        playerOrigin = player->pchar->porigin->pchName;
        playerArchetype = player->pchar->pclass->pchName;
    }


    // Award entity with each reward set.
    for(i = eaSize(&rewardSets)-1; i >= 0; i--)
    {
        set = rewardSets[i];

        // only use the "catch unlisted" reward sets when no applicable restricted set found
        if (set->catchUnlistedVGs || set->catchUnlistedArchetypes || set->catchUnlistedOrigins)
            continue;

        // Check villain group constraints:
        if ((check1 = isVillainGroupOK(set, vgroup)) == 1)
            usedRestrictedVG = 1;

        // Check origin constraints:
        if ((check2 = isOriginOK(set, playerOrigin)) == 1)
            usedRestrictedOrigin = 1;

        // Check archetype constraints:
        if ((check3 = isArchetypeOK(set, playerArchetype)) == 1)
            usedRestrictedArchetype = 1;

        // Check if there are any requires for this set
        if (set->rewardSetRequires) {
            checkRequires = player && player->pchar && chareval_Eval(player->pchar, set->rewardSetRequires, set->filename);

            if(!checkRequires && rewardPrintDebugInfo)
                printf("Skipping reward set: player eval does not match reward set eval\n");

        } else {
            checkRequires = 1; // pass automatically if there is no specific requires
        }

        // see if the set failed a check:
        if (!check1 || !check2 || !check3 || !checkRequires)
            continue;

        // Award the reward
        randRoll = ((float)rand()/(float)RAND_MAX)*100.f;    // The RewardSet chance must surpass this number to be rewarded.

        if(rewardPrintDebugInfo)
        {
            printf("RewardSet %d: (%f (rand) < %f (chance)) %s\n", i, randRoll, set->chance, set->flags & RSF_ALWAYS?"(always)":"");
        }

        if(set->chance > randRoll || set->flags & RSF_ALWAYS)
        {
            rewardSetAward(set, reward, player, vgroup, rewarditemsettarget_BitsFromEnt( player ), containerType, lootList);
            s_rewardSetsAwardDatamine(set,rewardTableName,iLevel,true);
        }
        else
        {
            s_rewardSetsAwardDatamine(set,rewardTableName,iLevel,false);
        }
    }

    if (usedRestrictedVG && usedRestrictedOrigin && usedRestrictedArchetype)
        return;

    // only use the "catch unlisted" reward sets when no applicable restricted set found
    for(i = eaSize(&rewardSets)-1; i >= 0; i--)
    {
        int num;
        set = rewardSets[i];

        // skip if it is a restricted only set
        if (!set->catchUnlistedVGs && !set->catchUnlistedOrigins && !set->catchUnlistedArchetypes)
            continue;

        num = numVillainGroupsAllowed(set);
        if (num > 0 && !isVillainGroupOK(set, vgroup))
            continue;
        if (num < 0 && usedRestrictedVG) // DefaultVillainGroup
            continue;


        num = numOriginsAllowed(set);
        if (num > 0 && !isOriginOK(set, playerOrigin))
            continue;
        if (num < 0 && usedRestrictedOrigin) // DefaultOrigin
            continue;


        num = numArchetypesAllowed(set);
        if (num > 0 && !isArchetypeOK(set, playerArchetype))
            continue;
        if (num < 0 && usedRestrictedArchetype) // DefaultArchetype
            continue;

        // Check if there are any requires for this set
        if (set->rewardSetRequires) {
            checkRequires = player && player->pchar && chareval_Eval(player->pchar, set->rewardSetRequires, set->filename);

            if(!checkRequires) 
            {    
                if(rewardPrintDebugInfo)
                    printf("Skipping reward set: player eval does not match reward set eval\n");

                continue;
            }
        }

        randRoll = ((float)rand()/(float)RAND_MAX)*100.f;    // The RewardSet chance must surpass this number to be rewarded.

        if(rewardPrintDebugInfo)
        {
             printf("RewardSet %d: (%f (rand) < %f (chance)) %s\n", i, randRoll, set->chance, set->flags & RSF_ALWAYS?"(always)":"");
        }

        if(set->chance > randRoll || set->flags & RSF_ALWAYS)
        {
            rewardSetAward(set, reward, player, vgroup, rewarditemsettarget_BitsFromEnt(player), containerType, lootList);
            s_rewardSetsAwardDatamine(set,rewardTableName,iLevel,true);
        }
        else
        {
            s_rewardSetsAwardDatamine(set,rewardTableName,iLevel,false);
        }
    }

    reward->villainGrp = vgroup;
}

//------------------------------------------------------------
// Returns a shared RewardAccumulator that is properly initialized.
// RewardAccumulator* a static reward, do not use outside of controlled situations!
// -AB: refactored to use init :2005 Feb 18 10:49 AM
//----------------------------------------------------------
RewardAccumulator* rewardGetAccumulator(void)
{
    static RewardAccumulator reward = {0};

    rewardaccumulator_Init(&reward);

    return &reward;
}


//------------------------------------------------------------
// init the passed object, allocating the member earrays, or
//  clearing their length to zero, if already alloc'd
//----------------------------------------------------------
RewardAccumulator *rewardaccumulator_Init(RewardAccumulator *pReward)
{
    if( verify( pReward ))
    {
        rewardaccumulator_Cleanup(pReward, false);
    }

    return pReward;
}


//------------------------------------------------------------
// clean up the members.
// Cleanup rather than Destroy to diff it from an alloc'd object that
// is freed.
// pReward: the item to cleanup, it is not freed.
//----------------------------------------------------------
void rewardaccumulator_Cleanup(RewardAccumulator *pReward, bool freeEArrays )
{
    if(pReward)
    {
        pReward->experience = 0;
        pReward->influence = 0;
        pReward->wisdom = 0;
        pReward->prestige = 0;
//        pReward->rewardTable = 0; // ARM NOTE: This appears to be unused, so I'm pulling it.  Revert if I'm wrong.
        pReward->bonus_experience = 0;

        eaClearEx(&pReward->powers, rewardaccumulatedpower_Destroy);
        eaClearEx(&pReward->salvages, rewardaccumulatedsalvage_Destroy);
        eaClearEx(&pReward->concepts, rewardaccumulatedconcept_Destroy);
        eaClearEx(&pReward->proficiencies, rewardaccumulatedproficiency_Destroy);
        eaClearEx(&pReward->detailRecipes,rewardaccumulateddetailrecipe_Destroy);
        eaClearEx(&pReward->details, rewardaccumulateddetail_Destroy);
        eaClearEx(&pReward->tokens, rewardaccumulatedtoken_Destroy);
        eaClearEx(&pReward->rewardTables, rewardaccumulatedrewardtable_Destroy);
        eaClearEx(&pReward->rewardTokenCount, rewardaccumulatedrewardtokencount_Destroy);
        eaClearEx(&pReward->incarnatePoints, rewardAccumulatedIncarnatePoints_Destroy);
        eaClearEx(&pReward->accountProducts, rewardAccumulatedAccountProduct_Destroy);

        if(freeEArrays)
        {
            eaDestroy(&pReward->powers);
            eaDestroy(&pReward->salvages);
            eaDestroy(&pReward->concepts);
            eaDestroy(&pReward->proficiencies);
            eaDestroy(&pReward->detailRecipes);
            eaDestroy(&pReward->details);
            eaDestroy(&pReward->rewardTables);
            eaDestroy(&pReward->rewardTokenCount);
            eaDestroy(&pReward->incarnatePoints);
            eaDestroy(&pReward->accountProducts);
        }
    }
}

//------------------------------------------------------------------------
// Public Reward interface
//------------------------------------------------------------------------
void rewardDisplayDebugInfo(int display)
{
    rewardPrintDebugInfo = (display ? 1 : 0);
}

const char *rewardGetTableNameForRank(VillainRank rank, Entity * e)
{
    if( g_ArchitectTaskForce && rank == VR_PET )
    {
        // if we are in architect mode, get a pets rank from their class table
        if( e && e->pchar && e->pchar->pclass )
        {
            const char * pchClass =  e->pchar->pclass->pchName;  
            if( strstriConst(pchClass, "Minion_Small") )
                rank = VR_SMALL;
            else if( strstriConst(pchClass, "Henchman") || strstriConst(pchClass, "Minion") )
                rank = VR_MINION;
            else if( strstriConst(pchClass, "Arch") || strstriConst(pchClass, "Monster") )
                rank = VR_ARCHVILLAIN;
            else if( strstriConst(pchClass, "Boss_Elite") )
                rank = VR_ELITE;
            else if( strstriConst( pchClass, "Boss") )
                rank = VR_BOSS;
            else if( strstriConst( pchClass, "Lt_") )
                rank = VR_LIEUTENANT;
            else
                rank = VR_PET;
        }
    }
    return StaticDefineIntRevLookup(ParseVillainRankEnum, rank);
}

static char *rewardGetSalvageTableName(Entity *e)
{
    const RewardDef *rw;
    static char ach[1024];

    if(!e || !e->villainDef)
        return NULL;

    strcpy(ach, "Salvage_");
    strcat(ach, e->villainDef->name);
    rw = rewardFind(ach, e->pchar->iCombatLevel);
    if(!rw)
    {
        strcpy(ach, "Salvage_");
        strcat(ach, StaticDefineIntRevLookup(ParseVillainRankEnum, e->villainDef->rank));
        rw = rewardFind(ach, e->pchar->iCombatLevel);
    }

    if(rw)
        return ach;

    return NULL;
}

static void rewarditemsetGetAllPossibleSalvageId(const U32 **heaSalvageDropsId, const RewardItemSet *itemset)
{
    int i;
    for(i = eaSize(&itemset->rewardItems)-1; i >= 0; --i)
    {
        const SalvageItem *salvage;
        const RewardItem *item = itemset->rewardItems[i];
        if(!item || item->type != kRewardItemType_Salvage)
            continue;
        salvage = salvage_GetItem(item->itemName);
        if(salvage)
            eaiPushUnique(heaSalvageDropsId,salvage->salId);
    }
}

static void rewardsetGetAllPossibleSalvageId(const U32 **heaSalvageDropsId, RewardSet *set)
{
    int j, k;

    for(j = eaSize(&set->dropGroups)-1; j >= 0; --j)
    {
        const RewardDropGroup *dropgroup = set->dropGroups[j];
        if(!dropgroup)
            continue;
        for(k = eaSize(&dropgroup->itemSetNames)-1; k >= 0; --k)
        {
            const RewardItemSet *itemset;
            if(!dropgroup->itemSetNames[k] || !devassert(!strnicmp(dropgroup->itemSetNames[k],"PL.",3)))
                continue;
            itemset = stashFindPointerReturnPointerConst(g_RewardDefinition->itemSetDictionary.hashItemSets, dropgroup->itemSetNames[k]+3);
            if(!itemset)
                continue;
            rewarditemsetGetAllPossibleSalvageId(heaSalvageDropsId,itemset);
        }
        for(k = eaSize(&dropgroup->itemSets)-1; k >= 0; --k)
        {
            const RewardItemSet *itemset = dropgroup->itemSets[k];
            if(!itemset)
                continue;
            rewarditemsetGetAllPossibleSalvageId(heaSalvageDropsId,itemset);
        }
    }
}

static void rewardtableGetAllPossibleSalvageId(const U32 **heaSalvageDropsId, const char *pchRewardTable, int level, VillainGroupEnum vgroup)
{
    char salvage_table[1024] = {0};
    const RewardDef *reward;
    int i;
    bool used_restricted = false;

    if(!heaSalvageDropsId || !pchRewardTable)
        return;

    strcpy(salvage_table,pchRewardTable);
    strcat(salvage_table,"_Salvage");

    reward = rewardFind(salvage_table,level+1);
    if(!reward)
        return;

    for(i = eaSize(&reward->rewardSets)-1; i >= 0; --i)
    {
        RewardSet *set = reward->rewardSets[i];
        if(set && !set->catchUnlistedVGs && isVillainGroupOK(set, vgroup) == 1)
        {
            rewardsetGetAllPossibleSalvageId(heaSalvageDropsId,set);
            used_restricted = true;
        }
    }
    if(used_restricted)
        return;

    for(i = eaSize(&reward->rewardSets)-1; i >= 0; --i)
    {
        RewardSet *set = reward->rewardSets[i];
        if(set && set->catchUnlistedVGs)
            rewardsetGetAllPossibleSalvageId(heaSalvageDropsId,set);
    }
}

void rewardGetAllPossibleSalvageId(const U32 **heaSalvageDropsId, Entity *target)
{
    int i;
    if(!target || !target->pchar)
        return;

    if (g_ArchitectTaskForce && (!(g_ArchitectTaskForce->architect_flags&ARCHITECT_DEVCHOICE)) && OnMissionMap() ) 
    {
        return;
    }
    rewardtableGetAllPossibleSalvageId(heaSalvageDropsId,rewardGetTableNameForRank(target->villainRank, target),target->pchar->iCombatLevel,target->villainDef->group);
    for(i = eaSize(&target->villainDef->additionalRewards)-1; i >= 0; --i)
        rewardtableGetAllPossibleSalvageId(heaSalvageDropsId,target->villainDef->additionalRewards[i],target->pchar->iCombatLevel,target->villainDef->group);
}

const RewardDef** rewardFindDefs(const char *nameTable)
{
    const RewardTable *rewardTable = stashFindPointerReturnPointerConst(g_RewardDefinition->rewardDictionary.hashRewardTable, nameTable);

    if( rewardTable )
        return rewardTable->ppDefs;
    else
        return NULL;
}

int rewardIsPermanentRewardToken( const char * pchRewardTable )
{
    const RewardTable *rewardTable = stashFindPointerReturnPointerConst(g_RewardDefinition->rewardDictionary.hashRewardTable, pchRewardTable);

    if( rewardTable && rewardTable->permanentRewardToken )
        return 1;
    return 0;
}

int rewardIsRandomItemOfPower( const char * pchRewardTable )
{
    const RewardTable *rewardTable = stashFindPointerReturnPointerConst(g_RewardDefinition->rewardDictionary.hashRewardTable, pchRewardTable);

    if( rewardTable && rewardTable->randomItemOfPower )
        return 1;
    return 0;
}

int reward_FindMeritCount( const char *storyarcName, int success, int checkBonus)
{
    const MeritRewardStoryArc *meritStoryArc = NULL;

    if (!storyarcName)
        return -1;
        
    meritStoryArc = stashFindPointerReturnPointerConst(g_RewardDefinition->meritRewardDictionary.hashMeritReward, storyarcName);

    if( meritStoryArc )
    {
        if (checkBonus)
        {
            return meritStoryArc->bonusMerits;
        }
        else
        {
            if (success)
                return meritStoryArc->merits;
            else 
                return meritStoryArc->failedmerits;
        }
    }
    return -1;
}

const char *reward_FindMeritTokenName( const char *storyarcName, int checkBonus)
{
    const MeritRewardStoryArc *meritStoryArc = NULL;

    if (!storyarcName)
        return NULL;

    meritStoryArc = stashFindPointerReturnPointerConst(g_RewardDefinition->meritRewardDictionary.hashMeritReward, storyarcName);

    if( meritStoryArc )
    {
        if (checkBonus)
        {
            return isNullOrNone(meritStoryArc->bonusToken) ? NULL : meritStoryArc->bonusToken;
        }
        else
        {
            if (stricmp("none", meritStoryArc->rewardToken) == 0)
                return NULL;
            else
                return meritStoryArc->rewardToken;
        }
    }

    return NULL;
}

const char *reward_FindBonusTableName( const char *storyarcName, int replaying)
{
    const MeritRewardStoryArc *meritStoryArc = NULL;

    if (!storyarcName)
        return NULL;

    meritStoryArc = stashFindPointerReturnPointerConst(g_RewardDefinition->meritRewardDictionary.hashMeritReward, storyarcName);

    if( meritStoryArc )
    {
        if (replaying)
            return isNullOrNone(meritStoryArc->bonusReplayTable) ? NULL : meritStoryArc->bonusReplayTable;
        else
            return isNullOrNone(meritStoryArc->bonusOncePerEpochTable) ? NULL : meritStoryArc->bonusOncePerEpochTable;
    }

    return NULL;
}

// Assumes the level value is 1 based.
const RewardDef* rewardFind(const char *nameTable, int level)
{
    int i;
    const RewardDef** defs = rewardFindDefs(nameTable);

    if(defs)
    {
        for(i=eaSize(&defs)-1; i>=0; i--)
        {
            if(defs[i]->level == level)
            {
                return defs[i];
            }
        }
    }

    return NULL;
}


bool rewardApplyLogWrapper(RewardAccumulator* reward, Entity *e, bool bGivePowers, bool bHandicapped, RewardSource source)
{
    float fPrestigeMod;
    float fInfluenceMod;
    bool bExemplar;
    bool bHadLoot;

    if(!e->pchar) return false;

    bExemplar = character_IsRewardedAsExemplar(e->pchar);

    if(!s_sbRewardLog.buff)
        initStuffBuff(&s_sbRewardLog, 1024);

    // Prefix the entry
    addStringToStuffBuff(&s_sbRewardLog, "RewardSet: %s\n{\n",e->name);

    // Log numeric rewards
    rewardGetSupergroupMods(e, &fPrestigeMod, &fInfluenceMod);
    fInfluenceMod += e->pchar->attrCur.fInfluenceGain;
    fPrestigeMod += e->pchar->attrCur.fPrestigeGain;

    addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d  %2d %2d |                     |                         |  %3.2f  %3.2f | %6d%s%6d%s%6d%s%6d%s",
                    e->name, e->db_id, e->pchar->iCombatLevel+1, e->pchar->iLevel+1,
                    fInfluenceMod, fPrestigeMod,
                    reward->experience, " ",
                    (int)(reward->influence*fInfluenceMod), " ",
                    reward->wisdom, " ",
                    (int)(reward->prestige*fPrestigeMod), " ");

    // Log power rewards
    logRewards(reward, bHandicapped, bExemplar, bGivePowers);

    // Actually do our application
    bHadLoot = rewardApply(reward, e, bGivePowers, bHandicapped, source, NULL);

    // Dump the log
    addStringToStuffBuff(&s_sbRewardLog, "}\n");
    LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "%s", s_sbRewardLog.buff);

    // clear out reward log.
    clearStuffBuff(&s_sbRewardLog);

    // If we actually awarded anything, return true
    return bHadLoot;
}

void levelupApply(Entity *e, int oldLevel)
{
    int iNewLevel;
    char time_buffer[100];
    char date_buffer_1[100], date_buffer_2[100];
    // This recalcs the combat level, and makes sure the character is
    // set up properly. It doesn't set the official level.
    Power * pInsp;

    if(!e || !e->pchar)
    {
        LOG( LOG_ERROR, LOG_LEVEL_VERBOSE, 0, "Warning: Attempt to apply levelup to a non-existant entity" );
        return;
    }

    e->general_update = 1;
    e->pchar->bRecalcStrengths = true;

    if(!e->pchar->attrCur.fHitPoints)
    {    
        pInsp = inspiration_Create(basePower_inspirationGetByName("Large", "Restoration" ), e->pchar, -1);
        character_AccrueBoosts(e->pchar, pInsp, 0.0f);
        character_ApplyPower(e->pchar, pInsp, erGetRef(e), e->pchar->vecLocationCurrent,0, 1);

        e->pchar->attrCur.fHitPoints = e->pchar->attrMax.fHitPoints;
        e->pchar->attrCur.fEndurance = e->pchar->attrMax.fEndurance;

        modStateBits(e, ENT_DEAD, 0, FALSE);
        e->pchar->bRecalcEnabledState = true;
    }
    else
    {
        e->pchar->attrCur.fHitPoints = e->pchar->attrMax.fHitPoints;
        e->pchar->attrCur.fEndurance = e->pchar->attrMax.fEndurance;
    }

    pInsp = inspiration_Create(basePower_inspirationGetByName("Large", "Phenomenal_Luck" ), e->pchar, -1);
    character_AccrueBoosts(e->pchar, pInsp, 0.0f);
    character_ApplyPower(e->pchar, pInsp, erGetRef(e), e->pchar->vecLocationCurrent,0, 1);

    pInsp = inspiration_Create(basePower_inspirationGetByName("Large", "Robust" ), e->pchar, -1);
    character_AccrueBoosts(e->pchar, pInsp, 0.0f);
    character_ApplyPower(e->pchar, pInsp, erGetRef(e), e->pchar->vecLocationCurrent,0, 1);

    pInsp = inspiration_Create(basePower_inspirationGetByName("Large", "Uncanny_Insight" ), e->pchar, -1);
    character_AccrueBoosts(e->pchar, pInsp, 0.0f);
    character_ApplyPower(e->pchar, pInsp, erGetRef(e), e->pchar->vecLocationCurrent,0, 1);

    pInsp = inspiration_Create(basePower_inspirationGetByName("Large", "Righteous_Rage" ), e->pchar, -1);
    character_AccrueBoosts(e->pchar, pInsp, 0.0f);
    character_ApplyPower(e->pchar, pInsp, erGetRef(e), e->pchar->vecLocationCurrent,0, 1);

    pInsp = inspiration_Create(basePower_inspirationGetByName("Large", "Escape" ), e->pchar, -1);
    character_AccrueBoosts(e->pchar, pInsp, 0.0f);
    character_ApplyPower(e->pchar, pInsp, erGetRef(e), e->pchar->vecLocationCurrent,0, 1);

    e->pchar->bNoModsLastTick = false;
    character_LevelFinalize(e->pchar, true);
    iNewLevel = character_CalcExperienceLevel(e->pchar)+1;
    if( character_IsMentor(e->pchar) )
        character_updateTeamLevel(e->pchar);
    ArenaPlayerAttributeUpdate(e);

    sendInfoBox(e, INFO_REWARD, "AbleToLevel", iNewLevel);
    serveFloater(e, e, "FloatLeveled", 0.0f, kFloaterStyle_Attention, 0);

    LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, 
            "LevelUp XPDebt: %d TeamMemberCount: %d TotalTimePlaying: %s OnSince: %s LastLogin: %s",
             character_GetExperienceDebt(e->pchar),  
             e->teamup ? e->teamup->members.count : 0,
             timerMakeOffsetStringFromSeconds(time_buffer, e->total_time), 
             timerMakeISO8601TimeStampFromSecondsSince2000(date_buffer_1, e->on_since),
             timerMakeISO8601TimeStampFromSecondsSince2000(date_buffer_2, e->last_login));

    if(iNewLevel<10)
        character_PlayFX(e->pchar, e->pchar, "generic/levelup1-10.fx", colorPairNone, 0, 0, PLAYFX_NO_TINT);
    else if(iNewLevel<20)
        character_PlayFX(e->pchar, e->pchar, "generic/levelup11-20.fx", colorPairNone, 0, 0, PLAYFX_NO_TINT);
    else if(iNewLevel<30)
        character_PlayFX(e->pchar, e->pchar, "generic/levelup21-30.fx", colorPairNone, 0, 0, PLAYFX_NO_TINT);
    else if(iNewLevel<40)
        character_PlayFX(e->pchar, e->pchar, "generic/levelup31-40.fx", colorPairNone, 0, 0, PLAYFX_NO_TINT);
    else if(iNewLevel<50)
        character_PlayFX(e->pchar, e->pchar, "generic/levelup41-50.fx", colorPairNone, 0, 0, PLAYFX_NO_TINT);
    else
    {
        character_PlayFX(e->pchar, e->pchar, "generic/levelup50Plus.fx", colorPairNone, 0, 0, PLAYFX_NO_TINT);
    }

    if (e->pl)
    {
        e->pl->currentAccessibleContactIndex = AccessibleContacts_FindFirst(e);
    }
    MARTY_NormalizeExpHistory(e, oldLevel);
}

typedef enum RewardMsgType
{
    INF_RECVD,
    INF_LOST,
    DEBT_INF_RECVD,
    DEBT_INF_RECVD_LOST,
    EXP_INF_RECVD,
    EXP_INF_RECVD_LOST,
    EXP_DEBT_INF_RECVD,
    EXP_DEBT_INF_RECVD_LOST
} RewardMsgType;

static char *rewardCurrencyHeroMsgs[] =
{
    "InfluenceYouReceived",
    "InfluenceYouLost",
    "DebtInfluenceYouReceived",
    "DebtInfluenceYouReceivedLost",
    "ExperienceInfluenceYouReceived",
    "ExperienceInfluenceYouReceivedLost",
    "ExperienceDebtInfluenceYouReceived",
    "ExperienceDebtInfluenceYouReceivedLost",
};

static char *rewardCurrencyVillainMsgs[] =
{
    "InfamyYouReceived",
    "InfamyYouLost",
    "DebtInfamyYouReceived",
    "DebtInfamyYouReceivedLost",
    "ExperienceInfamyYouReceived",
    "ExperienceInfamyYouReceivedLost",
    "ExperienceDebtInfamyYouReceived",
    "ExperienceDebtInfamyYouReceivedLost",
};

static char *rewardCurrencyPraetorianMsgs[] =
{
    "InformationYouReceived",
    "InformationYouLost",
    "DebtInformationYouReceived",
    "DebtInformationYouReceivedLost",
    "ExperienceInformationYouReceived",
    "ExperienceInformationYouReceivedLost",
    "ExperienceDebtInformationYouReceived",
    "ExperienceDebtInformationYouReceivedLost",
};

static unsigned int CurrencyEntIsUsing(Entity *e)
{
    unsigned int retval = 0;
    unsigned int base_map_id;
    const MapSpec *spec;

    // If they're on a mission map we need to know where they came from
    // to figure out what part of the world the mission is set in.
    if (OnMissionMap())
    {
        base_map_id = e->static_map_id;
    }
    else
    {
        base_map_id = db_state.base_map_id;
    }

    spec = MapSpecFromMapId(base_map_id);

    if (spec)
    {
        switch (spec->teamArea)
        {
            case MAP_TEAM_HEROES_ONLY:
            default:
                retval = 0;
                break;

            case MAP_TEAM_VILLAINS_ONLY:
                retval = 1;
                break;

            case MAP_TEAM_PRAETORIANS:
                retval = 2;
                break;

            // Praetorians use their own currency in shared areas because
            // they are not exactly heroes or villains. Primals use the
            // currency that matches their alignment. If a Rogue/Villain is
            // acting as a Hero, they'll get Influence.
            case MAP_TEAM_PRIMAL_COMMON:
            case MAP_TEAM_EVERYBODY:
                if (ENT_IS_IN_PRAETORIA(e))
                {
                    retval = 2;
                }
                else
                {
                    if (ENT_IS_ON_RED_SIDE(e))
                    {
                        retval = 1;
                    }
                    else
                    {
                        retval = 0;
                    }
                }

                break;
        }
    }

    return retval;
}

bool rewardSourceAllowedArchitect(RewardSource source)
{
    // These reward sources should always fully apply even on architect maps
    switch(source)    
    {
        case REWARDSOURCE_RECIPE:
        case REWARDSOURCE_INVENTION_RECIPE:
        case REWARDSOURCE_BADGE:
        case REWARDSOURCE_BADGE_BUTTON:
        case REWARDSOURCE_ATTRIBMOD:
        case REWARDSOURCE_CSR:
        case REWARDSOURCE_DEVCHOICE:
        case REWARDSOURCE_TFWEEKLY:
        case REWARDSOURCE_SALVAGE_OPEN:
        case REWARDSOURCE_CHOICE:        // intentional fallthrough
            return true;
        default:
            return false;
    }
}

// This should eventually replace prepareAtomicTransactionFromRewardString()...
static void prepareAtomicTransaction(Entity *e, const char *salvageName, RewardAccumulatedAccountProduct **products)
{
    int transactionCount, transactionIndex;
    Packet *pak_out;

    transactionCount = eaSize(&products);
    if (transactionCount)
    {
        pak_out = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_MULTI_GAME_TRANSACTION);
        pktSendBitsAuto(pak_out, e->auth_id);
        pktSendBitsAuto(pak_out, e->db_id);
        pktSendBitsAuto(pak_out, transactionCount);
        if (salvageName)
        {
            pktSendBits(pak_out, 1, 1);
            pktSendString(pak_out, salvageName);
        }
        else
        {
            pktSendBits(pak_out, 1, 0);
        }

        for (transactionIndex = 0; transactionIndex < transactionCount; transactionIndex++)
        {
            pktSendBitsAuto2(pak_out, skuIdFromString(products[transactionIndex]->productCode).u64);
            pktSendBitsAuto(pak_out, products[transactionIndex]->count);
            pktSendBitsAuto(pak_out, 0); // claimed
            pktSendBitsAuto(pak_out, products[transactionIndex]->rarity);
        }

        pktSend(&pak_out,&db_comm_link);
    }
}

// I don't like adding a global, but I don't want to plumb this through the whole reward system either...
static const char *s_salvageNameGlobalHack = NULL;

bool rewardApply(RewardAccumulator* reward, Entity* e, bool bGivePowers, bool bHandicapped, RewardSource source, char **lootList)
{
    int i;
    int iXPDebt = 0;
    int iXPReceived = 0;
    int iInfluence = 0;
    int iPrestige = 0;
    bool bRewardHadLoot = false;

    char *pchReport = NULL;
    char *pchPrestigeReport = NULL;

    int iPreviousExpLevel;
    bool bExemplar, bLevelCapped;
    float fPrestigeMod;
    float fInfluenceMod;
    bool bArchitect = 0;
    bool bArchitectScale = 0;
    bool bArchitectTest = 0;
    bool bArchitectAllRewards = e && e->pl && AccountCanGainAllMissionArchitectRewards(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags);
    TeamRewardDebug * pDebug=0;

    char **infMsgs = NULL;

    assert(reward && e && e->pchar);
    assert(source < REWARDSOURCE_COUNT);
    if(reward && !e && e->pchar)
        return false;

    if(g_ArchitectTaskForce)
    {
        if (!rewardSourceAllowedArchitect(source))
        {
            if ((g_ArchitectTaskForce->architect_flags & ARCHITECT_TEST))
                bArchitectTest = 1;
            // dev choice is treated like a normal mission
            if (!(g_ArchitectTaskForce->architect_flags & ARCHITECT_DEVCHOICE))
                bArchitect = 1;
        }

        // featured and dev choice missions scale experience, influence, and prestige normally
        if (!((g_ArchitectTaskForce->architect_flags & ARCHITECT_FEATURED) || (g_ArchitectTaskForce->architect_flags & ARCHITECT_DEVCHOICE) ||
              (g_ArchitectTaskForce->architect_flags & ARCHITECT_DEVCHOICE_NORMALREWARD)))
            bArchitectScale = 1;
    }

    if (source == REWARDSOURCE_DROP)
    {
        if (server_state.MARTY_enabled)
        {
            //    only drops can get throttled
            if (bArchitect && e->pl->MARTY_rewardData[MARTY_ExperienceArchitect].expThrottled)
                return false;
            else if (e->pl->MARTY_rewardData[MARTY_ExperienceCombined].expThrottled)
                return false;
        }
    }

    if( source == REWARDSOURCE_DEBUG )
        pDebug = getRewardDebug(e->db_id);


    bExemplar = character_IsRewardedAsExemplar(e->pchar);
    iPreviousExpLevel = character_CalcExperienceLevel(e->pchar);
    bLevelCapped = iPreviousExpLevel >= MAX_PLAYER_SECURITY_LEVEL-1;

    // Returns 1.0s when not in SG mode.
    rewardGetSupergroupMods(e, &fPrestigeMod, &fInfluenceMod);
    fInfluenceMod += e->pchar->attrCur.fInfluenceGain;
    fPrestigeMod += e->pchar->attrCur.fPrestigeGain;

    if( reward->bonus_experience )
    {
        awardRestExperience( e, reward->bonus_experience );
    }

    // Award the entity with experience.
    if(reward->experience && !bArchitectTest && (!bArchitect || bArchitectAllRewards))
    {
        e->general_update = 1;

        if (getPopHelpStateByTag(e, "CODEPH_Got_XP") == PopHelpState_Untriggered)
        {
            triggerPopHelpEventHappenedByTag(e, "CODEPH_Got_XP");
        }

        //    if you are in the architect
        if (bArchitect && (source == REWARDSOURCE_DROP))
        {
            //    accumuldate xp
            e->pl->MARTY_rewardData[MARTY_ExperienceArchitect].expMinuteAccum += reward->experience;
        }

        //    if we ignore caps, in combined, add it to the pool (even if the source isn't a drop)
        if (!reward->bIgnoreRewardCaps)
        {
            e->pl->MARTY_rewardData[MARTY_ExperienceCombined].expMinuteAccum += reward->experience;
        }


        if(bLevelCapped)
        {
            int iLeftOver = 0;
            character_GiveCappedExperience(e->pchar, reward->experience*(bArchitectScale?server_state.aescale:server_state.xpscale), &iXPReceived, &iXPDebt, &iLeftOver); // Added AE Exp Scaling

            if (source == REWARDSOURCE_DEBUG)
                pDebug->debt += iXPDebt;

            if (iLeftOver > 0)
            {
                reward->influence += iLeftOver;
            }

            if (iXPDebt > 0)
            {
                pchReport = "DebtYouReceived";
            }
        }
        else
        {

            float fXPScale = bArchitectScale?server_state.aescale:server_state.xpscale; // Added AE Exp Scaling
            int bPatrolXP = false;

            if( bExemplar )
                fXPScale *=  exemplarXPMod();

            // check if we want to use rest xp
            if (source == REWARDSOURCE_DROP && !bArchitect && e->pchar->iExperienceRest > 0 && 
                server_state.xpscale < dayjob_GetPatrolXPMultiplier())
            {
                fXPScale = dayjob_GetPatrolXPMultiplier();
                bPatrolXP = true;
            }

            if(e->pl->supergroup_mode)
            {
                fXPScale += sgroup_XPBonus(e);
            }
            fXPScale += e->pchar->attrCur.fExperienceGain;
            iXPReceived = reward->experience*fXPScale;

            if (iXPReceived < 0)
            {
                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]::Fail:XP %d XP is negative at first test! Aborting reward! %s trial.", iXPReceived, entIsTrial(e) ? "IS" : "NOT");
                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]::Fail:XP Authbits raw 0x%08x 0x%08x 0x%08x 0x%08x", e->pl->auth_user_data[0], e->pl->auth_user_data[1], e->pl->auth_user_data[2], e->pl->auth_user_data[3]);
                return false;
            }

            character_ApplyAndReduceDebt(e->pchar, &iXPReceived, &iXPDebt);
            if( source == REWARDSOURCE_DEBUG )
                pDebug->debt += iXPDebt;

            if (iXPReceived < 0)
            {
                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]::Fail:XP %d XP is negative at second test! Aborting reward! %s trial.", iXPReceived, entIsTrial(e) ? "IS" : "NOT");
                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]::Fail:XP Authbits raw 0x%08x 0x%08x 0x%08x 0x%08x", e->pl->auth_user_data[0], e->pl->auth_user_data[1], e->pl->auth_user_data[2], e->pl->auth_user_data[3]);
                return false;
            }

            if( bExemplar && e && e->pl && e->pl->noXPExemplar ) // exemplar that doesn't want XP gets it converted to influence
            {
                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Fail:XP %d (exemplar)", iXPReceived);
                reward->influence += iXPReceived;
                iXPReceived = 0;
            }

            if (e && e->pl && e->pl->noXP && db_state.base_map_id != 41) // blatant hack to ensure XP is always on in Destroyed Galaxy City
            {
                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Fail:XP %d (XP off)", iXPReceived);
                iXPReceived = 0;
            }


            if(bPatrolXP)
            {
                e->pchar->iExperienceRest -= iXPReceived;
                if (e->pchar->iExperienceRest < 0)
                    e->pchar->iExperienceRest = 0;
            }

            //if we're in a leveling pact and still eligible to receive xp
            if(e->levelingpact_id && iXPReceived > 0 && iPreviousExpLevel < MAX_PLAYER_SECURITY_LEVEL-1)//minue one because iPreviousExpLevel appears to be 0 based
            {
                // forward the xp to the statserver
                SgrpStat_SendPassthru(e->db_id,0, "statserver_levelingpact_addxp %d %d %d", e->levelingpact_id, iXPReceived, stat_TimeSinceXpReceived(e));
                if(e->levelingpact && e->levelingpact->count)
                {
                    if(e->levelingpact->experience/e->levelingpact->count == e->pchar->iExperiencePoints)
                        sendInfoBox(e, INFO_REWARD, "ExperienceYouReceivedLevelingPact", iXPReceived);
                    else
                        sendInfoBox(e, INFO_REWARD, "ExperienceNotReceivedLevelingPact", iXPReceived);
                }
                iXPReceived = 0;            
            }

            if( source == REWARDSOURCE_DEBUG )
                pDebug->experience += iXPReceived;
            else
                iXPReceived = character_GiveExperienceNoDebt(e->pchar, iXPReceived);

            if (iXPReceived < 0)
            {
                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]::Fail:XP %d XP is negative at third test! Aborting reward! %s trial.", iXPReceived, entIsTrial(e) ? "IS" : "NOT");
                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]::Fail:XP Authbits raw 0x%08x 0x%08x 0x%08x 0x%08x", e->pl->auth_user_data[0], e->pl->auth_user_data[1], e->pl->auth_user_data[2], e->pl->auth_user_data[3]);
                return false;
            }
            else if(iXPReceived)
            {
                stat_AddXPReceived(e, iXPReceived);
                rewardAppendToLootList(lootList, "XP %d", iXPReceived);
            }
            pchReport = iXPDebt>0 ? "ExperienceDebtYouReceived" : "ExperienceYouReceived";

        }
        bRewardHadLoot = true;
    }

    // Award the entity with influence points.
    if(reward->influence && !bArchitectTest && (!bArchitect || bArchitectAllRewards))
    {
        e->general_update = 1;
        iInfluence = ceil(reward->influence*fInfluenceMod*(bArchitectScale?server_state.aeinfluencescale:server_state.influencescale));

        if(iInfluence)
        {
            if(e->levelingpact_id && e->levelingpact)
            {
                int myInfluence = ceil(iInfluence/(F32)(e->levelingpact->members.count));
                int pactInfluence = iInfluence - myInfluence;
                U32 isVillain = SAFE_MEMBER2(e,pchar,playerTypeByLocation) == kPlayerType_Villain;
                // forward the influence to the statserver
                if(pactInfluence > 0)
                    SgrpStat_SendPassthru(e->db_id,0, "statserver_levelingpact_addinf %d %d %d", e->db_id, pactInfluence, isVillain);

                // give the rest to the player
                iInfluence = myInfluence;
            }

            if( source == REWARDSOURCE_DEBUG )
                pDebug->influence += iInfluence;
            else
            {
                stat_AddInfluenceReceived(e->pl, iInfluence);
                badge_RecordInfluence(e, iInfluence);
                rewardAppendToLootList(lootList, "influence %d", iInfluence);
                iInfluence = ent_AdjInfluence(e, iInfluence, "drop");
            }

            bRewardHadLoot = true;
        }
    }

    // Award the entity with prestige points.
    if(reward->prestige && e->pl->supergroup_mode && !bArchitectTest && (!bArchitect || bArchitectAllRewards))
    {
        iPrestige = reward->prestige*fPrestigeMod*(bArchitectScale?server_state.aeprestigescale:server_state.prestigescale);

        if( source == REWARDSOURCE_DEBUG )
            pDebug->prestige += iPrestige;
        else if (iPrestige != 0)
        {
            sgrprewards_PrestigeAdj(e, iPrestige);
            badge_RecordPrestige(e, iPrestige);
            rewardAppendToLootList(lootList, "prestige %d", iPrestige);
        }

        if( iPrestige < 0 )
        {
            pchPrestigeReport = "PrestigeYouLost";
        }
        else if (iPrestige > 0)
        {
            pchPrestigeReport = "PrestigeYouReceived";
        }
        bRewardHadLoot = true;
    }
    
    // XXX: Figure out the correct currency for the map here. This will
    // deal with tourists on static maps. Mission maps need more work.
    switch (CurrencyEntIsUsing(e))
    {
        case 0:
        default:
            infMsgs = rewardCurrencyHeroMsgs;
            break;
        case 1:
            infMsgs = rewardCurrencyVillainMsgs;
            break;
        case 2:
            infMsgs = rewardCurrencyPraetorianMsgs;
            break;
    }

    if (iInfluence > 0)
    {
        if (iXPReceived)
        {
            if (iXPDebt)
                pchReport = infMsgs[EXP_DEBT_INF_RECVD];
            else
                pchReport = infMsgs[EXP_INF_RECVD];
        }
        else
        {
            if (iXPDebt)
                pchReport = infMsgs[DEBT_INF_RECVD];
            else
                pchReport = infMsgs[INF_RECVD];
        }
    }
    else if (iInfluence < 0)
    {
        if (iXPReceived)
        {
            if (iXPDebt)
                pchReport = infMsgs[EXP_DEBT_INF_RECVD_LOST];
            else
                pchReport = infMsgs[EXP_INF_RECVD_LOST];
        }
        else
        {
            if (iXPDebt)
                pchReport = infMsgs[DEBT_INF_RECVD_LOST];
            else
                pchReport = infMsgs[INF_LOST];
        }
    }

    if(pchReport)
    {
        sendInfoBox(e, INFO_REWARD, pchReport, iXPReceived, abs(iInfluence), iXPDebt, 0, 0);
    }
    if(pchPrestigeReport)
    {
        sendInfoBox(e, INFO_REWARD, pchPrestigeReport, (e && e->supergroup) ? e->supergroup->name : "", iPrestige);
    }

    if(e->pl->supergroup_mode)
    {
        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Points XP: %d, Debt: %d, Inf: %d, Pres: %d (SGMode on (%d): Inf: %3.4f Pres: %3.4f)", iXPReceived, iXPDebt, iInfluence,  iPrestige, e->supergroup_id, fInfluenceMod, fPrestigeMod);
    }
    else
    {
        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Points XP: %d, Debt: %d, Inf: %d, Pres: %d (SGMode off (%d))", iXPReceived, iXPDebt, iInfluence, iPrestige, e->supergroup_id);
    }
    if(iPreviousExpLevel != character_CalcExperienceLevel(e->pchar))
    {
        levelupApply(e, iPreviousExpLevel);
    }

    // reward token counters
    // Do this before awarding powers so we can test against token state in an awarded auto power.
    for(i = eaSize(&reward->rewardTokenCount)-1; i >= 0; i--)
    {
        RewardAccumulatedRewardTokenCount *accd_token = reward->rewardTokenCount[i];

        if (accd_token)
        {
            addToRewardToken(e, accd_token->tokenName, accd_token->count);
            rewardAppendToLootList(lootList, "token.%s %d", accd_token->tokenName, accd_token->count);
        }
    }

    // Award the entity with inspirations and boosts.
    for(i = eaSize(&reward->powers)-1; i >= 0; i--)
    {
        RewardAccumulatedPower *accd_power = reward->powers[i];
        const PowerNameRef* power = accd_power->powerNameRef;
        int level = accd_power->level;
        bool bEveryone = accd_power->set_flags & RSF_EVERYONE;
        bool bForced = accd_power->set_flags & RSF_FORCED;

        // Even if this player isn't chosen to get stuff, they still get the
        //   item if it is marked for everyone.
        if (bEveryone)
        {
            if (rewardAddPower(e,power,level,bEveryone,source) == RAP_OK)
            {
                bRewardHadLoot = true;
                rewardAppendToLootList(lootList, "%s.%s.%s %d", power->powerCategory, power->powerSet, power->power, level);
            }
            continue;
        }

        // Continue if this character wasn't chosen to receive power rewards
        if (!bGivePowers)
            continue;

        // If the character is handicapped, they get nothing!
        if (bHandicapped)
        {
            LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Fail:Tpow %s.%s.%s (handicapped)",power->powerCategory,power->powerSet,power->power);
            continue;
        }

        // Ok, they were chosen to get a reward, and they're not handicapped
        if (rewardAddPower(e, power, level, !bExemplar || bForced, source) == RAP_OK)
        {
            bRewardHadLoot = true;
            rewardAppendToLootList(lootList, "%s.%s.%s %d", power->powerCategory, power->powerSet, power->power, level);
        }
    }

    // Award the entity with tokens.
    for(i = eaSize(&reward->tokens)-1; i >= 0; i--)
    {
        RewardAccumulatedToken *accd_token = reward->tokens[i];

        if (accd_token->remove)
        {
            removeRewardToken(e, accd_token->tokenName);
        }
        else
        {
            giveRewardToken(e, accd_token->tokenName);
        }
    }

    if( !bArchitectTest && !bArchitect)
    {
        // Award the entity with salvage. this is slightly different than the loop for powers
        // note: same loop as for proficiencies and concepts
        for(i = eaSize(&reward->salvages)-1; i >= 0; i--)
        {
            RewardAccumulatedSalvage *accd_salvage = reward->salvages[i];
            bool bEveryone = accd_salvage->set_flags & RSF_EVERYONE;
            bool bNoLimit = accd_salvage->set_flags & RSF_NOLIMIT;
            bool bForced = accd_salvage->set_flags & RSF_FORCED;
            const SalvageItem *salvage = accd_salvage->item;
            int count = accd_salvage->count;
            int color = 0;

            // Even if this player isn't chosen to get stuff, they still get the
            //   item if it is marked for everyone.
            if(bEveryone || bGivePowers)
            {
                if( pDebug )
                    pDebug->salvage_cnt++;
                else
                {
                    DATA_MINE_SALVAGE(salvage,"in",gRewardSourceLabels[source],count);
                    if(!bEveryone && bHandicapped && !bForced)
                    {
                        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Fail:Sal %s (handicapped)", salvage->pchName);
                        DATA_MINE_SALVAGE(salvage,"out","handicapped",count);
                    }
                    else if(!bNoLimit && salvageitem_IsInvention(salvage) && character_InventoryFull( e->pchar, kInventoryType_Salvage ))
                    {
                        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Fail:Sal %s (inventory full)", salvage->pchName);
                        DATA_MINE_SALVAGE(salvage,"out","salvage_full",count);
                    }
                    else if(!bNoLimit && salvageitem_IsInvention(salvage) && !AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("coliinvn")))
                    {
                        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Fail:Sal %s (no license)", salvage->pchName);
                        DATA_MINE_SALVAGE(salvage,"out","no_license",count);
                    }
                    else if(!bNoLimit && !character_CanSalvageBeChanged( e->pchar, salvage, count ))
                    {
                        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Fail:Sal %s (inventory full)", salvage->pchName);
                        DATA_MINE_SALVAGE(salvage,"out","item_full",count);
                    }
                    else // give it to them
                    {
                        character_AdjustSalvage( e->pchar, salvage, count, NULL, false );
                        rewardAppendToLootList(lootList, "salvage.%s %d", salvage->pchName, count);
                        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Sal %d x %s", count, salvage->pchName);
                        if (count > 1)
                            sendInfoBox(e, INFO_REWARD, "SalvageYouReceivedNum", count, salvage->ui.pchDisplayName);
                        else
                            sendInfoBox(e, INFO_REWARD, "SalvageYouReceived", salvage->ui.pchDisplayName);

                        switch (salvage->rarity) 
                        { 
                            case kSalvageRarity_Ubiquitous:
                            case kSalvageRarity_Common:
                                color = 0xffffffff;
                                break;
                            case kSalvageRarity_Uncommon:
                                color = 0xffff00ff;
                                break;
                            case kSalvageRarity_Rare:
                                color = 0xff5500ff;
                                break;
                            case kSalvageRarity_VeryRare:
                                color = 0xb020ffff;
                                break;
                            case kSalvageRarity_Unique:
                                color = 0x800080ff;
                                break;
                        }
                        if (salvage->pchDisplayDropMsg)
                        {
                            serveFloater(e, e, salvage->pchDisplayDropMsg, 0.0f, kFloaterStyle_Attention, color);
                        }
                        else if (salvageitem_IsInvention(salvage))
                        {
                            serveFloater(e, e, "FloatFoundInventSalvage", 0.0f, kFloaterStyle_Attention, color);
                        }
                        else
                        {
                            serveFloater(e, e, "FloatFoundSalvage", 0.0f, kFloaterStyle_Attention, color);
                        }
                        if(salvageitem_IsInvention(salvage))
                            badge_StatAdd(e,"drops.invention_salvage",count);
                        bRewardHadLoot = true;
                    }
                }
            }
        }

        for(i = eaSize(&reward->detailRecipes)-1; i >= 0; i--)
        {
            RewardAccumulatedDetailRecipe *accd_recipe = reward->detailRecipes[i];
            bool bEveryone = accd_recipe->set_flags & RSF_EVERYONE;
            bool bNoLimit = accd_recipe->set_flags & RSF_NOLIMIT;
            bool bForced = accd_recipe->set_flags & RSF_FORCED;
            DetailRecipe const *item = accd_recipe->item;

            if (item && item->Type == kRecipeType_Drop && // can only give drops as rewards
                (bEveryone || bGivePowers) ) // if the player is marked, or the reward is for everyone
            {
                if( pDebug )
                    pDebug->recipe_cnt++;
                else
                {
                    DATA_MINE_RECIPE(item,"in",gRewardSourceLabels[source],1);
                    if(!bEveryone && bHandicapped && !bForced)
                    {
                        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Fail:Rec  %s (handicapped)", item->pchName);
                        DATA_MINE_RECIPE(item,"out","handicapped",1);
                    }
                    else if(!bNoLimit && character_InventoryFull( e->pchar, kInventoryType_Recipe ))
                    {
                        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Fail:Rec %s (inventory full)", item->pchName);
                        DATA_MINE_RECIPE(item,"out","recipes_full",1);
                    }
                    else if(!bNoLimit && !character_CanRecipeBeChanged( e->pchar, item, 1 ))
                    {
                        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Fail:Sal %s (inventory full)", item->pchName);
                        DATA_MINE_RECIPE(item,"out","item_full",1);
                    }
                    else if(!bNoLimit && item->flags & RECIPE_INVENTION_LIC_REQUIRED && !AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("coliinvn")))
                    {
                        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Fail:Sal %s (requires inv license)", item->pchName);
                        DATA_MINE_RECIPE(item,"out","no_inv_license",1);
                    }
                    else // give it to them!
                    {
                        unsigned int color;
                        switch (item->Rarity)
                        {
                        case kRecipeRarity_Ubiquitous:
                        case kRecipeRarity_Common:
                            color = 0xffffffff;
                            break;
                        case kRecipeRarity_Uncommon:
                            color = 0xffff00ff;
                            break;
                        case kRecipeRarity_Rare:
                            color = 0xff5500ff;
                            break;
                        case kRecipeRarity_VeryRare:
                            color = 0x800080ff;
                            break;
                        }
                        character_AdjustRecipe( e->pchar, item, 1, NULL );
                        rewardAppendToLootList(lootList, "recipe.%s %d", item->pchName, 1);
                        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Rec %s", item->pchName);
                        sendInfoBox(e, INFO_REWARD, "DetailRecipeYouReceived", item->ui.pchDisplayName);
                        serveFloater(e, e, "FloatFoundRecipe", 0.0f, kFloaterStyle_Attention, color);
                        bRewardHadLoot = true;
                    }
                }
            }
        }

        for(i = eaSize(&reward->details)-1; i >= 0; i--)
        {
            RewardAccumulatedDetail *accd_recipe = reward->details[i];
            bool bEveryone = accd_recipe->set_flags & RSF_EVERYONE;
            Detail const *item = accd_recipe->item;

            // Even if this player isn't chosen to get stuff, they still get the
            //   item if it is marked for everyone.
            if (bEveryone || bGivePowers)
            {
                if( pDebug )
                    pDebug->base_item++;
                else
                {
                    character_AddBaseDetail( e->pchar, item->pchName, gRewardSourceLabels[source] );
                    rewardAppendToLootList(lootList, "detail.%s %d", item->pchName, 1);
                    LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:Detail %s", item->pchName);
                    sendInfoBox(e, INFO_REWARD, "DetailYouReceived",item->pchDisplayName);
                    if(source != REWARDSOURCE_BADGE_BUTTON)
                        serveFloater(e, e, "FloatFoundBaseDetail", 0.0f, kFloaterStyle_Attention, 0);
                    bRewardHadLoot = true;
                }
            }
        }

        for( i = eaSize( &reward->rewardTables ) - 1; i >= 0; --i)
        {
            RewardAccumulatedRewardTable *accd_reward = reward->rewardTables[i];
            bool bEveryone = accd_reward->set_flags & RSF_EVERYONE;
            const char *tableName  = accd_reward->tableName;
            int level = accd_reward->level;

            if (bEveryone || bGivePowers)
            {
                RewardLevel rl = {0};
                LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Apply %s %s",
                    tableName, accd_reward->rewardTableFlags & kRewardTableFlags_AtCritterLevel ? "(Forced to critter level)":"");

                if(accd_reward->rewardTableFlags & kRewardTableFlags_AtCritterLevel)
                {
                    rl.iExpLevel = rl.iEffLevel = MINMAX(level-1,0,MAX_PLAYER_SECURITY_LEVEL-1);
                    rl.type = kPowerSystem_Powers;
                }
                else
                {
                    rl = character_CalcRewardLevel(e->pchar);
                }

                bRewardHadLoot |= rewardFindDefAndApplyToEntShortLog( e, (const char**)eaFromPointerUnsafe(tableName), reward->villainGrp,
                                                                        level, false, rl, !bHandicapped, source, lootList);
            }
        }

        // eventually this is where this should go, after I've refactored the code to
        //   do the recursive accrual before applying the entire recursion stack at once.
        // prepareAtomicTransaction(e, s_salvageNameGlobalHack, reward->accountProducts);
        for (i = eaSize(&reward->accountProducts) - 1; i >= 0; i--)
        {
            rewardAppendToLootList(lootList, "accountproduct %s %d %d", reward->accountProducts[i]->productCode, reward->accountProducts[i]->count, reward->accountProducts[i]->rarity);
        }

        for (i = eaSize(&reward->incarnatePoints) - 1; i >= 0; i--)
        {
            RewardAccumulatedIncarnatePoints *accd_incarnate = reward->incarnatePoints[i];
            int pointCount = accd_incarnate->count;
            int subtype = accd_incarnate->subtype;

            LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Rcv:IXP:%s %d",
                g_IncarnateMods.incarnateExpList[subtype]->incarnateXPType, pointCount);

            Incarnate_RewardPoints(e, subtype, pointCount);
            rewardAppendToLootList(lootList, "incarnatepoints.%d %d", subtype, pointCount);
        }
    }

    return bRewardHadLoot;
}

// Helper function that returns the correct reward name based on a var lookup
// or just the name if no match or no vars
static const char* rewardVarsLookup(ScriptVarsTable* vars, const char* rewardName)
{
    return vars ? ScriptVarsTableLookup(vars, rewardName) : rewardName;
}

// Design decision to cap all story rewards at the players level to prevent power leveling exploits
#define    STORYREWARD_GRACELEVELS    3
int rewardStoryAdjustLevel(Entity* player, int iLevel, int iLevelAdjust)
{
    RewardLevel rl = character_CalcRewardLevel(player->pchar);
    
    if (rl.iEffLevel == rl.iExpLevel)
    {
        // Take the minimum of the reward level and the exp level(1 based and 3 level grace)
        int expLevelCap = (rl.iExpLevel + 1) + STORYREWARD_GRACELEVELS;
        iLevel = min(expLevelCap, iLevel);
    }
    return iLevel + iLevelAdjust;
}

//range is a number, or two numbers seperated by a dash.
// this currently doesn't check all the error conditions,
// just that the string only contains digits and '-'.
int is_a_range(const char *str)
{
    while(*str)
    {
        if (*str != '-' && !isdigit((unsigned char)*str))
        {
            return 0;
        }
        str++;
    }
    return 1;
}

static int is_in_range(int level, const char *range)
{
    int low;
    int high;
    char *p;
    
    low = atoi(range);
    p = strchr(range, '-');
    if (p)
        high = atoi(p+1);
    else
        high = low;
    return (level >= low && level <= high);
}

// Relays a reward to a player but caps the reward level at the player level plus level boost
void rewardStoryApplyToDbID(int db_id, const char **ppchName, VillainGroupEnum vgroup, int iLevel, int iLevelAdjust, ScriptVarsTable* vars, RewardSource source)
{
    Entity* player;
    int i, n = eaSize(&ppchName);

    assert(ppchName && db_id!=0);

    if ((player = entFromDbId(db_id)))
    {
        int rewardLevel = rewardStoryAdjustLevel(player, iLevel, iLevelAdjust);
        char** rewardList = NULL;

        // Do a var lookup on all the rewards and store to a temporary array
        for (i = 0; i < n; i++)
        {
            //check if it's a range filter
            if (is_a_range(ppchName[i]))
            {
                if (is_in_range(iLevel, ppchName[i]) && (i+1 < n))
                {
                    eaPush(&rewardList, strdup(rewardVarsLookup(vars, ppchName[i+1])));
                }
                i++;
            } 
            else
            {
                eaPush(&rewardList, strdup(rewardVarsLookup(vars, ppchName[i])));
            }
        }

        // Now apply the reward normally
        rewardFindDefAndApplyToEnt(player, rewardList, vgroup, rewardLevel, false, source, NULL);
        eaDestroyEx(&rewardList, NULL);
    }
    else
    {
        // relay the award
        StuffBuff rewardCommand;
        char *pStoryArc = rewardVarGet("StoryArcName");

        if (!pStoryArc)
            pStoryArc = "None";

        initStuffBuff(&rewardCommand, 32);

        // First send all the information about the reward, other than the names
        addStringToStuffBuff(&rewardCommand, "rewardstory %i %i %i %i %i %s ", db_id, vgroup, iLevel, iLevelAdjust, source, pStoryArc);

        // Now send the names of all the rewards using the semicolon as the delimiter
        for (i = 0; i < n; i++)
            addStringToStuffBuff(&rewardCommand, "%s:", rewardVarsLookup(vars, ppchName[i]));

        // Relay the reward command to the player
        serverParseClient(rewardCommand.buff, NULL);
        freeStuffBuff(&rewardCommand);
    }
}

void rewardFindDefAndApplyToDbID(int db_id, const char **ppchName, VillainGroupEnum vgroup, int iLevel, int bIgnoreHandicap, RewardSource source)
{
    Entity *e;

    assert(ppchName && db_id!=0);

    if((e=entFromDbId(db_id))!=NULL)
    {
        rewardFindDefAndApplyToEnt(e, ppchName, vgroup, iLevel, bIgnoreHandicap, source, NULL);
    }
    else
    {
        // send reward indirectly
        char givecmd[1024];
        StuffBuff rewardTables;
        int i;

        initStuffBuff(&rewardTables, 32);

        for( i = eaSize(&ppchName)-1; i >= 0; i-- )
            addStringToStuffBuff( &rewardTables, "%s:", ppchName[i] );

        sprintf(givecmd, "rewarddefapply %i %s %d %d %i", db_id, rewardTables.buff, vgroup, iLevel, source);
        serverParseClient(givecmd, NULL);

        freeStuffBuff(&rewardTables);
    }
}

// This is now a wrapper for a single reward def application
// You should call this unless you're going to be clearing and dumping the reward log yourself
bool rewardFindDefAndApplyToEnt(Entity* e, const char **ppchName, VillainGroupEnum vgroup, int iLevel, bool bIgnoreHandicap, RewardSource source, char **lootList)
{
    int i, count = eaSize(&ppchName);
    bool bRewardHadLoot = FALSE;

    assert(source < REWARDSOURCE_COUNT);

    if(!s_sbRewardLog.buff)
        initStuffBuff(&s_sbRewardLog, 1024);

    for(i=0; i<count; i++)
    {
        // Actually do application of a single item on the input list
        bRewardHadLoot |= rewardFindDefAndApplyToEntShortLog(e,(const char**)eaFromPointerUnsafe(ppchName[i]),vgroup,iLevel, true, character_CalcRewardLevel(e->pchar),
                                                                bIgnoreHandicap, source, lootList);

        // Dump the log
        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "%s", s_sbRewardLog.buff);

        // clear out reward log.
        clearStuffBuff(&s_sbRewardLog);
    }
    return bRewardHadLoot;
}

void clearRewardChoice(Entity *e, int clearsMoralChoice)
{
    int i;
    const RewardChoiceSet *rewardChoice;
    stashFindPointerConst( g_RewardDefinition->rewardChoiceDictionary.hashRewardChoice, e->pl->pendingReward.pendingRewardName, &rewardChoice );

    if (rewardChoice && (rewardChoice->isMoralChoice || rewardChoice->disableChooseNothing) && !clearsMoralChoice)
    {
        return;
    }
    else
    {
        strcpy(e->pl->pendingReward.pendingRewardName, e->pl->queuedRewards[0].pendingRewardName);
        e->pl->pendingReward.pendingRewardLevel = e->pl->queuedRewards[0].pendingRewardLevel;
        e->pl->pendingReward.pendingRewardVgroup = e->pl->queuedRewards[0].pendingRewardVgroup;
        e->pl->pendingRewardSent[0] = '\0';

        //    shift everything down
        for (i = 0; i < MAX_REWARD_TABLES; ++i)
        {
            RewardDBStore *current = &e->pl->queuedRewards[i];
            RewardDBStore *next = i < (MAX_REWARD_TABLES-1) ? &e->pl->queuedRewards[i+1] : NULL;
            if (next && next->pendingRewardName[0] != 0)
            {
                strcpy(current->pendingRewardName, next->pendingRewardName);
                current->pendingRewardLevel = next->pendingRewardLevel;
                current->pendingRewardVgroup = next->pendingRewardVgroup;
            }
            else
            {
                current->pendingRewardName[0] = '\0';
                current->pendingRewardLevel = 0;
                current->pendingRewardVgroup = 0;
                break;
            }
        }

        if( e->pl->pendingReward.pendingRewardName && e->pl->pendingReward.pendingRewardName[0] )
        {
            sendPendingReward(e);
        }
        else
        {
            s_updateSendPendingReward(e, rewardChoice);
            rewardChoiceSend(e, 0);
        }
    }
}

bool rewardFindDefAndApplyToEntShortLog(Entity* e, const char **ppchName, VillainGroupEnum vgroup, int iLevel, bool isVictimLevel, RewardLevel rl, 
                                        bool bIgnoreHandicap, RewardSource source, char **lootList)
{
    int i, count = eaSize(&ppchName);
    bool bRewardHadLoot = FALSE;

    assert(source < REWARDSOURCE_COUNT);

    for(i = 0; i < count; i++ )
    {
        const char* rewardName = ppchName[i];
        // This should never be true, but we'll be on the safe side
        if(!s_sbRewardLog.buff)
            initStuffBuff(&s_sbRewardLog, 1024);

        // first check to see if this is a reward choice
        if(stashFindElementConst(g_RewardDefinition->rewardChoiceDictionary.hashRewardChoice, rewardName, NULL))
        {
            // the reward is a choice, start doing choice stuff
            const RewardChoiceSet *rewardChoice =    stashFindPointerReturnPointerConst(g_RewardDefinition->rewardChoiceDictionary.hashRewardChoice, rewardName);

            if( rewardChoice )
            {
                if( e->pl->pendingReward.pendingRewardName[0] != 0 ) // somehow we got to another choice while the first choice is pending
                {
                    int j;
                    int ableToAward = 0;
                    for (j = 0; j < MAX_REWARD_TABLES; ++j)
                    {
                        if (e->pl->queuedRewards[j].pendingRewardName[0] == 0)
                        {
                            strcpy(e->pl->queuedRewards[j].pendingRewardName, rewardChoice->pchName);
                            e->pl->queuedRewards[j].pendingRewardLevel = iLevel - 1;
                            e->pl->queuedRewards[j].pendingRewardVgroup = vgroup;
                            ableToAward = 1;
                            break;
                        }
                    }
                    if (!ableToAward)
                    {
                        const RewardChoiceSet *rewardChoiceOld =    stashFindPointerReturnPointerConst(g_RewardDefinition->rewardChoiceDictionary.hashRewardChoice,e->pl->pendingReward.pendingRewardName);
                        if (rewardChoiceOld)
                        {
                            for (j = 0; j < eaSize(&rewardChoiceOld->ppChoices); ++j)
                            {
                                int k;
                                int validReward = 1;
                                for(k = eaSize(&rewardChoice->ppChoices[j]->ppVisibleRequirements)-1; k >= 0; k--)
                                {
                                    if(!chareval_Eval(e->pchar,rewardChoice->ppChoices[j]->ppVisibleRequirements[k]->ppchRequires, rewardChoice->pchSourceFile))
                                    {
                                        validReward = 0;
                                        break;
                                    }
                                }
                                for(k = eaSize(&rewardChoice->ppChoices[j]->ppRequirements)-1; k >= 0; k--)
                                {
                                    if(!chareval_Eval(e->pchar,rewardChoice->ppChoices[j]->ppRequirements[k]->ppchRequires, rewardChoice->pchSourceFile))
                                    {
                                        validReward = 0;
                                        break;
                                    }
                                }
                                if (validReward)
                                {
                                    ableToAward = 1;
                                    rewardChoiceReceive(e, j);
                                    break;
                                }
                            }
                        }
                        if (!ableToAward)
                        {
                            //    there were no more valid choices for them.. how weird
                            //    dump the whole choice and move on
                            clearRewardChoice(e, 1);
                        }
                        strcpy(e->pl->queuedRewards[MAX_REWARD_TABLES-1].pendingRewardName, rewardChoice->pchName);
                        e->pl->queuedRewards[MAX_REWARD_TABLES-1].pendingRewardLevel = iLevel - 1;
                        e->pl->queuedRewards[MAX_REWARD_TABLES-1].pendingRewardVgroup = vgroup;
                    }

                    if (rewardChoice->ppChoices)
                    {
                        addStringToStuffBuff(&s_sbRewardLog, "[Ch]:Sent: %s, %s, level %d\n", e->name, rewardChoice->pchName, iLevel);
                        bRewardHadLoot = TRUE;
                    }
                }
                else
                {

                    // save crucial information so reward can be issued at later time
                    strcpy(e->pl->pendingReward.pendingRewardName, rewardChoice->pchName);
                    e->pl->pendingReward.pendingRewardLevel = iLevel-1;
                    e->pl->pendingReward.pendingRewardVgroup = vgroup;


                    if (!rewardChoice->ppChoices)
                    { //we have an invalid reward
                        LOG( LOG_ERROR, LOG_LEVEL_IMPORTANT, 0, "Bad RewardChoice name %s, level %d, group %d found\n",
                            e->pl->pendingReward.pendingRewardName,e->pl->pendingReward.pendingRewardLevel,e->pl->pendingReward.pendingRewardVgroup);
                        clearRewardChoice(e, 1);
                    }
                    else if( e->client ) // player is online right now, offline players will be sent window when they are loaded next
                    {
                        addStringToStuffBuff(&s_sbRewardLog, "[Ch]:Sent: %s, %s, level %d\n", e->name, rewardChoice->pchName, iLevel);
                        if (rewardChoice->isMoralChoice)
                        {
                            if (eaSize(&rewardChoice->ppChoices) >= 2
                                && rewardChoice->ppChoices[0] && rewardChoice->ppChoices[1])
                            {
                                sendMoralChoice(e, rewardChoice->ppChoices[0]->pchDescription,
                                    rewardChoice->ppChoices[1]->pchDescription,
                                    rewardChoice->ppChoices[0]->pchWatermark,
                                    rewardChoice->ppChoices[1]->pchWatermark, true);
                            }
                        }
                        else
                        {
                            s_updateSendPendingReward(e, rewardChoice);
                            rewardChoiceSend( e, rewardChoice );
                        }
                        bRewardHadLoot = TRUE;
                    }
                }
            }
        }
        else // regular reward table
        {
            addStringToStuffBuff(&s_sbRewardLog, "Reward: %s, %s, level %d\n{\n", e->name, rewardName, iLevel);
            bRewardHadLoot |= rewardTeamMember(e,  // Team member to reward
                rewardName,      // Reward table to fetch reward from
                1.0f,            // The XP and influence scaling tweak
                iLevel-1,        // Level in table
                isVictimLevel,     // Passed in from caller
                1.0f,            // The portion of the entire reward to be given
                1.0f,            // The portion of the entire reward from the group bonus
                1.0f,            // The portion of the team part of the reward
                vgroup,          // The villain def of the defeated villain
                true,            // True if powers as well as xp and inf are given.
                bIgnoreHandicap, // True to ignore level handicapping.
                rl,
                source, 
                CONTAINER_TEAMUPS,
                0,
                lootList);
            addStringToStuffBuff(&s_sbRewardLog, "}\n");
        }

        // MAK - total hack needed temporarily for task forces:
        if (!stricmp(rewardName, "TaskForceReward"))
        {
            serveFloater(e, e, "FloatTaskForceComplete", 0.0f, kFloaterStyle_Attention, 0);
        }
    }
    return bRewardHadLoot;
}

// very dumb way of finding first power listed in this reward set
const RewardItem* rewardGetFirstItem(const RewardSet** rewardsets)
{
    if (eaSize(&rewardsets) &&
        eaSize(&rewardsets[0]->dropGroups) &&
        eaSize(&rewardsets[0]->dropGroups[0]->itemSets) &&
        eaSize(&rewardsets[0]->dropGroups[0]->itemSets[0]->rewardItems))
    {
        int i;
        const RewardItem **rewardItems = rewardsets[0]->dropGroups[0]->itemSets[0]->rewardItems;
        int n = eaSize(&rewardItems);

        for( i = 0; i < n; ++i )
        {
            if( rewardItems[i]->type == kRewardItemType_Power )
            {
                return rewardItems[i];
            }
        }
    }

    return NULL;
}
static int s_updateSendPendingReward(Entity *e, const RewardChoiceSet *rewardChoice)
{
    int retVal = 0;
    int visibleField = 0;
    int *disabledField = NULL;
    if(rewardChoice && verify(rewardChoice->ppChoices))
    {
        int i, count = eaSize(&rewardChoice->ppChoices);
        for( i = 0; i < count; i++ )
        {
            const RewardChoice *choice = rewardChoice->ppChoices[i];
            int j, visiblereqs = eaSize(&choice->ppVisibleRequirements), reqs = eaSize(&choice->ppRequirements);
            int disabled = 0, visible = 1;

            for(j = 0; j < visiblereqs; j++)
            {
                const RewardChoiceRequirement *req = choice->ppVisibleRequirements[j];
                if(!chareval_Eval(e->pchar,req->ppchRequires, rewardChoice->pchSourceFile))
                    visible = 0;
            }

            if (visible)
            {
                visibleField |= 1 << i;
            }
            else
            {
                eaiPush(&disabledField, 0);
                continue;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // TOTAL HACK - Please remove! <-- LIES, Vince is too scared to remove this now so its now become permanent.
            //        Required for hotfix to explain why players were not receiving holiday candy canes
            if (e->pchar && stricmp(rewardChoice->pchName, "Holiday2006") == 0 && i == 4 && 
                e->pl->pendingReward.pendingRewardLevel < character_GetExperienceLevelExtern(e->pchar) - 3)
            {
                disabled |= 1;
            }
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            for(j = 0; j < reqs; j++)
            {
                const RewardChoiceRequirement *req = choice->ppRequirements[j];
                if(!chareval_Eval(e->pchar,req->ppchRequires, rewardChoice->pchSourceFile))
                {
                    disabled |= 1 << (j+1);
                } 
            }
            eaiPush(&disabledField, disabled);
        }
    }
    if (e->pl->pendingRewardVisibleBitField != visibleField)
    {
        e->pl->pendingRewardVisibleBitField = visibleField;
        retVal = 1;
    }
    if (eaiCompare(&e->pl->pendingRewardDisabledBitField, &disabledField))
    {
        eaiCopy(&e->pl->pendingRewardDisabledBitField, &disabledField);
        retVal = 1;
    }
    return retVal;
}
void sendPendingReward(Entity *e)
{
    if( e->pl->pendingReward.pendingRewardName && e->pl->pendingReward.pendingRewardName[0] )
    {
        const RewardChoiceSet *rewardChoice = stashFindPointerReturnPointerConst(g_RewardDefinition->rewardChoiceDictionary.hashRewardChoice,e->pl->pendingReward.pendingRewardName);

        if (!rewardChoice || !rewardChoice->ppChoices)
        { //we have an invalid reward
            LOG( LOG_ERROR, LOG_LEVEL_IMPORTANT, 0, "Bad RewardChoice name %s, level %d, group %d found\n",e->pl->pendingReward.pendingRewardName,e->pl->pendingReward.pendingRewardLevel,e->pl->pendingReward.pendingRewardVgroup);
            clearRewardChoice(e, 1);
        }
        else
        {
            if (rewardChoice->isMoralChoice)
            {
                if (stricmp(e->pl->pendingReward.pendingRewardName, e->pl->pendingRewardSent))
                {
                    strcpy(e->pl->pendingRewardSent, e->pl->pendingReward.pendingRewardName);
                    if (eaSize(&rewardChoice->ppChoices) >= 2
                        && rewardChoice->ppChoices[0] && rewardChoice->ppChoices[1])
                    {
                        sendMoralChoice(e, rewardChoice->ppChoices[0]->pchDescription,
                            rewardChoice->ppChoices[1]->pchDescription,
                            rewardChoice->ppChoices[0]->pchWatermark,
                            rewardChoice->ppChoices[1]->pchWatermark, true);
                    }
                }
            }
            else
            {
                int sendChanged = s_updateSendPendingReward(e, rewardChoice);
                if (sendChanged || stricmp(e->pl->pendingReward.pendingRewardName, e->pl->pendingRewardSent))
                {
                    strcpy(e->pl->pendingRewardSent, e->pl->pendingReward.pendingRewardName);
                    rewardChoiceSend( e, rewardChoice );
                }
            }
        }
    }
}

void rewardChoiceSend( Entity *e, const RewardChoiceSet *rewardChoice )
{
    START_PACKET( pak, e, SERVER_SEND_REWARD_CHOICE )
    if(rewardChoice && verify(rewardChoice->ppChoices))
    {
        int i, count = eaSize(&rewardChoice->ppChoices);

        // Send the description and number of choices
        // For each choice, send whether it's disabled and the text for the choice
        // Choices with no text will not be shown to the player at all
        // Received by receiveRewardChoice() in uiNet.c

        pktSendString( pak, localizedPrintf(e,rewardChoice->pchDesc));
        
        pktSendBits( pak, 1, rewardChoice->disableChooseNothing);
        
        pktSendBitsPack( pak, 3, count );
        for( i = 0; i < count; i++ )
        {
            const RewardChoice *choice = rewardChoice->ppChoices[i];
            int j, reqs = eaSize(&choice->ppRequirements);
            int disabled = 0;
            int visible = e->pl->pendingRewardVisibleBitField & (1 << i);
            static char *display;

            if(!visible)
            {
                pktSendBits(pak,1,0);
                continue;
            }

            estrPrintCharString(&display,localizedPrintf(e,choice->pchDescription));

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // TOTAL HACK - Please remove! <-- LIES, Vince is too scared to remove this now so its now become permanent.
            //        Required for hotfix to explain why players were not receiving holiday candy canes
            if (e->pl->pendingRewardDisabledBitField[i] & 1)
            {
                disabled = 1;
                estrConcatf(&display,"<br><font color=red>%s</font>",localizedPrintf(e, "FloatTargetTooHigh"));
            }
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            for(j = 0; j < reqs; j++)
            {
                if(e->pl->pendingRewardDisabledBitField[i] & (1 << (j+1)))
                {
                    const RewardChoiceRequirement *req = choice->ppRequirements[j];
                    disabled = 1;
                    if(!req->pchWarning || !req->pchWarning[0])
                        estrClear(&display);
                    else if(estrLength(&display))
                        estrConcatf(&display,"<br><font color=red>%s</font>",localizedPrintf(e,req->pchWarning));
                } 
            }

            pktSendBits(pak,1,1);
            pktSendBits(pak,1,disabled);
            pktSendString(pak,display);
        }
    }
    else
    {
        pktSendString(pak,"");
        pktSendBits(pak,1,0);
        pktSendBitsPack(pak,3,0);
    }
    END_PACKET
}

void rewardChoiceReceive(Entity *e, int choice)
{
    int pendingRewardLevel;
    int pendingRewardVgroup;
    const RewardChoiceSet *rewardChoice;
    int i;
    RewardLevel rl = {0};

    if( !e->pl->pendingReward.pendingRewardName[0] ) // there was no choice pending...why did we get response?
    {
        // log this?
        return;
    }

    stashFindPointerConst( g_RewardDefinition->rewardChoiceDictionary.hashRewardChoice, e->pl->pendingReward.pendingRewardName, &rewardChoice );

    if (!rewardChoice || !rewardChoice->ppChoices)
    { //we have an invalid reward
        LOG( LOG_ERROR, LOG_LEVEL_IMPORTANT, 0, "Bad RewardChoice name %s, level %d, group %d found\n",e->pl->pendingReward.pendingRewardName,e->pl->pendingReward.pendingRewardLevel,e->pl->pendingReward.pendingRewardVgroup);
        clearRewardChoice(e, 1);
        return;
    }
    if( choice >= eaSize(&rewardChoice->ppChoices) || choice < 0 ) // got a value larger than current choice permits!
    {
        LOG( LOG_ERROR, LOG_LEVEL_VERBOSE, 0, "Bad RewardChoice selection %d from choice %s",choice,e->pl->pendingReward.pendingRewardName);
         sendPendingReward(e);
        return;
    }
    for(i = eaSize(&rewardChoice->ppChoices[choice]->ppVisibleRequirements)-1; i >= 0; i--)
    {
        if(!chareval_Eval(e->pchar,rewardChoice->ppChoices[choice]->ppVisibleRequirements[i]->ppchRequires, rewardChoice->pchSourceFile))
        {
            LOG( LOG_ERROR, LOG_LEVEL_VERBOSE, 0, "Invalid RewardChoice selection %d from choice %s (requires not met)",choice,e->pl->pendingReward.pendingRewardName);
            sendPendingReward(e);
            return;
        }
    }
    for(i = eaSize(&rewardChoice->ppChoices[choice]->ppRequirements)-1; i >= 0; i--)
    {
        if(!chareval_Eval(e->pchar,rewardChoice->ppChoices[choice]->ppRequirements[i]->ppchRequires, rewardChoice->pchSourceFile))
        {
            LOG( LOG_ERROR, LOG_LEVEL_VERBOSE, 0, "Invalid RewardChoice selection %d from choice %s (requires not met)",choice,e->pl->pendingReward.pendingRewardName);
            sendPendingReward(e);
            return;
        }
    }

    // Copy pendingReward information locally and clear EntPlayer's reward log.
    // This is done to allow choice tables to call other choice tables via reward tables.
    pendingRewardLevel = e->pl->pendingReward.pendingRewardLevel;
    pendingRewardVgroup = e->pl->pendingReward.pendingRewardVgroup;
    clearRewardChoice(e, 1);

    if(!s_sbRewardLog.buff)
        initStuffBuff(&s_sbRewardLog, 1024);

    if(rewardChoice->ppChoices[choice]->rewardTableFlags & kRewardTableFlags_AtCritterLevel)
    {
        rl.iExpLevel = rl.iEffLevel = MINMAX(pendingRewardLevel, 0, MAX_PLAYER_SECURITY_LEVEL - 1 );
        rl.type = kPowerSystem_Powers;
    }
    else
    {
        rl = character_CalcRewardLevel(e->pchar);
    }

    addStringToStuffBuff(&s_sbRewardLog, "Reward Choice Received: %s, %s, level %d\n{\n", 
        e->name, rewardChoice->pchName, pendingRewardLevel);

    for(i = eaSize(&rewardChoice->ppChoices[choice]->ppchRewardTable)-1; i >= 0; i--)
    {

        const char *pchRewardTable = rewardChoice->ppChoices[choice]->ppchRewardTable[i];

        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Ch]:Rcv level %d %s from set %s (%d) %s", pendingRewardLevel+1, 
                pchRewardTable, 
                rewardChoice->pchName, choice,
                rewardChoice->ppChoices[choice]->rewardTableFlags & kRewardTableFlags_AtCritterLevel ? "(Forced to critter level)":"");


        // seems to be ok, reward away!
        rewardTeamMember(e, pchRewardTable, 1.0f, e->pl->pendingReward.pendingRewardLevel, true, 1.f, 1.f, 1.f, 
                        e->pl->pendingReward.pendingRewardVgroup, true, true, 
                        rl, REWARDSOURCE_CHOICE, CONTAINER_TEAMUPS, 0, NULL);
    }

    addStringToStuffBuff(&s_sbRewardLog, "\n}\n");
    LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "%s", s_sbRewardLog.buff);

    // clear out reward log.
    clearStuffBuff(&s_sbRewardLog);
}

int playerCanRespec( Entity *e )
{
    int i, counter;

    if( e->pl->taskforce_mode )
        return false;

    // has free respec
    if(  e->pl->respecTokens&(TOKEN_AQUIRED<<6) )
        return true;

    // has counter respec
    counter = (e->pl->respecTokens >> 24);
    if (counter > 0)
        return true;

    // respec trial
    for (i = 0; i < 3; i++)
    {
        if ((e->pl->respecTokens&(TOKEN_AQUIRED<<(i*2)) && !(e->pl->respecTokens&(TOKEN_USED<<(i*2)))))
            return true;
    }

    // patron & veteran respecs
    for (i = 0; i < 4; i++)
    {
        if ((e->pl->respecTokens&(TOKEN_AQUIRED<<(8 + (i*2))) && !(e->pl->respecTokens&(TOKEN_USED<<(8 + (i*2))))))
            return true;
    }

    return false;
}

int playerCanGiveRespec( Entity *e )
{
    int iLevel = character_CalcExperienceLevel( e->pchar )+1;

    if( iLevel < FIRST_RESPEC )
        return false;
    else if( iLevel < SECOND_RESPEC && (e->pl->respecTokens&(TOKEN_AQUIRED<<0)) )
        return false;
    else if( iLevel < THIRD_RESPEC && (e->pl->respecTokens&(TOKEN_AQUIRED<<0)) && (e->pl->respecTokens&(TOKEN_AQUIRED<<2)) )
        return false;
    else if( (e->pl->respecTokens&(TOKEN_AQUIRED<<0)) && (e->pl->respecTokens&(TOKEN_AQUIRED<<2)) && (e->pl->respecTokens&(TOKEN_AQUIRED<<4)) )
        return false;

    return true;
}

int playerHasFreespec( Entity *e )
{
    if(  e->pl->respecTokens&(TOKEN_AQUIRED<<6) )
        return true;
    else 
        return false;
}



int playerGiveRespecToken(Entity *e, const char *pchRewardTable)
{
    if( strnicmp( pchRewardTable, "respec", 6) != 0 && 
        strnicmp( pchRewardTable, "patrespec", 9) != 0 && 
        stricmp( pchRewardTable, "vetrespec") != 0    &&
        stricmp( pchRewardTable, "counterrespec") != 0    &&
        stricmp( pchRewardTable, "freerespec") != 0    )
        return false; // not a respec

    // force the update down to client
    e->send_costume = 1;

    // award them in order
    if ( strnicmp( pchRewardTable, "respec", 6) == 0 )
    {
        if( !playerCanGiveRespec(e) )
            return false; // player is not eligable for a respec

        if( !(e->pl->respecTokens&(TOKEN_AQUIRED<<0)) )
        {
            e->pl->respecTokens |= (TOKEN_AQUIRED<<0);
            return true;
        }
        else if( !(e->pl->respecTokens&(TOKEN_AQUIRED<<2)) )
        {
            e->pl->respecTokens |= (TOKEN_AQUIRED<<2);
            return true;
        }
        else if( !(e->pl->respecTokens&(TOKEN_AQUIRED<<4)) )
        {
            e->pl->respecTokens |= (TOKEN_AQUIRED<<4);
            return true;
        }
        else
        {
            addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d | Respec: %s failed (no available tokens %d)\n",
                        e->name, e->db_id,
                        pchRewardTable,
                        e->pl->respecTokens);
            return true; // catch all, should never happen
                         // returning true since this function has handled this reward, although nothing is given
        }
    }
    else if ( strnicmp( pchRewardTable, "patrespec", 9) == 0 )
    {
        if( !(e->pl->respecTokens&(TOKEN_AQUIRED<<14)) )
        {
            e->pl->respecTokens |= (TOKEN_AQUIRED<<14);
            return true;
        }
        else
        {
            addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d | Respec: %s failed (patron respec already granted %d)\n",
                        e->name, e->db_id, 
                        pchRewardTable,
                        e->pl->respecTokens);
            return true; // already rewarded - returning true since this function has handled this reward, although nothing is given
        }
    } 
    else if ( stricmp( pchRewardTable, "freerespec") == 0 )
    {
        if( !(e->pl->respecTokens&(TOKEN_AQUIRED<<6)) )
        {
            e->pl->respecTokens |= (TOKEN_AQUIRED<<6);
            return true;
        }
        else
        {
            addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d | Respec: %s failed (free respec already granted %d)\n",
                e->name, e->db_id, 
                pchRewardTable,
                e->pl->respecTokens);
            return true; // already rewarded - returning true since this function has handled this reward, although nothing is given
        }
    } 
    else if ( stricmp( pchRewardTable, "vetrespec") == 0 )
    {
        if( !(e->pl->respecTokens&(TOKEN_AQUIRED<<8)) )
        {
            e->pl->respecTokens |= (TOKEN_AQUIRED<<8);
            return true;
        }
        else if( !(e->pl->respecTokens&(TOKEN_AQUIRED<<10)) )
        {
            e->pl->respecTokens |= (TOKEN_AQUIRED<<10);
            return true;
        }
        else if( !(e->pl->respecTokens&(TOKEN_AQUIRED<<12)) )
        {
            e->pl->respecTokens |= (TOKEN_AQUIRED<<12);
            return true;
        }
        else
        {
            addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d | Respec: %s failed (veteran respec already granted %d)\n",
            e->name, e->db_id, 
            pchRewardTable,
            e->pl->respecTokens);
            return true; // catch all, should never happen
                            // returning true since this function has handled this reward, although nothing is given
        }
    }
    else if ( stricmp( pchRewardTable, "counterrespec") == 0 )
    {
        int counter = (e->pl->respecTokens >> 24);

        if (counter < 255)
            counter++;

        counter = counter << 24;

        // clear out old entries
        e->pl->respecTokens &= 0x00ffffff;

        // put new counter in place
        e->pl->respecTokens |= counter;
        return true;

    } else {
        addStringToStuffBuff(&s_sbRewardLog, "%20.20s %9d | Respec: %s failed (not a respec table)\n",
                                e->name, e->db_id,
                                pchRewardTable    );
        return false; // not a respec... shouldn't get here either
    }
}

void playerUseRespecToken(Entity *e)
{
    int i, counter;

    // use free respec first
    if( (e->pl->respecTokens>>6)&TOKEN_AQUIRED ) // csr token
    {
        e->pl->respecTokens &= ~(TOKEN_AQUIRED<<6);
        return;
    }

    // use counter respec
    counter = (e->pl->respecTokens >> 24);
    if (counter > 0)
    {
        counter--;

        // clear out old entries
        e->pl->respecTokens &= 0x00ffffff;

        // put new counter in place
        counter = counter << 24;
        e->pl->respecTokens |= counter;

        return;
    }

    // use patron respec
    if( (e->pl->respecTokens>>14)&TOKEN_AQUIRED ) 
    {
        e->pl->respecTokens &= ~(TOKEN_AQUIRED<<14);
        return;
    }

    // use trial respecs 
    if( (e->pl->respecTokens>>0)&TOKEN_AQUIRED && !((e->pl->respecTokens>>0)&TOKEN_USED) )
    {
        e->pl->respecTokens |= TOKEN_USED;
        return;
    }
    if( (e->pl->respecTokens>>2)&TOKEN_AQUIRED && !((e->pl->respecTokens>>2)&TOKEN_USED) )
    {
        e->pl->respecTokens |= (TOKEN_USED<<2);
        return;
    }
    if( (e->pl->respecTokens>>4)&TOKEN_AQUIRED && !((e->pl->respecTokens>>4)&TOKEN_USED) )
    {
        e->pl->respecTokens |= (TOKEN_USED<<4);
        return;
    }

    // use veteran respec
    for (i = 0; i < 3; i++) 
    {
        if( (e->pl->respecTokens>>(8 + (2*i)))&TOKEN_AQUIRED && !((e->pl->respecTokens>>(8 + (2*i)))&TOKEN_USED) )
        {
            e->pl->respecTokens |= (TOKEN_USED<<(8 + (2*i)));
            return;
        }
    }

}

// Defines for reputation modification
#define REPUTATION_PER_DAY -2.0
#define REPUTATION_MIN 0.0
#define REPUTATION_MAX 400.0

// Totally crappy function to give or remove reputation-based rewards
void playerUpdateReputationRewards(Entity *e)
{
    static int init = 1;    // Init to 0 when the power and contact are in, otherwise mapserver complains
    static ContactHandle hPvPContact = 0;    // PvP Contact that we add or remove
    static const BasePower *pPvPRewardPower = NULL;    // Temporary power that we add or remove

    float rep = e->pl->reputationPoints; // Called by reputation modifying functions, no need to recheck

    if (!init)
    {
        init = 1;
        hPvPContact = ContactGetHandle("Contacts/PvP/PvPReward.contact");
        pPvPRewardPower = powerdict_GetBasePowerByName(&g_PowerDictionary, "Temporary_Powers", "Temporary_Powers", "PvPTempPower");
    }

/*
    if (rep <= -20.0) // Debuff accuracy and damage for PvP and PvE combat
    {
        // Award
    }
    else
    {
        //Cancel Award
    }

    if (rep <= -17.0) // Always displayed "Ganker" title
    {
        if (!e->pl->bIsGanker)
        {
            e->pl->bIsGanker = true;
            e->pvp_update = true;
        }
    }
    else
    {
        if (e->pl->bIsGanker)
        {
            e->pl->bIsGanker = false;
            e->pvp_update = true;
        }
    }

    if (rep <= -10.0) // Debuff accuracy & damage for PvP combat
    {
        // Award
    }
    else
    {
        //Cancel Award
    }

    if (rep >= 5.0) // Unlock PvP Zone Inspiration Contact
    {
        StoryInfo* info = e->storyInfo;
        if (info && hPvPContact && !ContactGetInfo(info, hPvPContact) && ContactAdd(e, info, hPvPContact))
        {
            sendInfoBox(e, INFO_REWARD, "PvPInspirationContactAdd");
        }
    }
    else
    {
        if (hPvPContact && ContactDrop(e, hPvPContact))
        {
            sendInfoBox(e, INFO_REWARD, "PvPInspirationContactRemove");
        }
    }
*/

    if (rep >= 100.0) // "ManHunter" title
    {
        badge_Award(e, "ManHunter", 1);
    }

/*
    if (rep >= 20.0) // Unlock Base/PvP Zone teleport
    {
        if (!getRewardToken(e,"PvPTeleportReward"))
        {
            giveRewardToken(e,"PvPTeleportReward");
        }
    }
    else
    {
        removeRewardToken(e,"PvPTeleportReward");
    }
*/

    if (rep >= 200.0) // PvP Costume Piece
    {
        costume_Award(e, "PvPCostumeReward", true, 0 );
    }

    if (rep >= 300.0) // PvP Temporary power
    {
        if (pPvPRewardPower
            && !character_OwnsPower(e->pchar, pPvPRewardPower)
            && character_AddRewardPower(e->pchar, pPvPRewardPower))
        {
            sendInfoBox(e, INFO_REWARD, "TempPowerYouReceived", pPvPRewardPower->pchDisplayName);
            serveFloater(e, e, "FloatFoundTempPower", 0.0f, kFloaterStyle_Attention, 0);
            power_sendTrayAdd( e, pPvPRewardPower );
        }
    }
    else
    {
        if (pPvPRewardPower && character_OwnsPower(e->pchar, pPvPRewardPower))
        {
            character_RemoveBasePower(e->pchar, pPvPRewardPower);
        }
    }

    if (rep >= 400.0) // "HeadHunter" badge
    {
        badge_Award(e, "HeadHunter", 1);
    }
}

float playerGetReputation(Entity *e)
{
    float fOldRep = e->pl->reputationPoints;
    U32 t = dbSecondsSince2000();
    long dt = (long)t - (long)e->pl->reputationUpdateTime;

    if (dt > 0 && e->pl->reputationPoints > 0.0)
    {
        float day = (float)dt/(60.0*60.0*24.0);
        e->pl->reputationPoints += REPUTATION_PER_DAY*day;
        if(e->pl->reputationPoints < REPUTATION_MIN)
            e->pl->reputationPoints = REPUTATION_MIN;
    }

    e->pl->reputationUpdateTime = t;

    playerUpdateReputationRewards(e);
    return e->pl->reputationPoints;
}

float playerSetReputation(Entity *e, float fRep)
{
    float fOldRep = e->pl->reputationPoints;
    U32 uOldTime = e->pl->reputationUpdateTime;

    e->pl->reputationPoints = CLAMP(fRep,REPUTATION_MIN,REPUTATION_MAX);
    e->pl->reputationUpdateTime = dbSecondsSince2000();

    playerUpdateReputationRewards(e);

    return e->pl->reputationPoints;
}

float playerAddReputation(Entity *e, float fRep)
{
    float fCur = playerGetReputation(e);
    return playerSetReputation(e,fCur+fRep) - fCur;
}


//------------------------------------------------------------
//  bit the bitfield that representing this ents state
//----------------------------------------------------------
RewardItemSetTarget rewarditemsettarget_BitsFromEnt(Entity *e)
{
    RewardItemSetTarget res = kItemSetTarget_NONE;

    if( verify( e && e->pl ))
    {
        res =  kItemSetTarget_Player
            |  (e->pl->supergroup_mode ? kItemSetTarget_Sgrp : 0);
    }
    return res;
}

RewardTokenType reward_GetTokenType( const char *rewardTokenName )
{
    RewardTokenType res = kRewardTokenType_None;
    const RewardTable *reward = stashFindPointerReturnPointerConst(g_RewardDefinition->rewardDictionary.hashRewardTable, rewardTokenName);

    if( reward )
    {
        res = reward->permanentRewardToken;
    }

    // --------------------
    // finally

    return res;
}

/**********************************************************************func*
 * character_IsRewardedAsExemplar
 *
 */
bool character_IsRewardedAsExemplar(Character *pchar)
{
    return (pchar->iCombatLevel < character_CalcExperienceLevel(pchar))
        || character_IsExemplar(pchar,1) || character_IsTFExemplar(pchar);
}


////////////////////////////////////////////////////////////////////////
static StashTable rewardVars;

void rewardVarValueDestructor(void *pValue)
{
    char *pString = pValue;

    if (pString)
        free(pString);
}

void rewardVarClear(void)
{
    if (rewardVars)
        stashTableDestroyEx(rewardVars, 0, rewardVarValueDestructor);

    rewardVars = NULL;
}

void rewardVarSet(char *key, char *value)
{

    if (!key || !value)
        return;

    if (!rewardVars)
        rewardVars = stashTableCreateWithStringKeys(20, StashDeepCopyKeys);

    stashAddPointer(rewardVars, key, strdup(value), true);
}

char *rewardVarGet(char *key)
{
    StashElement pElement = NULL;

    stashFindElement(rewardVars, key, &pElement);
    
    if (pElement)
    {
        return stashElementGetPointer(pElement);
    } else {
        return NULL;
    }
}
typedef struct WeeklyTFStruct
{
    unsigned int epochStartTime;
    MeritRewardStoryArc **weeklyTFTokenList;
}WeeklyTFStruct;

static WeeklyTFStruct weeklyTFStruct = { 0 };

static char *originalTokenList = NULL;
void reward_SetWeeklyTFList(char *tokenList)
{
    if (originalTokenList)
        free(originalTokenList);
    originalTokenList = strdup(tokenList);
}

void reward_UpdateWeeklyTFTokenList()
{
    char *tokenName = NULL, *context;
    if (!originalTokenList)
        return;
    eaDestroy(&weeklyTFStruct.weeklyTFTokenList);
    tokenName = strtok_s(originalTokenList, ":", &context);
    weeklyTFStruct.epochStartTime = (unsigned int) (atoi(tokenName));
    while (tokenName = strtok_s(NULL, ":", &context))
    {
        if (stricmp(tokenName, "none") != 0)
        {
            MeritRewardStoryArc *rewardArc = NULL;
            stashFindPointerConst(g_RewardDefinition->meritRewardDictionary.hashMeritReward, tokenName, &rewardArc);
            if (rewardArc && rewardArc->bonusToken)
            {
                eaPushUnique(&weeklyTFStruct.weeklyTFTokenList, rewardArc);
            }
        }
        else
        {
            break;
        }
    }
    if (originalTokenList)
    {
        free(originalTokenList);
        originalTokenList = NULL;
    }
}

void reward_PrintWeeklyTFTokenListActive(char **buffer)
{
    int numTokens;
    char datestr[200];
    timerMakeDateStringFromSecondsSince2000(datestr, weeklyTFStruct.epochStartTime);
    estrConcatf(buffer, "Epoch Start Time: %s\n", datestr);
    if (numTokens = eaSize(&weeklyTFStruct.weeklyTFTokenList))
    {
        int i;
        for (i = 0; i < numTokens; ++i)
        {
            estrConcatf(buffer,"%s\n",weeklyTFStruct.weeklyTFTokenList[i]->name);
        }
    }
    else
    {
        estrConcatStaticCharArray(buffer,"none\n");
    }
}

int reward_FindInWeeklyTFTokenList(char *tokenName)
{
    MeritRewardStoryArc *rewardArc = NULL;
    stashFindPointerConst(g_RewardDefinition->meritRewardDictionary.hashMeritReward, tokenName, &rewardArc);
    if (rewardArc && rewardArc->bonusToken)
    {
        if (eaFind(&weeklyTFStruct.weeklyTFTokenList, rewardArc) != -1)
        {
            return true;
        }
    }
    return false;
}

void reward_PrintWeeklyTFTokenListAll(char **buffer)
{
    int numArcs = eaSize(&g_RewardDefinition->meritRewardDictionary.ppStoryArc);
    if (numArcs)
    {
        int j = 0;
        int i;
        for (i = 0; i < numArcs; ++i)
        {
            const char *tokenName = g_RewardDefinition->meritRewardDictionary.ppStoryArc[i]->name;
            if (tokenName && g_RewardDefinition->meritRewardDictionary.ppStoryArc[i]->bonusToken)
            {
                MeritRewardStoryArc *rewardArc = NULL;
                stashFindPointerConst(g_RewardDefinition->meritRewardDictionary.hashMeritReward, tokenName, &rewardArc);
                if (rewardArc)
                {
                    estrConcatf(buffer,"%s%s",tokenName, (j < 3) ? ", " : "\n");
                    j = (j+1) %4;
                }
                else
                {
                    estrConcatf(buffer,"\nError. Bonus Token (%s) does not have a corresponding reward token. Check defs\\reward\\rewardTokens.reward\n", tokenName);
                    return;
                }
            }
        }
    }
    else
    {
        estrConcatStaticCharArray(buffer, "none\n");
    }
}

unsigned int weeklyTF_GetEpochStartTime()
{
    return weeklyTFStruct.epochStartTime;
}

// returns whether you should immediately decrement the salvage being opened...
// This should eventually be replaced with prepareAtomicTransaction()...
static bool prepareAtomicTransactionFromRewardString(Entity *e, const char *salvageName, char *desc)
{
    char *item, *rest;
    char *header = "accountproduct ";
    int headerLength = (int)strlen(header);
    int transactionCount = 0, transactionIndex;
    SkuId skuIds[MAX_MULTI_GAME_TRANSACTIONS];
    U32 grantedValues[MAX_MULTI_GAME_TRANSACTIONS];
    U32 rarities[MAX_MULTI_GAME_TRANSACTIONS];
    Packet *pak_out;

    while (desc != NULL)
    {
        rest = strchr(desc, ',');
        if (rest == NULL)
        {
            item = desc;
            desc = NULL;
        } else {
            *rest = 0;
            item = desc;
            desc = rest + 2;
        }

        if (item && strnicmp(item, header, headerLength) == 0)
        {
            char skuName[100];

            item += headerLength;   // remove header
            sscanf(item, "%99s %d %d", skuName, &grantedValues[transactionCount], &rarities[transactionCount]);

            if (!devassert(transactionCount < MAX_MULTI_GAME_TRANSACTIONS))
            {
                dbLog("Salvage:Open", e, "salvage count must be less than MAX_MULTI_GAME_TRANSACTIONS %s", salvageName);
                return false;
            }

            skuIds[transactionCount] = skuIdFromString(skuName);

            transactionCount++;
        }
    }
    
    if (transactionCount)
    {
        pak_out = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_MULTI_GAME_TRANSACTION);
        pktSendBitsAuto(pak_out, e->auth_id);
        pktSendBitsAuto(pak_out, e->db_id);
        pktSendBitsAuto(pak_out, transactionCount);
        if (salvageName)
        {
            pktSendBits(pak_out, 1, 1);
            pktSendString(pak_out, salvageName);
        }
        else
        {
            pktSendBits(pak_out, 1, 0);
        }

        for (transactionIndex = 0; transactionIndex < transactionCount; transactionIndex++)
        {
            pktSendBitsAuto2(pak_out, skuIds[transactionIndex].u64);
            pktSendBitsAuto(pak_out, grantedValues[transactionIndex]);
            pktSendBitsAuto(pak_out, 0); // claimed
            pktSendBitsAuto(pak_out, rarities[transactionIndex]);
        }

        pktSend(&pak_out,&db_comm_link);
    }

    return transactionCount == 0;
}

void openSalvage(Entity *e, SalvageItem const *sal)
{
    char *rewardList = NULL;
    U32 time = dbSecondsSince2000();

    if(!sal)
        return;
    else if (sal->ppchRewardTables == NULL)
        dbLog("Salvage:Open", e, "player tried to open an unopenable salvage named %s", sal->pchName);
    else if(!character_CanAdjustSalvage(e->pchar, sal, -1)) 
        dbLog("Salvage:Open", e, "require one of %s for open", sal->pchName );
    else if (sal->ppchOpenRequires != NULL && !chareval_Eval(e->pchar, sal->ppchOpenRequires, "defs/invention/salvage.salvage"))
        dbLog("Salvage:Open", e, "player failed salvage open requirements %s", sal->pchName);
    else if( time - e->pl->last_certification_request < 2 )
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CertificationThrottled"), INFO_USER_ERROR, 0 );
    else
    {
        estrCreate(&rewardList);
        rewardList[0] = 0;
        rewardFindDefAndApplyToEnt(e, sal->ppchRewardTables, VG_NONE, character_GetCombatLevelExtern(e->pchar), 
            false, REWARDSOURCE_SALVAGE_OPEN, &rewardList);

        if (prepareAtomicTransactionFromRewardString(e, sal->pchName, rewardList))
        {
            character_AdjustSalvage(e->pchar, sal, -1, "open", false);
        }

        e->pl->last_certification_request = time;
    }
}

void openSalvageByName(Entity *e, const char *nameSalvage)
{
    SalvageItem const *sal;

    sal = salvage_GetItem(nameSalvage);

    if (!sal)
        dbLog("cheater", e, "can't find salvage named %s", nameSalvage);
    else
        openSalvage(e, sal);
}

static void cmsalvage_UseNow_WeeklyStrikeTargetTimerReset( Entity* e, char** pEStr_ClientMsgStr, bool* bCanDo )
{
    if ( getRewardToken( e, "WeeklyTFSync" ) != NULL )
    {
        estrPrintCharString(pEStr_ClientMsgStr, "TimerReset_WST_HasToken" );
        *bCanDo = true;
    }
    else
    {
        estrPrintCharString(pEStr_ClientMsgStr, "TimerReset_WST_NoCanDo" );
        *bCanDo = false;
    }
}

static void cmsalvage_UseNow_MissionTimerReset( Entity* e, char** pEStr_ClientMsgStr, bool* bCanDo )
{
    int numEarned = alignmentshift_countPointsEarnedRecently( e );
    if ( numEarned == ALIGNMENT_POINT_MAX_COUNT )
    {
        estrPrintCharString(pEStr_ClientMsgStr, "TimerReset_AM_HasAllTokens" );
        *bCanDo = true;
    }
    else if ( numEarned != 0 )
    {
        estrPrintCharString(pEStr_ClientMsgStr, "TimerReset_AM_HasSomeTokens" );
        *bCanDo = true;
    }
    else
    {
        estrPrintCharString(pEStr_ClientMsgStr, "TimerReset_AM_NoCanDo" );
        *bCanDo = false;
    }
}

#if 0
static void cmsalvage_UseNow_EmpyreanTimerReset( Entity* e, char** pEStr_ClientMsgStr, bool* bCanDo )
{
    static const int kTotalTokens = 5;    // TBD, use the real number
    int numTokens = 5;    // LSTL_TEST, get the real value
    
    if ( numTokens == kTotalTokens )
    {
        estrPrintCharString(pEStr_ClientMsgStr, "TimerReset_EMP_HasAllTokens");
        *bCanDo = true;
    }
    else if ( numTokens != 0 )
    {
        estrPrintCharString(pEStr_ClientMsgStr, "TimerReset_EMP_HasSomeTokens");
        *bCanDo = true;
    }
    else
    {
        estrPrintCharString(pEStr_ClientMsgStr, "TimerReset_EMP_NoCanDo");
        *bCanDo = false;
    }
}
#endif

#if 0
static void cmsalvage_UseNow_SignatureStoryArcTimerReset( Entity* e, char** pEStr_ClientMsgStr, bool* bCanDo )
{
    static const int kTotalTokens = 4;
    int numTokens = 4;    // LSTL_TEST, get the real value
    
    if ( numTokens == kTotalTokens )
    {
        estrPrintCharString(pEStr_ClientMsgStr, "TimerReset_SSA_HasAllTokens");
        *bCanDo = true;
    }
    else if ( numTokens != 0 )
    {
        estrPrintCharString(pEStr_ClientMsgStr, "TimerReset_SSA_HasSomeTokens" );
        *bCanDo = true;
    }
    else
    {
        estrPrintCharString(pEStr_ClientMsgStr, "TimerReset_SSA_NoCanDo");
        *bCanDo = false;
    }
}
#endif

//------------------------------------------------------------
// check to see if this entity can process this salvage
// without having to apply it to a recipe.
//----------------------------------------------------------
void salvage_queryImmediateUseStatus( Entity * e, const char * salvageName, 
                U32* pFlags /*SalvageImmediateUseStatus OUT*/, char** pEStr_ClientMsgStr /*OUT*/ )
{
    typedef void (*tSalvageUsageApprovalCB)( Entity* e, char** pEStr_ClientMsgStr, bool* bCanDo );
    static struct {
        const char* salvageName;
        tSalvageUsageApprovalCB    cb;
    } approvalTable[] = {
        // Weekly Strike Target Timer
        { "s_TimerReset_WST", cmsalvage_UseNow_WeeklyStrikeTargetTimerReset },
        // Mission Timer
        { "s_TimerReset_AM", cmsalvage_UseNow_MissionTimerReset },
        // Empyrean Timer
        //{ "s_TimerReset_EMP", cmsalvage_UseNow_EmpyreanTimerReset },
        // Signature Story Arc Timer
        //{ "s_TimerReset_SSA", cmsalvage_UseNow_SignatureStoryArcTimerReset }
    };

    int i;
    bool bCanDo = false;
    bool bKnown = false;
    SalvageItem const *sal = salvage_GetItem(salvageName);

    estrClear(pEStr_ClientMsgStr);
        
    if (( !sal ) || ( !( sal->flags & SALVAGE_IMMEDIATE ) ))
    {
        *pFlags = kSalvageImmediateUseFlag_NotApplicable;
        return;
    }    

    for ( i=0; i < ARRAY_SIZE(approvalTable); i++ )
    {
        if ( stricmp( salvageName, approvalTable[i].salvageName ) == 0 )
        {
            (*approvalTable[i].cb)( e, pEStr_ClientMsgStr, &bCanDo );
            bKnown = true;
            break;
        }
    }
    *pFlags = 0;
    if ( bKnown )
    {
        if ( ! bCanDo )
        {
            *pFlags |= kSalvageImmediateUseFlag_MissingPrereqs;
        }
    }
    else
    {
        *pFlags |= kSalvageImmediateUseFlag_NotApplicable;
    }
}


/* End of File */

