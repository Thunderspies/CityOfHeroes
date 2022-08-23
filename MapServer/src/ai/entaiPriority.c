
#include <utilitieslib/stdtypes.h>
#include "ailib/aiBehaviorPublic.h"
#include "beacon/beacon.h"
#include "beacon/beaconPath.h"
#include "entity/character_base.h"
#include "cmdparse/cmdserver.h"
#include "gameSys/dooranim.h"
#include "generator/encounter.h"
#include "generator/encounterPrivate.h"
#include "entai.h"
#include "entaiBehaviorCoh.h"
#include "entaiBehaviorStruct.h"
#include "entaiCritterPrivate.h"
#include "entaiLog.h"
#include "entaiprivate.h"
#include "entaiPriority.h"
#include "entaiPriorityPrivate.h"
#include "entity/entity.h"
#include "entity/entserver.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/StringCache.h>        // for stringToReference
#include "entity/motion.h"
#include "entity/netfx.h"
#include "entity/powers.h"
#include "seq/seq.h"
#include "seq/seqstate.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/timing.h>

static TokenizerParseInfo parseAction[] = {
    { "{",                    TOK_START,        0                                        },
    { "Type:",                TOK_INT(AIPriorityAction,type,AI_PRIORITY_ACTION_NOT_SET) },
    { "DoNotRun:",            TOK_INT(AIPriorityAction,doNotRun,-1) },
    { "AnimBits:",            TOK_UNPARSED(AIPriorityAction,animBitNames) },
    { "Radius:",            TOK_F32(AIPriorityAction,radius,0)        },
    { "TargetType:",        TOK_INT(AIPriorityAction,targetType,0)    },
    { "TargetName:",        TOK_STRING(AIPriorityAction,targetName,0)    },
    { "FXFile:",            TOK_FILENAME(AIPriorityAction,fxFile,0)        },
    { "PowerName:",            TOK_STRING(AIPriorityAction,powerName,0)    },
    { "}",                    TOK_END,            0                                        },
    { "", 0, 0 }
};

static TokenizerParseInfo parseActionLine[] = {
    { "",                    TOK_STRUCTPARAM |
                            TOK_INT(AIPriorityAction,type,0)            },
    { "",                    TOK_INT(AIPriorityAction,doNotRun,-1) },
    { "\n",                    TOK_END,            0                                        },
    { "", 0, 0 }
};

static TokenizerParseInfo parseActionLine_SetPowerOn[] = {
    { "",                    TOK_STRUCTPARAM |
                            TOK_STRING(AIPriorityAction,powerName,0)    },
    { "",                    TOK_INT(AIPriorityAction,type,AI_PRIORITY_ACTION_SET_POWER_ON) },
    { "",                    TOK_INT(AIPriorityAction,doNotRun,-1) },
    { "\n",                    TOK_END,            0                                        },
    { "", 0, 0 }
};

static TokenizerParseInfo parseActionLine_SetPowerOff[] = {
    { "",                    TOK_STRUCTPARAM | TOK_STRING(AIPriorityAction,powerName,0)    },
    { "",                    TOK_INT(AIPriorityAction,type,AI_PRIORITY_ACTION_SET_POWER_OFF) },
    { "",                    TOK_INT(AIPriorityAction,doNotRun,-1) },
    { "\n",                    TOK_END,            0                                        },
    { "", 0, 0 }
};

static TokenizerParseInfo parseCondition[] = {
    { "{",                    TOK_START,        0                                                    },
    //{ "",                    TOK_INTSTRUCT,    offsetof(AIPriorityCondition,simple)                },
    { "",                    TOK_INT(AIPriorityCondition,complx.not,0)},
    { "Operator:",            TOK_INT(AIPriorityCondition,complx.op,AI_PRIORITY_CONDITION_OP_NOT_SET) },
    { "Param1:",            TOK_STRING(AIPriorityCondition,complx.param1,0)        },
    { "Param2:",            TOK_STRING(AIPriorityCondition,complx.param2,0)        },
    { "}",                    TOK_END,            0                                                    },
    { "", 0, 0 }
};

static TokenizerParseInfo parseNotCondition[] = {
    { "{",                    TOK_START,        0                                                    },
    { "",                    TOK_INT(AIPriorityCondition,complx.not,1)},
    //{ "",                    TOK_INTSTRUCT,    offsetof(AIPriorityCondition,simple)                },
    { "Operator:",            TOK_INT(AIPriorityCondition,complx.op,AI_PRIORITY_CONDITION_OP_NOT_SET) },
    { "Param1:",            TOK_STRING(AIPriorityCondition,complx.param1,0),        },
    { "Param2:",            TOK_STRING(AIPriorityCondition,complx.param2,0),        },
    { "}",                    TOK_END,            0                                                    },
    { "", 0, 0 }
};

static TokenizerParseInfo parseConditionLine[] = {
    //{ "",                    TOK_INTSTRUCT |
    //                        TOK_STRUCTPARAM,    offsetof(AIPriorityCondition,simple)                },
    { "\n",                    TOK_END,            0                                                    },
    { "", 0, 0 }
};

static TokenizerParseInfo parseEvent[] = {
    { "",                    TOK_STRUCTPARAM | TOK_STRING(AIPriorityEvent,name,0)            },
    { "{",                    TOK_START,            0                                        },
    { "AtTime:",            TOK_F32(AIPriorityEvent,atTime,-1) },
    //----------------------------
    //{ "Condition:",            TOK_STRUCT |
    //                        TOK_REDUNDANTNAME,offsetof(AIPriorityEvent,conditions),    sizeof(AIPriorityCondition),    parseConditionLine },
    { "Condition",            TOK_REDUNDANTNAME |
                            TOK_STRUCT(AIPriorityEvent,conditions,parseCondition) },
    { "NotCondition",        TOK_STRUCT(AIPriorityEvent,conditions,parseNotCondition) },
    //----------------------------
//    { "Set:",                TOK_INTSTRUCT,    offsetof(AIPriorityEvent,sets)            },
    // Actions--------------------
    { "Action:",            TOK_REDUNDANTNAME |
                            TOK_STRUCT(AIPriorityEvent,actions,parseActionLine) },
    { "Action",                TOK_STRUCT(AIPriorityEvent,actions,parseAction) },
    { "Action.SetPowerOn:",    TOK_REDUNDANTNAME |
                            TOK_STRUCT(AIPriorityEvent,actions,parseActionLine_SetPowerOn) },
    { "Action.SetPowerOff:",TOK_REDUNDANTNAME |
                            TOK_STRUCT(AIPriorityEvent,actions,parseActionLine_SetPowerOff) },
    // ElseActions----------------
    { "ElseAction:",        TOK_REDUNDANTNAME |
                            TOK_STRUCT(AIPriorityEvent,elseActions,parseActionLine) },
    { "ElseAction",            TOK_STRUCT(AIPriorityEvent,elseActions,parseAction) },
    { "ElseAction.SetPowerOn:",    TOK_REDUNDANTNAME |
                            TOK_STRUCT(AIPriorityEvent,elseActions,parseActionLine_SetPowerOn) },
    { "ElseAction.SetPowerOff:",TOK_REDUNDANTNAME |
                            TOK_STRUCT(AIPriorityEvent,elseActions,parseActionLine_SetPowerOff) },
    //----------------------------
    { "}",                    TOK_END,            0                                        },
    { "", 0, 0 }
};

static TokenizerParseInfo parsePriority[] = {
    { "",                    TOK_STRUCTPARAM | TOK_STRING(AIPriority,name,0)                },
    { "{",                    TOK_START,        0                                        },
    //---------------------------
    { "Action",                TOK_STRUCT(AIPriority,actions,parseAction) },
    { "Action:",            TOK_REDUNDANTNAME | TOK_STRUCT(AIPriority,actions,parseActionLine) },
    //---------------------------
    { "Condition:",            TOK_REDUNDANTNAME |
                            TOK_STRUCT(AIPriority,conditions,parseConditionLine) },
    { "Condition",            TOK_REDUNDANTNAME |
                            TOK_STRUCT(AIPriority,conditions,parseCondition) },
    { "NotCondition",        TOK_STRUCT(AIPriority,conditions,parseNotCondition) },
    //{ "ExitCondition:",        TOK_INTSTRUCT,    offsetof(AIPriority,exitConditions),    },
    //---------------------------
    { "HoldPriority:",        TOK_INT(AIPriority,holdPriority,0)        },
    //{ "Set:",                TOK_INTSTRUCT,    offsetof(AIPriority,sets)                },
    { "IAmA:",                TOK_UNPARSED(AIPriority,IAmA) },
    { "IReactTo:",            TOK_UNPARSED(AIPriority,IReactTo) },
    { "Event",                TOK_STRUCT(AIPriority,events,parseEvent) },
    { "}",                    TOK_END,            0                                         },
    { "", 0, 0 }
};

static TokenizerParseInfo parsePriorityList[] = {
    { "",                    TOK_STRUCTPARAM | TOK_STRING(AIPriorityList,name,0)            },
    { "Priority",            TOK_STRUCT(AIPriorityList,priorities,parsePriority) },
    { "{",                    TOK_START,        0                                        },
    { "}",                    TOK_END,            0                                        },
    { "", 0, 0 }
};

static TokenizerParseInfo parseAllPriorityLists[] = {
    { "PriorityList",        TOK_STRUCT(AIAllPriorityLists,lists,parsePriorityList) },
    { "", 0, 0 }
};

static DefineIntList defineMapping[] = {
    { "true",                            AI_PRIORITY_TRUE                            },
    { "false",                            AI_PRIORITY_FALSE                            },

    // Set.

    { "=",                                SETOPERATOR_EQUAL                            },

    // Comparison.

    { "==",                                CONDCOMP_EQUAL                                },
    { "!=",                                CONDCOMP_NOTEQUAL                            },
    { "<",                                CONDCOMP_LESS                                },
    { "<=",                                CONDCOMP_LESS_EQUAL                            },
    { ">",                                CONDCOMP_GREATER                            },
    { ">=",                                CONDCOMP_GREATER_EQUAL                        },

    // Condition Variables.

    { "me.attackerCount",                CONDVAR_ME_ATTACKER_COUNT                    },
    { "me.distanceToSavior",            CONDVAR_ME_DISTANCE_TO_SAVIOR                },
    { "me.distanceToTarget",            CONDVAR_ME_DISTANCE_TO_TARGET                },
    { "me.endurancePercent",            CONDVAR_ME_ENDURANCE_PERCENT                },
    { "me.hasAttackTarget",                CONDVAR_ME_HAS_ATTACK_TARGET                },
    { "me.hasFollowTarget",                CONDVAR_ME_HAS_FOLLOW_TARGET                },
    { "me.hasReachedTarget",            CONDVAR_ME_HAS_REACHED_TARGET                },
    { "me.healthPercent",                CONDVAR_ME_HEALTH_PERCENT                    },
    { "me.isLeader",                    CONDVAR_ME_IS_LEADER                        },
    { "me.outOfDoor",                    CONDVAR_ME_OUT_OF_DOOR                        },
    { "me.timeSinceCombat",                CONDVAR_ME_TIME_SINCE_COMBAT                },
    { "me.timeSinceWasAttacked",        CONDVAR_ME_TIME_SINCE_WAS_ATTACKED            },

    { "team.leaderHasReachedTarget",    CONDVAR_TEAM_LEADER_HAS_REACHED_TARGET        },
    { "team.timeSinceWasAttacked",        CONDVAR_TEAM_TIME_SINCE_WAS_ATTACKED        },

    { "global.time24",                    CONDVAR_GLOBAL_TIME24                        },

    { "random.percent",                    CONDVAR_RANDOM_PERCENT                        },
    
    // Condition Variables that can be used with "Set:".

    { "list.hold",                        CONDVAR_LIST_HOLD                            },

    { "me.alwaysGetHit",                CONDVAR_ME_ALWAYS_GET_HIT                    },
    { "me.destroy",                        CONDVAR_ME_DESTROY                            },
    { "me.doNotDrawOnClient",            CONDVAR_ME_DO_NOT_DRAW_ON_CLIENT            },
    { "me.doNotFaceTarget",                CONDVAR_ME_DO_NOT_FACE_TARGET                },
    { "me.doNotGoToSleep",                CONDVAR_ME_DO_NOT_GO_TO_SLEEP                 },
    { "me.doNotMove",                    CONDVAR_ME_DO_NOT_MOVE                        },


    { "me.feeling.bothered",            CONDVAR_ME_FEELING_BOTHERED                    },
    { "me.feeling.confidence",            CONDVAR_ME_FEELING_CONFIDENCE                },
    { "me.feeling.fear",                CONDVAR_ME_FEELING_FEAR                        },
    { "me.feeling.loyalty",                CONDVAR_ME_FEELING_LOYALTY                    },

    { "me.findTarget",                    CONDVAR_ME_FIND_TARGET                        },
    { "me.flying",                        CONDVAR_ME_FLYING                            },
    { "me.hasPatrolPoint",                CONDVAR_ME_HAS_PATROL_POINT                    },
    { "me.invincible",                    CONDVAR_ME_INVINCIBLE                        },
    { "me.invisible",                    CONDVAR_ME_INVISIBLE                        },
    { "me.limitedSpeed",                CONDVAR_ME_LIMITED_SPEED                    },
    { "me.onTeamHero",                    CONDVAR_ME_ON_TEAM_HERO                     },
    { "me.onTeamMonster",                CONDVAR_ME_ON_TEAM_MONSTER                     },
    { "me.onTeamVillain",                CONDVAR_ME_ON_TEAM_VILLAIN                     },
    { "me.onTeamGood",                    CONDVAR_ME_ON_TEAM_HERO                     },
    { "me.onTeamEvil",                    CONDVAR_ME_ON_TEAM_MONSTER                     },
    { "me.onTeamFoul",                    CONDVAR_ME_ON_TEAM_VILLAIN                     },
    { "me.overridePerceptionRadius",    CONDVAR_ME_OVERRIDE_PERCEPTION_RADIUS        },
    { "me.spawnPosIsSelf",                CONDVAR_ME_SPAWN_POS_IS_SELF                },
    { "me.targetInstigator",            CONDVAR_ME_TARGET_INSTIGATOR                },
    { "me.untargetable",                CONDVAR_ME_UNTARGETABLE                     },

    // Condition Operators.
    
    { "opHasTeammateNearby",            AI_PRIORITY_CONDITION_OP_HAS_TEAMMATE_NEARBY},

    // Actions.

    { "actionAddRandomAnimBit",            AI_PRIORITY_ACTION_ADD_RANDOM_ANIM_BIT            },
    { "actionAlertTeam",                AI_PRIORITY_ACTION_ALERT_TEAM                    },
    { "actionCallForHelp",                AI_PRIORITY_ACTION_CALLFORHELP                    },
    { "actionChoosePatrolPoint",        AI_PRIORITY_ACTION_CHOOSE_PATROL_POINT            },
    { "actionCombat",                    AI_PRIORITY_ACTION_COMBAT                         },
    { "actionDisablePriority",            AI_PRIORITY_ACTION_DISABLE_PRIORITY             },
    { "actionDoAnim",                    AI_PRIORITY_ACTION_DO_ANIM                        },
    { "actionExitCombat",                AI_PRIORITY_ACTION_EXIT_COMBAT                     },
    { "actionFollowLeader",                AI_PRIORITY_ACTION_FOLLOW_LEADER                },
    { "actionFollowRoute",                AI_PRIORITY_ACTION_FOLLOW_ROUTE                    },
    { "actionFrozenInPlace",            AI_PRIORITY_ACTION_FROZEN_IN_PLACE                 },
    { "actionHideBehindEnt",            AI_PRIORITY_ACTION_HIDE_BEHIND_ENT                },
    { "actionNextPriority",                AI_PRIORITY_ACTION_NEXT_PRIORITY                 },
    { "actionNeverForgetAttackTarget",    AI_PRIORITY_ACTION_NEVERFORGETTARGET            },
    { "actionNPC.Hostage",                AI_PRIORITY_ACTION_NPC_HOSTAGE                     },
    { "actionNPC.ThankHero",            AI_PRIORITY_ACTION_THANK_HERO                     },
    { "actionPatrol",                    AI_PRIORITY_ACTION_PATROL                         },
    { "actionPlayFX",                    AI_PRIORITY_ACTION_PLAY_FX                         },
    { "actionRunAway",                    AI_PRIORITY_ACTION_RUN_AWAY                     },
    { "actionRunIntoDoor",                AI_PRIORITY_ACTION_RUN_INTO_DOOR                },
    { "actionRunOutOfDoor",                AI_PRIORITY_ACTION_RUN_OUT_OF_DOOR                },
    { "actionRunToTarget",                AI_PRIORITY_ACTION_RUN_TO_TARGET                 },
    { "actionScriptControlledActivity",    AI_PRIORITY_ACTION_SCRIPT_CONTROLLED_ACTIVITY    },
    { "actionStandStill",                AI_PRIORITY_ACTION_STAND_STILL                     },
    { "actionSetPowerOn",                AI_PRIORITY_ACTION_SET_POWER_ON                    },
    { "actionSetPowerOff",                AI_PRIORITY_ACTION_SET_POWER_OFF                },
    { "actionSetTeammateAnimBits",        AI_PRIORITY_ACTION_SET_TEAMMATE_ANIM_BITS        },
    { "actionUseRandomPower",            AI_PRIORITY_ACTION_USE_RANDOM_POWER                },
    { "actionWander",                    AI_PRIORITY_ACTION_WANDER                         },

    // Targets.

    { "targetFarthestBeacon",            AI_PRIORITY_TARGET_FARTHEST_BEACON             },
    { "targetNearbyCritters",            AI_PRIORITY_TARGET_NEARBY_CRITTERS             },
    { "targetSpawnPoint",                AI_PRIORITY_TARGET_SPAWN_POINT                },
    { "targetWire",                        AI_PRIORITY_TARGET_WIRE                        },

    // End the list.

    { 0, 0 }
};

StaticDefineInt PriorityFlags[] = {
    DEFINE_INT
    { "Fly",                            AI_PL_FLAG_FLY                                },
    { "Run",                            AI_PL_FLAG_RUN                                },
    { "Walk",                            AI_PL_FLAG_WALK                                },
    { "Wire",                            AI_PL_FLAG_WIRE                                },
    { "DoorEntryOnly",                    AI_PL_FLAG_DOORENTRY_ONLY                    },
    DEFINE_END
};

static AIAllPriorityLists        allPriorityLists;
static StashTable                nameToPriorityListTable = 0;
static int                        priorityListHadAnError;
static int                        priorityListHadAnAnimBitError;

#define VALUE_BETWEEN(x,lo,hi) ((x) > (lo) && (x) < (hi))
#define VALUE_IN_SET(x,set) ((x) > (set##_LOW) && (x) < (set##_HIGH))
#define PRINT_SET(set) aiPriorityPrintSet(set##_LOW, set##_HIGH)

static void printPriorityErrorHeader(){
    if(!priorityListHadAnError){
        priorityListHadAnError = 1;

        printf("\n******************* BEGIN PRIORITY LIST ERRORS *******************\n\n");
    }
}

static void printPriorityErrorFooter(int forceReload){
    if(priorityListHadAnError){
        if(priorityListHadAnAnimBitError){
            int i;

            printf("    Available Anim Bits:\n");
            for(i = 0; i < g_MaxUsedStates ; i++){
                if( stateBits[i].name )
                    printf("      %s\n", stateBits[i].name);
            }
        }

        printf("\n******************** END PRIORITY LIST ERRORS ********************\n\n");
    }
    else if(forceReload){
        printf("Priority lists: NO ERRORS.\n\n");
    }
}

static const char* getDefineString(int value){
    int i;
    for(i = 0; i < ARRAY_SIZE(defineMapping); i++){
        if(defineMapping[i].value == value){
            return defineMapping[i].key;
        }
    }

    return "UnknownDefine";
}

static void aiPriorityPrintSet(int lo, int hi){
    int i;
    printf("    Available values:\n");
    for(i = 0; i < ARRAY_SIZE(defineMapping); i++){
        if(VALUE_BETWEEN(defineMapping[i].value,lo,hi)){
            printf("      %s\n", defineMapping[i].key);
        }
    }
}

static void aiPriorityValidateAction(const char* name, AIPriorityAction* a){
    int i;

    if(!VALUE_IN_SET(a->type, AI_PRIORITY_ACTION)){
        printPriorityErrorHeader();
        printf("  ERROR: %s: Invalid type.\n", name);
        PRINT_SET(AI_PRIORITY_ACTION);
        a->type = AI_PRIORITY_ACTION_NOT_SET;
    }

    // Check the doNotRun flag.

    if(a->doNotRun < -1 || a->doNotRun > 1){
        printPriorityErrorHeader();
        printf("  ERROR: %s: Invalid value for DoNotRun.\n", name);
        PRINT_SET(AI_PRIORITY_BOOL);
        a->doNotRun = -1;
    }else{
        //printf("%d\t%s\n", a->doNotRun, name);
    }

    // Count the number of anim bits that were entered.

    a->animBitCount = 0;

    for(i = 0; i < eaSize(&a->animBitNames); i++){
        TokenizerParams* tokenizerParams = a->animBitNames[i];
        a->animBitCount += eaSize(&tokenizerParams->params);
    }

    // Check all those anim bits.

    if(a->animBitCount){
        int j = 0;

        a->animBits = calloc(a->animBitCount, sizeof(*a->animBits));

        for(i = 0; i < eaSize(&a->animBitNames); i++){
            TokenizerParams* tokenizerParams = a->animBitNames[i];
            char** params = tokenizerParams->params;
            int k;

            for(k = 0; k < eaSize(&params); k++){
                a->animBits[j] = seqGetStateNumberFromName(params[k]);

                if(a->animBits[j] < 0){
                    printPriorityErrorHeader();
                    priorityListHadAnAnimBitError = 1;
                    printf(    "  ERROR: %s: Invalid anim bit \"%s\" for %s.\n",
                            name,
                            params[k],
                            getDefineString(a->type));
                }

                j++;
            }
        }
    }

    switch(a->type){
        case AI_PRIORITY_ACTION_DO_ANIM:{
            if(!a->animBitCount){
                printPriorityErrorHeader();
                printf(    "  ERROR: %s: No anim bits specified for %s.\n",
                        name,
                        getDefineString(a->type));
            }
            break;
        }

        case AI_PRIORITY_ACTION_RUN_TO_TARGET:{
            if(!VALUE_IN_SET(a->targetType, AI_PRIORITY_TARGET)){
                printPriorityErrorHeader();
                printf(    "  ERROR: %s: Invalid target for %s.\n",
                        name,
                        getDefineString(a->type));
                PRINT_SET(AI_PRIORITY_TARGET);
            }
            break;
        }

        case AI_PRIORITY_ACTION_PLAY_FX:{
            if(!a->fxFile || !a->fxFile[0]){
                printPriorityErrorHeader();
                printf(    "  ERROR: %s: No FXFile specified for %s.\n",
                        name,
                        getDefineString(a->type));
            }
            break;
        }

        case AI_PRIORITY_ACTION_SET_POWER_ON:
        case AI_PRIORITY_ACTION_SET_POWER_OFF:{
            if(!a->powerName || !a->powerName[0]){
                printPriorityErrorHeader();
                printf(    "  ERROR: %s: No PowerName specified for %s.\n",
                        name,
                        getDefineString(a->type));
            }
            break;
        }
        
        case AI_PRIORITY_ACTION_SET_TEAMMATE_ANIM_BITS:{
            if(!a->animBitCount){
                printPriorityErrorHeader();
                printf(    "  ERROR: %s: No anim bits specified for %s.\n",
                        name,
                        getDefineString(a->type));
            }
            
            if(!a->targetName || !a->targetName[0]){
                printPriorityErrorHeader();
                printf(    "  ERROR: %s: No TargetName specified for %s.\n",
                        name,
                        getDefineString(a->type));
            }
            break;
        }
    }
}

static void aiPriorityValidateSet(const char* name, int* s){
    int count = eaiSize(&s);

    if(count != 3){
        printPriorityErrorHeader();
        printf("  ERROR: %s: Set takes 3 parameters.\n", name);
    }

    if(count >= 3){
        if(!VALUE_IN_SET(s[0], CONDVAR_SET)){
            printPriorityErrorHeader();
            printf("  ERROR: %s: Bad variable.\n", name);
            PRINT_SET(CONDVAR_SET);
        }else{
            int rvalue = s[2];

            switch(s[0]){
                case CONDVAR_LIST_HOLD:
                case CONDVAR_ME_ALWAYS_GET_HIT:
                case CONDVAR_ME_DESTROY:
                case CONDVAR_ME_DO_NOT_DRAW_ON_CLIENT:
                case CONDVAR_ME_DO_NOT_FACE_TARGET:
                case CONDVAR_ME_DO_NOT_GO_TO_SLEEP:
                case CONDVAR_ME_DO_NOT_MOVE:
                case CONDVAR_ME_FIND_TARGET:
                case CONDVAR_ME_FLYING:
                case CONDVAR_ME_HAS_PATROL_POINT:
                case CONDVAR_ME_INVINCIBLE:
                case CONDVAR_ME_INVISIBLE:
                case CONDVAR_ME_ON_TEAM_HERO:
                case CONDVAR_ME_ON_TEAM_MONSTER:
                case CONDVAR_ME_ON_TEAM_VILLAIN:
                case CONDVAR_ME_SPAWN_POS_IS_SELF:
                case CONDVAR_ME_TARGET_INSTIGATOR:
                case CONDVAR_ME_UNTARGETABLE:
                {
                    if(count >= 3 && rvalue && rvalue != 1){
                        printPriorityErrorHeader();
                        printf(    "  ERROR: %s: %s is a boolean.\n",
                                name,
                                getDefineString(s[0]));
                        s[0] = 0;
                    }
                    break;
                }

                case CONDVAR_ME_FEELING_FEAR:
                case CONDVAR_ME_FEELING_CONFIDENCE:
                case CONDVAR_ME_FEELING_LOYALTY:
                case CONDVAR_ME_FEELING_BOTHERED:
                {
                    if(count < 3 || rvalue < 0 || rvalue > 100){
                        printPriorityErrorHeader();
                        printf(    "  ERROR: %s: %s is a percent [0-100].\n",
                                name,
                                getDefineString(s[0]));
                        s[0] = 0;
                    }
                    break;
                }
                case CONDVAR_ME_OVERRIDE_PERCEPTION_RADIUS:
                case CONDVAR_ME_LIMITED_SPEED:
                    if(count < 3 || rvalue < 0)
                    {
                        printPriorityErrorHeader();
                        printf( "  ERROR: %s: %s is a scalar, so cannot be negative.\n",
                                name,
                                getDefineString(s[0]));
                        s[0] = 0;
                    }
                    break;
            }
        }

        if(s[1] != SETOPERATOR_EQUAL){
            printPriorityErrorHeader();
            printf("  ERROR: %s: Operator must be =.\n", name);
            s[0] = 0;
        }
    }
}

static void aiPriorityValidateSets(const char* name, int** sets){
    int i;
    char buffer[1000];

    for(i = 0; i < eaSize(&sets); i++){
        sprintf(buffer, "%s: Set %d", name, i);

        aiPriorityValidateSet(buffer, sets[i]);
    }
}

static void aiPriorityValidateCondition(const char* name, AIPriorityCondition* c){
    if(c->simple){
        int* tokens = *c->simple;
        int count = eaiSize(c->simple);

        if(count != 1 && count != 3){
            printPriorityErrorHeader();
            printf("  ERROR: %s: Conditions take 1 or 3 parameters.\n", name);
        }

        if(count >= 1){
            if(!VALUE_IN_SET(tokens[0], CONDVAR) || tokens[0] == CONDVAR_SET_LOW || tokens[0] == CONDVAR_SET_HIGH){
                printPriorityErrorHeader();
                printf("  ERROR: %s: Bad variable.\n", name);
                PRINT_SET(CONDVAR);
            }else{
                switch(tokens[0]){
                    case CONDVAR_ME_HAS_ATTACK_TARGET:{
                        if(count >= 3){
                            int rvalue = tokens[2];
                            if(rvalue && rvalue != 1){
                                printPriorityErrorHeader();
                                printf(    "  ERROR: %s: Variable %s is a boolean.\n",
                                        name,
                                        getDefineString(tokens[0]));
                            }
                        }
                        break;
                    }
                }
            }

            if(count >= 2 && !VALUE_IN_SET(tokens[1], CONDCOMP)){
                printPriorityErrorHeader();
                printf(    "  ERROR: %s: Bad operator.\n",
                        name);
                PRINT_SET(CONDCOMP);
            }
        }
    }else{
        if(!VALUE_IN_SET(c->complx.op, AI_PRIORITY_CONDITION_OP)){
            printPriorityErrorHeader();
            printf("  ERROR: %s: Bad condition operator.\n", name);
            PRINT_SET(AI_PRIORITY_CONDITION_OP);
        }
    }
}

static void aiPriorityValidateEvent(const char* name, AIPriorityEvent* ev){
    int i;
    char buffer[1000];

    // Check name.

    if(!ev->name){
        printPriorityErrorHeader();
        printf("  ERROR: %s: No name.\n", name);
    }

    // Check Conditions.

    for(i = 0; i < eaSize(&ev->conditions); i++){
        sprintf(buffer, "%s: Condition %d", name, i);
        aiPriorityValidateCondition(buffer, ev->conditions[i]);
    }

    // Check Actions.

    for(i = 0; i < eaSize(&ev->actions); i++){
        sprintf(buffer, "%s: Action %d", name, i);
        aiPriorityValidateAction(buffer, ev->actions[i]);
    }

    // Check Sets.

    aiPriorityValidateSets(name, ev->sets);
}

static void aiPriorityValidatePriority(const char* name, AIPriority* p){
    int i;
    char buffer[1000];

    // Check name.

    if(!p->name){
        printPriorityErrorHeader();
        printf("  ERROR: %s: No name.\n", name);
    }

    // Check Actions.

    for(i = 0; i < eaSize(&p->actions); i++){
        sprintf(buffer, "%s: Action %d", name, i);
        aiPriorityValidateAction(buffer, p->actions[i]);
    }

    // Check Sets.

    aiPriorityValidateSets(name, p->sets);

    // Check Events.

    for(i = 0; i < eaSize(&p->events); i++){
        AIPriorityEvent* ev = p->events[i];
        sprintf(buffer, "%s: Event %d (%s)", name, i, ev->name);
        aiPriorityValidateEvent(buffer, ev);
    }

    // Check Conditions.

    for(i = 0; i < eaSize(&p->conditions); i++){
        sprintf(buffer, "%s: Condition %d", name, i);
        aiPriorityValidateCondition(buffer, p->conditions[i]);
    }

    //// Check ExitConditions.

    //for(i = 0; i < eaSize(&p->exitConditions); i++){
    //    sprintf(buffer, "%s: ExitCondition %d", name, i);
    //    aiPriorityValidateCondition(buffer, p->exitConditions[i]);
    //}
}

static void aiPriorityValidateList(const char* name, AIPriorityList* l){
    int i;
    char buffer[1000];

    // Check name.

    if(!l->name){
        printPriorityErrorHeader();
        printf("  ERROR: %s: No name.\n", name);
    }

    // Check Priorities.

    for(i = 0; i < eaSize(&l->priorities); i++){
        AIPriority* p = l->priorities[i];
        sprintf(buffer, "%s: Priority %d (%s)", name, i, p->name);
        aiPriorityValidatePriority(buffer, p);
    }
}

static int aiSetPriorityList(AIPriorityManager* manager, const char* listName);
AIPriorityList* aiGetPriorityListByName(const char* name);

static int comparePriorityListNames(const AIPriorityList** l1, const AIPriorityList** l2){
    if(!(*l1)->name)
        return -1;

    if(!(*l2)->name)
        return 1;

    return stricmp((*l1)->name, (*l2)->name);
}

int preprocessPriorityLists(TokenizerParseInfo pti[], void* structptr, int forceReload)
{
    int i;

    eaQSort(allPriorityLists.lists, comparePriorityListNames);

    if(forceReload){
        printf("\n%d priority lists found:\n", eaSize(&allPriorityLists.lists));

        for(i = 0; i < eaSize(&allPriorityLists.lists); i++){
            printf("  %s\n", allPriorityLists.lists[i]->name);
        }
    }

    for(i = 0; i < eaSize(&allPriorityLists.lists); i++){
        char buffer[1000];
        sprintf(buffer, "List %d (%s)", i, allPriorityLists.lists[i]->name);
        aiPriorityValidateList(buffer, allPriorityLists.lists[i]);
    }

    return !priorityListHadAnError;
}

void aiPriorityLoadFiles(int forceReload){
    int i;

    if(forceReload){
        // MS: This function leaks memory.  Guess who doesn't care.  That's right, it's me.
        //     It's just a debugging function anyway.

        printf("Reloading priorities...\n");

        ZeroStruct(&allPriorityLists);
    }

    if(!allPriorityLists.lists){
        // Create the defines object.

        DefineContext *defines = DefineCreateFromIntList(defineMapping);

        // Parse the priority list files.

        ParserLoadFiles("AIScript/priorities",
                        ".pri",
                        "priorities.bin",
                        (forceReload?PARSER_FORCEREBUILD:0) | PARSER_SERVERONLY,
                        parseAllPriorityLists,
                        &allPriorityLists,
                        defines, NULL, NULL);

        // Destroy the defines object.

        DefineDestroy(defines);

        // Verify that the priorities are correctly formatted.

        priorityListHadAnError = 0;
        priorityListHadAnAnimBitError = 0;

        preprocessPriorityLists(parseAllPriorityLists, &allPriorityLists, forceReload);

        // Create the hash table to lookup priority lists by name.

        nameToPriorityListTable = stashTableCreateWithStringKeys(10,  StashDeepCopyKeys);

        for(i = 0; i < eaSize(&allPriorityLists.lists); i++){
            StashElement element;
            stashFindElement(nameToPriorityListTable, allPriorityLists.lists[i]->name, &element);

            if(element){
                printPriorityErrorHeader();
                printf("  ERROR: Duplicate priority list name: %s\n", allPriorityLists.lists[i]->name);
            }else{
                stashAddPointer(nameToPriorityListTable, allPriorityLists.lists[i]->name, allPriorityLists.lists[i], false);
            }
        }

        printPriorityErrorFooter(forceReload);
    }

    if(forceReload){
        for(i = 0; i < entities_max; i++){
            Entity* e = entities[i];
            AIPriorityManager* manager;

            if(entInUse(i) && e && ENTAI(e) && (manager = aiBehaviorGetPriorityManager(e, 0, 0))){
                if(manager && manager->list){
                    manager->list = aiGetPriorityListByName(manager->list->name);

                    aiPriorityRestartList(manager);
                }
            }
        }

        printf("Done reloading priorities.\n");
    }
}

/*
void aiPriorityInit(Entity* e){
    if(!ENTAI(e)){
        return;
    }

    aiPriorityDestroy(ENTAI(e)->priorityManager);

    ENTAI(e)->priorityManager = calloc(1, sizeof(*ENTAI(e)->priorityManager));
}
*/

MP_DEFINE(AIPriorityManager);
AIPriorityManager* aiPriorityManagerCreate()
{
    MP_CREATE(AIPriorityManager, 100);
    return MP_ALLOC(AIPriorityManager);
}

void aiPriorityDestroy(AIPriorityManager* manager){
    if(!manager){
        return;
    }

    if(manager->stateArray){
        free(manager->stateArray);
    }

    eaiDestroy(&manager->flags);
    eaiDestroy(&manager->nextFlags);

    MP_FREE(AIPriorityManager, manager);
}

AIPriorityList* aiGetPriorityListByName(const char* name){
    StashElement element;

    if(!nameToPriorityListTable)
        return NULL;

    stashFindElement(nameToPriorityListTable, name, &element);

    if(element){
        return stashElementGetPointer(element);
    }else{
        return NULL;
    }
}

static int aiSetPriorityList(AIPriorityManager* manager, const char* listName){
    manager->nextList = aiGetPriorityListByName(listName);
    eaiCreate(&manager->nextFlags); 

    return manager->nextList || !listName || !listName[0] || (stricmp(listName, "NONE") == 0) ? 1 : 0;
}

void aiPriorityRestartList(AIPriorityManager* manager){
    if(!manager)
        return;

    manager->priority = PRIORITY_NOT_SET;
    manager->nextPriority = PRIORITY_NOT_SET;
    eaiDestroy(&manager->flags);
    eaiDestroy(&manager->nextFlags);
    manager->stateArrayIsValid = 0;
    manager->priorityTime = 0;
    manager->holdCurrentList = 0;
}

void aiPriorityQueuePriorityList(Entity* e, const char* listName, int forceStart, AIPriorityManager* man){
    AIPriorityManager* manager = man;
    static char* estr = NULL;

    if(e && ENTAI(e)){
        if(!manager)
        {
            estrPrintf(&estr, "PriorityList(%s)", listName);
            aiBehaviorAddString(e, ENTAI(e)->base, estr);
            return;
        }

        if(aiSetPriorityList(manager, listName)){ 
            AI_LOG(    AI_LOG_PRIORITY,
                (e,
                "Setting priority list: ^3%s\n",
                listName && listName[0] ? listName : "NONE"));
            AI_LOG(    AI_LOG_BEHAVIOR,
                (e,
                "Setting priority list: ^3%s\n",
                listName && listName[0] ? listName : "NONE"));

            if(forceStart){
                aiPriorityRestartList(manager);

                if(!manager->nextList)
                    manager->list = NULL;
            }
//            if(!man)
//                aiBehaviorRunPLNow(e);
        }else{
            AI_LOG(    AI_LOG_PRIORITY,
                    (e,
                    "^1FAILED ^0to set priority list: ^3%s\n",
                    listName));

            Errorf(    "SpawnDef: %s\n  %s doesn't exist.\n",
                    e->encounterInfo ? e->encounterInfo->parentGroup->active->spawndef->fileName : "NONE",
                    listName);
        }
    }
}

int aiPriorityAddFlag(Entity* e, char* flag_str)
{
    AIPriorityManager* manager;

    if(e && ENTAI(e) && (manager = aiBehaviorGetPriorityManager(e, 0, 0)) && flag_str)
    {
        int flag;
        AIVars *ai = ENTAI(e);

        flag = StaticDefineIntGetInt(PriorityFlags, flag_str);

        switch(flag)
        {
        xcase AI_PL_FLAG_FLY:
            AI_LOG(AI_LOG_PRIORITY, (e, "Setting flag: Fly"));
            aiSetFlying(e, ai, 1);
        xcase AI_PL_FLAG_RUN:
            AI_LOG(AI_LOG_PRIORITY, (e, "Setting flag: Run"));
            ai->inputs.doNotRunIsSet = 1;
            ai->inputs.doNotRun = ai->doNotRun = 0;
        xcase AI_PL_FLAG_WALK:
            AI_LOG(AI_LOG_PRIORITY, (e, "Setting flag: Walk"));
            ai->inputs.doNotRunIsSet = 1;
            ai->inputs.doNotRun = ai->doNotRun = 1;
        xcase AI_PL_FLAG_WIRE:
            AI_LOG(AI_LOG_PRIORITY, (e, "Setting flag: Wire"));
            ai->teamMemberInfo.team->patrolTypeWire = 1;
            ai->pitchWhenTurning = 1;
        xcase AI_PL_FLAG_DOORENTRY_ONLY:
            AI_LOG(AI_LOG_PRIORITY, (e, "Setting flag: DoorEntryOnly"));
            ai->doorEntryOnly = 1;
        xcase AI_PL_FLAG_UNKNOWN:
            AI_LOG(AI_LOG_PRIORITY, (e, "Unknown flag: %s", flag_str));
            Errorf("Unknown flag: %s", flag_str);
            return 0;
        xdefault:
            AI_LOG(AI_LOG_PRIORITY, (e, "Putting %s in the flags list", flag_str));
            if(manager->nextFlags)
                eaiPush(&manager->nextFlags, flag);
            else
                eaiPush(&manager->flags, flag);
        }
        return 1;
    }
    return 0;
}

static int aiPriorityCompareInt(int a, int op, int b){
    switch(op){
        case CONDCOMP_EQUAL:
            return a == b;
        case CONDCOMP_NOTEQUAL:
            return a != b;
        case CONDCOMP_GREATER:
            return a > b;
        case CONDCOMP_GREATER_EQUAL:
            return a >= b;
        case CONDCOMP_LESS:
            return a < b;
        case CONDCOMP_LESS_EQUAL:
            return a <= b;
        default:
            return 0;
    }
}

static int aiPriorityEvaluateSimpleCondition(Entity* e, int* tokens){
    AIVars* ai;
    int count = eaiSize(&tokens);

    if(count >= 1){
        int lvalue;
        int rvalue;

        if(count >= 3){
            rvalue = tokens[2];
        }else{
            rvalue = 0;
        }

        switch(tokens[0]){
            // "me." variables.

            case CONDVAR_ME_ATTACKER_COUNT:
                ai = ENTAI(e);
                lvalue = ai->attackerList.count;
                break;

            case CONDVAR_ME_DISTANCE_TO_SAVIOR:{
                Entity* savior = EncounterWhoSavedMe(e);

                if(!savior){
                    lvalue = INT_MAX;
                }else{
                    lvalue = distance3Squared(ENTPOS(e), ENTPOS(savior));
                    rvalue *= rvalue;
                }
                break;
            }

            case CONDVAR_ME_DISTANCE_TO_TARGET:{
                AIVars * ai = ENTAI(e);
                if(ai->followTarget.type){
                    lvalue = distance3(ENTPOS(e), ai->followTarget.pos);
                }else{
                    lvalue = 0;
                }
                break;
            }

            case CONDVAR_ME_ENDURANCE_PERCENT:
                lvalue = e->pchar && e->pchar->attrMax.fEndurance ? (e->pchar->attrCur.fEndurance* 100) / e->pchar->attrMax.fEndurance : 100;
                break;

            case CONDVAR_ME_HEALTH_PERCENT:
                lvalue = e->pchar && e->pchar->attrMax.fHitPoints ? (e->pchar->attrCur.fHitPoints * 100) / e->pchar->attrMax.fHitPoints : 100;
                break;

            case CONDVAR_ME_HAS_ATTACK_TARGET:
                ai = ENTAI(e);
                lvalue = ai->attackTarget.entity ? 1 : 0;
                break;

            case CONDVAR_ME_HAS_REACHED_TARGET:
                ai = ENTAI(e);
                if(ai->followTarget.type){
                    lvalue = (SQR(3) >= distance3Squared(ENTPOS(e), ai->followTarget.pos));
                }else{
                    lvalue = 0;
                }
                break;

            case CONDVAR_ME_HAS_FOLLOW_TARGET:{
                lvalue = ENTAI(e)->followTarget.type != AI_FOLLOWTARGET_NONE;
                break;
            }

            case CONDVAR_ME_HAS_PATROL_POINT:{
                lvalue = ENTAI(e)->hasNextPatrolPoint;
                break;
            }

            case CONDVAR_ME_IS_LEADER:{
                lvalue = e == aiTeamGetLeader(ENTAI(e));
                break;
            }

            case CONDVAR_ME_OUT_OF_DOOR:{
                lvalue = !(ENTAI(e)->teamMemberInfo.wantToRunOutOfDoor || getMoveFlags(e->seq, SEQMOVE_WALKTHROUGH)) && !ENTAI(e)->teamMemberInfo.waiting;
                break;
            }

            case CONDVAR_ME_TIME_SINCE_COMBAT:{
                // Determine amount of time since most recent combat-related action.

                U32 timeSince;

                ai = ENTAI(e);
                timeSince = ABS_TIME_SINCE(ai->time.friendWasAttacked);

                if(timeSince > ABS_TIME_SINCE(ai->attackTarget.time.set))
                    timeSince = ABS_TIME_SINCE(ai->attackTarget.time.set);

                if(timeSince > ABS_TIME_SINCE(ai->time.didAttack))
                    timeSince = ABS_TIME_SINCE(ai->time.didAttack);

                lvalue = ABS_TO_SEC(timeSince);
                break;
            }
            
            case CONDVAR_ME_TIME_SINCE_WAS_ATTACKED:{
                ai = ENTAI(e);
                lvalue = ABS_TO_SEC(ABS_TIME_SINCE(ai->time.wasAttacked));
                break;
            }

            // TODO: Check all targets, not just beacons
            case CONDVAR_TEAM_LEADER_HAS_REACHED_TARGET:{
                Entity* leader = aiTeamGetLeader(ENTAI(e));
                lvalue = 0;
                if(leader)
                {
                    AIVars* ai = ENTAI(leader);
                    if(    ai &&
                        ai->followTarget.type == AI_FOLLOWTARGET_BEACON &&
                        ai->followTarget.beacon)
                    {
                        lvalue = (SQR(ai->anchorRadius) >= distance3Squared(ENTPOS(leader), posPoint(ai->followTarget.beacon)));
                    }
                }
                break;

            case CONDVAR_TEAM_TIME_SINCE_WAS_ATTACKED:
                {
                    ai = ENTAI(e);
                    lvalue = ABS_TO_SEC(MIN(ABS_TIME_SINCE(ai->time.wasAttacked),ABS_TIME_SINCE(ai->time.friendWasAttacked)));
                    break;
                }
            }

            // "global." variables.

            case CONDVAR_GLOBAL_TIME24:
                lvalue = server_visible_state.time * 100;
                break;

            // "random." variables.

            case CONDVAR_RANDOM_PERCENT:
                lvalue = getCappedRandom(101);
                break;

            // default: unfound variable, return false comparison.

            default:
                //printf("  ERROR: Condition var not found: %d\n", tokens[0]);
                return 0;
        }

        if(count >= 3){
            return aiPriorityCompareInt(lvalue, tokens[1], rvalue);
        }else{
            return lvalue ? 1 : 0;
        }
    }
    
    return 0;
}

static int aiPriorityEvaluateComplexCondition(Entity* e, AIPriorityCondition* c){
    switch(c->complx.op){
        case AI_PRIORITY_CONDITION_OP_HAS_TEAMMATE_NEARBY:{
            if(c->complx.param1 && c->complx.param2){
                AIVars* ai = ENTAI(e);
                
                if(ai->teamMemberInfo.team){
                    F32 maxDist = simpleatof(c->complx.param2);
                    F32 maxDistSQR = SQR(maxDist);
                    AITeamMemberInfo* member;
                    
                    for(member = ai->teamMemberInfo.team->members.list; member; member = member->next){
                        Entity* entMember = member->entity;
                        if(entMember != e){
                            F32 distSQR = distance3Squared(ENTPOS(e), ENTPOS(entMember));
                            
                            if(distSQR <= maxDistSQR){
                                if(entMember->villainDef && !stricmp(entMember->villainDef->name, c->complx.param1)){
                                    // Found one!
                                    
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }else{
                return 0;
            }
            break;
        }
    }
    
    return 0;
}

static int aiPriorityEvaluateCondition(Entity* e, AIPriorityCondition* c){
    if(c->simple){
        return aiPriorityEvaluateSimpleCondition(e, *c->simple);
    }else{
        int eval = aiPriorityEvaluateComplexCondition(e, c);
        
        return c->complx.not ? !eval : eval;
    }
    
    return 0;
}

static int aiPriorityEvaluateConditions(Entity* e, AIPriorityCondition** conditions){
    int i;
    int count = eaSize(&conditions);

    for(i = 0; i < count; i++){
        if(!aiPriorityEvaluateCondition(e, conditions[i])){
            // All conditions must be met in order for priority to be accepted.

            return 0;
        }
    }

    return 1;
}

static AIPriorityState* aiPriorityGetStates(AIPriorityManager* manager, int create){
    if(create && manager->list && !manager->stateArrayIsValid){
        int count = eaSize(&manager->list->priorities);

        if(!manager->stateArray || count > manager->stateArraySize){
            if(manager->stateArray){
                free(manager->stateArray);
            }

            manager->stateArray = calloc(count, sizeof(*manager->stateArray));

            manager->stateArraySize = count;
        }else{
            ZeroMemory(manager->stateArray, sizeof(*manager->stateArray) * manager->stateArraySize);
        }

        manager->stateArrayIsValid = 1;
    }

    return manager->stateArrayIsValid ? manager->stateArray : NULL;
}

static void aiPriorityDoActions(Entity* e,
                                AIPriorityManager* manager,
                                AIPriorityUpdateParams* params,
                                AIPriorityAction** actions,
                                AIPriorityDoActionParams* outParams)
{
    int count = eaSize(&actions);

    if(count){
        int i;
        for(i = 0; i < count; i++){
            AIPriorityAction* action = actions[i];

            switch(action->type){
                case AI_PRIORITY_ACTION_DISABLE_PRIORITY:{
                    AIPriorityState* states = aiPriorityGetStates(manager, 1);

                    if(states){
                        states[manager->priority].disabled = 1;
                    }

                    break;
                }

                case AI_PRIORITY_ACTION_PLAY_FX:{
                    if(action->fxFile && action->fxFile[0]){
                        NetFx nfx = {0};

                        nfx.command = CREATE_ONESHOT_FX; //CREATE_MAINTAINED_FX

                        nfx.handle = stringToReference(action->fxFile);

                        if(nfx.handle){
                            entAddFx(e, &nfx);
                        }
                    }

                    break;
                }

                case AI_PRIORITY_ACTION_NEXT_PRIORITY:{
                    manager->nextPriority = manager->priority + 1;

                    if(manager->nextPriority >= eaSize(&manager->list->priorities)){
                        manager->nextPriority = PRIORITY_NOT_SET;
                    }

                    break;
                }

                default:{
                    params->doActionFunc(e, action, outParams);

                    outParams->switchedPriority = 0;

                    break;
                }
            }
        }
    }
    else if(outParams->switchedPriority){
        // There's no actions, so send the switched message if necessary.

        params->doActionFunc(e, NULL, outParams);

        outParams->switchedPriority = 0;
    }
}

static void aiPriorityDoSets(Entity* e, AIVars* ai, AIPriorityManager* manager, int** sets)
{
    int setCount = eaSize(&sets);
    int i;
    static char* buf = NULL;
    static char* buf2 = NULL;
    AIBehavior* behavior = manager->myBehavior;
    AIBDPriorityList* mydata = behavior ? behavior->data : 0;

    assert(behavior);
    
    estrClear(&buf2);

    for(i = 0; i < setCount; i++){
        int* s = sets[i];
        int rvalue = s[2];

        if(eaiSize(&s) == 3){
            switch(s[0]){
                case CONDVAR_ME_ALWAYS_GET_HIT:{
                    if(!behavior || !mydata->sets.AlwaysGetHit || mydata->values.AlwaysGetHit != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.alwaysGetHit to ^4%d\n", rvalue));
                        if(behavior)
                        {
                            estrConcatf(&buf, "AlwaysGetHit(%d)", rvalue);
                            mydata->sets.AlwaysGetHit = 1;
                            mydata->values.AlwaysGetHit = rvalue;
                        }
                        else
                            e->alwaysGetHit = rvalue;
                    }
                    break;
                }

                case CONDVAR_ME_DO_NOT_DRAW_ON_CLIENT:{
                    if(!behavior || !mydata->sets.Invisible || mydata->values.Invisible != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.doNotDrawOnClient to ^4%d\n", rvalue));
                        if(behavior)
                        {
                            estrConcatf(&buf, "Invisible(%d)", rvalue);
                            mydata->sets.Invisible = 1;
                            mydata->values.Invisible = rvalue;
                        }
                        else if(ENTHIDE(e) != rvalue){
                            SET_ENTHIDE(e) = rvalue;
                            e->draw_update = 1;
                        }
                    }
                    break;
                }
                
                case CONDVAR_ME_DO_NOT_FACE_TARGET:{
                    if(!behavior || !mydata->sets.DoNotFaceTarget || mydata->values.DoNotFaceTarget != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.doNotFaceTarget to ^4%d\n", rvalue));
                        if(behavior)
                        {
                            estrConcatf(&buf, "DoNotFaceTarget(%d)", rvalue);
                            mydata->sets.DoNotFaceTarget = 1;
                            mydata->values.DoNotFaceTarget = rvalue;
                        }
                        else
                            e->do_not_face_target = rvalue;
                    }
                    break;
                }

                case CONDVAR_ME_DO_NOT_GO_TO_SLEEP:{
                    if(!behavior || !mydata->sets.DoNotGoToSleep || mydata->values.DoNotGoToSleep != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.doNotGoToSleep = ^4%d\n", rvalue));
                        if(behavior)
                        {
                            estrConcatf(&buf, "DoNotGoToSleep(%d)", rvalue);
                            mydata->sets.DoNotGoToSleep = 1;
                            mydata->values.DoNotGoToSleep = rvalue;
                        }
                        else
                            ai->doNotGoToSleep = rvalue;
                    }
                    break;
                }

                case CONDVAR_ME_DO_NOT_MOVE:{
                    if(!behavior || !mydata->sets.DoNotMove || mydata->values.DoNotMove != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.doNotMove = ^4%d\n", rvalue));
                        if(behavior)
                        {
                            estrConcatf(&buf, "DoNotMove(%d)", rvalue);
                            mydata->sets.DoNotMove = 1;
                            mydata->values.DoNotMove = rvalue;
                        }
                        else
                            ai->doNotMove = rvalue;
                    }
                    break;
                }

                case CONDVAR_ME_FEELING_FEAR:{
                    if(!behavior || !mydata->sets.FeelingFear || mydata->values.FeelingFear != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.feeling.fear = ^4%d\n", rvalue));
                        if(behavior)
                        {
                            estrConcatf(&buf, "FeelingFear(%d)", rvalue);
                            mydata->sets.FeelingFear = 1;
                            mydata->values.FeelingFear = rvalue;
                        }
                        else
                            ai->feeling.sets[AI_FEELINGSET_BASE].level[AI_FEELING_FEAR] = (float)rvalue / 100.0;
                    }
                    break;
                }

                case CONDVAR_ME_FEELING_CONFIDENCE:{
                    if(!behavior || !mydata->sets.FeelingConfidence || mydata->values.FeelingConfidence != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.feeling.confidence = ^4%d\n", rvalue));
                        if(behavior)
                        {
                            estrConcatf(&buf, "FeelingConfidence(%d)", rvalue);
                            mydata->sets.FeelingConfidence = 1;
                            mydata->values.FeelingConfidence = rvalue;
                        }
                        else
                            ai->feeling.sets[AI_FEELINGSET_BASE].level[AI_FEELING_CONFIDENCE] = (float)rvalue / 100.0;
                    }
                    break;
                }

                case CONDVAR_ME_FEELING_LOYALTY:{
                    if(!behavior || !mydata->sets.FeelingLoyalty || mydata->values.FeelingLoyalty != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.feeling.loyalty = ^4%d\n", rvalue));
                        if(behavior)
                        {
                            estrConcatf(&buf, "FeelingLoyalty(%d)", rvalue);
                            mydata->sets.FeelingLoyalty = 1;
                            mydata->values.FeelingLoyalty = rvalue;
                        }
                        else
                            ai->feeling.sets[AI_FEELINGSET_BASE].level[AI_FEELING_LOYALTY] = (float)rvalue / 100.0;
                    }
                    else if(!behavior)
                    break;
                }

                case CONDVAR_ME_FEELING_BOTHERED:{
                    if(!behavior || !mydata->sets.FeelingBothered || mydata->values.FeelingBothered != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.feeling.bothered = ^4%d\n", rvalue));
                        if(behavior)
                        {
                            estrConcatf(&buf, "FeelingBothered(%d)", rvalue);
                            mydata->sets.FeelingBothered = 1;
                            mydata->values.FeelingBothered = rvalue;
                        }
                        else
                            ai->feeling.sets[AI_FEELINGSET_BASE].level[AI_FEELING_BOTHERED] = (float)rvalue / 100.0;
                    }
                    break;
                }

                case CONDVAR_ME_FIND_TARGET:{
                    if(!behavior || !mydata->sets.FindTarget || mydata->values.FindTarget != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.findTarget = ^4%d\n", rvalue));
                        if(behavior)
                        {
                            estrConcatf(&buf, "FindTarget(%d)", rvalue);
                            mydata->sets.FindTarget = 1;
                            mydata->values.FindTarget = rvalue;
                        }
                        else
                            ai->findTargetInProximity = rvalue;
                    }
                    break;
                }

                case CONDVAR_ME_FLYING:{
                    if(!behavior || !mydata->sets.Flying || mydata->values.Flying != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.flying = ^4%d\n", rvalue));
                        if(behavior)
                        {
                            estrConcatf(&buf, "Fly(%d)", rvalue);
                            mydata->sets.Flying = 1;
                            mydata->values.Flying = rvalue;
                        }
                        else
                            aiSetFlying(e, ai, rvalue);
                    }
                    break;
                }

                case CONDVAR_ME_INVINCIBLE:{
                    if(!behavior || !mydata->sets.Invincible || mydata->values.Invincible != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.invincible = ^4%d\n", rvalue));
                        if(behavior)
                        {
                            estrConcatf(&buf, "Invincible(%d)", rvalue);
                            mydata->sets.Invincible = 1;
                            mydata->values.Invincible = rvalue;
                        }
                        else
                            e->invincible = (e->invincible & ~INVINCIBLE_GENERAL)
                                    | (rvalue ? INVINCIBLE_GENERAL : 0);
                    }
                    break;
                }

                case CONDVAR_ME_INVISIBLE:{
                    if(!behavior || !mydata->sets.Invisible || mydata->values.Invisible != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.invisible = ^4%d\n", rvalue));
                        if(behavior)
                        {
                            estrConcatf(&buf, "Invisible(%d)", rvalue);
                            mydata->sets.Invisible = 1;
                            mydata->values.Invisible = rvalue;
                        }
                        else if(ENTHIDE(e) != rvalue){
                            SET_ENTHIDE(e) = rvalue;
                            e->draw_update = 1;
                        }
                    }
                    break;
                }

                case CONDVAR_ME_LIMITED_SPEED:
                {
                    if(!behavior || !mydata->sets.LimitedSpeed || mydata->values.LimitedSpeed != rvalue)
                    {
                        estrConcatf(&buf, "LimitedSpeed(%f)", (float)rvalue/10);
                        mydata->sets.LimitedSpeed = 1;
                        mydata->values.LimitedSpeed = rvalue;
                    }
                    break;
                }
                case CONDVAR_ME_OVERRIDE_PERCEPTION_RADIUS:
                    if(!behavior || !mydata->sets.OverridePerceptionRadius || mydata->values.OverridePerceptionRadius != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.overridePerceptionRadius = ^4%d\n", rvalue));
                        if(behavior)
                        {
                            estrConcatf(&buf, "OverridePerceptionRadius(%d)", rvalue);
                            mydata->sets.OverridePerceptionRadius = 1;
                            mydata->values.OverridePerceptionRadius = rvalue;
                        }
                        else
                            ai->overridePerceptionRadius = rvalue;
                    }
                    break;

                case CONDVAR_ME_SPAWN_POS_IS_SELF:{
                    if(!behavior || !mydata->sets.SpawnPosIsSelf || mydata->values.SpawnPosIsSelf != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.spawnPosIsSelf = ^4%d\n", rvalue));
                        if(behavior)
                        {
                            estrConcatf(&buf, "SpawnPosIsSelf(%d)", rvalue);
                            mydata->sets.SpawnPosIsSelf = 1;
                            mydata->values.SpawnPosIsSelf = rvalue;
                        }
                        else
                            ai->spawnPosIsSelf = rvalue;
                    }
                    break;
                }

                case CONDVAR_ME_TARGET_INSTIGATOR:{
                    if(!behavior || !mydata->sets.TargetInstigator || mydata->values.TargetInstigator != rvalue)
                    {
                        if(behavior)
                        {
                            estrConcatf(&buf, "TargetInstigator(%d)", rvalue);
                            mydata->sets.TargetInstigator = 1;
                            mydata->values.TargetInstigator = rvalue;
                        }
                        else
                        {
                            Entity* target = EncounterWhoInstigated(e);
                            if(target && ai->proxEntStatusTable){
                                AIProxEntStatus* status = aiGetProxEntStatus(ai->proxEntStatusTable, target, 1);

                                status->neverForgetMe = rvalue;
                            }
                        }
                    }
                    break;
                }

                case CONDVAR_ME_ON_TEAM_MONSTER:{
                    if(!behavior || !mydata->sets.TeamMonster || mydata->values.TeamMonster != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.onTeamMonster = ^4%d\n", rvalue));
                        if(behavior)
                        {
                            if(rvalue)
                            {
                                estrConcatStaticCharArray(&buf, "Team(Monster)");
                                mydata->sets.TeamMonster = 1;
                                mydata->values.TeamMonster = rvalue;
                            }
                        }
                        else
                            entSetOnTeamMonster(e, rvalue);
                    }
                    break;
                }

                case CONDVAR_ME_ON_TEAM_HERO:{
                    if(!behavior || !mydata->sets.TeamHero || mydata->values.TeamHero != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.onTeamHero = ^4%d\n", rvalue));
                        if(behavior)
                        {
                            if(rvalue)
                            {
                                estrConcatStaticCharArray(&buf, "Team(Hero)");
                                mydata->sets.TeamHero = 1;
                                mydata->values.TeamHero = rvalue;
                            }
                        }
                        else
                            entSetOnTeamHero(e, rvalue);
                    }
                    break;
                }

                case CONDVAR_ME_ON_TEAM_VILLAIN:{
                    if(!behavior || !mydata->sets.TeamVillain || mydata->values.TeamVillain != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.onTeamVillain = ^4%d\n", rvalue));
                        if(behavior)
                        {
                            if(rvalue)
                            {
                                estrConcatStaticCharArray(&buf, "Team(Villain)");
                                mydata->sets.TeamVillain = 1;
                                mydata->values.TeamVillain = rvalue;
                            }
                        }
                        else
                            entSetOnTeamVillain(e, rvalue);
                    }
                    break;
                }

                case CONDVAR_ME_UNTARGETABLE:{
                    if(!behavior || !mydata->sets.Untargetable || mydata->values.Untargetable != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.untargetable = ^4%d\n", rvalue));
                        if(behavior)
                        {
                            estrConcatf(&buf, "Untargetable(%d)", rvalue);
                            mydata->sets.Untargetable = 1;
                            mydata->values.Untargetable = rvalue;
                        }
                        else
                            e->untargetable = (e->untargetable & ~UNTARGETABLE_GENERAL)
                                        | (rvalue ? UNTARGETABLE_GENERAL : 0);
                    }
                    break;
                }

                case CONDVAR_ME_HAS_PATROL_POINT:{
                    if(!behavior || !mydata->sets.HasPatrolPoint || mydata->values.HasPatrolPoint != rvalue)
                    {
                        AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.hasPatrolPoint = ^4%d\n", rvalue));
                        if(behavior)
                        {
                            estrConcatf(&buf, "HasPatrolPoint(%d)", rvalue);
                            mydata->sets.HasPatrolPoint = 1;
                            mydata->values.HasPatrolPoint = rvalue;
                        }
                        else
                            ai->hasNextPatrolPoint = rvalue;
                    }
                    break;
                }

                case CONDVAR_LIST_HOLD:{
                    AI_LOG(AI_LOG_PRIORITY, (e, "Set: list.hold = ^4%d\n", rvalue));
                    manager->holdCurrentList = rvalue;
                    break;
                }
                
                case CONDVAR_ME_DESTROY:{
                    AI_LOG(AI_LOG_PRIORITY, (e, "Set: me.destroy to ^4%d\n", rvalue));
                    SET_ENTINFO(e).kill_me_please = rvalue;
                    break;
                }
            }
            if(buf && buf[0])
            {
                estrConcatf(&buf2, "%s,", buf);
                estrClear(&buf);
            }
        }
    }
    if(buf2 && buf2[0])
        aiBehaviorAddPLFlag(e, buf2);
}

void aiPriorityUpdate(Entity* e, AIVars* ai, AIPriorityManager* manager, AIPriorityUpdateParams* params){
    int i;
    int count;
    AIPriorityDoActionParams outParams;

    outParams.doAction = params->doAction;

    if(manager->nextList && (!manager->list || !manager->holdCurrentList)){
        manager->list = manager->nextList;
        manager->nextList = NULL;
        manager->flags = manager->nextFlags;
        manager->nextFlags = NULL;

        aiPriorityRestartList(manager);
    }

    if(manager && manager->list){
        float oldTime = manager->priorityTime;
        AIPriorityList* l = manager->list;

        manager->priorityTime += params->timePassed;

        count = eaSize(&l->priorities);

        if(manager->nextPriority < 0){
            for(i = 0; i < count; i++){
                AIPriority* p = l->priorities[i];
                AIPriorityState* states = aiPriorityGetStates(manager, 0);

                if(states && states[i].disabled)
                    continue;

                if(aiPriorityEvaluateConditions(e, p->conditions)){
                    manager->nextPriority = i;
                    break;
                }
            }
        }

        if(manager->nextPriority >= 0){
            AIPriority* p;
            int j;
            int eventCount;

            // Found my priority.

            outParams.firstTime = manager->priority != manager->nextPriority;
            outParams.switchedPriority = outParams.firstTime;

            manager->priority = manager->nextPriority;

            p = l->priorities[manager->priority];

            if(!p->holdPriority){
                manager->nextPriority = PRIORITY_NOT_SET;
            }

            // Do the actions.

            aiPriorityDoActions(e, manager, params, p->actions, &outParams);

            // Do the sets.

            aiPriorityDoSets(e, ai, manager, p->sets);

            // Check the events.

            if(outParams.firstTime){
                // Reset priority time.

                manager->priorityTime = 0;

                oldTime = -1;
            }

            // Check if any events should fire.

            eventCount = eaSize(&p->events);

            for(j = 0; j < eventCount; j++){
                AIPriorityEvent* ev = p->events[j];

                // NOTE: atTime must be greater than zero to be used. 
                // MW: That comment is a total lie.

                if(ev->atTime < 0 || (ev->atTime > oldTime && ev->atTime <= manager->priorityTime)){
                    // Check that the conditions are also true.

                    if(aiPriorityEvaluateConditions(e, ev->conditions)){
                        // Do the actions.

                        outParams.firstTime = 1;
                        outParams.switchedPriority = 0;

                        aiPriorityDoActions(e, manager, params, ev->actions, &outParams);

                        // Do the sets.

                        aiPriorityDoSets(e, ai, manager, ev->sets);
                    }
                    else if(ev->elseActions){
                        // Do the actions.

                        outParams.firstTime = 1;
                        outParams.switchedPriority = 0;

                        aiPriorityDoActions(e, manager, params, ev->elseActions, &outParams);
                    }
                }
            }

            // Finished processing this priority, so return.

            return;
        }
    }

    // Never found a valid priority, so just do the undefined thing.

    outParams.firstTime = manager->priority != PRIORITY_NONE;

    if(outParams.firstTime){
        manager->priority = PRIORITY_NONE;
        manager->priorityTime = 0;
    }

    outParams.switchedPriority = outParams.firstTime;

    params->doActionFunc(e, NULL, &outParams);
}

void aiPriorityUpdateGeneric(Entity* e, AIVars* ai, AIPriorityManager* manager, float timePassed, AIPriorityDoActionFunc doActionFunc, int doAction){
    AIPriorityUpdateParams params;

    params.timePassed = timePassed;
    params.doActionFunc = doActionFunc;
    params.doAction = doAction;

    aiPriorityUpdate(e, ai, manager, &params);
}


const char* aiGetPriorityName(AIPriorityManager* manager){
    if(manager && manager->list && manager->priority >= 0){
            return manager->list->priorities[manager->priority]->name;
    }

    return NULL;
}

char* aiGetEntityPriorityListName(Entity * e){
    if( e )
    {
        AIVars* ai = ENTAI(e);
        AIPriorityManager* manager;
        if(ai && (manager = aiBehaviorGetPriorityManager(e, 0, 0))) 
        {
            if( manager->nextList )
                return manager->nextList->name;
            if( manager->priority >= 0 )
                return manager->list->name;
        }
    }

    return NULL;
}

const char** aiGetIReactTo(Entity * e)
{
    return ENTAI(e)->IReactTo;
//     AIPriorityManager* manager;
//     if( !aiHasPriorityList(e) )
//         return NULL;
// 
//     manager = aiBehaviorGetPriorityManager(e, 0, 0);
// 
//     if( manager && manager->list && manager->priority >= 0)
//     {
//         TokenizerParams** tokenizerParams = manager->list->priorities[manager->priority]->IReactTo;
//         if( tokenizerParams && tokenizerParams[0] )
//             return tokenizerParams[0]->params;
//     }
//     return NULL;
}

const char** aiGetIAmA(Entity * e)
{
    return ENTAI(e)->IAmA;
//     AIPriorityManager* manager;
//     if( !aiHasPriorityList(e) )
//         return NULL;
// 
//     manager = aiBehaviorGetPriorityManager(e, 0, 0);
// 
//     if( manager && manager->list && manager->priority >= 0)
//     {
//         TokenizerParams** tokenizerParams = manager->list->priorities[manager->priority]->IAmA;
//         if( tokenizerParams && tokenizerParams[0] )
//             return tokenizerParams[0]->params;
//     }
//     return NULL;
}

float aiGetPriorityTime(AIPriorityManager* manager){
    if(manager){
        return manager->priorityTime;
    }

    return 0;
}

int aiGetPriorityListCount(){
    return eaSize(&allPriorityLists.lists);
}

const char* aiGetPriorityListNameByIndex(int i){
    if(i >= 0 && i < aiGetPriorityListCount()){
        return allPriorityLists.lists[i]->name;
    }

    return NULL;
}

bool aiHasPriorityList(Entity *e){
    AIVars* ai = ENTAI(e); 
    AIPriorityManager* manager;

    if(ai
        && (manager = aiBehaviorGetPriorityManager(e, 0, 0))) 
    {
        return 1;
    }

    return 0;
}

//The dialog can "call priority lists", but all it's really doing is setting
//aniamtion bits, and not setting a new priority list at all, we're just using priority lists
//so we don't have to maintain animations-to-bits in some new place, so
void setAnimationBasedOnPriorityList( Entity * e, const char * name )
{
    AIPriorityList* pl;
    AIPriorityAction * action;

    pl = aiGetPriorityListByName( name );

    if( pl && eaSize(&pl->priorities) && eaSize(&pl->priorities[0]->actions))
    {
        action = pl->priorities[0]->actions[0];
        if(action->animBitCount && action->type != AI_PRIORITY_ACTION_ADD_RANDOM_ANIM_BIT)
        {
            int i;
            for(i = 0; i < action->animBitCount; i++)
            {
                if(action->animBits[i] >= 0)
                {
                    SETB(e->seq->state, action->animBits[i]);
                    //SETB(e->seq->sticky_state, action->animBits[i]); //TO DO do I want this set for the time given?, add a sticky state release function...
                }
            }
        }
    }
    else
    {
        //not a valid PL list given for this guy
    }
}


void 
aiCritterPriorityDoActionFunc(    Entity* e,
                              const AIPriorityAction* action,
                              const AIPriorityDoActionParams* params)
{
    AIVars* ai = ENTAI(e);
    AIBehavior* behavior = aiBehaviorGetPLBehavior(e, 0);
    static char* behaviorbuf = NULL;

    if(!behavior)
        return;

    // Reset things if the priority switched.

    if(params->switchedPriority){
        AIBDPriorityList* mydata = behavior->data;

        seqClearState(e->seq->sticky_state); //TO DO: only clear priority set bits

        //aiSetConfigByName(e, ENTAI(e), ENTAI(e)->brain.configName);
        //e->untargetable = 0;

        if(e->noDrawOnClient){
            e->noDrawOnClient = 0;
            e->draw_update = 1;
        }

        if(ENTHIDE(e)){
            SET_ENTHIDE(e) = 0;
            e->draw_update = 1;
        }

        ai->findTargetInProximity = 1;
        ai->anchorRadius = 0;
        ai->doNotMove = 0;
        ZeroStruct(&ai->inputs);

        ZeroStruct(&mydata->sets);
        if(mydata->scopeVar)
        {
            mydata->scopeVar->finished = 1;
            mydata->scopeVar = NULL;
        }
        aiBehaviorReinitEntity(e, ai->base);
        // This is now a part of aiReInit
//        aiBehaviorSetAllVars(e, ai);
    }

    // doAction is 0, so we were just supposed to update our sets and list
    if(!params->doAction)
        return;

    // If there's no action, then do the default behavior.

    if(!action){ 
        aiSetActivity(e, ai, AI_ACTIVITY_STAND_STILL);
        aiSetMessageHandler(e, aiCritterMsgHandlerDefault);
        return;
    }

    // Set all the specified animation bits if the priority is not to add a random anim bit.

    if(    action->animBitCount &&
        action->type != AI_PRIORITY_ACTION_ADD_RANDOM_ANIM_BIT &&
        action->type != AI_PRIORITY_ACTION_SET_TEAMMATE_ANIM_BITS)
    {
        int i;
        for(i = 0; i < action->animBitCount; i++){
            if(action->animBits[i] >= 0){
                SETB(e->seq->state, action->animBits[i]);
                SETB(e->seq->sticky_state, action->animBits[i]);
            }
        }
    }

    switch(action->type){
        case AI_PRIORITY_ACTION_ADD_RANDOM_ANIM_BIT:{
            if(action->animBitCount){
                int i = getCappedRandom(action->animBitCount);

                if(action->animBits[i] >= 0){
                    SETB(e->seq->state, action->animBits[i]);
                    SETB(e->seq->sticky_state, action->animBits[i]);
                }
            }
            break;
        }

        case AI_PRIORITY_ACTION_COMBAT:{
            aiSetActivity(e, ai, AI_ACTIVITY_COMBAT);
            aiSetMessageHandler(e, aiCritterMsgHandlerCombat);
            break;
        }

        case AI_PRIORITY_ACTION_RUN_INTO_DOOR:
            if(params->firstTime)
            {
                aiBehaviorAddString(e, ENTAI(e)->base, "RunIntoDoor");
                behavior->finished = 1;
//                 aiDiscardFollowTarget(e, "Starting activityRunIntoDoor.", false);
//                 clearNavSearch(ai->navSearch);
//                 ai->time.lastFailedADoorPathCheck = 0;
            }

//             aiSetActivity(e, ai, AI_ACTIVITY_RUN_INTO_DOOR);
//             aiSetMessageHandler(e, aiCritterMsgHandlerDefault);
            break;

        case AI_PRIORITY_ACTION_RUN_OUT_OF_DOOR:
            devassertmsg(0, "Should not be hitting actionRunOutOfDoor, should be caught by behavior system");
            aiBehaviorAddString(e, ENTAI(e)->base, "RunOutOfDoor");
            behavior->finished = 1;
//            if(params->firstTime)
//            {
//                if(ai->teamMemberInfo.team)
//                {
//                    ai->teamMemberInfo.wantToRunOutOfDoor = 1;
//                    ai->teamMemberInfo.team->teamMemberWantsToRunOut = 1;
//                    SET_ENTHIDE(e) = 1;
//                    e->draw_update = 1;
//                    ai->doNotMove = 1;
//                    AI_LOG(AI_LOG_TEAM, (e, "Hid\n"));
//                }
//                else
//                    EntRunOutOfDoor(e, DoorAnimFindPoint(ENTPOS(e), DoorAnimPointExit, MAX_DOOR_SEARCH_DIST));
//            }
//            if(ai->teamMemberInfo.wantToRunOutOfDoor)
//            {
//                SET_ENTHIDE(e) = 1;
//                e->draw_update = 1;
//                AI_LOG(AI_LOG_TEAM, (e, "Hid Again\n"));
//            }
//
//            aiSetActivity(e, ai, AI_ACTIVITY_RUN_OUT_OF_DOOR);
//            aiSetMessageHandler(e, aiCritterMsgHandlerDefault);
            break;

        case AI_PRIORITY_ACTION_DO_ANIM:{
            if( params->firstTime ) //MW added, if you are told to do an animation, leave your combat mode.  I think this is an OK change
                aiDiscardCurrentPower(e);
            aiSetActivity(e, ai, AI_ACTIVITY_DO_ANIM);
            aiSetMessageHandler(e, aiCritterMsgHandlerDefault);

            break;
        }

        case AI_PRIORITY_ACTION_EXIT_COMBAT:{
            aiDiscardCurrentPower(e);
            //(Setting new PL implicitly abandons AI_ACTIVITY_COMBAT)

            break;
        }

        case AI_PRIORITY_ACTION_FOLLOW_ROUTE:
            {
                PatrolRoute *route = EncounterPatrolRoute(e);

                if(!route)
                    break;

//                if(params->firstTime)
//                {
                    estrPrintf(&behaviorbuf, "Patrol(OverridePerceptionRadius(25),SpawnPosIsSelf,%s",
                        action->targetType == AI_PRIORITY_TARGET_WIRE ? "DoNotGoToSleep,MotionWire," : "CombatOverride(Aggressive),");

                    if(action->doNotRun >= 0)
                        estrConcatf(&behaviorbuf, "DoNotRun(%d)", action->doNotRun);
                    estrConcatf(&behaviorbuf, ")%s",
                        action->targetType != AI_PRIORITY_TARGET_WIRE ? ",Combat" : "");

                    aiBehaviorAddString(e, ENTAI(e)->base, behaviorbuf);
                    behavior->finished = 1;
//                }
//                else if(ai->teamMemberInfo.team && ai->teamMemberInfo.team->finishedPatrol)
//                {
//                    aiBehaviorAddString(e, "Combat");
//                    ai->spawnPosIsSelf = 0;
//                    return;
//                }
//
//                aiSetActivity(e, ai, AI_ACTIVITY_FOLLOW_ROUTE);
                break;
            }

        case AI_PRIORITY_ACTION_FROZEN_IN_PLACE:{
            aiSetActivity(e, ai, AI_ACTIVITY_FROZEN_IN_PLACE);
            aiSetMessageHandler(e, aiCritterMsgHandlerDefault);
            break;
        }

        case AI_PRIORITY_ACTION_HIDE_BEHIND_ENT:{
            aiSetActivity(e, ai, AI_ACTIVITY_HIDE_BEHIND_ENT);
            aiSetMessageHandler(e, aiCritterMsgHandlerDefault);

            break;
        }

        case AI_PRIORITY_ACTION_NEVERFORGETTARGET:{
            if (ai->attackTarget.status){
                ai->attackTarget.status->neverForgetMe = 1;
            }
            break;
        }

        case AI_PRIORITY_ACTION_SCRIPT_CONTROLLED_ACTIVITY:
            break;

        case AI_PRIORITY_ACTION_USE_RANDOM_POWER:{
            aiCritterChooseRandomPower(e);
            break;
        }

        case AI_PRIORITY_ACTION_THANK_HERO:{
            aiSetMessageHandler(e, aiCritterMsgHandlerDefault);
            aiSetActivity(e, ai, AI_ACTIVITY_THANK_HERO);
            break;
        }

        case AI_PRIORITY_ACTION_WANDER:{
            aiSetActivity(e, ai, AI_ACTIVITY_WANDER);
            aiSetMessageHandler(e, aiCritterMsgHandlerDefault);

            switch(action->targetType){
                case AI_PRIORITY_TARGET_SPAWN_POINT:{
                    copyVec3(ai->spawn.pos, ai->anchorPos);
                    ai->anchorRadius = action->radius;
                    break;
                }
            }
            break;
        }

        case AI_PRIORITY_ACTION_PATROL:{ // goto patrol point
            aiSetActivity(e, ai, AI_ACTIVITY_PATROL);
            aiSetMessageHandler(e, aiCritterMsgHandlerDefault);
            ai->returnToSpawnDistance = 10000000;    // hack to prevent returning to spawn point after combat
            break;
        }

        case AI_PRIORITY_ACTION_SET_POWER_ON:
        case AI_PRIORITY_ACTION_SET_POWER_OFF:{
            int set = action->type == AI_PRIORITY_ACTION_SET_POWER_ON;

            if(action->powerName && action->powerName[0]){
                AIPowerInfo* info;
                for(info = ai->power.list; info; info = info->next){
                    if(info->isToggle && !stricmp(info->power->ppowBase->pchName, action->powerName)){
                        if(set != info->power->bActive){
                            aiSetTogglePower(e, info, set);
                        }
                        break;
                    }
                }
            }
            break;
        }

        case AI_PRIORITY_ACTION_SET_TEAMMATE_ANIM_BITS:{
            AITeamMemberInfo* member;
            F32 distSQR = SQR(action->radius);

            if(!ai->teamMemberInfo.team)
                break;

            for(member = ai->teamMemberInfo.team->members.list; member; member = member->next){
                if(member->entity->villainDef && !stricmp(member->entity->villainDef->name, action->targetName)){
                    if(distance3Squared(ENTPOS(e), ENTPOS(member->entity)) <= distSQR){
                        int i;

                        for(i = 0; i < action->animBitCount; i++){
                            SETB(member->entity->seq->state, action->animBits[i]);
                        }

                        break;
                    }
                }
            }

            break;
        }

        case AI_PRIORITY_ACTION_CALLFORHELP:{
            aiCallForHelp( e, ai, action->radius, action->targetName );
            break;
        }

        case AI_PRIORITY_ACTION_CHOOSE_PATROL_POINT:{ // select a random beacon to patrol to
            if(!ai->hasNextPatrolPoint)
            {
                Beacon * beacon = aiFindBeaconInRange(e, ai, ENTPOS(e), 200, 100, 100, 200, NULL);
                if(beacon)
                {
                    copyVec3(posPoint(beacon), ai->anchorPos);
                    ai->hasNextPatrolPoint = TRUE;
                    ai->time.assignedPatrolPoint = ABS_TIME;
                }
            }
            break;
        }

//         case AI_PRIORITY_ACTION_FOLLOW_LEADER:{
//             aiSetActivity(e, ai, AI_ACTIVITY_FOLLOW_LEADER);
//             aiSetMessageHandler(e, aiCritterMsgHandlerFollowLeader);
//             ai->returnToSpawnDistance = 10000000;    // hack to prevent returning to spawn point after combat
//             break;
//         }

        case AI_PRIORITY_ACTION_ALERT_TEAM:{
            if(!ai->teamMemberInfo.alerted){
                EncounterAISignal(e, ENCOUNTER_ALERTED);

                ai->teamMemberInfo.alerted = 1;

                AI_LOG(AI_LOG_TEAM, (e, "Alerted Team\n"));
            }
            else
            {
                AI_LOG(AI_LOG_TEAM, (e, "Did not alert team - it was already alerted!\n"));
            }
        }


        default:{
            aiSetActivity(e, ai, AI_ACTIVITY_STAND_STILL);
            aiSetMessageHandler(e, aiCritterMsgHandlerDefault);
            break;
        }
    }
}

void aiCritterDoActivity(Entity* e, AIVars* ai, AIActivity activity)
{
    switch(activity){
    xcase AI_ACTIVITY_COMBAT:
        if(entCanSetPYR(e)){
            PERFINFO_AUTO_START("ActivityCombat", 1);
                aiCritterActivityCombat(e, ai);
            PERFINFO_AUTO_STOP();
        }

    xcase AI_ACTIVITY_THANK_HERO:
        aiActivityThankHero(e, ai);

    xcase AI_ACTIVITY_DO_ANIM:
        PERFINFO_AUTO_START("ActivityDoAnim", 1);
            aiCritterActivityDoAnim(e, ai);
        PERFINFO_AUTO_STOP();

//     xcase AI_ACTIVITY_FOLLOW_LEADER:
//         PERFINFO_AUTO_START("ActivityFollowLeader", 1);
//             aiCritterActivityFollowLeader(e, ai);
//         PERFINFO_AUTO_STOP();

/*
    xcase AI_ACTIVITY_FOLLOW_ROUTE:
        PERFINFO_AUTO_START("ActivityFollowRoute", 1);
            aiCritterActivityFollowRoute(e, ai, ai->route);
        PERFINFO_AUTO_STOP();
*/

    xcase AI_ACTIVITY_FROZEN_IN_PLACE:
        PERFINFO_AUTO_START("ActivityFrozenInPlace", 1);
            aiCritterActivityFrozenInPlace(e, ai);
        PERFINFO_AUTO_STOP();

//     xcase AI_ACTIVITY_HIDE_BEHIND_ENT:
//         PERFINFO_AUTO_START("ActivityHideBehindEnt", 1);
//             aiCritterActivityHideBehindEnt(e, ai);
//         PERFINFO_AUTO_STOP();

//     xcase AI_ACTIVITY_PATROL:
//         PERFINFO_AUTO_START("ActivityPatrol", 1);
//             if(ABS_TIME_SINCE(ai->time.assignedPatrolPoint) > SEC_TO_ABS(3 * 60))
//                 ai->hasNextPatrolPoint = 0;
//             aiCritterActivityPatrol(e, ai);
//         PERFINFO_AUTO_STOP();

    xcase AI_ACTIVITY_RUN_INTO_DOOR:
        PERFINFO_AUTO_START("ActivityRunIntoDoor", 1);
            aiCritterActivityRunIntoDoor(e, ai);
        PERFINFO_AUTO_STOP();

    xcase AI_ACTIVITY_RUN_OUT_OF_DOOR:
        PERFINFO_AUTO_START("ActivityRunOutOfDoor", 1);
            aiCritterActivityRunOutOfDoor(e, ai);
        PERFINFO_AUTO_STOP();

    xcase AI_ACTIVITY_STAND_STILL:
        PERFINFO_AUTO_START("ActivityStandStill", 1);
            aiCritterActivityStandStill(e, ai);
        PERFINFO_AUTO_STOP();

    xcase AI_ACTIVITY_WANDER:
        PERFINFO_AUTO_START("ActivityWander", 1);
//             aiCritterActivityWander(e, ai, 0);
        PERFINFO_AUTO_STOP();
    }
}
