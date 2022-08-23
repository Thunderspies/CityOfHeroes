/*\
 *
 *    taskdetail.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    produces a small HTML page detailing player's progress
 *    along a compound task
 *
 */

#include "task.h"
#include "storyarcprivate.h"
#include "Reward.h"
#include "entity/powers.h"
#include "comm_game.h"
#include "entity/entity.h"
#include "entity/teamCommon.h"

// *********************************************************************************
//  Generating the detail page
// *********************************************************************************

static void TaskDetailAddClue(const char* cluename, StoryTaskInfo* task, const StoryClue** clues, U32* clueStates,
                       ScriptVarsTable* vars, char response[LEN_CONTACT_DIALOG])
{
    int index = ClueFindIndexByName(clues, cluename);
    if (BitFieldGet(clueStates, CLUE_BITFIELD_SIZE, index))
        return; // player already has this clue
    BitFieldSet(clueStates, CLUE_BITFIELD_SIZE, index, 1);
    if (index >= 0)
    {
        const StoryClue* clue = clues[index];
        strcatf(response, "<img src=%s align=right>", ClueIconFile(clue));
    }
}

static void TaskDetailReward(const char* title, const StoryReward* reward, StoryTaskInfo* task, const StoryClue** clues, U32* clueStates,
                      ScriptVarsTable* vars, char response[LEN_CONTACT_DIALOG])
{
    const char* str;
    const RewardItem* item;

    if (!reward) return; // ok
    if (!reward->rewardDialog) return;

    if (reward->addclues) // just right-aligning the clue to show up with dialog below
    {
        int i, n = eaSize(&reward->addclues);
        for (i = 0; i < n; i++)
        {
            TaskDetailAddClue(reward->addclues[i], task, clues, clueStates, vars, response);
        }
    }

    item = rewardGetFirstItem(reward->rewardSets);
    if (item)
    {
        const BasePower* power = powerdict_GetBasePowerByName(&g_PowerDictionary,
            item->power.powerCategory, item->power.powerSet, item->power.power);
        if (power)
        {
            strcatf(response, "<img align=right src=");
            if(power->eType == kPowerType_Boost)
            {
                strcatf(response, "pow:%s.%s.%s.%d", item->power.powerCategory, item->power.powerSet, item->power.power, item->power.level-1);
            }
            else
            {
                strcatf(response, "%s", power->pchIconName);
            }
            strcat(response, ">");
        }
    }

    // title
    if (title)
    {
        str = saUtilLocalize(0,title);
        strcatf(response, "<i><font face=small color=paragon>%s:</font></i> ", str);
    }

    // reward dialog
    str = saUtilTableLocalize(reward->rewardDialog, vars);
    strcat(response, "<font face=small>");
    strcat(response, str);
    strcat(response, "</font><BR><BR>");
}

// cycle through only those objectives that match the current cpos, print details for each
static void TaskDetailObjectives(const StoryTask* taskdef, int cpos, StoryTaskInfo* task, const StoryClue** clues, U32* clueStates,
                          ScriptVarsTable* vars, char response[LEN_CONTACT_DIALOG])
{
    int i;

    for (i = 0; i < MAX_OBJECTIVE_INFOS; i++)
    {
        if (!task->completeObjectives[i].set)
            break;
        if (task->completeObjectives[i].compoundPos != cpos) 
            continue;

        // only know how to do mission objectives for now
        if (task->completeObjectives[i].missionobjective && task->completeObjectives[i].success)
        {
            const MissionDef* mission = TaskMissionDefinition(&task->sahandle);
            const MissionObjective* objective;

            if (!mission) 
                return;    // bad task definition change
            objective = MissionObjectiveFromIndex(mission, task->completeObjectives[i].num);
            if (!objective)
            {
                log_printf("storyarc.log", "Invalid completeObjective found: %s (looking for objective %i)",
                    task->def->logicalname, task->completeObjectives[i].num);
            }
            else
            {
                if (!objective->reward)
                    continue;
                TaskDetailReward("MissionObjectiveHeader", objective->reward[0],
                    task, clues, clueStates, vars, response);
            }
        }
    }
}

static void TaskDetailSubtask(const StoryTask* taskdef, int cpos, StoryTaskInfo* task, const StoryClue** clues, U32* clueStates,
                       TaskState state, ScriptVarsTable* vars, char response[LEN_CONTACT_DIALOG], Entity *e )
{
    if (taskdef->taskBegin)
        TaskDetailReward(NULL, taskdef->taskBegin[0], task, clues, clueStates, vars, response);
    TaskDetailObjectives(taskdef, cpos, task, clues, clueStates, vars, response);

    // Exit String
    if (eaSize(&taskdef->missiondef))
    {
        const char *str = NULL;
        if (state == TASK_SUCCEEDED && taskdef->missiondef[0]->exittextsuccess)
            str = saUtilTableAndEntityLocalize(taskdef->missiondef[0]->exittextsuccess, vars, e);
        else if (state == TASK_FAILED && taskdef->missiondef[0]->exittextfail)
            str = saUtilTableAndEntityLocalize(taskdef->missiondef[0]->exittextfail, vars, e);
        if (str)
        {
            strcat(response, "<font face=small>");
            strcat(response, str);
            strcat(response, "</font><BR><BR>");
        }
    }
    
    if (!TASK_INPROGRESS(state))
    {
        char* header;
        const StoryReward* reward = NULL;
        int success = (state == TASK_SUCCEEDED);

        if (success && taskdef->taskSuccess)
            reward = taskdef->taskSuccess[0];
        else if (!success && taskdef->taskFailure)
            reward = taskdef->taskFailure[0];

        if (reward && taskdef->type == TASK_DELIVERITEM)
        {
            // delivery target portrait
            const char* target = ScriptVarsTableLookup(vars, taskdef->deliveryTargetNames[0]);
            int headshot = PNPCHeadshotIndex(target);
            if (headshot)
                strcatf(response, "<img src=npc:%d align=left width=46 height=46>", headshot);
            strcatf(response, "<i><font face=small color=paragon>%s:</font></i> ", PNPCDisplayName(target));
            // the following dialog will be spoken by the delivery target

            header = NULL;
        }
        else if (taskdef->type == TASK_MISSION)
        {
            if (success)
                header = "MissionCompleteHeader";
            else
                header = "MissionFailedHeader";
        }
        else
        {
            if (success)
                header = "TaskCompleteHeader";
            else
                header = "TaskFailedHeader";
        }
        if (reward) TaskDetailReward(header, reward, task, clues, clueStates, vars, response);
        else if (header)
        {
            const char* str = saUtilLocalize(0,header);
            strcatf(response, "<i><font face=small color=paragon>%s:</font></i> ", str);

            // reward dialog
            str = saUtilTableAndEntityLocalize(taskdef->inprogress_description, vars, e);
            strcat(response, "<font face=small>");
            strcat(response, str);
            strcat(response, "</font><BR><BR>");
        }
    }
}

// sg_player should be on the same team as the owner, and that should be all the caller has to worry about
void TaskGenerateDetail(int owner_id, Entity* sg_player, ContactHandle contactHandle, StoryTaskInfo* task, char response[LEN_CONTACT_DIALOG], char missionIntro[LEN_MISSION_INTRO])
{
    ScriptVarsTable vars = {0};
    U32    haveClues[CLUE_BITFIELD_SIZE];            // build a list of clues as we iterate
    const char* str;
    const StoryTask *parent;
    const StoryClue** clues;
    Contact *contact;

    if( !contactHandle  )
    {
        if( task->sahandle.bPlayerCreated )
        {
            // We know this contact
            char * contactfile = "Player_Created/MissionArchitectcontact.contact";
            contactHandle = ContactGetHandleLoose(contactfile);
        }
    }

    if(!contactHandle || !task->def )
        return; // we'll crash anyways if we continue
    
    contact = GetContact(contactHandle);

    // init vars - calls ScriptVarsTableClear at function end, potential mismatch
    saBuildScriptVarsTable(&vars, sg_player, owner_id, contactHandle, task, NULL, NULL, NULL, NULL, 0, 0, true); 

    // init an empty fake clues array
    memset(haveClues, 0, sizeof(U32) * CLUE_BITFIELD_SIZE);
    clues = TaskGetClues(&task->sahandle);

    // show the task title
    if (task->def && task->def->inprogress_description)
    {
        strcat(response, "<span align=center><font face=body color=wheat>");
        strcat(response, saUtilTableAndEntityLocalize(task->def->inprogress_description, &vars, sg_player));
        strcat(response, "</font></span><br>");
    }

    // intro with the contact
    parent = TaskParentDefinition(&task->sahandle);
    if (parent && parent->detail_intro_text)
    {
        strcatf(response, ContactHeadshot(contact, sg_player, 0));
        strcat(response, "<nolink>");
        if (!CONTACT_IS_NEWSPAPER(contact->def))
            strcatf(response, "<i><font face=small color=paragon>%s:</font></i> ", ContactDisplayName(contact->def,sg_player));
        str = saUtilTableAndEntityLocalize(parent->detail_intro_text, &vars, sg_player);
        strcat(response, "<font face=small>");
        strcat(response, str);
        strcat(response, "</font>");
        strcat(response, "<br><br>");
        strncpyt(missionIntro, str, LEN_MISSION_INTRO);

        str = saUtilTableAndEntityLocalize(parent->sendoff_text, &vars, sg_player);
        strcat(response, "<font face=small>");
        strcat(response, str);
        strcat(response, "</font></nolink>");
        strcat(response, "<br><br>");
        if (!missionIntro[0])
            strncpyt(missionIntro, str, LEN_MISSION_INTRO);
    }

    // cycle through all previous tasks in the compound task
    if (parent && parent->type == TASK_COMPOUND && task->sahandle.compoundPos > 0)
    {
        int cpos;
        for (cpos = 0; cpos < task->sahandle.compoundPos; cpos++)
        {
            const StoryTask* child = TaskChildDefinition(parent, cpos);
            TaskDetailSubtask(child, cpos, task, clues, haveClues,
                BitFieldGet(task->subtaskSuccess, SUBTASK_BITFIELD_SIZE, cpos)? TASK_SUCCEEDED: TASK_FAILED, &vars, response, sg_player); // TODO - keep track of whether they succeeded
        }
    }

    // finish with this one
    if( task->def )
        TaskDetailSubtask(task->def, task->sahandle.compoundPos, task, clues, haveClues, task->state, &vars, response, sg_player);
    ScriptVarsTableClear(&vars); // required by permanent saBuildScriptVarsTable
}

void TaskDebugDetailPage(ClientLink* client, Entity* player, int index)
{
    StoryInfo* info = StoryInfoFromPlayer(player);
    if (index >= 0 && index < eaSize(&info->tasks))
    {
        ContactResponse response;
        StoryTaskInfo* task = info->tasks[index];
        ContactHandle contact = TaskGetContactHandle(info, task);
        char missionIntro[LEN_MISSION_INTRO];
        missionIntro[0] = 0;

        memset(&response, 0, sizeof(ContactResponse));
        if (player->teamup_id && player->teamup->activetask->assignedDbId == player->db_id &&
            isSameStoryTask( &player->teamup->activetask->sahandle, &task->sahandle ) )
        {
            task = player->teamup->activetask; // get objectives from team state if possible
        }
        TaskGenerateDetail(player->db_id, player, contact, task, response.dialog, missionIntro);
        START_PACKET(pak, player, SERVER_CONTACT_DIALOG_OPEN)
            ContactResponseSend(&response, pak);
        END_PACKET
    }
}

