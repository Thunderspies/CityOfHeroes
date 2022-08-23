/*
 *
 *    EndGameRaid.h - Copyright 2010 NC Soft
 *        All Rights Reserved
 *        Confidential property of NC Soft
 *
 *    Setup and handling of end game raid instances
 *
 */

#ifndef ENDGAMERAID_H
#define ENDGAMERAID_H

#include <utilitieslib/stdtypes.h>

typedef struct EndGameRaidKickMods
{
    int lastKickTimeSeconds;
    int lastPlayerKickTimeMins;
    int minRaidTimeMins;
    int kickDurationSeconds;
}EndGameRaidKickMods;
extern EndGameRaidKickMods g_EndGameRaidKickMods;
char *EndGameRaidConstructInfoString(char *zoneEvent, int playerCount, int mobLevel, int instanceID, int missionID, int eventLocked);
int EndGameRaidDeconstructInfoString(char *mapinfo);
void EndGameRaidSetInfoString(char *info);
void EndGameRaidPlayerEnteredMap(Entity *e);
void EndGameRaidPlayerExitedMap(Entity *e);
void EndGameRaidRequestVoteKick(Entity *initiator, Entity *kicked);
void EndGameRaidReceiveVoteKickOpinion(int voter_dbid, int opinion);
void EndGameRaidTick();

#endif // ENDGAMERAID_H
