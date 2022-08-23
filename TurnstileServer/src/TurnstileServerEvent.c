// TurnstileServerEvent.c
//

#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/timing.h>
#include "entity/teamCommon.h"
#include "chatdefs.h"
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/genericlist.h>
#include "turnstile/TurnstileServerCommon.h"
#include "TurnstileServerGroup.h"
#include "TurnstileServer.h"
#include "TurnstileServerMsg.h"
#include "TurnstileServerEvent.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/log.h>

MP_DEFINE(MissionInstance);
static int s_instanceId = 0;
static StashTable s_missionInstances;
// This fires when we find a collection of players from the queue that can start the specified event.  We pull the groups out of the main queue and put them
// in a holding tank in the MissionInstance structure for this event.  Then send off event ready messages to all players, and wait for responses.
void TurnstileServerEventPrepare(TurnstileMission *mission, MissionInstance *instance, QueueGroup **groups, int numGroup, int numPlayer)
{
    int i;
    U32 now;
    QueueGroup *group;
    int newInstance = 0;
    if (!instance)
    {
        MP_CREATE(MissionInstance, 64);
        instance = MP_ALLOC(MissionInstance);
        assert(instance);                        // Not much else I can do.  It'll just kill the turnstile server ...
        // As a TODO we ought to have a "panic" shutdown routine that sends
        // a message to everyone in the queue that they're no longer queued,
        // to be used immediately before committing suicide.
        memset(instance, 0, sizeof(MissionInstance));
        instance->numPlayer = numPlayer;
        if (s_instanceId == 0)
            s_instanceId++;
        //    dont reuse a key until its been cleared (this is for the unlikely event that we collide with another key)
        while (stashIntFindPointer(s_missionInstances, s_instanceId, NULL))
        {
            s_instanceId++;
        }
        instance->eventId = s_instanceId++;
        instance->checkForSub = TIS_NONE;
        instance->playersAlerted = 0;
        instance->masterInstance = NULL;
        newInstance = 1;
        stashIntAddPointer(s_missionInstances, instance->eventId, instance, 0);
    }
    else
    {
        instance->numDeclined = 0;
        instance->numPlayer = instance->numAccept+numPlayer;    //    when refilling, the display count is the number of players currently in the event
                                                                //    plus the number of players we are refilling with current players 
    }

    now = timerSecondsSince2000();

    instance->eventStartTime = now + EVENT_READY_BASE_DELAY + 10;    //    add 10 for some latency buffer

    for (i = 0; i < numGroup; i++)
    {
        devassert(!groups[i]->parentInstance);
        group = GroupExtractGroupFromQueue(groups[i]->owner_dbid);
        devassert(group);
        if (group == NULL)
        {
            // TODO squawk very loudly
            
            continue;
        }
        devassert(group == groups[i]);
        if (group != groups[i])
        {
            // TODO squawk very loudly
            GroupDestroy(group);    // Note that destruction of this group will leave the members "hanging"
                                    // in that they won't be able to join further events.  Should maybe notify them of this fact ...
            continue;
        }
        // Add this to the grouplist for this event.
        group->parentInstance = instance;
        if (group->flags & TS_GROUP_FLAGS_WANTS_TO_OWN_GROUP)
        {
            devassert(numGroup == 1);
            instance->checkForSub = TIS_EVENT_LOCKED_FOR_GROUP;
        }
        listAddForeignMember(&instance->queuedGroups, group);
        // I'd love to start the mission map here, it'd allow it to be loading while the players are responding to the dialog.
        // Unfortunately, I need a player Entity to attach a team to that can be used for the map creation.  Sure, I could
        // just take the owner of the first group, but then a ton of bad stuff would happen if he declined the invite before
        // anyone else made it onto the map.
    }
    if (newInstance)
    {
        eaPush(&mission->instanceList, instance);
    }
}

// Return an instance to the memory pool
void TurnstileServerEventReleaseInstance(TurnstileMission *mission, MissionInstance *instance)
{
    int i;
    QueueGroup *group, *next;

    LOG(LOG_TURNSTILE_SERVER, LOG_LEVEL_IMPORTANT, 0, 
        "Releasing an instance.  eventId = %d, mapId = %d, mapOwner = %d.", 
        instance->eventId, instance->mapId, instance->mapOwner);

    for (group = instance->queuedGroups; group; group = next)
    {
        next = group->next;
        devassert(group->parentInstance);
        GroupExtractGroupFromQueue(group->owner_dbid);
        GroupDestroy(group);
    }
    instance->queuedGroups = NULL;
    instance->masterInstance = NULL;
    eaiDestroy(&instance->timeoutPlayerId);
    eaiDestroy(&instance->timeoutPlayerTimestamp);
    eaiDestroy(&instance->banIDList);

    if (mission)
    {
        for (i = 0; i < eaiSize(&mission->linkTo); ++i)
        {
            int j;
            TurnstileMission *linkedMission = turnstileConfigDef.missions[mission->linkTo[i]];
            for (j = eaSize(&linkedMission->instanceList)-1; j >= 0; --j)
            {
                MissionInstance *linkedInstance = linkedMission->instanceList[j];
                if (linkedInstance->masterInstance == instance)
                {
                    TurnstileServerEventReleaseInstance(linkedMission, linkedInstance);
                    eaRemove(&linkedMission->instanceList, j);
                }
            }
        }
    }
    stashIntRemovePointer(s_missionInstances, instance->eventId, NULL);
    MP_FREE(MissionInstance, instance);
}

// This starts the map.  It just runs through the list of players, looking for the first that accepted.  It also
// kicks players that declined.  If everyone declines, destroy the instance
void TurnstileServerEventStartMap(TurnstileMission *mission, MissionInstance *instance)
{
    int i;
    int startedMap;
    QueueGroup *group;
    QueuedPlayer *queuedPlayer;

    LOG(LOG_TURNSTILE_SERVER, LOG_LEVEL_IMPORTANT, 0,
        "Turnstile server is starting an event for '%s' with %d accepting players.", 
        mission->name, instance->numAccept);

    startedMap = 0;
    for (group = instance->queuedGroups; group; group = group->next)
    {
        for (i = 0; i < group->numPlayer; i++)    
        {
            queuedPlayer = &group->playerList[i];
            if (queuedPlayer->eventReadyFlags & EVENT_READY_ACCEPTED)
            {
                if (!startedMap)
                {
                    if (instance->masterInstance == instance)
                    {
                        turnstileServer_generateMapStart(group, mission, instance, i);
                    }
                    instance->mapId = -1;
                    instance->mapOwner = queuedPlayer->db_id;
                    startedMap = 1;
                }
            }
        }
    }
    if (!startedMap)
    {
        // Everyone declined.  Rip it all apart
        // DGNOTE 8/25/2010
        // Pull this instance out of the mission's instance list - we can safely use eaRemoveFast as long as the loop that
        // encloses the call into here walks backwards down the earray.  See the comments in TurnstileserverGroup.c with
        // matching DGNOTE tags for details.
        eaFindAndRemoveFast(&mission->instanceList, instance);
        // and return the instance to the pool
        TurnstileServerEventReleaseInstance(mission, instance);
    }
    else
    {
        devassert(instance->numAccept > 0);
    }
}

static void TurnstileFixupLeaderTeam(MissionInstance *instance)
{
    QueueGroup *group;
    int leaderTeam = -1;
    devassert(instance);
    if (instance)
    {
        for (group = instance->queuedGroups; group; group = group->next)
        {
            int i;
            for (i = 0; i < group->numPlayer; ++i)
            {
                if (instance->mapOwner == group->playerList[i].db_id)
                {
                    leaderTeam = group->playerList[i].teamNumber;
                    if (group->playerList[i].isTeamLeader)
                        group->playerList[i].isTeamLeader = 2;
                    break;
                }
            }
            if (leaderTeam >= 0)
            {
                break;
            }
        }

        if (leaderTeam != 0)
        {
            //    if not already the 0 team
            //    swap with the 0 team
            for (group = instance->queuedGroups; group; group = group->next)
            {
                int i;
                for (i = 0; i < group->numPlayer; ++i)
                {
                    if (group->playerList[i].teamNumber == 0)
                    {
                        group->playerList[i].teamNumber = leaderTeam;
                    }
                    else if (group->playerList[i].teamNumber == leaderTeam)
                    {
                        group->playerList[i].teamNumber = 0;
                    }
                }
            }
        }
    }
}
static int groupSizeComparator(const QueueGroup **a, const QueueGroup **b ) 
{ 
    devassert(a);
    devassert(b);
    devassert(*a);
    devassert(*b);
    if (a && b && *a && *b)
    {
        return ((*a)->numPlayer > (*b)->numPlayer) ? 1 : -1;
    }
    else
    {
        return 0;    //    who knows what this is at this point...
    }
}

//    The algorithm is a breadth fill for groups
//    Find the largest group of players (doesn't matter if league or not)
//        The largest group gets priority to fill. They fill up team slots first
//        When all team slots are filled, any remaining groups try to fit into the teams if possible
//            if the group cannot fit into the teams evenly, break them up, and fill into first available team
//                
//    TODO:    solo players maybe shouldn't get their own team
//            if we detect a solo player, just put him into the first valid team?
//            should we load balance groups that can't fit into the teams so they get spread out into the least full team?

static void TurnstileUpdateIntoProperGroups(TurnstileMission *mission, MissionInstance *instance)
{
    devassert(mission);
    devassert(instance);
    if (mission && instance)
    {
        if (instance->checkForSub)
        {
            //    set everyone's team to -1
            //    clear their leader bit
            QueueGroup *group;
            for (group = instance->queuedGroups; group; group = group->next)
            {
                int i;
                for (i = 0; i < group->numPlayer; ++i)
                {
                    group->playerList[i].teamNumber = -1;
                    group->playerList[i].isTeamLeader = 0;
                }
            }
        }
        else
        {
            QueueGroup *group;
            QueueGroup **groupList = NULL;
            int memberCount[MAX_LEAGUE_TEAMS];
            int i;
            int idealNumGroups = mission->maximumPlayers ?
                MIN(ceil(mission->maximumPlayers * 1.f/ MAX_TEAM_MEMBERS), MAX_LEAGUE_TEAMS) : 
                MAX_LEAGUE_TEAMS;

            for (i = 0; i < idealNumGroups; ++i)
            {
                memberCount[i] = 0;
            }

            for (group = instance->queuedGroups; group; group = group->next)
            {
                eaPush(&groupList, group);
            }
            eaQSort(groupList, groupSizeComparator);

            //    start with the largest group
            //    they get priority to fill team slots
            //    then fill as best fit
            for (i = (eaSize(&groupList)-1); i >= 0; --i)
            {
                int subGroupIndex;
                int groupMemberCount[MAX_LEAGUE_TEAMS];
                for (subGroupIndex = 0; subGroupIndex < MAX_LEAGUE_TEAMS; ++subGroupIndex)
                {
                    groupMemberCount[subGroupIndex] = 0;
                }
                group = groupList[i];
                for (subGroupIndex = 0; subGroupIndex < group->numPlayer; ++subGroupIndex)
                {
                    if (group->playerList[subGroupIndex].eventReadyFlags & EVENT_READY_ACCEPTED)
                    {
                        int desiredTeam = (group->playerList[subGroupIndex].desiredTeamNumber < 0) ? 0 : group->playerList[subGroupIndex].desiredTeamNumber;
                        groupMemberCount[desiredTeam]++;
                    }
                }

                for (subGroupIndex = 0; subGroupIndex < MAX_LEAGUE_TEAMS; ++subGroupIndex)
                {
                    //    loop through all the groups subgroups
                    //    if subgroup is empty (noone in the group accepted), stop here
                    if (groupMemberCount[subGroupIndex])
                    {
                        int tsGroupIndex;    //    turnstile group Index
                        int groupCanFit = 0;
                        for (tsGroupIndex = 0; tsGroupIndex < idealNumGroups; ++tsGroupIndex)
                        {
                            //    if this league team location is empty, let the group fill that one first (breadth fill)
                            if (!memberCount[tsGroupIndex])
                            {
                                int playerIndex;
                                //    add everyone who accepted into the proper location
                                for (playerIndex = 0; playerIndex < group->numPlayer; ++playerIndex)
                                {
                                    if (group->playerList[playerIndex].eventReadyFlags & EVENT_READY_ACCEPTED)
                                    {
                                        if ((group->playerList[playerIndex].desiredTeamNumber == subGroupIndex) ||
                                            (group->playerList[playerIndex].desiredTeamNumber == -1))
                                        {
                                            group->playerList[playerIndex].teamNumber = tsGroupIndex;
                                            memberCount[tsGroupIndex]++;
                                        }
                                    }
                                }
                                groupCanFit = 1;
                                break;
                            }
                        }

                        if (!groupCanFit)
                        {
                            //    look through all the currently filled groups, if we can fit, insert the whole group in
                            for (tsGroupIndex = 0; tsGroupIndex < idealNumGroups; ++tsGroupIndex)
                            {
                                if ((groupMemberCount[subGroupIndex] + memberCount[tsGroupIndex]) <= MAX_TEAM_MEMBERS)
                                {
                                    //    this group will fit in here
                                    //    everyone who accepted gets put in this team location
                                    int playerIndex;
                                    for (playerIndex = 0; playerIndex < group->numPlayer; ++playerIndex)
                                    {
                                        if (group->playerList[playerIndex].eventReadyFlags & EVENT_READY_ACCEPTED)
                                        {
                                            if ((group->playerList[playerIndex].desiredTeamNumber == subGroupIndex) ||
                                                (group->playerList[playerIndex].desiredTeamNumber == -1))
                                            {
                                                group->playerList[playerIndex].teamNumber = tsGroupIndex;
                                                group->playerList[playerIndex].isTeamLeader = 0;    //    unless you were the first group inserted here, you can't be the team leader
                                                memberCount[tsGroupIndex]++;
                                            }
                                        }
                                    }
                                    groupCanFit = 1;
                                    break;
                                }
                            }

                            //    if we couldn't fit into any open groups
                            //    break up the group and insert wherever it can go..
                            if (!groupCanFit)
                            {
                                int playerIndex;
                                for (playerIndex = 0; playerIndex < group->numPlayer; ++playerIndex)
                                {
                                    if (group->playerList[playerIndex].eventReadyFlags & EVENT_READY_ACCEPTED)
                                    {
                                        if ((group->playerList[playerIndex].desiredTeamNumber == subGroupIndex) ||
                                            (group->playerList[playerIndex].desiredTeamNumber == -1))
                                        {
                                            for (tsGroupIndex = 0; tsGroupIndex < idealNumGroups; ++tsGroupIndex)
                                            {
                                                if (memberCount[tsGroupIndex] < MAX_TEAM_MEMBERS)
                                                {
                                                    group->playerList[playerIndex].teamNumber = tsGroupIndex;
                                                    group->playerList[playerIndex].isTeamLeader = 0;    //    unless you were the first group inserted here, you can't be the team leader
                                                    memberCount[tsGroupIndex]++;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            TurnstileFixupLeaderTeam(instance);
            //    sanity check to make sure everyone got into a valid team
            for (i = (eaSize(&groupList)-1); i >= 0; --i)
            {
                int j;
                QueueGroup *curGroup = groupList[i];
                for (j = 0; j < curGroup->numPlayer; ++j)
                {
                    devassertmsg( (curGroup->playerList[j].teamNumber >= 0) && (curGroup->playerList[j].teamNumber < idealNumGroups),
                        "Idx: %i,%i|TeamNumber: %i|Ideal %i", i, j, curGroup->playerList[j].teamNumber, idealNumGroups);
                    if (!( (curGroup->playerList[j].teamNumber >= 0) && (curGroup->playerList[j].teamNumber < idealNumGroups) ))
                    {
                        curGroup->playerList[j].teamNumber = -1;
                    }
                }
            }
            eaDestroy(&groupList);
        }
    }
}
void TurnstileServerEventStartEvent(TurnstileMission *mission, MissionInstance *instance)
{
    QueueGroup *group;
    QueueGroup *next;

    TurnstileUpdateIntoProperGroups(mission, instance);
    for (group = instance->queuedGroups; group; group = next)
    {
        turnstileServer_generateEventStart(group, mission, instance);
        next = group->next;
        devassert(group->parentInstance);
        GroupExtractGroupFromQueue(group->owner_dbid);
        GroupDestroy(group);
    }
    instance->queuedGroups = NULL;
}

MissionInstance *TurnstileServerEventFindInstanceByID(int instanceID)
{
    MissionInstance *instance;
    stashIntFindPointer(s_missionInstances, instanceID, &instance);
    return instance;
}

void InstanceInit()
{
    s_missionInstances = stashTableCreateInt(256);
}