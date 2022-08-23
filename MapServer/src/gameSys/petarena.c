/*\
 *
 *    arenamap.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Code to run an instanced arena map.
 *
 */

#include "arenamapserver.h"
#include "arenamap.h"
#include "dbcomm/dbcomm.h"
#include "comm_game.h"
#include "svr/svr_base.h"
#include "svr/svr_player.h"
#include <utilitieslib/utils/error.h>
#include "stdio.h"
#include <utilitieslib/components/earray.h>
#include "entity/character_base.h"
#include "entity/character_combat.h"
#include "entity/character_tick.h"
#include "entity/entGameActions.h"
#include "entity/entserver.h"
#include "entity/entPlayer.h"
#include "container/team.h"
#include "dbcomm/dbdoor.h"
#include "entity/powers.h"
#include "entity/PowerInfo.h"
#include "entity/character_animfx.h"
#include "player/playerState.h"
#include <utilitieslib/utils/timing.h>
#include "player/parseClientInput.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "beacon/beaconPrivate.h"
#include "gridcoll/gridcoll.h"
#include "entity/character_target.h"
#include "TeamReward.h"
#include "entity/character_pet.h"
#include "gameComm/VillainDef.h"
#include "storyarc/storyarcutil.h"
#include "language/langServerUtil.h"
#include "player/badges_server.h"
#include "ai/entaiprivate.h"
#include "entity/powers.h"
#include <utilitieslib/components/HashFunctions.h>

#define MAX_PETS 20 //Corresponds to the number of "PetBattlePetX" in RewardTokens.reward, if you change, remember to rebuild templates 
#define PETREWARDSTRING "PetBattlePet"
#define DEFAULT_MAX_PET_ARMY_POINTS 1000


typedef struct PetBattleCreatureInfo
{
    char * name;                //Name is unique, and used to create tokenID for reward token
    char * VillainDefName;
    int spawnLevel;
    int tokenID;                //Generated 
    char * badgeRequired;
    int cost;
    char * baseBehavior;
} PetBattleCreatureInfo;

typedef struct PetBattleCreatureInfoList
{
    PetBattleCreatureInfo ** infos;
    int maxPoints;
} PetBattleCreatureInfoList;

TokenizerParseInfo ParsePetBattleCreatureInfo[] =
{
    { "CreatureInfo",            TOK_START,                        0                                                    }, // hack, for reloading
    { "Name",                    TOK_STRUCTPARAM | TOK_STRING( PetBattleCreatureInfo, name, 0            )    },
    { "VillainDef",                TOK_STRING( PetBattleCreatureInfo, VillainDefName, 0    )    },
    { "SpawnLevel",                TOK_INT( PetBattleCreatureInfo, spawnLevel, 0        )    },
    { "BadgeRequired",            TOK_STRING( PetBattleCreatureInfo, badgeRequired, 0    )    },
    { "BaseBehavior",            TOK_STRING( PetBattleCreatureInfo, baseBehavior, 0    )    },
    { "Cost",                    TOK_INT( PetBattleCreatureInfo, cost, 0            )    },
    {    "{",                    TOK_START,                        0 },
    {    "}",                    TOK_END,                            0 },
    { "", 0, 0 }

};


TokenizerParseInfo ParsePetBattleCreatureInfoList[] = 
{
    { "MaxPoints",        TOK_INT( PetBattleCreatureInfoList, maxPoints, DEFAULT_MAX_PET_ARMY_POINTS )        },
    { "CreatureInfo",    TOK_STRUCT( PetBattleCreatureInfoList, infos, ParsePetBattleCreatureInfo ) },
    { "", 0, 0 }
};

PetBattleCreatureInfoList g_petBattleCreatureInfoList;
StashTable    petBattleInfo_ht = 0;


void LoadPetBattleCreatureInfos()
{
    PetBattleCreatureInfo * info;
    int i;
    int infoCount;
    char buf[12];

    if ( petBattleInfo_ht ) stashTableDestroy(petBattleInfo_ht);
    petBattleInfo_ht = stashTableCreateWithStringKeys(300,  StashDeepCopyKeys | StashCaseSensitive );
    
    eaDestroy( &g_petBattleCreatureInfoList.infos ); //TO DO : a smarter version of destroy, and load files shared
    ParserLoadFiles(0, "defs/petBattleCreatureInfo.def", "petBattleCreatureInfo.bin", PARSER_SERVERONLY, ParsePetBattleCreatureInfoList, &g_petBattleCreatureInfoList, NULL, NULL, NULL);
    
    infoCount = eaSize( &g_petBattleCreatureInfoList.infos );

    for( i = 0 ; i < infoCount ; i++ )
    {
        int j;
        info = g_petBattleCreatureInfoList.infos[i];
        info->tokenID = hashString( info->name, true ); //Temp way to assign IDs, since I don't have these guys in the db
        assert( info->tokenID ); //Couls this ever hash to 0?
        //TO DO : case insensitive

        stashAddPointer( petBattleInfo_ht, itoa(info->tokenID, buf, 10), info, false); 

        //Debug 
        for( j = 0 ; j < i ; j++ )
        {
            if( g_petBattleCreatureInfoList.infos[j]->tokenID == info->tokenID )
            {
                assert( 0 == stricmp(g_petBattleCreatureInfoList.infos[j]->name, info->name ) );  //Hash Collision check, this is just temp until we decide whether to put this in the db, so don't worry about it's weirdness
                Errorf( "Multiple Entries in petBattleCreatureInfo.def for %s ", info->name );
            }
        }
        //End Debug 
    }
}


PetBattleCreatureInfo * getPetFromPetList( int tokenID )
{
    char buf[12];
    return stashFindPointerReturnPointer( petBattleInfo_ht, itoa( tokenID, buf, 10 ) );
}


void ArenaCreateArenaPets( Entity * e )
{
    RewardToken * pettoken;
    int petcount = MAX_PETS; 
    PetBattleCreatureInfo * info;
    int i;
    
    for( i = 0 ; i < petcount; i++ )
    {
        char buf[100];
        Entity * pPet;

        sprintf( buf, "%s%d", PETREWARDSTRING, i+1 );
        pettoken = getRewardToken( e, buf ); //Using reward tokens might be a little weak, but it works right now.

        if( !pettoken || !pettoken->val )
            continue;

        info = getPetFromPetList( pettoken->val );
        if( !info ) //Shouldn't ever ask for a pet that doesn't exist
            continue;

        // this gets overridden in character_createArenaPet right away anyway
//         if( info->baseBehavior )
//             pl = info->baseBehavior;

        pPet =  character_CreateArenaPet( e, info->VillainDefName, info->spawnLevel );

        ArenaGiveTempPower( pPet, "Pet_Arena_HP");
    }
}


void sendArenaManagePets( Entity * e )
{
    PetBattleCreatureInfo * info;
    int i;

    START_PACKET(pak,e,SERVER_ARENA_MANAGEPETS);

    ///////// Send all the available pets. (TO DO, the client can tell you if it already has this)
    {
        PetBattleCreatureInfo ** availablePets = 0;
        int infoCount;

        infoCount = eaSize( &g_petBattleCreatureInfoList.infos );
        for( i = 0 ; i < infoCount ; i++ )
        {
            info = g_petBattleCreatureInfoList.infos[i];
            if( !info->badgeRequired || 0 == stricmp(info->badgeRequired, "NONE") || 
                badge_OwnsBadge( e, info->badgeRequired ) )
            {
                eaPush( &availablePets, info );
            }
        }

        pktSendBitsPack(pak,1, eaSize(&availablePets) ); //I'm sending this many infos
        for( i = 0 ; i < eaSize(&availablePets) ; i++ )
        {
            const VillainDef* def;
            char name[128] = "MISSING NAME";
            info = availablePets[i];

            def = villainFindByName(info->VillainDefName);
            if( def ) 
            {
                const VillainLevelDef * levelDef = GetVillainLevelDef( def, info->spawnLevel );
                if( levelDef->level != info->spawnLevel )
                {
                    ; //Error or figure out a way to force it.
                }
                strcpy(name, localizedPrintf(e, levelDef->displayNames[0]));
            }

            pktSendString( pak, name );
            pktSendBitsPack( pak, 1, info->cost );
            pktSendBitsPack( pak, 1, info->tokenID );
        }
        eaDestroy( &availablePets );
    }

    ///////// Then Send all pets currently in your army (Do this every time)
    {
        PetBattleCreatureInfo ** currentPets = 0;
        
        for( i = 0 ; i < MAX_PETS; i++ )
        {
            RewardToken * pettoken;
            char buf[100];

            sprintf( buf, "%s%d", PETREWARDSTRING, i+1 );
            pettoken = getRewardToken( e, buf ); //Using reward tokens might be a little weak, but it works right now.

            if( !pettoken || !pettoken->val )
                continue;

            info = getPetFromPetList( pettoken->val );
            if( !info ) //List could change, maybe notify player they've lost their pet?
                continue;
            
            eaPush( &currentPets, info );
        }

        pktSendBitsPack(pak,1, eaSize(&currentPets) ); //I'm sending this many infos
        for( i = 0 ; i < eaSize(&currentPets) ; i++ )
        {
            info = currentPets[i];
            pktSendString( pak, info->VillainDefName );
            pktSendBitsPack( pak, 1, info->cost );
            pktSendBitsPack( pak, 1, info->tokenID );
        }

        eaDestroy( &currentPets );
    }

    END_PACKET;
}

//These next two functions are really general pet features not specific to pet arena
void sendUpdatePetState( Entity * pet )
{
    Entity * owner = erGetEnt( pet->erOwner );

    if( owner )
    {
        START_PACKET( pak, owner, SERVER_UPDATE_PET_STATE );

        pktSendBitsPack( pak, 1, pet->owner );
        pktSendBitsPack( pak, 1, pet->specialPowerState );    

        END_PACKET
    }
}


void updatePetState( Entity * e ) 
{
    char * specialPowerDisplayName = 0;
    int oldState = 0;

    if( e->villainDef && e->villainDef->specialPetPower )
    {
        //Init -- to do, add on failure don't try to reinit
        //TO DO move this logic out of entServer AIPowerInfo is private to the entAI and all this ought ot be moved into the private AI area
        if( !e->specialPowerDisplayName && e->villainDef && e->villainDef->specialPetPower && e->villainDef->specialPetPower[0] )
        {
            AIPowerInfo * powerInfo = aiFindAIPowerInfoByPowerName( e, e->villainDef->specialPetPower );
            if( powerInfo && powerInfo->power && powerInfo->power->ppowBase )
            {
                e->specialPowerDisplayName = strdup( powerInfo->power->ppowBase->pchDisplayName );
            }
        }
        //End logic to move

        oldState = e->specialPowerState;

        //Set state of special power state, and if it changed, send a packet to the player
        //e->specialPowerState = 1; //TO DO get specialPowerState
        {
            AIPowerInfo * powerInfo = aiFindAIPowerInfoByPowerName( e, e->villainDef->specialPetPower );
            if( !powerInfo )
                e->specialPowerState = 0;
            else if( powerInfo->power->fTimer > 0.25f ) //TO DO add endurance check
                e->specialPowerState = 3; //UNAVAILABLE : RECHARGING / NOT ENOUGH ENDURANCE
            else if( powerInfo->power->bActive || (ENTAI(e) && ENTAI(e)->wasToldToUseSpecialPetPower) )
                e->specialPowerState = 2; //USING / ATTEMPTING TO USE
            else
                e->specialPowerState = 1; //AVAILABLE
        }

        //Update owner if the state has changed
        if( oldState != e->specialPowerState )
            sendUpdatePetState( e );
    }
}

void PetArenaCommitPetChanges( Entity * e,  Packet* pak )
{
    int i;
    PetBattleCreatureInfo ** currentPets = 0;

    //Get new info from packet
    {
        int newPetCount;
        int id;
        PetBattleCreatureInfo * info;

        newPetCount = pktGetBitsPack(pak,1); 
        for( i = 0 ; i < newPetCount ; i++ )
        {
            id = pktGetBitsPack(pak,1); 
            info = getPetFromPetList( id );
            //TO DO Verify that the client didn't lie about what he can make, or send garbage
            if( info )
                eaPush( &currentPets, info );
        }
    }

    //Update RewardTokens based on new info
    //TO DO : If db performance matters, maybe try to minimize redundant adding and changing RewardTokens
    for( i = 0 ; i < MAX_PETS; i++ )
    {
        RewardToken * pettoken;
        char buf[100];

        sprintf( buf, "%s%d", PETREWARDSTRING, i+1 );
        pettoken = getRewardToken( e, buf ); //Using reward tokens might be a little weak, but it works right now.

        //Assign all the pets to rewardTokens
        if( i < eaSize( &currentPets ) )
        {
            if( !pettoken )
                pettoken = giveRewardToken( e, buf );
            pettoken->val = currentPets[i]->tokenID;
        }
        else //And Clean the rest out
        {
            if( pettoken )
                pettoken->val = 0;
        }
    }

    
    eaDestroy( &currentPets );

    //Ack the info to the client
    sendArenaManagePets( e ); 
}

int OnPetArenaMap()
{
    if (!OnArenaMap())
        return false;
    return g_arenamap_event->use_gladiators;
}