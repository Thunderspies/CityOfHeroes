#include "utilitieslib/network/fingerprint.h"
#include <stdlib.h>
#include <memory.h>
#include "utilitieslib/assert/assert.h"
#include <stdio.h>
#include "utilitieslib/stdtypes.h"

// Ripped from glib.
// Visit http://www.geocities.com/primes_r_us/ and produce a new table
// if this is not suitable.
static const unsigned int primes[] =
{
  11,
  19,
  37,
  73,
  109,
  163,
  251,
  367,
  557,
  823,
  1237,
  1861,
  2777,
  4177,
  6247,
  9371,
  14057,
  21089,
  31627,
  47431,
  71143,
  106721,
  160073,
  240101,
  360163,
  540217,
  810343,
  1215497,
  1823231,
  2734867,
  4102283,
  6153409,
  9230113,
  13845163,
};

static const unsigned short randomValue[256] =
{
  0xbcd1, 0xbb65, 0x42c2, 0xdffe, 0x9666, 0x431b, 0x8504, 0xeb46,
  0x6379, 0xd460, 0xcf14, 0x53cf, 0xdb51, 0xdb08, 0x12c8, 0xf602,
  0xe766, 0x2394, 0x250d, 0xdcbb, 0xa678, 0x02af, 0xa5c6, 0x7ea6,
  0xb645, 0xcb4d, 0xc44b, 0xe5dc, 0x9fe6, 0x5b5c, 0x35f5, 0x701a,
  0x220f, 0x6c38, 0x1a56, 0x4ca3, 0xffc6, 0xb152, 0x8d61, 0x7a58,
  0x9025, 0x8b3d, 0xbf0f, 0x95a3, 0xe5f4, 0xc127, 0x3bed, 0x320b,
  0xb7f3, 0x6054, 0x333c, 0xd383, 0x8154, 0x5242, 0x4e0d, 0x0a94,
  0x7028, 0x8689, 0x3a22, 0x0980, 0x1847, 0xb0f1, 0x9b5c, 0x4176,
  0xb858, 0xd542, 0x1f6c, 0x2497, 0x6a5a, 0x9fa9, 0x8c5a, 0x7743,
  0xa8a9, 0x9a02, 0x4918, 0x438c, 0xc388, 0x9e2b, 0x4cad, 0x01b6,
  0xab19, 0xf777, 0x365f, 0x1eb2, 0x091e, 0x7bf8, 0x7a8e, 0x5227,
  0xeab1, 0x2074, 0x4523, 0xe781, 0x01a3, 0x163d, 0x3b2e, 0x287d,
  0x5e7f, 0xa063, 0xb134, 0x8fae, 0x5e8e, 0xb7b7, 0x4548, 0x1f5a,
  0xfa56, 0x7a24, 0x900f, 0x42dc, 0xcc69, 0x02a0, 0x0b22, 0xdb31,
  0x71fe, 0x0c7d, 0x1732, 0x1159, 0xcb09, 0xe1d2, 0x1351, 0x52e9,
  0xf536, 0x5a4f, 0xc316, 0x6bf9, 0x8994, 0xb774, 0x5f3e, 0xf6d6,
  0x3a61, 0xf82c, 0xcc22, 0x9d06, 0x299c, 0x09e5, 0x1eec, 0x514f,
  0x8d53, 0xa650, 0x5c6e, 0xc577, 0x7958, 0x71ac, 0x8916, 0x9b4f,
  0x2c09, 0x5211, 0xf6d8, 0xcaaa, 0xf7ef, 0x287f, 0x7a94, 0xab49,
  0xfa2c, 0x7222, 0xe457, 0xd71a, 0x00c3, 0x1a76, 0xe98c, 0xc037,
  0x8208, 0x5c2d, 0xdfda, 0xe5f5, 0x0b45, 0x15ce, 0x8a7e, 0xfcad,
  0xaa2d, 0x4b5c, 0xd42e, 0xb251, 0x907e, 0x9a47, 0xc9a6, 0xd93f,
  0x085e, 0x35ce, 0xa153, 0x7e7b, 0x9f0b, 0x25aa, 0x5d9f, 0xc04d,
  0x8a0e, 0x2875, 0x4a1c, 0x295f, 0x1393, 0xf760, 0x9178, 0x0f5b,
  0xfa7d, 0x83b4, 0x2082, 0x721d, 0x6462, 0x0368, 0x67e2, 0x8624,
  0x194d, 0x22f6, 0x78fb, 0x6791, 0xb238, 0xb332, 0x7276, 0xf272,
  0x47ec, 0x4504, 0xa961, 0x9fc8, 0x3fdc, 0xb413, 0x007a, 0x0806,
  0x7458, 0x95c6, 0xccaa, 0x18d6, 0xe2ae, 0x1b06, 0xf3f6, 0x5050,
  0xc8e8, 0xf4ac, 0xc04c, 0xf41c, 0x992f, 0xae44, 0x5f1b, 0x1113,
  0x1738, 0xd9a8, 0x19ea, 0x2d33, 0x9698, 0x2fe9, 0x323f, 0xcde2,
  0x6d71, 0xe37d, 0xb697, 0x2c4f, 0x4373, 0x9102, 0x075d, 0x8e25,
  0x1672, 0xec28, 0x6acb, 0x86cc, 0x186e, 0x9414, 0xd674, 0xd1a5
};


static int checksum(const unsigned char* buf, int matchSize)
{
  int i = matchSize;
  unsigned short low  = 0;
  unsigned short high = 0;

  for (; i != 0; i -= 1)
  {
      low  += randomValue[*buf++];
      high += low;
  }

  return high << 16 | low;
}

int checksumInit(SimpleHashTable* table,const unsigned char* buf, int size)
{
    int        i;

    table->checksum_low = table->checksum_high = 0;
    for (i=0;i<size;i++)
    {
        table->checksum_low  += randomValue[buf[i]];
        table->checksum_high += table->checksum_low;
    }
    return table->checksum_high << 16 | table->checksum_low;
}

int checksumRoll(SimpleHashTable* table,const unsigned char* buf,int size)
{
    table->checksum_low = table->checksum_low - randomValue[buf[-1]] + randomValue[buf[size-1]];
    table->checksum_high = table->checksum_high - randomValue[buf[-1]] * size + table->checksum_low;
    return table->checksum_high << 16 | table->checksum_low;
}

static unsigned int determineRealTableSize(unsigned int requestedSize){
  int i;

  for(i = 0; i < sizeof(primes) / sizeof(unsigned int); i++){
    if (primes[i] > requestedSize)
      return primes[i];
  }

  return primes[sizeof(primes) / sizeof(unsigned int) - 1];
}

SimpleHashTable* createSimpleHashTable(){
    return calloc(1, sizeof(SimpleHashTable));
}

void destroySimpleHashTable(SimpleHashTable* table){
    if(table){
        if(table->collisionTable)
            destroySimpleHashTable(table->collisionTable);

        free(table->storage);
        free(table);
    }
}

void initSimpleHashTable(SimpleHashTable* table, unsigned int tableSize, unsigned int minMatchSize){
    table->maxSize = determineRealTableSize(tableSize);
    table->storage = calloc(table->maxSize, sizeof(SimpleHashElement));
    table->minMatchSize = minMatchSize;
}

static void lazyInitChildHashTable(SimpleHashTable* parent, SimpleHashTable** childPtr, int minMatchSize){
    SimpleHashTable* child;
    if(!*childPtr){
        child = createSimpleHashTable();
        initSimpleHashTable(child, parent->maxSize / 4, minMatchSize);
        child->recursiveDepth = parent->recursiveDepth + 1;
        *childPtr = child;
    }
}

// Returns whether the new entry was added successfully.
// The default behavior when a slot conflict is found is to discard the new entry.
int sHashAddElement(SimpleHashTable* table, void* key, unsigned int maxKeySize, void* value){
    int index = checksum(key, maxKeySize < table->minMatchSize ? maxKeySize : table->minMatchSize) % table->maxSize;

    table->insertionAttemptCount++;
    table->storage[index].hitCount++;
    
    if(table->minMatchSize > maxKeySize){
        return 0;
    }

    // Add the key and value to the hash table if there is nothing residing in the determined
    // spot.
    if(table->storage[index].key == 0 && table->storage[index].value == 0){
        table->size++;
        table->storage[index].key = key;
        table->storage[index].value = value;
        return table->minMatchSize;
    }else{
        // Otherwise, there has been some sort of conflict.
        table->insertionSlotConflict++;
        table->storage[index].hadCollision = 1;

        //    Set an arbitrary table depth so that the algorithm doesn't break the computer.

        // Do the contents of the conflicting keys match?
        lazyInitChildHashTable(table, &table->collisionTable, table->minMatchSize * 2);
        if(table->recursiveDepth >= 6)
            return 0;
        return sHashAddElement(table->collisionTable, key, maxKeySize, value);
    }
}

SimpleHashElement* sHashFindElementByHash(SimpleHashTable* table, void* key, unsigned int hash)
{
    int index = hash % table->maxSize;

    if(!table->storage[index].hadCollision)
    {
        if(table->storage[index].key && memcmp(table->storage[index].key, key, table->minMatchSize) == 0)
            return &table->storage[index];
        else
            return NULL;
    }
    else
    {
        SimpleHashElement* element;
        element = sHashFindElementByHash(table->collisionTable, key, hash);

        if(element && memcmp(element->key, key, table->collisionTable->minMatchSize) == 0)
            return element;
        else
            return &table->storage[index];
    }
}

void fingerPrintStats(SimpleHashTable *FingerPrintTable)
{
    //printf("min match size = %i\n", minMatchSize);
    printf("FingerPrintTable had:");
    printf("\n\t%i conflicts\n\t%i insertion attempts", FingerPrintTable->insertionSlotConflict, FingerPrintTable->insertionAttemptCount);
    printf("\n\t%i key collisions\n\t%i duplicate values", FingerPrintTable->insertionKeyCollisions, FingerPrintTable->insertionDuplicateValues);
    {
        int depthCounter = 0;
        SimpleHashTable* table = FingerPrintTable;
        while(1){
            printf("\n\tDepth %i: %i/%i used", depthCounter, table->size, table->maxSize);
            if(table->collisionTable){
                table = table->collisionTable;
                depthCounter++;
            }else
                break;
            
        }
    }
}
