#ifndef ARRAY_H
#define ARRAY_H

#include "../utils/memcheck.h"

typedef struct Array{
    int size;        // current array size
    int maxSize;    // maximum allowed size
    void** storage;    // actual array
} Array;

/****************************************************************************
 * Array related Functions
 *
 */
typedef void (*Destructor)(void*);
typedef void (*ItemCopy)(void**, void**);

Array* createArray();
#define initArray(array, size) initArray_dbg(array, size MEM_DBG_PARMS_INIT)
void initArray_dbg(Array* array, int size MEM_DBG_PARMS);
#define arrayPushBack(array, element) arrayPushBack_dbg(array, element MEM_DBG_PARMS_INIT)
void arrayPushBack_dbg(Array* array, void* element MEM_DBG_PARMS);
void* arrayPopBack(Array* array);
void* arrayGetBack(Array* array);
void arraySetAt(Array* array, int arrayIndex, void* element);
#define resizeArray(array, newSize) resizeArray_dbg(array, newSize MEM_DBG_PARMS_INIT)
void resizeArray_dbg(Array* array, int newSize MEM_DBG_PARMS);
int arrayIsFull(Array* array);
#define arrayCopy(source, destination, func) arrayCopy_dbg(source, destination, func MEM_DBG_PARMS_INIT)
void arrayCopy_dbg(Array* source, Array* destination, ItemCopy func MEM_DBG_PARMS);
void clearArray(Array* array);
void clearArrayEx(Array* array, Destructor func);
void destroyArrayPartial(Array* array);
void destroyArrayPartialEx(Array* array, Destructor func);
void destroyArray(Array* array);  // When array does not take ownership of the stored contents
void destroyArrayEx(Array* array, Destructor func);    // When array does take ownership of the stored contents
void arrayCompact(Array* array);
void arrayCompactStable(Array* array);
void arrayReverse(Array* array);
void arraySwap(Array* array1, Array* array2);
void arrayInsert(Array* array, unsigned int i, void* element);
void arrayRemoveAndFill(Array* array, unsigned int i);
void arrayRemoveAndShift(Array* array, unsigned int i);
int arrayFindElement(Array* array, void* element);

typedef void(*ArrayItemProcessor)(void* member, void* param);
void arrayForEachItem(Array* array, ArrayItemProcessor func, void* param);
void** arrayGetNextEmptySlot(Array* array, int* i);
void** arrayGetNextItem(Array* array, int* i);
void** arrayGetLastItem(Array* array, int* i);
void arrayAddElementToEmptySlot(Array* array, void* element);

/****************************************************************************
 * Heap/Priority Queue related Functions
 *    These functions sorts the array so that the highest rated item is on the
 *    top of the heap.
 */

/* Comparison operator
 *    Returns:
 *        -1    - if item2 should be the parent of item1
 *        0    - if item1 and item2 are equal
 *        1    - if item1 should be the parent of item2
 *     
 */
typedef int (*QueueCompare)(void* item1, void* item2);
typedef void (*QueueIndexUpdate)(void* item, int index);

void pqPercolateUp(Array* heap, int percTarget, QueueCompare comp, QueueIndexUpdate indexUpdate);
void heapify(Array* queue, QueueCompare comp, QueueIndexUpdate indexUpdate);
void pqPush(Array* queue, void* item, QueueCompare comp, QueueIndexUpdate indexUpdate);
void* pqPop(Array* queue, QueueCompare comp, QueueIndexUpdate indexUpdate);

void testHeap(void);

/****************************************************************************
 * Linked Array related Functions
 *    A simple hack that allows two arrays access to elements of two arrays
 *    using a single index.  A full blown implementation should allow
 *    an arbitrary number of arrays to be linked together.  Currently, the LinkedArray
 *    cheats by providing enough "links" to be useful to the floating beacon
 *    A* search only.
 */
typedef struct LinkedArray{
    int size;        // current array size
    int maxSize;    // maximum allowed size
    Array* array1;
    Array* array2;
} LinkedArray;

void initLArray(LinkedArray* array);
void larrayAddArray(LinkedArray* array, Array* element);
void* larrayGetAt(LinkedArray* array, int index);

#endif

