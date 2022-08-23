/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#if (!defined(POWERS_H__)) || (defined(POWERS_PARSE_INFO_DEFINITIONS)&&!defined(POWERS_PARSE_INFO_DEFINED))
#define POWERS_H__

#include <stdlib.h> // for offsetof

#include "entity/boost.h"
#include <utilitieslib/utils/Color.h>
#include "entity/power_system.h"

// POWERS_PARSE_INFO_DEFINITIONS must be defined before including this file
// to get the ParseInfo structures needed to read in files.

#ifdef POWERS_PARSE_INFO_DEFINITIONS
#define CHARACTER_ATTRIBS_PARSE_INFO_DEFINITIONS
#endif
#include "character_attribs.h"

#ifdef POWERS_PARSE_INFO_DEFINITIONS
#define ATTRIBMOD_PARSE_INFO_DEFINITIONS
#endif
#include "attribmod.h"

/***************************************************************************/
/***************************************************************************/

typedef struct BasePowerSet BasePowerSet;
typedef struct PowerCategory PowerCategory;
typedef struct AtlasTex AtlasTex;

typedef int GroupType;
typedef enum PowerMode
{
    kPowerMode_ServerTrayOverride = 0,
    // NOTE:  There are a lot more PowerModes that aren't defined here.
    // Only modes with effects in the code are defined here.
} PowerMode;


typedef struct StructLink StructLink;
#define ParseLink StructLink
typedef struct DimReturn DimReturn;
typedef struct BoostSet BoostSet;
typedef struct BoostSetBonus BoostSetBonus;
typedef struct BoostSetDictionary BoostSetDictionary;
typedef struct SharedMemoryHandle SharedMemoryHandle;

/***************************************************************************/
/***************************************************************************/

#define MAX_ATTRIBMOD_FX 4

/***************************************************************************/
/***************************************************************************/

extern ParseLink g_basepower_InfoLink;
extern ParseLink g_power_PowerInfoLink;
extern ParseLink g_power_SetInfoLink;
extern ParseLink g_power_CatInfoLink;

extern PowerMode g_eRaidAttackerMode;
extern PowerMode g_eRaidDefenderMode;

/***************************************************************************/
/***************************************************************************/

typedef struct PowerFX
{
    char *pchSourceFile;
        // What .pfx file this was loaded from
    int *piModeBits;
        // Sets the "mode" (combat, weapon, shotgun..) the player is in.
        // These bits are always set until the power is deselected.
        // Set always
        // aka SeqBits
    int *piActivationBits;
        // This is an optional pre-animation that happens when you first
        // click on a power button.
        // Set once
        // aka AttachedAnim
    int *piDeactivationBits;
        // This is an optional post-animation that happens when a toggle
        // power is shut off.
        // Set once
    int *piWindUpBits;
        // The wind up
        // Set once
    int *piInitialAttackBits;
        // This is the attack anim used when the character is entering
        //   the stance for the first time.
        // Set once
    int *piAttackBits;
        // This is the attack anim used when the character is already
        //   in the stance.
        // Set once
        // aka cast_anim
    int *piHitBits;
        // The hit reaction
        // Set once
        // aka hit_anim
    int *piBlockBits;
        // The block reaction
        // Set once
    int *piDeathBits;
        // This is used for the death animation if a specific one should
        //   be used instead of the default.
        // Set always??
        // aka deathanimbits
    int *piContinuingBits;
        // Sets the given bits for the lifetime of an AttribMod.
    int *piConditionalBits;
        // Sets the given bits while an AttribMod is alive and the attrCur
        //   for the modified attribute is greater than zero.

    char *pchActivationFX;
        // The effect that happens immediately as the power button on the UI
        // is selected.
        // aka AttachedFxName
    char *pchDeactivationFX;
        // The effect that happens when a toggle power is shut off.
    char *pchWindUpFX;
        // The effect that is played happens during windup
    char *pchInitialAttackFX;
        // The main effect fx filename used when the character is entering
        //   the stance for the first time.
    int iInitialAttackFXFrameDelay;
        // Time to wait before playing the initial attack fx.  Provided so they can use the same fx for AttackFx and InitialAttackFx.
    char *pchAttackFX;
        // The main effect fx filename used when the character is already
        //   in a stance.
        // aka TravellingProjectileEffect
    char *pchBlockFX;
        // block reaction fx filename
    char *pchHitFX;
        // hit reaction fx filename
        // aka AttachedToVictimFxName
    char *pchDeathFX;
        // death fx filename
    char *pchContinuingFX[MAX_ATTRIBMOD_FX];
        // If non-NULL, plays maintained FX for the lifetime of an AttribMod.
        //   When it times out, the FX is killed.
    char *pchConditionalFX[MAX_ATTRIBMOD_FX];
        // If non-NULL, plays maintained FX while an AttribMod is alive and
        //   the attrCur for the modified attribute is greater than zero.

    int iInitialFramesBeforeHit;
        // This is the time it takes for an attack to hit an enemy for the
        //   InitialAttackBits animation.
        // The default is set to happen on frame 15, so if it's different
        //   than that the time is put here.
        // 0 means use the default of 15.

    int iFramesBeforeHit;
        // This is the time it takes for an attack to hit an enemy for the
        //   AttackBits animation.
        // The default is set to happen on frame 15, so if it's different
        //   than that the time is put here.
        // 0 means use the default of 15.

    int iInitialFramesBeforeBlock;
        // This is the time to wait before starting the block animation for
        //   the InitialAttackBits animation.
        // The default is set to happen on frame 0, so if it's different
        //   than that the time is put here.

    int iFramesBeforeBlock;
        // This is the time to wait before starting the block animation.
        // The default is set to happen on frame 0, so if it's different
        //   than that the time is put here.

    int iFramesAttack;
        // This is the time it takes for the attack animation to complete.
        // 0 means use the default of 35.

    float fInitialTimeToHit;
        // Seconds. How long it takes from the beginning of the attack to
        //   when it strikes the target or launches a projectile.
        //   This is for the initial animation.

    float fTimeToHit;
        // Seconds. How long it takes from the beginning of the attack to
        //   when it strikes the target or launches a projectile.
        //   This is for the second animation.

    float fInitialTimeToBlock;
        // Seconds. How long it takes from the beginning of the attack to
        //   when the block animation should start.
        //   This is for the initial animation.

    float fTimeToBlock;
        // Seconds. How long it takes from the beginning of the attack to
        //   when the block animation should start.
        //   This is for the second animation.

    bool bDelayedHit;
        // If true, the hit animation is delayed according to how far away
        //   the victim is from the attacker. This is true for missle powers
        //   which have slow missles (like a fireball), and false for melee
        //   and fast missle powers (like guns).

    bool bImportant;
        // If true, then the FX for this power are "important" and shouldn't
        //   be suppressed for performance enhancement or by user choice.

    float fProjectileSpeed;
        // How fast the projectile moves when it leaves the entity.

    char *pchIgnoreAttackTimeErrors;
        // If not empty, ignores a mismatch between the TimeToActivate and 
        //   FramesAttack.

    ColorPair defaultTint;
        // The tint to use for non-customized powers. This lets artists reuse
        //   tintable assets.

} PowerFX;

typedef struct PowerCustomFX
{
    char *pchDisplayName;
        // Shown in the customization menu
    char *pchToken;
        // Use these settings If the player's costume has this token
    char **altThemes;
        // Alternate themes that can match this customfx if the token itself doesn't match
    char *pchCategory;
        // CustomPowerFx with the same category should be considered exclusive
        // of one another.  In the customization menu, the player will see
        // a list for each category.
    PowerFX fx;
    char* paletteName;
    struct ColorPalette **palette;
} PowerCustomFX;


/***************************************************************************/
/***************************************************************************/

typedef enum PowerType
{
    // Defines if the power is auto, toggle, or click power.

    // Click powers only activate when the user has activated them.
    // Toggle powers activate repeatedly when the character turns them on
    //   until they shut them off (or the character runs out of endurance).
    // Auto powers activate repeatedly.
    // Boosts apply to powers only.

    kPowerType_Click,
    kPowerType_Auto,
    kPowerType_Toggle,
    kPowerType_Boost,
    kPowerType_Inspiration,
    kPowerType_GlobalBoost,

    kPowerType_Count
} PowerType;

typedef enum AIReport
{
    kAIReport_Always,     // Report on hit or miss
    kAIReport_Never,      // Never report to AI
    kAIReport_HitOnly,    // Report only when hit
    kAIReport_MissOnly,   // Report only when missed

    kAIReport_Count
} AIReport;

typedef enum TargetVisibility
{
    // Defines what kind of visibility is required between the caster and
    // the target for successful execution of the power.

    kTargetVisibility_LineOfSight, // The caster must have direct line of sight to the target
    kTargetVisibility_None,        // No visibility is required (or checked)

    kTargetVisibility_Count
} TargetVisibility;

/***************************************************************************/
/***************************************************************************/

typedef enum TargetType
{
    // DANGER: If you change the order or add anything to this list, you
    //         MUST MUST update g_abTargetAllowedDead and
    //         g_abTargetAllowedAlive (below)

    // The thing which can be targetted. Used to specify which kinds of
    // entities are affected, auto-hit, etc by a power.

    kTargetType_None,

    kTargetType_Caster,              // the caster, dead or alive
    kTargetType_Player,              // Any living players except the caster
    kTargetType_PlayerHero,          // Any living player on Team Hero except the caster
    kTargetType_PlayerVillain,       // Any living player on Team Villain except the caster
    kTargetType_DeadPlayer,          // Any dead players including the caster
    kTargetType_DeadPlayerFriend,    // Any dead players on the same side as the caster except the caster and skill checks
    kTargetType_DeadPlayerFoe,       // Any dead players on the same side as the caster except the caster and skill checks

    kTargetType_Teammate,            // Any living teammate and their pets except the caster
    kTargetType_DeadTeammate,        // Any dead teammate and all teammate's dead pets except the caster
    kTargetType_DeadOrAliveTeammate, // All teammates and their pets, dead or alive except the caster

    kTargetType_Villain,             // Any living critter on team evil except the caster and skill checks
    kTargetType_DeadVillain,         // Any dead critter on team evil including the caster and skill checks
    kTargetType_NPC,                 // Any living NPC but villains and skill checks

    kTargetType_DeadOrAliveFriend,   // Anyone dead or alive on the same side as the caster except the caster and skill checks.
    kTargetType_DeadFriend,          // Anyone dead on the same side as the caster except the caster and skill checks.
    kTargetType_Friend,              // Anyone alive on the same side as the caster except the caster and skill checks.

    kTargetType_DeadOrAliveFoe,      // Anyone dead or alive on a different side from the caster except the caster and skill checks.
    kTargetType_DeadFoe,             // Anyone dead on a different side from the caster except the caster and skill checks.
    kTargetType_Foe,                 // Anyone alive on a different side from the caster except the caster and skill checks.

    kTargetType_Location,            // A specific location
    kTargetType_Any,                 // Any living entity which isn't dead
    kTargetType_Teleport,            // A specific location with constraints for teleporting.

    kTargetType_DeadOrAliveMyPet,    // Any target where the source is the owner
    kTargetType_DeadMyPet,           // The target is dead and is the source is the owner
    kTargetType_MyPet,               // The target is alive and is the source is the owner

    kTargetType_MyOwner,             // The target is the owner of the caster (this goes all the way back up to the original owner)

    kTargetType_MyCreator,             //    The target is the creator of the caster (this goes only one level up to the entity that created ent)

    kTargetType_MyCreation,             //    The target is alive and the source is the creator
    kTargetType_DeadMyCreation,         //    The target is dead and the source is the creator
    kTargetType_DeadOrAliveMyCreation,    //    Any target where the source is the creator

    kTargetType_Leaguemate,             //    Any living leaguemate and their pets except the caster
    kTargetType_DeadLeaguemate,         //    Any dead leaguemate and all leaguemate's dead pets except the caster
    kTargetType_DeadOrAliveLeaguemate,    //    All leaguemates and their pets, dead or alive except the caster

    kTargetType_Position,             // A position relative to an entity specified by the designer
    // DANGER: If you change the order or add anything to this list, you
    //         MUST MUST update g_abTargetAllowedDead and
    //         g_abTargetAllowedAlive (in character_target.c)

    kTargetType_Count
} TargetType;

// These are initialized at the top of character_target.c
extern bool g_abTargetAllowedDead[];
extern bool g_abTargetAllowedAlive[];

/***************************************************************************/
/***************************************************************************/

typedef enum EffectArea
{
    // The area effected by the power.

    kEffectArea_Character,
        // Any targeted entity
    kEffectArea_Cone,
        // A cone centered around the ray connecting the source to the target.
    kEffectArea_Sphere,
        // A sphere surrounding the target.
    kEffectArea_Location,
        // A single spot on the ground.
    kEffectArea_Volume,
        // In the same volume as the caster
    kEffectArea_NamedVolume,
        // In a volume named the name as the one the caster is in (not yet implemented)
    kEffectArea_Map,
        // Everybody on the same map as the caster
    kEffectArea_Room,
        // In the same Tray (Room) as the caster
    kEffectArea_Touch,
        // Capsules touch
    kEffectArea_Box,
        // A box positioned relative to the target, oriented along the regular xyz axes

    kEffectArea_Count,
} EffectArea;

/***************************************************************************/
/***************************************************************************/

typedef enum ToggleDroppable
{
    kToggleDroppable_Sometimes,
    kToggleDroppable_Always,
    kToggleDroppable_First,
    kToggleDroppable_Last,
    kToggleDroppable_Never,
} ToggleDroppable;

/***************************************************************************/
/***************************************************************************/

typedef struct BasePowerID
{
    // A handy way to get to this power is to know its index in the
    // data structure. This index is stable and constant for data versions
    // but may change between versions. BasePowerIDs are never stored, they
    // are used only during runtime.

    int icat;
    int iset;
    int ipow;

} BasePowerID;

/***************************************************************************/
/***************************************************************************/

typedef struct PowerStats
{
    float fDamageGiven;
    float fHealingGiven;
    int iCntUsed;
    int iCntHits;
    int iCntMisses;
} PowerStats;

/***************************************************************************/
/***************************************************************************/

typedef struct PowerVar
{
    char *pchName;
    int iIndex;
    float fMin;
    float fMax;
} PowerVar;

typedef enum ShowPowerSetting
{
    kShowPowerSetting_Never = 0, // old false.
    // If on a powerset that the player owns, do not show this powerset or any powers in it (no matter what settings the powers have).
    // If on a power, don't show it (if the powerset allows the power to control its display).

    kShowPowerSetting_Default = 1, // old true.
    // If on a powerset that the player owns, show this powerset, but then allow the power to determine whether it is shown.
    // If on a power, show it (if the powerset allows the power to control its display).

    kShowPowerSetting_Always,
    // If on a powerset that the player owns, show this powerset and all powers in it (no matter what settings the powers have).
    // If on a power, show it (if the powerset allows the power to control its display).

    kShowPowerSetting_IfUsable, 
    // If on a powerset that the player owns, display it, and display all powers in the powerset only if they are usable (no matter what settings the powers have).
    // If on a power, display it only if it is usable (if the powerset allows the power to control its display).

    kShowPowerSetting_IfOwned,
    // If on a powerset that the player owns, display it, and display all powers in the powerset only if they are owned by the player (no matter what settings the powers have).
    // If on a power, display it only if it is owned by the player (if the powerset allows the power to control its display).
} ShowPowerSetting;

typedef enum DeathCastableSetting
{
    kDeathCastableSetting_AliveOnly = 0, // old false.

    kDeathCastableSetting_DeadOnly = 1, // old true.

    kDeathCastableSetting_DeadOrAlive = 2,
} DeathCastableSetting;

/***************************************************************************/
/***************************************************************************/

typedef struct BasePower
{
    // The basic definition of a power. This struct contains all the attributes
    // of a power which are shared by all entities in the game. Character-specific
    // differences (such as number of boosts, level, etc.) are kept
    // in struct Power.

    const char *pchName;
        // Internal name of power.

    const char *pchFullName;
        // Full name, including source category and set

    int crcFullName;
        // CRC32c of pchFullName.  Should be used for a more efficient comparison than stricmp.

    const char *pchSourceName;
        // If this power was duplicated, the original full name

    const BasePowerSet *psetParent;
        // The power set which contains this power.

    BasePowerID id;
        // The current ID of the power. This ID is stable for a
        //   particular version of the data. The server and client must
        //   agree on these IDs.

    PowerSystem eSystem;
        // Which power system this power is associated with.
        //    (Powers or Skills, for now)

    bool bAutoIssue;
        // If true, this power is given away automatically if the player is
        //   allowed to have it. (Must pass ppchRequires and level
        //   requirements.)

    bool bAutoIssueSaveLevel;
        // If true, this autoissue power keeps track of the actual level it
        //    was purchased at, instead of the default behavior of setting
        //    its level to its available level.

    bool bFree;
        // If true, this power doesn't count towards the player's current
        //   count of powers.

    const char *pchDisplayName;
    const char *pchDisplayHelp;
    const char *pchDisplayShortHelp;
    const char *pchDisplayTargetHelp;
    const char *pchDisplayTargetShortHelp;
        // Various strings for user-interaction

    const char *pchDisplayAttackerAttack;
        // Message displayed as chat when the power is executed.

    const char *pchDisplayAttackerAttackFloater;
        // Floater text when the power is executed.

    const char *pchDisplayAttackerHit;
        // Message displayed to the attacker when he hits with this power.

    const char *pchDisplayVictimHit;
        // Message displayed to the victim when he gets hits with this power.

    const char *pchDisplayConfirm;
        // Message displayed to the victim to confirm the power.

    const char *pchDisplayFloatRewarded;
        // Message to float when this power is given as a reward.

    const char *pchDisplayDefenseFloat;
        // Message to float when this power is the defensive cause of a missed
        //   attack, and the attribmod does not specify its own float.

    const char *pchIconName;
        // Name of icon which represents this power.

    PowerFX fx;
        // Describes the visual effects for this power.

    const PowerCustomFX **customfx;
        // Per-costume selectable overrides for 'fx'

    PowerType eType;
        // The type of power: auto, click, or toggle.

    int iNumAllowed;
        // For temporary powers, the number of this power which they
        //   are allowed to have in their inventory. For other power types,
        //   this value is unused.

    const int *pAttackTypes;
        // The list of attack groups this power is part of.
        // Characters have defenses against each group individually.

    const char **ppchBuyRequires;
        // This requires statement is checked to see if the player is allowed to
        //   buy the power or have it autoissued.
        // The requirements expression for this power. If empty, the power's
        //   only requirement is that the character is high enough level.
        //   Otherwise, this is a postfix expression (each element being
        //   an operand or operator) evaluated to determine if the power is
        //   available to the player. See character_IsAllowedToHavePower
        //   for more info.

    const char **ppchActivateRequires;
        // This requires statement is checked to see if the player is allowed to
        //   activate the power. Ignored for Accesslevel.

    const char **ppchSlotRequires;
        // The requirements expression for slotting this boost. If empty, there 
        //   are no requirements for this boost.

    const char **ppchTargetRequires;
        // The requirements expression for what this power targets. If empty,
        //   there are no requirements for the targets of this power.

    char **ppchRewardRequires;
        // The requirements expression for when this power can be granted through the reward system. 
        // If empty, there are no requirements for when this power can be granted.

    const char **ppchAuctionRequires;
        // The requirements expression to determine if this power can be listed in the AuctionHouse. 
        // If empty, there are no requirements needed to be listed in the AuctionHouse.

    char *pchRewardFallback;
        // The power that will be granted if the ppchRewardRequires is present and not met.
        // If empty, then no power will be granted if the ppchRewardRequires is present and not met.

    float fAccuracy;
        // Probability. Chance to hit.

    bool bIgnoreStrength;
        // Bool. Ignore all AttribMod strength modifiers when calculating
        // the final strength for the power.

    bool bNearGround;
        // Bool. Requires the attacker to be on the the ground to succeed.

    bool bTargetNearGround;
        // Bool. Requires the target to be on the the ground to succeed.

    DeathCastableSetting eDeathCastableSetting;
        // Determines whether the power can be used only while alive,
        //   only while dead, or while either dead or alive.

    bool bCastThroughHold;
    bool bCastThroughSleep;
    bool bCastThroughStun;
    bool bCastThroughTerrorize;
        // Bool. Allows the power to be cast by the character through
        //   various control effects that normally disallow power use

    bool bToggleIgnoreHold;
    bool bToggleIgnoreSleep;
    bool bToggleIgnoreStun;
        // Bool. Allows the power (assuming it is a toggle power) to 
        //   remain active even when the use is affected by the given
        //   control effect.

    bool bShootThroughUntouchable;
        // Bool.  Allows the power to ignore the untouchable aspect of
        //   the target.

    bool bInterruptLikeSleep;
        // Bool.  Specifies that this power is only interrupted by
        //   attribmods that would also cancel sleep, rather than all 
        //   foe attribmods.

    bool bIgnoreLevelBought;
        // If true, this power will work regardless of when the power was bought.  This is used for
        //   certain boostsets and accolades.

    AIReport eAIReport;
        // Specifies when and if the AI is told about attacks with this
        // power.

    EffectArea eEffectArea;
        // Enum. Coverage of effect: target, cone, sphere

    int iMaxTargetsHit;
        // Count. Maximum number of targets allowed to be hit by the power,
        //   Used to limit AoE attacks. If more than MaxTargets could be
        //   hit, the ones farthest from the target point are rejected.
        // This is only used when EffectArea is Sphere or Cone.

    float fRadius;
        // Feet. Radius of effect. (radius around target)

    float fArc;
        // RADIANS!  NOT DEGREES!  Number of spherical radians of the cone, centered
        //   around a ray connecting the attacker to the target.

    Vec3 vecBoxOffset;
    Vec3 vecBoxSize;
        // Feet.  Used to define a cuboid volume positioned relative to the target,
        //   aligned to the basic xyz axes.

    float fRange;
        // Feet. Max distance to target for success

    float fRangeSecondary;
        // Feet. Max distance to secondary target for success

    float fTimeToActivate;
        // Seconds. How long it takes to do the whole attack (including
        //   wind up, time it takes to strike the target (or launch), and
        //   the follow through.

    float fRechargeTime;
        // Seconds. The time after a power is used that the power can be used
        //    again.

    float fInterruptTime;
        // Seconds. The period of time, starting at the beginning of the
        //    attack, where the attack can be interrupted.

    float fActivatePeriod;
        // Seconds. Time between automatic activations of the power.

    float fEnduranceCost;
        // Units. Cost of actviation.

    float fInsightCost;
        // Units. Cost of actviation.

    int iTimeToConfirm;
        // Seconds. If non-zero, each player affected by the power must confirm
        //   that they want to be hit. The player has the given number of
        //   seconds to confirm. If they do nothing, the power is cancelled.
        //   Endurance is NOT given back to the caster if cancelled.

    int iSelfConfirm;
        // If 1, a confirmation dialog will display for self-targeted powers.
        //   The default behavior is only other-targeted powers will use a confirmation window.

    const char **ppchConfirmRequires;
        // If the target of this power fails this requires statement,
        //   they will not receive a confirmation dialog for this power.
        //   The power will go off without the target's consent.

    int iMaxBoosts;
        // Count. Maximum number of boosts (aka enhancements) which can be
        //   on this power. This includes even free boosts given.

    bool bShowBuffIcon;
        // Bool. If true, then the buff icon is shown for this power,
        //   otherwise it is not.

    ShowPowerSetting eShowInInventory;
        // Determines whether the power is shown in the power inventory or not.

    bool bShowInManage;
        // Bool. If true, then the power is shown in the power management
        //   (i.e. the enhancement) and enh slot assignment screens, otherwise
        //   it is not.
        // Should I switch this to use ShowPowerSetting as well?

    bool bShowInInfo;
        // Bool. If true, then the power is shown in the power tab
        //   of the Player Info window.  Also, if this power is in the Inherent powerset,
        //   it will show up in the contact dialog notifications that occur when new powers
        //   are automatically granted upon training.

    bool bDeletable;
        //  If true AND this power is a temp power, it may be deleted by the player.

    bool bTradeable;
        // If this is true, the power may be traded to another player.

    bool bDoNotSave;
        // Bool. If true, then the power will never be saved to the database.
        //   This means that it will vanish on mapmoves, logout, and
        //   disconnects. Useful  for temporary powers which you never want
        //   to leave a zone.

    bool bHasInvokeSuppression;
        // Bool. If true, then this power contains attribmods that are suppressed
        //   by an invoke event.  If this is the case, this power should not
        //   itself generate some invoke events, to prevent self-suppression.
        //   This is calculated automatically.

    ToggleDroppable eToggleDroppable;
        // If this power is a toggle power, specifies how/when it responds
        //   to a kDropToggle AttribMod. The default is sometimes.

    /*********************************************************************/

    bool bDestroyOnLimit;
        // If true, the power is removed from the character's power inventory
        //   if it reaches the usage limit.

    bool bHasUseLimit;
        // If true, the power has some usage limit. iNumCharges and
        //   fUsageTime are used. If false, then iNumCharges and fUsageTime
        //   are ignored.

    bool bStackingUsage;
        // If true, this power will extend existing powers when granted multiple times
        
    int iNumCharges;
        // The number of times the power can be used.

    int iMaxNumCharges;
        // The max number of charges that iNumCharges can be extended to.

    float fUsageTime;
        // The number of seconds which the power can be "on" overall.
        // Used on toggle powers only.

    float fMaxUsageTime;
        // The max number of seconds that fUsageTime can be extended to.

    /*********************************************************************/

    bool bHasLifetime;
        // If true, the power has an overall lifetime. fLifetime and 
        //   fLifetimeInGame will be used to determine how long the power will exist.

    float fLifetime;
        // The number of seconds the power will function.

    float fLifetimeInGame;
        // The number of in-game seconds the power will function.

    float fMaxLifetime;
        // The max number of seconds that fLifetime can be extended to.

    float fMaxLifetimeInGame;
        // The max number of seconds that fLifetimeInGame can be extended to.

    /*********************************************************************/

    bool bBoostIgnoreEffectiveness;
        // For Boosts, if this is true, then the boost's level relative to
        //   the character level does not impact its effectiveness.

    bool bBoostAlwaysCountForSet;
        // For Boosts, if this is true, then the boost is always counted as
        //        part of the set, even if exemplared below the level of the boost.

    bool bBoostCombinable;
        // For Boosts, if this is true, then the boost can be combined.

    bool bBoostTradeable;
        // For Inspirations and Boosts, if this is true, then the boost can be traded.

    bool bBoostAccountBound;
        // For Inspirations and Boosts, if this is true, then the boost can only be traded to other characters on the same account

    bool bBoostBoostable;
        // For Boosts, if this is true, then the boost can be combined with enhancement boosters.

    bool bBoostUsePlayerLevel;
        // For Boosts, if this is true, then the boost uses the player's level rather than the boost's level

    int iBoostInventionLicenseRequiredLevel;
        // For Boosts, the lowest level of boost that a character can slot that requires an invention license

    int iMinSlotLevel;
        // For Boosts, the lowest level a character can be to slot this boost.

    int iMaxSlotLevel;
        // For Boosts, the highest level a character can be to slot this boost.

    int iMaxBoostLevel;
        // For Boosts the use player level, the highest level that will be used to calc boost power

    const char *pchBoostCatalystConversion;
        // For Boosts, if set, the boost may be combined with an enhancement catalyst to create the specified enhancement

    /*********************************************************************/

    const char *pchStoreProduct;
        // For Boosts & Inspirations, if set, the item will not display on the auction house if the item is not published on the store

    /*********************************************************************/

    TargetVisibility eTargetVisibility;
        // Specifies what kind of visibility is required for the target.

    TargetType eTargetType;
        // What things can be targetted by this power.

    TargetType eTargetTypeSecondary;
        // What things can be targetted by this power.

    const TargetType *pAffected;
        // TargetTypeList. Enttypes of things which are affected.

    const TargetType *pAutoHit;
        // TargetTypeList. Enttypes of things which are always affected.

    bool bTargetsThroughVisionPhase;
        // If true, can target, affect, and auto-hit things that are in a different vision phase.

    const BoostType *pBoostsAllowed;
        // List of boost types allowed by the power

    const GroupType *pGroupMembership;
        // List of power groups this power belongs to. Only one of the
        // powers in a power group can be on at a time. Using another
        // power from the same group shuts off any other powers which are
        // on.

    const PowerMode *pModesRequired;
        // If any modes are listed, the character must be in one of the modes
        //   to activate the power. If the character exits a mode which is
        //   required by the power, it is shut off.

    const PowerMode *pModesDisallowed;
        // If the character goes into any of the modes listed here,
        //   the power is shut off (if it's a toggle or auto) and the
        //   character will be unable to execute it.

    const char **ppchAIGroups;
        // List of AI groups this power belongs to. Determines how a
        // particular power is to be used.

    const PowerVar **ppVars;
        // List of variables which can be referenced by attrib mods on this
        //   power. These variables refer to values store for each instance
        //   of the base power (in struct Power). The major purpose of these
        //   vars is for the Invention system, where they will be used to
        //   make Boosts.

    const BoostSet **ppBoostSets;
        // List of boost sets which can be used in this power

    const BoostSet *pBoostSetMember;
        // The BoostSet to which this power belongs (used for boosts)

    const AttribModTemplate **ppTemplates;
        // List of AttribMods to apply (the "effects")

    bool bHasNonBoostTemplates;
        // True if the power has templates that are executed, rather than boosts.
        //   This is used for boosts and bonus powers that can attach new
        //   templates when the power is applied.

    bool bUseNonBoostTemplatesOnMainTarget;
        // True if the power is an AoE but you only want procs to go off once
        //   (on the main target) instead of on all targets.

    bool bMainTargetOnly;
        // If true, only the main target is animated and has FX put on
        //   him. Otherwise, everyone in the effect area will have the
        //   hit and block bits/fx applied.

    const int *pStrengthsDisallowed;
        // A list of character attributes whose strength cannot be modified.
        //   This can be used to make a Range buff not affect a power, for
        //   example.

    PowerStats stats;
        // Stats for balancing

    const char *pchSourceFile;
        // Filename this definition came from.

    const DimReturn ***apppDimReturns[CHAR_ATTRIB_COUNT];
        // The diminishing returns info for this power.

    const char **ppchHighlightEval;
        // The power will be highlighted in the UI when this is true

    const char *pchHighlightIcon;
        // This icon will be shown when ppchHighlightEval is true

    U8 rgbaHighlightRing[4];
        // A ring of this color will be shown around the power's icon when
        // ppchHighlightEval is true

    float fTravelSuppression;
        // if not zero, add this length of time to the combat travel power suppression
    float fPreferenceMultiplier;
        //    default this to 1.0f;

    bool bDontSetStance;
        //    default this to 0;

    //--------------------------------------------------
    // Values for gauging the 'worth' of a power, used only in AE atm
    F32 fPointVal;
    F32 fPointMultiplier; 
    
    int iForceLevelBought;

    const char *pchChainIntoPowerName;

    bool bInstanceLocked;
    bool bIsEnvironmentHit;
    bool bShuffleTargetList;

    bool bRefreshesOnActivePlayerChange;
        // This power is revoked and then granted again to any player that currently owns it when:
        // An Active Player reward token changes state for the team's current Active Player,
        // The team's current Active Player changes, or the player joins or leaves a team.
        // The initial purpose for this flag is to refresh vision phase powers automatically.

    bool bPowerRedirector;
    bool bCancelable;
    bool bIgnoreToggleMaxDistance;

    int iServerTrayPriority;

    bool bAbusiveBuff;
        // If true, attrib mods applied by this power will be cleared whenever entering a map
        //   that uses eRAT_ClearAbusiveBuffs.
    ModTarget ePositionCenter;
    float fPositionDistance;
    float fPositionHeight;
    float fPositionYaw;

    bool bFaceTarget;
    
#ifdef CLIENT
    float ht;
    float wd;
    // used for the power customization menu
#endif

} BasePower;


/***************************************************************************/
/***************************************************************************/

typedef struct PowerSet PowerSet;
typedef struct AIPowerInfo AIPowerInfo;
typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;

#define MAX_BOOSTS_BOUGHT 5
typedef struct Power
{
    // The character-specific aspects of a power

    const BasePower *ppowBase;
        // This is the current base power we're pointing at (it can get redirected)

    const BasePower *ppowOriginal;
        //    This is the original base power assigned upon creation and should never change after this
        
    PowerSet *psetParent;
        // The PowerSet which contains this power.

    int iUniqueID;
        // This uniquely identifies this power within its Character.

    int idx;
        // The index of the power within the PowerSet.
        // Not a reliable lookup index, because the power array can change.

    int iLevelBought;
        // Which character level the power was bought at. Used to determine how
        //   strong the power is.

    int iNumCharges;
        // How many times the power can be used before it is used up.

    float fUsageTime;
        // How long has the power been on total. (Toggle powers only)

    float fAvailableTime;
        // How long the power has been available total.

    U32 ulCreationTime;
        // The time when this power was created on the player. The time
        // is the number of seconds since 2000.

    int iNumBoostsBought;
        // no more than MAX_BOOSTS_BOUGHT allowed

    float fTimer;
        // This timer is used to count how long the power has before it
        //   can be used again.
        // This timer is also used to see if the power is interrupted.
        // For Auto powers, it is how long until the power will be
        //   automatically reapplied.

    bool bActive;
        // Toggle powers: true if the power is "on".
        // Auto powers: always true
        // Click powers: true if the power is currently happening.

    Boost **ppBoosts;
        // List of boosts which are on this Power

    Boost **ppGlobalBoosts;
        // List of global boosts which are being applied to this Power

    bool bBoostsCalcuated;
        // True if the boost calculation is up to date. Cleared whenever
        //   boosts are added and removed.

    bool bBonusAttributes;
        // True if the power has bonus attributes to execute from its
        //   boosts.

    const BoostSetBonus **ppBonuses;
        // List of bonuses from BoostSets that are active on this Power

    CharacterAttributes attrStrength;
        // Storage for calculated strength stats when this power
        //   is being used with enhancements.

    CharacterAttributes *pattrStrength;
        // The strength stats to use for the character when this power
        //   is being used.

    float fRangeLast;
        // The client needs to know the current range of the power. This
        // stores the last value sent to the client as well as the most
        // recent one.

    bool bOnLastTick;
        // true if the power was applied last tick. Used to minimize the
        //    number of spurious messages and other stuff which should only
        //    happen when the overall state of the power changes.

    bool bFirstAttackInStance;
        // True if this attack is the first one for the current stance.

    PowerStats stats;
        // Use for balancing

    AIPowerInfo* pAIPowerInfo;
        // Back-pointer for AI to quickly lookup internal info.
        // Valid if it's not NULL (usually only critters).

    float afVars[POWER_VAR_MAX_COUNT];
        // Values for each of the power's vars. (Defined in the BasePower.)


    PowerupSlot aPowerupSlots[POWERUP_COST_SLOTS_MAX_COUNT];
        // slots needed to activate this.

    AttribMod **ppmodChanceMods;
        // The earray of power chance mods currrently on this power

    Power *chainsIntoPower;
        //    the current power chains into the power specified here

    int instanceBought;

    bool bDontSetStance;

    bool bIgnoreConningRestrictions;
        // Custom Critter flag
        // Ignore rank and level requirements for the power, it was specifically selected

    bool bDisabledManually;
        // If this is true, the power has been disabled via direct player control
        //   (either unequipped Incarnate abilities or /power_disable.)

    bool bEnabled;
        // If this is true, we have triggered enable mods for this power but not the
        //   corresponding disable mods.  If false, we've either done neither or both of the above.
        // Things that trigger a change in this:  buy/revoke power, power modes,
        //   ActivateRequires, build switches, instance locks, death/resurrection.

    int attribModRefCount;
        // The count of the number of currently existing AttribMods that use this Power
        //    as their parentPowerInstance.
        // This Power cannot be freed while this count is nonzero.
        // Used only on the server.

    bool bHasBeenDeleted;
        // This flag is set once the system has determined this Power should be freed.
        // The power will be freed only once its attribModRefCount is zero and this flag is true.
        // Used only on the server.

} Power;

extern Power **g_deletedPowers;
    // This EArray lists all powers that were deleted but still have AttribMod references.

/* Structure PowerRef
*  References a single power.
*  It is expected that for this structure to be the first field in most
*  power info related structures so an array of them can be searched using
*  powerInfo_FindPowerXXX() functions.
*/
typedef struct PowerRef
{
    int buildNum;
    int powerSet;
    int power;
} PowerRef;

// These structures have elements of the concept of an "activation" or "invocation" of a power
//   on a target, but it's not used thoroughly enough for me to rename it to that.
typedef struct PowerListItem
{
    struct PowerListItem *next;
        // These must stay in order and so they work with genericlist.
    struct PowerListItem *prev;
        // These must stay in order and so they work with genericlist.

    Power *ppow;
    EntityRef erTarget;
    Vec3 vecTarget;
    unsigned int uiID;
} PowerListItem;

typedef struct PowerListIter
{
    PowerListItem **pphead;
        // The top of the list

    PowerListItem *pposNext;
    PowerListItem *pposCur;
        // Variables for tracking where the iteration is. The iterator is
        //   designed so that the "current" PowerListItem can safely be
        //   removed and the iteration can continue.

} PowerListIter;

typedef PowerListItem * PowerList;
    // Some syntactic sugar for Power lists.

/***************************************************************************/
/***************************************************************************/

typedef struct BasePowerSet
{
    // Defines a set of powers which are group together and become available
    // over time. Again, this defines the shared attributes. Character-specific
    // attributes (length of time held, for example) are found in struct
    // PowerSet.
    //
    // If the same Power appears in more than one PowerSet (and this includes
    // each class-specific power-pool sets) then it needs to be defined again.
    // This is true since each BasePower refers to a single PowerSet.

    const char *pchName;
        // Internal name

    const char *pchFullName;
        // Full name, including source category

    const PowerCategory *pcatParent;
        // The category which contains this Power Set

    const char *pchDisplayName;
    const char *pchDisplayHelp;
    const char *pchDisplayShortHelp;
        // Various strings for user-interaction

    const char *pchIconName;
        // Name of icon which represents this power set.

    const char **ppchCostumeKeys;
        // Costume keys given to players with this set

    const char **ppchCostumeParts;
        // Default costume pieces for new players, or parts to add for old ones

    PowerSystem eSystem;
        // Which power system this power set (and all the powers within it)
        //   is associated with. (Powers or Skills, for now)

    bool bIsShared;
        // If true, this powerset is not specific to one of a player's multiple builds
        //   and is instead shared among them.

    ShowPowerSetting eShowInInventory;
        // Determines whether the power is shown in the power inventory or not.

    bool bShowInManage;
        // Bool. If true, then the power set is shown in the power management
        //   (i.e. the enhancement) and enh slot assignment screens, otherwise
        //   it is not.
        // Should I switch this to use ShowPowerSetting as well?

    bool bShowInInfo;
        // Bool. If true, then the power set is shown in the power tab
        //   of the Player Info window.

    int iSpecializeAt;
        // If non-zero, this powerset is a specialization powerset, available
        // at this level as an additional set to pick from as well as their
        // chosen set - and thus cannot be picked as a main set.

    const char **ppSpecializeRequires;
        // Requires statement controlling whether specialization powerset
        // can be offered of not (so multiple specializations can be listed
        // for a class and once one or more is picked the rest are shut off).

    const char *pchAccountRequires;
        // Account evaluator statement controlling whether a player has access to this powerset.

    const char *pchAccountTooltip;
        // Tooltip to display when player has failed to meet the AccountRequires

    const char *pchAccountProduct;
        // Product that can be bought from the store to help the player meet the AccountRequires

    const char **ppchSetBuyRequires;
        // Character evaluator statement controlling whether a player has access to this powerset.

    const char *pchSetBuyRequiresFailedText;
        // Error message to display when the player fails the SetBuyRequires

    const BasePower **ppPowers;
        // The array of powers which are part of this power set.
        
    const char ** ppPowerNames;
        // The array of names of included powers
    cStashTable haPowerNames;

    const int *piAvailable;
        // How old the set has to be (in levels) before the power becomes
        //    available.

    const int *piAIMaxLevel;
        // AI ONLY : Beyond what level the power will no longer be used by the AI

    const int *piAIMinRankCon;
    const int *piAIMaxRankCon;
        // AI ONLY : Range of rank con that this power is usable by
    const int *piMinDifficulty;
    const int *piMaxDifficulty;

    const char *pchSourceFile;
        // Filename this definition came from.

    int iForceLevelBought;

#ifdef CLIENT
    int iCustomToken;
    const char **ppchCustomTokens;
    float ht;
    float wd;
    // used for the power customization menu
#endif
} BasePowerSet;

/***************************************************************************/
/***************************************************************************/

typedef struct PowerSet
{
    // The character-specific aspects of power set

    const BasePowerSet *psetBase;
        // The basic power set definition

    Character *pcharParent;
        // The character this PowerSet belongs to.

    int idx;
        // The index of the PowerSet within the Character

    int iLevelBought;
        // Which character level the power set at. Used to determine which
        //   powers are available.

    Power **ppPowers;
        // The array of powers the character has bought from this set.

} PowerSet;

/***************************************************************************/
/***************************************************************************/

typedef struct PowerCategory
{
    // Defines a set of related power sets. (Examples include melee,
    // defense, etc.)

    const char *pchName;
        // Internal name
    const char *pchDisplayName;
        // Name the user sees
    const char *pchDisplayHelp;
    const char *pchDisplayShortHelp;
        // Various strings for user-interaction

    const char *pchSourceFile;
        // Filename this definition came from.

    const BasePowerSet **ppPowerSets;
        // Array of power sets which make up this category.
    const char **ppPowerSetNames;
} PowerCategory;

/***************************************************************************/
/***************************************************************************/

typedef struct PowerDictionary
{
    // Defines all the categories, sets, and powers
    const PowerCategory **ppPowerCategories;
    cStashTable haCategoryNames;
    const BasePowerSet **ppPowerSets;
    cStashTable haSetNames;
    const BasePower **ppPowers;
    cStashTable haPowerCRCs;
} PowerDictionary;

/***************************************************************************/
/***************************************************************************/

extern SHARED_MEMORY PowerDictionary g_PowerDictionary;
extern BasePowerSet **g_SharedPowersets;
extern int g_numSharedPowersets;
extern int g_numAutomaticSpecificPowersets;

/***************************************************************************/
/***************************************************************************/

extern U32 g_ulPowersDebug;


//
// All the parser stuff has been moved to powers_load.c
//


//
// Parse stuff from powers_load.c
//

extern StaticDefineInt BoolEnum[];
extern TokenizerParseInfo ParseBasePower[];
extern TokenizerParseInfo ParsePowerDictionary[];
extern TokenizerParseInfo ParsePowerSetDictionary[];
extern TokenizerParseInfo ParsePowerCatDictionary[];


/***************************************************************************/
/***************************************************************************/

/* PowerDictionary ***************************************************/

void ReloadPowers(PowerDictionary *pdictDest, const PowerDictionary *pdictSrc);
void load_PowerDictionary(SHARED_MEMORY_PARAM PowerDictionary *ppow,char *pchFilename, bool bNewAttribs, void (*callback)(SharedMemoryHandle *phandle, bool bNewAttribs));
void powerdict_ValidateEntCreate(const PowerDictionary *pdict); // in powers_load.c - called after VillainDefs are loaded

bool CreateDimReturnsData(void); // in powers_load.c

void powerdict_ResetStats(PowerDictionary *pdict);
void powerdict_DumpStats(const PowerDictionary *pdict);

const PowerCategory *powerdict_GetCategoryByName(const PowerDictionary *pdict, const char *pch);
const BasePowerSet *powerdict_GetBasePowerSetByName(const PowerDictionary *pdict, const char *pchCat, const char *pchSet);
const BasePowerSet *powerdict_GetBasePowerSetByFullNameEx(const PowerDictionary *pdict, const char *pch, bool bShowErrors);
const BasePowerSet *powerdict_GetBasePowerSetByFullName(const PowerDictionary *pdict, const char *pch);
const BasePower *powerdict_GetBasePowerByName(const PowerDictionary *pdict, const char *pchCat, const char *pchSet, const char *pchPow);
const BasePower *powerdict_GetBasePowerByString(const PowerDictionary *pdict, const char *pchDesc, int *piLevel);
const BasePower *powerdict_GetBasePowerByFullNameEx(const PowerDictionary *pdict, const char *pch, bool bShowErrors);
const BasePower *powerdict_GetBasePowerByFullName(const PowerDictionary *pdict, const char *pch);
const BasePower *powerdict_GetBasePowerByCRC(const PowerDictionary *pdict, int CRC);
const BasePower *powerdict_GetBasePowerByIndex(const PowerDictionary *pdict, int icat, int iset, int ipow, const char **catName, const char **setName);
const BasePower *powerdict_GetBasePowerByID(const PowerDictionary *pdict, BasePowerID *pid, const char **catName, const char **setName);
int powerdict_GetCRCOfBasePower(const BasePower *ppowBase);
bool powerdict_GetBasePtrsByStringEx(const PowerDictionary *pdict, const char *pchDesc, const PowerCategory **ppCat, const BasePowerSet **ppsetBase, const BasePower **pppow, int *piLevel, bool bShowErrors);
bool powerdict_GetBasePtrsByString(const PowerDictionary *pdict, const char *pchDesc, const PowerCategory **ppCat, const BasePowerSet **ppsetBase, const BasePower **pppow, int *piLevel);
const BasePower *basepower_FromPath(char *path); // does replacement
char *basepower_ToPath(const BasePower *ppow);

char *powerdict_GetBasePowerByNameError(const PowerDictionary *pdict, const char *pchCat, const char *pchSet, const char *pchPow);


/* Category **********************************************************/
const BasePowerSet *category_GetBaseSetByName(const PowerCategory *pcat, const char *pch);
bool category_CheckForSet(const PowerCategory *pcat, const char *pch);


/* BaseSet ***********************************************************/
const BasePower *baseset_GetBasePowerByName(const BasePowerSet *pset, const char *pch);
int baseset_BasePowerAvailableByIdx(const BasePowerSet *pset, int iIdx, int iLevel);
bool baseset_IsCrafted(const BasePowerSet *pset);
static INLINEDBG bool baseset_IsSpecialization(const BasePowerSet *pset) { return (pset->iSpecializeAt ? true : false); }


/* BasePower *********************************************************/
int basepower_GetNames(const BasePower *ppow, const char **ppchCat, const char **ppchSet, const char **ppchPow);
void basepower_SetID(BasePower *ppow, int icat, int iset, int ipow);
AtlasTex *basepower_GetIcon(const BasePower *ppow);
int basepower_GetVarIdx(const BasePower *ppowBase, const char *pchName);
const AttribModTemplate *basepower_GetAttribByName(const BasePower *ppowBase, const char *pchName);

// Used to map PFX# specifications for continuing/conditional fx to an actual fx path.
const char* basepower_GetAttribModFX(const char* baseFXName, const char* const * attribModFX);

bool basepower_ShowInInventory(Character *pchar, const BasePower *ppowBase);


/* PowerSet **********************************************************/

PowerSet *powerset_Create(const BasePowerSet *psetBase);
void powerset_Destroy(PowerSet *pset, const char *context);
void powerset_Empty(PowerSet *pset, const char *context);

bool power_SetUniqueID(Character *pchar, Power *ppow, int uid, Character *oldCharacter);
Power *powerset_AddNewPower(PowerSet *pset, const BasePower *ppowBase, int uniqueID, Character *oldCharacter);
Power *powerset_SetNewPowerAt(PowerSet *pset, const BasePower *ppowBase, int idx, const char *context, int uniqueID, Character *oldCharacter);
void powerset_DeletePower(PowerSet *pset, Power *ppow, const char *context);
void powerset_DeleteBasePower(PowerSet *pset, const BasePower *ppowBase, const char *context);
void powerset_DeletePowerAt(PowerSet *pset, int idx, const char *context);

int powerset_BasePowerAvailableByIdx(PowerSet *pset, int iIdx, int iLevel);

bool powerset_ShowInInventory(PowerSet *pset);


/* Power ************************************************************/

Power *power_Create(const BasePower *ppowBase);
void power_Destroy(Power *ppow, const char *context);
void power_LevelFinalize(Power *ppow);

void power_AddBoostSlot(Power *ppow);
void power_RemoveBoostSlot(Power *ppow);
void power_ReplaceBoost(Power *ppow, int iIdx, const BasePower *pbase, int iLevel, int iNumCombines, float const *afVars, int numVars, PowerupSlot const *aPowerupSlots, int numPowerupSlots, const char *context);
void power_ReplaceBoostFromBoost(Power *ppow, int iIdx, Boost const *srcBoost, const char *context);

Boost *power_RemoveBoost(Power *ppow, int iIdx, const char *context);
void power_DestroyBoost(Power *ppow, int iIdx, const char *context);
void power_DestroyAllBoosts(Power *ppow, const char *context);
void power_DestroyAllChanceMods(Power *ppow);
float power_GetRadius(Power *ppow);

#ifdef SERVER
bool power_CheckUsageLimits(Power *ppow);
void power_ChangeActiveStatus(Power *ppow, int active);
void power_ChangeRechargeTimer(Power *ppow, float rechargeTime);
#endif

bool power_GetIndices(const Power *ppow, int *piSet, int *piPow);
void power_CombineBoosts( Character *pchar, Boost *pbA, Boost *pbB );
void power_BoosterBoost( Character *pchar, Boost *pbA );
void power_CatalyzeBoost( Character *pchar, Boost *pbA );

bool power_BoostCheckRequires(Power *ppow, Boost *pBoost);
bool power_sloteval_Eval(Power *pPower, Boost *pBoost, Entity *e, const char * const *ppchExpr);

bool power_ShowInInventory(Character *pchar, Power *ppow);

// Return the custom tint for the specified power based if the powerset is
// tinted for the owning character's current costume.  Return colorPairNone
// otherwise.
ColorPair power_GetTint(Power* power);

// Return whether the power has a custom tint associated.
bool power_IsTinted(Power* power);

const PowerFX* power_GetFX(Power* power);
const PowerFX* power_GetFXFromToken(const BasePower *power, const char * token);
const char* power_GetToken(Power* power);
const char* power_GetRootToken(Power* power);
Power* power_GetPetCreationPower(Character* character);

#if SERVER
void power_ClearBoostTimers(Power *ppow);
void power_IncrementBoostTimers(Power *ppow);
void power_DecrementBoostTimers(Power *ppow, float fRate);

bool power_AddPowerupslot( Power *b, RewardSlot *slot );
#endif

bool character_checkPowersAndRemoveBadPowers( Character * p );

// Can't be 1 or 0
#define ACTIVE_STATUS_QUEUE_CHANGED 2


/* PowerRef *******************************************************/

PowerRef* powerRef_Create(void);
PowerRef* powerRef_CreateFromPower(int buildNum, Power *ppow);
PowerRef* powerRef_CreateFromIdxs(int iIdxBuild, int iIdxSet, int iIdxPow);
void powerRef_Destroy(PowerRef* ppowRef);


/* PowerList ******************************************************/

PowerListItem *powlist_GetNext(PowerListIter *piter);
PowerListItem *powlist_GetFirst(PowerList *plist, PowerListIter *piter);
void powlist_RemoveCurrent(PowerListIter *piter);
void powlist_Add(PowerList *plist, Power *ppow, unsigned int uid);
void powlist_AddWithTargets(PowerList *plist, Power *ppow, EntityRef erTarget, Vec3 vecTarget, unsigned int invokeID);
void powlist_RemoveAll(PowerList *plist);


/* Inspiration ******************************************************/

Power *inspiration_Create(const BasePower *ppowBase, Character *p, int idx);
void inspiration_Destroy(Power *ppow);
bool inspiration_IsTradeable(const BasePower *power, const char *authFrom, const char *authTo);

#ifdef POWERS_PARSE_INFO_DEFINITIONS
#define POWERS_PARSE_INFO_DEFINED
#endif


/* ETC ******************************************************/

// some functions to quickly translate between base power names and the base set group name
void mapBoostSetGroupNameToBasePowerName(const BasePower *ppow, BoostSetDictionary *bdict);
const char *boostSetGroupNameFromBasePowerName(const char *basePowerName);
const char *boostSetNameFromBasePowerName(const char *basePowerName);

// when enhancements become obsolete, these functions replace the old names with the new ones
void power_LoadReplacementHash(void);
const char *power_GetReplacementPowerSetName(const char *pchPowerSetName);
const char *power_GetReplacementPowerName(const char *pchPowerName);
void power_findRedirection(Entity *src, Entity *target, Power *power);
const BasePower *basepower_GetReplacement(const BasePower *ppowBase);

int basepower_GetAIMaxLevel(const BasePower *pBasePower);
int basepower_GetAIMinLevel(const BasePower *pBasePower);
int basepower_GetAIMaxRankCon(const BasePower *pBasePower);
int basepower_GetAIMinRankCon(const BasePower *pBasePower);
int basepower_GetMaxDifficulty(const BasePower *pBasePower);
int basepower_GetMinDifficulty(const BasePower *pBasePower);
void power_verifyValidMMPowers();

float basepower_CalculateAreaFactor(const BasePower *ppowBase);

#endif /* #ifndef POWERS_H__ */

/* End of File */

