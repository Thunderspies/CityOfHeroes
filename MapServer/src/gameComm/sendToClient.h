#ifndef _SENDTOCLIENT_H
#define    _SENDTOCLIENT_H

#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/components/ArrayOld.h>
#include "entity/entVarUpdate.h"    // For INFO_BOX_MSGS
#include "entity/attribmod.h"

typedef struct Entity Entity;
typedef struct ClientLink ClientLink;

#define DEFAULT_BUBBLE_TIME            2.0 //Time before bringing up the next bubble

#include "storyarc/ScriptUIEnum.h"
#define DEFAULT_BUBBLE_DURATION        8.0 //Time this bubble lasts on screen

void sendMoralChoice(Entity * e, const char *leftText, const char *rightText, const char *leftWatermark, const char *rightWatermark, int requireConfirmation);

void conPrintf(    ClientLink* client, const char *s, ... );
void convPrintf( ClientLink* client, const char *s, va_list va );
void sendChatMsg( Entity *e, const char *msg, int type, int senderID );
void sendChatMsgEx( Entity *e, Entity *sender, const char *srcmsg, int type, F32 duration, int senderID, int sourceID, int enemy, F32* position );
void sendChatMsgFrom(Entity *ent, Entity *eSrc, int type, F32 duration, const char *s );
void sendPrivateCaptionMsg(Entity *player, const char *caption);
void sendInfoBox( Entity *ent, int type, const char *s, ... );
void sendInfoBoxAlreadyTranslated(Entity *ent, int type, const char *message);
#define sendInfoBoxDbg sendInfoBox

void sendDialogPowerRequest(Entity *e, const char *str, int translateName, const char *name, int id, int timeout);
void sendDialogLevelChange( Entity *e, int new_level, int player_level );
extern Array* CombatMessage;
extern Array* DamageMessage;

void sendCombatMessage(Entity* e, int chatType, CombatMessageType messageType, int isVictimMessage, int powerCRC, int modIndex, const char *otherEntityName, float float1, float float2);

void sendServerWaypoint(Entity* e, int id, int set, int compass, int pulse, const char* icon, const char* iconB, const char* name, const char* mapname, Vec3 loc, float angle);
void sendPopHelp( Entity * e, const char *tag );
void sendPopHelpReset( Entity * e );
void sendIncarnateTrialStatus(Entity *e, TrialStatus status);

#endif
