#include "utilitieslib/network/net_linklist.h"
#include "utilitieslib/network/net_link.h"
#include "utilitieslib/network/net_socket.h"
#include "utilitieslib/network/netio_core.h"
#include "utilitieslib/network/netio_enet.h"
#include "utilitieslib/network/net_masterlist.h"
#include "utilitieslib/network/net_structdefs.h"
#include "utilitieslib/network/netio_receive.h"
#include "utilitieslib/network/net_packet.h"

#include <stdio.h>
#include "utilitieslib/network/sock.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/log.h"

#ifndef FINAL
extern int isDevelopmentMode(void);
#else
#define isDevelopmentMode() 0
#endif

/****************************************************************************************************
 * Static function forward declarations                                                                *
 ****************************************************************************************************/
static void netLinkListRemoveDisconnected(NetLinkList* list);


/****************************************************************************************************
 * NetLinkList initialization                                                                        *
 ****************************************************************************************************/
/* Function netInit()
 *    Initializes a the given NetLinkList so it is ready to accept connections
 *    on the specified ports.
 *
 *    Parameters:
 *        nlist - The NetLinkList to be initialized.
 *        udp_port - The udp port to wait for connection on.  (This parameter is optional)
 *        tcp_port - The tcp port to wait for connection on.  (This parameter is optional)
 *
 */
int netInit(NetLinkList *nlist,int udp_port,int tcp_port){
    struct sockaddr_in    addr_in;

    netioEnterCritical();
    if (udp_port)
    {
        nlist->udp_sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
        sockSetAddr(&addr_in,htonl(INADDR_ANY),udp_port);
        if (!sockBind(nlist->udp_sock,&addr_in))
            goto fail;
        sockSetBlocking(nlist->udp_sock,0);
    }

    if (tcp_port)
    {
#ifndef _XBOX
        nlist->listen_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
#else
        nlist->listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif
        sockSetAddr(&addr_in,htonl(INADDR_ANY),tcp_port);
        if (!sockBind(nlist->listen_sock,&addr_in))
            goto fail;
        //sockSetBlocking(nlist->listen_sock,0);
        //netAddListenSock(nlist->listen_sock);
        {
            int result;
            result = listen(nlist->listen_sock, 15);
            if(result)
            {
                printf("listen error..\n");
                netioLeaveCritical();
                return 0;
            }
        }
    }
    nlist->sendBufferSize = 8*1024; // OS default
    nlist->sendBufferMaxSize = 256*1024;
    nlist->recvBufferSize = 8*1024; // OS default
    nlist->recvBufferMaxSize = 256*1024;
    netioLeaveCritical();
    return 1;

fail:
    if (nlist->udp_sock)
    {
        closesocket(nlist->udp_sock);
        nlist->udp_sock = 0;
    }
    if (nlist->listen_sock)
    {
        closesocket(nlist->listen_sock);
        nlist->listen_sock = 0;
    }
    netioLeaveCritical();
    return 0;
}



void netLinkListDisconnect(NetLinkList *nlist)
{
    netioEnterCritical();
    if (nlist->udp_sock)
    {
        closesocket(nlist->udp_sock);
        nlist->udp_sock = 0;
    }
    if (nlist->listen_sock)
    {
        closesocket(nlist->listen_sock);
        nlist->listen_sock = 0;
    }
    if (nlist->enet_host) {
	    netEnetListShutdown(nlist);
    }
    // Could add a loop over all links to send disconnect signals on them here
    netioLeaveCritical();
}

/* Function netLinkListAlloc() (For server only)
 *    The function allocates enough memory to hold the specified number of links, along
 *    with some number of user defined data to be held by the NetLinkList.  The given
 *    callback will be called when a link is being added.
 *
 *    This function now only allocates enough memory to old 1/4 of the requested number of
 *    links.  The intention is for the numlinks to be the expected number of links.  The
 *    available pool of links and user defined data memory will grow as needed.
 *
 */
int netLinkListAlloc(NetLinkList *nlist, int numlinks, int user_data_size, NetLinkAllocCallback cb)
{
    netioEnterCritical();

    nlist->expectedLinks = numlinks;

    if(numlinks>16) // arbitrary
        numlinks /= 4;
    
    // Initialize NetLink MemoryPool
    nlist->netLinkMemoryPool = createMemoryPoolNamed("NetLink", __FILE__, __LINE__);
    initMemoryPool(nlist->netLinkMemoryPool, sizeof(NetLink), numlinks);
    //mpSetMode(nlist->netLinkMemoryPool, TurnOffAllFeatures);

    // Initialize User Data MemoryPool
    if(user_data_size){
        nlist->userDataMemoryPool = createMemoryPoolNamed("NetLink::userData", __FILE__, __LINE__);
        initMemoryPool(nlist->userDataMemoryPool, user_data_size, numlinks);
    }

    // Create an array to hold active links.
    nlist->links = createArray();

    nlist->allocCallback = cb;
    netioLeaveCritical();
    return 1;
}

static void createUDPLinkHashKey(struct sockaddr_in* addr, char* output){
    unsigned int u32addr = addr->sin_addr.s_addr;
    unsigned int u32port = addr->sin_port;

    *output++ = 'U';
    *output++ = ':';
    
    while(1){
        *output++ = 'a' + (u32addr & 0xf);
        u32addr >>= 4;
        if(u32addr == 0)
            break;
    }
    
    while(1){
        *output++ = 'a' + (u32port & 0xf);
        u32port >>= 4;
        if(u32port == 0)
            break;
    }

    *output = 0;
}

static void createTCPLinkHashKey(SOCKET socket, char* output){
    *output++ = 'T';
    *output++ = ':';

    while(1){
        *output++ = 'a' + (socket & 0xf);
        socket >>= 4;
        if(socket == 0)
            break;
    }

    *output = 0;
}

static void createEnetLinkHashKey(U32 uid, char *output)
{
	*output++ = 'E';
	*output++ = ':';

	while (1) {
		*output++ = 'a' + (uid & 0xf);
		uid >>= 4;
		if (uid == 0)
			break;
	}

	*output = 0;
}

static void createLinkHashKey(NetLink* link, char* output){
    switch(link->type){
        case NLT_UDP:
            createUDPLinkHashKey(&link->addr, output);
            break;
        case NLT_TCP:
            createTCPLinkHashKey(link->socket, output);
	    break;
	case NLT_ENET:
		// ENet identity is the peer, not (addr,port); the UID key just
		// has to be unique so the stash add/remove pair stays balanced.
		createEnetLinkHashKey(link->UID, output);
		break;
        default:
            assert(0);
    }
}

/****************************************************************************************************
 * NetLinkList element management                                                                    *
 ****************************************************************************************************/
/* Function netAddLink() (For server only)
 *    Allocate an additional link from a pool of available links from the given
 *    NetLinkList.
 *
 */
static NetLink *netAddLink(NetLinkList *nlist, struct sockaddr_in *addr, int socket, NetLinkType type){
    NetLink    *link;
    void* userData;
    char hashKey[100];
    StashElement element;

    if (!nlist->publicAccess && !isLocalIp(addr->sin_addr.S_un.S_addr))
    {
        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "public connection attempt on private link from %s\n",makeIpStr(addr->sin_addr.S_un.S_addr));
        return 0;
    }

    // Create a new link
    //    Allocate a pice of memory for use as a link.
    link = mpAlloc(nlist->netLinkMemoryPool);
    
    //    Initialize the link.
    initNetLink(link);
    link->addr = *addr;
    link->recvBufferMaxSize = nlist->recvBufferMaxSize;
    link->sendBufferMaxSize = nlist->sendBufferMaxSize;
    link->type = type;
    
    // Create some user data
    //    Allocate some user data space and attach it to the new link.
    if(nlist->userDataMemoryPool){
        userData = mpAlloc(nlist->userDataMemoryPool);
        link->userData = userData;
    }

    if (socket) {
        link->socket = socket;
    } else if (nlist->udp_sock && !link->socket)
        link->socket = nlist->udp_sock;

    link->parentNetLinkList = nlist;
    link->flowControl = 1;
    link->bandwidthLowerLimit = 20 * 1024 / 8;            // 20kbit
    link->estimatedBandwidth = 5 * 1024 * 1024 / 8;        // 5mbit
    link->bandwidthUpperLimit = 10 * 1024 * 1024 / 8;    // 10 mbit
    link->lastSendTime = timerCpuTicks();
    link->lastSendTimeSeconds = 0;
    link->retransmit = 1;

    // Add to the list of links.

    if(!nlist->netLinkTable){
        nlist->netLinkTable = stashTableCreateWithStringKeys(100, StashDeepCopyKeys | StashCaseSensitive );
    }
    
    createLinkHashKey(link, hashKey);
    
    stashFindElement(nlist->netLinkTable, hashKey, &element);
    
    assert(!element);
    
    stashAddPointer(nlist->netLinkTable, hashKey, link, false);

    //    Add the new link to a list of active links.
    arrayPushBack(nlist->links, link);
    
    link->linkArrayIndex = nlist->links->size - 1;

    // Call the user defined initialization function to initialize the link
    // and/or user data.    
    if (nlist->allocCallback)
        nlist->allocCallback(link);

    //printf("ClientID = %d\n", nlist->links->size);
    return link;
}

/* Function netAddLinkEnetAccept() (For server only)
 *    Accepts an incoming ENet peer as a NetLink on the given list. Called from
 *    netio_enet.c on ENET_EVENT_TYPE_CONNECT; kept here so the memory-pool,
 *    stash, and allocCallback plumbing stays next to netAddLink.
 *
 *    ip is in network byte order, port in host byte order (ENet convention).
 *    Returns NULL if the connection is rejected (publicAccess gate).
 */
NetLink *netAddLinkEnetAccept(NetLinkList *nlist, U32 ip, int port,
			      SOCKET hostSocket)
{
	struct sockaddr_in addr = {0};
	NetLink *link;

	sockSetAddr(&addr, ip, port);

	// Pass 0 and assign the handle afterwards: netAddLink's socket
	// parameter is an int, which would truncate a 64-bit SOCKET.
	link = netAddLink(nlist, &addr, 0, NLT_ENET);
	if (link) {
		link->socket = hostSocket;
		// Mirror the legacy UDP accept (netValidateUDPPacket):
		// encryption policy comes from the list; the link is pumped
		// synchronously.
		link->encrypted = (U8)nlist->encrypted;
		link->opType = NLOT_SYNC;
		// ENet owns retransmission and throttling for these links.
		link->retransmit = 0;
		link->flowControl = 0;
	}
	return link;
}

/* Function netRemoveLinkInternal()
*    This function removes the given link from its parent linklist, destroys the given link, 
*    and returns the link to the memory pool it belongs to.
*/
void netRemoveLinkInternal(NetLink *link)
{
    NetLinkList* nlist;
    char hashKey[100];
    NetLink* fillLink;
    NetLink* removed;

    assert(link->parentNetLinkList);
    nlist = link->parentNetLinkList;

    if(nlist->destroyCallback && !link->destroyed)
    {
        nlist->destroyCallback(link);
        link->destroyed = 1;
    }

    if (nlist->userDataMemoryPool) {
        mpFree(nlist->userDataMemoryPool, link->userData);
    }
    
    createLinkHashKey(link, hashKey);
    

    stashRemovePointer(nlist->netLinkTable, hashKey, &removed);
    if(link != removed){
        assert(0);
    }
    
    // Remove the given link from the active NetLink list.

    fillLink = nlist->links->storage[nlist->links->size - 1];

    fillLink->linkArrayIndex = link->linkArrayIndex;
    
    assert(fillLink->linkArrayIndex < (U32)nlist->links->size);
    
    nlist->links->storage[fillLink->linkArrayIndex] = fillLink;
    
    nlist->links->size--;

    // Destroy the given link now.
    clearNetLink(link);
    mpFree(nlist->netLinkMemoryPool, link);
}

/* Function netRemoveLink()
 *    This function removes the given link from its parent linklist, destroys the given link, 
 *    and returns the link to the memory pool it belongs to.
 *
 *    If it's a TCP link and part of the NetMasterList, it just flags it for removal later
 */
void netRemoveLink(NetLink *link){
    NetLinkList* nlist;

    netioEnterCritical();
    assert(link->parentNetLinkList);
    nlist = link->parentNetLinkList;

    if (nlist->inNetMasterList && link->type == NLT_TCP) {
        netDiscardDeadLink(link);
    } else {
        netRemoveLinkInternal(link);
    }
    netioLeaveCritical();
}

/* Function findUdpNetLink()
 *    Locate a UDP based NetLink from the given list that connects to the specified address.
 *
 */
NetLink* findUdpNetLink(NetLinkList* nlist, struct sockaddr_in *addr){
    char hashKey[100];
    StashElement element;
    
    if(!nlist->netLinkTable)
        return NULL;

    createUDPLinkHashKey(addr, hashKey);

    stashFindElement(nlist->netLinkTable, hashKey, &element);
    
    if(!element)
        return NULL;
        
    return stashElementGetPointer(element);
}

/* Function findTcpNetLink()
 *    Locate a TCP based NetLink from the given list given the TCP socket that the NetLink is attached to.
 *    
 */
static NetLink* findTcpNetLink(NetLinkList* nlist, SOCKET sock){
    char hashKey[100];
    StashElement element;
    
    if(!nlist->netLinkTable)
        return NULL;
    
    createTCPLinkHashKey(sock, hashKey);

    stashFindElement(nlist->netLinkTable, hashKey, &element);
    
    if(!element)
        return NULL;
        
    return stashElementGetPointer(element);

#if 0 // poz ifdefed unreachable code    
    {
        int i;
        NetLink* link;
        for(i = 0; i < nlist->links->size; i++)
        {
            link = nlist->links->storage[i];

            if(NLT_TCP != link->type)
                continue;

            if(link->socket != sock)
                continue;

            return link;
        }
    }
    return 0;
#endif
}

/****************************************************************************************************
 * NetLinkList Monitoring                                                                            *
 ****************************************************************************************************/
/* Function netLinkListProcessMessages()
 *    Process all messages for all links controlled by the NetLinkList.
 *    This function is the NetLinkList equivilent of netMessageScan().
 *    
 */
void netLinkListProcessMessages(NetLinkList* list, NetPacketCallback *netCallBack){
    int        i;
    NetLink    *link;

    // Process each link in the NetLinkList.
    for(i=0;i<list->links->size;i++)
    {
        link = list->links->storage[i];

        //netIdleEx(link, 0, 10, current_time); // done in netLinkMaintenance

        // Process any data still on the link.
        //    This might cause a link to be removed.  Do not try to access a link
        //    after this point.
        netMessageScan(link, -1, false, netCallBack);
    }
}

static void netLinkListBatchRecieveInternal(NetLinkList* nlist, SOCKET sock)
{
    int            amt;
    Packet*        pak;
    NetLink* link;
    struct sockaddr_in addr;
    //int gotPacket = 0;
    static int msgCount = 0;

    if(sock == nlist->listen_sock){
        netGetTcpConnect(nlist);
    }else if(sock == nlist->udp_sock){
        while(1){
            // Grab a UDP packet.
            amt = pktGetUdp(&pak, nlist->udp_sock, &addr);
            
            // If there wasn't anything to read, don't do anything.
            if(0 == amt){
                /*
                if(!gotPacket){
                    printf("Select says there are packets in the UDP socket, but got nothing: %i\n", msgCount++);
                }
                */
                break;
            }

            //gotPacket = 1;
            // Did we really get some data or did we get a disconnect signal from the OS?
            if(-1 == amt)
            {
                link = findUdpNetLink(nlist, &addr);
                LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "%s:%d Received disconnect signal from OS", makeIpStr(link->addr.sin_addr.S_un.S_addr), link->addr.sin_port);
                if (link)
                    netDiscardDeadLink(link);
                continue;
            }

            link = netValidateUDPPacket(nlist, pak);
            if (!link) {
                // This packet is probably bad, or it's not a connect packet
                //  and it had no link, so it's discarded
                pak = NULL; // Already freed within netValidateUDPPacket
                continue;
            }

            link->lastRecvTime = timerCpuSeconds();
            
            // Put the packet into the receive queue of the link.
            if(!lnkAddToReceiveQueue(link, &pak)){
                pktFree(pak);
                pak = NULL;
            }
        }
    }else{
        
        // We know there is new data waiting on this socket, but we don't know who it's from and what it's for.
        // Findout which link this packet was sent on by the sender's socket
        link = findTcpNetLink(nlist, sock);
        sock = link->socket;            
        
        while(!qIsReallyFull(link->receiveQueue)){
            // Grab a TCP packet.
            amt = pktGetTcp(&pak, link);
            
            // If there wasn't anything to read, don't do anything.
            if (!amt){
                break;
            }
            
            link->lastRecvTime = timerCpuSeconds();
            
            // Put the packet into the receive queue of the link.
            if(!lnkAddToReceiveQueue(link, &pak)){
                pktFree(pak);
                pak = NULL;
            }
        }
    }
}

/* Function netLinkListBatchReceive()
 *    This function reads UDP packets and dispatches them into their
 *    corresponding NetLink in the give NetLinkList.  If a connect packet from
 *    an unknown sender is found, a new link is created.
 *
 *    See also: lnkBatchReceive() in netBatch
 */
void netLinkListBatchReceive(NetLinkList* nlist)
{
    fd_set        readSet;
    fd_set        writeSet;
    fd_set        errorSet;

    // Initialize the fd sets.
    {
        FD_ZERO(&readSet);
        FD_ZERO(&writeSet);
        FD_ZERO(&errorSet);

        FD_AddLinkList(&readSet, &writeSet, &errorSet, nlist);

        // If there is nothing to wait on, exit the function.
        if(!readSet.fd_count && !writeSet.fd_count && !errorSet.fd_count)
            return;
    }

    // Wait for incoming packet or outgoing buffer to be freed up.
    {        
        struct timeval interval;
        int selectResult;

        interval.tv_sec = 0;
        interval.tv_usec = 1;    // Wait up to one millisecond for an incoming packet.

        selectResult = select(0, &readSet, &writeSet, &errorSet, &interval);

        // Did I mess up the call to select somehow?
        assert(SOCKET_ERROR != selectResult);

        // Is there anything to be read?
        // If not, just leave the function.
        if(0 == selectResult)
            return;
    }
    
    {
        unsigned int i;

        // Call the user defined call back for each link that is ready for more outgoing data.
        for(i = 0; i < writeSet.fd_count; i++){
            SOCKET sock;
            NetLink* link;

            sock = writeSet.fd_array[i];

            // FIXME!!!  
            // UDP links cannot prefer sending over receiving.  In such a situation it should 
            if(sock == nlist->udp_sock)
                continue;

            if(!nlist->links->size)
                goto READ_PACKETS;    


            // We know there is new data waiting on this socket, but we don't know who it's from and what it's for.
            // Findout which link this packet was sent on by the sender's socket
            link = findTcpNetLink(nlist, sock);
            sock = link->socket;            

            link->transmitCallback(link);
        }

READ_PACKETS:
        // Check on all sockets that are ready for reading.
        for(i = 0; i < readSet.fd_count; i++)
            netLinkListBatchRecieveInternal(nlist, readSet.fd_array[i]);

        // Check on all sockets that reported errors
        for(i = 0; i < errorSet.fd_count; i++)
            netLinkListBatchRecieveInternal(nlist, errorSet.fd_array[i]);
    }
}

NetLink* netValidateUDPPacket(NetLinkList* linklist, Packet* pak)
{
    NetLink* link;
    int    cmd;
    U32 connect_id=0;
    
#define CLEANUP_AND_RETURN {    \
        /* Destroy the packet.*/    \
        pktFree(pak);                \
        pak = NULL;                    \
        return NULL;                \
    }
#define RETURN_ON_BAD_STREAM  if (pak->stream.errorFlags) CLEANUP_AND_RETURN;

    PERFINFO_AUTO_START("findUdpNetLink", 1);
        link = findUdpNetLink(linklist, &linklist->asyncRecvAddr);
    PERFINFO_AUTO_STOP();

    // XBox 360 check: xbox re-uses same port on connect, so a link we
    // have might think that it is receiving another packet on the
    // link when it is really a re-connect packet. this code tries to
    // see if an incoming packet is a reconnect packet before it tries
    // the decrypt.
    if( isDevelopmentMode() && link )
    {
        int paksize = pak->stream.size;
        if(pktDecryptAndCrc(pak,NULL,&linklist->asyncRecvAddr,false)
           && -1 != pktSkipWrapping(pak, NULL)
           && !pak->sib_count)
        {
            int cmd = pktGetBitsPack(pak,1);
            U32 connect_id = pktGetBitsPack(pak, 1);
            if (!pak->stream.errorFlags
                && cmd == COMM_CONNECT
                && connect_id != link->connect_id) 
            {
                netRemoveLink(link);
                link = NULL;
            }
        }
        pktReset(pak);
        pak->stream.size = paksize;
    }
    

    PERFINFO_AUTO_START("pktDecryptAndCrc", 1);
        if(!pktDecryptAndCrc(pak,link,&linklist->asyncRecvAddr,true))
        {
             PERFINFO_AUTO_STOP();
             CLEANUP_AND_RETURN;
        }
    PERFINFO_AUTO_STOP();
    
    //    Connection stuff:
    // If you get a connect on an empty link, make a new link.
    // If you get anything else on an empty link, skip that packet.
    // If you get a connect on a live link, and the connection id doesn't match, nuke the link and make a new one.

    // Skip over wrapping info.  
    //    Only critical info such as packet ID and such will be extracted.
    //    Other info that are dependent on the presence of a link will not be extracted/retained.
    if (-1==pktSkipWrapping(pak, link)) {
        badPacketLog(BADPACKET_BADDATA, linklist->asyncRecvAddr.sin_addr.S_un.S_addr, linklist->asyncRecvAddr.sin_port);
        // This is a bad packet!
        CLEANUP_AND_RETURN;
    }

    // If we got a sibling packet and there isn't already a connection, dump it.
    //    In this case, checking for the command ID will result in an unusable value
    //    since the beginning of the data section is in the middle of some larger packet.
    if(pak->sib_count && !link) {
        badPacketLog(BADPACKET_BADDATA, linklist->asyncRecvAddr.sin_addr.S_un.S_addr, linklist->asyncRecvAddr.sin_port);
        CLEANUP_AND_RETURN;
    }

    if (!pak->sib_count) { // If it doesn't have siblings, it might be a connect packet

        // Detect both the old an new style of connect packets (new ones are prefixed with bitsPack(0))
        bool isConnectCommand=false;
        cmd = pktGetBitsPack(pak,1);
        RETURN_ON_BAD_STREAM;
        if (cmd == COMM_CONNECT) { // Version 0-2 packets, and initial connect packet on all versions
            connect_id = pktGetBitsPack(pak,1);
            isConnectCommand = true;
            RETURN_ON_BAD_STREAM;
        }

        // A packet is from an unknown source and it is not asking for a connection,
        // do not waste any more time or resource processing it.
        // Dump the packet.
        if (!link && !isConnectCommand) {
            badPacketLog(BADPACKET_BADDATA, linklist->asyncRecvAddr.sin_addr.S_un.S_addr, linklist->asyncRecvAddr.sin_port);
            CLEANUP_AND_RETURN;
        }
        if (link && isConnectCommand)
        {
            if (connect_id != link->connection_id)
            {
                netRemoveLink(link);
                link = NULL;
            }
        }
    }
    pktReset(pak);

    // Add and initialize a link for future use.
    if (!link)
    {
        link = netAddLink(linklist,&linklist->asyncRecvAddr, 0, NLT_UDP);
        if (link)
        {
            link->encrypted = linklist->encrypted;
            link->socket = linklist->udp_sock;
            link->connection_id = connect_id;
            link->opType = NLOT_SYNC;
        }
    }

    return link;
#undef CLEANUP_AND_RETURN
#undef RETURN_ON_BAD_STREAM
}


/****************************************************************************************************
 * NetLinkList Maintenance                                                                            *
 ****************************************************************************************************/
/* Function netGetTcpConnect() (For server only)
 *    This function checks the TCP listen port specified in the given NetLinkList
 *    and attempts to accept any incoming connections.  The new connection, if any,
 *    is added to the given NetLinkList.
 *
 *
 */
int netGetTcpConnect(NetLinkList *nlist)
{    
    int        addrlen;
    int        socket;
    struct sockaddr_in addr;
    NetLink    *link;
    FD_SET readSet;
    FD_SET writeSet;

    if(!nlist->listen_sock)
        return 0;

    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);
    sockSetBlocking(nlist->listen_sock, 0);

    addrlen = sizeof(link->addr);
    socket = accept(nlist->listen_sock,(struct sockaddr *)&addr,&addrlen);

    if(INVALID_SOCKET == socket){
        return 0;
    }
    
    link = netAddLink(nlist, &addr, socket, NLT_TCP);    // Allocate a new link from the an existing link pool.
    if (!link)
        return 0;
    link->encrypted = nlist->encrypted;
    //link->type = NLT_TCP;                // This is a stream. i.e. not UDP.  =)
    link->socket = socket;                // This is where data will arrive and be sent on. //JE: already set in netAddLink
    link->flowControl = 0;                // TCP does flow control.
    link->retransmit = 0;                // TCP does retransmit.
    link->opType = NLOT_ASYNC;
    assert(!mpReclaimed(link));

    sockSetBlocking(link->socket, 0);
    sockSetDelay(link->socket, 0);

    // If the parent NetLinkList is in the NetMasterList, add this socket to the NetMasterList also.
    if(nlist->inNetMasterList){
        NMAddLink(link, nlist->packetCallback);
    }

    return 1;
}

void netDiscardDeadUDPLink(NetLinkList* list, struct sockaddr_in* addr){
    NetLink* link;
    link = findUdpNetLink(list, addr);

    netDiscardDeadLink(link);
}

// FIXME!!!
// DiscardDeadLink is no longer an appropriate name.
void netDiscardDeadLink(NetLink* link){
    if(link) {
        NetLinkList* nlist = link->parentNetLinkList;
        if (nlist && nlist->destroyLinkOnDiscard)
        {
		// !destroyed guard matches netRemoveLinkInternal: ENet links
		// can be discarded twice (in-band COMMCONTROL_DISCONNECT +
		// transport DISCONNECT event) and the callback must only ever
		// fire once.
		if (nlist->destroyCallback && !link->destroyed) {
			nlist->destroyCallback(link);
			link->destroyed = 1;
		}
	}

	if (link->type == NLT_UDP || link->type == NLT_ENET) {
		// Neither is individually registered with the IO completion
		// port, so nothing else will flag them; mark them reclaimable
		// directly.
		link->removedFromMasterList = 1;
	}

	link->disconnected = 1;
        link->connected = 0;
        NMNotifyDisconnectedLink(link);
        if (link->parentNetLinkList) {
            link->parentNetLinkList->hasDisconnectedLinks = 1;
        }
    }
}

void netLinkListMaintenance(NetLinkList* list){
    netLinkListRemoveDisconnected(list);
    //netLinkListHandleTcpConnect(list);
}


void netLinkListHandleTcpConnect(NetLinkList* list){
    if (list->listen_sock)
        while(netGetTcpConnect(list));
}

static void netLinkListRemoveDisconnected(NetLinkList* list){
    int i;
    if (!list->hasDisconnectedLinks)
        return;
    list->hasDisconnectedLinks = 0;
    for(i = 0; i < list->links->size; i++){
        NetLink* link;
        link = list->links->storage[i];

        if(link->disconnected){
            if(link->removedFromMasterList)
            {
                netRemoveLinkInternal(link);
                i--;
            }
            else
            {
                list->hasDisconnectedLinks = 1;
            }
        }
    }
}


void netForEachLink(NetLinkList* list, NetLinkCallback callback){
    int i;
    for(i = 0; i < list->links->size; i++){
        callback(list->links->storage[i]);
    }
}


