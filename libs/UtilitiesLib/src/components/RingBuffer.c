#include "utilitieslib/utils/memcheck.h"
#include "utilitieslib/components/RingBuffer.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/assert/assert.h"

typedef struct RingBufferHeader {
    size_t len;
} RingBufferHeader;

RingBuffer *createRingBuffer(size_t size)
{
    RingBuffer *rb = calloc(sizeof(RingBuffer), 1);
    ringBufferInit(rb, size);
    return rb;
}

void ringBufferInit(RingBuffer *rb, size_t size)
{
    rb->buffer = calloc(size, 1);
    rb->ringbuffer_size = size;
}

void destroyRingBuffer(RingBuffer *rb)
{
    SAFE_FREE(rb->buffer);
    SAFE_FREE(rb);
}

U8 *ringBufferAlloc(RingBuffer *rb, size_t len)
{
    bool tooLarge=false;
    U8 *ret=NULL;
    RingBufferHeader rbh;

    len += sizeof(RingBufferHeader);
    rbh.len = len;

    if (rb->ringbuffer_start > rb->ringbuffer_end) {
        // No free space end wrap
        if (rb->ringbuffer_end + len >= rb->ringbuffer_start) {
            // too large
            tooLarge = true;
        } else {
            // fits
            ret = rb->buffer + rb->ringbuffer_end;
            rb->ringbuffer_end += len;
        }
    } else {
        // free space wraps around the end
        if (rb->ringbuffer_end + len > rb->ringbuffer_size ||
            (rb->ringbuffer_end + len == rb->ringbuffer_size && rb->ringbuffer_start==0))
        {
            // Would go off the end
            if (len >= rb->ringbuffer_start) {
                // Doesn't fit at the beginning
                tooLarge = true;
            } else {
                // Fit at the beginning
                ret = rb->buffer;
                rb->ringbuffer_end = len;
            }
        } else {
            // Fits at end
            ret = rb->buffer + rb->ringbuffer_end;
            rb->ringbuffer_end += len;
            if (rb->ringbuffer_end == rb->ringbuffer_size)
                rb->ringbuffer_end = 0;
        }
    }
    assert(tooLarge == !ret);
    if (!ret) {
        ret = calloc(len, 1);
        rb->numMallocs++;
    }
    rb->numAllocations++;
    *(RingBufferHeader*)ret = rbh;
    return ret + sizeof(RingBufferHeader);
}

void ringBufferFree(RingBuffer *rb, U8 *data)
{
    U8 *actualdata = data - sizeof(RingBufferHeader);
    size_t len;
    if (!data)
        return;
    len = ((RingBufferHeader*)actualdata)->len;
    if (actualdata < rb->buffer || actualdata > rb->buffer + rb->ringbuffer_size) {
        free(actualdata);
    } else {
        // In queue
        if (actualdata == rb->buffer + rb->ringbuffer_start) {
            // Simple case
            rb->ringbuffer_start += len;
            if (rb->ringbuffer_start == rb->ringbuffer_size)
                rb->ringbuffer_start = 0;
        } else {
            // Must be at beginning
            assert(actualdata == rb->buffer);
            rb->ringbuffer_start = len;
        }
    }
}

char *ringBufferStrdup(RingBuffer *rb, const char *str)
{
    char    *mem;
    size_t    len;

    if (!str)
        return 0;
    len = strlen(str)+1;
    mem = ringBufferAlloc(rb, len);
    strcpy_s(mem,len,str);
    return mem;    
}
