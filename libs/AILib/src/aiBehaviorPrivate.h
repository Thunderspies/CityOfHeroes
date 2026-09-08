#ifndef AIBEHAVIORPRIVATE_H
#define AIBEHAVIORPRIVATE_H

#include "../include/ailib/aiBehaviorInterface.h"
#include "limits.h"
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/components/StashTable.h"

typedef struct AnimBitList                AnimBitList;
typedef struct AIBehavior                AIBehavior;
typedef struct AIBCondition                AIBCondition;
typedef struct AIBehavior                AIBehavior;
typedef struct AIBehaviorFlag            AIBehaviorFlag;
typedef struct AITeamBase                AITeamBase;
typedef struct AIVarsBase                AIVarsBase;
typedef struct Entity                    Entity;
typedef struct PerformanceInfo            PerformanceInfo;
typedef struct StaticDefineInt            StaticDefineInt;
typedef struct TokenizerFunctionCall    TokenizerFunctionCall;

#define AI_BEHAVIOR_MAX_MODS_PREV            25

// there is an EArray of AIGroupBehaviorData on the AITeam
typedef struct AIBehaviorTeamData
{
    U32 teamDataID;                    // for checking against the groupDataIndex on the behavior
    AIBehaviorInfo* behaviorInfo;    // the type of behavior I belong to
    U32 refCount;
    void* data;
}AIBehaviorTeamData;

typedef struct AIBehaviorMod
{
    AIBehavior* behavior;
    U32 time;
    const char* string;
}AIBehaviorMod;

typedef enum AIBOper{
    AIB_OPER_LT,
    AIB_OPER_EQ,
    AIB_OPER_GT,
}AIBOper;

typedef struct AIBCondition{
    int cat;    // enum specified in a game specific file
    AIBOper op;
    int rval;
}AIBCondition;

typedef struct AIBTableEntry
{
    const char* name;
    int type;
    union{
        AIBehaviorInfo* behaviorInfo;
        AIBehaviorFlagInfo* behaviorFlagInfo;
        AIBehaviorAliasInfo* behaviorAliasInfo;
        void* customPtr;
    };
}AIBTableEntry;

AIBehaviorInfo* aiBehaviorInfoFromString(const char* name);
void aiBehaviorDestroyCondition(void* condData);
void aiBehaviorDestroy(AIVarsBase* aibase, AIBehavior* behavior);
void aiBehaviorModDestroy(AIBehaviorMod* mod);
void aiBehaviorDataFree(Entity* e, AIVarsBase* aibase, void* data);
void aiBehaviorTeamDataRemoveRef(AITeamBase* team, AIBehavior* behavior);
void aiBehaviorParseError(Entity* e, const char* explanation, const char* badstr, const char* fullstr);
int aiBehaviorProcessAllStrings(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors);
AIBehaviorFlag* aiBehaviorFlagCreate(Entity* e, AIVarsBase* ai);
AIBehaviorAliasInfo* aiBehaviorAliasInfoFromString(const char* name);
AIBehavior* aiBehaviorCreatePERMVAR(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors);
void aiBehaviorUpdateAll(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors);
void aiBehaviorRegisterSystemBehaviors(void);

//void aiBehaviorFlagAddInternal(Entity* e, AIBehavior* behavior, AIBehaviorFlagType type, TokenizerFunctionCall** data);
void aiBehaviorFlagDebugString(Entity* e, AIBehavior* behavior, char** estr);
int aiBehaviorFlagProcessString(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior, char* processedStr, TokenizerFunctionCall* parsedflag);

// copies flag to e/ai's specified behavior
void aiBehaviorFlagDuplicate(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior, const AIBehaviorFlag* flag);

void aiBehaviorSetAllVars(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors);

#define AI_LOG(flagBit, params)
#define AI_LOG_ENT_NAME(e)            "stubbed out"
#define AI_LOG_ENABLED(flagBit)        0

#define ABS_TIME_SINCE(x) ((x) ? (unsigned int)(getABS_TIMECallback() - (x)) : UINT_MAX)

extern StashTable BehaviorSystemHashTable;
extern AIBTableEntry** BehaviorSystemTableList;
extern AIBTableEntry** BehaviorSystemDebugAliasList;

extern bool specialBehaviorInfoInitialized;
extern AIBehaviorInfo* combatBehaviorInfo;
extern AIBehaviorInfo* permVarBehaviorInfo;
extern AIBehaviorInfo* scopeVarBehaviorInfo;
extern AIBehaviorInfo* aliasBehaviorInfo;
extern AIBehaviorInfo* flagBehaviorInfo;
extern AIBehaviorInfo* cleanBehaviorInfo;
extern AIBehaviorInfo* doNothingBehaviorInfo;
extern AIBehaviorInfo* delayedPermVarBehaviorInfo;

extern int processingBehaviors;

extern aiBehaviorReinitCallback behaviorReinitCallback;
extern aiBehaviorCheckCombatOverrideCallback behaviorCheckCombatOverrideCallback;
extern aiBehaviorGetABS_TIMECallback getABS_TIMECallback;
extern aiBehaviorBlameFileCallback blameFileCallback;
extern aiBehaviorDefaultBehaviorCallback defaultBehaviorCallback;
extern aiBehaviorFlagProcessCallback flagProcessCallback;
extern aiBehaviorConditionValueCallback behaviorConditionValueCallback;

void aiBehaviorInitSpecialBehaviorInfo(void);

#define INITIALIZE_SPECIAL_INFO_IF_NEEDED\
    if(!specialBehaviorInfoInitialized)\
    aiBehaviorInitSpecialBehaviorInfo()\

typedef struct AIBDClean
{
    const char* origStr;
    AIBParsedString* parsed;
}AIBDClean;


#endif