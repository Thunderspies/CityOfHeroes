#include "entaiprivate.h"
#include "entaiCritterPrivate.h"
#include "beacon/beaconPath.h"
#include "beacon/beaconPrivate.h"
#include "entity/motion.h"
#include "gameComm/VillainDef.h"
#include "seq/seqstate.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "gridcoll/megaGrid.h"
#include "cmdparse/cmdcommon.h"
#include "entity/powers.h"
#include "entity/character_base.h"
#include "cmdparse/cmdserver.h"
#include "ailib/aiBehaviorPublic.h"
#include "entaiBehaviorCoh.h"
#include "entaiLog.h"
#include "entai.h"
#include <utilitieslib/utils/timing.h>
#include "entity/entityRef.h"
#include "entity/character_combat.h"
#include "entity/entGameActions.h"
#include "ai/entaiScript.h"
#include "entity/character_target.h"

#include "svr/svr_base.h"
#include "comm_game.h"
#include <utilitieslib/network/net_packetutil.h>
#include <utilitieslib/network/net_packet.h>
static float randomFloat(float min, float max)
{
    U32 rand = rule30Rand();
    F32 dif = max - min;
    F32 at = dif * rand / UINT_MAX;
    return min + at;
}

static void serveDebugFloatMessage(Entity *victim, char *msg)
{
	#define LIST_SIZE   ( MAX_ENTITIES_PRIVATE )
	#define RADIUS_TO_SEARCH ( 100 )
	int i=0, list[ LIST_SIZE ];

	buildCloseEntList(NULL, victim, RADIUS_TO_SEARCH, list, LIST_SIZE, TRUE); // true means send info to yourself

	while( list[i] != -1 && i < LIST_SIZE )
	{
		Entity  *e = entities[ list[i] ];

		if(ENTTYPE_BY_ID(list[i]) == ENTTYPE_PLAYER
			&& e->client
			&& e->client->ready >= CLIENTSTATE_REQALLENTS
			&& e->client->entDebugInfo == ENTDEBUGINFO_RUNNERDEBUG)
		{
			START_PACKET(pak1, e, SERVER_SEND_FLOATING_DAMAGE);
			pktSendBitsPack(pak1, 1, victim->owner);
			pktSendBitsPack(pak1, 1, victim->owner);
			pktSendBitsPack(pak1, 1, 0);
			pktSendString(pak1, msg);
			pktSendBits(pak1, 1, 0); // sendloc
			pktSendBits(pak1, 1, 1); // wasAbsorb
			END_PACKET
		}

		i++;
	}
}

static void aiCritterActivityCombatChoosePower(Entity* e, AIVars* ai){
    if(ai->doNotChangePowers)
        return;

    if(!ai->attackTarget.entity)
        return;

    if(ABS_TIME_SINCE(ai->time.chosePower) < SEC_TO_ABS(2)){
        return;
    }

    // IF: I don't have an active or queued power,
    // THEN: Choose a current and preferred power.

    if(!e->pchar->ppowCurrent && !e->pchar->ppowQueued)
    {
        AIPowerInfo* newCurrent;
        AIPowerInfo* newPreferred;
        AIVars* aiTarget;
        float distance;
        int stanceID;
        int inRangeRecently;
        int changedStanceRecently;
        int requireRange=0;

        inRangeRecently = ABS_TIME_SINCE(ai->time.lastInRange) < (SEC_TO_ABS(3) * ai->preferMeleeScale);
        changedStanceRecently = ABS_TIME_SINCE(ai->time.setCurrentStance) < SEC_TO_ABS(5);

        // IF: I changed stance recently,
        // OR: Me or my friend has been attacked in the last five seconds,
        //   AND: I've been in range in the last five seconds,
        // THEN: Use a power with the current stance.

        if(    changedStanceRecently ||
            ABS_TIME_SINCE(ai->attackTarget.status->time.attackMe) > SEC_TO_ABS(5) &&
            ABS_TIME_SINCE(ai->attackTarget.status->time.attackMyFriend) > SEC_TO_ABS(5) &&
            inRangeRecently)
        {
            stanceID = ai->power.lastStanceID;
        }
        else
        {
            stanceID = 0;
        }

        aiTarget = ENTAI(ai->attackTarget.entity);

        distance =    distance3(ENTPOS(e), ENTPOS(ai->attackTarget.entity)) -    aiTarget->collisionRadius;

        if (inRangeRecently && ai->preferMelee && distance <= 8.5f) 
        {
            requireRange = 1;
        }

        newCurrent = aiChooseAttackPower(    e, ai,
                                            distance,
                                            distance + 10,
                                            requireRange,
                                            AI_POWERGROUP_BASIC_ATTACK | AI_POWERGROUP_SECONDARY_TARGET,
                                            changedStanceRecently || inRangeRecently ? stanceID : 0);

        if (newCurrent && newCurrent->power && newCurrent->power->ppowBase && newCurrent->groupFlags & AI_POWERGROUP_SECONDARY_TARGET)
        {
            int success = 0;
            Entity *secondaryTarget = aiGetSecondaryTarget(e, ai, newCurrent->power);
            if (secondaryTarget)
            {
                TargetType targetType = newCurrent->power->ppowBase->eTargetType;
                switch (targetType)
                {
                case kTargetType_Location:
                case kTargetType_Teleport:
                    success = aiUsePowerOnLocation(e, newCurrent, ENTPOS(secondaryTarget));
                    break;
                default:
                    success = aiUsePower(e, newCurrent, secondaryTarget);
                    break;
                }
            }
            if (!success)
            {
                newCurrent->doNotChoose.startTime = ABS_TIME;
                newCurrent->doNotChoose.duration = SEC_TO_ABS(3);
            }
        }
        else
        {
            if (inRangeRecently &&    ai->preferMelee &&
                newCurrent && (aiGetPowerRange(e->pchar, newCurrent) > 10.f) &&
                ai->power.current && (aiGetPowerRange(e->pchar, ai->power.current) <= 10.f))
            {
                // We have been in range recently, but we are choosing a ranged power (perhaps out of range for a moment?)
                // Don't use it!
                newCurrent = ai->power.current;
            }

            // IF: I don't have a new power to use,
            // OR: I don't have a team target,
            //   AND: My target has attacked me in the last X seconds (scaled),
            // OR: I'm confused,
            //   AND: Some random factor is true,
            // OR: I'm not griefed,
            //   AND: I'm not afraid,
            //     OR: I've been afraid for more than ten seconds,
            //   AND: I have a team target,
            //   AND: I'm supposed to be melee,
            //   AND: My team target is my current target,
            // THEN: Prefer a melee power,
            // ELSE: Prefer my current power.

            if(    !newCurrent
                ||
                (    !ai->teamMemberInfo.orders.targetStatus &&
                ABS_TIME_SINCE(ai->attackTarget.status->time.attackMe) < (SEC_TO_ABS(20)*ai->preferMeleeScale))
                ||
                (    character_IsConfused(e->pchar) &&
                rand() & 1)
                ||
                (    !ai->isGriefed &&
                (    !ai->isAfraidFromAI &&
                !ai->isAfraidFromPowers
                ||
                ABS_TIME_SINCE(ai->time.begin.afraidState) > SEC_TO_ABS(10)) &&
                ai->teamMemberInfo.orders.targetStatus &&
                ai->teamMemberInfo.orders.role == AI_TEAM_ROLE_ATTACKER_MELEE &&
                ai->teamMemberInfo.orders.targetStatus->entity == ai->attackTarget.entity))
            {
                if (ai->preferMelee) {
                    newPreferred = aiChooseAttackPower(e, ai, 0, 20, 1, AI_POWERGROUP_BASIC_ATTACK, 0);
                } else {
                    newPreferred = aiChooseAttackPower(e, ai, 0, 20, 0, AI_POWERGROUP_BASIC_ATTACK, 0);
                }
            }else{
                if(inRangeRecently){
                    newPreferred = newCurrent;
                }else{
                    newPreferred = aiChooseAttackPower(e, ai, distance, distance + 10, 0, AI_POWERGROUP_BASIC_ATTACK, 0);
                }
            }

            if(    AI_LOG_ENABLED(AI_LOG_COMBAT) &&
                (    ai->power.preferred != newPreferred ||
                ai->power.current != newCurrent))
            {
                AI_LOG(    AI_LOG_COMBAT,
                    (e,
                    "Chose powers: ^2Cur:^5%s^0/^1Pref:^5%s^0, ^4%d^0s since in range\n",
                    newCurrent ? newCurrent->power->ppowBase->pchName : "NONE",
                    newPreferred ? newPreferred->power->ppowBase->pchName : "NONE",
                    ABS_TO_SEC(ABS_TIME_SINCE(ai->time.lastInRange))));
            }

            ai->power.preferred = newPreferred;

            ai->time.chosePower = ABS_TIME;

            if(newCurrent){
                int useNew = 0;

                if(ABS_TIME_SINCE(ai->time.lastInRange) > (SEC_TO_ABS(10)*ai->preferMeleeScale)){
                    useNew = 1;
                }else{
                    float rangeOld = ai->power.current ? aiGetPowerRange(e->pchar, ai->power.current) : 0;
                    float rangeNew = aiGetPowerRange(e->pchar, newCurrent);

                    if(rangeNew < rangeOld * 0.75 || rangeNew > rangeOld){
                        useNew = 1;
                    }
                }

                if(useNew){
                    aiSetCurrentPower(e, newCurrent, 0, 0);
                }
            }
        }
    }else{
        ai->time.lastInRange = ABS_TIME;
        ai->time.setCurrentStance = ABS_TIME;
    }
}

static void aiCritterActivityCombatCheckPathTarget(Entity* e, AIVars* ai){
    if(ai->navSearch->path.head){
        NavPathWaypoint* lastWaypoint = ai->navSearch->path.tail;

        int isTaunted = ai->attackTarget.status &&
                        ai->attackTarget.status->taunt.duration &&
                        ABS_TIME_SINCE(ai->attackTarget.status->taunt.begin) < ai->attackTarget.status->taunt.duration;
                        
        // Get some distances.

        if(    ai->isAfraidFromAI && !isTaunted ||
            ai->isAfraidFromPowers ||
            ai->isGriefed)
        {
            if(ai->attackTarget.entity){
                float distFromTargetToPathEndSQR =
                    distance3Squared(    ENTPOS(ai->attackTarget.entity),
                                        lastWaypoint->pos);

                float distToPathEndSQR =
                    distance3Squared(    ENTPOS(e),
                                        lastWaypoint->pos);

                if(    ABS_TIME_SINCE(ai->time.foundPath) > SEC_TO_ABS(1) &&
                    distFromTargetToPathEndSQR < distToPathEndSQR)
                {
                    aiSendMessage(e, AI_MSG_BAD_PATH);
                }
            }
        }
        else if(ai->followTarget.type == AI_FOLLOWTARGET_ENTITY){
            // If I'm closer to my target than the end of my path is, it's a bad path.

            float distToTargetSQR =
                distance3Squared(    ENTPOS(e),
                                    ai->followTarget.pos);

            float distFromTargetToWhereHeWasWhenPathWasMadeSQR =
                distance3Squared(    ai->followTarget.pos,
                                    ai->followTarget.posWhenPathFound);

            float distFromTargetToPathEndSQR =
                distance3Squared(    ENTPOS(ai->followTarget.entity),
                                    lastWaypoint->pos);

            if(    distToTargetSQR < distFromTargetToPathEndSQR &&
                distFromTargetToWhereHeWasWhenPathWasMadeSQR > distToTargetSQR)
            {
                aiSendMessage(e, AI_MSG_BAD_PATH);
            }
        }
    }
    else if(!ai->isAfraidFromAI &&
            !ai->isAfraidFromPowers &&
            !ai->isGriefed &&
            ai->waitingForTargetToMove &&
            ai->followTarget.type == AI_FOLLOWTARGET_ENTITY)
    {
        float targetMoveDistSQR = distance3SquaredXZ(ai->followTarget.posWhenPathFound, ai->followTarget.pos);
        float targetMoveDistY = fabs(distanceY(ai->followTarget.posWhenPathFound, ai->followTarget.pos));

        if(    targetMoveDistSQR > SQR(10) ||
            targetMoveDistY > 30)
        {
            aiSendMessage(e, AI_MSG_ATTACK_TARGET_MOVED);
        }
    }
}

static void addLeashBehavior(Entity* e, AIVars* ai, F32 x, F32 y, F32 z)
{
    //    The leash will pull the entity back to spawn
    //    On the way back, it will become untargetable and unselectable
    //    it will get full health back
    char behaviorString[400] = {0};
    int i;
    if( !ai->alwaysReturnToSpawn )
        aiBehaviorCombatFinished(e, ENTAI(e)->base);

    if (!ai->vulnerableWhileLeashing)
    {
        sprintf(behaviorString, "MoveToPos(pos(%f,%f,%f),Variation(5),TargetRadius(5),Unselectable,Untargetable,Invincible,DoAttackAI(0),JumpHeight(10000)),DoNothing(Timer(1),Invincible(0),Untargetable(0),Unselectable(0),DoAttackAI(1))", x, y, z);    //    Added the DoNothing so this doesn't become a permvar
    }
    else
    {
        sprintf(behaviorString, "MoveToPos(pos(%f,%f,%f),Variation(5),TargetRadius(5),DoAttackAI(0),JumpHeight(10000)),DoNothing(Timer(1),DoAttackAI(1))", x, y, z);    //    Added the DoNothing so this doesn't become a permvar
    }

    if (!ai->vulnerableWhileLeashing && e && e->pchar)
    {
        e->pchar->attrCur.fHitPoints = e->pchar->attrMax.fHitPoints;
    }
    aiDiscardFollowTarget(e, "Leashed back", false);
    aiDiscardAttackTarget(e, "Leashed back");
    ai->time.wasAttacked = ai->time.wasAttackedMelee = ai->time.wasDamaged = ai->time.didAttack = 0;
    for (i = 0; i < AI_FEELING_COUNT; ++i)
    {
        ai->feeling.sets[AI_FEELINGSET_BASE].level[i] = 0.0f;
    }
    if(!ai->doNotChangePowers){
        aiDiscardCurrentPower(e);
    }
    aiBehaviorAddString(e, ai->base,behaviorString);
}

static int aiCheckLeash(Entity* e, AIVars* ai)
{
    // Try to return to somewhere if I have no target.
    if(    ABS_TIME_SINCE(ai->time.foundPath) > SEC_TO_ABS(2) &&
        (    !(e->pchar && character_IsConfused(e->pchar)) &&
        ABS_TIME_SINCE(ai->attackTarget.time.set) > SEC_TO_ABS(3)))
    {
        if(!ai->doNotMove)
        {
            Entity* creator = erGetEnt(e->erCreator);
            int canBeLeashed = (    !ai->followTarget.type ||
                ai->followTarget.reason != AI_FOLLOWTARGETREASON_AVOID);
            if(creator && !ai->pet.useStayPos){
                // Return to my creator.

                float distSQR = distance3Squared(ENTPOS(e), ENTPOS(creator));

                canBeLeashed = canBeLeashed && (    !ai->motion.type ||
                ai->followTarget.type != AI_FOLLOWTARGET_ENTITY ||
                ai->followTarget.reason != AI_FOLLOWTARGETREASON_CREATOR);

                if(    (distSQR > SQR(ai->leashToSpawnDistance)) && canBeLeashed    )
                {
                    ai->time.foundPath = ABS_TIME;
                    aiSetFollowTargetEntity(e, ai, creator, AI_FOLLOWTARGETREASON_CREATOR);
                    aiGetProxEntStatus(ai->proxEntStatusTable, creator, 1);
                    aiConstructPathToTarget(e);
                    aiStartPath(e, 0, distSQR < SQR(AI_RUN_TO_CREATOR_DIST));
                    ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : (ai->collisionRadius + 10);
                    addLeashBehavior(e, ai, ENTPOS(creator)[0], ENTPOS(creator)[1], ENTPOS(creator)[2]);
                    return 1;
                }
            }
            else if(!ai->spawnPosIsSelf || !ai->pet.useStayPos)
            {
                // Return to spawn point.

                F32* spawnPos;
                float distSQR;
                float maxDist = ai->leashToSpawnDistance + ai->collisionRadius;

                spawnPos = ai->pet.useStayPos ? ai->pet.stayPos : ai->spawn.pos;

                canBeLeashed = canBeLeashed && (    !ai->motion.type ||
                ai->followTarget.type != AI_FOLLOWTARGET_POSITION ||
                ai->followTarget.reason != ai->pet.useStayPos ? AI_FOLLOWTARGETREASON_BEHAVIOR : AI_FOLLOWTARGETREASON_SPAWNPOINT);

                distSQR = distance3SquaredXZ(ENTPOS(e), spawnPos);

                if(    (distSQR > SQR(maxDist)) && canBeLeashed)
                {
                    ai->time.foundPath = ABS_TIME;
                    aiSetFollowTargetPosition(e, ai, spawnPos, ai->pet.useStayPos ? AI_FOLLOWTARGETREASON_BEHAVIOR : AI_FOLLOWTARGETREASON_SPAWNPOINT);
                    aiConstructPathToTarget(e);
                    aiStartPath(e, 0, 1);
                    ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : maxDist;
                    addLeashBehavior(e, ai, spawnPos[0], spawnPos[1], spawnPos[2]);
                    return 1;
                }
            }
        }
    }

    return 0;
}
void aiCritterDoDefault(Entity* e, AIVars* ai, int force_return){
    int returningToSpawn = force_return;
    
    if (ai->leashToSpawnDistance)
    {
        if (aiCheckLeash(e, ai))
        {
            return;
        }
    }
    
    if(ai->attackTarget.entity && !force_return)
        return;

    // Try to return to somewhere if I have no target.

    if(    !(e->pchar && character_IsConfused(e->pchar)) &&
        ABS_TIME_SINCE(ai->time.foundPath) > SEC_TO_ABS(2) &&
        ABS_TIME_SINCE(ai->attackTarget.time.set) > SEC_TO_ABS(3))
    {
        if(!ai->doNotMove)
        {
            Entity* creator = erGetEnt(e->erCreator);

            if(creator && !ai->pet.useStayPos){
                // Return to my creator.

                float distSQR = distance3Squared(ENTPOS(e), ENTPOS(creator));

                if(    distSQR > SQR(ai->returnToSpawnDistance) &&
                    (    !ai->followTarget.type ||
                        ai->followTarget.reason != AI_FOLLOWTARGETREASON_AVOID) &&
                    (    !ai->motion.type ||
                        ai->followTarget.type != AI_FOLLOWTARGET_ENTITY ||
                        ai->followTarget.reason != AI_FOLLOWTARGETREASON_CREATOR))
                {
                    ai->time.foundPath = ABS_TIME;
                    aiSetFollowTargetEntity(e, ai, creator, AI_FOLLOWTARGETREASON_CREATOR);
                    aiGetProxEntStatus(ai->proxEntStatusTable, creator, 1);
                    aiConstructPathToTarget(e);
                    aiStartPath(e, 0, distSQR < SQR(AI_RUN_TO_CREATOR_DIST));
                    ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : (ai->collisionRadius + 10);
                    returningToSpawn = 1;
                }
                else
                    aiBehaviorCombatFinished(e, ENTAI(e)->base);
            }
            else if(!ai->spawnPosIsSelf || !ai->pet.useStayPos)
            {
                // Return to spawn point.

                F32* spawnPos;
                float distSQR;
                float maxDist = ai->returnToSpawnDistance + ai->collisionRadius;

                spawnPos = ai->pet.useStayPos ? ai->pet.stayPos : ai->spawn.pos;

                distSQR = distance3SquaredXZ(ENTPOS(e), spawnPos);

                if(    e->encounterInfo &&  //If you have been detatched from your encounter, don't bother trying to return there
                    distSQR > SQR(maxDist) &&
                    (    !ai->followTarget.type ||
                        ai->followTarget.reason != AI_FOLLOWTARGETREASON_AVOID) &&
                    (    !ai->motion.type ||
                        ai->followTarget.type != AI_FOLLOWTARGET_POSITION ||
                        ai->followTarget.reason != ai->pet.useStayPos ? AI_FOLLOWTARGETREASON_BEHAVIOR : AI_FOLLOWTARGETREASON_SPAWNPOINT))
                {
                    ai->time.foundPath = ABS_TIME;
                    aiSetFollowTargetPosition(e, ai, spawnPos, ai->pet.useStayPos ? AI_FOLLOWTARGETREASON_BEHAVIOR : AI_FOLLOWTARGETREASON_SPAWNPOINT);
                    aiConstructPathToTarget(e);
                    aiStartPath(e, 0, 1);
                    ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : maxDist;
                    returningToSpawn = 1;
                }
                else
                    aiBehaviorCombatFinished(e, ENTAI(e)->base);
            }
        }
    }
    
    if(!returningToSpawn)
    {
        if(!ai->doNotChangePowers)
        {
            aiDiscardCurrentPower(e);
        }
        CLRB(e->seq->sticky_state, STATE_COMBAT);

        // Turn a random amount.
        if(!ai->motion.type && ai->turnWhileIdle && !e->IAmInEmoteChatMode )
        {
            if(ai->keepSpawnPYR)
            {
                aiTurnBodyByPYR(e, ai, ai->spawn.pyr);
            }
            else if(getCappedRandom(1000) < 3  && !e->cutSceneActor)
            {
                Vec3 pyr;
                copyVec3(ai->pyr, pyr);
                pyr[1] = addAngle(pyr[1], RAD(rand() % 360 - 180));
                aiTurnBodyByPYR(e, ai, pyr);
            }
        }
    }
    else
    {
        if( !ai->alwaysReturnToSpawn )
            aiBehaviorCombatFinished(e, ENTAI(e)->base);
    }
}

int aiCritterCountMinions(Entity *e, AIVars *ai, int minionBrainType)
{
    int minioncount=0;
    if (ai->teamMemberInfo.team)
    {
        AITeamMemberInfo* member;

        // Find minions.

        for(member = ai->teamMemberInfo.team->members.list; member; member = member->next){
            if(member->entity != e && member->entity->pchar->attrCur.fHitPoints > 0){
                AIVars* aiMember = ENTAI(member->entity);

                // Check if this guy is a minion

                if(    aiMember->brain.type == minionBrainType )
                {
                    minioncount++;
                }
            }
        }
    }
    return minioncount;
}

int aiCritterDoSummoning(Entity *e, AIVars *ai, int minionBrainType, int minMinionCount)
{
    // If we have a team and can use a power
    if(    !e->pchar->ppowCurrent &&
        !e->pchar->ppowQueued)
    {
        // If less than X minions left, summon more!

        int minioncount = aiCritterCountMinions(e, ai, minionBrainType);

        if (minioncount < minMinionCount)
        {
            AIPowerInfo* resInfo = aiChoosePower(e, ai, AI_POWERGROUP_SUMMON);

            if(resInfo){
                Power* resPower = resInfo->power;

                if(!resPower->bActive && resPower->fTimer == 0){
                    aiUsePower(e, resInfo, NULL);
                    return 1;
                }
            }
        }
    }
    return 0;
}

int aiCritterDoAutomaticSummoning( Entity *e, AIVars *ai )
{
    if(    !e->pchar->ppowCurrent && !e->pchar->ppowQueued)
    {
        AIPowerInfo* resInfo = aiChoosePower(e, ai, AI_POWERGROUP_SUMMON);

        if(resInfo)
        {
            Power* resPower = resInfo->power;
            int validTarget = 0;
            switch (resPower->ppowBase->eTargetType)
            {
            //    more may need to be added
            case kTargetType_Foe:
                validTarget = ai->attackTarget.entity ? 1 : 0;
                break;
            default:
                validTarget = 1;
                break;
            }
            if (validTarget)
            {
                if(!resPower->bActive && resPower->fTimer == 0)
                {
                    aiUsePower(e, resInfo,  ai->attackTarget.entity); 
                    return 1;
                }
            }
            //    if it wasn't a valid target, or the power failed to activate
            //    don't choose that power for a little bit
            resInfo->doNotChoose.startTime = ABS_TIME;
            resInfo->doNotChoose.duration = SEC_TO_ABS(2);
        }
    }
    return 0;
}

// Analyzes a buff/debuff power and uses it (based on if it's locational, etc)
int aiCritterExecuteBuff(Entity *e, AIVars *ai, AIPowerInfo *info)
{
    if (info->isToggle) {
        aiSetTogglePower(e, info, 1);
    }else{
        TargetType targetType = info->power->ppowBase->eTargetType;
        
        ai->helpTarget = NULL;

        switch(targetType){
            case kTargetType_Foe:
                {
                    if(info->isAttack){
                        aiSetCurrentPower(e, info, 0, 0);
                        ai->power.preferred = info;
                    }
                    return 0;
                }
            
            case kTargetType_Location:
            case kTargetType_Teleport:
                {
                    // Non-toggle power, locational, must be AoE
                    //if BUFF, then target nearest friend, if DEBUFF, target current attack target, else nearest, else 10ft
                    Entity *target=NULL;
                    if (info->groupFlags & AI_DEBUFF_FLAGS) {
                        // Current target
                        target = ai->attackTarget.entity;
                        if (!target) // No current target?  Choose someone in the attacker list?  (shouldn't happen?)
                            if (ai->attackerList.head)
                                target = ai->attackerList.head->attacker;
                    } else if (info->groupFlags & AI_BUFF_FLAGS) {
                        // Look for weak friend
                        target = aiCritterFindTeammateInNeed(e, ai, aiGetPowerRange(e->pchar, info), info, targetType);
                        if (!target) // No friends?  Target self
                            target = e;
                    }

                    if (target) {
                        aiUsePowerOnLocation(e, info, ENTPOS(target));
                    }
                    if (!target) {
                        Vec3 delta;
                        Vec3 dest = {0,0,8};
                        mulVecMat3(dest, ENTMAT(e), delta);
                        addVec3(ENTPOS(e), delta, dest);

                        aiUsePowerOnLocation(e, info, dest);
                    }
                    break;
                }
            case kTargetType_MyCreator:
            case kTargetType_MyOwner:
            case kTargetType_MyPet:
            case kTargetType_MyCreation:
            case kTargetType_Friend:
            case kTargetType_Teammate:
            case kTargetType_Leaguemate:
                {
                    // Buff for a friend, find a friend in need!

                    float range = aiGetPowerRange(e->pchar, info);
                    Entity* target = aiCritterFindTeammateInNeed(e, ai, 1000, info, targetType);

                    if(ai->brain.type == AI_BRAIN_MALTA_HERCULES_TITAN){
                        if(info->teamInfo && ABS_TIME_SINCE(info->teamInfo->time.lastUsed) < SEC_TO_ABS(5)){
                            return 0;
                        }
                    }
                    else if(ai->brain.type == AI_BRAIN_HELLIONS && info->groupFlags & AI_POWERGROUP_HELLIONFIRE)
                    {
                        int entArray[MAX_ENTITIES_PRIVATE];
                        int i, entcount = mgGetNodesInRange(0, ENTPOS(e), (void**)entArray, 0);
                        F32 dist = SQR(100), curdist;

                        target = NULL;    // can't use this on anything but the fires

                        for(i = 0; i < entcount; i++)
                        {
                            Entity* curEnt = entities[entArray[i]];

                            if(curEnt->villainDef && 
                                (!stricmp(curEnt->villainDef->name, "Hellions_Arson_Hellion_Window_Fire") ||
                                !stricmp(curEnt->villainDef->name, "Hellions_Arson_Hellion_Roof_Fire")))
                            {
                                curdist = distance3Squared(ENTPOS(e), ENTPOS(curEnt));

                                if(curdist < dist)
                                {
                                    dist = curdist;
                                    target = curEnt;
                                }
                            }
                        }
                    }

                    if (target) {
                        if (info->groupFlags & (AI_POWERGROUP_HEAL_ALLY | AI_POWERGROUP_HEAL_SELF)) {
                            if(    info->groupFlags & AI_POWERGROUP_HEAL_SELF ||
                                target->pchar->attrCur.fHitPoints <= target->pchar->attrMax.fHitPoints * 0.75)
                            {
                                AIVars* aiTarget = ENTAI(target);

                                range += aiTarget->collisionRadius;

                                if(distance3Squared(ENTPOS(e), ENTPOS(target)) < SQR(range)){
                                    aiUsePower(e, info, target);
                                }else{
                                    ai->helpTarget = target;

                                    // Force an entry into the status table.

                                    aiGetProxEntStatus(ai->proxEntStatusTable, target, 1);
                                }
                            } else {
                                return 0;
                            }
                        } else {
                            aiUsePower(e, info, target);
                        }
                    }
                    break;
                }
            case kTargetType_DeadMyCreation:
            case kTargetType_DeadMyPet:
            case kTargetType_DeadVillain:
            case kTargetType_DeadFriend:
            case kTargetType_DeadTeammate:
            case kTargetType_DeadLeaguemate:
                {
                    float range = aiGetPowerRange(e->pchar, info);
                    Entity *target = aiCritterFindDeadTeammate(e, ai, 1000, targetType);

                    // Commenting this change out as it is not currently needed and is causing COH-25045
                    //if(!target && targetType==kTargetType_DeadFriend || targetType==kTargetType_DeadVillain )
                    //    target = aiCritterFindNearbyDead(e, ai);

                    if (target)
                    {
                        if (target->pchar->attrCur.fHitPoints <= 0) {
                            AIVars* aiTarget = ENTAI(target);

                            range += aiTarget->collisionRadius;

                            if(distance3Squared(ENTPOS(e), ENTPOS(target)) < SQR(range)){
                                aiUsePower(e, info, target);
                            }else{
                                ai->helpTarget = target;

                                // Force an entry into the status table.

                                aiGetProxEntStatus(ai->proxEntStatusTable, target, 1);
                            }
                        } else {
                            return 0;
                        }
                    }
                    break;
                }
            default:
                {
                    // Some other power, just do it!
                    aiUsePower(e, info, e);
                    break;
                }
        }
    }
    return 1;
}

void aiCritterInitPowerGroupFlags(AIVars *ai)
{
    if (ai->power.groupFlags==0) {
        // Lazy init of groupFlags
        AIPowerInfo* info;

        for(info = ai->power.list; info; info = info->next){
            ai->power.groupFlags |= info->groupFlags;
        }
        if (ai->power.groupFlags==0) {
            // Shouldn't ever happen, but let's make sure the init doesn't happen more than once
            ai->power.groupFlags=1;
        }
    }
}

static int aiCritterDetermineCurrentStage(Entity *e, AIVars *ai)
{
    F32 hpp = e->pchar->attrCur.fHitPoints / e->pchar->attrMax.fHitPoints;
    if (hpp < 0.25) {
        return AI_POWERGROUP_ENDBATTLE;
    } else if (hpp < 0.50) {
        return AI_POWERGROUP_MIDBATTLE;
    } else if (hpp < 0.80) {
        return AI_POWERGROUP_EARLYBATTLE;
    }
    return 0;
}

static int aiCritterGetStagePriority(int groupFlags)
{
    int togglePriority;
    if (groupFlags & AI_POWERGROUP_ENDBATTLE) {
        // Most important powers
        if (groupFlags & AI_POWERGROUP_EARLYBATTLE) {
            // This is a Early+Mid+End most likely
            togglePriority = 8;
        } else if (groupFlags & AI_POWERGROUP_MIDBATTLE) {
            // This is Mid+End
            togglePriority = 9;
        } else {
            // Plain old End, must be important!
            togglePriority = 10;
        }
    } else if (groupFlags & AI_POWERGROUP_MIDBATTLE) {
        if (groupFlags & AI_POWERGROUP_EARLYBATTLE) {
            // Early+Mid
            togglePriority = 4;
        } else {
            // Mid
            togglePriority = 5;
        }
    } else if (groupFlags & AI_POWERGROUP_EARLYBATTLE) {
        // Just Early
        togglePriority = 1;
    } else {
        // Not a paced power
        togglePriority = -1;
    }
    return togglePriority;
}

int aiCritterDoDefaultPacedPowers(Entity *e, AIVars* ai)
{
    int currentStage = aiCritterDetermineCurrentStage(e, ai);
    AIPowerInfo *info = NULL;
    Character* pchar = e->pchar;

    aiCritterInitPowerGroupFlags(ai);

    if( ai->doNotUsePowers )
        return 0;

    if (!currentStage || !(ai->power.groupFlags & currentStage)) {
        // Either we're not in a stage (i.e. before battle starts), or none of our powers should be executed during the current stage
        if (ai->power.toggle.curPaced) {
            aiSetTogglePower(e, ai->power.toggle.curPaced, 0);
            aiClearCurrentBuff(&ai->power.toggle.curPaced);
        }
        return 0;
    }
    if (ai->power.toggle.curPaced && !(ai->power.toggle.curPaced->groupFlags & currentStage)) {
        // This power is not appropriate for this stage, kill it!
        aiSetTogglePower(e, ai->power.toggle.curPaced, 0);
        aiClearCurrentBuff(&ai->power.toggle.curPaced);
    }

    if (pchar->ppowCurrent || pchar->ppowQueued) {
        // Already trying to execute some other power
        return 0;
    }

    if (ai->power.toggle.curPaced) {
        // We have a TOGGLE power active (and it is valid for this stage)
        // Check to see if there's a higher priority TOGGLE power available, if so, kill this power
        info = aiChoosePowerEx(e, ai, currentStage, CHOOSEPOWER_TOGGLEONLY);
        if (info && aiCritterGetStagePriority(info->groupFlags) > ai->power.toggle.curPacedPriority) {
            // There is a toggle power we'd rather use!
            // Kill the old one
            aiSetTogglePower(e, ai->power.toggle.curPaced, 0);
            aiClearCurrentBuff(&ai->power.toggle.curPaced);
        } else {
            // Keep the current one, but feel free to use a click power
            info = NULL;
        }
    }
    if (!info) {
        if (!ai->power.toggle.curPaced) {
            // We don't have a toggle paced power started for this stage, but we want to!
            info = aiChoosePowerEx(e, ai, currentStage, CHOOSEPOWER_ANY);
        } else {
            // We have a toggle power already, only choose from non-toggle powers
            info = aiChoosePowerEx(e, ai, currentStage, CHOOSEPOWER_NOTTOGGLE);
        }
    }
    if (info && !info->power->fTimer && !info->power->bActive) {
        // Use it!
        int noMoreChecks = aiCritterExecuteBuff(e, ai, info);

        if (info->isToggle) {
            aiSetCurrentBuff(&ai->power.toggle.curPaced, info);
        }
        
        return noMoreChecks;
    }
    return 0;

}

int aiCritterDoDefaultBuffs(Entity* e, AIVars* ai)
{
    Character* pchar = e->pchar;
    int groupFlags; 

    if (ai->nearbyPlayerCount <1 || ai->doNotUsePowers )
        return 0;

    aiCritterInitPowerGroupFlags(ai);

    groupFlags = ai->power.groupFlags;

    if (groupFlags>1 && !pchar->ppowCurrent && !pchar->ppowQueued) {
        // We have at least some non-attack power and we're able to activate a power
        bool tryBuffFirst = ABS_TIME_SINCE(ai->power.time.buff) > ABS_TIME_SINCE(ai->power.time.debuff);
        if (tryBuffFirst && (groupFlags & AI_BUFF_FLAGS)) {
            // Timing for buffs: Don't do this in the 3 seconds after being attacked, any other time is fine
            if (aiCritterDoBuff(e, ai, &ai->power.time.buff, &ai->power.toggle.curBuff, AI_BUFF_FLAGS, SEC_TO_ABS(3), 0, SEC_TO_ABS(2)))
                return 1;
        }

        if (groupFlags & AI_DEBUFF_FLAGS) {
            // Timing for debuffs: Do this on in the 0-5 seconds after being attacked
            if (aiCritterDoBuff(e, ai, &ai->power.time.debuff, &ai->power.toggle.curDebuff, AI_DEBUFF_FLAGS, 0, SEC_TO_ABS(5), SEC_TO_ABS(10)))
                return 1;
        }

        if (!tryBuffFirst && (groupFlags & AI_BUFF_FLAGS)) {
            if (aiCritterDoBuff(e, ai, &ai->power.time.buff, &ai->power.toggle.curBuff, AI_BUFF_FLAGS, SEC_TO_ABS(3), 0, SEC_TO_ABS(2)))
                return 1;
        }
    }

    return 0;
}

static void aiCritterDoDefaultBrain(Entity* e, AIVars* ai)
{
    if( aiCritterDoAutomaticSummoning(e, ai) )
        return;
    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    if (aiCritterDoDefaultBuffs(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}


void aiCritterDoCustomHealAlways(Entity* e, AIVars* ai)
{
    AITeamMemberInfo* member;
    Entity *eClosest = NULL;
    float fClosestDistance = 99999999.0f;
    AIPowerInfo* closestPower = NULL;
    bool bCheckHeal = false;
    bool bCheckRes = false;

    if (e && e->pchar && ai && ai->teamMemberInfo.team)
    {
        AIPowerInfo *healPower = aiChoosePower(e, ai, AI_POWERGROUP_HEAL_ALLY);
        AIPowerInfo *resPower = aiChoosePower(e, ai, AI_POWERGROUP_RESURRECT);

        // validate power can be used at this time
        if (healPower && healPower->power && healPower->power->ppowBase && 
            ABS_TIME_SINCE(healPower->time.lastUsed) > SEC_TO_ABS(healPower->power->ppowBase->fActivatePeriod))
        {
            bCheckHeal = true;
        }

        if (resPower && resPower->power && resPower->power->ppowBase && 
            ABS_TIME_SINCE(resPower->time.lastUsed) > SEC_TO_ABS(resPower->power->ppowBase->fActivatePeriod))
        {
            bCheckRes = true;
        }

        if (bCheckHeal || bCheckRes)
        {
            for(member = ai->teamMemberInfo.team->members.list; member; member = member->next)
            {
                Entity* entMember = member->entity;

                if (entMember && entMember->pchar)
                {
                    if (bCheckHeal && 
                        entMember->pchar->attrCur.fHitPoints < entMember->pchar->attrMax.fHitPoints &&
                        entMember->pchar->attrCur.fHitPoints > 0.0f &&
                        aiPowerTargetAllowed(e, healPower, entMember, true))
                    {

                        float range = aiGetPowerRange(e->pchar, healPower);
                        float fDistance = distance3Squared(ENTPOS(e), ENTPOS(entMember));

                        if (fDistance <= fClosestDistance)
                        {
                            fClosestDistance = fDistance;
                            eClosest = entMember;
                            if(fDistance < SQR(range))
                            {    
                                closestPower = healPower;
                            }
                        }
                    } 
                    else if (bCheckRes && entMember->pchar->attrCur.fHitPoints <= 0.0f &&
                        aiPowerTargetAllowed(e, resPower, entMember, true))
                    {
                        float range = aiGetPowerRange(e->pchar, resPower);
                        float fDistance = distance3Squared(ENTPOS(e), ENTPOS(entMember));

                        if (fDistance <= fClosestDistance)
                        {
                            fClosestDistance = fDistance;
                            eClosest = entMember;
                            if(fDistance < SQR(range))
                            {    
                                closestPower = resPower;
                            }
                        }
                    }
                }
            }

            // we found someone who's close enough to use our power on
            if (closestPower != NULL && eClosest != NULL)
            {
                aiUsePower(e, closestPower, eClosest);
                return;
            }

            // we found someone we can help but they are too far away
            if (eClosest != NULL && ABS_TIME_SINCE(ai->time.foundPath) > SEC_TO_ABS(2))
            {
                aiSetFollowTargetEntity(e, ai, eClosest, AI_FOLLOWTARGETREASON_MOVING_TO_HELP); 
                aiConstructPathToTarget(e);
                aiStartPath(e, 0, 0);
            }
        }
    }
    
    if (aiCritterDoDefaultBuffs(e, ai))
        return;
    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}



static void aiCritterDo5thColumnTroop(Entity* e, AIVars* ai){
    if(!ai->brain.fifthColumn.weaponGroup && ai->teamMemberInfo.team){
        Entity*                friends[3] = {NULL, NULL};
        float                distSQRs[3];
        AITeamMemberInfo*    member;
        int                    i;
        int                    weaponGroups[3] = {1, 2, 3};
        int                    groupsRemaining = 3;

        // Find the two closest teammates with the same brain.

        for(member = ai->teamMemberInfo.team->members.list; member; member = member->next){
            if(member->entity && member->entity != e){
                Entity* entMember = member->entity;
                AIVars* aiMember = ENTAI(member->entity);

                if(aiMember->brain.type == ai->brain.type && !aiMember->brain.fifthColumn.weaponGroup){
                    float distSQR = distance3Squared(ENTPOS(e), ENTPOS(entMember));

                    for(i = 0; i < 2; i++){
                        if(!friends[i] || distSQR < distSQRs[i]){
                            // Move everyone forward in the list by one.

                            memmove(friends + i + 1, friends + i, sizeof(friends) - sizeof(friends[0]) * (i + 1));
                            memmove(distSQRs + i + 1, distSQRs + i, sizeof(distSQRs) - sizeof(distSQRs[0]) * (i + 1));

                            friends[i] = entMember;
                            distSQRs[i] = distSQR;

                            break;
                        }
                    }
                }
            }
        }

        for(i = 0; i < 2; i++){
            if(!friends[i]){
                break;
            }
        }

        if(i == 2){
            // Okay, got my two closest friends, so let's draw straws for weapons.

            friends[2] = e;

            for(i = 0; i < 3; i++){
                if(friends[i]){
                    AIVars* aiMember = ENTAI(friends[i]);
                    int group = rand() % groupsRemaining;

                    aiMember->brain.fifthColumn.weaponGroup = weaponGroups[group];

                    groupsRemaining--;

                    memmove(weaponGroups + group, weaponGroups + group + i, sizeof(weaponGroups[0]) * (groupsRemaining - group));
                }
            }
        }
    }
    else if(!ai->brain.fifthColumn.attackWeapon){
        switch(ai->brain.fifthColumn.weaponGroup){
            case 1:
                ai->brain.fifthColumn.attackWeapon = aiChooseAttackPower(e, ai, 0, 20, 0, AI_POWERGROUP_BASIC_ATTACK, 0);
                break;
            case 2:
                ai->brain.fifthColumn.attackWeapon = aiChooseAttackPower(e, ai, 20, 50, 0, AI_POWERGROUP_BASIC_ATTACK, 0);
                break;
            case 3:
                ai->brain.fifthColumn.attackWeapon = aiChooseAttackPower(e, ai, 50, 100, 0, AI_POWERGROUP_BASIC_ATTACK, 0);
                break;
        }
    }

    if(ai->attackTarget.entity && ai->brain.fifthColumn.attackWeapon){
        ai->doNotChangePowers = 1;

        if(ai->power.current != ai->brain.fifthColumn.attackWeapon){
            aiSetCurrentPower(e, ai->brain.fifthColumn.attackWeapon, 1, 0);

            ai->power.preferred = ai->power.current;
        }
    }
}

static void aiCritterDo5thColumnVampyr(Entity* e, AIVars* ai){
    Character* pchar = e->pchar;

    if(    ai->attackTarget.entity &&
        !pchar->ppowCurrent &&
        !pchar->ppowQueued)
    {
        if(    !ai->power.click.curBuff &&
            !ai->power.click.curDebuff &&
            ABS_TIME_SINCE(ai->power.time.buff) > SEC_TO_ABS(15))
        {
            if(pchar->attrCur.fHitPoints < pchar->attrMax.fHitPoints * 0.5){
                AIPowerInfo* heal = aiChoosePower(e, ai, AI_POWERGROUP_HEAL_SELF);

                if(    heal &&
                    heal->power->fTimer == 0 &&
                    ABS_TIME_SINCE(heal->time.lastUsed) > SEC_TO_ABS(15))
                {
                    ai->power.time.buff = ABS_TIME;

                    if(heal->isAttack){
                        aiSetCurrentPower(e, heal, 0, 0);
                        ai->power.preferred = heal;
                        ai->doNotChangePowers = 1;
                        ai->power.click.curBuff = heal;
                    }else{
                        aiUsePower(e, heal, NULL);
                    }
                }
            }
        }
        else if(ai->power.click.curBuff &&
                ai->power.click.curBuff == ai->power.current &&
                (    ABS_TIME_SINCE(ai->power.click.curBuff->time.lastUsed) < SEC_TO_ABS(10) ||
                    ABS_TIME_SINCE(ai->time.setCurrentPower) > SEC_TO_ABS(10)))
        {
            ai->doNotChangePowers = 0;
            ai->power.click.curBuff = NULL;
        }

        if(    !ai->power.click.curBuff &&
            ABS_TIME_SINCE(ai->power.time.debuff) > SEC_TO_ABS(30))
        {
            if(!ai->power.click.curDebuff){

                AIPowerInfo* debuff = aiChoosePower(e, ai, AI_POWERGROUP_DEBUFF);

                if(    debuff &&
                    debuff->power->fTimer == 0 &&
                    ABS_TIME_SINCE(debuff->time.lastUsed) > SEC_TO_ABS(30))
                {
                    ai->power.time.debuff = ABS_TIME;

                    if(debuff->isAttack){
                        aiSetCurrentPower(e, debuff, 0, 0);
                        ai->power.preferred = debuff;
                        ai->doNotChangePowers = 1;
                        ai->power.click.curDebuff = debuff;
                    }else{
                        aiUsePower(e, debuff, NULL);
                    }
                }
            }
        }
        else if(ai->power.click.curDebuff &&
                ai->power.click.curDebuff == ai->power.current &&
                (    ABS_TIME_SINCE(ai->power.click.curDebuff->time.lastUsed) < SEC_TO_ABS(10) ||
                    ABS_TIME_SINCE(ai->time.setCurrentPower) > SEC_TO_ABS(10)))
        {
            ai->doNotChangePowers = 0;
            ai->power.click.curDebuff = NULL;
        }
    }else{
        if(ai->power.click.curBuff || ai->power.click.curDebuff){
            ai->doNotChangePowers = 0;
            ai->power.click.curBuff = NULL;
            ai->power.click.curDebuff = NULL;
        }
    }

    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

static void aiCritterDoClockwork(Entity* e, AIVars* ai){
    U32 time_since = ABS_TIME_SINCE(ai->time.wasAttacked);
    Character* pchar = e->pchar;

    if(ai->attackTarget.entity){
        if(ai->attackTarget.hitCount){
            //ai->attackTarget.status->duration.notValidTarget = SEC_TO_ABS(30);
            //ai->attackTarget.status->time.wasMyTarget = ABS_TIME;

            ai->doNotChangeAttackTarget = 0;
        }else{
            ai->doNotChangeAttackTarget = 1;
        }
    }else{
        ai->doNotChangeAttackTarget = 0;
    }


    if(    !ai->power.toggle.curDebuff ||
        !ai->power.toggle.curDebuff->power->bActive)
    {
        if(ai->attackTarget.entity){
            F32 hpCur = pchar->attrCur.fHitPoints;
            F32 hpMax = pchar->attrMax.fHitPoints;

            if(    hpCur < hpMax * 0.25 &&
                time_since > SEC_TO_ABS(1) &&
                time_since < SEC_TO_ABS(10))
            {
                AIPowerInfo* newToggle = aiChooseTogglePower(e, ai, AI_POWERGROUP_BASIC_ATTACK);

                if(newToggle){
                    if(ai->power.toggle.curDebuff){
                        aiClearCurrentBuff(&ai->power.toggle.curDebuff);
                    }
                    
                    aiSetCurrentBuff(&ai->power.toggle.curDebuff, newToggle);
                    aiSetTogglePower(e, newToggle, 1);
                }
            }
        }
    }else{
        if(    time_since > SEC_TO_ABS(30) ||
            !ai->attackTarget.entity && time_since > SEC_TO_ABS(10))
        {
            aiSetTogglePower(e, ai->power.toggle.curDebuff, 0);
            aiClearCurrentBuff(&ai->power.toggle.curDebuff);
        }
    }

    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

static void aiCritterDoFreakshow(Entity* e, AIVars* ai){
    Character* pchar = e->pchar;

    if(    !ai->power.toggle.curBuff &&
        !pchar->ppowCurrent &&
        !pchar->ppowQueued &&
        ai->attackTarget.entity)
    {
        F32 hpRatio = pchar->attrCur.fHitPoints / pchar->attrMax.fHitPoints;

        if(hpRatio < 0.2){
            AIPowerInfo* curBuff = aiChoosePower(e, ai, AI_POWERGROUP_BUFF);

            if(curBuff){
                aiSetCurrentBuff(&ai->power.toggle.curBuff, curBuff);

                aiUsePower(e, curBuff, NULL);
            }
        }
    }

    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

static void aiCritterDoFreakshowTank(Entity* e, AIVars* ai){
    Character* pchar = e->pchar;

    aiCritterDoFreakshow(e, ai);

    if(    !pchar->ppowCurrent &&
        !pchar->ppowQueued &&
        ai->attackTarget.entity)
    {
        if(ai->attackTarget.hitCount >= 2){
            ai->attackTarget.status->invalid.begin = ABS_TIME;
            ai->attackTarget.status->invalid.duration = SEC_TO_ABS(10);
        }
    }

    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

static void aiCritterDoHydraHead(Entity* e, AIVars* ai){
    int i;

    if(ai->brain.hydraHead.useTeam ||
        ABS_TIME_SINCE(ai->brain.hydraHead.time.foundGenerators) > SEC_TO_ABS(2))
    {
        int forceFieldOn = 0;
        
        ai->brain.hydraHead.time.foundGenerators = ABS_TIME;

        if(ai->brain.hydraHead.useTeam)
        {
            AITeamMemberInfo* member;
            
            for(member = ai->teamMemberInfo.team->members.list; member; member = member->next)
            {
                Entity* memberEnt = member->entity;
                AIVars* memberAI = ENTAI(memberEnt);
                if(memberAI->brain.type == AI_BRAIN_HYDRA_FORCEFIELD_GENERATOR && memberEnt->pchar &&
                    memberEnt->pchar->attrCur.fHitPoints > 0)
                {
                    forceFieldOn = 1;
                    break;
                }
            }
        }
        else
        {
            int entArray[MAX_ENTITIES_PRIVATE];
            int entCount;
            entCount = mgGetNodesInRange(0, ENTPOS(e), (void**)entArray, ARRAY_SIZE(entArray));

            for(i = 0; i < entCount; i++){
                int proxID = (int)entArray[i];
                Entity* proxEnt = entities[proxID];
                AIVars* aiProx = ENTAI_BY_ID(proxID);

                if(    proxEnt &&
                    aiProx &&
                    aiProx->brain.type == AI_BRAIN_HYDRA_FORCEFIELD_GENERATOR &&
                    proxEnt->pchar &&
                    proxEnt->pchar->attrCur.fHitPoints > 0)
                {
                    forceFieldOn = 1;
                    break;
                }
            }
        }
        
        if(ai->brain.hydraHead.forceFieldOn != forceFieldOn){
            AIPowerInfo* forceField = ai->brain.hydraHead.forceField;

            ai->brain.hydraHead.forceFieldOn = forceFieldOn;
            
            if(!forceField){
                forceField = ai->brain.hydraHead.forceField = aiChoosePower(e, ai, AI_POWERGROUP_FORCEFIELD);
            }
            
            if(forceField){
                aiSetTogglePower(e, forceField, forceFieldOn);
            }
        }
    }

    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    if (aiCritterDoDefaultBuffs(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

static void aiCritterDoOctopusHead(Entity* e, AIVars* ai){
    int forceFieldOn = 0;
    int livingTentacles = 0;
    
    if(ai->teamMemberInfo.team){
        AITeamMemberInfo* member;
 
        for(member = ai->teamMemberInfo.team->members.list; member; member = member->next){
            Entity* entMember = member->entity;
            AIVars* aiMember = ENTAI(entMember);
            
            if(    aiMember->brain.type == AI_BRAIN_OCTOPUS_TENTACLE &&
                entMember->pchar->attrCur.fHitPoints > 0)
            {
                livingTentacles++;
            }
        }
    }

    if( livingTentacles > 2)
        forceFieldOn = 1;

    if(ai->brain.octopusHead.forceFieldOn != forceFieldOn){
        AIPowerInfo* forceField = ai->brain.octopusHead.forceField;

        ai->brain.octopusHead.forceFieldOn = forceFieldOn;
        
        if(!forceField){
            forceField = ai->brain.octopusHead.forceField = aiChoosePower(e, ai, AI_POWERGROUP_FORCEFIELD);
        }
        
        if(forceField){
            aiSetTogglePower(e, forceField, forceFieldOn);
        }
    }

    aiCritterDoDefaultBrain(e, ai);
}

static void aiCritterDoOutcasts(Entity* e, AIVars* ai){
    U32 time_since = ABS_TIME_SINCE(ai->time.wasAttacked);
    Character* pchar = e->pchar;

    if(!ai->power.toggle.curBuff){
        if(ai->attackTarget.entity){
            F32 hpRatio = pchar->attrCur.fHitPoints / pchar->attrMax.fHitPoints;

            if(    hpRatio < 0.5 &&
                time_since > SEC_TO_ABS(5) &&
                time_since < SEC_TO_ABS(15))
            {
                AIPowerInfo* curToggle = aiChooseTogglePower(e, ai, AI_POWERGROUP_BASIC_ATTACK);

                if(curToggle){
                    float fear_scale;

                    aiSetCurrentBuff(&ai->power.toggle.curBuff, curToggle);

                    aiSetTogglePower(e, curToggle, 1);

                    fear_scale = (float)(50 + rand() % 51) / 100.0;

                    aiFeelingAddModifierEx(    e,
                                            ai,
                                            AI_FEELING_FEAR,
                                            1.0 * fear_scale,
                                            0.5 * fear_scale,
                                            ABS_TIME,
                                            ABS_TIME + SEC_TO_ABS(10));
                }
            }
        }
    }else{
        if(    time_since > SEC_TO_ABS(30) ||
            !ai->attackTarget.entity && time_since > SEC_TO_ABS(10))
        {
            aiSetTogglePower(e, ai->power.toggle.curBuff, 0);
            aiClearCurrentBuff(&ai->power.toggle.curBuff);
        }
    }

    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

static void aiCritterDoRiktiSummoner(Entity* e, AIVars* ai){
    if(ai->attackTarget.entity){
        AIPowerInfo* info = aiChoosePower(e, ai, AI_POWERGROUP_PLANT);

        if(info && info->teamInfo){
            if(!info->teamInfo->usedCount){
                aiUsePower(e, info, NULL);
            }
        }
    }

    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

static void aiCritterDoRiktiUXB(Entity* e, AIVars* ai)
{
    if (e && e->pchar && ai)
    {
        if (!ai->brain.rikti.initialized)
        {
            // check to see if I need to hurt myself
            if (!ai->brain.rikti.fullHealth)
            {
                float maxHP = e->pchar->attrMax.fHitPoints;

                e->pchar->attrCur.fHitPoints = randomFloat( maxHP * 0.1f, maxHP * 0.75f);
            }

            ai->brain.rikti.initialized = true;
            ai->brain.rikti.deathTime = 0;
            return;
        }
        if (ai->brain.rikti.deathTime > 0 && 
            ABS_TIME_SINCE(ai->brain.rikti.deathTime) > SEC_TO_ABS(2))
        {
            e->pchar->attrCur.fHitPoints = 0;
        } 
        if (ai->brain.rikti.deathTime == 0)
        {
            if(e->pchar->attrCur.fHitPoints > e->pchar->attrMax.fHitPoints * 0.99f)
            {
                AIPowerInfo* info = aiChoosePower(e, ai, AI_POWERGROUP_KAMIKAZE);

                if (info)
                    aiUsePower(e, info, NULL);

                ai->brain.rikti.deathTime = ABS_TIME;
            }
        }
    }

    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}



static void aiCritterDoSkyRaidersEngineer(Entity* e, AIVars* ai){
    if(ai->attackTarget.entity){
        AIPowerInfo* info = aiChoosePower(e, ai, AI_POWERGROUP_PLANT);

        if(info && info->teamInfo){
            if(!info->teamInfo->usedCount){
                aiUsePower(e, info, NULL);
            }
        }
    }

    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

static void aiCritterDoSkyRaidersPorter(Entity* e, AIVars* ai){
    if(ai->navSearch->path.tail){
        F32* tailPos = ai->navSearch->path.tail->pos;
        F32 distSQR = distance3Squared(ENTPOS(e), tailPos);

        if(distSQR > SQR(20)){
            AIPowerInfo* info = aiChoosePower(e, ai, AI_POWERGROUP_TELEPORT);

            if(info && !info->power->bActive && ABS_TIME_SINCE(info->time.lastUsed) > SEC_TO_ABS(5)){
                F32 range = aiGetPowerRange(e->pchar, info);
                Vec3 teleportPos;

                if(distSQR < SQR(range)){
                    copyVec3(ai->navSearch->path.tail->pos, teleportPos);

                    vecY(teleportPos) += 1;

                    aiUsePowerOnLocation(e, info, teleportPos);

                    aiDiscardFollowTarget(e, "Teleporting there instead of walking.", false);
                }
            }
        }
    }

    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

static void aiCritterDoSwitchTargets(Entity* e, AIVars* ai)
{
    if(ai->brain.switchTargets.baseTime == 0)
        ai->brain.switchTargets.baseTime = 12.f;
    if(ai->brain.switchTargets.minDistance == 0)
        ai->brain.switchTargets.minDistance = 20.f;

    if(erGetEnt(ai->brain.switchTargets.target) != ai->attackTarget.entity)
    {
        ai->brain.switchTargets.target = erGetRef(ai->attackTarget.entity);
//         ai->brain.switchTargets.lastTargetSwitch = ABS_TIME;
    }

    if(ai->attackTarget.entity && ai->attackTarget.status->taunt.begin &&
        ABS_TIME_SINCE(ai->attackTarget.status->taunt.begin) < ai->attackTarget.status->taunt.duration
        || ai->doNotChangeAttackTarget)
        ; // make sure these don't get screwed up
    else if(ABS_TIME_SINCE(ai->brain.switchTargets.lastTargetSwitch) >
        SEC_TO_ABS(ai->brain.switchTargets.baseTime) && !(rand() % 4))
    {
        AIProxEntStatus* status;
        AIProxEntStatus* bestStatus = NULL;
        Entity* bestTarget = NULL;
        F32 maxDanger = -FLT_MAX;

        for(status = ai->proxEntStatusTable->statusHead; status; status = status->tableList.next)
        {
            if(status->entity && !status->entity->erOwner &&
                status->entity != ai->attackTarget.entity &&
                distance3Squared(ENTPOS(status->entity), ENTPOS(e)) >
                SQR(ai->brain.switchTargets.minDistance))
            {
                if(status->dangerValue > maxDanger)
                {
                    bestTarget = status->entity;
                    maxDanger = status->dangerValue;
                    bestStatus = status;
                }
            }
        }

        if(!bestTarget)
        {
            for(status = ai->proxEntStatusTable->statusHead; status; status = status->tableList.next)
            {
                if(status->entity && !status->entity->erOwner &&
                    status->entity != ai->attackTarget.entity)
                {
                    if(status->dangerValue > maxDanger)
                    {
                        bestTarget = status->entity;
                        maxDanger = status->dangerValue;
                        bestStatus = status;
                    }
                }
            }
        }
        
        if(bestTarget)
        {
            AI_LOG(AI_LOG_SPECIAL, (e, "Switched targets from %s to %s\n", ai->attackTarget.entity->name, bestTarget->name) );
            aiSetAttackTarget(e, bestTarget, bestStatus, "Switchtargets brain", 0, 0);
            ai->brain.switchTargets.target = erGetRef(bestTarget);
            ai->brain.switchTargets.lastTargetSwitch = ABS_TIME;
            if(ai->brain.switchTargets.teleport)
            {
                // copy and paste from tsoo healer, too lazy to make this a function...
                AIPowerInfo* telePower = aiChoosePower(e, ai, AI_POWERGROUP_TELEPORT);
                if(telePower && aiCritterDoTeleport(e, ai, telePower))
                    return;
            }
        }
    }

    aiCritterDoDefaultBrain(e, ai);
}

static void aiCritterDoTheFamily(Entity* e, AIVars* ai)
{
    U32 time_since = ABS_TIME_SINCE(ai->time.wasAttacked);
    Character* pchar = e->pchar;

    if(!ai->power.toggle.curBuff)
    {
        if(ai->attackTarget.entity)
        {
            F32 hpRatio = pchar->attrCur.fHitPoints / pchar->attrMax.fHitPoints;

            if(    hpRatio < 0.5 &&
                time_since > SEC_TO_ABS(5) &&
                time_since < SEC_TO_ABS(15))
            {
                AIPowerInfo* curToggle = aiChooseTogglePower(e, ai, AI_POWERGROUP_BASIC_ATTACK);

                if(curToggle)
                {
                    aiSetCurrentBuff(&ai->power.toggle.curBuff, curToggle);
                    aiSetTogglePower(e, curToggle, 1);
                }
            }
        }
    }
    else
    {
        if(    time_since > SEC_TO_ABS(30) || !ai->attackTarget.entity && time_since > SEC_TO_ABS(10))
        {
            aiSetTogglePower(e, ai->power.toggle.curBuff, 0);
            aiClearCurrentBuff(&ai->power.toggle.curBuff);
        }
    }

    if (aiCritterDoDefaultPacedPowers(e, ai))
        return;
    aiCritterDoDefault(e, ai, 0);
}

static void aiCritterDoVahzilokZombie(Entity* e, AIVars* ai)
{
    // Vahzilok zombies follow their reaper around.

    if(ai->teamMemberInfo.team)
    {
        AITeamMemberInfo* member;
        AITeamMemberInfo* reaper = NULL;
        float reaperDistSQR;

        // Find a reaper.

        for(member = ai->teamMemberInfo.team->members.list; member; member = member->next)
        {
            if(member->entity != e && member->entity->pchar->attrCur.fHitPoints > 0)
            {
                AIVars* aiMember = ENTAI(member->entity);

                // Check if this guy is a reaper, and if he's closer than the current best reaper.

                if(aiMember->brain.type == AI_BRAIN_VAHZILOK_REAPER)
                {
                    float distSQR = distance3Squared(ENTPOS(e), ENTPOS(member->entity));

                    if(!reaper || distSQR < reaperDistSQR)
                    {
                        reaper = member;
                        reaperDistSQR = distSQR;
                    }
                }
            }
        }

        if(reaper)
        {
            AIVars* aiReaper = ENTAI(reaper->entity);

            ai->teamMemberInfo.doNotShareInfoWithMembers = 0;
            ai->targetedSightAddRange = aiReaper->targetedSightAddRange;
            // Follow the reaper around.

            if(!ai->attackTarget.entity && reaperDistSQR > SQR(20))
            {
                CLRB(e->seq->sticky_state, STATE_COMBAT);
                aiBehaviorCombatFinished(e, ENTAI(e)->base);

                if(    !ai->motion.type ||
                    ai->followTarget.type != AI_FOLLOWTARGET_ENTITY ||
                    ai->followTarget.reason != AI_FOLLOWTARGETREASON_LEADER)
                {
                    aiSetFollowTargetEntity(e, ai, reaper->entity, AI_FOLLOWTARGETREASON_LEADER);
                    aiGetProxEntStatus(ai->proxEntStatusTable, reaper->entity, 1);
                    aiConstructPathToTarget(e);
                    aiStartPath(e, 0, 0);
                    ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : (ai->collisionRadius + 10);
                }
            }
        }
        else
        {
            ai->teamMemberInfo.doNotShareInfoWithMembers = 1;
            ai->targetedSightAddRange = 0;

            if(ai->attackTarget.entity)
            {
                if(ABS_TIME_SINCE(ai->attackTarget.status->time.posKnown) < ai->eventMemoryDuration)
                {
                    ai->time.beginCanSeeState = ABS_TIME;
                    ai->magicallyKnowTargetPos = 1;
                }
            }
            else
            {
                CLRB(e->seq->sticky_state, STATE_COMBAT);
                aiBehaviorCombatFinished(e, ENTAI(e)->base);
            }
        }
    }
    else
        aiCritterDoDefault(e,ai,0);
}


// This is a brain to prevent exploitative buff-bots on the architect missions.
// This will only allow one ally to place buffs
void aiCritterDoArchitectAlly(Entity *e, AIVars* ai)
{
    Entity * player_leader = e->erCreator?erGetEnt( e->erCreator ):0; 
    Entity * critter_buffer = aiGetArchitectBuffer(ai);

    if( aiCritterDoAutomaticSummoning(e, ai) )
        return;

    // Only the designated buffer can buff.
    if( player_leader && critter_buffer && (e==critter_buffer) )
    {
        if (aiCritterDoDefaultBuffs(e, ai))
            return;
        if (aiCritterDoDefaultPacedPowers(e, ai))
            return;
    }

    // The combat later will pick a combat power
    aiCritterDoDefault(e, ai, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// A R A C H N O S    A I    B R A I N S
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////////////////

// Returns true if a power was executed (and therefore you shouldn't do anything else this tick)
int aiCritterDoToggleBuff(Entity *e, AIVars *ai, U32 *time, AIPowerInfo **curBuff, int groupFlags, U32 minTimeSinceAttacked, U32 maxTimeSinceAttacked)
{
    if(ABS_TIME_SINCE(*time) > SEC_TO_ABS(10))
    {
        U32 time_since = ABS_TIME_SINCE(ai->time.wasAttacked);

        if(    !*curBuff &&
            time_since > minTimeSinceAttacked &&
            time_since < maxTimeSinceAttacked)
        {
            AIPowerInfo* info;
            info = aiChooseTogglePower(e, ai, groupFlags);

            if(info && !info->power->bActive)
            {
                if (e && e->pchar && (e->pchar->ppowCurrent || e->pchar->ppowQueued)) {
                    // Already trying to execute some other power
                    return 0;
                }

                aiSetCurrentBuff(curBuff, info);
                *time = ABS_TIME;

                aiSetTogglePower(e, info, 1);
            }

            return 1;
        }
        else if(*curBuff &&
            time_since > SEC_TO_ABS(20))
        {
            aiSetTogglePower(e, *curBuff, 0);
            aiClearCurrentBuff(curBuff);
        }
    }
    return 0;
}

Entity* aiCritterFindTeammateInNeed(Entity* e, AIVars *ai, F32 range, AIPowerInfo* info, int targetType)
{
    Entity* weakest=NULL, *myowner;
    F32 dHP;
    F32 weakestdHP=0;
    F32 rangeSQR = range*range;
    F32 distSQR;

    //    myowner only allows owner as target. This section tries to buff teammates
    //    the following section, targets the owner. it is after so that it can replace the teammates
    if(ai->teamMemberInfo.team && (targetType != kTargetType_MyOwner) && (targetType != kTargetType_MyCreator)){
        AITeamMemberInfo* member;
        S32 isTitan = ai->brain.type == AI_BRAIN_MALTA_HERCULES_TITAN;
        
        for(member = ai->teamMemberInfo.team->members.list; member; member = member->next){
            //    valid ents to buff are entities that aren't self and alive.
            //    targettype_mypet requires that the ent is a pet
            Entity *entMember = member->entity;
            int isValidEnt = (entMember != e) && (entMember->pchar->attrCur.fHitPoints > 0);
            if (targetType == kTargetType_MyPet)
            {
                isValidEnt &= (entMember->erOwner == erGetRef(e));
            }
            else if (targetType == kTargetType_MyCreation)
            {
                isValidEnt &= (entMember->erCreator == erGetRef(e));
            }
            if(isValidEnt){
                AttribMod *pmod;
                AttribModListIter iter;
                bool bActive=false;
                const BasePower *ppowBase = info ? info->power->ppowBase : NULL;
                
                if(isTitan){
                    AIVars* aiMember = ENTAI(entMember);
                    
                    if(aiMember->brain.type != AI_BRAIN_MALTA_HERCULES_TITAN){
                        continue;
                    }
                }

                if(info)
                {
                    if(!aiPowerTargetAllowed(e, info, entMember, true))
                        continue;
                }


                if(info && !(info->groupFlags & AI_POWERGROUP_HEAL_ALLY))
                {
                    EntityRef myRef = erGetRef(e);
                    pmod = modlist_GetFirstMod(&iter, &entMember->pchar->listMod);
                    while(pmod!=NULL){
                        if(pmod->ptemplate->ppowBase==ppowBase && pmod->erSource == myRef){
                            bActive=true;
                            break;
                        }
                        pmod = modlist_GetNextMod(&iter);
                    }
                }

                if(bActive)
                    continue;
                
                distSQR = distance3Squared(ENTPOS(e), ENTPOS(entMember));
                if (distSQR <= rangeSQR) {
                    dHP = entMember->pchar->attrMax.fHitPoints - entMember->pchar->attrCur.fHitPoints;
                    // Prefer weak members, otherwise prefer those in melee, 1/X chance of just skipping a guy, so that not everyone heals/buffs the same guy
                    if (!weakest || rand()%3 && (dHP > weakestdHP || (dHP == weakestdHP && ABS_TIME_SINCE(ENTAI(member->entity)->time.wasAttacked) < SEC_TO_ABS(5)))) {
                        weakest = entMember;
                        weakestdHP = dHP;
                    }
                }
            }
        }
    }
    if ((targetType != kTargetType_MyPet) && (targetType != kTargetType_MyCreation))
    {
        EntityRef owner = targetType == kTargetType_MyCreator ? e->erCreator : e->erOwner;
        if(owner && (myowner = erGetEnt(owner)))
        {
            AttribMod *pmod;
            AttribModListIter iter;
            bool bActive=false;
            const BasePower *ppowBase = info ? info->power->ppowBase : NULL;

            if(info && !(info->groupFlags & AI_POWERGROUP_HEAL_ALLY))
            {
                pmod = modlist_GetFirstMod(&iter, &myowner->pchar->listMod);
                while(pmod!=NULL){
                    if(pmod->ptemplate->ppowBase==ppowBase){
                        bActive=true;
                        break;
                    }
                    pmod = modlist_GetNextMod(&iter);
                }
            }

            distSQR = distance3Squared(ENTPOS(e), ENTPOS(myowner));
            if (!bActive && distSQR <= rangeSQR) {
                dHP = myowner->pchar->attrMax.fHitPoints - myowner->pchar->attrCur.fHitPoints;
                // Prefer weak members, otherwise prefer those in melee, 1/X chance of just skipping a guy, so that not everyone heals/buffs the same guy
                if (!weakest || rand()%3 && (dHP > weakestdHP || (dHP == weakestdHP && ABS_TIME_SINCE(ENTAI(myowner)->time.wasAttacked) < SEC_TO_ABS(5)))) {
                    weakest = myowner;
                    weakestdHP = dHP;
                }
            }
        }
    }
    return weakest;
}

Entity* aiCritterFindDeadTeammate(Entity* e, AIVars *ai, F32 range, int targetType)
{
    if(ai->teamMemberInfo.team){
        AIBrainType brainType = ai->brain.type;
        AITeamMemberInfo* member;
        F32 rangeSQR = range*range;
        VillainGroupEnum group = e->villainDef->group;

        for(member = ai->teamMemberInfo.team->members.list; member; member = member->next){
            //    valid ents to target are entities that aren't self and dead.
            //    targettype_deadmypet requires that the ent is a pet
            Entity *entMember = member->entity;
            int entIsPet = 0;
            int isValidEnt = (entMember != e) && (entMember->pchar->attrCur.fHitPoints <= 0);
            if (targetType == kTargetType_DeadMyPet)
            {
                entIsPet = (entMember->erOwner == erGetRef(e));
                isValidEnt &= entIsPet;
            }
            else if (targetType == kTargetType_DeadMyCreation)
            {
                entIsPet = (entMember->erCreator == erGetRef(e));
                isValidEnt &= entIsPet;
            }
            if(isValidEnt){
                Entity* entMember = member->entity;
                float distSQR = distance3Squared(ENTPOS(e), ENTPOS(entMember));
                if (distSQR <= rangeSQR) {
                    AIVars* aiMember = ENTAI(entMember);
                    
                    // Check if I'm allowed to be resurrected.
                    
                    if(!aiMember->isResurrectable){
                        continue;
                    }
                    
                    //    unless the target is a pet 
                    if (!entIsPet)
                    {
                        // check for same villain group
                        //    this is so that members of different groups (family and freakshow) don't rez each other
                        //    the lore doesn't make sense to allow that to happen
                        if(entMember->villainDef->group != group){
                            continue;
                        }
                    }
                        
                    // Check for brain-specific stuff.
                    
                    switch(brainType){
                        case AI_BRAIN_VAHZILOK_REAPER:{
                            if(aiMember->brain.type != AI_BRAIN_VAHZILOK_ZOMBIE){
                                continue;
                            }
                            break;
                        }
                    }
                    
                    // Check for archvillain.

                    if(!ai->power.resurrectArchvillains &&     entMember->villainDef && (entMember->villainDef->rank == VR_ARCHVILLAIN || entMember->villainDef->rank == VR_ARCHVILLAIN2))
                    {
                        continue;
                    }
                    
                    // Check if current sequencer move is selectable (to prevent targetting exploded zombies).
                    
                    if(!seqIsMoveSelectable(entMember->seq)){
                        continue;
                    }

                    return entMember;
                }
            }
        }
    }
    return NULL;
}

Entity* aiCritterFindNearbyDead(Entity* e, AIVars *ai)
{
    int entArray[MAX_ENTITIES_PRIVATE];
    int i, entCount;
    entCount = mgGetNodesInRange(0, ENTPOS(e), (void**)entArray, ARRAY_SIZE(entArray));

    for(i = 0; i < entCount; i++)
    {
        Entity * target = entities[entArray[i]];

        if( target && !target->erOwner && character_TargetMatchesType(e->pchar, target->pchar, kTargetType_DeadFriend, false) )
        { 
            AIVars* aiMember = ENTAI(target);

            // Check if I'm allowed to be resurrected.
            if(!aiMember->isResurrectable)
                continue;

            // Check for archvillain.
            if(!ai->power.resurrectArchvillains && target->villainDef && (target->villainDef->rank == VR_ARCHVILLAIN || target->villainDef->rank == VR_ARCHVILLAIN2))
                continue;

            if(!seqIsMoveSelectable(target->seq))
                continue;

            return target;
        }
    }

    return NULL;
}

static int hasTeammateUsedBuff(Entity *e, AIVars *ai, AIPowerInfo *info, U32 delayFromTeammates)
{
    if (info->teamInfo) {
        if (info->teamInfo->inuseCount > 0)
            return 1;
        if (ABS_TIME_SINCE(info->teamInfo->time.lastStoppedUsing) < delayFromTeammates)
            return 1;
    }

    return 0;
}

// Returns true if a power was executed (and therefore you shouldn't do any other powers this tick)
int aiCritterDoBuff(Entity *e, AIVars *ai, U32 *time, AIPowerInfo **curBuff, int groupFlags, U32 minTimeSinceAttacked, U32 maxTimeSinceAttacked, U32 delayFromTeammates)
{
    if(ABS_TIME_SINCE(*time) > SEC_TO_ABS(2)) // && rand()%2) // If we did anything related to buffing in the last X seconds, don't even check
    {
        U32 time_since;
        if (minTimeSinceAttacked) {
            // If there's any minimum time (i.e. healing buffs), only wait if *you* have been attacked recently,
            //    if a friend has been attacked, we *want* to heal them, ASAP!
            time_since = ABS_TIME_SINCE(ai->time.wasAttacked);
        } else {
            // If we have no minimum time (i.e. attack buffs that go off when entering battle), do it
            //    as soon as anyone in my team was attacked
            time_since = ABS_TIME_SINCE(ai->time.friendWasAttacked);
        }

        // IF: We don't have a current target
        //    AND: It has been more than the specified amount of time
        //    AND: Either it has been less than the max time and we have a target (debuffs)
        //        OR: there is no max time (buffs get cast even when not in immediate battle)
        if(    !*curBuff &&
            time_since > minTimeSinceAttacked &&
            ((time_since < maxTimeSinceAttacked && ai->attackTarget.entity) || maxTimeSinceAttacked==0))
        {
            AIPowerInfo* info;
            int togglePriority = -1;
            info = aiChoosePower(e, ai, groupFlags);

            if(info && !info->power->bActive && !info->power->fTimer)
            {
                // check team status and determine if anyone else is using this power recently, if so, abort
                if (!(info->groupFlags & (AI_POWERGROUP_HEAL_ALLY | AI_POWERGROUP_HEAL_SELF))) {
                    // Other than healing powers, if someone else just used this power, wait a bit before
                    //  using it again.
                    delayFromTeammates = 0;
                }
                if (!info->allowMultipleBuffs && hasTeammateUsedBuff(e, ai, info, delayFromTeammates))
                    return 0;

                // Check to see if another toggle power is going
                if (info->isToggle) {
                    AIPowerInfo *oldToggle=NULL;
                    int oldTogglePriority=0;

                    togglePriority = aiCritterGetStagePriority(info->groupFlags);

                    // If this is a toggle power, don't use it if we're using *any* other toggle powers
                    if (ai->power.toggle.curBuff && ai->power.toggle.curBuff->isToggle) {
                        oldToggle = ai->power.toggle.curBuff;
                        oldTogglePriority = -1;
                    }
                    if (ai->power.toggle.curDebuff && ai->power.toggle.curDebuff->isToggle) {
                        oldToggle = ai->power.toggle.curDebuff;
                        oldTogglePriority = -1;
                    }
                    if (ai->power.toggle.curPaced && ai->power.toggle.curPaced->isToggle) {
                        oldToggle = ai->power.toggle.curPaced;
                        oldTogglePriority = ai->power.toggle.curPacedPriority;
                    }
                    if (oldToggle && oldTogglePriority >= togglePriority) {
                        // Old one is just as important
                        return 0;
                    }
                    if (oldToggle) {
                        // Shut off the old one!
                        aiSetTogglePower(e, oldToggle, 0);
                        // Clear the old one (not really necessary, since it'll do it automatically in 20 seconds)
                        if (oldToggle == ai->power.toggle.curBuff) {
                            aiClearCurrentBuff(&ai->power.toggle.curBuff);
                        }
                        if (oldToggle == ai->power.toggle.curDebuff) {
                            aiClearCurrentBuff(&ai->power.toggle.curDebuff);
                        }
                        if (oldToggle == ai->power.toggle.curPaced) {
                            aiClearCurrentBuff(&ai->power.toggle.curPaced);
                        }
                    }
                }

                if(info->groupFlags & AI_POWERGROUP_HEAL_SELF)
                {
                    if(info->groupFlags & AI_POWERGROUP_STAGES){
                        int stage = aiCritterDetermineCurrentStage(e, ai);
                        
                        if(!stage || !(stage & info->groupFlags)){
                            return 0;
                        }
                    }
                    else if(e->pchar->attrCur.fHitPoints > e->pchar->attrMax.fHitPoints * 0.75){
                        return 0;
                    }
                }

                if (e && e->pchar && (e->pchar->ppowCurrent || e->pchar->ppowQueued)) {
                    // Already trying to execute some other power
                    return 0;
                }

                if (aiCritterExecuteBuff(e, ai, info)) {
                    *time = ABS_TIME;
                    aiSetCurrentBuff(curBuff, info);

                    return 1;
                } else {
                    return 0;
                }
            }
        }
        else if(*curBuff){
            // Turn off toggle powers after 20 seconds
            if ((*curBuff)->isToggle && 
                    (    ((*curBuff)->doNotToggleOff && !(*curBuff)->power->bActive) ||
                        (!(*curBuff)->doNotToggleOff && 
                            ABS_TIME_SINCE(*time) > SEC_TO_ABS(20) && 
                            !(rule30Rand()%5))
                    )
                )
            {
                aiSetTogglePower(e, *curBuff, 0);
                aiClearCurrentBuff(curBuff);
            }
            else if (!(*curBuff)->isToggle && !(*curBuff)->power->bActive){
                // Power doesn't seem to be active, double check that it isn't actually still active
                // (i.e. has a summoned entity hanging around that's attached to the caster)
//                AttribMod *pmod;
//                AttribModListIter iter;
//                bool bActive=false;
//                BasePower *ppowBase = (*curBuff)->power->ppowBase;
//
//                pmod = modlist_GetFirstMod(&iter, &e->pchar->listMod);
//                while(pmod!=NULL){
//                    if(pmod->ptemplate->ppowBase==ppowBase){
//                        bActive=true;
//                        break;
//                    }
//                    pmod = modlist_GetNextMod(&iter);
//                }
//
//                if(!bActive){
                    aiClearCurrentBuff(curBuff);
//                }
            }
        }
    }
    return 0;
}

static void aiCritterActivityCombatDoBrain(Entity* e, AIVars* ai){
    switch(ai->brain.type){
        case AI_BRAIN_CUSTOM_HEAL_ALWAYS:
            aiCritterDoCustomHealAlways(e, ai);
            break;

        case AI_BRAIN_5THCOLUMN_FOG:
        case AI_BRAIN_5THCOLUMN_FURY:
        case AI_BRAIN_5THCOLUMN_NIGHT:
            aiCritterDo5thColumnTroop(e, ai);
            break;

        case AI_BRAIN_5THCOLUMN_VAMPYR:
            aiCritterDo5thColumnVampyr(e, ai);
            break;

        case AI_BRAIN_BANISHED_PANTHEON:
            aiCritterDoBanishedPantheon(e, ai);
            break;

        case AI_BRAIN_BANISHED_PANTHEON_SORROW:
            aiCritterDoBanishedPantheonSorrow(e, ai);
            break;

        case AI_BRAIN_BANISHED_PANTHEON_LT:
            aiCritterDoBanishedPantheonLt(e, ai);
            break;

        case AI_BRAIN_CLOCKWORK:
            aiCritterDoClockwork(e, ai);
            break;

        case AI_BRAIN_COT_THE_HIGH:
            aiCritterDoCotTheHigh(e, ai);
            break;

        case AI_BRAIN_COT_BEHEMOTH:
            aiCritterDoCotBehemoth(e, ai);
            break;

        case AI_BRAIN_COT_CASTER:
            aiCritterDoCotCaster(e, ai);
            break;

        case AI_BRAIN_COT_SPECTRES:
            aiCritterDoCotSpectres(e, ai);
            break;

        case AI_BRAIN_DEVOURING_EARTH_MINION:
            aiCritterDoDevouringEarthMinion(e, ai);
            break;

        case AI_BRAIN_DEVOURING_EARTH_LT:
            aiCritterDoDevouringEarthLt(e, ai);
            break;

        case AI_BRAIN_DEVOURING_EARTH_BOSS:
            aiCritterDoDevouringEarthBoss(e, ai);
            break;

        case AI_BRAIN_FREAKSHOW:
            aiCritterDoFreakshow(e, ai);
            break;

        case AI_BRAIN_FREAKSHOW_TANK:
            aiCritterDoFreakshowTank(e, ai);
            break;

        case AI_BRAIN_HYDRA_HEAD:
            aiCritterDoHydraHead(e, ai);
            break;

        case AI_BRAIN_OCTOPUS_HEAD:
            aiCritterDoOctopusHead(e, ai);
            break;
        
        case AI_BRAIN_OUTCASTS:
            aiCritterDoOutcasts(e, ai);
            break;

        case AI_BRAIN_RIKTI_SUMMONER:
            aiCritterDoRiktiSummoner(e, ai);
            break;

        case AI_BRAIN_RIKTI_UXB:
            aiCritterDoRiktiUXB(e, ai);
            break;

        case AI_BRAIN_SKYRAIDERS_ENGINEER:
            aiCritterDoSkyRaidersEngineer(e, ai);
            break;

        case AI_BRAIN_SKYRAIDERS_PORTER:
            aiCritterDoSkyRaidersPorter(e, ai);
            break;

        case AI_BRAIN_SWITCH_TARGETS:
            aiCritterDoSwitchTargets(e, ai);
            break;

        case AI_BRAIN_THEFAMILY:
            aiCritterDoTheFamily(e, ai);
            break;

        case AI_BRAIN_TSOO_MINION:
            aiCritterDoTsooMinion(e, ai);
            break;

        case AI_BRAIN_TSOO_HEALER:
            aiCritterDoTsooHealer(e, ai);
            break;

        case AI_BRAIN_VAHZILOK_ZOMBIE:
            aiCritterDoVahzilokZombie(e, ai);
            break;

        case AI_BRAIN_ARACHNOS_WEB_TOWER:
            aiCritterDoArachnosWebTower(e, ai);
            break;

        case AI_BRAIN_ARACHNOS_REPAIRMAN:
            aiCritterDoArachnosRepairman(e, ai);
            break;

        case AI_BRAIN_ARACHNOS_DR_AEON:
            aiCritterDoArachnosDrAeon(e, ai);
            break;
/*
        case AI_BRAIN_ARACHNOS_MAKO:
            aiCritterDoArachnosMako(e, ai);
            break;
*/
        case AI_BRAIN_ARACHNOS_LORD_RECLUSE_STF:
            aiCritterDoArachnosLordRecluseSTF(e, ai);
            break;

        case AI_BRAIN_ARCHITECT_ALLY:
            aiCritterDoArchitectAlly(e, ai);
            break;

        default:
            aiCritterDoDefaultBrain(e, ai);
            break;
    }
}

static void aiCritterUpdateGriefedState(Entity* e, AIVars* ai){
    AITeam* team = ai->teamMemberInfo.team;
    int    teamMatesKilled = team ? team->members.killedCount : 0;

    if(!ai->isGriefed){
        if(    // I have a team (can be just me).
            team
            &&
            // The team was first attacked more than x seconds before it was last attacked.
            ( team->time.wasAttacked && ABS_TIME_SINCE(team->time.firstAttacked) >
                                ABS_TIME_SINCE(team->time.wasAttacked) + SEC_TO_ABS(5))        
            &&
            // I have a target.
            ai->attackTarget.entity
            &&
            // I've been targeting for more than x seconds.
            ABS_TIME_SINCE(ai->attackTarget.time.set) > ai->griefed.time.begin
            &&
            // Team hasn't attacked for more than x seconds.
            ABS_TIME_SINCE(team->time.didAttack) > ai->griefed.time.begin
            &&
            // Team was attacked in the last x seconds.
            ABS_TIME_SINCE(team->time.wasAttacked) < ai->griefed.time.end
            &&
            // Team was attacked more recently than it did an attack (by more than x seconds).
            (team->time.wasAttacked && ABS_TIME_SINCE(team->time.didAttack) > ABS_TIME_SINCE(team->time.wasAttacked) + SEC_TO_ABS(5))
            &&
            (
                // Someone on the team is under their grief HP.
                team->members.underGriefedHPCount
                ||
                // A teammate has died. (take into account the griefedHPRatio)
                ((F32)teamMatesKilled * ai->griefed.hpRatio / 0.7 >= 1.0f)
            )
            &&
            (
                //    if ent is a pet, is an architect ally, and not the architect buffer, he can't be griefed (to run far away)
                (ai->brain.type != AI_BRAIN_ARCHITECT_ALLY) || 
                !e->petByScript || 
                (e == aiGetArchitectBuffer(ai))
            )
        )
        {
            ai->isGriefed = 1;

            ai->time.begin.griefedState = ABS_TIME;
        }
    }else{
        if(    // I've been in griefed mode for x seconds.
            ABS_TIME_SINCE(ai->time.begin.griefedState) > ai->griefed.time.end
            &&
            // I haven't been attacked for x seconds.
            ABS_TIME_SINCE(team->time.wasAttacked) >= ai->griefed.time.begin
            ||
            // I attacked within x seconds of being attacked.
            team->time.wasAttacked && ABS_TIME_SINCE(team->time.didAttack) < ABS_TIME_SINCE(team->time.wasAttacked) + SEC_TO_ABS(5))
        {
            ai->isGriefed = 0;

            ai->time.begin.griefedState = ABS_TIME;
        }
    }
}

static struct {
    Entity* entity;
    AIVars* ai;
    Beacon* closestCombatBeacon;
} staticCheckAvoidBeacon;

static int aiCheckIfShouldAvoidBeacon(Beacon* beacon){
    BeaconAvoidNode* node;
    
    for(node = beacon->avoidNodes; node; node = node->beaconList.next){
        Entity* entAvoid = node->entity;
        AIVars* aiAvoid = ENTAI(entAvoid);
        S32 beaconEntityLevel = (staticCheckAvoidBeacon.entity->pchar->bIgnoreCombatMods ? (MAX_PLAYER_SECURITY_LEVEL+4) : staticCheckAvoidBeacon.entity->pchar->iCombatLevel);
        S32 avoidEntityLevel = entAvoid->pchar->bIgnoreCombatMods ? (MAX_PLAYER_SECURITY_LEVEL+4) : entAvoid->pchar->iCombatLevel;
        S32 levelDiff = beaconEntityLevel - avoidEntityLevel;
        AIAvoidInstance* avoid;
        
        for(avoid = aiAvoid->avoid.list; avoid; avoid = avoid->next){
            if(    levelDiff <= avoid->maxLevelDiff &&
                distance3Squared(ENTPOS(entAvoid), posPoint(beacon)) <= avoid->radius)
            {
                return 0;
            }
        }
    }
    
    return beaconGalaxyPathExists(staticCheckAvoidBeacon.closestCombatBeacon, beacon, 0,
                    staticCheckAvoidBeacon.ai->canFly);
}

void aiCritterActivityCombat(Entity* e, AIVars* ai){
    float                preferredAttackDistance;
    float                currentAttackDistance;
    int                    needToMove = 1;
    int                    inPreferredRange = 0;
    int                    isAfraid;
    int                    isAfraidFromAI;
    int                    isAfraidFromPowers;
    int                    isStunned;
    int                    isGriefed;
    int                    isTerrorized;
    int                    isInAvoidSphere;
    Entity*                target;
    AIProxEntStatus*    targetStatus;
    AIVars*                aiTarget;
    Character*            pchar;
    AITeam*                team = ai->teamMemberInfo.team;

    if(ai->preferGroundCombat && ai->attackTarget.entity && ai->isFlying && !ai->navSearch->path.head
        && !ai->motion.ground.notOnPath && !ai->inputs.flyingIsSet && !ai->alwaysFly)
    {
        F32 distSqr = distance3Squared(ENTPOS(e), ENTPOS(ai->attackTarget.entity));
        F32 powerRange = ai->power.current ? aiGetPowerRange(e->pchar, ai->power.current) : FLT_MAX;

        if(distSqr < SQR(powerRange))
        {
            F32 dyMe = beaconGetDistanceYToCollision(ENTPOS(e), -1 * 20);
            F32 dyTarget = beaconGetDistanceYToCollision(ENTPOS(ai->attackTarget.entity), -1 * 20);

            if(dyMe <= 10 && dyTarget <= 10)
            {
                aiSetFlying(e, ai, 0);
                aiMotionSwitchToNone(e, ai, "I want to stop flying");
            }
        }
    }

    aiCritterActivityCombatDoBrain(e, ai);

    if(!ai->doAttackAI)
    {
        aiDiscardAttackTarget(e, "Not doing attack AI.\n");
        return;
    }

    
    // Get a local copy of my pchar.
    pchar = e->pchar;

    // Update my afraid states.
    isAfraid = ai->isAfraidFromAI || ai->isAfraidFromPowers;
    
    if(ai->isAfraidFromAI)
    {
        isAfraidFromAI = ai->isAfraidFromAI = ai->feeling.total.level[AI_FEELING_FEAR] > 0.8 * ai->feeling.total.level[AI_FEELING_CONFIDENCE];
    }
    else
    {
        isAfraidFromAI = ai->isAfraidFromAI = ai->feeling.total.level[AI_FEELING_FEAR] > ai->feeling.total.level[AI_FEELING_CONFIDENCE];
    }
    isAfraidFromPowers = ai->isAfraidFromPowers = pchar->attrCur.fAfraid > 0;
    
    if(isAfraid != (ai->isAfraidFromAI || ai->isAfraidFromPowers))
    {
        isAfraid = ai->isAfraidFromAI || ai->isAfraidFromPowers;
        ai->time.begin.afraidState = ABS_TIME;

        if(    !isAfraid && ai->followTarget.reason == AI_FOLLOWTARGETREASON_ESCAPE)
        {
            aiDiscardFollowTarget(e, "Not afraid anymore.", false);
        }
    }

    // Update my terrorized state.
    isTerrorized = pchar->attrCur.fTerrorized > 0;

    if(ai->isTerrorized != isTerrorized)
    {
        ai->isTerrorized = isTerrorized;
        
        if(isTerrorized)
        {
            aiDiscardFollowTarget(e, "Terrorized now!", false);
        }
        else
        {
            aiDiscardFollowTarget(e, "Not terrorized anymore.", false);
        }
    }

    // Update my stunned state.

    isStunned = pchar->attrCur.fStunned > 0;

    if(ai->isStunned != isStunned)
    {
        ai->isStunned = isStunned;

        ai->time.begin.stunnedState = ABS_TIME;
    }

    // Update my griefed state.

    aiCritterUpdateGriefedState(e, ai);

    isGriefed = ai->isGriefed;

    // Check if I can see my attack target.

    PERFINFO_AUTO_START("UpdateTargetVis", 1);
        aiCritterUpdateAttackTargetVisibility(e, ai);
    PERFINFO_AUTO_STOP();

    // Get my current target information.

    target = ai->attackTarget.entity;

    if(target)
    {
        targetStatus = ai->attackTarget.status;
        aiTarget = ENTAI(ai->attackTarget.entity);
    }
    else
    {
        targetStatus = NULL;
        aiTarget = NULL;
    }

    // IF: I'm not bothered.
    // THEN: Do nothing.

     if(    !ai->helpTarget && 
        ai->feeling.total.level[AI_FEELING_BOTHERED] < 0.5 &&
        !ai->proxEntStatusTable->avoid.head &&
        !ai->alwaysAttack)
    {
        // If in script mode, we always want them to be "bothered" & therefore attack.
        
        return;
    }

    // Pick a power.
    aiCritterActivityCombatChoosePower(e, ai);

    // If I have an attack power readied, then stay near the outer edge of that range.
    if(ai->power.preferred)
    {
        preferredAttackDistance = aiGetPowerRange(pchar, ai->power.preferred);
    }
    else
    {
        //    if they have no power set, but they prefer range, give them a default distance they want to be in
        //    range critters have a large distance. if the ent is closer than this distance, ent will close to target
        //    otherwise it won't move
        if (ai->preferRanged)
            preferredAttackDistance = 60;        
        else
            preferredAttackDistance = 10;
    }

    if(ai->power.current)
    {
        currentAttackDistance = aiGetPowerRange(pchar, ai->power.current);
    }
    else
    {
        currentAttackDistance = 0;
    }

    if(aiTarget)
    {
        float fCollisionDist = character_TargetCollisionRadius(e,target);
        preferredAttackDistance += fCollisionDist;
        currentAttackDistance += fCollisionDist;
    }
    else
    {
        preferredAttackDistance = 0;
        currentAttackDistance = 0;
    }

    // Check if my path is still good enough for getting near my target.

    aiCritterActivityCombatCheckPathTarget(e, ai);

    if(!isStunned && target)
    {
        // IF: I can see my target,
        // THEN: Try attacking if I'm in range,
        // ELSE: Go to last known position.

        if(ai->canHitTarget)
        {
            F32 distSQR = distance3Squared(ENTPOS(e), ENTPOS(target));

            inPreferredRange = distSQR < SQR(preferredAttackDistance);

            // If I'm not afraid or I've been afraid for more than a few seconds, then attack.

            if(    distSQR < SQR(currentAttackDistance) &&
                ((!isAfraid && !isTerrorized) ||
                    ai->time.wasDamaged && ABS_TIME_SINCE(ai->time.wasDamaged) + SEC_TO_ABS(5) < ABS_TIME_SINCE(ai->time.didAttack)))
            {
                AIPowerInfo* current = ai->power.current;
                AIPowerInfo* preferred = ai->power.preferred;

                // IF: I'm in attack range of my target,
                //   THEN: send a message saying so.

                aiSendMessageInt(e, AI_MSG_ATTACK_TARGET_IN_RANGE, 1);

                // Not allowed to discard attack target when handling this message.

                assert(ai->attackTarget.entity);

                // IF: My preferred power's range is less than my current power's range,
                //   AND: I'm not currently using my preferred power,
                //        OR: I'm using my preferred stance
                //        OR: I'm not in a stance
                //        OR: My current power will take more than 10 seconds to recharge
                //   AND: I'm in range of my preferred power,
                //     OR: My current power has more than five seconds left to recharge.

                if(    !ai->doNotChangePowers
                    &&
                    preferred != current
                    &&
                    (    (preferredAttackDistance < currentAttackDistance)
                        ||
                        (preferred &&
                        (    !preferred->stanceID
                            ||
                            !ai->power.lastStanceID
                            ||
                            preferred->stanceID == ai->power.lastStanceID))
                        ||
                        (current &&
                        current->power->fTimer > 10.0)
                    )
                    &&
                    (    inPreferredRange
                        ||
                        (current &&
                        current->power->fTimer >= 5.0 &&
                        preferred &&
                        preferred->power->fTimer < 5.0)))
                {
                    AI_LOG(    AI_LOG_COMBAT,
                            (e,
                            "Switching to preferred power: ^5%s\n",
                            preferred->power->ppowBase->pchName));

                    aiSetCurrentPower(e, preferred, current ? 0 : 1, 0);
                }

                ai->targetWasInRange = 1;
            }
        }else{
            // IF: I'm allowed to move,
            //   AND: I can't see my target for 20 seconds,
            //   AND: I magically know his position,
            // THEN: Go to last known position.

            if(    !ai->doNotMove &&
                ABS_TIME_SINCE(ai->time.beginCanSeeState) >= SEC_TO_ABS(20) &&
                ai->magicallyKnowTargetPos)
            {
                // Run to the last known position.

                AI_LOG(    AI_LOG_COMBAT,
                        (e, "%s ^0unseen for ^4%d ^0seconds, running to last known point.\n",
                        AI_LOG_ENT_NAME(target),
                        ABS_TO_SEC(ABS_TIME_SINCE(ai->time.beginCanSeeState))));

                aiSetFollowTargetEntity(e, ai, target, AI_FOLLOWTARGETREASON_ATTACK_TARGET_LASTPOS);

                aiConstructPathToTarget(e);

                aiDiscardAttackTarget(e, "Going to last known position.");

                aiStartPath(e, 1, 0);

                ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 10;

                // Set this so that I won't update my movement target.

                ai->magicallyKnowTargetPos = 0;
            }
        }
    }

    // Get my current target information, again.

    target = ai->attackTarget.entity;

    if(target){
        targetStatus = ai->attackTarget.status;
        aiTarget = ENTAI(ai->attackTarget.entity);
    }else{
        targetStatus = NULL;
        aiTarget = NULL;
    }

    // IF: I'm not allowed to move,
    // OR: I'm in range of my preferred power,
    //   AND: The follow target was discarded in the message handler,
    //        OR: I'm moving because I'm circling combat
    // THEN: Don't try to move.

    if(    ai->doNotMove ||
        inPreferredRange && (!ai->followTarget.type || ai->followTarget.reason == AI_FOLLOWTARGETREASON_CIRCLECOMBAT))
    {
        needToMove = 0;
    }

    // IF: I have a target,
    // AND: I'm not afraid,
    // AND: I'm not stunned,
    // AND: I'm not in preferred-weapon range,
    // THEN: Send an out-of-range message.

    if(    target &&
        !isStunned &&
        !isAfraid &&
        !inPreferredRange)
    {
        aiSendMessageInt(e, AI_MSG_ATTACK_TARGET_IN_RANGE, 0);
    }
    
    isInAvoidSphere = 0;

    if(ai->proxEntStatusTable->avoid.head){
        AIProxEntStatus* status = ai->proxEntStatusTable->avoid.head;
        
        for(; status; status = status->avoidList.next){
            Entity* proxEnt = status->entity;
            
            if(proxEnt && proxEnt->pchar && aiIsLegalAttackTarget(proxEnt, e)){
                AIVars* aiProx = ENTAI(proxEnt);
                
                if(aiProx){
                    F32 distSQR = distance3Squared(ENTPOS(e), ENTPOS(status->entity));
                    S32 entLevel = (e->pchar->bIgnoreCombatMods ? (MAX_PLAYER_SECURITY_LEVEL+4) : e->pchar->iCombatLevel);
                    S32 proxEntLevel = (proxEnt->pchar->bIgnoreCombatMods ? (MAX_PLAYER_SECURITY_LEVEL+4) : proxEnt->pchar->iCombatLevel);
                    S32 combatLevelDiff = entLevel - proxEntLevel;
                    F32 collisionRadii = ai->collisionRadius + aiProx->collisionRadius;
                    AIAvoidInstance* avoid;
                    
                    for(avoid = aiProx->avoid.list; avoid; avoid = avoid->next){
                        if(combatLevelDiff <= avoid->maxLevelDiff){
                            F32 maxDist = avoid->radius + collisionRadii;
                            F32 maxDistSQR = SQR(maxDist);
                            
                            if(distSQR < maxDistSQR){
                                isInAvoidSphere = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    // IF: I have an attack target,
    //   OR: I am stunned,
    //   OR: I have a help target,
    // AND: My find-path timer has expired,
    // THEN: Figure out if I need to find a path.

    if( (isStunned || isInAvoidSphere || target || ai->helpTarget) &&
        ABS_TIME_SINCE(ai->time.foundPath) > SEC_TO_ABS(1))
    {
        int getPath = 0;
        int isTaunted = ai->attackTarget.status &&
                        ai->attackTarget.status->taunt.duration &&
                        ABS_TIME_SINCE(ai->attackTarget.status->taunt.begin) < ai->attackTarget.status->taunt.duration;

        // If I still magically know my target's position, then go there.

        if(isStunned){
            // I'm stunned, so just amble around like an idiot.

            int hasTarget = ai->followTarget.type &&
                            ai->followTarget.reason == AI_FOLLOWTARGETREASON_STUNNED;

            if(    ABS_TIME_SINCE(ai->time.begin.stunnedState) > SEC_TO_ABS(2) &&
                (    !hasTarget ||
                    ABS_TIME_SINCE(ai->time.foundPath) > SEC_TO_ABS(2)))
            {
                if(hasTarget){
                    aiDiscardFollowTarget(e, "Stunned, standing still for a while.", false);

                    ai->time.foundPath = ABS_TIME;
                }else{
                    // IF: I've been stunned,
                    // THEN: walk somewhere away from either my target or myself.

                    Beacon* destBeacon;
                    const F32*    pos;

                    AI_LOG(AI_LOG_PATH, (e, "Stunned and ambling around!\n"));

                    if(target){
                        pos = ENTPOS(target);
                    }else{
                        pos = ENTPOS(e);
                    }

                    destBeacon = aiFindBeaconInRange(e, ai, pos, 100, 100, 20, 70, NULL);

                    if(destBeacon){
                        aiDiscardFollowTarget(e, "Stunned and heading for beacon.", false);

                        aiSetFollowTargetPosition(e, ai, posPoint(destBeacon), AI_FOLLOWTARGETREASON_STUNNED);

                        ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : (10 + ai->collisionRadius);

                        getPath = 1;
                    }
                }
            }
        }
        else if(isInAvoidSphere){
            if(    !ai->motion.type &&
                (    ai->followTarget.type != AI_FOLLOWTARGET_POSITION ||
                    ai->followTarget.reason != AI_FOLLOWTARGETREASON_AVOID))
            {
                Beacon* destBeacon;
                
                getPath = 1;
                
                staticCheckAvoidBeacon.entity = e;
                staticCheckAvoidBeacon.ai = ai;
                staticCheckAvoidBeacon.closestCombatBeacon = beaconGetClosestCombatBeacon(ENTPOS(e), 1, NULL, GCCB_PREFER_LOS, NULL);

                if(devassertmsg(staticCheckAvoidBeacon.closestCombatBeacon, "Should be able to find a combat beacon but can't"))
                {
                    destBeacon = aiFindBeaconInRange(e, ai, ENTPOS(e), 100, 100, 20, 70, aiCheckIfShouldAvoidBeacon);

                    if(destBeacon){
                        aiDiscardFollowTarget(e, "Avoiding avoidance area.", false);

                        aiSetFollowTargetPosition(e, ai, posPoint(destBeacon), AI_FOLLOWTARGETREASON_AVOID);

                        getPath = 1;

                        ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : 3;
                    }
                }
            }
            
            ai->doNotRun = ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : 0;
        }
        else if(isAfraidFromAI && !isTaunted
                ||
                isAfraidFromPowers
                ||
                isGriefed && !isTerrorized)
        {
            // IF: I have a target,
            // AND: I'm griefed,
            //   OR: I've been afraid for a while,
            // AND: I'm not following the right type of target,
            // THEN: Run to a distant beacon.

            if(    target
                &&
                (    isGriefed
                    ||
                    ABS_TIME_SINCE(ai->time.begin.afraidState) > SEC_TO_ABS(1))
                &&
                (    !ai->motion.type
                    ||
                    ai->followTarget.type != AI_FOLLOWTARGET_POSITION
                    ||
                    ai->followTarget.reason != AI_FOLLOWTARGETREASON_ESCAPE))
            {
                Beacon* destBeacon;
                const float* pos;

                AI_LOG(AI_LOG_PATH, (e, "Running away!\n"));

                pos = ENTPOS(target);

                if(isGriefed){
                    destBeacon = aiFindBeaconInRange(e, ai, pos, 300, 100, 150, 300, NULL);
                }else{
                    destBeacon = aiFindBeaconInRange(e, ai, pos, 200, 100, 100, 200, NULL);
                }

                if(destBeacon){
                    aiDiscardFollowTarget(e, "Switching to afraid target.", false);

                    aiSetFollowTargetPosition(e, ai, posPoint(destBeacon), AI_FOLLOWTARGETREASON_ESCAPE);

                    ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : (10 + ai->collisionRadius);

                    getPath = 1;
                }
            }
        }
        else if(isTerrorized){
            // Do nothing if terrorized.
            
            getPath = 0;
        }
        else if(ai->helpTarget){
            if(    !ai->motion.type ||
                ai->followTarget.type != AI_FOLLOWTARGET_ENTITY ||
                ai->followTarget.reason != AI_FOLLOWTARGETREASON_NONE ||
                ai->followTarget.entity != ai->helpTarget)
            {
                aiDiscardFollowTarget(e, "Switching to follow my help target.", false);

                aiSetFollowTargetEntity(e, ai, ai->helpTarget, AI_FOLLOWTARGETREASON_NONE);

                ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : (3 + ai->collisionRadius);
                
                getPath = 1;
            }
        }
        else if(needToMove &&
                ai->magicallyKnowTargetPos &&
                (    isTaunted ||
                    ABS_TIME_SINCE(ai->time.begin.afraidState) >= SEC_TO_ABS(3)))
        {
            // IF: I can see my target,
            // AND: and I'm not near enough to attack,
            // THEN: go to my attack target.

            if (ai->followTarget.type != AI_FOLLOWTARGET_ENTITY ||
                    ai->followTarget.reason != AI_FOLLOWTARGETREASON_ATTACK_TARGET ||
                    ai->followTarget.entity != ai->attackTarget.entity)
            {
                aiDiscardFollowTarget(e, "Switching to follow my attack target.", false);

                aiSetFollowTargetEntity(e, ai, ai->attackTarget.entity, AI_FOLLOWTARGETREASON_ATTACK_TARGET);

                ai->followTarget.standoffDistance = ai->inputs.standoffDistance > 0 ? ai->inputs.standoffDistance : MAX(preferredAttackDistance - 3, ai->collisionRadius + aiTarget->collisionRadius);

                getPath = 1;
            }
            else if(!ai->motion.type)
            {
                getPath = 1;
            }
        }

        // IF: I need to get a path,
        // AND: I have a follow target,
        // AND: I don't already have a path,
        // THEN: get a path.

        if(    getPath &&
            ai->followTarget.type &&
            !ai->navSearch->path.head &&
            !e->pchar->attrCur.fImmobilized)
        {
            ai->time.foundPath = ABS_TIME;

            aiConstructPathToTarget(e);

            if(!ai->navSearch->path.head){
                // Path find failed.

                AI_LOG(    AI_LOG_PATH,
                        (e,
                        "Couldn't find path.\n"));

                aiDiscardFollowTarget(e, "Can't find a path.", false);
            }else{
                // Path find succeeded, let's follow it.

                aiStartPath(e, 0, isStunned ? 1 : 0);

                ai->waitingForTargetToMove = 1;
            }
        }
    }
}

void* aiCritterMsgHandlerCombat(Entity* e, AIMessage msg, AIMessageParams params){
    AIVars* ai = ENTAI(e);

    switch(msg){
        case AI_MSG_PATH_BLOCKED:{
            if(ai->preventShortcut){
                aiPathUpdateBlockedWaypoint(ai->navSearch->path.curWaypoint);

                if(ai->attackTarget.entity){
                    // If I'm stuck and short-circuiting is prevented, then just hang out.

                    AI_LOG(    AI_LOG_PATH,
                            (e,
                            "Hanging out until my target moves.\n"));

                    aiDiscardFollowTarget(e, "Waiting for my target to move.", false);

                    ai->waitingForTargetToMove = 1;
                }else{
                    // The target isn't a player, so I'm probably just wandering around.
                    //   I'll just find a new path eventually.

                    AI_LOG(    AI_LOG_PATH,
                            (e,
                            "Got stuck while not targeting a player.\n"));

                    aiDiscardFollowTarget(e, "Ran into something, not sure what.", false);

                    ai->waitingForTargetToMove = 0;
                }
            }else{
                // If I'm stuck and I have already tried shortcutting,
                //   then redo the path but don't allow shortcutting.

                AI_LOG(    AI_LOG_PATH,
                        (e,
                        "I got stuck, trying not shortcutting.\n"));

                aiConstructPathToTarget(e);

                aiStartPath(e, 1, ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : ai->doNotRun);

                ai->waitingForTargetToMove = 1;
            }

            break;
        }

        case AI_MSG_ATTACK_TARGET_MOVED:{
            // The target has moved, so reset the find path timer.

            AI_LOG(    AI_LOG_PATH,
                    (e,
                    "Attack target moved a lot, ^1cancelling path.\n"));

            clearNavSearch(ai->navSearch);

            ai->waitingForTargetToMove = 0;

            break;
        }

        case AI_MSG_BAD_PATH:{
            clearNavSearch(ai->navSearch);

            aiDiscardFollowTarget(e, "Bad path.", false);

            break;
        }

        case AI_MSG_FOLLOW_TARGET_REACHED:{
            int reachedTarget = 0;
            
            if(ai->followTarget.reason == AI_FOLLOWTARGETREASON_ATTACK_TARGET){
                if(ai->canSeeTarget){
                    reachedTarget = 1;
                }else{
                    if(ABS_TIME_SINCE(ai->time.checkedLOS) > SEC_TO_ABS(0.2)){
                        ai->time.checkedLOS = 0;
                        
                        aiCritterUpdateAttackTargetVisibility(e, ai);

                        if(ai->canSeeTarget){
                            reachedTarget = 1;
                        }
                    }
                }
            }else{
                reachedTarget = 1;
            }
            
            if(reachedTarget){
                aiDiscardFollowTarget(e, "Reached follow target.", true);

                ai->time.checkedForShortcut = 0;
                ai->doNotRun = ai->inputs.doNotRunIsSet ? ai->inputs.doNotRun : 0;
            }
            
            break;
        }

        case AI_MSG_ATTACK_TARGET_IN_RANGE:{
            int inRange = params.intData;

            if(inRange){
                AIPowerInfo* info = ai->power.current;
                //AIProxEntStatus* targetStatus = ai->attackTarget.status;
                int isAfraid = ai->isAfraidFromAI || ai->isAfraidFromPowers;//ABS_TIME_SINCE(targetStatus->scared.begin) < targetStatus->scared.duration;
                MotionState* motion = e->motion;

                ai->time.lastInRange = ABS_TIME;

                if(    info &&
                    !motion->jumping &&
                    (motion->input.flying || !motion->falling) &&
                    !e->pchar->ppowCurrent &&
                    !e->pchar->ppowQueued &&
                    !ai->doNotUsePowers &&
                    ai->motion.type != AI_MOTIONTYPE_JUMP)
                {
                    Power* power = info->power;

                    // If I'm in range of my attack target, then uh, attack him or something.

                    SETB(e->seq->sticky_state, STATE_COMBAT);

                    if(0 && ABS_TIME_SINCE(ai->time.beginCanSeeState) <= SEC_TO_ABS(1)){
                        //AI_LOG(    AI_LOG_COMBAT,
                        //        (e,
                        //        "^1NOT ATTACKING %s^0 with ^5%s^0: Not visible for long enough.\n",
                        //        AI_LOG_ENT_NAME(ai->attackTarget.entity),
                        //        power->ppowBase->pchName));
                    }
                    else if(power->bActive){
                        //AI_LOG(    AI_LOG_COMBAT,
                        //        (e,
                        //        "^1NOT ATTACKING %s^0 with ^5%s^0: Power still active.\n",
                        //        AI_LOG_ENT_NAME(ai->attackTarget.entity),
                        //        power->ppowBase->pchName));
                    }
                    else if(power->fTimer > 0){
                        //AI_LOG(    AI_LOG_COMBAT,
                        //        (e,
                        //        "^1NOT ATTACKING %s^0 with ^5%s^0: Power timer: ^4%1.2f^0s\n",
                        //        AI_LOG_ENT_NAME(ai->attackTarget.entity),
                        //        power->ppowBase->pchName, power->fTimer));
                    }
                    else if(isAfraid && ABS_TIME_SINCE(ai->time.didAttack) < SEC_TO_ABS(5)){
                        //AI_LOG(    AI_LOG_COMBAT,
                        //        (e,
                        //        "^1NOT ATTACKING %s^0 with ^5%s^0: I'm afraid and wait longer between attacks.\n",
                        //        AI_LOG_ENT_NAME(ai->attackTarget.entity),
                        //        power->ppowBase->pchName));
                    }
                    else{
                        AI_LOG(    AI_LOG_COMBAT,
                                (e,
                                "^2ATTACKING %s^0 with ^5%s^0!!!\n",
                                AI_LOG_ENT_NAME(ai->attackTarget.entity),
                                power->ppowBase->pchName));

                        //AI_POINTLOG(AI_LOG_COMBAT, (e, posPoint(e), 0xff8888, "ATTACK!"));

                        //entSendAddLine(posPoint(e), 0xffffffff, posPoint(ai->attackTarget.entity), 0xffff0000);

                        aiUsePower(e, info, ai->attackTarget.entity);

                        //    successful or not, don't hold onto this power
                        ai->power.current = 0;

                        ai->time.didAttack = ABS_TIME;

                        if(ai->teamMemberInfo.team){
                            ai->teamMemberInfo.team->time.didAttack = ABS_TIME;
                        }
                    }
                }
            }else{
                CLRB(e->seq->sticky_state, STATE_COMBAT);

                ai->time.chosePower = 0;

                // If my target was in range before, but is now out of range, then go get him.

                if(ai->targetWasInRange && !ai->followTarget.type){
                    clearNavSearch(ai->navSearch);

                    ai->waitingForTargetToMove = 0;

                    ai->targetWasInRange = 0;
                }
            }

            break;
        }
    }

    return 0;
}

