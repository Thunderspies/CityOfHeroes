#include "pvp.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "entity/powers.h"
#include "dbcomm/dbcomm.h"
#include "gameComm/sendToClient.h"
#include "language/langServerUtil.h"
#include "Reward.h"
#include "entity/character_base.h"
#include "entity/character_target.h"
#include "container/team.h"
#include "TeamReward.h"
#include <utilitieslib/utils/mathutil.h>
#include "entGameActions.h"
#include <utilitieslib/utils/error.h>
#include "dbcomm/logcomm.h"
#include "cmdparse/cmdserver.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/timing.h>
#include "svr/svr_player.h"
#include "gameSys/arenamap.h"
#include "cmdparse/cmdcommon.h"
#include "character_combat.h"
#include "entity/PowerInfo.h"
#include "TeamReward.h"
#include "dbcomm/logcomm.h"

typedef struct Entity Entity;

#define COMBAT_LEVEL_TIME    60

#define REP_DEFEAT_REWARD_BASE    1.0
#define REP_DEFEAT_REWARD_ADD    0.0
#define REP_DEFEAT_REWARD_SUB    0.0

#define REP_BOUNTY_THRESHOLD    20.0
#define REP_BOUNTY_REWARD        5.0

#define DIST_FOR_NO_REP_REWARD    SQR(200)

#define TAB "  "

static StuffBuff s_sbPvPDeathLog = {0};
static TeamDamageTracker **s_ppTeams;

typedef struct pvpEntState {
    float    time;
    float    hp;
    float    end;
} pvpEntState;

typedef struct pvpLogLine {
    int                dbid;
    float            lastHp;
    float            lastEnd;
    pvpEntState        **pEntStateList;
} pvpLogEntity;

typedef struct pvpLogStruct {
    StashTable st_EntsByDBID;
} pvpLogStruct;

pvpLogStruct pvpLog;

static void pvp_LogPlayerDeath_CharacterPowerSets(Character *pchar)
{
    int i;
    int n = eaSize(&pchar->ppPowerSets);
    for(i=2; i<n; i++)
    {
        PowerSet *pset = pchar->ppPowerSets[i];
        if(pset && pset->iLevelBought <= pchar->iCombatLevel)
        {
            if(i>2) addStringToStuffBuff(&s_sbPvPDeathLog, ", ");
            addStringToStuffBuff(&s_sbPvPDeathLog, "%s", pset->psetBase->pchName);
        }
    }
}

static void pvp_LogPlayerDeath_Entity(Entity *e, const Vec3 poi, float contrib, long seconds, bool victor)
{
    float fDist;

    if(ENTTYPE(e)!=ENTTYPE_PLAYER || !e->pchar || !e->pl)
        return;
    
    fDist = distance3(poi, ENTPOS(e)) + 0.5;

    if(contrib>0.0f)
    {
        addStringToStuffBuff(&s_sbPvPDeathLog, "%20.20s %9d  %2d %2d %15.15s %3d | %4d %4d%s %6d | ",
            e->name, e->db_id, e->pchar->iCombatLevel+1, e->pchar->iLevel+1, &e->pchar->pclass->pchName[6], (int)(e->pl->reputationPoints+0.5f),
            (int)fDist, (int)contrib, victor?"*":" ", seconds);
    }
    else
    {
        addStringToStuffBuff(&s_sbPvPDeathLog, "%20.20s %9d  %2d %2d %15.15s %3d | %4d    -       - | ",
            e->name, e->db_id, e->pchar->iCombatLevel+1, e->pchar->iLevel+1, &e->pchar->pclass->pchName[6], (int)(e->pl->reputationPoints+0.5f),
            (int)fDist);
    }

    pvp_LogPlayerDeath_CharacterPowerSets(e->pchar);
    addStringToStuffBuff(&s_sbPvPDeathLog,"\n");
}

static void pvp_LogPlayerDeath_VictimTeam(Entity *e, Vec3 vec)
{
    int* piMembers;
    int iCntMembers;
    int i;

    // Find all team members
    if (e->teamup_id)
    {
        TeamMembers *ptm = teamMembers(e, CONTAINER_TEAMUPS);

        if(!ptm)
            return;

        if(ptm->count > 1)
            addStringToStuffBuff(&s_sbPvPDeathLog, TAB "Victim Team:\n");

        piMembers = ptm->ids;
        iCntMembers = ptm->count;
    }
    else
    {
        return;
    }

    for (i = 0; i < iCntMembers; i++)
    {
        Entity *eMember;
        if(piMembers[i]!=e->db_id && NULL!=(eMember=entFromDbId(piMembers[i])))
        {
            pvp_LogPlayerDeath_Entity(eMember,vec,0.0f,0,false);
        }
    }
}

static float pvp_LogPlayerDeath_TeamAccum(Entity *pVictim)
{
    DamageTracker* dt;
    TeamDamageTracker* ptt = NULL;
    Entity *e;
    float fTotalDamage = 0.0f;
    int i;

    if(!s_ppTeams)
    {
        eaCreate(&s_ppTeams);
    }
    else
    {
        eaClearEx(&s_ppTeams, teamDamageTrackerDestroy);
    }

    // Build the team damage tracker list
    for(i=eaSize(&pVictim->who_damaged_me)-1; i>=0; i--)
    {
        dt = pVictim->who_damaged_me[i];

        // If the player is still on the map, update the teamup id. If
        // they aren't, then we'll used what was cached earlier and hope
        // that it works out.
        if((e=erGetEnt(dt->ref))!=NULL)
        {
            if(e->teamup_id!=dt->group_id)
            {
                dt->group_id = e->teamup_id;
            }
        }

        if(!dt->group_id)
        {
            // If this entity does not belong to any teams, it is on its own
            // team.
            dt->group_id = -erGetDbId(dt->ref);
        }

        ptt = teamDamageTrackerFindTeam(s_ppTeams, dt->group_id);
        if(ptt==NULL)
        {
            ptt = teamDamageTrackerCreate();
            ptt->idGroup = dt->group_id;
            eaPush(&s_ppTeams, ptt);
        }

        // We need at least one person from the team still on the server
        // so we can look at the team list and so on.
        if(ptt->erMember==0 && e!=NULL)
        {
            ptt->erMember=dt->ref;
        }

        fTotalDamage += dt->damage;
        ptt->fDamage += dt->damage;
        ptt->iCntMembers++;
    }

    return fTotalDamage;
}

static void pvp_LogPlayerDeath_OpponentTeam(Entity *pVictim, Entity *pVictor, TeamDamageTracker *ptt, float fTotalDamage)
{
    int i,j;
    int iCntMembers;
    int* piMembers;
    Entity *member = NULL;
    Vec3 vec;
    long time = (long)dbSecondsSince2000();

    member = erGetEnt(ptt->erMember);
    if(!member)
        return;

    copyVec3(ENTPOS(pVictim), vec);

    addStringToStuffBuff(&s_sbPvPDeathLog, TAB "Team (%d) dealt %d:\n", ptt->idGroup, (int)ptt->fDamage);

    // Find all team members
    if (member->teamup_id)
    {
        TeamMembers *ptm = teamMembers(member, CONTAINER_TEAMUPS);

        if(!ptm)
            return;

        piMembers = ptm->ids;
        iCntMembers = ptm->count;

        for(j = 0; j < iCntMembers; j++)
        {
            Entity *eMember = entFromDbId(piMembers[j]);
            if(eMember!=NULL)
            {
                float contrib = 0.0f;
                long dtime = 0;
                for(i=eaSize(&pVictim->who_damaged_me)-1; i>=0; i--)
                {
                    DamageTracker* dt = pVictim->who_damaged_me[i];
                    if(dt->ref == erGetRef(eMember))
                    {
                        contrib = dt->damage;
                        dtime = time-(long)dt->timestamp;
                        break;
                    }
                }
                pvp_LogPlayerDeath_Entity(eMember, vec, contrib, dtime, eMember==pVictor);
            }
        }
    }
    else
    {
        float contrib = 0.0f;
        long dtime = 0;
        for(i=eaSize(&pVictim->who_damaged_me)-1; i>=0; i--)
        {
            DamageTracker* dt = pVictim->who_damaged_me[i];
            if(dt->ref == ptt->erMember)
            {
                contrib = dt->damage;
                dtime = time-(long)dt->timestamp;
                break;
            }
        }
        pvp_LogPlayerDeath_Entity(member, vec, contrib, dtime, member==pVictor);
    }
}

// Does serious pvp death logging
void pvp_LogPlayerDeath(Entity *pVictim, Entity *pVictor)
{
    Vec3 vec;
    U32 time = dbSecondsSince2000();
    bool pvp = ENTTYPE(pVictor)==ENTTYPE_PLAYER;

    // init and clear out reward log
    if(!s_sbPvPDeathLog.buff)
    {
        initStuffBuff(&s_sbPvPDeathLog, 1024);
    }
    clearStuffBuff(&s_sbPvPDeathLog);

    copyVec3(ENTPOS(pVictim), vec);

    addStringToStuffBuff(&s_sbPvPDeathLog, "%s: %s (%d) on %s (%.1f, %.1f, %.1f)\n{\n", pvp?"PvP":"PvE", pVictim->name, pVictim->db_id, db_state.map_name, vec[0], vec[1], vec[2]);

    addStringToStuffBuff(&s_sbPvPDeathLog, "--------------  name      dbid com xp           class rep | dist damg    time | power sets\n");

    if(pvp)
    {
        float fTotalDamage;
        int i;

        fTotalDamage = pvp_LogPlayerDeath_TeamAccum(pVictim);

        pvp_LogPlayerDeath_Entity(pVictim, vec, fTotalDamage, 0, false);
        pvp_LogPlayerDeath_VictimTeam(pVictim, vec);

        for(i=eaSize(&s_ppTeams)-1; i>=0; i--)
        {
            pvp_LogPlayerDeath_OpponentTeam(pVictim,pVictor,s_ppTeams[i],fTotalDamage);
        }
    }
    else
    {
        pvp_LogPlayerDeath_Entity(pVictim, vec, 0.0f, 0, false);
    }

    addStringToStuffBuff(&s_sbPvPDeathLog, "}\n");
    LOG_OLD("%s",s_sbPvPDeathLog.buff);
}


StuffBuff s_sbPvPRewardLog = {0};

// Do the actual rewarding for an individual
static void pvp_RewardVictoryIndividual(Entity *pVictor, Entity *pVictim, float fRepScale)
{
    int iVictorLevel = pVictor->pchar->iCombatLevel;
    int iVictimLevel = pVictim->pchar->iCombatLevel;
    unsigned long curtime = dbSecondsSince2000();
    float fRep = REP_DEFEAT_REWARD_BASE;
    float victorRep = playerGetReputation(pVictor);
    float fBounty = 0.0;

    // Have we recently been in a scuffle?
    bool bHasDefeatedRecently = pvp_IsOnDefeatList(pVictim, pVictor);

    // Reward the bounty
    if (!bHasDefeatedRecently 
        && victorRep >= 0.0
        && victorRep+REP_BOUNTY_THRESHOLD < playerGetReputation(pVictim))
    {
        fBounty = playerAddReputation(pVictor,REP_BOUNTY_REWARD*fRepScale);
        sendInfoBox(pVictor, INFO_REWARD, "ReputationBountyYouReceived", fBounty);
        victorRep = playerGetReputation(pVictor);
        LOG_ENT( pVictor, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[PvP]:Rcv:Rep:Bounty %.2f (%.2f)", fBounty, victorRep);
    }
    
    // Adjust levels for sidekick/exemplar tomfoolerly
    if (pVictor->pchar->buddy.uTimeLastMentored+COMBAT_LEVEL_TIME>curtime)
    {
        iVictorLevel = pVictor->pchar->buddy.iCombatLevelLastMentored;
    }

    if (pVictim->pchar->buddy.uTimeLastMentored+COMBAT_LEVEL_TIME>curtime)
    {
        iVictimLevel = pVictim->pchar->buddy.iCombatLevelLastMentored;
    }

    // Adjust reward for level difference
    if (iVictorLevel > iVictimLevel)
    {
        fRep += (iVictorLevel - iVictimLevel) * REP_DEFEAT_REWARD_SUB;
    }
    else
    {
        fRep += (iVictimLevel - iVictorLevel) * REP_DEFEAT_REWARD_ADD;
    }

    if (fRep >= 0.0) // Giveth
    {
        fRep *= fRepScale; // Only scale down positive rewards

        if (bHasDefeatedRecently) // Unless they're being a jerk
        {
            sendInfoBox(pVictor, INFO_REWARD, "ReputationNoRewardGrief", pVictim->name);
            fRep = 0.0;
        }
        else
        {
            fRep = playerAddReputation(pVictor, fRep);
            victorRep = playerGetReputation(pVictor);
            LOG_ENT( pVictor, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[PvP]:Rcv:Rep %.2f (%.2f)", fRep, victorRep);
            sendInfoBox(pVictor, INFO_REWARD, "ReputationYouReceived", fRep);
        }
    }
    else // Taketh
    {
        fRep = playerAddReputation(pVictor, fRep);
        victorRep = playerGetReputation(pVictor);
        LOG_ENT( pVictor, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[PvP]:Rcv:Rep %.2f (%.2f)", fRep, victorRep);
        sendInfoBox(pVictor, INFO_REWARD, "ReputationYouLost", -fRep);
    }

    // Warn them if they're low
    if (victorRep <= -5.0)
    {
        sendInfoBox(pVictor, INFO_REWARD, "ReputationLowWarning");
    }

    addStringToStuffBuff(&s_sbPvPRewardLog, "%20.20s %9d  %2d %2d | %3.2f %3.2f %3.2f %s\n",
                        pVictor->name, pVictor->db_id, pVictor->pchar->iCombatLevel+1,iVictorLevel+1,
                        fRepScale,fRep,fBounty,(bHasDefeatedRecently)?"*" : " ");
}

void pvp_GrantRewardTable(Entity *pVictor, Entity *pVictim)
{

    if (!pVictim || !pVictor)
        return;
    
    if (!pvp_IsOnDefeatList(pVictim, pVictor))
    {
        rewardGroup(pVictor, pVictim, pVictor->teamup_id, "PlayerWithRep", 1.0f, true, kPowerSystem_Powers, ROTOR_RANK, LOG_NO_GROUP_INFO, CONTAINER_TEAMUPS);
    }

    rewardGroup(pVictor, pVictim, pVictor->teamup_id, "Player", 1.0f, true, kPowerSystem_Powers, ROTOR_RANK, LOG_NO_GROUP_INFO, CONTAINER_TEAMUPS);
}

// Adjust the victor's reputation, and that of his teammates
void pvp_RewardVictory(Entity *pVictor, Entity *pVictim)
{
    float fRepScale;
    int* piMembers;
    int iCntMembers;
    int iCntMembersUseful = 0;
    Entity * apEnts[MAX_TEAM_MEMBERS];
    int i;


    // Find all team members
    if (pVictor->teamup_id)
    {
        TeamMembers *ptm = teamMembers(pVictor, CONTAINER_TEAMUPS);

        if(!ptm)
            return;

        piMembers = ptm->ids;
        iCntMembers = ptm->count;
    }
    else
    {
        piMembers = &pVictor->db_id;
        iCntMembers = 1;
    }

    // Count and note the members on the map and within range
    for (i = 0; i < iCntMembers; i++)
    {
        apEnts[iCntMembersUseful] = entFromDbId(piMembers[i]);
        if (apEnts[iCntMembersUseful] 
            && distance3Squared(ENTPOS(apEnts[iCntMembersUseful]), ENTPOS(pVictim)) < DIST_FOR_NO_REP_REWARD)
        {
            iCntMembersUseful++;
        }
    }

    if(!s_sbPvPRewardLog.buff)
    {
        initStuffBuff(&s_sbPvPRewardLog, 1024);
    }

    addStringToStuffBuff(&s_sbPvPRewardLog, "Player Defeated: %s (%d), level %d\n{\n", pVictim->name, pVictim->db_id, pVictim->pchar->iCombatLevel+1);
    addStringToStuffBuff(&s_sbPvPRewardLog, TAB TAB "Breakdown: ");
    for(i=eaSize(&pVictim->who_damaged_me)-1; i>=0; i--)
    {
        char *playerName;
        DamageTracker *dt = pVictim->who_damaged_me[i];
        Entity *e = erGetEnt(dt->ref);

        if(!e || !e->pchar)
        {
            playerName = "Unknown name";
        }
        else
        {
            playerName = e->name;
        }

        addStringToStuffBuff(&s_sbPvPRewardLog, "'%s'(%i), team %d, %.2f; ", playerName, erGetDbId(dt->ref), dt->group_id, dt->damage);
    }
    addStringToStuffBuff(&s_sbPvPRewardLog, "\n");
    addStringToStuffBuff(&s_sbPvPRewardLog, "%20.20s %9.9s %3.3s %2.2s | %4.4s %4.4s %4.4s %s\n","name", "dbid", "com", "xp", "scl", "rep", "bnty", "timer");

    if (iCntMembersUseful > 0)
    {
        // Scale down reward
        fRepScale = 1.0/(float)iCntMembersUseful;

        for (i = 0; i < iCntMembersUseful; i++)
        {
            if (!OnArenaMap())
            {
                pvp_RewardVictoryIndividual(apEnts[i], pVictim, fRepScale);
            }
            pvp_RecordDefeat(pVictim,apEnts[i]);
        }
    }

    addStringToStuffBuff(&s_sbPvPRewardLog, "\n}\n");
    LOG_ENT( pVictor, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "%s", s_sbPvPRewardLog.buff);

    // clear out reward log
    clearStuffBuff(&s_sbPvPRewardLog);
}


// Note that the victor killed the victim
void pvp_RecordDefeat(Entity *pVictim, Entity *pVictor)
{
    int i;
    int old_idx = 0;
    int id = pVictor->db_id;
    DefeatRecord *defeats = pVictim->pl->pvpDefeatList;
    unsigned long curTime = dbSecondsSince2000();

    for (i = 0; i < MAX_DEFEAT_RECORDS; i++)
    {
        // If we already had a record for this guy, just update the time
        if (defeats[i].victor_id == id)
        {
            if (defeats[i].defeat_time < curTime - DEFEAT_RECORD_TIME)
            {
                defeats[i].defeat_time = curTime;
            }
            return;
        }

        // Figure out if this is an older record
        if (defeats[i].defeat_time < defeats[old_idx].defeat_time)
            old_idx = i;
    }

    defeats[old_idx].victor_id = id;
    defeats[old_idx].defeat_time = curTime;
}

// Check if the victor recently defeated the victim
bool pvp_IsOnDefeatList(Entity *pVictim, Entity *pVictor)
{
    int i;
    int id = pVictor->db_id;
    DefeatRecord *defeats = pVictim->pl->pvpDefeatList;
    unsigned long time_limit = dbSecondsSince2000() - DEFEAT_RECORD_TIME;

    for (i = 0; i < MAX_DEFEAT_RECORDS; i++)
    {
        if (defeats[i].defeat_time)
        {
            // We'll need to make sure it's valid if the id matches, 
            //  no harm in being proactive
            if (defeats[i].defeat_time < time_limit)
            {
                defeats[i].victor_id = 0;
                defeats[i].defeat_time = 0;
            }
            else if (defeats[i].victor_id == id)
            {
                return true;
            }
        }
    }

    return false;
}

// Deletes old records, sorts fresh ones oldest to newest
void pvp_CleanDefeatList(Entity *pVictim)
{
    int i,j;
    DefeatRecord *defeats = pVictim->pl->pvpDefeatList;
    unsigned long time_limit = dbSecondsSince2000() - DEFEAT_RECORD_TIME;

    for (i = 0; i < MAX_DEFEAT_RECORDS; i++)
    {
        // If it's old, toss it
        if (defeats[i].defeat_time < time_limit)
        {
            defeats[i].victor_id = 0;
            defeats[i].defeat_time = 0;
        }
    }

    for (i = 0; i < MAX_DEFEAT_RECORDS; i++) 
    {
        int low_idx = i;
        for (j = i+1; j < MAX_DEFEAT_RECORDS; j++)
        {
            if (defeats[j].defeat_time
                && (defeats[low_idx].defeat_time == 0
                    || defeats[j].defeat_time < defeats[low_idx].defeat_time))
            {
                low_idx = j;
            }
        }

        // We found a older defeat, swap
        if (low_idx != i)
        {
            int old_id = defeats[i].victor_id;
            int old_time = defeats[i].defeat_time;
            defeats[i].victor_id = defeats[low_idx].victor_id;
            defeats[i].defeat_time = defeats[low_idx].defeat_time;
            defeats[low_idx].victor_id = old_id;
            defeats[low_idx].defeat_time = old_time;
        }
    }
}


void pvp_SetSwitch(Entity *e, bool pvp)
{
    assert(e->pl);

    if (e->pl->inviter_dbid) // Not allowed while a team invite is pending
    {
        conPrintf(e->client, localizedPrintf(e,"NoPvPSwitchInvite"));
    }
    else if (e->teamup && e->teamup->members.count > 1) // Not allowed while on a team
    {
        conPrintf(e->client, localizedPrintf(e,"NoPvPSwitchTeam"));
    }
    else if (e->pl->pvpSwitch != pvp) // Switch em if only if we need to
    {
        e->pl->pvpSwitch = pvp;
        e->pvp_update = 1;

        conPrintf(e->client, localizedPrintf(e,pvp ? "YourPvPSwitchIsOn" : "YourPvPSwitchIsOff" ));
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// P V P    T A R G E T I N G    T R A C K I N G
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PVP_TARGETTING_TIMEOUT    5.0f
typedef struct pvpTargetingTrackingData {
    float serverTime;
    StashTable table;
} pvpTargetingTrackingData;

int pvp_TargettingCheckForOld(void* userData, StashElement element)
{
    pvpTargetingTrackingData *pTracker = (pvpTargetingTrackingData *) userData;
    float serverTime = pTracker->serverTime;
    float lastTargeted = (float) stashElementGetInt(element) / 10.0f;
    int dbid = stashElementGetIntKey(element);

    if (serverTime - lastTargeted > PVP_TARGETTING_TIMEOUT)
        stashIntRemoveInt(pTracker->table, dbid, NULL);

    return 1;
}

void pvp_UpdateTargetTracking(float serverTime)
{
    int i; 
    Entity *e;
    pvpTargetingTrackingData tracker;

    if (!server_state.enableSpikeSuppression)
        return;

    // clear old data
    tracker.serverTime = serverTime;
    for(i=1;i<entities_max;i++)
    {
        e = entities[i];
        if( (entity_state[i] & ENTITY_IN_USE) && (ENTTYPE(e) == ENTTYPE_PLAYER) && e && e->pchar && e->pchar->stTargetingMe != NULL)
        {
            tracker.table = e->pchar->stTargetingMe;
            stashForEachElementEx(e->pchar->stTargetingMe, pvp_TargettingCheckForOld, (void *) &tracker);
        }
    }

    // add current targets
    for(i=1;i<entities_max;i++)
    {
        int serverTime10 = (int) (serverTime * 10.0f);

        e = entities[i];
        if( (entity_state[i] & ENTITY_IN_USE) && (ENTTYPE(e) == ENTTYPE_PLAYER) && e && e->pchar)
        {
            Entity *pTarget = erGetEnt(e->pchar->erTargetInstantaneous);

            if (pTarget && (ENTTYPE(pTarget) == ENTTYPE_PLAYER) && pTarget->pchar && character_TargetIsFoe(e->pchar, pTarget->pchar))
            {
                if (!pTarget->pchar->stTargetingMe)
                    pTarget->pchar->stTargetingMe = stashTableCreateInt(8);

                stashIntAddInt(pTarget->pchar->stTargetingMe, e->db_id, serverTime10, true);
            }
        }
    }
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// P V P    P O W E R S
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void pvp_ApplyPvPPowers(Entity *e)
{
    Power *pPower = NULL;

    if ( (server_visible_state.isPvP || OnArenaMap()) && e && e->pchar)
    {

        // heal decay power
        if (server_state.enableHealDecay)
        {
            const BasePower *pBasePower = basepower_FromPath("Temporary_Powers.Temporary_Powers.PVP_Heal_Decay");
            if (pBasePower != NULL)
            {
                pPower = character_OwnsPower(e->pchar, pBasePower);
                if (!pPower)
                {
                    PowerSet *pset = character_OwnsPowerSet(e->pchar, pBasePower->psetParent);

                    if (pset == NULL)
                        pset = character_BuyPowerSet(e->pchar, pBasePower->psetParent);

                    if (pset)
                    {
                        pPower = character_BuyPower(e->pchar, pset, pBasePower, 0);
                    }
                }
                eaPush(&e->pchar->entParent->powerDefChange, powerRef_CreateFromPower(e->pchar->iCurBuild, pPower));
            }
        }


        if (server_state.enablePassiveResists)
        {
            const BasePower *pBasePower = basepower_FromPath("Temporary_Powers.Temporary_Powers.PVP_Resist_Bonus");
            if (pBasePower != NULL)
            {
                pPower = character_OwnsPower(e->pchar, pBasePower);
                if (!pPower)
                {
                    PowerSet *pset = character_OwnsPowerSet(e->pchar, pBasePower->psetParent);

                    if (pset == NULL)
                        pset = character_BuyPowerSet(e->pchar, pBasePower->psetParent);

                    if (pset)
                    {
                        pPower = character_BuyPower(e->pchar, pset, pBasePower, 0);
                    }
                }
                eaPush(&e->pchar->entParent->powerDefChange, powerRef_CreateFromPower(e->pchar->iCurBuild, pPower));
            }
        }

        character_ActivateAllAutoPowers(e->pchar);
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// P V P    L O G G I N G
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void pvp_LogDestructor(void* value)
{
    pvpLogEntity *pLogEnt = (pvpLogEntity *) value;

    if (!pLogEnt)
        return;

    // free state list
    if (pLogEnt->pEntStateList)
        eaDestroy(&pLogEnt->pEntStateList);

    // free struct
    SAFE_FREE(pLogEnt);
}

void pvp_LogStart(Entity *e)
{
    // save requesting ent

    // reset current log
    if (pvpLog.st_EntsByDBID)
    {
        stashTableDestroyEx(pvpLog.st_EntsByDBID, NULL, pvp_LogDestructor);
    }

}

int pvp_LogElementProcessor(StashElement element)
{
    int i, count;
    pvpLogEntity *pEntry = stashElementGetPointer(element);
    Entity *pEnt = entFromDbId(pEntry->dbid);

    if (!pEnt)
        return true;

    count = eaSize(&pEntry->pEntStateList);
    for (i = 0; i < count; i++)
    {
        pvpEntState *pState = pEntry->pEntStateList[i];
        LOG_ENT( pEnt, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "pvpMatchLog State: (%f) hp: %f, end %f", pState->time, pState->hp, pState->end);        
    }

    return true;
}

void pvp_LogStop(Entity *e)
{
    // dump current log
    stashForEachElement(pvpLog.st_EntsByDBID, pvp_LogElementProcessor);

    // reset current log
    if (pvpLog.st_EntsByDBID)
    {
        stashTableDestroyEx(pvpLog.st_EntsByDBID, NULL, pvp_LogDestructor);
        pvpLog.st_EntsByDBID = 0;
    }
}


void pvp_LogTick(void)
{
    F32 time = timerSeconds64 (timerCpuTicks64());
    static F32 lastUpdate = -1;
    int i;

    if (!server_state.enablePvPLogging)
        return;

    if (time - lastUpdate < 0.5f)
        return;

    lastUpdate = time;

    if (!pvpLog.st_EntsByDBID)
        pvpLog.st_EntsByDBID = stashTableCreateInt(16);

    // record all player entity health/end
    for (i = 0; i < player_ents[PLAYER_ENTS_ACTIVE].count; i++)
    {
        Entity *pPlayer = player_ents[PLAYER_ENTS_ACTIVE].ents[i];
        pvpLogEntity *pEntry = NULL;
        pvpEntState *pState = NULL;

        if (!pPlayer)
            continue;

        stashIntFindPointer(pvpLog.st_EntsByDBID, pPlayer->db_id, &pEntry);
        
        if (!pEntry)
        {
            pEntry = (pvpLogEntity *) malloc(sizeof(pvpLogEntity));
            memset(pEntry, 0, sizeof(pvpLogEntity));
            pEntry->dbid = pPlayer->db_id;
            stashIntAddPointer(pvpLog.st_EntsByDBID, pPlayer->db_id, pEntry, false);
        }

        if ((pEntry->lastEnd < pPlayer->pchar->attrCur.fEndurance * 0.9f || pEntry->lastEnd > pPlayer->pchar->attrCur.fEndurance * 1.1f) || 
            pEntry->lastHp != pPlayer->pchar->attrCur.fHitPoints)
        {
            pState = (pvpEntState *) malloc(sizeof(pvpEntState));
            if (!pState)
                continue;

            memset(pState, 0, sizeof(pvpEntState));

            pState->time = time;
            pEntry->lastHp = pState->hp = pPlayer->pchar->attrCur.fHitPoints;
            pEntry->lastEnd = pState->end = pPlayer->pchar->attrCur.fEndurance;

            eaPush(&pEntry->pEntStateList, pState);
        }
    }
}
