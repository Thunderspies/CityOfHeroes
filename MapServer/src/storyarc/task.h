/*\
 *
 *    task.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    basic functions for handling tasks
 *
 */

#ifndef __TASK_H
#define __TASK_H

#include "storyarcinterface.h"

// *********************************************************************************
//  Task info utility functions
// *********************************************************************************

StoryTaskInfo* TaskGetInfo(Entity* player, StoryTaskHandle *sahandle);
U32* TaskGetClueStates(StoryInfo* info, StoryTaskInfo* task);
int* TaskGetNeedIntroFlag(StoryInfo* info, StoryTaskInfo* task);
void TaskAddTitle(const StoryTask* taskDef, char *response, Entity *e, ScriptVarsTable *table);
void TaskMarkSelected(StoryInfo* info, StoryTaskInfo* task);
ContactHandle TaskGetContactHandle(StoryInfo* info, StoryTaskInfo* task);
ContactHandle TaskGetContactHandleFromStoryTaskHandle(StoryInfo* info, StoryTaskHandle* sahandle);
ContactHandle EntityTaskGetContactHandle(Entity* e, StoryTaskInfo* task);
StoryContactInfo* TaskGetContact(StoryInfo* info, StoryTaskInfo* task);
int TaskGetIndex(StoryInfo* info, StoryTaskInfo* task);
        
StoryTaskInfo* TaskGetMissionTask(Entity* player, int index);
int TaskAssignedCount(int player_id, StoryInfo* info);
void TaskReassignAll(int to_id, StoryInfo* info);
int TaskIsDuplicate(StoryInfo* info, int taskIndex);
int TaskIsMission(StoryTaskInfo* task);
int TaskIsSGMission(Entity* player, StoryTaskInfo* task);
int TaskIsSGMissionEx(Entity* player, StoryTaskHandle *sahandle);
int TaskIsZoneTransfer(StoryTaskInfo* task);
int TaskCanBeAutoCompleted(Entity* player, StoryTaskInfo* task);
int TaskCanBeAbandoned(Entity* player, StoryTaskInfo* task);
int TaskCanTeleportOnComplete(StoryTaskInfo* task);
int TaskRewardCompletion(Entity* player, StoryTaskInfo* task, StoryContactInfo* contactInfo, ContactResponse* response);
void TaskClose(Entity* player, StoryContactInfo* contactInfo, StoryTaskInfo* task, ContactResponse* response);
int TaskCompleted(StoryTaskInfo* task);
int TaskCompletedAndIncremented(StoryTaskInfo* task);
int TaskFailed(StoryTaskInfo* task);
int TaskInProgress(Entity* player, StoryTaskHandle *sahandle);
int TaskIsActiveMission(Entity* player, StoryTaskInfo* task);
int TaskGetForcedLevel(StoryTaskInfo* task);
StoryTaskInfo* TaskFindTeamCompleteFlaggedTask(Entity* player, StoryInfo* info);
int TaskIsZowie(StoryTaskInfo* task);


void LoadBrokenTaskList();
int TaskIsBroken(StoryTaskInfo* task);

void StoryArcRewardMerits(Entity *e, char *arcName, int success);

// *********************************************************************************
//  Adding and removing task info
// *********************************************************************************

void TaskMarkIssued(StoryInfo* info, StoryContactInfo* contactInfo, StoryTaskInfo* task, int isIssued);
int TaskAdd(Entity* player, StoryTaskHandle *sahandle, unsigned int seed, int limitlevel, int villainGroup);
int TaskAddToSupergroup(Entity* player, StoryTaskHandle *sahandle, unsigned int seed, int limitlevel, int villainGroup);
void TaskAbandon(Entity *player, StoryInfo *info, StoryContactInfo *contactInfo, StoryTaskInfo *task, int abandonEvenIfRunning);
void TaskAbandonInvalidContacts(Entity *player);
void TaskComplete(Entity* player, StoryInfo* info, StoryTaskInfo* task, ContactResponse* response, int success);
int TaskPromptTeamComplete(Entity* player, StoryInfo* info, StoryTaskInfo* task, int levelAdjust, int level);
void TaskReceiveTeamCompleteResponse(Entity* player, int response, int neverask);
void TaskMarkComplete(Entity* player, StoryTaskHandle *sahandle, int success);
void TaskSetComplete(Entity* player, StoryTaskHandle *sahandle, int any_position, int success);
void TaskAdvanceCompleted(Entity* player);
void TaskReceiveObjectiveComplete(Entity* player, StoryTaskHandle *sahandle, int objective, int success);

// *********************************************************************************
//  Mission-task interaction
// *********************************************************************************

int TaskMissionRequestShutdown(Entity* owner, StoryTaskHandle *sahandle, int map_id);
void TaskMissionForceShutdown(Entity* owner, StoryTaskHandle *sahandle);

// *********************************************************************************
//  Task Matching - search method for tasks
// *********************************************************************************

// pretty specialized function for use by other story arc code
int TaskMatching(Entity *pPlayer, const StoryTask** tasklist, U32* bitfield, U32* uniqueBitfield, U32 flag, int player_level, int* minLevel, 
                 StoryInfo* info, const StoryTask* resultlist[], NewspaperType* newsType, PvPType* pvpType, int meetsReqsForItemOfPowerMission,
                 const char **requiresFailedString);
    // returns number of matched tasks, and places pointers in resultlist
    // looks at all tasks in tasklist, and ignores any with same bit in bitfield set
    // tasklist must be an EArray
    // any flags set in flag must be matched with task
    // bitfield is assumed to be big enough for tasklist
    // only selects task types that can be issued by a contact
    // any unique tasks will not be selected if there is a bit present in uniqueBitfield

// *********************************************************************************
//  Encounter-task interaction
// *********************************************************************************

int TaskGetEncounter(EncounterGroup* group, Entity* player);

// *********************************************************************************
//  Task debug tools
// *********************************************************************************

const char* TaskDebugName(StoryTaskHandle *sahandle, unsigned int seed);
char* TaskDisplayType(const StoryTask* def);
char* TaskDisplayState(TaskState state);
void TaskPrintCluesHelper(char ** estr, StoryTaskInfo* task);
void TaskPrintClues(ClientLink* client, StoryTaskInfo* task);
void TaskDebugClearAllTasks(Entity* player);
const char *TaskDebugLogCommand(Entity *e, StoryTaskHandle *sahandle, char const *cmd);

// *********************************************************************************
//  Objective infos - let a task keep track of when objectives were completed
// *********************************************************************************

void ObjectiveInfoAdd(StoryObjectiveInfo* objectives, StoryTaskHandle * sahandle, int objectiveNum, int success);

// *********************************************************************************
//  Unique tasks - tasks that can't be repeated, even across stature sets
// *********************************************************************************

void UniqueTaskPreload();
int UniqueTaskGetIndex(const char* logicalname);
void UniqueTaskAdd(const char* logicalname);
const char* UniqueTaskFromAttribute(const char* attribute);

// *********************************************************************************
//  Task timeouts
// *********************************************************************************

void StoryArcCheckTimeouts();    // global hook to check all task timeouts

// *********************************************************************************
//  Kill, Delivery, Invention, Token, and Go To Volume Tasks
// *********************************************************************************

void KillTaskUpdateCount(Entity* player, Entity* victim);                        // called on each kill
int DeliveryTaskTargetInteract(Entity* player, Entity* targetEntity, ContactHandle targetHandle);
void InventionTaskCraft(Entity* player, const char* inventionName);                    // called on each craft
void TokenCountTasksModify(Entity* player, const char* tokenName);                    // called when tokens are modified
void TaskEnteredVolume(Entity *player, const char *volumeName);

// task.h includes everything task related
#include "taskdef.h"
#include "taskdetail.h"
#include "locationTask.h"
#include "TeamTask.h"

#endif // __TASK_H
