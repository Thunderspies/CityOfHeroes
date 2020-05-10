#include "utilitieslib/components/ReferenceSystem.h"
#include "utilitieslib/components/ReferenceSystem_Internal.h"

#include "utilitieslib/components/hashfunctions.h"
#include "utilitieslib/utils/stringutil.h"

#include "utilitieslib/utils/error.h"
#include "utilitieslib/components/earray.h"

#include "utilitieslib/components/stashtable.h"
#include "utilitieslib/utils/superassert.h"

#define DEBUG_PRINTF if (0) printf


#define TEST_SIMPLE_POINTER_REFERENCES 0

#define REFVER_STRING "REFVER:"
#define REFVER_STRING_LEN 7


ReferentInfoStruct *GetFreeReferentInfo(void);
void ReleaseReferentInfo_Internal(ReferentInfoStruct *pReferentInfo);

static bool sRefSystemInitted = false;
static int siNumReferenceDictionaries;
static ReferenceDictionary sReferenceDictionaries[MAX_REFERENCE_DICTIONARIES];

StashTable handleTable;
StashTable volatileHandleTable;
StashTable referentTable;

#define ASSERT_INITTED if (!sRefSystemInitted) RefSystem_Init();
#define ASSERT_DICTIONARY(eDict) assert(eDict >= 0 && eDict < REF_DICTIONARY_LAST)



#if REFSYSTEM_TEST_HARNESS
struct Referent *RTHDirectlyDecodeReference(struct ReferenceData *pRefData);
ReferenceHashValue RTHGetHashFromReferenceData(struct ReferenceData *pRefData);
struct ReferenceData *RTHCopyReferenceData(struct ReferenceData *pRefData);
void RTHFreeReferenceData(struct ReferenceData *pRefData);
bool RTHReferenceDataToString(char **ppEString, struct ReferenceData *pRefData);
struct ReferenceData *RTHStringToReferenceData(char *pString);

void RTH_Test(void);
#endif



#if TEST_SIMPLE_POINTER_REFERENCES
void TestSimplePointerReferences(void);
#endif

DictionaryHandle DictionaryHandleFromIndex(int i)
{
    return (void*)((uintptr_t)(i + 1));
}

DictionaryHandle RefSystem_RegisterDictionary(char *pName,
    RefCallBack_DirectlyDecodeReference *pDecodeCB,
    RefCallBack_GetHashFromReferenceData *pGetHashCB,
    RefCallBack_ReferenceDataToString *pRefDataToStringCB,
    RefCallBack_StringToReferenceData *pStringToRefDataCB,
    RefCallBack_CopyReferenceData *pCopyReferenceCB,
    RefCallBack_FreeReferenceData *pFreeReferenceCB)
{
    ASSERT_INITTED

    assert(pName);
    assert(strlen(pName) < REFERENCEDICTIONARY_MAX_NAME_LENGTH);
    assert(siNumReferenceDictionaries < MAX_REFERENCE_DICTIONARIES);


    strcpy_s(SAFESTR(sReferenceDictionaries[siNumReferenceDictionaries].name), pName);
    sReferenceDictionaries[siNumReferenceDictionaries].pCopyReferenceDataCB = pCopyReferenceCB;
    sReferenceDictionaries[siNumReferenceDictionaries].pDirectlyDecodeReferenceCB = pDecodeCB;
    sReferenceDictionaries[siNumReferenceDictionaries].pFreeReferenceDataCB = pFreeReferenceCB;
    sReferenceDictionaries[siNumReferenceDictionaries].pGetHashFromReferenceDataCB = pGetHashCB;
    sReferenceDictionaries[siNumReferenceDictionaries].pRefDataToStringCB = pRefDataToStringCB;
    sReferenceDictionaries[siNumReferenceDictionaries].pStringToRefDataCB = pStringToRefDataCB;


    sReferenceDictionaries[siNumReferenceDictionaries].iNameHash = hashString(pName, false);

    sReferenceDictionaries[siNumReferenceDictionaries].hashedRefDataTable = stashTableCreateInt(64);


    return DictionaryHandleFromIndex(siNumReferenceDictionaries++);
}

DictionaryHandle RefSystem_RegisterDictionaryWithStringRefData(char *pName,
     RefCallBack_DirectlyDecodeReference *pDecodeCB,
     bool bCaseSensitive)
{
    ASSERT_INITTED

    assert(pName);
    assert(strlen(pName) < REFERENCEDICTIONARY_MAX_NAME_LENGTH);
    assert(siNumReferenceDictionaries < MAX_REFERENCE_DICTIONARIES);


    strcpy_s(SAFESTR(sReferenceDictionaries[siNumReferenceDictionaries].name), pName);
    sReferenceDictionaries[siNumReferenceDictionaries].pDirectlyDecodeReferenceCB = pDecodeCB;


    sReferenceDictionaries[siNumReferenceDictionaries].iNameHash = hashString(pName, false);


    sReferenceDictionaries[siNumReferenceDictionaries].bUsesStringsAsReferenceData = true;
    sReferenceDictionaries[siNumReferenceDictionaries].bCaseSensitiveHashing = bCaseSensitive;
    
    sReferenceDictionaries[siNumReferenceDictionaries].hashedRefDataTable = stashTableCreateInt(64);

    return DictionaryHandleFromIndex(siNumReferenceDictionaries++);
}

DictionaryHandle RefSystem_RegisterSelfDefiningDictionary(char *pName, bool bCaseSensitive)
{
    ASSERT_INITTED

    assert(pName);
    assert(strlen(pName) < REFERENCEDICTIONARY_MAX_NAME_LENGTH);
    assert(siNumReferenceDictionaries < MAX_REFERENCE_DICTIONARIES);


    strcpy_s(SAFESTR(sReferenceDictionaries[siNumReferenceDictionaries].name), pName);



    sReferenceDictionaries[siNumReferenceDictionaries].iNameHash = hashString(pName, false);


    sReferenceDictionaries[siNumReferenceDictionaries].bUsesStringsAsReferenceData = true;
    sReferenceDictionaries[siNumReferenceDictionaries].bCaseSensitiveHashing = bCaseSensitive;
    sReferenceDictionaries[siNumReferenceDictionaries].bIsSelfDefining = true;

    sReferenceDictionaries[siNumReferenceDictionaries].hashedRefDataTable = stashTableCreateInt(64);


    return DictionaryHandleFromIndex(siNumReferenceDictionaries++);
}




ReferenceDictionary *RefDictionaryFromNameOrHandle(DictionaryHandle dictHandle)
{
    int iHash;
    int i;

    assert(dictHandle);

    if ((uintptr_t)dictHandle <= (uintptr_t)siNumReferenceDictionaries)
    {
        return &sReferenceDictionaries[((uintptr_t)dictHandle) - 1];
    }

    iHash = hashString((char*)dictHandle, false);

    for (i=0; i < siNumReferenceDictionaries; i++)
    {
        if (sReferenceDictionaries[i].iNameHash == iHash)
        {
            return &sReferenceDictionaries[i];
        }
    }

    assert(0);
    return false;
}


void RefSystem_DictionarySetVersionCB(DictionaryHandleOrName dictHandle, RefCallBack_GetVersionFromReferent *pGetVersionCB)
{
    ReferenceDictionary *pDictionary;

    ASSERT_INITTED

    pDictionary = RefDictionaryFromNameOrHandle(dictHandle);

    assert(pDictionary->bUsesStringsAsReferenceData && !pDictionary->pGetVersionCB);

    pDictionary->pGetVersionCB = pGetVersionCB;
}


bool RefSystem_DoesDictionaryExist(char *pDictionaryName)
{
    int iHash;
    int i;

    ASSERT_INITTED

    assert(pDictionaryName);
    iHash = hashString(pDictionaryName, false);

    for (i=0; i < siNumReferenceDictionaries; i++)
    {
        if (sReferenceDictionaries[i].iNameHash == iHash)
        {
            return true;
        }
    }

    return false;
}

DictionaryHandle RefSystem_GetDictionaryHandleFromName(char *pDictionaryName)
{
    int iHash = hashString(pDictionaryName, false);
    int i;

    ASSERT_INITTED

    for (i=0; i < siNumReferenceDictionaries; i++)
    {
        if (sReferenceDictionaries[i].iNameHash == iHash)
        {
            return DictionaryHandleFromIndex(i);
        }
    }

    return NULL;
}


static INLINEDBG int GetHashWithDictionary(ReferenceDictionary *pDictionary, struct ReferenceData *pRefData)
{
    if (pDictionary->bUsesStringsAsReferenceData)
    {
        return hashString( (char*)pRefData, pDictionary->bCaseSensitiveHashing );
    }
    else
    {
        return pDictionary->pGetHashFromReferenceDataCB(pRefData);
    }
}

ReferentInfoStruct *FindOrCreateReferentInfo(ReferenceDictionary *pDictionary, struct ReferenceData *pRefData,
    bool bRequireNonNULLReferent, U32 iRequiredVersion, struct Referent *pSelfDefiningReferent)
{
    ReferenceHashValue iReferenceHash =  GetHashWithDictionary(pDictionary, pRefData);
    struct Referent *pReferent;
    U32 iReferentVersion = 0;

    bool bReferentIsRefData = false;

    ReferentInfoStruct *pReferentInfo;

    if (stashIntFindPointer(pDictionary->hashedRefDataTable, iReferenceHash, &pReferentInfo))
    {
        if (bRequireNonNULLReferent && !pReferentInfo->pReferent)
        {
            return NULL;
        }

        if (iRequiredVersion && (pReferentInfo->iReferentVersion != iRequiredVersion))
        {
            return NULL;
        }

        //found a preexisting reference that matches our hash... return it, but first
        //check for hash collision
        if (pDictionary->bUsesStringsAsReferenceData)
        {
            if (pDictionary->bCaseSensitiveHashing)
            {
                assertmsgf(strcmp((char*)pRefData, pReferentInfo->pStringRefData) == 0, "Reference system hash collision: %s and %s",
                    (char*)pRefData, pReferentInfo->pStringRefData);
            }
            else
            {
                assertmsgf(stricmp((char*)pRefData, pReferentInfo->pStringRefData) == 0, "Reference system hash collision: %s and %s",
                    (char*)pRefData, pReferentInfo->pStringRefData);
            }
        }
        else
        {
            if (!pDictionary->bDontCheckForHashCollisions)
            {
                char *pTempString1, *pTempString2;
                estrStackCreate(&pTempString1,1000);
                pDictionary->pRefDataToStringCB(&pTempString1, pRefData);

                estrStackCreate(&pTempString2,1000);
                pDictionary->pRefDataToStringCB(&pTempString2, pReferentInfo->pOpaqueRefData);

                assertmsgf(strcmp(pTempString1, pTempString2) == 0, "Reference system hash collision: %s and %s",
                    pTempString1, pTempString2);

                estrDestroy(&pTempString1);
                estrDestroy(&pTempString2);
            }
        }


        return pReferentInfo;
    }

    if (pDictionary->bIsSelfDefining)
    {
        pReferent = pSelfDefiningReferent;
    }
    else
    {
        pReferent = pDictionary->pDirectlyDecodeReferenceCB(pRefData);
    }

    if (pReferent == REFERENT_INVALID)
    {

        return NULL;
    }

    if (bRequireNonNULLReferent && !pReferent)
    {
        return NULL;
    }

    if (pDictionary->pGetVersionCB)
    {
        if (pReferent)
        {
            iReferentVersion = pDictionary->pGetVersionCB(pReferent);

            if (iRequiredVersion && iRequiredVersion != iReferentVersion)
            {
                return NULL;
            }
        }
        else
        {
            iReferentVersion = 0;
        }
    }


    //have to do a special check to see if ref data and referent are equal. If they are, then this is a
    //"trivial" dictionary, and we need to reset the referent after calling the copy callback function,
    //so the referent pointer points to the right copy of the ref data.
    if ((void*)pReferent == (void*)pRefData)
    {
        bReferentIsRefData = true;
    }

    pReferentInfo = GetFreeReferentInfo();

    pReferentInfo->pReferent = pReferent;

    pReferentInfo->iReferenceHash = iReferenceHash;
    pReferentInfo->pDictionary = pDictionary;

    pReferentInfo->iReferentVersion = iReferentVersion;

    if (pDictionary->bUsesStringsAsReferenceData)
    {
        estrCreate(&pReferentInfo->pStringRefData);
        estrPrintCharString(&pReferentInfo->pStringRefData, (char*)pRefData);
    }
    else
    {
        pReferentInfo->pOpaqueRefData = pDictionary->pCopyReferenceDataCB(pRefData);
    }

    if (bReferentIsRefData)
    {
        pReferentInfo->pReferent = (struct Referent*)(pReferentInfo->pOpaqueRefData);
    }

    pReferentInfo->ppHandles = NULL;
    pReferentInfo->ppVolatileHandles = NULL;

    stashIntAddPointer(pDictionary->hashedRefDataTable, iReferenceHash, pReferentInfo, false);

    if (pReferent)
    {
        stashAddPointer(referentTable, pReferent, pReferentInfo, false);
    }


    return pReferentInfo;
}






bool RefSystem_AddHandle(DictionaryHandle dictHandle, struct ReferenceData *pRefData, ReferenceHandle *pHandle,
    bool bHandleIsVolatile, U32 iRequiredVersion)
{
    ReferentInfoStruct  *pReferentInfo;

    ReferenceDictionary *pDictionary;

    ASSERT_INITTED;

    assert(pRefData);
    assert(pHandle);

    pDictionary = RefDictionaryFromNameOrHandle(dictHandle);

    assert(!stashFindPointer(handleTable, pHandle, &pReferentInfo));

    pReferentInfo = FindOrCreateReferentInfo(pDictionary, pRefData, bHandleIsVolatile, iRequiredVersion, NULL);

    if (!pReferentInfo)
    {
        SetRefHandle(pHandle, NULL);
        return false;
    }

    if (bHandleIsVolatile)
    {

        eaPush(cpp_static_cast(mEArrayHandle*)(&pReferentInfo->ppVolatileHandles), pHandle);

        stashAddPointer(volatileHandleTable, pHandle, pReferentInfo, false);
    }
    else
    {
        eaPush(cpp_static_cast(mEArrayHandle*)(&pReferentInfo->ppHandles), pHandle);

        stashAddPointer(handleTable, pHandle, pReferentInfo, false);
    }

    SetRefHandle(pHandle, pReferentInfo->pReferent);


    return true;
}



void AttemptToExtractVersionFromString(const char **ppString, U32 *pVersion)
{
    if (strncmp(*ppString, REFVER_STRING, REFVER_STRING_LEN) == 0)
    {
        sscanf((*ppString) + REFVER_STRING_LEN, "%u", pVersion);

        *ppString += REFVER_STRING_LEN + 1;

        while (**ppString && **ppString != '_')
        {
            (*ppString)++;
        }
    }
}

bool RefSystem_AddHandleFromString(DictionaryHandle dictHandle, const char *pString, ReferenceHandle *pHandle,
    bool bHandleIsVolatile, bool bUseVersioningIfApplicable)
{

    struct ReferenceData *pRefData;
    ReferenceDictionary *pDictionary;
    bool bResult;

    ASSERT_INITTED;


    pDictionary = RefDictionaryFromNameOrHandle(dictHandle);

    if (pDictionary->bUsesStringsAsReferenceData)
    {
        U32 iRequestedVersion = 0;

        if (bUseVersioningIfApplicable && bHandleIsVolatile && pDictionary->pGetVersionCB)
        {
            AttemptToExtractVersionFromString(&pString, &iRequestedVersion);
        }        

        bResult = RefSystem_AddHandle(dictHandle, (struct ReferenceData*)pString, pHandle, bHandleIsVolatile, iRequestedVersion);

        assert(bResult);

        return bResult;
    }

    pRefData = pDictionary->pStringToRefDataCB(pString);

    if (!pRefData)
    {
        return false;
    }

    bResult = RefSystem_AddHandle(dictHandle, pRefData, pHandle, bHandleIsVolatile, 0);

    assert(bResult);

    pDictionary->pFreeReferenceDataCB(pRefData);

    return bResult;
}


void RefSystem_StringFromHandle(char **ppEString, ReferenceHandle *pHandle, bool bAttachVersionIfApplicable)
{

    ReferentInfoStruct *pReferentInfo;
    bool bIsVolatile = false;


    ASSERT_INITTED;
    assert(pHandle);

    if (!stashFindPointer(handleTable, pHandle, &pReferentInfo))
    {
        bIsVolatile = true;

        if (!stashFindPointer(volatileHandleTable, pHandle, &pReferentInfo))
        {
            assertmsg(0, "Inactive handle being dereferenced");
        }
    }

    if (pReferentInfo->pDictionary->bUsesStringsAsReferenceData)
    {
        estrConcatEString(ppEString, pReferentInfo->pStringRefData);
    }
    else
    {
        bool bResult;

        bResult = pReferentInfo->pDictionary->pRefDataToStringCB(ppEString, pReferentInfo->pOpaqueRefData);
    
        assert(bResult);
    }

    if (bAttachVersionIfApplicable && pReferentInfo->pDictionary->pGetVersionCB && bIsVolatile && pReferentInfo->pReferent)
    {
        char tempString[32];

        sprintf_s(tempString, 32, "%s%u ", REFVER_STRING, pReferentInfo->pDictionary->pGetVersionCB(pReferentInfo->pReferent));
        estrInsert(ppEString, 0, tempString, (int)strlen(tempString));
    }
}

bool RefSystem_IsHandleActive(ReferenceHandle *pHandle)
{
    ReferentInfoStruct *pReferentInfo;
    
    ASSERT_INITTED;
    assert(pHandle);

    return stashFindPointer(handleTable, pHandle, &pReferentInfo) || stashFindPointer(volatileHandleTable, pHandle, &pReferentInfo);
}


char *RefSystem_DictionaryNameFromReferenceHandle(ReferenceHandle *pHandle)
{
    ReferentInfoStruct *pReferentInfo;
    
    ASSERT_INITTED;
    assert(pHandle);

    if (!stashFindPointer(handleTable, pHandle, &pReferentInfo))
    {
        if (!stashFindPointer(volatileHandleTable, pHandle, &pReferentInfo))
        {
            assertmsg(0, "Trying to get dictionary name from non-handle");
        }
    }

    return pReferentInfo->pDictionary->name;

}


void RefSystem_AddReferent(DictionaryHandle dictHandle, struct ReferenceData *pRefData, struct Referent *pReferent)
{
    ReferentInfoStruct *pReferentInfo;
    ReferenceHashValue iHash;
    ReferenceDictionary *pDictionary;

    ASSERT_INITTED;

    pDictionary = RefDictionaryFromNameOrHandle(dictHandle);

    assert(pReferent);

    if (stashFindPointer(referentTable, pReferent, &pReferentInfo))
    {
        assertmsg(0, "Trying to add a referent where one already exists");
    }

    iHash = GetHashWithDictionary(pDictionary, pRefData);

    if (stashIntFindPointer(pDictionary->hashedRefDataTable, iHash, &pReferentInfo))
    {
        int i;
        int iSize;

        pReferentInfo->pReferent = pReferent;

        iSize = eaSizeUnsafe(&pReferentInfo->ppHandles);

        for (i=0; i < iSize; i++)
        {
            SetRefHandle(pReferentInfo->ppHandles[i], pReferent);
        }

        stashAddPointer(referentTable, pReferent, pReferentInfo, false);
    }
    else if (pDictionary->bIsSelfDefining)
    {
        pReferentInfo = FindOrCreateReferentInfo(pDictionary, pRefData, false, 0, pReferent);

        stashAddPointer(referentTable, pReferent, pReferentInfo, false);
    }

    if (pDictionary->pGetVersionCB)
    {
        pReferentInfo->iReferentVersion = pDictionary->pGetVersionCB(pReferent);
    }
}

void ReleaseReferentInfo(ReferentInfoStruct *pReferentInfo)
{
    stashIntRemovePointer(pReferentInfo->pDictionary->hashedRefDataTable, pReferentInfo->iReferenceHash, &pReferentInfo);

    if (pReferentInfo->pReferent)
    {
        stashRemovePointer(referentTable, pReferentInfo->pReferent, &pReferentInfo);
    }

    if (pReferentInfo->pDictionary->bUsesStringsAsReferenceData)
    {
        estrDestroy(&pReferentInfo->pStringRefData);
    }
    else
    {
        pReferentInfo->pDictionary->pFreeReferenceDataCB(pReferentInfo->pOpaqueRefData);
    }

    eaDestroy((void***)&pReferentInfo->ppHandles);

    ReleaseReferentInfo_Internal(pReferentInfo);
}

void RefSystem_RemoveHandle(ReferenceHandle *pHandle, bool bOKIfNotAHandle)
{
    bool bIsVolatile = false;

    ReferentInfoStruct *pReferentInfo;
    
    ASSERT_INITTED;
    assert(pHandle);

    if (!stashFindPointer(handleTable, pHandle, &pReferentInfo))
    {
        bIsVolatile = true;
        if (!stashFindPointer(volatileHandleTable, pHandle, &pReferentInfo))
        {
            if (bOKIfNotAHandle)
            {
                return;
            }

            assertmsg(0, "Trying to remove a non-handle");
        }
    }

    if (bIsVolatile)
    {
        stashRemovePointer(volatileHandleTable, pHandle, &pReferentInfo);

        eaFindAndRemoveFast((void***)&pReferentInfo->ppVolatileHandles, pHandle);
    }
    else
    {
        stashRemovePointer(handleTable, pHandle, &pReferentInfo);

        eaFindAndRemoveFast((void***)&pReferentInfo->ppHandles, pHandle);
    }

    if (eaSizeUnsafe(&pReferentInfo->ppHandles) == 0 && eaSizeUnsafe(&pReferentInfo->ppVolatileHandles) == 0 && !(pReferentInfo->pDictionary->bIsSelfDefining))
    {
        ReleaseReferentInfo(pReferentInfo);
    }
    
}

void RefSystem_MoveHandle(ReferenceHandle *pNewHandle, ReferenceHandle *pOldHandle)
{
    ReferentInfoStruct *pReferentInfo;
    int iIndex;
    bool bVolatile = false;

    ASSERT_INITTED;
    assert(pOldHandle);
    assert(pNewHandle);


    if (!stashFindPointer(handleTable, pOldHandle, &pReferentInfo))
    {
        bVolatile = true;
        if (!stashFindPointer(volatileHandleTable, pOldHandle, &pReferentInfo))
        {
            assertmsg(0, "Trying to move a non-handle");
        }
    }

    if (bVolatile)
    {
        stashRemovePointer(volatileHandleTable, pOldHandle, &pReferentInfo);
        stashAddPointer(volatileHandleTable, pNewHandle, pReferentInfo, false);
        
        iIndex = eaFind((void***)&pReferentInfo->ppVolatileHandles, pOldHandle);

        assertmsg(iIndex != -1, "Ref system corruption");

        pReferentInfo->ppVolatileHandles[iIndex] = pNewHandle;
    }
    else
    {
        stashRemovePointer(handleTable, pOldHandle, &pReferentInfo);
        stashAddPointer(handleTable, pNewHandle, pReferentInfo, false);
        
        iIndex = eaFind((void***)&pReferentInfo->ppHandles, pOldHandle);

        assertmsg(iIndex != -1, "Ref system corruption");

        pReferentInfo->ppHandles[iIndex] = pNewHandle;
    }
}

struct ReferenceData *RefSystem_RefDataFromHandle(ReferenceHandle *pHandle)
{
    ReferentInfoStruct *pReferentInfo;
    
    ASSERT_INITTED;
    assert(pHandle);

    if (!stashFindPointer(handleTable, pHandle, &pReferentInfo))
    {
        if (!stashFindPointer(volatileHandleTable, pHandle, &pReferentInfo))
        {
            assertmsg(0, "trying to get ref data from a non-handle");
        }
    }

    return pReferentInfo->pOpaqueRefData;
}

void RefSystem_CopyHandle(ReferenceHandle *pDstHandle, ReferenceHandle *pSrcHandle)
{
    ReferentInfoStruct *pReferentInfo;
    bool bResult;

    ASSERT_INITTED;
    assert(pSrcHandle);
    assert(pDstHandle);

    if (stashFindPointer(handleTable, pSrcHandle, &pReferentInfo))
    {
        bResult = RefSystem_AddHandle(pReferentInfo->pDictionary->name, pReferentInfo->pOpaqueRefData, pDstHandle, false, 0);
        assert(bResult);
        return;
    }

    if (stashFindPointer(volatileHandleTable, pSrcHandle, &pReferentInfo))
    {
        bResult = RefSystem_AddHandle(pReferentInfo->pDictionary->name, pReferentInfo->pOpaqueRefData, pDstHandle, true, 0);
        assert(bResult);
        return;
    }

    assertmsg(0, "Trying to copy a non-handle");


}


void RefSystem_RemoveReferent(struct Referent *pReferent, bool bCompletelyRemoveHandlesToMe)
{
    ReferentInfoStruct *pReferentInfo;
    int iSize, iVolatileSize, i;

    ASSERT_INITTED;
    assert(pReferent);

    if (!stashFindPointer(referentTable, pReferent, &pReferentInfo))
    {
        return;
    }

    iVolatileSize = eaSizeUnsafe(&pReferentInfo->ppVolatileHandles);
    iSize = eaSizeUnsafe(&pReferentInfo->ppHandles);

    stashRemovePointer(referentTable, pReferent, &pReferentInfo);

    pReferentInfo->pReferent = NULL;

    for (i=0; i < iVolatileSize; i++)
    {
        SetRefHandle(pReferentInfo->ppVolatileHandles[i], NULL);
        stashRemovePointer(volatileHandleTable, pReferentInfo->ppVolatileHandles[i], &pReferentInfo);
    }

    for (i=0; i < iSize; i++)
    {
        SetRefHandle(pReferentInfo->ppHandles[i], NULL);

        if (bCompletelyRemoveHandlesToMe)
        {
            stashRemovePointer(handleTable, pReferentInfo->ppHandles[i], &pReferentInfo);
        }
    }


    if (bCompletelyRemoveHandlesToMe)
    {        
        ReleaseReferentInfo(pReferentInfo);
    }
    else if (iSize == 0 && iVolatileSize == 0)
    {
        assertmsg(pReferentInfo->pDictionary->bIsSelfDefining, "Only self-defining dictionaries can have referents with no handles");
        ReleaseReferentInfo(pReferentInfo);
    }
}

struct Referent *RefSystem_GetReferentFromName(DictionaryHandle dictHandle, char *pReferentName)
{
    ReferentInfoStruct *pReferentInfo;
    ReferenceDictionary *pDictionary;
    ReferenceHashValue iHash;

    ASSERT_INITTED;

    pDictionary = RefDictionaryFromNameOrHandle(dictHandle);

    assertmsg(pDictionary->bIsSelfDefining, "GetReferentFromName only valid for self-defining dictionaries");

    iHash = GetHashWithDictionary(pDictionary, (struct ReferenceData*)pReferentName);
    
    if (!stashIntFindPointer(pDictionary->hashedRefDataTable, iHash, &pReferentInfo))
    {
        return NULL;
    }

    return pReferentInfo->pReferent;
}

void RefSystem_MoveReferent(struct Referent *pNewReferent, struct Referent *pOldReferent)
{
    ReferentInfoStruct *pReferentInfo;
    int iSize, i, iVolatileSize;

    ASSERT_INITTED;
    assert(pNewReferent);
    assert(pOldReferent);

    if (!stashFindPointer(referentTable, pOldReferent, &pReferentInfo))
    {
        return;
    }

    iSize = eaSizeUnsafe(&pReferentInfo->ppHandles);

    stashRemovePointer(referentTable, pOldReferent, &pReferentInfo);

    pReferentInfo->pReferent = pNewReferent;

    stashAddPointer(referentTable, pNewReferent, pReferentInfo, false);

    for (i=0; i < iSize; i++)
    {
        SetRefHandle(pReferentInfo->ppHandles[i], pNewReferent);
    }

    iVolatileSize = eaSizeUnsafe(&pReferentInfo->ppVolatileHandles);

    for (i=0; i < iVolatileSize; i++)
    {
        SetRefHandle(pReferentInfo->ppVolatileHandles[i], NULL);
        stashRemovePointer(volatileHandleTable, pReferentInfo->ppVolatileHandles[i], &pReferentInfo);
    }

    if (iSize == 0 && iVolatileSize == 0 && !pReferentInfo->pDictionary->bIsSelfDefining)
    {
        ReleaseReferentInfo(pReferentInfo);
    }
}

#define NUM_REFERENT_INFOS_TO_ALLOCATE_AT_ONCE 256

static ReferentInfoStruct *spFirstFreeReferentInfo;
static int sNumReferentInfos = 0;


void *AllocateLinkedBlocks(int iNumBlocks, int iBlockSize)
{
    char *pBuffer = calloc(iNumBlocks * iBlockSize, 1);
    int i;

    for (i=0; i < iNumBlocks; i++)
    {
        *((void**)(pBuffer + i * iBlockSize)) = (i == iNumBlocks - 1 ? NULL : pBuffer + (i + 1) * iBlockSize);
    }

    return pBuffer;
}

ReferentInfoStruct *GetFreeReferentInfo(void)
{
    ReferentInfoStruct *pRetVal;
    ASSERT_INITTED;

    if (!spFirstFreeReferentInfo)
    {
        spFirstFreeReferentInfo = AllocateLinkedBlocks(NUM_REFERENT_INFOS_TO_ALLOCATE_AT_ONCE, sizeof(ReferentInfoStruct));
        sNumReferentInfos += NUM_REFERENT_INFOS_TO_ALLOCATE_AT_ONCE;
    }

    pRetVal = spFirstFreeReferentInfo;
    spFirstFreeReferentInfo = *((ReferentInfoStruct**)(spFirstFreeReferentInfo));

#if REFSYSTEM_DEBUG_MODE
    pRetVal->iDebugID = -1;
#endif

    return pRetVal;
    
}

void ReleaseReferentInfo_Internal(ReferentInfoStruct *pReferentInfo)
{
    ASSERT_INITTED;

    *((ReferentInfoStruct**)(pReferentInfo)) = spFirstFreeReferentInfo;
    spFirstFreeReferentInfo = pReferentInfo;
}

void RefSystem_AssertListsAreEmpty(void)
{
    int count = 0;
    ReferentInfoStruct *pReferentInfo;


    pReferentInfo = spFirstFreeReferentInfo;

    while (pReferentInfo)
    {
        count++;
        pReferentInfo = *((ReferentInfoStruct**)(pReferentInfo));
    }

    assert(count == sNumReferentInfos);
}

//stuff relating to the "null dictionary"
struct Referent *NullDictDirectlyDecodeReference(struct ReferenceData *pRefData)
{
    return (struct Referent*)pRefData;
}

ReferenceHashValue NullDictGetHashFromReferenceData(struct ReferenceData *pRefData)
{
#ifdef _XBOX
    U32 *pInt = (int*)pRefData;

    return *pInt + *(pInt + 1);
#else

#ifdef _WIN64
    U32 *pInt = (int*)pRefData;

    return *pInt + *(pInt + 1);
#else
    return (ReferenceHashValue)((uintptr_t)pRefData);
#endif

#endif
}

struct ReferenceData *NullDictCopyReferenceData(struct ReferenceData *pRefData)
{
    
    return pRefData;
}

void NullDictFreeReferenceData(struct ReferenceData *pRefData)
{
}

//This function converts a reference data to a string, and APPENDS that string to the EString
bool  NullDictReferenceDataToString(char **ppEString, struct ReferenceData *pRefData)
{
    char tempString[32];
    sprintf_s(tempString, 31, "%Id", (uintptr_t)pRefData);
    estrConcatCharString(ppEString, tempString);
    return true;
}



struct ReferenceData *NullDictStringToReferenceData(const char *pString)
{
    Errorf("can't convert null dictionary entries to strings");
    assert(0);
    return NULL;

}

int RefSystem_PushIntoRefArray(void ***pppRefArray, DictionaryHandle dictHandle, struct ReferenceData *pRefData)
{
    int iIndex;

    assertmsg(pppRefArray, "Attempting to push into nonexistent REFARRAY");

    iIndex = eaPush(pppRefArray, NULL);

    RefSystem_AddHandle(dictHandle, pRefData, (ReferenceHandle*)(&((*pppRefArray)[iIndex])), false, 0);

    return iIndex;
}


void RefSystem_InsertIntoRefArray(void ***pppRefArray, DictionaryHandle dictHandle, struct ReferenceData *pRefData, int index)
{
    assertmsg(pppRefArray, "Attempting to push into nonexistent REFARRAY");

    eaInsert(pppRefArray, NULL, index);

    RefSystem_AddHandle(dictHandle, pRefData, (ReferenceHandle*)(&((*pppRefArray)[index])), false, 0);
}


void RefSystem_Init(void)
{
    ReferenceDictionary *pDictionary;

    if (sRefSystemInitted)
    {
        return;
    }

    memset(sReferenceDictionaries, 0, sizeof(sReferenceDictionaries));

    sRefSystemInitted = true;
    spFirstFreeReferentInfo = NULL;

    referentTable = stashTableCreateAddress(64);
    handleTable = stashTableCreateAddress(64);
    volatileHandleTable = stashTableCreateAddress(64);



    RefSystem_RegisterDictionary("nullDictionary",
        NullDictDirectlyDecodeReference,
        NullDictGetHashFromReferenceData,
        NullDictReferenceDataToString,
        NullDictStringToReferenceData,
        NullDictCopyReferenceData,
        NullDictFreeReferenceData);

    pDictionary = RefDictionaryFromNameOrHandle("nullDictionary");

    pDictionary->bDontCheckForHashCollisions = true;




    
#if REFSYSTEM_TEST_HARNESS
#if    TEST_HARNESS_STRINGVERSION
    RefSystem_RegisterDictionaryWithStringRefData("TestHarness",
        RTHDirectlyDecodeReference, true);
#else
    RefSystem_RegisterDictionary("TestHarness",
        RTHDirectlyDecodeReference,
        RTHGetHashFromReferenceData,
        RTHReferenceDataToString,
        RTHStringToReferenceData,
        RTHCopyReferenceData,
        RTHFreeReferenceData);
#endif


    RTH_Test();

#endif

#if TEST_SIMPLE_POINTER_REFERENCES
    TestSimplePointerReferences();
#endif


}


#if REFSYSTEM_DEBUG_MODE

struct Referent *RefSystem_ReferentFromHandle(ReferenceHandle *pHandle)
{
    struct Referent *pPointee;
    BSTNodeHandle nodeHandle;
    int iNumNodeHandles;
    BSTNode *pNode;
    ReferenceHandleInfoStruct *pRefInfo;

    pPointee = (struct Referent*)(*pHandle);
    iNumNodeHandles = BST_FindNodes(&sHandleTree, (intptr_t)pHandle, (intptr_t)pHandle, &nodeHandle, 1);

    if (iNumNodeHandles == 0)
    {
        //for some applications, it is legal for a NULL pointer to be a handle that is not active
        assert(pPointee == NULL);
        return pPointee;
    }

    assert(iNumNodeHandles == 1);
    
    pNode = BSTNodeFromHandle(nodeHandle);
    pRefInfo = (ReferenceHandleInfoStruct*)(pNode->data.pData);

    assert(pRefInfo->pHandle == pHandle);
    assert(pRefInfo->pReferent == pPointee);
    assert(pRefInfo->pReferent == pRefInfo->pReferentInfo->pDictionary->pDirectlyDecodeReferenceCB(pRefInfo->pReferentInfo->pOpaqueRefData));

    return pPointee;
}




void RefSystem_CheckIntegrity(bool bDumpReport)
{
    BinarySearchTreeSearcher searcher;
    BSTNodeHandle nodeHandle;

    static int iDebugID = 0;

    int iNodeCounts[MAX_REFERENCE_DICTIONARIES];
    int iDataCounts[MAX_REFERENCE_DICTIONARIES];

    memset(iNodeCounts, 0, sizeof(iNodeCounts));
    memset(iDataCounts, 0, sizeof(iNodeCounts));

    iDebugID++;

    BST_InitSearcher(&searcher, &sReferentTree, BST_SEARCHER_SPECIAL_FINDALL);

    while ((nodeHandle = BST_GetNextFromSearcher(&searcher)) != BSTNODE_HANDLE_INVALID)
    {
        BSTNode *pNode = BSTNodeFromHandle(nodeHandle);
        ReferenceHandleInfoStruct *pRefInfo = (ReferenceHandleInfoStruct*)(pNode->data.pData);

        //in REFSYSTEM_DEBUGMODE, RefSystem_ReferentFromHandle has all the checks we need already built into it
        RefSystem_ReferentFromHandle(pRefInfo->pHandle);

        iNodeCounts[pRefInfo->pReferentInfo->pDictionary - sReferenceDictionaries]++;

        //make sure we don't double-count data holders
        if (pRefInfo->pReferentInfo->iDebugID != iDebugID)
        {
            pRefInfo->pReferentInfo->iDebugID = iDebugID;
            iDataCounts[pRefInfo->pReferentInfo->pDictionary - sReferenceDictionaries]++;
        }

    }

    if (bDumpReport)
    {
        int i;

        printf("Pass 1:\n");

        for (i=0; i < siNumReferenceDictionaries; i++)
        {
            printf("Dictionary %s had %d active handles, %d distinct referents\n", sReferenceDictionaries[i].name, iNodeCounts[i], iDataCounts[i]);
        }
    }

    memset(iNodeCounts, 0, sizeof(iNodeCounts));
    memset(iDataCounts, 0, sizeof(iNodeCounts));

    iDebugID++;



    BST_InitSearcher(&searcher, &sRefHashTree, BST_SEARCHER_SPECIAL_FINDALL);

    while ((nodeHandle = BST_GetNextFromSearcher(&searcher)) != BSTNODE_HANDLE_INVALID)
    {
        BSTNode *pNode = BSTNodeFromHandle(nodeHandle);
        ReferenceHandleInfoStruct *pRefInfo = (ReferenceHandleInfoStruct*)(pNode->data.pData);

        //in REFSYSTEM_DEBUGMODE, RefSystem_ReferentFromHandle has all the checks we need already built into it
        RefSystem_ReferentFromHandle(pRefInfo->pHandle);

        iNodeCounts[pRefInfo->pReferentInfo->pDictionary - sReferenceDictionaries]++;

        //make sure we don't double-count data holders
        if (pRefInfo->pReferentInfo->iDebugID != iDebugID)
        {
            pRefInfo->pReferentInfo->iDebugID = iDebugID;
            iDataCounts[pRefInfo->pReferentInfo->pDictionary - sReferenceDictionaries]++;
        }
    }

    if (bDumpReport)
    {
        int i;

        printf("Pass 2:\n");

        for (i=0; i < siNumReferenceDictionaries; i++)
        {
            printf("Dictionary %s had %d active handles, %d distinct referents\n", sReferenceDictionaries[i].name, iNodeCounts[i], iDataCounts[i]);
        }
    }
}


#endif




#if REFSYSTEM_TEST_HARNESS

#if TEST_HARNESS_STRINGVERSION

//from here down, this is stuff for the reference test harness
#define RTH_NUM_OBJECTS 3000
#define RTH_NUM_REFS_PER_OBJECT 100
#define RTH_NUM_SPACES 10000

//for each object, what space it is currenly occupying, or -1 if it is "off"
int sRTHSpaceNums[RTH_NUM_OBJECTS];

struct RTHObject;


typedef struct
{
    REF_TO(struct RTHObject) objRef;
    int iObjectNum;
} RTHSingleReference;

typedef struct RTHObject
{
    bool bActive;
    RTHSingleReference references[RTH_NUM_REFS_PER_OBJECT];
} RTHObject;

static RTHObject sRTHSpace[RTH_NUM_SPACES];

static int sNumActiveRTHObjects;

int *gpTestRefArrayInts = NULL;
REFARRAY testRefArray;


int FindFreeRTHSpaceNum()
{
    int s = rand() % RTH_NUM_SPACES;

    while (sRTHSpace[s].bActive)
    {
        s = (s + 1 ) % RTH_NUM_SPACES;
    }

    return s;
}

struct ReferenceData *GetTempReferenceToObject(int iObjectNum)
{
    static char tempString[32];
    
    sprintf_s(tempString, 31, "<Object %d>", iObjectNum);    

    return (struct ReferenceData *)tempString;
}


void RTH_Test(void)
{
    int i, j, count;

    DictionaryHandle dictHandle = RefSystem_GetDictionaryHandleFromName("TestHarness");

    for (i=0; i < RTH_NUM_OBJECTS; i++)
    {
        sRTHSpaceNums[i] = -1;

        for (j=0; j < RTH_NUM_REFS_PER_OBJECT; j++)
        {
            CLEAR_UNATTACHED_REF(sRTHSpace[i].references[j].objRef);
        }
    }

    for (i=0; i < RTH_NUM_REFS_PER_OBJECT; i++)
    {
        sRTHSpace[i].bActive = false;
    }

    sNumActiveRTHObjects = 0;

    for (count=0; count < 1000; count++)
//    while (1)
    {
        int iRand;

        if (rand() % 10000 == 0)
        {
            //delete all objects and then assert that all lists are empty
            for (i=0; i < RTH_NUM_OBJECTS; i++)
            {
                if (sRTHSpaceNums[i] != -1)
                {
                    for (j=0; j < RTH_NUM_REFS_PER_OBJECT; j++)
                    {
                        REMOVE_REF_IF_ACTIVE(sRTHSpace[sRTHSpaceNums[i]].references[j].objRef);
                    }
                    sRTHSpace[sRTHSpaceNums[i]].bActive = false;
                    RefSystem_RemoveReferent((struct Referent*)(&sRTHSpace[sRTHSpaceNums[i]]), false);
                    sRTHSpaceNums[i] = -1;
                    sNumActiveRTHObjects--;

                }
            }

            RefSystem_AssertListsAreEmpty();
    
            RefSystem_CheckIntegrity(true);

            DEBUG_PRINTF("Removed all objects... things are shipshape\n");
        }

        //10% add an object, 5% remove an object, 20% remove and add handles, 65% move an object
        iRand = rand() % 100;

        if (iRand < 10) // add an object
        {
            if (sNumActiveRTHObjects < RTH_NUM_OBJECTS)
            {
                int iObjectNum = rand() % RTH_NUM_OBJECTS;
                int iSpaceNum = FindFreeRTHSpaceNum();
                RTHObject *pObject;

                while (sRTHSpaceNums[iObjectNum] != -1)
                {
                    iObjectNum = (iObjectNum + 1) % RTH_NUM_OBJECTS;
                }


                sRTHSpaceNums[iObjectNum] = iSpaceNum;
                
                pObject = &sRTHSpace[iSpaceNum];
                pObject->bActive = true;
            

                sNumActiveRTHObjects++;

                RefSystem_AddReferent(dictHandle, GetTempReferenceToObject(iObjectNum), (struct Referent*)pObject);

                for (i=0; i < RTH_NUM_REFS_PER_OBJECT; i++)
                {
                    pObject->references[i].iObjectNum = rand() % RTH_NUM_OBJECTS;

                    if (!ADD_REF("TestHarness", GetTempReferenceToObject(pObject->references[i].iObjectNum), pObject->references[i].objRef))
                    {
                        assert(0);
                    }
                }

                {
                    int iTemp = rand() % RTH_NUM_REFS_PER_OBJECT;
                    char *pString;
                    estrStackCreate(&pString,1000);
                    REF_STRING_FROM_HANDLE(&pString, pObject->references[iTemp].objRef);
                    DEBUG_PRINTF("Added object %d. Its reference %d is to %s\n", iObjectNum, iTemp, pString);
                    estrDestroy(&pString);
                }


            }
        }
        else if (iRand < 15) // remove an object
        {
            if (sNumActiveRTHObjects)
            {
                int iObjectNum = rand() % RTH_NUM_OBJECTS;

                while (sRTHSpaceNums[iObjectNum] == -1)
                {
                    iObjectNum = (iObjectNum + 1) % RTH_NUM_OBJECTS;
                }

                sRTHSpace[sRTHSpaceNums[iObjectNum]].bActive = false;
            
                for (j=0; j < RTH_NUM_REFS_PER_OBJECT; j++)
                {
                    REMOVE_REF_IF_ACTIVE(sRTHSpace[sRTHSpaceNums[iObjectNum]].references[j].objRef);
                }
    
                RefSystem_RemoveReferent((struct Referent*)&sRTHSpace[sRTHSpaceNums[iObjectNum]], false);
                sRTHSpaceNums[iObjectNum] = -1;
                sNumActiveRTHObjects--;

                DEBUG_PRINTF("Removed object %d\n", iObjectNum);

            }
        }
        else if (iRand < 35) //remove and add handles
        {
            if (sNumActiveRTHObjects)
            {
                int iObjectNum = rand() % RTH_NUM_OBJECTS;
                RTHObject *pObject;

                while (sRTHSpaceNums[iObjectNum] == -1)
                {
                    iObjectNum = (iObjectNum + 1) % RTH_NUM_OBJECTS;
                }

                pObject = &sRTHSpace[sRTHSpaceNums[iObjectNum]];

                for (i=0; i < RTH_NUM_REFS_PER_OBJECT; i++)
                {
                    REMOVE_REF(pObject->references[i].objRef);

                    pObject->references[i].iObjectNum = rand() % RTH_NUM_OBJECTS;

                    if (!ADD_REF(dictHandle, (struct ReferenceData*)GetTempReferenceToObject(pObject->references[i].iObjectNum), pObject->references[i].objRef))
                    {
                        assert(0);
                    }
                }
                
                DEBUG_PRINTF("Added and removed handles for object %d\n", iObjectNum);
            }
        }
        else // move an object
        {
            if (sNumActiveRTHObjects)
            {
                int iObjectNum = rand() % RTH_NUM_OBJECTS;
                int iNewSpaceNum = FindFreeRTHSpaceNum();

                while (sRTHSpaceNums[iObjectNum] == -1)
                {
                    iObjectNum = (iObjectNum + 1) % RTH_NUM_OBJECTS;
                }
                
                memcpy(&sRTHSpace[iNewSpaceNum], &sRTHSpace[sRTHSpaceNums[iObjectNum]], sizeof(RTHObject));

                for (j=0; j < RTH_NUM_REFS_PER_OBJECT; j++)
                {
                    if (IS_REF_HANDLE_ACTIVE(sRTHSpace[sRTHSpaceNums[iObjectNum]].references[j].objRef))
                    {
                        MOVE_REF(sRTHSpace[iNewSpaceNum].references[j].objRef,
                            sRTHSpace[sRTHSpaceNums[iObjectNum]].references[j].objRef);
                    }
                }
    
                RefSystem_MoveReferent((struct Referent*)&sRTHSpace[iNewSpaceNum],
                    (struct Referent *)&sRTHSpace[sRTHSpaceNums[iObjectNum]]);


                sRTHSpace[sRTHSpaceNums[iObjectNum]].bActive = false;
                sRTHSpaceNums[iObjectNum] = iNewSpaceNum;

                DEBUG_PRINTF("Moved object %d\n", iObjectNum);
            }
        }

        //now verify correctness
        RefSystem_CheckIntegrity(false);

        for (i=0; i < RTH_NUM_OBJECTS; i++)
        {
            if (sRTHSpaceNums[i] != -1)
            {
                int j;
                RTHObject *pObject = &sRTHSpace[sRTHSpaceNums[i]];

                for (j=0; j < RTH_NUM_REFS_PER_OBJECT; j++)
                {
                    RTHObject *pReferent;

                    if (sRTHSpaceNums[pObject->references[j].iObjectNum] == -1)
                    {
                        pReferent = NULL;
                    }
                    else
                    {
                        pReferent = &sRTHSpace[sRTHSpaceNums[pObject->references[j].iObjectNum]];
                    }
                    assert(GET_REF(pObject->references[j].objRef) == (struct Referent*)pReferent);
                }
            }
        }
    }

    //now start mucking with REFARRAY
    CREATE_REFARRAY(testRefArray);

    for (i=0;i<1000;i++)
    {
        int iObjectNum = rand() % RTH_NUM_OBJECTS;

        PUSH_REFARRAY("TestHarness", GetTempReferenceToObject(iObjectNum), testRefArray);
        eaiPush(&gpTestRefArrayInts, iObjectNum);
    }


    for (i=0; i < 1000; i++)
    {
        int iObjectNum = rand() % RTH_NUM_OBJECTS;
        int iSlotNum = rand() % REFARRAY_SIZE(testRefArray);

        INSERT_REFARRAY("TestHarness", GetTempReferenceToObject(iObjectNum), testRefArray, iSlotNum);
        eaiInsert(&gpTestRefArrayInts, iObjectNum, iSlotNum);
    }


    for (i=0; i < 1000; i++)
    {
        int iSlotNum = rand() % REFARRAY_SIZE(testRefArray);

        REMOVE_REFARRAY_FAST(testRefArray, iSlotNum);
        eaiRemoveFast(&gpTestRefArrayInts, iSlotNum);
    }

    for (i=0; i < REFARRAY_SIZE(testRefArray); i++)
    {
        struct Referent *pReferent = GET_REF_FROM_REFARRAY(testRefArray, i);
        struct Referent *pOtherReferent;

        int iObjNum = gpTestRefArrayInts[i];
        int iSpaceNum = sRTHSpaceNums[iObjNum];

        if (iSpaceNum == -1)
        {
            pOtherReferent = NULL;
        }
        else
        {
            pOtherReferent = (struct Referent*)&sRTHSpace[iSpaceNum];
        }

        assertmsg(pReferent == pOtherReferent, "REFARRAY corruption");
    }


}

struct Referent *RTHDirectlyDecodeReference(struct ReferenceData *pRefData)
{
    char *pString = (char*)pRefData;
    int iObjNum;

    sscanf(pString, "<Object %d>", &iObjNum);

    if (sRTHSpaceNums[iObjNum] == -1)
    {
        return NULL;
    }

    return (struct Referent*)&sRTHSpace[sRTHSpaceNums[iObjNum]];
}


#else





//from here down, this is stuff for the reference test harness
#define RTH_NUM_OBJECTS 3000
#define RTH_NUM_REFS_PER_OBJECT 100
#define RTH_NUM_SPACES 10000

//for each object, what space it is currenly occupying, or -1 if it is "off"
int sRTHSpaceNums[RTH_NUM_OBJECTS];

struct RTHObject;

typedef struct
{
    int iObjectNum;
} RTHReferenceData;

typedef struct
{
    REF_TO(struct RTHObject) objRef;
    int iObjectNum;
} RTHSingleReference;

typedef struct RTHObject
{
    bool bActive;
    RTHSingleReference references[RTH_NUM_REFS_PER_OBJECT];
} RTHObject;

static RTHObject sRTHSpace[RTH_NUM_SPACES];

static int sNumActiveRTHObjects;

RTHReferenceData *GetTempReferenceToObject(int iObjectNum)
{
    static RTHReferenceData refData;

    refData.iObjectNum = iObjectNum;

    return &refData;
}


int FindFreeRTHSpaceNum()
{
    int s = rand() % RTH_NUM_SPACES;

    while (sRTHSpace[s].bActive)
    {
        s = (s + 1 ) % RTH_NUM_SPACES;
    }

    return s;
}


void RTH_Test(void)
{
    int i, j;

    for (i=0; i < RTH_NUM_OBJECTS; i++)
    {
        sRTHSpaceNums[i] = -1;

        for (j=0; j < RTH_NUM_REFS_PER_OBJECT; j++)
        {
            CLEAR_UNATTACHED_REF(sRTHSpace[i].references[j].objRef);
        }
    }

    for (i=0; i < RTH_NUM_REFS_PER_OBJECT; i++)
    {
        sRTHSpace[i].bActive = false;
    }

    sNumActiveRTHObjects = 0;

//    for (j=0; j < 100; j++)
    while (1)
    {
        int iRand;

        if (rand() % 10000 == 0)
        {
            //delete all objects and then assert that all lists are empty
            for (i=0; i < RTH_NUM_OBJECTS; i++)
            {
                if (sRTHSpaceNums[i] != -1)
                {
                    sRTHSpace[sRTHSpaceNums[i]].bActive = false;
                    RefSystem_RemoveBlock(sRTHSpace + sRTHSpaceNums[i], sizeof(RTHObject), false);
                    sRTHSpaceNums[i] = -1;
                    sNumActiveRTHObjects--;

                }
            }

            RefSystem_AssertListsAreEmpty();

            BSTSystem_AssertIsEmpty();
    
            RefSystem_CheckIntegrity(true);

            DEBUG_PRINTF("Removed all objects... things are shipshape\n");
        }

        //10% add an object, 5% remove an object, 20% remove and add handles, 65% move an object
        iRand = rand() % 100;

        if (iRand < 10) // add an object
        {
            if (sNumActiveRTHObjects < RTH_NUM_OBJECTS)
            {
                int iObjectNum = rand() % RTH_NUM_OBJECTS;
                int iSpaceNum = FindFreeRTHSpaceNum();
                RTHObject *pObject;

                while (sRTHSpaceNums[iObjectNum] != -1)
                {
                    iObjectNum = (iObjectNum + 1) % RTH_NUM_OBJECTS;
                }


                sRTHSpaceNums[iObjectNum] = iSpaceNum;
                
                pObject = &sRTHSpace[iSpaceNum];
                pObject->bActive = true;
            

                sNumActiveRTHObjects++;

                RefSystem_AddReferent("TestHarness", (struct ReferenceData*)GetTempReferenceToObject(iObjectNum), (struct Referent*)pObject);

                for (i=0; i < RTH_NUM_REFS_PER_OBJECT; i++)
                {
                    char tempString[32];
                    pObject->references[i].iObjectNum = rand() % RTH_NUM_OBJECTS;
                    sprintf_s(tempString, 31, "<Object %d>", pObject->references[i].iObjectNum);

                    if (!ADD_REF_FROM_STRING("TestHarness", tempString, pObject->references[i].objRef))
                    {
                        assert(0);
                    }
                }

                {
                    int iTemp = rand() % RTH_NUM_REFS_PER_OBJECT;
                    char *pString;
                    estrStackCreate(&pString,1000);
                    REF_STRING_FROM_HANDLE(&pString, pObject->references[iTemp].objRef);
                    DEBUG_PRINTF("Added an object. Its reference %d is to %s\n", iTemp, pString);
                    estrDestroy(&pString);
                }


            }
        }
        else if (iRand < 15) // remove an object
        {
            if (sNumActiveRTHObjects)
            {
                int iObjectNum = rand() % RTH_NUM_OBJECTS;

                while (sRTHSpaceNums[iObjectNum] == -1)
                {
                    iObjectNum = (iObjectNum + 1) % RTH_NUM_OBJECTS;
                }

                sRTHSpace[sRTHSpaceNums[iObjectNum]].bActive = false;
                RefSystem_RemoveBlock(sRTHSpace + sRTHSpaceNums[iObjectNum], sizeof(RTHObject), false);
                sRTHSpaceNums[iObjectNum] = -1;
                sNumActiveRTHObjects--;

            }
        }
        else if (iRand < 35) //remove and add handles
        {
            if (sNumActiveRTHObjects)
            {
                int iObjectNum = rand() % RTH_NUM_OBJECTS;
                RTHObject *pObject;

                while (sRTHSpaceNums[iObjectNum] == -1)
                {
                    iObjectNum = (iObjectNum + 1) % RTH_NUM_OBJECTS;
                }

                pObject = &sRTHSpace[sRTHSpaceNums[iObjectNum]];

                for (i=0; i < RTH_NUM_REFS_PER_OBJECT; i++)
                {
                    REMOVE_REF(pObject->references[i].objRef);

                    pObject->references[i].iObjectNum = rand() % RTH_NUM_OBJECTS;

                    if (!ADD_REF("TestHarness", (struct ReferenceData*)GetTempReferenceToObject(pObject->references[i].iObjectNum), pObject->references[i].objRef))
                    {
                        assert(0);
                    }
                }
            }
        }
        else // move an object
        {
            if (sNumActiveRTHObjects)
            {
                int iObjectNum = rand() % RTH_NUM_OBJECTS;
                int iNewSpaceNum = FindFreeRTHSpaceNum();

                while (sRTHSpaceNums[iObjectNum] == -1)
                {
                    iObjectNum = (iObjectNum + 1) % RTH_NUM_OBJECTS;
                }
                
                memcpy(&sRTHSpace[iNewSpaceNum], &sRTHSpace[sRTHSpaceNums[iObjectNum]], sizeof(RTHObject));
                RefSystem_MoveBlock(&sRTHSpace[iNewSpaceNum], &sRTHSpace[sRTHSpaceNums[iObjectNum]], sizeof(RTHObject));
                sRTHSpace[sRTHSpaceNums[iObjectNum]].bActive = false;
                sRTHSpaceNums[iObjectNum] = iNewSpaceNum;
            }
        }

        //now verify correctness
        RefSystem_CheckIntegrity(false);

        for (i=0; i < RTH_NUM_OBJECTS; i++)
        {
            if (sRTHSpaceNums[i] != -1)
            {
                int j;
                RTHObject *pObject = &sRTHSpace[sRTHSpaceNums[i]];

                for (j=0; j < RTH_NUM_REFS_PER_OBJECT; j++)
                {
                    RTHObject *pReferent;

                    if (sRTHSpaceNums[pObject->references[j].iObjectNum] == -1)
                    {
                        pReferent = NULL;
                    }
                    else
                    {
                        pReferent = &sRTHSpace[sRTHSpaceNums[pObject->references[j].iObjectNum]];
                    }
                    assert(GET_REF(pObject->references[j].objRef) == (struct Referent*)pReferent);
                }
            }
        }
    }
}

struct Referent *RTHDirectlyDecodeReference(struct ReferenceData *pRefData)
{
    RTHReferenceData *pRTHRefData = (RTHReferenceData*)pRefData;

    int iObjNum = pRTHRefData->iObjectNum;
        
    assert(iObjNum >= 0 && iObjNum < RTH_NUM_OBJECTS);

    if (sRTHSpaceNums[iObjNum] == -1)
    {
        return NULL;
    }

    return (struct Referent*)&sRTHSpace[sRTHSpaceNums[iObjNum]];
}

ReferenceHashValue RTHGetHashFromReferenceData(struct ReferenceData *pRefData)
{
    RTHReferenceData *pRTHRefData = (RTHReferenceData*)pRefData;

    return pRTHRefData->iObjectNum + 1;
}

struct ReferenceData *RTHCopyReferenceData(struct ReferenceData *pRefData)
{
    RTHReferenceData *pRTHRefData = (RTHReferenceData*)pRefData;

    RTHReferenceData *pNewRefData = (RTHReferenceData*)malloc(sizeof(RTHReferenceData));

    memcpy(pNewRefData, pRTHRefData, sizeof(RTHReferenceData));
    

    return (struct ReferenceData*)pNewRefData;
}

void RTHFreeReferenceData(struct ReferenceData *pRefData)
{
    free(pRefData);
}

//This function converts a reference data to a string, and APPENDS that string to the EString
bool RTHReferenceDataToString(char **ppEString, struct ReferenceData *pRefData)
{
    char tempString[32];
    RTHReferenceData *pRTHRefData = (RTHReferenceData*)pRefData;


    sprintf_s(tempString, 31, "<Object %d>", pRTHRefData->iObjectNum);

    estrConcatMinimumWidth(ppEString, tempString, (int)strlen(tempString));
    return true;
}



//This function converts a string to a reference and returns the reference data, which can 
//be freed by calling RefCallBack_FreeReferenceData
struct ReferenceData *RTHStringToReferenceData(char *pString)
{
    int iObjectNum;
    RTHReferenceData *pNewRefData;
    int iRetVal = sscanf(pString, "<Object %d>", &iObjectNum);

    if (iRetVal != 1)
    {
        return NULL;
    }
    
    pNewRefData = (RTHReferenceData*)malloc(sizeof(RTHReferenceData));

    pNewRefData->iObjectNum = iObjectNum;


    return (struct ReferenceData*)pNewRefData;
}
#endif
#endif



#if TEST_SIMPLE_POINTER_REFERENCES
typedef struct fooStruct
{
    int x, y;
} fooStruct;

typedef struct barStruct
{
    REF_TO(fooStruct) hMyFoo;
} barStruct;



void TestSimplePointerReferences()
{
    barStruct myBar;
    fooStruct *pMyFoo;

    CLEAR_UNATTACHED_REF(myBar.hMyFoo);

    printf("myBar's foo = %x\n", GET_REF(myBar.hMyFoo));

    pMyFoo = malloc(sizeof(fooStruct));

    printf("pMyFoo is %x\n", pMyFoo);

    ADD_SIMPLE_POINTER_REFERENCE(myBar.hMyFoo, pMyFoo);

    printf("myBar's foo = %x\n", GET_REF(myBar.hMyFoo));

    RefSystem_RemoveBlock(pMyFoo, sizeof(fooStruct), true);

    printf("myBar's foo = %x\n", GET_REF(myBar.hMyFoo));
}


#endif













