/*\
 *
 *    structDefines.h - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides DefineXxx and StaticDefineXxx systems for
 *  use with textparser, or just independently.  Both
 *  systems allow string substitution.  DefineXxx is based 
 *  on a hash table, StaticDefineXxx is just a simple
 *  static table that can be defined as a constant in code.
 *
 */

#include "utilitieslib/utils/structDefines.h"
#include "utilitieslib/network/crypt.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/SuperAssert.h"

// *********************************************************************************
//  Dynamic defines
// *********************************************************************************

// need this because free() is redefined in memcheck
static void Free(void* p) { free(p); }

typedef struct DefineContext
{
    StashTable hash;
    struct DefineContext* higherlevel;
} DefineContext;

DefineContext* DefineCreate()
{
    DefineContext* result = malloc(sizeof(DefineContext));
    memset(result, 0, sizeof(DefineContext));
    result->hash = stashTableCreateWithStringKeys(100, StashDeepCopyKeys);
    return result;
}

DefineContext* DefineCreateFromList(DefineList defs[])
{
    DefineContext* result = DefineCreate();
    DefineAddList(result, defs);
    return result;
}

DefineContext* DefineCreateFromIntList(DefineIntList defs[])
{
    DefineContext* result = DefineCreate();
    DefineAddIntList(result, defs);
    return result;
}

void DefineAddList(DefineContext* lhs, DefineList defs[])
{
    DefineList* cur = defs;
    while (cur->key && cur->key[0])
    {
        DefineAdd(lhs, cur->key, cur->value);
        cur++;
    }
}

void DefineAddIntList(DefineContext* lhs, DefineIntList defs[])
{
    DefineIntList* cur = defs;
    while (cur->key && cur->key[0])
    {
        char buffer[20];
        sprintf_s(SAFESTR(buffer), "%i", cur->value);
        DefineAdd(lhs, cur->key, buffer);
        cur++;
    }
}

void DefineDestroy(DefineContext* context)
{
    stashTableDestroyEx(context->hash, NULL, Free);
    free(context);
}

void DefineAdd(DefineContext* context, const char* key, const char* value) // strings copied
{
    char *value_copy = _strdup(value);
    if (!stashAddPointer(context->hash, key, value_copy, false))
        free(value_copy);
}

void DefineSetHigherLevel(DefineContext* lower, DefineContext* higher)
{
    lower->higherlevel = higher;
}

static INLINEDBG DefineContext* DefineGetHigherLevel(DefineContext* context)
{
    return context->higherlevel;
}

// uses a hash table now
const char* DefineLookup(DefineContext* context, const char* key)
{
    DefineContext* curcontext = context;

    while (curcontext)
    {
        char* str = stashFindPointerReturnPointer(curcontext->hash, key);
        if (str) return str;
        curcontext = curcontext->higherlevel;
    }
    return NULL;
}

static const char* DefineRevLookup(DefineContext* context, char* value)
{
    StashTableIterator stashIterator;
    StashElement currElement = NULL;
    DefineContext* curcontext = context;

    while (curcontext)
    {
        for (stashGetIterator(curcontext->hash, &stashIterator); stashGetNextElement(&stashIterator, &currElement);)
        {
            char* storedVal = stashElementGetPointer(currElement);
            if (value && storedVal && !stricmp(value, storedVal))
                return stashElementGetStringKey(currElement);
        }
        curcontext = curcontext->higherlevel;
    }
    return NULL;
}

// *********************************************************************************
//  static defines
// *********************************************************************************

const char* StaticDefineLookup(StaticDefine* list, const char* key)
{
    static char scratchpad[200]; // may be needed to hold result value
    int curtype = DM_NOTYPE;
    StaticDefine* cur = list;

    if (!key) return NULL;
    while (1)
    {
        // look for key markers first
        if (cur->key == U32_TO_PTR(DM_END))
            return NULL; // failed lookup
        else if (cur->key == U32_TO_PTR(DM_INT))
            curtype = DM_INT;
        else if (cur->key == U32_TO_PTR(DM_STRING))
            curtype = DM_STRING;
        else if (cur->key == U32_TO_PTR(DM_DYNLIST))
        {
            // do a lookup in the dynamic list
            DefineContext* context = *(DefineContext**)cur->value;
            const char* result = DefineLookup(context, key);
            if (result) return result;
        }
        else if (curtype == DM_NOTYPE)
        {
            assertmsg(0, "StaticDefine table found without a type marker at top, (or bad pointer?)");
        }
        else
        {
            // do a real lookup on key
            if (!stricmp(cur->key, key))
            {
                switch (curtype)
                {
                case DM_INT:
                    itoa((intptr_t)cur->value, scratchpad, 10);
                    return scratchpad;
                case DM_STRING:
                    return cur->value;
                }
                FatalErrorf("StaticDefineLookup: list doesn't have a correct type marker\n");
            }
        }
        // keep looking for keys
        cur++;
    }
}

int StaticDefineIntLookupInt(StaticDefineInt* list, const char* key)
{
    int curtype = DM_NOTYPE;

    if(!key)
        return 0;

    for(;; list++)
    {
        // look for key markers first
        switch((intptr_t)list->key)
        {
            xcase DM_INT:
                curtype = DM_INT;

            xcase DM_STRING:
                curtype = DM_STRING;

            xcase DM_DYNLIST:
            {
                DefineContext* context = *(DefineContext**)list->value;
                const char* result = DefineLookup(context, key);
                if(result)
                    return atoi(result);
            }

            xdefault:
                assertmsg(curtype != DM_NOTYPE, "StaticDefine table found without a type marker at top, (or bad pointer?)");
                if(streq(list->key, key))
                {
                    switch(curtype)
                    {
                        xcase DM_INT:        return list->value;
                        xcase DM_STRING:    return atoi((char*)list->value);
                        xdefault:            FatalErrorf(__FUNCTION__": unsupported type marker %d", curtype);
                    }
                }

            xcase DM_END:
                return atoi(key);
        }
    }
}

const char* StaticDefineIntRevLookup(StaticDefineInt* list, int value)
{
    char valuestr[20];
    int curtype = DM_NOTYPE;
    StaticDefineInt* cur = list;

    sprintf_s(SAFESTR(valuestr), "%i", value);
    while (1)
    {
        if (cur->key == DM_END)
            return NULL; // failed lookup
        else if (cur->key == U32_TO_PTR(DM_INT))
            curtype = DM_INT;
        else if (cur->key == U32_TO_PTR(DM_STRING))
            curtype = DM_STRING;
        else if (cur->key == U32_TO_PTR(DM_DYNLIST))
        {
            // do a lookup in the dynamic list
            DefineContext* context = *(DefineContext**)cur->value;
            return DefineRevLookup(context, valuestr);
        }
        else if (curtype == DM_STRING)
        {
            if (stricmp((char*)cur->value, valuestr) == 0)
                return cur->key;
        }
        else if (curtype == DM_INT)
        {
            if (cur->value == value)
                return cur->key;
        }
        cur++;
    }
}

const char* StaticDefineIntRevLookupDisplay(StaticDefineInt* list, int value)
{
    char valuestr[20];
    int curtype = DM_NOTYPE;
    StaticDefineInt* cur = list;

    sprintf_s(SAFESTR(valuestr), "%i", value);
    while (1)
    {
        if (cur->key == DM_END)
            return NULL; // failed lookup
        else if (cur->key == U32_TO_PTR(DM_INT))
            curtype = DM_INT;
        else if (cur->key == U32_TO_PTR(DM_STRING))
            curtype = DM_STRING;
        else if (cur->key == U32_TO_PTR(DM_DYNLIST))
        {
            // do a lookup in the dynamic list
            DefineContext* context = *(DefineContext**)cur->value;
            return DefineRevLookup(context, valuestr);
        }
        else if (curtype == DM_STRING)
        {
            if (stricmp((char*)cur->value, valuestr) == 0)
                return cur->display;
        }
        else if (curtype == DM_INT)
        {
            if (cur->value == value)
                return cur->display;
        }
        cur++;
    }
}

const char* StaticDefineRevLookup(StaticDefine* list, char* value)
{
    char valuestr[20];
    int curtype = DM_NOTYPE;
    StaticDefine* cur = list;

    while (1)
    {
        if (cur->key == DM_END)
            return NULL; // failed lookup
        else if (cur->key == U32_TO_PTR(DM_INT))
            curtype = DM_INT;
        else if (cur->key == U32_TO_PTR(DM_STRING))
            curtype = DM_STRING;
        else if (cur->key == U32_TO_PTR(DM_DYNLIST))
        {
            // do a lookup in the dynamic list
            DefineContext* context = *(DefineContext**)cur->value;
            return DefineRevLookup(context, value);
        }
        else if (curtype == DM_STRING)
        {
            if (stricmp(cur->value, value) == 0)
                return cur->key;
        }
        else if (curtype == DM_INT)
        {
            sprintf_s(SAFESTR(valuestr), "%i", (intptr_t)cur->value);
            if (stricmp(valuestr, value) == 0)
                return cur->key;
        }
        cur++;
    }
}

int UpdateCrcFromDefineElement(StashElement e)
{
    const char* key = stashElementGetStringKey(e);
    char* value = (char*)stashElementGetPointer(e);
    cryptAdler32Update(key, (int)strlen(key));
    cryptAdler32Update(value, (int)strlen(value));
    return 1;
}

void UpdateCrcFromDefineList(DefineContext* defines)
{
    stashForEachElement(defines->hash, UpdateCrcFromDefineElement);
    if (defines->higherlevel) UpdateCrcFromDefineList(defines->higherlevel);
}

