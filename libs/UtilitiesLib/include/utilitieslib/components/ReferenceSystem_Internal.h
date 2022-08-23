#ifndef _REFERENCESYSTEM_INTERNAL_H_
#define _REFERENCESYSTEM_INTERNAL_H_

#include "referencesystem.h"
#include "StashTable.h"

#define REFERENCEDICTIONARY_MAX_NAME_LENGTH 32

#define MAX_REFERENCE_DICTIONARIES 64

typedef struct
{
    char name[REFERENCEDICTIONARY_MAX_NAME_LENGTH];
    int iNameHash;
    bool bUsesStringsAsReferenceData;
    bool bCaseSensitiveHashing;
    bool bIsSelfDefining;
    bool bDontCheckForHashCollisions;

    RefCallBack_DirectlyDecodeReference *pDirectlyDecodeReferenceCB;
    RefCallBack_GetHashFromReferenceData *pGetHashFromReferenceDataCB;
    RefCallBack_CopyReferenceData *pCopyReferenceDataCB;
    RefCallBack_FreeReferenceData *pFreeReferenceDataCB;
    RefCallBack_ReferenceDataToString *pRefDataToStringCB;
    RefCallBack_StringToReferenceData *pStringToRefDataCB;
    RefCallBack_GetVersionFromReferent *pGetVersionCB;

    StashTable hashedRefDataTable;

} ReferenceDictionary;

typedef struct
{
    union
    {
        //used by dictionaries with bUsesStringsAsReferenceData set
        char *pStringRefData;

        //used by other dictionaries
        struct ReferenceData *pOpaqueRefData;
    };
    struct Referent *pReferent;
    ReferenceDictionary *pDictionary;
    ReferenceHashValue iReferenceHash;

    //versions are only used for dictionaries that 
    //(1) have bUsesStringsAsReferenceData set
    //(2) have a RefCallBack_GetVersionFromReferent callback
    //
    //they are only relevant for volatile handles
    U32 iReferentVersion;

    ReferenceHandle **ppHandles; //Earray of pointers to handles.
    ReferenceHandle **ppVolatileHandles; //Earray of pointers to handles.

#if REFSYSTEM_DEBUG_MODE
    int iDebugID;
#endif
} ReferentInfoStruct;



ReferentInfoStruct *GetFreeReferentInfo(void);
void ReleaseReferentInfo(ReferentInfoStruct *pReferentInfo);


static INLINEDBG void SetRefHandle(ReferenceHandle *pHandle, struct Referent *pReferent)
{
    *pHandle = (void*)pReferent;
}

ReferenceDictionary *RefDictionaryFromName(char *pDictionaryName);


#endif
