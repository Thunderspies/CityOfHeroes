#include "utilitieslib/crt.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "utilitieslib/assert/assert.h"

#include "utilitieslib/stdtypes.h"
#include "utilitieslib/components/Queue.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/components/MemoryPool.h"

typedef struct QueueImp {
// public
    int maxSizeLimit;

// private
    void** storage;

    // The queue can currently hold a maximum of this number of elements.
    int maxSize;

    // The queue currently holds this elements.
    int size;

    // The head of the queue is currently at this index.  Read/Dequeue cursor.
    int head;

    // The tail of the queue is currently at this index.  Write/Enqueue cursor.
    int tail;
} QueueImp;

MP_DEFINE(QueueImp);
Queue createQueue(){
    MP_CREATE(QueueImp,4);
    return (Queue)MP_ALLOC(QueueImp);
}

void initQueue(QueueImp* queue, int initialQueueSize){
    assert(queue && initialQueueSize);
    if(!queue)
        return;

    assert(!queue->storage);
    queue->storage = calloc(initialQueueSize, sizeof(void*));
    queue->maxSize = initialQueueSize;
}

void destroyQueue(QueueImp* queue){
    if(!queue)
        return;
    if (queue->storage)
        free(queue->storage);
    MP_FREE(QueueImp, queue);
}

void destroyQueueEx(QueueImp* queue, Destructor destructor){
    if(!queue)
        return;
    if(destructor){
        int i;

        // Iterate through the entire queue and pass each valid
        // element to the destructor.
        for(i = 0; i < queue->maxSize; i++){
            void* element = queue->storage[i];

            if(!element)
                continue;

            destructor(element);
        }
    }

    destroyQueue(queue);
}


/* Function qEnqueue()
 *    Enqueues the give element into the given queue.
 *
 *    If the queue is full, it will be resized automatically if it's possible before
 *    attempting to enqueue the element.
 *
 *    Returns:
 *        1 - Element enqueued successfully.
 *        0 - Element not enqueued because the queue has reached full capacity.
 *
 *
 *
 */
int qEnqueue(QueueImp* queue, void* element){
    assert(queue && element);
    if(!queue)
        return 0;

    // Check if automatic initialization is needed.
    //    Has the queue been initialized?
    //    If not, initialize it now.
    if(!queue->storage)
        initQueue(queue, QUEUE_DEFAULT_SIZE);


    // Is the queue full?
    if(qIsFull(queue)){
        int resizeResult;

        // Module logic check.
        // When a queue is full, the tail should have wrapped around the array and be pointing
        // at the same element the head is pointing to.  The slot should already be occupied.
        // Otherwise, there is still room!
        // This check is disabled now so that things in the queue can be destroyed before it is
        // dequeued.
        //assert((queue->head == queue->tail) && (queue->storage[queue->tail]));    

        // The queue is full and needs to be resized first before the element can be
        // enqueued.
        resizeResult = qResize(queue, queue->maxSize * 2);

        // If the queue cannot be resized for some reason, the new element cannot be enqueued.
        if(!resizeResult)
            return 0;
    }


    // The tail of the queue is always pointing just beyond the last element of
    // in the queue, which should be an empty slot.
    // Enqueue the element.
    queue->storage[queue->tail] = element;
    

    // Update the tail position of the queue.
    // Wrap the tail position if neccessary.
    //    Note that we do not have to worry about the tail cursor overtaking the head cursor because
    //    it is impossible to reach here if the queue is full.
    queue->tail++;
    if(queue->tail == queue->maxSize)
        queue->tail = 0;
    


    // Update the number of elements there are in the queue.
    queue->size++;
    return 1;
}

/* Function qDequeue()
 *    Dequeues a element from the given queue.
 *
 *    Returns:
 *        1 - Element dequeued successfully.
 *        0 - Element not dequeued because the queue has reached full capacity.
 *
 *
 *
 */
void* qDequeue(QueueImp* queue){
    void* element;
    assert(queue);
    if(!queue)
        return NULL;

    // Is there anything in the queue at all?
    if(0 == queue->size)
        return NULL;
    
    // Retrive the element from the queue.
    element = queue->storage[queue->head];
    queue->storage[queue->head] = NULL;

    // Update the head pointer.
    queue->head++;
    if(queue->head == queue->maxSize)
        queue->head = 0;
    
    queue->size--;
    return element;
}

/* Function qPeek()
 *    Returns the next element in the queue but leaves it in the queue.
 *
 *
 *    Returns:
 *        1 - Element enqueued successfully.
 *        0 - Element not enqueued because the queue has reached full capacity.
 *
 *
 *
 */
void* qPeek(QueueImp* queue){
    if(!queue || 0 == queue->size)
        return NULL;

    return queue->storage[queue->head];
}

/* Function qFind()
 *    Returns whether the given element exists in the given queue.
 *
 *    This function performs a linear search from the head to tail
 *    of the queue.
 *
 */
int qFind(QueueImp* queue, void* target){
    int index;
    index = qFindIndex(queue, target);
    if(index == -1)
        return 0;
    else
        return 1;
}

int qFindIndex(QueueImp* queue, void* target){
    int i;
    void* element;

    // If there is nothing in the queue, the target element cannot be in the queue.
    //    This check must be here for the the following do-while loop to function correctly.
    //    There are two cases when the head and the tail of the queue overlapps:
    //        1) The queue is empty.
    //        2) The queue is full.
    //    This check rules out the first case where the queue has no elements and the head and tail
    //    cursors overlap.  The do-while loop that follows cannot deal with this case.
    if(!queue || !queue->size)
        return -1;

    // There is at least one element in the queue.
    // Begin the search at the head of the queue.
    i = queue->head;
    do{    
        element = queue->storage[i];
        if(element == target)
            return i;

        // Goto the next slot in the queue.
        i++;
        if(i >= queue->maxSize)
            i = 0;

    }while(i != queue->tail);

    return -1;
}

void** qGetWrappedElement(QueueImp* queue, int index)
{
    int realIndex;
    
    if(!queue)
        return NULL;

    realIndex = index + queue->head;

    while(realIndex >= queue->maxSize)
        realIndex -= queue->maxSize;

    return &queue->storage[realIndex];
}

void** qGetElement(QueueImp* queue, int idx){
    if(!queue)
        return 0;
    // FIXME!!!
    // Range check me!
    if(idx < 0)
        return NULL;
    return &queue->storage[idx];
}

/* Function qResize()
 *    Resizes the given queue to the given new size.
 *
 *    Note that this function will not resize the array beyond the user specified
 *    limit stored in queue::maxSizeLimit.  The function will accept a new size
 *    that is larger than the limit once only and reject all further requests
 *    to resize the queue beyond the limit.  The accepted request will not actually
 *    resize the queue beyond the limit, however, but make the queue as large
 *    as queue::maxSizeLimit.  This is a convenience mechanism so that the user 
 *    doesn't have to worry about the size limit while specifying a new size to 
 *    this function.
 *
 *    The main task of this function is to deal with "gaps" introduced by
 *    reszing the array.  It deals with this situation by copying the contents
 *    of the queue to a larger buffer in such a way that the head of the
 *    queue is always sitting at the beginning of the storage array.
 *
 *
 *    Returns:
 *        1 - Resized successfully.
 *        0 - Not resized.
 *
 */
int qResize(QueueImp* queue, int newSize){
    void** newStorage;
    int copyCount;

    assert(queue);
    if(!queue)
        return 0;
    
    // This function does not support resizing a queue to a smaller size.
    if(queue->maxSize > newSize)
        return 0;

    // Do not resize beyond the user defined max queue size limit.
    // Does a user defined limit exist?
    if(queue->maxSizeLimit != 0){

        // Is the queue already at the max size limit?
        if(queue->maxSize != queue->maxSizeLimit){
            
            // The queue is not at the max size limit yet and someone
            // is requesting a new size that is larger than then the limit?
            if(newSize > queue->maxSizeLimit)
                // Resize the queue up to the limit size.
                newSize = queue->maxSizeLimit;
        }else{

            // The queue is already at the max size limit and someone
            // is still trying to resize the queue further?  Deny the request.
            if(newSize > queue->maxSizeLimit)
                return 0;
        }
    }

    // Resize the array.
    // Create a new array of the requested size.
    newStorage = calloc(newSize, sizeof(void*));
    if(!newStorage)
        return 0;

    
    // Place the contents of the old queue into the new storage space.
    //    Copy everything from the head of the queue to the end of the storage array.
    copyCount = queue->maxSize - queue->head;
    memcpy(newStorage, &(queue->storage[queue->head]), copyCount * sizeof(void*));

    //    If the queue might have been wrapped, copy everything from the beginning of the storage
    //    array to the tail of the queue.
    if(queue->tail < queue->head || qIsFull(queue)){
        int lastElementPos;
        lastElementPos = queue->tail - 1;

        // If the last element is "wrapped" to the end of the storage array,
        // nothing further needs to be done, since the element has already been copied.
        // If the last element is not "wrapped", however...
        if(lastElementPos >= 0){
            // Copy everything from the beginning of the array up to the last element into
            // the new storage array at the place where the last memcpy left off.
            memcpy(&newStorage[copyCount], queue->storage, queue->tail * sizeof(void*));
        }
    }

    free(queue->storage);
    queue->head = 0;
    queue->tail = queue->size;
    queue->maxSize = newSize;
    queue->storage = newStorage;
    return 1;
}

int qSetMaxSizeLimit(QueueImp* queue, int newLimit){
    assert(queue);
    if(!queue)
        return 0;

    if(0 != newLimit && newLimit < queue->maxSize)
        return 0;

    queue->maxSizeLimit = newLimit;
    return 1;
}

int qGetMaxSizeLimit(QueueImp* queue){
    if(!queue)
        return 0;
    return queue->maxSizeLimit;
}

int qGetSize(QueueImp* queue){
    if(!queue)
        return 0;
    return queue->size;
}

int qGetMaxSize(QueueImp* queue){
    if(!queue)
        return 0;
    return queue->maxSize;
}

int qGetEmptySlots(QueueImp* queue){
    if(queue->maxSizeLimit){
        return queue->maxSizeLimit - queue->size;
    }else
        return queue->maxSize - queue->size;
}

/* Function qIsFull()
 *    Checks to see if the queue has reached its current capacity limit.  This function
 *    does not account for the queue's ability to resize.
 *
 *    Returns:
 *        1 - The queue is full and cannot accommodate any more elements without resizing.
 *        0 - The queue is not full.
 */
int qIsFull(QueueImp* queue){
    assert(queue);
    if(queue->size == queue->maxSize)
        return 1;
    else
        return 0;
}

/* Function qIsReallyFull()
 *    Checks to see if the queue has reached its resize limit.  This function is similar
 *    to qIsFull() except that it takes the queue's ability to resize into consideration.
 *
 *    Returns:
 *        1 - The queue is full and cannot be resize any further.
 *        0 - The queue is not full.
 */
int qIsReallyFull(QueueImp* queue){
    assert(queue);
    if(queue->size == queue->maxSize && queue->maxSize == queue->maxSizeLimit)
        return 1;
    else
        return 0;
}

int qIsEmpty(QueueImp* queue){
    if(queue->size == 0)
        return 1;
    else
        return 0;
}

void qRandomize(QueueImp* queue){
    int i;

    // The queue cannot be randomized if it is emtpy.
    if(!queue->size)
        return;

    // For each element in the queue,
    for(i = 0; i < queue->size; i++){
        int swapIndex;
        void** curElement;
        void** swapElement;
        void* temp;

        // Randomly choose another element to be swapped with.
        swapIndex = randInt(queue->size);
        
        curElement = qGetWrappedElement(queue, i);
        swapElement = qGetWrappedElement(queue, swapIndex);

        temp = *curElement;
        *curElement = *swapElement;
        *swapElement = temp;
    }
}

static void qDump(QueueImp* queue){
    int i;
    
    printf("Queue begin\n");
    for(i = 0; i < queue->maxSize; i++){
        if(queue->storage[i]){
            printf("Index %i,    Value %i", i, queue->storage[i]);
        }else
            printf("Index %i,    Empty", i);

        if(i == queue->head)
            printf("\thead");
        if(i == queue->tail)
            printf("\ttail");
        printf("\n");
    }
    printf("Queue end\n");
}

static void testQueueFind(QueueImp* queue, void* element){
    if(qFind(queue, element)){
        printf("Found \"%i\"\n", element);
    }else
        printf("\"%i\" not found\n", element);
}

#pragma warning(push)
#pragma warning(disable:4028) // parameter differs from declaration

void qGetIterator(QueueImp* pQueue, QueueIterator* pIter)
{
    pIter->pQueue = pQueue;
    pIter->iIndex = pQueue->head;
    pIter->bBackwards = false;
}

void qGetBackwardsIterator(QueueImp* pQueue, QueueIterator* pIter)
{
    pIter->pQueue = pQueue;
    pIter->iIndex = pQueue->tail-1;
    if ( pIter->iIndex < 0 )
        pIter->iIndex = pQueue->maxSize-1;
    pIter->bBackwards = true;
}

bool qGetNextElement(QueueIterator* pIter, void** ppElem)
{
    QueueImp* pQueue = pIter->pQueue;

    // Make sure the queue is valid
    if (!pQueue || pQueue->size == 0 )
        return false;

    //
    // Look through the queue starting at the head and ending at the tail
    //

    // We set the index to -1 if we've exhausted the queue 
    //
    // Note that we can't just check for the tail here because in a full table, the tail will equal the head
    // so the first index lookup will trigger the end case
    if ( pIter->iIndex < 0 )
        return false;

    // return the current indexed value, since we must be valid if we got this far
    // be sure to return true after this point
    if ( ppElem )
        *ppElem = pQueue->storage[pIter->iIndex];
    assert( pQueue->storage[pIter->iIndex] );

    if ( pIter->bBackwards )
    {
        int iBeforeHead = (pQueue->head-1 >= 0)?pQueue->head-1:pQueue->maxSize-1;
        // increment index mod queue size
        if ( --pIter->iIndex < 0 )
        {
            pIter->iIndex = pQueue->maxSize-1;
        }

        // Once we reach the tail, we're done (next time we return false)
        if ( pIter->iIndex == iBeforeHead )
        {
            pIter->iIndex = -1;
        }
    }
    else
    {
        // increment index mod queue size
        if ( ++pIter->iIndex == pQueue->maxSize )
            pIter->iIndex = 0;

        // Once we reach the tail, we're done (next time we return false)
        if ( pIter->iIndex == pQueue->tail )
        {
            pIter->iIndex = -1;
        }
    }


    return true;
}

#pragma warning(pop)


void testQueue(){
    Queue q;
    
    q = createQueue();
    
    qSetMaxSizeLimit(q, 6);

    printf("Queue created\n");
    qDump(q);
    
    // Test queue initialization + auto-initialization + enqueuing.
    //    Push something into a newly created queue.
    printf("Initialization, auto-initialization, and enqueue test\n");
    printf("Enqueueing \"1\"\n");
    qEnqueue(q, (void*) 1);
    qDump(q);

    // Test dequeuing.
    printf("\nDequeue test\n");
    printf("Dequeueing\n");
    qDequeue(q);
    qDump(q);
    
    // Test queue resizing.
    printf("\nQueue resize test\n");
    printf("Enqueueing \"1\" ~ \"4\"\n");
    qEnqueue(q, (void*) 1);
    qEnqueue(q, (void*) 2);
    qEnqueue(q, (void*) 3);
    qEnqueue(q, (void*) 4);
    qDump(q);
    
    printf("\nEnqueuing \"5\"\n");
    qEnqueue(q, (void*) 5);
    qDump(q);        // The queue should have been resized and reorganized here.

    // Test max size limiter
    printf("\nQueue max size limiter test\n");
    printf("Enqueuing \"6\", \"7\"\n");
    qEnqueue(q, (void*) 6);
    qEnqueue(q, (void*) 7); 
    qDump(q);    // The queue should not have resized again and 7 should not be in the queue.

    // Test queue search
    printf("\nQueue search test\n");
    qDequeue(q);
    qEnqueue(q, (void*) 8);
    qDump(q);
    printf("Looking for \"2\", \"7\", \"8\"\n");
    testQueueFind(q, (void*)2);
    testQueueFind(q, (void*)7);
    testQueueFind(q, (void*)8);
    
    // Test queue randomize
    printf("\nQueue randomize test\n");
    qDump(q);
    qRandomize(q);
    qDump(q);

    qDequeue(q);
    qRandomize(q);
    qDump(q);

    qEnqueue(q, S32_TO_PTR(9));
    qRandomize(q);
    qDump(q);

    // Remove all contents of queue.
    while(qPeek(q)){
        int element;

        element = PTR_TO_S32(qDequeue(q));
        printf("Got %i\n", element);
    }
    destroyQueue(q);
}
