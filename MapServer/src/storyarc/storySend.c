/*\
 *
 *    storySend.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    all functions to send contacts, tasks, clues to client
 *
 */

#include "storySend.h"
#include "storyarcprivate.h"
#include "zowie.h"
#include "dbcomm/dbdoor.h"
#include "comm_game.h"
#include "dbcomm/dbcomm.h"
#include "entity/teamup.h"
#include "TeamReward.h"
#include "language/langServerUtil.h"
#include "entity/entity.h"
#include "dbcomm/dbnamecache.h"
#include "dbcomm/staticMapInfo.h"
#include "entity/Supergroup.h"
#include "cmdparse/cmdserver.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "missionteamup.h"
#include "entity/alignment_shift.h"
#include "entity/character_eval.h"
#include "storyarc/pnpcCommon.h"
#include "entity/character_combat.h"
#include "entity/character_level.h"
#include <utilitieslib/utils/log.h>

// *********************************************************************************
//  Update flags
// *********************************************************************************

// refresh info for tasks this client can see
void StoryArcSendTaskRefresh(Entity* e)
{
    ContactStatusSendAll(e);
    ContactVisitLocationSendAll(e);
    TaskStatusSendAll(e);
}

// refresh all info for this client
void StoryArcSendFullRefresh(Entity* e)
{
    ContactStatusSendAll(e);
    ContactStatusAccessibleSendAll(e);
    ContactVisitLocationSendAll(e);
    ClueSendAll(e);
    KeyClueSendAll(e);
    scSendAllHeaders(e);
    TaskStatusSendAll(e);
}

// *********************************************************************************
//  Contact status
// *********************************************************************************

static int ContactStatusListSend(Entity* player, StoryInfo* info, Packet* pak);
static int ContactStatusListSendChanged(Entity* player, StoryInfo* info, Packet* pak);
static int ContactStatusAccessibleListSend(Entity* player, StoryInfo* info, Packet* pak);
static void ContactStatusSend(Entity* player, StoryContactInfo* contactInfo, StoryInfo* info, Packet* pak);
static int ContactStatusAccessibleSend(Entity* player, const ContactDef *contactDef, StoryInfo* info, Packet* pak, int hasTask);

typedef struct ContactLocation {
    int mapID;
    Vec3 coord;
    ContactHandle contact;
} ContactLocation;

ContactLocation** g_contactlocations = 0;

// destroy the cached contact locations
void ContactDestroyLocations(void)
{
    eaDestroy(&g_contactlocations);
}

// try to refresh the contact mapID and coord fields
int ContactFindLocation(ContactHandle contact, int *mapID, Vec3 *coord)
{
    int i, n;
    DoorEntry* npc;
    char shortname[MAX_PATH];
    char* dot;

    if (!g_contactlocations) eaCreate(&g_contactlocations);

    // find a cached location
    n = eaSize(&g_contactlocations);
    for (i = 0; i < n; i++)
    {
        if (contact == g_contactlocations[i]->contact)
        {
            *mapID = g_contactlocations[i]->mapID;
            copyVec3(g_contactlocations[i]->coord, *coord);
            return 1;
        }
    }

    // otherwise, find it if possible
    strcpy(shortname, ContactDefinition(contact)->filename);
    dot = strrchr(shortname, '.');
    if (dot) *dot = 0;
    npc = dbFindDoorWithName(DOORTYPE_PERSISTENT_NPC, shortname);
    if (npc)
    {
        ContactLocation* newloc = calloc(sizeof(ContactLocation), 1);
        newloc->contact = contact;
        *mapID = newloc->mapID = npc->map_id;
        copyVec3(npc->mat[3], newloc->coord);
        copyVec3(npc->mat[3], *coord);
        eaPush(&g_contactlocations, newloc);
        return 1;
    }
    return 0;
}

static void SendSelectContact(Entity* e, int sel)
{
    if (sel >= 0)
    {
        START_PACKET(pak, e, SERVER_CONTACT_SELECT);
        pktSendBitsAuto(pak, sel + 1);                // have to send +1 because of empty mission contact
        END_PACKET
    }
}

void SendSelectTask(Entity* e, int sel)
{
    if (sel >= 0)
    {
        character_InterruptMissionTeleport(e->pchar);
        START_PACKET(pak, e, SERVER_TASK_SELECT);
        pktSendBitsAuto(pak, sel + 1);                // have to send +1 because of empty mission contact
        END_PACKET
    }
}

void ContactStatusSendAll(Entity* e)
{
    StoryInfo* info = StoryInfoFromPlayer(e);
    int sel;

    if (!e->client || !e->client->ready) return; // will be updated when fully connected
    START_PACKET(pak, e, SERVER_CONTACT_STATUS);
    sel = ContactStatusListSend(e, info, pak);
    END_PACKET
    SendSelectContact(e, sel);
}

void ContactStatusSendChanged(Entity* e)
{
    StoryInfo* info = StoryInfoFromPlayer(e);
    int sel;

    if (!e->storyInfo->contactInfoChanged && !e->storyInfo->contactFullUpdate) return;

    if (!e->client || !e->ready) return; // will be updated when fully connected
    
    // partial update
    if (!info->contactFullUpdate)
    {
        START_PACKET(pak, e, SERVER_CONTACT_STATUS);
        sel = ContactStatusListSendChanged(e, info, pak);
        END_PACKET
        SendSelectContact(e, sel);
    }
    else
    {
        ContactStatusSendAll(e);
        e->storyInfo->contactFullUpdate = 0;
    }
    TaskStatusSendUpdate(e);
}

void ContactStatusAccessibleSendAll(Entity *e)
{
    StoryInfo* info = StoryInfoFromPlayer(e);
    int sel;

    if (!e->client || !e->client->ready) return; // will be updated when fully connected
    START_PACKET(pak, e, SERVER_ACCESSIBLE_CONTACT_STATUS);
    sel = ContactStatusAccessibleListSend(e, info, pak);
    END_PACKET
}

void ContactMarkDirty(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo)
{
    info->contactInfoChanged = 1;
    contactInfo->dirty = 1;
    player->storyInfo->taskStatusChanged = 1;
}

void ContactMarkSelected(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo)
{
    ContactMarkDirty(player, info, contactInfo);
    info->selectedContact = contactInfo;
    info->selectedTask = 0; // prevent collisions
}

// changes notify state and updates dirty flag appropriately
void ContactSetNotify(Entity* player, StoryInfo* info, StoryContactInfo* contactInfo, int newState)
{
    if(contactInfo->notifyPlayer == newState)
        return;

    contactInfo->notifyPlayer = newState;
    ContactMarkDirty(player, info, contactInfo);
}

// returns index of selected contact, or -1
static int ContactStatusListSend(Entity* player, StoryInfo* info, Packet* pak)
{
    int i, sel = -1;
    int iSize;
    static StoryContactInfo **contactsToSend = 0;
    RewardToken *tempToken;
    int seconds[ALIGNMENT_POINT_MAX_COUNT];
    int alignment[ALIGNMENT_POINT_MAX_COUNT];
    int pointIndex;
    //eaSetSize(&contactsToSend, 0); cleared in get valid list

    // first, clear out any contacts that need to be deleted
    // since we're doing a full update, the deletes will be implicitly handled
    for (i = eaSize(&info->contactInfos) - 1; i >= 0; i--)
    {
        StoryContactInfo *contactInfo = info->contactInfos[i];
        if (contactInfo->deleteMe)
        {
            eaRemove(&info->contactInfos, i);
            storyContactInfoDestroy(contactInfo);
            sel--;
        }
        else if (contactInfo == info->selectedContact)
        {
            sel = i;
        }
    }

    ContactGetValidList(player, &contactsToSend, &info->contactInfos);

    iSize = eaSize(&contactsToSend);
    
    // How long until I can earn an alignment point (for Going Rogue system)
    alignmentshift_determineSecondsUntilCanEarnAlignmentPoints(player, seconds, alignment);
    
    for (pointIndex = 0; pointIndex < ALIGNMENT_POINT_MAX_COUNT; pointIndex++)
    {
        pktSendBitsAuto(pak, seconds[pointIndex]);
        pktSendBitsAuto(pak, alignment[pointIndex]);
    }

    pktSendBitsAuto(pak, (tempToken = getRewardToken(player, "roguepoints_hero")) ? tempToken->val : 0);
    pktSendBitsAuto(pak, (tempToken = getRewardToken(player, "roguepoints_vigilante")) ? tempToken->val : 0);
    pktSendBitsAuto(pak, (tempToken = getRewardToken(player, "roguepoints_villain")) ? tempToken->val : 0);
    pktSendBitsAuto(pak, (tempToken = getRewardToken(player, "roguepoints_rogue")) ? tempToken->val : 0);

    // How many contacts are there?
    pktSendBitsAuto(pak, iSize);

    // How many contact deletions are we sending?
    pktSendBitsAuto(pak, 0);

    // How many contact updates are we sending?
    pktSendBitsAuto(pak, iSize);

    // Send individual contact updates.
    for(i = 0; i < iSize; i++)
    {
        pktSendBitsAuto(pak, i+1); // index is one-based to get past mission status
        ContactStatusSend(player, contactsToSend[i], info, pak);
    }

    // client has been brought up to date with the current state of all contacts.
    player->storyInfo->contactInfoChanged = 0;
    info->selectedContact = 0; // clean up server selection flag now that we've processed it
    return sel;
}

// returns selected contact index or -1
static int ContactStatusListSendChanged(Entity* player, StoryInfo* info, Packet* pak)
{
    int i;
    int updateCount = 0;
    int deleteCount = 0;
    int sel = -1;
    int iSize;
    RewardToken *tempToken;
    int seconds[ALIGNMENT_POINT_MAX_COUNT];
    int alignment[ALIGNMENT_POINT_MAX_COUNT];
    int pointIndex;

    static StoryContactInfo **contactsToSend = 0;
    //eaSetSize(&contactsToSend, 0); cleared in get valid list

    ContactGetValidList(player, &contactsToSend, &info->contactInfos);

    iSize = eaSize(&contactsToSend);

    // How long until I can earn an alignment point (for Going Rogue system)
    alignmentshift_determineSecondsUntilCanEarnAlignmentPoints(player, seconds, alignment);

    for (pointIndex = 0; pointIndex < ALIGNMENT_POINT_MAX_COUNT; pointIndex++)
    {
        pktSendBitsAuto(pak, seconds[pointIndex]);
        pktSendBitsAuto(pak, alignment[pointIndex]);
    }

    pktSendBitsAuto(pak, (tempToken = getRewardToken(player, "roguepoints_hero")) ? tempToken->val : 0);
    pktSendBitsAuto(pak, (tempToken = getRewardToken(player, "roguepoints_vigilante")) ? tempToken->val : 0);
    pktSendBitsAuto(pak, (tempToken = getRewardToken(player, "roguepoints_villain")) ? tempToken->val : 0);
    pktSendBitsAuto(pak, (tempToken = getRewardToken(player, "roguepoints_rogue")) ? tempToken->val : 0);

    // How many contacts are there?
    pktSendBitsAuto(pak, iSize);

    // Determine contacts to remove
    for (i = eaSize(&info->contactInfos) - 1; i >= 0; i--)
    {
        if (info->contactInfos[i]->deleteMe)
        {
            deleteCount++;
        }
    }

    // How many contacts do I need to remove?
    pktSendBitsAuto(pak, deleteCount);

    // Send notifications and actually delete contacts
    if (deleteCount)
    {
        for (i = eaSize(&info->contactInfos) - 1; i >= 0; i--)
        {
            StoryContactInfo *contactInfo = info->contactInfos[i];
            if (contactInfo->deleteMe)
            {
                pktSendBitsAuto(pak, contactInfo->handle);
                eaRemove(&info->contactInfos, i);
                storyContactInfoDestroy(contactInfo);
            }
        }
    }

    // How many contact updates are we sending?
    for(i = 0; i < iSize; i++)
    {
        StoryContactInfo* contactInfo = contactsToSend[i];
        if(contactInfo->dirty)
        {
            updateCount++;
        }
    }
    pktSendBitsAuto(pak, updateCount);

    // Send individual contact updates.
    for(i = 0; i < iSize; i++)
    {
        StoryContactInfo* contactInfo = contactsToSend[i];
        if(contactInfo->dirty)
        {
            pktSendBitsAuto(pak, i+1); // index is one-based to get past mission status
            ContactStatusSend(player, contactsToSend[i], info, pak);
            if (contactInfo == info->selectedContact)
            {
                sel = i;
            }
        }
    }

    // client has been brought up to date with the current state of all contacts.
    player->storyInfo->contactInfoChanged = 0;
    info->selectedContact = 0; // clean up server selection flag now that we've processed it
    return sel;
}

// returns selected contact index or -1
static int ContactStatusAccessibleListSend(Entity* player, StoryInfo* info, Packet* pak)
{
    int i;
    int sel = -1;
    int iSize;
    const ContactDef **contactsAccessibleNowToSend = 0;
    const ContactDef **contactsAccessibleSoonToSend = 0;
    int universeIndex = ENT_IS_IN_PRAETORIA(player) ? 1 : 0;
    int allianceIndex = ENT_IS_VILLAIN(player) ? 1 : 0;
    int levelIndex = character_GetExperienceLevelExtern(player->pchar) - 1;
    int accessibleNowIndex = universeIndex * ACCESSIBLE_ALLIANCE_COUNT * ACCESSIBLE_LEVEL_COUNT
        + allianceIndex * ACCESSIBLE_LEVEL_COUNT
        + levelIndex;
    int contactIndex, contactCount;

    // determine now contact list
    contactCount = eaSize(&g_contactlist.accessibleContactDefLists[accessibleNowIndex]);
    for (contactIndex = 0; contactIndex < contactCount; contactIndex++)
    {
        const ContactDef *def = g_contactlist.accessibleContactDefLists[accessibleNowIndex][contactIndex];

        if (!ContactGetInfoFromDefinition(info, def))
        {
            eaPushConst(&contactsAccessibleNowToSend, def);
        }
    }

    // send now contact list
    iSize = eaSize(&contactsAccessibleNowToSend);
    pktSendBitsAuto(pak, iSize);

    for (i = 0; i < iSize; i++)
    {
        if (ContactStatusAccessibleSend(player, contactsAccessibleNowToSend[i], info, pak, 1))
            sel = i;
    }

    return sel;
}

static char* ContactTaskKillGetVillainLocalizedName(const char* villainType, int singular)
{
    char* displayName;
    int villainGroup = villainGroupGetEnum(villainType);

    // Does the villain type appear to specify a villain group?
    if(villainGroup == VG_NONE)
    {
        const VillainDef *villain = villainFindByName(villainType);
        if (villain && villain->levels && villain->levels[0] && villain->levels[0]->displayNames)
        {
            const char *villainName = villain->levels[0]->displayNames[0];
            if (villainName)
            {
                displayName = localizedPrintf(0, villainName);
            }
            else
            {
                displayName = "UNKNOWN"; // should never happen
            }
        }
        else
        {
            const char* groupName;
            groupName = villainGroupGetPrintName(villainGroup);
            if(groupName)
                displayName = localizedPrintf(0, groupName);
            else
                displayName = "UNKNOWN"; // should never happen
        }
    }
    else
    {
        char type[1024];
        if (singular) {
            strcpy(type, villainGroupGetPrintNameSingular(villainGroup));
        } else {
            strcpy(type, villainGroupGetPrintName(villainGroup));
        }
        displayName = localizedPrintf(0, type);
    }

    return displayName;
}

static int bit_count(U32 n)
{
    int count;

    count = 0;
    while (n)
    {
        if (n&1)
            count++;
        n>>=1;
    }
    return count;
}

// constructs a detailed contact status if one is available, otherwise returns NULL
static const char* ContactTaskConstructStatusInternal(StoryTaskInfo* task, Entity* player, const ContactDef* contactdef)
{
    const char* result = NULL;
    ScriptVarsTable vars = {0};
    
    saBuildScriptVarsTable(&vars, player, player->db_id, ContactGetHandleFromDefinition(contactdef), task, NULL, NULL, NULL, NULL, 0, 0, false);
    
    if(TaskCompleted(task))
    {
        if (TaskFailed(task))
        {
            result = saUtilTableAndEntityLocalize(task->def->taskFailed_description, &vars, player);
            if (!result)
                result = saUtilLocalize(player,"TaskFailed");
        }
        else
        {
            result = saUtilTableAndEntityLocalize(task->def->taskComplete_description, &vars, player);
            if (!result)
                result = saUtilLocalize(player,"TaskComplete");
        }
    }
    else
    {
        switch(task->def->type)
        {
        case TASK_KILLX:
            {
                const char *villainType, *villainType2;
                char tempString[10];

                villainType = ScriptVarsTableLookup(&vars, task->def->villainType);
                villainType2 = ScriptVarsTableLookup(&vars, task->def->villainType2);

                if (villainType && stricmp(villainType,"Random")==0)
                {
                    villainType=villainGroupGetName(task->villainGroup);
                }
                if (villainType2 && stricmp(villainType2,"Random")==0)
                {
                    villainType2=villainGroupGetName(task->villainGroup);
                }

                // Display "Defeat <x> <villain name>" or "Defeat <x> more <villain name>"
                if(!villainType2)
                {
                    char villainTypeName[1024];
                    int remain = task->def->villainCount - task->curKillCount;

                    if (remain == 1 && task->def->villainSingularDescription)
                    {
                        result = saUtilTableAndEntityLocalize(task->def->villainSingularDescription, &vars, player);
                    }
                    else if (task->def->villainDescription)
                    {
                        ScriptVarsTablePushVar(&vars, "Remain", itoa(remain, tempString, 10));
                        result = saUtilTableAndEntityLocalize(task->def->villainDescription, &vars, player);
                    }
                    else
                    {
                        strncpy(villainTypeName, ContactTaskKillGetVillainLocalizedName(villainType, remain==1), 1024);
                        result = saUtilLocalize(player,remain == task->def->villainCount? "KillXProgress1Start" : "KillXProgress1", remain, villainTypeName);
                    }
                }
                else // 2 villain group task
                {
                    char villainTypeName[1024];
                    char villainTypeName2[1024];
                    int remain1 = task->def->villainCount - task->curKillCount;
                    int remain2 = task->def->villainCount2 - task->curKillCount2;
                    strncpy(villainTypeName, ContactTaskKillGetVillainLocalizedName(villainType, remain1==1), 1024);
                    strncpy(villainTypeName2, ContactTaskKillGetVillainLocalizedName(villainType2, remain2==1), 1024);

                    if (remain1 && remain2) // both remain
                    {
                        if (task->def->villainDescription && task->def->villainDescription2)
                        {
                            char* hackString;  
                            
                            if (remain1 == 1 && task->def->villainSingularDescription)
                            {
                                result = saUtilTableAndEntityLocalize(task->def->villainSingularDescription, &vars, player);
                            }
                            else
                            {
                                ScriptVarsTablePushVar(&vars, "Remain", itoa(remain1, tempString, 10));

                                result = saUtilTableAndEntityLocalize(task->def->villainDescription, &vars, player);
                            }

                            TODO(); // re-using char buffer returned from saUtilTableAndEntityLocalize, which could be NULL. Fix this to pass full string before localizing. 
                            hackString = (char*)result;
                            strcat(hackString, localizedPrintf(0,"MissionObjectiveSeparator"));

                            if (remain2 == 1 && task->def->villainSingularDescription2)
                            {
                                strcat(hackString, saUtilTableAndEntityLocalize(task->def->villainSingularDescription2, &vars, player));
                            }
                            else
                            {
                                ScriptVarsTablePushVar(&vars, "Remain", itoa(remain2, tempString, 10));

                                strcat(hackString, saUtilTableAndEntityLocalize(task->def->villainDescription2, &vars, player));
                            }
                        }
                        else
                        {
                            char* str;
                            if (remain1 == task->def->villainCount && remain2 == task->def->villainCount2)
                                str = "KillXProgress2Start";
                            else if (remain1 == task->def->villainCount)
                                str = "KillXProgress2Start1";
                            else if (remain2 == task->def->villainCount2)
                                str = "KillXProgress2Start2";
                            else
                                str = "KillXProgress2";

                            result = saUtilLocalize(player,str, remain1, villainTypeName, remain2, villainTypeName2);
                        }
                    }
                    else if (remain2)
                    {
                        if (remain1 == 1 && task->def->villainSingularDescription2)
                        {
                            result = saUtilTableAndEntityLocalize(task->def->villainSingularDescription2, &vars, player);
                        }
                        else if (task->def->villainDescription2)
                        {
                            ScriptVarsTablePushVar(&vars, "Remain", itoa(remain2, tempString, 10));
                            result = saUtilTableAndEntityLocalize(task->def->villainDescription2, &vars, player);
                        }
                        else
                        {
                            result = saUtilLocalize(player,remain2 == task->def->villainCount2? "KillXProgress1Start" : "KillXProgress1", remain2, villainTypeName2);
                        }
                    }
                    else
                    {
                        if (remain1 == 1 && task->def->villainSingularDescription)
                        {
                            result = saUtilTableAndEntityLocalize(task->def->villainSingularDescription, &vars, player);
                        }
                        else if (task->def->villainDescription)
                        {
                            ScriptVarsTablePushVar(&vars, "Remain", itoa(remain1, tempString, 10));
                            result = saUtilTableAndEntityLocalize(task->def->villainDescription, &vars, player);
                        }
                        else
                        {
                            result = saUtilLocalize(player,remain1 == task->def->villainCount? "KillXProgress1Start" : "KillXProgress1", remain1, villainTypeName);
                        }
                    }
                }
            }
            break;

        case TASK_VISITLOCATION:
            {
                // Display "Visit <x> locations" or "Visit <x> more locations"
                int total = eaSize(&task->def->visitLocationNames);
                int remain = total - task->nextLocation;
                if (total == 1)
                    result = NULL;
                else if (remain == 1)
                    result = saUtilLocalize(player,"VisitLocationProgressLast");
                else if (total == remain)
                    result = saUtilLocalize(player,"VisitLocationProgressStart", remain);
                else
                    result = saUtilLocalize(player,"VisitLocationProgress", remain);
            }
            break;

        case TASK_TOKENCOUNT:
            {
                RewardToken *pToken = NULL;
                int count = 0;

                if ((pToken = getRewardToken(player, task->def->tokenName)) != NULL) 
                {
                    count = pToken->val;
                }

                if (count == 0) 
                {
                    ScriptVarsTablePushVar(&vars, "token", "0");
                } else {
                    ScriptVarsTablePushVarEx(&vars, "token", (char*)(intptr_t)count, 'I', 0);
                }

                // Display 
                result = saUtilTableAndEntityLocalize(task->def->tokenProgressString, &vars, player);
            }
            break;

        case TASK_ZOWIE:
            {
                const char *zdesc;
                const char *zdesc_translated;
                int num_remaining;
                ScriptVarsTable vars = {0};
                char num_remaining_string[3];

                num_remaining = zowie_ObjectivesRemaining(task);
                snprintf(num_remaining_string, sizeof(num_remaining_string), "%d", num_remaining);
                ScriptVarsTablePushVar(&vars, "Remain", num_remaining_string);

                // Display "Zounds! {remain} more to go!"
                zdesc = "VisitLocationProgress";
                if (task->def->zowieDescription)
                    zdesc = task->def->zowieDescription;
                if ((num_remaining == 1) && (task->def->zowieSingularDescription != NULL))
                    zdesc = task->def->zowieSingularDescription;
                zdesc_translated = saUtilTableLocalize(zdesc, &vars);
                result = zdesc_translated;
            }
            break;

        case TASK_GOTOVOLUME:
            {
                const char *desc = "GoToVolumeProgress";

                if (task->def->volumeDescription)
                    desc = task->def->volumeDescription;

                result = saUtilTableLocalize(desc, 0);
            }
            break;

        default: // case TASK_MISSION, TASK_CRAFTINVENTION:
            break;
        } // switch
    } // else

    return result;
}

void ContactTaskConstructStatus(char* str, Entity* owner, StoryTaskInfo* task, const ContactDef* contactdef)
{
    if (!str)
        return;
    str[0] = '\0';

    // If the task is a mission...
    if (TaskIsMission(task))
    {
        // If the task/mission is complete, say so...
        //    Use default messages.
        if (TaskCompleted(task))
        {
            const char* status = NULL;
            if(task->compoundParent)
            {
                // Normally, complete compound subtasks would be advanced automatically, unless
                // it is a mission subtask.  In that case, we need to show the mission status of
                // the subtask for awhile before being able to move on to the next subtask.
                MissionConstructStatus(str, owner, task);
            }
            else
            {
                status = ContactTaskConstructStatusInternal(task, owner, contactdef);
            }
            if (status)
                strcpy(str, status);
        }

        // otherwise, return the status string stored in the teamup structure.
        //    Since only the mission mapserver knows that current status of the mission,
        //    "mission_status" is the only place where the status can be found.
        else if (owner->teamup && TaskIsActiveMission(owner, task))
        {
            strcpy(str, owner->teamup->mission_status);
        }
    }
    else
    {
        const char* status;
        status = ContactTaskConstructStatusInternal(task, owner, contactdef);
        if (status)
            strcpy(str, status);
    }
}

StoryLocation s_locations[50];

// numlocs is the size of the s_locations array, passed in and out when this function
// is called multiple times.
StoryLocation* ContactTaskGetLocations(Entity* player, StoryTaskInfo* task, int* numlocs)
{
    ScriptVarsTable vars = {0};

    // don't go over the array boundary
    if (*numlocs >= 50)
    {
        devassert("Too many locations in ContactTaskGetLocations!");
        return s_locations;
    }

    // Make sure it is the right map first
    if (db_state.local_server)
    {
        int baseMapId = staticMapInfoMatchNames(saUtilCurrentMapFile());
        int doormapid = task->doorId / 10000;
        if (!doormapid || (baseMapId != doormapid))
            return s_locations;
    }

    saBuildScriptVarsTable(&vars, player, player->db_id, TaskGetContactHandle(StoryInfoFromPlayer(player), task), task, NULL, NULL, NULL, NULL, 0, 0, false);

    // Search for specially-designated locations, which take precedence over task-type-generated ones
    if (task->def->taskLocationMap && strstri(db_state.map_name, task->def->taskLocationMap))
    {
        const char* deliveryTargetName = ScriptVarsTableLookup(&vars, task->def->taskLocationName);
        VisitLocation* loc = VisitLocationFind(deliveryTargetName);

        if (loc)
        {
            // if we got a location, set it up
            copyVec3(loc->coord, s_locations[*numlocs].coord);
            s_locations[*numlocs].type = CONTACTDEST_EXPLICITTASK;
            s_locations[*numlocs].mapID = db_state.map_id;
            s_locations[*numlocs].locationName[0] = '\0';

            (*numlocs)++;
            if (*numlocs == 50)
            {
                // return early so I don't accidentally stomp memory
                return s_locations;
            }
        }
    }
    
    // get the location from the task
    switch (task->def->type)
    {
    case TASK_MISSION:
        {
            char* doorname;

            if (task->doorId)    // Door Mission
            {
                // decide if we're on an active mission or not
                s_locations[*numlocs].type = CONTACTDEST_MISSION;
                if (player->teamup_id && player->teamup->activetask->doorId == task->doorId)
                    s_locations[*numlocs].type = CONTACTDEST_ACTIVEMISSION;

                // do other fields
                copyVec3(task->doorPos, s_locations[*numlocs].coord);
                s_locations[*numlocs].mapID = task->doorMapId;
                doorname = dbGetDoorName(task->doorMapId);
                if (doorname)
                    strcpy(s_locations[*numlocs].locationName, localizedPrintf(0,doorname)); // TODO - make sure these are going to be localized in menuMessages.ms
                else
                    strcpy(s_locations[*numlocs].locationName, saUtilLocalize(player,"MissionDoor"));

                (*numlocs)++;
                if (*numlocs == 50)
                {
                    // return early so I don't accidentally stomp memory
                    return s_locations;
                }
            } 
            else 
            {
                const MissionDef *pDef = task->def->missionref;
                if (pDef == NULL)
                    pDef = *(task->def->missiondef);
                if (pDef != NULL && pDef->doorNPC != NULL)// NPC Door Mission
                {
                    StoryLocation* loc;

                    loc = PNPCFindLocation(pDef->doorNPC);
                    if (!loc) break;
                    s_locations[*numlocs] = *loc;
                    s_locations[*numlocs].type = CONTACTDEST_TASK;

                    (*numlocs)++;
                    if (*numlocs == 50)
                    {
                        // return early so I don't accidentally stomp memory
                        return s_locations;
                    }
                }
            }
            break;
        }
    case TASK_CONTACTINTRO:
    case TASK_DELIVERITEM:
        {
            StoryLocation* loc;
            const char* deliveryTargetName;

            deliveryTargetName = ScriptVarsTableLookup(&vars, task->def->deliveryTargetNames[0]);
            loc = PNPCFindLocation(deliveryTargetName);
            if (!loc) break;
            s_locations[*numlocs] = *loc;
            s_locations[*numlocs].type = CONTACTDEST_TASK;

            (*numlocs)++;
            if (*numlocs == 50)
            {
                // return early so I don't accidentally stomp memory
                return s_locations;
            }

            break;
        }
    case TASK_VISITLOCATION:
        {
            const char* locname = LocationTaskNextLocation(task);
            VisitLocation* loc = VisitLocationFind(locname);
            if (!loc) break; // location not on this map

            // if we got a location, set it up
            copyVec3(loc->coord, s_locations[*numlocs].coord);
            s_locations[*numlocs].type = CONTACTDEST_TASK;
            s_locations[*numlocs].mapID = db_state.map_id;
            strcpy(s_locations[*numlocs].locationName, saUtilLocalize(player,"NextLocation"));

            (*numlocs)++;
            if (*numlocs == 50)
            {
                // return early so I don't accidentally stomp memory
                return s_locations;
            }

            break;
        }
    }

    return s_locations;
}

void ContactStatusSendEmpty(Packet* pak)
{
    // The fields sent here must match the ones in ContactStatusSend()
    // and ContactStatusAccessibleSend() which are immediately below,
    // and also the ones in ContactStatusReceive() in the client! 
    // If they do not, things like taskforces will break and there will be gloom and unhappiness!
    pktSendBits(pak, 1, 0);            // no filename coming
    pktSendString(pak, "");            // name
    pktSendString(pak, "");            // location
    pktSendBits(pak, 1, 0);            // call override
    pktSendBits(pak, 1, 0);            // ask about override
    pktSendBits(pak, 1, 0);            // leave override
    pktSendBits(pak, 1, 0);            // image override
    pktSendBitsAuto(pak, 0);        // costume
    pktSendBitsAuto(pak, 0);        // handle
    pktSendBitsAuto(pak, 0);        // no task
    pktSendBitsAuto(pak, 0);        // no task
    pktSendBits(pak, 1, 0);            // notify player
    pktSendBits(pak, 1, 0);            // has tasks
    pktSendBits(pak, 1, 0);            // can use cell
    pktSendBits(pak, 1, 0);            // is the newspaper
    pktSendBits(pak, 1, 0);            // no story arcs
    pktSendBits(pak, 1, 0);            // no mini arcs
    pktSendBitsPack(pak, 1, 0);            // tip type
    pktSendBits(pak, 1, 0);            // won't interact
    pktSendBits(pak, 1, 0);            // is meta contact
    pktSendBitsAuto(pak, 0);        // current contact points
    pktSendBitsAuto(pak, 0);        // friend level
    pktSendBitsAuto(pak, 0);        // confidant level
    pktSendBitsAuto(pak, 0);        // complete level
    pktSendBitsAuto(pak, 0);        // heist level
    pktSendBitsAuto(pak, 0);        // no alliance
    pktSendBits(pak, 1, 0);            // no location
}

// returns whether this contact should be selected
static void ContactStatusSend(Entity* player, StoryContactInfo* contactInfo, StoryInfo* info, Packet* pak)
{
    char contactname[MAX_SA_NAME_LEN];
    const char* s;
    StoryTaskInfo* task = ContactGetAssignedTask(player->db_id, contactInfo, info);
    StoryArcInfo *pSIInfo = StoryArcGetInfo(info, contactInfo);
    int numlocs = 0;
    int selected = 0;
    int mapID;
    Vec3 coord;
    int haveloc = 0;
    int hasTask;
    int cannotInteract = ContactCannotInteract(player, contactInfo->handle, CONTACTLINK_HELLO, 0);
    int canUseCell = CONTACT_CAN_USE_CELL(contactInfo);
    ContactTaskPickInfo shortTask;

    hasTask = (cannotInteract == 0 || cannotInteract == CONTACTLINK_HELLO)
                && (ContactHasNewTasksForPlayer(player, contactInfo, info) || ContactCanIntroduceNewContacts(player, info, contactInfo));

    // special cases for brokers
    if (CONTACT_IS_BROKER(contactInfo->contact->def))
    {
        if (!ContactGetInfo(info, ContactGetNewspaper(player)))
        {
            hasTask = true;
        }
        else if (ContactShouldGiveHeist(contactInfo))
        {
            ContactPickHeistTask(player, info, contactInfo, &shortTask);
            if (shortTask.sahandle.context)
                hasTask = true;
        }
        else 
        {
            hasTask = false;
        }
    }

    if ((CONTACT_IS_AUTOHIDE(contactInfo->contact->def) && !task) || 
        (CONTACT_IS_HIDEONCOMPLETE(contactInfo->contact->def) && contactInfo->contactRelationship >= CONTACT_CONFIDANT))
    {
        contactInfo->dirty = 0;
        ContactStatusSendEmpty(pak);
        return;
    }

    // If you are adding fields to the contact to be sent from here, make
    // sure you update ContactStatusSendEmpty() which is right above this
    // function! Otherwise you'll break things like taskforces and people
    // will become unhappy.
    pktSendBits(pak, 1, server_state.clickToSource);
    if (server_state.clickToSource)
        pktSendString(pak, saUtilDirtyFileName(contactInfo->contact->def->filename));
    s = ContactDisplayName(contactInfo->contact->def, player);
    pktSendString(pak, s);                                // contact name
    strcpy(contactname, s); // needed for loc later
    s = saUtilScriptLocalize(contactInfo->contact->def->locationName);
    pktSendString(pak, s);                                // descriptive location
    pktSendBits(pak, 1, !!contactInfo->contact->def->callTextOverride);
    if(!!contactInfo->contact->def->callTextOverride)
        pktSendString(pak, saUtilScriptLocalize(contactInfo->contact->def->callTextOverride));
    pktSendBits(pak, 1, !!contactInfo->contact->def->askAboutTextOverride);
    if(!!contactInfo->contact->def->askAboutTextOverride)
        pktSendString(pak, saUtilScriptLocalize(contactInfo->contact->def->askAboutTextOverride));
    pktSendBits(pak, 1, !!contactInfo->contact->def->leaveTextOverride);
    if(!!contactInfo->contact->def->leaveTextOverride)
        pktSendString(pak, saUtilScriptLocalize(contactInfo->contact->def->leaveTextOverride));
    pktSendBits(pak, 1, !!contactInfo->contact->def->imageOverride);
    if(!!contactInfo->contact->def->imageOverride)
        pktSendString(pak, contactInfo->contact->def->imageOverride);

    if (CONTACT_HAS_HEADSHOT(contactInfo->contact->def))
        pktSendBitsAuto(pak, ContactNPCNumber(contactInfo->contact->def,player));    // NPC costume
    else
        pktSendBitsAuto(pak, 0);
    pktSendBitsAuto(pak, contactInfo->handle);            // handle
    pktSendBitsAuto(pak, task? task->sahandle.context: 0);
    pktSendBitsAuto(pak, task? task->sahandle.subhandle: 0);
    //pktSendBitsAuto(pak, task? contact->revokeTime: 0);

    pktSendBits(pak, 1, contactInfo->notifyPlayer);                        // notify player
    pktSendBits(pak, 1, hasTask);                                    // has task
    pktSendBits(pak, 1, canUseCell);                                // can use cell
    pktSendBits(pak, 1, !(!CONTACT_IS_NEWSPAPER(contactInfo->contact->def)));    // is the newspaper

    if (pSIInfo == NULL)
    {    
        pktSendBits(pak, 1, 0);        // no story arcs
        pktSendBits(pak, 1, 0);        // no mini arcs
    } else {
        if (pSIInfo->def->flags & STORYARC_MINI)
        {
            pktSendBits(pak, 1, 0);        // no story arcs
            pktSendBits(pak, 1, 1);        // yes mini arcs
        } else {
            pktSendBits(pak, 1, 1);        // yes story arcs
            pktSendBits(pak, 1, 0);        // no mini arcs
        }
    }

    pktSendBitsPack(pak, 1, contactInfo->contact->def->tipType);
    pktSendBits(pak, 1, contactInfo->contact->def->interactionRequires ? 
                            !chareval_Eval(player->pchar, contactInfo->contact->def->interactionRequires, contactInfo->contact->def->filename) : 
                            0);            // won't interact
    pktSendBits(pak, 1, CONTACT_IS_METACONTACT(contactInfo->contact->def) ? 1 : 0); // is meta contact

    // contact relationship levels
    pktSendBitsAuto(pak, contactInfo->contactPoints);    // current contact points
    pktSendBitsAuto(pak, contactInfo->contact->def->friendCP);    // friend level
    pktSendBitsAuto(pak, contactInfo->contact->def->confidantCP); // confidant level
    pktSendBitsAuto(pak, contactInfo->contact->def->completeCP);    // complete level
    pktSendBitsAuto(pak, contactInfo->contact->def->heistCP);        // heist level

    pktSendBitsAuto(pak, contactInfo->contact->def->alliance);

    // Which map is the contact on?
    haveloc = ContactFindLocation(contactInfo->handle, &mapID, &coord);
    if (haveloc)
    {
        pktSendBits(pak, 1, 1);                            // have location
        if (contactInfo->contact->pnpcParent && contactInfo->contact->pnpcParent->name)
        {
            pktSendBits(pak, 1, 1);
            pktSendString(pak, contactInfo->contact->pnpcParent->name);
        }
        else
        {
            pktSendBits(pak, 1, 0);
        }
        if (contactInfo->contact->entParent)
        {
            pktSendBits(pak, 1, 1);
            pktSendBitsAuto(pak, contactInfo->contact->entParent->id);
        }
        else
        {
            pktSendBits(pak, 1, 0);
        }
        pktSendF32(pak, coord[0]);                        // coords
        pktSendF32(pak, coord[1]);
        pktSendF32(pak, coord[2]);
        if (db_state.local_server)
            pktSendString(pak, saUtilCurrentMapFile());
        else
            pktSendString(pak, dbGetMapName(mapID));        // map
        pktSendString(pak, contactname);                // name of location
    }
    else
    {
        pktSendBits(pak, 1, 0);                            // no location
    }

    // client has been brought up to date with the current state of the contact.
    contactInfo->dirty = 0;
}

// returns whether this contact should be selected, which is never
// assumes you don't have this contact in your contact list yet
static int ContactStatusAccessibleSend(Entity* player, const ContactDef* contactDef, StoryInfo* info, Packet* pak, int hasTask)
{
    char contactname[MAX_SA_NAME_LEN];
    const char* s;
    int numlocs = 0;
    int selected = 0;
    int mapID;
    Vec3 coord;
    int haveloc = 0;
    int canUseCell = 0; // not just yet, since you don't have this contact yet
    int contactHandle = ContactGetHandleFromDefinition(contactDef);
    Contact* contact = GetContact(contactHandle);

    if (!contactDef)
        return 0;

    // If you are adding fields to the contact to be sent from here, make
    // sure you update ContactStatusSendEmpty() which is right above this
    // function! Otherwise you'll break things like taskforces and people
    // will become unhappy.
    pktSendBits(pak, 1, server_state.clickToSource);
    if (server_state.clickToSource)
        pktSendString(pak, saUtilDirtyFileName(contactDef->filename));
    s = ContactDisplayName(contactDef, player);
    pktSendString(pak, s);                                // contact name
    strcpy(contactname, s); // needed for loc later
    s = saUtilScriptLocalize(contactDef->locationName);
    pktSendString(pak, s);                                // descriptive location
    pktSendBits(pak, 1, !!contactDef->callTextOverride);
    if(!!contactDef->callTextOverride)
        pktSendString(pak, saUtilScriptLocalize(contactDef->callTextOverride));
    pktSendBits(pak, 1, !!contactDef->askAboutTextOverride);
    if(!!contactDef->askAboutTextOverride)
        pktSendString(pak, saUtilScriptLocalize(contactDef->askAboutTextOverride));
    pktSendBits(pak, 1, !!contactDef->leaveTextOverride);
    if(!!contactDef->leaveTextOverride)
        pktSendString(pak, saUtilScriptLocalize(contactDef->leaveTextOverride));
    pktSendBits(pak, 1, !!contactDef->imageOverride);
    if(!!contactDef->imageOverride)
        pktSendString(pak, contactDef->imageOverride);

    if (CONTACT_HAS_HEADSHOT(contactDef))
        pktSendBitsAuto(pak, ContactNPCNumber(contactDef,player));    // NPC costume
    else
        pktSendBitsAuto(pak, 0);
    pktSendBitsAuto(pak, contactHandle);            // handle
    pktSendBitsAuto(pak, 0);
    pktSendBitsAuto(pak, 0);
    //pktSendBitsAuto(pak, 0);

    pktSendBits(pak, 1, 0);                                            // notify player
    pktSendBits(pak, 1, hasTask);                                    // has task
    pktSendBits(pak, 1, canUseCell);                                // can use cell
    pktSendBits(pak, 1, !(!CONTACT_IS_NEWSPAPER(contactDef)));        // is the newspaper

    pktSendBits(pak, 1, 0);        // no story arcs
    pktSendBits(pak, 1, 0);        // no mini arcs

    pktSendBitsPack(pak, 1, contactDef->tipType);
    pktSendBits(pak, 1, contactDef->interactionRequires ? 
                            !chareval_Eval(player->pchar, contactDef->interactionRequires, contactDef->filename) : 
                            0);            // won't interact
    pktSendBits(pak, 1, CONTACT_IS_METACONTACT(contactDef) ? 1 : 0); // is meta contact

    // contact relationship levels
    pktSendBitsAuto(pak, 0);    // current contact points
    pktSendBitsAuto(pak, contactDef->friendCP);    // friend level
    pktSendBitsAuto(pak, contactDef->confidantCP); // confidant level
    pktSendBitsAuto(pak, contactDef->completeCP);    // complete level
    pktSendBitsAuto(pak, contactDef->heistCP);        // heist level

    pktSendBitsAuto(pak, contactDef->alliance);

    // Which map is the contact on?
    haveloc = ContactFindLocation(contactHandle, &mapID, &coord);
    if (haveloc)
    {
        pktSendBits(pak, 1, 1);                            // have location
        if (contact->pnpcParent && contact->pnpcParent->name)
        {
            pktSendBits(pak, 1, 1);
            pktSendString(pak, contact->pnpcParent->name);
        }
        else
        {
            pktSendBits(pak, 1, 0);
        }
        if (contact->entParent)
        {
            pktSendBits(pak, 1, 1);
            pktSendBitsAuto(pak, contact->entParent->id);
        }
        else
        {
            pktSendBits(pak, 1, 0);
        }
        pktSendF32(pak, coord[0]);                        // coords
        pktSendF32(pak, coord[1]);
        pktSendF32(pak, coord[2]);
        if (db_state.local_server)
            pktSendString(pak, saUtilCurrentMapFile());
        else
            pktSendString(pak, dbGetMapName(mapID));        // map
        pktSendString(pak, contactname);                // name of location
    }
    else
    {
        pktSendBits(pak, 1, 0);                            // no location
    }

    // client has been brought up to date with the current state of the contact.
    return 0; // can't be selected, you don't have it in your list
}

// *********************************************************************************
//  Task status
// *********************************************************************************

static int TaskStatusSendPlayer(Entity* player, Packet* pak, Entity* sendingto)
{
    int selected = -1;
    int i;
    StoryInfo* info = StoryInfoFromPlayer(player);
    int iSize;

    if(!info)
        return selected;

    pktSendBitsPack(pak, 1, player->db_id);
    pktSendBits(pak, 1, player->db_id == sendingto->db_id?1:0);    // is this for the client's taskset?
    pktSendBits(pak,1, 1);
    pktSendBitsAuto(pak, player->storyInfo->difficulty.levelAdjust);
    pktSendBitsAuto(pak, player->storyInfo->difficulty.teamSize);
    pktSendBits(pak, 1, player->storyInfo->difficulty.dontReduceBoss);
    pktSendBits(pak, 1, player->storyInfo->difficulty.alwaysAV);


    if (db_state.is_endgame_raid)
    {
        pktSendBitsPack(pak, 1, 0); // how many tasks
        pktSendBitsPack(pak, 1, 0); // how many updates
    }
    else
    {
        iSize = eaSize(&info->tasks);
        pktSendBitsPack(pak, 1, iSize); // how many tasks
        pktSendBitsPack(pak, 1, iSize); // how many updates
        for (i = 0; i < iSize; i++)
        {
            StoryContactInfo *pContact = TaskGetContact(info, info->tasks[i]);
            pktSendBitsPack(pak, 1, i+1); // index is one-based to get past mission record
            TaskStatusSend(player, info->tasks[i], pContact?pContact->contact->def:NULL, pak);

            if (info->tasks[i] == info->selectedTask)
                selected = i;
        }
    }

    info->selectedTask = 0; // clean up server selection flag now that we've processed it
    return selected;
}


// only doing full updates of tasks for now
void TaskStatusSendAll(Entity* e)
{
    StoryInfo* info = StoryInfoFromPlayer(e);
    int selected = -1;
    Teamup* team = e->teamup;

    if (!e->client || !e->client->ready) 
        return; // will be updated when fully connected

    START_PACKET(pak, e, SERVER_TASK_STATUS);

    if(team)
    {
        int i;
        Entity** teammates = NULL;
        int teammateCount;

        // Record all entities that are on the mapserver.
        for(i = 0; i < team->members.count; i++)
        {
            Entity* teammate;

            // Sending task status for self?
            if(team->members.ids[i] == e->db_id)
            {
                eaPush(&teammates, e);
                continue;
            }

            // Sending task status for all teammates?
            teammate = entFromDbId(team->members.ids[i]);
            if(teammate)
                eaPush(&teammates, teammate);
        }

        // Send out task updates for all teammates that are on the mapserver.
        teammateCount = eaSize(&teammates);

        pktSendBitsPack(pak, 1, teammateCount);
        for(i = 0; i < teammateCount; i++)
        {
            Entity* teammate = teammates[i];
            int result;
            result = TaskStatusSendPlayer(teammate, pak, e);
            if(e == teammate)
                selected = result;
        }

        if(teammates)
            eaDestroy(&teammates);
    }
    else
    {
        pktSendBitsPack(pak, 1, 1);
        selected = TaskStatusSendPlayer(e, pak, e);
    }

    END_PACKET
    if (selected >= 0)
        SendSelectTask(e, selected);

}

void TaskStatusSendUpdate(Entity* e)
{
    StoryInfo* info = StoryInfoFromPlayer(e);
    int selected = -1;
    Teamup* team = e->teamup;

    if (!e->client || e->client->ready < CLIENTSTATE_REQALLENTS )
        return; // will be updated when fully connected
    
    if(team)
    {
        int i;
        int teammateCount = 1;

        // Send out this entity's task list update to all teammates on the mapserver.
        for(i = 0; i < team->members.count; i++)
        {
            Entity* teammate = entFromDbId(team->members.ids[i]);
            int result;

            if(!teammate || !teammate->ready )
                continue;

            START_PACKET(pak, teammate, SERVER_TASK_STATUS);
            pktSendBitsPack(pak, 1, teammateCount);
            result = TaskStatusSendPlayer(e, pak, teammate);
            END_PACKET

            if(e == teammate)
                selected = result;
        }
    }
    else if( e->ready )
    {
        START_PACKET(pak, e, SERVER_TASK_STATUS);
        pktSendBitsPack(pak, 1, 1);
        selected = TaskStatusSendPlayer(e, pak, e);
        END_PACKET
    }

    if ( e->ready && selected >= 0)
        SendSelectTask(e, selected);
}

void TaskStatusSendNotorietyUpdate(Entity* e)
{
    if (e->teamup)
    {
        int i;
        for(i = 0; i < e->teamup->members.count; i++)
        {
            Entity* teammate = entFromDbId(e->teamup->members.ids[i]);
            if (!teammate || !teammate->ready)
                continue;

            START_PACKET(pak, teammate, SERVER_TASK_STATUS);
            pktSendBitsPack(pak, 1, 1);            // Sending 1 task update set
            pktSendBitsPack(pak, 1, e->db_id);    // Sending task update set for this player.
            pktSendBits(pak, 1, teammate->db_id == e->db_id?1:0);    // This is part of the client's taskset
            pktSendBits(pak, 1, 1); // Updating Difficulty 
            pktSendBitsAuto(pak, e->storyInfo->difficulty.levelAdjust);
            pktSendBitsAuto(pak, e->storyInfo->difficulty.teamSize);
            pktSendBits(pak, 1, e->storyInfo->difficulty.dontReduceBoss);
            pktSendBits(pak, 1, e->storyInfo->difficulty.alwaysAV);
            pktSendBitsPack(pak, 1, -1);        // Not updating the number of tasks
            pktSendBitsPack(pak, 1, 0);            // Updating 0 records.
            END_PACKET
        }
    }
    else
    {
        START_PACKET(pak, e, SERVER_TASK_STATUS);
        pktSendBitsPack(pak, 1, 1);            // Sending 1 task update set
        pktSendBitsPack(pak, 1, e->db_id);    // Sending task update set for this player.
        pktSendBits(pak, 1, 1);                // This is part of the client's taskset
        pktSendBits(pak, 1, 1); // Updateing Difficulty
        pktSendBitsAuto(pak, e->storyInfo->difficulty.levelAdjust);
        pktSendBitsAuto(pak, e->storyInfo->difficulty.teamSize);
        pktSendBits(pak, 1, e->storyInfo->difficulty.dontReduceBoss);
        pktSendBits(pak, 1, e->storyInfo->difficulty.alwaysAV);
        pktSendBitsPack(pak, 1, -1);        // Not updating the number of tasks
        pktSendBitsPack(pak, 1, 0);            // Updating 0 records.
        END_PACKET
    }
}

void TaskSelectSomething(Entity* e)
{
    StoryInfo* info = StoryInfoFromPlayer(e);
    if (info)
    {
        StoryTaskInfo* task;
        int i, n;
        int found = 0;
        n = eaSize(&info->tasks);

        // look for a mission first
        for (i = 0; i < n; i++)
        {
            task = info->tasks[i];
            if (TaskIsMission(task))
            {
                TaskMarkSelected(info, task);
                found = 1;
                break;
            }
        }

        // or get the last regular task
        if (n && !found)
        {
            task = info->tasks[n-1];
            TaskMarkSelected(info, task);
        }
    }
}

int TaskIsRunningMission(Entity* player, StoryTaskInfo* task)
{
    Teamup* team = player->teamup;
    if(    team && (team->map_id || db_state.local_server) && // team->map_id is always zero on local mapservers
        team->activetask->assignedDbId == task->assignedDbId && isSameStoryTask( &team->activetask->sahandle, &task->sahandle) )
        return 1;
    else
        return 0;
}

void TaskStatusSend(Entity* player, StoryTaskInfo* task, const ContactDef* contactdef, Packet* pak)
{
    char description[1000];
    char taskStatus[1000];
    ScriptVarsTable vars = {0};
    StoryLocation *location;
    int haveloc = 0;
    int sel = 0;

    // init vars
    saBuildScriptVarsTable(&vars, player, player->db_id, ContactGetHandleFromDefinition(contactdef), task, NULL, NULL, NULL, NULL, 0, 0, false);
    
    if (!contactdef)
    {
        LOG( LOG_ERROR, LOG_LEVEL_IMPORTANT, 0, "Could not get contact for task %s, player %s", task->def->logicalname, player->name);
    }

    strcpy(description, saUtilTableAndEntityLocalize(task->def->inprogress_description, &vars, player));
    pktSendBits(pak, 1, server_state.clickToSource);
    if (server_state.clickToSource)
        pktSendString(pak, saUtilDirtyFileName(task->def->filename)); // filename
    pktSendString(pak, description);                                        // description
    pktSendString(pak, "");                                            // this player is the owner
    if (TASK_IS_SGMISSION(task->def))
        pktSendBitsPack(pak, 1, player->supergroup_id);
    else
        pktSendBitsPack(pak, 1, player->db_id);

    pktSendBitsPack(pak, 1, task->sahandle.context);
    pktSendBitsPack(pak, 1, task->sahandle.subhandle);
    pktSendBitsPack(pak, 1, task->level);

    pktSendBitsAuto(pak, task->difficulty.levelAdjust);
    pktSendBitsAuto(pak, task->difficulty.teamSize);
    pktSendBits(pak, 1, task->difficulty.dontReduceBoss);
    pktSendBits(pak, 1, task->difficulty.alwaysAV);


    pktSendBitsPack(pak, 2, storyTaskInfoGetAlliance(task) );

    taskStatus[0] = '\0';
    ContactTaskConstructStatus(taskStatus, player, task, contactdef);
    pktSendString(pak, taskStatus);                                            // current status

    pktSendBitsPack(pak, 1, task->timeout);                                    // timeout
    pktSendBitsAuto(pak, task->timezero);
    pktSendBitsAuto(pak, task->timerType);
    pktSendBitsAuto(pak, task->failOnTimeout);
    pktSendBits(pak, 1, TaskCompleted(task));                                // completed
    pktSendBits(pak, 1, TaskContainsMission(&task->sahandle)? 1: 0); // is mission
    pktSendBits(pak, 1, TaskIsRunningMission(player, task) && player->teamup->map_id);    // Task has running mission map? 
                                                    // NOTE: explicitly checking map_id because of isDevelopmentMode() hack in TaskIsRunningMission!
    pktSendBits(pak, 1, !(!TASK_IS_SGMISSION(task->def)));
    pktSendBits(pak, 1, TaskIsZoneTransfer(task));                        // Zone transfer task?
    pktSendBits(pak, 1, TaskCanTeleportOnComplete(task));                // Teleport on complete?
    pktSendBits(pak, 1, !(!TASK_FORCE_TIMELIMIT(task->def)));            // Task enforces time limit even after completion
    pktSendBits(pak, 1, TaskCanBeAbandoned(player, task));
    pktSendBits(pak, 1, TaskIsZowie(task));

    // get the location for the task
    // haveloc is returned as the current size of the location array, maxes at 50
    location = ContactTaskGetLocations(player, task, &haveloc);
    // this code assumes there's only one location, even though multiple could be returned
    if (haveloc)
    {
        pktSendBits(pak, 1, 1);
        pktSendF32(pak, location->coord[0]);
        pktSendF32(pak, location->coord[1]);
        pktSendF32(pak, location->coord[2]);
        pktSendBitsAuto(pak, location->type);
        if (db_state.local_server)
            pktSendString(pak, saUtilCurrentMapFile());
        else
            pktSendString(pak, dbGetMapName(location->mapID));
        pktSendString(pak, description);
    }
    else
    {
        pktSendBits(pak, 1, 0);

        if (task->def->taskLocationMap)
        {
             pktSendBits(pak, 1, 1);
            pktSendBitsAuto(pak, CONTACTDEST_EXPLICITTASK);
            pktSendString(pak, task->def->taskLocationMap);
        }
        else
        {
            pktSendBits(pak, 1, 0);
        }
    }

    if (TaskIsZowie(task))
    {
        int map_id;
        char baseName[MAX_PATH];

        pktSendBitsPack(pak, 1, task->completeSideObjectives);
        pktSendBits(pak, 32, task->seed);

        saUtilFilePrefix(baseName, task->def->villainMap);
        strcat_s(baseName, sizeof(baseName), ".txt");
        map_id = dbGetMapId(baseName);
        pktSendBitsPack(pak, 1, map_id);

        pktSendString(pak, task->def->zowieType);                        // zowie type
        pktSendString(pak, saUtilTableAndEntityLocalize(task->def->zowieDisplayName, &vars, player));    // zowie name
        pktSendString(pak, task->def->zowiePoolSize);                    // zowie pool sizes
    }
}

// *********************************************************************************
//  Sending clues
// *********************************************************************************
void ClueSend(const StoryClue* clue, ScriptVarsTable* vars, Packet* pak, Entity * e);

void ClueSendChanged(Entity* player)
{
    if(!player || !player->storyInfo)
        return;
    if(player->storyInfo->clueChanged)
    {
        ClueSendAll(player);
    }
}

void ClueSendAll(Entity* player)
{
    int i;
    StoryInfo* info;
    int iSizeTasks, iSizeArcs;
    int clueCount = 0;

    if(!player)
        return;

    info = StoryInfoFromPlayer(player);

    // Count the number of clues to be sent.
    iSizeTasks = eaSize(&info->tasks);
    for(i = 0; i < iSizeTasks; i++)
    {
        StoryTaskInfo* task = info->tasks[i];
        const StoryClue** clues = TaskGetClues(&task->sahandle);
        U32* clueStates = TaskGetClueStates(info, task);
        int taskClueCount = eaSize(&clues);
        int clueIndex; 

        // this line is confusing..  tasks shouldn't try to send clues if they are
        // part of a story arc, because the story arc will send its own clues below
        if (isStoryarcHandle(&task->sahandle))
            continue;
        if (!clueStates) 
            continue;

        for(clueIndex = BitFieldGetFirst(clueStates, CLUE_BITFIELD_SIZE);
            clueIndex != -1;
            clueIndex = BitFieldGetNext(clueStates, CLUE_BITFIELD_SIZE, clueIndex))
        {
            if (clueIndex >= taskClueCount) break; // screen out invalid clue bits
            clueCount++;
        }
    }
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
            if (clueIndex >= arcClueCount) break; // screen out invalid clue bits
            clueCount++;
        }
    }

    START_PACKET(pak, player, SERVER_CLUE_UPDATE)
        pktSendBitsPack(pak, 1, clueCount);

    // Send each clue.
    // Iterate over all active tasks.
    for(i = 0; i < iSizeTasks; i++)
    {
        StoryTaskInfo* task = info->tasks[i];
        ScriptVarsTable vars = {0};
        const StoryClue** clues = TaskGetClues(&task->sahandle);
        U32* clueStates = TaskGetClueStates(info, task);
        int taskClueCount = eaSize(&clues);
        int clueIndex;

        // this line is confusing..  tasks shouldn't try to send clues if they are
        // part of a story arc, because the story arc will send its own clues below
        if (isStoryarcHandle(&task->sahandle))
            continue;
        if (!clueStates) 
            continue;

        saBuildScriptVarsTable(&vars, player, player->db_id, TaskGetContactHandle(info, task), task, NULL, NULL, NULL, NULL, 0, 0, false);

        // Iterate over all clues for the task.
        for(clueIndex = BitFieldGetFirst(clueStates, CLUE_BITFIELD_SIZE);
            clueIndex != -1;
            clueIndex = BitFieldGetNext(clueStates, CLUE_BITFIELD_SIZE, clueIndex))
        {
            const StoryClue* clue;

            if (clueIndex >= taskClueCount) break; // screen out invalid clue bits
            clue = clues[clueIndex];
            ClueSend(clue, &vars, pak, player);
        }
    }
    for(i = 0; i < iSizeArcs; i++)
    {
        ScriptVarsTable vars = {0};
        StoryArcInfo* storyarc = info->storyArcs[i];
        int arcClueCount = eaSize(&storyarc->def->clues);
        int clueIndex;

        saBuildScriptVarsTable(&vars, player, player->db_id, storyarc->contact, NULL, NULL, NULL, storyarc, NULL, 0, 0, false);

        // Iterate over all clues for the task.
        for(clueIndex = BitFieldGetFirst(storyarc->haveClues, CLUE_BITFIELD_SIZE);
            clueIndex != -1;
            clueIndex = BitFieldGetNext(storyarc->haveClues, CLUE_BITFIELD_SIZE, clueIndex))
        {
            StoryClue* clue = 0;
            // StoryClueInfo *clueInfo = 0; // StoryClueInfo will be needed here if resurrected

            if (clueIndex >= arcClueCount) break; // screen out invalid clue bits
            clue = storyarc->def->clues[clueIndex];
            ClueSend(clue, &vars, pak, player);
        }
    }
    END_PACKET

    player->storyInfo->clueChanged = 0;
}

void ClueSend(const StoryClue* clue, ScriptVarsTable* vars, Packet* pak, Entity * e)
{
    const char* s;

    pktSendString(pak, clue->name);

    pktSendBits(pak, 1, server_state.clickToSource);
    if (server_state.clickToSource)
        pktSendString(pak, saUtilDirtyFileName(clue->filename));

    s = saUtilTableAndEntityLocalize(clue->displayname, vars, e);
    pktSendString(pak, s);

    s = saUtilTableAndEntityLocalize(clue->detailtext, vars, e);
    pktSendString(pak, s);

    pktSendString(pak, ClueIconFile(clue));
}

// *********************************************************************************
//  Key Clues
// *********************************************************************************

void KeyClueSendChanged(Entity* player)
{
    if (player->storyInfo->keyclueChanged)
        KeyClueSendAll(player);
}

// key clues are basically team clues
void KeyClueSendAll(Entity* player)
{
    ScriptVarsTable vars = {0};
    const MissionDef* mission;
    const StoryTaskInfo *task;
    int clueIndex;
    int clueCount = 0;
    player->storyInfo->keyclueChanged = 0;

    // most of the time..
    if (!player->teamup_id || !player->teamup || !player->teamup->keyclues)
    {
        START_PACKET(pak, player, SERVER_KEYCLUE_UPDATE)
        pktSendBitsPack(pak, 1, 0);
        END_PACKET
        return;
    }
    mission = TaskMissionDefinition(&player->teamup->activetask->sahandle);
    if (!mission)
    {
        log_printf("storyerrs.log", "KeyClueSendAll - player %s(%i) has team with keyclues, but no mission",
            player->name, player->db_id);
        START_PACKET(pak, player, SERVER_KEYCLUE_UPDATE)
        pktSendBitsPack(pak, 1, 0);
        END_PACKET
        return;
    }

    // otherwise count and send
    for(clueIndex = BitFieldGetFirst(&player->teamup->keyclues, KEYCLUE_BITFIELD_SIZE);
        clueIndex != -1;
        clueIndex = BitFieldGetNext(&player->teamup->keyclues, KEYCLUE_BITFIELD_SIZE, clueIndex))
    {
        if (clueIndex >= eaSize(&mission->keyclues)) break; // screen out invalid clue bits
        clueCount++;
    }
    START_PACKET(pak, player, SERVER_KEYCLUE_UPDATE)
    pktSendBitsPack(pak, 1, clueCount);

    // set up vars and send, potential mismatch
    task = SAFE_MEMBER2(player, teamup, activetask);
    saBuildScriptVarsTable(&vars, player, SAFE_MEMBER(task, assignedDbId), player->teamup->mission_contact, task, NULL, NULL, NULL, NULL, 0, 0, true);
    
    for(clueIndex = BitFieldGetFirst(&player->teamup->keyclues, KEYCLUE_BITFIELD_SIZE);
        clueIndex != -1;
        clueIndex = BitFieldGetNext(&player->teamup->keyclues, KEYCLUE_BITFIELD_SIZE, clueIndex))
    {
        if (clueIndex >= eaSize(&mission->keyclues)) break; // screen out invalid clue bits
        ClueSend(mission->keyclues[clueIndex], &vars, pak, player);
    }
    ScriptVarsTableClear(&vars); // required by permanent saBuildScriptVarsTable
    END_PACKET
}

// *********************************************************************************
//  Mission Objective string
// *********************************************************************************

#define OBJECTIVE_LIST_SIZE    25
typedef struct ObjectiveStateDescription {
    char description[256]; // what this objective group is called
    char singulardescription[256]; // optional singular version
    int completed; // how many have been completed
    int total; // how many total
} ObjectiveStateDescription;

typedef struct ObjectiveStateList {
    ObjectiveStateDescription states[OBJECTIVE_LIST_SIZE]; // that's all I'm counting up
} ObjectiveStateList;

// given a descriptive name of an objective, fit it into the state list
static int PlaceObjectiveState(ObjectiveStateList* list, const char* description, const char* singulardescription)
{
    int i;
    for (i = 0; i < OBJECTIVE_LIST_SIZE; i++)
    {
        if (!stricmp(list->states[i].description, description))
            return i;
        if (!list->states[i].description[0])
        {
            strcpy(list->states[i].description, description);
            if (singulardescription)
                strcpy(list->states[i].singulardescription, singulardescription);
            return i;
        }
    }
    return -1;
}

// run through every objective known, sorting them out by description
// and counting totals for each description string
void ConstructObjectiveStateList(ObjectiveStateList* list)
{
    int objCursor;
    int iSize = eaSize(&objectiveInfoList);

    // Process all mission objectives of the specified name.
    for(objCursor = 0; objCursor < iSize; objCursor++)
    {
        MissionObjectiveInfo* obj = objectiveInfoList[objCursor];
        int pos;
        const char *description, *singulardescription;

        // Side mission objectives dont add to the status
        if (obj->optional)
            continue;

        // Requirements must be fulfilled before objectives can be shown
        if (obj->def && obj->def->descRequires && !(MissionObjectiveExpressionEval(obj->def->descRequires)))
            continue;

        // checking for overrides in the objective itself
        if (obj->description && obj->description[0])
        {
            description = obj->description;
        } 
        else 
        {
            if (!obj->def || !obj->def->description || !obj->def->description[0]) 
                continue;

            description = obj->def->description;
        }
        if (obj->singulardescription && obj->singulardescription[0])
        {
            singulardescription = obj->singulardescription;
        } 
        else
        {
            singulardescription = obj->def->singulardescription;
        }

        pos = PlaceObjectiveState(list, description, singulardescription);

        if (pos < 0) 
            continue;

        list->states[pos].total++;

        if (obj->state == MOS_SUCCEEDED || obj->state == MOS_FAILED) 
            list->states[pos].completed++;
    }
}

// get a string describing state of all objectives: "3 hostages remaining, 2 bombs left"
static const char* ObjectiveStateString(ScriptVarsTable* vars, Entity * player)
{
    ObjectiveStateList list;
    static char result[1000];
    int i;
    int first = 1;

    if (g_overridestatus[0]) 
        return saUtilScriptLocalize(g_overridestatus);
    if (!g_activemission || !objectiveInfoList) 
        return "";
    memset(&list, 0, sizeof(ObjectiveStateList));
    ConstructObjectiveStateList(&list);
    result[0] = 0;
    for (i = 0; i < OBJECTIVE_LIST_SIZE; i++)
    {
        int remain = list.states[i].total - list.states[i].completed;

        if (!list.states[i].description[0]) 
            break;
        if (remain == 0) 
            continue;    // don't show stuff completed at all

        if (!first)
            strcat(result, localizedPrintf(0,"MissionObjectiveSeparator"));
        else
            first = 0;

        if (remain == 1 && list.states[i].singulardescription[0])
            strcat(result, saUtilTableAndEntityLocalize(list.states[i].singulardescription, vars, player));
        else
        {
            strcatf(result, "%i %s", remain, saUtilTableAndEntityLocalize(list.states[i].description, vars, player));
        }
    }

    if (i == 0) 
        return ""; // no objectives

    return result;
}

// create a string to describe the current state of the mission
void MissionConstructStatus(char* result, Entity* player, StoryTaskInfo* info)
{
    const char* str = "";
    char construct[2000] = "";

    if(info->state == TASK_FAILED)
    {
        if(info->def)
            str = saUtilScriptLocalize(info->def->taskFailed_description);
        else
            str = 0;
        if (!str) 
            str = saUtilLocalize(player,"MissionFailedDefault");
    }
    else if (info->state == TASK_SUCCEEDED)
    {
        if(info->def)
            str = saUtilScriptLocalize(info->def->taskComplete_description);
        else
            str = 0;
        if (!str) 
            str = saUtilLocalize(player,"MissionCompleteDefault");
    }
    else if(OnMissionMap() && g_activemission && isSameStoryTaskPos( &g_activemission->sahandle, &info->sahandle ))
    {
        // Did someone ask for the status of the running mission?
        strcat(construct, ObjectiveStateString(&g_activemission->vars, player));
        str = construct;
    }

    // make sure we can fit in 256
    strncpy(result, str, 255);
    result[255] = 0;
}

// *********************************************************************************
//  Mission Sending functions - weird versions of contact send and task send
// *********************************************************************************

// player should be on a team, and needs to get mission status refreshed
void SendMissionStatus(Entity* e)
{
    char missionStatus[1000];
    ScriptVarsTable vars = {0};
    const StoryTaskInfo *task;
    StoryLocation *location;
    int haveloc = 0;

    // When the player is on the mission mapserver, the info comes out of the mission server's g_activemission field.
    if(OnMissionMap() && g_activemission)
    {
        MissionInfo* m = g_activemission;
        if(!m || !m->task)
            return;

        START_PACKET(pak, e, SERVER_TASK_STATUS);
        pktSendBitsPack(pak, 1, 1);            // Sending 1 task update set
        pktSendBitsPack(pak, 1, e->db_id);    // Sending task update set for this player.
        pktSendBits(pak, 1, 1);                // This is part of the client's taskset
        pktSendBits(pak, 1, 0); // player's notoriety (unchanged)
        pktSendBitsPack(pak, 1, -1);        // we don't know how many contacts there are
        pktSendBitsPack(pak, 1, 1);            // we're updating one record
        pktSendBitsPack(pak, 1, 0);            // the zero'th task is always the mission

        pktSendBits(pak, 1, server_state.clickToSource);
        if (server_state.clickToSource)
            pktSendString(pak, saUtilDirtyFileName(m->task->filename)); // filename
        pktSendString(pak, saUtilTableAndEntityLocalize(m->task->inprogress_description, &g_activemission->vars, e)); // description
        if (TASK_IS_SGMISSION(m->task))
        {
            pktSendString(pak, dbSupergroupNameFromId(m->supergroupDbid));        // owner
            pktSendBitsPack(pak, 1, m->supergroupDbid);                        // task UID
        }
        else
        {
            pktSendString(pak, dbPlayerNameFromId(m->ownerDbid));
            pktSendBitsPack(pak, 1, m->ownerDbid);
        }
        pktSendBitsPack(pak, 1, m->sahandle.context);
        pktSendBitsPack(pak, 1, m->sahandle.subhandle);
        pktSendBitsPack(pak, 1, m->missionLevel);

        pktSendBitsAuto(pak, m->difficulty.levelAdjust);
        pktSendBitsAuto(pak, m->difficulty.teamSize);
        pktSendBits(pak, 1, m->difficulty.dontReduceBoss);
        pktSendBits(pak, 1, m->difficulty.alwaysAV);

        {
            const StoryArc* sa = NULL;
            int alliance = SA_ALLIANCE_BOTH;
            StoryTaskInfo *pTask = NULL;
                
            sa = StoryArcDefinition(&m->sahandle);
            if (sa != NULL)
                alliance = sa->alliance;
            else if (e->teamup && e->teamup->activetask)
            {
                alliance = storyTaskInfoGetAlliance(e->teamup->activetask);
            } else {
                alliance = m->task->alliance;
            }
            pktSendBitsPack(pak, 2, alliance);
        }



        {
            StoryTaskInfo info = {0};
            StoryHandleCopy( &info.sahandle, &g_activemission->sahandle );
            info.state = g_activemission->completed ? TASK_SUCCEEDED : TASK_ASSIGNED;
            info.def = g_activemission->task;

            MissionConstructStatus(missionStatus, e, &info);
            pktSendString(pak, missionStatus);                    // current status, task->state
        }

        pktSendBitsPack(pak, 1, m->timeout);                // timeout, task->timeout
        pktSendBitsAuto(pak, m->timezero);
        pktSendBitsAuto(pak, m->timerType);
        pktSendBitsAuto(pak, m->failOnTimeout);
        pktSendBits(pak, 1, m->completed);                    // completed, task->isComplete
        pktSendBits(pak, 1, 1);                                // is mission
        pktSendBits(pak, 1, 1);                                // Does the task have an associated mission map running? task->hasRunningMission
        pktSendBits(pak, 1, !(!TASK_IS_SGMISSION(m->task)));
        pktSendBits(pak, 1, MissionIsZoneTransfer(g_activemission->def));    // is zone-transfer mission
        pktSendBits(pak, 1, !(g_activemission->def->flags & MISSION_NOTELEPORTONCOMPLETE));  // can teleport on complete
        pktSendBits(pak, 1, !(!TASK_FORCE_TIMELIMIT(m->task)));            // Task enforces time limit even after completion
        pktSendBits(pak, 1, 0);                                // Active tasks cannot be abandoned while on the mission map
        pktSendBits(pak, 1, 0);                                // isZowieTask
        pktSendBits(pak, 1, 0);                                // have destination, task->hasLocation
        pktSendBits(pak, 1, 0);                                // don't have another map where the destination is
        END_PACKET
    }

    // Otherwise, the mission comes out of the entity's teamup info.
    else if(e && e->teamup && e->teamup->activetask && e->teamup->activetask->def) // ab: quick hack to help designers. why is def NULL?
    {
        START_PACKET(pak, e, SERVER_TASK_STATUS);
        pktSendBitsPack(pak, 1, 1);            // Sending 1 task update set
        pktSendBitsPack(pak, 1, e->db_id);    // Sending task update set for this player.
        pktSendBits(pak, 1, 1);                // This is part of the client's taskset
        pktSendBits(pak, 1, 0); //  notoriety (unchanged)
        pktSendBitsPack(pak, 1, -1);        // we don't know how many contacts there are
        pktSendBitsPack(pak, 1, 1);            // we're updating one record
        pktSendBitsPack(pak, 1, 0);            // the zero'th contact is always the mission

        // Potential mismatch
        task = e->teamup->activetask;
        saBuildScriptVarsTable(&vars, e, task->assignedDbId, e->teamup->mission_contact, task, NULL, NULL, NULL, NULL, 0, 0, true);

        pktSendBits(pak, 1, server_state.clickToSource);
        if (server_state.clickToSource)
            pktSendString(pak, saUtilDirtyFileName(e->teamup->activetask->def->filename)); // filename
        pktSendString(pak, saUtilTableAndEntityLocalize(e->teamup->activetask->def->inprogress_description, &vars, e)); // description
        if (TASK_IS_SGMISSION(e->teamup->activetask->def))
            pktSendString(pak, dbSupergroupNameFromId(e->teamup->activetask->assignedDbId));    // owner
        else
            pktSendString(pak, dbPlayerNameFromId(e->teamup->activetask->assignedDbId));
        pktSendBitsPack(pak, 1, e->teamup->activetask->assignedDbId);                    // task UID
        pktSendBitsPack(pak, 1, e->teamup->activetask->sahandle.context);
        pktSendBitsPack(pak, 1, e->teamup->activetask->sahandle.subhandle);
        pktSendBitsPack(pak, 1, e->teamup->activetask->level);

        pktSendBitsAuto(pak,e->teamup->activetask->difficulty.levelAdjust);
        pktSendBitsAuto(pak, e->teamup->activetask->difficulty.teamSize);
        pktSendBits(pak, 1, e->teamup->activetask->difficulty.dontReduceBoss);
        pktSendBits(pak, 1, e->teamup->activetask->difficulty.alwaysAV);

        pktSendBitsPack(pak, 2, storyTaskInfoGetAlliance(e->teamup->activetask));
        pktSendString(pak, e->teamup->mission_status);                            // current status

        pktSendBitsPack(pak, 1, e->teamup->activetask->timeout);                // timeout
        pktSendBitsAuto(pak, e->teamup->activetask->timezero);
        pktSendBitsAuto(pak, e->teamup->activetask->timerType);
        pktSendBitsAuto(pak, e->teamup->activetask->failOnTimeout);
        pktSendBits(pak, 1, TaskCompleted(e->teamup->activetask));                // completed
        pktSendBits(pak, 1, TaskContainsMission(&e->teamup->activetask->sahandle)? 1: 0);    // is mission
        pktSendBits(pak, 1, e->teamup->map_id ? 1 : 0);                        // Does the task have an associated mission map running?
        pktSendBits(pak, 1, !(!TASK_IS_SGMISSION(e->teamup->activetask->def)));
        pktSendBits(pak, 1, TaskIsZoneTransfer(e->teamup->activetask));    // is this a zone-transfer mission
        pktSendBits(pak, 1, TaskCanTeleportOnComplete(e->teamup->activetask));    // teleport on complete
        pktSendBits(pak, 1, !(!TASK_FORCE_TIMELIMIT(e->teamup->activetask->def)));// Task enforces time limit even after completion
        pktSendBits(pak, 1, e->teamup->activetask->assignedDbId == e->db_id && TaskCanBeAbandoned(e, e->teamup->activetask));
        pktSendBits(pak, 1, TaskIsZowie(e->teamup->activetask));


        // get the location for the task
        location = ContactTaskGetLocations(e, e->teamup->activetask, &haveloc);
        // this code assumes there's only one location, even though multiple could be returned
        if (haveloc)
        {
            pktSendBits(pak, 1, 1);                                                // have location
            pktSendF32(pak, location->coord[0]);                                    // coords
            pktSendF32(pak, location->coord[1]);
            pktSendF32(pak, location->coord[2]);
            pktSendBitsAuto(pak, location->type);
            if (db_state.local_server)
                pktSendString(pak, saUtilCurrentMapFile());
            else
                pktSendString(pak, dbGetMapName(location->mapID));                    // map
            pktSendString(pak, saUtilTableAndEntityLocalize(e->teamup->activetask->def->inprogress_description, &vars, e)); // location description
        }
        else
        {
            pktSendBits(pak, 1, 0);                                                // no location

            if (e->teamup->activetask && e->teamup->activetask->def
                && e->teamup->activetask->def->taskLocationMap)
            {
                pktSendBits(pak, 1, 1);
                pktSendBitsAuto(pak, CONTACTDEST_EXPLICITTASK);
                pktSendString(pak, e->teamup->activetask->def->taskLocationMap);
            }
            else
            {
                pktSendBits(pak, 1, 0);
            }
        }

        if (TaskIsZowie(e->teamup->activetask))
        {
            char baseName[MAX_PATH];
            int map_id;

            pktSendBitsPack(pak, 1, e->teamup->activetask->completeSideObjectives);

            pktSendBits(pak, 32, e->teamup->activetask->seed);

            saUtilFilePrefix(baseName, e->teamup->activetask->def->villainMap);
            strcat_s(baseName, sizeof(baseName), ".txt");
            map_id = dbGetMapId(baseName);
            pktSendBitsPack(pak, 1, map_id);

            pktSendString(pak, e->teamup->activetask->def->zowieType);                        // zowie type
            pktSendString(pak, saUtilTableAndEntityLocalize(e->teamup->activetask->def->zowieDisplayName, &vars, e)); // zowie name
            pktSendString(pak, e->teamup->activetask->def->zowiePoolSize);                    // pool sizes
        }

        END_PACKET
    }
    ScriptVarsTableClear(&vars); // required by permanent saBuildScriptVarsTable
}

// player has left team or mission and needs mission status cleared
void ClearMissionStatus(Entity* e)
{
    START_PACKET(pak, e, SERVER_TASK_STATUS);

    pktSendBitsPack(pak, 1, 1);            // Sending 1 task update set.
    pktSendBitsPack(pak, 1, e->db_id);    // Sending task update set for the specified entity.
    pktSendBits(pak, 1, 1);                // This is part of the client's taskset
    pktSendBits(pak, 1, 0); // player's notoriety (unchanged)
    pktSendBitsPack(pak, 1, -1);        // we don't know how many contacts there are
    pktSendBitsPack(pak, 1, 1);    // we're updating one record
    pktSendBitsPack(pak, 1, 0); // the zero'th contact is always the mission
    pktSendBits(pak, 1, 0);        // no filename coming
    pktSendString(pak, "");        // description
    pktSendString(pak, "");        // owner
    pktSendBitsPack(pak, 1, 0);        // task UID
    pktSendBitsPack(pak, 1, 0);        // task UID
    pktSendBitsPack(pak, 1, 0);        // task UID
    pktSendBitsPack(pak, 1, 0);        // task level

    pktSendBitsAuto(pak, 0);    // difficulty level
    pktSendBitsAuto(pak, 0);    // difficulty teamsize
    pktSendBits(pak, 1, 0);        // difficulty boss
    pktSendBits(pak, 1, 0);        // difficulty av

    pktSendBitsPack(pak, 2, 0);        // alliance
    pktSendString(pak, "");        // status
    pktSendBitsPack(pak, 1, 0); // timeout
    pktSendBitsAuto(pak, 0);    // timezero
    pktSendBitsAuto(pak, 0);    // timerType
    pktSendBitsAuto(pak, 0);    // failOnTimeout
    pktSendBits(pak, 1, 0);        // complete
    pktSendBits(pak, 1, 0);        // mission
    pktSendBits(pak, 1, 0);        // task has mission running?
    pktSendBits(pak, 1, 0);        // task is sg mission?
    pktSendBits(pak, 1, 0);        // zone transfer mission
    pktSendBits(pak, 1, 0);        // teleport on complete
    pktSendBits(pak, 1, 0);        // task enforces time limit
    pktSendBits(pak, 1, 0);        // task cannot be abandoned
    pktSendBits(pak, 1, 0);        // isZowieTask
    pktSendBits(pak, 1, 0);        // destination
    pktSendBits(pak, 1, 0);        // other map for destination
    END_PACKET
}

// *********************************************************************************
//  Visit locations
// *********************************************************************************

void ContactVisitLocationSendAll(Entity* e)
{
    int i;
    int visitLocationCount = 0;
    StoryInfo* info;
    int iSize;
    if(!e)
        return;

    info = StoryInfoFromPlayer(e);

    iSize = eaSize(&info->tasks);
    // Count how many incomplete visit location tasks there are.
    for(i = 0; i < iSize; i++)
    {
        StoryTaskInfo* task = info->tasks[i];
        if(!TaskCompleted(task) && (task->def->type == TASK_VISITLOCATION))
            visitLocationCount++;
    }

    START_PACKET(pak, e, SERVER_TASK_VISITLOCATION_UPDATE)
        // How many visit location group names are we sending?
        pktSendBitsPack(pak, 1, visitLocationCount);

    // Send individual visit location info.
    for(i = 0; i < iSize; i++)
    {
        StoryTaskInfo* task = info->tasks[i];
        if(!TaskCompleted(task) && (task->def->type == TASK_VISITLOCATION))
        {
            pktSendString(pak, LocationTaskNextLocation(task));
        }
    }
    END_PACKET

        // client has been brought up to date with the current state of all contacts.
    e->storyInfo->visitLocationChanged = 0;
}

void ContactVisitLocationSendChanged(Entity* e)
{
    if(e && e->storyInfo && e->storyInfo->visitLocationChanged)
    {
        ContactVisitLocationSendAll(e);
    }
}

// *********************************************************************************
//  Task detail page
// *********************************************************************************

bool TaskSendDetail(Entity* player, int targetDbid, StoryTaskHandle *sahandle)
{
    Entity* targetEntity;
    StoryInfo* info;
    char response[LEN_CONTACT_DIALOG];
    char intro[LEN_MISSION_INTRO];
    int ok = 1;
    int i, n;
    response[0] = 0;
    intro[0] = 0;

    // special case if this is the active team task
    if (player->teamup && player->teamup->activetask->assignedDbId == targetDbid
        && isSameStoryTask(&player->teamup->activetask->sahandle, sahandle) && player->teamup->activetask->def)
    {
        TaskGenerateDetail(targetDbid, player, player->teamup->mission_contact, 
            player->teamup->activetask, response, intro);
    }
    else
    {
        targetEntity = entFromDbId(targetDbid);
        if (TaskIsSGMissionEx(player, sahandle))
            TaskGenerateDetail(targetDbid, player, sahandle->context, player->supergroup->activetask, response, intro);
        else if(!targetEntity || (player->teamup_id && !team_IsMember(player, targetDbid)))
            ok = 0; // don't allow info for non team members
        else
        {
            info = StoryInfoFromPlayer(targetEntity);
            n = eaSize(&info->tasks);
            for (i = 0; i < n; i++)
            {
                if ( isSameStoryTask( &info->tasks[i]->sahandle, sahandle) )
                    break;
            }

            if (i < n)
            {
                StoryTaskInfo* task = info->tasks[i];
                ContactHandle contact = TaskGetContactHandle(info, task);
                TaskGenerateDetail(targetDbid, targetEntity, contact, task, response, intro);
            }
            else
                ok = 0;
        }
    }

    if(!ok)// received an invalid index, but don't leave client hanging
    {
        strcpy(response, "Unable to obtain task detail information");
    }

    START_PACKET(pak, player, SERVER_TASK_DETAIL)
        pktSendBitsPack(pak, 1, targetDbid);
        pktSendBitsPack(pak, 1, sahandle->context);
        pktSendBitsPack(pak, 1, sahandle->subhandle);
        pktSendString(pak, response);
        pktSendString(pak, intro);
    END_PACKET

    return ok;
}


