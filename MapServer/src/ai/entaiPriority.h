#include "entaivars.h"

#ifndef ENTAIPRIORITY_H
#define ENTAIPRIORITY_H

typedef struct AIPriorityManager    AIPriorityManager;
typedef struct TokenizerParams        TokenizerParams;

typedef enum AIPriorityEnum {
    // Begin values.
    
    AI_PRIORITY_PUBLIC_ENUM_BEGIN = -2,

    // Booleans.

    AI_PRIORITY_BOOL_LOW = -1,
    AI_PRIORITY_FALSE = 0,
    AI_PRIORITY_TRUE = 1,
    AI_PRIORITY_BOOL_HIGH,

    // Action.

    AI_PRIORITY_ACTION_LOW,
    AI_PRIORITY_ACTION_NOT_SET,

    AI_PRIORITY_ACTION_ADD_RANDOM_ANIM_BIT,
    AI_PRIORITY_ACTION_ALERT_TEAM,
    AI_PRIORITY_ACTION_CALLFORHELP,
    AI_PRIORITY_ACTION_CHOOSE_PATROL_POINT,
    AI_PRIORITY_ACTION_COMBAT,
    AI_PRIORITY_ACTION_DISABLE_PRIORITY,
    AI_PRIORITY_ACTION_DO_ANIM,
    AI_PRIORITY_ACTION_EXIT_COMBAT,
    AI_PRIORITY_ACTION_FOLLOW_LEADER,
    AI_PRIORITY_ACTION_FOLLOW_ROUTE,
    AI_PRIORITY_ACTION_FROZEN_IN_PLACE,
    AI_PRIORITY_ACTION_HIDE_BEHIND_ENT,
    AI_PRIORITY_ACTION_NEXT_PRIORITY,
    AI_PRIORITY_ACTION_NEVERFORGETTARGET,
    AI_PRIORITY_ACTION_NPC_HOSTAGE,
    AI_PRIORITY_ACTION_THANK_HERO,
    AI_PRIORITY_ACTION_PATROL,
    AI_PRIORITY_ACTION_PLAY_FX,
    AI_PRIORITY_ACTION_RUN_AWAY,
    AI_PRIORITY_ACTION_RUN_INTO_DOOR,
    AI_PRIORITY_ACTION_RUN_OUT_OF_DOOR,
    AI_PRIORITY_ACTION_RUN_TO_TARGET,
    AI_PRIORITY_ACTION_SET_POWER_ON,
    AI_PRIORITY_ACTION_SET_POWER_OFF,
    AI_PRIORITY_ACTION_SET_TEAMMATE_ANIM_BITS,
    AI_PRIORITY_ACTION_SCRIPT_CONTROLLED_ACTIVITY,
    AI_PRIORITY_ACTION_STAND_STILL,
    AI_PRIORITY_ACTION_USE_RANDOM_POWER,
    AI_PRIORITY_ACTION_WANDER,



    AI_PRIORITY_ACTION_HIGH,
    
    // Targets.

    AI_PRIORITY_TARGET_LOW,
    AI_PRIORITY_TARGET_FARTHEST_BEACON,
    AI_PRIORITY_TARGET_NEARBY_CRITTERS,
    AI_PRIORITY_TARGET_SPAWN_POINT,
    AI_PRIORITY_TARGET_WIRE,
    AI_PRIORITY_TARGET_HIGH,
    
    // ConditionOps
    
    AI_PRIORITY_CONDITION_OP_LOW,
    AI_PRIORITY_CONDITION_OP_NOT_SET,

    AI_PRIORITY_CONDITION_OP_HAS_TEAMMATE_NEARBY,

    AI_PRIORITY_CONDITION_OP_HIGH,
    
    // End values.

    AI_PRIORITY_PUBLIC_ENUM_END,
} AIPriorityEnum;

typedef struct AIPriorityAction {
    AIPriorityEnum        type;
    TokenizerParams**    animBitNames;        // animBitNames will actually be used as TokenizerParams**.
    int                    animBitCount;
    int*                animBits;
    float                radius;
    int                    targetType;
    char*                targetName;
    char*                fxFile;
    int                    doNotRun;
    char*                powerName;
} AIPriorityAction;

typedef struct AIPriorityDoActionParams {
    U32                firstTime            : 1;
    U32                switchedPriority    : 1;
    U32                doAction            : 1;
} AIPriorityDoActionParams;

typedef void (*AIPriorityDoActionFunc)(Entity* e, const AIPriorityAction* action, const AIPriorityDoActionParams* params);

typedef struct AIPriorityUpdateParams {
    float                    timePassed;
    AIPriorityDoActionFunc    doActionFunc;
    U32                        doAction : 1;
} AIPriorityUpdateParams;

// Functions.

void aiPriorityInit(Entity* e);

AIPriorityManager* aiPriorityManagerCreate();
void aiPriorityDestroy(AIPriorityManager* manager);

void aiPriorityUpdate(Entity* e, AIVars* ai, AIPriorityManager* manager, AIPriorityUpdateParams* params);
void aiPriorityUpdateGeneric(Entity* e, AIVars* ai, AIPriorityManager* manager, float timePassed, AIPriorityDoActionFunc doActionFunc, int doAction);

const char* aiGetPriorityName(AIPriorityManager* manager);
const char**  aiGetIAmA(Entity* e);
const char** aiGetIReactTo(Entity* e);

int aiGetPriorityListCount(void);
char* aiGetEntityPriorityListName(Entity * e);

const char* aiGetPriorityListNameByIndex(int i);
void setAnimationBasedOnPriorityList( Entity * e, const char * name );

void aiCritterPriorityDoActionFunc(    Entity* e,
                                   const AIPriorityAction* action,
                                   const AIPriorityDoActionParams* params);
void aiCritterDoActivity(Entity* e, AIVars* ai, AIActivity activity);

#endif