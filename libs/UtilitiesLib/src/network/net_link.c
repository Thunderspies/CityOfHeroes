#include "utilitieslib/network/net_link.h"
#include "utilitieslib/network/net_socket.h"
#include "utilitieslib/network/netio_core.h"
#include "utilitieslib/network/netio_send.h"
#include "utilitieslib/network/netio_receive.h"
#include "utilitieslib/network/net_packet.h"
#include "utilitieslib/network/net_linklist.h"    // This doesn't seem quite right.
#include "utilitieslib/network/crypt.h"
#include "utilitieslib/network/net_version.h"
#include "utilitieslib/network/net_masterlist.h"
#include "utilitieslib/network/netio.h"
#include "utilitieslib/components/Queue.h"
#include "zlib/zlib.h"

#include <stdio.h>
#include "utilitieslib/assert/assert.h"

#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/network/sock.h"
#include "utilitieslib/utils/log.h"
#include "utilitieslib/utils/utils.h"

#define NETLINK_RECEIVEQUEUE_INIT_SIZE    128
#define NETLINK_RECEIVEQUEUE_MAX_SIZE    0        // Unlimited

#define NETLINK_ACKWAITING_INIT_SIZE    128
#define NETLINK_ACKWAITING_MAX_SIZE        0        // Unlimited

/****************************************************************************************************
 * Junk                                                                                                *
 ****************************************************************************************************/
/*
static int netCheckEncrypt(NetLink *link,Packet *pak)
{
    int        encrypt = pktGetBitsPack(pak,1);
    if (encrypt)
        pktGetBitsArray(pak,512,link->public_key);
    return encrypt;
}
*/

/****************************************************************************************************
 * Static function forward declarations                                                                *
 ****************************************************************************************************/
static int netOpenSocketTcp(NetLink *link,const char *address,int port);
static int netOpenSocketUdp(NetLink *link,const char *address,int port);
//static int netOpenSocket(NetLink *link,char *address,int port,int tcp);
static void netAckConnect(NetLink *link, int encrypt, CommControlCommand cmd);
static void netAckDisconnect(NetLink *link);
Packet *getStoredPacket(NetLink *link);
int putStoredPacket(NetLink *link,Packet *pak);
static int netCheckEncrypt(NetLink *link,Packet *pak);
static unsigned int lnkReceivedPakIDHash(void* value);
static int netLinkMonitorInternal(NetLink* link, int lookForCommand, bool isControlCommand, NetPacketCallback* netCallBack);

/****************************************************************************************************
 * NetLink Creation/Destruction                                                                        *
 ****************************************************************************************************/
static int nextLinkID = 0;
NetLink* createNetLink(){
    return calloc(1, sizeof(NetLink));
}

void destroyNetLink(NetLink* link){
    clearNetLink(link);
    free(link);
}

void initNetLink(NetLink* link){
    netioEnterCritical();
    // Initialize all the queues in the NetLink.
    link->receiveQueue = createQueue();
    initQueue(link->receiveQueue, NETLINK_RECEIVEQUEUE_INIT_SIZE);            // The send queue's initial size is...
    qSetMaxSizeLimit(link->receiveQueue, NETLINK_RECEIVEQUEUE_MAX_SIZE);    // The send queue can grow up to this size.

    link->sendQueue2 = createQueue();
    initQueue(link->sendQueue2, NETLINK_SENDQUEUE_INIT_SIZE);        // The send queue's initial size is...
    qSetMaxSizeLimit(link->sendQueue2, 0);    // The send queue's size is tracked seperately
    link->sendQueuePacketCount = 0;
    link->totalQueuedBytes = 0;
    link->sendQueuePacketMax = NETLINK_SENDQUEUE_MAX_SIZE;

    initArray(&link->reliablePacketsArray, NETLINK_ACKWAITING_INIT_SIZE);

    // A link keeps a table of the last received packet ID's to determine if the link
    // has received a packet already.
    link->receivedPacketID = createSimpleSet();
    initSimpleSet(link->receivedPacketID,  16 * 1024, lnkReceivedPakIDHash);

    initArray(&link->pakTimeoutArray, 64);

    link->compressionAllowed = 0;
    link->compressionDefault = 0;
    link->orderedAllowed = 0;
    link->orderedDefault = 0;
    link->protocolVersion = 0;
    link->sendBufferSize = 8*1024; // OS default
    link->recvBufferSize = 8*1024; // OS default

    // Note: These two will be overridden in netAddLink if they have a parent netLinkList
    link->sendBufferMaxSize = 256*1024;
    link->recvBufferMaxSize = 256*1024;

    link->UID = ++nextLinkID;

    link->pLinkToMultiplexer = NULL;
    netioLeaveCritical();
}

void pktDequeueDestroy(Packet *pak) {
    pak->inRetransmitQueue = 0;
    pak->inSendQueue = 0;
    pktDestroy(pak);
}

static void pktDequeueDestroyAssertInSendqueue(void *entry)
{
	Packet *pak = entry;
    assert(pak->inSendQueue);
    pak->inRetransmitQueue = 0;
    pak->inSendQueue = 0;
    pktDestroy(pak);
}

static void pktDestroyIfNotInSendqueue(void *entry)
{
	Packet *pak = entry;
    if (!pak->inSendQueue) {
        pktDestroy(pak);
    }
}

static void pktDestroyCallback(void* arg0)
{
    pktDestroy((Packet*)arg0);
}

static void arrayOfPacketsDestory(void *entry)
{
	Array *array = entry;
    destroyArrayEx(array, pktDestroyCallback);
}

// FIXME!!!
//    Provide a single way to check if a link is connected or not.
//    Look into other places in the code to see if ppl are doing weird things to check if
//    the connection is still live.
//    For example, commCheck() checks NetLink::socket to see if the link is connected.
void clearNetLink(NetLink* link){
    netioEnterCritical();
    // Remove the link being destroyed from the flush list.
    if(link->waitingForFlush)
        lnkRemoveFromFlushList(link);

    if (link->socket >= 0){
        // If this is a tcp link or a UDP link that is not sharing its socket,
        // close the socket.
        if(NLT_TCP == link->type || (NLT_UDP == link->type && NULL == link->parentNetLinkList)){
            int result;

            // Cancel any pening async operations.
            if(link->opType == NLOT_ASYNC)
                CancelIo((HANDLE)link->socket);

            //printf("Closing socket 0x%x Sent %i Recv %i\n", link->socket, link->totalBytesSent, link->totalBytesRead);
            result = closesocket(link->socket);
        }

        link->socket = 0;
    }

    // Cleanup all the dynamic storage in the link.
    // Note that the packets held all dynamic storage should be freed also.
    if(link->receiveQueue)
		destroyQueueEx(link->receiveQueue, pktDestroyCallback);

    // Check to verify all packets in retransmitQueue are also in reliablePacketsArray
    if(link->retransmitQueue){
        int i;
        for (i=0; i<link->reliablePacketsArray.size; i++) {
            int idx;
            idx = qFindIndex(link->retransmitQueue, link->reliablePacketsArray.storage[i]);
            if(idx != -1){
                Packet** element;
                element = (Packet**)qGetElement(link->retransmitQueue, idx);
                (**element).inRetransmitQueue = 0;
                *element = NULL;
            }
        }
        while (!qIsEmpty(link->retransmitQueue)) {
            Packet *pak = qDequeue(link->retransmitQueue);
            assert(!pak);
        }
    }

    if(link->retransmitQueue)
        destroyQueue(link->retransmitQueue); // Don't destroy packets, reliablePacketsArray owns them

    // Some packets in the sendQueue might be in the reliablePacketsArray, if so, don't
    // destroy them!
    if(link->reliablePacketsArray.storage)
        destroyArrayPartialEx(&link->reliablePacketsArray, pktDestroyIfNotInSendqueue);

    if(link->sendQueue2)
        destroyQueueEx(link->sendQueue2, pktDequeueDestroyAssertInSendqueue);

    if(link->receivedPacketStorage.storage)
		destroyArrayPartialEx(&link->receivedPacketStorage,
			pktDestroyCallback);

    if(link->receivedPacketID)
        destroySimpleSet(link->receivedPacketID);

    if(link->pakTimeoutArray.storage)
        destroyArrayPartial(&link->pakTimeoutArray);

    // Destroy all packets being held in the sibling waiting area.
    if(link->siblingWaitingArea.storage)
        destroyArrayPartialEx(&link->siblingWaitingArea, arrayOfPacketsDestory);

    //Do not destroy the siblinMergeArray, all of these packets are also in the siblingWaitingArea, which we already destroyed
    //if(link->siblingMergeArray.storage)
    //    destroyArrayPartialEx(&link->siblingMergeArray, arrayOfPacketsDestory);
    destroyArrayPartial(&link->siblingMergeArray);

    if (link->pak) {
        pktFree(link->pak);
    }

    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0,  "Cleared link\n");

    if (link->zstream_pack)
    {
        deflateEnd(link->zstream_pack);
        free(link->zstream_pack);
    }
    if (link->zstream_unpack)
    {
        inflateEnd(link->zstream_unpack);
        free(link->zstream_unpack);
    }
    link->last_ordered_recv_id = 0;
    link->last_ordered_sent_id = 0;
    memset(link,0,sizeof(NetLink));

    netDiscardDeadLink(link);
    /*{
        extern _CrtMemState initialMemState;
        printf("Link closed!\n");
        _CrtMemDumpAllObjectsSince(&initialMemState);
    }*/
    netioLeaveCritical();
}



/****************************************************************************************************
 * NetLink Connection/Disconnection                                                                    *
 ****************************************************************************************************/
/* Function netConnect()
 *    Attempts to make a connection to the given ip+port using the given link.
 *
 */
int netConnectEx(NetLink *link, const char *ip_str_in, int port, NetLinkType link_type, F32 timeout, void (*idleCallback)(F32 timeLeft), int fakeYieldToOS)
{
    char    ip_str[128];
    int        timer,idx,last_idx=0;
    Packet*    pak;
    int        result;
    U32        connect_id;
    int        network_version = getDefaultNetworkVersion();
    int        disconnect_count=0;

    netioEnterCritical();

    Strncpyt(ip_str, ip_str_in); // Make local copy, since idle callbacks might change the static makeIpStr() returns

    // A connected link must be disconnected first before another connection can be made.
    if(link->connected)
    {
        netioLeaveCritical();
        return 0;
    }
    clearNetLink(link);
    // Start the timeout timer.
    timer = timerAlloc();
    timerStart(timer);

tryConnect:

    // Make all connections synchronous by default
    link->opType = NLOT_SYNC;

    // Turn off retransmission for tcp connections.
    if(link_type == NLT_TCP){
        link->retransmit = 0;
    }else
        link->retransmit = 1;

    if (idleCallback) {
        idleCallback(timeout - timerElapsed(timer));
    }

    // Create a socket and attach it to the link.
    while(!netOpenSocket(link,ip_str,port,link_type == NLT_TCP))
    {
        if (!(timeout == 0.0) && timerElapsed(timer) > timeout)
        {
            timerFree(timer);
            netioLeaveCritical();
            return 0;
        }

        if (idleCallback) {
            idleCallback(timeout - timerElapsed(timer));
        }

        if (fakeYieldToOS)
        {
            FakeYieldToOS();
        }
    }
    
    //FIXME!!!  //JE: this is probably fixed when all of the other leaks were fixed, but should be tested
    // There is a memory leak in the networking code.
    // Comment this line out to cause rapid connect/disconnect errors to see the leak.
    // Make sure the link is not marked as disconnected.
    link->disconnected = 0;

    applyNetworkVersionToLink(link, 0); // Reset to initial version
    assert(link->compressionDefault==0); // For the handshaking, we want to send packed bits!
    assert(link->compressionAllowed==0); // We also can't be sending the compression on/off bit on the connect packet!
    assert(link->controlCommandsSeparate==0); // We also can't be sending the extra bit before the connect command
    assert(link->orderedAllowed==0);
    pak = pktCreateControlCmd(link, COMMCONTROL_CONNECT);
    pak->reliable = 1;
    connect_id = timerCpuTicks();
    //printf("Connecting with connect_id %x\n", connect_id);
    pktSendBitsPack(pak,1,connect_id);
    pktSendBitsPack(pak,1,network_version);
    pktSend(&pak, link);
    lnkBatchSend(link);
    applyNetworkVersionToLink(link, network_version);

    while(1)
    {
        result = netLinkMonitorInternal(link, COMMCONTROL_CONNECT_SERVER_ACK_ACK, true, 0);

        // If the connection was made successfully, we're done.
        if(1 == result || -1 == result)
            break;

        // If the connection was aborted by the peer, restart the connection sequence.
        if (!(timeout == 0.0) && timerElapsed(timer) > timeout)
        {
            clearNetLink(link);
            timerFree(timer);
            netioLeaveCritical();
            return 0;
        }
        if(-2 == result) {
            // We were disconnected
            disconnect_count++;
            if (disconnect_count==5 && network_version > 0) {
                // Try an older network version (ServerMonitor
                network_version--;
            }
            goto tryConnect;
        }

        if (idleCallback) {
            idleCallback(timeout - timerElapsed(timer));
        }

        if (fakeYieldToOS)
        {
            FakeYieldToOS();
        }

        netIdle(link,1,10);
        idx = (int)timerElapsed(timer);
        if (idx != last_idx)
            ;// put in a callback here
    }
    timerFree(timer);
    netioLeaveCritical();
    return 1;
}

int netConnect(NetLink *link, const char *ip_str_in, int port, NetLinkType link_type, F32 timeout, void (*idleCallback)(F32 timeLeft))
{
    return netConnectEx(link, ip_str_in, port, link_type, timeout, idleCallback, 0);
}

/****************************************************************************************************
 * NetLink Asynchronous Connection                                                                    *
 ****************************************************************************************************/
int netConnectAsync(NetLink *link, const char *ip_str_in, int port, NetLinkType link_type, F32 timeout)
{
    char    ip_str[128];
    int        result;
    int        network_version = getDefaultNetworkVersion();
    int        disconnect_count=0;

    // Only do async connection on TCP sockets
    if (link_type != NLT_TCP)
    {
        return 0;
    }

    netioEnterCritical();

    Strncpyt(ip_str, ip_str_in); // Make local copy, since idle callbacks might change the static makeIpStr() returns

    // A connected link must be disconnected first before another connection can be made.
    if(link->connected || link->pending)
    {
        netioLeaveCritical();
        return 0;
    }
    clearNetLink(link);

    // Make all connections synchronous by default
    link->opType = NLOT_SYNC;

    // Turn off retransmission.
    link->retransmit = 0;

    // Create a socket and attach it to the link.
    result = netOpenSocketAsync(link,ip_str,port);

    if (result)
    {
        // set up a timer, and save the timeout
        link->asyncTimer = timerAlloc();
        timerStart(link->asyncTimer);
        link->asyncTimeout = timeout;
    }

    netioLeaveCritical();
    return result;
}

/* Function netConnectPoll()
 *    Check the current connection of a link with an async connection attempt
 *
 * Return value:
 *  -1: Failed
 *   0: Either link is not pending (an attempt wasn't made), or the attempt is still in progress
 *   1: Connection has been made successfully
 *
 */
int netConnectPoll(NetLink *link)
{
    Packet*    pak;
    int        result;
    U32        connect_id;
    int        network_version = getDefaultNetworkVersion();
    int        disconnect_count=0;

    // Link pending determines if there is an async connect attempt in progress.
    if (!link->pending)
    {
        return 0;
    }

    netioEnterCritical();

    if (timerElapsed(link->asyncTimer) > link->asyncTimeout)
    {
        // Connection timed out
        timerFree(link->asyncTimer);
        link->pending = 0;
        link->connecting = 0;
        netConnectAbort(link);
        netioLeaveCritical();
        return -1;
    }

    // Assuming link->pending is set, link->connecting then flags what condition the connection is in.
    // If clear the async connect(...) call is in progress
    if (link->connecting == 0)
    {
        // connect(...) in progress - poll socket for status
        result = sockCheckWriteConnection(link->socket);

        if (result == 0)
        {
            // Nothing happened
            netioLeaveCritical();
            return 0;
        }

        if (result == -1)
        {
            // Connection failed;
            timerFree(link->asyncTimer);
            link->pending = 0;
            link->connecting = 0;
            netConnectAbort(link);
            netioLeaveCritical();
            return -1;
        }

        // If we arrive here, the low level connect(...) attempt succeeded.  So now do the NetLink init required.
        initNetLink(link);

        // From here down is lifted directly from the synchronous call netConnect.

        //FIXME!!!  //JE: this is probably fixed when all of the other leaks were fixed, but should be tested
        // There is a memory leak in the networking code.
        // Comment this line out to cause rapid connect/disconnect errors to see the leak.
        // Make sure the link is not marked as disconnected.
        link->disconnected = 0;

        applyNetworkVersionToLink(link, 0); // Reset to initial version
        assert(link->compressionDefault==0); // For the handshaking, we want to send packed bits!
        assert(link->compressionAllowed==0); // We also can't be sending the compression on/off bit on the connect packet!
        assert(link->controlCommandsSeparate==0); // We also can't be sending the extra bit before the connect command
        assert(link->orderedAllowed==0);
        pak = pktCreateControlCmd(link, COMMCONTROL_CONNECT);
        pak->reliable = 1;
        connect_id = timerCpuTicks();
        //printf("Connecting with connect_id %x\n", connect_id);
        pktSendBitsPack(pak,1,connect_id);
        pktSendBitsPack(pak,1,network_version);
        pktSend(&pak, link);
        lnkBatchSend(link);
        applyNetworkVersionToLink(link, network_version);

        // OK.  The problem here is that we just did a pktSend, and I now need to wait for a reply.
        // I could just block here, but I really don't want to.  So I set link->connecting to flag that the
        // TCP socket is connected, but that I'm waiting on the reply to the COMMCONTROL_CONNECT packet.
        link->connecting = 1;
        netioLeaveCritical();
        return 0;
    }
    else
    {
        // Poll for the reply to the COMMCONTROL_CONNECT packet.
        result = netLinkMonitorInternal(link, COMMCONTROL_CONNECT_SERVER_ACK_ACK, true, 0);

        // If the connection was made successfully, we're done.
        if (result == 1 || result == -1)
        {
            timerFree(link->asyncTimer);
            link->pending = 0;
            link->connecting = 0;
            link->asyncTimer = 0;
            netioLeaveCritical();
            return 1;
        }
        // On a disconnect, it'll have to be up to the calling code to retry.
        if (result == -2)
        {
            timerFree(link->asyncTimer);
            link->pending = 0;
            link->connecting = 0;
            link->asyncTimer = 0;
            netConnectAbort(link);
            netioLeaveCritical();
            return -1;
        }
        // Otherwise we got a zero result which means we're still awaiting a response.
    }
    netioLeaveCritical();
    return 0;
}

// Terminate a connection attempt.
void netConnectAbort(NetLink *link)
{
    if (link->pending)
    {
        netioEnterCritical();

        // Which means just blowing away the Link
        clearNetLink(link);

        // And explicitly clearing the two flags I care about.  Thes two assignments are probably not necessary, since I believe that
        // clearNetLink does a memset of the entire link to zero.
        // However ... if that memset were ever removed and these flags remained set, it would be a "Bad Thing" (tm).
        link->pending = 0;
        link->connecting = 0;

        netioLeaveCritical();
    }
}

// Low level async socket open.
int netOpenSocketAsync(NetLink *link,const char *address,int port)
{
    int        ret;

    // This is total boilerplate
    link->socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (link->socket < 0)
    {
        return 0;
    }

    sockSetAddr(&link->addr,ipFromString(address),port);
    // set the socket to non-blocking
    sockSetBlocking(link->socket,0);
    ret = connect(link->socket,(struct sockaddr *)&link->addr,sizeof(link->addr));
    // allow an error return that is caused by a "Would Block" error to go through.  This will be the expected behavior.
    if (ret != 0 && WSAGetLastError() != WSAEWOULDBLOCK)
    {
        closesocket(link->socket);
        link->socket = 0;
        return 0;
    }
    link->type = NLT_TCP;
    link->pending = 1;
    link->connecting = 0;
    
    return 1;
}

/* Function netSendDisconnect()
 *    Send a request to disconnect on the specified link.  Wait for the disconnect
 *    acknowledgement for up to "timeout" number of seconds before terminating the
 *    connection.
 *
 */
void netSendDisconnect(NetLink *link,F32 timeout){
    Packet*    pak;
    int        timer;

    netioEnterCritical();
    if (link->socket <= 0)
    {
        netioLeaveCritical();
        return;
    }
    timer = timerAlloc();        // Timer starts here? But the disconnect packet haven't been sent yet.
    timerStart(timer);

    pak = pktCreateControlCmd(link, COMMCONTROL_DISCONNECT);
    pktSend(&pak, link);

    // Wait in blocking mode until the timer expires.
    // It might be a good idea to move this into the NetLink so that the timer can be
    // run on serverside also.
    while(1){
        if(netLinkMonitorInternal(link, COMMCONTROL_DISCONNECT_ACK, true, NULL)){
            break;
        }
        if(timerElapsed(timer) >= timeout){
            break;
        }
        netIdle(link,1,10);
    }
        
    timerFree(timer);

    // It could have been disconnected in netIdle if the server disconnected
    if (link->connected){
        // If the link belongs to a NetLinkList, ask for it to be cleaned up.
        if(link->parentNetLinkList){
            //printf("marking link for discard\n");
            netDiscardDeadLink(link);
        }else{
            // Otherwise, just wipe it clean.
            clearNetLink(link);
        }
    }
    netioLeaveCritical();
}

/****************************************************************************************************
 * NetLink Connection/Disconnection Helpers                                                            *
 ****************************************************************************************************/
/* Function netOpenSocket() (For client only)
 *    Prepares a netlink that is capable of communicating with the specified port
 *    on the specified address.
 *
 *    Note that UDP and TCP connects behaves slightly different.  For TCP connections,
 *    the usual call to connect() is made to establish a connection with the specified
 *    address and port.  For UDP connections, however, no attempt is made to actually 
 *    *connect* to the specified address and port. Therefore, there is no way to tell 
 *    if there is actually someone at the other end to receive data sent on the link 
 *    once netOpenSocket is finished "connecting" a UDP connection.  See netOpenSocketEx() 
 *    for a remedy.
 *
 *    Parameters:
 *        link    - the netlink to be prepared.
 *        address - the ip address to connect to in either numeric or qualified named form.
 *        port    - the port to establish a connection on.
 *        tcp        - whether the connection is tcp based.
 *                0 - UDP based
 *                1 - TCP based
 *
 *    Note that for UDP based sockets, the socket is initialized only.  An attempt at 
 *    establishing a connection is not made.
 */
int netOpenSocket(NetLink *link,const char *address,int port,int tcp){
    int        ret;
    int        one = 1;
    
    netioEnterCritical();
    if (!tcp)
        ret = netOpenSocketUdp(link,address,port);
    else
        ret = netOpenSocketTcp(link,address,port);
    if (!ret){
        // FIXME!!!
        // Might be a good idea to clearNetLink() to do the cleanup instead.
        if(link->socket){
            closesocket(link->socket);
            link->socket = 0;
        }
        netioLeaveCritical();
        return 0;
    }
    if (tcp)
        setsockopt(link->socket,IPPROTO_TCP,TCP_NODELAY,(void *)&one,sizeof(one));
    
    initNetLink(link);
    netioLeaveCritical();
    return ret;
}

/* Function netOpenSocketTcp() (For client only)
 *
 */
static int netOpenSocketTcp(NetLink *link,const char *address,int port){
    int        ret;

    link->socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (link->socket < 0)
    {
        return 0;
    }

    sockSetAddr(&link->addr,ipFromString(address),port);
    ret = connect(link->socket,(struct sockaddr *)&link->addr,sizeof(link->addr));
    if (ret != 0)
    {
        closesocket(link->socket);
        link->socket = 0;
        return 0;
    }
    link->type = NLT_TCP;
    
    sockSetBlocking(link->socket,0);
    
    return 1;
}

/* Function netOpenSocketUdp() (For client only)
 *
 */
static int netOpenSocketUdp(NetLink *link,const char *address,int port){
    unsigned int ip;

    link->socket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

    // Was the socket created successfully?
    if ((int)link->socket < 0)
    {
        int code = WSAGetLastError();
        UNUSED(code);
        return 0;
    }

    // Got a valid address?
    ip = ipFromString(address);
    if(INADDR_NONE == ip)
        return 0;

    sockSetAddr(&link->addr,ip,port);

    link->type = NLT_UDP;
    sockSetBlocking(link->socket,0);
    return 1;
}





/****************************************************************************************************
 * NetLink Monitoring                                                                                *
 ****************************************************************************************************/

/* Function netLinkMonitor()
 *    This is the all in one function for clients.  this function watches a single NetLink
 *    for incoming data as well as process them.
 *
 *    Returns:
 *        0 - command not found.
 *        1 - command found and processed.
 *        -1 - command found and processed, but the handler returned 0.
 *        -2 - the link was disconnected unexpectedly.
 */
static int netLinkMonitorInternal(NetLink* link, int lookForCommand, bool isControlCommand, NetPacketCallback* netCallBack){
    int    ret;

    netioEnterCritical();

    // These are also updated in NMonitor(), but not everybody uses NMonitor().
    g_NMCachedTimeSeconds = timerCpuSeconds();
    g_NMCachedTimeTicks = timerCpuTicks();

    if(link->type == NLT_NONE)
    {
        netioLeaveCritical();
        return LINK_DISCONNECTED;
    }

    // Grab all the data from the netlink.
    lnkBatchReceive(link);

    // Process all received data.
    ret = netMessageScan(link, lookForCommand, isControlCommand, netCallBack);

    if(link->disconnected){
        if(!link->parentNetLinkList)
            clearNetLink(link);
        netioLeaveCritical();
        return LINK_DISCONNECTED;
    }

    netioLeaveCritical();
    return ret;
}

int netLinkMonitor(NetLink* link, int lookForCommand, NetPacketCallback* netCallBack)
{
    return netLinkMonitorInternal(link, lookForCommand, false, netCallBack);
}


int netLinkMonitorBlock(NetLink* link, int lookForCommand, NetPacketCallback* netCallBack, int timeout){
    FD_SET readSet;
    FD_SET writeSet;
    FD_SET errorSet;
    int blockBeginTime;
    float fTimeout;
    int monitorResult;
    
    netioEnterCritical();
    fTimeout = (float)timeout / 1000.0f;
    monitorResult = netLinkMonitor(link, lookForCommand, netCallBack);
    if(LINK_DISCONNECTED == monitorResult)
    {
        netioLeaveCritical();
        return LINK_DISCONNECTED;
    }
    if(monitorResult)
    {
        netioLeaveCritical();
        return monitorResult;
    }
    
    // Wait for incoming packets as long as the specified timeout allows.
    while(fTimeout >= 0){
        // Wait for incoming network packet.
        blockBeginTime = timerCpuTicks();
        FD_ZERO(&readSet);
        FD_ZERO(&writeSet);
        FD_ZERO(&errorSet);
        FD_AddLink(&readSet, &writeSet, &errorSet, link);
        if(simpleSelect(&readSet, &writeSet, &errorSet, fTimeout) >= 0){
            // Extract and process incoming network packet.
            monitorResult = netLinkMonitor(link, lookForCommand, netCallBack);
            if(LINK_DISCONNECTED == monitorResult)
            {
                netioLeaveCritical();
                return LINK_DISCONNECTED;
            }

            // If we find the packet we're looking for, we are done.
            if(monitorResult) {
                netioLeaveCritical();
                return monitorResult;
            }
            
            // Otherwise, update the amount of time until we quit looking
            // for the packet.
        }
        fTimeout -= timerSeconds(timerCpuTicks() - blockBeginTime);
    }

    // We didn't find the packet.
    netioLeaveCritical();
    return 0;
}

/****************************************************************************************************
 * NetLink Helpers                                                                                    *
 ****************************************************************************************************/
/* Function netAckConnect
 *    Send a connect ack packet on the specified link.
 *
 *
 */
static void netAckConnect(NetLink *link, int encrypt, CommControlCommand cmd){
    Packet*    pak;

    pak = pktCreateControlCmd(link, cmd);
    if (encrypt) {
        assert(packetCanUseEncryption());

        pktSendBitsPack(pak,1,1); // use encryption

        // FIXME!!!
        //  Why does the make key pair function sometimes return empty keys?
        //    This causes the encryption key negotiation to fail.
        do{
            cryptMakeKeyPair(link->private_key,link->public_key);
        }while(0 == strlen((U8 *)link->public_key));

        pktSendBitsArray(pak,512,link->public_key);
    }
    else
        pktSendBitsPack(pak,1,0);
    pktSend(&pak,link);
}

/* Function netAckDisconnect() (For server only)
 *    Send a disconnect ack packet on the specified link.
 *
 *
 */
static void netAckDisconnect(NetLink *link){
    Packet*    pak;

    pak = pktCreateControlCmd(link, COMMCONTROL_DISCONNECT_ACK);
    pktSend(&pak,link);
    lnkBatchSend(link);

    // must come second. otherwise link looks dead and nothing can be sent on it.
    if(link->parentNetLinkList){
        netDiscardDeadLink(link);
    }
}

Packet *getStoredPacket(NetLink *link)
{
    Packet    *pak;

    if (!link->pkt_temp_store || qIsEmpty(link->pkt_temp_store))
        return 0;
    pak = qDequeue(link->pkt_temp_store);
    return pak;
}

int putStoredPacket(NetLink *link,Packet *pak)
{
    if (!link->pkt_temp_store) {
        link->pkt_temp_store = createQueue();
        initQueue(link->pkt_temp_store, 16);
    }
    qEnqueue(link->pkt_temp_store, pak);
    return 1;
}

// This functions handles all low-level network control commands (cmd < COMMCONTROL_MAX_CMD)
static int handleControlCommand(Packet *pak, CommControlCommand cmd, NetLink *link)
{
    netLog(link, "handleControlCommand(%d)\n", cmd);
    switch(cmd)
    {
        xcase COMMCONTROL_CONNECT:
            {
                // If the link does not already have an established connection,
                // accept the connection by sending back an connection acknowledgement.
                // Note that this means links that have existing connections will
                // ignore all future connection requests.  A temporarily disconnected
                // client will not be able to connect up to the same link again.

                // Get the client's requested network version, which must be at least as
                // high as our server's network version, and let them know what version
                // we're going to be used
                U32 connect_id = pktGetBitsPack(pak, 1);
                U32 requsted_version = 0;
                
                if(link)
                    link->connect_id = connect_id;
                
                if (!pktEnd(pak)) {
                    requsted_version = pktGetBitsPack(pak, 1);
                }
                if (requsted_version > getDefaultNetworkVersion()) {
                    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "Attempted connect from too new of a version (%d) from %s:%d, dropping link!\n", requsted_version, makeIpStr(link->addr.sin_addr.S_un.S_addr), link->addr.sin_port);
                    netAckDisconnect(link);
                    link = NULL;
                    return 0;
                }
                if (requsted_version != getDefaultNetworkVersion()) {
                    //printf("Warning: Accepted a connection from an older version, this will only work if all packets are created with pktCreateEx()\n");
                }
                netLog(link, "Accepted connection packet with version %d, key %x\n", requsted_version, connect_id);
                //printf("Server: Client requested version %d, applying\n", requsted_version);
                applyNetworkVersionToLink(link, requsted_version);
                if(!link->connected){
                    netAckConnect(link,link->encrypted, COMMCONTROL_CONNECT_SERVER_ACK);
                }
            }
        xcase COMMCONTROL_CONNECT_SERVER_ACK:
        case COMMCONTROL_CONNECT_CLIENT_ACK:
            // The other party has acknowledged the connection request.  This means
            // there is really someone at the other end of the line to respond to
            // messages.  A connection has been established.  Reply with another
            // connection acknowledgement to complete the handshake.
            // Each party knows they are fully connected once they receive a single
            // connection acknowledgement.  All future connection acks will be
            // ignored.
            if(!link->connected){

                U32    md5_buf[4];
                U32    their_public_key[512/32];
                int encrypt = pktGetBitsPack(pak,1);

                if (cmd == COMMCONTROL_CONNECT_SERVER_ACK)
                {
                    // Code ran on client only
                    link->encrypted = encrypt;

                    netAckConnect(link,encrypt,COMMCONTROL_CONNECT_CLIENT_ACK);
                }
                if (encrypt)
                {
                    pktGetBitsArray(pak,512,their_public_key);
                    cryptMakeSharedSecret(link->shared_secret,link->private_key,their_public_key);    // Why is the private key always empty?
                    cryptMD5Update((U8 *)link->shared_secret,512/8);
                    cryptMD5Final(md5_buf);
                    cryptBlowfishInitU32(&link->blowfish_ctx,md5_buf,ARRAY_SIZE(md5_buf));
                    link->decrypt_on = 1;
                }
                if (cmd == COMMCONTROL_CONNECT_CLIENT_ACK)
                {
                    // Code ran on server only
                    Packet    *pak = pktCreateControlCmd(link, COMMCONTROL_CONNECT_SERVER_ACK_ACK);
                    pktSend(&pak,link);

                    link->encrypt_on = link->encrypted;
                    link->connected = 1;
                }
            }

        xcase COMMCONTROL_CONNECT_SERVER_ACK_ACK:
            link->encrypt_on = link->encrypted;
            link->connected = 1;

        xcase COMMCONTROL_DISCONNECT:
            // The other party has sent a disconnect request.  Send back an acknowledgement
            // then disconnect without waiting for an acknowledgement.  Note that this means
            // the party initiating the disconnect will need to wait on a timeout.  This is
            // undesirable for servers with high volume traffic.  Let the client initiate the
            // request in such a case.
            netAckDisconnect(link);
            link = NULL;
            return 0;
        xcase COMMCONTROL_DISCONNECT_ACK:
            // Disreguard disconnect acknowledgements because the other party has already
            // terminiated their connection.

        xcase COMMCONTROL_BUFFER_RESIZE:
        {
            // The other party has resized their send buffer, we should resize our receive buffer accordingly
            U32 newsize = pktGetBits(pak, 32);
            //printf("Got resize message from other party, resizing to %d bytes\n", newsize);
            netLog(link, "Received resize command, resizing buffer to %d bytes\n", newsize);
            if (newsize <= link->recvBufferMaxSize && newsize > link->recvBufferSize)
                netLinkSetBufferSize(link, ReceiveBuffer, newsize);
        }
        xcase COMMCONTROL_IDLE:
        {
            // Got a idle packet.  Do nothing.
            // When looking at per cycle stats, we do not care about idle packets.
            link->idlePacketsReceived++;
            break;
        }
    }
    return 1;
}

int debug_oldBitPosition=-1; // For restoring/rerunning the state in a crash situation

/* Function netMessageScan()
 *    This function is used as an entry point to process all packets.
 *
 *    This function automatically extracts a packet type ID from the packet and 
 *    processes if possible.  If this function does not know how to process such 
 *    a packet and a callback is provided, all information required to process the 
 *    packet is passed to the callback function for processing.
 *
 *    All packets are assumed to have used pktSendBitsPack(, 1,) to place a numeric 
 *    identifier into the back as the first element in the packet.  This ID identifies
 *    what type of packet this packet is.  This number is also referred to as the
 *    "command" specified by the packet.
 *    
 *  isControlCommand specifies whether the value in lookForCommand is a control command
 *  or a user defined command.
 *
 *    Enhancement!
 *        It might be a good idea to be able to accept a list of callbacks instead of just 
 *        one.  This means that individual callbacks do not have to be chained together in
 *        advance.  It becomes easier to write individual network service modules (which 
 *        will have their own callbacks to handle all related commands) and be able to 
 *        combine them arbitrarily.
 *
 *    Returns:
 *        0 - the command was not found.
 *        1 - the command was found and processed.
 *        -1 - the command was found and processed, but the handler returned 0.
 */
int netMessageScan(NetLink *link, int lookForCommand, bool isControlCommand, NetPacketCallback *netCallBack){
    Packet*    pak;
    int        cmd, foundCommand = 0, controlcmd=-1;
    bool    foundControlCommand;
    int linkWasFull = 0;
    U32        cookie = 0;
    BitStreamCursor    cursor;

    if(!link || !link->socket || NLT_NONE == link->type)
        return 0;

    netioEnterCritical();
    // Is the link really full right now?
    linkWasFull = qIsReallyFull(link->receiveQueue);

    while(link && link->socket && !mpReclaimed(link))
    {
        pak = 0;
        if (lookForCommand <= 0)
            pak = getStoredPacket(link);
        if (!pak)
            pktGet(&pak,link);
        if (!pak)
            break;
        cursor = pak->stream.cursor;
        cmd = pktGetBitsPack(pak,1);
        foundControlCommand = false;
        controlcmd = -1;
        // Attempt to extract a control command if there is one, based on what network version the client is using
        if (link->controlCommandsSeparate && cmd == COMM_CONTROLCOMMAND) {
            foundControlCommand = true;
            if (pktEnd(pak)) {
                controlcmd = COMMCONTROL_IDLE;
            } else {
                controlcmd = pktGetBitsPack(pak,1);
                if (controlcmd < 0 || controlcmd >= COMMCONTROL_MAX) {
                    badPacketLog(BADPACKET_BADDATA, link->addr.sin_addr.S_un.S_addr, link->addr.sin_port);
                    //printf("Received bad control command in packet!  Dropping packet.\n");
                    pktFree(pak);
                    pak = NULL;
//                     netioLeaveCritical();
//                     return 0;
                    foundCommand = 0;
                    break;                // BREAK IN FLOW
                }
            }
        } else if (!link->controlCommandsSeparate || cmd == COMM_CONNECT) {
            // Old network format where control commands are 0-6, user are 7+
            if (cmd < COMM_OLDMAX_CMD) {
                foundControlCommand = true;
                controlcmd = cmd;
                cmd = COMM_CONTROLCOMMAND;
            } else {
                // Not a control command, but is really higher than what it should be under the new scheme!
                cmd = cmd - COMM_OLDMAX_CMD + COMM_MAX_CMD;
            }
        }

        // Look for cookies
        if (!foundControlCommand && (link->cookieSend || link->cookieEcho))
        {
            link->cookie_recv = cookie = pktGetBits(pak,32);
            link->last_cookie_recv_echo = pktGetBits(pak,32);
            if (cookie)
                link->last_cookie_recv = cookie;
        }
        if (lookForCommand > 0)
        {
            if (link->cookieSend && !foundControlCommand)
            {
                if (cookie == link->cookie_send)
                    foundCommand = 1;
            }
            else
            {
                if (!isControlCommand) {
                    // Looking for a normal command
                    if (cmd == lookForCommand)
                        foundCommand = 1;
                } else {
                    if (foundControlCommand && controlcmd == lookForCommand)
                        foundCommand = 1;
                }
            }
            
            if(!link->alwaysProcessPacket && !foundCommand && !foundControlCommand)
            {
                pak->stream.cursor = cursor;
                putStoredPacket(link,pak);
                continue;
            }
        }

        if (foundControlCommand)
            link->last_control_command_packet_id = pak->id;
        else
            link->last_processed_id = pak->id;

        assert((controlcmd==-1) == (!foundControlCommand));
        if (foundControlCommand) {
            debug_oldBitPosition = bsGetCursorBitPosition(&pak->stream);
            if (0==handleControlCommand(pak, controlcmd, link)) {
                pktFree(pak);
                pak = NULL;
//                 netioLeaveCritical();
//                 return 0;
                foundCommand = 0;
                break;
            }
        } else {
            // The command cannot be handled by these default handlers.  Let the callback
            // handle it.
            if(netCallBack){
                static int reset_pos = 0;
                int ret;

//printf("%d: packet size: %d bytes\n", time(NULL), pak->stream.size);

                debug_oldBitPosition = bsGetCursorBitPosition(&pak->stream);
                if(reset_pos) // debugger was optimizing this out
                {
                    bsSetCursorBitPosition(&pak->stream, debug_oldBitPosition);
                }

                ret = netCallBack(pak,cmd,link);
                if (ret == NETLINK_MONITOR_DO_NOT_FREE_PACKET)
                {
                    pak = NULL;
                }
                if (ret == NETLINK_MONITOR_ABORT_AND_DO_NOT_FREE_PACKET)
                {
                    pak = NULL;
                    break;
                }
                else if ((!ret && foundCommand) || ret < 0)
                    foundCommand = -1;
            }
        }
        pktFree(pak);
        pak = NULL;

        if (lookForCommand > 0 && foundCommand)
            break;
        if (NMMonitorShouldStopReceiving())
            break;
    }

    // Flush the link.
    if(link && !mpReclaimed(link))
        lnkBatchSend(link);

    // If the link was really full, and it was performing async reads, the full queue
    // would have prevented new async reads from being initiated.
    // Initiate a new async read now.
    if(linkWasFull && link->asyncTcpState != AST_None)
    {
        lnkReadTCPAsync(link, 0);
    }

    // --------------------
    // if there are any packets read but not processed, the async read
    // mechanism needs to know about it. post a message to the io
    // completion port
    // Because of the complexity of the link-linklist interaction, 
    // do this even on links that claim they are synchronous. 
    // (is handled properly)

    NMLinkQueueProcessStoredPackets(link);

    // --------------------
    // finally

    netioLeaveCritical();
    return foundCommand;
}

/****************************************************************************************************
 * NetLink idle packets                                                                                *
 ****************************************************************************************************/
/* Function netIdle()
 *    The function ensures that the link is never idle for more than X seconds.
 *    If the link has been idle for more than X seconds, it sends a idle message
 *    to the peer to keep the connection alive. (Is this true?)
 *
 *    This function might be more appropriately named "netKeepAlive()".
 *
 *    From reading other parts of this module, it seems that the primary function
 *    of this function is to provide a way to send back packet acknowledgement
 *    when the a reliable packet receiver isn't sending any packets back to the
 *    sender.
 *
 *
 */
void netIdle(NetLink* link, int do_sleep, float maxIdleTime)
{
    netIdleEx(link, do_sleep, maxIdleTime, timerCpuTicks());
}


/* Function netIdleEx()
 *    The function ensure that the link is never idle for more than the specified amount
 *    of time by sending idle messages if neccessary.
 *
 */
void netIdleEx(NetLink* link, int do_sleep, float maxIdleTime, U32 current_time)
{
    Packet*    pak;
    F32        fdt;
    U32        dt;

    netioEnterCritical();
    if (link->ack_count < MAX_PKTACKS/2) { // If we have a lot of acks outstanding, just send an idle immediately
        if(maxIdleTime > 0.0){
            dt = current_time - link->lastSendTime;
            if (dt > 0x80000000) {
                // lastSendtime is later than current_time, we must have sent something this tick
                fdt = 0;
                dt = 0;
            } else {
                fdt = timerSeconds(dt);
            }
            if (fdt >= 0 && fdt < maxIdleTime)
            {
                if (do_sleep)
                    Sleep(1);
                netioLeaveCritical();
                return;
            }
        }
    }

    if (!link->connected || qGetSize(link->sendQueue2))
    {
        if (do_sleep)
            Sleep(1);
        netioLeaveCritical();
        // Don't send idles before the link has been established, the network version might not yet have been negotiated!
        // Also, no need to send idles if the sendQueue has something in it (this will only happen if the TCP buffers fill up)
        return;
    }

    // Do *not* disable idle packets.
    // Required by UDP for single-sided tranfers.
    // Required by TCP for as app level keep-alives.
    pak = pktCreateControlCmd(link, COMMCONTROL_IDLE);
    pak->reliable = 0;
    pak->ordered = 0;
    pktSend(&pak,link);
    lnkBatchSend(link);

    //JS: Is this needed?
    if (do_sleep)
        Sleep(1);
    netioLeaveCritical();
}

/* Function netSendIdle()
 *    Adds a idle packet to the given link.  The packet will be sent out the next time
 *    all queued packets are sent, usually at the end of netLinkMonitor()
 *
 *
 *
 */
void netSendIdle(NetLink* link){
    Packet*    pak;

    if(link->ack_count){
        pak = pktCreateControlCmd(link, COMMCONTROL_IDLE);
        pak->reliable = 0;
        pktSend(&pak,link);
    }
}


/****************************************************************************************************
 * Misc functions                                                                                    *
 ****************************************************************************************************/

void lnkDumpUnsent(NetLink *link)
{
#ifdef TRACK_PACKET_ALLOC
    int        i;
    Packet    *pak;

    if (!link->reliablePacketsArray.size)
        return;
    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "\n\n\n\n\n\n\n\n\n-------------------------------------------------------------\n");
    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "dumping %d unsent packets on link %08x",link->reliablePacketsArray.size,link);
    for(i=0;i<link->reliablePacketsArray.size;i++)
    {
        pak = link->reliablePacketsArray.storage[i];
        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "%d:%d/%d %s\n",pak->id,pak->sib_id,pak->sib_count,pak->stack_dump);
    }
    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "\n-------------------done-------------------------------------------\n");
#endif
}

void dumpPacketArray(Array* array){
    int i;
    printf("Begin\n");
    for(i = 0; i < array->size; i++){
        Packet* pak = array->storage[i];
        printf("ID: %i\n", pak->id);
    }
    printf("End\n");
}

static unsigned int lnkReceivedPakIDHash(void* value){
    return PTR_TO_U32(value);
}

/****************************************************************************************************
 * Convenience functions                                                                            *
 ****************************************************************************************************/

Packet *pktCreateEx(NetLink *link,int cmd)
{
    Packet *pak = pktCreateImp("pktCreateEx",__LINE__);
    assert(cmd >= COMM_MAX_CMD);
    pak->link = link;
    applyNetworkVersionToPacket(link, pak);
    //if (link == (NetLink*)PACKET_COMPRESSED)
    //    link = NULL;
    pktSendCmd(link,pak,cmd);
    return pak;
}

Packet *pktCreateControlCmd(NetLink *link, CommControlCommand controlcmd)
{
    Packet *pak = pktCreateImp("pktCreateControlCmd",0);
    pak->link = link;
    applyNetworkVersionToPacket(link, pak);
    if (link->controlCommandsSeparate && controlcmd != COMMCONTROL_CONNECT) {
        pktSendBitsPack(pak, 1, COMM_CONTROLCOMMAND);
        if (controlcmd!=COMMCONTROL_IDLE)
            pktSendBitsPack(pak, 1, controlcmd);
    } else {
        if (controlcmd >= COMM_OLDMAX_CMD) {
            printf("Attempt to send a new control command to an old client!  Bad!  Sending idle instead\n");
            pktSendBitsPack(pak, 1, COMMCONTROL_IDLE);
        } else {
            pktSendBitsPack(pak, 1, controlcmd);
        }
    }
    return pak;
}


void netLinkSetMaxBufferSize(NetLink *link, NetLinkBufferType type, int size)
{
    if (type & SendBuffer) {
        link->sendBufferMaxSize = size;
    }
    if (type & ReceiveBuffer) {
        link->recvBufferMaxSize = size;
    }
}

void netLinkListSetMaxBufferSize(NetLinkList *nlist, NetLinkBufferType type, int size)
{
    if (type & SendBuffer) {
        nlist->sendBufferMaxSize = size;
    }
    if (type & ReceiveBuffer) {
        nlist->recvBufferMaxSize = size;
    }
}

#define DELAY_BETWEEN_RESIZES (timerCpuSpeed() >> 4) // Wait at least 1/16th of a second between resizing any given network buffer

void netLinkAutoGrowBuffer(NetLink* link, NetLinkBufferType type)
{
    U32 newsize;
    bool useNetLink; // true -> NetLink, false -> NetLinkList
    NetLinkList *nlist = link->parentNetLinkList;
    U32 lastresize;
    U32 curTime = timerCpuTicks();
    if (link->type == NLT_TCP) {
        // 1 socket per link, we're good to go
        useNetLink = true;
    } else if (link->type == NLT_UDP) {
        // Might be 1 socket for lots of links, let's check it's parent list?
        if (link->parentNetLinkList) {
            useNetLink = false;
        } else {
            useNetLink = true;
        }
    }
    else {
        netLog(link, "Invalid link type: %d\n", link->type);
        return;
    }
#define LINKVAR(var) (useNetLink?link->var:nlist->var)
    lastresize = LINKVAR(lastAutoResizeTime);
    if (curTime - lastresize < DELAY_BETWEEN_RESIZES) {
        // We just resized it a bit ago, wait a bit more!
        return;
    } else {
        if (useNetLink) {
            link->lastAutoResizeTime = curTime;
        } else {
            nlist->lastAutoResizeTime = curTime;
        }
    }
    if (type & SendBuffer) {
        newsize = MIN(LINKVAR(sendBufferSize)*2, LINKVAR(sendBufferMaxSize));
        if (newsize > LINKVAR(sendBufferSize)) {
            netLog(link, "Auto-grew send buffer to %d bytes\n", newsize);
            if (useNetLink) {
                netLinkSetBufferSize(link, SendBuffer, newsize);
            } else {
                netLinkListSetBufferSize(nlist, SendBuffer, newsize);
            }

            if (useNetLink && link->type == NLT_TCP && link->controlCommandsSeparate) {
                // Notify the other end of the link that we grew!  But not if this is a UDP netLinkList, since the buffer is shared!
                Packet *pak = pktCreateControlCmd(link, COMMCONTROL_BUFFER_RESIZE);
                pktSendBits(pak, 32, newsize);
                pktSend(&pak, link);
            } else {
                // We are not on a link that notifies of send buffer growth, so we should assume
                // symetry and resize our receive buffer as well
                if (!(type & ReceiveBuffer) && newsize > LINKVAR(recvBufferSize)) {
                    if (useNetLink) {
                        netLinkSetBufferSize(link, ReceiveBuffer, newsize);
                    } else {
                        netLinkListSetBufferSize(nlist, ReceiveBuffer, newsize);
                    }
                }
            }
        }
    }
    if (type & ReceiveBuffer) {
        newsize = MIN(LINKVAR(recvBufferSize)*2, LINKVAR(recvBufferMaxSize));
        if (newsize > LINKVAR(recvBufferSize)) {
            netLog(link, "Auto-grew receive buffer to %d bytes\n", newsize);
            if (useNetLink) {
                netLinkSetBufferSize(link, ReceiveBuffer, newsize);
            } else {
                netLinkListSetBufferSize(nlist, ReceiveBuffer, newsize);
            }
        }
    }
#undef LINKVAR
}

void netLinkSetBufferSize(NetLink* link, NetLinkBufferType type, int size)
{
    assert(!(link->type == NLT_UDP && link->parentNetLinkList));
    if (type & SendBuffer) {
        link->sendBufferSize = size;
    }
    if (type & ReceiveBuffer) {
        link->recvBufferSize = size;
    }
    socketSetBufferSize(link->socket, type, size);
}

void netLinkListSetBufferSize(NetLinkList* nlist, NetLinkBufferType type, int size)
{
    assert(nlist->udp_sock && !nlist->listen_sock); // We'd have to iterate over all TCP Links, this function is UDP only
    if (type & SendBuffer) {
        nlist->sendBufferSize = size;
    }
    if (type & ReceiveBuffer) {
        nlist->recvBufferSize = size;
    }
    socketSetBufferSize(nlist->udp_sock, type, size);
}

void netLog(NetLink *link, const char *fmt, ... )
{
    return; // NO DEBUG LOGGING

#if 0 // poz ifdefed unreachable code
    va_list va;
    char buf2[1024];
    char buf[1024];

    if (link) {
        sprintf_s(SAFESTR(buf2), "[%s:%d] %s", makeIpStr(link->addr.sin_addr.S_un.S_addr), link->addr.sin_port, fmt);
    } else {
        sprintf_s(SAFESTR(buf2), "[NULL] %s", fmt);
    }
    va_start(va, fmt);
    vsprintf(buf, buf2, va);
    va_end(va);

    if (IsDebuggerPresent()) {
        OutputDebugString(buf);
    } 
    else
    {
        printf("%s", buf);
    }

    if (buf[strlen(buf)-1]=='\n') {
        buf[strlen(buf)-1]=0;
    }
    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "%s", buf);
#endif
}
