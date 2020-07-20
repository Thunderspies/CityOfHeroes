#include "utilitieslib/stdtypes.h"
#include "../include/ailib/aiStruct.h"
#include "../include/ailib/aiBehaviorPublic.h"
#include "aiBehaviorPrivate.h"
#include "../include/ailib/aiBehaviorDebug.h"

#include "utilitieslib/components/earray.h"
#include "utilitieslib/components/EString.h"
#include "utilitieslib/components/StringCache.h"
#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/utils/textparser.h"
#include "utilitieslib/utils/timing.h"

static void aiBehaviorOnFinish(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior);
static void aiBehaviorUnsetVars(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior);
static void aiBehaviorRemove(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* remBehavior);

void aiBFDelayedPermVarRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior);

bool specialBehaviorInfoInitialized = false;
AIBehaviorInfo* combatBehaviorInfo = NULL;
AIBehaviorInfo* permVarBehaviorInfo = NULL;
AIBehaviorInfo* scopeVarBehaviorInfo = NULL;
AIBehaviorInfo* aliasBehaviorInfo = NULL;
AIBehaviorInfo* flagBehaviorInfo = NULL;
AIBehaviorInfo* cleanBehaviorInfo = NULL;
AIBehaviorInfo* doNothingBehaviorInfo = NULL;
AIBehaviorInfo* delayedPermVarBehaviorInfo = NULL;

void aiBehaviorInitSpecialBehaviorInfo()
{
    combatBehaviorInfo = aiBehaviorInfoFromString("Combat");
    permVarBehaviorInfo = aiBehaviorInfoFromString("PermVar");
    scopeVarBehaviorInfo = aiBehaviorInfoFromString("ScopeVar");
    aliasBehaviorInfo = aiBehaviorInfoFromString("Alias");
    flagBehaviorInfo = aiBehaviorInfoFromString("Flag");
    cleanBehaviorInfo = aiBehaviorInfoFromString("Clean");
    doNothingBehaviorInfo = aiBehaviorInfoFromString("DoNothing");
    delayedPermVarBehaviorInfo = aiBehaviorInfoFromString("DelayedPermVar");
    devassert(combatBehaviorInfo && permVarBehaviorInfo && scopeVarBehaviorInfo && aliasBehaviorInfo &&
        flagBehaviorInfo && cleanBehaviorInfo && doNothingBehaviorInfo && delayedPermVarBehaviorInfo);
    specialBehaviorInfoInitialized = true;
}

#define INITIALIZE_SPECIAL_INFO_IF_NEEDED\
    if(!specialBehaviorInfoInitialized)\
    aiBehaviorInitSpecialBehaviorInfo()\

int aiBehaviorCheckConditions(Entity* e, AIVarsBase* aibase, AIBCondition** conditions)
{
    int i, n = eaSize(&conditions);

    for(i = 0; i < n; i++)
    {
        int val = behaviorConditionValueCallback(e, aibase, conditions[i]->cat);

        switch(conditions[i]->op)
        {
        xcase AIB_OPER_LT:
            if(val < conditions[i]->rval)
                return 1;
        xcase AIB_OPER_EQ:
            if(val == conditions[i]->rval)
                return 1;
        xcase AIB_OPER_GT:
            if(val > conditions[i]->rval)
                return 1;
        }
    }

    return 0;
}

static void aiBehaviorChange(Entity* e, AIVarsBase* aibase)
{
    aiBehaviorReinitEntity(e, aibase);
}

static void aiBehaviorCleanup(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    aiBehaviorOnFinish(e, aibase, behaviors, behavior);
    devassert(!behavior->data);

    while(eaSize(&behavior->flags))
    {
        aiBehaviorFlagDestroy(e, aibase, (AIBehaviorFlag*)eaPop(&behavior->flags));
    }

    eaDestroy(&behavior->flags);

    eaDestroyEx(&behavior->endConditions, aiBehaviorDestroyCondition);

    aiBehaviorDestroy(aibase, behavior);
}

void aiBehaviorDestroyAll(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors)
{
    int i;
    int count = 0;

    while(i = eaSize(behaviors))
    {
        count++;

        AI_LOG(AI_LOG_BEHAVIOR, (e, "BehaviorDestroyAll: Cleaning up ^9%s^0\n",
            (*behaviors)[i-1]->info->name));

        aiBehaviorCleanup(e, aibase, behaviors, (*behaviors)[i-1]);
        eaPop(behaviors);
    }

    devassert(behaviors != &aibase->behaviors || !aibase->behaviorStructsAllocated &&
        !aibase->behaviorDataAllocated && !aibase->behaviorFlagsAllocated);

    eaDestroy(behaviors);
}

void aiBehaviorDestroyMods(Entity* e, AIVarsBase* aibase)
{
    eaDestroyEx(&aibase->behaviorMods, aiBehaviorModDestroy);
    eaDestroyEx(&aibase->behaviorPrevMods, aiBehaviorModDestroy);
}

static void aiBehaviorOnFinish(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    static char* estr = NULL;

    if(AI_LOG_ENABLED(AI_LOG_BEHAVIOR))
    {
        estrClear(&estr);

        estrPrintCharString(&estr, aiBehaviorGetDebugString(e, aibase, behavior, ""));
    }

    aiBehaviorUnsetVars(e, aibase, behaviors, behavior);

    if(behavior->info->finishFunc)
        behavior->info->finishFunc(e, aibase, behavior);

    if(behavior->info->structSize)
    {
        devassert(behavior->data);
        aiBehaviorDataFree(e, aibase, behavior->data);
        behavior->data = NULL;
    }

    if(behavior->info->teamDataStructSize)
    {
        devassert(behavior->teamDataID);
//        aiBehaviorTeamDataRemoveRef(aibase->teamMemberInfo.team, behavior);
    }

    AI_LOG(AI_LOG_BEHAVIOR, (e, "BehaviorFinish: ^9%s^2%i^0(%s^0)\n",
        behavior->info->name, aiBehaviorGetHandle(e, aibase, behaviors, behavior), estr));
}

static void aiBehaviorOnResume(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    static char* estr = NULL;

    if(AI_LOG_ENABLED(AI_LOG_BEHAVIOR))
    {
        estrClear(&estr);
    }

    //     aiBehaviorSetVars(e, aibase, behaviors, behavior);

    AI_LOG(AI_LOG_BEHAVIOR, (e, "BehaviorResume: ^9%s^2%i^0(%s^0)\n",
        behavior->info->name, aiBehaviorGetHandle(e, aibase, behaviors, behavior),
        estr ? estr : aiBehaviorGetDebugString(e, behavior, "")));
}

static void aiBehaviorOnStart(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    static char* estr = NULL;

    if(AI_LOG_ENABLED(AI_LOG_BEHAVIOR))
    {
        estrClear(&estr);
    }

    if(aibase->isNPC && !(behavior->info->specialFlags & AIB_NPC_ALLOWED))
    {
        aiBehaviorParseError(e, "Trying to run non-NPC behavior on an NPC",
            behavior->info->name, behavior->originalString);
        behavior->finished = 1;
        return;
    } 

    if(behavior->info->startFunc)
        behavior->info->startFunc(e, aibase, behavior);

    behavior->activated = 1;

    AI_LOG(AI_LOG_BEHAVIOR, (e, "BehaviorStart: ^9%s^2%i^0(^4%s^0)\n",
        behavior->info->name,
        aiBehaviorGetHandle(e, aibase, behaviors, behavior),
        estr ? estr : aiBehaviorGetDebugString(e, behavior, "")));
}

static void aiBehaviorOnSwitch(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    static char* estr = NULL;

    if(AI_LOG_ENABLED(AI_LOG_BEHAVIOR))
    {
        estrClear(&estr);
    }

    aiBehaviorUnsetVars(e, aibase, behaviors, behavior);

    AI_LOG(AI_LOG_BEHAVIOR, (e, "BehaviorSwitch: ^9%s^2%i^0(%s^0)\n",
        behavior->info->name, aiBehaviorGetHandle(e, aibase, behaviors, behavior),
        estr ? estr : behavior->info->name));
}

static void aiBehaviorRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    PERFINFO_AUTO_START_STATIC(behavior->info->name, &behavior->info->timer, 1);

    aibase->curBehavior = behavior;
    aibase->overriddenCombat = 0;

    if(behavior->lastRunTime)
        behavior->timeRun += ABS_TIME_SINCE(behavior->lastRunTime);

    behavior->lastRunTime = getABS_TIMECallback();

    if(behavior->info->runFunc)
        behavior->info->runFunc(e, aibase, behaviors, behavior);

    PERFINFO_AUTO_STOP();
}

static void aiBehaviorSetVars(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    if(flagProcessCallback)
        flagProcessCallback(e, aibase, behaviors, behavior);

    if(behavior->info->setFunc)
        behavior->info->setFunc(e, aibase, behavior);
}

static void aiBehaviorUnsetVars(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    if(behavior->info->unsetFunc)
        behavior->info->unsetFunc(e, aibase, behavior);

    behavior->lastRunTime = 0;
}

void aiBehaviorSetAllVars(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors)
{
    int i;

    INITIALIZE_SPECIAL_INFO_IF_NEEDED;

    for(i = 0; i < eaSize(behaviors); i++)
    {
        if((*behaviors)[i]->info == permVarBehaviorInfo || (*behaviors)[i]->info == scopeVarBehaviorInfo)
            aiBehaviorSetVars(e, aibase, behaviors, (*behaviors)[i]);

        if((*behaviors)[i]->running)
        {
            aiBehaviorSetVars(e, aibase, behaviors, (*behaviors)[i]);
            break;
        }
    }
}

void aiBehaviorProcess(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors)
{
    int i;
    bool runningBehavior = true;
    bool alreadyOverrodeCombat = false;

    if(!aibase)
        return;

    aibase->behaviorRequestOffTickProcessing = 0;    // This is a safety measure forcing someone who wants 
    // off-tick processing to set it every behavior tick.
    // Note that it can be very expensive to do off-tick 
    // processing, so if you're reading this you should
    // probably be very careful

    if(!eaSize(behaviors) && defaultBehaviorCallback)
    {
        char* defaultBehaviorStr = defaultBehaviorCallback(e, aibase);
        aiBehaviorAddString(e, aibase, defaultBehaviorStr);
    }

    processingBehaviors++;

    // process flags on all dirty behaviors
    aiBehaviorUpdateAll(e, aibase, behaviors);

    // keep executing the topmost behavior until one doesn't finish
    while(runningBehavior && eaSize(behaviors))
    {
        int index = eaSize(behaviors) - 1;
        AIBehavior* curBehavior = (*behaviors)[index];
        bool allowedToProcessAgain = true;

        if(!curBehavior->running)
        {
            bool switchedBehaviors = true;
            AIBehavior* nextBehavior = NULL;

            for(i = index - 1; i >= 0; i--)
            {
                if((*behaviors)[i]->uninterruptable)
                    nextBehavior = (*behaviors)[i];

                if((*behaviors)[i]->running)
                {
                    if((*behaviors)[i]->uninterruptable)
                    {
                        curBehavior = (*behaviors)[i];
                        switchedBehaviors = false;
                    }
                    else
                    {
                        aiBehaviorOnSwitch(e, aibase, behaviors, (*behaviors)[i]);
                        (*behaviors)[i]->running = 0;
                    }
                    break;
                }
            }

            if(switchedBehaviors)
            {
                if(nextBehavior)
                    curBehavior = nextBehavior;

                curBehavior->running = 1;
                aiBehaviorChange(e, aibase);

                aiBehaviorOnResume(e, aibase, behaviors, curBehavior);
            }
        }

        if(!alreadyOverrodeCombat && !curBehavior->overrideExempt && aibase->behaviorCombatOverride != AIB_COMBAT_OVERRIDE_PASSIVE)
        {
            if(behaviorCheckCombatOverrideCallback(e, aibase, behaviors, curBehavior, aibase->behaviorCombatOverride))
            {
                // combat override put a combat behavior on top, so run that next
                alreadyOverrodeCombat = true;
                continue;
            }
        }

        aiBehaviorRun(e, aibase, behaviors, curBehavior);

        if(curBehavior->finished)
            aiBehaviorRemove(e, aibase, behaviors, curBehavior);
        else
            runningBehavior = false;

        while(allowedToProcessAgain && eaSize(&aibase->behaviorMods))
        {
            allowedToProcessAgain = aiBehaviorProcessAllStrings(e, aibase, behaviors);
            aiBehaviorUpdateAll(e, aibase, behaviors);
        }
    }

    processingBehaviors--;
}

void aiBehaviorProcessOffTick(Entity* e, AIVarsBase* aibase)
{
    AIBehavior* behavior = aibase->behaviors[eaSize(&aibase->behaviors)-1];

    if(behavior->info->offTickFunc)
        behavior->info->offTickFunc(e, aibase, behavior);
}

static void aiBehaviorRemove(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* remBehavior)
{
    int i;

    for(i = eaSize(behaviors) - 1; i >= 0; i--)
        if((*behaviors)[i] == remBehavior)
            break;

    if(i < 0)
        return;

    remBehavior->running = 0;
    aiBehaviorCleanup(e, aibase, behaviors, remBehavior);
    (*behaviors)[i] = NULL;
    eaRemove(behaviors, i);
    aiBehaviorChange(e, aibase);
}

void aiBehaviorUpdateAll(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors)
{
    int i;
    int nonDelayedPermVarFound = false;
    AIBehavior* curBehavior;
    static int currentlyUpdatingBehavior = false;        //    reason why this exists
                                                        //    if aiBehaviorUpdateAll gets called recursively
                                                        //    the deepest call will remove the behavior
                                                        //    and any calls above it, will crash since the pointer is now bad
    

    if (currentlyUpdatingBehavior)
        return;

    currentlyUpdatingBehavior = true;
    for(i = eaSize(behaviors)-1; i >= 0; i--) 
    {
        curBehavior = (*behaviors)[i];

        if(!nonDelayedPermVarFound)
        {
            if(curBehavior->info == delayedPermVarBehaviorInfo)
            {
                AIBehavior* permVarBehavior = (*behaviors)[0];

                if(!permVarBehavior || permVarBehavior->info != permVarBehaviorInfo)
                {
                    // aiBFDelayedPermVarRun is going to create a PermVar at 0 in the behaviors list which will throw off our count
                    i++;
                }

                aiBFDelayedPermVarRun(e, aibase, behaviors, curBehavior);
            }
            else
                nonDelayedPermVarFound = true;
        }

        if(curBehavior->timerTime && curBehavior->timeRun >= (curBehavior->timerTime - 0.5) * 3000)
            curBehavior->finished = 1;

        if(eaSize(&curBehavior->endConditions) &&
            curBehavior->running &&
            aiBehaviorCheckConditions(e, aibase, curBehavior->endConditions))
            curBehavior->finished = 1;
    }

    for(i = 0; i < eaSize(behaviors); i++)
    {
        curBehavior = (*behaviors)[i];

        if(curBehavior->finished)
        {
            aiBehaviorRemove(e, aibase, behaviors, curBehavior);
            i--;
            continue;
        }

        if(curBehavior->dirty && (curBehavior->running || curBehavior->info == permVarBehaviorInfo || curBehavior->info == scopeVarBehaviorInfo)) 
            // only reprocess flags if a new one is added and this behavior is running or it is a PERMVAR/SCOPEVAR
        {
            if(flagProcessCallback)
                flagProcessCallback(e, aibase, behaviors, curBehavior);
            curBehavior->dirty = 0;
        }

        if(!curBehavior->activated)
            aiBehaviorOnStart(e, aibase, behaviors, curBehavior);
    }
    currentlyUpdatingBehavior = false;
}

// takes the TARGET entity and aivars (for bookkeeping purposes)
void aiBehaviorFlagDuplicate(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior, const AIBehaviorFlag* flag)
{
    int i;
    AIBehaviorFlag* dupeFlag = aiBehaviorFlagCreate(e, aibase);

    memcpy(dupeFlag, flag, sizeof(*dupeFlag));

    if(flag->info->datatype == FLAG_DATA_MULT_STR)
    {
        dupeFlag->strarray = NULL;
        for(i = eaSize(&flag->strarray)-1; i >= 0; i--)
            eaPushConst(&dupeFlag->strarray, flag->strarray[i]); // already allocAddString'ed anyway
    }

    if(!dupeFlag->info->allowDuplicates)
    {
        for(i = eaSize(&behavior->flags)-1; i >= 0; i--)
        {
            if(behavior->flags[i]->info == dupeFlag->info)
            {
                aiBehaviorFlagDestroy(e, aibase, eaRemove(&behavior->flags, i));
                AI_LOG(AI_LOG_BEHAVIOR, (e, "Removed flag ^8%s^0 from ^9%s^2%i^0 (^1replaced^0).\n",
                    dupeFlag->info->name,
                    behavior->info->name,
                    aiBehaviorGetHandle(e, aibase, behaviors, behavior)));
            }
        }
    }

    eaPush(&behavior->flags, dupeFlag);
    behavior->dirty = 1;

    AI_LOG(AI_LOG_BEHAVIOR, (e, "Adding flag ^8%s^0 to ^9%s^2%i^0.\n",
        dupeFlag->info->name,
        behavior->info->name,
        aiBehaviorGetHandle(e, aibase, behaviors, behavior)));
}

bool g_TestingBehaviorAliases = false;

//////////////////////////////////////////////////////////////////////////
// Generic Functions
//////////////////////////////////////////////////////////////////////////
AIBehavior* aiBehaviorGetCurBehaviorInternal(Entity* e, AIVarsBase* aibase)
{
    return aibase->curBehavior;
}

int aiBehaviorGetCurBehavior(Entity* e, AIVarsBase* aibase)
{
    if(!e || !aibase || !aibase->curBehavior)
        return 0;

    return aibase->curBehavior->id;
}

int aiBehaviorCurMatchesName(Entity* e, AIVarsBase* aibase, const char* name)
{
    int i;
    AIBehaviorInfo* info = aiBehaviorInfoFromString(name);

    if(!e || !aibase)
        return 0;

    for(i = eaSize(&aibase->behaviors)-1; i >= 0; i--)
    {
        if(aibase->behaviors[i]->running)
        {
            if(aibase->behaviors[i]->info == info)
                return 1;
            else
                return 0;
        }
    }

    return 0;
}

int aiBehaviorPropagateStringToChildren(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* startAfter, TokenizerFunctionCall* param)
{
    // give flag to each behavior in the stack and or the results
    int result = 0;
    int i = 0, n = eaSize(behaviors);

    if(startAfter)
    {
        while(i < n && (*behaviors)[i] != startAfter)
            i++;
        i++;
    }

    while(i < n)
    {
        result |= aiBehaviorFlagProcessString(e, aibase, behaviors, (*behaviors)[i], param->function, param);
        i++;
    }

    return result;
}

void aiBehaviorPropagateFlagToChildren(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* startAfter, const AIBehaviorFlag* flag)
{
    int i = 0, n = eaSize(behaviors);

    if(startAfter)
    {
        while(i < n && (*behaviors)[i] != startAfter)
            i++;
        i++;
    }

    while(i < n)
    {
        aiBehaviorFlagDuplicate(e, aibase, behaviors, (*behaviors)[i], flag);
        i++;
    }
}

//////////////////////////////////////////////////////////////////////////
// Clean
//////////////////////////////////////////////////////////////////////////
BEHAVIOR_DEBUG_FUNC(Clean);
BEHAVIOR_RUN_FUNC(Clean);
BEHAVIOR_STRING_FUNC(Clean);

//////////////////////////////////////////////////////////////////////////
// DelayedPermVar
//////////////////////////////////////////////////////////////////////////
BEHAVIOR_RUN_FUNC(DelayedPermVar);

AIBehaviorInfo SystemBehaviorTable[] = {
    { "Alias",
    NULL,                    aiBFAliasFinish,        aiBFAliasSet,            NULL,
    aiBFAliasRun,            NULL,                    
    aiBFAliasFlag,            aiBFAliasString,
    aiBFAliasDebugStr,
    sizeof(AIBDAlias),        0,                        AIB_NPC_ALLOWED    },
    { "Clean",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFCleanRun,            NULL,                    
    NULL,                    aiBFCleanString,
    aiBFCleanDebugStr,
    sizeof(AIBDClean),        0,                        AIB_NPC_ALLOWED    },
    { "DelayedPermVar",
    NULL,                    NULL,                    NULL,                    NULL,
    aiBFDelayedPermVarRun,    NULL,                    
    NULL,                    NULL,
    NULL,
    0,                        0,                        AIB_NPC_ALLOWED    },
};

void aiBehaviorRegisterSystemBehaviors()
{
    int i;

    for(i = sizeof(SystemBehaviorTable)/sizeof(AIBehaviorInfo)-1; i >= 0; i--)
    {
        aiBehaviorTableAddBehavior(&SystemBehaviorTable[i]);
    }
}

//////////////////////////////////////////////////////////////////////////
// Alias
//////////////////////////////////////////////////////////////////////////
void aiBFAliasDebugStr(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, char** estr, char* indent)
{
    AIBDAlias* mydata = (AIBDAlias*)behavior->data;
    int i;

    estrConcatf(estr, "%s^0Name: ^9%s^0\n", indent, mydata->aliasName);

    for(i = 0; i < eaSize(&mydata->behaviors); i++)
    {
        estrConcatf(estr, "%s%s", aiBehaviorGetDebugString(e, aibase, mydata->behaviors[i], indent), i < eaSize(&mydata->behaviors)-1 ? "\n" : "");
    }
}

void aiBFAliasFinish(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDAlias* mydata = (AIBDAlias*)behavior->data;

    aiBehaviorDestroyAll(e, aibase, &mydata->behaviors);
}

int aiBFAliasFlag(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, AIBehaviorFlag* flag)
{
    AIBDAlias* mydata = (AIBDAlias*) behavior->data;

    aiBehaviorPropagateFlagToChildren(e, aibase, &mydata->behaviors, NULL, flag);

    return FLAG_PROCESSED;
}

void aiBFAliasRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDAlias* mydata = (AIBDAlias*)behavior->data;

    if(eaSize(&mydata->behaviors))
        aiBehaviorProcess(e, aibase, &mydata->behaviors);
    else
        behavior->finished = 1;

    behavior->overrideExempt = aibase->curBehavior->overrideExempt;
}

void aiBFAliasSet(Entity* e, AIVarsBase* aibase, AIBehavior* behavior)
{
    AIBDAlias* mydata = (AIBDAlias*)behavior->data;

    aiBehaviorSetAllVars(e, aibase, &mydata->behaviors);
}

int aiBFAliasString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDAlias* mydata = (AIBDAlias*)behavior->data;

    if(!mydata->behaviors)
    {
        AIBehaviorAliasInfo* info;

        if(!strnicmp("PL_", param->function, 3))
            info = aiBehaviorAliasInfoFromString(param->function+3);
        else
            info = aiBehaviorAliasInfoFromString(param->function);

        if(info)
        {
            AIBParsedString* parsed = info->parsedStr;
            mydata->aliasName = allocAddString(param->function);
            AI_LOG(AI_LOG_BEHAVIOR, (e, "ProcessString: Processing ^9%s^0\n", param->function));
            aiBehaviorProcessString(e, aibase, &mydata->behaviors, param->function, parsed, false);
            return 1;
        }

        return 0;
    }
    else
    {
        return aiBehaviorPropagateStringToChildren(e, aibase, &mydata->behaviors, NULL, param);
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////
// Clean
//////////////////////////////////////////////////////////////////////////
void aiBFCleanDebugStr(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, char** estr, char* indent)
{
    AIBDClean* mydata = (AIBDClean*)behavior->data;

    estrConcatf(estr, "%s^0Name: ^9%s^0", indent, mydata->origStr);
}

void aiBFCleanRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBDClean* mydata = (AIBDClean*)behavior->data;

    aiBehaviorMarkAllFinished(e, aibase, 1);
    behavior->finished = 0; // not the cleanest solution, but we can't have clean removing itself before it's done
    if(mydata->origStr)
    {
        aiBehaviorProcessString(e, aibase, behaviors, mydata->origStr, mydata->parsed, false);
    }
    behavior->finished = 1;
}

int aiBFCleanString(Entity* e, AIVarsBase* aibase, AIBehavior* behavior, TokenizerFunctionCall* param)
{
    AIBDClean* mydata = (AIBDClean*)behavior->data;

    if(!mydata->origStr)
    {
        mydata->origStr = allocAddString(param->function);
        mydata->parsed = aiBehaviorParseString(mydata->origStr);
        return 1;
    }
    else
    {
        const char* blameFile = NULL;
        static char* buf = NULL;

        estrPrintf(&buf, "Clean does not take any arguments, are you sure you're using it right? (had: %s got: %s)",
            mydata->origStr, param->function);

        if(blameFileCallback)
            blameFile = blameFileCallback(e);

        if(blameFile)
            ErrorFilenamef(blameFile, "%s", buf);
        else
            Errorf("%s", buf);
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////
// DelayedPermVar
//////////////////////////////////////////////////////////////////////////
void aiBFDelayedPermVarRun(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    AIBehavior* permVar = aiBehaviorCreatePERMVAR(e, aibase, behaviors);
    int i;

    for(i = eaSize(&behavior->flags)-1; i >= 0; i--)
    {
        aiBehaviorFlagDuplicate(e, aibase, behaviors, permVar, behavior->flags[i]);
    }
    behavior->finished = 1;
}
