/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include "varutils.h"
#include <utilitieslib/network/netio.h>
#include "player/player.h"
#include "entity/entVarUpdate.h"
#include "entity/entStrings.h" // for mallocEntStrings
#include "gameData/BodyPart.h"
#include <utilitieslib/utils/error.h>
#include "entity/character_base.h"
#include "entity/costume.h"
#include "entity/power_customization.h"
#include "entity/PowerInfo.h"
#include <utilitieslib/components/earray.h>
#include "gameComm/wdwbase.h"
#include "entity/entStrings.h"
#include "entity/entPlayer.h"
#include "entity/teamup.h"
#include "entity/friendCommon.h"
#include "gameComm/trayCommon.h"
#include "entity/entity.h"
#include "entity/Supergroup.h"
#include "seq/seq.h"

#ifdef SERVER
    #include "entity/entGameActions.h"
    #include "container/containerEmail.h"
    #include "storyarc/storyarcinterface.h"
    #include "gameComm/trading.h"
    #include "entity/keybinds.h"
    #include "player/pl_stats.h"
    #include "dbcomm/dbcomm.h"
    #include "gameData/arenastruct.h"
    #include "gameComm/pet.h"
    #include "entity/mapHistory.h"
    #include "gameSys/Turnstile.h"
    #include "account/AccountInventory.h"
    #include "storyarc/contactdef.h"
#else
    #include "entity/attrib_net.h"
    #include "UI/uiKeybind.h"
    #include "UI/uiBuff.h"
    #include "UI/uiTray.h"
#endif

MP_DEFINE(EntPlayer);
MP_DEFINE(Character);
MP_DEFINE(Friend);
#ifdef SERVER
MP_DEFINE(WdwBase);
MP_DEFINE(ServerKeyBind);
#define DEFAULT_PLAYER_MEMPOOL_SIZE (db_state.is_static_map?16:4)
#else
#define DEFAULT_PLAYER_MEMPOOL_SIZE 4
#endif

//
//
void playerVarAlloc(Entity *e, EntType ent_type)
{
    int i, entID = e->owner;

    e->name[0] = '\0';
    e->strings = 0;
    e->ownedCostume = NULL;
    e->ownedPowerCustomizationList = NULL;
    e->customNPC_costume = NULL;
    e->CVG_overrideName = NULL;
#ifdef SERVER
    e->encounterInfo = 0;
    e->persistentNPCInfo = 0;
    e->bitfieldVisionPhasesDefault = 1; // default to be in prime phase

    if (ent_type == ENTTYPE_DOOR)
    {
        e->exclusiveVisionPhaseDefault = 1; // doors default to the All phase
    }
    else
    {
        e->exclusiveVisionPhaseDefault = 0;
    }

    e->storyInfo = 0;
    e->who_damaged_me = 0;
    e->birthtime = dbSecondsSince2000();
#endif

    // hmm.. can we keep the new rational entity world here?
    switch (ent_type)
    {
    case ENTTYPE_PLAYER:
        MP_CREATE(EntPlayer, DEFAULT_PLAYER_MEMPOOL_SIZE);
        e->pl = MP_ALLOC(EntPlayer);

        e->pl->winLocs = 0;
        e->pl->keybinds = 0;

        if( !e->pl->tray )
        {
            e->pl->tray = tray_Create();
        }

        if ( !e->superstat )
            e->superstat = createSupergroupStats();

        for( i = 0; i < ARRAY_SIZE(e->pl->costume); i++ )
        {
            e->pl->costume[i] = costume_create(GetBodyPartCount());
            e->pl->powerCustomizationLists[i] = StructAllocRaw(sizeof(PowerCustomizationList));
        }
        e->pl->superCostume = costume_create(GetBodyPartCount());

        entSetMutableCostumeUnowned( e, e->pl->costume[0] ); // e->costume = costume_as_const(e->pl->costume[0]);
        e->powerCust = e->pl->powerCustomizationLists[0];

        MP_CREATE_EX(Friend, DEFAULT_PLAYER_MEMPOOL_SIZE, MAX_FRIENDS * sizeof(Friend));
        e->friends = MP_ALLOC(Friend);

        e->strings = createEntStrings();
        initEntStrings( e->strings );

        eaCreate(&e->pl->rewardTokens);
        eaCreate(&e->pl->activePlayerRewardTokens);

#ifdef SERVER
        e->pl->desiredTeamNumber = -2;

        RandomFameMoved(e);
        eaCreate(&e->pl->arenaEvents);
        eaCreate(&e->pl->petNames);
        eaCreate(&e->pl->mapHistory);

        e->pl->currentAccessibleContactIndex = -1;
#else
        e->taskforce = createTaskForce();
#endif

        break;
    case ENTTYPE_NOT_SPECIFIED:
    case ENTTYPE_NPC:
    case ENTTYPE_CAR:
    case ENTTYPE_MOBILEGEOMETRY:
    case ENTTYPE_MISSION_ITEM:
    case ENTTYPE_DOOR:           // Just a simple door that players may interact with.
    case ENTTYPE_HERO:           // AI controlled Hero
    case ENTTYPE_CRITTER:
        // Nothing special for these
        break;
    }

    if (ent_type==ENTTYPE_PLAYER ||
        ent_type==ENTTYPE_CRITTER ||
        ent_type==ENTTYPE_HERO)
    {
        // Need a pchar
        // Allocate the character's attributes for combat and powers.
        assert(e->pchar==NULL);
        MP_CREATE(Character, 16);
        e->pchar = MP_ALLOC(Character);
#ifdef SERVER
        e->pcharLast = MP_ALLOC(Character);
#elif CLIENT
        assert(e->pkt_id_fullAttrDef==0);
        assert(e->pkt_id_basicAttrDef==0);
        eaCreate(&e->ppowBuffs);
#endif
    }

#if SERVER

    eaCreate(&e->who_damaged_me);

    if( ent_type == ENTTYPE_PLAYER )
    {
        int i;

        MP_CREATE_EX(WdwBase, DEFAULT_PLAYER_MEMPOOL_SIZE, sizeof(WdwBase)*MAX_WINDOW_COUNT);
        e->pl->winLocs= MP_ALLOC(WdwBase);

        // Copy the defaults in.
        for(i=0; i<MAX_WINDOW_COUNT; i++)
        {
            memset(&e->pl->winLocs[i], 0, sizeof(WdwBase));
        }

        MP_CREATE_EX(ServerKeyBind, DEFAULT_PLAYER_MEMPOOL_SIZE, sizeof(ServerKeyBind) * BIND_MAX);
        e->pl->keybinds = MP_ALLOC(ServerKeyBind);

        // init story arc structure
        e->storyInfo = storyInfoCreate();

        e->email_info = emailAllocInfo();

        stat_Init(e->pl);
    }

    // TODO: These really should be in a substructure.
    eaCreate(&e->powerDefChange);
    eaCreate(&e->activeStatusChange);
    eaCreate(&e->enabledStatusChange);
    eaCreate(&e->usageStatusChange);
    eaCreate(&e->rechargeStatusChange);
    eaiCreate(&e->inspirationStatusChange);
    eaiCreate(&e->boostStatusChange);
#elif CLIENT
    e->powerInfo = powerInfo_Create();
#endif

}


void playerVarFree( Entity *e )
{
    int i;
#if SERVER
    if( e->who_damaged_me )
    {
        eaDestroyEx(&e->who_damaged_me, damageTrackerDestroy);
    }

    if (e->storyInfo)
    {
        storyInfoDestroy(e->storyInfo);
        e->storyInfo = NULL;
    }
    if (e->email_info) {
        emailFreeInfo(e->email_info);
        e->email_info = NULL;
    }
#endif

    if(e->pchar != NULL)
    {
        character_Destroy(e->pchar,NULL);
        MP_FREE(Character,e->pchar);
    }

    // new nice structures
    if (e->pl)
    {
        eaDestroyEx(&e->pl->rewardTokens, rewardtoken_Destroy); //these strings don't need to be freed anymore, they are from
        eaDestroyEx(&e->pl->activePlayerRewardTokens, rewardtoken_Destroy);

#if SERVER
        stat_Free(e->pl);

        MP_FREE(WdwBase, e->pl->winLocs);
        MP_FREE(ServerKeyBind, e->pl->keybinds );

        if( e->pl->trade )
        {
            trade_cancel( e, 0 );
        }
        eaDestroyEx(&e->pl->arenaEvents, ArenaRefDestroy);
        eaDestroyEx(&e->pl->petNames, PetNameDestroy);
        eaDestroyEx(&e->pl->mapHistory, mapHistoryDestroy);
        eaDestroyEx(&e->pl->ppCertificationHistory, certificationRecord_Destroy);
        eaDestroyEx(&e->pl->completed_orders, NULL);
        eaDestroyEx(&e->pl->pending_orders, NULL);
#else
        if (e->pl->keybinds) {
            for( i = 0; i < BIND_MAX; i++ )
            {
                if (e->pl->keybinds[i].command[0]) {
                    SAFE_FREE(e->pl->keybinds[i].command[0]);
                }
            }
            SAFE_FREE(e->pl->keybinds);
        }
#endif
        // If a player entity is being destroyed, kill the costume structure also
        // because the entity owns it.
        // Otherwise, assume that we're dealing with some sort of NPC.  In that case,
        // do not destroy the costume because they are shared between NPCs.
        for( i = 0; i < MAX_COSTUMES; i++ )
        {
            if(e->pl->costume[i])
            {
                //printf("destroying costume: %d\n", e->owner);
                costume_destroy(e->pl->costume[i]);
                e->pl->costume[i] = NULL;
            }
            if (e->pl->powerCustomizationLists[i])
            {
                powerCustList_destroy(e->pl->powerCustomizationLists[i]);
                e->pl->powerCustomizationLists[i] = NULL;
            }
        }

        if(e->pl->superCostume)
        {
            costume_destroy(e->pl->superCostume);
            e->pl->superCostume = NULL;
        }
        if(e->pl->pendingCostume)
        {
            costume_destroy(e->pl->pendingCostume);
            e->pl->pendingCostume = NULL;
        }

        eaiDestroy(&e->pl->baseRaids);

        AccountInventorySet_Release( &e->pl->account_inventory );

        if (e->pl->tray)
        {
            tray_Destroy(e->pl->tray);
            e->pl->tray = NULL;
#ifndef SERVER
#ifndef TEST_CLIENT
            if (e == playerPtr())
                clearServerTrayPriorities();
#endif
#endif
        }

        MP_FREE(EntPlayer, e->pl);
    }

    if (e->ownedCostume)
    {
        costume_destroy(e->ownedCostume);
        e->ownedCostume = NULL;        
    }
    if (e->ownedPowerCustomizationList)
    {
        powerCustList_destroy(e->ownedPowerCustomizationList);
        e->ownedPowerCustomizationList = NULL;
    }
    if (e->customNPC_costume)
    {
        StructDestroy(ParseCustomNPCCostume,e->customNPC_costume);
        e->customNPC_costume = NULL;
    }
    if (e->CVG_overrideName)
    {
        StructFreeString(e->CVG_overrideName);
        e->CVG_overrideName = NULL;
    }
    for (i=0; i<e->friendCount; i++)
        friendDestroy(&e->friends[i]);
    MP_FREE(Friend, e->friends );
    destroyEntStrings(e->strings);
    e->strings = NULL;

    destroySupergroupStats( e->superstat );
    e->superstat = NULL;

    if(e->sgStats)
    {
        eaDestroyEx(&e->sgStats, destroySupergroupStats);
    }

#if SERVER
    // This block frees teamup_activePlayer no matter what teamupTimer_activePlayer says.
    // This is okay because this function is only called when the whole Entity is being freed,
    // so the timer isn't going to be around much longer to complain about it.
    if (e->teamup_activePlayer)
    {
        if (e->teamup_activePlayer != e->teamup)
        {
            destroyTeamup(e->teamup_activePlayer);
        }
        e->teamup_activePlayer = NULL;
    }
#endif
    if (e->teamup) {
        destroyTeamup(e->teamup);
        e->teamup = NULL;
    }
    if (e->taskforce) {
        destroyTaskForce(e->taskforce);
        e->taskforce = NULL;
    }
    if (e->supergroup) {
        destroySupergroup(e->supergroup);
        e->supergroup = NULL;
    }

    if (e->league) {
        destroyLeague(e->league);
        e->league = NULL;
    }

    if (e->seq) seqUpdateGroupTrackers(e->seq,e);

    SAFE_FREE(e->netfx_list);

    SAFE_FREE(e->costumeWorldGroup);
    SAFE_FREE(e->aiAnimList);

#ifdef SERVER
    if(e->pcharLast != NULL)
    {
        character_Destroy(e->pcharLast,NULL);
        MP_FREE(Character,e->pcharLast);
    }
    // TODO: These really should be in a substructure.
    if(e->powerDefChange)
    {
        eaDestroyEx(&e->powerDefChange, powerRef_Destroy);
    }

    if(e->activeStatusChange)
    {
        eaDestroyEx(&e->activeStatusChange, powerRef_Destroy);
    }

    if(e->enabledStatusChange)
    {
        eaDestroyEx(&e->enabledStatusChange, powerRef_Destroy);
    }

    if(e->usageStatusChange)
    {
        eaDestroyEx(&e->usageStatusChange, powerRef_Destroy);
    }

    if(e->rechargeStatusChange)
    {
        eaDestroyEx(&e->rechargeStatusChange, powerRef_Destroy);
    }

    if(e->inspirationStatusChange)
        eaiDestroy(&e->inspirationStatusChange);

    if(e->boostStatusChange)
        eaiDestroy(&e->boostStatusChange);

    SAFE_FREE(e->favoriteWeaponAnimList);
    SAFE_FREE(e->persistentNPCInfo);

#elif CLIENT
    if(e->powerInfo)
    {
        powerInfo_Destroy(e->powerInfo);
        e->powerInfo = NULL;
    }

    if(e->ppowBuffs)
    {
        eaDestroyEx(&e->ppowBuffs, freePowerBuff);
    }

    sdFreePktIds(SendFullCharacter, &e->pkt_id_fullAttrDef);
    sdFreePktIds(SendBasicCharacter, &e->pkt_id_basicAttrDef);

    SAFE_FREE(e->pchVillGroupName);
    SAFE_FREE(e->pchDisplayClassNameOverride);
#endif

}

#if SERVER
// Need external access to the character memory pool for respecs
//
Character *character_Allocate()
{
    Character *character;
        
    MP_CREATE(Character, 16);
    character = MP_ALLOC(Character);
        
    return character;
}

void character_Free(Character *character)
{
    MP_FREE(Character, character);
}
#endif
