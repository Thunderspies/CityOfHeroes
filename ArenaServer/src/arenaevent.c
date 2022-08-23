/*\
 *
 *    ArenaEvent.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Server logic for arena events
 *
 */

#include "arenaevent.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include "dbcomm/dbcontainer.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/error.h>
#include "arenaupdates.h"
#include "arenaserver.h"
#include "limits.h"
#include "dbcomm/dbcomm.h"
#include "entity/entVarUpdate.h"
#include "arenaplayer.h"
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/utils/mathutil.h>
#include "gameData/arenaschedule.h"
#include "arenaranking.h"
#include <utilitieslib/utils/log.h>

// *********************************************************************************
//  Global list of events
// *********************************************************************************

// unique id's get reset when the arenaserver loads.  they are basically
// a count of created events since the arenaserver started.  they aren't
// unique across restarts of the arenaserver, but events will no longer ever
// change a unique id
U32 g_nextuniqueid = 1;
static U32 GetUniqueId(void) { U32 ret = g_nextuniqueid++; if (g_nextuniqueid >= INT_MAX) g_nextuniqueid = 1; return ret; }
static void DeletingUniqueId(U32 id) { if (id > g_nextuniqueid && (id - g_nextuniqueid < 1000)) g_nextuniqueid = id + 1; }
    // try to prevent a situation where my unique id's are going to overlap this 'deleting' one soon

ArenaEventList g_events = {0};
extern ArenaRankingTable g_leaderboard[ARENA_NUM_RATINGS];
extern StaticDefineInt ParseArenaPhase[];

int g_disableEventLog = 1;                            // disable writing events to log file as they are destroyed

typedef struct ArenaRestriction
{
    int sanctioned;
    int minPlayers;
    int maxPlayers;
    int minSides;
    int maxSides;
    int minPlayersPerSide;
    int maxPlayersPerSide;
    ArenaVictoryType victoryType;
    int victoryValue;
    int restrictLevel;
} ArenaRestriction;

ArenaRestriction restrictions[ARENA_NUM_TEAM_STYLES][ARENA_NUM_TEAM_TYPES] = 
{        //sanctioned    //minPlayers    //maxPlayers    //minSides    //maxSides        //minPPS    // maxPPS            //duration
    {//ARENA_FREEFORM
        {    1,                2,                64,                0,            0,                0,            0,                    ARENA_TIMED,    10,        1    },    //solo duel
        {    1,                4,                64,                2,            8,                2,            8,                    ARENA_TIMED,    20,        1    },    //team duel
        {    1,                2,                128,            2,            8,                1,            64,                    ARENA_TIMED,    20,        1    },    //supergroup rumble
    },
    {//ARENA_STAR
        {    0,                0,                0,                0,            0,                0,            0,                    0,                0,        0    },    //                
        {    1,                10,                40,                2,            8,                5,            5,                    ARENA_TIMED,    20,        1    },    // STAR                
        {    0,                10,                40,                2,            8,                5,            5,                    ARENA_TIMED,    20,        1    },  //
    },
    {//ARENA_STAR_VILLAIN
        {    0,                0,                0,                0,            0,                0,            0,                    0,                0,        0    },    //
        {    0,                10,                40,                2,            8,                5,            5,                    ARENA_TIMED,    20,        1    },    // Villain STAR
        {    0,                10,                40,                2,            8,                5,            5,                    ARENA_TIMED,    20,        1    },    //
    },
    {//ARENA_STAR_VERSUS
        {    0,                0,                0,                0,            0,                0,            0,                    0,                0,        0    },    //
        {    0,                10,                40,                2,            8,                5,            5,                    ARENA_TIMED,    20,        1    },    // Versus STAR
        {    0,                10,                40,                2,            8,                5,            5,                    ARENA_TIMED,    20,        1    },    //
    },
    {//ARENA_7STAR
        {    0,                0,                0,                0,            0,                0,            0,                    0,                0,        0    },    //                
        {    0,                14,                56,                2,            8,                7,            7,                    ARENA_TIMED,    20,        1    },    // 7-STAR                
        {    0,                14,                56,                2,            8,                7,            7,                    ARENA_TIMED,    20,        1    },  //
    },
    {//ARENA_7STAR_VILLAIN
        {    0,                0,                0,                0,            0,                0,            0,                    0,                0,        0    },    //
        {    0,                14,                56,                2,            8,                7,            7,                    ARENA_TIMED,    20,        1    },    // Villain 7-STAR
        {    0,                14,                56,                2,            8,                7,            7,                    ARENA_TIMED,    20,        1    },    //
    },
    {//ARENA_7STAR_VERSUS
        {    0,                0,                0,                0,            0,                0,            0,                    0,                0,        0    },    //
        {    0,                14,                56,                2,            8,                7,            7,                    ARENA_TIMED,    20,        1    },    // Versus 7-STAR
        {    0,                14,                56,                2,            8,                7,            7,                    ARENA_TIMED,    20,        1    },    //
    },
};

static void arenaBalanceTeams(ArenaEvent * event, bool supergroup);
static void EventUpdateErrors( ArenaEvent *event);

// returns event
ArenaEvent* EventAdd(char* name)
{
    int i, n;
    
    // table scan for empty location
    if (!g_events.events)
    {
        eaCreate(&g_events.events);
        eaSetSize(&g_events.events, 10);
    }
    n = eaSize(&g_events.events);
    for (i = 1; i < n; i++)    // no zero event id
    {
        if (!g_events.events[i])
            break;
    }
    if (i >= n)
    {
        eaSetSize(&g_events.events, n*2);
    }

    // insert
    g_events.events[i] = ArenaEventCreate();
    g_events.events[i]->detailed = 1;
    g_events.events[i]->eventid = i;
    g_events.events[i]->uniqueid = GetUniqueId();
    strncpyt(g_events.events[i]->description, name, ARRAY_SIZE(g_events.events[i]->description));
    g_events.events[i]->teamStyle = ARENA_FREEFORM;
    g_events.events[i]->maxplayers = 128;    // todo, set this based on event type, etc..
    g_events.events[i]->inspiration_setting = ARENA_NORMAL_INSPIRATIONS;
    EventSave(i);
    return g_events.events[i];
}

ArenaEvent* EventGetAdd(U32 eventid, int initialLoad)
{
    if (!g_events.events)
    {
        eaCreate(&g_events.events);
        eaSetSize(&g_events.events, 10);
    }
    while (eventid >= (U32)eaSize(&g_events.events))
    {
        eaSetSize(&g_events.events, 2*eaSize(&g_events.events));
    }

    if (!g_events.events[eventid])
    {
        g_events.events[eventid] = ArenaEventCreate();
        g_events.events[eventid]->eventid = eventid;
    }
    if (!initialLoad)
        g_events.events[eventid]->uniqueid = GetUniqueId();
    return g_events.events[eventid];
}

ArenaEvent* EventGet(U32 eventid)
{
    if (eventid >= (U32)eaSize(&g_events.events))
        return NULL;
    return g_events.events[eventid];
}

int EventGetId(ArenaEvent *event)
{
    int eventIndex;
    int eventCount = eaSize(&g_events.events);

    for (eventIndex = eventCount - 1; eventIndex >= 0; eventIndex--)
    {
        if (g_events.events[eventIndex] == event)
        {
            return eventIndex;
        }
    }
    
    return eventCount;
}

ArenaEvent* EventGetByDbid(U32 dbid)
{
    int eventIndex, participantIndex;
    ArenaEvent *event;

    for (eventIndex = eaSize(&g_events.events) - 1; eventIndex >= 0; eventIndex--)
    {
        event = g_events.events[eventIndex];

        if (event)
        {
            for (participantIndex = eaSize(&event->participants) - 1; participantIndex >= 0; participantIndex--)
            {
                if (event->participants[participantIndex] && 
                    event->participants[participantIndex]->dbid == dbid)
                {
                    return event;
                }
            }
        }
    }
    
    return NULL;
}

ArenaEvent* EventGetUnique(U32 eventid, U32 uniqueid)
{
    if (eventid >= (U32)eaSize(&g_events.events)) 
        return NULL;
    if (!g_events.events[eventid])
        return NULL;
    if (g_events.events[eventid]->uniqueid != uniqueid)
        return NULL;
    return g_events.events[eventid];
}

void EventDelete(U32 eventid)
{
    int* dbids;
    ArenaClientList* list;
    int i, n;

    if (eventid >= (U32)eaSize(&g_events.events) || !g_events.events[eventid])
    {
        LOG_OLD_ERR("EventDelete - received bad eventid %i", eventid);
        return;
    }

    // note clients and dbids
    list = ClientListFromEvent(g_events.events[eventid], 0);
    DbidsFromEvent(&dbids, g_events.events[eventid]);

    // reserve this uniqueid in case I was about to use it
    DeletingUniqueId(g_events.events[eventid]->uniqueid);

    // destroy event
    ArenaEventDestroy(g_events.events[eventid]);
    g_events.events[eventid] = NULL;
    EventSave(eventid);

    // send client updates
    ClientUpdateEvent(list, eventid);
    n = eaiSize(&dbids);
    for (i = 0; i < n; i++)
        ClientUpdateParticipant(ClientListFromDbid(dbids[i], 0), dbids[i], 0);
    eaiDestroy(&dbids);
}

void UpdateArenaTitle(void);

// propagate to dbserver, clients
void EventSave(U32 eventid)
{
    if (eventid >= (U32)eaSize(&g_events.events))
    {
        LOG_OLD_ERR("EventSave - received bad eventid %i", eventid);
        return;
    }
    if (g_events.events[eventid])
    {
        char* data = packageArenaEvent(g_events.events[eventid]);
        dbAsyncContainerUpdate(CONTAINER_ARENAEVENTS, eventid, CONTAINER_CMD_CREATE_MODIFY, data, 0);
        free(data);
        ClientUpdateEvent(ClientListFromEvent(g_events.events[eventid], 0), eventid);
    }
    else
    {
        dbAsyncContainerUpdate(CONTAINER_ARENAEVENTS, eventid, CONTAINER_CMD_DELETE, "", 0);
    }
    UpdateArenaTitle();
}

static int et_count = 0;
static int etIter(ArenaEvent* ae) { et_count++; return 1; }
int EventTotal(void)
{
    et_count = 0;
    EventIter(etIter);
    return et_count;
}

void EventIter(int(*func)(ArenaEvent*))
{
    int i, n = eaSize(&g_events.events);
    for (i = n-1; i >= 0; i--)
    {
        if (g_events.events[i])
        {
            if (!func(g_events.events[i]))
                return;
        }
    }
}

void EventAddParticipant(ArenaEvent* event, NetLink* link, U32 dbid, const char* archetype, int level, U32 sgid, int sgleader)
{
    ArenaParticipant* part = NULL;
    int i, n;

    // make sure we don't already have participant
    if (!event->participants)
        eaCreate(&event->participants);
    n = eaSize(&event->participants);
    for (i = 0; i < n; i++)
    {
        if (dbid && event->participants[i]->dbid == dbid)
        {
            ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "AlreadyJoined");
            return;
        }
    }
    if(event->weightClass && GetWeightClass(level) < event->weightClass)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "LevelProblem");
        return;
    }

    if (event->teamType==ARENA_SUPERGROUP && !sgid)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "NotInEventSupergroup");
        return;
    }
    if (event->teamType==ARENA_SUPERGROUP && event->creatorsg && event->othersg && sgid!=event->creatorsg && sgid!=event->othersg)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "NotInEventSupergroup");
        return;
    }

    if(event->leader_ready)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "EventStarting");
        return;
    }


    if(n >= event->maxplayers)
    {
        // find open slots
        for(i = 0; i < n; i++)
        {
            part = event->participants[i];
            if(!part->dbid)
            {
                // TODO: check if the slot fits this player
                if(part->desired_archetype && stricmp(part->desired_archetype, archetype) != 0 )    // archetype is an allocAddString so this works
                    continue;
                if(part->desired_team)
                    part->side = part->desired_team;
                break;
            }
        }
        if(i == n)
        {
            ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "NoMatchingSlots");
            return;    // no fitting slots available
        }
    }
    else
    {
        part = ArenaParticipantCreate();
        eaPush(&event->participants, part);
    }

    part->dbid = dbid;
    part->archetype = archetype;
    part->level = level;
    part->sgid = sgid;
    part->sgleader = sgleader;

    if(!event->entryfee)
        part->paid = 1;


    ArenaEventCountPlayers(event);

    // only set teams on supergroup event
    if( event->teamType == ARENA_SUPERGROUP )
        arenaBalanceTeams(event, true);

    EventUpdateErrors(event);
    ClientUpdateParticipant(ClientListFromDbid(dbid, 0), dbid, 0);
    EventSave(event->eventid);
}

void EventRemoveParticipant(ArenaEvent* event, U32 dbid)
{
    int i, n;

    if (!dbid)
    {
        return;
    }

    if (!event->participants)
    {
        LOG_OLD_ERR("EventRemoveParticipant - dont have any participants (%i,%i)", event->eventid, dbid);
        return;
    }

    // no removing of players from events that have started ever...
    if(event->phase >= ARENA_STARTROUND)
    {
        n = eaSize(&event->participants);
        for (i = 0; i < n; i++)
        {
            if (event->participants[i]->dbid == dbid)
            {
                event->participants[i]->dropped = 1;
                return;
            }
        }
    }

    // update creatorid as necessary
    if(event->creatorid == dbid)
    {
        for(i = 0; i < eaSize(&event->participants); i++)
        {
            if(event->participants[i]->dbid && event->participants[i]->dbid != event->creatorid)
            {
                event->creatorid = event->participants[i]->dbid;
                event->creatorsg = event->participants[i]->sgid;
                if (event->teamType==ARENA_SUPERGROUP && event->creatorsg==event->othersg)
                {
                    int j;
                    for (j = 0; j < eaSize(&event->participants); j++)
                    {
                        if (event->participants[j]->sgid!=event->creatorsg)
                        {
                            event->othersg=event->participants[j]->sgid;
                            break;
                        }
                    }
                    if (event->creatorsg==event->othersg)
                        event->othersg = 0;
                }
                break;
            }
        }
    }

    if( event->leader_ready ) // only happens in player created events
    {
        event->start_time = 0;
        event->leader_ready = false;
    }

    // remove record
    n = eaSize(&event->participants);
    for (i = 0; i < n; i++)
    {
        if (event->participants[i]->dbid == dbid)
        {
            if( event->othersg == event->participants[i]->sgid && event->teamType == ARENA_SUPERGROUP )
            {
                event->participants[i]->sgid = 0;
                arenaBalanceTeams( event, 1 );
            }

            event->participants[i]->sgid = 0;
             event->participants[i]->dbid = 0;
            ArenaEventCountPlayers(event);
            if (event->numplayers == 0 && INSTANT_EVENT(event) && !SERVER_RUN(event))
            {
                ClientUpdateParticipant(ClientListFromDbid(dbid, 0), dbid, 0);
                EventDelete(event->eventid);
            }
            else
            {
                ClientUpdateParticipant(ClientListFromDbid(dbid, 0), dbid, 0);
                EventUpdateErrors(event);
                EventSave(event->eventid);
            }
            return;
        }
    }
    LOG_OLD_ERR("EventRemoveParticipant - dont have participant (%i,%i)", event->eventid, dbid);
}

ArenaParticipant* EventGetParticipant(ArenaEvent* event, U32 dbid)
{
    int i, n;

    if (!event->participants)
        return NULL;
    n = eaSize(&event->participants);
    for (i = 0; i < n; i++)
    {
        if (event->participants[i]->dbid == dbid)
            return event->participants[i];
    }
    return NULL;
}


// *********************************************************************************
//  Client requests
// *********************************************************************************

void handleRequestKiosk(Packet* pak,NetLink* link)
{
    U32 db_id=pktGetBitsPack(pak,PKT_BITS_TO_REP_DB_ID);
    const char * archetype=allocAddString(pktGetString(pak));
    int level=pktGetBitsPack(pak,1);
    int radioButtonFilter=pktGetBitsPack(pak,1);
    int tabFilter=pktGetBitsPack(pak,1);
    int i;

    Packet* ret = pktCreateEx(link, ARENASERVER_KIOSK);
    pktSendBitsPack(ret,PKT_BITS_TO_REP_DB_ID,db_id);
    if (radioButtonFilter==ARENA_FILTER_ELIGABLE)    //send a bit for eligible, the mapserver
        pktSendBits(ret,1,1);                        //needs to know if it should filter out
    else                                            //events based on supergroups
        pktSendBits(ret,1,0);
    for (i=0;i<eaSize(&g_events.events);i++)
    {
        EventKioskFilter(g_events.events[i], db_id, archetype, level, radioButtonFilter, tabFilter);
    }
    ArenaKioskSend(ret, &g_events);
    pktSend(&ret, link);
}

static int scheduled_count = 0;
ArenaEvent * active_event = NULL;

static int eventCountParticipating( ArenaEvent * event )
{
    ArenaParticipant* part = ArenaParticipantFind(event, g_request_dbid);
    
    if (part && !part->dropped && !COMPLETED_EVENT(event))
    {
        if( event->phase == ARENA_SCHEDULED && !ASAP_EVENT(event) )
            scheduled_count++;
        else
            active_event = event;
    }

    return 1;
}

void handleJoinEvent(Packet* pak, NetLink* link)
{
    ArenaEvent* event;
    U32 level, sgid;
    int sgleader,ignore_active, influence;
    const char *archetype;

    ArenaCommandStart(pak);
    strncpyt( g_request_name, pktGetString(pak), 32 );
    influence = pktGetBitsPack(pak, 1);
    ignore_active = pktGetBits(pak,1);
    g_request_eventid = pktGetBitsPack(pak, 1);
    g_request_uniqueid = pktGetBitsPack(pak, 1);
    archetype = allocAddString(pktGetString(pak));
    level = pktGetBitsPack(pak, 1);
    sgid = pktGetBitsPack(pak, 1);
    sgleader = pktGetBits(pak, 1);

    // find event
    event = EventGetUnique(g_request_eventid, g_request_uniqueid);
    if (!event)
    {
        ArenaCommandResult(link, ARENA_ERR_BADEVENT, 0, "BadEvent");
        return;
    }

    // make sure event isn't full
    if (event->numplayers >= event->maxplayers)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "EventFull");
        return;
    }

    if( (!event->listed || // make sure they are allowed to join
        event->inviteonly) && g_request_user != CLIENTINP_ARENA_REQ_JOIN_INVITE )
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "EventInviteOnly");
        return;
    }

    if( event->teamType == ARENA_SUPERGROUP )
    {
        if( sgid != event->creatorsg && event->othersg && sgid != event->othersg )
        {
            ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "NotInEventSupergroup");
            return;
        }
    }

    if(event->entryfee > influence)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "NotEnoughInfluence");
        return;
    }

    scheduled_count = 0;
    active_event = NULL;
    EventIter( eventCountParticipating );

    if( active_event && ACTIVE_EVENT(event) )
    {
        if( ignore_active )
        {
            if(active_event != event)
                EventRemoveParticipant(active_event, g_request_dbid);
            else
            {
                ArenaCommandResult(link, ARENA_SUCCESS, 0, NULL);
                return;
            }
        }
        else
        {
            ArenaCommandResult(link, ARENA_ERR_POPUP, 0, "ArenaActiveWarning");
            return;
        }
    }
    else if( event->scheduled && scheduled_count >= ARNEA_MAX_SCHEDULED )
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "MaxParticipatingEvents");
        return;
    }

    ArenaCommandResult(link, ARENA_SUCCESS, 0, NULL);
    event->lastupdate_dbid = g_request_dbid;
    event->lastupdate_cmd = g_request_user;
    strcpy( event->lastupdate_name, g_request_name );
    EventAddParticipant(event, link, g_request_dbid, archetype, level, sgid, sgleader);
}

void handleDropEvent(Packet* pak, NetLink* link)
{
    ArenaParticipant* part;
    ArenaEvent* event;
    U32 eventid, uniqueid;

    ArenaCommandStart(pak);
    strncpyt( g_request_name, pktGetString(pak), 32 );
    eventid = pktGetBitsPack(pak, 1);
    uniqueid = pktGetBitsPack(pak, 1);

    event = EventGetUnique(eventid, uniqueid);
    if (!event)
    {
        ArenaCommandResult(link, ARENA_ERR_BADEVENT, 0, "BadEvent");
        return;
    }
    part = ArenaParticipantFind(event, g_request_dbid);
    if (!part)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "DontBelongToEvent");
        return;
    }

    // otherwise, I know it will work, whether tourney has started or not
    ArenaCommandResult(link, ARENA_SUCCESS, 0, NULL);
    event->lastupdate_dbid = g_request_dbid;
    event->lastupdate_cmd = g_request_user;

    strcpy( event->lastupdate_name, g_request_name );
    if (event->phase < ARENA_STARTROUND) 
    {
        EventRemoveParticipant(event, g_request_dbid);
    }
    else
    {
        part->dropped = 1;
        EventSave(eventid);
    }
}

#define DEFAULT_ARENA_MAXPLAYER    4

void handleCreateEvent(Packet* pak, NetLink* link)
{
    ArenaEvent* event;
    char eventname[ARENA_DESCRIPTION_LEN];
    const char *archetype;
    char playername[MAX_NAME_LEN];
    int i,level, sgleader;
    U32 sgid;

    ArenaCommandStart(pak);
    strncpyt(eventname, pktGetString(pak), ARENA_DESCRIPTION_LEN);
    strncpyt(playername, pktGetString(pak), MAX_NAME_LEN);
    archetype = allocAddString(pktGetString(pak));
    level = pktGetBitsPack(pak, 1);
    sgid = pktGetBitsPack(pak, 1);
    sgleader = pktGetBits(pak, 1);

    for (i=0;i<eaSize(&g_events.events);i++) {
        int j;
        if (!g_events.events[i]) continue;
        if (!ACTIVE_EVENT(g_events.events[i])) continue;
        for (j=0;j<eaSize(&(g_events.events[i]->participants));j++) {
            if (g_events.events[i]->participants[j] &&
                    g_events.events[i]->participants[j]->dbid==g_request_dbid &&
                    !g_events.events[i]->participants[j]->dropped) {
                ArenaCommandResult(link, ARENA_ERR_BADEVENT, 0, "EventAddFailed");
                return;
            }
        }
    }


    event = EventAdd("");
    if (!event)
    {
        ArenaCommandResult(link, ARENA_ERR_BADEVENT, 0, "EventAddFailed");
        return;
    }

    ArenaCommandResult(link, ARENA_SUCCESS, 0, NULL);
    strcpy( event->creatorname, playername );
      event->creatorid = g_request_dbid;
    event->creatorsg = sgid;
    event->custom = true;
    event->maxplayers = DEFAULT_ARENA_MAXPLAYER;
    event->lastupdate_dbid = g_request_dbid;
    event->lastupdate_cmd = g_request_user;
    EventChangePhase(event, ARENA_NEGOTIATING);
    EventAddParticipant(event, link, g_request_dbid, archetype, level, sgid, sgleader);
    for( i = 1; i <= DEFAULT_ARENA_MAXPLAYER; i++ )
        EventAddParticipant(event, link, 0, 0, 0, 0, 0);
}

void handleDestroyEvent(Packet* pak, NetLink* link)
{
    U32 eventid, uniqueid;
    ArenaEvent* event;

    ArenaCommandStart(pak);
    eventid = pktGetBitsPack(pak, 1);
    uniqueid = pktGetBitsPack(pak, 1);

    event = EventGetUnique(eventid, uniqueid);
    if (!event)
    {
        ArenaCommandResult(link, ARENA_ERR_BADEVENT, 0, "BadEvent");
        return;
    }
    
    // need accesslevel for this command so nothing to verify

    ArenaCommandResult(link, ARENA_SUCCESS, 0, NULL);
    event->lastupdate_dbid = g_request_dbid;
    event->lastupdate_cmd = g_request_user;
    strcpy( event->lastupdate_name, g_request_name );
    EventLog(event, "eventdestroyed");
    EventDelete(eventid);
}

// this link just wants an immediate answer on its event
void handleRequestEvent(Packet* pak, NetLink* link)
{
    U32 eventid;
    ArenaEvent* event;

    eventid = pktGetBitsPack(pak, 1);
    event = EventGet(eventid);
    if (event)
    {
        ArenaCommandResult(link, ARENA_SUCCESS, 0, NULL);
        ClientUpdateEvent(ClientListFromClient((ArenaClientLink*)link->userData, 0), event->eventid);
    }
    else
    {
        ArenaCommandResult(link, ARENA_ERR_BADEVENT, 0, "BadEvent");
    }
}

void handleSetSide(Packet* pak, NetLink* link)
{
    U32 eventid, uniqueid;
    int side;
    ArenaParticipant* part;
    ArenaEvent* event;
    
    ArenaCommandStart(pak);
    eventid = pktGetBitsPack(pak, 1);
    uniqueid = pktGetBitsPack(pak, 1);
    side = pktGetBitsPack(pak, 1);

    if(side <= 0 || side > ARENA_MAX_SIDES)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "InvalidSide");
        return;
    }

    event = EventGetUnique(eventid, uniqueid);
    if (!event)
    {
        ArenaCommandResult(link, ARENA_ERR_BADEVENT, 0, "BadEvent");
        return;
    }
    part = EventGetParticipant(event, g_request_dbid);
    if (!part)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "NotParticipant");
        return;
    }

    // todo - check based on phase of event, etc.
    if(event->phase >= ARENA_STARTROUND)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "WrongPhase");
        return;
    }
    part->side = side;
    ArenaCommandResult(link, ARENA_SUCCESS, 0, NULL);
    event->lastupdate_dbid = g_request_dbid;
    event->lastupdate_cmd = g_request_user;
    EventSave(eventid);
}

void handleSetMap(Packet* pak, NetLink* link)
{
    U32 eventid, uniqueid;
    char* newmap;
    ArenaParticipant* part;
    ArenaEvent* event;

    ArenaCommandStart(pak);
    eventid = pktGetBitsPack(pak, 1);
    uniqueid = pktGetBitsPack(pak, 1);
    newmap = pktGetString(pak);

    event = EventGetUnique(eventid, uniqueid);
    if (!event)
    {
        ArenaCommandResult(link, ARENA_ERR_BADEVENT, 0, "BadEvent");
        return;
    }
    part = EventGetParticipant(event, g_request_dbid);
    if (!part)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "NotParticipant");
        return;
    }

    if(event->phase >= ARENA_STARTROUND)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "WrongPhase");
        return;
    }

    // todo - check based on phase of event, etc.
    strncpyt(event->mapname, newmap, ARRAY_SIZE(event->mapname));
    ArenaCommandResult(link, ARENA_SUCCESS, 0, NULL);
    event->lastupdate_dbid = g_request_dbid;
    event->lastupdate_cmd = g_request_user;
    EventSave(eventid);
}

void handleRequestPlayerUpdate(Packet* pak, NetLink* link)
{
    Packet * ret;
    ArenaPlayer * ap;
    int dbidSource;
    int dbidTarget;
    int requester;

    dbidSource=pktGetBitsPack(pak,PKT_BITS_TO_REP_DB_ID);
    dbidTarget=pktGetBitsPack(pak,PKT_BITS_TO_REP_DB_ID);
    requester =pktGetBitsPack(pak,1);

    if(dbidSource <= 0 || dbidTarget <= 0)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "InvalidDbId");
        return;
    }

    ap = PlayerGetAdd(dbidTarget,0);
    if (!ap) 
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "InvalidDbId");
        return;
    }

    ret = pktCreateEx(link, ARENASERVER_PLAYERUPDATE);
    pktSendBitsPack(ret, PKT_BITS_TO_REP_DB_ID, dbidSource);
    pktSendBitsPack(ret, 1, requester);
    ArenaPlayerSend(ret, ap);
    pktSend(&ret,link);
}

int EventVerifyFailed( ArenaEvent * event, char *problem )
{
    int x;

    if( !event->cannot_start ) // only add to strings while cannot_start is set
    {                           // it will be reset when needed.
        for(x = 0; x < ARENA_NUM_PROBLEMS; x++)
        {
            if(!event->eventproblems[x][0])
            {
                strncpy(event->eventproblems[x], (problem), ARENA_PROBLEM_STR_SIZE);
                break;
            }
        }
    }
    event->sanctioned = 0;
    return true;
}


static int EventSidesOk(ArenaEvent* event)
{
    int i, n, count = 0;

    // supergroup events are special
    if (event->teamType == ARENA_SUPERGROUP) // should be supergroup rumble
    {
        int creatorsg = 0;
        int othersg = 0;

        if (!event->creatorsg) 
            return !EventVerifyFailed(event,"YourNotInSG");

        if (!event->othersg) 
            return !EventVerifyFailed(event,"NoOtherSg");

        for (i = 0; i < eaSize(&event->participants); i++)
        {
            if (event->participants[i]->sgid == event->creatorsg)
                creatorsg++;
            else if (event->participants[i]->sgid == event->othersg)
                othersg++;
            else if( event->participants[i]->dbid )
                return !EventVerifyFailed(event,"NonSgPlayer");
        }

        if( creatorsg && othersg )
            return true;
        else
            return !EventVerifyFailed(event,"NoOtherSg");
    }
    // for server events, sides are going to be prepared when event starts
    else if (SERVER_RUN(event))
    {
        if (event->tournamentType == ARENA_TOURNAMENT_SWISSDRAW)
        {
            if(event->numplayers >= 8) // min swiss draw size
                return true;
            else
                return !EventVerifyFailed(event,"NotEnoughPlayer");
        }
        else
        {
            if( event->numplayers >= 2 )
                return true;
            else
                return !EventVerifyFailed(event,"NotEnoughPlayer");
        }
    }
    // for player events, just see if we have two different sides enumerated
    // make sure team size isn't too large
    else 
    {
        int sidecount[ARENA_MAX_SIDES+1];
        int numplayers = 0;

        memset(sidecount, 0, sizeof(sidecount));
        n = eaSize(&event->participants);

        for (i = 0; i < n; i++)
        {
            if (event->participants[i]->dbid)
            {
                numplayers++;

                if (event->participants[i]->side == 0) // ask for server to assign?
                    count++;
                else if (!sidecount[event->participants[i]->side])
                    count++;

                sidecount[event->participants[i]->side]++;
            }
        }

        // teamsize check
        if( event->teamType != ARENA_SUPERGROUP )
        {
            for( i = 1; i < ARENA_MAX_SIDES+1; i++ )
            {
                if( sidecount[i] > 8 )
                    return !EventVerifyFailed(event,"TeamTooLarge");
            }
        }

        if (event->tournamentType == ARENA_TOURNAMENT_SWISSDRAW)
        {
            if (count < 8)
            {
                return !EventVerifyFailed(event,"NotEnoughPlayer");
            }
        }
        else
        {
            if(numplayers < 2)
                return !EventVerifyFailed(event,"NotEnoughPlayer");

            if(count < 2)
                return !EventVerifyFailed(event,"NotEnoughSides");
        }

        // check mapsize if one was selected
        if (event->mapid >= 0 && event->mapid < eaSize(&g_arenamaplist.maps))
        {
            if (numplayers > g_arenamaplist.maps[event->mapid]->maxplayers)
                return !EventVerifyFailed(event,"TooManyPlayerForMap");
            if (numplayers < g_arenamaplist.maps[event->mapid]->minplayers)
                return !EventVerifyFailed(event,"NotEnoughPlayerForMap");
        }

        return true;

    }
}

int EventVerifyIfSanctioned(ArenaEvent* event)
{
    static int havesides[ARENA_MAX_SIDES+1];
    int i, j, k, curside, numsides = 0, numplayers = 0, failed = false;
    ArenaRestriction *rest = &restrictions[event->teamStyle][event->teamType];
    int playersperside = 0;

    // sides ok is a more basic check, run it first
    if( !EventSidesOk(event) ) 
        return true;

    if(!event->weightClass && rest->restrictLevel && !event->use_gladiators)
    {
        failed = EventVerifyFailed(event,"LevelProblem");
    }

    if(event->no_travel || event->no_stealth || 
        event->no_end || event->no_pool || event->no_travel_suppression ||
        event->no_diminishing_returns || event->no_heal_decay ||
        (event->inspiration_setting != ARENA_NORMAL_INSPIRATIONS) || event->enable_temp_powers)
    {
        failed = EventVerifyFailed(event,"PowerError");
    }

    if (event->victoryType != rest->victoryType)
    {
        failed = EventVerifyFailed(event, "VictoryTypeProblem");
    }

    if (event->victoryValue != rest->victoryValue)
    {
        failed = EventVerifyFailed(event, "VictoryValueProblem");
    }

    if( rest->restrictLevel )
    {
        for(i = 0; i < event->numplayers; i++ )
        {
            if(GetWeightClass(event->participants[i]->level) < event->weightClass)
            {
                failed = EventVerifyFailed(event,"LevelProblem");
                break;
            }
        }
    }

    ZeroMemory(havesides, (ARENA_MAX_SIDES+1) * sizeof(int));

    for(i = eaSize(&event->participants)-1; i >= 0; i--)
    {
        if(event->participants[i]->dbid)
        {
            numplayers++;
            if(!havesides[event->participants[i]->side]++)
                numsides++;
        }
    }

     if(rest->minPlayers && rest->minPlayers > numplayers)
    {
        return EventVerifyFailed(event,"LowPlayerErr");
    }

    if(rest->maxPlayers && rest->maxPlayers < numplayers)
    {
        return EventVerifyFailed(event,"HiPlayerErr");
    }

    if((rest->minSides && rest->minSides > numsides) || (rest->maxSides && rest->maxSides < numsides))
    {
        return EventVerifyFailed(event,"SidesProblem");
    }
    else
    {
        for(i = 0; i < ARENA_MAX_SIDES+1; i++)
        {
            if(havesides[i])
            {
                if(playersperside)
                {
                    if(playersperside != havesides[i])
                        failed = EventVerifyFailed(event,"UnevenTeams");
                }
                else
                    playersperside = havesides[i];
            }
        }
    }

    if( rest->maxSides && (rest->minPlayersPerSide && rest->minPlayersPerSide > playersperside) ||
        (rest->maxPlayersPerSide && rest->maxPlayersPerSide < playersperside))
    {
        failed = EventVerifyFailed(event,"PPSError");
    }

     ZeroMemory(havesides, (ARENA_MAX_SIDES+1) * sizeof(int));
 
    if (event->teamStyle == ARENA_STAR || 
        event->teamStyle == ARENA_STAR_VILLAIN ||
        event->teamStyle == ARENA_STAR_VERSUS ||
        event->teamStyle == ARENA_7STAR || 
        event->teamStyle == ARENA_7STAR_VILLAIN ||
        event->teamStyle == ARENA_7STAR_VERSUS)
    {
        char* classes[2][7];
        
        if (event->teamStyle == ARENA_STAR ||
            event->teamStyle == ARENA_7STAR)
        {
            classes[0][0] = "Class_Blaster";
            classes[0][1] = "Class_Controller";
            classes[0][2] = "Class_Defender";
            classes[0][3] = "Class_Scrapper";
            classes[0][4] = "Class_Tanker";
            classes[0][5] = "Class_Warshade";
            classes[0][6] = "Class_Peacebringer";
            classes[1][0] = "Class_Blaster";
            classes[1][1] = "Class_Controller";
            classes[1][2] = "Class_Defender";
            classes[1][3] = "Class_Scrapper";
            classes[1][4] = "Class_Tanker";
            classes[1][5] = "Class_Warshade";
            classes[1][6] = "Class_Peacebringer";
        }
        else if (event->teamStyle == ARENA_STAR_VILLAIN ||
                 event->teamStyle == ARENA_7STAR_VILLAIN)
        {
            classes[0][0] = "Class_Brute";
            classes[0][1] = "Class_Corruptor";
            classes[0][2] = "Class_Dominator";
            classes[0][3] = "Class_MasterMind";
            classes[0][4] = "Class_Stalker";
            classes[0][5] = "Class_Arachnos_Widow";
            classes[0][6] = "Class_Arachnos_Soldier";
            classes[1][0] = "Class_Brute";
            classes[1][1] = "Class_Corruptor";
            classes[1][2] = "Class_Dominator";
            classes[1][3] = "Class_MasterMind";
            classes[1][4] = "Class_Stalker";
            classes[1][5] = "Class_Arachnos_Widow";
            classes[1][6] = "Class_Arachnos_Soldier";
        }
        else if (event->teamStyle == ARENA_STAR_VERSUS ||
                 event->teamStyle == ARENA_7STAR_VERSUS)
        {
            // Villains are red team
            classes[0][0] = "Class_Brute";
            classes[0][1] = "Class_Corruptor";
            classes[0][2] = "Class_Dominator";
            classes[0][3] = "Class_MasterMind";
            classes[0][4] = "Class_Stalker";
            classes[0][5] = "Class_Arachnos_Widow";
            classes[0][6] = "Class_Arachnos_Soldier";
            // Heroes are blue team
            classes[1][0] = "Class_Blaster";
            classes[1][1] = "Class_Controller";
            classes[1][2] = "Class_Defender";
            classes[1][3] = "Class_Scrapper";
            classes[1][4] = "Class_Tanker";
            classes[1][5] = "Class_Warshade";
            classes[1][6] = "Class_Peacebringer";
        }

        for(i = 0; i < eaSize(&event->participants); i++)
        {
            if(!havesides[event->participants[i]->side])
            {
                curside = event->participants[i]->side;
                for(j = i; j < eaSize(&event->participants); j++)
                {
                    if(event->participants[j]->side == curside)
                    {
                        int num_classes;
                        
                        if (event->teamStyle == ARENA_STAR ||
                            event->teamStyle == ARENA_STAR_VILLAIN ||
                            event->teamStyle == ARENA_STAR_VERSUS)
                        {
                            num_classes = 5;
                        }
                        else
                        {
                            num_classes = 7;
                        }

                        for(k = 0; k < num_classes; k++)
                        {
                            if(classes[curside % 2][k] == event->participants[j]->archetype)
                            {
                                if(havesides[curside] & (1 << k))
                                {
                                    failed = EventVerifyFailed(event,"WrongArchetypes");
                                }
                                else
                                {
                                    havesides[curside] |= (1 << k);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return failed;
}



static void arenaMakeRestrictions(ArenaEvent *event )
{
    ArenaRestriction *rest = &restrictions[event->teamStyle][event->teamType];

    // Force numsides/maxplayers in bounds for event type
    if( rest->minPlayers && rest->minPlayers > event->maxplayers )
        event->maxplayers = rest->minPlayers;

    if(    rest->maxPlayers && rest->maxPlayers < event->maxplayers )
        event->maxplayers = rest->maxPlayers;

    if( rest->minPlayersPerSide && rest->minPlayersPerSide*event->numsides > event->maxplayers )
        event->maxplayers = rest->minPlayersPerSide*event->numsides;

    if( rest->maxPlayersPerSide && rest->maxPlayersPerSide*event->numsides < event->maxplayers )
        event->maxplayers = rest->maxPlayersPerSide*event->numsides;

    if( event->teamType != ARENA_SINGLE && rest->minSides && rest->minSides > event->numsides )
        event->numsides = rest->minSides;

    if( event->teamType != ARENA_SINGLE && rest->maxSides && rest->maxSides < event->numsides )
        event->numsides = rest->maxSides;
}

#define NUM_ARCH 14

static void arenaBalanceTeams(ArenaEvent * event, bool supergroup )
{
    int i;
    int num_players = eaSize(&event->participants);

     if (event->teamStyle == ARENA_STAR ||
        event->teamStyle == ARENA_STAR_VILLAIN ||
        event->teamStyle == ARENA_STAR_VERSUS ||
        event->teamStyle == ARENA_7STAR ||
        event->teamStyle == ARENA_7STAR_VILLAIN ||
        event->teamStyle == ARENA_7STAR_VERSUS) // star events are annoyingly precise
    {
        int j, count[NUM_ARCH] = {0};

        if (event->teamStyle == ARENA_STAR_VERSUS ||
            event->teamStyle == ARENA_7STAR_VERSUS)
        {
            // Set up the base "count" values, 
            // so villains start in the first (red) team
            // and heroes start in the second (blue).
            for (j = NUM_ARCH / 2; j < NUM_ARCH; j++)
            {
                count[j] = -1;
            }
        }

        // first evenly distribute existing players 
        for( i = 0; i < event->numplayers; i++ )
        {
            for( j = 0; j < NUM_ARCH; j++ )
            {
                if( event->participants[i]->archetype &&
                    stricmp( arenaArchetype[j], event->participants[i]->archetype) == 0 )
                {
                    if (event->teamStyle == ARENA_STAR_VERSUS ||
                        event->teamStyle == ARENA_7STAR_VERSUS)
                    {
                        // Jump over the team for the other faction.
                        count[j] += 2;
                    }
                    else
                    {
                        count[j]++;
                    }

                    event->participants[i]->side = count[j];
                    if( event->participants[i]->side > event->numsides )
                        event->participants[i]->side = 1;

                    break;
                }
            }
        }

        // now distribute empty slot remainders
         while( i < num_players )
        {
            int min_arch = 0;
            int num_classes_to_skip = 0;

            if (event->teamStyle == ARENA_STAR_VILLAIN ||
                event->teamStyle == ARENA_7STAR_VILLAIN)
            {
                min_arch = 7;
            }

            if (event->teamStyle == ARENA_STAR ||
                event->teamStyle == ARENA_STAR_VILLAIN || 
                event->teamStyle == ARENA_STAR_VERSUS)
            {
                num_classes_to_skip = 2;
            }

            // find the lowest count
            if (event->teamStyle == ARENA_STAR ||
                event->teamStyle == ARENA_STAR_VERSUS ||
                event->teamStyle == ARENA_7STAR ||
                event->teamStyle == ARENA_7STAR_VERSUS)
            {
                for( j = 0; j < NUM_ARCH / 2 - num_classes_to_skip; j++ ) 
                {
                    if( count[j] < count[min_arch] )
                        min_arch = j;
                }
            }

            if (event->teamStyle == ARENA_STAR_VILLAIN ||
                event->teamStyle == ARENA_STAR_VERSUS ||
                event->teamStyle == ARENA_7STAR_VILLAIN ||
                event->teamStyle == ARENA_7STAR_VERSUS)
            {
                for( j = NUM_ARCH / 2; j < NUM_ARCH - num_classes_to_skip; j++ ) 
                {
                    if( count[j] < count[min_arch] )
                        min_arch = j;
                }
            }
            
            if (event->teamStyle == ARENA_STAR_VERSUS ||
                event->teamStyle == ARENA_7STAR_VERSUS)
            {
                // Jump over the team for the other faction.
                count[min_arch] += 2;
            }
            else
            {
                count[min_arch]++;
            }

            event->participants[i]->desired_archetype = arenaArchetype[min_arch];
            event->participants[i]->side = event->participants[i]->desired_team = count[min_arch];
            if( event->participants[i]->side > event->numsides )
                event->participants[i]->side = 1;

            i++;
        }

    }
    else if (supergroup)
    {
        // for invitationals, everyone on on different side
        if (event->teamType == ARENA_SINGLE)
        {
            for (i = 0; i < num_players; i++)
                event->participants[i]->side = event->participants[i]->desired_team = 0;
        }
        // for rumbles, team is determined by supergroup
        else
        {
            int sgcount[2] = {0};
            event->othersg = 0;

            // pick the other supergroup, the first person in list that is not int the creator sg
            for( i = 0; i < num_players; i++ )
            {
                if( event->participants[i]->sgid && event->participants[i]->sgid != event->creatorsg )
                {
                    event->othersg = event->participants[i]->sgid;
                    break;
                }
            }
            
            // determine everyone's side based on their supergroup
             for (i = 0; i < num_players; i++)
            {
                if( event->creatorsg && event->participants[i]->sgid == event->creatorsg)
                {    
                    event->participants[i]->side = event->participants[i]->desired_team = 1;
                    sgcount[0]++;
                }
                else if( event->othersg && event->participants[i]->sgid == event->othersg)
                {
                    event->participants[i]->side = event->participants[i]->desired_team = 2;
                    sgcount[1]++;
                }
                else
                {
                    if( sgcount[0] < sgcount[1] )
                    {
                        event->participants[i]->side = event->participants[i]->desired_team = 1;
                        sgcount[0]++;
                    }
                    else
                    {
                        event->participants[i]->side = event->participants[i]->desired_team = 2;
                        sgcount[1]++;
                    }
                }
            }
        }
    }
    else
    {
        if (event->teamType != ARENA_SINGLE)
        {
            for( i = 0; i < num_players; i++ )        // distribute teams evenly across the players
                event->participants[i]->side = event->participants[i]->desired_team = (i%event->numsides)+1;
        }
        else
        {
            for( i = 0; i < num_players; i++ )        // Mark everyone as solo
                event->participants[i]->side = event->participants[i]->desired_team = 0;
        }
    }
}

static void EventUpdateErrors( ArenaEvent *event)
{
    int i;
    //clear old errors
    event->cannot_start = 0;
    for(i = 0; i < ARENA_NUM_PROBLEMS; i++)    \
        event->eventproblems[i][0] = '\0';

    if(event->verify_sanctioned)
    {
        event->sanctioned = !EventVerifyIfSanctioned(event);
        event->cannot_start = !event->sanctioned;
    }
    else if(event->sanctioned)
        event->sanctioned = !EventVerifyIfSanctioned(event);
    else
        event->cannot_start = !EventSidesOk(event);
}

void handleCreatorUpdate(Packet* pak, NetLink* link)
{
    ArenaEvent tmp_event = {0};
    ArenaEvent *event;
    int any_change = 0, balance_teams = 0;

    ArenaCommandStart(pak);
    ArenaCreatorUpdateRecieve( &tmp_event, pak );

    event = EventGetUnique( tmp_event.eventid, tmp_event.uniqueid );

    // do verification here
    if(!event)
    {
        ArenaCommandResult(link, ARENA_ERR_BADEVENT, 0, "BadEvent");
        return;
    }

    if(event->phase >= ARENA_STARTROUND)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "WrongPhase");
        return;
    }
    if(g_request_dbid != event->creatorid)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "NotCreator");
        return;
    }

     if( tmp_event.leader_ready && !event->leader_ready )
    {
        event->start_time = event->round_start = dbSecondsSince2000() + START_EVENT_DELAY;
    }
    if( !tmp_event.leader_ready && event->leader_ready )
    {
        event->start_time = 0;
    }

    if (tmp_event.teamType == ARENA_SINGLE)
    {
        tmp_event.teamStyle = ARENA_FREEFORM;
        tmp_event.numsides = tmp_event.numplayers;
    }

     any_change |= balance_teams |= ( event->teamStyle != tmp_event.teamStyle);
    any_change |= balance_teams |= ( event->teamType != tmp_event.teamType);
    any_change |= ( event->weightClass != tmp_event.weightClass);
    any_change |= ( event->victoryType != tmp_event.victoryType);
    any_change |= ( event->victoryValue != tmp_event.victoryValue);
    any_change |= ( event->tournamentType != tmp_event.tournamentType);
    any_change |= ( event->custom != tmp_event.custom);
    //any_change |= ( event->inviteonly != tmp_event.inviteonly);
    any_change |= ( event->tactical_start != tmp_event.tactical_start);
    any_change |= ( event->no_end != tmp_event.no_end);
    any_change |= ( event->no_pool != tmp_event.no_pool);
    any_change |= ( event->no_travel != tmp_event.no_travel);
    any_change |= ( event->no_stealth != tmp_event.no_stealth);
    any_change |= ( event->no_travel_suppression != tmp_event.no_travel_suppression);
    any_change |= ( event->no_diminishing_returns != tmp_event.no_diminishing_returns);
    any_change |= ( event->no_heal_decay != tmp_event.no_heal_decay);
    any_change |= ( event->inspiration_setting != tmp_event.inspiration_setting);
    any_change |= ( event->enable_temp_powers != tmp_event.enable_temp_powers);
    any_change |= ( event->use_gladiators != tmp_event.use_gladiators);
    any_change |= balance_teams |= (event->teamType != ARENA_SINGLE && event->numsides != tmp_event.numsides);
    any_change |= balance_teams |= (event->maxplayers != tmp_event.maxplayers);
    any_change |= ( event->numplayers != tmp_event.numplayers);
    any_change |= ( event->verify_sanctioned != tmp_event.verify_sanctioned);
    any_change |= ( event->no_observe != tmp_event.no_observe);
    any_change |= ( event->mapid != tmp_event.mapid);

    if( any_change )
    {
        event->start_time = 0;
        tmp_event.leader_ready = 0;
    }

    event->listed                    = tmp_event.listed;
    event->teamStyle                = tmp_event.teamStyle;
    event->teamType                    = tmp_event.teamType;
    event->weightClass                = tmp_event.weightClass;
    event->victoryType                = tmp_event.victoryType;
    event->victoryValue                = tmp_event.victoryValue;
    event->tournamentType            = tmp_event.tournamentType;
    event->custom                    = tmp_event.custom;
    event->inviteonly                = tmp_event.inviteonly;
    event->tactical_start            = tmp_event.tactical_start;
    event->no_end                    = tmp_event.no_end;
    event->no_pool                    = tmp_event.no_pool;
    event->no_travel                = tmp_event.no_travel;
    event->no_stealth                = tmp_event.no_stealth;
    event->no_travel_suppression    = tmp_event.no_travel_suppression;
    event->no_diminishing_returns    = tmp_event.no_diminishing_returns;
    event->no_heal_decay            = tmp_event.no_heal_decay;
    event->inspiration_setting        = tmp_event.inspiration_setting;
    event->enable_temp_powers        = tmp_event.enable_temp_powers;
    event->use_gladiators            = tmp_event.use_gladiators;
    event->numsides                    = tmp_event.numsides;
    event->maxplayers                = tmp_event.maxplayers;
    event->numplayers                = tmp_event.numplayers;
    event->no_observe                = tmp_event.no_observe;
    event->verify_sanctioned        = tmp_event.verify_sanctioned;
    event->leader_ready                = tmp_event.leader_ready;
    event->mapid                    = tmp_event.mapid;

    arenaMakeRestrictions(event);

    // Make sure there is a participant for every possible player in the event
    while( eaSize(&event->participants) < event->maxplayers )
        eaPush( &event->participants, ArenaParticipantCreate());

    // trim unused participants
    while( eaSize(&event->participants) > event->maxplayers )
        ArenaParticipantDestroy( eaRemove(&event->participants, eaSize(&event->participants)-1) );

    // rebalance teams if the maxplayers changed or number of teams changed
    if(balance_teams)
        arenaBalanceTeams( event, event->teamType == ARENA_SUPERGROUP );

    EventUpdateErrors(event);

    ArenaCommandResult(link, ARENA_SUCCESS, 0, NULL);
    event->lastupdate_dbid = g_request_dbid;
    event->lastupdate_cmd = g_request_user;
    EventSave(event->eventid);
}

// returns whether anything was changed except ready
static int updatePlayer( ArenaEvent * event, ArenaParticipant * participant )
{
    int changed = 0;
    if( participant->dbid )
    {
        int i;
        for( i = eaSize(&event->participants)-1; i>=0; i-- )
        {
            if( event->participants[i]->dbid == participant->dbid )
            {
                if( !SERVER_RUN(event) && event->teamType != ARENA_SUPERGROUP && event->participants[i]->side != participant->side )
                {
                    if(participant->side > 0 && participant->side < ARENA_MAX_SIDES)
                    {
                        event->participants[i]->side = participant->side;
                        changed = 1;
                    }
                }
            }
        }
    }
    else if ( !SERVER_RUN(event) )
    {
        if( participant->id >= eaSize(&event->participants) )
        {
            ArenaParticipant * ap = ArenaParticipantCreate();
            ap->desired_archetype = participant->desired_archetype;

            if( event->teamType != ARENA_SUPERGROUP )
                ap->desired_team = participant->desired_team;
            eaPush( &event->participants, ap );
        }
        else
        {
            if( event->teamType != ARENA_SUPERGROUP )
                event->participants[participant->id]->desired_team = participant->desired_team;
            event->participants[participant->id]->desired_archetype    = participant->desired_archetype;
        }
        changed = 1;
    }
    return changed;
}

void handlePlayerUpdate(Packet* pak, NetLink* link)
{
    ArenaEvent tmp_event = {0};
    ArenaEvent *event;
    ArenaParticipant tmp_ap;
    int changed;

    ArenaCommandStart(pak);
    ArenaParticipantUpdateRecieve( &tmp_event, &tmp_ap, pak );

    // verify here
    event = EventGetUnique( tmp_event.eventid, tmp_event.uniqueid );

    if(!event)
    {
        ArenaCommandResult(link, ARENA_ERR_BADEVENT, 0, "BadEvent");
        return;
    }

    if(event->phase >= ARENA_STARTROUND || event->leader_ready )
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "WrongPhase");
        return;
    }

    if(g_request_dbid != tmp_ap.dbid && g_request_dbid != event->creatorid)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "NoPermission");
        return;
    }

    changed = updatePlayer( event, &tmp_ap );

    EventUpdateErrors(event);

    ArenaCommandResult(link, ARENA_SUCCESS, 0, NULL);
    event->lastupdate_dbid = g_request_dbid;
    event->lastupdate_cmd = g_request_user;
    EventSave(event->eventid);
}

void handleFullPlayerUpdate(Packet* pak, NetLink* link)
{
    ArenaEvent tmp_event = {0};
    ArenaEvent *event;
    int i, count;

    ArenaCommandStart(pak);
    ArenaFullParticipantUpdateRecieve( &tmp_event, pak );

    // Verify here
    event = EventGetUnique( tmp_event.eventid, tmp_event.uniqueid );

    if(!event)
    {
        ArenaCommandResult(link, ARENA_ERR_BADEVENT, 0, "BadEvent");
        return;
    }

    if(event->phase >= ARENA_STARTROUND)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "WrongPhase");
        return;
    }

    if(g_request_dbid != event->creatorid)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "NoPermission");
        return;
    }

    count = eaSize(&tmp_event.participants);
    for( i = 0; i < count; i++ )
        updatePlayer( event, tmp_event.participants[i] );

    ArenaCommandResult(link, ARENA_SUCCESS, 0, NULL);
    event->lastupdate_dbid = g_request_dbid;
    event->lastupdate_cmd = g_request_user;
    EventSave(event->eventid);
}


void handleFeePayment(Packet* pak, NetLink* link)
{
    U32 eventid, uniqueid;
    int i, numpayments;
    ArenaParticipant* part;
    ArenaEvent* event;
    static int* dbids = 0;

    if (!dbids)
        eaiCreate(&dbids);
    else
        eaiClear(&dbids);

    eventid = pktGetBitsPack(pak, 1);
    uniqueid = pktGetBitsPack(pak, 1);
    numpayments = pktGetBitsPack(pak, 1);
    eaiSetSize(&dbids, numpayments);
    for (i = 0; i < numpayments; i++)
        dbids[i] = pktGetBitsPack(pak, PKT_BITS_TO_REP_DB_ID);

    event = EventGetUnique(eventid, uniqueid);
    if(!event)
    {
        ArenaCommandResult(link, ARENA_ERR_BADEVENT, 0, "BadEvent");
        return;
    }

    for (i = 0; i < numpayments; i++)
    {
        part = EventGetParticipant(event, dbids[i]);
        if(!part)
            continue;
        part->paid = 1;
    }
    EventSave(eventid);
}

void handleConfirmReward(Packet* pak, NetLink* link)
{
    int dbid;
    ArenaPlayer* ap;

    dbid = pktGetBitsPack(pak, PKT_BITS_TO_REP_DB_ID);
    ap = PlayerGet(dbid,1);
    if(!ap)
        return;
    ap->prizemoney = 0;
    ap->arenaReward[0] = 0;
    PlayerSave(dbid);
}


void handleRequestResults(Packet* pak, NetLink* link) {
    Packet * ret;
    ArenaEvent* event;
    int dbid    =pktGetBitsPack(pak,PKT_BITS_TO_REP_DB_ID);
    int eventid    =pktGetBitsPack(pak,6);
    int uniqueid=pktGetBitsPack(pak,6);

    event = EventGetUnique(eventid, uniqueid);

    if(!event)
    {
        ArenaCommandResult(link, ARENA_ERR_BADEVENT, 0, "BadEvent");
        return;
    }

    if(event->phase <= ARENA_NEGOTIATING)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "WrongPhase");
        return;
    }

    EventProcessRankings(event);

    ret = pktCreateEx(link, ARENASERVER_REQRESULTS);
    pktSendBitsPack(ret,PKT_BITS_TO_REP_DB_ID,dbid);
    ArenaRankingTableSend(ret,event->rankings);

    pktSend(&ret,link);
}

void handleReportKill(Packet *pak, NetLink *link)
{
    Packet *ret;
    int eventid, uniqueid, partid;
    ArenaEvent *event;
    ArenaParticipant *part;

    eventid = pktGetBitsAuto(pak);
    uniqueid = pktGetBitsAuto(pak);
    partid = pktGetBitsAuto(pak);

    event = EventGetUnique(eventid, uniqueid);
    if (!event)
    {
        return;
    }

    part = ArenaParticipantFind(event, partid);
    if (!part)
    {
        return;
    }

    part->kills[event->round]++;
    EventSave(eventid);

    ret = pktCreateEx(link, ARENASERVER_REPORTKILLACK);
    pktSend(&ret,link);
}

void handleMapResults(Packet* pak, NetLink* link)
{
    int mapid, eventid, uniqueid, seat, side, numpart, i;
    U32 time;
    ArenaEvent* event;
    ArenaSeating* seating;
    ArenaParticipant* part;
    static int* dbids = 0;
    static int* kills = 0;
    static int* deaths = 0;

    if (!dbids)
    {
        eaiCreate(&dbids);
        eaiCreate(&kills);
        eaiCreate(&deaths);
    }

    mapid = pktGetBitsPack(pak, 1);
    eventid = pktGetBitsPack(pak, 1);
    uniqueid = pktGetBitsPack(pak, 1);
    seat = pktGetBitsPack(pak, 1);
    side = pktGetBitsPack(pak, 1);
    time = pktGetBitsPack(pak, 1);
    numpart = pktGetBitsPack(pak, 1);
    eaiSetSize(&dbids, numpart);
    eaiSetSize(&kills, numpart);
    eaiSetSize(&deaths, numpart);
    for (i = 0; i < numpart; i++)
    {
        dbids[i] = pktGetBitsPack(pak, PKT_BITS_TO_REP_DB_ID);
        kills[i] = pktGetBitsPack(pak, 1);
        deaths[i] = pktGetBitsPack(pak, 1);
    }

    event = EventGetUnique(eventid, uniqueid);
    if (!event)
    {
        ArenaCommandResult(link, ARENA_ERR_BADEVENT, 0, "BadEvent");
        return;
    }
    if (seat < 0 || seat >= eaSize(&event->seating))
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "BadSeat");
        return;
    }
    seating = event->seating[seat];
    if (seating->mapid != mapid || seating->round != event->round)
    {
        ArenaCommandResult(link, ARENA_ERR_DISALLOWED, 0, "BadSeat");
        return;
    }

    for(i = 0; i < numpart; i++)
    {
        if ((part = ArenaParticipantFind(event, dbids[i])) && part->seats[event->round] == seat)
        {
            part->kills[event->round] = kills[i];
            part->death_time[event->round] = deaths[i];
        }
    }

    ArenaCommandResult(link, ARENA_SUCCESS, 0, NULL);
    seating->matchtime = time;
    seating->winningside = side;
    EventSave(eventid);
}

// the mapserver is giving us updated level, supergroup info.  
// check any events this player belongs to and update them.
static U32 hpau_dbid, hpau_sgid;
int hpau_level, hpau_sgleader;
static int hpau_iter(ArenaEvent* event)
{
    ArenaParticipant* part = ArenaParticipantFind(event, hpau_dbid);
    if (part)
    {
        part->level = hpau_level;
        part->sgid = hpau_sgid;
        part->sgleader = hpau_sgleader;
        EventSave(event->eventid);
    }
    return 1;
}
void handlePlayerAttributeUpdate(Packet* pak, NetLink* link)
{
    hpau_dbid = pktGetBitsPack(pak, PKT_BITS_TO_REP_DB_ID);
    hpau_level = pktGetBitsPack(pak, 1);
    hpau_sgid = pktGetBitsPack(pak, 1);
    hpau_sgleader = pktGetBits(pak, 1);
    ArenaCommandResult(link, ARENA_SUCCESS, 0, NULL);
    EventIter(hpau_iter);
}

void handleClearPlayerSGRatings(Packet* pak, NetLink* link)
{
    int sgidx, errval, dbid = pktGetBitsPack(pak, PKT_BITS_TO_REP_DB_ID);
    ArenaPlayer* ap = PlayerGetAdd(dbid, 0);

    if (!ap)
    {
        LOG_OLD_ERR("handleClearPlayerSGRatings got NULL dbid");
        return;
    }

    sgidx = EventGetRatingIndex(ARENA_FREEFORM, ARENA_SUPERGROUP, ARENA_TOURNAMENT_NONE, 2, 0, &errval);
    if(!errval)
    {
        ap->ratingsByRatingIndex[sgidx] = ARENA_DEFAULT_RATING;
        ap->provisionalByRatingIndex[sgidx] = 1;
        ap->winsByRatingIndex[sgidx] = 0;
        ap->lossesByRatingIndex[sgidx] = 0;
        ap->drawsByRatingIndex[sgidx] = 0;
    }


    sgidx = EventGetRatingIndex(ARENA_FREEFORM, ARENA_SUPERGROUP, ARENA_TOURNAMENT_SWISSDRAW, 2, 0, &errval);
    if(!errval)
    {
        ap->ratingsByRatingIndex[sgidx] = ARENA_DEFAULT_RATING;
        ap->provisionalByRatingIndex[sgidx] = 1;
        ap->winsByRatingIndex[sgidx] = 0;
        ap->lossesByRatingIndex[sgidx] = 0;
        ap->drawsByRatingIndex[sgidx] = 0;
    }
    PlayerSave(dbid);
}

void handleRequestLeaderUpdate(Packet* pak, NetLink* link)
{
    Packet* ret;

    int dbid = pktGetBitsPack(pak, PKT_BITS_TO_REP_DB_ID);

    ret = pktCreateEx(link, ARENASERVER_LEADERUPDATE);

    pktSendBitsPack(ret, PKT_BITS_TO_REP_DB_ID, dbid);

    ArenaLeaderBoardSend(ret, g_leaderboard);

    pktSend(&ret,link);
}

void handleRequestPlayerStats(Packet* pak, NetLink* link)
{
    Packet* ret = NULL;
    ArenaPlayer* ap = NULL;
    char* statstr = NULL;
    int dbid = pktGetBitsPack(pak, PKT_BITS_TO_REP_DB_ID);
    int count;

    estrCreate(&statstr);

    ret = pktCreateEx(link, ARENASERVER_PLAYERSTATS);
    pktSendBitsPack(ret, PKT_BITS_TO_REP_DB_ID, dbid);

    if(dbid)
    {
        ap = PlayerGet(dbid, 0);
    }

    if(ap)
    {
        estrConcatf(&statstr, "Stats for player DBID %d\n", ap->dbid);
        estrConcatf(&statstr, "Prize money: %d\n", ap->prizemoney);
        estrConcatf(&statstr, "Connection state %d, last connected %d\n", ap->connected, ap->lastConnected);
        estrConcatf(&statstr, "%d wins, %d draws, %d losses\n", ap->wins, ap->draws, ap->losses);

        estrConcatStaticCharArray(&statstr, "\nStats by event type:\n");

        for (count = 0; count < ARENA_NUM_EVENT_TYPES_FOR_STATS; count++)
        {
            estrConcatf(&statstr, "%3d: %d wins, %d draws, %d losses\n",
                ap->winsByEventType[count], ap->drawsByEventType[count],
                ap->lossesByEventType[count]);
        }

        estrConcatStaticCharArray(&statstr, "\nStats by team type:\n");

        for (count = 0; count < ARENA_NUM_TEAM_TYPES; count++)
        {
            estrConcatf(&statstr, "%3d: %d wins, %d draws, %d losses\n",
                ap->winsByTeamType[count], ap->drawsByTeamType[count],
                ap->lossesByTeamType[count]);
        }

        estrConcatStaticCharArray(&statstr, "\nStats by victory type:\n");

        for (count = 0; count < ARENA_NUM_VICTORY_TYPES; count++)
        {
            estrConcatf(&statstr, "%3d: %d wins, %d draws, %d losses\n",
                ap->winsByVictoryType[count], ap->drawsByVictoryType[count],
                ap->lossesByVictoryType[count]);
        }

        estrConcatStaticCharArray(&statstr, "\nStats by weight class:\n");

        for (count = 0; count < ARENA_NUM_WEIGHT_CLASSES; count++)
        {
            estrConcatf(&statstr, "%3d: %d wins, %d draws, %d losses\n",
                ap->winsByWeightClass[count], ap->drawsByWeightClass[count],
                ap->lossesByWeightClass[count]);
        }

        estrConcatStaticCharArray(&statstr, "\nStats by rating:\n");

        for (count = 0; count < ARENA_NUM_RATINGS; count++)
        {
            estrConcatf(&statstr, "%3d: %d wins, %d draws, %d losses\n",
                ap->winsByRatingIndex[count], ap->drawsByRatingIndex[count],
                ap->lossesByRatingIndex[count]);
        }
    }
    else
    {
        estrConcatf(&statstr, "No arena player stats for DBID %d", dbid);
    }

    pktSendString(ret, statstr);
    pktSend(&ret, link);

    estrDestroy(&statstr);
}

void handleReadyAck(Packet *pak, NetLink *link)
{
    int goodData = pktGetBitsAuto(pak);
    if (goodData)
    {
        int amReady = pktGetBitsAuto(pak);
        int dbid = pktGetBitsAuto(pak);
        char *playername = pktGetString(pak);
        ArenaEvent *event = EventGetByDbid(dbid);

        if (event && event->phase == ARENA_WAITREADY)
        {
            if (amReady)
            {
                event->finalReadiesLeft--;
            }
            else
            {
                ClientSendNotReadyMessage(EventGetId(event), playername);
                event->start_time = 0;
                event->leader_ready = false;
                EventChangePhase(event, ARENA_NEGOTIATING);
                EventSave(event->eventid);
            }
        }
    }
}

// *********************************************************************************
//  Server processing
// *********************************************************************************

// bunch of time limits for arena stuff - all in seconds
#define ARENA_ASAP_WAIT_AFTER_REQUIREMENTS    (2 * 60)
#define ARENA_RETAIN_ENDED_EVENTS            (15 * 60)
#define ARENA_RETAIN_CANCELLED_EVENTS        (5 * 60)
#define ARENA_CHECK_MAP_FREQ                10

// incremental or odd/even
static void AssignSides(ArenaEvent* event, int incrementally)
{
    int i, n;
    int count = 0;

    n = eaSize(&event->participants);
    for (i = 0; i < n; i++)
    {
        if (event->participants[i]->dbid)
        {
            count++;
            if (incrementally)
                event->participants[i]->side = count;
            else
                event->participants[i]->side = (count % 2)? 1: 2;
        }
    }
}

// fixup anyone with a side of zero - ARGG
static void AssignUnassignedSides(ArenaEvent* event)
{
    int sidecounts[ARENA_MAX_SIDES+1] = {0};
    int nextside = 1;
    int i, n;

    // count up our sides first
    n = eaSize(&event->participants);
    for (i = 0; i < n; i++)
    {
        if (!event->participants[i]->dbid) continue;
        if (event->participants[i]->side <= 0 || event->participants[i]->side > ARENA_MAX_SIDES) continue;
        sidecounts[event->participants[i]->side]++;
    }

    // now assign anybody left out
    for (i = 0; i < n; i++)
    {
        if (!event->participants[i]->dbid) continue;
        if (event->participants[i]->side <= 0 || event->participants[i]->side > ARENA_MAX_SIDES)
        {
            while (sidecounts[nextside] && nextside <= ARENA_MAX_SIDES) nextside++;
            if (nextside > ARENA_MAX_SIDES) nextside = 1;
            event->participants[i]->side = nextside;
            nextside++;
        }
    }
}

// directly before we run the event
void EventPrepare(ArenaEvent* event)
{
    // assign sides if necessary
    if (event->teamType == ARENA_SINGLE || event->teamType == ARENA_SUPERGROUP)
        AssignSides(event, 1);
    else if(SERVER_RUN(event))
        AssignSides(event, 0);

    // fill in rankings for sanctioned events
    if(event->sanctioned)
    {
        int i;
        ArenaPlayer* ap;
        ArenaParticipant* part;

        for(i = eaSize(&event->participants)-1; i >= 0; i--)
        {
            part = event->participants[i];
            if(part->dbid)
            {
                ap = PlayerGetAdd(part->dbid, 0);
                if (ap)
                {
                    part->rating = ap->ratingsByRatingIndex[EventGetRatingIndex(event->teamStyle, event->teamType, event->tournamentType, event->numsides, event->use_gladiators, NULL)];
                    part->provisional = ap->provisionalByRatingIndex[EventGetRatingIndex(event->teamStyle, event->teamType, event->tournamentType, event->numsides, event->use_gladiators, NULL)];
                }
            }
        }
    }

    if (!event->mapname[0])
        ArenaSelectMap(event);
}

void EventPrepareRound(ArenaEvent* event)
{
    int i;

    // first drop any empty participants
    for (i = eaSize(&event->participants)-1; i >= 0; i--)
    {
        if (!event->participants[i]->dbid)
            ArenaParticipantDestroy(eaRemove(&event->participants, i));
    }
    // now we only have slots with actual players..

    if (event->tournamentType == ARENA_TOURNAMENT_SWISSDRAW)
    {
        EventProcessRankings(event);
        EventCreatePairingsSwissDraw(event); // creates seating
        // TODO depends on round?
    }
    else // otherwise, just a single round/single seat competition
    {
        ArenaSeating* seat;

        // create a single seat
        seat = ArenaSeatingCreate();    // insert a dummy seat for ranking
        seat->round = 0;
        seat->winningside = -1;
        if (!event->seating)
            eaCreate(&event->seating);
        eaPush(&event->seating, seat);

        seat = ArenaSeatingCreate();
        seat->round = event->round;
        eaPush(&event->seating, seat);

        for (i = eaSize(&event->participants)-1; i >= 0; i--)
        {
            event->participants[i]->seats[1] = 1;
        }
    }
    // anything else?
}

// start a map for each seat in this round
int EventStartMaps(ArenaEvent* event)
{
    int i, n;

    n = eaSize(&event->seating);
    for (i = 0; i < n; i++)
    {
        if (event->seating[i]->round == event->round)
        {
            char buf[1000];
            char* mapinfo = ArenaConstructInfoString(event->eventid, i);
            sprintf(buf,"MapName \"%s\"\nMissionInfo \"%s\"\n",event->mapname,mapinfo);
            dbAsyncContainerUpdate(CONTAINER_MAPS,-1,CONTAINER_CMD_CREATE,buf,0);
            if (dbMessageScanUntil(__FUNCTION__, NULL))
            {
                event->seating[i]->mapid = db_state.last_id_acked;
                LOG_OLD_ERR("arenaserver request map %s, got %i", mapinfo, db_state.map_id);
            }
            else
            {
                LOG_OLD_ERR("EventStartMaps failed to complete request with dbserver");
                return 0;
            }
        }
    }
    return 1;
}

// signal an error if any of my arena maps have crashed
int EventCheckMaps(ArenaEvent* event)
{
    ContainerStatus c_list[1];
    int i, n, eventid, seat;

    n = eaSize(&event->seating);
    for (i = 0; i < n; i++)
    {
        if (event->seating[i]->round == event->round && !event->seating[i]->winningside)
        {
            c_list[0].id = event->seating[i]->mapid;
            dbSyncContainerStatusRequest(CONTAINER_MAPS, c_list, 1);
            if (!c_list[0].valid) 
            {
                printf("map not found: !c_list[0].valid\n");
                return 0;
            }
            
            if (!ArenaDeconstructInfoString(c_list[0].mission_info, &eventid, &seat))
            {
                printf("!ArenaDeconstructInfoString: %s\n",c_list[0].mission_info);
                return 0;
            }
            
            if (eventid != event->eventid || i != seat)
            {
                LOG_OLD("eventid(%i) != event->eventid(%i) || i(%i) != seat(%i)\n",eventid,event->eventid, i, seat);
                return 0;
            }
            
        }
    }

    return 1;
}

int AllMapsReported(ArenaEvent* event)
{
    int i, n;
    
    n = eaSize(&event->seating);
    for (i = 0; i < n; i++)
    {
        if (event->seating[i]->round == event->round &&
            !event->seating[i]->winningside)
            return 0;
    }
    return 1;
}

static int NumRounds(ArenaEvent* event)
{
    if (event->tournamentType == ARENA_TOURNAMENT_SWISSDRAW)
        return event->numplayers > 8 ? log2(event->numplayers) + 1 : log2(event->numplayers);
    return 1;
}

void EventLog(ArenaEvent* event, char* logfile)
{
    static char* estr = 0;
    if (g_disableEventLog) 
        return;
    if (!estr)
        estrCreate(&estr);
    estrClear(&estr);
    ArenaEventDebugPrint(event, &estr, dbSecondsSince2000());
    LOG(LOG_ARENA_EVENT, LOG_LEVEL_VERBOSE, 0, "%s", estr);
}

void EventChangePhase(ArenaEvent* event, ArenaPhase phase)
{
    if(g_arenaVerbose)
    {
        printf("%i-%i: Switching from phase %s to %s\n", event->eventid, event->uniqueid,
                        StaticDefineIntRevLookup(ParseArenaPhase, event->phase),
                        StaticDefineIntRevLookup(ParseArenaPhase, phase));
    }

    event->phase = phase;
}

static void DropAllPlayersInThisEventFromOtherEvents(ArenaEvent *event)
{
    int playerIndex, eventIndex, eventPlayerIndex;

    for (playerIndex = 0; playerIndex < event->numplayers; playerIndex++)
    {
        for (eventIndex = 0; eventIndex < eaSize(&g_events.events); eventIndex++)
        {
            ArenaEvent *iterEvent = g_events.events[eventIndex];
            if (!iterEvent || iterEvent == event)
            {
                continue;
            }
            for (eventPlayerIndex = 0; eventPlayerIndex < eaSize(&iterEvent->participants); eventPlayerIndex++)
            {
                if (iterEvent->participants[eventPlayerIndex]->dbid == event->participants[playerIndex]->dbid)
                {
                    EventRemoveParticipant(iterEvent, iterEvent->participants[eventPlayerIndex]->dbid);
                    continue;
                }
            }
        }
    }
}

// main loop to check all events & do state changes
static int* ep_removelist = NULL;
int EventProcessEach(ArenaEvent* event)
{
    int remove = 0;
    int dirty = 0;
    U32 time = dbSecondsSince2000();

    ArenaEventCountPlayers(event);
    switch (event->phase)
    {
    case ARENA_SCHEDULED:
        // note when we started this phase
         if (!event->round_start)
        {
            event->round_start = time;
            dirty = 1;
        }

         if (!event->scheduled) 
        {
            LOG_OLD_ERR( "Ended up with unscheduled event in scheduled state %i", event->eventid);
            EventChangePhase(event, ARENA_NEGOTIATING);
            event->start_time = time + ARENA_SERVER_NEGOTIATION_WAIT;
            dirty = 1;
        }
        else if (event->start_time) // scheduled for a particular time
        {
            if (time >= event->start_time - ARENA_SERVER_NEGOTIATION_WAIT)
            {
                // if we didn't get players in time, advance to next hour mark
                if (event->numplayers < event->minplayers)
                {
                    if (IncrementScheduledEvent(event)) // also resets ASAP events here, btw
                    {
                        printf("Rescheduling event %i-%i\n", event->eventid, event->uniqueid);
                        dirty = 1;
                    }
                    else
                        remove = 1;
                }
                else
                {
                    EventChangePhase(event, ARENA_NEGOTIATING);
                    dirty = 1;
                    
                    DropAllPlayersInThisEventFromOtherEvents(event);
                }
            }
            // if players are allowed to demand starting the event, start when maxplayers
            else if (event->numplayers == event->maxplayers && event->demandplay)
            {
                EventChangePhase(event, ARENA_NEGOTIATING);
                event->start_time = time + ARENA_SERVER_NEGOTIATION_WAIT;
                dirty = 1;

                DropAllPlayersInThisEventFromOtherEvents(event);
            }
        }
        else // ASAP tourney
        {
            if (event->numplayers >= event->minplayers && EventSidesOk(event))
            {
                // need additional time to see if we will get more than min players
                event->start_time = time + ARENA_ASAP_WAIT_AFTER_REQUIREMENTS;
                dirty = 1;
            }
            // FUTURE: if scheduled stops being equivalent to server-started, need a way to break out here
        }
        break;
    case ARENA_NEGOTIATING:
        if (SERVER_RUN(event))
        {
            if (!EventSidesOk(event))
            {
                event->round_start = time;
                EventChangePhase(event, ARENA_CANCELLED);
                strcpy(event->cancelreason, "TooManyDropped");
                dirty = 1;
            }
            else if (time >= event->start_time) // timed out of negotiation time
            {
                event->start_time = time;
                event->round_start = time;
                EventChangePhase(event, ARENA_STARTREADY);
                // don't set dirty, don't want to update to clients just yet
            }
            // TODO - this short circuit would be nice, but the arena server doesn't know if 
            //   players are in the lobby currently
            //else if (AllPlayersInLobby(event) && event->start_time - time > ARENA_SERVER_NEGOTIATION_READY)
            //    // need to short circuit one minute wait time, everyone is ready
            //{
            //    event->start_time = time + ARENA_SERVER_NEGOTIATION_READY;
            //    dirty = 1;
            //}
        }
        else // player events
        {
            DropAllPlayersInThisEventFromOtherEvents(event);
            if (event->numplayers == 0) // everyone dropped
            {
                event->round_start = time;
                EventChangePhase(event, ARENA_CANCELLED);
                strcpy(event->cancelreason, "TooManyDropped");
                dirty = 1;
            }
            else if ( event->start_time && time >= event->start_time && EventSidesOk(event))
            {
                event->start_time = time;
                event->round_start = time;
                EventChangePhase(event, ARENA_STARTREADY);
                // don't set dirty, don't want to update to clients just yet
            }
        }
        break;
    case ARENA_STARTREADY:
        event->finalReadiesLeft = event->numplayers;
        ClientRequestReadyMessages(event);
        EventChangePhase(event, ARENA_WAITREADY);
        break;
    case ARENA_WAITREADY:
        if (!event->finalReadiesLeft || time >= event->start_time + ARENA_WAIT_PERIOD_FOR_READIES)
        {
            EventChangePhase(event, ARENA_STARTROUND);
            dirty = 1;
        }
        break;
    case ARENA_STARTROUND:
        if (event->round <= 1)
        {
            event->round = 1;
            EventPrepare(event);
            printf("Starting event %i-%i: %s\n", event->eventid, event->uniqueid, 
                event->description[0]? event->description: event->creatorname);
        }
        EventPrepareRound(event);
        if (!EventStartMaps(event))
        {
            event->round_start = time;
            EventChangePhase(event, ARENA_CANCEL_REFUND);
            printf("Error starting maps for event %i-%i\n", event->eventid, event->uniqueid);
            strcpy(event->cancelreason, "ArenaServerError");
            dirty = 1;
            break;
        }
        event->round_start = time;
        event->mapcheck_time = time;
        dirty = 1;
        EventChangePhase(event, ARENA_RUNROUND);
        break;
    case ARENA_RUNROUND:
        if (time >= event->mapcheck_time + ARENA_CHECK_MAP_FREQ)
        {
            event->mapcheck_time = time;
            if (AllMapsReported(event))
            {
                EventChangePhase(event, ARENA_ENDROUND);
                dirty = 1;
            } 
            else if (!EventCheckMaps(event))
            {
                event->round_start = time;
                EventChangePhase(event, ARENA_CANCEL_REFUND);
                printf("Detected map crash, cancelling event %i-%i\n", event->eventid, event->uniqueid);
                strcpy(event->cancelreason, "ArenaMapError");
                dirty = 1;
            }
                
            
        }
        break;
    case ARENA_ENDROUND:
        EventProcessRankings(event);
        if (event->round >= NumRounds(event))
        {
            event->round_start = time;
            if(event->entryfee)
                EventChangePhase(event, ARENA_PAYOUT_WAIT);
            else
                EventChangePhase(event, ARENA_PRIZE_PAYOUT);
            dirty = 1;
        }
        else
        {
            EventChangePhase(event, ARENA_INTERMISSION);
            event->next_round_time = time + ARENA_INTERMISSION_WAIT;
            dirty = 1;
        }
        break;
    case ARENA_INTERMISSION:
        if (!event->arenaTourneyIntermissionStartMsgSent)
        {
            event->arenaTourneyIntermissionStartMsgSent = 1;
            ClientSendNextRoundMessage(event->eventid, event->next_round_time - time);
        }
        if (event->next_round_time && event->next_round_time - time <= 15 &&
            !event->arenaTourney15SecsMsgSent)
        {
            event->arenaTourney15SecsMsgSent = 1;
            ClientSendNextRoundMessage(event->eventid, event->next_round_time - time);
        }
        if (event->next_round_time && event->next_round_time - time <= 5 &&
            !event->arenaTourney5SecsMsgSent)
        {
            event->arenaTourney5SecsMsgSent = 1;
            ClientSendNextRoundMessage(event->eventid, event->next_round_time - time);
        }
        if (!event->next_round_time || event->next_round_time - time <= 0)
        {
            event->round++;
            event->next_round_time = 0;
            EventChangePhase(event, ARENA_STARTROUND);
            dirty = 1;
            event->arenaTourneyIntermissionStartMsgSent = 0;
            event->arenaTourney15SecsMsgSent = 0;
            event->arenaTourney5SecsMsgSent = 0;
        }
        break;
    case ARENA_PAYOUT_WAIT:
        if(time >= event->round_start + ARENA_WAIT_FOR_PAYOUT)
            EventChangePhase(event, ARENA_PRIZE_PAYOUT);
        break;
    case ARENA_PRIZE_PAYOUT:
        printf("Completed event %i-%i\n", event->eventid, event->uniqueid);
        EventProcessRankings(event);
        EventUpdateWinLossCounts(event);
        EventUpdateRatings(event);
        EventDistributePrizes(event);
        event->round_start = time;
        EventChangePhase(event, ARENA_ENDED);
        dirty = 1;
        break;
    case ARENA_ENDED:
        if (time >= event->round_start + ARENA_RETAIN_ENDED_EVENTS)
        {
            EventLog(event, "arenacomplete");
            remove = 1;
        }
        break;
    case ARENA_CANCEL_REFUND:
        EventProcessRankings(event);
        EventDistributeRefunds(event);
        event->round_start = time;
        EventChangePhase(event, ARENA_CANCELLED);
        dirty = 1;
        break;
    case ARENA_CANCELLED:
        if (time >= event->round_start + ARENA_RETAIN_CANCELLED_EVENTS)
        {
            EventLog(event, "arenacomplete");
            remove = 1;
        }
        break;
    }

    if (remove)
        eaiPush(&ep_removelist, event->eventid);
    else if (dirty) 
        EventSave(event->eventid);
    return 1;
}
void EventProcess(void)
{
    int i, n;

    if (!ep_removelist)
        eaiCreate(&ep_removelist);
    eaiSetSize(&ep_removelist, 0);

    EventIter(EventProcessEach);

    n = eaiSize(&ep_removelist);
    for (i = 0; i < n; i++)
    {
        EventDelete(ep_removelist[i]);
    }
}

// this player just disconnected.  he needs to drop from any events that are
// in negotiation.
U32 plo_dbid;
static int playerLoggedOut(ArenaEvent* event)
{
    if (event->phase == ARENA_NEGOTIATING)
    {
        ArenaParticipant* part = ArenaParticipantFind(event, plo_dbid);
        if (part)
        {
            EventRemoveParticipant(event, plo_dbid);
        }
    }
    return 1;
}
void EventPlayerLoggedOut(U32 dbid)
{
    plo_dbid = dbid;
    EventIter(playerLoggedOut);
}

// we just loaded the event list from the database, do any kind of sanity checks we need here
static int eventInit(ArenaEvent* event)
{
    event->numplayers = eaSize(&event->participants);
    if (event->numplayers == 0 && INSTANT_EVENT(event) && !SERVER_RUN(event))
    {
        EventDelete(event->eventid);
    }
    if (event->scheduled)
    {
        event->creatorid = 0;
        EventSave(event->eventid);
    }
    return 1;
}
void EventListInit(void)
{
    EventIter(eventInit);
}
