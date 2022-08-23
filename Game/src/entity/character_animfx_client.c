/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>

#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/earray.h>

#include "entity/entity.h"
#include "seq/seqstate.h"      // for SeqStateNames

#include "entity/character_base.h"
#include "entity/character_animfx_client.h"
#include "seq/seq.h"
#include "entity/netfx.h"
#include "entity/entrecv.h"
#include "cmdparse/cmdgame.h"
#include "demo.h"
#include "entity/entclient.h"
#include "entity/entrecv.h"
#include <utilitieslib/components/StringCache.h>
#include "graphics/FX/fx.h"

/**********************************************************************func*
 * character_SetAnimBits
 *
 * Sets the given animation states on the parent entity of the given
 * character.
 *
 */
void character_SetAnimClientBits(Character *p, int *piList)
{
    int i;
    U32 state[STATE_ARRAY_SIZE];

    assert(p!=NULL);

    memset(state, 0, sizeof(state));

    if(piList!=NULL)
    {
        for(i=eaiSize(&piList)-1; i>=0; i--)
        {
            // DEBUG:
            //
            // char *pchDest = p->entParent->name;
            // verbose_printf("\t%s: state %s  (in %4.2f)\n", pchDest, SeqStateNames[piList[i]], fDelay);

            seqSetState(state, 1, piList[i]);
        }
    }

    seqSetOutputs(p->entParent->seq->state, state);

}

//Special Version so I can do Martin's crazy delay thing
void character_SetAnimClientStanceBits(Character *p, int *piList)
{
    int i;
    U32 state[STATE_ARRAY_SIZE];

    assert(p!=NULL);

    memset(state, 0, sizeof(state));

    if(piList!=NULL)
    {
        for(i=eaiSize(&piList)-1; i>=0; i--)
        {
            seqSetState(state, 1, piList[i]);
        }
    }
    seqSetOutputs( p->entParent->seq->stance_state, state );
}

/**********************************************************************func*
* entity_PlayClientsideMaintainedFX
*
* copied from entity_PlayMaintainedFX and entReceiveNetFx
* Do NOT play multiple copies of the same FX on the same Entity at the same time!
* The current system isn't engineered to keep track of the two separately,
* which means that killing the FX will pseudorandomly kill one of the FX,
* with no way to specify which of the FX you really want.
*/
unsigned int entity_PlayClientsideMaintainedFX(Entity *pSrc, Entity *pTarget,
                                     char *pchName, ColorPair uiTint, 
                                     float fDelay, unsigned int iAttached, float fTimeOut, int iFlags)
{
    static int netfx_client_id_counter = 0; // should never be zero, positive numbers are for server-side FX
    int iFxNetId = 0;
    int iNameRef;
    assert(pSrc!=NULL);
    assert(pTarget!=NULL);

    if(pchName!=NULL)
    {
        iNameRef = stringToReference(pchName);

        if( iNameRef )
        {
            NetFx *netfx           = entrecvAllocNetFx(pSrc);
             netfx->command         = CREATE_MAINTAINED_FX;
            
            netfx_client_id_counter--;
            if (netfx_client_id_counter >= 0)
                netfx_client_id_counter = -1;
            netfx->net_id          = netfx_client_id_counter;
            
            netfx->pitchToTarget   = PLAYFX_PITCH_TO_TARGET(iFlags);
            netfx->powerTypeflags  = (PLAYFX_POWER_TYPE(iFlags)&PLAYFX_FROM_AOE?NETFX_POWER_FROM_AOE:0)
                | (PLAYFX_POWER_TYPE(iFlags)&PLAYFX_CONTINUING?NETFX_POWER_CONTINUING:0)
                | (PLAYFX_IS_IMPORTANT(iFlags)?NETFX_POWER_IMPORTANT:0)
                | (PLAYFX_POWER_TYPE(iFlags)&PLAYFX_AT_GEO_LOC?NETFX_POWER_AT_GEO_LOC:0);
            netfx->handle          = iNameRef;
            netfx->hasColorTint    = (uiTint.primary.a && !(PLAYFX_NO_TINT & iFlags))? 1 : 0;
            netfx->colorTint       = uiTint;
             netfx->clientTimer     = fDelay*30.0f;
             netfx->clientTriggerFx = iAttached;
            netfx->duration        = 0;
            netfx->radius          = 10;
            netfx->power           = 10;
            netfx->debris          = 0;
            
             netfx->originType      = NETFX_ENTITY;
             netfx->originEnt       = 0;
             netfx->originPos[0]    = 0.0;
             netfx->originPos[1]    = 0.0;
             netfx->originPos[2]    = 0.0;
            netfx->bone_id         = 0;

             netfx->targetType      = NETFX_ENTITY;
             netfx->targetEnt       = pTarget->svr_idx;
             netfx->targetPos[0]    = 0.0;
             netfx->targetPos[1]    = 0.0;
            netfx->targetPos[2]    = 0.0;

            //Demoplayer
            if(demo_recording)
            {
                demoSetAbsTimeOffset((netfx->clientTimer * ABSTIME_SECOND)/30);//last_packet_abs_time + (netfx->clientTimer * ABSTIME_SECOND)/30 - demoGetAbsTime());
                demoRecordFx(netfx);
                demoSetAbsTimeOffset(0);
            }
            //End demoplayer

            //Debug
            if( game_state.netfxdebug && pSrc == SELECTED_ENT  )
            {
                debugShowNetfx( pSrc, netfx );
            }
            //End Debug
        }
        else
        {
            verbose_printf("FX %s isn't in the string table\n", pchName);
        }
    }

    return iFxNetId;
}

void freeFxTracker( ClientNetFxTracker * fxtracker );

/**********************************************************************func*
* entity_KillClientsideMaintainedFX
*
* copied from entity_PlayClientsideMaintainedFX and entManageEntityNetFx
* Do NOT play multiple copies of the same FX on the same Entity at the same time!
* The current system isn't engineered to keep track of the two separately,
* which means that killing the FX will pseudorandomly kill one of the FX,
* with no way to specify which of the FX you really want.
*/
void entity_KillClientsideMaintainedFX(Entity *pSrc, Entity *pTarget, char *pchName)
{
    int iFxNetId = 0;
    int iNameRef, j;
    ClientNetFxTracker * fxtracker = 0;

    assert(pSrc!=NULL);
    assert(pTarget!=NULL);

    if(pchName!=NULL)
    {
        iNameRef = stringToReference(pchName);

        if( iNameRef )
        {
            for( j = 0 ; j < pSrc->maxTrackerIdxPlusOne ; j++ )
            {
                fxtracker = &(pSrc->fxtrackers[j]);

                //TO DO : what if the tracker is 
                if( fxtracker->netfx.handle == iNameRef )
                {
                    //(not guaranteed there even is an fx yet, it could have come this frame,
                    //or still be in the timing queue, but this fxDelete covers us)
                    fxDelete(  fxtracker->fxid, SOFT_KILL );
                    freeFxTracker( fxtracker );
                    break;
                }
            }
        }
        else
        {
            verbose_printf("FX %s isn't in the string table\n", pchName);
        }
    }
}


/* End of File */
