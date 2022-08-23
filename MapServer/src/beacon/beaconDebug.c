
#include "beaconPrivate.h"
#include "beaconConnection.h"
#include "beaconPath.h"
#include "beaconAStar.h"
#include "beaconFile.h"
#include "comm_game.h"
#include "cmdparse/cmdserver.h"
#include "svr/svr_base.h"
#include <utilitieslib/utils/SimpleParser.h>
#include "generator/entgenCommon.h"
#include "entity/entserver.h"
#include "ai/entai.h"
#include "gameComm/sendToClient.h"
#include "entity/entity.h"
#include <utilitieslib/components/earray.h>

#define HILITE(x,y,z,color) if(0){                \
    pktSendBitsPack(pak, 2, 0);                    \
    pktSendF32(pak,x);                            \
    pktSendF32(pak,y);                            \
    pktSendF32(pak,z);                            \
    pktSendBits(pak,32,color);                    \
}

static void sendLine(Packet* pak, F32 x1, F32 y1, F32 z1, F32 x2, F32 y2, F32 z2, U32 colorARGB1, U32 colorARGB2){
    pktSendBitsPack(pak, 2, 1);
    pktSendF32(pak,x1);
    pktSendF32(pak,y1);
    pktSendF32(pak,z1);
    pktSendF32(pak,x2);
    pktSendF32(pak,y2);
    pktSendF32(pak,z2);
    pktSendBits(pak,32,colorARGB1);
    if(colorARGB2 && colorARGB1 != colorARGB2){
        pktSendBits(pak,1,1);
        pktSendBits(pak,32,colorARGB2);
    }else{
        pktSendBits(pak,1,0);
    }
}

#define SEND_LINE(x1,y1,z1,x2,y2,z2,colorARGB1){            \
    sendLine(pak, x1, y1, z1, x2, y2, z2, colorARGB1, 0);    \
}

#define SEND_LINE_PT_PT(pt1,pt2,color)        SEND_LINE(pt1[0],pt1[1],pt1[2],pt2[0],pt2[1],pt2[2],color)
#define SEND_LINE_PT_XYZ(pt,x,y,z,color)    SEND_LINE(pt[0],pt[1],pt[2],x,y,z,color)
#define SEND_LINE_XYZ_PT(x,y,z,pt,color)    SEND_LINE(x,y,z,pt[0],pt[1],pt[2],color)

#define SEND_LINE_2(x1,y1,z1,x2,y2,z2,colorARGB1,colorARGB2){        \
    sendLine(pak, x1, y1, z1, x2, y2, z2, colorARGB1, colorARGB2);    \
}

#define SEND_LINE_PT_PT_2(pt1,pt2,color,color2)        SEND_LINE_2(pt1[0],pt1[1],pt1[2],pt2[0],pt2[1],pt2[2],color,color2)
#define SEND_LINE_PT_XYZ_2(pt,x,y,z,color,color2)    SEND_LINE_2(pt[0],pt[1],pt[2],x,y,z,color,color2)
#define SEND_LINE_XYZ_PT_2(x,y,z,pt,color,color2)    SEND_LINE_2(x,y,z,pt[0],pt[1],pt[2],color,color2)

#define SEND_BEACON(beacon,color){                \
    pktSendBitsPack(pak, 2, 2);                    \
    pktSendF32(pak,posX(beacon));                \
    pktSendF32(pak,posY(beacon));                \
    pktSendF32(pak,posZ(beacon));                \
    pktSendIfSetBits(pak,32,color);                \
}

static void BeaconCmdShowConnections(int useGroundConns, Beacon* b, Beacon* parent, int level, int skip, Packet* pak){
    int i;
    int colors[] = {
        0xff00ffff,
        0xff00ff00,
        0xffffff00,
        0xffff8000,
        0xffff0000,
    };
    int color = colors[min(ARRAY_SIZE(colors) - 1, b->connectionLevel)];
    float* src = posPoint(b);
    BeaconBlock* block = b->block;
    Array* connArray;
    
    if(level <= 0 || block->isGridBlock){
        return;
    }
    
    connArray = useGroundConns ? &b->gbConns : &b->rbConns;
    
    if(skip <= 0){
        if(!b->drawnConnections){
            b->drawnConnections = 1;
            
            for(i = 0; i < connArray->size; i++){
                BeaconConnection* conn = connArray->storage[i];
                Beacon* d = conn->destBeacon;
                
                //if(d->connectionLevel == -1 || d->connectionLevel >= b->connectionLevel){
                    float* dst = posPoint(d);
                    //float y = (d->connectionLevel == b->connectionLevel && d > b) ? 1 : 0;
                    
                    if(d->connectionLevel == -1){
                        d->connectionLevel = b->connectionLevel + 1;
                    }
                    
                    SEND_LINE_2(src[0], src[1], src[2],
                                dst[0], dst[1] + 0.2, dst[2],
                                color & 0xffffff | 0x00000000,
                                color);
                //}
            }
        }
    }else{
        for(i = 0; i < connArray->size; i++){
            BeaconConnection* conn = connArray->storage[i];
            Beacon* d = conn->destBeacon;
            
            if(d->connectionLevel > b->connectionLevel){
                BeaconCmdShowConnections(useGroundConns, conn->destBeacon, b, level - 1, skip - 1, pak);
            }
        }
    }
                    
    #undef TRIPLET
}

static Packet* createBeaconDebugPacket(void){
    Packet* pak = pktCreate();

    //pak->reliable = 1;
    pktSendBitsPack(pak,1,SERVER_DEBUGCMD);
    pktSendString(pak, "ShowBeaconDebugInfo");
    
    return pak;
}

static void sendBeaconDebugPacket(Packet* pak){
    pktSend(&pak, server_state.curClient->link);
}

void beaconCmd(Entity* ent, char* params){
    int result = 1;
    int cmdEnum;
    int i;
    Beacon* originBeacon = NULL;
    Mat4 originBeaconPos;
    Beacon* secondBeacon = NULL;
    Mat4 secondBeaconPos;
    Packet*    pak;
    BeaconBlock* block;
    Vec3 pos;
    float* src;
    Array* beaconSet;
    char buffer[1000];
    Beacon tempBeacons[2];

    beginParse(params);
    
    result &= getInt(&cmdEnum);

    if(!result || cmdEnum < 0 || cmdEnum > 15){
        conPrintf(    server_state.curClient,
                    "Possible values for shownbeacon:\n"
                    "  0: NPC beacon connections.\n"
                    "  1: COMBAT beacon connections.\n"
                    "  2: TRAFFIC beacon connections.\n"
                    "  3: Show beacon block connections for combat beacons.\n"
                    "  4: Show 5 degrees of ground connectivity from source beacon.\n"
                    "  5: Show beacons that have no in or outbound connections.\n"
                    "  6: Hilite everything in my block.\n"
                    "  7: Show galaxy.\n"
                    "  8: Show galaxy connections.\n"
                    "  9: Show where the beacon created from this location would be.\n"
                    "  10: Show connection between two points, whether there's beacons there or not.\n"
                    "  11: Show all beacons near the player.\n"
                    "  12: Show all beacons on the map.\n"
                    "  13: Show all beacons in the specified cluster.\n"
                    "  14: Show 5 degrees of raised connectivity from source beacon.\n"
                    "  15: Show all cluster connections.\n");

        endParse();
        return;
    }

    beaconSet = beaconsByTypeArray[(cmdEnum >= 0 && cmdEnum < BEACONTYPE_COUNT) ? cmdEnum : BEACONTYPE_COMBAT];
    
    if(cmdEnum == 13){
        BeaconBlock* cluster;
        int clusterIndex;
        
        result &= getInt(&clusterIndex);
        
        if(!result || clusterIndex < 0 || clusterIndex >= combatBeaconClusterArray.size){
            return;
        }
        
        cluster = combatBeaconClusterArray.storage[clusterIndex];
        
        pak = createBeaconDebugPacket();

        for(i = 0; i < cluster->subBlockArray.size; i++){
            BeaconBlock* galaxy = cluster->subBlockArray.storage[i];
            int j;
            
            for(j = 0; j < galaxy->subBlockArray.size; j++){
                BeaconBlock* subBlock = galaxy->subBlockArray.storage[j];
                int k;
                
                for(k = 0; k < subBlock->beaconArray.size; k++){
                    Beacon* beacon = subBlock->beaconArray.storage[k];
                    
                    SEND_BEACON(beacon, 0xffff8800);
                }
            }
        }
        
        sendBeaconDebugPacket(pak);
        
        return;
    }

    if(cmdEnum == 15){
        // Show all the cluster connections.
        
        Packet* pak = createBeaconDebugPacket();
        S32 i;
        
        for(i = 0; i < combatBeaconClusterArray.size; i++){
            BeaconBlock* cluster = combatBeaconClusterArray.storage[i];
            S32 j;
            S32 offset_hi = 5;
            S32 offset_lo = 2;
            
            if(!cluster->gbbConns.size){
                BeaconBlock* galaxy = cluster->subBlockArray.storage[0];
                BeaconBlock* subBlock = galaxy->subBlockArray.storage[0];
                Beacon* beacon = subBlock->beaconArray.storage[0];
                
                SEND_LINE_PT_XYZ(    posPoint(beacon),
                                    posX(beacon),
                                    posY(beacon) + offset_hi,
                                    posZ(beacon),
                                    0xffff8000);
            }
            
            for(j = 0; j < cluster->gbbConns.size; j++){
                BeaconClusterConnection* conn = cluster->gbbConns.storage[j];
                S32 k;
                
                SEND_LINE_PT_PT(posPoint(conn->source.beacon),
                                conn->source.pos,
                                0xffffff00);
                
                SEND_LINE_PT_XYZ(    posPoint(conn->source.beacon),
                                    posX(conn->source.beacon),
                                    posY(conn->source.beacon) + offset_hi,
                                    posZ(conn->source.beacon),
                                    0xff00ffaa);
                
                SEND_LINE_PT_XYZ_2(    posPoint(conn->source.beacon),
                                    posX(conn->target.beacon) + 0,
                                    posY(conn->target.beacon) + offset_lo,
                                    posZ(conn->target.beacon) + 0,
                                    0xffffffff,
                                    0xffff0000);
                                    
                SEND_LINE_2(        posX(conn->target.beacon) + 0,
                                    posY(conn->target.beacon) + offset_lo,
                                    posZ(conn->target.beacon) + 0,
                                    posX(conn->target.beacon),
                                    posY(conn->target.beacon),
                                    posZ(conn->target.beacon),
                                    0xffff0000,
                                    0xffffffff);

                for(k = 0; k < cluster->gbbConns.size; k++){
                    BeaconClusterConnection* conn2 = cluster->gbbConns.storage[k];
                    S32 i;
                    
                    for(i = 0; i < k; i++){
                        BeaconClusterConnection* conn3 = cluster->gbbConns.storage[i];
                        
                        if(conn3->source.beacon == conn2->source.beacon){
                            break;
                        }
                    }
                    
                    if(i < k){
                        continue;
                    }
                        
                    if(    conn < conn2 &&
                        conn->source.beacon != conn2->source.beacon)
                    {
                        SEND_LINE_2(posX(conn->source.beacon),
                                    posY(conn->source.beacon) + offset_hi,
                                    posZ(conn->source.beacon),
                                    posX(conn2->source.beacon),
                                    posY(conn2->source.beacon) + offset_hi,
                                    posZ(conn2->source.beacon),
                                    0x0555ffcc,
                                    0x0500ffaa);

                        //SEND_LINE_2(posX(conn2->source.beacon),
                        //            posY(conn2->source.beacon) + 50,
                        //            posZ(conn2->source.beacon),
                        //            posX(conn2->source.beacon),
                        //            posY(conn2->source.beacon) + 50,
                        //            posZ(conn2->source.beacon),
                        //            0x2000ffaa,
                        //            0x2000ffaa);
                    }
                }
            }
        }
        
        sendBeaconDebugPacket(pak);
        
        return;
    }
    
    // Extract origin beacon's location.
    result &= getFloat(&originBeaconPos[3][0]);
    result &= getFloat(&originBeaconPos[3][1]);
    result &= getFloat(&originBeaconPos[3][2]);

    if(!result){
        if(!server_state.curClient || !server_state.curClient->entity){
            endParse();
            return;
        }
        
        copyVec3(ENTPOS(server_state.curClient->entity), originBeaconPos[3]);
    }
        
    if(cmdEnum == 11 || cmdEnum == 12){
        pak = createBeaconDebugPacket();

        for(i = 0; i < combatBeaconArray.size; i++){
            Beacon* beacon = combatBeaconArray.storage[i];
            if(cmdEnum == 12 || distance3Squared(posPoint(beacon), originBeaconPos[3]) < SQR(100)){
                SEND_BEACON(beacon, 0xffff8800);
            }
        }
        
        sendBeaconDebugPacket(pak);
        
        return;
    }

    // Use the beacon's location to find the beacon itself
    
    originBeaconPos[3][1] += 1;
    originBeaconPos[3][1] += 1 - beaconGetDistanceYToCollision(originBeaconPos[3], -100);
    
    if(cmdEnum == 9){
        endParse();
        
        vecY(originBeaconPos[3]) += 2 - beaconGetPointFloorDistance(originBeaconPos[3]);

        pak = createBeaconDebugPacket();
        
        SEND_LINE_PT_XYZ(    originBeaconPos[3],
                            vecX(originBeaconPos[3]), vecY(originBeaconPos[3]) + 10, vecZ(originBeaconPos[3]),
                            0xffff0000);

        SEND_LINE(    vecX(originBeaconPos[3]) + 1, vecY(originBeaconPos[3]), vecZ(originBeaconPos[3]),
                    vecX(originBeaconPos[3]) - 1, vecY(originBeaconPos[3]), vecZ(originBeaconPos[3]),
                    0xffff0000);

        SEND_LINE(    vecX(originBeaconPos[3]), vecY(originBeaconPos[3]), vecZ(originBeaconPos[3]) + 1,
                    vecX(originBeaconPos[3]), vecY(originBeaconPos[3]), vecZ(originBeaconPos[3]) - 1,
                    0xffff0000);
                    
        ZeroArray(tempBeacons);
        
        originBeacon = &tempBeacons[0];

        copyVec3(originBeaconPos[3], posPoint(originBeacon));
        
        SEND_LINE_PT_XYZ(    posPoint(originBeacon),
                            posX(originBeacon), posY(originBeacon) + beaconGetCeilingDistance(originBeacon), posZ(originBeacon),
                            0xff4444ff);

        sendBeaconDebugPacket(pak);

        return;
    }
    else if(cmdEnum == 10){
        result &= getFloat(&secondBeaconPos[3][0]);
        result &= getFloat(&secondBeaconPos[3][1]);
        result &= getFloat(&secondBeaconPos[3][2]);
        
        endParse();
        
        if(!result)
            return;
            
        ZeroArray(tempBeacons);
        
        originBeacon = &tempBeacons[0];
        secondBeacon = &tempBeacons[1];

        vecY(originBeaconPos[3]) += 2 - beaconGetPointFloorDistance(originBeaconPos[3]);
        vecY(secondBeaconPos[3]) += 2 - beaconGetPointFloorDistance(secondBeaconPos[3]);

        copyVec3(originBeaconPos[3], posPoint(originBeacon));
        copyVec3(secondBeaconPos[3], posPoint(secondBeacon));
        
        cmdEnum = 1;
    }
    
    beaconCheckBlocksNeedRebuild();

    if(!originBeacon){
        originBeacon = posFindNearest(beaconSet, originBeaconPos);

        if(!originBeacon){
            endParse();
            return;
        }
        
        conPrintf(server_state.curClient, "Origin Beacon: %1.2f, %1.2f, %1.2f\n", posParamsXYZ(originBeacon)); 
    }

    if(!secondBeacon){
        result &= getFloat(&secondBeaconPos[3][0]);
        result &= getFloat(&secondBeaconPos[3][1]);
        result &= getFloat(&secondBeaconPos[3][2]);
        
        if(result){
            secondBeaconPos[3][1] += 1;
            secondBeaconPos[3][1] += 1 - beaconGetDistanceYToCollision(secondBeaconPos[3], -100);

            secondBeacon = posFindNearest(beaconSet, secondBeaconPos);
        }
    }

    block = originBeacon->block;

    // Make sure the beacon is connected to all the correct surrounding beacons.
    
    if(!block && cmdEnum < 3){
        if(!originBeacon->madeGroundConnections){
            beaconConnectToSet(originBeacon);
        }
    }
    
    pak = createBeaconDebugPacket();

    if(secondBeacon){
        BeaconBlock* sourceGalaxy;
        BeaconBlock* targetGalaxy;
        Entity* beaconCreatePathCheckEnt();
        Entity* ent = beaconCreatePathCheckEnt();
        int good = beaconConnectsToBeaconByGround(originBeacon, secondBeacon, NULL, 5, ent, 0);
        float minHeight, maxHeight;

        SEND_LINE_PT_PT(posPoint(originBeacon), posPoint(secondBeacon), good ? 0xffaaff44 : 0xffff0000);

        SEND_LINE_PT_XYZ(    posPoint(originBeacon),
                            posX(originBeacon), posY(originBeacon) + beaconGetCeilingDistance(originBeacon), posZ(originBeacon),
                            0xff4444ff);

        SEND_LINE(    posX(originBeacon),        posY(originBeacon) + beaconGetCeilingDistance(originBeacon),        posZ(originBeacon),
                    posX(originBeacon) + 1, posY(originBeacon) + beaconGetCeilingDistance(originBeacon) + 1,    posZ(originBeacon),
                    0xff4444ff);

        SEND_LINE(    posX(originBeacon),            posY(originBeacon) + beaconGetCeilingDistance(originBeacon),        posZ(originBeacon),
                    posX(originBeacon) + 0.3,    posY(originBeacon) + beaconGetCeilingDistance(originBeacon) - 0.3,    posZ(originBeacon),
                    0xff4444ff);

        SEND_LINE_PT_XYZ(    posPoint(secondBeacon),
                            posX(secondBeacon), posY(secondBeacon) + beaconGetCeilingDistance(secondBeacon), posZ(secondBeacon),
                            0xff4444ff);

        SEND_LINE(    posX(secondBeacon),        posY(secondBeacon) + beaconGetCeilingDistance(secondBeacon),        posZ(secondBeacon),
                    posX(secondBeacon) + 1, posY(secondBeacon) + beaconGetCeilingDistance(secondBeacon) + 1,    posZ(secondBeacon),
                    0xff4444ff);

        SEND_LINE(    posX(secondBeacon),            posY(secondBeacon) + beaconGetCeilingDistance(secondBeacon),        posZ(secondBeacon),
                    posX(secondBeacon) + 0.3,    posY(secondBeacon) + beaconGetCeilingDistance(secondBeacon) - 0.3,    posZ(secondBeacon),
                    0xff4444ff);

        if(lengthVec3(ENTPOS(ent))){
            SEND_LINE(    ENTPOSX(ent), ENTPOSY(ent) + 1, ENTPOSZ(ent),
                        ENTPOSX(ent), ENTPOSY(ent) + 7, ENTPOSZ(ent),
                        good ? 0xffaaff44 : 0xffff0000);

            SEND_LINE(    ENTPOSX(ent) - 1, ENTPOSY(ent) + 5, ENTPOSZ(ent),
                        ENTPOSX(ent) + 1, ENTPOSY(ent) + 5, ENTPOSZ(ent),
                        good ? 0xffaaff44 : 0xffff0000);

            SEND_LINE(    ENTPOSX(ent), ENTPOSY(ent) + 1, ENTPOSZ(ent),
                        ENTPOSX(ent) + 1, ENTPOSY(ent), ENTPOSZ(ent),
                        good ? 0xffaaff44 : 0xffff0000);

            SEND_LINE(    ENTPOSX(ent), ENTPOSY(ent) + 1, ENTPOSZ(ent),
                        ENTPOSX(ent) - 1, ENTPOSY(ent), ENTPOSZ(ent),
                        good ? 0xffaaff44 : 0xffff0000);
        }
                
        //entFree(ent);
        
        minHeight = posY(originBeacon);
        maxHeight = FLT_MAX;
        
        while(beaconGetPassableHeight(originBeacon, secondBeacon, &maxHeight, &minHeight)){
            int color = 0xff6666ff;
            int color2 = 0xff99ff66;
            
            SEND_LINE_PT_XYZ(    posPoint(originBeacon),
                                posX(originBeacon), posY(originBeacon) + maxHeight, posZ(originBeacon),
                                color);

            SEND_LINE_PT_XYZ(    posPoint(secondBeacon),
                                posX(secondBeacon), posY(originBeacon) + maxHeight, posZ(secondBeacon),
                                color);

            SEND_LINE(    posX(originBeacon), posY(originBeacon) + 0.1 + maxHeight, posZ(originBeacon),
                        posX(secondBeacon), posY(originBeacon) + 0.1 + maxHeight, posZ(secondBeacon),
                        color2);

            SEND_LINE(    posX(originBeacon), posY(originBeacon) + 0.1 + minHeight, posZ(originBeacon),
                        posX(secondBeacon), posY(originBeacon) + 0.1 + minHeight, posZ(secondBeacon),
                        color);
                        
            
            minHeight = posY(originBeacon) + maxHeight;
            maxHeight = FLT_MAX;
        }
        
        // Draw line to show if a galaxy path exists.
        
        if(beaconGalaxyPathExists(originBeacon, secondBeacon, 0, 0)){
            SEND_LINE(    posX(originBeacon), posY(originBeacon) + 0.2, posZ(originBeacon),
                        posX(secondBeacon), posY(secondBeacon) + 0.2, posZ(secondBeacon),
                        0xff00ff77);
        }else{
            SEND_LINE(    posX(originBeacon), posY(originBeacon) + 0.2, posZ(originBeacon),
                        posX(secondBeacon), posY(secondBeacon) + 0.2, posZ(secondBeacon),
                        0xffff77ff);
        }
        
        // Draw a line to show if the beacons are in the same cluster.
        
        sourceGalaxy = originBeacon->block ? originBeacon->block->galaxy : NULL;
        targetGalaxy = secondBeacon->block ? secondBeacon->block->galaxy : NULL;
        
        if(sourceGalaxy && targetGalaxy){
            if(    sourceGalaxy->cluster &&
                sourceGalaxy->cluster == targetGalaxy->cluster)
            {
                SEND_LINE(    posX(originBeacon), posY(originBeacon) + 0.4, posZ(originBeacon),
                            posX(secondBeacon), posY(secondBeacon) + 0.4, posZ(secondBeacon),
                            0xff0077ff);
            }else{
                Vec3 pos;

                // Draw a line to the cluster connection point.
                    
                if(beaconGetNextClusterWaypoint(sourceGalaxy->cluster, targetGalaxy->cluster, pos)){
                    SEND_LINE(    posX(originBeacon), posY(originBeacon) + 0.4, posZ(originBeacon),
                                vecX(pos), vecY(pos) + 0.4, vecZ(pos),
                                0xff66ff00);
                }else{
                    SEND_LINE(    posX(originBeacon), posY(originBeacon) + 0.4, posZ(originBeacon),
                                posX(secondBeacon), posY(secondBeacon) + 0.4, posZ(secondBeacon),
                                0xffff77ff);
                }
            }
        }
                
        if(0 && sourceGalaxy && targetGalaxy && sourceGalaxy != targetGalaxy){
            BeaconBlockConnection* conn;
            
            for(i = 0; i < sourceGalaxy->gbbConns.size; i++){
                conn = sourceGalaxy->gbbConns.storage[i];
                
                if(conn->destBlock == targetGalaxy){
                    printf("There is a galaxy ground connection.\n");
                    break;
                }
            }
            
            for(i = 0; i < sourceGalaxy->rbbConns.size; i++){
                conn = sourceGalaxy->rbbConns.storage[i];
                
                if(conn->destBlock == targetGalaxy){
                    float minHeight = FLT_MAX;
                    int j;
                    
                    printf("There is a galaxy raised connection.\n");

                    for(j = 0; j < sourceGalaxy->subBlockArray.size; j++){
                        BeaconBlock* subBlock = sourceGalaxy->subBlockArray.storage[j];
                        int k;
                        
                        for(k = 0; k < subBlock->beaconArray.size; k++){
                            Beacon* b = subBlock->beaconArray.storage[k];
                            int m;
                            
                            for(m = 0; m < b->rbConns.size; m++){
                                BeaconConnection* bconn = b->rbConns.storage[m];
                                Beacon* destBeacon = bconn->destBeacon;
                                BeaconBlock* destBlock = destBeacon->block;
                                
                                if(destBlock->galaxy == targetGalaxy){
                                    BeaconBlockConnection* blockConn = NULL;
                                    int n;
                                    
                                    printf(    "  connection:\t%1.1f\t%1.1f\n",
                                            bconn->minHeight,
                                            bconn->maxHeight);
                                    
                                    for(n = 0; n < subBlock->rbbConns.size; n++){
                                        blockConn = subBlock->rbbConns.storage[n];
                                        
                                        if(blockConn->destBlock == destBlock){
                                            printf("  block conn:\t%1.1f\n", blockConn->minHeight);
                                            
                                            if(bconn->minHeight < blockConn->minHeight){
                                                printf("**********************************************************************************\n");
                                            }
                                            break;
                                        }
                                    }

                                    if(bconn->minHeight < minHeight){
                                        minHeight = bconn->minHeight;

                                        printf("*** NEW MIN *** : %1.1f\t%1.1f\n", minHeight, blockConn->minHeight);
                                    }
                                }
                            }                            
                        }                        
                    }
                    
                    printf("minHeight        = %1.1f\n", minHeight);
                    printf("stored minHeight = %1.1f\n", conn->minHeight); 
                    
                    break;
                }
            }
        }
        
        cmdEnum = -1;
    }
    
    // Draw a little marker thing on top of the source beacon.
    if(cmdEnum != 5){
        float* src = posPoint(originBeacon);
        int color = 0xffff4952;

        SEND_LINE(    src[0], src[1], src[2],
                    src[0], src[1] + 8, src[2],
                    color);

        SEND_LINE(    src[0] - 2, src[1] + 8, src[2],
                    src[0] + 2, src[1] + 8, src[2],
                    color);

        SEND_LINE(    src[0] + 2, src[1] + 8, src[2],
                    src[0] + 2, src[1] + 10, src[2],
                    color);

        SEND_LINE(    src[0] + 2, src[1] + 10, src[2],
                    src[0] - 2, src[1] + 10, src[2],
                    color);

        SEND_LINE(    src[0] - 2, src[1] + 10, src[2],
                    src[0] - 2, src[1] + 8, src[2],
                    color);

        // H
        SEND_LINE(    src[0] - 1.8, src[1] + 8.2, src[2],
                    src[0] - 1.8, src[1] + 9.8, src[2],
                    color);

        SEND_LINE(    src[0] - 1.8, src[1] + 9, src[2],
                    src[0] - 0.2, src[1] + 9, src[2],
                    color);

        SEND_LINE(    src[0] - 0.2, src[1] + 8.2, src[2],
                    src[0] - 0.2, src[1] + 9.8, src[2],
                    color);

        // I
        SEND_LINE(    src[0] + 0.2, src[1] + 9.8, src[2],
                    src[0] + 1.8, src[1] + 9.8, src[2],
                            color);

        SEND_LINE(    src[0] + 1, src[1] + 8.2, src[2],
                    src[0] + 1, src[1] + 9.8, src[2],
                    color);

        SEND_LINE(    src[0] + 0.2, src[1] + 8.2, src[2],
                    src[0] + 1.8, src[1] + 8.2, src[2],
                    color);
    }
    
    if(block){
        sprintf(buffer, "blocked: %d", originBeacon->pathsBlockedToMe);
        aiEntSendAddText(originBeacon->mat[3], buffer, 0);
    
        // Show all beacons in the block
        if(cmdEnum == 6){
            for(i = 0; i < block->beaconArray.size; i++){
                Beacon* beacon = block->beaconArray.storage[i];
                float* source = beacon->mat[3];
                
                // Highlight the beacon orangeish.

                HILITE(source[0], source[1], source[2], 0xffff8000);
            }
            
            cmdEnum = -1;
        }
        
        // Show connections to other blocks.
        
        if(cmdEnum == 3){
            for(i = 0; i < block->gbbConns.size; i++){
                BeaconBlockConnection* blockConn = block->gbbConns.storage[i];
                float* src = block->pos;
                float* pos = blockConn->destBlock->pos;
                int color = 0xc0ffff00;
                int j;

                SEND_LINE(    src[0], src[1], src[2],
                            pos[0], pos[1], pos[2],
                            color);
                
                for(j = 0; j < blockConn->destBlock->beaconArray.size; j++){
                    Beacon* beacon = blockConn->destBlock->beaconArray.storage[j];
                    float* dst = beacon->mat[3];
                    int color = 0x40ffff00;
                    
                    SEND_LINE(    pos[0], pos[1], pos[2],
                                dst[0], dst[1], dst[2],
                                color);
                }
                
                // Show beacons that connect to connected blocks.  Holy crap this is out of hand.
                
                for(j = 0; j < block->beaconArray.size; j++){
                    Beacon* b = block->beaconArray.storage[j];
                    float* src = b->mat[3];
                    int k;
                    
                    for(k = 0; k < b->gbConns.size; k++){
                        BeaconConnection* beaconConn = b->gbConns.storage[k];
                        
                        if(beaconConn->destBeacon->block == blockConn->destBlock){
                            float* dst = beaconConn->destBeacon->mat[3];
                            int color = 0x80ff4080;
                            
                            SEND_LINE(    src[0], src[1], src[2],
                                        dst[0], dst[1], dst[2],
                                        color);

                            break;
                        }
                    }

                    for(k = 0; k < b->rbConns.size; k++){
                        BeaconConnection* beaconConn = b->rbConns.storage[k];
                        
                        if(beaconConn->destBeacon->block == blockConn->destBlock){
                            float* dst = beaconConn->destBeacon->mat[3];
                            int color = 0x8040ff80;
                            
                            SEND_LINE(    src[0], src[1] + 1, src[2],
                                        dst[0], dst[1] + 1, dst[2],
                                        color);
                            
                            break;
                        }
                    }
                }
            }

            for(i = 0; i < block->rbbConns.size; i++){
                BeaconBlockConnection* conn = block->rbbConns.storage[i];
                float* src = block->pos;
                float* pos = conn->destBlock->pos;
                int color = 0xc000ffff;
                int j;

                SEND_LINE(    src[0], src[1], src[2],
                            src[0], src[1] + conn->minHeight, src[2],
                            color);

                SEND_LINE(    src[0], src[1] + conn->minHeight, src[2],
                            pos[0], src[1] + conn->minHeight, pos[2],
                            color);

                SEND_LINE(    pos[0], src[1] + conn->minHeight, pos[2],
                            pos[0], pos[1], pos[2],
                            color);
                
                for(j = 0; j < conn->destBlock->beaconArray.size; j++){
                    Beacon* beacon = conn->destBlock->beaconArray.storage[j];
                    float* dst = beacon->mat[3];
                    int color = 0x4000ffff;
                    
                    SEND_LINE(    pos[0], pos[1], pos[2],
                                dst[0], dst[1], dst[2],
                                color);
                }
            }
        }
    }
    
    if(cmdEnum == 0){
        src = originBeacon->mat[3];
        
        beaconGetGroundConnections(originBeacon, beaconSet);
        
        for(i = 0; i < originBeacon->gbConns.size; i++){
            BeaconConnection* conn = originBeacon->gbConns.storage[i];
            float* pos = conn->destBeacon->mat[3];
            int color = (1 || conn->destBeacon->block == block) ? 0xc0ff8000 : 0xc000ff00;

            // Highlight the beacon.
            
            HILITE(pos[0], pos[1], pos[2], color);
            
            // Draw a line to the beacon.
            
            SEND_LINE(    src[0], src[1], src[2],
                        pos[0], pos[1], pos[2],
                        color);
        }
    }
    
    if(cmdEnum == 1 || cmdEnum == 2){
        src = originBeacon->mat[3];
        
        if(cmdEnum == 1){
            int i;
            for(i = 0; i < combatBeaconArray.size; i++){
                Beacon* beacon = combatBeaconArray.storage[i];
                if(distance3Squared(posPoint(beacon), src) < SQR(100)){
                    SEND_BEACON(beacon, 0);
                }
            }
        }
        
        // Draw the PYR for traffic beacons.
        
        if(cmdEnum == 2){
            int j;
            Mat3 mat;
            
            createMat3YPR(mat, originBeacon->pyr);

            SEND_LINE(    src[0], src[1] + 3, src[2],
                        src[0] + 10 * mat[2][0], src[1] + 3 + 10 * mat[2][1], src[2] + 10 * mat[2][2],
                        0xc0ffeecc );
                                    
            for(j = 0; j < originBeacon->gbConns.size; j++){
                BeaconCurvePoint* curve = originBeacon->curveArray.storage[j];
                int k;
                
                if(!curve)
                    continue;
                
                for(k = 0; k < BEACON_CURVE_POINT_COUNT - 1; k++){
                    float* src = curve[k].pos;
                    float* dst = curve[k+1].pos;
                    
                    SEND_LINE(    src[0], src[1], src[2],
                                dst[0], dst[1], dst[2],
                                0xc09977ff );
                }
            }
        }
        
        for(i = 0; i < originBeacon->gbConns.size; i++){
            BeaconConnection* conn = originBeacon->gbConns.storage[i];
            float* pos = conn->destBeacon->mat[3];
            int color = (1 || conn->destBeacon->block == block) ? 0xc0ff8000 : 0xc000ff00;

            // Highlight the beacon.

            HILITE(pos[0], pos[1], pos[2], color);
            
            // Draw a line to the beacon.

            SEND_LINE(    src[0], src[1], src[2],
                        pos[0], pos[1], pos[2],
                        color );

            if(conn->timeWhenIWasBad){
                sprintf(buffer, "blocked: %d\nsince bad: %1.2f", conn->destBeacon->pathsBlockedToMe, (ABS_TIME - conn->timeWhenIWasBad) / 3000.0);
            }else{
                sprintf(buffer, "blocked: %d", conn->destBeacon->pathsBlockedToMe);
            }
            aiEntSendAddText(conn->destBeacon->mat[3], buffer, 0);
        }
                    
        for(i = 0; i < originBeacon->rbConns.size; i++){
            BeaconConnection* conn = originBeacon->rbConns.storage[i];
            float* pos = conn->destBeacon->mat[3];
            int color = (1 || conn->destBeacon->block == block) ? 0xc00080ff : 0xc0ff00ff;

            assert(conn->minHeight > 0);            
            
            // Highlight the beacon.
            
            HILITE(pos[0], pos[1], pos[2], color);
            
            // Draw a line showing the low connection heights.
            
            SEND_LINE(    src[0], src[1], src[2],
                        src[0], src[1] + conn->minHeight, src[2],
                        color);

            SEND_LINE(    src[0], src[1] + conn->minHeight, src[2],
                        pos[0], src[1] + conn->minHeight, pos[2],
                        color);

            SEND_LINE(    pos[0], src[1] + conn->minHeight, pos[2],
                        pos[0], pos[1], pos[2],
                        color);

            // Draw a line showing the high connection heights.
            
            SEND_LINE(    src[0], src[1] + conn->minHeight, src[2],
                        src[0], src[1] + conn->maxHeight, src[2],
                        color);

            SEND_LINE(    pos[0], src[1] + conn->maxHeight, pos[2],
                        pos[0], src[1] + conn->minHeight, pos[2],
                        color);

            ((unsigned char*)&color)[1] = 0xff;
            ((unsigned char*)&color)[0] = 0x00;

            SEND_LINE(    src[0], src[1] + conn->maxHeight, src[2],
                        pos[0], src[1] + conn->maxHeight, pos[2],
                        color);
        }
    }
    
    if(cmdEnum == 4 || cmdEnum == 14){
        void clearBeaconConnectionLevel();
        
        clearBeaconConnectionLevel();
        
        originBeacon->connectionLevel = 0;
        
        for(i = 0; i < 5; i++){
            BeaconCmdShowConnections(cmdEnum == 4, originBeacon, NULL, 5, i, pak);
        }
    }
    
    if(cmdEnum == 5){
        // Show beacons that have no outbound ground connections (this is fast).
        
        for(i = 0; i < combatBeaconArray.size; i++){
            Beacon* beacon = combatBeaconArray.storage[i];
            
            if(!beacon->block->isGridBlock && !beacon->gbConns.size){
                float* src = posPoint(beacon);
                int color = 0xffff6688;
                
                SEND_LINE(    src[0], src[1], src[2],
                            src[0], src[1] + 1000, src[2],
                            color);
            }
        }

        // Show beacons that have no inbound ground connections (this is slow).
        
        for(i = 0; i < combatBeaconArray.size; i++){
            Beacon* beacon = combatBeaconArray.storage[i];
            int j;
            int found = 0;
            
            if(beacon->block->isGridBlock){
                continue;
            }
            
            // This is extremeley lazy, should be nearby blocks, not all beacons.
            
            for(j = 0; !found && j < combatBeaconArray.size; j++){
                Beacon* beacon2 = combatBeaconArray.storage[j];
                int k;
                
                if(i == j || beacon2->block->isGridBlock)
                    continue;
                
                for(k = 0; k < beacon2->gbConns.size; k++){
                    BeaconConnection* conn = beacon2->gbConns.storage[k];
                    
                    if(conn->destBeacon == beacon){
                        found = 1;
                        break;
                    }
                }
            }
            
            if(!found){
                float* src = posPoint(beacon);
                int color = 0xff6688ff;
                
                SEND_LINE(    src[0] + 0.2, src[1], src[2],
                            src[0] + 0.2, src[1] + 1000, src[2],
                            color);
            }
        }
    }

    if(cmdEnum == 7){
        if(originBeacon->block && originBeacon->block->galaxy){
            BeaconBlock* galaxy = originBeacon->block->galaxy;
            int j;
            
            src = posPoint(originBeacon);
            
            for(i = 0; i < galaxy->subBlockArray.size; i++){
                BeaconBlock* subBlock = galaxy->subBlockArray.storage[i];
                
                for(j = 0; j < subBlock->beaconArray.size; j++){
                    Beacon* b = subBlock->beaconArray.storage[j];
                    
                    SEND_LINE(    src[0], src[1], src[2],
                                posX(b), posY(b) + 0.1, posZ(b),
                                0xff00ff00);
                }                            
            }
        }
    }

    if(cmdEnum == 8){
        if(originBeacon->block && originBeacon->block->galaxy){
            BeaconBlock* galaxy = originBeacon->block->galaxy;

            src = posPoint(originBeacon);

            for(i = 0; i < galaxy->gbbConns.size; i++){
                BeaconBlockConnection* conn = galaxy->gbbConns.storage[i];
                BeaconBlock* destGalaxy = conn->destBlock;
                BeaconBlock* destBlock = destGalaxy->subBlockArray.storage[0];
                
                if(destBlock->beaconArray.size){
                    Beacon* b = destBlock->beaconArray.storage[0];
                    
                    SEND_LINE(    src[0], src[1], src[2],
                                posX(b), posY(b), posZ(b),
                                0xffff4444);
                }
            }

            for(i = 0; i < galaxy->rbbConns.size; i++){
                BeaconBlockConnection* conn = galaxy->rbbConns.storage[i];
                BeaconBlock* destGalaxy = conn->destBlock;
                BeaconBlock* destBlock = destGalaxy->subBlockArray.storage[0];
                
                if(destBlock->beaconArray.size){
                    Beacon* b = destBlock->beaconArray.storage[0];
                    
                    SEND_LINE(    src[0], src[1], src[2],
                                posX(b), posY(b), posZ(b),
                                0xffff44ff);
                }
            }
        }
    }
    
    copyVec3(originBeacon->mat[3], pos);
    
    beaconMakeGridBlockCoords(pos);
    
    scaleVec3(pos, combatBeaconGridBlockSize, pos);
    
    if(cmdEnum == 3){
        int i,j,k;
        Vec3 pt1, pt2;
        
        for(i = 0; i < 3; i++){
            int v = (i+1)%3;
            int w = (i+2)%3;
            
            pt1[i] = pos[i];
            pt2[i] = pos[i] + combatBeaconGridBlockSize;
            
            for(j = 0; j < 2; j++){
                for(k = 0; k < 2; k++){
                    pt1[v] = pt2[v] = pos[v] + j * combatBeaconGridBlockSize;
                    pt1[w] = pt2[w] = pos[w] + k * combatBeaconGridBlockSize;

                    SEND_LINE(    pt1[0], pt1[1], pt1[2],
                                pt2[0], pt2[1], pt2[2],
                                0xc0ff8040);
                }
            }
        }
    }
    
    sendBeaconDebugPacket(pak);
}

void beaconPrintDebugInfo(){
    U32 getAllocatedBeaconConnectionCount(void);
    
    char blockMemoryInfo[1000];
    int subBlockCount = 0;
    int beaconGroundConnCount = 0;
    int beaconRaisedConnCount = 0;
    int blockConnCount = 0;
    int i;
    int galaxyConnCount = 0;
    int clusterConnCount = 0;
    int npcConnCount = 0;
    int trafficConnCount = 0;
    
    // Count NPC beacon conns.
    
    for(i = 0; i < basicBeaconArray.size; i++){
        Beacon* beacon = basicBeaconArray.storage[i];
        npcConnCount += beacon->gbConns.size;
    }
    
    // Count traffic beacon conns.

    for(i = 0; i < trafficBeaconArray.size; i++){
        Beacon* beacon = trafficBeaconArray.storage[i];
        trafficConnCount += beacon->gbConns.size;
    }

    // Count combat beacon conns.
    
    for(i = 0; i < combatBeaconArray.size; i++){
        Beacon* beacon = combatBeaconArray.storage[i];
        beaconGroundConnCount += beacon->gbConns.size;
        beaconRaisedConnCount += beacon->rbConns.size;
    }

    for(i = 0; i < combatBeaconGridBlockArray.size; i++){
        BeaconBlock* gridBlock = combatBeaconGridBlockArray.storage[i];
        int j;

        subBlockCount += gridBlock->subBlockArray.size;

        //printf("sub-blocks: %d\n", gridBlock->subBlockArray.size);

        for(j = 0; j < gridBlock->subBlockArray.size; j++){
            BeaconBlock* subBlock = gridBlock->subBlockArray.storage[j];

            blockConnCount += subBlock->gbbConns.size + subBlock->rbbConns.size;

            //printf(    "  pos: (%f, %f, %f), beacons: %d, conns: %d+%d\n",
            //        vecParamsXYZ(subBlock->pos),
            //        subBlock->beaconArray.size,
            //        subBlock->gbbConns.size,
            //        subBlock->rbbConns.size);
        }
    }
    
    for(i = 0; i < combatBeaconGalaxyArray[0].size; i++){
        BeaconBlock* galaxy = combatBeaconGalaxyArray[0].storage[i];
        
        galaxyConnCount += galaxy->gbbConns.size + galaxy->rbbConns.size;
    }

    for(i = 0; i < combatBeaconClusterArray.size; i++){
        BeaconBlock* cluster = combatBeaconClusterArray.storage[i];
        
        clusterConnCount += cluster->gbbConns.size;
    }
    
    {
        void beaconGetBlockMemoryData(char* outString);

        beaconGetBlockMemoryData(blockMemoryInfo);
    }

    printf(    "\n"
            "\n"
            "\n"
            "----------------------------------------------------------------\n"
            "-- Utterly Important Beacon Info -------------------------------\n"
            "----------------------------------------------------------------\n"
            "  File Version: %d\n"
            "  NPC Beacons:  %d + %d conns\n"
            "  Traffic Bcns: %d + %d conns\n"
            "  Beacons:      %d\n"
            "  Beacon Conns: %d + %d = %d\n"
            "  Total Conns:  %d\n"
            "  Grid Blocks:  %d (HT size: %d)\n"
            "  Sub Blocks:   %d\n"
            "  Sub Conns:    %d\n"
            "  Galaxies:     %d (%d conns)\n"
            "  Clusters:     %d (%d conns)\n"
            "  ChunkMemory:  %s\n",
            beaconGetReadFileVersion(),
            basicBeaconArray.size,
            npcConnCount,
            trafficBeaconArray.size,
            trafficConnCount,
            combatBeaconArray.size,
            beaconGroundConnCount,
            beaconRaisedConnCount,
            beaconGroundConnCount + beaconRaisedConnCount,
            getAllocatedBeaconConnectionCount(),
            combatBeaconGridBlockArray.size,
            stashGetMaxSize(combatBeaconGridBlockTable),
            subBlockCount,
            blockConnCount,
            combatBeaconGalaxyArray[0].size,
            galaxyConnCount,
            combatBeaconClusterArray.size,
            clusterConnCount,
            blockMemoryInfo);
            
    printf("\nCluster Galaxies (Blocks+Beacons): ");
    
    for(i = 0; i < combatBeaconClusterArray.size; i++){
        BeaconBlock* cluster = combatBeaconClusterArray.storage[i];
        int j;
        int beaconCount = 0;
        int blockCount = 0;
        
        for(j = 0; j < cluster->subBlockArray.size; j++){
            BeaconBlock* galaxy = cluster->subBlockArray.storage[j];
            int k;
            
            blockCount += galaxy->subBlockArray.size;
            
            for(k = 0; k < galaxy->subBlockArray.size; k++){
                BeaconBlock* subBlock = galaxy->subBlockArray.storage[k];
                
                beaconCount += subBlock->beaconArray.size;
            }            
        }
        
        printf(    "%d(%d+%d),",
                cluster->subBlockArray.size,
                blockCount,
                beaconCount);
    }
    
    printf(    "\n"
            "\n"
            "----------------------------------------------------------------\n"
            "\n"
            "\n");
}

void beaconGotoCluster(ClientLink* client, int index){
    if(!client || !client->entity)
        return;
    
    if(index >= 0 && index < combatBeaconClusterArray.size){
        BeaconBlock* cluster = combatBeaconClusterArray.storage[index];
        
        if(cluster && cluster->subBlockArray.size){
            BeaconBlock* galaxy = cluster->subBlockArray.storage[0];
            
            if(galaxy && galaxy->subBlockArray.size){
                BeaconBlock* subBlock = galaxy->subBlockArray.storage[0];
                
                if(subBlock && subBlock->beaconArray.size){
                    Beacon* beacon = subBlock->beaconArray.storage[0];
                    
                    entUpdatePosInstantaneous(client->entity, posPoint(beacon));
                    
                    conPrintf(client, "Sending to cluster %d/%d (%d galaxies)", index, combatBeaconClusterArray.size, cluster->subBlockArray.size);
                }
            }
        }
    }else{
        index -= combatBeaconClusterArray.size;

        if(index >= 0 && index < combatBeaconGalaxyArray[0].size){
            BeaconBlock* galaxy = combatBeaconGalaxyArray[0].storage[index];
            
            if(galaxy && galaxy->subBlockArray.size){
                BeaconBlock* subBlock = galaxy->subBlockArray.storage[0];
                
                if(subBlock && subBlock->beaconArray.size){
                    Beacon* beacon = subBlock->beaconArray.storage[0];
                    
                    entUpdatePosInstantaneous(client->entity, posPoint(beacon));

                    conPrintf(client, "Sending to galaxy %d/%d (%d blocks)", index, combatBeaconGalaxyArray[0].size, galaxy->subBlockArray.size);
                }
            }
        }
    }
}

static StashTable varTable;

int beaconGetDebugVar(char* var){
    int iResult;
    if (varTable && stashFindInt(varTable, var, &iResult))
        return iResult;
        
    return 0;
}

int beaconSetDebugVar(char* var, int value){
    StashElement element;
    
    if(!varTable){
        varTable = stashTableCreateWithStringKeys(100, StashDeepCopyKeys);
    }
    
    if(stashFindElement(varTable, var, &element)){
        stashElementSetInt(element, value);
    }else{
        stashAddInt(varTable, var, value, false);
    }
    
    return value;
}

typedef struct NPCBeaconDebugCluster{
    Beacon* beacon;
    Vec3 pos;
    int size;
    int clusterNum;
}NPCBeaconDebugCluster;

static NPCBeaconDebugCluster** debugCluster= NULL;

void beaconMakeNPCClusters(){
    int i;

    eaClearEx(&debugCluster, NULL);
    eaClear(&debugCluster);

    for(i = basicBeaconArray.size - 1; i >= 0; i--){
        Beacon* b = basicBeaconArray.storage[i];
        b->NPCClusterProcessed = 0;
        b->userInt = 0;
    }

    for(i = basicBeaconArray.size - 1; i >= 0; i--){
        Beacon* b = basicBeaconArray.storage[i];

        if(!b->NPCClusterProcessed && !b->isEmbedded){
            int clusterCount = 0;
            NPCBeaconDebugCluster* newCluster = (NPCBeaconDebugCluster*)calloc(1, sizeof(NPCBeaconDebugCluster));

            devassert(!b->userInt);
            newCluster->beacon = b;
            copyVec3(posPoint(b), newCluster->pos);
            beaconNPCClusterTraverser(b, &newCluster->size, 0);
            newCluster->clusterNum = b->userInt;
            eaPush(&debugCluster, newCluster);
        }
    }

    // Do extra processing to clean up one way connections (that can be made by NPCs who got stuck)
    for(i = eaSize(&debugCluster)-1; i >= 0; i--){
        if(debugCluster[i]->beacon->userInt != debugCluster[i]->clusterNum){
            free(eaRemove(&debugCluster, i));
            i++;
        }
    }

}

int beaconGotoNPCCluster(ClientLink* client, int num){
    if(!debugCluster || num >= eaSize(&debugCluster))
    {
        int totalBeacons = 0;
        int i;

        beaconMakeNPCClusters();
        num = 0;

        for(i = eaSize(&debugCluster)-1; i >= 0; i--){
            totalBeacons += debugCluster[i]->size;
        }
        conPrintf(client, "(Re)built NPC cluster array (found %d clusters totaling %d beacons",
            eaSize(&debugCluster), totalBeacons);
    }

    conPrintf(client, "Cluster %d, %d beacons", num, debugCluster[num]->size);
    entUpdatePosInstantaneous(client->entity, debugCluster[num]->pos);
    return num + 1;
}