#include <math.h>
#include <string.h>
#include <utilitieslib/utils/mathutil.h>
#include "entity/entity.h"
#include "entity/EntPlayer.h"
#include "gridcoll/gridcoll.h"
#include "graphics/camera.h"
#include "varutils.h"
#include "entity/motion.h"
#include "utils/Position.h"
#include "entworldcoll.h"
#include "entity/character_base.h"
#include "gridcoll/gridcache.h"
#include "group/groupscene.h"
#include "seq/seqstate.h"
#include "render/model.h"
#include "seq/seq.h"
#include "entity/gametypes.h"
#include "gridcoll/megaGrid.h"
#include "seq/anim.h"
#include "group/group.h"
#include <utilitieslib/utils/qsortG.h>
#include "group/groupfileload.h"
#include <utilitieslib/utils/Quat.h>
#include "entity/character_target.h"

#ifdef CLIENT
    #include "graphics/font.h"
    #include "entity/entclient.h"
    #include "bases/baseedit.h"
    #include "UI/uiOptions.h"
    #include "UI/uiPet.h"
#else
    #include "entity/character_combat.h"
    #include "beacon/beaconConnection.h"
    #include "entity/entserver.h"
    #include "cmdparse/cmdcontrols.h"
    #include "player/pmotion.h"
    #include "beacon/beaconpath.h"
    #include "entity/character_target.h"
#endif

#ifdef CLIENT
#define CONTEXT_SENSITIVE_ID svr_idx
#else
#define CONTEXT_SENSITIVE_ID owner
#endif

GlobalMotionState global_motion_state;

#if CLIENT
    #include "player/player.h"
    #include "entity/entDebug.h"
    #include "cmdparse/cmdgame.h"
    #include "entity/entDebug.h"
    #define MDBG 0
    #define SETB_MOTION(a, b) {if(!global_motion_state.doNotSetAnimBits) SETB(a, b);}
    #define CLRB_MOTION(a, b) {if(!global_motion_state.doNotSetAnimBits) CLRB(a, b);}
#elif SERVER
    #include "entity/entGameActions.h"
    #include "svr/svr_base.h"
    #include "ai/entaiPrivate.h"
    #include "entity/entincludeserver.h"
    #include "storyarc/storyarcinterface.h"
    #define SETB_MOTION(a, b) SETB(a, b)
    #define CLRB_MOTION(a, b) CLRB(a, b)
#endif

#include <utilitieslib/utils/timing.h>

MP_DEFINE(MotionState);


MotionState* createMotionState(void)
{
    MP_CREATE(MotionState, 64);

    return MP_ALLOC(MotionState);
}

void destroyMotionState(MotionState* motion)
{
    MP_FREE(MotionState, motion);
}

//Position the capsule correctly.
//Output: collmat gets the capsule's orientation and the position of the near capsule focus 
void positionCapsule( const Mat4 entMat, EntityCapsule * cap, Mat4 collMat )
{
    Vec3 capVec;
    Vec3 capVec2;

    zeroVec3( capVec );

    copyMat4( entMat, collMat ); 

    if( cap->dir == 0 )  
        capVec[1] = -1 * cap->length/2;
    else
        capVec[1] = cap->radius;

    if( cap->dir == 2 ) 
        pitchMat3( RAD(-90), collMat );
    if( cap->dir == 0 )
        rollMat3( RAD(90), collMat );

    mulVecMat3( capVec, collMat, capVec2 );
    addVec3( capVec2, collMat[3], collMat[3] );

    //Only mission items use this nonsense
    if( cap->has_offset )
    {
        Vec3 offsetX;
        mulVecMat3( cap->offset, entMat, offsetX );
        addVec3(collMat[3], offsetX, collMat[3]);
    }
    //End nonsense

    //drawLine3DWidth( collMat[3], playerPtr()->mat[3], 0x0000ffff, 20);  
}

//
// get the capsule dimensions from the bounding box
//
void getCapsule( SeqInst * seq, EntityCapsule * cap )
{
    Vec3 bounds;
    F32 l, r;

    seqGetCapsuleSize( seq, bounds, cap->offset );

    if ( bounds[0] > bounds[1] )
    {
        if ( bounds[0] > bounds[2] )
        {
            l = bounds[0];
            cap->dir = 0;

            if ( bounds[1] > bounds[2] )
                r = bounds[1];
            else
                r = bounds[2];
        }
        else
        {
            l = bounds[2];
            r = bounds[0];
            cap->dir = 2; // this may get screwy and need to be centered
        }
    }
    else
    {
        if ( bounds[1] > bounds[2] )
        {
            l = bounds[1];
            cap->dir = 1;

            if ( bounds[0] > bounds[2] )
                r = bounds[0];
            else
                r = bounds[2];
        }
        else
        {
            l = bounds[2];
            r = bounds[1];
            cap->dir = 2;
        }
    }

    cap->length = l - r;
    cap->radius = r/2;
    cap->has_offset = !vec3IsZero( cap->offset );
    
    cap->max_coll_dist = lengthVec3(cap->offset) +
            cap->radius +
            cap->length;

    if(    cap->dir )
        cap->max_coll_dist += cap->radius;

}

static struct {
    int enabled;
    U32 client_abs;
    U32 client_abs_slow;
} entCollTimes;

void setEntCollTimes(int enabled, U32 client_abs, U32 client_abs_slow)
{
    entCollTimes.enabled = enabled;
    entCollTimes.client_abs = client_abs;
    entCollTimes.client_abs_slow = client_abs_slow;
}

static int compareServerIDs(const int* id1, const int* id2)
{
    return *id1 - *id2;
}

//eQROT the qrot of the collided entity that will be the same on client and server
static void applyBounceToCollision( Entity * staticEnt, Entity * movingEnt, Vec3 dv, Quat eQROT )
{
    MotionState* motion = movingEnt->motion;
    Vec3 vel;
    Vec3 vel_dir;

    switch(staticEnt->seq->type->collisionType)
    {
        xcase SEQ_ENTCOLL_REPULSION:
        {
            scaleVec3( dv, -1 * staticEnt->seq->type->bounciness, motion->addedVel );
        }
        xcase SEQ_ENTCOLL_BOUNCEUP:
        {
            scaleVec3( dv, -1 * staticEnt->seq->type->bounciness, vel );
            motion->addedVel[0] = 0;
            motion->addedVel[1] = lengthVec3( vel );
            motion->addedVel[2] = 0;
        }
        xcase SEQ_ENTCOLL_BOUNCEFACING:
        {
            F32 len;
            F32 scale = staticEnt->seq->type->bounciness;

            //createMat3_2_YPR(vel_dir, ePYR);
            quatToMat3_2(eQROT, vel_dir);
            scaleVec3(vel_dir, scale, vel);

            len = lengthVec3( dv );

            scaleVec3( vel, len, motion->addedVel );

            // This block is a temporary hack!  Please figure out what's going on and fix the root problem!
            if (lengthVec3Squared(motion->addedVel) > 10000000000)
            {
                AssertErrorf(__FILE__, __LINE__, "We're about to add a ton of velocity.  Stopping that from happening.  Tell Andy as soon as you see this.  COH-27814");
                zeroVec3(motion->addedVel);
            }
        }
        xcase SEQ_ENTCOLL_LAUNCHUP:
        {
            //Don't do this if we just bounced off this guy
            if( motion->lastEntCollidedWith != staticEnt->CONTEXT_SENSITIVE_ID ||
                (motion->tickOfLastEntCollision + 30) < motion->tickCounter)
            {
                motion->addedVel[1] = sqrt(2 * staticEnt->seq->type->bounciness * 0.06);
            }
            //printf( "%f %f %f", vecParamsXYZ(motion->addedVel) );
        }
        xcase SEQ_ENTCOLL_LAUNCHFACING:
        {
            //Don't do this if we just bounced off this guy
            if( motion->lastEntCollidedWith != staticEnt->CONTEXT_SENSITIVE_ID ||
                (motion->tickOfLastEntCollision + 3) < motion->tickCounter)
            {
                F32 scale = sqrt(2 * staticEnt->seq->type->bounciness * 0.06);
                //createMat3_2_YPR(vel_dir, ePYR);
                quatToMat3_2(eQROT, vel_dir);
                scaleVec3(vel_dir, scale, motion->addedVel);

                // This block is a temporary hack!  Please figure out what's going on and fix the root problem!
                if (lengthVec3Squared(motion->addedVel) > 10000000000)
                {
                    AssertErrorf(__FILE__, __LINE__, "We're about to add a ton of velocity.  Stopping that from happening.  Tell Andy as soon as you see this.  COH-27814");
                    zeroVec3(motion->addedVel);
                }
            }
        }
        xcase SEQ_ENTCOLL_STEADYUP:
        {
            motion->addedVel[1] = staticEnt->seq->type->bounciness;
        }
        xcase SEQ_ENTCOLL_STEADYFACING:
        {
            F32 scale = staticEnt->seq->type->bounciness;
            //createMat3_2_YPR(vel_dir, ePYR);
            quatToMat3_2(eQROT, vel_dir);
            scaleVec3(vel_dir, scale, motion->addedVel);

            // This block is a temporary hack!  Please figure out what's going on and fix the root problem!
            if (lengthVec3Squared(motion->addedVel) > 10000000000)
            {
                AssertErrorf(__FILE__, __LINE__, "We're about to add a ton of velocity.  Stopping that from happening.  Tell Andy as soon as you see this.  COH-27814");
                zeroVec3(motion->addedVel);
            }
        }
    }
}

int checkEntColl(Entity *movingEnt, int doApplyBounce, const Mat3 control_mat)
{
    S32                i;
    S32                was_coll=0;
    Entity*            staticEnt;
    F32             push;
    F32                scale;
    F32                mag;
    Vec3            dv;

    EntType            movingEntType = ENTTYPE(movingEnt);
    Vec3            movingEntLoc;
    Vec3            movingEntLocMoved;
    SeqInst*        movingEntSeq = movingEnt->seq;
    EntityCapsule    movingEntCap;
    Vec3            movingEntDirVec;

    S32                checkEnts[MAX_ENTITIES_PRIVATE];
    S32                checkEntsCount = 0;

    #if SERVER
        S32            allEntsCount;
        EntityRef    erMovingEnt;
        EntityRef    erMovingEntOwner;
    #endif

    if(    global_motion_state.noEntCollisions                            ||
        movingEnt->motion->input.no_ent_collision                    ||
        !movingEntSeq                                                ||
        movingEnt->move_type == MOVETYPE_WIRE                        ||
        getMoveFlags(movingEntSeq, SEQMOVE_NOCOLLISION)                ||
        movingEntType == ENTTYPE_MOBILEGEOMETRY                        ||
        movingEntType == ENTTYPE_DOOR                                ||
        movingEntSeq->type->collisionType == SEQ_ENTCOLL_DOOR        ||
        movingEntSeq->type->collisionType == SEQ_ENTCOLL_NONE        ||
        movingEntSeq->type->collisionType == SEQ_ENTCOLL_LIBRARYPIECE)
    {
        return 0;
    }

#if SERVER
    //PERFINFO_AUTO_START("gridFindObjectsInSphere", 1);
    //count = gridFindObjectsInSphere(&ent_collision_grid,(void **)entArray,ARRAY_SIZE(entArray),posPoint(ent),0);
    //PERFINFO_AUTO_STOP();
    
    erMovingEnt            = erGetRef(movingEnt);
    erMovingEntOwner    = movingEnt->erOwner;

    PERFINFO_AUTO_START("mgGetNodesInRange", 1);
        allEntsCount = mgGetNodesInRange(1, ENTPOS(movingEnt), (void**)checkEnts, 0);
    PERFINFO_AUTO_STOP();
#endif

    #if CLIENT
        // Checking entire "entities" array on client, so start at 1.

        for(i = 1; i < entities_max; i++)
    #else
        for(i = 0; i < allEntsCount; i++)
    #endif
    {
        EntType staticEntType;

        #if CLIENT
            staticEnt = entities[i];

            if(!(entity_state[i] & ENTITY_IN_USE))
            {
                continue;
            }

            staticEntType = ENTTYPE_BY_ID(i);
        #else
            int idx = checkEnts[i];
            
            staticEntType = ENTTYPE_BY_ID(idx);
        #endif

        if (staticEntType == ENTTYPE_MOBILEGEOMETRY || staticEntType == ENTTYPE_DOOR)
        {
            continue;
        }

        #if SERVER
            // Delay entity lookup.

            staticEnt = entities[idx];
        #endif

        if(    !staticEnt                                                        ||
            staticEnt == movingEnt                                            ||
#ifdef SERVER
            entinfo[idx].seq_coll_door_or_worldgroup                        ||
            !staticEnt->seq
#else
            !staticEnt->seq                                                    ||
            staticEnt->seq->type->collisionType == SEQ_ENTCOLL_DOOR            ||
            staticEnt->seq->type->collisionType == SEQ_ENTCOLL_LIBRARYPIECE 
#endif
            )
        {
            continue;
        }
            
        // Big monster flag must match, so big and little monsters don't collide.
        
        if( movingEntType != ENTTYPE_PLAYER &&
            staticEntType != ENTTYPE_PLAYER &&
            movingEnt->seq->type->bigMonster != staticEnt->seq->type->bigMonster)
        {
            continue;
        }

        if(    getMoveFlags(staticEnt->seq, SEQMOVE_NOCOLLISION)            ||
            staticEnt->seq->type->collisionType == SEQ_ENTCOLL_NONE        ||
            staticEnt->motion->input.no_ent_collision                    ||
            #ifdef SERVER
                staticEnt->client && staticEnt->client->controls.nocoll ||
                ENTHIDE_BY_ID(idx)
            #else
                staticEnt->fadedirection == ENT_FADING_OUT                ||
                !staticEnt->svr_idx
            #endif
            )
        {
            continue;
        }

        if (!entity_TargetIsInVisionPhase(movingEnt, staticEnt))
        {
            continue;
        }
        
        // MS: Experimental non-collision for NPCs against non-moving PCs.
        //if(entType == ENTTYPE_PLAYER && !vec3IsZero(e->motion->vel))
        //    continue;

        #if SERVER
            checkEnts[checkEntsCount++] = idx;
        #else
            checkEnts[checkEntsCount++] = staticEnt->svr_idx;
        #endif
    }

    if(!checkEntsCount)
    {
        return 0;
    }

    if(movingEntType == ENTTYPE_PLAYER)
    {
        PERFINFO_AUTO_START("qsort", 1);
            qsortG(checkEnts, checkEntsCount, sizeof(checkEnts[0]), compareServerIDs);
        PERFINFO_AUTO_STOP();
    }

    movingEntCap = movingEnt->motion->capsule;
    
    if(control_mat)
        copyVec3(control_mat[movingEntCap.dir], movingEntDirVec);
    else
        copyVec3(ENTMAT(movingEnt)[movingEntCap.dir], movingEntDirVec);

    if(movingEntCap.dir == 0)
        scaleVec3(movingEntDirVec, -movingEntCap.length * 0.5, movingEntLocMoved);
    else
        scaleVec3(movingEntDirVec, movingEntCap.radius, movingEntLocMoved);

    copyVec3(ENTPOS(movingEnt), movingEntLoc);

    addVec3(movingEntLoc, movingEntLocMoved, movingEntLocMoved);

    // Apply the offset, relative to the orientation of the entity.

    if( movingEntCap.has_offset )
    {
        Vec3 offsetX;
        if(control_mat)
            mulVecMat3( movingEntCap.offset, control_mat, offsetX );
        else
            mulVecMat3( movingEntCap.offset,  ENTMAT(movingEnt), offsetX );
        addVec3(movingEntLocMoved, offsetX, movingEntLocMoved);
    }

    PERFINFO_AUTO_START("checkAllEnts", 1);

    for(i = 0; i < checkEntsCount; i++)
    {
        S32                    foundLocAtTime = 0;
        Vec3                staticEntLoc;
        EntType                staticEntType;
        EntityCapsule        staticEntCap;
        F32                    maxCollDistance;
        F32                    maxCollDistanceSQR;
        F32                 radiusTotal;

        #if SERVER
            S32                pos_end;
            Vec3*            pos_pos;
            Quat*        pos_qrot = NULL;
            U32*            pos_time;
            S32                lo_idx;
            S32                hi_idx;
            F32                lo_factor;
            S32                staticEntIdx;
        #endif

        #if SERVER
            staticEntIdx    = checkEnts[i];
            staticEnt        = entities[staticEntIdx];
            staticEntType    = ENTTYPE_BY_ID(staticEntIdx);
        #else
            staticEnt        = ent_xlat[checkEnts[i]];
            staticEntType    = ENTTYPE(staticEnt);
        #endif

        staticEntCap = staticEnt->motion->capsule;

        radiusTotal = movingEntCap.radius + staticEntCap.radius;
        maxCollDistance = movingEntCap.max_coll_dist + staticEntCap.max_coll_dist;
        maxCollDistanceSQR = SQR(maxCollDistance);

        #if SERVER
            if(entCollTimes.enabled)
            {
                const int size = ARRAY_SIZE(staticEnt->pos_history.pos);
                S32 count = staticEnt->pos_history.count - (staticEnt->pos_history.end - staticEnt->pos_history.net_begin + size) % size;
                S32 lo = 0;
                S32 hi = count - 1;U32 find_time;
                F32 distSQR = distance3Squared(movingEntLoc, ENTPOS(staticEnt));
                F32 movedDist = maxCollDistance + staticEnt->dist_history.total;

                // If the distance between us is greater than the distance I've moved in the
                //   last x seconds plus the max distance that can have a collision,
                //   then collision is impossible.

                if(distSQR > SQR(movedDist))
                {
                    continue;
                }

                PERFINFO_AUTO_START("backInterp", 1);
                {
                    PERFINFO_AUTO_START("binSearch", 1);
                    {
                        pos_end = staticEnt->pos_history.net_begin + ENT_POS_HISTORY_SIZE;
                        pos_pos = staticEnt->pos_history.pos;
                        pos_qrot = staticEnt->pos_history.qrot;
                        pos_time = staticEnt->pos_history.time;

                        lo_idx = (pos_end - lo) % ENT_POS_HISTORY_SIZE;
                        hi_idx = (pos_end - hi) % ENT_POS_HISTORY_SIZE;

                        // Find where this entity was in history.

                        if(ENTINFO_BY_ID(staticEntIdx).send_on_odd_send)
                        {
                            find_time = entCollTimes.client_abs_slow;
                        }
                        else
                        {
                            find_time = entCollTimes.client_abs;
                        }

                        while(hi - lo > 1)
                        {
                            int mid = (hi + lo) / 2;
                            int mid_idx = (pos_end - mid) % ENT_POS_HISTORY_SIZE;
                            U32 diff = find_time - pos_time[hi_idx];
                            U32 max_diff = pos_time[mid_idx] - pos_time[hi_idx];

                            if(diff >= max_diff)
                            {
                                hi = mid;
                                hi_idx = mid_idx;
                            }
                            else
                            {
                                lo = mid;
                                lo_idx = mid_idx;
                            }
                        }
                    }
                    {
                        // Interp the binary search.
                        
                        U32 max_diff = pos_time[lo_idx] - pos_time[hi_idx];
                        U32 diff = find_time - pos_time[hi_idx];

                        if(diff <= max_diff)
                        {
                            lo_factor = (F32)diff / (F32)max_diff;

                            scaleVec3(pos_pos[hi_idx],        1 - lo_factor,    staticEntLoc);
                            scaleAddVec3(pos_pos[lo_idx],    lo_factor,        staticEntLoc,    staticEntLoc);

                            foundLocAtTime = 1;
                        }
                    }
                    PERFINFO_AUTO_STOP();
                }
                PERFINFO_AUTO_STOP();

                if(!foundLocAtTime)
                {
                    copyVec3(staticEnt->net_pos, staticEntLoc);
                }
            }
            else
            {
                copyVec3(ENTPOS(staticEnt), staticEntLoc);
            }
        #else
            copyVec3(ENTPOS(staticEnt), staticEntLoc);
        #endif

        if(distance3Squared(staticEntLoc, movingEntLocMoved) <= maxCollDistanceSQR)
        {
            Vec3 staticEntIntersect;
            Vec3 movingEntIntersect;
            Vec3 staticEntDirVec;
            Quat eQROT;
            
            #if CLIENT
                {
                    copyQuat(staticEnt->cur_interp_qrot, eQROT);

                    switch(staticEntCap.dir){
                        xcase 0:quatToMat3_0(eQROT, staticEntDirVec);
                        xcase 1:quatToMat3_1(eQROT, staticEntDirVec);
                        xcase 2:quatToMat3_2(eQROT, staticEntDirVec);
                    }
                }
            #endif

            #if SERVER
                if(!foundLocAtTime)
                {
                    if(entCollTimes.enabled)
                    {
                        copyQuat(staticEnt->net_qrot, eQROT);

                        switch(staticEntCap.dir){
                            xcase 0:quatToMat3_0(eQROT, staticEntDirVec );
                            xcase 1:quatToMat3_1(eQROT, staticEntDirVec );
                            xcase 2:quatToMat3_2(eQROT, staticEntDirVec );
                        }
                    }
                    else
                    {
                        // This is what happens when moving entity isn't a player.

                        switch(staticEntCap.dir){
                            xcase 0:copyVec3(ENTMAT(staticEnt)[0], staticEntDirVec);
                            xcase 1:copyVec3(ENTMAT(staticEnt)[1], staticEntDirVec);
                            xcase 2:copyVec3(ENTMAT(staticEnt)[2], staticEntDirVec);
                        }
                    }
                }
                else
                {
                    if(staticEntType == ENTTYPE_PLAYER)
                    {
                        staticEntDirVec[0] = 0;
                        staticEntDirVec[1] = 1;
                        staticEntDirVec[2] = 0;
                    }
                    else
                    {
                        PERFINFO_AUTO_START("interp2", 1);
                        {
                            S32 i;

                            for(i = 0; i < 3; i++)
                            {
                                /*
                                float diff = lo_factor * subAngle(pos_pyr[lo_idx][i], pos_pyr[hi_idx][i]);
                                ePYR[i] = addAngle(pos_pyr[hi_idx][i], diff);
                                */
                                quatInterp(lo_factor, pos_qrot[hi_idx], pos_qrot[lo_idx], eQROT);
                            }

                            switch(staticEntCap.dir){
                                xcase 0:quatToMat3_0(eQROT, staticEntDirVec);
                                xcase 1:quatToMat3_1(eQROT, staticEntDirVec);
                                xcase 2:quatToMat3_2(eQROT, staticEntDirVec);
                            }
                        }
                        PERFINFO_AUTO_STOP();
                    }
                }
            #endif

            if(staticEntCap.dir)
            {
                // Move one radius length along the dir vector for Y and Z capsules,
                // to align the tip of the capsule with the entity location.

                scaleAddVec3(staticEntDirVec, staticEntCap.radius, staticEntLoc, staticEntLoc);
            }
            else
            {
                // Capsule direction is X vector, so center it.
                
                F32 scale = staticEntCap.radius * -0.5;

                scaleAddVec3(staticEntDirVec, scale, staticEntLoc, staticEntLoc);
            }

            if(staticEntCap.has_offset)
            {
                #if SERVER
                    Vec3 offsetX;
                    if(entCollTimes.enabled)
                    {
                        quatRotateVec3(eQROT, staticEntCap.offset, offsetX);
                    }
                    else
                    {
                        mulVecMat3(staticEntCap.offset, ENTMAT(staticEnt), offsetX);
                    }
                    addVec3(staticEntLoc, offsetX, staticEntLoc);
                #else
                    Vec3 offsetX;
                    quatRotateVec3(eQROT, staticEntCap.offset, offsetX);
                    addVec3(staticEntLoc, offsetX, staticEntLoc);
                #endif
            }

            mag = coh_LineLineDist( staticEntLoc,        staticEntDirVec, staticEntCap.length,    staticEntIntersect,
                                movingEntLocMoved,    movingEntDirVec, movingEntCap.length,    movingEntIntersect);

            if(mag < SQR(radiusTotal))
            {
                // Capsules collided.
                 
                #define PRINT_ENTITY_COLLISIONS 0

                was_coll = 1;

                #if PRINT_ENTITY_COLLISIONS
                    #define TRIPLET "%1.8f, %1.8f, %1.8f"
                    #define QUARTET "%1.8f, %1.8f, %1.8f, %1.8f"
                    
                    #if SERVER
                        if(entCollTimes.enabled)
                        {
                    #endif
                            printf(    "%8d : ("TRIPLET")\tvs\n"
                                    "           ("TRIPLET")\n"
                                    "           quat ("QUARTET")%s\n"
                                    "           mag %1.8f, %1.8f\n",
                                    #if SERVER
                                        entCollTimes.client_abs,
                                    #else
                                        global_state.client_abs,
                                    #endif
                                    vecParamsXYZ(movingEntLocMoved),
                                    vecParamsXYZ(eLoc),
                                    quatParamsXYZW(eQROT),
                                    found_loc ? "" : " - end",
                                    mag,
                                    sqrt(mag));
                    #if SERVER
                        }
                    #endif
                #endif

                mag = sqrt(mag);

                mag = ((int)(mag * 1024 + 0.5)) / 1024.0;

                // Get push direction.
                
                subVec3(staticEntIntersect, movingEntIntersect, dv); 
                
                if(mag < 0.1)
                {
                    dv[0] = dv[2] = 0.1f;
                    mag = lengthVec3(dv);
                }

                // Get push distance.
                
                push = (radiusTotal - mag); 

                if(push > 0.01){
                    eCollType collisionType = staticEnt->seq->type->collisionType;

                    was_coll = 1;

                    scale = push/mag;

                    // Create push vector.
                    
                    scaleVec3(dv, scale, dv); 

                    if( dv[1] > 0 ) // don't push down ever (otherwise ground collision test will put entity back)
                        dv[1] = 0;  // this may need to be revisited

                     #if SERVER
                         if(    doApplyBounce && staticEnt->erOwner && staticEnt->seq->type->pushable && staticEnt->move_type == MOVETYPE_WALK)
                         {
                             EntityRef    erStaticEntOwner = staticEnt->erOwner;
                             Entity*        staticEntOwner = erStaticEntOwner ? erGetEnt(erStaticEntOwner) : NULL;

                            if(staticEntOwner && ENTTYPE(staticEntOwner) == ENTTYPE_PLAYER && lengthVec3Squared(staticEnt->motion->vel) < SQR(0.1))
                            {
                                F32 scale = 0;
                                Vec3 staticEntPush;

                                if (vec3IsZeroXZ(dv))
                                {
                                    //    sitting right on top of entity
                                    //    small nudge in a random direction
                                    float dir_radian = RAD(rand() % 360);
                                    staticEntPush[0] = 0.05f * sin(dir_radian);
                                    staticEntPush[2] = 0.05f * cos(dir_radian);
                                }
                                else
                                {
                                    if(erMovingEntOwner == erStaticEntOwner) // pet-pet collision
                                        scale = .05 / lengthVec3XZ(dv);
                                    else if( erMovingEnt == erStaticEntOwner ) // owner-pet collision
                                        scale = .25f / lengthVec3XZ(dv);
                                    else if( staticEntOwner ) // pet - somone else collision
                                    {
                                        if( staticEnt->pchar && movingEnt->pchar && character_TargetIsFoe( staticEnt->pchar, movingEnt->pchar ) ) // if they are enemies, pets don't want to move
                                            scale = .05 / lengthVec3XZ(dv);
                                        else
                                            scale = 2.f / lengthVec3XZ(dv);
                                    }

                                    scaleVec3(dv, scale, staticEntPush);
                                }
                                 staticEntPush[1] = 0;
                                 addVec3(staticEnt->motion->vel, staticEntPush, staticEnt->motion->vel);
                            }
                         }
                     #endif

                      if( doApplyBounce &&
                         collisionType != SEQ_ENTCOLL_CAPSULE &&
                         collisionType != SEQ_ENTCOLL_DOOR &&
                         collisionType != SEQ_ENTCOLL_LIBRARYPIECE )
                    {
                        //Do marshmallow collision

                        movingEnt->motion->bouncing = 1;
                        
                        applyBounceToCollision( staticEnt, movingEnt, dv, eQROT );

                        SETB_MOTION( movingEnt->seq->state, STATE_BOUNCING );
                    }
                    else //Do regular collsion
                    {
                        #if SERVER
                        if(    doApplyBounce && 
                            staticEnt->erOwner && 
                            staticEnt->seq->type->pushable && 
                            staticEnt->move_type == MOVETYPE_WALK && erMovingEnt == staticEnt->erOwner )
                        #elif CLIENT
                        if(    doApplyBounce && 
                            staticEnt->seq->type->pushable && 
                            staticEnt->move_type == MOVETYPE_WALK &&
                            entIsPet(staticEnt) && movingEnt == playerPtr() )
                        #endif
                        {
                            // do not move owner
                        }
                        else
                        {
                            subVec3(movingEntLocMoved, dv, movingEntLocMoved); // push entity
                            subVec3(movingEntLoc, dv, movingEntLoc);
                        }
                    }

                    //record what just happened
                    movingEnt->motion->tickOfLastEntCollision = movingEnt->motion->tickCounter;
                    movingEnt->motion->lastEntCollidedWith = staticEnt->CONTEXT_SENSITIVE_ID;
                    //end record


                    #if PRINT_ENTITY_COLLISIONS
                        #if SERVER
                            if(entCollTimes.enabled)
                            {
                        #endif
                                printf(    "  bounce:    %d\n"
                                        "  dir1:      "TRIPLET"\n"
                                        "  dir2:      "TRIPLET"\n"
                                        "  intersect1:"TRIPLET"\n"
                                        "  intersect2:"TRIPLET"\n"
                                        "  radius1:   %1.8f\n"
                                        "  radius2:   %1.8f\n"
                                        "  push:      "TRIPLET" (%1.8f / %1.8f)\n",
                                        doApplyBounce,
                                        vecParamsXYZ(movingEntDirVec),
                                        vecParamsXYZ(staticEntDirVec),
                                        vecParamsXYZ(intersect1),
                                        vecParamsXYZ(intersect2),
                                        movingEntCap.radius,
                                        staticEntCap.radius,
                                        vecParamsXYZ(dv),
                                        scale,
                                        push);
                        #if SERVER
                            }
                        #endif
                    #endif
                }
            }
        }
    }

    PERFINFO_AUTO_STOP();

    entUpdatePosInterpolated(movingEnt, movingEntLoc);

    return was_coll;
}

#ifdef SERVER
void addForceToEntity( Entity * e, Vec3 vel )
{
    if( ENTTYPE_PLAYER == ENTTYPE(e) )
    {
        if(e && e->client)
        {
            FuturePush* fp = addFuturePush(&e->client->controls, SEC_TO_ABS(0));
            copyVec3( vel, fp->add_vel );
        }
    }
    else
    {
        FuturePush fp = {0};
        copyVec3( vel, fp.add_vel );
        pmotionApplyFuturePush( e->motion, &fp);
    }
}


void collGridUpdateHandleNearbyEntities( DefTracker * tracker, bool added )
{
    Entity * nearbyEnts[MAX_ENTITIES_PRIVATE];
    int nearbyCount, i;
    Vec3 vel = { 0, 0.001, 0 };

    //Get all the nearby entities
    nearbyCount = entWithinDistance( tracker->mat, tracker->radius*2, nearbyEnts, 0, 0);

    for( i = 0 ; i < nearbyCount ; i++ )
    {
        Entity * e = nearbyEnts[i];

        if( added )
        {
            //TO DO entHandleAddedCollision()
        }
        else //removed 
        {
            if( e->motion && !e->motion->input.flying && !e->motion->falling && !e->noPhysics )
                addForceToEntity( e, vel ); //Just give em a nudge to wake up the physics
        }
    }
}
#endif

void entMotionFreeColl(Entity *e)
{
    if (!e->coll_tracker)
        return;

    #if SERVER
        beaconDestroyDynamicConnections(&e->beaconDynamicInfo);
        beaconClearBadDestinationsTable();
        collGridUpdateHandleNearbyEntities( e->coll_tracker, false );
    #endif

    // TO DO coll_tracker needs to use game_state.groupDefVersion or it can crash when editing a map.
    // I'm not doing it right now cause it seems time consuming and Bruce is threatening to make the trackers
    // handle based
    groupRefGridDel(e->coll_tracker);

    //If you made your own def, free it. Slightly Hack, assumes GEO_COLLSION is one and only made def
    if( e->coll_tracker->def && e->coll_tracker->def->model && 0 == stricmp( e->coll_tracker->def->model->name, "GEO_Collision" ) )
        free(e->coll_tracker->def);

    free(e->coll_tracker);
    e->coll_tracker = 0;
    e->checked_coll_tracker = 0;
    
    gridCacheInvalidate();
}

void setNoCollideOnTracker( DefTracker * tracker, int set )
{
    int i;
    tracker->no_collide = set; 
    for(i=0;i<tracker->count;i++)
    {
        DefTracker *ent = &tracker->entries[i];
        setNoCollideOnTracker( ent, set );
    }
}

bool isEntCurrentlyCollidable( Entity * e )
{
    if( !e->seq )
        return false;
 
    //if( getMoveFlags(e->seq, SEQMOVE_WALKTHROUGH ) )
        //return false;

    if( e->seq->noCollWithHealthFx == true )
        return false;

    if( e->seq->type->collisionType == SEQ_ENTCOLL_NONE )
        return false;

    if( getMoveFlags( e->seq, SEQMOVE_NOCOLLISION ) )
        return false;

    return true;
}

#if SERVER
void entMotionSetDynamicBeaconInfo(Entity* e, S32 enableConnections){
    beaconSetDynamicConnections(&e->beaconDynamicInfo,
                                e,
                                e->coll_tracker,
                                enableConnections,
                                ENTTYPE(e) == ENTTYPE_DOOR);
}
#endif

void entMotionUpdateCollGrid(Entity *e)
{
    const SeqMove *move;

    if(global_motion_state.noDynamicCollisions){
        return;
    }

    move = e->seq->animation.move;
    if (!move)
        return;

    // currently assuming move #0 (ready) is closed state
    if (!getMoveFlags(e->seq, SEQMOVE_WALKTHROUGH) && e->seq->noCollWithHealthFx == false ) //move->priority && TSTB(move->requires,STATE_SHUT))
    {
        if (!e->checked_coll_tracker && !e->coll_tracker)
        {
            Mat4 mat;

            #if SERVER
                // Set up the net vars so that the server-side dynamic collision is placed the same as on the client.

                if(!ENTINFO(e).net_prepared)
                {
                    entSendSetNetPositionAndOrientation(e);
                }

                quatToMat(e->net_qrot, mat);
                //createMat3YPR(mat, e->net_pyr);
                copyVec3(e->net_pos, mat[3]);

                if(!ENTINFO(e).net_prepared)
                {
                    entInitNetVars(e);
                }
            #else
                copyMat4(ENTMAT(e), mat);
            #endif

            if( e->seq->type->collisionType == SEQ_ENTCOLL_DOOR )
            {
                // If I'm supposed to be using the GEO_COLLSION file in the default graphics file.

                e->coll_tracker = createCollTracker(e->seq->type->graphics, 0, mat);
            }
            else if( e->seq->type->collisionType == SEQ_ENTCOLL_LIBRARYPIECE )
            {
                // If you are supposed to be using the worldGroup.
                if (e->seq->worldGroupPtr == NULL)
                {
                    seqUpdateWorldGroupPtr(e->seq,e); // Try to get it set up
                }
                
                if (e->seq->worldGroupPtr != NULL)
                    e->coll_tracker = createCollTracker(0, e->seq->worldGroupPtr, mat);
            }

            if (e->coll_tracker)
            {
                #if CLIENT
                    e->coll_tracker->ent_id = e->svr_idx;
                #else
                    e->coll_tracker->ent_id = e->owner;
                #endif
                //e->svr_idx an be -1, but that means the thing is about to die.
            }
        }
        if (!e->checked_coll_tracker)
        {
            e->checked_coll_tracker = 1;
            if (e->coll_tracker)
            {
                setNoCollideOnTracker( e->coll_tracker, 0 );

                #if SERVER
                if(!e->idDetail)
                {
                    entMotionSetDynamicBeaconInfo(e, 0);
                    beaconClearBadDestinationsTable();
                }
                //collGridUpdateHandleNearbyEntities( e->coll_tracker, true );
                #endif
            }
        }
    }
    else if (e->checked_coll_tracker && e->coll_tracker)   
    {
        int train = false;
        
        if (e->seq && e->seq->type && e->seq->type->seqTypeName && (stricmp(e->seq->type->seqTypeName, "door_train")  == 0|| stricmp(e->seq->type->seqTypeName, "door_subwaytrain")  == 0))
            train = true;
        
          e->checked_coll_tracker = 0;

        // Hack!: This keeps the trains running on time.  
        //            It prevents the door from becoming unclickable when the 
        //            train is in the station

        if (!train)    
            setNoCollideOnTracker( e->coll_tracker, 1 );
     
        #if SERVER
            entMotionSetDynamicBeaconInfo(e, 1);
            beaconClearBadDestinationsTable();
            //collGridUpdateHandleNearbyEntities( e->coll_tracker, true );
        #endif
    }
}

#ifdef CLIENT
#include "graphics/groupdraw.h"
#include "graphics/gfx.h"
#include "seq/tricks.h"
#include "graphics/zOcclusion.h"
#include "graphics/gfxwindow.h"
#include "render/renderprim.h"
#include "edit/edit_drawlines.h"
#include "group/grouptrack.h"

static void jiggleMat4(Mat4 mat){
    mat[3][0] += ((rand() % 101) - 50) * 0.0005;
    mat[3][1] += ((rand() % 101) - 50) * 0.0005;
    mat[3][2] += ((rand() % 101) - 50) * 0.0005;
}

void entMotionDrawDebugCollisionTrackers(void)
{
    int i;
    int wireframe_save = game_state.wireframe;
    game_state.wireframe = 1;

    for(i=1;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE ) {
            const SeqMove    *move;
            Entity            *e = entities[i];

            move = e->seq->animation.move;
            if (!move)
                continue;

            if (e->coll_tracker) {
                DefTracker*    tracker = e->coll_tracker;
                Mat4        viewmat, matx={0};
                TrickNode*    trick;
                S32            old_inherit = tracker->inherit;
                S32            inSamePlace =    sameVec3(tracker->mat[3], ENTPOS(e)) &&
                                            sameVec3(tracker->mat[0], ENTMAT(e)[0]) &&
                                            sameVec3(tracker->mat[1], ENTMAT(e)[1]) &&
                                            sameVec3(tracker->mat[2], ENTMAT(e)[2]);

                tracker->inherit = 1;

                gfxSetViewMat(cam_info.cammat,viewmat,0);
                mulMat4(viewmat,tracker->mat,matx);

                trick = allocTrackerTrick(tracker);
                trick->flags2 |= TRICK2_WIREFRAME;

                if(tracker->no_collide){
                    trick->trick_rgba[0] = 127;
                    trick->trick_rgba[1] = 255;
                    trick->trick_rgba[2] = 127;
                    trick->trick_rgba[3] = 255;
                }else{
                    trick->trick_rgba[0] = 127;
                    trick->trick_rgba[1] = 127;
                    trick->trick_rgba[2] = 255;
                    trick->trick_rgba[3] = 255;
                }

                if(game_state.debug_collision & 2){
                    jiggleMat4(matx);
                }
                
                groupDrawDefTracker(tracker->def,tracker,matx,0,0,1,0,-1);
                
                if(!inSamePlace){
                    trick->trick_rgba[0] = 255;
                    trick->trick_rgba[1] = 0;
                    trick->trick_rgba[2] = 0;
                    trick->trick_rgba[3] = 255;
    
                    mulMat4(viewmat,ENTMAT(e),matx);

                    if(game_state.debug_collision & 2){
                        jiggleMat4(matx);
                    }

                    groupDrawDefTracker(tracker->def,tracker,matx,0,0,1,0,-1);
                }

                tracker->inherit = old_inherit;
            }
        }
    }
    game_state.wireframe = wireframe_save;
}

void entMotionCheckDoorOccluders(void)
{
    int i;

    if (!game_state.zocclusion)
        return;

    PERFINFO_AUTO_START("entMotionCheckDoorOccluders", 1);

    for(i=1;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE ) {
            const SeqMove    *move;
            Entity            *e = entities[i];
            GroupDef        *def;

            if (!e->coll_tracker || !e->coll_tracker->def)
                continue;

            if (e->seq->alphasort)
                continue;

            if (strcmp(e->name, "Dr")!=0)
                continue;

            move = e->seq->animation.move;
            if (!move)
                continue;

            def = e->coll_tracker->def;

            if (move == e->seq->info->moves[0])
            {
                Mat4 viewmat, matx={0};
                gfxSetViewMat(cam_info.cammat,viewmat,0);
                mulMat4(cam_info.viewmat,ENTMAT(e),matx);

                if (!gfxSphereVisible(matx[3], def->radius))
                    continue;

                if (zoTest(def->min, def->max, matx, 1, -1, 0))
                    zoCheckAddOccluder(def->model, matx, -1);

//                if (0)
//                    drawBox3D(def->min, def->max, ENTMAT(e), 0xffff0f0f, 2);
            }
        }
    }

    PERFINFO_AUTO_STOP();
}

#endif

void entMotionClearAllWorldGroupPtr(void)
{
    int     i;

    for(i=0;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE
            && entities[i]->seq
            && entities[i]->seq->type->collisionType == SEQ_ENTCOLL_LIBRARYPIECE)
        {
            entities[i]->seq->worldGroupPtr = NULL;
        }
    }
}

void entMotionFreeCollAll(void)
{
    int     i;

    for(i=0;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE)
            entMotionFreeColl(entities[i]);
    }
}

int velIsZero(Vec3 vel){
    if(vel[0] == 0.0 && vel[1] == 0.0 && vel[2] == 0.0)
        return 1;
    else
        return 0;
}

int entCanSetPYR( Entity *e )
{
    if(    (e->pchar && (e->pchar->attrCur.fSleep > 0.0f ||
        e->pchar->attrCur.fHeld > 0.0f ||
        (ENTTYPE(e) == ENTTYPE_PLAYER 
        && e->pchar->attrCur.fTerrorized > 0.0f
        && e->pchar->fTerrorizePowerTimer >= 0.0) ||
        e->pchar->attrCur.fHitPoints <= 0.0f)) ||
        (e->seq && getMoveFlags(e->seq, SEQMOVE_CANTMOVE)))
    {
        return 0;
    }

    return 1;
}

#ifdef SERVER
int entCanSetInpVel( Entity *e )
{
    if(ENTAI(e)->doNotMove || !entCanSetPYR(e) ||
        (e->pchar
            && e->pchar->attrCur.fImmobilized > 0.0f))
    {
        return 0;
    }

    return 1;
}
#endif

static int physCount;

static void DoPhysicsOnce(Entity *e, const Mat3 control_mat)
{
    MotionState* motion = e->motion;
    
    physCount++;

    zeroVec3(motion->addedVel);

    PERFINFO_AUTO_START("entWorldCollide", 1);
        entWorldCollide(e, control_mat);
    PERFINFO_AUTO_STOP();

    // This block is a temporary hack!  Please figure out what's going on and fix the root problem!
    if (lengthVec3Squared(motion->addedVel) > 10000000000)
    {
        AssertErrorf(__FILE__, __LINE__, "We're about to add a ton of velocity.  Stopping that from happening.  Tell Andy as soon as you see this.  COH-27814");
        zeroVec3(motion->addedVel);
    }

    if(!vec3IsZero(motion->addedVel)){
        motion->on_surf = 0;
        motion->was_on_surf = 0;
        motion->falling = 1;

        addVec3(motion->addedVel, motion->vel, motion->vel);
    }

    // This block is a temporary hack!  Please figure out what's going on and fix the root problem!
    if (lengthVec3Squared(motion->vel) > 10000000000)
    {
        AssertErrorf(__FILE__, __LINE__, "We have a ton of velocity.  Stopping that from happening.  Tell Andy as soon as you see this.  COH-27814");
        zeroVec3(motion->vel);
    }
}

static void DoPhysics(Entity *e, const Mat3 control_mat)
{
    #define MAX_SPEED (1.f)
    F32                mag;
    F32                timestep;
    int                i;
    int                numsteps;
    MotionState*    motion = e->motion;

    //Vec3        add_vel;

    timestep = e->timestep;

    mag = lengthVec3(motion->vel) * timestep;

    if (mag < MAX_SPEED && timestep < 5)
    {
        //#if CLIENT
        //    Vec3 source;
        //    Vec3 target1;
        //    Vec3 target2;
        //    Vec3 intersection;
        //    copyVec3(motion->last_pos, source);
        //    addVec3(source, add_vel, target1);
        //    //entDebugAddLine(source, 0xffffffff, target1, motion->falling ? 0xff0000ff : 0xffff0000);
        //    scaleVec3(motion->inp_vel, e->timestep, target2);
        //    addVec3(source, target2, target2);
        //    //entDebugAddLine(source, 0xffffffff, target2, 0xffff00ff);
        //#endif

        DoPhysicsOnce(e, control_mat);

        //#if CLIENT
        //    //entDebugAddLine(source, 0xffffffff, e->mat[3], motion->falling ? 0xff00ffff : 0xff00ff00);
        //    setVec3(intersection, target1[0], e->mat[3][1], target1[2]);
        //    //entDebugAddLine(target1, 0xff777777, intersection, 0xff777777);
        //    //entDebugAddLine(e->mat[3], 0xff777777, intersection, 0xff777777);
        //#endif
    }
    else
    {
        numsteps =( mag + 0.99) / MAX_SPEED;
        if (numsteps > 5)
            numsteps = 5; // don't want to seize up and die
        else if(timestep >= 5)
            numsteps = 5;
        e->timestep /= numsteps;

        //Temp debug
        if( !FINITE( e->timestep ) || ( e->timestep < 0.0f || e->timestep > 100000.f ) )
        {
            e->timestep = 1.0;
        } //ENd temp debug

        for(i=0;i<numsteps;i++)
        {
            //#if CLIENT
            //    Vec3 source;
            //    Vec3 target1;
            //    Vec3 target2;
            //    Vec3 intersection;
            //    copyVec3(motion->last_pos, source);
            //    addVec3(source, add_vel, target1);
            //    //entDebugAddLine(source, 0xffffffff, target1, motion->falling ? 0xff0000ff : 0xffff0000);
            //    scaleVec3(motion->inp_vel, e->timestep, target2);
            //    addVec3(source, target2, target2);
            //    //entDebugAddLine(source, 0xffffffff, target2, 0xffff00ff);
            //#endif

            DoPhysicsOnce(e, control_mat);

            //#if CLIENT
            //    //entDebugAddLine(source, 0xffffffff, e->mat[3], motion->falling ? 0xff00ffff : 0xff00ff00);
            //    setVec3(intersection, target1[0], e->mat[3][1], target1[2]);
            //    //entDebugAddLine(target1, 0xff777777, intersection, 0xff777777);
            //    //entDebugAddLine(e->mat[3], 0xff777777, intersection, 0xff777777);
            //#endif
        }
        //scaleVec3(motion->vel,1.f/e->timestep,motion->vel);
        e->timestep *= numsteps;

        //Temp debug
        if( !FINITE( e->timestep ) || ( e->timestep < 0.0f || e->timestep > 100000.f ) )
        {
            AssertErrorf(__FILE__,__LINE__, "Timestep for this entity is very wrong %f.  Tell Andy as soon as you see this.  COH-27814", e->timestep );
            e->timestep = 1.0;
        } //ENd temp debug
    }

    if(e->seq && motion->falling){
        SETB_MOTION(e->seq->state, STATE_AIR);

        // poz - I changed this slightly. Let me know if it goes wonky.
        if( motion->vel[1] > 0.00 && motion->vel[1] < 0.3 ) //oooh, now this is some nice codin.
        {
            SETB_MOTION( e->seq->state, STATE_APEX );
        }
        else if( motion->vel[1] < 0.00 )
        {
            if( !( e->move_type & MOVETYPE_JUMPPACK ) )
                motion->jumping = 0;
            //setGravity( e ); BRGRAV
            CLRB_MOTION(e->seq->state, STATE_JUMP);
        }

        if(motion->vel[1]<-1.0f && entHeight(e, 10.0f)<10.0f)
        {
            SETB_MOTION( e->seq->state, STATE_LANDING );
        }

    }
    //MW added this because STATE_FLY and STATE_AIR should always get set at the same time.
    //Shannon says setting it here as well as pmotionSetState should be redundant. Something to look into.
    if(e->seq)
    {
        if(motion->jumping && e->move_type & MOVETYPE_JUMPPACK)
        {
            int bit = seqGetStateNumberFromName("PRESTIGEJUMPJETPACK");
            if (bit >= 0)
                SETB_MOTION(e->seq->state, bit);
        }
    }
}


void entMoveNoColl(Entity *e)
{
    Vec3            dv;
    F32                speed;
    MotionState*    motion = e->motion;
    Vec3            newpos;

    motion->falling = 1;
    zeroVec3(motion->vel);

    motion->move_time += e->timestep;
    speed = 1 * (motion->move_time + 6) / 6.f;
    if (speed > 50.f)
        speed = 50.f;
    speed *= e->timestep;
    scaleVec3(motion->input.vel, speed, dv);
    addVec3(dv, ENTPOS(e), newpos);
    entUpdatePosInterpolated(e, newpos);
    copyVec3(ENTPOS(e), motion->last_pos);

    if (nearSameVec3(motion->input.vel, zerovec3))
        motion->move_time = 0;
}

#ifdef SERVER
static void doCritterFacing(Entity *e, Vec3 last_pos)
{
    MotionState* motion = e->motion;

    if(ENTTYPE(e) != ENTTYPE_PLAYER && ENTAI(e) && ENTAI(e)->motion.type == AI_MOTIONTYPE_GROUND)
    {
        Vec3 diff;
        F32 diffCos;

        subVec3(ENTPOS(e), last_pos, diff);

        diffCos = dotVec3(diff, ENTMAT(e)[2]) / lengthVec3(diff);

        // If actual movement was less than 45 degrees from direction I'm facing,
        // then face towards movement direction


        //ST: Not sure this is safe with pyr's
        if(diffCos > 0.2f)
        {
            Vec3 pyr;
            Mat3 newmat;

            getMat3YPR(ENTMAT(e), pyr);

            pyr[1] = getVec3Yaw(diff);

            createMat3YPR(newmat, pyr);
            entSetMat3(e, newmat);
        }
    }
}
#endif

static void checkJump(Entity*e, SurfaceParams* surf)
{
    int                want_jump = 0;
    int                newJump = 0;
    MotionState*    motion = e->motion;

    if (motion->jump_time > 0)
        --motion->jump_time;

    if (e->move_type & MOVETYPE_FLY)
        return;

    if (motion->input.vel[1] > 0.001)
    {
        // this seems to say that you want to jump if you have an upward input velocity and 
        //        you're not stunned and you're holding the jump button or
        //        you're on a surface or you're using a jump pack
        if(!motion->input.stunned && (motion->jump_held || motion->surf_normal[1] > 0.3 || (e->move_type & MOVETYPE_JUMPPACK)))
            want_jump = 1;
        else
            motion->input.vel[1] = 0;
    }

    //Roughly simulate slowing as you go up. (JumpPacks don't slow.)

    if(motion->vel[1] > 0 && motion->jump_held && !want_jump && !(e->move_type & MOVETYPE_JUMPPACK))
    {
        float reduce_vel = e->motion->vel[1];

        motion->jump_held = 0;
        motion->was_on_surf = 0;

        if(reduce_vel > 1.5)
            reduce_vel = 1.5;

        e->motion->vel[1] -= reduce_vel * 0.5;
    }

    #if CLIENT
        //xyprintf( 10, 10, "jumping %d  want_jump %d  motion->jump_time %d motion->vel[1] %f, motion->inp_vel[1] %f", motion->jumping, want_jump, motion->jump_time, motion->vel[1], motion->inp_vel[1]);
    #endif

    newJump = 0;
    if(    !motion->falling &&
        !motion->input.flying &&
        motion->jump_time <= 0 &&
        !(e->move_type & MOVETYPE_JUMPPACK))
    {
        newJump = 1;
    }

    if(want_jump && !motion->jumping)
    {
        if(newJump && !motion->jump_still_held)
        {
            //printf("jumping!\n");
            motion->jumping             = 1;
            motion->jump_not_landed        = 1;
            motion->jump_height         = ENTPOSY(e);
            motion->jump_time            = 15;
            motion->jump_held            = 1;
            motion->jump_still_held        = 1;
        }
        else
        {
            motion->input.vel[1] = 0;
        }
    }
    else if( motion->jumping && !(e->move_type & MOVETYPE_JUMPPACK) )
    {
        if(motion->stuck_head
            || (ENTPOSY(e) - motion->jump_height) >= motion->input.max_jump_height
            || !want_jump
            || !motion->falling)
        {
             motion->jumping = 0; //hit max jump height, turn gravity on.
        }
    }

    // Jumppack.

    if(e->move_type & MOVETYPE_JUMPPACK)
    {
        if( !want_jump )
            motion->jumping = 0;

        if( want_jump )
        {
            // this should match the jump initiation code above
            motion->jumping             = 1;
            motion->jump_not_landed        = 1;
            motion->jump_height         = ENTPOSY(e);
            motion->jump_time            = 15;
            motion->jump_held            = 1;
            motion->jump_still_held        = 1;

            // reset the height to prevent massive falling damage
            motion->highest_height        = ENTPOSY(e);
        }

        if(motion->jumping)
        { 
//            SETB_MOTION(e->seq->state, STATE_JETPACK);
            int bit = seqGetStateNumberFromName("PRESTIGEJUMPJETPACK");
            if (bit >= 0)
                SETB_MOTION(e->seq->state, bit);
        }
        else
        {
//            CLRB_MOTION(e->seq->state, STATE_JETPACK);
            int bit = seqGetStateNumberFromName("PRESTIGEJUMPJETPACK");
            if (bit >= 0)
                CLRB_MOTION(e->seq->state, bit);
        }
    }
    else
    {
        if(motion->input.vel[1] && surf->gravity && motion->falling)
        {
            motion->input.vel[1] = 0;
        }
    }
}

static void truncateVelocity(MotionState *motion)
{
    float truncateThreshold = 0.00001;
    int i;
    for(i = 0; i < 3; i++)
    {
        if(motion->vel[i] < truncateThreshold && motion->vel[i] > -truncateThreshold)
        {
            motion->vel[i] = 0.0;
        }
    }
}

static void calcNewVelocity(Entity *e,SurfaceParams *surf)
{
    int                i,inp_sign,cur_sign;
    F32                inp_speed,cur_speed,speed;
    Vec3            cur_vel,max_inp_speed,friction_dir,inp_vel,dv;
    MotionState*    motion = e->motion;
    F32                friction_mag;

    friction_mag = surf->friction * e->timestep;

    if(friction_mag > 1)
        friction_mag = 1;
    else if(friction_mag < 0)
        friction_mag = 0;

    scaleVec3(motion->vel, -friction_mag, friction_dir);

    //if(!motion->flying)
    //{
    //    friction_dir[1] = 0;
    //}

    addVec3(motion->vel,friction_dir,cur_vel);                                     // Apply friction to the current entity velocity.

    scaleVec3(motion->input.vel, surf->traction * e->timestep, inp_vel);        // Get velocity increase for this time period.

    //If you are in the air on a jumppack, you don't get extra traction ( maybe give some for fun?)

    if( !motion->input.flying && !(e->move_type & MOVETYPE_JUMPPACK)  ) //No traction cut for jumping off a surface
        inp_vel[1] = motion->input.vel[1];

    //printf( "iv %f t %f JP %d FL %d FALL %d\n", inp_vel[1], surf->traction, airborneJumpPack, motion->flying, motion->falling );

    copyVec3(motion->input.vel, max_inp_speed);

    // Do not normalize + rescale inp_vel here.  pmotion code derives inp_vel by accounting for several speed scaling
    // values.

    for(i=0;i<3;i++)
    {
        inp_sign=cur_sign=1;
        inp_speed = fabs(inp_vel[i]);
        cur_speed = fabs(cur_vel[i]);
        if (inp_vel[i] < 0)
            inp_sign = -1;

        if (cur_vel[i] < 0)
            cur_sign = -1;
        else if(cur_vel[i] == 0)
            cur_sign = inp_sign;

        if (inp_sign != -cur_sign)
        {
            F32        curr,max;

            curr = inp_speed + cur_speed;
            max = MAX(fabs(max_inp_speed[i]),cur_speed);
            speed = MIN(curr,max);
        }
        else
        {
            speed = cur_speed - inp_speed;
        }
        speed *= cur_sign;
        dv[i] = speed;
    }

    {
        static U32 lastSurfFlags;

        if(motion->lastSurfFlags)
            lastSurfFlags = motion->lastSurfFlags;

        if(    dv[1] > 0 &&
            !(lastSurfFlags & (COLL_SURFACESLICK | COLL_SURFACEICY)) &&
            motion->input.vel[1] <= 0 &&
            !motion->jump_not_landed &&
            !motion->input.flying)
        {
            dv[1] = 0;
        }
    }

    copyVec3(dv,motion->vel);
}

static void moveEntity(Entity *e)
{
    Vec3            frame_vel;
    Vec3            new_pos;

    scaleVec3(e->motion->vel,e->timestep,frame_vel);
    addVec3(frame_vel,ENTPOS(e),new_pos);
    entUpdatePosInterpolated(e, new_pos);
}

static void entWalk(Entity* e, const Mat3 control_mat)
{
    MotionState*    motion = e->motion;
    SurfaceParams*    surf_mod;
    SurfaceParams    surf;
    Vec3            last_pos;
    Vec3            vel;
    int                slipperySurface = 0;

       copyVec3(ENTPOS(e), last_pos);

    entWorldGetSurface(e,&surf);
    surf_mod = entWorldGetSurfaceModifier(e);
     entWorldApplySurfMods(surf_mod,&surf);
    entWorldApplyTextureOverrides(&surf, motion->lastSurfFlags);
    
    //Check must be done here, or motion is changed

    if(surf.traction < 0.3 && !motion->falling && !motion->input.flying)
    {
        slipperySurface = 1;
    }
    
    if(motion->bouncing)
    {
        motion->bouncing = 0;
    }

    // Check if jump was released.

    if(motion->input.jump_released)
    {
        motion->input.jump_released = 0;
        motion->jump_still_held = 0;
    }
    
    // Check if I'm trying to jump.

    checkJump(e, &surf);

    landed_on_ground = 0;
    copyVec3(motion->vel, vel);

    #if CLIENT
    //{
    //    Vec3 source;
    //    Vec3 target;
    //    copyVec3(e->mat[3], source);
    //    copyVec3(e->mat[3], target);
    //    target[1] += 0.3;
    //    entDebugAddLine(source, 0xffff7700, target, 0xffff7700);
    //}
    #endif

    DoPhysics(e, control_mat);

    //if(0){
    //    static F32 last_fall;
    //    F32 fall = e->mat[3][1] - last_pos[1];
    //
    //    if(fall < 0)
    //    {
    //        printf("fall: %f\t%f\n", fall, fall - last_fall);
    //    }
    //
    //    last_fall = fall;
    //}
 
    /*/xyprintf( 40, 38, motion->vel[0], motion->vel[1], motion->vel[2] ); 
    if( e == playerPtr() )
    {
    printf( "%f %f %f   ", motion->vel[0], motion->vel[1], motion->vel[2] );  
    printf( "(%f) ", lengthVec3( motion->vel ) ); 
    //printf( "%f %f %f", motion->inp_vel[0], motion->inp_vel[1], motion->inp_vel[2] );
//    printf( "(%f) \n", lengthVec3( motion->inp_vel ) ); 
    }*/
    //IF you are on a slippery surface, and you are moving faster than your input velocity, you must be sliding 
    if( motion->sliding ) 
    {
        #define SLIDING_BIT_OFF_DELAY 7 //max is 7, since bitfield is 3
        //this serves as a timer.  If you lose sliding, don't actually unset the sliding bit for a few frames
        //because it looks bad and jerky when going fast down a hill and getting air.
        motion->sliding++;
        if( motion->sliding > SLIDING_BIT_OFF_DELAY )
            motion->sliding = 0;
    }

    #define MAXSPEED_TO_PLAY_SLIDING_BIT 0.01
    if( slipperySurface && lengthVec3Squared( motion->vel ) > MAXSPEED_TO_PLAY_SLIDING_BIT )    
    {
        //TO DO check for no air bit?
        //if( lengthVec3Squared( motion->vel ) > (lengthVec3Squared( motion->inp_vel )+0.01) )
        {
             motion->sliding = 1;
            SETB_MOTION( e->seq->state, STATE_SLIDING );
        }
    }

    // MS: This code disipates your velocity if you run into something, but it doesn't work quite right.
    //for(i = 0; i < 2; i++){
    //    i = i * 2;
    //    if(    vel[i] > 0 && e->mat[3][i] - last_pos[i] > 0 ||
    //        vel[i] < 0 && e->mat[3][i] - last_pos[i] < 0)
    //    {
    //        motion->vel[i] *= (e->mat[3][i] - last_pos[i]) / vel[i];
    //    }
    //}

    if(landed_on_ground)
    {
        // MS: Uncomment these two lines to get the recover-timer thing to work.

        //if(controls && vel[1] < -3)
        //    controls->recover_from_landing_timer = 90;

        #if CLIENT
            // MS: Put camera-shake back in on hard landing, cuz Matt Harvey liked it.

            if(!optionGet(kUO_DisableCameraShake) && vel[1] < -2)
            {
                game_state.camera_shake = min((-vel[1] - 2), 2) * 0.2;
                game_state.camera_shake_falloff = game_state.camera_shake * 0.05;
            }
        #endif

        #if SERVER
        {
            extern void doneFall(Entity *e, F32 fall_height, F32 yvel);
            doneFall(e, motion->highest_height - ENTPOSY(e), vel[1]);
        }
        #endif

        motion->highest_height = ENTPOSY(e);
    }

    #if SERVER
        // Q: WTF? why is this (doCritterFacing) here, and why does it exist at all?
        // A: MS: This is here to make critters that are sliding along walls or trying
        //    to run on the same line while running into each other not be facing a different
        //    direction than they are moving if the difference between those two directions
        //    is within some reasonable threshold.

        doCritterFacing(e, last_pos);
    #endif

    truncateVelocity(motion);

    copyVec3(ENTPOS(e), motion->last_pos);

    #if 0
        printf("in [%f %f %f]  out [%f %f %f]\n",
            motion->inp_vel[0],motion->inp_vel[1],motion->inp_vel[2],
            motion->vel[0],motion->vel[1],motion->vel[2]);
    #endif
}


#define NEAR_ZERO(f) ((f)<0.06 && (f)>-0.06)

int velIsNearlyZero(Vec3 vel)
{
    return (NEAR_ZERO(vel[0]) && NEAR_ZERO(vel[1]) && NEAR_ZERO(vel[2]));
}


void entMotion(Entity* e, const Mat3 control_mat)
{
    MotionState* motion = e->motion;
    int moving;
    EntType entType = ENTTYPE(e);
    int move_type = e->move_type;

    moving = motion->falling || !velIsZero(motion->vel) || !velIsZero(motion->input.vel);

    motion->tickCounter++; //how many times entMotion has been run.

    //WORLD's crappiest hack for a day so Al can work
    
    if( motion->tickCounter == 1 ) //always run physics when you are first born
    {
        if( e->seq && e->seq->type && !strnicmp(e->seq->type->name, "Rularuu_Summoned_Sentinel", 23 ) )
            moving = 1;
    }

    if(motion->low_traction_steps_remaining > 0)    
    {
        motion->low_traction_steps_remaining--;
    }

    //TL! entMotion Apply per frame traction loss and gravity gain
#define HIT_STUMBLE_RECOVERY_RATE 0.001
    //motion->hitStumbleTractionLoss = 0.0;    
    if(motion->hitStumbleTractionLoss > 0.0 )                 
    { 
        if( motion->hitStumbleTractionLoss < 0.9 )
            motion->hitStumbleTractionLoss -= 0.04;
        else
            motion->hitStumbleTractionLoss -= HIT_STUMBLE_RECOVERY_RATE; //TO DO base on strength of hit
        motion->hitStumbleTractionLoss = MAX( 0.0, motion->hitStumbleTractionLoss );
#ifdef CLIENT
        //xyprintf( 12, 12, "%f", motion->hitStumbleTractionLoss );
#endif
    }
    else
        motion->hitStumbleTractionLoss = 0.0; //no negatives, please

    //If you stumble, stop jumping, and when you hit the ground, lose velocity
    if( motion->hit_stumble )        
    {
        motion->hit_stumble_kill_velocity_on_impact = 1;
        motion->jumping = 0;
        motion->hit_stumble = 0; //TO DO does this slow me down
    }
    



#ifdef CLIENT  
    /*xyprintf( 12, 13, "motion->vel       %f %f %f", motion->vel[0], motion->vel[1], motion->vel[2] );
    xyprintf( 12, 14, "motion->input.vel %f %f %f", motion->input.vel[0], motion->input.vel[1], motion->input.vel[2] );
    {
        char * bar = "----$----1----$----2----$----3----$----4----$----5----$----6----$----7----$----8----$----9----$----0\n";
        char buf[300];
        F32 l=lengthVec3(motion->vel);
        int n = (int)(l*10.0);
        strncpy( buf, bar, n );
        buf[n] = '\0';
        xyprintf( 12, 16, "motion->vel %s %f", buf, l );
    }*/
#endif
    
    //END TL!
    


        
    // Don't move if this isn't a player and there's no velocity.
    
    if(!moving)
    {
        if (move_type & MOVETYPE_WALK)
        {
            int has_coll;

            if(entType != ENTTYPE_PLAYER )
                return;

            PERFINFO_AUTO_START("preCheckEntColl", 1);
                has_coll = checkEntColl(e, 0, control_mat);
            PERFINFO_AUTO_STOP();

            if(!has_coll)
                return;
        }
    }
#if SERVER
    // Interrupt powers if the entity is moving more than a tiny amount.

    if( e->pchar &&
        (    motion->falling && !motion->input.flying ||
            entType == ENTTYPE_PLAYER &&
            (    !velIsNearlyZero(motion->vel) ||
                !velIsNearlyZero(motion->input.vel))))
    {
        character_InterruptPower(e->pchar, NULL, 0.0f, false);
    }
#endif

    if (e->move_type & MOVETYPE_NOCOLL)
    {
        PERFINFO_AUTO_START("entMoveNoColl", 1);
            entMoveNoColl(e);
        PERFINFO_AUTO_STOP();
    }
    else if (e->move_type & MOVETYPE_WALK)
    {
        motion->move_time = 0;
#ifdef CLIENT        
        if(e == playerPtr() && baseedit_Mode())
            global_motion_state.noDetailCollisions = true;
#elif SERVER 
        if(e->pl && e->pl->architectMode)
            global_motion_state.noDetailCollisions = true;
#endif
        PERFINFO_AUTO_START("entWalk", 1);
            entWalk(e, control_mat);
        PERFINFO_AUTO_STOP();
        zeroVec3(motion->input.vel);

        global_motion_state.noDetailCollisions = false;
    }
    else
    {
        motion->falling = 0;//never hit
    }
}
