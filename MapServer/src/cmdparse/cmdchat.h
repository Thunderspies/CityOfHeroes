
#ifndef CMDCHAT_H
#define CMDCHAT_H

#include <utilitieslib/utils/cmdoldparse.h>

typedef struct ClientLink ClientLink;

void chatCommand( Cmd * cmd, ClientLink *client, char* str );
int player_online( char *name );
void cmdOldChatCheckCmds(void);

extern Cmd chat_cmds[];

#define MAX_CHAT_MESSAGE_LENGTH 1000
#define CHAT_TEMP_BUFFER_LENTH (MAX_CHAT_MESSAGE_LENGTH + 48) // Max chat length + room for a name + room for -->:, etc


#endif
