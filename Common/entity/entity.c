#include <string.h>
#include <utilitieslib/components/EString.h>
#include "entity/character_base.h"
#include "entity/entity.h"
#include "entity/motion.h"
#include "cmdparse/cmdcommon.h"
#include "cmdparse/cmdcontrols.h"
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/assert/assert.h>
#include "seq/seq.h"
#include <utilitieslib/utils/utils.h>
#include "varutils.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/mathutil.h>
#include "seq/seqskeleton.h"
#include "entity/teamCommon.h"
#include "entity/teamup.h"
#include "float.h"
#include <utilitieslib/utils/Quat.h>
#include "auth/authUserData.h"
#include "entity/entPlayer.h"
#include <utilitieslib/components/earray.h>
#include "storyarc/playerCreatedStoryarc.h"
#include <utilitieslib/utils/log.h>
#include "entity/costume.h"
#include "gameData/BodyPart.h"

#if SERVER

#include "dbcomm/dbnamecache.h"
#include "group/groupnetdb.h"
#include "cmdparse/cmdserver.h"
#include "svr/svr_base.h"
#include "gameComm/sendToClient.h"
#include "dbcomm/dbcomm.h"
#include "dbcomm/dbcontainer.h"
#include "group/groupnetsend.h"
#include "svr/svr_player.h"
#include "ai/entaivars.h"
#include "entity/entserver.h"
#include "entity/entsend.h"
#include "entity/entPlayer.h"
#include "player/badges_server.h"
#include <utilitieslib/utils/log.h>
#else

#include "graphics/FX/fx.h"
#include "graphics/seqgraphics.h"
#include "player/player.h"
#include "gameData/BodyPart.h"
#include "storyarc/playerCreatedStoryarc.h"

#endif

const float BASE_PLAYER_SIDEWAYS_SPEED = 1.0;    //as percentage of forward speed
const float BASE_PLAYER_VERTICAL_SPEED = 1.0;    //feet per tick
const float BASE_PLAYER_FORWARDS_SPEED = 0.7;    //feet per tick
const float BASE_PLAYER_BACKWARDS_SPEED = 0.75;    //as percentage of forward speed

Volume glob_dummyEmptyVolume; //should never be anything but 0s.

//Grid ent_grid;
//Grid ent_collision_grid;
//Grid ent_player_grid;

int entInUse(int id)
{
    return id >= 1 && id < entities_max && (entity_state[id] & ENTITY_IN_USE);
}

MP_DEFINE(Entity);

//ent Alloc Server
Entity * entAllocCommon()
{
    Entity * e = 0;
    int i,idx;
#if SERVER
    U32 cur_time = dbSecondsSince2000();
#endif

    MP_CREATE(Entity,64);

    //allocate e
    for(i=1;i<MAX_ENTITIES_PRIVATE;i++)
    {
#if SERVER
        if (!entity_state[i] && (!entities[i] || abs(cur_time - entities[i]->freed_time) > 5))
#else
        if (!entity_state[i])
#endif
        {
            if (i >= entities_max)
            {
                entities_max = i + 1;
                entities[i] = MP_ALLOC(Entity);
            }
            entity_state[i] = ENTITY_IN_USE;    //used[i] = 1;
            e = entities[i];
            memset(e, 0, sizeof(Entity));
            memset(entinfo + i, 0, sizeof(entinfo[0]));
#ifdef SERVER
            memset(entpos + i, 0, sizeof(entpos[0]));
#endif
            idx = i;
            break;
        }
    }
    assert(e);
    
    //do game/server common initializing

    e->motion = createMotionState();
    
    e->owner = idx;
    e->xlucency = 1.0f;
    e->namePtr = &e->name[0];

    //So these can be dereferenced without needing to check
    e->volumeList.neighborhoodVolume = &glob_dummyEmptyVolume;
    e->volumeList.materialVolume = &glob_dummyEmptyVolume;

    e->bitfieldVisionPhases = 1; // default to be in the prime phase
    e->exclusiveVisionPhase = 0; // default to be in the prime phase
    e->bitfieldVisionPhasesDefault = 1;
    e->exclusiveVisionPhaseDefault = 0;

    e->shouldIConYellow = false;

    entInitNetVars(e);

    entSetMat3(e, unitmat);

    return e;
}

//common stuff between svr entfree and client entfree
void entFreeCommon( Entity * e )
{
    assertmsg(entities[e->owner] == e, "Please get Garth or another programmer. Entity local idx corrupted!");

    entUntrackDbId(e);
    entMotionFreeColl(e);
    entSetSeq(e, NULL);

    destroyMotionState(e->motion);

    SAFE_FREE( e->specialPowerDisplayName );
    SAFE_FREE( e->petName );

    SAFE_FREE( e->volumeList.volumes );
    SAFE_FREE( e->volumeList.volumesLastFrame );

#ifdef SERVER
    SAFE_FREE( e->petList );
    e->petList_max = e->petList_size = 0;
#endif

    if (e->mission_inventory)
    {
#ifdef SERVER
        eaDestroyEx(&e->mission_inventory->pending_orders, NULL);
#endif
        stashTableDestroy(e->mission_inventory->stKeys );
        eaDestroyEx(&e->mission_inventory->unlock_keys, NULL);
        SAFE_FREE(e->mission_inventory);
    }

    entity_state[e->owner] = 0;
    ZeroStruct(e);
    e->id = -1;
    e->db_id = -1;
}

void entInitNetVars(Entity* e)
{
    static init;
    static U32 u32_comp_packed;
    static F32 f32_comp_unpacked;
    static U32 u32_comp1_packed;
    static F32 f32_comp1_unpacked;

    
    int i;
    
    if(!init)
    {
        init = 1;
        u32_comp_packed = packQuatElem(0, 9);
        f32_comp_unpacked = unpackQuatElem(u32_comp_packed, 9);
        u32_comp1_packed = packQuatElem(1.0f, 9);
        f32_comp1_unpacked = unpackQuatElem(u32_comp1_packed, 9);
    }
    
    // Initialize the net values (all are decoded to zero).
        
    for(i = 0; i < 3; i++)
    {
        e->net_pos[i] = 0;
        e->net_ipos[i] = (1 << (POS_BITS - 1));

        if ( i == 0)
        {
            e->net_qrot[i] = f32_comp1_unpacked;

            #if SERVER
                e->net_iqrot[i] = u32_comp1_packed;
            #endif
        }
        else
        {
            e->net_qrot[i] = f32_comp_unpacked;

            #if SERVER
                e->net_iqrot[i] = u32_comp_packed;
            #endif
        }
    }
    quatCalcWFromXYZ(e->net_qrot);
    quatNormalize(e->net_qrot);
}
    
void entSetSeq(Entity* e, SeqInst* seq)
{
    MotionState* motion = e->motion;
    bool reattach=false;
    SeqInst *oldseq = e->seq;
    int oldseqhandle=0;
#if CLIENT && !defined(TEST_CLIENT)
    int i, *tempFX=0;
#endif

    if (e->seq && e->seq != seq)
    {
#if CLIENT && !defined(TEST_CLIENT)
        if (seq) { // Only if we're changing the sequencer, not clearing it
            // Release old FX
            if( gfxTreeNodeIsValid( oldseq->gfx_root, oldseq->gfx_root_unique_id ) )
            {
                reattach = true;
                // Clear costume FX/constant FX/etc
                for( i = 0; i < eaiSize(&oldseq->seqcostumefx); i++)
                {
                    if(bodyPartList.bodyParts[i]->dont_clear)
                    {
                        eaiPush(&tempFX,oldseq->seqcostumefx[i]);
                        oldseq->seqcostumefx[i] = 0;
                    }
                    else
                        eaiPush(&tempFX,0);
                }

                seqClearExtraGraphics( oldseq, 0, 1 );
                gfxTreeDeleteAnimation( oldseq->gfx_root, oldseq->handle );
            }
            oldseqhandle = oldseq->handle;
        }
#endif

        seqFreeInst( oldseq );
    }

    e->seq = seq;

    if (reattach)
    {
#if CLIENT && !defined(TEST_CLIENT)
        // TODO: remap seqhandles on all FX!
        fxChangeSeqHandles(oldseqhandle, seq->handle);
        gfxTreeRelinkSuspendedNodes( seq->gfx_root, seq->handle );

        if(eaiSize(&seq->seqcostumefx)<eaiSize(&tempFX))
            eaiSetSize(&seq->seqcostumefx,eaiSize(&tempFX));
        for( i = eaiSize(&tempFX)-1; i >= 0; i--)
        {
            if(bodyPartList.bodyParts[i]->dont_clear)
                seq->seqcostumefx[i] = tempFX[i];
        }
        eaiDestroy(&tempFX);
#endif
        animCalcObjAndBoneUse(seq->gfx_root->child, seq->handle );
    }

    if(seq)
    {
        getCapsule(    e->seq, &motion->capsule);
#if SERVER
        SET_ENTFADE(e) = e->seq->type->fade[1];
        entinfo[e->owner].seq_coll_door_or_worldgroup =
            seq->type->collisionType == SEQ_ENTCOLL_DOOR
            || seq->type->collisionType == SEQ_ENTCOLL_LIBRARYPIECE;
#endif
    }
    else if(motion)
    {
        ZeroStruct(&motion->capsule);
    }

}

bool entUpdatePosInterpolated(Entity *e, const Vec3 pos)
{
    if ( !validateVec3(pos))
        return false;
#ifdef SERVER
    devassert(sameVec3(entpos[e->owner], e->fork[3]));
    copyVec3(pos, entpos[e->owner]);
#endif
    copyVec3(pos, e->fork[3]);
    return true;
}

#ifdef SERVER
void entSyncControl(Entity *e)
{
    if(e->client)
    {
        ServerControlState* scs;
        ControlState* controls = &e->client->controls;

        entSendPlrPosQrot(e);

        // Disable physics until the state is acknowledged, including all queued states.

        controls->server_state->physics_disabled = 1;

        for(scs = controls->queued_server_states.head; scs; scs = scs->next){
            scs->physics_disabled = 1;
        }

        svrResetClientControls(e->client, 0);

        getMat3YPR(ENTMAT(e), controls->pyr.cur);
    }
}
#endif

bool entUpdatePosInstantaneous(Entity *e,const Vec3 pos)
{
    if (!e || !e->motion)
        return false;

    if (!validateVec3(pos))
        return false;
#ifdef SERVER
    devassert(sameVec3(entpos[e->owner], e->fork[3]));
    copyVec3(pos,entpos[e->owner]);
#endif
    copyVec3(pos,e->fork[3]);
    copyVec3(pos,e->motion->last_pos);
    e->motion->highest_height = pos[1];
    e->motion->falling = !e->motion->input.flying;
    e->motion->jumping = 0;
    zeroVec3(e->motion->vel);
    
#ifdef SERVER
    {
        AIVars* ai = ENTAI(e);

        if(ai)
        {
            getMat3YPR(ENTMAT(e), ai->pyr);
        }
    }

    e->move_instantly = 1;
    
    // Reset distance history (don't affect maxIndex).
    
    //memset(&e->dist_history.dist, 0, sizeof(e->dist_history.dist));
    //e->dist_history.nextIndex = 0;
    //e->dist_history.total = 0;
    
    // Reset nearest beacon so it will be looked up again when needed.
    
    e->nearestBeacon = NULL;
    
    if (db_state.is_static_map)
        copyVec3(pos, e->static_pos);

    if (ENTTYPE(e) == ENTTYPE_PLAYER)
        entSyncControl(e);
#endif

#ifdef CLIENT
    if(playerPtr() == e)
    {
        copyVec3(pos,control_state.start_pos);
        copyVec3(pos,control_state.end_pos);
    }
#endif

    return true;
}

bool entUpdateMat4Instantaneous(Entity *e,const Mat4 mat)
{
    if ( !validateMat4(mat) )
        return false;
    copyMat3(mat,e->fork);
    entUpdatePosInstantaneous(e,mat[3]);

    return true;
    //if(ENTTYPE(e) == ENTTYPE_PLAYER)
    //{
    //    printf("orientation: %f, %f, %f\n", vecParamsXYZ(mat[2]));
    //}
}

bool entUpdateMat4Interpolated(Entity *e,const Mat4 mat)
{
    if ( !validateMat4(mat) )
        return false;
    copyMat3(mat, e->fork);
    entUpdatePosInterpolated(e, mat[3]);
    return true;
}

bool entSetMat3(Entity* e,const Mat3 mat)
{
    if ( !validateMat3(mat) )
        return false;
    copyMat3(mat, e->fork);
    return true;
}

static StashTable db_id_hashes;

// DO NOT USE THIS DIRECTLY! Only for routines that deal with entity map transfers
// For regular use, call entFromDbId which makes sure the player is actually connected
Entity *entFromDbIdEvenSleeping(int db_id)
{
    Entity    *e;

    if (!db_id_hashes)
        return 0;
    if ( db_id && stashIntFindPointer(db_id_hashes, db_id, &e))
        return e;
    return NULL;
}

void entTrackDbId(Entity *e)
{
    Entity *e2;

    if (e->db_id <= 0)
        return;
    if (!db_id_hashes)
    {
        db_id_hashes = stashTableCreateInt(4);
    }
    if (stashIntRemovePointer(db_id_hashes, e->db_id, &e2))
    {
        LOG_OLD("entTrackDbId - duplicate ent %d %d\n",e2->owner,e2->db_id);
    }
    stashIntAddPointer(db_id_hashes, e->db_id, e, false);
#ifdef SERVER
    dbAddToNameTables(e->name, e->db_id, e->gender, e->name_gender,
                        e->pl ? e->pl->playerType : 0,
                        e->pl ? e->pl->playerSubType : 0,
                        e->pchar ? e->pchar->playerTypeByLocation : 0,
                        e->pl ? e->pl->praetorianProgress : 0,
                        e->auth_id);
#endif
}

void entUntrackDbId(Entity *e)
{
    Entity    *e2;

    if (e->db_id <= 0)
        return;
    if (!db_id_hashes)
    {
        db_id_hashes = stashTableCreateInt(4);
    }

    if (stashIntFindPointer(db_id_hashes, e->db_id, &e2) && e2 == e)
    {
        stashIntRemovePointer(db_id_hashes, e->db_id, NULL);
    }
    else
    {
        if (e2)
        {
            LOG_OLD("entUntrackDbId failed, %d %d\n",e2->owner,e2->db_id);
        }
        else 
        {
            LOG_OLD("entUntrackDbId failed with null hash value\n");
        }
    }
}


Entity * entFromDbId(int db_id)
{
    Entity    *e;

    e = entFromDbIdEvenSleeping(db_id);
    if (!e)
        return 0;
    if (!(entity_state[e->owner] & ENTITY_IN_USE))
        return 0;
    assert(ENTTYPE_BY_ID(e->owner) == ENTTYPE_PLAYER && e->db_id == db_id);
    return e;
}

int ent_close( const float max_dist, const Vec3    p1, const Vec3 p2 )
{
    return ( max_dist * max_dist >= distance3Squared( p1, p2 ) );
}

void entGetNetInterpOffsets(float* array, int size){
    if(0)
    {
        int i;
        
        for(i = 0; i < 131; i++)
        {
            array[i] = (i >= 1 ? array[i - 1] : 1) + (i >= 3 ? array[i - 3] : 0);
        }

        for(i = 0; i < 131; i++)
        {
            array[i] = sqrt(array[i]) / 20.0;
        }

        //for(i = 0; i < 131; i++)
        //{
        //    printf("%3d: %1.4f\n", i, offset_table[i]);
        //}
    }
    else
    {
        int i;
        
        array[0] = 0;
        array[1] = 0;
        array[2] = 0.05;
        array[3] = 0.1;
        array[4] = 0.2;
        array[5] = 0.3;
        array[6] = 0.4;
        array[7] = 0.5;
        array[8] = 0.75;
        array[9] = 1.0;
        array[10] = 1.25;
        array[11] = 1.5;
        array[12] = 1.75;
        array[13] = 2.0;
        array[14] = 2.5;
        array[15] = 3.0;
        array[16] = 3.5;
        array[17] = 4.0;
        
        for(i = 18; i < size; i++){
            array[i] = 5.0 + i - 18;
        }
    }
}

S64 getEntMaxInf(Entity *e)
{
    S64 infCap = MAX_INFLUENCE;

#if (CLIENT && !defined(TEST_CLIENT)) || SERVER
    if (e && e->pl)
        infCap = accountLoyaltyRewardGetInfluenceCap(e->pl->loyaltyPointsEarned, ent_GetProductInventory( e ), e->pl->account_inventory.accountStatusFlags);
    else 
        infCap = accountLoyaltyRewardGetInfluenceCap(0, NULL, 0);    
#endif

    return infCap;
}

// 35,000,000
int ent_AdjInfluence(Entity *e, int iInfluence, char *stat)
{
    int starting_influence;
    static char *estr = NULL;
    int max;

    if(!e || !e->pchar || !e->pl)
        return 0;

    max = getEntMaxInf(e);
    starting_influence = e->pchar->iInfluencePoints; 

    // If they're over the cap, don't pull them down to the cap. Just don't
    // let them increase further. This avoids stripping players of inf if we
    // falsely determine they're a trial account, for example.

    // *infPtr = 8,208,410
    // max = 50,000
    // iInfluence = 35,000,000
    // 35,000,000 > 0 : yes
    // 8,208,410 > 50,000 - 35,000,000 : yes
    if(starting_influence > max)
        max = starting_influence;
    if (iInfluence > 0 && e->pchar->iInfluencePoints > max - iInfluence) //cap
        e->pchar->iInfluencePoints = max;
    else 
        e->pchar->iInfluencePoints += iInfluence;
    
    if(e->pchar->iInfluencePoints < 0)
        e->pchar->iInfluencePoints = 0;

#ifdef SERVER
    if (stat)
    {
        estrPrintf(&estr, "inf.%s", stat);
        badge_StatAdd(e, estr, e->pchar->iInfluencePoints - starting_influence);
    }
#endif

    return e->pchar->iInfluencePoints - starting_influence;
}

int ent_SetInfluence(Entity *e, int iInfluence)
{
    int max;

    if(!e || !e->pchar || !e->pl)
        return 0;

    max = getEntMaxInf(e);

    if(iInfluence < 0)
        e->pchar->iInfluencePoints = 0;
    else if(iInfluence > max)
        e->pchar->iInfluencePoints = max;
    else
        e->pchar->iInfluencePoints = iInfluence;

    return e->pchar->iInfluencePoints;
}

int ent_canAddInfluence( Entity *e, int iInfluence )
{
    INT64 max, total = 0;

    max = getEntMaxInf(e);

    total = (INT64)e->pchar->iInfluencePoints + iInfluence;

    // test influence here, but don't award it yet.  Item may fail
    if( total < 0 || (total > max) )
    {
        return 0;
    }

    return 1;
}

BOOL entIsTrial(Entity *e)
{
    return FALSE;
}

//    This should probably be put into a .h =/
int getEntAlignment(Entity *e) 
{
    if (e && e->pl)
    {
        if (e->pl->playerType == kPlayerType_Hero)
        {
            if (e->pl->playerSubType == kPlayerSubType_Rogue)    return ENT_ALIGNMENT_TYPE_VIGILANTE;
            else if (e->pl->playerSubType == kPlayerSubType_Normal)    return ENT_ALIGNMENT_TYPE_HERO;
            else if (e->pl->playerSubType == kPlayerSubType_Paragon) return ENT_ALIGNMENT_TYPE_INVALID;
        }
        else if (e->pl->playerType == kPlayerType_Villain)
        {
            if (e->pl->playerSubType == kPlayerSubType_Rogue)    return ENT_ALIGNMENT_TYPE_ROGUE;
            else if (e->pl->playerSubType == kPlayerSubType_Normal)    return ENT_ALIGNMENT_TYPE_VILLAIN;
            else if (e->pl->playerSubType == kPlayerSubType_Paragon) return ENT_ALIGNMENT_TYPE_INVALID;
        }
        assertmsg(0, "Undefined player type");
        return ENT_ALIGNMENT_TYPE_INVALID;
    }
    assertmsg(0, "invalid entity");
    return ENT_ALIGNMENT_TYPE_INVALID;

}

AccountInventorySet* ent_GetProductInventory( Entity* e )
{
    return (( e != NULL ) && ( e->pl != NULL )) ? &e->pl->account_inventory : NULL;
}

//--
// COSTUME 

// In some cases we want to use an existing entity to host a new
// costume for some manipulation. This method detaches the current
// costume information and stores the current costume information
// (e.g. owned, mutable, etc) in a structure so that it can be retored
// at a later time.
// Could be used for a 'costume' stack on the entity but does not appear to be necessary
// used when we want to save current costume state, swap in another costume for a bit, and then restore the original state
void    entDetachCostume(Entity* e, EntCostumeRestoreBlock* restore_block )
{
    // save current information
    restore_block->costume = e->costume;
    restore_block->costume_is_mutable = e->costume_is_mutable;
    restore_block->owned_costume = e->ownedCostume;
    restore_block->costume_is_owned = e->ownedCostume != NULL;
    // now detach the current information so that when new costume
    // information is set it doesn't cause the current information to be
    // discarded (e.g. ownedCostume destroyed)
    e->costume = NULL;
    e->ownedCostume = NULL;
    e->costume_is_mutable = false;
}

void    entRestoreDetachedCostume(Entity* e, EntCostumeRestoreBlock* restore_block)
{
    entReleaseCostume(e);    // release current
    // restore detached
    e->costume = restore_block->costume;
    e->ownedCostume = restore_block->owned_costume;
    e->costume_is_mutable = restore_block->costume_is_mutable;
}

void    entReleaseCostume(Entity* e) // release current costume and destroy if owned, etc
{
    entSetImmutableCostumeUnowned(e, NULL);
}

// use the costume information from another entity (unowned share)
void    entCostumeShare(Entity* e, Entity* e_to_share_from )
{
    if (e_to_share_from->costume_is_mutable)
    {
        entSetMutableCostumeUnowned( e, cpp_reinterpret_cast(Costume*)(e_to_share_from->costume) );
    }
    else
    {
        entSetImmutableCostumeUnowned( e, e_to_share_from->costume );
    }
}

// sometimes the entity gets handed a mutable costume to use for some period of time
// (e.g. tailor calculations)
void    entSetMutableCostumeUnowned(Entity* e, Costume* mutableCostume)
{
    Costume* previously_owned = NULL;
    // see if we need to discard previously owned mutable costume
    if (e->ownedCostume)
    {
        devassert((uintptr_t)e->costume == (uintptr_t)e->ownedCostume);
        previously_owned = e->ownedCostume;
    }
    e->ownedCostume = NULL;    // we don't own it
    e->costume = costume_as_const(mutableCostume);
    e->costume_is_mutable = true;    // can change directly as it is already copied or shared with a mutable copy

    if ( previously_owned )    // if we had a previous owned costume destroy it now
    {
        costume_destroy(previously_owned);
    }
}

void    entSetImmutableCostumeUnowned(Entity* e, const cCostume* immutableCostume)
{
    Costume* previously_owned = NULL;
    // see if we need to discard previously owned mutable costume
    if (e->ownedCostume)
    {
        devassert((uintptr_t)e->costume == (uintptr_t)e->ownedCostume);
        previously_owned = e->ownedCostume;
    }
    e->ownedCostume = NULL;    // we don't own it
    e->costume = immutableCostume;
    e->costume_is_mutable = false;    // need to copy if we want to change it

    if ( previously_owned )    // if we had a previous owned costume destroy it now
    {
        costume_destroy(previously_owned);
    }
}

static void    entSetOwnedCostume(Entity* e, Costume* mutableCostume)
{
    Costume* previously_owned = NULL;

    // see if we need to discard previously owned mutable costume
    if (e->ownedCostume)
    {
        devassert((uintptr_t)e->costume == (uintptr_t)e->ownedCostume);
        previously_owned = e->ownedCostume;
    }

    e->ownedCostume = mutableCostume;
    e->costume = costume_as_const(e->ownedCostume);
    e->costume_is_mutable = true;    // can change directly as it is already copied or shared with a mutable copy

    if ( previously_owned )    // if we had a previous owned costume destroy it now
    {
        costume_destroy(previously_owned);
    }
}

// gets a mutable copy of entity current costume
static Costume*    entGetMutableCostume_using_ownedCostume(Entity* e)
{
    if (e->ownedCostume)
    {
        devassert( e->costume_is_mutable );
        devassert((uintptr_t)e->costume == (uintptr_t)e->ownedCostume);
        return e->ownedCostume;
    }
    else if (e->costume_is_mutable)
    {
        // costume is mutable by virtue of being shared with a mutable costume
        // e.g., one of the costumes in the players array of costume slots
        return cpp_reinterpret_cast(Costume*)(e->costume);
    }
    else
    {
        // we need to make a mutable version of current const costume
        return entUseCopyOfConstCostume(e, e->costume);
    }
}

Costume*    entGetMutableCostume(Entity* e)
{
#if CLIENT
    // for now the client just casts to make mutable as its usage cases need
    // a more comprehensive review (e.g. player entity already has a table of mutable costumes)
    return costume_as_mutable_cast(e->costume);
#else
    return entGetMutableCostume_using_ownedCostume(e);
#endif
}

// If the entity has a mutable costume use it, otherwise give it a brand new
// costume to use as its mutable.
// This is in contrast to entGetMutableCostume which will make a clone of the
// current e->costume to make the mutable costume
Costume*    entGetMutableOrNewCostume(Entity* e)
{
#if CLIENT
    assert(false); // used by client?
    return NULL;
#else
    if (e->ownedCostume)
    {
        devassert((uintptr_t)e->costume == (uintptr_t)e->ownedCostume);
        return e->ownedCostume;
    }
    else
    {
        // we use a brand new costume as our mutable
        Costume* new_costume = costume_create(GetBodyPartCount());
        entSetOwnedCostume(e,new_costume);
        return new_costume;
    }
#endif
}

// give entity its own local copy of the supplied costume
// return pointer to entities new mutable costume

Costume*    entUseCopyOfCostume(Entity* e, Costume* aCostume)
{
    return entUseCopyOfConstCostume(e,costume_as_const(aCostume));
}

Costume*    entUseCopyOfConstCostume(Entity* e, const cCostume* aConstCostume)
{
    Costume* costume_copy = costume_clone( aConstCostume );
#if CLIENT
    e->costume = costume_as_const(costume_copy);
    e->costume_is_mutable = true;    // can change directly as it is already copied or shared with a mutable copy
#else
    entSetOwnedCostume(e,costume_copy);
#endif
    return costume_copy;
}

Costume*    entUseCopyOfCurrentCostumeAndOwn(Entity* e)    // force a clone of current const costume, and own it
{
    Costume* costume_copy = costume_clone( e->costume );
    entSetOwnedCostume(e,costume_copy);
    return costume_copy;
}

const cCostume*    entGetConstCostume(Entity* e)
{
    return e->costume;
}

#if CLIENT
// used by the client for the manipulating the e->costume for swapping in
// different costumes from the player costume array, or tailor, etc.
// Doesn't muck with the 'ownedCostume' used by NPCs/Pets
Costume*    entSetCostumeAsUnownedCopy(Entity* e,  Costume* aCostume)
{
    Costume* costume_copy = costume_clone( costume_as_const(aCostume) );
    e->costume = costume_as_const(costume_copy);
    e->costume_is_mutable = true;    // can change directly as it is already copied or shared with a mutable copy
    assert( e->ownedCostume == NULL );
    return costume_copy;
}

// set e->costume directly, does not make copies or adjust ownedCostume
void    entSetCostumeDirect(Entity* e,  Costume* aCostume)
{
    e->costume = costume_as_const(aCostume);
}
#endif
