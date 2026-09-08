#include "utilitieslib/network/net_masterlist.h"
#include "utilitieslib/network/net_masterlistref.h"
#include "utilitieslib/network/net_link.h"
#include "utilitieslib/network/net_linklist.h"
#include "utilitieslib/network/net_packet.h"
#include "utilitieslib/network/netio_receive.h"
#include "utilitieslib/network/net_socket.h"
#include "utilitieslib/network/netio_core.h"
#include "utilitieslib/network/sock.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/utils/log.h"

#include "utilitieslib/assert/assert.h"
#include <stdio.h>

#include "utilitieslib/utils/timing.h"

#define MAX_CONCURRENT_NETWORK_THREADS 1

extern int putStoredPacket(NetLink *link,Packet *pak);
extern Packet *getStoredPacket(NetLink *link);

U32  g_NMCachedTimeSeconds = 0;
long g_NMCachedTimeTicks = 0;

//----------------------------------------
//  the structure that holds all the participants in the net_masterlist API
//
// -AB: added syncElementList :12/12/05
//
// Note: the purpose of the syncLinks array is to drive links that are
// not currently handled by the io completion port. These appear to be
// only UDP links that are not part of netlinklists that are used by
// NetTest and other programs to receive UDP packets on. The correct
// solution is probably to change the 'NM_Link' case of 'AOT_Receive'
// in NMProcessAsyncIO so that it handles this, but I'm trying to keep
// a light touch...
//----------------------------------------
typedef struct NetMasterList {
    // Holds all known NetMasterListElements.
    Array *elementList;
    // Holds all LinkList elements of the NetMasterList
    Array *linkList;
    // Holds all the synchronous links (receiving UDP links)
    Array *syncElementList;
    // Flags for what maintenence needs to be ran this tick
    int hasDisconnectedElements :1;
} NetMasterList;

NetMasterList netMasterList = {0};

U32 lastMaintenanceTimeTicks=0;

HANDLE listenThreadHandle = 0;
DWORD listenThreadID = 0;

Array* listenSocks;
Array* listenEvents;
Array* listenLinkLists;

HANDLE completionPort = 0;

#ifdef LOG_NETIO_ACTIVITIES
#define if_log_netio(x) x
#else
#define if_log_netio(x)
#endif

static void netLinkSendIdles(NetLink* link);
static void NMMessageScanForAllLinks(void);
static void NMMessageScanElement(NetMasterListElement *element, bool ignoreIfHasParent);
/****************************************************************************************************
 * Junk                                                                                                *
 ****************************************************************************************************/

/****************************************************************************************************
 * Core                                                                                                *
 ****************************************************************************************************/
DWORD WINAPI listenThreadProc(LPVOID lpParameter){
    int waitResult;
    NetLinkList* list;

    while(1){
        // If there are no listen events to wait on,
        // keep sleeping until something wakes the thread up.
        if(!listenEvents || !listenEvents->size){
            SleepEx(INFINITE, 1);
            continue;
        }

        // Wait forever on the listen events.
        waitResult = WSAWaitForMultipleEvents(listenEvents->size, listenEvents->storage, 0, INFINITE, 1);

        // If we stopped waiting on the events because an user APC was executed...
        // a new listen socket + event has been added to their corresponding arrays.
        // Continue waiting on them.
        if(WAIT_IO_COMPLETION == waitResult)
            continue;

        // Findout which list has incoming connections pending.
        list = listenLinkLists->storage[waitResult - WAIT_OBJECT_0];

        // Connect all connection requests.
        while(netGetTcpConnect(list));
    }
}

void NMInit()
{
    if (!netMasterList.elementList) {
        netMasterList.elementList = createArray();
    }
    if(!netMasterList.linkList){
        netMasterList.linkList = createArray();
    }
    if(!netMasterList.syncElementList){
        netMasterList.syncElementList = createArray();
    }

    completionPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 1);
}

void NMNotifyDisconnectedLink(NetLink *link)
{
    // If we wanted this to be fast, we could make a queue of disconnected links
    //  both here and on every NetLinkList, but we would also need some quick method
    //  to remove it from the NetMasterList, which isn't trivial because the index into
    //  the array can change.
    // Instead, just flag for the disconnect logic to be ran
    netMasterList.hasDisconnectedElements = 1;
}

void NMLinkQueueProcessStoredPackets(NetLink *link)
{
    // queue it up if not already in progress and there are packets to
    // process
    if( link && completionPort
        && !link->storedRecvContext.inProgress 
        && link->receivedPacketStorage.size )
    {
        AsyncOpContext *storedRecv = &link->storedRecvContext;
        int result;

        ZeroStruct( storedRecv );
        storedRecv->type = AOT_StoredRecv;
        storedRecv->time = timerCpuTicks();
        storedRecv->inProgress = true;

        result = PostQueuedCompletionStatus(completionPort, -1, 0, &storedRecv->ol);
        assert(result || GetLastError() == ERROR_IO_PENDING);
    }
}

void NMExtractUDPPacket(NetMasterListElement* element, AsyncOpContext* context)
{
    NetLinkList* linklist;
    int amt;
    Packet* pak;
    NetLink* link;

    PERFINFO_AUTO_START("NMExtractUDPPacket", 1);
    
    linklist = element->storage;

    // Only udp link lists are handled here.
    // TCP links/linklists are "flattened" into single links for the completion port.
    assert(linklist->udp_sock);

    // ab: the 'asyncUdpState' values seems misnamed to me, but
    // basically the 'asyncUdpState' field is 'ASU_ExtractBody' if
    // there is none, or an async operation pending, and it is set to
    // 'ASU_ExtractBodyComplete' here, when processing an io
    // completion port message.
    if(context){
        if(ASU_ExtractBody == linklist->asyncUdpState){
            linklist->asyncUdpBytesRead = context->bytesTransferred;
            linklist->asyncUdpState++;
        }
        if(ASU_None == linklist->asyncUdpState)
            linklist->asyncUdpState++;
    }

    while(1){
        // Grab a UDP packet.
        pak=NULL;

        PERFINFO_AUTO_START("pktGetUdpAsync", 1);
            amt = pktGetUdpAsync(&pak, linklist, &linklist->asyncRecvAddr);
        PERFINFO_AUTO_STOP();

        // If there wasn't anything to read, don't do anything.
        if(0 == amt){
            break;
        }

        if(-1 == amt)
            continue;

        // We got a new packet, but we don't know who it's from and what it's for.
        // Findout which link this packet was sent on by the sender's address.
        PERFINFO_AUTO_START("netValidateUDPPacket", 1);
            link = netValidateUDPPacket(linklist, pak);
        PERFINFO_AUTO_STOP();
            
        if(!link) {
            continue;
        }

        link->lastRecvTime = timerCpuSeconds();

        // Put the packet into the receive queue of the link.

        PERFINFO_AUTO_START("lnkAddToReceiveQueue", 1);
            if(!lnkAddToReceiveQueue(link, &pak)){
                pktFree(pak);
                pak = NULL;
            }
        PERFINFO_AUTO_STOP();
    }
    
    PERFINFO_AUTO_STOP();
}

void NMExtractTCPPacket(NetMasterListElement* element, AsyncOpContext* context){
    NetLink* link;

    PERFINFO_AUTO_START("NMExtractTCPPacket", 1);

    link = element->storage;
    if (link->type == NLT_TCP) { // This link may have been cleared/disconnected, if so, don't read into it
        lnkReadTCPAsync(link, context->bytesTransferred);
    }

    PERFINFO_AUTO_STOP();
}


#define baseof(addr, structType, fieldName) (structType*)((ptrdiff_t)addr - (ptrdiff_t)&(((structType*)0x0)->fieldName));

char *link_TypeStr(NetLink *link)
{
    if( link )
    {
        return link->type == NLT_TCP ? "TCP" : (link->type == NLT_UDP ? "UDP" : "UNKNOWN");
    }
    return "NULL";
}


//_CrtMemState initialMemState;
static void NMProcessAsyncIO(int milliseconds)
{
    static NetMasterListElement** elementsRecvd = NULL;
    static bool recursive_call = false;
    int result;

    eaSetSize(&elementsRecvd,0);
    
    if(NO_TIMEOUT == milliseconds)
        milliseconds = INFINITE;
    else if(POLL == milliseconds)
        milliseconds = 0;

    // If there is nothing to wait on, then don't do anything.
    if(!(netMasterList.elementList && completionPort)){
        if(milliseconds)
            Sleep(milliseconds);
            
        return;
    }

    assert(!recursive_call); // not re-entrant
    recursive_call = true;

    while(1){
        DWORD bytesTransferred = 0;
        NetLink* link;
        ULONG_PTR elementref=0;
        NetMasterListElement* element = NULL;
        OVERLAPPED* ol = NULL;
        AsyncOpContext* context;

        PERFINFO_AUTO_START("GetQueuedCompletionStatus", 1);
    
            result = GetQueuedCompletionStatus(completionPort, &bytesTransferred, &elementref, &ol, milliseconds);
        
        PERFINFO_AUTO_STOP();
        
        if (elementref)
            element = netMasterListElementFromRef(elementref);
            
        // If this function is called with a time to wait, we only want it to wait on the first call, and return after
        //   handling all of the data, but not wait any longer when it's done.
        milliseconds = 0;
        if(!result || !bytesTransferred) // || !element)
        {
            // If we timed out waiting for socket events to happen, it's time to get out of here.
            
            PERFINFO_AUTO_START("failure", 1);

                if(WAIT_TIMEOUT == GetLastError()){
                    PERFINFO_AUTO_STOP();
                    break;
                }
                
                if(element && ol) {
                    context = baseof(ol, AsyncOpContext, ol);
                    context->bytesTransferred = bytesTransferred;
                    //assert(context->inProgress == 1);
                    context->inProgress = 0;
                    if(AOT_Send == context->type)
                        destroyAsyncOpContext(context);
                }
                // Once a peer socket is disconnected, it is marked as dead.  The dead link
                // will be destroyed the next time NMMonitor() is called.  As part of the
                // link destruction process, all pending async IO operations will be cancelled.
                // When this happens, there will be a stream of messages coming through the
                // IO completion port that says the operation has been aborted.  These messages
                // can be safely ignored.
                if(ERROR_OPERATION_ABORTED == GetLastError()){
                    PERFINFO_AUTO_STOP();
                    continue;
                }

                if(ERROR_MORE_DATA == GetLastError()){
                    // We received a packet bigger than our overlapped buffer could hold.
                    // As we make sure we never send more than this size, this packet must be
                    //   spoofed or in error, we should close the link
                    badPacketLog(BADPACKET_TOOBIG, 0, 0);
                    // Pretend this is a disconnect
                    bytesTransferred = 0;
                }

                // Is the socket disconnected?
                if(element && bytesTransferred == 0){
                    static int count = 0;

                    // If so, mark the socket as disconnected.
                    switch(element->type){
                        xcase NM_Link:
                        {
                            PERFINFO_AUTO_START("NM_Link", 1);
                                link = element->storage;
                                //assert(link->killed == 0);
                                if (link)
                                    log_printf("net_disconnect.log", "%s:%d Received %s link disconnect signal from OS", makeIpStr(link->addr.sin_addr.S_un.S_addr), link->addr.sin_port, link_TypeStr(link));
                                netDiscardDeadLink(link);
                            PERFINFO_AUTO_STOP();
                        }
                        xcase NM_LinkList:
                        {
                            NetLinkList* list = element->storage;
                            PERFINFO_AUTO_START("NM_LinkList", 1);
                                LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "%s:%d Received linklist disconnect signal from OS", makeIpStr(list->asyncRecvAddr.sin_addr.S_un.S_addr), list->asyncRecvAddr.sin_port);
                                netDiscardDeadUDPLink(list, &list->asyncRecvAddr);

                                // Get async udp socket extraction going again by trying to extract another udp packet.
                                NMExtractUDPPacket(element, NULL);
                            PERFINFO_AUTO_STOP();
                        }
                        xcase NM_Sock:
                        {
                            PERFINFO_AUTO_START("NM_Sock", 1);
                                closesocket((SOCKET)element->storage);
                                // Should this be the following?
                                //closesocket(((NMSock*)element->storage)->sock);
                                NMRemoveElement(NMFindElementIndex(element));
                            PERFINFO_AUTO_STOP();
                        }
                    }
                }else{
                    PERFINFO_AUTO_STOP();
                    break;
                }
            PERFINFO_AUTO_STOP();
        } else {
            // --------------------
            // some data was received successfully
            
            // this has an overlapped op associated with it.
            // that means it came from a direct WSARecv/WSARecvFrom
            // or from an async op posted to the io completion port (

            PERFINFO_AUTO_START("success", 1);

            if(ol)
            {
                NetMasterListElement *elementRecv = NULL;
                
                PERFINFO_AUTO_START("overlapped", 1);
                
                    context = baseof(ol, AsyncOpContext, ol);
                    context->bytesTransferred = bytesTransferred;
                    //assert(context->inProgress == 1);
                    context->inProgress = 0;
                    
                    switch(context->type)
                    {
                        xcase AOT_Receive:
                        {
                            if(element)
                            {
                                switch(element->type){
                                    xcase NM_LinkList:
                                    {
                                        PERFINFO_AUTO_START("AOT_Receive:NM_LinkList", 1);
                                            NMExtractUDPPacket(element, context);
                                        PERFINFO_AUTO_STOP();
                                    }
                                    xcase NM_Link:
                                    {
                                        PERFINFO_AUTO_START("AOT_Receive:NM_Link", 1);
                                            NMExtractTCPPacket(element, context);
                                        PERFINFO_AUTO_STOP();
                                    }
                                    xcase NM_Sock:
                                    {
                                        NMSock* sock;

                                        PERFINFO_AUTO_START("AOT_Receive:NM_Sock", 1);
                                            sock = element->storage;
                                            sock->processSocket(sock);
                                        PERFINFO_AUTO_STOP();
                                    }
                                }
                                elementRecv = element;
                            }
                        }
                        
                        xcase AOT_Send:
                        {
                            if(element)
                            {
                                LOG(LOG_NET, LOG_LEVEL_DEBUG, 0,  "Done aysnc tcp send\n");
                                        
                                switch(element->type)
                                {
                                    xcase NM_LinkList:
                                    {
                                        // A UDP NetLinkList has finished sending its data.
                                        // There isn't really any good way to continue flushing
                                        // queued data because there can be a very large number of
                                        // links attached to the UPD NetLinkList.
                                        
                                        assert(0);
                                    }
                                        
                                    xcase NM_Link:
                                    {
                                        NetLink* link = element->storage;
                                        PERFINFO_AUTO_START("AOT_Send:NM_Link", 1);
                                            //printf("Send history says: %i bytes per second\n", pktRate(&link->sendHistory));
                                            link->awaitingAsyncSendCompletion = 0;
                                            if(link->type == NLT_TCP){
                                                int pakIndex;
                                                Packet* pak;
                                                pakIndex = arrayFindElement(&link->reliablePacketsArray, context->pak);
                                                assert(pakIndex >= 0);
                                                if(pakIndex >= 0){
                                                    pak = link->reliablePacketsArray.storage[pakIndex];
                                                    pktFree(pak);
                                                    arrayRemoveAndFill(&link->reliablePacketsArray, pakIndex);
                                                }
                                            }
                                            // Give the user a chance to fill the send buffer.
                                            if(link->transmitCallback){
                                                link->transmitCallback(link);
                                            }
                                            
                                            // Flush the link.
                                            lnkFlush(link);
                                        PERFINFO_AUTO_STOP();
                                    }
                                        
                                    xcase NM_Sock:
                                    {
                                        NMSock* sock;
                                        PERFINFO_AUTO_START("AOT_Send:NM_Sock", 1);
                                            sock = element->storage;
                                            sock->processSocket(sock);
                                        PERFINFO_AUTO_STOP();
                                    }
                                        
                                    xdefault:
                                    {
                                        assert(0);
                                    }
                                }
                                    
                                destroyAsyncOpContext(context);
                            }
                            
                            PERFINFO_AUTO_STOP();
                        }
                            
                        xcase AOT_StoredRecv:
                        {
                            // --------------------
                            // the overlapped object refers to a link that
                            // had packets it couldn't process right away.
                            // Try processing any outstanding packets on
                            // this link.

                            NetLink *link = baseof(ol, NetLink, storedRecvContext);
                            elementRecv = link->parentElement;                        
                        }

                        xdefault:
                        {
                            // This test is useless if a "stand-alone" link is used with the NetMasterList.
                            // When the link is disconnected and cleared, it is going to fail this test.
                            
                            //if(NM_Link == element->type){
                            //    NetLink* link = element->storage;
                            //    if(!mpReclaimed(link))
                            //        assert(0);
                            //}
                        }
                    }

                    // --------------------
                    // if we received any data on this element, handle it

                    if( elementRecv && -1 == eaFind(&elementsRecvd,elementRecv) )
                        eaPush(&elementsRecvd, elementRecv);

                PERFINFO_AUTO_STOP();
            }
            // no overlapped op associated with it. this is from the
            // initial PostQueuedCompletionStatus when the element is created
            else if (element)
            {
                PERFINFO_AUTO_START("not overlapped", 1);
                    switch(element->type)
                    {
                        xcase NM_LinkList:
                        {
                            PERFINFO_AUTO_START("NM_LinkList", 1);
                                NMExtractUDPPacket(element, &((NetLinkList*)(element->storage))->receiveContext);
                            PERFINFO_AUTO_STOP();
                        }
                        xcase NM_Link:
                        {
                            PERFINFO_AUTO_START("NM_LinkList", 1);
                                NMExtractTCPPacket(element, &((NetLink*)(element->storage))->receiveContext);
                            PERFINFO_AUTO_STOP();
                        }
                        xcase NM_Sock:
                        {
                            NMSock* sock;
                            PERFINFO_AUTO_START("NM_Sock", 1);
                                sock = element->storage;
                                sock->processSocket(sock);
                            PERFINFO_AUTO_STOP();
                        }
                    }

                    // save this for handling later.
                    eaPush(&elementsRecvd, element);
                PERFINFO_AUTO_STOP();
            }

            PERFINFO_AUTO_STOP();
        }
    }

    PERFINFO_AUTO_START("process_recvd_packets", 1);
    {
        int i;
        int n = eaSize(&elementsRecvd);
        for( i = 0; i < n; ++i ) 
        {
            NetMasterListElement *elt = elementsRecvd[i];
            
            if(NMMonitorShouldStopReceiving())
                break;
            
            if( elt )
                NMMessageScanElement(elt, false);
        }
    }
    PERFINFO_AUTO_STOP(); // process_recvd_packets
    recursive_call = false;
}


void NMRemoveDisconnectedElements()
{
    int nmCursor;
    if (!netMasterList.hasDisconnectedElements) {
        return;
    }
    netMasterList.hasDisconnectedElements = 0;
    for(nmCursor = 0; nmCursor < netMasterList.elementList->size; nmCursor++){
        NetMasterListElement* element;
        element = netMasterList.elementList->storage[nmCursor];

        if(element->type == NM_Link){
            NetLink* link = element->storage;

            if(link->disconnected){
                int iElt = arrayFindElement(netMasterList.syncElementList, element);
                if(INRANGE0(iElt,netMasterList.syncElementList->size))
                {
                    arrayRemoveAndFill(netMasterList.syncElementList,iElt);
                }
                NMRemoveElement(nmCursor);
                nmCursor--;
            }
        }
    }
}


static bool g_nmmonitor_stop_receiving=false;
static bool g_nmmonitor_in_nmmonitor=false;
void NMMonitorStopReceiving(void)
{
    g_nmmonitor_stop_receiving = true;
}

bool NMMonitorShouldStopReceiving(void)
{
    return g_nmmonitor_stop_receiving && g_nmmonitor_in_nmmonitor;
}

//----------------------------------------
//  Second helper to verifying that all the packets have been handled
//----------------------------------------
static bool s_verifyMsgScan(NetLink *link)
{

    Packet *pak = getStoredPacket(link);
    if (!pak)
        pktGet(&pak,link);

    if(pak)
    {
        Errorf("%s:%d packet still on queue, should have been processed", makeIpStr(link->addr.sin_addr.S_un.S_addr), link->addr.sin_port);
        putStoredPacket(link, pak);
        return true;
    }
    return false;
}

//----------------------------------------
//  Put this in to verify backwards compatability with the IO
//  completion port change
//----------------------------------------
static bool s_VerifyAllPacketsProcessedScan(void)
{
    int i;
    for(i = 0; i < netMasterList.elementList->size; i++){
        NetMasterListElement* element;
        element = netMasterList.elementList->storage[i];
        if(element->type == NM_LinkList){
            // UDP Links will not be in the NetMasterList, just hanging off of their LinkList
//             netLinkListProcessMessages(element->storage, element->packetCallback);
            NetLinkList* list = element->storage;
            int        i;
            NetLink    *link;

            // Process each link in the NetLinkList.
            for(i=0;i<list->links->size;i++)
            {
                link = list->links->storage[i];
                // netMessageScan(link, -1, false, netCallBack);
                if(s_verifyMsgScan(link))
                    return true;
            }
        }else if(element->type == NM_Link){
            NetLink *link = element->storage;
            // TCP Links may both be hanging off their LinkList and in here individually, don't call it on them twice!
            if (!link->parentNetLinkList || !link->parentNetLinkList->inNetMasterList) {
                if (link->opType == NLOT_SYNC) {
                    // Synchronous outgoing UDP links added to the NetMasterList (only used by NetTest?)
                    // @todo -AB: track down this packetCallback usage :12/07/05

                    // netLinkMonitor(link, -1, element->packetCallback);
                    // eventually calls netMessageScan
                    if(s_verifyMsgScan(link))
                        return true;
                } else {
                    // Async links got their data in NMBatchRecieve
//                     netMessageScan(link, -1, false, element->packetCallback);
                    if(s_verifyMsgScan(link))
                        return true;
                }
            }
        }
        if (NMMonitorShouldStopReceiving())
            break;
    }
    return false;
}

/* Function NMMonitorAllSyncElements
 *    handle sending and receiving for static links.
 *    
 */
static INLINEDBG void NMMonitorAllSyncElements(void){
    int i;
    if(!netMasterList.syncElementList)
        return;
    for(i = 0; i < netMasterList.syncElementList->size; i++){
        NetMasterListElement* element;
        NetLink *link;
        element = netMasterList.syncElementList->storage[i];
        assert(element->type == NM_Link);
        
        link = element->storage;
        assert(link && link->opType == NLOT_SYNC);
        
        netLinkMonitor(link, -1, element->packetCallback);
    }
}

/* Function NMMonitor()
 *    Monitor all elements that are under the NetMasterList's control.
 *    "Monitoring" includes flushing and receiving data on all links.
 *
 *    Parameters:
 *        NO_TIMEOUT - Wait forever for incoming data.
 *        POLL - Check for incoming data.  Do not wait.
 *        int - wait up to this many milliseconds for incoming data.
 *        
 */
void NMMonitor(int milliseconds)
{
    netioEnterCritical();

    // Do this only once, not on every link
    g_NMCachedTimeSeconds = timerCpuSeconds();
    g_NMCachedTimeTicks = timerCpuTicks();

    if(!netMasterList.elementList)
    {
        Sleep(milliseconds);
        netioLeaveCritical();
        return;
    }

    g_nmmonitor_in_nmmonitor = true;
    PERFINFO_AUTO_START("netGetTcpConnect", 1);

        // Get any TCP connections O(#NetLinkLists) = fast
        NMForAllLinkLists(netLinkListHandleTcpConnect);

    PERFINFO_AUTO_STOP_START("netLinkSendIdles", 1);

        if (timerSeconds(g_NMCachedTimeTicks - lastMaintenanceTimeTicks) > 3.0) {
            // send keep-alives
            // O(#NetLinks) = slow!  Do this only once and a while
            NMForAllLinks(netLinkSendIdles);
            lastMaintenanceTimeTicks = g_NMCachedTimeTicks;
        }

    PERFINFO_AUTO_STOP_START("lnkFlushAll", 1);

        // Flush all links
        // O(#Links with data) = fast
        lnkFlushAll();

    PERFINFO_AUTO_STOP_START("NMProcessAsyncIO", 1);

        // Perform aync network IO
        // O(#Async operations completed) = fast
        NMProcessAsyncIO(milliseconds);

    PERFINFO_AUTO_STOP_START("NMRemoveElement", 1);

        // Destroy the element that has been disconnected.
        //    Remove the element from the NetMasterList.
        //  They will also be removed from their NetLinkLists in netLinkListMaintenance
        // general case: instant
        // worst case: O(#NetMasterListElements (i.e. TCP Links + NetLinkLists))
        //   happens whenever any NetLink gets disconnected

        NMRemoveDisconnectedElements();

    PERFINFO_AUTO_STOP_START("netLinkListMaintenance", 1);

        // Some links may have been disconnected during the packet processing step.
        // Get rid of those dead links now and free them
        // Must stay south of "link->disconnected" in NMRemoveDisconnectedElements above
        // This will only iterate over NetLinks that are members of LinkLists that have
        //  been flagged as having something disconnected this tick
        // general case: O(#NetLinkLists) = fast
        // worst case: O(#NetLinks) = slow, happens any time there was a disconnect on
        //   a NetLink that is a member of a NetLinkList
       
        NMForAllLinkLists(netLinkListMaintenance);
    
    PERFINFO_AUTO_STOP_START("netMessageScan", 1);

        // This is the slowest part of this function, it needs to go over every link and check a couple
        //   different queues to see if any packets have been received
        
        // This step may cause user callbacks to discard/remove a link.
        // All links that are disconnected have already been cleaned up.
        // 
        // Process any packets that has been queued for known links.
        //    This can probably be done in a more efficient manner if a list of
        //    links that are ready for processing is available.
        // ab: do this one more time for backwards compatability reasons.
        NMProcessAsyncIO(milliseconds);

        // scan the synchronous links that aren't added to the io completion port
        // currently these are UDP links with no linklist parent.
        NMMonitorAllSyncElements();

        // @testing-remove -AB: make sure all packets getting scanned :12/07/05
        if(!NMMonitorShouldStopReceiving() 
           && !isProductionMode() 
           && s_VerifyAllPacketsProcessedScan())
            NMMessageScanForAllLinks();

    PERFINFO_AUTO_STOP_START("lnkFlushAll", 1);
        // The packet processing step may have caused data to be added to the send queue.
        // Flush the links before we exit to keep transaction time short.
        lnkFlushAll();
    PERFINFO_AUTO_STOP();

    g_nmmonitor_stop_receiving = false;
    g_nmmonitor_in_nmmonitor = false;

    netioLeaveCritical();
}


/****************************************************************************************************
 * NM element manangement                                                                            *
 ****************************************************************************************************/
/*
static void CALLBACK addListenSock(NetLinkList* listenLinkList){
    WSAEVENT listenEvent;
    SOCKET listenSocket;
    int result;

    listenSocket = listenLinkList->listen_sock;

    if(!listenSocks)
        listenSocks = createArray();
    if(!listenEvents)
        listenEvents = createArray();
    if(!listenLinkLists)
        listenLinkLists = createArray();

    // Create an event that is signaled when the given socket is ready to
    // accept a connection.
    listenEvent = CreateEvent(0, 0, 0, 0);
    assert(WSA_INVALID_EVENT != listenEvent);

    result = WSAEventSelect(listenSocket, listenEvent, FD_ACCEPT);
    assert(SOCKET_ERROR != result);

    // Store the socket and the corresponding event.
    arrayPushBack(listenSocks, (void*)listenSocket);
    arrayPushBack(listenEvents, (void*)listenEvent);
    arrayPushBack(listenLinkLists, (void*)listenLinkList);
}
*/
static void NMAddListenSock(NetLinkList* list){
    /*
    // Craete the listen thread if it is not already running.
    if(!listenThreadHandle){
        listenThreadHandle = CreateThread(
            NULL,                            // SD
            0,                                // initial stack size
            listenThreadProc,                // thread function
            NULL,                            // thread argument
            0,                                // creation option
            &listenThreadID                    // thread identifier
            );
    }

    // Have the listen thread add another socket to listen on.
    QueueUserAPC((PAPCFUNC)addListenSock, listenThreadHandle, (ULONG_PTR)list);
    */
}

// add a link to the master list so that it will get processed during NMMonitor
void NMAddLink(NetLink* link, NetPacketCallback callback){
    NetMasterListElement* element;

    netioEnterCritical();
    //_CrtMemCheckpoint(&initialMemState);
    element = createNetMasterListElement();
    element->type = NM_Link;
    element->packetCallback = callback;
    element->storage = link;

    link->parentElement = element;

    if(!netMasterList.elementList){
        NMInit();
    }
    arrayPushBack(netMasterList.elementList, element);

    if (link->type == NLT_TCP) {
        assert(link->socket);
        link->opType = NLOT_ASYNC;
        // THIS FAILS ON XBOX. Tcp connections need to be done some other way
        completionPort = CreateIoCompletionPort((HANDLE)link->socket, completionPort, (ULONG_PTR)refFromNetMasterListElement(element), MAX_CONCURRENT_NETWORK_THREADS);
        {
            int result;
            result = PostQueuedCompletionStatus(completionPort, -1, (ULONG_PTR)refFromNetMasterListElement(element), NULL);

            if ( (!result && GetLastError( ) != ERROR_IO_PENDING))
            {            
                assert(0);
            }
        }
        assert(completionPort);
    }

    if( link->opType == NLOT_SYNC ) {
        arrayPushBack(netMasterList.syncElementList, element);
    }

    netioLeaveCritical();
}

void NMAddLinkList(NetLinkList* linklist, NetPacketCallback callback){
    NetMasterListElement* element;

    netioEnterCritical();
    element = createNetMasterListElement();
    element->type = NM_LinkList;
    element->packetCallback = callback;
    element->storage = linklist;

    linklist->packetCallback = callback;
    linklist->inNetMasterList = 1;

    if(!netMasterList.elementList){
        NMInit();
    }
    arrayPushBack(netMasterList.elementList, element);
    arrayPushBack(netMasterList.linkList, element);

    // If the list needs to listen on a tcp port...
    if(linklist->listen_sock){
        // Tell the listen thread to add a new socket to listen to.
        NMAddListenSock(linklist);
    }
    if(linklist->udp_sock){
        // Otherwise, if the list just needs to receive UDP packets,
        // associate the udp socket with the IO completion port.
        NetMasterListElement* element;
        element = createNetMasterListElement();
        element->type = NM_LinkList;
        element->packetCallback = linklist->packetCallback;
        element->storage = linklist;
        completionPort = CreateIoCompletionPort((HANDLE)linklist->udp_sock, completionPort, (ULONG_PTR)refFromNetMasterListElement(element), MAX_CONCURRENT_NETWORK_THREADS);

        {
            int result;
            result = PostQueuedCompletionStatus(completionPort, -1, (ULONG_PTR)refFromNetMasterListElement(element), NULL);

            if ( (!result && GetLastError( ) != ERROR_IO_PENDING))
            {            
                assert(0);
            }
        }
        assert(completionPort);
    }

    // If the linklist already has a some links in it,
    // add them to the completion port now.
    {
        int i;
        for(i = 0; i < linklist->links->size; i++){
            NetLink* link = linklist->links->storage[i];
            link->opType = NLOT_ASYNC;
            assert(!mpReclaimed(link));
            NMAddLink(link, linklist->packetCallback);
        }
    }
    netioLeaveCritical();
}

NMSock* NMAddSock(SOCKET sock, NMSockCallback callback, void* userData){
    NetMasterListElement* element;
    NMSock* nsock;

    netioEnterCritical();
    element = createNetMasterListElement();
    element->type = NM_Sock;
    

    nsock = calloc(1, sizeof(NMSock));
    nsock->sock = sock;
    nsock->processSocket = callback;
    nsock->userData = userData;

    element->storage = nsock;
    
    if(!netMasterList.elementList){
        NMInit();
    }
    arrayPushBack(netMasterList.elementList, element);

    completionPort = CreateIoCompletionPort((HANDLE)sock, completionPort, (ULONG_PTR)refFromNetMasterListElement(element), MAX_CONCURRENT_NETWORK_THREADS);
    netioLeaveCritical();
    return nsock;
}

NMSock* NMFindSock(SOCKET sock){
    int i;

    netioEnterCritical();
    // Examine all entries in the master list to see if it matches any of the UDP ports
    // of the links or linklists.
    for(i = 0; i < netMasterList.elementList->size; i++){
        NetMasterListElement* element;
        element = netMasterList.elementList->storage[i];

        if(element->type == NM_Sock){
            NMSock* nsock;

            nsock = element->storage;

            if(nsock->sock == sock)
            {
                netioLeaveCritical();
                return nsock;
            }
        }
    }
    netioLeaveCritical();
    return NULL;
}

NetMasterListElement* NMElementFromNMSock(NMSock *nsock)
{
    int i;

    netioEnterCritical();
    for(i = 0; i < netMasterList.elementList->size; i++){
        NetMasterListElement* element;
        element = netMasterList.elementList->storage[i];

        if(element->type == NM_Sock){
            NMSock* nsock_test;
            nsock_test = element->storage;

            if(nsock_test == nsock)
            {
                netioLeaveCritical();
                return element;
            }
        }
    }
    netioLeaveCritical();
    return NULL;
}

void NMCloseSock(NMSock *nsock)
{
    // This might leak/not clean up the AsyncOpContexts (done manually by the one current caller)
    NetMasterListElement *element;
    if (nsock->sock != -1) {
        CancelIo((HANDLE)nsock->sock);
        closesocket(nsock->sock);
        nsock->sock = (SOCKET)-1;
    }
    element = NMElementFromNMSock(nsock);
    if (element) {
        NMRemoveElement(NMFindElementIndex(element));
    }
}


int NMSockIsUDP(SOCKET sock){
    int i;

    // Examine all entries in the master list to see if it matches any of the UDP ports
    // of the links or linklists.
    for(i = 0; i < netMasterList.elementList->size; i++){
        NetMasterListElement* element;
        element = netMasterList.elementList->storage[i];

        if(element->type == NM_Link){
            NetLink* link;
            link = element->storage;

            if(link->type == NLT_UDP && link->socket == sock)
                return 1;
        }else{
            NetLinkList* linklist;
            linklist = element->storage;

            if(linklist->udp_sock == sock)
                return 1;
        }
    }

    return 0;
}


int NMFindElementIndex(NetMasterListElement* element)
{
    return arrayFindElement(netMasterList.elementList, element);
}


void NMRemoveElement(int elementIndex)
{
    NetMasterListElement* element;
    if(elementIndex < 0 || elementIndex >= netMasterList.elementList->size)
        return;

    element = netMasterList.elementList->storage[elementIndex];
    assert(element);

    if(element->type == NM_Link)
    {
        NetLink* link = element->storage;
        assert(link->disconnected);
        link->removedFromMasterList = 1;
    }
    else if(element->type == NM_Sock)
    {
        // IMPLEMENTME!!!
        // Clean up bare sockets.
    }
    else
    {
        assert(0); // Other types not implemented yet.
    }

    arrayRemoveAndFill(netMasterList.elementList, elementIndex);
    destroyNetMasterListElement(element);
}

/****************************************************************************************************
 * NM maintenance                                                                                    *
 ****************************************************************************************************/
static void netLinkSendIdles(NetLink* link){
    netIdleEx(link, 0, 10, g_NMCachedTimeTicks);
}

/* Function NMForAllLinks
 *    Send all NetLinks found in the net master list to the given callback.
 *    
 */
void NMForAllLinks(NetLinkCallback callback){
    int i;
    if(!netMasterList.elementList)
        return;
    for(i = 0; i < netMasterList.elementList->size; i++){
        NetMasterListElement* element;
        element = netMasterList.elementList->storage[i];
        if(element->type == NM_LinkList){
            // UDP Links will not be in the NetMasterList, just hanging off of their LinkList
            netForEachLink(element->storage, callback);
        }else if(element->type == NM_Link){
            NetLink *link = element->storage;
            // TCP Links may both be hanging off their LinkList and in here individually, don't call it on them twice!
            if (!link->parentNetLinkList || !link->parentNetLinkList->inNetMasterList) {
                callback(element->storage);
            }
        }
    }
}

static void NMMessageScanElement(NetMasterListElement *element, bool ignoreIfHasParent){
    PERFINFO_AUTO_START("NMMessageScanElement", 1);

    if(element->type == NM_LinkList){
        // UDP Links will not be in the NetMasterList, just hanging off of their LinkList
        netLinkListProcessMessages(element->storage, element->packetCallback);
    }else if(element->type == NM_Link){
        NetLink *link = element->storage;
        // TCP Links may both be hanging off their LinkList and in here individually, don't call it on them twice!
        if (!ignoreIfHasParent || !link->parentNetLinkList || !link->parentNetLinkList->inNetMasterList) {
            if (link->opType == NLOT_SYNC) {
                // Synchronous outgoing UDP links added to the NetMasterList (only used by NetTest?)
                netLinkMonitor(link, -1, element->packetCallback);
            } else {
                // Async links got their data in NMBatchRecieve
                netMessageScan(link, -1, false, element->packetCallback);
            }
        }
    }
    
    PERFINFO_AUTO_STOP();
}

    

/* Function NMMessageScanForAllLinks
*    Send all NetLinks found in the net master list to netMessageScan.
*
*     @deprecated -AB: don't use :12/08/05
*/
static void NMMessageScanForAllLinks(void){
    int i;
    for(i = 0; i < netMasterList.elementList->size; i++){
        NetMasterListElement* element;
        element = netMasterList.elementList->storage[i];
        NMMessageScanElement(element, true);
        if (NMMonitorShouldStopReceiving())
            break;
    }
}

/* Function NMForAllLinkLists
 *    Send all NetLinkLists found in the net master list to the given callback.
 *    
 */
void NMForAllLinkLists(NMLinkListCallback callback){
    int i;
    if(!netMasterList.linkList)
        return;
    for(i = 0; i < netMasterList.linkList->size; i++){
        NetMasterListElement* element;
        element = netMasterList.linkList->storage[i];
        assert(element->type == NM_LinkList);
        callback(element->storage);
    }
}


/****************************************************************************************************
 * NM debug utilities                                                                                *
 ****************************************************************************************************/

/* Function printAsyncState
 *    Prints out the async receive state that the link is currently in to the console.
 *    
 */
void printAsyncState(NetLink* link){
    char* statename;
    printf("In ");

    statename = getAsyncStateName(link);
    printf(statename);
    
    printf(" state\n");
}

/* Function getAsyncStateName
 *    Returns the async receive state that the link is currently in to the console.
 *    
 */
char* getAsyncStateName(NetLink* link){
    switch(link->asyncTcpState){
        case AST_None:
            return "none";
            break;
        case AST_WaitingToStart:
            return "waiting";
            break;
        case AST_ExtractHeader:
            return "extract header";
            break;
        case AST_ExtractHeaderComplete:
            return "complete extract header";
            break;
        case AST_ExtractBody:
            return "extract body";
            break;
        case AST_ExtractBodyComplete:
            return "complete extract body";
            break;
    }
    return "bad string";
}

