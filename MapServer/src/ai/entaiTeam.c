
#include <utilitieslib/stdtypes.h>
#include "ailib/aiBehaviorInterface.h"    // for the teambase creating and destroying
#include "ailib/aiBehaviorPublic.h"
#include "cmdparse/cmdserver.h"
#include "ai/entaiprivate.h"
#include "ai/entaiLog.h"
#include <utilitieslib/utils/error.h>
#include "entity/motion.h"
#include "seq/seqstate.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "gameSys/dooranim.h"
#include "cmdparse/cmdcommon.h"
#include "entity/powers.h"
#include "entity/character_base.h"
#include "entity/character_combat.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/components/MemoryPool.h>
#include "utils/position.h"
#include <utilitieslib/components/StashTable.h>
#include "gameComm/VillainDef.h"

MP_DEFINE(AITeam);
StashTable aiTeamTable;
U32    nextTeamId = 0;


#define MAX_AITEAM_ID    ((1 << 16) - 1)
static int nextAITeamId(){
    int wrap = 0;
    while (1)
    {
        ++nextTeamId;
        if (nextTeamId > MAX_AITEAM_ID)
        {
            if (wrap)
            {
                Errorf("Ran out of aiteam id's");
                return 0;
            }
            nextTeamId = 1;
            wrap = 1;
        }
        if (!stashIntFindPointer(aiTeamTable, nextTeamId, NULL))
            return nextTeamId;
    }
}

static AITeam* createAITeam(){
    AITeam* team;

    MP_CREATE(AITeam, 50);

    team = MP_ALLOC(AITeam);

    if (!aiTeamTable)
    {
        aiTeamTable = stashTableCreateInt(100);
    }
    team->teamid = nextAITeamId();
    stashIntAddPointer(aiTeamTable, team->teamid, team, false);

    team->proxEntStatusTable = createAIProxEntStatusTable(team, 1);

    team->runOutDoors[0] = NULL;

    team->base = aiSystemGetTeamData();

    return team;
}

static void destroyAITeam(AITeam* team){
    if(!team)
        return;

    destroyAIProxEntStatusTable(team->proxEntStatusTable);

    stashIntRemovePointer(aiTeamTable, team->teamid, NULL);

    aiSystemDestroyTeamData(team->base);

    MP_FREE(AITeam, team);
}

AITeam* aiTeamLookup(int teamid)
{
    AITeam* pResult;
    if (stashIntFindPointer(aiTeamTable, teamid, &pResult))
        return pResult;
    return NULL;
}

static AIBasePowerInfo* getBasePowerInfo(AITeam* team, const BasePower* basePower){
    AIBasePowerInfo* info;

    for(info = team->power.list; info && info->basePower != basePower; info = info->next);

    return info;
}

void aiAddTeamMemberPower(AITeam* team, AITeamMemberInfo* member, AIPowerInfo* info)
{
    AIBasePowerInfo* baseInfo = getBasePowerInfo(team, info->power->ppowBase);

    if(!baseInfo){
        baseInfo = createAIBasePowerInfo();

        baseInfo->basePower = info->power->ppowBase;

        baseInfo->next = team->power.list;
        baseInfo->prev = NULL;

        if(baseInfo->next){
            baseInfo->next->prev = baseInfo;
        }

        team->power.list = baseInfo;
    }

    assert(!info->teamInfo);

    info->teamInfo = baseInfo;

    baseInfo->refCount++;
}

static void addTeamMemberPowers(AITeam* team, AITeamMemberInfo* member){
    AIVars* ai = ENTAI(member->entity);
    AIPowerInfo* info;

    for(info = ai->power.list; info; info = info->next){
        aiAddTeamMemberPower(team, member, info);

        // MS: If I wasn't lazy, I'd update the times in the base info right here.
    }
    // Check to see if powers are in use, if so, inc the inusecount!
    if (ai->power.toggle.curBuff) {
        ai->power.toggle.curBuff->teamInfo->inuseCount++;
    }
    if (ai->power.toggle.curDebuff) {
        ai->power.toggle.curDebuff->teamInfo->inuseCount++;
    }
    if (ai->power.toggle.curPaced) {
        ai->power.toggle.curPaced->teamInfo->inuseCount++;
    }
    if (ai->brain.type == AI_BRAIN_COT_CASTER && ai->brain.cotcaster.curRoot) {
        ai->brain.cotcaster.curRoot->teamInfo->inuseCount++;
    }
    if (ai->brain.type == AI_BRAIN_DEVOURING_EARTH_LT && ai->brain.devouringearth.curPlant) {
        ai->brain.devouringearth.curPlant->teamInfo->inuseCount++;
    }
}

void aiRemoveTeamMemberPower(AITeam* team, AITeamMemberInfo* member, AIPowerInfo* info)
{
    AIVars* ai = ENTAI(member->entity);
    AIBasePowerInfo* baseInfo = info->teamInfo;

    assert(baseInfo);

    if (ai->power.toggle.curBuff == info) {
        aiClearCurrentBuff(&ai->power.toggle.curBuff);
    }
    if (ai->power.toggle.curDebuff == info) {
        aiClearCurrentBuff(&ai->power.toggle.curDebuff);
    }
    if (ai->power.toggle.curPaced == info) {
        aiClearCurrentBuff(&ai->power.toggle.curPaced);
    }
    if (ai->brain.type == AI_BRAIN_COT_CASTER && ai->brain.cotcaster.curRoot == info) {
        aiClearCurrentBuff(&ai->brain.cotcaster.curRoot);
    }
    if (ai->brain.type == AI_BRAIN_DEVOURING_EARTH_LT && ai->brain.devouringearth.curPlant == info) {
        aiClearCurrentBuff(&ai->brain.devouringearth.curPlant);
    }

    if(!--baseInfo->refCount){
        assert(baseInfo->inuseCount == 0);
        if(baseInfo->prev){
            baseInfo->prev->next = baseInfo->next;
        }else{
            team->power.list = baseInfo->next;
        }

        if(baseInfo->next){
            baseInfo->next->prev = baseInfo->prev;
        }

        destroyAIBasePowerInfo(baseInfo);
    }

    info->teamInfo = NULL;
}

    
static void removeTeamMemberPowers(AITeam* team, AITeamMemberInfo* member){
    AIVars* ai = ENTAI(member->entity);
    AIPowerInfo* info;

    for(info = ai->power.list; info; info = info->next){
        aiRemoveTeamMemberPower(team, member, info);
    }
}

static void addTeamMember(AITeam* team, AITeamMemberInfo* member)
{
    AITeamMemberInfo* prevMember = NULL;
    assert(!member->team);
    if(!team)
    {
        team = createAITeam();
        team->members.list = member;
    }
    else
    {
        prevMember = team->members.list;
        while( prevMember && prevMember->next )
        {
            prevMember = prevMember->next;
            if( prevMember == member || prevMember->next == member )
                return; // we are already on the team
        }
    }

    member->team = team;
    member->next = NULL;
    member->prev = prevMember;

    if(prevMember)
        prevMember->next = member;

    team->members.totalCount++;
    // if we get over 1000, aiTeamSetRoles is going to crash.  We also just don't want teams this big.
    assert(team->members.totalCount < 1000); 
    team->time.lastSetRoles = 0;

    // Add this member's powers.
    addTeamMemberPowers(team, member);
    AI_LOG(AI_LOG_TEAM, (member->entity, "Joined a new team... it now has %d members\n", team->members.totalCount));
}

static void removeTeamMember(AITeamMemberInfo* member){
    AITeam* team = member->team;

    if(!team)
        return;

    AI_LOG(AI_LOG_TEAM, (member->entity, "Leaving my team of %d members (including me)\n", team->members.totalCount));

    assert(team->members.totalCount > 0);

    removeTeamMemberPowers(team, member);

    if(--team->members.totalCount == 0){
        destroyAITeam(team);
    }else{
        if(member->prev)
            member->prev->next = member->next;
        else
            team->members.list = member->next;

        if(member->next)
            member->next->prev = member->prev;
    }

    member->team = NULL;
    member->next = NULL;
    member->prev = NULL;

    member->orders.role = AI_TEAM_ROLE_NONE;
    member->orders.targetStatus = NULL;
}

void aiTeamEntityRemoved(AITeam* team, Entity* removedEnt){
    AITeamMemberInfo* member;
    AIProxEntStatus* status;

    if(!team)
        return;

    status = aiGetProxEntStatus(team->proxEntStatusTable, removedEnt, 0);

    if(status){
        for(member = team->members.list; member; member = member->next){
            if(member->orders.targetStatus == status){
                member->orders.role = AI_TEAM_ROLE_NONE;
                member->orders.targetStatus = NULL;
            }
        }

        aiRemoveProxEntStatus(team->proxEntStatusTable, removedEnt, 0, 1);
    }
}

void aiTeamJoin(AITeamMemberInfo* memberToJoin, AITeamMemberInfo* newMember){
    if(memberToJoin == newMember && memberToJoin->team)
        return;

    //Don't team with a crate.
    if( !memberToJoin->entity->pchar || !newMember->entity->pchar )
        return;

    if (memberToJoin->team && memberToJoin->team->patrolDirection)
    {
        Entity *e = newMember->entity;
        if (e)
        {
            AIVars *ai = ENTAI(e);
            int behaviorHandle = aiBehaviorGetTopBehaviorByName(e, ai->base, "Patrol");
            if (behaviorHandle)
            {
                aiBehaviorMarkFinished(e, ai->base, behaviorHandle);
            }
        }
    }
    removeTeamMember(newMember);

    assert(!newMember->team);

    if(!memberToJoin->team){
        addTeamMember(NULL, memberToJoin);
    }

    assert(memberToJoin->team);

    if(memberToJoin != newMember){
        addTeamMember(memberToJoin->team, newMember);
    }
}

void aiTeamLeave(AITeamMemberInfo* member){
    removeTeamMember(member);
}

static AITeamMemberInfo* aiTeamGetClosestMember(AITeam* team, const Vec3 pos){
    AITeamMemberInfo* member;
    AITeamMemberInfo* bestMember = NULL;
    float bestDistSQR = FLT_MAX;

    for(member = team->members.list; member; member = member->next){
        float distSQR = distance3Squared(ENTPOS(member->entity), pos);

        if(distSQR < bestDistSQR){
            bestMember = member;
            bestDistSQR = distSQR;
        }
    }

    return bestMember;
}

static int aiTeamCompareDangerValues(const void* status1Data, const void* status2Data){
    const AIProxEntStatus** status1 = (const AIProxEntStatus**)status1Data;
    const AIProxEntStatus** status2 = (const AIProxEntStatus**)status2Data;
    float diff = (*status1)->dangerValue - (*status2)->dangerValue;

    // Sort from high to low.

    if(diff > 0)
        return -1;
    else if(diff == 0)
        return 0;
    else
        return 1;
}

static int aiTeamCompareMemberTargetRelation(const void* member1Data, const void* member2Data){
    const AITeamMemberInfo** member1 = (const AITeamMemberInfo**)member1Data;
    const AITeamMemberInfo** member2 = (const AITeamMemberInfo**)member2Data;
    int priority_diff = (*member1)->assignTargetPriority - (*member2)->assignTargetPriority;

    // Sort from high to low priority.

    if(priority_diff > 0){
        return -1;
    }
    else if (priority_diff < 0){
        return 1;
    }else{
        float diff = (*member1)->distanceToCurTargetSQR - (*member2)->distanceToCurTargetSQR;

        // Sort from low to high distance.

        if(diff > 0)
            return 1;
        else if(diff == 0)
            return 0;
        else
            return -1;
    }
}

void aiTeamSetRoles(AITeam* team){
    int i;
    AITeamMemberInfo* member;
    AIProxEntStatus* teamStatus;
    float totalDanger;
    float totalTeamHP;
    int targetCount = 0;
    AIProxEntStatus* orderedTargets[1000];
    float maxProximityBothered = 0;
    float maxEventMemoryDuration = 0;
    int didAssignToTargets = 0;
    Vec3 spawnCenter = {0,0,0};
    int spawnCount = 0;
    int wasAttacked = team->wasAttacked;
    U32 timeFirstAttacked = wasAttacked ? team->time.firstAttacked : 0;
    int bossRank = villainRankConningAdjustment(VR_BOSS);
    bool hasArchVillain = false;

    // Reset all the status values that are currently in the table.

    for(teamStatus = team->proxEntStatusTable->statusHead; teamStatus; teamStatus = teamStatus->tableList.next){
        teamStatus->damage.toMe = 0;
        teamStatus->damage.toMeCount = 0;
        teamStatus->damage.fromMe = 0;
        teamStatus->hasAttackedMe = 0;
        teamStatus->time.attackMe = 0;
        teamStatus->assignedAttackers = 0;
        teamStatus->time.posKnown = 0;
    }

    // Check all the team-members' status tables for values.

    team->members.aliveCount = 0;
    team->members.underGriefedHPCount = 0;

    for(member = team->members.list; member; member = member->next){
        Entity* e = member->entity;
        AIVars* ai = ENTAI(e);
        AIProxEntStatus* status;
        float bothered = ai->feeling.sets[AI_FEELINGSET_PROXIMITY].level[AI_FEELING_BOTHERED];
        Character* pchar = e->pchar;
        int memberAlive = pchar->attrCur.fHitPoints > 0;
        //int memberSleeping = pchar->attrCur.fSleep > 0;

        member->orders.role = AI_TEAM_ROLE_NONE;
        member->orders.targetStatus = NULL;

        addVec3(ai->spawn.pos, spawnCenter, spawnCenter);
        spawnCount++;

        if(pchar->attrCur.fHitPoints > 0){
            team->members.aliveCount++;
        }

        // Check if this team-member is unable to share information.

        if(member->doNotShareInfoWithMembers){
            continue;
        }

        // Check if this guys is an arch villain

        if(!hasArchVillain && villainRankConningAdjustment(member->entity->villainDef->rank) > bossRank)
            hasArchVillain = true;

        // See if I have the highest bothered feeling.

        if(bothered > maxProximityBothered){
            maxProximityBothered = bothered;
        }

        // See if I have the highest memory duration.

        if(ai->eventMemoryDuration > maxEventMemoryDuration){
            maxEventMemoryDuration = ai->eventMemoryDuration;
        }

        // Calculate my griefed HP ratio.

        if(    ai->griefed.hpRatio &&
            pchar->attrMax.fHitPoints &&
            pchar->attrCur.fHitPoints / pchar->attrMax.fHitPoints < ai->griefed.hpRatio)
        {
            member->underGriefedHPRatio = 1;
            team->members.underGriefedHPCount++;
        }
        else
        {
            member->underGriefedHPRatio = 0;
        }

        // Check if I was attacked earliest.

        if(ai->wasAttacked){
            if(!wasAttacked || ABS_TIME_SINCE(ai->time.firstAttacked) > ABS_TIME_SINCE(timeFirstAttacked)){
                timeFirstAttacked = ai->time.firstAttacked;
            }

            wasAttacked = 1;
        }

        // Run through my status table and do stuff.

        for(status = ai->proxEntStatusTable->statusHead; status; status = status->tableList.next){
            if(!status->entity || !status->entity->pchar ||
                status->entity->pchar->attrCur.fHitPoints <= 0)
            {
                continue;
            }

            teamStatus = aiGetProxEntStatus(team->proxEntStatusTable, status->entity, 1);

            // Add the damage done.

            if(status->damage.toMe > 0){
                teamStatus->damage.toMe += status->damage.toMe;
                teamStatus->damage.toMeCount++;
            }

            teamStatus->damage.fromMe += status->damage.fromMe;

            // Get the latest attack time.

            if(ABS_TIME_SINCE(status->time.attackMe) < ABS_TIME_SINCE(teamStatus->time.attackMe)){
                teamStatus->time.attackMe = status->time.attackMe;
            }

            // Get the latest seen time.

            if(1 /*&& memberAlive && !memberSleeping*/){
                U32 timeSince = ABS_TIME_SINCE(status->time.posKnown);

                if(    timeSince < ai->eventMemoryDuration &&
                    timeSince < ABS_TIME_SINCE(teamStatus->time.posKnown))
                {
                    teamStatus->time.posKnown = status->time.posKnown;
                }
            }

            // Get the attacked flag.

            if(/*memberAlive && !memberSleeping &&*/ status->hasAttackedMe){
                teamStatus->hasAttackedMe = 1;
            }
        }
    }

    scaleVec3(spawnCenter, 1.0 / spawnCount, spawnCenter);
    copyVec3(spawnCenter, team->spawnCenter);

    if(maxEventMemoryDuration <= 0){
        maxEventMemoryDuration = 0.1;
    }

    team->wasAttacked = wasAttacked;
    team->time.firstAttacked = timeFirstAttacked;

    // Add up the total HP of the team.

    team->totalMaxHP = 0;
    totalTeamHP = 0;

    for(member = team->members.list; member; member = member->next){
        Entity* e = member->entity;
        AIVars* ai = ENTAI(e);
        Character* pchar = e->pchar;

        team->totalMaxHP += pchar->attrMax.fHitPoints;
        totalTeamHP += pchar->attrCur.fHitPoints;

        if(ai->feeling.sets[AI_FEELINGSET_PROXIMITY].level[AI_FEELING_BOTHERED] < maxProximityBothered)
            ai->feeling.sets[AI_FEELINGSET_PROXIMITY].level[AI_FEELING_BOTHERED] = maxProximityBothered;
    }

    // Rank targets by priority.

    totalDanger = 0;

    for(teamStatus = team->proxEntStatusTable->statusHead; teamStatus; teamStatus = teamStatus->tableList.next){
        float damageFactor;
        float distance;
        float distanceFactor;
        float attackTimeFactor;
        float threatFactor;
        AITeamMemberInfo* closestMember;
        AIVars* aiTarget;
        Entity* target;
        Character* pcharTarget;

        target = teamStatus->entity;
        pcharTarget = target->pchar;

        if(    !target ||
            pcharTarget->attrCur.fHitPoints <= 0 ||
            target->untargetable ||
            ABS_TIME_SINCE(teamStatus->time.posKnown) > maxEventMemoryDuration)
        {
            continue;
        }

        aiTarget = ENTAI(target);

        if(    aiTarget &&
            aiTarget->teamMemberInfo.team == team)
        {
            continue;
        }

        if(teamStatus->damage.toMeCount)
            teamStatus->damage.toMe /= teamStatus->damage.toMeCount;

        teamStatus->damage.toMeFromMeRatio =    (teamStatus->damage.toMe + (team->totalMaxHP / team->members.totalCount)) /
                                                (teamStatus->damage.fromMe + pcharTarget->attrMax.fHitPoints);

        // DAMAGE TO TEAM Factor. *********************************************

        damageFactor = teamStatus->damage.toMeFromMeRatio * (0.1 + 0.9 * teamStatus->damage.toMe / team->totalMaxHP);

        // CLOSEST TEAMMATE Factor. *********************************************

        closestMember = aiTeamGetClosestMember(team, ENTPOS(teamStatus->entity));

        if(!closestMember)
            continue;

        distance = distance3(ENTPOS(closestMember->entity), ENTPOS(teamStatus->entity));

        distanceFactor = (300.0 - distance) / 300.0;

        if(distanceFactor < 0)
            continue;

        // Factor in the last time this guy attacked a team member.

        if(ABS_TIME_SINCE(teamStatus->time.attackMe) < maxEventMemoryDuration){
            attackTimeFactor = 0.1 + 0.9 * (float)(maxEventMemoryDuration - ABS_TIME_SINCE(teamStatus->time.attackMe)) / maxEventMemoryDuration;
        }else{
            attackTimeFactor = 0.1;
        }

        // THREAT LEVEL factor. *********************************************

        if(!hasArchVillain)
            threatFactor = pcharTarget->attrCur.fThreatLevel;
        else
            threatFactor = 1;

        // Combine all factors to make total danger value.

        teamStatus->dangerValue =    damageFactor *
                                    distanceFactor *
                                    attackTimeFactor *
                                    threatFactor;

        if(teamStatus->dangerValue > 0 && targetCount < ARRAY_SIZE(orderedTargets)){
            // Add danger value to total danger.

            totalDanger += teamStatus->dangerValue;

            // Add to the ordered targets array.

            orderedTargets[targetCount++] = teamStatus;
        }
    }

    // Sort the targets by danger value.

    qsort(orderedTargets, targetCount, sizeof(orderedTargets[0]), aiTeamCompareDangerValues);

    // Assign team members to targets.

    for(i = 0; i < targetCount; i++){
        int j;
        float dangerPercent;
        float initialDangerPercent;
        float likelyDamageBeforeDeath;
        float hpToAssignMelee;
        float hpToAssignRanged;
        int assignedMeleeCount = 0;
        int memberCount = 0;
        AITeamMemberInfo* teamArray[1000];
        Entity* target;
        Character* pcharTarget;

        teamStatus = orderedTargets[i];

        target = teamStatus->entity;
        pcharTarget = target->pchar;

        dangerPercent = teamStatus->dangerValue / totalDanger;
        initialDangerPercent = dangerPercent;

        likelyDamageBeforeDeath = dangerPercent * teamStatus->damage.toMeFromMeRatio * pcharTarget->attrCur.fHitPoints;

        hpToAssignMelee = likelyDamageBeforeDeath * teamStatus->entity->pchar->attrCur.fThreatLevel;
        hpToAssignRanged = likelyDamageBeforeDeath;

        // Set distance to current target for each remaining team member.

        for(member = team->members.list; member; member = member->next){
            Entity* e = member->entity;
            Character* pchar = e->pchar;
            AIVars* ai = ENTAI(e);

            if(    !member->alerted ||
                member->orders.role != AI_TEAM_ROLE_NONE ||
                pchar->attrCur.fHitPoints <= 0 ||
                pchar->attrCur.fSleep > 0 ||
                character_IsConfused(pchar) ||
                pchar->attrCur.fHeld > 0 ||
                pchar->attrCur.fStunned > 0)
            {
                continue;
            }

            member->distanceToCurTargetSQR = distance3Squared(ENTPOS(teamStatus->entity), ENTPOS(member->entity));

            teamArray[memberCount++] = member;

            assert(memberCount < 1000); // gonna crash anyways if this occurs, so assert in production to make the problem obvious
        }

        // If there's no one left to assign, then stop.

        if(!memberCount)
            break;

        // Sort the team list by distance from current target.

        qsort(teamArray, memberCount, sizeof(teamArray[0]), aiTeamCompareMemberTargetRelation);

        // Assign guys until enough have been assigned to the target.

        for(j = 0; (hpToAssignMelee > 0 || hpToAssignRanged > 0) && j < memberCount; j++){
            float hp;
            float pctHP;
            int useMelee;
            AIVars* ai;
            AIProxEntStatus* myStatus;

            member = teamArray[j];
            hp = member->entity->pchar->attrCur.fHitPoints;
            pctHP = hp / totalTeamHP;
            ai = ENTAI(member->entity);

            useMelee =    member->doNotAssignToRanged ||
                        (    assignedMeleeCount < 4
                            &&
                            !member->doNotAssignToMelee
                            &&
                            !ai->preferRanged
                            &&
                            (    ai->preferMelee
                                ||
                                ABS_TIME_SINCE(teamStatus->time.attackMe) < SEC_TO_ABS(10)));

            if(useMelee)
                assignedMeleeCount++;

            if(    useMelee && hpToAssignMelee <= 0 ||
                !useMelee && hpToAssignRanged <= 0)
            {
                continue;
            }

            member->orders.role = useMelee ? AI_TEAM_ROLE_ATTACKER_MELEE : AI_TEAM_ROLE_ATTACKER_RANGED;
            member->orders.targetStatus = teamStatus;

            teamStatus->assignedAttackers++;

            // Set my lastSeenTime.

            myStatus = aiGetProxEntStatus(ai->proxEntStatusTable, teamStatus->entity, 1);

            if(ABS_TIME_SINCE(teamStatus->time.posKnown) < ABS_TIME_SINCE(myStatus->time.posKnown)){
                myStatus->time.posKnown= teamStatus->time.posKnown;
            }

            if(ABS_TIME_SINCE(teamStatus->time.attackMe) < ABS_TIME_SINCE(myStatus->time.attackMyFriend)){
                myStatus->time.attackMyFriend = teamStatus->time.attackMe;
            }

            if(useMelee)
                hpToAssignMelee -= hp * 2;
            else
                hpToAssignRanged -= hp * 2 / ai->preferRangedScale;

            if(!ai->attackTarget.entity)
                ai->time.checkedProximity = 0;

            didAssignToTargets = 1;
        }
    }

    // Assign the rest of the members to ranged.

    for(member = team->members.list; member; member = member->next){
        Entity* e = member->entity;
        Character* pchar = e->pchar;
        AIVars* ai = ENTAI(e);
        float bestDistSQR;
        int bestTarget;

        if(    !member->alerted ||
            member->orders.role != AI_TEAM_ROLE_NONE ||
            pchar->attrCur.fHitPoints <= 0 ||
            pchar->attrCur.fSleep > 0 ||
            character_IsConfused(pchar) ||
            pchar->attrCur.fHeld > 0 ||
            pchar->attrCur.fStunned > 0)
        {
            continue;
        }

        bestDistSQR = FLT_MAX;
        bestTarget = -1;

        for(i = 0; i < targetCount; i++){
            if(ABS_TIME_SINCE(orderedTargets[i]->time.attackMe) < SEC_TO_ABS(20)){
                Entity* target = orderedTargets[i]->entity;
                float distSQR = distance3Squared(ENTPOS(e), ENTPOS(target));

                if(distSQR < bestDistSQR){
                    bestDistSQR = distSQR;
                    bestTarget = i;
                }
            }
        }

        if(bestTarget >= 0){
            AIProxEntStatus* teamStatus = orderedTargets[bestTarget];
            AIProxEntStatus* myStatus;

            member->orders.role = AI_TEAM_ROLE_ATTACKER_RANGED;
            member->orders.targetStatus = teamStatus;

            teamStatus->assignedAttackers++;

            myStatus = aiGetProxEntStatus(ai->proxEntStatusTable, teamStatus->entity, 1);

            if(ABS_TIME_SINCE(teamStatus->time.posKnown) < ABS_TIME_SINCE(myStatus->time.posKnown)){
                myStatus->time.posKnown = teamStatus->time.posKnown;
            }

            if(ABS_TIME_SINCE(teamStatus->time.attackMe) < ABS_TIME_SINCE(myStatus->time.attackMyFriend)){
                myStatus->time.attackMyFriend = teamStatus->time.attackMe;
            }
        }
    }

    if(!didAssignToTargets){
        team->time.lastSetRoles = ABS_TIME - SEC_TO_ABS(1.5);
        team->time.lastThink = ABS_TIME - SEC_TO_ABS(0.5);
    }
}

void aiTeamThink(AITeamMemberInfo* member){
    AITeam* team = member->team;

    if(!team)
        return;

    if(ABS_TIME_SINCE(team->time.lastThink) < SEC_TO_ABS(1))
        return;

    team->time.lastThink = ABS_TIME;

    if(team->teamMemberWantsToRunOut)
        aiTeamRunOutOfDoor(team);

    if(ABS_TIME_SINCE(team->time.lastSetRoles) > SEC_TO_ABS(2)){
        team->time.lastSetRoles = ABS_TIME;

        PERFINFO_AUTO_START("teamSetRoles", 1);
            aiTeamSetRoles(team);
        PERFINFO_AUTO_STOP();
    }
}


// consider the leader to be the first entity in the member list
Entity * aiTeamGetLeader(AIVars * ai)
{
    if(ai && ai->teamMemberInfo.team)
        return ai->teamMemberInfo.team->members.list->entity;
    else
        return 0; // no team/leader
}

int architectbuffer_choosenOneId = -1; // once a buffer is chosen, no one else can assume the role
Entity * aiGetArchitectBuffer(AIVars *ai)
{
    if( ai && ai->teamMemberInfo.team && !ai->doNotUsePowers )
    {
        AITeam *team = ai->teamMemberInfo.team;
        AITeamMemberInfo * member;

        for(member = team->members.list; member; member = member->next)
        {
            if( member->entity && member->entity->petByScript )
            {
                Entity *e = member->entity;
                if( architectbuffer_choosenOneId < 1 )
                {
                    int i;
                    architectbuffer_choosenOneId = e->id;
                    character_RestoreAutoPowers(e);
                    character_ActivateAllAutoPowers(e->pchar);
                    for (i = 0; i < e->petList_size; ++i)
                    {
                        Entity *pet = erGetEnt(e->petList[i]);
                        if (pet)
                        {
                            character_RestoreAutoPowers(pet);
                            character_ActivateAllAutoPowers(pet->pchar);
                        }
                    }
                }

                if(architectbuffer_choosenOneId == e->id)
                    return e;
            }
        }
        return 0;
    }
    else
        return 0; 
}

void aiTeamRunOutOfDoor(AITeam* team)
{
    int i;
    int doorsUsed[MAX_RUN_OUT_DOORS];
    AITeamMemberInfo* member;

    team->teamMemberWantsToRunOut = 0;    // set to 1 if someone can't run out and wants to later

//    printf("starting run out...\n");

    ZeroMemory(doorsUsed, MAX_RUN_OUT_DOORS * sizeof(int));
    if(!team->runOutDoors[0])
    {
        if(!DoorAnimFindGroupFromPoint(team->runOutDoors, DoorAnimFindPoint(ENTPOS(team->members.list->entity),
            DoorAnimPointExit, MAX_DOOR_SEARCH_DIST), &team->numDoors, MAX_RUN_OUT_DOORS))
        {
            for(member = team->members.list; member; member = member->next)
            {
                AIVars* ai=0;
                
                if( member->entity )
                    ai = ENTAI(member->entity);

                if( ai )
                {
                    int behaviorHandle = aiBehaviorGetTopBehaviorByName(member->entity, ai->base, "RunOutOfDoor");
                    if (behaviorHandle)
                    {
                        aiBehaviorMarkFinished(member->entity, ai->base, behaviorHandle);
                    }
                }

                member->currentlyRunningOutOfDoor =0;
                member->wantToRunOutOfDoor = 0;
                member->waiting = 0;
            }
            team->teamMemberWantsToRunOut = 0;
            return;
        }
    }

    for(i = 0, member = team->members.list; member; member = member->next, i++)
    {
        if(member->currentlyRunningOutOfDoor || member->waiting)
        {
            if(!(getMoveFlags(member->entity->seq, SEQMOVE_WALKTHROUGH)) && !member->waiting)
                member->currentlyRunningOutOfDoor = 0;
            else
                doorsUsed[i % team->numDoors] = 1;

            team->teamMemberWantsToRunOut = 1;
        }
        if(member->currentlyRunningOutOfDoor || member->waiting || member->wantToRunOutOfDoor)
            member->entity->motion->falling = 0;
        else
            member->entity->motion->falling = !member->entity->motion->input.flying;
    }

    for(i = 0, member = team->members.list; member; member = member->next, i++)
    {
        if(member->wantToRunOutOfDoor)
        {
            if(!member->currentlyRunningOutOfDoor)
            {
                member->myDoor = team->runOutDoors[i % team->numDoors];
                member->runOutTime = ABS_TIME + SEC_TO_ABS(((double)rand())/RAND_MAX);
                member->waiting = 1;
                doorsUsed[i % team->numDoors]++;
                member->currentlyRunningOutOfDoor = 1;
                member->wantToRunOutOfDoor = 0;
                team->teamMemberWantsToRunOut = 1;
            }
        }
    }

    if(!team->teamMemberWantsToRunOut)
        team->runOutDoors[0] = NULL;
}

