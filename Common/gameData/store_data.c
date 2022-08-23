/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <string.h>

#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/assert/assert.h>

#include "gameComm/Npc.h" // for PowerNameRef
#include "entity/powers.h"

#include "store.h"
#include "store_data.h"

static void ShareStructure(char *pchName, TokenizerParseInfo *ptpi, void *pvSrc, size_t iSizeSrc, void (*DoWork)(void *), void *param);
static void IndexStoresStatic(void);
static void IndexStoresShared(void);

static StoreIndex *CreateStoreIndex(Store *pstore);

SHARED_MEMORY Departments g_Departments;
    // All the dept names.

DepartmentContents g_DepartmentContents;
    // The contents of every department. Shared.

SHARED_MEMORY StoreItems g_Items;
    // All the items. Shared.

Stores g_Stores;
    // All the stores. Shared.

StoreHashes g_StoreHashes;

StashTable g_hashStores;
    // Maps store names to Store *s

#ifdef SERVER
#define SUFFIX "_SERVER"
#else
#define SUFFIX "_CLIENT"
#endif

/***************************************************************************/
/***************************************************************************/

TokenizerParseInfo ParseDepartment[] =
{
    { "Name",  TOK_STRUCTPARAM | TOK_STRING(Department, pchName, 0) },
    { "\n",    TOK_END,                      0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseDepartments[] =
{
    { "Department", TOK_STRUCT(Departments, ppDepartments, ParseDepartment) },
    { "", 0, 0 }
};

TokenizerParseInfo ParseDepartmentContent[] =
{
    { "Items", TOK_AUTOINTEARRAY(DepartmentContent, ppItems) }, // Here for memory sharing
    { "\n",    TOK_END,                      0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseDepartmentsContents[] =
{
    { "Content", TOK_STRUCT(DepartmentContents, ppDepartments, ParseDepartmentContent) },
    { "", 0, 0 }
};

/***************************************************************************/
/***************************************************************************/

TokenizerParseInfo ParseStoreItem[] =
{
    { "{",            TOK_START,                       0 },
    { "Name",         TOK_STRUCTPARAM | TOK_STRING(StoreItem, pchName, 0) },
    { "Power",        TOK_EMBEDDEDSTRUCT(StoreItem, power, ParsePowerNameRef) },
    { "Sell",         TOK_INT(StoreItem, iSell, 0) },
    { "Buy",          TOK_INT(StoreItem, iBuy, 0) },
    { "CountPerStore",TOK_INT(StoreItem, iCntPerStore, 0) },
    { "Departments",  TOK_INTARRAY(StoreItem, piDepartments) },
    { "}",            TOK_END,                         0 },
    { "", 0, 0 }
};


TokenizerParseInfo ParseStoreItems[] =
{
    { "Item",        TOK_STRUCT(StoreItems, ppItems, ParseStoreItem) },
    { "", 0, 0 }
};

/***************************************************************************/
/***************************************************************************/

TokenizerParseInfo ParseMarkup[] =
{
    { "Department",   TOK_STRUCTPARAM|TOK_INT(Markup, iDepartment, 0) },
    { "Markup",       TOK_STRUCTPARAM|TOK_F32(Markup, fMarkup, 0) },
    { "\n",           TOK_END,                         0 },
    { "", 0, 0 }
};

/***************************************************************************/
/***************************************************************************/

typedef struct ItemName
{
    char *pch;
} ItemName;

TokenizerParseInfo ParseItemName[] =
{
    { "Name",         TOK_STRUCTPARAM|TOK_STRING(ItemName, pch, 0) },
    { "\n",           TOK_END,                         0 },
    { "", 0, 0 }
};

typedef struct StoreDef
{
    char        *pchName;

    Markup        **ppSells;
    Markup        **ppBuys;
    ItemName    **ppItemNames;

    float        fBuySalvage;
    float        fBuyRecipe;
} StoreDef;

TokenizerParseInfo ParseStoreDef[] =
{
    { "{",            TOK_START,                       0 },
    { "Name",         TOK_STRUCTPARAM|TOK_STRING(StoreDef, pchName, 0) },
    { "Sell",         TOK_STRUCT(StoreDef, ppSells, ParseMarkup) },
    { "Buy",          TOK_STRUCT(StoreDef, ppBuys, ParseMarkup) },
    { "Item",         TOK_STRUCT(StoreDef, ppItemNames, ParseItemName) },
    { "Salvage",      TOK_F32(StoreDef, fBuySalvage, 0.0f ) },
    { "Recipe",          TOK_F32(StoreDef, fBuyRecipe, 0.0f ) },
    { "}",            TOK_END,                         0 },
    { "", 0, 0 }
};

/***************************************************************************/
/***************************************************************************/

typedef struct StoreDefs
{
    StoreDef **ppStoreDefs;
} StoreDefs;

StoreDefs g_StoreDefs;

TokenizerParseInfo ParseStoreDefs[] =
{
    { "Store",  TOK_STRUCT(StoreDefs, ppStoreDefs, ParseStoreDef) },
    { "", 0, 0 }
};


/***************************************************************************/
/***************************************************************************/

TokenizerParseInfo ParseStore[] = // used for sharing memory
{
    { "{",            TOK_START,                       0 },
    { "Name",         TOK_STRING(Store, pchName, 0) },
    { "Sell",         TOK_STRUCT(Store, ppSells, ParseMarkup) },
    { "Buy",          TOK_STRUCT(Store, ppBuys, ParseMarkup) },
    { "Salvage",      TOK_F32(Store, fBuySalvage, 0.0f ) },
    { "Recipe",          TOK_F32(Store, fBuyRecipe, 0.0f ) },
    { "}",            TOK_END,                         0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseStores[] = // used for sharing memory
{
    { "Store",        TOK_STRUCT(Stores, ppStores, ParseStore) },
    { "", 0, 0 }
};


TokenizerParseInfo ParseStoreIndex[] = // used for sharing memory
{
    { "hashBuy",    TOK_STASHTABLE(StoreIndex, hashBuy) },
    { "hashSell",    TOK_STASHTABLE(StoreIndex, hashSell) },
    { "", 0, 0 }
};

TokenizerParseInfo ParseStoreHashes[] = // used for sharing memory
{
    { "hashItemsByPower",    TOK_STASHTABLE(StoreHashes, hashItemsByPower) },
    { "StoreIndicies",        TOK_STRUCT(StoreHashes, StoreIndicies, ParseStoreIndex) },
    { "", 0, 0 }
};

/**********************************************************************func*
 * StoreHashesWork
 *
 */
static void StoreHashesWork(void)
{
    int i, iCnt;

    iCnt = eaSize(&g_Items.ppItems);

    if(!g_StoreHashes.hashItemsByPower)
    {
        g_StoreHashes.hashItemsByPower = stashTableCreateInt(iCnt);
    }
    for(i=0; i<iCnt; i++)
    {
        // Index the item by power pointer
        stashIntAddPointerConst(g_StoreHashes.hashItemsByPower,(int)(intptr_t)ITEM_HASH_FROM_ITEM(g_Items.ppItems[i]),g_Items.ppItems[i], false);
    }

    iCnt = eaSize(&g_Stores.ppStores);
    eaCreate(&g_StoreHashes.StoreIndicies);
    eaSetSize(&g_StoreHashes.StoreIndicies, iCnt);
}

/**********************************************************************func*
 * IndexStore
 *   Indexes the store stuff that's shared
 */
void store_IndexByIdx(int iStore)
{
    eaSet(&g_StoreHashes.StoreIndicies, CreateStoreIndex(g_Stores.ppStores[iStore]), iStore);
}

#if SERVER
/**********************************************************************func*
 * IndexStoresShared
 *   Indexes the store stuff that's shared
 */
static void IndexStoresShared(void)
{
    int i;
    int iCnt;

    iCnt = eaSize(&g_Stores.ppStores);
    for(i=0; i<iCnt; i++)
    {
        store_IndexByIdx(i);
    }
}

static void StoreHashesWorkServer(void *junk)
{
    StoreHashesWork();
    IndexStoresShared();
}

#else // CLIENT

/**********************************************************************func*
 * StoreHashesWorkClient
 *
 */
static void StoreHashesWorkClient(void *junk)
{
    StoreHashesWork();
}

#endif // else CLIENT

/**********************************************************************func*
 * IndexItems
 *
 */
static bool StoreItemsFinalProcess(ParseTable pti[], StoreItems *sitems, bool shared_memory)
{
    bool ret = true;
    int i, iCnt;

    iCnt = eaSize(&g_Items.ppItems);

    assert(!sitems->hashItems);
    sitems->hashItems = stashTableCreateWithStringKeys(stashOptimalSize(iCnt), stashShared(shared_memory));

    for(i=0; i<iCnt; i++)
    {
        // Index the item by name
        if(!stashAddPointerConst(sitems->hashItems, sitems->ppItems[i]->pchName, sitems->ppItems[i], false))
        {
            ErrorFilenamef("defs/stores/items.def", "Duplicate item name \"%s\"", sitems->ppItems[i]->pchName);
            ret = false;
        }
    }

    return ret;
}


/**********************************************************************func*
 * PopulateDepartmentContents
 *
 */
static void PopulateDepartmentContents(void *unused)
{
    int i;
    int iCntDept, iCntItems;

    if(g_DepartmentContents.ppDepartments==NULL)
        eaCreate(&g_DepartmentContents.ppDepartments);

    iCntDept = eaSize(&g_Departments.ppDepartments);
    for(i=0; i<iCntDept; i++)
    {
        DepartmentContent *pdc = ParserAllocStruct(sizeof(DepartmentContent));
        memset(pdc,0,sizeof(DepartmentContent));
        eaCreate(&pdc->ppItems);
        eaPush(&g_DepartmentContents.ppDepartments, pdc);
    }

    iCntItems = eaSize(&g_Items.ppItems);
    for(i=0; i<iCntItems; i++)
    {
        int j;
        int jCnt;
        jCnt = eaiSize(&g_Items.ppItems[i]->piDepartments);
        for(j=0; j<jCnt; j++)
        {
            int idx = g_Items.ppItems[i]->piDepartments[j];
            if( idx >= 0 && idx < iCntDept )
                eaPushConst(&g_DepartmentContents.ppDepartments[idx]->ppItems, g_Items.ppItems[i]);
        }
    }
}

/**********************************************************************func*
 * CreateStores
 *
 */
void CreateStores(void)
{
    int iCntStores;
    int iCntDepts = eaSize(&g_DepartmentContents.ppDepartments);
    int i;

    if(g_Stores.ppStores==NULL)
    {
        eaCreate(&g_Stores.ppStores);
    }
    else
    {
        // Erase all the store data and reload it.
        iCntStores = eaSize(&g_Stores.ppStores);
        for(i=0; i<iCntStores; i++)
        {
            Store *pstore = g_Stores.ppStores[i];
            eaClearEx(&pstore->ppBuys, NULL);
            eaClearEx(&pstore->ppSells, NULL);
            eaClearConst(&pstore->ppItems);
            free(pstore->pchName);
        }
        eaClearEx(&g_Stores.ppStores, NULL);
        eaCreate(&g_Stores.ppStores);
    }

    iCntStores = eaSize(&g_StoreDefs.ppStoreDefs);
    for(i=0; i<iCntStores; i++)
    {
        int j;
        int iCnt;
        Store *pstore = ParserAllocStruct(sizeof(Store));
        StoreDef *pdef = g_StoreDefs.ppStoreDefs[i];

        memset(pstore, 0, sizeof(Store));

        // First, remember the name
        pstore->pchName = pdef->pchName;
        pdef->pchName = NULL;

        pstore->iIdx = i;

        // OK, the StoreDef is a sparse list of the departments.
        // The real Store has a full list, which makes it easy to check to
        // see if a particular department is available in the store.
        eaCreate(&pstore->ppBuys);
        eaSetSize(&pstore->ppBuys, iCntDepts);
        eaCreate(&pstore->ppSells);
        eaSetSize(&pstore->ppSells, iCntDepts);

        iCnt = eaSize(&pdef->ppBuys);
        for(j=0; j<iCnt; j++)
        {
            if(pdef->ppBuys[j]->fMarkup!=0.0f)
            {
                eaSet(&pstore->ppBuys, pdef->ppBuys[j], pdef->ppBuys[j]->iDepartment);
                pdef->ppBuys[j] = NULL;
            }
        }
        iCnt = eaSize(&pdef->ppSells);
        for(j=0; j<iCnt; j++)
        {
            if(pdef->ppSells[j]->fMarkup!=0.0f)
            {
                eaSet(&pstore->ppSells, pdef->ppSells[j], pdef->ppSells[j]->iDepartment);
                pdef->ppSells[j] = NULL;
            }
        }

        // And now process all the items
        eaCreateConst(&pstore->ppItems);
        iCnt = eaSize(&pdef->ppItemNames);
        for(j=0; j<iCnt; j++)
        {
            const StoreItem *pitem = store_FindItemByName(pdef->ppItemNames[j]->pch);
            if(pitem)
            {
                eaPushConst(&pstore->ppItems, pitem);
            }
            else
            {
                ErrorFilenamef("defs/stores/stores.def", "Store %s: There is no item named \"%s\"", pstore->pchName, pdef->ppItemNames[j]->pch);
            }
        }

        // copy salvage and recipe costs
        pstore->fBuySalvage = pdef->fBuySalvage;
        pstore->fBuyRecipe = pdef->fBuyRecipe;

        // Add the store to the list.
        eaPush(&g_Stores.ppStores, pstore);
    }

    // At this point, all the data has been transferred from the StoreDefs
    // to the Stores. Data allocated to make the StoreDefs which is being used
    // by the Store has been NULLed, so it should be safe to to a
    // ParserDestroyStruct on the original.
    ParserDestroyStruct(ParseStoreDefs, &g_StoreDefs);
}

/**********************************************************************func*
 * PostProcItems
 *
 */
static bool PostProcItems(ParseTable pti[], StoreItems* sitems)
{
    int i;
    int iCnt = eaSize(&g_Items.ppItems);
    for(i=0; i<iCnt; i++)
    {
        StoreItem* item = cpp_const_cast(StoreItem*)(sitems->ppItems[i]);

        item->ppowBase = powerdict_GetBasePowerByName(&g_PowerDictionary,
            item->power.powerCategory,
            item->power.powerSet,
            item->power.power);

        if(!item->ppowBase)
        {
            ErrorFilenamef("defs/stores/items.def", "Item %s: There is no power named \"%s.%s.%s\"",
            item->pchName,
            item->power.powerCategory,
            item->power.powerSet,
            item->power.power);
            eaRemoveConst(&sitems->ppItems, i);
            i--;
            iCnt--;
        }
    }
    return true;
}


/**********************************************************************func*
 * ShareStructure
 *
 */
static void ShareStructure(char *pchName, TokenizerParseInfo *ptpi,
    void *pvSrc, size_t iSizeSrc, void (*DoWork)(void *), void *param)
{
    SharedMemoryHandle *shared_memory=NULL;
    SM_AcquireResult ret;

    // All this is to share the store info.
    ret = sharedMemoryAcquire(&shared_memory, pchName);
    if(ret==SMAR_DataAcquired)
    {
        memcpy(pvSrc, sharedMemoryGetDataPtr(shared_memory), iSizeSrc);
    }
    else if(ret==SMAR_Error)
    {
        DoWork(param);
    }
    else if(ret==SMAR_FirstCaller)
    {
        unsigned long size;
        void *pTemp;

        DoWork(param);

        // Load data and copy to shared memory
        // Copy to shared memory
        size = (unsigned long)ParserGetStructMemoryUsage(ptpi, pvSrc, iSizeSrc);
        sharedMemorySetSize(shared_memory, size);
        pTemp = ParserCompressStruct(ptpi, pvSrc, iSizeSrc, NULL, sharedMemoryAlloc, shared_memory);
        ParserDestroyStruct(ptpi, pvSrc);
        memcpy(pvSrc, pTemp, iSizeSrc);
        sharedMemoryUnlock(shared_memory);
    }
}

/**********************************************************************func*
 * LoadStores
 *
 */
static void LoadStores(void *pdef)
{
    // Stores are loaded into a temporary structure and then turned into
    // a more efficient form.
    ParserLoadFiles(NULL, "defs/stores/stores.def", "stores.bin",
        0, ParseStoreDefs, &g_StoreDefs,
        (DefineContext *)pdef, NULL, NULL);
    CreateStores();
}

/**********************************************************************func*
 * CreateStoreHash
 *
 */
static StashTable CreateStoreHash(Markup ***pppMarkups)
{
    int i, iCnt;
    int iTotal = 0;
    StashTable stash;

    // Make a good starting guess.
    iCnt = eaSize(pppMarkups);
    for(i=0; i<iCnt; i++)
    {
        if((*pppMarkups)[i]!=NULL)
        {
            iTotal += eaSize(&g_DepartmentContents.ppDepartments[i]->ppItems);
        }
    }

    stash = stashTableCreate(iTotal, StashDefault, StashKeyTypeFixedSize, sizeof(StoreItem));

    for(i=0; i<iCnt; i++)
    {
        if((*pppMarkups)[i]!=NULL)
        {
            int j;
            int jCnt;

            jCnt = eaSize(&g_DepartmentContents.ppDepartments[i]->ppItems);
            for(j=0; j<jCnt; j++)
            {
                StashElement elem;

                StoreItem *psi = g_DepartmentContents.ppDepartments[i]->ppItems[j];


                if(!stashFindElement(stash, psi, &elem))
                {
                    stashAddInt(stash, psi, i, false);
                }
                else
                {
                    int iDept = stashElementGetInt(elem);
                    if((*pppMarkups)[i]->fMarkup > (*pppMarkups)[iDept]->fMarkup)
                    {
                        stashElementSetInt(elem, i);
                    }
                }
            }
        }
    }

    return stash;
}

/**********************************************************************func*
 * CreateStoreIndex
 *
 */
static StoreIndex *CreateStoreIndex(Store *pstore)
{
    StoreIndex *pidx;

    pidx = (StoreIndex *)ParserAllocStruct(sizeof(StoreIndex));

    pidx->hashBuy = CreateStoreHash(&pstore->ppBuys);
    pidx->hashSell = CreateStoreHash(&pstore->ppSells);

    return pidx;
}

/**********************************************************************func*
* IndexStoresShared
*   Indexes the store stuff that's static in each mapserver
*/
static void IndexStoresStatic(void)
{
    int i;
    int iCnt;

    iCnt = eaSize(&g_Stores.ppStores);

    for(i=0; i<iCnt; i++)
    {
        if(!stashAddPointer(g_hashStores, g_Stores.ppStores[i]->pchName, g_Stores.ppStores[i], false))
        {
            ErrorFilenamef("defs/stores/stores.def", "Duplicate store name \"%s\"", g_Stores.ppStores[i]->pchName);
        }
    }
}

/**********************************************************************func*
 * load_Stores
 *
 */
void load_Stores(void)
{
    int i;
    int iCnt;
    DefineContext *pdef;

    // Make a few hash table for indexing the items and stores
    if(!g_hashStores)
        g_hashStores = stashTableCreateWithStringKeys(100,StashDefault);

    // Load up the Departments first. Their names are used in the
    // definition of items and stores.
    ParserLoadFilesShared("SM_Depts" SUFFIX, NULL, "defs/stores/depts.def", "depts.bin",
        0, ParseDepartments, &g_Departments, sizeof(Departments),
        NULL, NULL, NULL, NULL, NULL);

    pdef = DefineCreate();
    iCnt = eaSize(&g_Departments.ppDepartments);
    for(i=0; i<iCnt; i++)
    {
        char ach[256];
        char val[256];

        sprintf(ach, "k%s", g_Departments.ppDepartments[i]->pchName);
        sprintf(val, "%d", i);
        DefineAdd(pdef, ach, val);
    }

    // Load up all the Items
    ParserLoadFilesShared("SM_Items" SUFFIX, NULL, "defs/stores/items.def", "items.bin",
        0, ParseStoreItems, &g_Items, sizeof(StoreItems),
        pdef, NULL, NULL, PostProcItems, StoreItemsFinalProcess);
        0,

    ShareStructure("SM_DeptContents" SUFFIX, ParseDepartmentsContents,
        &g_DepartmentContents, sizeof(g_DepartmentContents),
        PopulateDepartmentContents, NULL);

    // Load up all the Stores and share them (Stores are funky which is why
    // this isn't a ParserLoadFilesShared).
    ShareStructure("SM_Stores" SUFFIX, ParseStores, &g_Stores, sizeof(g_Stores), LoadStores, pdef);

    // Index the items into the hash tables (g_StoreHashes.*)
    // The server makes all of the indicies up front. The client makes them
    // as they are needed.
    //
    // TODO: It might be a better idea to send the value for each of their
    //       items down from the server instead of keeping all the tables
    //       on the client. This is expedient, however.
#if SERVER
    ShareStructure("SM_StoreHashes" SUFFIX, ParseStoreHashes, &g_StoreHashes, sizeof(g_StoreHashes), StoreHashesWorkServer, NULL);
#else
    StoreHashesWorkClient(NULL);
#endif

    // Second half of the indexing (g_hashStores)
    IndexStoresStatic();

    DefineDestroy(pdef);
}

/* End of File */
