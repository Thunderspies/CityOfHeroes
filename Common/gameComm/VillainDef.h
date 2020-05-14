#ifndef VILLAINDEF_H
#define VILLAINDEF_H

#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/structdefines.h>

#include "entity/EntityRef.h"

typedef struct Costume Costume;
typedef struct Entity Entity;
typedef struct Actor Actor;
typedef struct PCC_Critter PCC_Critter;
typedef struct CVG_ExistingVillain CVG_ExistingVillain;
typedef struct ScriptDef ScriptDef;

// Fear me!
#define ParseVillainGroupEnum ((StaticDefineInt *) ParseVillainGroups)
#define DOPPEL_GROUP_NAME    "Doppels"
#define DOPPEL_NAME            "DDDoppelganger"

extern StaticDefine ParseVillainGroups[];
typedef int VillainGroupEnum;

// replacements for commonly used values under the old villain group enum system
extern VillainGroupEnum VG_NONE;
extern VillainGroupEnum VG_MAX;

typedef enum VillainGroupAlly
{
    VG_ALLY_NONE,
    VG_ALLY_HERO,
    VG_ALLY_VILLAIN,
    VG_ALLY_MONSTER,
} VillainGroupAlly;

typedef struct VillainGroup {
    const char                *name;                        // internal name
    const char                *displayName;                // display name (translated)
    const char                *displaySingluarName;        // singluar version of display name (translated)
    const char                *displayLeaderName;            // leader's name (translated)
    const char                *description;                // Villain group description
    int                    showInKiosk;                // 1 if shown in kiosk stats, 0 otherwise
    VillainGroupAlly    groupAlly;                    // Which ally is this group is
} VillainGroup;

typedef enum VillainRank
{
    VR_NONE,            // (conning level)
    VR_SMALL,            // -1
    VR_MINION,            // 0
    VR_LIEUTENANT,        // 1
    VR_SNIPER,            // 1
    VR_BOSS,            // 2
    VR_ELITE,            // 3 - Elite Boss Rank
    VR_ARCHVILLAIN,        // 5
    VR_ARCHVILLAIN2,    // 5
    VR_BIGMONSTER,        // 100
    VR_PET,                // 1
    VR_DESTRUCTIBLE,    // 1
    VR_MAX,
} VillainRank;
extern StaticDefineInt ParseVillainRankEnum[];

typedef enum VillainExclusion
{
    VE_NONE = 0,            // Allow in all games
    VE_COH = 1,                // Allow in CoH Only
    VE_COV = 1 << 1,        // Allow in CoV Only
    VE_MA = 1 << 2,
} VillainExclusion;
extern StaticDefineInt ParseVillainExclusion[];

typedef enum VillainDoppelFlags
{
    VDF_TALL = 1,
    VDF_SHORT = 1 << 1,
    VDF_SHADOW = 1 << 2,
    VDF_INVERSE = 1 << 3,
    VDF_REVERSE = 1 << 4,
    VDF_BLACKANDWHITE = 1 << 5,
    VDF_DEMON = 1 << 6,
    VDF_ANGEL = 1 << 7,
    VDF_PIRATE = 1 << 8,
    VDF_RANDOMPOWER = 1 << 9,
    VDF_GHOST = 1 << 10,
    VDF_NOPOWERS = 1 << 11,
    VDF_COUNT = 1 << 12, // this isn't really a count...
}VillainDoppelFlags;
extern StaticDefineInt ParseVillainDoppelFlags[];

typedef struct VillainLevelDef
{
    int level;                    // What is the villain level is this definition for?
    int experience;                // How much experience do I reward the player when defeated?
    const char** displayNames;    // What are the possible display names for the villain at this level?
    const char** costumes;        // Costume names, which references entries in NPC definitions.
}VillainLevelDef;

typedef struct PowerNameRef PowerNameRef;

typedef struct PetCommandStrings
{
    const char **ppchPassive;
    const char **ppchDefensive;
    const char **ppchAggressive;

    const char **ppchAttackTarget;
    const char **ppchAttackNoTarget;
    const char **ppchStayHere;
    const char **ppchUsePower;
    const char **ppchUsePowerNone;
    const char **ppchFollowMe;
    const char **ppchGotoSpot;
    const char **ppchDismiss;
} PetCommandStrings;

// for VillainDef flags
#define VILLAINDEF_NOGROUPBADGESTAT                    (1 << 0) // Don't count a badgestat for the villain group when defeated
#define VILLAINDEF_NORANKBADGESTAT                    (1 << 2) // Don't count a badgestat for the villain rank when defeated
#define VILLAINDEF_NONAMEBADGESTAT                    (1 << 3) // Don't count a badgestat for the villain name when defeated
#define VILLAINDEF_NOGENERICBADGESTAT                (VILLAINDEF_NOGROUPBADGESTAT | VILLAINDEF_NORANKBADGESTAT | VILLAINDEF_NONAMEBADGESTAT)

typedef struct VillainDef
{
    const char*            name;                    // Internal name.  NPCs should be referenced by this name.
    int                    gender;
    const char*            characterClassName;        // What kind of stats should I have?
    const char*            description;            // villain description
    const char*            groupdescription;        // (OPTIONAL) an override for the villain group description
    const char*            displayClassName;        // (OPTIONAL) an override for the class display name
    const char*            aiConfig;                // What AIConfig should I use?
    const char**        powerTags;                // (OPTIONAL) Tags which can be checked in powers for special effects.
    const char**        additionalRewards;        // (OPTIONAL) In addition to the class reward, what other rewards do I give?
    const char**        skillHPRewards;            // (OPTIONAL) For skill objects destroyed via HP, what rewards do I give?
    const char**        skillStatusRewards;        // (OPTIONAL) For skill objects destroyed via Status, what rewards do I give?
    const char *        ally;                    // (OPTIONAL) Am I a hero, villain or monster? (default is monster)
    const char *        gang;                    // (OPTIONAL) What gang am I on; any string will do
    float                rewardScale;            // (OPTIONAL) Scale XP and Inf given out by this value.
    VillainExclusion    exclusion;                // (OPTIONAL) Villain should only appear in this version of the game
    const char *        favoriteWeapon;            // (OPTIONAL) Weapon to use in encounter inactive state when you carry a weapon (linked to file Animation maintains)
    bool                ignoreCombatMods;        // if 1, this villain's attacks and defenses will ignore combat mods
    bool                copyCreatorMods;        // if 1, when spawned as a pet this villain gets a copy of its creator's attrib mods
    int                    spawnlimit;                // the maximum number of this villain that will be put in one spawn in non-mission maps (-1 == no limit)
    int                    spawnlimitMission;        // the maximum number of this villain that will be put in one spawn in mission maps (-1 == no limit, defaults to spawnlimit if unspecified)
    bool                ignoreReduction;        // if 1, this villain will not be reduced from an AV to an EB
    bool                canZone;                // if 1, this can follow its creater across zone boundaries
    const char *        customBadgeStat;        // (OPTIONAL) increment this badgestat on a kill of this villain
    U32                    flags;

    VillainRank            rank;                    // What is my rank?
    VillainGroupEnum    group;                    // Which villain group do I belong to?
    const PowerNameRef**    powers;                // What are my possible powers?
    const VillainLevelDef**    levels;

    const char *        specialPetPower;        // If I am a pet, what power, if any, do I wait for my master to tell me to use? (power must also be in my power list)
    PetCommandStrings**    petCommandStrings;        // For pets, reponses for when they are commanded.
    int                    petVisibility;            // For pets, to display in pet window
    int                    petCommadability;        // For pets, can they be commanded?

    const char *        fileName;                // For debugging
    U32                    fileAge;                // File modification time 
    U32                    processAge;                // Entry process time

#ifdef SERVER
    const ScriptDef        **scripts;
#endif
}VillainDef;

typedef struct
{
    const VillainDef** members[VR_MAX-1];
} VillainGroupMembers;

typedef struct
{
    VillainGroupMembers *groups;
} VillainGroupMembersList;

typedef struct{
    const VillainDef** villainDefs;
    cStashTable villainNameHash;
    VillainGroupMembersList villainGroupMembersList;
} VillainDefList;

extern SHARED_MEMORY VillainDefList villainDefList;

// does NOT keep track of every villain spawned, only those we care about (have SpawnLimit/SpawnLimitMission)
#define VILLAIN_SPAWN_COUNT_LIMIT 20            // shouldn't reasonably be more than 3 or 4 special villain types in a spawn
typedef struct VillainSpawnCount
{
    int numdefs;
    const VillainDef* defs[VILLAIN_SPAWN_COUNT_LIMIT];    // special defs we've seen
    int counts[VILLAIN_SPAWN_COUNT_LIMIT];                // how many villains spawned for this def
} VillainSpawnCount;


typedef struct PowerSetConversion
{
    const char * pchPlayerPowerSet;
    const char **ppchPowerSets;
}PowerSetConversion;

typedef struct PowerSetConversionTable
{
    const PowerSetConversion **ppConversions;
    cStashTable st_PowerConversion;
}PowerSetConversionTable;

extern SHARED_MEMORY PowerSetConversionTable g_PowerSetConversionTable;
//---------------------------------------------------------------------------------------------------------------
// Villain definition parsing
//---------------------------------------------------------------------------------------------------------------
void villainReadDefFiles();
Costume* villainDefsGetCostume(int npcIndex, int costumeIndex);

//---------------------------------------------------------------------------------------------------------------
// Villain creation/initialization
//---------------------------------------------------------------------------------------------------------------

// count parameter is always optional in following functions, used to limit # of villains in a single spawndef
const VillainDef* villainFindByCriteria(VillainGroupEnum group, VillainRank rank, int level, int hunters, VillainSpawnCount* count, VillainExclusion exclusion);
const VillainDef* villainFindByName(const char* villainName);
Entity* villainCreatePCC( char *displayName, char *overrideVGName, PCC_Critter *pcc, int level, VillainSpawnCount *count, bool bBossReductionMode, int isAlly);
Entity* villainCreateCustomNPC( CVG_ExistingVillain *ev, int level, VillainSpawnCount *count, bool bBossReductionMode, const char *classOverride );
Entity* villainCreateByCriteria(VillainGroupEnum group, VillainRank rank, int level, VillainSpawnCount* count,
                                bool bBossReductionMode, VillainExclusion exclusion, const char *classOverride, 
                                EntityRef creator, const struct BasePower* creationPower);
Entity* villainCreateByName(const char* villainName, int level, VillainSpawnCount* count, bool bBossReductionMode, const char *classOverride, EntityRef creator, const struct BasePower* creationPower);
Entity* villainCreateByNameWithASpecialCostume(const char* villainName, int level, VillainSpawnCount* count, bool bBossReductionMode, const char *classOverride, const char * costume, EntityRef creator, const struct BasePower* creationPower);
Entity* villainCreate(const VillainDef* def, int level, VillainSpawnCount* count, bool bBossReductionMode, const char *classOverride, const char * costumeOverride, EntityRef creator, const struct BasePower* creationPower);
bool villainCreateInPlace(Entity *e, const VillainDef* def, int level, VillainSpawnCount* count, bool bBossReductionMode, const char *classOverride, const char * costumeOverride, EntityRef creator, const struct BasePower* creationPower);
void villainRevertCostume(Entity *e);
Entity* villainCreateNictusHunter(VillainGroupEnum group, VillainRank rank, int level, bool bBossReductionMode);
Entity * villainCreateDoppelganger(const char *dopple, const char *overrideName, const char *overrideVillainGroupName, VillainRank villainrank, const char * villainclass,
                                   int level, bool bBossReductionMode, Mat4 pos, Entity *e, EntityRef erCreator, struct BasePower const *basepow );
//---------------------------------------------------------------------------------------------------------------
// Villain utility functions
//---------------------------------------------------------------------------------------------------------------
const char* villainGroupGetName(VillainGroupEnum group);                // returns static buffer
const char* villainGroupGetPrintName(VillainGroupEnum group);            // returns static buffer
const char* villainGroupGetPrintNameSingular(VillainGroupEnum group);    // returns static buffer
const char* villainGroupGetDescription(VillainGroupEnum group);    // returns static buffer
const char* villainGroupNameFromEnt(Entity* villain);            // uses group display override if possible, returns static buffer
VillainGroupEnum villainGroupGetEnum(const char* name);
int villainGroupShowInKiosk(VillainGroupEnum group);    // should this villain group show up in the kiosk?
int villainRankConningAdjustment(VillainRank rank);
const VillainLevelDef * GetVillainLevelDef( const VillainDef* def, int level );
const char* GetNameofVillainTarget(const VillainDef* def, int levelNumber);
VillainGroupAlly villainGroupGetAlly(VillainGroupEnum group);
const char** villainGroupGetListFromRank(VillainGroupEnum group, VillainRank rank, int minLevel, int maxLevel);    // returns static EArray
int villainAE_isAVorHigher(VillainRank villainRank);
bool villainDefHasTag(const VillainDef* def, const char* powerTag);
//---------------------------------------------------------------------------------------------------------------
// client-side villain editng commands
//---------------------------------------------------------------------------------------------------------------
void editNpcCostume(  Entity *e, const char * npcdef_name );
void editVillainCostume( Entity *e, const char * villaindef_name, int level );

#endif
