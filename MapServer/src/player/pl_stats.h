/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef PL_STATS_H__
#define PL_STATS_H__

#ifndef STATS_BASE_H__
#include "stats_base.h"
#endif

typedef struct PlayerStats
{
    int *cats[kStat_Count];
} PlayerStats;

typedef struct Entity Entity;
typedef struct EntPlayer EntPlayer;
typedef struct ClientLink ClientLink;

void stat_Init(EntPlayer *pl);
void stat_Free(EntPlayer *pl);
    // Used at player init to set up and destroy the stats.

void stat_AddKill(EntPlayer *pl, Entity *villain);
    // Notes the defeat of a villain for the given player in all appropriate
    // statistics.

void stat_AddDeath(EntPlayer *pl, Entity *villain);
    // Notes the defeat of a player by a given villain in all appropriate
    // statistics.

void stat_AddTimeInZone(EntPlayer *pl, int secs);
    // Adds to time spent in-zone

void stat_AddDamageTaken(EntPlayer *pl, int damage);
void stat_AddDamageGiven(EntPlayer *pl, int damage);
    // Adds to damage (or healing if the value is negative)

void stat_AddRaceTime(EntPlayer *pl, int time, const char* racetype);
    //Records best times for you to run this race

    //Gets the amount of time that has passed since the last call.

void stat_AddXPReceived(Entity *e, int iXP);
    // Adds to XP for the zone the player is in and overall.

void stat_AddWisdomReceived(EntPlayer *pl, int iWisdom);
    // Adds to Wisdom for the zone the player is in.

void stat_AddInfluenceReceived(EntPlayer *pl, int iInf);
    // Adds to Influence for the zone the player is in and overall.

void stat_Dump(ClientLink *client, Entity *e);
    // Dumps an Entity's stats to the console.

void stat_UpdatePeriod(Entity *e, U32 uLastUpdateTime);
    // Updates a player's stats every once and a while.

#endif /* #ifndef PL_STATS_H__ */

/* End of File */

