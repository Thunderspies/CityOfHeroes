/*\
 *
 *    ArenaServer.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Single shard-wide server to keep track of global list of 
 *  arena events.  Uses db to serialize out this list to sql.
 *
 */

#include <utilitieslib/network/netio.h>
#include "dbserver/servercfg.h"
#include "comm_backend.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/network/sock.h>
#include <stdio.h>
#include <conio.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/FolderCache.h>
#include "dbcomm/dbcomm.h"
#include "container/containerArena.h"
#include <utilitieslib/components/earray.h>
#include "arenaevent.h"
#include "arenaplayer.h"
#include "arenaserver.h"
#include "arenaupdates.h"
#include "arenaranking.h"
#include "gameData/arenaschedule.h"
#include <utilitieslib/network/net_linklist.h>
#include <utilitieslib/utils/sysutil.h>
#include <math.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/language/MultiMessageStore.h>
#include <utilitieslib/version/AppVersion.h>
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/utils/winutil.h>
#include <utilitieslib/utils/log.h>

#define MAX_CLIENTS 50
NetLinkList net_links;

int g_clearEventsOnStart = 0;                        // clear all events as soon as we start

// *********************************************************************************
//  arena message store
// *********************************************************************************

MultiMessageStore* g_arenaMessages;
int g_arenaLocale;
int g_arenaVerbose = 0;
static char g_localize_ret[10000];

void LoadArenaMessages(void)
{
    // this is a slight hack to load/share the mapserver's storyarc message store, which contains all of our messages

//    char* arenaMessageDirs[] = {
//        "/texts/defs",
//        NULL
//    };

    MessageStoreLoadFlags flags = MSLOAD_SERVERONLY | MSLOAD_NOFILES;

    // arena server locale
    g_arenaLocale = locGetIDInRegistry();

    // our message store
    LoadMultiMessageStore(&g_arenaMessages,"storyarcmsg",NULL,g_arenaLocale,NULL,NULL,NULL,NULL,flags);
//    LoadMultiMessageStore(&g_arenaMessages,NULL,"g_arenaMessagesArena",g_arenaLocale,NULL,NULL,arenaMessageDirs,NULL,MSLOAD_SERVERONLY);
}

char* ArenaLocalize(char* string)
{
    if (!string) return NULL;
    msxPrintf(g_arenaMessages, SAFESTR(g_localize_ret), g_arenaLocale, string, NULL, NULL, 0);
    return g_localize_ret;
}

// *********************************************************************************
//  arena main, etc.
// *********************************************************************************

static void ArenaDbConnect(void)
{
#define DB_CONNECT_TRIES 5
    int i;

    dbCommStartup();
    for (i = 0; i < DB_CONNECT_TRIES; i++)
    {
        if (dbTryConnect(15.0, DEFAULT_DBARENA_PORT)) break;
        if (i < DB_CONNECT_TRIES-1)
        {
            printf("failed to connect to dbserver, retrying..\n");
            Sleep(60);
        }
    }
    if (i == DB_CONNECT_TRIES)
    {
        printf("failed to connect to dbserver, exiting..\n");
        exit(1);
    }
#undef DB_CONNECT_TRIES
}

void UpdateArenaTitle(void)
{
    char buf[200];
    sprintf(buf, "ArenaServer - %i events, %i clients, %i rated players%s%s%s%s", 
        EventTotal(), 
        net_links.links? net_links.links->size: 0, 
        PlayerTotal(),
        db_comm_link.connected? "": " DISCONNECTED",
        g_disableEventLog? " NO EVENT LOG": "",
        g_disableScheduledEvents? " SCHEDULED EVENTS DISABLED": "",
        g_arenaVerbose? " VERBOSE": "");
    setConsoleTitle(buf);
}

static void ArenaLoadContainers(void)
{
    dbSyncContainerRequest(CONTAINER_ARENAEVENTS, 0, CONTAINER_CMD_LOAD_ALL, 0);
    EventListInit();
    printf("%i events, %i players loaded\n", EventTotal(), PlayerTotal());
    // MAKTODO - provide a way to clear arena players here or on dbserver, if the 
    // parent ent has been deleted
    UpdateArenaTitle();
}

static void AddTestEvent(void)
{
    int i;
    ArenaEvent * theEvent;
    char* names[] = {
        "foo", "bar", "whatzit", "arena", "server", "rocks", "world",
    };
    char* creatorNames[] = {
        "Running Wild","Thundernator","Landen","ZorbaTHut","dgarthur","Powerforce Mastery",
    };

    i = rand() % ARRAY_SIZE(names);
    theEvent=EventAdd(names[i]);
    strcpy(theEvent->creatorname,creatorNames[rand()%ARRAY_SIZE(creatorNames)]);
    theEvent->teamType = rand() % ARENA_NUM_TEAM_TYPES;
    theEvent->teamStyle = rand() % ARENA_NUM_TEAM_STYLES;
    theEvent->inviteonly = 0;
    theEvent->listed = 1;
    theEvent->maxplayers = 8;
    theEvent->minplayers = 2;

    EventSave(theEvent->eventid);
}

static int g_dte_index;
static int dteIter(ArenaEvent* ae)
{
    if (!g_dte_index)
    {
        EventLog(ae, "eventdestroyed");
        EventDelete(ae->eventid);
        return 0;
    }
    g_dte_index--;
    return 1;
}
static void DeleteTestEvent(void)
{
    if (!EventTotal()) return;
//    srand((unsigned int)time(NULL));
    g_dte_index = rand() % EventTotal();
    EventIter(dteIter);
}

static int daeIter(ArenaEvent* ae)
{
    EventLog(ae, "eventdestroyed");
    EventDelete(ae->eventid);
    return 1;
}
static void DeleteAllEvents(void)
{
    EventIter(daeIter);
}

static int paeIter(ArenaEvent* ae)
{
    ArenaEventCountPlayers(ae);
    printf("%4i-%-5i %-30s #P:%-4i %s\n", ae->eventid, ae->uniqueid, 
        ae->description[0]? ae->description: ae->creatorname, 
        ae->numplayers, StaticDefineIntRevLookup(ParseArenaPhase, ae->phase));
    return 1;
}
static void PrintAllEvents(void)
{
    EventIter(paeIter);
}

static void GenerateParticipants(ArenaEvent* event, int numPart)
{
    // generate participants for debugging
    int i;
    for(i = 0; i < numPart; i++)
    {
        ArenaParticipant *p = ArenaParticipantCreate();
        ArenaPlayer *ap = PlayerGetAdd(i+1, 0);
        p->dbid = i+1;
        p->side = i+1;
        p->rating = ap->ratingsByRatingIndex[2];    // solo swiss draw
        p->provisional = ap->provisionalByRatingIndex[2];
        eaPush(&event->participants, p);
        PlayerSave(i+1);
    }
}

static void PrintRankings(ArenaRankingTable* table)
{
    int i;
    for(i = 0; i < eaSize(&table->entries); i++)
    {
        printf("rank: %d side: %d played: %d SE: %f pts: %f opp_winpct: %f opp_opp_winpct: %f\n", table->entries[i]->rank, table->entries[i]->side, 
            table->entries[i]->played, table->entries[i]->SEpts, table->entries[i]->pts,
            table->entries[i]->opponentAvgPts, table->entries[i]->opp_oppAvgPts);
    }
}

static void RandomTournament(int randomrounds)
{
    int i, j, k;

    if (!g_events.events[1])
        EventAdd("testing");

    g_events.events[1]->sanctioned = 1;
    g_events.events[1]->teamStyle = ARENA_FREEFORM;
    g_events.events[1]->teamType = ARENA_SINGLE;
    g_events.events[1]->tournamentType = ARENA_TOURNAMENT_SWISSDRAW;

    for(i = 1, g_events.events[1]->round = 1; i <= randomrounds; i++)
    {
        EventProcessRankings(g_events.events[1]);
        EventCreatePairingsSwissDraw(g_events.events[1]);
        for(j = eaSize(&g_events.events[1]->seating)-1; j > 0 && g_events.events[1]->seating[j]->round == i; j--)
        {
            bool first = true;
            bool lose = false;
            for(k = 0; k < eaSize(&g_events.events[1]->participants); k++)
            {
                if(g_events.events[1]->participants[k]->seats[i] == j)
                {
                    if(first)
                    {
                        if(g_events.events[1]->roundtype[i] == ARENA_ROUND_SINGLE_ELIM)
                        {
                            if(rand() < RAND_MAX * 0.5)
                            {
                                g_events.events[1]->seating[j]->winningside = g_events.events[1]->participants[k]->side;
                                lose = true;
                            }
                        }
                        else if(g_events.events[1]->roundtype[i] == ARENA_ROUND_SWISSDRAW)
                        {
                            if(rand() < RAND_MAX * 0.33)
                            {
                                g_events.events[1]->seating[j]->winningside = g_events.events[1]->participants[k]->side;
                                lose = true;
                            }
                        }
                        first = false;
                    }
                    else
                    {
                        if(!lose)
                        {
                            if(g_events.events[1]->roundtype[i] == ARENA_ROUND_SINGLE_ELIM)
                            {
                                g_events.events[1]->seating[j]->winningside = g_events.events[1]->participants[k]->side;
                            }
                            else if(g_events.events[1]->roundtype[i] == ARENA_ROUND_SWISSDRAW)
                            {
                                if(rand() < RAND_MAX / 2)
                                    g_events.events[1]->seating[j]->winningside = g_events.events[1]->participants[k]->side;
                                else
                                    g_events.events[1]->seating[j]->winningside = -1;
                            }
                        }
                    }
                }
            }
        }
        g_events.events[1]->round++;
    }
    EventProcessRankings(g_events.events[1]);
    EventUpdateRatings(g_events.events[1]);
}

static void SetScheduled(ArenaEvent* event, int seconds)
{
    event->teamStyle = ARENA_FREEFORM;
    event->teamType = ARENA_SINGLE;
    EventChangePhase(event, ARENA_NEGOTIATING);
    event->scheduled = 1;
    event->start_time = event->round_start = dbSecondsSince2000() + seconds;
    ArenaEventCountPlayers(event);
    EventSave(event->eventid);
}

static U32 AddParticipant(ArenaEvent* event)
{
    ArenaParticipant* part = ArenaParticipantCreate();
    part->dbid = randInt(10000);
    eaPush(&event->participants, part);
    EventSave(event->eventid);
    return part->dbid;
}

static void RemoveParticipant(ArenaEvent* event)
{
    int i, n;

    n = eaSize(&event->participants);
    i = randInt(n);
    printf("Removing participant %i from the event\n", event->participants[i]->dbid);
    EventRemoveParticipant(event, event->participants[i]->dbid);
}

static int ArenaServerHandleMsg(Packet *pak,int cmd, NetLink *link)
{
    switch(cmd)
    {
        xcase ARENACLIENT_REQKIOSK:
            handleRequestKiosk(pak,link);
        xcase ARENACLIENT_REQJOINEVENT:
            handleJoinEvent(pak,link);
        xcase ARENACLIENT_REQDROPEVENT:
            handleDropEvent(pak, link);
        xcase ARENACLIENT_REGISTERPLAYERS:
            handleRegisterPlayers(pak, link);
        xcase ARENACLIENT_CREATEEVENT:
            handleCreateEvent(pak, link);
        xcase ARENACLIENT_DESTROYEVENT:
            handleDestroyEvent(pak, link);
        xcase ARENACLIENT_REQEVENT:
            handleRequestEvent(pak, link);
        xcase ARENACLIENT_SETSIDE:
            handleSetSide(pak, link);
        xcase ARENACLIENT_SETMAP:
            handleSetMap(pak, link);
        xcase ARENACLIENT_REQPLAYERUPDATE:
            handleRequestPlayerUpdate(pak, link);
        xcase ARENACLIENT_CREATORUPDATE:
            handleCreatorUpdate(pak, link);
        xcase ARENACLIENT_PLAYERUPDATE:
            handlePlayerUpdate(pak, link);
        xcase ARENACLIENT_FULLPLAYERUPDATE:
            handleFullPlayerUpdate(pak, link);
        xcase ARENACLIENT_MAPRESULTS:
            handleMapResults(pak, link);
        xcase ARENACLIENT_FEEPAYMENT:
            handleFeePayment(pak, link);
        xcase ARENACLIENT_CONFIRM_REWARD:
            handleConfirmReward(pak, link);
        xcase ARENACLIENT_REQRESULTS:
            handleRequestResults(pak, link);
        xcase ARENACLIENT_PLAYERATTRIBUTEUPDATE:
            handlePlayerAttributeUpdate(pak, link);
        xcase ARENACLIENT_CLEARPLAYERSGRATINGS:
            handleClearPlayerSGRatings(pak, link);
        xcase ARENACLIENT_REQLEADERS:
            handleRequestLeaderUpdate(pak, link);
        xcase ARENACLIENT_REQPLAYERSTATS:
            handleRequestPlayerStats(pak, link);
        xcase ARENACLIENT_READYACK:
            handleReadyAck(pak, link);
        xcase ARENACLIENT_REPORTKILL:
            handleReportKill(pak, link);
        xcase ARENACLIENT_SET_LOG_LEVELS:
            {
                int i;
                for( i = 0; i < LOG_COUNT; i++ )
                {
                    int log_level = pktGetBitsPack(pak,1);
                    logSetLevel(i,log_level);
                }
            }
        xcase ARENACLIENT_TEST_LOG_LEVELS:
        {
            LOG( LOG_DEPRECATED, LOG_LEVEL_ALERT, LOG_CONSOLE, "ArenaServer Log Line Test: Alert" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "ArenaServer Log Line Test: Important" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_VERBOSE, LOG_CONSOLE, "ArenaServer Log Line Test: Verbose" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_DEPRECATED, LOG_CONSOLE, "ArenaServer Log Line Test: Deprecated" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_DEBUG, LOG_CONSOLE, "ArenaServer Log Line Test: Debug" );
        }

        xdefault:
            printf("Unknown command %d\n",cmd);
            return 0;
    }
    return 1;
}

// fields set by ArenaCommandStart
U32 g_request_dbid;
U32 g_request_user;
U32 g_request_eventid;
U32 g_request_uniqueid;
char g_request_name[32];

// get initial header that starts most requests
void ArenaCommandStart(Packet* pak)
{
    g_request_dbid = pktGetBitsPack(pak, 1);
    g_request_user = pktGetBitsPack(pak, 1);
    g_request_eventid = 0;    // clear these varibles, they are not standard yet so
    g_request_uniqueid = 0; // if funcion is using them it can set them itself
}

void ArenaCommandResult(NetLink* link, ArenaServerResult res, U32 num, char* err )
{
    Packet* ret = pktCreateEx(link, ARENASERVER_RESULT);
    pktSendBitsPack(ret, 1, res);
    pktSendBitsPack(ret, 1, num);
    pktSendBitsPack(ret, 1, g_request_dbid);
    pktSendBitsPack(ret, 1, g_request_user);
    pktSendBitsPack(ret, 1, g_request_eventid);
    pktSendBitsPack(ret, 1, g_request_uniqueid);
    pktSendString(ret, err);
    pktSend(&ret, link);
}

static void ArenaShowDetail(ArenaEvent* event)
{
    char* estr = 0;
    estrCreate(&estr);
    ArenaEventDebugPrint(event, &estr, dbSecondsSince2000());
    printf("%s", estr);
    estrDestroy(&estr);
}

static void arenaEventCommand(char* idstr, char cmd)
{
    ArenaEvent* event;
    int id = atoi(idstr);
    char* estr = 0;

    printf("Event %s\n", idstr);
    if (!id)
    {
        printf("Invalid id\n");
        return;
    }
    event = EventGet(id);
    if (!event)
        printf("No event with id %i\n", id);
    else
    {
        switch (cmd)
        {
        case 'D':
            ArenaShowDetail(event);
            break;
        case 'R':
            EventLog(event, "eventdestroyed");
            EventDelete(id);
            printf("removed.\n");
            break;
        case 'K':
            printf("generating and printing rankings table..\n");
            PrintRankings(EventProcessRankings(event));
            break;
        case 'G':
            printf("generating 3 participants..\n");
            GenerateParticipants(event, 3);
            break;
        case 'S':
            printf("Setting event to be scheduled 90 seconds from now..\n");
            SetScheduled(event, 90);
            break;
        case 'P':
            printf("Adding participant with dbid %i\n", AddParticipant(event));
            break;
        case 'Q':
            RemoveParticipant(event);
            break;
        default:
            printf("arenaEventCommand missing cmd %c\n", cmd);
        }
    }
}

static void arenaDebugHook()
{
    static char idstr[200];
    static int enter_id=0;
    static char enter_cmd=' ';
    static int debug_enabled=0;
    char ch;

    if (_kbhit()) {

        // cheesy way to get event id from user..
        if (enter_id)
        {
            ch = _getch();
            if (ch == '\b')
            {
                if (enter_id > 1) 
                {
                    enter_id--;
                    printf("\b \b");
                }
            }
            else if (ch == '\n' || ch == '\r')
            {
                idstr[enter_id-1] = 0;
                printf("\n");
                arenaEventCommand(idstr, enter_cmd);
                enter_id = 0;
            }
            else
            {
                idstr[enter_id-1] = ch;
                enter_id++;
                printf("%c", ch);
                if (enter_id >= 199)
                {
                    idstr[199] = 0;
                    printf("\n");
                    arenaEventCommand(idstr, enter_cmd);
                    enter_id = 0;
                }
            }
        }
        else if (debug_enabled<2) {
            if (_getch()=='d') {
                debug_enabled++;
                if (debug_enabled==2) {
                    printf("Debugging hotkeys enabled (Press '?' for a list).\n");
                }
            } else {
                debug_enabled = 0;
            }
        } else {
            ch = _getch();
            switch (ch) {
                case 'a':
                    printf("adding a random event..\n");
                    AddTestEvent();
                    break;
                case 'c':
                    printf("disconnecting from dbserver and clients..\n");
                    clearNetLink(&db_comm_link);
                    netForEachLink(&net_links, netRemoveLink);
                    netLinkListDisconnect(&net_links);
                    UpdateArenaTitle();

                    printf("sleeping..\n");
                    Sleep(1000);

                    printf("reconnecting to dbserver..\n");
                    ArenaDbConnect();
                    UpdateArenaTitle();
                    ArenaLoadContainers();

                    printf("allowing client connections..\n");
                    while (!netInit(&net_links,0,DEFAULT_ARENASERVER_PORT))
                        Sleep(1);
                    break;
                case 'l':
                    printf("showing all events..\n");
                    PrintAllEvents();
                    printf("(%i total)\n", EventTotal());
                    break;
                case 'm':
                    memMonitorDisplayStats();
                    break;
                case 'n':
                    printf("db_link sendQueue size: %d\n", qGetSize(db_comm_link.sendQueue2));
                    printf("connected mapservers: %d\n", net_links.links? net_links.links->size: 0);
                    break;
                case 'p':
                    printf("SERVER PAUSED FOR 2 SECONDS PRESS 'p' AGAIN TO PAUSE INDEFINITELY...\n");
                    Sleep(2000);
                    if (_kbhit() && _getch()=='p') {
                        printf("SERVER PAUSED\n");
                        _getch();
                    }
                    printf("Server resumed\n");
                    break;
                case 'r':
                    if (EventTotal())
                    {
                        printf("deleting a random event..\n");
                        DeleteTestEvent();
                    }
                    else
                        printf("no events remaining\n");
                    break;
                case 's':
                    {
                        int i = 0, nump;
                        printf("starting test runs..\n");
                        while(!_kbhit())
                        {
                            printf("test run %d\n", ++i);
                            AddTestEvent();
                            nump = rand() * ARENA_MAX_SIDES / (RAND_MAX+1);
                            GenerateParticipants(g_events.events[1], nump);
                            RandomTournament(10);
                            EventProcessRankings(g_events.events[1]);
                            PrintRankings(g_events.events[1]->rankings);
                            EventLog(g_events.events[1], "eventdestroyed");
                            EventDelete(1);
//                            Sleep(1000);
                        }
                    }
                    break;
                case 't':
                    printf("randomly running tournament..\n");
                    RandomTournament(10);
                    break;
                case 'v':
                    g_arenaVerbose = !g_arenaVerbose;
                    printf("verbose mode %s\n", g_arenaVerbose ? "on" : "off");
                    UpdateArenaTitle();
                    break;
                case 'x':
                    {
                        g_disableScheduledEvents = !g_disableScheduledEvents;
                        UpdateArenaTitle();
                    }
                    break;
                case 'y':
                    {
                        g_disableEventLog = !g_disableEventLog;
                        UpdateArenaTitle();
                    }
                    break;
                case 'D': case 'R': case 'K': case 'G': case 'S': case 'P': case 'Q':
                    printf("enter an event id: ");
                    enter_id = 1;
                    enter_cmd = ch;
                    break;

                case '?':
                    printf("General commands:\n");
                    printf("  m - print memory usage\n");
                    printf("  n - print some network stats\n");
                    printf("  p - pause server\n");
                    printf("  c - reconnect to DbServer in 1s\n");
                    printf("  a/r/l - event testing (add/remove/list)\n");
                    printf("  t - randomly do a tournament for 5 out of the 10 rounds\n");
                    printf("  s - continuous swiss draw test runs\n");
                    printf("  x - toggle disabling scheduled events\n");
                    printf("  y - toggle disabling completed event log\n");
                    printf("Commands for specific events:\n");
                    printf("  D - detailed info on an event\n");
                    printf("  R - remove a specific event\n");
                    printf("  K - generate and print rankings for event\n");
                    printf("  G - generate participants and add to event for tournament\n");
                    printf("  S - set an event to be scheduled and starting in 90 seconds\n");
                    printf("  P - add a participant with random dbid\n");
                    printf("  Q - remove a participant with random dbid\n");
                    break;
            }
        }
    }
}

#define ARENASERVER_TICK_FREQ    10        // ticks/sec

static int *entsOnline = NULL;
static int *entsOffline = NULL;
static int netpacketcallback_SomeOnlineEnts(Packet *pak, int cmd, NetLink *link)
{
    int count = pktGetBitsAuto(pak);
    int i;
    eaiClear(&entsOnline);
    eaiClear(&entsOffline);
    for( i = 0; i < count; ++i ) 
    {
        int dbid = pktGetBitsAuto(pak);
        bool online = pktGetBitsPack(pak, 1);
        if( online )
        {
            eaiPush(&entsOnline,dbid);
        }
        else
        {
            eaiPush(&entsOffline,dbid);
        }
    }
    return 1;
}

int* reqSomeOnlineEnts(int *ids, bool wantOnline)
{
    static PerformanceInfo* perfInfo;
    int i;
    int count = eaiSize( &ids );
    Packet *pak = pktCreateEx(&db_comm_link, DBCLIENT_REQ_SOME_ONLINE_ENTS);
#if defined(_M_X64)
    #pragma message("The line below needs to be proper ported to 64 bit compatible code")
#endif
    pktSendBitsPack( pak, 1, (U32)(uintptr_t)netpacketcallback_SomeOnlineEnts);
    pktSendBitsAuto( pak, count);
    for(i = 0; i < count; ++i )
    {
        pktSendBitsAuto(pak,ids[i]);
    }
    pktSend(&pak, &db_comm_link);
    if(dbMessageScanUntil(__FUNCTION__, &perfInfo))
    {
        if( wantOnline )
        {
            return entsOnline;
        }
        else
        {
            return entsOffline;
        }
    }
    else
    {
        return NULL;
    }
}

int main(int argc,char **argv)
{
int        i,timer;

    EXCEPTION_HANDLER_BEGIN
    setAssertMode(ASSERTMODE_MINIDUMP | ASSERTMODE_DEBUGBUTTONS);
    memMonitorInit();
    for(i = 0; i < argc; i++){
        printf("%s ", argv[i]);
    }
    printf("\n");

    setWindowIconColoredLetter(compatibleGetConsoleWindow(), 'A', 0x8080ff);

    FolderCacheChooseMode();

    preloadDLLs(0);

    if (fileIsUsingDevData()) {
        bsAssertOnErrors(true);
        setAssertMode(ASSERTMODE_DEBUGBUTTONS |
            (!IsDebuggerPresent()? ASSERTMODE_MINIDUMP : 0));
    } else {
        // In production mode on the servers we want to save all dumps and do full dumps
        setAssertMode(ASSERTMODE_DEBUGBUTTONS |
            ASSERTMODE_FULLDUMP |
            ASSERTMODE_DATEDMINIDUMPS | ASSERTMODE_ZIPPED);
    }

    srand((unsigned int)time(NULL));
    consoleInit(110, 128, 0);
    UpdateArenaTitle();

    logSetDir("arenaserver");
    sockStart();
    serverCfgLoad();
    if (server_cfg.locale_id != -1)
    {
        locOverrideIDInRegistryForServersOnly(server_cfg.locale_id);
    }
    LoadArenaMessages();
    for(i=1;i<argc;i++)
    {
        if (stricmp(argv[i],"-db")==0)
            strcpy(server_cfg.db_server,argv[++i]);
        else if (stricmp(argv[i],"-disableschedule")==0)
            g_disableScheduledEvents = 1;
        else if (stricmp(argv[i],"-disablelog")==0)
            g_disableEventLog = 1;
        else if (stricmp(argv[i],"-clearevents")==0)
            g_clearEventsOnStart = 1;
        else if (stricmp(argv[i],"-locale")==0 && i+1 < argc)
        {
            g_arenaLocale = locGetIDByNameOrID(argv[++i]);
        }
        else
        {
            printf("-----INVALID PARAMETER: %s\n", argv[i]);
        }
    }
    strcpy(db_state.server_name, server_cfg.db_server);
    printf("Server locale: %s\n", locGetName(g_arenaLocale));

    loadstart_printf("Networking startup...");
    timer = timerAlloc();
    timerStart(timer);
    packetStartup(0,0);
    loadend_printf("");

    loadstart_printf("Connecting to dbserver (%s)..", db_state.server_name);
    ArenaDbConnect();
    ArenaLoadContainers();
    loadend_printf("");

    if (g_clearEventsOnStart)
    {
        loadstart_printf("CLEARING ALL EVENTS..");
        DeleteAllEvents();
        loadend_printf("");
    }

    loadstart_printf("Opening client port..");
    netLinkListAlloc(&net_links,MAX_CLIENTS,sizeof(ArenaClientLink),ArenaClientConnect);
    net_links.destroyCallback = ArenaClientDisconnect;
    while (!netInit(&net_links,0,DEFAULT_ARENASERVER_PORT))
        Sleep(1);
    NMAddLinkList(&net_links, ArenaServerHandleMsg);
    loadend_printf("");

    loadstart_printf("Building leaderboard..");
    EventInitLeaderList();
    loadend_printf("");

    loadstart_printf("Misc other..");
    arenaLoadMaps();
    LoadScheduledEvents();
    SetTTLForNegotiatingPlayers();
    loadend_printf("");

    printf("Ready.\n");
    for(;;)
    {
        dbComm();
        NMMonitor(1);
        netIdle(&db_comm_link,1,10);
        arenaDebugHook();
        if (timerElapsed(timer) > (1.f / ARENASERVER_TICK_FREQ))
        {
            timerStart(timer);
            EventProcess();
            PlayerListTick();
            ScheduledEventsTick();
            CheckForPlayerDisconnects();
        }
    }
    EXCEPTION_HANDLER_END
}
