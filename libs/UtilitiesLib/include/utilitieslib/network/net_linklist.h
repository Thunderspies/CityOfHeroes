#ifndef NET_LINKLIST_H
#define NET_LINKLIST_H

#include "../utils/wininclude.h"
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

/* Accept a peer on an allocated list. ip is in network byte order, port in
 * host byte order, and hostSocket is borrowed from the list's ENet host.
 * Returns a list-owned link or NULL when the list's admission rules reject it.
 * The ENet event loop calls this while holding the network critical section.
 */
NetLink *netAddLinkEnetAccept(NetLinkList *nlist, U32 ip, int port,
			      SOCKET hostSocket);

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
