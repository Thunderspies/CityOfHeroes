#include <utilitieslib/stdtypes.h>
#include "container/containerEventHistory.h"
#include <utilitieslib/utils/error.h>
#include "sql_fifo.h"
#include "dbinit.h"
#include "container_sql.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/utils/utils.h>
#include "comm_backend.h"
#include "namecache.h"
#include "dbserver/servercfg.h"

#include "dbEventHistory.h"

STATIC_ASSERT(KARMA_EVENT_MAX_STAGES == 30);

typedef struct EventHistoryCacheItem
{
    int containerId;
    KarmaEventHistory *event;
} EventHistoryCacheItem;

static EventHistoryCacheItem **eventhistory_cache;

// The highest ID in the cache. That is, the highest ID we've received from
// a SQL query.
static int s_highestContainerId = 0;

// The highest ID updated in the DbServer. If this is higher than the
// variable above, we need to run a SQL query to get up to date.
static int s_lastContainerId = 0;

// This is every column in the table. We need them all in order to cache
// them. If the number of stages changes this will also need to be changed
// hence the static assert above.
static char *all_columns =
    "ContainerID, TimeStamp, EventName, AuthName, Name, DBID, Level, "
    "Class, PrimaryPowerset, SecondaryPowerset, "
    "BandTable, PercentileTable, OfferedTable, RewardChosen, "
    "StagesPlayed, StagesQualified, StagesAvgScore, StagesAvgPercentile, "
    "TimePaused, StagesPaused, "

    "StageScore1, StageThreshold1, StageRank1, StagePercentile1, StageSamples1, "
    "StageScore2, StageThreshold2, StageRank2, StagePercentile2, StageSamples2, "
    "StageScore3, StageThreshold3, StageRank3, StagePercentile3, StageSamples3, "
    "StageScore4, StageThreshold4, StageRank4, StagePercentile4, StageSamples4, "
    "StageScore5, StageThreshold5, StageRank5, StagePercentile5, StageSamples5, "
    "StageScore6, StageThreshold6, StageRank6, StagePercentile6, StageSamples6, "
    "StageScore7, StageThreshold7, StageRank7, StagePercentile7, StageSamples7, "
    "StageScore8, StageThreshold8, StageRank8, StagePercentile8, StageSamples8, "
    "StageScore9, StageThreshold9, StageRank9, StagePercentile9, StageSamples9, "
    "StageScore10, StageThreshold10, StageRank10, StagePercentile10, StageSamples10, "
    "StageScore11, StageThreshold11, StageRank11, StagePercentile11, StageSamples11, "
    "StageScore12, StageThreshold12, StageRank12, StagePercentile12, StageSamples12, "
    "StageScore13, StageThreshold13, StageRank13, StagePercentile13, StageSamples13, "
    "StageScore14, StageThreshold14, StageRank14, StagePercentile14, StageSamples14, "
    "StageScore15, StageThreshold15, StageRank15, StagePercentile15, StageSamples15, "
    "StageScore16, StageThreshold16, StageRank16, StagePercentile16, StageSamples16, "
    "StageScore17, StageThreshold17, StageRank17, StagePercentile17, StageSamples17, "
    "StageScore18, StageThreshold18, StageRank18, StagePercentile18, StageSamples18, "
    "StageScore19, StageThreshold19, StageRank19, StagePercentile19, StageSamples19, "
    "StageScore20, StageThreshold20, StageRank20, StagePercentile20, StageSamples20, "
    "StageScore21, StageThreshold21, StageRank21, StagePercentile21, StageSamples21, "
    "StageScore22, StageThreshold22, StageRank22, StagePercentile22, StageSamples22, "
    "StageScore23, StageThreshold23, StageRank23, StagePercentile23, StageSamples23, "
    "StageScore24, StageThreshold24, StageRank24, StagePercentile24, StageSamples24, "
    "StageScore25, StageThreshold25, StageRank25, StagePercentile25, StageSamples25, "
    "StageScore26, StageThreshold26, StageRank26, StagePercentile26, StageSamples26, "
    "StageScore27, StageThreshold27, StageRank27, StagePercentile27, StageSamples27, "
    "StageScore28, StageThreshold28, StageRank28, StagePercentile28, StageSamples28, "
    "StageScore29, StageThreshold29, StageRank29, StagePercentile29, StageSamples29, "
    "StageScore30, StageThreshold30, StageRank30, StagePercentile30, StageSamples30"
;

extern AttributeList *var_attrs;

static void AddToCache(int container_id, KarmaEventHistory *event)
{
    EventHistoryCacheItem *item = NULL;

    if (event && container_id)
    {
        item = malloc(sizeof(EventHistoryCacheItem));

        item->containerId = container_id;
        item->event = event;

        eaPush(&eventhistory_cache, item);

        if (s_highestContainerId < container_id)
            s_highestContainerId = container_id;
    }
}

static void UpdateCache(int min_container_id, U32 min_timestamp)
{
    int row_count = 0;
    char *cols = NULL;
    ColumnInfo    *field_ptrs[200];    // 150 for all the stages, plus sundry
    char restriction[200];
    int idx = 0;
    int count = 0;
    int stage = 0;
    KarmaEventHistory *entry = NULL;
    int container_id;
    int attr;

    if (min_container_id && min_timestamp)
        sprintf(restriction, "WHERE ContainerID > %d AND TimeStamp > %u", min_container_id, min_timestamp);
    else if (min_container_id)
        sprintf(restriction, "WHERE ContainerID > %d", min_container_id);
    else if (min_timestamp)
        sprintf(restriction, "WHERE TimeStamp > %u", min_timestamp);
    else
        restriction[0] = '\0';


    cols = sqlReadColumnsSlow(eventhistory_list->tplt->tables,"",all_columns,restriction,&row_count,field_ptrs);

    for (count = 0; count < row_count; count++)
    {
        entry = calloc(1, sizeof(KarmaEventHistory));

        if (entry)
        {
            // ContainerID
            container_id = *(int *)(&cols[idx]);
            idx += field_ptrs[0]->num_bytes;
            // Timestamp
            entry->timeStamp = *(int *)(&cols[idx]);
            idx += field_ptrs[1]->num_bytes;
            // Event name
            entry->eventName = strdup(&cols[idx]);
            idx += field_ptrs[2]->num_bytes;
            // Auth name
            entry->authName = strdup(&cols[idx]);
            idx += field_ptrs[3]->num_bytes;
            // Player name
            entry->playerName = strdup(&cols[idx]);
            idx += field_ptrs[4]->num_bytes;
            // DBID
            entry->dbid = *(int *)(&cols[idx]);
            idx += field_ptrs[5]->num_bytes;
            // Level
            entry->playerLevel = *(int *)(&cols[idx]);
            idx += field_ptrs[6]->num_bytes;
            // Class
            attr = *(int *)(&cols[idx]);
            idx += field_ptrs[7]->num_bytes;
            entry->playerClass = strdup(var_attrs->names[attr]);
            // Primary powerset
            attr = *(int *)(&cols[idx]);
            idx += field_ptrs[8]->num_bytes;
            entry->playerPrimary = strdup(var_attrs->names[attr]);
            // Secondary powerset
            attr = *(int *)(&cols[idx]);
            idx += field_ptrs[9]->num_bytes;
            entry->playerSecondary = strdup(var_attrs->names[attr]);
            // Band table
            entry->bandTable = strdup(&cols[idx]);
            idx += field_ptrs[10]->num_bytes;
            // Percentile table
            entry->percentileTable = strdup(&cols[idx]);
            idx += field_ptrs[11]->num_bytes;
            // Which table offered
            entry->whichTable = *(int *)(&cols[idx]);
            idx += field_ptrs[12]->num_bytes;
            // Reward chosen
            entry->rewardChosen = strdup(&cols[idx]);
            idx += field_ptrs[13]->num_bytes;
            // number stages played
            entry->playedStages = *(int *)(&cols[idx]);
            idx += field_ptrs[14]->num_bytes;
            // Number of stages over threshold
            entry->qualifiedStages = *(int *)(&cols[idx]);
            idx += field_ptrs[15]->num_bytes;
            // Average points per stage
            entry->avgStagePoints = *(int *)(&cols[idx]);
            idx += field_ptrs[16]->num_bytes;
            // Average percentile per stage
            entry->avgStagePercentile = *(int *)(&cols[idx]);
            idx += field_ptrs[17]->num_bytes;
            // Total time paused (if any)
            entry->pauseDuration = *(int *)(&cols[idx]);
            idx += field_ptrs[18]->num_bytes;
            // Stages given credit for while paused
            entry->pausedStagesCredited = *(int *)(&cols[idx]);
            idx += field_ptrs[19]->num_bytes;

            for (stage = 0; stage < KARMA_EVENT_MAX_STAGES; stage++)
            {
                entry->stageScore[stage] = *(int *)(&cols[idx]);
                idx += field_ptrs[20 + stage * 5]->num_bytes;
                entry->stageThreshold[stage] = *(int *)(&cols[idx]);
                idx += field_ptrs[21 + stage * 5]->num_bytes;
                entry->stageRank[stage] = *(int *)(&cols[idx]);
                idx += field_ptrs[22 + stage * 5]->num_bytes;
                entry->stagePercentile[stage] = *(int *)(&cols[idx]);
                idx += field_ptrs[23 + stage * 5]->num_bytes;
                entry->stageSamples[stage] = *(int *)(&cols[idx]);
                idx += field_ptrs[24 + stage * 5]->num_bytes;
            }

            // We know this isn't a duplicate because we're just filling
            // the list from the database.
            AddToCache(container_id, entry);
        }
    }

    // We have everything from the SQL DB.
    s_lastContainerId = s_highestContainerId;
}

static void outputResult(int idx, KarmaEventHistory *result, Packet *pak_out)
{
    char buf[5000];
    char *name;
    char timestamp_note[100];
    char name_note[50];
    char pause_note[50];
    char reward_note[100];
    int stage;
    int stagesOutput;
    char stage_info[100];

    if (result)
    {
        struct tm tms;

        timerMakeTimeStructFromSecondsSince2000(result->timeStamp, &tms);
        sprintf(timestamp_note, "%4d-%02d-%02d:%02d:%02d", tms.tm_year + 1900,
            tms.tm_mon + 1, tms.tm_mday, tms.tm_hour, tms.tm_min);

        name = pnameFindById(result->dbid);

        if (!name)
            sprintf(name_note, ", deleted");
        else if (stricmp(name, result->playerName) != 0)
            sprintf(name_note, ", renamed to %s", name);
        else
            name_note[0] = '\0';

        if (result->pauseDuration)
        {
            sprintf(pause_note, "paused %d secs (%d credits)",
                result->pauseDuration, result->pausedStagesCredited);
        }
        else
        {
            if (result->pausedStagesCredited)
                sprintf(pause_note, "no pause but %d credit (error!)",
                    result->pausedStagesCredited);
            else
                sprintf(pause_note, "no pause");
        }

        if (result->whichTable == KARMA_TABLE_NONE)
            sprintf(reward_note, "no reward (%s nor %s)",
            result->bandTable, result->percentileTable);
        else if (result->whichTable == KARMA_TABLE_BAND)
            sprintf(reward_note, "band reward table %s (other %s)",
                result->bandTable, result->percentileTable);
        else if (result->whichTable == KARMA_TABLE_PERCENTILE)
            sprintf(reward_note, "percentile reward table %s (other %s)",
                result->percentileTable, result->bandTable);
        else
            sprintf(reward_note, "corrupt reward (error!)");

        sprintf(buf, "%2d) %s @ %s - %s:%s DBid(%d%s) Lvl %d (base 1)\nAT %s|Primary %s|Secondary %s\n\t%d stages (%d qualified), Avg %d (%d%%), %s, %s\n",
            idx, result->eventName, timestamp_note,
            result->authName, result->playerName, result->dbid,
            name_note, result->playerLevel+1, result->playerClass,
            result->playerPrimary, result->playerSecondary,
            result->playedStages, result->qualifiedStages,
            result->avgStagePoints, result->avgStagePercentile,
            pause_note, reward_note);

        stagesOutput = 0;

        for (stage = 0; stage < KARMA_EVENT_MAX_STAGES; stage++)
        {
            // If the stage is playable and has any participation
            // by this player we should list their performance.
            if (result->stageSamples[stage] > 0 &&
                result->stageThreshold[stage] > 0)
            {
                char *passfail;

                if (result->stageScore[stage] >= result->stageThreshold[stage])
                    passfail = "pass";
                else
                    passfail = "fail";

                sprintf(stage_info, "\t\t%2d) %d (avg of %d samples) vs threshold %d (%s)",
                    stage + 1, result->stageScore[stage], result->stageSamples[stage],
                    result->stageThreshold[stage], passfail);

                strncatt(buf, stage_info);

                if (stagesOutput % 2 == 1)
                    strncatt(buf, "\n");

                stagesOutput++;
            }
        }

        // Finish off the last line of stage data if necessary.
        if (stagesOutput && stagesOutput % 2 == 0)
            strncatt(buf, "\n");

        pktSendString(pak_out, buf);
    }
}

void dbEventHistory_Init(void)
{
    if (server_cfg.eventhistory_cache_days)
    {
        loadstart_printf("Reading last %d days of event history records...", server_cfg.eventhistory_cache_days);

        // Seconds per day times 180 days to get six months.
        UpdateCache(0, timerSecondsSince2000() - server_cfg.eventhistory_cache_days*(60*60*24));

        loadend_printf("\n\tLoaded %d records", eaSize(&eventhistory_cache));
    }
}

void dbEventHistory_StatusCb(DbContainer *container, char *buf)
{
    if (container && container->id > s_lastContainerId)
        s_lastContainerId = container->id;
}

void dbEventHistory_UpdateCb(DbContainer *container)
{
    if (container && container->id > s_lastContainerId)
        s_lastContainerId = container->id;
}

void handleEventHistoryFind(Packet *pak, NetLink *link)
{
    U32 min_time = 0;
    U32 max_time = 0;
    U32 sender_dbid = 0;
    char *auth = NULL;
    char *player = NULL;
    KarmaEventHistory **results = NULL;
    Packet *pak_out = NULL;
    int count;

    sender_dbid = pktGetBitsAuto(pak);
    min_time = pktGetBitsAuto(pak);
    max_time = pktGetBitsAuto(pak);
    strdup_alloca(auth, pktGetString(pak));
    strdup_alloca(player, pktGetString(pak));

    results = dbEventHistory_Find(min_time, max_time, auth, player);

    if (results)
    {
        pak_out = pktCreateEx(link, DBSERVER_EVENTHISTORY_RESULTS);

        // MapServer needs to know where to send the results.
        pktSendBitsAuto(pak_out, sender_dbid);

        if (eaSize(&results) > 99)
        {
            char str[100];

            sprintf(str, "This query would return %d results. The maximum allowed is 100.\n", eaSize(&results));

            pktSendString(pak_out, str);
        }
        else
        {
            for (count = 0; count < eaSize(&results); count++)
            {
                outputResult(count + 1, results[count], pak_out);
            }
        }

        // Empty string marks the end of the report.
        pktSendString(pak_out, "");

        pktSend(&pak_out, link);

        eaDestroy(&results);
    }
}

KarmaEventHistory **dbEventHistory_Find(U32 min_time, U32 max_time, char *auth, char *player)
{
    KarmaEventHistory **retval = NULL;
    int count;
    KarmaEventHistory *event;

    // Nothing to do if the cache is disabled.
    if (!server_cfg.eventhistory_cache_days)
    {
        return NULL;
    }

    // Do we need to perform a lazy update and cache newly-submitted
    // event history rows?
    if (s_lastContainerId > s_highestContainerId)
    {
        UpdateCache(s_highestContainerId, 0);
    }

    for (count = 0; count < eaSize(&eventhistory_cache); count++)
    {
        event = NULL;

        if (eventhistory_cache[count])
            event = eventhistory_cache[count]->event;

        if (event && (!min_time || event->timeStamp > min_time) &&
            (!max_time || event->timeStamp < max_time) &&
            (!auth || !strlen(auth) || stricmp(auth, event->authName) == 0) &&
            (!player || !strlen(player) || stricmp(player, event->playerName) == 0))
        {
            eaPush(&retval, event);
        }
    }

    return retval;
}

