/* File net_link.h
 *    Defines a NetLink and its public interface.
 *
 *    
 *    
 */

#ifndef NET_LINK_H
#define NET_LINK_H

#include "../network/net_structdefs.h"

C_DECLARATIONS_BEGIN

/****************************************************************************************************
 * NetLink Creation/Destruction                                                                        *
 ****************************************************************************************************/
NetLink* createNetLink();
void destroyNetLink(NetLink* link);
void initNetLink(NetLink* link);
void clearNetLink(NetLink* link);
/* Lazily allocate the legacy duplicate-ID table on non-NULL, live link.
 * Returns borrowed link-owned storage, freed by clearNetLink. ENet links do
 * not require this table. Serialize calls with other network operations.
 */
SimpleSet *lnkEnsureReceivedPacketID(
    NetLink *link); // legacy dup-detection table, created on first use

/****************************************************************************************************
 * NetLink Connection/Disconnection                                                                    *
 ****************************************************************************************************/
int netConnect(NetLink *link,const char *ip_str, int port, NetLinkType link_type, F32 timeout, void (*idleCallback)(F32 timeLeft));
int netConnectEx(NetLink *link,const char *ip_str, int port, NetLinkType link_type, F32 timeout, void (*idleCallback)(F32 timeLeft), int fakeYieldToOS);
void netSendDisconnect(NetLink *link,F32 timeout);
int netOpenSocket(NetLink *link,const char *address,int port,int tcp);

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
int netMessageScan(NetLink *link, int lookForCommand, bool isControlCommand, NetPacketCallback *netCallBack);

/****************************************************************************************************
 * NetLink idle packets                                                                                *
 ****************************************************************************************************/
void netIdle(NetLink* link, int do_sleep, float maxIdleTime);
void netIdleEx(NetLink* link, int do_sleep, float maxIdleTime, U32 current_time);
void netSendIdle(NetLink* link);

/****************************************************************************************************
 * Misc functions                                                                                    *
 ****************************************************************************************************/
void netResetCycleStats(NetLink* link);
void lnkDumpUnsent(NetLink *link);
void dumpPacketArray(Array* array);

/****************************************************************************************************
 * Convenience functions                                                                            *
 ****************************************************************************************************/
#define PACKET_DEFAULT = ((NetLink*)NULL) // To be passed to pktCreateEx, note that these don't work if a server is supporting multiple packet versions
Packet *pktCreateEx(NetLink *link, int cmd);
Packet *pktCreateControlCmd(NetLink *link, CommControlCommand controlcmd);

void netLinkAutoGrowBuffer(NetLink* link, NetLinkBufferType type);
void netLinkSetMaxBufferSize(NetLink *link, NetLinkBufferType type, int size);
void netLinkSetBufferSize(NetLink* link, NetLinkBufferType type, int size);
void netLinkListSetMaxBufferSize(NetLinkList *nlist, NetLinkBufferType type, int size);
void netLinkListSetBufferSize(NetLinkList* nlist, NetLinkBufferType type, int size);

void netLog(NetLink *link, FORMAT fmt, ... );

C_DECLARATIONS_END

#endif