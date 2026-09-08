/*\
 *
 *    contact.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    main file for contacts
 *    deals mainly with maintaining ContactInfo records on players
 *
 *    Contacts:
 *        - issue missions and tasks
 *        - issue storyarcs
 *        - contain stores
 *        - have extensive dialog
 *        - introduce other contacts
 *
 */

#include "contact.h"
#include "storyarcprivate.h"
#include "entity/character_base.h"
#include "entity/entPlayer.h"
#include "dbcomm/dbcomm.h"
#include "entity/origins.h"
#include "cmdparse/cmdserver.h"
#include "entity/entity.h"
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include "dbcomm/dbdoor.h"
#include "player/badges_server.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "storyarc/missionMapCommon.h"
#include "storySend.h"
#include "entity/character_eval.h"
#include "entity/alignment_shift.h"
#include "dbcomm/logcomm.h"
#include "storyarc/pnpcCommon.h"
#include <utilitieslib/components/earray.h>
#include "storyarc.h"
#include "TeamReward.h"

// *********************************************************************************
//  Contact info functions
// *********************************************************************************

StoryContactInfo* ContactGetInfo(StoryInfo* info, ContactHandle handle)
{
    int i, n;

    n = eaSize(&info->contactInfos);
    for (i = 0; i < n; i++)
    {
        if (info->contactInfos[i]->handle == handle)
            return info->contactInfos[i];
    }
    return NULL;
}

StoryContactInfo* ContactGetInfoFromDefinition(StoryInfo* info, const ContactDef *def)
{
    int i, n;

    n = eaSize(&info->contactInfos);
    for (i = 0; i < n; i++)
    {
        if (info->contactInfos[i]->contact->def == def)
            return info->contactInfos[i];
    }
    return NULL;
}

// corresponds to isActiveContact on the client
bool ContactIsActive(Entity *player, StoryInfo *info, StoryContactInfo *contactInfo)
{
    const ContactDef* contactDef;

    if (!player || !info || !contactInfo || !contactInfo->contact)
    {
        // default value for bad data
        return false;
    }
    
    contactDef = contactInfo->contact->def;
    if (contactDef->tipType)
    {
        return false;
    }

    if (contactDef->interactionRequires 
        && !chareval_Eval(player->pchar, contactDef->interactionRequires, contactDef->filename))
    {
        return false;
    }

    if (CONTACT_IS_NEWSPAPER(contactDef))
    {
        return true;
    }

    return (contactInfo->notifyPlayer 
            || ContactHasNewTasksForPlayer(player, contactInfo, info) 
            || ContactCanIntroduceNewContacts(player, info, contactInfo) 
            || ContactGetAssignedTask(player->db_id, contactInfo, info));

}

const ContactDef *ContactGetLastContact(Entity* player, int seed, int statureLevel)
{
    const ContactDef *returnMe = NULL;
    StoryInfo* info = StoryInfoFromPlayer(player);

    // Return an active contact first...
    if (info->taskforce)
    {
        returnMe = info->contactInfos[0]->contact->def;
    }
    else
    {
        int index;

        for (index = eaSize(&info->contactInfos) - 1; index >= 0; index--)
        {
            StoryContactInfo *contactInfo = info->contactInfos[index];
            if (contactInfo && ContactIsActive(player, info, contactInfo) && !CONTACT_IS_NEWSPAPER(contactInfo->contact->def))
            {
                returnMe = contactInfo->contact->def;
                break;
            }
        }
    }

    // Check the Contact Finder next...
    if (!returnMe)
    {
        returnMe = AccessibleContacts_getCurrent(player, NULL);
    }

    // final default to make sure we always return a valid ContactDef.
    if (!returnMe)
    {
        if (ENT_IS_IN_PRAETORIA(player))
        {
            returnMe = ContactDefinition(ContactGetHandle("Contacts/Praetoria/Nova_Praetoria/Marauder.contact"));
        }
        else if (ENT_IS_VILLAIN(player))
        {
            returnMe = ContactDefinition(ContactGetHandle("V_Contacts/MercyIsland/Operative_Kuzmin.contact"));
        }
        else
        {
            returnMe = ContactDefinition(ContactGetHandle("Contacts/Atlas_Park/Matthew_Habashy.contact"));
        }
    }

    return returnMe;
}

int ContactAdd(Entity* player, StoryInfo* info, ContactHandle handle, const char *desc)
{
    int n, max;
    const ContactDef * pContact = ContactDefinition(handle);

    if(!PlayerInTaskForceMode(player) && pContact && pContact->filename && desc)
    {
        LOG_ENT_OLD(player,"Contact:Add %s (via %s)", pContact->filename, desc);
    }

    n = eaSize(&info->contactInfos);
    max = StoryArcMaxContacts(info);
    if (pContact && CONTACT_IS_AUTODISMISSED(pContact))
    {
        sendInfoBox(player, INFO_SVR_COM, "ContactCannotAdd");
        return 0;
    }

    if (n < max 
        || isDevelopmentMode()) // add them anyway.
    {
        StoryContactInfo* contactInfo = storyContactInfoCreate(handle);
        if (!contactInfo)
        {
            LOG_OLD_ERR( "ContactAdd called with invalid contact handle %i", handle);
            return 0;
        }

        if(n>=max)
            Errorf("player has too many contacts. If you didn't cheat to cause this grab a programmer");

        contactInfo->contactRelationship = CONTACT_ACQUAINTANCE;
        contactInfo->contactIntroSeed = contactInfo->dialogSeed = (int)time(NULL);
        contactInfo->contactsIntroduced = CI_NONE;            // Initializing to none
        eaPush(&info->contactInfos, contactInfo);
        ContactMarkDirty(player, info, contactInfo);
        ContactMarkSelected(player, info, contactInfo);

        if (player && !PlayerInTaskForceMode(player))
        {
            const ContactDef* contactDef = contactInfo->contact->def;

            ContactTaskPickInfo shortTask={0};
            ContactTaskPickInfo longTask={0};

            // Check for automatic task grants
            ContactPickTask(player, info, contactInfo, TASK_SHORT, &shortTask, true);
            ContactPickTask(player, info, contactInfo, TASK_LONG, &longTask, true);
            ContactHideNonUrgentTask(&shortTask, &longTask);

            if (TASK_IS_AUTO_ISSUE(longTask.taskDef) // has a NULL check for the pointer baked in
                && (!longTask.taskDef->autoIssueRequires || chareval_Eval(player->pchar, longTask.taskDef->autoIssueRequires, longTask.taskDef->filename)))
            {
                unsigned int seed = contactInfo->dialogSeed;    // task seed will become our dialog seed
                int limitLevel = contactDef->maxPlayerLevel + contactDef->taskForceLevelAdjust;

                if (isStoryarcHandle(&longTask.sahandle))
                    seed = StoryArcGetSeed(info, &longTask.sahandle);

                TaskAdd(player, &longTask.sahandle, seed, limitLevel, 0);
            }
            else if (TASK_IS_AUTO_ISSUE(shortTask.taskDef) // has a NULL check for the pointer baked in
                && (!shortTask.taskDef->autoIssueRequires || chareval_Eval(player->pchar, shortTask.taskDef->autoIssueRequires, shortTask.taskDef->filename)))
            {
                unsigned int seed = contactInfo->dialogSeed;    // task seed will become our dialog seed
                int limitLevel = contactDef->maxPlayerLevel + contactDef->taskForceLevelAdjust;

                if (isStoryarcHandle(&shortTask.sahandle))
                    seed = StoryArcGetSeed(info, &shortTask.sahandle);

                TaskAdd(player, &shortTask.sahandle, seed, limitLevel, 0);
            }
        }

        return 1;
    }
    else
    {
        FatalErrorf("contact.c - player has too many contacts\n");
        return 0;
    }
}

int ContactRemove(StoryInfo* info, ContactHandle handle)
{
    int i, n;
    StoryContactInfo* contactInfo;

    n = eaSize(&info->contactInfos);
    for (i = 0; i < n; i++)
    {
        contactInfo = info->contactInfos[i];
        if (contactInfo->handle != handle) continue;
        contactInfo->deleteMe = 1;
        info->contactInfoChanged = 1;
        return 1;
    }
    return 0;
}

int ContactDismiss(Entity *e, ContactHandle contactHandle, int dismissEvenIfOnTask)
{
    int returnMe = 0;
    StoryInfo* info;
    bool iLockedStoryInfo;
    
    // This needs to be conditional because some of the calling functions have already locked the info
    if (StoryArcInfoIsLocked(e))
    {
        info = StoryInfoFromPlayer(e);
        iLockedStoryInfo = false;
    }
    else
    {
        info = StoryArcLockInfo(e);
        iLockedStoryInfo = true;
    }

    if (info)
    {
        int t, numTasks = eaSize(&info->tasks);
        int s, numStoryarcs = eaSize(&info->storyArcs);

        StoryTaskInfo* task = NULL;
        StoryArcInfo* arc = NULL;

        for (t = 0; t < numTasks && !task; t++)
            if (TaskGetContactHandle(info, info->tasks[t]) == contactHandle)
                task = info->tasks[t];

        for (s = 0; s < numStoryarcs && !arc; s++)
            if (info->storyArcs[s]->contact == contactHandle)
                arc = info->storyArcs[s];

        // If its the active mission, we want to give the player a chance to complete it
        if (!task || !TaskIsActiveMission(e, task) || TASK_COMPLETE(task->state) || dismissEvenIfOnTask)
        {
            // Cleanup the rest, order is important because of dependencies
            StoryContactInfo* contactInfo = ContactGetInfo(info, contactHandle);
            const ContactDef *contactDef = ContactDefinition(contactHandle);

            if (contactDef && (!task || !TASK_COMPLETE(task->state)))
            {
                if (contactDef->tipType == TIPTYPE_ALIGNMENT)
                {
                    giveRewardToken(e, "TotalAlignmentTipsDismissed");
                }
                else if (contactDef->tipType == TIPTYPE_MORALITY)
                {
                    giveRewardToken(e, "TotalMoralityTipsDismissed");
                }
            }

            if (contactDef && contactDef->dismissReward)
                StoryRewardApply(*contactDef->dismissReward, e, info, task, contactInfo, NULL, NULL);
            if (task) 
                TaskAbandon(e, info, contactInfo, task, dismissEvenIfOnTask);
            if (arc)
            {
                FlashbackLeftRewardApplyToTaskforce(e);
                RemoveStoryArc(e, info, arc, contactInfo, false);
            }
            returnMe = ContactRemove(info, contactHandle);
        }
    }

    if (iLockedStoryInfo)
    {
        StoryArcUnlockInfo(e);
    }
    return returnMe;
}

int ContactCanBeDismissed(Entity *player, StoryContactInfo *contactInfo)
{
    Teamup *team = player->teamup;
    StoryTaskInfo *contactTask = ContactGetAssignedTask(player->db_id, contactInfo, player->storyInfo);

    // can't dismiss contacts that aren't flagged as dismissable
    if (!CONTACT_IS_DISMISSABLE(contactInfo->contact->def) && !CONTACT_IS_AUTODISMISSED(contactInfo->contact->def))
        return false;

    // can't dismiss contacts if we're on a mission map from that contact
    if (team && TaskIsActiveMission(player, contactTask)
        && team->map_id && team->map_playersonmap != 0)
    {
        return false;
    }

    // otherwise, this can be dismissed
    return true;
}

// *********************************************************************************
//  Contact/Task interaction
// *********************************************************************************

const StoryTask* ContactTaskDefinition(const StoryTaskHandle *sahandle)
{
    const ContactDef* contact = ContactDefinition(sahandle->context);
    int taskindex = StoryHandleToIndex(sahandle);
    int n;

    if (!contact) 
        return NULL;
    n = eaSize(&contact->tasks);
    if (taskindex < 0) 
        return NULL;
    if (taskindex >= n) 
        return NULL;
    return contact->tasks[taskindex];
}

TaskSubHandle ContactGetTaskSubHandle(ContactHandle contacthandle, StoryTask* taskdef)
{
    const ContactDef* contact = ContactDefinition(contacthandle);
    int i, n;

    // search through all our tasks for the correct one
    if (!contact) 
        return 0;

    n = eaSize(&contact->tasks);
    for (i = 0; i < n; i++)
    {
        if (contact->tasks[i] == taskdef)
            return StoryIndexToHandle(i);
    }
    return 0;
}

/* Function ContactGetAssignedTask()
 *    Returns the StoryTaskInfo, or NULL
 */
StoryTaskInfo* ContactGetAssignedTask(int player_id, StoryContactInfo* contactInfo, StoryInfo* info)
{
    int i, n;
    StoryArcInfo* storyarc = StoryArcGetInfo(info, contactInfo);
    int context = storyarc?storyarc->sahandle.context:0;
    Entity* player = entFromDbId(player_id);

    // Check to see if the contact is a sg mission contact, and the sg already has a mission
    if (CONTACT_IS_SGCONTACT(contactInfo->contact->def) && player && player->supergroup && player->supergroup->activetask)
        return player->supergroup->activetask;

    // look for tasks directly attached to contact, or attached to my story arc
    n = eaSize(&info->tasks);
    for (i = 0; i < n; i++)
    {
        int assignedtome = info->tasks[i]->assignedDbId == player_id;
        int rightcontext = info->tasks[i]->sahandle.context == contactInfo->handle || info->tasks[i]->sahandle.context == context;
        if (assignedtome && rightcontext)
        {
            return info->tasks[i];
        }
    }
    return NULL;
}

// *********************************************************************************
//  Contact debugging
// *********************************************************************************

static int sortContacts(const void* leftData, const void* rightData)
{
    const ContactDef** left = (const ContactDef**)leftData;
    const ContactDef** right = (const ContactDef**)rightData;
    const ContactDef* lhs = *left;
    const ContactDef* rhs = *right;
    char lname[1000];
    char rname[1000];
    ScriptVarsTable vars = {0};

    if (lhs->stature < rhs->stature) return -1;
    if (lhs->stature > rhs->stature) return 1;

    ScriptVarsTablePushScope(&vars, &lhs->vs);
    strcpy_s(SAFESTR(lname), saUtilTableLocalize(lhs->displayname, &vars));
    ScriptVarsTablePopScope(&vars);

    ScriptVarsTablePushScope(&vars, &rhs->vs);
    strcpy_s(SAFESTR(rname), saUtilTableLocalize(rhs->displayname, &vars));
    return stricmp(lname, rname);
}

static char* datamineNA = "N/A";
static char* datamineHero = "HERO";
static char* datamineVillain = "VILLAIN";
static char* datamineBoth = "BOTH";

static int datamineStoryTasks(ClientLink* client, char* header,
    const StoryTask** tasks, int firstnum, int overriddenmin, int overriddenmax,
    bool overriddenheroes, bool overriddenvillains)
{
    int retval;
    int count;
    char* templine = NULL;

    retval = 0;

    for (count = 0; count < eaSize(&tasks); count++)
    {
        if (tasks[count] && !(tasks[count]->deprecated))
        {
            int min = overriddenmin;
            int max = overriddenmax;

            // Clamp to the extents on the mission if they are more
            // restrictive than the ones on the story arc/contact.
            if (min < tasks[count]->minPlayerLevel)
            {
                min = tasks[count]->minPlayerLevel;
            }

            if (max > tasks[count]->maxPlayerLevel)
            {
                max = tasks[count]->maxPlayerLevel;
            }

            if (tasks[count]->type == TASK_COMPOUND)
            {
                retval += datamineStoryTasks(client, header,
                    tasks[count]->children, firstnum + retval,
                    min, max, overriddenheroes, overriddenvillains);
            }
            else
            {
                const char* title = tasks[count]->taskTitle;
                const MissionDef* mission = NULL;
                const char* locationstr = datamineNA;
                const char* villainstr = datamineNA;
                const char* missionstr = datamineNA;
                const char* doortypestr = datamineNA;
                bool heroes = overriddenheroes;
                bool villains = overriddenvillains;
                char* alliancestr = datamineBoth;

                switch (tasks[count]->alliance)
                {
                    case SA_ALLIANCE_BOTH:
                    default:
                        break;
                    case SA_ALLIANCE_HERO:
                        villains = false;
                        break;
                    case SA_ALLIANCE_VILLAIN:
                        heroes = false;
                        break;
                }

                if (heroes)
                {
                    if (villains)
                    {
                        alliancestr = datamineBoth;
                    }
                    else
                    {
                        alliancestr = datamineHero;
                    }
                }
                else
                {
                    if (villains)
                    {
                        alliancestr = datamineVillain;
                    }
                    else
                    {
                        alliancestr = datamineNA;
                    }
                }

                if (tasks[count]->missionref)
                {
                    mission = tasks[count]->missionref;
                }
                else if (tasks[count]->missiondef &&
                    tasks[count]->missiondef[0])
                {
                    mission = tasks[count]->missiondef[0];
                }

                if (mission)
                {
                    if (mission->cityzone)
                    {
                        locationstr = mission->cityzone;
                    }

                    if (mission->villaingroup)
                    {
                        villainstr = mission->villaingroup;
                    }

                    if (mission->mapfile)
                    {
                        missionstr = mission->mapfile;
                    }
                    else
                    {
                        missionstr = StaticDefineIntRevLookup(ParseMapSetEnum, mission->mapset);
                    }

                    if (mission->doortype)
                    {
                        doortypestr = mission->doortype;
                    }
                }
                else
                {
                    if (tasks[count]->type == TASK_KILLX)
                    {
                        if (tasks[count]->villainType)
                        {
                            villainstr = tasks[count]->villainType;
                        }

                        if (tasks[count]->villainMap)
                        {
                            locationstr = tasks[count]->villainMap;
                        }
                    }
                    else
                    {
                        if (tasks[count]->villainGroup)
                        {
                            villainstr = textStdUnformattedConst(villainGroupGetPrintName(tasks[count]->villainGroup));
                        }
                    }
                }

                if (tasks[count]->type == TASK_DELIVERITEM ||
                    tasks[count]->type == TASK_CONTACTINTRO)
                {
                    missionstr = saUtilScriptLocalize(tasks[count]->deliveryTargetNames[0]);
                }

                // We have to print the data in two parts because we can't
                // have more than one saUtilScriptLocalize() outstanding.
                estrPrintf(&templine, "%s,%s,%d,%d,%s,%s",
                    header,
                    locationstr,
                    min,
                    max,
                    villainstr,
                    missionstr);

                // If nothing was entered we'll have a NULL, otherwise we'll
                // have a Pstring to be looked up.
                if (title)
                {
                    title = saUtilScriptLocalize(title);
                }
                else
                {
                    if (tasks[count]->logicalname)
                    {
                        title = tasks[count]->logicalname;
                    }
                    else
                    {
                        title = datamineNA;
                    }
                }

                conPrintf(client, "%s,%d,%s,%s,%s,%s\n",
                    templine,
                    firstnum + retval,
                    StaticDefineIntRevLookup(ParseTaskType, tasks[count]->type),
                    title,
                    alliancestr,
                    doortypestr);

                estrDestroy(&templine);

                retval++;
            }
        }
    }

    return retval;
}

static int ContactCountAvailableTasks(const ContactDef* def)
{
    int retval = 0;
    int count;

    if (def)
    {
        if (def->taskIncludes)
        {
            for (count = 0; count < eaSize(&(def->taskIncludes)); count++)
            {
                retval += TaskSetCountTasks(def->taskIncludes[count]->filename);
            }
        }

        if (def->mytasks)
        {
            for (count = 0; count < eaSize(&(def->mytasks)); count++)
            {
                if (!(def->mytasks[count]->deprecated))
                {
                    retval++;
                }
            }
        }
        if (def->storyarcrefs)
        {
            for (count = 0; count < eaSize(&(def->storyarcrefs)); count++)
            {
                retval += StoryArcCountTasks(&def->storyarcrefs[count]->sahandle);
            }
        }
    }

    return retval;
}

void ContactDebugShowAll(ClientLink* client)
{
    const ContactDef** contactlist;
    int i, n;
    int curlevel = 1000;

    // sort all contacts into stature levels
    eaCreateConst(&contactlist);
    for (i = 1; ; i++)
    {
        const ContactDef* def = ContactDefinition(i);
        if (!def) break;
        eaPushConst(&contactlist, def);
    }
    n = eaSize(&contactlist);
    eaQSortConst(contactlist, sortContacts);

    // then display in order
    conPrintf(client, "---%s:\n", saUtilLocalize(client->entity,"AvailContacts"));

    for (i = 0; i < n; i++)
    {
        const ContactDef* def = contactlist[i];
        ContactHandle handle = ContactGetHandle(def->filename);

        if (def->stature != curlevel)
        {
            curlevel = def->stature;
            conPrintf(client, "---%s %i:\n", saUtilLocalize(client->entity,"StatureLevel"), def->stature);
        }
        conPrintf(client, "  %i  %s  (%s)\n", handle, saUtilScriptLocalize(def->displayname), def->statureSet);
    }

    eaDestroyConst(&contactlist);
}

void ContactDebugDatamine(ClientLink* client, bool detailed)
{
    const ContactDef** contactlist;
    int i, n;
    int curlevel = 1000;
    int count;
    int count2;
    int tasknum;
    const StoryTaskSet* taskset;
    const StoryArc* storyarc;
    char* preheader = NULL;
    char* header = NULL;

    // sort all contacts into stature levels
    eaCreateConst(&contactlist);
    for (i = 1; ; i++)
    {
        const ContactDef* def = ContactDefinition(i);
        if (!def) break;
        eaPushConst(&contactlist, def);
    }
    n = eaSize(&contactlist);
    eaQSortConst(contactlist, sortContacts);

    for (i = 0; i < n; i++)
    {
        const ContactDef* def = contactlist[i];

        if (detailed)
        {
            const char* locname;
            bool heroes;
            bool villains;
            bool primals;
            bool praetorians;
            ScriptVarsTable vars = {0};
            ScriptVarsTablePushScope(&vars, &def->vs);

            estrPrintf(&preheader, "%s,\"",
                saUtilTableLocalize(def->displayname, &vars));

            if (def->locationName)
            {
                locname = saUtilScriptLocalize(def->locationName);
                estrConcatCharString(&preheader, locname);
            }
            else
            {
                estrConcatStaticCharArray(&preheader, "N/A");
            }

            estrConcatChar(&preheader, '\"');

            switch (def->alliance)
            {
                case ALLIANCE_NEUTRAL:
                default:
                    heroes = true;
                    villains = true;
                    break;
                case ALLIANCE_HERO:
                    heroes = true;
                    villains = false;
                    break;
                case ALLIANCE_VILLAIN:
                    heroes = false;
                    villains = true;
                    break;
            }

            switch (def->universe)
            {
                case UNIVERSE_BOTH:
                default:
                    primals = true;
                    praetorians = true;
                    break;
                case UNIVERSE_PRIMAL:
                    primals = true;
                    break;
                case UNIVERSE_PRAETORIAN:
                    praetorians = true;
                    break;
            }

            if (def->storyarcrefs)
            {
                for (count = 0; count < eaSize(&(def->storyarcrefs)); count++)
                {
                    storyarc = StoryArcDefinition(&def->storyarcrefs[count]->sahandle);

                    if (storyarc && !STORYARC_IS_DEPRECATED(storyarc))
                    {
                        int min = storyarc->minPlayerLevel;
                        int max = storyarc->maxPlayerLevel;
                        bool archeroes = heroes;
                        bool arcvillains = villains;

                        // The storyarc can be more restrictive than the
                        // contact in terms of its level range, but not more
                        // permissive.
                        if (min < def->minPlayerLevel)
                        {
                            min = def->minPlayerLevel;
                        }

                        if (max > def->maxPlayerLevel)
                        {
                            max = def->maxPlayerLevel;
                        }

                        // Restrict from the defaults.
                        switch (storyarc->alliance)
                        {
                            case SA_ALLIANCE_BOTH:
                            default:
                                break;
                            case SA_ALLIANCE_HERO:
                                arcvillains = false;
                                break;
                            case SA_ALLIANCE_VILLAIN:
                                archeroes = false;
                                break;
                        }

                        tasknum = 1;
                        estrPrintf(&header, "%s,%s", preheader,
                            storyarc->filename);

                        for (count2 = 0; count2 < eaSize(&(storyarc->episodes)); count2++)
                        {
                            tasknum += datamineStoryTasks(client, header,
                                storyarc->episodes[count2]->tasks, tasknum,
                                min, max, archeroes, arcvillains);
                        }

                        estrDestroy(&header);
                    }
                }
            }

            if (def->taskIncludes)
            {
                for (count = 0; count < eaSize(&(def->taskIncludes)); count++)
                {
                    tasknum = 1;
                    taskset = TaskSetFind(def->taskIncludes[count]->filename);

                    if (taskset && taskset->tasks)
                    {
                        estrPrintf(&header, "%s,%s", preheader,
                            taskset->filename);

                        tasknum += datamineStoryTasks(client, header,
                            taskset->tasks, tasknum, def->minPlayerLevel,
                            def->maxPlayerLevel, heroes, villains);

                        estrDestroy(&header);
                    }
                }
            }

            if (def->mytasks)
            {
                estrPrintf(&header, "%s,N/A", preheader);

                tasknum = datamineStoryTasks(client, header, def->mytasks, 1,
                    def->minPlayerLevel, def->maxPlayerLevel, heroes, villains);

                estrDestroy(&header);
            }

            estrDestroy(&preheader);
        }
        else
        {
            conPrintf(client, "%s,%d,%d,%d,%s\n",
                saUtilScriptLocalize(def->displayname),
                def->minPlayerLevel,
                def->maxPlayerLevel,
                ContactCountAvailableTasks(def),
                def->statureSet);
        }
    }

    eaDestroyConst(&contactlist);
}

// Script Generated Contacts need to show up with locations on the map
// This system will let you add contacts on the fly. Right now there they stay for the duration
// mapserver because changing things requires resending the doors to everyone and
// it would be best if we did that as infrequent as possible.
typedef struct ScriptGeneratedContact
{
    char*    contactName;
    Mat4    location;
} ScriptGeneratedContact;

ScriptGeneratedContact**    g_scriptgeneratedcontacts;

// Register a new contact as a PNPC Door with the dbserver
void ContactRegisterScriptContact(char* name, Mat4 location)
{
    ScriptGeneratedContact* newContact = malloc(sizeof(ScriptGeneratedContact));
    newContact->contactName = strdup(name);
    copyMat4(location, newContact->location);
    eaPush(&g_scriptgeneratedcontacts, newContact);
    dbRegisterNewContact(newContact->contactName, newContact->location);
    sendDoorsToDbServer(db_state.is_static_map);
}

// Add all the contacts to the local door list
void ContactGatherScriptContacts()
{
    int i, n = eaSize(&g_scriptgeneratedcontacts);
    for (i = 0; i < n; i++)
        dbRegisterNewContact(g_scriptgeneratedcontacts[i]->contactName, g_scriptgeneratedcontacts[i]->location);
}

void ContactMarkAllDirty(Entity *e, StoryInfo* info)
{
    StoryContactInfo **contacts = info->contactInfos;
    int nContacts = eaSize(&contacts);
    int i;

    for(i = 0; i < nContacts; i++)
        ContactMarkDirty(e, info, contacts[i]);
}

void ContactGetValidList(Entity *e, StoryContactInfo ***contactsDst, StoryContactInfo ***contactsSrc)
{
    int nContacts = eaSize(contactsSrc);
    int i;

    eaSetSize(contactsDst, 0);

    for(i=0;i<nContacts;i++)
    {
        int valid = 0;
        const ContactDef *cdef = (*contactsSrc)[i]->contact->def;

        //    very stupid hack for now. on endgame raids, no contacts are valid
        //    this will have to change if we want to show a contact during these raids
        valid = !db_state.is_endgame_raid && 
            (!cdef->interactionRequires || chareval_Eval(e->pchar, cdef->interactionRequires, cdef->filename) && !((*contactsSrc)[i]->deleteMe));

        if(valid)
            eaPush(contactsDst, (*contactsSrc)[i]);
    }
}

void ContactDebugShow(ClientLink* client, StoryInfo* info, StoryContactInfo* contactInfo, int detailed)
{
    char * estr = NULL;
    estrClear(&estr);
    ContactDebugShowHelper(&estr, client, info, contactInfo, detailed);
    conPrintf(client, "%s", estr);
    estrDestroy(&estr);
}

void ContactDebugShowHelper(char ** estr, ClientLink * client, StoryInfo* info, StoryContactInfo* contactInfo, int detailed)
{
    int i, n;
    char contactName[1024];
    char contactRelationship[1024];

    if (!contactInfo)
        return; // this can happen if you're on a tip mission map after completing the mission (and the tip has been dismissed)

    // basic info line
    strcpy(contactName, ContactDisplayName(contactInfo->contact->def, client->entity));
    strcpy(contactRelationship, ContactRelationshipGetName(client->entity, contactInfo));

    estrConcatf(estr, "%s (%i, %s) CXP: %i [%s] %s %s %s\n",
        contactName,
        contactInfo->handle,
        contactInfo->contact->def->statureSet,
        contactInfo->contactPoints,
        contactRelationship,
        contactInfo->notifyPlayer? saUtilLocalize(client->entity,"WantsToSeeYa"): "",
        ContactCorrectAlliance(client->entity->pl->playerType, contactInfo->contact->def->alliance) ? "" : saUtilLocalize(client->entity,"Escrowed"),
        ContactCorrectUniverse(client->entity->pl->praetorianProgress, contactInfo->contact->def->universe) ? "" : saUtilLocalize(client->entity,"WrongUniverse"));
    if (!detailed) return;

    // task details
    n = eaSize(&contactInfo->contact->def->tasks);
    for (i = 0; i < n; i++)
    {
        const StoryTask* taskdef = contactInfo->contact->def->tasks[i];
        TaskSubHandle taskhandle = StoryIndexToHandle(i);
        if (taskdef->deprecated) continue;
        estrConcatf(estr, "\t%s %s\n\t\t%s\n",
            TaskDisplayType(taskdef),
            BitFieldGet(contactInfo->taskIssued, TASK_BITFIELD_SIZE, i)? saUtilLocalize(client->entity,"BracketIssued"): "",
            TaskDebugName(tempStoryTaskHandle(contactInfo->handle, taskhandle, 0, 0), 0));
    }

    // storyarc details
    n = eaSize(&contactInfo->contact->def->storyarcrefs);
    for (i = 0; i < n; i++)
    {
        estrConcatf(estr, "\t%s %s\n",
            contactInfo->contact->def->storyarcrefs[i]->filename,
            BitFieldGet(contactInfo->storyArcIssued, STORYARC_BITFIELD_SIZE, i)? saUtilLocalize(client->entity,"BracketIssued"): "");
    }
    if (n == 0)
    {
        estrConcatf(estr, "\t%s\n", saUtilLocalize(client->entity,"NoStoryArcs"));
    }
}

void ContactDebugShowMine(ClientLink* client, Entity* e)
{
    char * estr = NULL;
    estrClear(&estr);
    ContactDebugShowMineHelper(&estr, client, e);
    conPrintf(client, "%s", estr);
    estrDestroy(&estr);
}

void ContactDebugShowMineHelper(char ** estr, ClientLink * client, Entity * e)
{
    int i, n;
    StoryInfo* info;

    estrConcatCharString(estr, saUtilLocalize(e,"ContactDash"));
    info = StoryInfoFromPlayer(e);
    n = eaSize(&info->contactInfos);
    for (i = 0; i < n; i++)
    {
        estrConcatStaticCharArray(estr, "\n\t");
        ContactDebugShowHelper(estr, client, info, info->contactInfos[i], 0);
    }
    if (i == 0) estrConcatf(estr, "\t%s\n", saUtilLocalize(0,"NoneParen"));
}

void ContactDebugShowDetail(ClientLink* client, Entity* e, char* filename)
{
    const char* name;
    StoryInfo* info;
    StoryContactInfo* contactInfo;
    ContactHandle chandle = ContactGetHandleLoose(filename);

    if (!chandle)
    {
        conPrintf(client,localizedPrintf(0,"CouldntFindContact", filename));
        return;
    }

    info = StoryInfoFromPlayer(e);
    contactInfo = ContactGetInfo(info, chandle);
    if (!contactInfo)
    {
        name = ContactDisplayName(ContactDefinition(chandle), e);
        conPrintf(client, saUtilLocalize(e,"YouDontHaveContact"), name);
        return;
    }

    ContactDebugShow(client, info, contactInfo, 1);
}

void ContactDebugAdd(ClientLink* client, Entity* e, const char* filename)
{
    const char* name;
    StoryInfo* info;
    ContactHandle handle = ContactGetHandleLoose(filename);
    StoryContactInfo* contactInfo;

    if (!handle)
    {
        conPrintf(client, localizedPrintf(0,"CouldntFindContact", filename));
        return;
    }

    if (PlayerInTaskForceMode(client->entity))
    {
        conPrintf(client, saUtilLocalize(client->entity, "CantGetTaskWhileInTaskforceMode"));
        return;
    }

    info = StoryArcLockInfo(e);
    name = ContactDisplayName(ContactDefinition(handle), e);
    contactInfo = ContactGetInfo(info, handle);
    if (contactInfo)
    {
        conPrintf(client, "%s\n", saUtilLocalize(e,"AlreadyHaveContact",name));
    }
    else if (name)
    {
        conPrintf(client, "%s\n", saUtilLocalize(e,"AddingContact",name, handle));
        if (!ContactAdd(e, info, handle, "debug command"))
            conPrintf(client, "%s\n", saUtilLocalize(e,"Unsuccess"));
        else
        {
            const ContactDef* contactdef = ContactDefinition(handle);
            if (contactdef->requiredBadge && !badge_OwnsBadge(e, contactdef->requiredBadge))
                badge_Award(e, contactdef->requiredBadge, 1);
        }
    }
    else
    {
        conPrintf(client, "%s\n", saUtilLocalize(e,"InvalidContact"));
    }
    StoryArcUnlockInfo(e);
}

void ContactDebugAddAll(ClientLink* client, Entity* e)
{
    const ContactDef** contactlist;
    int i, n;
    int curlevel = 1000;

    // sort all contacts into stature levels
    eaCreateConst(&contactlist);
    for (i = 1; ; i++)
    {
        const ContactDef* def = ContactDefinition(i);
        if (!def) break;
        eaPushConst(&contactlist, def);
    }
    n = eaSize(&contactlist);
    eaQSortConst(contactlist, sortContacts);

    // then add in order
    for (i = 0; i < n; i++)
    {
        const ContactDef* def = contactlist[i];
        ContactHandle handle = ContactGetHandle(def->filename);
        ContactDebugAdd(client, e, def->filename);
    }

    eaDestroyConst(&contactlist);
}

void ContactGoto(ClientLink* client, Entity* e, const char* filename, int select)
{
    ContactHandle contact = ContactGetHandleLoose(filename);
    char buf[1000];
    Vec3 pos;
    int mapID;
    int haveloc = 0;

    if (!contact)
    {
        conPrintf(client, localizedPrintf(0,"CouldntFindContact", filename));
        return;
    }
    haveloc = ContactFindLocation(contact, &mapID, &pos);
    if (!haveloc)
    {
        conPrintf(client, "%s\n", saUtilLocalize(e,"LostContact",filename) );
        return;
    }

    // use teleport helper function
    if(mapID == db_state.base_map_id)
        mapID = db_state.map_id;

    if (select)
    {
        sprintf(buf,"mapmoveposandselectcontact \"%s\" %d %f %f %f 0 1 %s",e->name,mapID,pos[0],pos[1],pos[2],filename);
    }
    else
    {
        sprintf(buf,"mapmovepos \"%s\" %d %f %f %f 0 1",e->name,mapID,pos[0],pos[1],pos[2]);
    }
    serverParseClientEx(buf, client, ACCESS_INTERNAL);
}

void ContactDebugSetCxp(ClientLink* client, Entity* player, char* contactfile, int cxp)
{
    ContactHandle handle = ContactGetHandleLoose(contactfile);
    StoryInfo* info;
    StoryContactInfo* contactInfo;

    if (!handle)
    {
        conPrintf(client, localizedPrintf(0,"CouldntFindContact", contactfile));
        return;
    }
    info = StoryArcLockInfo(player);
    contactInfo = ContactGetInfo(info, handle);
    if (!contactInfo)
    {
        conPrintf(client, "You don't have contact %s (use addcontact)\n", contactfile);
        StoryArcUnlockInfo(player);
        return;
    }
    contactInfo->contactPoints = cxp;
    ContactDetermineRelationship(contactInfo); // player won't get change strings because we're changing directly
    ContactMarkDirty(player, info, contactInfo);
    StoryArcUnlockInfo(player);
    conPrintf(client, "Cxp set to %i", cxp);
}

void ContactDebugOutputFileForFlowchart(void)
{
    FILE* f;
    char buf[100000];
    int contactIndex, secondIndex;//, temp;
    int contactCount = eaSize(&g_contactlist.contactdefs);
    int done = false;

    f = fopen("contactFlowchartInfo.txt", "w");
    if (!f) 
        return;

    sprintf(buf, "digraph Contacts {\n");
    fwrite(buf, 1, strlen(buf), f);

//     sprintf(buf, "\tIssueOnInteract0 [label=\"Issue on Interact\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);

//     for (temp = 1; temp <= 55; temp++)
//     {
//         sprintf(buf, "\tIssueOnInteract%i [label=\"Issue on Interact: Min Level %i\", shape=box]\n", temp, temp);
//         fwrite(buf, 1, strlen(buf), f);
//         sprintf(buf, "\tIssueOnInteract%i -> IssueOnInteract%i\n", temp - 1, temp);
//         fwrite(buf, 1, strlen(buf), f);
//     }

//     sprintf(buf, "\tAlignmentTip [label=\"Alignment Tip\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tHeroVigAlignmentTip [label=\"Hero-Vig\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
//     sprintf(buf, "\tAlignmentTip -> HeroVigAlignmentTip\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tHeroVig20AlignmentTip [label=\"20\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
//     sprintf(buf, "\tHeroVigAlignmentTip -> HeroVig20AlignmentTip\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tHeroVig30AlignmentTip [label=\"30\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
//     sprintf(buf, "\tHeroVig20AlignmentTip -> HeroVig30AlignmentTip\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tHeroVig40AlignmentTip [label=\"40\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
//     sprintf(buf, "\tHeroVig30AlignmentTip -> HeroVig40AlignmentTip\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tVilRogAlignmentTip [label=\"Vil-Rog\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
//     sprintf(buf, "\tAlignmentTip -> VilRogAlignmentTip\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tVilRog20AlignmentTip [label=\"20\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
//     sprintf(buf, "\tVilRogAlignmentTip -> VilRog20AlignmentTip\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tVilRog30AlignmentTip [label=\"30\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
//     sprintf(buf, "\tVilRog20AlignmentTip -> VilRog30AlignmentTip\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tVilRog40AlignmentTip [label=\"40\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
//     sprintf(buf, "\tVilRog30AlignmentTip -> VilRog40AlignmentTip\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tVigToVilAlignmentTip [label=\"Vig To Vil\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
//     sprintf(buf, "\tAlignmentTip -> VigToVilAlignmentTip\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tVigToVil20AlignmentTip [label=\"20\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
//     sprintf(buf, "\tVigToVilAlignmentTip -> VigToVil20AlignmentTip\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tVigToVil30AlignmentTip [label=\"30\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
//     sprintf(buf, "\tVigToVil20AlignmentTip -> VigToVil30AlignmentTip\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tVigToVil40AlignmentTip [label=\"40\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
//     sprintf(buf, "\tVigToVil30AlignmentTip -> VigToVil40AlignmentTip\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tRogToHeroAlignmentTip [label=\"Rog To Hero\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
//     sprintf(buf, "\tAlignmentTip -> RogToHeroAlignmentTip\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tRogToHero20AlignmentTip [label=\"20\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
//     sprintf(buf, "\tRogToHeroAlignmentTip -> RogToHero20AlignmentTip\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tRogToHero30AlignmentTip [label=\"30\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
//     sprintf(buf, "\tRogToHero20AlignmentTip -> RogToHero30AlignmentTip\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tRogToHero40AlignmentTip [label=\"40\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
//     sprintf(buf, "\tRogToHero30AlignmentTip -> RogToHero40AlignmentTip\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tMoralityTip [label=\"Morality Tip\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tSpecialTip [label=\"Special Tip\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);
// 
//     sprintf(buf, "\tHalloweenTip [label=\"Halloween Tip\", shape=box]\n");
//     fwrite(buf, 1, strlen(buf), f);

    for (contactIndex = 0; contactIndex < contactCount; contactIndex++)
    {
        const ContactDef *contactDef = g_contactlist.contactdefs[contactIndex];

        done = false;

        if (!contactDef->tasks || !eaSize(&contactDef->tasks) 
            || contactDef->tipType != TIPTYPE_NONE || CONTACT_IS_BROKER(contactDef))
            continue;

        sprintf(buf, "\t\"%s\" [label=\"%s\"];\n", contactDef->filename, saUtilScriptLocalize(contactDef->displayname));
        fwrite(buf, 1, strlen(buf), f);

        if (0) // stupid hack
        {
        }
//         else if (contactDef->tipType == TIPTYPE_ALIGNMENT)
//         {
//             if (strstri(contactDef->filename, "Hero-Vig"))
//             {
//                 if (contactDef->minPlayerLevel == 20
//                     || contactDef->minPlayerLevel == 30
//                     || contactDef->minPlayerLevel == 40)
//                 {
//                     sprintf(buf, "\tHeroVig%iAlignmentTip -> \"%s\";\n", contactDef->minPlayerLevel, contactDef->filename);
//                     fwrite(buf, 1, strlen(buf), f);
//                     done = true;
//                 }
//             }
//             else if (strstri(contactDef->filename, "Vil-Rog"))
//             {
//                 if (contactDef->minPlayerLevel == 20
//                     || contactDef->minPlayerLevel == 30
//                     || contactDef->minPlayerLevel == 40)
//                 {
//                     sprintf(buf, "\tVilRog%iAlignmentTip -> \"%s\";\n", contactDef->minPlayerLevel, contactDef->filename);
//                     fwrite(buf, 1, strlen(buf), f);
//                     done = true;
//                 }
//             }
//             else if (strstri(contactDef->filename, "VigToVil"))
//             {
//                 if (contactDef->minPlayerLevel == 20
//                     || contactDef->minPlayerLevel == 30
//                     || contactDef->minPlayerLevel == 40)
//                 {
//                     sprintf(buf, "\tVigToVil%iAlignmentTip -> \"%s\";\n", contactDef->minPlayerLevel, contactDef->filename);
//                     fwrite(buf, 1, strlen(buf), f);
//                     done = true;
//                 }
//             }
//             else if (strstri(contactDef->filename, "RogToHero"))
//             {
//                 if (contactDef->minPlayerLevel == 20
//                     || contactDef->minPlayerLevel == 30
//                     || contactDef->minPlayerLevel == 40)
//                 {
//                     sprintf(buf, "\tRogToHero%iAlignmentTip -> \"%s\";\n", contactDef->minPlayerLevel, contactDef->filename);
//                     fwrite(buf, 1, strlen(buf), f);
//                     done = true;
//                 }
//             }
// 
//             if (!done)
//             {
//                 sprintf(buf, "\tAlignmentTip -> \"%s\";\n", contactDef->filename);
//                 fwrite(buf, 1, strlen(buf), f);
//                 done = true;
//             }
//         }
//         else if (contactDef->tipType == TIPTYPE_MORALITY)
//         {
//             sprintf(buf, "\tMoralityTip -> \"%s\";\n", contactDef->filename);
//             fwrite(buf, 1, strlen(buf), f);
//         }
//         else if (contactDef->tipType == TIPTYPE_SPECIAL)
//         {
//             sprintf(buf, "\tSpecialTip -> \"%s\";\n", contactDef->filename);
//             fwrite(buf, 1, strlen(buf), f);
//         }
//         else if (contactDef->tipType == TIPTYPE_HALLOWEEN)
//         {
//             sprintf(buf, "\tHalloweenTip -> \"%s\";\n", contactDef->filename);
//             fwrite(buf, 1, strlen(buf), f);
//         }
        else if (CONTACT_IS_ISSUEONZONEENTER(contactDef))
        {
            sprintf(buf, "\tIssueOnZoneEnter%i -> \"%s\";\n", contactDef->minPlayerLevel, contactDef->filename);
            fwrite(buf, 1, strlen(buf), f);
        }
        else if (CONTACT_IS_ISSUEONINTERACT(contactDef))
        {
            sprintf(buf, "\tIssueOnInteract%i -> \"%s\";\n", contactDef->minPlayerLevel, contactDef->filename);
            fwrite(buf, 1, strlen(buf), f);
        }
        else
        {
            // find all friendly introductions?
            // find all complete introductions
            if (contactDef->statureSet && stricmp(contactDef->statureSet, "None"))
            {
                for (secondIndex = 0; secondIndex < contactCount; secondIndex++)
                {
                    const ContactDef *secondDef = g_contactlist.contactdefs[secondIndex];
                    if (contactIndex != secondIndex && secondDef->nextStatureSet
                        && !stricmp(contactDef->statureSet, secondDef->nextStatureSet)
                        && stricmp(contactDef->statureSet, secondDef->statureSet)
                        && !CONTACT_IS_NOCOMPLETEINTRO(secondDef))
                    {
                        sprintf(buf, "\t\"%s\" -> \"%s\";\n", secondDef->filename, contactDef->filename);
                        fwrite(buf, 1, strlen(buf), f);
                    }
                }
            }
        }
    }

    sprintf(buf, "}\n");
    fwrite(buf, 1, strlen(buf), f);

    fclose(f);
}

int ContactDebugPlayerHasFinishedTask(Entity *e, char *fileName, char *taskName)
{
    StoryInfo *info;
    int i, n;
    int k, taskCount;

    info = StoryInfoFromPlayer(e);
    
    if (!info)
        return false;

    // checking to see if they're already on the mission
    taskCount = eaSize(&info->tasks);
    for (i = 0; i < taskCount; i++)
    {
        if (info->tasks[i] && info->tasks[i]->def && 
            info->tasks[i]->def->logicalname != NULL &&
            info->tasks[i]->def->filename != NULL &&
            stricmp(info->tasks[i]->def->logicalname, taskName) == 0 &&
            stricmp(info->tasks[i]->def->filename, fileName) == 0)
            return false;
    }

    // checking to see if it's already been issued
    n = eaSize(&info->contactInfos);
    for (i = 0; i < n; i++)
    {
        StoryContactInfo *scInfo = ContactGetInfo(info, info->contactInfos[i]->handle);

        taskCount = eaSize(&scInfo->contact->def->tasks);
        for (k = 0; k < taskCount; k++)
        {
            if (scInfo->contact->def->tasks[k]->logicalname != NULL &&
                scInfo->contact->def->tasks[k]->filename != NULL &&
                stricmp(scInfo->contact->def->tasks[k]->logicalname, taskName) == 0 &&
                stricmp(scInfo->contact->def->tasks[k]->filename, fileName) == 0)
            {
                if (BitFieldGet(scInfo->taskIssued, TASK_BITFIELD_SIZE, k))
                    return true;
            }
        }
    }
    return false;
}


int ContactPlayerHasContact(Entity *e, char *contactName)
{
    ContactHandle handle;
    StoryInfo *info;
    StoryContactInfo *contactInfo;
    
    handle = ContactGetHandle(contactName);
    if (handle == 0)
    {
        return 0;
    }
    if (PlayerInTaskForceMode(e))
    {
        return 0;
    }
    info = StoryInfoFromPlayer(e);
    contactInfo = ContactGetInfo(info, handle);
    return contactInfo != NULL;
}

