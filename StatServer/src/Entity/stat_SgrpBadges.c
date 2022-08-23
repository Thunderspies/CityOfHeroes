/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "stat_SgrpBadges.h"
#include "statdb.h"
#include "entity/entity_enum.h"
#include "container/containerSupergroup.h"
#include "entity/teamup.h"
#include <utilitieslib/components/EString.h>
#include "entity/SgrpBadges.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/eval.h>
#include "entity/Supergroup.h"
#include "entity/SgrpStats.h"
#include "player/BadgeStats.h"
#include <utilitieslib/components/bitfield.h>
#include "dbcomm/logcomm.h"
#include "player/badges_load.h"
#include "player/badges.h"
#include "dbcomm/dbcontainer.h"
#include "comm_backend.h"
#include "entity/Supergroup.h"
#include "dbcomm/dbcomm.h"
#include "entity/supergroup_eval.h"
#include "statserver.h"
#include <utilitieslib/utils/log.h>

/**********************************************************************func*
 * Percent
 *
 */
static void BadgeMillionPercent(EvalContext *pcontext)
{
    float rhs = (float)eval_IntPop(pcontext);
    float lhs = (float)eval_IntPop(pcontext);

    eval_IntPush(pcontext, ((lhs*1000000.0f)/rhs));
}

static void StatFetch(EvalContext *pcontext)
{
    //@teating-remove ab: why is this crashing?
    /*
    Supergroup *sg = NULL;
    bool bFound = eval_FetchInt(pcontext, "Supergroup", (int *)&sg);
    char *rhs = eval_StringPop(pcontext);


    if(bFound && sg)
    {
        eval_IntPush(pcontext, sgrp_BadgeStatGet(sg, rhs));
        return;
    }
*/
//    int foo[100] = {0};
    const char *rhs = eval_StringPop(pcontext);
    Supergroup *sg = NULL;
    bool bFound = eval_FetchPointer(pcontext, "Supergroup", &sg);

    if(bFound && sg)
    {
        //int stat = sgrp_BadgeStatGet(sg, rhs);
        eval_IntPush(pcontext, sgrp_BadgeStatGet(sg, rhs));
        //eval_IntPush(pcontext, 0);
        return;
    }

    eval_IntPush(pcontext, 0);
}


/**********************************************************************func*
 * badge_EvalInit
 *
 * Note: keep in sync with badges_load.c:LoadBadgeStatUsage.
 */
EvalContext *sgrpbadges_EvalInit(void)
{
    // Create the evaluation context used by everyone.

    EvalContext *res = eval_Create();

    // Add in the standard Supergroup eval functions
    sgeval_AddDefaultFuncs(res);

    // Add in the ones specific to SG badges
    eval_ReRegisterFunc(res, "%",            BadgeMillionPercent,     1, 1);
    eval_RegisterFunc(res, "int>",           StatFetch,   1, 1);

    return res;
}


//------------------------------------------------------------
//  revoke a given badge. also marks the badges affected by badges.
//----------------------------------------------------------
static bool sgrp_badgestates_Revoke(int idSgrp, SgrpBadges *bs, const BadgeDef *pdef, StashTable idxBadgesFromName )
{
    if(verify(bs && idxBadgesFromName && pdef && pdef->iIdx>0))
    {
        dbLog("SgrpBadges:Revoke", NULL, "sgrp %d, revoke badge %s", idSgrp, pdef->pchName );
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, __FUNCTION__ ": revoke %s", pdef->pchName);

        MarkModifiedBadges(idxBadgesFromName, bs->eaiStates, "*sgbadge");
        BitFieldSet(bs->abitsOwned,ARRAY_SIZE( bs->abitsOwned ), pdef->iIdx, 0);
        bs->eaiStates[pdef->iIdx] = BADGE_CHANGED_MASK;
        return true;
    }
    else
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_IMPORTANT, 0, __FUNCTION__ ": couldn't revoke badge for sgrp %d,bs(%s),def(%s),hash idxBadgesFromName()", idSgrp, bs ? "valid":"NULL", pdef ? pdef->pchName: "NULL",idxBadgesFromName?"valid":"NULL");
    }

    return false;
}


//------------------------------------------------------------
//  sometimes you can't modify badges
//----------------------------------------------------------
static bool s_lockBadgeMods = false;

static void sgrpbadges_Tick(Supergroup *sg, int idSgrp )
{
    static EvalContext *s_pSgrpBadgeEval = NULL;
    int n;
    int i;
    const BadgeDef **ppBadgesToAward = NULL;
    const BadgeDef **ppBadgesToRevoke = NULL;
    const BadgeDef **ppBadgesToSetUpdated = NULL;
    const BadgeDef **ppBadgesToSetChanged = NULL;
    SgrpBadges *bs = NULL;
    BadgeStatHashes *pHashes = &g_BadgeStatsSgroup;
    const BadgeDef **ppBadges = g_SgroupBadges.ppBadges;

    if( !s_pSgrpBadgeEval )
    {
        s_pSgrpBadgeEval = sgrpbadges_EvalInit();
    }

    if( !(verify(sg) && pHashes && ppBadges && s_pSgrpBadgeEval))
    {
        if( !sg )
        {
            log_printf(LOGID, __FUNCTION__ ":NULL supergroup for id %d ", idSgrp);
        }
        return;
    }

    n = eaSize(&ppBadges);
    bs = &sg->badgeStates;

    if(n > BADGE_SG_MAX_BADGES)
    {
        ErrorFilenamef("defs/supergroup_badges.def", "Too many badges defined!");
        n = BADGE_SG_MAX_BADGES;
    }

    // gather the badges to act upon
    for(i=0; i<n; i++)
    {
        int iBadgeIdx = ppBadges[i]->iIdx;

        if(iBadgeIdx>0)
        {
            int iBadge = bs->eaiStates[iBadgeIdx];

            if(!BADGE_IS_UPDATED(iBadge))
            {
                if(BADGE_IS_OWNED(iBadge))
                {
                    eval_StorePointer(s_pSgrpBadgeEval, "Supergroup", sg);
                    eval_ClearStack(s_pSgrpBadgeEval);

                    if( ppBadges[i]->ppchRevoke
                       && eval_Evaluate(s_pSgrpBadgeEval, ppBadges[i]->ppchRevoke ))
                    {
                        eaPushConst(&ppBadgesToRevoke, ppBadges[i]);
                    }
                }

                iBadge = bs->eaiStates[iBadgeIdx];

                if(!BADGE_IS_OWNED(iBadge))
                {
                    eval_StorePointer(s_pSgrpBadgeEval, "Supergroup", sg);
                    eval_ClearStack(s_pSgrpBadgeEval);

                    if(sgrpbadges_IsOwned(bs, iBadgeIdx))
                    {
                        // badge is already owned, but out of sync with state
                        bs->eaiStates[iBadgeIdx] = BADGE_FULL_OWNERSHIP | BADGE_CHANGED_MASK;
                        eaPushConst(&ppBadgesToSetChanged, ppBadges[i]);
                    }
                    else if(eval_Evaluate(s_pSgrpBadgeEval, ppBadges[i]->ppchRequires))
                    {
                        bs->eaiStates[iBadgeIdx] = BADGE_FULL_OWNERSHIP | BADGE_CHANGED_MASK;
                        eaPushConst(&ppBadgesToAward, ppBadges[i]);
                        eaPushConst(&ppBadgesToSetChanged, ppBadges[i]);
                    }
                    else
                    {
                        if(eval_Evaluate(s_pSgrpBadgeEval, ppBadges[i]->ppchVisible))
                        {
                            BADGE_SET_VISIBLE(iBadge);

                            if(eval_Evaluate(s_pSgrpBadgeEval, ppBadges[i]->ppchKnown))
                            {
                                BADGE_SET_KNOWN(iBadge);
                            }

                            eval_Evaluate(s_pSgrpBadgeEval, ppBadges[i]->ppchMeter);
                            BADGE_CLEAR_COMPLETION(iBadge);

                            if(!eval_StackIsEmpty(s_pSgrpBadgeEval))
                            {
                                int val = eval_IntPop(s_pSgrpBadgeEval);
                                if(val>0)
                                {
                                    if(val>1000000)
                                        val=1000000;

                                    BADGE_SET_COMPLETION(iBadge, val);
                                }
                            }
                        }

                        if(bs->eaiStates[iBadgeIdx]!=iBadge)
                        {
                            bs->eaiStates[iBadgeIdx]=iBadge;
                            //BADGE_SET_CHANGED(bs->aiStates[iBadgeIdx]);
                            eaPushConst(&ppBadgesToSetChanged, ppBadges[i]);
                        }
                    }
                } // end if !owned

                //BADGE_SET_UPDATED(bs->aiStates[iBadgeIdx]);
                eaPushConst(&ppBadgesToSetUpdated, ppBadges[i] );

            } // end if not updated yet
        } // end if valid badge
    } // end for each badge

    // --------------------
    // if we have badges to award, lock the group

    if( eaSize(&ppBadgesToAward)
            + eaSize(&ppBadgesToRevoke) )
    {
        bool locked;
        int i;

        // load the supergroup (routes around to statdb.c:stat_dealWithContainer)
        // ab: I could try using the supergroup lock and load
        s_lockBadgeMods = true;
        locked = dbSyncContainerRequest(CONTAINER_SUPERGROUPS, idSgrp, CONTAINER_CMD_LOCK_AND_LOAD, 0 );

        if(verify(locked))
        {
            // --------------------
            // revoke

            for( i = eaSize( &ppBadgesToRevoke ) - 1; i >= 0; --i)
            {
                const BadgeDef *pdef = ppBadgesToRevoke[i];
                sgrp_badgestates_Revoke( idSgrp, bs, pdef, g_BadgeStatsSgroup.aIdxBadgesFromName );
            }

            // --------------------
            // award
            // NOTE: keep in sync with entity_AddSupergroupBadge

            for( i = eaSize( &ppBadgesToAward ) - 1; i >= 0; --i)
            {
                const BadgeDef *pdef = ppBadgesToAward[i];
                if( verify( pdef && pdef->iIdx > 0 ))
                {
                    // if the badge isn't owned.
                    if(verify(!sgrpbadges_IsOwned(bs, pdef->iIdx)))
                    {
                        // --------------------
                        //  mark any badges that are affected by badges

                        MarkModifiedBadges( g_BadgeStatsSgroup.aIdxBadgesFromName, bs->eaiStates,
                                            "*sgbadge");

                        // --------------------
                        // give ownership

                        sgrpbadges_SetOwned(bs, pdef->iIdx, 1);
                        dbLog("SgrpBadges:Reward", NULL, "sgrp %d, received badge %s", idSgrp, pdef->pchName );
                        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, __FUNCTION__ ": sgrp %d received %s", idSgrp, pdef->pchName);
                    }
                    else
                    {
                        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, __FUNCTION__ ": badge %s, idx %d already owned for sgrp %s, %d", pdef->pchName, pdef->iIdx, sg->name, idSgrp);
                    }

                }
                else
                {
                    LOG( LOG_STATSERVER, LOG_LEVEL_IMPORTANT, 0, __FUNCTION__ ":def at index %d is invalid: def(%s) idx %d",i, pdef?pdef->pchName:"NULL", pdef?pdef->iIdx:0);
                }

            }

            // send the updated ent back to the db server.
            {
                char *container_str;
                container_str = packageSuperGroup(sg);
                if(!verify(dbSyncContainerUpdate(CONTAINER_SUPERGROUPS,idSgrp,CONTAINER_CMD_UNLOCK,container_str)))
                {
                    LOG( LOG_STATSERVER, LOG_LEVEL_IMPORTANT, 0, __FUNCTION__ ":unlock/update failed for %s, id %d. str is '%s'",sg->name,idSgrp,container_str);
                }
                
                free(container_str);
                s_lockBadgeMods = false;
            }
        }
        else
        {
            LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, __FUNCTION__ ":lock for sgrp %s, id %d failed", sg->name, idSgrp);
        }


        // --------------------
        // floaters for the users

        for(i = eaSize( &ppBadgesToAward ) - 1; i >= 0; --i)
        {
             char *msg = NULL;
            estrCreate(&msg);
            estrPrintf(&msg, "sgrp_badgeaawardnotify_relay %s", ppBadgesToAward[i]->pchName);
            stat_sendToAllSgrpMembers( idSgrp, msg );
            estrDestroy(&msg);
        }

        // ab: no notify when you lose a badge
//         for(i = eaSize( &ppBadgesToRevoke ) - 1; i >= 0; --i)
//         {
//              char *msg = NULL;
//             estrCreate(&msg);
//             estrPrintf(&msg, "sgrp_badgeaawardnotify_relay 0 %s", ppBadgesToRevoke[i]->pchName, kFloaterStyle_Attention);
//             stat_sendToAllSgrpMembers( idSgrp, msg );
//             estrDestroy(&msg);
//         }

    }

    // --------------------
    // these only affect badge state, which is only tracked on the
    // statserver

    if( eaSize(&ppBadgesToSetChanged)
            + eaSize(&ppBadgesToSetUpdated) > 0)
    {
        // --------------------
        // for badges that have changed
        // send to the members

        if( eaSize( &ppBadgesToSetChanged ) > 0 )
        {
            int n = eaSize( &ppBadgesToSetChanged );
            char *cmd = NULL;
            estrCreate(&cmd);

            // --------------------
            // build the cmd
            // NOTE: see badgestates_estrCmdFromStates if you change how this works
            estrPrintf(&cmd, "sgrpBadgeStates %d \"", n);
            for( i = 0; i < n; ++i)
            {
                const BadgeDef *pdef = ppBadgesToSetChanged[i];
                int state = bs->eaiStates[pdef->iIdx];

                BADGE_SET_CHANGED(state);
                estrConcatf(&cmd, "%d,%d;", pdef->iIdx, state);
            }
            estrConcatChar(&cmd, '\"');

            // --------------------
            // send it

            stat_sendToAllSgrpMembers( idSgrp, cmd );

            // finally
            estrDestroy(&cmd);
        }

        // --------------------
        // set the badges that are updated

        for( i = eaSize( &ppBadgesToSetUpdated ) - 1; i >= 0; --i)
        {
            const BadgeDef *pdef = ppBadgesToSetUpdated[i];

            // finally, set as updated
            BADGE_SET_UPDATED(bs->eaiStates[pdef->iIdx]);
        }
    }


    // --------------------
    // finally

    eaDestroyConst( &ppBadgesToRevoke );
    eaDestroyConst( &ppBadgesToAward );
    eaDestroyConst( &ppBadgesToSetChanged );
    eaDestroyConst( &ppBadgesToSetUpdated );
}

//------------------------------------------------------------
//  Update badges
//----------------------------------------------------------
void sgrpbadges_TickOverHash(StashTable sgrpFromId)
{
    if( verify( sgrpFromId ))
    {
        StashTableIterator iSgrpIds = {0};
        StashElement elem;

        for(stashGetIterator( sgrpFromId, &iSgrpIds ); (stashGetNextElement(&iSgrpIds,&elem));)
        {
            int idSgrp = stashElementGetIntKey(elem);
            Supergroup *sg = stashElementGetPointer(elem);
            sgrpbadges_Tick( sg, idSgrp );
        }
    }
    else
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, __FUNCTION__ ": NULL parameter to sgrpbadges_TickOverHash");
    }
}

//------------------------------------------------------------
//  mark modified badges, if allowed.
//----------------------------------------------------------
void supergroup_MarkModifiedSgrpBadges(Supergroup *sg, char *category)
{
    if( verify( !sgrpbadges_Locked() && sg && category ))
    {
        MarkModifiedBadges( g_BadgeStatsSgroup.aIdxBadgesFromName, sg->badgeStates.eaiStates, category );
    }
    else
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, __FUNCTION__ ":sgrp badges failed: locked(%d),sg(%s),category(%s)", sgrpbadges_Locked(), sg?sg->name:"NULL", category?category:"NULL");

    }
}

bool sgrpbadges_Locked()
{
    return s_lockBadgeMods;
}

//----------------------------------------
//  helper for creating a command to send the badge states to a
// specific entity (that just logged on for example)
//----------------------------------------
char *badgestates_estrCmdFromStates(U32 *eaiStates)
{
    char *cmd = NULL;
    const BadgeDef **ppBadges = g_SgroupBadges.ppBadges;
    int n = eaSize(&ppBadges);
    int i;

    estrCreate(&cmd);

    // --------------------
    // build the cmd

    estrPrintf(&cmd, "sgrpBadgeStates %d \"", n);
    for( i = 0; i < n; ++i)
    {
        const BadgeDef *pdef = ppBadges[i];
        if( pdef )
        {
            int state = eaiStates[pdef->iIdx];
            if( state )
            {
                BADGE_SET_CHANGED(state); // this forces an update on the game side
                estrConcatf(&cmd, "%d,%d;", pdef->iIdx, state);
            }
        }
    }
    estrConcatChar(&cmd, '\"');

    return cmd;
}
