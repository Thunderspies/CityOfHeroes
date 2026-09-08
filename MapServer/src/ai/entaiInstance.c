
#include <utilitieslib/stdtypes.h>
#include "ailib/aiBehaviorPublic.h"
#include "ailib/aiBehaviorInterface.h" // for AIVarsBase allocation
#include "seq/AnimBitList.h"
#include "beacon/beaconPath.h"
#include "entity/character_animfx.h"
#include "entity/character_base.h"
#include "entity/character_target.h"
#include "entity/character_pet.h"
#include "cmdparse/cmdcommon.h"
#include "cmdparse/cmdserver.h"
#include "entai.h"
#include "entaiBehaviorCoh.h"
#include "entaiCritterPrivate.h"
#include "entaiLog.h"
#include "entaiprivate.h"
#include "entaiPriority.h"
#include "entity/entGameActions.h"
#include "generator/entgen.h"
#include "entity/entity.h"
#include "entity/entserver.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/StringCache.h>
#include "entity/motion.h"
#include "seq/seq.h"
#include "svr/svr_tick.h"
#include <utilitieslib/utils/timing.h>
#include "cmdparse/cmdserver.h"
#include <utilitieslib/components/StringCache.h>
#include "ai/entai.h"
#include "ai/entaiLog.h"
#include <utilitieslib/components/earray.h>
#include "gameComm/VillainDef.h"
#include "entaiBehaviorAlias.h"
#include <utilitieslib/components/StashTable.h>
#include "entity/entPlayer.h"
#include "storyarc/mission.h"
#include "storyarc/playerCreatedStoryarcServer.h"
#include "entity/character_combat.h"
#include "entity/alignment_shift.h"

#include "dbcomm/dbcomm.h"
#include "dbcomm/staticMapInfo.h"

static const char* aiGetConfig(Entity* e, AIVars* ai);

int AI_POWERGROUP_BASIC_ATTACK;
int AI_POWERGROUP_FLY_EFFECT;
int AI_POWERGROUP_RESURRECT;
int AI_POWERGROUP_BUFF;
int AI_POWERGROUP_DEBUFF;
int AI_POWERGROUP_ROOT;
int AI_POWERGROUP_VICTOR; // Used when a critter is the victor in combat (defeats an opponent)
int AI_POWERGROUP_POSTDEATH; // Used when a critter is the defeated in combat (must be a castable after death power!)
int AI_POWERGROUP_HEAL_SELF;
int AI_POWERGROUP_TOGGLEHARM;
int AI_POWERGROUP_TOGGLEAOEBUFF;
int AI_POWERGROUP_HEAL_ALLY;
int AI_POWERGROUP_SUMMON;
int AI_POWERGROUP_KAMIKAZE;
int AI_POWERGROUP_PLANT;
int AI_POWERGROUP_TELEPORT;
int AI_POWERGROUP_EARLYBATTLE;
int AI_POWERGROUP_MIDBATTLE;
int AI_POWERGROUP_ENDBATTLE;
int AI_POWERGROUP_FORCEFIELD;
int AI_POWERGROUP_HELLIONFIRE;
int AI_POWERGROUP_PET;
int AI_POWERGROUP_SECONDARY_TARGET;

int AI_POWERGROUP_STAGES;


const char* aiBrainTypeName[AI_BRAIN_COUNT];

static StashTable nameToAIConfigTable = 0;
static StashTable nameToBrainTypeTable = 0;

static MemoryPool staticActivityDataPool = 0;

AIAllConfigs aiAllConfigs;

typedef enum AIConfigPowerAssignmentFromType {
    FROMTYPE_NAME,
    FROMTYPE_FLAG,
    FROMTYPE_NOFLAG,

    FROMTYPE_COUNT,
} AIConfigPowerAssignmentFromType;

static TokenizerParseInfo parseConfigPowerAssignmentByName[] = {
    { "",                                    TOK_STRUCTPARAM | TOK_STRING(AIConfigPowerAssignment,nameTo,0) },
    { "",                                    TOK_STRUCTPARAM | TOK_STRING(AIConfigPowerAssignment,nameFrom,0) },
    //--------------------------------------------------------------------------
    { "",                                    TOK_INT(AIConfigPowerAssignment,fromType, FROMTYPE_NAME) },
    //--------------------------------------------------------------------------
    { "\n",                                    TOK_END,            0 },
    //--------------------------------------------------------------------------
    { "", 0, 0 }
};

static TokenizerParseInfo parseConfigPowerAssignmentByFlag[] = {
    { "",                                    TOK_STRUCTPARAM | TOK_STRING(AIConfigPowerAssignment,nameTo,0) },
    { "",                                    TOK_STRUCTPARAM | TOK_STRING(AIConfigPowerAssignment,nameFrom,0) },
    //------------------------------------------------------------------------
    { "",                                    TOK_INT(AIConfigPowerAssignment,fromType, FROMTYPE_FLAG) },
    //------------------------------------------------------------------------
    { "\n",                                    TOK_END,            0 },
    //--------------------------------------------------------------------------
    { "", 0, 0 }
};

static TokenizerParseInfo parseConfigPowerAssignmentNoFlags[] = {
    { "",                                    TOK_STRUCTPARAM | TOK_STRING(AIConfigPowerAssignment,nameTo,0) },
    //--------------------------------------------------------------------------
    { "",                                    TOK_INT(AIConfigPowerAssignment,fromType, FROMTYPE_NOFLAG) },
    //--------------------------------------------------------------------------
    { "\n",                                    TOK_END,            0 },
    //--------------------------------------------------------------------------
    { "", 0, 0 }
};

static TokenizerParseInfo parseConfigPowerConfig[] = {
    { "{",                                    TOK_IGNORE,        0 },
    { "PowerName:",                            TOK_STRING(AIConfigPowerConfig,            powerName,0) },
    { "AllowedTarget:",                        TOK_STRINGARRAY(AIConfigPowerConfig,    allowedTargets)    },
    { "GroundAttack:",                        TOK_INT(AIConfigPowerConfig,            groundAttack,0)    },
    { "AllowMultipleBuffs:",                TOK_INT(AIConfigPowerConfig,            allowMultipleBuffs,0) },
    { "DoNotToggleOff:",                    TOK_INT(AIConfigPowerConfig,            doNotToggleOff,0) },
    { "CritterOverridePrefMult:",            TOK_F32(AIConfigPowerConfig,        critterOverridePreferenceMultiplier, -1.0f)    },
    { "}",                                    TOK_END,            0 },
    { "", 0, 0 }
};

static TokenizerParseInfo parseConfigTargetPref[] = {
    { "{",                                    TOK_IGNORE,        0 },
    { "Archetype:",                            TOK_STRING(AIConfigTargetPref,archetype,0) },
    { "Factor:",                            TOK_F32(AIConfigTargetPref,factor,0)    },
    { "}",                                    TOK_END,            0 },
    { "", 0, 0 }
};
static TokenizerParseInfo parseConfigAOEUsage[] = {
    { "{",                                    TOK_IGNORE,        0 },
    { "SingleTargetPreference:",            TOK_F32(AIConfigAOEUsage,fConfigSingleTargetPreference,-1.0) },
    { "AOENumTargetMultiplier:",            TOK_F32(AIConfigAOEUsage,fAOENumTargetMultiplier,-1.0)    },
    { "}",                                    TOK_END,            0 },
    { "", 0, 0 }
};

#define CREATE_FIELD(name) AICONFIGFIELD_##name

typedef enum AIConfigField {
    CREATE_FIELD(BRAIN_NAME),
    //-------------------
    CREATE_FIELD(BRAIN_HYDRA_USE_TEAM),
    CREATE_FIELD(BRAIN_SWITCH_TARGETS_BASE_TIME),
    CREATE_FIELD(BRAIN_SWITCH_TARGETS_MIN_DISTANCE),
    CREATE_FIELD(BRAIN_SWITCH_TARGETS_TELEPORT),
    CREATE_FIELD(BRAIN_RIKTI_UXB_FULL_HEALTH),
    //-------------------
    CREATE_FIELD(TEAM_ASSIGN_TARGET_PRIORITY),
    //-------------------
    CREATE_FIELD(FEELING_SCALE_MEATTACKED),
    CREATE_FIELD(FEELING_SCALE_FRIENDATTACKED),
    CREATE_FIELD(FEELING_BASE_FEAR),
    CREATE_FIELD(FEELING_BASE_CONFIDENCE),
    CREATE_FIELD(FEELING_BASE_LOYALTY),
    CREATE_FIELD(FEELING_BASE_BOTHERED),
    //-------------------
    CREATE_FIELD(MISC_ALWAYS_FLY),
    CREATE_FIELD(MISC_ALWAYS_HIT),
    CREATE_FIELD(MISC_BOOST_SPEED),
    CREATE_FIELD(MISC_CONSIDER_ENEMY_LEVEL),
    CREATE_FIELD(MISC_DO_ATTACK_AI),
    CREATE_FIELD(MISC_DO_NOT_FACE_TARGET),
    CREATE_FIELD(MISC_DO_NOT_ASSIGN_TO_MELEE),
    CREATE_FIELD(MISC_DO_NOT_ASSIGN_TO_RANGED),
    CREATE_FIELD(MISC_DO_NOT_GO_TO_SLEEP),
    CREATE_FIELD(MISC_DO_NOT_MOVE),
    CREATE_FIELD(MISC_DO_NOT_SHARE_INFO_WITH_MEMBERS),
    CREATE_FIELD(MISC_EVENT_MEMORY_DURATION),
    CREATE_FIELD(MISC_FOV_DEGREES),
    CREATE_FIELD(MISC_FIND_TARGET_IN_PROXIMITY),
    CREATE_FIELD(MISC_GRIEF_HP_RATIO),
    CREATE_FIELD(MISC_GRIEF_TIME_BEGIN),
    CREATE_FIELD(MISC_GRIEF_TIME_END),
    CREATE_FIELD(MISC_IGNORE_FRIENDS_ATTACKED),
    CREATE_FIELD(MISC_IGNORE_TARGET_STEALTH_RADIUS),
    CREATE_FIELD(MISC_INVINCIBLE),
    CREATE_FIELD(MISC_IS_CAMERA),
    CREATE_FIELD(MISC_IS_RESURRECTABLE),
    CREATE_FIELD(MISC_KEEP_SPAWN_PYR),
    CREATE_FIELD(MISC_LIMITED_SPEED),
    CREATE_FIELD(MISC_NO_PHYSICS),
    CREATE_FIELD(MISC_ONTEAMVILLAIN),
    CREATE_FIELD(MISC_ONTEAMMONSTER),
    CREATE_FIELD(MISC_ONTEAMHERO),
    CREATE_FIELD(MISC_OVERRIDE_PERCEPTION_RADIUS),
    CREATE_FIELD(MISC_PATH_LIMITED),
    CREATE_FIELD(MISC_PERCEIVE_ATTACKS_WITHOUT_DAMAGE),
    CREATE_FIELD(MISC_PREFER_GROUND_COMBAT),
    CREATE_FIELD(MISC_PREFER_MELEE),
    CREATE_FIELD(MISC_PREFER_MELEE_SCALE),
    CREATE_FIELD(MISC_PREFER_RANGED),
    CREATE_FIELD(MISC_PREFER_RANGED_SCALE),
    CREATE_FIELD(MISC_PREFER_UNTARGETED),
    CREATE_FIELD(MISC_PROTECT_SPAWN_RADIUS),
    CREATE_FIELD(MISC_PROTECT_SPAWN_OUTSIDE_RADIUS),
    CREATE_FIELD(MISC_PROTECT_SPAWN_TIME),
    CREATE_FIELD(MISC_RELENTLESS),
    CREATE_FIELD(MISC_RETURN_TO_SPAWN_DISTANCE),
    CREATE_FIELD(MISC_SHOUT_RADIUS),
    CREATE_FIELD(MISC_SHOUT_CHANCE),
    CREATE_FIELD(MISC_SPAWN_POS_IS_SELF),
    CREATE_FIELD(MISC_STANDOFF_DISTANCE),
    CREATE_FIELD(MISC_TARGETED_SIGHT_ADD_RANGE),
    CREATE_FIELD(MISC_TURN_WHILE_IDLE),
    CREATE_FIELD(MISC_UNTARGETABLE),
    CREATE_FIELD(MISC_WALK_TO_SPAWN_RADIUS),
    //-------------------
    CREATE_FIELD(POWERS_MAX_PETS),
    CREATE_FIELD(POWERS_RESURRECT_ARCHVILLAINS),
    CREATE_FIELD(POWERS_USE_RANGE_AS_MAXIMUM),
} AIConfigField;

#undef CREATE_FIELD

static TokenizerParseInfo aiConfigSettingWrite[] = {
    { "",                                    TOK_INT(AIConfigSetting, field, 0) },
    { "",                                    TOK_RAW(AIConfigSetting, value)  },
    { "",                                    TOK_STRING(AIConfigSetting, string, 0) },
    { "", 0, 0 }
};

#define SETTING_STRUCT(name,tokenType,fieldName)                                                                            \
static TokenizerParseInfo aiConfigSetting##name[] = {                                                                        \
    { "",                                    TOK_STRUCTPARAM |                                                                \
                                            TOK_##tokenType(AIConfigSetting, fieldName, 0) },                        \
    { "",                                    TOK_INT(AIConfigSetting, field, AICONFIGFIELD_##name) },    \
    { "\n",                                    TOK_END,            0 },                                                        \
    { "", 0, 0 }                                                                                                            \
}
                                                                        \
#define SETTING_STRUCT_INT(name) SETTING_STRUCT(name,INT,value.s32)
#define SETTING_STRUCT_STRING(name) SETTING_STRUCT(name,STRING,string)
#define SETTING_STRUCT_F32(name) SETTING_STRUCT(name,F32,value.f32)

    SETTING_STRUCT_STRING(BRAIN_NAME);
    //-------------------
    SETTING_STRUCT_INT(BRAIN_HYDRA_USE_TEAM);
    SETTING_STRUCT_F32(BRAIN_SWITCH_TARGETS_BASE_TIME);
    SETTING_STRUCT_F32(BRAIN_SWITCH_TARGETS_MIN_DISTANCE);
    SETTING_STRUCT_INT(BRAIN_SWITCH_TARGETS_TELEPORT);
    SETTING_STRUCT_INT(BRAIN_RIKTI_UXB_FULL_HEALTH);
    //---------------
    SETTING_STRUCT_INT(TEAM_ASSIGN_TARGET_PRIORITY);
    //---------------
    SETTING_STRUCT_F32(FEELING_SCALE_MEATTACKED);
    SETTING_STRUCT_F32(FEELING_SCALE_FRIENDATTACKED);
    SETTING_STRUCT_F32(FEELING_BASE_FEAR);
    SETTING_STRUCT_F32(FEELING_BASE_CONFIDENCE);
    SETTING_STRUCT_F32(FEELING_BASE_LOYALTY);
    SETTING_STRUCT_F32(FEELING_BASE_BOTHERED);
    //---------------
    SETTING_STRUCT_INT(MISC_ALWAYS_FLY);
    SETTING_STRUCT_INT(MISC_ALWAYS_HIT);
    SETTING_STRUCT_F32(MISC_BOOST_SPEED);
    SETTING_STRUCT_INT(MISC_CONSIDER_ENEMY_LEVEL);
    SETTING_STRUCT_INT(MISC_DO_ATTACK_AI);
    SETTING_STRUCT_INT(MISC_DO_NOT_FACE_TARGET);
    SETTING_STRUCT_INT(MISC_DO_NOT_ASSIGN_TO_MELEE);
    SETTING_STRUCT_INT(MISC_DO_NOT_ASSIGN_TO_RANGED);
    SETTING_STRUCT_INT(MISC_DO_NOT_GO_TO_SLEEP);
    SETTING_STRUCT_INT(MISC_DO_NOT_MOVE);
    SETTING_STRUCT_INT(MISC_DO_NOT_SHARE_INFO_WITH_MEMBERS);
    SETTING_STRUCT_F32(MISC_EVENT_MEMORY_DURATION);
    SETTING_STRUCT_F32(MISC_FOV_DEGREES);
    SETTING_STRUCT_INT(MISC_FIND_TARGET_IN_PROXIMITY);
    SETTING_STRUCT_F32(MISC_GRIEF_HP_RATIO);
    SETTING_STRUCT_F32(MISC_GRIEF_TIME_BEGIN);
    SETTING_STRUCT_F32(MISC_GRIEF_TIME_END);
    SETTING_STRUCT_INT(MISC_IGNORE_FRIENDS_ATTACKED);
    SETTING_STRUCT_INT(MISC_IGNORE_TARGET_STEALTH_RADIUS);
    SETTING_STRUCT_INT(MISC_INVINCIBLE);
    SETTING_STRUCT_INT(MISC_IS_CAMERA);
    SETTING_STRUCT_INT(MISC_IS_RESURRECTABLE);
    SETTING_STRUCT_INT(MISC_KEEP_SPAWN_PYR);
    SETTING_STRUCT_F32(MISC_LIMITED_SPEED);
    SETTING_STRUCT_INT(MISC_NO_PHYSICS);
    SETTING_STRUCT_INT(MISC_ONTEAMVILLAIN);
    SETTING_STRUCT_INT(MISC_ONTEAMMONSTER);
    SETTING_STRUCT_INT(MISC_ONTEAMHERO);
    SETTING_STRUCT_INT(MISC_OVERRIDE_PERCEPTION_RADIUS);
    SETTING_STRUCT_INT(MISC_PATH_LIMITED);
    SETTING_STRUCT_INT(MISC_PERCEIVE_ATTACKS_WITHOUT_DAMAGE);
    SETTING_STRUCT_INT(MISC_PREFER_GROUND_COMBAT);
    SETTING_STRUCT_INT(MISC_PREFER_MELEE);
    SETTING_STRUCT_F32(MISC_PREFER_MELEE_SCALE);
    SETTING_STRUCT_INT(MISC_PREFER_RANGED);
    SETTING_STRUCT_F32(MISC_PREFER_RANGED_SCALE);
    SETTING_STRUCT_INT(MISC_PREFER_UNTARGETED);
    SETTING_STRUCT_F32(MISC_PROTECT_SPAWN_RADIUS);
    SETTING_STRUCT_F32(MISC_PROTECT_SPAWN_OUTSIDE_RADIUS);
    SETTING_STRUCT_F32(MISC_PROTECT_SPAWN_TIME);
    SETTING_STRUCT_INT(MISC_RELENTLESS);
    SETTING_STRUCT_F32(MISC_RETURN_TO_SPAWN_DISTANCE);
    SETTING_STRUCT_F32(MISC_SHOUT_RADIUS);
    SETTING_STRUCT_INT(MISC_SHOUT_CHANCE);
    SETTING_STRUCT_INT(MISC_SPAWN_POS_IS_SELF);
    SETTING_STRUCT_F32(MISC_STANDOFF_DISTANCE);
    SETTING_STRUCT_F32(MISC_TARGETED_SIGHT_ADD_RANGE);
    SETTING_STRUCT_INT(MISC_TURN_WHILE_IDLE);
    SETTING_STRUCT_INT(MISC_UNTARGETABLE);
    SETTING_STRUCT_F32(MISC_WALK_TO_SPAWN_RADIUS);
    //-------------------
    SETTING_STRUCT_INT(POWERS_MAX_PETS);
    SETTING_STRUCT_INT(POWERS_RESURRECT_ARCHVILLAINS);
    SETTING_STRUCT_INT(POWERS_USE_RANGE_AS_MAXIMUM);

#undef SETTING_STRUCT_STRING
#undef SETTING_STRUCT_F32
#undef SETTING_STRUCT_INT
#undef SETTING_STRUCT

#define SETTING_LINE(parseName, name)                                                                                                \
    { parseName, TOK_REDUNDANTNAME | TOK_STRUCT(AIConfig, settings, aiConfigSetting##name) }

static TokenizerParseInfo parseConfig[] = {
    { "",                                    TOK_STRUCTPARAM |
                                            TOK_STRING(AIConfig,name,0) },
    //--------------------------------------------------------------------------
    { "{",                                    TOK_IGNORE,            0 },
    //------------------------------------------------------------------------
    { "BaseConfig:",                        TOK_STRINGARRAY(AIConfig,baseConfigNames) },
    //------------------------------------------------------------------------
    { "",                                    TOK_STRUCT(AIConfig, settings, aiConfigSettingWrite) },
    //--------------------------------------------------------------------------
    SETTING_LINE("Brain.Name:",                            BRAIN_NAME),
    //--------------------------------------------------------------------------
    SETTING_LINE("Brain.Hydra.UseTeam:",                BRAIN_HYDRA_USE_TEAM),
    SETTING_LINE("Brain.SwitchTargets.BaseTime:",        BRAIN_SWITCH_TARGETS_BASE_TIME),
    SETTING_LINE("Brain.SwitchTargets.MinDistance:",    BRAIN_SWITCH_TARGETS_MIN_DISTANCE),
    SETTING_LINE("Brain.SwitchTargets.Teleport:",        BRAIN_SWITCH_TARGETS_TELEPORT),
    SETTING_LINE("Brain.RiktiUXB.FullHealth:",            BRAIN_RIKTI_UXB_FULL_HEALTH),
    //--------------------------------------------------------------------------
    SETTING_LINE("Team.AssignTargetPriority:",            TEAM_ASSIGN_TARGET_PRIORITY),
    //--------------------------------------------------------------------------
    SETTING_LINE("Feeling.Scale.MeAttacked:",            FEELING_SCALE_MEATTACKED),
    SETTING_LINE("Feeling.Scale.FriendAttacked:",        FEELING_SCALE_FRIENDATTACKED),
    SETTING_LINE("Feeling.Base.Fear:",                    FEELING_BASE_FEAR),
    SETTING_LINE("Feeling.Base.Confidence:",            FEELING_BASE_CONFIDENCE),
    SETTING_LINE("Feeling.Base.Loyalty:",                FEELING_BASE_LOYALTY),
    SETTING_LINE("Feeling.Base.Bothered:",                FEELING_BASE_BOTHERED),
    //--------------------------------------------------------------------------
    SETTING_LINE("Misc.AlwaysFly:",                        MISC_ALWAYS_FLY),
    SETTING_LINE("Misc.AlwaysHit:",                        MISC_ALWAYS_HIT),
    SETTING_LINE("Misc.BoostSpeed",                        MISC_BOOST_SPEED),
    SETTING_LINE("Misc.ConsiderEnemyLevel:",            MISC_CONSIDER_ENEMY_LEVEL),
    SETTING_LINE("Misc.DoAttackAI:",                    MISC_DO_ATTACK_AI),
    SETTING_LINE("Misc.DoNotFaceTarget:",                MISC_DO_NOT_FACE_TARGET),
    SETTING_LINE("Misc.DoNotAssignToMelee:",            MISC_DO_NOT_ASSIGN_TO_MELEE),
    SETTING_LINE("Misc.DoNotAssignToRanged:",            MISC_DO_NOT_ASSIGN_TO_RANGED),
    SETTING_LINE("Misc.DoNotGoToSleep:",                MISC_DO_NOT_GO_TO_SLEEP),
    SETTING_LINE("Misc.DoNotMove:",                        MISC_DO_NOT_MOVE),
    SETTING_LINE("Misc.DoNotShareInfoWithMembers:",        MISC_DO_NOT_SHARE_INFO_WITH_MEMBERS),
    SETTING_LINE("Misc.EventMemoryDuration:",            MISC_EVENT_MEMORY_DURATION),
    SETTING_LINE("Misc.FOVDegrees:",                    MISC_FOV_DEGREES),
    SETTING_LINE("Misc.FindTargetInProximity:",            MISC_FIND_TARGET_IN_PROXIMITY),
    SETTING_LINE("Misc.GriefHPRatio:",                    MISC_GRIEF_HP_RATIO),
    SETTING_LINE("Misc.GriefTimeBegin:",                MISC_GRIEF_TIME_BEGIN),
    SETTING_LINE("Misc.GriefTimeEnd:",                    MISC_GRIEF_TIME_END),
    SETTING_LINE("Misc.IgnoreFriendsAttacked:",            MISC_IGNORE_FRIENDS_ATTACKED),
    SETTING_LINE("Misc.IgnoreTargetStealthRadius:",        MISC_IGNORE_TARGET_STEALTH_RADIUS),
    SETTING_LINE("Misc.Invincible:",                    MISC_INVINCIBLE),
    SETTING_LINE("Misc.IsCamera:",                        MISC_IS_CAMERA),
    SETTING_LINE("Misc.IsResurrectable:",                MISC_IS_RESURRECTABLE),
    SETTING_LINE("Misc.KeepSpawnPYR:",                    MISC_KEEP_SPAWN_PYR),
    SETTING_LINE("Misc.LimitedSpeed",                    MISC_LIMITED_SPEED),
    SETTING_LINE("Misc.NoPhysics:",                        MISC_NO_PHYSICS),
    SETTING_LINE("Misc.OnTeamFoul:",                    MISC_ONTEAMVILLAIN),
    SETTING_LINE("Misc.OnTeamEvil:",                    MISC_ONTEAMMONSTER),
    SETTING_LINE("Misc.OnTeamGood:",                    MISC_ONTEAMHERO),
    SETTING_LINE("Misc.OnTeamVillain:",                    MISC_ONTEAMVILLAIN),
    SETTING_LINE("Misc.OnTeamMonster:",                    MISC_ONTEAMMONSTER),
    SETTING_LINE("Misc.OnTeamHero:",                    MISC_ONTEAMHERO),
    SETTING_LINE("Misc.OverridePerceptionRadius:",        MISC_OVERRIDE_PERCEPTION_RADIUS),
    SETTING_LINE("Misc.PathLimited:",                    MISC_PATH_LIMITED),
    SETTING_LINE("Misc.PerceiveAttacksWithoutDamage:",    MISC_PERCEIVE_ATTACKS_WITHOUT_DAMAGE),
    SETTING_LINE("Misc.PreferGroundCombat:",            MISC_PREFER_GROUND_COMBAT),
    SETTING_LINE("Misc.PreferMelee:",                    MISC_PREFER_MELEE),
    SETTING_LINE("Misc.PreferMeleeScale:",                MISC_PREFER_MELEE_SCALE),
    SETTING_LINE("Misc.PreferRanged:",                    MISC_PREFER_RANGED),
    SETTING_LINE("Misc.PreferRangedScale:",                MISC_PREFER_RANGED_SCALE),
    SETTING_LINE("Misc.PreferUntargeted:",                MISC_PREFER_UNTARGETED),
    SETTING_LINE("Misc.ProtectSpawnRadius:",            MISC_PROTECT_SPAWN_RADIUS),
    SETTING_LINE("Misc.ProtectSpawnOutsideRadius:",        MISC_PROTECT_SPAWN_OUTSIDE_RADIUS),
    SETTING_LINE("Misc.ProtectSpawnTime:",                MISC_PROTECT_SPAWN_TIME),
    SETTING_LINE("Misc.Relentless:",                    MISC_RELENTLESS),
    SETTING_LINE("Misc.ReturnToSpawnDistance:",            MISC_RETURN_TO_SPAWN_DISTANCE),
     SETTING_LINE("Misc.ShoutRadius:",                    MISC_SHOUT_RADIUS),
    SETTING_LINE("Misc.ShoutChance:",                    MISC_SHOUT_CHANCE),
    SETTING_LINE("Misc.SpawnPosIsSelf:",                MISC_SPAWN_POS_IS_SELF),
    SETTING_LINE("Misc.StandoffDistance:",                MISC_STANDOFF_DISTANCE),
    SETTING_LINE("Misc.TargetedSightAddRange:",            MISC_TARGETED_SIGHT_ADD_RANGE),
    SETTING_LINE("Misc.TurnWhileIdle:",                    MISC_TURN_WHILE_IDLE),
    SETTING_LINE("Misc.Untargetable:",                    MISC_UNTARGETABLE),
    SETTING_LINE("Misc.WalkToSpawnRadius:",                MISC_WALK_TO_SPAWN_RADIUS),
    //--------------------------------------------------------------------------
    SETTING_LINE("Powers.MaxPets:",                        POWERS_MAX_PETS),
    SETTING_LINE("Powers.ResurrectArchVillains:",        POWERS_RESURRECT_ARCHVILLAINS),
    SETTING_LINE("Powers.UseRangeAsMaximum:",            POWERS_USE_RANGE_AS_MAXIMUM),
    //--------------------------------------------------------------------------
    { "AssignPowerByName:",                    TOK_STRUCT(AIConfig, powerAssignment.list, parseConfigPowerAssignmentByName) },
    { "AssignPowerByFlag:",                    TOK_REDUNDANTNAME | TOK_STRUCT(AIConfig,powerAssignment.list, parseConfigPowerAssignmentByFlag) },
    { "AssignPowerNoFlags:",                TOK_REDUNDANTNAME | TOK_STRUCT(AIConfig,powerAssignment.list, parseConfigPowerAssignmentNoFlags) },
    { "PowerConfig",                        TOK_STRUCT(AIConfig, powerConfigs, parseConfigPowerConfig)    },
    { "TargetPreference",                    TOK_STRUCT(AIConfig, targetPref, parseConfigTargetPref)    },
    { "AOEUsage",                            TOK_OPTIONALSTRUCT(AIConfig, AOEUsageConfig, parseConfigAOEUsage), },
    //------------------------------------------------------------------------
    { "}",                                    TOK_END,            0 },
    //--------------------------------------------------------------------------
    { "", 0, 0 }
};

static TokenizerParseInfo parsePowerGroup[] = {
    { "",                                    TOK_STRUCTPARAM | TOK_STRING(AIConfigPowerGroup,name,0) },
    { "",                                    TOK_STRUCTPARAM | TOK_INT(AIConfigPowerGroup,bit,0) },
    { "\n",                                    TOK_END,            0 },
    //--------------------------------------------------------------------------
    { "", 0, 0 }
};

static TokenizerParseInfo parseAllConfigs[] = {
    { "Config:",        TOK_STRUCT(AIAllConfigs,configs,parseConfig)        },
    { "PowerGroup:",    TOK_STRUCT(AIAllConfigs,powerGroups,parsePowerGroup)    },
    //--------------------------------------------------------------------------
    { "", 0, 0 }
};

static DefineIntList initDefineMapping[] = {

    // End the list.

    { 0, 0 }
};

static int staticInitHadAnError;

static void printInitErrorHeader(){
    if(!staticInitHadAnError){
        staticInitHadAnError = 1;

        printf("\n******************* BEGIN AI INIT ERRORS *******************\n\n");
    }
}

static void printInitErrorFooter(int forceReload){
    if(staticInitHadAnError){
        printf("\n******************** END AI INIT ERRORS ********************\n\n");
    }
    else if(forceReload){
        printf("AI Config (%d count): NO ERRORS.\n\n", eaSize(&aiAllConfigs.configs));
    }
}

void aiConfigInitPowerAssignments(AIConfig* config){
    int count;
    int groupCount;
    int i;
    int j;

    if(config->powerAssignment.initialized)
        return;

    config->powerAssignment.initialized = 1;

    // Initialize all the base configs.

    count = eaSize(&config->baseConfigs);

    for(i = 0; i < count; i++){
        AIConfig* baseConfig = config->baseConfigs[i];

        if(baseConfig){
            aiConfigInitPowerAssignments(baseConfig);
        }
    }

    // Initialize all the "to" bits.

    count = eaSize(&config->powerAssignment.list);
    groupCount = eaSize(&aiAllConfigs.powerGroups);

    for(i = 0; i < count; i++){
        AIConfigPowerAssignment* a = config->powerAssignment.list[i];

        // Look up the name in the list.

        for(j = 0; j < groupCount; j++){
            if(!stricmp(a->nameTo, aiAllConfigs.powerGroups[j]->name)){
                a->toBit = aiAllConfigs.powerGroups[j]->bit;

                if(a->toBit < 0)
                    a->toBit = 0;
                else if (a->toBit > 31)
                    a->toBit = 31;

                break;
            }
        }

        // Store a new name.

        if(j == groupCount){
            printInitErrorHeader();
            Errorf("  ERROR: Unfound power group: %s\n", a->nameTo);
        }
    }
}

static int getPowerGroupFlags(const char* name){
    int count = eaSize(&aiAllConfigs.powerGroups);
    int i;

    for(i = 0; i < count; i++){
        if(!stricmp(aiAllConfigs.powerGroups[i]->name, name))
            return 1 << aiAllConfigs.powerGroups[i]->bit;
    }

    printInitErrorHeader();
    Errorf("  ERROR: Didn't find required power group: %s\n", name);

    return 0;
}

static int compareAIConfigNames(const void* c1Data, const void* c2Data){
    const AIConfig** c1 = (const AIConfig**)c1Data;
    const AIConfig** c2 = (const AIConfig**)c2Data;
    if(!(*c1)->name)
        return -1;

    if(!(*c2)->name)
        return 1;

    return stricmp((*c1)->name, (*c2)->name);
}

static bool preProcessAIConfigs(TokenizerParseInfo pti[], void* structptr){
    eaQSort(aiAllConfigs.configs, compareAIConfigNames);

    return true;
}

void aiInitLoadConfigFiles(int forceReload){
    int i;

    staticInitHadAnError = 0;
    server_error_sent_count = 0;

    if(forceReload){
        // MS: This function leaks memory.  Guess who doesn't care.  That's right, it's me.
        //     It's just a debugging function anyway.

        printf("Reloading AI config files...\n");

        memset(&aiAllConfigs, 0, sizeof(aiAllConfigs));
    }

    if(!aiAllConfigs.configs){
        // Create the defines object.

        DefineContext *defines = DefineCreateFromIntList(initDefineMapping);

        // Parse the priority list files.

        ParserLoadFiles("AIScript/config",
                        ".cfg",
                        "ai_config.bin",
                        (forceReload?PARSER_FORCEREBUILD:0) | PARSER_SERVERONLY,
                        parseAllConfigs,
                        &aiAllConfigs,
                        NULL,
                        NULL,
                        preProcessAIConfigs);

        // Destroy the defines object.

        DefineDestroy(defines);

        if(forceReload){
            printf("\n%d AI configs found:\n", eaSize(&aiAllConfigs.configs));

            for(i = 0; i < eaSize(&aiAllConfigs.configs); i++){
                printf("  %s\n", aiAllConfigs.configs[i]->name);
            }
        }

        // Create the hash table to lookup initializers by name.

        nameToAIConfigTable = stashTableCreateWithStringKeys(10,  StashDeepCopyKeys);

        for(i = 0; i < eaSize(&aiAllConfigs.configs); i++){
            if ( stashFindElement(nameToAIConfigTable, aiAllConfigs.configs[i]->name, NULL)){
                printInitErrorHeader();
                Errorf("  ERROR: Duplicate AI config name: %s\n", aiAllConfigs.configs[i]->name);
            }else{
                stashAddPointer(nameToAIConfigTable, aiAllConfigs.configs[i]->name, aiAllConfigs.configs[i], false);
            }
        }

        // Init the base configs from the base config names.

        for(i = 0; i < eaSize(&aiAllConfigs.configs); i++){
            AIConfig* config = aiAllConfigs.configs[i];
            int j;
            for(j = 0; j < eaSize(&config->baseConfigNames); j++){
                char* name = config->baseConfigNames[j];
                AIConfig* baseConfig;

                stashFindPointer( nameToAIConfigTable, name, &baseConfig );

                if(baseConfig){
                    eaPush(&config->baseConfigs, baseConfig);
                }else{
                    printInitErrorHeader();
                    Errorf("  ERROR: Unknown base config: %s\n", name);
                }
            }
        }

        // Init the power assignment thing.

        for(i = 0; i < eaSize(&aiAllConfigs.configs); i++){
            aiConfigInitPowerAssignments(aiAllConfigs.configs[i]);
        }
    }

    // Initialize the dynamic group bits.

    AI_POWERGROUP_BASIC_ATTACK            = getPowerGroupFlags("pgBasicAttack");
    AI_POWERGROUP_FLY_EFFECT            = getPowerGroupFlags("pgFlyEffect");
    AI_POWERGROUP_RESURRECT                = getPowerGroupFlags("pgResurrect");
    AI_POWERGROUP_BUFF                    = getPowerGroupFlags("pgBuff");
    AI_POWERGROUP_DEBUFF                = getPowerGroupFlags("pgDebuff");
    AI_POWERGROUP_ROOT                    = getPowerGroupFlags("pgRoot");
    AI_POWERGROUP_VICTOR                = getPowerGroupFlags("pgVictor");
    AI_POWERGROUP_POSTDEATH                = getPowerGroupFlags("pgPostDeath");
    AI_POWERGROUP_HEAL_SELF                = getPowerGroupFlags("pgHealSelf");
    AI_POWERGROUP_TOGGLEHARM            = getPowerGroupFlags("pgToggleHarm");
    AI_POWERGROUP_TOGGLEAOEBUFF            = getPowerGroupFlags("pgToggleAoEBuff");
    AI_POWERGROUP_HEAL_ALLY                = getPowerGroupFlags("pgHealAlly");
    AI_POWERGROUP_SUMMON                = getPowerGroupFlags("pgSummon");
    AI_POWERGROUP_KAMIKAZE                = getPowerGroupFlags("pgKamikaze");
    AI_POWERGROUP_PLANT                    = getPowerGroupFlags("pgPlant");
    AI_POWERGROUP_TELEPORT                = getPowerGroupFlags("pgTeleport");
    AI_POWERGROUP_EARLYBATTLE            = getPowerGroupFlags("pgEarlyBattle");
    AI_POWERGROUP_MIDBATTLE                = getPowerGroupFlags("pgMidBattle");
    AI_POWERGROUP_ENDBATTLE                = getPowerGroupFlags("pgEndBattle");
    AI_POWERGROUP_FORCEFIELD            = getPowerGroupFlags("pgForceField");
    AI_POWERGROUP_HELLIONFIRE            = getPowerGroupFlags("pgHellionFire");
    AI_POWERGROUP_PET                    = getPowerGroupFlags("pgPet");
    AI_POWERGROUP_SECONDARY_TARGET        = getPowerGroupFlags("pgSecondaryTarget");

    AI_POWERGROUP_STAGES = AI_POWERGROUP_EARLYBATTLE | AI_POWERGROUP_MIDBATTLE | AI_POWERGROUP_ENDBATTLE;

    if(forceReload){
        for(i = 0; i < entities_max; i++){
            Entity* e = entities[i];

            if(entInUse(i) && e && ENTAI(e)){
                aiSetConfigByName(e, ENTAI(e), ENTAI(e)->brain.configName);
            }
        }
    }

    // Print the error footer.

    printInitErrorFooter(forceReload);
}

void aiInitSystem(){
    static int init = 0;

    if(!init){
        int i;

        init = 1;

        // Set brain type names.

        #define SET_NAME(x) aiBrainTypeName[x] = #x + 9 // Horrible hack, please ignore.
            // Default.

            SET_NAME(AI_BRAIN_DEFAULT);

            // Custom but generic

            SET_NAME(AI_BRAIN_CUSTOM_HEAL_ALWAYS);

            // 5th Column.

            SET_NAME(AI_BRAIN_5THCOLUMN_FOG);
            SET_NAME(AI_BRAIN_5THCOLUMN_FURY);
            SET_NAME(AI_BRAIN_5THCOLUMN_NIGHT);
            SET_NAME(AI_BRAIN_5THCOLUMN_VAMPYR);

            // Banished Pantheon.

            SET_NAME(AI_BRAIN_BANISHED_PANTHEON);
            SET_NAME(AI_BRAIN_BANISHED_PANTHEON_SORROW);
            SET_NAME(AI_BRAIN_BANISHED_PANTHEON_LT);

            // Clockwork.

            SET_NAME(AI_BRAIN_CLOCKWORK);

            // Devouring Earth

            SET_NAME(AI_BRAIN_DEVOURING_EARTH_MINION);
            SET_NAME(AI_BRAIN_DEVOURING_EARTH_LT);
            SET_NAME(AI_BRAIN_DEVOURING_EARTH_BOSS);

            // Freakshow.

            SET_NAME(AI_BRAIN_FREAKSHOW);
            SET_NAME(AI_BRAIN_FREAKSHOW_TANK);

            // Hellions.

            SET_NAME(AI_BRAIN_HELLIONS);

            // Hydra.

            SET_NAME(AI_BRAIN_HYDRA_HEAD);
            SET_NAME(AI_BRAIN_HYDRA_FORCEFIELD_GENERATOR);

            // Malta.

            SET_NAME(AI_BRAIN_MALTA_HERCULES_TITAN);

            // Octopus.

            SET_NAME(AI_BRAIN_OCTOPUS_HEAD);
            SET_NAME(AI_BRAIN_OCTOPUS_TENTACLE);

            // Outcasts.

            SET_NAME(AI_BRAIN_OUTCASTS);

            // Prisoners.

            SET_NAME(AI_BRAIN_PRISONERS);

            // Rikti.

            SET_NAME(AI_BRAIN_RIKTI_SUMMONER);
            SET_NAME(AI_BRAIN_RIKTI_UXB);

            // Skulls.

            SET_NAME(AI_BRAIN_SKULLS);

            // SkyRaiders.

            SET_NAME(AI_BRAIN_SKYRAIDERS_ENGINEER);
            SET_NAME(AI_BRAIN_SKYRAIDERS_PORTER);

            // SwitchTargets,

            SET_NAME(AI_BRAIN_SWITCH_TARGETS);

            // TheFamily.

            SET_NAME(AI_BRAIN_THEFAMILY);

            // Thugs.

            SET_NAME(AI_BRAIN_THUGS);

            // Trolls.

            SET_NAME(AI_BRAIN_TROLLS);

            // Tsoo.

            SET_NAME(AI_BRAIN_TSOO_MINION);
            SET_NAME(AI_BRAIN_TSOO_HEALER);

            // Tsoo.

            SET_NAME(AI_BRAIN_TSOO_MINION);
            SET_NAME(AI_BRAIN_TSOO_HEALER);

            // Vahzilok.

            SET_NAME(AI_BRAIN_VAHZILOK_ZOMBIE);
            SET_NAME(AI_BRAIN_VAHZILOK_REAPER);
            SET_NAME(AI_BRAIN_VAHZILOK_BOSS);

            // Warriors.

            SET_NAME(AI_BRAIN_WARRIORS);

            // Circle of Thorns
            SET_NAME(AI_BRAIN_COT_THE_HIGH);
            SET_NAME(AI_BRAIN_COT_BEHEMOTH);
            SET_NAME(AI_BRAIN_COT_CASTER);
            SET_NAME(AI_BRAIN_COT_SPECTRES);

            // Arachnos
            SET_NAME(AI_BRAIN_ARACHNOS_WEB_TOWER);
            SET_NAME(AI_BRAIN_ARACHNOS_REPAIRMAN);
            SET_NAME(AI_BRAIN_ARACHNOS_DR_AEON);
            SET_NAME(AI_BRAIN_ARACHNOS_MAKO);
            SET_NAME(AI_BRAIN_ARACHNOS_GHOST_WIDOW);
            SET_NAME(AI_BRAIN_ARACHNOS_BLACK_SCORPION);
            SET_NAME(AI_BRAIN_ARACHNOS_SCIROCCO);
            SET_NAME(AI_BRAIN_ARACHNOS_LORD_RECLUSE_STF);

            //Architect
            SET_NAME(AI_BRAIN_ARCHITECT_ALLY);
        #undef SET_NAME

        for(i = 0; i < ARRAY_SIZE(aiBrainTypeName); i++){
            assert(aiBrainTypeName[i]);
        }

        nameToBrainTypeTable = stashTableCreateWithStringKeys(10,  StashDeepCopyKeys);

        for(i = 0; i < AI_BRAIN_COUNT; i++){
            if ( stashFindElement(nameToBrainTypeTable, aiBrainTypeName[i], NULL)){
                Errorf("  ERROR: Duplicate brain type name: %s\n", aiBrainTypeName[i]);
            }else{
                stashAddInt(nameToBrainTypeTable, aiBrainTypeName[i], i, false);
            }
        }

        // Reset critter count and total timeslice.

        ai_state.totalTimeslice = 0;
        ai_state.activeCritters = 0;

        // Load priority lists.

//         aiPriorityLoadFiles(0);

        // Load config files.

        aiInitLoadConfigFiles(0);

        // Load Anim lists

        loadAnimLists();

        // Init behavior system

        aiBehaviorCohInitSystem();
            
        // Load Behavior aliases

        aiBehaviorLoadAliases();
    }
}

MP_DEFINE(AIVars);

static void aiInitVars(Entity* e){
    MP_CREATE(AIVars, 50);

    assert(!ENTAI(e));

    SET_ENTAI(e) = MP_ALLOC(AIVars);
    ENTAI(e)->base = aiSystemGetBase();
}

static void aiInitMoveType(Entity* e){
    switch(ENTTYPE(e)){
        case ENTTYPE_CAR:
            e->move_type = MOVETYPE_WIRE;
            break;
        case ENTTYPE_CRITTER:
            e->move_type = MOVETYPE_WALK;
            break;
        case ENTTYPE_PLAYER:
            // Don't set the player move type.
            break;
        case ENTTYPE_DOOR:
        case ENTTYPE_MISSION_ITEM:
        default:
            e->move_type = 0;
            break;
    }
}

static void aiDestroyPowers(Entity* e, AIVars* ai){
    AIPowerInfo* info;

    for(info = ai->power.list; info;){
        AIPowerInfo* next = info->next;

        assert(info->power->pAIPowerInfo == info);

        info->power->pAIPowerInfo = NULL;

        destroyAIPowerInfo(info);

        info = next;
    }
}

static void aiInitPowerConfig(Entity* e, AIVars* ai, AIPowerInfo* info, AIConfig* config)
{
    int i, j;

    for(i = eaSize(&config->baseConfigs)-1; i >= 0; i--)
        aiInitPowerConfig(e, ai, info, config->baseConfigs[i]);

    for(i = eaSize(&config->powerConfigs)-1; i >= 0; i--)
    {
        if(!stricmp(info->power->ppowBase->pchName, config->powerConfigs[i]->powerName))
        {
            for(j = eaSize(&config->powerConfigs[i]->allowedTargets)-1; j >= 0; j--)
                eaPushConst(&info->allowedTargets, allocAddString(config->powerConfigs[i]->allowedTargets[j]));

            info->allowMultipleBuffs = !!config->powerConfigs[i]->allowMultipleBuffs;
            info->doNotToggleOff = !!config->powerConfigs[i]->doNotToggleOff;
            info->critterOverridePreferenceMultiplier = config->powerConfigs[i]->critterOverridePreferenceMultiplier;
        }
    }
}

static AIPowerInfo* aiAddPowerInfo(Entity* e, AIVars* ai, Power* power)
{
    const BasePower* basePower = power->ppowBase;
    AIPowerInfo* info;
    AIConfig* config;

    if(!e || !ai || !power)
        return NULL;

    // Create the new info.

    ai->power.count++;

    info = createAIPowerInfo();

    info->power = power;
    power->pAIPowerInfo = info;

    // Check for a new stance ID.

    if(!strnicmp(basePower->pchName, "Brawl", 5)){
        info->stanceID = 0;
    }else{
        // Check all the current powers for the same stance.

        AIPowerInfo* otherInfo;

        for(otherInfo = ai->power.list; otherInfo; otherInfo = otherInfo->next){
            if(    otherInfo != info &&
                otherInfo->stanceID > 0 &&
                character_StanceIsSame(otherInfo->power, power))
            {
                // Same stance, so take the ID.

                info->stanceID = otherInfo->stanceID;

                break;
            }
        }

        if(!otherInfo){
            // New stance ID.

            info->stanceID = ai->power.nextAvailableStanceID++;
        }
    }

    // Add the new info to the info list.

    if(!ai->power.list){
        ai->power.list = info;
    }else{
        ai->power.lastInfo->next = info;
    }

    info->next = NULL;

    // Classify the power.

    switch(basePower->eType){
        case kPowerType_Click:{
            if(    power_AffectsType(power, kTargetType_Foe) &&
                (    basePower->eTargetType == kTargetType_Caster ||
                    basePower->eTargetType == kTargetType_Foe))
            {
                info->isAttack = 1;
            }
            break;
        }

        case kPowerType_Auto:{
            info->isAuto = 1;
            break;
        }

        case kPowerType_Toggle:{
            info->isToggle = 1;
            break;
        }
    }

    switch(basePower->eTargetType){
        case kTargetType_Caster:
            info->sourceRadius = 0;
            break;

        default:
            info->sourceRadius = ai->collisionRadius;
    }

    if(basePower->bNearGround)
        info->isNearGround = 1;

    info->critterOverridePreferenceMultiplier = -1;        //    by default
    if( stashFindPointer( nameToAIConfigTable, aiGetConfig(e, ai), &config) )
        aiInitPowerConfig(e, ai, info, config);

    if( info && e->villainDef && e->villainDef->specialPetPower )
    {
        if( 0 == stricmp( e->villainDef->specialPetPower, info->power->ppowBase->pchName ) )
        {
            info->isSpecialPetPower = 1; 
        }
    }

    ai->power.lastInfo = info;
    return info;
}

void aiAddPower(Entity* e, Power* power)
{
    AIVars* ai;
    int rankCon = 0;

    if(!e || !e->pchar || !(ai = ENTAI(e)) || !power || ENTTYPE(e) == ENTTYPE_PLAYER)
        return;

    // checking for max level limits
    if( !power->bIgnoreConningRestrictions )
    {
        if ((basepower_GetAIMaxLevel(power->ppowBase) < e->pchar->iLevel) ||
            (basepower_GetAIMinLevel(power->ppowBase) > e->pchar->iLevel))
            return;

        // checking for rank con limits
        rankCon = villainRankConningAdjustment(e->villainRank);
        if ((basepower_GetAIMaxRankCon(power->ppowBase) < rankCon) ||
            (basepower_GetAIMinRankCon(power->ppowBase) > rankCon))
            return;
    }

    aiAddPowerInfo(e, ai, power);
    if(ai->teamMemberInfo.team)
        aiAddTeamMemberPower(ai->teamMemberInfo.team, &ai->teamMemberInfo, power->pAIPowerInfo);

    ai->power.dirty = 1;
}

static void aiRemovePowerInfo(Entity* e, AIVars* ai, AIPowerInfo* info)
{
    AIPowerInfo* next = info->next;
    AIPowerInfo* powerInfoPrev = NULL;
    AIPowerInfo* powerInfo = ai->power.list;

    while (powerInfo && powerInfo != info)
    {
        powerInfoPrev = powerInfo;
        powerInfo = powerInfo->next;
    }

    assert(info->power->pAIPowerInfo == info && powerInfo);

    info->power->pAIPowerInfo = NULL;

    destroyAIPowerInfo(info);

    if (powerInfoPrev)
        powerInfoPrev->next = next;
    else
        ai->power.list = next;

    if (next == NULL)
    {
        ai->power.lastInfo = powerInfoPrev;
    }

    ai->power.count--;

    if(ai->power.groupFlags)
        ai->power.groupFlags = 0;
}

void aiRemovePower(Entity* e, Power* power)
{
    AIVars* ai;
    if(!e || !(ai = ENTAI(e)) || !power || !power->pAIPowerInfo || ENTTYPE(e) == ENTTYPE_PLAYER)
        return;

    if(ai->teamMemberInfo.team)
        aiRemoveTeamMemberPower(ai->teamMemberInfo.team, &ai->teamMemberInfo, power->pAIPowerInfo);
    aiRemovePowerInfo(e, ai, power->pAIPowerInfo);

    ai->power.preferred = NULL;
    ai->power.current = NULL;
    ai->power.lastStanceID = 0;
}

static void aiInitPowers(Entity* e, AIVars* ai){
    int i;
    PowerSet** powerSets;
    int powerSetCount;
    int rankCon = 0;

    if(!e->pchar){
        return;
    }

    powerSets = e->pchar->ppPowerSets;
    powerSetCount = eaSize(&powerSets);

    // Classify individual powers.

    ai->power.count = 0;
    ai->power.nextAvailableStanceID = 1;

    for(i = 0; i < powerSetCount; i++){
        Power** powers = powerSets[i]->ppPowers;
        int powerCount = eaSize(&powers);
        int j;

        for(j = 0; j < powerCount; j++)
        {

            if( !powers[j]->bIgnoreConningRestrictions )
            {
                // checking for max level limits
                if ((basepower_GetAIMaxLevel(powers[j]->ppowBase) < e->pchar->iLevel) ||
                    (basepower_GetAIMinLevel(powers[j]->ppowBase) > e->pchar->iLevel))
                    continue;

                rankCon = villainRankConningAdjustment(e->villainRank);
                if ((basepower_GetAIMaxRankCon(powers[j]->ppowBase) < rankCon) ||
                    (basepower_GetAIMinRankCon(powers[j]->ppowBase) > rankCon))
                    continue;
            }
            aiAddPowerInfo(e, ai, powers[j]);
        }
    }
}

static void aiUnsetBrainType(Entity* e, AIVars* ai){
    switch(ai->brain.type){
        case AI_BRAIN_VAHZILOK_BOSS:
            break;
    }

    ai->brain.type = AI_BRAIN_DEFAULT;
}

static void aiSetBrainType(Entity* e, AIVars* ai, AIBrainType brainType){
    if(brainType < 0 || brainType >= AI_BRAIN_COUNT)
        return;

    aiUnsetBrainType(e, ai);

    ai->brain.type = brainType;

    switch(ai->brain.type){
        case AI_BRAIN_VAHZILOK_BOSS:
            break;
    }
}

static int powerMatchesAssignment(AIPowerInfo* powerInfo, AIConfigPowerAssignment* assignment){
    int i;
    int count;

    switch(assignment->fromType){
        case FROMTYPE_FLAG:{
            if(assignment->nameFrom){
                const char** aiGroups = powerInfo->power->ppowBase->ppchAIGroups;

                count = eaSize(&aiGroups);

                for(i = 0; i < count; i++){
                    if(!stricmp(aiGroups[i], assignment->nameFrom)){
                        return 1;
                    }
                }
            }

            break;
        }

        case FROMTYPE_NAME:{
            if(    assignment->nameFrom &&
                !stricmp(powerInfo->power->ppowBase->pchName, assignment->nameFrom))
            {
                return 1;
            }

            break;
        }

        case FROMTYPE_NOFLAG:{
            return powerInfo->groupFlags ? 0 : 1;
        }
    }

    return 0;
}

MP_DEFINE(AIConfigTargetPref);

void AIConfigTargetPrefDestroy(AIConfigTargetPref* pref)
{
    MP_FREE(AIConfigTargetPref, pref);
}

static void aiDoPowerAssignments(Entity* e, AIVars* ai, AIConfig* config)
{
    AIPowerInfo* powerInfo;

    for(powerInfo = ai->power.list; powerInfo; powerInfo = powerInfo->next){
        int count = eaSize(&config->powerAssignment.list);
        int j;

        for(j = 0; j < count; j++){
            if(powerMatchesAssignment(powerInfo, config->powerAssignment.list[j])){
                powerInfo->groupFlags |= 1 << config->powerAssignment.list[j]->toBit;
            }
        }
    }
}

static void aiInitConfig(Entity* e, AIVars* ai, AIConfig* config, int topLevel){
    if(config){
        int i;
        AIBrainType brainType;
        AIPowerInfo* powerInfo;
        AIConfigTargetPref* newPref;
        int baseConfigCount;
        int settingCount;

        if(topLevel){
            ai->brain.configName = config->name;

            for(powerInfo = ai->power.list; powerInfo; powerInfo = powerInfo->next){
                powerInfo->groupFlags = 0;
            }
        }

        // Power assignments.

        aiDoPowerAssignments(e, ai, config);

        ai->AOEUsage.fAOEPreference = 0.4f;
        ai->AOEUsage.fSingleTargetPreference = 1.0f;

        // Init the base configs.
        baseConfigCount = eaSize(&config->baseConfigs);

        for(i = 0; i < baseConfigCount; i++){
            AIConfig* baseConfig = config->baseConfigs[i];

            aiInitConfig(e, ai, baseConfig, 0);
        }

        // Settings.

        settingCount = eaSize(&config->settings);

        for(i = 0; i < settingCount; i++){
            AIConfigSetting* setting = config->settings[i];

            #define FIELD_CASE(name, stuff) case AICONFIGFIELD_##name:{stuff;}break
            #define FIELD_CASE_VAR(name, fieldName, outvar) FIELD_CASE(name, outvar = setting->fieldName)
            #define FIELD_CASE_STRING(name, outvar)         FIELD_CASE_VAR(name, string, outvar)
            #define FIELD_CASE_F32(name, outvar)            FIELD_CASE_VAR(name, value.f32, outvar)
            #define FIELD_CASE_INT(name, outvar)            FIELD_CASE_VAR(name, value.s32, outvar)
            #define FIELD_CASE_BIT(name, outvar)            FIELD_CASE_VAR(name, value.s32 ? 1 : 0, outvar)
            #define FIELD_CASE_SEC(name, outvar)            FIELD_CASE(name, outvar = SEC_TO_ABS(setting->value.f32))

            switch(setting->field){
                case AICONFIGFIELD_BRAIN_NAME:{
                    if(setting->string){
                        StashElement element;
                        stashFindElement(nameToBrainTypeTable, setting->string, &element);

                        if(element){
                            brainType = (AIBrainType)stashElementGetPointer(element);

                            aiSetBrainType(e, ai, brainType);
                        }else{
                            Errorf("ERROR: AI config %s: Invalid brain type: %s\n", config->name, setting->string);
                        }
                    }
                    break;
                }

                //---------------
                FIELD_CASE_BIT( BRAIN_HYDRA_USE_TEAM,                ai->brain.hydraHead.useTeam);
                FIELD_CASE_F32( BRAIN_SWITCH_TARGETS_BASE_TIME,        ai->brain.switchTargets.baseTime);
                FIELD_CASE_F32( BRAIN_SWITCH_TARGETS_MIN_DISTANCE,    ai->brain.switchTargets.minDistance);
                FIELD_CASE_BIT( BRAIN_SWITCH_TARGETS_TELEPORT,        ai->brain.switchTargets.teleport);
                FIELD_CASE_BIT( BRAIN_RIKTI_UXB_FULL_HEALTH,        ai->brain.rikti.fullHealth);
                //---------------
                FIELD_CASE_INT(    TEAM_ASSIGN_TARGET_PRIORITY,        ai->teamMemberInfo.assignTargetPriority);
                //---------------
                FIELD_CASE_F32(    FEELING_SCALE_MEATTACKED,            ai->feeling.scale.meAttacked);
                FIELD_CASE_F32(    FEELING_SCALE_FRIENDATTACKED,        ai->feeling.scale.friendAttacked);
                FIELD_CASE_F32(    FEELING_BASE_FEAR,                    ai->feeling.sets[AI_FEELINGSET_BASE].level[AI_FEELING_FEAR]);
                FIELD_CASE_F32(    FEELING_BASE_CONFIDENCE,            ai->feeling.sets[AI_FEELINGSET_BASE].level[AI_FEELING_CONFIDENCE]);
                FIELD_CASE_F32(    FEELING_BASE_LOYALTY,                ai->feeling.sets[AI_FEELINGSET_BASE].level[AI_FEELING_LOYALTY]);
                FIELD_CASE_F32(    FEELING_BASE_BOTHERED,                ai->feeling.sets[AI_FEELINGSET_BASE].level[AI_FEELING_BOTHERED]);
                //---------------
                FIELD_CASE_BIT(    MISC_ALWAYS_FLY,                    ai->alwaysFly);
                FIELD_CASE_BIT(    MISC_ALWAYS_HIT,                    e->alwaysHit);
                FIELD_CASE_F32( MISC_BOOST_SPEED,                    ai->inputs.boostSpeed);
                FIELD_CASE_BIT(    MISC_CONSIDER_ENEMY_LEVEL,            ai->considerEnemyLevel);
                FIELD_CASE_BIT(    MISC_DO_ATTACK_AI,                    ai->doAttackAI);
                FIELD_CASE_BIT(    MISC_DO_NOT_FACE_TARGET,            e->do_not_face_target);
                FIELD_CASE_BIT(    MISC_DO_NOT_ASSIGN_TO_MELEE,        ai->teamMemberInfo.doNotAssignToMelee);
                FIELD_CASE_BIT(    MISC_DO_NOT_ASSIGN_TO_RANGED,        ai->teamMemberInfo.doNotAssignToRanged);
                FIELD_CASE_BIT(    MISC_DO_NOT_GO_TO_SLEEP,            ai->doNotGoToSleep);
                FIELD_CASE_BIT( MISC_DO_NOT_MOVE,                    ai->doNotMove);
                FIELD_CASE_BIT(    MISC_DO_NOT_SHARE_INFO_WITH_MEMBERS,ai->teamMemberInfo.doNotShareInfoWithMembers);
                FIELD_CASE_SEC(    MISC_EVENT_MEMORY_DURATION,            ai->eventMemoryDuration);
                FIELD_CASE(        MISC_FOV_DEGREES,                    aiSetFOVAngle(e, ai, RAD(setting->value.f32) / 2.0));
                FIELD_CASE_BIT( MISC_FIND_TARGET_IN_PROXIMITY,        ai->findTargetInProximity);
                FIELD_CASE_F32(    MISC_GRIEF_HP_RATIO,                ai->griefed.hpRatio);
                FIELD_CASE_SEC(    MISC_GRIEF_TIME_BEGIN,                ai->griefed.time.begin);
                FIELD_CASE_SEC(    MISC_GRIEF_TIME_END,                ai->griefed.time.end);
                FIELD_CASE_BIT(    MISC_IGNORE_FRIENDS_ATTACKED,        ai->ignoreFriendsAttacked);
                FIELD_CASE_BIT(    MISC_IGNORE_TARGET_STEALTH_RADIUS,    ai->ignoreTargetStealthRadius);
                FIELD_CASE(        MISC_INVINCIBLE,                    e->invincible = (e->invincible & ~INVINCIBLE_GENERAL) | (setting->value.s32 ? INVINCIBLE_GENERAL : 0););
                FIELD_CASE_BIT(    MISC_IS_CAMERA,                        ai->isCamera);
                FIELD_CASE_BIT(    MISC_IS_RESURRECTABLE,                ai->isResurrectable);
                FIELD_CASE_BIT(    MISC_KEEP_SPAWN_PYR,                ai->keepSpawnPYR);
                FIELD_CASE_F32( MISC_LIMITED_SPEED,                    ai->inputs.limitedSpeed);
                FIELD_CASE_BIT(    MISC_NO_PHYSICS,                    e->noPhysics);
                FIELD_CASE(        MISC_ONTEAMVILLAIN,                    entSetOnTeamVillain(e, setting->value.s32));
                FIELD_CASE(        MISC_ONTEAMMONSTER,                    entSetOnTeamMonster(e, setting->value.s32));
                FIELD_CASE(        MISC_ONTEAMHERO,                     entSetOnTeamHero(e, setting->value.s32));
                FIELD_CASE_INT( MISC_OVERRIDE_PERCEPTION_RADIUS,    ai->overridePerceptionRadius);
                FIELD_CASE_BIT( MISC_PATH_LIMITED,                    ai->pathLimited);
                FIELD_CASE_BIT( MISC_PERCEIVE_ATTACKS_WITHOUT_DAMAGE,ai->perceiveAttacksWithoutDamage);
                FIELD_CASE_BIT( MISC_PREFER_GROUND_COMBAT,            ai->preferGroundCombat);
                FIELD_CASE_BIT(    MISC_PREFER_MELEE,                    ai->preferMelee);
                FIELD_CASE_F32(    MISC_PREFER_MELEE_SCALE,            ai->preferMeleeScale);
                FIELD_CASE_BIT(    MISC_PREFER_RANGED,                    ai->preferRanged);
                FIELD_CASE_F32(    MISC_PREFER_RANGED_SCALE,            ai->preferRangedScale);
                FIELD_CASE_BIT(    MISC_PREFER_UNTARGETED,                ai->preferUntargeted);
                FIELD_CASE_F32(    MISC_PROTECT_SPAWN_RADIUS,            ai->protectSpawnRadius);
                FIELD_CASE_F32(    MISC_PROTECT_SPAWN_OUTSIDE_RADIUS,    ai->protectSpawnOutsideRadius);
                FIELD_CASE_SEC(    MISC_PROTECT_SPAWN_TIME,            ai->protectSpawnDuration);
                FIELD_CASE_BIT(    MISC_RELENTLESS,                    ai->relentless);
                FIELD_CASE_F32(    MISC_RETURN_TO_SPAWN_DISTANCE,        ai->returnToSpawnDistance);
                FIELD_CASE_F32(    MISC_SHOUT_RADIUS,                    ai->shoutRadius);
                FIELD_CASE_INT(    MISC_SHOUT_CHANCE,                    ai->shoutChance);
                FIELD_CASE_BIT(    MISC_SPAWN_POS_IS_SELF,                ai->spawnPosIsSelf);
                FIELD_CASE_F32( MISC_STANDOFF_DISTANCE,                ai->followTarget.standoffDistance);
                FIELD_CASE_F32(    MISC_TARGETED_SIGHT_ADD_RANGE,        ai->targetedSightAddRange);
                FIELD_CASE_BIT(    MISC_TURN_WHILE_IDLE,                ai->turnWhileIdle);
                FIELD_CASE(        MISC_UNTARGETABLE,                    e->untargetable = (e->untargetable & ~UNTARGETABLE_GENERAL) | (setting->value.s32 ? UNTARGETABLE_GENERAL : 0));
                FIELD_CASE_F32(    MISC_WALK_TO_SPAWN_RADIUS,            ai->walkToSpawnRadius);
                //---------------
                FIELD_CASE_INT( POWERS_MAX_PETS,                    ai->power.maxPets);
                FIELD_CASE_BIT(    POWERS_RESURRECT_ARCHVILLAINS,        ai->power.resurrectArchvillains);
                FIELD_CASE_BIT( POWERS_USE_RANGE_AS_MAXIMUM,        ai->power.useRangeAsMaximum);
            }
        }

        MP_CREATE(AIConfigTargetPref, 10);
        for(i = 0; i < eaSize(&config->targetPref); i++)
        {
            newPref = MP_ALLOC(AIConfigTargetPref);
            newPref->archetype = allocAddString(config->targetPref[i]->archetype);
            newPref->factor = config->targetPref[i]->factor;
            eaPush(&ai->targetPref, newPref);
        }
        if (config->AOEUsageConfig)
        {
            ai->AOEUsage.fAOEPreference = (config->AOEUsageConfig->fAOENumTargetMultiplier < 0) ? ai->AOEUsage.fAOEPreference : config->AOEUsageConfig->fAOENumTargetMultiplier;
            ai->AOEUsage.fSingleTargetPreference = (config->AOEUsageConfig->fConfigSingleTargetPreference < 0) ? ai->AOEUsage.fSingleTargetPreference : config->AOEUsageConfig->fConfigSingleTargetPreference;
        }
    }
    else if(topLevel){
        ai->brain.configName = NULL;
    }
}

static int aiInitConfigByName(Entity* e, AIVars* ai, const char* configName)
{
    if(configName){
        AIConfig* config = stashFindPointerReturnPointer(nameToAIConfigTable, configName);

        aiInitConfig(e, ai, config, 1);

        return config ? 1 : 0;
    }

    return 0;
}

void aiSetConfigByName(Entity* e, AIVars* ai, const char* configName)
{
    if(!aiInitConfigByName(e, ai, configName))
        aiInitConfigByName(e, ai, "Default");
}

static const char* aiGetConfig(Entity* e, AIVars* ai)
{
    const VillainDef* def;

    if(e->aiConfig)
        return e->aiConfig;

    def = e->villainDef;
    if(def){
        if(def->aiConfig){
            return def->aiConfig;
        }
        else if(def->name){
            return def->name;
        }
    }

    return allocAddString("Default");
}

static void aiInitConfigFromDef(Entity* e, AIVars* ai)
{
    aiSetBrainType(e, ai, AI_BRAIN_DEFAULT);
    aiSetConfigByName(e, ai, aiGetConfig(e, ai));

    // this will need to be revisited if we add allies that can switch sides.
    if( g_ArchitectTaskForce && (ENTTYPE(e) != ENTTYPE_PLAYER) && e->pchar && 
        e->pchar->iGangID == ARCHITECT_GANG_ALLY && (e->petByScript || !e->erOwner ) )
    {
        aiSetBrainType(e, ai, AI_BRAIN_ARCHITECT_ALLY );

        // remove auto powers
        if ( e->id != architectbuffer_choosenOneId )
        {
            Entity *owner = NULL;
            if (e->erOwner && (owner = erGetEnt(e->erOwner)))
            {
                //    unless the owner is the chosen one
                if (owner->id != architectbuffer_choosenOneId)
                {
                    character_RevokeAutoPowers(e->pchar, 1);
                }
            }
            else
            {
                character_RevokeAutoPowers(e->pchar, 1);
            }
        }
    }
}

static void AIConfigTargetPrefDestroyAdapter(void* arg0)
{
    AIConfigTargetPrefDestroy((AIConfigTargetPref*)arg0);
}

void aiReInitCoh(Entity* e)
{
    AIVars* ai = ENTAI(e);
    eaClearEx(&ai->targetPref, AIConfigTargetPrefDestroyAdapter);
    ZeroStruct(&ai->inputs);
    ai->doNotChangePowers = 0;

    // todo: move this to AIConfig
    ai->base->behaviorCombatOverride = AIB_COMBAT_OVERRIDE_PASSIVE;
    ai->alwaysAttack = 0;
    ai->doNotChangeAttackTarget = 0;
    ai->doNotUsePowers = 0;
    ai->pet.useStayPos = 0;

    if(ENTHIDE(e)){
        SET_ENTHIDE(e) = 0;
        e->draw_update = 1;
    }

    if(e->noDrawOnClient){
        e->noDrawOnClient = 0;
        e->draw_update = 1;
    }

    entSetOnTeam(e, ai->originalAllyID);

    aiInitConfigFromDef(e, ai);
    ai->power.dirty = 0;
    ai->power.groupFlags = 0;
    
    if(ENTTYPE(e) == ENTTYPE_NPC) 
    {
        aiSetMessageHandler(e, aiNPCMsgHandlerDefault);
        if(e->move_type == MOVETYPE_WIRE){
            e->move_type = 0;
        }

        entSetSendOnOddSend(e, 0);

        SET_ENTINFO(e).send_hires_motion = 1;

        entSetProcessRate(e->owner, ENT_PROCESS_RATE_ALWAYS);

        ai->npc.wireWanderMode = 0;
        ai->npc.runningBehavior = 1;
    } 
    else if(ENTTYPE(e) == ENTTYPE_CRITTER) 
    {
        aiSetMessageHandler(e, aiCritterMsgHandlerDefault);
    }

    ai->pathLimited = ai->startedPathLimited;
}

void aiReInitPowerAssignments(Entity* e, AIVars* ai)
{
    int baseConfigCount, i;
    AIConfig* config = stashFindPointerReturnPointer(nameToAIConfigTable, aiGetConfig(e, ai));

    if(config)
    {
        aiDoPowerAssignments(e, ai, config);

        baseConfigCount = eaSize(&config->baseConfigs);

        for(i = 0; i < baseConfigCount; i++){
            AIConfig* baseConfig = config->baseConfigs[i];

            aiDoPowerAssignments(e, ai, baseConfig);
        }
    }
}

// Initialize the AI.

void aiInit(Entity* e, GeneratedEntDef* generatedType){
    AIVars* ai;
    int idx = e->owner;
    EntType entType = ENTTYPE_BY_ID(idx);

    // Init the AI system.

    aiInitSystem();

    if(ENTAI(e))
        return;

    aiInitVars(e);

    ai = ENTAI(e);
    ai->firstThinkTick = 1; // this allows us to let critters run one tick of AI
                            // so they can think before doing certain things (i.e. aiCritterCheckProximity)
    ai->base->behaviorCombatOverride = AIB_COMBAT_OVERRIDE_PASSIVE;
    ai->originalAllyID = e->pchar ? e->pchar->iAllyID : 0;

    aiLogClear(e);


    // Set my attacker reverse pointer thingy.

    ai->attackerInfo.attacker = e;

    // Create the priority manager.

//    aiPriorityInit(e);

    /*
    switch(entType){
    xcase ENTTYPE_CRITTER:
    xcase ENTTYPE_NPC:
        aiBehaviorAddString(e, "PriorityList");
    }
*/

    // Set my initial team stuff.

    ai->teamMemberInfo.entity = e;
    ai->teamMemberInfo.orders.role = AI_TEAM_ROLE_NONE;

    if(e->erCreator){
        ai->teamMemberInfo.alerted = 1;
    }

    // Set some defaults.

    if(    entType == ENTTYPE_PLAYER ||
        entType == ENTTYPE_CRITTER ||
        entType == ENTTYPE_DOOR)
    {
        entSetSendOnOddSend(e, 0);

        SET_ENTINFO(e).send_hires_motion = (entType != ENTTYPE_DOOR);
    }else{
        entSetSendOnOddSend(e, 1);

        SET_ENTINFO(e).send_hires_motion = 1;
    }

    entSetProcessRate(idx, ENT_PROCESS_RATE_ALWAYS);

    switch(entType)
    {
        xcase ENTTYPE_PLAYER:
            if (!e->teamup_id || e->teamup) // otherwise, do this when the teamup is loaded
            {
                alignmentshift_revokeAllBadTips(e); // call this every time, putting the logic in the requires instead of the code
            }
            else
            {
                e->revokeBadTipsOnTeamupLoad = 1;
            }

        xcase ENTTYPE_CRITTER:
            //SET_ENTHIDE(e) = e->pchar->attrMax.fHitPoints > 0; //Martin said this was obsolete

            aiSetTimeslice(e, 100);

            ai->feeling.trackFeelings = 1;

        xcase ENTTYPE_NPC:
            //SET_ENTHIDE(e) = 1; //Martin said this was obsolete

            ai->base->isNPC = 1;
            entSetProcessRate(idx, ENT_PROCESS_RATE_ON_ODD_SEND);

        xcase ENTTYPE_CAR:
            entSetProcessRate(idx, ENT_PROCESS_RATE_ON_ODD_SEND);

        xcase ENTTYPE_DOOR:
            entSetProcessRate(idx, ENT_PROCESS_RATE_ON_SEND);
            aiDoorInit(e);
    }

    ai->time.begin.afraidState = 0;

    aiSetShoutRadius(e, 30);
    aiSetShoutChance(e, 50);

    aiInitMoveType(e);

    aiSetFOVAngle(e, ai, RAD(90));

    // Get the collision width.
    ai->collisionRadius = e->motion->capsule.radius;  //This is only true for uprights, possible bug?

    // Initialize the orientation.

    getMat3YPR(ENTMAT(e), ai->pyr);

    // Set movement variables.

    if(generatedType){
        // Set run speed.

        if(generatedType->runSpeed > 0){
            setSpeed(e, SURFPARAM_GROUND, (generatedType->runSpeed + generatedType->runSpeedVariation * qfrand()) / BASE_PLAYER_FORWARDS_SPEED);
        }

        if(generatedType->priorityList){
//             aiPriorityQueuePriorityList(e, generatedType->priorityList, 1, NULL);
            aiBehaviorAddString(e, ENTAI(e)->base, generatedType->priorityList);
        }

        switch(entType){
            xcase ENTTYPE_NPC:{
                ai->npc.groundOffset = generatedType->groundOffset;
                
                ai->npc.scaredType = generatedType->scaredType;
            }
            
            xcase ENTTYPE_CAR:{
                ai->car.beaconTypeNumber = generatedType->beaconTypeNumber;
            }
        }
    }

    // Create a NavSearch object for pathfinding.

    if(ENTTYPE_PLAYER != ENTTYPE(e)){
        if(!ai->navSearch){
            ai->navSearch = createNavSearch();
        }
    }

    // Create the proximity entity status table.

    ai->proxEntStatusTable = createAIProxEntStatusTable(e, 0);

    // Set initial event time.

    ai->eventHistory.curEventSetStartTime = ABS_TIME;

    // Initialize my power info list.

    if(ENTTYPE(e) != ENTTYPE_PLAYER){
        aiInitPowers(e, ai);
    }

    // todo: move this to AIConfig
    ai->alwaysAttack = 0;
    ai->pet.useStayPos = 0;

    // Set my brain.

    aiInitConfigFromDef(e, ai);

    // Re-initialize position.

    entUpdateMat4Instantaneous(e, ENTMAT(e));

    if(aiCheckIfInPathLimitedVolume(ENTPOS(e)))
        ai->pathLimited = ai->startedPathLimited = 1;

}

// Destroys all the ai variables.

void aiDestroy(Entity* e){
    AIVars* ai = ENTAI(e);
    int isPlayer = ENTTYPE(e) == ENTTYPE_PLAYER;

    if(!ai)
        return;

    PERFINFO_AUTO_START("aiDestroy", 1);
    
            // The powers pets array gets entities added to it by the script, but then when those entities get deleted
            // the script has no way to clean them up... and seeing as it's just an array of indices for the entities
            // array, that could cause problems...

            if(e->erOwner)
                PlayerRemovePet(erGetEnt(e->erCreator), e);

        PERFINFO_AUTO_START("aiBehaviorDestroy", 1);

            // Destroy all behavior stuff

            aiBehaviorDestroyAll(e, ai->base, &ai->base->behaviors);
            aiBehaviorDestroyMods(e, ai->base);
            aiSystemDestroyBase(ai->base);
            ai->base = NULL;

        PERFINFO_AUTO_STOP_START("aiTeamLeave", 1);

            // Remove from my team.

            aiTeamLeave(&ai->teamMemberInfo);

        PERFINFO_AUTO_STOP_START("aiDiscardAttackTarget", 1);

            // Make attackers untarget me.

            while(ai->attackerList.count){
                Entity* attacker = ai->attackerList.head->attacker;
                AIVars* aiAttacker = ENTAI(attacker);

                assert(aiAttacker->attackTarget.entity == e);

                aiDiscardAttackTarget(attacker, "Target was destroyed.");
            }

        PERFINFO_AUTO_STOP_START("removeStatusEntries", 1);
            
            aiProximityRemoveEntitysStatusEntries(e, ai, !isPlayer);

        PERFINFO_AUTO_STOP_START("TargetPref", 1);
        {
            AIConfigTargetPref* pref;

            while(pref = eaPop(&ai->targetPref))
            {
                MP_FREE(AIConfigTargetPref, pref);
            }
            eaDestroy(&ai->targetPref);
        }

        PERFINFO_AUTO_STOP_START("aiDiscardAttackTarget", 1);

            // Untarget my target.

            aiDiscardAttackTarget(e, "Being destroyed.");

            // Free my timeslice.

            aiSetTimeslice(e, 0);

        PERFINFO_AUTO_STOP_START("destroyAIProxEntStatusTable", 1);

            // Destroy my entity status table.

            if(ai->proxEntStatusTable){
                destroyAIProxEntStatusTable(ai->proxEntStatusTable);
                ai->proxEntStatusTable = NULL;
            }

        PERFINFO_AUTO_STOP_START("aiEventClearHistory", 1);

            // Destroy my event history.

            aiEventClearHistory(ai);

        PERFINFO_AUTO_STOP_START("destroyNavSearch", 1);

            // Destroy my search stuff.

            if(ai->navSearch){
                destroyNavSearch(ai->navSearch);
                ai->navSearch = NULL;
            }

        PERFINFO_AUTO_STOP_START("aiDestroyPowers", 1);

            // Destroy power info.

            aiDestroyPowers(e, ai);

        PERFINFO_AUTO_STOP_START("aiFeelingDestroy", 1);

            // Destroy feeling info.

            aiFeelingDestroy(e, ai);

        PERFINFO_AUTO_STOP_START("aiRemoveAllAvoidInstances", 1);
        
            // Remove all avoid instances.
        
            aiRemoveAllAvoidInstances(e, ai);

        PERFINFO_AUTO_STOP_START("aiLogClear", 1);

            // Remove all allocated log space

            aiLogClear(e);

        PERFINFO_AUTO_STOP_START("free", 1);

            // Free my AI structure.

            MP_FREE(AIVars, ai);

            // Set my ai to NULL.

            SET_ENTAI(e) = NULL;

        PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();
}

void* aiActivityDataAlloc(Entity* e, U32 size){
    AIVars* ai = ENTAI(e);

    // Init activity data pool.

    if(!staticActivityDataPool){
        U32 curSize;
        U32 maxSize = 0;

        staticActivityDataPool = createMemoryPool();

        #define GET_SIZE(x) curSize = x;if(curSize > maxSize) maxSize = curSize

            GET_SIZE(aiNPCGetActivityDataSize());
            GET_SIZE(aiCritterGetActivityDataSize());

        #undef GET_SIZE

        initMemoryPool(staticActivityDataPool, maxSize, 100);
    }

    assert(size <= mpStructSize(staticActivityDataPool));

    ai->activityDataAllocated++;

    assert(ai->activityDataAllocated > 0);

    return mpAlloc(staticActivityDataPool);
}

void aiActivityDataFree(Entity* e, void* data){
    AIVars* ai = ENTAI(e);

    if(!data)
        return;

    assert(staticActivityDataPool);

    assert(ai->activityDataAllocated > 0);

    ai->activityDataAllocated--;

    mpFree(staticActivityDataPool, data);
}

const char* aiGetActivityName(AIActivity activity){
    switch(activity){
        case AI_ACTIVITY_COMBAT:
            return "Combat";
        case AI_ACTIVITY_DO_ANIM:
            return "DoAnim";
        case AI_ACTIVITY_FOLLOW_LEADER:
            return "FollowLeader";
        case AI_ACTIVITY_FOLLOW_ROUTE:
            return "FollowRoute";
        case AI_ACTIVITY_FROZEN_IN_PLACE:
            return "FrozenInPlace";
        case AI_ACTIVITY_HIDE_BEHIND_ENT:
            return "HideBehindEnt";
        case AI_ACTIVITY_NPC_HOSTAGE:
            return "Npc.hostage";
        case AI_ACTIVITY_PATROL:
            return "Patrol";
        case AI_ACTIVITY_RUN_AWAY:
            return "RunAway";
        case AI_ACTIVITY_RUN_INTO_DOOR:
            return "RunIntoDoor";
        case AI_ACTIVITY_RUN_OUT_OF_DOOR:
            return "RunOutOfDoor";
        case AI_ACTIVITY_RUN_TO_TARGET:
            return "RunToTarget";
        case AI_ACTIVITY_STAND_STILL:
            return "StandStill";
        case AI_ACTIVITY_THANK_HERO:
            return "NPC.ThankHero";
        case AI_ACTIVITY_WANDER:
            return "Wander";
        default:
            return "Unknown";
    }
}

