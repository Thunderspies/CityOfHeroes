#include <utilitieslib/stdtypes.h>
#include "entaiBehaviorCoh.h"
#include "ailib/aiBehaviorPublic.h"
#include "ailib/aiBehaviorInterface.h"
#include "ailib/aiStruct.h"
#include "ai/entaiBehaviorStruct.h"

#include "seq/AnimBitList.h"
#include "bases/basesystems.h"
#include "beacon/beacon.h"
#include "beacon/beaconPath.h"
#include "beacon/beaconPrivate.h"
#include "entity/character_base.h"
#include "dbcomm/dbdoor.h"
#include "gameSys/dooranim.h"
#include <utilitieslib/components/earray.h>
#include "generator/encounter.h"
#include "generator/encounterPrivate.h"
#include "entai.h"
#include "entaiCritterPrivate.h"
#include "entaiLog.h"
#include "entaivars.h"
#include "entaiprivate.h"
#include "entaiPriority.h"
#include "entaiPriorityPrivate.h"
#include "entaiScript.h"
#include "entity/entity.h"
#include "entity/entserver.h"
#include "entity/entGameActions.h"
#include <utilitieslib/utils/error.h>
#include "group/group.h"
#include "language/langServerUtil.h"
#include <utilitieslib/utils/mathutil.h>
#include "gridcoll/megaGrid.h"
#include "entity/motion.h"
#include "entity/netfx.h"
#include "player/pmotion.h"
#include "entity/powers.h"
#include "script/scriptengine.h"
#include "seq/seq.h"
#include "seq/seqstate.h"
#include <utilitieslib/components/StringCache.h>
#include "storyarc/storyarcutil.h"
#include <utilitieslib/utils/SuperAssert.h>
#include <utilitieslib/utils/textparser.h>
#include "gameComm/VillainDef.h"
#include "entity/character_combat.h"
#include "entity/character_tick.h"
#include "script/Scripthook/ScriptHookEntityTeam.h"
#include "script/Scripthook/ScriptHookInternal.h"
#include "script/Scripthook/ScriptHookMission.h"
#include "entity/character_animfx.h" // temporary for ExitStance
#include "language/commonLangUtil.h"

typedef struct AIBehaviorInfo AIBehaviorInfo;

// CoH Backward compatibility functions
static AIBehaviorInfo* priorityListBehaviorInfo = NULL;

#define MAX_RUN_INTO_DOOR_TIME (60.0)

static void aiBehaviorCohInitSpecialBehaviorInfo()
{
    priorityListBehaviorInfo = aiBehaviorInfoFromString("PriorityList");
}

AIBehavior* aiBehaviorGetPLBehavior(Entity* e, int add);

AIActivity aiBehaviorGetActivity(Entity* e)
{
    AIBehavior* PLBehavior = aiBehaviorGetPLBehavior(e, 0);
    AIBDPriorityList* mydata;

    if(!priorityListBehaviorInfo)
        aiBehaviorCohInitSpecialBehaviorInfo();

    if(!PLBehavior || PLBehavior->info != priorityListBehaviorInfo)
        return AI_ACTIVITY_NONE;

    mydata = (AIBDPriorityList*) PLBehavior->data;
    return mydata->activity;
}

AIBehavior* aiBehaviorGetPLBehavior(Entity* e, int add)
{
    int i;
    AIVars* ai;
    AIBehavior* PLBehavior = NULL;

    if(!e || !(ai = ENTAI(e)) || !ai->base)
        return NULL;

    if(!priorityListBehaviorInfo)
        aiBehaviorCohInitSpecialBehaviorInfo();

    for(i = eaSize(&ai->base->behaviors)-1; i >= 0; i--)
    {
        if(ai->base->behaviors[i]->info == priorityListBehaviorInfo)
            return ai->base->behaviors[i];
    }

    if(add)
        aiBehaviorAddString(e, ai->base, "PriorityList");

    return NULL;
}

AIPriorityManager* aiBehaviorGetPriorityManager(Entity* e, int add, int clearFinish)
{
    AIBehavior* PLBehavior;

    if(PLBehavior = aiBehaviorGetPLBehavior(e, add))
    {
        AIBDPriorityList* mydata = (AIBDPriorityList*) PLBehavior->data;
        if(clearFinish)
            PLBehavior->finished = 0;
        return mydata->manager;
    }

    return NULL;
}

void aiBehaviorAddPLFlag(Entity* e, char* string)
{
    AIVars* ai = ENTAI(e);
    AIBehavior* behavior = aiBehaviorGetCurBehaviorInternal(e, ai->base);
    AIBDPriorityList* mydata;

    if(!priorityListBehaviorInfo)
        aiBehaviorCohInitSpecialBehaviorInfo();

    if(!string || !behavior || behavior->info != priorityListBehaviorInfo)
        return;

    mydata = behavior->data;

    if(!mydata->scopeVar)
        mydata->scopeVar = aiBehaviorCreateSCOPEVAR(e, ai->base, &ai->base->behaviors, eaSize(&ai->base->behaviors)-1);

    if(devassert(mydata->scopeVar))
        aiBehaviorProcessFlag(e, ai->base, &ai->base->behaviors, mydata->scopeVar, string);
}

void aiBehaviorSetActivity(Entity* e, AIActivity activity)
{
    AIBehavior* PLBehavior = aiBehaviorGetPLBehavior(e, 1);

    if(PLBehavior)
    {
        AIBDPriorityList* mydata = (AIBDPriorityList*) PLBehavior->data;
        mydata->activity = activity;
    }
}

// CoH behaviors

AIBehaviorInfo BehaviorTable[] = {
    // Name,
    { "PERMVAR",
    //    Start,                    Finish,                    Set,                    Unset,
    NULL,                    NULL,                    NULL,                    NULL,
    //    Run,                    Offtick,
    NULL,                    NULL,
    //    Flag,                    Input String,
    NULL,                    NULL,
    //    Debug String,
    NULL,
    //    Struct size,            Uses group data,        Flags
    0,                        0,                        AIB_NPC_ALLOWED    },
    { "SCOPEVAR",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFSCOPEVARRun,        NULL,                    
    NULL,                    NULL,
    NULL,
    0,                        0,                        AIB_NPC_ALLOWED    },
    { "Combat",
    aiBFCombatStart,        NULL,                    aiBFCombatSet,            aiBFCombatUnset,
    aiBFCombatRun,            NULL,                    
    NULL,                    aiBFCombatString,
    aiBFCombatDebugStr,
    sizeof(AIBDCombat),        0,                        0    },
    { "PriorityList",
    aiBFPriorityListStart,    aiBFPriorityListFinish,    aiBFPriorityListSet,    aiBFPriorityListUnset,
    aiBFPriorityListRun,    NULL,                    
    aiBFPriorityListFlag,    aiBFPriorityListString,
    aiBFPriorityListDebugStr,
    sizeof(AIBDPriorityList),0,                        AIB_NPC_ALLOWED    },

    //--------------------------------------------------------------------------------------------------
    // ONLY ADD BEHAVIORS AFTER THIS LINE, THE ABOVE ALL NEED SPECIAL PROCESSING USING THEIR TABLE INDEX
    //--------------------------------------------------------------------------------------------------


    { "AlertTeam",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFAlertTeamRun,        NULL,                    
    NULL,                    NULL,
    NULL,
    0,                        0,                        0    },
    { "AttackTarget",
    aiBFAttackTargetStart,    NULL,                    aiBFAttackTargetSet,    aiBFAttackTargetUnset,
    aiBFAttackTargetRun,    NULL,                    
    aiBFAttackTargetFlag,    NULL,
    NULL,
    sizeof(AIBDAttackTarget),0,                        0    },
    { "AttackVolumes",
    NULL,                    aiBFAttackVolumesFinish,aiBFAttackVolumesSet,    NULL,
    aiBFAttackVolumesRun,    NULL,                    
    aiBFAttackVolumesFlag,    NULL,
    aiBFAttackVolumesDebugStr,
    sizeof(AIBDAttackVolumes),0,                    0    },
    { "BallTurretBaseCheckDeath",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFBallTurretBaseCheckDeathRun, NULL,                    
    NULL,                    NULL,
    NULL,
    0,                        0,                        0    },
    { "BallTurretSlideOut",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFBallTurretSlideOutRun, NULL,                    
    NULL,                    NULL,
    NULL,
    0,                        0,                        0    },
    { "BehaviorDone",                                // Renamed DoNothing - used for scripts to track when a longer behavior has been finished/exited
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFDoNothingRun,        NULL,                    
    NULL,                    aiBFDoNothingString,
    NULL,
    sizeof(AIBDDoNothing),    0,                        AIB_NPC_ALLOWED    },
    { "CallForHelp",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFCallForHelpRun,        NULL,                    
    NULL,                    aiBFCallForHelpString,
    NULL,
    sizeof(AIBDCallForHelp),0,                        0    },
    { "CameraScan",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFCameraScanRun,        NULL,                    
    aiBFCameraScanFlag,        NULL,
    NULL,
    sizeof(AIBDCameraScan),    0,                        0    },
    { "Chat",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFChatRun,            NULL,                    
    NULL,                    NULL,
    NULL,
    0,                        0,                        AIB_NPC_ALLOWED    },
    { "CutScene",
    NULL,                    aiBFAliasFinish,        aiBFAliasSet,            NULL,
    aiBFAliasRun,            NULL,                    
    NULL,                    aiBFCutSceneString,
    aiBFAliasDebugStr,
    sizeof(AIBDAlias),        0,                        AIB_NPC_ALLOWED    },
    { "DestroyMe",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFDestroyMeRun,        NULL,                    
    NULL,                    NULL,
    NULL,
    0,                        0,                        AIB_NPC_ALLOWED    },
    { "DisappearInStandardDoor",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFDisappearInStandardDoorRun,    NULL,                    
    NULL,                    NULL,
    NULL,
    sizeof(AIBDDisappearInStandardDoor),0,                        AIB_NPC_ALLOWED    },
    { "DoNothing",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFDoNothingRun,        NULL,                    
    NULL,                    aiBFDoNothingString,
    NULL,
    sizeof(AIBDDoNothing),    0,                        AIB_NPC_ALLOWED    },
    { "Flag",
    NULL,                    NULL,                    NULL,                    NULL,
    NULL,                    NULL,                    
    NULL,                    NULL,
    NULL,
    0,                        0,                        AIB_NPC_ALLOWED    },
    { "HideBehindEnt",
    NULL,                    NULL,                    aiBFHideBehindEntSet,    NULL,
    aiBFHideBehindEntRun,    NULL,                    
    NULL,                    NULL,
    NULL,
    sizeof(AIBDHideBehindEnt), 0,                    0 },
    { "HostageScared",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFHostageScaredRun,    NULL,                    
    NULL,                    NULL,
    NULL,
    sizeof(AIBDHostageScared), 0,                    AIB_NPC_ALLOWED },
    { "Loop",
    NULL,                    aiBFLoopFinish,            NULL,                    NULL,
    aiBFLoopRun,            NULL,                    
    NULL,                    aiBFLoopString,
    aiBFLoopDebugStr,
    sizeof(AIBDLoop),        0,                        AIB_NPC_ALLOWED    },
    { "MoveToPos",
    NULL,                    NULL,                    aiBFMoveToPosSet,        aiBFMoveToPosUnset,
    aiBFMoveToPosRun,        NULL,                    
    aiBFMoveToPosFlag,        NULL,
    aiBFMoveToPosDebugStr,
    sizeof(AIBDMoveToPos),    0,                        AIB_NPC_ALLOWED    },
    { "MoveToRandPoint",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFMoveToRandPointRun,    NULL,                    
    NULL,                    aiBFMoveToRandPointString,
    NULL,
    sizeof(AIBDMoveToRandPoint), 0,                    AIB_NPC_ALLOWED    },
    { "Patrol",
    aiBFPatrolStart,        NULL,                    aiBFPatrolSet,            aiBFPatrolUnset,
    aiBFPatrolRun,            NULL,                    
    aiBFPatrolFlag,            aiBFPatrolString,
    aiBFPatrolDebugStr,
    sizeof(AIBDPatrol),        0,                        0    },
    { "Pet",
    aiBFPetStart,            aiBFPetFinish,            aiBFPetSet,                aiBFPetUnset,
    aiBFPetRun,                NULL,                    
    NULL,                    aiBFPetString,
    aiBFPetDebugStr,
    sizeof(AIBDPet),        0,                        0    },
    { "Pet.Follow",
    NULL,                    NULL,                    aiBFPetFollowSet,        NULL,
    aiBFPetFollowRun,        NULL,                    
    NULL,                    NULL,
    NULL,
    0,                        0,                        AIB_NPC_ALLOWED    },
    { "PickRandomWeapon",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFPickRandomWeaponRun,NULL,                    
    NULL,                    NULL,
    NULL,
    0,                        0,                        0    },
    { "PlayFX",
    NULL,                    aiBFPlayFXFinish,        NULL,                    NULL,
    aiBFPlayFXRun,            NULL,                    
    NULL,                    aiBFPlayFXString,
    NULL,
    sizeof(AIBDPlayFX),        0,                        AIB_NPC_ALLOWED    },
    { "RunAwayFar",
    NULL,                    NULL,                    aiBFRunAwayFarSet,        NULL,
    aiBFRunAwayFarRun,        NULL,                    
    NULL,                    aiBFRunAwayFarString,
    NULL,
    sizeof(AIBDRunAwayFar),    0,                        AIB_NPC_ALLOWED    },
    { "RunIntoDoor",
    NULL,                    NULL,                    aiBFRunIntoDoorSet,        NULL,
    aiBFRunIntoDoorRun,        NULL,                    
    NULL,                    NULL,
    NULL,
    sizeof(AIBDRunIntoDoor),0,                        AIB_NPC_ALLOWED    },
    { "RunOutOfDoor",
    aiBFRunOutOfDoorStart,    NULL,                    aiBFRunOutOfDoorSet,    NULL,
    aiBFRunOutOfDoorRun,    aiBFRunOutOfDoorOffTick,    
    NULL,                    NULL,
    NULL,
    sizeof(AIBDRunOutOfDoor),0,                        0    },
    { "RunThroughDoor",
    aiBFRunThroughDoorStart,aiBFRunThroughDoorFinish,aiBFRunThroughDoorSet,    NULL,
    aiBFRunThroughDoorRun,    NULL,                    
    NULL,                    aiBFRunThroughDoorString,
    NULL,
    sizeof(AIBDRunThroughDoor),0,                    AIB_NPC_ALLOWED    },
    { "Say",
    NULL,                    aiBFSayFinish,            NULL,                    NULL,
    aiBFSayRun,                NULL,
    aiBFSayFlag,            aiBFSayString,
    NULL,
    sizeof(AIBDSay),        0,                        AIB_NPC_ALLOWED    },
    { "SetHealth",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFSetHealthRun,        NULL,
    NULL,                    aiBFSetHealthString,
    NULL,
    sizeof(AIBDSetHealth),        0,                    0    },
    { "SetObjective",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFSetObjectiveRun,    NULL,
    NULL,                    aiBFSetObjectiveString,
    NULL,
    sizeof(AIBDSetObjective),        0,                    AIB_NPC_ALLOWED    },
    { "Test",
    aiBFTestStart,            aiBFTestFinish,            aiBFTestSet,            aiBFTestUnset,
    aiBFTestRun,            aiBFTestOffTick,        
    aiBFTestFlag,            aiBFTestString,
    aiBFTestDebugStr,
    sizeof(AIBDTest),        0,                        AIB_NPC_ALLOWED    },
    { "ThankHero",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFThankHeroRun,        NULL,                    
    NULL,                    NULL,
    NULL,
    sizeof(AIBDThankHero),    0,                        AIB_NPC_ALLOWED    },
    { "UsePower",
    aiBFUsePowerStart,        NULL,                    aiBFUsePowerSet,        aiBFUsePowerUnset,
    aiBFUsePowerRun,        NULL,                    
    NULL,                    aiBFUsePowerString,
    NULL,
    sizeof(AIBDUsePower),    0,                        0    },
    { "WanderAround",
    NULL,                    NULL,                    aiBFWanderAroundSet,    NULL,
    aiBFWanderAroundRun,    NULL,                    
    NULL,                    aiBFWanderAroundString,
    aiBFWanderAroundDebugStr,
    sizeof(AIBDWanderAround),0,                        AIB_NPC_ALLOWED    },
    { "PatrolRiktiDropship",        // This allows the critter to use passing combat powers
    aiBFPatrolStart,        NULL,                    aiBFPatrolSet,            NULL,
    aiBFPatrolRiktiDropshipRun,            NULL,                    
    aiBFPatrolFlag,            aiBFPatrolString,
    aiBFPatrolDebugStr,
    sizeof(AIBDPatrol),        0,                        0    },

};

// CoH flags

typedef enum AIBehaviorFlagType
{
    // Behavior Input Flags
    AIBF_ENTITY_REF,
    AIBF_FLOATPARAM,
    AIBF_INTPARAM,
    AIBF_MOTION_WIRE,
    AIBF_POS,
    AIBF_SCRIPT_LOCATION,
    AIBF_STRINGPARAM,
    AIBF_VARIATION,
    // Generic Setting Flags
    AIBF_AGGRO_NON_AGGRO_TARGETS,
    AIBF_ALWAYS_ATTACK,
    AIBF_ALWAYS_GET_HIT,
    AIBF_ALWAYS_RETURN_TO_SPAWN,
    AIBF_ANIM_BIT,
    AIBF_ANIM_CLEAR_BIT,
    AIBF_ANIM_LIST,
    AIBF_BOOST_SPEED,
    AIBF_COMBAT_MOD_SHIFT,
    AIBF_COMBAT_OVERRIDE,
    AIBF_CONSIDER_ENEMY_LEVEL,
    AIBF_CREATOR,
    AIBF_DO_ATTACK_AI,
    AIBF_DO_NOT_AGGRO_CRITTERS,
    AIBF_DO_NOT_CHANGE_POWERS,
    AIBF_DO_NOT_DRAW_AGGRO,
    AIBF_DO_NOT_DRAW_ON_CLIENT,
    AIBF_DO_NOT_FACE_TARGET,
    AIBF_DO_NOT_GO_TO_SLEEP,
    AIBF_DO_NOT_MOVE,
    AIBF_DO_NOT_RUN,
    AIBF_DO_NOT_USE_ANY_POWERS,
    AIBF_END_CONDITION,
    AIBF_FEELING_BOTHERED,
    AIBF_FEELING_CONFIDENCE,
    AIBF_FEELING_FEAR,
    AIBF_FEELING_LOYALTY,
    AIBF_FIND_TARGET,
    AIBF_FLY,
    AIBF_FOV_DEGREES,
    AIBF_GANG,
    AIBF_GRIEF_HP_RATIO,
    AIBF_HAS_PATROL_POINT,
    AIBF_I_AM_A,
    AIBF_INVINCIBLE,
    AIBF_INVISIBLE,
    AIBF_I_REACT_TO,
    AIBF_JUMPHEIGHT,
    AIBF_JUMPSPEED,
    AIBF_LIMITED_SPEED,
    AIBF_NO_PHYSICS,
    AIBF_OVERRIDE_PERCEPTION_RADIUS,
    AIBF_OVERRIDE_SPAWN_POS,
    AIBF_OVERRIDE_SPAWN_POS_CUR,
    AIBF_PATH_LIMITED,
    AIBF_PATROL_FLY_TURN_RADIUS,
    AIBF_PROTECT_SPAWN_OUTER_RADIUS,
    AIBF_PROTECT_SPAWN_RADIUS,
    AIBF_PROTECT_SPAWN_DURATION,
    AIBF_RETURN_TO_SPAWN_DISTANCE,
    AIBF_SET_ATTACK_TARGET,
    AIBF_SET_NEW_SPAWN_POS,
    AIBF_SPAWN_POS_IS_SELF,
    AIBF_STANDOFF_DISTANCE,
    AIBF_TARGET_INSTIGATOR,
    AIBF_TEAM,
    AIBF_TIMER,
    AIBF_UNTARGETABLE,
    AIBF_UNSELECTABLE,
    AIBF_USE_ENTERABLE,
    AIBF_WALK_TO_SPAWN_RADIUS,
    AIBF_IGNORE_COMBAT_MODS,
    AIBF_ALWAYS_HIT,
    AIBF_LEASHED_TO_SPAWN,
    AIBF_VULNERABLE_LEASHED_TO_SPAWN,
    AIBF_DEPRECATED,
    AIBF_DISABLE_AUTO_POWERS,
    AIBF_PASS_TO_PETS,
    AIBF_IGNORE_COMBAT_MOD_SHIFT,
    AIBF_CON_YELLOW,
    AIBF_REWARD_SCALE_OVERRIDE,
    AIBF_RENAME,
    AIBF_EXIT_STANCE,
    AIBF_PRIORITY,
    AIBF_COUNT
}AIBehaviorFlagType;

StaticDefineInt ParseCombatOverride[] = {
    DEFINE_INT
    { "Passive",        AIB_COMBAT_OVERRIDE_PASSIVE            },
    { "Defensive",        AIB_COMBAT_OVERRIDE_DEFENSIVE        },
    { "Collective",        AIB_COMBAT_OVERRIDE_COLLECTIVE        },
    { "Aggressive",        AIB_COMBAT_OVERRIDE_AGGRESSIVE        },
    DEFINE_END
};

AIBehaviorFlagInfo BehaviorFlagTable[] = {
    { AIBF_ENTITY_REF,                    "EntRef",                    FLAG_DATA_ENTREF,    NULL,        0    },
    { AIBF_FLOATPARAM,                    "Float",                    FLAG_DATA_VEC3,        NULL,        0    },
    { AIBF_INTPARAM,                    "Int",                        FLAG_DATA_INT3,        NULL,        0    },
    { AIBF_MOTION_WIRE,                    "MotionWire",                FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_POS,                            "Pos",                        FLAG_DATA_VEC3,        NULL,        0    },
    { AIBF_SCRIPT_LOCATION,                "ScriptLocation",            FLAG_DATA_SPECIAL,    NULL,        0    },
    { AIBF_STRINGPARAM,                    "String",                    FLAG_DATA_ALLOCSTR,    NULL,        0    },
    { AIBF_VARIATION,                    "Variation",                FLAG_DATA_SPECIAL,    NULL,        0    },

    { AIBF_AGGRO_NON_AGGRO_TARGETS,        "AggroNonAggroTargets",        FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_ALWAYS_ATTACK,                "AlwaysAttack",                FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_ALWAYS_GET_HIT,                "AlwaysGetHit",                FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_ALWAYS_RETURN_TO_SPAWN,        "AlwaysReturnToSpawn",        FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_ANIM_BIT,                    "AnimBit",                    FLAG_DATA_MULT_STR,    NULL,        1    },
    { AIBF_ANIM_CLEAR_BIT,                "AnimClearBit",                FLAG_DATA_MULT_STR,    NULL,        1    },
    { AIBF_ANIM_LIST,                    "AnimList",                    FLAG_DATA_ALLOCSTR, NULL,        0    },
    { AIBF_BOOST_SPEED,                    "BoostSpeed",                FLAG_DATA_FLOAT,    NULL,        0    },
    { AIBF_COMBAT_MOD_SHIFT,            "CombatModShift",            FLAG_DATA_INT,        NULL,        0    },
    { AIBF_COMBAT_OVERRIDE,                "CombatOverride",            FLAG_DATA_PARSE,ParseCombatOverride,0},
    { AIBF_CONSIDER_ENEMY_LEVEL,        "ConsiderEnemyLevel",        FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_CREATOR,                        "Creator",                    FLAG_DATA_ALLOCSTR,    NULL,        0    },
    { AIBF_DO_NOT_AGGRO_CRITTERS,        "DoNotAggroCritters",        FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_DO_NOT_CHANGE_POWERS,        "DoNotChangePowers",        FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_DO_NOT_DRAW_AGGRO,            "DoNotDrawAggro",            FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_DO_NOT_DRAW_ON_CLIENT,        "DoNotDrawOnClient",        FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_DO_NOT_FACE_TARGET,            "DoNotFaceTarget",            FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_DO_NOT_GO_TO_SLEEP,            "DoNotGoToSleep",            FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_DO_NOT_MOVE,                    "DoNotMove",                FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_DO_NOT_RUN,                    "DoNotRun",                    FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_END_CONDITION,                "EndCondition",                FLAG_DATA_SPECIAL,    NULL,        1    },
    { AIBF_FEELING_BOTHERED,            "FeelingBothered",            FLAG_DATA_PERCENT,    NULL,        0    },
    { AIBF_FEELING_CONFIDENCE,            "FeelingConfidence",        FLAG_DATA_PERCENT,    NULL,        0    },
    { AIBF_FEELING_FEAR,                "FeelingFear",                FLAG_DATA_PERCENT,    NULL,        0    },
    { AIBF_FEELING_LOYALTY,                "FeelingLoyalty",            FLAG_DATA_PERCENT,    NULL,        0    },
    { AIBF_FIND_TARGET,                    "FindTarget",                FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_FLY,                            "Fly",                        FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_FOV_DEGREES,                    "FOVDegrees",                FLAG_DATA_INT,        NULL,        0    },
    { AIBF_GRIEF_HP_RATIO,                "GriefHPRatio",                FLAG_DATA_FLOAT,    NULL,        0    },
    { AIBF_HAS_PATROL_POINT,            "HasPatrolPoint",            FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_I_AM_A,                        "IAmA",                        FLAG_DATA_MULT_STR,    NULL,        1    },
    { AIBF_INVINCIBLE,                    "Invincible",                FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_INVISIBLE,                    "Invisible",                FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_I_REACT_TO,                    "IReactTo",                    FLAG_DATA_MULT_STR,    NULL,        1    },
    { AIBF_JUMPHEIGHT,                    "JumpHeight",                FLAG_DATA_FLOAT,    NULL,        0    },
    { AIBF_JUMPSPEED,                    "SuperJump",                FLAG_DATA_FLOAT,    NULL,        0    },
    { AIBF_LIMITED_SPEED,                "LimitedSpeed",                FLAG_DATA_FLOAT,    NULL,        0    },
    { AIBF_NO_PHYSICS,                    "NoPhysics",                FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_OVERRIDE_PERCEPTION_RADIUS,    "OverridePerceptionRadius",    FLAG_DATA_FLOAT,    NULL,        0    },
    { AIBF_OVERRIDE_SPAWN_POS,            "OverrideSpawnPos",            FLAG_DATA_VEC3,        NULL,        0    },
    { AIBF_OVERRIDE_SPAWN_POS_CUR,        "OverrideSpawnPosCur",        FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_PATH_LIMITED,                "PathLimited",                FLAG_DATA_ALLOCSTR,    NULL,        0    },
    { AIBF_PROTECT_SPAWN_OUTER_RADIUS,    "ProtectSpawnOuterRadius",    FLAG_DATA_FLOAT,    NULL,        0    },
    { AIBF_PROTECT_SPAWN_RADIUS,        "ProtectSpawnRadius",        FLAG_DATA_FLOAT,    NULL,        0    },
    { AIBF_PROTECT_SPAWN_DURATION,        "ProtectSpawnDuration",        FLAG_DATA_FLOAT,    NULL,        0    },
    { AIBF_RETURN_TO_SPAWN_DISTANCE,    "ReturnToSpawnDistance",    FLAG_DATA_FLOAT,    NULL,        0    },
    { AIBF_SET_ATTACK_TARGET,            "SelectAttackTarget",        FLAG_DATA_INT,        NULL,        0    },
    { AIBF_SET_NEW_SPAWN_POS,            "SetNewSpawnPos",            FLAG_DATA_ALLOCSTR,    NULL,        0    },
    { AIBF_SPAWN_POS_IS_SELF,            "SpawnPosIsSelf",            FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_STANDOFF_DISTANCE,            "StandoffDistance",            FLAG_DATA_FLOAT,    NULL,        0    },
    { AIBF_TARGET_INSTIGATOR,            "TargetInstigator",            FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_TEAM,                        "Team",                        FLAG_DATA_PARSE,    ParseTeamNames,0},
    { AIBF_GANG,                        "Gang",                        FLAG_DATA_INT,        NULL,        0    },
    { AIBF_TIMER,                        "Timer",                    FLAG_DATA_FLOAT,    NULL,        0    },
    { AIBF_UNTARGETABLE,                "Untargetable",                FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_UNSELECTABLE,                "Unselectable",                FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_USE_ENTERABLE,                "UseEnterable",                FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_WALK_TO_SPAWN_RADIUS,        "WalkToSpawnRadius",        FLAG_DATA_FLOAT,    NULL,        0    },
    { AIBF_PATROL_FLY_TURN_RADIUS,        "FlyTurnRadius",            FLAG_DATA_FLOAT,    NULL,        0    },
    { AIBF_DO_ATTACK_AI,                "DoAttackAI",                FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_DO_NOT_USE_ANY_POWERS,        "DoNotUseAnyPowers",        FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_IGNORE_COMBAT_MODS,            "IgnoreCombatMods",            FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_ALWAYS_HIT,                    "AlwaysHit",                FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_LEASHED_TO_SPAWN,            "LeashedToSpawnDistance",    FLAG_DATA_FLOAT,    NULL,        0    },
    { AIBF_VULNERABLE_LEASHED_TO_SPAWN, "VulnerableLeashedToSpawnDistance", FLAG_DATA_FLOAT,    NULL,0    },
    { AIBF_DEPRECATED,                    "LeashedToSpawn",            FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_DISABLE_AUTO_POWERS,            "DisableAutoPowers",        FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_PASS_TO_PETS,                "PassToPets",                FLAG_DATA_ALLOCSTR,    NULL,        0    },
    { AIBF_IGNORE_COMBAT_MOD_SHIFT,        "IgnoreCombatModLevelShift",    FLAG_DATA_INT,    NULL,        0    },
    { AIBF_CON_YELLOW,                    "ConYellow",                FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_REWARD_SCALE_OVERRIDE,        "RewardScaleOverride",        FLAG_DATA_FLOAT,    NULL,        0    },
    { AIBF_RENAME,                        "Rename",                    FLAG_DATA_ALLOCSTR,    NULL,        0    },
    { AIBF_EXIT_STANCE,                    "ExitStance",                FLAG_DATA_BOOL,        NULL,        0    },
    { AIBF_PRIORITY,                    "Priority",                    FLAG_DATA_INT,        NULL,        0    },
};

#define FLAG_DEBUG_PRINT                                                            \
    AI_LOG(AI_LOG_BEHAVIOR, (e, "Processing flag: ^8%s^0 on ^9%s^2%i\n",            \
    curFlag->info->name, behavior->info->name,                                        \
    aiBehaviorGetHandle(e, aibase, behaviors, behavior)))

#define REMOVE_AND_CONTINUE                                                            \
    FLAG_DEBUG_PRINT                                                                \
    aiBehaviorFlagDestroy(e, aibase, (AIBehaviorFlag*)eaRemove(&behavior->flags, i));    \
    continue;

// WARNING: currently flags are allowed to be processed multiple times
void aiBehaviorCohFlagProcess(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    int i = 0, j;
    AIVars* ai = ENTAI(e);

    while(i < eaSize(&behavior->flags))
    {
        AIBehaviorFlag* curFlag = behavior->flags[i];

        // let the behavior catch flags if it wants to (and get rid of the flags you process whenever you can
        // to cut down on reprocessing time on behavior cleanup
        // Flag function return values:
        // 0    didn't handle flag
        // 1    handled flag, please remove
        // 2    handled flag, don't remove (but don't process further
        // 3    move this flag to the end of the list please
        if(behavior->info->flagFunc)
        {
            switch(behavior->info->flagFunc(e, aibase, behavior, curFlag))
            {
                xcase FLAG_PROCESSED:
            REMOVE_AND_CONTINUE;
            xcase FLAG_PROCESSED_DONT_REMOVE:
            i++;
            FLAG_DEBUG_PRINT;
            continue;
            xcase FLAG_MOVE_TO_END:
            eaPush(&behavior->flags, eaRemove(&behavior->flags, i));
            }
        }

        // the behavior apparently didn't care about this flag, so process it normally
        switch(curFlag->info->type)
        {
            xcase AIBF_AGGRO_NON_AGGRO_TARGETS:
                ai->attackTargetsNotAttackedByJustAnyone = curFlag->intarray[0];
            xcase AIBF_ALWAYS_ATTACK:
                ai->alwaysAttack = curFlag->intarray[0];
            xcase AIBF_ALWAYS_GET_HIT:
                e->alwaysGetHit = curFlag->intarray[0];
            xcase AIBF_ALWAYS_RETURN_TO_SPAWN:
                ai->alwaysReturnToSpawn = curFlag->intarray[0];
            xcase AIBF_ANIM_BIT:
                for(j = eaSize(&curFlag->strarray)-1; j >= 0; j--)
                    seqSetState(e->seq->sticky_state, 1, seqGetStateNumberFromName(curFlag->strarray[j]));
            xcase AIBF_ANIM_CLEAR_BIT:
                for(j = eaSize(&curFlag->strarray)-1; j >= 0; j--)
                    seqSetState(e->seq->sticky_state, 0, seqGetStateNumberFromName(curFlag->strarray[j]));
            xcase AIBF_ANIM_LIST:
                alSetAIAnimListOnEntity(e, curFlag->allocstr);
            xcase AIBF_BOOST_SPEED:
                ai->inputs.boostSpeed = curFlag->floatarray[0];
            xcase AIBF_COMBAT_MOD_SHIFT:
                if (e->pchar)
                {
                    e->pchar->iCombatModShiftToSendToClient = e->pchar->iCombatModShift = curFlag->intarray[0];
                    e->pchar->bCombatModShiftedThisFrame = true;
                }
            xcase AIBF_COMBAT_OVERRIDE:
                ai->base->behaviorCombatOverride = curFlag->intarray[0];
            xcase AIBF_CONSIDER_ENEMY_LEVEL:
                ai->considerEnemyLevel = curFlag->intarray[0];
            xcase AIBF_CREATOR:
                {
                    AITeamMemberInfo* member;

                    if (ai->teamMemberInfo.team && curFlag->allocstr)
                    {
                        for(member = ai->teamMemberInfo.team->members.list; member; member = member->next)
                        {
                            if (member->entity)
                            {
                                char *name = EncounterActorNameFromActor(member->entity);
                                if (name && stricmp(curFlag->allocstr, name) == 0)
                                {
                                    e->erCreator = erGetRef(member->entity);
                                    break;
                                }
                            }
                        }
                    }
                }
            xcase AIBF_DO_NOT_AGGRO_CRITTERS:
                ai->doNotAggroCritters = curFlag->intarray[0];
            xcase AIBF_DO_NOT_CHANGE_POWERS:
                ai->doNotChangePowers = curFlag->intarray[0];
            xcase AIBF_DO_NOT_DRAW_AGGRO:
                e->notAttackedByJustAnyone = curFlag->intarray[0];
            xcase AIBF_DO_NOT_DRAW_ON_CLIENT:
                if(e->noDrawOnClient != curFlag->intarray[0])
                {
                    e->noDrawOnClient = curFlag->intarray[0];
                    e->draw_update = 1;
                }
            xcase AIBF_DO_NOT_FACE_TARGET:
                e->do_not_face_target = curFlag->intarray[0];
            xcase AIBF_DO_NOT_GO_TO_SLEEP:
            {
                ai->doNotGoToSleep = curFlag->intarray[0];
                if(ai->doNotGoToSleep)
                    SET_ENTSLEEPING(e) = 0;
            }
            xcase AIBF_DO_NOT_MOVE:
                ai->doNotMove = curFlag->intarray[0];
            xcase AIBF_DO_NOT_RUN:
                ai->inputs.doNotRunIsSet = 1;
                ai->inputs.doNotRun = ai->doNotRun = curFlag->intarray[0];
            xcase AIBF_FEELING_BOTHERED:
                ai->feeling.sets[AI_FEELINGSET_BASE].level[AI_FEELING_BOTHERED] = curFlag->floatarray[0] / 100.0;
            xcase AIBF_FEELING_CONFIDENCE:
                ai->feeling.sets[AI_FEELINGSET_BASE].level[AI_FEELING_CONFIDENCE] = curFlag->floatarray[0] / 100.0;
            xcase AIBF_FEELING_FEAR:
                ai->feeling.sets[AI_FEELINGSET_BASE].level[AI_FEELING_FEAR] = curFlag->floatarray[0] / 100.0;
            xcase AIBF_FEELING_LOYALTY:
                ai->feeling.sets[AI_FEELINGSET_BASE].level[AI_FEELING_LOYALTY] = curFlag->floatarray[0] / 100.0;
            xcase AIBF_FIND_TARGET:
                ai->findTargetInProximity = curFlag->intarray[0];
            xcase AIBF_FLY:
                ai->inputs.flyingIsSet = 1;
                ai->inputs.flying = curFlag->intarray[0];
                aiSetFlying(e, ai, curFlag->intarray[0]);
            xcase AIBF_FOV_DEGREES:
                aiSetFOVAngle(e, ai, RAD(curFlag->intarray[0]) / 2.0);
            xcase AIBF_GRIEF_HP_RATIO:
                ai->griefed.hpRatio = curFlag->floatarray[0];
            xcase AIBF_HAS_PATROL_POINT:
                ai->hasNextPatrolPoint = curFlag->intarray[0];
            xcase AIBF_I_AM_A:
                for(j = eaSize(&curFlag->strarray)-1; j >= 0; j--)
                    eaPushConst(&ai->IAmA, allocAddString(curFlag->strarray[j]));
            xcase AIBF_INVINCIBLE:
                e->invincible = (e->invincible & ~INVINCIBLE_GENERAL) |
                    (curFlag->intarray[0] ? INVINCIBLE_GENERAL : 0);
            xcase AIBF_INVISIBLE:
                if(ENTHIDE(e) != curFlag->intarray[0])
                {
                    SET_ENTHIDE(e) = curFlag->intarray[0];
                    e->draw_update = 1;
                }
            xcase AIBF_I_REACT_TO:
                for(j = eaSize(&curFlag->strarray)-1; j >= 0; j--)
                    eaPushConst(&ai->IReactTo, allocAddString(curFlag->strarray[j]));
            xcase AIBF_JUMPHEIGHT:
                ai->followTarget.jumpHeightOverride = curFlag->floatarray[0];
            xcase AIBF_LIMITED_SPEED:
                ai->inputs.limitedSpeed = curFlag->floatarray[0]/30.0;
            xcase AIBF_NO_PHYSICS:
                e->noPhysics = curFlag->intarray[0];
                e->motion->on_surf = 0;
                e->motion->was_on_surf = 0;
                e->motion->falling = 1;
            xcase AIBF_OVERRIDE_PERCEPTION_RADIUS:
                ai->overridePerceptionRadius = curFlag->floatarray[0];
            xcase AIBF_OVERRIDE_SPAWN_POS:
                ai->pet.useStayPos = 1;
                copyVec3(curFlag->floatarray, ai->pet.stayPos);
            xcase AIBF_OVERRIDE_SPAWN_POS_CUR:
                ai->pet.useStayPos = 1;
                copyVec3(ENTPOS(e), ai->pet.stayPos);
            xcase AIBF_PATH_LIMITED:
                ai->pathLimited = curFlag->allocstr ? 1 : 0;
            xcase AIBF_PROTECT_SPAWN_OUTER_RADIUS:
                ai->protectSpawnOutsideRadius = curFlag->floatarray[0];
            xcase AIBF_PROTECT_SPAWN_RADIUS:
                ai->protectSpawnRadius = curFlag->floatarray[0];
            xcase AIBF_PROTECT_SPAWN_DURATION:
                ai->protectSpawnDuration = curFlag->floatarray[0];
            xcase AIBF_RETURN_TO_SPAWN_DISTANCE:
                ai->returnToSpawnDistance = curFlag->floatarray[0];
            xcase AIBF_SET_ATTACK_TARGET:
                aiSetAttackTargetToAggroPlayer(e, ai, curFlag->intarray[0]);
            xcase AIBF_SPAWN_POS_IS_SELF:
                ai->spawnPosIsSelf = curFlag->intarray[0];
            xcase AIBF_STANDOFF_DISTANCE:
                ai->inputs.standoffDistance = ai->followTarget.standoffDistance = curFlag->floatarray[0];
            xcase AIBF_TARGET_INSTIGATOR:
                {
                    Entity* target = EncounterWhoInstigated(e);
                    if(target && ai->proxEntStatusTable)
                    {
                        AIProxEntStatus* status = aiGetProxEntStatus(ai->proxEntStatusTable, target, 1);

                        status->neverForgetMe = curFlag->intarray[0];
                    }
                }
            xcase AIBF_TEAM:
                entSetOnTeam(e, curFlag->intarray[0]);
            xcase AIBF_GANG:
                entSetOnGang(e, curFlag->intarray[0]);
            xcase AIBF_TIMER:
                behavior->timerTime = curFlag->floatarray[0];
            xcase AIBF_UNTARGETABLE:
                e->untargetable = (e->untargetable & ~UNTARGETABLE_GENERAL) |
                    (curFlag->intarray[0] ? UNTARGETABLE_GENERAL : 0);
            xcase AIBF_UNSELECTABLE:
                setEntNoSelectable(e, curFlag->intarray[0]);
            xcase AIBF_USE_ENTERABLE:
                ai->useEnterable = curFlag->intarray[0];
            xcase AIBF_WALK_TO_SPAWN_RADIUS:
                ai->walkToSpawnRadius = curFlag->floatarray[0];
            xcase AIBF_DO_ATTACK_AI:
                ai->doAttackAI = curFlag->intarray[0];
            xcase AIBF_DO_NOT_USE_ANY_POWERS:
                ai->doNotUsePowers = curFlag->intarray[0];
            xcase AIBF_IGNORE_COMBAT_MODS:
                if(e->pchar && e->pchar->bIgnoreCombatMods != curFlag->intarray[0] )
                    e->pchar->bIgnoreCombatMods = curFlag->intarray[0];
            xcase AIBF_IGNORE_COMBAT_MOD_SHIFT:
                if(e->pchar)
                    e->pchar->ignoreCombatModLevelShift = curFlag->intarray[0];
            xcase AIBF_CON_YELLOW:
                e->shouldIConYellow = curFlag->intarray[0];
            xcase AIBF_REWARD_SCALE_OVERRIDE:
                e->fRewardScaleOverride = CLAMP(curFlag->floatarray[0], -1, 1);
            xcase AIBF_ALWAYS_HIT:
                e->alwaysHit = curFlag->intarray[0] ? 1 : 0;
            xcase AIBF_LEASHED_TO_SPAWN:
                {
                    ai->leashToSpawnDistance = curFlag->floatarray[0];
                    ai->vulnerableWhileLeashing = false;
                }
            xcase AIBF_VULNERABLE_LEASHED_TO_SPAWN:
                {
                    ai->leashToSpawnDistance = curFlag->floatarray[0];
                    ai->vulnerableWhileLeashing = true;
                }
            xcase AIBF_DISABLE_AUTO_POWERS:
                {
                    if (e->pchar)
                    {
                        if (curFlag->intarray[0])
                        {
                            character_RevokeAutoPowers(e->pchar, 0);
                        }
                        else
                        {
                            int j;
                            character_RestoreAutoPowers(e);
                            character_ActivateAllAutoPowers(e->pchar);
                            for (j = 0; j < e->petList_size; ++j)
                            {
                                Entity *pet = erGetEnt(e->petList[j]);
                                if (pet)
                                {
                                    character_RestoreAutoPowers(pet);
                                    character_ActivateAllAutoPowers(pet->pchar);
                                }
                            }
                        }
                    }
                }
            xcase AIBF_PASS_TO_PETS:
                {
                    int j;
                    //    pass the original string to pets
                    //    minus the pass to pets
                    for (j = 0; j < e->petList_size; ++j)
                    {
                        Entity *pet = erGetEnt(e->petList[j]);
                        if (pet)
                        {
                            AIVarsBase *petBase = ENTAI(pet)->base;
                            if (petBase)
                            {
                                aiBehaviorAddString(pet, petBase, curFlag->allocstr);
                            }
                        }
                    }
                }
            xcase AIBF_RENAME:
                if(e->name && curFlag->allocstr && curFlag->allocstr[0])
                {    strcpy(e->name, localizedPrintf(e,curFlag->allocstr));
                    if(strcmp(e->name, curFlag->allocstr)==0)
                        strcpy(e->name,saUtilScriptLocalize(curFlag->allocstr));
                    e->name_update = 1;
                }
            xcase AIBF_EXIT_STANCE:
                if(e->pchar)
                {
                    character_EnterStance(e->pchar, NULL, true);
                    ai->power.lastStanceID = 0;
                }
            xcase AIBF_SET_NEW_SPAWN_POS:
                GetPointFromLocation(curFlag->allocstr, ENTAI(e)->spawn.pos);
        }

        FLAG_DEBUG_PRINT;
        i++;
    }
}
#undef REMOVE_AND_CONTINUE
#undef FLAG_DEBUG_PRINT

const char* aiBehaviorOverrideStringFromEnum(int override)
{
    return StaticDefineIntRevLookup(ParseCombatOverride, override);
}

// Coh Callbacks
void aiBehaviorCohRegisterBehaviors(void)
{
    int i;

    for(i = sizeof(BehaviorTable)/sizeof(AIBehaviorInfo)-1; i >= 0; i--)
    {
        aiBehaviorTableAddBehavior(&BehaviorTable[i]);
    }
}

void aiBehaviorCohRegisterFlags(void)
{
    int i;

    for(i = sizeof(BehaviorFlagTable)/sizeof(AIBehaviorFlagInfo)-1; i >= 0; i--)
    {
        aiBehaviorTableAddFlag(&BehaviorFlagTable[i]);
    }
}

int aiBehaviorCohSpecialFlagCallback(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior, AIBehaviorFlag* flag, TokenizerFunctionCall* param, char** estr)
{    
    int type = flag->info->type;
    F32 ftemp, ftemp2;
    static AIBehaviorFlagInfo* posInfo = NULL;
    static int initted = 0;

    if(!initted)
    {
        posInfo = aiBehaviorFlagInfoFromString("Pos");
    }

    switch(type)
    {
        // Behavior input flags
        xcase AIBF_END_CONDITION:
            WRONG_PARAMETERS_NODESTROY(3, "(supposed to be <category> <comparator> <number>)");
            {
                AIBCondition* cond = aiBehaviorParseCondition(param);
                if(cond)
                    eaPush(&behavior->endConditions, cond);
            }
            return true;
        xcase AIBF_SCRIPT_LOCATION:
            if(strncmp(param->function, "ScriptLocation", strlen(param->function))==0) // this is how we get the location if we described it explictly
            {
                WRONG_PARAMETERS_NODESTROY(1, "(supposed to be ScriptLocation(LOCATION))")
                if(!GetPointFromLocation(param->params[0]->function, flag->floatarray)) 
                {
                    Errorf("Wrong number of parameters for position flag, supposed to be a script position marker");
                    return false;
                }
            }
            else // this is how we get the string if it was picked up by aiBehaviorFlagProcessString
            {
                if(!GetPointFromLocation(param->function, flag->floatarray)) 
                {
                    Errorf("Wrong number of parameters for position flag, supposed to be a script position marker");
                    return false;
                }
            }
            flag->info = posInfo;
            estrConcatf(estr, "(^4%0.1f^0, ^4%0.1f^0, ^4%0.1f^0)", flag->floatarray[0], flag->floatarray[1], flag->floatarray[2]);
            return true;
        xcase AIBF_VARIATION:
            WRONG_PARAMETERS_NODESTROY(1, "(supposed to be 1 float)");
            ftemp = rule30Float();
            ftemp *= atof(param->params[0]->function);
            ftemp2 = rule30Float() * PI / 2;
            flag->floatarray[0] = cos(ftemp2) * ftemp;
            flag->floatarray[2] = sin(ftemp2) * ftemp;
            estrConcatf(estr, "(^4%0.1f^0, ^4%0.1f^0)", flag->floatarray[0], flag->floatarray[2]);
            return true;
    }

    return false;
}

char* aiBehaviorCohDefaultBehaviorCallback(Entity* e, AIVarsBase* aibase)
{
    if(ENTTYPE(e) == ENTTYPE_CRITTER)
        return "Combat";
    else
        return "PriorityList";
}

void aiBehaviorCohStringPreprocessCallback(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, TokenizerFunctionCall* string,
                                           char** resultStr, int* addSelfAsString, int* continueProcessing)
{
    char* replaceStr = NULL;
    int isPL = 0;

    if(!stricmp(string->function, "PL_Patrol"))
        replaceStr = "OldPatrol";
    else if(!stricmp(string->function, "PL_Patrol_Group"))
        replaceStr = "OldPatrolGroup";
    else if(!stricmp(string->function, "PL_CallForHelp"))
        replaceStr = "OldCallForHelp";
    else if(!stricmp(string->function, "PL_TargetInstigator"))
        replaceStr = "FeelingBothered(100), OldTargetInstigator";
    else if(!stricmp(string->function, "PL_Invisible"))
        replaceStr = "DoNothing(Invisible)";
    else if(!stricmp(string->function, "PL_None"))
        replaceStr = "DoNothing";
    else if(!stricmp(string->function, "PL_Float"))
        replaceStr = "DoNothing(AnimList(Floating))";
    else if(!stricmp(string->function, "PL_ThankHero"))
        replaceStr = "ThankHero,DisappearInStandardDoor";

    if(replaceStr)
    {
        AIBParsedString* replaceParsed = aiBehaviorParseString(replaceStr);
        aiBehaviorProcessString(e, aibase, behaviors, replaceStr, replaceParsed, false);
        *continueProcessing = 0;
        return;
    }

    // PriorityList is a valid behavior if you're an NPC and don't pass any 
    if(!stricmp(string->function, "PriorityList") && eaSize(&string->params))
    {
        *resultStr = "Alias";
        *addSelfAsString = false;
    }
    else if(!strnicmp(string->function, "PL_", 3))
    {
        *resultStr = string->function + 3;
        *addSelfAsString = true;
    }
    else
    {
        *resultStr = string->function;
        *addSelfAsString = true;
    }

    *continueProcessing = 1;
}

static const char* aiBehaviorCohBlameFileCallback(Entity* e)
{
    const SpawnDef* spawndef = EncounterGetMyResponsibleSpawnDef(e);

    if(spawndef)
        return spawndef->fileName;
    else
        return NULL;
}

int registerAnimListHelper(StashElement element)
{
    AnimBitList* list = (AnimBitList*)stashElementGetPointer(element);
    aiBehaviorTableAddCustom(list, AIB_ANIMLIST, list->listName);
    return 1;
}

void aiBehaviorCohRegisterAnimLists(void)
{
    alForEachAnimList(registerAnimListHelper);
}

void aiBehaviorCohProcessAnimList(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBTableEntry* entry,
        const char* origStr, const char* processedStr, TokenizerFunctionCall* param)
{
    AIBehavior* newbehavior = aiBehaviorPush(e, aibase, behaviors, aiBehaviorInfoFromString("DoNothing"));
    static char* flagString = NULL;
    if(flagString && flagString[0])
        devassertmsg(0, "Flagstring is not empty, is this getting called recursively?");
    estrPrintf(&flagString, "AnimList(%s)", processedStr);
    aiBehaviorAddFlag(e, aibase, 0, flagString);
    estrClear(&flagString);
    aiBehaviorParseFlags(e, aibase, behaviors, newbehavior, param->params);
    newbehavior->originalString = allocAddString(origStr);
}

static void aiBehaviorCohPreResetCleanup(Entity* e, AIVarsBase* aibase)
{
    seqClearState(e->seq->sticky_state);
    alSetAIAnimListOnEntity(e, NULL);
    aiDiscardFollowTarget(e, "Switching behaviors", false);
    clearNavSearch(ENTAI(e)->navSearch);
}

void aiBehaviorCohReinit(Entity* e, AIVarsBase* aibase)
{
    aiBehaviorCohPreResetCleanup(e, aibase);
    aiReInitCoh(e);
}

bool aiBehaviorCohOverrideCombat(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior, int override)
{
    const char* str = NULL;
    bool enterCombat = false;
    Entity* owner = erGetEnt(e->erOwner);
    AIVars* ai = ENTAI(e);

    if (override == AIB_COMBAT_OVERRIDE_AGGRESSIVE)  
    {
        // low priority --> fight any available targets
        if(   ai->attackTarget.entity  
            || ABS_TIME_SINCE(ai->time.wasAttacked) < SEC_TO_ABS(10))
        {
            if(ai->attackTarget.entity)
                str = allocAddString("Has Target");
            else
                str = allocAddString("Was recently attacked");

            AI_LOG( AI_LOG_BEHAVIOR, (e,"Aggressive Combat Override on ^9%s^0 (%s)\n",
                behavior->info->name, str) ); 
            enterCombat = true;
        }
    }
    else if (override == AIB_COMBAT_OVERRIDE_COLLECTIVE)
    {
        if(ABS_TIME_SINCE(ai->time.wasAttacked) < SEC_TO_ABS(10) ||
            (owner && ABS_TIME_SINCE(ENTAI(owner)->time.wasAttacked) < SEC_TO_ABS(10)))
        {
            str = allocAddString("Was recently attacked");
            AI_LOG( AI_LOG_BEHAVIOR, (e,"Combat Override on ^9%s^0 (%s)\n",
                behavior->info->name, str) ); 
            enterCombat = true;
        }
        else
        {
            EncounterGroup *group = e->encounterInfo ? e->encounterInfo->parentGroup : 0;

            if (group && group->active)
            {
                int entCount = eaSize(&group->active->ents);
                int entIndex;
                for (entIndex = 0; entIndex < entCount; entIndex++)
                {
                    AIVars *partnerAI = ENTAI(group->active->ents[entIndex]);
                    Entity *partnerOwner = erGetEnt(group->active->ents[entIndex]->erOwner);

                    if(ABS_TIME_SINCE(partnerAI->time.wasAttacked) < SEC_TO_ABS(10) ||
                        (partnerOwner && ABS_TIME_SINCE(ENTAI(partnerOwner)->time.wasAttacked) < SEC_TO_ABS(10)))
                    {
                        str = allocAddString("Collective was recently attacked");
                        AI_LOG( AI_LOG_BEHAVIOR, (e,"Combat Override on ^9%s^0 (%s)\n",
                            behavior->info->name, str) ); 
                        enterCombat = true;
                        break;
                    }
                }
            }
        }
    }
    else if (override == AIB_COMBAT_OVERRIDE_DEFENSIVE)
    {
        if(ABS_TIME_SINCE(ai->time.wasAttacked) < SEC_TO_ABS(10) ||
            (owner && ABS_TIME_SINCE(ENTAI(owner)->time.wasAttacked) < SEC_TO_ABS(10)))
        {
            str = allocAddString("Was recently attacked");
            AI_LOG( AI_LOG_BEHAVIOR, (e,"Combat Override on ^9%s^0 (%s)\n",
                behavior->info->name, str) ); 
            enterCombat = true;
        }
    }

    if(enterCombat)
    {
        aiBehaviorAddString(e, aibase, "Combat(OverriddenCombat)");
    }

    return enterCombat;
}

typedef enum AIBCondCategory{
    AIB_COND_CAT_HEALTH_PERCENT,
    AIB_COND_CAT_ATTACKER_COUNT,
    AIB_COND_CAT_HAS_ATTACK_TARGET,
    AIB_COND_CAT_TIME_SINCE_ATTACKED,
    AIB_COND_CAT_TIME_SINCE_I_ATTACKED,
    AIB_COND_CAT_NEAREST_ENEMY_DISTANCE,
    AIB_COND_CAT_NEAREST_FRIENDLY_DISTANCE,
}AIBCondCategory;

int aiBehaviorCohConditionValue(Entity* e, AIVarsBase* aibase, int condtype)
{
    AIVars* ai = ENTAI(e);

    switch(condtype)
    {
        xcase AIB_COND_CAT_HEALTH_PERCENT:
            return e->pchar->attrCur.fHitPoints * 100 / e->pchar->attrMax.fHitPoints;
        xcase AIB_COND_CAT_ATTACKER_COUNT:
            return ai->attackerList.count;
        xcase AIB_COND_CAT_HAS_ATTACK_TARGET:
            return !!ai->attackTarget.entity;
        xcase AIB_COND_CAT_TIME_SINCE_ATTACKED:
            {
                if (ai->time.wasAttacked == 0)
                {
                    return 99999;
                }
                else
                    return (int) ABS_TO_SEC(ABS_TIME_SINCE(ai->time.wasAttacked));

            }
        xcase AIB_COND_CAT_TIME_SINCE_I_ATTACKED:
            {
                if (ai->time.didAttack == 0)
                {
                    if (ai->attackTarget.time.set == 0)
                        return 99999;
                    else
                        return (int) ABS_TO_SEC(ABS_TIME_SINCE(ai->attackTarget.time.set));
                }
                else
                    return (int) ABS_TO_SEC(ABS_TIME_SINCE(ai->time.didAttack));

            }
        xcase AIB_COND_CAT_NEAREST_ENEMY_DISTANCE:
            {
                int minDistance = 99999;
                AIProxEntStatus *itr = NULL;
                for(itr = ai->proxEntStatusTable->statusHead;
                    itr;
                    itr = itr->tableList.next)
                {
                    Entity *target = itr->entity;
                    AIVars *aiTarget = ENTAI(target);
                    if (!ai->teamMemberInfo.team
                        ||
                        ai->teamMemberInfo.team != aiTarget->teamMemberInfo.team)
                    {
                        float distFromMe = distance3(ENTPOS(e), ENTPOS(target));
                        if (distFromMe < minDistance)
                        {
                            minDistance = distFromMe;
                        }
                    }
                }
                return minDistance;
            }
        xcase AIB_COND_CAT_NEAREST_FRIENDLY_DISTANCE:
            {
                int minDistance = 99999;
                AIProxEntStatus *itr = NULL;
                for(itr = ai->proxEntStatusTable->statusHead;
                    itr;
                    itr = itr->tableList.next)
                {
                    Entity *target = itr->entity;
                    AIVars *aiTarget = ENTAI(target);
                    if (!ai->teamMemberInfo.team
                        ||
                        ai->teamMemberInfo.team == aiTarget->teamMemberInfo.team)
                    {
                        float distFromMe = distance3(ENTPOS(e), ENTPOS(target));
                        if (distFromMe < minDistance)
                        {
                            minDistance = distFromMe;
                        }
                    }
                }
                return minDistance;
            }
    }

    return -1;
}

int aiBehaviorCohConditionParse(const char* catstring)
{
    if(!stricmp(catstring, "HealthPercent"))
        return AIB_COND_CAT_HEALTH_PERCENT;
    else if(!stricmp(catstring, "NumAttackers"))
        return AIB_COND_CAT_ATTACKER_COUNT;
    else if(!stricmp(catstring, "HasAttackTarget"))
        return AIB_COND_CAT_HAS_ATTACK_TARGET;
    else if(!stricmp(catstring, "TimeSinceAttacked"))
        return AIB_COND_CAT_TIME_SINCE_ATTACKED;
    else if(!stricmp(catstring, "TimeSinceIAttacked"))
        return AIB_COND_CAT_TIME_SINCE_I_ATTACKED;
    else if(!stricmp(catstring, "NearestEnemy"))
        return AIB_COND_CAT_NEAREST_ENEMY_DISTANCE;
    else if(!stricmp(catstring, "NearestFriendly"))
        return AIB_COND_CAT_NEAREST_FRIENDLY_DISTANCE;
    else
        return -1;
}

U32 aiBehaviorCohABS_TIMECallback(void)
{
    return ABS_TIME;
}

void aiBehaviorCohInitSystem()
{
    aiBehaviorSetBehaviorReloadCallback(aiBehaviorCohRegisterBehaviors);
    aiBehaviorSetBehaviorFlagReloadCallback(aiBehaviorCohRegisterFlags);
    aiBehaviorSetBehaviorTableCustomReloadCallback(aiBehaviorCohRegisterAnimLists);
    aiBehaviorRebuildLookupTable();

    aiBehaviorSetBehaviorCustomTableEntryCallback(aiBehaviorCohProcessAnimList);
    aiBehaviorSetStringPreprocessCallback(aiBehaviorCohStringPreprocessCallback);
    aiBehaviorSetBlameFileCallback(aiBehaviorCohBlameFileCallback);
    aiBehaviorSetSpecialFlagProcessingCallback(aiBehaviorCohSpecialFlagCallback);
    aiBehaviorSetFlagProcessCallback(aiBehaviorCohFlagProcess);
    aiBehaviorSetDefaultBehaviorCallback(aiBehaviorCohDefaultBehaviorCallback);
    aiBehaviorSetReinitCallback(aiBehaviorCohReinit);
    aiBehaviorSetCheckCombatOverrideCallback(aiBehaviorCohOverrideCombat);
    aiBehaviorSetConditionValueCallback(aiBehaviorCohConditionValue);
    aiBehaviorSetConditionParseCallback(aiBehaviorCohConditionParse);

    aiBehaviorSetGetABS_TIMECallback(aiBehaviorCohABS_TIMECallback);
}

//////////////////////////////////////////////////////////////////////////
// AlertTeam
//////////////////////////////////////////////////////////////////////////
void aiBFAlertTeamRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIVars* ai = ENTAI(e);

    if(!ai->teamMemberInfo.alerted){
        EncounterAISignal(e, ENCOUNTER_ALERTED);

        ai->teamMemberInfo.alerted = 1;

        AI_LOG(AI_LOG_TEAM, (e, "Alerted Team\n"));
    }
    else
    {
        AI_LOG(AI_LOG_TEAM, (e, "Did not alert team - it was already alerted!\n"));
    }

    behavior->finished = 1;
}

//////////////////////////////////////////////////////////////////////////
// AttackTarget
//////////////////////////////////////////////////////////////////////////
int aiBFAttackTargetFlag(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, AIBehaviorFlag* flag)
{
    AIBDAttackTarget* mydata = (AIBDAttackTarget*) behavior->data;
    if(flag->info->type == AIBF_ENTITY_REF)
    {
        memcpy(&mydata->target, &flag->entref, sizeof(mydata->target));
        return FLAG_PROCESSED;
    }
    else if(flag->info->type == AIBF_PRIORITY)
    {
        mydata->priority = flag->intarray[0];
        return FLAG_PROCESSED;
    }
    return FLAG_NOT_PROCESSED;
}

void aiBFAttackTargetRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDAttackTarget* mydata = behavior->data;
    Entity* mytarget = erGetEnt(mydata->target);
    AIVars* ai = ENTAI(e);

    if(!mytarget || entMode(mytarget, ENT_DEAD))
    {
        if(aibase->behaviorCombatOverride == AIB_COMBAT_OVERRIDE_AGGRESSIVE)
        {
            aiCritterCheckProximity(e, 1);
            if(ai->attackTarget.entity)
                mydata->target = erGetRef(ai->attackTarget.entity);
            else
            {
                behavior->finished = 1;
                return;
            }
        }
        else
        {
            behavior->finished = 1;
            return;
        }
    }

    if(!ai->attackTarget.entity || ai->attackTarget.entity != mytarget)
    {
        if(mydata->priority == LOW_PRIORITY && (ai->attackTarget.entity  
           || (ai->time.wasAttacked && ABS_TIME_SINCE(ai->time.wasAttacked) < SEC_TO_ABS(10))))
        {
            // LOW priority means we're clear to engage with whatever is around us
            // That entity was found in the think phase earlier in aiCritterCheckProximity, we're just letting it through
        }
        else if(mydata->priority == MEDIUM_PRIORITY && (ai->time.wasAttacked && ABS_TIME_SINCE(ai->time.wasAttacked) < SEC_TO_ABS(10)))
        {
            // MED priority means if we're attacked, we can engage
            // That entity was found in the think phase earlier in aiCritterCheckProximity, we're just letting it through
        }
        else
        {
            // If nothing else prevents it, we must attack our given target
            // This stomps over anything which was found in the think phase earlier in aiCritterCheckProximity
            aiSetAttackTarget(e, mytarget, NULL, "BehaviorAttackTarget", 1, 0);
        }
    }

    aiCritterActivityCombat(e, ai);
}

void aiBFAttackTargetStart(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDAttackTarget* mydata = (AIBDAttackTarget*)behavior->data;
    AIVars* ai = ENTAI(e);

    behavior->overrideExempt = 1;
    if (mydata->priority == UNSET_PRIORITY)
        mydata->priority = HIGH_PRIORITY;
}

void aiBFAttackTargetSet(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDAttackTarget* mydata = (AIBDAttackTarget*)behavior->data;
    AIVars* ai = ENTAI(e);
    AIProxEntStatus* status = aiGetProxEntStatus(ai->proxEntStatusTable, erGetEnt(mydata->target), 1);

    aiSetMessageHandler(e, aiCritterMsgHandlerCombat);
    if(status)
    {
        status->primaryTarget = 1;
        status->neverForgetMe = 1;
    }
    ai->alwaysAttack = 1;
    if(mydata->priority == HIGH_PRIORITY)
    {
        ai->doNotChangeAttackTarget = 1;
        ai->findTargetInProximity = 0;
    }
    else
    {
        ai->doNotChangeAttackTarget = 0;
        ai->findTargetInProximity = 1;
    }
}

void aiBFAttackTargetUnset(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDAttackTarget* mydata = (AIBDAttackTarget*)behavior->data;
    AIVars* ai = ENTAI(e);
    AIProxEntStatus* status = aiGetProxEntStatus(ai->proxEntStatusTable, erGetEnt(mydata->target), 1);

    if(status)
    {
        status->primaryTarget = 0;
        status->neverForgetMe = 0;
    }
    ai->doNotChangeAttackTarget = 0;
    ai->alwaysAttack = 0;
    ai->findTargetInProximity = 1;
    aiDiscardAttackTarget(e, "Behavior AttackTarget Unset.");
    if(!ai->doNotChangePowers){
        aiDiscardCurrentPower(e);
    }
}

//////////////////////////////////////////////////////////////////////////
// AttackVolumes
//////////////////////////////////////////////////////////////////////////
void aiBFAttackVolumesDebugStr(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, char** estr, char* indent)
{
    // Rebuilds the string that this behavior is derived from
    AIBDAttackVolumes* mydata = (AIBDAttackVolumes*)behavior->data;
    int i;

    estrConcatf(estr, "AttackVolumes(float(%f,0,0),string(", mydata->fSlowScale);
    for (i=0;i<eaSize(&mydata->targetVolumes);i++)
    {
        estrConcatf(estr, "%s;", mydata->targetVolumes[i]->name);
    }
    if (i==0) estrConcatChar(estr, ';');
    estrConcatStaticCharArray(estr, "))");
}

int aiBFAttackVolumesFlag(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, AIBehaviorFlag* flag)
{
    AIBDAttackVolumes* mydata = (AIBDAttackVolumes*) behavior->data;
    if(flag->info->type == AIBF_STRINGPARAM)
    {
        char* fronttok = flag->strptr;
        while (*fronttok)
        {
            int lentok;
            char* volumename;
            char* endtok = strchr(fronttok,';');
            bool malformed = false;
            AIBDAttackableVolume *v;
            EntityRef* perMe;

            lentok = (endtok) ? (int)(endtok - fronttok) : (int)strlen(fronttok);

            volumename = malloc(lentok+1);
            strncpy(volumename,fronttok,lentok);
            volumename[lentok] = '\0';

            // Convert text to Vec3 and save to earray (malformed are ignored)
            while (1) {
                char *front = volumename;
                char *delim = strchr(front,'_');
                v = malloc(sizeof(AIBDAttackableVolume));
                if (!delim) { malformed = true; break; }
                *delim = 0;
                v->pos[0] = (F32)atof(front);
                *delim = '_';
                front = ++delim;
                delim = strchr(front,'_');
                if (!delim) { malformed = true; break; }
                *delim = 0;
                v->pos[1] = (F32)atof(front);
                *delim = '_';
                front = ++delim;
                v->pos[2] = (F32)atof(front);
                v->name = volumename;
                break;
            }

            if (malformed)
            {
                free(v);
                free(volumename);
            }
            else
            {
                eaPush(&mydata->targetVolumes,v);

                // Add volumename-&entref(e) to the AttackVolumes hashtable
                if (!g_htAttackVolumes)
                {
                    g_htAttackVolumes = stashTableCreateWithStringKeys(30, StashDeepCopyKeys | StashCaseSensitive );
                }
                perMe = malloc(sizeof(EntityRef));
                *perMe = erGetRef(e);
                stashAddPointer(g_htAttackVolumes,volumename,perMe, false);
            }

            fronttok = (endtok) ? endtok+1 : fronttok+lentok;
        }
        return FLAG_PROCESSED;
    }
    else if(flag->info->type == AIBF_FLOATPARAM)
    {
        mydata->fSlowScale = flag->floatarray[0];
        return FLAG_PROCESSED;
    }

    return FLAG_NOT_PROCESSED;
}

void aiBFAttackVolumesRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDAttackVolumes* mydata = behavior->data;
    int n;
    Entity* entArray[MAX_ENTITIES_PRIVATE];
    Entity* mytarget = NULL;
    Vec3 myPos;
    AIVars* ai = ENTAI(e);

    if(mydata->targetVolumes)
    {
        copyVec3(ENTPOS(e), myPos);

        // Get the list of nearby entities.
        n = mgGetNodesInRange(0, myPos, (void **)entArray, 0);

        {
            F32 minDist = FLT_MAX;
            int i;

            // Look through all nearby entities
            for (i = 0; i < n; i++)
            {
                int proxID = (int)(intptr_t)entArray[i];
                Entity* proxEnt = entities[proxID];

                F32 entDist = distance3Squared(myPos, ENTPOS_BY_ID(proxID));

                // If this one is closer than the best we've found so far and it's a legal target
                if (entDist<minDist && aiIsLegalAttackTarget(e,proxEnt))
                {
                    VolumeList *volumeList = &proxEnt->volumeList;
                    int entv;

                    if (!volumeList || volumeList->groupDefVersion != server_state.groupDefVersion)
                        continue;

                    // Look through its volume list and see if it's in a target volume
                    for (entv = 0; entv < volumeList->volumeCount; entv++)
                    {
                        Volume *entVolume = &volumeList->volumes[entv];
                        if (entVolume && entVolume->volumePropertiesTracker)
                        {
                            int v;
                            char *entVolumeName = groupDefFindPropertyValue(entVolume->volumePropertiesTracker->def,"NamedVolume");
                            if (!entVolumeName) // For now just ignore any volume without this property
                                continue;
                            for (v = eaSize(&mydata->targetVolumes)-1; v >= 0; v--)
                            {
                                //                            if (0 == distance3Squared(entVolume->volumePropertiesTracker->mid,mydata->targetVolumes[v]->pos)) // && !strcmp(entVolumeName,targetVolumeName))
                                if (0 == strcmp(detailAttackVolumesVecName(entVolume->volumePropertiesTracker->mid),
                                    mydata->targetVolumes[v]->name))
                                {
                                    minDist = entDist;
                                    mytarget = proxEnt;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Increase the timer, which attempts to track seconds.  We generally only get in here every half-second.
    mydata->fSecondaryTimer += (e->timestep * e->pchar->attrCur.fRechargeTime)/2.0;

    if(!ai->attackTarget.entity || ai->attackTarget.entity != mytarget)
        aiSetAttackTarget(e, mytarget, NULL, "BehaviorAttackVolumes", 1, 0);

    if(!mytarget 
        && mydata->fSlowScale >= 0.0 
        && mydata->fSecondaryTimer >= (mydata->fSlowScale * mydata->fAttackRecharge))
    {
        // We've waited long enough without a volume target.  Find a regular target and fire away
        aiCritterCheckProximity(e,1);
    }

    // Kill skuls...
    aiCritterActivityCombat(e, ai);

    if(e->pchar->ppowCurrent)
    {
        mydata->fAttackRecharge = e->pchar->ppowCurrent->ppowBase->fRechargeTime;
        mydata->fSecondaryTimer = 0.0;
    }
}

void aiBFAttackVolumesFinish(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDAttackVolumes* mydata = behavior->data;
    if (mydata->targetVolumes) {
        // Clear me out of the AttackVolumes hashtable
        if (g_htAttackVolumes)
        {
            EntityRef erMe = erGetRef(e);
            int i = 0;
            for (i = eaSize(&mydata->targetVolumes)-1; i >= 0; i--)
            {
                EntityRef* perMe;
                StashElement element;
                stashFindElement(g_htAttackVolumes,mydata->targetVolumes[i]->name, &element);

                verify(element && "Terminated AttackVolumes behavior was tracking a volume not in the hash table");
                perMe = stashElementGetPointer(element);
                if (*perMe == erMe)
                {
                    free(perMe);
                    stashRemovePointer(g_htAttackVolumes,mydata->targetVolumes[i]->name, NULL);
                }

                free(mydata->targetVolumes[i]->name);
            }
        }

        // Clean up my internal list
        eaDestroyEx(&mydata->targetVolumes,NULL);
    }
}

// Basic init stuff that should only need to be done once
//  Ends up getting called both at start and resume
void aiBFAttackVolumesSet(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDAttackVolumes* mydata = behavior->data;
    AIVars* ai = ENTAI(e);
    if (mydata->fAttackRecharge == 0.0)
        mydata->fAttackRecharge = 1.0;
    mydata->fSecondaryTimer = 0.0;
    aiSetMessageHandler(e, aiCritterMsgHandlerCombat);
    ai->alwaysAttack = 1;
}

//////////////////////////////////////////////////////////////////////////
// BallTurretBaseCheckDeath
//////////////////////////////////////////////////////////////////////////
void aiBFBallTurretBaseCheckDeathRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIVars* ai = ENTAI(e);
    if(ai->teamMemberInfo.team){
        F32 maxDistSQR = SQR(3);
        AITeamMemberInfo* member;
        int found = 0;

        for(member = ai->teamMemberInfo.team->members.list; member; member = member->next)
        {
            Entity* entMember = member->entity;
            if(entMember != e)
            {
                F32 distSQR = distance3Squared(ENTPOS(e), ENTPOS(entMember));

                if(distSQR <= maxDistSQR)
                {
                    if(entMember->villainDef &&
                        strstriConst(entMember->villainDef->name, "Turret")){
                            // Found one!

                            found = 1;
                    }
                }
            }
        }

        if(!found)
            behavior->finished = 1;
    }
}

//////////////////////////////////////////////////////////////////////////
// BallTurretSlideOut
//////////////////////////////////////////////////////////////////////////
void aiBFBallTurretSlideOutRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AITeamMemberInfo* member;
    F32 distSQR = SQR(3);
    AIVars* ai = ENTAI(e);

    for(member = ai->teamMemberInfo.team->members.list; member; member = member->next){
        if(member->entity->villainDef &&
            !stricmp(member->entity->villainDef->name, "Turrets_PopUp_Turret_Base")){
                if(distance3Squared(ENTPOS(e), ENTPOS(member->entity)) <= distSQR){

                    SETB(member->entity->seq->state, seqGetStateNumberFromName("TURRETBASERISE"));
//                    SETB(member->entity->seq->state, seqGetStateNumberFromName("COMBAT"));
//                    SETB(member->entity->seq->state, seqGetStateNumberFromName("WEAPON"));
//                    SETB(member->entity->seq->state, seqGetStateNumberFromName("GUN"));
//                    SETB(member->entity->seq->state, seqGetStateNumberFromName("MACHINEGUN"));

                    break;
                }
        }
    }

    if(behavior->timeRun > SEC_TO_ABS(1))
        behavior->finished = 1;
}

//////////////////////////////////////////////////////////////////////////
// CallForHelp
//////////////////////////////////////////////////////////////////////////
int aiBFCallForHelpString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDCallForHelp* mydata = (AIBDCallForHelp*) behavior->data;

    if(!eaSize(&param->params))
        return 0;

    if(!stricmp(param->function, "Radius"))
    {
        mydata->radius = aiBehaviorParseFloat(param->params[0]);
        return 1;
    }
    if(!stricmp(param->function, "TargetName"))
    {
        mydata->target_name = allocAddString(param->params[0]->function);
        return 1;
    }

    return 0;
}

void aiBFCallForHelpRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDCallForHelp* mydata = (AIBDCallForHelp*)behavior->data;
    AIVars* ai = ENTAI(e);
    aiCallForHelp(e, ai, mydata->radius, mydata->target_name);
    behavior->finished = 1;
}

//////////////////////////////////////////////////////////////////////////
// CameraScan
//////////////////////////////////////////////////////////////////////////
int aiBFCameraScanFlag(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, AIBehaviorFlag* flag)
{
    AIBDCameraScan* mydata = (AIBDCameraScan*)behavior->data;

    //if(flag->type == AIBF_FLOATPARAM || flag->type == AIBF_POS)
    //{
    //    copyVec3( flag->floatarray, mydata->pos );
    //    return FLAG_PROCESSED;
    //}
    //if( flag->type == AIBF_STRINGPARAM )
    //{
    //if( curFlag->param->function)
    //mydata->flags = curFlag->estr
    //    return FLAG_PROCESSED;
    //}
    return FLAG_NOT_PROCESSED;
}

void aiBFCameraScanRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    Vec3 pyr; 
    F32 delta;
    F32 move;
    AIVars* ai = ENTAI(e);

    AIBDCameraScan * mydata = behavior->data;      

    // TO DO move out
    if( !mydata->initted ) //TO DO move to passed in?
    {
        int ePos = EncounterPosFromActor(e); 

        //Put in some default values in the unlikely event that there is no encounter position
        mydata->flags = STEALTH_CAMERA_ROTATE; 
        mydata->speed = 30.0; 
        mydata->sweepDir = 1;

        //Then look up the real values from the encounterPosition properties on the map
        if( ePos )
        {
            EncounterGetPositionInfo( e->encounterInfo->parentGroup, ePos, mydata->initialPyr, 0, 0, &mydata->flags, &mydata->arc, &mydata->speed );
        }

        if( mydata->flags == STEALTH_CAMERA_REVERSEARC )
            mydata->sweepDir = 0;

        mydata->initted = 1;
    }

    getMat3YPR( ENTMAT(e), pyr );

    if( mydata->flags == STEALTH_CAMERA_FROZEN || mydata->speed == 0.0 )
    {
        return;
    }
    if( mydata->flags == STEALTH_CAMERA_ROTATE )
    {
        mydata->sweepDir = 1;
    }
    else if( mydata->flags == STEALTH_CAMERA_REVERSEROTATE)
    {
        mydata->sweepDir = 0; //Go counterclockwise
    }
    else if( mydata->flags == STEALTH_CAMERA_ARC || mydata->flags == STEALTH_CAMERA_REVERSEARC )
    {
        //Calculate sweep direction
        delta = subAngle( pyr[1], mydata->initialPyr[1] );  
        if( delta < -1 * RAD(mydata->arc/2.0) )
            mydata->sweepDir = 1;  
        if( delta > RAD(mydata->arc/2.0) )
            mydata->sweepDir = 0;  
    }

    move = RAD(mydata->speed/30.0f);
    move *= (mydata->sweepDir ? 1 : -1); 
    pyr[1] = addAngle( pyr[1], move );
    aiTurnBodyByPYR( e, ai, pyr);
}

//////////////////////////////////////////////////////////////////////////
// Chat
//////////////////////////////////////////////////////////////////////////
void aiBFChatRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIVars* ai = ENTAI(e);
    ai->timerTryingToSleep = 0;
    SET_ENTSLEEPING(e) = 0;

    if(!e->IAmInEmoteChatMode)
        behavior->finished = 1;
}

//////////////////////////////////////////////////////////////////////////
// Combat
//////////////////////////////////////////////////////////////////////////
void aiBFCombatDebugStr(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, char** estr, char* indent)
{
    AIVars* ai = ENTAI(e);
    if(ai->attackTarget.entity)
        estrConcatf(estr, "Target: %s %s^0- Distance ^4%1.2f "
        "^0LOS: %s ^0- ^4%d^0s (^4%1.1f^0s)",
        AI_LOG_ENT_NAME(ai->attackTarget.entity),
        ai->brain.type!=AI_BRAIN_COT_BEHEMOTH?"":(ai->brain.cotbehemoth.statusPrimary&&ai->brain.cotbehemoth.statusPrimary==ai->attackTarget.status)?"^1(PRIMARY) ":"^2(non-primary) ",
        distance3(ENTPOS(e), ENTPOS(ai->attackTarget.entity)),
        ai->canSeeTarget ? "^2VISIBLE" : ai->canHitTarget ? "^2HITABLE^0, ^1NOT VISIBLE" : "^1NOT VISIBLE",
        ABS_TO_SEC(ABS_TIME_SINCE(ai->time.beginCanSeeState)),
        ABS_TO_SEC((F32)ABS_TIME_SINCE(ai->time.checkedLOS)));
    else
        estrConcatStaticCharArray(estr, "No Target");
}

void aiBFCombatRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDCombat* mydata = (AIBDCombat*) behavior->data;
    AIVars* ai = ENTAI(e);

    if(mydata->fromOverride)
        aibase->overriddenCombat = 1;

    if(mydata->neverForgetTargets && ai->attackTarget.status)
        ai->attackTarget.status->neverForgetMe = 1;

    aiCritterActivityCombat(e, ai);
}

void aiBFCombatSet(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    aiSetMessageHandler(e, aiCritterMsgHandlerCombat);
}

void aiBFCombatStart(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    behavior->overrideExempt = 1;    // combat can never be overridden
}

int aiBFCombatString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDCombat* mydata = (AIBDCombat*) behavior->data;

    if(!stricmp(param->function, "OverriddenCombat"))
    {
        mydata->fromOverride = 1;
        return 1;
    }
    else if(!stricmp(param->function, "NeverForgetTargets"))
    {
        mydata->neverForgetTargets = 1;
        return 1;
    }
    else if(!stricmp(param->function, "EndOverriddenCombat") && mydata->fromOverride)
    {
        behavior->finished = 1;
        return 1;
    }

    return 0;
}

void aiBFCombatUnset(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIVars* ai = ENTAI(e);

    aibase->overriddenCombat = 0;
    if (!ai->doNotChangePowers && e->pchar)
    {
        aiDiscardCurrentPower(e);
    }
}

//////////////////////////////////////////////////////////////////////////
// CutScene
//////////////////////////////////////////////////////////////////////////
int aiBFCutSceneString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDAlias* mydata = (AIBDAlias*)behavior->data;
    AIBParsedString* parsed;

    parsed = aiBehaviorParseString( param->function );
    //aiBehaviorDebugVerifyParse(param->function, parsed);
    AI_LOG(AI_LOG_BEHAVIOR, (e, "ProcessString: Processing ^9%s^0\n", param->function));
    aiBehaviorProcessString(e, aibase, &mydata->behaviors, param->function, parsed, false);

    return 1;
}

//////////////////////////////////////////////////////////////////////////
// DestroyMe
//        Note: Not safe to use on mastermind pets
//////////////////////////////////////////////////////////////////////////
void aiBFDestroyMeRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    // kill entity without giving credit for kill
    SET_ENTINFO(e).kill_me_please = 1;
}

//////////////////////////////////////////////////////////////////////////
// DisappearInStandardDoor
//////////////////////////////////////////////////////////////////////////
void aiBFDisappearInStandardDoorRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDDisappearInStandardDoor* mydata = (AIBDDisappearInStandardDoor*) behavior->data;
    AIVars* ai = ENTAI(e);

    Entity* mydoor = erGetEnt(mydata->door);
    
    // fix for entities unable to find their way to a door but not giving up
    if(behavior->timeRun > SEC_TO_ABS(MAX_RUN_INTO_DOOR_TIME))
    {
        behavior->finished = 1;
        e->fledMap = 1; //Nobody killed me.  I just ran off
        SET_ENTINFO(e).kill_me_please = 1;
        return;
    }

    // I'm running to a door.

    if(!mydata->waitForDeath)
    {
        if(!mydoor){
            Entity* closest = NULL;
            F32    minDist = FLT_MAX;
            int doorCount = 0;
            Entity* door;
            Beacon* startBeacon;

            beaconSetPathFindEntity(e, 0);

            startBeacon = beaconGetClosestCombatBeacon(ENTPOS(e), 1, NULL, GCCB_PREFER_LOS, NULL);

            //gridFindObjectsInSphere(    &ent_grid,
            //                            (void **)entArray,
            //                            ARRAY_SIZE(entArray),
            //                            posPoint(e),
            //                            1000);

            if(!startBeacon){
                SET_ENTINFO(e).kill_me_please = 1;
            }else{
                BeaconBlock* cluster = startBeacon->block->galaxy->cluster;
                BeaconBlock* galaxy;
                Beacon* doorBeacon;
                Entity* entArray[MAX_ENTITIES_PRIVATE];
                int    entCount;
                int i;

                beacon_state.galaxySearchInstance++;

                entCount = mgGetNodesInRange(0, ENTPOS(e), entArray, 0);

                for(i = 0; i < entCount; i++){
                    int idx = (int)(intptr_t)entArray[i];

                    if(ENTTYPE_BY_ID(idx) == ENTTYPE_DOOR){
                        door = validEntFromId(idx);

                        if(!door)
                            continue;

                        doorBeacon = entGetNearestBeacon(door);

                        if(!DoorAnimFindPoint(posPoint(doorBeacon), DoorAnimPointExit, MAX_DOORANIM_BEACON_DIST))
                            continue; // this door is probably outside of an invisible wall in an
                        // outdoor mission map

                        if(doorBeacon)
                        {
                            galaxy = doorBeacon->block->galaxy;

                            if(    galaxy->cluster == cluster &&
                                galaxy->searchInstance != beacon_state.galaxySearchInstance &&
                                distance3Squared(ENTPOS(door), posPoint(doorBeacon)) < SQR(20))
                            {
                                F32 distSQR = distance3Squared(ENTPOS(e), ENTPOS(door));

                                if(distSQR <= SQR(200)){
                                    if(beaconGalaxyPathExists(startBeacon, doorBeacon, 0, 0)){
                                        entArray[doorCount++] = door;
                                    }else{
                                        entArray[i] = 0;

                                        // Mark the galaxy as bad.

                                        galaxy->searchInstance = beacon_state.galaxySearchInstance;
                                        galaxy->isGoodForSearch = 0;
                                    }
                                }
                            }
                        }
                    }
                }

                if(!doorCount){
                    for(i = 0; i < entCount; i++){
                        int idx = (int)(intptr_t)entArray[i];

                        if(idx && ENTTYPE_BY_ID(idx) == ENTTYPE_DOOR){
                            door = validEntFromId(idx);

                            if(!door)
                                continue;

                            doorBeacon = entGetNearestBeacon(door);

                            if(doorBeacon){
                                galaxy = doorBeacon->block->galaxy;

                                if(    galaxy->cluster == cluster &&
                                    galaxy->searchInstance != beacon_state.galaxySearchInstance &&
                                    distance3Squared(ENTPOS(door), posPoint(doorBeacon)) < SQR(20))
                                {
                                    F32 distSQR = distance3Squared(ENTPOS(e), ENTPOS(door));

                                    if(    distSQR > SQR(200) &&
                                        distSQR <= SQR(1000))
                                    {
                                        if(beaconGalaxyPathExists(startBeacon, doorBeacon, 0, 0)){
                                            entArray[doorCount++] = door;
                                        }else{
                                            // Mark the galaxy as bad.

                                            galaxy->searchInstance = beacon_state.galaxySearchInstance;
                                            galaxy->isGoodForSearch = 0;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if(!doorCount){
                        SET_ENTINFO(e).kill_me_please = 1;
                        return;
                    }
                }

                mydoor = entArray[rand() % doorCount];
                mydata->door = erGetRef(mydoor);
            }
        }

        if(mydoor){
            if(distance3Squared(ENTPOS(e), ENTPOS(mydoor)) < SQR(10)){
                if(!mydata->timeOpenedDoor)
                {
                    aiDoorSetOpen(mydoor, 1);
                    mydata->timeOpenedDoor = ABS_TIME;
                }
                else if(ABS_TIME_SINCE(mydata->timeOpenedDoor) > SEC_TO_ABS(1))
                {
                    NavPathWaypoint* wp;

                    // Create a path through the door.

                    clearNavSearch(ai->navSearch);

                    // Door.

                    wp = createNavPathWaypoint();
                    wp->connectType = NAVPATH_CONNECT_WIRE;
                    copyVec3(ENTPOS(mydoor), wp->pos);
                    navPathAddTail(&ai->navSearch->path, wp);

                    // Behind door.

                    wp = createNavPathWaypoint();
                    wp->connectType = NAVPATH_CONNECT_WIRE;
                    scaleVec3(ENTMAT(mydoor)[2], -5, wp->pos);
                    addVec3(ENTPOS(mydoor), wp->pos, wp->pos);
                    navPathAddTail(&ai->navSearch->path, wp);

                    ai->navSearch->path.curWaypoint = ai->navSearch->path.head;

                    aiSetFollowTargetPosition(e, ai, wp->pos, AI_FOLLOWTARGETREASON_NONE);

                    aiStartPath(e, 1, 0);

                    ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 1;
                    mydata->waitForDeath = 1;
                }
            }
            else if(!ai->motion.type ||
                ai->followTarget.type != AI_FOLLOWTARGET_ENTITY ||
                ai->followTarget.reason != AI_FOLLOWTARGETREASON_NONE)
            {
                aiSetFollowTargetEntity(e, ai, mydoor, AI_FOLLOWTARGETREASON_NONE);

                aiGetProxEntStatus(ai->proxEntStatusTable, mydoor, 1);

                aiConstructPathToTarget(e);

                if(!ai->navSearch->path.head){
                    if(++mydata->failedPathFindCount > 5)
                        SET_ENTINFO(e).kill_me_please = 1;
                    return;
                }else{
                    ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 10;
                    aiStartPath(e, 0, 0);
                }
            }
        }
    }
    else
    {
        if(!ai->navSearch->path.head)
        {
            if(ai->followTarget.followTargetReached)
            {
                AI_LOG(AI_LOG_PATH, (e, "Killing self.\n"));
                SET_ENTINFO(e).kill_me_please = 1;
            }
            else
                mydata->waitForDeath = 0;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// DoNothing
//////////////////////////////////////////////////////////////////////////
void aiBFDoNothingRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDDoNothing* mydata = (AIBDDoNothing*) behavior->data;
    AIVars* ai = ENTAI(e);

    if(mydata->turn && ENTTYPE(e) == ENTTYPE_CRITTER && ai->turnWhileIdle && !e->IAmInEmoteChatMode){
        if(getCappedRandom(1000) < 3  && !e->cutSceneActor){
            Vec3 pyr;
            Mat3 newmat;

            getMat3YPR(ENTMAT(e), pyr);

            pyr[1] = addAngle(pyr[1], RAD(rand() % 360 - 180));

            createMat3YPR(newmat, pyr);
            entSetMat3(e, newmat);
        }
    }

    aiDiscardFollowTarget(e, "Frozen.", false);

    ai->waitForThink = 1;

    aiMotionSwitchToNone(e, ai, "Frozen in place.");
}

int aiBFDoNothingString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDDoNothing* mydata = (AIBDDoNothing*) behavior->data;

    if(!stricmp(param->function, "Turn"))
    {
        mydata->turn = 1;
        return 1;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////
// HideBehindEnt
//////////////////////////////////////////////////////////////////////////
void aiBFHideBehindEntRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDHideBehindEnt* mydata = (AIBDHideBehindEnt*) behavior->data;
    Vec3 temp;
    F32 distFromMe;
    Entity *protector = erGetEnt(mydata->protectEnt);
    AIVars* ai = ENTAI(e);

    if(!protector)
    {
        behavior->finished = 1;
        return;
    }

    if(ABS_TIME_SINCE(mydata->HBEcheckedProximity) > SEC_TO_ABS(1))
    {
        int entArray[MAX_ENTITIES_PRIVATE];
        int    entCount, numEnemies = 0;
        int i, idx;
        F32 dist;

        mydata->HBEcheckedProximity = ABS_TIME;
        entCount = mgGetNodesInRange(0, ENTPOS(e), (void**)entArray, 0);
        zeroVec3(mydata->enemyGeoAvg);
        mydata->amScared = false;

        for(i = 0; i < entCount; i++)
        {
            idx = entArray[i];

            if(ENTTYPE_BY_ID(idx) == ENTTYPE_CRITTER)
            {
                Entity* entProx = validEntFromId(idx);

                if(entProx && entProx->pchar
                    && (entProx->pchar->iAllyID==kAllyID_Monster))
                {
                    Character* pchar = entProx->pchar;

                    if(    pchar &&
                        pchar->attrCur.fHitPoints > 0)
                    {
                        dist = distance3Squared(ENTPOS(e), ENTPOS(entProx));

                        if(dist <= SQR(ai->HBEsearchdist))
                        {
                            if(aiEntityCanSeeEntity(e, entProx, mydata->HBEsearchdist))
                            {
                                /*
                                subVec3(entProx->mat[3], protector->mat[3], temp);
                                addVec3(geoAvg, temp, geoAvg);
                                */
                                addVec3(ENTPOS(entProx), mydata->enemyGeoAvg, mydata->enemyGeoAvg);
                                numEnemies++;
                                mydata->amScared = true;
                            }
                        }
                    }
                }
            }
        }
        if(numEnemies > 1)
        {
            mydata->enemyGeoAvg[0] = mydata->enemyGeoAvg[0] ? (mydata->enemyGeoAvg[0] / numEnemies) : 0;
            mydata->enemyGeoAvg[1] = mydata->enemyGeoAvg[1] ? (mydata->enemyGeoAvg[1] / numEnemies) : 0;
            mydata->enemyGeoAvg[2] = mydata->enemyGeoAvg[2] ? (mydata->enemyGeoAvg[2] / numEnemies) : 0;
        }
    }

    // check numEnemies here??
    if(!mydata->amScared) 
    {
        distFromMe = distance3Squared(ENTPOS(protector), ENTPOS(e));

        CLRB(e->seq->sticky_state, STATE_FEAR);  

        if(!ai->navSearch->path.head && distFromMe > SQR(20))
        {
            aiSetFollowTargetEntity(e, ai, protector, AI_FOLLOWTARGETREASON_SAVIOR);
            ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 7.0;
            aiConstructPathToTarget(e);
            aiStartPath(e, 0, 0/*distFromMe > SQR(30) ? 0 : 1*/);
        }
        return;
    }

    // find point five feet behind guy you're hiding behind
    subVec3(mydata->enemyGeoAvg, ENTPOS(protector), temp);
    normalVec3(temp);
    scaleVec3(temp, -5, temp);
    addVec3(temp, ENTPOS(protector), temp);

    distFromMe = distance3Squared(temp, ENTPOS(e));

    if(ai->navSearch->path.head)
    {
        F32 distFromPath = distance3Squared(temp, ai->navSearch->path.tail->pos);

        if(distFromMe < distFromPath || distFromPath > SQR(50))
        {
            clearNavSearch(ai->navSearch);
        }
    }

    if((!ai->navSearch->path.head && !ai->motion.type) &&
        distFromMe > ai->inputs.standoffDistance ? ai->inputs.standoffDistance : SQR(3))
    {
        ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 2.5;
        aiSetFollowTargetPosition(e, ai, temp, AI_FOLLOWTARGETREASON_COWER);
        aiConstructPathToTarget(e);

        aiStartPath(e, 0, 0);
    }

    SETB(e->seq->sticky_state, STATE_FEAR);
}

void aiBFHideBehindEntSet(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDHideBehindEnt* mydata = (AIBDHideBehindEnt*) behavior->data;
    AIVars* ai = ENTAI(e);

    if(!mydata->protectEnt && ai->protectEnt)
        mydata->protectEnt = ai->protectEnt;
}

//////////////////////////////////////////////////////////////////////////
// HostageScared
//////////////////////////////////////////////////////////////////////////
void aiBFHostageScaredRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDHostageScared* mydata = (AIBDHostageScared*) behavior->data;
    AIVars* ai = ENTAI(e);
    int entArray[MAX_ENTITIES_PRIVATE];
    int    entCount;
    int i;

    if(!mydata->whoScaredMe){
        if(ABS_TIME_SINCE(mydata->time.checkedProximity) > SEC_TO_ABS(2)){
            entCount = mgGetNodesInRange(0, ENTPOS(e), (void**)entArray, 0);

            for(i = 0; i < entCount; i++){
                int idx = entArray[i];

                if(ENTTYPE_BY_ID(idx) == ENTTYPE_CRITTER){
                    Entity* entProx = validEntFromId(idx);

                    if(entProx && entProx->pchar
                        && (entProx->pchar->iAllyID==kAllyID_Monster || entProx->pchar->iAllyID==kAllyID_Villain))
                    {
                        Character* pchar = entProx->pchar;

                        if(    pchar &&
                            pchar->attrCur.fHitPoints > 0)
                        {
                            F32 dist = distance3Squared(ENTPOS(e), ENTPOS(entProx));

                            if(dist <= SQR(50)){
                                if(aiEntityCanSeeEntity(e, entProx, 50)){
                                    mydata->whoScaredMe = entProx;

                                    aiGetProxEntStatus(ai->proxEntStatusTable, entProx, 1);

                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if(mydata->whoScaredMe){
        Character* pchar = mydata->whoScaredMe->pchar;

        if(    !pchar ||
            !pchar->attrCur.fHitPoints)
        {
            mydata->whoScaredMe = NULL;
        }
    }

    switch(mydata->state){
        case AIB_HOSTAGE_SCARED_INIT:{
            // Initialize the script.

            mydata->time.beginState = ABS_TIME;
            mydata->time.checkedProximity = ABS_TIME;
            mydata->state = AIB_HOSTAGE_SCARED_COWER;

            break;
                                     }

        case AIB_HOSTAGE_SCARED_COWER:{
            // Stand around looking scared.

            aiDiscardFollowTarget(e, "Being scared.", false);

            SETB(e->seq->sticky_state, STATE_FEAR);

            if(mydata->whoScaredMe){
                mydata->state = AIB_HOSTAGE_SCARED_RUN;
                mydata->time.beginState = ABS_TIME;
            }
            else if(ABS_TIME_SINCE(mydata->time.beginState) > SEC_TO_ABS(4)){
                U32 time_since = ABS_TIME_SINCE(mydata->time.beginState) - SEC_TO_ABS(4);

                if((U32)rand() % SEC_TO_ABS(4) < time_since){
                    mydata->state = AIB_HOSTAGE_SCARED_RUN;
                    mydata->time.beginState = ABS_TIME;
                }
            }

            break;
                                      }

        case AIB_HOSTAGE_SCARED_RUN:{
            if(!ai->motion.type && ABS_TIME_SINCE(ai->time.foundPath) > SEC_TO_ABS(1)){
                Vec3 pos;
                Beacon* sourceBeacon;

                ai->time.foundPath = ABS_TIME;

                if(mydata->whoScaredMe){
                    copyVec3(ENTPOS(mydata->whoScaredMe), pos);
                }else{
                    copyVec3(ENTPOS(e), pos);
                }

                vecX(pos) += getCappedRandom(201) - 100;
                vecY(pos) += getCappedRandom(101) - 50;
                vecZ(pos) += getCappedRandom(201) - 100;

                beaconSetPathFindEntity(e, aiGetJumpHeight(e));

                sourceBeacon = beaconGetClosestCombatBeacon(ENTPOS(e), 1, NULL, GCCB_PREFER_LOS, NULL);

                if(sourceBeacon){
                    // Find a targetBeacon that can be reached from the sourceBeacon, but don't use LOS because we don't care.

                    Beacon* targetBeacon = beaconGetClosestCombatBeacon(pos, 1, sourceBeacon, GCCB_IGNORE_LOS, NULL);

                    if(targetBeacon){
                        aiSetFollowTargetPosition(e, ai, posPoint(targetBeacon), AI_FOLLOWTARGETREASON_ESCAPE);

                        aiConstructPathToTarget(e);

                        if(!ai->navSearch->path.head){
                            aiDiscardFollowTarget(e, "Can't find a path.", false);
                        }else{
                            aiStartPath(e, 0, 0);
                        }
                    }
                }
            }

            if(ABS_TIME_SINCE(mydata->time.beginState) > SEC_TO_ABS(15)){
                mydata->state = AIB_HOSTAGE_SCARED_COWER;
                mydata->time.beginState = ABS_TIME;
            }

            break;
                                    }
    }
}

//////////////////////////////////////////////////////////////////////////
// Loop
//////////////////////////////////////////////////////////////////////////
void aiBFLoopDebugStr(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, char** estr, char* indent)
{
    AIBDLoop* mydata = (AIBDLoop*)behavior->data;

    if(mydata->times)
        estrConcatf(estr, "Times left: ^4%i^0,", mydata->times);

    estrConcatf(estr, "String:^9%s",  mydata->estr);
}

void aiBFLoopFinish(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDLoop* mydata = (AIBDLoop*) behavior->data;

    estrDestroy(&mydata->estr);
}

void aiBFLoopRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDLoop* mydata = (AIBDLoop*) behavior->data;
    AIBParsedString* parsed;
    int i, n;

    if(mydata->times && !--mydata->times)
    {
        behavior->finished = 1;
        return;
    }

    parsed = aiBehaviorParseString(mydata->estr);
    aiBehaviorProcessString(e, aibase, behaviors, mydata->estr, parsed, false);

    n = eaSize(&mydata->stringFlags);
    for(i = 0; i < n; i++)
        aiBehaviorPropagateStringToChildren(e, aibase, behaviors, behavior, mydata->stringFlags[i]);
    n = eaSize(&behavior->flags);
    for(i = 0; i < n; i++)
        aiBehaviorPropagateFlagToChildren(e, aibase, behaviors, behavior, behavior->flags[i]);
}

int aiBFLoopString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDLoop* mydata = (AIBDLoop*) behavior->data;

    if(!stricmp(param->function, "Times"))
    {
        if(eaSize(&param->params))
        {
            mydata->times = aiBehaviorParseFloat(param->params[0]);
            return 1;
        }
    }
    else if(!mydata->estr && !eaSize(&param->params))
    {
        estrPrintCharString(&mydata->estr, param->function);
        return 1;
    }
    else
    {
        eaPush(&mydata->stringFlags, param);
        return 1;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////
// MoveToPos
//////////////////////////////////////////////////////////////////////////
void* aiCritterMsgHandlerMoveToPos(Entity* e, AIMessage msg, AIMessageParams params);

void aiBFMoveToPosDebugStr(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, char** estr, char* indent)
{
    AIBDMoveToPos* mydata = (AIBDMoveToPos*)behavior->data;

    estrConcatf(estr, "%spos: (^4%0.1f^0, ^4%0.1f^0, ^4%0.1f^0), target_radius: ^4%0.1f^0", indent,
        mydata->pos[0], mydata->pos[1], mydata->pos[2], mydata->target_radius);
}

int aiBFMoveToPosFlag(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, AIBehaviorFlag* flag)
{
    AIBDMoveToPos* mydata = (AIBDMoveToPos*)behavior->data;
    if(flag->info->type == AIBF_FLOATPARAM || flag->info->type == AIBF_POS)
    {
        memcpy(mydata->pos, flag->floatarray, sizeof(mydata->pos));
        return FLAG_PROCESSED;
    }
    else if(flag->info->type == AIBF_INTPARAM)
    {
        mydata->target_radius = flag->intarray[0];
        return FLAG_PROCESSED;
    }
    else if(flag->info->type == AIBF_VARIATION)
    {
        if(mydata->pos[0] == 0.0 && mydata->pos[1] == 0.0 && mydata->pos[2] == 0.0)
        {
            if(flag == behavior->flags[eaSize(&behavior->flags)-1])
                return FLAG_PROCESSED;    // this is the last flag so we can't move it back... give up
            else
                return FLAG_MOVE_TO_END;
        }
        else
            addVec3(flag->floatarray, mydata->pos, mydata->pos);
        return FLAG_PROCESSED;
    }
    else if (flag->info->type == AIBF_JUMPSPEED)
    {
        float jumpSpeed = flag->floatarray[0];
        if (jumpSpeed <= 0.f)
        {
            jumpSpeed = 1.0f;
        }
        mydata->jumpSpeedMultiplier = jumpSpeed;
        return FLAG_PROCESSED;
    }
    return FLAG_NOT_PROCESSED;
}

int aiBFMoveToPosDo(Entity* e, AIVarsBase* aibase, Vec3 target, F32 radius)
{
    AIVars* ai = ENTAI(e);
    if(ai->motion.type == AI_MOTIONTYPE_NONE && !ai->navSearch->path.head &&
        (distance3Squared(ENTPOS(e), target) < SQR(radius) ||
        distance3XZ(ENTPOS(e), target) < SQR(AI_MINIMUM_STANDOFF_DIST)))
    {
        return 1;
    }

    if(ai->followTarget.type == AI_FOLLOWTARGET_NONE)
        aiSetFollowTargetPosition(e, ENTAI(e), target, AI_FOLLOWTARGETREASON_BEHAVIOR);

    if(!ai->navSearch->path.head && ai->motion.type == AI_MOTIONTYPE_NONE)
    {
        aiConstructPathToTarget(e);
        aiStartPath(e, 0, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);
    }

    return 0;
}

void aiBFMoveToPosRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDMoveToPos* mydata = (AIBDMoveToPos*) behavior->data;

    if(aiBFMoveToPosDo(e, aibase, mydata->pos, mydata->target_radius))
    {
        if (e->pchar)
            e->pchar->bForceMove = false;
        behavior->finished = 1;
    }
}

void aiBFMoveToPosSet(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDMoveToPos* mydata = (AIBDMoveToPos*) behavior->data;
    AIVars* ai = ENTAI(e);

    if(!mydata->target_radius)
        mydata->target_radius = 15.0;

    if (mydata->jumpSpeedMultiplier)
    {
        ai->motion.superJumping = 1;
        ai->motion.jump.superJumpSpeedMultiplier = mydata->jumpSpeedMultiplier;
        behavior->uninterruptable = 1;
    }

    aiDiscardFollowTarget(e, "Started behavior ^9MoveToPos^0", false);
    aiSetMessageHandler(e, aiCritterMsgHandlerMoveToPos);
    ai->sendEarlyBlock = 1;
    ai->followTarget.standoffDistance = ai->inputs.standoffDistance = AI_MINIMUM_STANDOFF_DIST;
    aiSetFollowTargetPosition(e, ENTAI(e), mydata->pos, AI_FOLLOWTARGETREASON_BEHAVIOR);
    clearNavSearch(ai->navSearch);
    aiConstructPathToTarget(e);
    aiStartPath(e, 0, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);
}

void aiBFMoveToPosUnset(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIVars* ai = ENTAI(e);


    ai->motion.superJumping = 0;
    ai->motion.jump.superJumpSpeedMultiplier = 1.f;
    aiDiscardFollowTarget(e, "Switching out of behavior ^9MoveToPos^0", false);
    ai->sendEarlyBlock = 0;
    clearNavSearch(ai->navSearch);
    aiSetMessageHandler(e, aiCritterMsgHandlerDefault);
}

void* aiCritterMsgHandlerMoveToPos(Entity* e, AIMessage msg, AIMessageParams params)
{
    AIBehavior* behavior = aiBehaviorGetCurBehaviorInternal(e, ENTAI(e)->base);
    AIBDMoveToPos* mydata;

    if(!e || !behavior)
    {
        Errorf("aiCritterMsgHandlerMoveToPos called without valid ent or behavior");
        return NULL;
    }

    mydata = (AIBDMoveToPos*) behavior->data;

    switch(msg)
    {
        xcase AI_MSG_PATH_EARLY_BLOCK:    // fall through
    case AI_MSG_FOLLOW_TARGET_REACHED:
        if(distance3Squared(ENTPOS(e), mydata->pos) < SQR(mydata->target_radius))
        {
            if (e->pchar)
                e->pchar->bForceMove = false;
            behavior->finished = 1;
        }
xdefault:
        aiCritterMsgHandlerDefault(e, msg, params);
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
// MoveToRandPoint
//////////////////////////////////////////////////////////////////////////
#define AI_FOLLOW_LEADER_DISTANCE (10)

void* aiCritterMsgHandlerFollowLeader(Entity* e, AIMessage msg, AIMessageParams params){
    AIVars* ai = ENTAI(e);

    switch(msg){
        case AI_MSG_PATH_BLOCKED:{
            if(ai->preventShortcut){
                aiPathUpdateBlockedWaypoint(ai->navSearch->path.curWaypoint);

                // The target isn't a player, so I'm probably just wandering around.
                //   I'll just find a new path eventually.

                AI_LOG(    AI_LOG_PATH,
                    (e,
                    "Got stuck while following leader.\n"));

                aiDiscardFollowTarget(e, "Ran into something, not sure what.", false);
                ai->waitingForTargetToMove = 0;

            }else{
                // If I'm stuck and I have already tried shortcutting,
                //   then redo the path but don't allow shortcutting.

                AI_LOG(    AI_LOG_PATH,
                    (e,
                    "I got stuck, trying not shortcutting.\n"));

                aiConstructPathToTarget(e);

                aiStartPath(e, 1, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);

                ai->waitingForTargetToMove = 1;
            }

            break;
                                 }

        case AI_MSG_BAD_PATH:{
            clearNavSearch(ai->navSearch);

            aiDiscardFollowTarget(e, "Bad path.", false);

            break;
                             }

        case AI_MSG_FOLLOW_TARGET_REACHED:{
            aiDiscardFollowTarget(e, "Reached follow target (leader).", true);

            ai->time.checkedForShortcut = 0;
            ai->doNotRun = ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : 0;

            break;
                                          }

        case AI_MSG_ATTACK_TARGET_MOVED:{
            // The target has moved, so reset the find path timer.

            AI_LOG(    AI_LOG_PATH,
                (e,
                "Attack target moved a lot, ^1cancelling path.\n"));

            aiDiscardFollowTarget(e, "Leader moved a lot, cancelling path", false);

            ai->waitingForTargetToMove = 0;

            break;
                                        }
    }

    return 0;
}


void aiBFMoveToRandPointRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDMoveToRandPoint* mydata = (AIBDMoveToRandPoint*) behavior->data;
    AIVars* ai = ENTAI(e);

    // Do behavior for non-leaders if we're in group mode
    // NOTE: follower behavior currently does not end at all. This is ok for now seeing as this
    // is just for backwards compatibility and only used from a looping alias that doesn't end either
    if(mydata->groupMode && ai->teamMemberInfo.team && 
        e != ai->teamMemberInfo.team->members.list->entity)
    {
        Entity* leader = ai->teamMemberInfo.team->members.list->entity;
        float distSQR = distance3Squared(ENTPOS(e), ENTPOS(leader));

        ai->doNotRun = ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : (distSQR < SQR(30)) ? TRUE : FALSE;

        if(    ai->followTarget.type != AI_FOLLOWTARGET_ENTITY ||
            ai->followTarget.entity != leader ||
            ai->motion.type == AI_MOTIONTYPE_NONE)
        {    
            if(distSQR > SQR(AI_FOLLOW_LEADER_DISTANCE))
            {
                ai->time.foundPath = ABS_TIME;

                ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : (AI_FOLLOW_LEADER_DISTANCE + 1);    

                aiDiscardFollowTarget(e, "Began following leader.", false);

                aiSetFollowTargetEntity(e, ai, leader, AI_FOLLOWTARGETREASON_LEADER);

                aiGetProxEntStatus(ai->proxEntStatusTable, leader, 1);

                aiConstructPathToTarget(e);

                aiStartPath(e, 0, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);
            }
        }

        return;
    }

    if(SQR(ai->followTarget.standoffDistance) >= distance3Squared(ENTPOS(e), mydata->patrolPoint))
    {
        behavior->finished = 1;
        return;
    }

    // IF i have been patrolling for too long
    //   THEN discard current patrol point (timed out)
    if(ABS_TIME_SINCE(mydata->patrolPointFoundTime) > SEC_TO_ABS(3 * 60))
        mydata->patrolPointValid = 0;

    // ELSE IF i have a target patrol point
    //   THEN start moving to it
    if(mydata->patrolPointValid)
    {    
        // start a new path
        if( !ai->motion.type ||
            !ai->navSearch->path.head ||
            ai->followTarget.type != AI_FOLLOWTARGET_POSITION ||
            ai->followTarget.reason != AI_FOLLOWTARGETREASON_PATROLPOINT ||
            !sameVec3(ai->followTarget.pos, mydata->patrolPoint))
        {
            aiSetFollowTargetPosition(e, ai, mydata->patrolPoint, AI_FOLLOWTARGETREASON_PATROLPOINT);
            aiConstructPathToTarget(e);

            //ai->doNotRun = 1; 
            aiStartPath(e, 0, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);

            // IF we failed to find a path, 
            //   OR it has taken too long to reach this point,
            // THEN discard the patrol point, as it is unreachable
            if(!ai->navSearch->path.head)
            {
                mydata->patrolPointValid = 0;
            }

            ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 10;
        }
    }
    else
    {
        Beacon * beacon = aiFindBeaconInRange(e, ai, ENTPOS(e), 200, 100, 100, 200, NULL);
        if(beacon)
        {
            copyVec3(posPoint(beacon), mydata->patrolPoint);
            mydata->patrolPointValid = TRUE;
            mydata->patrolPointFoundTime = ABS_TIME;
        }
        else
        {
            behavior->finished = 1;
            return;
        }
    }
}

void aiBFMoveToRandPointSet(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDMoveToRandPoint* mydata = (AIBDMoveToRandPoint*) behavior->data;
    AIVars* ai = ENTAI(e);

    if(mydata->groupMode)
    {
        aiCritterCheckProximity(e, 0);

        if(e != ai->teamMemberInfo.team->members.list->entity)
            aiSetMessageHandler(e, aiCritterMsgHandlerFollowLeader);
    }
}

int aiBFMoveToRandPointString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDMoveToRandPoint* mydata = (AIBDMoveToRandPoint*) behavior->data;

    if(!stricmp(param->function, "Group"))
    {
        if(ENTTYPE(e) != ENTTYPE_NPC)
        {
            mydata->groupMode = 1;
            return 1;
        }
        else
            Errorf("NPCs can use MoveToRandPoint but not in team mode");
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////
// Patrol
//////////////////////////////////////////////////////////////////////////
void aiBFPatrolHelperMoveFollowPoint(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, AIBDPatrol* mydata, int newDirection, int nextPoint)
{
    AIVars* ai = ENTAI(e);
    AITeam* team = ai->teamMemberInfo.team;
    F32 speedPerAITick = team->patrolSpeed * 30 / 2;
    F32 minimumReachedDistance = max(speedPerAITick, team->numPatrollers * ai->collisionRadius / 4);

    if(distance3Squared(mydata->followPoint[3], mydata->route->locations[nextPoint]) < SQR(minimumReachedDistance))
    {
        if(distance3Squared(ENTPOS(e), mydata->route->locations[nextPoint]) < SQR(minimumReachedDistance))
        {
            if(nextPoint == mydata->route->numlocs - 1 && mydata->route->routetype == PATROL_ONEWAY)
            {
                char buf[200];
                F32* targetPos = mydata->route->locations[ai->teamMemberInfo.team->curPatrolPoint];
                sprintf(buf, "OverrideSpawnPos(%f,%f,%f)", targetPos[0], targetPos[1], targetPos[2]);
                aiBehaviorAddPermFlag(e, aibase, buf);
                aiDiscardFollowTarget(e, "Reached the end of my patrol", true);

                behavior->finished = 1;
                return;
            }
            else if(newDirection)
                team->patrolDirection = newDirection;
            team->curPatrolPoint = nextPoint;
        }
        else
            copyVec3(mydata->route->locations[nextPoint], mydata->followPoint[3]);
    }
    else if(ai->canFly && distance3Squared(mydata->followPoint[3], ENTPOS(e)) < SQR(speedPerAITick) ||
        !ai->canFly && distance3SquaredXZ(mydata->followPoint[3], ENTPOS(e)))
    {
        Vec3 direction;

        subVec3(mydata->route->locations[nextPoint], mydata->followPoint[3], direction);
        normalVec3(direction);
        scaleVec3(direction, speedPerAITick, direction);

        addVec3(mydata->followPoint[3], direction, mydata->followPoint[3]);
    }
}

void* aiBFPatrolMsgHandler(Entity* e, AIMessage msg, AIMessageParams params)
{
    AIVars* ai = ENTAI(e);
    AIBehavior* behavior = aiBehaviorGetCurBehaviorInternal(e, ai->base);
    AIBDPatrol* mydata = (AIBDPatrol*)behavior->data;
    AITeam* team = ai->teamMemberInfo.team;
    U32 nextPoint = team->curPatrolPoint + team->patrolDirection;
    int newDirection = 0;

    if(nextPoint >= mydata->route->numlocs)
    {
        if(mydata->route->routetype == PATROL_PINGPONG)
        {
            newDirection = -1 * team->patrolDirection;
            nextPoint = team->curPatrolPoint + -1 * team->patrolDirection;
        }
        else if(mydata->route->routetype == PATROL_CIRCLE)
            nextPoint = 0;
        else
        {
            behavior->finished = 1;
            return NULL;
        }
    }

    switch(msg)
    {
    case AI_MSG_PATH_BLOCKED:
        if(ai->preventShortcut)
            aiPathUpdateBlockedWaypoint(ai->navSearch->path.curWaypoint);

        mydata->lastStuck = ABS_TIME;
        aiBFPatrolHelperMoveFollowPoint(e, ai->base, behavior, mydata, newDirection, nextPoint);

        aiConstructPathToTarget(e);
        aiStartPath(e, 1, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);
        break;
    case AI_MSG_FOLLOW_TARGET_REACHED:
        if(ai->teamMemberInfo.team->patrolTypeWire)
        {
            if(newDirection)
                team->patrolDirection = newDirection;
            team->curPatrolPoint = nextPoint;
            aiFollowPathWire(e, ai, mydata->route->locations[nextPoint], AI_FOLLOWTARGETREASON_PATROLPOINT);
        }
        else if(ai->teamMemberInfo.team->patrolTypeWireComplex)
        {
            // just using arbitrary numbers for the resolution and curve distance right now...
            aiDiscardFollowTarget(e, "Starting on complex wire path", false);
            CreateComplexWire(e, ai, mydata->route->locations, mydata->route->numlocs, 1.5, 20.0);
            ai->navSearch->path.curWaypoint = ai->navSearch->path.head;
            aiSetFollowTargetPosition(e, ai, ai->navSearch->path.head->pos, AI_FOLLOWTARGETREASON_PATROLPOINT);
            ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 0;
            aiStartPath(e, 1, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);
        }
        else
            aiBFPatrolHelperMoveFollowPoint(e, ai->base, behavior, mydata, newDirection, nextPoint);
    }
    return NULL;
}

void aiBFPatrolDebugStr(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, char** estr, char* indent)
{
    AIBDPatrol* mydata = (AIBDPatrol*)behavior->data;
    AITeam* team = ENTAI(e)->teamMemberInfo.team;

    estrConcatCharString(estr, indent);

    if(mydata && team)
    {
        if(!mydata->route)
        {
            estrConcatStaticCharArray(estr, "^2ERROR: NO ROUTE DEFINED");
            return;
        }

        if(mydata->route->name[0])
            estrConcatf(estr, "Named route: ^5%s^0 ", mydata->route->name);
        switch(mydata->route->routetype)
        {
            xcase PATROL_PINGPONG:        // default
                if(team->patrolDirection)
                    estrConcatStaticCharArray(estr, "Pingpong ^3+^0 ");
                else
                    estrConcatStaticCharArray(estr, "Pingpong ^3-^0 ");
            xcase PATROL_CIRCLE:
                estrConcatStaticCharArray(estr, "Circle ");
            xcase PATROL_ONEWAY:
                estrConcatStaticCharArray(estr, "Oneway ");
        }
        if(team->patrolTeamLeaderBlocked)
            estrConcatStaticCharArray(estr, "^1LEADER BLOCKED^0 ");
        if(team->patrolTypeWire)
            estrConcatStaticCharArray(estr, "^3WIRE^0 ");
        else if(team->patrolTypeWireComplex)
            estrConcatStaticCharArray(estr, "^3COMPLEX^0 ");
        estrConcatf(estr, "Heading from Patrol Point: ^4%d^0 to ^4%d^0",
            team->curPatrolPoint+1, team->curPatrolPoint + team->patrolDirection + 1);
    }
}

int aiBFPatrolFlag(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, AIBehaviorFlag* flag)
{
    AIBDPatrol* mydata = (AIBDPatrol*) behavior->data;
    if(flag->info->type == AIBF_MOTION_WIRE)
    {
        mydata->targetType = AI_PRIORITY_TARGET_WIRE;
        return FLAG_PROCESSED;
    } 
    else if (flag->info->type == AIBF_PATROL_FLY_TURN_RADIUS)
    {
        mydata->turnRadius = flag->floatarray[0];
        return FLAG_PROCESSED;
    }
    return FLAG_NOT_PROCESSED;
}

void aiBFPatrolRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDPatrol* mydata = (AIBDPatrol*) behavior->data;
    AIVars* ai = ENTAI(e);
    AITeam* team = ai->teamMemberInfo.team;
    F32 newSpeed, newMySpeed;

    devassert(mydata->targetType == AI_PRIORITY_TARGET_WIRE || mydata->followPoint);

    if(ai->teamMemberInfo.team->patrolTypeWire || ai->teamMemberInfo.team->patrolTypeWireComplex)
        return;

    if(!devassertmsg(ai->teamMemberInfo.team->patrolDirection, "Team changed since behaviorset got called"))
    {
        mydata->followPoint = ai->teamMemberInfo.team->followPoint;

        if(!mydata->followPoint[3][0] && !mydata->followPoint[3][1] && !mydata->followPoint[3][2])
            copyVec3(mydata->route->locations[0], mydata->followPoint[3]);
        if(!ai->teamMemberInfo.team->patrolDirection)
            ai->teamMemberInfo.team->patrolDirection = 1;
    }

    newMySpeed = e->motion->input.surf_mods[ai->isFlying ? SURFPARAM_AIR : SURFPARAM_GROUND].max_speed;
    if(mydata->mySpeed != newMySpeed)
    {
        newSpeed = team->patrolSpeed * team->numPatrollers;
        newSpeed -= mydata->mySpeed;
        newSpeed += newMySpeed;
        newSpeed /= team->numPatrollers;
        team->patrolSpeed = newSpeed;

        mydata->mySpeed = newMySpeed;
    }

    if(mydata->followPoint != NULL && distance3Squared(ENTPOS(e), mydata->followPoint[3]) > SQR(3 * team->patrolSpeed * 30))
        ai->inputs.limitedSpeed = team->patrolSpeed * 1.2;
    else
        ai->inputs.limitedSpeed = team->patrolSpeed;

    ai->followTarget.standoffDistance = ai->inputs.standoffDistance ?
        ai->inputs.standoffDistance : team->patrolSpeed * 30 / 4;
    ai->followTarget.standoffDistance = MAX(ai->followTarget.standoffDistance, 4.f); // hard limit for really slow moving patrols to not get stuck on stuff

    if(ABS_TIME_SINCE(mydata->lastStuck) > SEC_TO_ABS(1) && 
        ai->navSearch->path.head && 
        mydata->followPoint != NULL && 
        distance3Squared(ENTPOS(e), mydata->followPoint[3]) < SQR(150) && 
        aiCanWalkBetweenPointsQuickCheck(ENTPOS(e), 4, mydata->followPoint[3], 4))
    {
        clearNavSearch(ai->navSearch);
        aiConstructPathToTarget(e);
        aiStartPath(e, 0, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);
    }
    else if(!ai->followTarget.movingPoint || (!ai->navSearch->path.head && !ai->motion.type))
    {
        clearNavSearch(ai->navSearch);
        aiSetFollowTargetMovingPoint(e, ai, mydata->followPoint, AI_FOLLOWTARGETREASON_PATROLPOINT);
        ai->followTarget.standoffDistance = ai->inputs.standoffDistance ?
            ai->inputs.standoffDistance : team->patrolSpeed * 30 / 4;
        ai->followTarget.standoffDistance = MAX(ai->followTarget.standoffDistance, 4.f); // hard limit for really slow moving patrols to not get stuck on stuff
        aiConstructPathToTarget(e);
        aiStartPath(e, ABS_TIME_SINCE(mydata->lastStuck) < SEC_TO_ABS(3), ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);
    }
}

void aiBFPatrolSet(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDPatrol* mydata = (AIBDPatrol*) behavior->data;
    AIVars* ai = ENTAI(e);
    AITeam* team = ai->teamMemberInfo.team;
    F32 mySpeed = e->motion->input.surf_mods[ai->isFlying ? SURFPARAM_AIR : SURFPARAM_GROUND].max_speed;

    mydata->mySpeed = mySpeed;
    aiCritterCheckProximity(e, 0);    // force a team creation

    aiSetMessageHandler(e, aiBFPatrolMsgHandler);

    if(mydata->route != team->teamPatrolRoute)
    {
        team->patrolDirection = 0;
        copyVec3(zerovec3, team->followPoint[3]);
        team->curPatrolPoint = 0;
        team->teamPatrolRoute = mydata->route;
    }

    if(!team->patrolDirection)
        team->patrolDirection = 1;

    if(mydata->targetType == AI_PRIORITY_TARGET_WIRE)
    {
        if(ai->teamMemberInfo.team)
        {
            team->patrolTypeWire = 1;
            ai->pitchWhenTurning = 1;
        }
        else
        {
            behavior->finished = 1;
            return;
        }

        // setting up turn radius if we have one
        ai->turnSpeed = mydata->turnRadius;

        //        aiFollowPatrolRoute(e, ai, behavior, mydata->route, 0,
        //            ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun, behavior->combatOverride);
        aiFollowPathWire(e, ai, mydata->route->locations[ai->teamMemberInfo.team->curPatrolPoint], AI_FOLLOWTARGETREASON_PATROLPOINT);
    }
    else
    {
        team->patrolSpeed = (team->patrolSpeed * team->numPatrollers
            + mySpeed) / (team->numPatrollers+1);
        team->numPatrollers++;

        // set followtarget to be the followPoint
        mydata->followPoint = team->followPoint;

        if(!mydata->followPoint[3][0] && !mydata->followPoint[3][1] && !mydata->followPoint[3][2])
            copyVec3(mydata->route->locations[0], mydata->followPoint[3]);
        aiSetFollowTargetMovingPoint(e, ai, mydata->followPoint, AI_FOLLOWTARGETREASON_PATROLPOINT);
        aiConstructPathToTarget(e);
        aiStartPath(e, 0, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);
    }
}

void aiBFPatrolStart(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDPatrol* mydata = (AIBDPatrol*) behavior->data;

    if(!mydata->route)
        mydata->route = EncounterPatrolRoute(e);
    if(!mydata->route)
    {
        behavior->finished = 1;
        return;
    }
}

int aiBFPatrolString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDPatrol* mydata = (AIBDPatrol*) behavior->data;

    if(!eaSize(&param->params))
        return 0;

    if(!stricmp(param->function, "NamedRoute"))
    {
        mydata->route = PatrolRouteFind(param->params[0]->function);
        return 1;
    }

    return 0;
}

void aiBFPatrolUnset(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIVars* ai = ENTAI(e);
    AIBDPatrol* mydata = (AIBDPatrol*) behavior->data;
    
    // If coming out of a MotionWire path, unset the pitchWhenTurning flag
    if(mydata->targetType == AI_PRIORITY_TARGET_WIRE)
    {
        ai->pitchWhenTurning = 0;
    }
}

//////////////////////////////////////////////////////////////////////////
// PatrolRiktiDropship
//////////////////////////////////////////////////////////////////////////
void aiBFPatrolRiktiDropshipRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIVars* ai = ENTAI(e);
    AIProxEntStatus* status; 
    AIProxEntStatus* bestStatus = NULL;
    Entity* bestTarget = NULL;
    F32 maxDanger = -FLT_MAX;
    AIPowerInfo* info = aiCritterFindPowerByName( ai, "Energy_Burst" );

    if(info && info->teamInfo && 
        (e && e->pchar && !e->pchar->ppowCurrent && !e->pchar->ppowQueued) && 
        (info && info->power && info->power->fTimer == 0.0f))
    {

        for(status = ai->proxEntStatusTable->statusHead; status; status = status->tableList.next)
        {
            if(status->entity && !status->entity->erOwner &&
                status->entity != ai->attackTarget.entity &&
                distance3Squared(ENTPOS(status->entity), ENTPOS(e)) <
                SQR(250.0f)) 
            {
                if(status->dangerValue > maxDanger)
                {
                    bestTarget = status->entity;
                    maxDanger = status->dangerValue;
                    bestStatus = status;
                }
            }
        }

        if(bestTarget)
        {
            aiSetAttackTarget(e, bestTarget, bestStatus, "rikti dropship behavior", 0, 0);
        }

        if(ai->attackTarget.entity)
        {
            aiUsePower(e, info, NULL);
        }
    }

    aiBFPatrolRun(e, aibase, behaviors, behavior);
}

//////////////////////////////////////////////////////////////////////////
// Pet.Follow
//////////////////////////////////////////////////////////////////////////
void aiBFPetFollowRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIVars* ai = ENTAI(e);
    aiCritterDoDefault(e, ai, 1);
}

void aiBFPetFollowSet(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    if (ENTTYPE(e) == ENTTYPE_NPC) 
    {    
        AIVars* ai = ENTAI(e);

        if(e->move_type == MOVETYPE_WIRE){
            e->move_type = 0;
        }

        entSetSendOnOddSend(e, 0);

        SET_ENTINFO(e).send_hires_motion = 1;

        entSetProcessRate(e->owner, ENT_PROCESS_RATE_ALWAYS);

        ai->npc.wireWanderMode = 0;
        ai->npc.runningBehavior = 1;
    } 
}

//////////////////////////////////////////////////////////////////////////
// PickRandomWeapon
//////////////////////////////////////////////////////////////////////////
void aiBFPickRandomWeaponRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    aiCritterChooseRandomPower(e);
    behavior->finished = 1;
}

//////////////////////////////////////////////////////////////////////////
// PlayFX
//////////////////////////////////////////////////////////////////////////
void aiBFPlayFXFinish(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDPlayFX* mydata = (AIBDPlayFX*) behavior->data;
    estrDestroy(&mydata->estr);
}

void aiBFPlayFXRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDPlayFX* mydata = (AIBDPlayFX*) behavior->data;

    if(mydata->estr && mydata->estr[0]){
        NetFx nfx = {0};

        nfx.command = CREATE_ONESHOT_FX; //CREATE_MAINTAINED_FX
    
        nfx.handle = stringToReference(forwardSlashes(mydata->estr));

        if(nfx.handle){
            entAddFx(e, &nfx);
        }
    }

    behavior->finished = 1;
}

int aiBFPlayFXString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDPlayFX* mydata = (AIBDPlayFX*) behavior->data;

    if(!mydata->estr)
    {
        estrPrintCharString(&mydata->estr, param->function);
        return 1;
    }

    return 0;
}

extern StaticDefineInt PriorityFlags[];

//////////////////////////////////////////////////////////////////////////
// PriorityList
//////////////////////////////////////////////////////////////////////////
void aiBFPriorityListDebugStr(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, char** estr, char* indent)
{
    AIBDPriorityList* mydata = (AIBDPriorityList*)behavior->data;

    if(mydata)
    {
        int i;
        AIPriorityManager* manager = mydata->manager;
        //        estrConcatf(estr, "^3%s^0", aiGetPriorityName(mydata->manager));

        estrConcatf(estr, "%sActivity: ^8%s^0(^s^4%d^0x data^n), ", indent,
            aiGetActivityName(mydata->activity), ENTAI(e)->activityDataAllocated);

        estrConcatf(estr,
            "Priority%s: ^8%s^0.^8%s ^0(Time: ^4%1.1f^0)^0",
            manager->holdCurrentList ? "(held)" : "",
            manager->list ? manager->list->name : "NONE",
            aiGetPriorityName(manager),
            manager->priorityTime);

        if(manager->flags)
        {
            estrConcatStaticCharArray(estr, ", Flags: ");
            for(i = 0; i < eaiSize(&manager->flags); i++)
                estrConcatf(estr, "%s ", StaticDefineIntRevLookup(PriorityFlags, manager->flags[i]));
        }

        if(manager->nextList){
            estrConcatf(estr, 
                ", Next Priority: ^8%s",
                manager->nextList->name);
        }
    }
}

void aiBFPriorityListFinish(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDPriorityList* mydata = (AIBDPriorityList*)behavior->data;
    aiPriorityDestroy(mydata->manager);
    estrDestroy(&mydata->mypl);

    if(mydata->scopeVar)
        mydata->scopeVar->finished = 1;
}

int aiBFPriorityListFlag(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, AIBehaviorFlag* flag)
{
    AIBDPriorityList* mydata = (AIBDPriorityList*) behavior->data;
    if(flag->info->type == AIBF_STRINGPARAM)
    {
        estrPrintEString(&mydata->mypl, flag->strptr);
        return FLAG_PROCESSED;
    }
    return FLAG_NOT_PROCESSED;
}

void aiBFPriorityListRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDPriorityList* mydata = (AIBDPriorityList*) behavior->data;
    AIVars* ai = ENTAI(e);

    if(!mydata)
        return;

    if(ENTTYPE(e) == ENTTYPE_CRITTER)
    {
        PERFINFO_AUTO_START("UpdatePriorityCritter", 1);
        aiPriorityUpdateGeneric(e, ai, mydata->manager, .5f, aiCritterPriorityDoActionFunc, 1);
        PERFINFO_AUTO_STOP();

        PERFINFO_AUTO_START("DoActivityCritter", 1);
        aiCritterDoActivity(e, ai, mydata->activity);
        PERFINFO_AUTO_STOP();
    }
    else if(ENTTYPE(e) == ENTTYPE_NPC)
    {
        PERFINFO_AUTO_START("UpdatePriorityNPC", 1);
        aiPriorityUpdateGeneric(e, ai, mydata->manager, .5f, aiNPCPriorityDoActionFunc, 1);
        PERFINFO_AUTO_STOP();

        PERFINFO_AUTO_START("DoActivityNPC", 1);
        aiNPCDoActivity(e, ai);
        PERFINFO_AUTO_STOP();
    }
    else
    {
        PERFINFO_AUTO_START("UpdatePriorityDoor", 1);
        aiPriorityUpdateGeneric(e, ai, mydata->manager, .5f, aiDoorPriorityDoActionFunc, 1);
        PERFINFO_AUTO_STOP();
    }
}

void aiBFPriorityListSet(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDPriorityList* mydata = (AIBDPriorityList*) behavior->data;
    AIVars* ai = ENTAI(e);
    //    mydata->manager->list = aiGetPriorityListByName(mydata->manager->list->name);
    if(mydata->restartPL)
    {
        aiPriorityRestartList(mydata->manager);
        mydata->restartPL = 0;
    }

    if(ENTTYPE(e) == ENTTYPE_NPC) 
    {
        aiNPCActivityChanged(e, ai, AI_ACTIVITY_NONE);
        ai->npc.runningBehavior = 0;
    } 
}

void aiBFPriorityListStart(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDPriorityList* mydata = (AIBDPriorityList*) behavior->data;
    AIVars* ai = ENTAI(e);

    mydata->manager = aiPriorityManagerCreate();
    mydata->manager->myBehavior = behavior;

    if(mydata->mypl)
        aiPriorityQueuePriorityList(e, mydata->mypl, 0, mydata->manager);

    if(mydata->manager->list || mydata->manager->nextList)
    {
        if(ENTTYPE(e) == ENTTYPE_CRITTER)
            aiPriorityUpdateGeneric(e, ai, mydata->manager, .5f, aiCritterPriorityDoActionFunc, 0);
        else
            aiPriorityUpdateGeneric(e, ai, mydata->manager, .5f, aiNPCPriorityDoActionFunc, 0);
    }
    mydata->restartPL = 1;
}

int aiBFPriorityListString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDPriorityList* mydata = (AIBDPriorityList*) behavior->data;

    if(aiGetPriorityListByName(param->function))
    {
        estrPrintCharString(&mydata->mypl, param->function);
        return 1;
    }
    return 0;
}

void aiBFPriorityListUnset(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDPriorityList* mydata = (AIBDPriorityList*) behavior->data;
    AIVars* ai = ENTAI(e);

    if(mydata->manager->list || mydata->manager->nextList)
    {
        if(ENTTYPE(e) == ENTTYPE_CRITTER)
            aiPriorityUpdateGeneric(e, ai, mydata->manager, .5f, aiCritterPriorityDoActionFunc, 0);
        else
            aiPriorityUpdateGeneric(e, ai, mydata->manager, .5f, aiNPCPriorityDoActionFunc, 0);
    }
}

//////////////////////////////////////////////////////////////////////////
// RunAwayFar
//////////////////////////////////////////////////////////////////////////
void aiBFRunAwayFarRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDRunAwayFar* mydata = (AIBDRunAwayFar*) behavior->data;
    AIVars* ai = ENTAI(e);

    if(ABS_TIME_SINCE(ai->time.foundPath) > SEC_TO_ABS(2)){
        if(!ai->followTarget.type)
        {
            if(mydata->radius)
            {
                int i;
                float bestRatio = 1;
                Beacon* best = NULL;
                float bestNoDistRatio = 1;
                Beacon* bestNoDist = NULL;

                for(i = 0; i < basicBeaconArray.size; i++){
                    Beacon* b = basicBeaconArray.storage[i];
                    float ratio;
                    float beaconDistToMe = distance3(ENTPOS(e), posPoint(b));

                    ratio = distance3(ai->anchorPos, posPoint(b)) / beaconDistToMe;

                    if(ai->anchorRadius <= 0 || beaconDistToMe > ai->anchorRadius){
                        if(ratio > bestRatio){
                            best = b;
                            bestRatio = ratio;
                        }
                    }

                    if(ratio > bestNoDistRatio){
                        bestNoDist = b;
                        bestNoDistRatio = ratio;
                    }
                }

                if(!best){
                    best = bestNoDist;
                }

                if(best){
                    Vec3 pos;
                    copyVec3(posPoint(best), pos);
                    vecY(pos) -= 2;
                    aiSetFollowTargetPosition(e, ai, pos, AI_FOLLOWTARGETREASON_ESCAPE);
                }
                else
                {
                    aiDiscardFollowTarget(e, "Can't find beacon to run to", 0);
                    AI_LOG(AI_LOG_PRIORITY, (e, "Can't find farthest beacon.\n"));
                    behavior->finished = 1;
                    aiBehaviorAddString(e, ENTAI(e)->base, "RunIntoDoor");
                    return;
                }
            }
            else
            {
                int i;
                Beacon* best = NULL;
                F32 bestDist = -1.0;

                for(i = 0; i < basicBeaconArray.size; i++)
                {
                    Beacon* b = basicBeaconArray.storage[i];
                    float dist = distance3Squared(posPoint(b), ENTPOS(e));

                    if(dist > bestDist)
                    {
                        best = b;
                        bestDist = dist;
                    }
                }

                if(best)
                {
                    aiSetActivity(e, ai, AI_ACTIVITY_RUN_TO_TARGET);
                    aiSetFollowTargetBeacon(e, ai, best, AI_FOLLOWTARGETREASON_ESCAPE);
                }
                else
                {
                    aiDiscardFollowTarget(e, "Can't find beacon to run to", 0);
                    AI_LOG(AI_LOG_PRIORITY, (e, "Can't find farthest beacon.\n"));
                    behavior->finished = 1;
                    aiBehaviorAddString(e, ENTAI(e)->base, "RunIntoDoor");
                    return;
                }
            }
        }
        else if(ai->followTarget.type && !ai->navSearch->path.head){
            ai->time.foundPath = ABS_TIME;

            aiConstructPathToTarget(e);

            if(!ai->navSearch->path.head){
                SET_ENTINFO(e).kill_me_please = 1;
            }

            aiStartPath(e, 0, 0);
        }
    }

    if(ai->followTarget.type){
        if(mydata->radius)
        {
            if(!ai->navSearch->path.head)
            {
                ai->time.foundPath = ABS_TIME;

                aiConstructPathToTarget(e);

                aiStartPath(e, 0, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);
            }
            if(!ai->navSearch->path.head)
            {
                behavior->finished = 1;
                return;
            }

            if(ai->anchorRadius && distance3Squared(ENTPOS(e), ai->anchorPos) > SQR(ai->anchorRadius)){
                ai->waitForThink = 1;
            }
        }
        else
        {
            if(distance3Squared(ENTPOS(e), ai->followTarget.pos) < SQR(10)){
                aiDiscardFollowTarget(e, "Within 10 feet of target.", true);

                clearNavSearch(ai->navSearch);
                ai->waitForThink = 1;
            }
            else if(ai->navSearch->path.head){
                NavPath* path;
                NavPathWaypoint* lastWaypoint;
                Beacon* lastBeacon;
                float distToTargetSquared;
                float distFromTargetToPathEnd;

                path = &ai->navSearch->path;
                lastWaypoint = path->tail;
                lastBeacon = lastWaypoint->beacon;
                distToTargetSquared = distance3Squared(ENTPOS(e), ai->followTarget.pos);
                distFromTargetToPathEnd = distance3Squared(ai->followTarget.pos, posPoint(lastBeacon));

                if(distToTargetSquared < distFromTargetToPathEnd){
                    clearNavSearch(ai->navSearch);
                }
            }
        }
    }

    ai->doNotRun = ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->followTarget.type ? 0 : 1;
}

void aiBFRunAwayFarSet(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDRunAwayFar* mydata = (AIBDRunAwayFar*) behavior->data;
    AIVars* ai = ENTAI(e);
    Beacon* best = NULL;
    float bestDist = -1;
    int i;

    if(mydata->radius)
    {
        Vec3 centerPos = {0,0,0};
        int critterCount = 0;
        int entArray[MAX_ENTITIES_PRIVATE];
        int entCount;

        entCount = mgGetNodesInRange(0, ENTPOS(e), (void**)entArray, 0);

        for(i = 0; i < entCount; i++){
            Entity* proxEnt = entities[entArray[i]];

            if(    ENTTYPE(proxEnt) == ENTTYPE_CRITTER &&
                proxEnt->pchar->attrCur.fHitPoints >= 0 &&
                distance3Squared(ENTPOS(e), ENTPOS(proxEnt)) < SQR(50))
            {
                addVec3(centerPos, ENTPOS(proxEnt), centerPos);
                critterCount++;
            }
        }

        if(critterCount){
            scaleVec3(centerPos, 1.0f / critterCount, centerPos);
            copyVec3(centerPos, ai->anchorPos);
        }else{
            copyVec3(ENTPOS(e), ai->anchorPos);
        }

        AI_POINTLOG(AI_LOG_PATH, (e, ai->anchorPos, 0xffff0000, "%d Critters' Center", critterCount));
    }
}

int aiBFRunAwayFarString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDRunAwayFar* mydata = (AIBDRunAwayFar*) behavior->data;

    if(!stricmp(param->function, "radius") && eaSize(&param->params))
    {
        mydata->radius = aiBehaviorParseFloat(param->params[0]);
        return 1;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////
// RunIntoDoor
//////////////////////////////////////////////////////////////////////////
void aiBFRunIntoDoorRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDRunIntoDoor* mydata = (AIBDRunIntoDoor*)behavior->data;
    AIVars* ai = ENTAI(e);

    // temp? fix for the jump in hole animation ending with a looping CANTMOVE so movement doesn't get run
    if(ai->disappearInDoor && ai->navSearch->path.curWaypoint &&
        ai->navSearch->path.curWaypoint->connectType == NAVPATH_CONNECT_ENTERABLE && 
        getMoveFlags(e->seq, SEQMOVE_CANTMOVE))
    {
        behavior->finished = 1;
        e->fledMap = 1; //Nobody killed me.  I just ran off
        SET_ENTINFO(e).kill_me_please = 1;
        return;
    }

    if(!ai->navSearch->path.head && !ai->motion.type )
    {
        if( ABS_TIME_SINCE( mydata->timeLastFailedPathFind) > SEC_TO_ABS(2) )
        {
            DoorEntry* door = dbFindReachableLocalMapObj(e, 0, 10000);
            if( door )
            {
                mydata->timeOfLastDoorFind = ABS_TIME;
                ai->disappearInDoor = door;
                aiDiscardFollowTarget(e, "Going to run into some door and disappear now.", false);
                aiSetFollowTargetDoor(e, ai, door, AI_FOLLOWTARGETREASON_ENTERABLE);
                aiConstructPathToTarget(e);
                aiStartPath(e, 0, 0);
            }
            else if( ABS_TIME_SINCE( mydata->timeOfLastDoorFind) > SEC_TO_ABS(MAX_RUN_INTO_DOOR_TIME) || ENTTYPE(e) == ENTTYPE_NPC)
            {
                // fix for critters not managing to find a reachable door for a 60 seconds, or NPCs not managing to find one immediately
                behavior->finished = 1;
                e->fledMap = 1; //Nobody killed me.  I just ran off
                SET_ENTINFO(e).kill_me_please = 1;
            }
            else
            {
                mydata->timeLastFailedPathFind = ABS_TIME;
            }
        }
    }
}

void aiBFRunIntoDoorSet(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDRunIntoDoor* mydata = (AIBDRunIntoDoor*)behavior->data;
    mydata->timeLastFailedPathFind = 0;
    mydata->timeOfLastDoorFind = ABS_TIME;
}

//////////////////////////////////////////////////////////////////////////
// RunOutOfDoor
//////////////////////////////////////////////////////////////////////////
void aiBFRunOutOfDoorOffTick(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDRunOutOfDoor* mydata = (AIBDRunOutOfDoor*)behavior->data;
    AIVars* ai = ENTAI(e);

    if(ai->teamMemberInfo.runOutTime < ABS_TIME)
    {
        SET_ENTHIDE(e) = 0;
        e->draw_update = 1;

        e->motion->falling = 0;
        ai->base->behaviorRequestOffTickProcessing = 0;
        ai->teamMemberInfo.waiting = 0;
        ai->doNotMove = 0;
        if (ai->teamMemberInfo.myDoor && ai->teamMemberInfo.myDoor->numauxpoints)
        {
            EntRunOutOfDoor(e, ai->teamMemberInfo.myDoor, 0);
        }
        ai->base->behaviorRequestOffTickProcessing = 0;
        mydata->state = AIB_RUN_OUT_WAITING_FOR_ANIMATION;
        //        behavior->finished = 1;
    }
}

void aiBFRunOutOfDoorRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDRunOutOfDoor* mydata = (AIBDRunOutOfDoor*)behavior->data;
    AIVars* ai = ENTAI(e);
    AITeamMemberInfo* member = &ai->teamMemberInfo;

    if(member->waiting && mydata->state >= AIB_RUN_OUT_WAITING_FOR_TEAM)
    {
        mydata->state = AIB_RUN_OUT_WAITING_FOR_MY_TIME;

        if(member->runOutTime < ABS_TIME)
            aiBFRunOutOfDoorOffTick(e, aibase, behavior);
        else
            ai->base->behaviorRequestOffTickProcessing = 1;
    }
    else if(mydata->state == AIB_RUN_OUT_WAITING_FOR_ANIMATION && !(getMoveFlags(e->seq, SEQMOVE_WALKTHROUGH)))
        behavior->finished = 1;
}

void aiBFRunOutOfDoorSet(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDRunOutOfDoor* mydata = (AIBDRunOutOfDoor*)behavior->data;
    AIVars* ai = ENTAI(e);

    behavior->uninterruptable = 1;

    if(!ai->teamMemberInfo.team)
        aiCritterCheckProximity(e, 0);

    if(!devassertmsg(ai->teamMemberInfo.team, "Critter did not have a team in behavior function"))
    {
        EntRunOutOfDoor(e, DoorAnimFindPoint(ENTPOS(e), DoorAnimPointExit, MAX_DOOR_SEARCH_DIST), 0);
        behavior->finished = 1;
        return;
    }

    ai->teamMemberInfo.wantToRunOutOfDoor = 1;
    ai->teamMemberInfo.team->teamMemberWantsToRunOut = 1;
    SET_ENTHIDE(e) = 1;
    e->draw_update = 1;
    e->noPhysics = 1;
    ai->doNotMove = 1;
    mydata->state = AIB_RUN_OUT_WAITING_FOR_TEAM;
}

void aiBFRunOutOfDoorStart(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
//    behavior->uninterruptable = 1;
}

//////////////////////////////////////////////////////////////////////////
// RunThroughDoor
//////////////////////////////////////////////////////////////////////////
#define JUSTGETTHERE() mydata->tries > 1

void aiBFRunThroughDoorRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDRunThroughDoor* mydata = behavior->data;
    AIVars* ai = ENTAI(e);
    Entity* leader = erGetEnt(mydata->leaderRef);
    if(ai->followTarget.followTargetReached)
    {
        if(!leader || !leader->pl || !leader->pl->door_anim)
            behavior->finished = 1;

        return;
    }

    if(ai->followTarget.followTargetDiscarded || !devassert(ai->followTarget.type == AI_FOLLOWTARGET_ENTERABLE))
        aiSetFollowTargetDoor(e, ai, mydata->doorIn, AI_FOLLOWTARGETREASON_LEADER);
    
    if(!ai->navSearch->path.head && ai->motion.type == AI_MOTIONTYPE_NONE)
    {
        if(JUSTGETTHERE())
        {
            NavPathWaypoint* wp;

            // Create a path through the door.

            clearNavSearch(ai->navSearch);

            // Door.

            wp = createNavPathWaypoint();
            wp->connectType = NAVPATH_CONNECT_WIRE;
            copyVec3(mydata->doorIn->mat[3], wp->pos);
            navPathAddTail(&ai->navSearch->path, wp);

            ai->navSearch->path.curWaypoint = ai->navSearch->path.head;
        }
        else
            aiConstructPathToTarget(e);

        if(mydata->doorIn)
        {
            NavPathWaypoint* wp = createNavPathWaypoint();

            wp->door = mydata->doorIn;
            copyVec3(mydata->doorIn->mat[3], wp->pos);
            wp->connectType = NAVPATH_CONNECT_ENTERABLE;
            navPathAddTail(&ai->navSearch->path, wp);
        }
        if(mydata->doorOut)
        {
            NavPathWaypoint* wp = createNavPathWaypoint();

            wp->door = mydata->doorOut;
            copyVec3(ai->navSearch->path.tail->pos, wp->pos);
            wp->connectType = NAVPATH_CONNECT_ENTERABLE;
            navPathAddTail(&ai->navSearch->path, wp);
        }

        ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 1;
        aiStartPath(e, JUSTGETTHERE(), ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);
        mydata->tries++;
    }
}
#undef JUSTGETTHERE

void aiBFRunThroughDoorSet(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDRunThroughDoor* mydata = behavior->data;
    AIVars* ai = ENTAI(e);
    if(!mydata->doorIn || !mydata->doorOut)
    {
        behavior->finished = 1;
        return;
    }

    aiSetFollowTargetDoor(e, ai, mydata->doorIn, AI_FOLLOWTARGETREASON_LEADER);
}

int aiBFRunThroughDoorString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDRunThroughDoor* mydata = behavior->data;

    if(!eaSize(&param->params))
        return 0;

    if(!stricmp(param->function, "DoorIn"))
    {
        mydata->doorIn = (DoorEntry*)(intptr_t)atoll(param->params[0]->function);
        return 1;
    }
    if(!stricmp(param->function, "DoorOut"))
    {
        mydata->doorOut = (DoorEntry*)(intptr_t)atoll(param->params[0]->function);
        return 1;
    }
    if(!stricmp(param->function, "Leader"))
    {
        sscanf(param->params[0]->function, "%I64x", &mydata->leaderRef);
        return 1;
    }
    return 0;
}

void aiBFRunThroughDoorStart(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    behavior->uninterruptable = 1;
}

void aiBFRunThroughDoorFinish(Entity *e, AIVarsBase* aibase, AIBehavior* behavior)
{
    // if the behavior below this one is "Pet", set it to Follow
    int i = eaSize(&aibase->behaviors)-2;
    int handle = aiBehaviorGetTopBehaviorByName(e, aibase, "Pet");
    if(i>=0 && aibase->behaviors[i]->id == handle)
        aiBehaviorAddFlag(e, aibase, handle, "ActionFollow");
}

//////////////////////////////////////////////////////////////////////////
// SCOPEVAR
//////////////////////////////////////////////////////////////////////////
// This also uses permvar's flag function

void aiBFSCOPEVARRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    // SCOPEVARs are defined to just finish when they get executed
    behavior->finished = 1;
}

//////////////////////////////////////////////////////////////////////////
// Say
//////////////////////////////////////////////////////////////////////////
int aiBFSayFlag(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, AIBehaviorFlag* flag)
{
    AIBDSay* mydata = (AIBDSay*) behavior->data;
    if(flag->info->type == AIBF_PRIORITY)
    {
        mydata->priority = flag->intarray[0];
        return FLAG_PROCESSED;
    }
    return FLAG_NOT_PROCESSED;
}

void aiBFSayRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDSay* mydata = (AIBDSay*) behavior->data;

    if (e && mydata->estr)
    {
        Say(EntityNameFromEnt(e), mydata->estr, mydata->priority ? mydata->priority : 2);
    }

    behavior->finished = 1;
}

int aiBFSayString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDSay* mydata = (AIBDSay*) behavior->data;

    if(!mydata->estr && !eaSize(&param->params))
    {
        estrPrintCharString(&mydata->estr, param->function);
        return 1;
    }

    return 0;
}

void aiBFSayFinish(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDSay* mydata = (AIBDSay*) behavior->data;

    estrDestroy(&mydata->estr);
}

//////////////////////////////////////////////////////////////////////////
// SetHealth
//////////////////////////////////////////////////////////////////////////
void aiBFSetHealthRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDSetHealth* mydata = (AIBDSetHealth*) behavior->data;

    if (e && e->pchar)
    {
        F32 percentHealth = mydata->health;
        percentHealth = MAX( percentHealth, 0.0 );
        percentHealth = MIN( percentHealth, 1.0 );
                    
        e->pchar->attrCur.fHitPoints = percentHealth * e->pchar->attrMax.fHitPoints;
        character_HandleDeathAndResurrection(e->pchar, NULL);
    }

    behavior->finished = 1;
}

int aiBFSetHealthString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDSetHealth* mydata = (AIBDSetHealth*) behavior->data;

    if(!stricmp(param->function, "health") && eaSize(&param->params))
    {
        mydata->health = aiBehaviorParseFloat(param->params[0]);
        return 1;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////
// SetObjective
//////////////////////////////////////////////////////////////////////////
void aiBFSetObjectiveRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDSetObjective* mydata = (AIBDSetObjective*) behavior->data;
    
    if (mydata->estr && OnMissionMap())
    {
        SetMissionObjectiveComplete( mydata->failed ? OBJECTIVE_FAILED : OBJECTIVE_SUCCESS,  mydata->estr );
    }
    
    behavior->finished = 1;
}

int aiBFSetObjectiveString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDSetObjective* mydata = (AIBDSetObjective*) behavior->data;

    if(!stricmp(param->function, "success"))
    {
        return 1;
    }
    else if(!stricmp(param->function, "failed"))
    {
        mydata->failed = 1;
        return 1;
    }
    else if(!mydata->estr && !eaSize(&param->params))
    {
        estrPrintCharString(&mydata->estr, param->function);
        return 1;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////
// Test
//////////////////////////////////////////////////////////////////////////
void aiBFTestDebugStr(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, char** estr, char* indent)
{
}

void aiBFTestFinish(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
}

int aiBFTestFlag(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, AIBehaviorFlag* flag)
{
    return 0;
}

void aiBFTestOffTick(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
}

void aiBFTestRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
}

void aiBFTestSet(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
}

void aiBFTestStart(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
}

int aiBFTestString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    return 0;
}

void aiBFTestUnset(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
}

//////////////////////////////////////////////////////////////////////////
// ThankHero
//////////////////////////////////////////////////////////////////////////
void aiBFThankHeroRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{    
    AIBDThankHero* mydata = (AIBDThankHero*) behavior->data;
    AIVars* ai = ENTAI(e);
    Entity* savedMe = erGetEnt(mydata->hero);

    if(!savedMe){
        savedMe = EncounterWhoSavedMe(e);

        if(!savedMe){
            behavior->finished = 1;
            return;
        }

        mydata->hero = erGetRef(savedMe);
    }

    switch(mydata->state){
        case AIB_THANK_HERO_GO_TO_HERO:{
            // I'm trying to get to the hero.
            if(distance3Squared(ENTPOS(e), ENTPOS(savedMe)) < SQR(10)){
                aiDiscardFollowTarget(e, "Got to my hero, proceeding to thank.", false);

                mydata->state = AIB_THANK_HERO_THANK_HERO;
                mydata->storedTime= ABS_TIME;

                EncounterAISignal(e, ENCOUNTER_THANKHERO);

                //If the thank string has emotes, use those to time the thank length 
                if( e->IAmInEmoteChatMode )
                    mydata->thankStringHasEmotes = 1;

                break;
            }

            if(    !ai->motion.type ||
                ai->followTarget.type != AI_FOLLOWTARGET_ENTITY ||
                ai->followTarget.reason != AI_FOLLOWTARGETREASON_SAVIOR)
            {
                aiSetFollowTargetEntity(e, ai, savedMe, AI_FOLLOWTARGETREASON_SAVIOR);

                aiGetProxEntStatus(ai->proxEntStatusTable, savedMe, 1);

                aiConstructPathToTarget(e);

                if(!ai->navSearch->path.head){
                    behavior->finished = 1;
                    break;
                }else{
                    ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 10;
                    aiStartPath(e, 0, 0);
                }
            }

            break;
        }

        case AIB_THANK_HERO_THANK_HERO:{
            // I'm thanking the hero.

            Vec3 heroDir;

            subVec3(ENTPOS(savedMe), ENTPOS(e), heroDir);

            heroDir[1] = 0;

            aiTurnBodyByDirection(e, ai, heroDir);

            //If the thank you string had emotes in it, use the string's timing to decide when to stop thanking
            //Otherwise, just play the generic thank you for 5 seconds.
            if( mydata->thankStringHasEmotes )
            {
                if( !e->IAmInEmoteChatMode )
                    behavior->finished = 1;
            }
            else
            {
                SETB(e->seq->sticky_state, STATE_THANK);

                if(ABS_TIME_SINCE(mydata->storedTime) > SEC_TO_ABS(5)){
                    behavior->finished = 1;
                }
            }

            break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// UsePower
//////////////////////////////////////////////////////////////////////////
void aiBFUsePowerUnset(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDUsePower* mydata = (AIBDUsePower*) behavior->data;
    AIVars* ai = ENTAI(e);

    if (mydata->timesUsedIsSet && ai->power.current && (ai->power.current->usedCount > mydata->timesUsedAtSetPower))
        behavior->finished = 1;

    ai->wasToldToUseSpecialPetPower = 0;
    ai->power.lastStanceID = 0; //Stop using this power -- you're done
    ai->power.preferred = 0;
}

void aiBFUsePowerSet(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDUsePower* mydata = (AIBDUsePower*) behavior->data;
    AIVars* ai = ENTAI(e);

    if(!mydata->power)
    {
        behavior->finished = 1;
        return;
    }

    aiSetMessageHandler(e, aiCritterMsgHandlerCombat);

    if (!aiSetCurrentPower(e, mydata->power, 0, 1)) 
    {
        behavior->finished = 1;
        return;
    }
    
    ai->power.preferred = mydata->power;
    ai->doNotChangePowers = 1;

    if(!mydata->timesUsedIsSet)
    {
        mydata->timesUsedAtSetPower = ai->power.current->usedCount;
        mydata->timesUsedIsSet = 1;
    }
}

void aiBFUsePowerRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDUsePower* mydata = (AIBDUsePower*) behavior->data;
    AIVars* ai = ENTAI(e);
    Entity *target = NULL;
    int targetNeeded = 1;

    if(!mydata->power)
    {
        behavior->finished = 1;        //    This should already be set when the power is being set, but just in case
        return;
    }

    if (e && e->pchar && e->pchar->ppowCurrent && e->pchar->ppowCurrent->chainsIntoPower)
    {
        return;
    }

    if(mydata->power->isToggle)
    {
        aiSetTogglePower(e, mydata->power, mydata->toggleOff ? 0 : 1);
        behavior->finished = 1;
        return;
    }

    //    Find out if I need a target
    if (mydata->power && mydata->power->power && mydata->power->power->ppowBase)
    {
        int targetType = mydata->power->power->ppowBase->eTargetType;
        switch (targetType)
        {
        case kTargetType_Caster:
            target = e;
            targetNeeded = 0;
            break;
        default:
            break;
        }
    }

    if (targetNeeded)
    {
        //    Try to find a target through the combat system
        aiCritterActivityCombat(e, ai);
    }
    else if (ai->power.current)
    {
        aiUsePower(e, ai->power.current, target);
    }

    if( !ai->power.current || ai->power.current->usedCount > mydata->timesUsedAtSetPower )
        behavior->finished = 1;
}

void aiBFUsePowerStart(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDUsePower* mydata = (AIBDUsePower*) behavior->data;
    AIVars* ai = ENTAI(e);

    behavior->overrideExempt = 1;
    ai->wasToldToUseSpecialPetPower = 1; 
}

int aiBFUsePowerString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDUsePower* mydata = behavior->data;
    AIVars* ai = ENTAI(e);
    AIPowerInfo* power = aiCritterFindPowerByName(ai, param->function);

    if(!stricmp(param->function, "on"))
        return 1;
    if(!stricmp(param->function, "off"))
    {
        mydata->toggleOff = 1;
        return 1;
    }

    if(!power && !g_TestingBehaviorAliases)
        return 0;

    mydata->power = power;
    return 1;
}

//////////////////////////////////////////////////////////////////////////
// Wander
//////////////////////////////////////////////////////////////////////////
void aiBFWanderAroundDebugStr(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, char** estr, char* indent)
{
    AIBDWanderAround* mydata = (AIBDWanderAround*) behavior->data;
    estrConcatf(estr, "Radius: ^4%.1f^0", mydata->radius);
}

static Entity* simplePathEnt;
static int simplePathMaxDistanceSQR;
static Vec3 simplePathMaxDistanceAnchorPos;

static int beaconScoreWander(Beacon* src, BeaconConnection* conn){
    float* pos;

    if(conn){
        pos = posPoint(conn->destBeacon);
    }else{
        pos = posPoint(src);
    }

    if(ENTAI(simplePathEnt)->pathLimited)
    {
        if(conn && !conn->destBeacon->pathLimitedAllowed || !src->pathLimitedAllowed)
            return 0;
    }

    if(simplePathMaxDistanceSQR){
        float distanceSQR = distance3Squared(simplePathMaxDistanceAnchorPos, pos);
        if(distanceSQR > simplePathMaxDistanceSQR){
            return 0;
        }
    }

    if(distance3Squared(pos, ENTPOS(simplePathEnt)) > SQR(200)){
        return BEACONSCORE_FINISHED;
    }

    return rand();
}

void* aiCritterActivityWanderMessageHandler(Entity* e, AIMessage msg, AIMessageParams params);

void aiBFWanderAroundRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDWanderAround* mydata = (AIBDWanderAround*) behavior->data;
    AIVars* ai = ENTAI(e);

    // If the wandering timer has expired, then see if I should wander again.

    if(    !ai->navSearch->path.head && 
        ABS_TIME_SINCE(ai->time.foundPath) > SEC_TO_ABS(5)) 
    {
        F32* anchorPos = ai->pet.useStayPos ? ai->pet.stayPos : ai->spawn.pos;
        simplePathEnt = e;
        simplePathMaxDistanceSQR = SQR(mydata->radius);
        copyVec3(anchorPos, simplePathMaxDistanceAnchorPos);

        aiConstructSimplePath(e, beaconScoreWander);

        if(!ai->navSearch->path.head && simplePathMaxDistanceSQR){
            // Couldn't make a simple path, so go back to source position if possible.
            aiSetFollowTargetPosition(e, ai, anchorPos, 0);
            aiConstructPathToTarget(e);
        }

        // Check if both types of path failed.

        if(!ai->navSearch->path.head){
            ai->time.foundPath = ABS_TIME;

            behavior->finished = 1;
        }else{
            ai->time.wander.begin = ABS_TIME;

            aiStartPath(e, 1, 1);
            ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 10;
        }
    }

    if(!ai->navSearch->path.head || ABS_TIME_SINCE(ai->time.wander.begin) > SEC_TO_ABS(15)){
        // If the end of the path has been reached, then see what the entity wants to do.

        AI_LOG(    AI_LOG_PATH,
            (e,
            "Wandering path finished.\n"));

        aiDiscardFollowTarget(e, "Finished wandering.", false);
    }
}

void aiBFWanderAroundSet(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    aiSetMessageHandler(e, aiCritterActivityWanderMessageHandler);
}

int aiBFWanderAroundString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDWanderAround* mydata = (AIBDWanderAround*) behavior->data;

    if(!eaSize(&param->params))
        return 0;

    if(!stricmp(param->function, "Radius"))
    {
        mydata->radius = aiBehaviorParseFloat(param->params[0]);

        return 1;
    }

    return 0;
}

void* aiCritterActivityWanderMessageHandler(Entity* e, AIMessage msg, AIMessageParams params){
    AIVars* ai = ENTAI(e);

    switch(msg){
        case AI_MSG_PATH_BLOCKED:{
            if(ai->preventShortcut){
                aiPathUpdateBlockedWaypoint(ai->navSearch->path.curWaypoint);

                if(ai->attackTarget.entity){
                    // If I'm stuck and short-circuiting is prevented, then just hang out.

                    AI_LOG(    AI_LOG_PATH,
                        (e,
                        "Hanging out until my target moves.\n"));

                    aiDiscardFollowTarget(e, "Waiting for target to move.", false);

                    ai->waitingForTargetToMove = 1;
                }else{
                    // The target isn't a player, so I'm probably just wandering around.
                    //   I'll just find a new path eventually.

                    AI_LOG(    AI_LOG_PATH,
                        (e,
                        "Got stuck while not targeting a player.\n"));

                    aiDiscardFollowTarget(e, "Ran into something, not sure what.", false);

                    ai->waitingForTargetToMove = 0;
                }
            }else{
                // If I'm stuck and I have already tried short-circuiting,
                //   then redo the path but don't allow short-circuiting.

                AI_LOG(    AI_LOG_PATH,
                    (e,
                    "I got stuck, trying not short-circuiting.\n"));

                aiConstructPathToTarget(e);

                aiStartPath(e, 1, ai->attackTarget.entity ? 0 : 1);

                ai->waitingForTargetToMove = 1;
            }

            break;
                                 }

        case AI_MSG_ATTACK_TARGET_MOVED:{
            // The target has moved, so reset the find path timer.

            AI_LOG(    AI_LOG_PATH,
                (e,
                "Attack target moved a lot, cancelling path.\n"));

            clearNavSearch(ai->navSearch);

            ai->waitingForTargetToMove = 0;

            break;
                                        }

        case AI_MSG_FOLLOW_TARGET_REACHED:{
            aiDiscardFollowTarget(e, "Reached follow target.", true);

            ai->time.checkedForShortcut = 0;

            break;
                                          }
    }

    return 0;
}
