/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include <malloc.h>
#include <string.h>

#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/timing.h> // for timerSecondsSince2000
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/genericlist.h>
#include <utilitieslib/components/MemoryPool.h>

#include "entity/costume.h"
#include "entity/power_customization.h"
#include <utilitieslib/utils/eval.h>
#include "entity/entity.h"
#include "entity/powers.h"
#include "entity/PowerInfo.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/character_eval.h"
#include "entity/character_inventory.h"
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/utils/textparser.h>
#include "gameData/mininghelper.h"
#include "entity/boostset.h"
#include <utilitieslib/utils/log.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/components/EString.h>

#ifdef CLIENT
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/wininclude.h>        // @todo temporary for NEEDS_REVIEW()/TODO()
#include "UI/uiTray.h"
#include "player/player.h"
#endif

#ifdef SERVER
#include "entity/character_net_server.h"
#include "dbcomm/logcomm.h"     // for dbLog() messages
#include "dbghelper.h"
#include "ai/entai.h"     // For aiAdd/RemovePower
#include "cmdparse/cmdservercsr.h"
#include "entity/character_combat_eval.h"
#endif


///////////////////////////////////////////////////////////////////////////////////

U32 g_ulPowersDebug = 0;

// The central bucket for all the powers used in the game.
SHARED_MEMORY PowerDictionary g_PowerDictionary = {0};
BasePowerSet **g_SharedPowersets = 0;
int g_numSharedPowersets = -1; // set on powers dictionary load
int g_numAutomaticSpecificPowersets = 2; // explicit hack, matches character_GrantAutoIssueBuildSpecificPowerSets
Power **g_deletedPowers = 0;

ParseLink g_basepower_InfoLink = {
    (void***)&g_PowerDictionary.ppPowerCategories ,
    {
        { offsetof(PowerCategory,pchName), offsetof(PowerCategory,ppPowerSets) },
        { offsetof(BasePowerSet,pchName), offsetof(BasePowerSet,ppPowers) },
        { offsetof(BasePower,pchName),      0 },
    }
};

ParseLink g_power_PowerInfoLink = {
    (void***)&g_PowerDictionary.ppPowers,
    {
        { offsetof(BasePower,pchFullName), 0 },
    }
};

ParseLink g_power_SetInfoLink = {
    (void***)&g_PowerDictionary.ppPowerSets,
    {
        { offsetof(BasePowerSet,pchFullName), 0 },
    }
};

ParseLink g_power_CatInfoLink = {
    (void***)&g_PowerDictionary.ppPowerCategories,
    {
        { offsetof(PowerCategory,pchName), 0 },
    }
};

bool power_sloteval_Eval(Power *pPower, Boost *pBoost, Entity *e, const char * const *ppchExpr);

const BasePower *basepower_FromPath(char *path)
{
    return basepower_GetReplacement((const BasePower*)ParserLinkFromString((StructLink*)&g_basepower_InfoLink, path));
}

char *basepower_ToPath(const BasePower *ppow)
{
    static char path[512];
    return (char*)allocAddString(ParserLinkToString(&g_basepower_InfoLink, ppow, SAFESTR(path)));
}


/**********************************************************************func*
 * powerdict_ResetStats
 *
 */
void powerdict_ResetStats(PowerDictionary *pdict)
{
    int icat;

    assert(pdict!=NULL);

    for(icat=eaSize(&pdict->ppPowerCategories)-1; icat>=0; icat--)
    {
        int iset;
        const PowerCategory *pcat = pdict->ppPowerCategories[icat];

        for(iset=eaSize(&pcat->ppPowerSets)-1; iset>=0; iset--)
        {
            int ipow;
            const BasePowerSet *pset = pcat->ppPowerSets[iset];

            for(ipow=eaSize(&pset->ppPowers)-1; ipow>=0; ipow--)
            {
                memset((void*)&pset->ppPowers[ipow]->stats, 0, sizeof(PowerStats));
            }
        }
    }
}

/**********************************************************************func*
 * powerdict_DumpStats
 *
 */
void powerdict_DumpStats(const PowerDictionary *pdict)
{
    int icat;

    for(icat=eaSize(&pdict->ppPowerCategories)-1; icat>=0; icat--)
    {
        int iset;
        const PowerCategory *pcat = pdict->ppPowerCategories[icat];

        for(iset=eaSize(&pcat->ppPowerSets)-1; iset>=0; iset--)
        {
            int ipow;
            const BasePowerSet *pset = pcat->ppPowerSets[iset];

            for(ipow=eaSize(&pset->ppPowers)-1; ipow>=0; ipow--)
            {
                if(pset->ppPowers[ipow]->stats.iCntUsed > 0)
                {
                    LOG( LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "%s\t\t%s\t%s\t%s\t%1.2f\t\t%1.2f\t\t%d\t%d\t%d\t%1.2f\t%1.2f\n",
                        "*Overall*",
                        pcat->pchName,
                        pset->pchName,
                        pset->ppPowers[ipow]->pchName,
                        pset->ppPowers[ipow]->stats.fDamageGiven,
                        pset->ppPowers[ipow]->stats.fHealingGiven,
                        pset->ppPowers[ipow]->stats.iCntUsed,
                        pset->ppPowers[ipow]->stats.iCntHits,
                        pset->ppPowers[ipow]->stats.iCntMisses,
                        (pset->ppPowers[ipow]->stats.iCntHits+pset->ppPowers[ipow]->stats.iCntMisses) ?
                            100.0f*(float)pset->ppPowers[ipow]->stats.iCntHits/(float)(pset->ppPowers[ipow]->stats.iCntHits+pset->ppPowers[ipow]->stats.iCntMisses) : 0,
                        (pset->ppPowers[ipow]->stats.iCntHits+pset->ppPowers[ipow]->stats.iCntMisses) ?
                            100.0f*(float)pset->ppPowers[ipow]->stats.iCntMisses/(float)(pset->ppPowers[ipow]->stats.iCntHits+pset->ppPowers[ipow]->stats.iCntMisses) : 0);
                }
            }
        }
    }
}

const PowerCategory *powerdict_GetCategoryByNameEx(const PowerDictionary *pdict, const char *pch, bool bShowErrors)
{
    const PowerCategory *pcat = NULL;

    assert(pdict);
    assert(pdict->haCategoryNames);

    pcat = (const PowerCategory*)stashFindPointerReturnPointerConst(pdict->haCategoryNames, pch);
    if (!pcat && bShowErrors)
    {
        Errorf("Category name not found: <%s>\n", pch==NULL ? "(null)" : pch);
    }

    return pcat;
}

const PowerCategory *powerdict_GetCategoryByName(const PowerDictionary *pdict, const char *pch)
{
    return powerdict_GetCategoryByNameEx(pdict, pch, true);
}

const BasePowerSet *powerdict_GetBasePowerSetByFullNameEx(const PowerDictionary *pdict, const char *pch, bool bShowErrors)
{
    const BasePowerSet *pset = NULL;

    assert(pdict!=NULL);
    assert(pdict->haSetNames);

    pset = (const BasePowerSet*)stashFindPointerReturnPointerConst(pdict->haSetNames, pch);

    if (!pset && bShowErrors)
        Errorf("Powerset full name not found: <%s>\n", pch==NULL ? "(null)" : pch);

    return pset;
}

const BasePowerSet *powerdict_GetBasePowerSetByFullName(const PowerDictionary *pdict, const char *pch)
{
    return powerdict_GetBasePowerSetByFullNameEx(pdict, pch, true);
}

const BasePower *powerdict_GetBasePowerByCRCEx(const PowerDictionary *pdict, int CRC, bool bShowErrors)
{
    const BasePower *ppow = NULL;

    assert(pdict!=NULL);
    assert(pdict->haPowerCRCs);

    if (!stashIntFindPointerConst(pdict->haPowerCRCs, CRC, &ppow) && bShowErrors)
    {
        Errorf("Power full name not found: <%d>\n", CRC);
    }
    
    return ppow;
}

const BasePower *powerdict_GetBasePowerByCRC(const PowerDictionary *pdict, int CRC)
{
    return powerdict_GetBasePowerByCRCEx(pdict, CRC, true);
}

extern U32 Crc32cLowerStringHash(U32 hash, const char * s);

const BasePower *powerdict_GetBasePowerByFullNameEx(const PowerDictionary *pdict, const char *pch, bool bShowErrors)
{
    int CRC = Crc32cLowerStringHash(0, pch);
    const BasePower *ppow = powerdict_GetBasePowerByCRCEx(pdict, CRC, false);

    if (!ppow && bShowErrors)
    {
        Errorf("Power full name not found: <%s>\n", pch);
    }

    return ppow;
}

const BasePower *powerdict_GetBasePowerByFullName(const PowerDictionary *pdict, const char *pch)
{
    return powerdict_GetBasePowerByFullNameEx(pdict, pch, true);
}

int powerdict_GetCRCOfBasePower(const BasePower *ppowBase)
{
    return Crc32cLowerStringHash(0, ppowBase->pchFullName);
}

const BasePowerSet *powerdict_GetBasePowerSetByNameEx(const PowerDictionary *pdict, const char *pchCat, const char *pchSet, bool bShowErrors)
{
    const BasePowerSet *pset = NULL;
    if(pchCat && pchSet)
    {
        char* buf = NULL;        
        estrStackCreate(&buf, 256);
        estrPrintf(&buf, "%s.%s", pchCat, pchSet);
        pset = powerdict_GetBasePowerSetByFullNameEx(pdict, buf, false);
        estrDestroy(&buf);
    }

    if (!pset && bShowErrors)
        Errorf("BasePowerSet name not found: <%s.%s>\n", pchCat==NULL ? "(null)" : pchCat, pchSet==NULL ? "(null)" : pchSet);

    return pset;
}

const BasePowerSet *powerdict_GetBasePowerSetByName(const PowerDictionary *pdict, const char *pchCat, const char *pchSet)
{
    return powerdict_GetBasePowerSetByNameEx(pdict, pchCat, pchSet, true);
}

const BasePower *powerdict_GetBasePowerByNameEx(const PowerDictionary *pdict, const char *pchCat, const char *pchSet, const char *pchPow, bool bShowErrors)
{
    const BasePower *ppow = NULL;

    if(pchCat && pchSet && pchPow)
    {
        char* buf = NULL;        
        estrStackCreate(&buf, 512);
        estrPrintf(&buf, "%s.%s.%s", pchCat, pchSet, pchPow);
        ppow = powerdict_GetBasePowerByFullNameEx(pdict, buf, bShowErrors);
        estrDestroy(&buf);
    }

    return ppow;
}

const BasePower *powerdict_GetBasePowerByName(const PowerDictionary *pdict, const char *pchCat, const char *pchSet, const char *pchPow)
{
    return powerdict_GetBasePowerByNameEx(pdict, pchCat, pchSet, pchPow, true);
}

static const BasePowerSet *findSetInCategoryEx(const PowerCategory *pcat, const char *pch, bool bShowErrors)
{
    return powerdict_GetBasePowerSetByNameEx(&g_PowerDictionary, pcat->pchName, pch, bShowErrors);
}

static const BasePowerSet *findSetInCategory(const PowerCategory *pcat, const char *pch)
{
    return findSetInCategoryEx(pcat, pch, true);
}

/**********************************************************************func*
 * powerdict_GetBasePowerByNameError
 *
 */
char *powerdict_GetBasePowerByNameError(const PowerDictionary *pdict, const char *pchCat, const char *pchSet, const char *pchPow)
{
    const PowerCategory *pcat;
    const BasePowerSet *psetBase = NULL;
    const BasePower *ppow = NULL;
    static char ach[1024];

    assert(pdict!=NULL);
    pcat = powerdict_GetCategoryByName(pdict, pchCat);
    if(pcat) psetBase = findSetInCategory(pcat, pchSet);
    ppow = powerdict_GetBasePowerByName(pdict, pchCat, pchSet, pchPow);

    if(ppow==NULL)
    {
        sprintf(ach, "BasePower name not found: <%s(%s).%s(%s).%s(%s)>",
                pchCat==NULL ? "(null)" : pchCat,
                pcat ? "found" : "not found",
                pchSet==NULL ? "(null)" : pchSet,
                psetBase ? "found" : "not found",
                pchPow==NULL ? "(null)" : pchPow,
                ppow ? "found" : "not found");
    }
    else
    {
        strcpy(ach, "No error.");
    }

    return ach;
}

/**********************************************************************func*
 * powerdict_GetBasePowerByString
 *
 */
const BasePower *powerdict_GetBasePowerByString(const PowerDictionary *pdict, const char *pchDesc, int *piLevel)
{
    const PowerCategory *pcat;
    const BasePowerSet *psetBase;
    const BasePower *ppow = NULL;
    char *pch = (char*)_alloca(strlen(pchDesc)+1);
    strcpy(pch, pchDesc);

    assert(pdict!=NULL);

    pch = strtok(pch, ".");
    pcat = powerdict_GetCategoryByName(pdict, pch);
    if(pcat!=NULL)
    {
        pch = strtok(NULL, ".");
        psetBase = category_GetBaseSetByName(pcat, pch);
        if(psetBase!=NULL)
        {
            pch = strtok(NULL, ".");
            ppow = baseset_GetBasePowerByName(psetBase, pch);

            // Get the level if it is there.
            pch = strtok(NULL, ".");
            if(pch && piLevel)
            {
                *piLevel = atoi(pch);
            }
        }
    }

    if(ppow==NULL)
        Errorf("BasePower name not found: <%s>\n", pchDesc==NULL ? "(null)" : pchDesc);

    return ppow;
}

bool powerdict_GetBasePtrsByStringEx(const PowerDictionary *pdict, const char *pchDesc,
    const PowerCategory **ppCat, const BasePowerSet **ppsetBase, const BasePower **pppow,
    int *piLevel, bool bShowErrors)
{
    bool bRet = true;
    char *pch = (char*)_alloca(strlen(pchDesc)+1);
    strcpy(pch, pchDesc);

    assert(pdict!=NULL);

    pch = strtok(pch, ".");
    *ppCat = powerdict_GetCategoryByNameEx(pdict, pch, false);
    if(*ppCat!=NULL)
    {
        pch = strtok(NULL, ".");
        if(pch)
        {
            *ppsetBase = findSetInCategoryEx(*ppCat, pch, false);
            if(*ppsetBase!=NULL)
            {
                pch = strtok(NULL, ".");
                if(pch)
                {
                    *pppow = powerdict_GetBasePowerByNameEx(pdict, (*ppCat)->pchName, (*ppsetBase)->pchName, pch, false);
                    if(*pppow!=NULL)
                    {
                        // Get the level if it is there.
                        pch = strtok(NULL, ".");
                        if(pch && piLevel)
                        {
                            *piLevel = atoi(pch);
                        }
                    }
                    else
                    {
                        if (bShowErrors)
                            Errorf("Power name not found: <%s>\n", pchDesc==NULL ? "(null)" : pchDesc);
                        bRet = false;
                    }
                }
            }
            else
            {
                if (bShowErrors)
                    Errorf("Set name not found: <%s>\n", pchDesc==NULL ? "(null)" : pchDesc);
                bRet = false;
            }
        }
    }
    else
    {
        if (bShowErrors)
            Errorf("Category name not found: <%s>\n", pchDesc==NULL ? "(null)" : pchDesc);
        bRet = false;
    }

    return bRet;
}

bool powerdict_GetBasePtrsByString(const PowerDictionary *pdict, const char *pchDesc,
    const PowerCategory **ppCat, const BasePowerSet **ppsetBase, const BasePower **pppow,
    int *piLevel)
{
    return powerdict_GetBasePtrsByStringEx(pdict, pchDesc, ppCat, ppsetBase, pppow, piLevel, true);
}

/**********************************************************************func*
 * powerdict_GetBasePowerByIndex
 *
 */
const BasePower *powerdict_GetBasePowerByIndex(const PowerDictionary * pdict, int icat, int iset, int ipow, const char **catNamePtr, const char **setNamePtr)
{
    const PowerCategory *pcat = NULL;
    const BasePowerSet *pset = NULL;
    const BasePower *ppow = NULL;

    assert(pdict!=NULL);

    if(icat>=0 && icat<eaSize(&pdict->ppPowerCategories))
    {
        pcat = pdict->ppPowerCategories[icat];
        (*catNamePtr) = pcat->pchName;

        if(iset>=0 && iset<eaSize(&pcat->ppPowerSets))
        {
            pset = pcat->ppPowerSets[iset];
            (*setNamePtr) = pset->pchName;

            if(ipow>=0 && ipow<eaSize(&pset->ppPowers))
            {
                ppow = pset->ppPowers[ipow];
            }
        }
    }

    if(ppow==NULL) 
        Errorf("BasePower not found: <%d.%d.%d>\nNOTE: this is likely due to a data mismatch between the client and the server.\n", icat, iset, ipow);
    return ppow;
}

/**********************************************************************func*
 * powerdict_GetBasePowerByID
 *
 */
const BasePower *powerdict_GetBasePowerByID(const PowerDictionary *pdict, BasePowerID *pid, const char **catNamePtr, const char **setNamePtr)
{
    BasePower *ppow = NULL;

    assert(pdict!=NULL);
    assert(pid!=NULL);

    return powerdict_GetBasePowerByIndex(pdict, pid->icat, pid->iset, pid->ipow, catNamePtr, setNamePtr);
}

/**********************************************************************func*
 * category_CheckForSet
 *
 */
bool category_CheckForSet(const PowerCategory *pcat, const char *pch)
{
    return findSetInCategory(pcat, pch) != NULL;
}

/**************************************************************************/
/**************************************************************************/

/**********************************************************************func*
 * category_GetBaseSetByName
 *
 */
const BasePowerSet *category_GetBaseSetByName(const PowerCategory *pcat, const char *pch)
{
    const BasePowerSet *pset = findSetInCategory(pcat, pch);

    if(pset==NULL)
    {
        Errorf("BaseSet name not found: <%s>\n", pch==NULL ? "(null)" : pch);
    }
    return pset;
}

/**********************************************************************func*
 * baseset_GetBasePowerByName
 *
 */
const BasePower *baseset_GetBasePowerByName(const BasePowerSet *pset, const char *pch)
{
    assert(pset && pset->pcatParent);
    return powerdict_GetBasePowerByName(&g_PowerDictionary, pset->pcatParent->pchName, pset->pchName, pch);
}

/**********************************************************************func*
 * baseset_BasePowerAvailableByIdx
 *
 * Returns the number of levels needed until the power is available.
 * <= 0 means that it's available now.
 *
 */
int baseset_BasePowerAvailableByIdx(const BasePowerSet *pset, int iIdx, int iLevel)
{
    assert(pset!=NULL);
    assert(iIdx>=0);
    //assert(iIdx<eaiSize(&pset->piAvailable));

    return pset->piAvailable[iIdx]-iLevel;
}

bool baseset_IsCrafted(const BasePowerSet *pset) {
    return pset && pset->pchName && (strnicmp(pset->pchName, "crafted_", 8) == 0);
}

/**************************************************************************/
/**************************************************************************/


/**********************************************************************func*
 * power_GetNames
 *
 */
int basepower_GetNames(const BasePower *ppow, const char **ppchCat, const char **ppchSet, const char **ppchPow)
{
    assert(ppow!=NULL);

    *ppchPow = ppow->pchName;
    *ppchSet = ppow->psetParent->pchName;
    *ppchCat = ppow->psetParent->pcatParent->pchName;

    return 0;
}

/**********************************************************************func*
 * basepower_SetID
 *
 */
void basepower_SetID(BasePower *ppow, int icat, int iset, int ipow)
{
    assert(ppow!=NULL);

    ppow->id.icat = icat;
    ppow->id.iset = iset;
    ppow->id.ipow = ipow;
}

/**********************************************************************func*
 * basepower_GetVarIdx
 *
 */
int basepower_GetVarIdx(const BasePower *ppowBase, const char *pchName)
{
    int n = eaSize(&ppowBase->ppVars);
    int i;

    for(i=0; i<n; i++)
    {
        if(stricmp(pchName, ppowBase->ppVars[i]->pchName)==0)
        {
            return ppowBase->ppVars[i]->iIndex;
        }
    }

    return -1;
}

const AttribModTemplate *basepower_GetAttribByName(const BasePower *ppowBase, const char *pchName)
{
    int n = eaSize(&ppowBase->ppTemplates);
    int i;

    for(i=0; i<n; i++)
    {
        if(ppowBase->ppTemplates[i]->pchName && stricmp(pchName, ppowBase->ppTemplates[i]->pchName)==0)
        {
            return ppowBase->ppTemplates[i];
        }
    }

    return NULL;
}

#ifdef CLIENT
static StashTable s_basePowerAtlasTexture;

/**********************************************************************func*
 * basepower_GetIcon
 *
 */
AtlasTex *basepower_GetIcon(const BasePower *ppow)
{
    AtlasTex *ret = NULL;
    if (!ppow)
        return white_tex_atlas;

    if (!s_basePowerAtlasTexture)
        s_basePowerAtlasTexture = stashTableCreateAddress(1024);

    if (!stashAddressFindPointer(s_basePowerAtlasTexture, ppow, cpponly_reinterpret_cast(void**)(&ret)) || (ret == white_tex_atlas)) {
        ret = atlasLoadTexture(ppow->pchIconName);
        stashAddressAddPointer(s_basePowerAtlasTexture, ppow, ret, true);
    }

    return ret;
}
#endif

const char* basepower_GetAttribModFX(const char* baseFXName, const char* const * attribModFX)
{
    const char* indexStr;

    if (baseFXName == NULL)
        return NULL;

    if (strnicmp(baseFXName, "pfx", 3) != 0)
        return baseFXName;

    indexStr = baseFXName + 3;

    if (*indexStr == '\0')
        return attribModFX[0];

    return attribModFX[atoi(indexStr) - 1];
}

bool basepower_ShowInInventory(Character *pchar, const BasePower *ppowBase)
{
    Power *ppow;
    if (!pchar || !ppowBase)
    {
        return false;
    }
    else if (ppowBase->psetParent->eShowInInventory == kShowPowerSetting_Always)
    {
        return true;
    }
    else if (ppowBase->psetParent->eShowInInventory == kShowPowerSetting_Never)
    {
        return false;
    }
    else if (ppowBase->psetParent->eShowInInventory == kShowPowerSetting_IfUsable)
    {
        return ((ppow = character_OwnsPower(pchar, ppowBase)) != NULL) && ppow->bEnabled;
    }
    else if (ppowBase->psetParent->eShowInInventory == kShowPowerSetting_IfOwned)
    {
        return (character_OwnsPower(pchar, ppowBase) != NULL);
    }
    else // ppowBase->pSetParent->eShowInInventory == kShowPowerSetting_Default
    {
        if (ppowBase->eShowInInventory == kShowPowerSetting_Always ||
            ppowBase->eShowInInventory == kShowPowerSetting_Default)
        {
            return true;
        }
        else if (ppowBase->eShowInInventory == kShowPowerSetting_IfUsable &&
            (ppow = character_OwnsPower(pchar, ppowBase)) && ppow->bEnabled)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}


/**************************************************************************/
/**************************************************************************/

/**********************************************************************func*
 * AddPowerNotify
 *
 */
static void AddPowerNotify(Character *pchar, Power *ppow)
{
#if SERVER
    if(pchar && pchar->entParent)
        aiAddPower(pchar->entParent, ppow);
#endif

#if CLIENT
#ifndef TEST_CLIENT
    if (pchar->entParent == playerPtr() && ppow->ppowBase->iServerTrayPriority)
    {
        serverTray_AddPower(pchar->entParent, ppow, ppow->ppowBase->iServerTrayPriority);
    }
#endif
#endif
}

/**********************************************************************func*
 * RemovePowerNotify
 *
 */
static void RemovePowerNotify(Character *pchar, Power *ppow)
{
#if SERVER
    if(pchar && pchar->entParent)
        aiRemovePower(pchar->entParent, ppow);
#endif

#if CLIENT
#ifndef TEST_CLIENT
    if (pchar->entParent == playerPtr() && ppow->ppowBase->iServerTrayPriority)
    {
        serverTray_RemovePower(pchar->entParent, ppow);
    }
#endif
#endif
}

/**********************************************************************func*
 * powerset_Create
 *
 */
MP_DEFINE(PowerSet);
PowerSet *powerset_Create(const BasePowerSet *psetBase)
{
    PowerSet *pset;

    MP_CREATE(PowerSet, 16);
    pset = MP_ALLOC(PowerSet);

    assert(psetBase!=NULL);

    pset->psetBase = psetBase;

    eaCreate(&pset->ppPowers);

    return pset;
}

/**********************************************************************func*
 * powerset_Destroy
 *
 */
void powerset_Destroy(PowerSet *pset, const char *context)
{
    int i;
    assert(pset!=NULL);

    for(i=eaSize(&pset->ppPowers)-1; i>=0; i--)
    {
        power_Destroy(pset->ppPowers[i],context);
    }
    eaDestroy(&pset->ppPowers);

    MP_FREE(PowerSet, pset);
}

/**********************************************************************func*
 * powerset_Empty
 *
 */
void powerset_Empty(PowerSet *pset, const char *context)
{
    int i;
    assert(pset!=NULL);

    for(i=eaSize(&pset->ppPowers)-1; i>=0; i--)
    {
        power_Destroy(pset->ppPowers[i],context);
    }
    eaSetSize(&pset->ppPowers, 0);
}

// oldCharacter is used in respecs.  During respecs, we need to keep unique IDs the same
// whenever possible.  This means that if we buy a new power, we need to make sure it doesn't
// take a unique ID taken by an old power that we haven't chosen yet.  Buying a new power
// implies we passed in zero as uid, so we generate a new, random uid.  If we passed in a
// non-zero UID, then we are picking one of the old powers, so we shouldn't prevent collisions
// against the old set of powers (in fact, we expect it).
bool power_SetUniqueID(Character *pchar, Power *ppow, int uid, Character *oldCharacter)
{
    bool shouldAssertOnCollide = (uid != 0);

    assert(pchar && ppow);

    if (!pchar->powersByUniqueID)
        pchar->powersByUniqueID = stashTableCreate(100, StashDefault, StashKeyTypeInts, sizeof(int));

    while (!uid || stashIntFindElement(pchar->powersByUniqueID, uid, NULL))
    {
        devassertmsg(!shouldAssertOnCollide, "There has been a collision in unique IDs for powers.  Please tell Andy about this message.  You can hit Ignore if you're in a big hurry, but it's preferable to debug the problem."); // man assertion logic is fun
        shouldAssertOnCollide = false; // only assert once per function call
        do 
        {
            uid = randInt(INT_MAX);
        } while (oldCharacter && stashIntFindElement(oldCharacter->powersByUniqueID, uid, NULL));
    };

    ppow->iUniqueID = uid;

    return stashIntAddPointer(pchar->powersByUniqueID, uid, ppow, false);
}

// Pass zero for the uniqueID to assign a new one!
// oldCharacter is only used in respecs.
Power *powerset_AddNewPower(PowerSet *pset, const BasePower *ppowBase, int uniqueID, Character *oldCharacter)
{
    Power *ppow = power_Create(ppowBase);

    assert(pset!=NULL);
    assert(ppowBase!=NULL);

    SetCharacterAttributesToOne(&ppow->attrStrength);

    // By default, use the player's strength directly. If there are boosts,
    // this will be changed by AccrueBoosts.
    ppow->pattrStrength = &pset->pcharParent->attrStrength;

    ppow->psetParent = pset;
    power_SetUniqueID(ppow->psetParent->pcharParent, ppow, uniqueID, oldCharacter);
    ppow->idx = eaFind(&pset->ppPowers, NULL);

    if(ppow->idx<0)
    {
        ppow->idx = eaPush(&pset->ppPowers, ppow);
    }
    else
    {
        eaSet(&pset->ppPowers, ppow, ppow->idx);
    }
    AddPowerNotify(pset->pcharParent, ppow);

    return ppow;
}

// Pass zero for the uniqueID to assign a new one!
// oldCharacter is only used in respecs.
Power *powerset_SetNewPowerAt(PowerSet *pset, const BasePower *ppowBase, int idx, const char *context, int uniqueID, Character *oldCharacter)
{
    Power *ppowOld;
    Power *ppow = power_Create(ppowBase);

    assert(pset!=NULL);
    assert(ppowBase!=NULL);

    SetCharacterAttributesToOne(&ppow->attrStrength);

    // By default, use the player's strength directly. If there are boosts,
    // this will be changed by AccrueBoosts.
    ppow->pattrStrength = &pset->pcharParent->attrStrength;

    ppow->psetParent = pset;
    power_SetUniqueID(ppow->psetParent->pcharParent, ppow, uniqueID, oldCharacter);
    ppow->idx = idx;

    if(idx>=eaSize(&pset->ppPowers))
    {
        eaSetSize(&pset->ppPowers, idx+1);
    }

    ppowOld = (Power*)eaGet(&pset->ppPowers, idx);
    if(ppowOld!=NULL)
    {
        power_Destroy(ppowOld,context);
    }

    eaSet(&pset->ppPowers, ppow, ppow->idx);
    AddPowerNotify(pset->pcharParent, ppow);

    return ppow;
}

/**********************************************************************func*
 * powerset_DeletePower
 *
 * This does not compact the powers array, but leaves the power pointer NULL.
 *
 */
void powerset_DeletePower(PowerSet *pset, Power *ppow, const char *context)
{
    int i;

    assert(pset != NULL);
    assert(ppow != NULL);

    for (i = eaSize(&pset->ppPowers) - 1; i >= 0; i--)
    {
        if (pset->ppPowers[i] && ppow == pset->ppPowers[i])
        {
            power_Destroy(pset->ppPowers[i], context);
            pset->ppPowers[i] = NULL;
            break;
        }
    }
}

/**********************************************************************func*
 * powerset_DeleteBasePower
 *
 * This does not compact the powers array, but leaves the power pointer NULL.
 *
 */
void powerset_DeleteBasePower(PowerSet *pset, const BasePower *ppowBase, const char *context)
{
    int i;

    assert(pset != NULL);
    assert(ppowBase != NULL);

    for (i = eaSize(&pset->ppPowers) - 1; i >= 0; i--)
    {
        if (pset->ppPowers[i] && ppowBase == pset->ppPowers[i]->ppowOriginal)
        {
            power_Destroy(pset->ppPowers[i], context);
            pset->ppPowers[i] = NULL;
            break;
        }
    }
}

/**********************************************************************func*
 * powerset_DeletePowerAt
 *
 * This does not compact the powers array, but leaves the power pointer NULL.
 *
 */
void powerset_DeletePowerAt(PowerSet *pset, int idx, const char *context)
{
    assert(pset!=NULL);

    if(idx<eaSize(&pset->ppPowers))
    {
        power_Destroy(pset->ppPowers[idx],context);
        pset->ppPowers[idx] = NULL;
    }
}

/**********************************************************************func*
 * powerset_PowerIdx
 *
 * returns -1 on failure
 *
 */
int powerset_PowerIdx(PowerSet *pset, Power *ppow)
{
    assert(pset!=NULL);
    assert(ppow!=NULL);

    return eaFind(&pset->ppPowers, ppow);
}

/**********************************************************************func*
 * powerset_BasePowerAvailableByIdx
 *
 * Returns the number of levels needed until the power is available.
 * <= 0 means that it's available now.
 *
 */
int powerset_BasePowerAvailableByIdx(PowerSet *pset, int iIdx, int iLevel)
{
    assert(pset!=NULL);
    assert(iIdx>=0);
//    assert(iIdx<eaiSize(&pset->psetBase->piAvailable));

    return pset->psetBase->piAvailable[iIdx]-iLevel;

    // If availablility is based upon when the character bought the power,
    // you'd use this instead.
    //
    //return pset->psetBase->piAvailable[iIdx]+pset->iLevelBought-iLevel;
}


bool powerset_ShowInInventory(PowerSet *pset)
{
    return pset->psetBase->eShowInInventory != kShowPowerSetting_Never;
}

/**************************************************************************/
/**************************************************************************/

/**********************************************************************func*
 * power_Create
 *
 */
MP_DEFINE(Power);
Power *power_Create(const BasePower *ppowBase)
{
    Power *ppow;

    MP_CREATE(Power, 16);
    ppow = MP_ALLOC(Power);

    ppow->ppowBase = ppow->ppowOriginal = ppowBase;
    ppow->iNumCharges = ppowBase->iNumCharges;
    ppow->fUsageTime = ppowBase->fUsageTime;
    ppow->bDontSetStance = ppowBase->bDontSetStance;
    ppow->bIgnoreConningRestrictions = 0;
    ppow->ulCreationTime = timerSecondsSince2000();
    ppow->chainsIntoPower = NULL;
    eaCreate(&ppow->ppBoosts);

    return ppow;
}

/**********************************************************************func*
 * power_Destroy
 *
 */
void power_Destroy(Power *ppow, const char *context)
{
    if (!ppow)
        return;

    if (ppow->psetParent && !ppow->bHasBeenDeleted)
        RemovePowerNotify(ppow->psetParent->pcharParent, ppow);

    if ((!ppow->ppowBase || ppow->ppowBase->eType != kPowerType_Inspiration) && !ppow->bHasBeenDeleted)
    {
        power_DestroyAllBoosts(ppow,context);
        eaDestroy(&ppow->ppBoosts);
        eaDestroyConst(&ppow->ppBonuses);
        power_DestroyAllChanceMods(ppow);
    }

    if (ppow->psetParent && ppow->psetParent->pcharParent)
    {
        devassertmsg(stashIntRemoveInt(ppow->psetParent->pcharParent->powersByUniqueID, ppow->iUniqueID, NULL), "We just tried to remove a power from the unique ID table, but couldn't find it.  Please tell Andy about this message.  You can hit Ignore if you're in a big hurry, but it's preferable to debug the problem.");
    }

#if SERVER
    if (ppow->attribModRefCount)
    {
        ppow->bHasBeenDeleted = true;
        ppow->psetParent = NULL;
        eaPush(&g_deletedPowers, ppow);
        if (ppow->ppowBase)
        {
            //LOG_OLD( "DeletedPower: Add Added %s, new g_deletedPowers array size: %d", ppow->ppowBase->pchName, eaSize(&g_deletedPowers));
        }
    }
    else
#endif
    {
        MP_FREE(Power, ppow);
    }
}

/**********************************************************************func*
 * power_LevelFinalize
 *
 */
void power_LevelFinalize(Power *ppow)
{
    // Number of automatically given boost slots is based on when they
    // bought the power.
    int iNumBoosts = ppow->iNumBoostsBought +
        CountForLevel( character_GetLevel(ppow->psetParent->pcharParent)-ppow->iLevelBought, g_Schedules.aSchedules.piFreeBoostSlotsOnPower);
    iNumBoosts = MIN(iNumBoosts, ppow->ppowBase->iMaxBoosts);

    if(eaiSize(&ppow->ppowBase->pBoostsAllowed)>0)
    {
        while(eaSize(&ppow->ppBoosts) < iNumBoosts)
        {
            power_AddBoostSlot(ppow);
        }
    }
}

/**********************************************************************func*
 * power_AddBoostSlot
 *
 */
void power_AddBoostSlot(Power *ppow)
{
    assert(ppow!=NULL);

    eaPush(&ppow->ppBoosts, NULL);
}

/**********************************************************************func*
 * power_RemoveBoostSlot
 *
 * Warning: This function requires that the enhancement slot to be removed is empty
 *
 */
void power_RemoveBoostSlot(Power *ppow)
{
    assert(ppow!=NULL);
    assert(ppow->ppBoosts[eaSize(&ppow->ppBoosts)-1] == NULL);

    eaPop(&ppow->ppBoosts);
}

/**********************************************************************func*
 * power_BoostCheckRequires
 *
 */
bool power_BoostCheckRequires(Power *ppow, Boost *pBoost)
{
    if (pBoost->ppowBase->ppchSlotRequires != NULL)
        return power_sloteval_Eval(ppow, pBoost, ppow->psetParent->pcharParent->entParent, pBoost->ppowBase->ppchSlotRequires);
    else
        return true;
}


/**********************************************************************func*
 * power_ReplaceBoost
 *
 */
void power_ReplaceBoost(Power *ppow, int iIdx, const BasePower *pbase, int iLevel, int iNumCombines, float const *afVars, int numVars,
        PowerupSlot const *aPowerupSlots, int numPowerupSlots, const char *context)
{
    Boost *pBoost;

    assert(ppow!=NULL);
    assert(iIdx>=0);

    if(verify(eaSize(&ppow->ppBoosts)>iIdx))
    {
        pBoost = boost_Create(pbase, iIdx, ppow, iLevel, iNumCombines, afVars, numVars, aPowerupSlots, numPowerupSlots);

        if (!power_BoostCheckRequires(ppow, pBoost))
        {
            boost_Destroy(pBoost);
            return;
        }

        if(ppow->ppBoosts[iIdx]!=NULL)
        {
            boost_Destroy(ppow->ppBoosts[iIdx]);
        }
        ppow->ppBoosts[iIdx] = pBoost;
        ppow->bBoostsCalcuated = false;
        DATA_MINE_ENHANCEMENT(pbase,iLevel,"out",context,1);
    }
}


/***************************************************************************
 * power_ReplaceBoostFromBoost
 *
 */
void power_ReplaceBoostFromBoost(Power *ppow, int iIdx, Boost const *srcBoost, const char *context)
{
    if( verify( srcBoost ))
    {
        power_ReplaceBoost( ppow, iIdx,
                            srcBoost->ppowBase, srcBoost->iLevel, srcBoost->iNumCombines,
                            srcBoost->afVars, ARRAY_SIZE(srcBoost->afVars),
                            srcBoost->aPowerupSlots, ARRAY_SIZE( srcBoost->aPowerupSlots ), context);
    }
}


/**********************************************************************func*
 * power_RemoveBoost
 *
 */
Boost *power_RemoveBoost(Power *ppow, int iIdx, const char *context)
{
    Boost *pboostOld = NULL;

    assert(ppow!=NULL);
    assert(iIdx>=0);

    if(eaSize(&ppow->ppBoosts)>iIdx)
    {
        pboostOld = ppow->ppBoosts[iIdx];
        ppow->ppBoosts[iIdx] = NULL;
        ppow->bBoostsCalcuated = false;
// This should already be recorded as leaving the system via 'used'
//        if(pboostOld)
//            DATA_MINE_ENHANCEMENT(pboostOld->ppowBase,pboostOld->iLevel,"out",context);
    }

    return pboostOld;
}

/**********************************************************************func*
 * power_RemoveGlobalBoost
 *
 */
static Boost *power_RemoveGlobalBoost(Power *ppow, int iIdx, const char *context)
{
    Boost *pboostOld = NULL;

    assert(ppow!=NULL);
    assert(iIdx>=0);

    if(eaSize(&ppow->ppGlobalBoosts)>iIdx)
    {
        pboostOld = ppow->ppGlobalBoosts[iIdx];
        ppow->ppGlobalBoosts[iIdx] = NULL;
        ppow->bBoostsCalcuated = false;
        // This should already be recorded as leaving the system via 'used'
        //        if(pboostOld)
        //            DATA_MINE_ENHANCEMENT(pboostOld->ppowBase,pboostOld->iLevel,"out",context);
    }

    return pboostOld;
}

/**********************************************************************func*
 * power_DestroyBoost
 *
 */
void power_DestroyBoost(Power *ppow, int iIdx, const char *context)
{
    Boost *pboost = power_RemoveBoost(ppow, iIdx, context);
    if(pboost!=NULL)
    {
        boost_Destroy(pboost);
    }
}

/**********************************************************************func*
 * power_DestroyBoost
 *
 */
static void power_DestroyGlobalBoost(Power *ppow, int iIdx, const char *context)
{
    Boost *pboost = power_RemoveGlobalBoost(ppow, iIdx, context);
    if(pboost!=NULL)
    {
        boost_Destroy(pboost);
    }
}

/**********************************************************************func*
 * power_DestroyAllBoosts
 *
 */
void power_DestroyAllBoosts(Power *ppow, const char *context)
{
    int i;

    for(i=eaSize(&ppow->ppBoosts)-1; i>=0; i--)
    {
        power_DestroyBoost(ppow, i, context);
    }

    for(i=eaSize(&ppow->ppGlobalBoosts)-1; i>=0; i--)
    {
        power_DestroyGlobalBoost(ppow, i, context);
    }
}

/**********************************************************************func*
 * power_DestroyAllChanceMods
 *
 */
void power_DestroyAllChanceMods(Power *ppow)
{
#ifdef SERVER
    int i;
    for(i=eaSize(&ppow->ppmodChanceMods)-1; i>=0; i--)
    {
        attribmod_Destroy(ppow->ppmodChanceMods[i]);
    }
    eaDestroy(&ppow->ppmodChanceMods);
#endif
}

/**********************************************************************func*
 * power_CombineBoosts
 *
 */
void power_CombineBoosts( Character *pchar, Boost *pbA, Boost *pbB )
{
    Power *ppowDest;
    int iIdxDest;
    int iLevelDest;
    int iNumCombinesDest;
    Boost *pboost;
    float fChance;
    float fRand = (float)rand()/(float)(RAND_MAX+1);
    int iSuccess = 0, iSlot = 0, iReverse = 0;

    assert(pbA!=NULL);
    assert(pbB!=NULL);

    // Make A be the lower level of the two boosts
    if(pbA->iLevel>pbB->iLevel
           || (pbA->iLevel==pbB->iLevel && pbA->iNumCombines>pbB->iNumCombines))
    {
        pboost  = pbA;
        pbA     = pbB;
        pbB     = pboost;
        iReverse = 1;
    }

    // Figure out the destination location
      if( pbB->ppowParent )
    {
        ppowDest = pbB->ppowParent;
        iIdxDest = pbB->idx;

        if( iReverse )
            iSlot = 0;
    }
    else
    {
        ppowDest = pbA->ppowParent;
        iIdxDest = pbA->idx;

        if( !iReverse && pbA->iLevel!=pbB->iLevel )
            iSlot = 1;

    }

#if OLD_LEVEL_INCREASING_METHOD
    iLevelDest = pbB->iLevel+1;
    iNumCombinesDest = 0;
#else
    iLevelDest = pbB->iLevel;
    iNumCombinesDest = pbB->iNumCombines+1;
#endif

    fChance = boost_CombineChance(pbA, pbB);

    // No matter what, we're going to be nuking the smaller of the two.
    if(pbA->ppowParent)
    {
        power_DestroyBoost(pbA->ppowParent, pbA->idx, "combine");
    }
    else
    {
        character_DestroyBoost(pchar, pbA->idx, "combine");
    }

    // OK, now remove the second one from its location.
    if(pbB->ppowParent)
    {
        pboost = power_RemoveBoost(pbB->ppowParent, pbB->idx, NULL);
    }
    else
    {
        pboost = character_RemoveBoost(pchar, pbB->idx, NULL);
    }

    // Everything is now clean and ready for the new boost.

    if(fRand < fChance)
    {
#if OLD_LEVEL_INCREASING_METHOD
        int i;
#endif
        const BasePowerSet *pset = pboost->ppowBase->psetParent;
        const BasePower *ppowNew = NULL;

        iSuccess = true;

#if OLD_LEVEL_INCREASING_METHOD
        // Loop over the powers in the set and find the one which
        // most closely matches the level we want (without going over).
        for(i=eaSize(&pset->ppPowers)-1; i>=0; i--)
        {
            if(pset->ppPowers[i]->iLevel <= pbB->iLevel+1)
            {
                ppowNew = pset->ppPowers[i];
            }
        }
#else
        ppowNew = pboost->ppowBase;
#endif

        if(ppowNew!=NULL)
        {
#ifdef SERVER
            dbLog("Character:CombineEnhancement", pchar->entParent, "New boost %s.%d+%d now in %s.%d",
                dbg_BasePowerStr(ppowNew), iLevelDest, iNumCombinesDest,
                dbg_BasePowerStr(ppowDest->ppowBase), iIdxDest);
#endif
            power_ReplaceBoost(ppowDest, iIdxDest, ppowNew, iLevelDest, iNumCombinesDest, pboost->afVars, ARRAY_SIZE(pboost->afVars), pboost->aPowerupSlots, ARRAY_SIZE( pboost->aPowerupSlots ), NULL);
            boost_Destroy(pboost);
        }
        else
        {
            Errorf("Unable to find appropriate boost for combining original %s level %d\n", pbB->ppowBase->pchName, pbB->iLevel);
        }
    }
    else
    {
        // It failed. Replace the destination with the original.
#ifdef SERVER
        dbLog("Character:CombineEnhancement", pchar->entParent, "Failed: New boost %s.%d+%d now in %s.%d",
            dbg_BasePowerStr(pboost->ppowBase), pboost->iLevel, pboost->iNumCombines,
            dbg_BasePowerStr(ppowDest->ppowBase), iIdxDest);
#endif

        power_ReplaceBoostFromBoost(ppowDest, iIdxDest, pboost, NULL);
        boost_Destroy(pboost);
    }

#ifdef SERVER
    eaPush(&pchar->entParent->powerDefChange, powerRef_CreateFromPower(pchar->iCurBuild, ppowDest));
    character_SendCombineResponse( pchar->entParent, iSuccess, iSlot );
#endif
}

/**********************************************************************func*
* power_BoosterBoost
*
*/
void power_BoosterBoost( Character *pchar, Boost *pbA )
{
    Power *ppowDest;
    int iIdxDest;
    int iLevelDest;
    int iNumCombinesDest;
    Boost *pboost;
    float fChance;
    float fRand = (float)rand()/(float)(RAND_MAX+1);
    int iSuccess = 0, iSlot = 0, iReverse = 0;
    const SalvageItem *si = NULL;

    assert(pbA!=NULL);

    // Figure out the destination location
    ppowDest = pbA->ppowParent;
    iIdxDest = pbA->idx;
    iSlot = 0;

    iLevelDest = pbA->iLevel;
    iNumCombinesDest = pbA->iNumCombines+1;

    fChance = boost_BoosterChance(pbA);

    if(fRand < fChance)
    {
        const BasePowerSet *pset = NULL;
        const BasePower *ppowNew = NULL;

        iSuccess = true;
        pboost = power_RemoveBoost(pbA->ppowParent, pbA->idx, NULL);
        ppowNew = pboost->ppowBase;

        pset = pboost->ppowBase->psetParent;

#ifdef SERVER
        dbLog("Character:BoosterEnhancement", pchar->entParent, "New boost %s.%d+%d now in %s.%d",
            dbg_BasePowerStr(ppowNew), iLevelDest, iNumCombinesDest,
            dbg_BasePowerStr(ppowDest->ppowBase), iIdxDest);
#endif
        power_ReplaceBoost(ppowDest, iIdxDest, ppowNew, iLevelDest, iNumCombinesDest, pboost->afVars, ARRAY_SIZE(pboost->afVars), pboost->aPowerupSlots, ARRAY_SIZE( pboost->aPowerupSlots ), NULL);
        boost_Destroy(pboost);        
    }
    else
    {
        // It failed.
#ifdef SERVER
        dbLog("Character:BoosterEnhancement", pchar->entParent, "Failed: New boost %s.%d+%d now in %s.%d",
            dbg_BasePowerStr(pbA->ppowBase), pbA->iLevel, pbA->iNumCombines,
            dbg_BasePowerStr(ppowDest->ppowBase), iIdxDest);
#endif
    }

    // Consume enhancement booster either way
    si = salvage_GetItem( "S_EnhancementBooster" );
    if( si )
    {
        if(character_CanAdjustSalvage(pchar, si, -1) )
        {
            character_AdjustSalvage( pchar, si, -1, "Booster", false);
        }
    }

#ifdef SERVER
    eaPush(&pchar->entParent->powerDefChange, powerRef_CreateFromPower(pchar->iCurBuild, ppowDest));
    character_SendBoosterResponse( pchar->entParent, iSuccess, iSlot );
#endif
}



/**********************************************************************func*
* power_CatalyzeBoost
*
*/
void power_CatalyzeBoost( Character *pchar, Boost *pbA )
{
    Power *ppowDest;
    int iIdxDest;
    int iLevelDest;
    int iNumCombinesDest;
    Boost *pboost;
    int iSuccess = 0, iSlot = 0, iReverse = 0;
    const SalvageItem *si = NULL;
    const BasePowerSet *pset = NULL;
    const BasePower *ppowNew = NULL;

    assert(pbA!=NULL);

    // Figure out the destination location
    ppowDest = pbA->ppowParent;
    iIdxDest = pbA->idx;
    iSlot = 0;

    iLevelDest = pbA->iLevel;
    iNumCombinesDest = pbA->iNumCombines+1;

    iSuccess = true;
    pboost = power_RemoveBoost(pbA->ppowParent, pbA->idx, NULL);

    // find new power to replace this old one
    ppowNew = powerdict_GetBasePowerByFullName(&g_PowerDictionary, pboost->ppowBase->pchBoostCatalystConversion);
    pset = ppowNew->psetParent; 

#ifdef SERVER
    dbLog("Character:BoosterEnhancement", pchar->entParent, "New boost %s.%d+%d now in %s.%d",
        dbg_BasePowerStr(ppowNew), iLevelDest, iNumCombinesDest,
        dbg_BasePowerStr(ppowDest->ppowBase), iIdxDest);
#endif
    power_ReplaceBoost(ppowDest, iIdxDest, ppowNew, iLevelDest, 0, pboost->afVars, ARRAY_SIZE(pboost->afVars), pboost->aPowerupSlots, ARRAY_SIZE( pboost->aPowerupSlots ), NULL);
    boost_Destroy(pboost);

    // Consume enhancement booster either way
    si = salvage_GetItem( "S_EnhancementCatalyst" );
    if( si )
    {
        if(character_CanAdjustSalvage(pchar, si, -1) )
        {
            character_AdjustSalvage( pchar, si, -1, "Catalyst", false);
        }
    }

#ifdef SERVER
    eaPush(&pchar->entParent->powerDefChange, powerRef_CreateFromPower(pchar->iCurBuild, ppowDest));
    character_SendCatalystResponse( pchar->entParent, iSuccess, iSlot );
#endif
}

/**********************************************************************func*
* power_GetRadius
*
*/
float power_GetRadius(Power *ppow)
{
    assert(ppow!=NULL);

    return ppow->ppowBase->eEffectArea == kEffectArea_Cone
        ? ppow->ppowBase->fRadius*ppow->pattrStrength->fRange    // cones are modified by range
        : ppow->ppowBase->fRadius*ppow->pattrStrength->fRadius;
}


#ifdef SERVER

/**********************************************************************func*
* power_CheckUsageLimits
*
*/
bool power_CheckUsageLimits(Power *ppow)
{
    bool bLimitReached = false;

    if(ppow->ppowBase->bHasLifetime)
    {
        // If the power has expired, shut it off.
        // ARM NOTE:  We're explicitly reaching the usage limit on auto powers when their lifetime has expired!
        // character_AllowedToActivatePower explcitly DOES let auto powers pass, which means the auto power will activate.
        // So the lifetime-expired auto power will activate, tick (probably), and then deactivate all
        // within one character tick.
        if(ppow->ppowBase->fLifetime>0)
        {
            if(ppow->ulCreationTime + ((int) (ppow->ppowBase->fLifetime + 0.5f)) < timerSecondsSince2000())
            {
                bLimitReached = true;
            }
        }
        if(ppow->ppowBase->fLifetimeInGame>0)
        {
            if(ppow->fAvailableTime > ppow->ppowBase->fLifetimeInGame)
            {
                bLimitReached = true;
            }
        }
    }

    if(ppow->ppowBase->bHasUseLimit)
    {
        // If the power's use limits have been exceeded shut it off
        if(ppow->ppowBase->fUsageTime>0)
        {
            if(ppow->fUsageTime<=0)
            {
                bLimitReached = true;
            }
        }

        if(ppow->ppowBase->iNumCharges>0)
        {
            if(ppow->iNumCharges<=0)
            {
                bLimitReached = true;
            }
        }
    }

    if(bLimitReached)
    {
        //LOG_ENT( ppow->psetParent->pcharParent->entParent, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Power:Cancelled %s (usage limit reached)", dbg_BasePowerStr(ppow->ppowBase));
    }

    return bLimitReached;
}

/**********************************************************************func*
 * power_ChangeActiveStatus
 *
 * Changes the status of the power.  If the status really changed, change
 * will be reflected on the client of the power's owner.
 *
 * This function should always be used to change the power activation status
 * because activation status should always be reflected on the client.
 */
void power_ChangeActiveStatus(Power *ppow, int active)
{
    if((!!active)  == ppow->bActive)
        return;

    // If the power active status is being changed...
    // Takes advantage that true is 1 and not 0xffff
    ppow->bActive = (active&1 ? true : false);

    // Add this power to the list of powers that has changed its activation status
    // since last ent update.
    if(ppow->ppowBase->eType != kPowerType_Inspiration && ppow->ppowBase->eType != kPowerType_Boost)
    {
        // If this power has a usage limit, send a usage limit update
        if (ppow->ppowBase->bHasUseLimit)
            eaPush(&ppow->psetParent->pcharParent->entParent->usageStatusChange, powerRef_CreateFromPower(ppow->psetParent->pcharParent->iCurBuild, ppow));
        eaPush(&ppow->psetParent->pcharParent->entParent->activeStatusChange, powerRef_CreateFromPower(ppow->psetParent->pcharParent->iCurBuild, ppow));
    }
    
}

/**********************************************************************func*
 * power_ChangeRechargeTimer
 *
 * Changes the timer associated with the given power.  The timer status
 * will be reflected on the game client.
 *
 * This function should only be used to mark interesting recharge events.
 * The new value will always be transmitted to the client.
 */
void power_ChangeRechargeTimer(Power *ppow, float rechargeTime)
{
    ppow->fTimer = rechargeTime;

    if(ppow->ppowBase->eType != kPowerType_Inspiration && ppow->ppowBase->eType != kPowerType_Boost && ppow->ppowBase->eType != kPowerType_Auto)
    {
        eaPush(&ppow->psetParent->pcharParent->entParent->rechargeStatusChange, powerRef_CreateFromPower(ppow->psetParent->pcharParent->iCurBuild, ppow));
    }
}
#endif

/**********************************************************************func*
 * power_GetIndicies
 *
 */
bool power_GetIndices(const Power *ppow, int *piPowerSetIdx, int *piPowerIdx)
{
    assert(ppow!=NULL);
    assert(ppow->psetParent!=NULL);
    assert(piPowerSetIdx!=NULL);
    assert(piPowerIdx!=NULL);

    *piPowerSetIdx = ppow->psetParent->idx;
    *piPowerIdx = ppow->idx;

    return true;
}

#if SERVER
/***************************************************************************
 * power_AddPowerupslot
 *
 * Finds the next valid location to put the passed rewardslot
 */
bool power_AddPowerupslot( Power *b, RewardSlot *slot )
{
    bool res = false;
    int i;

    if( verify( b && slot ))
    {
        for( i = 0; i < ARRAY_SIZE(b->aPowerupSlots); ++i )
        {
            // NULL string will act as indicator of unused.
            if( b->aPowerupSlots[i].reward.name == NULL )
            {
                ZeroStruct(&b->aPowerupSlots[i]);
                b->aPowerupSlots[i].reward = *slot;

                res = true;
            }
        }
    }

    // --------------------
    // finally

    return res;
}

/***************************************************************************
 * power_ClearBoostTimers
 *
 */
void power_ClearBoostTimers(Power *ppow)
{
    int i;
    Boost **ppBoosts = ppow->ppBoosts;
    for(i=eaSize(&ppBoosts)-1; i>=0; i--)
    {
        if(ppBoosts[i] && ppBoosts[i]->ppowBase->fActivatePeriod > 0.0f)
        {
            ppBoosts[i]->fTimer = 0;
            //printf("%15.15s %15.15s Clr[%d]: %lf\n",
            //    ppow->psetParent->pcharParent->entParent->name, ppow->ppowBase->pchName, i, ppBoosts[i]->fTimer);
        }
    }
}

/***************************************************************************
 * power_IncrementBoostTimers
 *
 */
void power_IncrementBoostTimers(Power *ppow)
{
    int i;
    Boost **ppBoosts = ppow->ppBoosts;
    for(i=eaSize(&ppBoosts)-1; i>=0; i--)
    {
        // Only increment the boost timer if it's below the 0.0 point.
        // The 0 point is when the boost actually gets executed, so that's
        // the only time we want to increment.
        if(ppBoosts[i]
            && ppBoosts[i]->ppowBase->fActivatePeriod > 0.0f
            && ppBoosts[i]->fTimer <= 0.0f)
        {
            ppBoosts[i]->fTimer += ppBoosts[i]->ppowBase->fActivatePeriod;
            if(ppBoosts[i]->fTimer<-ppBoosts[i]->ppowBase->fActivatePeriod)
            {
                // This is here in case the server gets behind or the activate
                // period is very short. It makes sure the timer doesn't keep
                // going further and further negative.
                ppBoosts[i]->fTimer = -ppBoosts[i]->ppowBase->fActivatePeriod;
            }
            //printf("%15.15s %15.15s Inc[%d]: %lf\n",
            //    ppow->psetParent->pcharParent->entParent->name, ppow->ppowBase->pchName, i, ppBoosts[i]->fTimer);
        }
    }
}

/***************************************************************************
 * power_DecrementBoostTimers
 *
 */
void power_DecrementBoostTimers(Power *ppow, float fRate)
{
    int i;
    Boost **ppBoosts = ppow->ppBoosts;
    for(i=eaSize(&ppBoosts)-1; i>=0; i--)
    {
        if(ppBoosts[i] && ppBoosts[i]->ppowBase->fActivatePeriod > 0.0f)
        {
            ppBoosts[i]->fTimer -= fRate;
            //printf("%15.15s %15.15s Dec[%d]: %lf\n",
            //    ppow->psetParent->pcharParent->entParent->name, ppow->ppowBase->pchName, i, ppBoosts[i]->fTimer);
        }
    }
}
#endif


//-------------------------------------------------------------------------------------------
// PowerRef
//-------------------------------------------------------------------------------------------
MP_DEFINE(PowerRef);

PowerRef* powerRef_Create(void)
{
    MP_CREATE(PowerRef, 1000);
    return MP_ALLOC(PowerRef);
}

PowerRef* powerRef_CreateFromPower(int buildNum, Power *ppow)
{
    PowerRef *ppowRef = powerRef_Create();

    ppowRef->buildNum = buildNum;
    ppowRef->power = ppow->idx;
    ppowRef->powerSet = ppow->psetParent->idx;

    return ppowRef;
}

PowerRef* powerRef_CreateFromIdxs(int iIdxBuild, int iIdxSet, int iIdxPow)
{
    PowerRef *ppowRef = powerRef_Create();

    ppowRef->buildNum = iIdxBuild;
    ppowRef->powerSet = iIdxSet;
    ppowRef->power = iIdxPow;

    return ppowRef;
}

void powerRef_Destroy(PowerRef* ppowRef)
{
    // A spec of -1 is used as a sentinel, just ignore it.
    if(ppowRef && ppowRef != (PowerRef*)(intptr_t)-1)
    {
        MP_FREE(PowerRef, ppowRef);
    }
}


/***************************************************************************/
/***************************************************************************/


/**********************************************************************func*
 * powlist_GetNext
 *
 */
PowerListItem *powlist_GetNext(PowerListIter *piter)
{
    assert(piter!=NULL);
    assert(piter->pphead!=NULL);

    piter->pposCur = piter->pposNext;
    if(piter->pposNext!=NULL)
    {
        piter->pposNext = piter->pposNext->next;
    }

    return piter->pposCur;
}

/**********************************************************************func*
 * powlist_GetFirst
 *
 */
PowerListItem *powlist_GetFirst(PowerList *plist, PowerListIter *piter)
{
    assert(plist!=NULL);
    assert(piter!=NULL);

    piter->pphead = plist;
    piter->pposNext = *plist;

    return powlist_GetNext(piter);
}

/**********************************************************************func*
 * powlist_RemoveCurrent
 *
 */
void powlist_RemoveCurrent(PowerListIter *piter)
{
    assert(piter!=NULL);

    listFreeMember(piter->pposCur, (void**)piter->pphead);
}

/**********************************************************************func*
 * powlist_Add
 *
 */
void powlist_Add(PowerList *plist, Power *ppow, unsigned int uid)
{
    PowerListItem *ppowListItem;

    assert(plist!=NULL);
    assert(ppow!=NULL);

    ppowListItem = (PowerListItem *)listAddNewMember((void**)plist, sizeof(PowerListItem));
    ppowListItem->ppow = ppow;
    ppowListItem->uiID = uid;
}

/**********************************************************************func*
 * powlist_AddWithTargets
 *
 */
void powlist_AddWithTargets(PowerList *plist, Power *ppow, EntityRef erTarget, Vec3 vecTarget, unsigned int invokeID)
{
    PowerListItem *ppowListItem;

    assert(plist!=NULL);
    assert(ppow!=NULL);

    ppowListItem = (PowerListItem *)listAddNewMember((void**)plist, sizeof(PowerListItem));
    ppowListItem->ppow = ppow;
    ppowListItem->uiID = invokeID;
    ppowListItem->erTarget = erTarget;
    copyVec3(vecTarget, ppowListItem->vecTarget);
}

/**********************************************************************func*
 * powlist_RemoveAll
 *
 */
void powlist_RemoveAll(PowerList *plist)
{
    assert(plist!=NULL);

    listClearList((void**)plist);
}

/**********************************************************************func*
 * inspiration_Create
 *
 */
Power *inspiration_Create(const BasePower *ppowBase, Character *p, int idx)
{
    Power *ppow;

    MP_CREATE(Power, 16);

    ppow = MP_ALLOC(Power);

    assert(ppowBase!=NULL);
    assert(p!=NULL);
    assert(ppow!=NULL);

    ppow->ppowBase = ppow->ppowOriginal = ppowBase;
    ppow->psetParent = NULL; // I could fake one here, let's hope I don't need to.
    ppow->idx = idx;

    return ppow;
}

/**********************************************************************func*
 * inspiration_Destroy
 *
 */
void inspiration_Destroy(Power *ppow)
{
    assert(ppow!=NULL);

    power_Destroy(ppow, NULL);
}

bool inspiration_IsTradeable(const BasePower *power, const char *authFrom, const char *authTo)
{
    if (!power || !power->bBoostTradeable)
        return false;

    if (power->bBoostAccountBound && stricmp(authFrom, authTo) != 0)
        return false;

    return true;
}

/**********************************************************************func*
 * character_checkPowersAndRemoveBadPowers
 *
 */
bool character_checkPowersAndRemoveBadPowers(Character * p)
{
    int retval = true;
#if SERVER
    int iset, ipow;
    int iPowerSetCnt, iPowerCnt;
    bool bFoundInherent = 0, bFoundAccolade = 0, bFoundPrimary = 0, bFoundSecondary = 0;
    bool bIsVillainEpic = false;

    if (ENTTYPE(p->entParent) != ENTTYPE_PLAYER || p->entParent->access_level > 0)
        return true;

    iPowerSetCnt = eaSize(&p->ppPowerSets);

    if( p->pclass && p->pclass->pchName )
    {
        if( stricmp( p->pclass->pchName, "Class_Arachnos_Soldier" ) == 0 ||
            stricmp( p->pclass->pchName, "Class_Arachnos_Widow" ) == 0)
        {
            bIsVillainEpic = true;
        }
    }

    for( iset = 0; iset < iPowerSetCnt; iset++ )
    {
        PowerSet *pSet = p->ppPowerSets[iset];
        iPowerCnt = eaSize(&pSet->ppPowers);

        if(pSet && stricmp(pSet->psetBase->pchName, "Inherent")==0)
        {
            if(bFoundInherent)
            {
                //LOG_ENT_OLD(p->entParent, "character_checkPowersAndRemoveBadPowers CHEAT: Two copies of Inherent Powerset.");
            }
            else
            {
                bFoundInherent = true;
            }
        }
        else if(pSet && stricmp(pSet->psetBase->pcatParent->pchName, "Inherent")==0)
        {
            // TODO: Test something about other inherent powersets (fitness) here?
        }
        else if(pSet && stricmp(pSet->psetBase->pchName, "Temporary_Powers")==0)
        {

        }
        else if(pSet && stricmp(pSet->psetBase->pchName, "Accolades")==0)
        {
            if(bFoundAccolade) // found two accolade sets
            {
                //LOG_ENT_OLD(p->entParent, "character_checkPowersAndRemoveBadPowers CHEAT: Two Accolade Powersets.");
            }
            else
            {
                bFoundAccolade = true;
            }
        }
        else if(pSet && pSet->psetBase->pcatParent == p->pclass->pcat[kCategory_Primary])
        {
            if(bFoundPrimary && !bIsVillainEpic) // found two primaries where not allowed
            {
                //LOG_ENT_OLD(p->entParent, "character_checkPowersAndRemoveBadPowers CHEAT: Two Primary Powersets.");
            }
            else
            {
                bFoundPrimary = true;
            }
        }
        else if(pSet && pSet->psetBase->pcatParent == p->pclass->pcat[kCategory_Secondary])
        {
            if(bFoundSecondary && !bIsVillainEpic) // found two secondaries where not allowed
            {
                //LOG_ENT_OLD(p->entParent, "character_checkPowersAndRemoveBadPowers CHEAT: Two Secondary Powersets.");
            }
            else
            {
                bFoundSecondary = true;
            }
        }
        else if(pSet && pSet->psetBase->pcatParent == p->pclass->pcat[kCategory_Pool])
        {
            // TODO: Test something about pools here?
        }
        else if(pSet && pSet->psetBase->pcatParent == p->pclass->pcat[kCategory_Epic])
        {
            // TODO: Test something about epic power sets here?
        }
        else if(pSet && stricmp(pSet->psetBase->pcatParent->pchName, "Temporary_Powers")==0)
        {
            // TODO: Test something about random temp powers here?
        }
        else
        {
            //LOG_ENT_OLD(p->entParent, "character_checkPowersAndRemoveBadPowers CHEAT: Bad Powerset.");
        }

        if (pSet != NULL)
        {
            for( ipow = 0; ipow < iPowerCnt; ipow++ )
            {
                Power * pPowCheck = pSet->ppPowers[ipow];

                if (pPowCheck == NULL)
                    continue;


                // Are they high enough level to own this power?
                if ((baseset_BasePowerAvailableByIdx(pPowCheck->ppowBase->psetParent,
                                                        pPowCheck->ppowBase->id.ipow, 
                                                        pPowCheck->iLevelBought ) > 0 
                                                            || (pPowCheck->ppowBase->ppchBuyRequires
                                                                && !chareval_Eval(p, pPowCheck->ppowBase->ppchBuyRequires, pPowCheck->ppowBase->pchSourceFile))) &&
                    (strnicmp(pPowCheck->ppowBase->pchName, "prestige", 8) != 0))
                {
                    //LOG_ENT( p->entParent, LOG_CHEATING, LOG_LEVEL_IMPORTANT, 0, "CHEAT: Invalid. Power: %s, BoughtAt: %d", 
                    //            pPowCheck->ppowBase->pchName, pPowCheck->iLevelBought);
                }
            }
        }
    }
#endif
    return retval != 0;
}

/**************************************************************************/
/**************************************************************************/


typedef struct 
{
    char *oldName;
    char *newName;
} PowerNameMap;

// the hashes for replacing obsolete powers/powersets
typedef struct 
{
    PowerNameMap **names;
    PowerNameMap **sets;
    StashTable htPowerSetReplacement;
    StashTable htPowerReplacement;
} ReplacementHashes;

ReplacementHashes replacementHashes = {0};

/**********************************************************************func*
 * power_LoadReplacementHash
 *
 */
void power_LoadReplacementHash(void)
{
    TODO(); // Move to shared memory
    // This is currently a very small data set (and regular, mostly replacing the same suffixes)
    // But if this data is used in frequent traversals then it might benefit shared server
    // performance to have it in shared memory.
    if ( !replacementHashes.htPowerSetReplacement && !replacementHashes.htPowerReplacement )
    {
        int i;
        TokenizerParseInfo ParsePowerNameMap[] =
        {
            {"Old",        TOK_STRING(PowerNameMap, oldName, 0)},
            {"New",        TOK_STRING(PowerNameMap, newName, 0)},
            {"End",        TOK_END},
            { "", 0 }
        };
        TokenizerParseInfo ParsePowerReplacements[] =
        {
            {"PowerSet",    TOK_STRUCT(ReplacementHashes, sets, ParsePowerNameMap)},
            {"Power",        TOK_STRUCT(ReplacementHashes, names, ParsePowerNameMap)},
            { "", 0 }
        };

        //pReplacementHashes = calloc(1, sizeof(ReplacementHashes));
        ParserLoadFiles(NULL, "defs/replacepowernames.def", "replacepowernames.bin", PARSER_EMPTYOK, ParsePowerReplacements, &replacementHashes, NULL, NULL, NULL);

        if ( !replacementHashes.htPowerReplacement )
            replacementHashes.htPowerReplacement = stashTableCreateWithStringKeys(30, StashDefault);
        if ( !replacementHashes.htPowerSetReplacement )
            replacementHashes.htPowerSetReplacement = stashTableCreateWithStringKeys(30, StashDefault);

        for ( i = 0; i < eaSize(&replacementHashes.names); ++i )
        {
            stashAddPointer(replacementHashes.htPowerReplacement, replacementHashes.names[i]->oldName, replacementHashes.names[i]->newName, 0);
        }
        for ( i = 0; i < eaSize(&replacementHashes.sets); ++i )
        {
            stashAddPointer(replacementHashes.htPowerSetReplacement, replacementHashes.sets[i]->oldName, replacementHashes.sets[i]->newName, 0);
        }
    }
}

/**********************************************************************func*
 * power_GetReplacementPowerSetName
 *
 */
const char *power_GetReplacementPowerSetName(const char *pchPowerSetName)
{
    char *ret = NULL;
    if ( pchPowerSetName || replacementHashes.htPowerSetReplacement )
        stashFindPointer(replacementHashes.htPowerSetReplacement, pchPowerSetName, (void**)&ret);
    return ret;
}

/**********************************************************************func*
 * power_GetReplacementPowerName
 *
 */
const char *power_GetReplacementPowerName(const char *pchPowerName)
{
    char *ret = NULL;
    if ( replacementHashes.htPowerReplacement )
        stashFindPointer(replacementHashes.htPowerReplacement, pchPowerName, (void**)&ret);
    return ret;
}

/**********************************************************************func*
* basepower_GetReplacement
* TODO: caching these would make this a lot faster
*/
const BasePower* basepower_GetReplacement(const BasePower *ppowBase)
{
    if(ppowBase && ppowBase->pchName && 
        ppowBase->psetParent && ppowBase->psetParent->pchName && 
        ppowBase->psetParent->pcatParent && ppowBase->psetParent->pcatParent->pchName)
    {
        const char *newPowerName = power_GetReplacementPowerName(ppowBase->pchName);
        const char *newSetName = power_GetReplacementPowerSetName(ppowBase->psetParent->pchName);
        if(newPowerName && newSetName)
            return powerdict_GetBasePowerByName(&g_PowerDictionary,ppowBase->psetParent->pcatParent->pchName,ppowBase->psetParent->pchName,ppowBase->pchName);
    }
    return ppowBase;
}


/**************************************************************************/
/**************************************************************************/

/***************************************************************************
 * mapBoostSetGroupNameToBasePowerName
 *
 */
void mapBoostSetGroupNameToBasePowerName(const BasePower *ppow, BoostSetDictionary *bdict)
{
    if ( !ppow || !ppow->pchFullName || !ppow->pBoostSetMember || !ppow->pBoostSetMember->pchGroupName )
        return;

    stashAddPointerConst(bdict->htBasePowerToGroupNameMap, ppow->pchFullName, ppow->pBoostSetMember->pchGroupName, false);
    stashAddPointerConst(bdict->htBasePowerToSetNameMap, ppow->pchFullName, ppow->pBoostSetMember->pchDisplayName, false);
}

/***************************************************************************
 * boostSetGroupNameFromBasePowerName
 *
 */
const char *boostSetGroupNameFromBasePowerName(const char *basePowerName)
{
    const char *ret = NULL;
    if ( !stashFindPointerConst(g_BoostSetDict.htBasePowerToGroupNameMap, basePowerName, cpponly_reinterpret_cast(const void **)(&ret)) )
        return NULL;
    return ret;
}

/***************************************************************************
 * boostSetNameFromBasePowerName
 *
 */
const char *boostSetNameFromBasePowerName(const char *basePowerName)
{
    const char *ret = NULL;
    if ( !stashFindPointerConst(g_BoostSetDict.htBasePowerToSetNameMap, basePowerName, cpponly_reinterpret_cast(const void**)(&ret)) )
        return NULL;
    return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////
// P O W E R   S L O T T I N G   E V A L
/////////////////////////////////////////////////////////////////////////////////////////

static EvalContext *s_pPowerSlotEval;

/**********************************************************************func*
 * power_sloteval_powerBoostsSlotted
 *
 */
void power_sloteval_powerBoostsSlotted(EvalContext *pcontext)
{
    Entity *e;
    Power *pPower;
    const char *enhancement = eval_StringPop(pcontext);
    bool bFoundEnt = eval_FetchInt(pcontext, "Entity", (int *)&e);
    bool bFoundPower = eval_FetchInt(pcontext, "Power", (int *)&pPower);
    int k;
    int count = 0;
    int numBoosts;

    if(bFoundEnt && bFoundPower && e && e->pchar)
    {
        numBoosts = eaSize(&pPower->ppBoosts);
        for (k = 0; k < numBoosts; k++)
        {
            Boost *pBoost = pPower->ppBoosts[k];

            if (!pBoost)
                continue;

            if (!stricmp(enhancement, pBoost->ppowBase->pchName))
                count++;
        }
        eval_IntPush(pcontext, count);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}


/**********************************************************************func*
 * power_sloteval_Init
 *
 */
static void power_sloteval_Init(void)
{
    static bool s_bInitDone = false;

    if(s_bInitDone)
        return;

    // Create the evaluation context used by everyone.
    s_pPowerSlotEval = eval_Create();

    // Add in all basic entity and supergroup EvalFuncs
    chareval_AddDefaultFuncs(s_pPowerSlotEval);

    // Add in special cases for power.
    eval_RegisterFunc(s_pPowerSlotEval, "PowerBoostsSlotted>",   power_sloteval_powerBoostsSlotted,   1, 1); // to be added in the future!

    s_bInitDone=true;
}


/**********************************************************************func*
 * recipeeval_Eval
 *
 */
bool power_sloteval_Eval(Power *pPower, Boost *pBoost, Entity *e, const char * const *ppchExpr)
{
    if(!ppchExpr)
        return true;

    if(!s_pPowerSlotEval)
        power_sloteval_Init();

    eval_StoreInt(s_pPowerSlotEval, "Entity", (intptr_t)e);
    eval_StoreInt(s_pPowerSlotEval, "Power", (intptr_t)pPower);
    eval_StoreInt(s_pPowerSlotEval, "Boost", (intptr_t)pBoost);
    eval_ClearStack(s_pPowerSlotEval);

    eval_Evaluate(s_pPowerSlotEval, ppchExpr);
    return eval_IntPeek(s_pPowerSlotEval) != 0;
}


bool power_ShowInInventory(Character *pchar, Power *ppow)
{
    if (!pchar || !ppow)
    {
        return false;
    }
    else if (ppow->ppowBase->psetParent->eShowInInventory == kShowPowerSetting_Always)
    {
        return true;
    }
    else if (ppow->ppowBase->psetParent->eShowInInventory == kShowPowerSetting_Never)
    {
        return false;
    }
    else if (ppow->ppowBase->psetParent->eShowInInventory == kShowPowerSetting_IfUsable)
    {
        return (ppow->psetParent->pcharParent == pchar) && ppow->bEnabled;
    }
    else if (ppow->ppowBase->psetParent->eShowInInventory == kShowPowerSetting_IfOwned)
    {
        return (ppow->psetParent->pcharParent == pchar);
    }
    else // ppow->ppowBase->pSetParent->eShowInInventory == kShowPowerSetting_Default
    {
        if (ppow->ppowBase->eShowInInventory == kShowPowerSetting_Always ||
            ppow->ppowBase->eShowInInventory == kShowPowerSetting_Default)
        {
            return true;
        }
        else if (ppow->ppowBase->eShowInInventory == kShowPowerSetting_IfUsable &&
            character_OwnsPower(pchar, ppow->ppowBase) && ppow->bEnabled)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

static int power_GetCategory(Power* power)
{
    if (power && power->psetParent && power->psetParent->pcharParent &&
        power->psetParent->psetBase && power->psetParent->psetBase->pcatParent)
    {
        Character* character = power->psetParent->pcharParent;
        const PowerCategory* powerCategory = power->psetParent->psetBase->pcatParent;

        if (character->pclass)
        {
            int i;
            for (i = 0; i < kCategory_Count; ++i)
            {
                if (powerCategory == character->pclass->pcat[i])
                    return i;
            }
        }
    }
    
    return -1;
}


Power* power_GetPetCreationPower(Character* character)
{
    if (character && character->entParent && character->ppowCreatedMe)
    {
        Entity* creatorEntity = erGetEnt(character->entParent->erCreator);

        while (creatorEntity && creatorEntity->erCreator)
            creatorEntity = erGetEnt(creatorEntity->erCreator);

        if (creatorEntity)
        {
            return character_OwnsPower(creatorEntity->pchar,
                character->ppowCreatedMe);
        }
    }

    return NULL;
}


ColorPair power_GetTint(Power* power)
{
    if (power && power->psetParent && power->psetParent->pcharParent)
    {
        Character* character = power->psetParent->pcharParent;
        PowerCustomizationList* powerCust;

        if (character->ppowCreatedMe) // Is the character a pet?
        {
            Power* creationPower = power_GetPetCreationPower(character);
            
            // If things are sane this shouldn't infinitely recurse
            return creationPower ? power_GetTint(creationPower) : power->ppowBase->fx.defaultTint;
        }

        powerCust = powerCustList_current(character->entParent);
        if (powerCust)
        {
            PowerCustomization* customization = powerCust_FindPowerCustomization(powerCust, power->ppowOriginal);
            
            if (customization && !isNullOrNone(customization->token))
                return customization->customTint;
        }

        return power->ppowBase->fx.defaultTint;
    }

    return colorPairNone;
}


const char* power_GetToken(Power* power)
{
    if (power && power->psetParent && power->psetParent->pcharParent)
    {
        const BasePower *ppowBase = power->ppowOriginal;
        if (ppowBase && ppowBase->customfx)
        {
            Character* character = power->psetParent->pcharParent;

            PowerCustomizationList* powerCustList = powerCustList_current(character->entParent);

            if (powerCustList)
            {
                PowerCustomization* customization = powerCust_FindPowerCustomization(powerCustList, ppowBase);

                if (customization)
                    return customization->token;
            }
        }
    }

    return NULL;
}

const char* power_GetRootToken(Power* power)
{
    if (power && power->psetParent && power->psetParent->pcharParent && 
        power->psetParent->pcharParent->ppowCreatedMe) // Is the character a pet?
    {
        Power* creationPower = power_GetPetCreationPower(power->psetParent->pcharParent);
        return power_GetToken(creationPower);
    }
    else
        return power_GetToken(power);
}

const PowerFX* power_GetFXFromToken(const BasePower *ppowBase, const char * token)
{
    if (token != NULL && stricmp(token, "none"))
    {
        int i;

        for(i = eaSize(&ppowBase->customfx)-1; i >= 0; --i)
        {
            if(stricmp(ppowBase->customfx[i]->pchToken, token) == 0)
                return &ppowBase->customfx[i]->fx;
        }
    }

    return &ppowBase->fx;
}
const PowerFX* power_GetFX(Power* power)
{
    const char* token = NULL;
    if (!power || !power->ppowBase)
        return NULL;

    if (power->psetParent && power->psetParent->pcharParent &&
        power->psetParent->psetBase && power->psetParent->psetBase->pcatParent)
    {
        
        Character* character = power->psetParent->pcharParent;
        
        if (character->ppowCreatedMe) // Is the character a pet?
        {
            Power* creationPower = power_GetPetCreationPower(character);
            token = power_GetToken(creationPower);
        }
        else
            token = power_GetToken(power);
    }

    return power_GetFXFromToken(power->ppowBase, token);
}


bool power_IsTinted(Power* power)
{
    Character* character;
    const char* token;

    if (!power || !power->ppowBase)
        return false;

    if (!power->psetParent || !power->psetParent->pcharParent ||
        !power->psetParent->psetBase || 
        !power->psetParent->psetBase->pcatParent)
    {
        return false;
    }

    if (!power->psetParent || !power->psetParent->pcharParent)
        return false;

    character = power->psetParent->pcharParent;
    if (character->ppowCreatedMe)
    {
        Power* creationPower = power_GetPetCreationPower(character);
            
        // If things are sane this shouldn't infinitely recurse
        if(creationPower)
        {
            return power_IsTinted(creationPower);
        }
    }

    token = power_GetToken(power);
    if (!isNullOrNone(token))
        return true;

    return power->ppowBase->fx.defaultTint.primary.a != 0 &&
           power->ppowBase->fx.defaultTint.secondary.a != 0;
}

#if SERVER
void power_findRedirection(Entity *src, Entity *target, Power *power)
{
    if (!power || !src || !target)
        return;

    assert(power->ppowOriginal);

    power->ppowBase = power->ppowOriginal;

    if (power->ppowBase->bPowerRedirector)
    {
        combateval_StoreCustomFXToken(power_GetRootToken(power));
    }
    while (power->ppowBase->bPowerRedirector)
    {
        //    find the base power to use
        const BasePower *ppowBase = power->ppowBase;
        int i;
        for (i = 0; i < eaSize(&ppowBase->ppTemplates); ++i)
        {
            const AttribModTemplate *ptemplate = ppowBase->ppTemplates[i];
            if (ptemplate->offAttrib == kSpecialAttrib_PowerRedirect)
            {
                devassert(ptemplate->ppchApplicationRequires);
                if (ptemplate->ppchApplicationRequires && combateval_Eval(src, target, power, ptemplate->ppchApplicationRequires, power->ppowBase->pchSourceFile))
                {
                    const BasePower *newBase = powerdict_GetBasePowerByFullName(&g_PowerDictionary, ptemplate->ppchPrimaryStringList[0]);
                    if (newBase)
                    {
                        power->ppowBase = newBase;
                        break;
                    }
                }
            }
        }
        if (power->ppowBase == ppowBase)
        {
            Errorf("Power %s Redirection didn't find a valid base power", power->ppowBase->pchFullName);
            break;
        }
    }
}

#endif
/**********************************************************************func*
* basepower_GetAIMaxLevel
*
* Returns the max level the AI should use this power at.  If there is no max
*    9999 is returned.
*
*/
int basepower_GetAIMaxLevel(const BasePower *pBasePower)
{
    assert(pBasePower!=NULL);

    if (!EAINTINRANGE(pBasePower->id.ipow, pBasePower->psetParent->piAIMaxLevel) ||
        pBasePower->psetParent->piAIMaxLevel[pBasePower->id.ipow] == 0)
        return 9999;

    return pBasePower->psetParent->piAIMaxLevel[pBasePower->id.ipow];
}


/**********************************************************************func*
* basepower_GetAIMinLevel
*
* Returns the min level the AI should use this power at.  If there is no min
*    0 is returned.
*
*/
int basepower_GetAIMinLevel(const BasePower *pBasePower)
{
    assert(pBasePower!=NULL);

    if (!EAINTINRANGE(pBasePower->id.ipow, pBasePower->psetParent->piAvailable))
        return 0;

    return pBasePower->psetParent->piAvailable[pBasePower->id.ipow];
}


/**********************************************************************func*
* basepower_GetAIMaxRankCon
*
* Returns the max rank con the AI should use this power at.  If there is no min
*    9999 is returned.
*
*/
int basepower_GetAIMaxRankCon(const BasePower *pBasePower)
{
    assert(pBasePower!=NULL);

    if (!EAINTINRANGE(pBasePower->id.ipow, pBasePower->psetParent->piAIMaxRankCon))
        return 9999;

    return pBasePower->psetParent->piAIMaxRankCon[pBasePower->id.ipow];
}

int basepower_GetMaxDifficulty(const BasePower *pBasePower)
{
    assert(pBasePower!=NULL);

    if (!EAINTINRANGE(pBasePower->id.ipow, pBasePower->psetParent->piMaxDifficulty))
        return 9999;

    return pBasePower->psetParent->piMaxDifficulty[pBasePower->id.ipow];
}

/**********************************************************************func*
* basepower_GetAIMinRankCon
*
* Returns the min rank con the AI should use this power at.  If there is no min
*    -9999 is returned.
*
*/
int basepower_GetAIMinRankCon(const BasePower *pBasePower)
{
    assert(pBasePower!=NULL);

    if (!EAINTINRANGE(pBasePower->id.ipow, pBasePower->psetParent->piAIMinRankCon))
        return -9999;

    return pBasePower->psetParent->piAIMinRankCon[pBasePower->id.ipow];
}

int basepower_GetMinDifficulty(const BasePower *pBasePower)
{
    assert(pBasePower!=NULL);

    if (!EAINTINRANGE(pBasePower->id.ipow, pBasePower->psetParent->piMinDifficulty))
        return 2;

    return pBasePower->psetParent->piMinDifficulty[pBasePower->id.ipow];
}

void power_verifyValidMMPowers()
{
    int i;
    int minPowersForCategory[3] = { 1,1,0 };
    const PowerCategory *powerSet[3];
    powerSet[0]  = powerdict_GetCategoryByName(&g_PowerDictionary, "Mission_Maker_Attacks");
    powerSet[1]  = powerdict_GetCategoryByName(&g_PowerDictionary, "Mission_Maker_Secondary");
    powerSet[2]  = powerdict_GetCategoryByName(&g_PowerDictionary, "Mission_Maker_Movement");

    //    for attacks, secondary, movement powersets
    for ( i = 0; i < 3; ++i)
    {
        int j;
        //    for each powerset in the category
        for ( j = 0; j < eaSize(&powerSet[i]->ppPowerSets); ++j)
        {
            int difficulty;
            //    for each difficulty
            for ( difficulty = 0; difficulty < 3; difficulty++ )
            {
                int rankCon;
                //    minions rank 0 and archvillains rankcon to 5, so make sure to check those
                for (rankCon = 0; rankCon <= 5; rankCon++)
                {
                    int level;
                    for (level = 0; level <= 54; level++)
                    {
                        int powerCount = 0;
                        int k;

                        const BasePowerSet *bset;
                        bset = powerSet[i]->ppPowerSets[j];
                        //    count up all the powers that can be attained for difficulty
                        for ( k = 0; k < eaSize(&bset->ppPowers); ++k)
                        {
                            if (    ( difficulty >= basepower_GetMinDifficulty(bset->ppPowers[k]) ) &&
                                ( difficulty <= basepower_GetMaxDifficulty(bset->ppPowers[k]) ) && 
                                //    also check against the minion rank con since that will have the least amount of powers
                                ( rankCon >= basepower_GetAIMinRankCon(bset->ppPowers[k]) )  &&
                                ( rankCon <= basepower_GetAIMaxRankCon(bset->ppPowers[k]) ) && 
                                ( level >= basepower_GetAIMinLevel(bset->ppPowers[k]) ) &&
                                ( level <= basepower_GetAIMaxLevel(bset->ppPowers[k]) ) )
                            {
                                powerCount++;
                            }
                        }
                        //    if its less than the count for this set
                        if (powerCount < minPowersForCategory[i] )
                        {
                            // display a warning
//                            Errorf("Custom Critter Powers: Powerset %s only had %d powers available for rankCon %d of difficulty %d and level %d. Was expecting at least %d available powers. Confer with a powers designer.", 
//                                bset->pchFullName, powerCount, rankCon, difficulty, level, minPowersForCategory[i]);
                        }
                    }
                }
            }
        }
    }
}

float basepower_CalculateAreaFactor(const BasePower *ppowBase)
{
    if (ppowBase->bUseNonBoostTemplatesOnMainTarget)
        return 1.0f;
    else if (ppowBase->eEffectArea == kEffectArea_Cone)
        return (1.0f + (ppowBase->fRadius * 0.15)) - ((ppowBase->fRadius*0.00036667f)*(360 - DEG(ppowBase->fArc)));
    else if (ppowBase->eEffectArea == kEffectArea_Sphere)
        return (1.0f + (ppowBase->fRadius * 0.15));
    else
        return 1.0f;
}

/* End of File */
