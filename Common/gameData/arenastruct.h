/*\
 *
 *    arenastruct.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Arena structures and code to send/receive arena kiosk list.  
 *  This is a list of arena events, with only partial info available.
 *
 */

#ifndef ARENASTRUCT_H
#define ARENASTRUCT_H

#include <stdlib.h>
#include "arenaref.h"
#include <utilitieslib/utils/textparser.h>
#include "entity/gametypes.h"
#include <utilitieslib/utils/structDefines.h>

typedef struct Packet Packet;

// arena structures

#define ARENA_DESCRIPTION_LEN    256
#define ARENA_MAX_SIDES            512
#define ARENASIDES_NUMBITS        10        // side 0 isn't used, so 512 participants is 10 bits
#define ARENA_MAX_ROUNDS        11        // should fit with max participants, also container code assumes 11
                                        // round 0 is not used so should be one more than actual number
#define ARENA_WIN_POINTS        2        // tournament points to get for a win
#define ARENA_DRAW_POINTS        1
#define ARENA_POINT_DIVISOR        2

#define ARENA_SINGLE_ELIM_RDS    3        // number of rounds of single elimination to have in a swiss draw tournament

#define ARENA_PROVISIONAL_MATCHES    10    // number of matches a player's rating is considered provisional
#define ARENA_PROVISIONAL_BITS        4    // this needs to be the number of bits for ARENA_PROVISIONAL_MATCHES + 1!!

#define ARENA_TOURNAMENT_PRIZE1    60
#define ARENA_TOURNAMENT_PRIZE2    30

#define ARENA_READY_BIT            (1<<0)
#define ARENA_READY_ROOM_BIT    (1<<1)    
#define ARENA_READY_NUMBITS        2
#define ARENA_IS_READY(ready)    (ready == (ARENA_READY_BIT))

#define ARENA_TEAMSIDE_NUMBITS        4

#define ARENA_NUM_PROBLEMS            5    // max number of sanctioning problems to keep track of
#define ARENA_PROBLEM_STR_SIZE        32    // number of characters for the problem string

#define ARENA_READY_TIMER_SINCE_LAST_ATTACKED    10
#define ARENA_WAIT_PERIOD_FOR_READIES    5

#define ARENA_BUFF_TIMER            30  // How long the buff phase lasts
#define ARENA_COUNTDOWN_TIMER        5    // How long the countdown phase lasts (alternate mode for buff)

#define ARNEA_MAX_SCHEDULED            3

#define ARENA_LEADERBOARD_NUM_POS    10

#define START_EVENT_DELAY                    10
#define ARENA_SERVER_NEGOTIATION_WAIT        (2 * 60)    // time to wait for teleporting
#define ARENA_SERVER_NEGOTIATION_READY        (15)        // shortens to this time if everyone is ready

#define ARENA_SCHEDULED_TELEPORT_WAIT    30
#define ARENA_INTERMISSION_WAIT        120    // normally 120, 10 for debugging
#define ARENA_WAIT_FOR_PAYOUT        60

#define ARENA_RANDOMMAP_ID            9999

static char *arenaArchetype[] = { 
    "Class_Blaster",
    "Class_Controller",
    "Class_Defender",
    "Class_Scrapper",
    "Class_Tanker",
    "Class_Warshade",
    "Class_Peacebringer",
    "Class_Brute",
    "Class_Corruptor",
    "Class_Dominator",
    "Class_MasterMind",
    "Class_Stalker", 
    "Class_Arachnos_Widow", 
    "Class_Arachnos_Soldier", 
};

typedef enum
{
    ARENA_FREEFORM,
    ARENA_STAR,
    ARENA_STAR_VILLAIN,
    ARENA_STAR_VERSUS,
    ARENA_7STAR,
    ARENA_7STAR_VILLAIN,
    ARENA_7STAR_VERSUS,
    // ONLY ADD HERE
    ARENA_NUM_TEAM_STYLES,
} ArenaTeamStyle;
#define ARENATEAMSTYLE_NUMBITS    4
extern StaticDefineInt ParseArenaTeamStyle[];

// Used because the wins/losses/draws stats added in I14 use the old ArenaEventType enum
#define ARENA_NUM_EVENT_TYPES_FOR_STATS 10
#define ARENA_STAR_STATS_OFFSET 2
extern StaticDefineInt ParseArenaEventTypeForStats[];

typedef enum
{
    ARENA_SINGLE,
    ARENA_TEAM,
    ARENA_SUPERGROUP,
    // ONLY ADD HERE
    ARENA_NUM_TEAM_TYPES,
} ArenaTeamType;
#define ARENATEAMTYPE_NUMBITS    2
extern StaticDefineInt ParseArenaTeamType[];

typedef enum
{
    ARENA_TIMED,
    ARENA_TEAMSTOCK,
    ARENA_LASTMAN,
    ARENA_KILLS,
    ARENA_STOCK,
    // ONLY ADD HERE
    ARENA_NUM_VICTORY_TYPES,
} ArenaVictoryType;
#define ARENAVICTORYTYPE_NUMBITS 4
extern StaticDefineInt ParseArenaVictoryType[];

typedef enum
{
    ARENA_TOURNAMENT_NONE,
    ARENA_TOURNAMENT_SWISSDRAW,
    // ONLY ADD HERE
    ARENA_NUM_TOURNAMENT_TYPES,
} ArenaTournamentType;
#define ARENATOURNAMENTTYPE_NUMBITS 2
extern StaticDefineInt ParseArenaTournamentType[];

typedef enum
{
    ARENA_WEIGHT_ANY,
    ARENA_WEIGHT_0,
    ARENA_WEIGHT_5,
    ARENA_WEIGHT_10,
    ARENA_WEIGHT_15,
    ARENA_WEIGHT_20,
    ARENA_WEIGHT_25,
    ARENA_WEIGHT_30,
    ARENA_WEIGHT_35,
    ARENA_WEIGHT_40,
    ARENA_WEIGHT_45,
    // ONLY ADD HERE
    ARENA_NUM_WEIGHT_CLASSES,
} ArenaWeightClass;
#define ARENAWEIGHT_NUMBITS    4
extern StaticDefineInt ParseArenaWeightClass[];

typedef enum
{
    ARENA_NO_INSPIRATIONS,
    ARENA_SMALL_INSPIRATIONS,
    ARENA_SMALL_MEDIUM_INSPIRATIONS,
    ARENA_NORMAL_INSPIRATIONS,
    ARENA_ALL_INSPIRATIONS,
    ARENA_NUM_INSPIRATION_SETTINGS
} ArenaInspirationSetting;

static char * g_PhaseTranslationKeys[] = { "ScheduledStr",        // ARENA_SCHEDULED
                                            "NegotiatingStr",    // ARENA_NEGOTIATING
                                            "InProgressStr",    // ARENA_STARTROUND
                                            "InProgressStr",    // ARENA_RUNROUND
                                            "InProgressStr",    // ARENA_ENDROUND
                                            "InProgressStr",    // ARENA_INTERMISSION,
                                            "InProgressStr"        // ARENA_PAYOUT_WAIT,
                                            "InProgressStr",    // ARENA_PRIZE_PAYOUT
                                            "CompletedStr",        // ARENA_ENDED
                                            "CancelledStr",        // ARENA_CANCEL_REFUND
                                            "CancelledStr",        // ARENA_CANCELLED
                                        };
typedef enum // NOTE if you add to enum below, update translation string translation table
{
    ARENA_SCHEDULED,
    ARENA_NEGOTIATING,
    ARENA_STARTREADY,
    ARENA_WAITREADY,
    ARENA_STARTROUND,
    ARENA_RUNROUND,
    ARENA_ENDROUND,
    ARENA_INTERMISSION,
    ARENA_PAYOUT_WAIT,
    ARENA_PRIZE_PAYOUT,
    ARENA_ENDED,    
    ARENA_CANCEL_REFUND,
    ARENA_CANCELLED,

    ARENA_NUM_PHASES,
} ArenaPhase;
#define ARENAPHASE_NUMBITS 4
extern StaticDefineInt ParseArenaPhase[];

typedef enum
{
    ARENAKIOSK_DUEL,
    ARENAKIOSK_TEAM,
    ARENAKIOSK_SUPERGROUP,
    ARENAKIOSK_TOURNAMENT,
} ArenaKioskType;
#define ARENAKIOSK_NUMBITS 2

typedef enum
{
    ARENA_TAB_ALL,
    ARENA_TAB_SOLO,
    ARENA_TAB_TEAM,
    ARENA_TAB_SG,
    ARENA_TAB_TOURNY,
    ARENA_TAB_TOTAL,
}ArenaTabs;

typedef enum
{
    ARENA_FILTER_ELIGABLE,
    ARENA_FILTER_BEST_FIT,
    ARENA_FILTER_ONGOING,
    ARENA_FILTER_COMPLETED,
    ARENA_FILTER_MY_EVENTS,
    ARENA_FILTER_INELIGABLE,
    ARENA_FILTER_PLAYER,
    ARENA_FILTER_ALL,
    ARENA_FILTER_TOTAL,
}ArenaFilter;
#define ARENAFILTER_NUMBITS 3

typedef enum ArenaRoundType
{
    ARENA_ROUND_SWISSDRAW,
    ARENA_ROUND_SINGLE_ELIM,
    ARENA_ROUND_DONE,
    ARENA_NUM_ROUNDTYPE,
} ArenaRoundType;
#define ARENAROUNDTYPE_NUMBITS 2

typedef struct ArenaParticipant
{
    char*            name;    // only created by mapservers, strdup/free
    const char*        archetype;

    U32                dbid;
    int                level;
    U32                sgid;
    int                sgleader;            // 0 or 1 only

    int                side;
    int                seats[ARENA_MAX_ROUNDS];
    int                kills[ARENA_MAX_ROUNDS];
    int                death_time[ARENA_MAX_ROUNDS];
    int                id;
    F32                rating;
    int                provisional;
    int                paid;
    int                entryfee;

    const char*        desired_archetype;
    int                desired_team;
    
    int                roundlastfloated;
    int                seed;
    int                dropped;

    int                arena_team_id; // not persisted
    bool            client_countdown_started; // not persisted
} ArenaParticipant;

typedef struct EventHistoryEntry
{
    ArenaParticipant**    p;
    int                    side;
    int                    SEpoints;
    int                    points;
    int                    totalkills;
    int                    gamesplayed;
    int                    deathtime;    // only used for one round events right now so not bothering with an array
    int                    pastOpponents[ARENA_MAX_ROUNDS];
    int                    nextOpponent;
    int                    nextSeat;
    int                    dropped;
    int                    roundlastfloated;
    int                    seed;
    int                    hashadbye;
    int                    group;
    int                    fee;            // the event's entry fee
    int                    haveswitched;

    int                    wins;
    int                    draws;
    int                    losses;

    int                    numplayers;        // number of participants
    F32                    totalrating;    // total rating of all participants
    int                    numprov;        // number of provisional players on the team
    F32                    totalprovrating;// total provisional rating

    // tie breakers
    F32                    Pts;            // points divided by points divisor
    F32                    opp_AvgPts;
    F32                    opp_opp_AvgPts;
} EventHistoryEntry;

typedef struct EventHistory
{
    EventHistoryEntry* sidetable[ARENA_MAX_SIDES + 1];    // side 0 is not used
    EventHistoryEntry** sides;
} EventHistory;

typedef struct ArenaSeating
{
    U32                mapid;
    U32                matchtime;
    int                round;
    int                winningside;
} ArenaSeating;

// useful defines for status/types of events
#define SERVER_RUN(event) ((event)->creatorid? 0: 1)                // server-generated event
#define INSTANT_EVENT(event) ((event)->phase == ARENA_NEGOTIATING)    // event may start at any time
#define ASAP_EVENT(event) ((event)->phase == ARENA_SCHEDULED && !(event)->start_time)    // event will start as soon as possible
#define ACTIVE_EVENT(event) (((event)->phase >= ARENA_STARTROUND && (event)->phase <= ARENA_INTERMISSION) || \
                            INSTANT_EVENT(event))    // player only allowed to have one Active event
#define COMPLETED_EVENT(event) ((event)->phase > ARENA_INTERMISSION)    // event is no longer running for whatever reason

typedef struct ArenaRankingTableEntry
{
    int        rank;
    char*    playername;
    int        dbid;
    int        side;
    int        played;
    F32        SEpts;
    F32        pts;
    F32        opponentAvgPts;
    F32        opp_oppAvgPts;    // Avg win percentage of opponent's opponents
    F32        rating;
    int        kills;
    int        deathtime;
} ArenaRankingTableEntry;

typedef struct ArenaRankingTable
{
    ArenaRankingTableEntry** entries;
    ArenaTeamType teamType;
    ArenaTeamStyle teamStyle;
    ArenaVictoryType victoryType;
    int victoryValue;
    ArenaWeightClass weightClass;
    char desc[ARENA_DESCRIPTION_LEN];
} ArenaRankingTable;

typedef struct ArenaEvent 
{
    U32                eventid;
    U32                uniqueid;

    // PLAYER-CONTROLLED SETTINGS

    ArenaTeamStyle        teamStyle;
    ArenaTeamType        teamType;
    ArenaWeightClass    weightClass;
    ArenaVictoryType    victoryType;
    int                    victoryValue;
    ArenaTournamentType    tournamentType;

    int                verify_sanctioned;    // tells server it has to verify the event
    int                inviteonly;            // 1 or 0 only
    int                no_observe;
    int                numsides;
    int                maxplayers;

    int                        tactical_start;
    int                        no_end;
    int                        no_pool;
    int                        no_travel;
    int                        no_stealth;
    int                        use_gladiators;
    int                        no_travel_suppression;
    int                        no_diminishing_returns;
    int                        no_heal_decay;
    ArenaInspirationSetting    inspiration_setting;
    int                        enable_temp_powers;

    // INTERNAL VARIABLES

    U32                lastupdate_dbid;    // only really valid for immediate results of commands
    U32                lastupdate_cmd;
    char            lastupdate_name[32];

    ArenaPhase            phase;
    ArenaFilter            filter;

    int                round;
    U32                start_time;            // time event went into Negotiating
    U32                round_start;        // when this round started, also when event ended
    U32                mapcheck_time;        // when the last map check was - NOT SAVED
    U32                next_round_time;    // time next round will start in a multi-round event - NOT SAVED
    int                leader_ready;        // 1 or 0 only

    int                scheduled;            // 1 or 0 only
    int                serverEventHandle;    // handle to scheduled event list for server-scheduled events
    int                demandplay;            // 1 or 0 only

    int                sanctioned;            // 1 or 0 only
    int                listed;
    int                minplayers;
    int                numplayers;
    int                entryfee;
    int                custom; // unused

    char            description[ARENA_DESCRIPTION_LEN];
    U32                creatorid;
    U32                creatorsg;
    U32                othersg;                    // only set for SG rumble
    char            mapname[MAX_PATH];
    int                mapid;
    char            creatorname[MAX_NAME_LEN];    // this is the name of the original creator, not necessarily
                                                // matching creatorid
    char            cancelreason[MAX_NAME_LEN];

    int                cannot_start;
    char            eventproblems[ARENA_NUM_PROBLEMS][ARENA_PROBLEM_STR_SIZE];
    int                finalReadiesLeft;

    ArenaRoundType    roundtype[ARENA_MAX_ROUNDS];

    ArenaParticipant**    participants;    // kiosk list doesn't get this, must update _numplayers_
    ArenaSeating**        seating;        // kiosk list doesn't get this
    EventHistory*        history;        // not sent down, used for rankings and seatings
    ArenaRankingTable*    rankings;        // not sent down or saved

    unsigned int    kiosksend            : 1;    // should be sent next ArenaKioskSend() call
    unsigned int    detailed            : 1;

    unsigned int    arenaTourneyIntermissionStartMsgSent    : 1;
    unsigned int    arenaTourney15SecsMsgSent                : 1;
    unsigned int    arenaTourney5SecsMsgSent                : 1;
} ArenaEvent;
// additions to ArenaEvent need to be reflected in ArenaEventSend/Receive and packageEvent/unpack

typedef struct ArenaEventList
{
    ArenaEvent**    events;
} ArenaEventList;

#define ARENA_NUM_RATINGS        10        // must change containerArena to match
#define ARENA_DEFAULT_RATING    1500    // rating players start at

typedef enum ArenaPlayerConnection
{
    ARENAPLAYER_LOGOUT,                // lastconnected has timed out, player is logged out
    ARENAPLAYER_CONNECTED,            // there is a currently registered mapserver connection for this dbid
    ARENAPLAYER_DISCONNECTED,        // no mapserver, maybe in transfer, lastconnected field is set
} ArenaPlayerConnection;

#define RAID_BITS_PER_PLAYER        30
#define RAID_BITFIELD_SIZE            ((RAID_BITS_PER_PLAYER+31) / 32)

typedef struct ArenaPlayer
{
    U32        dbid;
    int        prizemoney;        // prizemoney has to be on ArenaPlayer because it might be awarded when players are not online

    ArenaPlayerConnection    connected;
    U32                        lastConnected;

    int        wins;
    int        draws;
    int        losses;

    int        winsByEventType[ARENA_NUM_EVENT_TYPES_FOR_STATS];
    int        drawsByEventType[ARENA_NUM_EVENT_TYPES_FOR_STATS];
    int        lossesByEventType[ARENA_NUM_EVENT_TYPES_FOR_STATS];

    int        winsByTeamType[ARENA_NUM_TEAM_TYPES];
    int        drawsByTeamType[ARENA_NUM_TEAM_TYPES];
    int        lossesByTeamType[ARENA_NUM_TEAM_TYPES];

    int        winsByVictoryType[ARENA_NUM_VICTORY_TYPES];
    int        drawsByVictoryType[ARENA_NUM_VICTORY_TYPES];
    int        lossesByVictoryType[ARENA_NUM_VICTORY_TYPES];

    int        winsByWeightClass[ARENA_NUM_WEIGHT_CLASSES];
    int        drawsByWeightClass[ARENA_NUM_WEIGHT_CLASSES];
    int        lossesByWeightClass[ARENA_NUM_WEIGHT_CLASSES];

    int        winsByRatingIndex[ARENA_NUM_RATINGS];        // Only counts rated games
    int        drawsByRatingIndex[ARENA_NUM_RATINGS];        // Only counts rated games
    int        lossesByRatingIndex[ARENA_NUM_RATINGS];        // Only counts rated games
//    int        dropsByRatingIndex[ARENA_NUM_RATINGS];
    F32        ratingsByRatingIndex[ARENA_NUM_RATINGS];
    int        provisionalByRatingIndex[ARENA_NUM_RATINGS];        // values are number of games played with a provisional ranking
    
    U32        raidparticipant[RAID_BITFIELD_SIZE];

    char    arenaReward[128];
} ArenaPlayer;

// you can get detailed weight info from this global table
typedef struct ArenaWeightRange
{
    ArenaWeightClass    weight;
    int                    minLevel;                // 0-based
    int                    maxLevel;                // 0-based
} ArenaWeightRange;
extern ArenaWeightRange g_weightRanges[ARENA_NUM_WEIGHT_CLASSES];

ArenaWeightClass GetWeightClass(int level);                    // 0-based

// arena maps
typedef struct ArenaMap
{
    char    *mapname;
    char    *displayName;
    int        minplayers;
    int        maxplayers;
} ArenaMap;

typedef struct ArenaMaps
{
    ArenaMap**    maps;
} ArenaMaps;
extern ArenaMaps g_arenamaplist;
void arenaLoadMaps();
void ArenaSelectMap(ArenaEvent* event);

//filter for ArenaKioskSendFilteredFunc that filters based on what is selected in the kiosk
void EventKioskFilter(ArenaEvent * event, U32 dbid, const char* archetype, int level, int radioButtonFilter, int tabFilter);

// send/receive
void ArenaKioskSend(Packet* pak, ArenaEventList* list);        // sends only those events marked with kiosksend
void ArenaKioskReceive(Packet* pak, ArenaEventList* list);
void ArenaEventSend(Packet* pak, ArenaEvent* event, int detailed);
void ArenaEventReceive(Packet* pak, ArenaEvent* event);
void ArenaPlayerSend(Packet* pak, ArenaPlayer* ap);
void ArenaPlayerReceive(Packet* pak, ArenaPlayer* ap);
void ArenaRankingTableSend(Packet* pak,ArenaRankingTable* art);
void ArenaRankingTableReceive(Packet* pak,ArenaRankingTable* art);
void ArenaLeaderBoardSend(Packet* pak, ArenaRankingTable leaderboard[ARENA_NUM_RATINGS]);
void ArenaLeaderBoardReceive(Packet* pak, ArenaRankingTable leaderboard[ARENA_NUM_RATINGS]);
void ArenaCreatorUpdateSend( ArenaEvent * event, Packet * pak );
void ArenaCreatorUpdateRecieve( ArenaEvent * event, Packet * pak );
void ArenaParticipantUpdateSend( ArenaEvent * event, ArenaParticipant * ap, Packet * pak );
void ArenaParticipantUpdateRecieve( ArenaEvent * event, ArenaParticipant * ap, Packet * pak );
void ArenaFullParticipantUpdateSend( ArenaEvent *event, Packet * pak );
void ArenaFullParticipantUpdateRecieve( ArenaEvent * event, Packet * pak );

// util
char* nameFromArenaParticipant(ArenaEvent* event, int dbid);
ArenaParticipant* ArenaParticipantFind(ArenaEvent* event, int dbid);
void ArenaEventCopy(ArenaEvent* target, ArenaEvent* source);  // DOESN'T COPY HISTORY
void ArenaEventCountPlayers(ArenaEvent* event); // fill out numplayers with info from participants list
char* ArenaConstructInfoString(int eventid, int seat);
int ArenaDeconstructInfoString(char* mapinfo, int *eventid, int *seat); // returns success
int EventGetRatingIndex(ArenaTeamStyle teamStyle, ArenaTeamType teamType, ArenaTournamentType tournamentType, int numteams, int use_gladiators, int* errval); // returns index into ap->ratings etc

// debug util
void ArenaEventDebugPrint(ArenaEvent* event, char** estr, U32 time);
int ArenaError(char const *fmt, ...);
void EventChangePhase(ArenaEvent* event, ArenaPhase phase);

// memory handling
ArenaEventList* ArenaEventListCreate(void);
void ArenaEventListDestroy(ArenaEventList* ae);
void ArenaEventListDestroyContents(ArenaEventList* ae);
ArenaEvent* ArenaEventCreate(void);
void ArenaEventDestroy(ArenaEvent* ae);
void ArenaEventDestroyContents(ArenaEvent* ae);
ArenaParticipant* ArenaParticipantCreate(void);
void ArenaParticipantDestroy(ArenaParticipant* ap);
ArenaSeating* ArenaSeatingCreate(void);
void ArenaSeatingDestroy(ArenaSeating* ap);
ArenaRef* ArenaRefCreate(void);
void ArenaRefDestroy(ArenaRef* ap);
ArenaRankingTableEntry* ArenaRankingTableEntryCreate(void);
void ArenaRankingTableEntryDestroy(ArenaRankingTableEntry* entry);
ArenaRankingTable* ArenaRankingTableCreate(void);
void ArenaRankingTableDestroy(ArenaRankingTable* table);
EventHistory* EventHistoryCreate(void);
void EventHistoryDestroy(EventHistory* hist);
EventHistoryEntry* EventHistoryEntryCreate(void);
void EventHistoryEntryDestroy(EventHistoryEntry* entry);
ArenaPlayer* ArenaPlayerCreate(void);
void ArenaPlayerDestroy(ArenaPlayer* entry);

#endif // ARENASTRUCT_H