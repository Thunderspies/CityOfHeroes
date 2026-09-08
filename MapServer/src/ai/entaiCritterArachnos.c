#include "entaiCritterPrivate.h"
#include "entaiprivate.h"
#include "entaiCritterPrivate.h"
#include "beacon/beaconPath.h"
#include "entity/motion.h"
#include "seq/seqstate.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "cmdparse/cmdcommon.h"
#include "entity/powers.h"
#include "entity/character_combat.h"
#include "entity/character_base.h"
#include "cmdparse/cmdserver.h"
#include <utilitieslib/utils/mathutil.h>
#include "utils/position.h"
#include "ai/entaiLog.h"
#include "entity/entGameActions.h"
#include <utilitieslib/components/earray.h>

static float randomFloat(float min, float max)
{
    U32 rand = rule30Rand();
    F32 dif = max - min;
    F32 at = dif * rand / UINT_MAX;
    return min + at;
}

static float getEntityHealthPercent(Entity *e)
{
    if (e && e->pchar)
        return e->pchar->attrCur.fHitPoints / e->pchar->attrMax.fHitPoints;
    else
        return 0.0f;
}

static int countEnemiesWithinRange(Entity *e, AIVars *ai, float rangeSquared, bool attackedMe)
{
    // Count number of melee attackers and check if enemies in range
    int count=0;
    AIProxEntStatus *status;

    for (status = ai->proxEntStatusTable->statusHead; status; status = status->tableList.next) {
        if (status->entity && 
            distance3Squared(ENTPOS(status->entity), ENTPOS(e)) < rangeSquared)
        {
            if (attackedMe) {
                if (status->hasAttackedMe && ABS_TIME_SINCE(status->time.attackMe) < SEC_TO_ABS(7))
                {
                    // Attacked me and in melee range
                    count++;
                }
            } else {
                count++;
            }
        }
    }
    return count;
}

static int aiCritterArachnosCanIUsePower(Entity* e, AIPowerInfo *pInfo)
{
    return ((e && e->pchar && !e->pchar->ppowCurrent && !e->pchar->ppowQueued) && 
            (pInfo && pInfo->power && pInfo->power->fTimer == 0.0f));
}



int aiCritterUsePowerOnTeammate(Entity* e, AIVars* ai, AIPowerInfo* info)
{
    if (info && info->power && info->power->ppowBase && 
        ABS_TIME_SINCE(info->time.lastUsed) > SEC_TO_ABS(info->power->ppowBase->fActivatePeriod))
    {
        AITeamMemberInfo* member;
        for(member = ai->teamMemberInfo.team->members.list; member; member = member->next)
        {
            Entity* entMember = member->entity;
            if(aiPowerTargetAllowed(e, info, entMember, true))
            {
                aiUsePower(e, info, entMember);
                return true;
            }
        }
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void aiCritterDoArachnosWebTower(Entity* e, AIVars* ai)
{
    int usedPower = false;
    AIPowerInfo* info = NULL;

    if (e->pchar)
    {
        // should I summon some repair men?
        if (((e->pchar->attrCur.fHitPoints / e->pchar->attrMax.fHitPoints) < 0.5f) &&
            (e->petList_size < 3) )
        {
            info = aiChoosePower(e, ai, AI_POWERGROUP_SUMMON);

            if (info)
            {
                aiUsePower(e, info, NULL);
                usedPower = true;
            }
        } 

        // if I haven't done any of that, should I buff my friends?
        if (!usedPower) 
        {
            AIPowerInfo* info = aiChoosePower(e, ai, AI_POWERGROUP_BUFF);

            usedPower = aiCritterUsePowerOnTeammate(e, ai, info);
        }
    }

    if (aiCritterDoDefaultBuffs(e, ai))
        return;
    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;

    aiCritterDoDefault(e, ai, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void aiCritterDoArachnosRepairman(Entity* e, AIVars* ai)
{
    AITeamMemberInfo* member;
    Entity *eClosest = NULL;
    float fClosestDistance = 99999999.0f;

    if (e->pchar && ai && ai->teamMemberInfo.team)
        for(member = ai->teamMemberInfo.team->members.list; member; member = member->next)
        {
            Entity* entMember = member->entity;

            if (entMember && entMember->pchar)
            {
                if (entMember->pchar->attrCur.fHitPoints < entMember->pchar->attrMax.fHitPoints &&
                    entMember->pchar->attrCur.fHitPoints > 0.0f)
                {
                    AIPowerInfo* info = aiChoosePower(e, ai, AI_POWERGROUP_HEAL_ALLY);

                    if (info && info->power && info->power->ppowBase && 
                        ABS_TIME_SINCE(info->time.lastUsed) > SEC_TO_ABS(info->power->ppowBase->fActivatePeriod) &&
                        aiPowerTargetAllowed(e, info, entMember, true))
                    {
                        float range = aiGetPowerRange(e->pchar, info);
                        float fDistance = distance3Squared(ENTPOS(e), ENTPOS(entMember));

                        if(fDistance < SQR(range))
                        {
                            aiUsePower(e, info, entMember);
                            return;
                        } else {
                            if (fDistance <= fClosestDistance)
                            {
                                fClosestDistance = fDistance;
                                eClosest = entMember;
                            }
                        }
                    }
                } 
                else if (entMember->pchar->attrCur.fHitPoints <= 0.0f)
                {
                    AIPowerInfo* info = aiChoosePower(e, ai, AI_POWERGROUP_RESURRECT);
                    if (info && info->power && info->power->ppowBase && 
                        ABS_TIME_SINCE(info->time.lastUsed) > SEC_TO_ABS(info->power->ppowBase->fActivatePeriod) &&
                        aiPowerTargetAllowed(e, info, entMember, true))
                    {
                        float range = aiGetPowerRange(e->pchar, info);
                        float fDistance = distance3Squared(ENTPOS(e), ENTPOS(entMember));

                        if(fDistance < SQR(range))
                        {
                            aiUsePower(e, info, entMember);
                            return;
                        } else {
                            if (fDistance <= fClosestDistance)
                            {
                                fClosestDistance = fDistance;
                                eClosest = entMember;
                            }
                        }
                    }
                }
            }
        }

        // we found someone we can help but they are too far away
        if (eClosest != NULL && ABS_TIME_SINCE(ai->time.foundPath) > SEC_TO_ABS(2))
        {
            aiSetFollowTargetEntity(e, ai, eClosest, AI_FOLLOWTARGETREASON_MOVING_TO_HELP); 
            aiConstructPathToTarget(e);
            aiStartPath(e, 0, 0);
        }

        if (aiCritterDoDefaultBuffs(e, ai))
            return;
        if (aiCritterDoDefaultPacedPowers(e, ai))
            return;
        aiCritterDoDefault(e, ai, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void aiCritterDoArachnosDrAeon(Entity* e, AIVars* ai)
{

    AIPowerInfo* info = aiChoosePower(e, ai, AI_POWERGROUP_BUFF);

    if(!entAlive(e) || !getMoveFlags(e->seq, SEQMOVE_CANTMOVE) )
    {
        if (info && info->power && info->power->ppowBase && 
            ABS_TIME_SINCE(info->time.lastUsed) > (SEC_TO_ABS(info->power->ppowBase->fActivatePeriod) + SEC_TO_ABS(info->power->ppowBase->fRechargeTime)))
        {
            if(aiPowerTargetAllowed(e, info, e, true))
            {
                aiUsePower(e, info, e);
                return;
            }
        }

        info = aiChoosePower(e, ai, AI_POWERGROUP_SUMMON);

        if (info && info->power && info->power->ppowBase && 
            ABS_TIME_SINCE(info->time.lastUsed) > (SEC_TO_ABS(info->power->ppowBase->fActivatePeriod) + SEC_TO_ABS(info->power->ppowBase->fRechargeTime)))
        {

            aiUsePower(e, info, NULL);
            return;
        }
    }

    if (aiCritterDoDefaultBuffs(e, ai))
        return;
    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

static void damageTrackerDestroyAdapter(void* arg0)
{
    damageTrackerDestroy((DamageTracker*)arg0);
}

void aiCritterDoArachnosLordRecluseSTF(Entity* e, AIVars* ai)
{
    AITeamMemberInfo* member;
    AIPowerInfo *pInfo = NULL;

    if ((!entAlive(e) || !getMoveFlags(e->seq, SEQMOVE_CANTMOVE)) && (e->pchar))
    {

        // do I need to protect a tower?
        for(member = ai->teamMemberInfo.team->members.list; member; member = member->next)
        {
            Entity* entMember = member->entity;

            // is this a tower?
            if (entMember && ENTAI(entMember)->brain.type == AI_BRAIN_ARACHNOS_WEB_TOWER)
            {
                if (entMember->pchar->attrCur.fHitPoints < entMember->pchar->attrMax.fHitPoints &&
                    entMember->pchar->attrCur.fHitPoints > 0.0f)
                {
                    if (distance3( ENTPOS(e), ENTPOS(entMember) ) > 450.0f)
                    {
                        pInfo = aiCritterFindPowerByName( ai, "Teleport" );
                        if (pInfo && aiCritterArachnosCanIUsePower(e, pInfo))
                        {
                            Vec3 location;
                            copyVec3(ENTPOS(entMember), location);
                            location[1] += 10.0; 
                            location[0] += randomFloat(10.0, 20.0);
                            location[2] += randomFloat(10.0, 20.0);
                            aiUsePowerOnLocation(e, pInfo, location); 
                            
                            // clear aggro
                            if (e->who_damaged_me != NULL)
                                eaClearEx(&e->who_damaged_me, damageTrackerDestroyAdapter);

                            // clear target
                            aiDiscardAttackTarget(e, "aiCritterDoArachnosLordRecluseSTF"); 
                        }
                    } 
                }
            }
        }
    }

    // bah, don't know what to do, so do default things...
    if (aiCritterDoDefaultBuffs(e, ai))
        return;
    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void aiCritterDoArachnosMako(Entity* e, AIVars* ai)
{
    AIPowerInfo *pInfo = NULL;


    if ((!entAlive(e) || !getMoveFlags(e->seq, SEQMOVE_CANTMOVE)) && (e->pchar))
    {
        // Elude, if I'm hurt
        if (getEntityHealthPercent(e) < 0.3f)
        {
             pInfo = aiCritterFindPowerByName( ai, "Elude" );
             if (aiCritterArachnosCanIUsePower(e, pInfo))
                 aiUsePower(e, pInfo, e);
        }

        // Hide if I can
        pInfo = aiCritterFindPowerByName( ai, "Hide" );
        if (aiCritterArachnosCanIUsePower(e, pInfo) && !pInfo->power->bActive)
            aiUsePower(e, pInfo, e);

        // Find me a target


//        aiSetFollowTargetEntity(e, ai, ai->attackTarget.entity, AI_FOLLOWTARGETREASON_ATTACK_TARGET);
//        ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : MAX(preferredAttackDistance - 3, ai->collisionRadius + aiTarget->collisionRadius);

}

    // bah, don't know what to do, so do default things...
    if (aiCritterDoDefaultBuffs(e, ai))
        return;
    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);

}

