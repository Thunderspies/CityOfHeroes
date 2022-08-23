
#include <utilitieslib/stdtypes.h>
#include "ailib/aiBehaviorPublic.h"
#include "ai/entaiprivate.h"
#include "beacon/beaconPath.h"
#include "ai/entaiPriority.h"
#include "generator/encounter.h"
#include "entity/entserver.h"
#include "beacon/beaconPrivate.h"
#include "beacon/beaconConnection.h"
#include "entity/motion.h"
#include "seq/seqstate.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "gridcoll/megaGrid.h"
#include "cmdparse/cmdcommon.h"
#include "entity/character_base.h"
#include "cmdparse/cmdserver.h"
#include "ai/entaiLog.h"
#include "ai/entai.h"
#include <utilitieslib/utils/timing.h>
#include "dbcomm/staticMapInfo.h"
#include "dbcomm/dbcomm.h"
#include <float.h>
#include "ai/entaiBehaviorCoh.h"

U32 aiNPCGetActivityDataSize(){
    U32 size = 0;

    // All activity data structs should be added here, or an assert might occur at run-time.

    #define GET_SIZE(type) size = max(size, sizeof(type));
        GET_SIZE(AINPCActivityDataThankHero);
        GET_SIZE(AINPCActivityDataHostage);
    #undef GET_SIZE

    return size;
}

void aiNPCActivityChanged(Entity* e, AIVars* ai, int oldActivity){
    int setAlwaysOn = 0;

    aiSetMessageHandler(e, aiNPCMsgHandlerDefault);

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

    switch(aiBehaviorGetActivity(e)){
        case AI_ACTIVITY_THANK_HERO:{
            if(!ai->activityData && devassert(!ai->activityDataAllocated))
                ai->activityData = AI_ALLOC_ACTIVITY_DATA(e, ai, AINPCActivityDataThankHero);

            setAlwaysOn = 1;

            break;
        }

        case AI_ACTIVITY_NPC_HOSTAGE:{
            if(!ai->activityData && devassert(!ai->activityDataAllocated))
                ai->activityData = AI_ALLOC_ACTIVITY_DATA(e, ai, AINPCActivityDataHostage);

            setAlwaysOn = 1;

            break;
        }

        case AI_ACTIVITY_RUN_AWAY:{
            setAlwaysOn = 1;

            break;
        }

        case AI_ACTIVITY_RUN_INTO_DOOR:
            setAlwaysOn = 1;
            break;

        case AI_ACTIVITY_RUN_TO_TARGET:{
            setAlwaysOn = 1;

            break;
        }

        case AI_ACTIVITY_STAND_STILL:{
            e->move_type = 0;

            ai->curSpeed = 0;

            entSetProcessRate(e->owner, ENT_PROCESS_RATE_ON_ODD_SEND);

            break;
        }

        case AI_ACTIVITY_WANDER:{
            if(e->move_type != MOVETYPE_WIRE){
                e->move_type = MOVETYPE_WIRE;
                ai->waitForThink = 1;
            }

            entSetProcessRate(e->owner, ENT_PROCESS_RATE_ON_ODD_SEND);

            break;
        }
    }

    if(setAlwaysOn){
        if(e->move_type == MOVETYPE_WIRE){
            e->move_type = 0;
        }

        entSetSendOnOddSend(e, 0);

        SET_ENTINFO(e).send_hires_motion = 1;

        entSetProcessRate(e->owner, ENT_PROCESS_RATE_ALWAYS);

        ai->npc.wireWanderMode = 0;
    }else{
        ai->npc.wireWanderMode = 1;
    }
}

void aiNPCEntityWasDestroyed(Entity* e, AIVars* ai, Entity* entDestroyed){
    if(ai->whoScaredMe == entDestroyed)
        ai->whoScaredMe = NULL;

    switch(aiBehaviorGetActivity(e)){
        case AI_ACTIVITY_THANK_HERO:{
            AINPCActivityDataThankHero* data = ai->activityData;

            if(data->hero == entDestroyed)
                data->hero = NULL;

            if(data->door == entDestroyed)
                data->door = NULL;

            break;
        }

        case AI_ACTIVITY_NPC_HOSTAGE:{
            AINPCActivityDataHostage* data = ai->activityData;

            if(data->whoScaredMe == entDestroyed)
                data->whoScaredMe = NULL;

            break;
        }
    }
}

static void aiNPCActivityHostage(Entity* e, AIVars* ai){
    AINPCActivityDataHostage* data = ai->activityData;
    int entArray[MAX_ENTITIES_PRIVATE];
    int    entCount;
    int i;

    if(!data->whoScaredMe){
        if(ABS_TIME_SINCE(data->time.checkedProximity) > SEC_TO_ABS(2)){
            entCount = mgGetNodesInRange(0, ENTPOS(e), (void**)entArray, 0);

            for(i = 0; i < entCount; i++){
                int idx = entArray[i];

                if(ENTTYPE_BY_ID(idx) == ENTTYPE_CRITTER){
                    Entity* entProx = validEntFromId(idx);

                    if(entProx && entProx->pchar
                        && (entProx->pchar->iAllyID==kAllyID_Monster || entProx->pchar->iAllyID==kAllyID_Villain))
                    {
                        Character* pchar = entProx->pchar;

                        if(    pchar &&
                            pchar->attrCur.fHitPoints > 0)
                        {
                            F32 dist = distance3Squared(ENTPOS(e), ENTPOS(entProx));

                            if(dist <= SQR(50)){
                                if(aiEntityCanSeeEntity(e, entProx, 50)){
                                    data->whoScaredMe = entProx;

                                    aiGetProxEntStatus(ai->proxEntStatusTable, entProx, 1);

                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if(data->whoScaredMe){
        Character* pchar = data->whoScaredMe->pchar;

        if(    !pchar ||
            !pchar->attrCur.fHitPoints)
        {
            data->whoScaredMe = NULL;
        }
    }

    switch(data->state){
        case 0:{
            // Initialize the script.

            data->time.beginState = ABS_TIME;
            data->time.checkedProximity = ABS_TIME;
            data->state = 1;

            break;
        }

        case 1:{
            // Stand around looking scared.

            aiDiscardFollowTarget(e, "Being scared.", false);

            SETB(e->seq->sticky_state, STATE_FEAR);

            if(data->whoScaredMe){
                data->state = 2;
                data->time.beginState = ABS_TIME;
            }
            else if(ABS_TIME_SINCE(data->time.beginState) > SEC_TO_ABS(4)){
                U32 time_since = ABS_TIME_SINCE(data->time.beginState) - SEC_TO_ABS(4);

                if((U32)rand() % SEC_TO_ABS(4) < time_since){
                    data->state = 2;
                    data->time.beginState = ABS_TIME;
                }
            }

            break;
        }

        case 2:{
            if(!ai->motion.type && ABS_TIME_SINCE(ai->time.foundPath) > SEC_TO_ABS(1)){
                Vec3 pos;
                Beacon* sourceBeacon;
                
                ai->time.foundPath = ABS_TIME;

                if(data->whoScaredMe){
                    copyVec3(ENTPOS(data->whoScaredMe), pos);
                }else{
                    copyVec3(ENTPOS(e), pos);
                }

                vecX(pos) += getCappedRandom(201) - 100;
                vecY(pos) += getCappedRandom(101) - 50;
                vecZ(pos) += getCappedRandom(201) - 100;
                
                beaconSetPathFindEntity(e, aiGetJumpHeight(e));
                
                sourceBeacon = beaconGetClosestCombatBeacon(ENTPOS(e), 1, NULL, GCCB_PREFER_LOS, NULL);

                if(sourceBeacon){
                    // Find a targetBeacon that can be reached from the sourceBeacon, but don't use LOS because we don't care.
                    
                    Beacon* targetBeacon = beaconGetClosestCombatBeacon(pos, 1, sourceBeacon, GCCB_IGNORE_LOS, NULL);
                    
                    if(targetBeacon){
                        aiSetFollowTargetPosition(e, ai, posPoint(targetBeacon), AI_FOLLOWTARGETREASON_ESCAPE);

                        aiConstructPathToTarget(e);

                        if(!ai->navSearch->path.head){
                            aiDiscardFollowTarget(e, "Can't find a path.", false);
                        }else{
                            aiStartPath(e, 0, 0);
                        }
                    }
                }
            }

            if(ABS_TIME_SINCE(data->time.beginState) > SEC_TO_ABS(15)){
                data->state = 1;
                data->time.beginState = ABS_TIME;
            }

            break;
        }
    }
}

//Called by critters now, too
void aiActivityThankHero(Entity* e, AIVars* ai){
    AINPCActivityDataThankHero* data = ai->activityData;
    Entity* entArray[MAX_ENTITIES_PRIVATE];
    int    entCount;
    int i;

    switch(data->state){
        case 0:{
            // I'm trying to get to the hero.

            if(!data->hero){
                data->hero = EncounterWhoSavedMe(e);

                if(!data->hero){
                    data->state = 2;
                    break;
                }
            }

            if(data->hero){
                if(distance3Squared(ENTPOS(e), ENTPOS(data->hero)) < SQR(10)){
                    aiDiscardFollowTarget(e, "Got to my hero, proceeding to thank.", false);

                    data->state = 1;
                    data->storedTime= ABS_TIME;

                    EncounterAISignal(e, ENCOUNTER_THANKHERO);

                    //If the thank string has emotes, use those to time the thank length 
                    if( e->IAmInEmoteChatMode )
                        data->thankStringHasEmotes = 1;

                    break;
                }

                if(    !ai->motion.type ||
                    ai->followTarget.type != AI_FOLLOWTARGET_ENTITY ||
                    ai->followTarget.reason != AI_FOLLOWTARGETREASON_SAVIOR)
                {
                    aiSetFollowTargetEntity(e, ai, data->hero, AI_FOLLOWTARGETREASON_SAVIOR);

                    aiGetProxEntStatus(ai->proxEntStatusTable, data->hero, 1);

                    aiConstructPathToTarget(e);

                    if(!ai->navSearch->path.head){
                        data->state = 2;
                        break;
                    }else{
                        ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 10;
                        aiStartPath(e, 0, 0);
                    }
                }
            }

            break;
        }

        case 1:{
            // I'm thanking the hero.

            Vec3 heroDir;

            if(!data->hero){
                data->state = 2;
                break;
            }

            subVec3(ENTPOS(data->hero), ENTPOS(e), heroDir);

            heroDir[1] = 0;

            aiTurnBodyByDirection(e, ai, heroDir);

            //If the thank you string had emotes in it, use the string's timing to decide when to stop thanking
            //Otherwise, just play the generic thank you for 5 seconds.
            if( data->thankStringHasEmotes )
            {
                if( !e->IAmInEmoteChatMode )
                    data->state = 2;
            }
            else
            {
                SETB(e->seq->sticky_state, STATE_THANK);

                if(ABS_TIME_SINCE(data->storedTime) > SEC_TO_ABS(5)){
                    data->state = 2;
                }
            }

            break;
        }

        case 2:{
            // I'm running to a door.

            CLRB(e->seq->sticky_state, STATE_THANK);

            if(!data->door){
                Entity* closest = NULL;
                F32    minDist = FLT_MAX;
                int doorCount = 0;
                Entity* door;
                Beacon* startBeacon;

                beaconSetPathFindEntity(e, 0);

                startBeacon = beaconGetClosestCombatBeacon(ENTPOS(e), 1, NULL, GCCB_PREFER_LOS, NULL);

                //gridFindObjectsInSphere(    &ent_grid,
                //                            (void **)entArray,
                //                            ARRAY_SIZE(entArray),
                //                            posPoint(e),
                //                            1000);

                if(!startBeacon){
                    SET_ENTINFO(e).kill_me_please = 1;
                }else{
                    BeaconBlock* cluster = startBeacon->block->galaxy->cluster;
                    BeaconBlock* galaxy;
                    Beacon* doorBeacon;

                    beacon_state.galaxySearchInstance++;

                    entCount = mgGetNodesInRange(0, ENTPOS(e), entArray, 0);

                    for(i = 0; i < entCount; i++){
                        int idx = (int)(intptr_t)entArray[i];

                        if(ENTTYPE_BY_ID(idx) == ENTTYPE_DOOR){
                            door = validEntFromId(idx);

                            if(!door)
                                continue;

                            doorBeacon = entGetNearestBeacon(door);

                            if(doorBeacon){
                                galaxy = doorBeacon->block->galaxy;

                                if(    galaxy->cluster == cluster &&
                                    galaxy->searchInstance != beacon_state.galaxySearchInstance)
                                {
                                    F32 distSQR = distance3Squared(ENTPOS(e), ENTPOS(door));

                                    if(distSQR <= SQR(200)){
                                        if(beaconGalaxyPathExists(startBeacon, doorBeacon, 0, 0)){
                                            entArray[doorCount++] = door;
                                        }else{
                                            entArray[i] = 0;

                                            // Mark the galaxy as bad.

                                            galaxy->searchInstance = beacon_state.galaxySearchInstance;
                                            galaxy->isGoodForSearch = 0;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if(!doorCount){
                        for(i = 0; i < entCount; i++){
                            int idx = (int)(intptr_t)entArray[i];

                            if(idx && ENTTYPE_BY_ID(idx) == ENTTYPE_DOOR){
                                door = validEntFromId(idx);

                                if(!door)
                                    continue;

                                doorBeacon = entGetNearestBeacon(door);

                                if(doorBeacon){
                                    galaxy = doorBeacon->block->galaxy;

                                    if(    galaxy->cluster == cluster &&
                                        galaxy->searchInstance != beacon_state.galaxySearchInstance)
                                    {
                                        F32 distSQR = distance3Squared(ENTPOS(e), ENTPOS(door));

                                        if(    distSQR > SQR(200) &&
                                            distSQR <= SQR(1000))
                                        {
                                            if(beaconGalaxyPathExists(startBeacon, doorBeacon, 0, 0)){
                                                entArray[doorCount++] = door;
                                            }else{
                                                // Mark the galaxy as bad.

                                                galaxy->searchInstance = beacon_state.galaxySearchInstance;
                                                galaxy->isGoodForSearch = 0;
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        if(!doorCount){
                            SET_ENTINFO(e).kill_me_please = 1;
                            break;
                        }
                    }

                    data->door = entArray[rand() % doorCount];
                }
            }

            if(data->door){
                if(distance3Squared(ENTPOS(e), ENTPOS(data->door)) < SQR(10)){
                    NavPathWaypoint* wp;

                    aiDoorSetOpen(data->door, 1);
                    data->state = 3;
                    data->storedTime = 1;

                    // Create a path through the door.

                    clearNavSearch(ai->navSearch);

                    // Door.

                    wp = createNavPathWaypoint();
                    wp->connectType = NAVPATH_CONNECT_WIRE;
                    copyVec3(ENTPOS(data->door), wp->pos);
                    navPathAddTail(&ai->navSearch->path, wp);

                    // Behind door.

                    wp = createNavPathWaypoint();
                    wp->connectType = NAVPATH_CONNECT_WIRE;
                    scaleVec3(ENTMAT(data->door)[2], -5, wp->pos);
                    addVec3(ENTPOS(data->door), wp->pos, wp->pos);
                    navPathAddTail(&ai->navSearch->path, wp);

                    ai->navSearch->path.curWaypoint = ai->navSearch->path.head;

                    aiSetFollowTargetPosition(e, ai, wp->pos, AI_FOLLOWTARGETREASON_NONE);

                    aiStartPath(e, 1, 1);

                    ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 0;

                    break;
                }

                if(    !ai->motion.type ||
                    ai->followTarget.type != AI_FOLLOWTARGET_ENTITY ||
                    ai->followTarget.reason != AI_FOLLOWTARGETREASON_NONE)
                {
                    aiSetFollowTargetEntity(e, ai, data->door, AI_FOLLOWTARGETREASON_NONE);

                    aiGetProxEntStatus(ai->proxEntStatusTable, data->door, 1);

                    aiConstructPathToTarget(e);

                    if(!ai->navSearch->path.head){
                        //printf(    "Couldn't find a path from\n  (%1.1f, %1.1f, %1.1f) to\n  (%1.1f, %1.1f, %1.1f)\n",
                        //        posParamsXYZ(e),
                        //        posParamsXYZ(data->door));
                        AI_LOG(AI_LOG_PATH, (e, "Killing self.\n"));
                        SET_ENTINFO(e).kill_me_please = 1;
                        break;
                    }else{
                        ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 10;
                        aiStartPath(e, 0, 0);
                    }
                }
            }

            break;
        }

        case 3:{
            // I'm waiting for my path to be cleared so I can kill myself.

            if(!ai->navSearch->path.head){
                AI_LOG(AI_LOG_PATH, (e, "Killing self.\n"));
                SET_ENTINFO(e).kill_me_please = 1;
                break;
            }
        }
    }
}

static void aiNPCActivityRunAway(Entity* e, AIVars* ai){
    if(ABS_TIME_SINCE(ai->time.foundPath) > SEC_TO_ABS(2)){
        if(!ai->followTarget.type){
            int i;
            float bestRatio = 1;
            Beacon* best = NULL;
            float bestNoDistRatio = 1;
            Beacon* bestNoDist = NULL;

            for(i = 0; i < basicBeaconArray.size; i++){
                Beacon* b = basicBeaconArray.storage[i];
                float ratio;
                float beaconDistToMe = distance3(ENTPOS(e), posPoint(b));

                ratio = distance3(ai->anchorPos, posPoint(b)) / beaconDistToMe;

                if(ai->anchorRadius <= 0 || beaconDistToMe > ai->anchorRadius){
                    if(ratio > bestRatio){
                        best = b;
                        bestRatio = ratio;
                    }
                }

                if(ratio > bestNoDistRatio){
                    bestNoDist = b;
                    bestNoDistRatio = ratio;
                }
            }

            if(!best){
                best = bestNoDist;
            }

            if(best){
                Vec3 pos;
                copyVec3(posPoint(best), pos);
                vecY(pos) -= 2;
                aiSetFollowTargetPosition(e, ai, pos, AI_FOLLOWTARGETREASON_ESCAPE);
            }
        }

        if(ai->followTarget.type && !ai->navSearch->path.head){
            ai->time.foundPath = ABS_TIME;

            aiConstructPathToTarget(e);

            aiStartPath(e, 0, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);
        }
    }

    if(ai->anchorRadius && distance3Squared(ENTPOS(e), ai->anchorPos) > SQR(ai->anchorRadius)){
        ai->waitForThink = 1;
    }
}

static void aiNPCActivityRunToTarget(Entity* e, AIVars* ai){
    if(ABS_TIME_SINCE(ai->time.foundPath) > SEC_TO_ABS(2)){
        if(ai->followTarget.type && !ai->navSearch->path.head){
            ai->time.foundPath = ABS_TIME;

            aiConstructPathToTarget(e);

            if(!ai->navSearch->path.head){
                SET_ENTINFO(e).kill_me_please = 1;
            }

            aiStartPath(e, 0, 0);
        }
    }

    if(ai->followTarget.type){
        if(distance3Squared(ENTPOS(e), ai->followTarget.pos) < SQR(10)){
            aiDiscardFollowTarget(e, "Within 10 feet of target.", true);

            clearNavSearch(ai->navSearch);
        }
        else if(ai->navSearch->path.head){
            NavPath* path;
            NavPathWaypoint* lastWaypoint;
            Beacon* lastBeacon;
            float distToTargetSquared;
            float distFromTargetToPathEnd;

            path = &ai->navSearch->path;
            lastWaypoint = path->tail;
            lastBeacon = lastWaypoint->beacon;
            distToTargetSquared = distance3Squared(ENTPOS(e), ai->followTarget.pos);
            distFromTargetToPathEnd = distance3Squared(ai->followTarget.pos, posPoint(lastBeacon));

            if(distToTargetSquared < distFromTargetToPathEnd){
                clearNavSearch(ai->navSearch);
            }
        }
    }

    ai->doNotRun = ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->followTarget.type ? 0 : 1;
}

static int beaconScoreNPCOnWire(Entity* e, Beacon* dest){
    Vec3 diff;
    float yaw;

    subVec3(posPoint(dest), ENTPOS(e), diff);

    yaw = getVec3Yaw(diff);

    return getCappedRandom(100) + 180 - fabs(DEG(subAngle(yaw, ENTAI(e)->pyr[1])));
}

static int NPCBeaconRadius = 0;

void aiNPCResetBeaconRadius()
{
    NPCBeaconRadius = 0;
}

static bool aiNPCFindEndOfIsland(AIVars* ai, Beacon* start, Beacon* next, bool connectAnyway)
{
    int i, j;
    Beacon *lastBeacon, *nextBeacon = start, *newNextBeacon = next;
    Beacon* connBeacon = NULL;
    bool valid = true;
    F32 minDist = INT_MAX;

    // find the "end" of the island... (i.e. a place where NPCs turn back along the path)
    // if this is a circular path it most likely doesn't matter from which beacon we try to find
    // a connection
    for(i = 0; newNextBeacon && i < 10; i++)
    {
        F32 minAngle = RAD(90);
        Vec3 dir;
        F32 yaw;

        lastBeacon = nextBeacon;
        nextBeacon = newNextBeacon;
        newNextBeacon = NULL;

        subVec3(posPoint(nextBeacon), posPoint(lastBeacon), dir);
        yaw = getVec3Yaw(dir);

        for(j = nextBeacon->gbConns.size-1; j >= 0; j--)
        {
            Vec3 newdir;
            F32 newyaw, angleDiff;
            BeaconConnection* conn = nextBeacon->gbConns.storage[j];
            subVec3(posPoint(conn->destBeacon), posPoint(nextBeacon), newdir);
            newyaw = getVec3Yaw(newdir);
            angleDiff = subAngle(newyaw, yaw);

            if(fabs(angleDiff) < minAngle)
            {
                minAngle = angleDiff;
                newNextBeacon = conn->destBeacon;
            }
        }
    }

    // I hope going through the beacons here is fast enough so I don't have to think of something better
    for(i = basicBeaconArray.size - 1; i >= 0; i--)
    {
        Beacon* compBeacon = basicBeaconArray.storage[i];

        if(compBeacon->userInt != nextBeacon->userInt)
        {
            F32 dist = distance3Squared(posPoint(nextBeacon), posPoint(compBeacon));

            if(dist < minDist)
            {
                connBeacon = compBeacon;
                minDist = dist;
            }
        }
    }

    if(!devassertmsg(connBeacon, "Does this map have NPC generators but no NPC beacons?"))
    {
        ai->npc.doNotCheckForIsland = 1;
        start->NPCNoAutoConnect = 1;
        return true;
    }

    if(!connectAnyway)
    {
        for(i = connBeacon->gbConns.size - 1; i >= 0; i--)
        {
            BeaconConnection* conn = connBeacon->gbConns.storage[i];
            if(conn->destBeacon == nextBeacon)
            {
                valid = false;
                break;
            }
        }
    }

    if(valid)
    {
        BeaconConnection* newConn = createBeaconConnection();
        newConn->destBeacon = connBeacon;
        arrayPushBack(&nextBeacon->gbConns, newConn);
    }

    return valid;
}

static void aiNPCMoveWire(Entity* e, AIVars* ai){
    MotionState* motion = e->motion;
    int moveForward = 0;
    float distToMove;

    // This is the code for regular NPCs who just run along wires with no physics.

    if(ai->inputs.limitedSpeed)
        distToMove = ai->inputs.limitedSpeed;
    else
        distToMove = motion->input.surf_mods[SURFPARAM_GROUND].max_speed;

    if(ai->doNotRun)
        distToMove /= 4.8;

    if(ai->curSpeed < distToMove){
        ai->curSpeed += 0.05 * e->timestep;

        if(ai->curSpeed > distToMove){
            ai->curSpeed = distToMove;
        }
    }
    else if(ai->curSpeed > distToMove){
        ai->curSpeed -= 0.05 * e->timestep;

        if(ai->curSpeed < distToMove){
            ai->curSpeed = distToMove;
        }
    }

    distToMove = ai->curSpeed * e->timestep;

    while(distToMove > 0){
        int findNewBeacon = 0;

        // Check if I've reached current target yet.

        if(ai->followTarget.type){
            moveForward = 1;

            if(ai->npc.distanceToTarget > distToMove){
                // If I'm farther from the current target than I can move in this frame, then just move along the line.

                Vec3 diff;
                Vec3 newpos;

                ai->npc.distanceToTarget -= distToMove;

                subVec3(ai->followTarget.pos, ENTPOS(e), diff);

                normalVec3(diff);

                scaleVec3(diff, distToMove, diff);

                addVec3(ENTPOS(e), diff, newpos);

                entUpdatePosInterpolated(e, newpos);

                break;
            }else{
                // I can make it to the current target, so do so.

                distToMove -= ai->npc.distanceToTarget;

                entUpdatePosInterpolated(e, ai->followTarget.pos);

                findNewBeacon = 1;
            }
        }else{
            findNewBeacon = 1;
        }

        if(findNewBeacon){
            Beacon* newDestBeacon;

            ai->pyr[1] = getVec3Yaw(ENTMAT(e)[2]);

            // Pick a new beacon.

            newDestBeacon = aiFindRandomNearbyBeacon(e, ai->npc.lastWireBeacon, &basicBeaconArray, beaconScoreNPCOnWire);

            if(!newDestBeacon && ai->npc.lastWireBeacon && !ai->npc.lastWireBeacon->gbConns.size){
                int i;
                for(i = 0; i < 10; i++){
                    newDestBeacon = aiFindRandomNearbyBeacon(e, NULL, &basicBeaconArray, beaconScoreNPCOnWire);
                    
                    if(newDestBeacon && newDestBeacon != ai->npc.lastWireBeacon){
                        BeaconConnection* conn = createBeaconConnection();
                        
                        conn->destBeacon = newDestBeacon;

                        arrayPushBack(&ai->npc.lastWireBeacon->gbConns, conn);
                        
                        break;
                    }else{
                        newDestBeacon = NULL;
                    }
                }
            }

            if(newDestBeacon){
                Vec3 vecToDest;
                Vec3 pyr;
                F32 randAngle, randDist;
                Mat3 newmat;

                if(!NPCBeaconRadius)
                {
                    const MapSpec* spec = MapSpecFromMapId(db_state.base_map_id);

                    if(!spec)
                        spec = MapSpecFromMapName(db_state.map_name);

                    if (spec)
                        NPCBeaconRadius = spec->NPCBeaconRadius;

                    if(!NPCBeaconRadius)
                        NPCBeaconRadius = 3;
                }

                if(!ai->npc.doNotCheckForIsland && ai->npc.antiIslandBeacon &&
                    ai->npc.lastWireBeacon && newDestBeacon == ai->npc.antiIslandBeacon)
                {
                    if(++ai->npc.antiIslandCounter > 3)    // I'm on an island
                    {
                        // make a new NPC beacon connection somehow (one way to not have ppl walk through geometry a lot)
                        int size = 0;
                        int noAutoConnect;

                        PERFINFO_AUTO_START("NPC Anti Island - check", 1);

                        noAutoConnect = beaconNPCClusterTraverser(newDestBeacon, &size, 10);

                        if(noAutoConnect || size >= 10)
                            ai->npc.doNotCheckForIsland = 1;
                        else
                        {
                            PERFINFO_AUTO_START("NPC Anti Island - connect", 1);

                            if(!aiNPCFindEndOfIsland(ai, ai->npc.lastWireBeacon, newDestBeacon, 0))
                            {
                                PERFINFO_AUTO_START("NPC Anti Island - connect backwards", 1);
                                aiNPCFindEndOfIsland(ai, newDestBeacon, ai->npc.lastWireBeacon, 1);
                                PERFINFO_AUTO_STOP();
                            }

                            PERFINFO_AUTO_STOP();
                        }

                        PERFINFO_AUTO_STOP();
                    }
                }
                if(!ai->npc.antiIslandBeacon || ++ai->npc.antiIslandTimer > 20)
                {
                    ai->npc.antiIslandTimer = 0;
                    ai->npc.antiIslandBeacon = newDestBeacon;
                }

                ai->npc.lastWireBeacon = newDestBeacon;

                aiSetFollowTargetBeacon(e, ai, newDestBeacon, 0);

                // Offset the position, but keep the beacon pointer.

                copyVec3(ai->followTarget.pos, ai->followTarget.storedPos);
                ai->followTarget.pos = ai->followTarget.storedPos;

                randAngle = rule30Float() * PI;
                randDist = rule30Float() * NPCBeaconRadius;

                copyVec3(ai->followTarget.pos, ai->npc.wireTargetPos);

                vecX(ai->npc.wireTargetPos) += cos(randAngle) * randDist;
                vecZ(ai->npc.wireTargetPos) += sin(randAngle) * randDist;

                vecY(ai->npc.wireTargetPos) += ai->npc.groundOffset - 2;

                ai->followTarget.pos = ai->npc.wireTargetPos;

                ai->npc.distanceToTarget = distance3(ENTPOS(e), ai->followTarget.pos);

                subVec3(ai->followTarget.pos, ENTPOS(e), vecToDest);

                vecY(vecToDest) = 0;

                pyr[1] = getVec3Yaw(vecToDest);

                pyr[0] = pyr[2] = 0;

                createMat3YPR(newmat, pyr);
                entSetMat3(e, newmat);
            }else{
                if(basicBeaconArray.size){
                    SET_ENTINFO(e).kill_me_please = 1;
                }

                break;
            }
        }
    }

    if(moveForward){
        // Snap NPCs to the ground.  This is kind of expensive.
        Vec3 newpos;

        if(!ai_state.noNPCGroundSnap){
            if(fabs(ENTPOSY(e) - motion->last_pos[1]) > 0.01){
                if(aiGetPointGroundOffset(ENTPOS(e), 5, 5, newpos)){
                    vecY(newpos) += ai->npc.groundOffset;
                    entUpdatePosInterpolated(e, newpos);
                }
            }
        }

        SETB(e->seq->state, STATE_FORWARD);

        if(ai->doNotRun){
            SETB(e->seq->state, STATE_WALK);
        }
    }

    copyVec3(ENTPOS(e), motion->last_pos);
}

static void aiNPCMoveWalk(Entity* e, AIVars* ai){
    float targetMoveDistSQR;
    float targetMoveDistY;

    // Check on the status of my follow target.

    if(ai->npc.runningBehavior)
        aiCheckFollowStatus(e, ai);
    else if(ai->followTarget.type == AI_FOLLOWTARGET_ENTITY){
        if(ai->navSearch->path.head){
            NavPathWaypoint* lastWaypoint = ai->navSearch->path.tail;

            float distFromTargetToPathEndSQR =
                distance3Squared(    ENTPOS(ai->followTarget.entity),
                                    lastWaypoint->pos);

            // Get some distances.

            if(ai->isAfraidFromAI || ai->isAfraidFromPowers){
                float distToPathEndSQR =
                    distance3Squared(    ENTPOS(e),
                                        lastWaypoint->pos);

                if(    ABS_TIME_SINCE(ai->time.foundPath) > SEC_TO_ABS(1) &&
                    distFromTargetToPathEndSQR < distToPathEndSQR)
                {
                    aiSendMessage(e, AI_MSG_BAD_PATH);
                }
            }else{
                // If I'm closer to my target than the end of my path is, it's a bad path.

                float distToTargetSQR =
                    distance3Squared(    ENTPOS(e),
                                        ENTPOS(ai->followTarget.entity));

                float distFromTargetToWhereHeWasWhenPathWasMadeSQR =
                    distance3Squared(    ENTPOS(ai->followTarget.entity),
                                        ai->followTarget.posWhenPathFound);

                if(    distToTargetSQR < distFromTargetToPathEndSQR &&
                    distFromTargetToWhereHeWasWhenPathWasMadeSQR > distToTargetSQR)
                {
                    aiSendMessage(e, AI_MSG_BAD_PATH);
                }
            }
        }
        else if(ai->waitingForTargetToMove){
            targetMoveDistSQR = distance3SquaredXZ(ai->followTarget.posWhenPathFound, ENTPOS(ai->followTarget.entity));
            targetMoveDistY = fabs(vecY(ai->followTarget.posWhenPathFound) - ENTPOSY(ai->followTarget.entity));

            if(    targetMoveDistSQR > SQR(10) ||
                targetMoveDistY > 30)
            {
                aiSendMessage(e, AI_MSG_ATTACK_TARGET_MOVED);
            }
        }
    }

    // Use generic follow and turn code.

    aiFollowTarget(e, ai);

    if(ai->motion.type != AI_MOTIONTYPE_ENTERABLE)
        aiTurnBody(e, ai, 0);
}

static void aiNPCPriorityDoRealActions(Entity* e, const AIPriorityAction* action, const AIPriorityDoActionParams* params){
    AIVars* ai = ENTAI(e);
    int entArray[MAX_ENTITIES_PRIVATE];
    int entCount;

    // Turn on all the animation bits.

    if(params->firstTime){
        aiSetActivity(e, ai, AI_ACTIVITY_NONE);
    }

    if(action->animBitCount){
        int i;
        for(i = 0; i < action->animBitCount; i++){
            if(action->animBits[i] >= 0){
                SETB(e->seq->state, action->animBits[i]);
                SETB(e->seq->sticky_state, action->animBits[i]);
            }
        }
    }

    if(action->doNotRun >= 0){
        ai->doNotRun = action->doNotRun;
    }

    switch(action->type){
        case AI_PRIORITY_ACTION_DO_ANIM:{
            aiSetActivity(e, ai, AI_ACTIVITY_STAND_STILL);

            break;
        }

        case AI_PRIORITY_ACTION_NPC_HOSTAGE:{
            aiSetActivity(e, ai, AI_ACTIVITY_NPC_HOSTAGE);
            break;
        }

        case AI_PRIORITY_ACTION_THANK_HERO:{
            aiSetActivity(e, ai, AI_ACTIVITY_THANK_HERO);
            break;
        }

        case AI_PRIORITY_ACTION_RUN_AWAY:{
            aiSetActivity(e, ai, AI_ACTIVITY_RUN_AWAY);

            switch(action->targetType){
                case AI_PRIORITY_TARGET_NEARBY_CRITTERS:{
                    if(params->firstTime){
                        int i;
                        Vec3 centerPos = {0,0,0};
                        int critterCount = 0;

                        entCount = mgGetNodesInRange(0, ENTPOS(e), (void**)entArray, 0);

                        //entCount = gridFindObjectsInSphere(    &ent_grid,
                        //                                    (void **)entArray,
                        //                                    ARRAY_SIZE(entArray),
                        //                                    posPoint(e),
                        //                                    0);

                        for(i = 0; i < entCount; i++){
                            Entity* proxEnt = entities[entArray[i]];

                            if(    ENTTYPE(proxEnt) == ENTTYPE_CRITTER &&
                                proxEnt->pchar->attrCur.fHitPoints >= 0 &&
                                distance3Squared(ENTPOS(e), ENTPOS(proxEnt)) < SQR(50))
                            {
                                addVec3(centerPos, ENTPOS(proxEnt), centerPos);
                                critterCount++;
                            }
                        }

                        if(critterCount){
                            scaleVec3(centerPos, 1.0f / critterCount, centerPos);
                            copyVec3(centerPos, ai->anchorPos);
                        }else{
                            copyVec3(ENTPOS(e), ai->anchorPos);
                        }

                        AI_POINTLOG(AI_LOG_PATH, (e, ai->anchorPos, 0xffff0000, "%d Critters' Center", critterCount));

                        ai->anchorRadius = action->radius;
                    }
                }
            }
            break;
        }

        case AI_PRIORITY_ACTION_RUN_INTO_DOOR:
            if(params->firstTime)
            {
                aiBehaviorAddString(e, ENTAI(e)->base, "RunIntoDoor");
//                 aiDiscardFollowTarget(e, "Starting activityRunIntoDoor.", false);
//                 clearNavSearch(ai->navSearch);
//                 ai->time.lastFailedADoorPathCheck = 0;
            }

//             aiSetActivity(e, ai, AI_ACTIVITY_RUN_INTO_DOOR);
            break;

        case AI_PRIORITY_ACTION_RUN_TO_TARGET:{
            if(params->firstTime){
                Beacon* best = NULL;
                float bestDist = -1;
                int i;

                for(i = 0; i < basicBeaconArray.size; i++){
                    Beacon* b = basicBeaconArray.storage[i];
                    float dist = distance3Squared(posPoint(b), ENTPOS(e));

                    if(dist > bestDist){
                        best = b;
                        bestDist = dist;
                    }
                }

                if(best){
                    aiSetActivity(e, ai, AI_ACTIVITY_RUN_TO_TARGET);
                    aiSetFollowTargetBeacon(e, ai, best, AI_FOLLOWTARGETREASON_ESCAPE);
                }else{
                    AI_LOG(AI_LOG_PRIORITY, (e, "Can't find farthest beacon.\n"));
                    aiSetActivity(e, ai, AI_ACTIVITY_WANDER);
                }
            }
            break;
        }

        case AI_PRIORITY_ACTION_STAND_STILL:{
            aiSetActivity(e, ai, AI_ACTIVITY_STAND_STILL);
            break;
        }

        case AI_PRIORITY_ACTION_WANDER:{
            ai->doNotRun = 1;    // this does fit in with ai->inputs.doNotRun, but makes the NPCs work better
            aiSetActivity(e, ai, AI_ACTIVITY_WANDER);
            break;
        }

        default:{
            aiSetActivity(e, ai, AI_ACTIVITY_STAND_STILL);
            break;
        }
    }
}

static void aiNPCPriorityDoBasicActions(Entity* e, const AIPriorityAction* action, const AIPriorityDoActionParams* params){
    AIVars* ai = ENTAI(e);
    int scared;

    if(ai->wasScared){
        scared = 1;
        ai->wasScared = 0;
    }
    else if(ai->isScared){
        U32 scaredDuration;
        
        switch(ai->npc.scaredType){
            xcase 1:
                scaredDuration = SEC_TO_ABS(5);
            xdefault:
                scaredDuration = SEC_TO_ABS(20);
        }
        
        scared = ABS_TIME_SINCE(ai->time.begin.afraidState) < scaredDuration;
    }
    else{
        scared = 0;
    }

    if(!ENTINFO(e).send_on_odd_send){
        entSetSendOnOddSend(e, 1);
    }

    if(scared){
        Vec3 pyr;
        Vec3 dir;

        if(!ai->isScared){
            // Reset scared timer.

            ai->time.begin.afraidState = ABS_TIME;
            ai->facingScarer = 1;
        }

        if(ai->facingScarer){
            // If I'm still facing scarer, see if I'm done.

            ai->followTarget.type = AI_FOLLOWTARGET_NONE;

            if(ABS_TIME_SINCE(ai->time.begin.afraidState) < SEC_TO_ABS(1)){
                // Face that the guy who scared me until it's time to run away.

                if(ai->whoScaredMe){
                    subVec3(ENTPOS(ai->whoScaredMe), ENTPOS(e), dir);
                    pyr[0] = 0;
                    pyr[1] = getVec3Yaw(dir);
                    pyr[2] = 0;
                    aiTurnBodyByPYR(e, ai, pyr);
                }
            }else{
                // Turn me away from who scared me.

                ai->facingScarer = 0;

                ai->whoScaredMe = NULL;

                copyVec3(ai->pyr, pyr);
                pyr[1] = addAngle(pyr[1], RAD(180));
                aiTurnBodyByPYR(e, ai, pyr);

                // Scrap my follow target.  This should send me back where I came from.

                //ai->npc.lastWireBeacon = NULL;
            }
        }
    }

    if(ai->isScared != scared){
        ai->isScared = scared;

        if(ai->npc.scaredType == 0){
            if(scared){
                SETB(e->seq->state, STATE_FEAR);
                SETB(e->seq->sticky_state, STATE_FEAR);
            }else{
                CLRB(e->seq->state, STATE_FEAR);
                CLRB(e->seq->sticky_state, STATE_FEAR);
            }
        }
    }else{
        switch(ai->npc.scaredType){
            xcase 1:
                ai->doNotRun = 1;
            xdefault:
                ai->doNotRun = !scared;
        }
    }

    if(!ai->isScared || !ai->facingScarer){
        aiSetActivity(e, ai, AI_ACTIVITY_WANDER);
    }else{
        aiSetActivity(e, ai, AI_ACTIVITY_STAND_STILL);
    }
}

void aiNPCPriorityDoActionFunc(Entity* e, const AIPriorityAction* action, const AIPriorityDoActionParams* params){
    AIVars* ai = ENTAI(e);

    if(params->switchedPriority){
        seqClearState(e->seq->sticky_state);

        ai->anchorRadius = 0;
        ai->doNotRun = ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : 0;

        if(ENTHIDE(e)){
            SET_ENTHIDE(e) = 0;
            e->draw_update = 1;
        }

        if(e->noDrawOnClient){
            e->noDrawOnClient = 0;
            e->draw_update = 1;
        }

        entSetSendOnOddSend(e, 1);
    }

    if(action){
        // Do the real priority actions.

        aiNPCPriorityDoRealActions(e, action, params);
    }else{
        // Act like a default dumb NPC.

        aiNPCPriorityDoBasicActions(e, action, params);
    }
}

static void aiNPCThink(Entity* e, AIVars* ai){
    ai->waitForThink = 0;

    aiBehaviorProcess(e, ai->base, &ai->base->behaviors);
}

void aiNPCDoActivity(Entity* e, AIVars* ai)
{
    switch(aiBehaviorGetActivity(e)){
        case AI_ACTIVITY_NPC_HOSTAGE:{
            aiNPCActivityHostage(e, ai);
            break;
                                     }

        case AI_ACTIVITY_THANK_HERO:{
            aiActivityThankHero(e, ai);
            break;
                                    }

        case AI_ACTIVITY_RUN_AWAY:{
            aiNPCActivityRunAway(e, ai);
            break;
                                  }

        case AI_ACTIVITY_RUN_TO_TARGET:{
            aiNPCActivityRunToTarget(e, ai);
            break;
                                       }

        default:{
            break;
                }
    }
}

void* aiNPCMsgHandlerDefault(Entity* e, AIMessage msg, AIMessageParams params){
    AIVars* ai = ENTAI(e);

    switch(msg){
        case AI_MSG_PATH_BLOCKED:{
            if(ai->preventShortcut){
                aiPathUpdateBlockedWaypoint(ai->navSearch->path.curWaypoint);

                ai->waitingForTargetToMove = 0;

                aiDiscardFollowTarget(e, "Ran into something.", false);
            }else{
                // If I'm stuck and I have already tried shortcutting,
                //   then redo the path but don't allow shortcutting.

                AI_LOG(    AI_LOG_PATH,
                        (e,
                        "I got stuck, trying not shortcutting.\n"));

                aiConstructPathToTarget(e);

                aiStartPath(e, 1, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);

                ai->waitingForTargetToMove = 1;
            }

            break;
        }

        case AI_MSG_FOLLOW_TARGET_REACHED:{
            aiDiscardFollowTarget(e, "Reached follow target.", true);

            ai->time.checkedForShortcut = 0;
            ai->doNotRun = ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : 0;

            break;
        }

        case AI_MSG_PATH_FINISHED:{
            aiDiscardFollowTarget(e, "Path finished.", false);

            break;
        }
    }

    return 0;
}

void aiNPC(Entity* e, AIVars* ai){
    //**************************************************************************************
    // Think.
    //**************************************************************************************

    ai->accThink -= e->timestep;

    if(ai->accThink <= 0){
        ai->waitForThink = 0;

        PERFINFO_AUTO_START("Think", 1);
            aiNPCThink(e, ai);
        PERFINFO_AUTO_STOP();

        do{
            ai->accThink += 15;
        }while(ai->accThink <= 0);
    }

    //**************************************************************************************
    // Move.
    //**************************************************************************************

    if(!ai->waitForThink){
        if(ai->npc.wireWanderMode){
            if(e->move_type == MOVETYPE_WIRE){
                PERFINFO_AUTO_START("MoveWire", 1);
                    aiNPCMoveWire(e, ai);
                PERFINFO_AUTO_STOP();
            }
        }else{
            PERFINFO_AUTO_START("Move", 1);
                switch(e->move_type){
                    case MOVETYPE_WIRE:{
                        break;
                    }

                    default:{
                        aiNPCMoveWalk(e, ai);
                        break;
                    }
                }
            PERFINFO_AUTO_STOP();
        }
    }
}

