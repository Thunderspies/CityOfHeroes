/*\
 *
 *    tokenStore.c - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    TokenStoreXxx functions give a nice interface to getting at the raw memory
 *  stored by a particular token.  They abstract away some of the details of
 *  different storage types like direct, fixed array, earray, etc.  They do, however
 *  require that the type you request is in fact _compatible_ with the storage
 *  type of the underlying token - they assert if this is not true.
 *
 */

#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/structInternals.h"
#include "utilitieslib/utils/textparser.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/utils/tokenstore.h"
#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/sysutil.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/components/StringCache.h"
#include "utilitieslib/components/SharedMemory.h"
#include "utilitieslib/components/referencesystem.h"

#ifdef ENABLE_LEAK_DETECTION
static char *parser_temp_allocation_name(const char *name)
{
    static char * buf = NULL;
    if (!buf)
        estrCreate(&buf);
    estrPrintCharString(&buf, parser_relpath ? parser_relpath : "(null)");
    estrConcatChar(&buf, ':');
    estrConcatCharString(&buf, name ? name : "(null)");
    return buf;
}
#endif

///////////////////////////////////////////// getting the storage type

int TokenStoreGetStorageType(StructTypeField type)
{
    U32 compat_value = 0;
    if (type & TOK_FIXED_ARRAY)
        compat_value = TOK_STORAGE_DIRECT_FIXEDARRAY;
    else if (type & TOK_EARRAY)
        compat_value = TOK_STORAGE_DIRECT_EARRAY;
    else 
        compat_value = TOK_STORAGE_DIRECT_SINGLE;

    // HACK
    if (type & TOK_INDIRECT)
        compat_value <<= 3; // shift up to the INDIRECT values

    return compat_value;
}

//////////////////////////////////////// TokenStoreXxxx functions

#define TS_REQUIRE(compat_bits) devassert(TokenStoreIsCompatible(tpi[column].type, compat_bits) && "internal textparser error")

// if allocated, free
void TokenStoreFreeString(ParseTable tpi[], int column, void* structptr, int index)
{
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    char* str;
    char** pstr;
    char*** parray;

    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY);
    switch (storage) {
    case TOK_STORAGE_DIRECT_SINGLE:
        str = (char*)((intptr_t)structptr + tpi[column].storeoffset);
        str[0] = 0;
        break; // don't need to free
    case TOK_STORAGE_INDIRECT_SINGLE:
        if (tpi[column].type & TOK_POOL_STRING)
        {
            pstr = (char**)((intptr_t)structptr + tpi[column].storeoffset);
            *pstr = 0; // don't need to free
        }
        else
        {
            pstr = (char**)((intptr_t)structptr + tpi[column].storeoffset);
            if (*pstr) StructFreeString(*pstr);
            *pstr = 0;
        }
        break;
    case TOK_STORAGE_INDIRECT_EARRAY:
        parray = (char***)((intptr_t)structptr + tpi[column].storeoffset);
        if ((*parray)[index] && !(tpi[column].type & TOK_POOL_STRING))
            StructFreeString((*parray)[index]);
        (*parray)[index] = 0;
        break;
    };
}

// clear the target string WITHOUT freeing it
void TokenStoreClearString(ParseTable tpi[], int column, void* structptr, int index)
{
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    char* str;
    char** pstr;
    char*** parray;

    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY);
    switch (storage) {
    case TOK_STORAGE_DIRECT_SINGLE:
        str = (char*)((intptr_t)structptr + tpi[column].storeoffset);
        str[0] = 0;
        break;
    case TOK_STORAGE_INDIRECT_SINGLE:
        pstr = (char**)((intptr_t)structptr + tpi[column].storeoffset);
        *pstr = 0;
        break;
    case TOK_STORAGE_INDIRECT_EARRAY:
        parray = (char***)((intptr_t)structptr + tpi[column].storeoffset);
        (*parray)[index] = 0;
        break;
    };
}

char* TokenStoreSetString(ParseTable tpi[], int column, void* structptr, int index, const char* str, CustomMemoryAllocator memAllocator, void* customData)
{
    char* newstr = 0;
    char** pstr = 0;
    char*** parray;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    
    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY);

    if (storage == TOK_STORAGE_DIRECT_SINGLE)
    {
        if (!str) str = "";

        // TODO - different error strategy?
        if ((int)strlen(str) >= tpi[column].param)
        {
            //ErrorFilenamef(TokenizerGetFileName(tok),
            //    "Parser: string parameter is too long in %s (expected < %d, found %d)\n", TokenizerGetFileAndLine(tok),
            //    PTR_TO_S32(pti.subtable), strlen(nexttoken));
            Errorf("Parser: string parameter is too long %s (expected < %d, found %d)\n", str, tpi[column].param, strlen(str));
            g_parseerr = 1;
        }

        newstr = (char*)((intptr_t)structptr + tpi[column].storeoffset);
        strcpy_s(newstr, tpi[column].param, str);
        return newstr;
    }

    // hide difference between null and empty strings - 
    //   earrays require emptry strings
    //     indirect strings should be set to NULL
    if (storage == TOK_STORAGE_INDIRECT_EARRAY)
    {
        if (!str) str = "";
    }
    else // indirect
    {
        if (str && !str[0]) str = NULL;
    }

    // if not fixed, choose allocation method
    if (!str)
        newstr = NULL;
    else if (memAllocator)
    {
        int len = (int)strlen(str) + 1;
        newstr = memAllocator(customData, len);
        if (newstr) strcpy_s(newstr, len, str);
    }
    else if (tpi[column].type & TOK_POOL_STRING)
        newstr = (char*)allocAddString(str);
    else
#ifdef ENABLE_LEAK_DETECTION
        newstr = StructAllocStringLenDbg(str, -1, parser_temp_allocation_name(tpi[column].name), -1);
#else
        newstr = StructAllocStringLenDbg(str, -1, parser_relpath, -1);
#endif

    // get string pointer
    if (storage == TOK_STORAGE_INDIRECT_EARRAY)
    {
        parray = (char***)((intptr_t)structptr + tpi[column].storeoffset);
        if (index >= 0 && index < eaSize(parray))
            pstr = &(*parray)[index];
        else
        {
            index = eaPush(parray, 0);
            pstr = &(*parray)[index];
        }
    }
    else if (storage == TOK_STORAGE_INDIRECT_SINGLE)
    {
        pstr = (char**)((intptr_t)structptr + tpi[column].storeoffset);
    }
    // What if storage != either of those two cases? pstr isn't set. --poz
    // I have initialized pstr to NULL, which will crash in those conditions. --poz

    if (*pstr && !(tpi[column].type & TOK_POOL_STRING)) // already a string allocated
        StructFreeString(*pstr);
    *pstr = newstr;

    return newstr;
}

char* TokenStoreGetString(ParseTable tpi[], int column, void* structptr, int index)
{
    return (char*)TokenStoreGetStringConst(tpi, column, structptr, index);
}

const char* TokenStoreGetStringConst(ParseTable tpi[], int column, const void* structptr, int index)
{
    const char* const* pstr;
    const char* const* const* parray;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    
    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY);

    switch (storage) {
    case TOK_STORAGE_DIRECT_SINGLE:
        return (const char*)((intptr_t)structptr + tpi[column].storeoffset);
    case TOK_STORAGE_INDIRECT_EARRAY:
        parray = (const char* const* const*)((intptr_t)structptr + tpi[column].storeoffset);
        if (index >= eaSize((cccEArrayHandle*)parray))
        {
            Errorf("internal textparser error");
            return 0;
        }
        return (*parray)[index];
    case TOK_STORAGE_INDIRECT_SINGLE:
        pstr = (const char* const*)((intptr_t)structptr + tpi[column].storeoffset);
        return *pstr;
    };
    return 0;
}

size_t TokenStoreGetStringMemUsage(ParseTable tpi[], int column, void* structptr, int index)
{
    char** pstr;
    char*** parray;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    
    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY);

    switch (storage) {
    case TOK_STORAGE_DIRECT_SINGLE:
        return 0; // string directly stored in struct
    case TOK_STORAGE_INDIRECT_EARRAY:
        parray = (char***)((intptr_t)structptr + tpi[column].storeoffset);
        if (index >= eaSize((cccEArrayHandle*)parray))
        {
            Errorf("internal textparser error");
            return 0;
        }
        return (*parray)[index]?strlen((*parray)[index])+1:0;
    case TOK_STORAGE_INDIRECT_SINGLE:
        pstr = (char**)((intptr_t)structptr + tpi[column].storeoffset);
        if (*pstr)
            return strlen(*pstr)+1;
        return 0;
    };
    return 0;
}

void TokenStoreSetInt(ParseTable tpi[], int column, void* structptr, int index, int value)
{
    int** parray;
    int* pint;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);

    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_DIRECT_FIXEDARRAY | TOK_STORAGE_DIRECT_EARRAY);

    switch (storage) {
    case TOK_STORAGE_DIRECT_FIXEDARRAY:
        pint = (int*)((intptr_t)structptr + tpi[column].storeoffset);
        if (index >= tpi[column].param)
            Errorf("extra integer parameter");
        else
            pint[index] = value;
        break;
    case TOK_STORAGE_DIRECT_EARRAY:
        parray = (int**)((intptr_t)structptr + tpi[column].storeoffset);
        if (index < 0)
            index = eaiSize(parray);
        if (index < eaiSize(parray))
            (*parray)[index] = value;
        else
            eaiPush(parray, value);
        break;
    case TOK_STORAGE_DIRECT_SINGLE:
        pint = (int*)((intptr_t)structptr + tpi[column].storeoffset);
        *pint = value;
        break;
    };
}

int TokenStoreGetInt(ParseTable tpi[], int column, const void* structptr, int index)
{
    const int* const* parray;
    const int* pint;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);

    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_DIRECT_FIXEDARRAY | TOK_STORAGE_DIRECT_EARRAY);

    switch (storage) {
    case TOK_STORAGE_DIRECT_FIXEDARRAY:
        pint = (const int*)((intptr_t)structptr + tpi[column].storeoffset);
        if (index >= tpi[column].param)
        {
            Errorf("extra integer parameter");
            return 0;
        }
        return pint[index];
    case TOK_STORAGE_DIRECT_EARRAY:
        parray = (const int* const*)((intptr_t)structptr + tpi[column].storeoffset);
        if (index >= eaiSize(parray))
        {
            Errorf("extra integer parameter");
            return 0;
        }
        return (*parray)[index];
    case TOK_STORAGE_DIRECT_SINGLE:
        pint = (const int*)((intptr_t)structptr + tpi[column].storeoffset);
        return *pint;
    };
    return 0;
}

void TokenStoreSetInt64(ParseTable tpi[], int column, void* structptr, int index, S64 value)
{
    S64* pint;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);

#ifndef _M_X64
    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_DIRECT_FIXEDARRAY);
#else
    void*** parray;
    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_DIRECT_FIXEDARRAY | TOK_STORAGE_DIRECT_EARRAY);
#endif

    switch (storage) {
    case TOK_STORAGE_DIRECT_FIXEDARRAY:
        pint = (S64*)((intptr_t)structptr + tpi[column].storeoffset);
        if (index >= tpi[column].param)
            Errorf("extra integer parameter");
        else
            pint[index] = value;
        break;
    case TOK_STORAGE_DIRECT_SINGLE:
        pint = (S64*)((intptr_t)structptr + tpi[column].storeoffset);
        *pint = value;
        break;
#ifdef _M_X64
    case TOK_STORAGE_DIRECT_EARRAY:
        parray = (void***)((intptr_t)structptr + tpi[column].storeoffset);
        if (index < 0)
            index = eaSizeUnsafe(parray);
        if (index < eaSizeUnsafe(parray))
            (*parray)[index] = (void*)(intptr_t)value;
        else
            eaPush(parray, (void*)(intptr_t)value);
        break;
        STATIC_INFUNC_ASSERT(sizeof(void*) == 8); // bit of a hack
#endif
    };
}

S64 TokenStoreGetInt64(ParseTable tpi[], int column, const void* structptr, int index)
{
    const S64* pint;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);

#ifndef _M_X64
    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_DIRECT_FIXEDARRAY);
#else
    const void* const* const* parray;
    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_DIRECT_FIXEDARRAY | TOK_STORAGE_DIRECT_EARRAY);
#endif

    switch (storage) {
    case TOK_STORAGE_DIRECT_FIXEDARRAY:
        pint = (const S64*)((intptr_t)structptr + tpi[column].storeoffset);
        if (index > tpi[column].param)
        {
            Errorf("extra integer parameter");
            return 0;
        }
        else
            return pint[index];
    case TOK_STORAGE_DIRECT_SINGLE:
        pint = (const S64*)((intptr_t)structptr + tpi[column].storeoffset);
        return *pint;
#ifdef _M_X64
    case TOK_STORAGE_DIRECT_EARRAY:
        parray = (const void* const* const*)((intptr_t)structptr + tpi[column].storeoffset);
        if (index >= eaSizeUnsafe(parray))
        {
            Errorf("extra integer parameter");
            return 0;
        }
        return (S64)(intptr_t)(*parray)[index];
        STATIC_INFUNC_ASSERT(sizeof(void*) == 8); // bit of a hack
#endif
    };
    return 0;
}

void TokenStoreSetInt16(ParseTable tpi[], int column, void* structptr, int index, S16 value)
{
    S16* pint;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);

    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_DIRECT_FIXEDARRAY);

    switch (storage) {
    case TOK_STORAGE_DIRECT_FIXEDARRAY:
        pint = (S16*)((intptr_t)structptr + tpi[column].storeoffset);
        if (index >= tpi[column].param)
            Errorf("extra integer parameter");
        else
            pint[index] = value;
        break;
    case TOK_STORAGE_DIRECT_SINGLE:
        pint = (S16*)((intptr_t)structptr + tpi[column].storeoffset);
        *pint = value;
        break;
    };
}

S16 TokenStoreGetInt16(ParseTable tpi[], int column, const void* structptr, int index)
{
    const S16* pint;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);

    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_DIRECT_FIXEDARRAY);

    switch (storage) {
    case TOK_STORAGE_DIRECT_FIXEDARRAY:
        pint = (const S16*)((intptr_t)structptr + tpi[column].storeoffset);
        if (index >= tpi[column].param)
        {
            Errorf("extra integer parameter");
            return 0;
        }
        return pint[index];
    case TOK_STORAGE_DIRECT_SINGLE:
        pint = (const S16*)((intptr_t)structptr + tpi[column].storeoffset);
        return *pint;
    };
    return 0;
}

void TokenStoreSetU8(ParseTable tpi[], int column, void* structptr, int index, U8 value)
{
    U8* pint;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);

    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_DIRECT_FIXEDARRAY);

    switch (storage) {
    case TOK_STORAGE_DIRECT_FIXEDARRAY:
        pint = (U8*)((intptr_t)structptr + tpi[column].storeoffset);
        if (index >= tpi[column].param)
            Errorf("extra integer parameter");
        else
            pint[index] = value;
        break;
    case TOK_STORAGE_DIRECT_SINGLE:
        pint = (U8*)((intptr_t)structptr + tpi[column].storeoffset);
        *pint = value;
        break;
    };
}

U8 TokenStoreGetU8(ParseTable tpi[], int column, const void* structptr, int index)
{
    const U8* pint;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);

    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_DIRECT_FIXEDARRAY);

    switch (storage) {
    case TOK_STORAGE_DIRECT_FIXEDARRAY:
        pint = (const U8*)((intptr_t)structptr + tpi[column].storeoffset);
        if (index >= tpi[column].param)
        {
            Errorf("extra integer parameter");
            return 0;
        }
        return pint[index];
    case TOK_STORAGE_DIRECT_SINGLE:
        pint = (const U8*)((intptr_t)structptr + tpi[column].storeoffset);
        return *pint;
    };
    return 0;
}

void TokenStoreSetF32(ParseTable tpi[], int column, void* structptr, int index, F32 value)
{
    F32** parray;
    F32* pfloat;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);

    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_DIRECT_FIXEDARRAY | TOK_STORAGE_DIRECT_EARRAY);

    switch (storage) {
    case TOK_STORAGE_DIRECT_FIXEDARRAY:
        pfloat = (F32*)((intptr_t)structptr + tpi[column].storeoffset);
        if (index >= tpi[column].param)
            Errorf("extra integer parameter");
        else
            pfloat[index] = value;
        break;
    case TOK_STORAGE_DIRECT_EARRAY:
        parray = (F32**)((intptr_t)structptr + tpi[column].storeoffset);
        if (index < 0)
            index = eafSize(parray);
        if (index < eafSize(parray))
            (*parray)[index] = value;
        else
            eafPush(parray, value);
        break;
    case TOK_STORAGE_DIRECT_SINGLE:
        pfloat = (F32*)((intptr_t)structptr + tpi[column].storeoffset);
        *pfloat = value;
        break;
    };
}

F32 TokenStoreGetF32(ParseTable tpi[], int column, const void* structptr, int index)
{
    const F32* const* parray;
    const F32* pfloat;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);

    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_DIRECT_FIXEDARRAY | TOK_STORAGE_DIRECT_EARRAY);

    switch (storage) {
    case TOK_STORAGE_DIRECT_FIXEDARRAY:
        pfloat = (const F32*)((intptr_t)structptr + tpi[column].storeoffset);
        if (index >= tpi[column].param)
        {
            Errorf("extra float parameter");
            return 0;
        }
        return pfloat[index];
    case TOK_STORAGE_DIRECT_EARRAY:
        parray = (const F32* const*)((intptr_t)structptr + tpi[column].storeoffset);
        if (index >= eafSize(parray))
        {
            Errorf("extra float parameter");
            return 0;
        }
        return (*parray)[index];
    case TOK_STORAGE_DIRECT_SINGLE:
        pfloat = (const F32*)((intptr_t)structptr + tpi[column].storeoffset);
        return *pfloat;
    };
    return 0;
}

void TokenStoreSetCapacity(ParseTable tpi[], int column, void* structptr, int capacity)
{
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    void*** eap;

    //TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY);

    switch (storage) {
    case TOK_STORAGE_INDIRECT_EARRAY:
        eap = (void***)((intptr_t)structptr + tpi[column].storeoffset);

        if (capacity < 0)
            capacity = 0;

        if (capacity > 0 && eaMemUsage(eap) == 0)
        {
            eaCreateWithCapacity(eap, capacity);
        }
        else
        {
            eaSetCapacity(eap, capacity);
        }
        break;
    };
}

void* TokenStoreAlloc(ParseTable tpi[], int column, void* structptr, int index, U32 size, CustomMemoryAllocator memAllocator, void* customData)
{
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    void* p;
    void** pp;
    void*** eap;

    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY);

    switch (storage) {
    case TOK_STORAGE_DIRECT_SINGLE: // should be pointing at correct amount of memory already
        devassert((U32)tpi[column].param == size && "internal textparser error");
        return (void*)((intptr_t)structptr + tpi[column].storeoffset);
    case TOK_STORAGE_INDIRECT_SINGLE:
        pp = (void**)((intptr_t)structptr + tpi[column].storeoffset);
        if (memAllocator)
            *pp = memAllocator(customData, size);
        else
#ifdef ENABLE_LEAK_DETECTION
            *pp = StructAllocRawDbg(size, parser_temp_allocation_name(tpi[column].name), -1);
#else
            *pp = StructAllocRawDbg(size, parser_relpath, -1);
#endif
        return *pp;
    case TOK_STORAGE_INDIRECT_EARRAY:
        eap = (void***)((intptr_t)structptr + tpi[column].storeoffset);

        if (memAllocator)
            p = memAllocator(customData, size);
        else
#ifdef ENABLE_LEAK_DETECTION
            p = StructAllocRawDbg(size, parser_temp_allocation_name(tpi[column].name), -1);
#else
            p = StructAllocRawDbg(size, parser_relpath, -1);
#endif

        if (index < 0)
            index = eaSizeUnsafe(eap);
        if (index < eaSizeUnsafe(eap))
            (*eap)[index] = p;
        else
            eaPush(eap, p);
        return p;
    };
    return NULL;
}

void TokenStoreFree(ParseTable tpi[], int column, void* structptr, int index)
{
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    void** pp;
    void*** eap;

    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY);
    switch (storage) {
    case TOK_STORAGE_DIRECT_SINGLE: return; // do nothing
    case TOK_STORAGE_INDIRECT_SINGLE:
        pp = (void**)((intptr_t)structptr + tpi[column].storeoffset);
        if (*pp) 
            StructFree(*pp);
        *pp = NULL;
        return;
    case TOK_STORAGE_INDIRECT_EARRAY:
        eap = (void***)((intptr_t)structptr + tpi[column].storeoffset);
        if ((*eap)[index])
            StructFree((*eap)[index]);
        (*eap)[index] = 0;
        return;
    };
}

// get the count field used by TOK_POINTER or TOK_USEDFIELD
int* TokenStoreGetCountField(ParseTable tpi[], int column, void* structptr)
{
    return (int*)TokenStoreGetCountFieldConst(tpi, column, structptr);
}

const int* TokenStoreGetCountFieldConst(ParseTable tpi[], int column, const void* structptr)
{
    TS_REQUIRE(TOK_STORAGE_INDIRECT_SINGLE);
    return (const int*)((intptr_t)structptr + tpi[column].param);
}

void TokenStoreSetPointer(ParseTable tpi[], int column, void* structptr, int index, void* ptr)
{
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    void** pointer;
    void*** parray;

    TS_REQUIRE(TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY);
    switch (storage) {
    case TOK_STORAGE_INDIRECT_SINGLE:
        pointer = (void**)((intptr_t)structptr + tpi[column].storeoffset);
        *pointer = ptr;
        break;
    case TOK_STORAGE_INDIRECT_EARRAY:
        parray = (void***)((intptr_t)structptr + tpi[column].storeoffset);
        if (index < 0)
            index = eaSizeUnsafe(parray);
        if (index < eaSizeUnsafe(parray))
            (*parray)[index] = ptr;
        else
            eaPush(parray, ptr);
        break;
    };
}

void* TokenStoreGetPointer(ParseTable tpi[], int column, void* structptr, int index)
{
    return (void*)TokenStoreGetPointerConst(tpi, column, structptr, index);
}

const void* TokenStoreGetPointerConst(ParseTable tpi[], int column, const void* structptr, int index)
{
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    const void* const* pp;
    const void* const* const* ea;

    TS_REQUIRE(TOK_STORAGE_DIRECT_SINGLE | TOK_STORAGE_DIRECT_FIXEDARRAY | TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY);

    switch (storage) {
    case TOK_STORAGE_DIRECT_SINGLE:
    case TOK_STORAGE_DIRECT_FIXEDARRAY:
        return (const void*)((intptr_t)structptr + tpi[column].storeoffset);
    case TOK_STORAGE_INDIRECT_SINGLE:
        pp = (const void* const*)((intptr_t)structptr + tpi[column].storeoffset);
        return *pp;
    case TOK_STORAGE_INDIRECT_EARRAY:
        ea = (const void* const* const*)((intptr_t)structptr + tpi[column].storeoffset);
        if (index >= eaSizeUnsafe(ea))
        {
            // fine, it's valid for clients to ask for a pointer and get NULL back
            return 0;
        }
        return (*ea)[index];
    };
    return 0;
}

void* TokenStoreRemovePointer(ParseTable tpi[], int column, void* structptr, void* ptr)
{
    void*** ea;
    TS_REQUIRE(TOK_STORAGE_INDIRECT_EARRAY);
    ea = (void***)((intptr_t)structptr + tpi[column].storeoffset);
    eaFindAndRemove(ea, ptr);
    return ptr;
}

void TokenStoreSetRef(ParseTable tpi[], int column, void* structptr, int index, const char* str)
{
    void*** parray;
    void** pp;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    TS_REQUIRE(TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY);

    if (storage == TOK_STORAGE_INDIRECT_SINGLE)
    {
        pp = (void**)((intptr_t)structptr + tpi[column].storeoffset);
    }
    else
    {
        parray = (void***)((intptr_t)structptr + tpi[column].storeoffset);
        if (index < 0)
            index = eaSizeUnsafe(parray);
        if (index >= eaSizeUnsafe(parray))
        {
            eaPush(parray, 0);
            index = eaSizeUnsafe(parray)-1;
        }
        pp = &(*parray)[index];
    }
    RefSystem_RemoveHandle(pp, true);
    RefSystem_AddHandleFromString((char*)tpi[column].subtable, str, pp, (bool)(tpi[column].type & TOK_VOLATILE_REF), (bool)(tpi[column].type & TOK_VOLATILE_REF));
}

void TokenStoreClearRef(ParseTable tpi[], int column, void* structptr, int index)
{
    void*** parray;
    void** pp;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    TS_REQUIRE(TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY);

    if (storage == TOK_STORAGE_INDIRECT_SINGLE)
    {
        pp = (void**)((intptr_t)structptr + tpi[column].storeoffset);
    }
    else
    {
        parray = (void***)((intptr_t)structptr + tpi[column].storeoffset);
        if (index < 0 || index >= eaSizeUnsafe(parray))
            return;
        pp = &(*parray)[index];
    }

    RefSystem_RemoveHandle(pp, true);
}

void TokenStoreCopyRef(ParseTable tpi[], int column, void* dest, void* src, int index)
{
    void*** parray;
    void **ppdest, **ppsrc;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    TS_REQUIRE(TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY);

    if (storage == TOK_STORAGE_INDIRECT_SINGLE)
    {
        ppdest = (void**)((intptr_t)dest + tpi[column].storeoffset);
        ppsrc = (void**)((intptr_t)src + tpi[column].storeoffset);
    }
    else
    {
        // dest
        parray = (void***)((intptr_t)dest + tpi[column].storeoffset);
        if (index < 0)
            index = eaSizeUnsafe(parray);
        if (index >= eaSizeUnsafe(parray))
        {
            eaPush(parray, 0);
            index = eaSizeUnsafe(parray)-1;
        }
        ppdest = &(*parray)[index];

        // src
        parray = (void***)((intptr_t)src + tpi[column].storeoffset);
        if (index < 0 || index >= eaSizeUnsafe(parray))
            ppsrc = NULL;
        else
            ppsrc = &(*parray)[index];
    }
    RefSystem_RemoveHandle(ppdest, true);
    
    if (ppsrc && RefSystem_IsHandleActive(ppsrc))
        RefSystem_CopyHandle(ppdest, ppsrc); // ok for ppsrc not to be ref?
}

bool TokenStoreGetRefString(ParseTable tpi[], int column, const void* structptr, int index, char* str, int str_size)
{
    void*** parray;
    void** pp;
    char* temp;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    TS_REQUIRE(TOK_STORAGE_INDIRECT_SINGLE | TOK_STORAGE_INDIRECT_EARRAY);

    if (storage == TOK_STORAGE_INDIRECT_SINGLE)
    {
        pp = (void**)((intptr_t)structptr + tpi[column].storeoffset);
    }
    else
    {
        parray = (void***)((intptr_t)structptr + tpi[column].storeoffset);
        if (index < 0 || index >= eaSizeUnsafe(parray))
        {
            str[0] = 0;
            return false;
        }
        pp = &(*parray)[index];
    }

    if (!RefSystem_IsHandleActive(pp))
    {
        str[0] = 0;
        return false;
    }

    // I would prefer not to be creating a temporary string here..
    estrStackCreate(&temp,str_size);
    RefSystem_StringFromHandle(&temp, pp, (bool)(tpi[column].type & TOK_VOLATILE_REF));
    strncpyt(str, temp, str_size);
    estrDestroy(&temp);
    return true;
}

void*** TokenStoreGetEArray(ParseTable tpi[], int column, void* structptr)
{
    return (void***)TokenStoreGetEArrayConst(tpi, column, structptr);
}

const void* const* const* TokenStoreGetEArrayConst(ParseTable tpi[], int column, const void* structptr)
{
    TS_REQUIRE(TOK_STORAGE_INDIRECT_EARRAY);
    return (const void* const* const*)((intptr_t)structptr + tpi[column].storeoffset);
}

int** TokenStoreGeteai(ParseTable tpi[], int column, void* structptr)
{
    return (int**)TokenStoreGeteaiConst(tpi, column, structptr);
}

const int* const* TokenStoreGeteaiConst(ParseTable tpi[], int column, const void* structptr)
{
    TS_REQUIRE(TOK_STORAGE_DIRECT_EARRAY);
    return (const int* const*)((intptr_t)structptr + tpi[column].storeoffset);
}

F32** TokenStoreGeteaf(ParseTable tpi[], int column, void* structptr)
{
    return (F32**)TokenStoreGeteafConst(tpi, column, structptr);
}

const F32* const* TokenStoreGeteafConst(ParseTable tpi[], int column, const void* structptr)
{
    TS_REQUIRE(TOK_STORAGE_DIRECT_EARRAY);
    return (const F32* const*)((intptr_t)structptr + tpi[column].storeoffset);
}

size_t TokenStoreGetEArrayMemUsage(ParseTable tpi[], int column, void* structptr)
{
    int** ea32;
    void*** ea;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    TS_REQUIRE(TOK_STORAGE_DIRECT_EARRAY | TOK_STORAGE_INDIRECT_EARRAY);

    switch (storage) {
    case TOK_STORAGE_DIRECT_EARRAY: // int** or F32**
        ea32 = (int**)((intptr_t)structptr + tpi[column].storeoffset);
        return ea32MemUsage(ea32);
    case TOK_STORAGE_INDIRECT_EARRAY:
        ea = (void***)((intptr_t)structptr + tpi[column].storeoffset);
        return eaMemUsage(ea);
    }
    return 0;
}

void TokenStoreSetEArraySize(ParseTable tpi[], int column, void* structptr, int size)
{
    int** ea32;
    void*** ea;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    TS_REQUIRE(TOK_STORAGE_DIRECT_EARRAY | TOK_STORAGE_INDIRECT_EARRAY);

    switch (storage) {
    case TOK_STORAGE_DIRECT_EARRAY: // int** or F32**
        ea32 = (int**)((intptr_t)structptr + tpi[column].storeoffset);
        ea32SetSize(ea32, size);
        break;
    case TOK_STORAGE_INDIRECT_EARRAY:
        ea = (void***)((intptr_t)structptr + tpi[column].storeoffset);
        eaSetSize(ea, size);
        break;
    }
}

void TokenStoreCopyEArray(ParseTable tpi[], int column, void* dest, void* src, CustomMemoryAllocator memAllocator, void* customData)
{
    int **ea32src, **ea32dest;
    void ***easrc, ***eadest;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    TS_REQUIRE(TOK_STORAGE_DIRECT_EARRAY | TOK_STORAGE_INDIRECT_EARRAY);

    switch (storage) {
    case TOK_STORAGE_DIRECT_EARRAY: // int** or F32**
        ea32src = (int**)((intptr_t)src + tpi[column].storeoffset);
        ea32dest = (int**)((intptr_t)dest + tpi[column].storeoffset);
        ea32Compress(ea32dest, ea32src, memAllocator, customData);
        break;
    case TOK_STORAGE_INDIRECT_EARRAY:
        easrc = (void***)((intptr_t)src + tpi[column].storeoffset);
        eadest = (void***)((intptr_t)dest + tpi[column].storeoffset);
        eaCompress(eadest, easrc, memAllocator, customData);
        break;
    }
}

void TokenStoreDestroyEArray(ParseTable tpi[], int column, void* structptr)
{
    int** ea32;
    void*** ea;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    TS_REQUIRE(TOK_STORAGE_DIRECT_EARRAY | TOK_STORAGE_INDIRECT_EARRAY);

    switch (storage) {
    case TOK_STORAGE_DIRECT_EARRAY: // int** or F32**
        ea32 = (int**)((intptr_t)structptr + tpi[column].storeoffset);
        ea32Destroy(ea32);
        break;
    case TOK_STORAGE_INDIRECT_EARRAY:
        ea = (void***)((intptr_t)structptr + tpi[column].storeoffset);
        eaDestroy(ea);
        break;
    }
}

int TokenStoreGetNumElems(ParseTable tpi[], int column, const void* structptr)
{
    const int* const* ea32;
    const void* const* const* ea;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);

    switch (storage) {
    case TOK_STORAGE_INDIRECT_SINGLE: // fall
    case TOK_STORAGE_DIRECT_SINGLE: return 1;
    case TOK_STORAGE_INDIRECT_FIXEDARRAY: // fall
    case TOK_STORAGE_DIRECT_FIXEDARRAY:    return tpi[column].param;
    case TOK_STORAGE_DIRECT_EARRAY: // const int* const* or const F32* const* - HACK - consider const int* const* because memory layout won't differ
        ea32 = (const int* const*)((intptr_t)structptr + tpi[column].storeoffset);
        return ea32Size(ea32);
    case TOK_STORAGE_INDIRECT_EARRAY:
        ea = (const void* const* const*)((intptr_t)structptr + tpi[column].storeoffset);
        return eaSizeUnsafe(ea);
    }
    return 0;
}

void TokenStoreMakeLocalEArray(ParseTable tpi[], int column, void* structptr)
{
    int **ea32, *newea32;
    void ***ea, **newea;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    TS_REQUIRE(TOK_STORAGE_DIRECT_EARRAY | TOK_STORAGE_INDIRECT_EARRAY);

    switch (storage) {
    case TOK_STORAGE_DIRECT_EARRAY: // int** or F32**
        ea32 = (int**)((intptr_t)structptr + tpi[column].storeoffset);
        if (isSharedMemory(*ea32))
        {
            ea32Compress(&newea32, ea32, NULL, NULL);
            *ea32 = newea32;
        }
        break;
    case TOK_STORAGE_INDIRECT_EARRAY:
        ea = (void***)((intptr_t)structptr + tpi[column].storeoffset);
        if (isSharedMemory(*ea))
        {
            eaCompress(&newea, ea, NULL, NULL);
            *ea = newea;
        }
        break;
    }
}
