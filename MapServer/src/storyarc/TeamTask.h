/*\
 *
 *    TeamTask.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    handles setting up a 'team task'.  One task that the team
 *    is going to work on, and will get updates across map servers
 *
 */

#ifndef TEAMTASK_H
#define TEAMTASK_H

#include "storyarcinterface.h"

// Do this when handling the CLIENT_READY message and when a team member gets added to the
// teamup.
void TeamTaskAddMemberTaskList(Entity* e);

// Have all team members get rid of tasks for the specified team member.
void TeamTaskClearMemberTaskList(Entity* e);

// Add or clear all teammate task lists.
void TeamTaskTeammatesAddAll(Entity* e);
void TeamTaskTeammatesClearAll(Entity* e);

typedef enum
{
    TTSE_SUCCESS,                    // Team task selected.
    TTSE_NOT_LEADER,                // The entity cannot change the team task because it is not the leader of the team.
    TTSE_MEMBER_NOT_ON_SERVER,        // The specified teammate is not on the same mapserver as the leader.
    TTSE_INVALID_MEMBER,            // The team task cannot be changed because the specified team member is not on the team.
    TTSE_INVALID_TASK,                // The team task cannot be changed because the specified task does not exist on the team member.
    TTSE_MISSION_IN_PROGRESS,        // The team task cannot be changed because the team is already on a mission.
    TTSE_TASK_COMPLETED,            // The team task cannot be set because the has been completed.
    TTSE_NOT_ON_MAP_ALLIANCE_MISH,    // The team task cannot be set because the mission is alliance specific and all players must be on the map to set this mission.
    TTSE_MIXED_ALLIANCE_MISH,        // The team task cannot be set because the mission is alliance specific and the team is of mixed types
    TTSE_NOT_ALL_ACK,                // Not everyone has confirmed this task yet.
    TTSE_TASK_FAILED_REQUIRES,        // The select failed the requires test.  Use custom text
    TTSE_SA_FAILED_REQUIRES,        // The select failed the requires test.  Use custom text
} TeamTaskSelectError;


int TeamTaskValidateStoryTask(Entity *e, StoryTaskHandle *saHandle);
void TeamTaskSelectClear(Teamup* team);
TeamTaskSelectError TeamTaskSelect(Entity* e, int dbid, StoryTaskHandle *sahandle);
void TeamTaskValidateSelection(Entity *e, StoryTaskHandle *sa);
void TeamTaskValidateSelectionAck(Entity *e, StoryTaskHandle *saHandle, int valid, int responder);
char* TeamTaskSelectGetError(TeamTaskSelectError error);
int TeamCanSelectMission(Entity* e); // can a team-member select a mission by clicking on door?


int TeamTaskIsSet(Entity* e);
int TaskIsTeamTask(StoryTaskInfo* task, Entity* owner);
void TeamTaskClear(Entity* e);
void TeamTaskClearNoLock(Teamup* team);
void TeamTaskBroadcastComplete(Entity* e, int success);

//-------------------------------------------------------------------------
// Team change message broadcast
//-------------------------------------------------------------------------
int TeamDeconstructChangeString(char* str, int* add, int* dbid);
void TeamHandleMemberChange(int add, Entity* e);

void TeamBroadcastTeamTaskSelected(int teamID);
int TeamTaskDeconstructSelectString(char* str, int* teamID);

//-------------------------------------------------------------------------
// Team task db update utilities
//-------------------------------------------------------------------------
void TeamTaskUpdateStatus(Entity* e);
void TeamTaskUpdateStatusInternal(Teamup* team, Entity* e);

// *********************************************************************************
//  SG missions
// *********************************************************************************
void TeamTaskCheckForSGComplete(Entity *e);

#endif