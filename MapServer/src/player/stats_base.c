/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>

#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/utils.h>

#include "dbcomm/staticMapInfo.h"
#include "gameComm/VillainDef.h"
#include "dbcomm/dbcomm.h"

#include "stats_base.h"
#include "pl_stats_internal.h"


extern StaticMapInfo** staticMapInfos;

StatResultDesc **g_ppResultDescs;

// These let me map from a name to a index and back again.
static const char **s_ppchStatNames;
static StashTable s_hashStatNames;
static StashTable s_hashResultTables;

static int s_iCntStats = 0;

static StatResultDesc descUniversal[]=
{
    { "overall",              kStat_Kills     },
    { "damage_given",         kStat_General   },
    { "healing_given",        kStat_General   },
    { "overall",              kStat_Influence },
    { "race_founders_falls",  kStat_General   },
    { "pvp",                  kStat_Kills     },
    { "pvp",                  kStat_Deaths    },
    { "arena",                kStat_Kills     },
    { "arena",                kStat_Deaths    },
    { 0 },
};

static char *s_apchSpecialStats[] =
{
    "damage_given",
    "damage_taken",
    "healing_given",
    "healing_taken",
    "overall",
    "race_founders_falls",
    NULL
};

static char *s_achNames[kStat_Count] =
{
    "General",
    "Kills",
    "Deaths",
    "Time",
    "XP",
    "Influence",
    "Wisdom",
};

static char *s_achPeriodNames[kStatPeriod_Count] =
{
    "Today",
    "Yesterday",
    "ThisMonth",
    "LastMonth",
};

/**********************************************************************func*
 * StuffStatNames
 *
 */
void StuffStatNames(StuffBuff *psb)
{
    int i;
    for(i=eaSize(&s_ppchStatNames)-1; i>=0; i--)
    {
        addStringToStuffBuff(psb,"\"%s\"\n", s_ppchStatNames[i]);
    }
}


/**********************************************************************func*
 * GetDescName
 *
 */
static char *GetDescName(const char *pchName, int iCat)
{
    static char ach[256];
    sprintf(ach, "%s:%d", pchName, iCat);
    return ach;
}

/**********************************************************************func*
 * InitResultDescs
 *
 */
static void InitResultDescs(void)
{
    StatResultDesc *pdesc;
    int i;
    int n;

    s_hashResultTables = stashTableCreateWithStringKeys(128, StashDeepCopyKeys);

    if(!g_ppResultDescs)
    {
        eaCreate(&g_ppResultDescs);
    }
    else
    {
        eaSetSize(&g_ppResultDescs, 0);
    }

    // Add the basic ones.
    for(pdesc=descUniversal; pdesc->pchItem!=NULL; pdesc++)
    {
        StatResultDesc *pnew = (StatResultDesc *)calloc(sizeof(StatResultDesc), 1);
        *pnew = *pdesc;

        stashAddInt(s_hashResultTables, GetDescName(pnew->pchItem, pnew->eCat), (int)(intptr_t)pnew, false);
        eaPush(&g_ppResultDescs, pnew);
    }

    // OK, add all the maps
    n = eaSize(&staticMapInfos);
    for (i = 0; i < n; i++)
    {
        StaticMapInfo* info = staticMapInfos[i];

        if(!info->dontAutoStart && !info->introZone)
        {
            StatResultDesc *pnew = (StatResultDesc *)calloc(sizeof(StatResultDesc), 1);

            pnew->pchItem = info->name;
            pnew->eCat = kStat_Kills;
            stashAddInt(s_hashResultTables, GetDescName(pnew->pchItem, pnew->eCat), (int)(intptr_t)pnew, false);
            eaPush(&g_ppResultDescs, pnew);

            pnew = (StatResultDesc *)calloc(sizeof(StatResultDesc), 1);
            pnew->pchItem = info->name;
            pnew->eCat = kStat_XP;
            stashAddInt(s_hashResultTables, GetDescName(pnew->pchItem, pnew->eCat), (int)(intptr_t)pnew, false);
            eaPush(&g_ppResultDescs, pnew);
        }
    }

    // All the villain groups
    for (i = 0; i < VG_MAX; i++)
    {
        StatResultDesc *pnew = (StatResultDesc *)calloc(sizeof(StatResultDesc), 1);

        pnew->pchItem = villainGroupGetName(i);
        pnew->eCat = kStat_Kills;
        stashAddInt(s_hashResultTables, GetDescName(pnew->pchItem, pnew->eCat), (int)(intptr_t)pnew, false);
        eaPush(&g_ppResultDescs, pnew);
    }

}

/**********************************************************************func*
 * InitStats
 *
 */
void InitStats(void)
{
    char **ppch;
    int i;
    int n;
    StaticDefineInt *sdi;
    StashElement he;


    InitResultDescs();


    s_hashStatNames = stashTableCreateWithStringKeys(128, StashDeepCopyKeys);
    eaCreateConst(&s_ppchStatNames);

#ifdef STAT_TRACK_LEVEL
    // level0 - level49
    for(i = 0; i<50; i++)
    {
        bool bFound = stashAddIntAndGetElement(s_hashStatNames, stat_LevelStatName(i), s_iCntStats, false, &he);
        assert( bFound );
        eaPush(&s_ppchStatNames, stashElementGetStringKey(he));

        s_iCntStats++;
    }
#endif

    // All the villain groups
    for (i = 0; i < VG_MAX; i++)
    {
        bool bFound = stashAddIntAndGetElement(s_hashStatNames, villainGroupGetName(i), s_iCntStats, false, &he);
        assertmsgf( bFound, "InitStats: failed to find villian group \"%s\" (duplicate name?)", villainGroupGetName(i));
        eaPushConst(&s_ppchStatNames, stashElementGetStringKey(he));
        s_iCntStats++;
    }


    // The various villain ranks
    sdi = ParseVillainRankEnum;
    while(sdi->key != DM_END)
    {
        if((uintptr_t)sdi->key > DM_NOTYPE)
        {
            bool bFound = stashAddIntAndGetElement(s_hashStatNames, sdi->key, s_iCntStats, false, &he);
            assert( bFound );
            eaPushConst(&s_ppchStatNames, stashElementGetStringKey(he));
            s_iCntStats++;
        }
        sdi++;
    }

#ifdef STAT_TRACK_VILLAIN_TYPE
    // And all the villains
    for(i =eaSize(&villainDefs)-1; i>= 0; i--)
    {
        bool bFound = stashAddIntAndGetElement(s_hashStatNames, villainDefs[i]->name, s_iCntStats, &he);
        assert( bFound );
        eaPush(&s_ppchStatNames, stashElementGetStringKey(he));

        s_iCntStats++;
    }
#endif

    // Zone stuff
    n = eaSize(&staticMapInfos);
    for (i = 0; i < n; i++)
    {
        StaticMapInfo* info = staticMapInfos[i];

        if(!info->dontAutoStart)
        {
            bool bFound =  stashAddIntAndGetElement(s_hashStatNames, info->name, s_iCntStats, false, &he);
            assert( bFound );
            eaPushConst(&s_ppchStatNames, stashElementGetStringKey(he));
            s_iCntStats++;
        }
    }

    // Plus anything extra
    ppch = s_apchSpecialStats;
    while(*ppch!=NULL)
    {
        bool bFound = stashAddIntAndGetElement(s_hashStatNames, *ppch, s_iCntStats, false, &he);
        assert( bFound );
        eaPushConst(&s_ppchStatNames, stashElementGetStringKey(he));
        s_iCntStats++;
        ppch++;
    }

}

/**********************************************************************func*
 * stat_GetStatCount
 *
 */
int stat_GetStatCount(void)
{
    return s_iCntStats;
}

/**********************************************************************func*
 * stat_GetStatIndex
 *
 */
int stat_GetStatIndex(const char *pch)
{
    int i;
    if(stashFindInt(s_hashStatNames, pch, &i))
    {
        return i;
    }

    return -1;
}

/**********************************************************************func*
 * stat_GetCategoryName
 *
 */
char *stat_GetCategoryName(int num)
{
    if(num < kStat_Count)
    {
        return s_achNames[num];
    }

    assert("GetCategoryName: Asked for slot with no name.");
    return NULL;
}

/**********************************************************************func*
 * stat_GetStatName
 *
 */
const char *stat_GetStatName(int num)
{
    if(num < s_iCntStats)
    {
        return s_ppchStatNames[num];
    }

    assert("GetStatName: Asked for slot with no name.");
    return NULL;
}

/**********************************************************************func*
 * stat_GetPeriodName
 *
 */
char *stat_GetPeriodName(int num)
{
    if(num < kStatPeriod_Count)
    {
        return s_achPeriodNames[num];
    }

    assert("GetPeriodName: No such time period.");
    return NULL;
}

/**********************************************************************func*
 * stat_ZoneStatName
 *
 */
char *stat_ZoneStatName(void)
{
    return db_state.map_name;
}

/**********************************************************************func*
 * stat_LevelStatName
 *
 */
char *stat_LevelStatName(int iLevel)
{
    static char s_achScratch[64];
    sprintf(s_achScratch, "level.%02d", iLevel);
    return s_achScratch;
}

/**********************************************************************func*
 * stat_GetTable
 *
 */
DBStatResult *stat_GetTable(const char *pchName, StatCategory cat, StatPeriod per)
{
    int i;

    if(stashFindInt(s_hashResultTables, GetDescName(pchName, cat), &i))
    {
        StatResultDesc *pdesc = (StatResultDesc*)(intptr_t)i;
        return &pdesc->aRes[per];
    }

    return NULL;
}

/* End of File */
