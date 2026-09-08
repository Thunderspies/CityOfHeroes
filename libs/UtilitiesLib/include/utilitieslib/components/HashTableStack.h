/* File HashTableStack
 *    Provides a interface that is similar to a regular StashTable.  Best case key lookup
 *    time is O(1).  Worst case key looup time is O(n), where n is the number of StashTables
 *    on the stack.
 *
 *    The main purpose of a HashTableStack is to override the value of a set of keys temporarily 
 *    while retaining the ability to find all non-overriden keys.  To override keys, create
 *    another hashtable that has the new associations and push it onto the stack.  All subsequent
 *    queries on those keys will return the overriden value.  To discard the overriden value,
 *    simply pop off the hashtable with the new associations.
 *
 *
 *    Creating a HashTableStack:
 *        Call createHashTableStack to create an instance of HashTableStack.  The HashTableStack
 *        must then be initialized by calling initHashTableStack to set the initial stack size.  Note
 *        that the stack will automatically resize.
 *
 *    Adding a key/value pair:
 *        Use hashStackGetTopTable() to get the table at the top of the stack, then add associations
 *        using functions in the StashTable module.
 *
 *    Looking up a key/value pair:
 *        Use the hashStackFindElement() or the hashStackFindValue() function.  They behave in the
 *        mostly the same manner as hashFindElement() and stashFindPointerReturnPointer() found in 
 *        StashTable.h.  A search for the key will performed on the entire stack of StashTables
 *        starting from the one on the top of the stack.  The element associated with
 *        the hashtable nearest to the stack top will be returned.
 *
 *    Defining StashTable "opacity":
 *        Usually, when a key is not found in a StashTable on an upper level of the stack,
 *        the search continues in lower level StashTables.  The StashTable opacity would
 *        dictate if a search "down the stack" is permitted.  In this way, a StashTable
 *        can block out all keys defined by lower level StashTables.  This functionality
 *        is not yet implemented.
 */


#ifndef HASHTABLESTACK_H
#define HASHTABLESTACK_H

#include "ArrayOld.h"

typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef struct StashElementImp *StashElement;
typedef struct Array *HashTableStack;

// constructor/destructors
HashTableStack createHashTableStack();
void initHashTableStack(HashTableStack stack, unsigned int size);
void stashTableDestroyStack(void* stackData);
void stashTableDestroyStackEx(HashTableStack stack);
void stashTableClearStack(HashTableStack stack);

// element insertion/lookup
StashElement hashStackAddElement(HashTableStack stack, const char* key, void* value);
StashElement hashStackFindElement(HashTableStack stack, const char* key);
void* hashStackFindValue(HashTableStack stack, const char* key);

void hashStackPushTable(HashTableStack stack, StashTable table);
StashTable hashStackPopTable(HashTableStack stack);
StashTable hashStackGetTopTable(HashTableStack stack);

int hashStackGetSize(HashTableStack stack);
int hashStackGetMaxSize(HashTableStack stack);

void hashStackCopy(HashTableStack src, HashTableStack dst);

// Opacity info is not yet implemented...
//int sHashIsOpaque(HashTableStack table);
//void sHashSetOpaque(HashTableStack table, bool value);

void testHashTableStack();

#endif
