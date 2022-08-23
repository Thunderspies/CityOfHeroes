/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>

#include <utilitieslib/utils/textparser.h>

#include "skillobj.h"

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


TokenizerParseInfo ParseMapSkillObjectives[] =
{
    { "{",                TOK_START,                   0 },
    { "Map",              TOK_STRING(MapSkillObjectives, pchMap, 0)          },
    { "VillainGroup",     TOK_STRING(MapSkillObjectives, pchVillainGroup, 0) },
    { "Objectives",       TOK_STRINGARRAY(MapSkillObjectives, ppchSkillObjectives)  },
    { "}",                TOK_END,                     0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseMapSkillObjectivesDict[] =
{
    { "MapObjectives",    TOK_STRUCT(MapSkillObjectivesDict, ppMapSkillObjectives, ParseMapSkillObjectives) },
    { "", 0, 0 }
};

SHARED_MEMORY MapSkillObjectivesDict g_MapSkillObjectivesDict;
cStashTable g_hashMapsAndVillainsToSkillObjectives;

/**********************************************************************func*
 * MapSkillObjectivesDictPostprocess
 *
 */
static bool MapSkillObjectivesDictPostprocess(TokenizerParseInfo pti[], MapSkillObjectivesDict *pdict, bool shared_memory)
{
    int i;
    int n;
    char *pch;

    estrCreate(&pch);

    if(!g_hashMapsAndVillainsToSkillObjectives)
        g_hashMapsAndVillainsToSkillObjectives = stashTableCreateWithStringKeys(100, StashDeepCopyKeys);

    n = eaSize(&pdict->ppMapSkillObjectives);
    for(i=0; i<n; i++)
    {
        const char *pchKey=NULL;

        if(pdict->ppMapSkillObjectives[i]->pchMap && pdict->ppMapSkillObjectives[i]->pchVillainGroup)
        {
            estrClear(&pch);

            estrConcatCharString(&pch, pdict->ppMapSkillObjectives[i]->pchMap);
            estrConcatChar(&pch, ':');
            estrConcatCharString(&pch, pdict->ppMapSkillObjectives[i]->pchVillainGroup);

            pchKey = pch;
        }
        else if(pdict->ppMapSkillObjectives[i]->pchMap && !pdict->ppMapSkillObjectives[i]->pchVillainGroup)
        {
            pchKey = pdict->ppMapSkillObjectives[i]->pchMap;
        }
        else if(pdict->ppMapSkillObjectives[i]->pchVillainGroup && !pdict->ppMapSkillObjectives[i]->pchMap)
        {
            pchKey = pdict->ppMapSkillObjectives[i]->pchVillainGroup;
        }

        if(pchKey)
        {
            stashAddPointerConst(g_hashMapsAndVillainsToSkillObjectives, pchKey, &pdict->ppMapSkillObjectives[i]->ppchSkillObjectives, false);
        }
    }

    estrDestroy(&pch);
    return true;
}

/**********************************************************************func*
 * MapSkillObjectivesDictPreprocess
 *
 */
static bool MapSkillObjectivesDictPreprocess(TokenizerParseInfo pti[], MapSkillObjectivesDict *pdict)
{
    int i;
    int n;

    n = eaSize(&pdict->ppMapSkillObjectives);
    for(i=0; i<n; i++)
    {
        if(pdict->ppMapSkillObjectives[i]->pchMap)
        {
            forwardSlashes(cpp_const_cast(char*)(pdict->ppMapSkillObjectives[i]->pchMap));
        }
    }

    return true;
}

/**********************************************************************func*
 * load_SkillObjectives
 *
 */
void load_SkillObjectives(void)
{
    int i = 0;
    ParserLoadFilesShared("SM_MapSkillObjectives", NULL, "defs/MapObjectives.def", "MapObjectives.bin",
        PARSER_SERVERONLY,
        ParseMapSkillObjectivesDict, &g_MapSkillObjectivesDict, sizeof(g_MapSkillObjectivesDict),
        NULL, NULL,
        MapSkillObjectivesDictPreprocess, NULL, MapSkillObjectivesDictPostprocess);
}

/**********************************************************************func*
 * skillobj_GetPossibleSkillObjectives
 *
 */
const char * const **skillobj_GetPossibleSkillObjectives(const char *pchMap, const char *pchVillainGroup)
{
    int iLen;
    cStashElement elem = NULL;

    // verbose_printf("obj_GetPossibleSkillObjectives(%s, %s)\n", pchMap?pchMap:"(null)", pchVillainGroup?pchVillainGroup:"(null)");
    if(pchMap && *pchMap && pchVillainGroup && *pchVillainGroup)
    {
        char *pchKey;
        char *pchTmp = strdup(pchMap);
        forwardSlashes(pchTmp);

        estrCreate(&pchKey);

        iLen = 0;
        while(!elem && iLen!=strlen(pchTmp))
        {
            estrPrintCharString(&pchKey, pchTmp);
            estrConcatChar(&pchKey, ':');
            estrConcatCharString(&pchKey, pchVillainGroup);

            // Look for an exact match
            // verbose_printf("   %s\n", pchKey);
            stashFindElementConst(g_hashMapsAndVillainsToSkillObjectives, pchKey, &elem);

            iLen = (int)strlen(pchTmp);
            pchTmp = getDirectoryName(pchTmp);
        }

        estrDestroy(&pchKey);
        free(pchTmp);
    }

    if(!elem && pchMap && *pchMap)
    {
        char *pchTmp = strdup(pchMap);
        forwardSlashes(pchTmp);

        iLen = 0;
        while(!elem && iLen!=strlen(pchTmp))
        {
            // verbose_printf("   %s\n", pchTmp);
            stashFindElementConst(g_hashMapsAndVillainsToSkillObjectives, pchTmp, &elem);

            iLen = (int)strlen(pchTmp);
            pchTmp = getDirectoryName(pchTmp);
        }

        free(pchTmp);
    }

    if(!elem && pchVillainGroup && *pchVillainGroup)
    {
        // verbose_printf("   %s\n", pchVillainGroup);
        stashFindElementConst(g_hashMapsAndVillainsToSkillObjectives, pchVillainGroup, &elem);
    }

    if(elem)
    {
        return (const char * const **)stashElementGetPointerConst(elem);
    }

    // verbose_printf("   Nothing found\n");
    // verbose_printf("Done\n");
    return NULL;
}



/* End of File */
