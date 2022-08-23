/*\
 *
 *    clue.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    functions relating to story clues
 *
 *  general story reward stuff is here right now too
 *
 */

#include "clue.h"
#include "storyarcprivate.h"
#include "Reward.h"
#include "entity/entGameActions.h"
#include "entity/entPlayer.h"
#include "dbcomm/dbcomm.h"
#include "TeamReward.h"
#include "entity/character_level.h"
#include "entity/costume.h"
#include "entity/power_customization.h"
#include "entity/entity.h"
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include "player/badges_server.h"
#include "cmdparse/cmdserver.h"
#include "svr/svr_player.h"
#include "taskforce.h"
#include <utilitieslib/components/HashFunctions.h>
#include "storyarc/pnpcCommon.h"
#include "dbcomm/logcomm.h"
#include "player/pophelp.h"
#include "entity/character_animfx.h"
#include "storyarcutil.h"

// *********************************************************************************
//  Clues
// *********************************************************************************

int ClueFindIndexByName(const StoryClue** clues, const char* clueName)
{
    int i;
    for(i = eaSize(&clues)-1; i >= 0; i--)
    {
        const StoryClue* clue = clues[i];
        if(stricmp(clue->name, clueName) == 0)
            return i;
    }

    return -1;
}

static const StoryClue* ClueFindByName(const StoryClue** clues, const char* clueName)
{
    int i;
    for(i = eaSize(&clues)-1; i >= 0; i--)
    {
        const StoryClue* clue = clues[i];
        if(stricmp(clue->name, clueName) == 0)
            return clue;
    }

    return NULL;
}

char* ClueFindDisplayNameByName(const StoryClue** clues, const char* clueName)
{
    const StoryClue* clue = ClueFindByName(clues, clueName);
    if(clue)
        return clue->displayname;
    else
        return NULL;
}

// we don't have any actual clue icons yet
const char* ClueIconFile(const StoryClue* clue)
{
    // return clue->iconfile;
    return "icon_clue_generic.tga";
}

/////////////////////////////////////////////////////////////////////////////////////
// CluePlayerHasByName
//            Checks to see if the given player has any clues that have the specified name.
//            If any clue has the name, the fuction will immediately return true.
//
int CluePlayerHasByName(Entity *pPlayer, const char *cluename)
{
    int i;
    int iSizeTasks, iSizeArcs;
    StoryInfo* info;

    if(!pPlayer || !cluename)
        return false;

    info = StoryInfoFromPlayer(pPlayer);
    if (!info)
        return false;

    // loop through all tasks - don't count tasks that belong to storyarcs
    iSizeTasks = eaSize(&info->tasks);
    for(i = 0; i < iSizeTasks; i++)
    {
        StoryTaskInfo* task = info->tasks[i];
        const StoryClue** clues = TaskGetClues(&task->sahandle);
        U32* clueStates = TaskGetClueStates(info, task);
        int taskClueCount = eaSize(&clues);
        int clueIndex; 

        // this line is confusing..  tasks shouldn't try to check clues if they are
        // part of a story arc, because the story arc will check its own clues below
        if (isStoryarcHandle(&task->sahandle))
            continue;
        if (!clueStates) 
            continue;

        for(clueIndex = BitFieldGetFirst(clueStates, CLUE_BITFIELD_SIZE);
            clueIndex != -1;
            clueIndex = BitFieldGetNext(clueStates, CLUE_BITFIELD_SIZE, clueIndex))
        {
            const StoryClue* clue;

            if (clueIndex >= taskClueCount) break; // screen out invalid clue bits
            clue = clues[clueIndex];

            if (stricmp(clue->name, cluename) == 0)
                return true;
        }
    }

    // check the story arc clues here
    iSizeArcs = eaSize(&info->storyArcs);
    for(i = 0; i < iSizeArcs; i++)
    {
        StoryArcInfo* storyarc = info->storyArcs[i];
        int arcClueCount = eaSize(&storyarc->def->clues);
        int clueIndex;

        for(clueIndex = BitFieldGetFirst(storyarc->haveClues, CLUE_BITFIELD_SIZE);
            clueIndex != -1;
            clueIndex = BitFieldGetNext(storyarc->haveClues, CLUE_BITFIELD_SIZE, clueIndex))
        {
            StoryClue* clue;

            if (clueIndex >= arcClueCount) break; // screen out invalid clue bits

            clue = storyarc->def->clues[clueIndex];
            if (stricmp(clue->name, cluename) == 0)
                return true;
        }
    }
    return false;
}

void ClueAddRemove(Entity *player, StoryInfo* info, const char* cluename, const StoryClue** clues, U32* clueStates, StoryTaskInfo* task, int *needintro, int add)
{
    int index;

    if (!player || !info || !cluename || !clues || !clueStates || !task || !needintro)
        return;

    index = ClueFindIndexByName(clues, cluename);
    if (index >= 0)
    {
        const StoryClue* clue = clues[index];

        if(add && !BitFieldGet(clueStates, CLUE_BITFIELD_SIZE, index))
            serveFloater(player, player, "FloatFoundClue", 0.0f, kFloaterStyle_Attention, 0);

        BitFieldSet(clueStates, CLUE_BITFIELD_SIZE, index, add);
        if (info) info->clueChanged = 1;
        if (add && clue->introstring)
            *needintro = index+1;
    }
}

// used by ClueAddRemoveInternal
void ClueAddRemoveFromTask(Entity *player, StoryInfo* info, StoryTaskInfo* task, char* cluename, int add)
{
    const StoryClue** clues = TaskGetClues(&task->sahandle);
    U32* clueStates = TaskGetClueStates(info, task);
    int* needintro = TaskGetNeedIntroFlag(info, task);

    if (clues && clueStates)
    {
        ClueAddRemove(player, info, cluename, clues, clueStates, task, needintro, add);
    }
}

// force given player to be given or lose a clue,
// usually called by other mapservers
void ClueAddRemoveInternal(Entity* player, StoryTaskHandle *sahandle, char* cluename, int add)
{
    StoryInfo* info;
    StoryTaskInfo* task;
    int didILockIt = false;

    // This is a blatant hack, but necessary because sometimes this function is called inside of a lock, sometimes not
    if (!StoryArcInfoIsLocked())
    {
        info = StoryArcLockInfo(player);
        didILockIt = true;
    }
    else
    {
        info = StoryInfoFromPlayer(player);
    }
    task = TaskGetInfo(player, sahandle);
    if (task)
        ClueAddRemoveFromTask(player, info, task, cluename, add);
    if (didILockIt)
        StoryArcUnlockInfo(player);
    if (!task && player->supergroup_id) // otherwise, try getting the task from the other supergroupmode
    {
        int tmp = player->pl->taskforce_mode;
        player->pl->taskforce_mode = 0;
        if (!StoryArcInfoIsLocked())
        {
            info = StoryArcLockInfo(player);
            didILockIt = true;
        }
        else
        {
            info = StoryInfoFromPlayer(player);
        }
        task = TaskGetInfo(player, sahandle);
        if (task)
            ClueAddRemoveFromTask(player, info, task, cluename, add);
        if (didILockIt)
            StoryArcUnlockInfo(player);
        player->pl->taskforce_mode = tmp;
    }
}

void ClueAddRemoveForCurrentMission(Entity *player, const char* cluename, int add)
{
    char command[400];

    if (!g_activemission)
        return;

    sprintf(command, "clueaddremoveinternal %i %%i %i %i %s", add, g_activemission->sahandle.context, g_activemission->sahandle.subhandle, cluename);
    MissionSendToOwner(command);
}

void ClueDebugAddRemove(Entity* player, int index, char* cluename, int add)
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

        ClueAddRemoveFromTask(player, info, taskinfo, cluename, add);
    }
}


// Handles special rewarding of contact points
void StoryRewardContactPoints(StoryContactInfo* contactInfo, StoryInfo* info, int contactPoints)
{
    if (CONTACT_IS_NEWSPAPER(contactInfo->contact->def))
    {
        StoryContactInfo* broker = ContactGetInfo(info, contactInfo->brokerHandle);
        if (broker)
            broker->contactPoints += contactPoints;
    }
    else
    {
        contactInfo->contactPoints += contactPoints;
    }
}

void StoryRewardFloatText(Entity* player, const char* floatText)
{
    // Sends a floater to everyone on the mission if on a mission map
    if (OnMissionMap())
    {
        PlayerEnts *players = &player_ents[PLAYER_ENTS_ACTIVE];
        int i;

        // Send the floater to all the players
        for (i = 0; i < players->count; i++)
            if (players->ents[i])
                serveFloater(players->ents[i], players->ents[i], floatText, 0.0f, kFloaterStyle_Attention, 0);
    }
    else // Sends a floater to just the player if not on a mission map
    {
        if (player)
            serveFloater(player, player, floatText, 0.0f, kFloaterStyle_Attention, 0);
    }
}

void StoryRewardHandleTokens(Entity *player, const char **addToPlayer, const char **addToAll, const char **removeFromPlayer, const char **removeFromAll)
{
    int i, playerIndex;

    for (i = eaSize(&addToPlayer) - 1; i >= 0; i--)
        giveRewardToken(player, addToPlayer[i]);
    for (i = eaSize(&removeFromPlayer) - 1; i >= 0; i--)
        removeRewardToken(player, removeFromPlayer[i]);

    if (eaSize(&addToAll) || eaSize(&removeFromAll))
    {
        if (player->taskforce)
        {
            for (playerIndex = player->taskforce->members.count - 1; playerIndex >= 0; playerIndex--)
            {
                int teammateID = player->taskforce->members.ids[playerIndex];
                char buf[1000];
                for (i = eaSize(&addToAll) - 1; i >= 0; i--)
                {
                    sprintf(buf, "rwtokentoplayer %s %i", addToAll[i], teammateID);
                    serverParseClientEx(buf, NULL, ACCESS_INTERNAL);
                }
                for (i = eaSize(&removeFromAll) - 1; i >= 0; i--)
                {
                    sprintf(buf, "rmtokenfromplayer %s %i", removeFromAll[i], teammateID);
                    serverParseClientEx(buf, NULL, ACCESS_INTERNAL);
                }
            }
        }
        else if (player->teamup)
        {
            for (playerIndex = player->teamup->members.count - 1; playerIndex >= 0; playerIndex--)
            {
                int teammateID = player->teamup->members.ids[playerIndex];
                char buf[1000];
                for (i = eaSize(&addToAll) - 1; i >= 0; i--)
                {
                    sprintf(buf, "rwtokentoplayer %s %i", addToAll[i], teammateID);
                    serverParseClientEx(buf, NULL, ACCESS_INTERNAL);
                }
                for (i = eaSize(&removeFromAll) - 1; i >= 0; i--)
                {
                    sprintf(buf, "rmtokenfromplayer %s %i", removeFromAll[i], teammateID);
                    serverParseClientEx(buf, NULL, ACCESS_INTERNAL);
                }
            }
        }
        else
        {
            for (i = eaSize(&addToAll) - 1; i >= 0; i--)
                giveRewardToken(player, addToAll[i]);
            for (i = eaSize(&removeFromAll) - 1; i >= 0; i--)
                removeRewardToken(player, removeFromAll[i]);
        }
    }
}

void StoryRewardHandleSounds(Entity *player, const StoryReward *reward)
{
    int channelCount = eaiSize(&reward->rewardSoundChannel);
    int volumeCount = eafSize(&reward->rewardSoundVolumeLevel);
    int channel;
    F32 volume;
    int soundIndex, playerIndex;

    if (!player || !reward)
        return;

    if (reward->rewardSoundFadeTime >= 0.0f)
    {
        if (player->teamup)
        {
            for (playerIndex = player->teamup->members.count - 1; playerIndex >= 0; playerIndex--)
            {
                Entity *teammate = entFromDbId(player->teamup->members.ids[playerIndex]);
                serveFadeSound(teammate, reward->rewardSoundFadeChannel, reward->rewardSoundFadeTime);
            }
        }
        else
        {
            serveFadeSound(player, reward->rewardSoundFadeChannel, reward->rewardSoundFadeTime);
        }

        if (reward->rewardSoundFadeChannel == SOUND_MUSIC)
        {
            mission_StoreCurrentServerMusic(0, 0); // clears
        }
    }

    for (soundIndex = eaSize(&reward->rewardSoundName) - 1; soundIndex >= 0; soundIndex--)
    {
        channel = soundIndex < channelCount ? reward->rewardSoundChannel[soundIndex] : SOUND_MUSIC;
        volume = soundIndex < volumeCount ? reward->rewardSoundVolumeLevel[soundIndex] : 1.0f;

        if (player->teamup)
        {
            for (playerIndex = player->teamup->members.count - 1; playerIndex >= 0; playerIndex--)
            {
                Entity *teammate = entFromDbId(player->teamup->members.ids[playerIndex]);
                servePlaySound(teammate, reward->rewardSoundName[soundIndex], channel, volume);
            }
        }
        else
        {
            servePlaySound(player, reward->rewardSoundName[soundIndex], channel, volume);
        }
        if (channel == SOUND_MUSIC)
        {
            mission_StoreCurrentServerMusic(reward->rewardSoundName[soundIndex], volume);
        }
    }
}

void TimedObjectiveAddBonusTime(NUMBER bonusTime); // hackery

// Adds bonus time to a mission, only works on mission maps
void StoryRewardBonusTime(int bonusTime)
{
    if (g_activemission)
    {
        PlayerEnts *players = &player_ents[PLAYER_ENTS_ACTIVE];
        char buf[100];
        int i;

        // Update the mission and send a floater to everyone on the map
        if (g_activemission->timeout)
        {
            g_activemission->timeout += bonusTime;
            g_activemission->timezero += bonusTime;

            // Relay to the player so that he updates the task
            sprintf(buf, "rewardbonustime %i %i %i %i", g_activemission->ownerDbid, g_activemission->sahandle.context, g_activemission->sahandle.subhandle, bonusTime);
            serverParseClient(buf, NULL);

            // Send a floater letting players know 
            for (i = 0; i < players->count; i++)
                if (players->ents[i])
                    serveFloater(players->ents[i], players->ents[i], (bonusTime>=0)?"FloatTimeExtended":"FloatTimeRemoved", 0.0f, kFloaterStyle_Attention, 0);

            TimedObjectiveAddBonusTime(bonusTime);
        }
    }
}

// *********************************************************************************
//  Story Rewards
// *********************************************************************************

//   Returns true if contact dialog has been internally handled by the dialog tree code.
// task and contactInfo are allowed to be NULL now!
int StoryRewardApply(const StoryReward* reward, Entity* player, StoryInfo *info, StoryTaskInfo* task, StoryContactInfo* contactInfo, ContactResponse* response, Entity* speaker)
{
    int returnMe = 0;
    int i, taskIndex;
    const char* rewardstr;
    RewardAccumulator* accumulator = rewardGetAccumulator();
    const StoryClue** clues = 0;
    U32* clueStates = 0;
    int* needintro = 0;
    VillainGroupEnum vgroup = VG_NONE;
    const MissionDef *missionDef = 0;
    ScriptVarsTable vars = {0};

    if (task && contactInfo)
    {
        clues = TaskGetClues(&task->sahandle);
        clueStates = TaskGetClueStates(info, task);
        needintro = TaskGetNeedIntroFlag(info, task);
        missionDef = TaskMissionDefinition(&task->sahandle);

        // setup vars
        saBuildScriptVarsTable(&vars, player, player->db_id, contactInfo->handle, task, NULL, NULL, NULL, NULL, 0, 0, false);
    }

    rewardVarClear();

    if (contactInfo)
    {
        rewardVarSet("StoryArcName", StoryArcGetLeafName(info, contactInfo));
    }

    // give reward text
    rewardstr = NULL;
    if (reward->rewardDialog)
    {
        rewardstr = saUtilTableAndEntityLocalize(reward->rewardDialog, &vars, player);
    }

    if (rewardstr && response && task && contactInfo) // if we're in contact dialog
    {
        if (reward->rewardDialog[0] == '[')
        {
            // This is a dialog tree hook, entering special case code here
            Entity *target = erGetEnt(player->storyInfo->interactEntityRef);
            char *dialogCopy = strdup(reward->rewardDialog + 1);
            char *dialogName = strtok(dialogCopy, ":");
            char *pageName = strtok(0, "]"); // note that there may be more characters after the ] that are ignored
            if (dialogName && pageName) // otherwise this isn't a valid dialog tree hook, continue with normal contact
            {
                const DialogDef *dialog = DialogDef_FindDefFromContact(contactInfo->contact->def, dialogName);
                int hash = hashStringInsensitive(pageName);
                memset(response, 0, sizeof(ContactResponse)); // reset the response
                
                if (!dialog)
                    dialog = DialogDef_FindDefFromStoryTask(task->def, dialogName);
                
                if (DialogDef_FindAndHandlePage(player, target, dialog, hash, 0, response, contactInfo->handle, &vars))
                    returnMe = 1;
            }
            free(dialogCopy);
        }
        else
        {
            // only add title if this is the first part
            if (response->dialog && strlen(response->dialog) <= 0)
                TaskAddTitle(task->def, response->dialog, player, &vars);
            rewardstr = saUtilTableAndEntityLocalize(reward->rewardDialog, &vars, player); // need to reget this since the above call can kill the static var
            strcat(response->dialog, rewardstr);
            strcat(response->dialog, "<BR><BR>");
            if (reward->SOLdialog)
                strcpy_s(SAFESTR(response->SOLdialog), saUtilTableAndEntityLocalize(reward->SOLdialog, &vars, player));
        }
    }
    else if (rewardstr && speaker) // there is an ent to say dialog
    {
        saUtilEntSays(speaker, ENTTYPE(speaker) == ENTTYPE_NPC, rewardstr);
    }
    else if (rewardstr) // otherwise, just give chat message
    {
        sendInfoBox(player, INFO_REWARD, "%s", rewardstr);
    }

    // give caption text
    if (reward->caption)
    {
        sendPrivateCaptionMsg(player, saUtilTableAndEntityLocalize(reward->caption, &vars, player));
    }

    // give and remove clues
    if(clues && clueStates && task && contactInfo)
    {
        int iSize = eaSize(&reward->addclues);
        for(i = 0; i < iSize; i++)
            ClueAddRemove(player, info, reward->addclues[i], clues, clueStates, task, needintro, 1);

        iSize = eaSize(&reward->removeclues);
        for (i = 0; i < iSize; i++)
            ClueAddRemove(player, info, reward->removeclues[i], clues, clueStates, task, needintro, 0);
    }

    // add and remove tokens
    StoryRewardHandleTokens(player, reward->addTokens, reward->addTokensToAll, reward->removeTokens, reward->removeTokensFromAll);

    for (i = eaSize(&reward->popHelp) - 1; i >= 0; i--)
        triggerPopHelpEventHappenedByTag(player, reward->popHelp[i]);

    if (info)
    {
        for (i = eaSize(&reward->abandonContacts) - 1; i >= 0; i--)
        {
            for (taskIndex = eaSize(&info->tasks) - 1; taskIndex >= 0; taskIndex--)
            {
                StoryContactInfo *contactInfo = TaskGetContact(info, info->tasks[taskIndex]);
                if (contactInfo && contactInfo->contact->def)
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

    if (reward->rewardPlayFXOnPlayer && player)
    {
        character_PlayFX(player->pchar, player->pchar, reward->rewardPlayFXOnPlayer, colorPairNone, 0, 0, PLAYFX_NO_TINT);
    }

    if( player->taskforce && player->taskforce->architect_flags )
    {
        // clean up
        rewardVarClear();
        return returnMe; // no standard rewards for architect missions
    }

    // basic reward stuff
    if (contactInfo)
        StoryRewardContactPoints(contactInfo, info, reward->contactPoints);

    rewardSetsAward(reward->rewardSets, accumulator, player, -1, -1, NULL, 0, NULL);
    rewardApplyLogWrapper(accumulator, player, true, false, REWARDSOURCE_STORY);
    
    // random fame
    if (reward->famestring)
    {
        const char* fame;
        fame = saUtilTableLocalize(reward->famestring, &vars);
        RandomFameAdd(player, fame);
    }

    // badge stat
    if (reward->badgeStat)
        badge_StatAdd(player, reward->badgeStat, 1);

    // bonus time
    if (reward->bonusTime)
        StoryRewardBonusTime(reward->bonusTime);

    // float text
    if (reward->floatText)
        StoryRewardFloatText(player, saUtilTableAndEntityLocalize(reward->floatText, &vars, player));

    // If there is a new contact reward, flag the contact if he has someone to introduce you to
    if (reward->newContactReward && contactInfo)
    {
        ContactHandle* picks;
        contactInfo->rewardContact = 1;
        picks = ContactPickNewContacts(player, info, contactInfo, NULL, 0);
        if (!picks || !picks[0])
            contactInfo->rewardContact = 0;
    }



    if( reward->costumeSlot && player->pl->num_costume_slots < costume_get_max_earned_slots(player))
    {
        player->pl->num_costume_slots++;
        costume_destroy( player->pl->costume[costume_get_num_slots(player)-1]  );
        player->pl->costume[costume_get_num_slots(player)-1] = NULL; // destroy does not set to NULL.  NULL is checked in costume_new_slot()
        player->pl->costume[costume_get_num_slots(player)-1] = costume_new_slot( player );
        powerCustList_destroy( player->pl->powerCustomizationLists[costume_get_num_slots(player)-1] );
        player->pl->powerCustomizationLists[costume_get_num_slots(player)-1] = powerCustList_new_slot( player );
        player->pl->num_costumes_stored = costume_get_num_slots(player);
        costumeFillPowersetDefaults(player);
        player->send_costume = true;
        player->send_all_costume = true;
        player->updated_powerCust = true;
        player->send_all_powerCust = true;
    }



    // Get the villain group if there is a mission.
    if (missionDef)
    {
        vgroup = StaticDefineIntGetInt(ParseVillainGroupEnum,
            ScriptVarsLookup(&missionDef->vs, missionDef->villaingroup, task->seed));
    }

    if (task && contactInfo)
    {
        // souvenir clues and reward tables are handled differently here
        if (info->taskforce)
        {
            // if not a mission, the primary reward is given to everybody.  
            // (if it was a mission, this will be taken care of by mission the participation system)
            if (!missionDef && reward->primaryReward)
            {
                for (i = 0; i < eaSize(&reward->primaryReward); i++)
                {
                    LOG_ENT( player, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Story]:Rcv %s given by %s to entire task force", 
                        reward->primaryReward[i], task->def->logicalname);
                }
                if (player->taskforce && player->taskforce->members.count)
                {
                    for (i = 0; i < player->taskforce->members.count; i++)
                    {
                        rewardStoryApplyToDbID(player->taskforce->members.ids[i], 
                            reward->primaryReward, vgroup, task->level, 0, &vars, REWARDSOURCE_STORY);
                    }
                }
                else
                {
                    log_printf("storyerrs.log", "player %d is being issued a task force reward but isn't on a task force??", player->db_id);
                    rewardStoryApplyToDbID(player->db_id, reward->primaryReward, vgroup, task->level, 0, &vars, REWARDSOURCE_STORY);
                }
            }
            // flashback taskforces mostly behave like missions, granting the
            // primary to the leader and the secondary to everyone else. if there
            // is no primary the leader gets the secondary so that old-style
            // taskforces can be flashbacked properly.
            else if (TaskForceIsOnFlashback(player)&& player->taskforce && player->taskforce->members.leader == player->db_id)
            {

                for (i = 0; i < eaSize(&reward->primaryReward); i++)
                {
                    LOG_ENT( player, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Story]:Rcv %s given to team by flashback task %s", 
                        reward->primaryReward[i], task->def->logicalname);
                }

                for (i = 0; i < eaSize(&reward->secondaryReward); i++)
                {
                    LOG_ENT( player, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Story]:Rcv %s given to team by flashback task %s", 
                        reward->secondaryReward[i], task->def->logicalname);
                }

                for (i = 0; i < player->taskforce->members.count; i++)
                {
                    const char **playerReward = NULL;

                    // leader gets the primary reward by default, everyone else
                    // gets the secondary. if there is no reward defined for them
                    // they get the other one.
                    if (player->taskforce->members.ids[i] == player->db_id)
                        playerReward = reward->primaryReward ? reward->primaryReward : reward->secondaryReward;
                    else
                        playerReward = reward->secondaryReward ? reward->secondaryReward : reward->primaryReward;

                    if (playerReward)
                    {
                        rewardStoryApplyToDbID(player->taskforce->members.ids[i], playerReward, vgroup, task->level, 0, &vars, REWARDSOURCE_STORY);
                    }
                }
            }

            // NOTE: souvenir clues are given by mission participation code for task forces
            //        if (!missionDef && reward->souvenirClues)
            if (reward->souvenirClues)
            {
                if (player->taskforce && player->taskforce->members.count)
                {
                    for (i = 0; i < player->taskforce->members.count; i++)
                    {
                        scApplyToDbId(player->taskforce->members.ids[i], reward->souvenirClues);
                    }
                }
                else
                {
                    log_printf("storyerrs.log", "player %d is being issued a task force reward but isn't on a task force??(2)", player->db_id);
                    scApplyToDbId(player->db_id, reward->souvenirClues);
                }
            }
        }
        else // not taskforce
        {
            // Give out table-driven primary rewards to owner (player),
            if (eaSize(&reward->primaryReward))
            {
                LOG_ENT( player, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Story]:Rcv %s given by task %s", ScriptVarsTableLookup(&vars, reward->primaryReward[0]), task->def->logicalname);
                rewardStoryApplyToDbID(player->db_id, reward->primaryReward, vgroup, task->level,task->difficulty.levelAdjust, &vars, REWARDSOURCE_STORY);
            }

            // secondary rewards given out by missions are given directly by the mission server
            if (task->def && task->def->type != TASK_MISSION)
            {
                if (player->teamup && player->teamup->members.count && reward->secondaryReward)
                {
                    for (i = 0; i < eaSize(&reward->secondaryReward); i++)
                    {
                        LOG_ENT( player, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Story]:Rcv %s (secondary reward) given by task %s to entire team", 
                            reward->secondaryReward[i], task->def->logicalname);
                    }
                    for (i = 0; i < player->teamup->members.count; i++)
                    {
                        if (player->teamup->members.ids[i] != player->db_id)
                            rewardStoryApplyToDbID(player->teamup->members.ids[i], 
                            reward->secondaryReward, vgroup, task->level, 0, &vars, REWARDSOURCE_STORY);
                    }
                }
            }

            // souvenir clues
            if(reward->souvenirClues)
            {
                scApplyToDbId(player->db_id, reward->souvenirClues);
            }
        }
    }
    // clean up
    rewardVarClear();

    return returnMe;
}