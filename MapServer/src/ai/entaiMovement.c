
#include "entaiprivate.h"
#include "entaiCritterPrivate.h"
#include "entity/motion.h"
#include "beacon/beaconConnection.h"
#include "beacon/beaconPath.h"
#include "beacon/beaconPrivate.h"
#include "generator/patrol.h"
#include "dbcomm/dbdoor.h"
#include "seq/seqstate.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "gameSys/dooranim.h"
#include "cmdparse/cmdcommon.h"
#include "entity/character_base.h"
#include "cmdparse/cmdserver.h"
#include "ai/entaiLog.h"
#include "ai/entai.h"
#include <utilitieslib/utils/timing.h>

void aiStartPath(Entity* e, int preventShortcut, int doNotRun){
    AIVars* ai = ENTAI(e);

    ai->time.checkedForShortcut = 0;
    ai->shortcutWasTaken = 0;
    ai->failedMoveCount = 0;
    ai->preventShortcut = preventShortcut;
    ai->doNotRun = doNotRun;
    ai->motion.ground.notOnPath = 0;
    ai->motion.ground.waitingForDoorAnim = 0;

    if(ai->motion.type != AI_MOTIONTYPE_JUMP){
        ai->motion.type = AI_MOTIONTYPE_STARTING;
    }
}


void aiTurnBody(Entity* e, AIVars* ai, int forceFaceTarget){ 
    Vec3 moveDir;

    // Point at the attack target if I'm not trying to move.

    if(    ai->attackTarget.entity &&
        !e->do_not_face_target &&
        !ai->isStunned &&
        (    forceFaceTarget ||
            (    !ai->motion.type &&
                ai->canHitTarget)))
    {
        subVec3(ENTPOS(ai->attackTarget.entity), ENTPOS(e), moveDir);

        vecY(moveDir) = 0;

        //assert(lengthVec3(moveDir));

        aiTurnBodyByDirection(e, ai, moveDir);

        //AI_LOG(    AI_LOG_PATH,
        //        (e,
        //        "Turning towards attack target: %s\n",
        //        AI_LOG_ENT_NAME(ai->attackTarget)));
    }
    else if(!forceFaceTarget && ai->motion.type){
        subVec3(ai->motion.target, ENTPOS(e), moveDir);

//        aiEntSendAddLine(ENTPOS(e), 0xffffffff, ai->motion.target, 0x0000ffff);
//        aiEntSendAddText(ai->motion.target, "Target", 0);

        if(fabs(vecX(moveDir)) > 0.01 || fabs(vecZ(moveDir)) > 0.01){

            if(!ai->pitchWhenTurning)
                vecY(moveDir) = 0;

            aiTurnBodyByDirection(e, ai, moveDir);

            //AI_LOG(    AI_LOG_PATH,
            //        (e,
            //        "Turning towards path target.\n"));
        }
    }
}

static void aiMotionOffsetJumpTarget(Entity* e, AIVars* ai, float floorDist){
    Vec3 posCheck;
    int maxOffset;
    int exclusiveVisionPhase = e ? e->exclusiveVisionPhase : 1;

    vecY(ai->motion.target) -= floorDist;

    for(maxOffset = 7; maxOffset > 0; maxOffset--){
        copyVec3(ai->motion.target, posCheck);
        vecX(posCheck) += rand() % (maxOffset * 2 + 1) - maxOffset;
        vecZ(posCheck) += rand() % (maxOffset * 2 + 1) - maxOffset;

        if(aiPointCanSeePoint(ai->motion.target, 2, posCheck, 2, 0)){
            if(aiGetPointGroundOffset(posCheck, 1, 2, posCheck)){
                if(aiCanWalkBetweenPointsQuickCheck(ai->motion.target, 3, posCheck, 3)){
                    copyVec3(posCheck, ai->motion.target);

                    return;
                }
            }
        }
    }
}

static int aiCanFlyToPosition(Entity* e, const Vec3 target){
    Vec3 p1, p2;
    int exclusiveVisionPhase = e ? e->exclusiveVisionPhase : 1;

    copyVec3(ENTPOS(e), p1);
    copyVec3(target, p2);

    vecY(p1) += 1;
    vecY(p2) += 1;

    if(!aiPointCanSeePoint(p1, 0, p2, 0, 0)){
        return 0;
    }

    vecY(p1) += 2;
    vecY(p2) += 2;

    if(!aiPointCanSeePoint(p1, 0, p2, 0, 0)){
        return 0;
    }

    vecY(p1) += 2;
    vecY(p2) += 2;

    if(!aiPointCanSeePoint(p1, 0, p2, 0, 0)){
        return 0;
    }

    return 1;
}

static void aiMotionSwitchToGround(Entity* e, AIVars* ai, const char* reason){
    AI_LOG(AI_LOG_WAYPOINT, (e, "Switching motion to ^2GROUND^0: %s\n", reason ? reason : "No Reason"));

    ai->motion.type = AI_MOTIONTYPE_GROUND;
}

static S32 aiMotionSwitchToFly(Entity* e, AIVars* ai, const char* reason){
    if(e->pchar && e->pchar->attrCur.fStunned > 0.0f){
        AI_LOG(AI_LOG_WAYPOINT, (e, "I can't switch to ^2FLY^0 when stunned: %s\n", reason ? reason : "No Reason"));
        
        ai->motion.type = AI_MOTIONTYPE_NONE;
        
        return 0;
    }
    
    AI_LOG(AI_LOG_WAYPOINT, (e, "Switching motion to ^2FLY^0: %s\n", reason ? reason : "No Reason"));

    ai->motion.type = AI_MOTIONTYPE_FLY;
    
    return 1;
}

static void aiMotionSwitchToJump(Entity* e, AIVars* ai, const char* reason){
    if(e->pchar && e->pchar->attrCur.fStunned > 0.0f){
        AI_LOG(AI_LOG_WAYPOINT, (e, "I can't switch to ^2JUMP^0 when stunned: %s\n", reason ? reason : "No Reason"));
        
        ai->motion.type = AI_MOTIONTYPE_NONE;
        
        return;
    }

    AI_LOG(AI_LOG_WAYPOINT, (e, "Switching motion to ^2JUMP^0: %s\n", reason ? reason : "No Reason"));
    
    ai->motion.type = AI_MOTIONTYPE_JUMP;
    ai->motion.jump.time = 0;
}

static void aiMotionSwitchToWire(Entity* e, AIVars* ai, const char* reason){
    AI_LOG(AI_LOG_WAYPOINT, (e, "Switching motion to ^2WIRE^0: %s\n", reason ? reason : "No Reason"));

    ai->motion.type = AI_MOTIONTYPE_WIRE;
}

void aiMotionSwitchToNone(Entity* e, AIVars* ai, const char* reason){
    if(ai->motion.type != AI_MOTIONTYPE_NONE){
        AI_LOG(AI_LOG_WAYPOINT, (e, "Switching motion to ^1NONE^0: %s\n", reason ? reason : "No Reason"));
        ai->motion.type = AI_MOTIONTYPE_NONE;
    }
}

static void aiMotionSwitchToTransition(Entity* e, AIVars* ai, const char* reason){
    AI_LOG(AI_LOG_WAYPOINT, (e, "Switching motion to ^2TRANSITION^0: %s\n", reason ? reason : "No Reason"));

    ai->motion.type = AI_MOTIONTYPE_TRANSITION;
}

static void aiMotionSwitchToEnterable(Entity* e, AIVars* ai, const char* reason)
{
    AI_LOG(AI_LOG_WAYPOINT, (e, "Switching motion to ^2ENTERABLE^0: %s\n", reason ? reason : "No Reason"));

    ai->motion.type = AI_MOTIONTYPE_ENTERABLE;

    ai->motion.ground.waitingForDoorAnim = 0;
}

static void aiMotionFailMove(Entity* e, AIVars* ai, F32 distFromTickTarget)
{
    NavPath* path = &ai->navSearch->path;

    if(ai->sendEarlyBlock)
        aiSendMessage(e, AI_MSG_PATH_EARLY_BLOCK);

    if(ai->failedMoveCount < 10){
        AI_LOG(    AI_LOG_PATH,
                (e,
                "^1Failed tick move ^4%d^0: ^4%1.4f ^0/ ^4%1.4f\n",
                ai->failedMoveCount,
                distFromTickTarget,
                ai->motion.ground.expectedTickMoveDist));

        ai->failedMoveCount++;
        
        return;
    }
    
    // Too many failed moves, so try something else.
    
    if(    path->curWaypoint &&
        path->curWaypoint->beacon &&
        path->curWaypoint->beacon->pathsBlockedToMe >= 3 &&
        (    ai->canFly ||
            distance3Squared(ENTPOS(e), posPoint(path->curWaypoint->beacon)) < SQR(30)))
    {
        AI_LOG(    AI_LOG_PATH,
                (e,
                "^1Failed final tick move ^4%d^0: ^4%1.4f ^0/ ^4%1.4f ^0- Jumping to (^4%1.1f^0,^4%1.1f^0,^4%1.1f^0).\n",
                ai->failedMoveCount,
                distFromTickTarget,
                ai->motion.ground.expectedTickMoveDist,
                vecParamsXYZ(ai->motion.target)));

        copyVec3(ENTPOS(e), ai->motion.source);

        ai->motion.minHeight = vecY(ai->motion.target) - ENTPOSY(e) + 3;
        ai->motion.maxHeight = ai->motion.minHeight;

        aiMotionSwitchToJump(e, ai, "Tick move failed too many times.");
    }else{
        ai->motion.failedPathWhileFalling = e->motion->falling;
        
        // I've been stuck for long enough to worry, notify entity.

        AI_LOG(    AI_LOG_PATH,
                (e,
                "^1Failed final tick move ^4%d^0: ^4%1.4f ^0/ ^4%1.4f ^0- Notifying AI.\n",
                ai->failedMoveCount,
                distFromTickTarget,
                ai->motion.ground.expectedTickMoveDist));

        aiSendMessage(e, AI_MSG_PATH_BLOCKED);
    }
}

static void aiMotionGroundCheckLastMove(Entity* e, AIVars* ai){
    NavSearch* navSearch;
    NavPath* path;
    F32 distFromTickTarget;

    if(ai->motion.ground.expectedTickMoveDist <= 0){
        return;
    }
    
    navSearch = ai->navSearch;
    path = &navSearch->path;

    //AI_LOG(    AI_LOG_PATH,
    //        (e, "expectedMoveDist == ^4%1.3f\n", ai->motion.ground.expectedTickMoveDist));

    // Calculate the XZ distance that I am from the position I wanted to be in on this frame.

    distFromTickTarget = distance3XZ(ENTPOS(e), ai->motion.ground.lastTickDestPos);

    if(distFromTickTarget <= ai->motion.ground.expectedTickMoveDist * 0.9){
        return;
    }
    
    aiMotionFailMove(e, ai, distFromTickTarget); // Tick move failed.
}

static int aiMotionGroundCheckShortcut(Entity* e, AIVars* ai){
    NavSearch* navSearch = ai->navSearch;
    NavPath* path = &navSearch->path;

    if(path->head && !ai->preventShortcut){
        if(ABS_TIME_SINCE(ai->time.checkedForShortcut) > SEC_TO_ABS(3)){
            int switched = 0;
            F32 distSQR = distance3Squared(ENTPOS(e), ai->followTarget.pos);

            // Check for shortcut to the target.

            ai->time.checkedForShortcut = ABS_TIME;

            if(distSQR < SQR(150)){
                if(distSQR > SQR(40) && ai->canFly && !ai->isFlying){
                    // Within range of target, so check if I can fly straight to the target.

                    if(aiCanFlyToPosition(e, ai->followTarget.pos)){
                        AI_LOG(    AI_LOG_PATH,
                                (e,
                                "Switching to flight, flying straight to target.\n"));

                        //aiEntSendAddLine(posPoint(e), 0xffffffff, ai->followTarget.pos, 0xffff0000);

                        clearNavSearch(navSearch);

                        ai->time.foundPath = ABS_TIME;
                        ai->shortcutWasTaken = 1;
                        ai->motion.fly.notOnPath = 1;
                        
                        if(aiMotionSwitchToFly(e, ai, "Flying straight from ground to target.")){
                            zeroVec3(e->motion->vel);
                        }

                        switched = 1;
                    }
                }

                if(    !switched &&
                    !ai->isFlying &&
                    !ai->motion.ground.notOnPath &&
                    aiCanWalkBetweenPointsQuickCheck(ENTPOS(e), 4, ai->followTarget.pos, 4) &&
                    ai->followTarget.type != AI_FOLLOWTARGET_ENTERABLE)
                {
                    // Within range of target, so check if I can walk straight to the target.

                    AI_LOG(    AI_LOG_PATH,
                            (e,
                            "Shortcutting path (^4%1.1f^0,^4%1.1f^0,^4%1.1f^0) - (^4%1.1f^0,^4%1.1f^0,^4%1.1f^0)\n",
                            ENTPOSPARAMS(e),
                            vecParamsXYZ(ai->followTarget.pos)));

                    clearNavSearch(navSearch);

                    ai->time.foundPath = ABS_TIME;
                    ai->shortcutWasTaken = 1;
                    ai->motion.ground.notOnPath = 1;

                    switched = 1;
                }
            }

            if(!switched && path->curWaypoint && path->curWaypoint->next){
                // Check if I can skip to the next path waypoint.

                NavPathWaypoint* nextWaypoint = path->curWaypoint->next;

                if(nextWaypoint->connectType != NAVPATH_CONNECT_GROUND){
                    Vec3 top;

                    copyVec3(nextWaypoint->pos, top);

                    if(ai->canFly && aiCanFlyToPosition(e, top)){
                        //aiEntSendAddLine(posPoint(e), 0xffffffff, top, 0xff00ff00);

                        AI_LOG(    AI_LOG_PATH,
                                (e,
                                "Switching to flight, flying to next waypoint (^4%1.f^0,^4%1.f^0,^4%1.f^0)\n",
                                vecParamsXYZ(top)));

                        if(aiMotionSwitchToFly(e, ai, "Shortcutting from ground to next waypoint.")){
                            ai->motion.fly.notOnPath = 0;

                            path->curWaypoint = nextWaypoint;

                            copyVec3(ENTPOS(e), ai->motion.source);
                            copyVec3(top, ai->motion.target);
                        }

                        return 1;
                    }
                }else{
                    if(    distance3Squared(ENTPOS(e), nextWaypoint->pos) < SQR(150) &&
                        aiCanWalkBetweenPointsQuickCheck(ENTPOS(e), 4, nextWaypoint->pos, 4))
                    {
                        AI_LOG(    AI_LOG_PATH,
                                (e,
                                "Shortcutting to waypoint at (^4%1.f^0,^4%1.f^0,^4%1.f^0)\n",
                                vecParamsXYZ(nextWaypoint->pos)));

                        path->curWaypoint = nextWaypoint;

                        ai->shortcutWasTaken = 1;

                        aiMotionSwitchToTransition(e, ai, "Shortcutting to a waypoint.");

                        return 1;
                    }
                }
            }
        }
    }
    else if(!path->head && ai->followTarget.type && ai->motion.ground.notOnPath)
    {
        if(!sameVec3(ai->motion.target, ai->followTarget.pos) &&
            !aiCanWalkBetweenPointsQuickCheck(ENTPOS(e), 4, ai->motion.target, 4))
        {
            aiMotionFailMove(e, ai, -1);
            return 1;
        }
    }

    return 0;
}

static void aiMotionGroundDoMovement(Entity* e, AIVars* ai){
    NavSearch* navSearch = ai->navSearch;
    NavPath* path = &navSearch->path;
    Vec3 movementDirection = {0,0,0};
    MotionState* motion = e->motion;
    F32 distToTargetXZ = distance3XZ(ENTPOS(e), ai->motion.target);
    S32 onSlope = 1 - e->motion->surf_normal[1] >= 0.3;
    F32 maxSpeed = e->motion->input.surf_mods[SURFPARAM_GROUND].max_speed;
    F32 topSpeed;

    if(ai->inputs.limitedSpeed > 0.0) 
        topSpeed = ai->inputs.limitedSpeed;
    else
        topSpeed = maxSpeed;

    if( ai->inputs.boostSpeed )
        topSpeed *= ai->inputs.boostSpeed;

    if(ai->doNotRun){
        topSpeed /= 4.8;
    }

    if(distToTargetXZ < 3){
        float offsetFromTargetY = ENTPOSY(e) - vecY(ai->motion.target);
        F32 minDist = MIN(ai->followTarget.standoffDistance, 2);

        if(distToTargetXZ < minDist && offsetFromTargetY > -0.5){
            // Within 2 feet of target and above half a foot below beacon floor position.

            if(path->curWaypoint){
                Beacon* beacon = path->curWaypoint->beacon;

                // Decrement the count of paths that were blocked to me.

                if(beacon && beacon->pathsBlockedToMe > 0){
                    beacon->pathsBlockedToMe--;
                }

                path->curWaypoint = path->curWaypoint->next;

                if(path->curWaypoint){
                    // More path to go, so go to transition mode.

                    aiMotionSwitchToTransition(e, ai, "Reached ground waypoint.");
                }else{
                    // End of the path, but got here by ground, so just walk to the end (hopefully not falling off a cliff).

                    ai->motion.ground.notOnPath = 1;
                }
            }else{
                aiMotionSwitchToNone(e, ai, "On ground and no next waypoint.");
            }

            if(ai->curSpeed){
                ai->setForwardBit = 1;
            }

            return;
        }
        else if(offsetFromTargetY > -8 &&
                offsetFromTargetY < -2 &&
                path->curWaypoint &&
                !path->curWaypoint->prev &&
                e->motion->jump_height > 0)
        {
            // Within two feet of first beacon, but way below beacon for some reason, so jump.

            copyVec3(ENTPOS(e), ai->motion.source);

            ai->motion.minHeight = vecY(ai->motion.target) - ENTPOSY(e) + 3;
            ai->motion.maxHeight = ai->motion.minHeight;

            aiMotionSwitchToJump(e, ai, "Below first beacon, not sure why.");

            return;
        }
        else if(distToTargetXZ < 0.1){
            NavPathWaypoint* curWaypoint = path->curWaypoint;

            // Very close to target, so just assume I'm there.

            if(curWaypoint){
                Beacon* beacon = curWaypoint->beacon;

                AI_LOG(    AI_LOG_WAYPOINT,
                        (e,
                        "Reached waypoint: (^4%1.f^0,^4%1.f^0,^4%1.f^0)\n",
                        vecParamsXYZ(curWaypoint->pos)));

                // Decrement the count of paths that were blocked to me.

                if(beacon && beacon->pathsBlockedToMe > 0){
                    beacon->pathsBlockedToMe--;
                }

                path->curWaypoint = curWaypoint = curWaypoint->next;

                aiMotionSwitchToTransition(e, ai, "Reached ground waypoint.");
            }else{
                aiMotionSwitchToNone(e, ai, "On ground, and very close to current target, but no next waypoint.");
            }

            if(ai->curSpeed){
                ai->setForwardBit = 1;
            }

            zeroVec3(e->motion->input.vel);
            zeroVec3(e->motion->vel);

            return;
        }
    }

    if(ai->curSpeed < topSpeed){
        ai->curSpeed += e->timestep * (topSpeed - ai->curSpeed) / 20.0f;
        if(ai->curSpeed > topSpeed){
            ai->curSpeed = topSpeed;
        }
    }
    else if(ai->curSpeed > topSpeed){
        ai->curSpeed -= e->timestep * (ai->curSpeed - topSpeed) / 10.0f;
        if(ai->curSpeed < topSpeed){
            ai->curSpeed = topSpeed;
        }
    }

    //AI_LOG(    AI_LOG_WAYPOINT,
    //        (e,
    //        "Waypoint Dist: ^4%1.2f^0, "
    //        "Yaw to waypoint: ^4%1.2f^0, ",
    //        distToTargetXZ,
    //        DEG(aiYawBetweenPoints(ai->motion.target, posPoint(e)))));

    // Move towards the target.

    subVec3(ai->motion.target, ENTPOS(e), movementDirection);

    vecY(movementDirection) = 0;

    normalVec3(movementDirection);

    // Set the velocity in the direction of movement.

    if(ai->curSpeed){
        float speed;

        if(ai->curSpeed > distToTargetXZ){
            speed = distToTargetXZ;
        }else{
            speed = ai->curSpeed;
        }

        if(ai->doNotRun && onSlope){
            speed *= 4.8;
        }

        e->motion->input.max_speed_scale = speed / maxSpeed;

        copyVec3(movementDirection, e->motion->input.vel);

        ai->setForwardBit = 1;
    }

    //AI_LOG(    AI_LOG_WAYPOINT,
    //        (e,
    //        "Speed: ^4%1.2f\n",
    //        lengthVec3XZ(e->motion->inp_vel)));
}

static int pointInAvoidArea(Entity* e, AIVars* ai, const Vec3 checkPos){
    AIProxEntStatus* status = ai->proxEntStatusTable->avoid.head;

    for(; status; status = status->avoidList.next){
        Entity* entAvoid = status->entity;
        AIVars* aiAvoid = ENTAI(entAvoid);
        S32 entityLevel = (e->pchar->bIgnoreCombatMods ? (MAX_PLAYER_SECURITY_LEVEL+4) : e->pchar->iCombatLevel);
        S32 entAvoidLevel = (entAvoid->pchar->bIgnoreCombatMods ? (MAX_PLAYER_SECURITY_LEVEL+4) : entAvoid->pchar->iCombatLevel);
        S32 levelDiff = entityLevel - entAvoidLevel;
        AIAvoidInstance* avoid;

        for(avoid = aiAvoid->avoid.list; avoid; avoid = avoid->next){
            if(levelDiff > avoid->maxLevelDiff){
                continue;
            }

            if(distance3Squared(checkPos, ENTPOS(entAvoid)) <= SQR(avoid->radius)){
                return 1;
            }
        }
    }

    return 0;
}

static int aiMotionGroundCheckAvoid(Entity* e, AIVars* ai, Vec3 tickVel){
    AIProxEntStatus* status = ai->proxEntStatusTable->avoid.head;
    Vec3 destPos;

    if(!status || ai->followTarget.reason == AI_FOLLOWTARGETREASON_AVOID || !e->pchar){
        return 0;
    }

    copyVec3(tickVel, destPos);
    normalVec3(destPos);
    scaleVec3(destPos, 5 + ai->collisionRadius, destPos);
    addVec3(ENTPOS(e), destPos, destPos);

    if(pointInAvoidArea(e, ai, destPos)){
        if(!pointInAvoidArea(e, ai, ENTPOS(e))){
            if(ai->motion.ground.notOnPath){
                ai->motion.ground.notOnPath = 0;
                
                {
                    //const char* reason = "Was about to walk into avoid sphere.";
                    //aiMotionSwitchToNone(e, ai, reason);
                    //aiDiscardFollowTarget(e, reason, false);
                    //ai->preventShortcut = 1;
                }
            }

            ai->motion.ground.expectedTickMoveDist = 0;

            zeroVec3(e->motion->input.vel);

            ai->setForwardBit = 0;
        }else{
            ai->doNotRun = 0;
        }

        return 1;
    }

    return 0;
}

static void aiMotionGroundCheckDestination(Entity* e, AIVars* ai){
    Vec3    tickVel;
    F32        expectedSpeed;

    // Scale the velocity by the timestep to get the expected destination.

    expectedSpeed = e->motion->input.surf_mods[0].max_speed * e->motion->input.max_speed_scale * e->timestep;

    scaleVec3(e->motion->input.vel, expectedSpeed, tickVel);

    addVec3(ENTPOS(e), tickVel, ai->motion.ground.lastTickDestPos);

    ai->motion.ground.expectedTickMoveDist = lengthVec3XZ(tickVel);

    if(expectedSpeed){
        int avoiding = aiMotionGroundCheckAvoid(e, ai, tickVel);

        if(    !avoiding &&
            ai->followTarget.type &&
            ( ai->followTarget.reason == AI_FOLLOWTARGETREASON_SPAWNPOINT ||
            ai->followTarget.reason == AI_FOLLOWTARGETREASON_CREATOR ) )
        {
            F32 distSQR = distance3Squared(ENTPOS(e), ai->followTarget.pos);

            if(ai->inputs.doNotRunIsSet)
                ai->doNotRun = ai->inputs.doNotRun;
            else if(ai->followTarget.reason == AI_FOLLOWTARGETREASON_SPAWNPOINT)
                ai->doNotRun = (distSQR < SQR(ai->walkToSpawnRadius));
            else if(ai->followTarget.reason == AI_FOLLOWTARGETREASON_CREATOR)
                ai->doNotRun = (distSQR < SQR(AI_RUN_TO_CREATOR_DIST));
        }
    }
}

static void aiMotionGround(Entity* e, AIVars* ai){
    NavSearch* navSearch = ai->navSearch;
    NavPath* path = &navSearch->path;
    MotionState* motion = e->motion;

    e->move_type = MOVETYPE_WALK;

    // Check if I'm stuck.

    aiMotionGroundCheckLastMove(e, ai);

    // If I'm falling or otherwise unable to set input velocity, then return.

    if(!entCanSetInpVel(e) || motion->falling){
        //ai->motion.ground.expectedTickMoveDist = 0;
        //if(e->motion->falling){
        //    AI_LOG(AI_LOG_PATH, (e, "falling!!!\n"));
        //}
        return;
    }

    //    should I really just be jumping there
    if (ai->motion.superJumping)
    {
        float y_diff;
        //    get jump distance

        copyVec3(ENTPOS(e), ai->motion.source);
        copyVec3(ai->followTarget.pos, ai->motion.target);

        y_diff = MAX(0, vecY(ai->motion.target) - vecY(ai->motion.source));

        ai->motion.minHeight = max(0, y_diff) + 3 + distance3XZ(ai->motion.source, ai->motion.target) / 6;
        ai->motion.maxHeight = ai->motion.minHeight;

        //            aiMotionOffsetJumpTarget(e, ai, 0);
        aiMotionSwitchToJump(e, ai, "SuperJump");
        return;
    }

    // Check if I can skip any nodes in the path.
    if(!aiMotionGroundCheckShortcut(e, ai)){
        // If I'm not following the path, then reset my motion target to be what I'm following.

        if(ai->followTarget.type && ai->motion.ground.notOnPath){
            copyVec3(ai->followTarget.pos, ai->motion.target);
        }

        // Assume if we got to here that motionSource and motionTarget are valid.

        aiMotionGroundDoMovement(e, ai);

        aiMotionGroundCheckDestination(e, ai);
    }
}

static void aiMotionJump(Entity* e, AIVars* ai){
    Vec3 horizontalVec;
    float distance = distance3XZ(ai->motion.source, ai->motion.target);
    float totalTime;
    float amountMoved;
    float height1, height2;
    float initialVelocity;
    float finalVelocity;
    float time1, time2;
    float gravity = -9.8 / 30.0 / 4;
    float minHeight = ai->motion.minHeight + min(10, 1 + sqrt(distance));
    float lastTime = e->timestep;
    float jumpTime;

    ai->motion.jump.time += e->timestep;
    if (ai->motion.superJumping)
    {
        gravity *= ai->motion.jump.superJumpSpeedMultiplier;        //    slower jump
    }
    jumpTime = ai->motion.jump.time;

    if(minHeight < 0)
        minHeight = 0;

    height1 = minHeight;
    height2 = vecY(ai->motion.source) + minHeight - vecY(ai->motion.target);

    if(height1 < 0 || height2 < 0){
        aiMotionSwitchToNone(e, ai, "Jump appears to be invalid.");
        return;
    }

    initialVelocity = sqrt(2.0 * -gravity * height1);
    finalVelocity = sqrt(2.0 * -gravity * height2);

    time1 = -initialVelocity / gravity;
    time2 = -finalVelocity / gravity;

    totalTime = time1 + time2;

    e->move_type = 0;

    // Calculate the apex of the jump.

    vecX(horizontalVec) = vecX(ai->motion.target) - vecX(ai->motion.source);
    vecY(horizontalVec) = 0;
    vecZ(horizontalVec) = vecZ(ai->motion.target) - vecZ(ai->motion.source);

    // How far have I jumped so far.

    amountMoved = jumpTime / totalTime;

    if(amountMoved < 1){
        Vec3 horizontalMovement;
        Vec3 dest;

        SETB(e->seq->state, STATE_FORWARD);
        SETB(e->seq->state, STATE_AIR);

        scaleVec3(horizontalVec, amountMoved, horizontalMovement);

        addVec3(ai->motion.source, horizontalMovement, dest);

        vecY(dest) = vecY(ai->motion.source) + initialVelocity * ai->motion.jump.time + gravity * SQR(ai->motion.jump.time) / 2.0;

        zeroVec3(e->motion->input.vel);
        zeroVec3(e->motion->vel);

        entUpdatePosInterpolated(e, dest);
        copyVec3(dest, e->motion->last_pos);

        if(jumpTime < time1){
            // Turn on the jump bit until the apex.

            if(jumpTime > time1 - 10){
                SETB(e->seq->state, STATE_APEX);
            }else{
                SETB(e->seq->state, STATE_JUMP);
            }

            //entSendAddText(dest, "set jump", 0);

        }
        else if(jumpTime < totalTime - 0.2 * 30.0){
            // Turn on the apex bit until right before the landing.

            if(lastTime < time1)
            {
                SETB(e->seq->state, STATE_APEX);
            }

            if(ENTPOSY(e)-ai->motion.target[1] < 10.0f)
            {
                //SETB(e->seq->state, STATE_LANDING);
            }

            //entSendAddText(dest, "set air", 0);
        }

    }else{
        if((ai->motion.jump.time - e->timestep) / totalTime < 1){
            entUpdatePosInterpolated(e, ai->motion.target);
            copyVec3(ai->motion.target, e->motion->last_pos);
        }

        e->move_type = MOVETYPE_WALK;

        if(ai->motion.jump.time - totalTime > 10.0){
            NavSearch* navSearch = ai->navSearch;
            NavPath* path = &navSearch->path;

            AI_LOG(    AI_LOG_PATH,
                    (e,
                    "Done jumping.\n"));

            if(path->curWaypoint){
                path->curWaypoint = path->curWaypoint->next;

                if(path->curWaypoint){
                    AI_LOG(    AI_LOG_WAYPOINT,
                            (e,
                            "Going to next waypoint (^4%1.f^0,^4%1.f^0,^4%1.f^0).\n",
                            vecParamsXYZ(path->curWaypoint->pos)));
                }
            }else{
                AI_LOG(    AI_LOG_WAYPOINT,
                        (e,
                        "End of the path.\n"));
            }

            aiMotionSwitchToTransition(e, ai, "Finished jumping.");
        }else{
            //AI_LOG(AI_LOG_PATH, (e, "Waiting to end jump: %1.2f\n", ai->motion.time));
        }
    }
}

#define FORWARD_MOVEMENT_ONLY    0
#define CLOSE_ENOUGH_DISTANCE    1.0f
static void aiMotionFly(Entity* e, AIVars* ai){ 
    float fullSpeed;
    Vec3 pos;
    Vec3 oldPos;
    F32 distRemaining;
    F32 totalDistMoved = 0;
    int checkForShortcut = 0;
    NavPath* path = &ai->navSearch->path;
    F32 maxSpeed = e->motion->input.surf_mods[SURFPARAM_AIR].max_speed;

    if(ai->inputs.limitedSpeed > 0.0)
        fullSpeed = ai->inputs.limitedSpeed;
    else
        fullSpeed = maxSpeed;

    if( ai->inputs.boostSpeed )
        fullSpeed *= ai->inputs.boostSpeed;

    e->move_type = 0;

    fullSpeed *= 1.0f;

    if(ai->followTarget.pos){
        float distSQR = distance3Squared(ENTPOS(e), ai->followTarget.pos);

        if(distSQR < SQR(50)){
            float maxSpeed = 0.4 + (fullSpeed - 0.4) * sqrt(distSQR) / 50;

            if(fullSpeed > maxSpeed)
                fullSpeed = maxSpeed;
        }

        if(ai->doNotRun){
            fullSpeed *= 0.5;
        }
    }

    // Ramp speed to be closer to desired speed.

    if(ai->curSpeed < fullSpeed){
        ai->curSpeed += e->timestep * fullSpeed / 20.0f;
        if(ai->curSpeed > fullSpeed){
            ai->curSpeed = fullSpeed;
        }
    }
    else if(ai->curSpeed > fullSpeed){
        ai->curSpeed -= e->timestep * fullSpeed / 20.0f;
        if(ai->curSpeed < fullSpeed){
            ai->curSpeed = fullSpeed;
        }
    }

    // Check if I can skip to either the destination or the next flight curve point.

    if(ABS_TIME_SINCE(ai->time.checkedForShortcut) > SEC_TO_ABS(0.5)){
        ai->time.checkedForShortcut = ABS_TIME;

        checkForShortcut = 1;

        if(ai->followTarget.type){
            float distSQR = distance3Squared(ENTPOS(e), ai->followTarget.pos);

            // If I can see the target and it's less than 150 feet away, fly straight to it.

            if(!ai->motion.fly.notOnPath){
                if(    !ai->preventShortcut &&
                    distSQR < SQR(150) &&
                    aiCanFlyToPosition(e, ai->followTarget.pos))
                {
                    //aiEntSendAddLine(posPoint(e), 0xffffffff, ai->followTarget.pos, 0xff0000ff);

                    ai->motion.fly.notOnPath = 1;
                }
            }
            else if(distSQR >= SQR(150) ||
                    !aiCanFlyToPosition(e, ai->followTarget.pos))
            {
                ai->motion.fly.notOnPath = 0;
                aiMotionSwitchToNone(e, ai, "Can't fly straight to target anymore.");
                zeroVec3(e->motion->vel);
                zeroVec3(e->motion->input.vel);
                clearNavSearch(ai->navSearch);
                return;
            }
        }

    }

    // Calculate how far to fly this turn.

    distRemaining = ai->curSpeed * e->timestep;

    if(ai->motion.fly.notOnPath)
    { 
        Vec3 diff;
        float length;

        ai->setForwardBit = 1;

        if(ai->followTarget.type){
            copyVec3(ai->followTarget.pos, ai->motion.target);
        }

        subVec3(ai->motion.target, ENTPOS(e), diff);

        length = lengthVec3(diff);

        if(distRemaining > length)
            distRemaining = length;

        if(length){
            scaleVec3(diff, distRemaining / length, diff);
        }

        if (FORWARD_MOVEMENT_ONLY)
        {
            // move ent only in the direction that I am facing
            scaleVec3(ENTMAT(e)[2], distRemaining, diff);
        }
        addVec3(ENTPOS(e), diff, pos);

        length -= distRemaining;
        totalDistMoved = distRemaining;

        if(length < CLOSE_ENOUGH_DISTANCE)
        {
            aiMotionSwitchToTransition(e, ai, "Reached flight waypoint.");
        }

    }else{
        copyVec3(ENTPOS(e), pos);

        // Check for shortcutting to a further waypoint.

        if(checkForShortcut && path->curWaypoint && path->curWaypoint->next){
            if(distance3Squared(ENTPOS(e), path->curWaypoint->next->pos) < SQR(150)){
                if(aiCanFlyToPosition(e, path->curWaypoint->next->pos)){
                    //aiEntSendAddLine(posPoint(e), 0xffffffff, path->curWaypoint->next->pos, 0xffffff00);

                    AI_LOG(    AI_LOG_WAYPOINT,
                            (e,
                            "^2SKIPPING ^0to next waypoint (^4%1.f^0,^4%1.f^0,^4%1.f^0) by FLYING.\n",
                            vecParamsXYZ(path->curWaypoint->next->pos)));

                    path->curWaypoint = path->curWaypoint->next;

                    copyVec3(path->curWaypoint->pos, ai->motion.target);
                }else{
                    AI_LOG(    AI_LOG_WAYPOINT,
                            (e,
                            "^1NOT SKIPPING ^0to next waypoint by FLYING.\n"));
                }
            }
        }

        while(distRemaining > 0){
            float moveDist;
            Vec3 moveVec;

            subVec3(ai->motion.target, pos, moveVec);
            moveDist = lengthVec3(moveVec);

            if(vecY(moveVec) < 0)
                distRemaining *= 1.5;

            if(moveDist > distRemaining){
                totalDistMoved += distRemaining;

                if (FORWARD_MOVEMENT_ONLY)
                {
                    // move ent only in the direction that I am facing
                    scaleVec3(ENTMAT(e)[2], distRemaining, moveVec);
                } else {
                    scaleVec3(moveVec, distRemaining / moveDist, moveVec);
                }

                distRemaining = 0;

                ai->setForwardBit = 1;
            }else{
                totalDistMoved += moveDist;

                distRemaining -= moveDist;

                if(    path->curWaypoint &&
                    path->curWaypoint->next &&
                    path->curWaypoint->next->connectType == NAVPATH_CONNECT_FLY)
                {
                    AI_LOG(    AI_LOG_WAYPOINT,
                            (e,
                            "Reached waypoint by flying, going to next one.\n"));

                    path->curWaypoint = path->curWaypoint->next;

                    copyVec3(path->curWaypoint->pos, ai->motion.target);
                }else{
                    aiMotionSwitchToTransition(e, ai, "Reached flight waypoint, next isn't fly connection.");

                    distRemaining = 0;
                }
            }

            addVec3(pos, moveVec, pos);
        }
    }

    copyVec3(ENTPOS(e), oldPos);

    entUpdatePosInterpolated(e, pos);
    copyVec3(pos, e->motion->last_pos);

    // If I hit other entities, check if I can actually move to where they pushed me to.

    if(checkEntColl(e, 0, NULL)){
        if(!aiCanFlyToPosition(e, oldPos)){
            if(ai->motion.fly.notOnPath){
                aiMotionSwitchToNone(e, ai, "Can't fly around entities.");
                clearNavSearch(ai->navSearch);
                return;
            }else{
                entUpdatePosInterpolated(e, oldPos);
            }
        }

        if(distance3Squared(pos, ENTPOS(e)) > SQR(totalDistMoved * 0.5)){
            // Tick move failed.

            if(ai->failedMoveCount < 10){
                AI_LOG(    AI_LOG_PATH,
                        (e,
                        "^1Failed flight tick move ^4%d^0\n",
                        ai->failedMoveCount));

                ai->failedMoveCount++;
            }else{
                // I've been stuck for long enough to worry, notify entity.

                AI_LOG(    AI_LOG_PATH,
                        (e,
                        "^1Failed final flight tick move ^4%d ^0- Notifying AI.\n",
                        ai->failedMoveCount));

                aiSendMessage(e, AI_MSG_PATH_BLOCKED);
            }
        }
    }
}

static void aiMotionWire(Entity* e, AIVars* ai){
    Vec3 toTarget;
    F32 dist;
    NavPath* path = &ai->navSearch->path;
    F32 distRemaining;

    e->move_type = 0;

    if(!path->curWaypoint){
        aiMotionSwitchToNone(e, ai, "In wire mode with no waypoint.");
        return;
    }

    if(ai->inputs.limitedSpeed)
        distRemaining = ai->inputs.limitedSpeed * e->timestep;
    else
        distRemaining = e->motion->input.surf_mods[SURFPARAM_GROUND].max_speed * e->timestep;

    if(ai->doNotRun){
        distRemaining /= 4.8;
    }

    subVec3(path->curWaypoint->pos, ENTPOS(e), toTarget);

    dist = lengthVec3(toTarget);

    if(distRemaining >= dist){
        entUpdatePosInterpolated(e, ai->motion.target);

        path->curWaypoint = path->curWaypoint->next;

        aiMotionSwitchToTransition(e, ai, "Reached wire waypoint.");
    }else{
        F32 scale = distRemaining / dist; 
        Vec3 newpos;

        if (ai->turnSpeed > 0)
        {
            scaleVec3(ENTMAT(e)[2], distRemaining, toTarget);
        } else {
            scaleVec3(toTarget, scale, toTarget);
        }

        addVec3(toTarget, ENTPOS(e), newpos);

        entUpdatePosInterpolated(e, newpos);
        copyVec3(newpos, e->motion->last_pos);

    }

    ai->setForwardBit = 1;
}

static void aiMotionEnterable(Entity* e, AIVars* ai)
{
    NavPath* path = &ai->navSearch->path;

    if(!path || !path->curWaypoint || !path->curWaypoint->door)
    {
        aiMotionSwitchToNone(e, ai, "No door found to enter.");
        return;
    }

    ai->setForwardBit = 0;

    if(!ai->doorAnim && !ai->motion.ground.waitingForDoorAnim)
    {
        DoorAnimPoint * doorAnimPoint = DoorAnimFindPoint(path->curWaypoint->door->mat[3], DoorAnimPointEnter, MAX_DOORANIM_BEACON_DIST);
        ai->doorAnim = DoorAnimInitState(doorAnimPoint,ENTPOS(e));
    }

    if(ai->doorAnim && ai->doorAnim->state != DOORANIM_DONE)
        DoorAnimCheckMove(e);
    else if(ai->motion.ground.waitingForDoorAnim && !getMoveFlags(e->seq, SEQMOVE_WALKTHROUGH))
    {
        ai->motion.ground.waitingForDoorAnim = 0;
        path->curWaypoint = path->curWaypoint->next;

        if(!path->curWaypoint && ai->followTarget.type == AI_FOLLOWTARGET_ENTERABLE)
            aiSendMessage(e, AI_MSG_FOLLOW_TARGET_REACHED);
    }
    else
    {
        bool walkThroughDoor;
        Entity* doorEnt = getTheDoorThisGeneratorSpawned(path->curWaypoint->door->mat[3]);

        if(    path->curWaypoint->door->type == DOORTYPE_FREEOPEN
            ||
            path->curWaypoint->door->type == DOORTYPE_KEY &&
            !path->curWaypoint->door->special_info)
        {
            walkThroughDoor = true;
        }else{
            walkThroughDoor = false;
        }

        if(ai->doorAnim)
        {
            if(ai->doorAnim->animation)
                free(ai->doorAnim->animation);
            free(ai->doorAnim);
            ai->doorAnim = NULL;
        }

        path->curWaypoint = path->curWaypoint->next;
        if(!walkThroughDoor && ai->disappearInDoor)
        {
            e->fledMap = 1; //Nobody killed me.  I just ran off
            if( (!e->erCreator && !erGetEnt(e->erCreator)) || e->petByScript )
            {
                SET_ENTINFO(e).kill_me_please = 1;
            }
        }
        else if(!walkThroughDoor &&
                path->curWaypoint &&
                path->curWaypoint->connectType == NAVPATH_CONNECT_ENTERABLE)
        {
            DoorAnimPoint* exitPoint = DoorAnimFindPoint(path->curWaypoint->door->mat[3], DoorAnimPointExit, MAX_DOORANIM_BEACON_DIST);


            if(exitPoint)
            {
                EntRunOutOfDoor(e, exitPoint, 0);
                ai->motion.ground.waitingForDoorAnim = 1;
            }
            else
            {
                entUpdatePosInstantaneous(e, posPoint(path->curWaypoint->door));
                path->curWaypoint = path->curWaypoint->next;
            }

            if(!path->curWaypoint && ai->followTarget.type == AI_FOLLOWTARGET_ENTERABLE)
                aiSendMessage(e, AI_MSG_FOLLOW_TARGET_REACHED);
        }

        if(doorEnt)
        {
            ENTAI(doorEnt)->door.notDisappearInDoor = 1;
            if(walkThroughDoor){
                Vec3 dir;
                subVec3(ENTPOS(doorEnt), ENTPOS(e), dir);
                aiTurnBodyByDirection(e, ai, dir);
                aiDoorSetOpen(doorEnt, 1);
            }
        }

        if(!path->curWaypoint)
            aiMotionSwitchToNone(e, ai, "Finished entering door");
    }
}

static int aiIsNearWaypointXZ(const Vec3 pos, NavPathWaypoint* wp){
    return distance3SquaredXZ(pos, wp->pos) < SQR(2);
}

static int aiIsNearWaypointY(const Vec3 pos, NavPathWaypoint* wp){
    if(wp->connectType == NAVPATH_CONNECT_GROUND)
        return vecY(pos) > vecY(wp->pos) - 0.5;
    else
        return distanceY(pos, wp->pos) < 3;
}

static void aiMotionNone(Entity* e, AIVars* ai){
    NavSearch* navSearch = ai->navSearch;
    NavPath* path = &navSearch->path;

    e->motion->jumping = 0;
    ai->setForwardBit = 0;

    if(ai->followTarget.type){
        if(path->curWaypoint){
            // I'm on a path.

            NavPathWaypoint* targetWaypoint = path->curWaypoint;
            NavPathConnectType connectType;

            // Expire all the beacons that I've already reached (if they're stacked on each other).

            while(1){
                if(targetWaypoint->connectType == NAVPATH_CONNECT_ENTERABLE ||
                    (targetWaypoint->connectType == NAVPATH_CONNECT_WIRE &&
                                        ai->teamMemberInfo.team && !ai->teamMemberInfo.team->patrolTypeWireComplex))
                    break;

                if(!aiIsNearWaypointXZ(ENTPOS(e), targetWaypoint)){
                    // Not near the waypoint XZ pos.
                    break;
                }
                else if(!aiIsNearWaypointY(ENTPOS(e), targetWaypoint)){
                    // Not near the waypoint Y pos.
                    break;
                }
                else{
                    AI_LOG(    AI_LOG_WAYPOINT,
                            (e,
                            "Reached waypoint: (^4%1.f^0,^4%1.f^0,^4%1.f^0).\n",
                            vecParamsXYZ(targetWaypoint->pos)));
                }

                if(!targetWaypoint->next){
                    AI_LOG(    AI_LOG_PATH,
                            (e,
                            "Cursor past end of path.  Clearing path.\n"));

                    clearNavSearch(navSearch);

                    if(ai->followTarget.type){
                        if(ai->isFlying || ai->alwaysFly){
                            if(    !ai->preventShortcut &&
                                aiCanFlyToPosition(e, ai->followTarget.pos))
                            {
                                //aiEntSendAddLine(posPoint(e), 0xffffffff, ai->followTarget.pos, 0xff00ffff);

                                if(aiMotionSwitchToFly(e, ai, "Hit end of path, flying to target.")){
                                    ai->motion.fly.notOnPath = 1;
                                }
                            }
                        }
                        else if(!ai->shortcutWasTaken
                                ||
                                !ai->preventShortcut &&
                                aiCanWalkBetweenPointsQuickCheck(ENTPOS(e), 4, ai->followTarget.pos, 4))
                        {
                            aiMotionSwitchToGround(e, ai, "Shortcutting to follow target.");

                            ai->motion.ground.notOnPath = 1;
                        }

                        copyVec3(ENTPOS(e), ai->motion.source);
                        copyVec3(ai->followTarget.pos, ai->motion.target);
                    }

                    if(!ai->motion.type){
                        aiSendMessage(e, AI_MSG_PATH_FINISHED);
                    }

                    return;
                }
                else if(targetWaypoint->connectionToMe){
                    BeaconConnection* conn = targetWaypoint->connectionToMe;

                    if(targetWaypoint->beacon){
                        targetWaypoint->beacon->pathsBlockedToMe = 0;
                    }

                    if(conn){
                        // Reset the paths blocked to this beacon if a connection was successful.

                        conn->timeWhenIWasBad = 0;
                    }
                }

                AI_LOG(AI_LOG_WAYPOINT, (e, "Reached waypoint, going to next one.\n"));

                targetWaypoint = path->curWaypoint = path->curWaypoint->next;
            }

            connectType = targetWaypoint->connectType;

            // Now that I've found the next beacon to target, set motion towards it.

            if(connectType == NAVPATH_CONNECT_WIRE){
                copyVec3(ENTPOS(e), ai->motion.source);
                copyVec3(targetWaypoint->pos, ai->motion.target);

                aiMotionSwitchToWire(e, ai, "Next connection is wire.");
            }
            else if (connectType == NAVPATH_CONNECT_ENTERABLE)
            {
                aiMotionSwitchToEnterable(e, ai, "Next connection is enterable");
            }
            else if(!targetWaypoint->prev){
                // I'm at the start of the path.

                copyVec3(ENTPOS(e), ai->motion.source);
                copyVec3(targetWaypoint->pos, ai->motion.target);

                if(connectType == NAVPATH_CONNECT_FLY || ai->alwaysFly){
                    if(ai->canFly){
                        aiSetFlying(e, ai, 1);

                        if(aiMotionSwitchToFly(e, ai, "Going to first waypoint in path.")){
                            ai->motion.fly.notOnPath = 0;
                        }
                    }else{
                        aiMotionSwitchToNone(e, ai, "Path said to fly, but I can't.");
                    }
                }
                else if(connectType == NAVPATH_CONNECT_GROUND &&
                        (!targetWaypoint->beacon || targetWaypoint->beacon->pathsBlockedToMe < 3))
                {
                    aiMotionSwitchToGround(e, ai, "Using ground motion to first waypoint.");
                }
                else{
                    float y_diff = vecY(ai->motion.target) - vecY(ai->motion.source);

                    // Jump to first beacon if it has been blocked a lot.

                    ai->motion.minHeight = max(0, y_diff) + 3 + distance3XZ(ai->motion.source, ai->motion.target) / 6;
                    ai->motion.maxHeight = ai->motion.minHeight;

                    aiMotionOffsetJumpTarget(e, ai, 0);

                    aiMotionSwitchToJump(e, ai, "First waypoint requires jump.");

                    AI_LOG(    AI_LOG_WAYPOINT,
                            (e,
                            "Jumping to waypoint: (^4%1.f^0,^4%1.f^0,^4%1.f^0).\n",
                            vecParamsXYZ(targetWaypoint->pos)));

                    // Update the beacon stats.

                    if(    targetWaypoint->beacon &&
                        targetWaypoint->beacon->pathsBlockedToMe > 0)
                    {
                        targetWaypoint->beacon->pathsBlockedToMe--;
                    }
                }
            }else{
                // I'm in the middle of the path.

                NavPathWaypoint* sourceWaypoint = path->curWaypoint->prev;
                BeaconConnection* conn = targetWaypoint->connectionToMe;
                int isWalkConnection = targetWaypoint->connectType == NAVPATH_CONNECT_GROUND;

                copyVec3(ENTPOS(e), ai->motion.source);
                copyVec3(targetWaypoint->pos, ai->motion.target);

                if(conn && conn->minHeight > 0){
                    ai->motion.minHeight = vecY(sourceWaypoint->pos) + conn->minHeight - vecY(ai->motion.source);
                    ai->motion.maxHeight = vecY(sourceWaypoint->pos) + conn->maxHeight - vecY(ai->motion.source);
                }else{
                    ai->motion.minHeight = ai->motion.maxHeight = 0;
                }

                // Jump to next beacon if it has been blocked a lot, or blocked recently,
                //   or if this is a jump connection.

                if(    !isWalkConnection ||
                    (    distance3Squared(ai->motion.source, ai->motion.target) < SQR(15) &&
                        (    /*targetWaypoint->beacon &&
                            targetWaypoint->beacon->pathsBlockedToMe >= 3
                            ||*/
                            conn &&
                            ABS_TIME_SINCE(conn->timeWhenIWasBad) < SEC_TO_ABS(5))) || ai->alwaysFly)
                {
                    if(ai->motion.maxHeight == 0){
                        ai->motion.minHeight += 3 + distance3XZ(ai->motion.source, ai->motion.target) / 6;
                        ai->motion.maxHeight = ai->motion.minHeight;
                    }

                    if(ai->canFly){
                        AI_LOG(    AI_LOG_WAYPOINT,
                                (e,
                                "Flying to waypoint: (^4%1.f^0,^4%1.f^0,^4%1.f^0).\n",
                                vecParamsXYZ(targetWaypoint->pos)));

                        aiSetFlying(e, ai, 1);
                        
                        if(aiMotionSwitchToFly(e, ai, isWalkConnection ? "Connection is bad." : "Is flying connection.")){
                            ai->motion.fly.notOnPath = 0;
                            zeroVec3(e->motion->vel);
                        }
                    }else{
                        AI_LOG(    AI_LOG_WAYPOINT,
                                (e,
                                "Jumping to waypoint: (^4%1.f^0,^4%1.f^0,^4%1.f^0).\n",
                                vecParamsXYZ(targetWaypoint->pos)));

                        aiMotionOffsetJumpTarget(e, ai, 0);

                        aiMotionSwitchToJump(e, ai, isWalkConnection ? "Connection is bad." : "Is jump connection.");

                        //assert(vecY(ai->motion.source) + ai->motion.minHeight >= vecY(ai->motion.target));
                    }

                    // Update the beacon stats.

                    //if(targetBeacon->pathsBlockedToMe > 0)
                    //    targetBeacon->pathsBlockedToMe--;
                }else{
                    //vecY(ai->motion.target) -= beaconGetFloorDistance(targetBeacon);

                    aiMotionSwitchToGround(e, ai, "Using ground motion to next waypoint.");

                    AI_LOG(    AI_LOG_WAYPOINT,
                            (e,
                            "Walking to waypoint: (^4%1.f^0,^4%1.f^0,^4%1.f^0).\n",
                            vecParamsXYZ(targetWaypoint->pos)));
                }
            }

            if(ai->motion.type == AI_MOTIONTYPE_GROUND){
                ai->motion.ground.expectedTickMoveDist = 0;
            }

            ai->motion.jump.time = 0;
        }else{
            if(navSearch->path.head){
                if(ai->teamMemberInfo.team && ai->teamMemberInfo.team->patrolTypeWireComplex)
                {
                    navSearch->path.curWaypoint = navSearch->path.head;
                }
                else
                {
                    AI_LOG(AI_LOG_PATH, (e, "Path empty or past end of path.  Clearing path.\n"));

                    clearNavSearch(navSearch);
                }
            }

            if(ABS_TIME_SINCE(ai->time.checkedForShortcut) > SEC_TO_ABS(1)){
                float distanceSQR = distance3SquaredXZ(ENTPOS(e), ai->followTarget.pos);

                ai->time.checkedForShortcut = ABS_TIME;

                if(    distanceSQR < SQR(150) &&
                    distanceSQR > SQR(ai->followTarget.standoffDistance) &&
                    !ai->preventShortcut)
                {
                    if(ai->canFly || ai->alwaysFly){
                        if(aiCanFlyToPosition(e, ai->followTarget.pos)){
                            // Fly straight at my target.

                            //aiEntSendAddLine(posPoint(e), 0xffffffff, ai->followTarget.pos, 0xffff00ff);

                            copyVec3(ENTPOS(e), ai->motion.source);
                            copyVec3(ai->followTarget.pos, ai->motion.target);
                            aiSetFlying(e, ai, 1);
                            
                            if(aiMotionSwitchToFly(e, ai, "Has straight shot to target.")){
                                ai->motion.fly.notOnPath = 1;
                                zeroVec3(e->motion->vel);
                            }
                        }else{
                            aiMotionSwitchToNone(e, ai, "No waypoint, and can't fly straight to target.");
                        }
                    }
                    else if(ai->motion.type == AI_MOTIONTYPE_TRANSITION ||
                            aiCanWalkBetweenPointsQuickCheck(ENTPOS(e), 4, ai->followTarget.pos, 4))
                    {
                        // I can walk straight to my follow target.

                        copyVec3(ENTPOS(e), ai->motion.source);
                        copyVec3(ai->followTarget.pos, ai->motion.target);

                        aiMotionSwitchToGround(e, ai, "No waypoint, but I'm gonna shortcut to target.");

                        ai->motion.ground.expectedTickMoveDist = 0;
                        ai->motion.ground.notOnPath = 1;
                    }else{
                        aiMotionSwitchToNone(e, ai, "No waypoint, can't fly or walk to target.");
                    }
                }else{
                    aiMotionSwitchToNone(e, ai, "No waypoint, and not in range to shortcut.");

                    //copyVec3(posPoint(e), ai->motion.source);
                    //copyVec3(ai->followTarget.pos, ai->motion.target);
                    //
                    //ai->motion.type = AI_MOTIONTYPE_GROUND;
                }
            }
        }
    }else{
        // Slow down.

        if(ai->curSpeed > 0){
            ai->curSpeed -= 0.1f;

            if(ai->curSpeed < 0){
                ai->curSpeed = 0;
            }
        }

        aiMotionSwitchToNone(e, ai, "No follow target.");
    }
}

void aiFollowTarget(Entity* e, AIVars* ai){
    // Detect when I have been knock-backed.

    if(ai->isFlying && lengthVec3Squared(e->motion->vel) > SQR(0.1)){
        scaleVec3(e->motion->vel, -1, e->motion->input.vel);
        e->move_type = MOVETYPE_WALK;
        //e->motion->falling = 1;
        ai->forceGetUp = 1;
        return;
    }
    else if(ai->forceGetUp){
        ai->forceGetUp = 0;
        SETB(e->seq->state, STATE_APEX);
        SETB(e->seq->state, STATE_AIR);
    }

    // Check if I can't move.

    if(ai->isFlying){
        SETB(e->seq->state, STATE_FLY);
    }

    if(    ai->motion.type != AI_MOTIONTYPE_JUMP &&
        (    !entCanSetInpVel(e)
            ||
            e->pchar && e->pchar->ppowCurrent))
    {
        if(    ai->motion.type &&
            ai->motion.type != AI_MOTIONTYPE_FLY &&
            ai->motion.type != AI_MOTIONTYPE_ENTERABLE)
        {
            aiMotionSwitchToNone(e, ai, "Lost movement control.");

            if(lengthVec3Squared(e->motion->vel) > SQR(1.0)){
                clearNavSearch(ai->navSearch);
            }
        }

        return;
    }

    switch(ai->motion.type){
        case AI_MOTIONTYPE_NONE:
        case AI_MOTIONTYPE_STARTING:
        case AI_MOTIONTYPE_TRANSITION:
            aiMotionNone(e, ai);
            break;
    }

    // Do that motion now.

    switch(ai->motion.type){
        case AI_MOTIONTYPE_GROUND:{
            PERFINFO_AUTO_START("Ground", 1);
                aiSetFlying(e, ai, 0);

                aiMotionGround(e, ai);

                //AI_LOG(    AI_LOG_PATH,
                //        (e,
                //        "expectedMoveDist = ^4%1.3f^0, "
                //        "curSpeed = ^4%1.3f^0/^4%1.3f^0, "
                //        "vel = (^4%1.1f^0,^4%1.1f^0,^4%1.1f^0), "
                //        "target = (^4%1.1f^0,^4%1.1f^0,^4%1.1f^0)\n",
                //        ai->motion.ground.expectedTickMoveDist,
                //        ai->curSpeed,
                //        e->motion->max_speed_scale,
                //        vecParamsXYZ(e->motion->inp_vel),
                //        vecParamsXYZ(ai->motion.ground.lastTickDestPos)));

            PERFINFO_AUTO_STOP();

            break;
        }

        case AI_MOTIONTYPE_JUMP:{
            PERFINFO_AUTO_START("Jump", 1);
                aiSetFlying(e, ai, 0);
                aiMotionJump(e, ai);
            PERFINFO_AUTO_STOP();

            break;
        }

        case AI_MOTIONTYPE_FLY:{
            PERFINFO_AUTO_START("Fly", 1);
                aiSetFlying(e, ai, 1);
                aiMotionFly(e, ai);
            PERFINFO_AUTO_STOP();

            break;
        }

        case AI_MOTIONTYPE_WIRE:{
            PERFINFO_AUTO_START("Wire", 1);
                aiMotionWire(e, ai);
            PERFINFO_AUTO_STOP();

            break;
        }

        case AI_MOTIONTYPE_ENTERABLE:
            PERFINFO_AUTO_START("Enterable", 1);
                aiMotionEnterable(e, ai);
            PERFINFO_AUTO_STOP();
            break;

        default:{
            ai->setForwardBit = 0;

            break;
        }
    }

    // Keep setting forward bit until the timer expires.

    if(ai->setForwardBit){
        SETB(e->seq->state, STATE_FORWARD);

        if(ai->doNotRun && e->motion->surf_normal[1] > 0.7){
            SETB(e->seq->state, STATE_WALK);

            if(!ai->isFlying && e->move_type){
                SETB(e->seq->state, STATE_IDLE);
            }
        }

        if(    e->motion->falling &&
            e->motion->vel[1] < -0.6)
        {
            SETB(e->seq->state, STATE_AIR);
            SETB(e->seq->state, STATE_BIGFALL);
        }
    }
}

void aiFollowPathToEnt(Entity *e, AIVars *ai, Entity *target)
{
    // because this might get called from aiCritterThink before everything is set up, if there is no follow target
    // and no target given, compute distance to self to avoid everything breaking
    float distSQR;

    if(!target)
        return;

    distSQR = distance3Squared(ENTPOS(e), ENTPOS(target));

    if(distSQR > SQR(AI_FOLLOW_STANDOFF_DIST - 1))
    {
        ai->time.foundPath = ABS_TIME;
        ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : AI_FOLLOW_STANDOFF_DIST;

        aiDiscardFollowTarget(e, "Began following leader along patrol route.", false);
        aiSetFollowTargetEntity(e, ai, target, AI_FOLLOWTARGETREASON_LEADER);
        aiGetProxEntStatus(ai->proxEntStatusTable, target, 1);

        if(ai->teamMemberInfo.team->patrolTypeWire)
        {
            aiFollowPathWire(e, ai, ENTPOS(target), AI_FOLLOWTARGETREASON_LEADER);
        }
        else
        {
            aiConstructPathToTarget(e);
            // use the leader's doNotRun variable because mine got reset with the aiDiscardFollowTarget call
            aiStartPath(e, 0, ENTAI(target)->doNotRun);
        }
    }
}

void aiFollowPathWire(Entity* e, AIVars* ai, const Vec3 target_pos, AIFollowTargetReason reason)
{
    NavPathWaypoint* wp;
    Vec3 direction;

    clearNavSearch(ai->navSearch);

    wp = createNavPathWaypoint();
    wp->connectType = NAVPATH_CONNECT_WIRE;
    copyVec3(target_pos, wp->pos);
    navPathAddTail(&ai->navSearch->path, wp);

    ai->navSearch->path.curWaypoint = ai->navSearch->path.head;

    aiSetFollowTargetPosition(e, ai, wp->pos, reason);

    subVec3(target_pos, ENTPOS(e), direction);

    aiTurnBodyByDirection(e, ai, direction);
    aiStartPath(e, 1, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);
}

#define MAX_CURVE_POINTS    100

void CreateComplexWire(Entity* e, AIVars* ai, Vec3* points, U32 numpoints, F32 resolution, F32 curve_dist)
{
    U32 i, j, numcurvepoints, total_waypoints;
    Vec3 temp, pyr, restart_point;
    Position start, end;
    float dist;
    NavPathWaypoint* wp;
    static BeaconCurvePoint curvepts[MAX_CURVE_POINTS];

    clearNavSearch(ai->navSearch);

    wp = createNavPathWaypoint();
    wp->connectType = NAVPATH_CONNECT_WIRE;
//    copyVec3(e->mat[3], wp->pos);
    subVec3(points[0], points[numpoints-1], temp);
    dist = lengthVec3(temp);
    if(dist > curve_dist)
    {
        normalVec3(temp);
        scaleVec3(temp, -1 * curve_dist, temp);
        addVec3(points[0], temp, restart_point);
        copyVec3(restart_point, wp->pos);
        navPathAddTail(&ai->navSearch->path, wp);
        total_waypoints = 1;
    }
    else
    {
        // something
    }
    ai->navSearch->path.curWaypoint = ai->navSearch->path.head;

/*
    navPathAddTail(&ai->navSearch->path, wp);
    total_waypoints = 1;
*/

    for(i = 0; i < numpoints; i++)
    {
        //find distance to first point
        subVec3(points[i], ai->navSearch->path.tail->pos, temp);
        dist = lengthVec3(temp);

        getVec3YP(temp, &pyr[1], &pyr[0]);
        pyr[2] = 0;
        createMat3YPR(start.mat, pyr);

        //add point - curve_dist to path
        if(dist > curve_dist)
        {
            wp = createNavPathWaypoint();
            wp->connectType = NAVPATH_CONNECT_WIRE;

            normalVec3(temp);
            scaleVec3(temp, -1 * curve_dist, temp);
            addVec3(points[i], temp, temp);
            copyVec3(temp, wp->pos);
            navPathAddTail(&ai->navSearch->path, wp);
            total_waypoints++;
        }

        // set start.mat[3] to either the starting point or curve_dist away from next point
        copyVec3(ai->navSearch->path.tail->pos, start.mat[3]);

        // find point curve_dist after way point, towards the way point after
        subVec3(points[(i+1)%numpoints], points[i], temp);
        dist = lengthVec3(temp);

        getVec3YP(temp, &pyr[1], &pyr[0]);
        pyr[2] = 0;
        createMat3YPR(end.mat, pyr);

        normalVec3(temp);
//        if(dist > (2 * curve_dist))
        if(dist > curve_dist)
        {
            scaleVec3(temp, curve_dist, temp);
        }
        else
        {
            scaleVec3(temp, dist / 2, temp);
//            copyVec3(points[(i+1)%numpoints], end.mat[3]); // TODO: this could be nicer
        }
        addVec3(points[i], temp, end.mat[3]);

        subVec3(start.mat[3], end.mat[3], temp);
        dist = lengthVec3(temp);

        //NOTE: this resolution thing is really arbitrary, especially because it uses the
        // straight line distance, but it's good enough for now

        numcurvepoints = (dist * resolution) > MAX_CURVE_POINTS ? MAX_CURVE_POINTS : (int)(dist * resolution);

        //create curve from curve_dist before way point to curve_dist after way point
        if(createCurve(&start, &end, curvepts, numcurvepoints, 1))
        {
            //add all points generated to path
            for(j = 0; j < numcurvepoints; j++)
            {
                wp = createNavPathWaypoint();
                wp->connectType = NAVPATH_CONNECT_WIRE;
                copyVec3(curvepts[j].pos, wp->pos);
                navPathAddTail(&ai->navSearch->path, wp);
                total_waypoints++;
            }
        }
        else
        {
            wp = createNavPathWaypoint();
            wp->connectType = NAVPATH_CONNECT_WIRE;
            copyVec3(end.mat[3], wp->pos);
            navPathAddTail(&ai->navSearch->path, wp);
            total_waypoints++;
        }

    }

    wp = createNavPathWaypoint();
    wp->connectType = NAVPATH_CONNECT_WIRE;
    copyVec3(restart_point, wp->pos);
    navPathAddTail(&ai->navSearch->path, wp);
    total_waypoints++;

    AI_LOG(AI_LOG_PATH, (e, "Total waypoints created for complex wire path: %d\n", total_waypoints));
}