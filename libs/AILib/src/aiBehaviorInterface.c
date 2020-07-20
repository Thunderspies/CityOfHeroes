#include "utilitieslib/stdtypes.h"
#include "../include/ailib/aiBehaviorPublic.h"
#include "aiBehaviorPrivate.h"
#include "../include/ailib/aiStruct.h"

#include "utilitieslib/components/earray.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/components/EString.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/utils/rand.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/components/StringCache.h"
#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/utils/textparser.h"
#include "utilitieslib/utils/utils.h"

int processingBehaviors = 0;
aiBehaviorDefaultBehaviorCallback defaultBehaviorCallback = NULL;
static aiBehaviorStringPreprocessCallback stringPreprocessCallback = NULL;
aiBehaviorBlameFileCallback blameFileCallback = NULL;
static aiBehaviorSpecialFlagProcessingCallback specialFlagProcessingCallback = NULL;
aiBehaviorFlagProcessCallback flagProcessCallback = NULL;

static aiBehaviorReloadCallback behaviorReloadCallback = NULL;
static aiBehaviorFlagReloadCallback behaviorFlagReloadCallback = NULL;
static aiBehaviorAliasReloadCallback behaviorAliasReloadCallback = NULL;
static aiBehaviorTableCustomReloadCallback behaviorTableCustomReloadCallback = NULL;

static aiBehaviorCustomTableEntryCallback behaviorCustomTableEntryCallback = NULL;

aiBehaviorGetABS_TIMECallback getABS_TIMECallback = NULL;

aiBehaviorReinitCallback behaviorReinitCallback = NULL;
aiBehaviorCheckCombatOverrideCallback behaviorCheckCombatOverrideCallback = NULL;

aiBehaviorConditionValueCallback behaviorConditionValueCallback = NULL;
aiBehaviorConditionParseCallback behaviorConditionParseCallback = NULL;

int aiBehaviorGetMaxDataSize();
int aiBehaviorGetMaxTeamDataSize();
static U32 aiBehaviorTeamDataAddRef(AITeamBase* team, AIBehaviorInfo* info);

int aiBehaviorParseInt(TokenizerFunctionCall* param);
F32 aiBehaviorParseFloat(TokenizerFunctionCall* param);
AIBParsedString* aiBehaviorParseString(const char* string);

void aiBehaviorProcessString(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, const char* origStr, AIBParsedString* parsed, bool uninterruptable);

// I guess this should be in an AIlib system file, but oh well, just two functions for now...
MP_DEFINE(AIVarsBase);
MP_DEFINE(AITeamBase);

AIVarsBase* aiSystemGetBase()
{
    MP_CREATE(AIVarsBase, 64);
    return MP_ALLOC(AIVarsBase);
}

void aiSystemDestroyBase(AIVarsBase* base)
{
    MP_FREE(AIVarsBase, base);
}

AITeamBase* aiSystemGetTeamData()
{
    MP_CREATE(AITeamBase, 32);
    return MP_ALLOC(AITeamBase);
}

void aiSystemDestroyTeamData(AITeamBase* base)
{
    MP_FREE(AITeamBase, base);
}

// DATA MANAGEMENT
static MemoryPool MP_NAME(AIBehaviorDataPool) = 0;
static MemoryPool MP_NAME(AIBehaviorTeamDataPool) = 0;

static void* aiBehaviorDataAlloc(Entity* e, AIVarsBase* aibase)
{
    // Init behavior data pool.

    if(!MP_NAME(AIBehaviorDataPool)){
        MP_NAME(AIBehaviorDataPool) =  createMemoryPoolNamed("AIBehaviorDataPool", __FILE__, __LINE__);

        initMemoryPool(MP_NAME(AIBehaviorDataPool), aiBehaviorGetMaxDataSize(), 100);
        mpSetMode(MP_NAME(AIBehaviorDataPool), ZERO_MEMORY_BIT);
    }

    aibase->behaviorDataAllocated++;

    devassert(aibase->behaviorDataAllocated > 0);

    return mpAlloc(MP_NAME(AIBehaviorDataPool));
}

void aiBehaviorDataFree(Entity* e, AIVarsBase* aibase, void* data)
{
    if(!data)
        return;

    devassert(MP_NAME(AIBehaviorDataPool));

    devassert(aibase->behaviorDataAllocated > 0);

    aibase->behaviorDataAllocated--;

    mpFree(MP_NAME(AIBehaviorDataPool), data);
}

MP_DEFINE(AIBehavior);

static AIBehavior* aiBehaviorCreate(Entity* e, AIVarsBase* aibase, AITeamBase* team, AIBehaviorInfo* info)
{
    AIBehavior* newbehavior;

    aibase->behaviorStructsAllocated++;
    devassert(aibase->behaviorStructsAllocated > 0);

    MP_CREATE(AIBehavior, 100);
    newbehavior = MP_ALLOC(AIBehavior);

    newbehavior->info = info;

    if(info->structSize)
        newbehavior->data = aiBehaviorDataAlloc(e, aibase);

    if(info->teamDataStructSize)
    {
        devassertmsg(0, "There are still bugs in the team data handling, please don't use until further notice");
        devassert(team);
        // TODO: set up a callback for creating a team or something
//        aiCritterCheckProximity(e, 0);    // force a team creation
        newbehavior->teamDataID = aiBehaviorTeamDataAddRef(team, info);
    }

    return newbehavior;
}

void aiBehaviorDestroy(AIVarsBase* aibase, AIBehavior* behavior)
{
    devassert(aibase->behaviorStructsAllocated > 0);
    aibase->behaviorStructsAllocated--;

    devassert(!behavior->data);
    MP_FREE(AIBehavior, behavior);
}

size_t aiBehaviorGetDataChunkSize()
{
    return mpStructSize(MP_NAME(AIBehaviorDataPool));
}

MP_DEFINE(AIBehaviorTeamData);

static AIBehaviorTeamData* aiBehaviorTeamDataCreate(AITeamBase* team)
{
    AIBehaviorTeamData* teamdata;

    static U32 groupDataIDCounter = 1;

    MP_CREATE(AIBehaviorTeamData, 100);
    teamdata = MP_ALLOC(AIBehaviorTeamData);
    teamdata->teamDataID = groupDataIDCounter++;

    // create memorypool
    if(!MP_NAME(AIBehaviorTeamDataPool)){
        MP_NAME(AIBehaviorTeamDataPool) =  createMemoryPoolNamed("AIBehaviorTeamDataPool", __FILE__, __LINE__);

        initMemoryPool(MP_NAME(AIBehaviorTeamDataPool), aiBehaviorGetMaxTeamDataSize(), 100);
        mpSetMode(MP_NAME(AIBehaviorTeamDataPool), ZERO_MEMORY_BIT);
    }

    team->behaviorTeamDataAllocated++;

    devassert(team->behaviorTeamDataAllocated > 0);

    teamdata->data = mpAlloc(MP_NAME(AIBehaviorTeamDataPool));

    return teamdata;
}

static void aiBehaviorTeamDataFree(AITeamBase* team, AIBehaviorTeamData* teamdata)
{
    if(!teamdata)
        return;

    devassert(MP_NAME(AIBehaviorTeamDataPool));

    devassert(team->behaviorTeamDataAllocated > 0);

    team->behaviorTeamDataAllocated--;

    mpFree(MP_NAME(AIBehaviorTeamDataPool), teamdata->data);
    MP_FREE(AIBehaviorTeamData, teamdata);
}

size_t aiBehaviorGetTeamDataChunkSize()
{
    return mpStructSize(MP_NAME(AIBehaviorTeamDataPool));
}

static U32 aiBehaviorTeamDataAddRef(AITeamBase* team, AIBehaviorInfo* info)
{
    AIBehaviorTeamData* teamdata;
    int i;

    for(i = eaSize(&team->behaviorData)-1; i >= 0; i--)
    {
        if(team->behaviorData[i]->behaviorInfo == info)
        {
            team->behaviorData[i]->refCount++;
            return team->behaviorData[i]->teamDataID;
        }
    }

    teamdata = aiBehaviorTeamDataCreate(team);
    teamdata->refCount = 1;
    teamdata->behaviorInfo = info;
    eaPush(&team->behaviorData, teamdata);

    return teamdata->teamDataID;
}

void aiBehaviorTeamDataRemoveRef(AITeamBase* team, AIBehavior* behavior)
{
    int i;

    for(i = eaSize(&team->behaviorData)-1; i >= 0; i--)
    {
        if(team->behaviorData[i]->teamDataID == behavior->teamDataID)
        {
            if(!--team->behaviorData[i]->refCount)
            {
                aiBehaviorTeamDataFree(team, team->behaviorData[i]);
                eaRemove(&team->behaviorData, i);
            }
            return;
        }
    }

    devassertmsg(0, "Was told to clean up AIBehaviorTeamData that wasn't found");
}

MP_DEFINE(AIBehaviorMod);

AIBehaviorMod* aiBehaviorModCreate()
{
    AIBehaviorMod* mod;
    MP_CREATE(AIBehaviorMod, 10);
    mod = MP_ALLOC(AIBehaviorMod);
    mod->time = getABS_TIMECallback();
    return mod;
}

void aiBehaviorModDestroy(AIBehaviorMod* mod)
{
    MP_FREE(AIBehaviorMod, mod);
}

AIBehavior* aiBehaviorGetFromHandle(Entity* e, AIVarsBase* aibase, int handle)
{
    int i;

    for(i = eaSize(&aibase->behaviors)-1; i >= 0; i--)
    {
        if(aibase->behaviors[i]->id == handle)
            return aibase->behaviors[i];
    }

    return NULL;
}

StashTable BehaviorSystemHashTable = 0;
AIBTableEntry** BehaviorSystemTableList = NULL;
AIBTableEntry** BehaviorSystemDebugAliasList = NULL;

StaticDefineInt ParseTableEntryType[] = {
    DEFINE_INT
    { "Behavior",        AIB_BEHAVIOR            },
    { "Flag",            AIB_FLAG                },
    { "Alias",            AIB_ALIAS                },
    { "Custom",            AIB_CUSTOM                },
    DEFINE_END
};

bool aiBehaviorAddToHashTable(AIBTableEntry* entry, int isAlias)
{
    AIBTableEntry* oldEntry;
    if(!BehaviorSystemHashTable)
        BehaviorSystemHashTable = stashTableCreateWithStringKeys(128, StashDefault);

    stashFindPointer(BehaviorSystemHashTable, entry->name, &oldEntry);

    if(oldEntry)
    {
        Errorf("Behavior system name collision: %s %s and %s %s\n",
            StaticDefineIntRevLookup(ParseTableEntryType, oldEntry->type), oldEntry->name,
            StaticDefineIntRevLookup(ParseTableEntryType, entry->type), entry->name);
    }
    else
    {
        stashAddPointer(BehaviorSystemHashTable, entry->name, entry, false);
        eaPush(&BehaviorSystemTableList, entry);

        if (isAlias)
            eaPush(&BehaviorSystemDebugAliasList, entry);

        return true;
    }

    return false;
}

void aiBehaviorTableAddBehavior(AIBehaviorInfo* info)
{
    AIBTableEntry* entry = (AIBTableEntry*)calloc(sizeof(AIBTableEntry), 1);

    entry->name = info->name;
    entry->type = AIB_BEHAVIOR;
    entry->behaviorInfo = info;

    if(!aiBehaviorAddToHashTable(entry, 0))
        SAFE_FREE(entry);
}

void aiBehaviorTableAddFlag(AIBehaviorFlagInfo* info)
{
    AIBTableEntry* entry = (AIBTableEntry*)calloc(sizeof(AIBTableEntry), 1);

    entry->name = info->name;
    entry->type = AIB_FLAG;
    entry->behaviorFlagInfo = info;

    if(!aiBehaviorAddToHashTable(entry, 0))
        SAFE_FREE(entry);
}

void aiBehaviorTableAddAlias(AIBehaviorAliasInfo* info)
{
    AIBTableEntry* entry = (AIBTableEntry*)calloc(sizeof(AIBTableEntry), 1);

    entry->name = info->name;
    entry->type = AIB_ALIAS;
    entry->behaviorAliasInfo = info;

    if(!aiBehaviorAddToHashTable(entry, 1))
        SAFE_FREE(entry);
}

void aiBehaviorTableAddCustom(void* info, int type, const char* name)
{
    AIBTableEntry* entry = (AIBTableEntry*)calloc(sizeof(AIBTableEntry), 1);

    entry->name = name;
    entry->type = type;
    entry->customPtr = info;

    if(!aiBehaviorAddToHashTable(entry, 0))
        SAFE_FREE(entry);
}

AIBTableEntry* aiBehaviorLookUpString(const char* name)
{
    AIBTableEntry* entry;

    stashFindPointer(BehaviorSystemHashTable, name, &entry);
    return entry;
}

AIBehaviorInfo* aiBehaviorInfoFromString(const char* name)
{
    AIBTableEntry* entry;

    entry = aiBehaviorLookUpString(name);

    if(entry && entry->type == AIB_BEHAVIOR)
        return entry->behaviorInfo;
    else
        return NULL;
}

AIBehaviorFlagInfo* aiBehaviorFlagInfoFromString(const char* name)
{
    AIBTableEntry* entry;

    entry = aiBehaviorLookUpString(name);

    if(entry && entry->type == AIB_FLAG)
        return entry->behaviorFlagInfo;
    else
        return NULL;
}

AIBehaviorAliasInfo* aiBehaviorAliasInfoFromString(const char* name)
{
    AIBTableEntry* entry;

    entry = aiBehaviorLookUpString(name);

    if(entry && entry->type == AIB_ALIAS)
        return entry->behaviorAliasInfo;
    else
        return NULL;
}

int cmpBehaviorTableEntry(const AIBTableEntry** l, const AIBTableEntry** r)
{
    const AIBTableEntry* lhs = *l;
    const AIBTableEntry* rhs = *r;
    return stricmp(lhs->name, rhs->name);
}

void aiBehaviorRebuildLookupTable()
{
    if (BehaviorSystemHashTable)
        stashTableClear(BehaviorSystemHashTable);
    eaDestroyEx(&BehaviorSystemTableList, NULL);

    aiBehaviorRegisterSystemBehaviors();

    if(behaviorReloadCallback)
        behaviorReloadCallback();
    if(behaviorFlagReloadCallback)
        behaviorFlagReloadCallback();
    if(behaviorAliasReloadCallback)
        behaviorAliasReloadCallback();
    if(behaviorTableCustomReloadCallback)
        behaviorTableCustomReloadCallback();

    eaQSort(BehaviorSystemTableList, cmpBehaviorTableEntry);
}

void aiBehaviorSortDebugAliasTable(void)
{
    eaQSort(BehaviorSystemDebugAliasList, cmpBehaviorTableEntry);
}

MP_DEFINE(AIBCondition);

AIBCondition* aiBehaviorConditionCreate()
{
    MP_CREATE(AIBCondition, 10);
    return MP_ALLOC(AIBCondition);
}

void aiBehaviorDestroyCondition(AIBCondition* cond)
{
    MP_FREE(AIBCondition, cond);
}

AIBCondition* aiBehaviorParseCondition(TokenizerFunctionCall* parse)
{
    AIBCondition cond;
    AIBCondition* retCond;
    bool valid = true;

    devassert(!stricmp(parse->function, "EndCondition"));

    cond.cat = behaviorConditionParseCallback(parse->params[0]->function);

    if(eaSize(&parse->params) != 3)
        return NULL;

    switch(parse->params[1]->function[0])
    {
    xcase '<':
        cond.op = AIB_OPER_LT;
    xcase '=':
        cond.op = AIB_OPER_EQ;
    xcase '>':
        cond.op = AIB_OPER_GT;
    xdefault:
        return NULL;
    }

    cond.rval = aiBehaviorParseInt(parse->params[2]);

    retCond = aiBehaviorConditionCreate();
    retCond->cat = cond.cat;
    retCond->op = cond.op;
    retCond->rval = cond.rval;

    return retCond;
}

AIBehavior* aiBehaviorAdd(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehaviorInfo* info, int insertAt)
{
    static U32 idCounter = 1;
    AIBehavior* behavior;

    if(!info)
        return 0;
    if(!*behaviors)
        eaCreate(behaviors);

    behavior = aiBehaviorCreate(e, aibase, NULL, info);
    eaInsert(behaviors, behavior, insertAt);
    behavior->id = idCounter++;

    return behavior;
}

// All other flag functions are in entaiBehaviorFlag.c now, but this function uses the parser stuff
// and is very much part of the external interface so I left it here
void aiBehaviorProcessFlag(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* in_behavior, const char* string)
{
    AIBehavior* behavior;
    AIBParsedString* parsed;
    int i;

    AI_LOG(AI_LOG_BEHAVIOR, (e, "AddFlag: ^9%s^0\n", string));

    if(!in_behavior && eaSize(behaviors))
        behavior = (*behaviors)[eaSize(behaviors)-1];
    else
        behavior = in_behavior;

    if(!behavior)
        return;

    parsed = aiBehaviorParseString(string);
    //     aiBehaviorDebugVerifyParse(string, parsed);

    for(i = 0; i < eaSize(&parsed->string); i++)
    {
        if(!aiBehaviorFlagProcessString(e, aibase, behaviors, behavior, parsed->string[i]->function, parsed->string[i]))
        {
            Errorf("Unrecognized flag in AddFlag: %s (on behavior %s)", parsed->string[i]->function, behavior->info->name);
        }
    }
    //aiBehaviorDestroyParsedString(&parsed);

    aiBehaviorUpdateAll(e, aibase, behaviors);

}

AIBehavior* aiBehaviorCreatePERMVAR(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors)
{
    AIBehavior* behavior = NULL;

    if(eaSize(behaviors))
        behavior = (*behaviors)[0];

    if(!behavior || behavior->info != permVarBehaviorInfo)
        behavior = aiBehaviorAdd(e, aibase, behaviors, permVarBehaviorInfo, 0); 

    return behavior;
}

AIBehavior* aiBehaviorCreateSCOPEVAR(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, int idx)
{
    AIBehavior* behavior = NULL;

    INITIALIZE_SPECIAL_INFO_IF_NEEDED;

    if(idx < 0 || idx >= eaSize(behaviors))
    {
        devassertmsg(0, "Invalid index passed to CreateSCOPEVAR");
        return NULL;
    }

    behavior = aiBehaviorCreate(e, aibase, NULL, scopeVarBehaviorInfo);

    if(!eaSize(behaviors))
        eaCreate(behaviors);

    // eaInsert will just return if the idx is off
    eaInsert(behaviors, behavior, idx);

    return behavior;
}

// Gets called to clean up CombatOverride initiated combat
void aiBehaviorCombatFinished(Entity* e, AIVarsBase* aibase)
{
    int i;

    if(!aibase->overriddenCombat)
        return;

    INITIALIZE_SPECIAL_INFO_IF_NEEDED;

    for(i = eaSize(&aibase->behaviors) - 1; i >= 0; i--)
    {
        // seeing as aibase->overriddenCombat is true, the top behavior has to be running combat
        // (aiBehaviorRun turns it off), therefore, even if this is not a combat behavior specifically
        // it must be a behavior that has its own internal behavior stack which IS running combat
        if(aibase->behaviors[i]->running /*&& aibase->behaviors[i]->info == combatBehaviorInfo*/)
        {
            aiBehaviorAddFlag(e, aibase, aibase->behaviors[i]->id, "EndOverriddenCombat");
//             AIBDCombat* mydata = (AIBDCombat*) behavior->data;
//             if(mydata->fromOverride)
//                 behavior->finished = 1;
        }
    }
}

int aiBehaviorGetHandle(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior)
{
    int i;

    for(i = eaSize(behaviors)-1; i >= 0; i--)
    {
        if((*behaviors)[i] == behavior)
            return (*behaviors)[i]->id;
    }

    return 0;
}

int aiBehaviorGetTopBehaviorByName(Entity* e, AIVarsBase* aibase, const char* name)
{
    int i;
    AIBehaviorInfo* info = aiBehaviorInfoFromString(name);

    for(i = eaSize(&aibase->behaviors)-1; i >= 0; i--)
    {
        if(aibase->behaviors[i]->info == info && !aibase->behaviors[i]->finished)
            return aibase->behaviors[i]->id;
    }
    return 0;
}


void aiBehaviorMarkAllFinished(Entity* e, AIVarsBase* aibase, int finishPermvar)
{
    int i;

    INITIALIZE_SPECIAL_INFO_IF_NEEDED;

    for(i = eaSize(&aibase->behaviors)-1; i >= 0; i--)
    {
        // only remove PERMVAR if specified
        if(finishPermvar || aibase->behaviors[i]->info != permVarBehaviorInfo)
            aibase->behaviors[i]->finished = 1;
    }
}

void aiBehaviorMarkFinished(Entity* e, AIVarsBase* aibase, int handle)
{
    int i;

    for(i = eaSize(&aibase->behaviors)-1; i >= 0; i--)
    {
        if(aibase->behaviors[i]->id == handle)
        {
            aibase->behaviors[i]->finished = 1;
            return;
        }
    }
}

TokenizerParseInfo ParseBehaviorString[] =
{
    { "Behavior",    TOK_FUNCTIONCALL(AIBParsedString, string)    },
    { "", 0, 0 }
};

MP_DEFINE(AIBParsedString);

AIBParsedString* aiBehaviorParseString(const char* string)
{
    static StashTable BehaviorParses = NULL;
    static char* estr = NULL;
    AIBParsedString* resultParse = NULL;
    const char* allocedStr = allocAddString(string);
    char escapebuf[TOKEN_BUFFER_LENGTH]; 

    if(!BehaviorParses)
    {
        BehaviorParses = stashTableCreateWithStringKeys(128, StashDefault);
        MP_CREATE(AIBParsedString, 128);
    }

    if(!estr)
        estrCreate(&estr);

    estrPrintf(&estr, "behavior %s", string);

    if(stashFindPointer(BehaviorParses, allocedStr, &resultParse))
        return resultParse;
    else
    {
        TokenizerHandle tok;
        AIBParsedString* parsed;
        static char* buf = NULL;
        size_t strLen = strlen(estr);
        char* cleanStart = estr;

        parsed = MP_ALLOC(AIBParsedString);
        estrClear(&buf);

        while(cleanStart = strstri(cleanStart, "clean"))
        {
            if(cleanStart - estr + 5 != strLen &&
                (cleanStart == estr || strchr(" (,<\"", *(cleanStart-1))) && 
                strchr(" (,<\"", *(cleanStart+5)))
            {
                cleanStart += strcspn(cleanStart, " (,");
                *cleanStart = '\0';
                TokenizerEscape(escapebuf, cleanStart+1);
                estrPrintf(&buf, "%s( %s )", estr, escapebuf);
                tok = TokenizerCreateString(buf, estrLength(&buf));
                break;
            }
            else
                cleanStart += 5;
        }

        if(buf && buf[0])
            tok = TokenizerCreateString(buf, estrLength(&buf));
        else
            tok = TokenizerCreateString(estr, estrLength(&estr));


        parsed->string = NULL;

        TokenizerParseList(tok, ParseBehaviorString, parsed, TokenizerErrorCallback);

        TokenizerDestroy(tok);

        stashAddPointer(BehaviorParses, allocedStr, parsed, false);

        return parsed;
    }
}

//void aiBehaviorDestroyParsedString(AIBParsedString* parsed)
//{
//    int i;
//
//    for(i = eaSize(&parsed->string)-1; i >=0; i--)
//        ParserFreeFunctionCall(parsed->string[i]);
//    eaDestroy(&parsed->string);
//}

void aiBehaviorAddFlag(Entity* e, AIVarsBase* aibase, int handle, const char* string)
{
    AIBehavior* behavior = NULL;
    AIBehaviorMod* mod = aiBehaviorModCreate();
    //     int len;
    static char* buf = NULL;

    if(handle)
    {
        behavior = aiBehaviorGetFromHandle(e, aibase, handle);

        mod->behavior = behavior;
        mod->string = allocAddString(string);
    }
    else
    {
        estrPrintf(&buf, "Flag(%s)", string);
        mod->string = allocAddString(buf);
    }

    if(processingBehaviors)
        eaPush(&aibase->behaviorMods, mod);
    else
    {
        eaPush(&aibase->behaviorPrevMods, mod);
        aiBehaviorProcessFlag(e, aibase, &aibase->behaviors, behavior, string);
    }
}

void aiBehaviorAddStringEx(Entity* e, AIVarsBase* aibase, const char* string, bool uninterruptable)
{
    AIBehaviorMod* mod;

    if(!string || !string[0] || !e || !aibase)
        return;

    mod = aiBehaviorModCreate();

    devassertmsg(e && aibase, "You can't call behavior add string without an entity that has an aibase");

    mod->string = allocAddString(string);
    //     strncpy(mod->string, string, AI_BEHAVIOR_MAX_STRING);
    // 
    //     devassertmsg(strlen(string)+1 < AI_BEHAVIOR_MAX_STRING, "Passed a string over AI_BEHAVIOR_MAX_STRING into aiBehaviorAddString");

    if(processingBehaviors)
        eaPush(&aibase->behaviorMods, mod);
    else
    {
        AIBParsedString* parsed;
        AI_LOG(AI_LOG_BEHAVIOR, (e, "ProcessString: Processing ^9%s^0\n", string));
        parsed = aiBehaviorParseString(string);
        //         aiBehaviorDebugVerifyParse(string, parsed);
        eaPush(&aibase->behaviorPrevMods, mod);
        aiBehaviorProcessString(e, aibase, &aibase->behaviors, string, parsed, uninterruptable);
        //aiBehaviorDestroyParsedString(&parsed);
    }
}

void aiBehaviorAddString(Entity* e, AIVarsBase* aibase, const char* string)
{
    aiBehaviorAddStringEx(e, aibase, string, false);
}

int aiBehaviorProcessAllStrings(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors)
{
    while(eaSize(&aibase->behaviorMods))
    {
        AIBehaviorMod* mod = aibase->behaviorMods[0];

        if(mod->behavior)
        {
            int i;
            bool processed = false;

            for(i = eaSize(behaviors) - 1; i >= 0; i--)
            {
                AIBehavior* behavior = (*behaviors)[i];
                if(behavior == mod->behavior)
                {
                    aiBehaviorProcessFlag(e, aibase, behaviors, mod->behavior, mod->string);
                    processed = true;
                }
            }

            if(!processed && &aibase->behaviors != behaviors)
                return false;    // if this is true, an alias caused a string to get added to a specific
                                // behavior that is not inside the alias, so we should not delete it
                                // and see if the specified behavior is on the top level stack (like the
                                // alias behavior probably is
        }
        else
        {
            AIBParsedString* parsed;

            AI_LOG(AI_LOG_BEHAVIOR, (e, "ProcessString: Processing ^9%s^0\n", mod->string));
            parsed = aiBehaviorParseString(mod->string);
            //             aiBehaviorDebugVerifyParse(mod->string, parsed);
            aiBehaviorProcessString(e, aibase, behaviors, mod->string, parsed, false);
            //aiBehaviorDestroyParsedString(&parsed);
        }

        eaPush(&aibase->behaviorPrevMods, (AIBehaviorMod*)eaRemove(&aibase->behaviorMods, 0));

        if(eaSize(&aibase->behaviorPrevMods) > AI_BEHAVIOR_MAX_MODS_PREV)
            aiBehaviorModDestroy((AIBehaviorMod*)eaRemove(&aibase->behaviorPrevMods, 0));
    }

    return true;
}

void aiBehaviorParseError(Entity* e, const char* explanation, const char* badstr, const char* fullstr)
{
    const char* blameFile = NULL; // TODO: add blamefile callback
    static char* buf = NULL;

    estrPrintf(&buf, "%s: %s", explanation, badstr);
    if(fullstr)
        estrConcatf(&buf, " in string: %s", fullstr);

    if(blameFileCallback)
        blameFile = blameFileCallback(e);

    if(blameFile)
        ErrorFilenamef(blameFile, "%s", buf);
    else
        Errorf("%s", buf);
}

void aiBehaviorParseFlags(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior, TokenizerFunctionCall** tokens)
{
    int i;

    for(i = 0; i < eaSize(&tokens); i++)
    {
        if(!aiBehaviorFlagProcessString(e, aibase, behaviors, behavior, tokens[i]->function, tokens[i]))
            aiBehaviorParseError(e, "Unrecognized token in behavior string:", tokens[i]->function, behavior->originalString);
    }
}

F32 aiBehaviorParseFloat(TokenizerFunctionCall* param)
{
    if(!stricmp(param->function, "Rand"))
    {
        F32 min, max;
        F32 rand;
        if(eaSize(&param->params) != 2)
            Errorf("Invalid Rand: should have two parameters");

        min = atof(param->params[0]->function);
        max = atof(param->params[1]->function);

        if(min > max)
        {
            Errorf("Invalid Rand: are you sure you meant (%.1f,%.1f)?", min, max);
            return 0;
        }

        rand = randomF32();
        return ABS(rand) * (max - min) + min;
    }

    return atof(param->function);
}

int aiBehaviorParseInt(TokenizerFunctionCall* param)
{
    if(!stricmp(param->function, "Rand"))
    {
        int min, max;
        if(eaSize(&param->params) != 2)
            Errorf("Invalid Rand: should have two parameters");

        min = atoi(param->params[0]->function);
        max = atoi(param->params[1]->function);

        if(min >= max)
        {
            Errorf("Invalid Rand: are you sure you meant (%d,%d)?", min, max);
            return 0;
        }

        return randomU32() % (max+1 - min) + min;
    }

    return atoi(param->function);
}

void aiBehaviorProcessString(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, const char* origStr, AIBParsedString* parsed, bool uninterruptable)
{
    int i, highestIdx = eaSize(&parsed->string);

    INITIALIZE_SPECIAL_INFO_IF_NEEDED;

    AI_LOG(AI_LOG_BEHAVIOR, (e, "ProcessString: Processing ^9%s^0\n", aiBehaviorDebugPrintParse(parsed)));

    aiBehaviorUpdateAll(e, aibase, behaviors);

    for(i = highestIdx-1; i >= 0; i--)
    {
        char* processedStr = NULL;
        int addSelfAsFlag = 0, processEntry = 0;
        AIBTableEntry* entry;

        if(stringPreprocessCallback)
            stringPreprocessCallback(e, aibase, behaviors, parsed->string[i], &processedStr, &addSelfAsFlag, &processEntry);

        if(!processEntry)
            continue;

        entry = aiBehaviorLookUpString(processedStr);

        if(!entry)
            aiBehaviorParseError(e, "Unrecognized token in behavior string:", parsed->string[i]->function, origStr);
        else if(entry->type == AIB_FLAG)
        {
            // add flag to push permflag
            AIBehavior* delayedPermVar = aiBehaviorPush(e, aibase, behaviors, delayedPermVarBehaviorInfo);
            aiBehaviorFlagProcessString(e, aibase, behaviors, delayedPermVar,
                parsed->string[i]->function, parsed->string[i]);
        }
        else if(entry->type == AIB_ALIAS)
        {
            // create new alias behavior
            // add string as a flag if "addSelfAsFlag"
            // add flags
            AIBehavior* aliasBehavior = aiBehaviorPush(e, aibase, behaviors, aliasBehaviorInfo);
            if(addSelfAsFlag)
                aiBehaviorFlagProcessString(e, aibase, behaviors, aliasBehavior, processedStr, parsed->string[i]);
            aiBehaviorParseFlags(e, aibase, behaviors, aliasBehavior, parsed->string[i]->params);

            aliasBehavior->originalString = allocAddString(origStr);
            aliasBehavior->uninterruptable = uninterruptable ? 1 : 0;
        }
        else if(entry->type == AIB_BEHAVIOR && entry->behaviorInfo == flagBehaviorInfo)
        {
            // add to top behavior on normal behavior stack
            aiBehaviorParseFlags(e, aibase, behaviors, (*behaviors)[eaSize(behaviors)-1],
                parsed->string[i]->params);
        }
        else if(entry->type == AIB_BEHAVIOR)
        {
            // add behavior
            // add flags
            AIBehavior* newbehavior = aiBehaviorPush(e, aibase, behaviors, entry->behaviorInfo);
            aiBehaviorParseFlags(e, aibase, behaviors, newbehavior, parsed->string[i]->params);

            newbehavior->originalString = allocAddString(origStr);
            newbehavior->uninterruptable = uninterruptable ? 1 : 0;
        }
        else if(entry->type == AIB_CUSTOM)
        {
            devassertmsg(behaviorCustomTableEntryCallback, "Need to specify a Custom TableEntry"
                " Callback for Behavior custom keywords to work");

            behaviorCustomTableEntryCallback(e, aibase, behaviors, entry, origStr, processedStr, parsed->string[i]);
        }
        else
            Errorf("Internal behavior system error, please get Raoul (while processing: %s)", origStr);
    }
    aiBehaviorUpdateAll(e, aibase, behaviors);
    // freeing the parsed string is now done in a separate function because this function gets
    // parsed aliases passed into it that shouldn't be freed
}

MP_DEFINE(AIBehaviorFlag);

AIBehaviorFlag* aiBehaviorFlagCreate(Entity* e, AIVarsBase* aibase)
{
    aibase->behaviorFlagsAllocated++;
    assert(aibase->behaviorFlagsAllocated > 0);

    MP_CREATE(AIBehaviorFlag, 100);
    return MP_ALLOC(AIBehaviorFlag);
}

void aiBehaviorFlagDestroy(Entity* e, AIVarsBase* aibase, AIBehaviorFlag* flag)
{
    assert(aibase->behaviorFlagsAllocated > 0);
    aibase->behaviorFlagsAllocated--;

    switch(flag->info->datatype)
    {
        xcase FLAG_DATA_ESTR:
    estrDestroy(&flag->strptr);
    xcase FLAG_DATA_MULT_STR:
    eaDestroy(&flag->strarray);
    }

    MP_FREE(AIBehaviorFlag, flag);
}

int aiBehaviorFlagAddInternal(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior, AIBehaviorFlagInfo* info, TokenizerFunctionCall* param)
{
    static char* estr = NULL;
    AIBehaviorFlag* flag;
    int i;
    bool processed = false;

    if(!behavior || !info)
        return 0;

    estrClear(&estr);
    flag = aiBehaviorFlagCreate(e, aibase);
    flag->info = info;

    // allow flag specific processing of the data
    if(flag->info->datatype == FLAG_DATA_SPECIAL && specialFlagProcessingCallback)
        processed = specialFlagProcessingCallback(e, aibase, behaviors, behavior, flag, param, &estr);

    if(!processed)
    {
        switch(flag->info->datatype)
        {
        xcase FLAG_DATA_ALLOCSTR:
            WRONG_PARAMETERS(1, "(supposed to be 1 string)");
            flag->allocstr = allocAddString(param->params[0]->function);
            estrConcatf(&estr, "(%s)", flag->strptr);
        xcase FLAG_DATA_BOOL:
            if(!eaSize(&param->params))
                flag->intarray[0] = 1;
            else if(eaSize(&param->params) == 1)
            {
                flag->intarray[0] = atoi(param->params[0]->function) ? 1 : 0;
                estrConcatf(&estr, "(%d)", flag->intarray[0]);
            }
            else
            {
                ERRORF_DESTROY("(supposed to have none (for a default of 1) or a bool)");
            }
        xcase FLAG_DATA_ENTREF:
            WRONG_PARAMETERS(1, "(supposed to be 1 float)");
            sscanf(param->params[0]->function, "%I64x", &flag->entref);
            // TODO: figure out what's going to happen to entrefs
    //         if(!erGetEnt(flag->entref))
    //             return false;

            estrConcatf(&estr, "(%s^0:^s^4%s^0^n)",
                AI_LOG_ENT_NAME(erGetEnt(flag->entref)), "TODO: get entity's owner through lib");//erGetEnt(flag->entref)->owner);
        xcase FLAG_DATA_ESTR:
            WRONG_PARAMETERS(1, "(supposed to be 1 string)");
            estrPrintCharString(&flag->strptr, param->params[0]->function);
            estrConcatf(&estr, "(%s)", flag->strptr);
        xcase FLAG_DATA_FLOAT:
            devassertmsg(
                eaSize(&param->params),
                "Wrong number of parameters: ParamCount= %d, FlagName= %s, FlagType= %d, FlagDataType= %d",
                eaSize(&param->params),
                flag->info->name ? flag->info->name : "",
                flag->info->type,
                flag->info->datatype);
            if (eaSize(&param->params) == 0 ||
                eaSize(&param->params) > 1 && stricmp(param->params[0]->function, "rand"))
            {
                Errorf("Wrong number of parameters to %s: %d %s", flag->info->name,
                    eaSize(&param->params), "(supposed to be 1 float)");
                aiBehaviorFlagDestroy(e, aibase, flag);
                return false;
            }
            flag->floatarray[0] = aiBehaviorParseFloat(param->params[0]);
            estrConcatf(&estr, "(^4%.1f^0)", flag->floatarray[0]);
        xcase FLAG_DATA_INT:
            WRONG_PARAMETERS(1, "(supposed to be 1 int)");
            flag->intarray[0] = aiBehaviorParseInt(param->params[0]);
            estrConcatf(&estr, "(^4%i^0)", flag->intarray[0]);
        xcase FLAG_DATA_INT3:
            WRONG_PARAMETERS(3, "(supposed to be 3 ints)");
            flag->intarray[0] = aiBehaviorParseInt(param->params[0]);
            flag->intarray[1] = aiBehaviorParseInt(param->params[1]);
            flag->intarray[2] = aiBehaviorParseInt(param->params[2]);;
            estrConcatf(&estr, "(^4%i^0,^4%i^0,^4%i^0)",
                flag->intarray[0], flag->intarray[1], flag->intarray[2]);
        xcase FLAG_DATA_MULT_STR:
            estrConcatChar(&estr, '(');
            for(i = 0; i < eaSize(&param->params); i++)
            {
                eaPushConst(&flag->strarray, allocAddString(param->params[i]->function));
            }
            estrConcatChar(&estr, ')');
        xcase FLAG_DATA_PARSE:
            WRONG_PARAMETERS(1, "(supposed to be a string)");
            flag->intarray[0] =
                StaticDefineIntGetInt(flag->info->parseTable, param->params[0]->function);
            if(flag->intarray[0] == -1)
            {
                Errorf("Invalid parameter to %s: %s (on svr idx %d)", flag->info->name,
                    param->params[0]->function, -1); // TODO: fill in ent owner equivalent
                aiBehaviorFlagDestroy(e, aibase, flag);
                return 0;
            }
            estrPrintf(&estr, "(%s)", param->params[0]->function);
        xcase FLAG_DATA_PERCENT:
            WRONG_PARAMETERS(1, "(supposed to be 1 float between 0 and 100)");
            flag->floatarray[0] = aiBehaviorParseFloat(param->params[0]);
            if(flag->floatarray[0] < 0 || flag->floatarray[0] > 100)
            {
                ERRORF_DESTROY("(supposed to be 1 float between 0 and 100)");
            }
            estrConcatf(&estr, "(^4%.1f^0)", flag->floatarray[0]);
        xcase FLAG_DATA_VEC3:
            if( eaSize(&param->params) == 1 && strchr( param->params[0]->function, ':') )
            {
                // TODO: readd script location handling
                Errorf("Reimplement script location through vec3 flags later if necessary");
                //GetPointFromLocation( param->params[0]->function, flag->floatarray );
            }
            else
            {
                WRONG_PARAMETERS(3, "(supposed to be 3 floats)");
                flag->floatarray[0] = aiBehaviorParseFloat(param->params[0]);
                flag->floatarray[1] = aiBehaviorParseFloat(param->params[1]);
                flag->floatarray[2] = aiBehaviorParseFloat(param->params[2]);
            }
            estrConcatf(&estr, "(^4%0.1f^0, ^4%0.1f^0, ^4%0.1f^0)",
                flag->floatarray[0], flag->floatarray[1], flag->floatarray[2]);
        xdefault:
            Errorf("Flag %s is using unhandled data type!!", param->function);
        }
    }

    if(!flag->info->allowDuplicates)
    {
        for(i = eaSize(&behavior->flags)-1; i >= 0; i--)
        {
            if(behavior->flags[i]->info == flag->info)
            {
                aiBehaviorFlagDestroy(e, aibase, eaRemove(&behavior->flags, i));
                AI_LOG(AI_LOG_BEHAVIOR, (e, "Removed flag ^8%s^0%s from ^9%s^2%i^0 (^1replaced^0).\n",
                    flag->info->name,
                    estr,
                    behavior->info->name,
                    aiBehaviorGetHandle(e, aibase, behaviors, behavior)));
            }
        }
    }

    eaPush(&behavior->flags, flag);
    behavior->dirty = 1;
    AI_LOG(AI_LOG_BEHAVIOR, (e, "Adding flag ^8%s^0%s to ^9%s^2%i^0.\n",
        flag->info->name,
        estr,
        behavior->info->name,
        aiBehaviorGetHandle(e, aibase, behaviors, behavior)));

    return true;
}

void aiBehaviorAddPermFlag(Entity* e, AIVarsBase* aibase, const char* string)
{
    if(!e || !aibase || !string || !string[0])
        return;

    aiBehaviorCreatePERMVAR(e, aibase, &aibase->behaviors);

    if (!aibase->behaviors)
        return;

    aiBehaviorProcessFlag(e, aibase, &aibase->behaviors, aibase->behaviors[0], string);
}

int aiBehaviorFlagProcessString(Entity* e, AIVarsBase* aibase, AIBehavior*** behaviors, AIBehavior* behavior, char* processedStr, TokenizerFunctionCall* parsedflag)
{
    AIBehaviorFlagInfo* flagInfo = aiBehaviorFlagInfoFromString(processedStr);

    if(flagInfo)
        aiBehaviorFlagAddInternal(e, aibase, behaviors, behavior, flagInfo, parsedflag);
    else if(!strnicmp(processedStr, "location:", strlen("location:")) ||
        !strnicmp(processedStr, "marker:", strlen("marker:")) ||
        !strnicmp(processedStr, "coord:", strlen("coord:")) ||
        !strnicmp(processedStr, "missionroom:", strlen("missionroom:")) ||
        !strnicmp(processedStr, "trigger:", strlen("trigger:")) ||
        !strnicmp(processedStr, "encountergroup:", strlen("encountergroup:")) ||
        !strnicmp(processedStr, "ent:", strlen("ent:")) ||
        !strnicmp(processedStr, "objective:", strlen("objective:")) ||
        !strnicmp(processedStr, "missionencounter:", strlen("missionencounter:")) ||
        !strnicmp(processedStr, "spawndef:", strlen("spawndef:")) ||
        !strnicmp(processedStr, "encounterid:", strlen("encounterid:")) || 
        !strnicmp(processedStr, "random:player", strlen("random:player"))  || 
        !strnicmp(processedStr, "door:", strlen("door:")) )
    {
        static AIBehaviorFlagInfo* scriptLocInfo = NULL;

        if(!scriptLocInfo)
        {
            scriptLocInfo = aiBehaviorFlagInfoFromString("ScriptLocation");
            if(!scriptLocInfo)
                Errorf("Passed what seems to be a scriptlocation(%s), but there is no script location flag", parsedflag->function);
        }

        if(scriptLocInfo)
            aiBehaviorFlagAddInternal(e, aibase, behaviors, behavior, scriptLocInfo, parsedflag);
        else
            return 0;
    }
    else if(behavior->info->stringFunc &&
        !behavior->info->stringFunc(e, aibase, behavior, parsedflag))
        return 0;

    return 1;
}

static int gotDataSizes = 0;
static U32 maxBehaviorDataSize = 0;
static U32 maxBehaviorTeamDataSize = 0;

int GetMaxDataSizeHelper(StashElement element)
{
    AIBTableEntry* entry = (AIBTableEntry*)stashElementGetPointer(element);

    if(entry->type == AIB_BEHAVIOR)
    {
        if(entry->behaviorInfo->structSize > maxBehaviorDataSize)
            maxBehaviorDataSize = entry->behaviorInfo->structSize;
        if(entry->behaviorInfo->teamDataStructSize > maxBehaviorTeamDataSize)
            maxBehaviorTeamDataSize = entry->behaviorInfo->teamDataStructSize;
    }
    return 1;
}

int aiBehaviorGetMaxDataSize()
{
    if(!gotDataSizes)
    {
        stashForEachElement(BehaviorSystemHashTable, GetMaxDataSizeHelper);
        gotDataSizes = 1;
    }
    return maxBehaviorDataSize;
}

int aiBehaviorGetMaxTeamDataSize()
{
    if(!gotDataSizes)
    {
        stashForEachElement(BehaviorSystemHashTable, GetMaxDataSizeHelper);
        gotDataSizes = 1;
    }
    return maxBehaviorTeamDataSize;
}

void aiBehaviorReinitEntity(Entity* e, AIVarsBase* aibase)
{
    behaviorReinitCallback(e, aibase);
    aiBehaviorSetAllVars(e, aibase, &aibase->behaviors);
}

//////////////////////////////////////////////////////////////////////////
// Callback Management
//////////////////////////////////////////////////////////////////////////

void aiBehaviorSetBehaviorReloadCallback(aiBehaviorReloadCallback callback)
{
    behaviorReloadCallback = callback;
}

void aiBehaviorSetBehaviorFlagReloadCallback(aiBehaviorFlagReloadCallback callback)
{
    behaviorFlagReloadCallback = callback;
}

void aiBehaviorSetBehaviorAliasReloadCallback(aiBehaviorAliasReloadCallback callback)
{
    behaviorAliasReloadCallback = callback;
}

void aiBehaviorSetBehaviorTableCustomReloadCallback(aiBehaviorTableCustomReloadCallback callback)
{
    behaviorTableCustomReloadCallback = callback;
}

void aiBehaviorSetBehaviorCustomTableEntryCallback(aiBehaviorCustomTableEntryCallback callback)
{
    behaviorCustomTableEntryCallback = callback;
}

void aiBehaviorSetBlameFileCallback(aiBehaviorBlameFileCallback callback)
{
    blameFileCallback = callback;
}

void aiBehaviorSetStringPreprocessCallback(aiBehaviorStringPreprocessCallback callback)
{
    stringPreprocessCallback = callback;
}

void aiBehaviorSetSpecialFlagProcessingCallback(aiBehaviorSpecialFlagProcessingCallback callback)
{
    specialFlagProcessingCallback = callback;
}

void aiBehaviorSetFlagProcessCallback(aiBehaviorFlagProcessCallback callback)
{
    flagProcessCallback = callback;
}

void aiBehaviorSetDefaultBehaviorCallback(aiBehaviorDefaultBehaviorCallback callback)
{
    defaultBehaviorCallback = callback;
}

void aiBehaviorSetReinitCallback(aiBehaviorReinitCallback callback)
{
    behaviorReinitCallback = callback;
}

void aiBehaviorSetCheckCombatOverrideCallback(aiBehaviorCheckCombatOverrideCallback callback)
{
    behaviorCheckCombatOverrideCallback = callback;
}

void aiBehaviorSetConditionParseCallback(aiBehaviorConditionParseCallback callback)
{
    behaviorConditionParseCallback = callback;
}

void aiBehaviorSetConditionValueCallback(aiBehaviorConditionValueCallback callback)
{
    behaviorConditionValueCallback = callback;
}

void aiBehaviorSetGetABS_TIMECallback(aiBehaviorGetABS_TIMECallback callback)
{
    getABS_TIMECallback = callback;
}