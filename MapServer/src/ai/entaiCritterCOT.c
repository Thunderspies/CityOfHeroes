#include "entaiCritterPrivate.h"
#include "entaiprivate.h"
#include "entaiCritterPrivate.h"
#include "beacon/beaconPath.h"
#include "entity/motion.h"
#include "entity/entity.h"
#include "cmdparse/cmdcommon.h"
#include "entity/powers.h"
#include "entity/character_combat.h"
#include "entity/character_base.h"
#include "cmdparse/cmdserver.h"
#include <utilitieslib/utils/mathutil.h>
#include "utils/position.h"
#include "ai/entaiLog.h"

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

void aiCritterDoCotTheHigh(Entity* e, AIVars* ai){
    //    If alone, or if there are more than 2 people attacking him melee, he will use a
    //        suicide attack
    if (ai->attackTarget.entity) {
        bool wantToKillMyself = false;
        int count=0;

        // Check to see if we're alone
        if (ai->teamMemberInfo.team && ai->teamMemberInfo.team->members.aliveCount==1) {
            // I'm the only one alive on my team!  I'm all alone!  I wish I would just die
            wantToKillMyself = true;
        } else {
            count = countEnemiesWithinRange(e, ai, SQR(10), true);
            //printf("%d attackers in melee range.\n", count);
            if (count >= 2)
                wantToKillMyself = true;
        }

        if(wantToKillMyself) {
            AIPowerInfo *info = aiChoosePower(e, ai, AI_POWERGROUP_KAMIKAZE);

            if(info && !info->power->bActive && !info->power->fTimer){
                if (count==0) {
                    float range = aiGetPowerRange(e->pchar, info);
                    // Hasn't been counted
                    count = countEnemiesWithinRange(e, ai, SQR(range), false);
                }
                if (count > 0) {
                    character_ActivatePower(e->pchar, erGetRef(e), ENTPOS(e), info->power);
                } else {
                    ai->preferMelee = 1;
                    ai->preferRanged = 0;
                    ai->power.preferred = info;
                }
            }
        }
    }
    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    if (aiCritterDoDefaultBuffs(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

void aiCritterDoCotCaster(Entity* e, AIVars* ai){
    Character* pchar = e->pchar;
    int groupFlags;

    aiCritterInitPowerGroupFlags(ai);

    groupFlags = ai->power.groupFlags;

    if (!pchar->ppowCurrent && !pchar->ppowQueued) {
        // See if we've been attacked recently in melee
        // if so, use a root power, add some fear, get out of there!
        if (ABS_TIME_SINCE(ai->time.wasAttackedMelee) < SEC_TO_ABS(5) && ABS_TIME_SINCE(ai->brain.cotcaster.time.ranAwayRoot) > 6) {
            // We were recently attacked in melee, and it's been X seconds since we last tried to run away from melee,
            // use a root, add some fear, get out of there!
            if(aiCritterDoBuff(e, ai, &ai->brain.cotcaster.time.ranAwayRoot, &ai->brain.cotcaster.curRoot, AI_POWERGROUP_ROOT, 0, SEC_TO_ABS(5), SEC_TO_ABS(2)))
            {
                // Spike the fear
                aiFeelingAddModifierEx(    e, ai,
                    AI_FEELING_FEAR,
                    1.0,
                    0,
                    ABS_TIME,
                    ABS_TIME + SEC_TO_ABS(3));

                aiFeelingAddModifierEx(    e, ai,
                    AI_FEELING_CONFIDENCE,
                    -1.0,
                    0,
                    ABS_TIME,
                    ABS_TIME + SEC_TO_ABS(3));

                return;
            }
        }

        // Then try debuff powers (normal rules, 'cept not Root powers)
        if (groupFlags & (AI_DEBUFF_FLAGS & ~AI_POWERGROUP_ROOT)) {
            // Timing for debuffs: Do this on in the 0-5 seconds after being attacked
            if (aiCritterDoBuff(e, ai, &ai->power.time.debuff, &ai->power.toggle.curDebuff, AI_DEBUFF_FLAGS & ~AI_POWERGROUP_ROOT, 0, SEC_TO_ABS(5), SEC_TO_ABS(10)))
                return;
        }

        // Then try buff powers (normal rules)
        if (groupFlags & AI_BUFF_FLAGS) {
            // Timing for buffs: Don't do this in the 3 seconds after being attacked, any other time is fine
            if (aiCritterDoBuff(e, ai, &ai->power.time.buff, &ai->power.toggle.curBuff, AI_BUFF_FLAGS, SEC_TO_ABS(3), 0, SEC_TO_ABS(2)))
                return;
        }
    }

    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

void aiCritterDoCotSpectres(Entity* e, AIVars* ai){
    aiCritterDoBanishedPantheonSorrow(e, ai);
}


void aiCritterDoCotBehemoth(Entity* e, AIVars* ai){
    // HtH - Can have multiple targets - pick a primary target. Attack primary, attack someone else in range, 
    //    attack primary...

    // Save primary target in brain
    // set high agro factor on primary
    // toggle flag on primary saying he is notValidTarget

    AIProxEntStatus *primary = ai->brain.cotbehemoth.statusPrimary;
    if (!primary) {
        if (ai->attackTarget.status) {
            // initial choose target
            ai->brain.cotbehemoth.statusPrimary = ai->attackTarget.status;
            ai->brain.cotbehemoth.countWithoutPrimary = 0;
        }
    } else { // has a primary target
        if (ABS_TIME_SINCE(ai->time.didAttack) < ABS_TIME_SINCE(ai->attackTarget.time.set)) {
            // We attacked him, switch to someone new!
            if (ai->attackTarget.status == primary) {
                // Currently targetting our primary, switch if we've attacked him
                AI_LOG(AI_LOG_SPECIAL, (e, "Looks like I attacked my primary, switching to another!\n"));
                primary->invalid.begin = ABS_TIME;
                primary->invalid.duration = UINT_MAX;
                primary->primaryTarget = 0;
                //aiDiscardAttackTarget(e, "Want to target someone other than primary target");
                ai->brain.cotbehemoth.statusPrimary = primary;
                ai->brain.cotbehemoth.countWithoutPrimary = 0;
            } else {
                if (ai->attackTarget.status) {
                    ai->attackTarget.status->primaryTarget = 0; // In case this flag is left over from a previous primary target
                }
                // TODO: count the number of secondary attacks, if > 3, forget primary, find another!
                // We have someone else targetted, switch if we've attacked them
                AI_LOG(AI_LOG_SPECIAL, (e, "Looks like I attacked someone else, switching to primary!\n"));
                primary->invalid.duration = 0;
                primary->primaryTarget = 1;
                //aiDiscardAttackTarget(e, "Want to target primary target");
                ai->brain.cotbehemoth.statusPrimary = primary;
                ai->brain.cotbehemoth.countWithoutPrimary++;
                if (ai->brain.cotbehemoth.countWithoutPrimary>=3) {
                    ai->brain.cotbehemoth.statusPrimary = NULL;
                }
            }
        }
    }
    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    if (aiCritterDoDefaultBuffs(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

