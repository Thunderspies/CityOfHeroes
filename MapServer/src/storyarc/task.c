/*\
 *
 *    task.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    basic functions for handling tasks
 *
 */

#include "task.h"
#include "storyarcprivate.h"
#include "dbcomm/dbcomm.h"
#include "entity/character_level.h"
#include "entity/character_eval.h"
#include "comm_game.h"
#include "entity/entPlayer.h"
#include "player/pmotion.h"            // for pmotionGetNeighborhoodFromPos
#include "pnpc.h"
#include "storyarc/pnpcCommon.h"
#include "dbcomm/staticMapInfo.h"
#include "container/team.h"
#include "entity/entity.h"
#include <utilitieslib/utils/scriptvars.h>
#include "taskRandom.h"
#include "reward.h"
#include "entity/supergroup.h"
#include "entity/sgrpserver.h"
#include "gameSys/raidmapserver.h"
#include "gameComm/svr_chat.h"
#include <utilitieslib/version/AppVersion.h>
#include <utilitieslib/components/StringTable.h>
#include "gameSys/sgraid.h"
#include "bases/basesystems.h"
#include "door.h"
#include "entity/TaskforceParams.h"
#include "Taskforce.h"
#include "player/badges_server.h"
#include "container/containerbroadcast.h"
#include "contactInteraction.h"
#include <utilitieslib/components/HashFunctions.h>
#include "teamReward.h"
#include "cmdparse/cmdServer.h"
#include "storyarc.h"
#include "storyarc/missionMapCommon.h"
#include "Notoriety.h"
#include "storyarc/storyarcCommon.h"
#include "entity/buddy_server.h"
#include "zowie.h"
#include "storySend.h"
#include "player/pophelp.h"
#include "dbcomm/logcomm.h"
#include "entity/character_animfx.h"

//#include "AccountInventory.h"
//#include "AccountData.h"

// *********************************************************************************
//  Broken Task List for AutoCompletion and Utility Functions
// *********************************************************************************

typedef struct BrokenTask
{
    char* storyarcname;
    char* taskname;
    char* contactname;
    char* version;

    int                invalid;
    int                compoundPos;

    StoryTaskHandle sahandle;
} BrokenTask;

typedef struct BrokenTaskList
{
    BrokenTask** brokenTasks;
} BrokenTaskList;

BrokenTaskList g_brokentasklist;
int g_ignorebrokentasklist = 1; // defaults to 1 unless overriden through commandline parameter

TokenizerParseInfo ParseBrokenTask[] =
{
    {    "{",                TOK_START,        0 },
    {    "Task",                TOK_STRING(BrokenTask,taskname,0)        },
    {    "SubtaskPosition",    TOK_INT(BrokenTask,compoundPos,0)    },
    {    "StoryArc",            TOK_STRING(BrokenTask,storyarcname,0)    },
    {    "Contact",            TOK_STRING(BrokenTask,contactname,0)    },
    {    "Version",            TOK_STRING(BrokenTask,version,0)        },
    {    "}",                TOK_END,            0 },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseBrokenTaskList[]=
{
    {    "BrokenTask",        TOK_STRUCT(BrokenTaskList,brokenTasks,ParseBrokenTask) }, 
    {    "", 0, 0 }
};

// Convert the Broken tasks from strings in Contact and Task Handles
void BrokenTaskListPostProcess()
{
    int i;
    const char* version = getExecutablePatchVersion("CoH Server");
    g_ignorebrokentasklist |= (!version || !stricmp(version, "Could not find registry key") || strstriConst(version, "dev"));

    if (g_ignorebrokentasklist)
        return;
    
    for (i = 0; i < eaSize(&g_brokentasklist.brokenTasks); i++)
    {
        BrokenTask* broken = g_brokentasklist.brokenTasks[i];

        // Invalid if theres no version, of it it is different from the current version
        broken->invalid = (!broken->version || stricmp(broken->version, version));
        
        // Handle story arc tasks
        if (broken->storyarcname)
        {
            broken->sahandle = StoryArcGetHandleLoose(broken->storyarcname);
            if (broken->taskname)
            {
                const StoryArc* arc = StoryArcDefinition(&broken->sahandle);
                if (arc)
                {
                    int i, n = eaSize(&arc->episodes);
                    int index = 0;
                    StoryTask* task = NULL;
                    for (i = 0; i < n; i++)
                    {
                        StoryEpisode* episode = arc->episodes[i];
                        int j, numTasks = eaSize(&episode->tasks);
                        for (j =0 ; j < numTasks; j++)
                        {
                            if (episode->tasks[j]->logicalname && !stricmp(episode->tasks[j]->logicalname, broken->taskname))
                                broken->sahandle.subhandle = StoryIndexToHandle(index);
                            else
                                index++;
                        }
                    }
                }
            }            
        }
        // Handle regular tasks
        else if (broken->contactname)
        {
            broken->sahandle.context = ContactGetHandleLoose(broken->contactname);
            if (broken->taskname)
            {
                const ContactDef* def = ContactDefinition(broken->sahandle.context);
                if (def)
                    broken->sahandle.subhandle = TaskFromLogicalName(def, broken->taskname);
            }
        }
        if (!broken->sahandle.subhandle || !broken->sahandle.context)
            broken->invalid = 1;
    }
}

// Load in the list of broken tasks for auto completion
void LoadBrokenTaskList()
{
    ParserLoadFiles(0, "server/db/brokentasklist.db", NULL, PARSER_SERVERONLY, ParseBrokenTaskList, &g_brokentasklist, NULL, NULL, NULL);
    BrokenTaskListPostProcess();
}

// Check to see if a task is broken
// Linear search because this list should never ever be that big, if it is, we have problems
int TaskIsBroken(StoryTaskInfo* task)
{
    int i;
    
    if (g_ignorebrokentasklist)
        return 0;

    for (i = 0; i < eaSize(&g_brokentasklist.brokenTasks); i++)
    {
        BrokenTask* broken = g_brokentasklist.brokenTasks[i];
        if (broken->invalid)
            continue;
        if ( isSameStoryTaskPos(&task->sahandle, &broken->sahandle) )
            return 1;
    }

    return 0;
}


// *********************************************************************************
//  Task info utility functions
// *********************************************************************************

// returns active information for specified task
StoryTaskInfo* TaskGetInfo(Entity* player, StoryTaskHandle *sahandle)
{
    int i, n;
    StoryInfo* info = StoryInfoFromPlayer(player);
    if (!info) 
        return NULL;

    if (TaskIsSGMissionEx(player, sahandle) )
        return player->supergroup->activetask;

    n = eaSize(&info->tasks);
    for (i = 0; i < n; i++)
    {
        if ( isSameStoryTask( &info->tasks[i]->sahandle, sahandle ) )
            return info->tasks[i];
    }
    return NULL;
}

U32* TaskGetClueStates(StoryInfo* info, StoryTaskInfo* task)
{
    if (!info || !task)
        return NULL;

    if (isStoryarcHandle(&task->sahandle))
    {
        StoryContactInfo* contactInfo = TaskGetContact(info, task);
        if(contactInfo)
        {
            StoryArcInfo* storyarc = StoryArcGetInfo(info, contactInfo);
            if (storyarc) 
                return storyarc->haveClues;
        }
        return NULL;
    }
    else
    {
        return task->haveClues;
    }
}

int* TaskGetNeedIntroFlag(StoryInfo* info, StoryTaskInfo* task)
{
    if (isStoryarcHandle(&task->sahandle))
    {
        StoryContactInfo* contactInfo = TaskGetContact(info, task);
        if(contactInfo)
        {
            StoryArcInfo* storyarc = StoryArcGetInfo(info, contactInfo);
            if (storyarc) 
                return &storyarc->clueNeedsIntro;
        }
        return NULL;
    }
    else
    {
        return &task->clueNeedsIntro;
    }
}

// adds appropriate title information if it exists
void TaskAddTitle(const StoryTask* taskDef, char *response, Entity *e, ScriptVarsTable *table)
{
    int found = false; 

    if (taskDef->taskTitle && stricmp(taskDef->taskTitle, ""))
    {
        strcat(response, saUtilTableAndEntityLocalize(taskDef->taskTitle, table, e));
        strcat(response, "<br>");
        found = true;
    }

    if (taskDef->taskSubTitle && stricmp(taskDef->taskSubTitle, ""))
    {
        strcat(response, saUtilTableAndEntityLocalize(taskDef->taskSubTitle, table, e));
        strcat(response, "<br>");
        found = true;
    }

    if (taskDef->taskIssueTitle && stricmp(taskDef->taskIssueTitle, ""))
    {
        strcat(response, saUtilTableAndEntityLocalize(taskDef->taskIssueTitle, table, e));
        strcat(response, "<br>");
        found = true;
    }

    if (found)
    {
        strcat(response, "<br>");
    }
}

void TaskMarkSelected(StoryInfo* info, StoryTaskInfo* task)
{
    // complete tasks just select their corresponding contact
    if (TASK_COMPLETE(task->state))
    {
        StoryContactInfo* contactInfo = TaskGetContact(info, task);
        if (contactInfo) 
        {
            info->selectedContact = contactInfo;
            info->selectedTask = 0; // prevent collisions
        }
    }
    else
    {
        info->selectedTask = task;
        info->selectedContact = 0; // prevent collisions
    }
}

ContactHandle TaskGetContactHandle(StoryInfo* info, StoryTaskInfo* task)
{
    ContactHandle ret;
    if (isStoryarcHandle(&task->sahandle))
        ret = StoryArcGetContactHandle(info, &task->sahandle);
    else
        ret = task->sahandle.context;
    return ret;
}

ContactHandle TaskGetContactHandleFromStoryTaskHandle(StoryInfo* info, StoryTaskHandle* sahandle)
{
    ContactHandle ret;
    if (isStoryarcHandle(sahandle))
        ret = StoryArcGetContactHandle(info, sahandle);
    else
        ret = sahandle->context;
    return ret;
}

ContactHandle EntityTaskGetContactHandle(Entity *e, StoryTaskInfo* task)
{
    StoryInfo *info = StoryInfoFromPlayer(e);

    return TaskGetContactHandle(info, task);
}

StoryContactInfo* TaskGetContact(StoryInfo* info, StoryTaskInfo* task)
{
    return ContactGetInfo(info, TaskGetContactHandle(info, task));
}

// get position of task in active list
int TaskGetIndex(StoryInfo* info, StoryTaskInfo* task)
{
    int i, n;
    n = eaSize(&info->tasks);
    for (i = 0; i < n; i++)
    {
        if (info->tasks[i] == task)
            return i;
    }
    return -1;
}

// Returns true if this task has the same context as an earlier task meaning its a duplicate
// This seems to be a certain form of corruption that arose in Korea.
int TaskIsDuplicate(StoryInfo* info, int taskIndex)
{
    int i, n = eaSize(&info->tasks);
    for (i = 0; i < n; i++)
    {
        if ((i != taskIndex) && info->tasks[i] && info->tasks[taskIndex] && 
            (info->tasks[i]->sahandle.context == info->tasks[taskIndex]->sahandle.context) && 
            (info->tasks[i]->sahandle.bPlayerCreated == info->tasks[taskIndex]->sahandle.bPlayerCreated))
            return 1;
    }
    return 0;
}

// returns true if the current subtask is a mission
int TaskIsMission(StoryTaskInfo* task)
{
    if(task && (task->sahandle.bPlayerCreated || (task->def && task->def->type == TASK_MISSION)) )
        return 1;
    else
        return 0;
}

// is this mission a 'zone-transfer' type?
int TaskIsZoneTransfer(StoryTaskInfo* task)
{
    const MissionDef* def;

    if (!TaskIsMission(task)) 
        return 0;
    def = TaskMissionDefinition(&task->sahandle);
    return def && MissionIsZoneTransfer(def);
}

// does this mission allow teleporting on completion?
int TaskCanTeleportOnComplete(StoryTaskInfo* task)
{
    const MissionDef* def;

    if (!TaskIsMission(task)) 
        return 0;
    def = TaskMissionDefinition(&task->sahandle);
    if (!def) 
        return 0;
    return !(def->flags & MISSION_NOTELEPORTONCOMPLETE);
}

// get the mission task from the player, returns NULL if not on mission
// 0 will get the first mission for the player, 1 second.. continue until NULL returned
StoryTaskInfo* TaskGetMissionTask(Entity* player, int index)
{
    StoryInfo* info = StoryInfoFromPlayer(player);

    int i, n;
    n = eaSize(&info->tasks);
    for (i = 0; i < n; i++)
    {
        if (TaskIsMission(info->tasks[i]))
        {
            if (index <= 0) 
                return info->tasks[i];
            index--;
        }
    }
    return NULL;
}

// count of how many tasks are actually assigned to you
int TaskAssignedCount(int player_id, StoryInfo* info)
{
    int i, n, count;
    n = eaSize(&info->tasks);
    if (info->taskforce)
    {
        count = 0;
        for (i = 0; i < n; i++)
        {
            if (info->tasks[i]->assignedDbId == player_id)
                count++;
        }
        return count;
    }
    return n;
}

void TaskReassignAll(int to_id, StoryInfo* info)
{
    int i, n;

    // iterate through all tasks
    n = eaSize(&info->tasks);
    for (i = 0; i < n; i++)
    {
        StoryTaskInfo* task = info->tasks[i];
        task->assignedDbId = to_id;
    }
}

int TaskCanBeAutoCompleted(Entity* player, StoryTaskInfo* task)
{
    // tasks cannot be auto-completed while the player is in taskforce mode
    if (PlayerInTaskForceMode(player))
        return false;

    // can't auto-complete missions that are flagged as no-auto-complete
    if (TASK_NO_AUTO_COMPLETE(task->def))
        return false;

    // can't auto-complete missions that are timed
    if (task->def->timeoutMins > 0)
        return false;

    // can't auto-complete missions that are completed
    if (TASK_COMPLETE(task->state) || TASK_MARKED(task->state))
        return false;

    // otherwise, this can be auto-completed
    return true;
}

int TaskCanBeAbandoned(Entity *player, StoryTaskInfo *task)
{
    Teamup* team = player->teamup;

    // can't abandon missions that are flagged as not abandonable
    if (!(TASK_IS_ABANDONABLE(task->def)) || (task->compoundParent && !(TASK_IS_ABANDONABLE(task->compoundParent))))
        return false;

    // can't abandon missions that are completed
    if (TASK_COMPLETE(task->state) || TASK_MARKED(task->state))
        return false;

    // can't abandon sg missions because they have no specific owner
    if (TASK_IS_SGMISSION(task->def))
        return false;

    // can't abandon active missions even if there's no-one on the map
    if (team && (team->activetask->assignedDbId == player->db_id)
        && isSameStoryTask(&team->activetask->sahandle, &task->sahandle))
    {
        return false;
    }

    // otherwise, this can be abandoned
    return true;
}

int TaskIsZowie(StoryTaskInfo* task)
{
    if (!task || !task->def)
        return 0;
    return (task->def->type == TASK_ZOWIE);
}

int TaskCompleted(StoryTaskInfo* task)
{
    return !TASK_INPROGRESS(task->state);
}

int TaskCompletedAndIncremented(StoryTaskInfo* task)
{
    return (TASK_SUCCEEDED == task->state || TASK_FAILED == task->state);
}

int TaskFailed(StoryTaskInfo* task)
{
    return (TASK_FAILED == task->state || TASK_MARKED_FAILURE == task->state);
}

int TaskInProgress(Entity* player, StoryTaskHandle *sahandle)
{
    StoryTaskInfo* task = TaskGetInfo(player, sahandle);
    if (!task) 
        return 0;
    return TASK_INPROGRESS(task->state);
}

// returns true if the task is currently attached by player's teamup
int TaskIsActiveMission(Entity* player, StoryTaskInfo* task)
{
    return task && player && player->teamup_id && isSameStoryTask( &player->teamup->activetask->sahandle, &task->sahandle) &&
        (player->teamup->activetask->assignedDbId == player->db_id || PlayerInTaskForceMode(player));
}

// returns true if the task is the supergroup's task, based on the context and subhandle
int TaskIsSGMissionEx(Entity* player, StoryTaskHandle *sahandle)
{
    return player && player->supergroup && player->supergroup->activetask && 
           isSameStoryTask( &player->supergroup->activetask->sahandle, sahandle ) &&
           player->supergroup->activetask->assignedDbId == player->supergroup_id;
}

int TaskGetForcedLevel(StoryTaskInfo* task)
{
    const StoryTask* rootdef = task->compoundParent? task->compoundParent: task->def;
    return rootdef->forceTaskLevel;    
}

// *********************************************************************************
//  Adding and removing task info
// *********************************************************************************

static int ValidateVillainName(const char* name)
{
    const VillainDef* def;

    if(!name)
        return 0;

    // Is the name a valid villain name?
    def = villainFindByName(name);
    if(def)
        return 1;

    // Is the name a valid villain group name?
    if(villainGroupGetEnum(name))
        return 1;

    return 0;
}


// initiate the current taskdef, called when starting a task or changing subtasks for a compound task
static int SubTaskInstantiate(Entity* player, StoryInfo* info, StoryTaskInfo* task)
{
    U32 failtime = 0;

    // clear all task-specific fields first
    task->state = TASK_ASSIGNED;
    task->spawnGiven = 0;
    task->doorId = 0;
    task->doorMapId = 0;
    task->missionMapId = 0;
    task->doorPos[0] = task->doorPos[1] = task->doorPos[2] = 0;
    task->villainType[0] = 0;
    task->curKillCount = 0;
    task->villainType2[0] = 0;
    task->curKillCount2 = 0;
    task->deliveryTargetName[0] = 0;
    task->nextLocation = 0;

    // set the timeout for this subtask
    if (task->def->timeoutMins && !TASK_DELAY_START(task->def))
    {
        task->failOnTimeout = true;
        task->timerType = -1;
        task->timeout = dbSecondsSince2000() + task->def->timeoutMins * 60;
        task->timezero = task->timeout;
    }
    else
    {
        task->failOnTimeout = false;
        task->timerType = -1;
        task->timeout = 0;
        task->timezero = 0;
    }

    switch(task->def->type)
    {
        case TASK_KILLX:
        {
            // Kill task specific initialization.

            // Make sure that the task has both villain type and count
            if(!task->def->villainType || !task->def->villainCount)
            {
                ErrorFilenamef(task->def->filename, "Kill task doesn't include villain type and count");
                return 0;
            }

            // If a second villain type exists, perform error checks on the parameters.
            if(task->def->villainType2 && !task->def->villainCount2)
            {
                ErrorFilenamef(task->def->filename, "Kill task doesn't include count for second villain type");
                return 0;
            }

            // Look up and validate the villain type.
            strcpy_s(SAFESTR(task->villainType), ScriptVarsLookup(&task->def->vs, task->def->villainType, task->seed)); // remove later
            if(!ValidateVillainName(ScriptVarsLookup(&task->def->vs, task->def->villainType, task->seed)))
            {
                ErrorFilenamef(task->def->filename, "Invalid villain type %s specified for kill task.  Need specific villain name or villain group.", task->villainType);
                return 0;
            }

            if(task->def->villainType2)
            {
                strcpy_s(SAFESTR(task->villainType2), ScriptVarsLookup(&task->def->vs, task->def->villainType2, task->seed)); // remove later
                if(!ValidateVillainName(ScriptVarsLookup(&task->def->vs, task->def->villainType2, task->seed)))
                {
                    ErrorFilenamef(task->def->filename, "Invalid villain type %s specified for kill task.  Need specific villain name or villain group.", task->villainType2);
                    return 0;
                }
            }
            break;
        }

        case TASK_CONTACTINTRO:
        case TASK_DELIVERITEM:
        {
            if(!task->def->deliveryTargetNames || !eaSize(&task->def->deliveryTargetNames))
            {
                ErrorFilenamef(task->def->filename, "Delivery target not given for task");
                return 0;
            }
            strcpy_s(SAFESTR(task->deliveryTargetName), ScriptVarsLookup(&task->def->vs, task->def->deliveryTargetNames[0], task->seed)); // remove later
            break;
        }

        case TASK_VISITLOCATION:
        {
            task->nextLocation = 0;
            if(!task->def->visitLocationNames)
            {
                ErrorFilenamef(task->def->filename, "Location not given for task");
                return 0;
            }
            info->visitLocationChanged = 1;
            break;
        }

        case TASK_MISSION:
        {
            const MissionDef *missionDef;
            int isVillain = ENT_IS_ON_RED_SIDE(player);

            missionDef = task->def->missionref;
            if(!missionDef && task->def->missiondef)
                missionDef = task->def->missiondef[0];

            if (missionDef->doortype)
            {
                if(!AssignMissionDoor(info, task, isVillain))
                {
                    // Allow a task to be added without a valid door only on local map server
                    // This allows the designers to be working on a map and then pull up a mission on it
                    // immediately using startmission
                    if (db_state.local_server)
                    {
                        return 1;
                    }
                    return 0;
                }
            }
            else if (!missionDef->doorNPC) // if true, don't need to do any door assignment
            {
                return 0;
            }
            break;
        }

        case TASK_CRAFTINVENTION:
        {
            break;
        }

        case TASK_TOKENCOUNT:
        {
            // Reset token
            if (TOKENCOUNT_RESET_AT_START(task->def) && task->def->tokenName != NULL)
            {
                RewardToken *pToken = getRewardToken( player, task->def->tokenName );

                if (pToken)
                {
                    pToken->val = 0;
                    pToken->timer = timerSecondsSince2000();
                }
        
            }

            break;
        }

        case TASK_ZOWIE:
        {
            //set all bits in completeSideObjectives that aren't to be used.
            if (getPopHelpStateByTag(player, "CODEPH_Zowie") == PopHelpState_Untriggered)
            {
                triggerPopHelpEventHappenedByTag(player, "CODEPH_Zowie");
            }
            return zowie_setup(task, 1);
        }

        case TASK_GOTOVOLUME:
        {
            if (!task->def->volumeMapName || !task->def->volumeName)
            {
                ErrorFilenamef(task->def->filename, "Go To Volume task doesn't have a volume name specified");
                return 0;
            }

            break;
        }

        default:
            Errorf("task.c - invalid task type in SubTaskInstantiate");
            return 0;
    }
    return 1;
}

// Create and initialize a StoryTaskInfo.
// Returns NULL on error.
static StoryTaskInfo* TaskInstantiate(StoryTaskHandle *sahandle, unsigned int seed, Entity* player, int setlevel, int limitlevel, int villainGroup, StoryDifficulty *pDiff)
{
    StoryInfo* info;
    StoryTaskInfo* task;
    int forcedlevel;

    if (!(task = storyTaskInfoCreate(sahandle)))
        return NULL;

    forcedlevel = TaskGetForcedLevel(task);
    task->state = TASK_ASSIGNED;
    task->seed = seed;
    task->assignedDbId = player->db_id;
    task->assignedTime = dbSecondsSince2000();
    if (forcedlevel)
    {
        task->level = forcedlevel;
        memset( &task->difficulty, 0, sizeof(StoryDifficulty) );
    }
    else
    {
        task->level = setlevel;
        if (limitlevel && task->level > limitlevel)
            task->level = limitlevel;

        if( pDiff )
            difficultyCopy(&task->difficulty, pDiff);
    }

    task->sahandle.compoundPos = 0;
    info = StoryInfoFromPlayer(player);

    //handle random assignment of villain groups and map sets
    if (getVillainGroupFromTask(task->def)==villainGroupGetEnum("Random") || getMapSetFromTask(task->def)==MAPSET_RANDOM)
    {
        VillainGroupEnum vge=villainGroup;
        MapSetEnum mse;
        char * filename;
        if (villainGroup)
            getRandomMapSetForVillainGroup(player,&mse,&filename,character_GetExperienceLevelExtern(player->pchar),villainGroup,seed);
        else
            getRandomFields(player,&vge,&mse,&filename,character_GetExperienceLevelExtern(player->pchar),task->def,seed);
        task->villainGroup=vge;
        task->mapSet=mse;
    }
    if (!SubTaskInstantiate(player, info, task))
        goto failedExit;


    return task;
failedExit:
    storyTaskInfoDestroy(task);
    return NULL;
}

// returns whether the subtask was incremented correctly.  If no more subtasks are
// available, returns false
static int TaskIncrementSubTask(Entity* player, StoryInfo* info, StoryTaskInfo* task)
{
    if (!task->compoundParent || task->sahandle.compoundPos+1 < 0 || task->sahandle.compoundPos+1 >= eaSize(&task->compoundParent->children))
        return 0;

    task->sahandle.compoundPos++;
    task->def = task->compoundParent->children[task->sahandle.compoundPos];
    SubTaskInstantiate(player, info, task);
    return 1;
}

void TaskMarkIssued(StoryInfo* info, StoryContactInfo* contactInfo, StoryTaskInfo* task, int isIssued)
{
    const StoryTask* parent;

    if (isStoryarcHandle(&task->sahandle))
    {
        StoryArcInfo* storyarc = StoryArcGetInfo(info, contactInfo);
        int offset;
        storyarc->clueNeedsIntro = 0;    // reset any time we issue a new task
        StoryArcTaskEpisodeOffset(&task->sahandle, NULL, &offset);

        if (offset == -1)
        {
            log_printf("storyerrs.log", "TaskMarkIssued: I tried to mark %s(%i,%i,%i,%i), but it doesn't belong to a storyarc?",
                task->def->logicalname, SAHANDLE_ARGSPOS(task->sahandle));
        }
        else
        {
            BitFieldSet(storyarc->taskIssued, TASK_BITFIELD_SIZE, offset, isIssued);
        }
    }
    else
    {
        BitFieldSet(contactInfo->taskIssued, TASK_BITFIELD_SIZE, StoryHandleToIndex(&task->sahandle), isIssued);
        parent = TaskParentDefinition(&task->sahandle);
        if (TASK_IS_UNIQUE(parent))
        {
            BitFieldSet(info->uniqueTaskIssued, UNIQUE_TASK_BITFIELD_SIZE, UniqueTaskGetIndex(parent->logicalname), isIssued);
        }
    }
}

// add an active task to the supergroup
int TaskAddToSupergroup(Entity* player, StoryTaskHandle *sahandle, unsigned int seed, int limitlevel, int villainGroup)
{
    int level;
    StoryTaskInfo* task;

    if (!player || !player->supergroup)
        return 0;

    if (teamLock(player, CONTAINER_SUPERGROUPS))
    {
        // If a SG task already exists, don't add a new one
        if (player->supergroup->activetask)
        {
            teamUpdateUnlock(player, CONTAINER_SUPERGROUPS);
            return 0;
        }
    
        // SGMissions should be forced, but use the player level just in case
        level = character_GetExperienceLevelExtern(player->pchar);
        task = TaskInstantiate(sahandle, seed, player, level, limitlevel, villainGroup, 0);

        // If anything goes wrong, abort task assignement.
        if (!task)
        {
            teamUpdateUnlock(player, CONTAINER_SUPERGROUPS);
            return 0;
        }
        task->assignedDbId = player->supergroup_id ;
        player->supergroup->activetask = task;

        // Flag all members to get an update
        sgroup_UpdateAllMembersAnywhere(player->supergroup);

        teamUpdateUnlock(player, CONTAINER_SUPERGROUPS);

        // log
        LOG_ENT(player, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "SGTask:Add %s", task->def->logicalname );

        return 1;
    }
    printf("task.c - couldn't lock the supergroup!\n");
    return 0;
}

// add an active task, 0 on error
int TaskAdd(Entity* player, StoryTaskHandle *sahandle, unsigned int seed, int limitlevel, int villainGroup)
{
    int i, n, max, level;
    StoryInfo* info;
    StoryTaskInfo* task;
    const StoryTask* parentdef;
    StoryContactInfo* contactInfo;
    StoryDifficulty pDiff = {0};

    if (!player) 
        return 0;

    info = StoryInfoFromPlayer(player);

    // check and see if we already have the tasks
    n = eaSize(&info->tasks);
    for (i = 0; i < n; i++)
    {
        if( isSameStoryTask( &info->tasks[i]->sahandle, sahandle) )
            return 0;
    }
    if (n && info->taskforce) // if on a task force, we can only have a single task
        return 0;

    max = StoryArcMaxTasks(info); // StoryArc Limit
    if (n >= max)
    {
        log_printf("storyerrs.log", "TaskAdd: Attempted to add a task which would have put the player over the limit!\n");
        return 0;
    }

    // Attempt to create a StoryTaskInfo from the task definition.
    if (info->taskforce && player->taskforce && !sahandle->bPlayerCreated)
    {
        level = player->taskforce->level;
        if( TaskForceIsOnFlashback(player) || TaskForceIsScalable(player))
            difficultySetFromPlayer( &pDiff, player);
    }
    else
    {
        level = character_GetExperienceLevelExtern(player->pchar);
        difficultySetFromPlayer( &pDiff, player);
    }

    task = TaskInstantiate(sahandle, seed, player, level, limitlevel, villainGroup, &pDiff);

    // If anything goes wrong, abort task assignement.
    if(!task)
        return 0;

    eaPush(&info->tasks, task);

    // log
    LOG_ENT(player, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Task:Add %s", task->def->logicalname);

    // Update the contact from which the task was issued.
    contactInfo = TaskGetContact(info, task);
    ContactMarkDirty(player, info, contactInfo);
    TaskMarkIssued(info, contactInfo, task, 1);
    TaskMarkSelected(info, task);
    parentdef = TaskParentDefinition(sahandle);
    if (parentdef && parentdef->taskBegin)
    {
        StoryRewardApply(*parentdef->taskBegin, player, info, task, contactInfo, NULL, NULL);
    }

    if (TaskForceIsOnFlashback(player))
    {
        if (parentdef && parentdef->taskBeginFlashback)
        {
            StoryRewardApply(*parentdef->taskBeginFlashback, player, info, task, contactInfo, NULL, NULL);
        }
    }
    else
    {
        if (parentdef && parentdef->taskBeginNoFlashback)
        {
            StoryRewardApply(*parentdef->taskBeginNoFlashback, player, info, task, contactInfo, NULL, NULL);
        }
    }

    // check token count tasks at launch time to see if they have already been completed.
    if (task->def->type == TASK_TOKENCOUNT)
    {
        TokenCountTasksModify(player, task->def->tokenName);
    }

    // check volume tasks at launch time to see if you're already in the volume.
    if (task->def->type == TASK_GOTOVOLUME
        && strstri(db_state.map_name, task->def->volumeMapName))
    {
        VolumeList *volumeList = &player->volumeList;
        int volumeIndex;

        if (volumeList)
        {
            // Look through its volume list and see if it's in a target volume
            for (volumeIndex = 0; volumeIndex < volumeList->volumeCount; volumeIndex++)
            {
                Volume *entVolume = &volumeList->volumes[volumeIndex];
                if (entVolume && entVolume->volumePropertiesTracker)
                {
                    char *entVolumeName = groupDefFindPropertyValue(entVolume->volumePropertiesTracker->def,"NamedVolume");
                    if (!entVolumeName) // For now just ignore any volume without this property
                        continue;

                    if (!stricmp(task->def->volumeName, entVolumeName))
                    {
                        //update clients before TaskComplete in case task is deleted on complete
                        ContactMarkDirty(player, info, TaskGetContact(info, task));
                        TaskComplete(player, info, task, NULL, 1);
                        return 1; // might not have task data anymore if task was "don't return to contact"
                    }
                }
            }
        }
    }

    if (TaskIsBroken(task))
    {
        TaskComplete(player, info, task, NULL, 1);
        chatSendToPlayer(player->db_id, saUtilLocalize(player, "TaskCompleteDueToError"), INFO_SVR_COM, 0);
    }

    return 1;
}

// remove an active task
// only called internally to the task code
void TaskRemove(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo, StoryTaskInfo* task)
{
    int i, n;

    // look for task
    n = eaSize(&info->tasks);
    for (i = 0; i < n; i++)
    {
        if (task == info->tasks[i])
        {
            int isTeamTask = TaskIsTeamTask(task, player);
            eaRemove(&info->tasks, i);
            storyTaskInfoDestroy(task);
            ContactMarkDirty(player, info, contactInfo);
            info->contactFullUpdate = 1;
            if(isTeamTask)
                TeamTaskClear(player);    //will run other queued commands from dbserver.
            return;
        }
    }
}

// Abandoning a task is removing it from your task list without completing it.
// An abandoned task can be readded to your task list by talking to the contact again.
void TaskAbandon(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo, StoryTaskInfo* task, int abandonEvenIfRunning)
{
    if (abandonEvenIfRunning || !TaskIsRunningMission(player, task))
    {
        const StoryTask *taskDef = TaskDefinition(&task->sahandle);
        if (taskDef && taskDef->taskAbandon)
        {
            StoryRewardApply(*taskDef->taskAbandon, player, info, task, contactInfo, NULL, NULL);
        }
        dbLog("TaskAbandon", player, "Player has abandoned \"%s\" task.", taskDef->filename);
        TaskMarkIssued(info, contactInfo, task, 0);
        TaskRemove(player, info, contactInfo, task);
    }
}

void TaskAbandonInvalidContacts(Entity *player)
{
    int count;
    StoryContactInfo *ci;
    ContactAlliance ca;
    ContactUniverse cu;

    if (player && player->storyInfo)
    {
        ca = ENT_IS_HERO(player) ? ALLIANCE_HERO : ALLIANCE_VILLAIN;
        cu = ENT_IS_IN_PRAETORIA(player) ? UNIVERSE_PRAETORIAN : UNIVERSE_PRIMAL;

        // Must go backwards since we'll be removing from the array.
        for (count = eaSize(&player->storyInfo->tasks) - 1; count >= 0; count--)
        {
            ci = TaskGetContact(player->storyInfo, player->storyInfo->tasks[count]);

            // You can't work with contacts which don't acknowledge your new
            // alliance or universe.
            if (!ci->contact->def 
                || !PlayerAllianceMatchesDefAlliance(ca, ci->contact->def->alliance) 
                || !PlayerUniverseMatchesDefUniverse(cu, ci->contact->def->universe))
            {
                TaskAbandon(player, player->storyInfo, ci, player->storyInfo->tasks[count], 0);
            }
        }
    }
}

// Closing a task is removing it from your task list after it has already been completed.
// Closing the task will not complete it, but should only be called after it has been completed.
void TaskClose(Entity* player, StoryContactInfo* contactInfo, StoryTaskInfo* task, ContactResponse* response)
{
    StoryInfo* info = StoryInfoFromPlayer(player);
    const StoryTask* rootdef = task->compoundParent;
    if (!rootdef) 
        rootdef = task->def;

    // If it's a supergroup mission, handle it appropriately
    if (TASK_IS_SGMISSION(task->def))
    {
        if (TaskIsTeamTask(task, player))
            TeamTaskClear(player);
        storyTaskInfoDestroy(player->supergroup->activetask);
        player->supergroup->activetask = NULL;
        return;
    }

    // if it's a story arc task, tell the story arc system
    if (isStoryarcHandle(&task->sahandle))
    {
        StoryArcCloseTask(player, contactInfo, task, response);
    }
    else 
    {
        // if it's repeatable and was failed, clear the issued flag
        if (TASK_CAN_REPEATONFAILURE(rootdef) && task->state == TASK_FAILED)
            BitFieldSet(contactInfo->taskIssued, TASK_BITFIELD_SIZE, StoryHandleToIndex(&task->sahandle), 0);
        if (TASK_CAN_REPEATONSUCCESS(rootdef) && task->state == TASK_SUCCEEDED)
            BitFieldSet(contactInfo->taskIssued, TASK_BITFIELD_SIZE, StoryHandleToIndex(&task->sahandle), 0);
    }

    TaskRemove(player, info, contactInfo, task);
}

// Finds a task that is flagged for team completion
StoryTaskInfo* TaskFindTeamCompleteFlaggedTask(Entity* player, StoryInfo* info)
{
    int i, n = eaSize(&info->tasks);
    for(i = 0; i < n; i++)
        if (info->tasks[i]->teamCompleted)
            return info->tasks[i];
    return NULL;
}

// Handles the response of the team complete prompt
void TaskReceiveTeamCompleteResponse(Entity* player, int response, int neverask)
{
    StoryInfo* info = StoryArcLockInfo(player);
    StoryTaskInfo* task = TaskFindTeamCompleteFlaggedTask(player, info);
    if (!task)
    {
        dbLog("cheater",NULL,"Player with authname %s tried to team complete a task without being prompted\n", player->auth_name);
    }
    else
    {
        if (response)
        {
            task->teamCompleted = 0;
            TaskComplete(player, info, task, NULL, 1);
        }
        else
        {
            // They said no, so apply secondary reward, this will only come upon success
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
                rewardStoryApplyToDbID(player->db_id, reward->secondaryReward, getVillainGroupFromTask(task->def), task->teamCompleted, task->difficulty.levelAdjust, &vars, REWARDSOURCE_TASK);
            }
            if (reward2 && eaSize(&reward2->secondaryReward))
            {
                ScriptVarsTable vars = {0};
                saBuildScriptVarsTable(&vars, player, player->db_id, TaskGetContactHandle(info, task), task, NULL, NULL, NULL, NULL, 0, 0, false);
                rewardStoryApplyToDbID(player->db_id, reward2->secondaryReward, getVillainGroupFromTask(task->def), task->teamCompleted, task->difficulty.levelAdjust, &vars, REWARDSOURCE_TASK);
            }

            // Restore their tasks original notoriety
            difficultyCopy( &task->difficulty, &info->difficulty );
            task->teamCompleted = 0;
        }
    }
    // 1 is always autocomplete, 2 is never autocomplete
    if (neverask && player->pl)
        player->pl->teamCompleteOption = response?1:2;
    StoryArcUnlockInfo(player);
}

// Returns 1 if player deserves secondary reward
int TaskPromptTeamComplete(Entity* player, StoryInfo* info, StoryTaskInfo* task, int levelAdjust, int level)
{
    if (player->pl->teamCompleteOption == 0)
    {
        task->teamCompleted = level;
        task->difficulty.levelAdjust = levelAdjust;
        START_PACKET(pak, player, SERVER_TEAMCOMPLETE_PROMPT);
        END_PACKET
        return 0;
    }
    else if (player->pl->teamCompleteOption == 1)
    {
        // Auto-complete it
        task->difficulty.levelAdjust = levelAdjust;
        TaskComplete(player, info, task, NULL, 1);
        return 0;
    }
    else // if (player->pl->teamCompleteOption == 2)
    {
        // Do nothing, award player with 2ndary reward
        return 1;
    }
}

// Completes the supergroup task for the supergroup
// Any major changes to how task completion works need to be done here too
void TaskCompleteSG(Entity* player, int success)
{
    int i, count;

    if (teamLock(player, CONTAINER_SUPERGROUPS))
    {
        if (player->supergroup->activetask)
        {
            static StoryContactInfo sgRewardContact;
            const char *typeStr;
            char buf[1000]; // for log output
            StoryTaskInfo* sgTask = player->supergroup->activetask;
            Contact* contact = GetContact(sgTask->sahandle.context);

            // log to dbLog
            if(success)
            {
                sgTask->state = TASK_SUCCEEDED;
                strcpy(buf, "SGTask:Success");
            }
            else
            {
                sgTask->state = TASK_FAILED;
                strcpy(buf, "SGTask:Failure");
            }
            typeStr = StaticDefineIntRevLookup(ParseTaskType, sgTask->def->type);
            LOG_ENT(player, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "%s, Supergroup: %s, Name: %s, Type: %s", buf, player->supergroup->name, sgTask->def->logicalname, typeStr);

            // record subtask success
            BitFieldSet(sgTask->subtaskSuccess, SUBTASK_BITFIELD_SIZE, sgTask->sahandle.compoundPos, success);
            
            // Create the temporary contact, then reward
            if (contact)
            {
                memset(&sgRewardContact, 0, sizeof(sgRewardContact));
                sgRewardContact.contact = contact;
                sgRewardContact.dialogSeed = (U32)time(NULL);
                sgRewardContact.interactionTemp = 1;
                sgRewardContact.handle = sgTask->sahandle.context;
                // give rewards (does not apply taskSuccessFlashback or taskSuccessNoFlashback because I don't think you can flashback sg tasks)
                if (success && sgTask->def->taskSuccess)
                    StoryRewardApply(sgTask->def->taskSuccess[0], player, StoryInfoFromPlayer(player), sgTask, &sgRewardContact, NULL, NULL);
                else if (!success && sgTask->def->taskFailure)
                    StoryRewardApply(sgTask->def->taskFailure[0], player, StoryInfoFromPlayer(player), sgTask, &sgRewardContact, NULL, NULL);
                TaskRewardCompletion(player, sgTask, &sgRewardContact, NULL);
            }

            // Close the task and update the supergroup
            TaskClose(player, NULL, player->supergroup->activetask, NULL);

            // Update task for all SG members
            count = player->supergroup->members.count;
            for(i = 0; i < count; i++)
            {
                int dbid = player->supergroup->members.ids[i];
                char buf[32];
                sprintf( buf, "sg_taskcomplete %i", dbid);
                serverParseClient( buf, NULL );
            }
            
        }
        teamUpdateUnlock(player, CONTAINER_SUPERGROUPS);
    }
}

void StoryArcRewardMerits(Entity *e, char *arcName, int success)
{
    char **pRewards = NULL;

    if (success)
        eaPush(&pRewards, "StoryArcMerit");
    else 
        eaPush(&pRewards, "StoryArcMeritFailed");

    rewardVarSet("StoryArcName", arcName);

    // Give out merit to owner of mission
    rewardStoryApplyToDbID(e->db_id, pRewards, VG_NONE, 1, 0, NULL, REWARDSOURCE_STORY);
    eaDestroy(&pRewards);
    rewardVarClear();
}

// Marks the specified task as completed and give the player proper rewards.
// response parameter is optional and used for dialog
// Occurs on the mission at the instant all objectives are finished.
void TaskComplete(Entity* player, StoryInfo* info, StoryTaskInfo* task, ContactResponse* response, int success)
{
    const char *typeStr;
    char buf[1000]; // for log output
    StoryContactInfo* contactInfo = TaskGetContact(info, task);
    int doneWithTask = 1;
    int taskAlreadyComplete = TASK_COMPLETE(task->state);

    badge_RecordTaskComplete(player, 1);

    // log to dbLog
    if(success)
    {
        task->state = TASK_SUCCEEDED;
        strcpy(buf, "Task:Success");
        badge_RecordTaskCompleteSuccess(player, 1);
    }
    else
    {
        task->state = TASK_FAILED;
        strcpy(buf, "Task:Failure");
        badge_RecordTaskCompleteFailed(player, 1);
    }

    typeStr = StaticDefineIntRevLookup(ParseTaskType, task->def->type);
    LOG_ENT(player, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "%s Name: %s, Type: %s", buf, task->def->logicalname, typeStr); 

    // record subtask success
    BitFieldSet(task->subtaskSuccess, SUBTASK_BITFIELD_SIZE, task->sahandle.compoundPos, success);

    // give rewards
    if (success && !taskAlreadyComplete)
    {
        if (task->def->taskSuccess)
        {
            StoryRewardApply(task->def->taskSuccess[0], player, info, task, contactInfo, response, NULL);
        }

        if (TaskForceIsOnFlashback(player))
        {
            if (task->def->taskSuccessFlashback)
            {
                StoryRewardApply(task->def->taskSuccessFlashback[0], player, info, task, contactInfo, response, NULL);
            }
        }
        else
        {
            if (eaSize(&task->def->taskSuccessNoFlashback))
            {
                StoryRewardApply(task->def->taskSuccessNoFlashback[0], player, info, task, contactInfo, response, NULL);
            }
        }
    }
    else if (!success && !taskAlreadyComplete)
    {
        if (task->def->taskFailure)
        {
            StoryRewardApply(task->def->taskFailure[0], player, info, task, contactInfo, response, NULL);
        }
    }


    // Check if this is the last episode of a storyarc
    if (StoryArcCheckLastEpisodeComplete(player, info, contactInfo, task))
    {
        char *arcname = StoryArcGetLeafName(info, contactInfo);

        // Check if the player completed the last mission of a taskforce (or
        // flashback). We need to figure out which challenges they've won if
        // so.
        if (player->taskforce_id && player->taskforce)
        {
            TaskForceCheckCompletionParameters(player, arcname, success);

            // Chalk another completed storyarc in flashback mode.
            if (TaskForceIsOnFlashback(player))
            {
                dbBroadcastMsg(CONTAINER_TASKFORCES, &player->taskforce_id,    INFO_SVR_COM, 0, 1, DBMSG_FLASHBACK_GETCREDIT);
            }
        }
        
        // this sends the 
        if(player->taskforce_id && player->taskforce && player->pl->taskforce_mode == TASKFORCE_ARCHITECT) 
        {
            dbBroadcastMsg(CONTAINER_TASKFORCES, &player->taskforce_id,    INFO_SVR_COM, 0, 1, DBMSG_ARCHITECT_COMPLETE);
        }
        else if(player->taskforce_id && player->taskforce)  
        {
            if (!taskAlreadyComplete)
            {
                char buffer[MAX_PATH];
                sprintf(buffer, "%s%d:%s", DBMSG_TASKFORCE_COMPLETE, success, arcname);
                dbBroadcastMsg(CONTAINER_TASKFORCES, &player->taskforce_id,    INFO_SVR_COM, 0, 1, buffer);
            }
        }
        else // Player doing a storyarc
        {
            if (!taskAlreadyComplete)
            {
                StoryArcRewardMerits(player, arcname, success);
            }
        }

        //cheesy check for praetorian tutoral complete moved to badges_server.c
    }
    else
    {
        // this sends the per-mission reward
        if(player->taskforce_id && player->taskforce && player->pl->taskforce_mode == TASKFORCE_ARCHITECT) 
        {
            dbBroadcastMsg(CONTAINER_TASKFORCES, &player->taskforce_id,    INFO_SVR_COM, 0, 1, DBMSG_ARCHITECT_COMPLETE_MISSION);
        }
    }

    // Perform any cleanup required for different task types.
    switch(task->def->type)
    {
        case TASK_VISITLOCATION:
            // Send the player updates on what locations they are able to visit.
            info->visitLocationChanged = 1;
            break;
    }
    ContactMarkDirty(player, info, contactInfo);
    // Do not perform the team update yet.
    // That will be done after TaskIncrementSubTask() is done to make sure we're updating the team task
    // with the correct status.

    // if the task is compound, set off the next in line
    if (TaskIncrementSubTask(player, info, task))
    {
        doneWithTask = 0;

        // check token count tasks at launch time to see if they have already been completed.
        if (task->def->type == TASK_TOKENCOUNT)
        {
            TokenCountTasksModify(player, task->def->tokenName);
        }

        // check volume tasks at launch time to see if you're already in the volume.
        if (task->def->type == TASK_GOTOVOLUME
            && strstri(db_state.map_name, task->def->volumeMapName))
        {
            VolumeList *volumeList = &player->volumeList;
            int volumeIndex;

            if (volumeList)
            {
                // Look through its volume list and see if it's in a target volume
                for (volumeIndex = 0; volumeIndex < volumeList->volumeCount; volumeIndex++)
                {
                    Volume *entVolume = &volumeList->volumes[volumeIndex];
                    if (entVolume && entVolume->volumePropertiesTracker)
                    {
                        char *entVolumeName = groupDefFindPropertyValue(entVolume->volumePropertiesTracker->def,"NamedVolume");
                        if (!entVolumeName) // For now just ignore any volume without this property
                            continue;

                        if (!stricmp(task->def->volumeName, entVolumeName))
                        {
                            //update clients before TaskComplete in case task is deleted on complete
                            ContactMarkDirty(player, info, TaskGetContact(info, task));

                            TaskComplete(player, info, task, NULL, 1);
                            return; // don't try to do anything further with the previous completed task
                        }
                    }
                }
            }
        }

    }

    TaskMarkSelected(info, task); // should be done after we decide what task state we're in

    if(!doneWithTask)
        return;

    // if we get here, then we are done with the task
    ContactSetNotify(player, info, contactInfo, 1);

    // Update the Newspaper Task history
    if (CONTACT_IS_NEWSPAPER(contactInfo->contact->def) && (success || !TASK_CAN_REPEATONFAILURE(task->def)))
    {
        ScriptVarsTable vars = {0};
        char* searchString;
        const char* uniqueString;

        saBuildScriptVarsTable(&vars, player, player->db_id, contactInfo->handle, task, NULL, NULL, NULL, NULL, 0, 0, false);

        searchString = "ITEM_NAME";
        uniqueString = ScriptVarsTableLookup(&vars, searchString);
        if(!stricmp(uniqueString, searchString))
        {
            searchString = "PERSON_NAME";
            uniqueString = ScriptVarsTableLookup(&vars, searchString);
        }
        // If an item or hostage name was associated with the mission, update the history
        if(stricmp(uniqueString, "ITEM_NAME") && stricmp(uniqueString, "PERSON_NAME"))
            ContactNewspaperTaskHistoryUpdate(info, uniqueString, searchString == "PERSON_NAME");
    }

    // if the task is flagged as not needing to return, make sure to reward and close
    if (TASK_IS_NORETURN(task->def))
    {
        TaskRewardCompletion(player, task, contactInfo, response);
        TaskClose(player, contactInfo, task, response);
        ContactDetermineRelationship(contactInfo);
        ContactSetNotify(player, info, contactInfo, 0);
    }

    // Ask the player what he thought of the mission.
    // But don't ask in the tutorial.
    if(strstriConst(contactInfo->contact->def->filename, "/TUTORIAL/")==NULL)
    {
        START_PACKET(pak, player, SERVER_MISSION_SURVEY)
        END_PACKET
    }

    // Remove tips (except "Special" tips, which are the Alignment Guides)
    if (success && CONTACT_IS_TIP(contactInfo->contact->def) && contactInfo->contact->def->tipType != TIPTYPE_SPECIAL)
    {
        bool locked = false;
        ContactRemove(info, contactInfo->handle);
        // Don't call ContactDismiss, since we don't want to update stats or award TaskAbandon rewards.
        // All important things that ContactDismiss does have already been done.
    }
}

/* Function TaskRewardCompletion()
*    Gives the player rewards for completing the specified task.
*    No rewards will be given if the specified task has not been marked as complete.
*   Returns true if contact dialog has been internally handled by the dialog tree code.
*/
int TaskRewardCompletion(Entity* player, StoryTaskInfo* task, StoryContactInfo* contactInfo, ContactResponse* response)
{
    int returnMe = 0;
    const StoryReward *reward = NULL, *reward2 = NULL;
    const StoryTask *def;

    // If the task has not been complete, do nothing.
    if (!TaskCompletedAndIncremented(task))
        return 0;

    // Look up the appropriate reward
    def = task->def;
    if (task->compoundParent) 
        def = task->compoundParent;
    if (task->state == TASK_SUCCEEDED)
    {
        if (eaSize(&def->returnSuccess)) 
            reward = def->returnSuccess[0];

        if (TaskForceIsOnFlashback(player))
        {
            if (eaSize(&def->returnSuccessFlashback)) 
                reward2 = def->returnSuccessFlashback[0];
        }
        else
        {
            if (eaSize(&def->returnSuccessNoFlashback)) 
                reward2 = def->returnSuccessNoFlashback[0];
        }
    }
    else
    {
        if (eaSize(&def->returnFailure)) 
            reward = def->returnFailure[0];
    }

    // give reward to player
    if (reward)
    {
        returnMe |= StoryRewardApply(reward, player, StoryInfoFromPlayer(player), task, contactInfo, response, NULL);
    }

    if (reward2)
    {
        returnMe |= StoryRewardApply(reward2, player, StoryInfoFromPlayer(player), task, contactInfo, response, NULL);
    }

    return returnMe;
}

void TaskMarkComplete(Entity* player, StoryTaskHandle *sahandle, int success)
{
    StoryInfo* info;
    StoryTaskInfo* task;
    TaskState newState = success ? TASK_MARKED_SUCCESS : TASK_MARKED_FAILURE;

    info = StoryArcLockInfo(player);
    task = TaskGetInfo(player, sahandle);
    if (task && (sahandle->compoundPos == -1 || task->sahandle.compoundPos == sahandle->compoundPos))
    {
        int childCount;
        if (!TASK_COMPLETE(task->state))
        {
            task->state = newState;
        }

        StoryArcUnlockInfo(player);

        // if last task in compound task, immediately set complete
        if (task->compoundParent && task->compoundParent->children &&
            (childCount = eaSize(&task->compoundParent->children)) &&
            sahandle->compoundPos == childCount - 1)
        {
            TaskSetComplete(player, sahandle, 0, success);
        }
    }
    else
    {
        StoryArcUnlockInfo(player);
    }

    if (!task && player->taskforce_id) // otherwise, try getting the task from the other supergroupmode
    {
        int tmp = player->pl->taskforce_mode;
        player->pl->taskforce_mode = 0;
        info = StoryArcLockInfo(player);
        task = TaskGetInfo(player, sahandle);
        if (task && (sahandle->compoundPos == -1 || task->sahandle.compoundPos == sahandle->compoundPos))
            task->state = newState;
        StoryArcUnlockInfo(player);
        player->pl->taskforce_mode = tmp;
    }
}

void TaskSetComplete(Entity* player, StoryTaskHandle *sahandle, int any_position, int success)
{
    StoryInfo* info;
    StoryTaskInfo* task;
    bool iLockedIt;

    if (StoryArcInfoIsLocked(player))
    {
        info = StoryInfoFromPlayer(player);
        iLockedIt = false;
    }
    else
    {
        info = StoryArcLockInfo(player);
        iLockedIt = true;
    }
    task = TaskGetInfo(player, sahandle);

    if (task && (any_position|| task->sahandle.compoundPos == sahandle->compoundPos))
    {
        if (TASK_IS_SGMISSION(task->def))
            TaskCompleteSG(player, success);
        else
            TaskComplete(player, info, task, NULL, success);
    }
    if (iLockedIt)
    {
        StoryArcUnlockInfo(player);
        iLockedIt = false;
    }
    if (!task && player->taskforce_id) // otherwise, try getting the task from the other taskforce mode
    {
        int tmp = player->pl->taskforce_mode;
        player->pl->taskforce_mode = 0;
        if (StoryArcInfoIsLocked(player))
        {
            info = StoryInfoFromPlayer(player);
            iLockedIt = false;
        }
        else
        {
            info = StoryArcLockInfo(player);
            iLockedIt = true;
        }
        task = TaskGetInfo(player, sahandle);
        if (task && (any_position || task->sahandle.compoundPos == sahandle->compoundPos))
            TaskComplete(player, info, task, NULL, success);
        if (iLockedIt)
        {
            StoryArcUnlockInfo(player);
            iLockedIt = false;
        }
        player->pl->taskforce_mode = tmp;
    }
}

// Look through all of the player's tasks and attempt to advance any completed ones.
void TaskAdvanceCompleted(Entity* player)
{
    int i;
    StoryInfo* info = StoryInfoFromPlayer(player);
    StoryTaskInfo* task;

    for(i = eaSize(&info->tasks)-1; i >= 0; i--)
    {
        task = info->tasks[i];
        if (TASK_MARKED(task->state))
        {
            int success = (task->state == TASK_MARKED_SUCCESS) ? 1 : 0;
            TaskSetComplete(player, &task->sahandle, 0, success);
        }
    }
}

// set the objective as received, make sure we do whatever clue operations this would imply
void TaskReceiveObjectiveCompleteHelper(Entity *player, StoryInfo* info, StoryTaskInfo* task, int objective, int success)
{
    const MissionDef* mission;
    const StoryClue** clues = TaskGetClues(&task->sahandle);
    U32* clueStates = TaskGetClueStates(info, task);
    int* needintro = TaskGetNeedIntroFlag(info, task);
    StoryReward* reward;
    int i, taskIndex;

    // set the objective info first
    ObjectiveInfoAdd(task->completeObjectives, &task->sahandle, objective, success);

    // see if we could have triggered any clue operations
    mission = TaskMissionDefinition(&task->sahandle);
    if (!mission)
    {
        log_printf("storyerrs.log", "TaskReceiveObjectiveComplete: got task %i,%i,%i,%i, but it's not a mission", SAHANDLE_ARGSPOS(task->sahandle) );
        return;
    }
    if (objective < 0 || objective >= eaSize(&mission->objectives))
    {
        log_printf("storyerrs.log", "TaskReceiveObjectiveComplete: got objective %i for task %i,%i,%i,%i.  There are only %i objectives for this mission",
            objective, SAHANDLE_ARGSPOS(task->sahandle), eaSize(&mission->objectives));
        return;
    }
    if (!eaSize(&mission->objectives[objective]->reward)) return;

    reward = mission->objectives[objective]->reward[0];

    // give and remove clues
    if (clues && clueStates)
    {
        for (i = eaSize(&reward->addclues)-1; i >= 0; i--)
            ClueAddRemove(player, info, reward->addclues[i], clues, clueStates, task, needintro, 1);
        for (i = eaSize(&reward->removeclues)-1; i >= 0; i--)
            ClueAddRemove(player, info, reward->removeclues[i], clues, clueStates, task, needintro, 0);
    }

    // add and remove tokens
    StoryRewardHandleTokens(player, reward->addTokens, reward->addTokensToAll, reward->removeTokens, reward->removeTokensFromAll);

    if (info)
    {
        for (i = eaSize(&reward->abandonContacts) - 1; i >= 0; i--)
        {
            for (taskIndex = eaSize(&info->tasks) - 1; taskIndex >= 0; taskIndex--)
            {
                StoryContactInfo *contactInfo = TaskGetContact(info, info->tasks[taskIndex]);
                if (contactInfo && contactInfo->contact)
                {
                    char *contactName = strrchr(contactInfo->contact->def->filename, '/');
                    if (contactName)
                    {
                        contactName++;
                        if (!strnicmp(contactName, reward->abandonContacts[i], strlen(reward->abandonContacts[i])))
                        {
                            TaskAbandon(player, info, contactInfo, info->tasks[taskIndex], 0);
                        }
                    }
                }
            }
        }
    }

    StoryRewardHandleSounds(player, reward);
}

// we received word from a mission map that an objective was completed
void TaskReceiveObjectiveComplete(Entity* player, StoryTaskHandle *sahandle, int objective, int success)
{
    StoryInfo* info;
    StoryTaskInfo* task;

    info = StoryArcLockInfo(player);
    task = TaskGetInfo(player, sahandle);    
    if (task && (sahandle->compoundPos == -1 || task->sahandle.compoundPos == sahandle->compoundPos))
        TaskReceiveObjectiveCompleteHelper(player, info, task, objective, success);
    StoryArcUnlockInfo(player);
    if (!task && player->supergroup_id) // otherwise, try getting the task from the other supergroupmode
    {
        int tmp = player->pl->taskforce_mode;
        player->pl->taskforce_mode = 0;
        info = StoryArcLockInfo(player);
        task = TaskGetInfo(player, sahandle);
        if (task && (sahandle->compoundPos == -1 || task->sahandle.compoundPos == sahandle->compoundPos))
            TaskReceiveObjectiveCompleteHelper(player, info, task, objective, success);
        StoryArcUnlockInfo(player);
        player->pl->taskforce_mode = tmp;
    }
}

// *********************************************************************************
//  Mission-task interaction
// *********************************************************************************

static int TrySetMissionMapId(Entity* owner, StoryTaskHandle *sahandle, int map_id)
{
    StoryInfo* info;
    StoryTaskInfo* task = TaskGetInfo(owner, sahandle);
    int sgLock = 0;

if (!task)
        return 0;
    info = StoryArcLockInfo(owner);
    if (TaskIsSGMissionEx(owner, sahandle) && teamLock(owner, CONTAINER_SUPERGROUPS))
        sgLock = 1;
    task = TaskGetInfo(owner, sahandle);
    if (task && task->sahandle.compoundPos == sahandle->compoundPos)
    {
        int prev_id = task->missionMapId;
        task->missionMapId = map_id;
        log_printf("missiondoor.log", "%s's mission set to %i, from %i", owner->name, map_id, prev_id);
        info->contactInfoChanged = 1; // owner needs to notice map_id change
    }
    if (sgLock)
        teamUpdateUnlock(owner, CONTAINER_SUPERGROUPS);
    StoryArcUnlockInfo(owner);
    return 1;
}

// update this player's mission with the newly created map id
void TaskSetMissionMapId(Entity* owner, StoryTaskHandle *sahandle, int map_id)
{
    // try first in player's current mode
    if (!TrySetMissionMapId(owner, sahandle, map_id))
    {
        // try in other mode
        int tmp = owner->pl->taskforce_mode;
        owner->pl->taskforce_mode = 0;
        TrySetMissionMapId(owner, sahandle, map_id);
        owner->pl->taskforce_mode = tmp;
    }
}

// return 1 if mission is allowed to shut down
// - it is only not allowed to shut down if I'm in the process of attaching a team to it
int TaskMissionRequestShutdown(Entity* owner, StoryTaskHandle *sahandle, int map_id)
{
    if (owner->teamup_id &&    owner->teamup->activetask->assignedDbId == owner->db_id &&
        isSameStoryTaskPos( &owner->teamup->activetask->sahandle, sahandle ) )
    {
        log_printf("missiondoor.log", "%s refusing mission %i shutdown request", owner->name, map_id);
        return 0;    // I've attached my teamup again in preparation for transferring
    }

    // otherwise, I'm fine with mission shutting down.  Make sure to mark my task as having no map id
    TaskSetMissionMapId(owner, sahandle, 0);
    return 1;
}

void TaskMissionForceShutdown(Entity* owner, StoryTaskHandle *sahandle)
{
    TaskSetMissionMapId(owner, sahandle, 0);
}

// *********************************************************************************
//  Task Matching - search method for tasks
// *********************************************************************************

// returns number of matched tasks, and places pointers in resultlist
// looks at all tasks in tasklist, and ignores any with same bit in bitfield set
// tasklist must be an EArray
// any flags set in flag must be matched with task
// only selects task types that can be issued by a contact
// any unique tasks will not be selected if there is a bit present in uniqueBitfield
// Checks NewspaperType and PvPType if they are not null.  If they are, this is ignored
int TaskMatching(Entity *pPlayer, const StoryTask** tasklist, U32* bitfield, U32* uniqueBitfield, U32 flag, int player_level, int* minLevel, 
                 StoryInfo* info, const StoryTask* resultlist[], NewspaperType* newsType, PvPType* pvpType, int meetsReqsForItemOfPowerMission,
                 const char **requiresFailedString)
{
    ContactHandle target;
    int result = 0;
    int i;
    int n = eaSize(&tasklist);
    int mask;
    int failed;
    int tempflag;

    // initialize requires failed string
    if (requiresFailedString != NULL) 
        *requiresFailedString = NULL;

    if( TaskForceIsOnFlashback(pPlayer) )
        player_level = character_CalcCombatLevel(pPlayer->pchar);

    // just a single pass checking constraints
    for (i = 0; i < n; i++)
    {
        const StoryTask* starttask = tasklist[i];
        const StoryTask* parenttask = starttask;
        int playerAlliance = ENT_IS_VILLAIN(pPlayer)?SA_ALLIANCE_VILLAIN:SA_ALLIANCE_HERO;

        if (tasklist[i]->type == TASK_COMPOUND)
            starttask = starttask->children[0];

        // don't issue if task is deprecated
        if (tasklist[i]->deprecated) 
            continue;
        
        // can't assign flashback only missions - these are assigned only as converted story arcs
        if (TASK_IS_FLASHBACK_ONLY(parenttask))
            continue;

        // check type
        if (starttask->type == TASK_RANDOMENCOUNTER) 
            continue;
        if (starttask->type == TASK_RANDOMNPC) 
            continue;

        // check alliance (ignored for taskforce)
        if (!(pPlayer && pPlayer->taskforce_id) && starttask->alliance != SA_ALLIANCE_BOTH && playerAlliance != starttask->alliance)
            continue;

        // intro tasks require we don't have contact already
        if (starttask->type == TASK_CONTACTINTRO)
        {
            const PNPCDef* pnpc = PNPCFindDef(starttask->deliveryTargetNames[0]);
            if (!pnpc || !pnpc->contact) 
                continue;
            target = ContactGetHandle(pnpc->contact);
            if (ContactGetInfo(info, target)) 
                continue;
        }

        // check unique tasks
        if (TASK_IS_UNIQUE(tasklist[i]))
        {
            if (uniqueBitfield && BitFieldGet(uniqueBitfield, UNIQUE_TASK_BITFIELD_SIZE, UniqueTaskGetIndex(tasklist[i]->logicalname)))
                continue;
        }

        // check flags
        tempflag = flag;
        mask = 1;
        failed = 0;
        while (tempflag && !failed)
        {
            // if this flag is set in parameter
            if (tempflag & 1)
            {
                // if it isn't in task
                if (!(mask & tasklist[i]->flags))
                {
                    failed = 1;
                    break;
                }
            }
            tempflag >>= 1;
            mask <<= 1;
        }
        if (failed)
            continue;

        // check bitfield
        if (bitfield && BitFieldGet(bitfield, (n + 7)/8, i)) 
            continue;

        // check player level
        if (player_level)
        {
            if (tasklist[i]->minPlayerLevel > player_level)
            {
                // return the min level for a new task - ONLY non-unique ones
                if (!TASK_IS_UNIQUE(tasklist[i]) && minLevel)
                {
                    if (*minLevel == 0)
                        *minLevel = tasklist[i]->minPlayerLevel;
                    else if (*minLevel > tasklist[i]->minPlayerLevel)
                        *minLevel = tasklist[i]->minPlayerLevel;
                }
                continue;
            }
            if (tasklist[i]->maxPlayerLevel < player_level)
            {
                continue;
            }
        }

        // check requires
        if (tasklist[i]->assignRequires && pPlayer && pPlayer->pchar)
        {
            if (!chareval_Eval(pPlayer->pchar, tasklist[i]->assignRequires, tasklist[i]->filename))
            {
                if (result == 0 && requiresFailedString != NULL && *requiresFailedString == NULL)
                {
                    *requiresFailedString = tasklist[i]->assignRequiresFailed;
                }
                continue;
            }
        }

        if( tasklist[i]->filename && strstriConst( tasklist[i]->filename, "CathedralOfPain" ) ) //TEMP hack TO DO replace with flag in task
        {
//            if( !IsCathedralOfPainOpen() || !meetsReqsForItemOfPowerMission || RaidIsRunning() || playerIsNowInAnIllegalBase() )
            if( RaidIsRunning() || playerIsNowInAnIllegalBase() ) 
                continue;
        }

        if (pvpType && (*pvpType != tasklist[i]->pvpType))
            continue;

        if (newsType && (*newsType != tasklist[i]->newspaperType))
            continue;

        // otherwise, we made it
        resultlist[result++] = tasklist[i];

        // clear any failed string
        if (requiresFailedString != NULL)
        {
            *requiresFailedString = NULL;
        }
    }
    return result;
}

// *********************************************************************************
//  Encounter-task interaction
// *********************************************************************************

// attempt to replace a random encounter with one from an active task
int TaskGetEncounter(EncounterGroup* group, Entity* player)
{
    StoryInfo* info;
    StoryTaskInfo* task;
    int i, n;

    // after we do selection we have these vars set
    StoryTaskHandle sahandle = {0};
    VillainGroupEnum villainGroup = 0;
    const SpawnDef* spawndef = NULL;
    int point = -1;                    // the spawn point to use within group

    ///////////////////////////////////////////////// BEFORE LOCKING STORYINFO

    // init
    if (!player) 
        return 0;
    info = StoryInfoFromPlayer(player);

    // decide if I have any tasks that want to replace the encounter
    n = eaSize(&info->tasks);
    for (i = 0; i < n; i++)
    {
        task = info->tasks[i];
        if (task->spawnGiven) 
            continue;
        if (!TASK_INPROGRESS(task->state)) 
            continue;
        spawndef = TaskSpawnDef(task->def);
        if (!spawndef) 
            continue;
        {
            const MapSpec* spec = MapSpecFromMapId(db_state.base_map_id);
            if (!spec)
                spec = MapSpecFromMapName(db_state.map_name);

            if (!MapSpecCanOverrideSpawn(spec, task->level)) 
            {
                spawndef = NULL;
                continue;
            }
        }

        // find a matching encounter point
        if (spawndef->spawntype)
        {
            ScriptVarsTable vars = {0};
            const char* spawntype;

            saBuildScriptVarsTable(&vars, player, player->db_id, TaskGetContactHandle(info, task), task, NULL, NULL, NULL, NULL, 0, 0, false);
            ScriptVarsTablePushScope(&vars, &spawndef->vs);
            spawntype = ScriptVarsTableLookup(&vars, spawndef->spawntype);

            point = EncounterGetMatchingPoint(group, spawntype);
            if (point == -1) // couldn't match our spawn type
            {
                spawndef = NULL;
                continue;
            }
            // otherwise, we have a valid point now
        }

        // we have a spawndef now
        StoryHandleCopy( &sahandle, &task->sahandle);
        break;
    }
    if (!spawndef)
        return 0;    // found nothing
    if (!TeamCanHaveAmbush(player))
        return 0;

    //////////////////////////////////////////////// AFTER LOCKING STORYINFO

    info = StoryArcLockInfo(player);
    task = TaskGetInfo(player, &sahandle);

    // go ahead and replace the encounter
    task->spawnGiven = 1;
    group->active->spawndef = spawndef;
    if (point != -1)
        group->active->point = point;

    // calls ScriptVarsTableClear in EncounterActiveInfoFree
    saBuildScriptVarsTable(&group->active->vartable, player, player->db_id, TaskGetContactHandle(info, task), task, NULL, NULL, NULL, NULL, 0, 0, true);
    ScriptVarsTablePushScope(&group->active->vartable, &spawndef->vs);
    group->active->seed = task->seed;
    group->active->villainbaselevel = character_GetCombatLevelExtern(player->pchar);
    group->active->villainlevel = MAX(0,group->active->villainbaselevel + task->difficulty.levelAdjust);
    group->active->player = erGetRef(player);
    group->active->taskcontext = sahandle.context;
    group->active->tasksubhandle = sahandle.subhandle;
    group->active->taskvillaingroup = task->villainGroup;
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

// *********************************************************************************
//  Task debug tools
// *********************************************************************************

const char *TaskDebugLogCommand(Entity *e, StoryTaskHandle *sahandle, char const *cmd)
{
    static char buf[256];

    sprintf(buf, "TaskLog:%s:%s:%d:%d", cmd, e->name, sahandle->context, sahandle->subhandle );

    return buf;
}

const char* TaskDebugName(StoryTaskHandle *sahandle, unsigned int seed)
{
    const StoryTask* taskdef = TaskParentDefinition(sahandle);
    if (taskdef && taskdef->logicalname)
    {
        return taskdef->logicalname;
    }
    else if (taskdef && taskdef->inprogress_description)
    {
        ScriptVarsTable vars = {0};
        saBuildScriptVarsTable(&vars, NULL, 0, 0, NULL, NULL, sahandle, NULL, NULL, 0, seed, false);
        return saUtilTableLocalize(taskdef->inprogress_description, &vars);
    }
    else return NULL;
}

// returns pointer to internal buffer
char* TaskDisplayType(const StoryTask* def)
{
    static char result[200];
    char* end = result;
    int flags, curflag;

    // print basic type
    end += sprintf(end, "%s", StaticDefineIntRevLookup(ParseTaskType, def->type));

    // iterate through flags
    for (flags = def->flags, curflag = 1; flags; flags >>= 1, curflag <<= 1)
    {
        if (flags % 2)
        {
            end += sprintf(end, " %s", StaticDefineIntRevLookup(ParseTaskFlag, curflag));
        }
    }
    return result;
}

char* TaskDisplayState(TaskState state)
{
    if (state >= TASK_NONE && state <= TASK_FAILED)
        return g_taskStateNames[state];
    return localizedPrintf(0,"InvalidState");
}

void TaskPrintCluesHelper(char ** estr, StoryTaskInfo* task)
{
    int i, n, found;

    if (!task->def->clues) return;

    estrConcatf(estr, "\n%s\n", saUtilLocalize(0,"ClueDebug"));
    found = 0;
    n = eaSize(&task->def->clues);
    for (i = 0; i < n; i++)
    {
        if (BitFieldGet(task->haveClues, CLUE_BITFIELD_SIZE, i))
        {
            estrConcatf(estr, "\t%s\n", saUtilScriptLocalize(task->def->clues[i]->displayname));
            found = 1;
        }
    }
    if (!found)
    {
        estrConcatf(estr, "\t%s\n", saUtilLocalize(0,"NoneParen"));
    }
}

void TaskPrintClues(ClientLink* client, StoryTaskInfo* task)
{
    char * estr = NULL;
    estrClear(&estr);
    TaskPrintCluesHelper(&estr, task);
    conPrintf(client, "%s", estr);
    estrDestroy(&estr);
}

// messy way to clear the progress of all active tasks
void TaskDebugClearAllTasks(Entity* player)
{

    StoryInfo* info;
    int i, n;

    info = StoryArcLockInfo(player);

    // clear active tasks
    eaSetSize(&info->tasks, 0);

    // clear task allocation info from contacts
    n = eaSize(&info->contactInfos);
    for (i = 0; i < n; i++)
    {
        BitFieldClear(info->contactInfos[i]->taskIssued, TASK_BITFIELD_SIZE);
        info->contactInfos[i]->dirty = 1;
    }
    info->contactInfoChanged = 1;
    StoryArcUnlockInfo(player);
    TeamTaskClear(player);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// This function is now used by the auto-complete mission system and is no longer simply a debug command
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void TaskDebugCompleteByTask(Entity* player, StoryTaskInfo* taskinfo, int succeeded)
{
    int onCurrentMap;
    StoryInfo *pSI = StoryInfoFromPlayer(player);
    StoryContactInfo *pContactInfo = TaskGetContact(pSI, taskinfo);

    // log information
    LOG_ENT(player, LOG_ADMIN, LOG_LEVEL_IMPORTANT, 0, "Task:ForceComplete Auth: %s Task: %s", player->auth_name, taskinfo->def->filename);
    if (player->pchar != NULL)
        LOG_ENT(player, LOG_ADMIN, LOG_LEVEL_IMPORTANT, 0, "Task:ForceComplete Character Level: %d", player->pchar->iLevel + 1);
    LOG_ENT(player, LOG_ADMIN, LOG_LEVEL_IMPORTANT, 0,"Task:ForceComplete Context: %d Subhandle: %d", taskinfo->sahandle.context, taskinfo->sahandle.subhandle);
    if (pContactInfo != NULL)
        LOG_ENT(player, LOG_ADMIN, LOG_LEVEL_IMPORTANT, 0,"Task:ForceComplete Contact Name: %s", ContactDisplayName(pContactInfo->contact->def,0));
    LOG_ENT(player, LOG_ADMIN, LOG_LEVEL_IMPORTANT, 0,"Task:ForceComplete Task Name: %s, File: %s", taskinfo->def->logicalname, taskinfo->def->filename);
    LOG_ENT(player, LOG_ADMIN, LOG_LEVEL_IMPORTANT, 0,"Task:ForceComplete Task Level: %d", taskinfo->level);
    if (taskinfo->def->type == TASK_MISSION && player->teamup != NULL)
    {
        const char *mapname = MissionGetMapName(player, taskinfo, TaskGetContactHandle(pSI, taskinfo));
        LOG_ENT(player, LOG_ADMIN, LOG_LEVEL_IMPORTANT, 0,"Task:ForceComplete MapId: %i, Door %i", taskinfo->missionMapId, taskinfo->doorId);
        LOG_ENT(player, LOG_ADMIN, LOG_LEVEL_IMPORTANT, 0,"Task:ForceComplete MapName: %s", mapname ? mapname : "-unknown-");
    }

    onCurrentMap = (taskinfo->missionMapId == db_state.map_id);
    if (taskinfo->missionMapId) // if attached to mission, take care of it too
    {
        MissionSetComplete(1);
        teamDetachMap(player);
    }
        
    // Prevent double rewards when command is executed on a mission map
    if (!onCurrentMap || db_state.local_server)
    {
        TaskSetComplete(player, &taskinfo->sahandle, 1, succeeded);
    }

    if (TASK_COMPLETE(taskinfo->state) && player->teamup && 
        player->teamup->activetask->assignedDbId == taskinfo->assignedDbId &&
        isSameStoryTaskPos( &player->teamup->activetask->sahandle, &taskinfo->sahandle) )
    {
        TeamTaskClear(player);
    }

    TaskStatusSendUpdate(player);
}

void TaskDebugComplete(Entity* player, int index, int succeeded)
{
    StoryInfo* info = StoryInfoFromPlayer(player);
    if (index >= -1 && index < eaSize(&info->tasks))
    {
        StoryTaskInfo* taskinfo;
        if (index != -1)
            taskinfo = info->tasks[index];
        else if (player->supergroup && player->supergroup->activetask)
            taskinfo = player->supergroup->activetask;
        else
            return;

        TaskDebugCompleteByTask(player, taskinfo, succeeded);
    }
}


// calls TaskForceGet(), but first finds a suitable contact to assign the task (Used for QA purposes)
bool TaskForceGetUnknownContact(ClientLink * client, const char * logicalname)
{
    int i,k,n,p;
    const ContactDef **pContacts = g_contactlist.contactdefs;

    // loop through all contacts until one is found containing this task
    n = eaSize(&pContacts);
    for (i = 0; i < n; i++)
    {
        p = eaSize(&pContacts[i]->tasks);
        for(k = 0; k < p; k++)
        {
            if(      pContacts[i]->tasks[k]->logicalname
                && ! stricmp(pContacts[i]->tasks[k]->logicalname, logicalname))
            {
                // match...
                conPrintf(client, "%s%s\n%s %s", saUtilLocalize(0,"TaskColon"), logicalname, saUtilLocalize(0,"UsingContact"), pContacts[i]->filename);
                TaskForceGet(client, pContacts[i]->filename, logicalname);
                return true;
            }
        }
    }

    conPrintf(client, saUtilLocalize(0,"BadTask", logicalname));
    return false;
}

void TaskForceGet(ClientLink* client, const char* filename, const char* logicalname)
{
    StoryInfo* info;
    StoryContactInfo* contactInfo;
    StoryTaskHandle sahandle = {0};
    int isSGMission;

    if (PlayerInTaskForceMode(client->entity))
    {
        conPrintf(client, saUtilLocalize(client->entity, "CantGetTaskWhileInTaskforceMode"));
        return;
    }

    // clear any other task first
    teamDetachMap(client->entity);
    TaskDebugClearAllTasks(client->entity);

    info = StoryArcLockInfo(client->entity);

    // get the contact first
    sahandle.context = ContactGetHandleLoose(filename);
    contactInfo = ContactGetInfo(info, sahandle.context);
    if (!contactInfo)
    {
        // try adding it
        StoryArcUnlockInfo(client->entity);
        ContactDebugAdd(client, client->entity, filename);
        info = StoryArcLockInfo(client->entity);
        contactInfo = ContactGetInfo(info, sahandle.context);
        if (!contactInfo)
        {
            StoryArcUnlockInfo(client->entity);
            return; // should have already printed error message
        }
    }

    // find the task
    sahandle.subhandle = TaskFromLogicalName(contactInfo->contact->def, logicalname);
    if (!sahandle.subhandle)
    {
        conPrintf(client, saUtilLocalize(client->entity,"CantFindTask", logicalname));
        StoryArcUnlockInfo(client->entity);
        return;
    }

    // added check to give warning message if player level is too high/low
    {
        const StoryTask* task = TaskParentDefinition(&sahandle);
        if(task)
        {
            int level = character_GetExperienceLevelExtern(client->entity->pchar);
            if(    level < task->minPlayerLevel
                || level > task->maxPlayerLevel)
            {
                conPrintf(client, saUtilLocalize(client->entity,"TaskLevelWarning", task->minPlayerLevel, task->maxPlayerLevel, level));
            }
        }
        isSGMission = TASK_IS_SGMISSION(task);
    }


    // assign the task
    if (isSGMission)
    {
        // If its a supergroup mission, remove the contact and add the task for the supergroup
        if (TaskAddToSupergroup(client->entity, &sahandle, (unsigned int)time(NULL), 0, 0))
            conPrintf(client, saUtilLocalize(client->entity,"TaskAdd"));
        ContactRemove(info, sahandle.context); // don't call ContactDrop, this isn't an actual drop
    }
    else if (TaskAdd(client->entity, &sahandle, (unsigned int)time(NULL), 0, 0))
        conPrintf(client, saUtilLocalize(client->entity,"TaskAdd"));
    else
        conPrintf(client, saUtilLocalize(client->entity,"UnableAssignTask"));
    StoryArcUnlockInfo(client->entity);
}

// *********************************************************************************
//  Objective infos - let a task keep track of when objectives were completed
// *********************************************************************************

// The only reason we save objective infos currently is so that we can display the detail string
// If we use them for other things later, update this function accordingly
static int ObjectiveInfoIsSaved(StoryTaskHandle *sahandle, int objectiveNum)
{
    const MissionDef* mission; 
    const MissionObjective* objective;

    mission = TaskMissionDefinition(sahandle);
    if (!mission)
        return 0;
    objective = MissionObjectiveFromIndex(mission, objectiveNum);
    return objective && objective->reward && objective->reward[0]->rewardDialog;
}

// add information about a particular mission objective being completed
void ObjectiveInfoAdd(StoryObjectiveInfo* objectives, StoryTaskHandle * sahandle, int objectiveNum, int success)
{
    int i;

    // Only save the objective infos that we care about
    if (!ObjectiveInfoIsSaved(sahandle, objectiveNum))
        return;

    // make sure we don't already have this objective listed
    for (i = 0; i < MAX_OBJECTIVE_INFOS; i++)
    {
        if (!objectives[i].set) 
            break;
        if (objectives[i].compoundPos == sahandle->compoundPos && objectives[i].num == objectiveNum) 
            return; // already have this information
    }

    // now add it to the end of the list
    if (i >= MAX_OBJECTIVE_INFOS)
    {
        Errorf("task.c - too many objectives to record!");
    }
    else
    {
        objectives[i].set = 1;
        objectives[i].compoundPos = sahandle->compoundPos;
        objectives[i].missionobjective = 1;
        objectives[i].num = objectiveNum;
        objectives[i].success = success;
    }
}

// *********************************************************************************
//  Unique tasks - tasks that can't be repeated, even across stature sets
// *********************************************************************************

StringTable g_uniquenames = 0;
StashTable g_uniquehashes = 0;
char* g_uniquetable[MAX_UNIQUE_TASKS];
int g_numuniques = 0;
#define UNIQUETASK_ATTR_PREFIX "xUNIQUETASK1_"

// load the attributes table and add any unique tasks found there
void UniqueTaskPreload()
{
    char        *s,*mem,*args[10];
    int            count;
    char*        fileName = "server/db/templates/vars.attribute";

    mem = fileAlloc(fileName, 0);

    if(!mem){
        printf("ERROR: Can't open %s\n", fileName);
        return;
    }

    s = mem;
    while((count=tokenize_line(s,args,&s)))
    {
        if (count != 2)    continue;
        UniqueTaskAdd(UniqueTaskFromAttribute(args[1])); // won't load anything without the ATTR_PREFIX
    }
    free(mem);
}

int UniqueTaskGetCount()
{
    return g_numuniques;
}

char* UniqueTaskByIndex(int i)
{
    if (i < 0 || i >= g_numuniques)
        return NULL;
    return g_uniquetable[i];
}

int UniqueTaskGetIndex(const char* logicalname)
{
    int index;
    if (!stashFindInt(g_uniquehashes, logicalname, &index))
        return -1;
    return index;
}

void UniqueTaskAdd(const char* logicalname)
{
    int index;

    // don't overflow
    if (!logicalname) return;
    if (g_numuniques == MAX_UNIQUE_TASKS)
    {
        ErrorFilenamef("task.c", "task.c - run out of unique task slots - increase MAX_UNIQUE_TASKS or yell at designers");
        g_numuniques = -1;
    }
    if (g_numuniques == -1)
        return;

    // init tables
    if (!g_uniquenames)
    {
        g_uniquenames = createStringTable();
        initStringTable(g_uniquenames, 1000);
    }
    if (!g_uniquehashes)
    {
        g_uniquehashes = stashTableCreateWithStringKeys(MAX_UNIQUE_TASKS * 2, StashDeepCopyKeys);
    }

    // add string
    if (stashFindInt(g_uniquehashes, logicalname, &index))
        return;
    index = g_numuniques++;
    g_uniquetable[index] = strTableAddString(g_uniquenames, logicalname);
    stashAddInt(g_uniquehashes, logicalname, index,false);
}

char* UniqueTaskGetAttribute(const char* logicalname)
{
    static char buf[1000];
    sprintf(buf, UNIQUETASK_ATTR_PREFIX "%s", logicalname);
    return buf;
}

const char* UniqueTaskFromAttribute(const char* attribute)
{
    int len = (int)strlen(UNIQUETASK_ATTR_PREFIX);
    if (strncmp(UNIQUETASK_ATTR_PREFIX, attribute, len))
        return NULL;
    return attribute + len;
}

void UniqueTaskDebugPage(ClientLink* client, Entity* player)
{
    int i, n;
    StoryInfo* info;

    info = StoryInfoFromPlayer(player);
    n = UniqueTaskGetCount();
    conPrintf(client, saUtilLocalize(player,"UniqueTask:"));
    for (i = 0; i < n; i++)
    {
        int done = BitFieldGet(info->uniqueTaskIssued, UNIQUE_TASK_BITFIELD_SIZE, i);
        conPrintf(client, "\t[%c] %s", done? 'x': ' ', UniqueTaskByIndex(i));
    }
}

// *********************************************************************************
//  Task timeouts
// *********************************************************************************

// check all the tasks in this story info for a timeout
void CheckTaskTimeouts(Entity* player, StoryInfo* info, int db_id)
{
    int i, n;
    n = eaSize(&info->tasks);
    for (i = 0; i < n; i++)
    {
        if (!TASK_INPROGRESS(info->tasks[i]->state))
            continue; // don't update tasks that are complete
        if (!info->tasks[i]->timeout || !info->tasks[i]->failOnTimeout)
            continue; // don't worry about tasks with no timeout
        if (db_id && info->tasks[i]->assignedDbId != db_id)
            continue; // if we are looking at a taskforce, only look at tasks assigned to me

        // task has timed out
        if (info->tasks[i]->timeout < dbSecondsSince2000())
        {
            // we have entity, so we can mark task as failed directly
            TaskSetComplete(player, &info->tasks[i]->sahandle, 1, 0);
            break; // just in case data changes as a result of above, ignore other tasks until next tick
        }
    }
    if (player->supergroup && player->supergroup->activetask && 
        player->supergroup->activetask->failOnTimeout &&
        (player->supergroup->activetask->timerType == -1 ?
            player->supergroup->activetask->timeout < dbSecondsSince2000() :
            player->supergroup->activetask->timeout > dbSecondsSince2000()))
    {
        TaskSetComplete(player, &player->supergroup->activetask->sahandle, 1, 0);
    }
}

// hook into global loop - check task timeouts
void StoryArcCheckTimeouts()
{
    static int tick = 0;
    int i;

    // only process every 30th player each tick
    //   - fast enough for each task to be checked once per second
    tick = (tick + 1) % 30;
    for(i = tick; i < entities_max; i += 30)
    {
        if (i == 0) continue;
        if (entity_state[i] & ENTITY_IN_USE &&
            ENTTYPE_BY_ID(i) == ENTTYPE_PLAYER)
        {
            // check each individual task this player has
            StoryInfo* info = entities[i]->storyInfo;
            CheckTaskTimeouts(entities[i], info, 0);

            // check any taskforce tasks this player has assigned to them
            if (entities[i]->taskforce_id)
            {
                info = entities[i]->taskforce->storyInfo;
                CheckTaskTimeouts(entities[i], info, entities[i]->db_id);

                // Deal with taskforce parameter timeouts. Also see if we
                // have any taskforce-related messages to send to the rest
                // of the team.
                if (entities[i]->taskforce)
                {
                    TaskForceCheckTimeout(entities[i]);
                    TaskForceCheckBroadcasts(entities[i]);
                }
            }

        }
    }
}

// *********************************************************************************
//  Kill Tasks
// *********************************************************************************

int TaskValidateVillainName(char* name)
{
    const VillainDef* def;

    if(!name)
        return 0;

    // Is the name a valid villain name?
    def = villainFindByName(name);
    if(def)
        return 1;

    // Is the name a valid villain group name?
    if(villainGroupGetEnum(name))
        return 1;

    return 0;
}

// DO NOT CALL KillTaskUpdatePlayerAfterSALock() from anywhere other than KillTaskUpdatePlayer()!
static void KillTaskUpdatePlayerAfterSALock(Entity* player, Entity* victim, char* neighborhood, StoryInfo *info)
{
    char mapbase[MAX_PATH];
    char villainbase[MAX_PATH];
    int taskCursor;
    int iSize;

    saUtilFilePrefix(mapbase, db_state.map_name);

    // Examine all player tasks.
    iSize = eaSize(&info->tasks);
    for(taskCursor = 0; taskCursor < iSize; taskCursor++)
    {
        StoryTaskInfo* task;
        const VillainDef* villainDef;
        const char *villainType, *villainType2;
        int oldCount;
        int countUpdated = 0;

        task = info->tasks[taskCursor];

        // Process only contact kill tasks that has not been completed.
        if(TASK_KILLX != task->def->type || TaskCompleted(task))
            continue;

        // If the killed entity matches the kill task criteria, update the task progress.
        //    StoryTask::villainType may contain a villain name or a villain group in string form.
        villainDef = victim->villainDef;
        villainType = ScriptVarsLookup(&task->def->vs, task->def->villainType, task->seed);
        villainType2 = ScriptVarsLookup(&task->def->vs, task->def->villainType2, task->seed);

        if (villainType && stricmp(villainType,"Random")==0)
        {
            villainType=villainGroupGetName(task->villainGroup);
        }
        if (villainType2 && stricmp(villainType2,"Random")==0)
        {
            villainType2=villainGroupGetName(task->villainGroup);
        }


        if (villainType && (stricmp(villainDef->name, villainType) == 0 || villainDef->group == villainGroupGetEnum(villainType)))
        {
            int failed = 0;
            if (task->def->villainMap)
            {
                saUtilFilePrefix(villainbase, task->def->villainMap);
                if (stricmp(mapbase,villainbase)!=0)
                    failed = 1;
            }
            if (task->def->villainNeighborhood && (!neighborhood || stricmp(task->def->villainNeighborhood, neighborhood)))
                failed = 1;

            if (task->def->villainVolumeName)
            {
                int found = 0, volumeIndex;
                VolumeList *volumeList = &victim->volumeList;

                if (volumeList)
                {
                    // Look through its volume list and see if it's in a target volume
                    for (volumeIndex = 0; volumeIndex < volumeList->volumeCount; volumeIndex++)
                    {
                        Volume *entVolume = &volumeList->volumes[volumeIndex];
                        if (entVolume && entVolume->volumePropertiesTracker)
                        {
                            char *entVolumeName = groupDefFindPropertyValue(entVolume->volumePropertiesTracker->def,"NamedVolume");
                            if (!entVolumeName) // For now just ignore any volume without this property
                                continue;
                            
                            if (!stricmp(entVolumeName, task->def->villainVolumeName))
                            {
                                found = 1;
                                break;
                            }
                        }
                    }
                }

                if (!found)
                    failed = 1;
            }

            if (!failed)
            {
                oldCount = task->curKillCount;
                task->curKillCount = MIN(task->curKillCount+1, task->def->villainCount);
                if(oldCount != task->curKillCount)
                    countUpdated = 1;
            }
        }

        if(villainType2 && (stricmp(villainDef->name, villainType2) == 0 || villainDef->group == villainGroupGetEnum(villainType2)))
        {
            int failed = 0;
            if (task->def->villainMap2)
            {
                saUtilFilePrefix(villainbase, task->def->villainMap2);
                if (stricmp(mapbase,villainbase)!=0)
                    failed = 1;
            }
            if (task->def->villainNeighborhood2 && (!neighborhood || stricmp(task->def->villainNeighborhood2, neighborhood)))
                failed = 1;

            if (task->def->villainVolumeName2)
            {
                int found = 0, volumeIndex;
                VolumeList *volumeList = &victim->volumeList;

                if (volumeList)
                {
                    // Look through its volume list and see if it's in a target volume
                    for (volumeIndex = 0; volumeIndex < volumeList->volumeCount; volumeIndex++)
                    {
                        Volume *entVolume = &volumeList->volumes[volumeIndex];
                        if (entVolume && entVolume->volumePropertiesTracker)
                        {
                            char *entVolumeName = groupDefFindPropertyValue(entVolume->volumePropertiesTracker->def,"NamedVolume");
                            if (!entVolumeName) // For now just ignore any volume without this property
                                continue;

                            if (!stricmp(entVolumeName, task->def->villainVolumeName2))
                            {
                                found = 1;
                                break;
                            }
                        }
                    }
                }

                if (!found)
                    failed = 1;
            }

            if (!failed)
            {
                oldCount = task->curKillCount2;
                task->curKillCount2 = MIN(task->curKillCount2+1, task->def->villainCount2);
                if(oldCount != task->curKillCount2)
                    countUpdated = 1;
            }
        }

        // Refresh the client's contact status to reflect the new kill count.
        if(countUpdated)
        {
            ContactMarkDirty(player, info, TaskGetContact(info, task));
        }

        // Has the player completed the task?
        {
            int completed = 1;

            if (task->def->villainType && task->curKillCount < task->def->villainCount)
                completed = 0;
            if (task->def->villainType2 && task->curKillCount2 < task->def->villainCount2)
                completed = 0;

            if (completed)
                TaskComplete(player, info, task, NULL, 1);
            else if ((villainType && !ValidateVillainName(villainType)) ||
                    (villainType2 && !ValidateVillainName(villainType2)))
            {
                log_printf("storyerrs.log", "Completed kill task %i:%i:%i:%i for player %s because I couldn't validate villain %s or %s",
                    SAHANDLE_ARGSPOS(task->sahandle), player->name, villainType, villainType2);
                TaskComplete(player, info, task, NULL, 1);
            }
        }
    }
}

// give this player credit for killing the critter
void KillTaskUpdatePlayer(Entity* player, Entity* victim, char* neighborhood)
{
    StoryInfo* info;
    bool didILockIt = false;

    // the kill doesn't count at all unless the player is equal or higher level than the villain
    //if (player->pchar->iCombatLevel > victim->pchar->iCombatLevel) return;

    // Make sure that:
    //        - The given player entity is a player.
    //        - The given victim entity is a villain.
    if(ENTTYPE(player) != ENTTYPE_PLAYER || !victim->villainDef)
        return;

    if (!StoryArcInfoIsLocked())
    {
        info = StoryArcLockInfo(player);
        didILockIt = true;
    }
    else
    {
        info = StoryInfoFromPlayer(player);
    }

    KillTaskUpdatePlayerAfterSALock(player, victim, neighborhood, info);
    
    if (didILockIt)
    {
        StoryArcUnlockInfo(player);
    }
}

// called whenever a player kills a villain, cycle through everyone on player's team
// and give them credit for the kill
void KillTaskUpdateCount(Entity* player, Entity* victim)
{
    int i;
    char* neighborhood = NULL;
    pmotionGetNeighborhoodFromPos(ENTPOS(victim), &neighborhood, NULL, NULL, NULL, NULL, NULL);

    if (player->teamup_id && !PlayerInTaskForceMode(player)) // give credit to whole team
    {
        for (i = 0; i < player->teamup->members.count; i++)
        {
            Entity* teammate = entFromDbId(player->teamup->members.ids[i]);
            if (teammate) KillTaskUpdatePlayer(teammate, victim, neighborhood);
        }
    }
    else // give credit to only single player
    {
        KillTaskUpdatePlayer(player, victim, neighborhood);
    }
}

// *********************************************************************************
//  Delivery Tasks
// *********************************************************************************

int *remoteDialogLookupContactHandleKeys = 0;
StashTable *remoteDialogLookupTables = 0;

// assumes you have StoryArcLockInfo(player) called already
static int DeliveryTaskHandleDialog(Entity* player, Entity* targetEntity, ContactHandle targetHandle, StoryTaskInfo *task, const char *dialog, const char *page)
{
    EntityRef tempEntityRef;
    int tempTarget, tempDialogTree;
    ContactResponse response = {0};
    const DialogDef *pDialog = NULL;
    int hash;
    ScriptVarsTable vars = {0};

    // find dialog to be used
    pDialog = DialogDef_FindDefFromStoryTask(task->def, dialog);

    // didn't find it.  Maybe parent has it?
    if (!pDialog && task->compoundParent != NULL)
    {
        pDialog = DialogDef_FindDefFromStoryTask(task->compoundParent, dialog);
    }

    if (pDialog)
    {
        DialogContext *pContext = NULL;

        pContext = (DialogContext *) malloc(sizeof(DialogContext));
        memset(pContext, 0, sizeof(DialogContext));

        pContext->pDialog = pDialog;
        pContext->ownerDBID = task->assignedDbId;
        StoryHandleCopy( &pContext->sahandle, &task->sahandle);
    
        saBuildScriptVarsTable(&vars, player, player->db_id, TaskGetContactHandle(StoryInfoFromPlayer(player), task), task, NULL, NULL, NULL, NULL, 0, 0, false);

        // store it on the NPCs
        if (targetEntity)
        {
            if (targetEntity->dialogLookup == NULL)
            {
                targetEntity->dialogLookup = (void *) stashTableCreateInt(4);
            }

            // set up dialog context for this conversation
            stashIntAddPointer(targetEntity->dialogLookup, player->db_id, pContext, true);
        }
        else
        {
            int remoteIndex;
            StashTable remoteTable;

            if (-1 != (remoteIndex = eaiFind(&remoteDialogLookupContactHandleKeys, targetHandle)))
            {
                remoteTable = remoteDialogLookupTables[remoteIndex];
            }
            else
            {
                remoteIndex = eaiSize(&remoteDialogLookupContactHandleKeys);
                eaiPush(&remoteDialogLookupContactHandleKeys, targetHandle);
                remoteTable = stashTableCreateInt(4);
                eaPush(&remoteDialogLookupTables, remoteTable);
            }

            stashIntAddPointer(remoteTable, player->db_id, pContext, true);
        }

        tempEntityRef = player->storyInfo->interactEntityRef;
        tempTarget = player->storyInfo->interactTarget;
        tempDialogTree = player->storyInfo->interactDialogTree;

        player->storyInfo->interactEntityRef = erGetRef(targetEntity);
        player->storyInfo->interactTarget = targetHandle;
        player->storyInfo->interactDialogTree = 1;
        // don't set these three, let them be whatever they were naturally
        //player->storyInfo->interactCell = 0;
        //player->storyInfo->interactPosLimited = 1;
        //copyVec3(ENTPOS(player), player->storyInfo->interactPos);

        // find start page
        hash = hashStringInsensitive(page);

        // fill out response based on dialog
        if (DialogDef_FindAndHandlePage(player, targetEntity, pDialog, hash, task, &response, targetHandle, &vars))
        {
            // open the interaction with the NPC
            START_PACKET(pak, player, SERVER_CONTACT_DIALOG_OPEN)
                ContactResponseSend(&response, pak);
            END_PACKET
            return true;
        }
        else
        {
            // reset interact* variables, we failed to dialog tree
            player->storyInfo->interactEntityRef = tempEntityRef;
            player->storyInfo->interactTarget = tempTarget;
            player->storyInfo->interactDialogTree = tempDialogTree;
        }
    }

    return false;
}

// called on the initial interaction with a potential delivery task target
// returns 1 if the target was a delivery target for a task the player has and the interaction was successful
int DeliveryTaskTargetInteract(Entity* player, Entity* targetEntity, ContactHandle targetHandle)
{
    int taskCursor;
    StoryInfo* info;
    int iSize;
    const char* targetName = NULL;

    if (ENTTYPE(player) != ENTTYPE_PLAYER)
    {
        return 0;
    }

    if (targetEntity && targetEntity->persistentNPCInfo && targetEntity->persistentNPCInfo->pnpcdef)
    {
        targetName = targetEntity->persistentNPCInfo->pnpcdef->name;
    }
    else
    {
        Contact *contact = GetContact(targetHandle);
        
        if (contact)
        {
            if (contact->pnpcParent)
            {
                targetName = contact->pnpcParent->name;
            }
            else
            {
                char filename[1024];
                const PNPCDef *pnpcDef;
                char *dotPtr;
                
                strcpy_s(filename, 1024, contact->def->filename);
                dotPtr = strrchr(filename, '.');
                dotPtr[1] = 'n';
                dotPtr[2] = 'p';
                dotPtr[3] = 'c';
                dotPtr[4] = '\0';


                pnpcDef = PNPCFindDefFromFilename(filename);

                if (pnpcDef)
                {
                    targetName = pnpcDef->name;
                }
            }
        }
    }

    if (!targetName)
        return 0;

    info = StoryArcLockInfo(player);

    //*****************************************************
    // Examine the team task.
    //*****************************************************

    //*****************************************************
    // Cannot interact with a delivery target if you aren't the task owner, so don't bother checking
    //*****************************************************

    //*****************************************************
    // Check for NPC mission door
    //*****************************************************

    if (player->teamup && player->teamup->activetask && player->teamup->activetask->def && 
        player->teamup->activetask->def->type == TASK_MISSION && !player->storyInfo->interactCell)
    {
        StoryTaskInfo *task = player->teamup->activetask;
        const MissionDef *pDef = task->def->missionref;

        if (pDef == NULL)
            pDef = *(task->def->missiondef);

        if (pDef != NULL && pDef->doorNPC != NULL && pDef->doorNPCDialog != NULL && pDef->doorNPCDialogStart != NULL)
        {
            // ok, it's a door NPC mission
            if (stricmp(targetName, pDef->doorNPC) == 0)
            {
                if (DeliveryTaskHandleDialog(player, targetEntity, targetHandle, task, pDef->doorNPCDialog, pDef->doorNPCDialogStart))
                {
                    StoryArcUnlockInfo(player);
                    return 1;
                }
            }
        }
    }

    //*****************************************************
    // Examine all of the player's tasks.
    //*****************************************************

    iSize = eaSize(&info->tasks);
    for(taskCursor = 0; taskCursor < iSize; taskCursor++)
    {
        StoryTaskInfo* task;
        const char* deliveryTargetName;
        int deliveryTargetIndex, deliveryTargetCount;
        ScriptVarsTable vars = {0};

        task = info->tasks[taskCursor];

        saBuildScriptVarsTable(&vars, player, player->db_id, TaskGetContactHandle(info, task), task, NULL, NULL, NULL, NULL, 0, 0, false);

        // if it's already complete, next!
        if (TaskCompleted(task))
            continue;

        //*****************************************************
        // Check for NPC mission door
        //*****************************************************

        if (task->def->type == TASK_MISSION && !player->storyInfo->interactCell)
        {
            // need to find mission def
            const MissionDef *pDef = task->def->missionref;
            if (pDef == NULL)
                pDef = *(task->def->missiondef);

            if (pDef && pDef->doorNPC
                && !stricmp(targetName, pDef->doorNPC)
                && pDef->doorNPCDialog && pDef->doorNPCDialogStart)
            {
                if (DeliveryTaskHandleDialog(player, targetEntity, targetHandle, task, pDef->doorNPCDialog, pDef->doorNPCDialogStart))
                {
                    StoryArcUnlockInfo(player);
                    return 1;
                }
            }
        }

        //*****************************************************
        // Check for delivery target (code assumes this is the last item in the for loop)
        //*****************************************************

        if (!task->def->deliveryTargetNames)
            continue;

        deliveryTargetCount = eaSize(&task->def->deliveryTargetNames);
        if (!deliveryTargetCount)
            continue;

        for (deliveryTargetIndex = 0; deliveryTargetIndex < deliveryTargetCount; deliveryTargetIndex++)
        {
            deliveryTargetName = ScriptVarsTableLookup(&vars, task->def->deliveryTargetNames[deliveryTargetIndex]);
            if (stricmp(targetName, deliveryTargetName) != 0)
                continue;
            if (((!task->def->deliveryTargetMethods || task->def->deliveryTargetMethods[deliveryTargetIndex] == DELIVERYMETHOD_INPERSONONLY) && player->storyInfo->interactCell)
                || (task->def->deliveryTargetMethods && task->def->deliveryTargetMethods[deliveryTargetIndex] == DELIVERYMETHOD_CELLONLY && !player->storyInfo->interactCell))
                continue;

            if (task->def->deliveryTargetDialogs != NULL &&
                task->def->deliveryTargetDialogs[deliveryTargetIndex] != NULL && 
                task->def->deliveryTargetDialogStarts != NULL &&
                task->def->deliveryTargetDialogStarts[deliveryTargetIndex] != NULL)
            {
                if (DeliveryTaskHandleDialog(player, targetEntity, targetHandle, task, task->def->deliveryTargetDialogs[deliveryTargetIndex], task->def->deliveryTargetDialogStarts[deliveryTargetIndex]))
                {
                    StoryArcUnlockInfo(player);
                    return 1;
                }
                // else continue searching through tasks because I failed the dialog tree's QualifyRequires
            } else {
                ContactResponse response = {0};

                response.responses[response.responsesFilled].link = CONTACTLINK_BYE;
                strcpy(response.responses[response.responsesFilled].responsetext, saUtilLocalize(player,"DeliveryTargetBye"));
                response.responsesFilled++;

                TaskComplete(player, info, task, &response, 1);
                if (!response.dialog[0]) // nothing filled in by task
                {
                    strcpy(response.dialog, saUtilLocalize(player,"DeliveryTargetDefault"));
                }

                START_PACKET(pak, player, SERVER_CONTACT_DIALOG_OPEN)
                    ContactResponseSend(&response, pak);
                END_PACKET;

                if (response.SOLdialog[0] != 0 && targetEntity)
                {
                    // Everything placed in SOLdialog is already saUtilTableAndEntityLocalize'd
                    saUtilEntSays(targetEntity, 1, response.SOLdialog);
                }

                StoryArcUnlockInfo(player);
                return 1;
            } 
        }
    }

    StoryArcUnlockInfo(player);
    return 0;
}

// *********************************************************************************
//  Invention Tasks
// *********************************************************************************

void InventionTaskCraft(Entity* player, const char* inventionName)
{
    StoryInfo* info;
    int i;
    int iSize;

    if (!player)
        return;

    info = StoryArcLockInfo(player);
    iSize = eaSize(&info->tasks);
    for(i = iSize - 1; i >= 0; i--) // walk backwards through the array because TaskComplete can delete
    {
        StoryTaskInfo* task = info->tasks[i];

        if(TaskCompleted(task) || !(task->def->type == TASK_CRAFTINVENTION))
            continue;

        if(!task->def->craftInventionName || !task->def->craftInventionName[0] || // any invention will work
           !stricmp(task->def->craftInventionName, inventionName))
        {
            ContactMarkDirty(player, info, TaskGetContact(info, task));
            TaskComplete(player, info, task, NULL, 1);
        }
    }
    StoryArcUnlockInfo(player);
}

// *********************************************************************************
//  Token Count Tasks
// *********************************************************************************

void TokenCountTasksModify(Entity* player, const char* tokenName)
{
    StoryInfo* info;
    int i;
    int iSize;
    int didILockIt = false;

    if (!player || !tokenName)
        return;

    if (!StoryArcInfoIsLocked())
    {
        info = StoryArcLockInfo(player);
        didILockIt = true;
    }
    else
    {
        info = StoryInfoFromPlayer(player);
    }

    iSize = eaSize(&info->tasks);
    for(i = iSize - 1; i >= 0; i--) // walk backwards through the array because TaskComplete can delete
    {
        StoryTaskInfo* task = info->tasks[i];

        if(TaskCompleted(task) || !(task->def->type == TASK_TOKENCOUNT))
            continue;

        if(!stricmp(task->def->tokenName, tokenName))
        {
            RewardToken *pToken = NULL;
            pToken = getRewardToken(player, tokenName);

            // update clients before TaskComplete in case task is deleted on complete
            ContactMarkDirty(player, info, TaskGetContact(info, task));

            // check to see if we've met the criteria
            if (pToken != NULL && pToken->val >= task->def->tokenCount)
                TaskComplete(player, info, task, NULL, 1);
        }
    }

    if (didILockIt)
        StoryArcUnlockInfo(player);
}

void TaskEnteredVolume(Entity *player, const char *volumeName)
{
    StoryInfo* info;
    int i;
    int iSize;

    if (!player || !volumeName)
        return;

    info = StoryArcLockInfo(player);
    iSize = eaSize(&info->tasks);
    for (i = iSize - 1; i >= 0; i--) // walk backwards through the array because TaskComplete can delete
    {
        StoryTaskInfo* task = info->tasks[i];

        if (TaskCompleted(task))
        {
            continue;
        }

        if (task->def->type == TASK_GOTOVOLUME
            && strstri(db_state.map_name, task->def->volumeMapName) 
            && !stricmp(task->def->volumeName, volumeName))
        {
            //update clients before TaskComplete in case task is deleted on complete
            ContactMarkDirty(player, info, TaskGetContact(info, task));

            TaskComplete(player, info, task, NULL, 1);
        }
    }
    StoryArcUnlockInfo(player);
}
