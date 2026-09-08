#include "utilitieslib/network/netio_core.h"
#include "utilitieslib/network/net_structdefs.h"
#include "utilitieslib/network/net_packet.h"
#include "utilitieslib/network/netio_stats.h"
#include "utilitieslib/network/netio_send.h"
#include "utilitieslib/network/crypt.h"
#include "utilitieslib/network/net_link.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/network/net_packetutil.h"
#include "utilitieslib/utils/endian.h"

#include "utilitieslib/assert/assert.h"
#include <stdio.h>

#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/network/sock.h"
#include "utilitieslib/utils/log.h"
#include "utilitieslib/utils/utils.h"

static int __cdecl cmpU32(const void* aData, const void* bData);
static int mergeSibs(Array* sibs, Packet **pak_dst);
static int pktProcessAck(U32 id, NetLink *link, U32 timestamp);
static void lnkAddSiblingPacket(NetLink* link, Packet* pak);
int g_assert_on_netlink_overflow=0;
int total_sendqueue_overflows=0;

#define PACKET_HEADER_SIZE    sizeof(PacketHeader)
/****************************************************************************************************
 * NetLink packet queuing                                                                            *
 ****************************************************************************************************/
/* Function lnkAddToSendQueue()
 *    This function takes the given packet and attempts to add it the send queue
 *    of the given link.
 *
 *    The ownership of the packet is taken away from the caller when the packet is
 *    queued successfully.
 *
 *  sibling_packets do not count againse the limit to number of packets
 *  sent per tick.
 *
 *    Returns:
 *        1 - Packet queued successfully.
 *        0 - Packet not queued successfully.  (Queue Overflow)
 *
 */
int lnkAddToSendQueue(NetLink* link, Packet** pakptr, int sibling_packet)
{
    Packet* pak = *pakptr;
    // Attempt to enqueue the packet into the send queue.

    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: queuing packet to send, addr: %x, req loc: %s, %i\n", pak->UID, pak, __FILE__, __LINE__);

    if (!sibling_packet && link->sendQueuePacketMax && link->sendQueuePacketCount >= link->sendQueuePacketMax)
    {
        // Unable to enqeue packet.

        // There wasn't any room in the send queue.  The packet will be discarded
        // regardless of the type (reliable/unreliable).
        // Note that if there isn't any room in the send queue and the packet is not
        // marked as reliable, the packet would be silently dropped.
        //
        // This should never happen under normal circumstances.
        // The packet code always clears out the send queue every tick.  The only way 
        // the send queue can be overflowed is by sending NETLINK_SENDQUEUE_MAX_SIZE 
        // number of packets all at once.  Note that if this happens, one way to cope 
        // with the problem is to change NETLINK_SENDQUEUE_MAX_SIZE.  It would probably
        // be better to figure out why so many packets are being sent in one tick.
        //
        // JE: When lots of people are logging into the dbserver on the same tick the sendQueue will
        // overflow, also, when writing via TCP to a hung mapserver, the sendQueue
        // can fill up.  Also some odd circumstance on the Updater is causing this to happen...

        // FIXME!    -    Although overflows should never happen under normal usage,
        //                it might be a good idea to deal with this error more gracefully.

        if(pak->reliable){
            static int lastport=0;
            if (g_assert_on_netlink_overflow) {
                assert(0);
            }
            link->overflow = 1;
            total_sendqueue_overflows++;
            if (link->addr.sin_port!=lastport) {
                printf("Send queue overflow to %s:%d (%d total)!\n", makeIpStr(link->addr.sin_addr.S_un.S_addr), link->addr.sin_port, total_sendqueue_overflows);
                lastport = link->addr.sin_port;
                LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "Overflow(s) adding packet uid: %d to sendQueue to %s:%d, %d total overflows", pak->UID, makeIpStr(link->addr.sin_addr.S_un.S_addr), link->addr.sin_port, total_sendqueue_overflows);
            }
        }

        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: cannot queue packet, addr: %x, req loc: %s, %i\n", pak->UID, pak, __FILE__, __LINE__);
        // Return an error code.
        return 0;
    }
    if (!sibling_packet) {
        link->sendQueuePacketCount++;
    }

    {
        int ret = qEnqueue(link->sendQueue2, pak);
        assert(ret);
    }

    // Packet enqueued properly.
    // Assign it an ID.
    pak->id = ++link->nextID;
    //pak->truncatedID = pak->id & ((1 << 16) - 1);
    pak->truncatedID = pak->id;
    pak->xferTime = timerCpuTicks();    // Fake the send time for network condition simulation.
    pak->inSendQueue = 1;
    lnkAddSendTime(link, pak->id, pak->xferTime);

    if( pak->ordered && !sibling_packet && link->type == NLT_UDP && verify(!pak->ordered_id && pak->reliable) )
        pak->ordered_id = ++link->last_ordered_sent_id;

    if(pak->reliable)
        link->reliablePacketsQueued++;

    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: queued packet, OutID: %i, addr: %x, req loc: %s, %i\n", pak->UID, pak->id, pak, __FILE__, __LINE__);
    *pakptr = NULL;

    // New data has just been queued into the link for sending.
    // Add this link to the list of links to be flushed.
    lnkFlushAddLink(link);

    return 1;
}

/* Function lnkAddToReceiveQueue()
 *    This function queues the given packet in the received queue in the given link.
 *    Other parts of the application can later access the link and process the packet
 *    in a context sensitive way.
 *
 *    Note that this function assumes that the given packet is wrapped and will proceed
 *    to unwrap it first before queueing it.
 *
 */

int lnkAddToReceiveQueue(NetLink* link, Packet** pakptr){
    Packet* pak = *pakptr;

    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: queuing packet from peer, addr: %x, req loc: %s, %i\n", pak->UID, pak, __FILE__, __LINE__);

    // Is there space in the queue for enqueuing?
    if(qIsReallyFull(link->receiveQueue)){
        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: cannot queue packet from peer, addr: %x, req loc: %s, %i\n", pak->UID, pak, __FILE__, __LINE__);
        return 0;
    }

    // Try to unwrap this packet first.
    // If unwrapping failed, something is wrong with this
    // packet.  Don't let it go into the queue.
    if(pktUnwrap(pak,link) < 0){
        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: cannot queue invalid packet from peer, addr: %x, req loc: %s, %i\n", pak->UID, pak, __FILE__, __LINE__);
        return 0;
    }

    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0,"ID: %i, msg: queued packet from peer, InID: %i, addr: %x, req loc: %s, %i\n", pak->UID, pak->id, pak, __FILE__, __LINE__);
    
    // If the packet has siblings, put it in the sibling waiting array instead of
    // the receive queue.
    // WARNING! FIXME! BUG!  EXPLOIT!  It is possible to make the receiver save off
    // an large number of fake sibling packets, which will use up all the resources
    // of the receiver, and eventually bring the entire system down.
    if(pak->sib_id){
        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: queued sibling packet from peer, SibID: %i, addr: %x, req loc: %s, %i\n", pak->UID, pak->sib_id, pak, __FILE__, __LINE__);
        //printf("Packet id: %d sib id: %d part %d/%d\n", pak->id, pak->sib_id, pak->sib_partnum, pak->sib_count);
        lnkAddSiblingPacket(link, pak);
    }
    else if( pak->ordered )
    {
        lnkAddOrderedPacket(link, pak);
    }
    // Enqueue the packet now.
    // This should always succeed since we've already determined that the receive
    // queue has at least one single slot for this packet earlier.
    else {
        if(!qEnqueue(link->receiveQueue, pak))
            assert(0);
        //printf("Packet id: %d size: %d\n", pak->id, pak->stream.size);
    }

    link->totalPacketsRead++;
    link->totalBytesRead += (*pakptr)->stream.size;
    
    *pakptr = NULL;

    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: queued packet from peer, addr: %x, req loc: %s, %i\n", pak->UID, pak, __FILE__, __LINE__);
    return 1;
}

/****************************************************************************************************
 * NetLink packet merging                                                                            *
 ****************************************************************************************************/
/* Function pktMerge()
 *    Attempts to retrieve a packet from the link by merging sibling packets.    
 *
 *    Parameters:
 *        link - the link to look for siblings in.
 *        pak_dst - the merged packet, if any, will be returned here.
 *
 *    Returns:
 *        1 - Found and merged some siblings in the queue.
 *        0 - Did not find any appropriate siblings for merging.
 *
 */
int pktMerge(NetLink* link,Packet** pak_dst)
{

    int        i;
    Array* sibs;
    int mergeResult;

    // If no siblings are ready for merging, nothing can be merged.
    assert(link->siblingMergeArray.size);
    
    // Merge the siblings in the first packet array that is ready for merging.
    sibs = link->siblingMergeArray.storage[0];
    mergeResult = mergeSibs(sibs, pak_dst);
    //assert(mergeResult);

    // Remove all siblings from the sibling waiting array.
    {
        Packet* sibPacket;

        // Destroy all packets in the sibling packets array.
        for(i = 0; i < sibs->size; i++){
            sibPacket = sibs->storage[i];
            LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: partial packet merged, addr: %x, req loc: %s, %i\n", sibPacket->UID, sibPacket, __FILE__, __LINE__);
            pktFree(sibPacket);
        }

        clearArray(sibs);
    }
    arrayRemoveAndFill(&link->siblingMergeArray, 0);

    return mergeResult;
}

/* Function cmpSib()
 *    Compares two sibling packets by comparing their part number.
 *    When paired with qsort, this function ensure that the siblings
 *    are sorted from smallest to largest part numbers.
 *
 *
 */
int __cdecl cmpSib(const void* aData, const void* bData)
{
    const Packet ** a = (const Packet **)aData;
    const Packet ** b = (const Packet **)bData;
    return (*a)->sib_partnum - (*b)->sib_partnum;
}

/* Function mergeSibs()
 *    Merge an array of sibling packets into a single packet.  It is assumed that the
 *    packets in the array are in no particular order.  The function leave the given
 *    array as is, and returns a valid packet via pak_dst.
 *
 *    Parameters:
 *        sibs - Array of sibling packets.
 *        
 *    Returns:
 *        1 - siblings merged successfully
 *        0 - siblings not merged successfully.
 *
 */
static int mergeSibs(Array* sibs, Packet **pak_dst){
    int    i;
    Packet* pak;
    Packet* sibPacket;
    
    // Make sure that given array can be merged.
    //assert(sibs->size && sibs->size == ((Packet*)(sibs->storage[0]))->sib_count);
    
    // Sort the sibling in increasing part number order.
    qsort(sibs->storage, sibs->size, sizeof(Packet*), cmpSib);
    
    //printf("merged sibling packets of size");
    // Concatenate all sibling data together into a new packet.
    pak = pktCreate();
    for(i = 0; i < sibs->size; i++)
    {
        sibPacket = sibs->storage[i];
        assert(sibPacket->sib_id == ((Packet*)sibs->storage[0])->sib_id);
        //printf(" %d,", sibPacket->stream.size);
        bsWriteBitsArray(&pak->stream, bsGetBitLength(&sibPacket->stream) - bsGetCursorBitPosition(&sibPacket->stream), &sibPacket->stream.data[sibPacket->stream.cursor.byte]);
        if (sibPacket->sib_count != sibs->size) {
            // Sib_count was written wrong on the sending side!
            pktFree(pak);
            return 0;
        }
    }
    //printf("\b \n");

    // Ready the packet for reading.
    bsChangeMode(&pak->stream, Read);
    
    sibPacket = sibs->storage[0];
    pak->id = sibPacket->id;
    pak->sib_count = sibPacket->sib_count;
    pak->reliable = sibPacket->reliable;
    pak->hasDebugInfo = sibPacket->hasDebugInfo;
    pak->compress = sibPacket->compress;
    pak->ordered = sibPacket->ordered;
    pak->ordered_id = sibPacket->ordered_id;
    pak->xferTime = ((Packet*)sibs->storage[0])->xferTime; //timerCpuTicks();
    pktSetByteAlignment(pak, sibPacket->stream.byteAlignedMode);

    *pak_dst = pak;

    return 1;

}

/* Function lnkAddSiblingPacket()
 *    This function adds the given sibling packet into the storage area of the given link.
 *    
 */
static void lnkAddSiblingPacket(NetLink* link, Packet* pak){
    Array* holdingArea = NULL;

    {
        int i;
        int foundHoldingArea = 0;
        Array* emptyArray = NULL;

        // Check all sibling holding areas for an array of packets with the same sibling id with the given packet.
        // if an empty array is found, use that to store this pak.
        for(i = 0; i < link->siblingWaitingArea.size; i++){
            holdingArea = link->siblingWaitingArea.storage[i];
            if(holdingArea->size > 0){
                if(((Packet*)holdingArea->storage[0])->sib_id == pak->sib_id){
                    foundHoldingArea = 1;
                    break;
                }
            }else
                emptyArray = holdingArea;
        }

        // If such an array was not found, create one or reuse an existing empty array now.
        if(!foundHoldingArea){
            // If we found an emtpy array while examining the siblingWaitingArea array, reuse it.
            if(emptyArray)
                holdingArea = emptyArray;
            else{
                // Otherwise, create a new one and add it to the waiting area array.
                holdingArea = createArray();
                arrayPushBack(&link->siblingWaitingArea, holdingArea);
            }
        }
    }
    
    {
        // If there are packets in the array, make sure that the given packet belongs in that array.
        // If the sibling ID's of the packets in the array and the sibling ID of the given packet
        // does not match, that means the link's sibling storage area is too small.
        // Either make it larger or start using a hash table.

        // At this point, either
        //    1) The array holds no other packets, or
        //    2) There are packets in the array and they appear to be the given packet's siblings.
        //    
        // In either case, it is now appropriate to add the packet to the array.
        arrayPushBack(holdingArea, pak);

        // If all siblings have arrived, the sibling packets are ready to be merged.
        if(holdingArea->size == pak->sib_count){
            // Place the holding area into the sibling merge array of the link so that
            // the packets can be merged the next time a packet is extracted from this link.
            arrayPushBack(&link->siblingMergeArray, holdingArea);
        }
        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: stored sibling packet from peer, %i/%i, req loc: %s, %i\n", pak->UID, holdingArea->size, pak->sib_count, __FILE__, __LINE__);
    }
}

/* Function cmpOrdered()
   * for qsort of ordered paks
 */
int __cdecl cmpOrdered(const Packet **a, const Packet **b, const void *context)
{
    return (*a)->ordered_id - (*b)->ordered_id;
}

/* Function lnkAddOrderedPacket()   
 *    Add an ordered packet to the ordered packet storage. if this is the first packet expected
 *    put it in the receive queue.
 */
static int cmpOrderedAdapter(const void* arg0, const void* arg1, const void* arg2)
{
    return cmpOrdered((const Packet **)arg0, (const Packet **)arg1, (const void *)arg2);
}

void lnkAddOrderedPacket(NetLink* link, Packet* pak)
{
    // tcp is already ordered, so just add it.
    if(link->type == NLT_TCP)
    {
        if(!qEnqueue(link->receiveQueue, pak))
            assert(0);
    }
    else if(verify(pak->ordered))
    {
        int i;
        
        arrayPushBack(&link->orderedWaitingPaks, pak);
        stableSort(link->orderedWaitingPaks.storage, link->orderedWaitingPaks.size, sizeof(pak), NULL, cmpOrderedAdapter);

        for(i = 0; 
            i < link->orderedWaitingPaks.size 
                && (link->last_ordered_recv_id+1) == ((Packet*)link->orderedWaitingPaks.storage[i])->ordered_id; 
            ++i)
        {
            Packet *pakHd = link->orderedWaitingPaks.storage[i];
            devassert(link->last_ordered_recv_id < pakHd->ordered_id);
            if(!qEnqueue(link->receiveQueue, pakHd))
                assert(0);
            link->last_ordered_recv_id = pakHd->ordered_id;
            link->orderedWaitingPaks.storage[i] = NULL;
        }

        if(i>0)
            arrayCompact(&link->orderedWaitingPaks);
        else
            link->ooo_ordered_packet_recv_count++;

        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: stored ordered packet from peer, %i/%i, req loc: %s, %i\n", pak->UID, link->orderedWaitingPaks.size, pak->ordered_id, __FILE__, __LINE__);
    }
}

/****************************************************************************************************
 * NetLink flushing                                                                                    *
 ****************************************************************************************************/
static Array linkFlushArray;        // Holds links that are waiting to be flushed.

/******************************************************************
 * Link Flush Functions
 *    These functions are used to help track which links needs to be flushed.
 *
 */

int lnkHasOutgoingData(NetLink* link)
{
    if(qGetSize(link->sendQueue2) == 0 && qGetSize(link->retransmitQueue) == 0)
        return 0;
    else
        return 1;
}

/* Function lnkFlush()
 *    This function flushes as much pending outgoing data as it can.
 *
 *    This function should be used in favor of lnkBatchSend().
 *    lnkBatchSend().
 *
 *    Note that this may not flush all data that are sitting in the link.
 */
void lnkFlush(NetLink* link){
    if (!link->disconnected)
        lnkBatchSend(link);

    //if(qGetSize(link->sendQueue2) == 0)
    //    link->waitingForFlush = 0;
}

void lnkFlushAddLink(NetLink* link){
    if(!link->waitingForFlush){
        arrayPushBack(&linkFlushArray, link);
        link->waitingForFlush = 1;
    }
}

void lnkFlushAll(){
    int i;

    netioEnterCritical();
    for(i = 0; i < linkFlushArray.size; i++){
        NetLink* link = linkFlushArray.storage[i];
        lnkFlush(link);
        if(!lnkHasOutgoingData(link))
        {
            link->waitingForFlush = 0;
            arrayRemoveAndFill(&linkFlushArray, i);
            i--;
        }
    }
    netioLeaveCritical();
}

void lnkRemoveFromFlushList(NetLink* link)
{
    int index = arrayFindElement(&linkFlushArray, link);
    if(index >= 0)
    {
        link->waitingForFlush = 0;
        arrayRemoveAndFill(&linkFlushArray, index);
    }
}
/*
 * Link Flush Functions
 ******************************************************************/

/****************************************************************************************************
 * NetLink misc                                                                                        *
 ****************************************************************************************************/
int linkLooksDead(NetLink *link, int threshold)
{
    int        dead = 0, t;

    if (link->type == NLT_UDP)
    {
        // If this link has not ever received anything, it is not possible to determine if
        // the link looks dead or not.
        if(link->lastRecvTime)
        {
            t = timerCpuSeconds() - link->lastRecvTime;

            // The link is considered to be dead if:
            //    1) no response was received in the past X seconds. (for low traffic links)
            if (t > threshold)
                dead = 1;
            //JE: Removed this condition, since an editing packet can be > 1.5M, which
            // instantly puts more than 1000 packets unack'ed, before they're even sent.
            //    2) more than 1000 packets are unack'ed.  (for high traffic links)
            //if (link->nextID - link->idAck > 1000)
            //    dead = 1;
        }
    }
    return dead;
}

void lnkSimulateNetworkConditions(NetLink* link, int lag, int lagVary, int packetLost, int outOfOrder)
{
    netioEnterCritical();
    link->simulatedLagVary = lagVary;
    link->simulatedLagTime = lag;
    link->simulatedPacketLostRate = packetLost;
    link->simulatedOutOfOrderDelivery = outOfOrder;
    if(lag | packetLost | lagVary | outOfOrder){
        link->simulateNetworkCondition = 1;
    }else
        link->simulateNetworkCondition = 0;
    if (outOfOrder && lag>250) {
        printf("Warning, a bug in our poor network conditions simulation may cause packets to never be delivered if Out of Order and Lag are both enabled\n");
    }
    netioLeaveCritical();
}


/****************************************************************************************************
 * Packet Wrapping                                                                                    *
 ****************************************************************************************************/

/* Function cmpU32()
 *    Compares two pointers to unsigned 32 bit integers.
 *
 *    Returns:
 *        >1 - "a" is greater
 *        0 - integers are equal
 *        <1 - "b" is greater.
 *
 */
static int __cdecl cmpU32(const void* aData, const void* bData)
{
    U32* a = (U32*)aData;
    U32* b = (U32*)bData;
    return *a - *b;
}

U32 pktSendAcks(Packet* pak, NetLink* link, int sendRequestCount, int* lastSentIndex, unsigned int maxPacketHeaderSize)
{
    int sentCount = 0;
    int i;
    assert(lastSentIndex);

    pktSendBitsPack(pak, 1, sendRequestCount);
    if(sendRequestCount){
        // Add all the ack's into the packet.
        // The first acknowledgement is sent using the "full", "truncated" ID of the packet to be ack'ed.
        pktSendBits(pak, 32, link->ids_toack[0]);
        sentCount++;
        *lastSentIndex = 0;

        //if (!pak->hasDebugInfo)
        //    assert(pak->stream.size < PACKET_MISCINFO_SIZE);

        // Subsequent acknowledgements are sent in incremental form.
        for(i = 1; i < link->ack_count && sentCount < sendRequestCount; i++){
            // Skip all acks with the same sequence number.
            if(link->ids_toack[i] == link->ids_toack[i-1])
                continue;

            {
                int value = link->ids_toack[i] - link->ids_toack[i-1] - 1;
                int len = pktGetLengthBitsPack(pak, 1, value);
                if (pak->stream.bitLength + len >= maxPacketHeaderSize*8) {
                    break;
                }

                pktSendBitsPack(pak, 1, value);
                sentCount++;
                *lastSentIndex = i;
            }
        }

        if (!pak->hasDebugInfo && pak->sib_count < 32) // With a lot of sib packets (should only happen with the editor) this might go over the limit on the first ack!
            assert(pak->stream.size <= maxPacketHeaderSize);
    }

    return sentCount;
}

/* Function pktWrap()
 *    Prepares the raw data to be sent over the socket to the bitstream
 *    by wrapping additional information around the given packet (user data).
 *
 *    pak_in - the packet containing the data
 *    stream - the stream to be filled with the wrapped data
 */
U32 pktWrap(Packet* pak_in, NetLink* link, BitStream* stream)
{
    int i;
    //BitStream cloneStream;
    int uniqueAckCount = 0;
    static int send_count;
    Packet* pak;
    Packet pak_buf;
    unsigned int bitLengthPosition;
    int cmd=-1;
    unsigned int maxPacketHeaderSize = (pak_in->hasDebugInfo?PACKET_MISCINFO_SIZE_WITH_DEBUG:PACKET_MISCINFO_SIZE_WITHOUT_DEBUG) - 8; // - blowfish pad
    //int maxPacketDataSize = sendPacketBufferSize - maxPacketHeaderSize;

    // Get the cmd from the packet
    if (!pak_in->sib_id) {
        bsRewind(&pak_in->stream);
        cmd = pktGetBitsPack(pak_in, 1);
        bsRewind(&pak_in->stream);
    }

    // Create a dummy packet to be filled with real outgoing data.
    pak = &pak_buf;
    *pak = *pak_in;
    pak->stream = *stream;

    // Write out a dummy bitlength
    //cloneBitStream(stream, &cloneStream);    // Save the place where the bitlength should be written to.
                                            // FIXME! The bitstream should know how to position itself without
                                            //          choking.  Implement something like ftell and fseek.
    bitLengthPosition = bsGetCursorBitPosition(stream);
    bsWriteBits(&pak->stream, 32, 0); //pak->stream.cursor.byte += 4;
    

    // FIXME!! Put in the checksum!
    // Calculate the checksum of the data held in the packet.
    bsWriteBits(&pak->stream, 32, pak_in->checksum);
    //*((int*)&pak->stream.data[pak->stream.cursor.byte]) = pak_in->checksum;
    //pak->stream.cursor.byte += 4;

    pak->stream.data[pak->stream.cursor.byte] = 0;
    assert(pak->stream.cursor.bit==0);

    bsWriteBits(&pak->stream, 1, pak_in->hasDebugInfo);

    pktSendBits(pak, 32, pak_in->truncatedID);

    // Has this packet been split up?
    pktSendBitsPack(pak, 1, pak_in->sib_count);
    if (pak_in->sib_count)
    {
        // If so, add identifiers to uniquely idenfity which
        // sibling this packet is.
        pktSendBitsPack(pak, 1, pak_in->sib_partnum);
        pktSendBits(pak, 32, pak_in->sib_id);
    }

    // Want to piggy back acknowlegements for received packets?
    // Count how many acks should be sent with this packet.
    if(link->ack_count){
        assert(link->ack_count < MAX_PKTACKS);

        // Sort all acks in increasing order.
        qsort(link->ids_toack,link->ack_count,sizeof(U32),
            cmpU32);

        // Count how many unique acks there are.
        uniqueAckCount = 1;                    // There is at least one unique ack.
        for(i=1;i<link->ack_count;i++){
            // Skip all acks with the same sequence number.
            if(link->ids_toack[i] == link->ids_toack[i-1])
                continue;

            uniqueAckCount++;
        }
    }    

    {
        int bitCursor = bsGetCursorBitPosition(&pak->stream);
        int sentCount;
        int sendReqCount = uniqueAckCount;
        int lastSentIndex = 0;

        // try to send all acks.
        sentCount = pktSendAcks(pak, link, sendReqCount, &lastSentIndex, maxPacketHeaderSize);

        // If it is not possible to set all acks, restore the bitstream cursor
        // and place the proper number of acks into the packet.
        if(sentCount != sendReqCount)
        {
            sendReqCount = sentCount;
            bsSetEndOfStreamPosition(&pak->stream, bitCursor);
            sentCount = pktSendAcks(pak, link, sendReqCount, &lastSentIndex, maxPacketHeaderSize);
            assert(sentCount == sendReqCount);
        }

        // Get rid of the sent acks.
        if(sentCount)
        {
            memmove(link->ids_toack, &link->ids_toack[lastSentIndex+1], (link->ack_count - (lastSentIndex+1))*sizeof(int));
            link->ack_count -= (lastSentIndex+1);
        }
    }

    if (link->compressionAllowed && cmd!=COMM_CONNECT) {
        // Only with version 1+ packets, and *not* on the connect packet under
        //  any circumstances (it was doing it when re-transmitting the
        //  packet)
        pktSendBits(pak, 1, pak->compress);
    } else {
        assert(!pak->compress);
    }

    // ordered
    if(link->orderedAllowed && cmd!=COMM_CONNECT)
    {
        pktSendBits(pak,1,pak->ordered);
        if( pak->ordered )
        {
            pktSendBitsAuto( pak, pak->ordered_id );
        }
    }

    // Pack up all the data in the given packet.
    pktSetByteAlignment(pak, link->compressionAllowed && pak->compress); // This is only needed so that the packetdebug info gets aligned as well, since sendBitsArray auto-aligns
    pktSendBitsArray(pak, bsGetBitLength(&pak_in->stream), pak_in->stream.data);
    

    // Write out the actual bitlength of the stream now.
    assert((bitLengthPosition & 7) == 0); // If it's not, we need to turn off byte aligning, write the data (and then turn it back on?)
    bsSetCursorBitPosition(&pak->stream, bitLengthPosition);
    bsWriteBits(&pak->stream, 32, bsGetBitLength(&pak->stream));
    // Seek to the end when done?  Not really needed.
    // The clone stream is still pointing at the beginning of the stream.
    ////bsWriteBits(&cloneStream, 32, bsGetBitLength(&pak->stream));
    //*((int*)&cloneStream.data[0]) = bsGetBitLength(&pak->stream);

    assert(!pak->stream.errorFlags);

    *stream = pak->stream;

    return 1;
}


#define RETURN_ON_BAD_STREAM  if (pak->stream.errorFlags) return -1;

/* Function pktUnwrap()
 *    This is the sibling function of pktWrapAndSend().
 *
 *    This function takes a wrapped packet, decodes all the wrappings and
 *    updates the NetLink with data neccessary to make the packet code
 *    function correctly.  All received packets must come through here.
 *
 *    Returns:
 *        -1  - Duplicate packet.
 *        int - The real length of the unwrapped packet.
 */
int pktUnwrap(Packet* pak, NetLink* link)
{
    int        num_acks, i;
    U32        id, id_part=0;
    int        saved_cursor;

    // Simulate packet lost.
    if(link->simulateNetworkCondition){
        if (randInt(100) < link->simulatedPacketLostRate)
        {
            LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "UID: %i, msg: packet rejected for packet lost simulation, addr: %x, req loc: %s, %i\n", pak->UID, pak, __FILE__, __LINE__);
            return -1;
        }
    }

    // Header info is not byteAligned
    pktSetByteAlignment(pak, 0);

    // How should fields in this packet be extracted? normally? or with debug info?
    pak->hasDebugInfo = bsReadBits(&pak->stream, 1);
    RETURN_ON_BAD_STREAM;

    // If a peer started sending packets that contains debug info, assume that all network traffic
    // should now enter debug mode.  Set the flag the forces all network traffic to be sent with debug info.
    // Turned off to prevent the effect from cascading through all servers.
    //if(pak->hasDebugInfo)
    //        pktSetDebugInfo();

    // Figure out the full packet ID from the truncated ID.
    //    Extract the truncated ID.
    pak->id = pak->truncatedID = pktGetBits(pak,32);
    RETURN_ON_BAD_STREAM;

    if (link->ack_count < MAX_PKTACKS-1){
        if (!link->ack_count || link->ids_toack[link->ack_count-1]!=pak->id)
            link->ids_toack[link->ack_count++] = pak->id;
    }

    pak->sib_count = pktGetBitsPack(pak,1);
    RETURN_ON_BAD_STREAM;
    if (pak->sib_count)
    {
        pak->sib_partnum = pktGetBitsPack(pak,1);
        RETURN_ON_BAD_STREAM;
        pak->sib_id = pktGetBits(pak,32);
        RETURN_ON_BAD_STREAM;
    }

    // Does this packet come with any piggy backed acknowledgements?
    num_acks = pktGetBitsPack(pak,1);
    RETURN_ON_BAD_STREAM;

    // If so, grab the first full packet ID.
    if (num_acks){
        id_part = pktGetBits(pak, 32);
        RETURN_ON_BAD_STREAM;
    }

    // Construct the full packet ID from the trucated packet id.
    id = id_part;

    if (num_acks) {
        int        processed_ack=0;
        U32        timestamp = timerCpuTicks();
        if (num_acks < 10) {
            // Any more than this and we might have had a stall, just average the values to inflate the ping
            pingBatchBegin(&link->pingHistory);
        }
        for(i = 0; i < num_acks; i++)
        {
            if (i) {
                id += pktGetBitsPack(pak,1) + 1;
                RETURN_ON_BAD_STREAM;
            }

            // Record the newest packet ID acknowledgement received on this link.
            if (id > link->idAck)
                link->idAck = id;

            processed_ack = pktProcessAck(id, link, timestamp) || processed_ack;
            if (link->recv_ack_count == ARRAY_SIZE(link->recv_acks))
                link->recv_ack_count = 0;
            link->recv_acks[link->recv_ack_count++] = id;
        }
        if (num_acks < 10) {
            pingBatchEnd(&link->pingHistory);
        }

        if (processed_ack) {
            // A packet was found and removed from the array, compact the cleared out elements
            arrayCompactStable(&link->reliablePacketsArray);
        }
    }

    /* Moved into pktGet()
    // Did we miss a packet from the sender?
    if (pak->id != link->last_recv_id+1)
    {
        link->lost_packet_recv = 1;
        link->lost_packet_recv_count++;
    }
    // Update the last processed packet ID.
    link->last_recv_id = pak->id;
    */

    // Decide whether to accept the packet or by examining its full ID.
    //    If this block decides to reject the packet, it will return -1 and stop
    //    all further processing, otherwise, it will make the proper data updates
    //    and allow the packet to continue through the function.
    //    To be accepted, a packet's ID must meet the following conditions:
    //        1) It must not be a duplicate or a previously receievd packet ID.
    //        2) It must be within a 16k packet window.
    //
    //    FIXME!!!
    //        
    {
        // If the link has already received this packet, reject the new packet.
        // Otherwise, accept the packet and update the table.
        
        SimpleSetElement* element;
        
        // Look up the packet id in the received packet ID table.
        element = sSetFindElement(link->receivedPacketID, S32_TO_PTR(pak->id));
        
        //    Was the slot in the set occupied at all?
        if(element){
            // If the ID matches, a packet with the particular ID has already been received.
            // The packet should be discarded.
            if(element->value == S32_TO_PTR(pak->id)){
                LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: packet duplicate rejected, addr: %x, req loc: %s, %i\n", pak->UID, pak, __FILE__, __LINE__);
                link->duplicate_packet_recv_count++;
                //consoleSetFGColor(COLOR_RED | COLOR_BRIGHT);
                //printf("***** duplicate packet %i\n", pak->id);
                //consoleSetFGColor(COLOR_RED | COLOR_GREEN | COLOR_BLUE);
                return -1;
            }
            
            // If the ID is smaller than the one retrieved, the packet is *very late*.  The
            // received packet IDs have already wrapped around the set/table.  This should NEVER happen, unless
            // the packet code isn't working correctly.
            if(element->value > S32_TO_PTR(pak->id)){
                LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: invalid/late packet rejected, addr: %x, req loc: %s, %i\n", pak->UID, pak, __FILE__, __LINE__);
                //consoleSetFGColor(COLOR_RED | COLOR_BRIGHT);
                //printf("***** very late %i\n", pak->id);
                //consoleSetFGColor(COLOR_RED | COLOR_GREEN | COLOR_BLUE);
                return -1;
            }

            
            // The packet is accepted.  Update the set/table now.
            element->value = S32_TO_PTR(pak->id);
        }else{
            // The slot where the ID should be stored is empty.  The packet has not been received before.
            // Add the packet ID as a new entry now.
            sSetAddElement(link->receivedPacketID, S32_TO_PTR(pak->id));
        }
    }

    // Read flag(s)
    if (link->compressionAllowed ) {
        // Version 1+ packets only
        pak->compress = pktGetBits(pak, 1);
        RETURN_ON_BAD_STREAM;
    } else {
        pak->compress = 0;
    }

    // ordered
    if(link->orderedAllowed )  {
        pak->ordered = pktGetBits(pak,1);
        RETURN_ON_BAD_STREAM;
        if( pak->ordered ) {
            pak->ordered_id = pktGetBitsAuto(pak);
            RETURN_ON_BAD_STREAM;
        }
    } else {
        pak->ordered = 0;
    }

    saved_cursor = bsGetCursorBitPosition(&pak->stream); // for debugging
    pktSetByteAlignment(pak, pak->compress); // All actual data was byteAligned if the packet was compressed
    RETURN_ON_BAD_STREAM;
    pktAlignBitsArray(pak);
    RETURN_ON_BAD_STREAM;

    // Update packets received history.
    pktAddHist(&link->recvHistory, pak->stream.size + PACKET_HEADER_SIZE + 28, 0);

    if (link->ack_count >= MAX_PKTACKS - 1)
    {
        // Flush these acks!
        netIdleEx(link, 0, 0, -1);
    }

    return pak->stream.size;
}


/* Function pktSkipWrapping()
 *    This is the sibling function of pktWrapAndSend().
 *
 *    This function takes a wrapped packet, skips over the unnecessary stuff
 *  and only grabs what is needed right away.  Eventually, pktUnwrap will
 *  update the netlink with the useful information extracted from here.
 *
 *    Returns:
 *        -1  - Duplicate or bad packet.
 *        int - The real length of the unwrapped packet.
 */
int pktSkipWrapping(Packet* pak, NetLink* link){
    int        num_acks, i;
    U32        id, id_part=0;

    // Header info is not byteAligned
    pktSetByteAlignment(pak, 0);

    pak->hasDebugInfo = bsReadBits(&pak->stream, 1);
    RETURN_ON_BAD_STREAM;

    //    Extract the truncated ID.
    pak->id = pak->truncatedID = pktGetBits(pak,32);
    RETURN_ON_BAD_STREAM;

    if (!link && pak->id > 1) {
        // Not connect packet, and has a higher ID, must be a resent packet from a closed link
        return -1;
    }
    
    pak->sib_count = pktGetBitsPack(pak,1);
    RETURN_ON_BAD_STREAM;
    if (pak->sib_count)
    {
        pak->sib_partnum = pktGetBitsPack(pak,1);
        RETURN_ON_BAD_STREAM;
        pak->sib_id = pktGetBits(pak,32);
        RETURN_ON_BAD_STREAM;
    }

    // Does this packet come with any piggy backed acknowledgements?
    num_acks = pktGetBitsPack(pak,1);
    RETURN_ON_BAD_STREAM;

    // If so, grab the first full packet ID.
    if (num_acks){
        id_part = pktGetBits(pak, 32);
        RETURN_ON_BAD_STREAM;
    }

    // Construct the full packet ID from the trucated packet id.
    id = id_part;

    for(i = 0; i < num_acks; i++)
    {
        if (i) {
            id += pktGetBitsPack(pak,1) + 1;
            RETURN_ON_BAD_STREAM;
        }
    }

    if (link && link->compressionAllowed && pak->id != 1) { 
        // Version 1+ packets only
        pak->compress = pktGetBits(pak, 1);
        RETURN_ON_BAD_STREAM;
    } else {
        pak->compress = 0;
    }

    if( link && link->orderedAllowed && pak->id != 1 )
    {
        pak->ordered = pktGetBits(pak,1);
        RETURN_ON_BAD_STREAM;
        if( pak->ordered ) {
            pak->ordered_id = pktGetBitsAuto(pak);
            RETURN_ON_BAD_STREAM;
        }
    } else {
        pak->ordered = 0;
    }
    

    // Actual data is byte aligned if the link and packet say so
    pktSetByteAlignment(pak, pak->compress);
    
    pktAlignBitsArray(pak);
    RETURN_ON_BAD_STREAM;

    return pak->stream.size;
}

/* Function pktProcessAck()
 *    Process an acknowlegement for a packet with the given ID in the given link.
 *
 *
 *
 */
static int pktProcessAck(U32 id, NetLink *link, U32 timestamp){
    int        i;
    Packet    *pak;
    int        foundPacket = 0;

    if (link->type != NLT_TCP) {
        // For TCP links we don't want to processes acks, since if the packet is still
        // in the reliablePacketsArray we need it to stay there for the async
        // operation to complete

        //printf("recv ack %d\n", id);

        // Look in the send queue for the packet with the matching ID.
        for(i = link->reliablePacketsArray.size-1; i >= 0; i--){

            pak = link->reliablePacketsArray.storage[i];
            
            // Found the packet with matching ID?
            if (pak && pak->id == id){

                LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: packet acknowledgement %i received, addr: %x, req loc: %s, %i\n", pak->UID, id, pak, __FILE__, __LINE__);

                // Calculate how long it took for the acknowledgement to get through since
                // packet creation and use it as the current ping of the link.
                pingAddHist(&link->pingHistory, (int)(timerSeconds(timestamp - pak->xferTime) * 1000));

                // The packet might have been queued for retransmission again.
                // Remove the packet from the retransmit queue.
                if(link->retransmitQueue){
                    int idx;

                    idx = qFindIndex(link->retransmitQueue, pak);
                    if(idx != -1){
                        Packet** element;
                        element = (Packet**)qGetElement(link->retransmitQueue, idx);
                        (**element).inRetransmitQueue = 0;
                        link->totalQueuedBytes -= pktGetSize(*element);
                        *element = NULL;
                    }
                }

                // The packet has been acknowledged.
                // Remove the packet from the ack waiting array
                pktFree(pak);                                        // Destroy the packet first.
                //JE: Changed this to a shift (NULL'd here and compacted later) so that it keeps the array ordered
                arraySetAt(&link->reliablePacketsArray, i, NULL);    // Remove the entry in the ack waiting array.
                link->rpaCleanAge++;

                foundPacket = 1;
                break;
            }
        }
    }

    // If the given packet ID was not found in the reliable packets array, try the pakSendTimes array.
    if(!foundPacket){
        int sendTime;
        sendTime = lnkGetSendTime(link, id);
        if(sendTime)
            pingAddHist(&link->pingHistory, (int)(timerSeconds(timerCpuTicks() - sendTime) * 1000));
        return 0;
    } else {
        return 1;
    }
}


int blowfishPad(int num){
    return 4 + ((num -4 +7) & ~7);
}

/****************************************************************************************************
 * Packet Validation                                                                                *
 ****************************************************************************************************/
int pktDecryptAndCrc(Packet *pak, NetLink *link, struct sockaddr_in *addr, bool log_err){
    U32 ck2,checksum;

    if (link && link->decrypt_on)
        cryptBlowfishDecrypt(&link->blowfish_ctx,pak->stream.data+4,pak->stream.size-4);

    checksum = *((U32 *)&pak->stream.data[4]);
    checksum = endianSwapIfBig(U32, checksum);
    ck2 = cryptAdler32(pak->stream.data+8,pak->stream.size-8);
    if (ck2 != checksum)
    {
        if (link)
            addr = &link->addr;

        if(log_err)
            badPacketLog(BADPACKET_BADCHECKSUM, addr?addr->sin_addr.S_un.S_addr:0, addr?addr->sin_port:0);
        //printf("bad checksum on packet (err #%d ip: %s, size: %d)\n",bad_checksum_count,addr ? makeIpStr(addr->sin_addr.S_un.S_addr) : "unknown addr", pak->stream.size);
        if (link)
            link->bytesRead = link->bytesToRead = 0;
        return 0;
    }
    // Repair the .size (previously it included the padded size)
    pak->stream.size = (pak->stream.bitLength + 7) >> 3;
    return 1;
}


/****************************************************************************************************
 * Debug Utilities                                                                                    *
 ****************************************************************************************************/
void dumpBufferToFile(unsigned char* buffer, int size, char* filename){
    static FILE    *file = NULL;

    if(!file){
        file = fopen(filename,"wb");
        if(!file){
            file = fopen("c:/temp/pkts.bin","wb");
            assert(file);
        }
    }

    fwrite(buffer, sizeof(unsigned char), size, file);
}

void dumpBufferToFile2(unsigned char* buffer, int size, char* filename){
    static FILE    *file = NULL;

    if(!file){
        file = fopen(filename,"wb");
        if(!file){
            file = fopen("c:/temp/pkts.bin","wb");
            assert(file);
        }
    }

    fwrite(buffer, sizeof(unsigned char), size, file);
}

void pktDump(Packet* pak, char* filename){
    FILE* file;


    file = fopen(filename, "wb");
    //fprintf(file, "--- Packet dump begin ---\n");
    //fprintf(file, "Bit length: %i\n", bsGetBitLength(&pak->stream));
    //fprintf(file, "Byte length: %i\n", pktGetSize(pak));
    //fprintf(file, "  --- Packet binary dump begin ---\n");
    fwrite(pak->stream.data, 1, pak->stream.size, file);
    //fprintf(file, "  --- Packet binary dump end ---\n");
    //fprintf(file, "--- Packet dump end ---\n\n\n");
    fclose(file);
}

static char *badPacketNames[] = {
    "Bad checksum",
    "Bad packet header (size mismatch)",
    "Too large of a packet",
    "Bad data",
};

int badPacketFlags[] = {
    0,
    1,
    1,
    0,
};

int glob_print_all_errors=0;

void badPacketLog(BadPacketType type, U32 ipaddr, U32 port)
{
    static U32 total_count[NUM_BADPACKETTYPES];
    static U32 count[NUM_BADPACKETTYPES];
    static U32 lastip[NUM_BADPACKETTYPES];

    PERFINFO_AUTO_START("badPacketLog", 1);
    
    //TODO: log these somehow!
    if (count[type]>12 && ipaddr != lastip[type] || 
        count[type] >= 1000)
    {
        if (badPacketFlags[type] || isDevelopmentMode()) {
            printf("%s err #%d, %d packets from %s\n", badPacketNames[type], total_count[type], count[type], lastip[type] ? makeIpStr(lastip[type]) : "unknown addr");
        }
        count[type]=0;
    } else if (ipaddr != lastip[type]) {
        count[type]=0;
    }
    lastip[type] = ipaddr;
    count[type]++;
    total_count[type]++;
    if (glob_print_all_errors) {
        printf("%s err #%d, %d packets from %s:%d\n", badPacketNames[type], total_count[type], count[type], lastip[type] ? makeIpStr(lastip[type]) : "unknown addr", port);
        count[type]=0;
    }
    
    PERFINFO_AUTO_STOP();
}

#if NETIO_THREAD_SAFE
static CRITICAL_SECTION netio_critical_section;
static int netio_critical_section_init;

void netioEnterCritical()
{
    netioInitCritical();
    EnterCriticalSection(&netio_critical_section);
}

void netioLeaveCritical()
{
    LeaveCriticalSection(&netio_critical_section);
}

void netioInitCritical()
{
    if(!netio_critical_section_init)
    {
        netio_critical_section_init = 1;

        InitializeCriticalSection(&netio_critical_section);
    }
}
#endif

