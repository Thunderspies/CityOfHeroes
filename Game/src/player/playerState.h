#include "entity/entity_enum.h"
/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef PLAYERSTATE_H__
#define PLAYERSTATE_H__

typedef enum ClientGameState
{
    STATE_SIMPLE,
    STATE_CREATE_TEAM_CONTAINER,
    STATE_CREATE_TEAM_CONTAINER_WAIT_MAPSVRR_RESPONSE,
    STATE_DEAD,
    STATE_RESURRECT,
    STATE_AWAITING_GURNEY_XFER,
    TOTAL_GAME_STATES,
} ClientGameState;

#define MAX_FLOAT_DMG_TIME  (60.0f)

void runState();
void setState(  ClientGameState st, int baseGurney);

void addFloatingDamage( Entity *damager, Entity *victim, int dmg, char *pch, float *loc, bool wasAbsorb);
void addFloatingInfo(int svr_idx, char *pch, U32 colorFG, U32 colorBG, U32 colorBorder, float fScale, float fLifetime, float fDelay, EFloaterStyle style, float *loc);
void DistributeUnclaimedFloaters(Entity *e);
void ClearUnclaimedFloaters(void);

void receiveServerSetClientState( Packet *pak );
void receiveFloatingDmg( Packet *pak );
void receiveFloatingInfo( Packet *pak );
void receivePlaySound( Packet *pak );
void receiveFadeSound( Packet *pak );
void receiveDesaturateInfo( Packet *pak );
void serveFloatingDmg();
void respawnYes(bool bBase);
bool isBaseGurney();
#if 0
bool isTeamMixedFactions();
#endif

#endif /* #ifndef PLAYERSTATE_H__ */

/* End of File */

