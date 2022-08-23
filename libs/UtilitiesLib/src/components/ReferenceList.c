#include "utilitieslib/components/ReferenceList.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/assert/assert.h"

typedef union ReferenceImp {
    struct 
    {
        unsigned long arrayIndex:16;
        unsigned long arrayOffs:8;
        unsigned long uid:8;
    };
    Reference ref;
} ReferenceImp;

typedef struct ReferenceNode {
    ReferenceImp ref;
    void *data; // Doubles to hold the "Next" pointer for the freelist
} ReferenceNode;

typedef struct ReferenceListImp {
    int    structCount; // Count per chunk
    ReferenceNode** chunkTable; // An EArray of memory chunks
    ReferenceNode*    freelist; // points to the next free Node
    ReferenceNode*    freelisttail; // points to the last free Node
    int    allocationCount; // Number of structs allocated.
} ReferenceListImp;


ReferenceList createReferenceList(void)
{
    ReferenceListImp *rlist = calloc(sizeof(ReferenceListImp),1);
    rlist->structCount = 256; // hard-coded now, could handle less, but not more...
    return rlist;
}

void destroyReferenceList(ReferenceList rlist)
{
    eaDestroy(&rlist->chunkTable);
    free(rlist);
}

void referenceListAddNewChunk(ReferenceList rlist)
{
    int i, indexBase;
    ReferenceNode *memoryChunk; 
    assert(!(rlist->freelist || rlist->freelisttail));

    memoryChunk = calloc(sizeof(ReferenceNode)*rlist->structCount,1);
    indexBase = eaPush(&rlist->chunkTable, memoryChunk);

    assert(indexBase < (1<<24));
    
    for (i=0; i<rlist->structCount; i++) {
        ReferenceNode *node = &memoryChunk[i];
        node->ref.arrayIndex = indexBase;
        node->ref.arrayOffs = i;
        node->ref.uid = 1; // Start at non-zero so that Reference(0) is never a valid reference
        node->data = node+1; // Next pointer
    }
    rlist->freelist = &memoryChunk[0];
    rlist->freelisttail = &memoryChunk[rlist->structCount-1];
    rlist->freelisttail->data = NULL;
}

Reference referenceListAddElement(ReferenceList rlist, void *data)
{
    ReferenceNode *node=NULL;

    if (!rlist->freelist) {
        assert(!rlist->freelisttail);
        referenceListAddNewChunk(rlist);
    }
    assert(rlist->freelist);
    node = rlist->freelist;
    rlist->freelist = node->data;
    // already allocated on free: node->ref.uid;
    if (rlist->freelisttail == node) {
        // Hit the end of the list
        rlist->freelisttail = NULL;
        assert(rlist->freelist == NULL);
    }
    rlist->allocationCount++;
    assert(rlist->allocationCount <= rlist->structCount * eaSize(&rlist->chunkTable));
    node->data = data;
    return node->ref.ref;
}

INLINEDBG ReferenceNode *referenceListFindNodeByRef(ReferenceList rlist, ReferenceImp ref)
{
#if 0
    if (ref.arrayIndex >= eaSize(&rlist->chunkTable)) {
        assert(!"Since referenceLists don't shrink, this should never happen!");
        return NULL;
    }
#endif
    return &(rlist->chunkTable[ref.arrayIndex])[ref.arrayOffs];
}

void *referenceListFindByRef(ReferenceList rlist, Reference _ref)
{
    ReferenceNode *node;

    node = referenceListFindNodeByRef(rlist, *(ReferenceImp*)&_ref);

    if (node->ref.ref == ((ReferenceImp*)&_ref)->ref) {
        return node->data;
    }
    return NULL;
}

void referenceListRemoveElement(ReferenceList rlist, Reference _ref)
{
    ReferenceNode *node;

    node = referenceListFindNodeByRef(rlist, *(ReferenceImp*)&_ref);

    assert(node);
    if (!(node->ref.ref == ((ReferenceImp*)&_ref)->ref)) {
        // freeing someone twice!
        assert(0);
        return;
    }

    if (!rlist->freelist) {
        // No freelist
        assert(!rlist->freelisttail);
        rlist->freelist = rlist->freelisttail = node;
    } else {
        // Return to end of freelist
        assert(rlist->freelist);
        rlist->freelisttail->data = node;
        rlist->freelisttail = node;
    }
    node->data = NULL;
    // Invalidate all old references (they would have the wrong UID now), use values 1-255
    //node->ref.uid = (node->ref.uid)%255 + 1;
    node->ref.uid++;
    if (node->ref.uid==0)
        node->ref.uid=1;

    rlist->allocationCount--;
    assert(rlist->allocationCount >= 0);
}

void referenceListMoveElement(ReferenceList rlist, Reference rdst, Reference rsrc)
{
    ReferenceNode *nodesrc;
    ReferenceNode *nodedst;

    nodesrc = referenceListFindNodeByRef(rlist, *(ReferenceImp*)&rsrc);
    nodedst = referenceListFindNodeByRef(rlist, *(ReferenceImp*)&rdst);
    assert(nodesrc && nodesrc->ref.ref == ((ReferenceImp*)&rsrc)->ref);
    if (!nodedst || nodedst->ref.ref != ((ReferenceImp*)&rdst)->ref) {
        ReferenceNode *node;
        // It's been freed! (expected, since FX uses this this way)
        assert(    nodedst->ref.uid == ((ReferenceImp*)&rdst)->uid + 1); // Not freed more than once
        assert( rlist->freelist );
        assert( nodedst );
        assert( rlist->freelisttail == nodedst ); // Not technically needed, but because of how the FX system uses this function, it should be true
        // walk the freelist and remove
        if (nodedst == rlist->freelist) { // a one-element list, most likely
            // first node is us!
            rlist->freelist = rlist->freelist->data;
            if (!rlist->freelist)
                rlist->freelisttail = NULL;
        } else { // we're not the head!  but probably the tail
            node = rlist->freelist;
            while ( node->data != nodedst ) {
                node = node->data;
            }
            node->data = nodedst->data;
            if (rlist->freelisttail == nodedst)
                rlist->freelisttail = node;
        }
        rlist->allocationCount++;
    }
    nodedst->data = nodesrc->data;
    nodedst->ref.uid = ((ReferenceImp*)&rdst)->uid;
}



#if 0 // Testing function for dropping into main.c speed vs. fxlist.c/h
#include "ReferenceList.h"
static void referenceListSppedTest()
{
    int timer = timerAlloc();
#define LOOPS 30000000
    int allocs[5000];
    int rnds[10000];
    int i;
    srand(1);
    newConsoleWindow();

    for (i=0; i<ARRAY_SIZE(rnds); i++) {
        rnds[i] = rand() % ARRAY_SIZE(allocs);
    }

    Sleep(1000);
    memset(allocs, 0, sizeof(allocs));
    hdlInitHandles(MAX_HANDLES);
    timerStart(timer);
    for (i=0; i<LOOPS; i++) {
        int o = rnds[i%ARRAY_SIZE(rnds)];
        if (allocs[o]) {
            void *p = hdlGetPtrFromHandle(allocs[o]);
            hdlClearHandle(allocs[o]);
        }
        allocs[o] = hdlAssignHandle((void*)(i+1));
    }
    printf("time: %f\n", timerElapsed(timer));
    //return;

    memset(allocs, 0, sizeof(allocs));
    {
        ReferenceList rl = createReferenceList();
        timerStart(timer);
        for (i=0; i<LOOPS; i++) {
            int o = rnds[i%ARRAY_SIZE(rnds)];
            if (allocs[o]) {
                void *p = referenceListFindByRef(rl, allocs[o]);
                referenceListRemoveElement(rl, allocs[o]);
            }
            allocs[o] = referenceListAddElement(rl, (void*)(i+1));
        }
    }

    printf("time: %f\n", timerElapsed(timer));
    timerFree(timer);
    Sleep(5000);
}
#endif
