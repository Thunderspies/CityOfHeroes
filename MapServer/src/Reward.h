/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef REWARD_H__
#define REWARD_H__

#include <utilitieslib/components/earray.h>
#include "gameComm/Npc.h"        // For PowerNameRef and ParsePowerNameRef
#include <utilitieslib/utils/textparser.h> // For TokenizerParseInfo
#include "entity/RewardItemType.h"
#include "entity/incarnate_server.h"

typedef struct Entity Entity;
typedef struct EntPlayer EntPlayer;
typedef struct RewardLevel RewardLevel;
typedef struct ScriptVarsTable ScriptVarsTable;

typedef enum RewardSource
{
    REWARDSOURCE_UNDEFINED,
    REWARDSOURCE_DROP,
    REWARDSOURCE_ENCOUNTER,
    REWARDSOURCE_STORY,
    REWARDSOURCE_TASK,
    REWARDSOURCE_MISSION,
    REWARDSOURCE_EVENT,
    REWARDSOURCE_ARENA,
    REWARDSOURCE_SCRIPT,
    REWARDSOURCE_PNPC_AUTO,
    REWARDSOURCE_NPC_ON_CLICK,
    REWARDSOURCE_RECIPE,
    REWARDSOURCE_INVENTION_RECIPE,
    REWARDSOURCE_BADGE,
    REWARDSOURCE_BADGE_BUTTON,
    REWARDSOURCE_ATTRIBMOD,
    REWARDSOURCE_CSR,
    REWARDSOURCE_DEVCHOICE,
    REWARDSOURCE_TFWEEKLY,
    REWARDSOURCE_SALVAGE_OPEN,
    REWARDSOURCE_CHOICE,
    REWARDSOURCE_DEBUG,
    REWARDSOURCE_COUNT,
} RewardSource;
extern const char* gRewardSourceLabels[REWARDSOURCE_COUNT];

typedef enum
{
    RSF_ALWAYS        = (1 << 0),
    RSF_EVERYONE    = (1 << 1), // Give to everyone, not just the lucky team and person.
    RSF_NOLIMIT        = (1 << 2), // Ignore inventory limits (currently only meaningful on salvage and recipes) -Garth 1/3/7
    RSF_FORCED        = (1 << 3), // Always granted, even if exemplared or side-kicked or not the proper level.
} RewardSetFlags;

//------------------------------------------------------------------------
// RewardAccumulator
//------------------------------------------------------------------------

/* RewardAccumulatedPower
 */
typedef struct RewardAccumulatedPower
{
     const PowerNameRef* powerNameRef;
     int level;
     RewardSetFlags set_flags;
} RewardAccumulatedPower;


//forward decl
typedef struct SalvageItem SalvageItem;

/* RewardAccumulatedSalvage
 */
typedef struct RewardAccumulatedSalvage
{
    const SalvageItem *item;
    RewardSetFlags set_flags;
    int count;
} RewardAccumulatedSalvage;

//forward decl
typedef struct ConceptItem ConceptItem;
typedef struct ConceptDef ConceptDef;

/* RewardAccumulatedConcept
 */
typedef struct RewardAccumulatedConcept
{
    const ConceptDef *item;
    RewardSetFlags set_flags;
} RewardAccumulatedConcept;

//forward decl
typedef struct ProficiencyItem ProficiencyItem;
typedef struct DetailRecipe DetailRecipe;
typedef struct Detail Detail;


/* RewardAccumulatedProficiency
 */
typedef struct RewardAccumulatedProficiency
{
    ProficiencyItem const *item;
    RewardSetFlags set_flags;
} RewardAccumulatedProficiency;

/* RewardAccumulatedDetailRecipe
 */
typedef struct RewardAccumulatedDetailRecipe
{
    DetailRecipe const *item;
    RewardSetFlags set_flags;
    int count;
    bool unlimited;
} RewardAccumulatedDetailRecipe;

/* RewardAccumulatedDetail
 */
typedef struct RewardAccumulatedDetail
{
    Detail const *item;
    RewardSetFlags set_flags;
} RewardAccumulatedDetail;

typedef struct RewardAccumulatedToken
{
    const char *tokenName;
    int remove;
} RewardAccumulatedToken;

/* RewardAccumulatedRewardTokenCount
*/
typedef struct RewardAccumulatedRewardTokenCount
{
    const char *tokenName;
    int count;
} RewardAccumulatedRewardTokenCount;

/* RewardAccumulatedRewardTable
 */
typedef struct RewardAccumulatedRewardTable
{
    const char *tableName;
    RewardSetFlags set_flags;
    int level;
    int rewardTableFlags;
} RewardAccumulatedRewardTable;


typedef struct RewardAccumulatedIncarnatePoints
{
    int count;
    int subtype;
} RewardAccumulatedIncarnatePoints;

typedef struct RewardAccumulatedAccountProduct
{
    char productCode[9]; // Sku_ids are i64s...
    int count;
    int rarity;
} RewardAccumulatedAccountProduct;

/* RewardAccumulator
 *  All rewards that should be rewarded to a player is accumulated in such
 *  a structure when the xxxAward functions are called.
 * see rewardaccumulator_Init/Cleanup when adding fields to this
 */
typedef struct RewardAccumulator
{
    int experience;
    int wisdom;
    int influence;
    int prestige;
    F32 bonus_experience;
    bool bIgnoreRewardCaps;
    bool bLeagueOnly;
//    const char **rewardTable;  // ARM NOTE: This appears to be unused, so I'm pulling it.  Revert if I'm wrong.

    RewardAccumulatedPower **powers;
    RewardAccumulatedSalvage **salvages;
    RewardAccumulatedConcept **concepts;
    RewardAccumulatedProficiency **proficiencies;
    RewardAccumulatedDetailRecipe **detailRecipes;
    RewardAccumulatedDetail **details;
    RewardAccumulatedToken **tokens;
    RewardAccumulatedRewardTable **rewardTables;
    RewardAccumulatedRewardTokenCount **rewardTokenCount;
    RewardAccumulatedIncarnatePoints **incarnatePoints;
    RewardAccumulatedAccountProduct **accountProducts;
    VillainGroupEnum villainGrp;
} RewardAccumulator;


//------------------------------------------------------------------------
// RewardSet
//------------------------------------------------------------------------
typedef struct RewardDef RewardDef;
typedef struct RewardSet RewardSet;
typedef struct RewardDropGroup RewardDropGroup;
typedef struct RewardItemSet RewardItemSet;

typedef enum RewardItemSetTarget
{
    kItemSetTarget_NONE,
    kItemSetTarget_Player = 1,
    kItemSetTarget_Sgrp = kItemSetTarget_Player<<1,
    kItemSetTarget_All = (kItemSetTarget_Sgrp | kItemSetTarget_Player | kItemSetTarget_NONE),
} RewardItemSetTarget;

bool rewarditemsettarget_Valid( RewardItemSetTarget e );
RewardItemSetTarget rewarditemsettarget_BitsFromEnt(Entity *e);


struct RewardDef
{
    int level;

    // Task only reward fields.
    char*   dialog;
    char**  addclues;
    char**  removeclues;
    int     contactPoints;

    RewardSet** rewardSets;
};

typedef struct RewardSet
{
    const char *filename;
    
    F32 chance;
    RewardSetFlags flags;

    bool catchUnlistedOrigins;
    const char** origins;    // Which origin is this set applicable to?

    bool catchUnlistedArchetypes;
    const char** archetypes; // Which archetype (class) is this set applicable to?

    bool catchUnlistedVGs; // Catch all villain groups not already restricted?
    const VillainGroupEnum * groups;       // Which villain groups is this set applicable to?

    const char** rewardSetRequires; // Any special conditions that must be met for this reward to be given

    int experience;  // How much xp to award the player when the set is activated?
    int wisdom;      // How much wisdom to award the player when the set is activated?
    int influence;   // How much ip to award the player when the set is activated?
    int prestige;    // how much prestige to award player's supergroup
    F32 bonus_experience; // 
    int incarnateSubtype;
    int incarnateCount;
    bool bIgnoreRewardCaps;
    bool bLeagueOnly;
    int level;
//    const char **rewardTable; // ARM NOTE: This appears to be unused, so I'm pulling it.  Revert if I'm wrong.


    const RewardDropGroup** sgrpDropGroups;   // What else to reward the player with?    
    const RewardDropGroup** dropGroups;   // What else to reward the player with?
                                    // Only one group in the array will be activated.
} RewardSet;

typedef struct RewardDropGroup
{
    int chance;
    const char** itemSetNames;
    const RewardItemSet** itemSets;
} RewardDropGroup;

//------------------------------------------------------------
// a reward item
// @see: RewardItemSet for places to change if you add a member to this   
//----------------------------------------------------------
typedef struct RewardItem
{
    int chance;
    union
    {
        PowerNameRef power;
        const char *itemName;
    };
    RewardItemType type;
    const char **chanceRequires;
    union
    {
        int count;                // kRewardItemType_DetailRecipe, kRewardItemType_Salvage, kRewardItemType_AccountProduct
        int rewardTableFlags;    // kRewardItemType_RewardTable: See RewardTableFlags below

        int padField1; // Must be bigger or equal in size to all other items in union. This is what is persisted.
    };
    union
    {
        int unlimited;            // kRewardItemType_DetailRecipe
        int rarity;                // kRewardItemType_AccountProduct
//        int fooRewardTable;        // kRewardItemType_RewardTable: unused

        int padField2; // Must be bigger or equal in size to all other items in union. This is what is persisted.
    };
    int remove;
} RewardItem;

enum RewardTableFlags
{
    kRewardTableFlags_AtCritterLevel = 1  // When rewarding the table, consider the player's levels to be the same as the critter level.
} RewardTableFlags;

//------------------------------------------------------------
// a collection of reward items, one of which will be rewarded 
// from this in a reward situation.
// -AB: added salvage :2005 Feb 25 04:31 PM
//----------------------------------------------------------
typedef struct RewardItemSet
{
    const char *name;
    const char *filename;
    const RewardItem** rewardItems;
//     const RewardItem** powerItems;
//     const RewardItem** genericItems; // see RewardItemSet.h for types: proficiency, salvage, concepts, etc.
    int verified;                // shortcut so we only have to verify each RewardItemSet once.
} RewardItemSet;

//--------------------------------------------------
typedef struct RewardChoiceRequirement
{
    const char **ppchRequires;
    const char *pchWarning;
}RewardChoiceRequirement;

typedef struct RewardChoice
{
    const char                    *pchDescription;
    const char                    *pchWatermark;
    const char                    **ppchRewardTable;
    const RewardChoiceRequirement **ppRequirements;
    const RewardChoiceRequirement **ppVisibleRequirements;
    int                        rewardTableFlags;
}RewardChoice;

typedef struct RewardChoiceSet
{
    const char *pchName;
    const char *pchSourceFile;
    const char *pchDesc;
    int isMoralChoice;
    bool disableChooseNothing;
    const RewardChoice **ppChoices;
}RewardChoiceSet;


StaticDefineInt rewardIncarnatePointsSubtypes[];

//------------------------------------------------------------------------
// Shared Reward interface
//------------------------------------------------------------------------
void allRewardNamesInit();
void allRewardNamesCleanup();

void rewardSetsAward(const RewardSet** rewardSets, RewardAccumulator* reward, Entity* player, VillainGroupEnum vgroup, int iLevel, const char *rewardTableName, int containerType, char **lootList); // VillainGroup can be -1 if unknown
RewardAccumulator* rewardGetAccumulator(void);
RewardAccumulator *rewardaccumulator_Init(RewardAccumulator *pReward);
void rewardaccumulator_Cleanup(RewardAccumulator *pReward, bool freeEArrays);

extern TokenizerParseInfo ParseRewardSet[];
cStashTable rewardTableStash();

void rewardVerifyErrorInfo(const char* prefix, const char* filename);
void rewardVerifyMaxErrorCount(int count);
int rewardSetsVerifyAndProcess(RewardSet** rewardSets); // returns success

//------------------------------------------------------------------------
// Public Reward interface
//------------------------------------------------------------------------
void rewardReadDefFiles();
void rewardAddBadgeToHashTable(const char *pchBadgeName, const char *pchFilename);
void rewardAddCostumePartToHashTable(const char *pchCostumePart);
void rewardAddPowerCustThemeToHashTable(char *pchPowerCustTheme);
void rewardDisplayDebugInfo(int display);

const char *rewardGetTableNameForRank(VillainRank rank, Entity *e);
char *rewardGetSalvageTableName(Entity *e);

// currently, this only checks _Salvage tables, and only restricts by villain group and level
void rewardGetAllPossibleSalvageId(const U32 **heaSalvageDropsId, Entity *target);

const RewardDef** rewardFindDefs(const char *nameTable);
const RewardDef* rewardFind(const char *nameTable, int level);
bool reward_FindName(const char * pchName);
int rewardIsPermanentRewardToken( const char * pchRewardTable );
int rewardIsRandomItemOfPower( const char * pchRewardTable );

bool rewardSourceAllowedArchitect(RewardSource source);

int reward_FindMeritCount( const char *storyarcName, int success, int checkBonus);
const char *reward_FindMeritTokenName( const char *storyarcName, int checkBonus);
const char *reward_FindBonusTableName( const char *storyarcName, int replaying);

void rewardDefAward(const RewardDef* def, RewardAccumulator* reward, Entity* player, VillainGroupEnum vgroup, const char *rewardTableName, int containerType, char **lootList);
bool rewardApplyLogWrapper(RewardAccumulator* reward, Entity* e, bool bGivePowers, bool bHandicapped, RewardSource source);
bool rewardApply(RewardAccumulator* reward, Entity* e, bool bGivePowers, bool bHandicapped, RewardSource source, char **lootList);
    // Returns true if the reward accumulator had loot in it. This loot
    //    may not have been given to the entity due to bGivePowers or 
    //    bHandicapped.

#define REWARDLEVELPARAM_USEPLAYERLEVEL (-1)
void rewardFindDefAndApplyToDbID(int db_id, const char **ppchName, VillainGroupEnum vgroup, int iLevel, int bIgnoreHandicap, RewardSource source);
bool rewardFindDefAndApplyToEnt(Entity* e, const char **pchName, VillainGroupEnum vgroup, int iLevel, bool bIgnoreHandicap, RewardSource source, char **lootList);
bool rewardFindDefAndApplyToEntShortLog(Entity* e, const char **ppchName, VillainGroupEnum vgroup, int iLevel, bool isVictimLevel, RewardLevel rl,
                                        bool bIgnoreHandicap, RewardSource source, char **lootList);

// Wrapper for the story system which uses vars and slightly different rules
int rewardStoryAdjustLevel(Entity* player, int iLevel, int iLevelAdjust);
void rewardStoryApplyToDbID(int db_id, const char **ppchName, VillainGroupEnum vgroup, int iLevel, int iLevelAdjust, ScriptVarsTable* vars, RewardSource source);

void rewardAppendToLootList(char **lootList, char *format, ...);

const RewardItem* rewardGetFirstItem(const RewardSet** rewardsets); // very dumb way of finding first power listed in this reward set

void rewardItemSetAccumByName(const char* itemSetName, RewardAccumulator* reward, Entity *e, int deflevel, RewardSetFlags set_flags, RewardItemSetTarget bitsTarget, char **lootList);

typedef enum
{
    RAP_OK = 0,                 // Power was added fine
    RAP_NO_ROOM = -1,           // No room in player inventory for the power type.
    RAP_INVALID_POWER = -2,     // The specified power cannot be found.
    RAP_BAD_REWARD_POWER = -3,  // The specified power type isn't meant to be rewarded.
} RAddPowerResult;
RAddPowerResult rewardAddPower(Entity* e, const PowerNameRef* power, int deflevel, bool bAllowBoost, RewardSource source);

void sendPendingReward(Entity *e);
void rewardChoiceSend( Entity *e, const RewardChoiceSet *rewardChoice );
void rewardChoiceReceive(Entity *e, int choice);
void clearRewardChoice(Entity *e, int clearsRewardChoice);
int playerCanRespec( Entity *e );
int playerCanGiveRespec(Entity *e);
int playerGiveRespecToken(Entity *e, const char *pchRewardTable);
void playerUseRespecToken(Entity *e);
int playerHasFreespec( Entity *e );
void levelupApply(Entity *e, int oldLevel);

float playerGetReputation(Entity *e);
float playerSetReputation(Entity *e, float fRep);
float playerAddReputation(Entity *e, float fRep);

typedef enum RewardTokenType
{
    kRewardTokenType_None,
    kRewardTokenType_Player,
    kRewardTokenType_Sgrp,
    kRewardTokenType_ActivePlayer,
    kRewardTokenType_Count,
} RewardTokenType;

RewardTokenType reward_GetTokenType( const char *rewardTokenName );

typedef struct Character Character;
bool character_IsRewardedAsExemplar(Character *pchar);

typedef enum
{
    SPECIALREWARDNAME_UNLOCKCAPES,
    SPECIALREWARDNAME_UNLOCKGLOWIE,
    SPECIALREWARDNAME_FREECOSTUMECHANGE,
    SPECIALREWARDNAME_COSTUMESLOT,
    SPECIALREWARDNAME_STORYARCMERIT,
    SPECIALREWARDNAME_STORYARCMERITFAILED,
    SPECIALREWARDNAME_DISABLEHALFHEALTH,
    SPECIALREWARDNAME_FINISHEDPRAETORIANTUTORIAL,
    SPECIALREWARDNAME_FINISHEDPRAETORIA,
    SPECIALREWARDNAME_MAX
} SpecialRewardIds;

extern char *specialRewardNames[SPECIALREWARDNAME_MAX];

////////////////////////////////////////////////////////////////////////
void rewardVarClear(void);
void rewardVarSet(char *key, char *value);
char *rewardVarGet(char *key);

///////////////////////
void reward_SetWeeklyTFList(char *tokenList);
void reward_UpdateWeeklyTFTokenList();
void reward_PrintWeeklyTFTokenListActive(char **buffer);
void reward_PrintWeeklyTFTokenListAll(char **buffer);
int reward_FindInWeeklyTFTokenList(char *tokenName);
unsigned int weeklyTF_GetEpochStartTime();

void openSalvage(Entity *e, SalvageItem const *salvage);
void openSalvageByName(Entity *e, const char *nameSalvage);

typedef struct Entity Entity;
void salvage_queryImmediateUseStatus( Entity * e, const char * salvageName, 
        U32* pFlags /*SalvageImmediateUseStatus OUT*/, char** pEStr_ClientMsgStr /*OUT*/ );

#endif /* #ifndef REWARD_H__ */

/* End of File */

