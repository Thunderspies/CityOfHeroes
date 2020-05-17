
#include <utilitieslib/stdtypes.h>
#include "entai.h"
#include "entaiLog.h"
#include "entaiPrivate.h"

#include "beacon/beaconConnection.h"
#include "beacon/beaconPath.h"
#include "beacon/beaconPrivate.h"
#include "cmdparse/cmdcommon.h"
#include "cmdparse/cmdserver.h"
#include "dbcomm/dbcomm.h"
#include "dbcomm/dbdoor.h"
#include "entity/entity.h"
#include "gridcoll/gridcoll.h"
#include "group/group.h"
#include "group/grouputil.h"
#include "entity/motion.h"
#include <utilitieslib/utils/timing.h>

#define MIL (1000000)

S64 aiPathLogCycleRanges[] = { 1 * MIL, 5 * MIL, 10 * MIL, 50 * MIL, 100 * MIL, 1000 * MIL };
AIPathLogEntrySet* aiPathLogSets[ARRAY_SIZE(aiPathLogCycleRanges) + 1];

AIPathLogEntry* aiPathLogAddEntry(S64 cycles){
    AIPathLogEntry* entry;
    int i;
    
    for(i = 0; i < ARRAY_SIZE(aiPathLogCycleRanges); i++){
        if(cycles <= aiPathLogCycleRanges[i]){
            break;
        }
    }
    
    if(!aiPathLogSets[i]){
        aiPathLogSets[i] = calloc(sizeof(aiPathLogSets[i][0]), 1);
        assert(aiPathLogSets[i]);
    }
    
    if(++aiPathLogSets[i]->cur >= ARRAY_SIZE(aiPathLogSets[i]->entries)){
        aiPathLogSets[i]->cur = 0;
    }
    
    entry = aiPathLogSets[i]->entries + aiPathLogSets[i]->cur;
    entry->cpuCycles = cycles;
    
    return entry;
}

int aiPathLogGetSetCount(){
    return ARRAY_SIZE(aiPathLogSets);
}

int aiCanWalkBetweenPointsQuickCheck(const Vec3 src, float srcHeight, const Vec3 dst, float dstHeight){
    CollInfo coll;
    Vec3 srcPos;
    Vec3 dstPos;
    Vec3 los;
    float d;
    float i;
    float curY;
    int success = 1;

    PERFINFO_AUTO_START("quickWalk", 1);

    copyVec3(src, srcPos);
    copyVec3(dst, dstPos);

    srcPos[1] += srcHeight;
    dstPos[1] += dstHeight;

    subVec3(dstPos, srcPos, los);

//    aiEntSendAddLine(srcPos, 0xffffffff, dstPos, 0xffff0000);

    if(collGrid(NULL, srcPos, dstPos, &coll, 0, COLL_NOTSELECTABLE | COLL_HITANY | COLL_BOTHSIDES)){
        PERFINFO_AUTO_STOP();
        return 0;
    }

    d = lengthVec3(los);

    for(i = 0; i <= d; i = (i < d) ? min(i + 3.0, d) : (d + 1)){
        Vec3 curPos;
        Vec3 groundPos;
        float scale = i / d;

        // Scale the line-of-sight by the current increment.

        scaleVec3(los, scale, curPos);

        if(ABS(vecY(curPos)) > 1.6)
        {
            success = 0;
            break;
        }

        // Add it to the source position.

        addVec3(curPos, srcPos, curPos);

        // Create the lowest allowable ground position.

        copyVec3(curPos, groundPos);

        if(i == 0){
            groundPos[1] -= 20;

            if(!collGrid(NULL, curPos, groundPos, &coll, 0, COLL_NOTSELECTABLE | COLL_DISTFROMSTART | COLL_BOTHSIDES)){
                success = 0;
                break;
            }

            curY = coll.mat[3][1];
        }else{
            float y;
            float yDiff;

            groundPos[1] = curY - 20;

            if(!collGrid(NULL, curPos, groundPos, &coll, 0, COLL_NOTSELECTABLE | COLL_DISTFROMSTART | COLL_BOTHSIDES)){
                success = 0;
                break;
            }

            y = coll.mat[3][1];

            yDiff = y - curY;

            if(ABS(yDiff) > 1.6){
                success = 0;
                break;
            }

            curY = y;
        }
    }

    PERFINFO_AUTO_STOP();

    return success;
}

void aiConstructSimplePath(Entity* e, BeaconScoreFunction func){
    AIVars* ai = ENTAI(e);

    if(!ai->navSearch)
        return;

    AI_LOG(    AI_LOG_PATH,
            (e,
            "Finding simple path: "));

    PERFINFO_AUTO_START("beaconPathFindSimpleStart", 1);
        beaconPathFindSimpleStart(ai->navSearch, (Position*)ENTMAT(e), func);
    PERFINFO_AUTO_STOP();

    if(ai->navSearch->path.head){
        NavPath* path = &ai->navSearch->path;
        NavPathWaypoint* waypoint = path->tail;

        AI_LOG(    AI_LOG_PATH,
                (e,
                "^2SUCCESS^0: ^4%d ^0nodes\n",
                ai->navSearch->path.waypointCount));

        AI_LOG(    AI_LOG_PATH,
                (e,
                "  Me: ( ^4%1.2f^0, ^4%1.2f^0, ^4%1.2f ^0)\n"
                "  Source Node: ( ^4%1.2f^0, ^4%1.2f^0, ^4%1.2f ^0)\n",
                ENTPOSPARAMS(e),
                posParamsXYZ(waypoint->beacon)));

        aiSetFollowTargetPosition(e, ai, posPoint(waypoint->beacon), 0);
    }else{
        AI_LOG(    AI_LOG_PATH,
                (e,
                "^1FAILED^0.\n"));

        aiDiscardFollowTarget(e, "Couldn't construct simple path.", false);
    }
}

static struct {
    Vec3        pos;
    F32            minDistSQR;
    Beacon*        minDistBeacon;
} staticBeaconSearchData;

static void aiFindClosestBeaconHelper(Array* beaconArray, void* unused){
    int i;

    for(i = 0; i < beaconArray->size; i++){
        Beacon* b = beaconArray->storage[i];

        if(b->gbConns.size || b->rbConns.size){
            float distSQR = distance3Squared(posPoint(b), staticBeaconSearchData.pos);

            if(!staticBeaconSearchData.minDistBeacon || distSQR <= staticBeaconSearchData.minDistSQR)
            {
                staticBeaconSearchData.minDistSQR = distSQR;
                staticBeaconSearchData.minDistBeacon = b;
            }
        }
    }
}

Beacon* aiFindClosestBeacon(const Vec3 centerPos)
{
    Beacon* bestBeacon = NULL;
    F32 minDist;
    int i;

    staticBeaconSearchData.minDistBeacon = NULL;
    copyVec3(centerPos, staticBeaconSearchData.pos);

    beaconForEachBlock(centerPos, 100, 500, 100, aiFindClosestBeaconHelper, NULL);

    if(staticBeaconSearchData.minDistBeacon){
        return staticBeaconSearchData.minDistBeacon;
    }

    for(i = 0; i < combatBeaconArray.size; i++){
        Beacon* b = combatBeaconArray.storage[i];
        F32 dist = distance3Squared(posPoint(b), centerPos);

        if(!bestBeacon || dist < minDist){
            bestBeacon = b;
            minDist = dist;
        }
    }

    return bestBeacon;
}

void aiConstructPathToTarget(Entity* e){
    AIVars* ai = ENTAI(e);
    NavSearch* navSearch = ai->navSearch;
    NavSearchResults results;
    char resultMsg[1000] = "";
    EntType entType = ENTTYPE(e);
    S64 startTime;
    S64 endTime;

    clearNavSearch(navSearch);

    if(    !ai->followTarget.type ||
        (    ai->motion.failedPathWhileFalling &&
            e->motion->falling &&
            !vec3IsZero(e->motion->vel)))
    {
        return;
    }

    if(ai->pathLimited && !aiCheckIfInPathLimitedVolume(ai->followTarget.pos))
        return;
    
    ai->motion.failedPathWhileFalling = 0;
    
    copyVec3(ai->followTarget.pos, ai->followTarget.posWhenPathFound);
    
    beaconSetPathFindEntity(e, ai->canFly ? 1000000 : aiGetJumpHeight(e));

    switch(ai->followTarget.type){
        xcase AI_FOLLOWTARGET_ENTITY:
            PERFINFO_AUTO_START("pathToEntity", 1);
        xcase AI_FOLLOWTARGET_BEACON:
            PERFINFO_AUTO_START("pathToBeacon", 1);
        xcase AI_FOLLOWTARGET_POSITION:
            PERFINFO_AUTO_START("pathToPosition", 1);
        xcase AI_FOLLOWTARGET_ENTERABLE:
            PERFINFO_AUTO_START("pathToEnterable", 1);
        xcase AI_FOLLOWTARGET_MOVING_POINT:
            PERFINFO_AUTO_START("pathToMovingPoint", 1);
        xdefault:
            PERFINFO_AUTO_START("pathToUnknown", 1);
    }
    {
        GET_CPU_TICKS_64(startTime);
    
        // Do the pathfinding.
        
        ZeroStruct(&results);

        if(AI_LOG_ENABLED(AI_LOG_PATH)){
            results.resultMsg = resultMsg;
        }
        
        beaconPathFind(navSearch, ENTPOS(e), ai->followTarget.pos, &results);
                        
        GET_CPU_TICKS_64(endTime);
    }
    PERFINFO_AUTO_STOP();

    // Add a path log entry.
    
    {
        AIPathLogEntry* newEntry = aiPathLogAddEntry(endTime - startTime);
        
        newEntry->entityRef = erGetRef(e);
        newEntry->entType = ENTTYPE(e);
        newEntry->canFly = ai->canFly;
        newEntry->jumpHeight = ai->canFly ? 1000 : aiGetJumpHeight(e);
        copyVec3(ENTPOS(e), newEntry->sourcePos);
        copyVec3(ai->followTarget.pos, newEntry->targetPos);
        newEntry->success = navSearch->path.head ? 1 : 0;
        newEntry->abs_time = ABS_TIME;
        copyVec3(results.sourceBeaconPos, newEntry->sourceBeaconPos);
        copyVec3(results.targetBeaconPos, newEntry->targetBeaconPos);
        newEntry->searchResult = results.resultType;
    }
    
    if(results.losFailed){
        // Move my spawn point if I can't get a beacon vis to my spawn point.
        
        if(ai->followTarget.reason == AI_FOLLOWTARGETREASON_SPAWNPOINT){
            if(sameVec3(ai->followTarget.pos, ai->spawn.pos)){
                NavPathWaypoint* wp_tail = navSearch->path.tail;
                if(wp_tail && wp_tail->beacon){
                    copyVec3(posPoint(wp_tail->beacon), ai->spawn.pos);
                }
            }
        }
    }
    
    if(!navSearch->path.head){
        if(    results.resultType == NAV_RESULT_NO_SOURCE_BEACON &&
            ABS_TIME_SINCE(ai->time.fixedStuckPosition) > SEC_TO_ABS(10))
        {
            // Can't find a source beacon so go to the closest beacon and log this position.
            
            Beacon* bestBeacon = NULL;
            
            PERFINFO_AUTO_START("FixStuckEntity", 1);
                ai->time.fixedStuckPosition = ABS_TIME;
                
                bestBeacon = aiFindClosestBeacon(ENTPOS(e));
                
                if(bestBeacon){
                    Vec3 newPos;
                    char state_buffer[1000] = "";
                    char* bufpos = state_buffer;
                    
                    copyVec3(posPoint(bestBeacon), newPos);
                    vecY(newPos) -= beaconGetFloorDistance(bestBeacon);
                    
                    if(ai->isFlying){
                        bufpos += sprintf(bufpos, "Flying, ");
                    }

                    if(ai->canFly){
                        bufpos += sprintf(bufpos, "CanFly, ");
                    }

                    bufpos += sprintf(bufpos, "Jump %1.2f", aiGetJumpHeight(e));

                    dbLog(    "AI:Stuck", e,
                            "Stuck at: (%1.2f, %1.2f, %1.2f).  Moved to: (%1.2f, %1.2f, %1.2f).  Spawn: (%1.2f, %1.2f, %1.2f).  State: (%s)",
                            ENTPOSPARAMS(e),
                            vecParamsXYZ(newPos),
                            vecParamsXYZ(ai->spawn.pos),
                            state_buffer);

                    entUpdatePosInstantaneous(e, newPos);
                    
                    e->move_instantly = 0;
                }
            PERFINFO_AUTO_STOP();
        }

        AI_LOG(    AI_LOG_PATH,
                (e,
                "Path ^1FAILED^0: ^4%s^0cycles: ^4%s\n",
                getCommaSeparatedInt(endTime - startTime),
                resultMsg));
    }else{
        if(AI_LOG_ENABLED(AI_LOG_PATH)){
            NavPath* path = &navSearch->path;
            NavPathWaypoint* waypoint = path->head;
            Beacon* sourceBeacon = waypoint->beacon;

            AI_LOG(    AI_LOG_PATH,
                    (e,
                    "Path ^2SUCCESS^0: ^4%d^0wps: ^4%s^0cycles: %s\n"
                    "  Beacon: ( ^4%1.1f^0, ^4%1.1f^0, ^4%1.1f ^0) - ^4%d\n"
                    "  Me: ( ^4%1.1f^0, ^4%1.1f^0, ^4%1.1f ^0)\n",
                    path->waypointCount,
                    getCommaSeparatedInt(endTime - startTime),
                    resultMsg,
                    posParamsXYZ(sourceBeacon),
                    sourceBeacon->pathsBlockedToMe,
                    ENTPOSPARAMS(e)));
        }
    }
}

static int beaconTypeNumber = 0;

static int validTrafficBeaconCondition(Beacon* dest, Entity* e){
    AIVars* ai = ENTAI(e);
    float newHeading, beaconHeading;
    float deltaHeading;
    Vec3 newDirection;
    int good;
    
    if(beaconTypeNumber && beaconTypeNumber != dest->trafficTypeNumber)
    {
        return 0;
    }

    // Find the entity's new direction given the new beacon
    subVec3(posPoint(dest), ENTPOS(e), newDirection);

    // Extract the new direction's heading
    newHeading = getVec3Yaw(newDirection);
    beaconHeading = getVec3Yaw(dest->mat[2]);

    // Make sure that the differece of the two headings doesn't exceed a certain tolerance level
    deltaHeading = subAngle(ai->pyr[1], beaconHeading);

    good = deltaHeading < RAD(100) && deltaHeading > RAD(-100);

    //printf("%s beacon %5.2f, %5.2f, %5.2f\n", good ? "accepting" : "rejecting", posX(dest), posY(dest), posZ(dest));

    return good;
}

static int validBeaconCondition(Beacon* dest, Entity* e){
    return !dest->madeGroundConnections || dest->gbConns.size > 0;
}

Beacon* aiFindRandomNearbyBeacon(Entity* e, Beacon* lastBeacon, Array* beaconSet, PickBeaconScoreFunction beaconScoreFunc){
    AIVars* ai = ENTAI(e);
    Array* result;
    Beacon* nearbyBeacon = NULL;
    int attemptCount = 0;
    BeaconConnection* conn;
    int i;

    // If the entity doesn't have a current beacon

    if(!lastBeacon){
        if(beaconSet && beaconSet->size){
            if(ENTTYPE(e) == ENTTYPE_CAR){
                beaconTypeNumber = ai->car.beaconTypeNumber;
                nearbyBeacon = posCondFindNearest(beaconSet, e, validTrafficBeaconCondition);
                if(!nearbyBeacon && beaconTypeNumber){
                    beaconTypeNumber = 0;
                    nearbyBeacon = posCondFindNearest(beaconSet, e, validTrafficBeaconCondition);
                }
            }

            if(!nearbyBeacon){
                nearbyBeacon = posCondFindNearest(beaconSet, e, validBeaconCondition);
            }

            return nearbyBeacon ? nearbyBeacon : (Beacon*) posFindNearest(beaconSet, ENTMAT(e));
        }else{
            return NULL;
        }
    }

    assert(ENTTYPE(e) != ENTTYPE_CRITTER);

    result = beaconGetGroundConnections(lastBeacon, beaconSet);

    if(!result->size){
        return NULL;
    }
    else if(result && result->size == 1){
        conn = result->storage[0];
    }
    else if(beaconScoreFunc){
        int best = -1;
        int bestScore = INT_MIN;
        
        assert(result->storage);

        for(i = 0; i < result->size; i++){
            int score = beaconScoreFunc(e, ((BeaconConnection*)result->storage[i])->destBeacon);

            if(score > bestScore || (score == bestScore && rand() & 1)){
                best = i;
                bestScore = score;
            }
        }

        assert(best >= 0);

        conn = result->storage[best];
    }
    else{
        conn = result->storage[getCappedRandom(result->size)];
    }

    nearbyBeacon = conn->destBeacon;

    return nearbyBeacon;
}

void aiSetFollowTargetEntity(Entity* e, AIVars* ai, Entity* target, AIFollowTargetReason reason){
    if (reason == AI_FOLLOWTARGETREASON_SPAWNPOINT && ai->spawnPosIsSelf)
    {
        //    This entity is using his own position as a spawn point
        //    He shouldn't be told to return to spawn since he is always there.
        //    As of 12/16/11 there were no known paths that did this -EL
        Errorf("Entity that uses his position as a spawn point is being told to return to spawn. Grab a programmer.");
    }
    else
    {
        assert(target);
        AI_LOG(    AI_LOG_PATH,
            (e,
            "^2Following Entity/%s^0: %s\n",
            aiGetFollowTargetReasonText(reason),
            AI_LOG_ENT_NAME(target)));

        ai->followTarget.type = AI_FOLLOWTARGET_ENTITY;
        ai->followTarget.entity = target;
        ai->followTarget.reason = reason;
        ai->followTarget.pos = ENTPOS(target);
        ai->followTarget.followTargetDiscarded = ai->followTarget.followTargetReached = 0;
    }
}

void aiSetFollowTargetBeacon(Entity* e, AIVars* ai, Beacon* target, AIFollowTargetReason reason){
    AI_LOG(    AI_LOG_PATH,
            (e,
            "^2Following Beacon/%s^0: (^4%1.f^0,^4%1.f^0,^4%1.f^0)\n",
            aiGetFollowTargetReasonText(reason),
            posParamsXYZ(target)));

    ai->followTarget.type = AI_FOLLOWTARGET_BEACON;
    ai->followTarget.beacon = target;
    ai->followTarget.reason = reason;
    ai->followTarget.pos = target->mat[3];
    ai->followTarget.followTargetDiscarded = ai->followTarget.followTargetReached = 0;
}

void aiSetFollowTargetPosition(Entity* e, AIVars* ai, const Vec3 pos, AIFollowTargetReason reason){
    AI_LOG(    AI_LOG_PATH,
        (e,
        "^2Following Position/%s^0: (^4%1.f^0,^4%1.f^0,^4%1.f^0)\n",
        aiGetFollowTargetReasonText(reason),
        vecParamsXYZ(pos)));

    ai->followTarget.type = AI_FOLLOWTARGET_POSITION;
    ai->followTarget.reason = reason;
    copyVec3(pos, ai->followTarget.storedPos);
    ai->followTarget.pos = ai->followTarget.storedPos;
    ai->followTarget.followTargetDiscarded = ai->followTarget.followTargetReached = 0;
}

void aiSetFollowTargetDoor(Entity* e, AIVars* ai, DoorEntry* door, AIFollowTargetReason reason){
    AI_LOG(    AI_LOG_PATH,
        (e,
        "^2Following Door/%s^0: (^4%1.f^0,^4%1.f^0,^4%1.f^0)\n",
        aiGetFollowTargetReasonText(reason),
        vecParamsXYZ(door->mat[3])));

    ai->followTarget.type = AI_FOLLOWTARGET_ENTERABLE;
    ai->followTarget.reason = reason;
    copyVec3(door->mat[3], ai->followTarget.storedPos);
    ai->followTarget.pos = ai->followTarget.storedPos;
    ai->followTarget.followTargetDiscarded = ai->followTarget.followTargetReached = 0;
}

void aiSetFollowTargetMovingPoint(Entity* e, AIVars* ai, Mat4 point, AIFollowTargetReason reason){

    if (!e || !ai || !point)
        return;

    AI_LOG(    AI_LOG_PATH,
        (e,
        "^2Following MovingPoint/%s^0: (^4%1.f^0,^4%1.f^0,^4%1.f^0)\n",
        aiGetFollowTargetReasonText(reason),
        vecParamsXYZ(point[3])));

    ai->followTarget.type = AI_FOLLOWTARGET_MOVING_POINT;
    ai->followTarget.reason = reason;
    ai->followTarget.movingPoint = point;
//    copyMat4(point, ai->followTarget.movingPoint);
    ai->followTarget.pos = ai->followTarget.movingPoint[3];
    ai->followTarget.followTargetDiscarded = ai->followTarget.followTargetReached = 0;
}

const char* aiGetFollowTargetReasonText(AIFollowTargetReason reason){
    switch(reason){
        case AI_FOLLOWTARGETREASON_ATTACK_TARGET:
            return "AttackTarget";
        case AI_FOLLOWTARGETREASON_ATTACK_TARGET_LASTPOS:
            return "LastPos";
        case AI_FOLLOWTARGETREASON_CIRCLECOMBAT:
            return "CircleCombat";
        case AI_FOLLOWTARGETREASON_ESCAPE:
            return "Escape";
        case AI_FOLLOWTARGETREASON_SAVIOR:
            return "Savior";
        case AI_FOLLOWTARGETREASON_SPAWNPOINT:
            return "SpawnPoint";
        case AI_FOLLOWTARGETREASON_PATROLPOINT:
            return "PatrolPoint";
        case AI_FOLLOWTARGETREASON_LEADER:
            return "Leader";
        case AI_FOLLOWTARGETREASON_STUNNED:
            return "Stunned";
        case AI_FOLLOWTARGETREASON_AVOID:
            return "Avoid";
        case AI_FOLLOWTARGETREASON_ENTERABLE:
            return "Enterable";
        case AI_FOLLOWTARGETREASON_COWER:
            return "Cower";
        case AI_FOLLOWTARGETREASON_BEHAVIOR:
            return "Behavior";
        case AI_FOLLOWTARGETREASON_MOVING_TO_HELP:
            return "MovingToHelp";
        case AI_FOLLOWTARGETREASON_CREATOR:
            return "Creator";
        default:
            return "None";
    }
}

void aiDiscardFollowTarget(Entity* e, const char* reason, bool reached){
    AIVars* ai = ENTAI(e);

    if(ai->followTarget.type){
        if(AI_LOG_ENABLED(AI_LOG_PATH)){
            const char* followReason = aiGetFollowTargetReasonText(ai->followTarget.reason);
            
            AI_LOG(    AI_LOG_PATH,
                    (e,
                    "^1Stopped following: ^0"));
                    
            switch(ai->followTarget.type){
                xcase AI_FOLLOWTARGET_BEACON:
                    AI_LOG(    AI_LOG_PATH,
                            (e,
                            "Beacon/%s(^4%1.2f^0,^4%1.2f^0,^4%1.2f^0)",
                            followReason,
                            posParamsXYZ(ai->followTarget.beacon)));
                
                xcase AI_FOLLOWTARGET_ENTITY:
                    AI_LOG(    AI_LOG_PATH,
                            (e,
                            "Entity/%s %s^0(^4%d^0) @ ^0(^4%1.2f^0,^4%1.2f^0,^4%1.2f^0)",
                            followReason,
                            ai->followTarget.entity ? AI_LOG_ENT_NAME(ai->followTarget.entity) : "NONE",
                            ai->followTarget.entity ? ai->followTarget.entity->owner : 0,
                            ENTPOSPARAMS(ai->followTarget.entity)));
                    
                xcase AI_FOLLOWTARGET_POSITION:
                    AI_LOG(    AI_LOG_PATH,
                            (e,
                            "Position/%s(^4%1.2f^0,^4%1.2f^0,^4%1.2f^0)",
                            followReason,
                            vecParamsXYZ(ai->followTarget.pos)));

                xcase AI_FOLLOWTARGET_ENTERABLE:
                    AI_LOG( AI_LOG_PATH,
                            (e,
                            "Enterable/%s(^4%1.2f^0,^4%1.2f^0,^4%1.2f^0)",
                            followReason,
                            vecParamsXYZ(ai->followTarget.pos)));

                xcase AI_FOLLOWTARGET_MOVING_POINT:
                    AI_LOG( AI_LOG_PATH,
                            (e,
                            "MovingPoint/%s(^4%1.2f^0,^4%1.2f^0,^4%1.2f^0)",
                            followReason,
                            vecParamsXYZ(ai->followTarget.pos)));
            }
            
            if(reason){
                AI_LOG(    AI_LOG_PATH,
                        (e,
                        " (^2%s^0)",
                        reason));
            }

            AI_LOG(AI_LOG_PATH, (e, "\n"));
        }

        ai->followTarget.type = AI_FOLLOWTARGET_NONE;
        ai->followTarget.pos = NULL;
        ai->followTarget.movingPoint = NULL;

        ai->followTarget.followTargetDiscarded = !reached;
        ai->followTarget.followTargetReached = !ai->followTarget.followTargetDiscarded;

        ai->doNotRun = ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : 0;
        
        if(ai->navSearch){
            clearNavSearch(ai->navSearch);
        }

        if(ai->motion.type != AI_MOTIONTYPE_JUMP){
            aiMotionSwitchToNone(e, ai, "Discarded follow target, and I'm not jumping.");
        }
    }
}

void aiPathUpdateBlockedWaypoint(NavPathWaypoint* wp)
{
    Beacon* b;

    if(!wp)
        return;

    b = wp->beacon;

    if(wp->connectionToMe){
        // Store the latest time that this connection failed.

        wp->connectionToMe->timeWhenIWasBad = ABS_TIME;
    }

    // Increment the count of paths that were blocked to this beacon.

    if(!wp->prev && b && b->pathsBlockedToMe < INT_MAX)
        b->pathsBlockedToMe++;
}

void aiCheckFollowStatus(Entity* e, AIVars* ai){
    if(ai->followTarget.type)
    {
        if(ai->pathLimited && !aiCheckIfInPathLimitedVolume(ai->followTarget.pos))
        {
            aiDiscardFollowTarget(e, "My followtarget is not in a path limited allowed volume.", 0);
            return;
        }

        if(ai->followTarget.type != AI_FOLLOWTARGET_ENTERABLE && !ai->motion.ground.waitingForDoorAnim){
            float distSquared;
            const float* standoffTarget = ai->followTarget.pos;
            float maxDist = ai->followTarget.standoffDistance - ai->collisionRadius;

            // Did I reach the target?

            if(ai->navSearch->path.head || ai->followTarget.type == AI_FOLLOWTARGET_ENTITY
                || ai->followTarget.reason == AI_FOLLOWTARGETREASON_BEHAVIOR){
                    distSquared = distance3Squared(ENTPOS(e), standoffTarget);
            }else{
                distSquared = distance3SquaredXZ(ENTPOS(e), standoffTarget);
            }

            if(distSquared < SQR(maxDist)){
                //AI_POINTLOG(AI_LOG_PATH, (e, posPoint(e), 0xffffff00, "Arrived: %1.2f", sqrt(distSquared)));

                // seeing as falling still cancels paths, we need this fix for the fix to the pathfinding while falling...
                ai->motion.failedPathWhileFalling = 0;
                aiSendMessage(e, AI_MSG_FOLLOW_TARGET_REACHED);
            }
        }
    }
}

typedef struct PathLimitedVolume{
    Vec3 min;
    Vec3 max;
}PathLimitedVolume;

static PathLimitedVolume** PathLimitedVolumes = NULL;

static int aiFindLimiterVolumesCallBack(GroupDef * def, Mat4 parent_mat)
{
    if(groupDefFindProperty(def, "PathLimitedVolume"))
    {
        PathLimitedVolume* vol = (PathLimitedVolume*)calloc(1, sizeof(PathLimitedVolume));
        addVec3(parent_mat[3], def->min, vol->min);
        vecY(vol->min) -= 0.5; // to avoid small offset problems with spawned guys
        addVec3(parent_mat[3], def->max, vol->max);
        eaPush(&PathLimitedVolumes, vol);
    }

    if(!def->child_properties)
        return 0;
    else
        return 1;
}

void aiFindPathLimitedVolumes()
{
    eaClearEx(&PathLimitedVolumes, NULL);
    groupProcessDef(&group_info, aiFindLimiterVolumesCallBack);
}

int aiCheckIfInPathLimitedVolume(const Vec3 pos)
{
    int i;
    for(i = eaSize(&PathLimitedVolumes) - 1; i >= 0; i--)
    {
        if(pointBoxCollision(pos, PathLimitedVolumes[i]->min, PathLimitedVolumes[i]->max))
            return 1;
    }
    return 0;
}

int aiGetPathLimitedVolumeCount()
{
    return eaSize(&PathLimitedVolumes);
}