/*\
 *
 *    arenamapserver.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Client code on the mapserver side for talking to arenaserver.
 *
 */

#include "arenamapserver.h"
#include "gameData/arenastruct.h"
#include "comm_backend.h"
#include "comm_game.h"
#include "dbcomm/dbcomm.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/utils.h>
#include "cmdparse/cmdcommon.h"
#include "svr/svr_base.h"
#include "gameComm/sendToClient.h"
#include "svr/svr_player.h"
#include <utilitieslib/utils/timing.h>
#include "entity/entPlayer.h"
#include "entity/character_combat.h"
#include "door.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/buddy_server.h"
#include "arenamap.h"
#include "entity/origins.h"
#include "entity/classes.h"
#include <utilitieslib/components/StringCache.h>
#include "gameData/arenastruct.h"
#include "gameComm/svr_chat.h"
#include "language/langServerUtil.h"
#include "dbcomm/staticMapInfo.h"
#include "dbcomm/dbmapxfer.h"
#include "dooranim.h"
#include "dbcomm/dbnamecache.h"
#include "entity/entity.h"
#include "entity/SgrpServer.h"
#include "kiosk.h"
#include "raidmapserver.h"
#include "entity/Supergroup.h"
#include "Reward.h"
#include "player/badges_server.h"
#include "entity/entGameActions.h"
#include <utilitieslib/utils/log.h>

#define ARENA_ALLOW_ENTRY_TIMEOUT    30

// *********************************************************************************
//  Networking
// *********************************************************************************

static char arena_comm_address[20];
NetLink arena_comm_link;
static int arena_comm_ack = 0;

static int g_kiosksubscribe = 0;
static U32 g_kiosklastupdate = 0;
ArenaEventList g_kiosk;
ArenaEvent** g_detaillist;
static int* g_kioskwaitlist;

// look for a particular event update
static int g_eventwaitfor = 0;

// ack back from arenaserver
static ArenaServerResult g_arenaresult = ARENA_ERR_NONE;
static U32 g_arenaresultnum = 0;
static U32 g_arenaresultdbid = 0;
static U32 g_arenaresultcmd = 0;
static U32 g_arenaresulteventid = 0;
static U32 g_arenaresultuniqueid = 0;
static char g_arenaresulterr[256];

static ArenaRankingTable g_mapserver_leaderboard[ARENA_NUM_RATINGS];

// if turned on, get periodic kiosk refreshes from arenaserver
void ArenaKioskSubscribe(int on)
{
    g_kiosksubscribe = on;
}

int ArenaClientCallback(Packet *pak, int cmd, NetLink *link)
{
    switch(cmd)
    {
        xcase ARENASERVER_KIOSK:
            handleArenaKiosk(pak);
/*            while (eaiSize(&g_kioskwaitlist))
            {
                U32 dbid = eaiPop(&g_kioskwaitlist);
                ArenaSendKioskInfo(dbid);
            }
*/
        xcase ARENASERVER_RESULT:
            handleArenaResult(pak);
        xcase ARENASERVER_EVENTUPDATE:
            handleEventUpdate(pak);
        xcase ARENASERVER_REQREADY:
            handleRequestReady(pak);
        xcase ARENASERVER_PARTICIPANTUPDATE:
            handleUpdateParticipant(pak);
        xcase ARENASERVER_PLAYERUPDATE:
            handleUpdatePlayer(pak);
        xcase ARENASERVER_REQRESULTS:
            handleClientRequestResults(pak);
        xcase ARENASERVER_LEADERUPDATE:
            handleUpdateLeaderList(pak);
        xcase ARENASERVER_PLAYERSTATS:
            handlePlayerStats(pak);
        xcase ARENASERVER_SENDNOTREADYMESSAGE:
            handleSendNotReadyMessage(pak);
        xcase ARENASERVER_SENDNEXTROUNDMESSAGE:
            handleSendNextRoundMessage(pak);
        xcase ARENASERVER_REPORTKILLACK:
            handleReportKillAck(pak);
                
        xdefault:
            return RaidClientCallback(pak, cmd, link);
            printf("Unknown command %d\n",cmd);
            return 0;
    }
    return 1;
}

#define MAX_KIOSK_INCREMENTAL_BYTES            256
void ArenaKioskSendIncremental(Entity* e)
{
    if (!e->pl->arenaKioskSend && !e->pl->arenaKioskSendInit)
        return;

    // this packet allows for incremental updates..  we do them based on actual
    // packet size as we go
    START_PACKET(pkt, e, SERVER_ARENA_KIOSK);
    pktSendBits(pkt, 1, 1);                                    // no error
    pktSendBits(pkt, 1, e->pl->arenaKioskSendInit? 1: 0);    // initial packet
    e->pl->arenaKioskSendInit = 0;

    // special case: can get here event though there are no events running..
    if (e->pl->arenaKioskSend)
    do 
    {
        ArenaEvent* event;
        if (e->pl->arenaKioskSendNext >= eaSize(&e->pl->arenaKioskSend)) break;
        event = e->pl->arenaKioskSend[e->pl->arenaKioskSendNext++];
        if (!event || !event->kiosksend) continue;
        pktSendBits(pkt, 1, 1);                                // have another event
        ArenaEventSend(pkt, event, 0);                        // event
    } while (pktGetSize(pkt) < MAX_KIOSK_INCREMENTAL_BYTES * 8); // keep filling packet as long as I have space
    pktSendBits(pkt, 1, 0);                                    // done with events
    END_PACKET

    // when I'm totally done with updates
    if (e->pl->arenaKioskSendNext >= eaSize(&e->pl->arenaKioskSend))
    {
        eaDestroyEx(&e->pl->arenaKioskSend,ArenaEventDestroy);
        e->pl->arenaKioskSend = NULL;
    }
}

void handleArenaKiosk(Packet* pak)
{
    int i;
    ArenaEventList list;
    Entity * e=entFromDbId(pktGetBitsPack(pak,PKT_BITS_TO_REP_DB_ID));
    int filterSupergroups=pktGetBits(pak,1);
    if(!e)
        return;
    list.events=0;

    ArenaKioskReceive(pak,&list);

    for (i=0;i<eaSize(&list.events);i++) {
        list.events[i]->kiosksend=1;
        EventIgnoreFilter(list.events[i], e);
        if (filterSupergroups)
            EventSupergroupFilter(list.events[i],e);
    }

    if (e->pl->arenaKioskSend)
    {
        eaDestroyEx(&e->pl->arenaKioskSend, ArenaEventDestroy);
    }
    e->pl->arenaKioskSend = list.events;
    e->pl->arenaKioskSendNext = 0;
    e->pl->arenaKioskSendInit = 1;
    // set up for incremental sends during server updates
}

// most arena command give back ARENASERVER_RESULT immediately,
// which echoes the dbid and command used, and optionally gives
// an error string
void handleArenaResult(Packet* pak)
{
    g_arenaresult = pktGetBitsPack(pak, 1);
    g_arenaresultnum = pktGetBitsPack(pak, 1);
    g_arenaresultdbid = pktGetBitsPack(pak, 1);
    g_arenaresultcmd = pktGetBitsPack(pak, 1);
    g_arenaresulteventid = pktGetBitsPack(pak, 1);
    g_arenaresultuniqueid = pktGetBitsPack(pak, 1);
    strncpyt(g_arenaresulterr, pktGetString(pak), ARRAY_SIZE(g_arenaresulterr));

    // errors get forwarded directly to client that requested operation (if any)
    if (g_arenaresult != ARENA_SUCCESS && g_arenaresultdbid)
    {
        Entity* e = entFromDbId(g_arenaresultdbid);
        if (e)
        {
            if( g_arenaresult == ARENA_ERR_POPUP )
            {
                START_PACKET(pkt, e, SERVER_ARENA_ERROR)
                    pktSendBitsPack(pkt, 1, g_arenaresultcmd);
                    pktSendBitsPack(pkt, 1, g_arenaresulteventid);
                    pktSendBitsPack(pkt, 1, g_arenaresultuniqueid);
                    pktSendString(pkt, g_arenaresulterr);
                END_PACKET
            }
            else
                sendChatMsg( e, localizedPrintf(e,g_arenaresulterr), INFO_ARENA_ERROR, 0 );


        }
    }
}

void handleArenaAddress(Packet* pak)
{
    int haveone = pktGetBits(pak, 1);
    if (haveone)
        strncpyt(arena_comm_address, pktGetString(pak), ARRAY_SIZE(arena_comm_address));
    else
        strcpy(arena_comm_address, "none");
}

#define ARENA_CONNECT_FREQ 10
typedef enum {
    AC_NONE,
    AC_REQUESTADDRESS,
    AC_WAITADDRESS,
    AC_WAITFAIL,
} ArenaConnectState;
static ArenaConnectState g_arena_connecting = 0;
static U32 g_arena_lastconnect = 0;
static int g_arena_connect_syncronous = 0;
static void ArenaConnectTick(void)
{
    switch (g_arena_connecting)
    {
    case AC_NONE: return;
    case AC_REQUESTADDRESS:
        arena_comm_address[0] = 0;
        dbReqArenaAddress(g_arena_connect_syncronous);
        g_arena_connecting = AC_WAITADDRESS;
        break;
    case AC_WAITADDRESS:
        if (0 == stricmp(arena_comm_address, "none"))
            goto act_fail;
        if (arena_comm_address[0])
        {
            if (!netConnect(&arena_comm_link,arena_comm_address,DEFAULT_ARENASERVER_PORT,NLT_TCP,1.f,NULL))
                goto act_fail;
            // success
            arena_comm_link.cookieSend = 1;
            g_arena_connecting = AC_NONE;
            ArenaRegisterAllPlayers();

            if(ArenaAsyncConnect())
            { // update logging levels when we connect
                int i;
                Packet * pak_out = pktCreateEx(&arena_comm_link, ARENACLIENT_SET_LOG_LEVELS);
                for( i = 0; i < LOG_COUNT; i++ ) 
                        pktSendBitsPack(pak_out, 1, logLevel(i) );
                pktSend(&pak_out, &arena_comm_link);
            }
            return;
        }
        break;
    case AC_WAITFAIL:
        if (g_arena_connect_syncronous || ABS_TIME_SINCE(g_arena_lastconnect) > SEC_TO_ABS(ARENA_CONNECT_FREQ))
        {
            g_arena_connecting = AC_NONE;
        }
        break;
    default:
        goto act_fail;
    }
    return;
act_fail:
    g_arena_connecting = AC_WAITFAIL;
    g_arena_lastconnect = ABS_TIME;
    while (eaiSize(&g_kioskwaitlist))
    {
        U32 dbid = eaiPop(&g_kioskwaitlist);
        ArenaSendKioskFail(dbid);
    }
}

int ArenaAsyncConnect(void)
{
    if (!g_arena_connecting && !arena_comm_link.connected) 
        g_arena_connecting = AC_REQUESTADDRESS;
    return arena_comm_link.connected;
}

int ArenaSyncConnect(void)
{
    if (!arena_comm_link.connected)
    {
        g_arena_connect_syncronous = 1;
        g_arena_connecting = AC_REQUESTADDRESS;
        while (g_arena_connecting) ArenaConnectTick();
        g_arena_connect_syncronous = 0;
    }
    return arena_comm_link.connected;
}

ArenaServerResult ArenaSyncResult(void)
{
    g_arenaresult = ARENA_ERR_NONE;

    PERFINFO_AUTO_START("ArenaSyncResult", 1);
    netLinkMonitorBlock(&arena_comm_link, LOOK_FOR_COOKIE, ArenaClientCallback, 100000000.f);
    PERFINFO_AUTO_STOP();

    return g_arenaresult;
}

void ArenaClientTick(void)
{
    ArenaAsyncConnect();
    ArenaConnectTick();
    netLinkMonitor(&arena_comm_link, 0, ArenaClientCallback);

    // kiosk clients
    if (g_kiosksubscribe)
    {
        // only once every ten seconds
        if (ABS_TIME_SINCE(g_kiosklastupdate) < SEC_TO_ABS(10))
            return;
        g_kiosklastupdate = ABS_TIME;
//        ArenaRequestKioskInfo();    the should be taking care of refreshing the kiosk
    }
    ArenaMapProcess();
}

void ArenaRequestKioskInfo(void)
{
    if (ArenaSyncConnect())
    {
        Packet* pak = pktCreateEx(&arena_comm_link, ARENACLIENT_REQKIOSK);
        pktSend(&pak, &arena_comm_link);
    }
}

// *********************************************************************************
//  Detailed event list - ordered list of events I have detailed info on
// *********************************************************************************

// yet another binary search.. 
ArenaEvent* FindEventDetail(U32 eventid, int add, int* index)
{
    int low, high, found = 0;

    if (!g_detaillist)
        eaCreate(&g_detaillist);
    low = 0;
    high = eaSize(&g_detaillist);
    while (high > low)
    {
        int mid = (high + low) / 2;
        if (g_detaillist[mid]->eventid == eventid)
            low = high = mid;
        else if (g_detaillist[mid]->eventid > eventid)
            high = mid;
        else if (mid == low)
            low = high;
        else
            low = mid;

    }
    found = (low < eaSize(&g_detaillist) && g_detaillist[low]->eventid == eventid);
    if (index) *index = low;
    if (add && !found)
    {
        eaInsert(&g_detaillist, ArenaEventCreate(), low);
        return g_detaillist[low];
    }
    if (found)
        return g_detaillist[low];
    return NULL;
}

// mapservers fill out participant names as they get detailed info
void FillParticipantNames(ArenaEvent* event)
{
    int i, n;
    n = eaSize(&event->participants);
    for (i = 0; i < n; i++)
    {
        event->participants[i]->name = strdup(dbPlayerNameFromId(event->participants[i]->dbid));
    }
}


static void ScheduledEventTeleportMsgSend(ArenaEvent* event, Entity* e)
{
    START_PACKET(pak, e, SERVER_ARENA_SCHEDULED_TELEPORT);
    pktSendBits(pak, 1, OnArenaMap() ? 1 : 0);
    pktSendBitsPack(pak, 1, event->eventid);
    END_PACKET;
}

void SendServerRunEventWindow(ArenaEvent* event, Entity* e)
{
    START_PACKET(pak, e, SERVER_ARENA_RUN_EVENT_WINDOW);
    ArenaEventSend(pak, event, 1);
    END_PACKET;
}

void ArenaSendScheduledMessages(Entity* e)
{
    int j;
    char buf[80];
    int time = timerSecondsSince2000();
    ArenaEvent *event = FindEventDetail(e->pl->arenaActiveEvent.eventid, 0, NULL);

    if (!event)
    {
        ArenaSyncRequestEvent(e->pl->arenaActiveEvent.eventid);
        event = FindEventDetail(e->pl->arenaActiveEvent.eventid, 0, NULL);
    }

    if(event && event->phase == ARENA_INTERMISSION)
    {
        for(j = eaSize(&event->participants)-1; j >= 0; j--)
        {
            if(!event->participants[j]->dropped && event->participants[j]->dbid == e->db_id)
            {
                conPrintf(e->client, "%s %s!", localizedPrintf(e,"NextRoundIn"), timerMakeOffsetStringFromSeconds(buf, event->next_round_time - time));
            }
        }
    }
}

void handleSendNotReadyMessage(Packet *pak)
{
    int eventid = pktGetBitsAuto(pak);
    char *playername = pktGetString(pak);
    int j;

    ArenaEvent *event = FindEventDetail(eventid, 0, NULL);

    if (event)
    {
        for (j = eaSize(&event->participants)-1; j >= 0; j--)
        {
            Entity *e = entFromDbId(event->participants[j]->dbid);
            if (event->participants[j] && !event->participants[j]->dropped && e)
            {
                conPrintf(e->client, "%s %s", playername, localizedPrintf(e, "PlayerNotReady"));
            }
        }
    }
}

void handleSendNextRoundMessage(Packet *pak)
{
    int eventid = pktGetBitsAuto(pak);
    int numseconds = pktGetBitsAuto(pak);
    int j;
    char buf[80];

    ArenaEvent *event = FindEventDetail(eventid, 0, NULL);

    for (j = eaSize(&event->participants)-1; j >= 0; j--)
    {
        Entity *e = entFromDbId(event->participants[j]->dbid);
        if (!event->participants[j]->dropped && e)
        {
            conPrintf(e->client, "%s %s!", localizedPrintf(e,"NextRoundIn"), timerMakeOffsetStringFromSeconds(buf, numseconds));
        }
    }
}

void handleReportKillAck(Packet *pak)
{
    ArenaRequestAllCurrentResults();
}

// handle any logical changes we may have to make in response
// to seeing an event
void EventUpdateProcessing(ArenaEvent* event)
{
    int i, n;
    U32 time = dbSecondsSince2000();

    // if we are negotiating a scheduled event, see if we need to 
    // get the player teleported to the arena
    if(event->scheduled && event->phase == ARENA_NEGOTIATING &&
        time < event->start_time - ARENA_SCHEDULED_TELEPORT_WAIT)
    {
        n = eaSize(&event->participants);
        for (i = 0; i < n; i++)
        {
            Entity* e = entFromDbId(event->participants[i]->dbid);
            if (e && e->client && e->client->ready && 
                !e->pl->door_anim && !e->dbcomm_state.in_map_xfer &&
                !PlayerInTaskForceMode(e))
            {
                //if(e->pl->arenaLobby)
                //{
                    if(!e->pl->poppedUpSchedEvent)
                    {
                        SendServerRunEventWindow(event, e);
                        e->pl->poppedUpSchedEvent = 1;
                    }
                //}
                //else
                //{
                    //ScheduledEventTeleportMsgSend(event, e);
                    //e->pl->arenaTeleportTimer = time + ARENA_SCHEDULED_TELEPORT_WAIT;
                    //e->pl->arenaTeleportEventId = event->eventid;
                    //e->pl->arenaTeleportUniqueId = event->uniqueid;
                //}
            }
        }
    }

    // if we are in the first minute of a round, check and see if there
    // are any participants we should be warping to that round
    if (event->phase == ARENA_RUNROUND
        && !OnArenaMap()
        && time < event->round_start + ARENA_ALLOW_ENTRY_TIMEOUT)
    {
        Entity* e;

        n = eaSize(&event->participants);

        if(event->entryfee)
        {
            int *dbids;
            eaiCreate(&dbids);

            for(i = eaSize(&event->participants)-1; i >= 0; i--)
            {
                e = entFromDbId(event->participants[i]->dbid);
                if(!event->participants[i]->paid && e && e->pl && e->pl->arena_paid != event->uniqueid)// && e->pl->arenaLobby)
                {
                    if(e->pchar->iInfluencePoints >= event->entryfee)
                    {
                        ent_AdjInfluence(e, -event->entryfee, NULL);
                        e->pl->arena_paid = event->uniqueid;
                        e->pl->arena_paid_amount = event->entryfee;
                        event->participants[i]->paid = 1;
                        eaiPush(&dbids, e->db_id);
                    }
                }
            }
            if(eaiSize(&dbids))
                ArenaFeePayment(dbids, event->eventid, event->uniqueid);

            eaiDestroy(&dbids);
        }

        for (i = 0; i < n; i++)
        {
            e = entFromDbId(event->participants[i]->dbid);
            if (e && e->client && e->client->ready &&
                !e->pl->door_anim && !e->dbcomm_state.in_map_xfer && !event->participants[i]->dropped)
            {
                // if the player hasn't made it to the lobby by now or they are in a taskforce, drop them
                if (//!e->pl->arenaLobby || 
                    (event->entryfee && !event->participants[i]->paid) || 
                    PlayerInTaskForceMode(e))
                {
                    ArenaDropPlayer(e->db_id, event->eventid, event->uniqueid);
                } else {
                    EnterArena(e, event);
                }
            }
        }
    }

    if (g_arenamap_eventid == event->eventid && g_arenamap_phase < ARENAMAP_FIGHT)
    {
        ArenaEventCopy(g_arenamap_event, event);
    }
}

void ProcessParticipantChanges(ArenaEvent* event)
{
    Entity* e;
    int i, active;

    // notice when an event goes active/inactive
    active = ACTIVE_EVENT(event);
    for (i = eaSize(&event->participants)-1; i >= 0; i--)
    {
        e = entFromDbIdEvenSleeping(event->participants[i]->dbid);
        if (e && e->pl)
        {
            if (active)
                ArenaRefCopy(&e->pl->arenaActiveEvent, event);
            else if (ArenaRefMatch(&e->pl->arenaActiveEvent, event))
            {
                ArenaRefZero(&e->pl->arenaActiveEvent);
                SendActiveEvent(e, NULL);
            }
        }
    }
}

void handleEventUpdate(Packet* pak)
{
    ArenaEvent* event;
    int deleted, i, n;
    U32 eventid;

    // signal that I was waiting for
    eventid = pktGetBitsPack(pak, 1);
    if (eventid == g_eventwaitfor)
        g_eventwaitfor = 0;

    deleted = pktGetBits(pak, 1);
    if (deleted)
    {
        if (FindEventDetail(eventid, 0, &i))
            ArenaEventDestroy(eaRemove(&g_detaillist, i));
        return;
    }
    event = FindEventDetail(eventid, 1, NULL);
    if(!event)
    {
        LOG_OLD_ERR("Got an invalid event update");
        return;
    }
    ArenaEventReceive(pak, event);
    FillParticipantNames(event);
    EventUpdateProcessing(event);

    ProcessParticipantChanges(event);

    // forward full event updates to any participants we control
    n = eaSize(&event->participants);
    for (i = 0; i < n; i++)
    {
        Entity* e = entFromDbIdEvenSleeping(event->participants[i]->dbid);
        if (e && ArenaRefMatch(&e->pl->arenaActiveEvent, event))
            SendActiveEvent(e, event);
    }
}

void handleRequestReady(Packet *pak)
{
    int dbid = pktGetBitsAuto(pak);

    Entity *player = entFromDbId(dbid);
    Packet *sendPak = pktCreateEx(&arena_comm_link, ARENACLIENT_READYACK);

    if (player && player->pchar)
    {
        int now = timerSecondsSince2000();
        int attackedTime = player->pchar->aulEventTimes[kPowerEvent_AttackedByOther];
        int amReady = (!attackedTime || now - attackedTime > ARENA_READY_TIMER_SINCE_LAST_ATTACKED);

        pktSendBitsAuto(sendPak, 1); // good to go
        pktSendBitsAuto(sendPak, amReady);
        pktSendBitsAuto(sendPak, player->db_id);
        pktSendString(sendPak, player->name);
    }
    else
    {
        pktSendBitsAuto(sendPak, 0); // bad data
    }

    pktSend(&sendPak, &arena_comm_link);
}

void handleUpdateParticipant(Packet* pak)
{
    Entity* player;
    int dbid, i, n;
    int activeEventFound = 0;

    dbid = pktGetBitsPack(pak, 1);
    player = entFromDbIdEvenSleeping(dbid);    // we get this refresh while player is connecting
    if (!player)
        return;

    n = pktGetBitsPack(pak, 1);
    while (eaSize(&player->pl->arenaEvents) < n)
        eaPush(&player->pl->arenaEvents, ArenaRefCreate());
    while (eaSize(&player->pl->arenaEvents) > n)
        ArenaRefDestroy(eaPop(&player->pl->arenaEvents));

    for (i = 0; i < n; i++)
    {
        player->pl->arenaEvents[i]->eventid = pktGetBitsPack(pak, 1);
        player->pl->arenaEvents[i]->uniqueid = pktGetBitsPack(pak, 1);

        if (ArenaRefMatch(player->pl->arenaEvents[i], &player->pl->arenaActiveEvent))
            activeEventFound = 1;
    }
    // I notice if my active event is dropped
    if (!activeEventFound)
    {
        if (player->pl->arenaActiveEvent.eventid)
        {
            SendActiveEvent(player, NULL);
        }
        ArenaRefZero(&player->pl->arenaActiveEvent);
    }

    // if the player is not in any events, clear teleport timers
    if (n == 0)
    {
        //player->pl->arenaTeleportTimer = 0;
        //player->pl->arenaTeleportEventId = 0;
        //player->pl->arenaTeleportUniqueId = 0;
    }
}

static char* arenaRatingClassificationNames[ARENA_NUM_RATINGS] = {
    "SoloDuel", "TeamDuel", "SupergroupRumble", "SwissDrawTournament", "SupergroupTournament", "BattleRoyale", "TeamBattleRoyale", "ArenaStar", "Gladiator", "TeamGladiator"
};

static void updateArenaBadgeStats(Entity *e, ArenaPlayer *ap)
{
    int i;
    char temp[100];

    badge_StatSet(e, "arena.wins", ap->wins);
    badge_StatSet(e, "arena.draws", ap->draws);
    badge_StatSet(e, "arena.losses", ap->losses);
    badge_StatSet(e, "arena.matches", ap->wins + ap->draws + ap->losses);

    badge_StatSet(e, "arena.wins.event.TotalStar", 0);
    badge_StatSet(e, "arena.draws.event.TotalStar", 0);
    badge_StatSet(e, "arena.losses.event.TotalStar", 0);
    badge_StatSet(e, "arena.matches.event.TotalStar", 0);
    // event types
    for (i = 0; i < ARENA_NUM_EVENT_TYPES_FOR_STATS; i++)
    {
        sprintf(temp, "arena.wins.event.%s", StaticDefineIntRevLookup(ParseArenaEventTypeForStats, i));
        badge_StatSet(e, temp, ap->winsByEventType[i]);
        sprintf(temp, "arena.draws.event.%s", StaticDefineIntRevLookup(ParseArenaEventTypeForStats, i));
        badge_StatSet(e, temp, ap->drawsByEventType[i]);
        sprintf(temp, "arena.losses.event.%s", StaticDefineIntRevLookup(ParseArenaEventTypeForStats, i));
        badge_StatSet(e, temp, ap->lossesByEventType[i]);
        sprintf(temp, "arena.matches.event.%s", StaticDefineIntRevLookup(ParseArenaEventTypeForStats, i));
        badge_StatSet(e, temp, ap->winsByEventType[i] + ap->drawsByEventType[i] + ap->lossesByEventType[i]);
        if (i >= 3 && i <= 8)
        {
            badge_StatAdd(e, "arena.wins.event.TotalStar", ap->winsByEventType[i]);
            badge_StatAdd(e, "arena.draws.event.TotalStar", ap->drawsByEventType[i]);
            badge_StatAdd(e, "arena.losses.event.TotalStar", ap->lossesByEventType[i]);
            badge_StatAdd(e, "arena.matches.event.TotalStar", ap->winsByEventType[i]);
            badge_StatAdd(e, "arena.matches.event.TotalStar", ap->drawsByEventType[i]);
            badge_StatAdd(e, "arena.matches.event.TotalStar", ap->lossesByEventType[i]);
        }
    }

    // team types
    for (i = 0; i < ARENA_NUM_TEAM_TYPES; i++)
    {
        sprintf(temp, "arena.wins.team.%s", StaticDefineIntRevLookup(ParseArenaTeamType, i));
        badge_StatSet(e, temp, ap->winsByTeamType[i]);
        sprintf(temp, "arena.draws.team.%s", StaticDefineIntRevLookup(ParseArenaTeamType, i));
        badge_StatSet(e, temp, ap->drawsByTeamType[i]);
        sprintf(temp, "arena.losses.team.%s", StaticDefineIntRevLookup(ParseArenaTeamType, i));
        badge_StatSet(e, temp, ap->lossesByTeamType[i]);
        sprintf(temp, "arena.matches.team.%s", StaticDefineIntRevLookup(ParseArenaTeamType, i));
        badge_StatSet(e, temp, ap->winsByTeamType[i] + ap->drawsByTeamType[i] + ap->lossesByTeamType[i]);
    }

    // victory types
    for (i = 0; i < ARENA_NUM_VICTORY_TYPES; i++)
    {
        sprintf(temp, "arena.wins.victory.%s", StaticDefineIntRevLookup(ParseArenaVictoryType, i));
        badge_StatSet(e, temp, ap->winsByVictoryType[i]);
        sprintf(temp, "arena.draws.victory.%s", StaticDefineIntRevLookup(ParseArenaVictoryType, i));
        badge_StatSet(e, temp, ap->drawsByVictoryType[i]);
        sprintf(temp, "arena.losses.victory.%s", StaticDefineIntRevLookup(ParseArenaVictoryType, i));
        badge_StatSet(e, temp, ap->lossesByVictoryType[i]);
        sprintf(temp, "arena.matches.victory.%s", StaticDefineIntRevLookup(ParseArenaVictoryType, i));
        badge_StatSet(e, temp, ap->winsByVictoryType[i] + ap->drawsByVictoryType[i] + ap->lossesByVictoryType[i]);
    }

    // weight classes
    for (i = 0; i < ARENA_NUM_WEIGHT_CLASSES; i++)
    {
        sprintf(temp, "arena.wins.weight.%s", StaticDefineIntRevLookup(ParseArenaWeightClass, i));
        badge_StatSet(e, temp, ap->winsByWeightClass[i]);
        sprintf(temp, "arena.draws.weight.%s", StaticDefineIntRevLookup(ParseArenaWeightClass, i));
        badge_StatSet(e, temp, ap->drawsByWeightClass[i]);
        sprintf(temp, "arena.losses.weight.%s", StaticDefineIntRevLookup(ParseArenaWeightClass, i));
        badge_StatSet(e, temp, ap->lossesByWeightClass[i]);
        sprintf(temp, "arena.matches.weight.%s", StaticDefineIntRevLookup(ParseArenaWeightClass, i));
        badge_StatSet(e, temp, ap->winsByWeightClass[i] + ap->drawsByWeightClass[i] + ap->lossesByWeightClass[i]);
    }

    // rating classifications
    badge_StatSet(e, "arena.wins.rating", 0);
    badge_StatSet(e, "arena.draws.rating", 0);
    badge_StatSet(e, "arena.losses.rating", 0);
    badge_StatSet(e, "arena.matches.rating", 0);
    for (i = 0; i < ARENA_NUM_RATINGS; i++)
    {
        sprintf(temp, "arena.wins.rating.%s", arenaRatingClassificationNames[i]);
        badge_StatSet(e, temp, ap->winsByRatingIndex[i]);
        badge_StatAdd(e, "arena.wins.rating", ap->winsByRatingIndex[i]);
        badge_StatAdd(e, "arena.matches.rating", ap->winsByRatingIndex[i]);
        sprintf(temp, "arena.draws.rating.%s", arenaRatingClassificationNames[i]);
        badge_StatSet(e, temp, ap->drawsByRatingIndex[i]);
        badge_StatAdd(e, "arena.draws.rating", ap->drawsByRatingIndex[i]);
        badge_StatAdd(e, "arena.matches.rating", ap->drawsByRatingIndex[i]);
        sprintf(temp, "arena.losses.rating.%s", arenaRatingClassificationNames[i]);
        badge_StatSet(e, temp, ap->lossesByRatingIndex[i]);        
        badge_StatAdd(e, "arena.losses.rating", ap->lossesByRatingIndex[i]);
        badge_StatAdd(e, "arena.matches.rating", ap->lossesByRatingIndex[i]);
        sprintf(temp, "arena.matches.rating.%s", arenaRatingClassificationNames[i]);
        badge_StatSet(e, temp, ap->winsByRatingIndex[i] + ap->drawsByRatingIndex[i] + ap->lossesByRatingIndex[i]);        
    }
}

// Requester 0-2 get passed on to the client.
void handleUpdatePlayer(Packet* pak)
{
    ArenaPlayer* ap;
    int dbid;
    Entity * sendto, *ent;
    int requester;

    // TODO - what to do with this?
    //relay information to client
    ap = ArenaPlayerCreate();
    dbid = pktGetBitsPack(pak, PKT_BITS_TO_REP_DB_ID);
    requester=pktGetBitsPack(pak, 1);
    ArenaPlayerReceive(pak, ap);

    sendto=entFromDbId(dbid);
    if (sendto)
    {
        START_PACKET(pkt,sendto,SERVER_ARENA_UPDATE_PLAYER);
        pktSendBitsPack(pkt,1,requester);
        ArenaPlayerSend(pkt, ap);
        END_PACKET

        ent=entFromDbId(ap->dbid);

        if (ent)
        {
            if (ap->prizemoney || ap->arenaReward[0])
            {
                ent_AdjInfluence(ent, ap->prizemoney, NULL);
                if( ap->arenaReward[0] )
                {
                    rewardFindDefAndApplyToEnt(ent, (const char**)eaFromPointerUnsafe(ap->arenaReward), VG_NONE, character_GetExperienceLevelExtern(ent->pchar), false, REWARDSOURCE_ARENA, NULL);
                    //conPrintf(ent->client, "%s", localizedPrintf(ent,"ArenaServerBadgeEarned", ap->prizemoney));
                }
                conPrintf(ent->client, "%s", localizedPrintf(ent,"ArenaServerRewardedYou", ap->prizemoney));
                ArenaConfirmReward(ent->db_id);
            }

            updateArenaBadgeStats(ent, ap);
        }
    }

    ArenaPlayerDestroy(ap);
}

void handleClientRequestResults(Packet* pak)
{
    ArenaRankingTable *art = ArenaRankingTableCreate();
    Entity * ent;
    int dbid;
    int i;

    dbid=pktGetBitsPack(pak,PKT_BITS_TO_REP_DB_ID);
    ent=entFromDbId(dbid);
    ArenaRankingTableReceive(pak,art);
    if (ent==NULL) return;

    // if we are not running with arena server
    if (!ArenaSyncConnect())
    {
        ArenaSendKioskFail(dbid);
        return;
    }

    for (i=0;i<eaSize(&art->entries);i++) {
        art->entries[i]->playername=strdup(dbPlayerNameFromId(art->entries[i]->dbid));
    }

    START_PACKET(pkt,ent,SERVER_ARENA_REQRESULTS);
        ArenaRankingTableSend(pkt,art);
    END_PACKET

    ArenaRankingTableDestroy(art);
}

void handleUpdateLeaderList(Packet* pak)
{
    int dbid = pktGetBitsPack(pak, PKT_BITS_TO_REP_DB_ID);
    Entity* e = entFromDbId(dbid);

    if(!e)
        return;

    ArenaLeaderBoardReceive(pak, g_mapserver_leaderboard);

    kiosk_Navigate(e, "arena", "0");
}

void handlePlayerStats(Packet* pak)
{
    int dbid = pktGetBitsPack(pak, PKT_BITS_TO_REP_DB_ID);
    Entity* e = entFromDbId(dbid);
    char* str = pktGetString(pak);

    if (e)
    {
        printf("%d:%s\n", dbid, str);
    }
}

// let the client know about the updated active event
void SendActiveEvent(Entity* e, ArenaEvent* event)
{
    if (e && e->client && e->client->ready)
    {
        START_PACKET(pak, e, SERVER_ARENA_EVENTUPDATE)
        if (event)
        {
            pktSendBits(pak, 1, 1);
            pktSendBitsPack(pak, 1, event->eventid);
            pktSendBitsPack(pak, 1, event->uniqueid);
            ArenaEventSend(pak, event, 1);
        }
        else
            pktSendBits(pak, 1, 0);
        END_PACKET
    }
}

void ArenaClearSupergroupRatings(Entity* e)
{
    if(e && e->db_id)
    {
        Packet* pak;

        pak = pktCreateEx(&arena_comm_link, ARENACLIENT_CLEARPLAYERSGRATINGS);
        pktSendBitsPack(pak, PKT_BITS_TO_REP_DB_ID, e->db_id);
        pktSend(&pak, &arena_comm_link);
    }
}

// *********************************************************************************
//  Game commands
// *********************************************************************************

//send kiosk request off to the arena server, then back for filtering, then to the client
void ArenaSendKioskInfo(Entity * e, int radioButtonFilter, int tabFilter)
{
    Packet * pak;

    // if we are not running with arena server
    if (!ArenaSyncConnect())
    {
        ArenaSendKioskFail(e->db_id);
        return;
    }

    pak = pktCreateEx(&arena_comm_link, ARENACLIENT_REQKIOSK);
    pktSendBitsPack(pak,PKT_BITS_TO_REP_DB_ID,e->db_id);
    pktSendString(pak,e->pchar->pclass->pchName);
    pktSendBitsPack(pak,1,character_CalcExperienceLevel(e->pchar));
    pktSendBitsPack(pak,1,radioButtonFilter);
    pktSendBitsPack(pak,1,tabFilter);
    pktSend(&pak, &arena_comm_link);

}


void EventIgnoreFilter(ArenaEvent * event,Entity * e)
{
    Entity * creator;
    if (event==NULL)
        return;

    creator=entFromDbId(event->creatorid);

    //filter also if ignored or ignoring
    if( isIgnored(e,event->creatorid) )
        event->kiosksend=0;

    if (creator!=NULL)
    {
        if( isIgnored(creator, e->db_id) )
            event->kiosksend=0;
    }
}

void EventSupergroupFilter(ArenaEvent * event,Entity * e) {
    if (event==NULL)
        return;
    if (event->teamType!=ARENA_SUPERGROUP)
        return;
    if (e->supergroup==0)
        event->kiosksend=0;
    if (event->creatorsg && event->othersg && e->supergroup_id!=event->creatorsg && e->supergroup_id!=event->othersg)
        event->kiosksend=0;
}


void ArenaSendKioskFail(int dbid)
{
    Entity* e = entFromDbId(dbid);
    if (!e || !e->client || !e->client->link || !e->client->link->connected)
        return;
    START_PACKET(pak, e, SERVER_ARENA_KIOSK);
    pktSendBits(pak, 1, 0);                        // failed
    END_PACKET
}

void ArenaRequestPlayerStats(int dbid)
{
    Packet* pak;

    if (ArenaSyncConnect())
    {
        pak = pktCreateEx(&arena_comm_link, ARENACLIENT_REQPLAYERSTATS);
        pktSendBitsPack(pak,PKT_BITS_TO_REP_DB_ID,dbid);
        pktSend(&pak, &arena_comm_link);
    }
    else
    {
        ArenaSendKioskFail(dbid);
    }
}

// send detailed info for each event the player belongs to
void ArenaSendDebugInfo(int dbid)
{
    int i, n, count;
    Entity* e;

    // if we are not running with arena server
    if (!ArenaSyncConnect())
    {
        ArenaSendKioskFail(dbid);
        return;
    }

    e = entFromDbId(dbid);
    if (!e || !e->client || !e->client->link || !e->client->link->connected)
        return;

    // find out how many we actually have full info on first..
    n = eaSize(&e->pl->arenaEvents);
    for (i = 0, count = 0; i < n; i++)
    {
        U32 eventid = e->pl->arenaEvents[i]->eventid;
        U32 uniqueid = e->pl->arenaEvents[i]->uniqueid;
        ArenaEvent* event = FindEventDetail(eventid, 0, NULL);
        if (event && event->uniqueid == uniqueid)
            count++;
    }

    // now batch and send
    START_PACKET(pak, e, SERVER_ARENA_EVENTS);
    pktSendBitsPack(pak, 1, count);
    for (i = 0, count = 0; i < n; i++)
    {
        U32 eventid = e->pl->arenaEvents[i]->eventid;
        U32 uniqueid = e->pl->arenaEvents[i]->uniqueid;
        ArenaEvent* event = FindEventDetail(eventid, 0, NULL);
        if (event && event->uniqueid == uniqueid)
            ArenaEventSend(pak, event, 1);
    }
    END_PACKET
}

void ArenaReportKill(ArenaEvent *event, int partid)
{
    Packet *pak = pktCreateEx(&arena_comm_link, ARENACLIENT_REPORTKILL);
    pktSendBitsAuto(pak, event->eventid);
    pktSendBitsAuto(pak, event->uniqueid);
    pktSendBitsAuto(pak, partid);
    pktSend(&pak, &arena_comm_link);
}

int ArenaReportMapResults(ArenaEvent* event, int seat, int side, U32 time)
{
    ArenaServerResult result;
    int i, n, report;
    Packet* pak;

    // I'm willing to try and report my results 5 times
    for (report = 0; report < 5; report++)
    {
        if (!ArenaSyncConnect())
            continue;

        pak = pktCreateEx(&arena_comm_link, ARENACLIENT_MAPRESULTS);
        pktSendBitsPack(pak, 1, db_state.map_id);
        pktSendBitsPack(pak, 1, event->eventid);
        pktSendBitsPack(pak, 1, event->uniqueid);
        pktSendBitsPack(pak, 1, seat);
        pktSendBitsPack(pak, 1, side);
        pktSendBitsPack(pak, 1, time);
        n = eaSize(&event->participants);
        pktSendBitsPack(pak, 1, n);
        for(i = 0; i < n; i++)
        {
            pktSendBitsPack(pak, PKT_BITS_TO_REP_DB_ID, event->participants[i]->dbid);
            pktSendBitsPack(pak, 1, event->participants[i]->kills[event->round]);
            pktSendBitsPack(pak, 1, event->participants[i]->death_time[event->round]);
        }
        pktSend(&pak, &arena_comm_link);
        result = ArenaSyncResult();
        if (result == ARENA_SUCCESS) break;
    }
    return result == ARENA_SUCCESS;
}

void ArenaRequestJoin(Entity* e, int eventid, int uniqueid, int invite, int ignore_active )
{
    Packet *pak;

    if (PlayerInTaskForceMode(e))
    {
        conPrintf(e->client, localizedPrintf(e,"NoJoinEventInTF"));
        return;
    }
    if(e->taskforce)
    {
        conPrintf(e->client, localizedPrintf(e,"NoArenaWithTaskforce"));
        return;
    }

      if (!ArenaSyncConnect())
    {
        conPrintf(e->client, localizedPrintf(e,"NoArenaServer"));
        return;
    } 

    pak = pktCreateEx(&arena_comm_link, ARENACLIENT_REQJOINEVENT);
    pktSendBitsPack(pak, 1, e->db_id);
    if(invite)
        pktSendBitsPack(pak, 1, CLIENTINP_ARENA_REQ_JOIN_INVITE);
    else
        pktSendBitsPack(pak, 1, CLIENTINP_ARENA_REQ_JOIN);
    pktSendString( pak, e->name );
    pktSendBitsPack(pak, 1, e->pchar->iInfluencePoints);
    pktSendBits(pak, 1, ignore_active);
    pktSendBitsPack(pak, 1, eventid);
    pktSendBitsPack(pak, 1, uniqueid);
    pktSendString(pak, e->pchar->pclass->pchName);
    pktSendBitsPack(pak, 1, character_CalcExperienceLevel(e->pchar));
    pktSendBitsPack(pak, 1, e->supergroup_id);
    pktSendBits(pak, 1, sgroup_hasPermission(e, SG_PERM_ARENA));
    pktSend(&pak, &arena_comm_link);

    e->pl->poppedUpSchedEvent = 0;
}

void ArenaDropPlayer(int dbid, int eventid, int uniqueid)
{
    Packet* pak;

    if (!ArenaAsyncConnect())
        return;
    pak = pktCreateEx(&arena_comm_link, ARENACLIENT_REQDROPEVENT);
    pktSendBitsPack(pak, 1, dbid);
     pktSendBitsPack(pak, 1, CLIENTINP_ARENA_DROP);
    pktSendString( pak, dbPlayerNameFromId(dbid) );
    pktSendBitsPack(pak, 1, eventid);
    pktSendBitsPack(pak, 1, uniqueid);
    pktSend(&pak, &arena_comm_link);

    ArenaRequestUpdatePlayer(dbid, dbid, -1); // To get it to call updateArenaBadgeStats
}

void ArenaDrop( Entity *e, int eventid, int uniqueid, int kicked_dbid )
{
    if( kicked_dbid != e->db_id ) // you can drop yourself
    {
        //verify can kick here
        ArenaEvent* event = FindEventDetail(eventid, 0, NULL);
        if(event->creatorid != e->db_id)
        {
            chatSendToPlayer( e->db_id, localizedPrintf(e,"OnlyCreatorCanKick"), INFO_ARENA_ERROR, 0 );
            return;
        }
    }

    e->pl->poppedUpSchedEvent = 0;
    ArenaDropPlayer( kicked_dbid, eventid, uniqueid );
}

void ArenaRequestUpdatePlayer(int dbidSource,int dbidTarget,int tabWindowRequest) {
    Packet * pak;

    pak = pktCreateEx(&arena_comm_link, ARENACLIENT_REQPLAYERUPDATE);
    pktSendBitsPack(pak, PKT_BITS_TO_REP_DB_ID,dbidSource);    //source player
    pktSendBitsPack(pak, PKT_BITS_TO_REP_DB_ID,dbidTarget);    //target player
    pktSendBitsPack(pak,1,tabWindowRequest);    //0 indicates that it is for the info tab window thingy
    pktSend(&pak,&arena_comm_link);
}

int ArenaCreateEvent(Entity* e, char* eventname)
{
    //ArenaServerResult result;
    Packet* pak;

    if (!ArenaSyncConnect())
    {
        conPrintf(e->client, localizedPrintf(e,"NoArenaServer"));
        return 0;
    }
    if (!eventname || !eventname[0])
    {
        conPrintf(e->client, "No arena name specified");
        return 0;
    }
    if (PlayerInTaskForceMode(e))
    {
        conPrintf(e->client, localizedPrintf(e,"NoCreateEventInTF"));
        return 0;
    }

    pak = pktCreateEx(&arena_comm_link, ARENACLIENT_CREATEEVENT);
    pktSendBitsPack(pak, 1, e->db_id);
    pktSendBitsPack(pak, 1, CLIENTINP_ARENA_REQ_CREATE);
    pktSendString(pak, eventname);
    pktSendString(pak, e->name);
    pktSendString(pak, e->pchar->pclass->pchName);
    pktSendBitsPack(pak, 1, character_CalcExperienceLevel(e->pchar));
    pktSendBitsPack(pak, 1, e->supergroup_id);
    pktSendBits(pak, 1, sgroup_hasPermission(e, SG_PERM_ARENA));

    pktSend(&pak, &arena_comm_link);

    return 1; // stopping the synchronous thing..
}

int ArenaDestroyEvent(Entity* e, int eventid, int uniqueid)
{
    ArenaServerResult result;
    Packet* pak;

    if(e->access_level < 3)
    {
        LOG_OLD_ERR("Person without access level trying to destroy event");
        return 0;
    }

    if (!ArenaSyncConnect())
    {
        conPrintf(e->client, localizedPrintf(e,"NoArenaServer"));
        return 0;
    }

    pak = pktCreateEx(&arena_comm_link, ARENACLIENT_DESTROYEVENT);
    pktSendBitsPack(pak, 1, e->db_id);
    pktSendBitsPack(pak, 1, CLIENTINP_ARENA_REQ_DESTROY);
    pktSendBitsPack(pak, 1, eventid);
    pktSendBitsPack(pak, 1, uniqueid);
    pktSend(&pak, &arena_comm_link);
    result = ArenaSyncResult();
    return result == ARENA_SUCCESS;
}

// synchronously request an event's information
void ArenaSyncRequestEvent(int eventid)
{
    ArenaServerResult result;
    Packet *pak;
    int i = 0;

    if (!ArenaSyncConnect())
        return;

    pak = pktCreateEx(&arena_comm_link, ARENACLIENT_REQEVENT);
    pktSendBitsPack(pak, 1, eventid);
    pktSend(&pak, &arena_comm_link);
    result = ArenaSyncResult();
    if (result == ARENA_SUCCESS)
    {
        while (arena_comm_link.connected && i++ < 50)
        {
            netLinkMonitorBlock(&arena_comm_link, 0, ArenaClientCallback, 10);
        }
    }
    else
    {
        LOG_OLD_ERR( __FUNCTION__ " didn't get a successful result");
    }
}

// syncronously update a single event
void ArenaSyncUpdateEvent(ArenaEvent* event)
{
    ArenaServerResult result;
    Packet* pak;
    int i = 0;

    if (!ArenaSyncConnect())
        return;

    event->uniqueid = 0;    // if successful, this will be set
    pak = pktCreateEx(&arena_comm_link, ARENACLIENT_REQEVENT);
    pktSendBitsPack(pak, 1, event->eventid);
    pktSend(&pak, &arena_comm_link);
    result = ArenaSyncResult();
    if (result == ARENA_SUCCESS)
    {
        g_eventwaitfor = event->eventid;
        while (arena_comm_link.connected && g_eventwaitfor && i++ < 50) // wait up to half a second
        {
            netLinkMonitorBlock(&arena_comm_link, 0, ArenaClientCallback, 10);
        }
    }
    else
    {
        LOG_OLD_ERR( __FUNCTION__ " didn't get a successful result");
    }
}

int ArenaSetSide(Entity* e, int eventid, int uniqueid, int side)
{
    ArenaServerResult result;
    Packet* pak;

    if (!ArenaSyncConnect())
    {
        conPrintf(e->client, localizedPrintf(e,"NoArenaServer"));
        return 0;
    }

    pak = pktCreateEx(&arena_comm_link, ARENACLIENT_SETSIDE);
    pktSendBitsPack(pak, 1, e->db_id);
    pktSendBitsPack(pak, 1, CLIENTINP_ARENA_REQ_SIDE);
    pktSendBitsPack(pak, 1, eventid);
    pktSendBitsPack(pak, 1, uniqueid);
    pktSendBitsPack(pak, 1, side);
    pktSend(&pak, &arena_comm_link);
    result = ArenaSyncResult();
    return result == ARENA_SUCCESS;
}

void ArenaSetInsideLobby(Entity* e, int inside)
{
    if (!e || !e->pl) 
        return;

    e->pl->arenaLobby = inside? 1: 0;
}

int ArenaSetMap(Entity* e, int eventid, int uniqueid, char* newmap)
{
    ArenaServerResult result;
    Packet* pak;

    if (!ArenaSyncConnect())
    {
        conPrintf(e->client, localizedPrintf(e,"NoArenaServer"));
        return 0;
    }

    // todo, check this against list of maps..

    pak = pktCreateEx(&arena_comm_link, ARENACLIENT_SETMAP);
    pktSendBitsPack(pak, 1, e->db_id);
    pktSendBitsPack(pak, 1, CLIENTINP_ARENA_REQ_SETMAP);
    pktSendBitsPack(pak, 1, eventid);
    pktSendBitsPack(pak, 1, uniqueid);
    pktSendString(pak, newmap);
    pktSend(&pak, &arena_comm_link);
    result = ArenaSyncResult();
    return result == ARENA_SUCCESS;
}

int ArenaEnter(Entity* e, int eventid, int uniqueid)
{
    ArenaEvent* event = FindEventDetail(eventid, 0, NULL);
    if (!event || event->uniqueid != uniqueid)
    {
        conPrintf(e->client, "Don't have that event detail");
        return 0;
    }

    if (!EnterArena(e, event))
    {
        conPrintf(e->client, "Couldn't enter match, check to see it is started and you are a member");
        return 0;
    }
    return 1;
}

void ArenaPlayerLeavingMap(Entity* e, int logout)
{
    ArenaRegisterPlayer(e, 0, logout);
    if(OnArenaMap())
    {
        ArenaEvent* event = GetMyEvent();
        ArenaParticipant* part = ArenaParticipantFind(event, e->db_id);

        if(g_arenamap_phase < ARENAMAP_FINISH)
        {
            if(part)
            {
                part->death_time[g_arenamap_event->round] = ABS_TIME - g_matchstarttime;
                part->dropped = 1;
            }
            ArenaDropPlayer(e->db_id, event->eventid, event->uniqueid);
        }
    }
}

void ArenaRegisterPlayer(Entity* e, int add, int logout)
{
    if (ArenaAsyncConnect())
    {
        Packet* pak = pktCreateEx(&arena_comm_link, ARENACLIENT_REGISTERPLAYERS);
        pktSendBits(pak, 1, 0);    // fullupdate
        pktSendBits(pak, 1, add);    // add
        pktSendBits(pak, 1, logout); // logout
        pktSendBitsPack(pak, 1, 1); // count
        pktSendBitsPack(pak, 1, e->db_id); // dbid
        pktSend(&pak, &arena_comm_link);
    }
    // if not connected, we'll do full update on next connection anyway
}

void ArenaRegisterAllPlayers(void)
{
    if (ArenaAsyncConnect())
    {
        int i;
        PlayerEnts* owned = &player_ents[PLAYER_ENTS_OWNED];
        Packet* pak = pktCreateEx(&arena_comm_link, ARENACLIENT_REGISTERPLAYERS);
        pktSendBits(pak, 1, 1);        // fullupdate
        pktSendBits(pak, 1, 1);        // add
        pktSendBits(pak, 1, 0);        // logout
        pktSendBitsPack(pak, 1, owned->count);    // count
        for (i = 0; i < owned->count; i++)
        {
            pktSendBitsPack(pak, 1, owned->ents[i]->db_id);
        }
        pktSend(&pak, &arena_comm_link);
    }
}

// supergroup, sg_rank, or level change - let the arenaserver know
// the updated info
void ArenaPlayerAttributeUpdate(Entity* e)
{
    if (ArenaAsyncConnect() && e->pl->arenaEvents)
    {
        Packet* pak = pktCreateEx(&arena_comm_link, ARENACLIENT_PLAYERATTRIBUTEUPDATE);
        pktSendBitsPack(pak, PKT_BITS_TO_REP_DB_ID, e->db_id);
        pktSendBitsPack(pak, 1, character_CalcExperienceLevel(e->pchar));
        pktSendBitsPack(pak, 1, e->supergroup_id);
        pktSendBits(pak, 1, sgroup_hasPermission(e, SG_PERM_ARENA));
        pktSend(&pak, &arena_comm_link);
    }
}

void ArenaRequestCreate(Entity* e)
{
    // Kiosk check
    if( !e->pl->arenaKioskOpen )
    {
        chatSendToPlayer( e->db_id, localizedPrintf(e,"NotNearKioskCreate"),INFO_ARENA_ERROR, 0);
        return;
    }        

    // Already in an event.
    if( 0 && e->pl->arenaActiveEvent.eventid )
    {
        chatSendToPlayer( e->db_id, localizedPrintf(e,"InEventAlready"),INFO_ARENA_ERROR, 0);
        return;
    }

    ArenaCreateEvent(e, e->name);
}

void ArenaRequestResults(Entity* e, int eventid, int uniqueid)
{
    //validation here? I can't think of anything to check
    Packet* pak;

    pak = pktCreateEx(&arena_comm_link, ARENACLIENT_REQRESULTS);
    pktSendBitsPack(pak, PKT_BITS_TO_REP_DB_ID, e->db_id);
    pktSendBitsPack(pak, 6, eventid);
    pktSendBitsPack(pak, 6, uniqueid);
    pktSend(&pak, &arena_comm_link);
}

void ArenaRequestCurrentResults(Entity *e)
{
    if (!e || !e->pl || !g_arenamap_event)
        return;

    ArenaRequestResults(e, g_arenamap_event->eventid, g_arenamap_event->uniqueid);
}

void ArenaRequestAllCurrentResults()
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int playerIndex;

    for (playerIndex = 0; playerIndex < players->count; playerIndex++)
    {
        ArenaRequestCurrentResults(players->ents[playerIndex]);
    }
}

void ArenaNotifyFinish(Entity *e)
{
    START_PACKET(pkt,e,SERVER_ARENA_NOTIFY_FINISH);
    END_PACKET
}

void ArenaRequestObserve(Entity* e, int eventid, int uniqueid)
{
    ArenaEvent* event = FindEventDetail(eventid, 0, NULL);

    if (!e->client || !e->client->ready || e->pl->door_anim || e->dbcomm_state.in_map_xfer)
        return; // just fail silently

    if (!event)
    {
        ArenaSyncRequestEvent(eventid);
        event = FindEventDetail(eventid, 0, NULL);
    }

    if (event && event->uniqueid == uniqueid)
    {
        if (event->phase < ARENA_STARTROUND || event->phase > ARENA_ENDROUND)
            chatSendToPlayer(e->db_id, localizedPrintf(e,"EventIsNotRunning"), INFO_ARENA_ERROR, 0);
        else
            EnterArena(e, event);
    }
    else
    {
        conPrintf(e->client, "Don't have that event detail");
        chatSendToPlayer(e->db_id, localizedPrintf(e,"EventNoExist"), INFO_ARENA_ERROR, 0);
    }
}

void ArenaReceiveCreatorUpdate( Entity * e, Packet * pak )
{
    ArenaEvent tmp_event;
    ArenaEvent *event;

    ArenaCreatorUpdateRecieve( &tmp_event, pak );


    event = FindEventDetail( tmp_event.eventid, 0, NULL );

    if( event )
    {
        if(tmp_event.teamType == ARENA_SUPERGROUP && !sgroup_hasPermission(e, SG_PERM_ARENA))
        {
            chatSendToPlayer(e->db_id, localizedPrintf(e, "NoSgArenaPermission"), INFO_ARENA_ERROR, 0);
            SendActiveEvent(e, event);
            return;
        }

        if(ArenaSyncConnect())
        {
            Packet* pkt = pktCreateEx(&arena_comm_link, ARENACLIENT_CREATORUPDATE);

            pktSendBitsPack(pkt, 1, e->db_id);
            pktSendBitsPack(pkt, 1, CLIENTINP_ARENA_CREATOR_UPDATE);

            ArenaCreatorUpdateSend( &tmp_event, pkt );

            pktSend(&pkt, &arena_comm_link);
        }
    }
    else
        chatSendToPlayer( e->db_id, localizedPrintf(e, "NoEventUpdate"), INFO_ARENA_ERROR, 0);
}

void ArenaReceivePlayerUpdate( Entity * e, Packet * pak )
{
    ArenaEvent tmp_event;
    ArenaEvent *event;
    ArenaParticipant tmp_ap;

    ArenaParticipantUpdateRecieve( &tmp_event, &tmp_ap, pak );

    event = FindEventDetail( tmp_event.eventid, 0, NULL );

    if( event )
    {
        Packet* pkt = pktCreateEx(&arena_comm_link, ARENACLIENT_PLAYERUPDATE);

        pktSendBitsPack(pkt, 1, e->db_id);
        pktSendBitsPack(pkt, 1, CLIENTINP_ARENA_PLAYER_UPDATE);
        ArenaParticipantUpdateSend( &tmp_event, &tmp_ap, pkt );

        pktSend(&pkt, &arena_comm_link);
    }
    else
        chatSendToPlayer( e->db_id, "NoEventUpdate", INFO_ARENA_ERROR, 0);
}

void ArenaReceiveFullPlayerUpdate( Entity * e, Packet * pak )
{
    ArenaEvent tmp_event = {0};
    ArenaEvent *event;

    ArenaFullParticipantUpdateRecieve( &tmp_event, pak );

    event = FindEventDetail( tmp_event.eventid, 0, NULL );

    if( event )
    {
        Packet* pkt = pktCreateEx(&arena_comm_link, ARENACLIENT_FULLPLAYERUPDATE);

        pktSendBitsPack(pkt, 1, e->db_id);
        pktSendBitsPack(pkt, 1, CLIENTINP_ARENA_FULL_PLAYER_UPDATE);
        ArenaFullParticipantUpdateSend( &tmp_event, pkt );

        pktSend(&pkt, &arena_comm_link);
    }
}


void ArenaFeePayment(int* dbids, int eventid, int uniqueid)
{
    Packet* pak;
    int i, n = eaiSize(&dbids);

    if(!n)
        return;

    if(!ArenaSyncConnect())
        return;

    pak = pktCreateEx(&arena_comm_link, ARENACLIENT_FEEPAYMENT);
    pktSendBitsPack(pak, 1, eventid);
    pktSendBitsPack(pak, 1, uniqueid);
    pktSendBitsPack(pak, 1, n);
    for(i = n-1; i >= 0; i--)
    {
        pktSendBitsPack(pak, PKT_BITS_TO_REP_DB_ID, dbids[i]);
    }
    pktSend(&pak, &arena_comm_link);
}

void ArenaConfirmReward(int dbid)
{
    Packet* pak;

    pak = pktCreateEx(&arena_comm_link, ARENACLIENT_CONFIRM_REWARD);
    pktSendBitsPack(pak, PKT_BITS_TO_REP_DB_ID, dbid);
    pktSend(&pak, &arena_comm_link);
}

void ArenaStartClientCountdown(Entity *e, char *str, int count)
{
    serveFloater(e, e, str, 0.0, kFloaterStyle_Attention, 0);
    START_PACKET(pak, e, SERVER_ARENA_START_COUNT);
    pktSendBitsPack(pak, 1, count);
    END_PACKET;
}

void ArenaSetClientCompassString(Entity* e, char* str, U32 timeUntilMatchEnds, U32 matchLength, U32 respawnPeriod)
{
    START_PACKET(pak, e, SERVER_ARENA_COMPASS_STRING);
    pktSendString(pak, str);
    pktSendBitsAuto(pak, timeUntilMatchEnds);
    pktSendBitsAuto(pak, matchLength);
    pktSendBitsAuto(pak, respawnPeriod);
    END_PACKET;
}

void ArenaSetAllClientsCompassString(ArenaEvent* event, char* str, int timerUntilMatchEnds)
{
    int i;
    Entity* e;

    for(i = eaSize(&event->participants)-1; i >= 0; i--)
    {
        if(event->participants[i]->dbid && (e = entFromDbId(event->participants[i]->dbid)))
        {
            ArenaSetClientCompassString(e, str, timerUntilMatchEnds, 0, 0);
        }
    }
}

void ArenaSendVictoryInformation(Entity* e, int numlives, int numkills, int infinite_resurrect)
{
    START_PACKET(pak, e, SERVER_ARENA_VICTORY_INFORMATION);
    pktSendBitsPack(pak, 1, numlives);
    pktSendBitsPack(pak, 1, numkills);
    pktSendBitsPack(pak, 1, infinite_resurrect);
    END_PACKET;
}

void ArenaRequestLeaderBoard(Entity* e)
{
    if(ArenaAsyncConnect())
    {
        Packet* pak;

        if(!e || !e->db_id)
            return;

        pak = pktCreateEx(&arena_comm_link, ARENACLIENT_REQLEADERS);
        pktSendBitsPack(pak, PKT_BITS_TO_REP_DB_ID, e->db_id);
        pktSend(&pak, &arena_comm_link);
    }
}

void DumpArenaLeaders(Entity *e, StuffBuff* psb, int iNumPlaces)
{
    int i, j;
    int iPlace;
    int iLast = -1;
    int iCnt = 0;

    iCnt = eaSize(&g_mapserver_leaderboard[0].entries);

    if(iCnt<1)
    {
        addStringToStuffBuff(psb, "<span align=center><scale .75>%s</scale></span><br>\n", localizedPrintf(e,"KioskNoInfo"));
        return;
    }

    for(j = 0; j < ARENA_NUM_RATINGS; j++)
    {
        iLast = -1;
        addStringToStuffBuff(psb, "<span align=center><scale 1.5>%s</scale></span>\n", localizedPrintf(e,arenaRatingClassificationNames[j]));
        addStringToStuffBuff(psb, "<scale 1.0><table><tr><scale .80><td width=30></td>");
        addStringToStuffBuff(psb, "<td align=right width=1>%s</td>",
            localizedPrintf(e,"RankString"));
        addStringToStuffBuff(psb, "<td align=right width=1>%s</td>",
            localizedPrintf(e,"RatingString"));
        addStringToStuffBuff(psb, "<td width=55><bsp>%s</td></scale></tr>\n",
            localizedPrintf(e,"HeroString"));

        iNumPlaces--;

        for(i=0,iPlace=0; i<iCnt && iPlace<iNumPlaces; i++)
        {
            addStringToStuffBuff(psb, "<tr><td></td>");

            if(iLast != g_mapserver_leaderboard[j].entries[i]->rating)
            {
                iPlace = i;
                addStringToStuffBuff(psb, "<td align=right>%8d&nbsp;</td>", iPlace+1);
            }
            else
            {
                addStringToStuffBuff(psb, "<td></td>");
            }
            iLast = g_mapserver_leaderboard[j].entries[i]->rating;

            addStringToStuffBuff(psb, "<td align=right>&nbsp;&nbsp;%.0f</td><td><bsp>%s</td>",
                g_mapserver_leaderboard[j].entries[i]->rating,
                dbPlayerNameFromId(g_mapserver_leaderboard[j].entries[i]->dbid));

            addStringToStuffBuff(psb, "</tr>\n");
        }
        addStringToStuffBuff(psb, "</table></scale><br>\n");
    }
}

// *********************************************************************************
//  Active event and kiosk tracking
// *********************************************************************************

// called when player leaves area of kiosk
void ArenaPlayerLeftKiosk(Entity* e)        
{
    // TODO..
}


