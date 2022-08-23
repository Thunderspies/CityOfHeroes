// bitfield.h - functions to deal with integer arrays of bits

#ifndef __BITFIELD_H
#define __BITFIELD_H

#ifndef EXTERNAL_TEST
    #include "../stdtypes.h"
#else
    #include "Test.h"
#endif

// Basically BitField's should be declared as an array of U32's.
// Pass the size of this array into the size parameter.
// These functions will let you deal with the array as a large set of bits.
int BitFieldGetNext(U32* bitfield, int size, int from);    // use to iterate through each set bit, returns -1 when done
int BitFieldGetFirst(U32* bitfield, int size);            // returns the index of the first set bit
U32 BitFieldGet(U32* bitfield, int size, int bit);        // get the value of the specified bit
void BitFieldSet(U32* bitfield, int size, int bit, U32 value);    // set the value of the specified bit
void BitFieldClear(U32* bitfield, int size);            // clear all bits
void BitFieldCopy(const U32* src, U32* dest, int size);    // copy one bitfield into another
int BitFieldCount(U32* bitfield, int size, int countOnBits); // Counts the number of on-bits or off-bits in the given bitfield.
U32 BitFieldCountSparseOn(U32* bitfield, int size);
void BitFieldOr(U32* src, U32* dest, int size);

int BitFieldAnd(U32* src, U32* dest, int size);

int BitFieldAnyAnd(U32* left, U32* right, int size);

static INLINEDBG int BitFieldGetFirst(U32* bitfield, int size) { return BitFieldGetNext(bitfield, size, -1); }

#endif // __BITFIELD_H