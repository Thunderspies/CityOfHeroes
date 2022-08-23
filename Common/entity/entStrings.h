//
//
// entStrings.h
//
// Send / Recieve functions (and any other utility functions that pop up) for entity strings
//---------------------------------------------------------------------------------------------------------

#ifndef ENTSTRINGS_H
#define ENTSTRINGS_H

typedef struct EntStrings EntStrings;
typedef struct Packet Packet;

void sendEntStrings(    Packet *Pak, EntStrings * strings );
void receiveEntStrings( Packet *Pak, EntStrings * strings );

void initEntStrings( EntStrings *strings );

EntStrings *createEntStrings(void);
void destroyEntStrings(EntStrings *entstrings);
char * breakHTMLtags( char * text );

#endif