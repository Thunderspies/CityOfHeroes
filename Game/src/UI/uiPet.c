
#include "gameComm/wdwbase.h"

#include "entity/entity.h"
#include <utilitieslib/components/earray.h>
#include "entity/entclient.h"
#include "entity/character_base.h"
#include "entity/powers.h"
#include "graphics/textureatlas.h"
#include "win/win_init.h"
#include <utilitieslib/utils/mathutil.h>
#include "clientcomm/clientcomm.h"

#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_base.h"
#include "gameComm/petCommon.h"
#include "player/player.h"
#include "arena/arenagame.h"

#include "UI/uiNet.h"
#include "UI/uiPet.h"
#include "UI/uiUtil.h"
#include "UI/uiTray.h"
#include "UI/uiChat.h"
#include "UI/uiGame.h"
#include "UI/uiTarget.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiCursor.h"
#include "UI/uiInput.h"
#include "UI/uiStatus.h"
#include "UI/uiGroupWindow.h"
#include "UI/uiToolTip.h" 
#include "UI/uiWindows.h"
#include "UI/uiWindows_init.h"
#include "UI/uiContextMenu.h"
#include "gameComm/trayCommon.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiOptions.h"
#include "gameComm/itemselect.h"
#include "entity/teamup.h"
#include "seq/seq.h"
#include "filter/validate_name.h"
#include "UI/uiDialog.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiInfo.h"
#include "UI/uiTray.h"
#include "UI/uiGift.h"
#include "UI/uiPowerInventory.h"
#include "UI/uiBuff.h"
#include "UI/uiInspiration.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/character_target.h"

//-----------------------------------------------------------------------------------------------
// Statics and Defs /////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------

static ToolTipParent GroupTipParent = {0};

typedef struct PetCommandInfo
{
    char * displayName;
    char * cmDisplayName;
    char * iconName;
}PetCommandInfo;

PetCommandInfo petStances[kPetStance_Count] = 
{
    { "", "", "" }, // kPetStance_Unknown
    { "PassiveCommandParameter",    "CMSetPetPassiveString",    "PetCommand_Stance_Passive.tga"        },
    { "DefensiveCommandParameter",    "CMSetPetDefensiveString",    "PetCommand_Stance_Defensive.tga"    },
    { "AggressiveCommandParameter",    "CMSetPetAggressiveString",    "PetCommand_Stance_Aggressive.tga"    },
};

STATIC_ASSERT(kPetStance_Count == 4);

PetCommandInfo petActions[kPetAction_Count] = 
{
    { "AttackCommandParameter",        "CMPetAttackMyTargetString",    "PetCommand_Action_Attack.tga",        },    // Attack
    { "GotoCommandParameter",        "CMPetGoToString",                "PetCommand_Action_Goto.tga",        },    // Go to
    { "FollowCommandParameter",        "CMPetFollowMeString",            "PetCommand_Action_Follow.tga",        },  // Follow Me
    { "StayCommandParameter",        "CMPetStayString",                "PetCommand_Action_Stay.tga",        },    // Stay
    { "SpecialCommandParameter",    "CMPetUseSpecialString",        "PetCommand_Action_Attack.tga",        },    // Special Action
    { "DismissCommandParameter",    "CMPetDismissString",            "PetCommand_Action_Dismiss.tga",    },    // Dismiss
};

int gRedoPetNames;

static int sAnyCommandPets;

#define MAX_PETS_IN_WINDOW 25

ContextMenu * gPetOptionContext;


//-----------------------------------------------------------------------------------------------
// PetOptions ///////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------

static void petOptionSimple( void * unused )
{
    optionToggle(kUO_AdvancedPetControls, 1);
}    

static char *petOptionSimpleTxt( void * unused)
{
    if( optionGet(kUO_AdvancedPetControls) )
        return textStd("SwitchToSimple");
    else
        return textStd("SwitchToAdvances");
}

static void petOptionIndividual( void * unused )
{
    optionToggle(kUO_ShowPetControls, 1);
}

static char *petOptionIndividualTxt( void * unused)
{
    if( optionGet(kUO_ShowPetControls) )
        return textStd("HideIndividualControl");
    else
        return textStd("ShowIndividualControl");
}

//-----------------------------------------------------------------------------------------------
// //////////////// /////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------
// Generic non mastermind functions that are valid in update 5

bool playerIsMasterMind()
{
    Entity * e = playerPtr();
    if (!e || !e->pchar || !e->pchar->pclass || !e->pchar->pclass->pchName)
        return false;

    if( stricmp( e->pchar->pclass->pchName, "Class_Mastermind") == 0 )
        return true;
    else
        return false;
}

bool entIsMyPet(Entity *e)
{
    Entity *pl = playerPtr();
    if ( !e || !e->pchar || !e->seq || !e->seq->type ||
         !e->erCreator || erGetEnt(e->erCreator) != pl ||
         !e->erOwner || erGetEnt(e->erOwner) != pl ) 
    {
        return false; // not a pet
    }
    if (e->pchar->iAllyID != pl->pchar->iAllyID)
        return false; //oil slick
    if ( !isPetDisplayed(e) )
        return false; //hostages
    if (e->seq->type->notselectable) 
        return false; //fire storm and such
    return true;
}

// Is this entity my pet? the definition of pet is fairly complicated
bool entIsMyHenchman(Entity *e)
{
    if (entIsMyPet(e)&&isPetCommadable(e))
        return true;
    return false;
}

bool entIsPlayerPet(Entity *e)
{
    Entity *owner;
    Entity *creator;

    if (!e || !e->pchar || !e->erOwner || !e->erCreator )
        return false;

    owner = erGetEnt(e->erOwner);
    creator = erGetEnt(e->erCreator);

    if ( !owner || !creator || (owner && (ENTTYPE(owner) != ENTTYPE_PLAYER)) )
        return false;

    if (e->pchar->iAllyID != creator->pchar->iAllyID)
        return false; //oil slick
    if (!isPetDisplayed(e))
        return false; //hostages
    if (!isEntitySelectable(e))
        return false; //fire storm and such
    return true;
}

bool entIsMyTeamPet(Entity *e)
{
    if (entIsPlayerPet(e))
    {
        Entity *owner = erGetEnt(e->erOwner);
        Entity *pl = playerPtr();
        if ((owner == pl) || team_IsMember(pl, owner->db_id))
            return true;
    }
    return false;}

bool entIsMyLeaguePet(Entity *e)
{
    if (entIsPlayerPet(e))
    {
        Entity *owner = erGetEnt(e->erOwner);
        Entity *pl = playerPtr();
        if ((owner == pl) || league_IsMember(pl, owner->db_id))
            return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------------
// Player Pets //////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------

#define MAX_DISPLAY_NAME 64
typedef struct PlayerPetPower
{
    int stance;         // stance for the power
    int action;         // action for the power
    int collapsed;     // the power is collapsed (not showing individual pets)]
    const BasePower *ppow; // the power
    char pchDisplayName[MAX_DISPLAY_NAME];

    ToolTip stanceTip;
    ToolTip actionTip;
    ToolTip simpleTip[4];
    
}PlayerPetPower;

typedef struct PlayerPet
{
    int svr_id;                // unique id for pet
    int stance;                // Mood
    int action;                // Current activity
    int dismissed;            // Dismissed pets do not show up
    int pow_count;            // If I'm the second entity created from a power this will be 1
                            // the third, this will be 2, etc.
    char name[128];            // if this is filled out, it will be the pet name with pow count appended 
    char oldPetName[128];    //The old pet name, update name if this changes

    PlayerPetPower * pppow;    // which power I'm from
    Entity * ent;            // entity ptr

    ToolTip stanceTip;
    ToolTip actionTip;
    ToolTip healthTip;
    ToolTip simpleTip[4];
}PlayerPet;

static PlayerPet **playerPets;
static PlayerPetPower **playerPetPowers;

static PlayerPet allPet = {0};
static PlayerPetPower allPetPower = {0};

static int comparePlayerPetPower(const void* ppet1Data, const void* ppet2Data)
{
    const PlayerPet** ppet1 = (const PlayerPet**)ppet1Data;
    const PlayerPet** ppet2 = (const PlayerPet**)ppet2Data;
    Entity * pet1 = entFromId( (*ppet1)->svr_id );
    Entity * pet2 = entFromId( (*ppet2)->svr_id ); 

    // I'm perhaps over analyzing this
    if( !pet1 )
        return -1;
    else if( !pet2 )
        return 1;
    else
    {
        //Sort arena pets into their own group, and sort by name, then by svr_id
        if( !pet1->pchar->ppowCreatedMe || !pet2->pchar->ppowCreatedMe )
        {
            if( pet1->pchar->ppowCreatedMe != pet2->pchar->ppowCreatedMe )
                return pet1->pchar->ppowCreatedMe < pet2->pchar->ppowCreatedMe;
            else
            {
                int result;
                result = strcmp( (*ppet1)->name, (*ppet2)->name );
                if( result )
                    return result;
                else
                    return (*ppet1)->svr_id - (*ppet2)->svr_id;
            }
        }
        else
        {
            int result;
            result = strcmp( pet1->pchar->ppowCreatedMe->pchName, pet2->pchar->ppowCreatedMe->pchName );
            if( result )
                return result;
            else 
                return (*ppet1)->pow_count - (*ppet2)->pow_count;
        }
    }
}


static PlayerPetPower *petPower_Add( const BasePower * ppow )
{
    int i, size;
    PlayerPetPower *ppp = NULL;

    if( !playerPetPowers )
        eaCreate(&playerPetPowers);

    size = eaSize(&playerPetPowers);
    for( i = 0; i < size; i++ )
    {
        if( playerPetPowers[i]->ppow == ppow )
            return playerPetPowers[i];
    }

    ppp = calloc( 1, sizeof(PlayerPetPower) );
    ppp->ppow = ppow;
    ppp->collapsed = false;
    ppp->action = kPetAction_Follow;
    ppp->stance = kPetStance_Defensive;
    if( ppow )
        strncpyt( ppp->pchDisplayName, ppp->ppow->pchDisplayName, MAX_DISPLAY_NAME );
    else
    {
        if( amOnArenaMap() )
            strncpyt( ppp->pchDisplayName, "ARENA", MAX_DISPLAY_NAME ); //TO DO translate
        else
            strncpyt( ppp->pchDisplayName, "ZoneEventPet", MAX_DISPLAY_NAME ); //TO DO translate
    }

    eaPush( &playerPetPowers, ppp );
    return ppp;
}

static void petPower_Clear()
{
    if( !playerPetPowers )
        return;

    while( eaSize(&playerPetPowers) )
    {
        PlayerPetPower * pp = eaRemove(&playerPetPowers, 0);
        if( pp ) 
        {
            int i;
            freeToolTip(&pp->actionTip);
            freeToolTip(&pp->stanceTip);
            for( i = 0; i < 4; i++ )
                freeToolTip(&pp->simpleTip[i]);
            SAFE_FREE( pp );
        }
    }
    eaDestroy( &playerPetPowers );
}


// Change pets with identical names to Bob 1, Bob 2 and so forth
static void pet_UpdateNames(void)
{
    int i;
    static int nameCount[MAX_PETS_IN_WINDOW]; 
    int size = eaSize(&playerPets);

    if (!gRedoPetNames)
        return; //don't update
    eaQSort(playerPets, comparePlayerPetPower);

    for( i = 0; i < size; i++ )
        nameCount[i] = 0; //reset the count

    for( i = 0; i < size; i++ )
    {
        int count = 1, j;
        if (nameCount[i] != 0 || !playerPets[i]->ent) 
            continue; //we've already set the count
        if (!playerPets[i]->ent->petName || playerPets[i]->ent->pchar->attrCur.fHitPoints <= 0.f)
        { 
            strcpy(playerPets[i]->name,playerPets[i]->ent->name);
            strcpy(playerPets[i]->oldPetName,playerPets[i]->ent->name);
            continue;
        }
        for (j = i+1; j < size; j++) //set the name of any duplicate
        { 
            if (playerPets[j]->ent && playerPets[j]->ent->petName && 
                strcmp(playerPets[i]->ent->petName,playerPets[j]->ent->petName) == 0 ) 
            {
                count++;
                nameCount[j] = count;
                sprintf( playerPets[j]->name, "%s %i", playerPets[j]->ent->petName, count ); 
                strcpy(playerPets[j]->oldPetName,playerPets[j]->ent->petName);
            }
        }
        if (count != 1)  //if more than one
        {
            sprintf( playerPets[i]->name, "%s %i", playerPets[i]->ent->petName, 1 ); 
            strcpy(playerPets[i]->oldPetName,playerPets[i]->ent->petName);
        } 
        else // if not, just print the basic name
        { 
            strcpy(playerPets[i]->name,playerPets[i]->ent->petName);
            strcpy(playerPets[i]->oldPetName,playerPets[i]->ent->petName);
        }
    }
    gRedoPetNames = false;
}


void pet_Add( int id )
{  
    int i, pow_count = 0, size;
    PlayerPet *pp;

    if( !playerPets )
        eaCreate(&playerPets);

    size = eaSize(&playerPets);
    if( size < MAX_PETS_IN_WINDOW )
    {
        int foundSpace = -1;
        Entity *ent = entFromId(id);
        for( i = 0; i < size; i++ )
        {
            if( playerPets[i]->svr_id == id ) 
            {
                if (ent->petName && strcmp(ent->petName,playerPets[i]->oldPetName) != 0) 
                { //Update pet names if they've changed
                    strcpy(playerPets[i]->oldPetName,ent->petName);
                    gRedoPetNames = true;
                }
                return;
            } 
            else if (foundSpace < 0 && playerPets[i]->pppow->ppow == ent->pchar->ppowCreatedMe &&
                (!playerPets[i]->ent || !playerPets[i]->ent->pchar || playerPets[i]->ent->pchar->attrCur.fHitPoints <= 0.f )) 
            {
                foundSpace = i;
                continue;
            }
        }

        if (foundSpace >= 0)
            pp = playerPets[foundSpace];
        else 
        {
            pp = calloc( 1, sizeof(PlayerPet) );
            pp->pow_count = 0;
        }

        if (((optionGet(kUO_ShowPets) && playerIsMasterMind()) || 
            ent->commandablePet) && window_getMode(WDW_PET) == WINDOW_DOCKED)
        {
            window_setMode(WDW_PET,WINDOW_GROWING);
        }

         pp->svr_id = id;
        pp->ent = ent;
        pp->pppow = petPower_Add(pp->ent->pchar->ppowCreatedMe);

        // default state?
        pp->stance = kPetStance_Defensive;
        pp->action = kPetAction_Follow;
        pp->dismissed = FALSE;
        if (ent->petName)
            strcpy(pp->oldPetName,ent->petName);
        else
            sprintf(pp->oldPetName,"");
         
        if (!pp->pow_count)//Use first non-used pow_count
        { 
            for(pp->pow_count = 0; pp->pow_count < MAX_PETS_IN_WINDOW; pp->pow_count++) 
            {
                int found = 0;
                for( i = 0; i < size; i++ )
                {
                    if( playerPets[i] != pp && playerPets[i]->pppow == pp->pppow && 
                        playerPets[i]->pow_count == pp->pow_count)
                    {
                        found = 1;
                        break;
                    }
                }
                if (found == 0) 
                    break;
            }
        }
        if (foundSpace < 0)
            eaPush(&playerPets, pp);
        gRedoPetNames = 1;
    }
    pet_UpdateNames();
}

void pet_Remove( int id )
{
    int i;
    for( i = eaSize(&playerPets)-1; i >= 0; i-- )
    {
        if( playerPets[i]->svr_id == id)
        {
            PlayerPet * pp = eaRemove(&playerPets, i);
            if( pp ) 
            {
                int j;
                freeToolTip(&pp->actionTip);
                freeToolTip(&pp->stanceTip);
                freeToolTip(&pp->healthTip);
                for( j = 0; j < 4; j++ )
                    freeToolTip(&pp->simpleTip[j]);
                SAFE_FREE( pp );
            }
            gRedoPetNames = TRUE;
            return;
        }
    }
}

void pets_Clear(void)
{
    if( !playerPets )
        return;

    while( eaSize(&playerPets) )
    {
        PlayerPet * pp = eaRemove(&playerPets, 0);
        if( pp ) 
        {
            int i;
            freeToolTip(&pp->actionTip);
            freeToolTip(&pp->stanceTip);
            freeToolTip(&pp->healthTip);
            for( i = 0; i < 4; i++ )
                freeToolTip(&pp->simpleTip[i]);
            SAFE_FREE( pp );
        }
    }
    eaDestroy( &playerPets );
    playerPets = 0;

    petPower_Clear();
}

void pet_select( int idx )
{
    if( idx >= 0 && idx < eaSize(&playerPets) )
    {
        current_target = playerPets[idx]->ent;
        current_pet = erGetRef(current_target);
    }
}

void pet_selectName( char * name )
{
    int i;
    for( i = 0; i < eaSize(&playerPets); i++ )
    {
        if( strstri(playerPets[i]->name, name) )
        {
            current_target = playerPets[i]->ent;
            current_pet = erGetRef(current_target);
        }
    }
}

bool entIsPet( Entity * e )
{
    int i;
    for( i = eaSize(&playerPets)-1; i >= 0; i-- )
    {
        if( playerPets[i]->ent == e )
            return true;
    }

    return false;
}


static PlayerPet *playerPetFromEnt( Entity *e )
{
    int i;
    for( i = eaSize(&playerPets)-1; i >= 0; i-- )
    {
        if( playerPets[i]->ent == e )
            return playerPets[i];
    }

    return NULL;
}

static bool petsMatchPowerAction( PlayerPetPower * ppp )
{
    int i, last_action = -1;

     for( i = eaSize(&playerPets)-1; i >= 0; i-- )
    {
        // action mismatched, see if they are all the same
        if( (playerPets[i]->pppow == ppp || !ppp->ppow) &&
             playerPets[i]->action != ppp->action && playerPets[i]->ent && 
            !playerPets[i]->dismissed && entIsMyHenchman(playerPets[i]->ent))
         {
            int j;
            for( j = eaSize(&playerPets)-1; j >= 0; j-- )
            {
                if( (playerPets[j]->pppow == ppp || !ppp->ppow) && playerPets[j]->ent)
                {
                    if( last_action < 0 )
                        last_action = playerPets[j]->action;
                    else if( playerPets[j]->action != last_action )
                        return false;
                }
            }
            break;
        }
    }

    if( last_action >= 0 )
        ppp->action = last_action;
    return true;
}

static bool petsMatchPowerStance( PlayerPetPower * ppp )
{
    int i, last_stance = -1;

     for( i = eaSize(&playerPets)-1; i >= 0; i-- )
    {
        // action mismatched, see if they are all the same
        if( (playerPets[i]->pppow == ppp || !ppp->ppow) &&
             playerPets[i]->stance != ppp->stance && playerPets[i]->ent && 
            !playerPets[i]->dismissed && entIsMyHenchman(playerPets[i]->ent))
        {
            int j;
             for( j = eaSize(&playerPets)-1; j >= 0; j-- )
            {
                if( (playerPets[j]->pppow == ppp || !ppp->ppow) && playerPets[j]->ent)
                {
                    if( last_stance < 0 )
                        last_stance = playerPets[j]->stance;
                    else if( playerPets[j]->stance != last_stance )
                        return false;
                }
            }
            break;
        }
    }

    if( last_stance >= 0 )
        ppp->stance = last_stance;
    return true;
}

//----------------------------------------------------------------------------------------------
// Pet Commands ////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------

void pet_update(int svr_idx,PetStance stance, PetAction action)
{
    int i;
    for( i = eaSize(&playerPets)-1; i >= 0; i-- )
    {
        if (playerPets[i]->svr_id == svr_idx)
        {
            playerPets[i]->stance = stance;
            playerPets[i]->action = action;
            return;
        }
    }
    // Couldn't find the pet, ignore for now.
}

static bool canSendPetCommand()
{
    static U32 badstatus = (1<<0) //held
        | (1<<1) //stunned
        | (1<<2) //slept
        | (1<<3) //terrorized
        | (1<<6);//onlyaffectself

    Entity * e = playerPtr();
    if(e && (e->ulStatusEffects & badstatus))
    {
        addSystemChatMsg( textStd("PetCommandFailBadStatus"), INFO_USER_ERROR, 0 );
        return false;
    }
    return true;
}



static void pet_commandCurrent( int stance, int action, Vec3 vec )
{
    int i;
    Entity * pet = erGetEnt( current_pet );

    if( !pet )
    {
        addSystemChatMsg( textStd("PetCommandFailNoTarget"), INFO_USER_ERROR, 0 );
        return;
    }

    if(!canSendPetCommand())
    {
        return;
    }

    // update UI
    for( i = eaSize(&playerPets)-1; i >= 0; i-- )
    {
        if( playerPets[i]->svr_id == pet->svr_idx && playerPets[i]->ent && entIsMyHenchman(playerPets[i]->ent) )
        {
            // Don't dismiss pets that can't be dismissed
            if (action == kPetAction_Dismiss && !isPetDismissable(playerPets[i]->ent))
                continue;

            if( stance >= 0 )
                playerPets[i]->stance = stance;
            if( action >= 0 )
                playerPets[i]->action = action;
            if( action == kPetAction_Dismiss ) 
            {
                playerPets[i]->dismissed = true;
                if (current_target == playerPets[i]->ent)
                    current_target = NULL;
                if (erGetEnt(current_pet) == playerPets[i]->ent)
                    current_pet = 0;
            }
        }
    }

    // send command
    sendPetCommand( pet->svr_idx, stance, action, vec);        
}

static void pet_commandName( int stance, int action, char * name, Vec3 vec )
{
    int i, bFoundMatch = false;

    if(!canSendPetCommand())
    {
        return;
    }

    // find pet
    for( i = eaSize(&playerPets)-1; i >= 0; i-- )
    {
        if( strstri( playerPets[i]->name, name ) && playerPets[i]->ent && entIsMyHenchman(playerPets[i]->ent) )
        {
            // Don't dismiss pets that can't be dismissed
            if (action == kPetAction_Dismiss && !isPetDismissable(playerPets[i]->ent))
                continue;

            // update UI
            if( stance >= 0 )
                playerPets[i]->stance = stance;
            if( action >= 0 )
                playerPets[i]->action = action;
            if( action == kPetAction_Dismiss )
            {
                playerPets[i]->dismissed = true;
                if (current_target == playerPets[i]->ent)
                    current_target = NULL;
                if (erGetEnt(current_pet) == playerPets[i]->ent)
                    current_pet = 0;
            }

            // send command
            sendPetCommand( playerPets[i]->svr_id, stance, action, vec);
            bFoundMatch = true;
        }
    }

    if( !bFoundMatch )
        addSystemChatMsg( textStd("PetCommandFailNoNameMatch", name), INFO_USER_ERROR, 0 );
}

static void pet_commandPower( int stance, int action, char * pow,  Vec3 vec )
{
    int i, bFoundMatch = false;

    if(!canSendPetCommand())
    {
        return;
    }

    // find power matches
    for( i = eaSize(&playerPets)-1; i >= 0; i-- )
    {
        // Don't dismiss pets that can't be dismissed
        if (action == kPetAction_Dismiss && !isPetDismissable(playerPets[i]->ent))
            continue;

        if( strstri( textStd(playerPets[i]->pppow->pchDisplayName), pow ) && 
            playerPets[i]->ent && entIsMyHenchman(playerPets[i]->ent) )
        {
            // update UI
            if( stance >= 0 )
                playerPets[i]->stance = stance;
            if( action >= 0 )
                playerPets[i]->action = action;
            if( action == kPetAction_Dismiss )
            {
                playerPets[i]->dismissed = true;
                if (current_target == playerPets[i]->ent)
                    current_target = NULL;
                if (erGetEnt(current_pet) == playerPets[i]->ent)
                    current_pet = 0;
            }

            // send command
            sendPetCommand( playerPets[i]->svr_id, stance, action, vec);
            bFoundMatch = true;
        }
    }

    if( !bFoundMatch )
        addSystemChatMsg( textStd("PetCommandFailNoPowerMatch", pow), INFO_USER_ERROR, 0 );
}
 
static void pet_commandAll( int stance, int action, Vec3 vec )
{
    int i;

    if(!canSendPetCommand())
    {
        return;
    } 
   
    // find power matches
    for( i = eaSize(&playerPets)-1; i >= 0; i-- )
    {
        if (!playerPets[i]->ent || (action != kPetAction_Dismiss && !entIsMyHenchman(playerPets[i]->ent)) )
            continue;

        // Don't dismiss pets that can't be dismissed
        if (action == kPetAction_Dismiss && !isPetDismissable(playerPets[i]->ent))
            continue;

        if( stance >= 0 )
            playerPets[i]->stance = stance;
        if( action >= 0 )
            playerPets[i]->action = action;

        if( action == kPetAction_Dismiss )
        {
            playerPets[i]->dismissed = true;
            if (current_target == playerPets[i]->ent)
                current_target = NULL;
            if (erGetEnt(current_pet) == playerPets[i]->ent)
                current_pet = 0;
        }

        // send command
        sendPetCommand( playerPets[i]->svr_id, stance, action, vec);
    }
}
 
void pet_selectCommand( int stance, int action, char * target, int command_type, Vec3 vec )
{
    switch( command_type )
    {
    case kPetCommandType_Current:
        pet_commandCurrent( stance, action, vec );
        break;
    case kPetCommandType_Name: 
        pet_commandName( stance, action, target, vec );
        break;
    case kPetCommandType_Power:
        pet_commandPower( stance, action, target, vec );
        break;
    case kPetCommandType_All:
        pet_commandAll( stance, action, vec );
        break;
    }
} 

#define MAX_ARGS    2
static TrayObj tray_obj;

static void setGotoCursor( int stance, int action, int command_type, char * target )
{
    tray_obj.iset = stance;
    tray_obj.ipow = action;
    tray_obj.islot = command_type;
    tray_obj.type = kTrayItemType_PetCommand;

    if( target )
        strncpyt( tray_obj.command, target, 255 );

    cursor.target_world = kWorldTarget_Normal;
    cursor.target_distance = 100.f;
    s_ptsActivated = &tray_obj;
}

static int parsePetCommand( char * cmd, int * stance, int * action, char * target, int command_type )
{
    char * args[MAX_ARGS];
    int i,j, count = tokenize_line_safe( cmd, args, MAX_ARGS, &cmd );
    *stance = -1, *action = -1;

    cursor.target_world = kWorldTarget_None;
    s_ptsActivated = NULL;

    if( count > MAX_ARGS )
    {
        addSystemChatMsg( textStd("PetCommandFailTooManyArgs"), INFO_USER_ERROR, 0 );
        return 0;
    }

    for( i = 0; i < count; i++ )
    {
        for( j = 1; j < kPetStance_Count; j++ ) // skip kPetStance_Unknown
        {
            if( commandParameterStrstr( petStances[j].displayName, args[i] ))
            {
                if( *stance >= 0 )
                {
                    addSystemChatMsg( textStd("PetCommandFailMultiStance"), INFO_USER_ERROR, 0 );
                    return 0;
                }
                else
                    *stance = j;
            }
        }

        for( j = 0; j < kPetAction_Count; j++)
        {
            if( commandParameterStrstr( petActions[j].displayName, args[i] ))
            {
                if( *action >= 0 )
                {
                    addSystemChatMsg( textStd("PetCommandFailMultiAction"), INFO_USER_ERROR, 0 );
                    return 0;
                }
                else
                    *action = j;
            }
        }
    }

    if( *action == kPetAction_Goto )
    {
        setGotoCursor( *stance, *action, command_type, target );
        return false;
    }

    if( *stance == -1 && *action == -1 )
    {
        addSystemChatMsg( textStd("PetCommandNoMatchFound"), INFO_USER_ERROR, 0 );
        return 0;
    }

    return true;
}



void pet_command( char * cmd, char *target_str, int command_type )
{
    int stance, action;

    if( eaSize(&playerPets) == 0)
    {
        addSystemChatMsg( textStd("PetCommandFailNoPets"), INFO_USER_ERROR, 0 );
        return;
    }

    if( parsePetCommand( cmd, &stance, &action, target_str, command_type ) )
        pet_selectCommand( stance, action, target_str, command_type, NULL );
}

void pet_say( char * msg, char * target_Str, int command_type )
{
    if( !amOnArenaMap() && !playerIsMasterMind() ) 
    {
        addSystemChatMsg( textStd("PetCommandFailNotMastermind"), INFO_USER_ERROR, 0 );
        return;
    }

    if( eaSize(&playerPets) == 0 )
    {
        addSystemChatMsg( textStd("PetCommandFailNoPets"), INFO_USER_ERROR, 0 );
        return;
    }

    if(command_type == kPetCommandType_Current)
    {
        Entity * pet = erGetEnt( current_pet );

        if( !pet )
        {
            addSystemChatMsg( textStd("PetCommandFailNoTarget"), INFO_USER_ERROR, 0 );
            return;
        }
        sendPetSay( pet->svr_idx, msg );
    }
    else
    {
        int i;
        bool bFoundMatch = false;

        for( i = eaSize(&playerPets)-1; i >= 0; i-- )
        {
            if (!playerPets[i]->ent || playerPets[i]->ent->pchar->attrCur.fHitPoints <= 0.f || !entIsMyHenchman(playerPets[i]->ent))
                continue;
            switch(command_type)
            {
                case kPetCommandType_Name:
                {
                    if( strstri( playerPets[i]->name, target_Str ) )
                    {
                        sendPetSay( playerPets[i]->svr_id, msg);
                        bFoundMatch = true;
                    }
                }break;

                case kPetCommandType_Power:
                {
                    if( strstri( textStd(playerPets[i]->pppow->pchDisplayName), target_Str ))
                    {
                        sendPetSay( playerPets[i]->svr_id, msg);
                        bFoundMatch = true;
                    }
                }break;

                case kPetCommandType_All:
                {
                    sendPetSay( playerPets[i]->svr_id, msg);
                    bFoundMatch = true;
                }break;
            }
        } // end for

        if( !bFoundMatch )
            addSystemChatMsg( textStd("PetCommandFailNoMatch"), INFO_USER_ERROR, 0 );
    }
}
// Pet Renaming

static void pet_rename_verify(Entity *pet, char * name)
{
    ValidateNameResult vnr = ValidateName(name,NULL,0,MAX_PETNAME_LENGTH);
    switch (vnr)
    {
    case VNR_Titled:
    case VNR_Reserved:
    case VNR_Valid:
        sendPetRename(pet->svr_idx,name);
        if (!pet->petName || strlen(name) > strlen(pet->petName))
        {
            SAFE_FREE(pet->petName);
            pet->petName = strdup( name );
        }
        else
            strcpy(pet->petName,name);

        gRedoPetNames = 1;
        break;        

    case VNR_Malformed:
    case VNR_Profane:
    case VNR_WrongLang:
    case VNR_TooLong:
    default:
        addSystemChatMsg( textStd("InvalidName"), INFO_USER_ERROR, 0 );
        break;
    }
}


void pet_rename( char * name, char * target_Str, int command_type )
{
    if( amOnArenaMap() || !playerIsMasterMind() ) 
    {
        addSystemChatMsg( textStd("PetCommandFailNotMastermind"), INFO_USER_ERROR, 0 );
        return;
    }

    if( eaSize(&playerPets) == 0 )
    {
        addSystemChatMsg( textStd("PetCommandFailNoPets"), INFO_USER_ERROR, 0 );
        return;
    }

    if(command_type == kPetCommandType_Current)
    {
        Entity * pet = erGetEnt( current_pet );

        if( !pet || !entIsMyHenchman(pet) || !pet->pchar->ppowCreatedMe )
        {
            addSystemChatMsg( textStd("PetCommandFailNoTarget"), INFO_USER_ERROR, 0 );
            return;
        }
        pet_rename_verify( pet, name );
    }
    else
    {
        int i;
        bool bFoundMatch = false;

        for( i = eaSize(&playerPets)-1; i >= 0; i-- )
        {
            if (!playerPets[i]->ent || playerPets[i]->ent->pchar->attrCur.fHitPoints <= 0.f)
                continue;
            if (command_type == kPetCommandType_Name)
            {
                if( strcmpi( playerPets[i]->name, target_Str ) == 0 && entIsMyHenchman(playerPets[i]->ent) && playerPets[i]->ent->pchar->ppowCreatedMe)
                {
                    pet_rename_verify( playerPets[i]->ent, name);
                    bFoundMatch = true;
                }
            }

        } // end for

        if( !bFoundMatch )
            addSystemChatMsg( textStd("PetCommandFailNoMatch"), INFO_USER_ERROR, 0 );
    }
}

void pet_inspireByName(char * pchInsp, char *pchName)
{
    int i, slot, row;

    if(!canSendPetCommand())
        return;

    if( inspiration_FindByName(pchInsp, &slot, &row) )
    {
        TrayObj *to = inspiration_getCur(slot, row);

        // find pet
        for( i = eaSize(&playerPets)-1; i >= 0 && to; i-- )
        {
            if( strstri( playerPets[i]->name, pchName ) && playerPets[i]->ent && entIsMyHenchman(playerPets[i]->ent) )
            {
                gift_SendAndRemove(playerPets[i]->svr_id,to);
            }
        }
    }
}

void pet_inspireTarget(char * pchInsp)
{
    int slot, row;

    if(!canSendPetCommand())
        return;

    if( inspiration_FindByName(pchInsp, &slot, &row) )
    {
        TrayObj *to = inspiration_getCur(slot, row);
        if( to && entIsMyHenchman(current_target) )
        {
            gift_SendAndRemove(current_target->svr_idx,to);
        }
    }
}

//----------------------------------------------------------------------------------------------
// Context Menus ///////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------

static ContextMenu * sStanceContext;
static ContextMenu * sActionContext;
static ContextMenu * sActionArenaContext;
static ContextMenu * sPowerStanceContext;
static ContextMenu * sPowerActionContext;
static ContextMenu * sPowerActionArenaContext;

static int petStanceIsSelected( Entity * pet, int stance )
{
    PlayerPet * ppet = playerPetFromEnt(pet);
    if( !ppet )
        return CM_HIDE;

    if( ppet->stance == stance )
        return CM_CHECKED;
    else
        return CM_AVAILABLE;
}

static int petPowStanceIsSelected( PlayerPetPower * ppp, int stance )
{
    if( ppp->stance == stance )
        return CM_CHECKED;
    else
        return CM_AVAILABLE;
}

static void petSelectStance( Entity * pet,int stance )
{
    PlayerPet * ppet = playerPetFromEnt(pet);
    if( !ppet )
        return;

    if( ppet->ent )
        current_pet = erGetRef( ppet->ent );
    pet_selectCommand( stance, -1, NULL, kPetCommandType_Current, NULL );
}

static void petPowSelectStance( PlayerPetPower * ppp, int stance )
{
    char buff[32];
    if (ppp->ppow == NULL) {
        pet_selectCommand( stance, -1, NULL, kPetCommandType_All, NULL );
        return;
    }
    strncpyt( buff, textStd(ppp->pchDisplayName), 32 );
    pet_selectCommand( stance, -1, buff, kPetCommandType_Power, NULL );
}

static int petActionIsSelected( Entity * pet, int action )
{
    PlayerPet * ppet = playerPetFromEnt(pet);
    if( !ppet )
        return CM_HIDE;

    if( ppet->action == action )
        return CM_CHECKED;
    else
        return CM_AVAILABLE;
}

static int petActionSpecialPowerState( Entity * pet )
{
    //If you don't have a special power, don't use it at all
    if( !pet->specialPowerDisplayName )
        return CM_HIDE;

    //TO DO enumerate -- the only reason I didn't was to avoid changing entity.h at this moment
    if( pet->specialPowerState == 0 ) //0 == DONT HAVE IT
        return CM_HIDE;

    if( pet->specialPowerState == 1 ) //1 == AVAILABLE
        return CM_AVAILABLE;

    if( pet->specialPowerState == 2 ) //2 == USING / ATTEMPTING TO USE 
        return CM_VISIBLE;

    if( pet->specialPowerState == 3 ) //3 == UNAVAILABLE : RECHARGING / NOT ENOUGH ENDURANCE
        return CM_VISIBLE;

    return CM_HIDE;
}

static int petPowActionIsSelected( PlayerPetPower * ppp, int action )
{
    if( ppp->action == action )
        return CM_CHECKED;
    else
        return CM_AVAILABLE;
}

static int petPowActionSpecialPowerState(void * pet)
{
    return CM_HIDE;
    //You can't show the special power line when selecting by power, since not all pets will
    //have the same special powers in the same states.  We might design a use-it-if-you-can 
    //interface.
}

static void petSelectAction( Entity * pet, int action )
{
    PlayerPet * ppet = playerPetFromEnt(pet);
    if( !ppet )
        return;

    current_pet = erGetRef( ppet->ent );
    if( action == kPetAction_Goto )
        setGotoCursor( -1, action, kPetCommandType_Current, NULL );
    else
        pet_selectCommand( -1, action, NULL, kPetCommandType_Current, NULL );
}

static void petPowSelectAction( PlayerPetPower * ppp, int action )
{
    char buff[32];
    strncpyt( buff, textStd(ppp->pchDisplayName), 32 );

    if( action == kPetAction_Goto )
    {
        if (ppp->ppow == NULL) {
            setGotoCursor( -1, action, kPetCommandType_All, NULL );
        }
        else
            setGotoCursor( -1, action, kPetCommandType_Power, buff );
    }
    else {
        if (ppp->ppow == NULL) {
            pet_selectCommand( -1, action, NULL, kPetCommandType_All, NULL );
        }
        else
            pet_selectCommand( -1, action, buff, kPetCommandType_Power, NULL );
    }
}

static void petDismiss( Entity * pet )
{
     PlayerPet * ppet = playerPetFromEnt(pet);
    if( !ppet )
        return;

    current_pet = erGetRef( ppet->ent );
    pet_selectCommand( -1, kPetAction_Dismiss, NULL, kPetCommandType_Current, NULL );
}

static void petPowDismiss( PlayerPetPower * ppp )
{
    char buff[32];
    if (ppp->ppow == NULL)
        pet_selectCommand( -1, kPetAction_Dismiss, NULL, kPetCommandType_All, NULL );
    else 
    {
        strncpyt( buff, textStd(ppp->pchDisplayName), 32 ); 
        pet_selectCommand( -1, kPetAction_Dismiss, buff, kPetCommandType_Power, NULL );
    }

}

static void petRenameDlgHandler(void * data)
{
    char buf[1000];
    sprintf(buf, "pet_rename \"%s\"", dialogGetTextEntry());
    cmdParse(buf);
}

static void petRenameDialog(void * foo)
{
    dialogNameEdit(textStd("PetRenameDialog"), NULL, NULL,  petRenameDlgHandler, NULL, 15, 0);
}

static void infoPet(Entity *pet)
{
    if (pet && pet->owner > 0 && pet->owner < entities_max)
    {
        info_Entity(pet,0);
    }
}

static void infoPetNumbers(Entity *pet)
{
    PlayerPet *ppet = playerPetFromEnt(pet);

    if (pet && entIsMyHenchman(pet) && ppet )
    {
        char tmp[128];
        sprintf_s(tmp, 127, "petViewAttributes %i", ppet->svr_id);
        cmdParse(tmp);
        window_setMode(WDW_COMBATNUMBERS, WINDOW_GROWING);
    }
}

                    // this is ghetto
static INLINEDBG int petStanceIsPassive(void *pet)        { return petStanceIsSelected( pet, kPetStance_Passive); }
static INLINEDBG int petStanceIsDefensive(void *pet)    { return petStanceIsSelected( pet, kPetStance_Defensive); }
static INLINEDBG int petStanceIsAggressive(void *pet)    { return petStanceIsSelected( pet, kPetStance_Aggressive); }

static INLINEDBG void petSetStancePassive(void *pet)    { petSelectStance(pet, kPetStance_Passive ); }
static INLINEDBG void petSetStanceDefensive(void *pet)    { petSelectStance(pet, kPetStance_Defensive ); }
static INLINEDBG void petSetStanceAggressive(void *pet)    { petSelectStance(pet, kPetStance_Aggressive ); }

static INLINEDBG int petActionIsAttack(void * pet)    { return petActionIsSelected(pet, kPetAction_Attack); }
static INLINEDBG int petActionIsGoto(void * pet)    { return petActionIsSelected(pet, kPetAction_Goto); }
static INLINEDBG int petActionIsFollow(void * pet)    { return petActionIsSelected(pet, kPetAction_Follow); }
static INLINEDBG int petActionIsStay(void * pet)    { return petActionIsSelected(pet, kPetAction_Stay); }

static INLINEDBG void petSetActionAttack(void * pet)    { petSelectAction(pet, kPetAction_Attack); }
static INLINEDBG void petSetActionGoto(void * pet)        { petSelectAction(pet, kPetAction_Goto); }
static INLINEDBG void petSetActionFollow(void * pet)    { petSelectAction(pet, kPetAction_Follow); }
static INLINEDBG void petSetActionStay(void * pet)        { petSelectAction(pet, kPetAction_Stay); }
static INLINEDBG void petSetActionSpecial(void * pet)    { petSelectAction(pet, kPetAction_Special); }

// ghetto-ness doubled by need for per power version
static INLINEDBG int petPowStanceIsPassive(void *pet)    { return petPowStanceIsSelected( pet, kPetStance_Passive); }
static INLINEDBG int petPowStanceIsDefensive(void *pet)    { return petPowStanceIsSelected( pet, kPetStance_Defensive); }
static INLINEDBG int petPowStanceIsAggressive(void *pet){ return petPowStanceIsSelected( pet, kPetStance_Aggressive); }

static INLINEDBG void petPowSetStancePassive(void *pet)        { petPowSelectStance(pet, kPetStance_Passive ); }
static INLINEDBG void petPowSetStanceDefensive(void *pet)    { petPowSelectStance(pet, kPetStance_Defensive ); }
static INLINEDBG void petPowSetStanceAggressive(void *pet)    { petPowSelectStance(pet, kPetStance_Aggressive ); }

static INLINEDBG int petPowActionIsAttack(void * pet)    { return petPowActionIsSelected(pet, kPetAction_Attack); }
static INLINEDBG int petPowActionIsGoto(void * pet)        { return petPowActionIsSelected(pet, kPetAction_Goto); }
static INLINEDBG int petPowActionIsFollow(void * pet)    { return petPowActionIsSelected(pet, kPetAction_Follow); }
static INLINEDBG int petPowActionIsStay(void * pet)        { return petPowActionIsSelected(pet, kPetAction_Stay); }

static INLINEDBG void petPowSetActionAttack(void * pet) { petPowSelectAction(pet, kPetAction_Attack); }
static INLINEDBG void petPowSetActionGoto(void * pet)    { petPowSelectAction(pet, kPetAction_Goto); }
static INLINEDBG void petPowSetActionFollow(void * pet) { petPowSelectAction(pet, kPetAction_Follow); }
static INLINEDBG void petPowSetActionStay(void * pet)    { petPowSelectAction(pet, kPetAction_Stay); }
static INLINEDBG void petPowSetActionSpecial(void * pet){ petPowSelectAction(pet, kPetAction_Special); }

static const char * petOptionSimpleTxtAdapter(void* arg0)
{
    return petOptionSimpleTxt((void *)arg0);
}

static const char * petOptionIndividualTxtAdapter(void* arg0)
{
    return petOptionIndividualTxt((void *)arg0);
}

static void petDismissAdapter(void* arg0)
{
    petDismiss((Entity *)arg0);
}

static void petPowDismissAdapter(void* arg0)
{
    petPowDismiss((PlayerPetPower *)arg0);
}

static void initPetContextMenus()
{
    if( gPetOptionContext )
        return;

    gPetOptionContext         = contextMenu_Create(NULL);
    sStanceContext             = contextMenu_Create(NULL);
    sActionContext             = contextMenu_Create(NULL);
    sActionArenaContext         = contextMenu_Create(NULL);
    sPowerStanceContext         = contextMenu_Create(NULL);
    sPowerActionContext         = contextMenu_Create(NULL);
    sPowerActionArenaContext = contextMenu_Create(NULL);

    contextMenu_addTitle( gPetOptionContext, "PetWindowOptions" );
    contextMenu_addVariableTextCode( gPetOptionContext, 0, 0, petOptionSimple, 0, petOptionSimpleTxtAdapter, 0, 0 );
    contextMenu_addVariableTextCode( gPetOptionContext, 0, 0, petOptionIndividual, 0, petOptionIndividualTxtAdapter, 0, 0 );

    contextMenu_addIconCheckBox( sActionContext, petActionIsAttack, NULL, petSetActionAttack, NULL, petActions[kPetAction_Attack].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Attack].iconName) );
    contextMenu_addIconCheckBox( sActionContext, petActionIsGoto, NULL, petSetActionGoto, NULL, petActions[kPetAction_Goto].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Goto].iconName) );
    contextMenu_addIconCheckBox( sActionContext, petActionIsFollow, NULL, petSetActionFollow, NULL, petActions[kPetAction_Follow].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Follow].iconName) );
    contextMenu_addIconCheckBox( sActionContext, petActionIsStay, NULL, petSetActionStay, NULL, petActions[kPetAction_Stay].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Stay].iconName) );

    contextMenu_addDivider( sActionContext );
    contextMenu_addCode( sActionContext, NULL, NULL, petDismissAdapter, NULL, petActions[kPetAction_Dismiss].cmDisplayName, 0 );

    contextMenu_addIconCheckBox( sActionArenaContext, petActionIsAttack, NULL, petSetActionAttack, NULL, petActions[kPetAction_Attack].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Attack].iconName) );
    contextMenu_addIconCheckBox( sActionArenaContext, petActionIsGoto, NULL, petSetActionGoto, NULL, petActions[kPetAction_Goto].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Goto].iconName) );
    contextMenu_addIconCheckBox( sActionArenaContext, petActionIsFollow, NULL, petSetActionFollow, NULL, petActions[kPetAction_Follow].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Follow].iconName) );
    contextMenu_addIconCheckBox( sActionArenaContext, petActionIsStay, NULL, petSetActionStay, NULL, petActions[kPetAction_Stay].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Stay].iconName) );

    contextMenu_addIconCheckBox( sStanceContext, petStanceIsPassive, NULL, petSetStancePassive, NULL, petStances[kPetStance_Passive].cmDisplayName, atlasLoadTexture(petStances[kPetStance_Passive].iconName) );
    contextMenu_addIconCheckBox( sStanceContext, petStanceIsDefensive, NULL, petSetStanceDefensive, NULL, petStances[kPetStance_Defensive].cmDisplayName, atlasLoadTexture(petStances[kPetStance_Defensive].iconName) );
    contextMenu_addIconCheckBox( sStanceContext, petStanceIsAggressive, NULL, petSetStanceAggressive, NULL, petStances[kPetStance_Aggressive].cmDisplayName, atlasLoadTexture(petStances[kPetStance_Aggressive].iconName) );    

    contextMenu_addIconCheckBox( sPowerActionContext, petPowActionIsAttack, NULL, petPowSetActionAttack, NULL, petActions[kPetAction_Attack].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Attack].iconName) );
    contextMenu_addIconCheckBox( sPowerActionContext, petPowActionIsGoto, NULL, petPowSetActionGoto, NULL, petActions[kPetAction_Goto].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Goto].iconName) );
    contextMenu_addIconCheckBox( sPowerActionContext, petPowActionIsFollow, NULL, petPowSetActionFollow, NULL, petActions[kPetAction_Follow].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Follow].iconName) );
    contextMenu_addIconCheckBox( sPowerActionContext, petPowActionIsStay, NULL, petPowSetActionStay, NULL, petActions[kPetAction_Stay].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Stay].iconName) );
    contextMenu_addDivider( sPowerActionContext );
    contextMenu_addIconCode( sPowerActionContext, NULL, NULL, petPowDismissAdapter, NULL, petActions[kPetAction_Dismiss].cmDisplayName, 0 ,atlasLoadTexture(petActions[kPetAction_Dismiss].iconName));

    contextMenu_addIconCheckBox( sPowerActionArenaContext, petPowActionIsAttack, NULL, petPowSetActionAttack, NULL, petActions[kPetAction_Attack].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Attack].iconName) );
    contextMenu_addIconCheckBox( sPowerActionArenaContext, petPowActionIsGoto, NULL, petPowSetActionGoto, NULL, petActions[kPetAction_Goto].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Goto].iconName) );
    contextMenu_addIconCheckBox( sPowerActionArenaContext, petPowActionIsFollow, NULL, petPowSetActionFollow, NULL, petActions[kPetAction_Follow].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Follow].iconName) );
    contextMenu_addIconCheckBox( sPowerActionArenaContext, petPowActionIsStay, NULL, petPowSetActionStay, NULL, petActions[kPetAction_Stay].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Stay].iconName) );

    contextMenu_addIconCheckBox( sPowerStanceContext, petPowStanceIsPassive, NULL, petPowSetStancePassive, NULL, petStances[kPetStance_Passive].cmDisplayName, atlasLoadTexture(petStances[kPetStance_Passive].iconName) );
    contextMenu_addIconCheckBox( sPowerStanceContext, petPowStanceIsDefensive, NULL, petPowSetStanceDefensive, NULL, petStances[kPetStance_Defensive].cmDisplayName, atlasLoadTexture(petStances[kPetStance_Defensive].iconName) );
    contextMenu_addIconCheckBox( sPowerStanceContext, petPowStanceIsAggressive, NULL, petPowSetStanceAggressive, NULL, petStances[kPetStance_Aggressive].cmDisplayName, atlasLoadTexture(petStances[kPetStance_Aggressive].iconName) );    
}

void petoption_open()
{
    float x, y;
    initPetContextMenus();
    childButton_getCenter( wdwGetWindow(WDW_PET), 0, &x, &y );
    contextMenu_set( gPetOptionContext, x, y, NULL );
}

int petcm_isDismissable( void * data )
{
    Entity *pet = (Entity *) data;
    if (isPetDisplayed(pet) && isPetDismissable(pet))
        return CM_AVAILABLE;
    return CM_HIDE;
}

int petcm_isRenamable( void * data )
{
    Entity *pet = (Entity *) data;
     if ( pet && playerIsMasterMind() && entIsMyHenchman(pet) && 
        pet->pchar &&pet->pchar->ppowCreatedMe)
    {
        return CM_AVAILABLE;
    }
    return CM_HIDE;
}

static void infoPetAdapter(void* arg0)
{
    infoPet((Entity *)arg0);
}

static void infoPetNumbersAdapter(void* arg0)
{
    infoPetNumbers((Entity *)arg0);
}

ContextMenu * interactPetMenu( void )
{
    ContextMenu * interactPet = contextMenu_Create( NULL );

    contextMenu_addTitle( interactPet, "StanceString" );
    contextMenu_addIconCheckBox( interactPet, petStanceIsPassive, NULL, petSetStancePassive, NULL, petStances[kPetStance_Passive].cmDisplayName, atlasLoadTexture(petStances[kPetStance_Passive].iconName) );
    contextMenu_addIconCheckBox( interactPet, petStanceIsDefensive, NULL, petSetStanceDefensive, NULL, petStances[kPetStance_Defensive].cmDisplayName, atlasLoadTexture(petStances[kPetStance_Defensive].iconName) );
    contextMenu_addIconCheckBox( interactPet, petStanceIsAggressive, NULL, petSetStanceAggressive, NULL, petStances[kPetStance_Aggressive].cmDisplayName, atlasLoadTexture(petStances[kPetStance_Aggressive].iconName) );    
    contextMenu_addDivider( interactPet );

    contextMenu_addTitle( interactPet, "ActionString" );
    contextMenu_addIconCheckBox( interactPet, petActionIsAttack, NULL, petSetActionAttack, NULL, petActions[kPetAction_Attack].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Attack].iconName) );
    contextMenu_addIconCheckBox( interactPet, petActionIsGoto, NULL, petSetActionGoto, NULL, petActions[kPetAction_Goto].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Goto].iconName) );
    contextMenu_addIconCheckBox( interactPet, petActionIsFollow, NULL, petSetActionFollow, NULL, petActions[kPetAction_Follow].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Follow].iconName) );
    contextMenu_addIconCheckBox( interactPet, petActionIsStay, NULL, petSetActionStay, NULL, petActions[kPetAction_Stay].cmDisplayName, atlasLoadTexture(petActions[kPetAction_Stay].iconName) );

    contextMenu_addDivider( interactPet );
    contextMenu_addIconCode( interactPet, petcm_isDismissable, NULL, petDismissAdapter, NULL, petActions[kPetAction_Dismiss].cmDisplayName, 0,atlasLoadTexture(petActions[kPetAction_Dismiss].iconName));
    contextMenu_addCode( interactPet, petcm_isRenamable, NULL, petRenameDialog, 0, "CMRenamePet", 0 );
    contextMenu_addCode( interactPet, NULL, NULL, infoPetAdapter, 0, "CMInfoString", 0 );
    contextMenu_addCode( interactPet, NULL, NULL, infoPetNumbersAdapter, 0, "CMInfoNumbersString", 0 );
    return interactPet;
}

static int isCurrentTargetAdapter(void)
{
    return isCurrentTarget();
}

ContextMenu * interactSimplePetMenu( void )
{
    ContextMenu * interactPet = contextMenu_Create( isCurrentTargetAdapter);
    
    contextMenu_addCode( interactPet, NULL, NULL, petDismissAdapter, 0, petActions[kPetAction_Dismiss].cmDisplayName, 0);
    contextMenu_addCode( interactPet, NULL, NULL, infoPetAdapter, 0, "CMInfoString", 0 );

    return interactPet;
}

//-----------------------------------------------------------------------------------------------
// Drawing functions ////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------

#define COM_WD    16.f

// This should be long enough for a name plus a stance.
// eg. "Freedom Corps Cataphract Aggressive"
#define PET_NAME_STANCE_MAX_LEN 60

static int drawPetCommandSimple( PlayerPet * ppet, float x, float y, float z, float sc, int isPet )
{
    CBox box;
    AtlasTex *icon;
    char command[255];
    char shortName[PET_NAME_STANCE_MAX_LEN];
    int i, color = CLR_WHITE;
    float xp, icon_sc;

    //--------------------------------------------------------------------------
    for( i = 0; i < ARRAY_SIZE(newbMastermindMacros); i++ )
    {
        xp = x + i*(COM_WD+4)*sc;
        BuildCBox( &box, xp - COM_WD/2, y - COM_WD/2, COM_WD+1, COM_WD );

        if( isPet )
        {
            clearToolTip( &ppet->actionTip );
            clearToolTip( &ppet->stanceTip );
            addToolTip(&ppet->simpleTip[i]);
             setToolTip(&ppet->simpleTip[i],&box,newbMastermindMacros[i].pchName,&GroupTipParent,MENU_GAME,WDW_PET);
        }
        else
        {
            clearToolTip( &ppet->pppow->actionTip );
            clearToolTip( &ppet->pppow->stanceTip );
            addToolTip(&ppet->pppow->simpleTip[i]);
            setToolTip(&ppet->pppow->simpleTip[i],&box,newbMastermindMacros[i].pchName,&GroupTipParent,MENU_GAME,WDW_PET);
        }

        icon = atlasLoadTexture( newbMastermindMacros[i].pchIconName );

        if( mouseCollision(&box) )
        {
            icon_sc  = ((COM_WD-2)/icon->height)*sc;
            xp += 1*sc;
        }
        else
            icon_sc  = (COM_WD/icon->height)*sc;

         display_sprite( icon, xp - icon->width*icon_sc/2, y - icon->height*icon_sc/2, z+1, icon_sc, icon_sc, color );
 
        if( ppet == &allPet )
        {
            strcpy( command, newbMastermindMacros[i].pchDisplayHelp );
            strcpy( shortName, textStd("AllString") );
            Strncatt( shortName, " " );    
        }
        else if( isPet )
        {
            sprintf( command, "petcom_name \"%s\" %s", 
                        textStd(ppet->name), newbMastermindMacros[i].pchDisplayHelp+strlen("petcom_all ") );    
            strcpy( shortName, textStd(ppet->name) );
            Strncatt( shortName, " " );    
        }
        else
        {
            sprintf( command, "petcom_pow \"%s\" %s", textStd(ppet->pppow->pchDisplayName), 
                     newbMastermindMacros[i].pchDisplayHelp+strlen("petcom_all ") );    
            strcpy( shortName, textStd(ppet->pppow->pchDisplayName) );
            Strncatt( shortName, " " );    
        }

         if( mouseClickHit( &box, MS_LEFT ) || mouseClickHit( &box, MS_RIGHT ) )
            cmdParse( command );

        if( mouseLeftDrag(&box) && !optionGet(kUO_PreventPetIconDrag) )
        {
            TrayObj to = {0};
            Strncatt( shortName, textStd(newbMastermindMacros[i].pchName) );    
             buildMacroTrayObj( &to, command, shortName, newbMastermindMacros[i].pchIconName, 1 );
            trayobj_startDragging( &to, icon, NULL );
            cursor.drag_obj.islot = -1;         // tag the slot this power is coming from
            cursor.drag_obj.itray = -1;
            cursor.drag_obj.icat = -1;
        }
    }

    return 0;
}

static int drawPetCommands( PlayerPet * ppet, float x, float y, float z, float sc, int isPet )
{
    AtlasTex * icon, *back = atlasLoadTexture( "tray_ring_power_background.tga" );
    float icon_sc;
    CBox box;
    int i, stance, action;
     float xp = x;
    int color = CLR_WHITE;
    bool bActionMatch = petsMatchPowerAction( ppet->pppow );
    bool bStanceMatch = petsMatchPowerStance( ppet->pppow );

       if( !optionGet(kUO_AdvancedPetControls) )
        return drawPetCommandSimple(ppet, x, y, z, sc, isPet );

    if( isPet ) // if all pet commands match power, dim pet commands, otherwise
    {            // dim the power commands
        stance = ppet->stance;
        action = ppet->action;
        if( bActionMatch && bStanceMatch )
            color = 0xffffff88;

        for( i = 0; i < 4; i++ )
            clearToolTip( &ppet->simpleTip[i] );
    }
    else
    {
        stance = ppet->pppow->stance;
        action = ppet->pppow->action;
        if( (!bActionMatch || !bStanceMatch) && (optionGet(kUO_ShowPetControls) || amOnPetArenaMap()) ) // don't dim if pet controls aren't shown
            color = 0xffffff88;

        for( i = 0; i < 4; i++ )
            clearToolTip( &ppet->pppow->simpleTip[i] );
    }

    // Stance
    //-------------------------------
     BuildCBox( &box, xp - COM_WD/2, y - COM_WD/2, COM_WD+1, COM_WD );

    // Is this a power control that has pets using other powers?  special icon
    if( !isPet && !bStanceMatch )
    {
        icon = atlasLoadTexture( "PetCommand_ConflictingCommand.tga" );
        addToolTip(&ppet->pppow->stanceTip);
        setToolTip(&ppet->pppow->stanceTip,&box,"ConflictingTip",&GroupTipParent,MENU_GAME,WDW_PET);
    }
    else
    {
        if (isPet) 
        {
            addToolTip(&ppet->stanceTip);
            setToolTip(&ppet->stanceTip,&box,petStances[stance].displayName,&GroupTipParent,MENU_GAME,WDW_PET);
        }
        else 
        {
            addToolTip(&ppet->pppow->stanceTip);
            setToolTip(&ppet->pppow->stanceTip,&box,petStances[stance].displayName,&GroupTipParent,MENU_GAME,WDW_PET);
        }
        icon = atlasLoadTexture( petStances[stance].iconName );
    }

    if( mouseCollision(&box) )
    {
        icon_sc  = ((COM_WD-2)/icon->height)*sc;
        xp += 1*sc;
    }
    else
        icon_sc  = (COM_WD/icon->height)*sc;

    display_sprite( icon, xp - icon->width*icon_sc/2, y - icon->height*icon_sc/2, z+1, icon_sc, icon_sc, color );

    if( mouseClickHit( &box, MS_LEFT ) || mouseClickHit( &box, MS_RIGHT ) )
    {
        if( isPet )
            contextMenu_displayEx( sStanceContext, ppet->ent );
        else
            contextMenu_displayEx( sPowerStanceContext, ppet->pppow );
    }

    if( mouseLeftDrag(&box) && (isPet||bStanceMatch) && !optionGet(kUO_PreventPetIconDrag)  )
    {
        TrayObj to = {0};
        char command[255];
        char shortName[PET_NAME_STANCE_MAX_LEN];

        if(isPet)
        {
            strcpy( shortName, textStd(ppet->name) );
            Strncatt( shortName, " " );
            sprintf( command, "petcom_name \"%s\" %s", ppet->name, textStd(petStances[stance].displayName) );
        }
        else
        {
            if( ppet == &allPet )
            {
                strcpy( shortName, textStd("AllString") );
                Strncatt( shortName, " " );
                sprintf( command, "petcom_all %s", textStd(petStances[stance].displayName) );
            }
            else
            {
                strcpy( shortName, textStd(ppet->pppow->pchDisplayName) );
                Strncatt( shortName, " " );
                sprintf( command, "petcom_pow \"%s\" %s", textStd(ppet->pppow->pchDisplayName), textStd(petStances[stance].displayName) );
            }
        }
        Strncatt( shortName, textStd(petStances[stance].displayName) );
        buildMacroTrayObj( &to, command, shortName, petStances[stance].iconName, 1 );
        trayobj_startDragging( &to, icon, NULL );
        cursor.drag_obj.islot = -1;         // tag the slot this power is coming from
        cursor.drag_obj.itray = -1;
        cursor.drag_obj.icat = -1;
    }

    // activity
    //-----------------------------
     xp = x + (COM_WD+4)*sc;
 
    BuildCBox( &box, xp - COM_WD/2, y - COM_WD/2, COM_WD+1, COM_WD );
    if( !isPet && !bActionMatch ) 
    {
        icon = atlasLoadTexture( "PetCommand_ConflictingCommand.tga" );
        addToolTip(&ppet->pppow->actionTip);
        setToolTip(&ppet->pppow->actionTip,&box,"ConflictingTip",&GroupTipParent,MENU_GAME,WDW_PET);
    }
    else
    {
        if (isPet)
        {
            addToolTip(&ppet->actionTip);
            setToolTip(&ppet->actionTip,&box,petActions[action].displayName,&GroupTipParent,MENU_GAME,WDW_PET);
        } 
        else 
        {
            addToolTip(&ppet->pppow->actionTip);
            setToolTip(&ppet->pppow->actionTip,&box,petActions[action].displayName,&GroupTipParent,MENU_GAME,WDW_PET);
        }
        icon = atlasLoadTexture( petActions[action].iconName );
    }

    if( mouseCollision(&box) )
    {
        icon_sc  = ((COM_WD-2)/icon->height)*sc;
        xp += 1*sc;
    }
    else
        icon_sc  = (COM_WD/icon->height)*sc;


    if( mouseClickHit( &box, MS_LEFT ) || mouseClickHit( &box, MS_RIGHT ) )
    {
        if( isPet )    
        {
            if (amOnPetArenaMap())
                contextMenu_displayEx( sActionArenaContext, ppet->ent );
            else
                contextMenu_displayEx( sActionContext, ppet->ent );
        }
        else
        {
            if (amOnPetArenaMap())
                contextMenu_displayEx( sPowerActionArenaContext, ppet->pppow );
            else
                contextMenu_displayEx( sPowerActionContext, ppet->pppow );
        }
    }

      display_sprite( icon, xp - icon->width*icon_sc/2, y - icon->height*icon_sc/2, z+1, icon_sc, icon_sc, color );

    if( mouseLeftDrag(&box) && (isPet||bActionMatch) && !optionGet(kUO_PreventPetIconDrag)  )
    {
        TrayObj to = {0};
        char command[255];
        char shortName[PET_NAME_STANCE_MAX_LEN];

        if(isPet)
        {
            strcpy( shortName, textStd(ppet->name) );
            Strncatt( shortName, " " );
            sprintf( command, "petcom_name \"%s\" %s", ppet->name, textStd(petActions[action].displayName) );
        }
        else
        {
            if( ppet == &allPet )
            {
                strcpy( shortName, textStd("AllString") ); 
                Strncatt( shortName, " " );
                sprintf( command, "petcom_all %s", textStd(petActions[action].displayName) );
            }
            else
            {
                strcpy( shortName, textStd(ppet->pppow->pchDisplayName) );
                Strncatt( shortName, " " );
                sprintf( command, "petcom_pow \"%s\" %s", textStd(ppet->pppow->pchDisplayName), textStd(petActions[action].displayName) );
            }
        }
        Strncatt( shortName, textStd(petActions[action].displayName) );
        buildMacroTrayObj( &to, command, shortName, petActions[action].iconName, 1 );
        trayobj_startDragging( &to, icon, NULL );
        cursor.drag_obj.islot = -1;         // tag the slot this power is coming from
        cursor.drag_obj.itray = -1;
        cursor.drag_obj.icat = -1;
    }
    // add more buttons here

    return 0;
}

#define PET_ENTRY_WD    185
static void drawPetPower( PlayerPet * ppet, float x, float y, float z,  float sc, float wd, float ht, int color )
{
    font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );

    if( !ppet->pppow  )
        cprntEx( x+R10*sc, y + ht/2, z, sc, sc, CENTER_Y,  "ZoneEventPet" );
    else
           cprntEx( x+R10*sc, y + ht/2, z, sc, sc, CENTER_Y,  ppet->pppow->pchDisplayName );

    x += R10*sc;
    if(!optionGet(kUO_AdvancedPetControls))
         x -= COM_WD;

      if( ppet == &allPet || entIsMyHenchman(ppet->ent) )
         drawPetCommands( ppet, x + (PET_ENTRY_WD-32)*sc, y + ht/2 - 1*sc, z, sc, false );
}


static int drawPetEntry( PlayerPet * ppet, float x, float y, float z,  float sc, float wd, float ht, int flipped, int i )
{
    CBox box;
    int bDead = 0, bHit = 0;
    float hp = 0, absorb = 0, endurance = 0;
    int fcolor, fbcolor;
    int selected = 0;
    AtlasTex *icon = 0;
    Entity *pet = ppet->ent;
    float pet_entry_wd = PET_ENTRY_WD-15;

       BuildCBox( &box, x+ 6*sc, y, pet_entry_wd*sc, ht*sc );
    if( mouseCollision( &box ) )
    {
        bHit = true;
        if(mouseClickHit(&box, MS_LEFT ) )
        {
            if( pet ) {
                current_target = ppet->ent;
                if (entIsMyHenchman(pet)) 
                    current_pet = erGetRef(current_target);
            }
        }

        if( !isDown(MS_LEFT) && playerIsMasterMind() )
        {
            if( cursor.drag_obj.type == kTrayItemType_Inspiration && !amOnPetArenaMap() )
            {
                gift_SendAndRemove( ppet->svr_id, &cursor.drag_obj );
            }
            trayobj_stopDragging();
        }
    }

    if( mouseClickHit(&box, MS_RIGHT) )
    {
        if ( pet)
        {
            if (entIsMyPet(pet)) 
            {
                current_target = ppet->ent;
                current_pet = erGetRef(pet);
                contextMenuForTarget(pet);
            }
        }
    }

    if( pet && current_target && current_target->svr_idx == pet->svr_idx )
        selected = 1;

     if( pet && pet->pchar )
    {
         if( pet->pchar->attrCur.fHitPoints == 0 )
        {
            icon = atlasLoadTexture( "teamup_skull.tga" );
            addToolTip(&ppet->healthTip);
            setToolTip(&ppet->healthTip,&box,"deadTip",&GroupTipParent,MENU_GAME,WDW_PET);        
            fcolor = 0xffffff88;
            fbcolor = 0x00000088;
            if (strcmp(pet->name,ppet->name)!= 0) gRedoPetNames = true;
        }
        else
        {
            hp = pet->pchar->attrCur.fHitPoints / pet->pchar->attrMax.fHitPoints;
            absorb = pet->pchar->attrMax.fAbsorb / pet->pchar->attrMax.fHitPoints;
            endurance = pet->pchar->attrCur.fEndurance / pet->pchar->attrMax.fEndurance;
            fcolor = getHealthColor(entity_TargetIsInVisionPhase(playerPtr(), pet), hp);
            fbcolor = fcolor & 0xffffff00;

            if ( bHit  )
                fbcolor |= 0xaaaaaa88;
            else
                fbcolor |= 0x00000044;
            addToolTip(&ppet->healthTip);
            setToolTip(&ppet->healthTip,&box,textStd("HealthTip",pet->pchar->attrCur.fHitPoints,pet->pchar->attrMax.fHitPoints),&GroupTipParent,MENU_GAME,WDW_PET);
        }

        // --------------------
        // draw the group item
        {
            float z_offset = z;
            float x_offset = x + 6*sc;

             drawGroupInterior( x_offset, y, ++z_offset, pet_entry_wd*sc, sc, hp, absorb, endurance, fcolor & 0xffffff88, ppet->name, selected, 0, 0, 0, icon, ppet->pow_count, 0, true, 0, optionGet(kUO_UseOldTeamUI));
        }
    }

    // draw buttons
     if( entIsMyHenchman(ppet->ent) && optionGet(kUO_ShowPetControls) )
            drawPetCommands( ppet, x + (pet_entry_wd+15)*sc, y + ht/2, z, sc, true );
            
    // Draw buff list
    set_scissor(false);
    if( optionGet(kUO_ShowPetBuffs) && pet && pet->pchar)
           linedrawBuffs( kBuff_Pet, pet, x, y, z, wd+3*PIX3*sc, ht, sc, flipped, 0 );
    set_scissor(true);

    return true;
}

//-----------------------------------------------------------------------------------------------
// Master Window ////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------
int petWindow(void)
{
    float x, y, ty, z, sc, wd, ht;
     int bcolor, color, w, h, i, flipped = 0, count = 0, displayed_pets = 0;
    const BasePower * ppowBase = 0;
    AtlasTex * buffArrow;
    static int numPetPowers = 0, last_ht = 30;

    if( amOnPetArenaMap() ) 
        window_setMode( WDW_PET, WINDOW_DISPLAYING );    

    for( i = 0; i < eaSize( &playerPets ); i++ )
    {
        Entity * pet = entFromId( playerPets[i]->svr_id );
        if(playerPets[i]->svr_id != -1  )
        {
            if (!entIsMyPet(pet))
            {
                pet_Remove( playerPets[i]->svr_id );
                continue;
            }
        }
    }
    pet_UpdateNames();

    if( window_getMode( WDW_PET) == WINDOW_DISPLAYING )
    {
        float win_wd = 225.f;

        if ( ((playerIsMasterMind() || sAnyCommandPets) && optionGet(kUO_ShowPetControls)) || amOnPetArenaMap())
        {
            if( optionGet(kUO_AdvancedPetControls) )
                win_wd = 255;
            else
                win_wd = 275;
        }
        else
            win_wd = 218;

        window_setDims( WDW_PET, -1, -1, win_wd*winDefs[WDW_PET].loc.sc, last_ht );
    }

     if( !window_getDims( WDW_PET, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ) )
        return 0;

    BuildCBox( &GroupTipParent.box, x, y, wd, ht);
    initPetContextMenus();
    initnewbMasterMindMacros();

    if( playerIsMasterMind() || amOnPetArenaMap() || sAnyCommandPets )
        window_addChild(wdwGetWindow(WDW_PET), 0, "PetOptions", "petoptions" );
    else
        window_removeChild(wdwGetWindow(WDW_PET), "PetOptions" );    

    if (allPet.pppow == NULL) 
    {    //initialize fake all power
        allPet.pppow = &allPetPower;
        sprintf(allPetPower.pchDisplayName,"%s",textStd("AllPets"));
        allPetPower.collapsed = false;
        allPetPower.action = kPetAction_Follow;
        allPetPower.stance = kPetStance_Defensive;
    }

      drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );

    set_scissor(true);
    scissor_dims( x + PIX3*sc, y + PIX3*sc, wd - PIX3*sc*2, ht-PIX3*sc*2 );

    windowClientSizeThisFrame(&w, &h);
    if( x + wd/2 > w/2 )
        flipped = true;

    // sort by power
     eaQSort(playerPets, comparePlayerPetPower );

     ty = y + PIX3*2*sc;

    //draw fake all pet power
    if ((numPetPowers > 1 && playerIsMasterMind()) || (eaSize(&playerPets)> 0 && sAnyCommandPets) )
    {
        drawPetPower( &allPet, x+PIX3*sc, ty, z, sc, wd - 4*PIX3*sc, 25*sc, color );
        ty += 25*sc;
        count++;
    }

    numPetPowers = 0;
    sAnyCommandPets = 0;

    // loop over pet array
    for( i = 0; i < eaSize( &playerPets ); i++ )
    {
        Entity *pet;
        if ( !playerPets[i]->ent || playerPets[i]->dismissed )
            continue;

        pet = entFromId( playerPets[i]->svr_id );
        
        if (entIsMyHenchman(pet))
            sAnyCommandPets = 1;

        // new power category
         if( ppowBase != playerPets[i]->pppow->ppow && !amOnPetArenaMap() )
        {
            ppowBase = playerPets[i]->pppow->ppow;
               drawPetPower( playerPets[i], x+PIX3*sc, ty, z, sc, wd - 4*PIX3*sc, 25*sc, color );
            ty += 25*sc;
            count++;
            numPetPowers++;
        }    

        if(drawPetEntry( playerPets[i], x+(R10+PIX3)*sc, ty, z, sc, wd - (4*PIX3+R10)*sc, 22*sc, flipped, i ))
         {
            ty += 25*sc;
            count++;
        }

        displayed_pets++;
    }

      ty += PIX3*2*sc;
    set_scissor(false);

     if( window_getMode( WDW_PET) == WINDOW_DISPLAYING )
    {
        float win_wd = 225.f;

          if( displayed_pets > 0 )
        {
            AtlasTex * arrow = atlasLoadTexture( "chat_separator_arrow_down.tga" );

            if( (!optionGet(kUO_ShowPetBuffs) && !flipped) || (optionGet(kUO_ShowPetBuffs) && flipped) )
                buffArrow = atlasLoadTexture( "teamup_arrow_expand.tga" );
            else
                buffArrow = atlasLoadTexture( "teamup_arrow_contract.tga" );

            if(D_MOUSEHIT == drawGenericButton( buffArrow, flipped?x+5*sc:x + wd - (buffArrow->width+5)*sc, y + (ht-buffArrow->height*sc)/2, z, sc, color, CLR_GREEN, 1 ) )
            {
                optionToggle(kUO_ShowPetBuffs, 1);
                collisions_off_for_rest_of_frame = TRUE;
            }

            // minimize/maximize arrow - copied almost entirely from uiChat.c
            if (!amOnPetArenaMap()) {
                AtlasTex * arrow = atlasLoadTexture( "chat_separator_arrow_down.tga" );
                Wdw * wdw = wdwGetWindow( WDW_PET );
                float arrowY, angle;

                if( !optionGet(kUO_ShowPetControls) )
                    angle = -PI/2;
                else
                    angle = PI/2;

                if( (!wdw->below && !wdw->flip) || (wdw->below && wdw->flip) )
                    arrowY = y - (JELLY_HT + arrow->height)*sc/2;
                else
                    arrowY = y + ht + (JELLY_HT - arrow->height)*sc/2;

                   if( D_MOUSEHIT == drawGenericRotatedButton( arrow, x + (PET_ENTRY_WD-20)*sc, arrowY, z, sc, (color & 0xffffff00) | (0x88), (color & 0xffffff00) | (0xff), 1, angle ) )
                {
                    optionToggle(kUO_ShowPetControls, 1);
                    collisions_off_for_rest_of_frame = true;
                }
            }    

            last_ht = ty-y;
        }
        else
        {
            font( &game_12 );
               font_color( CLR_WHITE, 0x888888ff );
            cprntEx( x + wd/2, y + ht/2, z, sc, sc,CENTER_X|CENTER_Y, "NoPetsString" );
            numPetPowers = 0;
            last_ht = 30;
         }
    }

    if( !optionGet(kUO_ShowPets) && !playerIsMasterMind() && !sAnyCommandPets )
        window_setMode( WDW_PET, WINDOW_DOCKED );

    return 0;
}
