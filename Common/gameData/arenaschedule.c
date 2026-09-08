/*\
*
*    arenaschedule.h/c - Copyright 2004, 2005 Cryptic Studios
*        All Rights Reserved
*        Confidential property of Cryptic Studios
*
*    Scheduled events started by the arena server
*
 */

#include "arenaschedule.h"
#ifndef SERVER
    #include "arenaevent.h"
    #include "arenaserver.h"
#endif
#include <utilitieslib/components/earray.h>
#include <time.h>
#include <utilitieslib/utils/timing.h>
#include "dbcomm/dbcomm.h"
#include <utilitieslib/utils/log.h>

#define SCHEDULE_MIN_TIME_BEFORE_START        120        // we won't schedule an event within this many seconds of when it should go

int g_disableScheduledEvents = 0;                    // starting new scheduled events is disabled

static INLINEDBG int ScheduleHandleToIndex(int handle) { return handle - 1; }
static INLINEDBG int ScheduleIndexToHandle(int index) { return index + 1; }

typedef struct Schedules {
    ScheduledEvent** events;
} Schedules;
Schedules g_schedules;

StaticDefineInt ParseEventRecurrence[] =
{
    DEFINE_INT
    {"ASAP",            EVENTRECUR_ASAP},
    {"Hourly",            EVENTRECUR_HOURLY},
    {"Daily",            EVENTRECUR_DAILY},
    {"Weekly",            EVENTRECUR_WEEKLY},
    {"Monthly",            EVENTRECUR_MONTHLY},
    {"Never",            EVENTRECUR_NEVER},
    DEFINE_END
};

StaticDefineInt ParseDayOfWeek[] = // days since sunday, from time.h: struct tm
{
    DEFINE_INT
    {"Sunday",            0},
    {"Monday",            1},
    {"Tuesday",            2},
    {"Wednesday",        3},
    {"Thursday",        4},
    {"Friday",            5},
    {"Saturday",        6},
    DEFINE_END
};

StaticDefineInt ParseMonth[] = // again to fit struct tm
{
    DEFINE_INT
    {"January",            0},
    {"February",        1},
    {"March",            2},
    {"April",            3},
    {"May",                4},
    {"June",            5},
    {"July",            6},
    {"August",            7},
    {"September",        8},
    {"October",            9},
    {"November",        10},
    {"December",        11},
    DEFINE_END
};

TokenizerParseInfo ParseScheduledEvent[] =
{
    { "{",                TOK_START,       0                                                                          },
    { "Description",    TOK_STRING(ScheduledEvent, description, 0)                                },
    { "Weight",            TOK_INT(ScheduledEvent, weightClass,    0),        ParseArenaWeightClass    },
    { "TeamStyle",        TOK_INT(ScheduledEvent, teamStyle,        0),        ParseArenaTeamStyle        },
    { "TeamType",        TOK_INT(ScheduledEvent, teamType,        0),        ParseArenaTeamType        },
    { "VictoryType",    TOK_INT(ScheduledEvent, victoryType,    0),        ParseArenaVictoryType    },
    { "VictoryValue",    TOK_INT(ScheduledEvent, victoryValue,    0)                                },
    { "TournamentType",    TOK_INT(ScheduledEvent, tournamentType,    0),        ParseArenaTournamentType},
    { "MinPlayers",        TOK_INT(ScheduledEvent, minplayers, 0)                                  },
    { "MaxPlayers",        TOK_INT(ScheduledEvent, maxplayers, 0)                                  },
    { "Deprecate",        TOK_BOOLFLAG(ScheduledEvent, deprecated, 0)                                },
    { "DemandPlay",        TOK_BOOLFLAG(ScheduledEvent, demandplay, 0)                                },
    { "EntryFee",        TOK_INT(ScheduledEvent, entryfee, 0)                                    },

    { "Recurs",            TOK_INT(ScheduledEvent, recurs,        0),        ParseEventRecurrence        },
    { "Year",            TOK_INT(ScheduledEvent, year, 0)                                        },
    { "Month",            TOK_INT(ScheduledEvent, month,        0),        ParseMonth                    },
    { "Day",            TOK_INT(ScheduledEvent, day, 0)                                         },
    { "DayOfWeek",        TOK_INT(ScheduledEvent, dayofweek,    0),        ParseDayOfWeek                },
    { "Hour",            TOK_INT(ScheduledEvent, hour, 0)                                        },

    { "}",                TOK_END,         0 },
    { 0 }
};

TokenizerParseInfo ParseArenaSchedules[] =
{
    { "ScheduledEvent",        TOK_STRUCT(Schedules, events, ParseScheduledEvent) },
    { 0 }
};

void LoadScheduledEvents(void)
{
    ParserLoadFiles(NULL, "defs/generic/arenaschedule.def", "arenaschedule.bin",
        PARSER_SERVERONLY | 0, ParseArenaSchedules, &g_schedules, NULL, NULL, NULL);
    if (!g_schedules.events)
    {
        ArenaError("Error loading arena scheduled events\n");
    }
}

ScheduledEvent* ScheduledEventGet(int handle)
{
    int index = ScheduleHandleToIndex(handle);
    if (index < 0 || index >= eaSize(&g_schedules.events))
        return NULL;
    return g_schedules.events[index];
}

// every tick we look for events that need to get scheduled
static int eventHandleErrorsPosted = 0;
static int checkRunningEvents(ArenaEvent* event)
{
    if (event->serverEventHandle && event->phase == ARENA_SCHEDULED)
    {
        ScheduledEvent* sched = ScheduledEventGet(event->serverEventHandle);
        if (sched)
            sched->scheduled = 1;
        else
        {
            // don't want to fill the log with these errors
            if (eventHandleErrorsPosted < 20)
            {
                eventHandleErrorsPosted++;
                ArenaError("have an existing event with invalid handle.  event (%i,%i,%s), handle %i",
                    event->eventid, event->uniqueid, event->description, event->serverEventHandle);
            }
        }
    }
    return 1;
}

#ifndef SERVER
void ScheduledEventsTick(void)
{
    int i;

    if (g_disableScheduledEvents)
        return;

    for (i = 0; i < eaSize(&g_schedules.events); i++)
        g_schedules.events[i]->scheduled = 0;
    EventIter(checkRunningEvents);

    for (i = 0; i < eaSize(&g_schedules.events); i++)
    {
        if (!g_schedules.events[i]->scheduled && !g_schedules.events[i]->expired)
        {
            AddScheduledEvent(ScheduleIndexToHandle(i));
        }
    }
}
#endif

// give me the next time >= fromtime that this event can be scheduled,
// returns 0 for ASAP
// also can return 0 (failure) for a non-recurring event in the past
U32 ScheduleNextTime(ScheduledEvent* sched, U32 fromtime)
{
    U32 result;
    struct tm tms;

    timerMakeTimeStructFromSecondsSince2000(fromtime, &tms);
    tms.tm_min = tms.tm_sec = tms.tm_isdst = 0;    // don't care about these

    switch (sched->recurs)
    {
    case EVENTRECUR_HOURLY:
        tms.tm_hour = 0;
        result = timerGetSecondsSince2000FromTimeStruct(&tms);
        while (result < fromtime)
            result += 3600;
        return result;
        break;
    case EVENTRECUR_DAILY:
        tms.tm_hour = sched->hour;
        result = timerGetSecondsSince2000FromTimeStruct(&tms);
        while (result < fromtime)
            result += 3600 * 24;
        return result;
    case EVENTRECUR_WEEKLY:
        tms.tm_hour = sched->hour;

        // get the day of the week I want, then rectify
        tms.tm_mday -= tms.tm_wday + sched->dayofweek;
        if (tms.tm_mday < 1) 
            tms.tm_mday += 7;
        if (tms.tm_mday > 28)
            tms.tm_mday -= 7;
        result = timerGetSecondsSince2000FromTimeStruct(&tms);

        result -= 3600 * 24 * 7 * 2; // my rectify may have pushed me into future, fix it
        while (result < fromtime)
            result += 3600 * 24 * 7;
        return result;
    case EVENTRECUR_MONTHLY:
        tms.tm_hour = sched->hour;
        tms.tm_mday = sched->day;

        result = timerGetSecondsSince2000FromTimeStruct(&tms);
        if (result >= fromtime)
            return result;

        // otherwise, need to increment a calendar month
        tms.tm_mon++;
        if (tms.tm_mon == 12)
        {
            tms.tm_mon = 0;
            tms.tm_year++;
        }
        return timerGetSecondsSince2000FromTimeStruct(&tms);
    case EVENTRECUR_NEVER:
        tms.tm_hour = sched->hour;
        tms.tm_mday = sched->day;
        tms.tm_mon = sched->month;
        tms.tm_year = sched->year - 1900;
        result = timerGetSecondsSince2000FromTimeStruct(&tms);
        if (result >= fromtime)
            return result;
        else
            return 0;
    }
    return 0;
}

#ifndef SERVER
void AddScheduledEvent(int handle)
{
    ScheduledEvent* sched;
    ArenaEvent* event;
    U32 start_time;

    sched = ScheduledEventGet(handle);
    if (!sched)
    {
        ArenaError(__FUNCTION__ " bad handle");
        return;
    }

    // catch past due non-recurring events here
    start_time = ScheduleNextTime(sched, dbSecondsSince2000() + SCHEDULE_MIN_TIME_BEFORE_START);
    if (start_time == 0 && sched->recurs != EVENTRECUR_ASAP)
    {
        LOG_OLD( "Arena skipping event %s because it occurs in the past", ArenaLocalize(sched->description));
        sched->expired = 1;
        return;
    }

    event = EventAdd(ArenaLocalize(sched->description));
    if (!event)
    {
        ArenaError(__FUNCTION__ " not able to add an event");
        return;
    }

    event->weightClass = sched->weightClass;
    event->teamStyle = sched->teamStyle;
    event->teamType = sched->teamType;
    event->creatorid = 0;
    strcpy(event->creatorname, "System");    // MAKTODO - localize?
    event->minplayers = sched->minplayers;
    event->maxplayers = sched->maxplayers;
    event->victoryType = sched->victoryType;
    event->victoryValue = sched->victoryValue;
    event->tournamentType = sched->tournamentType;

    event->sanctioned = 1;
    event->scheduled = 1;
    event->listed = 1;
    EventChangePhase(event, ARENA_SCHEDULED);
    event->serverEventHandle = handle;
    event->demandplay = sched->demandplay;
    event->start_time = start_time;
    event->entryfee = sched->entryfee;
    EventSave(event->eventid);
}
#endif

#ifndef SERVER
// event failed to hit start conditions before the scheduled time:
// repeating events get rescheduled
// single-time events expire and return 0
// asap events get reset to asap status
int IncrementScheduledEvent(ArenaEvent* event)
{
    ScheduledEvent* sched;

    if (!event->scheduled || !event->serverEventHandle)
    {
        ArenaError(__FUNCTION__ " bad flags on event (%i,%i,%s)\n",
            event->eventid, event->uniqueid, event->description);
        return 0;    // don't know why I hit here
    }

    if (g_disableScheduledEvents)
    {
        printf("not rescheduling event %i (scheduled events disabled)\n", event->eventid);
        return 0;
    }

    sched = ScheduledEventGet(event->serverEventHandle);
    if (!sched || sched->deprecated)
    {
        return 0;    // event was scheduled by previous update, pitch if it didn't hit
    }

    if (sched->recurs == EVENTRECUR_NEVER)
    {
        return 0;    // missed our only opportunity
    }
    else if (sched->recurs == EVENTRECUR_ASAP)
    {
        event->start_time = 0;    // signal that this event is ASAP
        EventChangePhase(event, ARENA_SCHEDULED);
        return 1;
    }
    else
    {
        event->start_time = ScheduleNextTime(sched, dbSecondsSince2000() + SCHEDULE_MIN_TIME_BEFORE_START + ARENA_SERVER_NEGOTIATION_WAIT);
        EventChangePhase(event, ARENA_SCHEDULED);
        return 1;
    }
}
#endif
