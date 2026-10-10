/* File netio.h
 *    This file includes everything that needs to be included for a program to start using the networking 
 *    module.  Furthermore, it is meant as the *only* interface to the networking module.  Do not include
 *    any other networking header files from outside of the networking module.
 *
 *    
 */


#ifndef NETIO_H
#define NETIO_H

#include "../network/net_structdefs.h"
#include "../network/netio_enet.h"
#include "../stdtypes.h"

C_DECLARATIONS_BEGIN

// Special return values for netLinkMonitor and NMMonitor
#define NETLINK_MONITOR_ABORT_AND_DO_NOT_FREE_PACKET    -2
#define NETLINK_MONITOR_DO_NOT_FREE_PACKET                INT_MAX

NetLink* createNetLink();
void destroyNetLink(NetLink* link);
void initNetLink(NetLink* link);
void clearNetLink(NetLink* link);
/****************************************************************************************************
 * NetLink Connection/Disconnection                                                                    *
 ****************************************************************************************************/
int netConnect(NetLink *link, const char *ip_str, int port, NetLinkType link_type, F32 timeout, void (*idleCallback)(F32 timeLeft));
void netSendDisconnect(NetLink *link,F32 timeout);
int netOpenSocket(NetLink *link, const char *address, int port, int tcp);

/****************************************************************************************************
 * NetLink Asynchronous Connection                                                                    *
 ****************************************************************************************************/
int netConnectAsync(NetLink *link, const char *ip_str_in, int port, NetLinkType link_type, F32 timeout);
int netConnectPoll(NetLink *link);
void netConnectAbort(NetLink *link);
int netOpenSocketAsync(NetLink *link,const char *address,int port);

/****************************************************************************************************
 * NetLink Monitoring                                                                                *
 ****************************************************************************************************/
int netLinkMonitor(NetLink* link, int lookForCommand, NetPacketCallback* netCallBack);
int netLinkMonitorBlock(NetLink* link, int lookForCommand, NetPacketCallback* netCallBack, int timeout);

/****************************************************************************************************
 * NetLink idle packets                                                                                *
 ****************************************************************************************************/
void netIdle(NetLink* link, int do_sleep, float maxIdleTime);
void netSendIdle(NetLink* link);


/****************************************************************************************************
 * NetLink data sending                                                                                *
 ****************************************************************************************************/
#define pktSend(pakptr,link) pktSendDbg(pakptr,link MEM_DBG_PARMS_INIT)
U32 pktSendDbg(Packet** pakptr, NetLink* link MEM_DBG_PARMS);

/****************************************************************************************************
 * NetLinkList initialization                                                                        *
 ****************************************************************************************************/
int netInit(NetLinkList *nlist,int udp_port,int tcp_port);
int netLinkListAlloc(NetLinkList *nlist, int numlinks, int user_data_size, NetLinkAllocCallback cb);

/****************************************************************************************************
 * NetLinkList element management                                                                    *
 ****************************************************************************************************/
void netRemoveLink(NetLink *link);

/****************************************************************************************************
 * NetLink misc                                                                                        *
 ****************************************************************************************************/
void lnkSimulateNetworkConditions(NetLink* link, int lag, int lagVary, int packetLost, int outOfOrder);

int pktRate(PktHist* hist);
int pingAvgRate(PingHistory* history);
int pingInstantRate(PingHistory* history);

void lnkFlushAll();

void netLinkAutoGrowBuffer(NetLink* link, NetLinkBufferType type);
void netLinkSetMaxBufferSize(NetLink *link, NetLinkBufferType type, int size);
void netLinkSetBufferSize(NetLink* link, NetLinkBufferType type, int size);
void netLinkListSetMaxBufferSize(NetLinkList *nlist, NetLinkBufferType type, int size);
void netLinkListSetBufferSize(NetLinkList* nlist, NetLinkBufferType type, int size);



/****************************************************************************************************
 * NM Monitoring                                                                                                *
 ****************************************************************************************************/
void NMMonitor(int milliseconds);
void NMMonitorStopReceiving(void);

/****************************************************************************************************
 * NM element manangement                                                                            *
 ****************************************************************************************************/
void NMAddLink(NetLink* link, NetPacketCallback callback);
void NMAddLinkList(NetLinkList* linklist, NetPacketCallback callback);

NMSock* NMAddSock(SOCKET sock, NMSockCallback callback, void* userData);
NMSock* NMFindSock(SOCKET sock);
void NMCloseSock(NMSock *nsock);


/****************************************************************************************************
 * NM maintenance                                                                                    *
 ****************************************************************************************************/

/****************************************************************************************************
 * NM debug utilities                                                                                *
 ****************************************************************************************************/



/****************************************************************************************************
 * Packet memory management                                                                            *
 ****************************************************************************************************/
void packetStartup(int maxPacketSize, int init_encryption);
void packetShutdown(void);
int packetCanUseEncryption(void);

#define pktCreate()        pktCreateImp(__FILE__, __LINE__)
#define pktFree(pak)    pktFreeImp(pak,__FILE__, __LINE__)

// Packet creation and destruction
Packet* pktCreateImp(char const* fname, int line);
void    pktDestroy(Packet* pak);                // Same as pktFreeImp.  Used as callback only.

void    pktFreeImp(Packet* pak, char const* fname, int line);

Packet *pktCreateEx(NetLink *link,int cmd);

#include "net_packet_common.h"

/****************************************************************************************************
 * Advanced netio functions                                                                            *
 *    Required to construct a function similar to netLinkMonitor()
 ****************************************************************************************************/
void lnkBatchReceive(NetLink* link);
void lnkBatchSend(NetLink* link);

int linkLooksDead(NetLink *link, int threshold);
int pktGet(Packet **pakptr,NetLink *link);

/****************************************************************************************************
 * utility functions                                                                            *
 ****************************************************************************************************/
char *makeIpStr(U32 ip);
char *makeHostNameStr(U32 ip);
int isLocalIp(U32 ip);
int setHostIpList(U32 ip_list[2]);
U32 getHostLocalIp();
U32 ipFromString(const char *s);

void packetGetAllocationCounts(size_t* packetCount, size_t* bsCount);

C_DECLARATIONS_END

#endif
