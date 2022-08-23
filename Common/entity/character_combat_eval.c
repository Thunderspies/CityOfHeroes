/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <float.h>

#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/eval.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/mathutil.h>

#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "gameComm/VillainDef.h"

#include "entity/origins.h"
#include "entity/classes.h"
#include "entity/character_base.h"
#include "entity/character_eval.h"
#include "entity/character_target.h"
#include "entity/character_combat_eval.h"
#include "entity/powers.h"

#include <utilitieslib/assert/assert.h>

EvalContext *s_pCombatEval;

/**********************************************************************func*
 * TargetTypeMatches
 *
 */
static void TargetTypeMatches(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

#ifdef SERVER
    if(bFound && e->villainDef)
    {
        if(stricmp(e->villainDef->name, rhs)==0)
        {
            eval_IntPush(pcontext, 1);
            return;
        }
    }
#endif

    eval_IntPush(pcontext, 0);
}

/**********************************************************************func*
 * TargetFetch
 *
 */
static void TargetFetch(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    
    eval_FetchInt(pcontext, "Target", (int *)&e);
    chareval_EntityFetchHelper(pcontext, e, rhs);
}

/**********************************************************************func*
* TargetOwnerFetch
*
*/
static void TargetOwnerFetch(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    
    eval_FetchInt(pcontext, "Target", (int *)&e);
    if (e && e->erOwner)
    {
        e = erGetEnt(e->erOwner);
    }
    chareval_EntityFetchHelper(pcontext, e, rhs);
}

/**********************************************************************func*
* TargetCreatorFetch
*
*/
static void TargetCreatorFetch(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    
    eval_FetchInt(pcontext, "Target", (int *)&e);
    if (e && e->erCreator)
    {
        e = erGetEnt(e->erCreator);
    }
    chareval_EntityFetchHelper(pcontext, e, rhs);
}

/**********************************************************************func*
 * TargetHasTag
 *
 */
static void TargetHasTag(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound)
    {
        chareval_EntityHasTagHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
* TargetHasBadge
*
*/
static void TargetHasBadge(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound)
    {
        chareval_EntityOwnsBadgeHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}


/**********************************************************************func*
* TargetTokenOwned
*
*/
static void TargetTokenOwned(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound)
    {
        chareval_EntityTokenOwnedHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}


/**********************************************************************func*
* TargetTokenVal
*
*/
static void TargetTokenVal(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound)
    {
        chareval_EntityTokenValHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}


/**********************************************************************func*
* TargetLoyaltyOwned
*
*/
static void TargetLoyaltyOwned(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound)
    {
        chareval_EntityLoyaltyOwnedHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}


/**********************************************************************func*
* SourceLoyaltyOwned
*
*/
static void SourceLoyaltyOwned(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound)
    {
        chareval_EntityLoyaltyOwnedHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
* TargetLoyaltyTeirOwned
*
*/
static void TargetLoyaltyTeirOwned(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound)
    {
        chareval_EntityLoyaltyTierOwnedHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}


/**********************************************************************func*
* SourceLoyaltyTeirOwned
*
*/
static void SourceLoyaltyTeirOwned(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound)
    {
        chareval_EntityLoyaltyTierOwnedHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
* TargetLoyaltyLevelOwned
*
*/
static void TargetLoyaltyLevelOwned(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound)
    {
        chareval_EntityLoyaltyLevelOwnedHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}


/**********************************************************************func*
* SourceLoyaltyLevelOwned
*
*/
static void SourceLoyaltyLevelOwned(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound)
    {
        chareval_EntityLoyaltyLevelOwnedHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
* TargetProductOwned
*
*/
static void TargetProductOwned(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound)
    {
        chareval_EntityProductOwnedHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}


/**********************************************************************func*
* SourceProductOwned
*
*/
static void SourceProductOwned(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound)
    {
        chareval_EntityProductOwnedHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}



/**********************************************************************func*
* TargetLoyaltyPointsEarned
*
*/
static void TargetLoyaltyPointsEarned(EvalContext *pcontext)
{
    Entity *e;
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound)
    {
        chareval_EntityLoyaltyPointsEarnedHelper(pcontext, e);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}


/**********************************************************************func*
* SourceLoyaltyPointsEarned
*
*/
static void SourceLoyaltyPointsEarned(EvalContext *pcontext)
{
    Entity *e;
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound)
    {
        chareval_EntityLoyaltyPointsEarnedHelper(pcontext, e);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}


/**********************************************************************func*
* TargetIsVIP
*
*/
static void TargetIsVIP(EvalContext *pcontext)
{
    Entity *e;
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound)
    {
        chareval_EntityIsVIPHelper(pcontext, e);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}


/**********************************************************************func*
* SourceLoyaltyPointsEarned
*
*/
static void SourceIsVIP(EvalContext *pcontext)
{
    Entity *e;
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound)
    {
        chareval_EntityIsVIPHelper(pcontext, e);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}


static void SourceIsAccountServerAvailable(EvalContext *pcontext)
{
    Entity *e;
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound && e)
    {
        chareval_EntityIsAccountServerAvailableHelper(pcontext, e);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}


static void TargetIsAccountServerAvailable(EvalContext *pcontext)
{
    Entity *e;
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound && e)
    {
        chareval_EntityIsAccountServerAvailableHelper(pcontext, e);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

static void SourceIsAccountInventoryLoaded(EvalContext *pcontext)
{
    Entity *e;
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound && e)
    {
        chareval_IsAccountInventoryLoadedHelper(pcontext, e);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}


static void TargetIsAccountInventoryLoaded(EvalContext *pcontext)
{
    Entity *e;
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound && e)
    {
        chareval_IsAccountInventoryLoadedHelper(pcontext, e);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
 * TargetEventCount
 *
 */
static void TargetEventCount(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound)
    {
        chareval_EntityEventCountHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
 * SourceEventCount
 *
 */
static void SourceEventCount(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound)
    {
        chareval_EntityEventCountHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}


/**********************************************************************func*
* TargetTeamSize
*
*/
static void TargetTeamSize(EvalContext *pcontext)
{
    Entity *e;
    float fRadius = eval_FloatPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound)
    {
        chareval_TeamSizeHelper(pcontext, e, fRadius);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
* SourceTeamSize
*
*/
static void SourceTeamSize(EvalContext *pcontext)
{
    Entity *e;
    float fRadius = eval_FloatPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound)
    {
        chareval_TeamSizeHelper(pcontext, e, fRadius);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}



/**********************************************************************func*
 * TargetEventTime
 *
 */
static void TargetEventTime(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound)
    {
        chareval_EntityEventTimeHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
 * SourceEventTime
 *
 */
static void SourceEventTime(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound)
    {
        chareval_EntityEventTimeHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
 * SourceFetch
 *
 */
static void SourceFetch(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    
    eval_FetchInt(pcontext, "Source", (int *)&e);
    chareval_EntityFetchHelper(pcontext, e, rhs);
}

/**********************************************************************func*
* SourceOwnerFetch
*
*/
static void SourceOwnerFetch(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    
    eval_FetchInt(pcontext, "Source", (int *)&e);
    if (e && e->erOwner)
    {
        e = erGetEnt(e->erOwner);
    }
    chareval_EntityFetchHelper(pcontext, e, rhs);
}

/**********************************************************************func*
* SourceCreatorFetch
*
*/
static void SourceCreatorFetch(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    
    eval_FetchInt(pcontext, "Source", (int *)&e);
    if (e && e->erCreator)
    {
        e = erGetEnt(e->erCreator);
    }
    chareval_EntityFetchHelper(pcontext, e, rhs);
}

/**********************************************************************func*
* TargetRelationFetch
*
*/
static void TargetIsFriend(EvalContext *pcontext)
{
    Entity *pTarget;
    Entity *pSrc;
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&pSrc);
    bFound &= eval_FetchInt(pcontext, "Target", (int *)&pTarget);
    
    if(bFound)
    {
        if (pSrc->pchar && pTarget->pchar)
        {
            eval_IntPush(pcontext, character_TargetIsFriend(pSrc->pchar, pTarget->pchar));
        }
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
 * SourceHasTag
 *
 */
static void SourceHasTag(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound)
    {
        chareval_EntityHasTagHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
* SourceHasBadge
*
*/
static void SourceHasBadge(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound)
    {
        chareval_EntityOwnsBadgeHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
* SourceTokenOwned
*
*/
static void SourceTokenOwned(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound)
    {
        chareval_EntityTokenOwnedHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
* SourceTokenVal
*
*/
static void SourceTokenVal(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound)
    {
        chareval_EntityTokenValHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

static void TargetTokenTime(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound)
    {
        chareval_EntityTokenTimeHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

static void SourceTokenTime(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound)
    {
        chareval_EntityTokenTimeHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
 * TargetInMode
 *
 */
static void TargetInMode(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound)
    {
        chareval_EntityInModeHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
 * SourceInMode
 *
 */
static void SourceInMode(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound)
    {
        chareval_EntityInModeHelper(pcontext, e, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

void chareval_TargetIsToggleActive(EvalContext *pcontext)
{
    Entity *e;
    const char *powerFullName = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if (bFound)
    {
        chareval_EntityIsToggleActiveHelper(pcontext, e, powerFullName);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

void chareval_SourceIsToggleActive(EvalContext *pcontext)
{
    Entity *e;
    const char *powerFullName = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if (bFound)
    {
        chareval_EntityIsToggleActiveHelper(pcontext, e, powerFullName);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
* TargetMapTeamArea
*
*/
static void TargetMapTeamArea(EvalContext *pcontext)
{
    Entity *e;
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);

    if(bFound)
    {
        chareval_MapTeamAreaHelper(pcontext, e);
    }
    else
    {
        eval_StringPush(pcontext, "Unknown");
    }
}

/**********************************************************************func*
* SourceMapTeamArea
*
*/
static void SourceMapTeamArea(EvalContext *pcontext)
{
    Entity *e;
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);

    if(bFound)
    {
        chareval_MapTeamAreaHelper(pcontext, e);
    }
    else
    {
        eval_StringPush(pcontext, "Unknown");
    }
}

static void CheckAuthBit(EvalContext *pcontext)
{
    Entity *e;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);
    int i = 0;

    if(bFound && e && e->pl)
    {
        chareval_AuthFetchHelper(pcontext, e->pl->auth_user_data, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
 * Distance
 *
 */
static void Distance(EvalContext *pcontext)
{
    Entity *eTarget;
    Entity *eSrc;
    bool bFoundSrc = eval_FetchInt(pcontext, "Source", (int *)&eSrc);
    bool bFoundTarget = eval_FetchInt(pcontext, "Target", (int *)&eTarget);

    if(bFoundSrc && bFoundTarget)
    {
        float f = distance3(ENTPOS(eTarget), ENTPOS(eSrc));
        eval_FloatPush(pcontext, f);
    }
    else
    {
        eval_FloatPush(pcontext, FLT_MAX);
    }
}

void chareval_TargetOnStoryArc(EvalContext *pcontext)
{
#if SERVER
    Entity *e;
    bool bFound = eval_FetchInt(pcontext, "Target", (int *)&e);
    const char *storyArcName = eval_StringPop(pcontext);

    if(bFound)
    {
        eval_IntPush(pcontext, entity_IsOnStoryArc(e, storyArcName));
        return;
    }
#endif

    eval_IntPush(pcontext, 0);
}

void chareval_SourceOnStoryArc(EvalContext *pcontext)
{
#if SERVER
    Entity *e;
    bool bFound = eval_FetchInt(pcontext, "Source", (int *)&e);
    const char *storyArcName = eval_StringPop(pcontext);

    if(bFound)
    {
        eval_IntPush(pcontext, entity_IsOnStoryArc(e, storyArcName));
        return;
    }
#endif

    eval_IntPush(pcontext, 0);
}

/**********************************************************************func*
 * chareval_TargetOwnsPower
 *
 */
static void chareval_TargetOwnsPower(EvalContext *pcontext)
{
    chareval_OwnsPower(pcontext, "Target");
}

/**********************************************************************func*
* chareval_SourceOwnsPower
*
*/
static void chareval_SourceOwnsPower(EvalContext *pcontext)
{
    chareval_OwnsPower(pcontext, "Source");
}

/**********************************************************************func*
 * chareval_TargetOwnsPowerNum
 *
 */
static void chareval_TargetOwnsPowerNum(EvalContext *pcontext)
{
    chareval_OwnsPowerNum(pcontext, "Target");
}

/**********************************************************************func*
* chareval_SourceOwnsPower
*
*/
static void chareval_SourceOwnsPowerNum(EvalContext *pcontext)
{
    chareval_OwnsPowerNum(pcontext, "Source");
}

/**********************************************************************func*
* chareval_TargetInVolume
*
*/
static void chareval_TargetInVolume(EvalContext *pcontext)
{
    chareval_EntityIsInVolume(pcontext, "Target");
}

/**********************************************************************func*
* chareval_SourceInVolume
*
*/
static void chareval_SourceInVolume(EvalContext *pcontext)
{
    chareval_EntityIsInVolume(pcontext, "Source");
}

// if ppow is not NULL, then we want the boosted value.
static void chareval_PowerFetchHelper(EvalContext *pcontext, BasePower *ppowBase, Power *ppow, const char *rhs)
{
    static StashTable enumFromParam = NULL;
    bool valid_param;

    // simple values
    // to add a new value:
    // * add the name to PARAMLIST, which will be used in eval statements
    // * add a case to switch(param) below
    // i didn't really want to do it this way myself, but since aaron started it, i thought i'd go all the way ;)
#define PARAMLIST            \
    ELEM(rechargetime)        \
    ELEM(activateperiod)    \
    ELEM(activatetime)        \
    ELEM(endurancecost)        \
    ELEM(accuracy)            \
    ELEM(effectarea)        \
    ELEM(radius)            \
    ELEM(arc)                \
    ELEM(maxtargetshit)        \
    ELEM(areafactor)        \
    ELEM(categoryname)        \
    ELEM(powersetname)        \
    ELEM(powername)            \


    // enumeration of values
    typedef enum ParamEnum
    {
#define ELEM(X) ParamEnum_##X,
        PARAMLIST // the syntax highlighter is pretty angry about this...
        ParamEnumCount // no underscore, to avoid a name collision
#undef ELEM
    } ParamEnum;
    ParamEnum param;

    // mapping of string to enum
    if(!enumFromParam)
    {
        enumFromParam = stashTableCreateWithStringKeys(ParamEnumCount*2, StashDefault);
#define ELEM(X)    if(!stashAddInt(enumFromParam, #X, ParamEnum_##X, false)) \
    FatalErrorf("duplicate param name %s", #X);
        PARAMLIST
#undef ELEM
    }

    // early out. note that ppow isn't being checked, because we allow it to be NULL.
    if(!ppowBase || !rhs)
    {
        eval_IntPush(pcontext, 0);
        return;
    }

    STATIC_INFUNC_ASSERT(sizeof(int) == sizeof(param)); // oh the things i do for typing...
    valid_param = stashFindInt(enumFromParam, rhs, (int*)&param);

    if(valid_param)
    {
        switch(param)
        {
#define CASE(X) xcase ParamEnum_##X

            CASE(rechargetime):
                if (ppow)
                    eval_FloatPush(pcontext, ppowBase->fRechargeTime / ppow->pattrStrength->fRechargeTime);
                else
                    eval_FloatPush(pcontext, ppowBase->fRechargeTime);

            CASE(activateperiod):
                if (ppow)
                    eval_FloatPush(pcontext, ppowBase->fActivatePeriod); // can't boost this
                else
                    eval_FloatPush(pcontext, ppowBase->fActivatePeriod);

            CASE(activatetime):
                if (ppow)
                    eval_FloatPush(pcontext, ppowBase->fTimeToActivate / ppow->pattrStrength->fTimeToActivate);
                else
                    eval_FloatPush(pcontext, ppowBase->fTimeToActivate);

            CASE(endurancecost):
                if (ppow)
                    eval_FloatPush(pcontext, ppowBase->fEnduranceCost / (ppow->pattrStrength->fEnduranceDiscount + 0.0001f));
                else
                    eval_FloatPush(pcontext, ppowBase->fEnduranceCost);

            CASE(accuracy):
                if (ppow)
                    eval_FloatPush(pcontext, ppowBase->fAccuracy * ppow->pattrStrength->fAccuracy);
                else
                    eval_FloatPush(pcontext, ppowBase->fAccuracy);

            CASE(effectarea):
                if (ppowBase->eEffectArea == kEffectArea_Character)
                    eval_StringPush(pcontext, "Character");
                else if (ppowBase->eEffectArea == kEffectArea_Cone)
                    eval_StringPush(pcontext, "Cone");
                else if (ppowBase->eEffectArea == kEffectArea_Sphere)
                    eval_StringPush(pcontext, "Sphere");
                else if (ppowBase->eEffectArea == kEffectArea_Location)
                    eval_StringPush(pcontext, "Location");
                else if (ppowBase->eEffectArea == kEffectArea_Volume)
                    eval_StringPush(pcontext, "Volume");
                else if (ppowBase->eEffectArea == kEffectArea_NamedVolume)
                    eval_StringPush(pcontext, "NamedVolume");
                else if (ppowBase->eEffectArea == kEffectArea_Map)
                    eval_StringPush(pcontext, "Map");
                else if (ppowBase->eEffectArea == kEffectArea_Room)
                    eval_StringPush(pcontext, "Room");
                else if (ppowBase->eEffectArea == kEffectArea_Touch)
                    eval_StringPush(pcontext, "Touch");
                else if (ppowBase->eEffectArea == kEffectArea_Box)
                    eval_StringPush(pcontext, "Box");

            CASE(radius):
                if (ppow)
                    eval_FloatPush(pcontext, power_GetRadius(ppow));
                else
                    eval_FloatPush(pcontext, ppowBase->fRadius);

            CASE(arc):
                if (ppow)
                    eval_FloatPush(pcontext, DEG(ppowBase->fArc) * ppow->pattrStrength->fArc);
                else
                    eval_FloatPush(pcontext, DEG(ppowBase->fArc));

            CASE(maxtargetshit):
                eval_IntPush(pcontext, ppowBase->iMaxTargetsHit);

            CASE(areafactor):
                eval_FloatPush(pcontext, basepower_CalculateAreaFactor(ppowBase));

            CASE(categoryname):
                eval_StringPush(pcontext, ppowBase->psetParent->pcatParent->pchName);

            CASE(powersetname):
                eval_StringPush(pcontext, ppowBase->psetParent->pchFullName);

            CASE(powername):
                eval_StringPush(pcontext, ppowBase->pchFullName);

            xdefault:
                Errorf("parameter %s declared, but not defined", rhs); // i feel this should be an assert...
                eval_IntPush(pcontext, 0);

#undef CASE
        };
    }
    else
    {
        // Gets here when all else fails.
        eval_IntPush(pcontext, 0);
    }
}

static void combateval_BasePowerFetch(EvalContext *pcontext)
{
    BasePower *ppowBase;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "BasePower", (int *)&ppowBase);

    if(bFound)
    {
        chareval_PowerFetchHelper(pcontext, ppowBase, 0, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

static void combateval_BoostedPowerFetch(EvalContext *pcontext)
{
    BasePower *ppowBase;
    Power *ppow;
    const char *rhs = eval_StringPop(pcontext);
    bool bFound = eval_FetchInt(pcontext, "BasePower", (int *)&ppowBase);
    bFound &= eval_FetchInt(pcontext, "Power", (int *)&ppow);

    if(bFound)
    {
        chareval_PowerFetchHelper(pcontext, ppowBase, ppow, rhs);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

static void combateval_PowerAttackTypeCount(EvalContext *pcontext)
{
    BasePower *ppowBase;
    bool bFound = eval_FetchInt(pcontext, "BasePower", (int *)&ppowBase);

    if (bFound)
    {
        eval_IntPush(pcontext, eaiSize(&ppowBase->pAttackTypes));
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

/**********************************************************************func*
* Now
*
*/
static void Now(EvalContext *pcontext)
{
    eval_IntPush(pcontext, timerSecondsSince2000());
}

/**********************************************************************func*
 * combateval_Init
 *
 */
void combateval_Init(void)
{
    static bool bInitDone = false;

    if(bInitDone)
        return;

    // Create the evaluation context used by everyone.
    s_pCombatEval = eval_Create();
    s_pCombatEval->bVerifyStackSize = true;
    eval_RegisterFunc(s_pCombatEval, "target.VillainName>",    TargetTypeMatches, 1, 1);

    eval_RegisterFunc(s_pCombatEval, "target>",                TargetFetch, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source>",                SourceFetch, 1, 1);

    eval_RegisterFunc(s_pCombatEval, "target.EventTime>",    TargetEventTime, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.EventTime>",    SourceEventTime, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "target.EventCount>",    TargetEventCount, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.EventCount>",    SourceEventCount, 1, 1);

    eval_RegisterFunc(s_pCombatEval, "target.TeamSize>",    TargetTeamSize, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.TeamSize>",    SourceTeamSize, 1, 1);

    eval_RegisterFunc(s_pCombatEval, "now",                    chareval_Now, 0, 1);
    eval_RegisterFunc(s_pCombatEval, "distance",            Distance, 0, 1);

    eval_RegisterFunc(s_pCombatEval, "target.HasTag?",        TargetHasTag, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.HasTag?",        SourceHasTag, 1, 1);

    eval_RegisterFunc(s_pCombatEval, "target.Owned?",        TargetHasBadge, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.Owned?",        SourceHasBadge, 1, 1);

    eval_RegisterFunc(s_pCombatEval, "target.TokenOwned?",    TargetTokenOwned, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.TokenOwned?",    SourceTokenOwned, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "target.TokenVal>",    TargetTokenVal, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.TokenVal>",    SourceTokenVal, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "target.TokenTime>",    TargetTokenTime, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.TokenTime>",    SourceTokenTime, 1, 1);

    eval_RegisterFunc(s_pCombatEval, "target.mode?",        TargetInMode, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.mode?",        SourceInMode, 1, 1);

    eval_RegisterFunc(s_pCombatEval, "target.ToggleActive?",    chareval_TargetIsToggleActive,    1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.ToggleActive?",    chareval_SourceIsToggleActive,    1, 1);

    eval_RegisterFunc(s_pCombatEval, "mapname>",            chareval_MapName, 0, 1);
    eval_RegisterFunc(s_pCombatEval, "source.mapTeamArea>",    SourceMapTeamArea, 0, 1);
    eval_RegisterFunc(s_pCombatEval, "target.mapTeamArea>",    TargetMapTeamArea, 0, 1);
    eval_RegisterFunc(s_pCombatEval, "auth>",                CheckAuthBit, 1, 1);

    eval_RegisterFunc(s_pCombatEval, "system>",                chareval_SystemFetch, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "target.OnStoryArc?",    chareval_TargetOnStoryArc, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.OnStoryArc?",    chareval_SourceOnStoryArc, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "isPvPMap?",            chareval_IsPvPMap, 0, 1);
    eval_RegisterFunc(s_pCombatEval, "isMissionMap?",        chareval_IsMissionMap, 0, 1);
    eval_RegisterFunc(s_pCombatEval, "isArchitectMap?",        chareval_IsArchitectMap, 0, 1);
    eval_RegisterFunc(s_pCombatEval, "target.ownPower?",    chareval_TargetOwnsPower,       1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.ownPower?",    chareval_SourceOwnsPower,       1, 1);
    eval_RegisterFunc(s_pCombatEval, "target.ownPowerNum?",    chareval_TargetOwnsPowerNum,    1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.ownPowerNum?",    chareval_SourceOwnsPowerNum,    1, 1);
    eval_RegisterFunc(s_pCombatEval, "target.inVolume>",    chareval_TargetInVolume, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.inVolume>",    chareval_SourceInVolume, 1, 1);

    eval_RegisterFunc(s_pCombatEval, "target.owner>",        TargetOwnerFetch, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.owner>",        SourceOwnerFetch, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "target.creator>",        TargetCreatorFetch, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.creator>",        SourceCreatorFetch, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "target.isFriend?",    TargetIsFriend, 0, 1);

    eval_RegisterFunc(s_pCombatEval, "target.LoyaltyOwned?",    TargetLoyaltyOwned, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.LoyaltyOwned?",    SourceLoyaltyOwned, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "target.LoyaltyTierOwned?",    TargetLoyaltyTeirOwned, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.LoyaltyTierOwned?",    SourceLoyaltyTeirOwned, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "target.LoyaltyLevelOwned?",    TargetLoyaltyLevelOwned, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.LoyaltyLevelOwned?",    SourceLoyaltyLevelOwned, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "target.ProductOwned?",    TargetProductOwned, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "source.ProductOwned?",    SourceProductOwned, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "ProductAvailable?",    chareval_ProductAvailable, 1, 1);
    eval_RegisterFunc(s_pCombatEval, "target.LoyaltyPointsEarned>",    TargetLoyaltyPointsEarned, 0, 1);
    eval_RegisterFunc(s_pCombatEval, "source.LoyaltyPointsEarned>",    SourceLoyaltyPointsEarned, 0, 1);
    eval_RegisterFunc(s_pCombatEval, "target.isVIP?",    TargetIsVIP, 0, 1);
    eval_RegisterFunc(s_pCombatEval, "source.isVIP?",    SourceIsVIP, 0, 1);
    eval_RegisterFunc(s_pCombatEval, "source.isAccountServerAvailable?",    SourceIsAccountServerAvailable, 0, 1);
    eval_RegisterFunc(s_pCombatEval, "target.isAccountServerAvailable?",    TargetIsAccountServerAvailable, 0, 1);
    eval_RegisterFunc(s_pCombatEval, "source.isAccountInventoryLoaded?",    SourceIsAccountInventoryLoaded, 0, 1);
    eval_RegisterFunc(s_pCombatEval, "target.isAccountInventoryLoaded?",    TargetIsAccountInventoryLoaded, 0, 1);

    eval_RegisterFunc(s_pCombatEval, "power.base>",                combateval_BasePowerFetch,            1, 1);
    eval_RegisterFunc(s_pCombatEval, "power.boosted>",            combateval_BoostedPowerFetch,        1, 1);
    eval_RegisterFunc(s_pCombatEval, "power.attacktypecount>",    combateval_PowerAttackTypeCount,    0, 1);

    eval_RegisterFunc(s_pCombatEval, "ZoneEvent>",    chareval_ScriptFetch, 1, 1);

    bInitDone=true;
}

float combateval_Eval(Entity *eSrc, Entity *eTarget, const Power *pPow, const char **ppchExpr, const char *dataFilename)
{
    //devassertmsg(eSrc, "No Valid Source");
    //devassertmsg(eTarget, "No Valid Target");
    //devassertmsg(pPow && pPow->ppowBase, "No Valid Power");

    //if (eSrc && eTarget)
    {
        eval_SetBlame(s_pCombatEval, dataFilename);
        PERFINFO_AUTO_START("combateval_Eval", 1);

        if (eTarget)
            eval_StoreInt(s_pCombatEval, "Target", (int)eTarget);
        else
            eval_ForgetInt(s_pCombatEval, "Target");

        if (eSrc)
            eval_StoreInt(s_pCombatEval, "Source", (int)eSrc);
        else
            eval_ForgetInt(s_pCombatEval, "Source");

        if (pPow)
        {
            eval_StoreInt(s_pCombatEval, "Power", (int)pPow);
            eval_StoreInt(s_pCombatEval, "BasePower", (int)pPow->ppowBase);
        }
        else
        {
            eval_ForgetInt(s_pCombatEval, "Power");
            eval_ForgetInt(s_pCombatEval, "BasePower");
        }

        eval_ClearStack(s_pCombatEval);

        eval_Evaluate(s_pCombatEval, ppchExpr);
        PERFINFO_AUTO_STOP();

        return eval_FloatPeek(s_pCombatEval);
    }
    return 0;
}

float combateval_EvalFromBasePower(Entity *eSrc, Entity *eTarget, const BasePower *pPowBase, const char **ppchExpr, const char *dataFilename)
{
    //devassertmsg(eSrc, "No Valid Source");
    //devassertmsg(eTarget, "No Valid Target");
    //devassertmsg(pPowBase, "No Valid Power");

    //if (eSrc && eTarget)
    {
        eval_SetBlame(s_pCombatEval, dataFilename);
        PERFINFO_AUTO_START("combateval_Eval", 1);

        if (eTarget)
            eval_StoreInt(s_pCombatEval, "Target", (int)eTarget);
        else
            eval_ForgetInt(s_pCombatEval, "Target");
        
        if (eSrc)
            eval_StoreInt(s_pCombatEval, "Source", (int)eSrc);
        else
            eval_ForgetInt(s_pCombatEval, "Source");
        
        eval_StoreInt(s_pCombatEval, "Power", 0);
        
        if (pPowBase)
            eval_StoreInt(s_pCombatEval, "BasePower", (int)pPowBase);
        else
            eval_ForgetInt(s_pCombatEval, "BasePower");
        
        eval_ClearStack(s_pCombatEval);

        eval_Evaluate(s_pCombatEval, ppchExpr);
        PERFINFO_AUTO_STOP();

        return eval_FloatPeek(s_pCombatEval);
    }
    return 0;
}

/**********************************************************************func*
 * combateval_StoreCustomFXToken
 *
 */
void combateval_StoreCustomFXToken(const char *token)
{
    eval_StoreString(s_pCombatEval, "CustomFX", token);
}

/**********************************************************************func*
 * combateval_StoreTargetsHit
 *
 */
void combateval_StoreTargetsHit(int targetsHit)
{
    eval_StoreInt(s_pCombatEval, "TargetsHit", targetsHit);
}

/**********************************************************************func*
 * combateval_StoreToHitInfo
 *
 */
void combateval_StoreToHitInfo(float fRand, float fToHit, bool bAlwaysHit, bool bForceHit)
{
    eval_StoreFloat(s_pCombatEval, "ToHitRoll", fRand);
    eval_StoreFloat(s_pCombatEval, "ToHit", fToHit);
    eval_StoreInt(s_pCombatEval, "AlwaysHit", bAlwaysHit);
    eval_StoreInt(s_pCombatEval, "ForceHit", bForceHit);
}

/**********************************************************************func*
 * combateval_StoreAttribCalcInfo
 *
 */
void combateval_StoreAttribCalcInfo(float fFinal, float fVal, float fScale, float fEffectiveness, float fStrength)
{
    eval_StoreFloat(s_pCombatEval, "StdResult", fFinal);
    eval_StoreFloat(s_pCombatEval, "Value", fVal);
    eval_StoreFloat(s_pCombatEval, "Scale", fScale);
    eval_StoreFloat(s_pCombatEval, "Effectiveness", fEffectiveness);
    eval_StoreFloat(s_pCombatEval, "Strength", fStrength);
}

void combateval_StoreChance(float fChanceFinal, float fChanceMods)
{
    eval_StoreFloat(s_pCombatEval, "Chance", fChanceFinal);
    eval_StoreFloat(s_pCombatEval, "ChanceMods", fChanceMods);
}

/**********************************************************************func*
 * combateval_Validate
 *
 */
void combateval_Validate(const char *pchInfo, const char **ppchExpr, const char *dataFilename)
{
#if SERVER
    // only validate on the map, no reason to duplicate work
    combateval_Eval(NULL, NULL, NULL, ppchExpr, dataFilename);
#endif
}

/* End of File */
