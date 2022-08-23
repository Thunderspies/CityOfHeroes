/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>

#include <utilitieslib/utils/eval.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/bitfield.h>

#include "entity/entity.h"
#include "entity/entPlayer.h"    // for PlayerType
#include "entity/Supergroup.h"

#include "player/badges.h"
#include "entity/RewardToken.h"
#include "bases/basedata.h"
#include "bases/DetailRecipe.h"

#include "supergroup_eval.h"
#include <utilitieslib/utils/utils.h>


//***************************************************************************
//***************************************************************************
//
// Helpers - These take get their arguments from the function call (as
//           opposed to the EvalContext) and put their result back into the
//           EvalContext. They are meant to be used by regular EvalFuncs.
//
//***************************************************************************
//***************************************************************************

/**********************************************************************func*
 * sgeval_SupergroupFetchHelper
 *
 */
void sgeval_SupergroupFetchHelper(EvalContext *pcontext, Supergroup *sg, const char *rhs)
{
    if(verify(sg) && rhs)
    {
        if(stricmp(rhs, "influence")==0)
        {
            eval_IntPush(pcontext, sg->influence);
        }
        else if(stricmp(rhs, "prestige")==0)
        {
            eval_IntPush(pcontext, sg->prestige);
        }
        else if(stricmp(rhs, "type")==0)
        {
            if(sg->playerType == kPlayerType_Villain)
                eval_StringPush(pcontext, "villain");
            else
                eval_StringPush(pcontext, "hero");
        }
        else
        {
            eval_IntPush(pcontext, 0);
        }
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
 * sgeval_SupergroupTokenOwnedHelper
 *
 */
void sgeval_SupergroupTokenOwnedHelper(EvalContext *pcontext, Supergroup *sg, const char *rhs)
{
    int i = -1;

    if(sg && rhs)
    {
        i = rewardtoken_IdxFromName(&sg->rewardTokens, rhs);
    }

    eval_IntPush(pcontext, i>=0 ? 1:0);
}


/**********************************************************************func*
 * sgeval_SupergroupTokenValHelper
 *
 */
void sgeval_SupergroupTokenValHelper(EvalContext *pcontext, Supergroup *sg, const char *rhs)
{
    int i = -1;

    if(sg && rhs)
    {
        i = rewardtoken_IdxFromName(&sg->rewardTokens, rhs);
    }

    eval_IntPush(pcontext, i>=0 ? sg->rewardTokens[i]->val : 0);
}


/**********************************************************************func*
 * sgeval_SupergroupRecipeOwnedHelper
 *
 */
void sgeval_SupergroupRecipeOwnedHelper(EvalContext *pcontext, Supergroup *sg, const char *rhs)
{
    int i = 0;

#ifndef STATSERVER
    if(sg && rhs)
    {
        const DetailRecipe *recipe = detailrecipedict_RecipeFromName(rhs);
        RecipeInvItem *item = sgrp_FindRecipeInvItem(sg, recipe);
        if(item)
        {
            i = item->count;
        }
    }
#endif

    eval_IntPush(pcontext, i);
}


/**********************************************************************func*
 * sgeval_SupergroupHelper
 *
 */
Supergroup *sgeval_SupergroupHelper(EvalContext *pcontext)
{
    Entity *e = NULL;
    Supergroup *sg = NULL;
    bool bFound = eval_FetchInt(pcontext, "Supergroup", (int *)&sg) || eval_FetchInt(pcontext, "Entity", (int *)&e);

    if(bFound && !sg && e)
    {
        sg = e->supergroup;
    }

    return sg;
}

//***************************************************************************
//***************************************************************************
//
// EvalFuncs - These take their arguments from the EvalContext and put their
//             results back into the EvalContext. Any or all of these could
//             be broken up into EvalFuncs and Helpers (above) if the need
//             arises. Helpers have only been made for things which actually
//             needed to be helpers.
//
//***************************************************************************
//***************************************************************************

/**********************************************************************func*
 * sgeval_SupergroupTokenOwned
 *
 */
void sgeval_SupergroupTokenOwned(EvalContext *pcontext)
{
    Supergroup *sg = sgeval_SupergroupHelper(pcontext);
    const char *rhs = eval_StringPop(pcontext);

    if(sg)
    {
        sgeval_SupergroupTokenOwnedHelper(pcontext, sg, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
 * sgeval_SupergroupTokenVal
 *
 */
void sgeval_SupergroupTokenVal(EvalContext *pcontext)
{
    Supergroup *sg = sgeval_SupergroupHelper(pcontext);
    const char *rhs = eval_StringPop(pcontext);

    if(sg)
    {
        sgeval_SupergroupTokenValHelper(pcontext, sg, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
 * sgeval_SupergroupRecipeOwned
 *
 */
void sgeval_SupergroupRecipeOwned(EvalContext *pcontext)
{
    Supergroup *sg = sgeval_SupergroupHelper(pcontext);
    const char *rhs = eval_StringPop(pcontext);

    if(sg)
    {
        sgeval_SupergroupRecipeOwnedHelper(pcontext, sg, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
 * sgeval_SupergroupOwnsBadge
 *
 */
void sgeval_SupergroupOwnsBadge(EvalContext *pcontext)
{
    Supergroup *sg = sgeval_SupergroupHelper(pcontext);
    const char *rhs = eval_StringPop(pcontext);

    if(sg)
    {
        int iIdx;
        if(sgrpbadge_GetIdxFromName(rhs, &iIdx))
        {
            if( iIdx <= g_SgroupBadges.idxMax )
            {
#if SERVER
                eval_IntPush(pcontext, sgrpbadges_IsOwned(&sg->badgeStates, iIdx));
#else
                if(EAINTINRANGE(iIdx,sg->badgeStates.eaiStates))
                {
                    int badgeField = sg->badgeStates.eaiStates[iIdx];
                    eval_IntPush(pcontext, BADGE_IS_OWNED(badgeField)?1:0);
                }
#endif
                return;
            }
        }
    }

    eval_IntPush(pcontext, 0);
}

/**********************************************************************func*
 * sgeval_SupergroupCountBadges
 *
 */
void sgeval_SupergroupCountBadges(EvalContext *pcontext)
{
    Supergroup *sg = sgeval_SupergroupHelper(pcontext);
    if(sg)
    {
        int i;
        int iCnt = 0;
        int iSize = eaSize(&g_SgroupBadges.ppBadges);
        for(i = 0; i < iSize; i++)
        {
            if(g_SgroupBadges.ppBadges[i]->eCategory!=kBadgeType_Internal
                && !g_SgroupBadges.ppBadges[i]->bDoNotCount
                && g_SgroupBadges.ppBadges[i]->iIdx<=g_SgroupBadges.idxMax)
            {
#if CLIENT
                int iBadge = sg->badgeStates.eaiStates[g_SgroupBadges.ppBadges[i]->iIdx];
                if(BADGE_IS_OWNED(iBadge))
#else
                if(sgrpbadges_IsOwned(&sg->badgeStates, g_SgroupBadges.ppBadges[i]->iIdx))
#endif
                {
                    iCnt++;
                }
            }
        }
        eval_IntPush(pcontext, iCnt);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
* sgeval_SupergroupEmptyIoPMounts
*
*/
void sgeval_SupergroupEmptyIoPMounts(EvalContext *pcontext)
{
    Supergroup *sg = sgeval_SupergroupHelper(pcontext);
    if(sg)
    {
        eval_IntPush(pcontext, sgrp_emptyMounts(sg));
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
 * sgeval_SupergroupFetch
 *
 */
void sgeval_SupergroupFetch(EvalContext *pcontext)
{
    Supergroup *sg = sgeval_SupergroupHelper(pcontext);
    const char *rhs = eval_StringPop(pcontext);

    if(sg)
    {
        sgeval_SupergroupFetchHelper(pcontext, sg, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}


/**********************************************************************func*
* sgeval_SupergroupHasPermission
*
*/
void sgeval_SupergroupHasPermission(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);

    if(e && e->supergroup)
    {
        eval_IntPush(pcontext, sgroup_hasPermissionByName( e, rhs));
        return;
    }

    eval_IntPush(pcontext, 0);
}

/**********************************************************************func*
* sgeval_SupergroupHasIoP
*
*/
void sgeval_SupergroupHasIoP(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Entity", (int *)&e);

    if(e && e->supergroup)
    {
        int i, count = eaSize(&e->supergroup->specialDetails);
        for (i = 0; i < count; i++)
        {
            if (e->supergroup->specialDetails[i]->pDetail && 
                stricmp(rhs, e->supergroup->specialDetails[i]->pDetail->pchName) == 0)
            {
                eval_IntPush(pcontext, 1);
                return;
            }

        }

        eval_IntPush(pcontext, 0);
        return;
    }

    eval_IntPush(pcontext, 0);
}


//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************

/**********************************************************************func*
 * sgeval_AddDefaultFuncs
 *
 */
void sgeval_AddDefaultFuncs(EvalContext *pContext)
{
    if(!pContext)
        return;

    eval_RegisterFunc(pContext, "sgrp>",                sgeval_SupergroupFetch,                1, 1);
    eval_RegisterFunc(pContext, "sg.Owns?",                sgeval_SupergroupOwnsBadge,            1, 1);
    eval_RegisterFunc(pContext, "sg.Owned?",            sgeval_SupergroupOwnsBadge,            1, 1);
    eval_RegisterFunc(pContext, "sg.BadgeOwned?",        sgeval_SupergroupOwnsBadge,            1, 1);
    eval_RegisterFunc(pContext, "sg.BadgeCount",        sgeval_SupergroupCountBadges,        0, 1);
    eval_RegisterFunc(pContext, "sg.TokenOwned?",        sgeval_SupergroupTokenOwned,        1, 1);
    eval_RegisterFunc(pContext, "sg.TokenVal",            sgeval_SupergroupTokenVal,            1, 1);
    eval_RegisterFunc(pContext, "sg.RecipeOwned?",        sgeval_SupergroupRecipeOwned,        1, 1);
    eval_RegisterFunc(pContext, "sg.HasPermission?",    sgeval_SupergroupHasPermission,        1, 1);
    eval_RegisterFunc(pContext, "sg.EmptyIoPMounts",    sgeval_SupergroupEmptyIoPMounts,    0, 1);
    eval_RegisterFunc(pContext, "sg.HasIoP?",            sgeval_SupergroupHasIoP,            1, 1);
}

/* End of File */
