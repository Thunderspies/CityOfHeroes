#include "entaiCritterPrivate.h"
#include "entaiprivate.h"
#include "entaiCritterPrivate.h"
#include "beacon/beaconPrivate.h"
#include "entity/entserver.h"
#include "entity/entity.h"
#include "cmdparse/cmdcommon.h"
#include "entity/powers.h"
#include "entity/character_base.h"

void aiCritterDoTsooMinion(Entity* e, AIVars* ai)
{
    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    if (aiCritterDoDefaultBuffs(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

void aiCritterDoTsooHealer(Entity* e, AIVars* ai)
{
    AIPowerInfo *healPower;
    bool didsomething=false;

    // If we're on a team, and there are members alive who are not Healers, then do wacky jumping stuff
    if (ai->teamMemberInfo.team && (int)ai->teamMemberInfo.team->members.aliveCount > 1 + aiCritterCountMinions(e, ai, AI_BRAIN_TSOO_HEALER)) {
        ai->doNotMove = 1;

        healPower = aiChoosePower(e, ai, AI_POWERGROUP_HEAL_ALLY);
        if (healPower && !healPower->power->bActive && !healPower->power->fTimer) {
            // Our healing power is ready!
            // See if we can teleport
            AIPowerInfo *telePower = aiChoosePower(e, ai, AI_POWERGROUP_TELEPORT);
            Entity *target = NULL;
            F32 targetRange;
            F32 powerRange = aiGetPowerRange(e->pchar, healPower);
            if (telePower && !telePower->power->bActive && !telePower->power->fTimer && (
                !telePower->teamInfo || ABS_TIME_SINCE(telePower->teamInfo->time.lastUsed)>SEC_TO_ABS(1.5)))
            {
                // We can teleport to anyone in need!
                targetRange = aiGetPowerRange(e->pchar, telePower);
            } else {
                targetRange = powerRange;
            }

            target = aiCritterFindTeammateInNeed(e, ai, targetRange, NULL, kTargetType_Friend);
            if (target && target->pchar->attrCur.fHitPoints <= target->pchar->attrMax.fHitPoints * 0.75) {
                // Found a guy we want to heal!
                F32 distSQR = distance3Squared(ENTPOS(e), ENTPOS(target));
                if (distSQR < powerRange*powerRange) {
                    // We are close enough to just use the heal power!
                    aiUsePower(e, healPower, target);
                    didsomething = true;
                } else if (telePower) {
                    // We need to teleport!
                    Vec3 location;
                    copyVec3(ENTPOS(target), location);
                    location[1]+=1.0;
                    aiUsePowerOnLocation(e, telePower, location);
                    didsomething = true;
                }
            } else {
                // There are no teammates within range that are injured
                // If none in range, we want to get closer,
                // If none injured, we want to be closer, but not within range of combat
                if (telePower && !telePower->power->bActive && !telePower->power->fTimer && (
                    !telePower->teamInfo || ABS_TIME_SINCE(telePower->teamInfo->time.lastUsed)>SEC_TO_ABS(1.5)) &&
                    ABS_TIME_SINCE(ai->time.friendWasAttacked) < SEC_TO_ABS(15))
                {
                    // We can teleport
                    // Jump to closest spot with least nearby enemies (beacon farthest from average enemy position)
                    if(aiCritterDoTeleport(e, ai, telePower))
                        return;
                }
            }
        }
    } else {
        ai->doNotMove = 0;
    }

    // returning when doing something already
    //if (didsomething)
    //    return;

    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    if (aiCritterDoDefaultBuffs(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}