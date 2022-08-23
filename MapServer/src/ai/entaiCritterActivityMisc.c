
#include "entaiprivate.h"
#include "entaiCritterPrivate.h"
#include "gameSys/dooranim.h"
#include "beacon/beaconPath.h"
#include "entity/entserver.h"
#include "dbcomm/dbdoor.h"
#include "seq/seqstate.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "gridcoll/megaGrid.h"
#include "cmdparse/cmdcommon.h"
#include "entity/character_base.h"
#include <utilitieslib/utils/mathutil.h>
#include "utils/position.h"

void aiCritterActivityDoAnim(Entity* e, AIVars* ai){
    aiDiscardFollowTarget(e, "Doing anim.", false);

    ai->waitForThink = 1;
}

void aiCritterActivityFrozenInPlace(Entity* e, AIVars* ai){
    aiDiscardFollowTarget(e, "Frozen.", false);

    ai->waitForThink = 1;
    
    aiMotionSwitchToNone(e, ai, "Frozen in place.");
    
    e->move_type = 0;
}

void aiCritterActivityFollowRoute(Entity* e, AIVars* ai, PatrolRoute* route)
{
/*
    AITeamMemberInfo *member;

    if(!ai->amPatrolLeader && !ai->navSearch->path.head)
    {
        if(ai->teamMemberInfo.team && !ai->teamMemberInfo.team->patrolLeader)
        {
            for(member = ai->teamMemberInfo.team->members.list; member; member = member->next)
            {
                if(aiBehaviorGetActivity(member->entity) == AI_ACTIVITY_FOLLOW_ROUTE)
                {
                    aiFollowPatrolRoute(member->entity, ENTAI(member->entity), NULL, ai->route,
                        ai->teamMemberInfo.team ? ai->teamMemberInfo.team->curPatrolPoint : 0, ai->doNotRun);
                }
            }
        }
        if(!ai->amPatrolLeader && ai->teamMemberInfo.team)    // might've become the patrol leader
        {
            aiFollowPathToEnt(e, ai, ai->teamMemberInfo.team->patrolLeader);
        }
    }
    else if(!ai->navSearch->path.head)
    {
        aiFollowPatrolRoute(e, ai, NULL, ai->route,
            ai->teamMemberInfo.team ? ai->teamMemberInfo.team->curPatrolPoint : 0, ai->doNotRun);
    }
*/
}

// void aiCritterActivityHideBehindEnt(Entity* e, AIVars* ai)
// {
//     int entArray[MAX_ENTITIES_PRIVATE];
//     int    entCount, numEnemies = 0;
//     Vec3 temp;
//     F32 distFromMe;
//     Entity *protector = erGetEnt(ai->protectEnt);
// 
//     if(!protector)
//         return;
// 
//     if(ABS_TIME_SINCE(ai->HBEcheckedProximity) > SEC_TO_ABS(1))
//     {
//         int i, idx;
//         F32 dist;
// 
//         ai->HBEcheckedProximity = ABS_TIME;
//         entCount = mgGetNodesInRange(0, ENTPOS(e), (void**)entArray, 0);
//         zeroVec3(ai->enemyGeoAvg);
//         ai->amScared = false;
//         
//         for(i = 0; i < entCount; i++)
//         {
//             idx = entArray[i];
// 
//             if(ENTTYPE_BY_ID(idx) == ENTTYPE_CRITTER)
//             {
//                 Entity* entProx = validEntFromId(idx);
// 
//                 if(entProx && entProx->pchar
//                     && (entProx->pchar->iAllyID==kAllyID_Monster))
//                 {
//                     Character* pchar = entProx->pchar;
// 
//                     if(    pchar &&
//                         pchar->attrCur.fHitPoints > 0)
//                     {
//                         dist = distance3Squared(ENTPOS(e), ENTPOS(entProx));
// 
//                         if(dist <= SQR(ai->HBEsearchdist))
//                         {
//                             if(aiEntityCanSeeEntity(e, entProx, ai->HBEsearchdist))
//                             {
// /*
//                                 subVec3(entProx->mat[3], protector->mat[3], temp);
//                                 addVec3(geoAvg, temp, geoAvg);
// */
//                                 addVec3(ENTPOS(entProx), ai->enemyGeoAvg, ai->enemyGeoAvg);
//                                 numEnemies++;
//                                 ai->amScared = true;
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//         if(numEnemies > 1)
//         {
//             ai->enemyGeoAvg[0] = ai->enemyGeoAvg[0] ? (ai->enemyGeoAvg[0] / numEnemies) : 0;
//             ai->enemyGeoAvg[1] = ai->enemyGeoAvg[1] ? (ai->enemyGeoAvg[1] / numEnemies) : 0;
//             ai->enemyGeoAvg[2] = ai->enemyGeoAvg[2] ? (ai->enemyGeoAvg[2] / numEnemies) : 0;
//         }
//     }
// 
//     // check numEnemies here??
//     if(!ai->amScared) 
//     {
//         distFromMe = distance3Squared(ENTPOS(protector), ENTPOS(e));
// 
//         CLRB(e->seq->sticky_state, STATE_FEAR);  
// 
//         if(!ai->navSearch->path.head && distFromMe > SQR(20))
//         {
//             aiSetFollowTargetEntity(e, ai, protector, AI_FOLLOWTARGETREASON_SAVIOR);
//             ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 7.0;
//             aiConstructPathToTarget(e);
//             aiStartPath(e, 0, 0/*distFromMe > SQR(30) ? 0 : 1*/);
//         }
//         return;
//     }
// 
//     // find point five feet behind guy you're hiding behind
//     subVec3(ai->enemyGeoAvg, ENTPOS(protector), temp);
//     normalVec3(temp);
//     scaleVec3(temp, -5, temp);
//     addVec3(temp, ENTPOS(protector), temp);
// 
//     distFromMe = distance3Squared(temp, ENTPOS(e));
// 
//     if(ai->navSearch->path.head)
//     {
//         F32 distFromPath = distance3Squared(temp, ai->navSearch->path.tail->pos);
// 
//         if(distFromMe < distFromPath || distFromPath > SQR(50))
//         {
//             clearNavSearch(ai->navSearch);
//         }
//     }
// 
//     if((!ai->navSearch->path.head && !ai->motion.type) &&
//         distFromMe > ai->inputs.standoffDistance ? ai->inputs.standoffDistance : SQR(3))
//     {
//         ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 2.5;
//         aiSetFollowTargetPosition(e, ai, temp, AI_FOLLOWTARGETREASON_COWER);
//         aiConstructPathToTarget(e);
// 
//         aiStartPath(e, 0, 0);
//     }
//     
//     SETB(e->seq->sticky_state, STATE_FEAR);
// }

void aiCritterActivityRunIntoDoor(Entity* e, AIVars* ai)
{
//     DoorEntry* door = NULL;
// 
//     if(!ai->navSearch->path.head && !ai->motion.type )
//     {
//         if( ABS_TIME_SINCE( ai->time.lastFailedADoorPathCheck) > SEC_TO_ABS(2) )
//         {
//             door = dbFindReachableLocalMapObj(e, 0, 10000);
//             if( door )
//             {
//                 ai->disappearInDoor = door;
//                 aiDiscardFollowTarget(e, "Going to run into some door and disappear now.", false);
//                 aiSetFollowTargetDoor(e, ai, door, AI_FOLLOWTARGETREASON_ENTERABLE);
//                 aiConstructPathToTarget(e);
//                 aiStartPath(e, 0, 0);
//             }
//             else
//             {
//                 ai->time.lastFailedADoorPathCheck = ABS_TIME;
//             }
//         }
//     }
}

void aiCritterActivityRunOutOfDoor(Entity* e, AIVars* ai){
    aiDiscardFollowTarget(e, "Frozen.", false);

    aiMotionSwitchToNone(e, ai, "Running out of door.");
    
    e->move_type = 0;
}

void aiCritterActivityStandStill(Entity* e, AIVars* ai){
    if(ai->turnWhileIdle && !e->IAmInEmoteChatMode){
        if(getCappedRandom(1000) < 3  && !e->cutSceneActor){
            Vec3 pyr;
            Mat3 newmat;
            
            getMat3YPR(ENTMAT(e), pyr);

            pyr[1] = addAngle(pyr[1], RAD(rand() % 360 - 180));
            
            createMat3YPR(newmat, pyr);
            entSetMat3(e, newmat);
        }
    }
    
    CLRB(e->seq->sticky_state, STATE_COMBAT);

    aiDiscardFollowTarget(e, "Standing still.", false);

    ai->waitForThink = 1;
}