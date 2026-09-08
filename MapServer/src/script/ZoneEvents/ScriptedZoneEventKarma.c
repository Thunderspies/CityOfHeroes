#include <utilitieslib/stdtypes.h>
#include "ScriptedZoneEventKarma.h"
#include <utilitieslib/components/earray.h>
#include "entity/entity.h"
#include <utilitieslib/utils/SuperAssert.h>
#include "entity/character_karma.h"
#include "script/scriptengine.h"
#include "script/Scripthook/ScriptHookInternal.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "container/containerEventHistory.h"
#include "container/EventHistory.h"
#include "ScriptedZoneEvent.h"
#include "entity/powers.h"

static ScriptKarmaBucketList **s_scriptKarmaBucketList = NULL;

int *g_zoneEventScriptStageAwardsKarma = NULL;

ScriptKarmaBucketList *getCurrentScriptKarmaBuckets(int scriptId)
{
    int i;
    ScriptKarmaBucketList *karmaBucketList = NULL;
    for (i = 0; i < eaSize(&s_scriptKarmaBucketList); ++i)
    {
        if (s_scriptKarmaBucketList[i]->scriptId == scriptId)
        {
            karmaBucketList= s_scriptKarmaBucketList[i];
            break;
        }
    }
    if (!karmaBucketList)
    {
        karmaBucketList = malloc(sizeof(ScriptKarmaBucketList));
        memset(karmaBucketList, 0, sizeof(ScriptKarmaBucketList));
        karmaBucketList->scriptId = scriptId;
        eaPush(&s_scriptKarmaBucketList, karmaBucketList);
    }
    return karmaBucketList;
}

////////////////////////////
// Karma helper functions
ScriptKarmaBucket * ScriptGetBucketForPlayer(Entity *e, int scriptId)
{
    int i;
    ScriptKarmaBucketList *scriptBuckets = getCurrentScriptKarmaBuckets(scriptId);
    for (i = 0; i < eaSize(&scriptBuckets->scriptKarmaBuckets); ++i)
    {
        if (scriptBuckets->scriptKarmaBuckets[i]->db_id == e->db_id)
        {
            return scriptBuckets->scriptKarmaBuckets[i];
        }
    }
    return NULL;
}
static int s_GetSize(ScriptKarmaBucket *bucket, int bucketType)
{
    assert(bucket);
    assert(bucketType < SKT_COUNT);
    if (bucket && bucketType < SKT_COUNT)
    {
        return eaiSize(&bucket->karmaArrays[bucketType].karma);

    }
    return 0;
}
static int s_GetAverage(ScriptKarmaBucket *bucket, int bucketType)
{
    assert(bucket);
    assert(bucketType < SKT_COUNT);
    if (bucket && bucketType < SKT_COUNT)
    {
        int size = s_GetSize(bucket, bucketType);
        if (size)
        {
            int i, sum = 0;
            for (i = 0; i < size; ++i)
            {
                sum += bucket->karmaArrays[bucketType].karma[i];
            }
            return (sum/size);
        }
    }
    return 0;
}
static int s_GetMedian(ScriptKarmaBucket *bucket, int bucketType)
{
    assert(bucket);
    assert(bucketType < SKT_COUNT);
    if (bucket && bucketType < SKT_COUNT)
    {
        int size = s_GetSize(bucket, bucketType);
        if (size)
        {
            int middle = size/2;
            int retVal = 0;
            if (size % 2)
            {
                //    i.e. if size == 5, then middle = 2
                //    return index 2 (the 3rd number in a set of 5)
                retVal = bucket->karmaArrays[bucketType].karma[middle];
            }
            else
            {
                //    i.e. if size == 4, then middle = 2
                //    we take index 1 and 2, and average for the median
                retVal = (bucket->karmaArrays[bucketType].karma[middle-1] + bucket->karmaArrays[bucketType].karma[middle])/2;
            }
            return retVal;
        }
    }
    return 0;
}
int ScriptKarmaGetAverage(Entity *e, int scriptId, int bucketType)
{
    assert(e);
    assert(bucketType < SKT_COUNT);
    if (e && (bucketType < SKT_COUNT))
    {
        ScriptKarmaBucket *bucket = ScriptGetBucketForPlayer(e, scriptId);
        if (bucket)
        {
            return s_GetAverage(bucket, bucketType);
        }
    }
    return 0;
}
int ScriptKarmaGetMedian(Entity *e, int scriptId, int bucketType)
{
    assert(e);
    assert(bucketType < SKT_COUNT);
    if (e && (bucketType < SKT_COUNT))
    {
        ScriptKarmaBucket *bucket = ScriptGetBucketForPlayer(e, scriptId);
        if (bucket)
        {
            return s_GetMedian(bucket, bucketType);
        }
    }
    return 0;
}
void ScriptAddStat(ScriptKarmaBucket *karmaBucket, int bucketType, Entity *e)
{
    assert(karmaBucket);
    assert(bucketType < SKT_COUNT);
    if (karmaBucket)
    {
        int amount = 0;
        int currentTime = g_currentKarmaTime;
        karmaBucket->timeUpdated = g_currentKarmaTime;
        switch (bucketType)
        {
        case SKT_SORTED_STAT:
            assert(e);
            amount = e ? CharacterCalculateKarma(e->pchar, currentTime) : 0;
            eaiSortedPush(&karmaBucket->karmaArrays[bucketType].karma, amount);
            break;
        case SKT_AVERAGE:
            amount = s_GetAverage(karmaBucket, SKT_SORTED_STAT);
            eaiPush(&karmaBucket->karmaArrays[bucketType].karma, amount);
            break;
        case SKT_MEDIAN:
            amount = s_GetMedian(karmaBucket, SKT_SORTED_STAT);
            eaiPush(&karmaBucket->karmaArrays[bucketType].karma, amount);
            break;
        case SKT_NUM:
            amount = s_GetSize(karmaBucket, SKT_SORTED_STAT);
            eaiPush(&karmaBucket->karmaArrays[bucketType].karma, amount);
            break;
        default:
            assert(0);
            break;
        }
    }

}
ScriptKarmaBucket *ScriptAddBucket(Entity *e, int scriptId)
{
    ScriptKarmaBucketList *karmaBucketList = getCurrentScriptKarmaBuckets(scriptId);
    ScriptKarmaBucket *karmaBucket = malloc(sizeof(ScriptKarmaBucket));
    memset(karmaBucket, 0, sizeof(ScriptKarmaBucket));
    karmaBucket->db_id = e->db_id;
    eaPush(&karmaBucketList->scriptKarmaBuckets, karmaBucket);
    return karmaBucket;
}
void ScriptAddStatToPlayer(Entity *e, int scriptId, int bucketType)
{
    assert(e);
    assert(e->pchar);
    assert(bucketType < SKT_COUNT);
    if (e && e->pchar && bucketType < SKT_COUNT)
    {
        ScriptKarmaBucket *karmaBucket = ScriptGetBucketForPlayer(e, scriptId);
        if (!karmaBucket)
        {
            karmaBucket = ScriptAddBucket(e, scriptId);
        }
        ScriptAddStat(karmaBucket, bucketType, e);
    }
}

void ScriptAddStatToAllActivePlayers(int reason)
{
    assert(reason < SKT_COUNT);
    assert(g_currentScript);
    if (reason < SKT_COUNT)
    {
        int i;
        int currentTime = g_currentKarmaTime;

        for (i = NumEntitiesInTeam(ALL_PLAYERS); i > 0; i--)
        {
            Entity *player = EntTeamInternal(ALL_PLAYERS, i-1, NULL);
            if (player && player->pchar && (player->pchar->karmaContainer.inZoneEvent == g_currentScript->id))
            {
                ScriptAddStatToPlayer(player, g_currentScript->id, reason);
            }
        }
    }
}
void ScriptAddStatToAllPlayers(int reason)
{
    assert(reason < SKT_COUNT);
    assert(g_currentScript);
    if ((reason < SKT_COUNT) && g_currentScript)
    {
        int i;
        ScriptKarmaBucketList *karmaBucketList = getCurrentScriptKarmaBuckets(g_currentScript->id);
        for (i = (eaSize(&karmaBucketList->scriptKarmaBuckets)-1); i >= 0; i--)
        {
            ScriptAddStat(karmaBucketList->scriptKarmaBuckets[i], reason, NULL);
        }
    }
}

void ScriptDestroyKarmaBucketsForAllPlayers(int method)
{
    assert(method < SKRT_COUNT);
    assert(g_currentScript);
    if ((method < SKRT_COUNT) && g_currentScript)
    {
        int i;
        for (i = NumEntitiesInTeam(ALL_PLAYERS); i > 0; i--)
        {
            Entity* e = EntTeamInternal(ALL_PLAYERS, i-1, NULL);
            if (e && e->pchar && (e->pchar->karmaContainer.inZoneEvent == g_currentScript->id))
            {
                CharacterDestroyKarmaBuckets(e->pchar);
            }
        }
        switch (method)
        {
        case SKRT_STAGE_KARMA:
            {
                ScriptKarmaBucketList *karmaBucketList = getCurrentScriptKarmaBuckets(g_currentScript->id);
                for (i = 0; i < eaSize(&karmaBucketList->scriptKarmaBuckets); ++i)
                {
                    eaiDestroy(&karmaBucketList->scriptKarmaBuckets[i]->karmaArrays[SKT_SORTED_STAT].karma);
                    //    they forfeit their pardon to be able to get the stage participation
                    if (karmaBucketList->scriptKarmaBuckets[i]->usedPardonTime)
                        karmaBucketList->scriptKarmaBuckets[i]->usedPardonTime = g_ZoneEventKarmaTable.pardonDuration;
                }
            }
            break;
        case SKRT_ALL_KARMA:
            {
                ScriptKarmaBucketList *karmaBucketList = getCurrentScriptKarmaBuckets(g_currentScript->id);
                for (i = (eaSize(&karmaBucketList->scriptKarmaBuckets)-1); i >= 0; --i)
                {
                    int j;
                    for (j = 0; j < SKT_COUNT; ++j)
                    {
                        eaiDestroy(&karmaBucketList->scriptKarmaBuckets[i]->karmaArrays[j].karma);
                    }
                    free(eaRemove(&karmaBucketList->scriptKarmaBuckets, i));
                }
                eaDestroy(&karmaBucketList->scriptKarmaBuckets);
                for (i = eaSize(&s_scriptKarmaBucketList)-1; i>= 0; --i)
                {
                    if (s_scriptKarmaBucketList[i] == karmaBucketList)
                    {
                        free(eaRemove(&s_scriptKarmaBucketList, i));
                        break;
                    }
                }
            }
            break;
        default:
            assert(0);
            break;
        }
    }
}

void ScriptAddKarmaToTeam(ENTITY player, int reason, int amount, int lifespan)
{
    assert(reason < CKR_COUNT);
    if (amount && (reason < CKR_COUNT))
    {
        int i;
        int currentTime = g_currentKarmaTime;
        TEAM team = GetPlayerTeamFromPlayer(player);
        for (i = NumEntitiesInTeam(team); i > 0; i--)
        {
            Entity* e = EntTeamInternal(team, i-1, NULL);
            if (e && e->pchar)
            {
                CharacterAddKarmaEx(e->pchar, reason, currentTime, amount, lifespan);
            }
        }
    }
}
void ScriptAddKarmaToAllPlayers(int reason, int amount, int lifespan)
{
    assert(g_currentScript);
    if (amount && g_currentScript)
    {
        assert(reason < CKR_COUNT);
        if (reason < CKR_COUNT)
        {
            int i;
            int currentTime = g_currentKarmaTime;

            for (i = NumEntitiesInTeam(ALL_PLAYERS); i > 0; i--)
            {
                Entity* e = EntTeamInternal(ALL_PLAYERS, i-1, NULL);
                if (e && e->pchar && (e->pchar->karmaContainer.inZoneEvent == g_currentScript->id))
                {
                    CharacterAddKarmaEx(e->pchar, reason, currentTime, amount, lifespan);
                }
            }
        }
    }
}

void ScriptKarmaAdvanceStage()
{
    //    get median for this stage and 
    //    increment participation count for all players
    ScriptAddStatToAllActivePlayers(SKT_SORTED_STAT);        //    give everyone the final tick of power
    ScriptAddStatToAllPlayers(SKT_MEDIAN);
    ScriptAddStatToAllPlayers(SKT_AVERAGE);
    ScriptAddStatToAllPlayers(SKT_NUM);

    //    reset the karma bucket for median stats
    ScriptDestroyKarmaBucketsForAllPlayers(SKRT_STAGE_KARMA);
}

static int isBucketActive(int currentTime, Entity *e)
{
    if (e && e->pchar)
    {
        if (!g_ZoneEventKarmaTable.activeDuration || ((currentTime - ((int)e->pchar->karmaContainer.bubbleActivatedTimestamp)) < g_ZoneEventKarmaTable.activeDuration))
        {
            return 1;
        }
    }
    return 0;
}
int ScriptKarmaGetActivePlayers(int scriptId)
{
    int i;
    int currentTime = g_currentKarmaTime;
    int count = 0;
    for (i = NumEntitiesInTeam(ALL_PLAYERS); i > 0; i--)
    {
        Entity* e = EntTeamInternal(ALL_PLAYERS, i-1, NULL);
        if (e && e->pchar && (e->pchar->karmaContainer.inZoneEvent == scriptId) &&
            isBucketActive(currentTime, e))
        {
            count++;
        }
    }
    return count;
}

int ScriptKarmaIsPlayerActive(int db_id)
{
    int currentTime = g_currentKarmaTime;

    Entity* e = entFromDbId(db_id);
    if (e && isBucketActive(currentTime, e))
    {
        return 1;
    }
    return 0;
}
#undef MIN_ACTIVE_AMOUNT

void ScriptUpdateBubbles(Entity *e)
{
    assert(e && e->pchar);
    if (e && e->pchar)
    {
        e->pchar->karmaContainer.numNearbyBubbles = 0;
        //    if in zone event and alive, count other players (otherwise i dont get other people's bubbles)
        if (e->pchar->karmaContainer.inZoneEvent && e->pchar->attrCur.fHitPoints)
        {
            int i;
            U32 currentTime = g_currentKarmaTime;
            for (i = NumEntitiesInTeam(ALL_PLAYERS); i > 0; i--)
            {
                Entity* mate = EntTeamInternal(ALL_PLAYERS, i-1, NULL);
                if (mate && mate->pchar)
                {
                    //    don't include self
                    if (e != mate)
                    {
                        //    only count people who have bubbles that are active
                        if ((mate->pchar->karmaContainer.inZoneEvent==e->pchar->karmaContainer.inZoneEvent) && mate->pchar->attrCur.fHitPoints)
                        {
                            if (((int)(currentTime - mate->pchar->karmaContainer.bubbleActivatedTimestamp)) < g_ZoneEventKarmaTable.powerBubbleDur)
                            {
                                //    only count people who are near self
                                if (distance3Squared(ENTPOS(e), ENTPOS(mate)) < (g_ZoneEventKarmaTable.powerBubbleRad*g_ZoneEventKarmaTable.powerBubbleRad))
                                {
                                    e->pchar->karmaContainer.numNearbyBubbles++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
#define KARMA_STAT_POLLING_INTERVAL        10
void ScriptKarmaPoll(int scriptId, int currentTime)
{
    int i;
    ScriptKarmaBucketList *karmaBucketList = getCurrentScriptKarmaBuckets(scriptId);
    for (i = 0; i < eaSize(&karmaBucketList->scriptKarmaBuckets); ++i)
    {
        ScriptKarmaBucket *bucket = karmaBucketList->scriptKarmaBuckets[i];
        if (bucket && ((bucket->db_id % KARMA_STAT_POLLING_INTERVAL) == (currentTime % KARMA_STAT_POLLING_INTERVAL)))
        {
            Entity *e = entFromDbId(bucket->db_id);
            if (e && e->pchar)
            {
                //    if they've started to use their pardon
                if (bucket->usedPardonTime)
                {
                    //    and they still have time remaining
                    if (bucket->usedPardonTime < g_ZoneEventKarmaTable.pardonDuration)
                    {
                        //    if they come back, they've forfeit the rest of it
                        //    but get a grace return period
                        bucket->usedPardonTime = g_ZoneEventKarmaTable.pardonDuration;
                        bucket->pardonReturnGrace = g_ZoneEventKarmaTable.pardonGrace;
                    }
                }
                if (!bucket->pardonReturnGrace)
                {
                    int currentKarma = CharacterCalculateKarma(e->pchar, currentTime);
                    ScriptKarmaBucket *bucket = ScriptGetBucketForPlayer(e, scriptId);
                    int currentStatSize = bucket ? eaiSize(&bucket->karmaArrays[SKT_SORTED_STAT].karma) : 0;
                    //    don't start adding karma until player has karma to add (this is to allow for travel/setup time at the start of stages. Once it starts.. don't stop =P
                    if (currentKarma || currentStatSize)
                        ScriptAddStatToPlayer(e, scriptId, SKT_SORTED_STAT);
                }
                else
                {
                    bucket->pardonReturnGrace = MAX(bucket->pardonReturnGrace - KARMA_STAT_POLLING_INTERVAL, 0);
                }
            }
            else
            {
                //    if ent is using their pardon
                if (bucket->usedPardonTime < g_ZoneEventKarmaTable.pardonDuration)
                {
                    bucket->usedPardonTime += KARMA_STAT_POLLING_INTERVAL;
                    if (bucket->usedPardonTime > g_ZoneEventKarmaTable.pardonDuration)        //    ran out of pardon time, clear their bucket
                        eaiClear(&bucket->karmaArrays[SKT_SORTED_STAT].karma);
                }
                else
                {
                    //    if they have no more pardon time, they just get zeroes =[[
                    eaiSortedPush(&bucket->karmaArrays[SKT_SORTED_STAT].karma, 0);
                }
            }
        }
    }
}

void ScriptSetKarmaContainer(ENTITY player)
{
    Entity *e = EntTeamInternal(player, 0, NULL);
    if(e && e->pchar && g_currentScript)
        e->pchar->karmaContainer.inZoneEvent = g_currentScript->id;
}

void ScriptClearKarmaContainer(ENTITY player)
{
    Entity *e = EntTeamInternal(player, 0, NULL);
    if(e && e->pchar)
        e->pchar->karmaContainer.inZoneEvent = 0;
}

void ScriptGlowieBubbleActivate(ENTITY activator, ENTITY player)
{
    Entity *source = EntTeamInternal(activator, 0, NULL);
    Entity *target = EntTeamInternal(player, 0, NULL);
    if (source && target && (ENTTYPE(target) == ENTTYPE_PLAYER) && target->pchar)
    {
        if (distance3Squared(ENTPOS(source), ENTPOS(target)) < (g_ZoneEventKarmaTable.powerBubbleRad*g_ZoneEventKarmaTable.powerBubbleRad))
            target->pchar->karmaContainer.glowieBubbleActivatedTimestamp = g_currentKarmaTime;
    }
}

static int PlayerRankComparator(const void* aData, const void* bData)
{
    const PlayerKarmaData ** a = (const PlayerKarmaData **)aData;
    const PlayerKarmaData ** b = (const PlayerKarmaData **)bData;
    return ((*a)->points > (*b)->points ) ? 1 : -1; 
}

#define REWARDMETHOD SKT_AVERAGE
static int rewardComparator(const void* aData, const void* bData)
{
    const ScriptReward ** a = (const ScriptReward **)aData;
    const ScriptReward ** b = (const ScriptReward **)bData;
    if ((*a)->numStagesRequired == (*b)->numStagesRequired)
    {
        devassert(
            (((*a)->percentileNeeded < 0) && ((*b)->percentileNeeded < 0)) || 
            (    ((*a)->pointsNeeded > (*b)->pointsNeeded ) == ((*a)->percentileNeeded > (*b)->percentileNeeded)));
        return ((*a)->pointsNeeded > (*b)->pointsNeeded ) ? 1 : -1; 
    }
    else if ((*a)->numStagesRequired < 0)
    {
        return 1;
    }
    else if ((*b)->numStagesRequired < 0)
    {
        return -1;
    }
    else
    {
        return ((*a)->numStagesRequired > (*b)->numStagesRequired ) ? 1 : -1; 
    }
}

void HandleRewardGroup(ScriptReward **rewards, int* stageThreshs, int ZERewardGroup, const char* scriptName)
{
    devassert(rewards);
    if (!rewards)
    {
        return;
    }
    else
    {
        devassert(eaSize(&rewards));
        if (eaSize(&rewards))
        {
            int i;
            PlayerKarmaData **rankList = NULL;
            int numPlayers = 0;
            int numStages = eaiSize(&stageThreshs);

            ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"Event %s | stage %i", scriptName ? textStd(scriptName) : "none", numStages);
            ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"\tCulling players by thresholds.");
            //    add every player in the zone event to the rank list to be sorted later
            for (i = NumEntitiesInTeam(ALL_PLAYERS); i > 0; --i )
            {
                KarmaEventHistory *history = NULL;
                Entity *e = EntTeamInternal(ALL_PLAYERS, i-1, 0);
                if (e && e->pchar)
                {
                    ScriptKarmaBucket *bucket = ScriptGetBucketForPlayer(e, g_currentScript->id);
                    if (bucket)
                    {
                        PlayerKarmaData *playerKarmaData = malloc(sizeof(PlayerKarmaData));
                        //    if the player has all the tokens for each event
                        int stageCountFromBucket = 0;
                        int total = 0;
                        int count = 0;
                        int averageStageKarma = 0;

                        if (ZERewardGroup && stageThreshs)
                        {
                            int j;
                            devassert(eaiSize(&bucket->karmaArrays[REWARDMETHOD].karma) == eaiSize(&bucket->karmaArrays[SKT_NUM].karma));
                            history = eventHistory_CreateFromEnt(e);
                            if (history)
                            {
                                history->timeStamp = SecondsSince2000();
                                history->eventName = strdup(scriptName ? textStd(scriptName) : "none");
                                history->pauseDuration = bucket->usedPardonTime;
                            }
                            ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"\t\tPlayer %s", e->name);
                            for (j = 0; j < eaiSize(&bucket->karmaArrays[REWARDMETHOD].karma); j++)
                            {
                                int numSamples = bucket->karmaArrays[SKT_NUM].karma[j];
                                int stageKarma = bucket->karmaArrays[REWARDMETHOD].karma[j];

                                history->stageSamples[j] = numSamples;
                                history->stageScore[j] = stageKarma;
                                history->stageThreshold[j] = stageThreshs[j];

                                ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"\t\t\tStage %i participation: %i (num samples %i)", j+1, stageKarma, numSamples);
                                total += (stageKarma*numSamples);
                                count += numSamples;
                                if (stageKarma >= stageThreshs[j])
                                {
                                    stageCountFromBucket++;
                                    ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"\t\t\tThreshold: %i - Qualified for stage", stageThreshs[j]);
                                }
                                else
                                {
                                    ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"\t\t\tThreshold: %i - Failed for stage", stageThreshs[j]);
                                }
                            }

                            if (history)
                            {
                                history->qualifiedStages = stageCountFromBucket;
                                history->playedStages = numStages;
                            }

                            devassert(stageCountFromBucket <= numStages);    //    if this is false without cheating, we aren't resetting things properly
                            ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"\t\tParticipated in %i/%i stages", stageCountFromBucket, numStages);
                            ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"\t\t-- -- -- -- ");
                            //    average them together and get his karma score
                            if (count)
                            {
                                averageStageKarma = total/count;
                            }
                        }
                        else
                        {
                            switch (REWARDMETHOD)
                            {
                            case SKT_AVERAGE:
                                averageStageKarma = ScriptKarmaGetAverage(e, g_currentScript->id, SKT_SORTED_STAT);
                            break;
                            case SKT_MEDIAN:
                                averageStageKarma = ScriptKarmaGetMedian(e, g_currentScript->id, SKT_SORTED_STAT);
                            break;
                            default:
                                devassert(0);
                            }
                            stageCountFromBucket = numStages;
                        }
                        playerKarmaData->points = averageStageKarma;
                        playerKarmaData->stages = stageCountFromBucket;
                        playerKarmaData->ePtr = e;
                        playerKarmaData->hPtr = history;
                        eaPush(&rankList, playerKarmaData);
                    }
                }
            }

            //    the table needs to be sorted
            eaQSort(rewards, rewardComparator);
            eaQSort(rankList, PlayerRankComparator);

            numPlayers = eaSize(&rankList);
            if (numPlayers)
            {
                for (i = 0; i < eaSize(&rewards); ++i)
                {
                    ScriptReward *reward = rewards[i];
                    if (reward->percentileNeeded >= 0)
                    {
                        //    player 0 is 0 percentile
                        //    player n-1 is 100 percentile
                        float percentile = (reward->percentileNeeded)*(numPlayers-1);
                        int lower = (int)percentile;
                        int upper = ceil(percentile);
                        reward->pointsNeededByPercentile = (rankList[lower]->points + rankList[upper]->points)/2.0f;
                    }
                    else
                    {
                        reward->pointsNeededByPercentile = rankList[numPlayers-1]->points+1;
                    }
                }
            }
            ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"\t*********");
            ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"\tRewarding players by rank");
            for (i = 0; i < numPlayers; ++i)
            {
                PlayerKarmaData *playerKarmaData= rankList[i];

                devassert(playerKarmaData->ePtr);
                if (playerKarmaData->ePtr)
                {
                    int rewardedPlayer = 0;
                    int j;
                    int numStageBracket = -1;
                    const char *ATName = "UnknownString";
                    const char *primary = "UnknownString";
                    const char *secondary = "UnknownString";
                    int percentileRank = (numPlayers > 1) ? (int)(i*100.f/(numPlayers-1)) : 100;
                    if (playerKarmaData && playerKarmaData->ePtr && playerKarmaData->ePtr->pchar)
                    {
                        Character *pchar = playerKarmaData->ePtr->pchar;
                        const CharacterClass *pclass = pchar->pclass;
                        if (pclass && pclass->pchDisplayName)
                        {
                            ATName =pclass->pchDisplayName;
                        }
                        for (j = 0; j < eaSize(&pchar->ppPowerSets); ++j)
                        {
                            if (pchar->ppPowerSets[j]->psetBase->pcatParent == pclass->pcat[kCategory_Primary])
                            {
                                primary = pchar->ppPowerSets[j]->psetBase->pchDisplayName;
                            }
                            if (pchar->ppPowerSets[j]->psetBase->pcatParent == pclass->pcat[kCategory_Secondary])
                            {
                                secondary = pchar->ppPowerSets[j]->psetBase->pchDisplayName;
                            }
                        }
                    }
                    ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"\t\tPlayer: %s | Rank: %i | AT: %s", playerKarmaData->ePtr->name, i+1, textStd(ATName));
                    ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"\t\tPrimary: %s | Secondary: %s", textStd(primary), textStd(secondary));
                    ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"\t\tAverage Karma Points: %i | Percentile Rank: %i", playerKarmaData->points, percentileRank );

                    if (playerKarmaData->hPtr)
                    {
                        playerKarmaData->hPtr->avgStagePoints = playerKarmaData->points;
                        playerKarmaData->hPtr->avgStagePercentile = percentileRank;
                    }

                    for (j = eaSize(&rewards)-1; j >= 0; --j)
                    {
                        ScriptReward *reward = rewards[j];
                        int numStagesReq = (reward->numStagesRequired < 0) ? numStages : reward->numStagesRequired;        //    if negative, assume they need all stages
                        if (numStageBracket >= 0)
                        {
                            if (numStagesReq < numStageBracket)
                            {
                                //    there is a gap between teh threshhold and the lowest reward
                                devassertmsg(0, "Dropped below rewardgroup bracket without received a reward. Check the reward defs");
                                break;
                            }
                        }
                        //    if the karma score is enough for each 
                        //    (this relies on the reward group table being sorted)
                        if (playerKarmaData->stages >= numStagesReq)
                        {
                            int hitByPoints = playerKarmaData->points >= reward->pointsNeeded;
                            int hitByPercentile = playerKarmaData->points >= reward->pointsNeededByPercentile;
                            if (numStageBracket < 0)    
                            {
                                numStageBracket = numStagesReq;
                            }

                            //    check against the reward dispensing values
                            //     if matching, dispense award
                            if (hitByPoints || hitByPercentile)
                            {
                                ENTITY playerName = EntityNameFromEnt(playerKarmaData->ePtr);
                                EntityGrantReward(playerName, reward->rewardTable);

                                if (playerKarmaData->hPtr)
                                {
                                    if (hitByPoints)
                                    {
                                        playerKarmaData->hPtr->whichTable = KARMA_TABLE_BAND;
                                        playerKarmaData->hPtr->bandTable = strdup(reward->rewardTable);
                                        }
                                    else
                                    {
                                        playerKarmaData->hPtr->whichTable = KARMA_TABLE_PERCENTILE;
                                        playerKarmaData->hPtr->percentileTable = strdup(reward->rewardTable);
                                    }
                                }

                                rewardedPlayer = 1;
                                if (ZERewardGroup)
                                {
                                    ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"\t\t\tAwarded reward table: %s", reward->rewardTable);
                                    ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"\t\t\t|%c|Points Needed design (%i)", hitByPoints ? 'x' : ' ', reward->pointsNeeded);
                                    ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"\t\t\t|%c|Points Needed percentile: %f(scaled points: %f)", hitByPercentile ? 'x' : ' ', reward->percentileNeeded, reward->pointsNeededByPercentile);
                                }
                                break;
                            }
                        }
                    }
                    if (!rewardedPlayer)
                    {
                        if (playerKarmaData->hPtr)
                            playerKarmaData->hPtr->whichTable = KARMA_TABLE_NONE;

                        ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"\t\tDid not qualify for a reward.");
                    }
                    if (ZERewardGroup)
                        ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"\t\t-- -- -- -- ");

                    if (playerKarmaData && playerKarmaData->hPtr)
                        eventHistory_SendToDB(playerKarmaData->hPtr);
                }
            }

            for (i = eaSize(&rankList)-1; i >= 0; --i)
            {
                if (rankList[i]->hPtr)
                    eventHistory_Destroy(rankList[i]->hPtr);

                free(rankList[i]);
            }
            eaDestroy(&rankList);
            ScriptedZoneEventSendDebugPrintf(SZE_Log_Reward,"Done handing out rewards");
        }
    }
}

void ScriptEnableKarma()
{
    if (g_currentScript)
        eaiPush(&g_zoneEventScriptStageAwardsKarma, g_currentScript->id);
}

void ScriptDisableKarma()
{
    if (g_currentScript)
        eaiFindAndRemove(&g_zoneEventScriptStageAwardsKarma, g_currentScript->id);
}

void ScriptResetKarma()
{
    ScriptDestroyKarmaBucketsForAllPlayers(SKRT_ALL_KARMA);
    VarSetNumber("LastKarmaUpdate", 0);
}

bool IsKarmaEnabled()
{
    if (g_currentScript)
        return (eaiFind(&g_zoneEventScriptStageAwardsKarma, g_currentScript->id) != -1);
    return false;
}

void ScriptUpdateKarma(bool pause)
{
    int i;
    if (!VarGetNumber("LastKarmaUpdate"))
    {
        VarSetNumber("LastKarmaUpdate", g_currentKarmaTime-1);
    }
    if (VarGetNumber("LastKarmaUpdate") != g_currentKarmaTime)        //    this check makes sure we don't have 2 ticks in one second
    {
        //    now, we need to account for slow ticks (and regular ticks)
        //    set time to lastTime+1 and go until currentTime catches up (and then reset is after)
        U32 saveCurrentTime = g_currentKarmaTime;
        devassert(((int)(g_currentKarmaTime - VarGetNumber("LastKarmaUpdate"))) > 0);
        g_currentKarmaTime = VarGetNumber("LastKarmaUpdate")+1;
        while (((int)(saveCurrentTime - g_currentKarmaTime)) >= 0)
        {
            //    This has to be all players or else the bucket will pause when the entity leaves the volume
            for (i = NumEntitiesInTeam(ALL_PLAYERS); i > 0; i--)
            {
                Entity* e = EntTeamInternal(ALL_PLAYERS, i-1, NULL);
                if (e && e->pchar)
                {
                    if ((e->db_id % KARMA_BUBBLE_POLLING_INTERVAL) == (g_currentKarmaTime % KARMA_BUBBLE_POLLING_INTERVAL))
                    {
                        ScriptUpdateBubbles(e);
                    }
                    if (ScriptGetBucketForPlayer(e, g_currentScript->id) == NULL)
                    {
                        ScriptAddBucket(e, g_currentScript->id);
                    }
                }
            }
            if (!pause)
                ScriptKarmaPoll(g_currentScript->id, g_currentKarmaTime);
            
            g_currentKarmaTime++;
        }
        g_currentKarmaTime = saveCurrentTime;
        VarSetNumber("LastKarmaUpdate", g_currentKarmaTime);
    }
}

void ScriptAddObjectiveKarma(ENTITY player, NUMBER teamKarmaValue, NUMBER teamKarmaLife, NUMBER allKarmaValue, NUMBER allKarmaLife)
{
    ScriptAddKarmaToTeam(player, CKR_TEAM_OBJ_COMPLETE, teamKarmaValue, teamKarmaLife);
    ScriptAddKarmaToAllPlayers(CKR_ALL_OBJ_COMPLETE, allKarmaValue, allKarmaLife );
}