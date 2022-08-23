/*\
 *
 *    scripthookcallbacks.c - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides all the functions CoH script files should be referencing -
 *        essentially a list of hooks into game systems.
 *
 *    This file contains all functions related to callbacks
 *
 */

#include "script/script.h"
#include "script/scriptengine.h"
#include "script/scriptutil.h"

#include "storyarc/storyarcprivate.h"
#include "generator/encounterPrivate.h"
#include "ai/entai.h"
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
#include "entity/entity.h"
#include "storyarc/pnpcCommon.h"

#include "script/Scripthook/ScriptHookInternal.h"

// *********************************************************************************
//  Enter & exit map hooks
// *********************************************************************************
 
ScriptClosureList g_playerExitingMapCallbacks;

void OnPlayerExitingMap(PlayerExitingMapFunc f)
{
    SetScriptClosure(&g_playerExitingMapCallbacks, g_currentScript->id, f, NULL);
}
void luaCallback_OnPlayerExitingMap(STRING lua_f)
{
    SetScriptClosure(&g_playerExitingMapCallbacks, g_currentScript->id, lua_f ? lua_callbackEntityEntity : NULL, lua_f);
}
static int playerExitingMap(void* func, Entity* player, void* v)
{
    ((PlayerExitingMapFunc)func)(EntityNameFromEnt(player));
    return 0;
}
void ScriptPlayerExitingMap(Entity* player)
{
    Vec3            none = {0};

    // call all exit callbacks
    ScriptClosureListCall(&g_playerExitingMapCallbacks, playerExitingMap, player, NULL);
    
    // remove any script controlled waypoints
    ClearAllWaypoints(EntityNameFromEnt(player));

    // Check the ScriptUI callbacks
    ScriptUIPlayerExitedMap(player);
}


ScriptClosureList g_playerEnteringMapCallbacks;


void OnPlayerEnteringMap(PlayerEnteringMapFunc f)
{
    OnPlayerEnteringMapEx(f, NULL);
}

void OnPlayerEnteringMapEx(PlayerEnteringMapFunc f, STRING lua_f)
{
    int n, i;

    if (f)
        SetScriptClosure(&g_playerEnteringMapCallbacks, g_currentScript->id, f, NULL);
    else if (lua_f)
        SetScriptClosure(&g_playerEnteringMapCallbacks, g_currentScript->id, lua_callbackEntityEntity, lua_f);
    else
        SetScriptClosure(&g_playerEnteringMapCallbacks, g_currentScript->id, NULL, NULL);

    // need to retroactively run the callback on any players that might already be in the
    //        map prior to this callback being register

    if (f || lua_f) {
        n = NumEntitiesInTeam(ALL_PLAYERS_READYORNOT);
        for (i = 0; i < n; i++)
        {
            Entity* e = EntTeamInternal(ALL_PLAYERS_READYORNOT, i, NULL);

            if(f)
                ((PlayerEnteringMapFunc)f)(EntityNameFromEnt(e));
            else if(lua_f)
                lua_callback(lua_f, EntityNameFromEnt(e), NULL);
        }
    }
}

static int playerEnteringMap(void* func, Entity* player, void* v)
{
    ((PlayerEnteringMapFunc)func)(EntityNameFromEnt(player));
    return 0;
}
void ScriptPlayerEnteringMap(Entity* player)
{
    Vec3            none = {0};

    // remove any script controlled waypoints
    ClearAllWaypoints(EntityNameFromEnt(player));

    // call all enter callbacks
    ScriptClosureListCall(&g_playerEnteringMapCallbacks, playerEnteringMap, player, NULL);

    // Check the ScriptUI callbacks
    ScriptUIPlayerEnteredMap(player);
}


// *********************************************************************************
//  Entity Enters or Leaves Volume Script Hooks
// *********************************************************************************

ScriptClosureList g_playerLeavesVolumeCallbacks;

void OnPlayerLeavesVolume(PlayerLeavesVolumeFunc f)
{
    SetScriptClosure(&g_playerLeavesVolumeCallbacks, g_currentScript->id, f, NULL);
}
void luaCallback_OnPlayerLeavesVolume(STRING lua_f)
{
    SetScriptClosure(&g_playerLeavesVolumeCallbacks, g_currentScript->id, lua_f ? lua_callbackEntityString : NULL, lua_f);
}

static int playerLeavesVolume(void* func, Entity* player, void* v)
{
    ((PlayerLeavesVolumeFunc)func)(EntityNameFromEnt(player), v);
    return 0;
}

void ScriptPlayerLeavesVolume(Entity* player, char *name)
{
    ScriptClosureListCall(&g_playerLeavesVolumeCallbacks, playerLeavesVolume, player, name);
}

ScriptClosureList g_playerEntersVolumeCallbacks;

void OnPlayerEntersVolume(PlayerEntersVolumeFunc f)
{
    SetScriptClosure(&g_playerEntersVolumeCallbacks, g_currentScript->id, f, NULL);
}
void luaCallback_OnPlayerEntersVolume(STRING lua_f)
{
    SetScriptClosure(&g_playerEntersVolumeCallbacks, g_currentScript->id, lua_f ? lua_callbackEntityString : NULL, lua_f);
}

static int playerEntersVolume(void* func, Entity* player, void* v)
{
    ((PlayerEntersVolumeFunc)func)(EntityNameFromEnt(player), v);
    return 0;
}

void ScriptPlayerEntersVolume(Entity* player, char *name)
{
    ScriptClosureListCall(&g_playerEntersVolumeCallbacks, playerEntersVolume, player, name);
}

// *********************************************************************************
//  Entity Joins or Leaves Team Script Hooks
// *********************************************************************************

ScriptClosureList g_playerLeavesTeamCallbacks;

void OnPlayerLeavesTeam(PlayerLeavesTeamFunc f)
{
    SetScriptClosure(&g_playerLeavesTeamCallbacks, g_currentScript->id, f, NULL);
}
void luaCallback_OnPlayerLeavesTeam(STRING lua_f)
{
    SetScriptClosure(&g_playerLeavesTeamCallbacks, g_currentScript->id, lua_f ? lua_callbackEntityEntity : NULL, lua_f);
}
static int playerLeavesTeam(void* func, Entity* player, void* v)
{
    ((PlayerLeavesTeamFunc)func)(EntityNameFromEnt(player));
    return 0;
}
void ScriptPlayerLeavesTeam(Entity* player)
{
    ScriptClosureListCall(&g_playerLeavesTeamCallbacks, playerLeavesTeam, player, NULL);
}


ScriptClosureList g_playerJoinsTeamCallbacks;

void OnPlayerJoinsTeam(PlayerJoinsTeamFunc f)
{
    SetScriptClosure(&g_playerJoinsTeamCallbacks, g_currentScript->id, f, NULL);
}
void luaCallback_OnPlayerJoinsTeam(STRING lua_f)
{
    SetScriptClosure(&g_playerJoinsTeamCallbacks, g_currentScript->id, lua_f ? lua_callbackEntityEntity : NULL, lua_f);
}
static int playerJoinsTeam(void* func, Entity* player, void* v)
{
    ((PlayerJoinsTeamFunc)func)(EntityNameFromEnt(player));
    return 0;
}
void ScriptPlayerJoinsTeam(Entity* player)
{
    ScriptClosureListCall(&g_playerJoinsTeamCallbacks, playerJoinsTeam, player, NULL);
}


// *********************************************************************************
//  Entity Is Created Or Destroyed Script Hooks
// *********************************************************************************

ScriptClosureList g_entityFreedCallbacks;

void OnEntityFreed(EntityFreedFunc f)
{
    SetScriptClosure(&g_entityFreedCallbacks, g_currentScript->id, f, NULL);
}
void luaCallback_OnEntityFreed(STRING lua_f)
{
    SetScriptClosure(&g_entityFreedCallbacks, g_currentScript->id, lua_f ? lua_callbackEntityEntity : NULL, lua_f);
}
static int entityFreed(void* func, Entity* entity, void* v)
{
    ((EntityFreedFunc)func)(EntityNameFromEnt(entity));
    return 0;
}
void ScriptEntityFreed(Entity* entity)
{
    ScriptClosureListCall(&g_entityFreedCallbacks, entityFreed, entity, NULL);
    EntityScriptsStop(entity);
}


ScriptClosureList g_entityCreatedCallbacks;

void OnEntityCreated(EntityCreatedFunc f)
{
    SetScriptClosure(&g_entityCreatedCallbacks, g_currentScript->id, f, NULL);
}
void luaCallback_OnEntityCreated(STRING lua_f)
{
    SetScriptClosure(&g_entityCreatedCallbacks, g_currentScript->id, lua_f ? lua_callbackEntityEntity : NULL, lua_f);
}
static int entityCreated(void* func, Entity* entity, void* v)
{
    ((EntityCreatedFunc)func)(EntityNameFromEnt(entity));
    return 0;
}
void ScriptEntityCreated(Entity* entity)
{
    ScriptClosureListCall(&g_entityCreatedCallbacks, entityCreated, entity, NULL);
}

ScriptClosureList g_entityDefeatedCallbacks;

void OnEntityDefeated(EntityDefeatedFunc f)
{
    SetScriptClosure(&g_entityDefeatedCallbacks, g_currentScript->id, f, NULL);
}
void luaCallback_OnEntityDefeated(STRING lua_f)
{
    SetScriptClosure(&g_entityDefeatedCallbacks, g_currentScript->id, lua_f ? lua_callbackEntityEntity : NULL, lua_f);
}
static int entityDefeated(void* func, Entity* entity, Entity* victor)
{
    ((EntityDefeatedFunc)func)(EntityNameFromEnt(entity), EntityNameFromEnt(victor));
    return 0;
}
void ScriptEntityDefeated(Entity* entity, Entity* victor)
{
    ScriptClosureListCall(&g_entityDefeatedCallbacks, entityDefeated, entity, victor);
}

// *********************************************************************************
//  Reward hooks
// *********************************************************************************
ScriptClosureList g_teamRewardCallbacks;

void OnTeamRewarded(TeamRewardedFunc f)
{
    SetScriptClosure(&g_teamRewardCallbacks, g_currentScript->id, f, NULL);
}
void luaCallback_OnTeamRewarded(STRING lua_f)
{
    SetScriptClosure(&g_teamRewardCallbacks, g_currentScript->id, lua_f ? lua_callbackEntityPlayerTeam : NULL, lua_f);
}
static int teamRewarded(void* func, Entity* entity, Entity* victor)
{
    ((TeamRewardedFunc)func)(EntityNameFromEnt(entity), GetPlayerTeamFromPlayer(EntityNameFromEnt(victor)));
    return 0;
}
void ScriptTeamRewarded(Entity* entity, Entity* victor)
{
    if (entity && victor)
        ScriptClosureListCall(&g_teamRewardCallbacks, teamRewarded, entity, victor);
}


// *********************************************************************************
//  Player defeated hook
// *********************************************************************************

ScriptClosureList g_playerDefeatedByPlayerCallbacks;


void OnPlayerDefeatedByPlayer(PlayerDefeatedByPlayerFunc f)
{
    SetScriptClosure(&g_playerDefeatedByPlayerCallbacks, g_currentScript->id, f, NULL);
}
void luaCallback_OnPlayerDefeatedByPlayer(STRING lua_f)
{
    SetScriptClosure(&g_playerDefeatedByPlayerCallbacks, g_currentScript->id, lua_f ? lua_callbackEntityEntity : NULL, lua_f);
}

static int playerDefeatedByPlayer(void* func, Entity* entity, void* v)
{
    ((PlayerDefeatedByPlayerFunc)func)(EntityNameFromEnt(entity), EntityNameFromEnt(v));
    return 0;  
}

void ScriptPlayerDefeatedByPlayer(Entity* player, Entity* victor)
{
    ScriptClosureListCall(&g_playerDefeatedByPlayerCallbacks, playerDefeatedByPlayer, player, victor);
}


// *********************************************************************************
//  Player teleporting hook
//  Used in base raids to handle flag dropping when a player ports
// *********************************************************************************

ScriptClosureList g_playerTeleportingCallbacks;

void OnPlayerTeleporting(PlayerTeleportingFunc f)
{
    SetScriptClosure(&g_playerTeleportingCallbacks, g_currentScript->id, f, NULL);
}
void luaCallback_OnPlayerTeleporting(STRING lua_f)
{
    SetScriptClosure(&g_playerTeleportingCallbacks, g_currentScript->id, lua_f ? lua_callbackEntityEntity : NULL, lua_f);
}

static int playerTeleporting(void* func, Entity* player, void* v)
{
    return ((PlayerTeleportingFunc)func)(EntityNameFromEnt(player));
}

int ScriptPlayerTeleporting(Entity* player)
{
    // call all teleport callbacks
    return ScriptClosureListCall(&g_playerTeleportingCallbacks, playerTeleporting, player, NULL);
}

// *********************************************************************************
//  Player being teleported hook - used in mission teleport
//  Used when a script wants to teleport a player to determine where they actually get sent
// *********************************************************************************

ScriptClosureList g_playerMissionPortCallbacks;

void OnPlayerMissionPort(PlayerMissionPortFunc f)
{
    SetScriptClosure(&g_playerMissionPortCallbacks, g_currentScript->id, f, NULL);
}
void luaCallback_OnPlayerMissionPort(STRING lua_f)
{
    SetScriptClosure(&g_playerMissionPortCallbacks, g_currentScript->id, lua_f ? lua_callbackEntityEntity : NULL, lua_f);
}

static int playerMissionPort(void* func, Entity* player, void* v)
{
    return ((PlayerMissionPortFunc)func)(EntityNameFromEnt(player));
}

int ScriptPlayerMissionPort(Entity* player)
{
    return ScriptClosureListCall(&g_playerMissionPortCallbacks, playerMissionPort, player, NULL);
}

// *********************************************************************************
//  General message hook - used to allow anything to send a message to a script
// *********************************************************************************

ScriptClosureList g_scriptMessageCallbacks;

void OnScriptMessage(ScriptMessageFunc f)
{
    SetScriptClosure(&g_scriptMessageCallbacks, g_currentScript->id, f, NULL);
}
void luaCallback_OnScriptMessage(STRING lua_f)
{
    SetScriptClosure(&g_scriptMessageCallbacks, g_currentScript->id, lua_f ? lua_callback : NULL, lua_f);
}

static int scriptMessage(void* func, const char *message, void* v)
{
    return ((ScriptMessageFunc)func)(message);
}

int ScriptSendScriptMessage(const char* message)
{
    return ScriptClosureListCall(&g_scriptMessageCallbacks, scriptMessage, (void*)message, NULL);
}

// *********************************************************************************
//  Player interact with npc hook
// *********************************************************************************

void OnPlayerInteractWithNPCOrGlowie(PlayerInteractWithNPCOrGlowieFunc f, TEAM target)
{
    OnPlayerInteractWithNPCOrGlowieEx(f, NULL, target);
}

void luaCallback_OnPlayerInteractWithNPCOrGlowie(STRING lua_f, TEAM target)
{
    OnPlayerInteractWithNPCOrGlowieEx(lua_f ? lua_callbackEntityEntity : NULL, lua_f, target);
}

void OnPlayerInteractWithNPCOrGlowieEx(void* f, STRING lua_f, TEAM target)
{
    int i, n;

    n = NumEntitiesInTeam(target);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(target, i, NULL);
        if (ENTTYPE_BY_ID(e->owner) != ENTTYPE_NPC)
        {
            ScriptError("Script %s tried to attach an interaction to a non-npc", g_currentScript->function->name);
            continue;
        }
        SetScriptClosure(&e->onClickHandler, g_currentScript->id, f, lua_f);
    }
}

int npcClicked(void* func, Entity* player, Entity* npc)
{
    return ((PlayerInteractWithNPCOrGlowieFunc)func)(EntityNameFromEnt(player), EntityNameFromEnt(npc));
}

int ScriptNpcClick(Entity* player, Entity* npc)
{
    if (npc) // now allowing contacts to work
    {
        return ScriptClosureListCall(&npc->onClickHandler, npcClicked, player, npc);
    } else {
        return 0;
    }
}

// *********************************************************************************
//  Script controlled contacts hook
// *********************************************************************************

typedef struct {
    Entity* contact;
    int link;
    ContactResponse* response;
} ScriptContactInteraction;

void OnScriptedContactInteract(ScriptedContactInteractFunc f, TEAM target)
{
    int i, n;
    Contact* contact;

    n = NumEntitiesInTeam(target);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(target, i, NULL);
        if (ENTTYPE_BY_ID(e->owner) != ENTTYPE_NPC && ENTTYPE_BY_ID(e->owner) != ENTTYPE_CRITTER)
        {
            ScriptError("Script %s tried to attach an interaction to a non-npc/non-critter", g_currentScript->function->name);
            continue;
        }

        if (e->scriptedContactHandler)
        {
            ScriptError("Script %s tried to attach an interaction to an already active NPC", g_currentScript->function->name);
            continue;
        }

        if (!e->persistentNPCInfo)
        {
            e->persistentNPCInfo = calloc(sizeof(PNPCInfo), 1);
        }
        e->persistentNPCInfo->pnpcdef = 0;
        e->persistentNPCInfo->contact = ContactGetHandle("contacts/scriptcontacts/scriptcontact1.contact");
         if ((contact = GetContact(e->persistentNPCInfo->contact)) && contact->pnpcParent)
         {
            e->bitfieldVisionPhasesDefault = contact->pnpcParent->bitfieldVisionPhases;
             e->exclusiveVisionPhaseDefault = contact->pnpcParent->exclusiveVisionPhase;
         }
        // don't initialize to default values if it was created by a spawn
        // it was already initialized in SpawnEncounterActor().
        else if (!e->encounterInfo || e->encounterInfo->actorID == -1) 
        {
            e->bitfieldVisionPhasesDefault = 1; // default to be in prime phase
            e->exclusiveVisionPhaseDefault = 0;
        }
        e->contactOrPnpc = true;
        e->status_effects_update = true;


        SetScriptClosure(&e->scriptedContactHandler, g_currentScript->id, f, NULL);
    }
}

void OnScriptedContactInteractFromContactDef(STRING contactDef, TEAM target)
{
    int i, n;
    Contact *contact;

    n = NumEntitiesInTeam(target);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(target, i, NULL);
        if (ENTTYPE_BY_ID(e->owner) != ENTTYPE_NPC)
        {
            ScriptError("Script %s tried to attach an interaction to a non-npc", g_currentScript->function->name);
            continue;
        }

        if (e->persistentNPCInfo || e->scriptedContactHandler != NULL)
        {
            ScriptError("Script %s tried to attach an interaction to an already active NPC", g_currentScript->function->name);
            continue;
        }

        e->persistentNPCInfo = calloc(sizeof(PNPCInfo), 1);
        e->persistentNPCInfo->pnpcdef = 0;
        e->persistentNPCInfo->contact = ContactGetHandle(contactDef);
        if ((contact = GetContact(e->persistentNPCInfo->contact)) && contact->pnpcParent)
        {
            e->bitfieldVisionPhasesDefault = contact->pnpcParent->bitfieldVisionPhases;
            e->exclusiveVisionPhaseDefault = contact->pnpcParent->exclusiveVisionPhase;
        }
        else
        {
            e->bitfieldVisionPhasesDefault = 1; // default to be in prime phase
            e->exclusiveVisionPhaseDefault = 0;
        }
        e->contactOrPnpc = true;

//        SetScriptClosure(&e->scriptedContactHandler, g_currentScript->id, f);
    }
}

static int scriptContactClicked(void* func, Entity* player, ScriptContactInteraction* pInteraction)
{
    return ((ScriptedContactInteractFunc) func)(EntityNameFromEnt(player), 
                EntityNameFromEnt(pInteraction->contact),
                pInteraction->link, pInteraction->response);
}

int ScriptScriptedContactClick(Entity* player, int link, ContactResponse* response)
{
    ScriptContactInteraction *pInteraction = 0;
    int retval = 0;

    if(!player->storyInfo->interactEntityRef || !erGetEnt(player->storyInfo->interactEntityRef))    //fix to handle bad data more gracefully.
        return 0;

    pInteraction = calloc(sizeof(ScriptContactInteraction), 1);

    pInteraction->contact = erGetEnt(player->storyInfo->interactEntityRef);
    pInteraction->link = link;
    pInteraction->response = response;

    retval= ScriptClosureListCall(&pInteraction->contact->scriptedContactHandler, scriptContactClicked, player, pInteraction);

    free(pInteraction);

    return retval;
}

