/*\
 *
 *    tokenStore.h - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    TokenStoreXxx functions give a nice interface to getting at the raw memory
 *  stored by a particular token.  They abstract away some of the details of
 *  different storage types like direct, fixed array, earray, etc.  They do, however
 *  require that the type you request is in fact _compatible_ with the array
 *  type of the underlying token - they assert if this is not true.
 *
 *  Note it is still up to you to make sure you are addressing the correct basic type,
 *  TokenStore just deals with raw memory and will not give any errors over the basic token type
 *
 */

#ifndef TOKENSTORE_H
#define TOKENSTORE_H

typedef struct ParseTable ParseTable;

///////////////////////////////////////////////////// TokenStoreXxx functions
// these are all pretty self-explanatory.  
// index always refers to the position in the array that you want to manipulate,
// should be zero for non-arrays

int TokenStoreGetNumElems(ParseTable tpi[], int column, const void* structptr); // valid for any or token type

void TokenStoreFreeString(ParseTable tpi[], int column, void* structptr, int index);
void TokenStoreClearString(ParseTable tpi[], int column, void* structptr, int index);
char* TokenStoreSetString(ParseTable tpi[], int column, void* structptr, int index, const char* str, CustomMemoryAllocator memAllocator, void* customData);
char* TokenStoreGetString(ParseTable tpi[], int column, void* structptr, int index);
const char* TokenStoreGetStringConst(ParseTable tpi[], int column, const void* structptr, int index);
size_t TokenStoreGetStringMemUsage(ParseTable tpi[], int column, void* structptr, int index);

void TokenStoreSetInt(ParseTable tpi[], int column, void* structptr, int index, int value);
int TokenStoreGetInt(ParseTable tpi[], int column, const void* structptr, int index);
void TokenStoreSetInt64(ParseTable tpi[], int column, void* structptr, int index, S64 value);
S64 TokenStoreGetInt64(ParseTable tpi[], int column, const void* structptr, int index);
void TokenStoreSetInt16(ParseTable tpi[], int column, void* structptr, int index, S16 value);
S16 TokenStoreGetInt16(ParseTable tpi[], int column, const void* structptr, int index);
void TokenStoreSetU8(ParseTable tpi[], int column, void* structptr, int index, U8 value);
U8 TokenStoreGetU8(ParseTable tpi[], int column, const void* structptr, int index);
void TokenStoreSetF32(ParseTable tpi[], int column, void* structptr, int index, F32 value);
F32 TokenStoreGetF32(ParseTable tpi[], int column, const void* structptr, int index);

void TokenStoreSetCapacity(ParseTable tpi[], int column, void* structptr, int capacity);
void* TokenStoreAlloc(ParseTable tpi[], int column, void* structptr, int index, U32 size, CustomMemoryAllocator memAllocator, void* customData);
void TokenStoreFree(ParseTable tpi[], int column, void* structptr, int index);
int* TokenStoreGetCountField(ParseTable tpi[], int column, void* structptr);
const int* TokenStoreGetCountFieldConst(ParseTable tpi[], int column, const void* structptr);
void TokenStoreSetPointer(ParseTable tpi[], int column, void* structptr, int index, void* ptr);
void* TokenStoreGetPointer(ParseTable tpi[], int column, void* structptr, int index);
const void* TokenStoreGetPointerConst(ParseTable tpi[], int column, const void* structptr, int index);
void* TokenStoreRemovePointer(ParseTable tpi[], int column, void* structptr, void* ptr);

void TokenStoreSetRef(ParseTable tpi[], int column, void* structptr, int index, const char* str);
void TokenStoreClearRef(ParseTable tpi[], int column, void* structptr, int index);
void TokenStoreCopyRef(ParseTable tpi[], int column, void* dest, void* src, int index);
bool TokenStoreGetRefString(ParseTable tpi[], int column, const void* structptr, int index, char* str, int str_size);

void*** TokenStoreGetEArray(ParseTable tpi[], int column, void* structptr);
const void* const* const* TokenStoreGetEArrayConst(ParseTable tpi[], int column, const void* structptr);
int** TokenStoreGeteai(ParseTable tpi[], int column, void* structptr);
const int* const* TokenStoreGeteaiConst(ParseTable tpi[], int column, const void* structptr);
F32** TokenStoreGeteaf(ParseTable tpi[], int column, void* structptr);
const F32* const* TokenStoreGeteafConst(ParseTable tpi[], int column, const void* structptr);
size_t TokenStoreGetEArrayMemUsage(ParseTable tpi[], int column, void* structptr);
void TokenStoreSetEArraySize(ParseTable tpi[], int column, void* structptr, int size);
void TokenStoreCopyEArray(ParseTable tpi[], int column, void* dest, void* src, CustomMemoryAllocator memAllocator, void* customData);
void TokenStoreDestroyEArray(ParseTable tpi[], int column, void* structptr);
void TokenStoreMakeLocalEArray(ParseTable tpi[], int column, void* structptr);



//////////////////////////////////////// getting the storage type for a field

// these storage enums are for verification of parse tables - each token type is
// compatible with different methods of storage
#define TOK_STORAGE_DIRECT_SINGLE        (1 << 0)        // int member;
#define TOK_STORAGE_DIRECT_FIXEDARRAY    (1 << 1)        // int members[3];
#define TOK_STORAGE_DIRECT_EARRAY        (1 << 2)        // int* members;
#define TOK_STORAGE_INDIRECT_SINGLE        (1 << 3)        // char* str;
#define TOK_STORAGE_INDIRECT_FIXEDARRAY (1 << 4)        // char* strs[3];
#define TOK_STORAGE_INDIRECT_EARRAY        (1 << 5)        // char** strs;
// ORDER OF THESE IS IMPORTANT - check TokenStoreGetStorageType to see hack

int TokenStoreGetStorageType(StructTypeField type);
#define TokenStoreIsCompatible(type, type_compat_bits) (TokenStoreGetStorageType(type) & (type_compat_bits))

#endif // TOKENSTORE_H
