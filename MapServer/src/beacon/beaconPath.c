
#include <limits.h>
#include <stddef.h>
#include "beaconPrivate.h"
#include "beaconPath.h"
#include "beaconConnection.h"
#include "beaconAStar.h"
#include "gridcoll/gridcoll.h"
#include "cmdparse/cmdserver.h"
#include "ai/entaivars.h"
#include "comm_game.h"
#include "svr/svr_base.h"
#include <utilitieslib/utils/utils.h>
#include "ai/entaiprivate.h"
#include "dbcomm/dbdoor.h"
#include "entity/entity.h"
#include <utilitieslib/components/EString.h>
#include "dbcomm/logcomm.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/rand.h>

#define DIST_TO_COST_SCALE (128)
#define MINHEIGHT (0)

static Array                staticBeaconSortArray;
static int                    staticBeaconSortMinY;
static int                    staticBeaconSortMaxY;
static AStarSearchData*        staticSearchData = NULL;
static EntityRef            staticDebugEntity;
static Packet*                staticDebugPacket = NULL;
static int                    staticAvailableBeaconCount = 0;

static struct{
    Entity*        ent;
    AIVars*        ai;
    Character*    pchar;
    F32            maxJumpHeight;
    F32            maxJumpXZDiffSQR;
    DoorEntry*    disappearInDoor;
    S32            canFly;
    U32            useEnterable;
    U32            pathLimited;
} pathFindEntity;

void beaconSetPathFindEntity(Entity* ent, F32 maxJumpHeight){
    pathFindEntity.ent                = ent;
    pathFindEntity.ai                = ent ? ENTAI(ent) : NULL;
    pathFindEntity.pchar            = ent ? ent->pchar : NULL;
    pathFindEntity.canFly            = pathFindEntity.ai ? pathFindEntity.ai->canFly : 0;
    pathFindEntity.maxJumpHeight    = maxJumpHeight;
    pathFindEntity.maxJumpXZDiffSQR    = SQR(maxJumpHeight * 2);
    pathFindEntity.disappearInDoor    = ent ? ENTAI(ent)->disappearInDoor : NULL;
    pathFindEntity.useEnterable        = ent ? ENTAI(ent)->useEnterable : 0;
    pathFindEntity.pathLimited        = ent ? ENTAI(ent)->pathLimited : 0;
}

void beaconSetPathFindEntityAsParameters(F32 maxJumpHeight, S32 canFly, U32 useEnterable){
    pathFindEntity.ent                = NULL;
    pathFindEntity.ai                = NULL;
    pathFindEntity.pchar            = NULL;
    pathFindEntity.canFly            = canFly;
    pathFindEntity.maxJumpHeight    = maxJumpHeight;
    pathFindEntity.maxJumpXZDiffSQR    = SQR(maxJumpHeight * 2);
    pathFindEntity.disappearInDoor    = NULL;
    pathFindEntity.useEnterable        = useEnterable;
    pathFindEntity.pathLimited        = 0;

    beaconCheckBlocksNeedRebuild();
}

void beaconSetDebugClient(Entity* e){
    staticDebugEntity = erGetRef(e);
}

//****************************************************
// NavPathWaypoint
//****************************************************

MP_DEFINE(NavPathWaypoint);

NavPathWaypoint* createNavPathWaypoint(){
    NavPathWaypoint* waypoint;

    MP_CREATE(NavPathWaypoint, 1000);

    waypoint = MP_ALLOC(NavPathWaypoint);

    return waypoint;
}

void destroyNavPathWaypoint(NavPathWaypoint* waypoint){
    if(!waypoint)
        return;
        
    MP_FREE(NavPathWaypoint, waypoint);
}

void navPathAddTail(NavPath* path, NavPathWaypoint* wp){
    wp->next = NULL;

    if(!path->head){
        path->head = path->tail = wp;
        wp->prev = NULL;
    }else{
        wp->prev = path->tail;
        path->tail->next = wp;
        path->tail = wp;
    }
    
    path->waypointCount++;
}

void navPathAddHead(NavPath* path, NavPathWaypoint* wp){
    wp->prev = NULL;
    
    if(!path->head){
        path->head = path->tail = wp;
        wp->next = NULL;
    }else{
        wp->next = path->head;
        path->head->prev = wp;
        path->head = wp;
    }

    path->waypointCount++;
}

//****************************************************
// NavSearch
//****************************************************

MP_DEFINE(NavSearch);

NavSearch* createNavSearch(){
    NavSearch* search;
    
    MP_CREATE(NavSearch, 100);
    
    search = MP_ALLOC(NavSearch);
    
    return search;
}

void destroyNavSearch(NavSearch* search){
    if(search){
        clearNavSearch(search);
    
        MP_FREE(NavSearch, search);
    }
}

void clearNavSearch(NavSearch* search){
    if(search){
        NavPathWaypoint* cur = search->path.head;
        
        search->searching = 0;
        search->path.head = search->path.tail = search->path.curWaypoint = NULL;
        search->path.waypointCount = 0;
        
        while(cur){
            NavPathWaypoint* next = cur->next;
            destroyNavPathWaypoint(cur);
            cur = next;
        }
    }
}

//****************************************************
// Beacon search callback functions.
//****************************************************

static Beacon*        staticTargetBeacon;
static Vec3            staticSourcePosition;
static Vec3            staticTargetPosition;

static int beaconSearchCostToTarget(    AStarSearchData* data,
                                        Beacon* beaconParent,
                                        Beacon* beacon,
                                        BeaconConnection* connectionToBeacon)
{
    Vec3 sourcePos;
    
    // Create the source position.

    vecX(sourcePos) = posX(beacon);
    vecZ(sourcePos) = posZ(beacon);
    
    if(pathFindEntity.canFly){
        if(beaconParent){
            vecY(sourcePos) = beaconParent->userFloat;
        }else{
            vecY(sourcePos) = beacon->userFloat;
        }
    }else{
        vecY(sourcePos) = posY(beacon);
    }

    // Get the positions in range of the connection.
    
    return DIST_TO_COST_SCALE * distance3(sourcePos, staticTargetPosition);
}

static int beaconSearchCost(AStarSearchData* data,
                            Beacon* sourceBeacon,
                            BeaconConnection* conn)
{
    Beacon* targetBeacon = conn->destBeacon;
    U32 timeWhenIWasBad = conn->timeWhenIWasBad;
    S32 searchDistance;

    if(pathFindEntity.pathLimited && !targetBeacon->pathLimitedAllowed)
        return INT_MAX;
    
    if(!conn->searchDistance){
        // Generate the quick distance lookup for ground connections.
        
        conn->searchDistance = distance3(posPoint(sourceBeacon), posPoint(targetBeacon)) * DIST_TO_COST_SCALE;

        if(!conn->searchDistance){
            conn->searchDistance = 1;
        }
    }
    
    if(pathFindEntity.canFly){
        // Find the distance from the flying position on the source beacon.
        
        F32        top_y;
        F32        bottom_y;
        Vec3     sourcePos;
        Vec3     targetPos;
        Vec3     sourceInRangePos;
        Vec3     targetInRangePos;
        
        setVec3(sourcePos,
                posX(sourceBeacon),
                sourceBeacon->userFloat,
                posZ(sourceBeacon));
        
        bottom_y = posY(sourceBeacon) + conn->minHeight;
        top_y = posY(sourceBeacon) + conn->maxHeight - MINHEIGHT;
        
        vecX(targetPos) = posX(targetBeacon);
        vecZ(targetPos) = posZ(targetBeacon);

        if(targetBeacon == data->targetNode){
            vecY(targetPos) = targetBeacon->userFloat;
        }else{
            vecY(targetPos) = CLAMPF32(vecY(sourcePos), bottom_y, top_y);
        }
        
        setVec3(sourceInRangePos,
                vecX(sourcePos),
                CLAMPF32(vecY(sourcePos), bottom_y, top_y),
                vecZ(sourcePos));
        
        setVec3(targetInRangePos,
                vecX(targetPos),
                CLAMPF32(vecY(targetPos), bottom_y, top_y),
                vecZ(targetPos));
        
        searchDistance = DIST_TO_COST_SCALE * (    fabs(vecY(targetPos) - vecY(targetInRangePos)) +
                                                fabs(vecY(sourcePos) - vecY(sourceInRangePos)) +
                                                distance3(sourceInRangePos, targetInRangePos));
    }
    else if(*(int*)&conn->minHeight){
        // I need to jump for this connection.
        
        F32 minHeight = conn->minHeight + sourceBeacon->floorDistance;
    
        searchDistance =    10 * DIST_TO_COST_SCALE +
                            (int)(minHeight * (2 * DIST_TO_COST_SCALE)) +
                            2 * conn->searchDistance;

        if(    targetBeacon->avoidNodes &&
            pathFindEntity.pchar &&
            aiShouldAvoidBeacon(pathFindEntity.ent, pathFindEntity.ai, targetBeacon))
        {
            searchDistance += DIST_TO_COST_SCALE * 50;
        }
    }
    else{
        // This is a ground connection.
        
        searchDistance = conn->searchDistance;

        // Check for avoid.
        
        if(    targetBeacon->avoidNodes &&
            pathFindEntity.pchar &&
            aiShouldAvoidBeacon(pathFindEntity.ent, pathFindEntity.ai, targetBeacon))
        {
            searchDistance += DIST_TO_COST_SCALE * 50;
        }
    }
        
    // If I was a bad connection in the last x seconds, factor that in.
    
    if(timeWhenIWasBad){
        U32 timeSinceBad = ABS_TIME_SINCE(timeWhenIWasBad);
        
        if(timeSinceBad < SEC_TO_ABS(5)){
            int inverse = SEC_TO_ABS(5) - timeSinceBad;
            
            searchDistance += inverse * 10 * DIST_TO_COST_SCALE;
        }
    }
    
    return searchDistance;
}

static int beaconSearchGetConnections(    AStarSearchData* data,
                                        Beacon* sourceBeacon,
                                        BeaconConnection*** connBuffer,
                                        Beacon*** nodeBuffer,
                                        int* position,
                                        int* count)
{
    BeaconConnection** connections = *connBuffer;
    Beacon** beacons = *nodeBuffer;
    BeaconBlock* block;
    int start = *position;
    void** storage;
    int size;
    int i;
    int connCount;
    int max = *count;
    int sourceHeight = beaconGetFloorDistance(sourceBeacon);

    // connCount is the count of returned connections.
    
    connCount = 0;
        
    block = sourceBeacon->block;
    
    // Return ground connections sub-array if still in that range.
    
    if(start < sourceBeacon->gbConns.size){
        size = sourceBeacon->gbConns.size;
        storage = sourceBeacon->gbConns.storage;

        for(i = start; i < size; i++){
            BeaconConnection* conn = storage[i];
            Beacon* targetBeacon = conn->destBeacon;
            
            if(targetBeacon->block->searchInstance == beacon_state.beaconSearchInstance){
                connections[connCount] = conn;
                beacons[connCount] = targetBeacon;

                if(++connCount == max){
                    break;
                }
            }
        }
        
        if(connCount){
            *position = i;
            *count = connCount;

            return 1;
        }
        
        start = 0;
    }else{
        start -= sourceBeacon->gbConns.size;
    }

    // Return raised connections sub-array if still in that range.
    
    if(start < sourceBeacon->rbConns.size){
        size = sourceBeacon->rbConns.size;
        storage = sourceBeacon->rbConns.storage;

        for(i = start; i < size; i++){
            BeaconConnection* conn = storage[i];
            Beacon* targetBeacon = conn->destBeacon;
            
            if(targetBeacon->block->searchInstance == beacon_state.beaconSearchInstance){
                if(    !pathFindEntity.ent
                    ||
                    pathFindEntity.canFly
                    ||
                    pathFindEntity.maxJumpHeight >= conn->minHeight + sourceHeight &&
                    pathFindEntity.maxJumpXZDiffSQR >= distance3SquaredXZ(posPoint(sourceBeacon), posPoint(targetBeacon)))
                {
                    if(    MINHEIGHT <= 0 ||
                        conn->maxHeight - conn->minHeight >= MINHEIGHT)
                    {
                        connections[connCount] = conn;
                        beacons[connCount] = targetBeacon;

                        if(++connCount == max){
                            break;
                        }
                    }
                }
            }
        }
        
        if(connCount){
            *position = sourceBeacon->gbConns.size + i;
            *count = connCount;
            return 1;
        }
    }
    
    return 0;
}

static void beaconSearchOutputPath(    AStarSearchData* data,
                                    AStarInfo* tailInfo)
{
    NavSearch*            search = data->userData;
    AStarInfo*            curInfo = tailInfo;
    BeaconConnection*    connFromMe = NULL;
    Beacon*                destBeacon = search->dst;
    S32                    first = 1;
    F32                    last_y = posY(search->src);
    
    while(curInfo){
        AStarInfo*            parentInfo;
        NavPathWaypoint*    wp;
        Beacon*                beacon;
        BeaconConnection*    connToMe;
        Beacon*                parentBeacon = NULL;
        F32                 bottom_y;
        F32                 top_y;
        F32                 beacon_y;
        S32                    makeWaypoint = 1;
        
        parentInfo = curInfo->parentInfo;

        beacon = curInfo->ownerNode;
        connToMe = curInfo->connFromPrevNode;
                
        beacon_y = pathFindEntity.canFly ? beacon->userFloat : posY(beacon);
        
        if(pathFindEntity.canFly){
            // Create the destination waypoint.
            
            if(beacon == destBeacon && beacon_y != search->destinationHeight){
                wp = createNavPathWaypoint();
                
                wp->beacon = beacon;
                
                wp->connectType = NAVPATH_CONNECT_FLY;

                vecX(wp->pos) = posX(beacon);
                vecY(wp->pos) = search->destinationHeight;
                vecZ(wp->pos) = posZ(beacon);
                
                last_y = vecY(wp->pos);
                
                navPathAddHead(&search->path, wp);
            }
            
            // Add the waypoint that's in range of connToMe.

            if(connToMe && connToMe->minHeight){
                parentBeacon = parentInfo->ownerNode;
                
                bottom_y = posY(parentBeacon) + connToMe->minHeight;
                top_y = posY(parentBeacon) + connToMe->maxHeight - MINHEIGHT;
                
                if(last_y >= bottom_y && last_y <= top_y){
                    makeWaypoint = 0;
                }
            }
        }
                
        if(makeWaypoint){
            wp = createNavPathWaypoint();

            wp->beacon = beacon;
            wp->connectionToMe = connToMe;
            
            vecX(wp->pos) = posX(beacon);
            vecY(wp->pos) = beacon_y;
            vecZ(wp->pos) = posZ(beacon);
            
            if(connToMe && *(int*)&connToMe->minHeight){
                if(pathFindEntity.canFly){
                    wp->connectType = NAVPATH_CONNECT_FLY;
                }else{
                    wp->connectType = NAVPATH_CONNECT_JUMP;

                    vecY(wp->pos) -= beaconGetFloorDistance(beacon);
                }
            }else{
                if(    //pathFindEntity.ai->isFlying ||
                    pathFindEntity.canFly && beacon->userFloat - posY(beacon) > 5)
                {
                    assert(!connToMe || connToMe->minHeight > 0);
                    wp->connectType = NAVPATH_CONNECT_FLY;
                }else{
                    wp->connectType = NAVPATH_CONNECT_GROUND;
                    
                    vecY(wp->pos) = posY(beacon) - beaconGetFloorDistance(beacon);
                }
            }
            
            navPathAddHead(&search->path, wp);
        }
        
        // Add the waypoint over parent beacon that's clamped to connToMe, if necessary.

        if(pathFindEntity.canFly){
            if(connToMe && connToMe->minHeight){
                beacon_y = parentBeacon->userFloat;
                
                bottom_y = posY(parentBeacon) + connToMe->minHeight;
                top_y = posY(parentBeacon) + connToMe->maxHeight - MINHEIGHT;
                
                wp = createNavPathWaypoint();
                
                wp->beacon = parentBeacon;
                
                wp->connectType = NAVPATH_CONNECT_FLY;
                
                vecX(wp->pos) = posX(parentBeacon);
                vecY(wp->pos) = CLAMPF32(beacon_y, bottom_y, top_y);
                vecZ(wp->pos) = posZ(parentBeacon);
                
                last_y = vecY(wp->pos);

                navPathAddHead(&search->path, wp);
            }
            else if(parentInfo){
                BeaconConnection* connToParent = parentInfo->connFromPrevNode;
                
                if(connToParent && connToParent->minHeight){
                    parentBeacon = parentInfo->ownerNode;

                    wp = createNavPathWaypoint();
                    
                    wp->beacon = parentBeacon;
                    wp->connectionToMe = connToParent;
                    
                    wp->connectType = NAVPATH_CONNECT_FLY;
                    
                    vecX(wp->pos) = posX(parentBeacon);
                    vecY(wp->pos) = posY(parentBeacon);
                    vecZ(wp->pos) = posZ(parentBeacon);
                    
                    last_y = vecY(wp->pos);

                    navPathAddHead(&search->path, wp);
                }
            }
        }
        
        // And go to the parent node.
        
        first = 0;
                
        curInfo = parentInfo;
    }
    
    search->path.curWaypoint = search->path.head;
}

static void beaconSearchCloseNode(    Beacon* parentBeacon,
                                    BeaconConnection* conn,
                                    Beacon* closedBeacon)
{
    F32 top_y, bottom_y;
    F32 cur_y;
    F32 beacon_y;
    
    if(!parentBeacon)
        return;
        
    if(!*(int*)&conn->minHeight){
        closedBeacon->userFloat = posY(closedBeacon);
        return;
    }
        
    cur_y = parentBeacon->userFloat;
    
    beacon_y = posY(parentBeacon);
    
    bottom_y = beacon_y + conn->minHeight;
    top_y = beacon_y + conn->maxHeight;//- MINHEIGHT;
    
    closedBeacon->userFloat = CLAMPF32(cur_y, bottom_y, top_y);
}

static const char* beaconSearchGetNodeInfo(    Beacon* beacon,
                                            Beacon* parentBeacon,
                                            BeaconConnection* conn)
{
    static char* buffer = NULL;
    
    if(!buffer)
        buffer = malloc(1000);
    
    if(parentBeacon){
        sprintf(buffer,
                "(%1.f, %1.f, %1.f) --> (%1.f, %1.f, %1.f)\t[%1.f,%1.f]",
                posX(parentBeacon), parentBeacon->userFloat, posZ(parentBeacon),
                posX(beacon), beacon->userFloat, posZ(beacon),
                conn->minHeight,
                conn->maxHeight);
    }else{
        sprintf(buffer, "(%1.f, %1.f, %1.f)", posX(beacon), beacon->userFloat, posZ(beacon));
    }
    
    return buffer;
}

void beaconSearchReportClosedInfo(    AStarInfo* closedInfo,
                                    int checkedConnectionCount)
{
    Beacon* beacon = closedInfo->ownerNode;
    Packet* pak = staticDebugPacket;
    int i;
    
    // New set.
    
    pktSendBits(pak, 1, 1);
    
    pktSendBitsPack(pak, 10, checkedConnectionCount);
    pktSendBitsPack(pak, 10, closedInfo->costSoFar);
    pktSendBitsPack(pak, 10, closedInfo->totalCost);
    
    for(i = 0; i < beacon->gbConns.size; i++){
        BeaconConnection* conn = beacon->gbConns.storage[i];
        Beacon* b = conn->destBeacon;
        AStarInfo* info = b->astarInfo;
        
        pktSendBits(pak, 1, 1);

        if(info){
            pktSendBits(pak, 1, 1);
            pktSendBitsPack(pak, 8, info->queueIndex);
            pktSendBitsPack(pak, 24, info->totalCost);
            pktSendBitsPack(pak, 24, info->costSoFar);
        }else{
            pktSendBits(pak, 1, 0);
        }

        pktSendF32(pak, posX(b));
        pktSendF32(pak, posY(b));
        pktSendF32(pak, posZ(b));
    }

    for(i = 0; i < beacon->rbConns.size; i++){
        BeaconConnection* conn = beacon->rbConns.storage[i];
        Beacon* b = conn->destBeacon;
        AStarInfo* info = b->astarInfo;
        
        pktSendBits(pak, 1, 1);
        
        if(info){
            pktSendBits(pak, 1, 1);
            pktSendBitsPack(pak, 8, info->queueIndex);
            pktSendBitsPack(pak, 24, info->totalCost);
            pktSendBitsPack(pak, 24, info->costSoFar);
        }else{
            pktSendBits(pak, 1, 0);
        }

        pktSendF32(pak, posX(b));
        pktSendF32(pak, posY(b));
        pktSendF32(pak, posZ(b));
    }

    pktSendBits(pak, 1, 0);

    while(closedInfo){
        Beacon* b = closedInfo->ownerNode;
        BeaconConnection* conn = closedInfo->connFromPrevNode;
        
        pktSendBits(pak, 1, 1);

        pktSendF32(pak, posX(b));
        pktSendF32(pak, posY(b));
        pktSendF32(pak, posZ(b));
        pktSendF32(pak, pathFindEntity.canFly ? b->userFloat : 0);
        
        if(conn){
            pktSendBits(pak, 1, 1);
            
            pktSendIfSetF32(pak, conn->minHeight);
            pktSendIfSetF32(pak, conn->maxHeight);
        }else{
            pktSendBits(pak, 1, 0);
        }
        
        closedInfo = closedInfo->parentInfo;
    }

    pktSendBits(pak, 1, 0);
}

//****************************************************
// BeaconBlock search callback functions.
//****************************************************

static struct {
    BeaconBlock* sourceBlock;
    BeaconBlock* targetBlock;
} blockSearch;

static int beaconBlockSearchCostToTarget(    AStarSearchData* data,
                                            BeaconBlock* blockParent,
                                            BeaconBlock* block,
                                            BeaconBlockConnection* connectionToBlock)
{
    if(pathFindEntity.pathLimited && !block->pathLimitedAllowed)
        return INT_MAX;

    if(pathFindEntity.canFly){
        Vec3 blockPos;
        
        blockPos[0] = vecX(block->pos);
        blockPos[2] = vecZ(block->pos);
        
        if(blockParent){
            F32 minY = vecY(blockParent->pos) + connectionToBlock->minHeight;
            F32 maxY = vecY(blockParent->pos) + connectionToBlock->maxHeight;
            
            blockPos[1] = CLAMPF32(blockParent->searchY, minY, maxY);
        }else{
            blockPos[1] = block->searchY;
        }
        
        return distance3(blockPos, staticTargetPosition);
    }else{
        return distance3(block->pos, staticTargetPosition);
    }
}

static int beaconBlockSearchCost(    AStarSearchData* data,
                                    BeaconBlock* sourceBlock,
                                    BeaconBlockConnection* conn)
{
    BeaconBlock* targetBlock = conn->destBlock;
    
    if(pathFindEntity.canFly){
        Vec3 sourcePos = {vecX(sourceBlock->pos), sourceBlock->searchY, vecZ(sourceBlock->pos)};
        F32 minY = vecY(sourceBlock->pos) + conn->minHeight;
        F32 maxY = vecY(sourceBlock->pos) + conn->maxHeight;
        Vec3 targetPos = {vecX(targetBlock->pos), CLAMPF32(vecY(sourcePos), minY, maxY), vecZ(targetBlock->pos)};
        
        return distance3(sourcePos, targetPos);
    }else{
        F32* srcPos;
        F32* dstPos;

        if(targetBlock == blockSearch.targetBlock){
            dstPos = staticTargetPosition;
        }else{
            dstPos = targetBlock->pos;
        }
        
        if(sourceBlock == blockSearch.sourceBlock){
            srcPos = staticSourcePosition;
        }else{
            srcPos = sourceBlock->pos;
        }
        
        return distance3(srcPos, dstPos);
    }
}

static int beaconBlockSearchGetConnections(    AStarSearchData* data,
                                            BeaconBlock* subBlock,
                                            BeaconBlockConnection*** connBuffer,
                                            BeaconBlock*** nodeBuffer,
                                            int* position,
                                            int* count)
{
    int start = *position;
    BeaconBlockConnection** connections = *connBuffer;
    BeaconBlock** blocks = *nodeBuffer;
    int i;
    int j;
    
    // Return ground connections sub-array if still in that range.
    
    if(start < subBlock->gbbConns.size){
        if(*count > subBlock->gbbConns.size - start){
            *count = subBlock->gbbConns.size - start;
        }
        
        *position += *count;
        
        *connBuffer = (BeaconBlockConnection**)subBlock->gbbConns.storage + start;
        connections = *connBuffer;
        
        j = *count;
        
        for(i = 0; i < j; i++){
            blocks[i] = connections[i]->destBlock;
        }
        
        return 1;
    }

    // Return raised connections sub-array if still in that range.

    start -= subBlock->gbbConns.size;
    
    if(start < subBlock->rbbConns.size){
        int size = subBlock->rbbConns.size;
        int max = *count;

        if(!pathFindEntity.ent || pathFindEntity.canFly){
            for(i = start, j = 0; i < size; i++){
                BeaconBlockConnection* conn = subBlock->rbbConns.storage[i];
                
                connections[j] = conn;
                blocks[j] = conn->destBlock;
                    
                if(++j == max){
                    break;
                }
            }
        }else{
            for(i = start, j = 0; i < size; i++){
                BeaconBlockConnection* conn = subBlock->rbbConns.storage[i];
                
                if(pathFindEntity.maxJumpHeight >= conn->minJumpHeight){
                    connections[j] = conn;
                    blocks[j] = conn->destBlock;
                    
                    if(++j == max){
                        break;
                    }
                }
            }
        }
        
        if(j){
            *count = j;
            *position = subBlock->gbbConns.size + i;
            return 1;
        }
    }
    
    return 0;
}

static void beaconBlockSearchOutputPath(    AStarSearchData* data,
                                            AStarInfo* tailInfo)
{
    AStarInfo* blockTail;
        
    // Now there's a block path, determine which block each previous block should be heading for.
    // Do this by going through the block path until arriving at a block that is the opposite increment
    // from the previous block, meaning that if the previous block is FARTHER away from the destination
    // than two blocks back is, AND that the current block is CLOSER than the previous block to the
    // destination, then we've found a turning point in the path.
    
    // Set the search instance for all the valid blocks.
    
    beacon_state.beaconSearchInstance++;

    staticAvailableBeaconCount = 0;
    
    if(staticDebugPacket){
        Packet* pak = staticDebugPacket;
        
        for(blockTail = tailInfo; blockTail; blockTail = blockTail->parentInfo){
            BeaconBlock* block = blockTail->ownerNode;
            int i;
            
            pktSendBits(pak, 1, 1);
            
            pktSendF32(pak, block->pos[0]);
            pktSendF32(pak, block->pos[1]);
            pktSendF32(pak, block->pos[2]);
            
            pktSendF32(pak, block->searchY);
            
            pktSendBitsPack(pak, 8, block->beaconArray.size);
            
            for(i = 0; i < block->beaconArray.size; i++){
                Beacon* b = block->beaconArray.storage[i];

                pktSendF32(pak, posX(b));
                pktSendF32(pak, posY(b));
                pktSendF32(pak, posZ(b));
            }
        }

        pktSendBits(pak, 1, 0);
    }

    PERFINFO_AUTO_START("Mark Blocks", 1);
        for(blockTail = tailInfo; blockTail; blockTail = blockTail->parentInfo){
            BeaconBlock* block = blockTail->ownerNode;
            int i;
            int connCount;
            BeaconBlockConnection** conns;
            
            if(block->searchInstance != beacon_state.beaconSearchInstance){
                staticAvailableBeaconCount += block->beaconArray.size;
                block->searchInstance = beacon_state.beaconSearchInstance;
            }
            
            // Set the search instance for all the neighbor blocks to give the path that fuzzy feeling.
            
            connCount = block->gbbConns.size;
            conns = (BeaconBlockConnection**)block->gbbConns.storage;
            
            for(i = 0; i < connCount; i++){
                BeaconBlockConnection* conn = conns[i];

                if(conn->destBlock->searchInstance != beacon_state.beaconSearchInstance){
                    staticAvailableBeaconCount += conn->destBlock->beaconArray.size;
                    conn->destBlock->searchInstance = beacon_state.beaconSearchInstance;
                }
            }
            
            connCount = block->rbbConns.size;
            conns = (BeaconBlockConnection**)block->rbbConns.storage;

            for(i = 0; i < connCount; i++){
                BeaconBlockConnection* conn = conns[i];

                if(conn->destBlock->searchInstance != beacon_state.beaconSearchInstance){
                    staticAvailableBeaconCount += conn->destBlock->beaconArray.size;
                    conn->destBlock->searchInstance = beacon_state.beaconSearchInstance;
                }
            }
        }
    PERFINFO_AUTO_STOP();
}

static void beaconBlockSearchCloseNode(    BeaconBlock* parentBlock,
                                        BeaconBlockConnection* conn,
                                        BeaconBlock* closedBlock)
{
    F32 curY;
    F32 minY;
    F32 maxY;
    
    if(parentBlock){
        curY = parentBlock->searchY;
        minY = vecY(parentBlock->pos) + conn->minHeight;
        maxY = vecY(parentBlock->pos) + conn->maxHeight;

        closedBlock->searchY = CLAMPF32(curY, minY, maxY);
    }
}

//****************************************************
// Beacon galaxy search callback functions.
//****************************************************

static int beaconGalaxySearchCostToTarget(    AStarSearchData* data,
                                            BeaconBlock* galaxyParent,
                                            BeaconBlock* galaxy,
                                            BeaconBlockConnection* connectionToGalaxy)
{
    if(blockSearch.targetBlock == galaxy)
        return 0;
    
    return 1;
}

static int beaconGalaxySearchCost(    AStarSearchData* data,
                                    BeaconBlock* sourceGalaxy,
                                    BeaconBlockConnection* conn)
{
    BeaconBlock* destGalaxy = conn->destBlock;

    if(blockSearch.targetBlock == destGalaxy)
        return 0;
    
    return 1;
}

static int beaconGalaxySearchGetConnections(AStarSearchData* data,
                                            BeaconBlock* galaxy,
                                            BeaconBlockConnection*** connBuffer,
                                            BeaconBlock*** nodeBuffer,
                                            int* position,
                                            int* count)
{
    int start = *position;
    BeaconBlockConnection** connections = *connBuffer;
    BeaconBlock** destBlocks = *nodeBuffer;
    int i;
    int connCount;

    // Return ground connections sub-array if still in that range.
    
    if(start < galaxy->gbbConns.size){
        if(*count > galaxy->gbbConns.size - start){
            *count = galaxy->gbbConns.size - start;
        }
        
        *position += *count;
        
        *connBuffer = (BeaconBlockConnection**)galaxy->gbbConns.storage + start;
        connections = *connBuffer;
        
        connCount = *count;
        
        for(i = 0; i < connCount; i++){
            destBlocks[i] = connections[i]->destBlock;
        }
        
        return 1;
    }

    // Return raised connections sub-array if still in that range.

    if(pathFindEntity.maxJumpHeight > 0){
        start -= galaxy->gbbConns.size;
        
        if(start < galaxy->rbbConns.size){
            int size = galaxy->rbbConns.size;
            int max = *count;
            
            if(pathFindEntity.canFly){
                for(i = start, connCount = 0; i < size; i++){
                    BeaconBlockConnection* conn = galaxy->rbbConns.storage[i];
                    
                    // All connections are okay for flying.
                    
                    connections[connCount] = conn;
                    destBlocks[connCount] = conn->destBlock;
                    
                    if(++connCount == max){
                        break;
                    }
                }
            }else{
                for(i = start, connCount = 0; i < size; i++){
                    BeaconBlockConnection* conn = galaxy->rbbConns.storage[i];
                    
                    // Check if the entity's maxJumpHeight is >= the connection minJumpHeight.
                    
                    if(pathFindEntity.maxJumpHeight >= conn->minJumpHeight){
                        connections[connCount] = conn;
                        destBlocks[connCount] = conn->destBlock;
                        
                        if(++connCount == max){
                            break;
                        }
                    }
                }
            }
            
            if(connCount){
                *count = connCount;
                *position = galaxy->gbbConns.size + i;
                return 1;
            }
        }
    }
        
    return 0;
}

//****************************************************
// Beacon cluster search callback functions.
//****************************************************

static int beaconClusterSearchCostToTarget(    AStarSearchData* data,
                                            BeaconBlock* clusterParent,
                                            BeaconBlock* cluster,
                                            BeaconClusterConnection* connectionToGalaxy)
{
    if(blockSearch.targetBlock == cluster)
        return 0;
    
    return 1;
}

static int beaconClusterSearchCost(    AStarSearchData* data,
                                    BeaconBlock* sourceCluster,
                                    BeaconClusterConnection* conn)
{
    BeaconBlock* targetCluster = conn->targetCluster;

    if(blockSearch.targetBlock == targetCluster)
        return 0;
    
    return 1;
}

static int beaconClusterSearchGetConnections(    AStarSearchData* data,
                                                BeaconBlock* cluster,
                                                BeaconClusterConnection*** connBuffer,
                                                BeaconBlock*** nodeBuffer,
                                                int* position,
                                                int* count)
{
    int start = *position;
    BeaconClusterConnection** connections = *connBuffer;
    BeaconBlock** targetClusters = *nodeBuffer;
    int i;
    int connCount;

    if(start < cluster->gbbConns.size){
        if(*count > cluster->gbbConns.size - start){
            *count = cluster->gbbConns.size - start;
        }
        
        *position += *count;
        
        *connBuffer = (BeaconClusterConnection**)cluster->gbbConns.storage + start;
        connections = *connBuffer;
        
        connCount = *count;
        
        for(i = 0; i < connCount; i++){
            targetClusters[i] = connections[i]->targetCluster;
        }
        
        return 1;
    }

    return 0;
}

static struct {
    BeaconClusterConnection* connToNextCluster;
} clusterSearch;

static void beaconClusterSearchOutputPath(    AStarSearchData* data,
                                            AStarInfo* tailInfo)
{
    while(    tailInfo->parentInfo &&
            tailInfo->parentInfo->ownerNode != blockSearch.sourceBlock)
    {
        tailInfo = tailInfo->parentInfo;
    }

    if(tailInfo->parentInfo && tailInfo->parentInfo->ownerNode == blockSearch.sourceBlock){
        clusterSearch.connToNextCluster = tailInfo->connFromPrevNode;
    }
}

//****************************************************
// Non-astar-callback functions.
//****************************************************

static int beaconVisibleFromGroundPos(Beacon* b, Vec3 pos, float radius){
    CollInfo info;
    Vec3 beaconPos;
    Vec3 pos2;

    if(!b->block)
        return 0;

    copyVec3(posPoint(b), beaconPos);
    copyVec3(pos, pos2);

    vecY(beaconPos) += 3;
    vecY(pos2) += 3;

    if(distance3Squared(pos2, beaconPos) > SQR(combatBeaconGridBlockSize))
        return 0;
        
    if(collGrid(NULL, pos2, beaconPos, &info, radius, COLL_NOTSELECTABLE | COLL_HITANY | COLL_BOTHSIDES))
        return 0;
    
    return 1;
}

static void beaconGetClosestAirPos(Beacon* b, const Vec3 pos, Vec3 posOut){
    float pos_y = posY(b);
    float top_y = pos_y + beaconGetCeilingDistance(b);
    float bottom_y = pos_y - beaconGetFloorDistance(b) + 0.2;

    if(top_y > pos_y + 1.0)
        top_y -= 1.0;
    
    vecX(posOut) = posX(b);
    vecY(posOut) = CLAMPF32(vecY(pos), bottom_y, top_y);
    vecZ(posOut) = posZ(b);
}

static int beaconVisibleFromAirPos(Beacon* b, Vec3 pos, float radius){
    CollInfo info;
    Vec3 beaconPos;
    Vec3 fromPos;

    if(!b->block)
        return 0;
        
    copyVec3(pos, fromPos);
    
    vecY(fromPos) += 3;
        
    beaconGetClosestAirPos(b, fromPos, beaconPos);
        
    if(distance3SquaredXZ(fromPos, beaconPos) > SQR(combatBeaconGridBlockSize))
        return 0;
        
    if(collGrid(NULL, fromPos, beaconPos, &info, radius, COLL_NOTSELECTABLE | COLL_HITANY | COLL_BOTHSIDES))
        return 0;
    
    return 1;
}

static int __cdecl sortBeaconArrayByDistanceHelper(const Beacon** b1, const Beacon** b2){
    if((*b1)->userFloat > (*b2)->userFloat)
        return 1;
    else if((*b1)->userFloat == (*b2)->userFloat)
        return 0;
    else
        return -1;
}

static void sortBeaconArrayByDistance(Array* array, const Vec3 sourcePos, int calcDistances){
    if(calcDistances){
        int i;
        Vec3 diff;
        
        for(i = 0; i < array->size; i++){
            Beacon* beacon = array->storage[i];
            
            subVec3(sourcePos, posPoint(beacon), diff);
            
            beacon->userFloat = lengthVec3Squared(diff);
        }
    }
        
    PERFINFO_AUTO_START("beaconSort", 1);

    qsort(array->storage, array->size, sizeof(array->storage[0]), sortBeaconArrayByDistanceHelper);
    
    PERFINFO_AUTO_STOP();
}

static void makeSortedNearbyBeaconArray(Array* array, const Vec3 sourcePos, float searchRadius){
    int min_block[3];
    int max_block[3];
    int x, z;
    
    array->size = 0;
    
    min_block[0] = beaconMakeGridBlockCoord(sourcePos[0] - searchRadius);
    max_block[0] = beaconMakeGridBlockCoord(sourcePos[0] + searchRadius);
    min_block[2] = beaconMakeGridBlockCoord(sourcePos[2] - searchRadius);
    max_block[2] = beaconMakeGridBlockCoord(sourcePos[2] + searchRadius);
        
    if(pathFindEntity.canFly){
        // I can fly, so anything in my column of blocks is okay.
        
        min_block[1] = staticBeaconSortMinY;
        max_block[1] = staticBeaconSortMaxY;
    }else{
        // If I can't fly, then stick to beacons that are within 100 feet.
        
        min_block[1] = beaconMakeGridBlockCoord(sourcePos[1] - searchRadius);
        max_block[1] = beaconMakeGridBlockCoord(sourcePos[1] + searchRadius);
    }

    for(x = min_block[0]; x <= max_block[0]; x++){
        for(z = min_block[2]; z <= max_block[2]; z++){
            BeaconBlock* block;
            int y;
            
            for(y = min_block[1]; y <= max_block[1]; y++){
                block = beaconGetGridBlockByCoords(x, y, z, 0);
                                                                        
                // Add all beacons that are within the size of a grid block.
                
                if(block){
                    int j;
                    
                    for(j = 0; j < block->beaconArray.size; j++){
                        Beacon* beacon = block->beaconArray.storage[j];
                        
                        if(!beacon->gbConns.size && !beacon->rbConns.size){
                            continue;
                        }

                        if(pathFindEntity.pathLimited && !beacon->pathLimitedAllowed){
                            continue;
                        }
                        
                        if(pathFindEntity.canFly){
                            // If I'm flying, then project to the nearest point on the vertical beacon line.
                            
                            Vec3 pos;

                            beaconGetClosestAirPos(beacon, sourcePos, pos);
                            
                            beacon->userFloat = distance3Squared(pos, sourcePos);
                        }else{
                            beacon->userFloat = distance3Squared(posPoint(beacon), sourcePos);
                        }
                                    
                        if(beacon->userFloat <= SQR(searchRadius) && array->size < array->maxSize){
                            array->storage[array->size++] = beacon;
                        }

                        if(beacon->pathsBlockedToMe){
                            beacon->userFloat *= SQR(beacon->pathsBlockedToMe);
                        }
                    }
                }
            }
        }
    }
    
    sortBeaconArrayByDistance(array, sourcePos, 0);
}

struct {
    BeaconBlock*                sourceCluster;
    BeaconBlock*                sourceGalaxy;
    Vec3                        sourcePos;
    Beacon*                        sourceBeacon;
    U32                            wantClusterConnect : 1;        // Target beacon is in a different cluster.
    Beacon*                        clusterConnTargetBeacon;    // Beacon on other side of a cluster connection.
    BeaconDynamicConnection*    dynConnToTarget;
} beaconFind;

static void initStaticSearchData(){
    if(!staticSearchData){
        staticSearchData = createAStarSearchData();
        initAStarSearchData(staticSearchData);
    }
}

static Beacon* getClusterConnect(    BeaconBlock* sourceCluster,
                                    BeaconBlock* targetCluster)
{
    NavSearchFunctions clusterSearchFunctions = {
        (NavSearchCostToTargetFunction)        beaconClusterSearchCostToTarget,
        (NavSearchCostFunction)                beaconClusterSearchCost,
        (NavSearchGetConnectionsFunction)    beaconClusterSearchGetConnections,
        (NavSearchOutputPath)                beaconClusterSearchOutputPath,
        (NavSearchCloseNode)                NULL,
        (NavSearchGetNodeInfo)                NULL,
        (NavSearchReportClosedInfo)            NULL,
    };

    if(    !sourceCluster ||
        !targetCluster)
    {
        return NULL;
    }

    initStaticSearchData();

    staticSearchData->sourceNode = sourceCluster;
    staticSearchData->targetNode = targetCluster;
    staticSearchData->searching = 0;
    staticSearchData->pathWasOutput = 0;
    staticSearchData->steps = 0;
    staticSearchData->nodeAStarInfoOffset = offsetof(BeaconBlock, astarInfo);

    clusterSearch.connToNextCluster = NULL;

    blockSearch.sourceBlock = sourceCluster;
    blockSearch.targetBlock = targetCluster;
    
    beaconFind.clusterConnTargetBeacon = NULL;
    beaconFind.dynConnToTarget = NULL;

    PERFINFO_AUTO_START("A* cluster", 1);
        AStarSearch(staticSearchData, &clusterSearchFunctions);
    PERFINFO_AUTO_STOP();

    if(clusterSearch.connToNextCluster){
        beaconFind.clusterConnTargetBeacon = clusterSearch.connToNextCluster->target.beacon;
        
        beaconFind.dynConnToTarget = clusterSearch.connToNextCluster->dynConnToTarget;
        
        return clusterSearch.connToNextCluster->source.beacon;
    }

    return NULL;
}

static Beacon* checkForClusterConnection(BeaconBlock* targetGalaxy){
    Beacon* result = getClusterConnect(beaconFind.sourceCluster, targetGalaxy->cluster);

    if(    result && 
        beaconGalaxyPathExists(    beaconFind.sourceBeacon,
                                result,
                                pathFindEntity.maxJumpHeight,
                                pathFindEntity.canFly))
    {
        beaconFind.wantClusterConnect = 1;
        return result;
    }

    return NULL;
}

static Beacon* getBestBeaconGround(F32 curRadius, int* outFoundLOS){
    int i;
    
    for(i = 0; i < staticBeaconSortArray.size; i++){
        Beacon* targetBeacon = staticBeaconSortArray.storage[i];
        BeaconBlock* targetBlock;
        BeaconBlock* targetGalaxy;
        int good;
        
        if(    !targetBeacon
            ||
            !targetBeacon->gbConns.size && 
            !targetBeacon->rbConns.size)
        {
            continue;
        }

        targetBlock = targetBeacon->block;
        targetGalaxy = targetBlock ? targetBlock->galaxy : NULL;
        
        if( targetGalaxy &&
            (    targetGalaxy->searchInstance == beacon_state.galaxySearchInstance &&
                !targetGalaxy->isGoodForSearch
                ||
                !pathFindEntity.useEnterable &&
                beaconFind.sourceCluster &&
                targetGalaxy->cluster != beaconFind.sourceCluster))
        {
            // This galaxy was already found to be bad, so ignore it.
            
            staticBeaconSortArray.storage[i] = NULL;
            
            continue;
        }
        
        PERFINFO_AUTO_START("ground", 1);
            PERFINFO_AUTO_START("vis", 1);
                good = beaconVisibleFromGroundPos(targetBeacon, beaconFind.sourcePos, curRadius);
            PERFINFO_AUTO_STOP();
        
            if(!good){
                PERFINFO_AUTO_STOP();
                continue;
            }
            
            *outFoundLOS = 1;
        
            if(    (    !beaconFind.sourceCluster ||
                    targetGalaxy->cluster == beaconFind.sourceCluster)
                &&
                (    !beaconFind.sourceBeacon ||
                    !beaconFind.sourceGalaxy ||
                    beaconGalaxyPathExists(    beaconFind.sourceBeacon,
                                            targetBeacon,
                                            pathFindEntity.maxJumpHeight,
                                            pathFindEntity.canFly)))
            {
                PERFINFO_AUTO_STOP();
                return targetBeacon;
            }
            else if(pathFindEntity.useEnterable && 
                    targetGalaxy &&
                    beaconFind.sourceCluster &&
                    targetGalaxy->cluster != beaconFind.sourceCluster)
            {
                // Just make a path to the cluster connection.
                
                Beacon* result = checkForClusterConnection(targetGalaxy);
                
                if(result){
                    PERFINFO_AUTO_STOP();
                    return result;
                }else{
                    staticBeaconSortArray.storage[i] = NULL;
                }
            }
            else{
                if(targetGalaxy){
                    // Mark galaxy bad for this search instance.

                    targetGalaxy->searchInstance = beacon_state.galaxySearchInstance;
                    targetGalaxy->isGoodForSearch = 0;
                }
            }
        PERFINFO_AUTO_STOP();
    }
    
    return NULL;
}

static Beacon* getBestBeaconAir(F32 curRadius, int* outFoundLOS){
    int i;
    
    for(i = 0; i < staticBeaconSortArray.size; i++){
        Beacon* targetBeacon = staticBeaconSortArray.storage[i];
        BeaconBlock* targetBlock;
        BeaconBlock* targetGalaxy;
        int good;

        if(    !targetBeacon
            ||
            !targetBeacon->gbConns.size && !targetBeacon->rbConns.size)
        {
            continue;
        }
        
        targetBlock = targetBeacon->block;
        targetGalaxy = targetBlock ? targetBlock->galaxy : NULL;
        
        if( targetGalaxy &&
            (    targetGalaxy->searchInstance == beacon_state.galaxySearchInstance &&
                !targetGalaxy->isGoodForSearch
                ||
                !pathFindEntity.useEnterable &&
                beaconFind.sourceCluster &&
                targetGalaxy->cluster != beaconFind.sourceCluster))
        {
            // This galaxy was already found to be bad, so ignore it.
            staticBeaconSortArray.storage[i] = NULL;
            continue;
        }

        PERFINFO_AUTO_START("air", 1);
            PERFINFO_AUTO_START("vis", 1);
                good = beaconVisibleFromAirPos(targetBeacon, beaconFind.sourcePos, curRadius);
            PERFINFO_AUTO_STOP();
        
            if(!good){
                PERFINFO_AUTO_STOP();
                continue;
            }

            *outFoundLOS = 1;
        
            if(    (    !targetGalaxy ||
                    !beaconFind.sourceCluster ||
                    targetGalaxy->cluster == beaconFind.sourceCluster)
                &&
                (    !beaconFind.sourceBeacon ||
                    !beaconFind.sourceGalaxy ||
                    beaconGalaxyPathExists(    beaconFind.sourceBeacon,
                                            targetBeacon,
                                            pathFindEntity.maxJumpHeight,
                                            pathFindEntity.canFly)))
            {
                PERFINFO_AUTO_STOP();
                return targetBeacon;
            }
            else if(pathFindEntity.useEnterable &&
                    targetGalaxy && 
                    beaconFind.sourceCluster &&
                    targetGalaxy->cluster != beaconFind.sourceCluster)
            {
                // Just make a path to the cluster connection.
                
                Beacon* result = checkForClusterConnection(targetGalaxy);
                
                if(result){
                    PERFINFO_AUTO_STOP();
                    return result;
                }else{
                    staticBeaconSortArray.storage[i] = NULL;
                }
            }
            else{
                if(targetGalaxy){
                    // Mark galaxy bad for this search instance.
                
                    targetGalaxy->searchInstance = beacon_state.galaxySearchInstance;
                    targetGalaxy->isGoodForSearch = 0;
                }
            }
        PERFINFO_AUTO_STOP();
    }
    
    return NULL;
}

static Beacon* getBestBeaconNoLOS(){
    int i;
    
    for(i = 0; i < staticBeaconSortArray.size; i++){
        Beacon* beacon = staticBeaconSortArray.storage[i];
        
        if(    beacon &&
            (beacon->gbConns.size || beacon->rbConns.size))
        {
            BeaconBlock* block = beacon->block;
            BeaconBlock* galaxy = block ? block->galaxy : NULL;

            if(    pathFindEntity.useEnterable && 
                galaxy &&
                beaconFind.sourceCluster &&
                galaxy->cluster != beaconFind.sourceCluster)
            {
                // Just make a path to the cluster connection.
                
                Beacon* result = checkForClusterConnection(galaxy);

                if(result){
                    return result;
                }else{
                    staticBeaconSortArray.storage[i] = NULL;
                }
            }
            else if(galaxy &&
                    (    galaxy->searchInstance == beacon_state.galaxySearchInstance &&
                        !galaxy->isGoodForSearch
                        ||
                        beaconFind.sourceCluster &&
                        galaxy->cluster != beaconFind.sourceCluster))
            {
                // This galaxy or cluster was already found to be bad, so ignore it.
                continue;
            }
            else if(beaconGalaxyPathExists(beaconFind.sourceBeacon, beacon, pathFindEntity.maxJumpHeight, pathFindEntity.canFly)){
                return beacon;
            }
            else{
                if(galaxy){
                    // Mark galaxy bad for this search instance.

                    galaxy->searchInstance = beacon_state.galaxySearchInstance;
                    galaxy->isGoodForSearch = 0;
                }
                
                //printf(    "rejecting (%1.1f, %1.1f, %1.1f) to (%1.1f, %1.1f, %1.1f)\n",
                //        posParamsXYZ(sourceBeacon),
                //        posParamsXYZ(beacon));
            }
        }
    }
    
    return NULL;
}

static StashTable htBadDestinations;

void beaconClearBadDestinationsTable()
{
    if( htBadDestinations )
        stashTableClear(htBadDestinations);
}

// Notice that GCCB_IGNORE_LOS only works if a source beacon is passed in.

Beacon* beaconGetClosestCombatBeacon(    const Vec3 sourcePos,
                                        F32 maxLOSRayRadius,
                                        Beacon* sourceBeacon,
                                        GCCBflags losFlags,
                                        S32* losFailed)
{
    bool ranMakeArray100 = false;
    char hashKey[500] = "";
    StashElement badElement = NULL;
    Beacon* bestBeacon = NULL;
    int foundLOS = 0;

    if(losFailed){
        *losFailed = 0;
    }

    beaconCheckBlocksNeedRebuild();

    //Fill out beaconFind struct
    beaconFind.sourceCluster = NULL;
    beaconFind.sourceGalaxy = NULL;
    copyVec3(sourcePos, beaconFind.sourcePos);
    beaconFind.sourceBeacon = sourceBeacon;

    if(sourceBeacon){
        beaconFind.sourceGalaxy = sourceBeacon->block->galaxy;
        
        if(beaconFind.sourceGalaxy){
            beaconFind.sourceCluster = beaconFind.sourceGalaxy->cluster;
        }
    }
    
    PERFINFO_AUTO_START("makeSortedNearbyBeaconArray", 1);
    
        if(!staticBeaconSortArray.maxSize){
            beaconPathInit(1);
        }

        // Get a list of beacons sorted by distance from sourcePos.
        makeSortedNearbyBeaconArray(&staticBeaconSortArray, sourcePos, 30.0);

        if(staticBeaconSortArray.size < 3 || ((Beacon*)staticBeaconSortArray.storage[staticBeaconSortArray.size - 1])->userFloat > SQR(30))
        {
            ranMakeArray100 = true;
            PERFINFO_AUTO_START("makeSortedNearbyBeaconArray100", 1);
            makeSortedNearbyBeaconArray(&staticBeaconSortArray, sourcePos, 100.0);
            PERFINFO_AUTO_STOP();
        }

    PERFINFO_AUTO_STOP();
        
    beacon_state.galaxySearchInstance++;

    if(!(losFlags & GCCB_IGNORE_LOS)){
        F32 curRadius;
        
        sprintf(hashKey, "%1.3f,%1.3f,%1.3f,%d", vecParamsXYZ(sourcePos), losFlags );

        if(!htBadDestinations || !stashFindElement(htBadDestinations, hashKey, &badElement))
        {
            PERFINFO_AUTO_START("beaconFind - LOS", 1);
            
                // Get the closest beacon that matches the condition.

                if(!pathFindEntity.canFly){
                    for(curRadius = maxLOSRayRadius;
                        curRadius >= 0 && !bestBeacon;
                        curRadius = (curRadius == 0) ? -1 : (curRadius - 0.5 <= 0) ? 0 : curRadius - 0.5)
                    {
                        bestBeacon = getBestBeaconGround(curRadius, &foundLOS);

                    }

                    if(!bestBeacon && !ranMakeArray100)
                    {
                        ranMakeArray100 = true;
                        PERFINFO_AUTO_START("makeSortedNearbyBeaconArray100", 1);
                        makeSortedNearbyBeaconArray(&staticBeaconSortArray, sourcePos, 100.0);
                        PERFINFO_AUTO_STOP();

                        for(curRadius = maxLOSRayRadius;
                            curRadius >= 0 && !bestBeacon;
                            curRadius = (curRadius == 0) ? -1 : (curRadius - 0.5 <= 0) ? 0 : curRadius - 0.5)
                        {
                            bestBeacon = getBestBeaconGround(curRadius, &foundLOS);
                        }
                    }
                }
                            

                if(!bestBeacon){
                    for(curRadius = maxLOSRayRadius;
                        curRadius >= 0 && !bestBeacon;
                        curRadius = (curRadius == 0) ? -1 : (curRadius - 0.5 <= 0) ? 0 : curRadius - 0.5)
                    {
                        bestBeacon = getBestBeaconAir(curRadius, &foundLOS);
                    }

                    if(!bestBeacon && !ranMakeArray100)
                    {
                        ranMakeArray100 = true;
                        PERFINFO_AUTO_START("makeSortedNearbyBeaconArray100", 1);
                        makeSortedNearbyBeaconArray(&staticBeaconSortArray, sourcePos, 100.0);
                        PERFINFO_AUTO_STOP();

                        for(curRadius = maxLOSRayRadius;
                            curRadius >= 0 && !bestBeacon;
                            curRadius = (curRadius == 0) ? -1 : (curRadius - 0.5 <= 0) ? 0 : curRadius - 0.5)
                        {
                            bestBeacon = getBestBeaconAir(curRadius, &foundLOS);
                        }
                    }
                }
                
                if(losFailed){
                    *losFailed = !foundLOS;
                }
            PERFINFO_AUTO_STOP();
        }
    }
    
    if( !bestBeacon &&
        sourceBeacon &&
        beaconFind.sourceGalaxy)
    {
        if(    !badElement &&
            !(losFlags & GCCB_IGNORE_LOS) &&
            !foundLOS)
        {
            assert(hashKey[0]);
            
            if(!htBadDestinations){
                htBadDestinations = stashTableCreateWithStringKeys(500, StashDeepCopyKeys);
            }
            
            if(stashGetValidElementCount(htBadDestinations) > 1000){
                stashTableClear(htBadDestinations);
            }
            
            stashAddPointer(htBadDestinations, hashKey, "nothing", false);
        }
        
        // No beacon found yet, so find the closest good beacon. 
        
        if( !(losFlags & GCCB_REQUIRE_LOS) &&
            (    !foundLOS ||
                !(losFlags & GCCB_IF_ANY_LOS_ONLY_LOS)))
        {
            PERFINFO_AUTO_START("beaconFind - NoLOS", 1);
            bestBeacon = getBestBeaconNoLOS();
            PERFINFO_AUTO_STOP();

            if(    !bestBeacon &&
                !ranMakeArray100)
            {
                ranMakeArray100 = true;
                PERFINFO_AUTO_START("makeSortedNearbyBeaconArray100", 1);
                makeSortedNearbyBeaconArray(&staticBeaconSortArray, sourcePos, 100.0);
                PERFINFO_AUTO_STOP();

                PERFINFO_AUTO_START("beaconFind - NoLOS - after second mSNBA", 1);
                bestBeacon = getBestBeaconNoLOS();
                PERFINFO_AUTO_STOP();
            }
        }
    }
    
    //if(!bestBeacon && staticBeaconSortArray.size){
    //    bestBeacon = staticBeaconSortArray.storage[0];
    //}
    
    if(!bestBeacon || !bestBeacon->block){
        return NULL;
    }
    
    return bestBeacon;
}

Beacon* beaconGetClosestCombatBeaconVerifier(Vec3 sourcePos){
    pathFindEntity.canFly = 1;
    
    return beaconGetClosestCombatBeacon(sourcePos, 1, NULL, GCCB_PREFER_LOS, NULL);
}

int beaconGalaxyPathExists(Beacon* source, Beacon* target, F32 maxJumpHeight, S32 canFly){
    static struct {
        char*                name;
        PerformanceInfo*    timer;
    } galaxySearchTimer[ARRAY_SIZE(combatBeaconGalaxyArray)];
    
    NavSearchFunctions galaxySearchFunctions = {
        (NavSearchCostToTargetFunction)        beaconGalaxySearchCostToTarget,
        (NavSearchCostFunction)                beaconGalaxySearchCost,
        (NavSearchGetConnectionsFunction)    beaconGalaxySearchGetConnections,
        (NavSearchOutputPath)                NULL,
        (NavSearchCloseNode)                NULL,
        (NavSearchGetNodeInfo)                NULL,
        (NavSearchReportClosedInfo)            NULL,
    };

    BeaconBlock* sourceGalaxy;
    BeaconBlock* targetGalaxy;
    F32 heightRemaining;
    int galaxySet = 0;
    
    beaconCheckBlocksNeedRebuild();

    if(    !source->block ||
        !target->block)
    {
        return 0;
    }
    
    sourceGalaxy = source->block->galaxy;
    targetGalaxy = target->block->galaxy;
    
    if(!sourceGalaxy || !targetGalaxy || sourceGalaxy->cluster != targetGalaxy->cluster){
        return 0;
    }
    else if(canFly){
        return 1;
    }

    for(heightRemaining = maxJumpHeight;
        heightRemaining >= BEACON_GALAXY_GROUP_JUMP_INCREMENT;
        heightRemaining -= BEACON_GALAXY_GROUP_JUMP_INCREMENT)
    {
        if(    !sourceGalaxy->galaxy ||
            !targetGalaxy->galaxy)
        {
            break;
        }

        sourceGalaxy = sourceGalaxy->galaxy;
        targetGalaxy = targetGalaxy->galaxy;
        galaxySet++;

        if(sourceGalaxy == targetGalaxy){
            return 1;
        }
    }

    if(sourceGalaxy == targetGalaxy){
        return 1;
    }

    pathFindEntity.maxJumpHeight = maxJumpHeight;

    initStaticSearchData();

    staticSearchData->sourceNode = sourceGalaxy;
    staticSearchData->targetNode = targetGalaxy;
    staticSearchData->searching = 0;
    staticSearchData->pathWasOutput = 0;
    staticSearchData->steps = 0;
    staticSearchData->nodeAStarInfoOffset = offsetof(BeaconBlock, astarInfo);

    blockSearch.targetBlock = targetGalaxy;

    PERFINFO_RUN(
        if(!galaxySearchTimer[0].name){
            int i;
            for(i = 0; i < ARRAY_SIZE(combatBeaconGalaxyArray); i++){
                char buffer[100];
                sprintf(buffer, "A* galaxy (%1.1fft)", (F32)(i * BEACON_GALAXY_GROUP_JUMP_INCREMENT));
                galaxySearchTimer[i].name = strdup(buffer);
            }
        }
    );

    PERFINFO_AUTO_START_STATIC(galaxySearchTimer[galaxySet].name, &galaxySearchTimer[galaxySet].timer, 1);
        AStarSearch(staticSearchData, &galaxySearchFunctions);
    PERFINFO_AUTO_STOP();
    
    return staticSearchData->pathWasOutput;
}

int beaconGetNextClusterWaypoint(    BeaconBlock* sourceCluster,
                                    BeaconBlock* targetCluster,
                                    Vec3 outPos)
{
    NavSearchFunctions clusterSearchFunctions = {
        (NavSearchCostToTargetFunction)        beaconClusterSearchCostToTarget,
        (NavSearchCostFunction)                beaconClusterSearchCost,
        (NavSearchGetConnectionsFunction)    beaconClusterSearchGetConnections,
        (NavSearchOutputPath)                beaconClusterSearchOutputPath,
        (NavSearchCloseNode)                NULL,
        (NavSearchGetNodeInfo)                NULL,
        (NavSearchReportClosedInfo)            NULL,
    };

    if(    !sourceCluster ||
        !targetCluster)
    {
        return 0;
    }
    
    if(sourceCluster == targetCluster){
        return 1;
    }
    
    initStaticSearchData();
    
    staticSearchData->sourceNode = sourceCluster;
    staticSearchData->targetNode = targetCluster;
    staticSearchData->searching = 0;
    staticSearchData->pathWasOutput = 0;
    staticSearchData->steps = 0;
    staticSearchData->nodeAStarInfoOffset = offsetof(BeaconBlock, astarInfo);

    clusterSearch.connToNextCluster = NULL;

    blockSearch.sourceBlock = sourceCluster;
    blockSearch.targetBlock = targetCluster;

    PERFINFO_AUTO_START("A* cluster", 1);
        AStarSearch(staticSearchData, &clusterSearchFunctions);
    PERFINFO_AUTO_STOP();
    
    if(clusterSearch.connToNextCluster){
        copyVec3(clusterSearch.connToNextCluster->source.pos, outPos);
        
        return 1;
    }
    
    return 0;
}

static int beaconPathFindSimpleHelper(NavSearch* search, Beacon* src, BeaconScoreFunction funcBeaconScore){
    int i;
    int bestScore;
    BeaconConnection* conn;
    BeaconConnection* bestConn;
    Beacon* dst = NULL;
    Beacon* best = NULL;
    
    src->searchInstance = beacon_state.beaconSearchInstance;
    
    if(!src->block->madeConnections){
        return 0;
    }
    
    if(!src->gbConns.size){
        return 1;
    }
    
    for(i = 0; i < src->gbConns.size; i++){
        int score;
        
        conn = src->gbConns.storage[i];
        dst = conn->destBeacon;
    
        if(dst->searchInstance == beacon_state.beaconSearchInstance){
            continue;
        }

        score = funcBeaconScore(src, conn);
        
        if(score < 0){
            if(score == BEACONSCORE_FINISHED){
                // Success, this is a good destination.

                NavPathWaypoint* wp = createNavPathWaypoint();

                wp->beacon = dst;
                wp->connectType = NAVPATH_CONNECT_GROUND;
                copyVec3(posPoint(dst), wp->pos);
                vecY(wp->pos) -= beaconGetFloorDistance(dst);

                navPathAddTail(&search->path, wp);
                
                return 1;
            }
            else if(score == BEACONSCORE_CANCEL){
                // Kill the search.
                
                return 0;
            }
        }
        else if(score && (!best || score > bestScore)){
            best = dst;
            bestConn = conn;
            bestScore = score;
        }
    }
    
    if(best){
        if(beaconPathFindSimpleHelper(search, best, funcBeaconScore)){
            NavPathWaypoint* wp = createNavPathWaypoint();

            wp->beacon = best;
            wp->connectType = NAVPATH_CONNECT_GROUND;
            copyVec3(posPoint(best), wp->pos);
            vecY(wp->pos) -= beaconGetFloorDistance(dst);

            navPathAddTail(&search->path, wp);

            return 1;
        }else{
            return 0;
        }
    }else{
        // Success, there are no more destinations.
        return 1;
    }
}

void beaconPathFindSimple(NavSearch* search, Beacon* src, BeaconScoreFunction funcCheckBeacon){
    clearNavSearch(search);

    if(!src){
        return;
    }
    
    beaconCheckBlocksNeedRebuild();

    beacon_state.beaconSearchInstance++;

    if(!src->gbConns.size)
    {
        int i, n;
        static BeaconConnection** possibleConns = NULL;

        if(possibleConns)
            eaSetSize(&possibleConns, 0);

        for(i = 0; i < src->rbConns.size; i++)
        {
            BeaconConnection* conn = src->rbConns.storage[i];

            if(conn->destBeacon->gbConns.size)
                eaPush(&possibleConns, conn);
        }

        n = eaSize(&possibleConns);

        if(n)
        {
            NavPathWaypoint* wp = createNavPathWaypoint();
            BeaconConnection* conn = possibleConns[randomU32() % n];

            wp->beacon = src;
            wp->connectType = NAVPATH_CONNECT_GROUND;
            copyVec3(posPoint(src), wp->pos);
            vecY(wp->pos) -= beaconGetFloorDistance(src);

            navPathAddHead(&search->path, wp);

            search->path.curWaypoint = search->path.head;

            wp = createNavPathWaypoint();

            wp->beacon = conn->destBeacon;
            wp->connectType = NAVPATH_CONNECT_JUMP;
            copyVec3(posPoint(conn->destBeacon), wp->pos);
            vecY(wp->pos) -= beaconGetFloorDistance(conn->destBeacon);
            wp->connectionToMe = conn;

            navPathAddTail(&search->path, wp);

            return;
        }
    }

    if(beaconPathFindSimpleHelper(search, src, funcCheckBeacon)){
        NavPathWaypoint* wp = createNavPathWaypoint();

        wp->beacon = src;
        wp->connectType = NAVPATH_CONNECT_GROUND;
        copyVec3(posPoint(src), wp->pos);
        vecY(wp->pos) -= beaconGetFloorDistance(src);

        navPathAddHead(&search->path, wp);
        
        search->path.curWaypoint = search->path.head;
    }
}

void beaconPathFindSimpleStart(NavSearch* search, Position* pos, BeaconScoreFunction funcCheckBeacon){
    Beacon* beacon = beaconGetClosestCombatBeacon(posPoint(pos), 1, NULL, GCCB_PREFER_LOS, NULL);
    
    if(beacon && funcCheckBeacon(beacon, NULL)){
        beaconPathFindSimple(search, beacon, funcCheckBeacon);
    }
}

void beaconPathFindRaw(    NavSearch* search,
                        AStarSearchData* searchData,
                        Beacon* src,
                        Beacon* dst,
                        int steps,
                        NavSearchResults* results)
{
    NavSearchFunctions blockSearchFunctions = {
        (NavSearchCostToTargetFunction)        beaconBlockSearchCostToTarget,
        (NavSearchCostFunction)                beaconBlockSearchCost,
        (NavSearchGetConnectionsFunction)    beaconBlockSearchGetConnections,
        (NavSearchOutputPath)                beaconBlockSearchOutputPath,
        (NavSearchCloseNode)                NULL,
        (NavSearchGetNodeInfo)                NULL,
        (NavSearchReportClosedInfo)            NULL,
    };

    NavSearchFunctions beaconSearchFunctions = {
        (NavSearchCostToTargetFunction)        beaconSearchCostToTarget,
        (NavSearchCostFunction)                beaconSearchCost,
        (NavSearchGetConnectionsFunction)    beaconSearchGetConnections,
        (NavSearchOutputPath)                beaconSearchOutputPath,
        (NavSearchCloseNode)                NULL,
        (NavSearchGetNodeInfo)                beaconSearchGetNodeInfo,
        (NavSearchReportClosedInfo)            NULL,
    };
    
    beaconCheckBlocksNeedRebuild();

    if(pathFindEntity.canFly){
        blockSearchFunctions.closeNode = (NavSearchCloseNode)beaconBlockSearchCloseNode;
        beaconSearchFunctions.closeNode = (NavSearchCloseNode)beaconSearchCloseNode;
    }
    
    // Start a debug packet.
    
    if(    *(int*)&staticDebugEntity &&
        *((int*)&staticDebugEntity + 1) &&
        erGetEnt(staticDebugEntity) &&
        erGetEnt(staticDebugEntity)->client &&
        pathFindEntity.ent->owner == entSendGetInterpStoreEntityId())
    {
        staticDebugPacket = pktCreate();
        pktSendBitsPack(staticDebugPacket,1,SERVER_DEBUGCMD);
        pktSendString(staticDebugPacket, "AStarRecording");
    }else{
        staticDebugPacket = NULL;
    }
    
    if(!search->searching){
        if(results){
            copyVec3(posPoint(src), results->sourceBeaconPos);
            copyVec3(posPoint(dst), results->targetBeaconPos);
        }

        // Make sure that beacons are in a block.
        
        if(    src->block->isGridBlock ||
            dst->block->isGridBlock ||
            !src->block->madeConnections ||
            !dst->block->madeConnections)
        {
            if(results){
                results->resultType = NAV_RESULT_BLOCK_ERROR;
                
                if(results->resultMsg){
                    sprintf(results->resultMsg, "Source or destination beacon is still in grid block, or connections not made.");
                }
            }                
            return;
        }

        // Do the block-level search.
        
        searchData->sourceNode = src->block;
        searchData->targetNode = dst->block;
        searchData->searching = 0;
        searchData->pathWasOutput = 0;
        searchData->steps = 0;
        searchData->nodeAStarInfoOffset = offsetof(BeaconBlock, astarInfo);
        
        blockSearch.sourceBlock = src->block;
        staticSourcePosition[0] = blockSearch.sourceBlock->pos[0];
        staticSourcePosition[2] = blockSearch.sourceBlock->pos[2];

        if(pathFindEntity.canFly && pathFindEntity.ent){
            src->block->searchY = ENTPOSY(pathFindEntity.ent);
            staticSourcePosition[1] = ENTPOSY(pathFindEntity.ent);
        }else{
            staticSourcePosition[1] = blockSearch.sourceBlock->pos[1];
        }

        blockSearch.targetBlock = dst->block;
        staticTargetPosition[0] = blockSearch.targetBlock->pos[0];
        staticTargetPosition[1] = search->destinationHeight;
        staticTargetPosition[2] = blockSearch.targetBlock->pos[2];
        
        PERFINFO_AUTO_START("Block Search 1", 1);
            AStarSearch(searchData, &blockSearchFunctions);
        PERFINFO_AUTO_STOP();

        if(!searchData->pathWasOutput){
            if(0){
                searchData->searching = 0;
                
                searchData->findClosestOnFail = 1;

                PERFINFO_AUTO_START("Block Search 2", 1);
                    AStarSearch(searchData, &blockSearchFunctions);
                PERFINFO_AUTO_STOP();
            }
            
            if(!searchData->pathWasOutput){
                if(results){
                    results->resultType = NAV_RESULT_NO_BLOCK_PATH;
                    
                    if(results->resultMsg){
                        sprintf(results->resultMsg,
                                "No block path!: ^4%d ^0conns, ^4%d ^0nodes (^4%1.f^0,^4%1.f^0,^4%1.f^0) to (^4%1.f^0,^4%1.f^0,^4%1.f^0).",
                                searchData->checkedConnectionCount,
                                searchData->exploredNodeCount,
                                posParamsXYZ(src),
                                posParamsXYZ(dst));
                    }
                }

                return;
            }
        }
    }
    
    // Do the beacon-level search.

    if(!search->searching){
        if(pathFindEntity.canFly){
            Vec3 pos;
            
            // userFloat will be the Y position to go to on the destination beacon.
            
            beaconGetClosestAirPos(src, ENTPOS(pathFindEntity.ent), pos);
            
            src->userFloat = vecY(pos);
        }

        search->src = src;
        search->dst = dst;
        searchData->searching = 0;
        searchData->pathWasOutput = 0;
        searchData->exploredNodeCount = 0;
        searchData->findClosestOnFail = 0;
    }

    searchData->sourceNode = search->src;
    searchData->targetNode = search->dst;
    searchData->steps = steps;
    searchData->nodeAStarInfoOffset = offsetof(Beacon, astarInfo);
    searchData->userData = search;
    
    staticTargetBeacon = search->dst;

    // Create the target position.

    vecX(staticTargetPosition) = posX(staticTargetBeacon);
    vecZ(staticTargetPosition) = posZ(staticTargetBeacon);
    
    if(pathFindEntity.canFly){
        vecY(staticTargetPosition) = staticTargetBeacon->userFloat;
    }else{
        vecY(staticTargetPosition) = posY(staticTargetBeacon);
    }
    
    // Send the destination point.
    
    if(staticDebugPacket){
        pktSendF32(staticDebugPacket, posX(search->dst));
        pktSendF32(staticDebugPacket, search->dst->userFloat);//search->destinationHeight);
        pktSendF32(staticDebugPacket, posZ(search->dst));

        beaconSearchFunctions.reportClosedInfo = (NavSearchReportClosedInfo)beaconSearchReportClosedInfo;
    }
    
    // Run the search.
    
    PERFINFO_AUTO_START("Beacon Search", 1);
        AStarSearch(searchData, &beaconSearchFunctions);
    PERFINFO_AUTO_STOP();
    
    // Send the debug packet.
    
    if(staticDebugPacket){    
        pktSendBits(staticDebugPacket, 1, 0);
        pktSend(&staticDebugPacket, erGetEnt(staticDebugEntity)->client->link);
        staticDebugPacket = NULL;
    }
    
    // Print a message to resultMsg.
    
    if(results){
        if(searchData->pathWasOutput)
            results->resultType = NAV_RESULT_SUCCESS;
        else
            results->resultType = NAV_RESULT_NO_BEACON_PATH;
    
        if(results->resultMsg){
            sprintf(results->resultMsg,
                    "%s^4%d^d/^4%d ^dbcns, ^4%d ^dconns (^4%1.f^d,^4%1.f^d,^4%1.f^d)-(^4%1.f^d,^4%1.f^d,^4%1.f^d)",
                    searchData->pathWasOutput ? "" : "No beacon path!: ",
                    searchData->exploredNodeCount,
                    staticAvailableBeaconCount,
                    searchData->checkedConnectionCount,
                    posX(search->src), pathFindEntity.canFly ? search->src->userFloat : posY(search->src), posZ(search->src), 
                    posX(search->dst), pathFindEntity.canFly ? search->dst->userFloat : posY(search->dst), posZ(search->dst));
        }
    }
    
    search->searching = searchData->searching;
}

static Entity* getFirstDoorEntity(BeaconDynamicConnection* dynConn){
    Entity* bestDoor = NULL;
    
    if(SAFE_MEMBER(dynConn, doorBlockedCount)){
        S32 size = eaSize(&dynConn->infos);
        S32 i;
        
        for(i = 0; i < size; i++){
            if(    dynConn->infos[i]->isDoor &&
                dynConn->infos[i]->entity &&
                !dynConn->infos[i]->connsEnabled)
            {
                Entity* curDoor = dynConn->infos[i]->entity;
                
                if(bestDoor){
                    F32 curDistSQR = distance3Squared(ENTPOS(curDoor), posPoint(dynConn->source));
                    F32 bestDistSQR = distance3Squared(ENTPOS(bestDoor), posPoint(dynConn->source));
                    
                    if(curDistSQR > bestDistSQR){
                        continue;
                    }
                }
                
                bestDoor = curDoor;
            }
        }
    }
    
    return bestDoor;
}

void beaconPathFind(NavSearch* search, const Vec3 sourcePos, const Vec3 targetPos, NavSearchResults* results){
    Beacon* sourceBeacon = NULL;
    Beacon* targetBeacon = NULL;
    Vec3 pos;

    if(results){
        results->losFailed = 0;
    }

    clearNavSearch(search);

    beaconFind.clusterConnTargetBeacon = NULL;
    beaconFind.wantClusterConnect = 0;
    
    if(!sourcePos || !targetPos){
        if(results){
            results->resultType = NAV_RESULT_BAD_POSITIONS;
        
            if(results->resultMsg)
                sprintf(results->resultMsg, "Missing source or destination position.");
        }
                    
        return;
    }

    //printf(    "\n************ Finding Path ************\n"
    //        "+ Source:        %5.f, %5.f, %5.f\n"
    //        "+ Dest:          %5.f, %5.f, %5.f\n",
    //        posX(source), posY(source), posZ(source),
    //        posX(destination), posY(destination), posZ(destination));

    // Get the closest beacon to the source.
    
    PERFINFO_AUTO_START("findSourceBeacon", 1);
        sourceBeacon = beaconGetClosestCombatBeacon(sourcePos, 1, NULL, GCCB_PREFER_LOS, NULL);
    PERFINFO_AUTO_STOP();

    if(!sourceBeacon){
        if(results){
            results->resultType = NAV_RESULT_NO_SOURCE_BEACON;
        
            if(results->resultMsg){
                sprintf(results->resultMsg,
                        "No beacon found near source (%1.1f,%1.1f,%1.1f).",
                        vecParamsXYZ(sourcePos));
            }
        }
            
        return;
    }
    
    // Get the closest beacon to the destination.
    
    PERFINFO_AUTO_START("findTargetBeacon", 1);
    {
        S32 losFailed;

        targetBeacon = beaconGetClosestCombatBeacon(targetPos, 1, sourceBeacon, GCCB_PREFER_LOS, &losFailed);

        if(results && losFailed){
            results->losFailed = 1;
        }
    }
    PERFINFO_AUTO_STOP();
    
    if(!targetBeacon){
        if(results){
            results->resultType = NAV_RESULT_NO_TARGET_BEACON;
        
            if(results->resultMsg){
                sprintf(results->resultMsg,
                        "No beacon found near target (%1.1f,%1.1f,%1.1f) from src (%1.1f,%1.1f,%1.1f) and bcn (%1.1f,%1.1f,%1.1f).",
                        vecParamsXYZ(targetPos),
                        vecParamsXYZ(sourcePos),
                        posParamsXYZ(sourceBeacon));
            }
        }

        //devassertmsg(0, "Couldn't find targetBeacon");
            
        return;
    }
    
    beaconGetClosestAirPos(targetBeacon, targetPos, pos);
    
    targetBeacon->userFloat = vecY(pos);
    
    search->destinationHeight = vecY(pos);

    if(!search->searching){
        // Initialize the global searchData object.
        
        initStaticSearchData();
    }
            
    PERFINFO_AUTO_START("A* Pathfinding", 1);
        beaconPathFindRaw(search, staticSearchData, sourceBeacon, targetBeacon, 0, results);
    PERFINFO_AUTO_STOP();

    if(    beaconFind.wantClusterConnect &&
        beaconFind.clusterConnTargetBeacon
        ||
        pathFindEntity.disappearInDoor)
    {
        NavPathWaypoint* pathtail = SAFE_MEMBER(search, path.tail);
        DoorEntry* doorEntry = NULL;;

        if(!pathtail){
            // MS: This would be weird, I think.
            
            return;
        }

        if(pathFindEntity.disappearInDoor){
            doorEntry = pathFindEntity.disappearInDoor;
        }else{
            Entity* doorEntity = getFirstDoorEntity(beaconFind.dynConnToTarget);
            S32        checkDoorEntry = 0;
            
            if(doorEntity){
                doorEntry = dbFindLocalMapObj(ENTPOS(doorEntity), 0, 10000.0);
                checkDoorEntry = 1;
            }
            
            // Don't find a DoorEntry near the tail IF the cluster connection
            // has a dynamic beacon connection (which implies that it's going through
            // some dynamic collision object).

            if(!doorEntry){
                if(!beaconFind.dynConnToTarget){
                    doorEntry = dbFindLocalMapObj(pathtail->pos, 0, 10000.0);
                    checkDoorEntry = 1;
                }else{
                    // The path leads through a dynamic collision object, so we can't get through.
                    
                    if(results){
                        results->resultType = NAV_RESULT_CLUSTER_CONN_BLOCKED;
                        
                        if(results->resultMsg){
                            sprintf(results->resultMsg, "The cluster connection is through a dynamic collision object.");
                        }
                    }
                    
                    clearNavSearch(search);
                    
                    return;
                }
            }

            if(checkDoorEntry){
                devassertmsg(doorEntry, "Wanted to use a cluster connection but couldn't find a door");
            }
        }
                            
        if(doorEntry){
            // Add a door connection.
        
            NavPathWaypoint* wp = createNavPathWaypoint();
            
            wp->door = doorEntry;

            copyVec3(wp->door->mat[3], wp->pos);
            wp->connectType = NAVPATH_CONNECT_ENTERABLE;
            navPathAddTail(&search->path, wp);

            if(    !pathFindEntity.disappearInDoor &&
                wp->door->type != DOORTYPE_FREEOPEN &&
                (    wp->door->type != DOORTYPE_KEY ||
                    wp->door->special_info ))
            {
                // If the door is a walkthrough door, just opening it should suffice.
                
                const F32* targetPos = beaconFind.clusterConnTargetBeacon->mat[3];
                DoorEntry* exitdoor = dbFindLocalMapObj(targetPos, 0, 50.0);
                
                // Add another waypoint.
                
                if(exitdoor)
                {
                    wp = createNavPathWaypoint();
                    wp->door = exitdoor;
                    copyVec3(pathtail->pos, wp->pos);
                    wp->connectType = NAVPATH_CONNECT_ENTERABLE;
                    navPathAddTail(&search->path, wp);
                }
                else
                {
                    static char* errorbuf = NULL;
                    estrPrintf(&errorbuf,
                        "Cluster connection without doors: (%1.2f, %1.2f, %1.2f) to (%1.2f, %1.2f, %1.2f),"
                        " pathfinding from (%1.2f, %1.2f, %1.2f) to (%1.2f, %1.2f, %1.2f)",
                        vecParamsXYZ(pathtail->pos), posParamsXYZ(beaconFind.clusterConnTargetBeacon),
                        vecParamsXYZ(sourcePos), vecParamsXYZ(targetPos));
                    dbLog("Path:ClusterConnection", pathFindEntity.ent, errorbuf);
                    devassertmsg(exitdoor, "%s", errorbuf);
                }
            }
        }
    }
}

void beaconPathInit(int forceUpdate){
    int dx = 3;
    int dz = 3;
    const int dxz = dx * dz;
    int i;
    int maxBeaconsInBigCube = 0;
    int min_y = INT_MAX;
    int max_y = INT_MIN;
    
    if(staticBeaconSortArray.maxSize){
        if(forceUpdate){
            destroyArrayPartial(&staticBeaconSortArray);
        }else{
            return;
        }
    }
    
    if(!combatBeaconGridBlockArray.size){
        return;
    }
    
    PERFINFO_AUTO_START("beaconPathInit", 1);
    
        for(i = 0; i < combatBeaconGridBlockArray.size; i++){
            BeaconBlock* block = combatBeaconGridBlockArray.storage[i];
            
            if(block->pos[1] < min_y){
                min_y = block->pos[1];
            }

            if(block->pos[1] > max_y){
                max_y = block->pos[1];
            }
        }
        
        for(i = 0; i < combatBeaconGridBlockArray.size; i++){
            BeaconBlock* block = combatBeaconGridBlockArray.storage[i];
            int j;
            int total = 0;
            
            for(j = 0; j < dxz; j++){
                BeaconBlock* block2;
                int y;
                
                for(y = min_y; y <= max_y; y++){
                    int x = block->pos[0] - (dx/2) + j % dz;
                    int z = block->pos[2] - (dz/2) + j / dz;
                    
                    block2 = beaconGetGridBlockByCoords(x, y, z, 0);
                                                                
                    if(block2){
                        total += block2->beaconArray.size;
                    }
                }
            }
            
            if(total > maxBeaconsInBigCube){
                maxBeaconsInBigCube = total;
            }
        }
        
        staticBeaconSortMinY = min_y;
        staticBeaconSortMaxY = max_y;
        
        if(maxBeaconsInBigCube){
            initArray(&staticBeaconSortArray, maxBeaconsInBigCube);
        }
    PERFINFO_AUTO_STOP();
}

void beaconPathResetTemp(){
    int i;
    
    for(i = 0; i < combatBeaconArray.size; i++){
        Beacon* b = combatBeaconArray.storage[i];
        int j;
        
        for(j = 0; j < b->gbConns.size; j++){
            BeaconConnection* conn = b->gbConns.storage[j];
            conn->searchDistance = 0;
        }

        for(j = 0; j < b->rbConns.size; j++){
            BeaconConnection* conn = b->rbConns.storage[j];
            conn->searchDistance = 0;
        }
    }
}

int beaconPathExists( Vec3 pos1, Vec3 pos2, int canFly, F32 jumpHeight, int beaconlessPositionsAreErrors )
{
    int success = 0;
    Beacon * beacon1;
    Beacon * beacon2;
    
    beaconSetPathFindEntityAsParameters(canFly ? 99999 : jumpHeight, canFly, 1);

    beacon1 = beaconGetClosestCombatBeacon(pos1, 1, NULL, GCCB_IF_ANY_LOS_ONLY_LOS, NULL);
    if( !beacon1 )
    {
        if( beaconlessPositionsAreErrors )
            Errorf( "Start position given to beaconPathExists that has no beacons within los (source: %1.2f, %1.2f, %1.2f)", vecParamsXYZ(pos1));    
    }
    else
    {
        beacon2 = beaconGetClosestCombatBeacon(pos2, 1, beacon1, GCCB_IF_ANY_LOS_ONLY_LOS, NULL);
        if( !beacon2 )
        {
            if(isDevelopmentMode())
            {
                beacon2 = beaconGetClosestCombatBeacon(pos2, 1, NULL, GCCB_IF_ANY_LOS_ONLY_LOS, NULL);

                if( beaconlessPositionsAreErrors )
                {
                    if(beacon2)
                        //MW rm Errorf: The function is asking whether this path exists, so why would a no answer be an error?
                        ;//Errorf( "Source (%1.2f, %1.2f, %1.2f) and target (%1.2f, %1.2f, %1.2f) are not in the same cluster and don't have a direct cluster connection", vecParamsXYZ(pos1), vecParamsXYZ(pos2));
                    else
                        Errorf( "End position given to beaconPathExists that has no beacons within los (target: %1.2f, %1.2f, %1.2f)", vecParamsXYZ(pos2) );    
                }
            }
        }
        else
        {
            success = beaconGalaxyPathExists( beacon1, beacon2, jumpHeight, canFly);
        }
    }
    return success;
}