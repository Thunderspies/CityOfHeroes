/*\
 *
 *    scripthookwaypoint.c - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides all the functions CoH script files should be referencing -
 *        essentially a list of hooks into game systems.
 *
 *    This file contains all functions related to script entities and teams
 *
 */

#include "script/script.h"
#include "script/scriptengine.h"
#include "script/scriptutil.h"

#include "entity/entity.h"
#include <utilitieslib/components/MemoryPool.h>
#include "generator/encounterPrivate.h"
#include "storyarc/storyarcprivate.h"

#include "ailib/aiBehaviorPublic.h"
#include "ai/entai.h"
#include "ai/entaiScript.h"
#include "ai/entaivars.h"
#include "ai/entaiprivate.h"
#include "entity/entPlayer.h"
#include "entity/teamCommon.h"
#include "entity/entGameActions.h"
#include "svr/svr_player.h"
#include "dbcomm/dbmapxfer.h"
#include "entity/buddy_server.h"
#include "entity/character_tick.h"
#include "entity/character_target.h"
#include "entity/character_level.h"
#include "entity/character_base.h"
#include "entity/character_eval.h"
#include "entity/character_combat.h"
#include "Reward.h"
#include "dbghelper.h"
#include "door.h"
#include "gridcoll/gridcoll.h"
#include "entity/motion.h"
#include "entity/pvp.h"
#include "cmdparse/cmdcontrols.h"
#include "cmdparse/cmdserver.h"
#include "entity/origins.h"
#include "language/commonLangUtil.h"

#include "script/Scripthook/ScriptHookInternal.h"
#include "script/ZoneEvents/ScriptedZoneEventKarma.h"



// *********************************************************************************
//  Script Teams
// *********************************************************************************

typedef struct ScriptTeam
{
    char*        name;
    EntityRef** members;
} ScriptTeam;

MP_DEFINE(ScriptTeam);
MP_DEFINE(EntityRef);


int ScriptTeamValid(const char* name)
{
    if (strchr(name, ':')) return 0;
    if (strchr(name, '_')) return 0;
    if (strchr(name, ',')) return 0;
    return 1;
}

void examineScript(void)
{
    int i;

    ScriptTeam* steam;

    if(1)return;
    steam = g_currentScript->teams[2];
    for (i = eaSize(&steam->members)-1; i >= 0; i--)
    {
        Entity * e = erGetEnt(*steam->members[i]);
        printf("");
        if( e->owner == 0x18 )
            printf("x");
        e->seq;
    }
}

static ScriptTeam* ScriptTeamCreate(const char* name)
{
    ScriptTeam* steam;
    MP_CREATE(ScriptTeam, 10);
    MP_CREATE(EntityRef, 10);
    steam = MP_ALLOC(ScriptTeam);
    steam->name = strdup(name);
    if (!g_currentScript->teams)
        eaCreate(&g_currentScript->teams);
    eaPush(&g_currentScript->teams, steam);
    return steam;
}

static ScriptTeam* ScriptTeamFind(const char* name)
{
    int i;

    if (g_currentScript && g_currentScript->teams) 
    {
        for (i = eaSize(&g_currentScript->teams)-1; i >= 0; i--)
        {
            if (stricmp(name, g_currentScript->teams[i]->name)==0)
                return g_currentScript->teams[i];
        }
    }
    return NULL;
}

void ScriptTeamDestroy(ScriptTeam* steam)
{
    int i;

    if (!steam)
        return;

    for (i = eaSize(&steam->members)-1; i >= 0; i--)
    {
        MP_FREE(EntityRef, steam->members[i]);
    }
    eaDestroy(&steam->members);
    free(steam->name);
    MP_FREE(ScriptTeam, steam);
}

void ScriptTeamDestroyAll(ScriptEnvironment* script)
{
    int i;

    if (!script && script->teams)
        return;

    for (i = eaSize(&script->teams)-1; i >= 0; i--)
    {
        ScriptTeamDestroy(script->teams[i]);
    }
    eaDestroy(&script->teams);
    script->teams = NULL;
}

void ScriptTeamAddEnt(Entity* e, TEAM team)
{
    ScriptTeam* steam;
    EntityRef* ref;

    if (!team || !team[0])
        return;

    if (!ScriptTeamValid(team))
    {
        ScriptError("ScriptTeam given invalid team name: %s", team);
        return;
    }

    steam = ScriptTeamFind(team);
    if (!steam)
        steam = ScriptTeamCreate(team);
    ref = MP_ALLOC(EntityRef);
    *ref = erGetRef(e);
    eaPush(&steam->members, ref);
}

void ScriptTeamRemoveEnt(Entity* e, TEAM team)
{
    int i;
    ScriptTeam* steam;
    EntityRef ref = erGetRef(e);

    if (!e || !ref || !team || !team[0])
        return;

    if (!ScriptTeamValid(team))
    {
        ScriptError("ScriptTeam given invalid team name: %s", team);
        return;
    }
    steam = ScriptTeamFind(team);
    if (steam)
    {
        for (i = eaSize(&steam->members)-1; i >= 0; i--)
        {
            if (ref == *steam->members[i])
            {
                MP_FREE(EntityRef, steam->members[i]);
                eaRemove(&steam->members, i);
            }
        }
    }
}

static void ScriptTeamPrune(ScriptTeam* steam)
{
    int i;
    for (i = eaSize(&steam->members)-1; i >= 0; i--)
    {
        if (!erGetEnt(*steam->members[i]))
        {
            MP_FREE(EntityRef, steam->members[i]);
            eaRemove(&steam->members, i);
        }
    }
}

// *********************************************************************************
//  Team References
// *********************************************************************************

PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];

// get specific entity by index (0-based), or get count
    // Xxx                    - team name
    // aiteam:idx            - id number of a group created by an encounter
    // ent:refstring        - entref string of a single entity
    // ent:none                - no one in team
    // player:db_id            - reference to an online or offline player
    // playerTeam:db_id        - reference to an online player's current Teamup
    // each:player,critter    - all players or critters on this server
    // encounterid:idnum    - all ents attached to this encounter
    // pnpc:name            - logical name of a persistent npc
Entity* EntTeamInternal(TEAM team, int index, int* num)
{
    return EntTeamInternalEx(team, index, num, 0, 0);
}

Entity* EntTeamInternalEx(TEAM team, int index, int* num, int onlytargetable, int countDead)
{
    Entity* e;
    int i, count = 0;

    if(team == 0 )
    {
        //none
    }
    else if (stricmp(team, TEAM_NONE)==0)
    {
        // none
    }
    else if (stricmp(team, ALL_PLAYERS)==0)
    {
        for (i = 0; i < players->count; i++)
        {
            // don't count admins spying as players on the map
            if (ENTINFO(players->ents[i]).hide) 
                continue;
            
            // don't count players who are not ready
            if (!players->ents[i]->ready) 
                continue;
            if (!(entity_state[players->ents[i]->owner] & ENTITY_IN_USE))
                continue;

            // untargetable players might be excluded
            if (onlytargetable && players->ents[i]->untargetable)
                continue;

            if (index == count++) 
                return players->ents[i];
        }
    }
    else if (stricmp(team, ALL_PLAYERS_READYORNOT)==0)
    {
        for (i = 0; i < players->count; i++)
        {
            // don't count admins spying as players on the map
            if (ENTINFO(players->ents[i]).hide) 
                continue;
            
            if (!(entity_state[players->ents[i]->owner] & ENTITY_IN_USE))
                continue;
            
            // untargetable players might be excluded
            if (onlytargetable && players->ents[i]->untargetable)
                continue;

            if (index == count++) 
                return players->ents[i];
        }
    }
    else if (stricmp(team, ALL_CRITTERS)==0)
    {
        for(i=1;i<entities_max;i++)
        {
            e = entities[i];
            if ((entity_state[i] & ENTITY_IN_USE) &&
                e && ENTTYPE(e) == ENTTYPE_CRITTER &&
                (!e->pchar || e->pchar->attrCur.fHitPoints > 0 || countDead) &&     // I see dead people (not)
                (!onlytargetable || !e->untargetable))                // exclude untargetable critters if necessary
            {
                if (index == count++) return e;
            }
        }
    }
    else if (stricmp(team, ALL_DOORS)==0)
    {
        for(i=1;i<entities_max;i++)
        {
            e = entities[i];
            if ((entity_state[i] & ENTITY_IN_USE) &&
                e && ENTTYPE(e) == ENTTYPE_DOOR)
            {
                if (index == count++) return e;
            }
        }
    }
    else if (strnicmp(team, "aiteam:", strlen("aiteam:"))==0)
    {
        const char* id = team += strlen("aiteam:");
        AITeam* aiteam = aiTeamLookup(atoi(id));
        if (aiteam)
        {
            AITeamMemberInfo* member = aiteam->members.list;
            while (member)
            {
                if (member->entity->pchar && (member->entity->pchar->attrCur.fHitPoints > 0 || countDead) && (!onlytargetable || !member->entity->untargetable))
                {
                    if (index == count++) return member->entity;
                }
                member = member->next;
            }
        }
    }
    else if (strnicmp(team, "ent:", strlen("ent:"))==0)
    {
        const char* id = team += strlen("ent:");
        e = erGetEntFromString(id);
        if (e)
        {
            if (index == count++) return e;
        }
    }
    else if (strnicmp(team, "player:", strlen("player:"))==0)
    {
        const char* id = team += strlen("player:");
        e = entFromDbId(atoi(id));
        if (e)
        {
            if (index == count++) return e;
        }
    }
    else if (strnicmp(team, "playerTeam:", strlen("playerTeam:"))==0)
    {
        const char* id = team += strlen("playerTeam:");
        e = entFromDbId(atoi(id));
        if (e)
        {
            if (!e->teamup_id)
            {
                if (index == count++) return e;
            } else {
                // cycle through team members
                for (i = 0; i < e->teamup->members.count; i++)
                {
                    Entity* ent = entFromDbId(e->teamup->members.ids[i]);

                    if (onlytargetable && ent->untargetable)
                        continue;

                    if (index == count++) return ent;
                }
            }
        }
    }
    else if (strnicmp(team, "encounterid:", strlen("encounterid:"))==0)
    {
        EncounterGroup* group = EncounterGroupFromHandle(atoi(team + strlen("encounterid:")));
        if (!group)
        {
            // this should be error, because it should be impossible for scripts to
            // get a hold of an invalid handle
            ScriptError("EntTeamInternal passed an invalid encounter handle %s", team);
            if (num) *num = 0;
            return NULL;
        }
        if (group->active)
        {
            for (i = 0; i < eaSize(&group->active->ents); i++)
            {
                Entity* e = group->active->ents[i];
                if (e && (!e->pchar || e->pchar->attrCur.fHitPoints > 0 || countDead) &&
                    (!onlytargetable || !e->untargetable))
                {
                    if (index == count++) return e;
                }
            }
        }
    }
    else if (strnicmp(team, "pnpc:", strlen("pnpc:"))==0)
    {
        Entity* e = PNPCFindEntity(team + strlen("pnpc:"));
        if (e)
        {
            if (index == count++) return e;
        }
    }
    else if (strnicmp(team, "objective:", strlen("objective:"))==0)
    {
        // Process all mission objectives of the specified name.
        int foundObj = 0;
        int objCursor;
        const char * objective = team + strlen("objective:");

        for( objCursor = eaSize(&objectiveInfoList)-1; objCursor >= 0; objCursor-- )
        {
            MissionObjectiveInfo* obj = objectiveInfoList[objCursor];
            if(stricmp(obj->name, objective) != 0)
                continue;

            foundObj = 1;

            if( obj->objectiveEntity )
            {
                Entity * objEnt = erGetEnt( obj->objectiveEntity );
                if( objEnt )
                    return objEnt;
            }
        }

        if( foundObj )
            ScriptError("BAD LOCATION: The objective %s exists, but doesn't have an encounter or objective associated with it, so has no location", team);
        else
            ScriptError("BAD LOCATION: The objective %s is not an objective on this mission.", team);

        return 0;
    }
    else
    {
        ScriptTeam* steam = ScriptTeamFind(team);
        if (steam)
        {
            if (num) ScriptTeamPrune(steam); // funny little hack
            for (i = 0; i < eaSize(&steam->members); i++)
            {
                Entity* e = erGetEnt(*steam->members[i]);
                if (e && (!e->pchar || e->pchar->attrCur.fHitPoints > 0 || countDead) && (!onlytargetable || !e->untargetable))
                {
                    if (index == count++) return e;
                }
            }
        }
    }
    if (num) *num = count;
    return NULL;
}

// get an AITeam id by index (0-based), or count of AITeams
static int AITeamInternal(TEAM team, int index, int* num)
{
    static int* teamids = 0;
    int i, n;

    // compile list of ai team ids
    if (!teamids)
        eaiCreate(&teamids);
    eaiSetSize(&teamids, 0);
    EntTeamInternal(team, -1, &n);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if (e && ENTAI(e) && (!e->pchar || e->pchar->attrCur.fHitPoints > 0))
        {
            AIVars* ai = ENTAI(e);
            if (ai && ai->teamMemberInfo.team && ai->teamMemberInfo.team->teamid)
            {
                int id = ai->teamMemberInfo.team->teamid;
                if (eaiFind(&teamids, id) < 0)
                    eaiPush(&teamids, id);
            }
        }
    }

    // give result
    n = eaiSize(&teamids);
    if (num)
        *num = n;
    if (index >= 0 && index < n)
        return teamids[index];
    return 0;
}

// local allocs string
ENTITY EntityNameFromEnt(Entity* e)
{
    if (e && ENTINFO(e).type == ENTTYPE_PLAYER)
    {
        return StringAdd("player:", NumberToString(e->db_id));
    }
    else if (e && ENTINFO(e).type == ENTTYPE_CRITTER)
    {
        return StringAdd("ent:", erGetRefString(e));
    }
    else if (e && (ENTINFO(e).type == ENTTYPE_NPC || ENTINFO(e).type == ENTTYPE_MISSION_ITEM))
    {
        return StringAdd("ent:", erGetRefString(e));
    }
    return TEAM_NONE;
}

int NumEntitiesInTeam(TEAM team)
{
    int count = 0;
    EntTeamInternalEx(team, -1, &count, 0, 0);
    return count;
}

int NumEntitiesInTeamEvenDead(TEAM team)
{
    int count = 0;
    EntTeamInternalEx(team, -1, &count, 0, 1);
    return count;
}

int NumTargetableEntitiesInTeam(TEAM team)
{
    int count = 0;
    EntTeamInternalEx(team, -1, &count, 1, 0);
    return count;
}

// count excluding one specified entity (the hostage) and anything of rank Pet which is also untargetable (like Caltrops)
int NumRescueValidEntitiesInTeam(TEAM team, ENTITY exclude)
{
    int count = 0;
    int i;
    Entity *rescuee = EntTeamInternal(exclude, 0, NULL);
    EntTeamInternalEx(team, -1, &count, 0, 0);
    for(i = count - 1; i >= 0; i--)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if( e == rescuee || (e->villainRank == VR_PET && e->untargetable) )
            count--;
    }

    return count;
}

ENTITY GetEntityFromTeam(TEAM team, int number)
{
    Entity* e = EntTeamInternal(team, number-1, NULL);
    return EntityNameFromEnt(e);
}

// only valid for playerTeam: type teams
ENTITY GetPlayerFromPlayerTeam(TEAM team, int index)
{
    Entity *e;
    int i, count = 1;

    if (strnicmp(team, "playerTeam:", strlen("playerTeam:"))==0)
    {
        const char* id = team += strlen("playerTeam:");
        e = entFromDbId(atoi(id));
        if (e)
        {
            if (!e->teamup_id)
            {
                return TEAM_NONE;
            } else {
                // cycle through team members
                for (i = 0; i < e->teamup->members.count; i++)
                {
                    if (index == count++) 
                    {
                        return StringAdd("player:", NumberToString(e->teamup->members.ids[i]));
                    }
                }
                return TEAM_NONE;
            }
        }
    } 
    return TEAM_NONE;
}

ENTITY GetPlayerTeamFromPlayer(ENTITY player)
{
    if (strnicmp(player, "player:", strlen("player:"))==0)
    {
        const char* id = player += strlen("player:");
        Entity *e = entFromDbId(atoi(id));
        if (e)
        {
            return StringAdd("playerTeam:", NumberToString(e->db_id));
        }
    }

    // unable to find player specified
    return TEAM_NONE;
}

static void damageTrackerDestroyAdapter(void* arg0)
{
    damageTrackerDestroy((DamageTracker*)arg0);
}

void Kill(TEAM team, int giveKillCredit )
{
    int i, numents = 0;
    EntTeamInternal(team, -1, &numents);
    for (i = numents-1; i >= 0; i--)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if (e && e->pchar && !ENTSLEEPING(e))
        {
            // Deliver fatal blow and let dying animation play etc.
            e->pchar->attrCur.fHitPoints = -999999;
            if( !giveKillCredit )
                eaClearEx(&e->who_damaged_me, damageTrackerDestroyAdapter);
        }
        else
        {
            // Not a character or is a sleeping character. Just
            // free the entity immediately
            entFree(e);
        }
    }
}

/* NOT SAFE
void Destroy(TEAM team)
{
    int i, numents = 0;
    EntTeamInternal(team, -1, &numents);
    for (i = numents-1; i >= 0; i--)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if (e && (ENTTYPE(e) != ENTTYPE_PLAYER))
        {
            entFree(e);
        }
    }
}
*/

// priorityLevel 0 == if you are saying anything else, don't say this
// priorityLevel 1 == Reserved for later
// priorityLevel 2 == say this no matther what
// priorityLevel 3 == say to e->audience only
void Say(ENTITY entity, STRING text, int priorityLevel )
{
    Entity* e;
    Entity * audience;
    int missionOwnerIfNoAudience = 0;
    ScriptVarsTable vars = {0};
    
    if( !text || text == "" )
        return;

    e = EntTeamInternal(entity, 0, NULL);
    if (e)
    {
        if( priorityLevel == 0 && e->IAmInEmoteChatMode )
            return;

        audience = erGetEnt( e->audience );
        if( audience && ENTTYPE(audience) != ENTTYPE_PLAYER ) //Only players 
            audience = 0;
        if( (priorityLevel < 3 || !audience) && OnMissionMap() )
            missionOwnerIfNoAudience = g_activemission->ownerDbid;
        if( !audience )
        {
            AIVars* ai=ENTAI_BY_ID(e->owner);
            if( ai ) //Then look for your attack target, a nice generic thing 
                audience = ai->attackTarget.entity; //(Martin says this is always valid or null
        }

        if(g_currentScript)
            ScriptVarsTableScopeCopy(&vars, &g_currentScript->initialvars);
        else if(EncounterVarsFromEntity(e))
            ScriptVarsTableScopeCopy(&vars, EncounterVarsFromEntity(e));
        else if(OnMissionMap())
            ScriptVarsTableScopeCopy(&vars, &g_activemission->vars);

        if (priorityLevel == 3 && erGetEnt( e->audience )) 
        {
            Entity *hearingEnt = erGetEnt( e->audience );
            saBuildScriptVarsTable(&vars, hearingEnt, hearingEnt->db_id, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, false);
            saUtilEntSays(e, kEntSay_NPCDirect, saUtilTableAndEntityLocalize(text, &vars, hearingEnt));
        }
        else 
        {
            Entity *owner = entFromDbId(missionOwnerIfNoAudience);
            if(owner && !audience)
                audience = owner;
    
            // Potential mismatch
            saBuildScriptVarsTable(&vars, audience, audience ? audience->db_id : missionOwnerIfNoAudience, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, false);

            saUtilEntSays(e, !e->pchar || e->pchar->iAllyID==kAllyID_Good, saUtilTableAndEntityLocalize(text, &vars, audience));
        }
    }

    ScriptVarsTableClear(&vars);
}

void ScriptServeFloater(ENTITY team, STRING text)
{
    NUMBER index;
    NUMBER count = NumEntitiesInTeam(ALL_PLAYERS);

    for (index = 0; index < count; index++)
    {
        ENTITY entity = GetEntityFromTeam(team, index + 1);
        Entity *e = EntTeamInternal(entity, 0, NULL);
        if (e)
        {
            serveFloater(e, e, StringLocalizeWithVars(text, entity), 0, kFloaterStyle_Attention, 0);
        }
    }
}

void ScriptSendChatMessage(ENTITY team, STRING text)
{
    NUMBER index;
    NUMBER count = NumEntitiesInTeam(ALL_PLAYERS);

    for (index = 0; index < count; index++)
    {
        ENTITY entity = GetEntityFromTeam(team, index + 1);
        Entity *e = EntTeamInternal(entity, 0, NULL);
        if (e)
        {
            sendChatMsg(e, StringLocalizeWithVars(text, entity), INFO_SVR_COM, 0);
        }
    }
}

void SetAudience(ENTITY entity, ENTITY audience)
{
    Entity* e;
    Entity *a;

    e = EntTeamInternal(entity, 0, NULL);
    a = EntTeamInternal(audience, 0, NULL);
    if (e && a)
    {
        e->audience = erGetRef(a);
    }
}

void SayOnClick(TEAM team, STRING text)
{
    int i, n;
    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if (e) 
            setSayOnClick(e, text); // need to pass NULL's through
    }
}

void ConditionOnClick(TEAM team, OnClickConditionType type, STRING condition, STRING say, STRING reward)
{
    int i, n;
    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if (e)
            addOnClickCondition(e, type, condition, say, reward);
    }
}

void ConditionOnClickClear(TEAM team)
{
    int i, n;
    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if (e)
            clearallOnClickCondition(e);
    }
}


// Didn't bother to make it fancy till we think of better heuristic
void RewardOnClick(TEAM team, STRING reward)
{
    int i, n;
    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if (e)
            setRewardOnClick(e, reward); // need to pass NULL's through
    }
}

void SayOnKillHero(TEAM team, STRING text)
{
    int i, n;
    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if (e)
            setSayOnKillHero(e, text); // need to pass NULL's through
    }
}

int NumTeamsInTeam(TEAM team)
{
    int count = 0;
    AITeamInternal(team, -1, &count);
    return count;
}

TEAM GetTeamFromTeam(TEAM team, NUMBER number)
{
    int teamid = AITeamInternal(team, number-1, NULL);
    if (teamid)
        return StringAdd("aiteam:", NumberToString(teamid));
    else
        return TEAM_NONE;
}


ENTITY CreateVillainNearEntity(TEAM teamName, STRING villainType, NUMBER level, STRING name, ENTITY target, FRACTION radius)
{
    Entity* e = EntTeamInternal(target, 0, NULL);
    Vec3 vLoc = { 0 };
    Vec3 vTop = { 0 };
    Vec3 vOffset = { 0 };
    char buf[2000];
    Vec3 UPVec = {0.0, 300.0, 0.0};
    Vec3 vVec = {0.0, 1.0, 0.0};
    bool bHit = 0;
    CollInfo coll;

    if (e)
    {
        vOffset[0] = RandomFraction(-1.0f, 1.0f) * radius;
        vOffset[2] = RandomFraction(-1.0f, 1.0f) * radius;
        addVec3(ENTPOS(e), vOffset, vOffset);
        vOffset[1] += 1.0f;

        // check to make sure this isn't inside something
        addVec3(vOffset, vVec, vLoc);
        addVec3(vOffset, UPVec, vTop);
        bHit = collGrid(NULL, vLoc, vTop, &coll, 1.0f, COLL_NOTSELECTABLE|COLL_HITANY|COLL_CYLINDER|COLL_BOTHSIDES);

        if (!bHit)
        {
            sprintf(buf, "coord:%f,%f,%f", vOffset[0], vOffset[1], vOffset[2]);
            return CreateVillain(teamName, villainType, level, name, buf);
        } else {
            return TEAM_NONE;
        }
    }
    return TEAM_NONE;
}

ENTITY CreateVillain(TEAM teamName, STRING villainType, NUMBER level, STRING name, LOCATION location)
{
    Vec3 pos;
    Entity* e;

    if (!GetPointFromLocation(location, pos))
    {
        ScriptError("CreateVillain given invalid position %s", location);
        return TEAM_NONE;
    }
    e = villainCreateByName(villainType, level, NULL, false, NULL, 0, NULL);
    if (!e)
    {
        ScriptError("Couldn't create villain %s, level %i", villainType, level);
        return TEAM_NONE;
    }

    if( name )
        strcpy(e->name, saUtilScriptLocalize(name));
    entUpdatePosInstantaneous(e, pos);
    aiInit(e, NULL);
    ScriptTeamAddEnt(e, teamName);
    return EntityNameFromEnt(e);
}

/*Entity * villainCreateDoppelganger(const char * dopplename, const char *overrideName, const char *overrideVillainGroupName, VillainRank villainrank, const char * villainclass,
                                   int level, bool bBossReductionMode, Mat4 pos, Entity * orig, EntityRef erCreator, const BasePower *creationPower ) */
ENTITY CreateDoppelganger(TEAM teamName, STRING doppleFlags, NUMBER level, STRING name, LOCATION location, ENTITY original, STRING customCritterDef,
                          STRING villainRank, STRING displayGroup, bool bossReduction)
{
    Mat4 mat;
    Entity* e;
    VillainRank rank = StaticDefineIntGetInt(ParseVillainRankEnum, villainRank);
    Entity* orig = EntTeamInternalEx(original, 0, NULL, 0, 1);

    if (!GetMatFromLocation(location, mat, orig ? ENTPOS(orig) : NULL))
    {
        ScriptError("CreateDoppelganger given invalid position %s", location ? location : "none");
        return TEAM_NONE;
    }
    if (!orig)
    {
        ScriptError("CreateDoppelganger given invalid original entity %s", original ? original : "none");
        return TEAM_NONE;
    }
    e = villainCreateDoppelganger(doppleFlags, name, displayGroup, rank, customCritterDef, level, bossReduction, mat, orig, 0, NULL);
    if (!e)
    {
        ScriptError("Couldn't create Doppelganger");
        return TEAM_NONE;
    }

    entUpdateMat4Instantaneous(e, mat);
    aiInit(e, NULL);
    ScriptTeamAddEnt(e, teamName);
    return EntityNameFromEnt(e);
}

void EntityRotateToFaceDirection(ENTITY entity, FRACTION facing)
{ 
    Entity *e;
    Mat4 mat;

    e = EntTeamInternal(entity, 0, NULL);

    if (e != NULL)
    {
        copyMat3(unitmat, mat);
        yawMat3(facing, mat);

        copyVec3(ENTPOS(e), mat[3]);

        entUpdateMat4Instantaneous(e, mat);
    }
}

int IsEntityOutside(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if (e)
    {
        Vec3 vLoc = { 0 };
        Vec3 vTop = { 0 };
        Vec3 UPVec = {0.0, 300.0, 0.0};
        Vec3 vVec = {0.0, 1.0, 0.0};
        bool bHit = 0;
        CollInfo coll;

        addVec3(ENTPOS(e), vVec, vLoc);
        addVec3(ENTPOS(e), UPVec, vTop);

        bHit = collGrid(NULL, vLoc, vTop, &coll, 1.0f, COLL_NOTSELECTABLE|COLL_HITANY|COLL_CYLINDER|COLL_BOTHSIDES);
        return !bHit;
    }
    return false;
}

int IsEntityNearGround(ENTITY entity, FRACTION dist)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if (e)
    {
        Vec3 vLoc = { 0 };
        Vec3 vTop = { 0 };
        Vec3 vDown = {0.0, 300.0, 0.0};
        Vec3 vVec = {0.0, 1.0, 0.0};
        bool bHit = 0;
        CollInfo coll;

        vDown[1] = -dist;
        addVec3(ENTPOS(e), vVec, vLoc);
        addVec3(ENTPOS(e), vDown, vTop);

        bHit = collGrid(NULL, vLoc, vTop, &coll, 1.0f, COLL_NOTSELECTABLE|COLL_HITANY|COLL_CYLINDER|COLL_BOTHSIDES);
        return bHit;
    }
    return false;
}

int IsEntityMoving(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if (e)
    {
        if (e->motion)
        {
            return (lengthVec3Squared(e->motion->vel) > 1.0f);
        } else {
            return false;
        }
    }
    return false;
}

ENTITY MissionOwner( void )
{
    Entity * e = 0;

    if( OnMissionMap() )
        e = entFromDbId(g_activemission->ownerDbid);

    return EntityNameFromEnt(e);

}

ENTITY EntityFromDBID( NUMBER dbid) 
{
    Entity* e = entFromDbIdEvenSleeping(dbid);

    if (e)
        return EntityNameFromEnt(e);
    else
        return TEAM_NONE;
}

NUMBER DBIDFromPlayer(ENTITY player)
{
    if (strnicmp(player, "player:", strlen("player:"))==0)
    {
        return atoi(player + strlen("player:"));
    }
    return 0;
}

STRING EntityName(ENTITY ent)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);
    if (e)
        return StringDupe(e->name);
    else
        return "";
}

STRING EntityPowerCreatedMe(ENTITY ent)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);
    if (e && e->pchar && e->pchar->ppowCreatedMe)
    {
        return StringDupe(dbg_BasePowerStr(e->pchar->ppowCreatedMe));
    }
    else
        return "";
}

void SetAITeamHero(TEAM team)
{
    SetAIAlly(team, kAllyID_Hero);
}

void SetAITeamMonster(TEAM team)
{
    SetAIAlly(team, kAllyID_Monster);
}

void SetAITeamVillain(TEAM team)
{
    SetAIAlly(team, kAllyID_Villain);
}

void SetAITeamVillain2(TEAM team)
{
    SetAIAlly(team, kAllyID_Villain2);
}

void SetAITeamToMatchMissionPlayers(TEAM team)
{
    if( g_activemission->missionAllyGroup == ENC_FOR_VILLAIN )
        SetAIAlly(team, kAllyID_Villain);
    else
        SetAIAlly(team, kAllyID_Hero);
}


NUMBER SameTeam( ENTITY ent1, ENTITY ent2 )
{
    Entity* e1 = EntTeamInternal( ent1, 0, NULL );
    Entity* e2 = EntTeamInternal( ent2, 0, NULL );

    if( e1 && e2 && e1->pchar && e2->pchar )
        return character_TargetIsFriend(e1->pchar, e2->pchar);
    return 0;

}

NUMBER ValidForPvPReward( ENTITY killer, ENTITY target)
{
    Entity* eKiller = EntTeamInternal( killer, 0, NULL );
    Entity* eTarget = EntTeamInternal( target, 0, NULL );
    NUMBER bValid = false;

    if( eKiller && eTarget )
         bValid = !pvp_IsOnDefeatList(eTarget, eKiller);

    return bValid;
}

// this is hacked together right now, & does not check for a specific team yet...
FRACTION GetHealthInternal( Entity * e )
{
    if ( !e )
        return 0.0; //Error instead?

    if( e->pchar )
    {
        if( !e->pchar->attrMax.fHitPoints )
            return 0.0;
        return    (float)e->pchar->attrCur.fHitPoints / (float)e->pchar->attrMax.fHitPoints;
    }
    else
    {
        return 1.0; //If you are an NPC, you are always at 100% health.
    }
}

// this is hacked together right now, & does not check for a specific team yet...
FRACTION GetHealth( ENTITY ENTITY )
{
    Entity* e = EntTeamInternal(ENTITY, 0, NULL); 

    return GetHealthInternal( e );
}

FRACTION GetEndurance(ENTITY entity)
{
    Entity * e = EntTeamInternalEx(entity, 0, NULL, 0, 1);

    if ( !e )
        return 0.0;

    if( e->pchar )
    {
        if( !e->pchar->attrMax.fEndurance )
            return 0.0;
        return    (float)e->pchar->attrCur.fEndurance / (float)e->pchar->attrMax.fEndurance;
    }
    else
    {
        return 1.0;
    }
}

FRACTION GetAbsorb(ENTITY entity)
{
    Entity * e = EntTeamInternalEx(entity, 0, NULL, 0, 1);

    if ( !e )
        return 0.0;

    if( e->pchar )
    {
        // Absorb is measured from a gameplay perspective relative to max HP
        if( !e->pchar->attrMax.fHitPoints )
            return 0.0;
        return    (float)e->pchar->attrCur.fAbsorb / (float)e->pchar->attrMax.fHitPoints;
    }
    else
    {
        return 0.0;
    }
}

NUMBER EntityFledMap( ENTITY entity )
{
    Entity* e = EntTeamInternal(entity, 0, NULL);

    if( e && e->fledMap)
        return 1;
    return 0;
}

// Determine if all members of the team are currently on the map.  We can't use a script TEAM and/or
// the entermap / exitp map logic, otherwise players on the team that never enter the map will not be
// counted.
NUMBER AreAllTeamMembersPresent( ENTITY ent )
{
    Entity* e = EntTeamInternal( ent, 0, NULL );

    if (e == NULL)                // NULL  pointer sanity check - this should never happen.
    {
        return 1;
    }
    if (e->teamup == NULL)        // If I'm not on a team, then by implication everyone is here
    {
        return 1;
    }
    if (e->teamup->map_playersonmap == e->teamup->members.count)
    {
        return 1;
    }
    return 0;
}

// Flag if we should override use of a prison gurney on a mission map
void OverridePrisonGurney( ENTITY ent, NUMBER overrideFlag )
{
    Entity* e = EntTeamInternal( ent, 0, NULL );

    if (e == NULL)
    {
        // sanity check - this should never happen.
        return;
    }
    // Overriding the prison gurney?
    if (overrideFlag)
    {
        // Set the mission_gurney_map_id to zero, this turns it off for this entity.
        e->mission_gurney_map_id = 0;  
        return;
    }
    // We want to turn back on now, first check if this map has gurnies, and that it actually is a mission map
    if ((db_state.has_gurneys || db_state.has_villain_gurneys) && OnMissionMap())
    {
        // Looks good, set to current map_id and we're done.
        e->mission_gurney_map_id = db_state.map_id;
        return;
    }
    // Something went wrong, turn it off
    e->mission_gurney_map_id = 0;
}

void OverridePrisonGurneyWithTarget( ENTITY player, NUMBER overrideFlag, STRING spawnTarget )
{
    Entity* e = EntTeamInternal( player, 0, NULL );

    if (e && ENTTYPE_PLAYER == ENTTYPE(e) )
    {
        // Overriding the map specific gurney?
        if (overrideFlag)
        {
            // Set the mission_gurney_map_id to zero, this turns it off for this entity.
            e->mission_gurney_map_id = 0;
        }
        else
        {
            SAFE_FREE(e->mission_gurney_target);
            if(spawnTarget && spawnTarget[0])
            {
                e->mission_gurney_map_id = db_state.map_id;
                e->mission_gurney_target = strdup( spawnTarget );
            }
        }
    }
}

// teleport a team / entity to a location.  
void TeleportToLocation(ENTITY team, LOCATION loc)
{
    int n;
    int i;
    int j;
    Entity *e;
    Entity *pet;
    Mat4 mat;
    Vec3 ref = { 0 };

    if (!GetMatFromLocation(loc, mat, ref))
    {
        return;
    }

    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        if (e = EntTeamInternal(team, i, NULL))
        {
            // Except we don't do this if the player is in the middle of going through a door
            if (!e->doorFrozen)
            {
                entUpdatePosInstantaneous(e, mat[3]);

                // Now drag all living pets along
                for(j = e->petList_size - 1; j >= 0; j--)
                {
                    pet = erGetEnt(e->petList[j]);
                    if (pet && pet->pchar && pet->pchar->attrCur.fHitPoints > 0.0f)
                    {
                        entUpdatePosInstantaneous(pet, mat[3]);
                    }
                }
            }
        }
    }
}

// Used to allow players to transfer from map to map under script control.
// Takes the team, and moves them all to the provided zone name, arriving at the given spawn name
void ScriptEnterScriptDoor(TEAM team, STRING zoneName, STRING spawnName)
{
    int i;
    int j;
    int n;
    Entity *e;

    spawnName = StringCopy(spawnName);
    // DGNOTE 08/26/2009
    // spawnName is munged together with the mapID that corresponds to zoneName using a '.' as a separator.
    // This string is subsequently torn apart in character_NamedTeleport(...) over in character_tick.c.  The
    // problem is that someone uses strtok(..., ".") to do this which completely destroys "coord:" style
    // spawnNames, since they have decimal points in them.  So we convert the '.'s to '!'s on the understanding
    // that setInitialPosition(...) over in parseClientInput.c will convert back.  That guy is the final and only
    // consumer of this data, so this will not have any unforseen side effects.
    if (strnicmp(spawnName, "coord:", 6 /* == strlen("coord:") */ ) == 0)
    {
        for (j = 0; spawnName[j]; j++)
        {
            if (spawnName[j] == '.')
            {
                ((char*)spawnName)[j] = '!';
            }
        }
    }
    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        if (e = EntTeamInternal(team, i, NULL))
        {
            // Entity must have a pl and a pchar.  Although I suspect the existance of a pl imples a valid pchar
            // Also check if a special return is already in progress, and do nothing if so
            if (e->pl && e->pchar && !e->pl->specialMapReturnInProgress)
            {
                // Send them on their way.
                enterScriptDoor(e, zoneName, spawnName, 0, 0);
            }
        }
    }
}

void ScriptSetSpecialReturnInProgress(TEAM team)
{
    int i;
    int n;
    Entity *e;

    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        if (e = EntTeamInternal(team, i, NULL))
        {
            if (e->pl)
            {
                e->pl->specialMapReturnInProgress = 1;
            }
        }
    }
}

// Destroy all pets of all entities on the team
void ScriptDestroyAllPets(TEAM team)
{
    int n;
    int i;
    int j;
    Entity *e;
    Entity *pet;

    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        if (e = EntTeamInternal(team, i, NULL))
        {
            for(j = e->petList_size - 1; j >= 0; j--)
            {
                pet = erGetEnt(e->petList[j]);
                if (pet && pet->pchar)
                {
                    pet->pchar->attrCur.fHitPoints = 0.0f;
                }
            }
        }
    }
}

// Have an Entity activate a power.  If a target is given, activate the power on that target
void EntityActivatePower(ENTITY entity, ENTITY target, STRING powerTitle)
{
    Entity *e;
    Entity *t;
    EntityRef tr;
    char *localPower;
    char *powerCat;
    char *powerSet;
    char *powerName;
    const BasePower *ppowBase;
    Power *power;

    strdup_alloca(localPower, powerTitle);
    powerCat = strtok(localPower, ".");
    powerSet = strtok(NULL, ".");
    powerName = strtok(NULL, ".");
    // Yes, I could combine these into a single if.
    // No, I am not going to because the compiler is going to generate the same code in either case, and this is a metric crap-ton easier to debug.
    if (powerCat == NULL || powerSet == NULL || powerName == NULL)
    {
        return;
    }
    if ((ppowBase = powerdict_GetBasePowerByName(&g_PowerDictionary, powerCat, powerSet, powerName)) == NULL)
    {
        return;
    }
    if ((e = EntTeamInternal(entity, 0, NULL)) == NULL)
    {
        return;
    }
    if (target != NULL)
    {
        if ((t = EntTeamInternal(target, 0, NULL)) == NULL)
        {
            return;
        }
        if ((tr = erGetRef(t)) == 0)
        {
            return;
        }
    }
    else
    {
        tr = 0;
    }
    if (e->pchar == NULL)
    {
        return;
    }
    if ((power = character_OwnsPower(e->pchar, ppowBase)) == NULL)
    {
        return;
    }
    // Activate the power.  If there's a target, have the entity target the target, and then set the queued power to the
    // one we want.  character_tick will take care of it from here ...
    if (tr != 0)
    {
        e->pchar->erTargetCurrent = tr;
        e->pchar->erTargetQueued = tr;
    }
    e->pchar->ppowQueued = power;
    // assert(e->pchar->ppowCurrent == NULL);
}

// Have an Entity deactivate a toggle power.  I ought to check that the entity has the power turned on first ...
// Much of this is a cut and paste from above.  I'm on the fence regarding keeping two separate routines, or
// creating one monolithic "PowerControl" routine.  
void EntityDeactivateTogglePower(ENTITY entity, STRING powerTitle)
{
    Entity *e;
    char *localPower;
    char *powerCat;
    char *powerSet;
    char *powerName;
    const BasePower *ppowBase;
    Power *power;

    strdup_alloca(localPower, powerTitle);
    powerCat = strtok(localPower, ".");
    powerSet = strtok(NULL, ".");
    powerName = strtok(NULL, ".");
    // Yes, I could combine these into a single if.
    // No, I am not going to because the compiler is going to generate the same code in either case, and this is a metric crap-ton easier to debug.
    if (powerCat == NULL || powerSet == NULL || powerName == NULL)
    {
        return;
    }
    if ((ppowBase = powerdict_GetBasePowerByName(&g_PowerDictionary, powerCat, powerSet, powerName)) == NULL)
    {
        return;
    }
    if ((e = EntTeamInternal(entity, 0, NULL)) == NULL)
    {
        return;
    }
    if (e->pchar == NULL)
    {
        return;
    }
    if ((power = character_OwnsPower(e->pchar, ppowBase)) == NULL)
    {
        return;
    }
    // And turn it off
    character_ShutOffTogglePower(e->pchar, power);
}

// this is hacked together right now, & does not check for a specific team yet...
FRACTION SetHealth( ENTITY entity, FRACTION percentHealth, NUMBER unAttackedOnly )
{
    F32 newHealth = 0;
    Entity* e = EntTeamInternal(entity, 0, NULL);

    //If the health was being set for some crazy reason like falling when walking across the world
    //don't do it if the guy is anywhere near combat
    if( e && unAttackedOnly)
    {
        AIVars * ai = ENTAI(e);
        if( ai->wasAttacked )
            newHealth = 0.0; //Failure should be own flag
    }
    percentHealth = MAX( percentHealth, 0.0 );
    percentHealth = MIN( percentHealth, 1.0 );

    if (e && e->pchar)
    {
        newHealth = e->pchar->attrCur.fHitPoints = percentHealth * e->pchar->attrMax.fHitPoints;
        character_HandleDeathAndResurrection(e->pchar, NULL);
    }
    return newHealth; 
}

void SetPosition( ENTITY entity, LOCATION loc )
{
    Vec3 pos;
    Entity* e;

    if (!GetPointFromLocation(loc, pos) )
        return;

    e = EntTeamInternal(entity, 0, NULL);

    if( e )
    {
        entUpdatePosInstantaneous( e, pos );
    }
}

void SetMap (ENTITY entity, NUMBER map, STRING spawnTarget)
{
    Entity* e;

    if (map == db_state.map_id)
        return;

    e = EntTeamInternal(entity, 0, NULL);

    if( e )
    {
        e->adminFrozen = 1;
        e->db_flags |= DBFLAG_DOOR_XFER;
        if (spawnTarget)    
            strcpy(e->spawn_target, spawnTarget);

        dbAsyncMapMove(e,map,NULL,XFER_STATIC | XFER_FINDBESTMAP);
    }
}

void ScriptApplyKnockBack(ENTITY player, Vec3 push, int dokb)
{
    Entity *e;
    MotionState *motion;
    FuturePush *fp;

    // ?? Do I want to let this apply KB to a whole team?
    if (e = EntTeamInternal(player, 0, NULL))
    {
        if ((motion = e->motion) && ENTTYPE(e) == ENTTYPE_PLAYER && e->client)
        {
            fp = addFuturePush(&e->client->controls, SEC_TO_ABS(0));
            copyVec3(push, fp->add_vel);
            fp->do_knockback_anim = dokb ? 1 : 0;
        }
    }
}

STRING GetVillainDefName( ENTITY entity )
{
    Entity* e = EntTeamInternal(entity, 0, NULL);

    if (e && e->villainDef) {
        return StringDupe(e->villainDef->name);
    } else {
        return NULL;
    }
}

// Find a critter on the map with the given villain def name.  Note that if there are multiple critters with the same villaindef this will
// return one of them, it's just not defined which one.
// This would be hideously inefficient without some sort of caching, we'd have to run through every critter on the map every time it was called.
// So we ask the caller to cache for us.  The contract we have with the calling site is that they keep a persistent value and init it to NULL.
// This arrives as cachedEntity.  The first test determines if this is valid, and if so whether it points to the right type of critter.  This
// test is very fast, and if it works we're all done.  The intent is that most of the time, we'll have already found the critter, so we just
// keep handing it back.  If it dies and gets recreated, or if this is the first time thru, the test will fail, in which case we do the full
// search.
// The two main forms of abuse of this are trying to find an entity that doesn't exist, and not saving the returned result.  Neither of these
// are fatal, but they do risk a performance hit.
ENTITY GetVillainByDefName( STRING defName, ENTITY cachedEntity )
{
    int n;
    int i;
    Entity *e;

    if (!StringEmpty(cachedEntity) && StringEqual(defName, GetVillainDefName(cachedEntity)))
    {
        return cachedEntity;
    }

    EntTeamInternalEx(ALL_CRITTERS, -1, &n, 0, 0);

    for (i = 0; i < n; i++)
    {
        e = EntTeamInternal(ALL_CRITTERS, i, NULL);
        if (e && e->villainDef && StringEqual(defName, e->villainDef->name))
        {
            return EntityNameFromEnt(e);
        }
    }
    return TEAM_NONE;
}

STRING GetVillainDefClassName( ENTITY entity )
{
    Entity* e = EntTeamInternal(entity, 0, NULL);

    if (e && e->villainDef && e->villainDef->characterClassName) {
        return StringDupe(e->villainDef->characterClassName);
    } else {
        return NULL;
    }
}


NUMBER GetVillainGroupID( ENTITY entity )
{
    Entity* e = EntTeamInternal(entity, 0, NULL);

    if ( e && e->villainDef ) {
        return e->villainDef->group;
    } else {
        return 0;
    }
}


NUMBER GetVillainGroupIDFromName( STRING name )
{
    return villainGroupGetEnum(name);
}


STRING GetDisplayName( ENTITY entity )
{

    Entity* e = EntTeamInternal(entity, 0, NULL);

    if (e && e->name) {
        return StringDupe(e->name);
    } else {
        return NULL;
    }
}

NUMBER GetLevel( ENTITY entity )
{
    Entity* e = EntTeamInternal(entity, 0, NULL);

    if (e && e->pchar) {
        return character_GetSecurityLevelExtern(e->pchar);
    } else {
        return 0;
    }
}

NUMBER GetCombatLevel( ENTITY entity )
{
    Entity* e = EntTeamInternal(entity, 0, NULL);

    if (e && e->pchar) {
        return character_GetCombatLevelExtern(e->pchar);
    } else {
        return 0;
    }
}

NUMBER GetExperienceLevel( ENTITY entity )
{
    Entity* e = EntTeamInternal(entity, 0, NULL);

    if (e && e->pchar) {
        return character_GetExperienceLevelExtern(e->pchar);
    } else {
        return 0;
    }
}

NUMBER CanLevel( ENTITY entity )
{
    Entity* e = EntTeamInternal(entity, 0, NULL);

    if (e && e->pchar && character_CanLevel(e->pchar)) {
        return 1;
    } else {
        return 0;
    }
}

NUMBER GetAccessLevel( ENTITY entity )
{
    Entity* e = EntTeamInternal(entity, 0, NULL);

    if (e) {
        return e->access_level;
    } else {
        return 0;
    }
}


FRACTION GetReputation( ENTITY entity )
{
    Entity* e = EntTeamInternal(entity, 0, NULL);

    if (e && (ENTTYPE(e) == ENTTYPE_PLAYER)) {
        return playerGetReputation(e);
    } else {
        return 0;
    }
}

NUMBER GetNPCIndex( ENTITY entity )
{ 
    Entity* e = EntTeamInternal(entity, 0, NULL);

    if (e && (ENTTYPE(e) == ENTTYPE_NPC)) {
        return (e->npcIndex);
    } else {
        return 0;
    }
}

void SetAIAlly(TEAM team, int allyID)
{
    int i, n;

    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);

        if (!e)
            continue;

        if (allyID == kAllyID_None)
            aiBehaviorAddPermFlag( e, ENTAI(e)->base, "Team(None)" );
        else if( allyID == kAllyID_Hero )
            aiBehaviorAddPermFlag( e, ENTAI(e)->base, "Team(Hero)" ); 
        else if( allyID == kAllyID_Villain )
            aiBehaviorAddPermFlag( e, ENTAI(e)->base, "Team(Villain)" ); 
        else if( allyID == kAllyID_Monster )
            aiBehaviorAddPermFlag( e, ENTAI(e)->base, "Team(Monster)" ); 
        else if( allyID == kAllyID_Villain2 )
            aiBehaviorAddPermFlag( e, ENTAI(e)->base, "Team(Villain2)" ); 
    }
}

void SetAIGang(TEAM team, int gangID)
{
    int i, n;
    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);

        if (e)
            aiScriptSetGang(e, gangID);
    }
}

void SetAIGangPerm(TEAM team, int gangID)
{
    int i, n;
    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);

        if (e)
        {
            aiScriptSetGang(e, gangID);
            if(e->pchar)
                e->pchar->iGandID_Perm=1;
        }
    }
}

NUMBER GetAIGang(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if( e && e->pchar )
        return e->pchar->iGangID;
    return 0;
}

NUMBER GetAIAlly(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if( e && e->pchar )
        return e->pchar->iAllyID;
    return 0;
}

STRING GetAllyString(ENTITY entity)
{
    Entity * e = EntTeamInternalEx(entity, 0, NULL, 0, 1);
    STRING lookupString = NULL;
    if(e && e->pchar)
        lookupString = StaticDefineIntRevLookup(ParseTeamNames, e->pchar->iAllyID);
    return lookupString ? lookupString : "other";
}

NUMBER GetPlayerType(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if( e && e->pl )
        return e->pl->playerType;
    return 0;
}

NUMBER GetPlayerTypeByLocation(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if( e && e->pchar )
        return e->pchar->playerTypeByLocation;
    return 0;
}

STRING GetPlayerAlignmentString(ENTITY entity)
{
    Entity * e = EntTeamInternalEx(entity, 0, NULL, 0, 1);
    STRING lookupString = NULL;
    STATIC_INFUNC_ASSERT(kPlayerType_Count == 2 && kPlayerSubType_Count == 3)
    if(e->pl)
        lookupString = StaticDefineIntRevLookup(ParseAlignment, getEntAlignment(e));
    return lookupString ? lookupString : "hero";
}

NUMBER GetPlayerPraetorianProgress(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if( e && e->pl )
        return e->pl->praetorianProgress;
    return 0;
}

STRING GetPlayerPraetorianProgressString(ENTITY entity)
{
    Entity * e = EntTeamInternalEx(entity, 0, NULL, 0, 1);
    STRING lookupString = NULL;
    STATIC_INFUNC_ASSERT(kPraetorianProgress_Count == 7)
    if(e && e->pl)
        lookupString = StaticDefineIntRevLookup(ParsePraetorianProgress, e->pl->praetorianProgress);
    return lookupString ? lookupString : "normal";
}

NUMBER GetPlayerSGID(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if (e)
        return e->supergroup_id;
    return 0;
}

// TODO change this to return the raid ID.
// For now, it returns the SG ID
NUMBER GetPlayerRaidID(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if (e)
        return e->supergroup_id;
    return 0;
}

int IsEntityHero(ENTITY entity)
{
    return (GetPlayerType(entity) == kPlayerType_Hero);
}

int IsEntityOnBlueSide(ENTITY entity)
{
    return GetPlayerTypeByLocation(entity) == kPlayerType_Hero;
}

int IsEntityVillain(ENTITY entity)
{
    return (GetPlayerType(entity) == kPlayerType_Villain);
}

int IsEntityOnRedSide(ENTITY entity)
{
    return GetPlayerTypeByLocation(entity) == kPlayerType_Villain;
}

int IsEntityMonster(ENTITY entity)
{
    return (GetAIAlly(entity) == kAllyID_Monster);
}

int IsEntityPrimal(ENTITY entity)
{
    int ret = 0;
    NUMBER praetorianProgress = GetPlayerPraetorianProgress(entity);
    STATIC_INFUNC_ASSERT(kPraetorianProgress_Count == 7)
    if (praetorianProgress == kPraetorianProgress_PrimalBorn ||
        praetorianProgress == kPraetorianProgress_PrimalEarth )
    {
        ret = 1;
    }

    return ret;
}

int IsEntityPraetorian(ENTITY entity)
{
    int ret = 0;
    NUMBER praetorianProgress = GetPlayerPraetorianProgress(entity);
    STATIC_INFUNC_ASSERT(kPraetorianProgress_Count == 7)
    if (praetorianProgress == kPraetorianProgress_Praetoria ||
        praetorianProgress == kPraetorianProgress_Tutorial ||
        praetorianProgress == kPraetorianProgress_TravelHero ||
        praetorianProgress == kPraetorianProgress_TravelVillain )
    {
        ret = 1;
    }

    return ret;
}

int IsEntityPraetorianTutorial(ENTITY entity)
{
    int ret = 0;
    STATIC_INFUNC_ASSERT(kPraetorianProgress_Count == 7)
    if (GetPlayerPraetorianProgress(entity) == kPraetorianProgress_Tutorial)
    {
        ret = 1;
    }

    return ret;
}

int IsEntityPlayer(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if( e )
    {
        return (ENTTYPE(e) == ENTTYPE_PLAYER);
    } else {
        return false;
    }
}

int IsEntityCritter(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if( e )
    {
        return (ENTTYPE(e) == ENTTYPE_CRITTER);
    } else {
        return false;
    }
}

int IsEntityHidden(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if( e )
    {
        return (ENTINFO(e).hide);
    } else {
        return false;
    }
}

int IsEntityOnTeam(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if( e )
    {
        if (ENTTYPE(e) == ENTTYPE_PLAYER)
        {
            return (e->teamup != NULL);
        } else {
            return false;
        }
    } else {
        return false;
    }
}

int IsEntityOnScriptTeam(ENTITY entity, TEAM team)
{
    int i;
    ScriptTeam* steam;
    Entity* e;
    EntityRef ref;

    if (!entity || !entity[0] || !team || !team[0])
        return 0;

    e = EntTeamInternal(entity, 0, NULL);
    ref = e ? erGetRef(e) : 0;

    if (!ref)
        return 0;

    if (!ScriptTeamValid(team))
    {
        ScriptError("IsPlayerOnScriptTeam given invalid team name: %s", team);
        return 0;
    }
    steam = ScriptTeamFind(team);
    if (steam)
    {
        for (i = eaSize(&steam->members) - 1; i >= 0; i--)
        {
            if (ref == *steam->members[i])
            {
                return 1;
            }
        }
    }
    return 0;
}

int IsEntityUntargetable(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if( e )
    {
        return e->untargetable;
    }
    return false;
}

int IsEntityAFK(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if( e && (ENTTYPE(e) == ENTTYPE_PLAYER) && e->pl)
    {
        return e->pl->afk;
    }
    return false;
}

int IsEntityPhased(ENTITY entity)
{
    Entity * e = EntTeamInternal(entity, 0, NULL); 
    return e && e->pchar && !(e->pchar->bitfieldCombatPhases & 1);
}

int IsEntityActive(ENTITY entity)
{
    Entity * e = EntTeamInternalEx(entity, 0, NULL, 0, 1);
    return e && ScriptKarmaIsPlayerActive(e->db_id);
}

int IsEntityOnTaskforce(ENTITY entity)
{
    Entity * e = EntTeamInternalEx(entity, 0, NULL, 0, 1);
    return e && e->pl && e->pl->taskforce_mode;
}

int IsEntityOnFlashback(ENTITY entity)
{
    return EntityHasRewardToken(entity, "OnFlashback");
}

int IsEntityOnArchitect(ENTITY entity)
{
    return EntityHasRewardToken(entity, "OnArchitect");
}

int GetLevelingPact(ENTITY entity)
{
    Entity * e = EntTeamInternalEx(entity, 0, NULL, 0, 1);
    return e ? e->levelingpact_id : 0;
}

STRING GetOrigin(ENTITY entity)
{
    Entity * e = EntTeamInternalEx(entity, 0, NULL, 0, 1);
    return (e && e->pchar && e->pchar->porigin) ? e->pchar->porigin->pchName : "unknown";
}

STRING GetClass(ENTITY entity)
{
    Entity * e = EntTeamInternalEx(entity, 0, NULL, 0, 1);
    return (e && e->pchar && e->pchar->pclass) ? e->pchar->pclass->pchName : "unknown";
}

STRING GetGender(ENTITY entity)
{
    Entity * e = EntTeamInternalEx(entity, 0, NULL, 0, 1);
    STRING lookupString = NULL;
    if(e)
        lookupString = StaticDefineIntRevLookup(ParseGender, e->gender);
    return lookupString ? lookupString : "undefined";
}

int EvalEntityRequires(ENTITY entity, STRING expr, STRING dataFilename)
{
    char        *required;
    char        *argv[100];
    StringArray exprArray;
    int            argc;
    int            i;
    Entity*        e = EntTeamInternal(entity, 0, NULL);
    int            retval = false;

    if( e )
    {
        required = strdup(expr);
        argc = tokenize_line(required, argv, 0);
        if (argc > 0)
        {
            eaCreate(&exprArray);
            for (i = 0; i < argc; i++)
                eaPush(&exprArray, argv[i]);

            retval = chareval_Eval(e->pchar, exprArray, dataFilename);
        }
        if (required)
            free(required);
    }
    return retval;
}

NUMBER GetMonsterCreateTime(ENTITY entity)
{
    Entity* e = EntTeamInternal(entity, 0, NULL);
    if( e )
        return e->birthtime;
    return 0;
}

//Sets Team to be part of another team in addition to its current team
void SetScriptTeam( TEAM currentTeam, TEAM additionalTeam )
{
    int i, n;
    n = NumEntitiesInTeam(currentTeam);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(currentTeam, i, NULL);
        if( e )
            ScriptTeamAddEnt( e, additionalTeam );
    }
}

// Sets Team to be on the new team and removes them from the current team
void SwitchScriptTeam( TEAM team, TEAM fromTeam, TEAM newTeam )
{
    int i;

    // Run this loop backwards, otherwise this will break very badly if team and fromTeam are the same
    for (i = NumEntitiesInTeam(team) - 1; i >= 0; i--)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if( e ) 
        {
            ScriptTeamRemoveEnt(e, fromTeam);
            ScriptTeamAddEnt( e, newTeam );
        }
    }
}

// Remove an entity from all teams
void LeaveAllScriptTeams(ENTITY entity)
{
    int i;
    Entity *e = EntTeamInternal(entity, 0, NULL);

    if (g_currentScript && g_currentScript->teams) 
    {
        for (i = eaSize(&g_currentScript->teams) - 1; i >= 0; i--)
        {
            ScriptTeamRemoveEnt(e, g_currentScript->teams[i]->name);
        }
    }
}

void SetSpecialMapReturnData(TEAM team, STRING type, STRING value)
{
    int i;
    int n;
    Entity *e;
    STRING returnValue;

    // TODO - remove the third test here when we're sure that LWR return works right
    if (StringEmpty(type) || StringEmpty(value) || server_state.disableSpecialReturn)
    {
        returnValue = "";
    }
    else
    {
        returnValue = StringAdd(StringAdd(type, "="), value);
    }
    if (strlen(returnValue) > 127)
    {
        // If this ever happens, things are probably going to go south. :/
        devassertmsg(0, "SetSpecialMapReturnData about to terminate a string at 127 characters in read only memory");
        ((char*)returnValue)[127] = 0;
    }
    n = NumEntitiesInTeam(team);
    for (i =  0; i < n; i++)
    {
        e = EntTeamInternal(team, i, NULL);
        if (e && e->pl)
        {
            strcpy(e->pl->specialMapReturnData, returnValue);
        }
    }
}

STRING GetSpecialMapReturnData(ENTITY player)
{
    Entity *e;

    e = EntTeamInternal(player, 0, NULL);
    if (e && e->pl && !StringEmpty(e->pl->specialMapReturnData))
    {
        return StringCopy(e->pl->specialMapReturnData);
    }
    return "";
}

void ScriptSendDialog(TEAM team, STRING message)
{
    int i;
    int n;
    Entity *e;

    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        e = EntTeamInternal(team, i, NULL);
        if (e)
        {
            sendDialog(e, message);
        }
    }
}

NUMBER GetTeamSizeOverride( void )
{
    return g_encounterForceTeam;
}

static Entity * staticTeamList[MAX_ENTITIES_PRIVATE];

static int entArray[MAX_ENTITIES_PRIVATE];
NUMBER GetAllEntities( ENTITY    * entList, 
                    STRING    villainGroup,    
                    STRING    villainDef,
                    STRING    villainHasTag,
                    STRING    entType,        //Not yet implemented
                    STRING  scriptTeam,    
                    FRACTION radius, 
                    LOCATION center,
                    AREA    area,
                    NUMBER  isAlive,
                    NUMBER  isDead,
                    NUMBER  max,
                    NUMBER  onceOnly)
{
    int cnt;
    int entCount;
    int i;
    Vec3 centerPoint;
    int usedGrid = 0;
    int hasLoc;
    int villainGroupEnum = 0;
    int teamSize;

    hasLoc = GetPointFromLocation(center, centerPoint);
    if( villainGroup )
        villainGroupEnum = villainGroupGetEnum(villainGroup);

    //If you are given a center and a radius, cull the list using the entity grid
    if( hasLoc && radius && radius < MIN_PLAYER_CRITTER_RADIUS )
    {
        entCount = mgGetNodesInRange(0, centerPoint, (void **)entArray, 0);
        usedGrid = 1;
    }

    cnt = 0;

    //If you are given an entteam requirement, go ahead and get a list of all the entities on that team
    //TO DO this may be too expensive, and it may benefit from a quicksort
    teamSize = 0;
    if( scriptTeam )
    {
        int i;
        Entity* e;
        teamSize = NumEntitiesInTeam(scriptTeam);
        for(i = 0; i < teamSize; i++)
        {
            e = EntTeamInternal(scriptTeam, i, NULL);
            if (e)
                staticTeamList[i] = e;
        }
    }


    //If this didn't work, just give me everybody
    if( !usedGrid )
        entCount = entities_max;
    
    for(i=0; i<entCount; i++)
    {
        Entity * e;
        //EntType entType = ENTTYPE_BY_ID(idx);

        if( cnt >= max )
            break;

        //Get Next Entity based on what kind of list you have
        if( !usedGrid )
        {
            e = entities[i];
            if( !(entity_state[i] & ENTITY_IN_USE) )
                continue;
        }
        else
        {
            e = entities[entArray[i]];
        }
        
        //Distance Check
        if( hasLoc && radius && !ent_close( radius, centerPoint, ENTPOS(e) ) )
            continue;

        //Area Check
        if( area && !PointInArea(ENTPOS(e), area, e) )
            continue;

        //Health Check
        if( isDead && GetHealthInternal( e ) > 0.0 )
            continue;
        if( isAlive && GetHealthInternal( e ) <= 0.0 )
            continue;
        
        //Group Check
        if( villainGroupEnum )
        {
            if( !e->villainDef || (e->villainDef->group != villainGroupEnum) ) 
                continue;
        }

        //VillainDef Name Check
        if( villainDef )
        {
            if( !e->villainDef || stricmp(e->villainDef->name, villainDef) != 0 )
                continue;
        }

        //VillainDef Power Tag Check
        if( villainHasTag )
        {
            if( !e->villainDef || !villainDefHasTag(e->villainDef, villainHasTag) )
                continue;
        }

        //Script Team Check (TODO this is slow)
        if( scriptTeam )
        {
            int j, hit = 0;
            for( j = 0 ; j < teamSize ; j++ )
            { 
                if( e == staticTeamList[j] )
                {
                    hit = 1;
                    break;
                }
            }
            if( !hit )
                continue;
        }

        //Temporary till I add death callback.  Means only find this guy once, ever
        if( onceOnly )
        {
            if( e->taggedAsScriptFound ) 
                continue;
            e->taggedAsScriptFound = 1;
        }

        entList[cnt++]= EntityNameFromEnt(e);
    }

    return cnt;
}

NUMBER GetAllPets(    ENTITY    source,
                    ENTITY    * entList, 
                    STRING    villainGroup,    
                    STRING    villainDef,
                    STRING    villainHasTag,
                    STRING    entType,        //Not yet implemented
                    STRING  scriptTeam,        //Not yet implemented
                    FRACTION radius, 
                    LOCATION center,
                    AREA    area,
                    NUMBER  isAlive,
                    NUMBER  isDead,
                    NUMBER  max )
{
    Entity *owner;
    int cnt;
    int i;
    Vec3 centerPoint;
    int hasLoc;
    int villainGroupEnum = 0;

    hasLoc = GetPointFromLocation(center, centerPoint);
    if( villainGroup )
        villainGroupEnum = villainGroupGetEnum(villainGroup);

    cnt = 0;

    owner = EntTeamInternal(source, 0, NULL);
    if(!owner)
        return 0;
    
    for (i = 0; i < owner->petList_size; i++)
    {
        Entity * e = erGetEnt(owner->petList[i]);

        if(!e)
            continue;

        if( cnt >= max )
            break;
        
        //Distance Check
        if( hasLoc && radius && !ent_close( radius, centerPoint, ENTPOS(e) ) )
            continue;

        //Area Check
        if( area && !PointInArea(ENTPOS(e), area, e) )
            continue;

        //Health Check
        if( isDead && GetHealthInternal( e ) > 0.0 )
            continue;
        if( isAlive && GetHealthInternal( e ) <= 0.0 )
            continue;
        
        //VillainDef VillainGroup Check
        if( villainGroupEnum )
        {
            if( !e->villainDef || (e->villainDef->group != villainGroupEnum) ) 
                continue;
        }

        //VillainDef Name Check
        if( villainDef )
        {
            if( !e->villainDef || stricmp(e->villainDef->name, villainDef) != 0 )
                continue;
        }

        //VillainDef Power Tag Check
        if( villainHasTag )
        {
            if( !e->villainDef || !villainDefHasTag(e->villainDef, villainHasTag) )
                continue;
        }

        entList[cnt++]= EntityNameFromEnt(e);
    }

    return cnt;
}

char * getContactHeadshotScriptSMF(ENTITY entContact, ENTITY entPlayer)
{
    static char * estr;
    Entity *e = EntTeamInternal(entContact, 0, NULL);
    Entity *player = EntTeamInternal(entPlayer, 0, NULL);

    estrClear(&estr);
    if( e && player )
    {
        if( e && e->doppelganger )
            estrConcatf(&estr, "<img align=left src=svr:%d>", e->id );
        else
            estrConcatf(&estr, "<img align=left src=npc:%d>", e->npcIndex );
    }
    return estr;
}

ENTITY MyEntity()
{
    if (!g_currentScript->entity)
    {
        ScriptError("MyEntity called from non-entity script %s\n", g_currentScript->function->name);
        return TEAM_NONE;
    }
    return EntityNameFromEnt(g_currentScript->entity);
}
