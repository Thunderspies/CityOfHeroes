/*\
 *
 *    sgraidClient.h/c - Copyright 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Client UI stuff for supergroup raids
 *
 */

#ifndef SGRAIDCLIENT_H
#define SGRAIDCLIENT_H

#include <utilitieslib/stdtypes.h>

typedef struct Packet Packet;
typedef struct SupergroupRaidInfo SupergroupRaidInfo;

void receiveSGRaidCompassString(Packet *pak);
void receiveSGRaidUpdate(Packet* pak);
void receiveSGRaidError(Packet* pak);
void receiveSGRaidOffer(Packet *pak);
char* getSGRaidCompassString(void);
char* getSGRaidInfoString(void);

char* getSGName(U32 sgid);    // dictionary built when we receive raid info

extern SupergroupRaidInfo** g_raidinfos;
void receiveSGRaidInfo(Packet* pak);

void raidClientTick(void);

#endif // SGRAIDCLIENT_H