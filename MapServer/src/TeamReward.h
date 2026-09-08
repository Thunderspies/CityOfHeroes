/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef TEAMREWARD_H__
#define TEAMREWARD_H__

#include "Reward.h"
#include "entity/entGameActions.h"

typedef struct Entity Entity;
typedef struct Entity Entity;
typedef struct RewardToken RewardToken;
typedef int VillainGroupEnum;
typedef enum PowerSystem PowerSystem;
typedef struct Character Character;

void teamRewardReadDefFiles(void);
int villainWillGiveItems(int villainLevel, int playerLevel);
int adjustRewardLevelForSidekick(Entity* e, int iLevel);
void dispenseRewards(Entity *e, DeathBy eDeathBy,int debug);
RewardToken * giveRewardToken( Entity * e, const char * rewardTokenName ); // returns NULL if token already exists on entity
void rewardGetSupergroupMods(Entity *e, float *pfPrestige, float *pfInf);
void logRewards(RewardAccumulator *accum, bool bHandicapped, bool bExemplar, bool bGiveItems);

RewardToken * giveRewardToken( Entity * e, const char * rewardTokenName ); // returns NULL if token already exists on entity
RewardToken * addToRewardToken( Entity * e, const char * rewardTokenName, int val );
RewardToken * setRewardToken( Entity * e, const char * rewardTokenName, int val );
RewardToken * modifyRewardToken( Entity * e, const char * rewardTokenName, int val, bool updateTime, bool setValue );
void removeRewardToken( Entity * e, const char * rewardTokenName ); // only removes player reward tokens, not supergroup reward tokens
void removeAllActivePlayerRewardTokens(Entity * e);

// flags for rewardGroup
#define LOG_GROUP_DEBUG_INFO   2
#define LOG_GROUP_INFO     1
#define LOG_NO_GROUP_INFO  0

#define ROTOR_RANK        0
#define ROTOR_SALVAGE     1 // depricated - used for base salvage
#define ROTOR_ADDITIONAL  2

void rewardGroup(Entity *e, Entity *eVictim, int group_id, const char *pchRewardTable, float fPercent, bool bGetItems, 
                PowerSystem eSystem, int idxRotor,     U32 flags, int groupType);


typedef struct TeamDamageTracker
{
    float fDamage;       // How much damage did this team deal?
    int idGroup;          // Which team did this?
    int iWeight;         // What is the overall weight of the team? (sum of combat levels)
    int iCntMembers;     // How many people from this team participated in defeating the target?
    EntityRef erMember;  // Any member of team.  If teamID == 0, this is the only member.

} TeamDamageTracker;

TeamDamageTracker* teamDamageTrackerCreate();
void teamDamageTrackerDestroy(void* tData);
TeamDamageTracker* teamDamageTrackerFindTeam(TeamDamageTracker** t, int teamID);

//------------------------------------------------------------
//  Helper for gathering the types of level used in reward calculation
//----------------------------------------------------------
typedef struct RewardLevel
{
    PowerSystem type;
    int iExpLevel;    // the earned level of the character (i.e. experience level)
    int iEffLevel;      // the effective level of the character for
                      // calculating if the villain will give items
                      // and for determining the level at which rewards are given.
    int iVillainConAdjust; // Used to adjust the handicap, i.e., allow bosses lower than 3 levels
                           // that still con a color to drop inspirations and enhancements.
} RewardLevel;
RewardLevel character_CalcRewardLevel(Character *p);
int character_CalcEffectiveCombatLevel(Character *pchar);

bool rewardTeamMember(Entity *e,      // Team member to reward
    const char *pchRewardTable,       // Reward table to fetch reward from
    float fRewardScale,               // Scale XP and Inf by this much (used to tweak villains)
    int iLevel,                       // Level in table
    bool isVictimLevel,                  // True means iLevel is Victim's level, needs to be calculated
                                      // False means iLevel is the final level and shouldn't be calculated
    float fPercent,                   // Portion of reward to be given to team as a whole
    float fSizeBonusPercent,          // The group size bonus
    float fTeamPercent,               // The portion of the team part of the reward
    VillainGroupEnum  eVillainGroup,  // The villain def of the defeated villain
    bool bGiveItems,                  // True if powers as well as xp and inf are given.
    bool bIgnoreHandicap,             // True to ignore level handicaps.
    RewardLevel rl,                   // the reward level, take by value because needs to be altered
    RewardSource source,              // Source of the reward, for logging and special handling
    int containerType,                   // type of group (team or league)
    int levelOffset,                  // Offset - only used when per critter ignore combat mods level shiftuing is going on
    char **lootList);                  // Estring list of actual rewards given - used by open salvage
           // Returns true if the reward accumulator had loot in it. This loot
        //    may not have been given to the entity due to bGivePowers or 
        //    handicapping.


//------------------------------------------------------------
//  Real-time reward debugging
//----------------------------------------------------------

typedef struct TeamRewardDebug
{
    int member_id;
    int kill_count;
    int item_count;
    int influence;
    int experience;
    int debt;
    int prestige;

    int salvage_cnt;
    int recipe_cnt;
    int enhancement_cnt;
    int inspirations;
    int base_item;
}TeamRewardDebug;

extern TeamRewardDebug **g_ppTeamRewardDebug;

TeamRewardDebug * getRewardDebug( int id );
void rewardDebugTarget( Entity *e, Entity *target, int iterations );

#endif /* #ifndef TEAMREWARD_H__ */

/* End of File */

