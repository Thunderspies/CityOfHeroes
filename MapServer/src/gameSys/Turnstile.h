/*
 *
 *    Turnstile.h - Copyright 2010 NC Soft
 *        All Rights Reserved
 *        Confidential property of NC Soft
 *
 *    Turnstile system for matching players for large raid content
 *
 */

#ifndef TURNSTILE_H
#define TURNSTILE_H

#include <utilitieslib/stdtypes.h>

struct playerDatatype
{
    float timeout;
    int requestBits;
};

void turnstileMapserver_init();
void turnstileMapserver_handleRequestEventList(Entity *e);
void turnstileMapserver_handleQueueForEvents(Entity *e, Packet *pak);
void turnstileMapserver_handleRemoveFromQueue(Entity *e);
void turnstileMapserver_handleEventReadyAck(Entity *e, Packet *pak);
void turnstileMapserver_handleEventResponse(Entity *e, Packet *pak);

void turnstileMapserver_handleEventWaitTimes(Packet *pak);
void turnstileMapserver_handleQueueStatus(Packet *pak);
void turnstileMapserver_handleEventReady(Packet *pak);
void turnstileMapserver_handleEventReadyAccept(Packet *pak);
void turnstileMapserver_handleEventFailedStart(Packet *pak);
void turnstileMapserver_handleMapStart(Packet *pak);
void turnstileMapserver_handleEventStart(Packet *pak);

void turnstileMapserver_generateTurnstilePing(Entity *e);
void turnstileMapserver_handleTurnstilePong(Packet *pak);

void turnstileMapserver_generateTurnstileError(Entity *e, char *errorMsg);
void turnstileMapserver_handleTurnstileError(Packet *pak);
int turnstileMapserver_teamInterlockCheck(Entity *e, int isteamOperation);
void turnstileMapserver_handleRejoinFail(Packet *pak_in);

void turnstileMapserver_shardXferOut(Entity *e, char *shardName);
void turnstileMapserver_shardXferBack(Entity *e);

void turnstileMapserver_generateGroupUpdate(int oldLeaderDBID, int newLeaderDBID, int quitterDBID);
void turnstileMapserver_setMapInfo(int eventID, int missionID, int eventLocked);
void turnstileMapserver_getMapInfo(int *eventID, int *missionID);
int turnstileMapserver_eventLockedByGroup();
void turnstileMapserver_completeInstance();
void turnstileMapserver_addBanID(int auth_id);
void turnstileMapserver_rejoinInstance(Packet *pak_in, Entity *e);
void turnstileMapserver_playerLeavingRaid(Entity *e, int voluntaryLeave);
void turnstileMapserver_generateTrialComplete();
void turnstileMapserver_QueueForSpecificMissionInstance(Entity *e, int missionID, int instanceID);

#endif // TURNSTILE_H
