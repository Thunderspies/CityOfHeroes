/* File HashTableStack.c
 *    In this implementation, a HashTableStack is simply an Array.  Most functions
 *    simply call their Array counterparts.
 *
 *    Possible Opacity implementations:
 *    1)    Store a key "Opacity" with a boolean value in every StashTable.
 *        While this implementation is very simple, it will have an impact
 *        on the lookup time.  Everytime the search algorithm attempts to
 *        go one level lower into the stack, it must first search for
 *        the key "Opacity".  This may not be a very big deal since
 *        we are already giving up fast search time for the ability to
 *        override/restore values.
 *    2)    Adding another field into the actual StashTable structure.
 *        It is possible to simply augment the current StashTable implementation
 *        to support "Opacity".  But that doesn't seem like a very clean
 *        implementation.  What does it mean to turn a standalone StashTable
 *        "opaque"?
 *
 *    There are really no implementation difficulties in adding the "opacity" feature.
 *    But since it is not needed right now, I'll wait until either a better solution
 *    when a need for the feature shows up.
 */

#include "utilitieslib/components/HashTableStack.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/components/StashTable.h"

HashTableStack createHashTableStack(){
    return createArray();
}

void initHashTableStack(Array* stack, unsigned int size){
    initArray(stack, size);
}

void stashTableDestroyStack(void* stackData){
    Array* stack = (Array*)stackData;
    destroyArray(stack);
}

static void stashTableDestroyCallback(void* arg0)
{
    stashTableDestroy((StashTable)arg0);
}

void stashTableDestroyStackEx(Array* stack){    
    destroyArrayEx(stack, stashTableDestroyCallback);
}

void stashTableClearStack(Array* stack){
    clearArray(stack);
}

StashElement hashStackFindElement(Array* stack, const char* key){
    int i;
    StashTable table = 0;
    StashElement element = 0;
    assert(stack && key);

    // Try all hashtables stored in the stack
    for(i = stack->size - 1; i > -1; i--){

        // Get to table to be used for this iteration.
        table = stack->storage[i];

        // Try to find the specified key in the current table.
        stashFindElement(table, key, &element);

        // Return the element if the one with the corresponding key is found.
        if(element)
            return element;
    }

    // None of the hashtables in the stack has a match for the specified key,
    // the element cannot be found.
    return 0;
}

// Adds a key/value pair to the top hash tables in the stack.
StashElement hashStackAddElement(Array* stack, const char* key, void* value){
    StashTable table = arrayGetBack(stack);
    StashElement elem = NULL;
    if(table)
        stashAddPointerAndGetElement(table, key, value, false, &elem);
    return elem;
}

void* hashStackFindValue(Array* stack, const char* key){
    StashElement element = hashStackFindElement(stack, key);

    if(element)
        return stashElementGetPointer(element);
    else
        return 0;
}


void hashStackPushTable(Array* stack, StashTable table){
    arrayPushBack(stack, table);
}

StashTable hashStackPopTable(Array* stack){
    StashTable table = arrayGetBack(stack);
    arrayPopBack(stack);
    return table;
}

StashTable hashStackGetTopTable(Array* stack){
    return arrayGetBack(stack);
}

int hashStackGetSize(Array* stack){
    return stack->size;
}

int hashStackGetMaxSize(Array* stack){
    return stack->maxSize;
}

void hashStackCopy(Array* src, Array* dst){
    arrayCopy(src, dst, NULL);
}
