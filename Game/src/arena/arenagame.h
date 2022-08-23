/*\
 *
 *    arenagame.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Game client logic for arena events.  Mainly networking stuff
 *
 */

#ifndef ARENAGAME_H
#define ARENAGAME_H

typedef struct Packet Packet;
typedef struct ArenaEventList ArenaEventList;

// countdown before the match
extern bool arenaCountdown;
extern unsigned long arenaCountdownMomentInitial;
extern int arenaCountdownTimerInitial;
extern int arenaCountdownTimerCurrent;

void requestArenaKiosk(int radioButtonFilter,int tabFilter);

void receiveArenaKiosk(Packet *pak);
void receiveArenaKioskOpen(Packet* pak);
ArenaEventList* getArenaKiosk(void);

void receiveArenaEvents(Packet* pak);
void receiveArenaUpdate(Packet* pak);
void receiveArenaUpdateTrayDisable(Packet* pak);
void receiveServerRunEventWindow(Packet* pak);
void receiveArenaFinishNotification(Packet *pak);
void requestReadyTeleport(void);

void receiveArenaResults(Packet * pak);
void arenaStartCountdown(int timer);
void receiveArenaStartCountdown(Packet *pak);
void receiveArenaCompassString(Packet *pak);
void receiveArenaVictoryInformation(Packet *pak);
void receiveArenaScheduledTeleport(Packet *pak);

char* getArenaCompassString();
char* getArenaInfoString();
char* getArenaRespawnTimerString();
int getArenaStockLives();

void playArenaStatsLoop();
void resetArenaVars();
int amOnArenaMap();
int amOnPetArenaMap();
int activeEvent(void);

int arenaAllowReturnStaticMap();

void handleClientArenaPlayerUpdate(Packet * pak);

#endif // ARENAGAME_H