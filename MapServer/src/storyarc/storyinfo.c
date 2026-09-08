/*\
 *
 *    storyinfo.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    handles StoryInfo, StoryContactInfo, StoryTaskInfo, StoryArcInfo structs
 *
 */

#include "storyinfo.h"
#include "storyarcprivate.h"
#include "dbcomm/staticMapInfo.h"
#include "container/team.h"
#include "dbcomm/dbcomm.h"
#include "entity/character_base.h"
#include "entity/entPlayer.h"
#include "container/containerbroadcast.h"
#include "entity/origins.h"
#include "taskforce.h"
#include "entity/entity.h"
#include "entity/alignment_shift.h"
#include "TeamReward.h"

// *********************************************************************************
//  Info struct constructors/destructors
// *********************************************************************************

MP_DEFINE(StoryContactInfo);
StoryContactInfo* storyContactInfoAlloc() { MP_CREATE(StoryContactInfo, 10); return MP_ALLOC(StoryContactInfo); }
void storyContactInfoFree(StoryContactInfo* info) { MP_FREE(StoryContactInfo, info); }

StoryContactInfo* storyContactInfoCreate(ContactHandle contact)
{
    StoryContactInfo* ret = storyContactInfoAlloc();
    ret->handle = contact;
    if (storyContactInfoInit(ret))
        return ret;
    storyContactInfoFree(ret);
    return NULL;
}

void storyContactInfoDestroy(StoryContactInfo* info)
{
    storyContactInfoFree(info);
}

int storyContactInfoInit(StoryContactInfo* info)
{
    info->contact = GetContact(info->handle);
    if (!info->contact)
    {
        Errorf("Failed lookup of contact handle %i", info->handle);
        log_printf("storyhandleerrors", "Failed lookup of contact handle %i", info->handle);
        return 0;
    }
    info->deleteMe = 0;
    return 1;
}

MP_DEFINE(StoryArcInfo);
StoryArcInfo* storyArcInfoAlloc() { MP_CREATE(StoryArcInfo, 10); return MP_ALLOC(StoryArcInfo); }
void storyArcInfoFree(StoryArcInfo* info) { MP_FREE(StoryArcInfo, info); }

StoryArcInfo* storyArcInfoCreate(StoryTaskHandle *sahandle, ContactHandle contact)
{
    StoryArcInfo* ret = storyArcInfoAlloc(); 
    memcpy( &ret->sahandle, sahandle, sizeof(StoryTaskHandle) );
    ret->contact = contact;
    if (storyArcInfoInit(ret))
        return ret;
    storyArcInfoFree(ret);
    return NULL;
}

void storyArcInfoDestroy(StoryArcInfo* info)
{
    storyArcInfoFree(info);
}

int storyArcInfoInit(StoryArcInfo* info)
{
    info->def = StoryArcDefinition(&info->sahandle);
    info->contactDef = ContactDefinition(info->contact);
    if (!info->def)
    {
        Errorf("Failed lookup of storyarc handle %i, with contact %i", info->sahandle.context, info->contact);
        log_printf("storyhandleerrors", "Failed lookup of storyarc handle %i, with contact %i", info->sahandle.context, info->contact);
        return 0;
    }
    if (!info->contactDef)
    {
        Errorf("Failed lookup of contact %i for storyarc %i", info->contact, info->sahandle.context);
        log_printf("storyhandleerrors", "Failed lookup of contact %i for storyarc %i", info->contact, info->sahandle.context);
        return 0;
    }
    if (info->episode >= eaSize(&info->def->episodes))
    {
        Errorf("Player's storyarc was on episode %i when storyarc %i only has %i episodes", info->episode, info->sahandle.context, eaSize(&info->def->episodes));
        log_printf("storyhandleerrors", "Player's storyarc was on episode %i when storyarc %i only has %i episodes", info->episode, info->sahandle.context, eaSize(&info->def->episodes));
        return 0;
    }
    return 1;
}


StoryTaskInfo* storyTaskInfoCreate(StoryTaskHandle *sahandle)
{
    StoryTaskInfo* ret = storyTaskInfoAlloc();
    ret->sahandle.context = sahandle->context;
    ret->sahandle.subhandle = sahandle->subhandle;
    ret->sahandle.bPlayerCreated = sahandle->bPlayerCreated;
    if (storyTaskInfoInit(ret))
        return ret;
    storyTaskInfoFree(ret);
    return NULL;
}


int storyTaskInfoInit(StoryTaskInfo* info)
{
    info->def = TaskParentDefinition(&info->sahandle);
    if (info->def && info->def->type == TASK_COMPOUND)
    {
        info->compoundParent = info->def;
        info->def = TaskChildDefinition(info->def, info->sahandle.compoundPos);
    }
    else
    {
        info->compoundParent = 0;
    }
    if (!info->def || !TaskSanityCheck(info->def))
    {
        if (isStoryarcHandle(&info->sahandle))
        {
            const StoryArc* sa = StoryArcDefinition(&info->sahandle);
            if (sa)
            {
                Errorf("Failed lookup of task handle (%i,%i,%i,%i) for storyarc %s", SAHANDLE_ARGSPOS(info->sahandle), sa->filename);
                log_printf("storyhandleerrors", "Failed lookup of task handle (%i,%i,%i,%i) for storyarc %s", SAHANDLE_ARGSPOS(info->sahandle), sa->filename);
                return 0;
            }
            else if ( !info->sahandle.bPlayerCreated )
            {
                Errorf("Failed lookup of task handle (%i,%i,%i,%i) for unknown storyarc",SAHANDLE_ARGSPOS(info->sahandle));
                log_printf("storyhandleerrors", "Failed lookup of task handle (%i,%i,%i,%i) for unknown storyarc", SAHANDLE_ARGSPOS(info->sahandle));
                return 0;
            }
        }
        else
        {
            const ContactDef* contact = ContactDefinition(info->sahandle.context);
            if (contact)
            {
                Errorf("Failed lookup of task handle (%i,%i,%i,%i) for contact %s", SAHANDLE_ARGSPOS(info->sahandle), contact->filename);
                log_printf("storyhandleerrors", "Failed lookup of task handle (%i,%i,%i,%i) for contact %s", SAHANDLE_ARGSPOS(info->sahandle), contact->filename);
                return 0;
            }
            else
            {
                Errorf("Failed lookup of task handle (%i,%i,%i,%i) for unknown contact", SAHANDLE_ARGSPOS(info->sahandle));
                log_printf("storyhandleerrors", "Failed lookup of task handle (%i,%i,%i,%i) for unknown contact", SAHANDLE_ARGSPOS(info->sahandle));
                return 0;
            }
        }
    }

    memset( &info->difficulty, 0, sizeof(StoryDifficulty) );

    return 1;
}

int storyTaskInfoGetAlliance(StoryTaskInfo* task)
{
    const StoryArc* sa = NULL;
    int alliance = SA_ALLIANCE_BOTH;

    if (!task || !task->def)
        return alliance;

    sa = StoryArcDefinition(&task->sahandle);
    if (sa != NULL)
        alliance = sa->alliance;
    else if (task->compoundParent != NULL)
        alliance = task->compoundParent->alliance;
    else 
        alliance = task->def->alliance;

    return alliance;
}

const char **storyTaskHandleGetTeamRequires(StoryTaskHandle *sahandle, const char **outputFilename)
{
    const StoryArc* sa = StoryArcDefinition(sahandle);
    const StoryTask* task = TaskParentDefinition(sahandle);

    if (!task)
    {
        if (outputFilename)
            *outputFilename = NULL;

        return NULL;
    }

    if (sa != NULL && sa->teamRequires != NULL)
    {
        if (outputFilename)
            *outputFilename = sa->filename;

        return sa->teamRequires;
    }
    else if (task->teamRequires != NULL)
    {
        if (outputFilename)
            *outputFilename = task->filename;

        return task->teamRequires;
    }
    else
    {
        task = TaskDefinition(sahandle);
        if (outputFilename)
            *outputFilename = task->filename;

        return task->teamRequires;
    }
}

const char *storyTaskHandleGetTeamRequiresFailedText(StoryTaskHandle *sahandle)
{
    const StoryArc* sa = StoryArcDefinition(sahandle);
    const StoryTask* task = TaskParentDefinition(sahandle);

    if (!task)
        return NULL;

    if (sa != NULL && sa->teamRequiresFailed != NULL)
        return sa->teamRequiresFailed;
    else if (task->teamRequiresFailed != NULL)
        return task->teamRequiresFailed;
    else
        return (TaskDefinition(sahandle))->teamRequiresFailed;
}

MP_DEFINE(StoryInfo);
StoryInfo* storyInfoCreate()
{
    StoryInfo* info;
    MP_CREATE(StoryInfo,10);
    info = MP_ALLOC(StoryInfo);
    eaCreate(&info->contactInfos);
    eaCreate(&info->storyArcs);
    eaCreate(&info->tasks);
    eaCreateConst(&info->souvenirClues);

    return info;
}

void storyContactInfoDestroy(StoryContactInfo* info);
static void storyContactInfoDestroyCallback(void* arg0)
{
    storyContactInfoDestroy((StoryContactInfo*)arg0);
}

void storyArcInfoDestroy(StoryArcInfo* info);
static void storyArcInfoDestroyCallback(void* arg0)
{
    storyArcInfoDestroy((StoryArcInfo*)arg0);
}

static void storyTaskInfoDestroyCallback(void* arg0)
{
    storyTaskInfoDestroy((StoryTaskInfo*)arg0);
}

void storyInfoDestroy(StoryInfo* info)
{
    int i;
    if(!info)
        return;

    eaDestroyEx(&info->contactInfos, storyContactInfoDestroyCallback);
    eaDestroyEx(&info->storyArcs, storyArcInfoDestroyCallback);
    eaDestroyEx(&info->tasks, storyTaskInfoDestroyCallback);

    //only free souvenirClues that have uid==-1, otherwise they are pointing
    //to shared memory
    for (i=0;i<eaSize(&info->souvenirClues);i++)
    {
        if (info->souvenirClues[i]->uid==-1)
            freeConst(info->souvenirClues[i]);
    }
    eaDestroyConst(&info->souvenirClues);

    MP_FREE(StoryInfo, info);
}

void StoryArcLeavingPraetoriaReset(Entity* ent)
{
    if (!ent)
        return;

    if (ent->storyInfo)
    {
        int i;
        for (i = eaSize(&ent->storyInfo->contactInfos)-1; i >=0; --i)
        {
            StoryContactInfo *contactInfo = ent->storyInfo->contactInfos[i];

            // Remove Praetorian Universe contacts only - Do not change this to ContactCannotInteract or a similar check
            // which checks InteractionRequires as this function takes place before product inventory load
            if (!contactInfo || !contactInfo->contact->def || !PlayerUniverseMatchesDefUniverse(UNIVERSE_PRIMAL, contactInfo->contact->def->universe))
            {
                ContactDismiss(ent, contactInfo->handle, 1);
            }
        }
    }
    else
    {
        ent->storyInfo = storyInfoCreate();
    }

    if (ent->storyInfo)
    {
        ContactHandle handle;

        if (ENT_IS_VILLAIN(ent))
        {
            if (rand() % 2)
                handle = ContactGetHandle("V_Contacts/Brokers/Mickey_The_Filch.contact");
            else
                handle = ContactGetHandle("V_Contacts/Brokers/Kara_The_Scorpion.contact");

            if (handle)
                ContactAdd(ent, ent->storyInfo, handle, "auto-issue arriving from Praetoria");

            handle = ContactGetHandle("V_Contacts/SharkheadIsle/Dean_MacArthur.contact");
            if (handle)
                ContactAdd(ent, ent->storyInfo, handle, "auto-issue arriving from Praetoria");

            handle = ContactGetHandle("V_Contacts/SharkheadIsle/Darrin_Wade.contact");
            if (handle)
                ContactAdd(ent, ent->storyInfo, handle, "auto-issue arriving from Praetoria");

            // Science and tech get Capt. Petrovich.
            // Magic, Mutation and Natural get Lorenz Ansaldo.
            if (!stricmp(ent->pchar->porigin->pchName, "Technology") || !stricmp(ent->pchar->porigin->pchName, "Science"))
                handle = ContactGetHandle("V_Contacts/SharkheadIsle/Captain_Petrovich.contact");
            else
                handle = ContactGetHandle("V_Contacts/SharkheadIsle/Lorenz_Ansaldo.contact");

            if (handle)
                ContactAdd(ent, ent->storyInfo, handle, "auto-issue arriving from Praetoria");
        }
        else
        {
            if (!stricmp(ent->pchar->porigin->pchName, "Magic"))
            {
                switch (rand() % 5)
                {
                    case 0:
                    default:
                        handle = ContactGetHandle("Contacts/Level_4/Andrea_Mitchell.contact");
                        break;
                    case 1:
                        handle = ContactGetHandle("Contacts/Level_4/Cain_Royce.contact");
                        break;
                    case 2:
                        handle = ContactGetHandle("Contacts/Level_4/Joesef_Keller.contact");
                        break;
                    case 3:
                        handle = ContactGetHandle("Contacts/Level_4/Oliver_Haak.contact");
                        break;
                    case 4:
                        handle = ContactGetHandle("Contacts/Level_4/Piper_Irving.contact");
                        break;
                }
            }
            else if (!stricmp(ent->pchar->porigin->pchName, "Technology"))
            {
                switch (rand() % 4)
                {
                    case 0:
                    default:
                        handle = ContactGetHandle("Contacts/Level_4/Andrew_Fiore.contact");
                        break;
                    case 1:
                        handle = ContactGetHandle("Contacts/Level_4/Claire_Childress.contact");
                        break;
                    case 2:
                        handle = ContactGetHandle("Contacts/Level_4/Lt_Col_Hugh_McDougal.contact");
                        break;
                    case 3:
                        handle = ContactGetHandle("Contacts/Level_4/Vic_Garland.contact");
                        break;
                }
            }
            else if (!stricmp(ent->pchar->porigin->pchName, "Natural"))
            {
                if (rand() % 2)
                    handle = ContactGetHandle("Contacts/Level_4/John_Strobel.contact");
                else
                    handle = ContactGetHandle("Contacts/Level_4/Barry_Gosford.contact");
            }
            else if (!stricmp(ent->pchar->porigin->pchName, "Mutation"))
            {
                switch (rand() % 4)
                {
                    case 0:
                    default:
                        handle = ContactGetHandle("Contacts/Level_4/Hinckley_Rasmussen.contact");
                        break;
                    case 1:
                        handle = ContactGetHandle("Contacts/Level_4/Jim_Bell.contact");
                        break;
                    case 2:
                        handle = ContactGetHandle("Contacts/Level_4/Manuel_Ruiz.contact");
                        break;
                    case 3:
                        handle = ContactGetHandle("Contacts/Level_4/Tyler_French.contact");
                        break;
                }
            }
            else // Science
            {
                handle = ContactGetHandle("Contacts/Level_4/Polly_Cooper.contact");
            }

            if (handle)
                ContactAdd(ent, ent->storyInfo, handle, "auto-issue arriving from Praetoria");

            handle = ContactGetHandle("Contacts/Steel_Canyon/Montague_Castanella.contact");
            if (handle)
                ContactAdd(ent, ent->storyInfo, handle, "auto-issue arriving from Praetoria");

            handle = ContactGetHandle("Contacts/Talos_Island/Field_Agent_Keith_Nance.contact");
            if (handle)
                ContactAdd(ent, ent->storyInfo, handle, "auto-issue arriving from Praetoria");
        }
    }
}

// *********************************************************************************
//  Init new players
// *********************************************************************************

// This function no longer auto-issues contacts.
// We're using data to drive those.
void StoryArcNewPlayer(Entity* ent)
{
    if(ent->storyInfo)
    {
        storyInfoDestroy(ent->storyInfo);
        ent->storyInfo = NULL;
    }

    ent->storyInfo = storyInfoCreate();
}

// clear all storyarc info
void StoryArcInfoResetUnlocked(Entity* e)
{
    StoryInfo* info;
    info = StoryInfoFromPlayer(e);
    if (e->pl->taskforce_mode)
    {
        storyInfoDestroy(info);
        info = storyInfoCreate();
        e->taskforce->storyInfo = info;
        info->contactInfoChanged = 1;
        info->clueChanged = 1;
        info->visitLocationChanged = 1;
    }
    else
    {
        StoryArcNewPlayer(e);
        info->contactInfoChanged = 1;
        info->clueChanged = 1;
        info->visitLocationChanged = 1;
    }
    if (e->teamup && e->teamup->activetask->assignedDbId == e->db_id)
        TeamTaskClear(e);
}

// actually used for production now, when player moves from
// tutorial to regular zone
void StoryArcInfoReset(Entity* e)
{
    teamDetachMap(e);
    StoryArcLockInfo(e);
    StoryArcInfoResetUnlocked(e);
    StoryArcUnlockInfo(e);
    alignmentshift_resetAllTips(e);
    alignmentshift_resetPoints(e);
    removeAllActivePlayerRewardTokens(e);
    StoryArcPlayerEnteredMap(e);
}

// *********************************************************************************
//  Locking & task force mode transparency
// *********************************************************************************

// get correct info (task force or normal)
StoryInfo* StoryInfoFromPlayer(Entity* player)
{
    if (PlayerInTaskForceMode(player))
    {
        if (!player->taskforce || !player->taskforce_id)
        {
            log_printf("storyerrs.log", "StoryInfoFromPlayer: Player %s(%i) was set to task force mode but didn't have one",
                player->name, player->db_id);
            player->pl->taskforce_mode = 0;
            assert(player->storyInfo);
            return player->storyInfo;
        }
        assert(player->taskforce->storyInfo);
        return player->taskforce->storyInfo;
    }
    else
    {
        assert(player->storyInfo);
        return player->storyInfo;
    }
}

int PlayerInTaskForceMode(Entity* ent)
{
    if (!ent || !ent->pl || !ent->taskforce_id)
        return 0;
    else if( !ent->pl->taskforce_mode ) // safety check in case we somehow get out of synch
    {
        if( ent->taskforce ) // your mode always depends on whether you have a taskforce
        {
            if(estrLength(&ent->taskforce->playerCreatedArc))
                ent->pl->taskforce_mode = TASKFORCE_ARCHITECT;
            else
                ent->pl->taskforce_mode = TASKFORCE_DEFAULT;
        }
    }

    return ent->pl->taskforce_mode;
}

ContactHandle PlayerTaskForceContact(Entity* ent)
{
    if (!ent || !ent->taskforce)
        return 0;
    if (!eaSize(&ent->taskforce->storyInfo->contactInfos))
        return 0;
    return ent->taskforce->storyInfo->contactInfos[0]->handle;
}

int *PlayerGetTasksMapIDs(Entity *ent)
{
    int *retval = NULL;
    StoryInfo *info;
    int count;
    StoryLocation *storylocs = NULL;
    int numlocs = 0;

    if (ent)
    {
        info = StoryInfoFromPlayer(ent);

        if (info)
        {
            for (count = 0; count < eaSize(&info->tasks); count++)
            {
                if (info->tasks[count])
                {
                    storylocs = ContactTaskGetLocations(ent, info->tasks[count], &numlocs);
                }
            }

            for (count = 0; count < numlocs; count++)
            {
                if (storylocs[count].mapID)
                {
                    eaiSortedPushUnique(&retval, storylocs[count].mapID);
                }
            }
        }
    }

    return retval;
}

static int g_storyarclock = 0;
static Entity *g_storyarclockPlayer = NULL;

Entity *StoryArcInfoIsLocked()
{
    return (g_storyarclockPlayer);
}


StoryInfo* StoryArcLockInfo(Entity* player)
{
    assert(g_storyarclock == 0);    // just a little sanity check
    g_storyarclock++;
    g_storyarclockPlayer = player;

    if (!player->owned)
    {
        log_printf("storyerrs.log", "StoryArcLockInfo: %s(%i) isn't owned!", player->name, player->db_id);
    }

    // lock the teamup if necessary
    if (PlayerInTaskForceMode(player))
        teamLock(player, CONTAINER_TASKFORCES);

    return StoryInfoFromPlayer(player);
}

// Gets the players info even if they are in a task force
StoryInfo* StoryArcLockPlayerInfo(Entity* player)
{
    assert(g_storyarclock == 0);    // just a little sanity check
    g_storyarclock++;
    g_storyarclockPlayer = player;

    if (!player->owned)
    {
        log_printf("storyerrs.log", "StoryArcLockPlayerInfo: %s(%i) isn't owned!", player->name, player->db_id);
    }
    assert(player->storyInfo);
    return player->storyInfo;
}

// Unlocks the players info even when in a task force
void StoryArcUnlockPlayerInfo(Entity* player)
{
    StoryInfo* info = StoryInfoFromPlayer(player);
    assert(g_storyarclock == 1 && player == g_storyarclockPlayer);    // just a little sanity check
    g_storyarclock--;
    g_storyarclockPlayer = NULL;

}

void StoryArcUnlockInfo(Entity* player)
{
    StoryInfo* info = StoryInfoFromPlayer(player);
    int destroyTaskForce = info->destroyTaskForce;
    int taskRefresh = 0;
    int fullRefresh = 0;

    assert(g_storyarclock == 1 && player == g_storyarclockPlayer);    // just a little sanity check
    g_storyarclock--;
    g_storyarclockPlayer = NULL;

    if (PlayerInTaskForceMode(player))
    {
        if (!destroyTaskForce) // don't do updates if we are about to destroy
        {
            if (info->contactInfoChanged || info->visitLocationChanged)
            {
                info->contactInfoChanged = info->visitLocationChanged = 0;
                taskRefresh = 1;
            }
            if (info->clueChanged || info->contactFullUpdate)
            {
                info->clueChanged = info->contactFullUpdate = 0;
                fullRefresh = 1;
            }
        }

        // tell the task force to delete itself if necessary
        if (destroyTaskForce)
            player->taskforce->deleteMe = 1;

        // unlock the teamup
        teamUpdateUnlock(player, CONTAINER_TASKFORCES);

        // otherwise refresh the appropriate info (after it's been saved)
        if (!destroyTaskForce && fullRefresh)
        {
            dbBroadcastMsg(CONTAINER_TASKFORCES, &player->taskforce_id, INFO_SVR_COM, 0, 1, DBMSG_STORYARC_FULL_REFRESH);
        }
        else if (!destroyTaskForce && taskRefresh)
        {
            if (player->teamup_id)
            {
                dbBroadcastMsg(CONTAINER_TEAMUPS,&player->teamup_id, INFO_SVR_COM, 0, 1, DBMSG_STORYARC_TASK_REFRESH);
            }
            else
            {
                StoryArcSendTaskRefresh(player);
            }
        }
    }
}

// check player for team task updates
// (the idea is to pull the team lock/unlock out of other processing so it simplifies logic)
void StoryTick(Entity* player)
{
    StoryInfo* info = player->storyInfo;
    if (info->taskStatusChanged)
    {
        info = StoryArcLockPlayerInfo(player);
        info->taskStatusChanged = 0;
        TeamTaskUpdateStatus(player);
        StoryArcUnlockPlayerInfo(player);
    }
}

// how many long arcs does this player have?
int StoryArcCountLongArcs(StoryInfo* info)
{
    int n =    eaSize(&info->storyArcs);
    int i, count = 0;

    for (i = 0; i < n; i++)
    {
        if (!(info->storyArcs[i]->def->flags & STORYARC_MINI))
            count++;
    }
    return count;
}

int StoryArcMaxStoryarcs(StoryInfo* info)
{
    if (info->taskforce)
        return STORYARCS_PER_TASKFORCE;
    else
        return STORYARCS_PER_PLAYER;
}

int StoryArcMaxLongStoryarcs(StoryInfo* info)
{
    return MAX_LONG_STORYARCS;
}

int StoryArcMaxContacts(StoryInfo* info)
{
    if (info->taskforce)
        return CONTACTS_PER_TASKFORCE;
    else
        return CONTACTS_PER_PLAYER;
}

int StoryArcMaxTasks(StoryInfo* info)
{
    if (info->taskforce)
        return TASKS_PER_TASKFORCE;
    else
        return TASKS_PER_PLAYER;
}

