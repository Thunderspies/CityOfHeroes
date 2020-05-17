/* File BitStream.c
 *
 * FIXME!!!
 *    Stop tracking bitstream size and bitlength independently of the cursor.
 *    This is error-prone.
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utilitieslib/components/BitStream.h"
#include "utilitieslib/stdtypes.h"
#include "zlib/zlib.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/mathutil.h"

#define ROUND_BITS_UP(x) ((x + 7) & ~7)

int g_bAssertOnBitStreamError = false;
// Assert macro to use in the case where asserts are handly gracefully
#define BSASSERT(exp) assert(!g_bAssertOnBitStreamError || (exp))

void bsAssertOnErrors(int allow) {
    g_bAssertOnBitStreamError = allow?true:false;
}

int bsIsBad(BitStream *bs) {
    return bs->errorFlags;
}


void initBitStream(BitStream* bs, unsigned char* buffer, unsigned int bufferSize, BitStreamMode initMode, unsigned int byteAligned, MemoryAllocator alloc){
    memset(bs, 0, sizeof(BitStream));
    bs->data = buffer;
    bs->maxSize = bufferSize;
    bs->memAllocator = alloc;
    bs->mode = initMode;
    //memset(bs->data, 0, bufferSize);
    bs->data[0] = 0;
    bs->data[1] = 0;
    bs->data[2] = 0;
    bs->data[3] = 0;
    bs->byteAlignedMode=byteAligned;
}

void cloneBitStream(BitStream* orig, BitStream* clone){
    memcpy(clone, orig, sizeof(BitStream));
}

void bsSetByteAlignment(BitStream* bs, int align) {
    bs->byteAlignedMode = align;
    if (align)
        bsAlignByte(bs);
}

int bsGetByteAlignment(BitStream* bs) {
    return bs->byteAlignedMode;
}


void bsWriteBits(BitStream* bs, int numbits, unsigned int val){

    if (bs->errorFlags) {
        // Can't write to a stream in an erroneous state!
        return;
    }

    // Make sure the given bitstream is in write mode.
    if(bs->mode != Write){
        assert(0);
        return;
    }

    assert(numbits<=32);

    if (numbits == 0)
        return;

    if (bs->byteAlignedMode) {
        if (numbits!=32) { // Mask out bits that we don't want sent
            int mask = ((1 << numbits)-1);
            val = val & mask;
        }
        numbits = ROUND_BITS_UP(numbits);
        assert(bs->cursor.bit==0);

    }

    // See if we need to increase the max size
    {
        unsigned int lastByteModified = bs->cursor.byte + ROUND_BITS_UP(bs->cursor.bit + numbits);
        if (lastByteModified > bs->maxSize)
        {
            if (bs->memAllocator)
            {
                bs->memAllocator(bs);
                assert(lastByteModified < bs->maxSize);
            }
            else
            {
                // Apparently this should never happen
                BSASSERT(!"No memory allocator and we need more");
                bs->errorFlags = BSE_OVERFLOW;
                return;
            }
        }
    }

#if 1
    // Setup and do 32-bit copies
    {
        char *cursor_byte = bs->data + bs->cursor.byte;
        U32 *cursor_U32 = (U32*)((size_t)cursor_byte & ~3);
        U32 cursor_U32_bit = bs->cursor.bit + 8 * ((size_t)cursor_byte & 3);
        U32 old_mask = (1 << cursor_U32_bit)-1;

        // Do shifted 32-bit masked copy
        if (cursor_U32_bit > 0)
        {
            int max_bits_to_copy = 32 - cursor_U32_bit;

            int bits_to_copy = (numbits < max_bits_to_copy) ? numbits : max_bits_to_copy;

            *cursor_U32 = (*cursor_U32 & old_mask) | ((val & ((1 << bits_to_copy)-1)) << cursor_U32_bit);

            val >>= bits_to_copy;
            cursor_U32_bit += bits_to_copy;

            bs->cursor.byte = (unsigned int)(((size_t)cursor_U32 + (cursor_U32_bit >> 3)) - (size_t)bs->data);
            bs->cursor.bit = cursor_U32_bit & 7;

            numbits -= bits_to_copy;

            cursor_U32 += cursor_U32_bit >> 5;
            cursor_U32_bit &= 31;
            old_mask = (1 << cursor_U32_bit)-1;
        }

        // Do 32-bit copy
        if (numbits == 32)
        {
            assert(bs->cursor.bit == 0);
            assert(cursor_U32_bit == 0);

            *cursor_U32 = val;
            bs->cursor.byte += 4;
            numbits = 0;
        }
        // Do 32-bit masked copy
        else if (numbits)
        {
            int bits_to_copy = numbits;

            assert(bs->cursor.bit == 0);
            assert(cursor_U32_bit == 0);

            // NOTE: 1 << bits_to_copy will overfloat the 32-bit integer when bits_to_copy is 32.
            // That's why we have the "Do 32-bit copy" section above
            *cursor_U32 = (val & ((1 << bits_to_copy)-1));

            cursor_U32_bit += bits_to_copy;

            bs->cursor.byte = (unsigned int)(((size_t)cursor_U32 + (cursor_U32_bit >> 3)) - (size_t)bs->data);
            bs->cursor.bit = cursor_U32_bit & 7;

            numbits -= bits_to_copy;
        }
    }
#else
    // OLD 8-bit logic (cleaned up) for reference

    // OPTIMIZATION: Fast pass for doing byte copies
    if (bs->cursor.bit == 0)
    {
        while(numbits >= 8)
        {
            bs->data[bs->cursor.byte] = val & 0xFF;

            val >>= 8;
            bs->cursor.byte++;
            numbits -= 8;
        }
    }

    // Insert the bits now.
    while(numbits)
    {
        int outBits, bits;

        if (bs->cursor.bit == 0)
            bs->data[bs->cursor.byte] = 0;

        // Calculate the number of bits to be outputted this iteration.
        //    How many bits until the current byte is filled?
        outBits = 8 - bs->cursor.bit;

        //    Is it possible to send all remaining bits?  Or do we have to settle for filling this
        //    byte only?
        bits = (numbits < outBits) ? numbits : outBits;

        // Mask out data that is not wanted, then put it at the correct bit location
        // as specified by the cursor.
        bs->data[bs->cursor.byte] |= (val & ((1 << bits)-1)) << bs->cursor.bit;

        val >>= bits;
        bs->cursor.bit += bits;

        // Normalize cursor.
        bs->cursor.byte += bs->cursor.bit >> 3;
        bs->cursor.bit &= 7;

        numbits -= bits;
    }
#endif

    // The other bsWrite functions assume that the next byte is zeroed out
    if (bs->cursor.bit == 0)
        bs->data[bs->cursor.byte] = 0;

    // Only increase size if cursor is beyond it.
    // We don't want to increase the size when we're overwriting data in the middle!
    bs->size = MAX(bs->size, bs->cursor.byte);

    assert(bs->size <= bs->maxSize);

    // Update the recorded bitlength of the stream.
    {
        unsigned int cursorBitPos;
        cursorBitPos = bsGetCursorBitPosition(bs);
        bs->bitLength = MAX(bs->bitLength, cursorBitPos);
    }

}

int bsGetLengthWriteBits(BitStream* bs, int numbits, unsigned int val){
    int    outBits, bits;
    int ret=0;

    if (bs->errorFlags) {
        // Can't write to a stream in an erroneous state!
        return 0;
    }

    if (bs->byteAlignedMode) {
        if (numbits!=32) { // Mask out bits that we don't want sent
            int mask = ((1 << numbits)-1);
            val = val & mask;
        }
        numbits = ROUND_BITS_UP(numbits);
        assert(bs->cursor.bit==0);
    }
    assert(numbits<=32);

    // Make sure the given bitstream is in write mode.
    if(bs->mode != Write){
        assert(0);
        return 0;
    }

    // Insert the bits now.
    while(1){
        // Calculate the number of bits to be outputted this iteration.
        //    How many bits until the current byte is filled?
        outBits = 8 - bs->cursor.bit;

        //    Is it possible to send all remaining bits?  Or do we have to settle for filling this
        //    byte only?
        bits = (numbits < outBits) ? numbits : outBits;

        // Mask out data that is not wanted, then put it at the correct bit location
        // as specified by the cursor.
        //NOT HERE bs->data[bs->cursor.byte] |= (val & ((1 << bits)-1)) << bs->cursor.bit;

        val >>= bits;
        //NOT HERE bs->cursor.bit += bits;
        ret += bits;

        //NOT HERE Normalize cursor.
        numbits -= bits;
        if (numbits <= 0)
            break;
    }

    //NOT HERE Update the recorded bitlength of the stream.
    return ret;
}

unsigned int bsReadBits(BitStream* bs, int numbits){
    unsigned int        val = 0;
    int        in_bits, curr_shift=0, bits;

    if (bs->errorFlags) {
        return -1;
    }

    assert(numbits <= 32);

    if (bs->byteAlignedMode) {
        numbits = ROUND_BITS_UP(numbits);
    }

    if ((bs->cursor.byte<<3) + bs->cursor.bit + numbits > bs->bitLength) {
        BSASSERT(!"Read off of the end of the bitstream!");
        bs->errorFlags = BSE_OVERRUN;
        return -1;
    }

    while(1)
    {
        in_bits = 8 - bs->cursor.bit;
        bits = (numbits < in_bits) ? numbits : in_bits;
        val |= ((bs->data[bs->cursor.byte] >> bs->cursor.bit) & ((1 << bits)-1)) << curr_shift;
        bs->cursor.bit += bits;
        if (bs->cursor.bit >= 8)
        {
            bs->cursor.bit = 0;
            bs->cursor.byte++;
        }
        curr_shift += bits;
        numbits -= bits;
        if (numbits <= 0)
            break;
    }
    return val;
}


int bsWriteBitsArray(BitStream* bs, int numbits, const void* data){
    int    fullBytes;
    //int bytesAdded;
    //int bitsAdded;

    // Note: this always aligns to bytes currently, if that changes, we'll need to add a case for byteAligned here

    // Make sure the given bitstream is in write mode.
    if(bs->mode != Write){
        assert(0);
        return 0;
    }

    if (bs->errorFlags) {
        return 0;
    }

    // How many full bytes are being sent?
    fullBytes = (numbits + 7) >> 3;

    // Align the bitstream cursor with the next byte.
    // This prepares the bitstream for a direct memory copy, avoiding
    // any bit alignment issues.
    if(bs->cursor.bit){
        bs->bitLength += 8 - bs->cursor.bit;
        bs->cursor.bit = 0;
        bs->cursor.byte++;
        if (bs->size<bs->cursor.byte)  // Only increase size if cursor matches it, we don't want to increase the size when we're overwriting data in the middle!
            bs->size++;
        assert(bs->size>=bs->cursor.byte);
    }

    // Make sure there is enough room in the bitstream buffer to hold the
    // incoming data.
    while((bs->cursor.byte + fullBytes) >= bs->maxSize){
        if(bs->memAllocator){
            bs->memAllocator(bs);
        }else {
            BSASSERT(!"No memory allocator and we need more");
            bs->errorFlags = BSE_OVERFLOW;
            return 0;
        }
    }

    // Copy the given data into the bitstream.
    memcpy(&bs->data[bs->cursor.byte], data, fullBytes);

    // Update bitstream cursor.
    bs->size += fullBytes;
    bs->cursor.byte += fullBytes;
    bs->bitLength += numbits;
    //JE: Note that right here cursor.byte and bitLength and inconsistent with eachother!  Unfortunately,
    //  that's the way it was, and that's the way it's gotta be for now, since we often "duplicate" one
    //  stream into another by using this function, and we expect the bitLength of the two to be the
    //  same!  But, if anything gets appended to this, we would want it to start at the next byte (although
    //  I don't think that happens)

    bs->data[bs->cursor.byte] = 0;
    return 1;
}

int bsReadBitsArray(BitStream* bs, int numbits, void* data){
    int fullBytes;

    // Note: this always aligns to bytes currently, if that changes, we'll need to add a case for byteAligned here

    // How many full bytes are being retrieved?
    fullBytes = (numbits + 7)/8;

    if(bs->cursor.bit){ // align to bye
        bs->cursor.bit = 0;
        bs->cursor.byte++;
    }
    if ((bs->cursor.byte<<3) + bs->cursor.bit + numbits > bs->bitLength) {
        BSASSERT(!"Read off of the end of the bitstream!");
        bs->errorFlags = BSE_OVERRUN;
        return 0;
    }

    memcpy(data, &bs->data[bs->cursor.byte], fullBytes);
    bs->cursor.byte += fullBytes;

    return 1;
}

// measure how the packed bits stuff is doing
unsigned int g_packetsizes_one[33];
unsigned int g_packetsizes_success[33];
unsigned int g_packetsizes_failed[33];

void bsWriteBitsPack(BitStream* bs, int minbits, unsigned int val){
    unsigned int bitmask;
    int success = 1;
    int one = minbits == 1;
    int measured_bits = count_bits(val);

    if (bs->byteAlignedMode) {
        bsWriteBits(bs, 32, val);
        return;
    }
    if (bs->errorFlags) {
        return;
    }

    for(;;)
    {
        // Produce a minbits long mask that contains all 1's
        bitmask = (1 << minbits)-1;

        // If the value to be written can be represented by minbits...
        if (val < bitmask || minbits >= 32)
        {
            // Write the value.
            bsWriteBits(bs,minbits,val);
            if (success)
                g_packetsizes_success[measured_bits]++;
            else
                g_packetsizes_failed[measured_bits]++;
            if (one)
                g_packetsizes_one[measured_bits]++;
            break;
        }
        bsWriteBits(bs,minbits,bitmask);
        minbits <<= 1;
        if (minbits > 32)
            minbits = 32;
        val -= bitmask;
        success = 0;
    }
}

int bsGetLengthWriteBitsPack(BitStream* bs, int minbits, unsigned int val){
    unsigned int bitmask;
    int ret=0;

    if (bs->byteAlignedMode) {
        return bsGetLengthWriteBits(bs, 32, val);
    }
    if (bs->errorFlags) {
        return 0;
    }

    for(;;)
    {
        // Produce a minbits long mask that contains all 1's
        bitmask = (1 << minbits)-1;

        // If the value to be written can be represented by minbits...
        if (val < bitmask || minbits >= 32)
        {
            // Write the value.
            ret += bsGetLengthWriteBits(bs,minbits,val);
            break;
        }
        ret += bsGetLengthWriteBits(bs,minbits,bitmask);
        minbits <<= 1;
        if (minbits > 32)
            minbits = 32;
        val -= bitmask;
    }
    return ret;
}


unsigned int bsReadBitsPack(BitStream* bs,int minbits){
    unsigned int val, base=0;
    unsigned int bitmask;
    int success = 1;
    int one = minbits == 1;

    if (bs->byteAlignedMode) {
        return bsReadBits(bs, 32);
    }

    if (bs->errorFlags) {
        return -1;
    }

    for(;;)
    {
        val = bsReadBits(bs,minbits);
        if (bs->errorFlags)
            return -1;
        bitmask = (1 << minbits)-1;
        if (val < bitmask || minbits == 32)
        {
            int measured_bits = count_bits(val+base);
            if (success)
                g_packetsizes_success[measured_bits]++;
            else
                g_packetsizes_failed[measured_bits]++;
            if (one)
                g_packetsizes_one[measured_bits]++;
            return val + base;
        }
        base += bitmask;
        minbits <<= 1;
        if (minbits > 32)
            minbits = 32;
        success = 0;
    }
    return val + base;
}

void bsWriteString(BitStream* bs, const char* str){
    U8        *data,c;
    U32        shift,len=0,maxsize,orig_size;

    if (bs->byteAlignedMode) {
        assert(bs->cursor.bit==0);
    }

    shift = bs->cursor.bit;
    data = bs->data + bs->cursor.byte;
    orig_size = bs->size;
    maxsize = bs->maxSize - orig_size;
    do
    {
        if(++len >= maxsize)
        {
            if(bs->memAllocator){
                bs->memAllocator(bs);
            }else {
                BSASSERT(!"No memory allocator and we need more");
                bs->errorFlags = BSE_OVERFLOW;
                return;
            }
            maxsize = bs->maxSize - orig_size;
            data = bs->data + bs->cursor.byte;// + len - 1;
        }
        c = str[len-1];
        data[len-1] |= c << shift;
        data[len] = c >> (8-shift);
    } while (c);
    bs->size = len + orig_size;
    bs->cursor.byte += len;
    bs->bitLength = bsGetCursorBitPosition(bs);
}

void bsWriteStringAligned(BitStream *bs, const char *str)
{
    bsAlignByte(bs);
    bsWriteString(bs, str);
}

char* bsReadStringAndLength(BitStream* bs, int *pLen){
    THREADSAFE_STATIC char *buf = NULL;
    THREADSAFE_STATIC int buf_size = 0;
    static char *null_string = "(null)";
    int            i,shift = bs->cursor.bit;
    U8            *data;

    THREADSAFE_STATIC_MARK(buf);

    if ((bs->cursor.byte << 3) + bs->cursor.bit > (bs->bitLength)) {
        BSASSERT(!"Read off of the end of the bitstream!");
        bs->errorFlags = BSE_OVERRUN;
        return null_string;
    }

    data = bs->data + bs->cursor.byte;
    for(i=0;;i++)
    {
        if (i >= buf_size)
        {
            if (!buf_size)
                buf_size = 1000;
            buf_size <<= 1;
            buf = realloc(buf,buf_size);
        }
        buf[i] = *data++ >> shift;
        buf[i] |= *data << (8 - shift);
        if (! buf[i])
            break;
        if ((bs->cursor.byte << 3) + bs->cursor.bit + (i<<3) > (bs->bitLength)) {
            BSASSERT(!"Read off of the end of the bitstream!");
            bs->errorFlags = BSE_OVERRUN;
            return null_string;
        }
    }
    bs->cursor.byte += i+1;
    *pLen = i;
    return buf;
}

char* bsReadStringAligned(BitStream *bs)
{
    static char *null_string = "(null)";
    char *str;

    bsAlignByte(bs);
    str = bs->data + bs->cursor.byte;

    if(bs->cursor.bit)
    {
        BSASSERT(!"Reading non-aligned bytes");
        bs->errorFlags = BSE_TYPEMISMATCH;
        return null_string;
    }

    for(;;)
    {
        if((bs->cursor.byte << 3) > bs->bitLength)
        {
            BSASSERT(!"Read off of the end of the bitstream!");
            bs->errorFlags = BSE_OVERRUN;
            return null_string;
        }
        if(!bs->data[bs->cursor.byte++])
            break;
    }

    return str;
}

void bsWriteF32(BitStream* bs, float f){
    bsWriteBits(bs,32,* ((int *)&f));
}

float bsReadF32(BitStream* bs){
    unsigned int val;

    val = bsReadBits(bs,32);
    return *((float *) &val);
}

void bsAlignByte(BitStream* bs){
    if(bs->cursor.bit){
        bs->cursor.byte++;
        if (bs->mode == Write) {
            if (bs->size < bs->cursor.byte) {  // Only increase size if cursor matches it, we don't want to increase the size when we're overwriting data in the middle!
                bs->size++;
                bs->bitLength += 8 - bs->cursor.bit;
                bs->data[bs->size]=0; // Because the data might not be zeroed out to start with?
            }
        }
        bs->cursor.bit = 0;
    }
}

void bsRewind(BitStream* bs){
    bs->cursor.byte = 0;
    bs->cursor.bit = 0;
    bs->errorFlags = BSE_NOERROR;
}

void bsChangeMode(BitStream* bs, BitStreamMode newMode){
    if(newMode == bs->mode)
        return;

    bsRewind(bs);
    bs->mode = newMode;
}

/* Function bsGetBitLength()
 *    Grabs the total number of bits currently held in the stream.
 *
 *    BitStream::bitLength is used to explicitly track the number of
 *    bits in the bitstream.  This is required because there would be no
 *    other way to determine where the data really ends in the bitstream.
 *    bsEnded() needs this value to determine if the end of the stream
 *    has been reached.
 */
unsigned int bsGetBitLength(BitStream* bs){
    return bs->bitLength;
}

unsigned int bsGetLength(BitStream* bs){
    return ((bs->cursor.byte << 3) + bs->cursor.bit) >> 3;
}

unsigned int bsEnded(BitStream* bs){
    // When a bitstream is being written, testing for end-of-stream does not work.
    //JE: Removed this, pktEnd call in pktWrap needs to know if it's ended, if this breaks
    // something else we could put something special in pktWrap
    //if(Write == bs->mode)
    //    return 0;

    if (bs->errorFlags & BSE_OVERRUN)
        return 1;

    return (bsGetCursorBitPosition(bs) >= bsGetBitLength(bs));
}

unsigned int bsGetCursorBitPosition(BitStream* bs){
    return (bs->cursor.byte << 3) + bs->cursor.bit;
}

void bsSetCursorBitPosition(BitStream* bs, unsigned int position){
    assert (position <= bs->bitLength);
    bs->cursor.byte = position >> 3;
    bs->cursor.bit = position & 7;
}

void bsSetEndOfStreamPosition(BitStream *bs, unsigned int position){
    bsSetCursorBitPosition(bs, position);
    bs->size = bs->cursor.byte;
    bs->bitLength = bsGetCursorBitPosition(bs);
    // Write zeroes to the rest of the byte we're at
    bs->data[bs->cursor.byte] &= (1 << bs->cursor.bit)-1;
}

static char *debugBytesToString(char *buf, int size, int max) {
    int numbytes = min(size, max);
    int i;
    static char buff[256];
    buff[0]=0;
    for (i=0; i<numbytes; i++) {
        strcatf(buff, "%02x", (unsigned char)buf[i]);
        if (i%4==3)
            strcat(buff, " ");
    }
    return buff;
}

#define SMALLEST_DATA_TO_COMPRESS 12 // in bytes
// This value shouldn't be more than 13, since gzip has a 12 byte header, on
//  the receiving end, anything 13bytes or more is implied to have been compressed
// If this was, for example, 16, there would be no way of autmatically detecting
//  whether a 14 byte incoming packet is a 14 byte packet, or a bigger packet compressed
//  down to 14 bytes.  The *actual* range for the smallest gzipped packet is probably more
//  than 13 bytes, but I don't know.
// Also, on the receiving end, the BitsPack containing the length of the zipped
//  data is added to this, so perhaps 14 would be fine for a value too


// Compresses the entire stream
// Format of data afterwards: (the first two might be typedBitsPack instead)
//    BitsPack,1    - the first BitsPack in the input stream (not compressed)
// Then if the remaining data is >= SMALLEST_DATA_TO_COMPRESS
//    BitsPack,8    - the length of the unpacked data
//    BitsArray - gziped data
// Otherwise, just raw data, uncompressed
void bsCompress(BitStream *bs, int useTypedBits, void *zstream_void) {
    char temp_buffer[100000];
    char *dest;
    unsigned long byteLength;
    unsigned long destLen;
    int ret;
    int firstBitsPack;
    char buf[256];
    z_stream    *z = zstream_void;

    assert(bs->byteAlignedMode);

    // Get first BitsPack
    bsRewind(bs);
    if (useTypedBits) {
        firstBitsPack = bsTypedReadBitsPack(bs, 1);
    } else {
        firstBitsPack = bsReadBitsPack(bs, 1);
    }

    // Find the size of the data to be compressed
    byteLength = (bs->bitLength >> 3) - bs->cursor.byte;

    strcpy(buf, debugBytesToString(bs->data+bs->cursor.byte, byteLength, 8));

    if (byteLength < SMALLEST_DATA_TO_COMPRESS) {
        // a) This packet cannot get any smaller
        // b) We may need this packet uncompressed (see not above)
        // Just leave as is!
        //printf("Refusing to compress bitStream, CMD: %d, data size: %d first 8 bytes : '%s'\n", firstBitsPack, byteLength, buf);
        bsRewind(bs);
        return;
    }

    assert((bs->bitLength & 7) ==0);

    destLen = (unsigned long)(byteLength*1.0125+12); // 1% + 12 bigger, so says the zlib docs
    if(destLen <= sizeof(temp_buffer))
        dest = temp_buffer;
    else
        dest = calloc(destLen, 1);

    PERFINFO_AUTO_START("compress", 1);
    if (z)
    {
        z->avail_out = destLen;
        z->next_out = dest;
        z->avail_in = byteLength;
        z->next_in = bs->data+bs->cursor.byte;
        ret = deflate(z,Z_SYNC_FLUSH);
        destLen -= z->avail_out;
    }
    else
        ret = compress(dest, &destLen, bs->data+bs->cursor.byte, byteLength);
    PERFINFO_AUTO_STOP();

    if (ret != Z_OK && ret != Z_STREAM_END) {
        if (!g_bAssertOnBitStreamError) {
            bs->errorFlags = BSE_COMPRESSION;
            if(dest != temp_buffer)
                free(dest);
            return;
        }
        switch (ret) {
        case Z_MEM_ERROR:
            FatalErrorf("Error compressing BitStream data : Memory error");
            break;
        case Z_BUF_ERROR:
            FatalErrorf("Error compressing BitStream data : Not a big enough buffer");
            break;
        default:
            FatalErrorf("Error compressing BitStream data : Unknown error");
            break;
        }
    }

//    if (destLen > byteLength && byteLength > 16) {
//        printf("Warning: compressed packet bigger than original (%d > %d)\n", destLen, byteLength);
//    }

    // Copy to destination
    bs->size = bs->bitLength = 0;
    // Clear the current bitStream
    memset(bs->data, 0, bs->maxSize);
    // Place header and size of decompressed data in stream
    bsRewind(bs);
    if (useTypedBits) {
        bsTypedWriteBitsPack(bs, 1, firstBitsPack);
        bsTypedWriteBitsPack(bs, 8, byteLength); // Write size of uncompressed data
        bsTypedWriteBitsPack(bs, 8, destLen); // Also write number of bytes that were encoded
        bsWriteBitsArray(bs, destLen*8, dest);
    } else {
        bsWriteBitsPack(bs, 1, firstBitsPack);
        bsWriteBitsPack(bs, 8, byteLength); // Write size of uncompressed data
        bsWriteBitsArray(bs, destLen*8, dest);
    }
    if(dest != temp_buffer)
        free(dest);

    //printf("Compressing bitStream, CMD: %d, orig size: %d, packed size: %d+4 first 8 bytes : '%s'\n", firstBitsPack, byteLength, destLen, buf);

}

static z_stream*            zStream;
static CRITICAL_SECTION        zCS;

static void* bsZAlloc(void* opaque, U32 items, U32 size)
{
    return calloc(items * size, 1);
}

static void bsZFree(void* opaque, void* address)
{
    free(address);
}

void bsInitZStreamStruct(void *zstream_void)
{
    z_stream    *z = zstream_void;

    if (z)
    {
        z->zalloc        = bsZAlloc;
        z->zfree        = bsZFree;
    }
}

static void bsInitZStream()
{
    if(!zStream)
    {
        InitializeCriticalSection(&zCS);

        zStream    = calloc(1, sizeof(*zStream));

        bsInitZStreamStruct(zStream);

        EnterCriticalSection(&zCS);
    }
    else
    {
        EnterCriticalSection(&zCS);

        inflateEnd(zStream);
    }

    inflateInit(zStream);
}

static int bsZUncompress(void* outbuf, U32* outsize, void* inbuf, U32 insize)
{
    int ret;

    bsInitZStream();

    zStream->avail_out        = *outsize;
    zStream->next_out        = outbuf;
    zStream->next_in        = inbuf;
    zStream->avail_in        = insize;
    ret = inflate(zStream, Z_FINISH);

    LeaveCriticalSection(&zCS);

    //assert(zStream->avail_out == 0);

    return ret;
}

void bsUnCompress(BitStream *bs, int useTypedBits, void *zstream_void) {
    char *dest;
    unsigned long byteLength;
    unsigned long destLen, oldDestLen;
    int ret;
    int firstBitsPack;
    unsigned long debug_byte_length = 0;
    unsigned int cursorPosition = bsGetCursorBitPosition(bs);
    z_stream    *z = zstream_void;

    assert(bs->cursor.bit==0);
    assert(bs->byteAlignedMode);

    // Get first BitsPack
    if (useTypedBits) {
        firstBitsPack = bsTypedReadBitsPack(bs, 1);
    } else {
        firstBitsPack = bsReadBitsPack(bs, 1);
    }

    // Determine size of remaining data
    byteLength = (bs->bitLength >> 3) - bs->cursor.byte;
    //printf("Receiving packet with %d bytes header, %d bytes compressed data\n", bs->cursor.byte-4, byteLength+4);

    if (byteLength < SMALLEST_DATA_TO_COMPRESS || bs->bitLength>>3 < bs->cursor.byte) {
        // If the data is this small, it must not have been compressed!
        // Just reset the cursor
        //printf("No need to uncompress bitStream, CMD: %d, data size: %d first 8 bytes : '%s'\n", firstBitsPack, byteLength, debugBytesToString(bs->data+bs->cursor.byte, byteLength, 8));
        bsSetCursorBitPosition(bs, cursorPosition);
        return;
    }

    if ((bs->bitLength & 7) !=0 ) {
        // not a valid compressed bitstream!
        BSASSERT(!"Odd bit compressed bitstream?");
        bs->errorFlags = BSE_COMPRESSION;
        return;
    }

    // Get size of uncompressed data
    if (useTypedBits) {
        oldDestLen = destLen = bsTypedReadBitsPack(bs, 8);
        debug_byte_length = bsTypedReadBitsPack(bs, 8);
    } else {
        oldDestLen = destLen = bsReadBitsPack(bs, 8);
    }
    // Allocate memory for uncompressed data
    if (destLen > 1024*1024*1024) {
        // More than 1 GB is probably bad data!
        BSASSERT(!"Larger than 1 GB packet!  Probably corrupt data.");
        bs->errorFlags = BSE_COMPRESSION;
        return;
    }
    dest = calloc(destLen, 1);
    // Determine size of compressed stream
    byteLength = (bs->bitLength >> 3) - bs->cursor.byte;
    if (useTypedBits && debug_byte_length!=byteLength) {
        BSASSERT(!"Data size does not match debug length");
        bs->errorFlags = BSE_TYPEMISMATCH;
        free(dest);
        return;
    }
    if (z)
    {
        z->avail_out    = destLen;
        z->next_out        = dest;
        z->next_in        = bs->data+bs->cursor.byte;
        z->avail_in        = byteLength;

        // z->zalloc and z->zfree should have been set before calling inflateInit()
        assert(z->zalloc == bsZAlloc);
        assert(z->zfree == bsZFree);

        ret = inflate(z,Z_SYNC_FLUSH);
        assert(z->avail_out == 0);
    }
    else
    {
        ret = bsZUncompress(dest, &destLen, bs->data+bs->cursor.byte, byteLength);
    }
    if (ret != Z_OK && ret != Z_STREAM_END) {
        if (!g_bAssertOnBitStreamError) {
            bs->errorFlags = BSE_COMPRESSION;
            free(dest);
            return;
        }
        switch (ret) {
        case Z_MEM_ERROR:
            FatalErrorf("Error uncompressing BitStream data : Memory error");
            break;
        case Z_BUF_ERROR:
            FatalErrorf("Error uncompressing BitStream data : Not a big enough buffer");
            break;
        case Z_DATA_ERROR:
            FatalErrorf("Error uncompressing BitStream data : Bad input data");
            break;
        default:
            FatalErrorf("Error uncompressing BitStream data : Unknown error");
            break;
        }
    }

    //printf("UNCompressing bitStream, CMD: %d, orig size: %d, packed: %d first 8 bytes : '%s'\n", firstBitsPack, oldDestLen, byteLength+sizeof(int), debugBytesToString(dest, oldDestLen, 8));
    if (destLen != oldDestLen) {
        BSASSERT(!"Size after uncompression does not match the size it said it should be!");
        bs->errorFlags = BSE_COMPRESSION;
        free(dest);
        return;
    }

    bsRewind(bs);
    memset(bs->data, 0, 6);
    bs->bitLength = bs->size = 0;
    bsChangeMode(bs, Write);
    if (useTypedBits) {
        bsTypedWriteBitsPack(bs, 1, firstBitsPack);
    } else {
        bsWriteBitsPack(bs, 1, firstBitsPack);
    }
    assert(bs->cursor.bit==0);

    // Copy to destination
    while (destLen >= bs->maxSize - bs->cursor.byte){
        if(bs->memAllocator){
            bs->memAllocator(bs);
        } else {
            BSASSERT(!"No memory allocator and we need more");
            bs->errorFlags = BSE_OVERFLOW;
            free(dest);
            return;
        }
    }
    bsWriteBitsArray(bs, destLen*8, dest);
    bsChangeMode(bs, Read);
    //memcpy(bs->data, dest, destLen);
    free(dest);
    //bs->bitLength = destLen * 8;
    //bs->size = destLen;
    bsRewind(bs);
}

void testBitStream(){
    BitStream bs;
    #define bufferSize 4 * 1024
    unsigned char buffer[bufferSize];

    bsAssertOnErrors(1);
    // Simple read/write test.
    {
        int result;
        int opBitCount = 9;
        int opData = 257;

        initBitStream(&bs, buffer, bufferSize, Write, false, NULL);

        printf("Inserting %i bits into the stream with value %i\n", opBitCount, opData);
        bsWriteBits(&bs, opBitCount, opData);

        bsChangeMode(&bs, Read);
        result = bsReadBits(&bs, opBitCount);
        printf("Retrieving %i bits from the stream returned %i\n", opBitCount, result);
    }

    // Multiple read/write test.
    {
        int resultInt;
        char resultStr[1024];
        int opBitCount = 9;
        int opData = 257;
        int opData2 = -1;
        F32 opFloat = -1.;
        F32 resultFloat;
        char str[] = "Hello world!";
        char str2[] = "Hello again!";

        initBitStream(&bs, buffer, bufferSize, Write, false, NULL);

        printf("\nInserting %i bits into the stream with value %i\n", opBitCount, opData);
        bsWriteBits(&bs, opBitCount, opData);

        printf("Inserting %i bitsPack into the stream with value %i\n", opBitCount, opData2);
        bsWriteBitsPack(&bs, opBitCount, opData2);

        printf("Inserting F32 into the stream with value %f\n", opFloat);
        bsWriteF32(&bs, opFloat);

        printf("Inserting %i bits into the stream with value %s\n", sizeof(str) << 3, str);
        bsWriteBitsArray(&bs, sizeof(str) << 3, str);

        printf("Inserting %i bits into the stream with value %s\n", sizeof(str2) << 3, str2);
        bsWriteBitsArray(&bs, sizeof(str2) << 3, str2);


        bsChangeMode(&bs, Read);
        resultInt = bsReadBits(&bs, opBitCount);
        printf("Retrieving %i bits from the stream returned %i\n", opBitCount, resultInt);

        resultInt = bsReadBitsPack(&bs, opBitCount);
        printf("Retrieving %i bits from the stream returned %i\n", opBitCount, resultInt);

        resultFloat = bsReadF32(&bs);
        printf("Retrieving F32 from the stream returned %f\n", resultFloat);

        bsReadBitsArray(&bs, sizeof(str) << 3, resultStr);
        printf("Retrieving %i bits from the stream returned %s\n", sizeof(str) << 3, resultStr);

        bsReadBitsArray(&bs, sizeof(str2) << 3, resultStr);
        printf("Retrieving %i bits from the stream returned %s\n", sizeof(str2) << 3, resultStr);
    }

}





typedef enum{
    BS_BITS = 3,
    BS_PACKEDBITS,
    BS_BITARRAY,
    BS_STRING,
    BS_F32
} BitType;

// During bsTypedXXX, "DataTypeBitLength" bits are placed before the actual data
// to denote the "type" of the data.
#define DataTypeBitLength 3

// During bsTypedXXX, at least "DataLengthMinBits" bits are used to encode the length
// of data associated with that particular operation.  The "length" means different
// things depending on the type.
#define DataLengthMinBits 5

void bsTypedWriteBits(BitStream* bs, int numbits, unsigned int val){
    bsWriteBits(bs, DataTypeBitLength, BS_BITS);
    bsWriteBitsPack(bs, DataLengthMinBits, numbits);
    bsWriteBits(bs, numbits, val);
}

static const char* bsGetBitTypeName(BitType bitType){
    switch(bitType){
        case BS_BITS:
            return "BS_BITS";
        case BS_PACKEDBITS:
            return "BS_PACKEDBITS";
        case BS_BITARRAY:
            return "BS_BITARRAY";
        case BS_STRING:
            return "BS_STRING";
        case BS_F32:
            return "BS_F32";
        default: {
            static char buf[64];
            sprintf_s(SAFESTR(buf), "UNKNOWN (%d)", bitType);
            return buf;
        }
    }
}

#define ASSERT_INTVALUE(name, rightParam, wrongParam){        \
    int right = rightParam;                                    \
    int wrong = wrongParam;                                    \
    if(right != wrong){                                        \
        if (g_bAssertOnBitStreamError) {                    \
            assertNumbits(name, right, wrong);                \
        }                                                    \
        bs->errorFlags = BSE_TYPEMISMATCH;                    \
    }                                                        \
}

#define READ_DATALENGTH_NAME(name, expectedLength){            \
    int dataLength = bsReadBitsPack(bs, DataLengthMinBits);    \
    ASSERT_INTVALUE(name, dataLength, expectedLength);        \
}

#define READ_DATALENGTH(expectedLength)                        \
    READ_DATALENGTH_NAME(#expectedLength, expectedLength)

static DataTypeErrorCb s_datatypeErrCb = NULL;

DataTypeErrorCb setDataTypeErrorCb(DataTypeErrorCb dtErrCb)
{
    DataTypeErrorCb cur = s_datatypeErrCb;
    s_datatypeErrCb = dtErrCb;
    return cur;
}

static void checkDataTypeError(BitStream* bs, int type, int expected){
    char buffer[1000] = "";
    int dataLength;
    char* str;

    strcatf(buffer,
            "Bitstream Error:\n"
            "  Found type: %s\n",
            bsGetBitTypeName(type));
            
    strcatf(buffer,
            "  Expected type: %s\n",
            bsGetBitTypeName(expected));

    switch(type){
        xcase BS_BITS:
        case BS_PACKEDBITS:
        case BS_BITARRAY:
        case BS_F32:
            dataLength = bsReadBitsPack(bs, DataLengthMinBits);
            strcatf(buffer, "Expected bits: %d\n", dataLength);

        xcase BS_STRING:
            dataLength = bsReadBitsPack(bs, DataLengthMinBits);
            str = bsReadString(bs);
            strcatf(buffer, "String: \"%s\"\n  Length: %d\n  Expected Length: %d\n", str, dataLength, strlen(str));
    }

    printf("\n%s", buffer);

    if( s_datatypeErrCb )
        s_datatypeErrCb(buffer);
    else
        assertmsg(0, buffer);
}

static int readDataType(BitStream* bs, int expectedType){
    int gotType = bsReadBits(bs, DataTypeBitLength);

    if (gotType != expectedType) {
        if (g_bAssertOnBitStreamError) {
            checkDataTypeError(bs, gotType, expectedType);
        }
        bs->errorFlags = BSE_TYPEMISMATCH;
        return 0;
    }
    
    return 1;
}

#define READ_DATATYPE(expectedType) if(!readDataType(bs, expectedType)) return 0;

static void assertNumbits(const char* name, int right, int wrong){
    char buffer[100];
    sprintf_s(SAFESTR(buffer), "Wrong %s.  Right: %d.  Wrong: %d\n", name, right, wrong);

    printf("\n%s", buffer);
    assertmsg(0, buffer);
}

unsigned int bsTypedReadBits(BitStream* bs, int numbits){
    // Is the correct type being extracted?

    READ_DATATYPE(BS_BITS);

    // Is the correct number of bits being extracted?

    READ_DATALENGTH(numbits);

    return bsReadBits(bs, numbits);
}

int bsTypedWriteBitsArray(BitStream* bs, int numbits, const void* data){
    bsWriteBits(bs, DataTypeBitLength, BS_BITARRAY);
    bsWriteBitsPack(bs, DataLengthMinBits, numbits);
    return bsWriteBitsArray(bs, numbits, data);
}

int bsTypedReadBitsArray(BitStream* bs, int numbits, void* data){
    // Is the correct type being extracted?

    READ_DATATYPE(BS_BITARRAY);

    // Is the correct number of bits being extracted?

    READ_DATALENGTH(numbits);

    return bsReadBitsArray(bs, numbits, data);
}

void bsTypedWriteBitsPack(BitStream* bs, int minbits, unsigned int val){
    bsWriteBits(bs, DataTypeBitLength, BS_PACKEDBITS);
    bsWriteBitsPack(bs, DataLengthMinBits, minbits);
    bsWriteBitsPack(bs, minbits, val);
}

int bsGetLengthTypedWriteBitsPack(BitStream* bs, int minbits, unsigned int val){
    int ret=0;
    ret += bsGetLengthWriteBits(bs, DataTypeBitLength, BS_PACKEDBITS);
    ret += bsGetLengthWriteBitsPack(bs, DataLengthMinBits, minbits);
    ret += bsGetLengthWriteBitsPack(bs, minbits, val);
    return ret;
}

unsigned int bsTypedReadBitsPack(BitStream* bs,int minbits){
    // Is the correct type being extracted?
    
    READ_DATATYPE(BS_PACKEDBITS);

    // Is the correct number of bits being extracted?
    
    READ_DATALENGTH(minbits);

    return bsReadBitsPack(bs, minbits);
}

void bsTypedWriteString(BitStream* bs, const char* str){
    if(!str)
        str = "";

    bsWriteBits(bs, DataTypeBitLength, BS_STRING);
    bsWriteBitsPack(bs, DataLengthMinBits, (int)strlen(str));
    bsWriteString(bs, str);
}

void bsTypedWriteStringAligned(BitStream* bs, const char* str)
{
    if(!str)
        str = "";

    bsWriteBits(bs, DataTypeBitLength, BS_STRING);
    bsWriteBitsPack(bs, DataLengthMinBits, (int)strlen(str));
    bsWriteStringAligned(bs, str);
}

char* bsTypedReadStringAndLength(BitStream* bs, int *pLen)
{
    // does not find aligned/unaligned mismatches

    int dataLength;
    char* str;

    // Is the correct type being extracted?
    
    READ_DATATYPE(BS_STRING);

    // How long should the string be?

    dataLength = bsReadBitsPack(bs, DataLengthMinBits);
    str = bsReadStringAndLength(bs, pLen);

    // Did the string match the expected string length?

    ASSERT_INTVALUE("stringlength", dataLength, (int)strlen(str));

    return str;
}

char* bsTypedReadStringAligned(BitStream *bs)
{
    // does not find aligned/unaligned mismatches

    int dataLength;
    char* str;

    // Is the correct type being extracted?
    READ_DATATYPE(BS_STRING);

    // How long should the string be?
    dataLength = bsReadBitsPack(bs, DataLengthMinBits);
    str = bsReadStringAligned(bs);

    // Did the string match the expected string length?
    ASSERT_INTVALUE("stringlength", dataLength, (int)strlen(str));

    return str;
}

void bsTypedWriteF32(BitStream* bs, float f){
    bsWriteBits(bs, DataTypeBitLength, BS_F32);
    bsWriteBitsPack(bs, DataLengthMinBits, 32);
    bsWriteF32(bs, f);
}

float bsTypedReadF32(BitStream* bs){
    // Is the correct type being extracted?
    
    READ_DATATYPE(BS_F32);

    // Is the correct number of bits being extracted?

    READ_DATALENGTH_NAME("floatsize", sizeof(float) * 8);

    return bsReadF32(bs);
}

int bsTypedAlignBitsArray(BitStream* bs){
    int dataLength;

    // Read and discard the bit array "type" marker.

    READ_DATATYPE(BS_BITARRAY);

    // Read and discard the bit array length.
    //    WARNING!  Using this function means the bit array length
    //    will not be checked!
    dataLength = bsReadBitsPack(bs, DataLengthMinBits);

    // Align the cursor to where the bit array starts.
    bsAlignByte(bs);
    return 1;
}

// To call this on a good packet, for example, run these:
// bsReadBits(&pak->stream, 3)
// bsReadBitsPack(&pak->stream, 5)
// bsTypedWalkStream(&pak->stream, firstvalue, secondvalue)
void bsTypedWalkStream(BitStream* bs, BitType initialBitType, int initialBitLength)
{
    BitType bt = initialBitType;
    int bl = initialBitLength;
    int val;
    F32 fval;
    int quit=0;
    char *str = "check stack around checkDataTypeError";
    // Assumes we hit a bsAssert (or a dataLength assert), and that the bitType and dataLength have
    //  already  been read
    do {
        switch (bt) {
        case BS_BITS:
            val = bsReadBits(bs, bl);
            printf("%d bit%s = %d\n", bl, (bl>1)?"s":"", val);
        xcase BS_PACKEDBITS:
            val = bsReadBitsPack(bs, bl);
            printf("%d packedBits = %d\n", bl, val);
        xcase BS_BITARRAY:
            printf("BitArrayNotHandled\n");
            quit=1;
        xcase BS_F32:
            fval = bsReadF32(bs);
            printf("F32 = %f\n", fval);
        xcase BS_STRING:
            printf("String = %s\n", str);
        xdefault:
            printf("error/bad stream?");
            quit = 1;
        }
        if (bsEnded(bs)) {
            quit = 1;
        } else {
            bt = bsReadBits(bs, DataTypeBitLength);
            bl = bsReadBitsPack(bs, DataLengthMinBits);
            if (bt==BS_STRING) {
                str = bsReadString(bs);
            }
        }
    } while (!quit && !bsEnded(bs));
    printf("done.\n");
}
