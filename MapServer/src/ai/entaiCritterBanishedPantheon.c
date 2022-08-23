#include "entaiCritterPrivate.h"
#include "entaiprivate.h"
#include "entaiCritterPrivate.h"
#include "beacon/beaconPath.h"
#include "entity/motion.h"
#include "entity/entity.h"
#include "cmdparse/cmdcommon.h"
#include "entity/character_base.h"
#include <utilitieslib/utils/mathutil.h>
#include "utils/position.h"

// Banished Pantheon Minions
void aiCritterDoBanishedPantheon(Entity* e, AIVars* ai){
    AIProxEntStatus *status;
    // Decay
    for (status = ai->proxEntStatusTable->statusHead; status; status = status->tableList.next) {
        status->damage.toMe *=0.98;
    }

    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    if (aiCritterDoDefaultBuffs(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

static void aiCritterFindCirclePosition(AIVars *ai, const Vec3 vCurPos, Vec3 vDest, F32 maxrange) {
    Vec3 vTemp;
    F32 angle = ai->brain.banishedpantheon.clockwise?(PI/6):(-PI/6);
    F32 sint;
    F32 cost;
    F32 scale = 1.0;
    F32 length;
    U32 i;

    for (i=0; i<ai->brain.banishedpantheon.retries; i++) {
        angle /= 2;
    }

    sint = fsin(angle);
    cost = fcos(angle);

    // Calc delta
    subVec3(vCurPos, ENTPOS(ai->attackTarget.entity), vDest);
    length = lengthVec3(vDest);
    if (length < maxrange * 0.6) { // Try to stay maxrange ft out or more
        scale = 1.5;
    }
    // Rotate the delta vector
    vTemp[0] = (cost*vDest[0] + sint*vDest[2])*scale;
    vTemp[1] = vDest[1]/1.2;
    vTemp[2] = (cost*vDest[2] - sint*vDest[0])*scale;
    addVec3(ENTPOS(ai->attackTarget.entity), vTemp, vDest);
}

void aiCritterDoBanishedPantheonSorrow(Entity* e, AIVars* ai){
    if (ai->attackTarget.entity && !ai->isAfraidFromAI && !ai->isAfraidFromPowers) {
        // We have someone we're trying to attack and we're not afraid

        // IF: We're not confused
        //   AND it's been more than X seconds
        //   AND we're in range of our target
        //   AND we're not flagged as not moving?
        //   AND we've got a power
        // THEN: find a point 30 degrees clockwise to move to
        if(    !character_IsConfused(e->pchar) &&
            ABS_TIME_SINCE(ai->time.foundPath) > SEC_TO_ABS(1) &&
            //            ai->followTarget.type == AI_FOLLOWTARGET_NONE &&
            ai->targetWasInRange &&
            !ai->doNotMove &&
            ai->power.preferred)
        {
            Vec3 vDest;
            F32 maxrange = aiGetPowerRange(e->pchar, ai->power.preferred);

            ai->brain.banishedpantheon.retries = 0;
            aiCritterFindCirclePosition(ai, ENTPOS(e), vDest, maxrange);
            aiSetFollowTargetPosition(e, ai, vDest, AI_FOLLOWTARGETREASON_CIRCLECOMBAT);
            aiConstructPathToTarget(e);

            if (!ai->navSearch->path.head) {
                ai->brain.banishedpantheon.retries++;
                aiCritterFindCirclePosition(ai, ENTPOS(e), vDest, maxrange);
                aiSetFollowTargetPosition(e, ai, vDest, AI_FOLLOWTARGETREASON_CIRCLECOMBAT);
                aiConstructPathToTarget(e);
                if (!ai->navSearch->path.head) {
                    ai->brain.banishedpantheon.clockwise ^= 1; // invert it
                    ai->brain.banishedpantheon.retries = 0;
                }
            }

            if (ai->navSearch->path.head) {
                ai->time.foundPath = ABS_TIME;
                aiStartPath(e, 0, 1);
            }

            ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : (ai->collisionRadius + 2);
        }
    }
    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    if (aiCritterDoDefaultBuffs(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

void aiCritterDoBanishedPantheonLt(Entity* e, AIVars *ai){
    if (rand() % 100 < 30) {
        if (aiCritterDoSummoning(e, ai, AI_BRAIN_BANISHED_PANTHEON, 3))
            return;
    }
    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    // Buff/debuff powers
    if (aiCritterDoDefaultBuffs(e, ai))
        return;

    aiCritterDoDefault(e, ai, 0);
}
