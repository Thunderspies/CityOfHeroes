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

#include "gameData/arenastruct.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/StringCache.h>
#include "entity/character_base.h"
#include "comm_game.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/error.h>
#include "entity/entVarUpdate.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/log.h>

typedef struct Character Character;

ArenaWeightRange g_weightRanges[ARENA_NUM_WEIGHT_CLASSES] = {
    {    ARENA_WEIGHT_ANY,    0,  60  },
    {    ARENA_WEIGHT_0,        0,    4    },
    {    ARENA_WEIGHT_5,        5,    10    },
    {    ARENA_WEIGHT_10,    11,    12    },
    {    ARENA_WEIGHT_15,    13,    20    },
    {    ARENA_WEIGHT_20,    21,    25    },
    {    ARENA_WEIGHT_25,    26,    30    },
    {    ARENA_WEIGHT_30,    31,    36    },
    {    ARENA_WEIGHT_35,    37,    39    },
    {    ARENA_WEIGHT_40,    40,    45    },
    {    ARENA_WEIGHT_45,    46,    49    },
};

int GetWeightClass(int level)
{
    int i;
    for (i = 1; i < ARENA_NUM_WEIGHT_CLASSES; i++)
    {
        if (g_weightRanges[i].maxLevel >= level) return i;
    }
    return -1;
}

void ArenaKioskSend(Packet* pak, ArenaEventList* list)
{
    int count, i, n;

    count = 0;
    n = eaSize(&list->events);
    for (i = 0; i < n; i++)
        if (list->events[i] && list->events[i]->kiosksend) count++;

    pktSendBitsPack(pak, 1, count);
    for (i = 0; i < n; i++)
    {
        ArenaEvent* event = list->events[i];
        if (!event || !event->kiosksend) continue;
        event->lastupdate_cmd = event->lastupdate_dbid = event->lastupdate_name[0] = 0;    // these fields get cleared often this way
        ArenaEventSend(pak, event, 0);
    }
}

//ArenaKioskReceive clears out list before adding all the incoming events to it
void ArenaKioskReceive(Packet* pak, ArenaEventList* list) {
    int count, i;
    count=pktGetBitsPack(pak,1);
    if (list==NULL)
        return;
    eaDestroyEx(&list->events,ArenaEventDestroy);
    list->events=0;
    for (i = 0; i < count; i++)
    {
        ArenaEvent* event = ArenaEventCreate();
        ArenaEventReceive(pak,event);
        eaPush(&list->events, event);
    }
}

//filters out events that do not match with the tab and radio button selected on the arena kiosk
void EventKioskFilter(ArenaEvent * event, U32 dbid, const char* archetype, int level, int radioButtonFilter, int tabFilter)
{
    int size;
    int availableSlot=false;
    ArenaParticipant* part;

    // don't send down private or unlisted matches
    if (!event) 
        return;

    if( !event->listed || event->inviteonly && radioButtonFilter < ARENA_FILTER_ONGOING  )
    {
        event->kiosksend = 0;
        return;
    }
    event->kiosksend=1;
    event->filter = ARENA_FILTER_INELIGABLE;

    // first check easy mutally exclusive cases
    
    // Player Only
    if (radioButtonFilter == ARENA_FILTER_PLAYER && event->creatorid == 0)
        event->kiosksend=0;

    // Completed
    if (event->phase>ARENA_INTERMISSION)
        event->filter = ARENA_FILTER_COMPLETED;
    else if (radioButtonFilter==ARENA_FILTER_COMPLETED)
        event->kiosksend=0;
        
    // Ongoing
    if (event->phase>=ARENA_STARTROUND && event->phase<=ARENA_INTERMISSION)
        event->filter = ARENA_FILTER_ONGOING;
    else if (radioButtonFilter==ARENA_FILTER_ONGOING)
        event->kiosksend=0;

    // Now best fit/eligable
    size = eaSize(&event->participants);
    if(size >= event->maxplayers)
    {
        int j;
        // find open slots
        for(j = 0; j < size; j++)
        {
            ArenaParticipant * part = event->participants[j];
            if(part->dbid)
                continue;
            if(part->desired_archetype!=NULL && stricmp(archetype,part->desired_archetype)!=0)
                continue;
            availableSlot=true;
        }
    }
    else 
        availableSlot=true;

     if( event->phase <= ARENA_NEGOTIATING && availableSlot )
    {
        if( !event->weightClass || GetWeightClass(level) >= event->weightClass)
            event->filter = ARENA_FILTER_ELIGABLE;
        else if( radioButtonFilter==ARENA_FILTER_ELIGABLE )
            event->kiosksend = 0;

        if( !event->weightClass || GetWeightClass(level) == event->weightClass )
            event->filter = ARENA_FILTER_BEST_FIT;
        else if( radioButtonFilter==ARENA_FILTER_BEST_FIT )
            event->kiosksend = 0;
    }
    else if( radioButtonFilter==ARENA_FILTER_BEST_FIT || radioButtonFilter==ARENA_FILTER_ELIGABLE )
        event->kiosksend = 0;

    // Your scheduled events overrides all
    if( (part = ArenaParticipantFind(event, dbid)) && !part->dropped && !COMPLETED_EVENT(event) )
        event->filter = ARENA_FILTER_MY_EVENTS;
    else if (  radioButtonFilter==ARENA_FILTER_MY_EVENTS  )
        event->kiosksend = 0;

    // filter on tabs now
    if (tabFilter==ARENA_TAB_SOLO) {
        if (event->teamType!=ARENA_SINGLE)
            event->kiosksend=0;
    }
    if (tabFilter==ARENA_TAB_TEAM) {
        if (event->teamType!=ARENA_TEAM)
            event->kiosksend=0;
    }
    if (tabFilter==ARENA_TAB_SG) {
        if (event->teamType!=ARENA_SUPERGROUP)
            event->kiosksend=0;
    }
    if (tabFilter==ARENA_TAB_TOURNY) {
        if (event->tournamentType==ARENA_TOURNAMENT_NONE)
            event->kiosksend=0;
    }
}

void ArenaEventSend(Packet* pak, ArenaEvent* event, int detailed)
{
    int i, j, n;

    pktSendBitsPack(pak, 1, event->eventid);
    pktSendBitsPack(pak, 1, event->uniqueid);
    pktSendBits(pak, ARENATEAMSTYLE_NUMBITS, event->teamStyle);
    pktSendBits(pak, ARENATEAMTYPE_NUMBITS, event->teamType);
    pktSendBits(pak, ARENAWEIGHT_NUMBITS, event->weightClass);
    pktSendBitsPack(pak, ARENAFILTER_NUMBITS, event->filter);
    pktSendBitsAuto(pak, event->tournamentType);

    pktSendBits(pak, ARENAPHASE_NUMBITS, event->phase);
    pktSendBitsPack(pak, 1, event->round);
    pktSendBitsPack(pak, 1, event->start_time);
    pktSendBitsPack(pak, 1, event->leader_ready);

    pktSendBits(pak, 1, event->scheduled);
    pktSendBits(pak, 1, event->sanctioned);
    pktSendBits(pak, 1, event->inviteonly);

    pktSendBits( pak, 1, event->no_observe);

    pktSendBitsPack(pak, 1, event->minplayers);
    pktSendBitsPack(pak, 1, event->maxplayers);
    pktSendBitsPack(pak, 1, event->numplayers);
    pktSendBitsPack(pak, 16, event->entryfee);
    pktSendString(pak, event->description);

    pktSendBitsPack(pak, 1, event->creatorid);
    pktSendString(pak, event->creatorname);

    pktSendBitsPack(pak, 1, event->creatorsg);
    pktSendBitsPack(pak, 1, event->othersg);

    pktSendBitsPack(pak, 8, event->mapid );

    if (!event->detailed) 
        detailed = 0;
    pktSendBits(pak, 1, detailed);
    if (detailed)
    {
        pktSendBitsPack(pak, 1, event->lastupdate_dbid);
        pktSendBitsPack(pak, 1, event->lastupdate_cmd);
        pktSendString(pak, event->lastupdate_name);

        pktSendBits(pak, ARENAVICTORYTYPE_NUMBITS, event->victoryType);
        pktSendBits(pak, 16, event->victoryValue);
        pktSendBits(pak, 1, event->custom);
        pktSendBits(pak, 1, event->listed);

        pktSendBitsPack(pak, 1, event->round_start);
        pktSendBitsPack(pak, 1, event->next_round_time);
        pktSendBits(pak, 1, event->verify_sanctioned);
        pktSendBits(pak, 1, event->tactical_start);
        pktSendBits(pak, 1, event->no_end);
        pktSendBits(pak, 1, event->no_pool);
        pktSendBits(pak, 1, event->no_travel);
        pktSendBits(pak, 1, event->no_stealth);
        pktSendBits(pak, 1, event->use_gladiators);
        pktSendBits(pak, 1, event->no_travel_suppression);
        pktSendBits(pak, 1, event->no_diminishing_returns);
        pktSendBits(pak, 1, event->no_heal_decay);
        pktSendBitsAuto(pak, event->inspiration_setting);
        pktSendBits(pak, 1, event->enable_temp_powers);

        pktSendBitsPack(pak, 1, event->numsides);

        pktSendString(pak, event->mapname);
        pktSendString(pak, event->cancelreason);    // not used right now, so out of kiosk update

        pktSendBits( pak, 1, event->cannot_start );
        if( event->cannot_start )
        {
            pktSendString(pak, event->eventproblems[0]);
            pktSendString(pak, event->eventproblems[1]);
            pktSendString(pak, event->eventproblems[2]);
            pktSendString(pak, event->eventproblems[3]);
            pktSendString(pak, event->eventproblems[4]);
        }

        n = eaSize(&event->participants);
        pktSendBitsPack(pak, 1, n);
        for (i = 0; i < n; i++)
        {
            if (event->participants[i]->name)
            {
                pktSendBits(pak, 1, 1);
                pktSendString(pak, event->participants[i]->name);
            }
            else
                pktSendBits(pak, 1, 0);

            pktSendBitsPack(pak, 1, event->participants[i]->dbid);
            pktSendString(pak, event->participants[i]->archetype);
            pktSendBitsPack(pak, 1, event->participants[i]->level);
            pktSendBitsPack(pak, 1, event->participants[i]->sgid);
            pktSendBits(pak, 1, event->participants[i]->sgleader);
            pktSendBits(pak, 1, event->participants[i]->paid);
            pktSendBits(pak, 1, event->participants[i]->dropped);
            pktSendBitsPack(pak, 1, event->participants[i]->side);

            pktSendBits(pak, ARENA_TEAMSIDE_NUMBITS, event->participants[i]->desired_team );
            if (event->participants[i]->desired_archetype)
            {
                pktSendBits(pak, 1, 1);
                pktSendString(pak, event->participants[i]->desired_archetype);
            }
            else
                pktSendBits(pak, 1, 0);

            for (j = 0; j < ARRAY_SIZE(event->participants[i]->seats); j++)
                pktSendBitsPack(pak, 1, event->participants[i]->seats[j]);

            for (j = 0; j < ARRAY_SIZE(event->participants[i]->kills); j++)
                pktSendBitsPack(pak, 1, event->participants[i]->kills[j]);
        }

        n = eaSize(&event->seating);
        pktSendBitsPack(pak, 1, n);
        for (i = 0; i < n; i++)
        {
            pktSendBitsPack(pak, 1, event->seating[i]->mapid);
            pktSendBitsPack(pak, 1, event->seating[i]->round);
            pktSendBitsPack(pak, 1, event->seating[i]->winningside);
        }

        for(i = 0; i < ARRAY_SIZE(event->roundtype); i++)
            pktSendBits(pak, ARENAROUNDTYPE_NUMBITS, event->roundtype[i]);
    } // if detailed
}

void ArenaEventReceive(Packet* pak, ArenaEvent* event)
{
    int detailed, i, j, n;

    ArenaEventDestroyContents(event);

    event->eventid = pktGetBitsPack(pak, 1);
    event->uniqueid = pktGetBitsPack(pak, 1);
    event->teamStyle = pktGetBits(pak, ARENATEAMSTYLE_NUMBITS);
    event->teamType = pktGetBits(pak, ARENATEAMTYPE_NUMBITS);
    event->weightClass = pktGetBits(pak, ARENAWEIGHT_NUMBITS);
    event->filter = pktGetBitsPack(pak, ARENAFILTER_NUMBITS);
    event->tournamentType = pktGetBitsAuto(pak);

    event->phase = pktGetBits(pak, ARENAPHASE_NUMBITS);
    event->round = pktGetBitsPack(pak, 1);
    event->start_time = pktGetBitsPack(pak, 1);
    event->leader_ready = pktGetBitsPack(pak, 1);

    event->scheduled = pktGetBits(pak, 1);
    event->sanctioned = pktGetBits(pak, 1);
    event->inviteonly = pktGetBits(pak, 1);

    event->no_observe = pktGetBits( pak, 1);

    event->minplayers = pktGetBitsPack(pak, 1);
    event->maxplayers = pktGetBitsPack(pak, 1);
    event->numplayers = pktGetBitsPack(pak, 1);
    event->entryfee = pktGetBitsPack(pak, 16);
    strncpyt(event->description, pktGetString(pak), ARRAY_SIZE(event->description));

    event->creatorid = pktGetBitsPack(pak, 1);
    strncpyt(event->creatorname, pktGetString(pak), ARRAY_SIZE(event->creatorname));

    event->creatorsg = pktGetBitsPack(pak, 1);
    event->othersg = pktGetBitsPack(pak, 1);

    event->mapid = pktGetBitsPack(pak, 8);

    detailed = pktGetBits(pak, 1);
    if (detailed)
    {
        event->detailed = 1;

        event->lastupdate_dbid = pktGetBitsPack(pak, 1);
        event->lastupdate_cmd = pktGetBitsPack(pak, 1);
        strncpyt( event->lastupdate_name, pktGetString(pak), ARRAY_SIZE(event->lastupdate_name) );

        event->victoryType = pktGetBits(pak, ARENAVICTORYTYPE_NUMBITS);
        event->victoryValue = pktGetBits(pak, 16);
        event->custom = pktGetBits(pak, 1);
        event->listed = pktGetBits(pak, 1);

        event->round_start                = pktGetBitsPack(pak, 1);
        event->next_round_time            = pktGetBitsPack(pak, 1);
        event->verify_sanctioned        = pktGetBits(pak, 1);
        event->tactical_start            = pktGetBits(pak, 1);
        event->no_end                    = pktGetBits(pak, 1);
        event->no_pool                    = pktGetBits(pak, 1);
        event->no_travel                = pktGetBits(pak, 1);
        event->no_stealth                = pktGetBits(pak, 1);
        event->use_gladiators            = pktGetBits(pak, 1);
        event->no_travel_suppression    = pktGetBits(pak, 1);
        event->no_diminishing_returns    = pktGetBits(pak, 1);
        event->no_heal_decay            = pktGetBits(pak, 1);
        event->inspiration_setting        = pktGetBitsAuto(pak);
        event->enable_temp_powers        = pktGetBits(pak, 1);

        event->numsides = pktGetBitsPack(pak, 1);

        strncpyt(event->mapname, pktGetString(pak), ARRAY_SIZE(event->mapname));
        strncpyt(event->cancelreason, pktGetString(pak), ARRAY_SIZE(event->cancelreason));

        event->cannot_start = pktGetBits(pak, 1);
        if( event->cannot_start )
        {
            strncpyt(event->eventproblems[0], pktGetString(pak), ARRAY_SIZE(event->eventproblems[0]));
            strncpyt(event->eventproblems[1], pktGetString(pak), ARRAY_SIZE(event->eventproblems[1]));
            strncpyt(event->eventproblems[2], pktGetString(pak), ARRAY_SIZE(event->eventproblems[2]));
            strncpyt(event->eventproblems[3], pktGetString(pak), ARRAY_SIZE(event->eventproblems[3]));
            strncpyt(event->eventproblems[4], pktGetString(pak), ARRAY_SIZE(event->eventproblems[4]));
        }

        eaCreate(&event->participants);
        n = pktGetBitsPack(pak, 1);
        for (i = 0; i < n; i++)
        {
            ArenaParticipant* p = ArenaParticipantCreate();
            if (pktGetBits(pak, 1))
                p->name = strdup(pktGetString(pak));
            p->dbid = pktGetBitsPack(pak, 1);
            p->archetype = allocAddString(pktGetString( pak ));
            p->level = pktGetBitsPack(pak, 1);
            p->sgid = pktGetBitsPack(pak, 1);
            p->sgleader = pktGetBits(pak, 1);
            p->paid = pktGetBits(pak, 1);
            p->dropped = pktGetBits(pak, 1);
            p->side = pktGetBitsPack(pak, 1);

            p->desired_team = pktGetBits(pak, ARENA_TEAMSIDE_NUMBITS);
            if (pktGetBits(pak, 1))
                p->desired_archetype = allocAddString(pktGetString(pak));

            for (j = 0; j < ARRAY_SIZE(p->seats); j++)
                p->seats[j] = pktGetBitsPack(pak, 1);
            for (j = 0; j < ARRAY_SIZE(p->kills); j++)
                p->kills[j] = pktGetBitsPack(pak, 1);
            eaPush(&event->participants, p);
        }

        eaCreate(&event->seating);
        n = pktGetBitsPack(pak, 1);
        for (i = 0; i < n; i++)
        {
            ArenaSeating* seat = ArenaSeatingCreate();
            seat->mapid = pktGetBitsPack(pak, 1);
            seat->round = pktGetBitsPack(pak, 1);
            seat->winningside = pktGetBitsPack(pak, 1);
            eaPush(&event->seating, seat);
        }

        for(i = 0; i < ARRAY_SIZE(event->roundtype); i++)
            event->roundtype[i] = pktGetBits(pak, ARENAROUNDTYPE_NUMBITS);
    } // if detailed
}

void ArenaPlayerSend(Packet* pak, ArenaPlayer* ap)
{
    int i;
    
    pktSendBitsPack(pak, 1, ap->dbid);
    pktSendBitsPack(pak, 1, ap->prizemoney);
    pktSendString(pak, ap->arenaReward);

    pktSendBitsPack(pak, 1, ap->wins);
    pktSendBitsPack(pak, 1, ap->draws);
    pktSendBitsPack(pak, 1, ap->losses);
    
    for (i = 0; i < ARENA_NUM_EVENT_TYPES_FOR_STATS; i++)
    {
        pktSendBitsPack(pak, 1, ap->winsByEventType[i]);
        pktSendBitsPack(pak, 1, ap->drawsByEventType[i]);
        pktSendBitsPack(pak, 1, ap->lossesByEventType[i]);
    }
    
    for (i = 0; i < ARENA_NUM_TEAM_TYPES; i++)
    {
        pktSendBitsPack(pak, 1, ap->winsByTeamType[i]);
        pktSendBitsPack(pak, 1, ap->drawsByTeamType[i]);
        pktSendBitsPack(pak, 1, ap->lossesByTeamType[i]);
    }

    for (i = 0; i < ARENA_NUM_VICTORY_TYPES; i++)
    {
        pktSendBitsPack(pak, 1, ap->winsByVictoryType[i]);
        pktSendBitsPack(pak, 1, ap->drawsByVictoryType[i]);
        pktSendBitsPack(pak, 1, ap->lossesByVictoryType[i]);
    }

    for (i = 0; i < ARENA_NUM_WEIGHT_CLASSES; i++)
    {
        pktSendBitsPack(pak, 1, ap->winsByWeightClass[i]);
        pktSendBitsPack(pak, 1, ap->drawsByWeightClass[i]);
        pktSendBitsPack(pak, 1, ap->lossesByWeightClass[i]);
    }

    for (i = 0; i < ARENA_NUM_RATINGS; i++)
    {
        pktSendBitsPack(pak, 1, ap->winsByRatingIndex[i]);
        pktSendBitsPack(pak, 1, ap->drawsByRatingIndex[i]);
        pktSendBitsPack(pak, 1, ap->lossesByRatingIndex[i]);
//        pktSendBitsPack(pak, 1, ap->dropsByRatingIndex[i]);
        pktSendF32(pak, ap->ratingsByRatingIndex[i]);
        pktSendBits(pak, ARENA_PROVISIONAL_BITS, ap->provisionalByRatingIndex[i]);
    }
}

void ArenaPlayerReceive(Packet* pak, ArenaPlayer* ap)
{
    int i;

    ap->dbid = pktGetBitsPack(pak, 1);
    ap->prizemoney = pktGetBitsPack(pak, 1);
    strcpy( ap->arenaReward, pktGetString( pak ) );

    ap->wins = pktGetBitsPack(pak, 1);
    ap->draws = pktGetBitsPack(pak, 1);
    ap->losses = pktGetBitsPack(pak, 1);

    for (i = 0; i < ARENA_NUM_EVENT_TYPES_FOR_STATS; i++)
    {
        ap->winsByEventType[i] = pktGetBitsPack(pak, 1);
        ap->drawsByEventType[i] = pktGetBitsPack(pak, 1);
        ap->lossesByEventType[i] = pktGetBitsPack(pak, 1);
    }

    for (i = 0; i < ARENA_NUM_TEAM_TYPES; i++)
    {
        ap->winsByTeamType[i] = pktGetBitsPack(pak, 1);
        ap->drawsByTeamType[i] = pktGetBitsPack(pak, 1);
        ap->lossesByTeamType[i] = pktGetBitsPack(pak, 1);
    }

    for (i = 0; i < ARENA_NUM_VICTORY_TYPES; i++)
    {
        ap->winsByVictoryType[i] = pktGetBitsPack(pak, 1);
        ap->drawsByVictoryType[i] = pktGetBitsPack(pak, 1);
        ap->lossesByVictoryType[i] = pktGetBitsPack(pak, 1);
    }

    for (i = 0; i < ARENA_NUM_WEIGHT_CLASSES; i++)
    {
        ap->winsByWeightClass[i] = pktGetBitsPack(pak, 1);
        ap->drawsByWeightClass[i] = pktGetBitsPack(pak, 1);
        ap->lossesByWeightClass[i] = pktGetBitsPack(pak, 1);
    }

    for (i = 0; i < ARENA_NUM_RATINGS; i++)
    {
        ap->winsByRatingIndex[i] = pktGetBitsPack(pak, 1);
        ap->drawsByRatingIndex[i] = pktGetBitsPack(pak, 1);
        ap->lossesByRatingIndex[i] = pktGetBitsPack(pak, 1);
//        ap->dropsByRatingIndex[i] = pktGetBitsPack(pak, 1);
        ap->ratingsByRatingIndex[i] = pktGetF32(pak);
        ap->provisionalByRatingIndex[i] = pktGetBits(pak, ARENA_PROVISIONAL_BITS);
    }
}


void ArenaRankingTableSend(Packet * pak,ArenaRankingTable * art) {
    int i;
    int size=eaSize(&art->entries);
    pktSendBitsPack(pak,1,size);
    for (i=0;i<size;i++) {
        pktSendF32(pak,art->entries[i]->opp_oppAvgPts);
        pktSendF32(pak,art->entries[i]->opponentAvgPts);
        pktSendBitsPack(pak,1,art->entries[i]->played);
        pktSendF32(pak,art->entries[i]->pts);
        pktSendF32(pak,art->entries[i]->SEpts);
        pktSendBitsPack(pak,1,art->entries[i]->kills);
        pktSendBitsPack(pak,1,art->entries[i]->side);
        pktSendBitsPack(pak,1,art->entries[i]->rank);
        if (art->entries[i]->playername!=NULL) {
            pktSendBitsPack(pak,1,1);
            pktSendString(pak,art->entries[i]->playername);
        } else
            pktSendBitsPack(pak,1,0);
        pktSendF32(pak,art->entries[i]->rating);
        pktSendBitsPack(pak,1,art->entries[i]->dbid);
        pktSendBitsPack(pak,1,art->entries[i]->deathtime);
    }
    pktSendBitsPack(pak,ARENATEAMSTYLE_NUMBITS,art->teamStyle);
    pktSendBitsPack(pak,ARENATEAMTYPE_NUMBITS,art->teamType);
    pktSendBitsPack(pak,ARENAVICTORYTYPE_NUMBITS,art->victoryType);
    pktSendBitsPack(pak,ARENAWEIGHT_NUMBITS,art->weightClass);
    pktSendString(pak,art->desc);
}

void ArenaRankingTableReceive(Packet * pak,ArenaRankingTable * art) {
    int i=pktGetBitsPack(pak,1);
    while (i--) {
        ArenaRankingTableEntry * arte=ArenaRankingTableEntryCreate();
        arte->opp_oppAvgPts=pktGetF32(pak);
        arte->opponentAvgPts=pktGetF32(pak);
        arte->played=pktGetBitsPack(pak,1);
        arte->pts=pktGetF32(pak);
        arte->SEpts=pktGetF32(pak);
        arte->kills=pktGetBitsPack(pak,1);
        arte->side=pktGetBitsPack(pak,1);
        arte->rank=pktGetBitsPack(pak,1);
        if (pktGetBitsPack(pak,1))
            arte->playername=strdup(pktGetString(pak));
        else
            arte->playername=NULL;
        arte->rating=pktGetF32(pak);
        arte->dbid=pktGetBitsPack(pak,1);
        arte->deathtime=pktGetBitsPack(pak,1);
        eaPush(&art->entries,arte);
    }
    art->teamStyle = pktGetBitsPack(pak,ARENATEAMSTYLE_NUMBITS);
    art->teamType = pktGetBitsPack(pak,ARENATEAMTYPE_NUMBITS);
    art->victoryType = pktGetBitsPack(pak,ARENAVICTORYTYPE_NUMBITS);
    art->weightClass = pktGetBitsPack(pak,ARENAWEIGHT_NUMBITS);
    strcpy( art->desc, pktGetString(pak));
}

void ArenaLeaderBoardSend(Packet* pak, ArenaRankingTable leaderlist[ARENA_NUM_RATINGS])
{
    int i, j;
    for(i = 0; i < ARENA_NUM_RATINGS; i++)
    {
        for(j = 0; j < ARENA_LEADERBOARD_NUM_POS; j++)
        {
            pktSendBitsPack(pak, PKT_BITS_TO_REP_DB_ID, leaderlist[i].entries[j]->dbid);
            pktSendF32(pak, leaderlist[i].entries[j]->rating);
        }
    }
}

void ArenaLeaderBoardReceive(Packet* pak, ArenaRankingTable leaderlist[ARENA_NUM_RATINGS])
{
    int i, j;
    for(i = 0; i < ARENA_NUM_RATINGS; i++)
    {
        for(j = 0; j < ARENA_LEADERBOARD_NUM_POS; j++)
        {
            if(!leaderlist[i].entries || eaSize(&leaderlist[i].entries) < j+1)
                eaPush(&leaderlist[i].entries, ArenaRankingTableEntryCreate());
            leaderlist[i].entries[j]->dbid = pktGetBitsPack(pak, PKT_BITS_TO_REP_DB_ID);
            leaderlist[i].entries[j]->rating = pktGetF32(pak);
        }
    }
}

// *********************************************************************************
//  Misc util
// *********************************************************************************

ArenaParticipant* ArenaParticipantFind(ArenaEvent* event, int dbid)
{
    int i, n;

    if( !event )
        return NULL;

    n = eaSize(&event->participants);
    for (i = 0; i < n; i++)
    {
        if (event->participants[i]->dbid == dbid)
            return event->participants[i];
    }
    return NULL;
}

char* nameFromArenaParticipant(ArenaEvent* event, int dbid)
{
    int i, n;

    n = eaSize(&event->participants);
    for (i = 0; i < n; i++)
    {
        if (event->participants[i]->dbid == dbid)
            return event->participants[i]->name;
    }
    return NULL;
}

// DOESN'T COPY HISTORY
void ArenaEventCopy(ArenaEvent* target, ArenaEvent* source)
{
    int i, n;

    ArenaEventDestroyContents(target);
    memcpy(target, source, sizeof(*source));

    target->participants = NULL;
    n = eaSize(&source->participants);
    if (n)
    {
        eaCreate(&target->participants);
        eaSetSize(&target->participants, n);
        for (i = 0; i < n; i++)
        {
            target->participants[i] = ArenaParticipantCreate();
            memcpy(target->participants[i], source->participants[i], sizeof(*(source->participants[i])));
            target->participants[i]->name = strdup( source->participants[i]->name );
        }
    }

    target->seating = NULL;
    n = eaSize(&source->seating);
    if (n)
    {
        eaCreate(&target->seating);
        eaSetSize(&target->seating, n);
        for (i = 0; i < n; i++)
        {
            target->seating[i] = ArenaSeatingCreate();
            memcpy(target->seating[i], source->seating[i], sizeof(*(source->seating[i])));
        }
    }

    target->history = NULL;
    // DOESN'T COPY HISTORY
}

void ArenaEventCountPlayers(ArenaEvent* event)
{
    int i, n;
    int count = 0;

    n = eaSize(&event->participants);
    for (i = 0; i < n; i++)
    {
        if (event->participants[i]->dbid)
            count++;
    }
    event->numplayers = count;
}

char* ArenaConstructInfoString(int eventid, int seat)
{
    static char buf[100];
    sprintf(buf, "A:%i:%i", eventid, seat);
    return buf;
}

int ArenaDeconstructInfoString(char* mapinfo, int *eventid, int *seat)
{
    return 2 == sscanf(mapinfo, "A:%i:%i", eventid, seat);
}

// *********************************************************************************
//  Parsing enums
// *********************************************************************************

// these are used by debug print functions & scheduled event parsing
StaticDefineInt ParseArenaTeamStyle[] =
{
    DEFINE_INT
    {"Freeform",        ARENA_FREEFORM},
    {"Star",            ARENA_STAR},
    {"VillainStar",        ARENA_STAR_VILLAIN},
    {"VersusStar",        ARENA_STAR_VERSUS},
    {"7Star",            ARENA_7STAR},
    {"7VillainStar",    ARENA_7STAR_VILLAIN},
    {"7VersusStar",        ARENA_7STAR_VERSUS},
    DEFINE_END
};

StaticDefineInt ParseArenaEventTypeForStats[] =
{
    DEFINE_INT
    {"Duel",            0},
    {"SwissDraw",        1},
    {"FreeForAll",        2},
    {"Star",            3},
    {"VillainStar",        4},
    {"VersusStar",        5},
    {"7Star",            6},
    {"7VillainStar",    7},
    {"7VersusStar",        8},
    {"Gladiator",        9},
    DEFINE_END
};

StaticDefineInt ParseArenaTeamType[] =
{
    DEFINE_INT
    {"Single",            ARENA_SINGLE},
    {"Team",            ARENA_TEAM},
    {"Supergroup",        ARENA_SUPERGROUP},
    DEFINE_END
};

StaticDefineInt ParseArenaWeightClass[] =
{
    DEFINE_INT
    {"AnyLevel",        ARENA_WEIGHT_ANY},
    {"StrawWeight",        ARENA_WEIGHT_0},
    {"FlyWeight",        ARENA_WEIGHT_5},
    {"BantamWeight",    ARENA_WEIGHT_10},
    {"FeatherWeight",    ARENA_WEIGHT_15},
    {"LightWeight",        ARENA_WEIGHT_20},
    {"WelterWeight",    ARENA_WEIGHT_25},
    {"MiddleWeight",    ARENA_WEIGHT_30},
    {"CruiserWeight",    ARENA_WEIGHT_35},
    {"HeavyWeight",        ARENA_WEIGHT_40},
    {"SuperHeavyWeight",ARENA_WEIGHT_45},
    DEFINE_END
};

StaticDefineInt ParseArenaVictoryType[] =
{
    DEFINE_INT
    {"LastManStanding",        ARENA_LASTMAN},
    {"Timed",                ARENA_TIMED},
    {"Kills",                ARENA_KILLS},
    {"Stock",                ARENA_STOCK},
    {"TeamStock",            ARENA_TEAMSTOCK},
    DEFINE_END
};

StaticDefineInt ParseArenaTournamentType[] =
{
    DEFINE_INT
    {"None",            ARENA_TOURNAMENT_NONE},
    {"SwissDraw",        ARENA_TOURNAMENT_SWISSDRAW},
    DEFINE_END
};

StaticDefineInt ParseArenaPhase[] =
{
    DEFINE_INT
    {"Scheduled",        ARENA_SCHEDULED},
    {"Negotiating",        ARENA_NEGOTIATING},
    {"StartRound",        ARENA_STARTROUND},
    {"RunRound",        ARENA_RUNROUND},
    {"EndRound",        ARENA_ENDROUND},
    {"Intermission",    ARENA_INTERMISSION},
    {"PayoutWait",        ARENA_PAYOUT_WAIT},
    {"PrizePayout",        ARENA_PRIZE_PAYOUT},
    {"Ended",            ARENA_ENDED},
    {"CancelRefund",    ARENA_CANCEL_REFUND},
    {"Cancelled",        ARENA_CANCELLED},
    DEFINE_END
};

// *********************************************************************************
//  Debug util
// *********************************************************************************

void ArenaEventDebugPrint(ArenaEvent* event, char** estr, U32 time)
{
    int i, nump, p, nums, s;
    char timestr[20];

    // try to infer what kind of time we're dealing with
    #define INFER_TIME(t) (t < 700000? t: (t < time? time-t: t-time))

    ArenaEventCountPlayers(event);
    estrClear(estr);
    estrConcatf(estr, "Event %s (%i/%i)\n", event->description, event->eventid, event->uniqueid);
    estrConcatf(estr, "  Created by %s (%i)\n", event->creatorname, event->creatorid);
    estrConcatf(estr, "  Map %s\n", event->mapname[0]? event->mapname: "(Random)");
    estrConcatf(estr, "  Type %s/%s, %s, %s(%i)\n", 
        StaticDefineIntRevLookup(ParseArenaTeamStyle, event->teamStyle),
        StaticDefineIntRevLookup(ParseArenaTeamType, event->teamType),
        StaticDefineIntRevLookup(ParseArenaWeightClass, event->weightClass),
        StaticDefineIntRevLookup(ParseArenaVictoryType, event->victoryType),
        event->victoryValue);
    estrConcatf(estr, "  Round %i, %s", event->round,
        StaticDefineIntRevLookup(ParseArenaPhase, event->phase));
    estrConcatf(estr, ", Start time: %s",
        timerMakeOffsetStringFromSeconds(timestr, INFER_TIME(event->start_time)));
    estrConcatf(estr, ", Round start: %s", 
        timerMakeOffsetStringFromSeconds(timestr, INFER_TIME(event->round_start)));
    estrConcatf(estr, ", Next Round: %s\n",
        timerMakeOffsetStringFromSeconds(timestr, INFER_TIME(event->next_round_time)));
    estrConcatf(estr, "  Numsides %i, Numplayers %i, Maxplayers %i\n", 
        event->numsides, event->numplayers, event->maxplayers);
    if(event->entryfee)
        estrConcatf(estr, "  Entryfee %i\n", event->entryfee);

    estrConcatStaticCharArray(estr, "  Flags:");
    if (event->scheduled) estrConcatStaticCharArray(estr, " SCHEDULED");
    if (event->sanctioned) estrConcatStaticCharArray(estr, " SANCTIONED");
    if (event->verify_sanctioned) estrConcatStaticCharArray(estr, " VERIFY_SANC");
    if (event->inviteonly) estrConcatStaticCharArray(estr, " INVITE_ONLY");
    estrConcatChar(estr, '\n');

    estrConcatStaticCharArray(estr, "  Rules:");
    if (event->no_travel_suppression) estrConcatStaticCharArray(estr, " NO_TRAVEL_SUPPRESSION");
    if (event->tactical_start) estrConcatStaticCharArray(estr, " TACTICAL_START");
    if (event->no_end) estrConcatStaticCharArray(estr, " NO_END");
    if (event->no_diminishing_returns) estrConcatStaticCharArray(estr, " NO_DIMINISHING_RETURNS");
    if (event->no_heal_decay) estrConcatStaticCharArray(estr, " NO_HEAL_DECAY");
    if (event->no_pool) estrConcatStaticCharArray(estr, " NO_POOL");
    if (event->no_travel) estrConcatStaticCharArray(estr, " NO_TRAVEL");
    //if (event->inspiration_setting) estrConcatStaticCharArray(estr, " NO_INSPIRATION");
    if (event->enable_temp_powers) estrConcatStaticCharArray(estr, " ENABLE_TEMP_POWERS");
    if (event->no_stealth) estrConcatStaticCharArray(estr, " NO_STEALTH");

    estrConcatChar(estr, '\n');

    estrConcatStaticCharArray(estr, "  Sanction problems:");
    for (i = 0; i < ARENA_NUM_PROBLEMS; i++)
    {
        if (event->eventproblems[i][0])
            estrConcatCharString(estr, event->eventproblems[i]);
    }
    estrConcatChar(estr, '\n');

    nump = eaSize(&event->participants);
    estrConcatStaticCharArray(estr, "  Participants:\n");
    for (p = 0; p < nump; p++)
    {
        ArenaParticipant* part = event->participants[p];
        estrConcatf(estr, "    Name: %s (%i)\n", part->name, part->dbid);
        estrConcatf(estr, "      lvl %i, arch %s, side %i %s\n", 
            part->level, part->archetype, part->side, part->dropped ? "Dropped" : "");
        estrConcatf(estr, "      arch %s, side %i\n",
            part->desired_archetype, part->desired_team);

        estrConcatf(estr, "      seats: %i", part->seats[0]);
        for (i = 1; i < ARENA_MAX_ROUNDS; i++)
        {
            if (part->seats[i])
                estrConcatf(estr, ", %i", part->seats[i]);
            else break;
        }
        estrConcatChar(estr, '\n');
        if (part->sgid)
            estrConcatf(estr, "      sgid: %i, leader: %i\n", part->sgid, part->sgleader);
    }
    if (nump == 0)
        estrConcatStaticCharArray(estr, "    (none)\n");

    nums = eaSize(&event->seating);
    estrConcatStaticCharArray(estr, "  Seating:\n");
    for (s = 0; s < nums; s++)
    {
        ArenaSeating* seat = event->seating[s];
        estrConcatf(estr, "    Seat %i, round %i, winning %i, map %i\n",
            s, seat->round, seat->winningside, seat->mapid);
    }
}

int ArenaError(char const *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_va( LOG_ERROR, LOG_LEVEL_DEPRECATED, 0, (char*)fmt, ap );
    va_end(ap);
    return 0;
}

static int EventGetRating(ArenaTeamStyle teamStyle, ArenaTeamType teamType, ArenaTournamentType tournamentType, int numsides, int use_gladiators)
{
    if(teamStyle == ARENA_FREEFORM)
    {
        if(use_gladiators)
        {
            if(teamType == ARENA_SINGLE)
                return 8;
            else if(teamType == ARENA_TEAM)
                return 9;
        }
        else if(tournamentType == ARENA_TOURNAMENT_SWISSDRAW)
        {
            if(teamType == ARENA_SINGLE)
                return 3;
            else if(teamType == ARENA_SUPERGROUP)
                return 4;
        }
        else if (numsides == 2)
        {
            if(teamType == ARENA_SINGLE)
                return 0;
            else if(teamType == ARENA_TEAM)
                return 1;
            else if(teamType == ARENA_SUPERGROUP)
                return 2;
        }
        else
        {
            if(teamType == ARENA_SINGLE)
                return 5;
            else if(teamType == ARENA_TEAM)
                return 6;
        }
    }
    else if(teamStyle == ARENA_STAR)
    {
        return 7;
    }

    return -1;
}

int EventGetRatingIndex(ArenaTeamStyle teamStyle, ArenaTeamType teamType, ArenaTournamentType tournamentType, int numsides, int use_gladiators, int* errval)
{
    int retval = EventGetRating(teamStyle, teamType, tournamentType, numsides, use_gladiators);
    if(retval >= 0 && retval < ARENA_NUM_RATINGS)
    {
        if(errval)
            *errval = 0;
        return retval;
    }
    else
    {
        if(errval)
            *errval = 1;
        return 0;
    }
}

// *********************************************************************************
//  Memory handling for these objects
// *********************************************************************************

MP_DEFINE(ArenaEventList);
MP_DEFINE(ArenaEvent);
MP_DEFINE(ArenaParticipant);
MP_DEFINE(ArenaSeating);
MP_DEFINE(ArenaRef);
MP_DEFINE(ArenaRankingTableEntry);
MP_DEFINE(ArenaRankingTable);
MP_DEFINE(EventHistory);
MP_DEFINE(EventHistoryEntry);
MP_DEFINE(ArenaPlayer);

ArenaEventList* ArenaEventListCreate(void)
{
    MP_CREATE(ArenaEventList, 100);
    return MP_ALLOC(ArenaEventList);
}
void ArenaEventListDestroy(ArenaEventList* al)
{
    eaDestroyEx(&al->events, ArenaEventDestroy);
    MP_FREE(ArenaEventList, al);
}
void ArenaEventListDestroyContents(ArenaEventList* al)
{
    eaDestroyEx(&al->events, ArenaEventDestroy);
}

ArenaEvent* ArenaEventCreate(void)
{
    ArenaEvent *pEvent;
    MP_CREATE(ArenaEvent, 100);

    pEvent = MP_ALLOC(ArenaEvent);
    pEvent->mapid = ARENA_RANDOMMAP_ID;
    return pEvent;
}
void ArenaEventDestroy(ArenaEvent* ae)
{
    ArenaEventDestroyContents(ae);
    MP_FREE(ArenaEvent, ae);
}
void ArenaEventDestroyContents(ArenaEvent* ae)
{
    eaDestroyEx(&ae->participants, ArenaParticipantDestroy);
    eaDestroyEx(&ae->seating, ArenaSeatingDestroy);
    if (ae->history)
    {
        EventHistoryDestroy(ae->history);
        ae->history = NULL;
    }
    if (ae->rankings)
    {
        ArenaRankingTableDestroy(ae->rankings);
        ae->rankings = NULL;
    }
}
ArenaParticipant* ArenaParticipantCreate(void)
{
    MP_CREATE(ArenaParticipant, 100);
    return MP_ALLOC(ArenaParticipant);
}
void ArenaParticipantDestroy(ArenaParticipant* ap)
{
    if (ap->name) free(ap->name);
    MP_FREE(ArenaParticipant, ap);
}
ArenaSeating* ArenaSeatingCreate(void)
{
    MP_CREATE(ArenaSeating, 100);
    return MP_ALLOC(ArenaSeating);
}
void ArenaSeatingDestroy(ArenaSeating* ap)
{
    MP_FREE(ArenaSeating, ap);
}
ArenaRef* ArenaRefCreate(void)
{
    MP_CREATE(ArenaRef, 100);
    return MP_ALLOC(ArenaRef);
}
void ArenaRefDestroy(ArenaRef* ap)
{
    MP_FREE(ArenaRef, ap);
}
ArenaRankingTableEntry* ArenaRankingTableEntryCreate(void)
{
    MP_CREATE(ArenaRankingTableEntry, 100);
    return MP_ALLOC(ArenaRankingTableEntry);
}
void ArenaRankingTableEntryDestroy(ArenaRankingTableEntry* entry)
{
    if(entry->playername)
        free(entry->playername);
    MP_FREE(ArenaRankingTableEntry, entry);
}
ArenaRankingTable* ArenaRankingTableCreate(void)
{
    MP_CREATE(ArenaRankingTable, 100);
    return MP_ALLOC(ArenaRankingTable);
}
void ArenaRankingTableDestroy(ArenaRankingTable* table)
{
    eaDestroyEx(&table->entries, ArenaRankingTableEntryDestroy);
    MP_FREE(ArenaRankingTable, table);
}
EventHistory* EventHistoryCreate(void)
{
    MP_CREATE(EventHistory, 100);
    return MP_ALLOC(EventHistory);
}
void EventHistoryDestroy(EventHistory* hist)
{
    eaDestroyEx(&hist->sides, EventHistoryEntryDestroy);

    MP_FREE(EventHistory, hist);
}
EventHistoryEntry* EventHistoryEntryCreate(void)
{
    MP_CREATE(EventHistoryEntry, 100);
    return MP_ALLOC(EventHistoryEntry);
}
void EventHistoryEntryDestroy(EventHistoryEntry* entry)
{
    eaDestroy(&entry->p);
    MP_FREE(EventHistoryEntry, entry);
}
ArenaPlayer* ArenaPlayerCreate(void)
{
    MP_CREATE(ArenaPlayer, 100);
    return MP_ALLOC(ArenaPlayer);
}
void ArenaPlayerDestroy(ArenaPlayer* entry)
{
    MP_FREE(ArenaPlayer, entry);
}

void ArenaCreatorUpdateSend( ArenaEvent * event, Packet * pak )
{
    pktSendBits( pak, 32, event->eventid );
    pktSendBits( pak, 32, event->uniqueid );

    pktSendBits( pak, ARENATEAMSTYLE_NUMBITS, event->teamStyle);
    pktSendBits( pak, ARENATEAMTYPE_NUMBITS, event->teamType);
    pktSendBits( pak, ARENAWEIGHT_NUMBITS, event->weightClass);
    pktSendBits( pak, ARENAVICTORYTYPE_NUMBITS, event->victoryType);
    pktSendBits( pak, 16, event->victoryValue);
    pktSendBitsAuto(pak, event->tournamentType);
    pktSendBits( pak, 1, event->custom );
    pktSendBits( pak, 1, event->listed );

    pktSendBits( pak, 1, event->verify_sanctioned);

    pktSendBits( pak, 1, event->inviteonly);
    pktSendBits( pak, 1, event->tactical_start);
    pktSendBits( pak, 1, event->no_end);
    pktSendBits( pak, 1, event->no_pool);
    pktSendBits( pak, 1, event->no_travel);
    pktSendBits( pak, 1, event->no_stealth);
    pktSendBits( pak, 1, event->use_gladiators);
    pktSendBits( pak, 1, event->no_travel_suppression);
    pktSendBits( pak, 1, event->no_diminishing_returns);
    pktSendBits( pak, 1, event->no_heal_decay);
    pktSendBitsAuto( pak, event->inspiration_setting);
    pktSendBits( pak, 1, event->enable_temp_powers);
    pktSendBits( pak, 1, event->no_observe);

    pktSendBitsPack( pak, 3, event->numsides);
    pktSendBitsPack( pak, 8, event->maxplayers);
    pktSendBitsPack( pak, 8, event->numplayers);
    pktSendBitsPack( pak, 1, event->leader_ready );
    pktSendBitsPack( pak, 8, event->mapid );
}

void ArenaCreatorUpdateRecieve( ArenaEvent * event, Packet * pak )
{
    event->eventid = pktGetBits( pak, 32 );
    event->uniqueid = pktGetBits( pak, 32 );

    event->teamStyle        = pktGetBits( pak, ARENATEAMSTYLE_NUMBITS);
    event->teamType            = pktGetBits( pak, ARENATEAMTYPE_NUMBITS);
    event->weightClass        = pktGetBits( pak, ARENAWEIGHT_NUMBITS);
    event->victoryType        = pktGetBits( pak, ARENAVICTORYTYPE_NUMBITS);
    event->victoryValue        = pktGetBits( pak, 16 );
    event->tournamentType    = pktGetBitsAuto(pak);
    event->custom            = pktGetBits( pak, 1 );
    event->listed            = pktGetBits( pak, 1 );

    event->verify_sanctioned    = pktGetBits(pak, 1);

    event->inviteonly                = pktGetBits( pak, 1);
    event->tactical_start            = pktGetBits( pak, 1);
    event->no_end                    = pktGetBits( pak, 1);
    event->no_pool                    = pktGetBits( pak, 1);
    event->no_travel                = pktGetBits( pak, 1);
    event->no_stealth                = pktGetBits( pak, 1);
    event->use_gladiators            = pktGetBits( pak, 1);
    event->no_travel_suppression    = pktGetBits( pak, 1);
    event->no_diminishing_returns    = pktGetBits( pak, 1);
    event->no_heal_decay            = pktGetBits( pak, 1);
    event->inspiration_setting        = pktGetBitsAuto(pak);
    event->enable_temp_powers        = pktGetBits( pak, 1);
    event->no_observe                = pktGetBits( pak, 1);

    event->numsides        = pktGetBitsPack( pak, 3);
    event->maxplayers    = pktGetBitsPack( pak, 8);
    event->numplayers    = pktGetBitsPack( pak, 8);
    event->leader_ready = pktGetBitsPack( pak, 1);
    event->mapid        = pktGetBitsPack( pak, 8);

}

void ArenaParticipantUpdateSend( ArenaEvent * event, ArenaParticipant * ap, Packet * pak )
{
    pktSendBits( pak, 32, event->eventid );
    pktSendBits( pak, 32, event->uniqueid );

    pktSendBits( pak, 32, ap->dbid );

    if( ap->dbid )
        pktSendBits( pak, ARENASIDES_NUMBITS, ap->side );
    else
    {
        pktSendBits( pak, 6, ap->id );
        pktSendBits( pak, ARENASIDES_NUMBITS, ap->desired_team );
        if( ap->desired_archetype )
        {
            pktSendBits(pak,1,1);
            pktSendString( pak, ap->desired_archetype );
        }
        else
            pktSendBits(pak,1,0);
    }
}

void ArenaParticipantUpdateRecieve( ArenaEvent * event, ArenaParticipant * ap, Packet * pak )
{
    event->eventid    = pktGetBits( pak, 32);
    event->uniqueid = pktGetBits( pak, 32 );

    ap->dbid        = pktGetBits( pak, 32 );

    if( ap->dbid )
        ap->side        = pktGetBits( pak, ARENASIDES_NUMBITS );
    else
    {
        ap->id                    = pktGetBits( pak, 6 );
        ap->desired_team        = pktGetBits( pak, ARENASIDES_NUMBITS );
        if( pktGetBits(pak,1))
            ap->desired_archetype = allocAddString(pktGetString( pak ));
        else
            ap->desired_archetype = NULL;
    }

}

void ArenaFullParticipantUpdateSend( ArenaEvent *event, Packet * pak )
{
    int i, count = eaSize(&event->participants);

    pktSendBitsPack( pak, 3, count );

    for( i = 0; i < count; i++ )
        ArenaParticipantUpdateSend( event, event->participants[i], pak );
}

void ArenaFullParticipantUpdateRecieve( ArenaEvent * event, Packet * pak )
{
    int i, count = pktGetBitsPack(pak, 3);

    if( !event->participants )
        eaCreate(&event->participants);

    for( i = 0; i < count; i++ )
    {
        ArenaParticipant * ap;

        if( i < eaSize(&event->participants) )
            ap = event->participants[i];
        else
        {
            ap = ArenaParticipantCreate();
            eaPush( &event->participants, ap );
        }

        ArenaParticipantUpdateRecieve( event, ap, pak );
    }
}

// *********************************************************************************
//  Arena map list
// *********************************************************************************

ArenaMaps g_arenamaplist;

TokenizerParseInfo ParseArenaMap[] =
{
    { "MapDisplayName",    TOK_STRUCTPARAM|TOK_STRING(ArenaMap, displayName, 0) },
    { "MapName",        TOK_STRUCTPARAM|TOK_STRING(ArenaMap, mapname, 0) },
    { "MinPlayers",        TOK_STRUCTPARAM|TOK_INT(ArenaMap, minplayers, 0) },
    { "MaxPlayers",        TOK_STRUCTPARAM|TOK_INT(ArenaMap, maxplayers, 0) },
    { "\n",                TOK_END, 0 },
    { 0 }
};

TokenizerParseInfo ParseArenaMaps[] =
{
    { "{",            TOK_START,       0 },
    { "map",        TOK_STRUCT(ArenaMaps, maps, ParseArenaMap) },
    { "}",            TOK_END,         0 },
    { 0 }
};

void arenaLoadMaps()
{
    if (!g_arenamaplist.maps)
    {
        ParserLoadFiles(NULL, "defs/generic/arenamaps.def", "arenamaps.bin",
            0, ParseArenaMaps, &g_arenamaplist, NULL, NULL, NULL);
    }

// No need to silently abort in the beaconizer because it doesn't use this
// list anyway.
#if !defined(BEACONIZER)
    if(!g_arenamaplist.maps)
    {
        ArenaError("Error loading mapnames\n");
    }
#endif
}

void ArenaSelectMap(ArenaEvent* event)
{
    int i, mapCount;
    static ArenaMap** maps = 0;

    if (!maps) 
        eaCreate(&maps);
    eaSetSize(&maps, 0);

    mapCount = eaSize(&g_arenamaplist.maps);
    if (!mapCount) 
        return;

    if (event->mapid != ARENA_RANDOMMAP_ID && event->mapid >=0 && event->mapid <= mapCount)
    {
        strcpy(event->mapname, g_arenamaplist.maps[event->mapid]->mapname);
    } else {
        for (i = 0; i < mapCount; i++)
        {
            ArenaMap* map = g_arenamaplist.maps[i];
            if (event->tournamentType == ARENA_TOURNAMENT_SWISSDRAW && event->teamType == ARENA_SINGLE)
            {
                if (map->minplayers <= 2 && map->maxplayers >= 2)
                {
                    eaPush(&maps, map);
                }
            }
            else
            {
                if (map->minplayers <= event->numplayers && map->maxplayers >= event->numplayers)
                {
                    eaPush(&maps, map);
                }
            }
        }

        mapCount = eaSize(&maps);
        if (!mapCount)
        {
            strcpy(event->mapname, g_arenamaplist.maps[0]->mapname);
            return;
        }
        i = rand() * mapCount / (RAND_MAX+1);
        strcpy(event->mapname, maps[i]->mapname);
    }
}