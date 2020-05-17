// -------------------------------------------------------------------------------------------------------------------
// NOVODEX SDK STREAM INTERFACE
// Written by Tom Lassanske, 03-01-05
// -------------------------------------------------------------------------------------------------------------------
#include "NovodeX/NwWrapper.h"
#if NOVODEX

#include "NwSharedStream.h"


NxU8* NwSharedStream::pTempBuffer;
NxU32 NwSharedStream::uiBufferSize;

// -------------------------------------------------------------------------------------------------------------------
// CONSTRUCTION                                                                                           CONSTRUCTION
// -------------------------------------------------------------------------------------------------------------------
NwSharedStream::NwSharedStream(const char* streamName, bool load)
{
    // allocate a temp buffer
    if ( !pTempBuffer )
    {
        // 1 k for now...
        uiBufferSize = 1024;
        pTempBuffer = (NxU8*)calloc(uiBufferSize, 1);
    }

    pCursor = pTempBuffer;

}

NwSharedStream::NwSharedStream(const NxU8* pcStream, int iStreamSize, bool bCopy)
{
    initWithCharBuffer(pcStream, iStreamSize);
}
// -------------------------------------------------------------------------------------------------------------------
NwSharedStream::~NwSharedStream()
{
}
// -------------------------------------------------------------------------------------------------------------------
bool NwSharedStream::isValid() const
{
    return (pCursor != NULL);
}

void NwSharedStream::resetByteCounter() 
{
    pCursor = pTempBuffer;
}

void NwSharedStream::checkTempBufferSize( NxU32 size )
{
    if ( pCursor + size >= pTempBuffer + uiBufferSize)
    {
        NxU8* pOldBuffer = pTempBuffer;
        NxU32 uiOldBufferSize = uiBufferSize;

        // we must grow temp buffer, by 50% for now
        NxU32 uiGrowth = uiOldBufferSize >> 1;
        while ( uiGrowth < size ) //just in case, will probably never happen
            uiGrowth <<= 1; // double it

        uiBufferSize = uiOldBufferSize + uiGrowth;
        pTempBuffer = (NxU8*)malloc(uiBufferSize);
        assert(pTempBuffer);

        memcpy(pTempBuffer, pOldBuffer, (pCursor - pOldBuffer));

        pCursor += ( pTempBuffer - pOldBuffer );

        free(pOldBuffer);
    }
}

void NwSharedStream::freeTempBuffer()
{
    free( pTempBuffer );
    pTempBuffer = NULL;
}

NxU8* NwSharedStream::getBuffer()
{
    return pTempBuffer;
}

int    NwSharedStream::getBufferSize()
{
    return (pCursor - pTempBuffer);
}

void NwSharedStream::initWithCharBuffer(const NxU8* pcBuffer, int iBufferSize )
{
    uiBufferSize = iBufferSize;
    pTempBuffer = (NxU8*)malloc(uiBufferSize);
    assert(pTempBuffer);
    memcpy(pTempBuffer, pcBuffer, iBufferSize);
    pCursor = pTempBuffer;
}


// -------------------------------------------------------------------------------------------------------------------
// LOADING API                                                                                             LOADING API
// -------------------------------------------------------------------------------------------------------------------
NxU8 NwSharedStream::readByte() const
{
    NxU8 a;
    memcpy(&a, pCursor, sizeof(a));
    pCursor += sizeof(a);
    return a;
}
// -------------------------------------------------------------------------------------------------------------------
NxU16 NwSharedStream::readWord() const
{
    NxU16 a;
    memcpy(&a, pCursor, sizeof(a));
    pCursor += sizeof(a);
    return a;
}
// -------------------------------------------------------------------------------------------------------------------
NxU32 NwSharedStream::readDword() const
{
    NxU32 a;
    memcpy(&a, pCursor, sizeof(a));
    pCursor += sizeof(a);
    return a;
}
// -------------------------------------------------------------------------------------------------------------------
float NwSharedStream::readFloat() const
{
    float a;
    memcpy(&a, pCursor, sizeof(a));
    pCursor += sizeof(a);
    return a;
}
// -------------------------------------------------------------------------------------------------------------------
double NwSharedStream::readDouble() const
{
    double a;
    memcpy(&a, pCursor, sizeof(a));
    pCursor += sizeof(a);
    return a;
}
// -------------------------------------------------------------------------------------------------------------------
void NwSharedStream::readBuffer(void* buffer, NxU32 size)    const
{
    memcpy(buffer, pCursor, size);
    pCursor += size;
}
// -------------------------------------------------------------------------------------------------------------------
// SAVING API                                                                                               SAVING API
// -------------------------------------------------------------------------------------------------------------------
NxStream& NwSharedStream::storeByte(NxU8 a)
{
    checkTempBufferSize(sizeof(a));
    memcpy(pCursor, &a, sizeof(a));
    pCursor += sizeof(a);
    return *this;
}
// -------------------------------------------------------------------------------------------------------------------
NxStream& NwSharedStream::storeWord(NxU16 a)
{
    checkTempBufferSize(sizeof(a));
    memcpy(pCursor, &a, sizeof(a));
    pCursor += sizeof(a);
    return *this;
}
// -------------------------------------------------------------------------------------------------------------------
NxStream& NwSharedStream::storeDword(NxU32 a)
{
    checkTempBufferSize(sizeof(a));
    memcpy(pCursor, &a, sizeof(a));
    pCursor += sizeof(a);
    return *this;
}
// -------------------------------------------------------------------------------------------------------------------
NxStream& NwSharedStream::storeFloat(NxReal a)
{
    checkTempBufferSize(sizeof(a));
    memcpy(pCursor, &a, sizeof(a));
    pCursor += sizeof(a);
    return *this;
}
// -------------------------------------------------------------------------------------------------------------------
NxStream& NwSharedStream::storeDouble(NxF64 a)
{
    checkTempBufferSize(sizeof(a));
    memcpy(pCursor, &a, sizeof(a));
    pCursor += sizeof(a);
    return *this;
}
// -------------------------------------------------------------------------------------------------------------------
NxStream& NwSharedStream::storeBuffer(const void* buffer, NxU32 size)
{
    checkTempBufferSize(size);
    memcpy(pCursor, buffer, size);
    pCursor += size;
    return *this;

}
// -------------------------------------------------------------------------------------------------------------------
#endif