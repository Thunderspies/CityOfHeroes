/*\
*
*    ArenaRanking.h/c - Copyright 2004, 2005 Cryptic Studios
*        All Rights Reserved
*        Confidential property of Cryptic Studios
*
*    Server logic for ranking of events, pairing swiss draw events, 
*  and prize distribution.  
*
 */

#include "arenaevent.h"
#include "arenaranking.h"
#include "arenaplayer.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/EString.h>
#include "entity/entity.h"
#include <utilitieslib/utils/log.h>

static int ratinglookup[49] =
{0, 7, 14, 21, 29, 36, 43, 50, 57, 65, 72, 80, 87, 95, 102, 110, 117, 125, 133, 141, 149, 158, 166, 175, 184, 193,
202, 211, 220, 230, 240, 251, 262, 273, 284, 296, 309, 322, 336, 383, 401, 422, 444, 470, 501, 538, 589, 677, 700};

static int recursion_limit = 0;
static int g_use_deathtime = 0;
#define ARENA_MAX_RECURSIONS    5000

ArenaRankingTable g_leaderboard[ARENA_NUM_RATINGS];

static int compareEventHistoryEntry(const EventHistoryEntry** a, const EventHistoryEntry** b)
{
    const EventHistoryEntry* l = *a;
    const EventHistoryEntry* r = *b;

    if(l->SEpoints != r->SEpoints)
        return r->SEpoints - l->SEpoints;

    if(l->points != r->points)
        return r->points - l->points;

    if(g_use_deathtime)    // no deathtime means participant didn't die in a last man standing event -> won
    {
        if(l->deathtime && !r->deathtime)
            return 1;
        else if(r->deathtime && !l->deathtime)
            return -1;
        else
            return r->deathtime - l->deathtime;
    }

    if(l->dropped != r->dropped)
        return l->dropped - r->dropped;

    if(l->seed && r->seed)
        return l->seed - r->seed;
    else if(l->seed || r->seed)    // sides with SE seeds are always higher than those without
        return r->seed - l->seed;

    if(ABS(l->opp_AvgPts - r->opp_AvgPts) > 0.00001)
        return (r->opp_AvgPts - l->opp_AvgPts) * 10000;

    if(ABS(r->opp_opp_AvgPts - l->opp_opp_AvgPts) > 0.00001)
        return (r->opp_opp_AvgPts - l->opp_opp_AvgPts) * 10000;

    return r->totalkills - l->totalkills;
}

static int compareArenaRankingTableEntry(const ArenaRankingTableEntry** a, const ArenaRankingTableEntry** b)
{
    const ArenaRankingTableEntry* l = *a;
    const ArenaRankingTableEntry* r = *b;

    if(l->rank != r->rank)
        return l->rank - r->rank;

    if(g_use_deathtime)    // no deathtime means participant didn't die in a last man standing event -> won
    {
        if(l->deathtime && !r->deathtime)
            return 1;
        else if(r->deathtime && !l->deathtime)
            return -1;
        else
            return r->deathtime - l->deathtime;
    }

    if(l->kills != r->kills)
        return r->kills - l->kills;

    return 1;
}

// Swiss draw ranking and pairing
ArenaRankingTable* EventProcessRankings(ArenaEvent* event)
{
    ArenaRankingTable* table = ArenaRankingTableCreate();
    ArenaRankingTableEntry* tableentry;
    EventHistory *hist = EventHistoryCreate();
    EventHistoryEntry** sides = NULL;
    EventHistoryEntry* temp = NULL;
    ArenaParticipant* part;
    int i, j, k, curSide;
    bool oneplayerperteam = true;
    bool haveSEpoints = false;

    if(isDevelopmentMode())
    {
        for(i = 0; i < eaSize(&event->participants); i++)
        {
            if(event->participants[i]->side == 0)
            {
//                ArenaError("Side 0 found");
            }
        }
    }

    strncpy(table->desc,event->description,ARENA_DESCRIPTION_LEN);
    table->teamStyle = event->teamStyle;
    table->teamType = event->teamType;
    table->victoryType = event->victoryType;
    table->victoryValue = event->victoryValue;
    table->weightClass = event->weightClass;

    for(i = 0; i < eaSize(&event->participants); i++)
    {
        part = event->participants[i];
        curSide = part->side;

        if(!hist->sidetable[curSide])
        {
            temp = EventHistoryEntryCreate();
            eaCreate(&temp->p);
            eaPush(&temp->p, part);
            temp->side = curSide;
            temp->roundlastfloated = part->roundlastfloated;
            temp->seed = part->seed;
            temp->dropped = part->dropped;
            temp->deathtime = part->death_time[event->round];

            temp->numplayers = 1;
            if(!event->participants[i]->provisional)
            {
                temp->totalrating = part->rating;
            }
            else
            {
                temp->numprov = 1;
                temp->totalprovrating = part->rating;
            }

            hist->sidetable[curSide] = temp;
            eaPush(&sides, temp);
        }
        else
        {
            eaPush(&hist->sidetable[curSide]->p, part);
            oneplayerperteam = false;
            hist->sidetable[curSide]->numplayers++;
            if(!part->provisional)
            {
                hist->sidetable[curSide]->totalrating += part->rating;
            }
            else
            {
                hist->sidetable[curSide]->totalprovrating += part->rating;
                hist->sidetable[curSide]->numprov++;
            }

            if(hist->sidetable[curSide]->dropped)
            {
                if(!part->dropped)
                    hist->sidetable[curSide]->dropped = 0;
            }
            if(part->death_time[event->round] > hist->sidetable[curSide]->deathtime)
                hist->sidetable[curSide]->deathtime = part->death_time[event->round];
        }

        for(j = 1; j <= event->round; j++)
            temp->totalkills += part->kills[j];
    }

    for(i = 0; i < eaSize(&sides); i++)
    {
        for(j = 1; j <= event->round; j++)    // rounds start at 1
        {
            if(sides[i]->p[0]->seats[j])
            {
                // find my opponent in round j
                if(!sides[i]->pastOpponents[j])
                {
                    for(k = i+1; k < eaSize(&sides); k++)
                    {
                        if(sides[i]->p[0]->seats[j] == sides[k]->p[0]->seats[j])
                        {
                            sides[i]->pastOpponents[j] = sides[k]->side;
                            sides[k]->pastOpponents[j] = sides[i]->side;
                        }
                    }
                    if(!sides[i]->pastOpponents[j])    // was rewarded a bye
                    {
                        sides[i]->pastOpponents[j] = 0;
                        sides[i]->hashadbye = true;
                    }
                }

                // did I win in round j?
                if(event->seating[sides[i]->p[0]->seats[j]]->winningside != 0)
                {
                    sides[i]->gamesplayed++;
                    if(event->seating[sides[i]->p[0]->seats[j]]->winningside == sides[i]->side)
                    {
                        sides[i]->wins++;
                        if(event->roundtype[j])
                        {
                            haveSEpoints = true;
                            sides[i]->SEpoints += ARENA_WIN_POINTS;
                        }
                        else
                            sides[i]->points += ARENA_WIN_POINTS;
                    }
                    else if(event->seating[sides[i]->p[0]->seats[j]]->winningside == -1)
                    {
                        sides[i]->draws++;
                        if(event->roundtype[j])
                        {
                            haveSEpoints = true;
                            sides[i]->SEpoints += ARENA_DRAW_POINTS;
                        }
                        else
                            sides[i]->points += ARENA_DRAW_POINTS;
                    }
                    else
                    {
                        sides[i]->losses++;
                    }
                }
            }
        }
        if(sides[i]->gamesplayed)
            sides[i]->Pts = (F32)sides[i]->points / ARENA_POINT_DIVISOR;
        else
            sides[i]->Pts = 0;
    }

    // calculate the avg win pct of each side's opponents and total time taken
    for(i = 0; i < eaSize(&sides); i++)
    {
        for(j = 1; j <= event->round; j++)
        {
            if(sides[i]->pastOpponents[j] > 0)    // getting a bye gets you 0 win percentage
                sides[i]->opp_AvgPts += hist->sidetable[sides[i]->pastOpponents[j]]->Pts;
        }

        if(sides[i]->gamesplayed)
            sides[i]->opp_AvgPts /= sides[i]->gamesplayed - (sides[i]->hashadbye ? 1 : 0);
        else
            sides[i]->opp_AvgPts = 0;
    }

    for(i = 0; i < eaSize(&sides); i++)
    {
        for(j = 1; j <= event->round; j++)
        {
            if(sides[i]->pastOpponents[j] > 0)    // getting a bye gets you 0 win percentage
                sides[i]->opp_opp_AvgPts += hist->sidetable[sides[i]->pastOpponents[j]]->opp_AvgPts;
        }

        if(sides[i]->gamesplayed)
            sides[i]->opp_opp_AvgPts /= sides[i]->gamesplayed - (sides[i]->hashadbye ? 1 : 0);
        else
            sides[i]->opp_opp_AvgPts = 0;
    }

    if(event->victoryType == ARENA_LASTMAN ||
        event->victoryType == ARENA_TEAMSTOCK || event->victoryType == ARENA_STOCK)
        g_use_deathtime = 1;

    eaQSort(sides, compareEventHistoryEntry);

    for(i = 0; i < eaSize(&sides); i++)
    {
        for(j = 0; j < eaSize(&sides[i]->p); j++)
        {
            tableentry = ArenaRankingTableEntryCreate();

            tableentry->side = sides[i]->side;
            tableentry->dbid = sides[i]->p[j]->dbid;

            for(k = 1; k <= event->round; k++)
            {
                tableentry->kills += sides[i]->p[j]->kills[k];
            }

            if(g_use_deathtime)
                tableentry->deathtime = sides[i]->p[j]->death_time[event->round];
            else
                tableentry->deathtime = -1;

            if(event->tournamentType != ARENA_TOURNAMENT_NONE)
            {
                tableentry->rank = i+1;
                tableentry->pts = sides[i]->Pts;
                tableentry->played = sides[i]->gamesplayed;
                tableentry->SEpts = haveSEpoints ? (float)sides[i]->SEpoints / ARENA_POINT_DIVISOR : -1;
                tableentry->opponentAvgPts = sides[i]->opp_AvgPts;
                tableentry->opp_oppAvgPts = sides[i]->opp_opp_AvgPts;
            }
            else
            {
                tableentry->rank = i + 1;
                tableentry->pts = -1;
                tableentry->played = -1;
                tableentry->SEpts = -1;
                tableentry->opponentAvgPts = -1;
                tableentry->opp_oppAvgPts = -1;
            }

            if(event->sanctioned)
                tableentry->rating = sides[i]->p[j]->rating;
            else
                tableentry->rating = -1;

            eaPush(&table->entries, tableentry);
        }
    }

    eaQSort(table->entries, compareArenaRankingTableEntry);

    g_use_deathtime = 0;

    if(event->rankings)
        ArenaRankingTableDestroy(event->rankings);
    if(event->history)
        EventHistoryDestroy(event->history);

    event->rankings = table;
    event->history = hist;
    event->history->sides = sides;

    return table;
}

#define CHECK_AND_RECURSE                                                                \
    valid_opponent = true;                                                                \
    for(j = 0; j < event->round && valid_opponent; j++)                                    \
{                                                                                    \
    if(sides[pairidx]->pastOpponents[j] == sides[i]->side)                            \
    valid_opponent = false;                                                        \
}                                                                                    \
    if(valid_opponent)                                                                    \
{                                                                                    \
    sides[pairidx]->nextOpponent = sides[i]->side;                                    \
    sides[i]->nextOpponent = sides[pairidx]->side;                                    \
    \
    if(downpairing)                                                                    \
    eaSwap(&sides, pairidx + groupsize/2, i);                                \
        else                                                                            \
        eaSwap(&sides, pairidx - groupsize/2, i);                                \
        \
        valid_opponent = EventPairGroup(event, sides, startidx,                            \
        downpairing ? pairidx+1 : pairidx-1, groupsize, downpairing, mediangroup);    \
        \
        if(!valid_opponent)                                                                \
{                                                                                \
    if(downpairing)                                                                \
    eaSwap(&sides, pairidx + groupsize/2, i);                            \
            else                                                                        \
            eaSwap(&sides, pairidx - groupsize/2, i);                            \
            \
            sides[pairidx]->nextOpponent = sides[i]->nextOpponent = 0;                    \
}                                                                                \
}

bool EventPairGroup(ArenaEvent* event, EventHistoryEntry** sides, int startidx,
                    int pairidx, int groupsize, int downpairing, int mediangroup)
{
    int i, j;
    bool valid_opponent = false;

    if(recursion_limit > ARENA_MAX_RECURSIONS)
    {
        ArenaError("Reached recursion limit");
        return false;
    }
    recursion_limit++;

    if(downpairing && (pairidx < startidx || pairidx >= startidx + groupsize) ||
        !downpairing && (pairidx > startidx || pairidx <= startidx - groupsize))
        return false;

    if(sides[pairidx]->nextOpponent)    // already have an opponent
    {
        if(downpairing)
        {
            if(pairidx + 1 < startidx + groupsize)
                return EventPairGroup(event, sides, startidx, pairidx+1, groupsize, downpairing, mediangroup);
        }
        else
        {
            if(pairidx - 1 > startidx - groupsize)
                return EventPairGroup(event, sides, startidx, pairidx-1, groupsize, downpairing, mediangroup);
        }
        return true;
    }

    if((downpairing && pairidx == startidx + groupsize - 1) ||
        (!downpairing && pairidx == startidx - groupsize + 1))    // needs to float
    {
        if(mediangroup)
            return false;

        if(!sides[pairidx]->roundlastfloated || groupsize == 1 || sides[pairidx]->roundlastfloated < event->round - 1)
        {
            sides[pairidx]->roundlastfloated = event->round;
            if(downpairing)
                sides[pairidx]->group++;
            else
                sides[pairidx]->group--;
            return true;
        }

        return false;    // not allowed to float two rounds in a row
    }

    if(downpairing)
    {
        for(i = pairidx + groupsize/2; i < startidx + groupsize && !valid_opponent; i++)
        {
            CHECK_AND_RECURSE
        }

        for(i = startidx + groupsize/2 - 1; i > pairidx && !valid_opponent; i--)
        {
            CHECK_AND_RECURSE
        }
    }
    else    // for uppairing, the startidx is the *highest* index in the team
    {
        for(i = pairidx - groupsize/2; i > startidx - groupsize && !valid_opponent; i--)
        {
            CHECK_AND_RECURSE
        }

        for(i = startidx - groupsize/2 + 1; i < pairidx && !valid_opponent; i++)
        {
            CHECK_AND_RECURSE
        }
    }

    if(!valid_opponent && !mediangroup && !sides[i]->haveswitched)    // volunteer to float
    {
        if(downpairing)
        {
            sides[pairidx]->haveswitched = 1;
            eaSwap(&sides, pairidx, startidx + groupsize - 1);
            valid_opponent = EventPairGroup(event, sides, startidx, pairidx, groupsize, 1, 0);
            if(!valid_opponent)
                eaSwap(&sides, pairidx, startidx + groupsize - 1);
        }
        else
        {
            sides[pairidx]->haveswitched = 1;
            eaSwap(&sides, pairidx, startidx - groupsize + 1);
            valid_opponent = EventPairGroup(event, sides, startidx, pairidx, groupsize, 0, 0);
            if(!valid_opponent)
                eaSwap(&sides, pairidx, startidx - groupsize + 1);
        }
    }

    return valid_opponent;
}

// all places this gets used had the same variables... convenient :)
#define CHECK_PAST_OPPONENTS                                                            \
    haveopponent = true;                                                                \
    for(k = 1; k < event->round; k++)                                                    \
{                                                                                    \
    if(sides[i]->pastOpponents[k] == sides[j]->side)                                \
{                                                                                \
    haveopponent = false;                                                        \
    break;                                                                        \
}                                                                                \
}

#define CHECK_VALID_GROUPS                                    \
    for(q = 0; q < eaSize(&sides)-1; q++)            \
    assert(sides[q]->group <= sides[q+1]->group);            \

#define UPDATE_PARTICIPANTS                                                                        \
    for(i = 0; i < eaSize(&event->participants); i++)                                    \
    {                                                                                            \
        ArenaParticipant *curpart = event->participants[i];                                        \
        if(curpart->dropped)                                                                    \
            continue;                                                                            \
        if(event->history->sidetable[curpart->side]->nextSeat < 0 ||                            \
            event->history->sidetable[curpart->side]->nextSeat >= eaSize(&event->seating))    \
        {                                                                                        \
            ArenaError("Invalid nextseat for participant %d in event (%d-%d)",                    \
                i, event->eventid, event->uniqueid);                                            \
        }                                                                                        \
        if(!event->history->sidetable[curpart->side]->nextSeat)                                    \
            curpart->dropped = 1;                                                                \
        else                                                                                    \
        {                                                                                        \
            curpart->seats[event->round] = event->history->sidetable[curpart->side]->nextSeat;    \
            curpart->roundlastfloated = event->history->sidetable[curpart->side]->roundlastfloated;    \
            curpart->seed = event->history->sidetable[curpart->side]->seed;                        \
        }                                                                                        \
    }


void EventCreatePairingsSwissDraw(ArenaEvent* event)
{
    // EventProcessRankings has to be called before this function to insure that sides is up to date and sorted
    EventHistoryEntry** sides = event->history->sides;
    ArenaRoundType roundtype;
    int i, numrounds;

    if(!sides)
        return;

    for(i = eaSize(&sides)-1; i >= 0 && sides[i]->dropped; i--)
    {
        EventHistoryEntryDestroy(eaRemove(&sides, i));
    }

    numrounds = log2(eaSize(&event->participants));    // log2 rounds up
    if(eaSize(&sides) - 1 <= 1)
    {
        roundtype = ARENA_ROUND_DONE;
    }
    if(eaSize(&sides) <= pow(2,ARENA_SINGLE_ELIM_RDS))
    {
        if(event->round <= numrounds)
            roundtype = ARENA_ROUND_SINGLE_ELIM;
        else
            roundtype = ARENA_ROUND_DONE;
    }
    else
    {
        numrounds++;
        if(event->round <= numrounds - ARENA_SINGLE_ELIM_RDS)
            roundtype = ARENA_ROUND_SWISSDRAW;
        else if(event->round <= numrounds)
            roundtype = ARENA_ROUND_SINGLE_ELIM;
        else
            roundtype = ARENA_ROUND_DONE;
    }

    if(!eaSize(&event->seating))    // create a dummy seating so that 0 can mean undefined
    {
        eaPush(&event->seating, ArenaSeatingCreate());
        event->seating[0]->round = -1;
        event->seating[0]->winningside = 0;
    }

    if(roundtype == ARENA_ROUND_DONE)
        return;
    else if(roundtype == ARENA_ROUND_SINGLE_ELIM)
    {
        event->roundtype[event->round] = 1;
        EventCreatePairingsSingleElim(event, numrounds);
    }
    else if(roundtype == ARENA_ROUND_SWISSDRAW)
    {
        if(event->round > 1)
        {
            int j, k, groupsize, groupstart, numgroups, curgroup, mediangroup, medianstart;
            int curseat;
            int numupfloats = 0, numdownfloats = 0;
            bool haveopponent;
            F32 curpoints;

            //if number of sides is odd, hand out the bye
            if(eaSize(&sides) % 2)
            {
                ArenaSeating *bye;
                i = eaSize(&sides)-1;

                while(i >= 0 && sides[i]->hashadbye)
                    i--;

                bye = ArenaSeatingCreate();
                bye->round = event->round;
                bye->winningside = sides[i]->side;
                sides[i]->nextSeat = eaPush(&event->seating, bye);
                sides[i]->nextOpponent = -1;
                eaPush(&sides, eaRemove(&sides, i));
            }

            for(i = 0, curpoints = sides[0]->points, numgroups = 0; i < eaSize(&sides); i++)
            {
                if(sides[i]->points == curpoints)
                    sides[i]->group = numgroups;
                else
                {
                    curpoints = sides[i]->points;
                    sides[i]->group = ++numgroups;
                }
            }

            curgroup = -1;
            // see if everyone has a valid opponent in their group
            for(i = 0; sides[i]->group < numgroups / 2; i++)
            {
                if(curgroup != sides[i]->group)
                {
                    numdownfloats = 0;
                    curgroup = sides[i]->group;
                    groupstart = i;
                    groupsize = 1;
                    while(i+groupsize < eaSize(&sides) && sides[i+groupsize]->group == curgroup)
                        groupsize++;
                }

                haveopponent = false;
                for(j = groupstart; sides[j]->group == curgroup && !haveopponent; j++)
                {
                    if(i != j)
                    {
                        CHECK_PAST_OPPONENTS;
                    }
                }

                // float ppl who don't have an opponent down
                if(!haveopponent && sides[i]->roundlastfloated != (event->round - 1))
                {
                    sides[i]->group++;
                    sides[i]->roundlastfloated = event->round;
                    eaSwap(&sides, i, groupstart + groupsize - 1);
                    groupsize--;
                    i = groupstart - 1;
                    numdownfloats++;
                    continue;
                }
                // try to pair the group if this is the last guy
                if(sides[i+1]->group != curgroup)
                {
                    recursion_limit = 0;
                    while(!EventPairGroup(event, sides, groupstart, groupstart, groupsize, 1, 0))
                    {
                        if(groupsize)
                        {
                            sides[i]->group++;
                            groupsize--;
                            i--;
                            numdownfloats++;
                        }
                        else
                            break;
                    }

                    if(groupsize && sides[i]->group != curgroup)
                    {
                        i--;
                        numdownfloats++;
                    }
                }
            }

            medianstart = i;
            mediangroup = sides[i]->group;

            curgroup = -1;

            i = eaSize(&sides) - 1;
            if(i >= 0 && sides[i]->nextOpponent == -1)    // check for bye
                i--;

            for(; sides[i]->group > mediangroup; i--)
            {
                if(curgroup != sides[i]->group)
                {
                    numupfloats = 0;
                    curgroup = sides[i]->group;
                    groupstart = i;
                    groupsize = 1;
                    while(i-groupsize > 0 && sides[i-groupsize]->group == curgroup)
                        groupsize++;
                }

                haveopponent = false;
                for(j = groupstart; sides[j]->group == curgroup && !haveopponent; j--)
                {
                    if(i != j)
                    {
                        CHECK_PAST_OPPONENTS;
                    }
                }

                // float ppl who don't have an opponent up
                if(!haveopponent && sides[i]->roundlastfloated != (event->round - 1))
                {
                    sides[i]->group--;
                    sides[i]->roundlastfloated = event->round;
                    numupfloats++;
                    eaSwap(&sides, i, groupstart - groupsize + 1);
                    groupsize--;
                    i = groupstart + 1;
                    numupfloats++;
                    continue;
                }
                // try to pair the group if this is the last guy
                if(groupsize && sides[i-1]->group != curgroup)
                {
                    recursion_limit = 0;
                    while(!EventPairGroup(event, sides, groupstart, groupstart, groupsize, 0, 0))
                    {
                        if(groupsize)
                        {
                            sides[i]->group--;
                            groupsize--;
                            i++;
                            numupfloats++;
                        }
                        else
                            break;
                    }

                    if(groupsize && sides[i]->group != curgroup)
                    {
                        i++;
                        numupfloats++;
                    }
                }
            }

            i = medianstart;
            groupsize = 0;
            while(i + groupsize < eaSize(&sides) && sides[i+groupsize]->group == mediangroup)
                groupsize++;

            for(; i < eaSize(&sides) && sides[i]->group == mediangroup; i++)
            {
                haveopponent = false;
                for(j = medianstart; j < eaSize(&sides) && sides[j]->group == mediangroup && !haveopponent; j++)
                {
                    CHECK_PAST_OPPONENTS;
                }

                // if this player does not have an opponent, start unwrapping the surrounding groups until he does
                if(!haveopponent)
                {
                    if(numupfloats < numdownfloats)
                    {
                        for(j = medianstart + groupsize; j < eaSize(&sides) && !haveopponent; j++)
                        {
                            CHECK_PAST_OPPONENTS;
                        }
                        for(j = medianstart - 1; j >= 0 && !haveopponent; j--)
                        {
                            CHECK_PAST_OPPONENTS;
                        }
                    }
                    else
                    {
                        for(j = medianstart - 1; j >= 0 && !haveopponent; j--)
                        {
                            CHECK_PAST_OPPONENTS;
                        }
                        for(j = medianstart + groupsize; j < eaSize(&sides) && !haveopponent; j++)
                        {
                            CHECK_PAST_OPPONENTS;
                        }
                    }

                    if(j > medianstart)
                    {
                        numupfloats += 2;
                        k = medianstart + groupsize;
                        while(k < eaSize(&sides) && sides[j]->nextOpponent != sides[k]->side)
                            k++;
                    }
                    else
                    {
                        numdownfloats += 2;
                        k = medianstart - 1;
                        while(k >= 0 && sides[j]->nextOpponent != sides[k]->side)
                            k--;
                    }

                    sides[k]->group = sides[j]->group = mediangroup;
                    sides[k]->nextOpponent = sides[j]->nextOpponent = 0;

                    eaMove(&sides, medianstart + groupsize-1, j);
                    eaMove(&sides, medianstart + groupsize, k);
                    if(j < medianstart)
                    {
                        medianstart -= 2;
                        i -= 2;
                    }
                    groupsize += 2;
                }
                if(i == eaSize(&sides)-1 || sides[i+1]->side != mediangroup)
                {
                    recursion_limit = 0;
                    while(!EventPairGroup(event, sides, medianstart, medianstart, groupsize, 1, 1))
                    {
                        for(j = medianstart; sides[j]->group == mediangroup; j++)
                            sides[j]->nextOpponent = 0;

                        if(numupfloats > numdownfloats)
                        {
                            numdownfloats += 2;
                            j = medianstart - 1;
                            k = j - 1;
                            while(k >= 0 && j >= 0 &&
                                sides[j]->nextOpponent != sides[k]->side)
                                k--;
                        }
                        else
                        {
                            numupfloats += 2;
                            j = medianstart + groupsize;
                            k = j + 1;
                            while(k < eaSize(&sides) && j < eaSize(&sides) &&
                                sides[j]->nextOpponent != sides[k]->side)
                                k++;
                        }

                        if(k >= 0 && k < eaSize(&sides) && j >= 0 && j < eaSize(&sides))
                        {
                            sides[k]->group = sides[j]->group = mediangroup;
                            sides[k]->nextOpponent = sides[j]->nextOpponent = 0;

                            eaMove(&sides, medianstart + groupsize-1, j);
                            eaMove(&sides, medianstart + groupsize, k);
                            if(j < medianstart)
                            {
                                medianstart -= 2;
                                i -= 2;
                            }

                            groupsize += 2;
                        }
                        recursion_limit = 0;
                    }
                }
            }

            for(i = 0; i < eaSize(&sides); i++)
            {
                if(!sides[i]->nextSeat)
                {
                    ArenaSeating* newseat = ArenaSeatingCreate();

                    newseat->round = event->round;
                    curseat = eaPush(&event->seating, newseat);

                    sides[i]->nextSeat = curseat;
                    event->history->sidetable[sides[i]->nextOpponent]->nextSeat = curseat;
                }
            }
        }
        else // first round
        {
            ArenaSeating* seating;
            int i, curseat, numseats = eaSize(&sides)/2; // this assumes the number of sides is even
            // (i.e. there is a bye-side as the last side if there's an odd number)

            for(i = 0; i < numseats; i++)
            {
                seating = ArenaSeatingCreate();
                seating->round = 1;
                curseat = eaPush(&event->seating, seating);

                sides[i]->nextSeat = curseat;
                sides[i+numseats]->nextSeat = curseat;
            }
        }
    }

    UPDATE_PARTICIPANTS;
}

void EventCreatePairingsSingleElim(ArenaEvent* event, int totalrounds)
{
    EventHistoryEntry** sides = event->history->sides;

    int i, curseat, firstseat, numseats;

    if(!sides)
        return;

    if(!sides[0]->seed)
    {
        for(i = 0; i < pow(2, 1 + totalrounds - event->round) && i < eaSize(&sides); i++)
            sides[i]->seed = i+1;
    }

    numseats = pow(2, 1 + totalrounds - event->round - 1);
    firstseat = eaSize(&event->seating);
    for(i = 0; i < numseats; i++)
    {
        curseat = eaPush(&event->seating, ArenaSeatingCreate());
        event->seating[curseat]->round = event->round;
    }
    for(i = 0; i < 2*numseats && i < eaSize(&sides); i++)
    {
        sides[i]->nextSeat = firstseat + ((sides[i]->seed - 1) % numseats);    // - 1 doesn't really matter but it
    }                                                                    // corrects for the seats starting at 1

    UPDATE_PARTICIPANTS;
}
#undef UPDATE_PARTICIPANTS
#undef CHECK_PAST_OPPONENTS
#undef CHECK_VALID_GROUPS

static int compareLeaderBoardEntry(const ArenaRankingTableEntry** a, const ArenaRankingTableEntry** b)
{
    int retval;
    const ArenaRankingTableEntry* l = *a;
    const ArenaRankingTableEntry* r = *b;

    retval = (int)(1000 * (r->rating - l->rating));

    return retval;
}

void EventInitLeaderList()
{
    int i, j, k;
    ArenaPlayer** players = PlayerGetAll();
    
    for(i = 0; i < ARENA_NUM_RATINGS; i++)
    {
        for(j = 0; j < ARENA_LEADERBOARD_NUM_POS; j++)
        {
            if(!g_leaderboard[i].entries || eaSize(&g_leaderboard[i].entries) < j + 1)
                eaPush(&g_leaderboard[i].entries, ArenaRankingTableEntryCreate());
            if(j < eaSize(&players))
            {
                g_leaderboard[i].entries[j]->dbid = players[j]->dbid;
                g_leaderboard[i].entries[j]->rating = players[j]->ratingsByRatingIndex[i];
            }
        }
        eaQSort(g_leaderboard[i].entries, compareLeaderBoardEntry);
    }

    for(i = eaSize(&players)-1; i >= ARENA_LEADERBOARD_NUM_POS; i--)
    {
        for(j = 0; j < ARENA_NUM_RATINGS; j++)
        {
            if(players[i]->ratingsByRatingIndex[j] > g_leaderboard[j].entries[ARENA_LEADERBOARD_NUM_POS-1]->rating)
            {
                for(k = ARENA_LEADERBOARD_NUM_POS-2; k >= 0 && players[i]->ratingsByRatingIndex[j] > g_leaderboard[j].entries[k]->rating; k--)
                {
                    g_leaderboard[j].entries[k+1]->dbid = g_leaderboard[j].entries[k]->dbid;
                    g_leaderboard[j].entries[k+1]->rating = g_leaderboard[j].entries[k]->rating;
                }
                g_leaderboard[j].entries[k+1]->dbid = players[i]->dbid;
                g_leaderboard[j].entries[k+1]->rating = players[i]->ratingsByRatingIndex[j];
            }
        }
    }
}

void EventUpdateLeaderList(int ratingidx, ArenaPlayer* ap)
{
    int i;
    int tempdbid;
    F32 temprating;

    for(i = 0; i < ARENA_LEADERBOARD_NUM_POS; i++)
    {
        if(ap->dbid == g_leaderboard[ratingidx].entries[i]->dbid)
        {
            if(i && g_leaderboard[ratingidx].entries[i-1]->rating < ap->ratingsByRatingIndex[ratingidx])
            {
                tempdbid = g_leaderboard[ratingidx].entries[i-1]->dbid;
                temprating = g_leaderboard[ratingidx].entries[i-1]->rating;
                g_leaderboard[ratingidx].entries[i-1]->dbid = g_leaderboard[ratingidx].entries[i]->dbid;
                g_leaderboard[ratingidx].entries[i-1]->rating = g_leaderboard[ratingidx].entries[i]->rating;
                g_leaderboard[ratingidx].entries[i]->dbid = tempdbid;
                g_leaderboard[ratingidx].entries[i]->rating = temprating;
                i -= 2;
            }
            else if(i < ARENA_LEADERBOARD_NUM_POS-1 && g_leaderboard[ratingidx].entries[i+1]->rating > ap->ratingsByRatingIndex[ratingidx])
            {
                tempdbid = g_leaderboard[ratingidx].entries[i+1]->dbid;
                temprating = g_leaderboard[ratingidx].entries[i+1]->rating;
                g_leaderboard[ratingidx].entries[i+1]->dbid = g_leaderboard[ratingidx].entries[i]->dbid;
                g_leaderboard[ratingidx].entries[i+1]->rating = g_leaderboard[ratingidx].entries[i]->rating;
                g_leaderboard[ratingidx].entries[i]->dbid = tempdbid;
                g_leaderboard[ratingidx].entries[i]->rating = temprating;
            }
            else if(i == ARENA_LEADERBOARD_NUM_POS-1)
            {
                EventInitLeaderList();
                return;
            }
            else
                return;
        }
    }

    if(ap->ratingsByRatingIndex[ratingidx] > g_leaderboard[ratingidx].entries[ARENA_LEADERBOARD_NUM_POS-1]->rating)
    {
//        free(g_leaderboard[ratingidx].entries[ARENA_LEADERBOARD_NUM_POS-1]->playername);
        for(i = ARENA_LEADERBOARD_NUM_POS-2; i >= 0 && ap->ratingsByRatingIndex[ratingidx] > g_leaderboard[ratingidx].entries[i]->rating; i--)
        {
            g_leaderboard[ratingidx].entries[i+1]->dbid = g_leaderboard[ratingidx].entries[i]->dbid;
            g_leaderboard[ratingidx].entries[i+1]->rating = g_leaderboard[ratingidx].entries[i]->rating;
//            g_leaderboard[ratingidx].entries[i+1]->playername = g_leaderboard[ratingidx].entries[i]->playername;
        }
        g_leaderboard[ratingidx].entries[i+1]->dbid = ap->dbid;
        g_leaderboard[ratingidx].entries[i+1]->rating = ap->ratingsByRatingIndex[ratingidx];
//        strcpy(g_leaderboard[ratingidx].entries[i+1]->playername, dbPlayerNameFromId(ap->dbid));
    }
}

void EventUpdateWinLossCounts(ArenaEvent *event)
{
    int i;
    EventHistoryEntry** sides = event->history->sides;
    ArenaTeamStyle        teamStyle;
    ArenaTeamType        teamType;
    ArenaVictoryType    victoryType;
    ArenaWeightClass    weightClass;
    int eventRatingIndex, ratingerr;

    if (!sides)
    {
        return;
    }

    teamStyle = event->teamStyle;
    teamType = event->teamType;
    victoryType = event->victoryType;
    weightClass = event->weightClass;
    eventRatingIndex = EventGetRatingIndex(teamStyle, teamType, event->tournamentType, event->numsides, event->use_gladiators, &ratingerr);

    for (i = 0; i < eaSize(&event->participants); i++)
    {
        ArenaParticipant *part;
        int curside;
        ArenaPlayer *ap;
        Entity *e;

        part = event->participants[i];
        curside = part->side;
        ap = PlayerGetAdd(part->dbid, 0);
        e = entFromDbId(part->dbid);

        // Ensure totals contain all old rating games
        if (ap)
        {
            int ratedWins = 0, ratedDraws = 0, ratedLosses = 0;
            int ratingIndex;

            for (ratingIndex = 0; ratingIndex < ARENA_NUM_RATINGS; ratingIndex++)
            {
                ratedWins += ap->winsByRatingIndex[ratingIndex];
                ratedDraws += ap->drawsByRatingIndex[ratingIndex];
                ratedLosses += ap->lossesByRatingIndex[ratingIndex];
            }

            if (ap->wins < ratedWins)
            {
                ap->wins = ratedWins;
            }
            if (ap->draws < ratedDraws)
            {
                ap->draws = ratedDraws;
            }
            if (ap->losses < ratedLosses)
            {
                ap->losses = ratedLosses;
            }
        }

        // Update values
        if (ap)
        {
            int wins = event->history->sidetable[curside]->wins;
            int draws = event->history->sidetable[curside]->draws;
            int losses = event->history->sidetable[curside]->losses;
            int eventTypeForStats;

            if (event->use_gladiators)
            {
                eventTypeForStats = 9;
            }
            else if (event->tournamentType == ARENA_TOURNAMENT_SWISSDRAW)
            {
                eventTypeForStats = 1;
            }
            else if (event->teamStyle >= ARENA_STAR && event->teamStyle <= ARENA_7STAR_VERSUS)
            {
                eventTypeForStats = event->teamStyle + ARENA_STAR_STATS_OFFSET;
            }
            else if (event->numsides == 2)
            {
                eventTypeForStats = 0;
            }
            else
            {
                eventTypeForStats = 2;
            }

            ap->wins += wins;
            ap->draws += draws;
            ap->losses += losses;

            ap->winsByEventType[eventTypeForStats] += wins;
            ap->drawsByEventType[eventTypeForStats] += draws;
            ap->lossesByEventType[eventTypeForStats] += losses;

            ap->winsByTeamType[teamType] += wins;
            ap->drawsByTeamType[teamType] += draws;
            ap->lossesByTeamType[teamType] += losses;

            ap->winsByVictoryType[victoryType] += wins;
            ap->drawsByVictoryType[victoryType] += draws;
            ap->lossesByVictoryType[victoryType] += losses;

            ap->winsByWeightClass[weightClass] += wins;
            ap->drawsByWeightClass[weightClass] += draws;
            ap->lossesByWeightClass[weightClass] += losses;

            if (event->sanctioned && !ratingerr)
            {
                ap->winsByRatingIndex[eventRatingIndex] += wins;
                ap->drawsByRatingIndex[eventRatingIndex] += draws;
                ap->lossesByRatingIndex[eventRatingIndex] += losses;
//                ap->dropsByRatingIndex[eventRatingIndex] += part->dropped ? 1 : 0;
            }
            PlayerSave(part->dbid);
        }
    }
}

#define ARENA_RATING_K    10
void EventUpdateRatings(ArenaEvent* event)
{
    int i, j, curside, numplayers = 0, ratingdiff;
    F32 avg = 0, newrating, realPts, expectedPts;
    ArenaPlayer *ap;
    ArenaParticipant *part;
    EventHistoryEntry** sides = event->history->sides;
    bool lowerrating;
    int ratingidx, ratingerr;

    if(!sides)
        return;
    if(!event->sanctioned)
        return;

    ratingidx = EventGetRatingIndex(event->teamStyle, event->teamType, event->tournamentType, event->numsides, event->use_gladiators, &ratingerr);
    if(ratingerr)
        return;

    for(i = 0; i < eaSize(&sides); i++)
    {
        avg += sides[i]->totalrating;
        numplayers += sides[i]->numplayers - sides[i]->numprov;
    }

    if(!numplayers)
    {
        for(i = 0; i < eaSize(&sides); i++)
        {
            avg += sides[i]->totalprovrating;
            numplayers += sides[i]->numprov;
        }
    }

    if(!numplayers)
        return;

    avg /= numplayers;

    for(i = 0; i < eaSize(&event->participants); i++)
    {
        part = event->participants[i];
        curside = part->side;
        newrating = (avg * (numplayers+1) - part->rating) / numplayers;
        ratingdiff = (int) newrating - part->rating;

        if(ratingdiff > 0)
            lowerrating = false;
        else
        {
            lowerrating = true;
            ratingdiff *= -1;
        }

        for(j = 0; ratinglookup[j] > ratingdiff; j++)
            ;
        // 50+/-j% is the chance of winning

        realPts = (F32) event->history->sidetable[curside]->points / ARENA_POINT_DIVISOR;
        expectedPts = ((50.0 + (lowerrating ? -1 : 1) * j) * event->history->sidetable[curside]->gamesplayed / 100);

        if(realPts > 0.0001)
            newrating = part->rating + ARENA_RATING_K * (realPts - expectedPts);
        else
            newrating = part->rating + (ARENA_RATING_K / eaSize(&sides)) * (realPts - expectedPts);

        ap = PlayerGetAdd(part->dbid, 0);
        if (ap)
        {
            LOG_OLD( "ArenaRankings dbid %d, eventid %d, uniqueid %d, teamStyle: %d, teamType: %d, points %f, old ranking %d, new ranking %d",
                part->dbid, event->eventid, event->uniqueid, event->teamStyle, event->teamType, realPts, ap->ratingsByRatingIndex[ratingidx], newrating);
            ap->ratingsByRatingIndex[ratingidx] = newrating;
            if(ap->provisionalByRatingIndex[ratingidx])
            {
                ap->provisionalByRatingIndex[ratingidx] =
                    (ap->provisionalByRatingIndex[ratingidx] + 1) %
                    (ARENA_PROVISIONAL_MATCHES + 1);
            }
            PlayerSave(part->dbid);
            EventUpdateLeaderList(ratingidx, ap);
        }
        else
        {
            LOG_OLD_ERR( __FUNCTION__ " - Could not get player record for dbid %i", part->dbid);
        }
    }
}
#undef ARENA_RATING_K

#define WINS_NEEDED_FOR_GRAND_LANISTA_BADGE 10

void EventDistributePrizes(ArenaEvent* event)
{
    int totalprize = 0;
    int i, winside, runupside, winprize, runupprize;
    ArenaPlayer* ap;

    if(eaSize(&event->history->sides) < 2 || !event->history->sides[0] || !event->history->sides[1] ||
        !event->history->sides[0]->numplayers || !event->history->sides[1]->numplayers)
    {
        LOG_OLD_ERR( "EventDistributePrizes: No winner and/or Runner-up");
        EventDistributeRefunds(event);
        return;
    }

    for(i = eaSize(&event->participants) - 1; i >= 0; i--)
    {
        if(event->participants[i]->paid)
            totalprize += event->entryfee;
    }

    winside = event->history->sides[0]->side;
    // prize per participant
    winprize = max(event->entryfee, (totalprize * ARENA_TOURNAMENT_PRIZE1 / 100) / event->history->sides[0]->numplayers);
    runupside = event->history->sides[1]->side;
    runupprize = (totalprize * ARENA_TOURNAMENT_PRIZE2 / 100) / event->history->sides[1]->numplayers;

    for(i = eaSize(&event->participants) - 1; i >= 0; i--)
    {
        if(event->participants[i]->dbid && !event->participants[i]->dropped &&
            (event->participants[i]->side == winside || event->participants[i]->side == runupside))
        {
            ap = PlayerGetAdd(event->participants[i]->dbid, 0);
            if (ap)
            {
                if(event->participants[i]->side == winside)
                {
                    if( event->use_gladiators ) 
                    {
                        int gladiatorWins = 0;

                        gladiatorWins += ap->winsByRatingIndex[ EventGetRatingIndex(ARENA_FREEFORM, ARENA_SINGLE, ARENA_TOURNAMENT_NONE, 2, 1, NULL) ];
                        gladiatorWins += ap->winsByRatingIndex[ EventGetRatingIndex(ARENA_FREEFORM, ARENA_TEAM,   ARENA_TOURNAMENT_NONE, 2, 1, NULL) ];

                        if( gladiatorWins >= WINS_NEEDED_FOR_GRAND_LANISTA_BADGE )
                            strcpy( ap->arenaReward, "GrandLanista" );
                        else
                            strcpy( ap->arenaReward, "Lanista" );
                    }
                    ap->prizemoney += winprize;
                }
                else
                    ap->prizemoney += runupprize;


                PlayerSave(event->participants[i]->dbid);
            }
        }
    }
}

void EventDistributeRefunds(ArenaEvent* event)
{
    int i;
    ArenaPlayer* ap;

    for(i = eaSize(&event->participants) - 1; i >= 0; i--)
    {
        if(event->participants[i]->dbid && event->participants[i]->paid)
        {
            ap = PlayerGetAdd(event->participants[i]->dbid, 0);
            if (ap)
            {
                ap->prizemoney += event->entryfee;
                PlayerSave(event->participants[i]->dbid);
            }
        }
    }
}
