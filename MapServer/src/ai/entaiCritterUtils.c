
#include "entaiprivate.h"
#include "beacon/beaconPath.h"
#include "beacon/beaconPrivate.h"
#include "entity/entity.h"
#include "cmdparse/cmdcommon.h"
#include "entity/character_base.h"
#include "cmdparse/cmdserver.h"
#include "ai/entaiLog.h"
#include "entity/entserver.h"

void aiCritterUpdateAttackTargetVisibility(Entity* e, AIVars* ai){
    F32 distSQR;
    F32 maxDist;
    AIProxEntStatus* status;
    Entity* target;
    AIVars* aiTarget;
    F32 perceptionRadius = ai->overridePerceptionRadius ?
        ai->overridePerceptionRadius : e->pchar->attrCur.fPerceptionRadius;

    target = ai->attackTarget.entity;

    if(!target)
        return;
        
    aiTarget = ENTAI(target);
    
    distSQR = distance3Squared(ENTPOS(e), ENTPOS(target));

    maxDist =    perceptionRadius +
                ai->targetedSightAddRange +
                ai->collisionRadius +
                aiTarget->collisionRadius;
                
    status = ai->attackTarget.status;

    // Check if my target is within range to check for visibility.
    
    if(distSQR >= SQR(maxDist)){
        // I'm outside the visibility range.
        
        if(    (!ai->teamMemberInfo.team || aiTarget->teamMemberInfo.team != ai->teamMemberInfo.team) &&
            ABS_TIME_SINCE(status->time.posKnown) > ai->eventMemoryDuration &&
            ABS_TIME_SINCE(status->time.attackMe) > ai->eventMemoryDuration &&
            ABS_TIME_SINCE(status->time.attackMyFriend) > ai->eventMemoryDuration)
        {
            ai->magicallyKnowTargetPos = 0;
            
            aiDiscardAttackTarget(e, "Forgot target's position.");
            
            if(ai->followTarget.reason == AI_FOLLOWTARGETREASON_ATTACK_TARGET){
                aiDiscardFollowTarget(e, "Forgot about attack target.", false);
            }
        }

        // Check if I can hit with my current power.
        
        if(ai->power.current){
            float losDistance = aiGetPowerRange(e->pchar, ai->power.current) +
                                ai->collisionRadius +
                                aiTarget->collisionRadius;

            ai->canHitTarget = aiEntityCanSeeEntity(e, target, losDistance);
        }
        
        // If I can currently see the target, then toggle state.

        if(ai->canSeeTarget){
            ai->canSeeTarget = 0;
            ai->time.beginCanSeeState = ABS_TIME;
        }
        
        if(status->neverForgetMe){
            ai->magicallyKnowTargetPos = 1;
            ai->time.beginCanSeeState = ABS_TIME;
        }
    }
    else if(ABS_TIME_SINCE(ai->time.checkedLOS) > SEC_TO_ABS(1.5)){
        // I'm inside the visibility range.
        
        int canSee;
        
        if(distSQR < SQR(30)){
            // Update faster when in close proximity.
            
            ai->time.checkedLOS = ABS_TIME - SEC_TO_ABS(0.5);
        }else{
            ai->time.checkedLOS = ABS_TIME;
        }
        
        // Check if I can see my target.
        
        canSee = aiEntityCanSeeEntity(e, target, maxDist);
        
        if(canSee != ai->canSeeTarget){
            AI_LOG(    AI_LOG_PATH,
                    (e,
                    "%s SEE %s^0!!!\n",
                    canSee ? "^2CAN" : "^1CANNOT",
                    AI_LOG_ENT_NAME(target)));

            //AI_POINTLOG(AI_LOG_PATH,
            //            (e,
            //            posPoint(e),
            //            0xff0000,
            //            "%s SEE %s^0!!!\n",
            //            canSee ? "^2CAN" : "^1CANNOT",
            //            AI_LOG_ENT_NAME(target)));

            ai->canSeeTarget = canSee;
            ai->canHitTarget = canSee;
            ai->time.beginCanSeeState = ABS_TIME;
        }
        
        if(canSee || status->neverForgetMe){
            ai->magicallyKnowTargetPos = 1;

            status->time.posKnown = ABS_TIME;
        }
    }
}

void* aiCritterMsgHandlerDefault(Entity* e, AIMessage msg, AIMessageParams params){
    AIVars* ai = ENTAI(e);

    switch(msg){
        case AI_MSG_FOLLOW_TARGET_REACHED:
            {
                int reachedTarget = 0;

                if(ai->followTarget.reason == AI_FOLLOWTARGETREASON_ATTACK_TARGET){
                    if(ai->canSeeTarget){
                        reachedTarget = 1;
                    }else{
                        if(ABS_TIME_SINCE(ai->time.checkedLOS) > SEC_TO_ABS(0.2)){
                            ai->time.checkedLOS = 0;

                            aiCritterUpdateAttackTargetVisibility(e, ai);

                            if(ai->canSeeTarget){
                                reachedTarget = 1;
                            }
                        }
                    }
                }else{
                    reachedTarget = 1;
                }

                if(reachedTarget){ 
                    aiDiscardFollowTarget(e, "Reached follow target.", true);

                    ai->time.checkedForShortcut = 0;
                    ai->doNotRun = ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : 0;
                }

                //MW when you get where you're going, go back to your regularly scheduled ai
                //instead of waiting for the script to tell you what to do next.
                // RvP - this is now handled at the end of the script processing in aiCritterThink
/*
                if( ai->script.releaseScriptControlWhenDone )
                    entaiScriptReset(e);
*/
                
                break;
            }
        case AI_MSG_PATH_BLOCKED:{
            if(ai->preventShortcut){
                aiPathUpdateBlockedWaypoint(ai->navSearch->path.curWaypoint);
            
                // The target isn't a player, so I'm probably just wandering around.
                //   I'll just find a new path eventually.
                
                AI_LOG(    AI_LOG_PATH,
                        (e,
                        "Got stuck while not targeting a player.\n"));
                
                aiDiscardFollowTarget(e, "Ran into something.", false);

                ai->waitingForTargetToMove = 0;
            }else{
                // If I'm stuck and I have already tried short-circuiting,
                //   then redo the path but don't allow short-circuiting.
                
                AI_LOG(    AI_LOG_PATH,
                        (e,
                        "I got stuck, trying not short-circuiting.\n"));
                
                aiConstructPathToTarget(e);
                
                aiStartPath(e, 1, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun /*ai->attackTarget.entity ? 0 : 1*/);

                ai->waitingForTargetToMove = 1;
            }

            break;
        }
    }
    
    return 0;
}

int aiFindAverageEnemyPos(Entity* e, AIVars* ai, Vec3 averageEnemy)
{
    int count=0;
    setVec3(averageEnemy, 0, 0, 0);
    if (ai->teamMemberInfo.team) {
        AIProxEntStatus *status = ai->teamMemberInfo.team->proxEntStatusTable->statusHead;
        for (; status; status = status->tableList.next) {
            if (status->entity) {
                count++;
                addVec3(averageEnemy, ENTPOS(status->entity), averageEnemy);
            }
        }
        if (count) {
            averageEnemy[0]/=count;
            averageEnemy[1]/=count;
            averageEnemy[2]/=count;
        }
    }
    return count;
}

static struct {
    BeaconBlock*    beaconCluster;
    Vec3            pos;
    float            maxRangeSQR;
    float            bestRangeSQR;
    Beacon*         best;
} staticBeaconSearchData;

static void aiFindBeaconFarthestFromHelper(Array* beaconArray, void* unused){
    int i;

    for(i = 0; i < beaconArray->size; i++){
        Beacon* b = beaconArray->storage[i];
        BeaconBlock* beaconCluster = b->block && b->block->galaxy ? b->block->galaxy->cluster : NULL;

        if(    (!staticBeaconSearchData.beaconCluster || staticBeaconSearchData.beaconCluster == beaconCluster) && 
            (b->gbConns.size || b->rbConns.size))
        {
            float distSQR = distance3Squared(posPoint(b), staticBeaconSearchData.pos);

            if(distSQR <= staticBeaconSearchData.maxRangeSQR && distSQR > staticBeaconSearchData.bestRangeSQR)
            {
                staticBeaconSearchData.best = b;
                staticBeaconSearchData.bestRangeSQR = distSQR;
            }
        }
    }
}

static Beacon* aiFindBeaconFarthestFrom(Vec3 centerPos, BeaconBlock* beaconCluster, float maxRange)
{
    copyVec3(centerPos, staticBeaconSearchData.pos);
    staticBeaconSearchData.beaconCluster = beaconCluster;
    staticBeaconSearchData.maxRangeSQR = SQR(maxRange);
    staticBeaconSearchData.bestRangeSQR = 0;
    staticBeaconSearchData.best = NULL;

    beaconForEachBlock(centerPos, maxRange, maxRange, maxRange, aiFindBeaconFarthestFromHelper, NULL);

    return staticBeaconSearchData.best;
}

int aiCritterDoTeleport(Entity* e, AIVars* ai, AIPowerInfo* telePower)
{
    Vec3 averageEnemy;
    if (aiFindAverageEnemyPos(e, ai, averageEnemy)) {
        Beacon*    nearestBeacon = entGetNearestBeacon(e);
        BeaconBlock* beaconGalaxy = nearestBeacon && nearestBeacon->block ? nearestBeacon->block->galaxy : NULL;
        BeaconBlock* beaconCluster = beaconGalaxy ? beaconGalaxy->cluster : NULL;
        Beacon* dest;

        dest = aiFindBeaconFarthestFrom(averageEnemy, beaconCluster, 80.0);

        if (dest) {
            aiUsePowerOnLocation(e, telePower, posPoint(dest));
            return 1;
        }
    }
    return 0;
}