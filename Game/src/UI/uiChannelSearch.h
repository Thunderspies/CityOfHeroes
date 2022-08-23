/*\
*
*    uiChannelsearch.h/c - Copyright 2004, 2005 Cryptic Studios
*        All Rights Reserved
*        Confidential property of Cryptic Studios
*
*    Shows a list of global chat channels and and allows the player to search them
*
 */

#ifndef UICHANNELSEARCH_H
#define UICHANNELSEARCH_H

int channelSearchWindow(void);
void receiveChannelData(char* name, char* description, int members, int online);
void chanRequestSearch(char* mode, char* value);

#endif // UICHANNELSEARCH_H