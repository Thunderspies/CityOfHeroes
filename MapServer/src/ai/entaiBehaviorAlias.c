#include <utilitieslib/stdtypes.h>
#include "entaiBehaviorAlias.h"
#include "ailib/aiBehaviorInterface.h"
#include "ailib/aiBehaviorPublic.h"

#include "cmdparse/cmdserver.h"
#include <utilitieslib/components/earray.h>
#include "ai/entai.h"
#include "entity/entity.h"
#include "entity/entserver.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/StashTable.h>
#include "gameComm/VillainDef.h"
#include <utilitieslib/components/SharedMemory.h>

typedef struct AIBehaviorAlias{
    char* aliasStr;
    char* resolveStr;
    char* filename;
}AIBehaviorAlias;

AIBehaviorAlias** DebugAliasList = NULL;

typedef struct AllBehaviorAliases {
    AIBehaviorAlias** aliases;
}AllBehaviorAliases;

AllBehaviorAliases allAliases = {0};

static TokenizerParseInfo parseBehaviorAlias[] = {
    { "",                    TOK_STRUCTPARAM|TOK_STRING(AIBehaviorAlias,aliasStr, 0)        },
    { "",                    TOK_STRUCTPARAM|TOK_STRING(AIBehaviorAlias,resolveStr, 0)    },
    { "",                    TOK_CURRENTFILE(AIBehaviorAlias,filename)        },
    //--------------------------------------------------------------------------
    { "\n",                    TOK_END,            0 },
    //--------------------------------------------------------------------------
    { "", 0, 0 }
};

static TokenizerParseInfo parseAllBehaviorAliases[] = {
    { "Alias:",        TOK_STRUCT(AllBehaviorAliases,aliases,parseBehaviorAlias) },
    { "", 0, 0 }
};

static StashTable BehaviorAliasTable = 0;
static Entity* testEnt = NULL;
static AIBehaviorAliasInfo** BehaviorAliasList = NULL;

void aiBehaviorAliasInfoDestroy(AIBehaviorAliasInfo* info)
{
    eaDestroyEx(&info->parsedStr->string, ParserFreeFunctionCall);
    SAFE_FREE(info);
}

int cmpBehaviorAlias(const AIBehaviorAlias** l, const AIBehaviorAlias** r)
{
    const AIBehaviorAlias* lhs = *l;
    const AIBehaviorAlias* rhs = *r;
    return stricmp(lhs->aliasStr, rhs->aliasStr);
}

void aiBehaviorProcessAliases(void)
{
    int i;
    int count;

    g_TestingBehaviorAliases = true;
    count = eaSize(&allAliases.aliases);
    for(i = 0; i < count; i++)
    {
        AIBParsedString* parsed;
        AIVars* ai;
        AIBehaviorAliasInfo* info = (AIBehaviorAliasInfo*)malloc(sizeof(AIBehaviorAliasInfo));

        parsed = aiBehaviorParseString(allAliases.aliases[i]->resolveStr);

        if(isDevelopmentMode() && !server_state.levelEditor && sharedMemoryGetMode() == SMM_DISABLED)
        {
            if(!testEnt)
            {
                testEnt = villainCreateByName("hellions_brawl_thug", 1, NULL, false, NULL, 0, NULL);
                SET_ENTTYPE(testEnt) = ENTTYPE_CRITTER;
                testEnt->fade = 1;

                aiInit(testEnt, NULL);
            }
            else
                aiBehaviorMarkAllFinished(testEnt, ENTAI(testEnt)->base, true);

            ai = ENTAI(testEnt);

            aiBehaviorProcessString(testEnt, ai->base, &ai->base->behaviors,
                allAliases.aliases[i]->resolveStr, parsed, false);
        }

        info->name = allAliases.aliases[i]->aliasStr;
        info->resolveStr = allAliases.aliases[i]->resolveStr;
        info->parsedStr = parsed;

        aiBehaviorTableAddAlias(info);
        eaPush(&BehaviorAliasList, info); //allAliases.aliases[i]
    }

    aiBehaviorSortDebugAliasTable();

    if(testEnt)
    {
        entFree(testEnt);
        testEnt = NULL;
    }
    g_TestingBehaviorAliases = false;
}

static void aiBehaviorAliasInfoDestroyAdapter(void* arg0)
{
    aiBehaviorAliasInfoDestroy((AIBehaviorAliasInfo*)arg0);
}

static void aiBehaviorReloadAliasCallback(const char* relpath, int when)
{
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    if(ParserReloadFile(relpath, parseAllBehaviorAliases, sizeof(AIBehaviorAlias), &allAliases, NULL, NULL))
    {
        eaClearEx(&BehaviorAliasList, aiBehaviorAliasInfoDestroyAdapter);
        aiBehaviorRebuildLookupTable();
    }
    else
        Errorf("Error reloading Behavior Aliases (%s)", relpath);
}

void aiBehaviorLoadAliases()
{
    g_TestingBehaviorAliases = true;

    ParserLoadFiles("AIScript", ".bal", "behavioralias.bin", 0, parseAllBehaviorAliases, &allAliases, NULL, NULL, NULL);

    aiBehaviorProcessAliases();
    aiBehaviorSetBehaviorAliasReloadCallback(aiBehaviorProcessAliases);

    // set up callback
    if(isDevelopmentMode())
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "AIScript/*.bal", aiBehaviorReloadAliasCallback);
}