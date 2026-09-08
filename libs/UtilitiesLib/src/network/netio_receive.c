#include "utilitieslib/network/netio_receive.h"
#include "utilitieslib/network/net_structdefs.h"
#include "utilitieslib/network/net_packet.h"
#include "utilitieslib/network/netio_core.h"
#include "utilitieslib/network/net_socket.h"
#include "utilitieslib/network/net_linklist.h"
#include "zlib/zlib.h"

#include "utilitieslib/assert/assert.h"
#include <stdio.h>

#include "utilitieslib/network/sock.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/endian.h"
#include "utilitieslib/utils/log.h"

#ifdef LOG_NETIO_ACTIVITIES
#define if_log_netio(x) x
#else
#define if_log_netio(x)
#endif

#define PACKET_HEADER_SIZE    sizeof(PacketHeader)
/****************************************************************************************************
 * Batch packet receiving                                                                            *
 ****************************************************************************************************/
/* Function lnkBatchReceive()
 *    Retrieves all available data waiting in the socket associated with the given
 *    link until either the link receive queue is full or the socket has no more
 *    data for retrieval.
 *
 */
void lnkBatchReceive(NetLink* link){
    int        amt = 0;
    struct    sockaddr_in addr;
    Packet* pak=NULL;
    static int count;
    int gotRawPackets = 0;

    netioEnterCritical();
    // Continue grabbing packets from the socket until the receive queue is full.
    while(!qIsReallyFull(link->receiveQueue)){
        // Grab some data according to the type of the link.
        if (NLT_TCP == link->type)
            amt = pktGetTcp(&pak,link);
        else
        {
            amt = pktGetUdp(&pak,link->socket,&addr);
            // Did we really get some data or did we get a disconnect signal from the OS?
            if(-1 == amt)
            {
                netDiscardDeadLink(link);
            }

            if (amt > 0 && !pktDecryptAndCrc(pak,link,&addr,true))
            {
                amt = 0;
            }
        }
        
        // If there is no more data on the socket, quit out of this data grabbing loop.
        if (amt <= 0){
            if (pak) {
                pktFree(pak);
                pak = NULL;
            }
            break;
        }

        gotRawPackets = 1;

        // Perform an data integrity check in case data was corrupted or tampered with.
        // FIXME! It appears that checksums are not used at all right now.
        //if (pak->checksum != crc(-1,pak->data,pak->length - PACKET_HEADER_SIZE))
        //    FatalErrorf("Packet checksum error!");        // FIXME! This is a BAD idea, as tampered packets will halt the entire program.
        
        if(!lnkAddToReceiveQueue(link, &pak)){
            pktFree(pak);
            pak = NULL;
        }

    }

    if(gotRawPackets)
        link->lastRecvTime = timerCpuSeconds();
    netioLeaveCritical();
}



/****************************************************************************************************
 * Single packet receiving                                                                            *
 ****************************************************************************************************/
static int __cdecl packetCompare(const void* pak1Data, const void* pak2Data){
    const Packet** pak1 = (const Packet**)pak1Data;
    const Packet** pak2 = (const Packet**)pak2Data;
    return (*pak2)->id - (*pak1)->id;
}

/* Function pktGet()
 *    A top level function that attempts to retrieve a packet from the given
 *    NetLink.
 *
 *    Returns:
 *        non-zero - packet retrieved successfully.
 *        0 - unable to retrieve packet.
 *
 */
int pktGet(Packet **pakptr,NetLink *link){
    int gotNewPacket = 0;
    Packet *pakptr_temp=NULL;

    netioEnterCritical();
    // Try to merge some packets together first if it is possible.
    // ab: the siblingMergeArray contains Array*'s of completed receives of sibling
    // packets. see lnkAddSiblingPacket for where these are added to this array.
    // Why aren't they merged there? the world will never know.
    while(link->siblingMergeArray.size){
        if(!pktMerge(link, &pakptr_temp))
            continue;
            
        if( pakptr_temp->ordered )
        {
            lnkAddOrderedPacket(link, pakptr_temp);
        }
        else
        {
            arrayPushBack(&link->receivedPacketStorage, pakptr_temp);
            gotNewPacket = 1;
        }
    }
    
    // If merging packets didn't work, try to grab something directly from the receive queue.
    while(qGetSize(link->receiveQueue)){
        pakptr_temp = qDequeue(link->receiveQueue);

        arrayPushBack(&link->receivedPacketStorage, pakptr_temp);
        gotNewPacket = 1;
    }

    if(gotNewPacket && link->receivedPacketStorage.size){
        qsort( link->receivedPacketStorage.storage, link->receivedPacketStorage.size, sizeof(void*), packetCompare);
    }

    if(link->receivedPacketStorage.size){
        *pakptr = arrayPopBack(&link->receivedPacketStorage);
    }else
        *pakptr = NULL;


    if (*pakptr) {
        // Did we miss a packet from the sender?
        if ((*pakptr)->id < link->last_recv_id)
        {
            link->lost_packet_recv = 1;
            link->lost_packet_recv_count++;
        }
        // Update the last processed packet ID.
        link->last_recv_id = (*pakptr)->id;

        //printf("Extracted packet of size %d, bitLength %d, cursor %d.%d, eff bL: %d\n", (*pakptr)->stream.size, (*pakptr)->stream.bitLength, (*pakptr)->stream.cursor.byte, (*pakptr)->stream.cursor.bit, (*pakptr)->stream.bitLength - ((*pakptr)->stream.cursor.byte<<3) + (*pakptr)->stream.cursor.bit);
        if((*pakptr)->compress) {
            assert(link->compressionAllowed);
            if(link->type==NLT_TCP || ((*pakptr)->ordered && (*pakptr)->reliable))
            {
                if ( !link->zstream_unpack )
                {
                    link->zstream_unpack = calloc(sizeof(z_stream),1);

                    // This has to be called before inflateInit() because the zalloc and zfree pointers
                    // in link->zstream_unpack have to be setup before inflateInit() is called!
                    bsInitZStreamStruct(link->zstream_unpack);

                    inflateInit(link->zstream_unpack);
                }
                bsUnCompress(&(*pakptr)->stream, (*pakptr)->hasDebugInfo, link->zstream_unpack);
            }
            else
            {
                bsUnCompress(&(*pakptr)->stream, (*pakptr)->hasDebugInfo, NULL);
            }
        }
    }
    netioLeaveCritical();
    return *pakptr? 1: 0;
}


int pktGetUdpAsync(Packet** pakptr, NetLinkList* list, struct sockaddr_in *addr){
    int        amt;
    PacketHeader header;
    static Packet* pak = NULL;

    list->asyncRecvAddrLen = sizeof(struct sockaddr_in);

    if(ASU_ExtractBody == list->asyncUdpState){
        // Create a packet to be filled with the incoming data.
        //    The main purpose of this packet is to grab some buffer space that can be taken out
        //    of this function without a memcpy.
        if(!pak){
            pak = pktCreate();
        }
        pak->xferTime = timerCpuTicks();

        // Start a async receive operation.
        while(1){
            amt = simpleWSARecvFrom(list->udp_sock, pak->stream.data, pak->stream.maxSize, 0, (struct sockaddr *)addr, &list->asyncRecvAddrLen, &list->receiveContext);

            if(SOCKET_ERROR == amt){
                int errVal;

                errVal = WSAGetLastError();
                if(isDisconnectError(errVal)){
                    netDiscardDeadUDPLink(list, addr);
                }

                continue;
            }

            return 0;
        }
    }

    if(ASU_ExtractBodyComplete == list->asyncUdpState){
        int amt;
        amt = list->asyncUdpBytesRead;
        list->asyncUdpBytesRead = 0;

        // Move back to the previous state
        list->asyncUdpState = ASU_ExtractBody;

        if (amt < sizeof(PacketHeader)) {// This packet was less than 8 bytes long!  discard it
            badPacketLog(BADPACKET_SIZEMISMATCH, addr?addr->sin_addr.S_un.S_addr:0, addr?addr->sin_port:0);
            return -1;
        }
        // Grab the packet header.
        memcpy(&header, pak->stream.data, sizeof(PacketHeader));

        header.packetBitLength = endianSwapIfBig(U32,header.packetBitLength);
        header.packetChecksum = endianSwapIfBig(U32,header.packetChecksum);

        // Did we get the entire UDP packet?
        // If not, discard it by saying nothing was read.
        if (((header.packetBitLength + 7) >> 3) > (U32)amt) {// amt can be larger due to 8 byte blowfish blocks
            badPacketLog(BADPACKET_SIZEMISMATCH, addr?addr->sin_addr.S_un.S_addr:0, addr?addr->sin_port:0);
            return -1;
        }

        if (((header.packetBitLength + 7) >> 3) + 7 < (U32)amt) {
            badPacketLog(BADPACKET_SIZEMISMATCH, addr?addr->sin_addr.S_un.S_addr:0, addr?addr->sin_port:0);
            return -1;
        }

        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: raw packet received from addr: %i.%i.%i.%i:%i, req loc: %s, %i\n", pak->UID, 
            addr->sin_addr.S_un.S_un_b.s_b1,
            addr->sin_addr.S_un.S_un_b.s_b2,
            addr->sin_addr.S_un.S_un_b.s_b3,
            addr->sin_addr.S_un.S_un_b.s_b4,
            addr->sin_port,
            __FILE__, __LINE__);

        pak->stream.bitLength = header.packetBitLength;            // Set the bit length of the stream manually since we're not going through the bitstream interface.
        // FIXME!!!
        // It is not a good idea to rely on other modules to constantly fixup the state of a bitstream.
        // If the implementation of a bitstream should change slightly, there would be a bunch of places
        // where implementation dependent code in other modules must also be changed.

        pak->stream.size = (header.packetBitLength + 7) >> 3;    // Update the packet size
        bsChangeMode(&pak->stream, Read);                        // Change the packet's bitstream to read mode.
        pak->stream.size = amt; // actual size + blowFish encoding
        pak->stream.cursor.byte += PACKET_HEADER_SIZE;            // Point the stream to just after the packet header.

        *pakptr = pak;                                            // Return the packet to the calling function and
        pak = NULL;                                                // give up ownership of the packet.
        return amt;
    }

    assert(0);
    return 0;
}


/* Function pktGetUdp()
 *    Grabs a datagram from the given socket then returns the retrieved data
 *    through the given packet.
 *
 *    Note that sometimes the UDP socket will receive a "WSAECONNRESET" error
 *    when the peer disconnects unexpectedly.  This does not mean there are
 *    no more packets waiting on the socket, but it does mean that nothing
 *    has been extracted.
 *
 *    Parameters:
 *        pak - valid pointer to a packet to be filled with incoming data.
 *        socket - a valid socket to read data from.
 *        addr - a valid pointer sockaddr structure to be filled with address info of the sender.
 *
 *    Returns:
 *        > 0 - number of bytes read.
 *        0 - no more message to be read.
 */
int pktGetUdp(Packet** pakptr,int socket,struct sockaddr_in *addr)
{
    int        amt;
    int        addr_len = sizeof(struct sockaddr_in);
    PacketHeader header;
    static Packet* pak = NULL;

    // Create a packet to be filled with the incoming data.
    //    The main purpose of this packet is to grab some buffer space that can be taken out
    //    of this function without a memcpy.
    if(!pak){
        pak = pktCreate();
    }
    pak->xferTime = timerCpuTicks();

    // Grab a UDP packet.
    amt = recvfrom(socket, pak->stream.data, pak->stream.maxSize,0,(struct sockaddr *)addr,&addr_len);
    // Don't do anything if a packet could not be properly retrieved because either,
    //    1) Nothing is waiting in the socket.  Or
    //    2) The client sent a message that exceeds max packet length.
    if (SOCKET_ERROR == amt){
        int errVal;
        /*
        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0,"ID: %i, msg: Received partial header, %i bytes long, addr: %x, req loc: %s, %i\n", pak->UID, amt, pak, __FILE__, __LINE__);
        );
        */
        errVal = WSAGetLastError();
        if(isDisconnectError(errVal))
        {
            // Tell the caller that someone has disconnected.
            return -1;
        }
            
        return 0;
    }
    

    // Grab the packet header.
    memcpy(&header, pak->stream.data, sizeof(PacketHeader));

    header.packetBitLength = endianSwapIfBig(U32,header.packetBitLength);
    header.packetChecksum = endianSwapIfBig(U32,header.packetChecksum);

    // Did we get the entire UDP packet?
    // If not, discard it by saying nothing was read.
    if (((header.packetBitLength + 7) >> 3) > (U32)amt) {// amt can be larger due to 8 byte blowfish blocks
        badPacketLog(BADPACKET_SIZEMISMATCH, addr?addr->sin_addr.S_un.S_addr:0, addr?addr->sin_port:0);
        return -1;
    }

    if (((header.packetBitLength + 7) >> 3) + 7 < (U32)amt) {
        badPacketLog(BADPACKET_SIZEMISMATCH, addr?addr->sin_addr.S_un.S_addr:0, addr?addr->sin_port:0);
        return -1;
    }

    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: raw packet received from addr: %i.%i.%i.%i:%i, req loc: %s, %i\n", pak->UID, 
            addr->sin_addr.S_un.S_un_b.s_b1,
            addr->sin_addr.S_un.S_un_b.s_b2,
            addr->sin_addr.S_un.S_un_b.s_b3,
            addr->sin_addr.S_un.S_un_b.s_b4,
            addr->sin_port,
            __FILE__, __LINE__);

    pak->stream.bitLength = header.packetBitLength;            // Set the bit length of the stream manually since we're not going through the bitstream interface.
                                                            // FIXME!!!
                                                            // It is not a good idea to rely on other modules to constantly fixup the state of a bitstream.
                                                            // If the implementation of a bitstream should change slightly, there would be a bunch of places
                                                            // where implementation dependent code in other modules must also be changed.

    pak->stream.size = (header.packetBitLength + 7) >> 3;    // Update the packet size
    bsChangeMode(&pak->stream, Read);                        // Change the packet's bitstream to read mode.
    pak->stream.size = amt;                                    // Set the size to the full, padded size, because blowfish expects it
    pak->stream.cursor.byte += PACKET_HEADER_SIZE;            // Point the stream to just after the packet header.

    *pakptr = pak;                                            // Return the packet to the calling function and
    pak = NULL;                                                // give up ownership of the packet.

    return amt;
}



//#undef if_log_packets
//#define if_log_packets(x) x

/* Function pktGetTcp()
 *    Grabs a packet from the given NetLink.  The function will do nothing
 *    unless the link is TCP (stream) based.
 *
 *    The function Accumulates received data in the given packet as the
 *    function gets called over and over.  It eventually returns 1 when
 *    the entire packet has been assembled.
 *
 *    WARNING!
 *        The way the function is implemented means that the caller
 *        must pass back the same packet back to this function until
 *        the function returns 1 because the function returns 0
 *        even if it has partially filled the packet with incoming data.
 *
 *    Returns:
 *        1 - A full packet has been retrieved.
 *        0 - A full packet has not been retrieved.
 *
 */
int pktGetTcpAsync(Packet** pakptr, NetLink* link)
{
    int        amt;
    Packet* pak;
    PacketHeader header;
    int        extractedFullPacket = 0;
    
    // Create a packet to be filled with the incoming data to be held in by the link.
    if(!link->pak){
        link->pak = pktCreate();
    }

    pak = link->pak;
    pak->xferTime = timerCpuTicks();

    if(AST_None == link->asyncTcpState || AST_WaitingToStart == link->asyncTcpState){
        link->asyncTcpState = AST_ExtractHeader;
    }
    
    // Did we retrieve the packet header yet?
    // If not, retrieve it now.
    if(AST_ExtractHeader == link->asyncTcpState){
        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: Extracting Tcp header, req loc: %s, %i\n", pak->UID, __FILE__, __LINE__);

        //assert((PACKET_HEADER_SIZE - link->bytesRead) > 0);
        pak->stream.cursor.byte += link->asyncTcpBytesRead;
        link->bytesRead += link->asyncTcpBytesRead;
        link->asyncTcpBytesRead = 0;

        if(PACKET_HEADER_SIZE == link->bytesRead){
            link->asyncTcpState++;
            goto ExtractHeaderComplete;
        }
        amt = simpleWSARecv(link->socket, pak->stream.data + link->bytesRead, PACKET_HEADER_SIZE - link->bytesRead, 0, &link->receiveContext);

        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: Extracted %i tcp header bytes, req loc: %s, %i\n", pak->UID, amt, __FILE__, __LINE__);

        // If nothing can be retrieved from the socket, nothing else can be done.
        if(amt <= 0){
            int errVal = WSAGetLastError();

            // If the peer has disconnected, mark the socket to be destroyed.
            if(isDisconnectError(errVal)){
                netDiscardDeadLink(link);
            }

            LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: Header read pending, req loc: %s, %i\n", pak->UID, __FILE__, __LINE__);
            return extractedFullPacket;
        }

        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0,"ID: %i, msg: Header read completed, req loc: %s, %i\n", pak->UID, __FILE__, __LINE__);    
        return extractedFullPacket;

#if 0 // poz ifdefed this unreachable code

        // If data was retrieved, we must have gotten the entire header.  Otherwise, WSARecv should have returned
        // 0 and started an async read.
        assert(amt == PACKET_HEADER_SIZE);

        // Update the amount of data read and goto the next async state.
        link->asyncTcpBytesRead = amt;
        

        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "Falling through to extract header complete, req loc: %s, %i\n", __FILE__, __LINE__);
#endif
    }


    if(AST_ExtractHeaderComplete == link->asyncTcpState){
ExtractHeaderComplete:
        //printf("Completed extracting header\n");
        // The header has been retrieved.
        // Account for the bytes that were read during the async operation.
        assert(PACKET_HEADER_SIZE == link->bytesRead);
        link->bytesRead += link->asyncTcpBytesRead;
        link->asyncTcpBytesRead = 0;

        // The entire packet header should have been read.
        // Otherwise, there is no reason for the async read operation to become "completed".
        assert(link->bytesRead == PACKET_HEADER_SIZE);

        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: Received complete header, addr: %x, req loc: %s, %i\n", pak->UID, pak, __FILE__, __LINE__);

        // Retrieve the packet header.
        memcpy(&header, pak->stream.data, sizeof(PacketHeader));

        header.packetBitLength = endianSwapIfBig(U32,header.packetBitLength);
        header.packetChecksum = endianSwapIfBig(U32,header.packetChecksum);

        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: BitLength = %i, Checksum = 0x%x, req loc: %s, %i\n", pak->UID, header.packetBitLength, header.packetChecksum, __FILE__, __LINE__);

        link->bytesToRead = ((header.packetBitLength + 7) >> 3);
        if (1 || link->decrypt_on)
            link->bytesToRead = blowfishPad(link->bytesToRead);

        pak->stream.bitLength = header.packetBitLength;
        //pak->stream.size = (pak->stream.bitLength + 7) >> 3; // This value gets overwritten down below

        // Goto the next state now.
        link->asyncTcpState++;

        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "Falling through to extract body, req loc: %s, %i\n", __FILE__, __LINE__);
    }

    if(AST_ExtractBody == link->asyncTcpState){
        // Attempt to read the body of the data now.
        //assert((link->bytesToRead - link->bytesRead) > 0);

        link->bytesRead += link->asyncTcpBytesRead;
        link->asyncTcpBytesRead = 0;

        if(link->bytesRead == link->bytesToRead){
            link->asyncTcpState++;
            goto ExtractBodyComplete;
        }

        // Discard packets that have too much data!
        if (link->bytesToRead > (int)pak->stream.maxSize) {
            badPacketLog(BADPACKET_TOOBIG, link->addr.sin_addr.S_un.S_addr, link->addr.sin_port);
            netDiscardDeadLink(link);
            return 0;
        }
        if (link->bytesToRead < link->bytesRead) {
            // The header tells us the packet is smaller than the header!  Someone's messing with us.
            badPacketLog(BADPACKET_SIZEMISMATCH, link->addr.sin_addr.S_un.S_addr, link->addr.sin_port);
            netDiscardDeadLink(link);
            return 0;
        }

        link->asyncReadBodyCount++;
        amt = simpleWSARecv(link->socket, pak->stream.data + link->bytesRead, link->bytesToRead - link->bytesRead, 0, &link->receiveContext);

        //printf("got %i bytes\n", amt);
        // Didn't get any data?
        // The packet is still only partially read.
        // Report nothing has been read yet.
        if(amt <= 0){
            int errVal = WSAGetLastError();

            LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "IO pending?, req loc: %s, %i\n", __FILE__, __LINE__);
            // If the peer has disconnected, marked the socket to be destroyed.
            if(isDisconnectError(errVal)){
                netDiscardDeadLink(link);
            }
            return extractedFullPacket;
        }

        link->asyncReadBodyCompletedImm++;
        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0,"Read completed immediately, amt: %i, req loc: %s, %i\n", amt, __FILE__, __LINE__);
        
        return extractedFullPacket;

#if 0 // poz ifdefed this unreachable code

        // We should have gotten all of the packet body.
        assert((amt + 8) == link->bytesToRead);

        link->asyncTcpBytesRead = amt;
#endif        
    }


    if(AST_ExtractBodyComplete == link->asyncTcpState){
ExtractBodyComplete:
        // The header has been retrieved.
        // Account for the bytes that were read during the async operation.
        link->bytesRead += link->asyncTcpBytesRead;
        link->asyncTcpBytesRead = 0;


        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0,"ID: %i, msg: Received complete TCP packet, %i bytes long, addr: %x, req loc: %s, %i\n", pak->UID, link->bytesRead, pak, __FILE__, __LINE__);

#ifdef DUMP_RECV_RAW_PACKETS
        dumpBufferToFile(pak->stream.data, link->bytesRead, "c:\\logs\\client\\rawpacket.bin");
#endif


        // All data for this packet has been read.
        // Retrieve the packet checksum.
        // FIXME!  Don't do this for now.  It appears that checksums are not used at all right now.
        //pak->checksum = *((U32 *)(pak->data + sizeof(pak->length)));

        // Report that one packet has been read.
        *pakptr = link->pak;
        link->pak = NULL;
        link->bytesRead = 0;
        // Warning, this sets the stream.size so that it is not consistent with the bitLength,
        //  *but* this is required because the crypt and CRC stuff expect this alrger, padded size
        pak->stream.size = link->bytesToRead;

        if (!pktDecryptAndCrc(pak,link,0,true))
        {
            link->bytesRead = link->bytesToRead = 0;
            badPacketLog(BADPACKET_BADDATA, link->addr.sin_addr.S_un.S_addr, link->addr.sin_port);
            netDiscardDeadLink(link);
            return extractedFullPacket;
        }

        bsChangeMode(&pak->stream, Read);                        // Change the packet's bitstream to read mode.
        pak->stream.cursor.byte += PACKET_HEADER_SIZE;            // Point the stream to just after the packet header.

        link->asyncTcpState = AST_WaitingToStart;
        extractedFullPacket = 1;
    }

    return extractedFullPacket;
}
//#undef if_log_packets
//#define if_log_packets(x) 

static int checkReturnForDisconnect(int amt, NetLink *link) {
    if(amt <= 0){
        int errVal = WSAGetLastError();

        // If the peer has disconnected, marked the socket to be destroyed.
        if(isDisconnectError(errVal) || amt==0){
            netDiscardDeadLink(link);
        }
        return 1;
    }    
    return 0;
}


int pktGetTcp(Packet** pakptr, NetLink* link)
{
    int        amt;
    Packet* pak;
    PacketHeader header;
    

    // Create a packet to be filled with the incoming data to be held in by the link.
    if(!link->pak){
        link->pak = pktCreate();
    }

    pak = link->pak;
    pak->xferTime = timerCpuTicks();

    // Did we retrieve the packet header yet?
    // If not, retrieve it now.
    if(link->bytesRead < PACKET_HEADER_SIZE){
        amt = recv(link->socket, pak->stream.data + link->bytesRead, PACKET_HEADER_SIZE - link->bytesRead, 0);

        // If nothing can be retrieved from the socket, nothing else can be done.
        if (checkReturnForDisconnect(amt, link))
            return 0;

#ifdef PATCHCLIENT
        dumpBufferToFile2(pak->stream.data + link->bytesRead, amt, "c:\\logs\\client\\rawpacket2.bin");
#endif

        // Otherwise, some data has been retrieved.
        pak->stream.cursor.byte += amt;
        link->bytesRead += amt;

        // Did we get the full header?
        if(link->bytesRead < PACKET_HEADER_SIZE){
            LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: Received partial header, %i bytes long, addr: %x, req loc: %s, %i\n", pak->UID, amt, pak, __FILE__, __LINE__);
            return 0;
        }
        
        
        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0,"ID: %i, msg: Received complete header, addr: %x, req loc: %s, %i\n", pak->UID, pak, __FILE__, __LINE__);

        // Retrieve the packet header.
        memcpy(&header, pak->stream.data, sizeof(PacketHeader));

        header.packetBitLength = endianSwapIfBig(U32,header.packetBitLength);
        header.packetChecksum = endianSwapIfBig(U32,header.packetChecksum);

        link->bytesToRead = ((header.packetBitLength + 7) >> 3);
        if (1 || link->decrypt_on)
            link->bytesToRead = blowfishPad(link->bytesToRead);
        pak->stream.bitLength = header.packetBitLength;
    }

    // Discard packets that have too much data!
    if (link->bytesToRead > (int)pak->stream.maxSize) {
        badPacketLog(BADPACKET_TOOBIG, link->addr.sin_addr.S_un.S_addr, link->addr.sin_port);
        netDiscardDeadLink(link);
        return 0;
    }
    if (link->bytesToRead < link->bytesRead) {
        // The header tells us the packet is smaller than the header!  Someone's messing with us.
        badPacketLog(BADPACKET_SIZEMISMATCH, link->addr.sin_addr.S_un.S_addr, link->addr.sin_port);
        netDiscardDeadLink(link);
        return 0;
    }

    // Attempt to read the body of the data now.
    amt = recv(link->socket, pak->stream.data + link->bytesRead, link->bytesToRead - link->bytesRead,0);

    // Didn't get any data?
    // The packet is still only partially read.
    // Report nothing has been read yet.
    if (checkReturnForDisconnect(amt, link))
        return 0;

    link->bytesRead += amt;

    // Did we get all the data for this packet?
    // If not, report nothing has been read yet.
    if (link->bytesRead < link->bytesToRead){
        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: Received partial TCP packet, %i bytes long, addr: %x, req loc: %s, %i\n", pak->UID, amt, pak, __FILE__, __LINE__);
        return 0;
    }

    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0,"ID: %i, msg: Received complete TCP packet, %i bytes long, addr: %x, req loc: %s, %i\n", pak->UID, link->bytesRead, pak, __FILE__, __LINE__);

#ifdef DUMP_RECV_RAW_PACKETS
    dumpBufferToFile(pak->stream.data, link->bytesRead, "c:\\logs\\client\\rawpacket.bin");
#endif

                
    // All data for this packet has been read.
    // Retrieve the packet checksum.
    // FIXME!  Don't do this for now.  It appears that checksums are not used at all right now.
    //pak->checksum = *((U32 *)(pak->data + sizeof(pak->length)));

    // Report that one packet has been read.
    *pakptr = link->pak;
    link->pak = NULL;
    link->bytesRead = 0;
    pak->stream.size = link->bytesToRead;

    
    if (!pktDecryptAndCrc(pak,link,0,true))
    {
        link->bytesRead = link->bytesToRead = 0;
        return 0;
    }
    bsChangeMode(&pak->stream, Read);                        // Change the packet's bitstream to read mode.
    pak->stream.cursor.byte += PACKET_HEADER_SIZE;            // Point the stream to just after the packet header.

    return 1;
}


int pktGetTcpSync(Packet** pakptr, NetLink* link)
{
    int        amt;
    Packet* pak;
    PacketHeader header;
    

    // Create a packet to be filled with the incoming data to be held by the link.
    if(!link->pak){
        link->pak = pktCreate();
    }

    pak = link->pak;    
    pak->xferTime = timerCpuTicks();

    while(1){
        link->bytesRead += link->asyncTcpBytesRead;
        link->asyncTcpBytesRead = 0;

        switch(link->asyncTcpState){
            case AST_None:
            case AST_WaitingToStart:
            case AST_ExtractHeader:
                link->asyncTcpState = AST_ExtractHeader;

                // Was the header already retrieved by a previous async operation?
                // If so, go on to the next step.
                if(link->bytesRead >= PACKET_HEADER_SIZE){
                    link->asyncTcpState = AST_ExtractHeaderComplete;
                    break;;
                }

                // If not, retrieve the header now.
                amt = recv(link->socket, pak->stream.data + link->bytesRead, PACKET_HEADER_SIZE - link->bytesRead, 0);

                // If nothing can be retrieved from the socket, nothing else can be done.
                if (checkReturnForDisconnect(amt, link)) {
                    return 0;
                }

                link->asyncTcpBytesRead = amt;

                // Did we get the full header?
                if(link->bytesRead + amt < PACKET_HEADER_SIZE){
                    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: Received partial header, %i bytes long, addr: %x, req loc: %s, %i\n", pak->UID, amt, pak, __FILE__, __LINE__);
                    return 0;
                }

                // Did we just retrieve the full header?
                // If so, go on to the next step.
                if(link->bytesRead + amt >= PACKET_HEADER_SIZE){
                    link->asyncTcpState = AST_ExtractHeaderComplete;
                    break;
                }
                break;

            case AST_ExtractHeaderComplete:
                // Packet header has been retrieved completely.
                // Retrieve the packet header.
                memcpy(&header, pak->stream.data, sizeof(PacketHeader));
                
                header.packetBitLength = endianSwapIfBig(U32,header.packetBitLength);
                header.packetChecksum = endianSwapIfBig(U32,header.packetChecksum);

                link->bytesToRead = ((header.packetBitLength + 7) >> 3);
                if (1 || link->decrypt_on)
                    link->bytesToRead = blowfishPad(link->bytesToRead);
                pak->stream.bitLength = header.packetBitLength;

                link->asyncTcpState = AST_ExtractBody;
                break;

            case AST_ExtractBody:
                // Discard packets that have too much data!
                if (link->bytesToRead > (int)pak->stream.maxSize) {
                    badPacketLog(BADPACKET_TOOBIG, link->addr.sin_addr.S_un.S_addr, link->addr.sin_port);
                    netDiscardDeadLink(link);
                    return 0;
                }
                if (link->bytesToRead < link->bytesRead) {
                    // The header tells us the packet is smaller than the header!  Someone's messing with us.
                    badPacketLog(BADPACKET_SIZEMISMATCH, link->addr.sin_addr.S_un.S_addr, link->addr.sin_port);
                    netDiscardDeadLink(link);
                    return 0;
                }

                // Attempt to read the body of the packet now.
                amt = recv(link->socket, pak->stream.data + link->bytesRead, link->bytesToRead - link->bytesRead,0);

                // Didn't get any data?
                // The packet is still only partially read.
                // Report nothing has been read yet.
                link->asyncTcpState = AST_ExtractBody;
                if (checkReturnForDisconnect(amt, link)) {
                    return 0;
                }

                // Did we get all the data for this packet?
                // If not, report nothing has been read yet.
                if (link->bytesRead + amt < link->bytesToRead){
                    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: Received partial TCP packet, %i bytes long, addr: %x, req loc: %s, %i\n", pak->UID, amt, pak, __FILE__, __LINE__);
                    return 0;
                }

                link->asyncTcpBytesRead = amt;
                link->asyncTcpState = AST_ExtractBodyComplete;
                break;

            case AST_ExtractBodyComplete:
                LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: Received complete TCP packet, %i bytes long, addr: %x, req loc: %s, %i\n", pak->UID, link->bytesRead, pak, __FILE__, __LINE__);

                // All data for this packet has been read.
                link->asyncTcpState = AST_None;

                
                // Prepare the packet for reading by moving the cursor beyond the header.
                pak->stream.cursor.byte = PACKET_HEADER_SIZE;
                *pakptr = link->pak;
                link->pak = NULL;
                pak->stream.size = link->bytesToRead;
                link->bytesRead = link->bytesToRead = 0;


                if (!pktDecryptAndCrc(pak,link,0,true))
                {
                    pktFree(pak);
                    pak = NULL;
                    *pakptr = NULL;

                    // Got a bad packet.
                    // Since we didn't really get a packet this time, try again.
                    link->asyncTcpState = AST_ExtractHeader;
                    break;
                }
                bsChangeMode(&pak->stream, Read);                        // Change the packet's bitstream to read mode.
                pak->stream.cursor.byte += PACKET_HEADER_SIZE;            // Point the stream to just after the packet header.

                // Report that one packet has been read.
                return 1;    
        }
    }
}

/* Function lnkReadTCPAsync
 *    This function can be used to initiate or continue an async read operation on a TCP link.
 *    
 *    FIXME!!!
 *        Generalize me into lnkReadAsync().
 *     
 */
void lnkReadTCPAsync(NetLink* link, int bytesExtracted){
    int amt;
    Packet* pak;

    // Only TCP links are sent to the completion port as links.
    assert(link->type == NLT_TCP);

    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0,"lnkReadTCPAsyncStart: extracted %i, req loc: %s, %i\n", bytesExtracted, __FILE__, __LINE__);
    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0,"Bytes extracted last time: %i, req loc: %s, %i\n", bytesExtracted, __FILE__, __LINE__);

    // If we just finished extracting data asyncronously, update the amount of data read and move
    // into the next packet extraction state state.
    if(bytesExtracted)
        link->asyncTcpBytesRead = bytesExtracted;

    // FIXME! This piece of code is unsafe and can break fairly easily.  If the sender is sending faster
    // than a link and can receive and process the packets, this logic will stop new async reads from being
    // queued and stop the read sequence on the link entirely.
    if(!qIsReallyFull(link->receiveQueue))
    {
        // Grab a TCP packet.
        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "Starting TCP read op, req loc: %s, %i\n", __FILE__, __LINE__);
        amt = pktGetTcpAsync(&pak, link);

        // If there wasn't anything to read, don't do anything.
        if (!amt){
            return;
        }
        
        link->lastRecvTime = timerCpuSeconds();

        // Put the packet into the receive queue of the link.
        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "current queue size is %i\n", qGetSize(link->receiveQueue));
        if(!lnkAddToReceiveQueue(link, &pak)){
            LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "could not add packet to receive queue");
            pktFree(pak);
            pak = NULL;
        }

        // If the queue isn't full yet, start the next async receive operation.
        if(!qIsReallyFull(link->receiveQueue))
            pktGetTcpAsync(&pak, link);

        // If the queue is full, do not start the next async receive operation.
        // There is no room for the packet to go if it is immediately available.
    }else{
        LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "Receive queue is full, req loc: %s, %i\n", __FILE__, __LINE__);
    }
}
