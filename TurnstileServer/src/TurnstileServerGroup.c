// TurnstileServerGroup.c
//

#include <utilitieslib/network/netio.h>
#include "malloc.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/components/genericlist.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/mathutil.h>
#include "entity/teamCommon.h"
#include "chatdefs.h"
#include "turnstile/TurnstileServerCommon.h"
#include "TurnstileServerMsg.h"
#include "TurnstileServerEvent.h"
#include "TurnstileServerGroup.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/log.h>

static StashTable s_queuedGroups;
static QueueGroup *s_turnstileQueueHead;
static QueueGroup *s_turnstileQueueTail;
static QueueGroup *s_freeList;
static QueueGroup *s_updateGroup;

void GroupInit()
{
    s_queuedGroups = stashTableCreateInt(256);
    s_turnstileQueueHead = NULL;
    s_turnstileQueueTail = NULL;
    s_freeList = NULL;
}

// I ought to use a memory pool for this, however I'm lazy and just use a classic freelist approach.
QueueGroup *GroupAlloc()
{
    QueueGroup *group;
    if (s_freeList)
    {
        group = s_freeList;
        s_freeList = s_freeList->next;
    }
    else
    {
        group = malloc(sizeof(QueueGroup));
    }

    // In practice, I suspect we'll be unlikely to run out of memory here.  sizeof(QueueGroup) is around 20K,
    // the big item being the ignorelists at 48 players * 100 entries * 4 bytes per entry == 19,200 bytes
    // At a maximum concurrency of 20,000 players, each playing solo, all queued at the same moment we're
    // looking at 400 MB of data all told.

    // If we ever bump the ignorelist size above 200 or the league size above 64, this static assert will trip,
    // in which case you're headed for a small to medium rewrite.  Bottom line is that you'll need to be a bit
    // smarter about allocating this struct: I'd suggest individual mallocs / earrays to keep the memory use to
    // the absolute minimum, either that or compute the size of the chunk needed and allocate as one blob, and
    // then do the necessary shenanigens to set up the pointers.  In either instance, you'll want to alter this
    // to take the necessary size data as parameters.  Since this is only called one place in the code, it
    // shouldn't be hideously painful.
    STATIC_INFUNC_ASSERT(MAX_IGNORES <= 200 && MAX_LEAGUE_MEMBERS <= 64);

    if (group)
    {
        memset(group, 0, sizeof(QueueGroup));
    }
    return(group);
}

void GroupDestroy(QueueGroup *group)
{
    devassert(!group->inFreeListQueue);
    if (!group->inFreeListQueue)
    {
        group->inFreeListQueue = 1;
        eaiDestroy(&group->ignoreAuths);
        group->next = s_freeList;
        s_freeList = group;
    }
}

void GroupAddToQueue(QueueGroup *group, int insertAtHead)
{
    int i;
    QueueGroup *existingGroup;

    if (stashIntFindPointer(s_queuedGroups, group->owner_dbid, &existingGroup))
    {
        // This is okay now!
        // This occurs if the following occurs:
        // - A team joins the turnstile queue.
        // - An event is started that contains that team.
        // - Part of the team accepts, but the team leader declines the event.
        // - The event doesn't start due to not having enough players.
        // - (At this point, the turnstile group's member list contains the players that
        //     accepted the first event, but not the players that declined it.
        //     The team's member list contains both sets of players.)
        // - Before the remainder of the team receives an event, the team leader queues again.
        GroupRemoveGroupFromQueue(existingGroup->owner_dbid, existingGroup->link, false);
    }

    if (!stashIntAddPointer(s_queuedGroups, group->owner_dbid, group, 0))
    {
        GroupDestroy(group);
        // This shouldn't be possible... should've fixed it above.
        return;
    }

    //    build an ignore list for this group
    for (i = 0; i < group->numPlayer; ++i)
    {
        int j;
        for (j = 0; j < group->playerList[i].numIgnores; ++j)
        {
            eaiSortedPushUnique(&group->ignoreAuths, group->playerList[i].ignoredAuthId[j]);
        }
    }

    if (insertAtHead)
    {
        //    if inserting at head, don't reset the joinTime since this group had a failed start
        // we maintain a separate tail pointer.
        if (s_turnstileQueueTail == NULL)
        {
            s_turnstileQueueTail = group;
        }
        listAddForeignMember(&s_turnstileQueueHead, group);
    }
    else
    {
        group->joinTime = timerSecondsSince2000();
        if (!s_turnstileQueueHead)
        {
            //    empty list
            s_turnstileQueueTail = s_turnstileQueueHead = group;
            group->prev = group->next = NULL;
        }
        else
        {
            devassert(s_turnstileQueueTail);
            if (s_turnstileQueueTail)
            {
                s_turnstileQueueTail->next = group;
                group->prev = s_turnstileQueueTail;
                group->next = NULL;
                s_turnstileQueueTail = group;
            }
        }
    }

    // Inform the entire group they're queued
    turnstileServer_generateQueueStatus(group, 0, 1, NULL);
}

void GroupRemoveGroupFromQueue(int owner_dbid, NetLink *link, bool generateStatus)
{
    QueueGroup *group;
    if (stashIntRemovePointer(s_queuedGroups, owner_dbid, &group))
    {
        if (generateStatus)
        {
            // Inform the entire group they're no longer queued
            turnstileServer_generateQueueStatus(group, 0, 0, NULL);
        }

        if (group->parentInstance)
        {
            MissionInstance *instance = group->parentInstance;
            listRemoveMember(group, &instance->queuedGroups);
        }
        else
        {
            // Adjust tail if needed.
            if (group == s_turnstileQueueTail)
            {
                s_turnstileQueueTail = group->prev;
            }

            listRemoveMember(group, &s_turnstileQueueHead);
        }
        GroupDestroy(group);
    }
    else
    {
        if (generateStatus)
        {
            turnstileServer_generateQueueStatus(NULL, owner_dbid, 0, link);
        }
    }
}

void GroupRemovePlayerFromGroup(int owner_dbid, int player_dbid, NetLink *link)
{
    int i;
    QueueGroup *group;

    if (stashIntFindPointer(s_queuedGroups, owner_dbid, &group))
    {
        for (i = 0; i < group->numPlayer; i++)
        {
            if (group->playerList[i].db_id == player_dbid)
            {
                if (group->parentInstance)
                {
                    if (group->playerList[i].eventReadyFlags & EVENT_READY_ACCEPTED)
                    {
                        group->parentInstance->numAccept--;
                    }
                    group->parentInstance->numPlayer--;
                }

                // Order doesn't matter, so we can just copy the last to here
                if (i < group->numPlayer - 1)
                {
                    group->playerList[i] = group->playerList[group->numPlayer - 1];
                }
                group->numPlayer--;
                // Tell this player they're out of the queue
                turnstileServer_generateQueueStatus(group, player_dbid, 0, NULL);
                break;
            }
        }
        
        if (group->numPlayer == 0)
        {
            GroupRemoveGroupFromQueue(owner_dbid, NULL, true);
        }
    }
    else
    {
        turnstileServer_generateQueueStatus(NULL, player_dbid, 0, link);
    }
}

void GroupChangeGroupOwner(int old_owner_dbid, int new_owner_dbid)
{
    QueueGroup *group;

    if (stashIntRemovePointer(s_queuedGroups, old_owner_dbid, &group))
    {
        group->owner_dbid = new_owner_dbid;
        if (!stashIntAddPointer(s_queuedGroups, new_owner_dbid, group, 0))
        {
            // TODO report the error to the group members
            GroupDestroy(group);
        }
        else
        {
            turnstileServer_generateQueueStatus(group, new_owner_dbid, 1, NULL);
        }
    }
}


QueueGroup *GroupExtractGroupFromQueue(int owner_dbid)
{
    QueueGroup *group;

    if (stashIntFindPointer(s_queuedGroups, owner_dbid, &group))
    {    
        if (group->parentInstance)
        {
            MissionInstance *instance = group->parentInstance;
            listRemoveMember(group, &instance->queuedGroups);
            stashIntRemovePointer(s_queuedGroups, owner_dbid, NULL);
        }
        else
        {
            if (group == s_turnstileQueueTail)
            {
                s_turnstileQueueTail = group->prev;
            }
            listRemoveMember(group, &s_turnstileQueueHead);
        }
    }
    else
    {
        group = NULL;
    }
    return group;
}

QueueGroup *GroupFindGroupByOwner(int owner_dbid)
{
    QueueGroup *group;

    stashIntFindPointer(s_queuedGroups, owner_dbid, &group);
    return group;
}

bool GroupIsPlayerInGroup(QueueGroup *group, int player_dbid)
{
    int i;

    if (!group)
        return false;

    for (i = 0; i < group->numPlayer; i++)
    {
        if (group->playerList[i].db_id == player_dbid)
        {
            return true;
        }
    }

    return false;
}

// Glue function used by both of the next two.  A player is identified by an (instanceID, owner_dbidm, player_dbid)
// triple.  Both of these guys need the QueuedPlayer * for the player, this tracks it down.
static QueuedPlayer *FindInstanceAndPlayer(U32 instanceID, int owner_dbid, int player_dbid, MissionInstance **pInstance)
{
    int i;
    QueueGroup *group;

    // Rip up through all groups in this instance, looking for the one with the selected owner_dbid
    stashIntFindPointer(s_queuedGroups, owner_dbid, &group);
    if (pInstance)
        *pInstance = TurnstileServerEventFindInstanceByID(instanceID);
    if (group)
    {
        for (i = 0; i < group->numPlayer; i++)
        {
            if (group->playerList[i].db_id == player_dbid)
            {
                break;
            }
        }
        if (i == group->numPlayer)
        {
            // TDDO - complain here, this is an error.  Not that there's anything we can do about it.
            return NULL;
        }
        return &group->playerList[i];
    }
    return NULL;
}

// This handles the ack generated by the client to inform us it's aware that the event has to start.  If this doesn't arrive within
// five seconds, the event ready message is sent again.  This is designed to cover the case where the event ready gets lost in
// transit because the player was in the middle of a mapmove at the time.
void GroupEventReadyAck(U32 instanceID, int owner_dbid, int player_dbid)
{
    QueuedPlayer *queuedPlayer;

    queuedPlayer = FindInstanceAndPlayer(instanceID, owner_dbid, player_dbid, NULL);
    if (queuedPlayer != NULL)
    {
        queuedPlayer->eventReadyFlags |= EVENT_READY_ACKED;
        queuedPlayer->eventReadyTimer = timerSecondsSince2000();
    }
}

// This handles the final response from the player.  We do not actually do anything here other than flag what the response was.
// Determination of what stage the instance is in is done in the second half of GroupTick()
void GroupEventResponse(U32 instanceID, int owner_dbid, int player_dbid, int accept)
{
    QueuedPlayer *queuedPlayer;
    MissionInstance *instance;

    instance = NULL;
    queuedPlayer = FindInstanceAndPlayer(instanceID, owner_dbid, player_dbid, &instance);
    if (queuedPlayer != NULL)
    {
        queuedPlayer->eventReadyFlags |= accept ? EVENT_READY_RESPONDED | EVENT_READY_ACCEPTED : EVENT_READY_RESPONDED;
    }
    if (instance)
    {
        QueueGroup *group;
        if (accept)
        {
            instance->numAccept++;
            if (queuedPlayer != NULL && queuedPlayer->iLevel > instance->maxLevel)
            {
                // Keep track of the maximum player level of accepting players
                instance->maxLevel = queuedPlayer->iLevel;
            }
        }
        else
        {
            instance->numDeclined++;
        }
        if (instance->numAccept)
        {
            //    send a packet out
            for (group = instance->queuedGroups; group; group = group->next)
            {
                int j;
                for (j = 0; j < group->numPlayer; j++)    
                {
                    if (group->playerList[j].eventReadyFlags & EVENT_READY_ACCEPTED)
                    {
                        turnstileServer_generateEventReadyAccept(group, instance, j);
                    }
                }
            }
        }
    }
}

// This handles the mapID packet originally created by the mapserver.  We just drop the mapID into the instance.
// GroupTick will see this and punt people onto the map.
void GroupMapID(U32 instanceID, int mapID, Vec3 pos)
{
    MissionInstance *instance;
    if (instance = TurnstileServerEventFindInstanceByID(instanceID))
    {
        int i;
        instance->mapId = mapID;
        for (i = 0; i < 3; i++)
        {
            instance->pos[i] = pos[i];
        }
    }
}
static int GroupIgnoreListsCollide(QueueGroup **groups, int numGroup, QueueGroup *newGroup, MissionInstance *instance )
{
    int i;
    for (i = 0; i < numGroup; ++i)
    {
        int j;
        QueueGroup *curGroup = groups[i];
        for (j = 0; j < newGroup->numPlayer; ++j)
        {
            if (eaiSortedFind(&curGroup->ignoreAuths, newGroup->playerList[j].auth_id) != -1)
            {
                return 1;
            }
        }
        for (j = 0; j < curGroup->numPlayer; ++j)
        {
            if (eaiSortedFind(&newGroup->ignoreAuths, curGroup->playerList[j].auth_id) != -1)
            {
                return 1;
            }
        }
    }
    if (instance && eaiSize(&instance->banIDList))
    {
        for (i = 0; i < newGroup->numPlayer; ++i)
        {
            if (eaiSortedFind(&instance->banIDList, newGroup->playerList[i].auth_id) != -1)
            {
                return 1;
            }
        }
    }
    return 0;
}
static void GroupAddGroupsToInstance(TurnstileMission *currentMission, MissionInstance *currentInstance)
{
    int i;
    int groupSeed;                                    // Seed used to select groups from the initial subset of 8
    int bestCount = 0;                                // Best non-maximum count we've seen so far
    int bestNumGroup;
    QueueGroup *groups[MAX_LEAGUE_MEMBERS];            // Group list being assembled based on current seed
    QueueGroup *bestGroups[MAX_LEAGUE_MEMBERS];        // Groups that form the best set
    U32 now = timerSecondsSince2000();
    // This is a loosened version of the knapsack problem.  In the classic knapsack, the target is a single integer.
    // In our case, the target is a range of integers, with the proviso that attaing the maximum value in the solution
    // set is vastly preferable, since it permits an event to start without waiting.

    // Rather than try every possible arrangement, which is O(2 ^ n), we take a small group from the start of the queue,
    // try all possible subsets of that group, and then do a linear time "lazy fill" over the rest of the data.  Very
    // arbitrarily, the initial setsize is 8, meaning 255 non-empty subsets to try.  These can easily be iterated over
    // by counting from 1 to 255.

    // In practice, this counts in reverse: 255 to 1, and uses the bits in reverse order, meaning bit 7 corresponds to
    // the tail of the queue, i.e. the group that have been queued the longest.  This will tend to favor forming a group
    // that uses them.

    for (groupSeed = 255; groupSeed >= 1; groupSeed--)
    {
        // Set seed test bit one spot to the left, we update at the top of the loop so it'll land in the right place
        int seedTestBit = 256;                                // Bit we actually care about in the above    
        int numGroup = 0;
        int queued = 0;
        QueueGroup *group;

        for (group = s_turnstileQueueHead; group; group = group->next)
        {
            seedTestBit = seedTestBit >> 1;
            for (i = 0; i < group->numEvent; i++)
            {
                if (group->eventList[i] == currentMission->missionID)
                {
                    // This group is interested in this event
                    break;
                }
            }
            // If we get here and this test succeeds, the group isn't interested in the current event
            if (i >= group->numEvent)
            {
                continue;
            }
            // DGNOTE TODO - Take some time to think about moving the
            // seedTestBit = seedTestBit >> 1;
            // line down to here.  My reason for considering this is that it might be wise to do the bit iteration & selection only on 
            // groups that actually care about this event.

            // See if the appropriate bit for this group is set, if not skip them for now.  Once seedTestBit
            // gets shifted to zero we're done with the initial selection, in which case this test turns itself off
            if (seedTestBit && (groupSeed & seedTestBit) == 0)
            {
                continue;
            }
            // If this group puts us over the max allowed, skip them
            if (queued + group->numPlayer > currentMission->maximumPlayers)
            {
                continue;
            }

            //    we are refilling an instance, make sure the players are willing to join
            if (currentInstance)
            {
                if ((group->flags & TS_GROUP_FLAGS_WILLING_TO_SUB) != TGS_WILLING)
                {
                    continue;
                }
                if ((group->flags & TS_GROUP_FLAGS_WANTS_TO_OWN_GROUP))
                {
                    continue;
                }
            }
            else
            {
                if (group->flags & TS_GROUP_FLAGS_WANTS_TO_OWN_GROUP)
                {
                    if (numGroup)
                    {
                        //    this group wants to own the entire event, don't put them with others
                        continue;
                    }
                }

                //    this sanity check makes sure if there is any desync between the map and the turnstile server, the group doesn't get put w/ others
                if (numGroup)
                {
                    if (groups[0]->flags & TS_GROUP_FLAGS_WANTS_TO_OWN_GROUP)
                    {
                        continue;
                    }
                }
            }
            if (GroupIgnoreListsCollide((QueueGroup**)groups, numGroup, group, currentInstance))
            {
                continue;
            }
            // If we ever decide to add checks for group composition, they would go here.
            groups[numGroup++] = group;
            queued += group->numPlayer;
            if (queued > bestCount)
            {
                int startMin = (group->flags & TS_GROUP_FLAGS_WANTS_TO_OWN_GROUP) ? currentMission->preformedMinimumPlayers : currentMission->pickupMinimumPlayers;
                if (queued + (currentInstance ? currentInstance->numAccept : 0) >= startMin)
                {
                    bestCount = queued;
                    bestNumGroup = numGroup;
                    memcpy(bestGroups, groups, numGroup * sizeof(QueueGroup *));
                    if ((bestCount == currentMission->maximumPlayers) || (group->flags & TS_GROUP_FLAGS_WANTS_TO_OWN_GROUP))
                    {
                        // We found an optimal group - break out now, and kick it off
                        // This will break us out of the outer loop
                        //    or this group is ready to go now with the current members
                        groupSeed = 0;
                        // and get out of the inner loop
                        break;
                    }
                }
            }
        }
    }

    if (bestCount > 0)
    {
        if (currentInstance)
        {
            int currentcount;
            int threshold;
            currentInstance->numRefillAttempts++;

            //    only add to queue if this attempt exceeds the attempt threshold
            //    1rst try:    must fill up event
            //    2nd try:    must fill up to 1/2 way
            //    3rd try:    must fill up to 1/3 way
            currentcount = bestCount + currentInstance->numAccept;
            threshold = (currentMission->preformedMinimumPlayers+(currentMission->maximumPlayers-currentMission->preformedMinimumPlayers)/currentInstance->numRefillAttempts);
            if (currentcount >= threshold)
            {
                // If we can refill it, try adding people into it
                TurnstileServerEventPrepare(currentMission, currentInstance, bestGroups, bestNumGroup, bestCount);
                currentInstance->checkForSub = TIS_WAITING_FOR_RESPONSE;
            }
        }
        else
        {
            // OK, we have a contender group for this event.
            if ((bestCount == currentMission->maximumPlayers) || (bestGroups[0]->flags & TS_GROUP_FLAGS_WANTS_TO_OWN_GROUP))
            {
                // If we've got a full complement, kick it off immediately.
                TurnstileServerEventPrepare(currentMission, NULL, bestGroups, bestNumGroup, bestCount);
            }
            else if (currentMission->possibleStartTime == 0)
            {
                // If possibleStartTime is zero here, this is the first time we've been able to start this.  Note the
                // time at which this took place
                currentMission->possibleStartTime = now;
            }
            else if (now > currentMission->possibleStartTime + currentMission->startDelay)
            {
                // Otherwise if it's been pending in a ready state long enough, kick it off
                TurnstileServerEventPrepare(currentMission, NULL, bestGroups, bestNumGroup, bestCount);
            }
        }
    }
    else
    {
        if (!currentInstance)
        {
            // Can't start, reset possibleStartTime
            currentMission->possibleStartTime = 0;
        }
    }
}
static void GroupAddGroupsToRefillInstances(TurnstileMission *currentMission)
{
    int i;
    U32 now = timerSecondsSince2000();
    int numInstances = eaSize(&currentMission->instanceList);

    for (i = 0; i < numInstances; ++i)
    {
        MissionInstance *currentInstance = currentMission->instanceList[i];
        if ((now > (currentInstance->lastCheckTime + (1+currentInstance->numRefillAttempts)*10)) || (numInstances < 10))
        {
            currentInstance->lastCheckTime = now;
            if (currentInstance->checkForSub == TIS_LOOKING)
            {
                GroupAddGroupsToInstance(currentMission, currentInstance);
            }
        }
    }
}
static void TurnstileGroupReturnAcceptedToQueue(MissionInstance *instance)
{
    QueueGroup *group = instance->queuedGroups;
    //    go through the all the groups
    while (group)
    {
        int j;
        QueueGroup *nextGroup = group->next;        //    the current group might get blown away from the remove
                                                    //    save where we should look next
        for (j = group->numPlayer -1; j >= 0; --j)
        {
            //    remove any players that were rejected from the group
            if (!(group->playerList[j].eventReadyFlags & EVENT_READY_ACCEPTED))
            {
                GroupRemovePlayerFromGroup(group->owner_dbid, group->playerList[j].db_id, NULL);
            }
            else
            {
                //    reset the remaining players status
                group->playerList[j].eventReadyFlags = 0;
                turnstileServer_generateEventFailedStart(group, j);
            }
        }

        devassert(group->parentInstance);
        if (GroupExtractGroupFromQueue(group->owner_dbid))        //    remove cleanly
        {
            if (group->flags & TS_GROUP_FLAGS_WANTS_TO_OWN_GROUP)
            {
                GroupRemoveGroupFromQueue(group->owner_dbid, NULL, true);
            }
            else
            {
                group->parentInstance = NULL;
                GroupAddToQueue(group, 1);                                    //    reinsert
            }
        }
        else
        {
            //    the group was already destroyed by (GroupRemovePlayerFromGroup) and doesn't need to be removed again
            //    destroying a 2nd time actually puts the freelist in a bad state so don't do that...
        }

        group = nextGroup;
    }

    //    remove the group from the instance queue
    instance->queuedGroups = NULL;

    //    free the instance
    TurnstileServerEventReleaseInstance(NULL, instance);
}
static int s_linksAreSetup(MissionInstance *instance, TurnstileMission *mission)
{
    int i;
    MissionInstance **instanceList = NULL;
    if (!mission || !instance)
        return false;

    //    already linked up
    if (instance->masterInstance)
        return true;

    if (mission->isLink)
        return false;

    instance->masterInstance = instance;
    for (i = 0; i < eaiSize(&mission->linkTo); ++i)
    {
        int j;
        int missionID = mission->linkTo[i];
        TurnstileMission *linkedMission = turnstileConfigDef.missions[missionID];
        int linkFound = 0;
        for (j = 0; j < eaSize(&linkedMission->instanceList); ++j)
        {
            if ((linkedMission->instanceList[j] != instance) &&            //    don't link to ourselves
                (!linkedMission->instanceList[j]->masterInstance) &&    //    or an instance that is already linked
                (linkedMission->instanceList[j]->queuedGroups) &&        //    or one that isn't ready
                (linkedMission->instanceList[j]->checkForSub == TIS_NONE))    //    or one that is just refilling subs
            {
                //    link it
                eaPush(&instanceList, linkedMission->instanceList[j]);
                linkedMission->instanceList[j]->masterInstance = instance;
                linkFound = 1;
                break;
            }
        }
        if (!linkFound)
        {
            //    we couldn't make one of our links
            //    have to break it all
            for (j = 0; j < eaSize(&instanceList); ++j)
            {
                instanceList[j]->masterInstance = NULL;
            }
            eaDestroy(&instanceList);
            instance->masterInstance = NULL;
            return false;
        }
    }
    return true;
}

void GroupTick()
{
    int i;
    int j;
    int swap;
    int curEvent;
    U32 now;
    int allPlayersResponded;
    TurnstileMission *currentMission;                // Mission under consideration this tick. Selected via shuffled list in eventOrder in phase 1
    QueuedPlayer *queuedPlayer;
    static int *eventOrder = NULL;                    // Shuffled event list, see comments above second if () test for explanations
    static int numEvent = 0;
    static int curEventIdx = 0;
    static U32 lastUpdateTime = 0;

    now = timerSecondsSince2000();

    // First time initialisation
    if (eventOrder == NULL)
    {
        numEvent = eaSize(&turnstileConfigDef.missions);
        eventOrder = malloc(numEvent * sizeof(int));
        // Set up so we'll reshuffle the order immediately
        curEventIdx = numEvent;
        assert(eventOrder != NULL && "Memory allocation error.  We're in deep trouble.");
    }

    // First half - rip through the queue looking to see if we can form any events.

    // DGNOTE - THIS IS VERY IMPORTANT!
    // When we separate events into classes (e.g. endgame / classic TF / storyarc / whatever), this will probably get a lot more gnarly.
    // Maybe involving the formation of some sort of per event class structure to keep track of all this, coupled with running separate
    // eventOrder lists for each class, and processing one entry from each class per pass through here.

    // Shuffle the group order.  This solves the problem of someone who repeatedly queues for 
    // events 2 and 3.  In the absence of a shuffle, they'd obviously get into event 2 far more often.
    if (curEventIdx == numEvent)
    {
        eventOrder[0] = 0;
        for (i = 1; i < numEvent; i++)
        {
            swap = rule30Rand() % (unsigned int) (i + 1);        // Yes, I did mean (i + 1).  See the section about initializing and shuffling at the same time 
                                                                // towards the bottom of http://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
            eventOrder[i] = eventOrder[swap];
            eventOrder[swap] = i;
        }
        curEventIdx = 0;
    }
    curEvent = eventOrder[curEventIdx];
    currentMission = turnstileConfigDef.missions[curEvent];

    devassert(currentMission->missionID == curEvent);
    GroupAddGroupsToRefillInstances(currentMission);
    GroupAddGroupsToInstance(currentMission, NULL);

    // Send out status updates every thirty seconds
    if (now > lastUpdateTime + 30 && s_updateGroup == NULL)
    {
        s_updateGroup = s_turnstileQueueHead;
        lastUpdateTime = now;
    }

    // handle 100 groups per tick to spread the load
    for (i = 0; i < 100 && s_updateGroup; i++, s_updateGroup = s_updateGroup->next)
    {
        turnstileServer_generateQueueStatus(s_updateGroup, 0, 1, NULL);
    }

    curEventIdx++;

    // Now that the queue has been taken care of, we turn our attention to the events that are trying to start.
    // DGNOTE 8/25/2010
    // See comments below with the same DGNOTE tag as to why this loop *must* walk the array from back to front.
    for (i = eaSize(&currentMission->instanceList) - 1; i >= 0; i--)
    {
        MissionInstance *instance = currentMission->instanceList[i];

// Re-add this if COH-27360 keeps crashing!
//        if ((int)instance == (int)instance->queuedGroups) // already freed by MP_FREE?
//        {
//            // This is causing crashes, so ditch the data.  Not a great solution.
//            LOG(LOG_TURNSTILE_SERVER, LOG_LEVEL_IMPORTANT, 0, 
//                "Removed a MissionInstance from the list because it looks like it was freed.  THIS IS BAD AND SHOULDN'T HAPPEN!  eventId = %d, mapId = %d, mapOwner = %d.", 
//                instance->eventId, instance->mapId, instance->mapOwner);
//            eaRemove(&currentMission->instanceList, i);
//            continue;
//        }

        // Once an event has started, the queuedGroups list no longer exists
        if (instance->queuedGroups != NULL)
        {
            if (!s_linksAreSetup(instance, currentMission))
            {
                if (currentMission->autoQueueMyLinks || (currentMission->isLink == 2))
                {
                    if (eaSize(&currentMission->instanceList) > 1)
                    {
                        QueueGroup *group = instance->queuedGroups;
                        eaFindAndRemoveFast(&currentMission->instanceList, instance);
                        //    go through the all the groups
                        while (group)
                        {
                            int j;
                            QueueGroup *nextGroup = group->next;        //    the current group might get blown away from the remove
                            devassert(group->parentInstance);
                            for (j = 0; j < group->numEvent; ++j)
                            {
                                if (currentMission->missionID == group->eventList[j])
                                {
                                    group->eventList[j] *= -1;
                                }
                                else if (group->eventList[j] < 0)
                                {
                                    if (turnstileConfigDef.missions[group->eventList[j]*-1]->autoQueueMyLinks)
                                    {
                                        group->eventList[j] *= -1;
                                    }
                                }
                            }
                            if (GroupExtractGroupFromQueue(group->owner_dbid))        //    remove cleanly
                            {
                                if (group->flags & TS_GROUP_FLAGS_WANTS_TO_OWN_GROUP)
                                {
                                    GroupRemoveGroupFromQueue(group->owner_dbid, NULL, true);
                                }
                                else
                                {
                                    group->parentInstance = NULL;                                
                                    GroupAddToQueue(group, 1);                                    //    reinsert
                                }
                            }
                            group = nextGroup;
                        }
                        //    remove the group from the instance queue
                        instance->queuedGroups = NULL;

                        //    free the instance
                        TurnstileServerEventReleaseInstance(currentMission, instance);
                    }
                }
            }
            else
            {
                instance->mapId = instance->mapOwner ? instance->masterInstance->mapId : 0;
                // mapID holds status about the map creation prior to holding the actual mapID
                // Initially it's zero meaning we're still waiting for accept / decline responses.
                // Once everyone has replied, send out a mapstart message, and set it to -1.
                // One of two things will happen:  either the map is successfully created, in which
                // case it gets set to the actual ID, which will be a number strictly greater than zero.
                // In the event the map creation fails, which is possible, it gets set to -2 which causes
                // a retry of the create.  Failure to create can happen when the message to the intended
                // league leader is lost in transit due to them zoning.
                if (!instance->playersAlerted)
                {
                    QueueGroup *group;
                    for (group = instance->queuedGroups; group; group = group->next)
                    {
                        if (currentMission->isLink || eaiSize(&currentMission->linkTo))
                        {
                            //    for linked missions, auto accept everyone..
                            //    this is so we don't have to deal with putting everyone back into the queue
                            //    laziness for now
                            for (j = 0; j < group->numPlayer; j++)
                            {
                                group->playerList[j].eventReadyTimer = now;
                                group->playerList[j].eventReadyFlags = EVENT_READY_ACKED;
                                GroupEventResponse(instance->eventId, group->owner_dbid, group->playerList[j].db_id, 1);
                            }
                        }
                        else
                        {
                            turnstileServer_generateEventReady(group, currentMission, instance, -1, instance->checkForSub != TIS_NONE);
                        }
                        if (group->numPlayer <= ((currentMission->preformedMinimumPlayers+1)/2))
                        {
                            //    only update the average if less than the 1/2 the players were in the group
                            //    large groups probably organized on their own, so we don't want their time (outside of turnstile) to weight the average
                            turnstileConfigCfg.overallAverage = updateAverageWaitTime(turnstileConfigCfg.overallAverage, group->numPlayer, now - group->joinTime);
                            currentMission->avgWait = updateAverageWaitTime(currentMission->avgWait, group->numPlayer, now - group->joinTime);
                        }
                    }
                    instance->playersAlerted = 1;
                }
                else if ((instance->mapId == 0)|| (instance->checkForSub == TIS_WAITING_FOR_RESPONSE))
                {
                    QueueGroup *group;
                    // Assume everyone has responded, then clear this if we find someone that hasn't
                    allPlayersResponded = 1;
                    if (timerSecondsSince2000() < instance->eventStartTime)
                    {
                        for (group = instance->queuedGroups; group; group = group->next)
                        {
                            for (j = 0; j < group->numPlayer; j++)    
                            {
                                queuedPlayer = &group->playerList[j];
                                if (!(queuedPlayer->eventReadyFlags & EVENT_READY_ACKED))
                                {
                                    if (now > queuedPlayer->eventReadyTimer + ACK_WAIT_DELAY)
                                    {
                                        turnstileServer_generateEventReady(group, currentMission, instance, j, 0);
                                    }
                                    allPlayersResponded = 0;
                                }
                                else if (!(queuedPlayer->eventReadyFlags & EVENT_READY_RESPONDED))
                                {
                                    if (now > queuedPlayer->eventReadyTimer + SERVER_RESPONSE_DELAY)
                                    {
                                        queuedPlayer->eventReadyFlags |= EVENT_READY_RESPONDED;
                                    }
                                    else
                                    {
                                        allPlayersResponded = 0;
                                    }
                                }
                            }
                        }
                    }

                    if (allPlayersResponded)
                    {
                        if (instance->checkForSub == TIS_WAITING_FOR_RESPONSE)
                        {
                            instance->checkForSub = TIS_ALL_RESPONDED;
                        }
                        else
                        {
                            if (instance->numAccept < currentMission->preformedMinimumPlayers)
                            {
                                eaFindAndRemoveFast(&currentMission->instanceList, instance);
                                TurnstileGroupReturnAcceptedToQueue(instance);
                            }
                            else
                            {
                                // DGNOTE 8/25/2010
                                // Everyone has replied.  Kick off the map.
                                // Note that if everyone declines, we'll detect that in here, and kill the instance. Among other things
                                // this will remove the instance from the currentMission->instanceList earray.  I can safely use an
                                // eaRemoveFast call for that because we're walking backwards down the earray.
                                TurnstileServerEventStartMap(currentMission, instance);
                            }
                        }
                    }
                }
                else if (instance->mapId == -1)
                {
                    //    waiting for mapserver to respond
                }
                else if (instance->mapId == -2)
                {
                    // Failure.  Retry or rip everything apart?
                    eaFindAndRemoveFast(&currentMission->instanceList, instance);
                    TurnstileGroupReturnAcceptedToQueue(instance);
                }
                else
                {
                    // Finally good to go - the last thing we needed was the map, we just got it.
                    TurnstileServerEventStartEvent(currentMission, instance);
                    if (currentMission->type == TUT_MissionType_Contact)
                    {
                        eaFindAndRemoveFast(&currentMission->instanceList, instance);
                        TurnstileServerEventReleaseInstance(currentMission, instance);
                    }
                    else if (currentMission->type == TUT_MissionType_Instance)
                    {
                        //    the design is that as long as we have the minimum (which should be guaranteed before the mapstart)
                        //    this instance is no longer looking for members
                        //    when we are refilling, this is the only time the number can remain below the min 
                        //    if it does, take the people we can get, but keep looking for people
                        if (instance->checkForSub != TIS_EVENT_LOCKED_FOR_GROUP)
                        {
                            instance->checkForSub = (instance->numAccept < currentMission->pickupMinimumPlayers) ? TIS_LOOKING : TIS_NONE;
                        }
                        instance->numRefillAttempts = 0;
                    }
                    else
                    {
                        assert(0);        //    unhandled type
                    }
                }
            }
        }
    }
}
