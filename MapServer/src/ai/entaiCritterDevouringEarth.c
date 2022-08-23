#include "entaiCritterPrivate.h"
#include "entaiprivate.h"
#include "entaiCritterPrivate.h"
#include "beacon/beaconPath.h"
#include "entity/entGameActions.h"
#include "gameComm/VillainDef.h"
#include "entity/entity.h"
#include "cmdparse/cmdcommon.h"
#include "entity/powers.h"
#include "entity/character_base.h"
#include <utilitieslib/components/earray.h>
#include "cmdparse/cmdserver.h"
#include <utilitieslib/utils/mathutil.h>
#include "utils/position.h"
#include "ai/entaiLog.h"

void aiCritterDoDevouringEarthMinion(Entity* e, AIVars* ai)
{
    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    if (aiCritterDoDefaultBuffs(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

static int countNumberOfTeammatesInRange(AIVars *ai, Vec3 pos, F32 radiusSquared, int *totalCount)
{
    int count=0;
    AITeamMemberInfo* member;
    *totalCount=0;
    for(member = ai->teamMemberInfo.team->members.list; member; member = member->next){
        AIVars *aimember = ENTAI(member->entity);
        if (aimember->brain.type != AI_BRAIN_DEVOURING_EARTH_LT &&
            aimember->brain.type != AI_BRAIN_DEVOURING_EARTH_MINION ||
            member->entity->villainDef->rank == VR_SMALL)
            continue;
        if(member->entity->pchar->attrCur.fHitPoints > 0) {
            if (distance3Squared(ENTPOS(member->entity), pos) < radiusSquared){
                count++;
            }
            (*totalCount)++;
        }
    }
    return count;
}

void aiCritterFindBestTeamPuddlePosition(Entity *e, AIVars * ai, F32 maxRangeSquared, F32 radiusSquared, OUT Vec3 *pos, OUT Vec3 *pos2)
{
    // Loop through teammates, find center point
    // Loop through points and find the one that encompasses the most teammates
    Vec3 *locations;
    int *counts;
    int count, i, j, max, max2;
    Vec3 average = {0,0,0};
    AITeamMemberInfo* member;

    if(!ai->teamMemberInfo.team){
        copyVec3(ENTPOS(e), *pos);
    } else {
        for(member = ai->teamMemberInfo.team->members.list, count=0; member; member = member->next){
            if(member->entity->pchar->attrCur.fHitPoints > 0 && distance3Squared(ENTPOS(member->entity), ENTPOS(e)) < maxRangeSquared){
                AIVars *aimember = ENTAI(member->entity);
                if (aimember->brain.type != AI_BRAIN_DEVOURING_EARTH_LT &&
                    aimember->brain.type != AI_BRAIN_DEVOURING_EARTH_MINION ||
                    member->entity->villainDef->rank == VR_SMALL)
                    continue;
                count++;
            }
        }
        count++; // For average location
        locations = _alloca(sizeof(*locations)*count);
        counts = _alloca(sizeof(*counts)*count);
        ZeroMemory(counts, sizeof(*counts)*count);
        for(member = ai->teamMemberInfo.team->members.list, i=0; member; member = member->next){
            if(member->entity->pchar->attrCur.fHitPoints > 0 && distance3Squared(ENTPOS(member->entity), ENTPOS(e)) < maxRangeSquared){
                AIVars *aimember = ENTAI(member->entity);
                if (aimember->brain.type != AI_BRAIN_DEVOURING_EARTH_LT &&
                    aimember->brain.type != AI_BRAIN_DEVOURING_EARTH_MINION ||
                    member->entity->villainDef->rank == VR_SMALL)
                    continue;
                copyVec3(ENTPOS(member->entity), locations[i]);
                addVec3(average, locations[i], average);
                i++;
            }
        }
        average[0] /= count-1;
        average[1] /= count-1;
        average[2] /= count-1;
        copyVec3(average, locations[i]);
        for (i=0; i<count; i++) {
            if (i!=count-1)
                counts[i]++; // Will get self!
            for (j=i+1; j<count; j++) {
                F32 dist = distance3Squared(locations[i], locations[j]);
                if (dist <= radiusSquared) {
                    if (j!=count-1) // The Average isn't a person, we don't gain anything here!
                        counts[i]++;
                    counts[j]++;
                }
            }
        }
        for (i=1, max=0, max2=-1; i<count; i++) {
            if (counts[i] >= counts[max]) {
                max2 = max;
                max = i;
            } else if (max2==-1 || counts[i] > counts[max2]) {
                max2 = i;
            }
        }
        copyVec3(locations[max], *pos);
    } // if team
}

void aiCritterDoDevouringEarthBoss(Entity* e, AIVars* ai)
{
    // Summon minions if you're lonely
    if (rand() % 100 < 30) {
        if (aiCritterDoSummoning(e, ai, AI_BRAIN_DEVOURING_EARTH_MINION, 2))
            return;
    }
    // The choose two targets thing
    aiCritterDoCotBehemoth(e, ai);
}


void aiCritterDoDevouringEarthLt(Entity* e, AIVars* ai)
{
    bool didsomething=false;
    // Summon minions if you're lonely
    if (rand() % 100 < 30) {
        if (aiCritterDoSummoning(e, ai, AI_BRAIN_DEVOURING_EARTH_MINION, 2))
            return;
    }
#define DEFAULT_RANGE 40
#define TOLERANCE 9.0

    if (ABS_TIME >= ai->brain.devouringearth.time.nextThinkAboutPet) {
        // Check to see if Pet Puddle is out of range of most of the combat, if so, summon a new one
        //  closer to combat (and kill the old one)
        switch(ai->brain.devouringearth.state) {
        case DES_NONE:
            // If we can use a power, and we haven't already chosen a power
            if (!e->pchar->ppowCurrent && !e->pchar->ppowQueued)
            {

                AIPowerInfo *info = aiChoosePower(e, ai, AI_POWERGROUP_PLANT);
                if (info && !info->power->bActive && !info->power->fTimer && info->teamInfo && info->teamInfo->inuseCount==0) {
                    Vec3 second;
                    int justspawnit=false;
                    // Find location where it can help the most friends!
                    // Move at most 100 feet
                    aiCritterFindBestTeamPuddlePosition(e, ai, SQR(100), SQR(DEFAULT_RANGE), &ai->brain.devouringearth.target, &second);

                    if (distance3Squared(ENTPOS(e), ai->brain.devouringearth.target) < SQR(TOLERANCE)) {
                        AI_LOG(AI_LOG_SPECIAL, (e, "Best spot is close enough, just spawning it!\n"));
                        justspawnit = true;
                    } else {

                        aiSetFollowTargetPosition(e, ai, ai->brain.devouringearth.target, AI_FOLLOWTARGETREASON_CIRCLECOMBAT);
                        aiConstructPathToTarget(e);

                        if (ai->navSearch->path.head) {
                            ai->time.foundPath = ABS_TIME;
                            aiStartPath(e, 0, 1);
                            ai->brain.devouringearth.state = DES_MOVING_TO_TARGET;
                            ai->brain.devouringearth.time.nextThinkAboutPet = ABS_TIME + SEC_TO_ABS(2);
                            AI_LOG(AI_LOG_SPECIAL, (e, "Moving to where I want to spawn my pet\n"));
                        } else {
                            // Bad point!  Try the second best point

                            copyVec3(second, ai->brain.devouringearth.target);

                            aiSetFollowTargetPosition(e, ai, ai->brain.devouringearth.target, AI_FOLLOWTARGETREASON_CIRCLECOMBAT);
                            aiConstructPathToTarget(e);

                            if (ai->navSearch->path.head) {
                                ai->time.foundPath = ABS_TIME;
                                aiStartPath(e, 0, 1);
                                ai->brain.devouringearth.state = DES_MOVING_TO_TARGET;
                                ai->brain.devouringearth.time.nextThinkAboutPet = ABS_TIME + SEC_TO_ABS(2);
                                AI_LOG(AI_LOG_SPECIAL, (e, "Moving to where I want to spawn my pet\n"));
                            } else {
                                // I can't get anywhere!  Just plop one right on top of me!
                                aiDiscardFollowTarget(e, "Can't get to where I want to go.  Here is good enough.", false);
                                AI_LOG(AI_LOG_SPECIAL, (e, "Can't get to where I'm going, spawning pet here!\n"));
                                justspawnit = true;
                            }
                        }
                    }
                    if (justspawnit) {
                        aiUsePowerOnLocation(e, info, ai->brain.devouringearth.target);
                        aiSetCurrentBuff(&ai->brain.devouringearth.curPlant, info);
                        ai->brain.devouringearth.state = DES_WAITING_FOR_PET;
                        copyVec3(ENTPOS(e), ai->brain.devouringearth.target); // Store location that the pet was spawned
                        ai->brain.devouringearth.time.nextThinkAboutPet = ABS_TIME + SEC_TO_ABS(2);
                    }
                    didsomething = true;
                }
            }
            break;
        case DES_MOVING_TO_TARGET:
            if (!e->pchar->ppowCurrent && !e->pchar->ppowQueued)
            {
                AIPowerInfo *info = aiChoosePower(e, ai, AI_POWERGROUP_PLANT);
                if (info && !info->power->bActive && !info->power->fTimer && info->teamInfo->inuseCount==0) {
                    if (distance3Squared(ENTPOS(e), ai->brain.devouringearth.target) < SQR(TOLERANCE)) {
                        // We made it here!
                        aiDiscardFollowTarget(e, "Got to where I want to plany my power", false);
                        AI_LOG(AI_LOG_SPECIAL, (e, "Got to my target location, using pet!\n"));
                        didsomething = true;
                    } else if (ai->followTarget.type == AI_FOLLOWTARGET_NONE) {
                        // Lost my path!
                        // Here most be as good a spot as anywhere, since apparently I'm in combat
                        AI_LOG(AI_LOG_SPECIAL, (e, "Lost my path, Putting it here\n"));
                        didsomething = true;
                    }
                    if (didsomething) {
                        aiUsePowerOnLocation(e, info, ai->brain.devouringearth.target);
                        aiSetCurrentBuff(&ai->brain.devouringearth.curPlant, info);
                        ai->brain.devouringearth.state = DES_WAITING_FOR_PET;
                        copyVec3(ENTPOS(e), ai->brain.devouringearth.target); // Store location that the pet was spawned
                        ai->brain.devouringearth.time.nextThinkAboutPet = ABS_TIME + SEC_TO_ABS(2);
                    }
                } else {
                    // Can't use this power (perhaps a teammate just used it)
                    ai->brain.devouringearth.state = DES_NONE;
                    ai->brain.devouringearth.time.nextThinkAboutPet = ABS_TIME + SEC_TO_ABS(2);
                    AI_LOG(AI_LOG_SPECIAL, (e, "Can't use my plant power for some reason, reseting thinker\n"));
                }
            }
            break;
        case DES_WAITING_FOR_PET:
            break;
        case DES_HAS_PET:
            {
                // I've got a pet plant
                Entity *petPlant = erGetEnt(ai->brain.devouringearth.pet);
                if (petPlant==NULL || petPlant->pchar->attrCur.fHitPoints <= 0) {
                    // They killed my pet!  Make them pay!.. . . ..or just get another one.
                    ai->brain.devouringearth.pet = 0;
                    if (ai->brain.devouringearth.curPlant) {
                        aiClearCurrentBuff(&ai->brain.devouringearth.curPlant);
                    }
                    ai->brain.devouringearth.state = DES_NONE;
                    AI_LOG(AI_LOG_SPECIAL, (e, "They ^1killed ^0my pet!\n"));
                    ai->brain.devouringearth.savedHealth = 0;
                    // Wait 90 seconds before spawning another one
                    ai->brain.devouringearth.time.nextThinkAboutPet = ABS_TIME + SEC_TO_ABS(90);
                    didsomething = true;
                } else {
                    AIPowerInfo *info = aiChoosePower(e, ai, AI_POWERGROUP_PLANT);

                    if (ai->brain.devouringearth.savedHealth!=0) {
                        AI_LOG(AI_LOG_SPECIAL, (e, "Restoring %s^0's health to %.1f\n", aiLogEntName(petPlant), ai->brain.devouringearth.savedHealth));
                        petPlant->pchar->attrCur.fHitPoints = ai->brain.devouringearth.savedHealth;
                        ai->brain.devouringearth.savedHealth = 0;
                    }

                    // Only do checks if the power has recharged
                    if (info && !info->power->bActive && !info->power->fTimer) {
                        // Do checks
                        int totalCount;
                        int count = countNumberOfTeammatesInRange(ai, ai->brain.devouringearth.target, SQR(DEFAULT_RANGE), &totalCount);

                        if (count < (int)(totalCount+1) / 2) {
                            // If we're not covering half of our teammates, try to do better next time!
                            // Kill the old pet
                            
                            {
                                // Set me (the Devouring Earth guy) as a damager, so that
                                // the players only get partial XP
                                DamageTracker *dt = damageTrackerCreate();
                                eaPush(&petPlant->who_damaged_me, dt);

                                dt->damage += petPlant->pchar->attrCur.fHitPoints;
                                ai->brain.devouringearth.savedHealth = petPlant->pchar->attrCur.fHitPoints;
                                petPlant->pchar->erLastDamage = dt->ref = erGetRef(e);
                                // Update the team in case of a team switch.
                                dt->group_id = e->teamup_id;
                            }

                            petPlant->pchar->attrCur.fHitPoints = -1000000;
                            AI_LOG(AI_LOG_SPECIAL, (e, "Pet too far away, ^1killing it^0\n"));
                            ai->brain.devouringearth.state = DES_NONE;
                            if(ai->brain.devouringearth.curPlant){
                                aiClearCurrentBuff(&ai->brain.devouringearth.curPlant);
                            }
                            didsomething = true;
                        } else {
                            // Wait 2 seconds before checking again
                            ai->brain.devouringearth.time.nextThinkAboutPet = ABS_TIME + SEC_TO_ABS(2);
                        }
                    }
                }
            }
            break;
        default:
            assert(0);
        }
    }

    if (didsomething)
        return;
    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    if (aiCritterDoDefaultBuffs(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

