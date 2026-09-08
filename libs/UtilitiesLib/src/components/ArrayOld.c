#include "utilitieslib/components/ArrayOld.h"

#include <stdlib.h>
#include <memory.h>
#include "utilitieslib/assert/assert.h"
#include <string.h>
//#include "Common.h"

#include "utilitieslib/components/MemoryPool.h"

#include "utilitieslib/stdtypes.h"

MP_DEFINE(Array);

Array* createArray(){
/*    Array* array = malloc(sizeof(Array));
    if(array)
        memset(array, 0, sizeof(Array)); 
    return array;
    */
    MP_CREATE(Array, 20);
    return MP_ALLOC(Array);
};

void initArray_dbg(Array* array, int size MEM_DBG_PARMS){
    if(size > 0){
        array->storage = scalloc(size, sizeof(void*));
        assert(array->storage);
    }else{
        array->storage = NULL;
    }

    array->maxSize = size;
    array->size = 0;
}
    
void arrayPushBack_dbg(Array* array, void* element MEM_DBG_PARMS){
    if(0 == array->maxSize)
        initArray_dbg(array, 4 MEM_DBG_PARMS_CALL);

    if(array->size == array->maxSize){
        resizeArray_dbg(array, array->maxSize * 2 MEM_DBG_PARMS_CALL);
    }
    
    array->storage[array->size++] = element;
}

void arrayAddElementToEmptySlot(Array* array, void* element){
    // Get the first empty slot
    void** slot = arrayGetNextEmptySlot(array, NULL);

    
    if(!slot){
        // If there are no empty slots, push the new element into
        // the array.  The array will automatically resize if
        // needed.
        arrayPushBack(array, element);
    }else{
        // If there is an empty slot, fill it.
        *slot = element;
    }
}

void* arrayPopBack(Array* array){
    void* backPtr;

    // if there is nothing to pop...
    if(0 == array->size)
        return NULL;

    backPtr = array->storage[array->size - 1];

    array->storage[--array->size] = NULL;
    return backPtr;
}

void* arrayGetBack(Array* array){
    if(0 == array->size)
        return NULL;
    return array->storage[array->size - 1];
}

void arraySetAt(Array* array, int arrayIndex, void* element){
    if(arrayIndex > array->maxSize)
        assert(0);
    
    array->storage[arrayIndex] = element;
    if (arrayIndex >= array->size)
        array->size++;
}

void resizeArray_dbg(Array* array, int newSize MEM_DBG_PARMS){
    array->storage = (void**) srealloc(array->storage, newSize * sizeof(void*));

    // Zero out the new allocated space
    memset(&array->storage[array->size], 0, (newSize - array->size) * sizeof(void*));

    // Update array size
    array->maxSize = newSize;
}

int arrayIsFull(Array* array){
    if(array->size == array->maxSize)
        return 1;
    else
        return 0;
}

/* Function arrayCopy
 *    Copies the content of the source array into the destination array.  If
 *    a copy function is supplied, the function will be used to perform a
 *    deep copy of the items.  Otherwise, a shallow copy will be performed.
 *
 *
 */
void arrayCopy_dbg(Array* source, Array* destination, ItemCopy func MEM_DBG_PARMS){
    int i;

    // If the destination array is not large enough, resize it.
    if(destination->maxSize < source->size){
        resizeArray_dbg(destination, source->size MEM_DBG_PARMS_CALL);
    }

    if(NULL == func)
        memcpy(destination->storage, source->storage, sizeof(void*) * source->size);
    else
    {
        for(i = 0; i < source->size; i++){
            func(&source->storage[i], &destination->storage[i]);
        }
    }

    destination->size = source->size;
}

// Clean out array contents without releasing anying memory used by the array
void clearArray(Array* array){
    array->size = 0;
}

// Destroys array contents without releasing anying memory used by the array
void clearArrayEx(Array* array, Destructor func){
    int i;
    if(!array)
        return;
    
    if(func == NULL)
        // step through the entire array and destroy all valid items using free()
        for(i = 0; i < array->size; i++){
            if(array->storage[i]) {
                free(array->storage[i]);
            }
            array->storage[i] = NULL;
        }
        
    else
        // step through the entire array and destroy all valid items using the
        // specified destructor
        for(i = 0; i < array->size; i++){
            if(array->storage[i]) {
                func(array->storage[i]);
            }
            array->storage[i] = NULL;
        }

    array->size = 0;
}

// Clears array contents, then free any memory held by the array, but not the array itself
void destroyArrayPartial(Array* array){
    if(array->storage){
        clearArray(array);
        free(array->storage);
    }
    memset(array, 0, sizeof(Array));
}

// Destroys array contents, then free any memory held by the array, but not the array itself
void destroyArrayPartialEx(Array* array, Destructor func){
    if(array->storage){
        clearArrayEx(array, func);
        free(array->storage);
    }
    memset(array, 0, sizeof(Array));
}

// Clears array contents, then free any memory held by the array, including the array itself
void destroyArray(Array* array){
    if(array){
        destroyArrayPartial(array);
        MP_FREE(Array, array);
    }
}


/* Function destroyArrayEx
 *    This function destroys the given array using the specified function
 *    on each element of the array.  The function then destroys the array itself.
 *
 */
void destroyArrayEx(Array* array, Destructor func){
    if(array){
        destroyArrayPartialEx(array, func);
        MP_FREE(Array, array);
    }
}

void arrayCompact(Array* array){
    void** lastNonEmptySlot;
    void** firstEmptySlot;
    int lastNonEmptyIndex = array->size;
    int firstEmptyIndex = -1;

    while(1){
        // Find the first open slot in the array
        firstEmptySlot = arrayGetNextEmptySlot(array, &firstEmptyIndex);

        // Move the last item in the array into the empty slot
        lastNonEmptySlot = arrayGetLastItem(array, &lastNonEmptyIndex);

        // Stop when the entire array has been processed.  This is true when
        // the "tail" pointer is now in front of the "head" pointer.
        if(lastNonEmptyIndex <= firstEmptyIndex){
            break;
        }

        *firstEmptySlot = *lastNonEmptySlot;
        *lastNonEmptySlot = NULL;
    }

    // update array size
    array->size = lastNonEmptyIndex + 1;
}

void arrayCompactStable(Array* array){
    int readIndex, writeIndex;

    for (readIndex=writeIndex=0; readIndex<array->size; readIndex++) {
        if (array->storage[readIndex]) {
            if (readIndex!=writeIndex) {
                array->storage[writeIndex]=array->storage[readIndex];
            }
            writeIndex++;
        }
    }
    array->size = writeIndex;
}

void arrayReverse(Array* array){
    void** head = array->storage;
    void** tail = head + array->size - 1;

    // Start the index at the two "outer most" elements of the array.
    // Each interation through the loop, swap the two elements that are indexed,
    // then bring the indices one step closer to each other.
    for(; head < tail; head++, tail--){
        void* tempBuffer = *head;
        *head = *tail;
        *tail = tempBuffer;
    }
}

void arraySwap(Array* array1, Array* array2){
    Array temp;

    assert(array1 && array2);
    temp = *array1;
    *array1 = *array2;
    *array2 = temp;
}

/* Function arrayInsert()
 *    Inserts a new element at the specified location in the array.  If the
 *    specified location points beyond the current size of the array the 
 *    specified element will be added to the back of the array.
 *
 *
 */
void arrayInsert(Array* array, unsigned int insertIdx, void* element){
    int i;
    assert(array && element);


    // Add the specified element to the back of the array to ensure that
    // there is enough room to hold this new element.
    arrayPushBack(array, element);

    // If the index points beyond the current size of the array, the correct
    // behavior is to add the element to the back of the array, which is
    // already done.
    if(array->size <= (int)insertIdx){
        return;
    }

    for(i = array->size; i > (int)insertIdx; i--){
        array->storage[i] = array->storage[i - 1];
    }
    array->storage[insertIdx]=element;
}

void arrayRemoveAndFill(Array* array, unsigned int i){
    assert(array);
    assert(i < (unsigned int)array->size);

    // If an invalid index was specified, simply ignore the remove request.
    // Since the element does not exist, it is already "removed".
    if(i >= (unsigned int)array->size)
        return;

    // Move the last element to the specified location.  The specified element
    // is effectively removed from the array.  At the same time, the empty slot
    // is now fill by the last element in the array.
    array->storage[i] = array->storage[array->size - 1];
    arrayPopBack(array);
}

void arrayRemoveAndShift(Array* array, unsigned int i){
    assert(array);
    assert(i < (unsigned int)array->size);

    // If an invalid index was specified, simply ignore the remove request.
    // Since the element does not exist, it is already "removed".
    if(i >= (unsigned int)array->size)
        return;

    if (array->size - i - 1) {
        memmove(&array->storage[i], &array->storage[i+1], sizeof(array->storage[i])*(array->size - i - 1));
    }
    --array->size;
}

/*************************************************************************
 * Begin Array "iterator" functions 
 *
 *
 */

/* Function arrayForEachItem
 *    Sends each non-null item in the array to the ArrayItemProcessor.
 *    
 *    Parameters:
 *        array - the array to walk through
 *        func - the ArrayItemProcessor to call when a non-null item is found
 *        param - an additional parameter to pass back to the ArrayItemProcessor
 *
 */
void arrayForEachItem(Array* array, ArrayItemProcessor func, void* param){
    int i;
    for(i = 0; i < array->size; i++){
        if(NULL != array->storage[i])
            func(array->storage[i], param);
    }
}



/* Function arrayGetNextEmtpySlot
 *    Returns to the first pointer in the array after the index i that is NULL.
 *    If the i points to NULL, the search begins at the first emtpy slot.
 *
 */
void** arrayGetNextEmptySlot(Array* array, int* i){
    int iBuffer = 0;
    if(NULL == i)
        i = &iBuffer;
    else
        (*i)++;
    
    for(;*i < array->size; (*i)++){
        if(NULL == array->storage[*i])
            return &array->storage[*i];
    }

    return NULL;
}



/* Function arrayGetNextEmtpySlot
 *    Returns to the first pointer in the array after the index i that is non-NULL.
 *    If the i points to NULL, the search begins at the first emtpy slot.
 *
 */
void** arrayGetNextItem(Array* array, int* i){
    int iBuffer = 0;
    if(NULL == i)
        i = &iBuffer;
    else
        (*i)++;

    for(;*i < array->size; (*i)++){
        if(NULL != array->storage[*i])
            return &array->storage[*i];
    }
    
    return NULL;
}



/* Function arrayGetLastItem
 *    Performs a linear search of the array in reverse.  Returns to the first 
 *    pointer in the array before the index i that is NULL.  If the i points 
 *    to NULL, the search begins at the end of the array.
 *
 */
void** arrayGetLastItem(Array* array, int* i){
    int iBuffer = array->size;
    if(NULL == i)
        i = &iBuffer;
    else
        (*i)--;

    for(;*i >= 0; (*i)--){
        if(NULL != array->storage[*i])
            return &array->storage[*i];
    }
    
    return NULL;
}

/*
 *
 *
 * End Array "iterator" functions 
 *************************************************************************/



int arrayFindElement(Array* array, void* element){
    int i;
    for(i = 0; i < array->size; i++){
        if(element == array->storage[i])
            return i;
    }
    return -1;
}


/*************************************************************************
 * Begin Heap manipuation functions
 *
 *
 */


/* Function percolateDown()
 *    Moves the percolate target down in the heap until heap order is restored.
 *
 *    Parameters:
 *        heap - the heap to operate on.
 *        percTarget - the index of the element in the heap to be moved.
 *        comp - a comparision function for comparing "rankings" between elements.
 *
 */
static void percolateDown(Array* heap, int percTarget, QueueCompare comp, QueueIndexUpdate indexUpdate){
    int parent, lchild, rchild, swpTarget;
    void* swpBuffer;
    void* targetValue;
    void** storage = heap->storage;
    int size = heap->size;

    // can't use a target that's greater than the heap size
    if(percTarget > size)
        return;
        
    parent = percTarget;
    targetValue = storage[parent];

    while(1){
        // Consider the larger child as a possible swap target.

        // If the child is larger, the heap order has been 
        // violated.  Restore heap order by swapping
        // the child with the parent.

        // get left child index
        lchild = parent << 1;

        // get right child index
        rchild = lchild + 1;

        // decide which child might be the possible swap target
        //    The current parent node has no children.  Percolate down
        //    completed.
        if(lchild <= size){
            //    If the right child doesn't exist, the only possible swap
            //    target is the left child.
            if(rchild > size)
                swpTarget = lchild;
            else
                // otherwise, the swap target should be the larger of the
                // two child nodes
                swpTarget = (-1 == comp(storage[lchild], storage[rchild]) ? rchild : lchild);


            // if the heap order is violated
            if(-1 == comp(targetValue, storage[swpTarget])){
                // swap the parent and the child
                swpBuffer = storage[swpTarget];
                storage[parent] = swpBuffer;
                
                if(indexUpdate){
                    indexUpdate(swpBuffer, parent);
                }

                // update new perc target
                parent = swpTarget;
                
                continue;
            }
        }

        // heap is okay, so put the target value in and we're done.
        storage[parent] = targetValue;
        
        if(indexUpdate){
            indexUpdate(targetValue, parent);
        }
    
        // heap order not violated... order restored...
        return;
    }
}



void pqPercolateUp(Array* heap, int percTarget, QueueCompare comp, QueueIndexUpdate indexUpdate){
    int parent;
    void* swpBuffer;
    void* targetValue;
    void** storage = heap->storage;
    int size = heap->size;

    // can't use a target that's greater than the heap size
    if(percTarget >= size)
        return;

    targetValue = storage[percTarget];

    while(1){
        // Compare the current perc target against its parent.
        // If the parent is smaller, heap order has been violated.
        // Restore the order by swapping the target and its parent.

        // Stop further processing if the top of the heap has been reached.
        if(1 != percTarget){
            parent = percTarget >> 1;
            
            if(-1 == comp(storage[parent], targetValue)){
                // swap the parent and the target
                swpBuffer = storage[parent];
                storage[percTarget] = swpBuffer;
                
                if(indexUpdate){
                    indexUpdate(swpBuffer, percTarget);
                }
                
                // update new perc target
                percTarget = parent;
                
                continue;
            }
        }

        // heap is okay, so put the target value in and we're done.
        storage[percTarget] = targetValue;
        
        if(indexUpdate){
            indexUpdate(targetValue, percTarget);
        }
    
        // heap order not violated... order restored...
        
        return;
    }
}


/* Function heapify()
 *    Instates heap order.  Given an array and a function to compare the elements, this 
 *    function sorts the array so that the parents nodes are always "greater" than
 *    its children.
 *
 *    Note that it is also possible to reverse the heap order by passing this function
 *    a comparision function that returns the reversed rankings.
 */
void heapify(Array* queue, QueueCompare comp, QueueIndexUpdate indexUpdate){
    int i;

    // Starting at then n - 1 level deep and working up the
    // tree, check all child nodes for correct heap order.
    for(i = queue->size >> 1 ; i > 0; i--){
        percolateDown(queue, i, comp, indexUpdate);
    }
}

void pqPush(Array* queue, void* item, QueueCompare comp, QueueIndexUpdate indexUpdate){
    // Push dummy value into element 0.  It is never used as part of the heap.
    if(0 == queue->size)
        arrayPushBack(queue, NULL);

    arrayPushBack(queue, item);
    pqPercolateUp(queue, queue->size - 1, comp, indexUpdate);
}


void* pqPop(Array* queue, QueueCompare comp, QueueIndexUpdate indexUpdate){
    void* topItem;

    // If there is nothing but the dummy element in the queue, return nothing...
    if(queue->size < 1)
        return NULL;

    // remove the item with highest ranking
    topItem = queue->storage[1];

    // move the item with the lowest ranking to the top
    queue->storage[1] = queue->storage[--queue->size];

    // restore heap order by perculating the lowest ranking item down
    percolateDown(queue, 1, comp, indexUpdate);
    return topItem;
}






/************************************************
 * Heap test related
 */
#pragma warning(push)
#pragma warning(disable: 4022) // pointer mismatch error for putting int in place of void*

#include <stdio.h>

static int intCompare(void *item1, void *item2){
    int i1 = (int)(intptr_t)item1;
    int i2 = (int)(intptr_t)item2;
    if(i1 < i2)
        return -1;
    
    if(i1 == i2)
        return 0;
    
    return 1;
}

void testHeap(void){
    int i;
    Array* array;
    void* poppedItem;
    
    
    // setup an array to be used as a pqueue
    array = createArray();
    initArray(array, 11);
    arrayPushBack(array, (void*)(intptr_t)999);
    for(i = 1; i < array->maxSize; i++){
        arrayPushBack(array, (void*)(intptr_t)i);
    }
    
    
    
    printf("Before heapifying, the array looks like: ");
    for(i = 0; i < array->size; i++){
        printf("%i ", array->storage[i]);
    }
    printf("\n");
    
    
    heapify(array, intCompare, NULL);
    
    
    printf("After heapifying, the array looks like: ");
    for(i = 0; i < array->size; i++){
        printf("%i ", array->storage[i]);
    }
    printf("\n");
    
    pqPush(array, (void*)(intptr_t)11, intCompare, NULL);
    printf("After insertion, the array looks like: ");
    for(i = 0; i < array->size; i++){
        printf("%i ", array->storage[i]);
    }
    printf("\n");
    
    poppedItem = pqPop(array, intCompare, NULL);
    printf("After extraction, the array looks like: ");
    for(i = 0; i < array->size; i++){
        printf("%i ", array->storage[i]);
    }
    printf("\n");
}
#pragma warning(pop)


/*
 * 
 *
 * Begin Heap manipuation functions
 *************************************************************************/

void initLArray(LinkedArray* array){
    memset(array, 0, sizeof(LinkedArray));
}

void larrayAddArray(LinkedArray* array, Array* element){
    if(!array->array1)
        array->array1 = element;
    else
        array->array2 = element;

    array->size += element->size;
    array->maxSize += element->maxSize;
}

void* larrayGetAt(LinkedArray* array, int index){
    // If the index specifies an element in the first array, return the element
    if(index < array->array1->size)
        return array->array1->storage[index];
    
    // Otherwise, the index specifies an element in the 2nd array.  Turn the
    // given index into something usable for the 2nd array, the return the element.
    index -= array->array1->size;
    if(index < array->array2->size)
        return array->array2->storage[index];
    

    // The index doesn't point to any elements tracked by the two arrays...
    return NULL;
}

