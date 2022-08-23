
#include "entaiprivate.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "gridcoll/megaGrid.h"
#include "cmdparse/cmdcommon.h"
#include "cmdparse/cmdserver.h"
#include <utilitieslib/utils/mathutil.h>
#include "utils/position.h"
#include "ai/entai.h"
#include "ai/entaiLog.h"

void aiPlayer(Entity* e, AIVars* ai){
//    AIProxEntStatus* status;
    Vec3 pos;
    
    // Set these things, just in case something else messes up.
    
    SET_ENTPAUSED(e) = 0;
    SET_ENTSLEEPING(e) = 0;
    
    // Update avoid state.
    
    aiUpdateAvoid(e, ai);    

    // Do proximity checks.

    copyVec3(ENTPOS(e), pos);

    if(ABS_TIME_SINCE(ai->time.checkedProximity) >= SEC_TO_ABS(1)){
        int entArray[MAX_ENTITIES_PRIVATE];
        int entCount;
        int i;
        int idxSelf = e->owner;

        ai->time.checkedProximity = ABS_TIME;

        // Wake up guys that are near me.
        
        entCount = mgGetNodesInRange(0, ENTPOS(e), (void**)entArray, 0);

        //count = gridFindObjectsInSphere(&ent_grid, entArray, ARRAY_SIZE(entArray), posPoint(e), 0);
        
        for(i = 0; i < entCount; i++){
            int proxID = entArray[i];
            EntType entType = ENTTYPE_BY_ID(proxID);
            Entity* proxEnt;
            AIVars* aiProx;
            
            if(entType == ENTTYPE_PLAYER)
                continue;

            proxEnt = entities[proxID];
            aiProx = ENTAI_BY_ID(proxID);

            // Wake up nearby guys.
            
            if(ENTSLEEPING_BY_ID(proxID) || (aiProx && aiProx->timerTryingToSleep)){
                F32 distSQR = distance3Squared(ENTPOS_BY_ID(proxID), pos);
                F32 maxDistSQR = SQR(ENTFADE_BY_ID(proxID));

                if(distSQR < maxDistSQR){
                    AI_LOG(    AI_LOG_BASIC,
                            (proxEnt,
                            "Woken up by: %s^0\n", e->name));

                    SET_ENTSLEEPING_BY_ID(proxID) = 0;

                    if(aiProx){
                        aiProx->timerTryingToSleep = 0;
                    }
                }
            }

            switch(entType){
                case ENTTYPE_CRITTER:{
                    // Update timeslice for awake critters.
                    
                    if(!ENTSLEEPING_BY_ID(proxID)){
                        F32 distSQR = distance3Squared(ENTPOS(proxEnt), pos);
                        F32 maxDistSQR = SQR(ENTFADE_BY_ID(proxID));

                        if(distSQR < maxDistSQR){
                            float oldDistance = 400 - ai->timeslice * 4;
                            
                            if(distSQR < SQR(oldDistance)){
                                aiSetTimeslice(proxEnt, (400 - sqrt(distSQR) + 3) / 4);
                            }
                        }
                    }
                    break;
                }
                
                //case ENTTYPE_DOOR:
                //{
                //    // Put doors in proximity table.
                //
                //    F32 distSQR = distance3Squared(posPoint(proxEnt), pos);

                //    if(distSQR < SQR(100)){
                //        aiGetProxEntStatus(ai->proxEntStatusTable, proxEnt, 1);
                //    }
                //    break;
                //}
            }
        }
    }
    
//    if(0){
//        status = ai->proxEntStatusTable->statusHead;
//        
//        while(status){
//            AIProxEntStatus* next = status->tableList.next;
//            Entity* proxEnt = status->entity;
//            AIVars* aiProx;
//            
//            if(proxEnt){
//                EntType entType = ENTTYPE(proxEnt);
//                if(entType == ENTTYPE_DOOR){
//                    float distSQR = distance3Squared(ENTPOS(proxEnt), pos);
//                    
//                    if(distSQR < SQR(15)){
//                        aiProx = ENTAI(proxEnt);
//                        
//                        //if(aiProx->door.open){
//                        //    // Set the open time if it's already open.
//
//                        //    //aiProx->door.timeOpened = ABS_TIME;
//                        //}
//                        //else 
//                        if(aiProx->door.autoOpen){
//                            // Open the door if it's an auto door.
//                            
//                            aiDoorSetOpen(proxEnt, 1);
//                        }
//                    }
//                    else if(distSQR > SQR(100)){
//                        // Remove from proximity table if more than 100 feet away.
//                    
//                        aiRemoveProxEntStatus(ai->proxEntStatusTable, proxEnt, 0, 1);
//                    }
//                }
//            }
//            
//            status = next;
//        }
//    }
}