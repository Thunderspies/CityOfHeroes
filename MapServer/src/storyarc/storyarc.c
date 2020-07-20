/*\
 *
 *    storyarc.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    storyarc static definitions, and storyarc functions
 *       - storyarcs are a way of linking tasks and encounters
 *        together.  they accomplish this by hooking into those
 *        systems and replacing them
 *
 */

#include "storyarc.h"
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/utils/cmdoldparse.h>
#include "storyarcprivate.h"
#include "entity/character_level.h"
#include "entity/character_eval.h"
#include "reward.h"
#include "svr/svr_player.h"
#include "container/team.h"
#include "TeamReward.h"
#include "container/containerloadsave.h"
#include "dbcomm/staticMapInfo.h"
#include "entity/entity.h"
#include "dbcomm/dbnamecache.h"
#include "comm_game.h"
#include "entity/supergroup.h"
#include "entity/sgrpserver.h"
#include "gameComm/svr_chat.h"
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/fileutil.h>
#include "cmdparse/cmdserver.h"
#include "entity/TaskForceParams.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "player/badges_server.h"
#include "playerCreatedStoryarcServer.h"
#include "storyarc/pnpcCommon.h"
#include "taskforce.h"
#include "zowie.h"
#include "dbcomm/logcomm.h"
#include "entity/character_animfx.h"
#include "entity/motion.h"
#include "entity/character_eval.h"
// story arc probabilities
#define SA_CHANCE_GIVEFIRSTARC        50    // percentage chance a player will be put on an arc if he doesn't have one
#define SA_CHANCE_GIVESECONDARC        27    // percentage chance a player will be put on an arc if already on one
#define SA_CHANCE_REPLACEENCOUTER    100 // percentage chance a random encounter will be replaced by story arc if possible
#define SA_CHANCE_REPLACETASK        100    // percentage chance a task will be replaced by story arc if possible

// *********************************************************************************
//  Story arc defs
// *********************************************************************************

TokenizerParseInfo ParseStoryClue[] = {
    { "", TOK_STRUCTPARAM | TOK_NO_TRANSLATE | TOK_STRING(StoryClue,name,0) },
    { "",                TOK_CURRENTFILE(StoryClue,filename) },
    { "{",                TOK_START,        0},

    // strings
    { "Name",            TOK_STRING(StoryClue,displayname,0) },
    { "DetailString",    TOK_STRING(StoryClue,detailtext,0) },
    { "Icon",            TOK_FILENAME(StoryClue,iconfile,0) },

    // is this required?
    { "IntroString",    TOK_STRING(StoryClue,introstring,0) },

    // Semi deprecated, used with the mission editor
    { "ClueName",        TOK_REDUNDANTNAME | TOK_STRING(StoryClue,name,0) },
    { "Display",        TOK_REDUNDANTNAME | TOK_STRING(StoryClue,displayname,0) },

    // deprecated
    { "Detail",            TOK_REDUNDANTNAME | TOK_STRING(StoryClue,detailtext,0)    },
    { "}",                TOK_END,            0},
    { "", 0, 0 }
};

TokenizerParseInfo ParseStoryEpisode[] = {
    { "{",                TOK_START,        0},
    { "TaskDef",        TOK_STRUCT(StoryEpisode,tasks,ParseStoryTask) },
    { "ReturnSuccess",    TOK_STRUCT(StoryEpisode,success,ParseStoryReward) },

    // deprecated stuff
    { "Success",        TOK_REDUNDANTNAME | TOK_STRUCT(StoryEpisode,success,ParseStoryReward) },
    { "}",                TOK_END,        0},
    { "", 0, 0 }
};

StaticDefineInt ParseStoryArcFlag[] = {
    DEFINE_INT
    { "Deprecated",                STORYARC_DEPRECATED            },
    { "MiniStoryarc",            STORYARC_MINI                },
    { "Repeatable",                STORYARC_REPEATABLE            },
    { "NoFlashback",            STORYARC_NOFLASHBACK        },
    { "ScalableTf",                STORYARC_SCALABLE_TF        },
    { "FlashbackOnly",            STORYARC_FLASHBACK_ONLY        },
    DEFINE_END
};

StaticDefineInt ParseStoryArcAlliance[] = {
    DEFINE_INT
    { "Both",                    SA_ALLIANCE_BOTH            },
    { "Hero",                    SA_ALLIANCE_HERO            },
    { "Villain",                SA_ALLIANCE_VILLAIN            },
    DEFINE_END
};

TokenizerParseInfo ParseStoryArc[] = {
    { "{",                                TOK_START,        0 },
    { "",                                TOK_CURRENTFILE(StoryArc,filename) },
    { "Deprecated",                        TOK_BOOLFLAG(StoryArc,deprecated,0), },
    { "Flags",                            TOK_FLAGS(StoryArc,flags,0),                        ParseStoryArcFlag },
    { "Episode",                        TOK_STRUCT(StoryArc,episodes,ParseStoryEpisode) },
    { "ClueDef",                        TOK_STRUCT(StoryArc,clues,ParseStoryClue) },
    { "MinPlayerLevel",                    TOK_INT(StoryArc,minPlayerLevel,0)  },
    { "MaxPlayerLevel",                    TOK_INT(StoryArc,maxPlayerLevel,0)  },

    // If you add something here you probably need to add it to tasks as well to support Flashbackable tasks
    // see StoryArcPostprocessAppendFlashback
    { "CompleteRequires",                TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryArc, completeRequires) },
    { "FlashbackRequires",                TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryArc, flashbackRequires) },
    { "FlashbackRequiresFailedText",    TOK_STRING(StoryArc, flashbackRequiresFailed, 0) },
     { "FlashbackTeamRequires",            TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryArc, flashbackTeamRequires) },
     { "FlashbackTeamRequiresFailedText",TOK_STRING(StoryArc, flashbackTeamFailed, 0) },
    { "Requires",                        TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryArc, assignRequires) },
    { "RequiresFailedText",                TOK_STRING(StoryArc, assignRequiresFailed, 0) },
    { "TeamRequires",                    TOK_NO_TRANSLATE | TOK_STRINGARRAY(StoryArc, teamRequires) },
    { "TeamRequiresFailedText",            TOK_STRING(StoryArc, teamRequiresFailed, 0) },
    
    { "Name",                            TOK_STRING(StoryArc, name, 0) },
    { "Alliance",                        TOK_INT(StoryArc,alliance, SA_ALLIANCE_BOTH), ParseStoryArcAlliance },
    { "FlashbackDescription",            TOK_STRING(StoryArc, flashbackDescription, 0) },
    { "FlashbackCostMultiplier",        TOK_F32(StoryArc,flashbackCostMulitplier, 1.0) },
    { "FlashbackLeft",                    TOK_STRUCT(StoryArc, rewardFlashbackLeft, ParseStoryReward) },
    { "architectAboutContact",            TOK_STRING(StoryArc, architectAboutContact, 0) },
    { "}",                                TOK_END,        0 },
    SCRIPTVARS_STD_PARSE(StoryArc)
    { "", 0, 0 }
};

TokenizerParseInfo ParseStoryArcList[] = {
    { "StoryArcDef",    TOK_STRUCT(StoryArcList,storyarcs,ParseStoryArc) },
    { "", 0, 0 }
};

StoryArcList g_storyarclist = {0};


StringArray g_flashbacklist = NULL;
static Contact *StoryArcFindContact(StoryTaskHandle *sahandle);


StoryTaskHandle * tempStoryTaskHandle( int context, int sub, int cpos, int player_created)
{  
    static StoryTaskHandle sa = {0};
    sa.context = context;
    sa.subhandle = sub;
    sa.compoundPos = cpos;
    sa.bPlayerCreated = player_created;
    return &sa;
}

void StoryHandleSet(StoryTaskHandle *sahandle, int context, int subhandle, int cpos, bool playerCreated)
{
    sahandle->context = context;
    sahandle->subhandle = subhandle;
    sahandle->compoundPos = cpos;
    sahandle->bPlayerCreated = playerCreated;
}

void StoryHandleCopy(StoryTaskHandle *dst, StoryTaskHandle *src)
{
    memcpy(dst, src, sizeof(StoryTaskHandle)); 
}


void StoryArcFlashbackMission(StoryTask* task)
{

    if (g_flashbacklist == NULL)
        eaCreate(&g_flashbacklist);

    eaPush(&g_flashbacklist, strdup(task->logicalname));
}

int StoryArcProcessAndCheckLevels(StoryArc* story, int minlevel, int maxlevel)
{
    int ep, epnum;
    int ta, tanum;
    int ret = 1;

    // if my level is further restricted, this is ok
    if (story->minPlayerLevel > minlevel)
        minlevel = story->minPlayerLevel;

    epnum = eaSize(&story->episodes);
    for (ep = 0; ep < epnum; ep++)
    {
        StoryEpisode* episode = story->episodes[ep];
        tanum = eaSize(&episode->tasks);
        for (ta = 0; ta < tanum; ta++)
        {
            if (!TaskProcessAndCheckLevels(episode->tasks[ta], story->filename, minlevel, maxlevel))
                ret = 0;
        }
    }
    return ret;
}

bool StoryArcPostprocessAppendFlashback(TokenizerParseInfo pti[], StoryArcList *sal)
{
    int i, count = eaSize(&g_flashbacklist);

    if (sal->storyarcs != NULL)
    {
        // go through our list of flash backs
        for (i = 0; i < count; i++)
        {    
            const StoryTask *pTask = TaskDefFromLogicalName(g_flashbacklist[i]);

            // create a stub storyarc
            StoryTask *pTaskClone = TaskDefStructCopy(pTask);
            StoryEpisode *pEpisode = StructAllocRaw(sizeof(StoryEpisode));
            StoryArc *pStoryArc = StructAllocRaw(sizeof(StoryArc));

            memset(pEpisode, 0, sizeof(StoryEpisode));
            memset(pStoryArc, 0, sizeof(StoryArc));

            eaCreate(&pEpisode->tasks);
            eaPush(&pEpisode->tasks, pTaskClone);

            pStoryArc->filename = StructAllocString(pTask->logicalname);
            if (pTask->taskTitle)
                pStoryArc->name = StructAllocString(pTask->taskTitle);
            if (pTask->flashbackDescription)
                pStoryArc->flashbackDescription = StructAllocString(pTask->flashbackDescription);
            if (pTask->flashbackRequiresFailed)
                pStoryArc->flashbackRequiresFailed = StructAllocString(pTask->flashbackRequiresFailed);
            if (pTask->flashbackTeamFailed)
                pStoryArc->flashbackTeamFailed = StructAllocString(pTask->flashbackTeamFailed);
            if (pTask->assignRequiresFailed)
                pStoryArc->assignRequiresFailed = StructAllocString(pTask->assignRequiresFailed);
            if (pTask->teamRequiresFailed)
                pStoryArc->teamRequiresFailed = StructAllocString(pTask->teamRequiresFailed);

            if (pTask->completeRequires)
            {
                int j, stringCount = eaSize(&pTask->completeRequires);
                eaCreate(&pStoryArc->completeRequires);

                for (j = 0; j < stringCount; j++)
                {
                    eaPush(&pStoryArc->completeRequires, StructAllocString(pTask->completeRequires[j]));
                }
            }
            if (pTask->flashbackRequires)
            {
                int j, stringCount = eaSize(&pTask->flashbackRequires);
                eaCreate(&pStoryArc->flashbackRequires);

                for (j = 0; j < stringCount; j++)
                {
                    eaPush(&pStoryArc->flashbackRequires, StructAllocString(pTask->flashbackRequires[j]));
                }
            }
            if (pTask->flashbackTeamRequires)
            {
                int j, stringCount = eaSize(&pTask->flashbackTeamRequires);
                eaCreate(&pStoryArc->flashbackTeamRequires);

                for (j = 0; j < stringCount; j++)
                {
                    eaPush(&pStoryArc->flashbackTeamRequires, StructAllocString(pTask->flashbackTeamRequires[j]));
                }
            }
            if (pTask->assignRequires)
            {
                int j, stringCount = eaSize(&pTask->assignRequires);
                eaCreate(&pStoryArc->assignRequires);

                for (j = 0; j < stringCount; j++)
                {
                    eaPush(&pStoryArc->assignRequires, StructAllocString(pTask->assignRequires[j]));
                }
            }
            if (pTask->teamRequires)
            {
                int j, stringCount = eaSize(&pTask->teamRequires);
                eaCreate(&pStoryArc->teamRequires);

                for (j = 0; j < stringCount; j++)
                {
                    eaPush(&pStoryArc->teamRequires, StructAllocString(pTask->teamRequires[j]));
                }
            }

            pStoryArc->deprecated = true;
            pStoryArc->flags |= STORYARC_FLASHBACK;
            pStoryArc->minPlayerLevel = pTask->minPlayerLevel;
            pStoryArc->maxPlayerLevel = pTask->maxPlayerLevel;
            pStoryArc->alliance = pTask->alliance;
            eaCreate(&pStoryArc->episodes);
            eaPush(&pStoryArc->episodes, pEpisode);

            // add them to the list of storyarcs
            eaPush(&sal->storyarcs, pStoryArc);
        }
    }

    return true;
}

static void StoryArcValidate(StoryArc *def)
{
    int ep, epnum = eaSize(&def->episodes);
    for (ep = 0; ep < epnum; ep++)
    {
        StoryEpisode* episode = def->episodes[ep];
        int ta, tanum = eaSize(&episode->tasks);
        for (ta = 0; ta < tanum; ta++)
        {
            TaskValidate(episode->tasks[ta], def->filename);
        }
    }

    if (def->assignRequires)
        chareval_Validate(def->assignRequires, def->filename);
    if (def->completeRequires)
        chareval_Validate(def->completeRequires, def->filename);
    if (def->flashbackRequires)
        chareval_Validate(def->flashbackRequires, def->filename);
    if (def->flashbackTeamRequires)
        chareval_Validate(def->flashbackTeamRequires, def->filename);
    if (def->teamRequires)
        chareval_Validate(def->teamRequires, def->filename);
}

void StoryArcValidateAll()
{
    int sa, sanum = eaSize(&g_storyarclist.storyarcs);
    for (sa = 0; sa < sanum; sa++)
        StoryArcValidate(g_storyarclist.storyarcs[sa]);
}

static void StoryArcPostprocess(StoryArc* def)
{
    int ep, epnum = eaSize(&def->episodes);
    for (ep = 0; ep < epnum; ep++)
    {
        StoryEpisode* episode = def->episodes[ep];
        int ta, tanum = eaSize(&episode->tasks);
        for (ta = 0; ta < tanum; ta++)
        {
            episode->tasks[ta]->alliance = def->alliance;            // sa alliance overrides tasks
            episode->tasks[ta]->vs.higherscope = &def->vs;            // hook up the tasks's var scope so that it can reference vars defined in the StoryArc
            TaskPostprocess(episode->tasks[ta], def->filename);
        }
    }

    // check to see if there is an entry in the storyarc merit list
    if (!(def->flags & STORYARC_FLASHBACK) && reward_FindMeritCount(StoryArcGetLeafNameByDef(def), true, 0) < 0)
        ErrorFilenamef(def->filename, "No merit reward specified for this storyarc (%s). Please add this story arc to the merit reward file (data\\defs\\rewards\\storyarc.merits)", StoryArcGetLeafNameByDef(def));
}

bool StoryArcPostprocessAll(TokenizerParseInfo pti[], StoryArcList *sal, bool shared_memory)
{
    int sa, sanum = eaSize(&sal->storyarcs);
    for (sa = 0; sa < sanum; sa++)
        StoryArcPostprocess(sal->storyarcs[sa]);
    return 1;
}

static int StoryArcPreprocess(StoryArc* def)
{
    int ret = 1;
    int ep, epnum = eaSize(&def->episodes);
    saUtilPreprocessScripts(ParseStoryArc, def);
    for (ep = 0; ep < epnum; ep++)
    {
        StoryEpisode* episode = def->episodes[ep];
        int ta, tanum = eaSize(&episode->tasks);
        assert(eaSize(&episode->tasks) < TASKS_PER_CONTACT); // otherwise TaskMatching could overflow pickList
        for (ta = 0; ta < tanum; ta++)
        {
            if (!TaskPreprocess(episode->tasks[ta], def->filename, 1, 1))
                ret = 0;
        }
    }
    return ret;
}

bool StoryArcPreprocessAll(TokenizerParseInfo pti[], StoryArcList *sal)
{
    bool ret = true;
    int sa, sanum = eaSize(&sal->storyarcs);
    for (sa = 0; sa < sanum; sa++)
        ret &= StoryArcPreprocess(sal->storyarcs[sa]);
    return ret;
}

// Search for a recently reloaded storyarc def that still needs to be preprocessed
// Different search because the def has not yet been processed
static StoryArc* StoryArcFindUnprocessed(const char* relpath)
{
    char cleanedName[MAX_PATH];
    int i, n = eaSize(&g_storyarclist.storyarcs);
    strcpy(cleanedName, strstri((char*)relpath, SCRIPTS_DIR));
    forwardSlashes(_strupr(cleanedName));
    for (i = 0; i < n; i++)
        if (!stricmp(cleanedName, g_storyarclist.storyarcs[i]->filename))
            return g_storyarclist.storyarcs[i];
    return NULL;
}

// Reloads the latest version of a def, finds it, then processes
// MAKE SURE to do all the pre/post/etc stuff here that gets done during regular loading
static void StoryArcReloadCallback(const char* relpath, int when)
{
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    if (ParserReloadFile(relpath, ParseStoryArcList, sizeof(StoryArcList), &g_storyarclist, NULL, NULL))
    {
        StoryArc* storyarc = StoryArcFindUnprocessed(relpath);
        StoryArcPreprocess(storyarc);
        StoryArcPostprocess(storyarc);

        // If the parser reload changed the task significantly, we need to
        // force the server to restart the mission before the next server
        // tick to prevent the server from crashing.
        if (isDevelopmentMode() && OnMissionMap())
        {
            PlayerEnts * players = &player_ents[PLAYER_ENTS_ACTIVE];
            if (players->count) {
                EncounterForceReload(players->ents[0]);
            }
        }
    }
    else
    {
        Errorf("Error reloading storyarc: %s\n", relpath);
    }
}

static void StoryArcSetupCallbacks()
{
    if (!isDevelopmentMode())
        return;
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "scripts.loc/*.storyarc", StoryArcReloadCallback);
}

void StoryArcPreload()
{
    int flags = PARSER_SERVERONLY;

    if (g_storyarclist.storyarcs) {
        flags |= PARSER_FORCEREBUILD;
        if (!isSharedMemory(g_storyarclist.storyarcs)) {
            ParserDestroyStruct(ParseStoryArcList, &g_storyarclist);
        } else {
            sharedMemoryUnshare(g_storyarclist.storyarcs);
        }
        memset(&g_storyarclist, 0, sizeof(g_storyarclist));
    }

    if (!ParserLoadFilesShared("SM_storyarc.bin", SCRIPTS_DIR, ".storyarc", "storyarc.bin", flags, ParseStoryArcList, 
                                &g_storyarclist, sizeof(g_storyarclist), NULL, NULL, StoryArcPreprocessAll, 
                                StoryArcPostprocessAppendFlashback, StoryArcPostprocessAll))
    {
        printf("Error loading story arcs\n");
    }
    StoryArcSetupCallbacks();
}

int StoryArcTotalTasks(void)
{
    int result = 0;
    int i, n, ep, numep;
    n = eaSize(&g_storyarclist.storyarcs);
    for (i = 0; i < n; i++)
    {
        if (!STORYARC_IS_DEPRECATED(g_storyarclist.storyarcs[i]))
        {
            numep = eaSize(&g_storyarclist.storyarcs[i]->episodes);
            for (ep = 0; ep < numep; ep++)
            {
                result += eaSize(&g_storyarclist.storyarcs[i]->episodes[ep]->tasks);
            }
        }
    }    
    return result;
}

// *********************************************************************************
//  Handles and utility functions
// *********************************************************************************

// story arc handles are negative and 1 based
static INLINEDBG intptr_t StoryArcHandleTransform(int x){ return (-x - 1); }

static INLINEDBG intptr_t StoryArcInvalidIndex(int index){ return (index < 0 || index >= eaSize(&g_storyarclist.storyarcs));}

int StoryArcNumDefinitions()
{
    return eaSize(&g_storyarclist.storyarcs);
}

intptr_t StoryArcContextFromFileName(const char* filename)
{
    char buf[MAX_PATH];
    int i, n;

    // just cycle through our global list
    n = eaSize(&g_storyarclist.storyarcs);
    saUtilCleanFileName(buf, filename);
    for (i = 0; i < n; i++)
    {
        if (!stricmp(buf, g_storyarclist.storyarcs[i]->filename))
            return StoryArcHandleTransform(i);
    }
    return 0;
}

StoryTaskHandle StoryArcGetHandle(const char* filename)
{
    StoryTaskHandle sa = {0};
    sa.context = StoryArcContextFromFileName(filename);
    return sa; // we don't have it
}

// a less restrictive version of StoryArcGetHandle - only for debugging purposes
StoryTaskHandle StoryArcGetHandleLoose(const char* filename)
{
    char buf[MAX_PATH], buf2[MAX_PATH];
    int i, n;

    // try to look it up exactly
    StoryTaskHandle ret = StoryArcGetHandle(filename);
    if (ret.context) 
        return ret;

    // otherwise, try matching the initial part of the filename
    saUtilFilePrefix(buf, filename);
    n = eaSize(&g_storyarclist.storyarcs);
    for (i = 0; i < n; i++)
    {
        saUtilFilePrefix(buf2, g_storyarclist.storyarcs[i]->filename);
        if (!stricmp(buf, buf2))
        {
            ret.context = StoryArcHandleTransform(i);
        }
    }
    return ret;
}

// returns whether contact has a reference to this storyarc (can issue it)
int ContactReferencesStoryArc(StoryContactInfo* contactInfo, StoryTaskHandle *sahandle)
{
    int i, n;
    n = eaSize(&contactInfo->contact->def->storyarcrefs);
    for (i = 0; i < n; i++)
    {
        if (contactInfo->contact->def->storyarcrefs[i]->sahandle.context == sahandle->context && 
            contactInfo->contact->def->storyarcrefs[i]->sahandle.bPlayerCreated == sahandle->bPlayerCreated )
            return 1;
    }
    return 0;
}

const StoryArc* StoryArcDefinition(const StoryTaskHandle *sahandle)
{
    if( sahandle->bPlayerCreated )
    {
        return playerCreatedStoryArc_GetStoryArc( sahandle->context );
    }
    else
    {
        int index = StoryArcHandleTransform(sahandle->context);
        if (StoryArcInvalidIndex(index)) 
            return NULL;
        return g_storyarclist.storyarcs[index];
    }
}

int StoryArcCountTasks(const StoryTaskHandle *sahandle)
{
    int retval = 0;
    const StoryArc* arc = StoryArcDefinition(sahandle);
    int count;
    int tcount;

    if (arc && !STORYARC_IS_DEPRECATED(arc))
    {
        for (count = 0; count < eaSize(&(arc->episodes)); count++)
        {
            for (tcount = 0; tcount < eaSize(&(arc->episodes[count]->tasks)); tcount++)
            {
                if (!(arc->episodes[count]->tasks[tcount]->deprecated))
                {
                    retval++;
                }
            }
        }
    }

    return retval;
}

const char* StoryArcFileName(intptr_t i)
{
    // TODO: Deal with player created filenames?
    const StoryArc* def = StoryArcDefinition(tempStoryTaskHandle(i,0,0,0));
    if (!def) 
        return NULL;

    return def->filename;
}

StoryTask* StoryArcTaskDefinition(const StoryTaskHandle *sahandle)
{
    const StoryArc* sa = StoryArcDefinition(sahandle);
    int taskindex = StoryHandleToIndex(sahandle);
    int i, n;

    if (!sa) 
        return NULL;
    if (taskindex < 0) 
        return NULL;

    // have to cycle through episodes counting tasks
    n = eaSize(&sa->episodes);
    for (i = 0; i < n; i++)
    {
        int numtasks = eaSize(&sa->episodes[i]->tasks);
        if (taskindex < numtasks) 
            return sa->episodes[i]->tasks[taskindex];
        else 
            taskindex -= numtasks;
    }
    log_printf("storyerrs.log", "StoryArcTaskDefinition: failed lookup for %i, %i", sahandle->context, sahandle->subhandle);
    return NULL;
}

TaskSubHandle StoryArcGetTaskSubHandle(StoryTaskHandle *sahandle, StoryTask* taskdef)
{
    const StoryArc* sa = StoryArcDefinition(sahandle);
    int numEps, curEp;
    int numTasks, curTask;
    int index = 0;

    // find the task index by cycling through all episodes
    if (!sa) 
        return 0;
    numEps = eaSize(&sa->episodes);
    for (curEp = 0; curEp < numEps; curEp++)
    {
        StoryEpisode* ep = sa->episodes[curEp];
        numTasks = eaSize(&ep->tasks);
        for (curTask = 0; curTask < numTasks; curTask++)
        {
            if (ep->tasks[curTask] == taskdef) 
            {
                sahandle->subhandle = StoryIndexToHandle(index);
                return sahandle->subhandle;
            }
            else 
                index++;
        }
    }
    return 0;
}

// returns the episode this task comes from, and the offset of the task within that episode.
// the episode parameter is optional.  offset set to -1 on failure
void StoryArcTaskEpisodeOffset(StoryTaskHandle *sahandle, int *episode, int *offset)
{
    const StoryArc* sa = StoryArcDefinition(sahandle);
    int index = StoryHandleToIndex(sahandle);
    int numEps, curEp;
    int numTasks, curTask;

    if (index < 0 || sahandle->context == 0 || !sa)
    {
        *offset = -1;
        return;
    }

    // cycle through episodes counting offsets
    numEps = eaSize(&sa->episodes);
    for (curEp = 0; curEp < numEps; curEp++)
    {
        StoryEpisode* ep = sa->episodes[curEp];
        numTasks = eaSize(&ep->tasks);
        for (curTask = 0; curTask < numTasks; curTask++)
        {
            if (index == 0)
            {
                if (episode) 
                    *episode = curEp;
                *offset = curTask;
                return;
            }
            else 
                index--;
        }
    }

    // if we get here, we failed
    *offset = -1;
}

ContactHandle StoryArcGetContactHandle(StoryInfo* info, StoryTaskHandle *sahandle)
{
    int i, n;

    n = eaSize(&info->storyArcs);
    for (i = 0; i < n; i++)
    {
        if (info->storyArcs[i]->sahandle.context == sahandle->context && 
            info->storyArcs[i]->sahandle.bPlayerCreated == sahandle->bPlayerCreated )
        {
            return info->storyArcs[i]->contact;
        }
    }
    return 0;
}

// get the corresponding story arc if this contact has one
StoryArcInfo* StoryArcGetInfo(StoryInfo* info, StoryContactInfo* contactInfo)
{
    int i, n;

    if (!info || !contactInfo)
        return NULL;

    n = eaSize(&info->storyArcs);
    for (i = 0; i < n; i++)
    {
        StoryArcInfo* sainfo = info->storyArcs[i];
        if (sainfo && sainfo->contact == contactInfo->handle)
            return sainfo;
    }
    return NULL;
}

// get the story arc seed if player has this story arc
unsigned int StoryArcGetSeed(StoryInfo* info, StoryTaskHandle *sahandle)
{
    int i, n;
    n = eaSize(&info->storyArcs);
    for (i = 0; i < n; i++)
    {
        StoryArcInfo* sainfo = info->storyArcs[i];
        if (sainfo->sahandle.context == sahandle->context && 
            sainfo->sahandle.bPlayerCreated == sahandle->bPlayerCreated )
            return sainfo->seed;
    }
    return 0;
}

// return 1 if the current episode has all its component parts done and
// is the last one of the story arc, 0 otherwise
int StoryArcCheckLastEpisodeComplete(Entity* e, StoryInfo* info, StoryContactInfo* contactInfo, StoryTaskInfo* task)
{
    StoryArcInfo* arc;
    StoryEpisode* current_ep;
    int offset;
    int episode;
    int ep_tasks;
    int count;

    // silently fail if we get passed bad values
    if (!e || !info || !contactInfo || !task)
    {
        return 0;
    }

    arc = StoryArcGetInfo(info, contactInfo);

    // either we're not in a story arc or we're not at the last episode
    // of it yet. we add one to the count because it's an array index
    // so if the next index is still within the array, there must be
    // another episode after this one
    if (!arc || (arc->episode + 1) < eaSize(&(arc->def->episodes)))
    {
        return 0;
    }

    // we're at the last episode but there might be multiple tasks for it
    // and we need to make sure we just finished the last task if so
    if (task->compoundParent &&    (task->sahandle.compoundPos + 1) < eaSize(&(task->compoundParent->children)))
    {
        return 0;
    }

    StoryArcTaskEpisodeOffset(&task->sahandle, &episode, &offset);

    // something crazy is wrong with the task, it'll get fixed when they
    // call the contact but we can't check it against the story arc now
    if ((offset == -1) || (episode != arc->episode))
    {
        return 0;
    }

    current_ep = arc->def->episodes[arc->episode];
    ep_tasks = eaSize(&(current_ep->tasks));

    // the episode is complete if all the tasks are already marked complete,
    // deprecated or are the just-completed task we know about (which won't
    // get its bit set until they call the contact)
    for (count = 0; count < ep_tasks; count++)
    {
        if (TASK_IS_REQUIRED(current_ep->tasks[count]) &&
            !current_ep->tasks[count]->deprecated)
        {
            if (!BitFieldGet(arc->taskComplete, EP_TASK_BITFIELD_SIZE, count) &&
                count != offset)
            {
                return 0;
            }
        }
    }

    return 1;
}

char* StoryArcGetLeafNameByDef(const StoryArc* arc)
{
    static char buf[500];
    char* leafstart;
    char* leafend;
    int leaflen;

    if (!arc || !arc->filename)
    {
        return NULL;
    }

    leafstart = strrchr(arc->filename, '\\');

    if (!leafstart)
    {
        leafstart = strrchr(arc->filename, '/');
    }

    if (leafstart)
    {
        // Skip the separator - it's not actually part of the name itself.
        leafstart++;
    }
    else
    {
        // There was no separator so we start at the beginning.
        leafstart = arc->filename;
    }

    leaflen = (int)strlen(leafstart);

    // Chop off the ".storyarc" or whatever (if there is one).
    leafend = strrchr(leafstart, '.');

    // Make sure we don't copy the extension, just the name.
    if (leafend)
    {
        leaflen = leafend - leafstart;
    }

    // The filename might have ended on a separator or been immediately
    // followed by a period (eg. "foo\bar\.storyarc").
    if (leaflen <= 0)
    {
        return NULL;
    }

    strncpy_s(buf, 500, leafstart, leaflen);

    return buf;
}

char* StoryArcGetLeafName(StoryInfo* info, StoryContactInfo* contactInfo)
{
    const StoryArc* arc;
    StoryArcInfo *pInfo = StoryArcGetInfo(info, contactInfo);

    if (!pInfo)
        return NULL;

    arc = pInfo->def;

    return StoryArcGetLeafNameByDef(arc);
}


int StoryArcIsScalable(StoryArcInfo* arcinfo)
{
    int retval = 0;

    if (arcinfo && arcinfo->def && (arcinfo->def->flags & STORYARC_SCALABLE_TF))
    {
        retval = 1;
    }

    return retval;
}

// *********************************************************************************
//  Story arc processing - advancing episodes, etc.
// *********************************************************************************


// pick a story arc from the contact and return it
// if level isn't set, the story arc isn't restricted by level
static StoryTaskHandle PickStoryArc(Entity *pPlayer, StoryInfo* info, StoryContactInfo* contactInfo, int* isLong, int level, int* minlevel, const char **requiresFailed)
{
    StoryTaskHandle saList[STORYARCS_PER_CONTACT] = {0};
    int saCount = 0;
    int i, n;

    // reset failed string
    if (requiresFailed != NULL)
        *requiresFailed = NULL;

    // pass through story arcs checking contraints, create list of valid arcs
    n = eaSize(&contactInfo->contact->def->storyarcrefs);
    for (i = 0; i < n; i++)
    {
        int ninfos, curinfo;
        int samestoryarc;
        const StoryArc* sa;
        int playerAlliance = ENT_IS_VILLAIN(pPlayer)?SA_ALLIANCE_VILLAIN:SA_ALLIANCE_HERO;

        if (BitFieldGet(contactInfo->storyArcIssued, STORYARC_BITFIELD_SIZE, i))
            continue; // already been issued

        sa = StoryArcDefinition(&contactInfo->contact->def->storyarcrefs[i]->sahandle);

        if (!sa || STORYARC_IS_DEPRECATED(sa) || sa->flags & STORYARC_FLASHBACK_ONLY)
            continue; // not supposed to be issued anymore or is only available on flashbacks

        // check alliance (ignored for taskforce)
        if (!(pPlayer && pPlayer->taskforce_id) && sa->alliance != SA_ALLIANCE_BOTH && playerAlliance != sa->alliance)
            continue;

        // check storyarc level restrictions
        if (sa->minPlayerLevel && level && sa->minPlayerLevel > level)
        {
            if (minlevel && (*minlevel == 0 || *minlevel > sa->minPlayerLevel)) 
                *minlevel = sa->minPlayerLevel;
            continue;
        }
        if (sa->maxPlayerLevel && level && sa->maxPlayerLevel < level)
            continue;

        // check requires
        if (sa->assignRequires && pPlayer && pPlayer->pchar)
        {
            if (!chareval_Eval(pPlayer->pchar, sa->assignRequires, sa->filename))
            {
                if (saCount == 0 && requiresFailed != NULL && *requiresFailed == NULL)
                {
                    *requiresFailed = sa->assignRequiresFailed;
                }
                continue;
            }
        }

        // make sure that this player doesn't already have the same story arc
        samestoryarc = 0;
        ninfos = eaSize(&info->storyArcs);
        for (curinfo = 0; curinfo < ninfos; curinfo++)
        {
            if (info->storyArcs[curinfo]->sahandle.context == contactInfo->contact->def->storyarcrefs[i]->sahandle.context &&
                info->storyArcs[curinfo]->sahandle.bPlayerCreated == contactInfo->contact->def->storyarcrefs[i]->sahandle.bPlayerCreated )
            {
                samestoryarc = 1;
                break;
            }
        }
        if (samestoryarc)
            continue;

        // otherwise, its ok
        saList[saCount++].context = contactInfo->contact->def->storyarcrefs[i]->sahandle.context; // add to list
    }
    if (!saCount)
        return saList[0]; // no more can be issued

    // reset failed string - we have something so this string is unneeded
    if (requiresFailed != NULL)
        *requiresFailed = NULL;

    // MAK 6/8/4 - designers would prefer storyarcs come in order
    //sel = saSeedRoll(contact->dialogSeed + 123, saCount);
    if (isLong)
    {
        const StoryArc* sa = StoryArcDefinition(&saList[0]);
        *isLong = !(sa->flags & STORYARC_MINI);
    }
    return saList[0];
}

// add the given storyarc to the player
StoryArcInfo* AddStoryArc(StoryInfo* info, StoryContactInfo* contactInfo, StoryTaskHandle *sahandle)
{
    StoryArcInfo* sainfo;
    int i, n;

    // mark as issued
    n = eaSize(&contactInfo->contact->def->storyarcrefs);
    for (i = 0; i < n; i++)
    {
        if (contactInfo->contact->def->storyarcrefs[i]->sahandle.context == sahandle->context &&
            contactInfo->contact->def->storyarcrefs[i]->sahandle.bPlayerCreated == sahandle->bPlayerCreated )
        {
            BitFieldSet(contactInfo->storyArcIssued, STORYARC_BITFIELD_SIZE, i, 1);
            break;
        }
    }
    if (i == n)
    {
        log_printf("storyerrs.log", "AddStoryArc failed.  I was asked to add %i to contact %s, but he doesn't have it", sahandle->context, contactInfo->contact->def->filename);
        return NULL;
    }

    // add to list
    sainfo = storyArcInfoCreate(sahandle, contactInfo->handle);
    eaPush(&info->storyArcs, sainfo);
    sainfo->seed = contactInfo->dialogSeed;


    return sainfo;
}

// remove the given storyarc from the player
void RemoveStoryArc(Entity* player, StoryInfo* info, StoryArcInfo* storyarc, StoryContactInfo* contactInfo, int bClearAssignedFlag)
{
    int i, n;
    n = eaSize(&info->storyArcs);
    for (i = 0; i < n; i++)
    {
        if (info->storyArcs[i]->sahandle.context == storyarc->sahandle.context &&
            info->storyArcs[i]->sahandle.bPlayerCreated == storyarc->sahandle.bPlayerCreated )
        {
            StoryArcInfo* sainfo = info->storyArcs[i];

            // log
            LOG_ENT_OLD(player, "Storyarc:Remove handle: %d", storyarc->sahandle.context);

            // If it is repeatable, flag the contact as if it was never issued
            if (storyarc->def->flags & STORYARC_REPEATABLE || bClearAssignedFlag)
            {
                int j, numRefs = eaSize(&contactInfo->contact->def->storyarcrefs);
                for (j = 0; j < numRefs; j++)
                {
                    if (contactInfo->contact->def->storyarcrefs[j]->sahandle.context == storyarc->sahandle.context &&
                        contactInfo->contact->def->storyarcrefs[j]->sahandle.bPlayerCreated == storyarc->sahandle.bPlayerCreated )
                    {
                        BitFieldSet(contactInfo->storyArcIssued, STORYARC_BITFIELD_SIZE, j, 0);
                    }
                }
            }

            eaRemove(&info->storyArcs, i);
            storyArcInfoDestroy(sainfo);
            if (info->taskforce)
                info->destroyTaskForce = 1; // task forces are always destroyed at end of story arc

            return;
        }
    }
    log_printf("storyerrs.log", "RemoveStoryArc failed.  I was asked to remove %s from player %s, but he doesn't have the arc",
        storyarc->def->filename, player->name);
}

// adds a random storyarc to the player, no probability checks
StoryArcInfo* StoryArcForceAdd(Entity *pPlayer, StoryInfo* info, StoryContactInfo* contactInfo)
{
    StoryTaskHandle sahandle = PickStoryArc(pPlayer, info, contactInfo, NULL, 0, NULL, NULL);
    if (!sahandle.context)
        return NULL; // doesn't have any

    // log
    LOG_ENT(pPlayer, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Taskforce:Add Task handle: %d", sahandle.context);

    return AddStoryArc(info, contactInfo, &sahandle);
}

// *********************************************************************************
//  Story arc hooks - basically all entrances to story arcs are by replacement
// *********************************************************************************

// see if the player can have a story arc added for this contact
StoryArcInfo* StoryArcTryAdding(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo, int mustGetArc, int* minlevel, const char **requiresFailed)
{
    int n, max;
    unsigned int prob;
    int islong = 0;
    StoryTaskHandle sahandle = {0};

    // can't get on a story arc until you've completed something for the contact
    // UNLESS the contact can't offer you a task
    if (contactInfo->contactPoints == 0 && !ContactHasOnlyStoryArcTasks(contactInfo->contact->def) && !mustGetArc) 
        return NULL;

    // restrict the player to MaxStoryarcs
    n = eaSize(&info->storyArcs);
    max = StoryArcMaxStoryarcs(info);
    if (n >= max) 
        return NULL;

    // pick a story arc
    sahandle = PickStoryArc(player, info, contactInfo, &islong, character_GetExperienceLevelExtern(player->pchar), minlevel, requiresFailed);
    if (!sahandle.context)
        return NULL; // none left

    // restrict the number of long storyarcs the player can have
    if (islong)
    {
        n = StoryArcCountLongArcs(info);
        if (n >= StoryArcMaxLongStoryarcs(info))
            return NULL;

        // decide how probable it is they will get a new one
        if (!mustGetArc)
        {
            if (n == 0) 
                prob = SA_CHANCE_GIVEFIRSTARC;
            else 
                prob = SA_CHANCE_GIVESECONDARC;
            if (saSeedRoll(contactInfo->dialogSeed + 17, 100) > prob)
                return NULL;
        }
    }

    // log
    LOG_ENT_OLD(player, "Storyarc:Add Handle %d", sahandle.context);

    // if we're clear, add it
    return AddStoryArc(info, contactInfo, &sahandle);
}

// player is talking to contact.  get a story arc task if possible
int StoryArcGetTask(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo, int taskLength, ContactTaskPickInfo* pickinfo, int mustReplace, int mustGetArc,
                    const char **requiresFailedString, int justChecking)
{
    StoryEpisode* episode;
    StoryArcInfo* storyarc;
    StoryTask* pickList[TASKS_PER_CONTACT];
    int numPicks, sel;
    int tempArcAdd = false;

    // see if we are on a story arc or can get on one
    storyarc = StoryArcGetInfo(info, contactInfo);
    if (!storyarc)
    {
        storyarc = StoryArcTryAdding(player, info, contactInfo, mustGetArc, &pickinfo->minLevel, requiresFailedString);
        if (storyarc != NULL && justChecking)
            tempArcAdd = true;
    }
    if (!storyarc)
        return 0;    // not on story arc

    // decide if we can issue a task for the story arc
    if (!mustReplace && !info->taskforce && (saSeedRoll(contactInfo->dialogSeed + 23, 100) > SA_CHANCE_REPLACETASK))
    {    
        if (tempArcAdd)
            RemoveStoryArc(player, info, storyarc, contactInfo, true);

        return 0;    // failed roll
    }

    // select a task from the ones remaining in this episode
    episode = storyarc->def->episodes[storyarc->episode];

    // try to select an urgent task first
    assert(eaSize(&episode->tasks) < TASKS_PER_CONTACT); // otherwise TaskMatching could overflow pickList

    numPicks = TaskMatching(player, episode->tasks, storyarc->taskIssued, 0, TASK_URGENT,
                                character_GetExperienceLevelExtern(player->pchar), &pickinfo->minLevel, info, pickList, 
                                NULL, NULL, player->supergroup?1:0, NULL);
    if (!numPicks)
        numPicks = TaskMatching(player, episode->tasks, storyarc->taskIssued, 0, 0,
                                character_GetExperienceLevelExtern(player->pchar), &pickinfo->minLevel, info, pickList, 
                                NULL, NULL, player->supergroup?1:0, NULL);

    // now we have a task if possible
    if (!numPicks)
    {
        // MAK 5/11 - this shouldn't be possible..  the contact can only ask us to offer
        // a task now if we don't already have a story arc task - this has been true since
        // we cut out the old supergroup story plan.  
        //   So, if a random encounter isn't what we're waiting on, this storyarc is
        // broken somehow.  We'll use a brute-force method to fix it..
        if (!justChecking && !StoryArcPickRandomEncounter(NULL, info, storyarc))  // don't fix it if we're just checking to see if there is a task
        {
            log_printf("storyarcfix.log", "Fixing storyarc %s for player %s(%i), was on episode %i", storyarc->def->filename, player->name, player->db_id, storyarc->episode+1);

            // manually advance the episode
            storyarc->episode++;
            BitFieldClear(storyarc->taskComplete, EP_TASK_BITFIELD_SIZE);
            BitFieldClear(storyarc->taskIssued, EP_TASK_BITFIELD_SIZE);

            // check and see if the story arc is complete
            if (storyarc->episode >= eaSize(&storyarc->def->episodes))
            {
                RemoveStoryArc(player, info, storyarc, contactInfo, false);
                return 0;
            }
            return StoryArcGetTask(player, info, contactInfo, taskLength, pickinfo, mustReplace, 0, requiresFailedString, justChecking); // if at first you don't succeed..
        }

        if (tempArcAdd)
            RemoveStoryArc(player, info, storyarc, contactInfo, true);

        return 0;    // no tasks to give
    }

    sel = saSeedRoll(contactInfo->dialogSeed, numPicks);
    if (!(pickList[sel]->flags & taskLength))
    {
        if (tempArcAdd)
            RemoveStoryArc(player, info, storyarc, contactInfo, true);
        return 0; // if we task we picked isn't the right length
    }

    // return the task we selected
    pickinfo->taskDef = pickList[sel];
    if( storyarc->sahandle.context )
    {
        pickinfo->sahandle.context = storyarc->sahandle.context;
        StoryArcGetTaskSubHandle(&pickinfo->sahandle, pickList[sel]);
    }
    else
        pickinfo->sahandle.bPlayerCreated = 1;

    if (tempArcAdd)
        RemoveStoryArc(player, info, storyarc, contactInfo, true);

    return 1;
}

// given a story arc, look for a random encounter task in current
// episode that could be used for an encounter
TaskSubHandle StoryArcPickRandomEncounter(EncounterGroup* group, StoryInfo* info, StoryArcInfo* storyarc)
{
    TaskSubHandle* matches;    // list of tasks that we could use
    TaskSubHandle ret = 0;
    StoryEpisode* episode;
    int i, n, sel;

    // iterate through tasks
    episode = storyarc->def->episodes[storyarc->episode];
    eaiCreate(&matches);
    n = eaSize(&episode->tasks);
    for (i = 0; i < n; i++)
    {
        // task must be a random encounter, not issued yet, not deprecated
        const SpawnDef* spawndef = TaskSpawnDef(episode->tasks[i]);
        if (TaskStartingType(episode->tasks[i]) == TASK_RANDOMENCOUNTER &&
            !BitFieldGet(storyarc->taskIssued, EP_TASK_BITFIELD_SIZE, i) &&
            !episode->tasks[i]->deprecated &&
            spawndef)
        {
            // if the spawndef is attached to a particular spawn type, this group has to have one
            if (spawndef->spawntype && group) // HACK - if group not given, just return whether we have an encounter at all
            {
                // look up variable spawntype
                ScriptVarsTable vars = {0};
                StoryTaskHandle sahandle = {0};
                const char* spawntype;

                sahandle.context = storyarc->sahandle.context;
                sahandle.subhandle = StoryIndexToHandle(i);

                saBuildScriptVarsTable(&vars, NULL, 0, 0, NULL, NULL, &sahandle, storyarc, NULL, 0, 0, false);

                spawntype = ScriptVarsTableLookup(&vars, spawndef->spawntype);
                if (EncounterGetMatchingPoint(group, spawntype) != -1)
                    eaiPush(&matches, StoryIndexToHandle(i));
            }
            else // otherwise, we don't care about spawn type
            {
                eaiPush(&matches, StoryIndexToHandle(i));
            }
        }
    }
    if (eaiSize(&matches))
    {
        sel = rand() % eaiSize(&matches);
        ret = matches[sel];
    }
    eaiDestroy(&matches);
    return ret;
}

// check the story arcs for the selected person, fill out active field
// in group and return 1 if a story arc is used.
int StoryArcGetEncounter(EncounterGroup* group, Entity* player)
{
    StoryInfo* info;
    StoryArcInfo* storyarc = NULL;    // will end up being storyarc selected
    StoryContactInfo* contactInfo = NULL;    // will end up being contact selected
    const StoryTask* taskdef;
    StoryTaskInfo* task;
    int i, n, max, limitLevel;

    // these vars are set during task selection
    ContactHandle contacthandle = 0;
    const SpawnDef* spawndef = NULL;
    StoryTaskHandle sahandle = {0};

    ///////////////////////////////////////////////////// BEFORE LOCKING STORYINFO

    // make sure player isn't filled up on tasks
    if (!player) 
        return 0;
    {
        const MapSpec* spec = MapSpecFromMapId(db_state.base_map_id);
        if (!spec)
            spec = MapSpecFromMapName(db_state.map_name);

        if (!MapSpecCanOverrideSpawn(spec, character_GetExperienceLevelExtern(player->pchar))) 
            return 0;
    }
    info = StoryInfoFromPlayer(player);
    max = StoryArcMaxTasks(info);
    if (eaSize(&info->tasks) >= max)
        return 0;

    // iterate through story arcs
    //        HACK - not doing this randomly, probably ok.
    //            not that many random encounters will be available
    n = eaSize(&info->storyArcs);
    for (i = 0; i < n; i++)
    {
        // make sure player doesn't have a task open with this contact
        storyarc = info->storyArcs[i];
        if (!storyarc) 
            continue;
        contactInfo = ContactGetInfo(info, storyarc->contact);
        if ( !contactInfo || ContactGetAssignedTask(player->db_id, contactInfo, info) ) 
            continue;
        if (info->taskforce && TaskAssignedCount(player->db_id, info))
            continue;

        // find a random encounter task from current episode
        sahandle.subhandle = StoryArcPickRandomEncounter(group, info, storyarc);
        if (sahandle.subhandle)
            break;
    }
    if (!sahandle.subhandle)
        return 0;
    sahandle.context = storyarc->sahandle.context;
    contacthandle = contactInfo->handle;
    if (!TeamCanHaveAmbush(player))
        return 0;

    ////////////////////////////////////////////////////////// AFTER LOCKING STORYINFO

    info = StoryArcLockInfo(player);
    contactInfo = ContactGetInfo(info, contacthandle);
    storyarc = StoryArcGetInfo(info, contactInfo);

    // if succeeded, issue task
    limitLevel = contactInfo->contact->def->maxPlayerLevel + contactInfo->contact->def->taskForceLevelAdjust;
    if (!TaskAdd(player, &sahandle, storyarc->seed, limitLevel, 0))
    {
        StoryArcUnlockInfo(player);
        return 0; // couldn't issue task for some reason
    }
    task = TaskGetInfo(player, &sahandle);
    if (!task)
    {
        log_printf("storyerrs.log", "Got to weird place in StoryArcGetEncounter -> task %i:%i", sahandle.context, sahandle.subhandle);
        StoryArcUnlockInfo(player);
        return 0;
    }

    // replace encounter
    taskdef = TaskDefinition(&sahandle);    // must be first child of compound task
    spawndef = TaskSpawnDef(taskdef);
    if (!spawndef)
    {
        log_printf("storyerrs.log", "StoryArcGetEncounter: I got to bottom, but then couldn't get spawndef from %s", taskdef->logicalname);
        StoryArcUnlockInfo(player);
        return 0;
    }
    group->active->spawndef = spawndef;
    
    // calls ScriptVarsTableClear in EncounterActiveInfoFree
    saBuildScriptVarsTable(&group->active->vartable, player, player->db_id, contacthandle, task, NULL, NULL, NULL, NULL, 0, 0, true);
    ScriptVarsTablePushScope(&group->active->vartable, &spawndef->vs);

    if (spawndef->spawntype)
    {
        const char* spawntype = ScriptVarsTableLookup(&group->active->vartable, spawndef->spawntype);
        group->active->point = EncounterGetMatchingPoint(group, spawntype);
    }
    group->active->seed = storyarc->seed;
    group->active->villainbaselevel = character_GetCombatLevelExtern(player->pchar);
    group->active->villainlevel = MAX(0,group->active->villainbaselevel + task->difficulty.levelAdjust);
    group->active->player = erGetRef(player);
    group->active->taskcontext = sahandle.context;
    group->active->tasksubhandle = sahandle.subhandle;
    group->active->mustspawn = 1;

    // Flag the encounter the same as the storyarc owner if spawndef does not specify
    group->active->allyGroup = group->active->spawndef->encounterAllyGroup;
    if(group->active->allyGroup == ENC_UNINIT)
    {
        if (ENT_IS_ON_RED_SIDE(player))
        {
            group->active->allyGroup = ENC_FOR_VILLAIN;
        }
        else
        {
            group->active->allyGroup = ENC_FOR_HERO;
        }
    }

    StoryArcUnlockInfo(player);
    return 1;
}

// signal from encounter system that encounter was completed.
// I can actually get two calls.  The first (optional) call with success == 1
// when the encounter has been defeated.  The second call with success == 0
// is whenever the encounter is cleaned up.  group->taskcomplete must be set
// so I ignore the second call
void StoryArcEncounterComplete(EncounterGroup* group, int success)
{
    StoryInfo* info;
    StoryTaskInfo* task;
    Entity* player;

    // decide if this was a story arc encounter at all
    player = erGetEnt(group->active->player);
    if (!player || !group->active->taskcontext || !group->active->tasksubhandle)
        return;

    // random encounters are closed this way
    if (!group->active->taskcomplete)
    {
        info = StoryArcLockInfo(player);
        task = TaskGetInfo(player, tempStoryTaskHandle(group->active->taskcontext, group->active->tasksubhandle,0, TaskForceIsOnArchitect(player)));
        if (task && task->def->type == TASK_RANDOMENCOUNTER)
            TaskComplete(player, info, task, NULL, success);
        StoryArcUnlockInfo(player);
    }

    // don't check completion again
    group->active->taskcomplete = 1;
}

// signal from encounter system that a reward needs to be given to a rescuer.
// if 1 is returned, StoryArcEnconterReward is assumed to have handled it
int StoryArcEncounterReward(EncounterGroup* group, Entity* rescuer, StoryReward* reward, Entity* speaker)
{
    Entity* player;
    StoryInfo* info;
    StoryTaskInfo* task;
    StoryContactInfo* contactInfo;

    // decide if this was a story arc encounter at all
    if (!group->active->taskcontext || !group->active->tasksubhandle)
        return 0;

    // do a normal reward process for the key player
    player = erGetEnt(group->active->player);
    if (player && player->db_id > 0)
    {
        info = StoryInfoFromPlayer(player);
        task = TaskGetInfo(player, tempStoryTaskHandle(group->active->taskcontext, group->active->tasksubhandle,0,TaskForceIsOnArchitect(player)));
        contactInfo = TaskGetContact(info, task);
        StoryRewardApply(reward, player, info, task, contactInfo, NULL, speaker);
    }

    // the rescuer gets the secondary reward
    if(rescuer && rescuer->db_id > 0 && eaSize(&reward->secondaryReward))
    {
        int levelAdjust = 0;
        if (group->active->villainbaselevel)
            levelAdjust = group->active->villainlevel - group->active->villainbaselevel;
        rewardStoryApplyToDbID(rescuer->db_id,    reward->secondaryReward, group->active->villaingroup, group->active->villainlevel, levelAdjust, &group->active->vartable, REWARDSOURCE_STORY);
    }

    return 1; // I took care of reward
}

void FlashbackLeftRewardApplyToTaskforce(Entity *player)
{
    if (TaskForceIsOnFlashback(player)
        && player->taskforce->storyInfo->storyArcs[0]->def->rewardFlashbackLeft) // pretty sure I can assume all the intermediary pointers are non-NULL
    {
        char buf[1000];
        int playerIndex;
        for (playerIndex = player->taskforce->members.count - 1; playerIndex >= 0; playerIndex--)
        {
            sprintf(buf, "flashback_left_reward_apply %i %i", player->taskforce->members.ids[playerIndex], StoryArcGetHandle(player->taskforce->storyInfo->storyArcs[0]->def->filename).context);
            serverParseClientEx(buf, NULL, ACCESS_INTERNAL);
        }
    }
}

void StoryArcCheckEpisode(Entity* player, StoryArcInfo* storyarc, StoryTaskInfo* task, StoryContactInfo* contactInfo, ContactResponse* response)
{
    StoryEpisode* currentep = storyarc->def->episodes[storyarc->episode];
    StoryInfo* info = StoryInfoFromPlayer(player);
    int i, n, complete, failed;

    // decide if the episode is complete
    n = eaSize(&currentep->tasks);
    complete = 1;
    failed = 0;
    for (i = 0; i < n; i++)
    {
        if (TASK_IS_REQUIRED(currentep->tasks[i]) && !currentep->tasks[i]->deprecated)
        {
            if (!BitFieldGet(storyarc->taskComplete, EP_TASK_BITFIELD_SIZE, i))
            {
                // if issued but not complete, means failed
                if (BitFieldGet(storyarc->taskIssued, EP_TASK_BITFIELD_SIZE, i))
                {
                    failed = 1;
                }
                else
                {
                    complete = 0;
                    break;
                }
            }
        }
    }
    if (!complete)
        return;

    // issue reward
    if (eaSize(&currentep->success) && !failed)
        StoryRewardApply(currentep->success[0], player, info, task, contactInfo, response, NULL);

    // go to next episode
    storyarc->episode++;
    BitFieldClear(storyarc->taskComplete, EP_TASK_BITFIELD_SIZE);
    BitFieldClear(storyarc->taskIssued, EP_TASK_BITFIELD_SIZE);

    // check and see if the story arc is complete
    if (storyarc->episode >= eaSize(&storyarc->def->episodes))
    {
        FlashbackLeftRewardApplyToTaskforce(player);
        RemoveStoryArc(player, info, storyarc, contactInfo, false);
    }
}

void StoryArcCloseTask(Entity* player, StoryContactInfo* contactInfo, StoryTaskInfo* task, ContactResponse* response)
{
    StoryInfo* info = StoryInfoFromPlayer(player);
    StoryArcInfo* storyarc = StoryArcGetInfo(info, contactInfo);
    int episode, offset;
    const StoryTask* rootdef = task->compoundParent;
    if (!rootdef) 
        rootdef = task->def;

    if (!storyarc) 
        return;
    StoryArcTaskEpisodeOffset(&task->sahandle, &episode, &offset);

    // sanity check - this was actually hit once in production.. don't know why
    if ((offset == -1) || (episode != storyarc->episode))
    {
        log_printf("storyerrs.log", "StoryArcCloseTask - closing (%i,%i,%i,%i) for player %s(%i).  Got offset %i, episode %i for task.  "
            "I thought I was on episode %i, storyarc %s.",
            SAHANDLE_ARGSPOS(task->sahandle), player->name, player->db_id, offset, episode, storyarc->episode, storyarc->def->filename);
        // storyarc should be able to right itself anyway..
        return;
    }

    // mark completed if successful
    if (task->state == TASK_SUCCEEDED)
    {
        BitFieldSet(storyarc->taskComplete, EP_TASK_BITFIELD_SIZE, offset, 1);
        if (TASK_CAN_REPEATONSUCCESS(rootdef))
        {
            BitFieldSet(storyarc->taskIssued, EP_TASK_BITFIELD_SIZE, offset, 0);
        }
    }

    // clear issue mark if task is repeatable
    if (TASK_CAN_REPEATONFAILURE(rootdef) && task->state == TASK_FAILED)
    {
        BitFieldSet(storyarc->taskIssued, EP_TASK_BITFIELD_SIZE, offset, 0);
    }

    // complete episode if possible
    StoryArcCheckEpisode(player, storyarc, task, contactInfo, response);
}

// *********************************************************************************
//  Story arc debugging
// *********************************************************************************

// for debugging purposes - reloads all story arc information for this mapserver.
// makes sure that ALL players are reloaded.  Sends a full refresh to only the
// specified player
void StoryArcReloadAll(Entity* player)
{
    PlayerEnts    *players = &player_ents[PLAYER_ENTS_ACTIVE];    // reference to global player list
    char*        playerinfo[MAX_ENTITIES_PRIVATE];                // string for each player online
    int i;

    ErrorfResetCounts();

    // package all players to strings - this is an easy way to refresh the data
    // on each player, as if it took a round trip from database
    memset(playerinfo, 0, sizeof(char*) * MAX_ENTITIES_PRIVATE);
    for(i=0;i<players->count;i++)
    {
        if (!players->ents[i]->dbcomm_state.map_xfer_step && players->ents[i]->ready)
        {
            playerinfo[i] = packageEnt(players->ents[i]);
        }
    }

    // unload the mission info
    if (OnMissionMap())
    {
        MissionForceUninit();
    }

    // reload all story arc info
    StoryPreload();

    // unpack player info - this will reset all the story handles correctly
    for (i = 0; i < players->count; i++)
    {
        if (playerinfo[i])
        {
            unpackEnt(players->ents[i], playerinfo[i]);
            free(playerinfo[i]);
        }
    }

    // make sure the teams get set up correctly again
    for (i = 0; i < players->count; i++)
    {
        if (playerinfo[i])
        {
            teamGetLatest(players->ents[i], CONTAINER_TASKFORCES);
            teamGetLatest(players->ents[i], CONTAINER_SUPERGROUPS);
            teamGetLatest(players->ents[i], CONTAINER_TEAMUPS);
            teamGetLatest(players->ents[i], CONTAINER_LEAGUES);
        }
    }

    // refresh mission if we are on a mission map
    if (OnMissionMap())
    {
        MissionLoad(1, db_state.map_id, db_state.map_name);
        MissionForceReinit(player);
    }

    // send full refresh to given client
    if (player) 
        StoryArcSendFullRefresh(player);
}

void StoryArcPrintCluesHelper(char ** estr, StoryInfo* info, StoryArcInfo* storyarc)
{
    int i, n, found;

    estrConcatf(estr, "\n%s", saUtilLocalize(0,"ClueDebug"));
    found = 0;
    n = eaSize(&storyarc->def->clues);
    for (i = 0; i < n; i++)
    {
        if (BitFieldGet(storyarc->haveClues, CLUE_BITFIELD_SIZE, i))
        {
            if (!found) estrConcatChar(estr, '\n');
            estrConcatf(estr, "\t%s\n", saUtilScriptLocalize(storyarc->def->clues[i]->displayname));
            found = 1;
        }
    }
    if (!found)
    {
        estrConcatf(estr, "\t%s\n", saUtilLocalize(0,"NoneParen"));
    }
}

// returns 1 if any clues were printed
void StoryArcPrintClues(ClientLink * client, StoryInfo* info, StoryArcInfo* storyarc)
{
    char * estr = NULL;
    estrClear(&estr);
    StoryArcPrintCluesHelper(&estr, info, storyarc);
    conPrintf(client, "%s", estr);
    estrDestroy(&estr);
}

void StoryArcDebugShow(char ** estr, StoryInfo* info, StoryArcInfo* storyarc, int detailed)
{
    StoryEpisode* episode = storyarc->def->episodes[storyarc->episode];
    int i, n;

    estrConcatf(estr, "%s (%i) %s\n",storyarc->def->filename, -storyarc->sahandle.context, saUtilLocalize(0,"FromEpisode", ContactDisplayName(ContactDefinition(storyarc->contact),0),    storyarc->episode+1));
    if (!detailed)
        return;

    // task details
    estrConcatf(estr, "%s\n", saUtilLocalize(0,"CurEpisode"));
    n = eaSize(&episode->tasks);
    for (i = 0; i < n; i++)
    {
        StoryTask* taskdef = episode->tasks[i];

        StoryArcGetTaskSubHandle(&storyarc->sahandle, taskdef);
        if (taskdef->deprecated)
            continue;
        estrConcatf(estr, "\t%s %s %s\n\t\t%s\n", TaskDisplayType(taskdef),
            BitFieldGet(storyarc->taskIssued, EP_TASK_BITFIELD_SIZE, i)? "[Issued]": "",
            BitFieldGet(storyarc->taskComplete, EP_TASK_BITFIELD_SIZE, i)? "[Completed]": "",
            TaskDebugName(&storyarc->sahandle,0));
    }

    // clue details
    StoryArcPrintCluesHelper(estr, info, storyarc);
}

void StoryArcDebugShowMineHelper(char ** estr, StoryInfo* info)
{
    int i, n;

    estrConcatStaticCharArray(estr, "Storyarcs -\n");
    n = eaSize(&info->storyArcs);
    for (i = 0; i < n; i++)
    {
        estrConcatChar(estr, '\t');
        StoryArcDebugShow(estr, info, info->storyArcs[i], 0);
    }
    if (i == 0) estrConcatStaticCharArray(estr, "\t(none)\n");
}

void StoryArcDebugShowMine(ClientLink * client, StoryInfo* info)
{
    char * estr = NULL;
    estrClear(&estr);
    StoryArcDebugShowMineHelper(&estr, info);
    conPrintf(client, "%s", estr);
    estrDestroy(&estr);
}

void StoryArcDebugPrint(ClientLink* client, Entity* player, int detailed)
{
    char * estr = NULL;
    estrClear(&estr);
    StoryArcDebugPrintHelper(&estr, client, player, detailed);
    conPrintf(client, "%s", estr);
    estrDestroy(&estr);
}

void StoryArcDebugPrintHelper(char ** estr, ClientLink * client, Entity* player, int detailed)
{
    int i, count, n;
    StoryInfo* info;

    if (!player)
    {
        estrConcatf(estr, "\n%s\n", saUtilLocalize(player,"NoPlayer"));
        return;
    }

    info = StoryInfoFromPlayer(player);

    if (!info)
    {
        estrConcatf(estr, "\n%\n", saUtilLocalize(player,"NoStoryArc"));
        return;
    }

    if (info->taskforce)
    {
        estrConcatf(estr, "%s\n", saUtilLocalize(player,"TaskForce") );
        estrConcatf(estr, "%s %s\n", saUtilLocalize(player,"TFName"), player->taskforce->name);
        estrConcatf(estr, "%s %s\n", saUtilLocalize(player,"TFLeader"), dbPlayerNameFromId(player->taskforce->members.leader));
        estrConcatf(estr, "%s %i\n", saUtilLocalize(player,"TFLevel"), player->taskforce->level);
        if(TaskForceIsOnArchitect(player))    //if they're on an architect mission, give the ID
            estrConcatf(estr, "%s %i\n", saUtilLocalize(player,"TFArchitectId"), player->taskforce->architect_id);
    }
    else
        estrConcatf(estr, "%s\n", saUtilLocalize(player,"NormalMode") );

    // do an inventory of contacts
    ContactDebugShowMineHelper(estr, client, player);

    // show story arc info
    StoryArcDebugShowMineHelper(estr, info);

    // do an inventory of tasks
    estrConcatCharString(estr, saUtilLocalize(player,"TasksDash"));
    n = eaSize(&info->tasks);
    for (i = 0; i < n; i++)
    {
        StoryContactInfo *contactInfo = TaskGetContact(info, info->tasks[i]);

        estrConcatf(estr, "\n\t(%i) %s\n",
            i,
            saUtilLocalize( player,"TaskFromLevel",TaskDisplayState(info->tasks[i]->state),contactInfo ? ContactDisplayName(contactInfo->contact->def,0) : "", info->tasks[i]->level));
        estrConcatf(estr, "\t\t%s %s",
            TaskDebugName(&info->tasks[i]->sahandle, info->tasks[i]->seed),
            info->tasks[i]->spawnGiven? "[SpawnGiven]": "");
        if (info->tasks[i]->compoundParent)
            estrConcatf(estr, "\t\tSubtask %i", info->tasks[i]->sahandle.compoundPos);
        TaskPrintCluesHelper(estr, info->tasks[i]);
        if (TaskIsMission(info->tasks[i]))
        {
            estrConcatf(estr, "\n\t\t%s, pos (%1.2f,%1.2f,%1.2f)\n", 
                saUtilLocalize(player,"MissionAttachDoor",info->tasks[i]->doorId), info->tasks[i]->doorPos[0], info->tasks[i]->doorPos[1], info->tasks[i]->doorPos[2]);
            if (info->tasks[i]->missionMapId)
                estrConcatf(estr, "\n\t\t%s\n", saUtilLocalize(player,"MissionAttachMap",info->tasks[i]->missionMapId));
        }
    }
    if (i == 0) 
        estrConcatf(estr, "\n\t%s", saUtilLocalize(player,"NoneParen"));

    // show inventory of clues
    estrConcatf(estr, "\n%s", saUtilLocalize(player,"StoryArcClueDash"));
    n = eaSize(&info->storyArcs);
    // Show minis, then longs
    count = 0;
    for (i = 0; i < n; i++)
    {
        if (info->storyArcs[i]->def->flags & STORYARC_MINI)
        {
            estrConcatf(estr, "\n\tMiniArc: %s", info->storyArcs[i]->def->filename);
            StoryArcPrintCluesHelper(estr, info, info->storyArcs[i]);
            count++;
        }
    }
    if (count && (count != n))
        estrConcatStaticCharArray(estr, "\n---------------------");
    for (i = 0; i < n; i++)
    {
        if (!(info->storyArcs[i]->def->flags & STORYARC_MINI))
        {
            estrConcatf(estr, "\n\tLongArc: %s", info->storyArcs[i]->def->filename);
            StoryArcPrintCluesHelper(estr, info, info->storyArcs[i]);
        }
    }

    if (n == 0)
    {
        estrConcatf(estr, "\n\t%s", saUtilLocalize(player,"NoneParen"));
    }

    // additional detailed info
    if (!detailed) 
        return;

    // detail any contacts I have tasks open with
    n = eaSize(&info->tasks);
    for (i = 0; i < n; i++)
    {
        ContactDebugShowHelper(estr, client, info, TaskGetContact(info, info->tasks[i]), 1);
    }

    // detail any storyarcs I have
    n = eaSize(&info->storyArcs);
    for (i = 0; i < n; i++)
    {
        StoryArcDebugShow(estr, info, info->storyArcs[i], 1);
    }
}

// messy way to clear all active storyarcs
void StoryArcDebugClearAll(Entity* player)
{
    StoryInfo* info;
    int i, n;

    // clear any tasks first
    TaskDebugClearAllTasks(player);

    info = StoryArcLockInfo(player);

    // clear any open story arcs
    eaSetSize(&info->storyArcs, 0);

    // clear story arc allocation info from contacts
    n = eaSize(&info->contactInfos);
    for (i = 0; i < n; i++)
    {
        BitFieldClear(info->contactInfos[i]->storyArcIssued, STORYARC_BITFIELD_SIZE);
        info->contactInfos[i]->dirty = 1;
    }
    info->contactInfoChanged = 1;
    StoryArcUnlockInfo(player);
}

// finds the story arc and then assigns the appropriate episode
int StoryArcForceGetTask(ClientLink* client, char* logicalname, char* episodename)
{
    StoryInfo* info;
    Entity* player = client->entity;
    int i, n, ret = 0;

    if (PlayerInTaskForceMode(client->entity))
    {
        conPrintf(client, saUtilLocalize(client->entity, "CantGetTaskWhileInTaskforceMode"));
        return ret;
    }

    StoryArcForceGetUnknownContact(client, logicalname);
    info = StoryArcLockInfo(player);
    if (eaSize(&info->storyArcs))
    {
        StoryArcInfo* storyarc = info->storyArcs[0];
        n = eaSize(&storyarc->def->episodes);
        for (i=0; i<n; i++)
        {
            StoryEpisode* episode = storyarc->def->episodes[i];
            int j, numTasks = eaSize(&episode->tasks);
            for (j=0; j<numTasks; j++)
            {
                if (episode->tasks[j]->logicalname && !stricmp(episode->tasks[j]->logicalname, episodename))
                {
                    StoryTask* pickList[TASKS_PER_CONTACT];
                    StoryContactInfo* contactInfo = ContactGetInfo(info, storyarc->contact);
                    int numPicks = TaskMatching(player, episode->tasks, storyarc->taskIssued, 0, 0, 0, 0, info, pickList, NULL, NULL, 
                                                player->supergroup?1:0, NULL);

                    storyarc->episode = i;
                    storyarc->seed = time(NULL);
                    if (numPicks)
                    {
                        int sel = saSeedRoll(contactInfo->dialogSeed, numPicks);
                        StoryArcGetTaskSubHandle(&storyarc->sahandle, pickList[sel]);
                        TaskAdd(player, &storyarc->sahandle, storyarc->seed, 0, 0);
                        ret = 1;
                    }
                }
            }
        }
    }
    StoryArcUnlockInfo(player);
    return ret;
}

// calls StoryArcForceGet(), but first finds a suitable contact to assign the StoryArc (Used for QA purposes)
void StoryArcForceGetUnknownContact(ClientLink * client, char * logicalname)
{
    int i,k,n,p;
    StoryTaskHandle sahandle = StoryArcGetHandleLoose(logicalname);
    const ContactDef **pContacts = g_contactlist.contactdefs;

    if(sahandle.context)
    {
        // loop through all contacts until one is found containing a matching storyarc handle
        n = eaSize(&pContacts);
        for (i = 0; i < n; i++)
        {
            if(pContacts[i])
            {
                p = eaSize(&pContacts[i]->storyarcrefs);
                for(k = 0; k < p; k++)
                {
                    if( pContacts[i]->storyarcrefs[k] && sahandle.context == pContacts[i]->storyarcrefs[k]->sahandle.context && sahandle.bPlayerCreated == pContacts[i]->storyarcrefs[k]->sahandle.bPlayerCreated )
                    {
                        // match...
                        conPrintf(client, "%s %s\n", saUtilLocalize(0,"StoryArcColon"), logicalname);
                        conPrintf(client, "%s %s\n", saUtilLocalize(0,"UsingContact"), pContacts[i]->filename);
                        StoryArcForceGet(client, pContacts[i]->filename, logicalname);
                        return;
                    }
                }
            }
        }
        conPrintf(client, saUtilLocalize(0,"CantFindIssue", logicalname));
        return;
    }

    conPrintf(client, saUtilLocalize(0,"BadStoryArcName", logicalname));
}

// Auto issue contact list for brokers and PvP Contacts
#define MAX_NUM_BROKERS 2
typedef struct AutoAddContactList {
    ContactHandle brokers[MAX_NUM_BROKERS];
    ContactHandle pvpContacts[2];    // 2 pvp contacts 0 = hero, 1 = villain
    ContactHandle *issueOnZoneEnter;
    int initialized;
} AutoAddContactList;

AutoAddContactList g_autoaddcontactlist = {0};

// Initialize the list of auto add contacts so that we only do it once
void StoryArcInitializeAutoAddContactList()
{
    int i, n = eaSize(&g_pnpcdeflist.pnpcdefs);
    int whichBroker = 0;
    
    eaiCreate(&g_autoaddcontactlist.issueOnZoneEnter);

    // Search through all the npcs in the zone and save to a list the handles
    // of the PvP Contacts and Brokers in the zone
    for (i = 0; i < n; i++)
    {
        const PNPCDef* def = g_pnpcdeflist.pnpcdefs[i];
        if (def && def->contact)
        {
            const ContactDef* contact = ContactDefinition(ContactGetHandle(def->contact));
            if (contact && CONTACT_IS_PVPCONTACT(contact) && PNPCFindLocally(def->name))
            {
                int index = (contact->alliance == ALLIANCE_VILLAIN);
                ContactHandle handle = ContactGetHandle(contact->filename);
                g_autoaddcontactlist.pvpContacts[index] = handle;
            }
            else if (contact && CONTACT_IS_BROKER(contact) && PNPCFindLocally(def->name))
            {
                ContactHandle handle = ContactGetHandle(contact->filename);
                if (whichBroker < MAX_NUM_BROKERS)
                    g_autoaddcontactlist.brokers[whichBroker++] = handle;
            } 
            else if (contact && CONTACT_IS_ISSUEONZONEENTER(contact) && PNPCFindLocally(def->name))
            {
                ContactHandle handle = ContactGetHandle(contact->filename);
                if (whichBroker < MAX_NUM_BROKERS)
                    eaiPush(&g_autoaddcontactlist.issueOnZoneEnter, handle);
            }
        }
    }
    g_autoaddcontactlist.initialized = 1;
}

// Add all issue on zone enter contacts for a player
// and remove all auto-dismiss contacts from the player
void StoryArc_AddAllIssueOnZoneEnterContacts(Entity* player, StoryInfo* info)
{
    ContactHandle hContact;
    const ContactDef *contactDef;
    int i, count;

    if (!player || !player->pl || PlayerInTaskForceMode(player))
        return;

    if (!g_autoaddcontactlist.initialized)
        StoryArcInitializeAutoAddContactList();

    count = eaiSize(&g_autoaddcontactlist.issueOnZoneEnter);

    for (i = 0; i < count; i++)
    {
        hContact = g_autoaddcontactlist.issueOnZoneEnter[i];
        contactDef = ContactDefinition(hContact);

        // Add contact if player doesn't already have
        if (hContact && contactDef && !ContactGetInfo(info, hContact)
            && ContactCorrectAlliance(player->pl->playerType, contactDef->alliance)
            && ContactCorrectUniverse(player->pl->praetorianProgress, contactDef->universe)
            && (!contactDef->interactionRequires || chareval_Eval(player->pchar, contactDef->interactionRequires, contactDef->filename)))
        {
            ContactAdd(player, info, hContact, "Auto-Issue On Zone Enter");
        }
    }
}

void StoryArc_RemoveAllAutoDismissContacts(Entity *player, StoryInfo *info)
{
    const ContactDef *contactDef;
    int i, count;

    if (g_disableAutoDismiss)
    {
        sendInfoBox(player, INFO_SVR_COM, "Debug disable of auto-dismissal of contacts is on");
    }
    else
    {
        count = eaSize(&info->contactInfos);
        for (i = count - 1; i >= 0; i--)
        {
            if (info->contactInfos[i])
            {
                contactDef = info->contactInfos[i]->contact->def;

                if (contactDef && CONTACT_IS_AUTODISMISSED(contactDef))
                {
                    ContactDismiss(player, ContactGetHandleFromDefinition(contactDef), 1);
                }
            }
        }
    }
}


// Add the appropriate pvp contact for a player
void StoryArcAddCurrentPvPContact(Entity* player, StoryInfo* info)
{
    ContactHandle pvpContact;
    if (!g_autoaddcontactlist.initialized)
        StoryArcInitializeAutoAddContactList();
    pvpContact = g_autoaddcontactlist.pvpContacts[ENT_IS_VILLAIN(player)];

    // Add contact if player doesn't already have
    if (pvpContact && !ContactGetInfo(info, pvpContact) && !PlayerInTaskForceMode(player))
        ContactAdd(player, info, pvpContact, "Auto-Issue PvP Contact");
}

// Get the appropriate broker for a player
ContactHandle StoryArcGetCurrentBroker(Entity* player, StoryInfo* info)
{
    int numberOfBrokers = 0;
    if (!g_autoaddcontactlist.initialized)
        StoryArcInitializeAutoAddContactList();
    
    // If there are no brokers, return 0
    numberOfBrokers = !(!g_autoaddcontactlist.brokers[0]) + !(!g_autoaddcontactlist.brokers[1]);
    if (!numberOfBrokers)
        return 0;

    // If the player has either, return which he has(backwards compatibility)
    if (ContactGetInfo(info, g_autoaddcontactlist.brokers[0]))
        return g_autoaddcontactlist.brokers[0];
    if (ContactGetInfo(info, g_autoaddcontactlist.brokers[1]))
        return g_autoaddcontactlist.brokers[1];

    // Return a random broker based on db_id and the current map
    srand(player->db_id + db_state.base_map_id);
    return g_autoaddcontactlist.brokers[rand()%numberOfBrokers];
}

// Add the appropriate broker
void StoryArcAddCurrentBroker(Entity* player, StoryInfo* info)
{
    ContactHandle broker = StoryArcGetCurrentBroker(player, info);
    if (broker && !ContactGetInfo(info, broker) && !PlayerInTaskForceMode(player))
        ContactAdd(player, info, broker, "Auto-Issue Broker");
}

void StoryArcForceGet(ClientLink* client, const char* contactfile, const char* storyarcfile)
{
    StoryInfo* info;
    StoryContactInfo* contactInfo;
    StoryArcInfo* storyarc;
    ContactHandle contacthandle;
    StoryTaskHandle sahandle = {0};

    // clear any other storyarcs first
    StoryArcDebugClearAll(client->entity);
    info = StoryArcLockInfo(client->entity);

    // find or add the contact
    contacthandle = ContactGetHandleLoose(contactfile);
    if (!contacthandle)
    {
        conPrintf(client, saUtilLocalize(client->entity, "CantFindContact", contactfile));
        StoryArcUnlockInfo(client->entity);
        return;
    }
    contactInfo = ContactGetInfo(info, contacthandle);
    if (!contactInfo)
    {
        // try adding it
        StoryArcUnlockInfo(client->entity);
        ContactDebugAdd(client, client->entity, contactfile);
        info = StoryArcLockInfo(client->entity);
        contactInfo = ContactGetInfo(info, contacthandle);
        if (!contactInfo)
        {
            StoryArcUnlockInfo(client->entity);
            return; // should have already printed error message
        }
    }

    // find or add the story arc
    sahandle = StoryArcGetHandleLoose(storyarcfile);
    if (!sahandle.context)
    {
        conPrintf(client, saUtilLocalize(client->entity,"CantFindStoryArc", storyarcfile));
        StoryArcUnlockInfo(client->entity);
        return;
    }
    if (!ContactReferencesStoryArc(contactInfo, &sahandle))
    {
        // manually add the story arc to the contact's list
        const StoryArc* def = StoryArcDefinition(&sahandle);
        StoryArcRef* ref = calloc(sizeof(StoryArcRef), 1);
        eaPushConst(&contactInfo->contact->def->storyarcrefs, ref);
        ref->filename = strdup(def->filename);
        ref->sahandle.context = sahandle.context;
        conPrintf(client, saUtilLocalize(client->entity,"ManualAdd") );
    }

    // issue the story arc
    storyarc = AddStoryArc(info, contactInfo, &sahandle);
    if(storyarc)
        conPrintf(client, saUtilLocalize(client->entity,"ArcIssued") );
    else
        conPrintf(client, saUtilLocalize(client->entity,"CantIssueArc"));

    StoryArcUnlockInfo(client->entity);
}

int StoryArcAddPlayerCreated(Entity *e, StoryInfo* pInfo, int tf_id )
{
    StoryArcInfo* pSAinfo;
    StoryContactInfo *pContact;
    StoryArc* pArc;
    int contacthandle;
    char * contactfile = "Player_Created/MissionArchitectContact.contact";

    // find or add the contact
    contacthandle = ContactGetHandleLoose(contactfile);
    if (!contacthandle)
    {
        return 0;
    }
    pContact = ContactGetInfo(pInfo, contacthandle);
    if (!pContact)
    {
        // try adding it
        StoryArcUnlockInfo(e);
        ContactDebugAdd(e->client, e, contactfile);
        pInfo = StoryArcLockInfo(e);
        pContact = ContactGetInfo(pInfo, contacthandle);
        if (!pContact)
        {
            return 0; // should have already printed error message
        }
    }

    pArc = playerCreatedStoryArc_GetStoryArc( tf_id );
    if(!pArc)
        return 0;

    // issue the story arc
    pSAinfo = storyArcInfoAlloc(); 
    pSAinfo->contact = pContact->handle;
    memset(&pSAinfo->sahandle, 0, sizeof(StoryTaskHandle)); 
    pSAinfo->sahandle.context = pArc->uID;
    pSAinfo->sahandle.bPlayerCreated = 1;
    pSAinfo->def = pArc;
    pSAinfo->contactDef = ContactDefinition(pSAinfo->contact);

    eaPush(&pInfo->storyArcs, pSAinfo);

    return 1;
}


//////////////////////////////////////////////////////////////////////////
// evaluates the flashback requires (if it exists) to let system know if player should have 
// access to this storyarc
int StoryArcIsAvailable(Entity *e, StoryArc *pArc)
{
    // if the requires failed text exists, display the storyarc in the flashback list even if
    // they don't pass the requires, we'll give them the fail string if they try to choose it
    if (pArc->flashbackRequires != NULL && e->pchar != NULL && pArc->flashbackRequiresFailed == NULL)
        return chareval_Eval(e->pchar, pArc->flashbackRequires, pArc->filename);

    return true;
}


//////////////////////////////////////////////////////////////////////////
// sends the list of valid flashbacks down to the client
void StoryArcFlashbackList(Entity* e)
{
    int i, count = eaSize(&g_storyarclist.storyarcs);
    int sendCount = 0;
    const char *str;
    int complete = 0;

    // need to count how many actual are going to be sent.
    for (i = 0; i < count; i++)
    {
        StoryArc *pArc = g_storyarclist.storyarcs[i];
        Contact *pContact = StoryArcFindContact(tempStoryTaskHandle(StoryArcHandleTransform(i),0,0,0)); 

        if (pArc != NULL && pContact != NULL &&                                            // bad arc or no contact gives this arc
            !(pArc->flags & STORYARC_NOFLASHBACK) &&                                    // not available for flashback
            (!STORYARC_IS_DEPRECATED(pArc) || (pArc->flags & STORYARC_FLASHBACK)) &&    // is not depricated or a specially flagged flashback mission
            StoryArcIsAvailable(e, pArc))                                                // passes the required statement
            sendCount++;
    }


    // Send the list
    START_PACKET(pak, e, SERVER_FLASHBACK_LIST);

    pktSendBitsPack(pak, 1, sendCount);

    for (i = 0; i < count; i++)
    {
        StoryArc *pArc = g_storyarclist.storyarcs[i];
        Contact *pContact = StoryArcFindContact(tempStoryTaskHandle(StoryArcHandleTransform(i),0,0,0)); 
        
        if (pArc != NULL && pContact != NULL &&                                            // bad arc or no contact gives this arc
            !(pArc->flags & STORYARC_NOFLASHBACK) &&                                    // not available for flashback
            (!STORYARC_IS_DEPRECATED(pArc) || (pArc->flags & STORYARC_FLASHBACK)) &&    // is not depricated or a specially flagged flashback mission
            StoryArcIsAvailable(e, pArc))                                                // passes the required statement
        {
            U32 bronze;
            U32 silver;
            U32 gold;
            int iMaxLevel = pContact->def->maxPlayerLevel;

            // adjusting max level
            if (iMaxLevel > pArc->maxPlayerLevel)
                iMaxLevel = pArc->maxPlayerLevel;


            // file name
            pktSendString(pak, pArc->filename);

            // time limits
            TaskForceFindTimeLimits(pArc, &bronze, &silver, &gold);
            pktSendBitsAuto(pak, bronze);
            pktSendBitsAuto(pak, silver);
            pktSendBitsAuto(pak, gold);

            // name
            str = saUtilScriptLocalize(pArc->name);
            pktSendString(pak, str);

            // level ranges
            pktSendBitsPack(pak, 4, pArc->minPlayerLevel);
            pktSendBitsPack(pak, 4, iMaxLevel);
            
            // alliance
            pktSendBitsPack(pak, 1, pArc->alliance);

            // flashback cost multiplier
            pktSendF32(pak, pArc->flashbackCostMulitplier);

            // completion requires
            if (pArc->completeRequires)
                complete = chareval_Eval(e->pchar, pArc->completeRequires, pArc->filename);
            pktSendBitsPack(pak, 1, complete);
        }        
    }
    END_PACKET
}

//////////////////////////////////////////////////////////////////////////
// Given a specific storyarc, finds a contact that can assign this storyarc.
//
static Contact *StoryArcFindContact(StoryTaskHandle *sahandle)
{
    int i,k,n,p;
    Contact *contacts = g_contacts;

    if(sahandle->context)
    {
        // loop through all contacts until one is found containing a matching storyarc handle
        n = eaSize(&g_contactlist.contactdefs);
        for (i = 0; i < n; i++)
        {
            if(contacts[i].def)
            {
                p = eaSize(&contacts[i].def->storyarcrefs);
                for(k = 0; k < p; k++)
                {
                    if (contacts[i].def->storyarcrefs[k] && sahandle->context == contacts[i].def->storyarcrefs[k]->sahandle.context && sahandle->bPlayerCreated == contacts[i].def->storyarcrefs[k]->sahandle.bPlayerCreated)
                    {
                        return &contacts[i];
                    }
                }
            }
        }
    }
    return NULL;
}

// Given a specific storyarc, finds a contact that can assign this storyarc.
ContactHandle StoryArcFindContactHandle(StoryTaskHandle *sahandle)
{
    int i,k,n,p;
    const ContactDef **pContacts = g_contactlist.contactdefs;

    if(sahandle->context)
    {
        // loop through all contacts until one is found containing a matching storyarc handle
        n = eaSize(&pContacts);
        for (i = 0; i < n; i++)
        {
            if(pContacts[i])
            {
                p = eaSize(&pContacts[i]->storyarcrefs);
                for(k = 0; k < p; k++)
                {
                    if( pContacts[i]->storyarcrefs[k] && sahandle->context == pContacts[i]->storyarcrefs[k]->sahandle.context && sahandle->bPlayerCreated == pContacts[i]->storyarcrefs[k]->sahandle.bPlayerCreated)
                    {
                        return StoryIndexToHandle(i);
                    }
                }
            }
        }
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////
// Sends the details of a specified storyarc to the client for use in the 
//        flashback interface
void StoryArcFlashbackDetails(Entity* e, char *fileID)
{
    const StoryArc*        pArc;
    const StoryTask*    pTaskDef = NULL;
    char                str[LEN_CONTACT_DIALOG];
    Contact                *pContact = NULL;
    ContactHandle        contactHandle = 0;
    ScriptVarsTable        taskvars = {0};
    int                    meetsRequires = true;
    int                    meetsBadgeRequires = true;
    int                    meetsContactRequires = true;
    StoryTaskHandle        sahandle = {0};
    
    sahandle = StoryArcGetHandle(fileID);
    sahandle.subhandle = 1;
    pArc = StoryArcDefinition(&sahandle);

    if (!pArc)
        return;

    contactHandle = StoryArcFindContactHandle(&sahandle);
    pContact = GetContact(contactHandle);
    if (!pContact)
        return;

    pTaskDef = TaskParentDefinition(&sahandle);
    if (!pTaskDef)
        return;

    // check requires on arc
    if (pArc->assignRequires != NULL && e && e->pchar)
    {
        meetsRequires = chareval_Eval(e->pchar, pArc->assignRequires, pArc->filename);
    }

    if (pContact->def->interactionRequires)
        meetsContactRequires = chareval_Eval(e->pchar, pContact->def->interactionRequires, pContact->def->filename);

    if (pContact->def->requiredBadge)
        meetsBadgeRequires = badge_OwnsBadge(e, pContact->def->requiredBadge);


    START_PACKET(pak, e, SERVER_FLASHBACK_DETAILS);

    // ID
    pktSendString(pak, pArc->filename);

    // intro string
    str[0] = 0;

    // ScriptVars HACK: seed here is complete bullshit, and this does not support VG Random / Mapset Random fields
    saBuildScriptVarsTable(&taskvars, e, e->db_id, contactHandle, NULL, NULL, &sahandle, NULL, NULL, 0, 0, false);

    strcatf_s(str, LEN_CONTACT_DIALOG, ContactHeadshot(pContact, e, 0));

    // show intro string
//    strcatf_s(str, LEN_CONTACT_DIALOG, saUtilTableLocalize(pContact->firstVisit, &taskvars));
//    strcatf_s(str, LEN_CONTACT_DIALOG, "<br><br>");

    // show mission string
    TaskAddTitle(pTaskDef, str, e, &taskvars);
    if (!meetsRequires && pArc->assignRequiresFailed != NULL) 
    {
        strcat_s(str, LEN_CONTACT_DIALOG, saUtilTableLocalize(pArc->assignRequiresFailed, &taskvars));
    } else if (!meetsBadgeRequires && pContact->def->badgeNeededString != NULL) {
        strcat_s(str, LEN_CONTACT_DIALOG, saUtilTableLocalize(pContact->def->badgeNeededString, &taskvars));
    } else if (!meetsContactRequires && pContact->def->interactionRequiresFailString != NULL) {
        strcat_s(str, LEN_CONTACT_DIALOG, saUtilTableLocalize(pContact->def->interactionRequiresFailString, &taskvars));
    }
    else {
        strcat_s(str, LEN_CONTACT_DIALOG, saUtilTableLocalize(pTaskDef->detail_intro_text, &taskvars));

        // add exemplar information
        {
            int iLevel = pContact->def->maxPlayerLevel;

            if (iLevel > pArc->maxPlayerLevel)
                iLevel = pArc->maxPlayerLevel;

            strcatf_s(str, LEN_CONTACT_DIALOG, "<br><br><font color=deepskyblue>%s %d.</font>", 
                textStd("ExemplarWarningString"),
                iLevel);
        }
    }

    pktSendString(pak, str);

    // accept string
    if (!meetsRequires || !meetsBadgeRequires || !meetsContactRequires)
        strcpy_s(str, LEN_CONTACT_DIALOG, "Leave");
    else
        strcpy_s(str, LEN_CONTACT_DIALOG, saUtilTableLocalize(pTaskDef->yes_text, &taskvars));
    pktSendString(pak, str);

    
    END_PACKET
}

void StoryArcFlashbackCheckEligibility(Entity *e, char *filename)
{
    int level = character_CalcExperienceLevel(e->pchar) + 1;
    StoryTaskHandle sahandle = StoryArcGetHandle(filename);
    const StoryArc* pArc;
    Contact *pContact; 
    Teamup *team = e->teamup;
    
    sahandle.subhandle = 1;
    pArc = StoryArcDefinition(&sahandle);
    pContact = StoryArcFindContact(&sahandle);

    if(team && team->members.leader != e->db_id)
    {
        sendChatMsg(e, localizedPrintf(e, "CannotFlashbackNeedTeamLeader"), INFO_SVR_COM, 0);
        return;
    }

    // handle strange case where level 50 can take missions in highest level bucket
    if (level == 50)
        level = 51;

    // max level of the arc is the lower of the arc and the contact's level
    // to be eligible for a flashback, you must be ineligible by level for that arc normally
    // the below if check is a simplification of the above logic
    if (pArc && pContact && pContact->def 
        && level <= pArc->maxPlayerLevel && level <= pContact->def->maxPlayerLevel)
    {
        // shouldn't be able to get here without hacking the client, but it's best to not trust the client...
        sendChatMsg(e, localizedPrintf(e, "CannotFlashbackString"), INFO_SVR_COM, 0);
        return;
    }
    
    if (pArc && pArc->flashbackRequires && pArc->flashbackRequiresFailed)
    {
        if (!chareval_Eval(e->pchar, pArc->flashbackRequires, pArc->filename))
        {
            ScriptVarsTable vars = {0};
            sendChatMsg(e, saUtilTableAndEntityLocalize(pArc->flashbackRequiresFailed, &vars, e), INFO_SVR_COM, 0);
            return;
        }
    }

    if (pArc && pArc->flashbackTeamRequires)
    {
        if (!team)
        {
            if (!chareval_Eval(e->pchar, pArc->flashbackTeamRequires, pArc->filename))
            {
                if (pArc->flashbackTeamFailed)
                {
                    ScriptVarsTable vars = {0};
                    sendChatMsg(e, saUtilTableAndEntityLocalize(pArc->flashbackTeamFailed, &vars, e), INFO_SVR_COM, 0);
                }
                else
                {
                    sendChatMsg(e, localizedPrintf(e, "CannotFlashbackString"), INFO_SVR_COM, 0);
                }
                return;
            }
        }
        else
        {
            // send relay command to validate flashback to all members of the team
            int i;

            TeamTaskSelectClear(team);

            team->taskSelect.activeTask.context = sahandle.context;
            team->taskSelect.activeTask.subhandle = sahandle.subhandle;
            team->taskSelect.activeTask.compoundPos = sahandle.compoundPos;
            team->taskSelect.activeTask.bPlayerCreated = sahandle.bPlayerCreated;
            team->taskSelect.team_mentor_dbid = e->db_id;

            for(i = 0; i < team->members.count; i++)
            {
                char cmdbuf[2000];
                sprintf(cmdbuf, "flashbackteamrequiresvalidate %i %i %i %i %i", team->members.ids[i], sahandle.context, sahandle.subhandle, sahandle.compoundPos, sahandle.bPlayerCreated);
                serverParseClientEx(cmdbuf, NULL, ACCESS_INTERNAL);
            }
            return;
        }
    }

    // if I haven't returned yet, I must be immediately eligible
    START_PACKET(pak, e, SERVER_FLASHBACK_IS_ELIGIBLE);
    END_PACKET
}

void StoryArcFlashbackTeamRequiresValidate(Entity *e, StoryTaskHandle *sahandle)
{
    const StoryArc* pArc = StoryArcDefinition(sahandle);
    char cmdbuf[2000];

    if (devassert(pArc && pArc->flashbackTeamRequires) && e->teamup)
    {
        sprintf(cmdbuf, "flashbackteamrequiresvalidateack %i %i %i %i %i %i %i", e->teamup->members.leader, sahandle->context, sahandle->subhandle, sahandle->compoundPos, sahandle->bPlayerCreated, chareval_Eval(e->pchar, pArc->flashbackTeamRequires, pArc->filename), e->db_id);
        serverParseClientEx(cmdbuf, NULL, ACCESS_INTERNAL);
    }
}

// logic comes from TeamTaskValidateSelectionAck
void StoryArcFlashbackTeamRequiresValidateAck(Entity *e, StoryTaskHandle *sahandle, int result, int responder)
{
    const StoryArc* pArc = StoryArcDefinition(sahandle);
    int i, teamCount;
    Teamup *team = e->teamup;

    if (!team || !devassert(pArc))
        return; // not sure what else to do here

    if (!result)
    {
        if (pArc->flashbackTeamFailed)
        {
            ScriptVarsTable vars = {0};
            sendChatMsg(e, saUtilTableAndEntityLocalize(pArc->flashbackTeamFailed, &vars, e), INFO_SVR_COM, 0);
        }
        else
        {
            sendChatMsg(e, localizedPrintf(e, "CannotFlashbackString"), INFO_SVR_COM, 0);
        }

        TeamTaskSelectClear(team);
    }
    else
    {
        for (i = eaiSize(&team->taskSelect.memberValid)-1; i >= 0; i--)
        {
            if (team->taskSelect.memberValid[i] == responder)
                break;
        }

        if (i < 0)
        {
            eaiPush(&team->taskSelect.memberValid, responder);
        }

        teamCount = team->members.count;

        if (eaiSize(&team->taskSelect.memberValid) >= team->members.count)
        {
            for (i = teamCount - 1; i >= 0; i--)
            {
                if (eaiFind(&team->taskSelect.memberValid, team->members.ids[i]) < 0)
                {
                    break;
                }
            }

            if (i < 0)
            {
                START_PACKET(pak, e, SERVER_FLASHBACK_IS_ELIGIBLE);
                END_PACKET
                
                TeamTaskSelectClear(team);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Starts the player on the specified flashback 

// returns 1 if all team members of player are on this mapserver, 0 if no team
// TODO:  Un-static this and remove the copy/pasted version from contactDialog.c
static int AllTeamMembersPresent(Entity* player)
{
    int i;

    if (!player->teamup_id) return 0;
    for (i = 0; i < player->teamup->members.count; i++)
    {
        if (!entFromDbId(player->teamup->members.ids[i]))
            return 0;
    }
    return 1;
}

int AllTeamMembersPassRequires(Entity *player, char **required, char *dataFilename)
{
    int i;

    if (!player->teamup_id)
    {
        return chareval_Eval(player->pchar, required, dataFilename);
    }
    else
    {
        for (i = 0; i < player->teamup->members.count; i++)
        {
            Entity *ent = entFromDbId(player->teamup->members.ids[i]);
            if (!ent || !chareval_Eval(ent->pchar, required, dataFilename))
                return 0;
        }
        return 1;
    }
}

// returns 1 if all team members are within the given level requirements
// TODO:  Un-static this and remove the copy/pasted version from contactDialog.c
static int AllTeamMembersMeetLevels(Entity* player, int minlevel, int maxlevel)
{
    int i;

    if (!player->teamup_id) return 0;
    for (i = 0; i < player->teamup->members.count; i++)
    {
        Entity* e = entFromDbId(player->teamup->members.ids[i]);

        if (!e) return 0;
        //if (character_GetCombatLevelExtern(e->pchar) > maxlevel) return 0; letting higher level on now, exemplaring them
        if (character_GetExperienceLevelExtern(e->pchar) < minlevel) return 0;
    }
    return 1;
}

static int SAPlayerCantFormFlashbackTaskForce(Entity* player, ContactDef* contact)
{
    int teamsize = 1;

    if (contact->requiredToken && 0 == getRewardToken(player, contact->requiredToken))
        return false;

    if (character_GetSecurityLevelExtern(player->pchar) < contact->minPlayerLevel)
        return false;

    if (player->teamup && player->teamup->members.leader != player->db_id)
        return false;

    if (player->teamup && !AllTeamMembersPresent(player))
        return false;

    if (player->teamup && !AllTeamMembersMeetLevels(player, contact->minPlayerLevel, contact->maxPlayerLevel))
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////

static void StoryArcCleanupTokenRequired(Entity* player, StoryInfo* info)
{
    int c, numContacts = eaSize(&info->contactInfos);

    // Now clear out any contacts that are only active when the token is around
    for (c = numContacts-1; c >= 0; c--)
    {
        StoryContactInfo* contactInfo = info->contactInfos[c];
        if (contactInfo->contact->def->mapTokenRequired && MapGetDataToken(contactInfo->contact->def->mapTokenRequired) != 1 && contactInfo->handle)
        {
            ContactDismiss(player, contactInfo->handle, 0);
        }
    }
}

#define STORY_MESSAGE_SHOW 1
#define STORY_MESSAGE_HIDE 2

// Callback everytime a player enters a new map
// Uses the player info, if you have taskforce related things, create a new function
void StoryArcPlayerEnteredMap(Entity* player)
{
    StoryInfo* info;
    const MissionDef* missiondef;
    StoryContactInfo* broker;
    StoryContactInfo* newspaper;
    int i, n;

    // All of this only happens on non-mission maps
    if (OnMissionMap())
        return;
    info = StoryArcLockPlayerInfo(player);

    // Cleanup Token Required Stuff for Events
    StoryArcCleanupTokenRequired(player, info);

    // Cleanup Broken Tasks and reprompt teamcompleted tasks if neccessary
    // Readded the dont return to contact tasks for backwards compatibility
    n = eaSize(&info->tasks);
    for (i = n-1; i >= 0; i--)
    {
        StoryTaskInfo* task = info->tasks[i];
        if (TaskIsBroken(task) && TASK_INPROGRESS(task->state))
        {
            TaskComplete(player, info, task, NULL, 1);
            chatSendToPlayer(player->db_id, saUtilLocalize(player, "TaskCompleteDueToError"), INFO_SVR_COM, 0);
        }
        else if (TASK_IS_NORETURN(task->def) && TASK_COMPLETE(task->state))
        {
            StoryContactInfo* dontreturncontact = TaskGetContact(info, task);
            TaskRewardCompletion(player, task, dontreturncontact, NULL);
            TaskClose(player, dontreturncontact, task, NULL);
        }
        else if (task->teamCompleted && TASK_INPROGRESS(task->state))
        {
            if (TaskPromptTeamComplete(player, info, task, task->difficulty.levelAdjust, task->teamCompleted))
            {
                const StoryReward *reward = NULL, *reward2 = NULL;
                if (eaSize(&task->def->taskSuccess))
                {
                    reward = task->def->taskSuccess[0];
                }

                if (TaskForceIsOnFlashback(player))
                {
                    if (eaSize(&task->def->taskSuccessFlashback))
                    {
                        reward2 = task->def->taskSuccessFlashback[0];
                    }
                }
                else
                {
                    if (eaSize(&task->def->taskSuccessNoFlashback))
                    {
                        reward2 = task->def->taskSuccessNoFlashback[0];
                    }
                }
            
                if (reward && eaSize(&reward->secondaryReward))
                {
                    ScriptVarsTable vars = {0};
                    saBuildScriptVarsTable(&vars, player, player->db_id, TaskGetContactHandle(info, task), task, NULL, NULL, NULL, NULL, 0, 0, false);
                    MissionGrantSecondaryReward(player->db_id, reward->secondaryReward, getVillainGroupFromTask(task->def), task->teamCompleted, task->difficulty.levelAdjust, &vars);
                }
                if (reward2 && eaSize(&reward2->secondaryReward))
                {
                    ScriptVarsTable vars = {0};
                    saBuildScriptVarsTable(&vars, player, player->db_id, TaskGetContactHandle(info, task), task, NULL, NULL, NULL, NULL, 0, 0, false);
                    MissionGrantSecondaryReward(player->db_id, reward2->secondaryReward, getVillainGroupFromTask(task->def), task->teamCompleted, task->difficulty.levelAdjust, &vars);
                }

                // Restore their tasks original notoriety
                difficultyCopy(&task->difficulty, &info->difficulty);
                task->teamCompleted = 0;

            }
        }
        // Set up the zowie array
        // Note that zowie_setup checks we're on the correct map, and that the task is a zowie task, so it's safe to call it for
        // any task.
        zowie_setup(task, 0);
    }

    // Add/remove contacts due to player entering the map
    StoryArcAddCurrentBroker(player, info);
    StoryArcAddCurrentPvPContact(player, info);
    StoryArc_AddAllIssueOnZoneEnterContacts(player, info);
    StoryArc_RemoveAllAutoDismissContacts(player, info);

    // Reseed the newspaper and fixup the broker handle if neccessary
    newspaper = ContactGetInfo(info, ContactGetNewspaper(player));
    if (newspaper)
    {
        const ContactDef* def = ContactDefinition(newspaper->brokerHandle);
        if (!def || !CONTACT_IS_BROKER(def))
            newspaper->brokerHandle = StoryArcGetCurrentBroker(player, info);
        newspaper->dialogSeed = (unsigned int)time(NULL);
    }

    // Alert the player that he has a new contact reward
    broker = ContactGetInfo(info, StoryArcGetCurrentBroker(player, info));
    if (broker && broker->rewardContact == STORY_MESSAGE_SHOW)
    {
        const char* displayName = ContactDisplayName(broker->contact->def,0);
        const char* alertText;
        if (broker->contact->def->alliance == ALLIANCE_HERO)
        {
            alertText = saUtilLocalize(player, "DetectiveHasNewContact", displayName); // Does NOT V_ translate, based off of playerTypeByLocation!
        }
        else
        {
            alertText = saUtilLocalize(player, "BrokerHasNewContact", displayName); // Does NOT V_ translate, based off of playerTypeByLocation!
        }
        START_PACKET(pak, player, SERVER_BROKER_CONTACT);
        pktSendString(pak, alertText);
        END_PACKET    
        broker->rewardContact = STORY_MESSAGE_HIDE;
    }

    // send the exit mission text to the player
    missiondef = TaskMissionDefinition(&info->exithandle);
    if (missiondef && info->exitMissionSuccess != STORY_MESSAGE_HIDE)
    {
        const char* exittext = NULL;
        ScriptVarsTable vars = {0};

        // Potential mismatch - TaskGetInfo could be NULL in which case we use the exithandle instead but lose seed information
        saBuildScriptVarsTable(&vars, player, info->exitMissionOwnerId, TaskGetContactHandleFromStoryTaskHandle(info, &info->exithandle),
            TaskGetInfo(player, &info->exithandle), NULL, &info->exithandle, NULL, NULL, 0, 0, false);

        if (info->exitMissionSuccess && missiondef->exittextsuccess)
            exittext = saUtilTableAndEntityLocalize(missiondef->exittextsuccess, &vars, player);
        else if (!info->exitMissionSuccess && missiondef->exittextfail)
            exittext = saUtilTableAndEntityLocalize(missiondef->exittextfail, &vars, player);

        // Set to prevent showing the exit text multiple times
        info->exitMissionSuccess = STORY_MESSAGE_HIDE;

        if (exittext)
        {
            START_PACKET(pak, player, SERVER_MISSION_EXIT);
            pktSendString(pak, exittext);
            END_PACKET                    
        }
    }

    StoryArcUnlockPlayerInfo(player);
}

void StoryArc_Tick(Entity *player)
{
    if (player && player->storyInfo && player->storyInfo->delayedSelectedContact)
    {
        // lock is inside if to lower work done... 99.999% of the time this is NULL
        StoryInfo *info = StoryArcLockPlayerInfo(player);
        StoryContactInfo *contactStoryInfo = ContactGetInfo(info, info->delayedSelectedContact);

        if (!contactStoryInfo)
        {
            ContactAdd(player, info, info->delayedSelectedContact, "Contact Finder award");
            contactStoryInfo = ContactGetInfo(info, info->delayedSelectedContact);
        }
        else
        {
            ContactMarkSelected(player, info, contactStoryInfo);
        }

        if (contactStoryInfo && contactStoryInfo->contact->entParent)
        {
            entity_FaceTarget(player, contactStoryInfo->contact->entParent);
            info->delayedSelectedContact = 0;
        }

        StoryArcUnlockPlayerInfo(player);
    }
}

// Returns a player's task that matches the context/subhandles/cpos
StoryTaskInfo* StoryArcGetAssignedTask(StoryInfo* info, StoryTaskHandle *sahandle)
{
    int i, n = eaSize(&info->tasks);
    for(i = 0; i < n; i++)
    {
        StoryTaskInfo* task = info->tasks[i];
        if ( isSameStoryTaskPos( &task->sahandle, sahandle ))
            return task;
    }
    return NULL;
}

// Cleans up any issued tasks on the player's story arc episodes that they don't sync up with their
// task list. This will reset the Failure state of any non RepeatOnFail missions in that episode
void fixupStoryArcs(Entity *player)
{
    int i, n=0;
    StoryInfo *info = StoryArcLockPlayerInfo(player);

    n = eaSize(&info->storyArcs);
    for (i = 0; i < n; i++)
    {
        StoryArcInfo* storyArc = info->storyArcs[i];
        StoryEpisode* current_ep;
        int ep_tasks;
        int count;
        
        if(!storyArc->def || storyArc->episode >= eaSize(&storyArc->def->episodes))
            continue;
        
        current_ep = storyArc->def->episodes[storyArc->episode];

        ep_tasks = eaSize(&(current_ep->tasks));

        for (count = 0; count < ep_tasks; count++)
        {
            if (!BitFieldGet(storyArc->taskComplete, EP_TASK_BITFIELD_SIZE, count) &&
                    BitFieldGet(storyArc->taskIssued, EP_TASK_BITFIELD_SIZE, count) )
            {
                // if a task was Issued but not Completed and is not in their current task list
                // we need to clear the Issued flag
                int m;
                bool matchingTask = false;
                StoryTask* task = current_ep->tasks[count];

                for(m = eaSize(&info->tasks) - 1; m >= 0; m--)
                {
                    if(info->tasks[m]->def == task)
                        matchingTask = true;
                }

                if(!matchingTask)
                    BitFieldSet(storyArc->taskIssued, EP_TASK_BITFIELD_SIZE, count, 0);
            }
        }
    }

    StoryArcUnlockPlayerInfo(player);
}

// StoryArcs

// File Name:        SL3-9_FB_Introdction_H.xls

// StoryArc Name: Ouroboros Initiation

// File Path:          C:\game\data\scripts\Contacts\Flashback\Tasks\_src\SL3-9_FB_Introduction_H.xls

// Alliance:            Hero

// Min Level:          25

// Max Level:         50

// Complete Req:   SL3-9_FB_Intro_H

// StoryArc Flags: StoryArc, MiniStoryArc (This might need to be a separate column for each flag)

 

// Contact:            The Pilgrim (Note, this information comes from the Contact file, not the StoryArc file)

// Location:           Ouroboros (Again, from the Contact file, not the StoryArc file)

// Taskforce:         False (Taken from the contacts page)

 

// # of Missions:    1

// Mission Flags:   taskLong, taskFlashback (Taken from the top of the Mission sheets, might also need to be broken out for each flag)

// Mission Type:    taskCompound (Depending on the number of missions this could be multiple types).

// Villain Group:     5th Column, Contaminated, Rikti, Shivan

// Map Type:         5th Column Flashback, maps\Missions\Outdoor_Missions\Outdoor_Flashback\IOM_FB_Outbreak\IOM_FB_Outbreak.txt, Rikti, maps\Missions\Outdoor_Missions\Outdoor_Flashback\IOM_FB_Atlas_Park_WTO/IOM_FB_Atlas_Park_WTO.txt (This is from the specific mission file, it might also need to be broken out to different columns).

typedef struct InfoCtxt
{
    struct Row
    {
        char **vals;
    } **rows;
    int max_col;
    StashTable idxFromCol;
    StashTable rowFromFname;
    StashTable colFromIdx;
} InfoCtxt;

static void SpawnDefInfo(InfoCtxt *ctxt, const SpawnDef **spawndefs, const SpawnDef *spawnref)
{
    const SpawnDef *spawndef = NULL;
    int j;
    
    for(j = -1; j < eaSize(&spawndefs); ++j)
    {
        struct Row *row;
        if(j < 0) // special case, grab the spawnref.
            spawndef = spawnref;
        else
            spawndef = eaGetConst(&spawndefs,j);
        
        if(!spawndef)
            continue;
        
// if(!stashFindPointer(ctxt.rowFromFname,spawndef->fileName,&row)) ab: filename is the storyarc its in, not the spawndef itself
        {
            row = calloc(sizeof(*row),1);
            stashAddPointer(ctxt->rowFromFname,spawndef->fileName,row,TRUE);
            eaPush(&ctxt->rows,row);
        }
        
        // add fields
        {
            int i;
            
#define FLD_S(F) {OFFSETOF(SpawnDef,F),#F,PARSETYPE_STR}
//#define FLD_I(F) {OFFSETOF(SpawnDef,F),#F,PARSETYPE_S32}
            
#define SET_ROW_VAL(COL,VAL) {                                          \
                int idx;                                                \
                if(!stashFindInt(ctxt->idxFromCol,COL,&idx)) {           \
                    stashAddInt(ctxt->idxFromCol,allocAddString(COL),idx=ctxt->max_col++,true); \
                    stashIntAddPointer(ctxt->colFromIdx,idx,(void*)allocAddString(COL),true); \
                }                                                       \
                eaSetForced(&row->vals,(void*)allocAddString(VAL),idx); \
            }       
            
            static const struct SpawnField
            {
                int offset;
                char *name;
                int type;
            } fields[] = 
                  {
                      FLD_S(fileName),
                      FLD_S(spawntype),
                  };
            
            // easy to grab fields
            for( i = 0; i < ARRAY_SIZE( fields ); ++i ) 
            {
                struct SpawnField const *fld = fields+i;
                char const *val = NULL;
                switch (fld->type)
                {
                case PARSETYPE_STR:
                    val = OFFSET_GET(spawndef,fld->offset,char*); 
                    val = ScriptVarsLookup(&spawndef->vs,(char*)val,0); 
                    val = allocAddString(val);
                    break;
                };
                
                if(!val)
                    printf("couldn't get value for fld %s\n",fld->name);
                
                SET_ROW_VAL(fld->name,val);
            }
            
            // ----------
            // special fields
            {
                int col_idx = 0;
                static char *etmp = NULL;
                
                // actors
                col_idx = 0;
                for( i = eaSize(&spawndef->actors) - 1; i >= 0; --i)
                {
                    const Actor *actor = spawndef->actors[i];
                    bool field_added = FALSE;
#define ACTOR_FIELD(MEMBER)                                             \
                    if(actor->MEMBER){                                  \
                        const char *name_val = ScriptVarsLookup(&spawndef->vs,actor->MEMBER,0); \
                        char actor_colname[128];                        \
                        if(name_val != actor->MEMBER){                  \
                            name_val = textStd(name_val);                         \
                            sprintf(actor_colname,"Actor%i."#MEMBER".k",col_idx); \
                            SET_ROW_VAL(actor_colname, actor->MEMBER);           \
                            sprintf(actor_colname,"Actor%i."#MEMBER".v",col_idx); \
                            SET_ROW_VAL(actor_colname, name_val);           \
                            field_added = TRUE;                         \
                        }                                               \
                    }
                    
                    ACTOR_FIELD(actorname); 
                    ACTOR_FIELD(ally);
                    ACTOR_FIELD(gang);
                    ACTOR_FIELD(displayname);
                    ACTOR_FIELD(shoutrange);
                    ACTOR_FIELD(shoutchance);
                    ACTOR_FIELD(wanderrange);
                    ACTOR_FIELD(notrequired);
//                     ACTOR_FIELD(model);
//                     ACTOR_FIELD(villain);
//                     ACTOR_FIELD(villaingroup);
//                     ACTOR_FIELD(villainrank);
//                     ACTOR_FIELD(villainclass);
                    ACTOR_FIELD(ai_states[ACT_WORKING]);
                    ACTOR_FIELD(ai_states[ACT_INACTIVE]);
                    ACTOR_FIELD(ai_states[ACT_ACTIVE]);
                    ACTOR_FIELD(ai_states[ACT_ALERTED]);
                    ACTOR_FIELD(ai_states[ACT_COMPLETED]);
                    ACTOR_FIELD(ai_states[ACT_SUCCEEDED]);
                    ACTOR_FIELD(ai_states[ACT_FAILED]);
                    ACTOR_FIELD(ai_states[ACT_THANK]);

                    if(field_added)
                        col_idx++;
                }
                
                // scripts
                col_idx = 0;
                for( i = eaSize(&spawndef->scripts) - 1; i >= 0; --i)
                {
                    const ScriptDef *script = spawndef->scripts[i];
                    estrPrintf(&etmp,"ScriptName%i",i);
                    SET_ROW_VAL(etmp, script->scriptname);
                }
                
            }                                
        }
    }    
}


void DumpSpawndefs()
{
    int i;
    char **hdrs = NULL;
    InfoCtxt ctxt = {0};            
    ctxt.max_col = 1; // stupid int stashtables can't handle 0
    ctxt.idxFromCol = stashTableCreateWithStringKeys(128, StashDefault);
    ctxt.rowFromFname = stashTableCreateWithStringKeys(128, StashDefault);
    ctxt.colFromIdx = stashTableCreateInt(128);
    for( i = eaSize(&g_storyarclist.storyarcs) - 1; i >= 0; --i)
    {
        int j;
        StoryArc *sarc = g_storyarclist.storyarcs[i];
        for( j = eaSize(&sarc->episodes) - 1; j >= 0; --j)
        {
            StoryEpisode *sepisode = sarc->episodes[j];
            int i;
            
            // tasks
            for( i = eaSize(&sepisode->tasks) - 1; i >= 0; --i)
            {
                int j;
                StoryTask *task = sepisode->tasks[i];
                SpawnDefInfo(&ctxt,task->spawndefs, task->spawnref);
                
                SpawnDefInfo(&ctxt,SAFE_MEMBER(task->missionref,spawndefs),NULL);
                for( j = eaSize(&task->missiondef) - 1; j >= 0; --j)
                {
                    const MissionDef *missiondef = task->missiondef[j];
                    SpawnDefInfo(&ctxt, missiondef->spawndefs, NULL);
                }
            }
        }
    }
    
    // --------------------
    // printout
    {
        FILE *fp = fopen("c:/storyarcinfo.csv","w");
        if(!fp)
            Errorf("couldn't open c:/storyarcinfo.csv");
        else
        {
            
            // header
            for( i = 0; i < ctxt.max_col; ++i ) 
            {
                char *col;
                stashIntFindPointer(ctxt.colFromIdx,i,&col);
                fprintf(fp,"%s,",col?col:"");
            }
            fprintf(fp,"\n");
            
            // rows
            for( i = 0; i < eaSize(&ctxt.rows); ++i ) 
            {
                struct Row const *row = ctxt.rows[i];
                int j;
                for( j = 0; j < eaSize(&row->vals); ++j)
                {
                    char *val = row->vals[j];
                    fprintf(fp,"%s,",val?val:"");
                }
                fprintf(fp,"\n");
            }
            fclose(fp);
        }
    }            
    
    // cleanup
    stashTableDestroy(ctxt.idxFromCol);
    stashTableDestroy(ctxt.colFromIdx);
    stashTableDestroy(ctxt.rowFromFname);
    for( i = eaSize(&ctxt.rows) - 1; i >= 0; --i)
        eaDestroy(&ctxt.rows[i]->vals);
    eaDestroy(&ctxt.rows);
}

int entity_IsOnStoryArc(Entity *e, const char *storyArcName)
{
    int returnMe = 0;
    int index, count;

    if (e && e->storyInfo && storyArcName)
    {
        count = eaSize(&e->storyInfo->storyArcs);
        for (index = 0; index < count; index++)
        {
            if (e->storyInfo->storyArcs[index] && e->storyInfo->storyArcs[index]->def
                && e->storyInfo->storyArcs[index]->def->filename 
                && strstriConst(e->storyInfo->storyArcs[index]->def->filename, storyArcName))
            {
                returnMe = 1;
                break;
            }
        }
    }

    return returnMe;
}
