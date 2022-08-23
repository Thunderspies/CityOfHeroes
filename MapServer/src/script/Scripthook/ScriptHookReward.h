/*\
 *
 *    scripthookReward.h - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides all the functions CoH script files should be referencing -
 *        essentially a list of hooks into game systems.
 *
 *    This file contains all functions related to rewards
 *
 */

#ifndef SCRIPTHOOKREWARD_H
#define SCRIPTHOOKREWARD_H

// rewards
void EntityGrantReward(ENTITY ent, STRING rewardTable);
void TeamGrantReward(TEAM team, STRING rewardTable);
void MissionGrantReward(STRING rewardTable); // grants reward to all players in mission who deserve it

// powers
void EntityRemoveTempPower(ENTITY ent, STRING powerset, STRING power);
void EntityGrantTempPower(ENTITY ent, STRING powerset, STRING power);
void EntityRemovePower(ENTITY ent, STRING power);
void EntityGrantPower(ENTITY ent, STRING power);

// Salvage
void EntityGrantSalvage(ENTITY ent, STRING salvage, NUMBER amount);
NUMBER EntityCountSalvage(ENTITY ent, STRING salvage);


// XP and Influence
NUMBER EntityGetXP(ENTITY ent);
void EntitySetXP(ENTITY ent, NUMBER xp);
void EntitySetXPByLevel(ENTITY ent, NUMBER level);
NUMBER EntityGetInfluence(ENTITY ent);
void EntityAddInfluence(ENTITY ent, NUMBER inf);

// Badges
int EntityHasBadge(ENTITY ent, STRING badgeName);
void EntityGrantBadge(ENTITY ent, STRING badgeName, bool notify);
void TeamGrantBadge(TEAM team, STRING badgeName, bool notify);

// player reward tokens
int EntityHasRewardToken(ENTITY ent, STRING token);
int EntityGetRewardToken(ENTITY ent, STRING token);        // returns -1 if no token of this name
int EntityGetRewardTokenTime(ENTITY ent, STRING token); // returns -1 if no token of this name, otherwise returns time that the token was last awarded in seconds since 2000
void EntityGrantRewardToken(ENTITY ent, STRING token, NUMBER value);
void EntityRemoveRewardToken(ENTITY ent, STRING token);

// Raid Points
void EntityAddRaidPoints(ENTITY ent, NUMBER points, NUMBER isRaid);

//////////////////////////////////////////////////////////////////////////////////
// Map Data Tokens
//        These tokens are shared across ALL mapserver in the shard
int MapHasDataToken(STRING token);
int MapGetDataToken(STRING token); // returns -1 if no token of this name
void MapSetDataToken(STRING token, NUMBER value);
void MapRemoveDataToken(STRING token);

// invention token
NUMBER IsInventionEnabled();

//////////////////////////////////////////////////////////////////////////////////
// Incarnate points
void Script_Incarnate_RewardPoints(ENTITY ent, NUMBER isTypeB, NUMBER pointCount);

#endif