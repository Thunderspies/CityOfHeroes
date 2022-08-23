

#include "entity/entity.h"
#include <utilitieslib/components/earray.h>
#include "entity/powers.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/utils/mathutil.h> 
#include "svr/svr_base.h"
#include "gameComm/svr_chat.h"
#include "gameComm/petCommon.h"
#include "comm_game.h"
#include "entity/entPlayer.h"
#include "entity/entserver.h"
#include "ai/entaivars.h"
#include "gameComm/VillainDef.h"
#include <utilitieslib/components/MemoryPool.h>
#include "entity/entVarUpdate.h"
#include "storyarc/storyarcutil.h"
#include "ai/entaiprivate.h"
#include "filter/validate_name.h"
#include <utilitieslib/network/net_packetutil.h>
#include "entity/character_base.h"
#include "language/langServerUtil.h"
#include "entity/character_target.h"
#include "pet.h"
#include "seq/seqstate.h"
#include "seq/seq.h"
#include <utilitieslib/language/MessageStore.h>
#include "ailib/aiBehaviorPublic.h"

#define MAX_PET_NAMES_ON_PLAYER        100

static char * stance_flags[kPetStance_Count] = 
{ 
    "",                    // kPetStance_Unknown
    "StancePassive",    // kPetStance_Passive
    "StanceDefensive",    // kPetStance_Defensive
    "StanceAggressive",    // kPetStance_Aggressive
};

STATIC_ASSERT(kPetStance_Count == 4);

MP_DEFINE(PetName);

PetName* petname_Create(void)
{
    MP_CREATE(PetName, 100);
    return MP_ALLOC(PetName);
}

void PetNameDestroy(PetName* pn) 
{
    MP_FREE(PetName, pn);
}


static void PetSays(Entity *owner, Entity *pet, unsigned int offString, const char *targetname, const char *powername)
{
    static int initted = 0;
    static Array *tags;
    char message[1024];        // WARNING! Watch for overflow!
    char ***messages;
    int i;

    if(!initted)
    {
        tags = msCompileMessageType("{OwnerName, %s}, {MyName, %s}, {TargetName, %s}, {PowerName, %r}");
        initted = 1;
    }

    if(!pet || !pet->villainDef || !pet->villainDef->petCommandStrings)
        return;

    messages = (char***)(((intptr_t)pet->villainDef->petCommandStrings[0]) + offString);
    i = eaSize(messages);
    if(i>0)
    {
        i = rule30Rand() % i;
        if((*messages)[i] && (*messages)[i][0])
        {
            msxPrintf(svrMenuMessages, SAFESTR(message), owner->playerLocale, (*messages)[i], tags, NULL, translateFlag(owner), owner->name, pet->name, targetname, powername);
            saUtilEntSays(pet, kEntSay_PetCommand, message);
        }
    }
}

#define PETSTR(x) offsetof(PetCommandStrings, ppch##x)

void petCommand( Entity * pet_owner, Entity * pet, int stance, int action, Vec3 go_to_pos )
{
    char * string = NULL;
    CharacterAttributes* attribs = &pet_owner->pchar->attrCur;

    // A held mastermind loses command ability
    if(attribs->fStunned > 0.0f || attribs->fHeld > 0.0f ||
        attribs->fTerrorized > 0.0f || attribs->fSleep > 0.0f || attribs->fOnlyAffectsSelf > 0.0f ||
        aiBehaviorGetTopBehaviorByName(pet, ENTAI(pet)->base, "RunThroughDoor"))
    {
        return; // Chat message?
    }

    if( action >= 0 && pet->pchar->attrCur.fHitPoints > 0.f) //dead pets don't get to do stuff
    {
        if( action == kPetAction_Dismiss && pet->petDismissable )
        {
             PetSays(pet_owner, pet, PETSTR(Dismiss), "", "");
            estrPrintCharString(&string, "ActionDismiss");

        }
        else if(action == kPetAction_Attack)
        {
            Entity * target = erGetEnt(pet_owner->pchar->erTargetInstantaneous);

             if(!target) 
            {
                PetSays(pet_owner, pet, PETSTR(AttackNoTarget), "", "");
            }
            else
            {
                if( target->pchar && character_TargetIsFoe(pet_owner->pchar,target->pchar))
                {
                    PetSays(pet_owner, pet, PETSTR(AttackTarget), target->name, "");
                    estrPrintf(&string, "ActionAttack(%s)", erGetRefString(target));
                }
                else
                    PetSays(pet_owner, pet, PETSTR(AttackNoTarget), "", "");
            }
        }
        else if(action == kPetAction_Stay)
        {
            PetSays(pet_owner, pet, PETSTR(StayHere), "", "");
            estrPrintCharString(&string, "ActionStay");
        }
        else if(action == kPetAction_Special)
        {
            //TO DO AIPowerInfo is private to the entAI and all this ought to be moved into the private AI area
            AIPowerInfo * powerInfo = aiFindAIPowerInfoByPowerName( pet, pet->villainDef->specialPetPower );
            if( !powerInfo )
            {
                PetSays(pet_owner, pet, PETSTR(UsePowerNone), "", "");
            }
            else
            {
                PetSays(pet_owner, pet, PETSTR(UsePower), "", powerInfo->power->ppowBase->pchDisplayName);
                estrPrintf( &string, "ActionUsePower(%s)", pet->villainDef->specialPetPower );
            }
        }
        else if(action == kPetAction_Follow)
        {
            PetSays(pet_owner, pet, PETSTR(FollowMe), "", "");
            estrPrintCharString(&string, "ActionFollow");
        }
        else if(action == kPetAction_Goto)
        {
            PetSays(pet_owner, pet, PETSTR(GotoSpot), "", "");
            estrPrintf(&string, "ActionGoto(%1.1f,%1.1f,%1.1f)", go_to_pos[0], go_to_pos[1], go_to_pos[2]);
        }

        if(string && string[0])
            aiBehaviorAddFlag(pet, ENTAI(pet)->base, aiBehaviorGetTopBehaviorByName(pet, ENTAI(pet)->base, "Pet"), string);
    }
    if( stance >= 0  && pet->pchar->attrCur.fHitPoints > 0.f)
    {
        switch(stance)
        {
            case kPetStance_Passive:
                PetSays(pet_owner, pet, PETSTR(Passive), "", "");
                break;
            case kPetStance_Defensive:
                PetSays(pet_owner, pet, PETSTR(Defensive), "", "");
                break;
            case kPetStance_Aggressive:
                PetSays(pet_owner, pet, PETSTR(Aggressive), "", "");
                break;
        }

        aiBehaviorAddFlag(pet,ENTAI(pet)->base, aiBehaviorGetTopBehaviorByName(pet, ENTAI(pet)->base, "Pet"), stance_flags[stance]);
    }
}


void petReceiveCommand( Entity * e, Packet * pak )
{
    int svr_id = pktGetBits( pak, 32 );
    int stance = pktGetBits( pak, 32 );
    int action = pktGetBits( pak, 32 );
    Entity *pet = entFromId( svr_id );
    Vec3 vec;

    if ( !isPetDisplayed(pet) )
    {
        chatSendToPlayer( e->db_id, localizedPrintf(e,"PetCommandFailed"), INFO_USER_ERROR, 0 );
        return;
    }

    if (!isPetCommadable(pet))
    {   
        if( action == kPetAction_Dismiss && pet->petDismissable ) //allow everyone to dismiss their pets
        {
            SETB(pet->seq->sticky_state, STATE_DISMISS);
            pet->pchar->attrCur.fHitPoints = 0;
        }
        else
        {
            chatSendToPlayer( e->db_id, localizedPrintf(e,"PetCommandFailNotMastermind"), INFO_USER_ERROR, 0 );
            return;
        }
    }

    if( action == kPetAction_Goto )
        pktGetVec3(pak, vec);

    petCommand( e, pet, stance, action, vec );

}

void petReceiveSay( Entity * e, Packet * pak )
{
    int svr_id = pktGetBits( pak, 32 );
    char * msg = pktGetString( pak );
    Entity *pet = entFromId( svr_id );

    if (chatBanned(e,msg))
        return;

    if ( !isPetDisplayed(pet) ) 
    {
        chatSendToPlayer( e->db_id, localizedPrintf(e,"PetCommandFailed"), INFO_USER_ERROR, 0 );
        return;
    }

    if( !isPetCommadable(pet) )
    {
        chatSendToPlayer( e->db_id, localizedPrintf(e,"PetCommandFailNotMastermind"), INFO_USER_ERROR, 0 );
        return;
    }

    if (pet->pchar->attrCur.fHitPoints <= 0.f)
        return; //don't let dead pets talk

    saUtilEntSays(pet, kEntSay_PetSay, msg);
}

static char *getFullPowerName(BasePower *pow) 
{
    const char *pName = pow->pchName;
    const char *sName = pow->psetParent->pchName;
    const char *cName = pow->psetParent->pcatParent->pchName;
    char *buf = calloc(strlen(pName)+strlen(sName)+strlen(cName)+3,sizeof(char)); //3 = 2 .'s and a null
    sprintf(buf,"%s.%s.%s",cName,sName,pName);
    return buf;
}


static int comparePetName(const PetName** ppet1, const PetName** ppet2 )
{
    int result = strcmp((*ppet1)->pchEntityDef,(*ppet2)->pchEntityDef);
    if (result) 
        return result;
    else 
        return (*ppet1)->petNumber-(*ppet2)->petNumber;
}

void petResetPetNames(Entity* e) 
{
    int j, petCount = e->petList_size;
    for (j = 0; j < petCount; j++)
    {
        Entity *pet = erGetEnt(e->petList[j]);
        
        if(!pet)
            continue;

        if (!pet->petName || strlen(pet->name) > strlen(pet->petName))
        {
            SAFE_FREE(pet->petName);
            pet->petName = strdup( pet->name );
        }
        else
            strcpy(pet->petName,pet->name);

        pet->petinfo_update = TRUE;
    }
}

void petReceiveRename( Entity * e, Packet * pak )
{
    int svr_id = pktGetBits( pak, 32 );
    char * name = pktGetString( pak );
    Entity *pet = entFromId( svr_id );
    ValidateNameResult vnr;

    if ( !pet || !pet->pchar ||                // no entity found
        ENTTYPE(pet) != ENTTYPE_CRITTER ||  // not a critter
        !pet->erCreator || !pet->erOwner || // not a pet 
        erGetEnt(pet->erOwner) != e ||        // not my pet
        erGetEnt(pet->erCreator) != e || 
        !pet->pchar->ppowCreatedMe ) // MUST be created by a power
    {
        chatSendToPlayer( e->db_id, localizedPrintf(e,"PetCommandFailed"), INFO_USER_ERROR, 0 );
        if (pet)
            pet->petinfo_update = 1;
        return;
    }

    if( !isPetCommadable(pet) )
    {    
        pet->petinfo_update = 1;
        return; // don't let forcefield be renamed
    }

    //Mastermind only For arena pets, no power created you, so you don't have to be a mastermind
    if( stricmp( e->pchar->pclass->pchName, "Class_Mastermind") != 0 )
    {
        chatSendToPlayer( e->db_id, localizedPrintf(e,"PetCommandFailNotMastermind"), INFO_USER_ERROR, 0 );
        pet->petinfo_update = 1;
        return;
    }

    vnr = ValidateName(name,NULL,0,MAX_PETNAME_LENGTH);

    // checking for same name as player
    if (vnr == VNR_Valid || vnr == VNR_Titled)
    {
        if (strcmp(e->name, name) == 0) 
        {
            vnr = VNR_Reserved;
        }
    }

    switch (vnr) {
    case VNR_Titled:
    case VNR_Valid:
        {    
            int i, nameCount, nameFound = 0;
            PetName *pn;
            const char *entDefName = (pet->villainDef && pet->villainDef->name) ? pet->villainDef->name : "none";
            nameCount = eaSize(&e->pl->petNames);

            if (nameCount <= MAX_PET_NAMES_ON_PLAYER)
            {
                for (i = 0; i < eaSize(&e->pl->petNames);i++)
                {
                    pn = e->pl->petNames[i];
                    if (strcmp(pn->pchEntityDef,entDefName) == 0) {            
                        if (strcmp(pn->petName,pet->petName) == 0)
                        {
                            strcpy(pn->petName,name);
                            if (!pet->petName || strlen(name) > strlen(pet->petName))
                            {
                                SAFE_FREE(pet->petName);
                                pet->petName = strdup( name );
                            }
                            else
                                strcpy(pet->petName,name);
                            pet->petinfo_update = TRUE;
                            return;
                        } 
                        else
                            nameFound++;
                    }
                }

                //conPrintf(client,"Adding new name %s for type %s",name,powername);
                pn = petname_Create();
                strcpy(pn->pchEntityDef,entDefName);
                strcpy(pn->petName,name);
                if (!pet->petName || strlen(name) > strlen(pet->petName))
                {
                    SAFE_FREE(pet->petName);
                    pet->petName = strdup( name );
                }
                else 
                    strcpy(pet->petName,name);

                pn->petNumber = nameFound;
                eaPush(&e->pl->petNames,pn);
                eaQSort(e->pl->petNames, comparePetName);
                pet->petinfo_update = TRUE;
            } else {
                // clear up really big number of pet names
                for (i = nameCount - 1; i > MAX_PET_NAMES_ON_PLAYER; i--)
                {
                    if (strcmp(e->pl->petNames[i]->pchEntityDef,entDefName) == 0)
                        eaRemove(&e->pl->petNames, i);
                }
            }
            return;
        }
    case VNR_Malformed:
    case VNR_Profane:
    case VNR_WrongLang:
    case VNR_TooLong:
    case VNR_Reserved:
    default:
        chatSendToPlayer( e->db_id, localizedPrintf(e,"InvalidName"), INFO_USER_ERROR, 0 );
        pet->petinfo_update = TRUE;
        return;
    }
}

void petUpdateClientUI(Entity* e, PetAction action, PetStance stance)
{
    Entity* owner = erGetEnt(e->erOwner);

    if(owner)
    {
        START_PACKET(pak, owner, SERVER_PET_UI_UPDATE);
        pktSendBitsPack(pak, 1, e->owner);
        pktSendBits(pak, PET_ACTION_BITS, action);
        pktSendBits(pak, PET_STANCE_BITS, stance);
        END_PACKET;
    }
}