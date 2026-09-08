#include "utilitieslib/components/MemRef.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/wininclude.h"

#define MEMREF_TAG 0xC0DF15CE

typedef struct MemRef {
    volatile LONG reference_count;
    U32 tag;
} MemRef;

void *memrefAllocInternal(size_t size MEM_DBG_PARMS)
{
    MemRef *memref = scalloc(size + sizeof(MemRef), 1);
    memref->reference_count = 1;
    memref->tag = MEMREF_TAG;
    return (memref+1);
}

int memrefIncrement(void *data)
{
    int ret;
    MemRef *memref = ((MemRef*)data)-1;
    assert(memref->tag==MEMREF_TAG);
    ret = InterlockedIncrement(&memref->reference_count);
    assert(ret>0);
    return ret;
}

// Will free the data if releasing the last reference
int memrefDecrement(void *data)
{
    int ret;
    MemRef *memref = ((MemRef*)data)-1;
    assert(memref->tag==MEMREF_TAG);
    ret = InterlockedDecrement(&memref->reference_count);
    assert(ret>=0);
    if (ret==0) {
        free(memref);
    }
    return ret;
}
