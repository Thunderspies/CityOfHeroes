/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "entity/entity.h"
#include "graphics/camera.h"
#include "cmdparse/cmdgame.h"
#include "graphics/FX/fx.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/SuperAssert.h>
#include "entity/entclient.h"
#include <utilitieslib/utils/error.h>
#include "entity/motion.h"
#include "player/player.h"
#include "graphics/font.h"
#include "graphics/gfxwindow.h"
#include "render/rendertricks.h"
#include "graphics/light.h"
#include <utilitieslib/utils/utils.h>
#include "varutils.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/HashFunctions.h>
#include "entity/entVarUpdate.h"
#include "gameComm/trayClient.h"
#include "UI/uiGame.h"
#include "seq/seq.h"
#include "seq/seqsequence.h"
#include "graphics/seqgraphics.h"
#include "seq/seqanimate.h"
#include "gameComm/initCommon.h"
#include "entity/costume_client.h"
#include "gameData/costume_data.h"
#include "entity/entStrings.h"
#include "UI/uiWindows.h"
#include "group/groupdyn.h"
#include "gameComm/wdwbase.h"
#include "entity/character_base.h"
#include <utilitieslib/utils/timing.h>
#include "entity/PowerInfo.h"
#include "entity/attrib_net.h"
#include "UI/uiDock.h"
#include "UI/uiChat.h"
#include "UI/uiTarget.h"
#include "entity/character_net.h"
#include "gameComm/initClient.h"
#include "player/playerSticky.h"
#include "gridcoll/gridcoll.h"
#include <utilitieslib/components/StringCache.h>
#include "UI/uiGame.h"
#include "UI/uiCursor.h"
#include "demo.h"
#include "UI/uiUtilGame.h"
#include "entity/entrecv.h"
#include "render/model_cache.h"
#include "render/rendershadow.h"
#include "render/renderbonedmodel.h"
#include "render/rendertree.h"
#include <utilitieslib/components/earray.h>
#include "player/pmotion.h"
#include "group/grouputil.h"
#include "graphics/clothnode.h"
#include "graphics/FX/fxcapes.h"
#include "seq/gfxtree.h"
#include "seq/seqregistration.h"
#include "gameComm/itemselect.h"
#include "entity/netfx.h"
#include "entity/entDebug.h"
#include "render/tex.h"
#include "seq/tricks.h"
#include "seq/seqstate.h"
#include "entity/gametypes.h"
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/file.h>
#include "graphics/vistray.h"
#include "graphics/zOcclusion.h"
#include <utilitieslib/utils/strings_opt.h>
#include "NovodeX/NwRagdoll.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/mailbox.h>
#include "fxinfo.h"
#include "graphics/groupdraw.h"
#include "gameData/costume_critter.h"
#include "entity/motion.h"
#include "graphics/sun.h"
#include "entity/entPlayer.h"
#include "entity/teamup.h"
#include <utilitieslib/utils/Quat.h>
#include "edit/edit_cmd.h"
#include "entity/power_customization.h"
#include "graphics/gfx.h"
#include "render/thread/rt_shadowmap.h"    // for kShadowCapsuleCull

#ifndef TEST_CLIENT
#include "edit/edit_cmd.h"
#endif

#define DEATH_SHADOW_FADE_RATE 0.01
#define DO_ERR 0  //Turn off the error messages till I can get around to fixing them

extern int globMapLoadedLastTick;
static F32 g_playerMoveRateThisFrame;

#ifdef TEST_CLIENT
//////////////////////////////////////////////////////////////////////////
/// Begin hack for test client
#pragma warning (disable:4002)
#pragma warning (disable:4700)
#define seqClearState()
#define fxUpdateFxStateFromParentSeqs()
#define xyprintf()
#define xyprintfcolor()
#define printLod()
#define printScreenSeq()
#define entMotionUpdateCollGrid()
#define seqSetStaticLight() 0
#define gfxTreeFindBoneInAnimation() 0
#define animCheckForLoadingObjects() 0
#define animPlayInst()
#define lightEnt()
#define gfxNodeSetAlpha()
#define seqManageSeqTriggeredFx()
#define animSetHeader()
#define seqProcessClientInst() 0
#define seqProcessInst() 0
#define seqCheckMoveTriggers() -1
#define seqSetMoveTrigger()
#define seqSynchCycles()
#define stateParse()
#define seqSetLod()
#define seqManageVolumeFx()
#define shell_menu() 0
#define fxNotifyMailboxOnEvent()
#define fxCreate() 0
#define fxExtendDuration()
#define fxInitFxParams(x) ((x)->keycount=0)
#define fxIsFxAlive() 0
#define fxDelete()
#define collGrid() 0
#define modelFind() (Model*)1
#define playerSetFollowMode()
#define gfxTreeDelete()
#define updateASplat()
#define shadowStartScene()
#define gfxTreeFindWorldSpaceMat()
#define initSplatNode() 0
#define gfxTreeNodeIsValid() 0
#define seqReinitSeq()
#define seqPrepForReinit()
#define seqListAllSetStates(a,b,c) a
#define isThisDebugEnt() 0
#define uiReset()
#define positionCapsule()
#define seqFindGfxNodeGivenBoneNum(...) NULL
#define adjustLimbs(...)
/// End hack for test client
//////////////////////////////////////////////////////////////////////////
#endif







//This is the most an animation move the hips before it risks throwing off the
//animation/visibility code, and requires an extra large visibility check
#define MAX_OK_HIP_DISPLACEMENT 20.0 //TO DO: shrink and add vissdist to cars
#define ENT_FADE_IN_RATE 7.0    //subtract this many of 256 every TIMESTEP
#define ENT_FADE_OUT_RATE 3.0
#define MAX_ENT_AMBIENT 0.2
#define ENT_XLUCENCY_RATE (7.0f/255.0f)

#define FXT_NOT_IN_USE                0
#define FXT_RUNNING_MAINTAINED_FX    1
#define FXT_WAITING_TO_FIRE            2

extern void playerVarFree(Entity *e);
extern Mailbox globalEntityFxMailbox;

Entity*        entities[MAX_ENTITIES_PRIVATE];
Entity*        ent_xlat[MAX_ENTITIES_PRIVATE];
U8            entity_state[MAX_ENTITIES_PRIVATE];
EntityInfo    entinfo[MAX_ENTITIES_PRIVATE];
int            entities_max;

static bool    s_entity_do_shadowmap_culling = false;

Entity * entFindEntAtThisPosition(Vec3 pos, F32 tolerance)
{
    int i;

    for(i=1;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE )
        {
            if( nearSameVec3Tol(pos , ENTPOS(entities[i]), tolerance ) )
                return entities[i];
        }
    }
    return 0;
}


/*#####################################################################################
Manage Entity's Fx*/

//fetch a fxtracker
//TO DO use current used count instead of fxtrackersAllocedCount
static ClientNetFxTracker * getAFreeFxTracker( Entity * e )
{
    ClientNetFxTracker * fxtracker = 0;
    int j;

    //Prefer an fx tracker in the list
    for( j = 0 ; j < e->fxtrackersAllocedCount ; j++ )
    {
        if( e->fxtrackers[j].state == FXT_NOT_IN_USE ) //if slot is not in use
        {
            fxtracker = &e->fxtrackers[j];
        }
    }

    //or alloc new ones, just using realloc till I straighten out the fxtracker maxUsed thing
    if( !fxtracker )
    {
        fxtracker = dynArrayAdd(&e->fxtrackers,sizeof(e->fxtrackers[0]),&e->maxTrackerIdxPlusOne,&e->fxtrackersAllocedCount,1);
    }

    //Strictly for performance of the list check
    //Each time a new trackers is added, figure out the highest active tracker
    //It doesn't matter if this goes down, it just needs to never be too small.
    e->maxTrackerIdxPlusOne = 0;
    for( j = 0 ; j < e->fxtrackersAllocedCount ; j++ )
    {
        if( &e->fxtrackers[j] == fxtracker || e->fxtrackers[j].state != FXT_NOT_IN_USE) //if slot is not in use
        {
            e->maxTrackerIdxPlusOne = j+1;
        }
    }

    assert(!fxtracker->fxid);
    return fxtracker;
}

void freeFxTracker( ClientNetFxTracker * fxtracker )
{
    if( fxtracker )
    {
        memset( fxtracker, 0, sizeof( ClientNetFxTracker ) );
        fxtracker->state = FXT_NOT_IN_USE;
    }
}


/*Fire an fx on an entity fron the net.  (It may have been delayed by a timer.)

If it's CREATE MAINTAINED_FX create the fx. and record the the fx's status
in the fxtracker so it can be told to die later

If it's CREATE_ONESHOT_FX just create the fx and forget it
*/
static void entCreateEntityFx( Entity * e, ClientNetFxTracker * fxtracker, NetFx * netfx )
{
    FxParams fxp;
    const char *fxname;
    Entity * entOrigin = 0, *entTarget = 0;
    int fxid = 0;
    int failedToInitFxParams = 0;

    assert(e && e->seq && netfx && netfx->handle && netfx->net_id);

    //Create this FX
    fxname = stringFromReference( netfx->handle );

    fxInitFxParams(&fxp);

    entTarget = entFromId( netfx->targetEnt );
    if(entTarget && entTarget->seq) 
        fxp.targetSeqHandle = entTarget->seq->handle;

    //Get Origin (either the spawning Ent or an XYX position )
    //( no netfx->originEnt means use the spawner )
    if( netfx->originType == NETFX_ENTITY )
    {
        if( netfx->originEnt )
            entOrigin = entFromId( netfx->originEnt );
        else
            entOrigin = e;

        if( entOrigin && entOrigin->seq )
        {
            fxp.keys[fxp.keycount].seq     = entOrigin->seq;
            fxp.keys[fxp.keycount].bone  = netfx->bone_id;
            fxp.keycount++;
        }
        else
        {
            failedToInitFxParams = 1; //You must have an origin or the fx will behave all wrong
        }
    }
    else if( netfx->originType == NETFX_POSITION )
    {
        FxKey * key;
        key = &fxp.keys[fxp.keycount];
        copyMat3( unitmat, key->offset );
        copyVec3( netfx->originPos, key->offset[3] );
        fxp.keycount++;
    }
    else assert ( 0 == "Bad netfx" );

    //Get Target (either the some Ent or an XYX position )
    //(no netfx->targetEnt means you don't need a target)
    if( netfx->targetType == NETFX_ENTITY )
    {
        if( netfx->targetEnt )
        {
            if( entTarget && entTarget->seq ) 
            {
                fxp.keys[fxp.keycount].seq    = entTarget->seq;  
                fxp.keys[fxp.keycount].bone = 2; //Made it Chest. I don't know why I took this out?

                fxp.keycount++;
            }
            else //TO DO source of trouble : hit fx from pets that are really short duration don't work right.
            {
                failedToInitFxParams = 1; //Only if you think you need a target must you have one
            }
        }
        //AIMPITCH TOTAL HACK PLACE TO PUT THIS FIND A BETTER WAY
        if( netfx->pitchToTarget && e->svr_idx != netfx->targetEnt )//Hack to skip over fx that use you as target
        {
            e->seq->powerTargetEntId = netfx->targetEnt;
            zeroVec3( e->seq->powerTargetPos );
        }
        //END TOTAL HACK
    }
    else if( netfx->targetType == NETFX_POSITION )
    {
        FxKey * key;
        key = &fxp.keys[fxp.keycount];
        copyMat3( unitmat, key->offset );
        copyVec3( netfx->targetPos, key->offset[3] );
        if(netfx->powerTypeflags & NETFX_POWER_AT_GEO_LOC)
            fxp.dontRewriteKeys = 1;
        fxp.keycount++;

        //AIMPITCH TOTAL HACK PLACE TO PUT THIS FIND A BETTER WAY
        if( netfx->pitchToTarget )
        {
            copyVec3( netfx->targetPos, e->seq->powerTargetPos );
            e->seq->powerTargetEntId = 0;
        }
        //END TOTAL HACK
    }
    else assert ( 0 == "Bad netfx" );

    if (netfx->hasColorTint)
    {
        int i;
        fxp.numColors = 2;
        for (i=0; i<4; i++) 
        {
            fxp.colors[0][i] = netfx->colorTint.primary.rgba[i];
            fxp.colors[1][i] = netfx->colorTint.secondary.rgba[i];
        }
        fxp.isUserColored = 1;
    }
    else
    {
        fxp.isUserColored = 0;
    }

    if (game_state.tintFxByType) {
        if (netfx->powerTypeflags & (NETFX_POWER_FROM_AOE|NETFX_POWER_CONTINUING)) {
            fxp.numColors = 1;
            fxp.colors[0][0] = 0;
            fxp.colors[0][1] = (netfx->powerTypeflags & NETFX_POWER_CONTINUING)?255:0;
            fxp.colors[0][2] = (netfx->powerTypeflags & NETFX_POWER_FROM_AOE)?255:0;
            fxp.colors[0][3] = 255;
        } else {
            fxp.numColors = 1;
            fxp.colors[0][0] = 0;
            fxp.colors[0][1] = 0;
            fxp.colors[0][2] = 5;
            fxp.colors[0][3] = 255;
        }
    }

    fxp.duration   = netfx->duration;
    fxp.radius       = netfx->radius;
    fxp.power       = netfx->power; 
    fxp.net_id       = netfx->net_id;
    fxp.isPlayer   = e==playerPtr();
    fxp.isPersonal = netfx->command == CREATE_MAINTAINED_FX;
    //fxp.vanishIfNoBone = e == playerPtr() && e->seq && TSTB(e->seq->state, seqGetStateNumberFromName("VANISHIFNOBONE"));

    // Check to see if this FX should be discarded
    if (game_state.tintFxByType && netfx->powerTypeflags &&
        !(netfx->powerTypeflags & NETFX_POWER_IMPORTANT))
    {
        if (e != playerPtr() && // Always show things on the player
            entOrigin && entTarget && // has entities as target and origin
            ENTTYPE(entOrigin) == ENTTYPE_PLAYER &&
            ENTTYPE(entTarget) == ENTTYPE_PLAYER && // Both the target and source are players
            ENT_IS_ON_RED_SIDE(entOrigin) == ENT_IS_ON_RED_SIDE(entTarget) && // It's not a villain targetting a hero or vice versa
            entOrigin != playerPtr() &&
            entTarget != playerPtr() &&  //Neither target nor source are me
            ( // Neither target nor source are on my team
                !team_IsMember(playerPtr(), entOrigin->db_id) &&
                !team_IsMember(playerPtr(), entTarget->db_id)
            ) 
            )
        {
            if (game_state.tintFxByType) {
                fxp.numColors = 1;
                fxp.colors[0][0] = 255;
                fxp.colors[0][1] = 0;
                fxp.colors[0][2] = 0;
                fxp.colors[0][3] = 255;
            } else {
                failedToInitFxParams = 1; // Not failed, but treat it that way so it doesn't get created
            }
        }
    }

    //For Oil Slick.  It shouldn't show buffs or debuffs. Something of a hack
    if( netfx->command == CREATE_MAINTAINED_FX && 
        (e->seq->type->rejectContinuingFX == SEQ_REJECTCONTFX_ALL  ||
         e->seq->type->rejectContinuingFX == SEQ_REJECTCONTFX_EXTERNAL && entOrigin != e) )
    {
        failedToInitFxParams = 1;
    }

    // Yet another hack.  If it's a cameraspace fx trying to play on someone else, toss it in the dumpster
    if (fxIsCameraSpace(fxname) && e != playerPtr() && netfx->targetType != NETFX_POSITION)
    {
        failedToInitFxParams = 1;
    }

    fxid = 0;
    if( !failedToInitFxParams )
    {
        fxid = fxCreate( fxname, &fxp );

        //Debug 
        if( isDevelopmentMode() && fxid && (netfx->command == CREATE_ONESHOT_FX) )
        {
            FxDebugAttributes attribs;
            FxDebugGetAttributes( fxid, 0, &attribs );
            if( attribs.lifeSpan <= 0 && !(attribs.type & FXTYPE_TRAVELING_PROJECTILE) )
                Errorf( "The FX %s is being used as a oneshot, but has no way to die and will hang around forever", fxname );
        }
        //End Debug
    }

    //do bookkeeping, whether you succeeded or not...
    if( netfx->command == CREATE_MAINTAINED_FX )  //Have the entity keep track of this fx
    {
        fxtracker->state  = FXT_RUNNING_MAINTAINED_FX;
        fxtracker->fxid   = fxid;
    }
    else if( netfx->command == CREATE_ONESHOT_FX )     //If this is a ONESHOT, kill the tracker
    {
        freeFxTracker( fxtracker );
    }
}



/*Check if a delayed fx's trigger has come around, and if it has, create it.
Note only triggers now are time and another fx hit you.
*/
static void entCheckFxTriggers(Entity * e)
{
    ClientNetFxTracker * fxtracker;
    int i,trigger;

    for( i = 0 ; i < e->maxTrackerIdxPlusOne ; i++ )
    {
        fxtracker = &(e->fxtrackers[i]);

        if( fxtracker->state == FXT_WAITING_TO_FIRE )
        {
            trigger = 0;

            /*if trigger is time*/
            fxtracker->age += TIMESTEP;  //could do this once
            if( fxtracker->netfx.clientTimer )
                trigger |= (fxtracker->age >= fxtracker->netfx.clientTimer) ? 1 : 0;

            /*if trigger is another fx has hit its target*/
            if( fxtracker->netfx.clientTriggerFx )
                trigger |= lookForThisMsgInMailbox( &globalEntityFxMailbox, fxtracker->netfx.clientTriggerFx, FX_STATE_PRIMEHIT  );

            //Error checking
            if( fxtracker->netfx.clientTriggerFx && fxtracker->age > 300 ) //ten seconds
            {
                if( game_state.fxdebug & FX_DEBUG_BASIC ) 
                    Errorf( "NETFX ERROR: The server told you to play the FX %s when the travelling FX hit it's target.  It's been 10 seconds and it still hasn't happened. Probably, the person who set up the travelling effect didn't tag it as DelayedHit in the PFX file. Tell Doug.",
                    stringFromReference( fxtracker->netfx.handle ) );
                trigger = 1;

            } //End Error check

            if(trigger)
                entCreateEntityFx(e, fxtracker, &(fxtracker->netfx));
        }
    }
}

#define CHOSENENT_YOU 1
#define CHOSENENT_ME 2
Entity * getChosenEnt( int target )
{
    if( target == CHOSENENT_ME )
    {
        return playerPtr();
    }
    else if( target == CHOSENENT_YOU )
    {
        return current_target;
    }
    else return 0;
}



static void debugCheckFxTrackers( Entity * e )
{
    ClientNetFxTracker * fxtracker;
    NetFx * netfx;
    int i,y = 10;
    int x = 5;
    char * stateStrings[4] = { "NOT_IN_USE", "RUNNING_MAINTAINED_FX", "WAITING_TO_FIRE" };
    {
        int count = 0;
        for( i = 0 ; i < e->maxTrackerIdxPlusOne ; i++ )
        {
            fxtracker = &(e->fxtrackers[i]);
            if( fxtracker->state != FXT_NOT_IN_USE )
                count++;
        }
        xyprintf( x,y++, " Total FX Trackers: %d (max %d)", count, e->maxTrackerIdxPlusOne );
    }

    if( game_state.netfxdebug > 1 )
    {
        for( i = 0 ; i < e->maxTrackerIdxPlusOne ; i++ )
        {
            fxtracker = &(e->fxtrackers[i]);
            if( fxtracker->state != FXT_NOT_IN_USE )
            {
                xyprintf( x,y++, "%s %d, Age: %2.2f ", stateStrings[fxtracker->state], fxtracker->fxid, fxtracker->age ) ;
                if( fxtracker->netfx.net_id)
                {
                    netfx = &fxtracker->netfx;
                    if( netfx->handle )
                    {
                        xyprintf( x,y++, "          FxName: %s", stringFromReference( netfx->handle ) );
                        xyprintf( x,y++, "         command: %d", netfx->command  );
                        xyprintf( x,y++, "           NetID: %d", netfx->net_id );
                        if( game_state.netfxdebug > 3 )
                        {
                            xyprintf( x,y++, "          handle: %d", netfx->handle );
                            xyprintf( x,y++, "       originEnt: %d", netfx->originEnt );
                            xyprintf( x,y++, "       originPos: %3.3f %3.3f %3.3f", netfx->originPos[0],  netfx->originPos[1],  netfx->originPos[2]  );
                            xyprintf( x,y++, "         bone_id: %d", netfx->bone_id );
                            xyprintf( x,y++, "       targetEnt: %d", netfx->targetEnt );
                            xyprintf( x,y++, "       targetPos: %3.3f %3.3f %3.3f", netfx->targetPos[0],  netfx->targetPos[1],  netfx->targetPos[2]  );
                            xyprintf( x,y++, "     clientTimer: %3.3f", netfx->clientTimer );
                            xyprintf( x,y++, " clientTriggerFx: %d", netfx->clientTriggerFx  );
                            xyprintf( x,y++, "        duration: %2.2f", netfx->duration );
                            xyprintf( x,y++, "          radius: %2.2f", netfx->radius );
                            xyprintf( x,y++, "           power: %d", netfx->power );
                            xyprintf( x,y++, "          debris: %d", netfx->debris );
                        }
                    }
                }
            }
        }
    }
}

/*When the server wants a client entity to create an effect, the server puts the info about
the effect into the entity's netfx_list[] mailbox and sets netfx_count to raise the flag. Every
frame, entity loops through any new netFx and handles them, them clears the buffer */
static void entManageEntityNetFx(Entity * e)
{
    int i, j;
    NetFx * netfx;
    ClientNetFxTracker * fxtracker = 0;

    //What new netFx arrived over the wire last frame?
    for( i = 0 ; i < e->netfx_count ; i++ )
    {
        netfx = &(e->netfx_list[i]);

        //Debug
        if( game_state.netfxdebug && e == SELECTED_ENT )
            printf( "Managing \n");
        //End Debug


        //Handle the command

        //If it's a create fx command
        if( netfx->command == CREATE_ONESHOT_FX || netfx->command == CREATE_MAINTAINED_FX )
        {
            int alreadyDestroyed;
  
            //Not debug: seems to be cropping up for mission objectives when you first create a mission 
            //map.  I don't know why.  Fix this sometime.
            assert(e && e->seq && netfx && netfx->handle && netfx->net_id);

            if( netfx->command == CREATE_MAINTAINED_FX ) 
            {
                int passOnThisOne = 0;
                for( j = 0 ; j < e->maxTrackerIdxPlusOne  ; j++ )
                {
                    if( e->fxtrackers[j].netfx.net_id == netfx->net_id )
                    {
                        ////Errorf( "NETFX ERROR: Repeated request for same maintained fx (same net_id). %s, id %d", stringFromReference( netfx->handle ), netfx->net_id );
                        passOnThisOne = 1;
                        continue;  //skip this fx
                    }
                } 
                if( passOnThisOne )
                    continue;
            }

            if( game_state.netfxdebug && e == SELECTED_ENT )
                printf( "Create %d (cmd %d)\n", netfx->net_id, netfx->command );
            //End not debug

            //Look through the orphaned destroys and try to match this up
            //This is for out of order packets
            alreadyDestroyed = 0;
            for( j = 0 ; j < e->outOfOrderFxDestroyCount ; j++ )
            {
                NetFx * oofxdestroy;
                oofxdestroy = &e->outOfOrderFxDestroy[j];
                if( oofxdestroy->net_id == netfx->net_id )
                {
                    alreadyDestroyed = 1;
                    memmove( &e->outOfOrderFxDestroy[j], &e->outOfOrderFxDestroy[j+1], ( sizeof(NetFx) * (e->outOfOrderFxDestroyCount-j-1)) );
                    e->outOfOrderFxDestroyCount--;
                    if( game_state.netfxdebug && e == SELECTED_ENT )
                        printf( "++++++++++Match found for FX Destroy! %d (new count %d)\n", netfx->net_id, e->outOfOrderFxDestroyCount );
                    break;
                }
            }

            //If it hasn't been already destroyed (99.9%)
            if( !alreadyDestroyed )
            {
                //Get an FxTracker if needed
                fxtracker = 0;
                if( (netfx->command == CREATE_MAINTAINED_FX) || netfx->clientTimer || netfx->clientTriggerFx )
                {
                    fxtracker = getAFreeFxTracker( e );
                    assert( fxtracker );

                    fxtracker->netfx = *netfx;
                }

                //If timer or trigger, set up the FxTracker to wait to be triggered
                if( netfx->clientTimer || netfx->clientTriggerFx )
                {
                    fxtracker->state = FXT_WAITING_TO_FIRE;
                    fxtracker->age     = 0.0;
                }
                //Otherwise, go ahead and create it now
                else
                {
                    entCreateEntityFx( e, fxtracker, netfx );
                }
            }
        }
        //If it's a destroy command
        else if( netfx->command == DESTROY_FX ) 
        {
            int fxFound = 0;

            //Debug
            if( game_state.netfxdebug && e == SELECTED_ENT )
                printf( "Kill %d (cmd %d)\n", netfx->net_id, netfx->command );
            //End debug

            //Search through the trackers for the fx you want to kill
            for( j = 0 ; j < e->maxTrackerIdxPlusOne ; j++ )
            {
                fxtracker = &(e->fxtrackers[j]);

                //TO DO : what if the tracker is 
                if( fxtracker->netfx.net_id == netfx->net_id )
                {
                    //(not guaranteed there even is an fx yet, it could have come this frame,
                    //or still be in the timing queue, but this fxDelete covers us)
                    fxDelete(  fxtracker->fxid, SOFT_KILL );
                    freeFxTracker( fxtracker );
                    fxFound = 1;
                    break;
                }
            }

            //Missed a packet
            if( !fxFound ) 
            {
                NetFx * oofxdestroy;
                oofxdestroy = dynArrayAdd(&e->outOfOrderFxDestroy,sizeof(e->outOfOrderFxDestroy[0]),&e->outOfOrderFxDestroyCount,&e->outOfOrderFxDestroyMax,1);
                *oofxdestroy = *netfx;
                //Debug
                if( game_state.netfxdebug && e == SELECTED_ENT )
                    printf( "++++++++++Unmatched FX Destroy! %d (new count %d)\n", netfx->net_id, e->outOfOrderFxDestroyCount );
                //End debug
            }
        }
        else if( isDevelopmentMode() )
        {
            Errorf( "NETFX ERROR: Nonsense fx request on ent %s tell Woomer. Thank you", e->seq->type->name );
        }
    }

    //After all newly received netFx have been handled, clear the list
    memset( e->netfx_list, 0, sizeof( NetFx ) * e->netfx_count );
    e->netfx_count = 0; 

    if( game_state.netfxdebug && e == SELECTED_ENT )
        debugCheckFxTrackers( e );
}


//################# End Handle Client Entity Fx #####################################

static void calculatePlayerMoveThisFrame()
{
    Entity * e;
    e = playerPtr(); 
    if( e )
    {
        Vec3 velocity;
        subVec3( ENTPOS(e), e->posLastFrame, velocity );
        g_playerMoveRateThisFrame = lengthVec3( velocity );
        if (TIMESTEP)
            g_playerMoveRateThisFrame /= TIMESTEP;
        //xyprintf( 42, 42, "##### player move %f ", g_playerMoveRateThisFrame );
    }
}

//float    combat_transition_vec;
static void handleEntityFading( Entity * e )    
{
    int    doSmoothXlucencyFade;

    //Do fade in or xlucency fade.  one or the other not both

    //If you start this tick fading in or out, then e->xlucency shouldn't try to fade to it's
    //alpha level, but should just jump so the two methods don't trample on each other
    //Having two methods for fading like this is weak, but I ain't touching it at this point
    if( e->fadedirection == ENT_NOT_FADING )
        doSmoothXlucencyFade = 1;
    else
        doSmoothXlucencyFade = 0;

    //System 1 for fading a character into and out of existence 
    if( e->fadedirection == ENT_FADING_IN )      
    {
        if( e->currfade < 255 ) //TO DO PERF I MY HAVE MESSED THIS UP, BUT IT DIDN'T WORK RIGHT BEFORE
        {
            F32 fadeInAmount;
            fadeInAmount = ceil(TIMESTEP * ENT_FADE_IN_RATE);  

            //This little trick says that if the player is booking along, guys should fade in faster so they don't fade in past you
            if( g_playerMoveRateThisFrame > 1.0 )
                fadeInAmount *= g_playerMoveRateThisFrame * 3; 

            e->currfade = MIN(255, (e->currfade + (U8)fadeInAmount));
        }
        else
            e->fadedirection = ENT_NOT_FADING;
    }
    else if( e->fadedirection == ENT_FADING_OUT )
    {
        if( !e->seq || e->seq->type->ticksToFadeAwayAfterDeath < 1) // seq might be null on the testclient.
            e->currfade = 0;
        else
            e->currfade = MAX(0, (e->currfade - ceil(TIMESTEP * (255.0/(F32)e->seq->type->ticksToFadeAwayAfterDeath) )));//(MAX(1, (TIMESTEP * ENT_FADE_OUT_RATE )))));
    }
    else //e->fadedirection == ENT_NOT_FADING
    {
        e->currfade = 255;
    }

    //System 2 for smooth fading when set by powers
    //For smooth fading of powers, disable for fading in stuff
    if( doSmoothXlucencyFade )
    {
        if(fabs(e->xlucency-e->curr_xlucency)<(TIMESTEP*ENT_XLUCENCY_RATE))
        {
            e->curr_xlucency = e->xlucency;
        }
        else if(e->xlucency < e->curr_xlucency)
        {
            e->curr_xlucency = MIN(1.0f, e->curr_xlucency-TIMESTEP*ENT_XLUCENCY_RATE);
        }
        else if(e->xlucency > e->curr_xlucency)
        {
            e->curr_xlucency = MAX(0.0f, e->curr_xlucency+TIMESTEP*ENT_XLUCENCY_RATE);
        }
    }
    else
    {
        e->curr_xlucency = e->xlucency;
    }
}


static BasicTexture *checkForTextureSwaps2(DefTracker *node, BasicTexture *texture)
{
#if !defined(TEST_CLIENT)
    // Check parent for texture swaps first
    if (node->parent)
        texture = checkForTextureSwaps2(node->parent, texture);

    // See modelSubGetBaseTexture() and modelGetFinalTexturesInline() in Game\render\rendermodel.c
    if (node->def->def_tex_swaps)
    {
        int i;
        for (i = 0; i < eaSize(&node->def->def_tex_swaps); i++)
        {
            DefTexSwap *texSwap = node->def->def_tex_swaps[i];

            if (texSwap && texSwap->composite)
            {
                if (texSwap->comp_tex_bind && texSwap->comp_replace_bind
                    && texSwap->comp_tex_bind->tex_layers[TEXLAYER_BASE]->actualTexture == texture
                    && texSwap->comp_replace_bind->tex_layers[TEXLAYER_BASE]->actualTexture)
                {
                    texture = texSwap->comp_replace_bind->tex_layers[TEXLAYER_BASE]->actualTexture;
                }
            }
            else
            {
                if (texSwap && texSwap->tex_bind && texSwap->tex_bind->actualTexture == texture && texSwap->replace_bind)
                {
                    texture = texSwap->replace_bind;
                }
            }
        }
    }

#endif
    return texture;
}

static BasicTexture *checkForTextureSwaps(DefTracker *node, BasicTexture *texture)
{
#if !defined(TEST_CLIENT)
    int count, i;
    int *didSwap=NULL;
    TexSwap **swaplist;
    TexBind *bind = NULL;

    if (!texture)
        return texture;

    // First, check the scene swap list
    swaplist = sceneGetTexSwaps(&count);

    // This logic is similar to texCheckForSceneSwaps() in Game\render\tex.c
    for (i = 0; i < count; i++)
    {
        if (strcmp(swaplist[i]->src, texture->name) == 0)
        {
            BasicTexture *swapTexture = texFind(swaplist[i]->dst);
            if (swapTexture)
            {
                texture = swapTexture;
                break;
            }
        }
    }

    // Next, check the material swap list
    bind = texFindComposite(texture->name);

    if (bind && bind->tex_layers[TEXLAYER_BASE])
        texture = bind->tex_layers[TEXLAYER_BASE];

#endif
    // Finally, check the node hierarchy for swaps
    return checkForTextureSwaps2(node, texture);
}


static void calcSurfaceEntityIsStandingOn(Entity *e)
{
    Vec3    start,end;
    CollInfo    coll;
    Entity* player = playerPtr();
    Vec3 d;
    F32 dpossquared;

    if(!player)
        return;

    if (ENTTYPE(e)==ENTTYPE_NPC || ENTTYPE(e)==ENTTYPE_CAR) {
        if (0) {
            Vec3 dv;
            subVec3(ENTPOS(e),ENTPOS(player),dv);
            if (lengthVec3Squared(dv) > SQR(50))
                return;
        } else {
            // NPCs and cars do not use the surface flags ala M. Henry
            return;
        }
    }

    PERFINFO_AUTO_START("calcSurface", 1);

    subVec3(ENTPOS(e), e->posLastFrame, d);
    dpossquared = lengthVec3Squared(d);
    if (dpossquared < SQR(0.01)) {
        // just use last frame's data
        seqSetState( e->seq->state, 1, e->seq->surface );
        
    } else {
        int hit;
        
        copyVec3(ENTPOS(e),start);
        copyVec3(start,end);
        start[1] += 1;
        end[1] -= 1;
        
        PERFINFO_AUTO_START("collGrid", 1);
            hit = collGrid(0,start,end,&coll,0,COLL_DISTFROMSTART);
        PERFINFO_AUTO_STOP();
        
        if (hit)
        {
            if (coll.surfTex) {
                BasicTexture *surfTex = checkForTextureSwaps(coll.node, coll.surfTex);

                if (surfTex->actualTexture->texopt_surface) {
                    e->seq->surface = surfTex->actualTexture->texopt_surface;
                } else if (surfTex->texopt_surface) {
                    // For backwards compatibility/swapping to textures that *don't* have a surface
                    e->seq->surface = surfTex->texopt_surface;
                } else {
                    e->seq->surface = 0;
                }
            } else {
                e->seq->surface = 0;
            }
            if( !e->seq->surface )
                e->seq->surface = STATE_CONCRETE; //Default surface
            seqSetState( e->seq->state, 1, e->seq->surface );
            
            // fpe 11/13/09 -- added GROUND state to indicate on ANY surface.
            //    FX scripts check for an exact match on bits, so can't OR together
            //    state bits in the script since all must be set.  Thus we have GROUND
            //    state that gets set whenever on a surface.  Script can then specify
            //    one triggerbit for GROUND instead of having to have a separate entry
            //    for each surface type.
            seqSetState( e->seq->state, 1, STATE_GROUND );
        }
        else
        {
            e->seq->surface = 0;
        }
    }

    PERFINFO_AUTO_STOP();
}

/*mw first pass at a little non randomness*/
/*steepness 1 = Purely random
  steepness 10 = very strong preference for very different result */
F32 nonRandomRoll( F32 recentResult, int steepness )
{
    int i;
    F32 newRand = 0;

    for( i = 0 ; i < steepness ; i++ )
        newRand += rand();

    newRand /= (F32)steepness;

    newRand += recentResult;
    if( newRand > 1.0 )
        newRand -= 1.0;

    return newRand;
}

void gfxNodeTest(GfxNode *node,int seqHandle)
{
    for(;node;node = node->next)
    {
        if(node->seqHandle == seqHandle && node->useFlags )
        {
            if (node->child)
                gfxNodeTest(node->child,seqHandle);
        }
    }
}


void gfxNodeSetEntAlpha(GfxNode *node,U8 alpha,int seqHandle)
{
    for(;node;node = node->next)
    {
        if(node->seqHandle == seqHandle && node->useFlags )
        {
            node->alpha = alpha;
            if (node->child)
                gfxNodeSetEntAlpha(node->child,alpha,seqHandle);
        }
    }
}



static void updateSimpleShadow( SeqInst * seq, F32 distFromCamera, F32 maxDeathAlpha )
{
    static SplatParams sp;
    SplatParams * splatParams;

    splatParams = &sp;

    {
        //Make sure you have a good node   
        if( !gfxTreeNodeIsValid(seq->simpleShadow, seq->simpleShadowUniqueId ) )
        { 
            seq->simpleShadow = initSplatNode( &seq->simpleShadowUniqueId, seq->type->shadowTexture?seq->type->shadowTexture:SEQ_DEFAULT_SHADOW_TEXTURE, "white", 0 );
        }  
        assert( gfxTreeNodeIsValid( seq->simpleShadow, seq->simpleShadowUniqueId ) );
        splatParams->node = seq->simpleShadow;  
    }

    //node+uniqueID + shadowParams
    //Fill out the shadow node parameters:
    mulVecVec3( seq->type->shadowSize, seq->currgeomscale, splatParams->shadowSize ); //Get shadowsize
    scaleVec3(splatParams->shadowSize, 0.5f, splatParams->shadowSize);

    //Get alpha for shadow
    {
        F32 distAlpha;
        F32 drawDist;
        U8 maxAlpha; //maxAlpha possible under the circumstances
        
        drawDist = SHADOW_BASE_DRAW_DIST * ( splatParams->shadowSize[0] / SEQ_DEFAULT_SHADOW_SIZE );
 
        //If you are outside of your comfort zome, fade out over 30 feet
        distAlpha = (1.0 - (( distFromCamera - drawDist ) / SHADOW_FADE_OUT_DIST )) ; 
        distAlpha = MINMAX( distAlpha, 0, 1.0 );

        //Find MaxAlpha
        maxAlpha = seq->seqGfxData.alpha;                            //Character's alpha
        maxAlpha = MIN( maxAlpha, (U8)(g_sun.shadowcolor[3] * 255) );    //Time of Day alpha
        maxAlpha = MIN( maxAlpha, (U8)(distAlpha * 255) );              //Distance from camera 
        maxAlpha = MIN( maxAlpha, (U8)(maxDeathAlpha * 255) );        //Death alpha

        splatParams->maxAlpha = maxAlpha;
    }
    
    //Use hips if available...
    if( seq->gfx_root->child )
    {
        Mat4 hipsMat;
        F32 diff;
        gfxTreeFindWorldSpaceMat( hipsMat, seq->gfx_root->child );
        diff = hipsMat[3][1] - seq->gfx_root->mat[3][1];
        copyVec3( hipsMat[3], splatParams->projectionStart ); 
        splatParams->projectionStart[1] -= diff;  
    }
    else
    {
        copyVec3( seq->gfx_root->mat[3], splatParams->projectionStart );   //Get Projection Start   
    }

    //DO ShadowOffset, kind of a hack so artists can twiddle exactly where they want the shadow
    {
        Mat4 mat;
        Mat4 mCorrectedGfxRootMat;
        copyMat4( unitmat, mat );
        copyVec3( seq->type->shadowOffset, mat[3] );

        copyVec3(seq->gfx_root->mat[3], mCorrectedGfxRootMat[3]);
        copyMat3(unitmat, mCorrectedGfxRootMat);

        mulMat4( mCorrectedGfxRootMat, mat, splatParams->mat ); 

        {
            Vec3 tv;
            mulVecMat3( seq->type->shadowOffset, splatParams->mat, tv );
            addVec3( splatParams->projectionStart, tv, splatParams->projectionStart );
        }
    }

    

    splatParams->projectionStart[1]+= SHADOW_SPLAT_OFFSET; //won't work for longerstuff maybe should be scaled like fxbhvr setBack?  

    shadowStartScene(); //Just calculates light direction of sun
    copyVec3( g_sun.shadow_direction, splatParams->projectionDirection );  //Get Light Direction 
    splatParams->projectionDirection[0] = 0;
    splatParams->projectionDirection[1] = -1;
    splatParams->projectionDirection[2] = 0;

    //splatParams->up = ??

    

    //Set the Collision Density Rejection Coefficient
    switch(seq->type->shadowQuality)
    {
        xcase SEQ_LOW_QUALITY_SHADOWS:
            splatParams->max_density = SPLAT_LOW_DENSITY_REJECTION;
        xcase SEQ_MEDIUM_QUALITY_SHADOWS:
            splatParams->max_density = SPLAT_MEDIUM_DENSITY_REJECTION;
        xcase SEQ_HIGH_QUALITY_SHADOWS:
            splatParams->max_density = SPLAT_HIGH_DENSITY_REJECTION;
    }

    splatParams->rgb[0] = 0;  
    splatParams->rgb[1] = 0;
    splatParams->rgb[2] = 0;

    splatParams->falloffType = SPLAT_FALLOFF_SHADOW;
    splatParams->flags |= SPLAT_ROUNDTEXTURE;

    {
        Splat* splat = splatParams->node->splat;
        Vec3 vMovement;
        subVec3(splat->previousStart, splatParams->projectionStart, vMovement);
        
        if ( game_state.simpleShadowDebug 
            || lengthVec3Squared(vMovement) > 0.001f
            || fabsf(splat->width - splatParams->shadowSize[0]) > 0.001f
            || fabsf(splat->height - splatParams->shadowSize[2]) > 0.001f
            || fabsf(splat->depth - splatParams->shadowSize[1]) > 0.001f
            )
        {
            updateASplat( splatParams ); 
        
        }

        updateSplatTextureAndColors(splatParams, 2.0f * splatParams->shadowSize[0], 2.0f * splatParams->shadowSize[2]);

    }

}


static F32 seqGetMovementSpeedAnimScale( SeqInst * seq, const Vec3 pos, const Vec3 posLastFrame )
{
    Vec3 velocity;
    F32 moveDist,actualMoveRate,targetMoveRate,moveScale, cappedMoveScale;
    const SeqMove * currmove;
        
    currmove = seq->animation.move;    

    if( !currmove || !(currmove->flags & SEQMOVE_MOVESCALE) )          
        return 1.0;

    //TO DO clear moved instantly somewhere else after fx use it
    if( seq->moved_instantly )
        return 1.0;

    if( seq->animation.typeGfx->moveRate )
        targetMoveRate = seq->animation.typeGfx->moveRate; 
    else if( currmove->moveRate )
        targetMoveRate = currmove->moveRate;
    else if ( isDevelopmentMode() )
        Errorf( "Error in Sequencer %s Move %s Type %s : You set moveScale flag, but gave me no moveRate to do it with.", seq->type->seqname, currmove->name, seq->animation.typeGfx->type );   
    

    subVec3( pos, posLastFrame, velocity );
    moveDist = lengthVec3( velocity );

    actualMoveRate = moveDist / (TIMESTEP?TIMESTEP:1);

    moveScale = actualMoveRate / targetMoveRate;    

    //Cap the move scaler so hitting a wall won't freeze you and going super fast wont be ridiculous
    cappedMoveScale = MAX( 0.25, moveScale ); //Cap at half speed 
    cappedMoveScale = MIN( 2.0, cappedMoveScale );  //Cap at 2x speed

    //Debug 
    if( (game_state.seq_info == 2) && current_target && seq == current_target->seq )
    {
        xyprintf( 50,49, "TIMESTEP  %f", TIMESTEP );  
        xyprintf( 50,50, "actualMoveRate  %f", actualMoveRate );  
        xyprintf( 50,51, "targetMoveRate  %f", targetMoveRate );  
        xyprintf( 50,52, "movescale %f", moveScale );
        xyprintf( 50,53, "capped movescale  %f", cappedMoveScale );
        xyprintf( 50,54, "movestep  %f", moveScale * TIMESTEP );
        xyprintf( 50,55, "diff      %f", TIMESTEP - (moveScale * TIMESTEP) );
    }

    return cappedMoveScale; 
}

static void smoothSprintAnimation( SeqInst * seq, F32 moveRateAnimScale )
{
    GfxNode * node; 
    F32 base, modifier;

    modifier = moveRateAnimScale*moveRateAnimScale;//this seems like the best mod

    base = seq->animation.typeGfx->smoothSprint; 

    if( base )
    {
        node = gfxTreeFindBoneInAnimation( 0, seq->gfx_root->child, seq->handle, 1 );
        node->mat[3][1] = base + ( (node->mat[3][1] - base) / modifier );
    }

    /*if( e == playerPtr() )
    {
    static F32 moveHistory[100];
    static int line;
    int i;
    moveHistory[line] = modifier;   
    for( i = 0 ; i < 100 ; i++ )
    {    
    if( i == line )
    xyprintfcolor( 30, 1+i, 255, 255, 255, "%f", moveHistory[i] );
    else
    xyprintfcolor( 30, 1+i, 255, 50, 100, "%f", moveHistory[i] );
    }
    line++;
    if( line == 100 )
    line = 0;
    }*/
}

#if NOVODEX
static void entFreeNovodexCapsule(Entity* e)
{
#if !TEST_CLIENT
    if ( !e->nxCapsuleKinematic)
        return;
    if (!nwEnabled())
        return;
    // need to remove it
    nwDeleteActor(nwGetActorFromEmissaryGuid(e->nxCapsuleKinematic), NX_CLIENT_SCENE);
    e->nxCapsuleKinematic = 0;
#endif
}

static void entUpdateNovodex( Entity* e, F32 entDistance)
{
#if !TEST_CLIENT
    const float fActorSporeRadius = 50.0f; // FIXME - shouldn't be hard coded (copied from groupnovodex.c)
    bool bUpdate = ( entDistance <= fActorSporeRadius );

    if (!nwEnabled())
    {
        e->nxCapsuleKinematic = 0;
        return;
    }


    if ( isEntCurrentlyCollidable( e ) && bUpdate && nx_state.entCollision )
    {
        Mat4 mCapsuleMat;
        Vec3 vOffset;
        F32 fLength = (e->motion->capsule.length > 0.0f) ? e->motion->capsule.length : 0.0002f;
        NwEmissaryData* pData = nwGetNwEmissaryDataFromGuid(e->nxCapsuleKinematic);
        /*
        copyMat4(ENTMAT(e), mCapsuleMat);
        zeroVec3(vOffset);
        vOffset[1] = e->motion->capsule.length * 1.0f ;//+ e->motion->capsule.radius;
        addVec3(mCapsuleMat[3], vOffset, mCapsuleMat[3]);
        */
        positionCapsule(ENTMAT(e), &e->motion->capsule, mCapsuleMat);
        scaleVec3(mCapsuleMat[1], fLength * 0.5f, vOffset);
        addVec3(vOffset, mCapsuleMat[3], mCapsuleMat[3]);

        if ( !pData )
        {
            // Need to create it
            e->nxCapsuleKinematic = nwPushCapsuleActor(mCapsuleMat, e->motion->capsule.radius, fLength, nxGroupKinematic, -1.0f, NX_CLIENT_SCENE );
        }
        else 
        {
            // already exists, just update it
            if ( pData->pActor )
                nwSetActorMat4(pData->uiActorGuid, (float*)mCapsuleMat);
        }
    }
    else
    {
        entFreeNovodexCapsule(e);
        }
    // else do nothing
#endif
}
#endif

//AIMPITCH calculate the pitch to be used
//TO DO a smoother system than feeding off fx. Knowing better when you are dont, so it doesn't rely on 
//      the data from the power and the seqeucner to all be just right
//      Happen for a few frames, but you don't want it sticking around to invade a new move
//      Shouldn't ever happen, since any move with the flag also comes down with a target, but 
//      still, it seems a little shakey.  Maybe say once it's been used at all, it can't be used 
//      again till reset.
static void seqDoAnimationPitching( SeqInst * seq )
{
#define DISTANCE_TO_START_DAMPENING 1.0
#define MAX_HIGH_TARGETING_PITCH 1.0
#define MAX_LOW_TARGETING_PITCH -1.0
#define IS_NOTHING    0
#define IS_ROOT        1
#define IS_CHEST    2

    F32 goalTargetingPitch;
    int doPitching;
 
    //### calculate seq->goalTargetingPitch
    
    //Should I do pitching at all? Only in moves tagged as pitching moves, and only
    //if they are in the part of the move tagged as pitching
    doPitching = ( seq->animation.move && seq->animation.move->flags & SEQMOVE_PITCHTOTARGET );
    if( doPitching )
    {
        F32 firstFrame, lastFrame;

        firstFrame = seq->animation.typeGfx->pitchStart;
        if( seq->animation.typeGfx->pitchEnd == -1 )
            lastFrame = seq->animation.typeGfx->animP[0]->lastFrame;
        else
            lastFrame = seq->animation.typeGfx->pitchEnd;

        if( firstFrame > seq->animation.frame || lastFrame < seq->animation.frame )
            doPitching = 0;
    }

    
    if( doPitching ) //Then get goal targeting pitch         
    {
        Vec3 targetPos, myPos; 
        int whatTargetPositionIs = IS_NOTHING;

        //HACKY method to get targetPos
        if( seq->powerTargetEntId )  
        {
            Entity * e2 = entFromId( seq->powerTargetEntId );

            if( e2 ) 
            {
                GfxNode *chestNode = seqFindGfxNodeGivenBoneNum(e2->seq, BONEID_CHEST);
                if( chestNode )
                {
                    Mat4 chestMat;
                    gfxTreeFindWorldSpaceMat(chestMat, chestNode);
                    copyVec3( chestMat[3], targetPos );
                    whatTargetPositionIs = IS_CHEST;
                }
                else
                {
                    copyVec3( ENTPOS(e2), targetPos );
                    whatTargetPositionIs = IS_ROOT;
                }
            }
            else
            {
                zeroVec3( seq->powerTargetPos );
                seq->powerTargetEntId = 0;
            }
        }
        else
        {
            //Very occasionally, powerTargetPos is empty, but I don't know why.  
            //This check prevents it from pitching wildly, anyway.
            Vec3 zero;
            zeroVec3(zero);
            if( !nearSameVec3( seq->powerTargetPos, zero ) )
            {
                copyVec3( seq->powerTargetPos, targetPos );
                whatTargetPositionIs = IS_ROOT; //Power miss postion
            }
        } 
        //End Hacky method of finding targetPos


        //Find pitch
        if( whatTargetPositionIs != IS_NOTHING ) 
        {
            Vec3 dv;
            F32 len;

            //Get My position
            if(whatTargetPositionIs == IS_CHEST)
            {
                GfxNode *chestNode = seqFindGfxNodeGivenBoneNum(seq, BONEID_CHEST);
                if( chestNode )
                {
                    Mat4 chestMat;
                    gfxTreeFindWorldSpaceMat(chestMat, chestNode);
                    copyVec3( chestMat[3], myPos );
                }
                else
                {
                    whatTargetPositionIs = IS_ROOT;
                }
            }

            //If any of the searches fail, go to the non fancy version
            if(whatTargetPositionIs == IS_ROOT)
            {
                copyVec3( seq->gfx_root->mat[3], myPos );
                //copyVec3( e2->mat[3], targetPos );  //TO DO recover and make target zero if you arent
            }

            //Find pitch
            subVec3( targetPos, myPos, dv );
            len = normalVec3( dv );

            goalTargetingPitch = asin( dv[1] );

            //proximity dampening, test: only do for entities
            if( seq->powerTargetEntId )
            {
                F32 proximityDampener;
                proximityDampener = (len / DISTANCE_TO_START_DAMPENING);
                proximityDampener = MIN( 1.0, proximityDampener );
                goalTargetingPitch *= proximityDampener;
            }

            goalTargetingPitch = MINMAX( goalTargetingPitch, MAX_LOW_TARGETING_PITCH, MAX_HIGH_TARGETING_PITCH );
        }
        else
            goalTargetingPitch = 0;
    }
    else //You are not in a move that takes targeting pitch
    {
        goalTargetingPitch = 0;
    }

    ///$####################################################################### 
    //use the pitchAngle in the move to divide seq->goalTargetingPitch into pitch and roll, 
    //-1.0 means all roll left, -1.0 = all roll right, 0 = all pitch backwards (Default value for pitchAngle is 0, all pitch)
    {
        Vec2 goalTargetingPitchVec;
        Vec3 pitchRateVec;
        F32 pitchRate;
        Vec2 dist;
        int i;
        F32 roll;

        roll= seq->animation.typeGfx->pitchAngle; 
        goalTargetingPitchVec[0] = goalTargetingPitch * ( 1.0 - ABS( roll ) ); 
        goalTargetingPitchVec[1] = goalTargetingPitch * roll;

        //Divide the rate based on progress needed
        pitchRate = seq->animation.typeGfx->pitchRate;
        dist[0] = seq->currTargetingPitchVec[0] - goalTargetingPitchVec[0];

        dist[1] = seq->currTargetingPitchVec[1] - goalTargetingPitchVec[1];


        if( dist[0] || dist[1] )
        {
            dist[0] = ABS( dist[0] );
            dist[1] = ABS( dist[1] );
            pitchRateVec[0] = pitchRate * (dist[0] / (dist[0] + dist[1]));
            pitchRateVec[1] = pitchRate * (dist[1] / (dist[0] + dist[1]));
        }
        else if( !dist[1] ) //stupid divide by zeros
        {
            pitchRateVec[0] = pitchRate;
            pitchRateVec[1] = 0;
        }
        else if( !dist[0] )
        {
            pitchRateVec[0] = 0;
            pitchRateVec[1] = pitchRate;
        }

        //### calculate currTargetingPitchVec to smoothly go into and out of the pitch.
        for( i = 0 ; i < 2 ; i++ ) 
        {
            if( seq->currTargetingPitchVec[i] > goalTargetingPitchVec[i] )         
            {
                seq->currTargetingPitchVec[i] = seq->currTargetingPitchVec[i] - ( pitchRateVec[i] * TIMESTEP );
                if( seq->currTargetingPitchVec[i] < goalTargetingPitchVec[i] )
                    seq->currTargetingPitchVec[i] = goalTargetingPitchVec[i];
            }
            else if( seq->currTargetingPitchVec[i] < goalTargetingPitchVec[i] )
            {            
                seq->currTargetingPitchVec[i] = seq->currTargetingPitchVec[i] + ( pitchRateVec[i] * TIMESTEP );
                if( seq->currTargetingPitchVec[i] > goalTargetingPitchVec[i] )
                    seq->currTargetingPitchVec[i] = goalTargetingPitchVec[i];
            }
        }
    }
}



// apparently breaking up entClientUpdate is not done, but i'm doing it anyway, a litle
static void updateSkeletonFromRagdoll(Entity *e)
{
#ifdef RAGDOLL
    /* clientSideRagdoll deprecated
    if ( clientSideRagdoll() )
    {
//        drawRagdollSkeleton(e->ragdoll, e->mat);
        setSkeltonIdentity(e->seq, e->seq->gfx_root->child);
        nwSetRagdollFromQuaternions( e->ragdoll, unitQuat );
        setSkeletonFromRagdoll(e->ragdoll, e->seq, e->seq->gfx_root->child);
        nwGetActorMat4(e->ragdoll->pActor, (float*)e->mat);
        copyMat4(e->mat, e->seq->gfx_root->mat);

    }
    else
    */
    {
        if (firstValidRagdollTimestamp( e->ragdoll ) < global_state.client_abs 
            && !control_state.no_ragdoll )
        {
            Vec3 vScale;
            interpRagdoll(e->ragdoll, global_state.client_abs);
            setSkeletonIdentity(e->seq, e->seq->gfx_root->child);
            setSkeletonFromRagdoll(e->ragdoll, e->seq, e->seq->gfx_root->child);
            extractScale(e->seq->gfx_root->mat,vScale);
            copyMat4(ENTMAT(e), e->seq->gfx_root->mat );
            scaleMat3Vec3(e->seq->gfx_root->mat,vScale);

            /*        
            if ( secondValidRagdollTimestamp(e->ragdoll) > global_state.client_abs )
            {
                Mat4 mSeqGfxRootMat;
                Mat3 mOffsetMat;
                copyMat4(e->mat, mSeqGfxRootMat );
                addVec3(mSeqGfxRootMat[3], e->ragdoll_offset_pos, mSeqGfxRootMat[3] );
                // multiply by offset rotation
                quatToMat(e->ragdoll_offset_qrot, mOffsetMat);
                mulMat3(mOffsetMat, e->mat, mSeqGfxRootMat);
                
                copyMat4(mSeqGfxRootMat, e->seq->gfx_root->mat);
            }
            else
            */
        }
    }
#endif
}

#include "sound/sound.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "win/win_init.h"
#include "UI/uiUtil.h"
#include "UI/uiReticle.h"
#include <float.h>

int g_EntsUpdatedByClient;
int g_EntsProcessedForDrawing;

static int testSphereVisibility(Vec3 pos, F32 rad) 
{
    int clip = CLIP_NONE;
#ifndef TEST_CLIENT
    if (s_entity_do_shadowmap_culling)
    {
        if ( !isShadowExtrusionVisible( pos, rad ) )
        {
            clip = 0;
        }
    }
    else
#endif
    {
        clip = gfxSphereVisible(pos, rad);
    }

    return clip;
}

static int isGroupDefVisible( GroupDef * def, Mat4 worldMat )
{
    Mat4 matx;
    Vec3 mid;
    int clip;
    bool visible;

    if( !def || !worldMat ) 
        return 0;

    mulMat4( cam_info.viewmat, worldMat, matx );

    //mulVecMat4( def->mid, mat, groupEnt->mid_cache);

    mulVecMat4( def->mid, matx, mid );

    clip = testSphereVisibility( mid, def->radius );

    visible = clip ? true : false;
#ifndef TEST_CLIENT
    if ( visible && game_state.zocclusion && !((editMode() || game_state.see_everything & 1) && !edit_state.showvis))
    {
        if ( !zoTestSphere( mid, def->radius, clip & CLIP_NEAR ) )
            visible = false; //Occluded
    }
#endif
    return visible;
}

static void entDebugShowVolumeInfo( Entity * e )
{
    VolumeList * vl = &e->volumeList;
    int i;
    int x = 20;
    int y = 20;

    //Print debug info about collisions
    xyprintf( x, y++, " Material     %p %p ", vl->materialVolume->volumeTracker, vl->materialVolume->volumePropertiesTracker );
    xyprintf( x, y++, " Neighborhood %p %p ", vl->neighborhoodVolume->volumeTracker, vl->neighborhoodVolume->volumePropertiesTracker );
    y++;
    for( i = 0 ; i < vl->volumeCount ; i++)    
    {
        char * name = 0;
        char * namep = 0;
        char * musicp = 0;
        int villainMin = 0, villainMax = 0, minTeam = 0, maxTeam = 0;

        //Volume
        xyprintf( x, y, "%d ", i );  
        xyprintf( x + 5, y, "Volume %p", vl->volumes[i].volumeTracker); 

        xyprintf( x + 22 , y, "PropertyTracker %p: ", vl->volumes[i].volumePropertiesTracker );

        //TO DO : just list all properties
        pmotionGetNeighborhoodPropertiesInternal(vl->volumes[i].volumePropertiesTracker, &namep, &musicp, &villainMin, &villainMax, &minTeam, &maxTeam);
        if(namep)
            xyprintf( x + 0 , y+1, "NHoodName %s ", namep);
        if( musicp )
            xyprintf( x + 30 , y+1, "NHoodSound %s ", musicp);
        if( villainMin || villainMax )
            xyprintf( x + 60 , y+1, "NHoodVillainMin/Max %d-%d ", villainMin, villainMax);
        if( minTeam || maxTeam )
            xyprintf( x + 90 , y+1, "NHoodMin/MaxTeam %d-%d ", minTeam, maxTeam);
        if(namep || musicp || villainMin || villainMax || minTeam || maxTeam)
            y++;

        if(vl->volumes[i].volumePropertiesTracker)
            name = groupDefFindPropertyValue(vl->volumes[i].volumePropertiesTracker->def,"NamedVolume");
        if( name )
            xyprintf( x + 0 , ++y, "NamedVolume %s ", name);

        //TO DO : list volume trick properties

        y++;
    }
}

static void handleEntityVolumeChecks( Entity * e )
{
    if(    ENTTYPE(e) != ENTTYPE_DOOR &&
        ENTTYPE(e) != ENTTYPE_CAR &&
        ENTTYPE(e) != ENTTYPE_NPC)
    {
        PERFINFO_AUTO_START("pmotionCheckVolumeTrigger", 1);
        pmotionCheckVolumeTrigger(e);
        if( (game_state.checkVolume && e == playerPtr() ) )// TO DO make selection option isThisDebugEnt( SeqInst * seq )   ) 
            entDebugShowVolumeInfo( e ); //Debugging 
        PERFINFO_AUTO_STOP();
    }

    PERFINFO_AUTO_START("seqManageVolumeFx", 1);
    seqManageVolumeFx(e->seq, e->vol_collision, e->prev_vol_collision, e->vel_last_frame);
    copyVec3( e->motion->vel, e->vel_last_frame ); //know how fast you were going before splash
    PERFINFO_AUTO_STOP();
}

// This has been commented out as it apparently is not used for anything useful and is causing the following bugs:
//    COH-16334, COH-21327, COH-25016
static void handleStickingFeet( Entity * e )
{
    SeqInst * seq = e->seq;

    if( game_state.viewCutScene )//&&  ENTTYPE(e) == ENTTYPE_PLAYER)          
    {
#define MAX_STICKY_YAW RAD(90)
        Vec3 currpyr; 
        F32 backYaw;
        int feetWereStuck;

        getMat3YPR(seq->gfx_root->mat, currpyr);   
        currpyr[1] = fixAngle( currpyr[1] );

        feetWereStuck = seq->feetAreStuck; 
        seq->feetAreStuck++;    

        //////////////////// Figure out if you are currently stuck
        //Turn off stuck feet if you are moving
        if( !nearSameVec3Tol(seq->gfx_root->mat[3], e->posLastFrame, 0.01 ) ) 
            seq->feetAreStuck = 0;

        //########### New
        //If you've been turned for a while and you are in an action (non predictable move, start swiveling the feet
        if( 0 && feetWereStuck > 10 )//&& !TSTB(seq->info->moves[e->move_idx]->flags,SEQMOVE_PREDICTABLE) )
        {
            seq->stuckFeetPyr[1] = fixAngle( seq->stuckFeetPyr[1] );

        }

        //###### End NEw

        backYaw = subAngle(seq->stuckFeetPyr[1], currpyr[1]) ;
        backYaw = fixAngle(backYaw);

        //Turn off stuck feet if you turn more than 90 degrees
        if( !( -MAX_STICKY_YAW < backYaw && backYaw < MAX_STICKY_YAW ) )
            seq->feetAreStuck = 0;

        ///////////////////If you just got stuck, copy down your stuck pyr
        if( !feetWereStuck && seq->feetAreStuck  )
            copyVec3( currpyr, seq->stuckFeetPyr  );


        ///////////////////If you are still coming out of being stuck, interpolate back to currpyr
        if( !seq->feetAreStuck && backYaw  )   
        {
            if( 0 )
            {
                F32 removeYaw;
                removeYaw = TIMESTEP * 0.1;
                if( removeYaw > ABS(backYaw) )
                    backYaw = 0;
                else
                    backYaw -= (removeYaw * (backYaw/ABS(backYaw) ) );

                subAngle(seq->stuckFeetPyr[1], backYaw);
            }
            else
            {
                backYaw = 0;
                copyVec3( currpyr, seq->stuckFeetPyr  );
            }
        }

        /////////////////If you have any stuckFeetPyr, spread it over the head and chest
        //xyprintf( 10, 10, "currPyr      %f %f %f", currpyr[0], currpyr[1], currpyr[2] );
        //xyprintf( 10, 11, "stuckFeetPyr %f %f %f", seq->stuckFeetPyr[0], seq->stuckFeetPyr[1], seq->stuckFeetPyr[2] );
        //xyprintf( 10, 12, "backYaw %f", backYaw );    
        if( seq->stuckFeetPyr[1] != currpyr[1] )   
        {
            F32 headPercent;
            F32 chestPercent;
            F32 percentTurn;   
            //createMat3YPR(seq->gfx_root->mat, seq->stuckFeetPyr);  
            yawMat3(fixAngle(backYaw), seq->gfx_root->mat); 
            percentTurn = ABS(backYaw/MAX_STICKY_YAW);

            if( 1 || (seq->info->moves[e->move_idx]->flags & SEQMOVE_PREDICTABLE) )
                headPercent = 0.33 + ((1.0-percentTurn)*0.66);   
            else
                headPercent = 0;

            chestPercent = (1-headPercent);

            seq->chestYaw = -backYaw * chestPercent;     //gives head more turn early on
            seq->headYaw =  -backYaw * headPercent;  

            //xyprintf( 10, 14, "headPercent   %f", headPercent );
            //xyprintf( 10, 15, "seq->headYaw  %f", seq->headYaw );
            //xyprintf( 10, 16, "chestPercent %f", chestPercent );    
            //xyprintf( 10, 17, "seq->chestYaw %f", seq->chestYaw );
        }
        else
        {

        }

    }
    else
    {
        //copyVec3( currpyr, seq->stuckFeetPyr  );
        seq->chestYaw = 0;      
        seq->headYaw = 0;  
        seq->feetAreStuck = 0;
    }
}


static bool isEntityInVisibleTray( Entity * e )
{
    SeqInst * seq = e->seq;

    if (game_state.game_mode == SHOW_GAME && !e->ragdoll )
    { 
        Vec3 trayVisCheckPos;
        copyVec3( ENTPOS(e), trayVisCheckPos );
        trayVisCheckPos[1] += 1.1;  //to do : use ent's mat to xform this?

        assert( seq->gfx_root->seqGfxData );

        if( !nearSameVec3Tol( e->seq->posTrayWasLastChecked, ENTPOS(e), 0.001 ) )
        {
            copyVec3( ENTPOS(e), e->seq->posTrayWasLastChecked );

            PERFINFO_AUTO_START("trayUpdate", 1);
            seq->gfx_root->seqGfxData->tray = groupFindInside( trayVisCheckPos, FINDINSIDE_TRAYONLY, 0, 0 );
            PERFINFO_AUTO_STOP();

            /*if( p != seq->seqGfxData.tray )
            {
            if(    ENTTYPE(e) != ENTTYPE_DOOR &&
            ENTTYPE(e) != ENTTYPE_CAR &&
            ENTTYPE(e) != ENTTYPE_NPC)
            assert(p == seq->seqGfxData.tray );

            }*/
        }

#ifndef TEST_CLIENT
        if (!vistrayIsVisible(seq->gfx_root->seqGfxData->tray) && !editMode())
        {
            return false;
        }
#endif
    }

    return true;

    //Don't do the tray check when you are not in the game.  It doesn't work right. 
}

bool isEntityIntentionallyHidden( Entity * e )
{
    int hidden = 0;

    //Is the entity or seq explicitly hidden (ez check)
    if((game_state.ent_debug_client & 4) || ENTHIDE(e) || e->noDrawOnClient || ( getMoveFlags(e->seq, SEQMOVE_HIDE) ) )
        hidden = 1;

    // keep npcs and other players from drawing in the shell menus
    if( game_state.game_mode == SHOW_TEMPLATE && !e->showInMenu && e != playerPtrForShell(0) )
        hidden = 1;

    return hidden;
}

static bool isEntityOnCamera( Entity * e, F32 ent_dist_from_player, Vec3 gfx_pos_vs_camera, bool isPlayer, int * isOccluded )
{
    SeqInst * seq = e->seq;
    bool visible = false;
    int groupDefIsVisible = 0;

    *isOccluded = 0;

    //////// Check if Player (Player doesn't do frustum or occlusion checks, he's always treated as visible if not explicitly hidden )
    if( isPlayer )
    {
        visible = true;
        return visible;
    }

    if (game_state.game_mode == SHOW_TEMPLATE && e->showInMenu)
    {
        visible = true;
        return visible;
    }

    //////// Check Distance
    
#if !defined(TEST_CLIENT)
    // Viewports may only want characters within a limited radius of the player or the character (e.g. reflections)
    if( ent_dist_from_player > gfx_state.vis_limit_entity_dist_from_player ) 
    {
        visible = false;
        return visible;
    }
#endif

    // (Is this ent's pos too far away to be seen? (vs player's position, not camera -- it feels better ))
    if( ent_dist_from_player > seq->type->fade[1] ) 
    {
        visible = false;
        return visible;
    }

    /////// Check Tray
    if( visible )
    {
        visible = isEntityInVisibleTray( e );
        return visible;
    }

    ////// Check View Frustum and Occlusion 
    ///If any of the Entities graphics or its capsule are visible, it's visible

    //Check Library Piece
    if( isGroupDefVisible( seq->worldGroupPtr, seq->gfx_root->mat ) )
        visible = true;

    //Check Animation If not, check the animation and the capsule
    if( !visible ) 
    {
        int clip;
        int animVisible = 1;
        F32 animRadius;

        //Old Vissphere radius calc -- a little clunky, but I don't want to change it
        animRadius = seq->type->vissphereradius * seq->currgeomscale[0] * 1.1f; //If the enttype is specific, use that

        //// Is the Animation visible? 
        clip = testSphereVisibility( gfx_pos_vs_camera, animRadius );
        if (!clip )
        {
            animVisible = 0;
        }
#ifndef TEST_CLIENT
        else if (game_state.zocclusion && clip && !((editMode() || game_state.see_everything & 1) && !edit_state.showvis))
        {
            if (!zoTestSphere(gfx_pos_vs_camera, animRadius, clip & CLIP_NEAR))
            {
                *isOccluded = 1;
                animVisible = 0;
            }
        }
#endif
        if( animVisible )
            visible = true;
    }

    //Check Capsule
    //TO DO : maybe only do this check if there's reason to think this is much different from animation check
    if( !visible )
    {
        Mat4 collMat;
        EntityCapsule cap;
        Vec3 p;
        Vec3 capCenter;
        F32 capsuleRadius;
        Vec3 capsule_center_pos_vs_camera;
        int capsuleVisible = 1;
        int clip;

        //Get Capsule
        getCapsule(e->seq, &cap ); 
        positionCapsule( ENTMAT(e), &cap, collMat ); 

        //Find the center of the Entity's capsule
        p[0] = 0;
        p[1] = cap.length/2.0;
        p[2] = 0;
        mulVecMat4( p, collMat, capCenter );

        capsuleRadius =( cap.length/2.0 + cap.radius );

        mulVecMat4( capCenter, cam_info.viewmat, capsule_center_pos_vs_camera );

        clip = testSphereVisibility( capsule_center_pos_vs_camera, capsuleRadius );

        //If the Capsule isn't visible either, then you can't see it
        if (!clip )
        {
            capsuleVisible = 0;
        }
#ifndef TEST_CLIENT
        else if (game_state.zocclusion && clip && !((editMode() || game_state.see_everything & 1) && !edit_state.showvis))
        {
            if (!zoTestSphere(capsule_center_pos_vs_camera, capsuleRadius, clip & CLIP_NEAR))
            {
                capsuleVisible = 0;
                *isOccluded = 1;
            }
        }
#endif
        if( capsuleVisible )
            visible = true;
    }


    return visible;
}


static F32 setEntityAlpha( Entity * e, F32 ent_dist_from_player )
{
    SeqInst * seq = e->seq;
    F32 alpha;
    //Set alpha
    {
        if (game_state.game_mode == SHOW_TEMPLATE)
            alpha = 1.f;
        else
        {
            alpha = 1.f - ( (ent_dist_from_player - seq->type->fade[0]) / (seq->type->fade[1] - seq->type->fade[0]) );
            alpha = MINMAX(alpha,0,1);
        }
        entSetAlpha( e, (int)(alpha * 255), SET_BY_DISTANCE ); 
    }

    entSetAlpha( e, (int)(e->curr_xlucency * 255), SET_BY_SERVER );
    entSetAlpha( e, e->currfade, SET_BY_BIRTH_OR_DEATH ); 

    alpha = 255; 
    alpha = MIN(alpha, seq->alphadist);
    alpha = MIN(alpha, seq->alphacam);
    alpha = MIN(alpha, seq->alphasvr);
    alpha = MIN(alpha, seq->alphafade);
    alpha = MIN(alpha, seq->alphafx);
    alpha = MIN(alpha, seq->type->maxAlpha);

    if( seq->type->reverseFadeOutDist )
    {
        F32 rfd_alpha;

        if( ent_dist_from_player < seq->type->reverseFadeOutDist )
        {
            rfd_alpha = 255.0 - ((seq->type->reverseFadeOutDist - ent_dist_from_player) * 10.0);
            alpha = MIN(alpha, rfd_alpha);
            //xyprintf(50, 50, "RFD_alpha: %d  Alpha %d EntDist %2.2f", rfd_alpha, (int)alpha, ent_dist);
        }
    }
    alpha = MAX(alpha, 0);

    if( 0 && e == playerPtr() )
    {
        int y = 10;
        xyprintf( 10, y++, "seq->alphacam      %d", seq->alphacam );
        xyprintf( 10, y++, "seq->alphadist     %d", seq->alphadist );
        xyprintf( 10, y++, "seq->alphasvr      %d", seq->alphasvr );
        xyprintf( 10, y++, "seq->alphafade     %d", seq->alphafade );
        xyprintf( 10, y++, "seq->alphafx       %d", seq->alphafx );
        xyprintf( 10, y++, "seq->type->maxAlpha %d", seq->type->maxAlpha );
        xyprintf( 10, y++, "seq->type->reverseFadeOutDist %f", seq->type->reverseFadeOutDist);
    }

    return alpha;
}



static void doEntityRagdollFrames( Entity * e, Mat4 mOldSeqRoot )
{
#ifdef RAGDOLL
    if ( e->ragdollFramesLeft > 0 )
    {
        Vec3 vObjectSpaceHipsPos;
        F32 fRatio;// = 1.0f - (F32)e->ragdollFramesLeft / (F32)e->maxRagdollFramesLeft;
        Ragdoll* pTargetRagdoll = nwCreateRagdollNoPhysics( e );
        F32 fTimeleftRatio = CLAMP(
            1.0f - e->ragdollFramesLeft / e->maxRagdollFramesLeft,
            0.0f,
            1.0f
            );


        fRatio = 0.05f + (fTimeleftRatio*fTimeleftRatio*0.95f);
        // interp seq root
        {
            Quat qOldSeqRoot;
            Quat qNewSeqRoot;
            Quat qInterpSeqRoot;
            Vec3 vOldSeqRoot;
            Vec3 vNewSeqRoot;
            Vec3 vInterpSeqRoot;
            Vec3 vScale;
            extractScale(mOldSeqRoot, vScale); // throw this scale away
            extractScale(e->seq->gfx_root->mat, vScale);
            mat3ToQuat(mOldSeqRoot, qOldSeqRoot);
            mat3ToQuat(e->seq->gfx_root->mat, qNewSeqRoot);
            quatNormalize(qOldSeqRoot);
            quatForceWPositive(qOldSeqRoot);
            quatNormalize(qNewSeqRoot);
            quatForceWPositive(qNewSeqRoot);
            copyVec3(mOldSeqRoot[3], vOldSeqRoot);
            copyVec3(e->seq->gfx_root->mat[3], vNewSeqRoot);
            quatInterp(fRatio, qOldSeqRoot, qNewSeqRoot, qInterpSeqRoot);
            quatForceWPositive(qInterpSeqRoot);
            //printf("Old = ");
            //printQuat(qOldSeqRoot);
            //printf("    New = ");
            //printQuat(qNewSeqRoot);
            //printf("\n");
            posInterp(fRatio, vOldSeqRoot, vNewSeqRoot, vInterpSeqRoot);
            quatToMat(qInterpSeqRoot, e->seq->gfx_root->mat);
            copyVec3(vInterpSeqRoot, e->seq->gfx_root->mat[3]);
            scaleMat3Vec3(e->seq->gfx_root->mat, vScale);
        }
        interpolateNonRagdollSkeleton(fRatio, e->seq, e->seq->gfx_root->child, e->nonRagdollBoneRecord);
        if(interpBetweenTwoRagdolls(e->ragdoll, pTargetRagdoll, e->ragdoll, fRatio))
        {
            copyVec3(e->seq->gfx_root->child->mat[3], vObjectSpaceHipsPos);
            setSkeletonFromRagdoll(e->ragdoll, e->seq, e->seq->gfx_root->child);
            posInterp(fRatio, e->ragdollHipBonePos, vObjectSpaceHipsPos, e->seq->gfx_root->child->mat[3]);
            copyVec3(e->seq->gfx_root->child->mat[3], e->ragdollHipBonePos);
            e->ragdollFramesLeft -= TIMESTEP/30.0f;
        }
        else
            e->ragdollFramesLeft = 0;

        nwDeleteRagdoll(pTargetRagdoll);
        if ( e->ragdollFramesLeft <= 0 )// cleanup
        {
            nwDeleteRagdoll(e->ragdoll);
            e->ragdoll = NULL;
            free(e->ragdollCompressed);
            e->ragdollCompressed = NULL;
            SAFE_FREE(e->nonRagdollBoneRecord);
        }
    }
#endif
}


static void manageEntitySimpleSplatShadows( Entity * e, F32 gfx_dist_from_camera )
{
    SeqInst * seq = e->seq;

    PERFINFO_AUTO_START("SimpleShadows",1);
    //Manage Simple Splat Shadows
    if( seq->type->shadowType == SEQ_SPLAT_SHADOW && !game_state.disableSimpleShadows && game_state.game_mode == SHOW_GAME)
    {
        //Trick to fade out Shadow on dead guys.
        if( getMoveFlags( seq, SEQMOVE_NOCOLLISION ) )   
        {
            if( seq->maxDeathAlpha > 0.0 )
                seq->maxDeathAlpha -= DEATH_SHADOW_FADE_RATE * TIMESTEP;
        }
        else if( seq->maxDeathAlpha < 1.0 )
            seq->maxDeathAlpha += DEATH_SHADOW_FADE_RATE * TIMESTEP;
        seq->maxDeathAlpha = MINMAX( seq->maxDeathAlpha, 0, 1.0 ); 
        //End Death trick

        //xyprintf( 20, 20, "%f", seq->maxDeathAlpha );
        PERFINFO_AUTO_START("updateSimpleShadow",1);

        updateSimpleShadow( seq, gfx_dist_from_camera, seq->maxDeathAlpha );
        //TRAYVIS trick, not terribly elegant
        if( seq->simpleShadow )
        {
            seq->simpleShadow->anim_id = 199; //Says I'm a shadow node
            seq->simpleShadow->seqGfxData = &seq->seqGfxData;
        }

        PERFINFO_AUTO_STOP();
    }
    else if( seq->simpleShadow )
    {
        PERFINFO_AUTO_START("gfxTreeDelete",1);
        gfxTreeDelete( seq->simpleShadow );
        PERFINFO_AUTO_STOP();
        seq->simpleShadow = 0;
        seq->simpleShadowUniqueId = 0;
    }
    //End Manage Simple Splat Shadows
    PERFINFO_AUTO_STOP();
}


static void printEntClientUpdateDebugInfo( Entity * e )
{
    SeqInst * seq = e->seq;

    //##### Debug #######################
    if( ( game_state.checkcostume == 1 && e == current_target ) ||
        ( game_state.checkcostume == 2 && e == playerPtr() ) )
    {
        if (game_state.checkcostume == 2 && game_state.game_mode == SHOW_TEMPLATE)
            printScreenSeq(playerPtrForShell(0)->seq);
        else
            printScreenSeq(seq);
    }

    if( ( game_state.checklod == 1 && e == current_target ) ||
        ( game_state.checklod == 2 && e == playerPtr() ) ) 
    {
        printLod(seq);
    }


    //if ( visible && (e == playerPtr() || strstri(seq->type->name, "Thug")) && game_state.seq_info )   
    if ( game_state.seq_info )   
    {
        static int glob_currseqdebugline = 0; //quick fix to make this debug stuff work after I ditched seq->owner
        Entity * e2 = current_target;
        char * visString = "?";

        if( seq->visStatus == ENT_VISIBLE )
            visString = "ENT_VISIBLE";
        else if( seq->visStatus == ENT_OCCLUDED )
            visString = "ENT_OCCLUDED";
        else if( seq->visStatus == ENT_NOT_IN_FRONT_OF_CAMERA )
            visString = "ENT_NOT_IN_FRONT_OF_CAMERA";
        else if( seq->visStatus == ENT_INTENTIONALLY_HIDDEN )
            visString = "ENT_INTENTIONALLY_HIDDEN";

        if( !e2 || (e2 && ( e2->seq == seq || e == playerPtr() ) ) )
        {
            int r = 255, g = 255, b = 255;

            glob_currseqdebugline++;
            if(e == playerPtr())  //added advantage that the player is always on top 
            {
                glob_currseqdebugline = 0;
                b = 0;
                if (!getMoveFlags(seq, SEQMOVE_CANTMOVE))
                    r = 0;
            }
            else if (getMoveFlags(seq, SEQMOVE_CANTMOVE))
                g = b = 0;

            xyprintfcolor(25,10 + glob_currseqdebugline *1, r, g, b,
                "%3i %10.30s: FRAME % -5.1f  SEQ: %s LOD: %d VIS: %s ALPHA: %d CURRX: %3.2f X: %3.2f TRAY: %d",
                e->owner,
                seq->type->name, 
                seq->animation.frame,
                seq->animation.move->name, 
                seq->lod_level, 
                visString,
                e->seq->seqGfxData.alpha, 
                e->curr_xlucency, 
                e->xlucency, 
                (int)e->seq->seqGfxData.tray);

            //xyprintf( 80, (10 + (int)(seq->animation.frame)), "%f", seq->animation.frame ); 

            //Print selected entity's state to the screen
            if( e == e2 ) //Debug
            {
                int allstates[MAXSTATES];
                int allcnt, i;
                seqListAllSetStates( allstates, &allcnt, seq->state );
                for(i = 0 ; i < allcnt ; i++)  
                    xyprintf(10, 14+i , "%s", seqGetStateNameFromNumber( allstates[i] ) );
            }
        }
    }
}

static void entDoStaticLighting( SeqInst * seq )
{
    if( seq->static_light_done != STATIC_LIGHT_DONE &&
        seq->static_light_done != STATIC_LIGHT_ON_CREATE_FAILED &&
        0 == animCheckForLoadingObjects( seq->gfx_root->child, seq->handle ) )
    {
        if( seqSetStaticLight(seq, 0) )
            seq->static_light_done = STATIC_LIGHT_DONE;
        else
            seq->static_light_done = STATIC_LIGHT_ON_CREATE_FAILED;
    }
}

//TO DO Move to seq
static void handleHitReactHack( SeqInst * seq )
{
    //HIT REACT HACK
    //if we were just in hit react, and this move is ready, instead go back to 
    //what you were doing before the hit react
    //TO DO use the HITREACT flag, and make sure it works for cycle moves
    if( seq->animation.lastMoveBeforeTriggeredMove && seq->animation.prev_move )
    {
        const SeqMove * newMove;

        newMove = seq->animation.move;

        //printf("A new move %s while holding %s\n", newMove->name, seq->animation.lastMoveBeforeTriggeredMove );

        //You have arrived here because the hit react fell gently through to here.

        //If we are done with the hit react, restore the pre-hit-react move then back calculate where the 
        //move whould be if the hit react hadn't happened
        //All hit reacts resolve to here one of these two types of moves
        if( strstriConst( newMove->name, "Ready" ) || strstriConst( newMove->name, "hit_cycle" ) )
        {
            int totalTicksToCover;

            //printf("Doing my trigger thang!\n");

            seqProcessClientInst( seq, 0, seq->animation.lastMoveBeforeTriggeredMove->raw.idx, 1 );

            totalTicksToCover = (int)(((F32)(ABS_TIME_SINCE(seq->animation.triggeredMoveStartTime))) / 100.0);
            totalTicksToCover = MIN( totalTicksToCover, 5 * 30 ); //Five seconds max

            while(totalTicksToCover > 0 )
            {
                seqProcessClientInst( seq, 2.0, 0, 0 );
                totalTicksToCover -= 2;
            }

            //In any event, we did our best, so quit worrying about this hit react
            seq->animation.lastMoveBeforeTriggeredMove = 0;
        }
    }
}

static void seqAddState( int * toState, const int * fromState )
{
    int i;
    for(i = 0; i < STATE_ARRAY_SIZE; i++)
    {
        toState[i] |= fromState[i];
    }
}

static void findTriggeredMoves( Entity * e )
{
    SeqInst * seq = e->seq;
    int triggermove;

    //Check any extant triggered moves
    triggermove = seqCheckMoveTriggers( seq->futuremovetrackers );  

    if(EAINRANGE(triggermove, seq->info->moves)) // Sanity check
    {
        if( seqAInterruptsB( seq->info->moves[triggermove], seq->info->moves[e->move_idx] ) )
        {
            e->move_updated |= (e->move_idx != triggermove); //moves don't interrupt themselves...
            e->move_idx = triggermove;

            //If you are in an interesting move, and you don't already have an interesting move then remember it.
            if( !seq->animation.lastMoveBeforeTriggeredMove && //don't stomp on old one
                !strstriConst( seq->animation.move->name, "hit_cycle" ) &&
                !strstriConst( seq->animation.move->name, "block" ))
            {
                //printf( "recording interesting move %s\n", seq->animation.move->name );
                seq->animation.lastMoveBeforeTriggeredMove = seq->animation.move; //HIT REACT HACK
                seq->animation.triggeredMoveStartTime = ABS_TIME;
            }

            //Debug 
            if( game_state.show_entity_state && isThisDebugEnt( seq ) && e->move_updated )
            {
                consoleSetFGColor( COLOR_BLUE | COLOR_RED | COLOR_BRIGHT );
                printf( "\nTriggered: %s ", e->seq->info->moves[e->move_idx]->name );
                consoleSetFGColor( COLOR_BLUE | COLOR_GREEN | COLOR_RED );
            }
            //End debug
        }
    }
}

static void setMoveStickyStateBits(U32* state, U32 move_bits) 
{
    if (move_bits & (NEXTMOVEBIT_RIGHTHAND | NEXTMOVEBIT_LEFTHAND | NEXTMOVEBIT_TWOHAND |
        NEXTMOVEBIT_DUALHAND | NEXTMOVEBIT_EPICRIGHTHAND))
    {
        // Ensure that there are no incompatible weapon bits set.
        seqSetState(state, 0, STATE_RIGHTHAND);
        seqSetState(state, 0, STATE_LEFTHAND);
        seqSetState(state, 0, STATE_TWOHAND);
        seqSetState(state, 0, STATE_DUALHAND);
        seqSetState(state, 0, STATE_EPICRIGHTHAND);
    }

    seqOrState(state, move_bits&NEXTMOVEBIT_RIGHTHAND, STATE_RIGHTHAND);
    seqOrState(state, move_bits&NEXTMOVEBIT_LEFTHAND, STATE_LEFTHAND);
    seqOrState(state, move_bits&NEXTMOVEBIT_TWOHAND, STATE_TWOHAND);
    seqOrState(state, move_bits&NEXTMOVEBIT_DUALHAND, STATE_DUALHAND);
    seqOrState(state, move_bits&NEXTMOVEBIT_EPICRIGHTHAND, STATE_EPICRIGHTHAND);
    seqOrState(state, move_bits&NEXTMOVEBIT_SHIELD, STATE_SHIELD);
    seqOrState(state, move_bits&NEXTMOVEBIT_HOVER, STATE_HOVER);
    seqOrState(state, move_bits&NEXTMOVEBIT_AMMO, STATE_AMMO);
    seqOrState(state, move_bits&NEXTMOVEBIT_CRYOROUNDS, STATE_CRYOROUNDS);
    seqOrState(state, move_bits&NEXTMOVEBIT_INCENDIARYROUNDS, STATE_INCENDIARYROUNDS);
    seqOrState(state, move_bits&NEXTMOVEBIT_CHEMICALROUNDS, STATE_CHEMICALROUNDS);
}

static const SeqMove * runSequencerMoveUpdate( Entity *e, int *needCostumeApply )
{
    const SeqMove * newMove = NULL;
    SeqInst * seq = e->seq;
    seqSynchCycles(seq, e->owner);

    //If something is broken and the server told you to do something you don't 
    //know how to do, go to ready.  In the past, this has always been because of bad data
    //But for some reason we really should figure out, this has been happening with 
    //the polymorph power, too    
    if( e->next_move_idx >= eaSize(&e->seq->info->moves) )
    {
        e->next_move_idx = 0;
        e->next_move_bits = 0;
        e->move_change_timer = 0;
    }

    //Run out of door has this flag so even if it comes before or after the position change, it will know to play as soon as it arrives
    if( e->move_change_timer > 0 && e->seq->info->moves[e->next_move_idx]->flags & SEQMOVE_DONOTDELAY )  
    {
        e->move_change_timer = 0;
    }

    //Set any newly received triggered moves
    if( e->triggered_move_count )  
        seqSetMoveTrigger( seq->futuremovetrackers, e->triggered_moves, &e->triggered_move_count );

    //Check if it's time to switch the net move.
    if(e->move_change_timer >= 0 && (e->next_move_idx != e->move_idx || e->next_move_bits != e->move_bits))
    {
        e->move_change_timer -= TIMESTEP;
        //if(e == playerPtr()) printf("FRAME %d next move %s @ %d in (%1.3f)\n", game_state.client_frame, e->seq->info->moves[e->next_move_idx]->name, e->net_move_change_time, e->move_change_timer);
        if(e->move_change_timer <= 0) 
        {    
            e->move_updated = 1;
            e->move_idx = e->next_move_idx;
            e->move_bits = e->next_move_bits;
        }
    }

    ///Special Trickiness/////
    //To support the whole delayed-animations so they fit with movement thing,
    //Predicted mode bits set by powers need to be delayed when a move has been sent
    //down because otherwise the predictor will send you to a move that this move might 
    //not match, and you get the hiccup as it starts one anim then a couple frames later
    //it goes to the one the server sent down.  (If we want to be extry crazy, we could check and see if 
    //the predicted move matches the sent down move, and if it does go ahead and play it, but 
    //I think the predictor will be wrong usually because it doesn't know about attack and
    //activation bits, so it wouldn't be worth it.)

    //// While move is in the hopper, wait on setting any *new* sticky bits
    if( e->move_change_timer > 0 && (e->move_idx != e->next_move_idx) )
    {
        int i; 
        //Check each bit, only play the sticky bit if it was played last frame also.
        for( i = 0 ; i < STATE_ARRAY_SIZE ; i++ )
            seq->state[i] |= seq->state_lastframe[i] & seq->stance_state[i];    
    }
    else //just do it
    {
        seqSetOutputs( seq->state, seq->stance_state );
    }
    seqClearState( seq->stance_state );
    //End Special Trickiness /////////////


    setMoveStickyStateBits(seq->state, e->move_bits);

    //If something is broken and the server told you to do something you don't 
    //know how to do, go to ready.  In the past, this has always been because of bad data
    //But for soime reason we really should figure out, this has been happening whith 
    //the polymorph power, too    
    if( e->move_idx >= eaSize(&e->seq->info->moves) )
        e->move_idx = 0;

#ifndef FINAL
    //{ F32 delta3 = distance3Squared(e->mat[3], e->posLastFrame);
    //if( delta3 > 10.0 )
    //    printf( "\nXPORT moveImIn %s frame %d, clientabs %d moved %f", e->seq->info->moves[e->move_idx]->name, game_state.client_frame, global_state.client_abs, delta3);  
    //}
    if( game_state.show_entity_state && isThisDebugEnt( seq ) && e->move_updated )  
    {
        F32 delta = distance3Squared(ENTPOS(e), e->posLastFrame);
        consoleSetFGColor( COLOR_GREEN | COLOR_RED | COLOR_BRIGHT );
        printf( "\nServer orders: %s (frame %d, clientabs %d moved %f", e->seq->info->moves[e->move_idx]->name, game_state.client_frame, global_state.client_abs, delta );

        consoleSetFGColor( COLOR_BLUE | COLOR_GREEN | COLOR_RED );
    }
#endif

    //If a new move comes down from the server, don't be trying to restore the pre hit react move
    //presumable the hit react will be interrupted by this move, and all will be well
    //TO DO make sure hit react is interruptable by literally everything...
    if( e->move_updated ) //HIT REACT HACK
    {
        //if( seq->animation.lastMoveBeforeTriggeredMove )
        //    printf( "clearing interesting move %s because I received %s from the server\n", seq->animation.lastMoveBeforeTriggeredMove->name, seq->info->moves[e->move_idx]->name ); 
        seq->animation.lastMoveBeforeTriggeredMove = 0;
    }

    //See if any of the moves the server sent down are ready to play yet (All server sent moves go through this queue 
    //and most been given a time delay by networking code and/or powers code or a wait till-some-fx-hits command)
    //Also, more dubiously, if this is a hit or block, we record the move you were in before because hits and blocks aren't
    //played on the server and we need to be able to return to the old move without the server's help
    findTriggeredMoves( e ); 

    {
        F32 animSpeedTimestep;
        int moveIsPredictable = 0; 

        //This bit of craziness is designed to solve the problem where the server chooses a predictable move, but the 
        //client is in a state where it predicting the move will result in it predicting not just the wrong move,
        //but a wrong move that is itself not predictable and cycles.  The client is now waiting for the server to tell
        //it to exit that move, but the call will never come.  TO DO move this to getSeq, or just make sure this fully 
        //solv es the problem.
        {
            const SeqMove * currmove = 0;
            const SeqMove * nextmove = 0;
            currmove = e->seq->animation.move;
            nextmove = seq->info->moves[currmove->raw.nextMove[0]]; //SEQPARSE nextmove
            assert( nextmove );
            //If the current move's nextmove is a cycling not predictable move, don't be trying to predict it.
            if(  (nextmove->flags & (SEQMOVE_CYCLE | SEQMOVE_REQINPUTS) ) && 
                !(nextmove->flags &  SEQMOVE_PREDICTABLE)                 )
            {
                moveIsPredictable = 0;
                if( game_state.show_entity_state && isThisDebugEnt( seq ) && e->move_updated )
                    printf( "## Move %s is onle of the weird moves, so I turned off predictability", nextmove->name );
            }
            else
            {
                moveIsPredictable = (seq->info->moves[e->move_idx]->flags & SEQMOVE_PREDICTABLE); 
            }
        }

        //If you are a BLOCK and you have already gone off, go into predictable mode so you can be properly interrupted by new stuff.
        if( !e->move_updated )  
        {
            if( ( testSparseBit( &seq->info->moves[e->move_idx]->raw.required, STATE_BLOCK ) && !e->move_updated ) ||
                ( testSparseBit( &seq->info->moves[e->move_idx]->raw.required, STATE_HIT   ) && !e->move_updated ) )
                moveIsPredictable = 1;
        }

        /* Debug Force you into a particular move{
        e->move_idx = seqGetMoveIdxFromName( "1st_clawspin3", seq->info );
        moveIsPredictable = 0;
        e->move_updated = 1;
        }*/ //End Debug

        assert(!e->move_updated || INRANGE0(e->move_idx, U16_MAX)); // move indexes are stored in U16s

        seq->moveRateAnimScale = seqGetMovementSpeedAnimScale( seq, ENTPOS(e), e->posLastFrame );
        animSpeedTimestep = seq->moveRateAnimScale * TIMESTEP;

        //sanity check: if the server sent you a crazy move because of out of order packets or bad data, ignore it.
        if( e->move_idx >= eaSize( &seq->info->moves ) )
            e->move_idx = 0;

#ifndef FINAL
        if( game_state.show_entity_state && isThisDebugEnt( seq ) )
            stateParse(seq, seq->state);
#endif

        if( ( e == playerPtr() && moveIsPredictable && control_state.predict && !game_state.nopredictplayerseq )
            || (game_state.game_mode == SHOW_TEMPLATE && (e == playerPtrForShell(0) || e->showInMenu))) //shellMenu() means do what CW says, not what the server tells you
        {    
#ifndef FINAL
            if( game_state.show_entity_state && isThisDebugEnt( seq ) && e->move_updated )
            {
                consoleSetFGColor( COLOR_GREEN | COLOR_RED | COLOR_BRIGHT );
                printf( "(but I'm predicting)" );
                consoleSetFGColor( COLOR_BLUE | COLOR_GREEN | COLOR_RED );
            }
#endif

            newMove = seqProcessInst( seq, animSpeedTimestep );

            seq->animation.lastMoveBeforeTriggeredMove = 0; //if anything interesting has happened here, cancel the restoration
        }
        else
        {        
#ifndef FINAL
            if( game_state.show_entity_state && isThisDebugEnt( seq ) && e->move_updated )
            {
                consoleSetFGColor( COLOR_GREEN | COLOR_RED | COLOR_BRIGHT );
                printf( "(and I'm obeying)" );
                consoleSetFGColor( COLOR_BLUE | COLOR_GREEN | COLOR_RED );
            }
#endif
            newMove = seqProcessClientInst( seq, animSpeedTimestep, e->move_idx, e->move_updated ) ;
        }

        if( newMove ) {
            handleHitReactHack( seq ); //Hack to prefer old move to ready if you just finished a hit react or Block (see findTriggeredMoves() )
            //if(strlen(e->name)) printf("runSequencerMoveUpdate: entity \"%s\" has new move \"%s\"\n", e->name, newMove->name);
#if 0 //def CLIENT // fpe for testing
            if( e->name[0] && !strstr(e->name, "Dr") )
            {                
                int allstates[MAXSTATES];
                int allcnt;
                int i,printstate = 0;

                printf("runSequencerMoveUpdate: new move '%s' for entity '%s'\n", newMove->name, e->name);

                // bunch of code to print statebits
                if( 0 )
                {
                    seqListAllSetStates( allstates, &allcnt, seq->state ); 

                    for(i = 0 ; i < allcnt ; i++)
                    {
                        consoleSetFGColor( COLOR_RED | COLOR_BLUE | COLOR_BRIGHT );           
                        printf("   %s \n", seqGetStateNameFromNumber( allstates[i] ) );  
                        consoleSetFGColor( COLOR_RED | COLOR_BLUE | COLOR_GREEN);   
                    }
                    if(!allcnt)
                    {
                        consoleSetFGColor( COLOR_RED | COLOR_BLUE | COLOR_BRIGHT );           
                        printf("   NO BITS SET \n" );  
                        consoleSetFGColor( COLOR_RED | COLOR_BLUE | COLOR_GREEN); 
                    }
                }
            }
#endif
        }

        //Reset move stuff, as it's been managed.
        e->move_idx = (int) (e->seq->animation.move->raw.idx);
        e->move_updated = 0;
        // Catch costume change emotes here.  This is where we segue out of the "pre"
        // move into the post move.  If the move has the costumechange bit set, apply
        // the pending costume change.
        if (ENTTYPE(e) == ENTTYPE_PLAYER && e->pl && e->pl->pendingCostumeNumber)
        {
            if (e->seq->animation.move->flags & SEQMOVE_COSTUMECHANGE || timerSecondsSince2000() > e->pl->pendingCostumeForceTime)
            {
                if (e == playerPtr())
                {
                    // DGNOTE 4/24/2009
                    // If someone ever figures how to trigger one of these, and then toggle SG mode before the change strikes,
                    // this is likely to break.  Very badly.
                    // Maybe look at the SG toggle code and deny them while there's a pending change.  The problem is, that doesn't
                    // fix the issue of switching in the window between issuing the command and having the pend change get back from the server.
                    // Late arriving note.  /sgmode and it's ilk are handled on the mapserver.  If we find that e->pl->pendingCostumeForceTime is
                    // non-zero, deny the request.
                    // Addition to that note.  I ought to refactor this to save the current timerSecondsSince2000() everywhere, instead of
                    // saving timerSecondsSince2000() + offset.  That would allow one variable to handle multiple timeouts, since the timeout
                    // value would be specified at the point the variable is used.  If and when I need to handle SG change problem, I'll make
                    // this change at that time.
                    entSetMutableCostumeUnowned( e, e->pl->pendingCostume ); // e->costume = costume_as_const(e->pl->pendingCostume);
                    if (e->pl->pendingCostumeNumber & 256)
                    {
                        costume_destroy(e->pl->superCostume);
                        e->pl->superCostume = e->pl->pendingCostume;
                    }
                    e->pl->current_powerCust = e->pl->current_costume = (e->pl->pendingCostumeNumber & 255) - 1;
                }
                else
                {
                    costume_destroy(costume_as_mutable_cast(e->costume));
                    e->costume = costume_as_const(e->pl->pendingCostume);;
                    e->pl->costume[e->pl->current_costume] = e->pl->pendingCostume;
                }
                e->pl->pendingCostumeForceTime = 0;
                e->pl->pendingCostumeNumber = 0;
                e->pl->pendingCostume = NULL;
                // I was originally applying the costume here.  That caused all sorts of hell because, under certain edge cases, it could
                // replace the sequencer.  That in turn left entClientUpdate(...) with a dangling pointer, which more often than not
                // resulted in a client crash.
                // Rather than keep on applying kluge after kluge after kluge, the obvious solution is "Don't apply the costume here".
                // Instead I set a flag saying we have to do so, and then actually apply the costume in entClientUpdate(...) after the
                // seq variable is no longer needed.
                if (needCostumeApply)
                {
                    *needCostumeApply = 1;
                }
            }
        }
    }

    return newMove;
}

//You have to be off camera for a time before you aren't drawn -- 
//otherwise the occluder's jittering is intolerable
static bool doOccluderDelay( Entity * e, bool visible, int isOccluded )
{
    bool newVisible = visible;
    SeqInst * seq = e->seq;

    #define FRAMES_ENTITY_MUST_BE_OCCLUDED_BEFORE_WE_STOP_DRAWING_HIM 30.0

    if( isOccluded )
    {
        //If you've been on camera before and you have only been briefly occluded, you aren't occluded
        //Otherwise 
        if( seq->hasBeenOnCamera && seq->framesOccluded < FRAMES_ENTITY_MUST_BE_OCCLUDED_BEFORE_WE_STOP_DRAWING_HIM )
            newVisible = true;

        seq->framesOccluded += TIMESTEP;
    }
    else
    {
        seq->framesOccluded = 0;
    }

    if( visible )
        seq->hasBeenOnCamera = 1;

    return newVisible;
}

static eVisStatus setEntityVisibility( Entity * e, F32 ent_dist_from_player, Vec3 gfx_pos_vs_camera, bool isPlayer ) 
{
    eVisStatus visStatus = ENT_VISIBLE;
    bool visible = ENT_VISIBLE;

    if( isEntityIntentionallyHidden( e ) )
        visStatus = ENT_INTENTIONALLY_HIDDEN;
    
    if( visStatus == ENT_VISIBLE )
    {
        int isOccluded = 0;
        bool isVisible = 0;
        isVisible = isEntityOnCamera( e, ent_dist_from_player, gfx_pos_vs_camera, isPlayer, &isOccluded );

        ///To prevent occluder jitter, you must be occluded for a time before you aren't drawn
        if (game_state.zocclusion)
            isVisible = doOccluderDelay( e, isVisible, isOccluded );
        
        if( !isVisible )
        {
            if( isOccluded )
                visStatus = ENT_OCCLUDED;
            else
                visStatus = ENT_NOT_IN_FRONT_OF_CAMERA; //Includes too far away
        }
    }

    if( visStatus == ENT_VISIBLE )
    {
        e->seq->gfx_root->flags &= ~GFXNODE_HIDE;
        if(e->seq->worldGroupPtr)
        {
            e->seq->worldGroupPtr->hideForHeadshot = 0;
        }
    }
    else //if(visStatus != ENT_INTENTIONALLY_HIDDEN)
    {
        e->seq->gfx_root->flags |= GFXNODE_HIDE;
        if(e->seq->worldGroupPtr)
        {
            e->seq->worldGroupPtr->hideForHeadshot = 1;
        }
    }

    return visStatus;
}

//This might have been tuned down to a total waste of time
static void addYSpringToCharacterGraphics( SeqInst * seq )      
{
#define MAX_SPRING_LENGTH 1.0
#define MAX_LATERAL_MOVE_DIST_TO_APPLY_Y_SPRING 2.0
    F32 tgt = seq->gfx_root->mat[3][1];
    F32 curr = seq->posLastFrame[1];
    F32 realDelta;
    F32 idealDelta;
    F32 ySpringScale;

    idealDelta = tgt - curr;      

    {
        Vec3 t;
        F32 lateralMoveDist;

        subVec3( seq->gfx_root->mat[3], seq->posLastFrame, t );
        t[1] = 0;
        lateralMoveDist = lengthVec3(t);
        ySpringScale = 1.0 - (lateralMoveDist / (MAX_LATERAL_MOVE_DIST_TO_APPLY_Y_SPRING*TIMESTEP));
        ySpringScale = MAX( 0, ySpringScale );

        //xyprintf( 10, 10, "lateralMoveDist %f", lateralMoveDist );
        //xyprintf( 10, 11, "ySpringScale    %f", ySpringScale );
    }

    if( idealDelta > 0 )
    {
        realDelta = idealDelta * 0.33 * TIMESTEP; 

        // Clamp real delta to ideal delta in case timestep is large.
        if(fabs(realDelta) > fabs(idealDelta))
            realDelta = idealDelta;

        if( realDelta > 0.0 )
            realDelta = MAX( realDelta, idealDelta - (MAX_SPRING_LENGTH * ySpringScale) );

        if( realDelta < 0.0 )
            realDelta = MIN( realDelta, idealDelta + (MAX_SPRING_LENGTH  * ySpringScale) );

        curr = curr + realDelta;

        seq->gfx_root->mat[3][1] = curr;
    }
}

static void entClientUpdate(Entity *e)
{
    SeqInst * seq = e->seq;

    F32 ent_dist_from_player;        //distance from entity to player

    int        isNewMove;
    int needCostumeApply;
    Entity * player;

    ////// Some calculations are done in relation to the player's position (this can cause cutScene problems)
    player = playerPtr();
    if (game_state.game_mode == SHOW_TEMPLATE && e == playerPtrForShell(1))
        player = playerPtrForShell(1);

    ////// Call this every frame to twiddle with fade settings for stealth
    isEntitySelectable(e);

    ////// Fading In and Out
    handleEntityFading( e ); //Fade in or fade out
    if(e->waitingForFadeOut && 0 == e->currfade)
    {
        entFree(e);
        return;
    }

    g_EntsUpdatedByClient++; //Debug count

    if (!seq)
        return;

PERFINFO_AUTO_START("Top of Ent Client Update",1);

    ////// Set GFX_ROOT mat (pos, orientation and scale)
    copyMat4(seq->gfx_root->mat, seq->oldSeqRoot);
    if(ENTTYPE(e) == ENTTYPE_PLAYER)    
    {
        Mat4 matx;
        copyMat4(ENTMAT(e), matx);
        matx[3][0] += matx[1][0] * -5;
        matx[3][1] += matx[1][1] * -5 + 5;
        matx[3][2] += matx[1][2] * -5;
        copyMat4(matx, seq->gfx_root->mat);
    }
    else
    {
        copyMat4(ENTMAT(e), seq->gfx_root->mat);
    }
    scaleMat3Vec3( seq->gfx_root->mat, seq->currgeomscale ); 
#ifndef TEST_CLIENT
    if(seq->legScale) // legscale is applied to root node, do this once here
        scaleBone(seq->gfx_root, 1, (1.0 + (seq->type->legScaleRatio * seq->legScale)), 1);
#endif

    ////// STICKING FEET (currently unused (previously cutscene only))
    // commenting this out as it apparently is not used for anything useful and is causing the following bugs:
    //    COH-16334, COH-21327, COH-25016
    //handleStickingFeet( e );

    ////// SETBACK Move the entity back a little so his animations fit better into his capsule (ugly)
    if( seq->type->placement == SEQ_PLACE_SETBACK && isMenu( MENU_GAME ) )
        scaleAddVec3(seq->gfx_root->mat[2], -COMBATBIAS_DISTANCE, seq->gfx_root->mat[3], seq->gfx_root->mat[3]);

    ////// HEAD TURN currently unused
    //calcHeadTurn(seq); //Part of Stuckfeet but must be done after setback

    /*if(!game_state.test7)*/
    {
    //Cushion Y changes
    addYSpringToCharacterGraphics( seq );// Adds a little spring to the gfx_root->mat[3][1] to ease you character going over little rocks 
    }

    ////// Record Position Last Frame for Run Animation scaling
    copyVec3( seq->gfx_root->mat[3], seq->posLastFrame ); 
        
    ////// Surface calculation
    calcSurfaceEntityIsStandingOn(e); 

    ////// Volume Check (Set volume fx) TO DO (may be wrong to use visibility cuz of sound)
    handleEntityVolumeChecks( e );

    ////// Reticle Interpolation Upkeep
    seq->reticleInterpFrame -= TIMESTEP;
    seq->selectionUsedLastFrame = seq->selectionUsedThisFrame;

    ////// Some Useful Calculations used throughout function
    // ent_dist_from_player
    {
        Vec3 ent_pos_vs_player;            //position of this entity relative to the player entity
        Entity* followEnt = erGetEnt(game_state.camera_follow_entity);
        const F32* vispos = followEnt ? ENTPOS(followEnt) : ENTPOS(player);

        if( game_state.viewCutScene )
            vispos = game_state.cutSceneCameraMat[3];

        subVec3( seq->gfx_root->mat[3], vispos, ent_pos_vs_player );
        ent_dist_from_player = lengthVec3( ent_pos_vs_player );
    }

#if NOVODEX
#ifndef TEST_CLIENT
    //if ( !nx_state.hardware || 1)
    entUpdateNovodex(e, ent_dist_from_player);
#endif
#endif

    ////// Set Constantly Reset States (TO DO combine with constantStates)
    if( seq->type->constantStateStr ) 
        seqAddState( seq->state, seq->type->constantState );
    if( e->favoriteWeaponAnimList )
        seqAddState( seq->state, e->favoriteWeaponAnimListState );
    if( e->aiAnimList )
        seqAddState( seq->state, e->aiAnimListState );

    if( ENTTYPE(e) == ENTTYPE_PLAYER )
        seqSetState( seq->state, 1, STATE_PLAYER );


    ///////////  Run Sequencer 
PERFINFO_AUTO_STOP_START("Sequencer",1);

    needCostumeApply = 0;
    isNewMove = runSequencerMoveUpdate( e, &needCostumeApply ) != NULL ? 1 : 0;
    seq->updated_appearance |= isNewMove;  

    /////////// Reset Graphics, and manage fx as needed
PERFINFO_AUTO_STOP_START("Anim Set Header",1);

    //Reset gfx if you just finished loading geometry (because you need bone_info from fully loaded object)
    if( seq->loadingObjects == OBJECT_LOADING_IN_PROGRESS )
    {
        seq->loadingObjects = animCheckForLoadingObjects( seq->gfx_root->child, seq->handle ); 
        if( seq->loadingObjects == OBJECT_LOADING_COMPLETE )
            seq->updated_appearance = 1;
    }

    ////// Anim Set Header
    if(seq->updated_appearance )
    {
        animSetHeader( seq, 0 );
        seq->updated_appearance = 0;
    }

    ////// Manage Fx that are triggerd by various entity events
    seqManageSeqTriggeredFx( e->seq, isNewMove, e );

PERFINFO_AUTO_STOP_START("Manage HP Triggered Changed ",1);
    manageEntityHitPointTriggeredfx( e );

    ////////// Manage Net Fx, Updating Collision Grid, Static Lighting Check
PERFINFO_AUTO_STOP_START("NetFx, Updating Collision Grid, and Static Lighting Check",1);

    ////// Generate or kill any fx as e->fxlist indicates
    entManageEntityNetFx(e);  //in this order and after PlayInst
    entCheckFxTriggers(e); 

    entDoStaticLighting( seq );

    ////// If this seq was changed by an fx attached to it, clear the changes.  The fx must refresh it's changes each frame
    seq->alphafx = 255;
    seq->fxSetGeomScale[0] = 1; 
    seq->fxSetGeomScale[1] = 1;  
    seq->fxSetGeomScale[2] = 1;  

    ////// Insert self into the Collision Grid if needed 
    if( e->seq->type->collisionType == SEQ_ENTCOLL_DOOR || e->seq->type->collisionType == SEQ_ENTCOLL_LIBRARYPIECE )
    {
        if(    !e->checked_coll_tracker || e->coll_tracker )
        {
            PERFINFO_AUTO_START("entMotionUpdateCollGrid 1",1);
            entMotionUpdateCollGrid(e);
            PERFINFO_AUTO_STOP();
        }
    }

    ////// Record Position Last Frame for Surface Check and Fade in (probably redundant with seq->posLastFrame)
    copyVec3( ENTPOS(e), e->posLastFrame );

    if (needCostumeApply)
    {
        // runSequencerMoveUpdate(...) decided it's time to apply a pending costume change.  As noted inside that routine, doing
        // a costume_Apply(...) call can change e->seq, leaving this routine with it's seq pointer dangling.  So we defer the
        // call to here, since we don't care what happens to e->seq at this point.
        Animation anim;
        int const_seqfx[MAX_SEQFX];
        int itr;
        for (itr = 0; itr < MAX_SEQFX; ++itr)
        {
            const_seqfx[itr] = e->seq->const_seqfx[itr];
            e->seq->const_seqfx[itr] = 0;
        }
        anim = e->seq->animation;
        costume_Apply(e);
        e->seq->animation = anim;
        for (itr = 0; itr < MAX_SEQFX; ++itr)
        {
            e->seq->const_seqfx[itr] = const_seqfx[itr];
        }

        // If we're demo recording, we need to record the costume change we just applied.
        if(demo_recording)
        {
            // Set the entity index, so the demorecord code knows who is changing their costume
            demoSetEntIdx(e->svr_idx);
            // And record the change
            demoRecordAppearance(e);
        }
    }

    ////// Debug info
    printEntClientUpdateDebugInfo( e );

PERFINFO_AUTO_STOP(); 
}

static void entClientUpdateVisibility(Entity *e)
{
    SeqInst * seq = e->seq;

    F32 ent_dist_from_player;        //distance from entity to player
    Vec3 gfx_pos;                    //Position of this entities graphics (can be different from entity position)
    Vec3 gfx_pos_vs_camera;            //pos of this ent's gfx relative to camera
    F32 gfx_dist_from_camera;        //distance from ent's gfx to camera
    
    Entity * player;

    if(e->waitingForFadeOut && 0 == e->currfade)
        return;

    if (!seq)
        return;

PERFINFO_AUTO_START("Top of entClientUpdateVisibility",1);

    ////// Some calculations are done in relation to the player's position (this can cause cutScene problems)
    player = playerPtr();
    if (game_state.game_mode == SHOW_TEMPLATE && e == playerPtrForShell(1))
        player = playerPtrForShell(1);

    ////// Some Useful Calculations used throughout function
    // ent_dist_from_player, gfx_pos, gfx_pos_vs_camera, gfx_dist_from_camera
    {
        Vec3 ent_pos_vs_player;            //position of this entity relative to the player entity
        Entity* followEnt = erGetEnt(game_state.camera_follow_entity);
        const F32* vispos = followEnt ? ENTPOS(followEnt) : ENTPOS(player);

        if( game_state.viewCutScene )
            vispos = game_state.cutSceneCameraMat[3];

        subVec3( seq->gfx_root->mat[3], vispos, ent_pos_vs_player );
        ent_dist_from_player = lengthVec3( ent_pos_vs_player );

        // find where the gfx are(animation gfx can theoretically be far from the ent pos)
        if ( seq->gfx_root->child )
            mulVecMat4( seq->gfx_root->child->mat[3], seq->gfx_root->mat, gfx_pos );
        else
            copyVec3( seq->gfx_root->mat[3], gfx_pos );
        mulVecMat4( gfx_pos, cam_info.viewmat, gfx_pos_vs_camera );

        gfx_dist_from_camera = lengthVec3( gfx_pos_vs_camera );
    }

    ////// Set LODs
    seqSetLod( seq, gfx_dist_from_camera ); //must be up here, or there's a flicker

    /////////// Determine Visibility
PERFINFO_AUTO_STOP_START("Test Entity Visibility and Occlusion",1);
    seq->visStatusLastFrame = seq->visStatus;
    seq->visStatus = setEntityVisibility( e, ent_dist_from_player, gfx_pos_vs_camera, ((e == player)? true : false) );

    ////////// If Visible, play animation, do lighting, alpha, ragdoll.
PERFINFO_AUTO_STOP_START("graphics",1); 
    if( seq->visStatus == ENT_VISIBLE )
    {
        g_EntsProcessedForDrawing++; //Debug

        ////// Set Alpha
        seq->seqGfxData.alpha = setEntityAlpha( e, ent_dist_from_player );

        ////// Set Light
        {
            Vec3 lightPos; 

            if( globMapLoadedLastTick ) //We've finished loading the map, and can do static lighting now
                memset( &seq->seqGfxData.light, 0, sizeof( EntLight ) );

            copyVec3( gfx_pos, lightPos );           
            lightPos[1] += 0.1;  //add a trivial amount so you're sure to be inside the tray
            lightEnt(&e->seq->seqGfxData.light, lightPos, e->seq->type->minimumambient, 
                (game_state.maxEntAmbient <= 0.0f) ? 0.2f : game_state.maxEntAmbient);
        }

        ////// Anim Set Header
        // seq->updated_appearance may have been set by seqSetLod() above
        if(seq->updated_appearance )
        {
            // Do this to apply the new LOD geometry
            animSetHeader( seq, 1 );

            seq->updated_appearance = 0;
            seq->lastFrameUpdate = -1;
        }

        if (seq->lastFrameUpdate != game_state.client_frame)
        {
            ////// Animate
            if (!e->ragdoll || !clientHasCaughtUpToRagdollStart( e->ragdoll, global_state.client_abs ) || e->ragdollFramesLeft > 0)
            {
                seqDoAnimationPitching( seq );  //Bend at Waist toward target

                animPlayInst( seq );

                if( seq->moveRateAnimScale > 1.05 && ( seq->animation.move->flags & SEQMOVE_SMOOTHSPRINT ) ) 
                    smoothSprintAnimation( seq, seq->moveRateAnimScale );  //Mild hack smooth out the sprint animations 

                doEntityRagdollFrames( e, seq->oldSeqRoot );
            }

            if (e->ragdoll && e->ragdollFramesLeft <= 0)
                updateSkeletonFromRagdoll(e);

            e->seq->bCurrentlyInRagdollMode = (e->ragdoll != NULL);

            adjustLimbs(seq);

            seq->lastFrameUpdate = game_state.client_frame;
        }

        ////// Splat Shadows
        manageEntitySimpleSplatShadows( e, gfx_dist_from_camera );
    } 
    else if( e->ragdoll )
    {
        // Hold off on running this until we reach the last viewport and we still are not visible
        if (
#if !defined(TEST_CLIENT)            
            gfx_state.renderPass == RENDERPASS_COLOR && 
#endif
            seq->lastFrameUpdate != game_state.client_frame)
        {
            ////// Ragdoll work to do even if not visible
            updateSkeletonFromRagdoll(e);

            seq->lastFrameUpdate = game_state.client_frame;
        }
    }

    ////// Record Position Last Frame for Surface Check and Fade in (probably redundant with seq->posLastFrame)
    // already have in entClientUpdate -- copyVec3( ENTPOS(e), e->posLastFrame );

    ////// Debug info
    printEntClientUpdateDebugInfo( e );

PERFINFO_AUTO_STOP(); 
}

// This should only be called once a frame to update the simulation
void entClientProcess()
{
    int        i;

    if (!playerPtr())
        return;

    PERFINFO_AUTO_START("entClientUpdate",1);

    //Get this for an obsure fading reason
    calculatePlayerMoveThisFrame(); 

    updateClothFrame();
    
    // Update the clients
    if(game_state.fixRegDbg)
    {
         xyprintf(10,20, "Shoulder Scale: %f",    playerPtr()->seq->shoulderScale);
        xyprintf(10,21, "Arm Fix %s",            (game_state.fixArmReg ? "On" : "OFF"));
         xyprintf(10,22, "Leg Fix %s",            (game_state.fixLegReg ? "On" : "OFF"));
    }
 
    for(i=1;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE )
            entClientUpdate(entities[i]);
    }
    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_START("entClientProcessOddsAndEnds",1);
    //Fx that need to, grab the state of your parents before it goes away...
    fxUpdateFxStateFromParentSeqs();
    clearMailbox(&globalEntityFxMailbox);  //all msgs from fx system read and handled, clear the mailbox

    //Clear all entities state for next time
    for(i=1;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE && entities[i]->seq ) //to do: better validating?
            seqClearState( entities[i]->seq->state );
    }
    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_START("powerInfo_UpdateTimers",1);
    // Update the power info on the player entity.
    if(playerPtr() && playerPtr()->powerInfo)
        powerInfo_UpdateTimers(playerPtr()->powerInfo, TIMESTEP / 30);
    PERFINFO_AUTO_STOP();
}

// This should be called for every viewport to update visibility
void entClientProcessVisibility()
{
    int        i;

    if (!playerPtr())
        return;

    PERFINFO_AUTO_START("entClientProcessVisibility",1);

#ifndef TEST_CLIENT
    s_entity_do_shadowmap_culling = gfx_state.renderPass >= RENDERPASS_SHADOW_CASCADE1 && gfx_state.renderPass <= RENDERPASS_SHADOW_CASCADE_LAST &&
                                    game_state.shadowDebugFlags & kShadowCapsuleCull;
#else
    s_entity_do_shadowmap_culling = false;
#endif

    for(i=1;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE )
            entClientUpdateVisibility(entities[i]);
    }

    PERFINFO_AUTO_STOP();
}

Entity *entCreate(char * type)
{
Entity    *e;
Vec3    crit_pos = {0,0,0}, crit_pyr = {0,0,0};
int        fly = 0;
Mat3    newmat;

    type = 0; //is now unused
    e = entAlloc();
    entSetMat3(e, unitmat);
    entUpdatePosInterpolated(e, crit_pos);
    if (fly)
        e->move_type = MOVETYPE_NOCOLL;
    else
        e->move_type = MOVETYPE_WALK;
    e->xlucency = 1.0;
    createMat3RYP(newmat,crit_pyr);
    entSetMat3(e, newmat);

    return e;
}

void entSetAlpha( Entity * e, int alpha, int type )
{
    if( e && e->seq)
    {
        if(type == SET_BY_CAMERA)
            e->seq->alphacam = alpha;
        else if(type == SET_BY_DISTANCE)
            e->seq->alphadist = alpha;
        else if(type == SET_BY_SERVER)
            e->seq->alphasvr = alpha;
        else if(type == SET_BY_BIRTH_OR_DEATH)
            e->seq->alphafade = alpha;
        else if(type == SET_BY_FX)
            e->seq->alphafx = alpha;
    }
}


void changeBody(Entity * e, const char * type)
{
    //Debug
    if( !type )
    {
        if( isDevelopmentMode() )
            Errorf("Bad call to changeBody");
        return;
    }
    if( !e )
    {
        if( isDevelopmentMode() )
            Errorf( "CUSTOM BODY ERROR: changeBody %s called on garbage entity", type);
        return;
    }
    //End Debug

    // Is the current body the same as the requested body (and the body isn't dynamic)?
    // If so, don't do anything else because we already have the correct body.
    // Otherwise ditch the old seq, and assign the new onw
    if( e->seq )
    {
        if(stricmp(e->seq->type->name, type) == 0 
            && !( e->seq->type->flags & SEQ_USE_DYNAMIC_LIBRARY_PIECE ) )
            return;
    }
    //doDelAllCostumeFx(e, 0);
    entSetSeq(e, seqLoadInst( type, 0, SEQ_LOAD_FULL, e->randSeed, e ));

    //Debug
    if(!e->seq && isDevelopmentMode() )
        Errorf( "CUSTOM BODY ERROR: bad body name %s", type);
    //End Debug
}

int changeGeo(Entity * entity, int bone, char newgeo[], int warn)
{
    //Debug Checking
    int    good = TRUE;
    char * none = "NONE";
    char filepath[300];
    char buf[128], *geometry;
    char * file = NULL;

    if(!entity || !entity->seq)
    {
        if( isDevelopmentMode() )
            Errorf( "changeGeo called on Garbage Entity! %s", newgeo );
        return FALSE;
    }

    if(stricmp(newgeo, "NONE"))
    {
        //sprintf(buf, "%s", newgeo );
        strcpy(buf, newgeo);
        file = strtok(buf, ".");                            //filename
        //sprintf(filepath, "player_library/%s.geo", file);
        STR_COMBINE_BEGIN(filepath);
        STR_COMBINE_CAT("player_library/");
        STR_COMBINE_CAT(file);
        STR_COMBINE_CAT(".geo");
        STR_COMBINE_END();
        geometry = strtok(NULL, ".");                        //geometry name
        if(!modelFind( geometry, filepath, LOAD_BACKGROUND, GEO_INIT_FOR_DRAWING | GEO_USED_BY_GFXTREE))
        {
            if( strstri( geometry, "Larm") ) //E3 hack since it's ok to have
            {
                geometry = none;
                strcpy(filepath,"");
                good = TRUE;
            }
            else
            {
                if( isDevelopmentMode() && warn )
                {
                    Errorf( "BAD DATA: Custom Geometry %s in %s doesn't exist!", geometry, filepath );
                }
                good = FALSE;
            }
        }
    }
    else
    {
        geometry = none;
        strcpy(filepath,"");
        good = TRUE; //None is always good
    }
    assert(bone_IdIsValid(bone));
    //End Debug

    if(entity && entity->seq && bone_IdIsValid(bone))
    {
        if(    !entity->seq->newgeometry[bone] || !entity->seq->newgeometryfile[bone] ||
            _stricmp(entity->seq->newgeometry[bone], geometry) != 0 ||
            _stricmp(entity->seq->newgeometryfile[bone], filepath) != 0 ) // this used to compare to 'file', which I assume is wrong
        {
            entity->seq->newgeometryfile[bone] = filepath[0] ? allocAddString(filepath) : NULL;
            entity->seq->newgeometry[bone] = allocAddString(geometry);
            entity->seq->updated_appearance = 1;
        }
        if(vec3IsZero(entity->seq->seqGfxData.shieldpos) && vec3IsZero(entity->seq->seqGfxData.shieldpyr))
        {
            costume_getShieldOffset(newgeo, entity->seq->seqGfxData.shieldpos, entity->seq->seqGfxData.shieldpyr);
        }
        else
        {
            Vec3 pos = {0}, pyr = {0};
            costume_getShieldOffset(newgeo, pos, pyr);
            if(    (!vec3IsZero(pos) || !vec3IsZero(pyr)) &&
                (!sameVec3(pos, entity->seq->seqGfxData.shieldpos) || !sameVec3(pyr, entity->seq->seqGfxData.shieldpyr)) )
            {
                Errorf("Conflicting shield offsets in costume geodata");
            }
        }
    }

    return good;
}

void changeTexture(Entity * e, BoneId bone, char newtexture1[], char newtexture2[])
{
    TexBind * texture1=0;
    SeqInst * seq;

    if(!e || !e->seq)
    {
        if( isDevelopmentMode() )
            Errorf("CUSTOM TEXTURE ERROR: changeTexture called on garbage entity");
        return;
    }
    assert( bone_IdIsValid(bone) );

    seq = e->seq;

    ///#### Texture 1 ########
    if(    newtexture1 )
    {
        if( _stricmp(newtexture1, "none") )
        {
            texture1 = texLoad(newtexture1,TEX_LOAD_IN_BACKGROUND, TEX_FOR_ENTITY);
            if(!texture1 || (texture1 && texture1==white_tex_bind && !strstri( newtexture1, "white" ) ) )
            {
                if( isDevelopmentMode() && !quickload )
                {
                    Errorf( "CUSTOM TEXTURE ERROR: %s wants nonexistent texture %s as texture 1 on bone %s.",
                            e->seq->type->name,
                            newtexture1,
                            bone_NameFromId(bone) );
                }
            }
        }
        else //"None" is the same as "white"
        {
            texture1 = white_tex_bind;
        }
        seq->newtextures[bone].base = texture1;
    } else {
        seq->newtextures[bone].base = NULL;
    }

    ///#### Texture 2 ########
    if(    newtexture2 ) // && (!texture1 || texture1->bind_blend_mode.shader != BLENDMODE_MULTI) ) // We ignore texture 2 specifications for Multi9 textures
    {
        BasicTexture * texture=0;
        if( _stricmp(newtexture2, "none") )
        {
            texture = texLoadBasic(newtexture2,TEX_LOAD_IN_BACKGROUND, TEX_FOR_ENTITY);
            if(!texture || (texture && texture==white_tex && !strstri( newtexture2, "white" ) ))
            {
                if( isDevelopmentMode() && !quickload )
                {
                    Errorf( "CUSTOM TEXTURE ERROR: %s wants nonexistent texture %s as texture 2 on bone %s.",
                            e->seq->type->name,
                            newtexture2,
                            bone_NameFromId(bone) );
                }
            }
        }
        else //"None" is the same as "white"
        {
            texture = white_tex;
        }
        seq->newtextures[bone].generic = texture;
    } else {
        seq->newtextures[bone].generic = NULL;
    }

    seq->updated_appearance = 1;
}

void changeColor(Entity * e, BoneId bone, char rgba1[], char rgba2[])
{
    int i;
    int change;
    SeqInst * seq;

    if(!e || !e->seq)
    {
        if( isDevelopmentMode() ) Errorf( "CUSTOM COLOR ERROR: changeColor called on garbage entity" );
        return;
    }

    if(devassert(bone_IdIsValid(bone)) && e && e->seq)
    {
        change = 0;
        seq = e->seq;
        for(i = 0 ; i < 3 ; i++)
        {
            if(seq->newcolor1[bone][i] != rgba1[i])
            {
                seq->newcolor1[bone][i] = rgba1[i];
                change = 1;
            }
            if(seq->newcolor2[bone][i] != rgba2[i])
            {
                seq->newcolor2[bone][i] = rgba2[i];
                change = 1;
            }
        }
        seq->newcolor1[bone][3] = 255;
        seq->newcolor2[bone][3] = 255;

        seq->updated_appearance |= change;
    }
}

//Change Scale needs to be called for everybody, so server does size randomizing
int changeScale(Entity * e, Vec3 newscale )
{
    SeqInst * seq;
    Vec3    effectivescale;
    if(!e || !e->seq)
    {
        if( isDevelopmentMode() )Errorf( "CUSTOM SCALE ERROR: changeScale called on garbage entity");
        return 0;
    }
    seq = e->seq;

    //NULL scale call, we decided, is just meaningless
    if( newscale[0] == 0 && newscale[1] == 0 && newscale[2] == 0.0 )
        return 0;

    //copyVec3(seq->type->geomscale, seq->currgeomscale);
    mulVecVec3( newscale, seq->type->geomscale,  effectivescale );
    if (!sameVec3(effectivescale, seq->currgeomscale))
    {

        copyVec3(effectivescale, seq->currgeomscale);

        //Reset animation scale to be proportional to new size
        if(seq->currgeomscale[1])
            seq->curranimscale = (1.0 / seq->currgeomscale[1]);
        else
            seq->curranimscale = 1.0;

        e->seq->updated_appearance = 1;
    }

    return 1;
}

void entReloadSequencersClient()
{
    int i;

    if( !game_state.local_map_server )  
        Errorf( "Client is not in localmapserver mode, so reloading sequencers is a bad idea. (But I'll do it anyway)" );

    seqPrepForReinit();
    //TO DO (sometime) check file dates and only reload if something has changed
    for(i=1;i<entities_max;i++) 
    {
        if (entity_state[i] & ENTITY_IN_USE && entities[i]->seq ) //to do: better validating?
        {
            seqReinitSeq( entities[i]->seq, SEQ_LOAD_FULL, entities[i]->randSeed, entities[i] );
            animSetHeader( entities[i]->seq, 0 ); //client only
            entSetSeq( entities[i], entities[i]->seq );
            costume_Apply(entities[i]); // Re-apply bonescales
        }
    }
}

// 
//-------------------------------------------------------------------------------------------------------------


char *nameFromDbId( int db_id )
{
    Entity *e = entFromDbId(db_id);

    if(e)
        return e->name;
    else
        return "Unknown1";
}

char * handleFromNameSlow( char * name )
{
    int i;
    for(i=1;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE )
        {
            Entity *e = entities[i];
            if( e->name && e->pl && e->pl->chat_handle && stricmp( name, e->name ) == 0 )
                return e->pl->chat_handle;
        }
    }
    return NULL;
}

//
//
char *playerName()
{
    Entity    *e = playerPtr();

    if( e )
        return e->name;
    else
        return "Unknown3";
}

//entalloc client
Entity *entAlloc()
{
    Entity    *e;

    e = entAllocCommon();

    e->posLastFrame[1] = -10000; //Ensures the graphics doesn;t think guy spawned at 0 0 0 never moved.
    return e;
}


void entReset()
{
    int        i;
    extern U32 last_pkt_id;

    // Player entity about to get wiped.
    // Have all ui stuff reset themselves.
    uiReset();

    //printf("entReset has been called.\n");

    // Must do this *before* entFree, because we assert that it's == 0
    memset(ent_xlat,0,sizeof(ent_xlat));
    for(i=1;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE )
            entFree(entities[i]);
    }

    memset(entity_state,0,sizeof(entity_state));
    last_pkt_id = 0;

    clearQueuedDeletes();
}

// Similar to entReset(), but does not mess with the player entity
void entResetNonPlayer()
{
    int i;
    for(i=1;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE && entities[i] != playerPtr())
        {
            entFree(entities[i]);
            ent_xlat[entities[i]->svr_idx] = 0;
            entity_state[i] = 0;
        }
    }

    clearQueuedDeletes();
}

Entity *entFromId(int id)
{
    if (id < 1 || id >= ARRAY_SIZE(ent_xlat))
        return 0;
    return ent_xlat[id];
}

Entity *validEntFromId(int id)
{
    // Returns an entity pointer only if that entity is in use.

    Entity* e = entFromId(id);

    if(!e || entInUse(e->owner)){
        return e;
    }

    return NULL;
}

void entFree(Entity *ent)
{
    extern Entity* current_target;
    Entity localCopy; // Purely for debugging purposes.

    int    i;
    if (!ent)
        return;

    localCopy = *ent;

    assertmsg(ent->svr_idx <= 0 || !ent_xlat[ent->svr_idx], "Please get Garth or another programmer. Freeing server entity!");

    if(ent == current_target)
    {
        current_target = NULL;
        gSelectedDBID  = 0;
        gSelectedHandle[0] = 0;
    }

    if(ent == getMySlave())
        setMySlave(NULL, NULL, 0);

    if(ent == playerPtrForShell(0))
        setPlayerPtrForShell(NULL);

    //free any outstanding fx...
    for( i = 0 ; i < ent->maxTrackerIdxPlusOne ; i++ )
    {
        if( ent->fxtrackers[i].state == FXT_RUNNING_MAINTAINED_FX )
        {
            //assert( ent->fxtrackers[i].fxid );
            fxDelete( ent->fxtrackers[i].fxid, SOFT_KILL );
        }
    }

    entity_state[ent->owner] = 0;//ENTITY_UNAVAILABLE | ENTITY_RELEASE_TIMER;    //ies_used[idx] = 0;

    playerVarFree(ent);
    
    demoFreeEntityRecordInfo(ent);

#if NOVODEX
    entFreeNovodexCapsule(ent);
#endif

    entFreeCommon(ent);
}

static void clearBonesWithStaticLights(GfxNode *node, int seqHandle)
{
    for(; node; node = node->next)
    {
        if( node->seqHandle == seqHandle )
        {
            if( bone_IdIsValid(node->anim_id) && node->model && (node->model->flags & OBJ_STATICFX) )
            {    
                node->rgbs = NULL;
            }
            if(node->child)
            {
                clearBonesWithStaticLights(node->child, seqHandle);
            }
        }
    }

}

void entResetTrickFlags(void)
{
    int i, j;
    for(i=1;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE ) {
            Entity *e = entities[i];
            if (e->seq) {
                SeqInst *seq = e->seq;
                setVec3(seq->posTrayWasLastChecked, 20000, 21000, 22000);
                memset( &seq->seqGfxData.light, 0, sizeof( EntLight ) );
                // Reset static lighting
                seq->static_light_done = STATIC_LIGHT_NOT_DONE;
                for(j = 0; j < ARRAY_SIZE(seq->rgbs_array); j++)
                {
                    if( seq->rgbs_array[j] )
                        modelFreeRgbBuffer(seq->rgbs_array[j]);
                    seq->rgbs_array[j] = NULL;
                }
                clearBonesWithStaticLights(seq->gfx_root->child, seq->handle);
                if( e->costume ) //Why do some things not have costumes?
                    costume_Apply(e);
            }
            setVec3(e->volumeList.posVolumeWasLastChecked, 20000, 21000, 22000);
        }
    }
}

void entResetModels(void)
{
    int i;
    for(i=1;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE ) {
            Entity *e = entities[i];
            if (e->seq) {
                SeqInst *seq = e->seq;
                int randSeed = e->randSeed;
                seqAssignBonescaleModels(seq, &randSeed);
            }
        }
    }
}


/* End of File */
