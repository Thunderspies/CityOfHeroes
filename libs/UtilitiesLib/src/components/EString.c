#define ESTRING_IMPL
#include "utilitieslib/components/EString.h"
#include <stddef.h>
#include <stdlib.h>
#include <memory.h>
#include "utilitieslib/assert/assert.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "utilitieslib/utils/StringUtil.h"
#include "utilitieslib/utils/mathutil.h"
#include "zlib/zlib.h"

extern int quick_vscprintf(char *format,const char *args);

static INLINEDBG char* estrToStr(EString* str)
{
    return str->str;
}

static INLINEDBG EString* estrFromStr(char* str)
{
    return (EString*)(str - EStrHeaderSize);
}

static INLINEDBG const EString* cestrFromStr(const char* str)
{
    return (const EString*)(str - EStrHeaderSize);
}

// Returns the size of the entire EString object.
static INLINEDBG int estrObjSize(EString* str)
{
    return str->bufferCapacity + EStrHeaderSize + EStrTerminatorSize;
}

// Returns the size of the EString object that must be copied to move the object.
static INLINEDBG int estrObjDataSize(const EString* str)
{
    return str->stringLength + EStrHeaderSize + EStrTerminatorSize;
}

//-------------------------------------------------------------------
// Memory allocators
//-------------------------------------------------------------------
typedef void*    (*EStrAllocate)(unsigned int size);
typedef void*    (*EStrReallocate)(void* memory, unsigned int newSize);
typedef void    (*EStrDeallocate)(void* memory);

static void* estrHeapAllocate(unsigned int size)
{
    char* memory;
    memory = malloc(size);
    memset(memory, 0, sizeof(EString));
    return memory;
}

static void* estrHeapReallocate(void* memory, unsigned int newSize)
{
    return realloc(memory, newSize);
}

static void estrHeapDeallocate(void* memory)
{
    free(memory);
}



typedef struct 
{
    EStrAllocate    allocate;
    EStrReallocate    reallocate;
    EStrDeallocate    deallocate;
} EStrMemoryAllocator;

static EStrMemoryAllocator EStrHeapAllocator = 
{
    estrHeapAllocate,
    estrHeapReallocate,
    estrHeapDeallocate
};

static INLINEDBG EStrMemoryAllocator* estrGetAllocator(void)
{
    return &EStrHeapAllocator;    
}

static int highBitIndex(unsigned int value)
{
    int mask = 0x80000000;
    int count = 0;

    while(mask && !(mask & value))
    {
        mask >>= 1;
        count++;
    }

    return 32 - count;
}

//-------------------------------------------------------------------
// Constructor/Destructors
//-------------------------------------------------------------------
void estrCreate(char** str)
{
    // By default, allocate the string from the heap.
    // Specify a 0 size string to have estrCreateEx() use the default string size.
    estrCreateEx(str, 0);
}

void estrCreateEx(char** str, int initLength)
{
    EStrMemoryAllocator* allocator;
    EString* estr;

    if(!str)
        return;
    
    if(0 == initLength)
        initLength = ESTR_DEFAULT_SIZE;

    allocator = estrGetAllocator();
    estr = allocator->allocate(EStrHeaderSize + EStrTerminatorSize + initLength);
    memcpy(estr->header, "ESTR", 4);
    estr->bufferCapacity = initLength;
    estr->stringLength = 0;
    estr->isStack = false;
    *str = estrToStr(estr);
}

char* estrCloneEString(const char* src)
{
    EStrMemoryAllocator* allocator;
    const EString* estrSrc;
    EString* estrDst;

    if(!src)
        return NULL;

    estrSrc = cestrFromStr(src);
    allocator = estrGetAllocator();
    estrDst = allocator->allocate(estrObjDataSize(estrSrc));
    memcpy(estrDst, estrSrc, estrObjDataSize(estrSrc));

    return estrToStr(estrDst);
}

char* estrCloneCharString(const char* src)
{
    char* str;
    int len;

    if(!src)
        return NULL;

    len = (int)strlen(src);
    estrCreateEx(&str,len);
    estrConcatFixedWidth(&str,src,len);
    return str;
}

void estrDestroy(char** str)
{
    EStrMemoryAllocator* allocator;
    EString* estr;

    if(!str || !*str)
        return;

    estr = estrFromStr(*str);
    allocator = estrGetAllocator();
    if (!estr->isStack)
    {
        allocator->deallocate(estr);
    }
    else
    {
        // If it's on the stack, make sure that stack address did not rollback after the allocation, but before the destroy
        devassertmsg((uintptr_t)alloca(0) < (uintptr_t)estr, "Stack address is invalid.  Incorrect usage of estrTemp / estrStackCreate?");
    }
    *str = NULL;
}

void estrClear(char** str)
{
    EString* estr;

    if(!str || !*str)
        return;

    estr = estrFromStr(*str);
    estr->stringLength = 0;
    estrTerminateString(estr);
}

unsigned int estrGetCapacity(const char** str)
{
    const EString* estr;
    if(!str || !*str)
        return 0;

    estr = cestrFromStr(*str);
    return estr->bufferCapacity;
}

unsigned int estrReserveCapacity(char** str, unsigned int newCapacity)
{
    EStrMemoryAllocator* allocator;
    int index;
    int newObjSize;
    int newBufferSize;
    EString* estr;

    if(!str)
        return 0;
    if(!*str)
        estrCreate(str);

    estr = estrFromStr(*str);
    if(estr->bufferCapacity >= newCapacity)
        return estr->bufferCapacity;

    // If the capacity is already larger than the specified reserve size,
    // the operation is already complete.
    index = highBitIndex(newCapacity + EStrHeaderSize + EStrTerminatorSize);
    if(32 == index)
    {
        newObjSize = 0xffffffff;    
        newBufferSize = newObjSize - EStrHeaderSize - EStrTerminatorSize;
        // I hope this never happens.  =)
        // Allocating 4gb of memory is always a bad idea.
    }
    else
    {
        newBufferSize = 1 << index;
        newObjSize = newBufferSize + EStrHeaderSize + EStrTerminatorSize;
    }

    allocator = estrGetAllocator();
    if (estr->isStack)
    {
        // We need to copy from the stack to the heap
        EString *newMemory;
        estr->isStack = false;
        newMemory = allocator->allocate(newObjSize);
        memcpy(newMemory,estr,estr->bufferCapacity + EStrHeaderSize + EStrTerminatorSize);
        estr = newMemory;
    }
    else
    {
        estr = allocator->reallocate(estr, newObjSize);
    }    
    estr->bufferCapacity = newBufferSize;
    *str = estrToStr(estr);
    return newBufferSize;
}

unsigned int estrLength(const char* const * str)
{
    const EString* estr;

    if(!str || !*str)
        return 0;

    estr = cestrFromStr(*str);
    return estr->stringLength;
}

unsigned int estrSetLength(char** str, unsigned int newLength)
{
    EString* estr;
    if(!str)
        return 0;
    if(!*str)
        estrCreate(str);

    estrReserveCapacity(str, newLength);
    estr = estrFromStr(*str);
    if(newLength < estr->stringLength)
    {
        estr->stringLength = newLength;
        estr->str[newLength] = 0;
        return newLength;
    }
    memset(estr->str + estr->stringLength, 0, newLength - estr->stringLength + EStrTerminatorSize);
    estr->stringLength = newLength;
    return newLength;
}

unsigned int estrSetLengthNoMemset(char** str, unsigned int newLength)
{
    EString* estr;
    if(!str)
        return 0;
    if(!*str)
        estrCreate(str);

    estrReserveCapacity(str, newLength);
    estr = estrFromStr(*str);
    if(newLength < estr->stringLength)
    {
        estr->stringLength = newLength;
        estr->str[newLength] = 0;
        return newLength;
    }
    estr->stringLength = newLength;
    memset(estr->str + estr->stringLength, 0, EStrTerminatorSize);
    return newLength;
}

unsigned int estrUTF8CharacterCount(const char** str)
{
    if(!str || !*str)
        return 0;
    return UTF8GetLength((char*)*str);
}

void estrPrintEString(char** str, const char* src)
{
    estrClear(str);
    estrConcatEString(str, src);
}

void estrPrintCharString(char** str, const char* src)
{
    estrClear(str);
    estrConcatCharString(str, src);
}

int estrPrintf(char** str, const char* format, ...)
{
    int count;
    VA_START(args, format);
    estrClear(str);
    count = estrConcatfv(str, format, args);
    VA_END();
    return count;
}

void estrConcatEString(char** str, const char* src)
{
    const EString* estrSrc;

    if(!str || !src)
        return;
    if(!*str)
        estrCreate(str);

    estrSrc = cestrFromStr(src);

    estrConcatFixedWidth(str, estrSrc->str, estrSrc->stringLength);
}

void estrConcatCharString(char** str, const char* src)
{
    if(!src)
        return;

    estrConcatFixedWidth((char**)str, src, (int)strlen(src));
}

void estrConcatChar(char** str, char src)
{
    EString* estr;

    if(!str)
        return;
    if(!*str)
        estrCreate(str);

    estr = estrFromStr(*str);
    if (estr->stringLength == estr->bufferCapacity) {
        estrReserveCapacity(str, estr->stringLength + 1);
        estr = estrFromStr(*str);
    }
    // Append the char, advance the length
    *(*str+estr->stringLength++)=src;
    // Null terminate
    estrTerminateString(estr);
}

void estrConcatFixedWidth(char** str, const char* src, int width)
{
    EString* estrDst;
    if(!str)
        return;
    if(!*str)
        estrCreate(str);

    estrDst = estrFromStr(*str);
    estrReserveCapacity(str, estrDst->stringLength + width);
    estrDst = estrFromStr(*str);
    memcpy(estrDst->str + estrDst->stringLength, src, width);
    estrDst->stringLength += width;
    estrTerminateString(estrDst);
}

void estrConcatMinimumWidth(char** str, const char* src, int minWidth)
{
    EString* estr;
    unsigned int requiredLength;
    char *s;
    int widthleft;

    if(!str)
        return;
    if(!*str)
        estrCreate(str);

    estr = estrFromStr(*str);
    requiredLength = estr->stringLength + minWidth + EStrTerminatorSize;
    if (requiredLength > estr->bufferCapacity) {
        estrReserveCapacity(str, requiredLength);
        estr = estrFromStr(*str);
    }
    // Append the string
    for (s=*str+estr->stringLength, widthleft=minWidth; *src && widthleft; *s++=*src++, widthleft--);
    // Pad if less than the width
    for (;widthleft; *s++=' ', widthleft--);
    // Increment stored length
    estr->stringLength += minWidth;
    // Check to see if anything didn't fit in the width
    if (*src) {
        // There's more stuff to append
        // ARM NOTE: This call is what makes this function append at least the minimum width
        //   instead of exactly the specified width.
        estrConcatMinimumWidth(str, src, (int)strlen(src));
        return;
    }
    // Null terminate
    estrTerminateString(estr);
    assert(estr->stringLength <= estr->bufferCapacity);
}

int estrConcatf(char** str, const char* format, ...)
{
    int count;
    VA_START(args, format);
    count = estrConcatfv(str, format, args);
    VA_END();
    return count;
}

int estrConcatfv(char** str, const char* format, va_list args)
{
    EString* estr;
    int printCount;

    if(!str)
        return 0;
    if(!*str)
        estrCreate(str);

    estr = estrFromStr(*str);

    // Try to print the string.
    printCount = quick_vsnprintf(estr->str + estr->stringLength, estr->bufferCapacity - estr->stringLength,
        estr->bufferCapacity - estr->stringLength, format, args);
    if(printCount >= 0) {
        // Good!  It fit.
    } else {
        
        printCount = quick_vscprintf((char*)format, args);
        estrReserveCapacity(str, estr->stringLength + printCount + 1);
        estr = estrFromStr(*str);

        printCount = quick_vsnprintf(estr->str + estr->stringLength, estr->bufferCapacity - estr->stringLength,
            estr->bufferCapacity - estr->stringLength, format, args);
        
        assert(printCount >= 0);
    }

    estr->stringLength += printCount;

    estrTerminateString(estr);

    return printCount;
}

void estrInsert(char** str, int index, const char* src, int count)
{
    EString* estr;
    int verifiedInsertByteIndex;

    if(!str || !src)
        return;
    if(!*str)
        estrCreate(str);

    estr = estrFromStr(*str);
    verifiedInsertByteIndex = MINMAX(index, 0, (int)estr->stringLength);
    estrReserveCapacity(str, estr->stringLength + count);

    estr = estrFromStr(*str);    // String might have been reallocated.
    memmove(estr->str + verifiedInsertByteIndex + count, estr->str + verifiedInsertByteIndex, estr->stringLength + EStrTerminatorSize - verifiedInsertByteIndex);
    memcpy(estr->str + verifiedInsertByteIndex, src, count);
    estr->stringLength += count;
}

void estrRemove(char** str, int index, int count)
{
    EString* estr;
    int verifiedRemoveByteIndex;

    if(!str || !*str)
        return;

    estr = estrFromStr(*str);
    verifiedRemoveByteIndex = MINMAX(index, 0, (int)estr->stringLength);

    // make sure the bytes to remove is valid. take a guess to the end of the string
    if( !verify( (verifiedRemoveByteIndex + count) <= (int)estr->stringLength ))
    {
        count = estr->stringLength - verifiedRemoveByteIndex;
    }

    memmove(estr->str + verifiedRemoveByteIndex, estr->str + verifiedRemoveByteIndex + count, estr->stringLength + EStrTerminatorSize - (verifiedRemoveByteIndex + count));
    estr->stringLength -= count;
    estrTerminateString(estr);
}

void estrPackData(char **str, const void *src, int srclen)
{
    int dstlen = estrReserveCapacity(str, compressBound(srclen)+4)-4; // leave room for an S32
    *(S32*)(*str) = (S32)srclen;
    compress(*str+4, &dstlen, src, srclen);
    estrSetLengthNoMemset(str, dstlen+4);
}

void estrUnpackStr(char **str, const char * const *src)
{
    if(estrLength(src))
    {
        int dstlen = *(S32*)(*src);
        estrSetLengthNoMemset(str, dstlen);
        uncompress(*str, &dstlen, *src+4, estrLength(src)-4);
    }
    else
    {
        estrDestroy(str);
    }
}


void estrWideConcatCharString(unsigned short** str, unsigned short* src) 
{ 
    if(!src)
        return;
    estrConcatFixedWidth((char**)str, (char*)src, (int)wcslen(src)*2); 
}

void estrConvertToNarrow(char **output, unsigned short *input)
{
    int requiredLength;
    if(!input || !output)
        return;

    requiredLength = WideToUTF8StrConvert(input, NULL, 0);
    estrSetLengthNoMemset(output, requiredLength);
    WideToUTF8StrConvert(input, *output, requiredLength);
}

void estrConvertToWide(unsigned short **output, char *input)
{
    int requiredLength;
    if(!input || !output)
        return;

    requiredLength = UTF8ToWideStrConvert(input, NULL, 0);
    estrWideSetLengthNoMemset(output, requiredLength);
    UTF8ToWideStrConvert(input, *output, requiredLength);
}

void estrRemoveDoubleQuotes(char **str)
{
    int i;

	for (i = estrLength((const char *const *)str) - 1; i >= 0; i--)
    {
        if ((*str)[i] == '\"')
            estrRemove(str, i, 1);
    }
}