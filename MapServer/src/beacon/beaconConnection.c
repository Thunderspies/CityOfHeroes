
#include "beaconPrivate.h"
#include "cmdparse/cmdcommon.h"
#include "beacon/beaconConnection.h"
#include "entity/entserver.h"
#include "generator/entgen.h"
#include "group/group.h"
#include "group/groupnetsend.h"
#include "cmdparse/cmdserver.h"
#include "gridcoll/gridcoll.h"
#include "entity/motion.h"
#include "entity/entGameActions.h"
#include <utilitieslib/utils/utils.h>
#include "ai/entai.h"
#include "group/grouptrack.h"
#include <utilitieslib/components/EString.h>
#include "group/groupfilelib.h"
#include <utilitieslib/utils/strings_opt.h>
#include "group/groupscene.h"
#include "beacon/beaconGenerate.h"
#include <utilitieslib/components/earray.h>
#include "dbcomm/dbcomm.h"
#include "ai/entaiprivate.h"
#include "beacon/beaconPath.h"
#include "dbcomm/dbdoor.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "seq/anim.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/timing.h>
#include "svr/svr_player.h"
#include "comm_game.h"
#include "svr/svr_base.h"

// Memory pools.

MP_DEFINE(BeaconConnection);
MP_DEFINE(BeaconBlockConnection);
MP_DEFINE(BeaconClusterConnection);
MP_DEFINE(BeaconBlock);

// Timer.

const char* beaconCurTimeString(int start){
    static U32 staticStartTime;
    U32 curTime = timerSecondsSince2000();
    
    if(start){
        staticStartTime = curTime;
        
        return NULL;
    }else{
        static char* buffer = NULL;
        
        U32 elapsedTime = curTime - staticStartTime;

        estrPrintf(&buffer, "%d:%2.2d:%2.2d", elapsedTime / 3600, (elapsedTime / 60) % 60, elapsedTime % 60);
        
        return buffer;
    }
}

// BeaconConnection

BeaconConnection* createBeaconConnection(void){
    MP_CREATE(BeaconConnection, 5000);
    
    return MP_ALLOC(BeaconConnection);
}

void destroyBeaconConnection(BeaconConnection* conn){
    MP_FREE(BeaconConnection, conn);
}

U32 getAllocatedBeaconConnectionCount(void){
    return (U32)mpGetAllocatedCount(MP_NAME(BeaconConnection));
}

// BeaconBlockConnection

BeaconBlockConnection* createBeaconBlockConnection(void){
    BeaconBlockConnection* conn;
    
    MP_CREATE(BeaconBlockConnection, 1000);
    
    conn = MP_ALLOC(BeaconBlockConnection);
    
    return conn;
}

void destroyBeaconBlockConnection(BeaconBlockConnection* conn){
    MP_FREE(BeaconBlockConnection, conn);
}

// BeaconClusterConnection

BeaconClusterConnection* createBeaconClusterConnection(void){
    BeaconClusterConnection* conn;
    
    MP_CREATE(BeaconClusterConnection, 1000);
    
    conn = MP_ALLOC(BeaconClusterConnection);
    
    return conn;
}

static void destroyBeaconClusterConnection(BeaconClusterConnection* conn){
    MP_FREE(BeaconClusterConnection, conn);
}

// BeaconBlock

BeaconBlock* createBeaconBlock(void){
    BeaconBlock* block;
    
    MP_CREATE(BeaconBlock, 100);
    
    block = MP_ALLOC(BeaconBlock);
    
    return block;    
}

void destroyBeaconSubBlock(BeaconBlock* block){
    if(!block)
        return;
        
    clearArrayEx(&block->gbbConns, destroyBeaconBlockConnection);
    clearArrayEx(&block->rbbConns, destroyBeaconBlockConnection);

    MP_FREE(BeaconBlock, block);
}

void destroyBeaconGridBlock(BeaconBlock* block){
    if(!block)
        return;
        
    destroyArrayPartial(&block->beaconArray);
    clearArrayEx(&block->subBlockArray, destroyBeaconSubBlock);
    
    MP_FREE(BeaconBlock, block);
}

void destroyBeaconGalaxyBlock(BeaconBlock* block){
    if(!block)
        return;

    clearArrayEx(&block->gbbConns, destroyBeaconBlockConnection);
    clearArrayEx(&block->rbbConns, destroyBeaconBlockConnection);
    
    MP_FREE(BeaconBlock, block);
}

void destroyBeaconClusterBlock(BeaconBlock* block){
    if(!block)
        return;

    destroyArrayPartialEx(&block->gbbConns, destroyBeaconClusterConnection);
    
    MP_FREE(BeaconBlock, block);
}

// BeaconBlock memory pool.

typedef struct BeaconMemoryChunk {
    U32        alloced;
    U32        remaining;
    U8*        memory;
} BeaconMemoryChunk;

typedef struct BeaconMemoryChunkArray {
    BeaconMemoryChunk*            chunks;
    int                            size;
    int                            maxSize;
} BeaconMemoryChunkArray;

static void freeAllChunkMemory(BeaconMemoryChunkArray* chunks, int freeMemory){
    int i;
    
    for(i = 0; i < chunks->size; i++){
        if(freeMemory){
            SAFE_FREE(chunks->chunks[i].memory);
        }else{
            chunks->chunks[i].remaining += chunks->chunks[i].alloced;
            chunks->chunks[i].alloced = 0;
        }
    }
    
    if(freeMemory){
        ZeroMemory(chunks->chunks, sizeof(chunks->chunks[0]) * chunks->maxSize);
        
        chunks->size = 0;
    }
}

static void* getChunkMemory(BeaconMemoryChunkArray* chunks, U32 size){
    int chunkCount = chunks->size;
    int i;
    
    // Align to 4 bytes.
    
    size = (size + 3) & ~3;
    
    for(i = 0; i < chunkCount; i++){
        if(size < chunks->chunks[i].remaining){
            break;
        }
    }
    
    if(i == chunkCount){
        BeaconMemoryChunk* newChunk = dynArrayAddStruct(&chunks->chunks, &chunks->size, &chunks->maxSize);
        U32 newSize = max(size, SQR(1024));
        
        newChunk->memory = calloc(newSize, 1);
        newChunk->remaining = newSize;
    }

    chunks->chunks[i].alloced += size;
    chunks->chunks[i].remaining -= size;
    
    return chunks->chunks[i].memory + chunks->chunks[i].alloced - size;
}

static void getChunkMemoryUsage(BeaconMemoryChunkArray* chunks, U32* usedOut, U32* unusedOut){
    int i;

    *usedOut = 0;
    *unusedOut = 0;    
    
    for(i = 0; i < chunks->size; i++){
        *usedOut += chunks->chunks[i].alloced;
        *unusedOut += chunks->chunks[i].remaining;
    }
}

// Beacon memory chunks.

static BeaconMemoryChunkArray bmChunks;

void beaconFreeAllMemory(void){
    freeAllChunkMemory(&bmChunks, 1);
}

void* beaconGetMemory(U32 size){
    return getChunkMemory(&bmChunks, size);
}

void beaconInitArray(Array* array, U32 count){
    array->storage = beaconGetMemory(sizeof(array->storage) * count);
    array->size = 0;
    array->maxSize = count;
}

void beaconInitCopyArray(Array* array, Array* source){
    beaconInitArray(array, source->size);
    memcpy(array->storage, source->storage, sizeof(source->storage[0]) * source->size);
    array->size = source->size;
}

// BeaconBlock memory chunks.

static BeaconMemoryChunkArray bbmChunks;

void beaconBlockFreeAllMemory(int freeAllMemory){
    freeAllChunkMemory(&bbmChunks, freeAllMemory);
}

void* beaconBlockGetMemory(U32 size){
    return getChunkMemory(&bbmChunks, size);
}

void beaconBlockInitArray(Array* array, U32 count){
    array->storage = beaconBlockGetMemory(sizeof(array->storage) * count);
    array->size = 0;
    array->maxSize = count;
}

void beaconBlockInitCopyArray(Array* array, Array* source){
    beaconBlockInitArray(array, source->size);
    memcpy(array->storage, source->storage, sizeof(source->storage[0]) * source->size);
    array->size = source->size;
}

// Debug thing.

void beaconGetBlockMemoryData(char* outString){
    U32 used;
    U32 unused;
    char* pos = outString;
    
    getChunkMemoryUsage(&bmChunks, &used, &unused);
    pos += sprintf(pos, "Beacon: %d/%d, ", used, unused);

    getChunkMemoryUsage(&bbmChunks, &used, &unused);
    pos += sprintf(pos, "Block: %d/%d", used, unused);
}

// Connection stuff.

static int usedPhysicsSteps;

typedef enum BeaconWalkResult {
    WALKRESULT_SUCCESS = 0,
    WALKRESULT_NO_LOS,
    WALKRESULT_BLOCKED_SHORT,
    WALKRESULT_BLOCKED_LONG,
    WALKRESULT_TOO_HIGH,
} BeaconWalkResult;

static BeaconWalkResult beaconCanWalkBetweenBeacons(Beacon* src, Beacon* dst, Vec3 startPos, Entity* e){
    MotionState* motion = e->motion;
    F32 distance;
    F32 minDistance = FLT_MAX;
    S32 badCount = 0;
    F32 dstFloorDistance = beaconGetFloorDistance(dst) + 1.4;
    BeaconWalkResult result;

    if(beaconGetCeilingDistance(src) < 4){
        return WALKRESULT_BLOCKED_SHORT;
    }    

    if(startPos){
        entUpdatePosInstantaneous(e, startPos);
    }else{
        Vec3 newpos;
        copyVec3(posPoint(src), newpos);
        vecY(newpos) -= beaconGetFloorDistance(src) - 0.1f;
        entUpdatePosInstantaneous(e, newpos);
    }
    
    copyVec3(ENTPOS(e), motion->last_pos);
    
    zeroVec3(motion->vel);
    motion->falling = 0;
    motion->stuck_head = 0;
    
    distance = distance3XZ(ENTPOS(e), posPoint(dst));

    global_motion_state.noEntCollisions = 1;

    while(    badCount < 5 &&
            (    motion->falling ||
                distance3SquaredXZ(ENTPOS(e), posPoint(dst)) > SQR(2) ||
                posY(dst) - ENTPOSY(e) > dstFloorDistance))
    {
        subVec3(dst->mat[3], ENTPOS(e), motion->input.vel);
        motion->input.vel[1] = 0;
        normalVec3(motion->input.vel);
        scaleVec3(motion->input.vel, 0.5f, motion->input.vel);
        
        e->timestep = 1;
        
        entMotion(e, NULL);
        
        usedPhysicsSteps++;
        
        distance = distance3XZ(ENTPOS(e), posPoint(dst));
        
        if(distance >= minDistance - 0.1f){
            badCount++;
        }

        if(distance < minDistance){
            minDistance = distance;
        }
    }
    
    global_motion_state.noEntCollisions = 0;
    
    {
        F32 dist_e_dst_SQR = distance3SquaredXZ(ENTPOS(e), posPoint(dst));
        F32 dist_src_e = distance3XZ(posPoint(src), ENTPOS(e));
        F32 dist_src_dst = distance3XZ(posPoint(src), posPoint(dst));
        F32 dy = posY(dst) - ENTPOSY(e);
        
        if(    dist_e_dst_SQR > SQR(10) &&
            dist_src_e < dist_src_dst + 2)
        {
            // Stopped a good distance away.
            
            //if(dist_src_e >= dist_src_dst + 2)
            //    printf("");
                
            result = WALKRESULT_BLOCKED_LONG;
        }
        else if(dist_e_dst_SQR > SQR(2)
                &&
                dist_src_e < dist_src_dst + 2)
        {
            // Stopped nearby-ish.
            
            result = WALKRESULT_BLOCKED_SHORT;
        }
        else if(dy > dstFloorDistance){
            result = WALKRESULT_TOO_HIGH;
        }
        else{
            result = WALKRESULT_SUCCESS;
        }
    }
    
    return result;
}

Entity* beaconCreatePathCheckEnt(void){
    static struct {
        Entity        entity;
        MotionState motionState;
        SeqInst        seq;
    }* entStuff;
    
    Entity* e;
    
    if(!entStuff){
        entStuff = calloc(sizeof(*entStuff), 1);
    }
    
    e = &entStuff->entity;

    e->motion = &entStuff->motionState;
    e->seq = &entStuff->seq;
    
    //assertmsg(e, "Failed: Can't create \"male\" entity.\n");
    
    e->move_type = MOVETYPE_WALK;
    SET_ENTTYPE(e) = ENTTYPE_CRITTER;
    
    setSurfaceModifiers(e, SURFPARAM_GROUND, 1, 1, 1, 1);
    setSurfaceModifiers(e, SURFPARAM_AIR, 0, 0, 0, 1);
    setSpeed(e, SURFPARAM_GROUND, 1);
    setSpeed(e, SURFPARAM_AIR, 1);
    
    return e;
}

int createCurve(Position* source, Position* target, BeaconCurvePoint* output, int pointCount, int makeLastPoint){
    int i;
    float a1, a2, c1, c2, d1, d2;
    float denom;
    float x, z;
    Vec3 vec1, vec2;
    float yaw1, yaw2;
    float r1, r2;
    float yawDiff;
    Vec3 pyr;

    // Calculate coefficients for source line.
    
    a1 = source->mat[2][0];
    if(fabs(a1) < 0.0001f)
        a1 = 0;
    c1 = source->mat[2][2];
    if(fabs(c1) < 0.0001f)
        c1 = 0;
    d1 = -( a1 * posX(source) + c1 * posZ(source) );
            
    // Calculate coefficients for target line.
    
    a2 = target->mat[2][0];
    if(fabs(a2) < 0.0001f)
        a2 = 0;
    c2 = target->mat[2][2];
    if(fabs(c2) < 0.0001f)
        c2 = 0;
    d2 = -( a2 * posX(target) + c2 * posZ(target) );
            
    denom = a2 * c1 - a1 * c2;
    
    // If denominator is zero, lines are parallel, so store a NULL curve.
    
    if (fabs(denom) < 0.001) {
        return 0;
    }
    
    z = ( a1 * d2 - a2 * d1 ) / denom;
    
    if(a1){
        x = - ( d1 + c1 * z ) / a1;
    }else if(a2){
        x = - ( d2 + c2 * z ) / a2;
    }else{
        return 0;
    }
    
    vec1[0] = posX(source) - x;
    vec1[1] = 0;
    vec1[2] = posZ(source) - z;

    r1 = sqrt(vec1[0] * vec1[0] + vec1[2] * vec1[2]);
    
    vec2[0] = posX(target) - x;
    vec2[1] = 0;
    vec2[2] = posZ(target) - z;

    r2 = sqrt(vec2[0] * vec2[0] + vec2[2] * vec2[2]);
    
    yaw1 = getVec3Yaw(vec1);
    yaw2 = getVec3Yaw(vec2);
    yawDiff = yaw2 - yaw1;
    
    if(yawDiff > RAD(180))
        yawDiff -= RAD(360);
    else if(yawDiff < RAD(-180))
        yawDiff += RAD(360);

    // Calculate the points along the curve by changing the yaw.
    
    for(i = 0; i < pointCount; i++){
        F32 factor = (F32)i / ( pointCount - (makeLastPoint ? 1 : 0) );
        F32 yaw = yaw1 + yawDiff * factor;
        F32 r = r1 + ( r2 - r1 ) * factor;

        output[i].pos[0] = x + r * fsin(yaw);
        output[i].pos[1] = posY(source) + factor * (posY(target) - posY(source));
        output[i].pos[2] = z + r * fcos(yaw);
    }

    // Set pitch and roll to zero.  Pitch is calculated on the fly.
    
    pyr[0] = 0;
    pyr[2] = 0;
        
    for(i = 0; i < pointCount; i++){
        Vec3 vectorToNext;
        float junk;

        // Roll is always zero.
        
        if(i == 0){
            getMat3YPR(source->mat, output[i].pyr);
        }
        else if(i < pointCount - 1){
            // Make each point's yaw point at the next point.
            
            subVec3(output[i + 1].pos, output[i].pos, vectorToNext);
            
            normalVec3(vectorToNext);

            getVec3YP(vectorToNext, &pyr[1], &junk);
            
            copyVec3(pyr, output[i].pyr);
        }
        else if(makeLastPoint){
            // Making the last point, so just take the target PYR.
            
            getMat3YPR(target->mat, output[i].pyr);
        }
        else{
            // Not making last point, so make the last point's yaw towards the target position.
            
            subVec3(posPoint(target), output[i].pos, vectorToNext);
            
            normalVec3(vectorToNext);

            getVec3YP(vectorToNext, &pyr[1], &junk);
            
            copyVec3(pyr, output[i].pyr);
        }
    }
    
    return 1;
}

BeaconCurvePoint* beaconGetCurve(Beacon* source, Beacon* target){
    BeaconCurvePoint positions[BEACON_CURVE_POINT_COUNT];
    BeaconCurvePoint* curvePointArray = NULL;
    
    // If this is a curve-head beacon, create the curve.
    
    if(source->trafficGroupFlags & 1){
        if(createCurve((Position*)source, (Position*)target, positions, BEACON_CURVE_POINT_COUNT, 1)){
            curvePointArray = calloc(1, sizeof(positions));
            
            memcpy(curvePointArray, positions, sizeof(positions));
        }
    }
    else{
        Vec3 sourcePYR;
        Vec3 targetPYR;
        Vec3 newDirection;
        float newHeading;
        float deltaHeading1, deltaHeading2;

        getMat3YPR(source->mat, sourcePYR);

        // Find the yaw from the source to the target
        //    Find direction vector from source to target
        
        subVec3(target->mat[3], source->mat[3], newDirection);

        // Extract yaw from direction vector
        
        newHeading = getVec3Yaw(newDirection);

        // Extract target PYR
        
        getMat3YPR(target->mat, targetPYR);

        // Is the target beacon within the source beacon's FOV?
        
        deltaHeading1 = subAngle(sourcePYR[1], newHeading);
        
        // Are the two arrows pointing at radically different directions?
        
        deltaHeading2 = subAngle(sourcePYR[1], targetPYR[1]);
        
        if(    deltaHeading1 > RAD(10) || deltaHeading1 < RAD(-10) ||
            deltaHeading2 > RAD(5) || deltaHeading2 < RAD(-5))
        {
            Position center;
            Vec3 centerDir;
            Vec3 pyr;
            
            // Get the center point.
            
            addVec3(source->mat[3], target->mat[3], center.mat[3]);
            scaleVec3(center.mat[3], 0.5, center.mat[3]);
            
            // Get the center direction.
            
            subVec3(target->mat[3], source->mat[3], centerDir);
            normalVec3(centerDir);
            
            // Get the center Mat4.
            
            getVec3YP(centerDir, &pyr[1], &pyr[0]);
            pyr[2] = 0;
            
            createMat3YPR(center.mat, pyr);
            
            // Create the curve.
            
            if(createCurve((Position*)source, &center, positions, (BEACON_CURVE_POINT_COUNT + 1) / 2, 0)){
                if(createCurve(&center, (Position*)target, positions + (BEACON_CURVE_POINT_COUNT + 1) / 2, BEACON_CURVE_POINT_COUNT / 2, 1)){
                    curvePointArray = calloc(1, sizeof(positions));
                    
                    memcpy(curvePointArray, positions, sizeof(positions));
                }
            }
        }
    }
    
    return curvePointArray;
}
                
/* Function beaconGetPassableHeight()
 *    Checks to see if there is a passage between two beacons somewhere above ground.  If
 *    a passable does exist, the "highest" and "lowest" passable heights are modified to
 *    their approximate passable height (accurate within 10 vertical feet).  Otherwise,
 *    "highest" and "lowest" are not modified.
 *
 *    Parameters:
 *        source
 *        target - beacons to be used
 *        highest [out] - highest passable height
 *        lowest [out] - lowest passable height
 *    Returns:
 *        Whether a passage exists between the source beacon and the target beacon.
 */
int beaconGetPassableHeight(Beacon* source, Beacon* target, float* highest, float* lowest){
    CollInfo info;
    float sourceCeiling, destCeiling;
    float commonCeiling, commonFloor;
    Vec3 horizRayStart, horizRayEnd;
    float curHeight;
    int lowestFound = 0;
    int highestFound = 0;
    float startHeight = *lowest;
    float endHeight = *highest;

    // Get ceiling height above source.

    sourceCeiling = posY(source) + beaconGetCeilingDistance(source);

    // Get ceiling height above target.

    destCeiling = posY(target) + beaconGetCeilingDistance(target);
    
    commonCeiling = MIN(MIN(sourceCeiling, destCeiling), endHeight);
    commonFloor = MAX(MAX(posY(source), posY(target)), startHeight);

    if(commonFloor > commonCeiling) // Not possible for a passage to exist
        return 0;

    copyVec3(source->mat[3], horizRayStart);
    copyVec3(target->mat[3], horizRayEnd);

    // Test for collisions every ten feet above the beacons, up to the common ceiling height

    for(curHeight = commonFloor; curHeight < commonCeiling; curHeight += 2.5){
        int collision;
        
        // Move the horizontal ray end points
        vecY(horizRayStart) = curHeight;
        vecY(horizRayEnd) = curHeight;
        
        collision = collGrid(NULL, horizRayStart, horizRayEnd, &info, 1.8, COLL_NOTSELECTABLE | COLL_HITANY | COLL_ENTBLOCKER | COLL_BOTHSIDES);
        
        if(!collision){
            // There was no collision, record passable height
            // Limit the number of collsion checks to a reasonable number
            //openingsFound++;
            //if(openingsFound >= 5)
            //    break;

            if(!lowestFound){
                *lowest = curHeight;
                lowestFound = 1;
            }
            else{
                *highest = curHeight;
                highestFound = 1;
            }
        }
        else if(lowestFound){
            if(highestFound){
                // There was a collision, stop checking for stuff.
                break;
            }else{
                lowestFound = 0;
            }
        }
    }

    if(!lowestFound || !highestFound){
        return 0;
    }
    
    *highest -= posY(source);
    *lowest -= posY(source);

    return 1;
}

static int beaconNearbyCondTraffic(Beacon* b, Beacon* ref, float radius){
    CollInfo info;

    return    posWithinDistance(b->mat, ref->mat, radius) &&
            (    fabs( posY(b) - posY(ref) ) < 20.0f ||
                !collGrid(NULL, posPoint(ref), posPoint(b), &info, 0, COLL_NOTSELECTABLE | COLL_HITANY | COLL_BOTHSIDES));
}

static int beaconIsVisibleFromBeacon(Vec3 src, Vec3 dst){
    CollInfo coll;

    if(!collGrid(NULL, src, dst, &coll, 0, COLL_NOTSELECTABLE | COLL_HITANY | COLL_BOTHSIDES)){
        return 1;
    }

    return 0;
}

int beaconWithinRadiusXZ(Beacon* source, Beacon* target, float extraDistance){
    float maxdist = max(source->proximityRadius, target->proximityRadius) + extraDistance;
    
    return distance3SquaredXZ(posPoint(source), posPoint(target)) < SQR(maxdist);
}

static BeaconWalkResult last_walk_result;

int beaconConnectsToBeaconByGround(    Beacon* source,
                                    Beacon* target,
                                    Vec3 startPos,
                                    F32 maxRaiseHeight,
                                    Entity* ent,
                                    S32 createEnt)
{
    Vec3 srcPos, dstPos;
    int visible = 0;
    int canWalk;
    int ownsEnt = !ent;
    int i;
    
    if(!ent){
        ent = createEnt ? beaconCreatePathCheckEnt() : NULL;
    }
    
    maxRaiseHeight = min(maxRaiseHeight, beaconGetCeilingDistance(source) - 1);
    maxRaiseHeight = min(maxRaiseHeight, beaconGetCeilingDistance(target) - 1);
    
    copyVec3(posPoint(source), srcPos);
    copyVec3(posPoint(target), dstPos);
    
    for(i = 0; i <= maxRaiseHeight; i++){
        copyVec3(posPoint(source), srcPos);
        copyVec3(posPoint(target), dstPos);

        srcPos[1] += min(i, maxRaiseHeight);
        dstPos[1] += min(i, maxRaiseHeight);

        if(beaconIsVisibleFromBeacon(srcPos, dstPos)){
            visible = 1;                                                                            
            break;
        }
    }
        
    if(visible && !ent){
        last_walk_result = WALKRESULT_SUCCESS;
    }else{
        last_walk_result = beaconCanWalkBetweenBeacons(source, target, startPos, ent);
        
        if(startPos && last_walk_result == WALKRESULT_SUCCESS){
            if(distance3SquaredXZ(startPos, posPoint(source)) > SQR(5)){
                last_walk_result = beaconCanWalkBetweenBeacons(source, target, NULL, ent);
            }
        }
    }
                        
    canWalk = last_walk_result == WALKRESULT_SUCCESS;

    //if(ownsEnt && ent)
    //    entFree(ent);
        
    return canWalk && visible;
}

#define NPC_BEACON_RADIUS 3.f    // replace this with the mapspec one
int beaconConnectsToBeaconNPC(Beacon* source, Beacon* target){
    Vec3 dir;
    Vec3 pyr;
    Mat3 mat;
    int i;
    int passedCount = 0, failedCount = 0;
    
    subVec3(posPoint(target), posPoint(source), dir);
    
    getVec3YP(dir, &pyr[1], &pyr[0]);
    
    pyr[2] = 0;
    
    createMat3YPR(mat, pyr);
    
    for(i = -6; i <= 6; i++){
        Vec3 p1, p2;
        
        scaleVec3(mat[0], i * NPC_BEACON_RADIUS / 6.f, dir);
        
        addVec3(posPoint(source), dir, p1);
        addVec3(posPoint(target), dir, p2);

        if(beaconIsVisibleFromBeacon(p1, p2)){
            if(++passedCount == 8){
                return 1;
            }
        }
        else{
            if(++failedCount == 6){
                return 0;
            }
        }
    }
    
    return 0;
}

int beaconCheckEmbedded(Beacon* source){
    Vec3 p1, p2, dir;

    zeroVec3(dir);
    vecX(dir) += NPC_BEACON_RADIUS;
    addVec3(posPoint(source), dir, p1);
    subVec3(posPoint(source), dir, p2);

    if(!beaconIsVisibleFromBeacon(p1, p2))
        return 0;

    zeroVec3(dir);
    vecZ(dir) += NPC_BEACON_RADIUS;
    addVec3(posPoint(source), dir, p1);
    subVec3(posPoint(source), dir, p2);

    if(!beaconIsVisibleFromBeacon(p1, p2))
        return 0;
    else
        return 1;
}

static int beaconCanConnectToTrafficBeacon(Beacon* source, Vec3 sourcePYR, Beacon* target){
    Vec3 targetPYR;
    Vec3 newDirection;
    float newHeading;
    float deltaHeading1;
    float deltaHeading2;
    float deltaHeading3;

    if(source == target || !beaconNearbyCondTraffic(target, source, source->proximityRadius)){
        return 0;
    }
    
    // If this is a curve-head beacon, only go to matching target beacons.
    
    if(source->trafficGroupFlags & 1){
        // If target doesn't match any ~1 bits, then skip this beacon.
        if((target->trafficGroupFlags & 1) || !((target->trafficGroupFlags & ~1) & source->trafficGroupFlags)){
            return 0;
        }
    }
    
    // The type number must match, if either is non-zero.
    
    if(source->trafficTypeNumber != target->trafficTypeNumber){
        return 0;
    }

    // Find the yaw from the source to the target.
    //    Find direction vector from source to target.
    
    subVec3(target->mat[3], source->mat[3], newDirection);

    // Extract yaw from direction vector.
    
    newHeading = getVec3Yaw(newDirection);

    // Extract target PYR.
    
    getMat3YPR(target->mat, targetPYR);

    // Is the target beacon within the source beacon's FOV?
    
    deltaHeading1 = subAngle(sourcePYR[1], newHeading);
    
    // Are the two arrows pointing at radically different directions?
    
    deltaHeading2 = subAngle(sourcePYR[1], targetPYR[1]);
    
    // Get angle between the vector from source to target and the V vector of target,
    // which must be within 90 degrees of each other.
    
    deltaHeading3 = subAngle(getVec3Yaw(newDirection), targetPYR[1]);
    
    if( deltaHeading1 < RAD(80) && deltaHeading1 > RAD(-80) &&
        deltaHeading2 < RAD(120) && deltaHeading2 > RAD(-120) &&
        deltaHeading3 < RAD(90) && deltaHeading3 > RAD(-90))
    {
        return 1;
    }
    
    return 0;
}

static int beaconCanConnectToTrafficBeaconContingency(Beacon* source, Vec3 sourcePYR, Beacon* target){
    Vec3 newDirection;
    float newHeading;
    Vec3 targetPYR;
    float deltaHeading1;
    float deltaHeading2;

    if(source == target){
        return 0;
    }
    
    // Find the yaw from the source to the target
    //    Find direction vector from source to target
    
    subVec3(posPoint(target), posPoint(source), newDirection);

    // Extract yaw from direction vector
    
    newHeading = getVec3Yaw(newDirection);

    // Extract target PYR
    
    getMat3YPR(target->mat, targetPYR);

    // Are the two arrows pointing at radically different directions?
    
    deltaHeading1 = subAngle(sourcePYR[1], targetPYR[1]);
    
    // Get angle between the vector from source to target and the V vector of target,
    // which must be within 120 degrees of each other.
    
    deltaHeading2 = subAngle(getVec3Yaw(newDirection), targetPYR[1]);
    
    if( deltaHeading1 > RAD(120) || deltaHeading1 < RAD(-120) ||
        deltaHeading2 > RAD(120) || deltaHeading2 < RAD(-120))
    {
        return 0;
    }
    
    return 1;
}

void beaconConnectToSet(Beacon* source){
    Array* beaconSet;
    int i;

    assert(source->type >= 0 && source->type < BEACONTYPE_COUNT);
    
    beaconSet = beaconsByTypeArray[source->type];
    
    if(!beaconSet)
        return;
    
    source->madeGroundConnections = 1;

    // Are we looking at traffic beacons?  Traffic beacons are special!
    
    switch(source->type){
        case BEACONTYPE_TRAFFIC:{
            // Traffic beacons are directional, and have ground connections only.

            Beacon* target;
            Vec3 sourcePYR;

            PERFINFO_AUTO_START("trafficConn", 1);

            // Extract the source beacon's PYR

            getMat3YPR(source->mat, sourcePYR);

            for(i = 0; i < beaconSet->size; i++){
                target = beaconSet->storage[i];
                
                if(beaconCanConnectToTrafficBeacon(source, sourcePYR, target)){
                    BeaconConnection* conn = createBeaconConnection();
                    
                    conn->destBeacon = target;
                    
                    arrayPushBack(&source->gbConns, conn);
                    
                    arrayPushBack(&source->curveArray, beaconGetCurve(source, target));
                }
            }
            
            if(!source->gbConns.size){
                // No connections found, switch to contingency plan.
                
                sourcePYR[1] = addAngle(sourcePYR[1], RAD(180));
                
                for(i = 0; i < beaconSet->size; i++){
                    target = beaconSet->storage[i];
                    
                    if(beaconCanConnectToTrafficBeaconContingency(source, sourcePYR, target)){
                        BeaconConnection* conn;
                        
                        if(source->gbConns.size){
                            conn = source->gbConns.storage[0];
                            
                            // Check if this connection is closer than the previous connection.
                            
                            if(    distance3Squared(posPoint(source), posPoint(target)) <
                                distance3Squared(posPoint(source), posPoint(conn->destBeacon)))
                            {
                                conn->destBeacon = target;
                            }
                        }else{
                            conn = createBeaconConnection();

                            conn->destBeacon = target;

                            arrayPushBack(&source->gbConns, conn);
                        }
                    }
                }

                // Create the curve array if a suitable beacon was found.
                
                if(source->gbConns.size){
                    target = ((BeaconConnection*)source->gbConns.storage[0])->destBeacon;

                    arrayPushBack(&source->curveArray, beaconGetCurve(source, target));
                }
            }

            PERFINFO_AUTO_STOP();
            
            break;
        }
        
        case BEACONTYPE_BASIC:{
            if(source->isEmbedded){
                return;
            }

            for(i = 0; i < beaconSet->size; i++){
                Beacon* target = beaconSet->storage[i];

                if(target != source && !target->isEmbedded &&
                        (source->proximityRadius > target->proximityRadius ||
                        source->proximityRadius == target->proximityRadius && posX(source) <= posX(target)) &&
                        beaconWithinRadiusXZ(source, target, 3)){
                    if(beaconConnectsToBeaconNPC(source, target)){
                        // Create a ground connection.
                        
                        BeaconConnection* conn = createBeaconConnection();
                        
                        conn->destBeacon = target;

                        arrayPushBack(&source->gbConns, conn);

                        conn = createBeaconConnection();

                        conn->destBeacon = source;

                        arrayPushBack(&target->gbConns, conn);
                    }
                }
            }

            break;
        }
    }
}

static int beaconHasGroundConnectionToBeacon(Beacon* source, Beacon* target, int* index){
    BeaconConnection** conns = (BeaconConnection**)source->gbConns.storage;
    int lo = 0;
    int hi = source->gbConns.size - 1;
    
    while(lo <= hi){
        int mid = (hi + lo) / 2;
        Beacon* midBeacon = conns[mid]->destBeacon;
        
        if((ptrdiff_t)target > (ptrdiff_t)midBeacon){
            lo = mid + 1;
        }
        else if((ptrdiff_t)target < (ptrdiff_t)midBeacon){
            hi = mid - 1;
        }
        else{
            if(index){
                *index = mid;
            }
            
            return 1;
        }
    }

    return 0;
}

static int beaconHasRaisedConnection(Beacon* source, BeaconConnection* targetConn, int* index){
    BeaconConnection** conns = (BeaconConnection**)source->rbConns.storage;
    int lo = 0;
    int hi = source->rbConns.size - 1;
    
    while(lo <= hi){
        int mid = (hi + lo) / 2;
        BeaconConnection* midConn = conns[mid];
        
        if((ptrdiff_t)targetConn > (ptrdiff_t)midConn){
            lo = mid + 1;
        }
        else if((ptrdiff_t)targetConn < (ptrdiff_t)midConn){
            hi = mid - 1;
        }
        else{
            if(index){
                *index = mid;
            }
            
            return 1;
        }
    }

    return 0;
}

static Array tempBeacons;
static Array tempSubBlocks;

static int __cdecl compareBeaconConnections(const void* c1pData, const void* c2pData){
    const BeaconConnection** c1p = (const BeaconConnection**)c1pData;
    const BeaconConnection** c2p = (const BeaconConnection**)c2pData;
    const BeaconConnection* c1 = *c1p;
    const BeaconConnection* c2 = *c2p;
    
    if(c1 < c2){
        return -1;
    }
    else if(c1 == c2){
        return 0;
    }
    else{
        return 1;
    }
}

static int __cdecl compareBeaconConnectionTargets(const void* c1pData, const void* c2pData){
    const BeaconConnection** c1p = (const BeaconConnection**)c1pData;
    const BeaconConnection** c2p = (const BeaconConnection**)c2pData;
    const BeaconConnection* c1 = *c1p;
    const BeaconConnection* c2 = *c2p;
    
    if(c1->destBeacon < c2->destBeacon){
        return -1;
    }
    else if(c1->destBeacon == c2->destBeacon){
        return 0;
    }
    else{
        return 1;
    }
}

static void beaconSortConnsByTarget(Beacon* beacon, int raised){
    if(raised){
        if(beacon->raisedConnsSorted)
            return;
            
        beacon->raisedConnsSorted = 1;

        qsort(    beacon->rbConns.storage,
                beacon->rbConns.size,
                sizeof(beacon->rbConns.storage[0]),
                compareBeaconConnections);
    }else{
        if(beacon->groundConnsSorted)
            return;
            
        beacon->groundConnsSorted = 1;

        qsort(    beacon->gbConns.storage,
                beacon->gbConns.size,
                sizeof(beacon->gbConns.storage[0]),
                compareBeaconConnectionTargets);
    }
}

static void beaconSplitBlock(BeaconBlock* gridBlock){
    Beacon** beaconArray = (Beacon**)gridBlock->beaconArray.storage;
    int beaconsRemaining = gridBlock->beaconArray.size - 1;
    int placedCount = 0;
    Beacon* beacon;
    BeaconBlock* subBlock = NULL;
    int i;

    // Check if the block has already been split.
    
    if(gridBlock->subBlockArray.size){
        return;
    }
    
    // Use searchInstance as a temporary index variable for swapping.

    PERFINFO_AUTO_START("sortConns", 1);
        for(i = 0; i < gridBlock->beaconArray.size; i++){

            beacon = beaconArray[i];
            
            beacon->block = gridBlock;
            beacon->searchInstance = i;

            if(!beacon->groundConnsSorted){
                beaconSortConnsByTarget(beacon, 0);
            }
            
            //{
            //    int j;
            //    for(j = 1; j < beacon->gbConns.size; j++){
            //        assert(    ((BeaconConnection*)beacon->gbConns.storage[j])->destBeacon > 
            //                ((BeaconConnection*)beacon->gbConns.storage[j-1])->destBeacon);
            //    }
            //}
        }
    PERFINFO_AUTO_STOP();
        
    tempSubBlocks.size = 0;
    
    for(i = 0; i < gridBlock->beaconArray.size; i++){
        int connCount;
        int j;
        
        beacon = beaconArray[i];
        
        if(beacon->block == gridBlock){
            // Time to start a new subBlock.
            
            if(subBlock){
                beaconBlockInitCopyArray(&subBlock->beaconArray, &tempBeacons);
            }
            
            tempBeacons.size = 0;
        
            subBlock = createBeaconBlock();
            subBlock->parentBlock = gridBlock;
            subBlock->pathLimitedAllowed = beacon->pathLimitedAllowed;
            
            arrayPushBack(&tempBeacons, beacon);
            arrayPushBack(&tempSubBlocks, subBlock);
            
            beacon->block = subBlock;
            assert(placedCount == i);
            placedCount++;
        }else{
            assert(!beacon->block->isGridBlock);
            subBlock = beacon->block;
            assert(subBlock->parentBlock == gridBlock);
        }
        
        connCount = beacon->gbConns.size;
        
        for(j = 0; j < connCount; j++){
            BeaconConnection* conn = beacon->gbConns.storage[j];
            Beacon* destBeacon = conn->destBeacon;
            
            if(    destBeacon->block == gridBlock &&
                destBeacon->pathLimitedAllowed == beacon->pathLimitedAllowed &&
                beaconHasGroundConnectionToBeacon(destBeacon, beacon, NULL))
            {
                // Swap with first unplaced beacon.
                
                Beacon* tempBeacon = beaconArray[placedCount];
                
                assert(destBeacon->searchInstance >= placedCount);
                assert(tempBeacon->searchInstance == placedCount);

                tempBeacon->searchInstance = destBeacon->searchInstance;
                destBeacon->searchInstance = placedCount;

                beaconArray[destBeacon->searchInstance] = destBeacon;
                beaconArray[tempBeacon->searchInstance] = tempBeacon;

                placedCount++;
                
                // Put destBeacon in the current subBlock.

                destBeacon->block = subBlock;
                
                arrayPushBack(&tempBeacons, destBeacon);
            }
        }
    }
    
    if(subBlock){
        beaconBlockInitCopyArray(&subBlock->beaconArray, &tempBeacons);
    }

    beaconBlockInitCopyArray(&gridBlock->subBlockArray, &tempSubBlocks);

    assert(placedCount == gridBlock->beaconArray.size);
    
    // Find the center position of each sub-block by averaging the positions of all the beacons in it.
    
    for(i = 0; i < gridBlock->subBlockArray.size; i++){
        BeaconBlock* subBlock = gridBlock->subBlockArray.storage[i];
        int j;
        Vec3 pos = {0,0,0};
        
        for(j = 0; j < subBlock->beaconArray.size; j++){
            Beacon* beacon = subBlock->beaconArray.storage[j];
            
            addVec3(pos, posPoint(beacon), pos);
        }
        
        scaleVec3(pos, 1.0f / subBlock->beaconArray.size, subBlock->pos);
    }
}

F32 beaconMakeGridBlockCoord(F32 xyz){
    return floor(xyz / combatBeaconGridBlockSize);
}

void beaconMakeGridBlockCoords(Vec3 v){
    v[0] = beaconMakeGridBlockCoord(v[0]);
    v[1] = beaconMakeGridBlockCoord(v[1]);
    v[2] = beaconMakeGridBlockCoord(v[2]);
}

int beaconMakeGridBlockHashValue(int x, int y, int z){
    return ((0x3<<30) | ((x&0x3ff)<<20) | ((y&0x3ff)<<10) | (z&0x3ff));
    //sprintf(hashStringOut, "%d,%d,%d", x, y, z);
}

BeaconBlock* beaconGetGridBlockByCoords(int x, int y, int z, int create){
    if (combatBeaconGridBlockTable)
    {
        int blockHashValue = beaconMakeGridBlockHashValue(x, y, z);
        BeaconBlock* block;    // stash will NULL if not found

        if(!stashIntFindPointer(combatBeaconGridBlockTable, blockHashValue, &block) && create){
            block = createBeaconBlock();

            setVec3(block->pos, x, y, z);
            block->isGridBlock = 1;

            stashIntAddPointer(combatBeaconGridBlockTable, blockHashValue, block, false);

            arrayPushBack(&combatBeaconGridBlockArray, block);
        }
        return block;
    }
    else
    {
        return NULL;
    }
}

Array* beaconGetGroundConnections(Beacon* source, Array* beaconSet){
    if(!source){
        return NULL;
    }

    // If the nearby beacons have already been found, return it directly
    
    if(!source->madeGroundConnections){
        //static int i = 0;
        
        //printf("Connecting beacon to set: %d... ", i++);

        beaconConnectToSet(source);
        
        //printf("done.\n");
    }
        
    return &source->gbConns;
}

static F32 getMinJumpHeight(Beacon* source, BeaconConnection* conn){
    F32 distXZ = distance3XZ(posPoint(source), posPoint(conn->destBeacon));
    F32 minJumpHeight = distXZ / 2;
    F32 minHeight = conn->minHeight + beaconGetFloorDistance(source);
    
    return (minJumpHeight < minHeight) ? minHeight : minJumpHeight;
}

static BeaconBlockConnection* findBlockConnection(Array* connsArray, BeaconBlock* destBlock, int* newIndex){
    BeaconBlockConnection** conns = (BeaconBlockConnection**)connsArray->storage;
    int size = connsArray->size;
    int lo = 0;
    int hi = size - 1;
    
    while(lo <= hi){
        int mid = (hi + lo) / 2;
        BeaconBlock* midBlock = conns[mid]->destBlock;
        
        if((ptrdiff_t)destBlock > (ptrdiff_t)midBlock){
            lo = mid + 1;
        }
        else if((ptrdiff_t)destBlock < (ptrdiff_t)midBlock){
            hi = mid - 1;
        }
        else{
            return conns[mid];
        }
    }

    // Hopefully at this point, lo contains the index that the new connection should be inserted into.
    
    if(newIndex){
        *newIndex = lo;
    }

    return NULL;
}

static Array tempGroundConns;
static Array tempRaisedConns;

void beaconMakeBlockConnections(BeaconBlock* gridBlock){
    int subBlockCount = gridBlock->subBlockArray.size;
    BeaconBlock** subBlocks = (BeaconBlock**)gridBlock->subBlockArray.storage;
    int i;
    
    for(i = 0; i < subBlockCount; i++){
        BeaconBlock*    subBlock = subBlocks[i];
        Beacon**        beacons = (Beacon**)subBlock->beaconArray.storage;
        int                beaconCount = subBlock->beaconArray.size;
        int                k;
        
        tempGroundConns.size = tempRaisedConns.size = 0;
        
        for(k = 0; k < beaconCount; k++){
            Beacon*                    beacon = beacons[k];
            BeaconConnection*        beaconConn;
            BeaconBlock*            destBlock;
            BeaconBlockConnection*    blockConn;
            int                     newIndex;
            int                     j;

            // Check the ground connections.
            
            for(j = 0; j < beacon->gbConns.size; j++){
                beaconConn = beacon->gbConns.storage[j];
                destBlock = beaconConn->destBeacon->block;
                
                assert(!destBlock->isGridBlock);
                    
                if(destBlock == subBlock)
                    continue;
                    
                // Look for an existing connection to destBlock.
                
                //PERFINFO_AUTO_START("findBlockConnection - ground", 1);
                    blockConn = findBlockConnection(&tempGroundConns, destBlock, &newIndex);
                //PERFINFO_AUTO_STOP();
                
                if(!blockConn){
                    // New connection.
                    
                    assert(newIndex >= 0 && newIndex <= tempGroundConns.size);
                
                    blockConn = createBeaconBlockConnection();
                    blockConn->destBlock = destBlock;
                    
                    arrayPushBack(&tempGroundConns, NULL);
                    
                    if(newIndex != tempGroundConns.size - 1){
                        // Nudge everything after the new element forward.
                        
                        //PERFINFO_AUTO_START("memmove - ground", 1);
                            CopyStructs(tempGroundConns.storage + newIndex + 1,
                                        tempGroundConns.storage + newIndex,
                                        tempGroundConns.size - newIndex - 1);
                        //PERFINFO_AUTO_STOP();
                    }

                    tempGroundConns.storage[newIndex] = blockConn;
                }
            }

            // Check the raised connections.
            
            for(j = 0; j < beacon->rbConns.size; j++){
                F32 minJumpHeight;
                F32 beaconHeight;
                F32 minHeight;
                F32 maxHeight;

                beaconConn = beacon->rbConns.storage[j];
                destBlock = beaconConn->destBeacon->block;
                
                assert(!destBlock->isGridBlock);
                    
                if(destBlock == subBlock)
                    continue;

                minJumpHeight = getMinJumpHeight(beacon, beaconConn);
                beaconHeight = beaconGetFloorDistance(beacon);
                minHeight = beaconHeight + beaconConn->minHeight;
                maxHeight = beaconHeight + beaconConn->maxHeight;

                // Look for an existing connection to destBlock.
                
                //PERFINFO_AUTO_START("findBlockConnection - raised", 1);
                    blockConn = findBlockConnection(&tempRaisedConns, destBlock, &newIndex);
                //PERFINFO_AUTO_STOP();

                if(blockConn){
                    if(minJumpHeight < blockConn->minJumpHeight){
                        blockConn->minJumpHeight = minJumpHeight;
                    }
                    
                    if(minHeight < blockConn->minHeight){
                        blockConn->minHeight = beaconConn->minHeight;
                    }
                    
                    if(maxHeight > blockConn->maxHeight){
                        blockConn->maxHeight = maxHeight;
                    }
                }else{
                    // New connection.
                    
                    F32 minJumpHeight = getMinJumpHeight(beacon, beaconConn);
                    
                    assert(newIndex >= 0 && newIndex <= tempRaisedConns.size);
                
                    blockConn = createBeaconBlockConnection();
                    blockConn->destBlock = destBlock;
                    blockConn->minHeight = minHeight;
                    blockConn->maxHeight = maxHeight;
                    blockConn->minJumpHeight = minJumpHeight;

                    arrayPushBack(&tempRaisedConns, NULL);

                    if(newIndex != tempRaisedConns.size - 1){
                        // Nudge everything after the new element forward.
                        
                        //PERFINFO_AUTO_START("memmove - raised", 1);
                            CopyStructs(tempRaisedConns.storage + newIndex + 1,
                                        tempRaisedConns.storage + newIndex,
                                        tempRaisedConns.size - newIndex - 1);
                        //PERFINFO_AUTO_STOP();
                    }

                    tempRaisedConns.storage[newIndex] = blockConn;
                }
            }
        }
        
        // Copy the connections into permanent storage.

        beaconBlockInitCopyArray(&subBlock->gbbConns, &tempGroundConns);
        beaconBlockInitCopyArray(&subBlock->rbbConns, &tempRaisedConns);
    }
}

void* beaconAllocateMemory(U32 size){
    void* mem = malloc(size);
    
    if(!mem){
        assertmsg(0, "Beaconizer out of memory.");
        return NULL;
    }
    
    ZeroMemory(mem, size);
    
    beacon_process.memoryAllocated += size;
    
    return mem;
}

static struct {
    Array    array;
    F32        minRadiusSQR;
    F32        maxRadiusSQR;
} compileNearbyData;

static void beaconCompileNearbyBeaconsHelper(Array* beaconArray, void* userData){
    Beacon* source = compileNearbyData.array.storage[0];
    int i;
    
    for(i = 0; i < beaconArray->size; i++){
        Beacon* target = beaconArray->storage[i];
        F32 distSQR;

        if(target == source)
            continue;    
            
        distSQR = distance3SquaredXZ(posPoint(source), posPoint(target));

        if(distSQR >= compileNearbyData.minRadiusSQR && distSQR < compileNearbyData.maxRadiusSQR){
            if(compileNearbyData.array.size < compileNearbyData.array.maxSize){
                compileNearbyData.array.storage[compileNearbyData.array.size++] = target;
            }
        }
    }
}

static Array* beaconCompileNearbyBeacons(Beacon* source, F32 minRadius, F32 maxRadius){
    static Beacon** tempBeaconBuffer;
    static Array tempArray;

    if(!tempBeaconBuffer){
        tempBeaconBuffer = calloc(sizeof(*tempBeaconBuffer), 10000);
        
        tempArray.maxSize = 9999;
        tempArray.storage = tempBeaconBuffer + 1;
    }
    
    compileNearbyData.array.size = 1;
    compileNearbyData.array.maxSize = 10000;
    compileNearbyData.array.storage = tempBeaconBuffer;
    compileNearbyData.array.storage[0] = source;
    compileNearbyData.minRadiusSQR = SQR(minRadius);
    compileNearbyData.maxRadiusSQR = SQR(maxRadius);
    
    beaconForEachBlock(posPoint(source), 100, scene_info.maxHeight + 2000, 100, beaconCompileNearbyBeaconsHelper, NULL);
    
    tempArray.size = compileNearbyData.array.size - 1;

    return &tempArray;
}

static int __cdecl compareBeaconProcessConnectionDistanceXZ(const void* b1Data, const void* b2Data){
    const BeaconProcessConnection* b1 = (const BeaconProcessConnection*)b1Data;
    const BeaconProcessConnection* b2 = (const BeaconProcessConnection*)b2Data;
    if(b1->distanceXZ > b2->distanceXZ){
        return 1;
    }
    else if(b1->distanceXZ < b2->distanceXZ){
        return -1;
    }
    else{
        int index1 = b1->targetIndex;
        int index2 = b2->targetIndex;

        if(index1 > index2)
            return 1;
        else if(index1 < index2)
            return -1;
    }

    assert(0);

    return 0;
}

static void beaconCreateBeaconProcessInfo(Beacon* source){
    BeaconProcessInfo* info = beacon_process.infoArray + source->userInt;
    BeaconProcessConnection* conns;
    int connCount;
    Array* beaconArray;
    int i;
    
    beaconArray = beaconCompileNearbyBeacons(source, 0, 100);
    
    info->beaconCount = connCount = beaconArray->size;
    info->beacons = conns = beaconAllocateMemory(connCount * sizeof(*info->beacons));

    //printf("allocating: %d nearby beacons = %d bytes\n", beaconCount, beaconCount * sizeof(*info->beacons));
    
    for(i = 0; i < connCount; i++){
        Vec3 diff;
        float yaw;
        Beacon* target = beaconArray->storage[i + 1];
        
        conns[i].targetIndex = target->userInt;

        // Calculate the distance from source.
        
        conns[i].distanceXZ = floor(distance3XZ(posPoint(source), posPoint(target)) + 0.5);
        
        // Calculate the yaw from source.
        
        subVec3(posPoint(target), posPoint(source), diff);
        
        yaw = getVec3Yaw(diff);
        
        conns[i].yaw90 = floor(DEG(yaw) / 2 + 0.5);
    }
    
    // Sort the list by distance ascending.
    
    qsort(conns, connCount, sizeof(*conns), compareBeaconProcessConnectionDistanceXZ);
}

static void beaconCreateRaisedConnections(    Beacon* source,
                                            BeaconProcessConnection* conn,
                                            F32 openMin,
                                            F32 openMax,
                                            F32 ignoreMin,
                                            F32 ignoreMax)
{
    BeaconProcessRaisedConnection tempRaisedConns[1000];
    int raisedCount = 0;
    float myMin, myMax;
    Beacon* target = combatBeaconArray.storage[conn->targetIndex];
    int i;
    
    //return;
    
    PERFINFO_AUTO_START("getPassableHeight", 1);
    
        if(1){
            // Create the lower raised connections.
                
            myMin = posY(source) + 0.1;
            myMin = max(myMin, openMin);
            myMax = ignoreMin;
            
            while(    raisedCount < ARRAY_SIZE(tempRaisedConns) &&
                    beaconGetPassableHeight(source, target, &myMax, &myMin))
            {
                tempRaisedConns[raisedCount].minHeight = posY(source) + myMin;
                tempRaisedConns[raisedCount].maxHeight = posY(source) + myMax;

                raisedCount++;

                myMin = posY(source) + myMax;
                myMax = ignoreMin;
            }

            if(ignoreMax > ignoreMin){
                F32 highMax = min(openMax, FLT_MAX);
                
                // Create the higher raised connections.
                
                myMin = ignoreMax;
                myMax = highMax;

                while(    raisedCount < ARRAY_SIZE(tempRaisedConns) &&
                        beaconGetPassableHeight(source, target, &myMax, &myMin))
                {
                    tempRaisedConns[raisedCount].minHeight = posY(source) + myMin;
                    tempRaisedConns[raisedCount].maxHeight = posY(source) + myMax;

                    raisedCount++;

                    myMin = posY(source) + myMax;
                    myMax = highMax;
                }
            }
        }
                
        conn->minHeight = FLT_MAX;
        conn->maxHeight = -FLT_MAX;
        
        if(raisedCount){
            int buffer_size = raisedCount * sizeof(*tempRaisedConns);
            
            conn->raisedConns = beaconAllocateMemory(buffer_size);
            
            conn->raisedCount = raisedCount;

            memcpy(conn->raisedConns, tempRaisedConns, buffer_size);
        
            for(i = 0; i < raisedCount; i++){
                conn->minHeight = min(conn->minHeight, tempRaisedConns[i].minHeight);
                conn->maxHeight = max(conn->maxHeight, tempRaisedConns[i].maxHeight);
            }
        }else{
            conn->raisedConns = NULL;
            conn->raisedCount = 0;
        }
    PERFINFO_AUTO_STOP();
}

static void beaconMakeBeaconLegal(Beacon* beacon){
    U32 legalBeaconIndex = beacon->userInt;
    BeaconProcessInfo* info = beacon_process.infoArray + legalBeaconIndex;
    
    if(info->isLegal){
        return;
    }
    
    info->isLegal = 1;
    
    assert(beacon_process.legalCount < combatBeaconArray.size);
    
    if(beacon_process.processOrder){
        if(info->diskSwapBlock == beacon_process.curDiskSwapBlock){
            // Put anything that's in the current BeaconDiskSwapBlock at the front of the process array.
        
            beacon_process.processOrder[beacon_process.legalCount++] = beacon_process.processOrder[beacon_process.nextDiskSwapBlockIndex];
            beacon_process.processOrder[beacon_process.nextDiskSwapBlockIndex++] = legalBeaconIndex;
            
            assert(beacon_process.nextDiskSwapBlockIndex <= beacon_process.legalCount);
        }else{
            beacon_process.processOrder[beacon_process.legalCount++] = legalBeaconIndex;
        }
    }
}

static U32 totalConnectionCount;
static U32 checkedConnectionCount;

//static BeaconProcessConnection* beaconConnectToBeacons(Beacon* source, Beacon* target, S32 targetIndex, F32* minHeight, F32* maxHeight){
//    F32 minHeightLocal;
//    F32 maxHeightLocal;
//    BeaconProcessInfo* info = beacon_process.infoArray + source->userInt;
//    BeaconProcessConnection* nearbyBeacons;
//    BeaconProcessConnection* targetInfo;
//    S32 beaconCount;
//    
//    if(!minHeight)
//    {
//        minHeightLocal = FLT_MAX;
//        minHeight = &minHeightLocal;
//    }
//        
//    if(!maxHeight)
//    {
//        maxHeightLocal = -FLT_MAX;
//        maxHeight = &maxHeightLocal;
//    }
//        
//    //printf("\n0x%8.8x, 0x%8.8x\t- %f\n", source, target, distance3(source->mat[3], target->mat[3]));
//
//    // Build the list of nearby beacons.
//    
//    if(info->beaconCount <= 0){
//        // This beacon has been processed and it had no connections.
//        return NULL;
//    }
//    
//    nearbyBeacons = info->beacons;
//    beaconCount = info->beaconCount;
//
//    // Lookup target beacon.
//    
//    if(targetIndex < 0){
//        BeaconProcessConnection* beaconInfo;
//
//        // Binary search for the beacon.
//        
//        int lo = 0;
//        int hi = beaconCount - 1;
//        int mid = 0;
//        U8 distXZ = floor(distance3XZ(posPoint(source), posPoint(target)) + 0.5);
//        int targetBeaconIndex = target->userInt;
//        
//        PERFINFO_AUTO_START("binSearch", 1);
//        
//            while(hi > lo){
//                int beaconIndex;
//                
//                mid = (lo + hi + 1) / 2;
//                
//                beaconInfo = nearbyBeacons + mid;
//                
//                beaconIndex = beaconInfo->targetIndex;
//                
//                if(distXZ < beaconInfo->distanceXZ)
//                    hi = hi > mid ? mid : mid - 1;
//                else if(distXZ > beaconInfo->distanceXZ)
//                    lo = lo < mid ? mid : mid + 1;
//                else if(targetBeaconIndex < beaconIndex)
//                    hi = hi > mid ? mid : mid - 1;
//                else if(targetBeaconIndex > beaconIndex)
//                    lo = lo < mid ? mid : mid + 1;
//                else
//                    break;
//            }
//
//        PERFINFO_AUTO_STOP();
//
//        mid = (hi + lo + 1) / 2;
//        
//        if(nearbyBeacons[mid].targetIndex == target->userInt){
//            targetIndex = mid;
//        }else{
//            return NULL;
//        }
//    }
//        
//    assert(targetIndex >= 0 && targetIndex < beaconCount);
//
//    targetInfo = nearbyBeacons + targetIndex;
//    
//    assert(targetInfo->targetIndex == target->userInt);
//    
//    if(targetInfo->hasBeenChecked){
//        if(targetInfo->minHeight < *minHeight)
//            *minHeight = targetInfo->minHeight;
//
//        if(targetInfo->maxHeight > *maxHeight)
//            *maxHeight = targetInfo->maxHeight;
//    }else{
//        int i;
//        int reachedByRaised = 0;
//        int hasIndirectGroundConnection = 0;
//        int hasBlockedIndirectConnection = 0;
//        
//        // For all the beacons j before this one, check if source connects j, and j to i.
//        
//        targetInfo->hasBeenChecked = 1;
//        targetInfo->minHeight = FLT_MAX;
//        targetInfo->maxHeight = -FLT_MAX;
//        
//        checkedConnectionCount++;
//
//        for(i = 0; i < targetIndex; i++){
//            // Check all the closer beacons within 10 degrees.
//            
//            BeaconProcessConnection* midInfo = nearbyBeacons + i;
//            int angleDiff90 = midInfo->yaw90 - targetInfo->yaw90;
//            
//            // Make sure angle diff is between -90 and 90 (i.e. -180 and 180).
//
//            if(angleDiff90 < -90)
//                angleDiff90 += 180;
//            else if(angleDiff90 > 90)
//                angleDiff90 -= 180;
//            
//            if(angleDiff90 >= -5 && angleDiff90 <= 5){
//                Beacon* midTarget = combatBeaconArray.storage[midInfo->targetIndex];
//
//                // Make all intermediate connections.
//                
//                F32 myMin1 = FLT_MAX;
//                F32 myMax1 = -FLT_MAX;
//                
//                if(midInfo->hasBeenChecked){
//                    if(midInfo->minHeight < myMin1){
//                        myMin1 = targetInfo->minHeight;
//                    }
//
//                    if(midInfo->maxHeight > myMax1){
//                        myMax1 = targetInfo->maxHeight;
//                    }
//                }else{
//                    beaconConnectToBeacons(source, midTarget, i, &myMin1, &myMax1);
//                }
//                
//                if(midInfo->failedWalkCheck){
//                    hasBlockedIndirectConnection = 1;
//                }
//                
//                if(    midInfo->reachedByRaised ||
//                    !hasIndirectGroundConnection && !hasBlockedIndirectConnection && midInfo->reachedByGround)
//                {
//                    F32 myMin2 = FLT_MAX;
//                    F32 myMax2 = -FLT_MAX;
//                    BeaconProcessConnection* midToTargetInfo;
//                    
//                    midToTargetInfo = beaconConnectToBeacons(midTarget, target, -1, &myMin2, &myMax2);
//                    
//                    if(midToTargetInfo){
//                        if(midToTargetInfo->reachedBySomething){
//                            if(midInfo->reachedByRaised && midToTargetInfo->reachedByRaised){
//                                reachedByRaised = 1;
//                                
//                                *minHeight = min(*minHeight, myMin1);
//                                *minHeight = min(*minHeight, myMin2);
//                                *maxHeight = max(*maxHeight, myMax1);
//                                *maxHeight = max(*maxHeight, myMax2);
//                            }
//                            
//                            if(    !hasIndirectGroundConnection &&
//                                !hasBlockedIndirectConnection &&
//                                midInfo->reachedByGround &&
//                                midToTargetInfo->reachedByGround)
//                            {
//                                hasIndirectGroundConnection = 1;
//                            }
//                        }
//                        
//                        if(midToTargetInfo->failedWalkCheck){
//                            hasBlockedIndirectConnection = 1;
//                        }
//                    }
//                }
//            }
//        }
//        
//        // Store the raised connections.
//        
//        beaconCreateRaisedConnections(source, targetInfo, -FLT_MAX, FLT_MAX, *minHeight, *maxHeight);
//        
//        targetInfo->minHeight = *minHeight = min(*minHeight, targetInfo->minHeight);
//        targetInfo->maxHeight = *maxHeight = max(*maxHeight, targetInfo->maxHeight);
//        
//        if(reachedByRaised || targetInfo->raisedCount){
//            targetInfo->reachedByRaised = targetInfo->reachedBySomething = 1;
//        }
//
//        // Store the ground connection.
//        
//        if(hasIndirectGroundConnection){
//            // An indirect connection already exists.
//            
//            targetInfo->reachedByGround = targetInfo->reachedBySomething = 1;
//            targetInfo->makeGroundConnection = 0;
//        }
//        else if(!hasBlockedIndirectConnection){
//            int connectsByGround;
//            
//            // A connection doesn't exist yet, so figure out if target is reachable.
//            
//            usedPhysicsSteps = 0;
//            
//            if(source->noGroundConnections || target->noGroundConnections){
//                connectsByGround = 0;
//            }else{
//                PERFINFO_AUTO_START("connectsByGround", 1);
//                    connectsByGround = beaconConnectsToBeaconByGround(source, target, NULL, 9, beacon_process.entity, 0);
//                PERFINFO_AUTO_STOP();
//            }
//                        
//            if(connectsByGround){
//                targetInfo->reachedByGround = targetInfo->reachedBySomething = 1;
//                targetInfo->makeGroundConnection = 1;
//            }else{
//                targetInfo->failedWalkCheck = last_walk_result == WALKRESULT_BLOCKED;
//            }
//            
//            if(usedPhysicsSteps){
//                info->stats.physicsSteps += usedPhysicsSteps;
//                info->stats.walkedConnections++;
//                info->stats.totalDistance += distance3(posPoint(source), posPoint(target));
//            }
//        }
//
//        // Add to legal list.
//        
//        if(targetInfo->reachedBySomething){
//            if(!beacon_process.infoArray[target->userInt].isLegal){
//                // Put target into the legal beacon area of the process array.
//                
//                beaconMakeBeaconLegal(target);
//            }
//        }
//    }
//
//    return targetInfo;
//}

//static void beaconProcessCombatBeacon(Beacon* source){
//    BeaconProcessInfo* info = beacon_process.infoArray + source->userInt;
//    int i;
//    int hasConnectionCount = 0;
//    int keepConnectionCount = 0;
//    BeaconProcessConnection* nearbyBeacons;
//    int beaconCount;
//    
//    nearbyBeacons = info->beacons;
//    beaconCount = info->beaconCount;
//    
//    // For each nearby beacon, find anything closer that connects, then check if that beacon connects.
//
//    for(i = 0; i < beaconCount; i++){
//        Beacon* target = combatBeaconArray.storage[nearbyBeacons[i].targetIndex];
//        
//        PERFINFO_AUTO_START("beaconConnectToBeacons", 1);
//        
//            beaconConnectToBeacons(source, target, i, NULL, NULL);
//            
//            if(    nearbyBeacons[i].reachedBySomething ||
//                nearbyBeacons[i].failedWalkCheck)
//            {
//                keepConnectionCount++;
//            }
//
//        PERFINFO_AUTO_STOP();
//    }
//    
//    // Consolidate all connections into the exact-sized array.
//    
//    PERFINFO_AUTO_START("consolidate", 1);
//    
//        if(!keepConnectionCount){
//            SAFE_FREE(info->beacons);
//            info->beaconCount = -1;
//        }
//        else if(keepConnectionCount < beaconCount){
//            BeaconProcessConnection* newNearbyBeacons;
//            int j = 0;
//            
//            //printf(    "reducing from %d to %d: %d saved\n",
//            //        beaconCount,
//            //        canBeReachedCount,
//            //        beaconCount - canBeReachedCount);
//            
//            newNearbyBeacons = beaconAllocateMemory(keepConnectionCount * sizeof(*newNearbyBeacons));
//
//            for(i = 0; i < beaconCount; i++){
//                if(    nearbyBeacons[i].reachedBySomething ||
//                    nearbyBeacons[i].failedWalkCheck)
//                {
//                    assert(j < keepConnectionCount);
//                    newNearbyBeacons[j] = nearbyBeacons[i];
//                    j++;
//                }
//            }
//            
//            free(info->beacons);
//            
//            info->beacons = newNearbyBeacons;
//            info->beaconCount = keepConnectionCount;
//        }
//    
//    PERFINFO_AUTO_STOP();
//}

static int __cdecl compareBeaconDistanceXZ(const void* b1ParamData, const void* b2ParamData){
    const Beacon** b1Param = (const Beacon**)b1ParamData;
    const Beacon** b2Param = (const Beacon**)b2ParamData;
    const Beacon* b1 = *b1Param;
    const Beacon* b2 = *b2Param;
    
    if(b1->pyr[0] > b2->pyr[0]){
        return 1;
    }
    else if(b1->pyr[0] < b2->pyr[0]){
        return -1;
    }

    return 0;
}

static struct {
    Beacon*    source;
    Array    group[5];
} compileNearbyGroupsData;

static void beaconCompileNearbyBeaconGroupsHelper(Array* beaconArray, void* userData){
    Beacon* source = compileNearbyGroupsData.source;
    int i;
    
    for(i = 0; i < beaconArray->size; i++){
        Beacon* target = beaconArray->storage[i];
        F32 dist;
        int index;

        if(target == source)
            continue;    
            
        dist = distance3XZ(posPoint(source), posPoint(target));
        
        index = dist / 20;

        if(index >= 0 && index < ARRAY_SIZE(compileNearbyGroupsData.group)){
            arrayPushBack(&compileNearbyGroupsData.group[index], target);
            
            // Store the distance in pyr[0] because it's available.
            
            target->pyr[0] = dist;
        }
    }
}

static void beaconCompileNearbyBeaconGroups(Beacon* source){
    int i;
    
    compileNearbyGroupsData.source = source;
    
    for(i = 0; i < ARRAY_SIZE(compileNearbyGroupsData.group); i++){
        compileNearbyGroupsData.group[i].size = 0;
    }

    beaconForEachBlock(posPoint(source), 100, scene_info.maxHeight + 2000, 100, beaconCompileNearbyBeaconGroupsHelper, NULL);
}

void beaconProcessCombatBeacon2(Beacon* source){
    static struct {
        BeaconProcessConnection* buffer;
        int maxCount;
    } tempConns;
    
    F32 sourceCeilingHeight = beaconGetCeilingDistance(source);
    F32 sourceFloorHeight = beaconGetFloorDistance(source);
    const int fill_angle_delta = 7;
    BeaconProcessInfo* info = beacon_process.infoArray + source->userInt;
    BeaconProcessInfo tempInfo;
    int tempConnsCount = 0;
    int i;
    
    if(!info){
        info = &tempInfo;
        ZeroStruct(info);
    }
    
    if(1){
        Vec3 pos = {116, 0, -313};
        //Vec3 pos = {117, 0, -294};
        
        if(distance3XZ(pos, posPoint(source)) <= 1.0f){
            printf("");
        }
    }
    
    if(!info->angleInfo){
        info->angleInfo = beaconAllocateMemory(sizeof(*info->angleInfo));
        
        for(i = 0; i < ARRAY_SIZE(info->angleInfo->angle); i++){
            copyVec3(posPoint(source), info->angleInfo->angle[i].posReached);
            vecY(info->angleInfo->angle[i].posReached) -= sourceFloorHeight - 0.1;
        }
    }
    
    beaconCompileNearbyBeaconGroups(source);
    
    // Keep finding stuff until empty.
    
    while(    info->angleInfo->curGroup < ARRAY_SIZE(compileNearbyGroupsData.group) &&
            info->angleInfo->completedCount < ARRAY_SIZE(info->angleInfo->angle))
    {
        Array* beaconArray = &compileNearbyGroupsData.group[info->angleInfo->curGroup++];
        int checkedCount = 0;
        int validCount = 0;
        
        for(i = 0; i < beaconArray->size; i++){
            BeaconProcessAngleInfo* angleInfo;
            Beacon* target = beaconArray->storage[i];
            Vec3 diff;
            int yaw90;

            subVec3(posPoint(target), posPoint(source), diff);
            
            yaw90 = 90 + floor(DEG(getVec3Yaw(diff)) / 2 + 0.5);
            yaw90 = (yaw90 + 180) % 180;
            
            assert(yaw90 >= 0 && yaw90 < 180);
            
            angleInfo = info->angleInfo->angle + yaw90;
            
            if(!angleInfo->done){
                beaconArray->storage[validCount++] = target;
                target->pyr[1] = yaw90;
            }
        }
        
        if(!validCount)
            continue;
        
        beaconArray->size = validCount;
        
        qsort(beaconArray->storage, beaconArray->size, sizeof(beaconArray->storage[0]), compareBeaconDistanceXZ);

        // For each nearby beacon, find anything closer that connects, then check if that beacon connects.

        for(i = 0; i < beaconArray->size; i++){
            BeaconProcessAngleInfo* angleInfo;
            Beacon* target = beaconArray->storage[i];
            BeaconProcessConnection conn;
            int yaw90 = target->pyr[1];
            F32 targetCeilingHeight = beaconGetCeilingDistance(target);
            F32 minCeilingHeight = min(sourceCeilingHeight, targetCeilingHeight);

            if(0){
                Vec3 testpos = {-331, 66, 86.86};
                Vec3 testpos2 = {-330.52, 66.00, 72};
                F32 threshold = 1;
                if(    distance3(testpos, posPoint(source)) < threshold &&
                    distance3(testpos2, posPoint(target)) < threshold)
                {
                    assert(0);
                    printf("");
                }
            }
            
            conn.targetIndex = target->userInt;
            conn.makeGroundConnection = 0;
            conn.raisedCount = 0;

            angleInfo = info->angleInfo->angle + yaw90;
            
            if(angleInfo->done)
                continue;
                
            checkedCount++;
            
            if(!angleInfo->handledForGround){
                int connectsByGround;
                
                // A connection doesn't exist yet, so figure out if target is reachable.
                
                usedPhysicsSteps = 0;
                
                if(source->noGroundConnections){
                    entUpdatePosInterpolated(beacon_process.entity, posPoint(target));

                    if(target->noGroundConnections && fabs(posY(source) - posY(target)) < 1){
                        connectsByGround = 1;
                        last_walk_result = WALKRESULT_SUCCESS;
                    }else{
                        connectsByGround = 0;
                        last_walk_result = WALKRESULT_NO_LOS;
                    }
                }
                else if(target->noGroundConnections){
                    entUpdatePosInterpolated(beacon_process.entity, angleInfo->posReached);
                    connectsByGround = 0;
                    last_walk_result = WALKRESULT_NO_LOS;
                }
                else{
                    PERFINFO_AUTO_START("connectsByGround", 1);
                        connectsByGround = beaconConnectsToBeaconByGround(    source,
                                                                            target,
                                                                            angleInfo->posReached,
                                                                            min(minCeilingHeight, 9),
                                                                            beacon_process.entity,
                                                                            0);
                    PERFINFO_AUTO_STOP();
                }
                                
                copyVec3(ENTPOS(beacon_process.entity), angleInfo->posReached);
                angleInfo->posReachedDistXZ = distance3SquaredXZ(posPoint(source), angleInfo->posReached);
                
                if(    connectsByGround ||
                    last_walk_result == WALKRESULT_BLOCKED_LONG)
                {
                    int cur_delta = connectsByGround ? fill_angle_delta : 1;
                    int offset;
                    
                    if(connectsByGround){
                        conn.makeGroundConnection = 1;
                        beaconMakeBeaconLegal(target);
                    }

                    for(offset = -cur_delta; offset <= cur_delta; offset++){
                        int angle90 = (yaw90 + offset + ANGLE_INCREMENT_COUNT) % ANGLE_INCREMENT_COUNT;
                        BeaconProcessAngleInfo* offsetInfo = info->angleInfo->angle + angle90;
                        
                        if(angleInfo->posReachedDistXZ > offsetInfo->posReachedDistXZ){
                            offsetInfo->posReachedDistXZ = angleInfo->posReachedDistXZ;
                            copyVec3(angleInfo->posReached, offsetInfo->posReached);
                        }

                        if(!offsetInfo->handledForGround){
                            offsetInfo->handledForGround = 1;
                            
                            if(offsetInfo->handledForRaised){
                                // This angle is done.
                                
                                offsetInfo->done = 1;
                                info->angleInfo->completedCount++;
                            }
                        }
                    }
                }
                
                if(usedPhysicsSteps){
                    info->stats.physicsSteps += usedPhysicsSteps;
                    info->stats.walkedConnections++;
                    info->stats.totalDistance += distance3(posPoint(source), posPoint(target));
                }
            }
            
            if(!angleInfo->handledForRaised){
                int offset;
                int fill_angles = 0;
                
                if(angleInfo->ignoreMin == 0 && angleInfo->ignoreMax == 0){
                    angleInfo->ignoreMin = FLT_MAX;
                    angleInfo->ignoreMax = -FLT_MAX;
                    angleInfo->openMin = posY(source) + 0.1;
                    angleInfo->openMax = posY(source) + sourceCeilingHeight;
                }
                
                beaconCreateRaisedConnections(    source,
                                                &conn,
                                                angleInfo->openMin,
                                                angleInfo->openMax,
                                                angleInfo->ignoreMin,
                                                angleInfo->ignoreMax);
                
                if(conn.raisedCount){
                    beaconMakeBeaconLegal(target);

                    fill_angles = 1;
                    
                    if(conn.minHeight < angleInfo->ignoreMin)
                        angleInfo->ignoreMin = conn.minHeight;
                    if(conn.maxHeight > angleInfo->ignoreMax)
                        angleInfo->ignoreMax = conn.maxHeight;
                }
                
                if(posY(target) + targetCeilingHeight > angleInfo->openMax - 3){
                    if(posY(target) < angleInfo->openMax)
                        angleInfo->openMax = posY(target);
                }
                
                if(posY(target) < angleInfo->openMin + 3){
                    if(posY(target) + targetCeilingHeight > angleInfo->openMin){
                        angleInfo->openMin = posY(target) + targetCeilingHeight;
                    }
                }
                
                if(    angleInfo->ignoreMin < posY(source) + 6 &&
                    angleInfo->ignoreMax > posY(source) + sourceCeilingHeight - 6)
                {
                    if(!angleInfo->handledForRaised){
                        angleInfo->handledForRaised = 1;
                        
                        if(angleInfo->handledForGround){
                            // This angle is done.

                            angleInfo->done = 1;
                            info->angleInfo->completedCount++;
                        }
                    }
                }

                if(fill_angles){
                    for(offset = -fill_angle_delta; offset <= fill_angle_delta; offset++){
                        int angle90 = (yaw90 + offset + ANGLE_INCREMENT_COUNT) % ANGLE_INCREMENT_COUNT;
                        BeaconProcessAngleInfo* offsetInfo = info->angleInfo->angle + angle90;
                        
                        if(offsetInfo->ignoreMin == 0 && offsetInfo->ignoreMax == 0){
                            offsetInfo->ignoreMin = FLT_MAX;
                            offsetInfo->ignoreMax = -FLT_MAX;
                            offsetInfo->openMin = -FLT_MAX;
                            offsetInfo->openMax = FLT_MAX;
                        }

                        if(angleInfo->ignoreMin < offsetInfo->ignoreMin)
                            offsetInfo->ignoreMin = angleInfo->ignoreMin;
                        if(angleInfo->ignoreMax > offsetInfo->ignoreMax)
                            offsetInfo->ignoreMax = angleInfo->ignoreMax;
                            
                        if(angleInfo->openMin > offsetInfo->openMin)
                            offsetInfo->openMin = angleInfo->openMin;
                        if(angleInfo->openMax < offsetInfo->openMax)
                            offsetInfo->openMax = angleInfo->openMax;
                        
                        // Finish this angle if the center angle is finished.
                        
                        if(angleInfo->handledForRaised && !offsetInfo->handledForRaised){
                            offsetInfo->handledForRaised = 1;
                            
                            if(offsetInfo->handledForGround){
                                // This angle is done.

                                offsetInfo->done = 1;
                                info->angleInfo->completedCount++;
                            }
                        }
                    }
                }
            }
            
            if(conn.makeGroundConnection || conn.raisedCount){
                BeaconProcessConnection* newConn;
                
                newConn = dynArrayAddStruct(&tempConns.buffer,
                                            &tempConnsCount,
                                            &tempConns.maxCount);
                                        
                newConn->targetIndex = conn.targetIndex;
                newConn->reachedByGround = conn.makeGroundConnection;
                newConn->reachedByRaised = conn.raisedCount ? 1 : 0;
                newConn->reachedBySomething = newConn->reachedByGround || newConn->reachedByRaised;
                newConn->makeGroundConnection = conn.makeGroundConnection;
                newConn->raisedCount = conn.raisedCount;
                newConn->raisedConns = conn.raisedConns;

                assert(    tempConnsCount < 2 ||
                        tempConns.buffer[tempConnsCount-1].targetIndex != tempConns.buffer[tempConnsCount-2].targetIndex);
            }
        }
        
        //printf("[%d/%d]", checkedCount, info->angleInfo->completedCount);
    }
    
    SAFE_FREE(info->angleInfo);
    
    // Consolidate all connections into the exact-sized array.
    
    PERFINFO_AUTO_START("consolidate", 1);
    
        if(tempConnsCount){
            BeaconProcessConnection* newConns;
            int bufferSize = tempConnsCount * sizeof(*newConns);
            int j = 0;
            
            newConns = beaconAllocateMemory(bufferSize);

            memcpy(newConns, tempConns.buffer, bufferSize);

            SAFE_FREE(info->beacons);
            
            info->beacons = newConns;
            info->beaconCount = tempConnsCount;
        }
    
    PERFINFO_AUTO_STOP();
}

static void beaconForwardPackDiskSwapBlock(BeaconDiskSwapBlock* swapBlock, int start){
    int i;
    int j;
                
    beacon_process.curDiskSwapBlock = swapBlock;
    
    assert(beacon_process.curDiskSwapBlock);
    
    for(i = start, j = start; i < beacon_process.legalCount; i++){
        int index = beacon_process.processOrder[i];
        BeaconProcessInfo* info = beacon_process.infoArray + index;
        
        if(info->diskSwapBlock == swapBlock){
            beacon_process.processOrder[i] = beacon_process.processOrder[j];
            beacon_process.processOrder[j] = index;
            j++;
        }
    }
    
    beacon_process.nextDiskSwapBlockIndex = j;
}

static int blockHasGroundConnectionToBlock(BeaconBlock* source, BeaconBlock* target){
    if(findBlockConnection(&source->gbbConns, target, NULL)){
        return 1;
    }
    
    return 0;
}

static int blockHasRaisedConnectionToBlock(BeaconBlock* source, BeaconBlock* target, F32 maxJumpHeight){
    BeaconBlockConnection* conn = findBlockConnection(&source->rbbConns, target, NULL);
    
    return conn && conn->minJumpHeight <= maxJumpHeight;
}

static int staticPropagateGalaxyCount;
static int staticSubBlockCount = 0;
static Array tempGalaxySubBlocks;

static void beaconPropagateGalaxy(BeaconBlock* subBlock, BeaconBlock* galaxy, int quiet){
    int i;
    
    subBlock->galaxy = galaxy;
    
    arrayPushBack(&tempGalaxySubBlocks, subBlock);
    
    if(!quiet){
        printf(".");
    }
    
    if(!quiet){
        beaconProcessSetTitle(100.0 * staticPropagateGalaxyCount++ / staticSubBlockCount, NULL);
    }
    
    for(i = 0; i < subBlock->gbbConns.size; i++){
        BeaconBlockConnection* conn = subBlock->gbbConns.storage[i];
        BeaconBlock* destBlock = conn->destBlock;
        
        if(!destBlock->galaxy && blockHasGroundConnectionToBlock(destBlock, subBlock)){
            beaconPropagateGalaxy(destBlock, galaxy, quiet);
        }
    }
}

static int getConnectedGalaxyCount(BeaconBlock* galaxy){
    int i;
    int count = 1;
    
    galaxy->searchInstance = 1;
    
    for(i = 0; i < galaxy->gbbConns.size; i++){
        BeaconBlockConnection* conn = galaxy->gbbConns.storage[i];
        
        if(!conn->destBlock->searchInstance){
            count += getConnectedGalaxyCount(conn->destBlock);
        }
    }

    //for(i = 0; i < galaxy->rbbConns.size; i++){
    //    BeaconBlockConnection* conn = galaxy->rbbConns.storage[i];
    //    
    //    if(!conn->destBlock->searchInstance){
    //        count += getConnectedGalaxyCount(conn->destBlock);
    //    }
    //}
    
    return count;
}

static Array tempGalaxyArray;

static void beaconPropagateGalaxies(int quiet){
    int i;
    
    if(!quiet){
        beaconProcessSetTitle(0, "Galaxize");
    }
    
    tempGalaxyArray.size = 0;

    for(i = 0; i < combatBeaconGridBlockArray.size; i++){
        BeaconBlock* gridBlock = combatBeaconGridBlockArray.storage[i];
        int j;
        
        for(j = 0; j < gridBlock->subBlockArray.size; j++){
            BeaconBlock* subBlock = gridBlock->subBlockArray.storage[j];

            if(!subBlock->galaxy){
                BeaconBlock* galaxy = createBeaconBlock();
                
                galaxy->isGalaxy = 1;
                
                tempGalaxySubBlocks.size = 0;
                
                beaconPropagateGalaxy(subBlock, galaxy, quiet);
                
                beaconBlockInitCopyArray(&galaxy->subBlockArray, &tempGalaxySubBlocks);

                arrayPushBack(&tempGalaxyArray, galaxy);
            }
        }
    }
    
    beaconBlockInitCopyArray(&combatBeaconGalaxyArray[0], &tempGalaxyArray);

    if(!quiet){
        beaconProcessSetTitle(100, NULL);
    }
}

static void beaconMakeGalaxyConnections(int galaxySet, int quiet){
    int i, j, k;
    
    if(!quiet){
        beaconProcessSetTitle(0, "GalaxyConn");
    }
    
    for(i = 0; i < combatBeaconGalaxyArray[galaxySet].size; i++){
        BeaconBlock* galaxy = combatBeaconGalaxyArray[galaxySet].storage[i];
        
        if(!quiet){
            beaconProcessSetTitle(100.0 * i / combatBeaconGalaxyArray[galaxySet].size, NULL);
        }

        tempGroundConns.size = tempRaisedConns.size = 0;

        for(j = 0; j < galaxy->subBlockArray.size; j++){
            BeaconBlock*            subBlock = galaxy->subBlockArray.storage[j];
            BeaconBlockConnection*    destConn;
            BeaconBlock*            destGalaxy;
            BeaconBlockConnection*    galaxyConn;
            int                        newIndex;
            
            // Add the ground connections.
            
            for(k = 0; k < subBlock->gbbConns.size; k++){
                destConn = subBlock->gbbConns.storage[k];
                destGalaxy = destConn->destBlock->galaxy;
                
                assert(destGalaxy->isGalaxy);
                
                if(destGalaxy == galaxy)
                    continue;
                    
                // Look for an existing connection.
                
                galaxyConn = findBlockConnection(&tempGroundConns, destGalaxy, &newIndex);
                
                if(!galaxyConn){
                    // New connection.
                    
                    assert(newIndex >= 0 && newIndex <= tempGroundConns.size);
                
                    galaxyConn = createBeaconBlockConnection();
                    galaxyConn->destBlock = destGalaxy;
                    
                    arrayPushBack(&tempGroundConns, NULL);

                    if(newIndex != tempGroundConns.size - 1){
                        // Nudge everything after the new element forward.
                        
                        int elementsToMove = tempGroundConns.size - newIndex - 1;
                        
                        CopyStructsFromOffset(    tempGroundConns.storage + newIndex + 1,
                                                -1,
                                                elementsToMove);
                    }

                    tempGroundConns.storage[newIndex] = galaxyConn;
                }
            }

            // Add the raised connections.
            
            for(k = 0; k < subBlock->rbbConns.size; k++){
                destConn = subBlock->rbbConns.storage[k];
                destGalaxy = destConn->destBlock->galaxy;

                assert(destGalaxy->isGalaxy);
                
                if(destGalaxy == galaxy)
                    continue;
                
                // Look for an existing connection.

                galaxyConn = findBlockConnection(&tempRaisedConns, destGalaxy, &newIndex);

                if(galaxyConn){
                    galaxyConn->minHeight        = min(destConn->minHeight,        galaxyConn->minHeight);
                    galaxyConn->maxHeight        = max(destConn->maxHeight,        galaxyConn->maxHeight);
                    galaxyConn->minJumpHeight    = min(destConn->minJumpHeight,    galaxyConn->minJumpHeight);
                }else{
                    // Make a new connection.
                    
                    assert(newIndex >= 0 && newIndex <= tempRaisedConns.size);

                    galaxyConn = createBeaconBlockConnection();
                    
                    galaxyConn->destBlock        = destGalaxy;
                    galaxyConn->minHeight        = destConn->minHeight;
                    galaxyConn->maxHeight        = destConn->maxHeight;
                    galaxyConn->minJumpHeight    = destConn->minJumpHeight;
                    
                    arrayPushBack(&tempRaisedConns, NULL);
                    
                    if(newIndex != tempRaisedConns.size - 1){
                        // Nudge everything after the new element forward.
                        
                        int elementsToMove = tempRaisedConns.size - newIndex - 1;
                        
                        CopyStructsFromOffset(    tempRaisedConns.storage + newIndex + 1,
                                                -1,
                                                elementsToMove);
                    }

                    tempRaisedConns.storage[newIndex] = galaxyConn;
                }
            }
        }
        
        beaconBlockInitCopyArray(&galaxy->gbbConns, &tempGroundConns);
        beaconBlockInitCopyArray(&galaxy->rbbConns, &tempRaisedConns);
    }
    
    // Get the reverse connection heights.

    for(i = 0; i < combatBeaconGalaxyArray[galaxySet].size; i++){
        BeaconBlock* galaxy = combatBeaconGalaxyArray[galaxySet].storage[i];
        
        
    }

    if(!quiet){
        beaconProcessSetTitle(100, NULL);
    }
}

static void beaconCreateGalaxies(int quiet){
    int i, j;
    int groundConnCount = 0;
    int raisedConnCount = 0;

    // Count the subBlocks and clear galaxies.
    
    staticSubBlockCount = 0;
    
    for(i = 0; i < combatBeaconGridBlockArray.size; i++){
        BeaconBlock* gridBlock = combatBeaconGridBlockArray.storage[i];
        
        staticSubBlockCount += gridBlock->subBlockArray.size;

        for(j = 0; j < gridBlock->subBlockArray.size; j++){
            BeaconBlock* subBlock = gridBlock->subBlockArray.storage[j];

            subBlock->galaxy = NULL;
        }
    }
    
    // Propagate the galaxies.
    
    PERFINFO_AUTO_START("propagate", 1);
        beaconPropagateGalaxies(quiet);
    PERFINFO_AUTO_STOP();

    // Create galaxy connections.

    PERFINFO_AUTO_START("connect", 1);
        beaconMakeGalaxyConnections(0, quiet);
    PERFINFO_AUTO_STOP();
    
    // Print some stats.

    if(!quiet){
        printf(    "DONE!\n\n"
                "  Galaxies:     %d\n"
                "  Blocks:       %d\n"
                "  Ground conns: %d\n"
                "  Raised conns: %d\n\n",
                combatBeaconGalaxyArray[0].size,
                staticSubBlockCount,
                groundConnCount,
                raisedConnCount);
    }
    
    if(0){
        for(i = 0; i < combatBeaconGalaxyArray[0].size; i++){
            BeaconBlock* galaxy = combatBeaconGalaxyArray[0].storage[i];
            int connCount;
            BeaconBlock* subBlock = galaxy->subBlockArray.storage[0];
            Beacon* beacon = subBlock->beaconArray.storage[0];

            for(j = 0; j < combatBeaconGalaxyArray[0].size; j++){
                BeaconBlock* galaxy = combatBeaconGalaxyArray[0].storage[j];
                
                galaxy->searchInstance = 0;
            }
            
            connCount = getConnectedGalaxyCount(galaxy);
            
            printf(    "%4d:\t%d+%d\t(%1.1f,\t%1.1f,\t%1.1f)\t%d\n",
                    i,
                    galaxy->gbbConns.size,
                    galaxy->rbbConns.size,
                    posParamsXYZ(beacon),
                    connCount);
        }    
    }
}

static void beaconPropagateGalaxyGroup(BeaconBlock* childGalaxy, BeaconBlock* parentGalaxy, F32 maxJumpHeight){
    BeaconBlockConnection* conn;
    BeaconBlock* destGalaxy;
    int i;
    
    childGalaxy->galaxy = parentGalaxy;
    
    arrayPushBack(&tempGalaxySubBlocks, childGalaxy);
    
    for(i = 0; i < childGalaxy->gbbConns.size; i++){
        conn = childGalaxy->gbbConns.storage[i];
        destGalaxy = conn->destBlock;
        
        if(!destGalaxy->galaxy && blockHasGroundConnectionToBlock(destGalaxy, childGalaxy)){
            beaconPropagateGalaxyGroup(destGalaxy, parentGalaxy, maxJumpHeight);
        }
    }

    for(i = 0; i < childGalaxy->rbbConns.size; i++){
        conn = childGalaxy->rbbConns.storage[i];
        destGalaxy = conn->destBlock;
        
        if(    !destGalaxy->galaxy &&
            conn->minJumpHeight <= maxJumpHeight &&
            blockHasRaisedConnectionToBlock(destGalaxy, childGalaxy, maxJumpHeight))
        {
            beaconPropagateGalaxyGroup(destGalaxy, parentGalaxy, maxJumpHeight);
        }
    }
}

static void beaconCreateGalaxyGroups(int quiet){
    int i;
    
    for(i = 0; i < combatBeaconGalaxyArray[0].size; i++){
        BeaconBlock* galaxy = combatBeaconGalaxyArray[0].storage[i];

        galaxy->galaxy = NULL;
    }
    
    for(i = 1; i < ARRAY_SIZE(combatBeaconGalaxyArray); i++){
        int j;
        int childSet = i - 1;
        int childSetSize = combatBeaconGalaxyArray[childSet].size;
        
        PERFINFO_AUTO_START("propagate", 1);
            tempGalaxyArray.size = 0;

            for(j = 0; j < childSetSize; j++){
                BeaconBlock* childGalaxy = combatBeaconGalaxyArray[childSet].storage[j];
                
                if(!childGalaxy->galaxy){
                    BeaconBlock* parentGalaxy = createBeaconBlock();
                    
                    arrayPushBack(&tempGalaxyArray, parentGalaxy);

                    parentGalaxy->isGalaxy = 1;
                    
                    tempGalaxySubBlocks.size = 0;
                    
                    beaconPropagateGalaxyGroup(childGalaxy, parentGalaxy, i * BEACON_GALAXY_GROUP_JUMP_INCREMENT);
                    
                    beaconBlockInitCopyArray(&parentGalaxy->subBlockArray, &tempGalaxySubBlocks);
                    
                    assert(childGalaxy->galaxy == parentGalaxy);
                }
            }
            
            beaconBlockInitCopyArray(&combatBeaconGalaxyArray[i], &tempGalaxyArray);
        PERFINFO_AUTO_STOP();
        
        if(!quiet){
            printf("(%d,", combatBeaconGalaxyArray[i].size);
        }
        
        PERFINFO_AUTO_START("connect", 1);
            beaconMakeGalaxyConnections(i, 1);
        PERFINFO_AUTO_STOP();

        if(!quiet){
            printf("%d)", combatBeaconGalaxyArray[i].size);
        }
    }
}

static void beaconSplitBlocks(int quiet){
    int i, j;
    
    if(!quiet){
        beaconProcessSetTitle(0, "Split");
    }

    for(i = 0; i < combatBeaconGridBlockArray.size; i++){
        BeaconBlock* block = combatBeaconGridBlockArray.storage[i];
        
        // Destroy the the sub-blocks in this 

        clearArrayEx(&block->subBlockArray, destroyBeaconSubBlock);

        assert(!block->subBlockArray.size);

        beaconSplitBlock(block);
        
        if(!quiet){
            printf("%s%d", i ? "," : "", block->subBlockArray.size);
            
            beaconProcessSetTitle(100.0f * i / combatBeaconGridBlockArray.size, NULL);
        }
        
        for(j = 0; j < block->subBlockArray.size; j++){
            BeaconBlock* subBlock = block->subBlockArray.storage[j];

            subBlock->madeConnections = 1;
        }
    }

    if(!quiet){
        beaconProcessSetTitle(100, NULL);
    }
}

void beaconSplitBlocksAndGalaxies(int quiet){
    int i;
    
    if(!quiet){
        printf("Splitting Blocks: ");
    }
    
    PERFINFO_AUTO_START("split", 1);
        beaconSplitBlocks(quiet);
    PERFINFO_AUTO_STOP();
    
    //{
    //    // Temp:
    //    int i;
    //    for(i = 0; i < combatBeaconArray.size; i++){
    //        Beacon* b = combatBeaconArray.storage[i];
    //        
    //        assert(b->block && !b->block->isGridBlock);
    //    }
    //}
        
    if(!quiet){
        printf("\nConnecting Blocks: ");
    }

    PERFINFO_AUTO_START("connect", 1);
        for(i = 0; i < combatBeaconGridBlockArray.size; i++){
            BeaconBlock* gridBlock = combatBeaconGridBlockArray.storage[i];

            if(!quiet){
                printf(".");
            }

            beaconMakeBlockConnections(gridBlock);
        }
    PERFINFO_AUTO_STOP();

    if(!quiet){
        printf("\nCreating Galaxies: ");
    }
    
        //for(i = 0; i < combatBeaconArray.size; i++){
        //    Beacon* b = combatBeaconArray.storage[i];
        //    
        //    assert(b->block && !b->block->isGridBlock && b->block->parentBlock->isGridBlock && !b->block->galaxy);
        //}

    PERFINFO_AUTO_START("galaxize", 1);
        beaconCreateGalaxies(quiet);
    PERFINFO_AUTO_STOP();
    
    //{
    //    // Temp:
    //    int i;
    //    
    //    for(i = 0; i < combatBeaconGridBlockArray.size; i++){
    //        BeaconBlock* gridBlock = combatBeaconGridBlockArray.storage[i];
    //        int j;
    //        
    //        for(j = 0; j < gridBlock->subBlockArray.size; j++){
    //            BeaconBlock* subBlock = gridBlock->subBlockArray.storage[j];
    //            int k;
    //            
    //            assert(subBlock->galaxy);
    //            
    //            for(k = 0; k < subBlock->beaconArray.size; k++){
    //                Beacon* b = subBlock->beaconArray.storage[k];
    //                
    //                assert(b->block == subBlock);
    //            }
    //        }
    //    }
    //    
    //    
    //    for(i = 0; i < combatBeaconArray.size; i++){
    //        Beacon* b = combatBeaconArray.storage[i];
    //        BeaconBlock* subBlock = b->block;
    //        int j;
    //        
    //        assert(subBlock->parentBlock && subBlock->parentBlock->isGridBlock);
    //        
    //        for(j = 0; j < combatBeaconGridBlockArray.size; j++){
    //            if(combatBeaconGridBlockArray.storage[j] == subBlock->parentBlock)
    //                break;
    //        }
    //        
    //        assert(j < combatBeaconGridBlockArray.size);
    //        
    //        for(j = 0; j < subBlock->parentBlock->subBlockArray.size; j++){
    //            if(subBlock->parentBlock->subBlockArray.storage[j] == subBlock)
    //                break;
    //        }
    //        
    //        assert(subBlock->parentBlock->subBlockArray.size);

    //        for(j = 0; j < subBlock->beaconArray.size; j++){
    //            if(subBlock->beaconArray.storage[j] == b)
    //                break;
    //        }
    //        
    //        assert(j < subBlock->beaconArray.size);
    //        
    //        assert(b->block && !b->block->isGridBlock && b->block->galaxy);
    //        
    //        
    //    }
    //}

    if(!quiet){
        printf("\nDONE!\n\n");
    }
}

static int galaxyHasConnectionToGalaxy(BeaconBlock* source, BeaconBlock* target){
    if(    findBlockConnection(&source->gbbConns, target, NULL) ||
        findBlockConnection(&source->rbbConns, target, NULL))
    {
        return 1;
    }
    
    return 0;
}

static int staticPropagateClusterCount;

static void beaconPropagateCluster(BeaconBlock* galaxy, BeaconBlock* cluster, int quiet){
    int i;
    
    if(!quiet){
        beaconProcessSetTitle(100.0f * staticPropagateClusterCount++ / combatBeaconGalaxyArray[0].size, NULL);
    }
    
    galaxy->cluster = cluster;
    
    arrayPushBack(&tempGalaxyArray, galaxy);
    
    for(i = 0; i < galaxy->gbbConns.size; i++){
        BeaconBlockConnection* conn = galaxy->gbbConns.storage[i];
        BeaconBlock* destGalaxy = conn->destBlock;
        
        if(!destGalaxy->cluster && galaxyHasConnectionToGalaxy(destGalaxy, galaxy)){
            beaconPropagateCluster(destGalaxy, cluster, quiet);
        }
    }

    for(i = 0; i < galaxy->rbbConns.size; i++){
        BeaconBlockConnection* conn = galaxy->rbbConns.storage[i];
        BeaconBlock* destGalaxy = conn->destBlock;
        
        if(!destGalaxy->cluster && galaxyHasConnectionToGalaxy(destGalaxy, galaxy)){
            beaconPropagateCluster(destGalaxy, cluster, quiet);
        }
    }
}

static int __cdecl compareBeaconClusterSize(const void* b1Data, const void* b2Data){
    const BeaconBlock** b1 = (const BeaconBlock**)b1Data;
    const BeaconBlock** b2 = (const BeaconBlock**)b2Data;
    int size1 = (*b1)->subBlockArray.size;
    int size2 = (*b2)->subBlockArray.size;

    if(size1 < size2){
        return 1;
    }
    else if(size1 == size2)
        return 0;
    else
        return -1;
}

typedef struct ReverseBlockConnection {
    struct ReverseBlockConnection* next;
    BeaconBlock* source;
} ReverseBlockConnection;

MP_DEFINE(ReverseBlockConnection);

ReverseBlockConnection* createReverseBlockConnection(void){
    MP_CREATE(ReverseBlockConnection, 10000);
    
    return MP_ALLOC(ReverseBlockConnection);
}

void destroyReverseBlockConnection(ReverseBlockConnection* conn){
    MP_FREE(ReverseBlockConnection, conn);
}

static StashTable reverseConnTable;

static void createReverseConnectionTable(void){
    if(reverseConnTable){
        return;
    }
    
    reverseConnTable = stashTableCreateAddress(100);
}

static ReverseBlockConnection* getFirstReverseConnection(BeaconBlock* block){
    ReverseBlockConnection* pResult;

    if(!reverseConnTable){
        return NULL;
    }

    if ( reverseConnTable && stashAddressFindPointer(reverseConnTable, block, &pResult) )
        return pResult;
    return NULL;
}

static void addReverseConnection(BeaconBlock* target, BeaconBlock* source){
    ReverseBlockConnection* conn = getFirstReverseConnection(target);
    ReverseBlockConnection* prev = NULL;
    
    for(; conn; prev = conn, conn = conn->next){
        if(conn->source == source){
            return;
        }
    }
    
    if(prev){
        prev->next = createReverseBlockConnection();
        prev->next->source = source;
    }else{
        conn = createReverseBlockConnection();
        conn->source = source;
        stashAddressAddPointer(reverseConnTable, source, conn, false);
    }
}

static void removeArrayIndex(Array* array, int i){
    assert(i >= 0 && i < array->size);
    
    array->storage[i] = array->storage[--array->size];
}

static void removeFirstGalaxySubBlock(BeaconBlock* galaxy){
    BeaconBlock* subBlock = galaxy->subBlockArray.storage[0];
    BeaconBlock* gridBlock = subBlock->parentBlock;
    BeaconBlock* tempSubBlock;
    int i;
    
    // Mark indices for all beacons.
    
    for(i = 0; i < gridBlock->beaconArray.size; i++){
        Beacon* beacon = gridBlock->beaconArray.storage[i];
        
        beacon->searchInstance = i;
    }
    
    // Remove all beacons from this subBlock.
    
    for(i = 0; i < subBlock->beaconArray.size; i++){
        Beacon* beacon = subBlock->beaconArray.storage[i];
        Beacon* tempBeacon;
        
        // Remove the beacon from the grid block.
        
        tempBeacon = gridBlock->beaconArray.storage[--gridBlock->beaconArray.size];
        gridBlock->beaconArray.storage[beacon->searchInstance] = tempBeacon;
        tempBeacon->searchInstance = beacon->searchInstance;
        
        // Remove the beacon from the combat beacon array.
        
        assert(beacon->userInt >= 0 && beacon->userInt < combatBeaconArray.size);
        tempBeacon = combatBeaconArray.storage[--combatBeaconArray.size];
        combatBeaconArray.storage[beacon->userInt] = tempBeacon;
        tempBeacon->userInt = beacon->userInt;
        
        // Free the connections.
        
        clearArrayEx(&beacon->gbConns, destroyBeaconConnection);
        ZeroStruct(&beacon->gbConns);
        clearArrayEx(&beacon->rbConns, destroyBeaconConnection);
        ZeroStruct(&beacon->rbConns);
    }
    
    // Remove the subBlock from the gridBlock.
    
    tempSubBlock = gridBlock->subBlockArray.storage[--gridBlock->subBlockArray.size];
    gridBlock->subBlockArray.storage[subBlock->searchInstance] = tempSubBlock;
    tempSubBlock->searchInstance = subBlock->searchInstance;
    
    // Remove the gridBlock if it has no more subBlocks.
    
    if(!gridBlock->subBlockArray.size){
        BeaconBlock* tempGridBlock = combatBeaconGridBlockArray.storage[--combatBeaconGridBlockArray.size];
        combatBeaconGridBlockArray.storage[gridBlock->searchInstance] = tempGridBlock;
        tempGridBlock->searchInstance = gridBlock->searchInstance;
                
        stashIntRemovePointer(combatBeaconGridBlockTable, beaconMakeGridBlockHashValue(gridBlock->pos[0], gridBlock->pos[1], gridBlock->pos[2]), NULL);

        destroyBeaconGridBlock(gridBlock);
    }

    clearArrayEx(&subBlock->beaconArray, destroyCombatBeacon);
    destroyBeaconSubBlock(subBlock);
    
    // Remove the subBlock from the galaxy.
    
    galaxy->subBlockArray.storage[0] = galaxy->subBlockArray.storage[--galaxy->subBlockArray.size];
}

static int beaconSubBlockRemoveInvalidConnectionsFromArray(Array* conns){
    int removedCount = 0;
    int i;

    for(i = 0; i < conns->size; i++){
        BeaconBlockConnection* conn = conns->storage[i];
        
        if(!conn->destBlock->galaxy->cluster){
            destroyBeaconBlockConnection(conn);
            removeArrayIndex(conns, i--);
            removedCount++;
        }
    }
    
    return removedCount;
}

static int beaconGalaxyRemoveInvalidConnectionsFromArray(Array* conns){
    int removedCount = 0;
    int i;
    
    for(i = 0; i < conns->size; i++){
        BeaconBlockConnection* conn = conns->storage[i];
        
        if(!conn->destBlock->cluster){
            destroyBeaconBlockConnection(conn);
            removeArrayIndex(conns, i--);
            removedCount++;
        }
    }

    return removedCount;
}

static int beaconRemoveInvalidConnectionsFromArray(Array* conns){
    int removedCount = 0;
    int i;
    
    for(i = 0; i < conns->size; i++){
        BeaconConnection* conn = conns->storage[i];
        
        if(!conn->destBeacon->block->galaxy->cluster){
            destroyBeaconConnection(conn);
            removeArrayIndex(conns, i--);
            removedCount++;
        }
    }

    return removedCount;
}

static void beaconGalaxyRemoveInvalidGalaxyConnections(BeaconBlock* galaxy){
    int removedCount;
    int i;
    
    if(!galaxy->cluster){
        return;
    }
    
    removedCount =    beaconGalaxyRemoveInvalidConnectionsFromArray(&galaxy->gbbConns) +
                    beaconGalaxyRemoveInvalidConnectionsFromArray(&galaxy->rbbConns);
    if(!removedCount)
    {
        return;
    }

    for(i = 0; i < galaxy->subBlockArray.size; i++){
        BeaconBlock* subBlock = galaxy->subBlockArray.storage[i];
        int j;

        removedCount =    beaconSubBlockRemoveInvalidConnectionsFromArray(&subBlock->gbbConns) +
                        beaconSubBlockRemoveInvalidConnectionsFromArray(&subBlock->rbbConns);
        
        if(!removedCount)
        {
            continue;
        }
        
        for(j = 0; j < subBlock->beaconArray.size; j++){
            Beacon* beacon = subBlock->beaconArray.storage[j];
        
            beaconRemoveInvalidConnectionsFromArray(&beacon->gbConns);
            beaconRemoveInvalidConnectionsFromArray(&beacon->rbConns);
        }
    }
}

static void beaconPruneInvalidGalaxies(void){
    S32 i;
    
    // Pre-setup the reverse connection table.
    
    // Prune!

    beaconProcessSetTitle(0, "Prune");

    // Enumerate all blocks for fast deletion.
    
    for(i = 0; i < combatBeaconGridBlockArray.size; i++){
        BeaconBlock* gridBlock = combatBeaconGridBlockArray.storage[i];
        int j;
        
        gridBlock->searchInstance = i;
        
        for(j = 0; j < gridBlock->subBlockArray.size; j++){
            BeaconBlock* subBlock = gridBlock->subBlockArray.storage[j];
            
            subBlock->searchInstance = j;
        }
    }
    
    // Remove galaxy connections to invalid galaxies.
    
    for(i = 0; i < combatBeaconGalaxyArray[0].size; i++){
        BeaconBlock* galaxy = combatBeaconGalaxyArray[0].storage[i];
        
        beaconGalaxyRemoveInvalidGalaxyConnections(galaxy);
    }

    // Check that all valid-to-invalid beacon connections are gone.
    
    for(i = 0; i < combatBeaconArray.size; i++){
        Beacon* beacon = combatBeaconArray.storage[i];
        int j;
        
        if(!beacon->block->galaxy->cluster){
            continue;
        }
        
        for(j = 0; j < beacon->gbConns.size; j++){
            BeaconConnection* conn = beacon->gbConns.storage[j];
            assert(conn->destBeacon->block->galaxy->cluster);
        }

        for(j = 0; j < beacon->rbConns.size; j++){
            BeaconConnection* conn = beacon->rbConns.storage[j];
            assert(conn->destBeacon->block->galaxy->cluster);
        }
    }
    
    // Remove the invalid galaxies.
    
    for(i = 0; i < combatBeaconGalaxyArray[0].size; i++){
        BeaconBlock* galaxy = combatBeaconGalaxyArray[0].storage[i];

        if(galaxy->cluster){
            continue;
        }
        
        while(galaxy->subBlockArray.size){
            removeFirstGalaxySubBlock(galaxy);
        }

        destroyBeaconGalaxyBlock(galaxy);
        
        removeArrayIndex(&combatBeaconGalaxyArray[0], i--);
    }

    beaconProcessSetTitle(100, NULL);
}

void beaconClusterizeGalaxies(int requireValid, int quiet){
    int i;
    int originalBeaconCount = combatBeaconArray.size;
    
    PERFINFO_AUTO_START("clusterize", 1);

    if(!quiet){
        printf("FORMING GALAXY CLUSTERS:");
    }

    destroyArrayPartialEx(&combatBeaconClusterArray, destroyBeaconClusterBlock);

    for(i = 0; i < combatBeaconGalaxyArray[0].size; i++){
        BeaconBlock* galaxy = combatBeaconGalaxyArray[0].storage[i];

        assert(galaxy->isGalaxy);

        galaxy->cluster = NULL;
    }
    
    // Propagate the clusters.
    
    if(!quiet){
        beaconProcessSetTitle(0, "Clusterize");
    }

    for(i = 0; i < combatBeaconArray.size; i++){
        Beacon* beacon = combatBeaconArray.storage[i];
        
        if(!quiet && !(i % 100)){
            printf(".");
        }
        
        if(!requireValid || beacon->isValidStartingPoint){
            BeaconBlock* galaxy = beacon->block->galaxy;
            
            assert(galaxy);
            
            if(!galaxy->cluster){
                BeaconBlock* cluster = createBeaconBlock();
                
                cluster->isCluster = 1;
                
                tempGalaxyArray.size = 0;
                
                beaconPropagateCluster(galaxy, cluster, quiet);
                
                beaconBlockInitCopyArray(&cluster->subBlockArray, &tempGalaxyArray);
                
                if(!quiet){
                    printf("(%d)", cluster->subBlockArray.size);
                }
                
                arrayPushBack(&combatBeaconClusterArray, cluster);
            }
        }
    }
    
    if(!quiet){
        beaconProcessSetTitle(100, NULL);
    }

    // Remove useless galaxies.
    
    if(requireValid){
        beaconPruneInvalidGalaxies();
    }
    
    // Sort the cluster array by cluster size.
    
    qsort(    combatBeaconClusterArray.storage,
            combatBeaconClusterArray.size,
            sizeof(combatBeaconClusterArray.storage[0]),
            compareBeaconClusterSize);

    PERFINFO_AUTO_STOP();

    // Create more galaxy groups.
    
    //printf("----------------------------------------------------------------------------------------------------\n");
    
    PERFINFO_AUTO_START("grouping", 1);

    if(!quiet){
        printf("DONE!\n\nGROUPING GALAXIES: ");
    }

    beaconCreateGalaxyGroups(quiet);
    
    PERFINFO_AUTO_STOP();

    if(!quiet){
        printf(    "DONE!\n\n"
                "  Clusters:        %d\n"
                "  Removed Beacons: %d/%d\n"
                "  Galaxy Groups: ",
                combatBeaconClusterArray.size,
                originalBeaconCount - combatBeaconArray.size,
                originalBeaconCount);

        for(i = 0; i < ARRAY_SIZE(combatBeaconGalaxyArray); i++){
            printf("%s%d", i ? ", " : "", combatBeaconGalaxyArray[i].size);
        }
        
        printf("\n");
    }
}

static struct {
    Vec3        source;
    Beacon**    beacons;
    int            count;
    int            maxCount;
    F32            maxDistSQR;
} collectedBeacons;

static void collectNearbyBeacons(Array* beaconArray, void* userData){
    int i;
    
    for(i = 0; i < beaconArray->size; i++){
        Beacon* b = beaconArray->storage[i];
        
        F32 distSQR = distance3Squared(posPoint(b), collectedBeacons.source);
        
        if(    collectedBeacons.count < collectedBeacons.maxCount &&
            distSQR <= collectedBeacons.maxDistSQR)
        {
            collectedBeacons.beacons[collectedBeacons.count++] = b;
            
            b->userFloat = distSQR;
        }
    }
}

static int __cdecl compareBeaconDistance(const void* b1Data, const void* b2Data){
    const Beacon** b1 = (const Beacon**)b1Data;
    const Beacon** b2 = (const Beacon**)b2Data;
    F32 d1 = (*b1)->userFloat;
    F32 d2 = (*b2)->userFloat;
    
    if(d1 < d2)
        return -1;
    else if (d1 == d2)
        return 0;
    else
        return 1;
}

static BeaconClusterConnection* beaconGetClusterConnection(    BeaconBlock* sourceCluster,
                                                            Vec3 sourcePos,
                                                            Entity* sourceEntity,
                                                            BeaconBlock* targetCluster,
                                                            Vec3 targetPos,
                                                            Entity* targetEntity)
{
    int i;
    
    for(i = 0; i < sourceCluster->gbbConns.size; i++){
        BeaconClusterConnection* conn = sourceCluster->gbbConns.storage[i];
        
        if(    conn->targetCluster == targetCluster &&
            sameVec3(conn->source.pos, sourcePos) &&
            erGetEnt(conn->source.entity) == sourceEntity &&
            sameVec3(conn->target.pos, targetPos) &&
            erGetEnt(conn->target.entity) == targetEntity)
        {
            return conn;
        }
    }
    
    return NULL;
}

static int beaconCreateClusterConnections(    Entity* sourceEntity, Vec3 sourcePos, F32 radius1,
                                            Entity* targetEntity, Vec3 targetPos, F32 radius2)
{
    Beacon* sourceBeacons[10000];
    int sourceCount;
    Beacon* targetBeacons[10000];
    int targetCount;
    int i;
    int connCount = 0;

    // Get a list of source beacons.
    
    copyVec3(sourcePos, collectedBeacons.source);
    collectedBeacons.beacons = sourceBeacons;
    collectedBeacons.count = 0;
    collectedBeacons.maxCount = ARRAY_SIZE(sourceBeacons);
    collectedBeacons.maxDistSQR = SQR(radius1);
    
    beaconForEachBlock(collectedBeacons.source, radius1, radius1, radius1, collectNearbyBeacons, NULL);
    
    sourceCount = collectedBeacons.count;
    
    if(!sourceCount){
        return 0;
    }
    
    // Get a list of target beacons.

    copyVec3(targetPos, collectedBeacons.source);
    collectedBeacons.beacons = targetBeacons;
    collectedBeacons.count = 0;
    collectedBeacons.maxCount = ARRAY_SIZE(targetBeacons);
    collectedBeacons.maxDistSQR = SQR(radius2);
    
    beaconForEachBlock(collectedBeacons.source, radius2, radius2, radius2, collectNearbyBeacons, NULL);
    
    targetCount = collectedBeacons.count;
    
    if(!targetCount){
        return 0;
    }
    
    // Sort the beacons by distance.
    
    qsort(sourceBeacons, sourceCount, sizeof(sourceBeacons[0]), compareBeaconDistance);
    qsort(targetBeacons, targetCount, sizeof(targetBeacons[0]), compareBeaconDistance);
    
    // Create connections.

    for(i = 0; i < sourceCount; i++){
        Beacon* sourceBeacon = sourceBeacons[i];
        BeaconBlock* sourceCluster;
        int j;
        
        if(    !sourceBeacon->block ||
            !sourceBeacon->block->galaxy ||
            !sourceBeacon->block->galaxy->cluster)
        {
            continue;
        }
        
        sourceCluster = sourceBeacon->block->galaxy->cluster;

        for(j = 0; j < targetCount; j++){
            Beacon* targetBeacon = targetBeacons[j];
            BeaconClusterConnection* conn;
            BeaconBlock* targetCluster = SAFE_MEMBER3(targetBeacon, block, galaxy, cluster);

            if(!targetCluster){
                continue;
            }

            conn = beaconGetClusterConnection(    sourceCluster, sourcePos, sourceEntity,
                                                targetCluster, targetPos, targetEntity);
            
            if(!conn){
                conn = createBeaconClusterConnection();
                
                conn->targetCluster = targetCluster;
                
                copyVec3(sourcePos, conn->source.pos);
                conn->source.beacon = sourceBeacon;
                conn->source.entity = erGetRef(sourceEntity);
                
                copyVec3(targetPos, conn->target.pos);
                conn->target.beacon = targetBeacon;
                conn->target.entity = erGetRef(targetEntity);
                
                arrayPushBack(&sourceCluster->gbbConns, conn);
            }

            connCount++;
        }
    }
    
    return connCount;
}

static int countGroupDefs(GroupDef* def, Mat4 parent_mat){
    beacon_process.groupDefCount++;
    
    return 1;
}

static void beaconGenerateCombatBeaconsWrapper(void){
    #if 0
    int oldBeaconCount = combatBeaconArray.size;
    
    consoleSetDefaultColor();
    
    // Count the defs.
    
    beacon_process.groupDefCount = 0;
    
    groupProcessDef(&group_info, countGroupDefs);

    printf(    "----------------------------------------------------------------------------------------------------\n"
            "- CREATING BEACONS (%d refs)...\n",
            beacon_process.groupDefCount);

    // Generate.

    beaconProcessSetTitle(0, "Generate");
    {
        beaconGenerateCombatBeacons();
    }
    beaconProcessSetTitle(100, NULL);
    
    printf(    "- Generated Beacons: %d\n"
            "- DONE! (%s)\n"
            "----------------------------------------------------------------------------------------------------\n",
            combatBeaconArray.size - oldBeaconCount,
            beaconCurTimeString(0));
    #endif
}

void beaconProcessCombatBeacons(int doGenerate, int doProcess){
    #if 0
    int i, j;
    Beacon* beacon;
    BeaconBlock* block;
    int requireValid;
    
    entMotionFreeCollAll();

    // Generate combat beacons from world geometry.
    
    if(doGenerate)
    {
        beaconGenerateCombatBeaconsWrapper();
    }
    
    if(!doProcess)
    {
        return;
    }
    
    beacon_process.memoryAllocated = 0;

    beacon_process.entity = beaconCreatePathCheckEnt();
    
    assert(combatBeaconArray.size < (1 << 24));

    beacon_process.infoArray = beaconAllocateMemory(combatBeaconArray.size * sizeof(*beacon_process.infoArray));
    beacon_process.processOrder = beaconAllocateMemory(combatBeaconArray.size * sizeof(*beacon_process.processOrder));
    beacon_process.legalCount = 0;
    
    // Pack all the legal beacons at the front of the array.

    for(i = 0; i < combatBeaconArray.size; i++){
        Beacon* b = combatBeaconArray.storage[i];
        
        if(b->isValidStartingPoint){
            beacon_process.legalCount++;
        }
    }
    
    for(i = 0, j = 0; i < combatBeaconArray.size; i++){
        Beacon* b = combatBeaconArray.storage[i];
        
        if(!beacon_process.legalCount || b->isValidStartingPoint){
            beacon_process.infoArray[i].isLegal = 1;
            beacon_process.processOrder[j++] = i;
            assert(!beacon_process.legalCount || j <= beacon_process.legalCount);
        }
    }

    requireValid = beacon_process.legalCount ? 1 : 0;

    if(!beacon_process.legalCount){
        beacon_process.legalCount = combatBeaconArray.size;
    }

    assert(j == beacon_process.legalCount);

    // Store the beacon index for quick lookup.

    for(i = 0; i < combatBeaconArray.size; i++){
        beacon = combatBeaconArray.storage[i];
        beacon->userInt = i;
    }

    // Make ground connections.
    
    beaconDoBeaconProcessing();
    
    // Free the processing data.

    SAFE_FREE(beacon_process.infoArray);
    beacon_process.memoryAllocated = 0;
    
    // Break beacons up into sub-blocks.
    
    for(i = 0; i < combatBeaconGridBlockArray.size; i++){
        block = combatBeaconGridBlockArray.storage[i];
        
        for(j = 0; j < block->beaconArray.size; j++){
            beacon = block->beaconArray.storage[j];
            
            assert(beacon->block == block);
        }
    }

    printf(    "----------------------------------------------------------------------------------------------------\n"
            "SPLITTING %d BLOCKS @ %s: ",
            combatBeaconGridBlockArray.size,
            beaconCurTimeString(0));

    beaconSplitBlocks(0);

    printf(".DONE!\n\n");

    // Make connections between beacon blocks.
    
    printf(    "----------------------------------------------------------------------------------------------------\n"
            "CONNECTING %d BLOCKS @ %s: ",
            combatBeaconGridBlockArray.size,
            beaconCurTimeString(0));

    beaconProcessSetTitle(0, "BlockConn");

    for(i = 0; i < combatBeaconGridBlockArray.size; i++){
        block = combatBeaconGridBlockArray.storage[i];

        printf(".");
        
        beaconProcessSetTitle(100.0f * i / combatBeaconGridBlockArray.size, NULL);

        beaconMakeBlockConnections(block);
    }

    beaconProcessSetTitle(100, NULL);

    printf("DONE!\n\n");

    // Calculate block galaxies.
    
    printf(    "----------------------------------------------------------------------------------------------------\n"
            "FORMING SUB-BLOCK GALAXIES @ %s: ",
            beaconCurTimeString(0));

    beaconCreateGalaxies(0);

    // Clusterize.
    
    printf("----------------------------------------------------------------------------------------------------\n");

    beaconClusterizeGalaxies(requireValid, 0);
    
    // Delete the temp dude.

    //entFree(beacon_process.entity);

    // Done.

    printf("----------------------------------------------------------------------------------------------------\n");
    #endif
}

void temp_beaconRepairConnections(void){
    F32 y = -1956.0;
    int i;
    
    assert(0);
    
    // This function needs to be fixed if it's needed at all.
    
    for(i = 0; i < combatBeaconArray.size; i++){
        Beacon* b = combatBeaconArray.storage[i];
        
        if(fabs(posY(b) - y) < 5){
            int j;
            
            for(j = 0; j < b->rbConns.size; j++){
                BeaconConnection* c = b->rbConns.storage[j];
                
                if(    fabs(posY(c->destBeacon) - y) < 5 &&
                    !beaconHasGroundConnectionToBeacon(b, c->destBeacon, NULL))
                {
                    BeaconConnection* newConn = createBeaconConnection();
                    
                    newConn->destBeacon = c->destBeacon;
                    
                    arrayPushBack(&b->gbConns, newConn);
                }
            }
        }
    }
}

void beaconClearAllBlockData(int freeBlockMemory){
    int i;
    
    // Destroy the current block data.
    
    for(i = 0; i < combatBeaconGridBlockArray.size; i++){
        BeaconBlock* gridBlock = combatBeaconGridBlockArray.storage[i];
        
        clearArrayEx(&gridBlock->subBlockArray, destroyBeaconSubBlock);
    }
    
    for(i = 0; i < ARRAY_SIZE(combatBeaconGalaxyArray); i++){
        clearArrayEx(combatBeaconGalaxyArray + i, destroyBeaconGalaxyBlock);
    }
    
    beaconBlockFreeAllMemory(freeBlockMemory);

    destroyArrayPartialEx(&combatBeaconClusterArray, destroyBeaconClusterBlock);
}

void beaconProcessPathLimiterVolumes(void)
{
    int i, beaconCount = combatBeaconArray.size;

    if(!aiGetPathLimitedVolumeCount())
        return;

    for(i = 0; i < beaconCount; i++)
    {
        Beacon* beacon = (Beacon*)combatBeaconArray.storage[i];
        beacon->pathLimitedAllowed = aiCheckIfInPathLimitedVolume(posPoint(beacon));
    }
}

static void beaconSetBlocksForRebuild(void){
    S32 i;
    
    beaconClearAllBlockData(0);
    
    beacon_state.needBlocksRebuilt = 1;

    for(i = 0; i < player_ents[PLAYER_ENTS_ACTIVE].count; i++){
        Entity* player = player_ents[PLAYER_ENTS_ACTIVE].ents[i];
        ClientLink* client = clientFromEnt(player);
        
        if(    client &&
            !client->sentResendWaypointUpdate)
        {
            client->sentResendWaypointUpdate = 1;
        
            START_PACKET(pak, player, SERVER_RESEND_WAYPOINT_REQUEST);
            END_PACKET
        }
    }
}

void beaconCheckBlocksNeedRebuild(void){
    if(beacon_state.needBlocksRebuilt){
        beacon_state.needBlocksRebuilt = 0;

        beaconRebuildBlocks(0, 1);
        
        dbDoorCreateBeaconConnections();
    }
}

void beaconRebuildBlocks(int requireValid, int quiet){
    PERFINFO_AUTO_START("beaconRebuildBlocks", 1);
        // Clear out the old block data.
    
        PERFINFO_AUTO_START("clear blocks", 1);
        
            beaconClearAllBlockData(0);

        // Mark beacons with PathLimiter volume info

        PERFINFO_AUTO_STOP_START("process volumes", 1);
        
            beaconProcessPathLimiterVolumes();
        
        // Recreate the blocks.
        
        PERFINFO_AUTO_STOP_START("split", 1);
        
            beaconSplitBlocksAndGalaxies(quiet);
        
        PERFINFO_AUTO_STOP_START("clusterize", 1);
        
            beaconClusterizeGalaxies(requireValid, quiet);
        
        PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();
}

typedef struct CheckTriangle {
    Vec3             min_xyz;
    Vec3             max_xyz;
    Vec3             verts[3];
    Vec3             normal;
} CheckTriangle;

static struct {
    Vec3                        min_xyz;
    Vec3                        max_xyz;
    CheckTriangle*                tris;
    S32                            tri_count;
    
    BeaconDynamicInfo*            dynamicInfo;
} gatherConnsData;

static int checkInterpedIntersection(Vec3 a, Vec3 b, int interp_idx, int check_idx, F32 interp_to_value){
    F32 diff = b[interp_idx] - a[interp_idx];
    F32 ratio;
    F32 check_value;
    
    if(diff == 0){
        return 0;
    }
    
    ratio = (interp_to_value - a[interp_idx]) / diff;
    
    check_value = a[check_idx] + ratio * (b[check_idx] - a[check_idx]);
    
    if(    check_value >= gatherConnsData.min_xyz[check_idx] &&
        check_value <= gatherConnsData.max_xyz[check_idx])
    {
        return 1;
    }
    
    return 0;
}

static int connIntersectsBoxXZ(    Beacon* source,
                                BeaconConnection* conn,
                                Vec3 min_xyz,
                                Vec3 max_xyz,
                                Vec3 seg_min_xyz_param,
                                Vec3 seg_max_xyz_param,
                                int write_seg_minmax)
{
    Beacon* target = conn->destBeacon;
    Vec3 seg_min_xyz;
    Vec3 seg_max_xyz;
    int found_outside = 0;
    int i;
    
    if(write_seg_minmax){
        // Get the box around the connection segment.
        
        for(i = 0; i < 3; i++){
            if(source->mat[3][i] < target->mat[3][i]){
                seg_min_xyz[i] = source->mat[3][i];
                seg_max_xyz[i] = target->mat[3][i];
            }else{
                seg_min_xyz[i] = target->mat[3][i];
                seg_max_xyz[i] = source->mat[3][i];
            }

            if(i != 1 && (seg_min_xyz[i] > max_xyz[i] || seg_max_xyz[i] < min_xyz[i])){
                return 0;
            }
        }

        copyVec3(seg_min_xyz, seg_min_xyz_param);
        copyVec3(seg_max_xyz, seg_max_xyz_param);
    }else{
        copyVec3(seg_min_xyz_param, seg_min_xyz);
        copyVec3(seg_max_xyz_param, seg_max_xyz);

        for(i = 0; i < 3; i += 2){
            if(seg_min_xyz[i] > max_xyz[i] || seg_max_xyz[i] < min_xyz[i]){
                return 0;
            }
        }
    }
            
    // Got to here, so XZ overlap exists somewhere.
    
    for(i = 0; i < 3; i += 2){
        if(seg_min_xyz[i] <= min_xyz[i]){
            found_outside = 1;
            if(checkInterpedIntersection(posPoint(source), posPoint(target), i, 2 - i, min_xyz[i])){
                return 1;
            }
        }
        
        if(seg_max_xyz[i] >= max_xyz[i]){
            found_outside = 1;
            if(checkInterpedIntersection(posPoint(source), posPoint(target), i, 2 - i, max_xyz[i])){
                return 1;
            }
        }
    }
    
    // If nothing was found outside the box then segment must be fully contained in the box.
    
    return !found_outside;
}

static void getACD(Vec3 p0, Vec3 p1, F32* a, F32* c, F32* d){
    // Do the subtraction and rotation at the same time to get the segment normal.

    *a = vecZ(p1) - vecZ(p0);
    *c = -(vecX(p1) - vecX(p0));
    *d = -(*a * vecX(p0) + *c * vecZ(p0));
}

static int lineIntersectsTriangle(Vec3 p0, Vec3 p1, CheckTriangle* tri){
    Vec3     dv0p0;
    Vec3     dp1p0;
    Vec3     ipos;
    int        side_count = 0;
    int        i;
    F32        r;
    
    subVec3(tri->verts[0], p0, dv0p0);
    subVec3(p1, p0, dp1p0);
    
    r = dotVec3(tri->normal, dv0p0) / dotVec3(tri->normal, dp1p0);
    
    scaleVec3(dp1p0, r, ipos);
    addVec3(ipos, p0, ipos);
    
    if(    distance3Squared(p0, ipos) > distance3Squared(p0, p1) ||
        distance3Squared(p1, ipos) > distance3Squared(p1, p0))
    {
        return 0;
    }
    
    // Check if the point is in the triangle.
    
    for(i = 0; i < 3; i++){
        Vec3 temp;
        Vec3 pnormal;
        subVec3(tri->verts[(i+1)%3], tri->verts[i], temp);
        crossVec3(temp, tri->normal, pnormal);
        subVec3(ipos, tri->verts[i], temp);
        if(dotVec3(pnormal, temp) >= 0){
            side_count++;
        }
    }
    
    if(!side_count || side_count == 3){
        #if 0
        // This will send the connection points to the client.
        
        {
            Vec3 pos;
            S32 i;
            
            for(i = 0; i < 3; i++){
                copyVec3(ipos, pos);
                pos[i] += 0.5f;
                aiEntSendAddLine(ipos, 0xffffffff, pos, 0xffff0000);

                copyVec3(ipos, pos);
                pos[i] -= 0.5f;
                aiEntSendAddLine(ipos, 0xffffffff, pos, 0xffff0000);
            }
        }
        #endif
        
        return 1;
    }
    
    return 0;
}

static int connIntersectsTriangles(Beacon* source, BeaconConnection* conn, Vec3 min_xyz, Vec3 max_xyz){
    Beacon* target = conn->destBeacon;
    F32        sourceTargetDistSQRXZ = distance3XZ(posPoint(source), posPoint(target));
    F32        sourceToTargetDistY = posY(target) - posY(source);
    F32        connMinY = posY(source) + conn->minHeight;
    F32        connMaxY = posY(source) + conn->maxHeight;
    F32        a, c, d;
    int        i;
    
    getACD(posPoint(source), posPoint(target), &a, &c, &d);
    
    for(i = 0; i < gatherConnsData.tri_count; i++){
        CheckTriangle* tri = gatherConnsData.tris + i;
        int j;
        
        for(j = 0; j < 3; j++){
            CheckTriangle curTri = *tri;
            Vec3 offset;
            int k;
            
            scaleVec3(curTri.normal, j - 1, offset);
            
            for(k = 0; k < 3; k++){
                addVec3(curTri.verts[k], offset, curTri.verts[k]);
            }
            
            addVec3(curTri.min_xyz, offset, curTri.min_xyz);
            addVec3(curTri.max_xyz, offset, curTri.max_xyz);

            if(connIntersectsBoxXZ(source, conn, curTri.min_xyz, curTri.max_xyz, min_xyz, max_xyz, 0)){
                if(lineIntersectsTriangle(posPoint(source), posPoint(target), &curTri)){
                    return 1;
                }
            
                for(k = 0; k < 3; k++){
                    F32 ta, tc, td;
                    F32 denom;
                    Vec3 vert0;
                    Vec3 vert1;
                    
                    copyVec3(curTri.verts[k], vert0);
                    copyVec3(curTri.verts[(k + 1) % 3], vert1);
                    
                    getACD(vert0, vert1, &ta, &tc, &td);
                    
                    denom = c * ta - tc * a;
                    
                    if(denom != 0){
                        Vec3 ipos;
                        int j;
                        int on_segs;
                        
                        ipos[2] = (d * ta - td * a) / denom;
                        
                        if(a != 0){
                            ipos[0] = (c * ipos[2] + d) / a;
                        }
                        else if(ta != 0){
                            ipos[0] = (tc * ipos[2] + td) / ta;
                        }
                        else{
                            // These segments overlap I guess.  Fuck.  Uh, ignore it?  Maybe?
                            
                            continue;
                        }

                        // Check that the intersection point is on both line segments.
                        
                        on_segs = 1;

                        for(j = 0; j < 3; j += 2){
                            if(source->mat[3][j] < target->mat[3][j]){
                                if(ipos[j] < source->mat[3][j] || ipos[j] > target->mat[3][j]){
                                    on_segs = 0;
                                    break;
                                }
                            }else{
                                if(ipos[j] < target->mat[3][j] || ipos[j] > source->mat[3][j]){
                                    on_segs = 0;
                                    break;
                                }
                            }
                            
                            if(vert0[j] < vert1[j]){
                                if(ipos[j] < vert0[j] || ipos[j] > vert1[j]){
                                    on_segs = 0;
                                    break;
                                }
                            }else{
                                if(ipos[j] < vert1[j] || ipos[j] > vert0[j]){
                                    on_segs = 0;
                                    break;
                                }
                            }
                        }
                        
                        if(tc){
                            // Interp y based on x.
                            
                            ipos[1] = (ipos[0] - vert0[0]) * (vert1[1] - vert0[1]) / (vert1[0] - vert0[0]);
                        }else{
                            // Interp y based on z.
                            
                            ipos[1] = (ipos[2] - vert0[2]) * (vert1[1] - vert0[1]) / (vert1[2] - vert0[2]);
                        }
                        
                        // Get the y value at the same x,z on the line between source and target.
                        
                        if(*(int*)&conn->minHeight){
                            // Check position is between the min and max heights of the connection.
                            
                            if(ipos[1] >= connMinY && ipos[1] <= connMaxY){
                                return 1;
                            }
                        }else{
                            // Check the segment between source and target for proximity to the triangle.
                            
                            F32 y = posY(source) + sourceToTargetDistY * distance3XZ(posPoint(source), ipos) / sourceTargetDistSQRXZ;

                            if(fabs(y - vecY(ipos)) < 10){
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    
    return 0;
}

static StashTable beaconConnToDynamicConnTable;

MP_DEFINE(BeaconDynamicConnection);

static BeaconDynamicConnection* createBeaconDynamicConnection(void){
    MP_CREATE(BeaconDynamicConnection, 100);
    
    return MP_ALLOC(BeaconDynamicConnection);
}

static void destroyBeaconDynamicConnection(BeaconDynamicConnection* conn){
    if(conn){
        eaDestroy(&conn->infos);
        
        MP_FREE(BeaconDynamicConnection, conn);
    }
}

MP_DEFINE(BeaconDynamicInfo);

static BeaconDynamicInfo* createBeaconDynamicInfo(void){
    MP_CREATE(BeaconDynamicInfo, 100);
    
    return MP_ALLOC(BeaconDynamicInfo);
}

static void destroyBeaconDynamicInfo(BeaconDynamicInfo* info){
    MP_FREE(BeaconDynamicInfo, info);
}

static BeaconDynamicConnection* getExistingDynamicConnection(BeaconConnection* conn){
    BeaconDynamicConnection* dynConn;

    if(beaconConnToDynamicConnTable && stashAddressFindPointer(beaconConnToDynamicConnTable, conn, &dynConn)){
        return dynConn;
    }
    
    return NULL;
}

static BeaconDynamicConnection* getDynamicConnection(Beacon* source, int raised, int index, int create){
    Array* conns = raised ? &source->rbConns : &source->gbConns;
    BeaconDynamicConnection* dynConn;
    BeaconConnection* conn;
    
    assert(index >= 0 && index < conns->size);
    
    conn = conns->storage[index];
    
    dynConn = getExistingDynamicConnection(conn);
    
    if(!dynConn && create){
        dynConn = createBeaconDynamicConnection();
        
        if(!beaconConnToDynamicConnTable){
            beaconConnToDynamicConnTable = stashTableCreateAddress(100);
        }
        
        stashAddressAddPointer(beaconConnToDynamicConnTable, conn, dynConn, false);
        
        dynConn->source = source;
        dynConn->conn = conn;
        dynConn->raised = raised;
    }
    
    return dynConn;
}

static void checkConnection(Beacon* b,
                            BeaconConnection* conn,
                            BeaconDynamicConnection* dynConn,
                            S32 raised,
                            S32 index)
{
    Vec3 min_xyz;
    Vec3 max_xyz;
    
    if(connIntersectsBoxXZ(    b,
                            conn,
                            gatherConnsData.min_xyz,
                            gatherConnsData.max_xyz,
                            min_xyz, 
                            max_xyz,
                            1))
    {
        if(connIntersectsTriangles(b, conn, min_xyz, max_xyz)){
            //aiEntSendAddLine(posPoint(b), 0xffffffff, posPoint(conn->destBeacon), 0xffff6000);
            
            if(!dynConn){
                dynConn = getDynamicConnection(b, raised, index, 1);
            }else{
                assert(dynConn->conn == conn);
            }
            
            eaPush(&gatherConnsData.dynamicInfo->conns, dynConn);
            
            eaPush(&dynConn->infos, gatherConnsData.dynamicInfo);
        }
    }
}

static void checkConnectionsForBeacon(Beacon* b, int raised){
    Array* array = raised ? &b->rbConns : &b->gbConns;
    int j;
    
    for(j = 0; j < array->size; j++){
        checkConnection(b, array->storage[j], NULL, raised, j);
    }
}

static void checkDisabledConnectionsForBeacon(Beacon* b){
    BeaconDynamicConnection* dynConn;
    
    for(dynConn = b->disabledConns; dynConn; dynConn = dynConn->next){
        checkConnection(b, dynConn->conn, dynConn, 0, 0);
    }
}

static void gatherAllBeaconConnections(Beacon* beacon, void* unused){
    checkConnectionsForBeacon(beacon, 0);
    checkConnectionsForBeacon(beacon, 1);
    checkDisabledConnectionsForBeacon(beacon);
}
        
static void gatherConnections(Array* beaconArray, void* userData){
    arrayForEachItem(beaconArray, gatherAllBeaconConnections, NULL);
}

#define MAX_MODELS 100

typedef struct GatheredModel {
    Model*    model;
    Mat4    mat;
} GatheredModel;

static void getTheModels(GroupDef* def, Mat4 mat, GatheredModel* modelList, int* modelCount){
    int i;

    if(    *modelCount >= MAX_MODELS ||
        def->no_coll)
    {
        return;
    }

    if(def->model){
        modelCreateCtris(def->model);
        
        if(def->model->grid.size > 0.0f){
            modelList[*modelCount].model = def->model;
            copyMat4(mat, modelList[*modelCount].mat);
            
            (*modelCount)++;
        }
    }

    for(i = 0; i < def->count; i++){
        Mat4 matChild;
        
        mulMat4(mat, def->entries[i].mat, matChild);
        
        getTheModels(def->entries[i].def, matChild, modelList, modelCount);
    }
}

typedef struct BeaconDynamicDefTimer {
    char*                name;
    PerformanceInfo*    perfInfo;
} BeaconDynamicDefTimer;

static void startNamedTimer(const char* name){
    static StashTable stTimers = NULL;
    
    BeaconDynamicDefTimer* defTimer;
    
    if(!stTimers){
        stTimers = stashTableCreateWithStringKeys(100, 0);
    }

    if(!stashFindPointer(stTimers, name, (void**)&defTimer)){
        callocStruct(defTimer, BeaconDynamicDefTimer);
        
        defTimer->name = strdup(name);

        stashAddPointer(stTimers, defTimer->name, defTimer, true);
    }
    
    PERFINFO_AUTO_START_STATIC(defTimer->name, &defTimer->perfInfo, 1);
}

static void beaconCreateDynamicConnectionSet(    BeaconDynamicInfo* dynamicInfo,
                                                DefTracker* tracker)
{
    CheckTriangle tri_buffer[1000];
    int h,i;
    GatheredModel modelList[MAX_MODELS];
    int modelCount = 0;
    int totalTris = 0;
    Vec3 min_xyz;
    Vec3 max_xyz;
    Vec3 box_center;
    Vec3 span_xyz;
    
    PERFINFO_RUN(startNamedTimer(tracker->def->name););
    
    getTheModels(tracker->def, tracker->mat, modelList, &modelCount);

    // Calculate the bounding box.

    setVec3same(min_xyz, FLT_MAX);
    setVec3same(max_xyz, -FLT_MAX);
    
    totalTris = 0;
    for(h = 0; h < modelCount; h++)
    {
        GatheredModel* gatheredModel = modelList + h;
        Model* model = gatheredModel->model;

        PERFINFO_RUN(startNamedTimer(model->name););

        if(!model->ctris){
            modelCreateCtris(model);
            assert(model->ctris);
        }

        for(i = 0 ; i < model->tri_count ; i++){
            CTri* tri;
            Vec3 verts[3];
            int j;
            CheckTriangle* checkTri = tri_buffer + totalTris;

            if(totalTris >= ARRAY_SIZE(tri_buffer))
                break;
                
            totalTris++;
            
            tri = model->ctris + i;

            mulVecMat3(tri->norm, gatheredModel->mat, checkTri->normal);

            expandCtriVerts(tri, verts);

            setVec3same(checkTri->min_xyz, FLT_MAX);
            setVec3same(checkTri->max_xyz, -FLT_MAX);

            for(j = 0; j < 3; j++){
                Vec3 vert;
                int k;

                mulVecMat4(verts[j], gatheredModel->mat, vert);

                for(k = 0; k < 3; k++){
                    min_xyz[k] = min(min_xyz[k], vert[k]);
                    max_xyz[k] = max(max_xyz[k], vert[k]);
                }

                copyVec3(vert, checkTri->verts[j]);

                for(k = 0; k < 3; k++){
                    checkTri->min_xyz[k] = min(vert[k], checkTri->min_xyz[k]);
                    checkTri->max_xyz[k] = max(vert[k], checkTri->max_xyz[k]);
                }
            }

            for(j = 0; j < 3; j++){
            }
            
            totalTris++;
        }

        PERFINFO_AUTO_STOP();
    }
    
    gatherConnsData.tri_count = totalTris;
    gatherConnsData.tris = tri_buffer;

    // Calculate the center of the box.
    
    for(i = 0; i < 3; i++){
        min_xyz[i] -= 1;
        max_xyz[i] += 1;
    }
    
    addVec3(min_xyz, max_xyz, box_center);
    scaleVec3(box_center, 0.5, box_center);
    
    // Calculate the size of the box.
    
    subVec3(max_xyz, min_xyz, span_xyz);
    
    // Check all the triangles against the stuff.
    
    copyVec3(min_xyz, gatherConnsData.min_xyz);
    copyVec3(max_xyz, gatherConnsData.max_xyz);
    
    gatherConnsData.dynamicInfo = dynamicInfo;

    if(span_xyz[0] < 1000 && span_xyz[1] < 1000 && span_xyz[2] < 1000)
    {
        PERFINFO_AUTO_START("compare beacon conns", 1);
            beaconForEachBlock(box_center, span_xyz[0] + 100, span_xyz[1] + 100, span_xyz[2] + 100, gatherConnections, NULL);
        PERFINFO_AUTO_STOP();
    }
    
    PERFINFO_AUTO_STOP();
}

static void beaconCheckNavSearchData(Entity* e){
    AIVars* ai = ENTAI(e);
    
    if(ai && ai->navSearch){
        NavPath* path = &ai->navSearch->path;
        NavPathWaypoint* wp = path->head;

        while(wp){
            if(wp->connectionToMe && wp->prev && wp->prev->beacon){
                BeaconDynamicConnection* dynConn = getExistingDynamicConnection(wp->connectionToMe);
                
                if(SAFE_MEMBER(dynConn, blockedCount)){
                    // This connection is bad, so just scrap the whole path.
                    
                    aiDiscardFollowTarget(e, "My path went through a door that just closed.", false);
                    clearNavSearch(ai->navSearch);
                    
                    break;
                }
            }
            
            wp = wp->next;
        }        
    }
}

static void beaconResetEntityDynamicData(void){
    int i;
    
    for(i = 0; i < entities_max; i++){
        if(entity_state[i] & ENTITY_IN_USE){
            Entity* e = entities[i];
            
            e->nearestBeacon = NULL;

            beaconCheckNavSearchData(e);
        }
    }
}

void beaconSetDynamicConnections(    BeaconDynamicInfo** dynamicInfoParam,
                                    Entity* entity,
                                    DefTracker* tracker,
                                    int connsEnabled,
                                    int isDoor)
{
    BeaconDynamicInfo* dynamicInfo = *dynamicInfoParam;
    int count;
    int i;
    
    if(combatBeaconArray.size > 20000 || db_state.is_static_map || isDevelopmentMode() && combatBeaconArray.size > 20000){
        return;
    }
    
    if(!dynamicInfo){
        dynamicInfo = *dynamicInfoParam = createBeaconDynamicInfo();
        
        dynamicInfo->connsEnabled = 1;
        dynamicInfo->entity = entity;
        dynamicInfo->isDoor = isDoor;

        PERFINFO_AUTO_START("beaconCreateDynamicConnectionSet", 1);
            beaconCreateDynamicConnectionSet(dynamicInfo, tracker);
        PERFINFO_AUTO_STOP();
    }
    
    if(!eaSize(&dynamicInfo->conns)){
        return;
    }
    
    connsEnabled = connsEnabled ? 1 : 0;
    
    if(connsEnabled == dynamicInfo->connsEnabled){
        return;
    }
    
    if(connsEnabled){
        PERFINFO_AUTO_START("beaconEnableConns", 1);
    }else{
        PERFINFO_AUTO_START("beaconDisableConns", 1);
    }

    dynamicInfo->connsEnabled = connsEnabled;
    
    count = eaSize(&dynamicInfo->conns);
    
    for(i = 0; i < count; i++){
        BeaconDynamicConnection* dynConn = dynamicInfo->conns[i];
        Beacon* b = dynConn->source;
        Array* conns = dynConn->raised ? &b->rbConns : &b->gbConns;
        
        beaconSortConnsByTarget(b, dynConn->raised);

        if(connsEnabled){
            if(dynamicInfo->isDoor){
                assert(dynConn->doorBlockedCount > 0);

                dynConn->doorBlockedCount--;
            }
            
            assert(dynConn->blockedCount > 0);
            
            if(--dynConn->blockedCount > 0){
                continue;
            }
            
            // All references removed, so re-enable it, and force a re-sort of the connection array.
            
            assert(conns->size < conns->maxSize);
            
            conns->storage[conns->size++] = dynConn->conn;
            
            if(dynConn->raised)
                dynConn->source->raisedConnsSorted = 0;
            else
                dynConn->source->groundConnsSorted = 0;
                
            // Remove from the source beacon's list of disable conns.
            
            if(dynConn->prev){
                dynConn->prev->next = dynConn->next;
            }else{
                assert(dynConn->source->disabledConns == dynConn);
                dynConn->source->disabledConns = dynConn->next;
            }
            
            if(dynConn->next){
                dynConn->next->prev = dynConn->prev;
            }
        }else{
            int index;
            int found;
            
            if(dynamicInfo->isDoor){
                dynConn->doorBlockedCount++;
            }

            if(++dynConn->blockedCount != 1){
                continue;
            }

            // First reference to the connection, so remove it from the beacon.
            
            if(dynConn->raised){
                found = beaconHasRaisedConnection(b, dynConn->conn, &index);
            }else{
                found = beaconHasGroundConnectionToBeacon(b, dynConn->conn->destBeacon, &index);
            }
            
            assert(found && conns->size && conns->storage[index] == dynConn->conn);
            
            conns->size--;
            
            CopyStructs(conns->storage + index,
                        conns->storage + index + 1,
                        conns->size - index);
            
            // Add the dynamic connection to the source beacon's list of disabled connections.
            
            dynConn->next = dynConn->source->disabledConns;
            dynConn->prev = NULL;
            dynConn->source->disabledConns = dynConn;
            
            if(dynConn->next){
                dynConn->next->prev = dynConn;
            }
        }
    }

    if(connsEnabled){
        // When enabling connections, they need to be re-sorted.
        
        for(i = 0; i < count; i++){
            BeaconDynamicConnection* dynConn = dynamicInfo->conns[i];

            beaconSortConnsByTarget(dynConn->source, dynConn->raised);
        }
    }

    beaconResetEntityDynamicData();
    
    beaconSetBlocksForRebuild();
    
    PERFINFO_AUTO_STOP();
}

void beaconDestroyDynamicConnections(BeaconDynamicInfo** dynamicInfoParam){
    BeaconDynamicInfo* dynamicInfo = *dynamicInfoParam;
    
    if(dynamicInfo){
        int i;
        
        beaconSetDynamicConnections(dynamicInfoParam, NULL, NULL, 1, 0);
        
        for(i = eaSize(&dynamicInfo->conns) - 1; i >= 0; i--){
            BeaconDynamicConnection*    dynConn = dynamicInfo->conns[i];
            S32                            index = eaFind(&dynConn->infos, dynamicInfo);
            
            assert(index >= 0 && index < eaSize(&dynConn->infos));
            
            eaRemove(&dynConn->infos, index);
            
            if(!eaSize(&dynConn->infos)){
                BeaconDynamicConnection* checkDynConn;

                // Removed last reference, so deleted the dynamic connection info.
                
                assert(!dynConn->blockedCount);

                if(!stashAddressRemovePointer(beaconConnToDynamicConnTable, dynConn->conn, &checkDynConn)){
                    // Couldn't find it, that's an error.
                    
                    assert(0);
                }
                
                assert(checkDynConn == dynConn);
                
                destroyBeaconDynamicConnection(dynConn);
            }
        }
        
        eaDestroy(&dynamicInfo->conns);
        
        destroyBeaconDynamicInfo(dynamicInfo);
        
        *dynamicInfoParam = NULL;
    }    
}

void beaconCreateAllClusterConnections(DoorEntry* doorEntries, int doorCount){
    char special_info[100];
    int i;

    beaconCheckBlocksNeedRebuild();
    
    // Create a map_id string to compare against.
    
    PERFINFO_AUTO_START("createDoorClusterConns", 1);
    
    itoa(db_state.map_id, special_info, 10);
    
    for(i = 0; i < doorCount; i++){
        DoorEntry* sourceDoor = doorEntries + i;
        int j;
        
        if(    (    sourceDoor->type != DOORTYPE_GOTOSPAWN
                ||
                sourceDoor->special_info &&
                stricmp(sourceDoor->special_info, special_info))
            &&
            sourceDoor->type != DOORTYPE_KEY)
        {
            continue;
        }
            
        for(j = 0; j < doorCount; j++){
            DoorEntry* targetDoor = doorEntries + j;
            F32 radius;
            
            if(    targetDoor->type != DOORTYPE_SPAWNLOCATION
                ||
                sourceDoor->type == DOORTYPE_GOTOSPAWN &&
                stricmp(sourceDoor->name, targetDoor->name)
                ||
                sourceDoor->type == DOORTYPE_KEY &&
                (    !sourceDoor->special_info ||
                    stricmp(sourceDoor->special_info, targetDoor->name)))
            {
                continue;
            }

            for(radius = 15; radius <= 75; radius += 15){
                int found = beaconCreateClusterConnections(NULL, sourceDoor->mat[3], radius, NULL, targetDoor->mat[3], radius);
                
                if(found){
                    break;
                }
            }
        }
    }
    
    PERFINFO_AUTO_STOP_START("createBrokenClusterConns", 1);
    
    #if 1
    if(beaconConnToDynamicConnTable)
    {
        StashTableIterator    it;
        StashElement        element;

        stashGetIterator(beaconConnToDynamicConnTable, &it);
        
        while(stashGetNextElement(&it, &element)){
            BeaconDynamicConnection* dynConn = stashElementGetPointer(element);
            
            if(dynConn->blockedCount){
                Beacon*         sourceBeacon = dynConn->source;
                Beacon*         targetBeacon = dynConn->conn->destBeacon;
                BeaconBlock*     sourceCluster = SAFE_MEMBER3(sourceBeacon, block, galaxy, cluster);
                BeaconBlock*     targetCluster = SAFE_MEMBER3(targetBeacon, block, galaxy, cluster);
                
                if(    sourceCluster &&
                    targetCluster &&
                    sourceCluster != targetCluster)
                {
                    BeaconClusterConnection* conn;

                    conn = beaconGetClusterConnection(    sourceCluster, posPoint(sourceBeacon), NULL,
                                                        targetCluster, posPoint(sourceBeacon), NULL);
                    
                    if(!conn){
                        conn = createBeaconClusterConnection();
                        
                        conn->targetCluster = targetCluster;
                        conn->dynConnToTarget = dynConn;
                        
                        copyVec3(posPoint(sourceBeacon), conn->source.pos);
                        conn->source.beacon = sourceBeacon;
                        
                        copyVec3(posPoint(targetBeacon), conn->target.pos);
                        conn->target.beacon = targetBeacon;
                        
                        arrayPushBack(&sourceCluster->gbbConns, conn);
                    }
                }
            }
        }
    }
    #endif
    
    PERFINFO_AUTO_STOP();
}

static int clusterID = 0;

int beaconNPCClusterTraverserInternal(Beacon* beacon, int* curSize, int maxSize, int cluster){
    int i;
    int noAutoConnect = 0;

    beacon->NPCClusterProcessed = 1;
    beacon->userInt = cluster;
    (*curSize)++;

    if(beacon->NPCNoAutoConnect)
        noAutoConnect = 1;

    for(i = beacon->gbConns.size - 1; i >= 0; i--)
    {
        BeaconConnection* conn = beacon->gbConns.storage[i];
        devassertmsg(conn->destBeacon != beacon, "Found an NPC beacon with a connection to itself");
        if(conn->destBeacon->userInt != cluster)
            noAutoConnect |= beaconNPCClusterTraverserInternal(conn->destBeacon, curSize, maxSize, cluster);
        if(maxSize && *curSize > maxSize)
            return noAutoConnect;
    }

    return noAutoConnect;
}

int beaconNPCClusterTraverser(Beacon* beacon, int* curSize, int maxSize){
    return beaconNPCClusterTraverserInternal(beacon, curSize, maxSize, ++clusterID);
}