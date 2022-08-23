/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/utils/error.h>
#include "entity/SgrpBadges.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/eval.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/assert/assert.h>

#include "entity/entPlayer.h"

#include "player/badges.h"
#include "BadgeStats.h"
#include "badges_server.h"
#include <utilitieslib/components/StashTable.h>

StashTable g_hashBadgeStatUsage;
StashTable g_hashBadgeStatNames;
BadgeStatHashes g_BadgeStatsSgroup;
int g_iMaxBadgeStatIdx;

/**********************************************************************func*
 * LoadBadgeStatNames
 *
 */
int LoadBadgeStatNames(char *fileName, StashTable hashBadgeStatNames, StashTable ghNameFromIdx )
{
    char *s,*mem,*args[10];
    int idx,max_idx=0,count;

    mem = fileAlloc(fileName, 0);

    if(!mem)
    {
        printf("ERROR: Can't open %s\n", fileName);
        return 0;
    }

    s = mem;
    while((count=tokenize_line(s,args,&s)))
    {
        StashElement elt;
        if (count != 2)
            continue;
        idx = atoi(args[0]);
        if(idx>max_idx)
            max_idx=idx;

        stashAddInt(hashBadgeStatNames, args[1], idx, false);
        stashFindElement(hashBadgeStatNames, args[1], &elt);    
        if( ghNameFromIdx && verify(elt))
        {
            // piggy-back on the interned string.
            stashIntAddPointer(ghNameFromIdx, idx, (char*)stashElementGetStringKey(elt), false);
        }
    }
    free(mem);

    return max_idx;
}

/**********************************************************************func*
 * BadgeCatch
 *
 */
static void PushDependencyForCurrentBadge(EvalContext *pcontext, const char *pchStat)
{
    StashTable hashBadgeStatUsage = 0;
    StashElement elem;
    int *piBadgeList=NULL;
    int iBadgeIdx;

    if(eval_FetchInt(pcontext, "Badge", (int *)&iBadgeIdx)
       && eval_FetchInt(pcontext, "hashBadgeStatUsage", (int*)&hashBadgeStatUsage))
    {
        stashFindElement(hashBadgeStatUsage, pchStat, &elem);
        if(elem)
        {
            int i, n;
            piBadgeList = (int *)stashElementGetPointer(elem);

            // Don't put in duplicate badges.
            // This is done only at startup, so this linear search shouldn't
            //   be a problem.
            n = eaiSize(&piBadgeList);
            for(i=0; i<n; i++)
            {
                if(piBadgeList[i]==iBadgeIdx)
                {
                    return;
                }
            }
        }
        else
        {
            eaiCreate(&piBadgeList);
        }
        eaiPush(&piBadgeList, iBadgeIdx);
        stashAddInt(hashBadgeStatUsage, pchStat, (int)(intptr_t)piBadgeList, true);
    }
}

/**********************************************************************func*
 * StatCatch
 *
 */
static void StatCatch(EvalContext *pcontext)
{
    int *piBadgeList=NULL;

    const char *rhs = eval_StringPop(pcontext);
    eval_IntPush(pcontext, 0);

    PushDependencyForCurrentBadge(pcontext, rhs);
}

/**********************************************************************func*
 * CharCatch
 *
 */
static void CharCatch(EvalContext *pcontext)
{
    int *piBadgeList=NULL;

    const char *rhs = eval_StringPop(pcontext);

    if(stricmp(rhs, "origin")==0
        || strnicmp(rhs, "arch", 4)==0
        || strnicmp(rhs, "rank", 4)==0
        || stricmp(rhs, "type")==0
        || stricmp(rhs, "group")==0
        || stricmp(rhs, "entref")==0
        || stricmp(rhs, "enttype")==0
        || stricmp(rhs, "costume")==0)
    {
        eval_StringPush(pcontext, "*string*");
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }

    PushDependencyForCurrentBadge(pcontext, "*char");
}

/**********************************************************************func*
 * SystemCatch
 *
 */
static void SystemCatch(EvalContext *pcontext)
{
    const char *rhs = eval_StringPop(pcontext);

    if(stricmp(rhs, "locale")==0
        || stricmp(rhs, "ShardEvent.Name")==0
        || stricmp(rhs, "ShardEvent.Signal")==0)
    {
        eval_StringPush(pcontext, "*string*");
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }

    PushDependencyForCurrentBadge(pcontext, "*system");
}

/**********************************************************************func*
 * SystemCatchNoParam
 *
 */
static void SystemCatchNoParam(EvalContext *pcontext)
{
    eval_IntPush(pcontext, 0);

    PushDependencyForCurrentBadge(pcontext, "*system");
}


/**********************************************************************func*
 * TokenOwned
 *
 */
static void TokenOwned(EvalContext *pcontext)
{
    const char *tokenName = eval_StringPop(pcontext);
    eval_IntPush(pcontext, 0);
    PushDependencyForCurrentBadge(pcontext, "*char");
}

/**********************************************************************func*
 * SgrpCatch
 *
 */
static void SgrpCatch(EvalContext *pcontext)
{
    int *piBadgeList=NULL;

    const char *rhs = eval_StringPop(pcontext);
    if(stricmp(rhs, "type")==0)
    {
        eval_StringPush(pcontext, "*string*");
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }


    PushDependencyForCurrentBadge(pcontext, "*sgrp");
}

/**********************************************************************func*
 * SgrpTokenOwned
 *
 */
static void SgrpTokenOwned(EvalContext *pcontext)
{
    int *piBadgeList=NULL;

    const char *rhs = eval_StringPop(pcontext);
    eval_IntPush(pcontext, 0);

    PushDependencyForCurrentBadge(pcontext, "*sgrp");
}

/**********************************************************************func*
 * BadgeCatch
 *
 */
static void BadgeCatch(EvalContext *pcontext)
{
    int *piBadgeList=NULL;

    const char *rhs = eval_StringPop(pcontext);
    eval_IntPush(pcontext, 0);

    PushDependencyForCurrentBadge(pcontext, "*badge");
}

/**********************************************************************func*
 * SgrpBadgeCatch
 *
 */
static void SgrpBadgeCatch(EvalContext *pcontext)
{
    const char *rhs = eval_StringPop(pcontext);
    eval_IntPush(pcontext, 0);

    PushDependencyForCurrentBadge(pcontext, "*sgbadge");
}

/**********************************************************************func*
 * BadgeCatchNoParam
 *
 */
static void BadgeCatchNoParam(EvalContext *pcontext)
{
    int *piBadgeList=NULL;

    eval_IntPush(pcontext, 0);

    PushDependencyForCurrentBadge(pcontext, "*badge");
}

/**********************************************************************func*
* LoyaltyCatchNoParam
*
*/
static void LoyaltyCatchNoParam(EvalContext *pcontext)
{
    int *piBadgeList=NULL;

    eval_IntPush(pcontext, 0);

    PushDependencyForCurrentBadge(pcontext, "*char");
}


/**********************************************************************func*
 * SgrpBadgeCatchNoParam
 *
 */
static void SgrpBadgeCatchNoParam(EvalContext *pcontext)
{
    int *piBadgeList=NULL;

    eval_IntPush(pcontext, 0);

    PushDependencyForCurrentBadge(pcontext, "*sgbadge");
}

/**********************************************************************func*
 * SouvenirCatch
 *
 */
static void SouvenirCatch(EvalContext *pcontext)
{
    const char *rhs = eval_StringPop(pcontext);
    eval_IntPush(pcontext, 0);

    PushDependencyForCurrentBadge(pcontext, "*souvenir");
}

/**********************************************************************func*
 * ModeCatch
 *
 */
static void ModeCatch(EvalContext *pcontext)
{
    const char *rhs = eval_StringPop(pcontext);
    eval_IntPush(pcontext, 0);

    PushDependencyForCurrentBadge(pcontext, "*mode");
}

/**********************************************************************func*
 * AuthCatch
 *
 */
static void AuthCatch(EvalContext *pcontext)
{
    const char *rhs = eval_StringPop(pcontext);
    eval_IntPush(pcontext, 0);

    PushDependencyForCurrentBadge(pcontext, "*auth");
}

/**********************************************************************func*
 * badge_LoadStatUsage
 *
 */
StashTable LoadBadgeStatUsage(StashTable hashBadgeStatUsage, const BadgeDefs *badgeDefs, char *dataFilename)
{
    int i;
    EvalContext *pcontext;

    int n = eaSize(&badgeDefs->ppBadges);

    // Clear out the existing table of names so we just see the ones
    //   actually being referenced.
    if(hashBadgeStatUsage)
    {
        stashTableDestroy(hashBadgeStatUsage);
    }
    hashBadgeStatUsage = stashTableCreateWithStringKeys(128, StashDeepCopyKeys);

    // Create the eval context. This one uses a special int> to capture
    //   the names of the stats.
    // NOTE: keep in sync with badges_server.c:void badge_EvalInit(void).
    pcontext = eval_Create();
    eval_SetBlame(pcontext, dataFilename);

    eval_RegisterFunc(pcontext, "int>",                  StatCatch,             1, 1);
    eval_RegisterFunc(pcontext, "stat>",                 StatCatch,             1, 1);
    eval_RegisterFunc(pcontext, "char>",                 CharCatch,             1, 1);
    eval_RegisterFunc(pcontext, "ent>",                  CharCatch,             1, 1);
    eval_RegisterFunc(pcontext, "sgrp>",                 SgrpCatch,             1, 1);
    eval_RegisterFunc(pcontext, "owns?",                 BadgeCatch,            1, 1);
    eval_RegisterFunc(pcontext, "owned?",                BadgeCatch,            1, 1);
    eval_RegisterFunc(pcontext, "BadgeOwned?",           BadgeCatch,            1, 1);
    eval_RegisterFunc(pcontext, "sg.owns?",              SgrpBadgeCatch,        1, 1);
    eval_RegisterFunc(pcontext, "sg.owned?",             SgrpBadgeCatch,        1, 1);
    eval_RegisterFunc(pcontext, "badgecount",            BadgeCatchNoParam,     0, 1);
    eval_RegisterFunc(pcontext, "sg.badgecount",         SgrpBadgeCatchNoParam, 0, 1);
    eval_RegisterFunc(pcontext, "CostumeKey?",           TokenOwned,            1, 1);
    eval_RegisterFunc(pcontext, "TokenOwned?",           TokenOwned,            1, 1);
    eval_RegisterFunc(pcontext, "TokenVal>",             CharCatch,             1, 1);
    eval_RegisterFunc(pcontext, "sg.TokenOwned?",        SgrpTokenOwned,        1, 1);
    eval_RegisterFunc(pcontext, "HasSouvenir?",          SouvenirCatch,         1, 1);
    eval_RegisterFunc(pcontext, "mode?",                 ModeCatch,             1, 1);
    eval_RegisterFunc(pcontext, "system>",                SystemCatch,           1, 1);
    eval_RegisterFunc(pcontext, "auth>",                AuthCatch,             1, 1);
    eval_RegisterFunc(pcontext, "LoyaltyNodesBought>",    LoyaltyCatchNoParam,   0, 1);
    eval_RegisterFunc(pcontext, "LoyaltyPointsEarned>",    LoyaltyCatchNoParam,   0, 1);
    eval_RegisterFunc(pcontext, "ProductOwned?",           TokenOwned,            1, 1);
    eval_RegisterFunc(pcontext, "ProductAvailable?",       TokenOwned,            1, 1);
    eval_RegisterFunc(pcontext, "LoyaltyOwned?",           TokenOwned,            1, 1);
    eval_RegisterFunc(pcontext, "hasContact?",            CharCatch,             1, 1);
    eval_RegisterFunc(pcontext, "isBetaShard?",            SystemCatchNoParam,       0, 1);
    eval_RegisterFunc(pcontext, "isVIP?",                LoyaltyCatchNoParam,   0, 1);
    eval_RegisterFunc(pcontext, "isAccountInventoryLoaded?", LoyaltyCatchNoParam,   0, 1);

    // Left out since it's for critters only:
    // eval_RegisterFunc(pContext, "HasTag?",     CharCatch,             1, 1);

    // Left out since it doesn't affect "dirtyness" of the badge
    // eval_RegisterFunc(pcontext, "%",           Percent,               1, 1);

    // store the stat hashtable
    eval_StoreInt(pcontext, "hashBadgeStatUsage", (int)(intptr_t)hashBadgeStatUsage );

    
    for(i=0; i<n; i++)
    {
        char *errorString = NULL;
        bool success = false;
        if(badgeDefs->ppBadges[i]->iIdx > 0 )
        {
            eval_StoreInt(pcontext, "Badge", badgeDefs->ppBadges[i]->iIdx);
        }
        else
        {
            ErrorFilenamef( dataFilename, "badge %s appears to be invalid, its id '%d' is not valid",
                            badgeDefs->ppBadges[i]->pchName, badgeDefs->ppBadges[i]->iIdx );
        }


        success = eval_Validate(pcontext, badgeDefs->ppBadges[i]->pchName, badgeDefs->ppBadges[i]->ppchKnown,&errorString);
        if (!success)
            ErrorFilenamef(dataFilename, errorString);

        success = eval_Validate(pcontext, badgeDefs->ppBadges[i]->pchName, badgeDefs->ppBadges[i]->ppchVisible,&errorString);
        if (!success)
            ErrorFilenamef(dataFilename, errorString);

        success = eval_Validate(pcontext, badgeDefs->ppBadges[i]->pchName, badgeDefs->ppBadges[i]->ppchRequires,&errorString);
        if (!success)
            ErrorFilenamef(dataFilename, errorString);

        success = eval_Validate(pcontext, badgeDefs->ppBadges[i]->pchName, badgeDefs->ppBadges[i]->ppchMeter,&errorString);
        if (!success)
            ErrorFilenamef(dataFilename, errorString);

        eval_Evaluate(pcontext, badgeDefs->ppBadges[i]->ppchKnown);
        // Also pick up trailing and singleton names
        while(!eval_StackIsEmpty(pcontext))
        {
            eval_IntPop(pcontext);
        }

        eval_Evaluate(pcontext, badgeDefs->ppBadges[i]->ppchVisible);
        // Also pick up trailing and singleton names
        while(!eval_StackIsEmpty(pcontext))
        {
            eval_IntPop(pcontext);
        }

        eval_Evaluate(pcontext, badgeDefs->ppBadges[i]->ppchRequires);
        // Also pick up trailing and singleton names
        while(!eval_StackIsEmpty(pcontext))
        {
            eval_IntPop(pcontext);
        }

        eval_Evaluate(pcontext, badgeDefs->ppBadges[i]->ppchRevoke);
        // Also pick up trailing and singleton names
        while(!eval_StackIsEmpty(pcontext))
        {
            eval_IntPop(pcontext);
        }

        eval_Evaluate(pcontext, badgeDefs->ppBadges[i]->ppchMeter);
        // Also pick up trailing and singleton names
        while(!eval_StackIsEmpty(pcontext))
        {
            eval_IntPop(pcontext);
        }

    }

    eval_Destroy(pcontext);

    return hashBadgeStatUsage;
}

/* End of File */
