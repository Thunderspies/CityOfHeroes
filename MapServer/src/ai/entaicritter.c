
#include "entaiprivate.h"
#include "entaiCritterPrivate.h"
#include "entaiPriority.h"
#include "beacon/beaconPath.h"
#include "entity/entserver.h"
#include "entity/motion.h"
#include "generator/encounterPrivate.h"
#include "dbcomm/dbcomm.h"
#include "entaiPriorityPrivate.h"
#include "seq/seqstate.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "gamesys/dooranim.h"
#include "gridcoll/megaGrid.h"
#include "entity/powers.h"
#include "entity/character_base.h"
#include "entity/character_combat.h"
#include "beacon/beacon.h"
#include "cmdparse/cmdserver.h"
#include "ailib/aiBehaviorPublic.h"
#include "ai/entaiBehaviorCoh.h"
#include "ai/entaiLog.h"
#include "ai/entai.h"
#include "gridcoll/ctri.h"
#include <utilitieslib/utils/timing.h>
#include "gameComm/VillainDef.h"
#include <utilitieslib/utils/Quat.h>
#include "script/scriptengine.h"
#include "entity/character_target.h"
#include "entity/entworldcoll.h"

#define MAX_DIST_LOOK_FOR_ENCOUNTER        15.0


StaticDefineInt aiParseActivity[] = {
    DEFINE_INT
    { "None",            AI_ACTIVITY_NONE            },
    { "Combat",            AI_ACTIVITY_COMBAT            },
    { "DoAnim",            AI_ACTIVITY_DO_ANIM            },
    { "FollowLeader",    AI_ACTIVITY_FOLLOW_LEADER    },
    { "FollowRoute",    AI_ACTIVITY_FOLLOW_ROUTE    },
    { "FrozenInPlace",    AI_ACTIVITY_FROZEN_IN_PLACE    },
    { "HideBehindEnt",    AI_ACTIVITY_HIDE_BEHIND_ENT    },
    { "NPC.Hostage",    AI_ACTIVITY_NPC_HOSTAGE        },
    { "Patrol",            AI_ACTIVITY_PATROL            },
    { "RunAway",        AI_ACTIVITY_RUN_AWAY        },
    { "RunIntoDoor",    AI_ACTIVITY_RUN_INTO_DOOR    },
    { "RunOutOfDoor",    AI_ACTIVITY_RUN_OUT_OF_DOOR    },
    { "RunToTarget",    AI_ACTIVITY_RUN_TO_TARGET    },
    { "StandStill",        AI_ACTIVITY_STAND_STILL        },
    { "ThankHero",        AI_ACTIVITY_THANK_HERO        },
    { "Wander",            AI_ACTIVITY_WANDER            },
    DEFINE_END
};

//**************************************************************************************
// Critter AI.
//**************************************************************************************

U32 aiCritterGetActivityDataSize(){
    return 0;
}

void aiCritterActivityChanged(Entity* e, AIVars* ai, int oldActivity){
    switch(oldActivity){
        case AI_ACTIVITY_THANK_HERO:{
            AI_FREE_ACTIVITY_DATA(e, ai);
            break;
        }

        case AI_ACTIVITY_NPC_HOSTAGE:{
            AI_FREE_ACTIVITY_DATA(e, ai);
            break;
        }
    }
    
    ai->activityData = NULL;

    switch(aiBehaviorGetActivity(e)){
        case AI_ACTIVITY_THANK_HERO:
            AI_ALLOC_ACTIVITY_DATA(e, ai, AINPCActivityDataThankHero);
            break;

        case AI_ACTIVITY_COMBAT:
            e->move_type = MOVETYPE_WALK;
            break;
    }
}

void aiCritterEntityWasDestroyed(Entity* e, AIVars* ai, Entity* entDestroyed){
    if(aiBehaviorGetActivity(e) == AI_ACTIVITY_THANK_HERO)
    {
        AINPCActivityDataThankHero* data = ai->activityData;

        if(data->hero == entDestroyed)
            data->hero = NULL;

        if(data->door == entDestroyed)
            data->door = NULL;
    }
}

static void aiCritterUpdateProxEntStatus(Entity* e, AIVars* ai, Entity** entArray, int entCount){
    int i;
    F32 minTargetDistSQR = FLT_MAX;
    F32 minLiveEntDistSQR = FLT_MAX;
    int timeslice;
    Vec3 myPos;
    Character* pchar = e->pchar;
    F32 botherDistance = ai->overridePerceptionRadius ? ai->overridePerceptionRadius : 
            pchar->attrCur.fPerceptionRadius;
    F32 botherDistanceSQR = SQR(botherDistance);
    F32 proxFear = 0;
    int myLevel = pchar->bIgnoreCombatMods ? (MAX_PLAYER_SECURITY_LEVEL+4) : pchar->iCombatLevel;
    int considerEnemyLevel = ( ai->considerEnemyLevel && !server_state.disableConsiderEnemyLevel );
    F32 maxNoticeRange = max(100, botherDistance);
    F32 maxNoticeRangeSQR = SQR(maxNoticeRange);

    copyVec3(ENTPOS(e), myPos);

    ai->nearbyPlayerCount = 0;

    for(i = 0; i < entCount; i++){
        Entity* proxEnt = entArray[i];

        if(proxEnt != e){
            AIVars* aiProx = ENTAI(proxEnt);
            EntType entTypeProx = ENTTYPE(proxEnt);
            F32 distSQR = distance3Squared(myPos, ENTPOS(proxEnt));
            AIProxEntStatus* statusProx = NULL;

            // Add nearby targets to my status table.

            if(aiIsLegalAttackTarget(e, proxEnt)){
                if(distSQR < maxNoticeRangeSQR){
                    Character* pcharProx = proxEnt->pchar;

                    if(!statusProx){
                        statusProx = aiGetProxEntStatus(ai->proxEntStatusTable, proxEnt, 1);
                    }
                    
                    if(distSQR < minTargetDistSQR){
                        minTargetDistSQR = distSQR;
                    }

                    if(distSQR < minLiveEntDistSQR){
                        minLiveEntDistSQR = distSQR;
                    }

                    if(    ( considerEnemyLevel && !server_state.disableConsiderEnemyLevel )&&
                        pcharProx &&
                        pcharProx->attrCur.fHitPoints > 0.2 * pcharProx->attrMax.fHitPoints &&
                        pcharProx->attrCur.fHeld <= 0 &&
                        pcharProx->attrCur.fStunned <= 0 &&
                        pcharProx->attrCur.fSleep <= 0)
                    {
                        if(ABS_TIME_SINCE(statusProx->time.posKnown) < SEC_TO_ABS(10)){
                            int proxLevel = pchar->bIgnoreCombatMods ? (MAX_PLAYER_SECURITY_LEVEL+4) : pcharProx->iCombatLevel;

                            if(proxLevel > myLevel){
                                proxFear += 0.001 * (maxNoticeRange - sqrt(distSQR)) * (proxLevel - myLevel);
                            }
                        }
                    }
                }
            }else{
                // Not a target, so figure out if it's a friend.
                
                if(aiProx && aiProx->teamMemberInfo.team == ai->teamMemberInfo.team){
                    Character* pcharProx = proxEnt->pchar;

                    if(    ( considerEnemyLevel && !server_state.disableConsiderEnemyLevel ) &&
                        pcharProx &&
                        pcharProx->attrCur.fHitPoints > 0.2 * pcharProx->attrMax.fHitPoints &&
                        pcharProx->attrCur.fHeld <= 0 &&
                        pcharProx->attrCur.fStunned <= 0 &&
                        pcharProx->attrCur.fSleep <= 0)
                    {
                        if(distSQR < maxNoticeRangeSQR){
                            int proxLevel = pchar->bIgnoreCombatMods ? (MAX_PLAYER_SECURITY_LEVEL+4) : pcharProx->iCombatLevel;

                            if(proxLevel > myLevel){
                                proxFear -= 0.001 * (maxNoticeRange - sqrt(distSQR)) * (proxLevel - myLevel);
                            }else{
                                proxFear -= 0.001 * (maxNoticeRange - sqrt(distSQR));
                            }
                        }
                    }
                }

                if(distSQR < minLiveEntDistSQR){
                    minLiveEntDistSQR = distSQR;
                }
            }
            
            // Check if this entity can target me.
            
            if(aiProx && aiProx->avoid.list && aiIsLegalAttackTarget(proxEnt, e)){
                F32 avoidDistance = aiProx->avoid.maxRadius + ai->collisionRadius + aiProx->collisionRadius;
                
                if(distSQR <= SQR(avoidDistance)){
                    if(!statusProx){
                        statusProx = aiGetProxEntStatus(ai->proxEntStatusTable, proxEnt, 1);
                    }
                }
            }

            // Increment my nearby player count, and update nearest distance.

            if(entTypeProx == ENTTYPE_PLAYER){
                ai->nearbyPlayerCount++;
            }
        }
    }

    if(proxFear < 0){
        proxFear = 0;
    }

    aiFeelingSetValue(e, ai, AI_FEELINGSET_PROXIMITY, AI_FEELING_FEAR, proxFear);

    if(minTargetDistSQR <= botherDistanceSQR){
        float dist = sqrt(minTargetDistSQR);

        aiFeelingAddValue(e, ai, AI_FEELINGSET_PROXIMITY, AI_FEELING_BOTHERED, 0.5 / (1.0 + 4.0 * sqrt(dist / botherDistance)));
    }else{
        aiFeelingAddValue(e, ai, AI_FEELINGSET_PROXIMITY, AI_FEELING_BOTHERED, -0.05);
    }

    // Calculate new timeslice based on nearest distance to a player.

    if(ai->nearbyPlayerCount || ai->attackTarget.entity){
        if(ai->nearbyPlayerCount){
            minLiveEntDistSQR = sqrt(minLiveEntDistSQR);
        }else{
            minLiveEntDistSQR = distance3(myPos, ENTPOS(ai->attackTarget.entity));
        }

        if(minLiveEntDistSQR < 0){
            minLiveEntDistSQR = 0;
        }
        else if(minLiveEntDistSQR > 399){
            minLiveEntDistSQR = 399;
        }

        timeslice = 400 - minLiveEntDistSQR;
    }
    else{
        timeslice = 0;
    }

    // Scale timeslice down to 0-100 range.

    timeslice = (timeslice + 3) / 4;

    aiSetTimeslice(e, timeslice);
}

// Adde to AI CONFIG or enttype
//Vis Dis
//fov 
//DetectionType angle \ cone
//perception radius
void drawPerceptionCone( Mat4 mat, F32 length, F32 radius )
{
    Vec3 p  = { 0, 1, 0 };
    Vec3 p2;
    Vec3 px;
    Vec3 p2x;
    Vec3 last;
    int count;
    int i;

    count = max(radius, 12);  

    for( i = 0 ; i < count+1 ; i++ )  
    {
        p[0]    = 0;
        p[2]    = length;  
        p[1]    = 0;

        p2[0]    = radius * sin(RAD(360) * i / count + RAD(global_state.global_frame_count * 1));
        p2[2]    = length;  
        p2[1]    = radius * cos(RAD(360) * i / count + RAD(global_state.global_frame_count * 1));

        mulVecMat4( p,  mat, px  );
        mulVecMat4( p2, mat, p2x );

        aiEntSendAddLine(px, 0xffffffff, p2x, 0xffffffff);
        aiEntSendAddLine(mat[3], 0xffffffff, p2x, 0xffffffff);

        if( i > 0 )
            aiEntSendAddLine( p2x, 0xffffffff,last, 0xffffffff );
        copyVec3( p2x, last );
    }

    aiEntSendAddLine( p2x, 0xffffffff,last, 0xffffffff );

}



//coneMat was also calculated in targetTime outside this func, TO DO fix
int aiEntConeCollision( Entity * target, const Mat4 coneMat, F32 coneLength, F32 coneRadius )
{
    Vec3 conePt; //point on the cone's line that is closest to the capsule line
    EntityCapsule tCap;
    Vec3 coneDir;
    Vec3 entDir;
    F32 mag;
    Mat4 collMat;
    Mat4 targetMatAtTargetTime;
    Vec3 t;
    int result = 1; 

    {
        Quat qrot;
        entGetPosAtEntTime( target, target, targetMatAtTargetTime[3], qrot); 
        quatToMat(qrot, targetMatAtTargetTime); 
    }

    tCap = target->motion->capsule;        
    positionCapsule( targetMatAtTargetTime, &tCap, collMat );  

    //Get cone and ent dir
    t[0] = 0;
    t[1] = 1;
    t[2] = 0;
    mulVecMat3( t, collMat, entDir );

    t[0] = 0;
    t[1] = 0;
    t[2] = 1;
    mulVecMat3( t, coneMat, coneDir );

    /*{
        Vec3 x, y;
        scaleVec3( entDir, tCap.length, x );
        addVec3( collMat[3], x, y );  
        aiEntSendAddLine(collMat[3], 0xffffffff, y, 0xff00ffff);

        scaleVec3( coneDir, coneLength, x );
        addVec3( coneMat[3], x, y );  
        aiEntSendAddLine(coneMat[3], 0xffffffff, y, 0xff00ffff);
    }//*/

    //Find conePt and mag. dist between capsule line and cone line
    mag = coh_LineLineDist(    coneMat[3],    coneDir,    coneLength,        conePt,
                            collMat[3],    entDir,        tCap.length,    t );

    //If the capsule is past the end of the cone, you're done
    //if( nearSameVec3( conePt, endOfCone ) && mag > tCap.radius )
    //    return 0;

    //Get the radius of the cone at the intersection point and look for intersection
    {
        F32 percent;
        F32 coneRadAtIntersect; 
        F32 dist;

        dist = distance3( conePt, coneMat[3] );

        percent = dist / coneLength;     
        coneRadAtIntersect = percent * coneRadius;

        //printf("coneRad %f + tCap.radius %f VS mag %f ", coneRadAtIntersect, tCap.radius, sqrt(mag) );

        if( mag > SQR(tCap.radius + coneRadAtIntersect) )
        {
            result = 0;
        }
    }

    /*
    if(0){
        Vec3 linePt;  
        LineLineDist(    collMat[3], entDir, tCap.length, linePt, 
                        coneMat[3], coneDir, coneLength, NULL );

        aiEntSendAddLine(conePt, 0xffff0000, linePt, 0xff0000ff);

        Vec3 yy;
        copyVec3( conePt, yy); yy[1]+=2.0; yy[0]+=1.0;
        aiEntSendAddLine(conePt, 0xffff0000, yy, 0xff0000ff);
        copyVec3( conePt, yy); yy[1]+=2.0; yy[0]-=1.0;
        aiEntSendAddLine(conePt, 0xffff0000, yy, 0xff0000ff);
        copyVec3( conePt, yy); yy[1]+=2.0; yy[2]+=1.0;
        aiEntSendAddLine(conePt, 0xffff0000, yy, 0xff0000ff);
        copyVec3( conePt, yy); yy[1]+=2.0; yy[2]-=1.0;
        aiEntSendAddLine(conePt, 0xffff0000, yy, 0xff0000ff);

        if( result )
        {
            aiEntSendAddLine(conePt, 0xffff0000, linePt, 0xff0000ff);
            printf(" HIT!!!!!!!! \n" );
        }
        else
        {
            printf(" MISS \n" );
            aiEntSendAddLine(conePt, 0xff000000, linePt, 0xff000000);
        }
    }

    aiEntSendAddLine(coneMat[3], 0xff00ff00, collMat[3], 0xff00ff00);
    //*/
    
    return result; 

}

typedef enum EntityAggroLevel {
    AGGRO_LEVEL_NONE,
    AGGRO_LEVEL_DMG,
    AGGRO_LEVEL_LIEUTENANT,
    AGGRO_LEVEL_BOSS,
    AGGRO_LEVEL_TAUNT,
    AGGRO_LEVEL_MAX,
} EntityAggroLevel;

static EntityAggroLevel getEntityAggro(Entity* e, AIProxEntStatus* status)
{
    if(status && !status->damage.toMe)
        return AGGRO_LEVEL_NONE;
    else if(status && status->taunt.duration)
        return AGGRO_LEVEL_TAUNT;
    else if(e && e->villainDef && e->villainDef->rank >= VR_BOSS)
        return AGGRO_LEVEL_BOSS;
    else if(e && e->villainDef && e->villainDef->rank >= VR_LIEUTENANT)
        return AGGRO_LEVEL_LIEUTENANT;

    return AGGRO_LEVEL_DMG;
}

typedef struct EntityToDistance {
    Entity*    entity;
    float    distanceSQR;
} EntityToDistance;

static int compareEntityDistances(const EntityToDistance* e1, const EntityToDistance* e2){
    if(e1->distanceSQR < e2->distanceSQR)
        return -1;
    else if(e1->distanceSQR == e2->distanceSQR)
        return 0;
    else
        return 1;
}

static void aiCritterFindTarget(Entity* e,
                                AIVars* ai,
                                Entity** entArray,
                                int entCount,
                                EntityToDistance* critterArray,
                                int critterCount,
                                int findTarget,
                                int findTeam)
{
    Entity* newTarget = NULL;
    Character* pchar = e->pchar;
    float unknownVisDistance = ai->overridePerceptionRadius ? ai->overridePerceptionRadius : pchar->attrCur.fPerceptionRadius;
    AIProxEntStatus* status;
    AIProxEntStatus* newTargetStatus = NULL;
    float maxTotal = -FLT_MAX;
    int alertHelpers = 0;
    int doNotChangeAttackTarget = ai->doNotChangeAttackTarget || ai->relentless;
    Entity* attackTarget = ai->attackTarget.entity;
    Entity* discardEntity = NULL;
    int myLevel = pchar->bIgnoreCombatMods ? (MAX_PLAYER_SECURITY_LEVEL+4) : pchar->iCombatLevel;

    // If I'm on a wire path, don't attempt to join other AI teams
    if (ai->motion.type == AI_MOTIONTYPE_WIRE)
        findTeam = false;

    // Go through status table, calculate danger factors, find a target.

    if(findTarget){
        for(status = ai->proxEntStatusTable->statusHead;
            status;
            status = status ? status->tableList.next : ai->proxEntStatusTable->statusHead)
        {
            Entity* target;
            AIVars* aiTarget;
            AIProxEntStatus* attackTargetStatus;
            U32 timeSinceSeen;
            U32 lastAttack;
            F32 distFromMe;
            F32 tauntFactor;
            F32 distanceFactor;
            F32 damageFactor;
            F32 lastAttackFactor;
            F32 teamOrdersFactor;
            F32 spawnDistanceFactor;
            F32 validTargetFactor;
            F32 sleepingFactor;
            F32 primaryTargetFactor;
            F32 untargetedFactor;
            F32 stickyFactor;
            F32 preferenceFactor;
            F32 total;
            S32 neverForgetMe;
            S32 isLegalAttackTarget;


            target = status->entity;

            if(!target || !target->pchar)
                continue;

            if(target->notAttackedByJustAnyone && !ai->attackTargetsNotAttackedByJustAnyone )
                continue;

            if(target->pchar->attrCur.fHitPoints <= 0){
                status->time.posKnown = 0;
                continue;
            }

            // IF: I'm not supposed to change target,
            //   AND: I have a current target
            //   AND: this isn't my current target,
            //   AND: he's not taunting me OR I'm relentless
            //   THEN: ignore him.

            if(    doNotChangeAttackTarget &&
                attackTarget &&
                target != attackTarget &&
                (ABS_TIME_SINCE(status->taunt.begin) > status->taunt.duration || ai->relentless))
            {
                continue;
            }

            aiTarget = ENTAI(target);

            neverForgetMe = status->neverForgetMe;

            if(neverForgetMe){
                status->time.posKnown = ABS_TIME;
            }

            timeSinceSeen = ABS_TIME_SINCE(status->time.posKnown);

            distFromMe = distance3(ENTPOS(e), ENTPOS(target));

            isLegalAttackTarget = aiIsLegalAttackTarget(e, target);
            
            // Check if this target should be avoided.
            
            if(aiTarget->avoid.list){
                int avoidTarget = aiShouldAvoidEntity(e, ai, target, aiTarget);
                
                if(avoidTarget != status->inAvoidList){
                    if(avoidTarget){
                        aiProximityAddToAvoidList(status);
                    }else{
                        aiProximityRemoveFromAvoidList(status);
                    }
                }
            }
            
            // IF: I have seen you in the last X seconds,
            //   THEN: I still know where you are.

            if(    timeSinceSeen > ai->eventMemoryDuration
                &&
                (
                    // You're not on my team.
                    !ai->teamMemberInfo.team
                    ||
                    ai->teamMemberInfo.team != aiTarget->teamMemberInfo.team
                )
                &&
                (
                    // I'm afraid or you're not my team target.
                    ai->isAfraidFromAI
                    ||
                    ai->isAfraidFromPowers
                    ||
                    !ai->teamMemberInfo.orders.targetStatus
                    ||
                    ai->teamMemberInfo.orders.targetStatus->entity != target
                ))
            {
                // IF: I have NOT seen you for X seconds,
                //   THEN: I'll check if I can see you now.

                if(isLegalAttackTarget){
                    int inViewRange = 0; 
                    AIProxEntStatus* teamStatus;

                    if(ai->teamMemberInfo.team && ai->teamMemberInfo.alerted){
                        teamStatus = aiGetProxEntStatus(ai->teamMemberInfo.team->proxEntStatusTable, target, 1);
                    }else{
                        teamStatus = NULL;
                    }

                    if(!inViewRange)
                    {
                        //MW added isCamera 
                        if( ai->isCamera )
                        {
                            Mat4 mat;
                            Quat qrot;
                            
                            //TO DO : use FOV and vis dist
                            //ai->fov.angle
                            //ai->fov.angleCosine
                            
                            entGetPosAtEntTime( e, target, mat[3], qrot); 
                            quatToMat(qrot, mat); 
  
                            //drawPerceptionCone( mat, 45, 5 );  

                            if( aiEntConeCollision( target, mat, 45, 5.0 ) ) //A bit strange to be using entTime inside and outside func
                            {
                                //if( server_state.netfxdebug
                                //    printf( "Stealth Camera I see you, %s", target->name );
                                inViewRange = 1;
                            } 
                        }
                        else
                        {
                            inViewRange = distFromMe < 10;
                            if( !inViewRange )
                            {

                                if(    teamStatus && ABS_TIME_SINCE(teamStatus->time.posKnown) < SEC_TO_ABS(3)
                                    ||
                                    aiPointInViewCone(ENTMAT(e), ENTPOS(target), ai->fov.angleCosine))
                                {
                                    inViewRange = 1;
                                }
                            }
                        }
                    }

                    if(inViewRange){
                        F32 sightDistance = unknownVisDistance;

                        if(teamStatus){
                            U32 timeSince = ABS_TIME_SINCE(teamStatus->time.posKnown);
                            U32 eventMemoryDuration = ai->eventMemoryDuration;

                            if(timeSince < eventMemoryDuration)
                            {
                                F32 scale = (F32)(eventMemoryDuration - timeSince) / eventMemoryDuration;

                                sightDistance += scale * ai->targetedSightAddRange;
                            }
                        }

                        if(!ai->ignoreTargetStealthRadius){
                            sightDistance -= target->pchar->attrCur.fStealthRadius;
                        }

                        if(    sightDistance > 0
                            &&
                            aiEntityCanSeeEntity(e, target, sightDistance))
                        {
                            status->time.posKnown = ABS_TIME;
                        }
                    }
                }

                if(ABS_TIME_SINCE(status->time.posKnown) > ai->eventMemoryDuration){
                    status->dangerValue = 0;
                    continue;
                }
            }

            if(!isLegalAttackTarget){
                status->dangerValue = 0;
                continue;
            }

            if(    !neverForgetMe
                &&
                (ai->considerEnemyLevel && !server_state.disableConsiderEnemyLevel )
                &&
                ABS_TIME_SINCE(status->time.attackMe) > SEC_TO_ABS(60)
                &&
                ABS_TIME_SINCE(status->time.attackMyFriend) > SEC_TO_ABS(60)
                &&
                (myLevel < ((target->pchar->bIgnoreCombatMods ? (MAX_PLAYER_SECURITY_LEVEL+4) : target->pchar->iCombatLevel ) - 3 ) ) )
            {
                status->dangerValue = 0;
                continue;
            }

            // TAUNT factor. ***************************************

            //   IF: target taunted me,
            //   AND: he's got more duration than twice my current target's remaining taunt,
            //   THEN: I'll switch to you, otherwise I'm scrapping your taunt.

            attackTargetStatus = ai->attackTarget.status;

            if(    ABS_TIME_SINCE(status->taunt.begin) < status->taunt.duration &&
                (    !attackTargetStatus ||
                    attackTargetStatus == status ||
                    !attackTargetStatus->taunt.duration ||
                    status->taunt.duration >
                        2 * (ai->attackTarget.status->taunt.duration - ABS_TIME_SINCE(ai->attackTarget.status->taunt.begin))
                ))
            {
                tauntFactor = 10 * (1 + status->taunt.duration - ABS_TIME_SINCE(status->taunt.begin));
            }else{
                status->taunt.duration = 0;
                tauntFactor = 1;
            }

            // PLACATE factor. ***************************************

            //   IF: target placated me,
            //   AND: he's not taunting me,
            //   AND: he hasn't attacked me since placating me,
            //   THEN: there's no way I will target you.
            //   OTHERWISE: I'll scrap your placate

            if(    ABS_TIME_SINCE(status->placate.begin) < status->placate.duration &&
                !status->taunt.duration &&
                status->time.attackMe < status->placate.begin)
            {
                status->dangerValue = 0;
                continue;
            }
            else if(status->placate.duration && !status->taunt.duration) // placated critters forget about their targets completely (unless they got taunted)
            {
                status = status->tableList.prev;
                aiRemoveProxEntStatus(ai->proxEntStatusTable, target, 0, 1);
                continue;
            }
            else
                status->placate.duration = 0;

            // DISTANCE factor. ***************************************

            distanceFactor = 0.1 + 0.9 * (100.0 - distFromMe) / 100.0;

            if(distanceFactor < 0.1)
                distanceFactor = 0.1;

            // DAMAGE factor. ***************************************

            if(neverForgetMe)
                damageFactor = 1 + pchar->attrMax.fHitPoints;
            else
                damageFactor = 1 + status->damage.toMe;// + 0.1 * status->damage.toMyFriends;

            // ATTACK TIME factor. ***************************************

            if(ai->brain.type == AI_BRAIN_CLOCKWORK){
                // Clockwork guys exponentially favor the most recent attack time.

                lastAttack = ABS_TIME_SINCE(status->time.attackMe);

                if(ABS_TIME_SINCE(status->time.attackMyFriend) < lastAttack)
                    lastAttack = ABS_TIME_SINCE(status->time.attackMyFriend);

                if(lastAttack < SEC_TO_ABS(20)){
                    F32 ratio = (float)(SEC_TO_ABS(20) - lastAttack) / SEC_TO_ABS(20);

                    if(ratio < 0)
                        ratio = 0;

                    lastAttackFactor = powf(100 * ratio, 5);
                }else{
                    lastAttackFactor = 0.001;
                }
            }else{
                if(neverForgetMe)
                    lastAttack = 0;
                else
                    lastAttack = ABS_TIME_SINCE(status->time.attackMe);

                if(status->damage.toMe > 0 && lastAttack < SEC_TO_ABS(20)){
                    F32 ratio = (float)(SEC_TO_ABS(20) - lastAttack) / SEC_TO_ABS(20);

                    lastAttackFactor = 0.7 + 0.3 * ratio;
                }else{
                    lastAttackFactor = 0.7;
                }
            }

            // TEAM ORDERS factor. ***************************************

            if(    ai->teamMemberInfo.orders.role == AI_TEAM_ROLE_ATTACKER_MELEE
                ||
                ai->teamMemberInfo.orders.role == AI_TEAM_ROLE_ATTACKER_RANGED)
            {
                Entity* teamTarget = ai->teamMemberInfo.orders.targetStatus->entity;

                if(teamTarget == target){
                    if(ai->teamMemberInfo.orders.role == AI_TEAM_ROLE_ATTACKER_MELEE)
                        teamOrdersFactor = 2;
                    else
                        teamOrdersFactor = 1.5;
                }else{
                    if(    ABS_TIME_SINCE(status->time.attackMe) > ai->eventMemoryDuration
                        &&
                        ABS_TIME_SINCE(status->time.attackMyFriend) > ai->eventMemoryDuration)
                    {
                        teamOrdersFactor = 0.01;
                    }else{
                        teamOrdersFactor = 1;
                    }
                }
            }else{
                teamOrdersFactor = 1;
            }

            // SPAWN DISTANCE factor. ***************************************

            if(    !neverForgetMe
                &&
                ABS_TIME_SINCE(status->time.attackMe) >= ai->protectSpawnDuration
                &&
                ABS_TIME_SINCE(status->time.attackMyFriend) >= ai->protectSpawnDuration
                &&
                !ai->relentless
                //&&
                //(    !ai->teamMemberInfo.orders.targetStatus ||
                //    ai->teamMemberInfo.orders.targetStatus->entity != status->entity) &&
                )
            {
                // This guy hasn't attacked me for a while, so he needs to be in my spawn area.

                const F32* spawnPos;
                F32 targetDistFromSpawn;
                F32 remainingDist;
                Entity* creator = e->erCreator ? erGetEnt(e->erCreator) : NULL;
                int useDistanceToSpawnPos = 0;
                F32 distToCompare;

                if(ai->pet.useStayPos){
                    spawnPos = ai->pet.stayPos;
                }
                else if(ai->spawnPosIsSelf){
                    spawnPos = ENTPOS(e);
                }
                else if(creator){
                    spawnPos = ENTPOS(creator);
                    //useDistanceToSpawnPos = 1;
                }
                else if(ai->teamMemberInfo.team){
                    spawnPos = ai->teamMemberInfo.team->spawnCenter;
                }
                else{
                    spawnPos = ai->spawn.pos;
                }

                targetDistFromSpawn = distance3(ENTPOS(target), spawnPos);
                remainingDist = 2 * ai->protectSpawnRadius - targetDistFromSpawn;
                
                distToCompare = useDistanceToSpawnPos ? targetDistFromSpawn : distFromMe;

                if(remainingDist < ai->protectSpawnOutsideRadius){
                    remainingDist = ai->protectSpawnOutsideRadius;
                }

                if(distToCompare > remainingDist){
                    spawnDistanceFactor = 0;

                    status->time.posKnown = 0;
                }else{
                    spawnDistanceFactor = 1.0 - distToCompare / remainingDist;
                }

                //spawnDistanceFactor = 1.0 - dist / ai->protectSpawnRadius;

                // Clamp the top 1/3 to full value.

                if(spawnDistanceFactor > 0.66)
                    spawnDistanceFactor = 0.66;
                //else if(spawnDistanceFactor <= 0){
                //    spawnDistanceFactor = 0;
                //
                //    status->time.posKnown = 0;
                //}
            }else{
                // This has attacked me or a teammate recently, so he gets full spawn distance factor.

                spawnDistanceFactor = 1;
            }

            // VALID TARGET factor. ***************************************

            if(ABS_TIME_SINCE(status->invalid.begin) >= status->invalid.duration){
                validTargetFactor = 1;
            }else{
                validTargetFactor = 0.001;
            }

            // SLEEPING factor. ***************************************

            if(target->pchar->attrCur.fSleep > 0){
                sleepingFactor = 0.01;
            }else{
                sleepingFactor = 1;
            }

            // PRIMARY TARGET factor. ***************************************

            if (status->primaryTarget) {
                if(aiBehaviorCurMatchesName(e, ai->base, "AttackTarget"))
                    primaryTargetFactor = 1;
                else
                    primaryTargetFactor = 1000;
            }
            else {

                if(aiBehaviorCurMatchesName(e, ai->base, "AttackTarget"))
                {
                    // Desired Behavior: agent can be interrupted if someone attacks them enough. Total damage and time since last attack are
                    // factored into the value.
                    // Weakness: If a team gangs up on a single agent, the agent will continue to attack primary target
                    primaryTargetFactor = (1 / e->pchar->attrMax.fHitPoints);    // proportional to hit points to offset damage factor

                       primaryTargetFactor *= 20;

                    // scale based on time
                    if(ABS_TO_SEC(ABS_TIME_SINCE(status->time.attackMe)) < 20)
                        primaryTargetFactor *= (30.0f - ABS_TO_SEC(ABS_TIME_SINCE(status->time.attackMe))) / 100.0f;
                    else
                        primaryTargetFactor *= 0.0001;
                }
                else
                    primaryTargetFactor = 1;
            }

            // UNTARGETED factor. ***************************************

            if(    ai->preferUntargeted &&
                (    aiTarget->attackerList.count == 0 ||
                    ai->attackTarget.entity == target && aiTarget->attackerList.count == 1))
            {
                untargetedFactor = 10;
            }else{
                untargetedFactor = 1;
            }

            // STICKY factor. *********************************

            // this is meant to stop a lot of flipflopping that critters on a team by themselves 
            // have a tendency to do (especially single mastermind pets)
            // it is mostly relative to the teamOrdersFactor
            if(ai->stickyTarget && status->entity == ai->attackTarget.entity)
                stickyFactor = 2.5;
            else
                stickyFactor = 1;

            // PREFERRED TARGET factor. *********************************

            if(status->targetPrefFactor)
                preferenceFactor = status->targetPrefFactor;
            else
                preferenceFactor = 1;

            // TOTAL. ***************************************

            total = tauntFactor *
                    damageFactor *
                    distanceFactor *
                    lastAttackFactor *
                    teamOrdersFactor *
                    spawnDistanceFactor *
                    validTargetFactor *
                    sleepingFactor *
                    primaryTargetFactor *
                    untargetedFactor *
                    stickyFactor *
                    preferenceFactor;

            if(character_IsConfused(e->pchar)){
                total *= rand() % 100;
            }

            status->dangerValue = total;

            if(total > maxTotal){
                // AGGRO CAP processing. *********************************
                if(status->entity != attackTarget)    // if you're already targeting this guy it's ok
                                                    // even if he's capping out
                {
                    if ((int)aiTarget->attackerList.count >= server_state.aggrocap)
                    {
                        AIAttackerInfo* attackerInfo;
                        Entity* leastAggro = NULL;
                        AIProxEntStatus* leastAggroStatus = NULL;
                        EntityAggroLevel lowestAggroLevel = AGGRO_LEVEL_MAX, entityAggroLevel, myAggroLevel;

                        myAggroLevel = getEntityAggro(e, status);

                        for(attackerInfo = aiTarget->attackerList.head; attackerInfo; attackerInfo = attackerInfo->next)
                        {
                            Entity* aggroListEnt = attackerInfo->attacker;
                            AIProxEntStatus* aggroListStatus =
                                aiGetProxEntStatus(ENTAI(aggroListEnt)->proxEntStatusTable, target, 0);

                            if(!aggroListStatus)
                                continue; // RvP: check if this is a cleanup problem or a valid case when I have time.

                            entityAggroLevel = getEntityAggro(aggroListEnt, aggroListStatus);

                            if((entityAggroLevel == lowestAggroLevel &&
                                (aggroListEnt->pchar->iLevel < leastAggro->pchar->iLevel ||
                                    (aggroListEnt->pchar->iLevel == leastAggro->pchar->iLevel &&
                                    aggroListStatus->damage.toMe < leastAggroStatus->damage.toMe))) ||
                                    entityAggroLevel < lowestAggroLevel)
                            {
                                lowestAggroLevel = entityAggroLevel;
                                leastAggro = aggroListEnt;
                                leastAggroStatus = aggroListStatus;
                            }
                        }

                        if(myAggroLevel > lowestAggroLevel ||
                            (    myAggroLevel == lowestAggroLevel &&
                                (e->pchar->iLevel > leastAggro->pchar->iLevel ||
                                (e->pchar->iLevel == leastAggro->pchar->iLevel &&
                                status->damage.toMe > leastAggroStatus->damage.toMe))))
                        {
                            discardEntity = leastAggro;
                        }
                        else
                        {
                            status->dangerValue = 0;
                            ZeroStruct(&status->damage);
                            ZeroStruct(&status->time);
                            ZeroStruct(&status->taunt);
                            continue;
                        }
                    }
                    else
                        discardEntity = NULL;
                }
                else
                    discardEntity = NULL;

                newTargetStatus = status;
                maxTotal = total;
            }

              AI_LOG(    AI_LOG_COMBAT,
                      (e,
                      "Agent %s gives target score of %f to %s (ptf: %f)\n",
                      AI_LOG_ENT_NAME(e),
                      total,
                      AI_LOG_ENT_NAME(status->entity),
                      primaryTargetFactor));
        }

        if(newTargetStatus && maxTotal > 0){
            newTarget = newTargetStatus->entity;
            if(discardEntity)
                aiDiscardAttackTarget(discardEntity, "Got booted because of aggro cap.");
        }

        // Nobody new, so discard my current target.

        if(!newTarget){
            aiDiscardAttackTarget(e, "No valid targets around.");
        }else{
            Entity* oldTarget = ai->attackTarget.entity;
            AIVars* aiTarget = ENTAI(newTarget);

            if(newTarget != oldTarget){
                aiSetAttackTarget(e, newTarget, newTargetStatus, NULL, 0, 0);

                if(!ai->teamMemberInfo.alerted){
                    EncounterAISignal(e, ENCOUNTER_ALERTED);

                    ai->teamMemberInfo.alerted = 1;
                }

                ai->time.alertedNearbyHelpers = 0; // ABS_TIME_SINCE(0) now returns UINT_MAX
            }

            if(    ai->teamMemberInfo.team &&
                ai->teamMemberInfo.team == aiTarget->teamMemberInfo.team
                ||
                ai->teamMemberInfo.orders.targetStatus &&
                ai->teamMemberInfo.orders.targetStatus->entity == newTarget)
            {
                ai->magicallyKnowTargetPos = 1;

                if(!ai->canSeeTarget){
                    ai->time.beginCanSeeState = ABS_TIME;
                }
            }

            // Alert other critters to the presence of my target every three seconds.

            if(ABS_TIME_SINCE(ai->time.alertedNearbyHelpers) > SEC_TO_ABS(3)){
                ai->time.alertedNearbyHelpers = ABS_TIME;
                alertHelpers = 1;
            }
        }
    }

    // Look for a team every five seconds if I don't have one, or I'm alone.
    //    if I have a team and I am on patrol, don't join other teams (i'm on patrol after all)
    if(    findTeam &&
        (!(aiBehaviorGetTopBehaviorByName(e, ai->base, "Patrol") && ai->teamMemberInfo.team)) &&
        (!ai->time.lookedForTeam || ABS_TIME_SINCE(ai->time.lookedForTeam) > SEC_TO_ABS(5)) &&
        (    !ai->teamMemberInfo.team ||
            ai->teamMemberInfo.team->members.totalCount == 1))
    {
        ai->time.lookedForTeam = ABS_TIME;
        findTeam = 1;
    }else{
        findTeam = 0;
    }

    if(alertHelpers || findTeam){
        int i;
        Entity* closestCritter = NULL;
        AIVars* closestAI = NULL;
        int foundTeam = 0;
        float closestDistSQR = SQR(50);
        float shoutRadiusSQR = SQR(ai->shoutRadius);
        int shoutChance = ai->shoutChance;
        int inEncounter = e->encounterInfo != NULL;
        float hpRatio = e->pchar->attrCur.fHitPoints / e->pchar->attrMax.fHitPoints;

        // Sort the critter list by distance.

        qsort(critterArray, critterCount, sizeof(critterArray[0]), compareEntityDistances);

        //if(shoutChance < 100 && hpRatio > 0.75){
        //    shoutChance *= 0.5 + 0.5 * (1.0 - hpRatio) / 0.25;
        //}

        for(i = 0; i < critterCount; i++){
            Entity* proxCritter;
            float distSQR;
            AIVars* aiProx;
            int proxInEncounter;

            proxCritter = critterArray[i].entity;

            distSQR = critterArray[i].distanceSQR;

            aiProx = ENTAI(proxCritter);

            proxInEncounter = proxCritter->encounterInfo != NULL;

            if(findTeam){
                if(proxCritter->pchar->attrCur.fHitPoints && aiIsMyFriend(e, proxCritter)){
                    int useNew = 0;

                    if(    erGetEnt(e->erCreator) == proxCritter ||
                        erGetEnt(e->erOwner) == proxCritter)
                    {
                        foundTeam = 1;
                        closestDistSQR = distSQR;
                        closestCritter = proxCritter;
                        closestAI = aiProx;
                        break;
                    }


                    if(    inEncounter != proxInEncounter ||
                        (    inEncounter &&
                            e->encounterInfo->parentGroup != proxCritter->encounterInfo->parentGroup))
                    {
                        continue;
                    }

                    if(e->erOwner && e->erOwner != proxCritter->erOwner)
                        continue;

                    if(foundTeam){
                        if(    distSQR < closestDistSQR &&
                            aiProx->teamMemberInfo.team)
                        {
                            useNew = 1;
                        }
                    }else{
                        if(    distSQR < closestDistSQR ||
                            aiProx->teamMemberInfo.team && distSQR < SQR(50))
                        {
                            useNew = 1;
                        }
                    }

                    if(useNew){
                        closestDistSQR = distSQR;
                        closestCritter = proxCritter;
                        closestAI = aiProx;

                        if(closestAI->teamMemberInfo.team)
                            foundTeam = 1;
                    }
                }
            }

            if(alertHelpers)
            {
                if(    aiProx &&
                    !aiProx->attackTarget.entity &&
                    aiIsLegalAttackTarget(proxCritter, newTarget) &&
                    ai->teamMemberInfo.team == aiProx->teamMemberInfo.team &&
                    distSQR < shoutRadiusSQR)
                {
                    int roll = rand() % 100;

                    if(roll < shoutChance)
                    {
                        AI_LOG(    AI_LOG_ENCOUNTER,
                                (e,
                                "^2ALERTED ^0(^4%d^0/^4%d^0) nearby critter ^4%d^0/^4%d^0(%s^0(^4%d^0)) ^0to presence of %s\n",
                                roll,
                                shoutChance,
                                i + 1,
                                critterCount,
                                AI_LOG_ENT_NAME(proxCritter),
                                proxCritter->owner,
                                AI_LOG_ENT_NAME(newTarget)));

                        AI_LOG(    AI_LOG_ENCOUNTER,
                                (proxCritter,
                                "I was alerted by %s ^0to presence of %s\n",
                                AI_LOG_ENT_NAME(e),
                                AI_LOG_ENT_NAME(newTarget)));

                        EncounterAISignal(proxCritter, ENCOUNTER_ALERTED);

                        aiProx->teamMemberInfo.alerted = 1;

                        aiFeelingSetValue(    proxCritter, aiProx,
                                            AI_FEELINGSET_PERMANENT,
                                            AI_FEELING_BOTHERED,
                                            1);
                    }else{
                        AI_LOG(    AI_LOG_ENCOUNTER,
                                (e,
                                "^1DID NOT ^0alert(^4%d^0/^4%d^0) nearby critter ^4%d^0/^4%d^0(%s^0) ^0about %s^0, stopping alerts.\n",
                                roll,
                                shoutChance,
                                i + 1,
                                critterCount,
                                AI_LOG_ENT_NAME(proxCritter),
                                AI_LOG_ENT_NAME(newTarget)));

                        // Stop after the first guy we don't notify.

                        alertHelpers = 0;
                    }
                }
            }
        }

        if(findTeam){
            AIVars* aiFriend;

            if(closestCritter){
                // Team up with that guy.

                aiFriend = ENTAI(closestCritter);
            }else{
                // Team up with myself.

                aiFriend = ai;
            }

            aiTeamJoin(&aiFriend->teamMemberInfo, &ai->teamMemberInfo);
        }
    }

    if(!ai->attackTarget.entity)
    {
        // Discard current power if I have one or if I'm in a combat stance and it's been 5 seconds since I had a target
        if (!ai->doNotChangePowers 
            && (ai->power.current 
                || (pchar->ppowStance && ABS_TIME_SINCE(ai->time.lastHadTarget) > SEC_TO_ABS(5))))
        {
            aiDiscardCurrentPower(e);
        }
    }
    else
        ai->time.lastHadTarget = ABS_TIME;
}

void aiCritterCheckProximity(Entity* e, int findTarget){
    AIVars* ai = ENTAI(e);
    MotionState* motion = e->motion;
    Entity* entArray[MAX_ENTITIES_PRIVATE];
    int entCount;
    EntityToDistance critterArray[ARRAY_SIZE(entArray)];
    int critterCount = 0;
    int i, j;
    float velLength = lengthVec3(motion->vel);
    Vec3 unitVel;
    int onTeamEvil = e->pchar->iAllyID==kAllyID_Monster || e->pchar->iAllyID==kAllyID_Villain;
    int scareCivilians = !ENTHIDE(e) && onTeamEvil;
    float maxThinkDist = SQR(250);

    if (ai->brain.type == AI_BRAIN_ARACHNOS_MAKO)
    {
        maxThinkDist = ai->brain.makoPatron.perceptionDistance;
    }

    // Increase consider area if perception is overriden
//    if (ai->overridePerceptionRadius > 0)
//        maxThinkDist = SQR(ai->overridePerceptionRadius);

    if(velLength){
        copyVec3(motion->vel, unitVel);
        normalVec3(unitVel);
    }

    // Get the list of nearby entities.

    j = mgGetNodesInRange(0, ENTPOS(e), (void **)entArray, 0); 

    // Do appropriate things with each entity in range.

    for(i = 0, entCount = 0; i < j; i++){
        int proxID = (int)(intptr_t)entArray[i];
        Entity* proxEnt = entities[proxID];
        AIVars* aiProx;
        float distSQR = distance3Squared(ENTPOS_BY_ID(proxID), ENTPOS(e));
        EntType proxEntType;

        if(proxEnt == e || distSQR > maxThinkDist){
            continue;
        }

        aiProx = ENTAI_BY_ID(proxID);
        proxEntType = ENTTYPE_BY_ID(proxID);

        entArray[entCount++] = proxEnt;

        // Deal with NPCs.

        if(    proxEntType == ENTTYPE_NPC &&
            !ENTHIDE_BY_ID(proxID) &&
            aiProx)
        {
            if (AreAllNPCsScared())
            {
                aiProx->wasScared = 1;
                aiProx->whoScaredMe = e;
                aiGetProxEntStatus(aiProx->proxEntStatusTable, e, 1);
            } 
            else if (scareCivilians)
            {
                // NPCs within 50 feet need to be scared.
                if(distSQR < SQR(50)){
                    Vec3 dirToEnt;
                    int scareMe = 0;

                    subVec3(ENTPOS_BY_ID(proxID), ENTPOS(e), dirToEnt);

                    if(distSQR < SQR(10)){
                        // I'm within 10 feet, so scare him.

                        scareMe = 1;
                    }
                    else if(velLength){
                        // If I'm moving towards him, then scare him.

                        float angleDiff = dotVec3(dirToEnt, unitVel);

                        angleDiff /= lengthVec3(dirToEnt);

                        if(angleDiff > 0.1)
                            scareMe = 1;
                    }

                    if(scareMe){
                        aiProx->wasScared = 1;

                        // If I didn't scare this guy, or the guy who did scare him is farther away,
                        //   then I get to scare him.

                        if(    aiProx->whoScaredMe != e
                            ||
                            distSQR < distance3Squared(ENTPOS_BY_ID(proxID), ENTPOS(aiProx->whoScaredMe)))
                        {
                            aiProx->whoScaredMe = e;
                            aiGetProxEntStatus(aiProx->proxEntStatusTable, e, 1);
                        }
                    }
                }
            }
        }

        // Make a separate critter list.

        if(proxEntType == ENTTYPE_CRITTER && critterCount < ARRAY_SIZE(critterArray)){
            critterArray[critterCount].entity = proxEnt;
            critterArray[critterCount].distanceSQR = distSQR;
            critterCount++;
        }
    }

    // Update nearby entity status.

    aiCritterUpdateProxEntStatus(e, ai, entArray, entCount);

    // If there are no players nearby and I've lost my target, just go to sleep.

    if(    !ai->nearbyPlayerCount &&
        !ai->attackTarget.entity &&
        ai->motion.type != AI_MOTIONTYPE_JUMP &&
        ai->motion.type != AI_MOTIONTYPE_FLY &&
        !ai->doNotGoToSleep 
        && !e->ragdoll // don't sleep if there's a ragdoll, so we don't leave it processing
        )
    {
        // Go to sleep if I've been trying to sleep for a while.
        AttribModListIter iter;
        AttribMod *pmod = modlist_GetFirstMod(&iter, &e->pchar->listMod);
        while(pmod!=NULL)
        {
            if(pmod->ptemplate->ppowBase->eType != kPowerType_Auto)
            {
                ai->timerTryingToSleep = 0;
                break;
            }

            pmod = modlist_GetNextMod(&iter);
        }

        if(++ai->timerTryingToSleep >= 6){
            const char* reason = "Going to sleep.";

            aiDiscardFollowTarget(e, reason, false);
            aiDiscardAttackTarget(e, reason);

            if(!ai->doNotChangePowers){
                aiDiscardCurrentPower(e);
            }

            // Put me to sleep, there's no one nearby.

            AI_LOG(AI_LOG_BASIC, (e, "%s\n", reason));

            SET_ENTSLEEPING(e) = 1;

            return;
        }
    }

    //**************************************************************************************
    // Determine attack target.
    //**************************************************************************************

    aiCritterFindTarget(e, ai, entArray, entCount, critterArray, critterCount, findTarget, 1);
}

void aiCallForHelp( Entity * me, AIVars * ai, F32 radius, const char * targetName ){
    int j,i;
    int entArray[MAX_ENTITIES_PRIVATE];

    //Gather the nearby dudes
    j = mgGetNodesInRange(0, ENTPOS(me), (void **)entArray, 0);

    for(i = 0; i < j; i++){
        int proxID = entArray[i];
        Entity* proxEnt = entities[proxID];
        AIVars* aiProx = ENTAI_BY_ID(proxID);

        EntType proxEntType = ENTTYPE_BY_ID(proxID);

        if(proxEnt == me )
            continue;

        
        if( targetName )
        {
            //If "group", then reject all non groupmates
            if( stricmp( targetName, "AITeam" ) == 0 )
            {
                if( !aiProx->teamMemberInfo.team || ai->teamMemberInfo.team != aiProx->teamMemberInfo.team )
                    continue;

            }
            //If you are calling a particular monster, find that monster
            else if( stricmp( targetName, proxEnt->name ) != 0 )
            {
                continue;
            }
        }

        //If you are calling all monsters in a particular radius, look for things in that radius
        if( radius ){
            float distSQR = distance3Squared(ENTPOS(proxEnt), ENTPOS(me));
            if( distSQR > SQR(radius))
                continue;
        }


        //Figure out who's my bud
        if( aiIsMyFriend(me, proxEnt) )
        {
            AIProxEntStatus * status;

            //Look up the guys who have attacked me in the last 30 seconds, for each one
            for( status = ai->proxEntStatusTable->statusHead ; status ; status = status->tableList.next )
            {
                if( status->entity && status->hasAttackedMe && ABS_TIME_SINCE(status->time.attackMe) < SEC_TO_ABS(30) )
                {
                    //Add my attacker to my friend's status table
                    AIProxEntStatus * status2 = aiGetProxEntStatus( aiProx->proxEntStatusTable, status->entity, 1);

                    //Kick friend and say attacker did it
                    status2->hasAttackedMe = 1;
                    status2->time.attackMe = ABS_TIME;
                    status2->time.posKnown = ABS_TIME;
                    aiFeelingSetValue( proxEnt, aiProx, AI_FEELINGSET_PERMANENT, AI_FEELING_BOTHERED, 1);
                }
            }
        }
    }
}


//FOr cutscene
void aiCritterUsePowerNow( Entity * e )
{
    AIPowerInfo * powerInfo = 0;
    AIVars* ai = ENTAI(e);

    if( !e || !ai )     
        return;
 
    //aiSetActivity(e, ai, AI_ACTIVITY_STAND_STILL);
    //aiSetMessageHandler(e, aiCritterMsgHandlerDefault);

    //aiSetActivity(e, ENTAI(e), AI_ACTIVITY_COMBAT);
    //aiSetMessageHandler(e, aiCritterMsgHandlerCombat);

    //Use the power right now
    if( ai->power.current )
    {
        aiUsePower(e, ai->power.current,  ai->attackTarget.entity);
    }

    //TO DO make sure you actually do this and only this
}

AIPowerInfo * aiCritterFindPowerByName( AIVars* ai, const char * powerName )
{
    AIPowerInfo * powerInfo = 0;

    //Set this power as your current power, then attack!
    //TO DO First look for this power by Name 
    if( powerName )
    {
        AIPowerInfo * info;
        for(info = ai->power.list; info; info = info->next)
        {
            if(!stricmp(powerName, info->power->ppowBase->pchName))
            {
                powerInfo = info;
                break;
            }
        }
    }
    return powerInfo;
}

//Outside normal power picking scheme.  Select this power and use it right now
int aiTogglePowerNowByName( Entity * e, const char * powerName, int on )
{
    AIPowerInfo * powerInfo = 0;
    AIVars* ai = ENTAI(e);

    powerInfo = aiCritterFindPowerByName( ai, powerName );
    if( !powerInfo )
        return 0;

    aiSetTogglePower( e, powerInfo, on );

    return 1;
}

//Outside normal power picking scheme.  Select this power and use it right now
//Difference between this and aiCritterUsePowerNow is murky, and should be straightened
int aiUsePowerNowByName( Entity * e, const char * powerName, Entity * target )
{
    AIPowerInfo * powerInfo = 0;
    AIVars* ai = ENTAI(e);

    powerInfo = aiCritterFindPowerByName( ai, powerName );
    if( !powerInfo )
        return 0;

    aiUsePower(e, powerInfo, target );

    return 1;
}

int aiNeverChooseThisPower( Entity * e, const char * powerName )
{
    AIPowerInfo * powerInfo = 0;
    AIVars* ai = ENTAI(e);

    powerInfo = aiCritterFindPowerByName( ai, powerName );

    if( powerInfo )
    {
        powerInfo->neverChoose = 1;
        return 1;
    }
    return 0;
}

//For cutscene
int aiCritterSetCurrentPowerByName( Entity * e, const char * powerName )
{
    AIPowerInfo * powerInfo = 0;
    AIVars* ai = ENTAI(e);

    powerInfo = aiCritterFindPowerByName( ai, powerName );
    
    if( 0 == stricmp( powerName, "BestAttack" ) )
    {
        F32 maxRecharge = 0;
        AIPowerInfo * info;
        AIPowerInfo * bestInfo = 0;
        for(info = ai->power.list; info; info = info->next)
        {
            if(!stricmp(powerName, info->power->ppowBase->pchName))
            {
                if(info->groupFlags & AI_POWERGROUP_BASIC_ATTACK)
                {
                    if( maxRecharge < info->power->ppowBase->fRechargeTime )
                    {
                        bestInfo = info;
                        maxRecharge = info->power->ppowBase->fRechargeTime;
                    }
                }
            }
        }
        powerInfo = bestInfo;
    }

    //Characteristics
    //If no good, then look for this power by power group (//TO DO xlate powerName into groups)
    if( !powerInfo )
        powerInfo = aiChooseAttackPower(e, ENTAI(e), 0, 1000, 0, AI_POWERGROUP_BASIC_ATTACK, 0);

    if( powerInfo )  
    {
        //aiSetActivity(e, ENTAI(e), AI_ACTIVITY_COMBAT);
        //aiSetMessageHandler(e, aiCritterMsgHandlerCombat);
        aiSetCurrentPower( e, powerInfo, 0, 0 ); 
        ai->power.preferred = powerInfo; //TO DO check with martin on this
        ai->doNotChangePowers = 1;
        return 1;
    }

    return 0;
}


void aiCritterChooseRandomPower(Entity* e){
    AIVars* ai = ENTAI(e);
    AIPowerInfo* powers[1000];
    int powersCount = 0;
    AIPowerInfo* info;

    for(info = ai->power.list; info; info = info->next){
        if(    info->isAttack &&
            powersCount < ARRAY_SIZE(powers))
        {
            powers[powersCount++] = info;
        }
    }

    if(powersCount){
        int i = getCappedRandom(powersCount);

        aiSetCurrentPower(e, powers[i], 1, 0);
    }
}

static void aiCritterThink(Entity* e, AIVars* ai){
    float checkTime;

    // powers have been added since the last think call, so update the ai configs
    if(ai->power.dirty)
        aiBehaviorReinitEntity(e, ai->base);
    
    // Update my avoid stuff.
    PERFINFO_AUTO_START("UpdateAvoid", 1);
        aiUpdateAvoid(e, ai);  
    PERFINFO_AUTO_STOP();

    // Check proximity in accordance with my timeslice.

    checkTime = TICKS_TO_ABS(20 + 101 - ai->timeslice);

    if( !ai->time.checkedProximity || ABS_TIME_SINCE(ai->time.checkedProximity) >= checkTime || ai->isCamera){  //MW cameras look for targets as often as they can
        AI_LOG(AI_LOG_BASIC, (e, "Checking Prox\n"));

        ai->time.checkedProximity = ABS_TIME;

        PERFINFO_AUTO_START("checkProx", 1);
            aiCritterCheckProximity(e, ai->findTargetInProximity && ai->doAttackAI && !e->cutSceneActor && !ai->doNotChangeAttackTarget);
        PERFINFO_AUTO_STOP();
    }

    // Do team AI.
    aiTeamThink(&ai->teamMemberInfo);

    // Update feeling table.
    PERFINFO_AUTO_START("updateFeelings", 1);
        aiFeelingUpdateTotals(e, ai);
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("BehaviorProcess", 1);
        aiBehaviorProcess(e, ai->base, &ai->base->behaviors);
    PERFINFO_AUTO_STOP();

    // copy and paste from entaiCritterActivityCombat.c
    if(ai->navSearch->path.head && (ai->followTarget.type == AI_FOLLOWTARGET_ENTITY ||
        ai->followTarget.type == AI_FOLLOWTARGET_MOVING_POINT))
    {
        NavPathWaypoint* lastWaypoint = ai->navSearch->path.tail;
        // If I'm closer to my target than the end of my path is, it's a bad path.

        float distToTargetSQR =
            distance3Squared(    ENTPOS(e),
                                ai->followTarget.pos);

        float distFromTargetToWhereHeWasWhenPathWasMadeSQR =
            distance3Squared(    ai->followTarget.pos,
                                ai->followTarget.posWhenPathFound);

        float distFromTargetToPathEndSQR = ai->followTarget.type == AI_FOLLOWTARGET_ENTITY ? 
                distance3Squared(    ENTPOS(ai->followTarget.entity),        lastWaypoint->pos) :
                distance3Squared(    ai->followTarget.pos,                    lastWaypoint->pos);

        if(    distToTargetSQR < distFromTargetToPathEndSQR &&
            distFromTargetToWhereHeWasWhenPathWasMadeSQR > distToTargetSQR)
        {
            aiSendMessage(e, AI_MSG_BAD_PATH);
        }
    }
    ai->firstThinkTick = 0;
}

void aiCritter(Entity* e, AIVars* ai){
    int canFly;

    if(e->pchar->attrCur.fHitPoints <= 0.0f)
        return;

    canFly = e->pchar->attrCur.fFly > 0.0f && e->pchar->attrCur.fStunned <= 0.0f;

    if(canFly != ai->canFly){
        ai->canFly = canFly;

        if(!canFly && ai->isFlying){
            aiSetFlying(e, ai, 0);
            aiMotionSwitchToNone(e, ai, "Was flying, but I can't fly anymore.");
            zeroVec3(e->motion->vel);
            zeroVec3(e->motion->input.vel);
            e->move_type = MOVETYPE_WALK;
        }
    }

    if(ai->pathLimited && !aiCheckIfInPathLimitedVolume(ENTPOS(e)))
    {
        aiBehaviorAddString(e, ENTAI(e)->base, "Clean,RunIntoDoor(Invincible),DestroyMe");
        ai->startedPathLimited = false;
        ai->pathLimited = false;
    }

    //**************************************************************************************
    // Think.
    //**************************************************************************************

    // Think twice per second.

    ai->accThink -= e->timestep; 

    if(ai->accThink <= 0 || ai->isCamera){
        ai->waitForThink = 0;

        PERFINFO_AUTO_START("Think", 1);
            aiCritterThink(e, ai);
        PERFINFO_AUTO_STOP();

        do{
            ai->accThink += 15;
        }while(ai->accThink <= 0);
    }

    if(!ai->waitForThink || ai->motion.type != AI_MOTIONTYPE_NONE)
    {
        int isPowerActive;

        //**************************************************************************************
        // If I'm following something, then see if I have reached it yet.
        //**************************************************************************************

        PERFINFO_AUTO_START("CheckFollowStatus", 1);
            aiCheckFollowStatus(e, ai);
        PERFINFO_AUTO_STOP();

        //**************************************************************************************
        // If I still have a movement target but haven't reached it, then go towards it.
        //**************************************************************************************

        PERFINFO_AUTO_START("FollowTarget", 1);
            aiFollowTarget(e, ai);
        PERFINFO_AUTO_STOP();

        PERFINFO_AUTO_START("BehaviorOffTick", 1);
            if(ai->base->behaviorRequestOffTickProcessing)
                aiBehaviorProcessOffTick(e, ai->base);
        PERFINFO_AUTO_STOP();

        //**************************************************************************************
        // Turn me to point at whatever I should be pointing at.
        //**************************************************************************************

        // Turn if I'm allowed to or I'm currently using a power.

        isPowerActive = e->pchar->ppowCurrent || e->pchar->ppowQueued;

        if(!ai->motion.ground.waitingForDoorAnim)
        {
            int isLocationPower = 0;
            //    check the current if it is a location power, if it is, lock down the turning so entity will look at the location (not the target entity)
            if (e->pchar->ppowCurrent)
            {
                isLocationPower =    (e->pchar->ppowCurrent->ppowBase->eTargetType == kTargetType_Location)
                                    || (e->pchar->ppowCurrent->ppowBase->eTargetType == kTargetType_Teleport)
                                    || (e->pchar->ppowCurrent->ppowBase->eTargetType == kTargetType_Position);
            }
            else if (e->pchar->ppowQueued)
            {
                //    if the current is not a location power, then check the queued power if it is a location 
                //    dont do this if the current power isn't a location power
                //    current always has higher priority
                isLocationPower =    (e->pchar->ppowQueued->ppowBase->eTargetType == kTargetType_Location)
                                    || (e->pchar->ppowQueued->ppowBase->eTargetType == kTargetType_Teleport)
                                    || (e->pchar->ppowQueued->ppowBase->eTargetType == kTargetType_Position);
            }

            if(    (!isPowerActive &&
                ai->motion.type != AI_MOTIONTYPE_ENTERABLE &&
                entCanSetPYR(e) &&
                (    !ai->isTerrorized || 
                    ai->motion.type) &&
                (    !e->motion->falling ||
                    ai->motion.type == AI_MOTIONTYPE_JUMP ||
                    ai->motion.type == AI_MOTIONTYPE_FLY))
                ||
                (isPowerActive && !isLocationPower &&
                ai->attackTarget.entity))
            {
                aiTurnBody(e, ai, e->pchar->ppowCurrent ? 1 : 0);
            }
            else if(ai->isTerrorized && !ai->motion.type)
            {
                SETB(e->seq->state, STATE_FEAR);
                CLRB(e->seq->sticky_state, STATE_COMBAT);
                aiDiscardCurrentPower(e);
                ai->power.preferred = NULL;
            }
        }
    }
}

static int compareStatusAggro(const AIProxEntStatus **a, const AIProxEntStatus **b ) 
{ 
    return ((*a)->dangerValue - (*b)->dangerValue); 
}
Entity * aiSetAttackTargetToAggroPlayer(Entity *e, AIVars *ai, int critterAggroIndex)
{
    AIProxEntStatus *status = NULL, *newTargetStatus = NULL, **aggroList = NULL;

    assert(e);
    assert(ai);
    assert(critterAggroIndex);        //    critterAggroIndex is base 1 in order to be more accesible to designers

    if (!e || !ai || !critterAggroIndex)
        return NULL;

    for(status = ai->proxEntStatusTable->statusHead;
        status;
        status = status ? status->tableList.next : ai->proxEntStatusTable->statusHead)
    {
        if (ENTTYPE_PLAYER == (ENTTYPE(status->entity)))
        {
            eaPush(&aggroList, status);
        }
    }
    if (eaSize(&aggroList) == 0)
        return NULL;

    eaQSort(aggroList, compareStatusAggro);
    critterAggroIndex = MIN(critterAggroIndex, eaSize(&aggroList));
    newTargetStatus = aggroList[critterAggroIndex-1];
    if (newTargetStatus)
    {
        Entity* oldTarget = ai->attackTarget.entity;
        Entity* newTarget = newTargetStatus->entity;

        if(newTarget != oldTarget)
        {
            aiSetAttackTarget(e, newTarget, newTargetStatus, NULL, 0, 0);
            return newTarget;
        }
        else
        {
            return oldTarget;
        }
    }
    return NULL;
}

Entity *aiGetSecondaryTarget(Entity *e, AIVars *ai, const Power *ppow)
{    
    AIProxEntStatus *status = NULL, *newTargetStatus = NULL, **aggroList = NULL;

    assert(e);
    assert(ai);

    if (!e || !ai)
        return NULL;

    for(status = ai->proxEntStatusTable->statusHead;
        status;
        status = status ? status->tableList.next : ai->proxEntStatusTable->statusHead)
    {
        if (status->entity)
        {
            if (character_PowerAffectsTarget(e->pchar, status->entity->pchar, ppow))
            {
                if(!ppow->ppowBase->bTargetNearGround || entHeight(status->entity, 2.0f)<=1.0f)
                {
                    eaPush(&aggroList, status);
                }
            }
        }
    }

    if (eaSize(&aggroList) == 0)
        return NULL;

    return aggroList[getCappedRandom(eaSize(&aggroList))]->entity;
}