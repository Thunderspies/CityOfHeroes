/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/timing.h>

#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/earray.h>

#include "entity/entity.h"
#include "entsend.h"       // for entSetTriggeredMove, entAddFxm, entDelFxByName
#include "graphics/camera.h"        // for camLookAt
#include "seq/seqstate.h"      // for SeqStateNames
#include "entity/powers.h"
#include "entity/netfx.h"

#include "entity/character_base.h"
#include "character_animfx.h"
#include "character_combat.h"
#include "entity/character_target.h"
#include "entity_power.h"

#include "svr/svr_base.h"
#include "comm_game.h"
#include "gameComm/Npc.h"
#include "entity/entPlayer.h"
#include "entserver.h"
#include "entity/costume.h"

#include <utilitieslib/components/StringCache.h>      // for stringToReference
#include "seq/seq.h"

/**********************************************************************func*
 * character_StanceIsSame
 *
 */
bool character_StanceIsSame(Power *pA, Power *pB)
{
    int i;
    int *piA, *piB;
    const PowerFX *fxA, *fxB;

    if(pA==pB)
    {
        return true;
    }

    if(pA==NULL || pB==NULL)
    {
        return false;
    }

    fxA = power_GetFX(pA);
    fxB = power_GetFX(pB);

    if( fxA == NULL || fxB == NULL ||
        fxA->pchActivationFX != fxB->pchActivationFX &&
        ( !fxA->pchActivationFX || !fxB->pchActivationFX ||
          stricmp(fxA->pchActivationFX, fxB->pchActivationFX)!=0 ) )
    {
        return false;
    }

    piA = fxA->piActivationBits;
    piB = fxB->piActivationBits;
    if(eaiSize(&piA)!=eaiSize(&piB))
    {
        return false;
    }

    for(i=eaiSize(&piA)-1; i>=0; i--)
    {
        if(piA[i]!=piB[i])
        {
            return false;
        }
    }

    return true;
}

/**********************************************************************func*
 * character_EnterStance
 *
 */
void character_EnterStance(Character *p, Power *ppow, bool bReplaceStance)
{
    assert(p!=NULL);

    if(!character_StanceIsSame(p->ppowStance, ppow))
    {
        // HACK: If the power we're switching to is brawl and we already
        //       have a stance active, don't change into brawl stance.
        //       This should probably be done by looking at a member of the
        //       power.fx instead, but Brawl is the only power like this.
        if(p->ppowStance!=NULL && ppow!=NULL)
        {
            if(strnicmp(ppow->ppowBase->pchName,"Brawl",5)==0)
            {
                ppow->bFirstAttackInStance = true;
                return;
            }
        }

        if(ppow!=NULL)
        {
            const PowerFX* ppowFX = power_GetFX(ppow);
            assert(ppowFX);

            // If we have no stance or if the stance is different than before,
            //    set the new anim bits and mark this as entering a new
            //    stance.

            seqOrState(p->entParent->seq->state, 1, STATE_STARTOFFENSESTANCE);

            character_SetAnimBits(p, ppowFX->piActivationBits, 0.0f, PLAYFX_NOT_ATTACHED);

            character_PlayMaintainedFX(p, p, ppowFX->pchActivationFX, 
                power_GetTint(ppow), 0.0f, PLAYFX_NOT_ATTACHED,
                PLAYFX_NO_TIMEOUT, PLAYFX_NORMAL | PLAYFX_TINT_FLAG(ppow));

            ppow->bFirstAttackInStance = true;
        }



        if(bReplaceStance)
        {
            if(p->ppowStance!=NULL)
            {
                const PowerFX* ppowStanceFX = power_GetFX(p->ppowStance);
                assert(ppowStanceFX);

                character_KillMaintainedFX(p, p, ppowStanceFX->pchActivationFX);
                // Set off deactivation FX
                character_PlayFX(p, p, ppowStanceFX->pchDeactivationFX,
                    power_GetTint(p->ppowStance), 0, 0, 
                    PLAYFX_DONTPITCHTOTARGET | PLAYFX_TINT_FLAG(p->ppowStance));
                character_SetAnimBits(p, ppowStanceFX->piDeactivationBits, 0.0f, PLAYFX_NOT_ATTACHED);
            }

            p->ppowStance = ppow;

            entity_StanceSend(p->entParent, ppow);
        }
    }
    else if(ppow!=NULL)
    {
        ppow->bFirstAttackInStance = false;
    }
}

/**********************************************************************func*
 * character_AddStickyState
 *
 */
void character_AddStickyState(Character *p, int iState)
{
    eaiPush(&p->piStickyAnimBits, iState);
}

/**********************************************************************func*
 * character_AddStickyStates
 *
 */
void character_AddStickyStates(Character *p, const int *piList)
{
    PERFINFO_AUTO_START("character_AddStickyStates",1);
    if(piList!=NULL && eaiSize(&piList)>0)
    {
        int i;
        for(i=eaiSize(&piList)-1; i>=0; i--)
        {
            eaiPush(&p->piStickyAnimBits, piList[i]);
        }
    }
    PERFINFO_AUTO_STOP();
}

/**********************************************************************func*
 * character_SetStickyStates
 *
 */
void character_SetStickyStates(Character *p)
{
    character_SetAnimBits(p, p->piStickyAnimBits, 0, 0);
}

/**********************************************************************func*
 * character_SetAnimBits
 *
 * Sets the given animation states on the parent entity of the given
 * character.
 *
 * The state setting is postponed by the number of seconds given by
 * fDelay.
 *
 */
void character_SetAnimBits(Character *p, const int *piList, float fDelay, int iNetID)
{
    int count;
    assert(p!=NULL);

    count = eaiSize(&piList);
    if(piList!=NULL && count>0)
    {
        int i;

        if(fDelay > 0.0f || iNetID>0)
        {
            U32 state[STATE_ARRAY_SIZE];
            memset(state, 0, sizeof(state));
            for(i=count-1; i>=0; i--)
            {
                // DEBUG:
                //
                // char *pchDest = p->entParent->name;
                // verbose_printf("\t%s: state %s  (in %4.2f)\n", pchDest, SeqStateNames[piList[i]], fDelay);
                SETB(state, piList[i]);
            }

            if(iNetID>0) //If move is triggered by Fx Hit.
                entSetTriggeredMove(p->entParent, state, 0.0f, iNetID);
            else if(fDelay > 0.0f)  //If move is time delayed.
                entSetTriggeredMove(p->entParent, state, fDelay*30.0f, 0);    
        }
        else //Just go through the regular channels
        {
            // set entity state bits directly
            U32* pState = p->entParent->seq->state;
            for(i=count-1; i>=0; i--) {
                SETB(pState, piList[i]);
            }
        }
    }
}

/**********************************************************************func*
 * character_PlayFX
 *
 * Plays the named effect for a character. The "owner" of the effect
 * is pSrc, the "target" is pTarget.
 *
 * The effect is postponed the number of seconds given by fDelay.
 *
 * The effect will last for fDuration seconds. If fDuration is 0, then
 * the effect lasts it's built-in time.
 *
 * Returns the effect ID (net_id) of the created effect.
 *
 */
unsigned int character_PlayFX(Character *pSrc, Character *pTarget,
    const char *pchName, ColorPair uiTint, 
    float fDelay, unsigned int iAttached, int iFlags)
{
    int iFxNetId = 0;
    int iNameRef;

    if(pSrc!=NULL && pTarget!=NULL && pchName!=NULL)
    {
        iNameRef = stringToReference(pchName);

        if( iNameRef )
        {
            NetFx nfx = {0};

            nfx.command         = CREATE_ONESHOT_FX;
            nfx.handle          = iNameRef;
            nfx.hasColorTint    = (uiTint.primary.a && !(PLAYFX_NO_TINT & iFlags))? 1 : 0;
            nfx.colorTint       = uiTint;

            nfx.pitchToTarget   = PLAYFX_PITCH_TO_TARGET(iFlags);
            nfx.bone_id         = 0;
            nfx.duration        = 0;
            nfx.radius          = 10;
            nfx.power           = 10;
            nfx.debris          = 0;
            nfx.serverTimeout   = 0;
            nfx.powerTypeflags  = (PLAYFX_POWER_TYPE(iFlags)&PLAYFX_FROM_AOE?NETFX_POWER_FROM_AOE:0)
                | (PLAYFX_POWER_TYPE(iFlags)&PLAYFX_CONTINUING?NETFX_POWER_CONTINUING:0)
                | (PLAYFX_IS_IMPORTANT(iFlags)?NETFX_POWER_IMPORTANT:0)
                | (PLAYFX_POWER_TYPE(iFlags)&PLAYFX_AT_GEO_LOC?NETFX_POWER_AT_GEO_LOC:0);

            nfx.clientTriggerFx = iAttached;
            nfx.clientTimer     = fDelay*30.0f;

            nfx.originType      = NETFX_ENTITY;
            nfx.originEnt       = 0;
            nfx.originPos[0]    = 0.0;
            nfx.originPos[1]    = 0.0;
            nfx.originPos[2]    = 0.0;

            nfx.targetType      = NETFX_ENTITY;
            nfx.targetEnt       = pTarget->entParent->owner;
            nfx.targetPos[0]    = 0.0;
            nfx.targetPos[1]    = 0.0;
            nfx.targetPos[2]    = 0.0;

            iFxNetId = entAddFx( pSrc->entParent, &nfx );
        }
        else
        {
            verbose_printf("FX %s isn't in the string table\n", pchName);
        }
    }

    return iFxNetId;
}

/**********************************************************************func*
 * character_PlayFXLocation
 *
 * Plays the named effect for a location. The "owner" of the effect
 * is pSrc, the "target" is a vecTarget.
 *
 * The effect is postponed the number of seconds given by fDelay.
 *
 * The effect will last for fDuration seconds. If fDuration is 0, then
 * the effect lasts it's built-in time.
 *
 * Returns the effect ID (net_id) of the created effect.
 *
 */
unsigned int character_PlayFXLocation(Character *pSrc, Character *pTarget,
    const Vec3 vecTarget, const char *pchName, ColorPair uiTint, 
    float fDelay, unsigned int iAttached, int iFlags)
{
    int iFxNetId = 0;
    int iNameRef;
    assert(pSrc!=NULL);

    if(pchName!=NULL)
    {
        iNameRef = stringToReference(pchName);

        if( iNameRef )
        {
            NetFx nfx = {0};

            nfx.command         = CREATE_ONESHOT_FX;
            nfx.handle          = iNameRef;
            nfx.hasColorTint    = (uiTint.primary.a && !(PLAYFX_NO_TINT & iFlags))? 1 : 0;
            nfx.colorTint        = uiTint;

            nfx.pitchToTarget   = PLAYFX_PITCH_TO_TARGET(iFlags);
            nfx.bone_id         = 0;
            nfx.duration        = 0;
            nfx.radius          = 10;
            nfx.power           = 10;
            nfx.debris          = 0;
            nfx.serverTimeout   = 0;
            nfx.powerTypeflags  = (PLAYFX_POWER_TYPE(iFlags)&PLAYFX_FROM_AOE?NETFX_POWER_FROM_AOE:0)
                | (PLAYFX_POWER_TYPE(iFlags)&PLAYFX_CONTINUING?NETFX_POWER_CONTINUING:0)
                | (PLAYFX_IS_IMPORTANT(iFlags)?NETFX_POWER_IMPORTANT:0)
                | (PLAYFX_POWER_TYPE(iFlags)&PLAYFX_AT_GEO_LOC?NETFX_POWER_AT_GEO_LOC:0);

            nfx.clientTriggerFx = iAttached;
            nfx.clientTimer     = fDelay*30.0f;

            nfx.originType      = NETFX_ENTITY;
            nfx.originEnt       = 0;
            nfx.originPos[0]    = 0.0;
            nfx.originPos[1]    = 0.0;
            nfx.originPos[2]    = 0.0;

            nfx.targetType      = NETFX_POSITION;
            nfx.targetEnt       = (pTarget && pTarget->entParent) ? pTarget->entParent->owner : 0;
            copyVec3(vecTarget, nfx.targetPos);

            iFxNetId = entAddFx( pSrc->entParent, &nfx );
        }
        else
        {
            verbose_printf("FX %s isn't in the string table\n", pchName);
        }
    }

    return iFxNetId;
}

/**********************************************************************func*
 * entity_PlayMaintainedFX
 *
 * copied to entity_PlayClientsideMaintainedFX
 */
unsigned int entity_PlayMaintainedFX(Entity *pSrc, Entity *pTarget,
    const char *pchName, ColorPair uiTint, 
    float fDelay, unsigned int iAttached, float fTimeOut, int iFlags)
{
    int iFxNetId = 0;
    int iNameRef;
    assert(pSrc!=NULL);
    assert(pTarget!=NULL);

    if(pchName!=NULL)
    {
        iNameRef = stringToReference(pchName);

        if( iNameRef )
        {
            NetFx nfx = {0};
            nfx.command         = CREATE_MAINTAINED_FX;
            nfx.handle          = iNameRef;
            nfx.hasColorTint    = (uiTint.primary.a && !(PLAYFX_NO_TINT & iFlags))? 1 : 0;
            nfx.colorTint       = uiTint;

            nfx.pitchToTarget   = PLAYFX_PITCH_TO_TARGET(iFlags);
            nfx.bone_id         = 0;
            nfx.duration        = 0;
            nfx.radius          = 10;
            nfx.power           = 10;
            nfx.debris          = 0;
            nfx.serverTimeout   = fTimeOut*30.0f;
            nfx.powerTypeflags  = (PLAYFX_POWER_TYPE(iFlags)&PLAYFX_FROM_AOE?NETFX_POWER_FROM_AOE:0)
                | (PLAYFX_POWER_TYPE(iFlags)&PLAYFX_CONTINUING?NETFX_POWER_CONTINUING:0)
                | (PLAYFX_IS_IMPORTANT(iFlags)?NETFX_POWER_IMPORTANT:0)
                | (PLAYFX_POWER_TYPE(iFlags)&PLAYFX_AT_GEO_LOC?NETFX_POWER_AT_GEO_LOC:0);

            nfx.clientTriggerFx = iAttached;
            nfx.clientTimer     = fDelay*30.0f;

            nfx.originType      = NETFX_ENTITY;
            nfx.originEnt       = 0;
            nfx.originPos[0]    = 0.0;
            nfx.originPos[1]    = 0.0;
            nfx.originPos[2]    = 0.0;

            nfx.targetType      = NETFX_ENTITY;
            nfx.targetEnt       = pTarget->owner;
            nfx.targetPos[0]    = 0.0;
            nfx.targetPos[1]    = 0.0;
            nfx.targetPos[2]    = 0.0;

            iFxNetId = entAddFx( pSrc, &nfx );
        }
        else
        {
            verbose_printf("FX %s isn't in the string table\n", pchName);
        }
    }

    return iFxNetId;
}

/**********************************************************************func*
 * character_PlayMaintainedFX
 *
 */
unsigned int character_PlayMaintainedFX(Character *pSrc, Character *pTarget,
    const char *pchName, ColorPair uiTint, 
    float fDelay, unsigned int iAttached, float fTimeOut, int iFlags)
{
    assert(pSrc!=NULL);
    assert(pTarget!=NULL);

    return entity_PlayMaintainedFX(pSrc->entParent, pTarget->entParent, 
        pchName, uiTint, 
        fDelay, iAttached, fTimeOut, iFlags);
}

/**********************************************************************func*
 * entity_KillMaintainedFX
 *
 * copied to entity_KillClientsideMaintainedFX
 */
void entity_KillMaintainedFX(Entity *pSrc, Entity *pTarget, const char *pchName)
{
    assert(pSrc!=NULL);

    if(pchName!=NULL)
    {
        entDelFxByName(pSrc, pchName);
    }
}

/**********************************************************************func*
 * character_KillMaintainedFX
 *
 */
void character_KillMaintainedFX(Character *pSrc, Character *pTarget, const char *pchName)
{
    assert(pSrc!=NULL);
    entity_KillMaintainedFX(pSrc->entParent, NULL, pchName);
}

/**********************************************************************func*
 * entity_FaceTarget
 *
 */
void entity_FaceTarget(Entity *entToTurn, Entity *entTarget)
{
    assert(entToTurn!=NULL);

    if(entTarget==NULL)
    {
        return;
    }

    if(entToTurn->do_not_face_target)
        return;

    if( ENTTYPE(entToTurn) == ENTTYPE_CRITTER )
    {
        Vec3 d;
        Vec3 dest_pyr;
        Mat3 dest_mat;
        Mat3 new_mat;

        subVec3(ENTPOS(entTarget), ENTPOS(entToTurn), d);
        d[1] = 0;
        camLookAt(d, dest_mat);
        getMat3YPR(dest_mat, dest_pyr);

        createMat3PYR(new_mat, dest_pyr);
        entSetMat3(entToTurn, new_mat);
    }
    else
    {
        ClientLink* client = entToTurn->client;

        if(client){
            client->controls.forcedTurn.erEntityToFace = erGetRef(entTarget);
            client->controls.forcedTurn.timeRemaining = 30;
        }

        // Tell the client to face the target.

        START_PACKET(pak, entToTurn, SERVER_FACE_ENTITY);
        pktSendBitsPack(pak, 3, entTarget->owner);
        END_PACKET
    }
}

/**********************************************************************func*
 * character_FaceLocation
 *
 */
void character_FaceLocation(Character *p, Vec3 vecTarget)
{
    Entity* entParent;

    assert(p!=NULL);

    entParent = p->entParent;

    if(entParent->do_not_face_target)
        return;

    if( ENTTYPE(entParent) == ENTTYPE_CRITTER )
    {
        Vec3 d;
        Vec3 dest_pyr;
        Mat3 dest_mat;
        Mat3 new_mat;

        subVec3(vecTarget, ENTPOS(p->entParent), d);
        d[1] = 0;
        camLookAt(d, dest_mat);
        getMat3YPR(dest_mat, dest_pyr);

        createMat3PYR(new_mat, dest_pyr);
        entSetMat3(p->entParent, new_mat);
    }
    else
    {
        ClientLink* client = entParent->client;

        if(client){
            client->controls.forcedTurn.erEntityToFace = 0;
            copyVec3(vecTarget, client->controls.forcedTurn.locToFace);
            client->controls.forcedTurn.timeRemaining = 30;
        }

        // Tell the client to face the target.

        START_PACKET(pak, entParent, SERVER_FACE_LOCATION);
        pktSendF32(pak, vecTarget[0]);
        pktSendF32(pak, vecTarget[1]);
        pktSendF32(pak, vecTarget[2]);
        END_PACKET
    }
}

/**********************************************************************func*
 * character_Teleport
 *
 */
void character_Teleport(Character *p, Vec3 v)
{
    assert(p!=NULL);

    if(IsTeleportTargetValid(v) && !p->entParent->doorFrozen)
    {
        entUpdatePosInstantaneous(p->entParent, v);
        p->entParent->move_instantly = 1;
    }
}

/**********************************************************************func*
 * character_SetCostume
 *
 */
void character_SetCostume(Character *p, const char *pchCostumeName, int priority)
{
    int index;
    const NPCDef *npc = npcFindByName(pchCostumeName, &index);
    const cCostume *npcCostume = npcDefsGetCostume(index, 0);
    AttribModListIter iter;
    AttribMod *pmod;

    if(npcCostume && priority > p->entParent->costumePriority &&
        (p->entParent->costume!=npcCostume || p->entParent->npcIndex!=index))
    {
        p->entParent->costume = npcCostume;
        svrChangeBody(p->entParent, entTypeFileName(p->entParent));
        p->entParent->npcIndex = index;
        p->entParent->send_costume = 1;
        p->entParent->costumePriority = priority;

        if(p->entParent->pl)
            p->entParent->pl->npc_costume = index;
            
        pmod = modlist_GetFirstMod(&iter, &p->listMod);
        while (pmod != NULL)
        {
            if (pmod->ptemplate && pmod->ptemplate->pchContinuingFX)
                mod_CancelFX(pmod, p);
            pmod = modlist_GetNextMod(&iter);
        }
    }
}

/**********************************************************************func*
 * character_RestoreCostume
 *
 */
void character_RestoreCostume(Character *p)
{
    AttribModListIter iter;
    AttribMod *pmod;

    if(p->entParent->pl)
    {
        const cCostume *pCostume = NULL;

        if(p->entParent->pl->current_costume<0
            || p->entParent->pl->current_costume>=MAX_COSTUMES)
        {
            p->entParent->pl->current_costume = 0;
            p->entParent->pl->current_powerCust = 0;
        }

        if(p->entParent->pl->supergroup_mode && p->entParent->supergroup)
            pCostume = costume_as_const(costume_makeSupergroup(p->entParent));
        else
            pCostume = costume_as_const(p->entParent->pl->costume[p->entParent->pl->current_costume]);

        if(pCostume)
        {
            p->entParent->costume = pCostume;
            svrChangeBody(p->entParent, entTypeFileName(p->entParent));
            p->entParent->npcIndex = 0;
            p->entParent->costumePriority = 0;

            p->entParent->send_costume = true;
            p->entParent->send_all_costume = true;

            if(p->entParent->pl)
                p->entParent->pl->npc_costume = 0;
                
            pmod = modlist_GetFirstMod(&iter, &p->listMod);
            while (pmod != NULL)
            {
                if (pmod->ptemplate && pmod->ptemplate->pchContinuingFX)
                    mod_CancelFX(pmod, p);
                pmod = modlist_GetNextMod(&iter);
            }
        }
    }
    else if(ENTTYPE(p->entParent) == ENTTYPE_CRITTER)
    {
        villainRevertCostume(p->entParent);
    }
}



/* End of File */
