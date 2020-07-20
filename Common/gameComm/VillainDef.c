/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "gameComm/villainDef.h"
#include "gameComm/npc.h"        // For NPC structure defintion
#include <utilitieslib/utils/error.h>

#include <utilitieslib/components/earray.h>

#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/log.h>

// For character stat assignment
#if SERVER
#include "dbcomm/dbcomm.h"
#include "entity/entserver.h"        // For svrChangeBody
#include "entity/character_combat.h" // For character_ActivateAllAutoPowers
#include "generator/entgen.h"        // For entCreateEx()
#include "language/langServerUtil.h"    // For server-side message stores.
#include "generator/encounterPrivate.h"
#include "storyarc/mission.h"
#include "seq/animbitlist.h"
#include "entity/PCC_Critter.h"
#include "entity/CustomVillainGroup.h"
#include "entity/Supergroup.h"
#include "entity/power_customization.h"
#include "dbcomm/logcomm.h"
#include "script/scriptengine.h"
#endif

#include "entity/Entity.h"
#include "entity/character_base.h"   // For character structure definition
#include "entity/character_level.h"  // For character_Level
#include "entity/classes.h"    // For character class initialization
#include "entity/origins.h"    // For character origin initialization
#include "entity/powers.h"        // For character power assignment
#include "entity/costume.h"    // For structure Costume
#include "language/commonLangUtil.h"
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/version/AppVersion.h>
#include "comm_game.h"
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/components/StashTable.h>
#include "gameData/costume_data.h"
#include "gameData/BodyPart.h"
#include <utilitieslib/components/SharedHeap.h>


//---------------------------------------------------------------------------------------------------------------
// Villain definition parsing
//---------------------------------------------------------------------------------------------------------------

static bool villainConstructCriteriaLookupTables(ParseTable pti[], VillainDefList* vlist, bool shared_memory);
static void villainCheckRequirements();
static void freeVillainGroupMembersList(VillainDefList* vlist);

SHARED_MEMORY VillainDefList villainDefList;

typedef struct {
    const VillainGroup **villainGroups;
} VillainGroupList;
SHARED_MEMORY VillainGroupList villainGroups;

DefineContext *g_pParseVillainGroups = NULL;
STATIC_DEFINE_WRAPPER(ParseVillainGroups, g_pParseVillainGroups);

SHARED_MEMORY PowerSetConversionTable g_PowerSetConversionTable;

VillainGroupEnum VG_NONE = 0;
VillainGroupEnum VG_MAX = 0;

// used with villainGroupGetEnum
#define VG_NICTUS "Nictus"

StaticDefineInt ParseVillainRankEnum[] =
{
    DEFINE_INT
    {"Small",            VR_SMALL},
    {"Minion",            VR_MINION},
    {"Lieutenant",        VR_LIEUTENANT},
    {"Sniper",            VR_SNIPER},
    {"Boss",            VR_BOSS},
    {"Elite",            VR_ELITE},
    {"ArchVillain",        VR_ARCHVILLAIN},
    {"ArchVillain2",    VR_ARCHVILLAIN2},
    {"BigMonster",        VR_BIGMONSTER},
    {"Pet",                VR_PET},
    {"Destructible",    VR_DESTRUCTIBLE},
    DEFINE_END
};

// this is how the color is level-adjusted
static int villainRankConningAdjust[] =
{
    0,                    // VR_NONE
    -1,                    // VR_SMALL
    0,                    // VR_MINION
    1,                    // VR_LIEUTENANT
    1,                    // VR_SNIPER
    2,                    // VR_BOSS
    3,                    // VR_ELITE
    5,                    // VR_ARCHVILLAIN
    5,                    // VR_ARCHVILLAIN2
    100,                // VR_BIGMONSTER
    1,                    // VR_PET
    1,                    // VR_DESTRUCTIBLE
};

StaticDefineInt ParseVillainExclusion[] =
{
    DEFINE_INT
    {"CoHOnly",            VE_COH},
    {"CoVOnly",            VE_COV},
    DEFINE_END
};

StaticDefineInt ModBoolEnum[];

TokenizerParseInfo ParseVillainLevelDef[] =
{
    {    "Level",        TOK_STRUCTPARAM | TOK_INT(VillainLevelDef, level, 0)},
    {    "DisplayNames",    TOK_STRINGARRAY(VillainLevelDef, displayNames)},
    {    "Costumes",        TOK_STRINGARRAY(VillainLevelDef, costumes)},
    {    "XP",            TOK_INT(VillainLevelDef, experience, 0)},
    {    "{",            TOK_START,        0 },
    {    "}",            TOK_END,            0 },
    {    "", 0, 0 }
};

TokenizerParseInfo ParsePetCommandStrings[] =
{
    {    "{",                TOK_START,        0 },
    {    "Passive",            TOK_STRINGARRAY(PetCommandStrings, ppchPassive) },
    {    "Defensive",        TOK_STRINGARRAY(PetCommandStrings, ppchDefensive) },
    {    "Aggressive",        TOK_STRINGARRAY(PetCommandStrings, ppchAggressive) },

    {    "AttackTarget",        TOK_STRINGARRAY(PetCommandStrings, ppchAttackTarget) },
    {    "AttackNoTarget",    TOK_STRINGARRAY(PetCommandStrings, ppchAttackNoTarget) },
    {    "StayHere",            TOK_STRINGARRAY(PetCommandStrings, ppchStayHere) },
    {    "UsePower",            TOK_STRINGARRAY(PetCommandStrings, ppchUsePower) },
    {    "UsePowerNone",        TOK_STRINGARRAY(PetCommandStrings, ppchUsePowerNone) },
    {    "FollowMe",            TOK_STRINGARRAY(PetCommandStrings, ppchFollowMe) },
    {    "GotoSpot",            TOK_STRINGARRAY(PetCommandStrings, ppchGotoSpot) },
    {    "Dismiss",            TOK_STRINGARRAY(PetCommandStrings, ppchDismiss) },
    {    "}",                TOK_END,            0 },
    {    "", 0, 0 }
};

StaticDefineInt ParseVillainDefFlags[] =
{
    DEFINE_INT
    { "NoGroupBadgeStat",            VILLAINDEF_NOGROUPBADGESTAT },
    { "NoRankBadgeStat",            VILLAINDEF_NORANKBADGESTAT },
    { "NoNameBadgeStat",            VILLAINDEF_NONAMEBADGESTAT },
    { "NoGenericBadgeStat",            VILLAINDEF_NOGENERICBADGESTAT },
    DEFINE_END
};

extern TokenizerParseInfo ParseFakeScriptDef[];

TokenizerParseInfo ParseVillainDef[] =
{
    {    "Name",                        TOK_STRUCTPARAM | TOK_STRING(VillainDef, name, 0)},
    {    "Class",                    TOK_STRING(VillainDef, characterClassName, 0)},
    {    "Gender",                    TOK_INT(VillainDef, gender,    0),    ParseGender },
    {    "DisplayDescription",        TOK_STRING(VillainDef, description, 0)},
    {    "GroupDescription",            TOK_STRING(VillainDef, groupdescription, 0) },
    {    "DisplayClassName",            TOK_STRING(VillainDef, displayClassName, 0) },
    {    "AIConfig",                    TOK_STRING(VillainDef, aiConfig, 0)},
    {    "VillainGroup",                TOK_INT(VillainDef, group,    0), ParseVillainGroupEnum},
    {    "Power",                    TOK_STRUCT(VillainDef, powers, ParsePowerNameRef)},
    {    "Level",                    TOK_STRUCT(VillainDef, levels, ParseVillainLevelDef)},
    {    "Rank",                        TOK_INT(VillainDef, rank,        0), ParseVillainRankEnum},
    {    "Ally",                        TOK_STRING(VillainDef, ally, 0) },
    {    "Gang",                        TOK_STRING(VillainDef, gang, 0) },
    {    "Exclusion",                TOK_INT(VillainDef, exclusion ,    0), ParseVillainExclusion},
    {    "IgnoreCombatMods",            TOK_BOOLFLAG(VillainDef, ignoreCombatMods, 0) },
    {    "CopyCreatorMods",            TOK_BOOLFLAG(VillainDef, copyCreatorMods, 0) },
    {    "IgnoreReduction",            TOK_BOOLFLAG(VillainDef, ignoreReduction, 0) },
    {    "CanZone",                    TOK_BOOLFLAG(VillainDef, canZone, 0) },
    {    "SpawnLimit",                TOK_INT(VillainDef, spawnlimit, -1) },
    {    "SpawnLimitMission",        TOK_INT(VillainDef, spawnlimitMission, -2) },
    {    "AdditionalRewards",        TOK_REDUNDANTNAME | TOK_STRINGARRAY(VillainDef, additionalRewards), },
    {    "SuccessRewards",            TOK_STRINGARRAY(VillainDef, additionalRewards), },
    {    "FavoriteWeapon",            TOK_STRING(VillainDef, favoriteWeapon, 0), },
    {    "DeathFailureRewards",        TOK_STRINGARRAY(VillainDef, skillHPRewards), },
    {    "IntegrityFailureRewards",    TOK_REDUNDANTNAME | TOK_STRINGARRAY(VillainDef, skillStatusRewards), },
    {    "StatusFailureRewards",        TOK_STRINGARRAY(VillainDef, skillStatusRewards), },
    {    "RewardScale",                TOK_F32(VillainDef, rewardScale, 1)},
    {    "PowerTags",                TOK_STRINGARRAY(VillainDef, powerTags), },
    {    "SpecialPetPower",            TOK_STRING(VillainDef, specialPetPower, 0), },
    {   "FileName",                    TOK_CURRENTFILE(VillainDef, fileName), },
    {    "FileAge",                    TOK_TIMESTAMP(VillainDef, fileAge), },
    {    "PetCommandStrings",        TOK_STRUCT(VillainDef, petCommandStrings, ParsePetCommandStrings) },
    {    "PetVisibility",            TOK_INT(VillainDef, petVisibility, -1)  },
    {    "PetCommandability",        TOK_INT(VillainDef, petCommadability, 0), },
    {    "BadgeStat",                TOK_STRING(VillainDef, customBadgeStat, 0), },
    {    "Flags",                    TOK_FLAGS(VillainDef,flags,0), ParseVillainDefFlags },
#if SERVER
    { "ScriptDef",                        TOK_STRUCT(VillainDef,scripts, ParseScriptDef) },
#else
    { "ScriptDef",                        TOK_NULLSTRUCT(ParseFakeScriptDef) },
#endif
    {    "{",                        TOK_START,            0 },
    {    "}",                        TOK_END,            0 },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseVillainDefBegin[] =
{
    {    "VillainDef",    TOK_STRUCT(VillainDefList, villainDefs, ParseVillainDef) },
    {    "", 0, 0 }
};

StaticDefineInt ParseGroupAlly[] =
{
    DEFINE_INT
    {"None",            VG_ALLY_NONE},
    {"Hero",            VG_ALLY_HERO},
    {"Villain",            VG_ALLY_VILLAIN},
    {"Monster",            VG_ALLY_MONSTER},
    DEFINE_END
};

TokenizerParseInfo ParseVillainGroup[] =
{
    {    "Name",                    TOK_STRUCTPARAM | TOK_STRING(VillainGroup, name, 0)},
    {    "DisplayName",            TOK_STRING(VillainGroup, displayName, 0)},
    {    "DisplaySingluar",        TOK_STRING(VillainGroup, displaySingluarName, 0)},
    {    "DisplayLeaderName",    TOK_STRING(VillainGroup, displayLeaderName, 0)},
    {    "Description",            TOK_STRING(VillainGroup, description, 0)},
    {    "ShowInKiosk",            TOK_INT(VillainGroup, showInKiosk,    0),    ModBoolEnum },
    {    "Ally",                    TOK_INT(VillainGroup, groupAlly,    VG_ALLY_NONE),    ParseGroupAlly },
    {    "{",                    TOK_START,        0 },
    {    "}",                    TOK_END,            0 },
    {    "", 0, 0 }
};


TokenizerParseInfo ParseVillainGroupBegin[] =
{
    {    "VillainGroup",            TOK_STRUCT(VillainGroupList, villainGroups, ParseVillainGroup)},
    {    "", 0, 0 }
};

StaticDefineInt ParseVillainDoppelFlags[] =
{
    DEFINE_INT
    {    "Tall",                VDF_TALL            },
    {    "Short",            VDF_SHORT            },
    {    "Shadow",            VDF_SHADOW            },
    {    "Inverse",            VDF_INVERSE            },
    {    "Reverse",            VDF_REVERSE            },
    {    "BlackAndWhite",    VDF_BLACKANDWHITE    },
    {    "Demon",            VDF_DEMON            },
    {    "Angel",            VDF_ANGEL            },
    {    "Pirate",            VDF_PIRATE            },
    {    "RandomPower",        VDF_RANDOMPOWER        },
    {    "Ghost",            VDF_GHOST            },
    {    "NoPowers",            VDF_NOPOWERS        },
    DEFINE_END
};

typedef enum aiConfigEnum
{
    AICONFIG_RANGED,
    AICONFIG_MELEE,
    AICONFIG_COUNT
} aiConfigEnum;
static const char* aiConfigs[]  = {"Default_Ranged", "Default_Melee"};

static int __cdecl compareVillainDefNames(const VillainDef** def1, const VillainDef** def2)
{
    return stricmp((*def1)->name ? (*def1)->name : "", (*def2)->name ? (*def2)->name : "");
}

static int villainDefPreProcess(VillainDef *def)
{
    int ret = 1;
    int j;
    int levelCursor;

    // Default SpawnLimitMission to SpawnLimit if unspecified
    if (def->spawnlimitMission == -2)
        def->spawnlimitMission = def->spawnlimit;

    // We've now processed this
    def->processAge = def->fileAge;

    // Make sure the villain has some levels defined.
    if(!eaSize(&def->levels))
    {
        ErrorFilenamef(def->fileName, "VillainDef: \"%s\" does not have any levels defined\n", def->name);
        return 0;
    }

    for(j = 0, levelCursor = def->levels[0]->level; j < eaSize(&def->levels); j++, levelCursor++)
    {
        const VillainLevelDef* levelDef = def->levels[j];

        if(levelCursor != levelDef->level)
        {
            ErrorFilenamef(def->fileName, "VillainDef: \"%s\" levels are not contiguous or in order\n", def->name);
            return 0;
        }
    }

#if SERVER
    for(j = 0; j < eaSize(&def->levels); j++)
    {
        const VillainLevelDef* levelDef = def->levels[j];
        int k;

        for(k = eaSize(&levelDef->costumes) - 1; k >= 0; k--)
        {
            const char* costumeName = levelDef->costumes[k];
            const NPCDef* npcDef = npcFindByName(costumeName, NULL);

            if(!npcDef || stricmp(npcDef->name, costumeName) != 0)
            {
                ErrorFilenamef(def->fileName, "VillainDef: \"%s\" level %d: Invalid costume \"%s\"\n", def->name, levelDef->level, costumeName);
                ret = 0;
            }
        }
    }

    // Validate all NPC powers
    for(j = eaSize(&def->powers) - 1; j >= 0; j--)
    {
        const BasePowerSet* basePowerSet;
        const BasePower* basePower;
        const PowerNameRef* powerNameRef = eaGetConst(&def->powers, j);

        if((basePowerSet = powerdict_GetBasePowerSetByName(&g_PowerDictionary, powerNameRef->powerCategory, powerNameRef->powerSet)) == NULL)
        {
            ErrorFilenamef(def->fileName, "VillainDef: \"%s\" Invalid power set \"%s.%s\"\n", def->name, powerNameRef->powerCategory, powerNameRef->powerSet);
            ret = 0;
        }
        if(powerNameRef->power && powerNameRef->power[0] != '*')
        {
            if((basePower = powerdict_GetBasePowerByName(&g_PowerDictionary, powerNameRef->powerCategory, powerNameRef->powerSet, powerNameRef->power))==NULL)
            {
                ErrorFilenamef(def->fileName, "VillainDef: \"%s\" Invalid power \"%s.%s.%s\"\n", def->name, powerNameRef->powerCategory, powerNameRef->powerSet, powerNameRef->power);
                ret = 0;
            }
        }
    }

#endif
    return ret;
}

static bool villainDefsPreProcessAll(TokenizerParseInfo pti[], VillainDefList* vlist)
{
    int i;
    bool ret = true; // assume successful

    eaQSort(cpp_const_cast(VillainDef**)(vlist->villainDefs), compareVillainDefNames);

    for(i = eaSize(&vlist->villainDefs)-1; i >= 0; i--)
    {
        VillainDef* def = cpp_const_cast(VillainDef*)(vlist->villainDefs[i]);
        ret &= villainDefPreProcess(def);
    }
    return ret;
}

static int villainDefsPreProcessModified(VillainDefList* vlist)
{
    int i;
    int ret = 1; // assume successful

    eaQSort(cpp_const_cast(VillainDef**)(vlist->villainDefs), compareVillainDefNames);

    for(i = eaSize(&vlist->villainDefs)-1; i >= 0; i--)
    {
        VillainDef* def = cpp_const_cast(VillainDef*)(vlist->villainDefs[i]);
        if (def->fileAge > def->processAge) //only process unprocessed
        {
            ret &= villainDefPreProcess(def);
        }
    }
    return ret;
}

static void villainDefReload(const char* relpath, int when)
{
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    if (ParserReloadFile(relpath, ParseVillainDefBegin, sizeof(VillainDefList), cpp_const_cast(VillainDefList*)(&villainDefList), NULL, NULL))
    {
        VillainDefList* vlist = cpp_const_cast(VillainDefList*)(&villainDefList);

        // clean up before init again
        stashTableDestroyConst(vlist->villainNameHash);
        vlist->villainNameHash = NULL;

        villainDefsPreProcessModified(vlist);
        freeVillainGroupMembersList(vlist);
        villainConstructCriteriaLookupTables(ParseVillainDefBegin, vlist, false);

        // Validate pet powers again now that we've changed VillainDefs
        powerdict_ValidateEntCreate(&g_PowerDictionary);
    }
    else
    {
        Errorf("Error reloading villain: %s\n", relpath);
    }
}



void villainReadDefFiles(bool bNewAttribs)
{
    int i;

#if SERVER 
    int flags = PARSER_SERVERONLY;
#else
    int flags = 0;
#endif

    /**********************************************************************/
    // Loading all the villain groups

    ParserLoadFilesShared("SM_VillainGroups.bin", NULL, "Defs/villainGroups.def", "VillainGroups.bin", flags, ParseVillainGroupBegin,
        &villainGroups, sizeof(villainGroups), NULL, NULL, NULL, NULL, NULL);

    // creating parse table for villain groups
    if(g_pParseVillainGroups!=NULL)
    {
        DefineDestroy(g_pParseVillainGroups);
    }
    g_pParseVillainGroups = DefineCreate();
    VG_MAX = eaSize(&villainGroups.villainGroups);
    for (i = 0; i < VG_MAX; i ++)
    {
        char buffer[20];
        sprintf(buffer, "%i", i);
        DefineAdd(g_pParseVillainGroups, villainGroups.villainGroups[i]->name, buffer);
        
        // initializing VG_NONE based on only criteria
        if (!stricmp("generic", villainGroups.villainGroups[i]->name)) {
            VG_NONE = i;
        }
    }

    // Note that memory will be leaked if this function is called more than once.
    // Entries that are already loaded cannot be unloaded unless we are sure all villains on the
    // mapserver is dead.
    ParserLoadFilesShared("SM_VillainDefs.bin", "Defs\\Villains", ".villain", "VillainDef.bin", flags, ParseVillainDefBegin,
        &villainDefList, sizeof(villainDefList), NULL, NULL, villainDefsPreProcessAll, NULL, villainConstructCriteriaLookupTables);

    // Validate pet powers now that we've loaded all the VillainDefs
    powerdict_ValidateEntCreate(&g_PowerDictionary);

    // To be added at a later date
    //    villainCheckRequirements();

    // Set up callbacks for reloading in dev mode
    if (!isDevelopmentMode())
        return;
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "Defs/Villains/*.villain", villainDefReload);
};



static const char* villainGetGroupName(VillainDef* def)
{
    VillainGroupEnum group;

    group = def->group;

    if(group == VG_NONE)
        return "None";


    return villainGroupGetPrintName(group);
    /*
    for(i = 1; ParseVillainGroupEnum[i].key != DM_END; i++)
    {
        if(ParseVillainGroupEnum[i].value == group)
            return ParseVillainGroupEnum[i].key;
    }
    */

    return NULL;
}

#if SERVER
//---------------------------------------------------------------------------------------------------------------
// Villain spawn counts
//---------------------------------------------------------------------------------------------------------------
static void villainSpawnCountAdd(VillainSpawnCount* count, const VillainDef* def)
{
    int i;
    int spawnlimit;

    if(!def)
        return;

    spawnlimit = OnMissionMap() ? def->spawnlimitMission : def->spawnlimit;

    if (!count || spawnlimit == -1 || spawnlimit == 0) return; // I don't care

    for (i = 0; i < count->numdefs; i++)
    {
        if (count->defs[i] == def)
        {
            count->counts[i]++;
            return;
        }
    }
    if (count->numdefs < VILLAIN_SPAWN_COUNT_LIMIT)
    {
        count->defs[count->numdefs] = def;
        count->counts[count->numdefs] = 1;
        count->numdefs++;
    }
}

// 1 if I'm allowed to add another of this villain type
static int villainSpawnCountAvail(VillainSpawnCount* count, const VillainDef* def)
{
    int i;
    int spawnlimit;

    if(!def)
        return 0;

    spawnlimit = OnMissionMap() ? def->spawnlimitMission : def->spawnlimit;

    if (!count || spawnlimit == -1) return 1; // I don't care
    if (spawnlimit == 0) return 0;

    for (i = 0; i < count->numdefs; i++)
    {
        if (count->defs[i] == def)
        {
            if (count->counts[i] < spawnlimit)
                return 1;
            else
                return 0;
        }
    }
    return 1;
}

static int villainCanSpawn(const VillainDef* def, int hunters, VillainSpawnCount* count)
{
    if (hunters)
    {
        // HACK - if we're looking for hunters, we're looking for guys with spawncount == 0
        // and with "Nictus" in their names
        if (!strstr(def->name, "Nictus")) return 0;
    }
    else
    {
        if (!villainSpawnCountAvail(count, def)) return 0;
    }
    return 1;
}
#endif

static void freeVillainGroupMembersList(VillainDefList* vlist)
{
    if (vlist->villainGroupMembersList.groups)
    {
        int i,j;
        for (i = 0; i < VG_MAX; i++)
        {
            VillainGroupMembers *group = &vlist->villainGroupMembersList.groups[i];
            for (j = 0; j < VR_MAX - 1; j++)
            {
                const VillainDef ** groupMembers = group->members[j];
                if (groupMembers)
                    eaDestroyConst(&groupMembers);
            }
        }

        free(vlist->villainGroupMembersList.groups); 
        vlist->villainGroupMembersList.groups = NULL;
    }
}

static bool createVillainGroupMembersList(VillainDefList* vlist, bool shared_memory)
{
    int villainDefCursor;
    bool ret = true;
    size_t vgMembersSize;

    assert(!vlist->villainGroupMembersList.groups);
    vgMembersSize = sizeof(VillainGroupMembers) * (VG_MAX);
    if(shared_memory)
        vlist->villainGroupMembersList.groups = customSharedMalloc(NULL, vgMembersSize);
    else
        vlist->villainGroupMembersList.groups = malloc(vgMembersSize);
    memset(vlist->villainGroupMembersList.groups, 0, vgMembersSize);
    
    // Construct the villainGroupMembersList.
    //
    for(villainDefCursor = eaSize(&vlist->villainDefs)-1; villainDefCursor >= 0; villainDefCursor--)
    {
        VillainGroupMembers* group;
        const VillainDef*** groupMembers;
        const VillainDef* vDef = vlist->villainDefs[villainDefCursor];

        if(VG_NONE == vDef->group)
        {
            Errorf("VillainDef: \"%s\" does not have a villain group\n", vDef->name);
            ret = false;
            continue;
        }

        if(VR_NONE == vDef->rank)
        {
            Errorf("VillainDef: \"%s\" does not have a villain rank\n", vDef->name);
            ret = false;
            continue;
        }

        // Get the member list for the entire villain group.
        group = &vlist->villainGroupMembersList.groups[vDef->group - 1];

        // Get the member list for the entire villain rank.
        groupMembers = &group->members[vDef->rank - 1];
        if(!*groupMembers)
        {
            eaCreateConst(groupMembers);
        }

        // Add the current villain to the correct rank in the correct villain group.
        eaPushConst(groupMembers, vDef);
    }

    // if using shared memory, compress lists created above to shared memory
    if(shared_memory)
    {
        int i, j;
        for(i = 0; i<VG_MAX; i++)
        {
            VillainGroupMembers* group = &vlist->villainGroupMembersList.groups[i];            
            for(j=1; j<VR_MAX; j++)
            {
                const VillainDef **groupMembers, **groupMembersNew;
                groupMembers = group->members[j - 1];
                if(!groupMembers)
                    continue;
                eaCompressConst(&groupMembersNew, &groupMembers, customSharedMalloc, NULL);
                eaDestroyConst(&groupMembers);
                group->members[j - 1] = groupMembersNew;
            }
        }
    }

    return ret;
}

static bool villainConstructCriteriaLookupTables(ParseTable pti[], VillainDefList* vlist, bool shared_memory)
{    
    bool ret = true;
    int i;

    // Construct a villain name to villain def table
    assert(!vlist->villainNameHash);
    vlist->villainNameHash = stashTableCreateWithStringKeys(stashOptimalSize(eaSize(&vlist->villainDefs)), stashShared(shared_memory));

    for(i = 0; i < eaSize(&vlist->villainDefs); i++)
    {
        const VillainDef* vDef = vlist->villainDefs[i];
        const VillainDef* duplicateEntry;

        if (duplicateEntry = stashFindPointerReturnPointerConst(vlist->villainNameHash, vDef->name))
        {
            Errorf("Villain def name conflict for %s in %s and %s", vDef->name, villainGroupGetName(vDef->group), villainGroupGetName(duplicateEntry->group));
            ret = false;
        }
        else
            stashAddPointerConst(vlist->villainNameHash, vDef->name, vDef, false);
    }

    if(ret)
        ret = createVillainGroupMembersList(vlist, shared_memory);

    return ret;
}

#if SERVER

#define MAX_LEVEL_CHECK 200
#define REQUIRED_NUM 3

// Returns 1 if it found any without a spawn limit
static int villainCountSpread(const VillainDef** villains, int* spread, int* min, int* max)
{
    int i, k, num = eaSize(&villains);
    int foundSome = 0;
    *min = 1000;
    *max = -1000;
    for (i = 0; i < num; i++)
    {                
        const VillainDef* def = villains[i];
        int minLevel = def->levels[0]->level;
        int maxLevel = ((VillainLevelDef*)(def->levels[eaSize(&def->levels)-1]))->level;
        int numSpawns = (def->spawnlimit == -1)?REQUIRED_NUM:def->spawnlimit;
        assert((maxLevel < MAX_LEVEL_CHECK) || !strlen("Apparently max level went over 200, so change MAX_LEVEL_CHECK"));
        for (k = minLevel; k <= maxLevel; k++)
            spread[k] += numSpawns;
        *min = MIN(*min, minLevel);
        *max = MAX(*max, maxLevel);
        if (spread[minLevel] >= REQUIRED_NUM)
            foundSome = 1;
    }
    return foundSome;
}

static void villainFlagError(int* minionSpread, int* compareSpread, int minMinions, int maxMinions, const char* vgName, const char* type, const char* filename)
{
    // Create a list of levels that don't meet the requirement
    static char* errmsg;
    int begin = 0;
    estrCreateEx(&errmsg, 100);
    estrPrintf(&errmsg, "%s must have at least %i possible %s for every level it has non-spawnlimited minions. Level(s): ", vgName, REQUIRED_NUM, type);
    for (; minMinions <= maxMinions; minMinions++)
    {
        if (!begin && minionSpread[minMinions] && compareSpread[minMinions] < REQUIRED_NUM)
        {
            begin = minMinions;
            estrConcatf(&errmsg, "%i", minMinions);
        }
        else if (begin && ((minionSpread[minMinions] && compareSpread[minMinions] >= REQUIRED_NUM) || (!minionSpread[minMinions] && compareSpread[minMinions] < REQUIRED_NUM)))
        {
            if (begin != minMinions-1)
                estrConcatf(&errmsg, "-%i, ", minMinions-1);
            else
                estrConcatStaticCharArray(&errmsg, ", ");
            begin = 0;
        }
    }
    if (begin && begin != maxMinions)
        estrConcatf(&errmsg, "-%i, ", maxMinions);
    else if (begin)
        estrConcatStaticCharArray(&errmsg, ", ");

    // Check for comma and clear it
    if (errmsg[strlen(errmsg)-2] == ',')
    {
        errmsg[strlen(errmsg)-2] = '\0';
        ErrorFilenamef(filename, errmsg);        
    }
}

void villainCheckRequirements()
{
    int i;
    if (isProductionMode())
        return;
    for (i = 1; i < VG_MAX; i++)
    {
        VillainGroupMembers* villainGroupMembers = &villainDefList.villainGroupMembersList.groups[i-1];
        const VillainDef** minions = villainGroupMembers->members[VR_MINION-1];
        const VillainDef** bosses = villainGroupMembers->members[VR_BOSS-1];
        const VillainDef** lieuts = villainGroupMembers->members[VR_LIEUTENANT-1];
        char* vgName = localizedPrintf(NULL, villainGroupGetPrintName(i));
        int minionSpread[MAX_LEVEL_CHECK] = {0};
        int bossSpread[MAX_LEVEL_CHECK] = {0};
        int lieutSpread[MAX_LEVEL_CHECK] = {0};
        int minMinions, maxMinions, min, max;
        int foundMinions = 0, foundBoss = 0, foundLieut = 0;

        // Count up all the numbers and spreads
        foundMinions = villainCountSpread(minions, minionSpread, &minMinions, &maxMinions);
        foundBoss = villainCountSpread(bosses, bossSpread, &min, &max);
        foundLieut = villainCountSpread(lieuts, lieutSpread, &min, &max);

        // Yay for Hacks, if you think of something better, change it
        if (foundMinions && foundBoss && stricmp(vgName, "Practice Robots"))
            villainFlagError(minionSpread, bossSpread, minMinions, maxMinions, vgName, "bosses", minions[0]->fileName);
        if (foundMinions && foundLieut && stricmp(vgName, "Practice Robots"))
            villainFlagError(minionSpread, lieutSpread, minMinions, maxMinions, vgName, "lieutenants", minions[0]->fileName);
    }
}

const VillainDef* villainFindByCriteria(VillainGroupEnum group, VillainRank rank, int level, int hunters, VillainSpawnCount* count, VillainExclusion exclusion)
{
    VillainGroupMembers* villainGroupMembers;
    const VillainDef** villainRankMembers;
    const VillainDef** matchlist;
    const VillainDef* def;
    int i;
    int bestSmallerLevel = -1000;
    int bestLargerLevel = 1000;
    int iSize;

    if(VG_NONE == group)
        return NULL;
    if(VR_NONE == rank)
        return NULL;

    villainGroupMembers = &villainDefList.villainGroupMembersList.groups[group - 1];
    if(!villainGroupMembers)
        return NULL;
    villainRankMembers = villainGroupMembers->members[rank - 1];
    if(!villainRankMembers)
        return NULL;

    // look through list of villains and clamp the input level to something that's valid
    iSize = eaSize(&villainRankMembers);
    for(i = 0; i < iSize; i++)
    {
        int lowestLevel;
        int highestLevel;

        def = eaGetConst(&villainRankMembers, i);

        if (exclusion & VE_MA)
        {
            if (!playerCreatedStoryarc_isValidVillainDefName(def->name, 0))
            {
                continue;
            }
        }
        else if (def->exclusion != VE_NONE && def->exclusion != (exclusion & (VE_COH | VE_COV) ) ) continue;
        if (!villainCanSpawn(def, hunters, count))
            continue;
        lowestLevel = def->levels[0]->level;
        highestLevel = (cpp_reinterpret_cast(const VillainLevelDef*)(eaGetConst(&def->levels, eaSize(&def->levels) - 1)))->level;
        // Assuming that the villain always has a contiguous block of "levels",
        if(lowestLevel <= level && level <= highestLevel)
        {
            bestSmallerLevel = bestLargerLevel = level;
            break;
        }

        // If this villain did not match, how close to the desired level is this villain?
        if(lowestLevel > level && lowestLevel < bestLargerLevel)
        {
            bestLargerLevel = lowestLevel;
        }
        else if(highestLevel < level && highestLevel > bestSmallerLevel)
        {
            bestSmallerLevel = highestLevel;
        }
    }

    // clamp to closest level we could find
    if (level - bestSmallerLevel < bestLargerLevel - level)
        level = bestSmallerLevel;
    else
        level = bestLargerLevel;

    // look through the list of villains and find all that match the clamped level
    eaCreateConst(&matchlist);
    for (i = 0; i < iSize; i++)
    {
        int lowestLevel;
        int highestLevel;

        def = eaGetConst(&villainRankMembers, i);
        if (exclusion & VE_MA)
        {
            if (!playerCreatedStoryarc_isValidVillainDefName(def->name, 0))
            {
                continue;
            }
        }
        else if (def->exclusion != VE_NONE && def->exclusion != (exclusion & (VE_COH | VE_COV) ) ) continue;
        if (!villainCanSpawn(def, hunters, count)) continue;

        lowestLevel = def->levels[0]->level;
        highestLevel = (cpp_reinterpret_cast(const VillainLevelDef*)(eaGetConst(&def->levels, eaSize(&def->levels) - 1)))->level;

        // Assuming that the villain always has a contiguous block of "levels",
        // we can determine if the villain can be of a particular level by determining
        // if the wanted level is within the level bounds of the current villain.
        if(lowestLevel <= level && level <= highestLevel)
        {
            eaPushConst(&matchlist, def);
        }
    }

    // we can't find anything - there are no villains of the given rank in this group
    if (eaSize(&matchlist) == 0)
    {
        eaDestroyConst(&matchlist);
        return NULL;
    }

    // otherwise, do a random selection
    i = rand() % eaSize(&matchlist);
    def = matchlist[i];
    eaDestroyConst(&matchlist);
    return def;
}

#endif

const VillainDef* villainFindByName(const char* villainName)
{
    return stashFindPointerReturnPointerConst(villainDefList.villainNameHash, villainName);
}

#if SERVER

// general function for handling more extensible boss difficulty reduction code
static VillainRank villainRankGetReduced(VillainRank originalRank)
{
    switch(originalRank)
    {
        case VR_ARCHVILLAIN:        //    intentional fallthrough
        case VR_ARCHVILLAIN2:
        case VR_BIGMONSTER:
            return VR_ELITE;
        case VR_ELITE:
            return VR_BOSS;
        case VR_BOSS:
            return VR_LIEUTENANT;
        default:
            return originalRank;    // not a reducible rank
    }
}

static int s_getDoppelFlags(const char *dopplename)
{
    if (dopplename)
    {
        int i = VDF_COUNT >> 1;
        int activeFlags = 0;

        while (i)
        {
            if (strstriConst(dopplename, StaticDefineIntRevLookup(ParseVillainDoppelFlags, i)))
            {
                activeFlags |= i;
            }
            i = i >> 1;
        }
        return activeFlags;
    }
    return 0;
}
static void applyDoppelFlags(Entity *e, Entity *orig, int flags)
{
    assert(e);
    if (e)
    {
        int i;
        // currently when this is called from villainCreateDoppelganger() we will already have
        // an ownedCostume for the doppel that is mutable, so there shouldn't be any work to do other than hand that back
        Costume* mutable_costume = entGetMutableCostume(e);

        if( flags & VDF_TALL )
            mutable_costume->appearance.fScale = 300;
        else if( flags & VDF_SHORT )
            mutable_costume->appearance.fScale = -60;

        if( flags & VDF_SHADOW)
        {
            setColorFromRGBA(&mutable_costume->appearance.colorSkin, 0x000000ff);
        }
        else if( flags & VDF_INVERSE )
        {
            int color = RGBAFromColor( mutable_costume->appearance.colorSkin );
            color ^= 0xffffffff;
            color |= 0xff;
            setColorFromRGBA(&mutable_costume->appearance.colorSkin, color);
        }

        if( (flags & VDF_ANGEL) || (flags & VDF_DEMON) ) 
        {
            int aura_id = costume_GlobalPartIndexFromName( "Aura" );
            int wing_id = costume_GlobalPartIndexFromName( "Cape" );


            if( (flags & VDF_DEMON) )
            {
                setColorFromRGBA( &mutable_costume->parts[aura_id]->color[0], 0xff0000ff );
                setColorFromRGBA( &mutable_costume->parts[wing_id]->color[0], 0x000000ff );
                setColorFromRGBA( &mutable_costume->parts[wing_id]->color[1], 0xff0000ff );
                setColorFromRGBA( &mutable_costume->parts[wing_id]->color[2], 0x000000ff );
                setColorFromRGBA( &mutable_costume->parts[wing_id]->color[3], 0xff0000ff );
            }
            else
            {
                setColorFromRGBA( &mutable_costume->parts[aura_id]->color[0], 0xffff00ff );
                setColorFromRGBA( &mutable_costume->parts[wing_id]->color[0], 0xffffffff );
                setColorFromRGBA( &mutable_costume->parts[wing_id]->color[1], 0xffffffff );
                setColorFromRGBA( &mutable_costume->parts[wing_id]->color[2], 0xffffffff );
                setColorFromRGBA( &mutable_costume->parts[wing_id]->color[3], 0xffffffff );
            }

            if( (flags & VDF_DEMON) )
                costume_PartSetTexture1(mutable_costume, wing_id, "!X_Wings_Demon_01");
            else
                costume_PartSetTexture1(mutable_costume, wing_id, "!X_Wings_Angel_01");

            if( e->costume->appearance.bodytype == kBodyType_Male )
            {
                if( (flags & VDF_DEMON) )
                {
                    costume_PartSetFx( mutable_costume, aura_id, "Costumes\\WinterEventHalos\\Male_FlamingHalo.fx" );
                    costume_PartSetFx( mutable_costume, wing_id, "GENERIC/fx_wings_Demon.fx" );
                }
                else
                {
                    costume_PartSetFx( mutable_costume, aura_id, "Costumes\\WinterEventHalos\\Male_Halo.fx" );
                    costume_PartSetFx( mutable_costume, wing_id, "GENERIC/FX_wings_ANGEL.fx" );
                }
            }
            if( e->costume->appearance.bodytype == kBodyType_Female )
            {
                if( (flags & VDF_DEMON) )
                {
                    costume_PartSetFx( mutable_costume, aura_id, "Costumes\\WinterEventHalos\\Female_FlamingHalo.fx" );
                    costume_PartSetFx( mutable_costume, wing_id, "GENERIC/fx_wings_Female_Demon.fx" );
                }
                else
                {
                    costume_PartSetFx( mutable_costume, aura_id, "Costumes\\WinterEventHalos\\Female_Halo.fx" );
                    costume_PartSetFx( mutable_costume, wing_id, "GENERIC/fx_wings_Female_ANGEL.fx" );
                }
            }
            if( e->costume->appearance.bodytype == kBodyType_Huge )
            {
                if( (flags & VDF_DEMON) )
                {
                    costume_PartSetFx( mutable_costume, aura_id, "Costumes\\WinterEventHalos\\Huge_FlamingHalo.fx" );
                    costume_PartSetFx( mutable_costume, wing_id, "GENERIC/fx_wings_Huge_Demon.fx" );
                }
                else
                {
                    costume_PartSetFx( mutable_costume, aura_id, "Costumes\\WinterEventHalos\\Huge_Halo.fx" );
                    costume_PartSetFx( mutable_costume, wing_id, "GENERIC/fx_wings_Huge_ANGEL.fx" );
                }
            }
        }

        for( i = eaSize(&e->costume->parts)-1; i>=0; i-- )
        {
            int j;
            CostumePart * part = mutable_costume->parts[i];

            if( flags & VDF_BLACKANDWHITE )
            {
                setColorFromRGBA(&part->color[0], 0xffffffff);
                setColorFromRGBA(&part->color[2], 0xffffffff);
                setColorFromRGBA(&part->color[1], 0x000000ff);
                setColorFromRGBA(&part->color[3], 0x000000ff);
            }

            for( j = 0; j < 4; j++)
            {
                if( flags & VDF_SHADOW )
                    setColorFromRGBA(&part->color[j], 0x000000ff);
                else if( flags & VDF_INVERSE )
                {
                    int color = RGBAFromColor( part->color[j] );
                    color ^= 0xffffffff;
                    color |= 0xff;
                    setColorFromRGBA(&part->color[j], color);
                }
            }
            if( flags & VDF_REVERSE )
            {
                SWAP32( part->color[0].integer, part->color[1].integer );
                SWAP32( part->color[3].integer, part->color[4].integer );
            }
        }

        // determine and buy powersets
        if( !(flags & VDF_NOPOWERS) )
        {
            if( flags & VDF_RANDOMPOWER )
            {
                for( i = 0; i < 2; i++ ) // give two random sets
                {
                    int count, idx = rand()%eaSize(&g_PowerSetConversionTable.ppConversions);
                    const PowerSetConversion * pConvert = g_PowerSetConversionTable.ppConversions[idx];

                    idx = rand()%eaSize(&pConvert->ppchPowerSets);
                    count = pcccritter_buyAllPowersInPowerset(e, pConvert->ppchPowerSets[idx], PCC_DIFF_HARD, 0, 0, 0, 0, 0);
                    if( !count )
                        pcccritter_buyAllPowersInPowerset(e, pConvert->ppchPowerSets[idx], PCC_DIFF_HARD, 0, -1, 0, 0, 0);
                }

                for(i = eaSize(&e->pchar->ppPowerSets)-1; i >= 0; --i)
                {
                    int j;
                    PowerSet *pset = e->pchar->ppPowerSets[i];

                    if(!pset || !pset->psetBase)
                        continue;

                    for(j = eaSize(&pset->psetBase->ppchCostumeParts)-1; j >= 0; --j)
                    {
                        CostumePart *part = costume_getTaggedPart(pset->psetBase->ppchCostumeParts[j], e->costume->appearance.bodytype);
                        int bpId = bpGetIndexFromName(SAFE_MEMBER(part,pchName));

                        if(!part || bpId < 0)
                            continue;

                        costume_PartSetAll(mutable_costume, bpId, part->pchGeom, part->pchTex1, part->pchTex2, part->color[0], part->color[1] );
                        costume_PartSetFx( mutable_costume, bpId, part->pchFxName );
                    }
                }
            }
            else
            {
                for( i = 0; i < eaSize(&orig->pchar->ppPowerSets); i++ )
                {
                    const PowerSetConversion * pConvert; 
                    if( stashFindPointerConst( g_PowerSetConversionTable.st_PowerConversion, orig->pchar->ppPowerSets[i]->psetBase->pchFullName, &pConvert ) )
                    {
                        int count, idx = rand()%eaSize(&pConvert->ppchPowerSets);
                        count = pcccritter_buyAllPowersInPowerset(e, pConvert->ppchPowerSets[idx], PCC_DIFF_HARD, 0, 0, 0, 0, 0);
                        if( !count )
                            pcccritter_buyAllPowersInPowerset(e, pConvert->ppchPowerSets[idx], PCC_DIFF_HARD, 0, -1, 0, 0, 0);
                    }
                }
            }
        }

        if( flags & VDF_GHOST )
        {
            pcccritter_buyAllPowersInPowerset(e, "Rularuu.Reflections", PCC_DIFF_EXTREME, 0, 0, 0,0, 0);
        }
    }
}

Entity * villainCreateDoppelganger(const char * dopplename, const char *overrideName, const char *overrideVillainGroupName, VillainRank villainrank, const char * villainclass,
                                   int level, bool bBossReductionMode, Mat4 pos, Entity * orig, EntityRef erCreator, const BasePower *creationPower )
{
    const VillainDef *def;
    const char *pchRankDef;
    static int last_svr_id = -1;
    int team_rotate=0;

    if(dopplename && strstriConst(dopplename,"Team"))
        team_rotate = 1;

    if( !orig )  // entity not passed in, find one
    {
        if( last_svr_id < 0 || !team_rotate ) // this is the first time requesting a doppelganger,
        {
            if( g_activemission && g_activemission->ownerDbid )
                orig = entFromDbId(g_activemission->ownerDbid);

            if( !orig ) // can't find mission owner
            {
                orig = EncounterNearestPlayerByPos(pos);
                if( !orig )
                    return 0;
            }

            if( !orig->pchar )
                return 0;

            last_svr_id = orig->id;
        }
        else
        {
            int i;
            for(i=0;i<entities_max;i++)
            {
                int actual_id = last_svr_id + i + 1;
                if( actual_id >= entities_max)
                    actual_id -= entities_max;

                if (entity_state[actual_id] & ENTITY_IN_USE  && ENTTYPE_BY_ID(actual_id) == ENTTYPE_PLAYER ) 
                {
                    orig = entities[actual_id];
                    last_svr_id = actual_id;
                    break;
                }
            }

            if( !orig ) // everyone left map?
                return 0;
        }
    }

    if( ENTTYPE(orig) == ENTTYPE_CRITTER )
    {
        return villainCreate( orig->villainDef, level, 0, bBossReductionMode, villainclass, 0, erCreator, creationPower );
    }

    if( !villainclass )
    {
        if ( bBossReductionMode && ( villainrank < 0 || villainrank == VR_BOSS ) )
            villainrank = VR_LIEUTENANT;            // This smells hacky, it should really be going through the whole reduction song and dance
        switch (villainrank)
        {
        case VR_MINION:
            pchRankDef = "CustomCritter_Minion";
            break;
        case VR_LIEUTENANT:
            pchRankDef = "CustomCritter_Lt";
            break;
        case VR_BOSS:
            pchRankDef = "CustomCritter_Boss";
            break;
        case VR_ELITE:
            pchRankDef = "CustomCritter_Elite";
            break;
        case VR_ARCHVILLAIN:
            pchRankDef = "CustomCritter_AV";
            break;
        default:
            pchRankDef = "CustomCritter_Boss";
        }
    }
    else
        pchRankDef = villainclass;

    def = villainFindByName(pchRankDef); 
    if (!def)
        return NULL;
    else
    {
        Entity *e = entCreateEx(NULL, ENTTYPE_CRITTER);
        if (e)
        {    
            const VillainLevelDef *levelDef;
            int realVillainLevel;
            bool bReduced = false;
            bool avReduced = false;
            const char * reducedClass;

            e->doppelganger = 1;

            levelDef = GetVillainLevelDef( def, level );
            realVillainLevel = levelDef->level; //If the villain wasn't defined at the level given, you got the closest we could get.

            // The level specified by the villain definitions is 1 based.
            // Why is this line after you've calculated the levelDef -- that makes no sense.  Come to think of it, this line makes no sense.
            realVillainLevel = MAX(1, realVillainLevel);

            // Now even if we got the closest we could, we still need to boost the level by the notoriety/pacing boosts
            // Only applies on within the scope of an active mission
            if (g_activemission && level > realVillainLevel)
            {
                int levelAdjust = level - MAX(g_activemission->baseLevel, realVillainLevel);
                if (levelAdjust > 0)
                    realVillainLevel = MIN(realVillainLevel + levelAdjust, MAX_VILLAIN_LEVEL);
            }

            // If its an AV thats not being reduced, it follows special notoriety level rules
            if (g_activemission && class_UsesAVStyleReduction(def->characterClassName))
            {
                if (!def->ignoreReduction && 
                    (!g_activemission->taskforceId || g_activemission->flashback || g_activemission->scalableTf) &&
                    difficultyDoAVReduction(&g_activemission->difficulty, MissionNumHeroes()))
                    avReduced = 1;
                else
                {
                    if (!g_activemission->taskforceId || g_activemission->flashback || g_activemission->scalableTf) // do not adjust level when on taskforce
                    {
                        realVillainLevel = difficultyAVLevelAdjust( &g_activemission->difficulty, realVillainLevel, MissionNumHeroes());
                        realVillainLevel = MIN(realVillainLevel, MAX_VILLAIN_LEVEL);
                    }
                }        
            }

            if( orig->pl )  // copy from slot storage to prevent kheldian forms or costume temp powers from being copied.
                entUseCopyOfCostume( e, orig->pl->costume[orig->pl->current_costume] );
            else
                entUseCopyOfConstCostume( e, orig->costume );

            e->powerCust = orig->powerCust;// = powerCustList_clone(orig->powerCust);    //    for the future

            e->villainDef = def;
            e->costumePriority = 0;
            e->ready = 1;    // Only ready entities are processed.
            e->custom_critter = 1;

            //    override villain description
            if ( overrideVillainGroupName )
            {
                if (e->CVG_overrideName) 
                    StructFreeString(e->CVG_overrideName);
                e->CVG_overrideName = StructAllocString(overrideVillainGroupName);    //    does this need to be duped?
            }
            else if( orig->supergroup )
                e->CVG_overrideName = StructAllocString(orig->supergroup->name);    //    does this need to be duped?
            else
                e->CVG_overrideName = StructAllocString(localizedPrintf(e, "DoppelgangerVillainGroup"));

            if(orig->description)
                e->description = orig->description;    // Pointer copy ok, this is allocAddString'd

            //    setting the fighting preference
            if ( rand() > RAND_MAX/2 )
                e->aiConfig = aiConfigs[AICONFIG_RANGED];
            else
                e->aiConfig = aiConfigs[AICONFIG_MELEE];

            reducedClass = class_GetReduced(def->characterClassName);
            if (avReduced && reducedClass)
            {
                npcInitClassOriginByName(e, reducedClass);
                e->villainRank = villainRankGetReduced(def->rank);
                e->dontOverrideName = 1;
            }
            else
            {
                npcInitClassOriginByName(e, def->characterClassName);
                e->villainRank = def->rank;
            }

            character_SetLevelExtern(e->pchar,realVillainLevel);

            applyDoppelFlags(e, orig, s_getDoppelFlags(dopplename));

            if (stricmp(pchRankDef, "CustomCritter_AV") == 0)
            {
                pcccritter_buyAllPowersInPowerset(e, "Mission_Maker_Special.AV_Resistances", PCC_DIFF_EXTREME, 0, 0, 0, 0, 0);
            }

            if( e->pchar )
            {
                e->pchar->ppowCreatedMe = creationPower;
                e->pchar->bIgnoreCombatMods = def->ignoreCombatMods ? true : false;
                if( !def->copyCreatorMods )
                    character_ActivateAllAutoPowers(e->pchar);
            }

            //    give critter a name
            if (overrideName)
                strcpy(e->name, overrideName);
            else
                strcpy(e->name, orig->name);

            //Set the villain on a alliance
            {
                int team = kAllyID_Evil;
                if( def->ally )
                {
                    if( 0 == stricmp( def->ally, "Hero" ) )
                        team = kAllyID_Good;
                    else if( 0 == stricmp( def->ally, "Villain" ) )
                        team = kAllyID_Foul;
                    else if( 0 == stricmp( def->ally, "Villain2" ) )
                        team = kAllyID_Villain2;
                    else if ( 0 != stricmp( def->ally, "Monster" ) )//Default
                    {
                        Errorf( "VillainCreate: %s ally group %s, doesn't exist; Using Monster\n", def->name, def->ally);
                    }
                }
                entSetOnTeam( e, team );
            }

            //Set the villain on a gang
            if( def->gang )
            {
                entSetOnGang( e, entGangIDFromGangName( def->gang ) );
            }

            LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "Create:Villain Level %d %s (%s)%s%s",
                realVillainLevel,
                def->name,
                StaticDefineIntRevLookup(ParseVillainRankEnum, def->rank),
                level==realVillainLevel ? "" : " (wanted a level %d)",
                bReduced ? " (reduced boss to lt)" : "");
            return e;
        }
        else
        {
            return NULL;
        }        
    }    
}


Entity* villainCreatePCC( char *overrideName, char *overrideVillainGroupName, PCC_Critter *pcc,
                         int level, VillainSpawnCount *count, bool bBossReductionMode, int isAlly)
{
    const VillainDef *def;
    char *pccRankDef;
    int adjustedRank;

    if (!pcc)
        return NULL;
    
    adjustedRank = pcc->rankIdx;
    if (bBossReductionMode)
    {
        if ( ( adjustedRank > PCC_RANK_LT ) && ( !isAlly ) )
        {
            //    if the notoriety is zero and the critter is a boss or higher, go down one rank
            adjustedRank--;
        }
    }
    switch (adjustedRank)
    {
    case PCC_RANK_MINION:
        pccRankDef = "CustomCritter_Minion";
        break;
    case PCC_RANK_LT:
        pccRankDef = "CustomCritter_Lt";
        break;
    case PCC_RANK_BOSS:
        pccRankDef = "CustomCritter_Boss";
        break;
    case PCC_RANK_ELITE_BOSS:
        pccRankDef = "CustomCritter_Elite";
        break;
    case PCC_RANK_ARCH_VILLAIN:
        pccRankDef = "CustomCritter_AV";
        break;
    case PCC_RANK_CONTACT:
        pccRankDef = "CustomCritter_Boss";
        break;
    default:
        pccRankDef = NULL;
    }

    def = villainFindByName(pccRankDef);
    if (!def)
    {
        return NULL;
    }
    else
    {
        Entity *e = entCreateEx(NULL, ENTTYPE_CRITTER);
        if (e)
        {    
            const VillainLevelDef *levelDef;
            int realVillainLevel, power_count=0;
            bool bReduced = false;
            int bAttackPower = 0;
            int difficultyMet[2] = {-1,-1};
            bool avReduced = false;
            F32 custom_power_points;
            const char * reducedClass;

            e->fRewardScale = 0;
            villainSpawnCountAdd(count, def);

            levelDef = GetVillainLevelDef( def, level );
            realVillainLevel = levelDef->level; //If the villain wasn't defined at the level given, you got the closest we could get.

            // The level specified by the villain definitions is 1 based.
            // Why is this line after you've calculated the levelDef -- that makes no sense.  Come to think of it, this line makes no sense.
            realVillainLevel = MAX(1, realVillainLevel);

            // Now even if we got the closest we could, we still need to boost the level by the notoriety/pacing boosts
            // Only applies on within the scope of an active mission
            if (g_activemission && level > realVillainLevel)
            {
                int levelAdjust = level - MAX(g_activemission->baseLevel, realVillainLevel);
                if (levelAdjust > 0)
                    realVillainLevel = MIN(realVillainLevel + levelAdjust, MAX_VILLAIN_LEVEL);
            }

            // If its an AV thats not being reduced, it follows special notoriety level rules
            if (g_activemission && class_UsesAVStyleReduction(def->characterClassName))
            {
                if (!def->ignoreReduction && 
                    (!g_activemission->taskforceId || g_activemission->flashback || g_activemission->scalableTf) &&
                    difficultyDoAVReduction(&g_activemission->difficulty, MissionNumHeroes()))
                    avReduced = 1;
                else
                {
                    if (!g_activemission->taskforceId || g_activemission->flashback || g_activemission->scalableTf) // do not adjust level when on taskforce
                    {
                        realVillainLevel = difficultyAVLevelAdjust( &g_activemission->difficulty, realVillainLevel, MissionNumHeroes());
                        realVillainLevel = MIN(realVillainLevel, MAX_VILLAIN_LEVEL);
                    }
                }        
            }

            entUseCopyOfCostume( e, pcc->pccCostume );
//            e->powerCust = e->ownedPowerCustomizationList = powerCustList_clone(pcc->powerCustomizationList)    //    for the future
            e->villainDef = def;
            e->costumePriority = 0;
            e->ready = 1;    // Only ready entities are processed.
            e->custom_critter = 1;
            e->pcc = pcc;

            //    override villain description
            if ( overrideVillainGroupName )
            {
                if (e->CVG_overrideName) StructFreeString(e->CVG_overrideName);
                e->CVG_overrideName = StructAllocString(overrideVillainGroupName);    //    does this need to be duped?
            }
            else if (pcc->villainGroup)
            {
                if (e->CVG_overrideName) StructFreeString(e->CVG_overrideName);
                e->CVG_overrideName = StructAllocString(pcc->villainGroup);    //    does this need to be duped?
            }

            //    setting the fighting preference
            if (pcc->isRanged)
                e->aiConfig = aiConfigs[AICONFIG_RANGED];
            else
                e->aiConfig = aiConfigs[AICONFIG_MELEE];

            reducedClass = class_GetReduced(def->characterClassName);
            if (avReduced && reducedClass)
            {
                npcInitClassOriginByName(e, reducedClass);
                e->villainRank = villainRankGetReduced(def->rank);
                e->dontOverrideName = 1;
            }
            else
            {
                npcInitClassOriginByName(e, def->characterClassName);
                e->villainRank = def->rank;
            }

            //    buy new powers
            character_SetLevelExtern(e->pchar,realVillainLevel);
            power_count += pcccritter_buyAllPowersInPowerset(e, pcc->primaryPower, pcc->difficulty[0], 0, 
                pcc->difficulty[0] == PCC_DIFF_CUSTOM ? pcc->selectedPowers[0] : 0, &bAttackPower, &difficultyMet[0], 0);

            power_count += pcccritter_buyAllPowersInPowerset(e, pcc->secondaryPower, pcc->difficulty[1], 0, 
                pcc->difficulty[1] == PCC_DIFF_CUSTOM ? pcc->selectedPowers[1] : 0, &bAttackPower, &difficultyMet[1], 0);

            if( power_count < 0 ) 
            {
                entFreeCommon(e);
                return NULL;
            }

            custom_power_points = pcccritter_tallyPowerValue(e, adjustedRank, realVillainLevel);

            if( bAttackPower ) // no attacks, no xp
            {
                if( pcc->difficulty[0] == PCC_DIFF_CUSTOM || pcc->difficulty[1] == PCC_DIFF_CUSTOM  )
                {
                    e->fRewardScale += custom_power_points;
                }
                else
                {
                    e->fRewardScale += (g_PCCRewardMods.fDifficulty[pcc->difficulty[0]]/2);
                    e->fRewardScale += (g_PCCRewardMods.fDifficulty[pcc->difficulty[1]]/2);
                }
            }

            if (stricmp(pcc->travelPower, "none") != 0)    
                pcccritter_buyAllPowersInPowerset(e, pcc->travelPower, PCC_DIFF_EXTREME, 0, 0, 0, 0, 0);

            if (stricmp(pccRankDef, "CustomCritter_AV") == 0)
            {
                pcccritter_buyAllPowersInPowerset(e, "Mission_Maker_Special.AV_Resistances", PCC_DIFF_EXTREME, 0, 0, 0, 0, 0);
            }

            if( e->pchar )
            {
                e->pchar->bIgnoreCombatMods = def->ignoreCombatMods ? true : false;
                if( !def->copyCreatorMods )
                    character_ActivateAllAutoPowers(e->pchar);
            }

            //    give critter a name
            if (overrideName)
                strcpy(e->name, overrideName);
            else if ( pcc->name)
                strcpy(e->name, pcc->name);
            else
                strcpy(e->name, "CustomCritterName");

            //Set the villain on a alliance
            {
                int team = kAllyID_Evil;
                if( def->ally )
                {
                    if( 0 == stricmp( def->ally, "Hero" ) )
                        team = kAllyID_Good;
                    else if( 0 == stricmp( def->ally, "Villain" ) )
                        team = kAllyID_Foul;
                    else if( 0 == stricmp( def->ally, "Villain2" ) )
                        team = kAllyID_Villain2;
                    else if ( 0 != stricmp( def->ally, "Monster" ) )//Default
                    {
                        Errorf( "VillainCreate: %s ally group %s, doesn't exist; Using Monster\n", def->name, def->ally);
                    }
                }
                entSetOnTeam( e, team );
            }

            //Set the villain on a gang
            if( def->gang )
            {
                entSetOnGang( e, entGangIDFromGangName( def->gang ) );
            }

            LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "Create:Villain Level %d %s (%s)%s%s",
                realVillainLevel,
                def->name,
                StaticDefineIntRevLookup(ParseVillainRankEnum, def->rank),
                level==realVillainLevel ? "" : " (wanted a level %d)",
                bReduced ? " (reduced boss to lt)" : "");
            return e;
        }
        else
        {
            return NULL;
        }        
    }    
}

Entity* villainCreateCustomNPC( CVG_ExistingVillain *ev, int level, VillainSpawnCount *count, bool bBossReductionMode, const char *classOverride )
{
    Entity *returnEnt = NULL;
    returnEnt = villainCreateByName(ev->pchName, level, count, bBossReductionMode, classOverride, 0, NULL);
    if (returnEnt)
    {
        if (ev->npcCostume)
        {
            if (!returnEnt->customNPC_costume)
            {
                returnEnt->customNPC_costume = StructAllocRaw(sizeof(CustomNPCCostume));
            }
            StructCopy(ParseCustomNPCCostume, ev->npcCostume, returnEnt->customNPC_costume, 0,0);
        }
        if (ev->displayName)
        {
            strcpy(returnEnt->name, ev->displayName);
        }
    }
    return returnEnt;
}
Entity* villainCreateByCriteria(VillainGroupEnum group, VillainRank rank, int level, VillainSpawnCount* count,
                                bool bBossReductionMode, VillainExclusion exclusion, const char *classOverride,
                                EntityRef creator, const BasePower* creationPower)
{
    const VillainDef* def = villainFindByCriteria(group, rank, level, 0, count, exclusion);
    if(!def)
        return NULL;
    return villainCreate(def, level, count, bBossReductionMode, classOverride, NULL, creator, creationPower);
}

Entity* villainCreateByName(const char* villainName, int level, VillainSpawnCount* count, bool bBossReductionMode, 
                            const char *classOverride, EntityRef creator, const BasePower* creationPower)
{
    const VillainDef* def = villainFindByName(villainName);
    if(!def)
    {
        return NULL;
    }
    return villainCreate(def, level, count, bBossReductionMode, classOverride, NULL, creator, creationPower);
}

Entity* villainCreateByNameWithASpecialCostume(const char* villainName, int level, VillainSpawnCount* count, 
                                               bool bBossReductionMode, const char *classOverride, 
                                               const char * costumeOverride, EntityRef creator, 
                                               const BasePower* creationPower)
{
    const VillainDef* def = villainFindByName(villainName);
    if(!def)
        return NULL;
    return villainCreate(def, level, count, bBossReductionMode, classOverride, costumeOverride, creator, creationPower);
}
#endif

const VillainLevelDef * GetVillainLevelDef( const VillainDef* def, int level )
{
    int i;
    int lowestLevel;
    int highestLevel;
    const VillainLevelDef* levelDef = 0;

    // Figure out which villain level to spawn.
    assertmsg(def != NULL, "GetVillainLevelDef called with NULL villiandef, will crash");
    lowestLevel = def->levels[0]->level;
    highestLevel = def->levels[eaSize(&def->levels)-1]->level;

    if(lowestLevel <= level && level <= highestLevel)
    {
        int iSize = eaSize(&def->levels);
        for(i = 0; i < iSize; i++)
        {
            levelDef = def->levels[i];
            if(levelDef->level == level)
                break;
        }
    }
    else if(lowestLevel >= level)
    {
        levelDef = def->levels[0];

        //Errorf( "VillainCreate: %s has no level %d, using %d", def->name, level, lowestLevel);
    }
    else if(highestLevel <= level)
    {
        levelDef = def->levels[eaSize(&def->levels)-1];

        //Errorf( "VillainCreate: %s has no level %d, using %d\n", def->name, level, highestLevel);
    }
    return levelDef;
}

const char* GetNameofVillainTarget(const VillainDef* def, int levelNumber)
{
    if (def->levels && def->levels[levelNumber] && def->levels[levelNumber]->displayNames)
    {
        //0 was used to obtain the display name because it is assumed that
        //villain pets will only have one display name associated
        //with it.  If a pet has more than one display name, this function
        //will always grab the first display name
        return def->levels[levelNumber]->displayNames[0];
    }
    else
    {
        return NULL;
    }
}

#if SERVER
int villainAE_isAVorHigher(VillainRank villainRank)
{
    switch(villainRank)
    {
    case VR_ARCHVILLAIN:        //    intentional fallthrough
    case VR_ARCHVILLAIN2:
    case VR_BIGMONSTER:
        return 1;
    default:
        return 0;
    }
}

bool villainCreateInPlace(Entity *e, const VillainDef* def, int level, VillainSpawnCount* count, bool bBossReductionMode, const char *classOverride, const char * costumeOverride, EntityRef creator, const BasePower* creationPower)
{
    const VillainLevelDef* levelDef;
    int realVillainLevel;
    const NPCDef* npcDef;
    int npcIndex;
    int costumeNameIndex;
    bool bReduced = false;
    bool avReduced = false;
    const char * costumeName;
    const char * reducedClass;
    
    villainSpawnCountAdd(count, def);

    levelDef = GetVillainLevelDef( def, level );
    realVillainLevel = levelDef->level; //If the villain wasn't defined at the level given, you got the closest we could get.

    // The level specified by the villain definitions is 1 based.
    // Why is this line after you've calculated the levelDef -- that makes no sense.  Come to think of it, this line makes no sense.
    realVillainLevel = MAX(1, realVillainLevel);

    // Now even if we got the closest we could, we still need to boost the level by the notoriety/pacing boosts
    // Only applies on within the scope of an active mission
    if (g_activemission && level > realVillainLevel) // do not adjust level when on taskforce)
    {
        int levelAdjust = level - MAX(g_activemission->baseLevel, realVillainLevel);
        if (levelAdjust > 0)
            realVillainLevel = MIN(realVillainLevel + levelAdjust, MAX_VILLAIN_LEVEL);
    }

    // If its an AV thats not being reduced, it follows special notoriety level rules
    if (g_activemission && class_UsesAVStyleReduction(def->characterClassName))
    {
        if (!def->ignoreReduction && 
            (!g_activemission->taskforceId || g_activemission->flashback || g_activemission->scalableTf) &&
            difficultyDoAVReduction(&g_activemission->difficulty, MissionNumHeroes()))
            avReduced = 1;
        else
        {
            if (!g_activemission->taskforceId || g_activemission->flashback || g_activemission->scalableTf) // do not adjust level when on taskforce
            {
                realVillainLevel = difficultyAVLevelAdjust( &g_activemission->difficulty, realVillainLevel, MissionNumHeroes());
                realVillainLevel = MIN(realVillainLevel, MAX_VILLAIN_LEVEL);
            }
        }        
    }

    // Assign the NPC a random costume.`
    //    Get a random "costume name" from the level definition.
    costumeNameIndex = randInt(eaSize(&levelDef->costumes));

    if( costumeOverride )
        costumeName = costumeOverride;
    else
        costumeName = levelDef->costumes[costumeNameIndex];

    //    The NPC definition is doubling a costume definition here.
    //    Get the first costume associated with the npc.
    npcDef = npcFindByName( costumeName, &npcIndex );
    if (!npcDef)
    {
        Errorf("Failed finding costume %s for %s\n",
            levelDef->costumes[costumeNameIndex], def->name);
        return false;
    }

    if(npcDef->pLocalOverride)
    {
        // Create the NPC with the correct body.
        svrChangeBody(e, npcDef->pLocalOverride->appearance.entTypeFile);
        entSetImmutableCostumeUnowned( e, npcDef->pLocalOverride ); // e->costume = npcDef->pLocalOverride;
    }
    if(npcDef->costumes)
    {
        // Create the NPC with the correct body.
        svrChangeBody(e, npcDef->costumes[0]->appearance.entTypeFile);
        entSetImmutableCostumeUnowned( e, npcDef->costumes[0] ); // e->costume = npcDef->costumes[0];
    }
    else
    {
        svrChangeBody(e, levelDef->costumes[costumeNameIndex]);
        entSetImmutableCostumeUnowned( e, NULL ); // e->costume = NULL;
    }

    e->npcIndex = npcIndex;
    e->costumeIndex = 0;
    e->villainDef = def;
    e->costumeNameIndex = costumeNameIndex;
    e->costumePriority = 0;
    e->description = NULL;
    e->fRewardScale = 1.f; // used to penalize critters as they spawn

    //Set Permanent AnimList bits on Villain
    if( def->favoriteWeapon )
    {
        alSetFavoriteWeaponAnimListOnEntity( e, def->favoriteWeapon );
    }

    e->ready = 1; // Only ready entities are processed.

    // Boss Reduction now checks the class for class to reduce to and whether to use the Boss or AV reduction flag
    // This was to support reduction of Praetorian endgame classes
    // What Rank (rewards, conning) to reduce to still mostly hardcoded
    reducedClass = class_GetReduced(def->characterClassName);
    if (!def->ignoreReduction && bBossReductionMode && classOverride == NULL && !avReduced &&
        reducedClass )
    {
        npcInitClassOriginByName(e, reducedClass);
        e->villainRank = villainRankGetReduced(def->rank);
        bReduced = true;
    }
    else if (avReduced && reducedClass)
    {
        npcInitClassOriginByName(e, reducedClass);
        e->villainRank = villainRankGetReduced(def->rank);
        e->dontOverrideName = 1;
    }
    else
    {
        if (classOverride != NULL)
            npcInitClassOriginByName(e, classOverride);
        else
            npcInitClassOriginByName(e, def->characterClassName);
        e->villainRank = def->rank;
    }

    if (e->pchar ) 
    {
        if( g_ArchitectTaskForce || (g_activemission && g_activemission->sahandle.bPlayerCreated) )
        {
            realVillainLevel =    MIN(realVillainLevel, level);
        }
        character_SetLevelExtern(e->pchar, realVillainLevel );
    }

    if (!npcInitPowers(e, def->powers, 0))
    {
        Errorf("Failed adding powers for %s in %s\n",def->name,def->fileName);
    }

    e->erCreator = creator;

    if( e->pchar )
    {
        e->pchar->ppowCreatedMe = creationPower;
        e->pchar->bIgnoreCombatMods = def->ignoreCombatMods ? true : false;
        if( !def->copyCreatorMods )
            character_ActivateAllAutoPowers(e->pchar);
    }

    // Assign the villain a name.
    if(eaSize(&levelDef->displayNames))
    {
        int randRoll;
        randRoll = rand() % eaSize(&levelDef->displayNames);
        strcpy( e->name, localizedPrintf(e,levelDef->displayNames[randRoll]) );
    }

    //Set the villain on a alliance
    {
        int team = kAllyID_Evil;
        if( def->ally )
        {
            if( 0 == stricmp( def->ally, "Hero" ) )
                team = kAllyID_Good;
            else if( 0 == stricmp( def->ally, "Villain" ) )
                team = kAllyID_Foul;
            else if( 0 == stricmp( def->ally, "Villain2" ) )
                team = kAllyID_Villain2;
            else if ( 0 != stricmp( def->ally, "Monster" ) )//Default
            {
                Errorf( "VillainCreate: %s ally group %s, doesn't exist; Using Monster\n", def->name, def->ally);
            }
        }
        entSetOnTeam( e, team );
    }

    //Set the villain on a gang
    if( def->gang )
    {
        entSetOnGang( e, entGangIDFromGangName( def->gang ) );
    }

    //Start any Entity scripts attached to this villain def
    if(def->scripts)
    {
        EntityScriptsBegin(e, def->scripts, eaSize(&def->scripts), NULL, def->fileName);
    }

    LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "Create:Villain Level %d %s (%s)%s%s",
        realVillainLevel,
        def->name,
        StaticDefineIntRevLookup(ParseVillainRankEnum, def->rank),
        level==realVillainLevel ? "" : " (wanted a level %d)",
        bReduced ? " (reduced boss to lt)" : "");

    return true;
}

// Reverts the villain to the costume in which it was spawned
void villainRevertCostume(Entity *e)
{
    if (e->doppelganger && e->ownedCostume)
    {
        // Switch to the NPC with the correct body
        svrChangeBody(e, e->ownedCostume->appearance.entTypeFile);
        e->costume = costume_as_const(e->ownedCostume);
        e->npcIndex = 0;
        e->costumePriority = 0;
        e->send_costume = true;
        e->send_all_costume = true;
    }
    else
    {
        int level = e->pchar->iLevel + 1;
        const VillainDef *def = e->villainDef;
        const VillainLevelDef *levelDef = GetVillainLevelDef( def, level );
        
        if(e->costumeNameIndex < eaSize(&levelDef->costumes))
        {
            int npcIndex;
            const NPCDef* npcDef = npcFindByName(levelDef->costumes[e->costumeNameIndex], &npcIndex);
            if (npcDef && npcDef->costumes && e->npcIndex!=npcIndex)
            {
                // Switch to the NPC with the correct body
                svrChangeBody(e, npcDef->costumes[0]->appearance.entTypeFile);
                entSetImmutableCostumeUnowned( e, npcDef->costumes[0] ); // e->costume = npcDef->costumes[0];
                e->npcIndex = npcIndex;
                e->costumePriority = 0;
                e->send_costume = true;
                e->send_all_costume = true;
            }
        }
    }
}

Entity* villainCreate(const VillainDef* def, int level, VillainSpawnCount* count, bool bBossReductionMode, const char *classOverride, const char * costumeOverride, EntityRef creator, const BasePower* creationPower)
{
    Entity *e = entCreateEx(NULL, ENTTYPE_CRITTER);

    if(e)
    {
        if(!villainCreateInPlace(e, def, level, count, bBossReductionMode, classOverride, costumeOverride, creator, creationPower))
        {
            entFree(e);
            e = NULL;
        }
    }

    return e;
}

Entity* villainCreateNictusHunter(VillainGroupEnum group, VillainRank rank, int level, bool bBossReductionMode)
{
    // look first for a nictus hunter in given villain group
    // otherwise, get from Nictus villain group
    const VillainDef* def = NULL;

    // if I'm not forcing Nictus group, try correct group
    if (rule30FloatPct() >= g_nictusOptions.chanceNictusGroup)
        def = villainFindByCriteria(group, rank, level, 1, NULL, VE_NONE);
    if(!def)
        def = villainFindByCriteria(villainGroupGetEnum(VG_NICTUS), rank, level, 0, NULL, VE_NONE);
    if (!def)
        return NULL;
    return villainCreate(def, level, NULL, bBossReductionMode, NULL, NULL, 0, NULL);
}
#endif
//---------------------------------------------------------------------------------------------------------------
// Villain utility functions
//---------------------------------------------------------------------------------------------------------------


const char* villainGroupGetName(VillainGroupEnum group)
{
    if (group >= 0 && group < VG_MAX) {
        return villainGroups.villainGroups[group]->name;
    } else {
        return NULL;
    }

}

// returns static buffer
const char* villainGroupGetPrintName(VillainGroupEnum group)
{

    if (group >= 0 && group < VG_MAX) {
        return villainGroups.villainGroups[group]->displayName;
    } else {
        return NULL;
    }

}



// returns static buffer
const char* villainGroupGetPrintNameSingular(VillainGroupEnum group)
{

    if (group >= 0 && group < VG_MAX) {
        return villainGroups.villainGroups[group]->displaySingluarName;
    } else {
        return NULL;
    }
}

const char* villainGroupGetDescription(VillainGroupEnum group)
{

    if (group >= 0 && group < VG_MAX) {
        return villainGroups.villainGroups[group]->description;
    } else {
        return NULL;
    }
}


VillainGroupEnum villainGroupGetEnum(const char* name)
{
    int i;
    for (i = 0; i < VG_MAX; i ++)
    {
        if(stricmp(villainGroups.villainGroups[i]->name, name) == 0)
            return i;
    }
    /*
    StaticDefineInt* define;
    for(define = &ParseVillainGroupEnum[1]; define->key; define++)
    {
        if(stricmp(define->key, name) == 0)
            return define->value;
    }
    */
    return VG_NONE;
}

int villainRankConningAdjustment(VillainRank rank)
{
    if (rank <= VR_NONE || rank >= VR_MAX) return 0;
    return villainRankConningAdjust[rank];
}

int villainGroupShowInKiosk(VillainGroupEnum group)
{
    return villainGroups.villainGroups[group]->showInKiosk;
}

VillainGroupAlly villainGroupGetAlly(VillainGroupEnum group)
{
    return villainGroups.villainGroups[group]->groupAlly;
}

bool villainDefHasTag(const VillainDef* def, const char* powerTag)
{
    int i, n;

    if(!def || !powerTag)
        return false;

    n = eaSize(&def->powerTags);
    for(i=0; i<n; i++)
    {
        if(stricmp(def->powerTags[i], powerTag)==0)
            return true;
    }

    return false;
}

#if SERVER

const char** villainGroupGetListFromRank(VillainGroupEnum group, VillainRank rank, int minLevel, int maxLevel)
{
    static const char** villainList = NULL;
    VillainGroupMembers* villainGroupMembers = &villainDefList.villainGroupMembersList.groups[group - 1];
    const VillainDef** villainRankMembers = villainGroupMembers->members[rank - 1];
    int i, n = eaSize(&villainRankMembers);

    if (!villainList)
        eaCreateConst(&villainList);
    eaSetSizeConst(&villainList, 0);
    for (i = 0; i < n; i++)
    {
        int ok = 1;
        if (minLevel || maxLevel)
        {
            const VillainLevelDef* realMinLevel = GetVillainLevelDef(villainRankMembers[i], minLevel);
            const VillainLevelDef* realMaxLevel = GetVillainLevelDef(villainRankMembers[i], maxLevel);
            if (realMinLevel && (realMinLevel->level != minLevel))
                ok = 0;
            else if (realMaxLevel && (realMaxLevel->level != maxLevel))
                ok = 0;
        }
        if (ok)
            eaPushConst(&villainList, villainRankMembers[i]->name);
    }

    return villainList;
}
// returns static buffer
const char* villainGroupNameFromEnt(Entity* villain)
{
    if (!villain || !villain->villainDef)
        return NULL;
    if (villain->CVG_overrideName)
        return villain->CVG_overrideName;
    if (villain->villainDef->groupdescription)
        return villain->villainDef->groupdescription;
    return villainGroupGetPrintName(villain->villainDef->group);
}


void editNpcCostume(  Entity *e, const char * npcdef_name )
{
    int npcIndex;
    const NPCDef* npcDef = npcFindByName(npcdef_name, &npcIndex);

    if( !npcDef )
    {
        conPrintf( e->client, "NPC def %s not found!", npcdef_name );
        return;
    }

    START_PACKET(pak, e, SERVER_EDIT_VILLAIN_COSTUME);
        pktSendBits(pak,32,npcIndex);
    END_PACKET;
}

void editVillainCostume( Entity *e, const char * villaindef_name, int level )
{
    const VillainDef* def = villainFindByName(villaindef_name);
    const VillainLevelDef* levelDef;

    if( !def )
    {
        conPrintf( e->client, "Villain def %s not found!", villaindef_name );
        return;
    }

    levelDef = GetVillainLevelDef( def, level );
    editNpcCostume(e, levelDef->costumes[0]);
}

#endif

/* End of File */
