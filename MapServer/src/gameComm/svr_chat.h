
#ifndef _SVR_CHAT_H
#define    _SVR_CHAT_H

#include <utilitieslib/network/net_link.h>
#include "entity/entVarUpdate.h"

typedef struct Entity Entity;
typedef struct ClientLink ClientLink;

// ***************************************************
// START send chat messages

// location-based channels
void chatSendLocal( Entity *e, const char * msg );
void chatSendEmote( Entity *e, const char *msg);
void chatSendEmoteCostumeChange( Entity *e, const char *emote, int costume );
void chatSendPetSay( Entity * pet, const char * msg );

// zone-wide channels
void chatSendBroadCast( Entity *e, const char * msg );
void chatSendRequest( Entity *e, const char * msg );
void chatSendToArena( Entity *e, const char *msg, int type);

// server-wide channels
void chatSendToSupergroup( Entity *e, const char *msg, int type);
void chatSendToLevelingpact(Entity *e, const char *msg, int type);
void chatSendToAlliance( Entity *e, const char *msg);
void chatSendToLeague( Entity *e, const char *msg, int type);
void chatSendToTeamup( Entity *e, const char *msg, int type);
void chatSendToArenaChannel( Entity *e, const char *msg );
void chatSendToLookingForGroup( Entity *e, const char * msg );

// cross-server channels
void chatSendToArchitectGlobal( Entity *e, const char *msg );
void chatSendToHelpChannel( Entity *e, const char *msg );

// direct communication
void chatSendPrivateMsg( Entity *e, char *incmsg, int toLeader, int allowReply, int isReply );
void chatSendReply( Entity *e, const char * msg );
void chatSendTellLast( Entity *e, const char * msg );

// ARM NOTE: not sure what these do off the top of my head...
void chatSendToPlayer(int container, const char *msg, int type, int senderID);
void chatSendToAll( Entity *e, const char *msg);
void chatSendDebug( const char *msg );
void chatSendServerBroadcast( const char * msg );
void chatSendMapAdmin( Entity *e, const char * msg );
void chatSendToFriends( Entity *e, const char * msg );
void chatSendTrialMessage(Entity * from, char *toName);

// END send chat messages
// ***************************************************

void recvChatMsg( Entity *e, Packet *pak );

void setChatChannel( Entity * e, int newChannel, int userChannelIdx );

int chatBanned(Entity *e, const char *msg);

int isIgnored( Entity *e, int db_id );
int isIgnoredAuth( Entity *e, int authId );

void removeUserFromIgnoreList( ClientLink * client, char* nameOfUserToRemove, Entity* userRemoving );
void addUserToIgnoreList( ClientLink *client, char* nameOfUserToIgnore, int authToIgnore, Entity* userIgnoring, bool spammer );

#endif