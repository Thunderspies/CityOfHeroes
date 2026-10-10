#define MEASURE_NETWORK_TRAFFIC_OPT_OUT
#include "utilitieslib/network/net_packet.h"
#include "utilitieslib/network/net_version.h"
#include "utilitieslib/network/net_masterlist.h"
#include "utilitieslib/network/netio_core.h"
#include "utilitieslib/network/netio_enet.h"
#include "utilitieslib/network/crypt.h"
#include "utilitieslib/network/sock.h"

#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/components/MemoryBank.h"
#include "utilitieslib/assert/assert.h"
#include <stdio.h>
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/log.h"

// Should packets include data type and length info?
//    This variable can be set through pktSendDebugInfo().
#ifdef FULLDEBUG
static int sendPacketDebugInfo = 1;
#else
static int sendPacketDebugInfo = 0;
#endif
static int printPacketSendData = 0;
static int printPacketGetData = 0;

#define PACKET_HEADER_SIZE    sizeof(PacketHeader)
/*****************************************************************************************
 * Packet memory management
 */
static int pktLargePacketAllocator(BitStream* bs);

static MemoryPool PacketMemoryPool = 0;
static MemoryPool BitStreamBufferMemoryPool = 0;
static MemoryBank LargeBitStreamBufferBank = 0;
static int packetAllocCount = 0;

// Indicates if the packet module has been started or not.
static int packetStarted;
static int packetSupportEncryption;

/**********************************************************
 * Packet size constants
 */
unsigned int sendPacketBufferSize = DEFAULT_SEND_PACKET_BUFFER_SIZE;                // Each packet will normally have a 2k buffer size.
//unsigned int maxPacketDataSize = DEFAULT_PACKET_BUFFER_SIZE - PACKET_MISCINFO_SIZE;    // Why - 200? Is this guestimation? or calculated?
/*
 * Packet size constants
 **********************************************************/

// ENHANCE ME!  Make memory pool size variable.
void packetStartup(int maxSendPacketSize, int init_encryption)
{
    netioInitCritical();
    if(packetStarted)
    {
        return;
    }
    else
        packetStarted = 1;

    sockStart();

    // How large can the packets be?
    if(maxSendPacketSize != 0){
        if(maxSendPacketSize < 50)
            maxSendPacketSize = 50; // Reasonable Minimum?
        if(maxSendPacketSize > DEFAULT_PACKET_BUFFER_SIZE)
            maxSendPacketSize = DEFAULT_PACKET_BUFFER_SIZE; // Can't be larger than this
        sendPacketBufferSize = maxSendPacketSize;
    }

    netEnetStartup(sendPacketBufferSize);

    BitStreamBufferMemoryPool = createMemoryPoolNamed("PacketBuffer", __FILE__, __LINE__);
    initMemoryPool(BitStreamBufferMemoryPool, DEFAULT_PACKET_BUFFER_SIZE, 32);    // allocate 32 of 2k memory chunks.
    mpSetMode(BitStreamBufferMemoryPool, TurnOffAllFeatures);            // Don't zero out memory during allocation.
    mpSetCompactParams(BitStreamBufferMemoryPool, 500, 0.5f);

    PacketMemoryPool = createMemoryPoolNamed("Packet", __FILE__, __LINE__);
    initMemoryPool(PacketMemoryPool, sizeof(Packet), 128);
    mpSetCompactParams(PacketMemoryPool, 500, 0.5f);

    LargeBitStreamBufferBank = createMemoryBank();
    initMemoryBank(LargeBitStreamBufferBank, 8);

    //PacketTimeoutGroupMemoryPool = createMemoryPool();
    //initMemoryPool(PacketTimeoutGroupMemoryPool, sizeof(PacketTimeoutGroup), 256);
    if (init_encryption) {
        cryptInit();
        packetSupportEncryption = 1;
    }
    cryptMD5Init();
    cryptAdler32Init();
}

void packetShutdown()
{
    netioInitCritical();
    netioEnterCritical();
    if(!packetStarted)
    {
        netioLeaveCritical();
        return;
    }
    else
        packetStarted = 0;

    destroyMemoryPool(BitStreamBufferMemoryPool);
    destroyMemoryPool(PacketMemoryPool);
    destroyMemoryBank(LargeBitStreamBufferBank);
    //destroyMemoryPool(PacketTimeoutGroupMemoryPool);
    netEnetShutdown();
    netioLeaveCritical();
}

int packetCanUseEncryption()
{
    return packetSupportEncryption;
}

void packetGetAllocationCounts(size_t* packetCount, size_t* bsCount)
{
    if(packetCount && PacketMemoryPool)
        *packetCount = mpGetAllocatedCount(PacketMemoryPool);
    else
        *packetCount = 0;
        
    if(bsCount && BitStreamBufferMemoryPool)
        *bsCount = mpGetAllocatedCount(BitStreamBufferMemoryPool);
    else
        *bsCount = 0;
}

//#define PACKET_SOURCE_DEBUGGING
#ifdef PACKET_SOURCE_DEBUGGING
#include "StashTable.h"
static int pak_count=0;
static StashTable htPackets=0;
#endif

Packet* pktCreateImp(char const* fname, int line){
    Packet* pak;
    unsigned char* buffer;

    // Assign the packet a new unique ID.
    netioEnterCritical();
    pak = mpAlloc(PacketMemoryPool);
    pak->UID = ++packetAllocCount;

    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: create, addr: %x, req loc: %s, %i\n", pak->UID, pak, fname, line);

#ifdef PACKET_SOURCE_DEBUGGING
    printf("Packet count : %5d\n", ++pak_count);
    if (!htPackets) {
        htPackets = stashTableCreateWithStringKeys(1024, StashDeepCopyKeys | StashCaseSensitive );
    }
    {
        char key[20];
        char value[260];
        sprintf_s(SAFESTR(key), "%d", pak);
        sprintf_s(SAFESTR(value), "%s (%d) id: %d", fname, line, pak->UID);
        //if (pak->UID==47)
            printf("%s\n", value);
        stashAddPointer(htPackets, key, strdup(value), false);
    }
#endif

    buffer = mpAlloc(BitStreamBufferMemoryPool);

    initBitStream(&pak->stream, buffer, (int)mpStructSize(BitStreamBufferMemoryPool), Write, true, pktLargePacketAllocator);
    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: Got BitStream buffer, addr: %x, req loc: %s, %i\n", pak->UID, pak->stream.data, fname, line);

    pak->hasDebugInfo = sendPacketDebugInfo;
    pak->reliable = 1; // default to on, since only a couple packet types will turn this off.
    applyNetworkVersionToPacket(NULL, pak);

    pak->creationTime = g_NMCachedTimeSeconds;
    pak->retransCount = 0;
    netioLeaveCritical();
    return pak;
}


/* Function pktFree()
 *    Frees all resources held by the given packet.  Note that this
 *    function does not free the packet itself.
 *
 *    The packet code is designed around the assumption that all packets
 *    will be allocated on the stack, and destroyed automatically.
 *
 */
void pktFreeImp(Packet* pak, char const* fname, int line){
    if(!pak)
        return;

    LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: free, addr: %x, req loc: %s, %i\n", pak->UID, pak, fname, line);

    pktDestroy(pak);
}


void pktDestroy(Packet* pak){

    netioEnterCritical();
    assert(!pak->inRetransmitQueue);
    assert(!pak->inSendQueue);
    //if we've got user data that can be destroyed, destroy it.
    if(pak->userData && pak->delUserDataCallback)
        pak->delUserDataCallback(pak);
    // Return the temporary stream buffer back to where it came from if any.
    if(pak->stream.data){
        // Did the memory come from the LargeBitStreamBufferBank because the packet
        // needed more memory than the standard memory pool could allocate?
        if(pak->stream.userData){
            LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: releasing packet buffer to memory bank, addr: %x\n", pak->UID, pak->stream.data);
            // If so, release the memory back to the memory bank.
            mbFree((MemoryLoan) pak->stream.userData);
        }else{
            LOG(LOG_NET, LOG_LEVEL_DEBUG, 0, "ID: %i, msg: releasing packet buffer to memory pool, addr: %x\n", pak->UID, pak->stream.data);
            // Otherwise, release the memory back to the memory pool.
            mpFree(BitStreamBufferMemoryPool, pak->stream.data);
        }
    }

#ifdef PACKET_SOURCE_DEBUGGING
    printf("Packet count : %5d\n", --pak_count);
    {
        char key[20];
        sprintf_s(SAFESTR(key), "%d", pak);
        stashRemovePointer(htPackets, key, NULL);
        // Print all outstanding packets
        if (pak_count<15) {
            StashElement element;
            StashTableIterator it;
            stashGetIterator(htPackets, &it);

            while(stashGetNextElement(&it, &element))
            {
                char *value = stashElementGetPointer(element);
                char *key = stashElementGetStringKey(element);
                Packet *pak = (Packet*)((U64)atoi(key));
                printf("\t%d: %s\n", pak->id, value);
            }
        }
    }
#endif
    // Return the packet to its memory pool also.
    mpFree(PacketMemoryPool, pak);
    netioLeaveCritical();
}


/* Function pktLargePacketAllocator()
 *    This function is a callback for the bitstream code in case the initial bitstream buffer is
 *    not large enough.  Currently, this only happens when the amount of data being placed in
 *    the packet is greater than "sentPacketBufferSize".
 *
 *    The initial bitstream buffer for packets are allocated out of a memory pool which can only
 *    deal with fixed size allocations.  To accomandate large packets, this function allocates
 *    buffer from the "LargeBitStreamBufferBank".  Note that the bank has a limit on the number of
 *    loans it is capable of controlling.  If the current limit is too small, the bank can be enlarged
 *    in packetStartup().  Note that it is a good idea to keep the bank to a small size.  MemoryBanks
 *    are not meant to be used as a generic memory allocation mechanism.
 *
 */
static int pktLargePacketAllocator(BitStream* bs){
    MemoryLoan loan;

    // Try to take out a memory loan that is twice as large as the current bitstream buffer.
    if(bs->userData){
        // If the bitstream has already taken out a memory loan before...

        int result;

        // Alter the existing loan.
        loan = (MemoryLoan)bs->userData;
        result = mbRealloc(loan, bs->maxSize * 2);

        // FIXME!!
        // If the allocation didn't succeed, the memory bank is not large enough.  There is currently
        // no way of handling this condition.
        assert(result);

        bs->data = mbGetMemory(loan);
    }else{
        // If the bitstream is taking out a memory loan for the first time...
        loan = mbAlloc(LargeBitStreamBufferBank, bs->maxSize * 2);

        // Record the memory loan with the bitstream.  This loan needs to be returned when the bitstream
        // is destroyed.
        bs->userData = (void*)loan;


        // FIXME!!
        assert(loan);

        // There is enough memory for the bitstream to proceed now.
        // Copy the contents of the original memory over.
        memcpy(mbGetMemory(loan), bs->data, bs->maxSize);

        // Release the original buffer back into the memory pool.
        mpFree(BitStreamBufferMemoryPool, bs->data);

        // Give the new memory to the bitstream.
        bs->data = mbGetMemory(loan);
    }

    // Update the bitstream maxSize to its new value.
    bs->maxSize = bs->maxSize * 2;
    return 1;
}
/*
 * Packet memory management
 *****************************************************************************************/

/*****************************************************************************************
 * Basic packet operations
 */

void pktSetDebugInfoTo(int on)
{
    sendPacketDebugInfo = on;
}

void pktSetDebugInfo(){
    sendPacketDebugInfo = 1;
}



int pktGetDebugInfo(){
    return sendPacketDebugInfo;
}

void pktSetByteAlignment(Packet* pak, int align)
{
    bsSetByteAlignment(&pak->stream, align);
}

int pktGetByteAlignment(Packet* pak)
{
    return pak->stream.byteAlignedMode;
}

void pktSetCompression(Packet* pak, int compression)
{
    // can't change the compression once data has been added to
    // the packet. this is because the packet command is read
    // as 32 bits in length if compression is not set.
    if(devassert( pak->stream.bitLength == 0 ))
    {
        pktSetByteAlignment(pak, compression);
        pak->compress = compression?1:0;
    }
}

int pktGetCompression(Packet* pak) {
    return pak->compress;
}

void pktSetOrdered(Packet* pak, bool ordered)
{
    pak->ordered = ordered;
    if(ordered)
        pak->reliable = 1;
}

bool pktGetOrdered(Packet* pak) {
    return pak->ordered;
}

void pktSendBits(Packet *pak,int numbits,U32 val){
    if(printPacketSendData)
        printf("sending %d bits: %d\n", numbits, val & ((1 << numbits) - 1));

    if(pak->hasDebugInfo)
        bsTypedWriteBits(&pak->stream, numbits, val);
    else
        bsWriteBits(&pak->stream, numbits, val);
}

U32 pktGetBits(Packet *pak,int numbits){
    if(printPacketGetData)
        printf("getting %d bits\n", numbits);

    if(pak->hasDebugInfo)
        return bsTypedReadBits(&pak->stream, numbits);
    else
        return bsReadBits(&pak->stream, numbits);
}

void pktSendBitsArray(Packet *pak,int numbits,const void *data){
    if(printPacketSendData)
        printf("sending %d bits array\n", numbits);

    if(pak->hasDebugInfo)
        bsTypedWriteBitsArray(&pak->stream, numbits, data);
    else
        bsWriteBitsArray(&pak->stream, numbits, data);
}

void pktGetBitsArray(Packet *pak,int numbits,void *data){
    if(printPacketGetData)
        printf("getting %d bits array\n", numbits);

    if(pak->hasDebugInfo)
        bsTypedReadBitsArray(&pak->stream, numbits, data);
    else
        bsReadBitsArray(&pak->stream, numbits, data);
}

void pktSendBitsPack(Packet *pak,int minbits,U32 val){
    if(printPacketSendData)
        printf("sending %d bits pack: %d\n", minbits, val);

    if(pak->hasDebugInfo)
        bsTypedWriteBitsPack(&pak->stream, minbits, val);
    else
        bsWriteBitsPack(&pak->stream, minbits, val);
}

int pktGetLengthBitsPack(Packet *pak,int minbits,U32 val){
    if(pak->hasDebugInfo)
        return bsGetLengthTypedWriteBitsPack(&pak->stream, minbits, val);
    else
        return bsGetLengthWriteBitsPack(&pak->stream, minbits, val);
}

U32 pktGetBitsPack(Packet *pak,int minbits){
    if(printPacketGetData)
        printf("getting %d bits pack\n", minbits);

    if(pak->hasDebugInfo)
        return bsTypedReadBitsPack(&pak->stream, minbits);
    else
        return bsReadBitsPack(&pak->stream, minbits);
}

void pktSendString(Packet *pak,const char *str){
    if (!str)
        str = "";

    if(printPacketSendData)
        printf("sending string: \"%s\"\n", str);

    if(pak->hasDebugInfo)
        bsTypedWriteString(&pak->stream, str);
    else
        bsWriteString(&pak->stream, str);
}

void pktSendStringAligned(Packet *pak, const char *str)
{
    if(!str)
        str = "";

    if(printPacketSendData)
        printf("sending aligned string: \"%s\"\n", str);

    if(pak->hasDebugInfo)
        bsTypedWriteStringAligned(&pak->stream, str);
    else
        bsWriteStringAligned(&pak->stream, str);
}

char *pktGetStringAndLength(Packet *pak, int *pLen){
    if(printPacketGetData)
        printf("getting string\n");

    if(pak->hasDebugInfo)
        return bsTypedReadStringAndLength(&pak->stream, pLen);
    else
        return bsReadStringAndLength(&pak->stream, pLen);
}

char* pktGetStringTemp(Packet *pak)
{
    if(printPacketGetData)
        printf("getting string\n");

    if(pak->hasDebugInfo)
        return bsTypedReadStringAligned(&pak->stream);
    else
        return bsReadStringAligned(&pak->stream);
}

void pktSendF32(Packet *pak,float f){
    if(printPacketSendData)
        printf("sending F32: %f\n", f);

    if(pak->hasDebugInfo)
        bsTypedWriteF32(&pak->stream, f);
    else
        bsWriteF32(&pak->stream, f);
}

F32    pktGetF32(Packet *pak){
    if(printPacketGetData)
        printf("getting F32\n");

    if(pak->hasDebugInfo)
        return bsTypedReadF32(&pak->stream);
    else
        return bsReadF32(&pak->stream);
}

void pktAlignBitsArray(Packet *pak){
    if(pak->hasDebugInfo)
        bsTypedAlignBitsArray(&pak->stream);
    else
        bsAlignByte(&pak->stream);
}

void pktAppend(Packet* pak, Packet* data)
{
    int bitsToSend = bsGetBitLength(&data->stream);
    int dataIndex = 0;

    // Can't append two different packet types!
    assert(pak->compress == data->compress && pak->stream.byteAlignedMode==data->stream.byteAlignedMode);
    assert(pak->hasDebugInfo == data->hasDebugInfo);

    // This isn't endian safe
#ifndef _XBOX
    while(bitsToSend >= 32)
    {
        bsWriteBits(&pak->stream, 32, *(U32*)&data->stream.data[dataIndex]);
        bitsToSend -= 32;
        dataIndex+=4;
    }
#endif
    // We can't just cast the last bytes to a U32 because it's possible the last 2 bytes hang into memory
    // that we don't own =(  So, byte-by-byte it is...
    while(bitsToSend >= 8)
    {
        bsWriteBits(&pak->stream, 8, data->stream.data[dataIndex]);
        bitsToSend -= 8;
        dataIndex++;
    }

    if (bitsToSend)
        bsWriteBits(&pak->stream, bitsToSend, data->stream.data[dataIndex]);
}

void pktAppendRemainder(Packet* pak, Packet* data)
{
    BitStream* from = &data->stream;
    int bitsToSend = from->bitLength - (from->cursor.byte << 3) - from->cursor.bit;

    while (bitsToSend > 0)
    {
        int now = bitsToSend > 32? 32: bitsToSend;
        bsWriteBits(&pak->stream, now, bsReadBits(&data->stream, now));
        bitsToSend -= now;
    }
}

U32 pktGetSize(Packet *pak){
    assert(pak);
    return bsGetLength(&pak->stream);
}

U32 pktEnd(Packet *pak){
    return bsEnded(&pak->stream);
}

void pktReset(Packet *pak){
    pak->stream.cursor.bit = 0;
    pak->stream.cursor.byte = PACKET_HEADER_SIZE;
}
/*
 * Basic packet operations
 *****************************************************************************************/

void pktMakeCompatible(Packet *pak, Packet const *pak_in)
{
    pak->reliable = pak_in->reliable;
    pak->hasDebugInfo = pak_in->hasDebugInfo;
    pak->compress = pak_in->compress;
    pak->ordered = pak_in->ordered;
}
