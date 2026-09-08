#ifndef NETIO_RECEIVE_H
#define NETIO_RECEIVE_H

#include "../stdtypes.h"
#include "../network/net_typedefs.h"

struct sockaddr_in;

/****************************************************************************************************
 * Batch packet receiving                                                                            *
 ****************************************************************************************************/
void lnkBatchReceive(NetLink* link);

/****************************************************************************************************
 * Single packet receiving                                                                            *
 ****************************************************************************************************/
int pktGet(Packet **pakptr,NetLink *link);

int pktGetUdpAsync(Packet** pakptr, NetLinkList* list, struct sockaddr_in *addr);
int pktGetUdp(Packet** pakptr,int socket,struct sockaddr_in *addr);
int pktGetTcpAsync(Packet** pakptr, NetLink* link);
int pktGetTcp(Packet** pakptr, NetLink* link);
int pktGetTcpSync(Packet** pakptr, NetLink* link);

void lnkReadTCPAsync(NetLink* link, int bytesExtracted);
#endif
