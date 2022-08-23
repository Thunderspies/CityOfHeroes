/*\
 *
 *    scripthookReward.c - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides all the functions CoH script files should be referencing -
 *        essentially a list of hooks into game systems.
 *
 *    This file contains all functions related to rewards
 *
 */

#include "script/script.h"
#include "script/scriptengine.h"
#include "script/scriptutil.h"

#include "entity/entity.h"
#include <utilitieslib/components/MemoryPool.h>
#include "generator/encounterPrivate.h"
#include "storyarc/storyarcprivate.h"

#include "ai/entai.h"
#include "ai/entaiScript.h"
#include "ai/entaivars.h"
#include "ai/entaiprivate.h"
#include "entity/entPlayer.h"
#include "entity/teamCommon.h"
#include "entity/entGameActions.h"
#include "svr/svr_player.h"
#include "entity/buddy_server.h"
#include "entity/character_tick.h"
#include "entity/character_target.h"
#include "entity/character_level.h"
#include "entity/character_base.h"
#include "entity/character_inventory.h"
#include "Reward.h"
#include "TeamReward.h"
#include "container/mapgroup.h"
#include "cmdparse/cmdserver.h"
#include "entity/SgrpServer.h"
#include "auth/authUserData.h"
#include "player/badges_server.h"
#include "entity/incarnate_server.h"

#include "script/Scripthook/ScriptHookInternal.h"

//////////////////////////////////////////////////////////////////////////////////
// T O K E N S 
//////////////////////////////////////////////////////////////////////////////////

int EntityHasRewardToken(ENTITY ent, STRING token)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);

    if (e)
    {
        if (!StringEmpty(token))
        {
            if (getRewardToken(e, token) != NULL) 
            {
                return true;
            } else {
                return false;
            }
        }
    }
    return false;
}

int EntityGetRewardToken(ENTITY ent, STRING token)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);
    RewardToken *pToken = NULL;

    if (e)
    {
        if (!StringEmpty(token))
        {
            if ((pToken = getRewardToken(e, token)) != NULL) 
            {
                return pToken->val;
            } else {
                return -1;
            }
        }
    }

    return false;
}


int EntityGetRewardTokenTime(ENTITY ent, STRING token)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);
    RewardToken *pToken = NULL;

    if (e)
    {
        if (!StringEmpty(token))
        {
            if ((pToken = getRewardToken(e, token)) != NULL) 
            {
                return pToken->timer;
            } else {
                return -1;
            }
        }
    }

    return false;
}


void EntityGrantRewardToken(ENTITY ent, STRING token, NUMBER value)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);

    if (e)
    {
        if (!StringEmpty(token))
        {    
            setRewardToken(e, token, value);
        }
    }
}

void EntityRemoveRewardToken(ENTITY ent, STRING token)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);

    if (e)
    {
        if (!StringEmpty(token))
        {
            removeRewardToken(e, token);
        }
    }
}

void EntityAddRaidPoints(ENTITY ent, NUMBER points, NUMBER isRaid)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);

    if (e && e->supergroup)
    {
        entity_AddSupergroupRaidPoints(e, points, isRaid);
    }
}


//////////////////////////////////////////////////////////////////////////////////
// S T A N D A R D    R E W A R D S 
//////////////////////////////////////////////////////////////////////////////////

void EntityGrantReward(ENTITY ent, STRING rewardTable)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);

    if (!StringEmpty(rewardTable)) { 
        if (e)
        {
            // give it to the entity, if we got one
            rewardFindDefAndApplyToEnt(e, (const char**)eaFromPointerUnsafe(rewardTable), VG_NONE, character_GetCombatLevelExtern(e->pchar), false, REWARDSOURCE_SCRIPT, NULL);
        } 
        else if (strnicmp(ent, "player:", strlen("player:"))==0)
        {
            // see if we can grant it through the dbid
            const char* id = ent += strlen("player:");
            rewardFindDefAndApplyToDbID(atoi(id), (const char**)eaFromPointerUnsafe(rewardTable), VG_NONE, REWARDLEVELPARAM_USEPLAYERLEVEL, 0, REWARDSOURCE_SCRIPT);
        }
    }
}

void TeamGrantReward(TEAM team, STRING rewardTable)
{
    NUMBER i, numPlayers;
    ENTITY target;

    numPlayers = NumEntitiesInTeam(team);
    for(i = 1; i <= numPlayers; i++)
    {
        target = GetEntityFromTeam(team, i);
        EntityGrantReward(target, rewardTable);
    }
}
  
void MissionGrantReward(STRING rewardTable)
{
    if (!StringEmpty(rewardTable) && OnMissionMap()) 
    {
        // check for bonus time
        if (strnicmp(rewardTable, "bonustime ", strlen("BonusTime "))==0)
        {
            const char* time = rewardTable += strlen("bonustime ");
            int timeValue = atoi(time);

            if (timeValue != 0)
                StoryRewardBonusTime(timeValue);
        } else {
            MissionRewardActivePlayersSpecific(rewardTable);
        }
    }
}


// Didn't bother to make it fancy till we think of better heuristic
void GiveRewardToAllInRadius(STRING reward, LOCATION location, FRACTION radius )
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int i;
    Vec3 pos;

    if( !reward || !reward[0] || !radius )
        return;

    if( !GetPointFromLocation( location, pos ) ) //don't do radius check if you can't figure out where from
        return;

    for (i = 0; i < players->count; i++)
    {
        Entity * e = players->ents[i];
        // don't count admins spying as players on the map
        if (ENTINFO(e).hide) continue;
        if (!e->ready) continue;
        if( distance3Squared( ENTPOS(e), pos ) < radius * radius ) 
            rewardFindDefAndApplyToEnt(e, (const char**)eaFromPointerUnsafe(reward), VG_NONE, 1, false, REWARDSOURCE_SCRIPT, NULL);
    }
}

//////////////////////////////////////////////////////////////////////////////////
// S A L V A G E
//////////////////////////////////////////////////////////////////////////////////

NUMBER EntityCountSalvage(ENTITY ent, STRING salvage)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);
    SalvageInventoryItem *sii = NULL;

    if(e && e->pchar ) // Sanity checks
    {
        sii = character_FindSalvageInv(e->pchar, salvage);
        if( sii )
        {
            return sii->amount;
        }
    }

    return 0;
}

// Adjusts the number of salvage the given 
void EntityGrantSalvage(ENTITY ent, STRING salvage, NUMBER amount)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);
    const SalvageItem *si = NULL;

    if(e && e->pchar ) // Sanity checks
    {
        si = salvage_GetItem( salvage );
        if( si )
        {
            if(character_CanAdjustSalvage(e->pchar, si, amount) )
                character_AdjustSalvage( e->pchar, si, amount, "script", false);
        }
    }
}


//////////////////////////////////////////////////////////////////////////////////
// P O W E R S
//////////////////////////////////////////////////////////////////////////////////

void EntityGrantTempPower(ENTITY ent, STRING powerset, STRING power)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);
    if(e && e->pchar) // Sanity checks
    {
        const BasePower *ppowBase = powerdict_GetBasePowerByName(&g_PowerDictionary, "Temporary_Powers", powerset, power);
        if(ppowBase!=NULL)
        {
            character_AddRewardPower(e->pchar, ppowBase);
        }
    }
}


void EntityRemoveTempPower(ENTITY ent, STRING powerset, STRING power)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);
    if(e && e->pchar) // Sanity checks
    {
        const BasePower *ppowBase = powerdict_GetBasePowerByName(&g_PowerDictionary, "Temporary_Powers", powerset, power);
        if(ppowBase!=NULL)
        {
            character_RemoveBasePower(e->pchar, ppowBase);
        }
    }
}

void EntityGrantPower(ENTITY ent, STRING power)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);
    if(e && e->pchar) // Sanity checks
    {
        const BasePower *ppowBase = powerdict_GetBasePowerByFullName(&g_PowerDictionary, power);
        if(ppowBase!=NULL)
        {
            character_AddRewardPower(e->pchar, ppowBase);
        }
    }
}

void EntityRemovePower(ENTITY ent, STRING power)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);
    if(e && e->pchar) // Sanity checks
    {
        const BasePower *ppowBase = powerdict_GetBasePowerByFullName(&g_PowerDictionary, power);
        if(ppowBase!=NULL)
        {
            character_RemoveBasePower(e->pchar, ppowBase);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////
// X P  &   I N F L U E N C E
//////////////////////////////////////////////////////////////////////////////////

NUMBER EntityGetXP(ENTITY ent)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);

    if(e && e->pchar) // Sanity checks
    {
        return e->pchar->iExperiencePoints;
    }
    return 0;
}

void EntitySetXP(ENTITY ent, NUMBER xp)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);

    if(e && e->pchar) // Sanity checks
    {
        if (e->pchar->iExperiencePoints < xp) 
        {
            e->pchar->iExperiencePoints = 
                    MIN(xp ,g_ExperienceTables.aTables.piRequired[MAX_PLAYER_SECURITY_LEVEL-1]);
            e->pchar->iCombatLevel = character_CalcCombatLevel(e->pchar);
            character_HandleCombatLevelChange(e->pchar);
        }
    }
}

void EntitySetXPByLevel(ENTITY ent, NUMBER level)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);
    int xp = 0;
    
    if (level < 0 || level >= MAX_PLAYER_SECURITY_LEVEL)
        return;

    xp = g_ExperienceTables.aTables.piRequired[level-1];

    if(e && e->pchar) // Sanity checks
    {
        if (e->pchar->iExperiencePoints < xp) 
        {
            e->pchar->iExperiencePoints = xp;
            e->pchar->iCombatLevel = character_CalcCombatLevel(e->pchar);
            character_HandleCombatLevelChange(e->pchar);
        }
    }
}

NUMBER EntityGetInfluence(ENTITY ent)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);

    if(e && e->pchar) // Sanity checks
    {
        return(e->pchar->iInfluencePoints);
    } else {
        return 0;
    }
}

void EntityAddInfluence(ENTITY ent, NUMBER inf)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);

    ent_AdjInfluence(e, inf, NULL);
}

int EntityHasBadge(ENTITY ent, STRING badgeName)
{
    Entity *e = EntTeamInternal(ent, 0, NULL);
    return badge_OwnsBadge(e, badgeName);
}

void EntityGrantBadge(ENTITY ent, STRING badgeName, bool notify)
{
    Entity *e = EntTeamInternal(ent, 0, NULL);
    badge_Award(e, badgeName, notify);
}

void TeamGrantBadge(TEAM team, STRING badgeName, bool notify)
{
    NUMBER i, numPlayers;
    ENTITY target;

    numPlayers = NumEntitiesInTeam(team);
    for(i = 1; i <= numPlayers; i++)
    {
        target = GetEntityFromTeam(team, i);
        EntityGrantBadge(target, badgeName, notify);
    }
}

//Possibly faster version
/*void GiveRewardToAllInRadius( STRING reward , LOCATION location, FRACTION radius )
{
    Vec3 pos;
    int apEntities[MAX_ENTITIES_PRIVATE];
    int iCntFound;

    if( !reward || !reward[0] || !radius )
        return;

    if( GetPointFromLocation( location, pos ) )
    {
        iCntFound = mgGetNodesInRange(0, pos, (void **)apEntities, MAX_ENTITIES_PRIVATE);

        for(iCntFound-=1; iCntFound>=0; iCntFound--)
        {
            Entity *e = entities[apEntities[iCntFound]];

            if( e && e->pchar !=NULL && (ENTTYPE(e)==ENTTYPE_PLAYER ) )
            {
                if( distance3Squared( e->mat[3], pos ) < (radius * radius) )
                {
                    rewardFindDefAndApplyToEnt(e, (const char**)eaFromPointerUnsafe(reward), VG_NONE, 1, false);
                }
            }
        }
    }
}*/


// *********************************************************************************
//  Map Data Tokens
// *********************************************************************************

int MapHasDataToken(STRING token)
{
    MapDataToken *pTok;
    
    if (!mapGroupIsInitalized())
        mapGroupInit("AllStaticMaps");

    pTok = getMapDataToken(token);

    if (pTok != NULL)
        return true;
    else
        return false;
}

int MapGetDataToken(STRING token)
{
    MapDataToken *pTok;

    if (!mapGroupIsInitalized())
        mapGroupInit("AllStaticMaps");

    pTok = getMapDataToken(token);

    if (pTok != NULL)
        return pTok->val;
    else
        return -1;
}

void MapSetDataToken(STRING token, NUMBER value)
{
    MapDataToken *pTok;

    if (!mapGroupIsInitalized())
        mapGroupInit("AllStaticMaps");

    mapGroupLock();
    pTok = giveMapDataToken(token);

    if (pTok)
        pTok->val = value;

    mapGroupUpdateUnlock();

}

void MapRemoveDataToken(STRING token)
{
    if (!mapGroupIsInitalized())
        mapGroupInit("AllStaticMaps");

    mapGroupLock();
    removeMapDataToken(token);
    mapGroupUpdateUnlock();
}


// *********************************************************************************
//  Invention Enabled Token
// *********************************************************************************

NUMBER IsInventionEnabled()
{
    return true;
}