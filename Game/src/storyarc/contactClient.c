#include <utilitieslib/utils/utils.h>
#include "storyarc/contactClient.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/utils/error.h>
#include "UI/uiCompass.h"
#include "UI/uiConsole.h"
#include "UI/uiAutomap.h"
#include "UI/uiChat.h"
#include "UI/uiContact.h"
#include "UI/uiMission.h"
#include "player/player.h"
#include "group/groupnetrecv.h"
#include "UI/uiDialog.h"
#include "clientcomm/clientcomm.h"
#include "cmdparse/cmdgame.h"
#include "entity/entPlayer.h"
#include "UI/uiMissionSummary.h"
#include "UI/uiCompass.h"
#include "UI/uiUtil.h"
#include "storyarc/storyarcCommon.h"
#include "storyarc/zowieClient.h"
#include "entity/entity.h"
#include "graphics/textureatlas.h"
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/uiAutomap.h"

ContactStatus** contacts = 0;
ContactStatus** contactsAccessibleNow = 0;
int contactsDirty = false;
U32 timeWhenCanEarnAlignmentPoint[ALIGNMENT_POINT_MAX_COUNT] = {0, 0, 0, 0, 0};
U32 earnedAlignments[ALIGNMENT_POINT_MAX_COUNT] = {0, 0, 0, 0, 0};
U32 heroAlignmentPoints_self = 0;
U32 vigilanteAlignmentPoints_self = 0;
U32 villainAlignmentPoints_self = 0;
U32 rogueAlignmentPoints_self = 0;
U32 heroAlignmentPoints_other = 0;
U32 vigilanteAlignmentPoints_other = 0;
U32 villainAlignmentPoints_other = 0;
U32 rogueAlignmentPoints_other = 0;
U32 lastTimeWasSaved = 0;
U32 timePlayedAsCurrentAlignmentAsOfTimeWasLastSaved = 0;
U32 lastTimeAlignmentChanged = 0;
U32 countAlignmentChanged = 0;
U32 alignmentMissionsCompleted_Paragon = 0;
U32 alignmentMissionsCompleted_Hero = 0;
U32 alignmentMissionsCompleted_Vigilante = 0;
U32 alignmentMissionsCompleted_Rogue = 0;
U32 alignmentMissionsCompleted_Villain = 0;
U32 alignmentMissionsCompleted_Tyrant = 0;
U32 alignmentTipsDismissed = 0;
U32 switchMoralityMissionsCompleted_Paragon = 0;
U32 switchMoralityMissionsCompleted_Hero = 0;
U32 switchMoralityMissionsCompleted_Vigilante = 0;
U32 switchMoralityMissionsCompleted_Rogue = 0;
U32 switchMoralityMissionsCompleted_Villain = 0;
U32 switchMoralityMissionsCompleted_Tyrant = 0;
U32 reinforceMoralityMissionsCompleted_Paragon = 0;
U32 reinforceMoralityMissionsCompleted_Hero = 0;
U32 reinforceMoralityMissionsCompleted_Vigilante = 0;
U32 reinforceMoralityMissionsCompleted_Rogue = 0;
U32 reinforceMoralityMissionsCompleted_Villain = 0;
U32 reinforceMoralityMissionsCompleted_Tyrant = 0;
U32 moralityTipsDismissed = 0;

char** visitLocations;

//------------------------------------------------------------------------
// Task Status Set management
//------------------------------------------------------------------------


TaskStatusSet** teamTaskSets = 0;    // Task set for all team members including the player.

static TaskStatusSet* TaskStatusSetCreate();
static TaskStatus*** TaskStatusListGetOrAdd(int dbid);
static void TaskStatusSetDestroy(TaskStatusSet* set);
static TaskStatus*** TaskStatusListAdd(int dbid);
static TaskStatus*** TaskStatusListGet(int dbid);
static void TaskProcessZowieGlowFlags(int isAdd);
static void TaskStatusSetDifficulty(int dbid, int levelAdjust, int teamsize, int boss, int av);
static void TaskStatusListRemove(int dbid);
static void TaskStatusSetsSort();

//------------------------------------------------------------------------
// Contact Interaction
//------------------------------------------------------------------------
ContactDialogContext dialogContext;

ContactResponseOption* ContactResponseOptionCreate()
{
    return calloc(1, sizeof(ContactResponseOption));
}

void ContactResponseOptionDestroy(ContactResponseOption* option)
{
    free(option);
}

// TODO - It would be nice if the contact response structure on the client side is dynamic.
// TODO - Make sure ContactResponseReceive() works.
void ContactResponseReceive(ContactResponse* response, Packet* pak)
{
    char* str;
    int responseCount;
    int i;

    response->type = pktGetBitsPack(pak, 1);
    str = pktGetString(pak);
    strcpy(response->dialog, str);

    response->responsesFilled = responseCount = pktGetBitsPack(pak, 1);
    for(i = 0; i < responseCount; i++)
    {
        ContactResponseOption* r = &response->responses[i];
        r->link = pktGetBitsPack(pak, 1);
        r->rightLink = pktGetBitsAuto(pak);
        str = pktGetString(pak);
        strcpy(r->responsetext, str);
        str = pktGetString(pak);
        strcpy(r->rightResponseText, str);
    }
}


//------------------------------------------------------------------------
// Contact Status info
//------------------------------------------------------------------------

void ContactStatusClear(ContactStatus* status);
void ContactStatusReceive(ContactStatus* status, int index, Packet* pak);

ContactStatus* ContactStatusCreate()
{
    return calloc(1, sizeof(ContactStatus));
}

void ContactStatusDestroy(ContactStatus* status)
{
    ContactStatusClear(status);
    free(status);
}

// receive a contact status update from server-
//    mission status is always held in contact 0
void ContactStatusListReceive(Packet* pak)
{
    int contactCount;
    int contactIndex;
    int deleteCount;
    int deleteIndex;
    int desiredSize;
    int contactSize;
    int pointIndex;
    U32 now = timerServerTime();

    // init global list if necessary
    if (!contacts)
    {
        eaCreate(&contacts);
        eaPush(&contacts, ContactStatusCreate()); // always must have mission status
    }

    for (pointIndex = 0; pointIndex < ALIGNMENT_POINT_MAX_COUNT; pointIndex++)
    {
        timeWhenCanEarnAlignmentPoint[pointIndex] = now + pktGetBitsAuto(pak);
        earnedAlignments[pointIndex] = pktGetBitsAuto(pak);
    }
    heroAlignmentPoints_self = pktGetBitsAuto(pak);
    vigilanteAlignmentPoints_self = pktGetBitsAuto(pak);
    villainAlignmentPoints_self = pktGetBitsAuto(pak);
    rogueAlignmentPoints_self = pktGetBitsAuto(pak);

    // How many contacts are there, NOT including the mission info?
    //        -1 can be passed in here, and the contact list size will not be adjusted
    contactSize = pktGetBitsAuto(pak);
    if (contactSize >= 0)
        desiredSize = contactSize+1;
    else
        desiredSize = eaSize(&contacts);

    // Handle any manual deletions first (used in change-only updates)
    deleteCount = pktGetBitsAuto(pak);

    for (deleteIndex = 0; deleteIndex < deleteCount; deleteIndex++)
    {
        int deleteHandle = pktGetBitsAuto(pak);
        for (contactIndex = 0; contactIndex < eaSize(&contacts); contactIndex++)
        {
            if (contacts[contactIndex]->handle == deleteHandle)
            {
                ContactStatusDestroy(eaRemove(&contacts, contactIndex));
                break;
            }
        }
    }

    // Handle automatic deletions next (used in full updates)
    while (eaSize(&contacts) > desiredSize)
    {
        ContactStatusDestroy(eaPop(&contacts));
    }
    while (eaSize(&contacts) < desiredSize)
    {
        eaPush(&contacts, ContactStatusCreate());
    }

    // How many incoming contact status updates are there?
    contactCount = pktGetBitsAuto(pak);
    for(contactIndex = 0; contactIndex < contactCount; contactIndex++)
    {
        ContactStatus* status;
        int contactNumber;

        //    Which contact is being updated?
        contactNumber = pktGetBitsAuto(pak);
        status = contacts[contactNumber];

        //    Unpack contact status.
        ContactStatusReceive(status, contactNumber, pak);
    }

    TaskUpdateDestination(&waypointDest);
    TaskUpdateDestination(&activeTaskDest);
#if !TEST_CLIENT
    contactReparse();
#endif
}

// Always does a full send
static void ContactStatusDestroyAdapter(void* arg0)
{
    ContactStatusDestroy((ContactStatus*)arg0);
}

void ContactStatusAccessibleListReceive(Packet *pak)
{
    int contactIndex, contactCount;

    if (!contactsAccessibleNow)
    {
        eaCreate(&contactsAccessibleNow);
    }
    else
    {
        eaClearEx(&contactsAccessibleNow, ContactStatusDestroyAdapter);
    }

    // How many incoming contact status updates are there?
    contactCount = pktGetBitsAuto(pak);
    for(contactIndex = 0; contactIndex < contactCount; contactIndex++)
    {
        ContactStatus* status = ContactStatusCreate();

        //    Unpack contact status.
        ContactStatusReceive(status, -1, pak);

        eaPush(&contactsAccessibleNow, status);
    }
}

void ContactStatusClear(ContactStatus* status)
{
    if (status->name) 
        free(status->name);
    if (status->locationDescription) 
        free(status->locationDescription);
    if (status->filename) 
        free(status->filename);
    memset(status, 0, sizeof(ContactStatus));
}

void assignContactIcon(ContactStatus *status)
{
    if (!status)
        return;

    if (status->hasTask)
    {
        status->location.icon = atlasLoadTexture("map_mission_available.tga");
        status->location.type = ICON_CONTACT_HAS_NEW_MISSION;
    }
    else if (status->notifyPlayer)
    {
        status->location.icon = atlasLoadTexture("map_mission_completed.tga");
        status->location.type = ICON_CONTACT_COMPLETED_MISSION;
    }
    else if (status->taskcontext)
    {
        status->location.icon = atlasLoadTexture("map_mission_in_progress.tga");
        status->location.type = ICON_CONTACT_ON_MISSION;
    }
    else
    {
        status->location.icon = atlasLoadTexture("map_enticon_contact.tga");
        status->location.type = ICON_CONTACT;
    }
}

void ContactStatusReceive(ContactStatus* status, int index, Packet* pak)
{
    int overheadState = status->currentOverheadIconState;

    ContactStatusClear(status);
    status->currentOverheadIconState = overheadState;

    if (pktGetBits(pak, 1))
        status->filename = strdup(pktGetString(pak));
    status->name = strdup(pktGetString(pak));
    status->locationDescription = strdup(pktGetString(pak));

    status->callOverride = (pktGetBits(pak, 1))?strdup(pktGetString(pak)):NULL;
    status->askAboutOverride = (pktGetBits(pak, 1))?strdup(pktGetString(pak)):NULL;
    status->leaveOverride = (pktGetBits(pak, 1))?strdup(pktGetString(pak)):NULL;
    status->imageOverride = (pktGetBits(pak, 1))?strdup(pktGetString(pak)):NULL;

    status->npcNum = pktGetBitsAuto(pak);
    status->handle = pktGetBitsAuto(pak);
    status->taskcontext = pktGetBitsAuto(pak);
    status->tasksubhandle = pktGetBitsAuto(pak);
    //    status->timeAutoRemokes = pktGetBitsAuto(pak);//+ SecondsSince2000();

    status->notifyPlayer = pktGetBits(pak, 1);
    status->hasTask = pktGetBits(pak, 1);
    status->canUseCell = pktGetBits(pak, 1);
    status->isNewspaper = pktGetBits(pak, 1);
    status->onStoryArc = pktGetBits(pak, 1);
    status->onMiniArc = pktGetBits(pak, 1);
    status->tipType = pktGetBitsPack(pak, 1);
    status->wontInteract = pktGetBits(pak, 1);
    status->metaContact = pktGetBits(pak, 1);

    status->currentCP = pktGetBitsAuto(pak);
    status->friendCP = pktGetBitsAuto(pak);
    status->confidantCP = pktGetBitsAuto(pak);
    status->completeCP = pktGetBitsAuto(pak);
    status->heistCP = pktGetBitsAuto(pak);
    if (status->completeCP && (status->currentCP > status->completeCP))
        status->currentCP = status->completeCP; // just clamp this for display
    else if (!status->completeCP && status->currentCP > status->heistCP) 
        status->currentCP = status->heistCP; // just clamp this for display

    status->alliance = pktGetBitsAuto(pak);

    status->hasLocation = pktGetBits(pak, 1);

    if (status->hasLocation)
    {
        int temp;

        assignContactIcon(status);
        status->location.handle = status->handle;

        status->location.destPNPC = 0;
        if (pktGetBits(pak, 1))
        {
            char pnpcName[500];
            strcpy(pnpcName, pktGetString(pak));
            status->location.destPNPC = PNPCFindDef(pnpcName);
        }

        status->location.destEntity = 0;
        if (pktGetBits(pak, 1))
        {
            temp = pktGetBitsAuto(pak);
            if (temp)
            {
                int i;

                for (i = 1; i < entities_max; i++)
                {
                    if (entities[i]->id == temp)
                    {
                        status->location.destEntity = entities[i];
                        break;
                    }
                }
            }
        }

        status->location.world_location[3][0] = pktGetF32(pak);
        status->location.world_location[3][1] = pktGetF32(pak);
        status->location.world_location[3][2] = pktGetF32(pak);
        strcpy(status->location.mapName, pktGetString(pak));
        strcpy(status->location.name, pktGetString(pak));
        if (index != -1)
        {
            status->location.id = CONTACT_LOCATIONS + index;
        }
    }
    else if (index != -1) // we may have lost this location, make sure we don't have waypoint
    {
        memset(&status->location, 0, sizeof(status->location));
        compass_ClearAllMatchingId(CONTACT_LOCATIONS + index);
    }

    contactsDirty = true;
}

ContactStatus* ContactStatusGet(int handle)
{
    int i, n;

    if (!contacts) 
        return NULL;
    n = eaSize(&contacts);
    for (i = 0; i < n; i++)
    {
        if (contacts[i]->handle == handle)
            return contacts[i];
    }
    return NULL;
}

//------------------------------------------------------------------------
// Task Status info
//------------------------------------------------------------------------

TaskStatus* TaskStatusCreate()
{
    return calloc(sizeof(TaskStatus), 1);
}

void TaskStatusDestroy(TaskStatus* task)
{
#ifndef TEST_CLIENT
    if (task->pSMFBlock)
    {
        smfBlock_Destroy(task->pSMFBlock);
    }
#endif
    // Let the cache in updateAutomapZowies(...) know that we just threw this task into the bit bucket
    updateAutomapZowies(task, DESTROY_CACHED);
    free(task);
}

// task list is maintained similarly to contact list - the 0th entry is reserved for the
// current mission.
void TaskStatusListReceive(Packet* pak)
{
    int taskCount;
    int desiredSize;
    int taskSize;
    int i;
    int taskSetCount, currentSet;
    TaskStatus*** taskList;
    TaskStatus* activetask;

    // Since tasks are stored in a 2D earray of earrays, it'd be a ton too much work to figure all the edge cases of tasks getting added,
    // removed, and changed.  So we take the safe approach and simply blow everything away here, receive the update, and then turn on all
    // glow flags at the bottom.
    TaskProcessZowieGlowFlags(0);

    taskSetCount = pktGetBitsPack(pak, 1);
    for(currentSet = 0; currentSet < taskSetCount; currentSet++)
    {
        int level = 256, teamsize, boss, av;
        int dbid = pktGetBitsPack(pak, 1);
        int forme = pktGetBits(pak, 1);

        if( pktGetBits(pak,1) )
        {
            level = pktGetBitsAuto(pak);
            teamsize = pktGetBitsAuto(pak);
            boss = pktGetBits(pak, 1);
            av = pktGetBits(pak, 1);
        }

        // How many tasks are there, NOT including the mission info?
        //        -1 can be passed in here, and the task list size will not be adjusted
        taskSize = pktGetBitsPack(pak, 1);

        // How many incoming task status updates are there?
        taskCount = pktGetBitsPack(pak, 1);

        // What kind of update is in this set?
        // Does it contain the player's own task list?
        taskList = TaskStatusListGetOrAdd(dbid);
        if(!eaSize(taskList))
        {
            eaPush(taskList, TaskStatusCreate()); // always must have mission status
        }

        if( level != 256)
            TaskStatusSetDifficulty(dbid, level, teamsize, boss, av);

        if (taskSize >= 0)
            desiredSize = taskSize+1;
        else
            desiredSize = eaSize(taskList);

        while (eaSize(taskList) > desiredSize)
        {
            TaskStatusDestroy(eaPop(taskList));
        }
        while (eaSize(taskList)< desiredSize)
        {
            eaPush(taskList, TaskStatusCreate());
        }

        for(i = 0; i < taskCount; i++)
        {
            TaskStatus* status;
            int taskIndex;

            //    Which task is being updated?
            taskIndex = pktGetBitsPack(pak, 1);
            status = (*taskList)[taskIndex];

            //    Unpack contact status.
            TaskStatusReceive(status, taskIndex, TaskStatusSetIndex(dbid), pak);
        }

        // Empty task list?
        if(eaSize(taskList) == 1 && !(*taskList)[0]->description[0])
        {
            TaskStatusListRemove(dbid);
        }
    }

    TaskStatusSetsSort();
    // Turn zowie glow flags on.  See comment at the top of this routine for details on what is going on.
    TaskProcessZowieGlowFlags(1);
    // Update automap zowies in case one got turned off during this update
    // DGNOTE 7/22/2010
    // The second parameter to updateAutomapZowies(...) determines if the first is to be cached or not.  Setting it to 0 causes this routine to
    // ignore the first parameter and use the cached copy instead.
    updateAutomapZowies(NULL, USE_CACHED);
    // need to auto-select active task whenever we get an update
    activetask = PlayerGetActiveTask();
    if (activetask)
    {
        compass_SetTaskDestination(&activeTaskDest, activetask);
    }
    else
    {
        clearDestination(&activeTaskDest);
    }

    TaskUpdateDestination(&waypointDest);
#if !TEST_CLIENT
    missionReparse();
#endif
}


void TaskStatusReceive(TaskStatus* task, int index, int listid, Packet* pak)
{
    if (pktGetBits(pak, 1))
        strcpy(task->filename, pktGetString(pak));
    strncpyt(task->description, pktGetString(pak), 256);
    strncpyt(task->owner, pktGetString(pak), MAX_NAME_LEN);

    task->ownerDbid = pktGetBitsPack(pak, 1);                        // task UID
    task->context = pktGetBitsPack(pak, 1);
    task->subhandle = pktGetBitsPack(pak, 1);
    task->level = pktGetBitsPack(pak, 1);


    task->difficulty.levelAdjust = pktGetBitsAuto(pak);
    task->difficulty.teamSize = pktGetBitsAuto(pak);
    task->difficulty.dontReduceBoss = pktGetBits(pak,1);
    task->difficulty.alwaysAV = pktGetBits(pak,1);

    task->alliance = pktGetBitsPack(pak, 2);

    strncpyt(task->state, pktGetString(pak), 256);

    task->timeout = pktGetBitsPack(pak, 1);
    task->timezero = pktGetBitsAuto(pak);
    task->timerType = pktGetBitsAuto(pak);
    task->failOnTimeout = pktGetBitsAuto(pak);
    task->isComplete = pktGetBits(pak, 1);
    task->isMission = pktGetBits(pak, 1);
    task->hasRunningMission = pktGetBits(pak, 1);
    task->isSGMission = pktGetBits(pak, 1);
    task->zoneTransfer = pktGetBits(pak, 1);
    task->teleportOnComplete = pktGetBits(pak, 1);
    task->enforceTimeLimit = pktGetBits(pak, 1);
    task->isAbandonable = pktGetBits(pak, 1);
    task->isZowie = pktGetBits(pak, 1);
    task->hasLocation = pktGetBits(pak, 1);
    if (task->hasLocation)
    {
        task->location.type = ICON_MISSION;
        task->location.icon = atlasLoadTexture("map_enticon_mission_a");
        task->location.iconB = atlasLoadTexture("map_enticon_mission_b");
        task->location.iconBInCompass = 1;
        task->location.dbid = task->ownerDbid;
        task->location.context = task->context;
        task->location.subhandle = task->subhandle;

        task->location.angle = PI/4;
        task->location.world_location[3][0] = pktGetF32(pak);
        task->location.world_location[3][1] = pktGetF32(pak);
        task->location.world_location[3][2] = pktGetF32(pak);
        task->location.contactType = pktGetBitsAuto(pak);
        strncpyt(task->location.mapName, pktGetString(pak), ARRAY_SIZE(task->location.mapName));
        strncpyt(task->location.name, pktGetString(pak), ARRAY_SIZE(task->location.name));
    }
    else // we may have lost this location, make sure we don't have waypoint
    {
        memset(&task->location, 0, sizeof(task->location));
        // don't clear if it's a zowie task, because we don't have legit data until after updateAutomapZowies().
        if (!task->isZowie) 
        {
            if (task->isSGMission && (index < 0))
                compass_ClearAllMatchingId(SG_LOCATION);
            else
                compass_ClearAllMatchingId(TASK_LOCATIONS + index + TASK_SET_LOCATIONS * listid);
        }

        // if we have a taskLocationMap on another map
         if (pktGetBits(pak, 1))
         {
            task->location.contactType = pktGetBitsAuto(pak);
             strncpyt(task->location.mapName, pktGetString(pak), ARRAY_SIZE(task->location.mapName));
         }
    }

    // always set the location id even if !hasLocation. This is needed for zowie tasks.
    if (task->isSGMission && (index < 0))
        task->location.id = SG_LOCATION;
    else
        task->location.id = TASK_LOCATIONS + index + TASK_SET_LOCATIONS * listid;

    if (task->isZowie)
    {
        int i;
        char *poolSize;

        task->zinfo = pktGetBitsPack(pak, 1);
        task->zseed = pktGetBits(pak, 32);
        task->zmap_id = pktGetBitsPack(pak, 1);
        strncpyt(task->ztype, pktGetString(pak), MAX_ZOWIE_NAME);
        strncpyt(task->zname, pktGetString(pak), MAX_ZOWIE_NAME);
        poolSize = pktGetString(pak);
        // Decomposition of the pool sizes string is much simpler here, since the mapserver has already vetted the data in zowie_setup(...) in zowie.c
        // Also, it is safe to hand the string returned from PktGetString directly to strtok.  Sure, strtok, damages the passed string, but this is
        // not a problem since it lives in a static buffer maintained by PktGetString, and can be safely treated as non-const.  it'll get totalled on
        // the next call to PktgetString anyways, so we really don't care.
        poolSize = strtok(poolSize, " ");
        i = 0;
        while (poolSize != NULL)
        {
            task->zpoolSize[i++] = atoi(poolSize);
            poolSize = strtok(NULL, " ");
        }
    }

    // invalidate the detail field whenever we get an update
    task->detail[0] = 0;
    task->detailInvalid = 1;
}

void TaskStatusListRemoveTeammates(Packet* pak)
{
    TaskStatusRemoveTeammates();
#if !TEST_CLIENT
    missionReparse();
#endif
} 

void TaskDetailReceive(Packet* pak)
{
    int dbid = pktGetBitsPack(pak, 1);
    int context = pktGetBitsPack(pak, 1);
    int handle = pktGetBitsPack(pak, 1);
    TaskStatus* task = TaskStatusGet(dbid, context, handle);
    
    // Read in the two strings either way
    if (!task)
    {
        pktGetString(pak);
        pktGetString(pak);
    }
    else
    {
        strncpyt(task->detail, pktGetString(pak), LEN_CONTACT_DIALOG);
        strncpyt(task->intro, pktGetString(pak), LEN_MISSION_INTRO);
        task->detailInvalid = 0;
    #if !TEST_CLIENT
        missionReparse();
    #endif
    }
}

//------------------------------------------------------------------------
// Other stuff
//------------------------------------------------------------------------

// get back active task if any from contact
TaskStatus* TaskStatusFromContact(ContactStatus* contact)
{
    Entity* player = playerPtr();
    TaskStatus** tasks = TaskStatusGetPlayerTasks();
    if (!player || !tasks || !contact || contact->taskcontext == 0) 
        return NULL;
    return TaskStatusGet(player->db_id, contact->taskcontext, contact->tasksubhandle);
}

// select a contact from the list
void ContactSelect(int index, int userSelected)
{
    int n;
    n = eaSize(&contacts);

    // Selecting a valid contact?
    if (index >= 0 && index < n)
    {
        ContactStatus* contact = contacts[index];

        compass_SetContactDestination(&waypointDest, contact);
    }
}

// select a contact from our list
void ContactSelectReceive(Packet* pak)
{
    int index = pktGetBitsAuto(pak);
    ContactSelect(index, 0);
}

// select a task from our list
void TaskSelectReceive(Packet* pak)
{
    int index = pktGetBitsAuto(pak);
    TaskStatus** tasks = TaskStatusGetPlayerTasks();
    int n = eaSize(&tasks);
    if (index >= 0 && index < n && tasks[index]->description[0])
        compass_SetTaskDestination(&waypointDest, tasks[index]);
}

// show all client contact info
void ContactListDebugPrint(void)
{
    int i, n;
    n = eaSize(&contacts);
    for (i = 0; i < n; i++) // for each contact
    {
        if (!contacts[i]->handle) 
            continue;
        conPrintf("%s (%i): %s %s\n", contacts[i]->name, contacts[i]->handle, contacts[i]->locationDescription,    contacts[i]->notifyPlayer? "(Notify)":"");
        if (contacts[i]->hasLocation)
        {
            Destination* location = &contacts[i]->location;
            char* mapName = strrchr(location->mapName, '/');
            if (mapName) 
                mapName++;
            else 
                mapName = location->mapName;
            conPrintf("   %s (%s: %0.2f,%0.2f)\n", location->name, mapName,
                location->world_location[3][0], location->world_location[3][2]);
        }
    }
}

static void TaskListDebugPrintInternal(TaskStatus** list)
{
    int i, n;

    if(!list)
        return;
    n = eaSize(&list);
    for (i = 0; i < n; i++) // for each task
    {
        if (!list[i]->description[0]) 
            continue;
        conPrintf("%i %s", i, list[i]->description);
        if (list[i]->state[0])
            conPrintf(" - %s", list[i]->state);
        if (list[i]->owner[0])
            conPrintf(" (belongs to %s)", list[i]->owner);
    }
}

// show all task info
void TaskListDebugPrint(void)
{
    int i;
    int iSize = eaSize(&teamTaskSets);
    Entity* player = playerPtr();

    conPrintf("Player tasks:\n");
    TaskListDebugPrintInternal(TaskStatusGetPlayerTasks());

    conPrintf("\nTeam tasks:\n");
    for(i = 0; i < iSize; i++)
    {
        if(teamTaskSets[i]->dbid != player->db_id)
            TaskListDebugPrintInternal(teamTaskSets[i]->list);
    }
    conPrintf("\n\n");
}

//------------------------------------------------------------------------
// Task Status Set management
//------------------------------------------------------------------------
static TaskStatusSet* TaskStatusSetCreate()
{
    return calloc(sizeof(TaskStatusSet), 1);
}

void TaskStatusDestroy(TaskStatus* task);
static void TaskStatusDestroyCallback(void* arg0)
{
    TaskStatusDestroy((TaskStatus*)arg0);
}

static void TaskStatusSetDestroy(TaskStatusSet* set)
{
    if(!set)
        return;

    if(set->list)
    {
        eaDestroyEx(&set->list, TaskStatusDestroyCallback);
    }

    free(set);
}

TaskStatusSet* TaskStatusSetGet(int dbid)
{
    int i;

    for(i = eaSize(&teamTaskSets)-1; i >= 0; i--)
    {
        TaskStatusSet* set = teamTaskSets[i];
        if(set->dbid == dbid)
            return teamTaskSets[i];
    }
    return NULL;
}

// as above, but we just want a unique index returned
int TaskStatusSetIndex(int dbid)
{
    int i;
    for(i = eaSize(&teamTaskSets)-1; i >= 0; i--)
    {
        TaskStatusSet* set = teamTaskSets[i];
        if(set->dbid == dbid)
            return i;
    }
    return 0;
}

TaskStatusSet* TaskStatusSetGetPlayer()
{
    TaskStatusSet* set;
    Entity* player = playerPtr();
    if (!player) 
        return NULL;
    set = TaskStatusSetGet(player->db_id);
    return set;
}

static int taskStatusSetCompare(const void* set1Data, const void* set2Data)
{
    const TaskStatusSet** set1 = (const TaskStatusSet**)set1Data;
    const TaskStatusSet** set2 = (const TaskStatusSet**)set2Data;
    Entity* player = playerPtr();
    Teamup* team;
    int i;

    if(!player || !player->teamup)
        return 0;

    team = player->teamup;
    for(i = 0; i < team->members.count; i++)
    {
        if(team->members.ids[i] == (*set1)->dbid)
            return -1;
        if(team->members.ids[i] == (*set2)->dbid)
            return 1;
    }

    return 0;
}

static void TaskStatusSetsSort()
{
    eaQSort(teamTaskSets, taskStatusSetCompare);
#if !TEST_CLIENT
    missionReparse();
#endif
}

static TaskStatus*** TaskStatusListAdd(int dbid)
{
    TaskStatusSet* set = TaskStatusSetCreate();
    set->dbid = dbid;
    eaPush(&teamTaskSets, set);
    return &set->list;
}

static TaskStatus*** TaskStatusListGet(int dbid)
{
    TaskStatusSet* set = TaskStatusSetGet(dbid);
    if(!set)
        return NULL;

    return &set->list;
}

static TaskStatus*** TaskStatusListGetOrAdd(int dbid)
{
    TaskStatus*** list;
    list = TaskStatusListGet(dbid);
    if(list)
        return list;

    return TaskStatusListAdd(dbid);
}

static void TaskStatusListRemove(int dbid)
{
    int i;

    for(i = eaSize(&teamTaskSets)-1; i >= 0; i--)
    {
        TaskStatusSet* set = teamTaskSets[i];
        if(set->dbid == dbid)
        {
            eaRemove(&teamTaskSets, i);
            TaskStatusSetDestroy(set);
        }
    }
}

// Turn zowie glow flags on or off
static void TaskProcessZowieGlowFlags(int isAdd)
{
    int i;
    int j;
    TaskStatusSet *set;
    TaskStatus *task;
    Entity *e;

    if (isAdd)
    {
        if ((e = playerPtr()) != NULL)
        {
            for (i = eaSize(&teamTaskSets) - 1; i >= 0; i--)
            {
                if ((set = teamTaskSets[i]) != NULL && set->dbid == e->db_id)
                {
                    for (j = eaSize(&set->list) - 1; j >= 0; j--)
                    {
                        if ((task = set->list[j]) != NULL && task->isZowie)
                        {
                            zowieAddGlowFlags(task);
                        }
                    }
                }
            }
        }
    }
    else
    {
        zowieRemoveGlowFlags();
    }
}

static void TaskStatusSetDifficulty(int dbid, int levelAdjust, int teamsize, int boss, int av)
{
    TaskStatusSet* set = TaskStatusSetGet(dbid);
    if (!set) 
        return;

    set->difficulty.levelAdjust = levelAdjust;
    set->difficulty.teamSize = teamsize;
    set->difficulty.dontReduceBoss = boss;
    set->difficulty.alwaysAV = av;
}

StoryDifficulty* TaskStatusGetNotoriety(int dbid)
{
    TaskStatusSet* set = TaskStatusSetGet(dbid);
    if (!set)
        return 0;
    return &set->difficulty;
}

TaskStatus** TaskStatusGetPlayerTasks()
{
    TaskStatus*** list;
    Entity* player = playerPtr();
    if(!player)
        return NULL;
    list = TaskStatusListGet(player->db_id);
    if(list)
        return *list;
    else
        return NULL;
}

void TaskStatusRemoveTeammates()
{
    Entity* e = playerPtr();
    int i;

    for(i = eaSize(&teamTaskSets)-1; i >= 0; i--)
    {
        if(teamTaskSets[i]->dbid != e->db_id)
        {
            TaskStatusSetDestroy(teamTaskSets[i]);
            eaRemove(&teamTaskSets, i);
        }
    }
}

int TaskStatusIsTeamTask(TaskStatus* task)
{
    return TaskStatusIsSame(task, PlayerGetActiveTask());
}

int TaskStatusIsSame(TaskStatus* task, TaskStatus* task2)
{
    if(!task || !task2) 
        return 0;
    return task->ownerDbid == task2->ownerDbid && task->context == task2->context && task->subhandle == task2->subhandle;
}

int PlayerOnRealTeam()
{
    Entity* player = playerPtr();
    Teamup* team;
    if (!player) 
        return 0;
    team = player->teamup;
    if (!team) 
        return 0;
    return team->members.count > 1;
}

int PlayerOnMyTeam(int dbid)
{
    int i;
    Teamup* team;
    Entity* player = playerPtr();

    if (!player) 
        return 0;
    if (player->db_id == dbid) 
        return 1;
    team = player->teamup;
    if (!team) 
        return 0;
    for (i = 0; i < team->members.count; i++)
    {
        if (team->members.ids[i] == dbid) 
            return 1;
    }
    return 0;
}

TaskStatus* PlayerGetActiveTask()
{
    TaskStatusSet* set = TaskStatusSetGetPlayer();
    Entity* player = playerPtr();
    TaskStatus* task;
    if(!player || !set)
        return NULL;

    // Get the first task, which should be either the player mission or a team task.
    task = set->list[0];

    if (task && task->ownerDbid && task->context )//&& task->subhandle)
        return task;
    else
        return NULL;
}

TaskStatus* TaskStatusGet(int dbid, int context, int subhandle)
{
    int i, n;
    TaskStatusSet* set;
    TaskStatus* task;
    extern TaskStatus zoneEvent;
    Entity* player = playerPtr();

    //in this special case we return the zone event that the client
    //is participating in
    if (dbid==-1 && context==-1 && subhandle==-1)
        return &zoneEvent;

    // we may be looking at the active mission
    set = TaskStatusSetGetPlayer();
    if (set && eaSize(&set->list))
    {
        task = set->list[0];
        if (task->ownerDbid == dbid && task->context == context && task->subhandle == subhandle)
            return task;
    }

    // We may be looking at the supergroup mission
    if (player && player->supergroup && player->supergroup->sgMission)
    {
        task = player->supergroup->sgMission;
        if (task->ownerDbid == dbid && task->context == context && task->subhandle == subhandle)
            return task;
    }

    // otherwise, look through teammates and I
    set = TaskStatusSetGet(dbid);
    if (!set) 
        return NULL;
    n = eaSize(&set->list);
    for (i = 0; i < n; i++)
    {
        if (set->list[i]->ownerDbid == dbid && 
            set->list[i]->context == context &&
            set->list[i]->subhandle == subhandle)
        {
            return set->list[i];
        }
    }
    return NULL;
}

void TaskUpdateColor(Destination* dest)
{
    if (dest->type == ICON_MISSION)
    {
        if (destinationEquiv(&activeTaskDest, dest)) 
            dest->colorB = CLR_RED;
        else if (destinationEquiv(&waypointDest, dest))
            dest->colorB = CLR_YELLOW;
        else
            dest->colorB = CLR_WHITE;
    }
    else if (dest->type == ICON_MISSION_WAYPOINT)
        dest->colorB = CLR_RED;
    else if (dest->type == ICON_MISSION_KEYDOOR)
        dest->colorB = CLR_YELLOW;
}

int DestinationIsAContact(Destination *dest)
{
    return dest && (dest->type == ICON_CONTACT
        || dest->type == ICON_CONTACT_HAS_NEW_MISSION 
        || dest->type == ICON_CONTACT_ON_MISSION 
        || dest->type == ICON_CONTACT_COMPLETED_MISSION
        || dest->type == ICON_TRAINER
        || dest->type == ICON_NOTORIETY);
}

// check the most current set of data in case the contact or task
// location needs to be updated
void TaskUpdateDestination(Destination* dest)
{
    if (dest->type == ICON_MISSION)
    {
        TaskStatus* task = TaskStatusGet(dest->dbid, dest->context, dest->subhandle);
        compass_SetTaskDestination(dest, task);
    }
    else if (DestinationIsAContact(dest))
    {
        ContactStatus* contact = ContactStatusGet(dest->handle);
        compass_SetContactDestination(dest, contact);
    }
}


char * difficultyString( StoryDifficulty *pDiff )
{
    if(!pDiff)
        return "";

    if( pDiff->alwaysAV )
        return textStd( "DifficultyClientAV", pDiff->levelAdjust, pDiff->teamSize );
    else
        return textStd( "DifficultyClientNoAV", pDiff->levelAdjust, pDiff->teamSize );
}
