/***************************************************************************
 *     Copyright (c) 2003-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#if (!defined(ATTRIBMOD_H__)) || (defined(ATTRIBMOD_PARSE_INFO_DEFINITIONS)&&!defined(ATTRIBMOD_PARSE_INFO_DEFINED))
#define ATTRIBMOD_H__

// ATTRIBMOD_PARSE_INFO_DEFINITIONS must be defined before including this file
// to get the ParseInfo structures needed to read in files.

#include <stdlib.h>   // for offsetof

#include <utilitieslib/stdtypes.h>  // for bool
#include "entityRef.h" // for EntityRef
#include <utilitieslib/utils/Color.h>

typedef struct Character Character;
typedef struct Power Power;
typedef struct CharacterAttributes CharacterAttributes;
typedef struct CharacterAttribSet CharacterAttribSet;
typedef struct BasePower BasePower;
typedef struct PowerFX PowerFX;
typedef int BoostType;

/***************************************************************************/
/***************************************************************************/

#define ASPECT_ATTRIB_GET_PTR(pattribset, offAspect, offAttr) \
    ((float *)((*(char **)(((char *)(pattribset))+(offAspect)))+(offAttr)))

#define ATTRIB_GET_PTR(pattr, offAttr) \
    ((float *)(((char *)(pattr))+(offAttr)))

#define ATTRIBMOD_DURATION_FOREVER (999999)
    // A very large number of seconds which is essentially forever. This is
    // used as a flag; anything larger than or equal to this value will
    // be handled specially.

#define ACTIVATE_PERIOD_FOREVER (99999)
    // A smaller but still very large number of seconds which is essentially forever.
    // Powers which only have ATTRIBMOD_DURATION_FOREVER mods should have an activation period
    // of this or greater.

/***************************************************************************/
/***************************************************************************/

typedef enum ModApplicationType
{
    kModApplicationType_OnTick,            // while the power is running
    kModApplicationType_OnActivate,        // when the power is turned on
    kModApplicationType_OnDeactivate,    // when the power is turned off
    kModApplicationType_OnExpire,        // a limited version of onDeactivate

    // ARM NOTE:  There are some strange edge cases around character_Reset() and related
    //   functionality where enable/disable appear to be triggering more often than they should,
    //   and in improper order.  At some point we're going to need to fix those edge cases,
    //   but I've invested enough time into this right now that I'm just going to let them be.
    //   For current usage (just granting a power on enable and revoking the power on disable),
    //   the current code appears to have the correct output, even if it doesn't get there in
    //   any understandable way...
    kModApplicationType_OnEnable,        // when the power becomes able to be turned on
    kModApplicationType_OnDisable,        // when the power becomes no longer able to be turned on
    
    // Keep this last
    kModApplicationType_Count,
} ModApplicationType;

typedef enum ModTarget
{
    kModTarget_Caster,
    kModTarget_CastersOwnerAndAllPets,
    kModTarget_Focus,
    kModTarget_FocusOwnerAndAllPets,
    kModTarget_Affected,
    kModTarget_AffectedsOwnerAndAllPets,
    kModTarget_Marker,

    // Keep this last
    kModTarget_Count,
} ModTarget;

typedef enum ModType
{
    kModType_Duration,
    kModType_Magnitude,
    kModType_Constant,
    kModType_Expression,
    kModType_SkillMagnitude,

    // Keep this last
    kModType_Count,
} ModType;

typedef enum CasterStackType
{
    kCasterStackType_Individual,    // Stacking is handled for each caster individually
    kCasterStackType_Collective,    // Stacking is handled for all casters collectively

    // Keep this last
    kCasterStackType_Count
} CasterStackType;

typedef enum StackType
{
    // Determines how multiple identical AttribMods from the same power and
    //   caster are handled.

    kStackType_Stack,            // Stack up (allow multiples)
    kStackType_Ignore,            // Ignore the new duplicate (do nothing)
    kStackType_Extend,            // Update the parameters in and extend the existing AttribMod
    kStackType_Replace,            // Update the parameters and replace the existing AttribMod
    kStackType_Overlap,            // Update the parameters in the existing AttribMod, don't extend the duration
    kStackType_StackThenIgnore,    // Stack up to StackCount times (if count < StackCount, then stack, else ignore)
    kStackType_Refresh,            // Update the duration in all copies of the existing matching AttribMods, then add a new copy on
    kStackType_RefreshToCount,    // If count < StackCount, then Refresh and add a new copy, else just Refresh
    kStackType_Maximize,        // If mag is greater Replace, else Ignore
    kStackType_Suppress,        // Keep all, but suppress all but the greatest magnitude

    // Keep this last
    k_StackType_Count
} StackType;

typedef enum PowerEvent
{
    kPowerEvent_Activate,    // Must be first invoke-related event
    kPowerEvent_ActivateAttackClick,
    kPowerEvent_Attacked,
    kPowerEvent_AttackedNoException,
    kPowerEvent_Helped,
    kPowerEvent_Hit,
    kPowerEvent_Miss,
    kPowerEvent_EndActivate, // Must be last invoke-related event

    kPowerEvent_AttackedByOther, // Must be the first apply-related event
    kPowerEvent_AttackedByOtherClick,
    kPowerEvent_HelpedByOther,
    kPowerEvent_HitByOther,
    kPowerEvent_HitByFriend,
    kPowerEvent_HitByFoe,
    kPowerEvent_MissByOther,
    kPowerEvent_MissByFriend,
    kPowerEvent_MissByFoe,       // Must be the last apply-related event

    kPowerEvent_Damaged,
    kPowerEvent_Healed,

    kPowerEvent_Stunned,     // Must be first status event for macro below
    kPowerEvent_Immobilized,
    kPowerEvent_Held,
    kPowerEvent_Sleep,
    kPowerEvent_Terrorized,
    kPowerEvent_Confused,
    kPowerEvent_Untouchable,
    kPowerEvent_Intangible,
    kPowerEvent_OnlyAffectsSelf,
    kPowerEvent_AnyStatus,   // Leave as last status event

    kPowerEvent_Knocked,

    kPowerEvent_Defeated,

    kPowerEvent_MissionObjectClick,

    kPowerEvent_Moved,

    kPowerEvent_Defiant,

    // Keep this last
    kPowerEvent_Count
} PowerEvent;

#define POWEREVENT_STATUS(x) ((x)>=kPowerEvent_Stunned && (x)<kPowerEvent_AnyStatus)
#define POWEREVENT_INVOKE(x) ((x)>=kPowerEvent_Activate && (x)<=kPowerEvent_EndActivate)
#define POWEREVENT_APPLY(x) ((x)>=kPowerEvent_AttackedByOther && (x)<=kPowerEvent_MissByFoe)

/***************************************************************************/
/***************************************************************************/

typedef struct SuppressPair
{
    int idxEvent;
        // The index of the event to check. (See PowerEvent enum in character_base.h)

    U32 ulSeconds;
        // How many seconds it must be after the event before this attribmod
        //   is allowed to go off.

    bool bAlways;
        // If true, the attribmod will always be suppressed when in the
        //  event window. If false, then if the attribmod has already been
        //  applied once, it continue to gets applied.

} SuppressPair;

typedef struct AttribModTemplate
{
    // This defines an actual effect of a power. A power may have multiple
    //   AttribModTemplates. When a power is used, these AMTemplates are
    //   pared down to AttribMods and attached to the targeted character.

    const char *pchName;
        // Internal name of effect (for matching later on)

    int iIdx;
        // Index of this template in the power.

    const char *pchDisplayAttackerHit;
        // Message displayed to the attacker when he hits with this power.

    const char *pchDisplayVictimHit;
        // Message displayed to the victim when he gets hits with this power.

    const char *pchDisplayFloat;
        // Message displayed over the victim's head when this attrib mod
        //   goes off.

    const char *pchDisplayDefenseFloat;
        // Message displayed over the victim's head when this attrib mod
        //   is the defense that caused some attack to miss the victim.

    bool bShowFloaters;
        // If true, shows floaters (damage and healing numbers, for example)
        //   over the affected's head. If specified, pchDisplayFloat is always
        //   shown, even if this is false. 
    
    const BasePower *ppowBase;
        // The power which contains this AttribModTemplate

    int ppowBaseIndex;
        // The index into the power's list of attrib mods that corresponds to this mod

    ModTarget eTarget;
        // Who is the target of this particular modifier.

    const char *pchTable;
        // The name of the table to use for scaling the power by level.
        // Tables are defined in the class.

    int iVarIndex;
        // If >=0, then use a instance-specific value instead of the table.

    float fScale;
        // How much to scale the basic value given by the class table for
        //   the given attribute.

    size_t offAttrib;
        // Byte offset to the attribute in the CharacterAttributes struct.

    size_t offAspect;
        // Byte offset to the structure in the Character to the CharacterAttributes
        //   to modify

    bool bIgnoresBoostDiminishing;
        // Determines whether this attribmod ignores diminishing returns on boosts (aka Enhancement Diversification).
        // only used if aspect is Strength.

    ModApplicationType eApplicationType;
        // Determines when this attrib mod is applied during the lifecycle of a power.

    ModType eType;
        // Determines if the duration or the magnitude is what is calculated.

    float fDuration;
        // How long the effect lasts on the target. Booleans calculate this
        //   value, others use it directly.

    float fProcsPerMinute;
        // Boosts: If set, this will cause the chance to activate to be calculated automatically to attempt to set the 
        //                this boost to active this number of times per minute.

    const char **ppchDuration;
        // An expression which calculates the duration of the attribmod. 
        //   If empty, the fDuration field is used instead.

    bool bUseDurationCombatMods;
        // If true, the level difference between the source and the target
        //   are used to modify the effect's duration.
        //   Only used for ModType_Expression.

    bool bUseDurationResistance;
        // If true, the target's resistance is used to modify the scale
        //   of the applied duration.

    float fMagnitude;
        // Default for how much to change the attribute. Booleans use this
        //   value, others calculate it.

    const char **ppchMagnitude;
        // An expression which calculates the magnitude of the attribmod. 
        //   If empty, the fMagnitude field is used instead.
        //   Only used for ModType_Expression.

    bool bUseMagnitudeCombatMods;
        // If true, the level difference between the source and the target
        //   are used to modify the effect's magnitude.

    bool bUseMagnitudeResistance;
        // If true, the target's resistance is used to modify the scale
        //   of the applied magnitude.

    float fRadiusInner;
    float fRadiusOuter;
        // If the target is between these two radii, then they will be
        //   affected by the AttribMod. By default, all things
        //   withing the target area are hit.

    float fDelay;
        // How long to wait before applying the attrib modifier for the
        //   first time.

    float fPeriod;
        // The attrib modifier is applied every fPeriod seconds.

    float fChance;
        // The chance that this attrib modifier will be applied to the
        //   target. (1.0 == 100%)

    bool bCancelOnMiss;
        // If true and the test governed by fChance above fails, then
        //   the attribute modifier will be expired.

    const int *piCancelEvents;
        // A list of PowerEvents which will cancel this attribmod outright.

    bool bNearGround;
        // If true, only applies the attrib modifier if the target is on
        //   the ground.

    bool bAllowStrength;
        // If true, the attacker's strength is used to modify the scale
        //   of the effects. (Usually true. Boosts are probably false.)

    bool bAllowResistance;
        // If true, the target's resistance is used to modify the scale
        //   of the applied effects. (Usually true. Boosts are probably false.)

    bool bAllowCombatMods;
        // If true, the level difference between the source and the target
        //   are used to modify the effect's magnitude and/or duration.

    bool bBoostTemplate;
        // If true, this is supposed to be a boost template.  This is used
        //   in boosts and powers used as set bonuses, which can include both
        //   boost templates and additional normal templates.

    bool bMatchExactPower;
        // If true, this power will only match the EXACT power that applied this mod 
        //   for replacement purposes.

    bool bDisplayTextOnlyIfNotZero;
        // If true and pchDisplayFloat, pchDisplayAttackerHit or pchDisplayVictimeHit 
        //        is specified, it will only display if the
        //        attribute evaluates to a non-zero value.

    const char **ppchApplicationRequires;
        // An expression which describes the conditions under which the
        //   template may be applied. If there is no expression, then the
        //   template is always applied. This is used for special powers
        //   which have very special effects against certain targets.

    CasterStackType eCasterStack;
        // Determines how identical AttribMods from the same power
        //   but from different casters are handled.

    StackType eStack;
        // Determines how multiple AttribMods that pass the CasterStackType check are handled.

    int iStackLimit;
        // Used for kStackType_StackThenIgnore.
        // Determines how many times the AttribMod should stack before it is ignored.

    int iStackKey;
        // If this is not zero, we compare this instead of stacking by the template.

    const SuppressPair **ppSuppress;
        // An earray of events to check against to determine if this attribmod
        //   is allowed to go off. This doesn't reject the attribmod entirely
        //   (like the Requires does). If the time passes after the event and
        //   the attribmod still has time left on it, it will work.
        
    const char *pchIgnoreSuppressErrors;
        // If npt empty, don't check the suppression list against the attribmods
        //   of the power.

    bool bSuppressMethodAlways;
        // Determined from the bAlways tags in ppSuppress.  If all suppressions
        //   have a matching bAlways tag, we set this appropriately.  If there 
        //   is a mix, we complain loudly.

    const int *piContinuingBits;
        // Sets the given bits for the lifetime of the AttribMod.

    const char *pchContinuingFX;
        // If non-NULL, plays maintained FX for the lifetime of the AttribMod.
        //   When it times out, the FX is killed.

    const int *piConditionalBits;
        // Sets the given bits while the AttribMod is alive and the attrCur
        //   for the modified attribute is greater than zero.

    const char *pchConditionalFX;
        // If non-NULL, plays maintained FX while the AttribMod is alive and
        //   the attrCur for the modified attribute is greater than zero.

    const char *pchCostumeName;
        // For powers which change the player's costume, the name of the
        //   NPC costume to be used.
    
    const char *pchReward;
        // For powers which give a reward or a power, then name of the reward 
        //   table to apply or power name (Cat.Set.Name) to give.

    const char *pchParams;
        // Additional parameters for powers.

    //
    // Entity Spawn attributes
    //

    const char *pchEntityDef;
        // For powers which spawn entities, the name of the VillainDef which
        //   defines the basic entity being spawned.

    const char *pchPriorityListOffense;
        // For powers which spawn entities, the name of the priority list
        //   the new entity will run when in offensive mode.

    const char *pchPriorityListDefense;
        // For powers which spawn entities, the name of the priority list
        //   the new entity will run when in defensive mode.

    const char *pchPriorityListPassive;
        // For powers which spawn entities, the name of the priority list
        //   the new entity will run when in passive mode.
        // This is the default when a pet is created.

    bool bVanishEntOnTimeout;
        // If true, if the pet times out or is otherise destroyed by the 
        //   server (as opposed to being defeated) then the entity is
        //   vanished as opposed to going through the usual DieNow code.
        // (Only for powers which spawn entities.)

    bool bDoNotTintCostume;
        // If true, do not apply custom tinting to the spawned pet's costume.

    bool bKeepThroughDeath;
        // If true, do not clean out this attribmod when the entity dies

    bool bDelayEval;
        // If true, delay any evaluations associated with this attribmod until the last possible moment in mod_process
        // this means you will have to store all the Eval stashes until that time comes

    U32    evalFlags;
        // Flags created at bin time based upon what special combat eval parameters need to be pushed for evaluation

    BoostType boostModAllowed;
        // If specified, a power must specifically allow this BoostType in order for this mod to apply as part of a Boost, even if the Boost itself applies
        // This is for Boosts with mixed BoostTypes such as Hami-Os where Damage boosts can be slotted into Damage Resist powers for exploitative gain

    char **ppchPrimaryStringList;
    char **ppchSecondaryStringList;
} AttribModTemplate;

// AtrribModTemplate evalFlags
#define ATTRIBMOD_EVAL_HIT_ROLL        (1 << 0)
#define ATTRIBMOD_EVAL_CALC_INFO    (1 << 1)
#define ATTRIBMOD_EVAL_CUSTOMFX        (1 << 2)
#define ATTRIBMOD_EVAL_TARGETSHIT    (1 << 3)
#define ATTRIBMOD_EVAL_CHANCE        (1 << 4)

#ifdef ATTRIBMOD_PARSE_INFO_DEFINITIONS

extern StaticDefine ParsePowerDefines[]; // defined in load_def.c

//
// All these things with ks on the front probably don't need them any more.
//   This is a leftover from when they were globally defined and they needed
//   to be mutually exclusive.
//

StaticDefineInt ModApplicationEnum[] =
{
    DEFINE_INT
    { "kOnTick",                kModApplicationType_OnTick            },
    { "kOnActivate",            kModApplicationType_OnActivate        },
    { "kOnDeactivate",            kModApplicationType_OnDeactivate    },
    { "kOnExpire",                kModApplicationType_OnExpire        },
    { "kOnEnable",                kModApplicationType_OnEnable        },
    { "kOnDisable",                kModApplicationType_OnDisable        },
    // The following are deprecated, please use the above instead.
    { "kOnIncarnateEquip",        kModApplicationType_OnEnable        },
    { "kOnIncarnateUnequip",    kModApplicationType_OnDisable        },
    DEFINE_END
};

StaticDefineInt DurationEnum[] =
{
    DEFINE_INT
    { "kInstant",            -1  },
    { "kUntilKilled",        ATTRIBMOD_DURATION_FOREVER },
    { "kUntilShutOff",       ATTRIBMOD_DURATION_FOREVER },
    DEFINE_END
};

StaticDefineInt ModTargetEnum[] =
{
    DEFINE_INT
    { "kCaster",                        kModTarget_Caster                        },
    { "kCastersOwnerAndAllPets",        kModTarget_CastersOwnerAndAllPets        },
    { "kFocus",                            kModTarget_Focus                        },
    { "kFocusOwnerAndAllPets",            kModTarget_FocusOwnerAndAllPets            },
    { "kAffected",                        kModTarget_Affected                        },
    { "kAffectedsOwnerAndAllPets",        kModTarget_AffectedsOwnerAndAllPets        },
    { "kMarker",                        kModTarget_Marker                        },
    // the below are deprecated, please use the above from now on...
    { "kSelf",                            kModTarget_Caster                        },
    { "kSelfsOwnerAndAllPets",            kModTarget_CastersOwnerAndAllPets        },
    { "kTarget",                        kModTarget_Affected                        },
    { "kTargetsOwnerAndAllPets",        kModTarget_AffectedsOwnerAndAllPets        },
    DEFINE_END
};

StaticDefineInt ModTypeEnum[] =
{
    DEFINE_INT
    { "kDuration",           kModType_Duration        },
    { "kMagnitude",          kModType_Magnitude       },
    { "kConstant",           kModType_Constant        },
    { "kExpression",         kModType_Expression      },
    { "kSkillMagnitude",     kModType_SkillMagnitude  },
    DEFINE_END
};

StaticDefineInt CasterStackTypeEnum[] =
{
    DEFINE_INT
    { "kIndividual",        kCasterStackType_Individual    },
    { "kCollective",        kCasterStackType_Collective },
    DEFINE_END
};

StaticDefineInt StackTypeEnum[] =
{
    DEFINE_INT
    { "kStack",                kStackType_Stack   },
    { "kIgnore",            kStackType_Ignore  },
    { "kExtend",            kStackType_Extend  },
    { "kReplace",            kStackType_Replace },
    { "kOverlap",            kStackType_Overlap },
    { "kStackThenIgnore",    kStackType_StackThenIgnore },
    { "kRefresh",            kStackType_Refresh },
    { "kRefreshToCount",    kStackType_RefreshToCount },
    { "kMaximize",            kStackType_Maximize },
    { "kSuppress",            kStackType_Suppress },
    DEFINE_END
};

StaticDefineInt AspectEnum[] =
{
    DEFINE_INT
    // Aspects
    { "kCurrent",            offsetof(CharacterAttribSet, pattrMod)           },
    { "kMaximum",            offsetof(CharacterAttribSet, pattrMax)           },
    { "kStrength",           offsetof(CharacterAttribSet, pattrStrength)      },
    { "kResistance",         offsetof(CharacterAttribSet, pattrResistance)    },
    { "kAbsolute",           offsetof(CharacterAttribSet, pattrAbsolute)      },
    { "kCurrentAbsolute",    offsetof(CharacterAttribSet, pattrAbsolute)      },
    { "kCur",                offsetof(CharacterAttribSet, pattrMod)           },
    { "kMax",                offsetof(CharacterAttribSet, pattrMax)           },
    { "kStr",                offsetof(CharacterAttribSet, pattrStrength)      },
    { "kRes",                offsetof(CharacterAttribSet, pattrResistance)    },
    { "kAbs",                offsetof(CharacterAttribSet, pattrAbsolute)      },
    { "kCurAbs",             offsetof(CharacterAttribSet, pattrAbsolute)      },
    DEFINE_END
};

StaticDefineInt ModBoolEnum[] =
{
    DEFINE_INT
    { "false",               0  },
    { "kFalse",              0  },
    { "true",                1  },
    { "kTrue",               1  },
    DEFINE_END
};

StaticDefineInt PowerEventEnum[] =
{
    DEFINE_INT
    { "Activate",             kPowerEvent_Activate        },
    { "ActivateAttackClick",  kPowerEvent_ActivateAttackClick },
    { "EndActivate",          kPowerEvent_EndActivate     },

    { "Hit",                  kPowerEvent_Hit             },
    { "HitByOther",           kPowerEvent_HitByOther      },
    { "HitByFriend",          kPowerEvent_HitByFriend     },
    { "HitByFoe",             kPowerEvent_HitByFoe        },
    { "Miss",                 kPowerEvent_Miss            },
    { "MissByOther",          kPowerEvent_MissByOther     },
    { "MissByFriend",         kPowerEvent_MissByFriend    },
    { "MissByFoe",            kPowerEvent_MissByFoe       },
    { "Attacked",             kPowerEvent_Attacked        },
    { "AttackedNoException",  kPowerEvent_AttackedNoException  },

    { "AttackedByOther",      kPowerEvent_AttackedByOther },
    { "AttackedByOtherClick", kPowerEvent_AttackedByOtherClick },
    { "Helped",               kPowerEvent_Helped          },
    { "HelpedByOther",        kPowerEvent_HelpedByOther   },

    { "Damaged",              kPowerEvent_Damaged         },
    { "Healed",               kPowerEvent_Healed          },

    { "Stunned",              kPowerEvent_Stunned         },
    { "Stun",                 kPowerEvent_Stunned         },
    { "Immobilized",          kPowerEvent_Immobilized     },
    { "Immobilize",           kPowerEvent_Immobilized     },
    { "Held",                 kPowerEvent_Held            },
    { "Sleep",                kPowerEvent_Sleep           },
    { "Terrorized",           kPowerEvent_Terrorized      },
    { "Terrorize",            kPowerEvent_Terrorized      },
    { "Confused",             kPowerEvent_Confused        },
    { "Confuse",              kPowerEvent_Confused        },
    { "Untouchable",          kPowerEvent_Untouchable     },
    { "Intangible",           kPowerEvent_Intangible      },
    { "OnlyAffectsSelf",      kPowerEvent_OnlyAffectsSelf },
    { "AnyStatus",            kPowerEvent_AnyStatus       },

    { "Knocked",              kPowerEvent_Knocked         },

    { "Defeated",             kPowerEvent_Defeated        },

    { "MissionObjectClick",   kPowerEvent_MissionObjectClick },

    { "Moved",                kPowerEvent_Moved   },

    { "Defiant",              kPowerEvent_Defiant },

    DEFINE_END
};

StaticDefineInt SupressAlwaysEnum[] =
{
    DEFINE_INT
    { "WhenInactive",       0  },
    { "Always",             1  },
    DEFINE_END
};

TokenizerParseInfo ParseSuppressPair[] =
{
    { "Event",      TOK_STRUCTPARAM|TOK_INT(SuppressPair, idxEvent, 0), PowerEventEnum  },
    { "Seconds",    TOK_STRUCTPARAM|TOK_INT(SuppressPair, ulSeconds, 0) },
    { "Always",     TOK_STRUCTPARAM|TOK_BOOL(SuppressPair, bAlways, 0), SupressAlwaysEnum },
    { "\n",         TOK_END,                         0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseAttribModTemplate[] =
{
    { "{",                   TOK_START,  0 },
    { "Name",                TOK_STRING(AttribModTemplate, pchName, 0)                 },
    { "DisplayAttackerHit",  TOK_STRING(AttribModTemplate, pchDisplayAttackerHit, 0)   },
    { "DisplayVictimHit",    TOK_STRING(AttribModTemplate, pchDisplayVictimHit, 0)     },
    { "DisplayFloat",        TOK_STRING(AttribModTemplate, pchDisplayFloat, 0)         },
    { "DisplayAttribDefenseFloat", TOK_STRING(AttribModTemplate, pchDisplayDefenseFloat, 0)  },
    { "ShowFloaters",        TOK_BOOL(AttribModTemplate, bShowFloaters, 1), ModBoolEnum},

    { "Attrib",              TOK_AUTOINT(AttribModTemplate, offAttrib, 0), ParsePowerDefines },
    { "Aspect",              TOK_AUTOINT(AttribModTemplate, offAspect, 0), AspectEnum   },
    { "BoostIgnoreDiminishing",    TOK_BOOL(AttribModTemplate, bIgnoresBoostDiminishing, 0), ModBoolEnum},

    { "Target",              TOK_INT(AttribModTemplate, eTarget, kModTarget_Affected), ModTargetEnum },
    { "Table" ,              TOK_STRING(AttribModTemplate, pchTable, 0) },
    { "Scale",               TOK_F32(AttribModTemplate, fScale, 0) },

    { "ApplicationType",        TOK_INT(AttribModTemplate, eApplicationType, kModApplicationType_OnTick), ModApplicationEnum },
    { "Type",                    TOK_INT(AttribModTemplate, eType, kModType_Magnitude), ModTypeEnum },
    { "Delay",                    TOK_F32(AttribModTemplate, fDelay, 0)             },
    { "Period",                    TOK_F32(AttribModTemplate, fPeriod, 0)            },
    { "Chance",                    TOK_F32(AttribModTemplate, fChance, 0)            },
    { "CancelOnMiss",            TOK_BOOL(AttribModTemplate, bCancelOnMiss, 0), ModBoolEnum },
    { "CancelEvents",            TOK_INTARRAY(AttribModTemplate, piCancelEvents), PowerEventEnum },
    { "NearGround",                TOK_BOOL(AttribModTemplate, bNearGround, 0), ModBoolEnum },
    { "AllowStrength",            TOK_BOOL(AttribModTemplate, bAllowStrength, 1), ModBoolEnum },
    { "AllowResistance",        TOK_BOOL(AttribModTemplate, bAllowResistance, 1), ModBoolEnum },
    { "UseMagnitudeResistance",    TOK_BOOL(AttribModTemplate, bUseMagnitudeResistance, -1), ModBoolEnum },
    { "UseDurationResistance",    TOK_BOOL(AttribModTemplate, bUseDurationResistance, -1), ModBoolEnum },
    { "AllowCombatMods",        TOK_BOOL(AttribModTemplate, bAllowCombatMods, 1), ModBoolEnum },
    { "UseMagnitudeCombatMods",    TOK_BOOL(AttribModTemplate, bUseMagnitudeCombatMods, -1), ModBoolEnum },
    { "UseDurationCombatMods",    TOK_BOOL(AttribModTemplate, bUseDurationCombatMods, -1), ModBoolEnum },
    { "BoostTemplate",            TOK_BOOL(AttribModTemplate, bBoostTemplate, 0), ModBoolEnum },
    { "Requires",                TOK_STRINGARRAY(AttribModTemplate, ppchApplicationRequires)       },
    { "PrimaryStringList",        TOK_STRINGARRAY(AttribModTemplate, ppchPrimaryStringList)       },
    { "SecondaryStringList",    TOK_STRINGARRAY(AttribModTemplate, ppchSecondaryStringList)       },
    { "CasterStackType",        TOK_INT(AttribModTemplate, eCasterStack, kCasterStackType_Individual), CasterStackTypeEnum },
    { "StackType",                TOK_INT(AttribModTemplate, eStack, kStackType_Replace), StackTypeEnum },
    { "StackLimit",                TOK_INT(AttribModTemplate, iStackLimit, 2)        },
    { "StackKey",                TOK_INT(AttribModTemplate, iStackKey, -1), ParsePowerDefines        },
    { "Duration",                TOK_F32(AttribModTemplate, fDuration, -1), DurationEnum  },
    { "DurationExpr",            TOK_STRINGARRAY(AttribModTemplate, ppchDuration)       },
    { "Magnitude",                TOK_F32(AttribModTemplate, fMagnitude, 0), ParsePowerDefines },
    { "MagnitudeExpr",            TOK_STRINGARRAY(AttribModTemplate, ppchMagnitude)      },
    { "RadiusInner",            TOK_F32(AttribModTemplate, fRadiusInner, -1) },
    { "RadiusOuter",            TOK_F32(AttribModTemplate, fRadiusOuter, -1) },

    { "Suppress",            TOK_STRUCT(AttribModTemplate, ppSuppress, ParseSuppressPair) },
    { "IgnoreSuppressErrors",TOK_STRING(AttribModTemplate, pchIgnoreSuppressErrors, 0)},

    { "ContinuingBits",      TOK_INTARRAY(AttribModTemplate, piContinuingBits), ParsePowerDefines },
    { "ContinuingFX",        TOK_FILENAME(AttribModTemplate, pchContinuingFX, 0)    },
    { "ConditionalBits",     TOK_INTARRAY(AttribModTemplate, piConditionalBits), ParsePowerDefines },
    { "ConditionalFX",       TOK_FILENAME(AttribModTemplate, pchConditionalFX, 0)   },

    { "CostumeName",         TOK_FILENAME(AttribModTemplate, pchCostumeName, 0)     },

    { "Power",               TOK_REDUNDANTNAME|TOK_STRING(AttribModTemplate, pchReward, 0)               },
    { "Reward",              TOK_STRING(AttribModTemplate, pchReward, 0)               },
    { "Params",                 TOK_STRING(AttribModTemplate, pchParams, 0)                },
    { "EntityDef",           TOK_STRING(AttribModTemplate, pchEntityDef, 0)            },
    { "PriorityListDefense", TOK_STRING(AttribModTemplate, pchPriorityListDefense, 0)  },
    { "PriorityListOffense", TOK_STRING(AttribModTemplate, pchPriorityListOffense, 0)  },
    { "PriorityListPassive", TOK_STRING(AttribModTemplate, pchPriorityListPassive, 0)  },
    { "PriorityList",        TOK_REDUNDANTNAME|TOK_STRING(AttribModTemplate, pchPriorityListPassive, 0)  },
    { "DisplayOnlyIfNotZero",TOK_BOOL(AttribModTemplate, bDisplayTextOnlyIfNotZero, 0), ModBoolEnum},
    { "MatchExactPower",     TOK_BOOL(AttribModTemplate, bMatchExactPower, 0), ModBoolEnum},
    { "VanishEntOnTimeout",  TOK_BOOL(AttribModTemplate, bVanishEntOnTimeout, 0), ModBoolEnum},
    { "DoNotTint",             TOK_BOOL(AttribModTemplate, bDoNotTintCostume, 0), ModBoolEnum},
    { "KeepThroughDeath",     TOK_BOOL(AttribModTemplate, bKeepThroughDeath, 0), ModBoolEnum},
    { "DelayEval",             TOK_BOOL(AttribModTemplate, bDelayEval, 0), ModBoolEnum},
    { "BoostModAllowed",     TOK_INT(AttribModTemplate, boostModAllowed, 0), ParsePowerDefines },
    { "EvalFlags",             TOK_INT(AttribModTemplate, evalFlags, 0)        },                        // created automatically at bin time from Requires statements
    { "ProcsPerMinute",         TOK_F32(AttribModTemplate, fProcsPerMinute, 0)         },

    { "}",                   TOK_END,      0 },
    { "", 0, 0 }
};
#endif

// combat eval relevant information stored for delayed Eval usage
typedef struct AttribModEvalInfo
{
    float fRand;
    float fToHit;
    bool bAlwaysHit;
    bool bForceHit;
    int targetsHit;

    float fFinal;
    float fVal;
    float fEffectiveness;
    float fStrength;

    float fChanceMods;
    float fChanceFinal;

    int refCount;

} AttribModEvalInfo;

/***************************************************************************/
/***************************************************************************/

typedef struct AttribMod
{
    // After a power hits a character, it is factored down into simple
    //   AttribMods which the target is tagged with. These AttribMods
    //   have a lifetime which eventually expires.

    struct AttribMod *next;
        // This must stay first so this works with genericlist.
    struct AttribMod *prev;
        // This must stay second so this works with genericlist.

    EntityRef erSource;
        // The entity which caused this modifier.

    EntityRef erCreated;
        // The entity which was created by this modifier, if any.

    Vec3 vecLocation;
        // The base location where this modifier was created, if any.

    unsigned int uiInvokeID;
        // And ID which increments every power invokation. Allows you
        //   to know which attribmods are part of the same power
        //   invokation.

    const AttribModTemplate *ptemplate;
        // A reference to the base template which this AttribMod came from.

    Power *parentPowerInstance;
        // A reference to the power instance that applied this attrib mod.
        // DO NOT set this value outside of attribmod_setParentPowerInstance()!
        // This is ONLY accurate (and only non-NULL) if this attrib mod was made in mod_Fill()!
        // When attrib mods are saved to the database, they lose this connection to a Power.

    const PowerFX *fx;
        // PowerFX information for this particular attribmod, changeable
        // with power customization.

    float fDuration;
        // How long the effect will last.

    float fTimer;
        // A timer used to count the time between applications of the power
        //    and delay the first application of the power.

    float fMagnitude;
        // How much to change the attribute.

    float fChance;
        // The chance that this attrib modifier will be applied to the
        //   target. (1.0 == 100%)

    float fCheckTimer;
        // If a character is defeated before the CheckTimer runs out, then
        //    the attribmod is removed. (Because it won't happen.)
    bool bSourceChecked;
        // If true, then the source entity has been checked to see if it is
        //    alive or not.

    bool bDelayEvalChecked;
        // If true, either this mod doesn't use DelayEval or it has
        //      processed the DelayEval.

    bool bReportedOnLastTick;
        // true if the attribmod was reported on last tick. Used to minimize
        //   the number of spurious messages. Thi is set up to go off each
        //   period of the atribmod.

    bool bContinuingFXOn;
        // True if the ContinuingFX and ContinuingBits are on for this
        //   attribmod.

    bool bIgnoreSuppress;
        // True if this AttribMod should no longer pay attention to suppression.

    bool bUseLocForReport;
        // True if this AttribMod should use the location for reporting floaters.

    ColorPair uiTint;
        // The tint color of the power which created the attribmod

    const char *customFXToken;
        // Token used to find/fire the appropriate fx

    int petFlags;
        // field use to help pets zone
    
    AttribModEvalInfo *evalInfo;

    bool bSuppressedByStacking;
        // True if this AttribMod is currently being suppressed due to kStackType_Suppress.

} AttribMod;


#define PETFLAG_TRANSFER    (1<<0)
#define PETFLAG_UPGRADE1    (1<<1)
#define PETFLAG_UPGRADE2    (1<<2)

/***************************************************************************/
/***************************************************************************/

typedef struct AttribModList
{
    // Implements a list of AttribMods.

    AttribMod *phead;
        // Points to the top of the list.

    AttribMod *ptail;
        // Points to the end of the list.
        // NOTE:  GenericList doesn't support tail pointers!
        // We have to do the tracking ourselves.

} AttribModList;

typedef struct AttribModListIter
{
    AttribModList *plist;

    AttribMod *pposNext;
    AttribMod *pposCur;
        // Variables for tracking where the iteration is. The iterator is
        //   designed so that the "current" AttribMod can safely be
        //   removed and the iteration can continue.
} AttribModListIter;

/***************************************************************************/
/***************************************************************************/

AttribMod *attribmod_Create(void);
void attribmod_Destroy(AttribMod *pmod);

AttribModEvalInfo *attribModEvalInfo_Create(void);
void attribModEvalInfo_Destroy(AttribModEvalInfo *evalInfo);

AttribMod *modlist_GetNextMod(AttribModListIter *piter);
AttribMod *modlist_GetFirstMod(AttribModListIter *piter, AttribModList *plist);
void modlist_RemoveAndDestroyCurrentMod(AttribModListIter *piter);

AttribMod *modlist_FindUnsuppressed(AttribModList *plist, const AttribModTemplate *ptemplate, Power *ppower, Character *pSrc);
AttribMod *modlist_FindLargestMagnitudeSuppressed(AttribModList *plist, const AttribModTemplate *ptemplate, Power *ppower, Character *pSrc);
int modlist_Count(AttribModList *plist, const AttribModTemplate *ptemplate, Power *ppower, Character *pSrc);
void modlist_AddMod(AttribModList *plist, AttribMod *pmod);
void modlist_AddModToTail(AttribModList *plist, AttribMod *pmod);
void modlist_RemoveAndDestroyMod(AttribModList *plist, AttribMod *pmod);
AttribMod* modlist_FindEarlierMod(AttribModList *plist, AttribMod *modA, AttribMod *modB);
void modlist_MoveModToTail(AttribModListIter *piter, AttribMod *mod);

void modlist_RemoveAll(AttribModList *plist);
void modlist_CancelAll(AttribModList *plist, Character *pchar);

void mod_Fill(AttribMod *pmod, Character *pTarget, Character *pSrc,
    unsigned int uiInvokeID, Power *ppow, const AttribModTemplate *ptemplate,
    const Vec3 vec, float fTimeBeforeHit, float fDelay, float fEffectiveness,
    float fToHitRoll, float fToHit, float fChance);

void modlist_RefreshAllMatching(AttribModList *plist, const AttribModTemplate *ptemplate, Power *ppower, Character *pSrc, float fDuration);

bool mod_Process(AttribMod *pmod, Character *pchar, CharacterAttribSet *pattribset,
    CharacterAttributes *pattrResBuffs, CharacterAttributes *pattrResDebuffs,
    CharacterAttributes *pResistance, bool *pbRemoveSleepMods, bool *pbDisallowDefeat, float fRate);

void mod_Cancel(AttribMod *pmod, Character *pchar);
void mod_CancelFX(AttribMod *pmod, Character *pchar);
void attribmod_SetParentPowerInstance(AttribMod *pmod, Power *ppow);

void HandleSpecialAttrib(AttribMod *pmod, Character *pchar, float f, bool *pbDisallowDefeat);

AttribMod *modlist_BlameDefenseMod(AttribModList *plist, float fPercentile, int iType);

typedef enum CombatMessageType
{
    CombatMessageType_DamagingMod,
    CombatMessageType_PowerActivation,
    CombatMessageType_PowerHitRoll,
} CombatMessageType;

/***************************************************************************/
/***************************************************************************/

#ifdef ATTRIBMOD_PARSE_INFO_DEFINITIONS
#define ATTRIBMOD_PARSE_INFO_DEFINED
#endif

#endif /* #ifndef ATTRIBMOD_H__ */

// These next two comments will not make sense unless you're here because you were 
// changing stuff in seqstate.h.  Just don't delete them, they do serve a purpose.

// This is a different line at the bottom of attribmod.h
// This is a line at the bottom of attribmod.h


/* End of File */
