/*\
*
*    sgraid.h/c - Copyright 2005 Cryptic Studios
*        All Rights Reserved
*        Confidential property of Cryptic Studios
*
*    Code to run a supergroup raid - probably hosted in a supergroup base
*
 */

#ifndef SGRAID_H
#define SGRAID_H

#include <utilitieslib/stdtypes.h>

typedef struct Entity Entity;
typedef struct Character Character;
typedef struct RoomDetail RoomDetail;

// data stuff
void RaidHandleUpdate(int* ids, int deleting);
int RaidMapLevelOverride(Character* p);

// running a supergroup raid
void RaidInitPlayer(Entity* e);
int RaidIsRunning(void);
int RaidIsInstant(void);
U32 RaidDefendingSG(void);    // right now, on this map
U32 RaidAttackingSG(void);
void RaidAttackerWarp(Entity* e, int override_id, int debug_print);
void RaidTick(void);
void RaidComplete(int attackerswin, const char* why, const char * nameOfItemOfPowerCaptured, int creationTimeOfItemOfPowerCaptured );
int RaidTimeRemaining(void);
void RaidDisplayFloat(U32 sg, const char *message);
void RaidMapHandleKill(Entity * killer,Entity * victim);

// actually in baseRaid.c
void RaidItemInteract(Entity* player, RoomDetail* det);

#endif // SGRAID_H