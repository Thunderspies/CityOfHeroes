#ifndef NET_LINKLIST_H
#define NET_LINKLIST_H

#include "../stdtypes.h"
#include "../network/net_typedefs.h"

struct sockaddr_in;

C_DECLARATIONS_BEGIN

/****************************************************************************************************
 * NetLinkList initialization                                                                        *
 ****************************************************************************************************/
int netInit(NetLinkList *nlist,int udp_port,int tcp_port);
int netLinkListAlloc(NetLinkList *nlist, int numlinks, int user_data_size, NetLinkAllocCallback cb);
void netLinkListDisconnect(NetLinkList *nlist);


/****************************************************************************************************
 * NetLinkList element management                                                                    *
 ****************************************************************************************************/
//static NetLink *netAddLink(NetLinkList *nlist, struct sockaddr_in *addr);
void netRemoveLink(NetLink *link);


NetLink* findUdpNetLink(NetLinkList* nlist, struct sockaddr_in *addr);
//static NetLink* findTcpNetLink(NetLinkList* nlist, SOCKET sock);



/****************************************************************************************************
 * NetLinkList Monitoring                                                                            *
 ****************************************************************************************************/
void netLinkListProcessMessages(NetLinkList* list, NetPacketCallback *netCallBack);
void netLinkListBatchReceive(NetLinkList* nlist);
NetLink* netValidateUDPPacket(NetLinkList* linklist, Packet* pak);

/****************************************************************************************************
 * NetLinkList Maintenance                                                                            *
 ****************************************************************************************************/
int netGetTcpConnect(NetLinkList *nlist);
void netDiscardDeadUDPLink(NetLinkList* list, struct sockaddr_in* addr);
void netDiscardDeadLink(NetLink* link);
void netLinkListMaintenance(NetLinkList* list);
void netLinkListHandleTcpConnect(NetLinkList* list);
//static void netLinkListRemoveDisconnected(NetLinkList* list);

void netForEachLink(NetLinkList* list, NetLinkCallback callback);

C_DECLARATIONS_END

#endif
