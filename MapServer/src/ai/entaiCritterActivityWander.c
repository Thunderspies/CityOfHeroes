
#include "entaiprivate.h"
#include "entaiCritterPrivate.h"
#include "beacon/beaconPath.h"
#include "beacon/beaconPrivate.h"
#include "entity/entity.h"
#include "cmdparse/cmdcommon.h"
#include "cmdparse/cmdserver.h"
#include "ai/entaiLog.h"
#include "ai/entai.h"
#include "ai/entaiScript.h"
#include "ai/entaiBehaviorCoh.h"

// static Entity* simplePathEnt;
// static int simplePathMaxDistanceSQR;
// static Vec3 simplePathMaxDistanceAnchorPos;
// 
// static int beaconScoreWander(Beacon* src, BeaconConnection* conn){
//     float* pos;
// 
//     if(conn){
//         pos = posPoint(conn->destBeacon);
//     }else{
//         pos = posPoint(src);
//     }
// 
//     if(simplePathMaxDistanceSQR){
//         float distanceSQR = distance3Squared(simplePathMaxDistanceAnchorPos, pos);
//         if(distanceSQR > simplePathMaxDistanceSQR){
//             return 0;
//         }
//     }
//     
//     if(distance3Squared(pos, ENTPOS(simplePathEnt)) > SQR(200)){
//         return BEACONSCORE_FINISHED;
//     }
//     
//     return rand();
// }
// 
// 
// 
// 
// void aiCritterActivityWander(Entity* e, AIVars* ai, F32 radius){
//     // If the wandering timer has expired, then see if I should wander again.
//     
//     if(    !ai->navSearch->path.head && 
//         ABS_TIME_SINCE(ai->time.foundPath) > SEC_TO_ABS(5)) 
//     {
//         simplePathEnt = e;
//         simplePathMaxDistanceSQR = SQR(radius);
//         copyVec3(ai->spawn.pos, simplePathMaxDistanceAnchorPos);
//         
//         aiConstructSimplePath(e, beaconScoreWander);
//         
//         if(!ai->navSearch->path.head && simplePathMaxDistanceSQR){
//             // Couldn't make a simple path, so go back to source position if possible.
//             aiSetFollowTargetPosition(e, ai, ai->spawn.pos, 0);
//             aiConstructPathToTarget(e);
//         }
//         
//         // Check if both types of path failed.
//         
//         if(!ai->navSearch->path.head){
//             ai->time.foundPath = ABS_TIME;
// 
//             aiSetActivity(e, ai, AI_ACTIVITY_STAND_STILL);
//             aiSetMessageHandler(e, aiCritterMsgHandlerDefault);
//         }else{
//             ai->time.wander.begin = ABS_TIME;
//             
//             aiStartPath(e, 1, 1);
//             ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 10;
//         }
//     }
// 
//     if(!ai->navSearch->path.head || ABS_TIME_SINCE(ai->time.wander.begin) > SEC_TO_ABS(15)){
//         // If the end of the path has been reached, then see what the entity wants to do.
//         
//         AI_LOG(    AI_LOG_PATH,
//                 (e,
//                 "Wandering path finished.\n"));
// 
//         aiDiscardFollowTarget(e, "Finished wandering.", false);
//     }
// }
// 
// void* aiCritterActivityWanderMessageHandler(Entity* e, AIMessage msg, AIMessageParams params){
//     AIVars* ai = ENTAI(e);
// 
//     switch(msg){
//         case AI_MSG_PATH_BLOCKED:{
//             if(ai->preventShortcut){
//                 if(ai->attackTarget.entity){
//                     // If I'm stuck and short-circuiting is prevented, then just hang out.
//                     
//                     AI_LOG(    AI_LOG_PATH,
//                             (e,
//                             "Hanging out until my target moves.\n"));
//                 
//                     aiDiscardFollowTarget(e, "Waiting for target to move.", false);
//                     
//                     ai->waitingForTargetToMove = 1;
//                 }else{
//                     // The target isn't a player, so I'm probably just wandering around.
//                     //   I'll just find a new path eventually.
//                     
//                     AI_LOG(    AI_LOG_PATH,
//                             (e,
//                             "Got stuck while not targeting a player.\n"));
//                     
//                     aiDiscardFollowTarget(e, "Ran into something, not sure what.", false);
// 
//                     ai->waitingForTargetToMove = 0;
//                 }
//             }else{
//                 // If I'm stuck and I have already tried short-circuiting,
//                 //   then redo the path but don't allow short-circuiting.
//                 
//                 AI_LOG(    AI_LOG_PATH,
//                         (e,
//                         "I got stuck, trying not short-circuiting.\n"));
//                 
//                 aiConstructPathToTarget(e);
//                 
//                 aiStartPath(e, 1, ai->attackTarget.entity ? 0 : 1);
// 
//                 ai->waitingForTargetToMove = 1;
//             }
// 
//             break;
//         }
// 
//         case AI_MSG_ATTACK_TARGET_MOVED:{
//             // The target has moved, so reset the find path timer.
//             
//             AI_LOG(    AI_LOG_PATH,
//                     (e,
//                     "Attack target moved a lot, cancelling path.\n"));
//             
//             clearNavSearch(ai->navSearch);
//             
//             ai->waitingForTargetToMove = 0;
// 
//             break;
//         }
// 
//         case AI_MSG_FOLLOW_TARGET_REACHED:{
//             aiDiscardFollowTarget(e, "Reached follow target.", true);
//             
//             ai->time.checkedForShortcut = 0;
// 
//             break;
//         }
//     }
//     
//     return 0;
// }


// Agents following a leader will attempt to be at least this close to the leader at all times
// #define AI_FOLLOW_LEADER_DISTANCE (10)
// 
// void aiCritterActivityFollowLeader(Entity* e, AIVars* ai){
// 
//     // if i have a leader, and i am not already following him, start following!
// 
//     Entity * leader = aiTeamGetLeader(ai);
//     if(leader)
//     {
//         float distSQR = distance3Squared(ENTPOS(e), ENTPOS(leader));
// 
//         ai->doNotRun = ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : (distSQR < SQR(30)) ? TRUE : FALSE;
// 
//         if(    ai->followTarget.type != AI_FOLLOWTARGET_ENTITY ||
//             ai->followTarget.entity != leader ||
//             ai->motion.type == AI_MOTIONTYPE_NONE)
//         {    
//             if(distSQR > SQR(AI_FOLLOW_LEADER_DISTANCE))
//             {
//                 ai->time.foundPath = ABS_TIME;
// 
//                 ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : (AI_FOLLOW_LEADER_DISTANCE + 1);    
// 
//                 aiDiscardFollowTarget(e, "Began following leader.", false);
// 
//                 aiSetFollowTargetEntity(e, ai, leader, AI_FOLLOWTARGETREASON_LEADER);
// 
//                 aiGetProxEntStatus(ai->proxEntStatusTable, leader, 1);
//                 
//                 aiConstructPathToTarget(e);
// 
//                 aiStartPath(e, 0, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);
//             }
//         }
//     }
// }
// 
// 
// 
// 
// 
// void* aiCritterMsgHandlerFollowLeader(Entity* e, AIMessage msg, AIMessageParams params){
//     AIVars* ai = ENTAI(e);
// 
//     switch(msg){
//         case AI_MSG_PATH_BLOCKED:{
//             if(ai->preventShortcut){
//                 aiPathUpdateBlockedWaypoint(ai->navSearch->path.curWaypoint);
// 
//                 // The target isn't a player, so I'm probably just wandering around.
//                 //   I'll just find a new path eventually.
// 
//                 AI_LOG(    AI_LOG_PATH,
//                     (e,
//                     "Got stuck while following leader.\n"));
// 
//                 aiDiscardFollowTarget(e, "Ran into something, not sure what.", false);
//                 ai->waitingForTargetToMove = 0;
// 
//             }else{
//                 // If I'm stuck and I have already tried shortcutting,
//                 //   then redo the path but don't allow shortcutting.
// 
//                 AI_LOG(    AI_LOG_PATH,
//                     (e,
//                     "I got stuck, trying not shortcutting.\n"));
// 
//                 aiConstructPathToTarget(e);
// 
//                 aiStartPath(e, 1, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);
// 
//                 ai->waitingForTargetToMove = 1;
//             }
// 
//             break;
//                                  }
// 
//         case AI_MSG_BAD_PATH:{
//             clearNavSearch(ai->navSearch);
// 
//             aiDiscardFollowTarget(e, "Bad path.", false);
// 
//             break;
//                              }
// 
//         case AI_MSG_FOLLOW_TARGET_REACHED:{
//             aiDiscardFollowTarget(e, "Reached follow target (leader).", true);
// 
//             ai->time.checkedForShortcut = 0;
//             ai->doNotRun = ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : 0;
// 
//             break;
//                                           }
// 
//         case AI_MSG_ATTACK_TARGET_MOVED:{
//             // The target has moved, so reset the find path timer.
// 
//             AI_LOG(    AI_LOG_PATH,
//                 (e,
//                 "Attack target moved a lot, ^1cancelling path.\n"));
// 
//             aiDiscardFollowTarget(e, "Leader moved a lot, cancelling path", false);
// 
//             ai->waitingForTargetToMove = 0;
// 
//             break;
//                                         }
//     }
// 
//     return 0;
// }
// 
// 
void aiCritterActivityPatrol(Entity* e, AIVars* ai){

    // IF i have been patrolling for too long
    //   THEN discard current patrol point (timed out)
    // ELSE IF i have a target patrol point
    //   THEN start moving to it

    if(ai->hasNextPatrolPoint)
    {    
        // start a new path
        if( !ai->motion.type ||
            !ai->navSearch->path.head ||
            ai->followTarget.type != AI_FOLLOWTARGET_POSITION ||
            ai->followTarget.reason != AI_FOLLOWTARGETREASON_PATROLPOINT ||
            !sameVec3(ai->followTarget.pos, ai->anchorPos))
        {
            aiSetFollowTargetPosition(e, ai, ai->anchorPos, AI_FOLLOWTARGETREASON_PATROLPOINT);
            aiConstructPathToTarget(e);

            //ai->doNotRun = 1; 
            aiStartPath(e, 0, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);    // not sure if we should prevent shortcut here...

            // IF we failed to find a path, 
            //   OR it has taken too long to reach this point,
            // THEN discard the patrol point, as it is unreachable
            if(!ai->navSearch->path.head)
            {
                ai->hasNextPatrolPoint = 0;
            }
            
            ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 10;
        }
    }
}
// 
