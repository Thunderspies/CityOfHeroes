#include "utilitieslib/components/MetaDictionary.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/utils/error.h"

#define STARTING_NUM_DICTIONARIES 64

typedef struct
{
    const char *pDictionaryName;
    const void *pDictionaryData;

    DictionaryS2P S2PFunc;
    DictionaryP2S P2SFunc;
} DictionaryDef;

static StashTable sMetaDictionary = NULL;

MP_DEFINE(DictionaryDef);

void MetaDictionaryAddDictionary(const char *pDictionaryName, void *pDictionaryData, DictionaryS2P S2PFunc, DictionaryP2S P2SFunc)
{
    DictionaryDef *pDef;
    
    MP_CREATE(DictionaryDef,STARTING_NUM_DICTIONARIES);


    if (!sMetaDictionary)
    {
        sMetaDictionary = stashTableCreateWithStringKeys(STARTING_NUM_DICTIONARIES, StashDeepCopyKeys);
        assert(sMetaDictionary);
    }
    else
    {
        //check if we already have a dictionary with this name
        void *pTemp;

        assert(!stashFindPointer(sMetaDictionary, pDictionaryName, &pTemp));
    }

     pDef = MP_ALLOC(DictionaryDef);


    assert(pDef);

    pDef->pDictionaryName = pDictionaryName;
    pDef->pDictionaryData = pDictionaryData;
    pDef->S2PFunc = S2PFunc;
    pDef->P2SFunc = P2SFunc;

    stashAddPointer(sMetaDictionary, pDictionaryName, pDef, false);
}

bool MetaDictionaryStringToPointer(void **ppOutPointer, const char *pInString, const char *pDictionaryName)
{
    DictionaryDef *pDictionary;

    if (!stashFindPointer(sMetaDictionary, pDictionaryName, &pDictionary))
    {
        Errorf("Couldn't find dictionary %s\n", pDictionaryName);
        return false;
    }

    return pDictionary->S2PFunc(ppOutPointer, pInString, pDictionary->pDictionaryData);
}






bool MetaDictionaryPointerToString(char outString[METADICTIONARY_MAX_STRING_LENGTH], void *pInPointer, const char *pDictionaryName)
{
    DictionaryDef *pDictionary;

    if (!stashFindPointer(sMetaDictionary, pDictionaryName, &pDictionary))
    {
        Errorf("Couldn't find dictionary %s\n", pDictionaryName);
        return false;
    }

    return pDictionary->P2SFunc(outString, pInPointer, pDictionary->pDictionaryData);
}



