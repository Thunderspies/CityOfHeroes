
#include "entaiprivate.h"
#include "beacon/beaconPrivate.h"
#include "entity/motion.h"
#include "seq/seqstate.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "cmdparse/cmdcommon.h"
#include "cmdparse/cmdserver.h"

static int beaconScoreCar(Entity* e, Beacon* b){
    AIVars* ai = ENTAI(e);
    Vec3 diff;
    float yaw;
    float myYaw = ai->pyr[1];
    int corner;
    
    if(ai->car.beaconTypeNumber && ai->car.beaconTypeNumber != b->trafficTypeNumber){
        return -500;
    }
    
    subVec3(posPoint(b), ENTPOS(e), diff);
    
    yaw = getVec3Yaw(diff);
    
    yaw = fabs(DEG(subAngle(myYaw, yaw)));
    
    corner = DEG(fabs(subAngle(myYaw, b->pyr[1]))) > 20;
    
    if(yaw < 5 || corner){
        if(corner){
            ai->time.foundPath = 0;
        }    
        
        return 180 + getCappedRandom(20);
    }
    
    if(ABS_TIME_SINCE(ai->time.foundPath) > 120 * 100 && rand() & 1){
        ai->time.foundPath = ABS_TIME;

        return INT_MAX;
    }
    
    return getCappedRandom(10);
}

//**************************************************************************************
// Car AI.
//**************************************************************************************

void aiCar(Entity* e, AIVars* ai){
    int setDir = 0;
    float moveReallyFast = 0;
    Vec3 diffVec;
    float distToMove = e->motion->input.surf_mods[SURFPARAM_GROUND].max_speed * e->timestep;
    int moveLoops = 0;

    static int highestMoveLoops = 0;
    static int totalMoveOverflows = 0;
    static int totalMoveChecks = 0;
    static int bucketMoveChecks = 0;
    static int bucketMoveLoops = 0;

    // Keep moving until I've moved the amount I'm supposed to move this frame.

    while(moveLoops < 50 && distToMove > 0){
        float distToTarget;
        Beacon* lastTargetBeacon = ai->followTarget.type ? ai->followTarget.beacon : NULL;
        Beacon* targetBeacon = lastTargetBeacon;
        Vec3 targetPos;
        Vec3 sourcePos;

        moveLoops++;

        // If the last beacon was a curve-head, then check current position in curve.
        
        if(ai->car.curveArray){
            int point = ai->car.curvePoint;
        
            if(point < 1 || point >= BEACON_CURVE_POINT_COUNT){
                point = ai->car.curvePoint = 1;
            }
            
            // Point at the current curve destination.
            
            copyVec3(ai->car.curveArray[point].pos, targetPos);
            
            targetPos[1] -= 2;
            
            subVec3(targetPos, ENTPOS(e), diffVec);
            
            distToTarget = lengthVec3(diffVec);
            
            if(distToMove < distToTarget){
                // I can't make it to the next curve point on this tick.
                Vec3 newpos;
                
                scaleVec3(diffVec, distToMove / distToTarget, diffVec);

                addVec3(ENTPOS(e), diffVec, newpos);

                entUpdatePosInterpolated(e, newpos);
                
                distToTarget -= distToMove;
                
                distToMove = 0;
                
                //AI_LOG(AI_LOG_CAR, (e, "^4%1.2f ^0from (^4%1.2f^0, ^4%1.2f^0, ^4%1.2f^0).\n"));
            }else{
                // I can make it to the next point, so just go there.
                
                distToMove -= distToTarget;
                
                distToTarget = 0;
                
                entUpdatePosInterpolated(e, targetPos);
                
                if(++ai->car.curvePoint == BEACON_CURVE_POINT_COUNT){
                    if(targetBeacon->killerBeacon){
                        SET_ENTINFO(e).kill_me_please = 1;

                        return;
                    }

                    targetBeacon = NULL;
                }
            }
        }
        else if(targetBeacon){
            // I have a target but no source, or it's not a curve, so just move towards it.
            
            copyVec3(posPoint(targetBeacon), targetPos);
            
            targetPos[1] -= 2;
            
            subVec3(targetPos, ENTPOS(e), diffVec);
            
            distToTarget = lengthVec3(diffVec);
            
            if(distToMove < distToTarget){
                Vec3 newpos;

                scaleVec3(diffVec, distToMove / distToTarget, diffVec);
                
                addVec3(ENTPOS(e), diffVec, newpos);

                entUpdatePosInterpolated(e, newpos);
                
                distToTarget -= distToMove;
                
                distToMove = 0;
            }else{
                distToMove -= distToTarget;
                
                distToTarget = 0;
                
                entUpdatePosInterpolated(e, targetPos);
                
                if(targetBeacon->killerBeacon){
                    SET_ENTINFO(e).kill_me_please = 1;

                    return;
                }

                targetBeacon = NULL;
            }
        }
        
        // If the target was reached, then get a new target.
        
        if(!targetBeacon){
            Vec3 diffVec;
            Vec3 newPYR;
            
            // Find a new target and get the new source and target.
            
            getMat3YPR(ENTMAT(e), ai->pyr);
            
            targetBeacon = aiFindRandomNearbyBeacon(e, lastTargetBeacon, &trafficBeaconArray, beaconScoreCar);

            if(targetBeacon){
                aiSetFollowTargetBeacon(e, ai, targetBeacon, 0);
            }else{
                aiDiscardFollowTarget(e, "Can't find nearby beacon.", false);

                if (!server_state.noKillCars) {
                    SET_ENTINFO(e).kill_me_please = 1;
                }

                return;
            }

            // Get the target pos, 2 feet below the beacon.
            
            copyVec3(posPoint(targetBeacon), targetPos);
            
            targetPos[1] -= 2;
            
            // First destination on the curve is the SECOND point, because we start at the first point.
            
            ai->car.curveArray = NULL;
            ai->car.curvePoint = 1;

            // Get the index of the curve from source beacon to target beacon.
            
            if(lastTargetBeacon){
                int i;
                
                copyVec3(posPoint(lastTargetBeacon), sourcePos);
                sourcePos[1] -= 2;

                for(i = 0; i < lastTargetBeacon->gbConns.size; i++){
                    BeaconConnection* conn = lastTargetBeacon->gbConns.storage[i];
                    
                    if(conn->destBeacon == targetBeacon){
                        ai->car.curveArray = lastTargetBeacon->curveArray.storage[i];
                        break;
                    }
                }
            }
            
            if(1 || !ai->car.curveArray){
                Mat3 newmat;
                subVec3(targetPos, ENTPOS(e), diffVec);
                
                getVec3YP(diffVec, &newPYR[1], &ai->car.pitch);

                // Turn the yaw by 180 degrees for some unknown reason.
                
                //newPYR[1] = addAngle(newPYR[1], RAD(180));
                
                // Get the pitch from the movement direction, and then make it negative for some reason.
                
                newPYR[0] = ai->car.pitch;
                
                newPYR[2] = 0;
                
                // Put the pyr back into the Mat4.
            
                createMat3YPR(newmat, newPYR);
                entSetMat3(e, newmat);
            }
            
            if(ai->car.curveArray){
                ai->car.distanceFromSourceToTarget = distance3(ai->car.curveArray[0].pos, ai->car.curveArray[1].pos);
            }
            else if(lastTargetBeacon){
                ai->car.distanceFromSourceToTarget = distance3(sourcePos, targetPos);
            }
            else{
                ai->car.distanceFromSourceToTarget = distance3(ENTPOS(e), targetPos);
            }
        }
    }

    // High water mark shouldn't count if we clamp it, we only want to know
    // about 'normal' boundaries.
    if(moveLoops >= 50)
    {
        if(!totalMoveOverflows)
        {
            Errorf("Car got stuck at %.2f, %.2f, %.2f!", ENTPOS(e)[0], ENTPOS(e)[1], ENTPOS(e)[2]);
        }

        totalMoveOverflows++;
    }
    else
    {
        if(moveLoops > highestMoveLoops)
            highestMoveLoops = moveLoops;
    }

    // Accrue the moves to the total so we can average it.
    bucketMoveLoops += moveLoops;
    bucketMoveChecks++;
    totalMoveChecks++;

    // Collapse down the average so we don't overflow the move loops counter.
    if(bucketMoveChecks >= 100000)
    {
        bucketMoveLoops = bucketMoveLoops / bucketMoveChecks;
        bucketMoveChecks = 1;
    }

    // Set the PYR.
    
    if(ai->followTarget.beacon){
        float curDist;
        Vec3 curPYR;

        if(ai->car.curveArray){
            BeaconCurvePoint* dstPoint = ai->car.curveArray + ai->car.curvePoint;
            BeaconCurvePoint* srcPoint = ai->car.curveArray + ai->car.curvePoint - 1;
            float pyrDiff;
            Mat3 newmat;

            // How far away are we from the target position?
            //    Get distance to the source.
            
            curDist = ai->car.distanceFromSourceToTarget - distance3(dstPoint->pos, ENTPOS(e));
            
            if(curDist < 0)
                curDist = 0;

            // Multiply the pyr components by the ratio of how much turning has been done so far.
            
            pyrDiff = subAngle(dstPoint->pyr[1], srcPoint->pyr[1]);
    
            if(ai->car.distanceFromSourceToTarget){
                pyrDiff *= curDist / ai->car.distanceFromSourceToTarget;
            }

            curPYR[1] = addAngle(srcPoint->pyr[1], pyrDiff);

            // Get the pitch from the movement direction, and then make it negative for some reason.
            
            curPYR[0] = ai->car.pitch;
            
            curPYR[2] = 0;
            
            // Put the pyr back into the Mat4.
        
            createMat3YPR(newmat, curPYR);
            entSetMat3(e, newmat);
        }
    
        SETB(e->seq->state, STATE_FORWARD);
    }
}


