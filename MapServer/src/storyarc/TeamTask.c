/*\
 *
 *    teamtask.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    handles setting up a 'team task'.  One task that the team
 *    is going to work on, and will get updates across map servers
 *
 */

#include "TeamTask.h"
#include "storyarcprivate.h"
#include "storyarcutil.h"
#include "comm_game.h"
#include "container/team.h"
#include "dbcomm/dbcontainer.h"
#include "container/containerbroadcast.h"
#include "language/langServerUtil.h"
#include "gameComm/svr_chat.h"
#include "entity/entity.h"
#include "entity/supergroup.h"
#include "entity/character_eval.h"
#include "entity/character_level.h"
#include "cmdparse/cmdserver.h"
#include "entity/alignment_shift.h"

// *********************************************************************************
//  Team task general
// *********************************************************************************

void TeamTaskAddMemberTaskList(Entity* e)
{
    Teamup* team = e->teamup;

    if(ENTTYPE_PLAYER != ENTTYPE(e) || !e->storyInfo || !team)
        return;

    TaskStatusSendUpdate(e);
}

// Have all team members get rid of tasks for the specified team member.
void TeamTaskClearMemberTaskList(Entity* e)
{
    int i;
    Teamup* team = e->teamup;

    if(ENTTYPE_PLAYER != ENTTYPE(e) || !e->storyInfo || !team)
        return;

    // If the entity is the only team member, don't bother clearing the task list.
    if(team->members.count <= 1)
        return;

    // Otherwise, construct and send an empty task update for the dbid to all team memebers.
    for(i = 0; i < team->members.count; i++)
    {
        Entity* teammate = entFromDbId(team->members.ids[i]);
        if(teammate && teammate->db_id != e->db_id)
        {
            START_PACKET(pak, teammate, SERVER_TASK_STATUS);
                pktSendBitsPack(pak, 1, 1);            // Sending 1 task update set
                pktSendBitsPack(pak, 1, e->db_id);    // Sending task update set for this player.
                pktSendBits(pak, 1, teammate->db_id == e->db_id?1:0);    // This is part of the client's taskset
                pktSendBits(pak, 1, 0);  // notoriety level (unchanged)
                pktSendBitsPack(pak, 1, 0);            // Clear the task list for the specified player.
                pktSendBitsPack(pak, 1, 0);            // Updating 0 records.
            END_PACKET
        }
    }
}

// Send all teammate's task lists.
void TeamTaskTeammatesAddAll(Entity* e)
{
    if(ENTTYPE_PLAYER != ENTTYPE(e) || !e->storyInfo || !e->teamup)
        return;
    TaskStatusSendAll(e);
}

// Clear all teammate's task lists.
void TeamTaskTeammatesClearAll(Entity* e)
{
    int i;
    Teamup* team = e->teamup;

    if(ENTTYPE_PLAYER != ENTTYPE(e) || !e->storyInfo || !team)
        return;

    // If the entity is the only team member, don't bother clearing the task list.
    if(team && team->members.count <= 1)
        return;

    // Otherwise, construct and send an empty task update for the dbid to all team memebers.
    START_PACKET(pak, e, SERVER_TASK_STATUS);
        pktSendBitsPack(pak, 1, team->members.count-1);    // Sending empty updates for all teammates except for self.
        for(i = 0; i < team->members.count; i++)
        {
            if(team->members.ids[i] != e->db_id)
            {
                pktSendBitsPack(pak, 1, team->members.ids[i]);    // Sending task update set for this player.
                pktSendBits(pak, 1, team->members.ids[i] == e->db_id?1:0);    // This is part of the client's taskset
                pktSendBits(pak, 1, 0);  // notoriety level (unchanged)
                pktSendBitsPack(pak, 1, 0);                    // Clear the task list for the specified player.
                pktSendBitsPack(pak, 1, 0);                    // Updating 0 records.
            }
        }
    END_PACKET
}

// returns true if a team member can auto-select a mission by clicking on a door
int TeamCanSelectMission(Entity* e)
{
    if (!e->teamup) 
        return 1;
    if (!e->teamup->activetask->sahandle.context) 
        return 1;
    if (e->teamup->map_id) 
        return 0;
    if ( e->teamup->activetask->sahandle.bPlayerCreated ) 
        return 1;
    if (!TASK_INPROGRESS(e->teamup->activetask->state)) 
        return 1;
    return 0;
}

void TeamTaskSelectClear(Teamup* team)
{
    Entity * leader = 0;
    if (team->taskSelect.memberValid != NULL)
    {
        eaiDestroy(&team->taskSelect.memberValid);
        team->taskSelect.memberValid = NULL;
    }

    team->taskSelect.activeTask.context = 0;
    team->taskSelect.activeTask.subhandle = 0;
    team->taskSelect.activeTask.compoundPos = 0;
    team->taskSelect.activeTask.bPlayerCreated = 0;
    team->taskSelect.team_mentor_dbid = 0;
}

TeamTaskSelectError TeamTaskSelect(Entity* e, int dbid, StoryTaskHandle *sahandle)
{
    Teamup* team = e->teamup;
    Entity* teammate = NULL;
    StoryTaskInfo* taskInfo = NULL;
    StoryInfo* storyInfo;
    int alliance = SA_ALLIANCE_BOTH;
    const char **teamRequires = NULL; 

    // Check if the entity is the leader of the team.
    // If not, the entity is not allowed to set the team task.
    if(!team || team->members.leader != e->db_id)
        return TTSE_NOT_LEADER;

    if (TaskIsSGMissionEx(e, sahandle))
    {
        taskInfo = e->supergroup->activetask;
    }
    else // Find the specified team member.
    {
        int i;
        for(i = 0; i < team->members.count; i++)
        {
            if(team->members.ids[i] == dbid)
            {
                teammate = entFromDbIdEvenSleeping(dbid);

                // If the teammate is found but isn't on this mapserver, do nothing.
                if(!teammate)
                    return TTSE_MEMBER_NOT_ON_SERVER;

                break;
            }
        }

        // If the team member cannot be found, do nothing.
        if(!teammate)
            return TTSE_INVALID_MEMBER;

        // Get the specified task from the team member.
        storyInfo = StoryInfoFromPlayer(teammate);
        if(!storyInfo || !storyInfo->tasks)
            return TTSE_INVALID_TASK;
        taskInfo = TaskGetInfo(teammate, sahandle);
    }

    // If the task index is not valid, do nothing.
    if(!taskInfo)
        return TTSE_INVALID_TASK;

    // Do not allow completed tasks to be selected.  There is currently
    // no good way to generate the contact location to get to.
    if(!TASK_INPROGRESS(taskInfo->state))
        return TTSE_TASK_COMPLETED;

    // If the task is already chosen, do nothing.
    if((team->activetask->assignedDbId == dbid || team->activetask->assignedDbId == e->supergroup_id)
        && isSameStoryTask( &team->activetask->sahandle, &taskInfo->sahandle ) )
    {
        return TTSE_SUCCESS;
    }

    // If another mission is already chosen and running, do nothing.
    //    If a mission map for the task is already running...
    if(team->map_id)
    {
        // And there are people already on the map, the team task cannot be changed.
        if(team->map_playersonmap != 0)
        {
            return TTSE_MISSION_IN_PROGRESS;
        }

        // There is no one on the map, the mission map should be shutdown so the
        // team may go on another mission or task.
    }


    // ask everyone to see if they can access this task
    alliance = storyTaskInfoGetAlliance(taskInfo);
    teamRequires = storyTaskHandleGetTeamRequires(&taskInfo->sahandle, NULL);
    if (alliance != SA_ALLIANCE_BOTH || teamRequires != NULL)
    {
        // send relay command to validate mission to all members of the team
        Entity *pTeammate = NULL;
        int i;

        TeamTaskSelectClear(team);

        team->taskSelect.activeTask.context = taskInfo->sahandle.context;
        team->taskSelect.activeTask.subhandle = taskInfo->sahandle.subhandle;
        team->taskSelect.activeTask.compoundPos = taskInfo->sahandle.compoundPos;
        team->taskSelect.activeTask.bPlayerCreated = taskInfo->sahandle.bPlayerCreated;
        team->taskSelect.team_mentor_dbid = dbid;

        for(i = 0; i < team->members.count; i++)
        {
            char cmdbuf[2000];
            sprintf(cmdbuf, "taskselectvalidate %i %i %i %i %i", team->members.ids[i], SAHANDLE_ARGSPOS(taskInfo->sahandle));
            serverParseClientEx(cmdbuf, NULL, ACCESS_INTERNAL);

        }
        return TTSE_NOT_ALL_ACK;
    }

    // Set the new team task.
    if (teammate) // teammates task
        MissionAttachTeamup(teammate, taskInfo, 1);
    else // supergroups task
        MissionAttachTeamup(e, taskInfo, 1);

    // Change everyone's active task to the new team task.
    TeamBroadcastTeamTaskSelected(e->teamup_id);

    return TTSE_SUCCESS;
}

int TeamTaskValidateStoryTask(Entity *e, StoryTaskHandle *saHandle)
{
    const StoryArc *pArc = StoryArcDefinition(saHandle);
    const StoryTask *pTask = TaskParentDefinition(saHandle);
    char *dataFilename;
    const char **teamRequires = storyTaskHandleGetTeamRequires(saHandle, &dataFilename);
    
    int retval = TTSE_SUCCESS;
    int playerAlliance = ENT_IS_ON_RED_SIDE(e)?SA_ALLIANCE_VILLAIN:SA_ALLIANCE_HERO;

    if (!e->teamup)
        return TTSE_NOT_LEADER;

    if (pArc)
    {
        // check alliance requirements
        if (pArc->alliance != SA_ALLIANCE_BOTH && playerAlliance != pArc->alliance)
            retval = TTSE_MIXED_ALLIANCE_MISH;

        // check arc requirements
        if (retval == TTSE_SUCCESS && teamRequires && !chareval_Eval(e->pchar, teamRequires, dataFilename))
            retval = TTSE_SA_FAILED_REQUIRES;
    }

    if (retval == TTSE_SUCCESS && pTask)
    {
        // check task requirements
        if (pTask->alliance != SA_ALLIANCE_BOTH && playerAlliance != pTask->alliance)
            retval = TTSE_MIXED_ALLIANCE_MISH;

        if (retval == TTSE_SUCCESS && teamRequires && !chareval_Eval(e->pchar, teamRequires, dataFilename))
            retval = TTSE_TASK_FAILED_REQUIRES;    
    }

    return retval;
}

void TeamTaskValidateSelection(Entity *e, StoryTaskHandle *saHandle)
{
    int retval = TeamTaskValidateStoryTask(e, saHandle);
    char cmdbuf[2000];

    // send back to team leader 
    if(e && e->teamup)
    {
        sprintf(cmdbuf, "taskselectvalidateack %i %i %i %i %i %i %i", e->teamup->members.leader, SAHANDLE_ARGSPOS((*saHandle)), retval, e->db_id);
        serverParseClientEx(cmdbuf, NULL, ACCESS_INTERNAL);
    }

    // send message if selecting alignment mission but new player cannot earn alignment points
    if (alignmentshift_secondsUntilCanEarnAlignmentPoint(e))
    {
        if (e && e->teamup)
        {
            sprintf(cmdbuf, "sendMessageIfOnAlignmentMission %i %i", e->teamup->members.leader, e->db_id);
            serverParseClient(cmdbuf, NULL);
        }
        else if (e)
        {
            sprintf(cmdbuf, "sendMessageIfOnAlignmentMission %i %i", e->db_id, e->db_id);
            serverParseClient(cmdbuf, NULL);
        } // else don't bother
    }
}

// logic used in StoryArcFlashbackTeamRequiresValidateAck
void TeamTaskValidateSelectionAck(Entity *e, StoryTaskHandle *saHandle, int valid, int responder)
{
    Teamup* team = e->teamup;
    Entity* teammate = NULL;
    StoryTaskInfo* taskInfo = NULL;
    StoryInfo *storyInfo;
    const char* errorMsg = NULL;
    TeamTaskSelectError error = TTSE_SUCCESS;
    int i, count;

    // see if this matches the current outstanding task request
    if (!team || team->members.leader != e->db_id)
    {
        error = TTSE_NOT_LEADER;
        TeamTaskSelectClear(team);
    }

    // see if this is the same teamleader that attempted to set this task
//    if (team->taskSelect.missionOwnerDBID != e->db_id)
    if (team->members.leader != e->db_id)
    {
        error = TTSE_NOT_LEADER;
        TeamTaskSelectClear(team);
    }

    // see if this person rejected the assignment
    if (valid == TTSE_TASK_FAILED_REQUIRES ||
        valid == TTSE_SA_FAILED_REQUIRES ||
        valid == TTSE_MIXED_ALLIANCE_MISH)
    {
        TeamTaskSelectClear(team);
        error = valid;

        // find the right error message
        if (valid == TTSE_TASK_FAILED_REQUIRES || valid == TTSE_SA_FAILED_REQUIRES)
        {
            errorMsg = storyTaskHandleGetTeamRequiresFailedText(saHandle);
        }
    }

    // add player to the list
    if (error == TTSE_SUCCESS)
    {
        count = eaiSize(&team->taskSelect.memberValid);
        for (i = count-1; i >= 0; i--)
        {
            if (team->taskSelect.memberValid[i] == responder)
                break;
        }

        if (i < 0)
        {
            eaiPush(&team->taskSelect.memberValid, responder);
        }
    }

    if (error == TTSE_SUCCESS)
    {
        if (eaiSize(&team->taskSelect.memberValid) < team->members.count)
            error = TTSE_NOT_ALL_ACK;
    }

    // are all the people on the list still members of the team?
    if (error == TTSE_SUCCESS)
    {
        count = team->members.count;
        for (i = count-1; i >= 0; i--)
        {
            if (eaiFind(&team->taskSelect.memberValid, team->members.ids[i]) < 0)
            {
                error = TTSE_NOT_ALL_ACK;
                break;
            }
        }
    }

    // attempt to set the task
    if (error == TTSE_SUCCESS)
    {
        if (TaskIsSGMissionEx(e, saHandle))
        {
            taskInfo = e->supergroup->activetask;
        }
        else // Find the specified team member.
        {
            int i;
            for(i = 0; i < team->members.count; i++)
            {
                if(team->members.ids[i] == e->teamup->taskSelect.team_mentor_dbid)
                {
                    teammate = entFromDbIdEvenSleeping(e->teamup->taskSelect.team_mentor_dbid);

                    // If the teammate is found but isn't on this mapserver, do nothing.
                    if(!teammate)
                        error = TTSE_MEMBER_NOT_ON_SERVER;
                    break;
                }
            }

            if (error == TTSE_SUCCESS)
            {
                // If the team member cannot be found, do nothing.
                if(!teammate)
                {
                    error = TTSE_INVALID_MEMBER;
                } else {
                    // Get the specified task from the team member.
                    storyInfo = StoryInfoFromPlayer(teammate);
                    if(!storyInfo || !storyInfo->tasks)
                    {
                        error = TTSE_INVALID_TASK;
                    } else {
                        taskInfo = TaskGetInfo(teammate, saHandle);
                    }
                }
            }
        }

        if (error == TTSE_SUCCESS)
        {
            // Set the new team task.
            if (teammate) // teammates task
                MissionAttachTeamup(teammate, taskInfo, 1);
            else // supergroups task
                MissionAttachTeamup(e, taskInfo, 1);

            // Change everyone's active task to the new team task.
            TeamBroadcastTeamTaskSelected(e->teamup_id);

            // remove old data
            TeamTaskSelectClear(team);
        }
    }

    if (error != TTSE_TASK_FAILED_REQUIRES &&
        error != TTSE_SA_FAILED_REQUIRES)
    {
        errorMsg = TeamTaskSelectGetError(error);
        if (errorMsg)
            sendChatMsg(e, localizedPrintf(e, errorMsg), INFO_SVR_COM, 0);
    }
    else if (errorMsg)
    {
        sendChatMsg(e, saUtilScriptLocalize(errorMsg), INFO_SVR_COM, 0);
    }

}

char* TeamTaskSelectGetError(TeamTaskSelectError error)
{
    switch(error)
    {
    case TTSE_SUCCESS:            // Team task selected.
        return "TeamTaskSelected";
        break;
    case TTSE_NOT_LEADER:        // The entity cannot change the team task because it is not the leader of the team.
        return "TeamTaskNotLeaderError";
        break;
    case TTSE_MEMBER_NOT_ON_SERVER:    // The specified teammate is not on the same mapserver as the leader.
        return "TeamTaskMemberNotOnSameServerError";
        break;
    case TTSE_INVALID_MEMBER:    // The team task cannot be changed because the specified team member is not on the team.
        return "TeamTaskInvalidMemberError";
        break;
    case TTSE_INVALID_TASK:        // The team task cannot be changed because the specified task does not exist on the team member.
        return "TeamTaskInvalidTaskError";
        break;
    case TTSE_MISSION_IN_PROGRESS:
        return "TeamTaskMissionInProgressError";
        break;
    case TTSE_TASK_COMPLETED:
        return "TeamTaskTaskCompletedError";
        break;
    case TTSE_NOT_ON_MAP_ALLIANCE_MISH:
        return "TeamTaskNotOnMapAllianceMissionError";
        break;
    case TTSE_MIXED_ALLIANCE_MISH:
        return "TeamTaskMixedTeamAllianceMissionError";
        break;
    case TTSE_NOT_ALL_ACK:
    default:
        return NULL;
    }
}

int TaskIsTeamTask(StoryTaskInfo* task, Entity* owner)
{
    Teamup* team;
    if(!task || !owner || !owner->teamup)
        return 0;

    team = owner->teamup;
    if((team->activetask->assignedDbId == owner->db_id || (TASK_IS_SGMISSION(task->def) && 
        team->activetask->assignedDbId == owner->supergroup_id)) 
        && isSameStoryTask( &team->activetask->sahandle, &task->sahandle ) )
        return 1;
    else
        return 0;
}

void TeamTaskClearNoLock(Teamup* team)
{
    memset(team->activetask, 0, sizeof(StoryTaskInfo));
    team->mission_contact = 0;
    team->mission_status[0] = 0;
}

void TeamTaskBroadcastComplete(Entity* e, int success)
{
    if (e->teamup)
    {
        if (success)
            chatSendToTeamup(e, localizedPrintf(e,"TeamTaskCompleted"), INFO_SVR_COM);
        else
            chatSendToTeamup(e, localizedPrintf(e,"TeamTaskFailed"), INFO_SVR_COM);
    }
}

int TeamTaskIsSet(Entity* e)
{
    Teamup* team = e->teamup;

    // If the entity is not on a team, it can't have a team task.
    if(!team)
        return 0;

    if(team->activetask && team->members.count > 1)
        return 1;
    else
        return 0;
}

void TeamTaskClear(Entity* e)
{
    if(!e || !e->teamup)
        return;

    // Clear out fields in the teamup structure that describes the team task.
    if(teamLock(e, CONTAINER_TEAMUPS))
    {
        TeamTaskClearNoLock(e->teamup);
        teamUpdateUnlock(e, CONTAINER_TEAMUPS);
    }
}

// *********************************************************************************
//  Team change message broadcast
// *********************************************************************************

void TeamHandleMemberChange(int add, Entity* e)
{
    if(add)
    {
        // Send the new teammate's task list to everyone.
        TeamTaskAddMemberTaskList(e);

        // Send everyone else's task list to the new teammate.
        TeamTaskTeammatesAddAll(e);
    }
    else
    {
        // Clear the old teammate's tasksfrom everyone's list.
        TeamTaskClearMemberTaskList(e);

        // Clear everyone else's task from the old teammate's list.
        TeamTaskTeammatesClearAll(e);
    }
}

void TeamBroadcastTeamTaskSelected(int teamID)
{
    char buffer[256];
    sprintf(buffer, "%s%i", DBMSG_TEAM_TASK_SELECTED, teamID);
    dbBroadcastMsg(CONTAINER_TEAMUPS, &teamID, INFO_SVR_COM, 0, 1, buffer);
}

int TeamTaskDeconstructSelectString(char* str, int* teamID)
{
    int fields = 0;
    if(!str || !teamID)
        return 0;
    fields = sscanf(str, "%i", teamID);
    if(fields != 1)
        return 0;
    else
        return 1;
}

/* Function TeamTaskUpdateStatus()
 *    This function updates the teamup structure to reflect the current
 *    state of the task.
 *
 *  Should be called when any player's task or mission status gets updated.
 *    Note that mission status can only be generated by the mapserver currently
 *    running the mission.
 */
void TeamTaskUpdateStatus(Entity* e)
{
    Teamup* team;

    // Only proceed if the entity is on a team.
    if(!e || !e->teamup)
        return;
    team = e->teamup;

    // Only proceed if team task belongs to the entity
    //   in taskforce mode, I know I will have access to this task
    if (!e->taskforce_id && (e->db_id != team->activetask->assignedDbId))
        return;

    // Update the status of the task.
    if (teamLock(e, CONTAINER_TEAMUPS))
    {
        TeamTaskUpdateStatusInternal(e->teamup, e);
        teamUpdateUnlock(e, CONTAINER_TEAMUPS);
    }
}

void TeamTaskUpdateStatusInternal(Teamup* team, Entity* e)
{
    int activeTaskUpToDate = 1;

    // Only proceed if team task belongs to the entity
    //   in taskforce mode, I know I can get this info
    if (!e->taskforce_id && (e->db_id != team->activetask->assignedDbId))
        return;

    {
        StoryInfo* info = StoryInfoFromPlayer(e);
        int i;
        for(i = eaSize(&info->tasks)-1; i >= 0; i--)
        {
            StoryTaskInfo* task = info->tasks[i];
            if( isSameStoryTask( &task->sahandle, &team->activetask->sahandle) 
                && task->sahandle.compoundPos != team->activetask->sahandle.compoundPos )
            {
                activeTaskUpToDate = 0;
            }
        }
    }

    // Update the status of the task.
    if(activeTaskUpToDate && TaskIsMission(team->activetask))
    {
        MissionConstructStatus(team->mission_status, e, team->activetask);
    }
    else
    {
        // We're looking at a non-mission task.
        int i;
        StoryInfo* info = StoryInfoFromPlayer(e);
        int iSize = eaSize(&info->tasks);

        // Find the team task on the given entity.
        for(i = 0; i < iSize; i++)
        {
            StoryTaskInfo* task = info->tasks[i];
            if(isSameStoryTask( &task->sahandle, &team->activetask->sahandle))
            {
                // Clear out the team task if it is completed.
                if(!TASK_INPROGRESS(task->state))
                {
                    TeamTaskClearNoLock(team);
                    TeamTaskBroadcastComplete(e, (task->state != TASK_FAILED));
                }
                else
                {
                    // The task is not completed yet.  Update the status.
                    memcpy(team->activetask, task, sizeof(StoryTaskInfo));
                    ContactTaskConstructStatus(team->mission_status, e, task, ContactDefinition(team->mission_contact));
                }
                break;
            }
        }
    }
}

// *********************************************************************************
//  SG missions
// *********************************************************************************


void TeamTaskCheckForSGComplete(Entity *e)
{
    Teamup* team = e->teamup;

    if(!team)
        return;

    if (TASK_IS_SGMISSION(team->activetask->def) &&
        e->supergroup && e->supergroup->activetask &&
        isSameStoryTask( &e->supergroup->activetask->sahandle, &team->activetask->sahandle) &&
        team->activetask->assignedDbId == e->supergroup_id)
    {
        TeamTaskClear(e);
    }
}