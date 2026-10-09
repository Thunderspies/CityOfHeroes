/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef CHARACTER_BASE_H__
#define CHARACTER_BASE_H__

#include <utilitieslib/stdtypes.h>
#include "entity/RewardItemType.h"
#include "entity/classes.h"   // for kCategory_...
#include "attribmod.h" // for AttribModList and PowerEvent
#include "entity/entityRef.h" // for EntityRef
#include "buddy.h"     // for Buddy
#include "entity/salvage.h"   // for kSalvageRarity_Count
#include "entity/gametypes.h" // for PlayerType

#ifdef SERVER
#include "character_karma.h"
#endif

#ifndef POWER_SYSTEM_H__
#include "entity/power_system.h"
#endif

// forward decls
typedef struct Entity Entity;
typedef struct BasePower BasePower;
typedef struct BasePowerSet BasePowerSet;
typedef struct PowerSet PowerSet;
typedef struct Power Power;
typedef struct PowerListItem PowerListItem;
typedef struct PowerListItem *PowerList;
typedef struct CharacterOrigin CharacterOrigin;
typedef struct Packet Packet;
typedef struct Boost Boost;
typedef int PowerMode;
typedef struct SalvageInventoryItem SalvageInventoryItem;
typedef struct ConceptInventoryItem ConceptInventoryItem;
typedef struct RecipeInventoryItem RecipeInventoryItem;
typedef struct ConceptItem ConceptItem;
typedef struct PowerupSlot PowerupSlot;
typedef struct Invention Invention; 
typedef struct DetailInventoryItem DetailInventoryItem;
typedef struct Workshop Workshop;
typedef struct uiEnhancement uiEnhancement;
typedef struct AuctionInventory AuctionInventory;
typedef struct CharacterKarmaContainer CharacterKarmaContainer;
typedef struct RewardToken RewardToken;

#define NUM_STD_INITIAL_POWER_SETS  2
#define NUM_STD_INITIAL_POWERS      2

#define MAX_INFLUENCE 2000000000

#define MAX_INF_LENGTH_WITH_COMMAS    15    //-2,000,000,000 + the terminating null.

// Because we are mean, trial accounts must be poor forever
#define MAX_INFLUENCE_TRIAL 50000

// Maximum number of possible builds.  This count should match the number of LevelBuild# fields in the database
// in the ents2 table.  Meaning if you increase this, go add more fields to the database.
#define    MAX_BUILD_NUM                8

// Length of a build name
#define BUILD_NAME_LEN                32

// Security level (zero based) at which character is informed of multiple builds
#define    MULTIBUILD_NOTIFY_LEVEL        8

// Security level (zero based) at which character gains their first multiple build
#define    MULTIBUILD_INITIAL_LEVEL    9

// Access level at which all builds are always available
#define    MULTIBUILD_DEVMODE_LEVEL    10



// The maximum number of powers a character can have
// Was 300, boosted to 1000 to cover Incarnate abilities
#define MAX_POWERS 1000


/***************************************************************************/
/***************************************************************************/

typedef struct CharacterStats
{
    float fDamageGiven;
    float fDamageReceived;
    float fHealingGiven;
    float fHealingReceived;
    int iCntKnockbacks;
    int iCntUsed;
    int iCntKills;
    int iCntHits;
    int iCntMisses;
    int iCntBeenHit;
    int iCntBeenMissed;
    Power *ppowLastAttempted;
} CharacterStats;


/***************************************************************************/

typedef enum AllyID
{
    kAllyID_None,
    kAllyID_Good,
    kAllyID_Evil,
    kAllyID_Foul,
    kAllyID_Villain2,
    kAllyID_Hero = kAllyID_Good,
    kAllyID_Monster = kAllyID_Evil,
    kAllyID_Villain = kAllyID_Foul,

    // kAllyID_Count  // no good maximum, arena maps set each player side (512) to a different team
    kAllyID_FirstArenaSide,
} AllyID;

//Bits are &ed with the gangID to give each way the gangID is set a unique range  
typedef enum GangIDRangeBits
{
    GANG_ID_IS_TEAMUP_ID    = ( 1 << 30 ),  //Set based on the teamup ID
    GANG_ID_IS_STATIC        = ( 1 << 29 ),  //Set based on villain definition

} GangIDRangeBits;

/***************************************************************************/
/***************************************************************************/


typedef struct Character
{
    // Defines all the bits of a character

    int iLevel;
        // The level of the character. May be lower than potential level
        //   dictated by XP since the player needs to go "level" himself
        //   (choose new powers, etc.).

    int iCurBuild;
        // Zero based current build the character is using
    int iActiveBuild;
        // Zero based current build the character is using - the different between this an the above is that the heavyweight build switch only alters this.
        // This variable is mostly for debugging and auditing to ensure we maintain consistency when using the lightweight build switch.
    int iBuildChangeTime;
        // Seconds since 2000 at which we can next change builds

    int iBuildLevels[MAX_BUILD_NUM];
        // Array of iLevels, indexed by iCurBuild.  If this ever disagrees
        // with iLevel, assume iLevel trumps iBuildLevels[iCurBuild], adjust to match,
        // log a complaint, and carry on.
        // This has been deprecated on the client.

    int iCombatLevel;
        // The the character fights at. May be larger than iLevel.

    int iCombatModShift;
        // The shift added to iEffCombatLevel when determining the level used for looking up combatmods.

    int iCombatModShiftToSendToClient;
        // Only sums the combat mod shifts without DoNotDisplayShift attached.

    bool bCombatModShiftedThisFrame;
        // Hack flag to prevent character_Tick from overwriting the behavior-assigned value unless necessary.

    int iExperiencePoints;
        // The number of experience points the character has. Duh.

    int iExperienceDebt;
        // The number of experience points of debt the character has.

    int iExperienceRest;
        // The number of experience points of rest the character has.

    int iWisdomLevel;
        // The wisdom (skill) level of the character. May be lower than 
        //   potential level dictated by WisdomPoints since the player needs 
        //   to go "level" himself (choose new Skills, etc.).

    int iWisdomPoints;
        // The number of wisdom points the character has. Duh.

    PlayerType playerTypeByLocation;
        // This indicates whether the player is in the Rogue Isles or Paragon City.
        // This is identical to playerType in all cases except for Going-Rogue-system tourists
        // (aka Vigilantes in the Rogue Isles and Rogues in Paragon City)

    int iInfluencePoints;
        // How much money the character has.

        // Deprecated, should always be zero now that we're using a single currency

    int iInfluenceApt;
        // influence/infamy used in the character's base (apartment)

    Entity *entParent;
        // The parent entity to the character.

    const CharacterOrigin *porigin;
        // Defines the character origin, which has mainly character creation
        //   info.

    const CharacterClass *pclass;
        // Defines the character class, which contains references to the
        //   available categories, power sets, and powers. It also has
        //   some other extra info for character creation.

    int iAllyID;
        // An identifier which determines which "side" the character is on.
        //   It is used if either this character or the target have no
        //   defined iSectID.
        // For pets, a copy of this exists as originalAllyID in AIVars

    int iGangID;
        // Identifies the sub-group that this character belongs to.  If this
        //   is zero, all targeting involving this character falls back to
        //   iAllyId; otherwise everyone with your iSectID is a friend, and
        //   everyone else non-zero is a foe.

    int iGandID_Perm;
        // don't allow the gand ID to change again after this set.


    int bitfieldCombatPhases;
    // bitfield that designates which CombatPhases you are in.  
    // Is mapserver-specific and should not be persisted.

    int bitfieldCombatPhasesLastFrame;
    float fMagnitudeOfBestCombatPhase;

    bool bPvPActive;
        // Notes if this character is allowed to engage in PvP combat

    /**********************************************************************/

    Buddy buddy;
        // A character's buddy is either their mentor or sidekick.

    /**********************************************************************/

    CharacterAttributes attrCur;
        // A cache of the current attributes
        //   (basically, Clamp(attr*attrMod, attrMin, attrMax)).
        //   Don't clamp ToHit here; it'll be clamped during math

    CharacterAttributes attrMod;
        // The combined modifiers to be applied to your stats.

    CharacterAttributes attrMax;
        // The maximum value for each attribute. The attribute is clamped to this
        //   maximum when it is modified. All of these are looked up from
        //   tables in the class and are dependant on level.

    CharacterAttributes attrStrength;
        // The "strength" of the character's application of modifications for
        //   this attribute. When this character inflicts a power on someone,
        //   this strength is used to beef up (or diminish) the effect.
        //   These values come from the current combined modifiers applied to
        //   attribute strengths, starting with 1.0 (100%)

    CharacterAttributes attrResistance;
        // The "resistance" against application of modifications for
        //   this attribute.
        //   These values come from the current combined modifiers applied to
        //   attribute resistances, starting with 0.0 (0%)

    CharacterAttributes attrStrengthLast;
        // A copy of attrStrength from the last tick. This is used to
        //   determine if boosts need to be recalculated.

    bool bRecalcStrengths;
        // If true, all of a character's boosts need to be recalculated.

    float fRegenerationTimer;
    float fRecoveryTimer;
    float fInsightRecoveryTimer;

    /**********************************************************************/

    AttribModList listMod;
        // The list of active effects on this character. These are placed on
        //   a character by powers.

    bool bNoModsLastTick;
        // True if the character had no attrib mods on them last tick.
        // This is used to skip attrib mod accrual when possible.

    EntityRef erKnockedFrom;
        // The entity who most recently knocked this character back.
        // Used to figure out which way to launch the character.

    const PowerFX **ppLastDamageFX;
        // The power(s) which did the most recent hitpoint damage to the
        // character.

    EntityRef erLastDamage;
        // The entity which caused the last hitpoint damage.

    Vec3 vecTeleportDest;
        // The destination of the pending teleport.

    const char *pchTeleportDest;
        // The destination of the pending teleport if crossmap teleport.

    Power *pchTeleportPower;
        // The power originating the pending teleport, if applicable.
        // Used to add a charge to the power if the teleport fails. (NYI for all types)

    PowerMode *pPowerModes;
        // The list of modes that are currently active for the character.

    bool bRecalcEnabledState;
        // True if the list of modes has changed since last tick. Used to
        //   optimize power changes required by mode changes.

    bool bIgnoreCombatMods;
        // True if combat mods are ignored for this character (both as a
        //   source and as a target).

    int ignoreCombatModLevelShift;
        // If bIgnoreCombatMods is true, offset the effective level of the mob by this much

    /**********************************************************************/

    PowerSet **ppPowerSets;
        // A mangaged array of power sets which the character owns.

    PowerSet **ppBuildPowerSets[MAX_BUILD_NUM];
        // Holding tank for multiple copies of the above.

    /**********************************************************************/

    PowerList listTogglePowers;
        // The list of active toggle powers

    PowerList listAutoPowers;
        // The list of auto powers

    PowerList listGlobalBoosts;
        // The list of global boosts

    Power *ppowStance;
        // The click power whose stance we are in.

    int *piStickyAnimBits;
        // The anim bits to set each tick.

    int iSequentialMisses;
        // How many times the character has missed in a row
    float fSequentialMinToHit;
        // Tracks the minimum ToHit during this series of misses.
    float fLastChance;
        // their most recent chance to hit

    bool bCurrentPowerActivated;
        // A state variable to know if the current power has actually
        //   been activated.

    bool bIgnoreBoosts;
        // Just use the base value of the powers, don't add in 

    bool bIgnoreInspirations;
        // Don't let them use inspirations from their tray

    bool bHalfMaxHealth;
        // Don't let current hitpoints go above 50% of max hitpoints. Used in the
        // Praetorian tutorial.

    bool bCannotDie;
        // Character cannot be defeated. While this flag is set no combat badge
        // stats (eg. healing, damage) will accrue. Used in the Praetorian
        // tutorial.

    Power *ppowCurrent;
    EntityRef erTargetCurrent;
    Vec3 vecLocationCurrent;
        // The click power currently in the midst of activation (and target).

    Power *ppowQueued;
    EntityRef erTargetQueued;
    Vec3 vecLocationQueued;
        // The next click power to be activated (and target).

    //These fields were added to address a crash that would occur when a player
    //feeds an inspiration to a pet while it is performing a chaining power. (COH-24838)
    //When a player gives an inspiration to a pet, it adds the inspiration to the
    //pet's power queue, overwriting any value that's currently in that field.
    //ppowAlternateQueue is used to store the inspiration without overwriting
    //ppowQueued.  These variables are a HACK and should only be used to address
    //this issue.  DO NOT extend the purposes of this variable.  --JH
    Power *ppowAlternateQueue;
    EntityRef erTargetAlternateQueued;
    Vec3 vecLocationAlternateQueued;

    Power *ppowTried;
    EntityRef erTargetTried;
    Vec3 vecLocationTried;
        // The last power that was tried but failed (and its target).

    EntityRef erTargetInstantaneous;
        // The target currently clicked on.

//    U32 uiNumTargeting;
        // number of players currently targeting this ent

    StashTable stTargetingMe;
        // listing of hostile players currently targeting me 

    EntityRef erAssist;
        // The entity which this entity is assisting.

    AttribMod *pmodTaunt;
        // The currently operating taunt, or NULL if there isn't one.

    AttribMod **ppmodPlacates;
        // The earray of placates currrently operating.

    AttribMod **ppmodChanceMods;
        // The earray of global chance mods currrently operating.

    float fTerrorizePowerTimer;
        // Using a power causes this timer to start a countdown
        //   If you are terrorized, and this timer has decreased to 0.0, and you take damage,
        //   the timer is set negative, allowing to use powers (which will restart the timer)

    U32 aulEventTimes[kPowerEvent_Count];
        // The seconds since 2000 when an event last occured for the character.

    U32 aulEventCounts[kPowerEvent_Count];
        // The number of times the event occurred since the last tick.

    U32 aulPhaseOneEventCounts[kPowerEvent_Count];
        // A copy of the previous tick's EvenCounts (for use in the following
        // PhaseOne)

    const BasePower *ppowLastActivated;
        // The power which caused the last Activate event. (used to avoid
        //   self-suppression.)

    U32 uiLastApplied;
        // The invokeID which caused the last Apply event. (used to avoid
        //   self-suppression.)

    Vec3 vecLastLoc;
        // The location of the character at the end of the last combat tick.
        //   This is compared to the character's current location at the
        //   beginning of the next tick to set the Moved event

    U32 ulTimeHeld;
    U32 ulTimeSleep;
    U32 ulTimeImmobilized;
    U32 ulTimeStunned;
        // The seconds since 2000 when they character started being held,
        //   put to sleep, immobilized, and stunned. Used for badge stat
        //   tracking.

    Power *ppowDefault;
        // The default power to execute when nothing else is queued up.

    const BasePower *ppowCreatedMe;
        // this is the base power that was used to create me (relevent for pets only)

    PowerList listRechargingPowers;
        // The list of powers used which aren't ready to be used again.

    bool bJustBoughtPower;
        // Dirty bit set by buying a power. This can be checked each tick for
        // things which need to update when new powers are added.

    /**********************************************************************/

    /**********************************************************************/

#define CHAR_INSP_MAX_COLS (5)
#define CHAR_INSP_MAX_ROWS (4)
#define CHAR_INSP_MAX      (CHAR_INSP_MAX_ROWS*CHAR_INSP_MAX_COLS)

    int iNumInspirationCols;
    int iNumInspirationRows;
    const BasePower *aInspirations[CHAR_INSP_MAX_COLS][CHAR_INSP_MAX_ROWS];
        // This is the inspiration inventory. It is a fixed-size array
        //   containing the max number of Inspirations ever allowed.
        //   At lower levels, only a portion of the array is used.

#define CHAR_BOOST_MAX  (70)
    int iNumBoostSlots;
    Boost *aBoosts[CHAR_BOOST_MAX];
        // The boost inventory

    /**********************************************************************/

    CharacterStats stats;

    /**********************************************************************/
    // the inventory
    // NOTE: this is purely for db reasons. inventory that doesn't change
    // between saves to the db needs to keep its original position. for that
    // reason the inventory may not be packed and valid at all times. so when
    // using it watch out for NULL members in items.
    // - an empty inventory slot is specified by a NULL type for an item and amount 0.

    SalvageInventoryItem **salvageInv;
        //  the representation of salvage items in the character's inventory
        // the salvage is broken into each type, and then the position within
        // the inventory of this salvage determines its display position.
    int                    salvageInvBonusSlots;
    int                    salvageInvTotalSlots;
    int                    salvageInvCurrentCount;

    SalvageInventoryItem **storedSalvageInv;
    int                    storedSalvageInvBonusSlots;
    int                    storedSalvageInvTotalSlots;
    int                    storedSalvageInvCurrentCount;
    
    DetailInventoryItem **detailInv;
        // the EArray of detail recipes for use in base creation

    AuctionInventory *auctionInv;
    bool auctionInvUpdated;
        // the auction inventory. has special properties: 
        // - it is not persisted
        // - it can contain all inventory types
    int                    auctionInvBonusSlots;
    int                    auctionInvTotalSlots;

    RecipeInventoryItem **recipeInv;                
    // the EArray of recipes for use in crafting    
    int                    recipeInvBonusSlots;
    int                    recipeInvTotalSlots;
    int                    recipeInvCurrentCount;


    ConceptInventoryItem **conceptInv;                // Not Used
    // the EArray of concepts in the inventory        // Not Used

    StashTable stSentBuffs;
        // remember which buffs we've been sent
    StashTable stNewBuffs; 
        // stash table of all buffs sent this tick
        // used to determine which buffs are no longer valid
//     int *ppClearedBuffs;
//         // earray of buffs IDs that have been removed
    int update_buffs;

    
    struct RecipeHardeningState
    {
#if CLIENT
        int *slottedConceptIdxs; // indexes in conceptInv for concepts to harden
        int recipeBeingHardenedInvIdx; // the source of hardening info
#else // SERVER
        int finalBoostIdx; // points to aBoosts. the power being created.
#endif
    } hardening;
        // tracks the state of the character during the construction
        // of an enhancement (hardening).
    
#ifdef SERVER
    struct 
    {
        int *type;
        int *idx;
    } invStatusChange;    
        // tracks salvage changes. indexes the character's inventory
        // by rarity, then idx. rarity[n] and idx[n] always correspond.
        // These boost slots have changed since last ent update.
    CharacterKarmaContainer karmaContainer;
#endif

    Invention *invention;

    const BasePower ** expiredPowers;

    int countInspirationPowersetRemoval; // not persisted

    int incarnateSlotTimesSlottable[10];
        // The dbSecondsSince2000() instants when the incarnate slots will be slottable again

    char **designerStatuses;

    RewardToken    **critterRewardTokens; // for combat use of reward tokens on non-players, not persisted

    StashTable powersByUniqueID;
        // A fast lookup for all Powers on this Character.  Guaranteed to never have collisions.
        // NOTE:  We do a shallow copy of Powers pointers for shared powersets between builds.
        // In following that precedent, the shared powers only have one entry in this table.

    bool bForceMove;
        // Set to true when a force move is applied to the character.
        // Set to false when the force move has finished running.

    Vec3 vecLocationForceMove;
        // Temporary structure used to pull a vector out of the inner guts of the powers
        //   system to the upper layers of the character tick.  I hope that helps keep the code fast.

    AttribMod **ppModsToPromoteOutOfSuppression;
} Character;

/***************************************************************************/
/***************************************************************************/

typedef struct CharacterRef
{
    Character *pchar;
    float fDist;
} CharacterRef;

typedef struct CharacterList
{
    int iMaxCount;
    int iCount;
    CharacterRef *pBase;
} CharacterList;

typedef struct CharacterIter
{
    CharacterList *plist;
    int iCur;

} CharacterIter;


/***************************************************************************/
/***************************************************************************/

/* Character ***************************************************************/
static INLINEDBG bool character_IsInitialized(Character *pchar) { return pchar && pchar->pclass && pchar->entParent; }

// ARM NOTE:  This is an ugly hack.  I'm delaying the auto-issue power grant until after
//   all powers have been received.  This prevents resetting UIDs.
void character_CreatePartOne(Character *pchar, Entity *e, const CharacterOrigin *porigin, const CharacterClass *pclass);
void character_CreatePartTwo(Character *pchar);
void character_Create(Character *pchar, Entity *e, const CharacterOrigin *porigin, const CharacterClass *pclass);

void character_Reset(Character *pchar, bool resetToggles);
void character_Destroy(Character *pchar, const char *context);
void character_CancelPowers(Character *pchar);
void character_UncancelPowers(Character *pchar);
void character_ResetBoostsets(Character *pchar);
void character_ClearBoostsetAutoPowers(Character *pchar);

void character_ResetStats(Character *pchar);
void character_DumpStats(Character *pchar, char **pestr);
extern char *g_estrDestroyedCritters;
extern char *g_estrDestroyedPlayers;
extern char *g_estrDestroyedPets;
extern U32 g_uTimeCombatStatsStarted;

void character_GetBaseAttribs(Character *p, CharacterAttributes *pattr);

PowerSet *character_BuyPowerSet(Character *p, const BasePowerSet *psetBase );

Power *character_BuyPowerEx(Character *p, PowerSet *pset, const BasePower *ppowBase, bool *pbAdded, int bIgnoreRankRescrictions, int uniqueID, Character *oldCharacter);
Power *character_BuyPower(Character *p, PowerSet *pset, const BasePower *ppowBase, int uniqueID);

bool character_CanBuyPowerWithoutOverflow(Character *pchar, const BasePower *ppowTest);

bool character_CanAddTemporaryPower( Character *p, Power * pow );
bool character_AddTemporaryPower( Character *p, Power * pow );
bool character_CanRemoveTemporaryPower( Character *p, Power * pow );

bool character_BuyBoostSlot(Character *p, Power *ppow);
bool character_RemoveBoostSlot(Character *p, Power *ppow);

bool character_DisableBoosts(Character *p);
bool character_EnableBoosts(Character *p);

bool character_DisableInspirations(Character *p);
bool character_EnableInspirations(Character *p);

PowerSet *character_AddNewPowerSet(Character *p, const BasePowerSet *psetBase);
void character_RemovePowerSet(Character *p, const BasePowerSet *psetBase, const char *context);
PowerSet *character_OwnsPowerSet(Character *pchar, const BasePowerSet *psetTest);
PowerSet *character_OwnsPowerSetInAnyBuild(Character *pchar, const BasePowerSet *psetTest);

// ARM NOTE: We should deprecate use of this function.
const Power *character_GetPowerFromArrayIndices(Character *pchar, int iset, int ipow);

// Functions properly when NumAllowed > 1.
Power *character_OwnsPowerByUID(Character *pchar, int powerUID, int basePowerCRC);

// ARM NOTE: We should stop using character_OwnsPower whenever possible
//   use character_OwnsPowerByUID() instead.
// Doesn't function properly when NumAllowed > 1.
Power *character_OwnsPower(Character *pchar, const BasePower *ppowTest);

int character_OwnsPowerNum(Character *pchar, const BasePower *ppowTest);
int character_NumPowersOwned(Character *p);
int character_NumPowerSetsOwned(Character *p);
void character_VerifyPowerSets(Character *p);

bool character_AtLevelCanUsePower( Character *p, Power* ppow);
bool character_IsAllowedToHavePowerHypothetically(Character *p, const BasePower *ppowBase);
bool character_IsAllowedToHavePowerSetHypothetically(Character *p, const BasePowerSet *psetBase);
bool character_IsAllowedToHavePowerSetHypotheticallyEx(Character *p, const BasePowerSet *psetBase, const char **outputMessage);
bool character_IsAllowedToHavePowerSetHypotheticallyAtSpecializedLevel(Character *p, int specializedLevel, const BasePowerSet *psetBase, const char **outputMessage);
bool character_IsAllowedToSpecializeInPowerSet(Character *p, int specializedLevel, const BasePowerSet *psetBase);
bool character_WillBeAllowedToSpecializeInPowerSet(Character *p, const BasePowerSet *psetBase);

bool character_IsAllowedToHavePower(Character *p, const BasePower *ppowBase);
bool character_WillBeAllowedToHavePower(Character *p, const BasePower *ppowBase);
bool character_IsAllowedToHavePowerAtSpecializedLevel(Character *p, int specializedLevel, const BasePower *ppowBase);
void character_GrantEntireCategory(Character *p, const PowerCategory *pcat, int iLevel);
void character_GrantAllPowers(Character *p);
void character_GrantAutoIssueBuildSharedPowerSets(Character *p);
void character_GrantAutoIssueBuildSpecificPowerSets(Character *p);
void character_GrantAutoIssuePowers(Character *p);

bool character_ModesAllowPower(Character *p, const BasePower *ppowBase);
void character_SetPowerMode(Character *p, PowerMode mode);
void character_UnsetPowerMode(Character *p, PowerMode mode);
bool character_IsInPowerMode(Character *p, PowerMode mode);

#if SERVER
bool character_DeathAllowsPower(Character *p, const BasePower *ppowBase);
#endif

void character_SlotPowerFromSource(Power *ppow, Power *ppowSrc, int iLevelChange);
Power *character_AddRewardPower(Character *p, const BasePower *ppowBase);
void character_RemovePower(Character *p, Power *ppow);
void character_RemoveBasePower(Character *p, const BasePower *ppowBase);

int character_AddInspiration(Character *p, const BasePower *ppowBase, const char *context);
void character_AddInspirationByName(Character* p, char* set, char* power, const char *context);
const BasePower * basePower_inspirationGetByName(char* set, char* power);
bool character_MoveInspiration( Character *p, int srcCol, int srcRow, int destCol, int destRow );
void character_SetInspiration(Character *p, const BasePower *ppowBase, int iCol, int iRow);
bool character_RemoveInspiration(Character *p, int iCol, int iRow, const char *context);
void character_RemoveAllInspirations(Character *p, const char *context);
void character_CompactInspirations(Character *p);
bool character_InspirationInventoryFull(Character *p);
int character_InspirationInventoryEmptySlots(Character *p);
void character_MergeInspiration( Entity * e, char * sourceName, char * destName );
int pcccritter_buyAllPowersInPowerset(Entity *e, const char *powerSetName, int pccDifficulty, int minPowersFromThisSet, int selectedPowers, 
                                      int * bAttackPower, int *difficultyRequirementsMet, int autopower_only );
F32 pcccritter_tallyPowerValueFromPowers(const BasePower ***ppPowers, int rank, int level);
F32 pcccritter_tallyPowerValue(Entity *e, int rank, int level);
/* Inventory ***********************************************************/

void character_SetGenericInventory(Character *p, /*InventoryType*/int type, int id, int idx, int amount);
U32 character_GetGenericInvId(Character *p, /*InventoryType*/int type, int idx);

int character_AddBoost(Character *p, const BasePower *ppowBase, int iLevel, int iNumCombines, const char *context);
int character_AddBoostIgnoreLimit(Character *p, const BasePower *ppowBase, int iLevel, int iNumCombines, const char *context);
int character_AddBoostSpecificIndex(Character *p, const BasePower *ppowBase, int iLevel, int iNumCombines, const char *context, int index);
void character_SetBoost(Character *p, int idx, const BasePower *ppowBase, int iLevel, int iNumCombines,  float const *afVars, int numVars, PowerupSlot const *aPowerupSlots, int numPowerupSlots);
void character_BoostsSetLevel(Character *p, int iLevel);
void character_SetBoostFromBoost(Character *p, int idx, Boost const *srcBoost);
void character_SwapBoosts(Character *p, int idx0, int idx1);

void character_DestroyBoost(Character *p, int idx, const char *context);
Boost *character_RemoveBoost(Character *p, int idx, const char *context);
bool character_BoostInventoryFull(Character *p);
int character_BoostInventoryEmptySlots(Character *p);

char *character_BoostGetHelpText(Entity *pEnt, const Boost *pBoost, const BasePower *pBase, int iLevel);
char *character_BoostGetHelpTextInEntLocale(Entity *pEnt, const Boost *pBoost, const BasePower *pBase, int iLevel);
// These return an EString that is allocated by the function. Please remember to free it when you are 
//        finished with it.


/*  ***********************************************************/

bool character_IsConfused(Character *p);
bool character_IsPvPActive(Character *p);

/* CharacterList ***********************************************************/

CharacterRef *charlist_GetNext(CharacterIter *piter);
CharacterRef *charlist_GetFirst(CharacterList *plist, CharacterIter *piter);
void charlist_Add(CharacterList *plist, Character *pchar, float fDist);
void charList_shuffle(CharacterList *pList);
void charlist_Destroy(CharacterList *plist);
void charlist_Clear(CharacterList *plist);

/* Respec ********************************************************************/
void character_RespecSwap(Entity *e, Character *pcharOrig, Character *pcharNew, int influenceChange);
void character_RespecTrayReset(Entity *e);

/* Multiple Builds **********************************************************/
void character_ResetAllPowerTimers(Character *pchar);
#if SERVER
void character_SelectBuild(Entity *e, int buildNum, int hardSwitch);
void character_SelectBuildLightweight(Entity *e, int buildNum);
void character_TellClientToSelectBuild(Entity *e, int buildNum);
void character_BuildRename(Entity *e, Packet *pak);
#else
void character_SelectBuildClient(Entity *e, int buildNum);
void character_SelectBuildLightweight(Entity *e, int buildNum);
#endif
int character_GetCurrentBuild(Entity *e);
int character_GetActiveBuild(Entity *e);
int character_MaxBuildsAvailable(Entity *e);

bool isPetCommadable(Entity *e);
bool isPetDismissable(Entity *e);
bool isPetDisplayed(Entity *e);

// This should ONLY be used for debugging, and not left in the code forever!
#define VALIDATE_CHARACTER_POWER_SETS 1
void ValidateCharacterPowerSets(Character *p);

static INLINEDBG void character_RecordEvent(Character *pchar, int iEvent, U32 ulTime) 
    { 
        if(pchar) 
        {
            if(ulTime>0) pchar->aulEventTimes[iEvent] = ulTime;
            pchar->aulEventCounts[iEvent]++;
            if(POWEREVENT_STATUS(iEvent)) 
            {
                if(ulTime>0) pchar->aulEventTimes[kPowerEvent_AnyStatus] = ulTime;
                pchar->aulEventCounts[kPowerEvent_AnyStatus]++;
            }
        }
    }

// True if the character is in the Defiant state (no HP, but has "extra" HP remaining)
static INLINEDBG bool character_IsDefiant(Character *p)
{
    return
        (p
            && p->attrCur.fHitPoints==1.0f
            && p->pclass
            && p->pclass->offDefiantHitPointsAttrib!=0
            && (*(ATTRIB_GET_PTR(&p->attrCur, p->pclass->offDefiantHitPointsAttrib)) > 0.0f)
        );
}


extern char * g_smallInspirations[]; 
extern char * g_mediumInspirations[];
extern char * g_largeInspirations[];


int isRangedCreditPower(const BasePower * pow);

void character_RevokeAutoPowers(Character *p, int onlyAEAutoPowers);
void character_RestoreAutoPowers(Entity *e);

// You can see anyone in any phase, period.
#define EXCLUSIVE_VISION_PHASE_ALL 1

// You can only see things that aren't players or player pets in your phase
// (and implicitly things in the "All" or "All But No Traffic" phases)
#define EXCLUSIVE_VISION_PHASE_SEE_NO_OTHER_PLAYERS 2

// You can see anyone in any phase, unless they are in the "No Traffic" phase
// Only Pedestrians and Cars should use this phase
#define EXCLUSIVE_VISION_PHASE_ALL_BUT_NO_TRAFFIC 3

// You can see anyone in your own phase or in the "All" phase,
// but cannot see anything in the "All But No Traffic" phase (this is the only phase that cannot)
#define EXCLUSIVE_VISION_PHASE_NO_TRAFFIC 4

#endif /* #ifndef CHARACTER_BASE_H__ */

/* End of File */

