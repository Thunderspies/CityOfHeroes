#ifndef BITSTREAM_H
#define BITSTREAM_H

#include "../stdtypes.h"

C_DECLARATIONS_BEGIN

typedef enum {
    Read,
    Write
} BitStreamMode;

typedef enum {
    BSE_NOERROR=0,
    BSE_OVERFLOW, // Buffer filled up
    BSE_OVERRUN, // Read past end of buffer
    BSE_TYPEMISMATCH, // Debug bit type mismatch
    BSE_COMPRESSION, // Misc compression errors (bad data, probably)
} BitStreamError;

/* Structure BitStreamCursor
 *    The bitstream cursor is used to point to locate a single bit in a bitstream.
 *    The cursor points at a position relative to a base position in a bitstream.
 *
 */
typedef struct{
    unsigned int    byte;    // points at the byte offset from bitstream base position.
    unsigned int    bit;    // points at the bit offset relative to the byte offset.
} BitStreamCursor;

typedef struct BitStream BitStream;
typedef int (*MemoryAllocator)(BitStream* bs);
typedef void (*DataTypeErrorCb)(char *buffer);
DataTypeErrorCb setDataTypeErrorCb(DataTypeErrorCb dtErrCb);

/* Structure BitStream
 */
typedef struct BitStream{
    BitStreamMode    mode;
    BitStreamCursor cursor;

    unsigned char*    data;        // current contents of the bitstream
    unsigned int    size;        // current size of the bitstream stored in "data" (in bytes)
    unsigned int    maxSize;    // maximum allowable size of the bitstream (in bytes)
    unsigned int    bitLength;    // How many bits are there in this stream total? (derived from cursor position when appropriate)
    MemoryAllocator memAllocator;
    void*            userData;
    unsigned int    byteAlignedMode;
    BitStreamError    errorFlags; // Set to non-zero if an error occurs in the bitsteam, must be checked by the caller
} BitStream;

void bsAssertOnErrors(int allow);
void bsInitZStreamStruct(void *zstream_void);

void initBitStream(BitStream* bs, unsigned char* buffer, unsigned int bufferSize, BitStreamMode initMode, unsigned int byteAligned, MemoryAllocator alloc);
void cloneBitStream(BitStream* orig, BitStream* clone);

int bsIsBad(BitStream *bs);

void bsSetByteAlignment(BitStream* bs, int align);
int bsGetByteAlignment(BitStream* bs);

void bsWriteBits(BitStream* bs, int numbits, unsigned int val);
int bsGetLengthWriteBits(BitStream* bs, int numbits, unsigned int val);
unsigned int bsReadBits(BitStream* bs, int numbits);

int bsWriteBitsArray(BitStream* bs, int numbits, const void* data);
int bsReadBitsArray(BitStream* bs, int numbits, void* data);

void bsWriteBitsPack(BitStream* bs, int minbits, unsigned int val);
int bsGetLengthWriteBitsPack(BitStream* bs, int minbits, unsigned int val);
unsigned int bsReadBitsPack(BitStream* bs,int minbits);

void bsWriteString(BitStream* bs, const char* str);
void bsWriteStringAligned(BitStream *bs, const char *str);
char* bsReadStringAndLength(BitStream* bs, int *pLen);
char* bsReadStringAligned(BitStream *bs);

static _inline char* bsReadString(BitStream *bs)
{
    int temp;

    return bsReadStringAndLength(bs, &temp);
}

void bsWriteF32(BitStream* bs, float f);
float bsReadF32(BitStream* bs);


void bsAlignByte(BitStream* bs);
void bsRewind(BitStream* bs);
void bsChangeMode(BitStream* bs, BitStreamMode newMode);

unsigned int bsGetBitLength(BitStream* bs);
unsigned int bsGetLength(BitStream* bs);
unsigned int bsEnded(BitStream* bs);

unsigned int bsGetCursorBitPosition(BitStream* bs);
void bsSetCursorBitPosition(BitStream* bs, unsigned int position);
void bsSetEndOfStreamPosition(BitStream *bs, unsigned int position);

void testBitStream();

void bsCompress(BitStream *bs, int useTypedBits, void *zstream);
void bsUnCompress(BitStream *bs, int useTypedBits, void *zstream);


void bsTypedWriteBits(BitStream* bs, int numbits, unsigned int val);
unsigned int bsTypedReadBits(BitStream* bs, int numbits);

int bsTypedWriteBitsArray(BitStream* bs, int numbits, const void* data);
int bsTypedReadBitsArray(BitStream* bs, int numbits, void* data);

void bsTypedWriteBitsPack(BitStream* bs, int minbits, unsigned int val);
int bsGetLengthTypedWriteBitsPack(BitStream* bs, int minbits, unsigned int val);
unsigned int bsTypedReadBitsPack(BitStream* bs,int minbits);

void bsTypedWriteString(BitStream* bs, const char* str);
void bsTypedWriteStringAligned(BitStream *bs, const char *str);
char* bsTypedReadStringAndLength(BitStream* bs, int *pLen);
char* bsTypedReadStringAligned(BitStream *bs);
static _inline char *bsTypedReadString(BitStream *bs)
{
    int temp;
    return bsTypedReadStringAndLength(bs, &temp);
}

void bsTypedWriteF32(BitStream* bs, float f);
float bsTypedReadF32(BitStream* bs);

int bsTypedAlignBitsArray(BitStream* bs);

static INLINEDBG int count_bits(unsigned int val) // returns 1 at a minimum
{
    int bits = 0;

    do
    {
        val >>= 1;
        ++bits;
    }
    while (val);

    return bits;
}

C_DECLARATIONS_END

#endif
