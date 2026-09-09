#include "utilitieslib/network/netio_send.h"
#include "utilitieslib/network/net_structdefs.h"
#include "utilitieslib/network/net_packet.h"
#include "utilitieslib/network/netio_stats.h"
#include "utilitieslib/network/netio_core.h"
#include "utilitieslib/network/net_socket.h"
#include "utilitieslib/network/net_link.h"
#include "utilitieslib/network/net_linklist.h"
#include "utilitieslib/network/crypt.h"
#include <zlib.h>
#include "utilitieslib/utils/endian.h"

#include <stdio.h>
#include "utilitieslib/assert/assert.h"

#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/network/sock.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/log.h"
#define UDPIP_OVERHEAD        28

static int lnkBatchSendGenericErrorCheck(NetLink* link);
static void lnkBatchSendAsync(NetLink* link);
static void lnkBatchSendSync(NetLink* link);
static void lnkBatchSendRaw(NetLink* link);
static int lnkCheckUnresponsiveLink(NetLink* link);
static int lnkQueueRetransmits(NetLink* link);
static int __cdecl comparePacketID(const void* pak1InData, const void* pak2InData);

/****************************************************************************************************
 * Batch packet sending                                                                                *
 ****************************************************************************************************/
void lnkBatchSend(NetLink* link)
{
    netioEnterCritical();
    if(link->opType == NLOT_SYNC)
        lnkBatchSendSync(link);
    else
        lnkBatchSendAsync(link);
    netioLeaveCritical();
}

/* Function lnkBatchSendSync()
 *    Syncronous variant of lnkBatchSend().
 *
 */
static void lnkBatchSendSync(NetLink* link){
    if(lnkBatchSendGenericErrorCheck(link))
        lnkBatchSendRaw(link);
}

/* Function lnkBatchSendAsync()
 *    Asyncronous variant of lnkBatchSend().
 *
 */
static void lnkBatchSendAsync(NetLink* link){
    if(!lnkBatchSendGenericErrorCheck(link) || link->awaitingAsyncSendCompletion)
        return;

    lnkBatchSendRaw(link);
}


int num_failed_pktSendRaw = 0;

/* Function lnkBatchSendRaw()
 *    This function sends all packets waiting in the send queue on the given link through
 *    the associated socket.
 *
 *    It implements only the core batch send functionality.
 *
 */
static void lnkBatchSendRaw(NetLink* link){
    Packet* pak;
    int        sentPacket = 0;
    Queue    outgoingQueue;
    Queue    queues[2];
    int        i;
    int        pakSendResult;
    bool    bufferFilled=false;
    int        emptyQueues = 0;

    if (!link->socket)
        return;

    if (linkLooksDead(link, 60) && !link->notimeout) {
        // Kill the link if it is unresponsive.
        if(lnkCheckUnresponsiveLink(link))
        {
            if(!link->disconnected)
                LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "%s:%d Link is unresponsive, discarding", makeIpStr(link->addr.sin_addr.S_un.S_addr), link->addr.sin_port);
            link->logout_disconnect = 1;
            netDiscardDeadLink(link);
        }
        return;
    }

    if (link->awaitingAsyncSendCompletion)
        return;

    if(link->flowControl){
        // How much data can the link send this tick?
        link->availableTickBandwidth += (U32)(link->estimatedBandwidth * timerSeconds(timerCpuTicks() - link->lastSendTime));
        if(link->availableTickBandwidth > link->bandwidthUpperLimit || link->availableTickBandwidth < link->bandwidthLowerLimit)
            link->availableTickBandwidth = link->bandwidthLowerLimit;
    }

    // Send out all packets that need retransmission first.
    lnkQueueRetransmits(link);

    if(link->simulateNetworkCondition && link->simulatedOutOfOrderDelivery){
        if(link->retransmitQueue)
            qRandomize(link->retransmitQueue);
        qRandomize(link->sendQueue2);
    }

    queues[0] = link->retransmitQueue;
    queues[1] = link->sendQueue2;

    for(i = 0; i < 2; i++){
        outgoingQueue = queues[i];
        if(!outgoingQueue)
            continue;

        // Send out all packets in the send queue.
        //    Keep sending the packets while there are still packets in the send queue.
        while(1){
            U32 pakSize;
            // Is there another packet in the queue?
            pak = qPeek(outgoingQueue);

            // Didn't get a packet from the queue?
            if(!pak){
                // Is the queue empty?  If so, all packets have been sent.  We are done sending.
                if(qIsEmpty(outgoingQueue))
                {
                    emptyQueues++;
                    break;
                }
                
                // Otherwise, someone has killed a packet while it was still queued.  Simply discard
                // the invalid entry and continue sending.
                qDequeue(outgoingQueue);
                continue;
            }
            else
                pakSize = pktGetSize(pak);

            if (outgoingQueue == link->retransmitQueue) {
                netLog(link, "Retrans packet with id %d\n", pak->id);
            }
            pakSendResult = pktSendRaw(pak, link);

            // Send the packet.
            if(pakSendResult){
                sentPacket = 1;
                // If the packet was sent successfully,
                // remove the packet from the send queue.
                qDequeue(outgoingQueue);
                //update the enqueued packet size
                link->totalQueuedBytes -= pakSize;

                if(pak->reliable)
                    link->reliablePacketsQueued--;
                // If the packet is relaible, queue it in the acknowledgement waiting queue so that it may be
                // retransmitted if neccessary later.
                // Otherwise, the packet is no longer needed.  Destroy the packet to release resources held by the packet.
                if(outgoingQueue == link->sendQueue2)
                {
                    pak->inSendQueue = 0;
                    if (pak->sib_partnum == 0)
                        link->sendQueuePacketCount--;
                    assert(link->sendQueuePacketCount <= qGetSize(link->sendQueue2));
                    assert(link->sendQueuePacketCount >= 0);
                    // If the packet was sent asyncronously or if the packet needs retransmission,
                    //    keep the packet around.
                    //    Async case:
                    //        OS requires the data to be sent to be available throughout the duration of the operation.
                    //    Retransmission:
                    //        The packet will be needed later should the peer not acknowledge the packet.
                    if(pakSendResult == 2 || (pak->reliable && link->retransmit)){
                        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: moving to acknowlegement waiting array, addr: %x, req loc: %s, %i\n", pak->UID, pak, __FILE__, __LINE__);
                        if (pak->reliable && link->retransmit && link->reliablePacketsArray.size) {
                            assert(pak->id > ((Packet*)link->reliablePacketsArray.storage[link->reliablePacketsArray.size-1])->id ||
                                link->simulateNetworkCondition && link->simulatedOutOfOrderDelivery);
                        }
                        arrayPushBack(&link->reliablePacketsArray, pak);
                    }
                    else
                        pktFree(pak);
                }else{
                    // If the packet came out of the retransmition queue, it is already reliable.
                    pak->inRetransmitQueue = 0;
                }

                if(pakSendResult == 2) {// Async send
                    bufferFilled = true;
                    break;
                }
            }else{
                num_failed_pktSendRaw++;
                // No more data can be sent on this link, quit out of loop.
                LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "Exiting lnkBatchSend raw loop\n");
                bufferFilled = true;
                break;
            }
        }
        
    }

    if(emptyQueues >= 2)    //"2" because there are two queues in a given link.  See the for loop above.
        link->totalQueuedBytes = 0;

    if(link->transmitCallback && link->sendQueuePacketMax - link->sendQueuePacketCount >= 30)
        link->transmitCallback(link);

    //if (sentPacket && count > 7 || qGetSize(link->sendQueue2)) {
    //    printf("Sent: %i (%i), left: %i\n", link->packetsSent, count, qGetSize(link->sendQueue2));
    //}
    if (bufferFilled) {
        // We were unable to send all of our data!
        netLinkAutoGrowBuffer(link, SendBuffer);
    }

    if(sentPacket) {
        link->lastSendTime = timerCpuTicks();
        link->lastSendTimeSeconds = timerCpuSeconds();
    }
}


/* Function lnkBatchSendGenericErrorCheck()
 *    Used by lnkBatchSendXXX to check if the link is capable batch sending.
 *    
 *    Returns:
 *        1 - No error on link.  It safe to proceed with batch sending.
 *        0 - An error has been detected.  Do not perform batch send on the given link.
 */
static int lnkBatchSendGenericErrorCheck(NetLink* link){
    if(!link->socket)
        return 0;

    if(0 && linkLooksDead(link, 60))
        return 0;

    return 1;
}

static int lnkCheckUnresponsiveLink(NetLink* link)
{
    Packet* pak;
    int i;
    U32 curTime = timerCpuSeconds();
    for(i = 0; i < link->reliablePacketsArray.size; i++)
    {
        pak = link->reliablePacketsArray.storage[i];
        if(curTime - pak->creationTime >= 5 * 60)
            return 1;
    }

    return 0;
}

unsigned int calcTimeout(int retransCount, int ping, int old_bias)
{
//    old retransmit numbers
//    if (ping < 65) ping = 65;
//    return ((float)ping/1000 * timerCpuSpeed() * 4);
    int timeout;
    if (old_bias) { // Retransmit time for packets older than ones we've acknowledged
        timeout = ping * 3/2;
    } else { // Retransmit time for the oldest packet that is newer than the the last ack we've received
        timeout = ping * 4;
    }
    retransCount = MIN(retransCount, 2);
    switch (retransCount) {
        case 2: // *4
            timeout <<=1;
        case 1: // *2
            timeout <<=1;
    }
    timeout = MIN(5000, MAX(250, timeout));
    return (unsigned int)((float)timeout/1000 * timerCpuSpeed());
//    return 250/1000.f * timerCpuSpeed(); // For debugging in case ping scheme is wrong
}

static int lnkQueueRetransmits(NetLink* link){
    int i;
    Packet* pak;
    unsigned int curTime;
    unsigned int elapsedTime;
    int    retransmitted = 0;
    static int up = 0;
    static int down = 0;
    int ping;
    U32 oldest_unacknowledged_id=0;

    if(link->disableRetransmits)
        return retransmitted;

    if(!link->retransmit)
        return retransmitted;

    if(!link->reliablePacketsArray.size)
        return retransmitted;

    if(!link->retransmitQueue){
        link->retransmitQueue = createQueue();
        initQueue(link->retransmitQueue, NETLINK_SENDQUEUE_INIT_SIZE);        
        qSetMaxSizeLimit(link->retransmitQueue, NETLINK_SENDQUEUE_MAX_SIZE);    
    }

/*    reliablePacketsArray is now kept sorted
    // Does the reliable packets array need to be cleaned (reordered) first?
    //    Because the way pktProcessAck() works, the reliable packets array eventually becomes
    //    messy.  The packet IDs of the packets in the array becomes more and more mixed up
    //    with each acknowledgement successfully processed.  Eventually, the array will reach
    //    such a state that very very old reliable packets will still be waiting in the array
    //    because it is not getting retransmitted when it needs to be.

    // If 1/4 of the reliable packets in the array has been acknowledged + removed, it's time
    // to clean up the array.
    if(link->rpaCleanAge > (link->reliablePacketsArray.size >> 2) && link->reliablePacketsArray.size > 4){
        static sortCount = 0;
        //printf("sorting: %i\n", ++sortCount);
        qsort(link->reliablePacketsArray.storage, link->reliablePacketsArray.size, sizeof(void*), comparePacketID);
        link->rpaCleanAge = 0;
    }
*/

    // FIXME!!!
    // PktHistory uses floats to track history.  Ping history does not need the precision and it might cause some performance problems.
    curTime = timerCpuTicks();
    ping = pingAvgRate(&link->pingHistory);
    if(ping == 0)
        ping = 250;
    /* now done in clampTimeout
    if (ping>800) {
        ping = 800;
    } else if (ping < 65) {
        ping = 65; // We never want to re-send a packet quicker than once every quarter of a second, since that's how long the server takes to respond
    }
    */

    // Go through each reliable packet that is waiting for an acknowledgement.
    // Only send the packet if a) it is the oldest unacknowledged packet or b) we have received an ack on a later packet
    oldest_unacknowledged_id = ((Packet*)link->reliablePacketsArray.storage[0])->id;
    for(i = 0; i < link->reliablePacketsArray.size; i++){
        unsigned int local_timeout;
        if(qIsReallyFull(link->retransmitQueue))
            break;

        pak = link->reliablePacketsArray.storage[i];

        // Calculate effective timeout
        local_timeout = calcTimeout(pak->retransCount, ping, pak->id < oldest_unacknowledged_id);

        // Caluclate how much time has elapsed.
        elapsedTime = curTime - pak->xferTime;
        
        // Check if the elapsed time is not larger than the timeout time.
        if(elapsedTime <= local_timeout)
            continue;

        // Check to see if this is not the oldest packet
        if (pak->id != oldest_unacknowledged_id) {
            // If so, only retransmit it if we have received a more recent acknoledgement
            if (pak->id > link->idAck)
                continue;
        }

        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: retransmitting packet, elapsed: %f, timeout: %f, req loc: %s, %i\n", pak->UID, timerSeconds(elapsedTime), timerSeconds(local_timeout), __FILE__, __LINE__);
        
        if(!pak->inRetransmitQueue){
            //printf("Marking packet for retrans: %i, current ping %dms, timeout %d, retranscount %d\n", pak->id, ping, local_timeout*1000/timerCpuSpeed(), pak->retransCount);
            qEnqueue(link->retransmitQueue, pak);
            pak->inRetransmitQueue = 1;

            pak->xferTime = timerCpuTicks();
            lnkAddSendTime(link, pak->id, pak->xferTime);

/*
            if(link->addr.sin_addr.S_un.S_un_b.s_b4 == 113){
                printf("Retransmitted packet to %i.%i.%i.%i:%i\n", link->addr.sin_addr.S_un.S_un_b.s_b1, link->addr.sin_addr.S_un.S_un_b.s_b2, link->addr.sin_addr.S_un.S_un_b.s_b3, link->addr.sin_addr.S_un.S_un_b.s_b4, link->addr.sin_port);
                printf("pak id = %i\n", pak->id);
            }
*/
            retransmitted = 1;
            pak->retransCount++;

            link->lost_packet_sent_count++;

            //    If the packet's ID indicates it is sent after the last bandwidth adjustment,
            if(pak->id > link->bandwidthCheckpointID){
                //    Adjust the bandwidth and record the latest packet ID.
                down++;
                link->estimatedBandwidth = link->estimatedBandwidth * 10 / 9;
                if(link->estimatedBandwidth < link->bandwidthLowerLimit)
                    link->estimatedBandwidth = link->bandwidthLowerLimit;
                link->bandwidthCheckpointID = link->nextID;
            }
        }
    }

    // Nothing needed to be retransmitted?
    if(!retransmitted){
        up++;
        // Step up the estimated bandwidth a little.
        link->estimatedBandwidth = link->estimatedBandwidth * 15 / 10;
        if(link->estimatedBandwidth > link->bandwidthUpperLimit)
            link->estimatedBandwidth = link->bandwidthUpperLimit;
        link->bandwidthCheckpointID = link->nextID;
    }

    return retransmitted;
}

static int __cdecl comparePacketID(const void* pak1InData, const void* pak2InData){
    const Packet** pak1In = (const Packet**)pak1InData;
    const Packet** pak2In = (const Packet**)pak2InData;
    const Packet* pak1;
    const Packet* pak2;
    
    pak1 = *pak1In;
    pak2 = *pak2In;
    
    if(pak1->id == pak2->id)
        return 0;
    if(pak1->id > pak2->id)
        return 1;
    else
        return -1;
}

/****************************************************************************************************
 * Single packet sending                                                                            *
 ****************************************************************************************************/
/* Function pktSend()
 *    This function enqueues the given packet into the given link if possible.
 *
 *    This is the top level function that should be used to send a user constructed packet
 *    on the specified NetLink.  Note that in all cases, this function will take ownership
 *    of the packet given to it.
 *    
 *    The packet is first split up into smaller packets if neccessary, then passed on to
 *    pktWrapAndSend to append any necessary info, then added to the send queue of the NetLink.
 *
 */
U32 pktSendDbg(Packet** pakptr, NetLink* link MEM_DBG_PARMS)
{
    Packet* pak_in;
    Packet* pak;
    int        i,bitlen, totalSiblingCount;
    int        maxPacketDataSize;
    U32        sizeOfPacket;

    //printf("*** SENDING PACKET %d: %d bytes\n", time(NULL) % 10, (*pakptr)->stream.size);
    netioEnterCritical();
    if (!link || !link->socket || NLT_NONE == link->type)
    {
        pktFree(*pakptr);
        *pakptr = NULL;
        netioLeaveCritical();
        return 0;
    }


    pak_in = *pakptr;
    // Compress the bitstream
    if(link->compressionAllowed && pak_in->compress)
    {
        F32 before_size = (F32)pktGetSize(pak_in);
        F32 after_size;
        F32 compression_ratio;
        
        // if this is a tcp connection, or its ordered and reliable
        // use the link's streaming facility, otherwise just
        // compress the individual packet
        if(link->type==NLT_TCP || (pak_in->ordered && pak_in->reliable))
        {
            if (!link->zstream_pack)
            {
                link->zstream_pack = calloc(sizeof(z_stream),1);
                deflateInit(link->zstream_pack,Z_DEFAULT_COMPRESSION);
            }
            bsCompress(&pak_in->stream, pak_in->hasDebugInfo, link->zstream_pack);
        }
        else
        {
            bsCompress(&pak_in->stream, pak_in->hasDebugInfo, NULL);
        }
        sizeOfPacket = pktGetSize(pak_in);
        after_size = (F32)sizeOfPacket;
        compression_ratio = before_size > 0 ? after_size/before_size : 0.f;
        
        link->runningCompressionRatio = link->runningCompressionRatio?(.9375f*link->runningCompressionRatio + .0625f*compression_ratio):compression_ratio;
    }
    else
        sizeOfPacket = pktGetSize(pak_in);
    //printf("Sending packet of size %d, bitLength %d\n", (*pakptr)->stream.size, (*pakptr)->stream.bitLength);

    maxPacketDataSize = sendPacketBufferSize - (pak_in->hasDebugInfo?PACKET_MISCINFO_SIZE_WITH_DEBUG:PACKET_MISCINFO_SIZE_WITHOUT_DEBUG);
    assert(maxPacketDataSize>0); // -packetdebug and -mtu < 201 will cause this
    // If the packet is not too large, just send the data.
    if(pak_in->stream.size < (unsigned int)maxPacketDataSize){
        int enqueueResult;

#if _CRTDBG_MAP_ALLOC
        //sprintf((*pakptr)->stack_dump,"%s: %d",caller_fname,line);
#endif
        enqueueResult = lnkAddToSendQueue(link, pakptr, 0);

        if(!enqueueResult){
            LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: cannot enqueue packet for send, addr: %x, req loc: %s, %i\n", pak_in->UID, pak_in, __FILE__, __LINE__);
            pktFree(*pakptr);
            *pakptr = 0;
        }
        else
        {
            link->totalQueuedBytes += sizeOfPacket;
        }
        
        netioLeaveCritical();
        return enqueueResult;
    }

    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: splitting packets for send, addr: %x, req loc: %s, %i\n", pak_in->UID, pak_in, __FILE__, __LINE__);

    // The packet is too large to send all in one go...
    // The packet needs to be split up to smaller packets.
    // Assign the packet a new sibling ID.
    link->nextSibID++;

    // Calculate how many siblings there is going to be.
    totalSiblingCount = (pak_in->stream.size + maxPacketDataSize - 1) / maxPacketDataSize;

    // Split up the packet into smaller sibling packets and add them to the send queue.
    for(i=0;i<totalSiblingCount;i++)
    {
        int enqueueResult;
        pak = pktCreate(); // Not pktCreateEx because we do *not* have a CMD to send
        pak->sib_id = link->nextSibID;
        pak->sib_count = totalSiblingCount;
        pak->sib_partnum = i;
        //assert(pak->hasDebugInfo == pak_in->hasDebugInfo);
        pktMakeCompatible(pak,pak_in);
        
        bitlen = maxPacketDataSize * 8;    // The bit length for each sibling packet is equal.

        // Except for the last sibling packet.
        if (i == totalSiblingCount - 1)
            bitlen = bsGetBitLength(&pak_in->stream) - ((i * maxPacketDataSize) * 8);

        // Grab the piece of data that is supposed to be in this sibling packet.
        //    Do not use the pktSendBitsArray here.
        //    That function has now been changed to incorporate "type" information.
        //    Treat the packet as raw data and do not attach additional bitstream "type" info.
        //    This is done intentionally so that the split packets are of the same format as
        //    the other single packets.  The rest of the packet code can then just treat all
        //    packets as "single" packets without worrying where some additional type info might
        //    be attached.
        bsWriteBitsArray(&pak->stream, bitlen, pak_in->stream.data + i * maxPacketDataSize);
        
        // Send the sibling packet.
#if _CRTDBG_MAP_ALLOC
        //sprintf(pak->stack_dump,"%s: %d",caller_fname,line);
#endif

        enqueueResult = lnkAddToSendQueue(link, &pak, i!=0);
        if(!enqueueResult)
        {
            LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: cannot enqueue packet for send, addr: %x, req loc: %s, %i\n", pak_in->UID, pak_in, __FILE__, __LINE__);
            pktFree(pak);
            break;
        }
        else if(i == 0)
            link->totalQueuedBytes += sizeOfPacket;
    }

    // The original packet has been split up into multiple child packets.  The original is no longer needed.
    pktFree(pak_in);
    *pakptr = NULL;
    
    netioLeaveCritical();
    return 0;
}

/* Function pktSendRaw()
 *    This function sends the given packet out to the socket after wrapping some
 *    information around it.
 *
 *    Returns:
 *        0 - Packet not sent.
 *        1 - Packet sent.
 *        2 - Packet sent asyncronously.
 */
int pktSendRaw(Packet* pak, NetLink* link){
    int amt_sent,retval=1;
    int    length;
    static char* buf = NULL;
    static int sentPartialPacket;
    BitStream stream;

    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: sending, addr: %x, req loc: %s, %i\n", pak->UID, pak, __FILE__, __LINE__);

    // Add simulated lag time here if requested.
    if(link->simulateNetworkCondition){
        F32        lag;
        int        lagVary;

        // Why the heck is the if statement there?
        //if(link->simulatedLagVary >= 1000)
        //    lag = (randInt(link->simulatedLagVary - 1000) + link->simulatedLagTime) * 0.0005f;

        lagVary = randInt(link->simulatedLagVary);
        if(randInt(2)){
            lagVary = -lagVary;
        }

        lag = (lagVary + link->simulatedLagTime) * 0.001f;

        // Don't send the packet unless the simulated lag time is up.
        if(timerSeconds(timerCpuTicks() - pak->xferTime) < lag)
            return 0;
    }

    if(!buf)
        buf = malloc(DEFAULT_PACKET_BUFFER_SIZE);

    assert(DEFAULT_PACKET_BUFFER_SIZE >= sendPacketBufferSize); // Otherwise horrible things will happen

    // Initialize a bitstream.
    initBitStream(&stream, buf, DEFAULT_PACKET_BUFFER_SIZE, Write, false, NULL);

    // Fill the bitstream with the actual data to be sent.
    // "buf" will contain the data after pktWrap() is done.
    pktWrap(pak, link, &stream);
    
    length = (bsGetBitLength(&stream) + 7) >> 3;    // calculate the length of the packet in bytes.
                                                        // This caluclation is needed because additional bits are needed to accommodate the
                                                        // additional bits indicated by pak->stream.cursor.bit.  We round to the nearest byte here.

    if(link->flowControl){
        // Stop sending data if there is no more bandwidth available this tick.
        if(length + UDPIP_OVERHEAD - (int)link->availableTickBandwidth > 0){
            return 0;
        }
    }

    // This causes uninitialized data to checksummed and sent over the network
    length = blowfishPad(length);

    {
        U32 checksum;

        checksum = cryptAdler32(buf+8,length-8);
        *((U32 *)&buf[4]) = endianSwapIfBig(U32, checksum);
    }

    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: BitLength = %i, Checksum = 0x%x, req loc: %s, %i\n", pak->UID, *(int*)&buf[0], *(int*)&buf[4], __FILE__, __LINE__);

    if (link->encrypt_on)
        cryptBlowfishEncrypt(&link->blowfish_ctx,buf+4,length-4);

//    if (pak->sib_id)
//        printf("Send Packet id: %d size : %d sib id: %d part %d/%d  Link UID: %d\n", pak->id, pak->stream.size, pak->sib_id, pak->sib_partnum, pak->sib_count, link->UID);
//    else 
//        printf("Send Packet id: %d size: %d  Link UID: %d\n", pak->id, pak->stream.size, link->UID);
//    assert(!mpReclaimed(pak));

    // Send data according to the type of the link.
    if (NLT_TCP == link->type)
        amt_sent = send(link->socket, buf, length, 0);
    else
        amt_sent = SendToSock(link->socket, buf, length, &link->addr);

    if(link->packetSentCallback){
        link->packetSentCallback(link, pak, length, amt_sent == length);
    }

    // If the number of bytes sent doesn't match the requested send length, the packet was
    // not sent successfully.
    if(amt_sent != length){
        int errorCode;

//        consoleSetFGColor(COLOR_RED | COLOR_BLUE | COLOR_BRIGHT);
//        printf("********** error sending packet %i\n", pak->id);
//        consoleSetFGColor(COLOR_RED | COLOR_GREEN | COLOR_BLUE);
        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: send failed, %i bytes, addr: %x, req loc: %s, %i\n", pak->UID, amt_sent, pak, __FILE__, __LINE__);

        // Hey! TCP sent a partial packet!  The packet code can't deal with this yet.
        assert(amt_sent <= 0);
        errorCode = WSAGetLastError();
        //printf("Error code %i\n", errorCode);

        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: unable to send packet, addr: %x, req loc: %s, %i\n", pak->UID, pak, __FILE__, __LINE__);

        if (NLT_TCP == link->type && NLOT_ASYNC == link->opType){
            AsyncOpContext* context;

            LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: sending packet async, addr: %x, req loc: %s, %i\n", pak->UID, pak, __FILE__, __LINE__);

            context = createAsyncOpContext();
            context->pak = pak;
            retval = 2; // async operation is going unless otherwise noted
            if(!simpleWsaSend(link->socket, buf, length, 0, context)){
                int errVal = WSAGetLastError();

                if(isDisconnectError(errVal)){
                    netDiscardDeadLink(link);
                } else {
                    // It gets into this case if an async operation was queued up
                    if (errVal == WSAENOBUFS) {
                        // The send failed for some reason (possibly 10055="An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.")
                        // There was no async operation queued up, stop trying
                        retval = 0;
                    }
                }
            }
            if (retval == 2) {
                assert(link->awaitingAsyncSendCompletion==0);

                length += UDPIP_OVERHEAD;
                link->totalBytesSent += (length);

                link->awaitingAsyncSendCompletion = 1;
            }
        } else {
            retval = 0;
        }
    }
    else
    {
        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: send succeeded, %i bytes, addr: %x, req loc: %s, %i\n", pak->UID, amt_sent, pak, __FILE__, __LINE__);

#ifdef DUMP_SENT_RAW_PACKETS
        dumpBufferToFile(buf, amt_sent, "c:\\logs\\server\\rawpacket.bin");
#endif

        // Otherwise, the packet was sent successfully.
        length += UDPIP_OVERHEAD;

        // Update this tick's available bandwidth.
        link->availableTickBandwidth -= length;

        // Update the link's packet history.
        // FIXME! This is probably not a good idea anymore.  The packet code now does batch sends and it
        // doesn't make sense for these values to be update individually, especially for high send traffic links.
        link->totalBytesSent += (length);
        
        pktAddHist(&link->sendHistory, length, 1);

        //link->packetsSent++;
        link->totalPacketsSent++;
    }

    return retval;
}


void pktSendCmd(NetLink *link,Packet *pak,int cmd)
{
    devassert(link);
    if (link )
    {
        if(!link->controlCommandsSeparate) {
            // Old client, need to change command number!
            pktSendBitsPack(pak,1,cmd - COMM_MAX_CMD + COMM_OLDMAX_CMD);
        } else {
            pktSendBitsPack(pak,1,cmd);
        }
    }

    assert(cmd >= COMM_MAX_CMD);
    if (cmd >= COMM_MAX_CMD) {
        if (link && link->cookieSend)
        {
            if (!++link->cookie_send)
                link->cookie_send = 1;
            pktSendBits(pak,32,link->cookie_send);
            pktSendBits(pak,32,link->last_cookie_recv);
        }
        else if (link && link->cookieEcho)
        {
            pktSendBits(pak,32,link->cookie_recv);
            pktSendBits(pak,32,link->last_cookie_recv);
            link->cookie_recv = 0;
        }
    }
}
