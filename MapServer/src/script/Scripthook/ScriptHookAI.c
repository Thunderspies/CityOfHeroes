/*\
 *
 *    scripthookAI.c - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides all the functions CoH script files should be referencing -
 *        essentially a list of hooks into game systems.
 *
 *    This file contains all functions related to AI
 *
 */

#include "script/script.h"
#include "script/scriptengine.h"
#include "script/scriptutil.h"

#include "cmdparse/cmdserver.h"
#include "storyarc/storyarcprivate.h"
#include "generator/encounterPrivate.h"
#include "ai/entai.h"
#include "ai/entaiLog.h"
#include "ailib/aiBehaviorPublic.h"
#include "ai/entaiBehaviorCoh.h"
#include "ai/entaiScript.h"
#include "ai/entaivars.h"
#include "ai/entaiprivate.h"
#include "svr/svr_player.h"
#include "entity/entPlayer.h"
#include "entity/entGameActions.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/character_target.h"
#include "entity/character_tick.h"
#include "entity/character_pet.h"
#include "entity/entity.h"
#include "seq/seqstate.h"
#include "beacon/beacon.h"

#include "script/Scripthook/ScriptHookInternal.h"

// *********************************************************************************
//  AI Behavior stuff
// *********************************************************************************

void AddAIBehavior(TEAM team, STRING behaviorstring)
{
    int n, i;

    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);

        if (e)
            aiBehaviorAddString(e, ENTAI(e)->base, behaviorstring);
    }
}

void AddAIBehaviorFlag(TEAM team, STRING behaviorstring)
{
    int n, i;

    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if (e)
            aiBehaviorAddFlag( e, ENTAI(e)->base, 0, behaviorstring );
    }
}

void AddAIBehaviorPermFlag(TEAM team, STRING behaviorstring)
{
    int n, i;

    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if (e)
            aiBehaviorAddPermFlag( e, ENTAI(e)->base, behaviorstring );
    }
}

void RemoveAllBehaviors(TEAM team)
{
    int n, i;

    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        
        if (e)
            aiBehaviorAddString(e, ENTAI(e)->base, "Clean");
    }
}

void RemoveAIBehaviorByType(TEAM team, STRING behaviorname)
{
    int n, i;

    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        
        if (e)
            aiBehaviorMarkFinished(e, ENTAI(e)->base, aiBehaviorGetTopBehaviorByName(e, ENTAI(e)->base, behaviorname));
    }
}

NUMBER CurAIBehaviorMatchesName(ENTITY entity, STRING behaviorname)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    
    if (e)
        return aiBehaviorCurMatchesName(e, ENTAI(e)->base, behaviorname);
    else
        return false;
}

NUMBER CurAIBehaviorOverridenByCombat(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    
    if (e)
        return ENTAI(e)->base->overriddenCombat;
    else
        return false;
}

typedef struct ProbString
{
    F32 prob;
    char* string;
} ProbString;

ProbString angryPLs[] = {
    { 1,    "PL_PicketMarch" },
    { 1,    "PL_PicketAngry" },
    { 3,    "PL_Pipe_Angry" },
    { 1,    "PL_Menace" },
    { 3,    "PL_LookOut" },
    { 1,    "PL_Patrol_Pipe" },
    { 1,    "PL_HammerRampage" },
    { 2,    "PL_ArmsCrossed" },
    { 1,    "PL_CrazyMenace" },
    { 1,    "PL_Patrol_Pickaxe" },
    { 1,    "PL_PipeRampage" },
    { 1,    "PL_WanderDynamite" },
    { 4,    "PL_WeaponCheering" },
};
static const int angryPlsCnt = (sizeof(angryPLs)/sizeof(angryPLs[0]));

static STRING GetRandomAngryPL()
{
    STRING buf = 0;
    FRACTION r;
    NUMBER i;
    FRACTION totalProb;
    STRING string = 0;

    //This randomized string should be generalized
    //TO DO you should only have to do this once
    totalProb = 0;
    for ( i = 0; i < angryPlsCnt ; i++)
        totalProb += angryPLs[i].prob;

    r = RandomFraction( 0, totalProb );

    totalProb = 0;
    for ( i = 0; i < angryPlsCnt ; i++) 
    {
        totalProb += angryPLs[i].prob;
        if( r <= totalProb )
        {
            string = angryPLs[i].string;
            break;
        }
    } 

    buf = StringAdd( buf, string );

    return buf;
}

void AddRandomAngryPLAndFight( TEAM team )
{
    int n, i;

    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);

        if (e) {
            STRING pl;
            pl = GetRandomAngryPL();
            assert( pl );
            aiBehaviorAddString(e, ENTAI(e)->base, pl);
            aiBehaviorAddFlag( e, ENTAI(e)->base, 0, "CombatOverride(Aggressive)");
        }
    }
}

// *********************************************************************************
//  AI
// *********************************************************************************

// debug function to determine if an entity should be given AI script commands
bool isAIEntity(int i, TEAM group)
{
    return (   entity_state[i] & ENTITY_IN_USE
            && ! stricmp(entities[i]->name, group));
}

void JoinAITeam( TEAM currAiTeamMembers, TEAM joiners )
{
    int i, n;
    Entity* leader;

    leader = EntTeamInternal(currAiTeamMembers, 0, NULL);

    if( leader )
    {
        n = NumEntitiesInTeam(joiners);
        for (i = 0; i < n; i++)
        {
            Entity* e = EntTeamInternal(joiners, i, NULL);

            if (e)
            {
                AI_LOG(AI_LOG_TEAM, (e, "Scripthook JoinAITeam is joining \"%s\" to \"%s\"\n", joiners, currAiTeamMembers));
                aiTeamJoin(&ENTAI(leader)->teamMemberInfo, &ENTAI(e)->teamMemberInfo);
            }
        }
    }
}

void SetAIMoveTarget(TEAM team, LOCATION movetarget, PRIORITY priority, NUMBER doNotRun, NUMBER releaseOnFinish, FRACTION variation )
{
    int i, n;
    Vec3 pos = {0};

    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        char *behaviorStr = NULL;
        char *combatOverride = NULL;

        if (!e)
            continue;

        if (!GetPointFromLocation(movetarget, pos)) 
            return;

        switch(priority)
        {
            xcase LOW_PRIORITY:
                combatOverride = "Aggressive";
            xcase MEDIUM_PRIORITY:
                combatOverride = "Defensive";
            xcase HIGH_PRIORITY:
                combatOverride = "Passive";
            xdefault:
                combatOverride = "Defensive";
        }

        estrCreate(&behaviorStr);
        estrPrintf(&behaviorStr, "MoveToPos(ScriptLocation(\"%s\"),DoNotRun(%d),CombatOverride(%s),Variation(%f),UseEnterable(1),SpawnPosIsSelf(1),StandoffDistance(10),ReturnToSpawnDistance(10000000)),OverrideSpawnPosCur",
            movetarget, doNotRun, combatOverride, variation);

        if(!releaseOnFinish)
            estrConcatStaticCharArray(&behaviorStr, ",BehaviorDone(CombatOverride(Passive))");

        aiBehaviorAddString(e, ENTAI(e)->base, behaviorStr );
        estrDestroy(&behaviorStr);

        aiBehaviorAddPermFlag( e, ENTAI(e)->base, "DoNotGoToSleep" );
    }
}

// similar to SetAIAttackTarget - designed to break up the movingTeam so they are evenly divided amongst members of targetTeam
void SetAIMoveTargetTeam(TEAM movingTeam, TEAM targetTeam, PRIORITY priority, NUMBER doNotRun, NUMBER releaseOnFinish, FRACTION variation)
{
    int i, targetTeamSize, movingTeamSize, currTarget;

    targetTeamSize = NumEntitiesInTeam(targetTeam);
    if( !targetTeamSize )
    {
        ScriptError("SetAIMoveTargetTeam called with target %s, but this doesn't exist\n", targetTeam);
        return;
    }

    movingTeamSize = NumEntitiesInTeam(movingTeam);
    if( !movingTeamSize )
    {
        ScriptError("SetAIMoveTargetTeam called with mover %s, but this doesn't exist\n", movingTeam);
        return;
    }

    currTarget = RandomNumber(0, targetTeamSize-1);
    for (i = 0; i < movingTeamSize; i++)
    {
        Entity * mover    = EntTeamInternal(movingTeam, i, NULL);
        Entity * target   = EntTeamInternal(targetTeam, currTarget, NULL);

        if (mover && target) {
            SetAIMoveTarget(EntityNameFromEnt(mover), EntityNameFromEnt(target), priority, doNotRun, releaseOnFinish, variation);
        }

        currTarget++;
        if( currTarget > targetTeamSize-1 )
            currTarget = 0;
    }
}

void SetAIStandoffDistance(TEAM team, FRACTION distance)
{
    char buf[500];

    sprintf(buf, "StandoffDistance(%f)", distance);
    AddAIBehaviorPermFlag(team, buf);
}

void SetAIAttackTarget(TEAM attackTeam, TEAM targetTeam, PRIORITY priority)
{
    int i, targetTeamSize, attackTeamSize, currTarget;

    targetTeamSize = NumEntitiesInTeam(targetTeam);
    if( !targetTeamSize )
    {
        ScriptError("SetAIAttackTarget called with target %s, but this doesn't exist\n", targetTeam);
        return;
    }

    attackTeamSize = NumEntitiesInTeam(attackTeam);
    if( !attackTeamSize )
    {
        ScriptError("SetAIAttackTarget called with attacker %s, but this doesn't exist\n", attackTeam);
        return;
    }

    currTarget = RandomNumber(0, targetTeamSize-1);
    for (i = 0; i < attackTeamSize; i++)
    {
        Entity * attacker = EntTeamInternal(attackTeam, i, NULL);
        Entity * target   = EntTeamInternal(targetTeam, currTarget, NULL);

        if (attacker && target) {
            char *attackTargetBehavior = NULL;

            estrCreate(&attackTargetBehavior);
            estrPrintf(&attackTargetBehavior, "AttackTarget(EntRef(%I64x),Priority(%d))", erGetRef(target), priority);
            aiBehaviorAddString(attacker, ENTAI(attacker)->base, attackTargetBehavior);
            estrDestroy(&attackTargetBehavior);
        }

        currTarget++;
        if( currTarget > targetTeamSize-1 )
            currTarget = 0;
    }
}

// Priority Lists are depreciated, but the behaviors system can run them as if they were behaviors
void SetAIPriorityList(TEAM team, STRING prioritylist)
{
    AddAIBehavior(team, prioritylist);
}

//if no leader, then free all the followers from their current leader
void SetFollowers(ENTITY leader, TEAM newFollowerTeam)
{
    SetFollowersEx(leader, newFollowerTeam, true);
}

void SetFollowersEx(ENTITY leader, TEAM newFollowerTeam, NUMBER addDefaultFollowBehavior)
{
    Entity * leaderPtr = NULL;
    int i, n;

    n = NumEntitiesInTeam(newFollowerTeam);
    if (leader)
        leaderPtr = EntTeamInternal(leader, 0, NULL);
    
    if (!leader)
        return;

    for (i = 0; i < n; i++)
    {
        Entity* followerPtr = EntTeamInternal(newFollowerTeam, i, NULL);
        if( leaderPtr )
        {
            if(addDefaultFollowBehavior)
            {
                if (ENTTYPE(followerPtr) == ENTTYPE_NPC)
                    aiBehaviorAddString(followerPtr, ENTAI(followerPtr)->base, "DoNothing");
                else
                    aiBehaviorAddString(followerPtr, ENTAI(followerPtr)->base, "Combat");
            }
            aiScriptSetFollower( leaderPtr, followerPtr );
            followerPtr->audience = erGetRef(leaderPtr);;
        }
        else
        {
            Entity *pOldCreator = erGetEnt(followerPtr->erCreator);

            if (pOldCreator)
                PlayerRemovePet(pOldCreator, followerPtr);

            followerPtr->erCreator = 0;
            followerPtr->erOwner = 0;
            aiDiscardFollowTarget(followerPtr, "Script requests dropping follow.", false);
        }
    }
}

void SetAsPets(ENTITY owner, TEAM petTeam)
{
    Entity * ownerPtr = NULL;
    int i, n;

    if (owner)
        ownerPtr = EntTeamInternal(owner, 0, NULL);
    else 
        return;

    n = NumEntitiesInTeam(petTeam);
    for (i = 0; i < n; i++)
    {
        Entity* petPtr = EntTeamInternal(petTeam, i, NULL);

        if (petPtr && petPtr->pchar)
        {
            character_BecomePet(petPtr->pchar, ownerPtr, NULL);
            

            if( ownerPtr )
            {
                petPtr->commandablePet = 1;
                petPtr->petDismissable = 0;
                aiBehaviorAddString(petPtr, ENTAI(petPtr)->base, "FeelingBothered(1.0),ConsiderEnemyLevel(0),"
                    "FOVDegrees(360),ProtectSpawnRadius(50),ProtectSpawnOuterRadius(0),"
                    "ProtectSpawnDuration(0),WalkToSpawnRadius(15),GriefHPRatio(0),Pet");
                aiBehaviorAddFlag( petPtr, ENTAI(petPtr)->base, 0, "StanceAggressive" );
            } 
            else
            {
                petPtr->commandablePet = 0;
            }
        }
    }
}

void SetAsControlledPets(ENTITY owner, TEAM petTeam)
{
    Entity * ownerPtr = NULL;
    int i, n;

    if (owner)
        ownerPtr = EntTeamInternal(owner, 0, NULL);
    else 
        return;

    n = NumEntitiesInTeam(petTeam);
    for (i = 0; i < n; i++)
    {
        Entity* petPtr = EntTeamInternal(petTeam, i, NULL);

        if (petPtr && petPtr->pchar)
        {
            character_BecomePet(petPtr->pchar, ownerPtr, NULL);
            
            if( ownerPtr && ownerPtr->pchar )
            {
                char *petBehavior = NULL;

                estrCreate(&petBehavior);
                estrPrintf(&petBehavior, "Gang(%d),FeelingBothered(1.0),ConsiderEnemyLevel(0),"
                    "FOVDegrees(360),ProtectSpawnRadius(50),ProtectSpawnOuterRadius(0),"
                    "ProtectSpawnDuration(0),WalkToSpawnRadius(15),GriefHPRatio(0),Pet",
                    ownerPtr->pchar->iGangID);
                aiBehaviorAddString(petPtr, ENTAI(petPtr)->base, petBehavior);
                estrDestroy(&petBehavior);

                SetAIAlly(petTeam, ownerPtr->pchar->iAllyID);
                
                petPtr->commandablePet = 1;
                petPtr->petDismissable = 1;
                
                aiBehaviorAddFlag(petPtr, ENTAI(petPtr)->base, 0, "StanceAggressive" );
            } 
            else
            {
                petPtr->commandablePet = 0;
            }
        }
    }
}

// this is hacked together right now, & does not check for a specific team yet...
NUMBER GetAIReachedObjective(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);

    if(e && aiBehaviorCurMatchesName(e, ENTAI(e)->base, "BehaviorDone"))
        return 1;
    return 0;
}

void MarkAIReachedObjective(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);

    if(e && aiBehaviorCurMatchesName(e, ENTAI(e)->base, "BehaviorDone"))
        aiBehaviorMarkFinished(e, ENTAI(e)->base, aiBehaviorGetCurBehavior(e, ENTAI(e)->base));
    else
        ScriptError("MarkAIReachedObjective called on non-waiting entity");
}

ENTITY GetAIWasHitBy(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if (e)
    {
        AIProxEntStatus* status;
        AIVars* ai = ENTAI(e);
        if (ai)
        {
            for (status = ai->proxEntStatusTable->statusHead; status; status = status->tableList.next)
            {
                if (!status->hasAttackedMe) continue;
                if (status->entity) return EntityNameFromEnt(status->entity);
            }
        }
    }
    return TEAM_NONE;
}

NUMBER GetAIAttackerCount(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if (e)
    {
        return aiGetAttackerCount(e);
    }
    return 0;
}

NUMBER GetAIHasTarget(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if (e)
    {
        AIVars* ai = ENTAI(e);
        if (ai)
        {
            int hasTarget = ai->attackTarget.entity != NULL;

            return hasTarget;
        }
    }
    return 0;
}

void SetChatObjective( TEAM team, ENTITY objectiveStr )
{
    int i, n;
    Entity* e;

    Entity* objective;

    objective = EntTeamInternal( objectiveStr, 0, NULL );

    if( objective )
    {
        n = NumEntitiesInTeam(team);
        for(i = 0; i < n; i++)
        {
            e = EntTeamInternal(team, i, NULL);

            if (e)
                e->objective = erGetRef(objective);
        }
    }
}

void SetChatAudience( TEAM team, ENTITY audienceStr )
{
    int i, n;
    Entity* e;

    Entity* audience;

    audience = EntTeamInternal( audienceStr, 0, NULL );

    if( audience )
    {
        n = NumEntitiesInTeam(team);
        for(i = 0; i < n; i++)
        {
            e = EntTeamInternal(team, i, NULL);
            if (e)
                e->audience = erGetRef(audience);
        }
    }
}

#define MAX_STEALTH_BEFORE_FOLLOWERS_LOSE_YOU 20.0

// This function is NOT NPC friendly...

void FollowerFollows( ENTITY followerName, FRACTION controlRadius, NUMBER noRecapture, NUMBER seeStealth )
{
    Entity* leader = 0;
    Entity* follower = 0;
    Entity *pDebuff = NULL;
    int lastFollowingGood;
    int beenRescued;

    lastFollowingGood = VarGetNumber( "LastFollowingGood" ); //Really should be passed in with other vars
    beenRescued = VarGetNumber( "BeenRescued" );

    follower = EntTeamInternal(followerName, 0, NULL);
    if( !follower || !follower->pchar)        // if the follower couldn't be found or if the follower is not an entity
        return;

    if( follower->erCreator )
        leader = erGetEnt( follower->erCreator );

    if( leader )
    {    
        float distFromLeaderSqr = distance3Squared( ENTPOS(follower), ENTPOS(leader) );
        if( !entAlive(leader) ) 
            leader = 0;
        else if( distFromLeaderSqr > (controlRadius * controlRadius) )
        {
            if( beaconEntitiesInSameCluster( follower, leader ) )
            {
                leader = 0;
            }
            else
            {
                //TO DO maybe a timer or something to give me time to get through the door?
            }
        }

        //For guys who you are just leading, and aren't attacking anything,
        //If the leader turns invisible, boom ,they are gone
        // Unless the SeeStealth flag is provided, in which case we ignore this section (designers now use untargetable combat pets)
        if( follower && follower->untargetable && !seeStealth )
        {
            if( leader && leader->pchar && leader->pchar->attrCur.fStealthRadius > MAX_STEALTH_BEFORE_FOLLOWERS_LOSE_YOU )
            {
                float lookRadius = VarGetFraction( "LookForLeaderInRadius" );
                if (distFromLeaderSqr > (lookRadius * lookRadius))
                    leader = 0;
            }
        }

        //If for any reason you are ever following a friend who is not a player, stop doing that, because it's 
        //Impossible for a player to kill the friend and take you over
        if( leader && ENTTYPE(leader) != ENTTYPE_PLAYER && character_TargetIsFriend( leader->pchar, follower->pchar )   )
            leader = 0;

        // if you're following an enemy who is untargettable, don't do that either.  It's not nice.
        if (leader && leader->untargetable && !character_TargetIsFriend( leader->pchar, follower->pchar ))
            leader = 0; 
    }
    
    if( leader ) //No Change to state 
        return;

    //Look around for a new leader
    {
        Entity * nearbyEnts[MAX_ENTITIES_PRIVATE];
        Entity * missionOwner = 0;
        int nearbyCount, i;
        F32 nearestGuyDistSqr;
        int friendFound = 0;
 
        //Get all the nearby entities (TO DO, why a pointer? change params)
        nearbyCount = entWithinDistance(ENTMAT(follower), VarGetFraction( "LookForLeaderInRadius" ), nearbyEnts, 0, 0);;
  
        if( OnMissionMap() )
            missionOwner = entFromDbId(g_activemission->ownerDbid);

        nearestGuyDistSqr = 1000000.0;

        //Decide who to make your new leader
        for( i = 0 ; i < nearbyCount ; i++ )
        {
            Entity * e = nearbyEnts[i];

            //Don't follow inanimate objects (rough indicator)
            if( !e->pchar || e->do_not_face_target )
                continue;

            //Dont follow yourself
            if( e == follower )
                continue;

            //Dont follow the Dead.
            if( !entAlive(e) )
                continue;

            //Don't follow other pets
            if( e->erCreator || e->erOwner )
                continue;

            //If a friendly player is around, prefer him over all others
            if( character_TargetIsFriend( e->pchar, follower->pchar ) && ENTTYPE(e) == ENTTYPE_PLAYER )
            {
                leader = e;
                friendFound = 1;
                continue;
            }

            //If this is the mission owner, follow him, and you are done
            if( e == missionOwner )
            {
                leader = e; 
                break;
            }
            
            //Only if recapturing is allowed
            if(!noRecapture)
            {    //Otherwise, follow nearest non-friend -- you've been recaptured
                if( !friendFound && !character_TargetIsFriend( e->pchar, follower->pchar ) && !g_ArchitectTaskForce ) // don't allow this to happen on architect missions
                {
                    F32 distSqr = distance3Squared( ENTPOS(e), ENTPOS(follower) );
                    if( distSqr < nearestGuyDistSqr )
                    {
                        leader = e;
                        nearestGuyDistSqr = distSqr; 
                    }
                }
            }
        }
    }

    //////////What new State does this newLeader represent?

    if( !leader && !follower->erCreator ) //I didn't have a leader before, and still don't
    {
        //still stranded...
    }
    else if( !leader && follower->erCreator ) //I had a leader, lost him, and can't find a new one
    {
        Entity *pOldCreator = erGetEnt(follower->erCreator);

        if (pOldCreator)
            PlayerRemovePet(pOldCreator, follower);

        follower->erCreator = 0;
        follower->erOwner = 0;

        aiDiscardFollowTarget(follower, "Script requests dropping follow.", false);

        SetAIPriorityList( followerName, VarGet( "PL_OnStranded" ) );
        if( GetHealth( VarGet( "Rescuee" ) ) > 0.0 )
            Say( followerName, VarGet( "SayOnStranded" ), 2 ); 
        aiBehaviorAddPermFlag(follower, ENTAI(follower)->base, "ReturnToSpawnDistance(10000000),WalkToSpawnRadius(10)");
    }
    else if( leader )  //I found a new leader
    {
        int lastStranded = !follower->erCreator;
        Entity *pOldCreator = erGetEnt(follower->erCreator);

        if (pOldCreator)
            PlayerRemovePet(pOldCreator, follower);

        //Needs to be above Setting PLs because for example PL action hide_behind_Ent expects to have a leader set
        SetFollowers( EntityNameFromEnt(leader), followerName ); 
        aiBehaviorAddPermFlag( follower, ENTAI(follower)->base, "BoostSpeed(1.7)" );//    aiBehaviorAddPermFlag( follower, "BoostSpeed(4.3)" );

        aiBehaviorAddPermFlag(follower, ENTAI(follower)->base, "ReturnToSpawnDistance(15),WalkToSpawnRadius(10),StandoffDistance(10)");

        if( !character_TargetIsFriend( leader->pchar, follower->pchar ) ) //I'm now following a bad guy (only on non-architect maps)
        {
            SetAIPriorityList( followerName, VarGet( "PL_OnRecapture" ) );
            if( (lastStranded || lastFollowingGood) && GetHealth( VarGet( "Rescuee" ) ) > 0.0  ) //If I was stranded, or I just switched from good to bad
            {
                SayOnClick( followerName, VarGet("SayOnClickAfterRecapture") );
                Say( followerName, VarGet( "SayOnRecapture" ), 2 );
            }
            lastFollowingGood = 0;
        }
        if( character_TargetIsFriend( leader->pchar, follower->pchar ) ) //I'm now following a good guy
        {
            if( !beenRescued )
            {
                SetAIPriorityList( followerName, VarGet( "PL_OnRescue" ) );
            } else {
                SetAIPriorityList( followerName, VarGet( "PL_OnReRescue" ) );
            }
 
            if( (lastStranded || !lastFollowingGood) && GetHealth( VarGet( "Rescuee" ) ) > 0.0 ) //If I was stranded , or I just switched from bad to good
            {
                if( !beenRescued )
                {
                    SayOnClick( followerName, VarGet("SayOnClickAfterRescue") );
                    Say( followerName, VarGet( "SayOnRescue" ), 2 );
                }
                else
                {
                    SayOnClick( followerName, VarGet("SayOnClickAfterRescue") );
                    Say( followerName, VarGet( "SayOnReRescue" ), 2 );
                }
            }
            lastFollowingGood = 1;
            beenRescued = 1;
        }
    }

    //Should some or all of this be in the changed priority list?
    { //Possibly a big hack. 
        AIVars* ai = ENTAI(follower);
        //If you are following a good guys, fight 

        //Record this before you start monkeying with it
        if( !VarGetNumber("AttackStatePreserved") ) 
        {
            VarSetNumber( "AttackStatePreserved", 1 );
            VarSetNumber( "PreservedAttackAIHack", ai->doAttackAI );
            VarSetNumber( "PreservedUntargetableHack", follower->untargetable );
        }

        if( leader && character_TargetIsFriend(leader->pchar, follower->pchar) )
        {
            //Do your normal attacking if you have any
            follower->untargetable = VarGetNumber( "PreservedUntargetableHack" );
            ai->doAttackAI = VarGetNumber( "PreservedAttackAIHack" );
        }
        else //Otherwise, be passive
        {
            follower->untargetable |= UNTARGETABLE_GENERAL; 
            ai->doAttackAI = 0;
        }

        ai->useEnterable = 1;
        //ai->spawnPosIsSelf = 1; //Probably needs to be zero
    }
 
    VarSetNumber( "LastFollowingGood", lastFollowingGood ); 
    VarSetNumber( "BeenRescued", beenRescued );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
void FollowerFollowsLeader( ENTITY followerName, FRACTION controlRadius )
{
    Entity* leader = 0;
    Entity* follower = 0;
    Entity *pDebuff = NULL;

    follower = EntTeamInternal(followerName, 0, NULL);
    if( !follower )
        return;

    // checking for lost rescuer
    if (StringEqual(VarGet("Rescuer"), ""))
    { 
        Entity *pOldCreator = erGetEnt(follower->erCreator);

        if (pOldCreator)
            PlayerRemovePet(pOldCreator, follower);

        follower->erCreator = 0;
        follower->erOwner = 0;
        aiDiscardFollowTarget(follower, "Script requests dropping follow.", false);
        SetAIPriorityList( followerName, VarGet( "PL_OnStranded" ) );
        aiBehaviorAddPermFlag(follower, ENTAI(follower)->base, "ReturnToSpawnDistance(10000000),WalkToSpawnRadius(10)");
    }

    if( follower->erCreator )
        leader = erGetEnt( follower->erCreator );

    if( leader )
    {    
        if( !entAlive(leader) ) {
            // leader killed
            leader = 0;

             SET_STATE("Released");
        }
        else if( distance3Squared( ENTPOS(follower), ENTPOS(leader) ) > controlRadius * controlRadius )
        {
            // leader too far away
            if( beaconEntitiesInSameCluster( follower, leader ) )
            {
                leader = 0;

                // set state back to being unowned
                 SET_STATE("Released");
            }
            else
            {
                //TO DO maybe a timer or something to give me time to get through the door?
            }
        }
    }
    
    if( leader ) //No Change to state 
        return;

    if( !leader && !StringEqual("", VarGet("Rescuer")))  //I found a new leader
    {
        int lastStranded = !follower->erCreator;

        //Needs to be above Setting PLs because for example PL action hide_behind_Ent expects to have a leader set
        SetFollowers( VarGet("Rescuer"), followerName );

        RemoveAllBehaviors(followerName); 
        AddAIBehavior(followerName, "Pet.Follow" );

        aiBehaviorAddPermFlag( follower, ENTAI(follower)->base, "BoostSpeed(1.7)" );//    aiBehaviorAddPermFlag( follower, "BoostSpeed(4.3)" );
        aiBehaviorAddPermFlag( follower, ENTAI(follower)->base, "JumpHeight(10.0),ReturnToSpawnDistance(15),WalkToSpawnRadius(10),StandoffDistance(10)");

        // spawn the debuffer
        pDebuff = villainCreateByName("Objects_Escort_Stealth_Debuffer_Warburg", 50, NULL, false, NULL, 0, NULL);
        if (follower && pDebuff != NULL)
        {
            SET_ENTTYPE(pDebuff) = ENTTYPE_CRITTER;
            pDebuff->fade = 1;
            entUpdateMat4Instantaneous(pDebuff, ENTMAT(follower));
            aiInit(pDebuff, NULL);
        }
    } 
}


void SetAIUsePowerNow(TEAM team, ENTITY target, STRING power )
{
    int i, n;
    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);

        if (!e)
            continue;

        if( target )
        {
            Entity* targetEnt = EntTeamInternal(target, 0, NULL);
            if( targetEnt )
                aiSetAttackTarget(e, targetEnt, NULL, NULL, 1, 0);
        }
        aiCritterSetCurrentPowerByName( e, power );
        aiCritterUsePowerNow( e );
    }
}

//How is this function different from the one above?  Well, the one above sets the power as your current 
//power, doing all the stance stuff, this one ignores everything else and just uses the power
//We should clean up and better name both, and figure out in which situations to use either.
void SetAIUsePowerRightNow(TEAM team, ENTITY target, STRING power )
{
    Entity* targetEnt;
    int i, n;
    n = NumEntitiesInTeam(team);
    
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if( e )
        {
            targetEnt = 0;
            if( target )
                targetEnt = EntTeamInternal(target, 0, NULL);
            aiUsePowerNowByName( e, power, targetEnt );
        }
    }
}

void SetAITogglePowerRightNow(TEAM team, STRING power, NUMBER on )
{
    int i, n;
    n = NumEntitiesInTeam(team);

    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if( e )
            aiTogglePowerNowByName( e, power, on );
    }
}

void SetAINeverChooseThisPowerUnlessITellYouTo( TEAM team, STRING power )
{
    int i, n;
    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if( e )
            aiNeverChooseThisPower( e, power );
    }
}

ENTITY GetOwner( ENTITY petName )
{
    Entity * pet;
    ENTITY owner = TEAM_NONE;

    pet = EntTeamInternal(petName, 0, NULL);
    if( pet )
    {
        if( pet->erCreator )
            owner = EntityNameFromEnt( erGetEnt( pet->erCreator ) );
        else if( pet->erOwner )
            owner = EntityNameFromEnt( erGetEnt( pet->erOwner ) );
    }
    return owner;
}

ENTITY GetTrueOwner( ENTITY petName )
{
    Entity * pet;
    ENTITY owner = TEAM_NONE;

    pet = EntTeamInternal(petName, 0, NULL);
    if( pet )
    {
        if( pet->erOwner )
            owner = EntityNameFromEnt( erGetEnt( pet->erOwner ) );
    }
    return owner;
}

//Pick a random selection of minions to say something
void MinionSays( TEAM team, STRING says )
{
    int numEnts;
    ENTITY minion;

    if( team && says )
    {
        numEnts = NumEntitiesInTeam(team);
        minion = GetEntityFromTeam(team, RandomNumber(1, numEnts) );
        if( numEnts )
            Say( minion, says, 0 );
    }
}

void SetShowOnMap(TEAM team, NUMBER flag)
{
    int i, n;

    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if (!e)
            continue;
        if (e->showOnMap != flag)
        {
            e->showOnMap = flag ? 1 : 0;
            e->draw_update = 1;
        }
    }
}


