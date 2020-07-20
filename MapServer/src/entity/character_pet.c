/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>

#include <utilitieslib/components/earray.h>

#include "ailib/aiBehaviorPublic.h"
#include "ai/entai.h"
#include "ai/entaiLog.h"
#include "ai/entaivars.h"
#include "gameComm/villainDef.h"
#include "entGameActions.h"
#include "dbcomm/dbcomm.h"
#include "dbghelper.h"

#include "entity/powers.h"
#include "entity/character_base.h"
#include "character_combat.h"
#include "entity/character_level.h"
#include "character_combat_eval.h"

#include "character_pet.h"
#include "entserver.h"
#include "entity/motion.h"
#include "generator/encounter.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include <utilitieslib/components/earray.h>
#include "seq/seq.h"
#include "cmdparse/cmdserver.h"
#include "seq/seqstate.h"
#include "gameComm/pet.h"
#include "entity/character_mods.h"
#include "entity/costume.h"
#include "entity/power_customization.h"
#include "storyarc/mission.h"
#include "buddy_server.h"
#include "dbcomm/dbmapxfer.h"
#include "dbcomm/logcomm.h"
#include "entity/attribmod.h"
/**********************************************************************func*
 * PlayerAddPet
 *
 */
void PlayerAddPet(Entity * eCreator, Entity *pet)
{
    int i, alreadyThere = 0;
    EntityRef petRef = erGetRef(pet);
    EntityRef *ref;

    for(i = 0; i < eCreator->petList_size; i++)
    {
        if(    eCreator->petList[i] == petRef )
            alreadyThere = 1;
    }

    if( !alreadyThere )
    {
        assert(eCreator->petList_size < 1000);
        ref = dynArrayAddStruct(&eCreator->petList, &eCreator->petList_size, &eCreator->petList_max);
        *ref = petRef;
    }

    if( !eCreator->pl )
    {
        Entity* petowner;
        AIVars* ai;

        if(!eCreator->erOwner || !(petowner = erGetEnt(eCreator->erOwner)))
            petowner = eCreator;

        // ARM NOTE:  This looks mildly scary.  We add to the creator's Entity::petList,
        // but the owner's ai->power.numPets...  at least it's consistent with the delete...
        ai = ENTAI(petowner);
        if(ai)
            ai->power.numPets++;

        return;
    }
}

/**********************************************************************func*
 * PlayerRemovePet
 *
 */
void PlayerRemovePet(Entity * eCreator, Entity *pet)
{
    int i;
    EntityRef petRef = erGetRef(pet);

    if( !eCreator || !pet )
        return;

    for(i = eCreator->petList_size - 1; i >= 0 ; i--)
    {
        if(eCreator->petList[i] == petRef)
        {
            eCreator->petList[i] = eCreator->petList[eCreator->petList_size - 1];
            eCreator->petList_size--;
        }
    }

    if( !eCreator->pl )
    {
        Entity* petowner;
        AIVars* ai;

        if(!eCreator->erOwner || !(petowner = erGetEnt(eCreator->erOwner)))
            petowner = eCreator;

        // ARM NOTE:  This looks mildly scary.  We delete from the creator's Entity::petList,
        // but the owner's ai->power.numPets...  at least it's consistent with the add...
        ai = ENTAI(petowner);
        if(ai)
            ai->power.numPets--;

        return;
    }
}


/**********************************************************************func*
 * player_DeAggressivePets
 *
 */
void character_MakePetsUnaggressive(Character *p)
{
    Entity *e = p->entParent;
    if( e && e->pl && e->petList_size )
    {
        int i;
        for(i=e->petList_size-1; i>=0; i--)
        {
            Entity *pet = erGetEnt(e->petList[i]);
            if(pet)
            {
                int behaviorHandle = aiBehaviorGetTopBehaviorByName(e, ENTAI(e)->base, "Pet");
                if(behaviorHandle)
                    aiBehaviorAddFlag(pet, ENTAI(pet)->base, behaviorHandle, "NotAggressive");
            }
        }
    }
}


/**********************************************************************func*
 * character_GetOwner
 *
 */
Character *character_GetOwner(Character *p)
{
    Entity *pOwner = p->entParent;

    if(pOwner->erOwner)
    {
        // This is a pet, return the owner.
        pOwner = erGetEnt(pOwner->erOwner);
    }

    return pOwner?pOwner->pchar:p;
}


/**********************************************************************func*
* MW version of Pet Creation for the Creature arena
*
*/
Entity * character_CreateArenaPet( Entity * eCreator, char * petVillainDef, int level )
{
    Entity * pPet = 0;

    if( !eCreator || !petVillainDef )
        return 0;

    AI_LOG(AI_LOG_POWERS, (eCreator, "Process :    Creating arena pet %s.\n", petVillainDef));

    pPet=villainCreateByName( petVillainDef, level, NULL, false, NULL, erGetRef(eCreator), NULL);

    if(pPet!=NULL)
    {
        character_SetLevelExtern(pPet->pchar, level); //Force guy to the level you wanted, in case it failed

        SET_ENTTYPE(pPet) = ENTTYPE_CRITTER;
        pPet->erOwner    = erGetRef( eCreator );

        entUpdateMat4Instantaneous(pPet, ENTMAT(eCreator));

        entSetOnTeam(pPet, eCreator->pchar->iAllyID);
        entSetOnGang(pPet, eCreator->pchar->iGangID);

        pPet->fade = 0; // They appear instantly (as opposed to fading in).

        character_ActivateAllAutoPowers(pPet->pchar);

        // Finally, set the priority list for the pet
        aiInit(pPet, NULL);
        aiBehaviorAddString(pPet, ENTAI(pPet)->base, "FeelingBothered(1.0),ConsiderEnemyLevel(0),"
            "FOVDegrees(360),ProtectSpawnRadius(50),ProtectSpawnOuterRadius(0),"
            "ProtectSpawnDuration(0),WalkToSpawnRadius(15),GriefHPRatio(0),Pet(StanceAggressive)");

        // Notify AI.
        {
            AIEventNotifyParams params;

            params.notifyType = AI_EVENT_NOTIFY_PET_CREATED;
            params.source = eCreator;
            params.target = pPet;

            aiEventNotify(&params);
        }

        // Add to encounter of parent
        EncounterEntAddPet(eCreator, pPet); 
        PlayerAddPet(eCreator, pPet); 

        SETB( pPet->seq->state, STATE_SPAWNEDASPET );

        pPet->commandablePet = 1;
        pPet->petDismissable = 1;
        pPet->petVisibility = pPet->villainDef->petVisibility;
        pPet->petName = strdup(pPet->name);

        LOG_ENT( eCreator, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "Create:ArenaPet Level %d %s (owned by %s)",
            level,
            petVillainDef,
            dbg_NameStr( eCreator ) );
    }

    return pPet;
}

void character_DestroyPetByEnt(Entity *eCreator, Entity *ePet)
{
    devassert(eCreator && ePet);
    if (eCreator && ePet)
    {
        devassert(ePet->erCreator == erGetRef(eCreator));
        if (ePet->erCreator == erGetRef(eCreator))
        {
            int delay = 0;

            LOG_ENT(eCreator, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "Die:Pet %s", dbg_NameStr(ePet));
            if (ePet->seq)
                delay = ePet->seq->type->ticksToLingerAfterDeath;

            if (ePet->pchar)
                PlayerRemovePet(eCreator, ePet);

            SET_ENTINFO(ePet).kill_me_please = 1;
        }
    }
}

void character_DestroyAllArenaPets( Character * pchar )
{
    Entity * e;
    int petCount;
    int i;

    if( !pchar ) 
        return;
    e = pchar->entParent;
    if( !e || !e->pl || !e->petList_size )
        return;

    petCount = e->petList_size;

    //Warning, when this is called during character destruction, erGetRef( e ) works, 
    //but the reverse does not.  This is a potential problem for all the functions called
    //in playerVarFree.  Martin knows about the issue, but we're not going to change playerVarFree around
    //right before CoV release, so I just avoid erGetEnt( pet->erOwner ) 

    //Only destroy arena pets
    for( i = petCount-1 ; i >= 0 ; i-- )
    {
        Entity * pet = erGetEnt(e->petList[i]);
        if( pet && pet->owner > 0 && pet->erOwner == erGetRef( e ) && pet->commandablePet && !pet->petByScript) 
        {
            LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "Die:ArenaPet %s", dbg_NameStr(pet) );

            character_DestroyPetByEnt( e, pet );
        }
    }
}

// Sort first by power, then by pet number
static int comparePetName(const PetName** ppet1, const PetName** ppet2 )
{
    int result = strcmp((*ppet1)->pchEntityDef,(*ppet2)->pchEntityDef);
    if (result) return result;
    else return (*ppet1)->petNumber-(*ppet2)->petNumber;
}

// Deprecated
char* deprecated_getNewPetNameByPower(const char *powerName, const char *entDefName, Entity *e) {
    int petCount, nameCount, i, foundUnused = -1;
    assert(e->pl != NULL);

    petCount = e->petList_size;
    nameCount = eaSize(&e->pl->petNames);

    // Look for the start of the petnames devoted to this power
    for (i = 0; i < nameCount; i++) {
        if (strcmp(e->pl->petNames[i]->DEPRECATED_pchPowerName,powerName) == 0) {
            int j, found = 0;
            for (j = 0; j < petCount; j++) {
                Entity *pet = erGetEnt(e->petList[j]);
                if (pet && pet->petName && pet->pchar->attrCur.fHitPoints > 0.f && 
                    strcmp(e->pl->petNames[i]->petName,pet->petName) == 0) //found alive match
                {
                    found = true;
                    break;
                }
            }
            if (!found) { //this name is free to be used
                strcpy(e->pl->petNames[i]->DEPRECATED_pchPowerName, "");    //    clear out deprecated name
                strcpy(e->pl->petNames[i]->pchEntityDef, entDefName);        //    use the entity def name from now on
                return e->pl->petNames[i]->petName;
            }
        }
    }

    // If we've gotten here, no petnames left for this power, return null
    return NULL;
}

// return the appropriate name for a new pet
char* getNewPetName(const char *entDefName, Entity *e) {
    int petCount, nameCount, i, foundUnused = -1;
    int entDefStart = -1;
    assert(e->pl != NULL);

    // sort by power name then pet number, so petnames are in order to be searched
    eaQSort(e->pl->petNames, comparePetName);

    petCount = e->petList_size;
    nameCount = eaSize(&e->pl->petNames);

    // Look for the start of the petnames devoted to this power
    for (i = 0; i < nameCount; i++) {
        if (strcmp(e->pl->petNames[i]->pchEntityDef,entDefName) == 0) {
            entDefStart = i;
            break;
        }
    }
    if (entDefStart > -1) { //if we found any for this ent def
        i = entDefStart;
        while (i < nameCount && strcmp(e->pl->petNames[i]->pchEntityDef,entDefName) == 0) {
            int j, found = 0;
            for (j = 0; j < petCount; j++) {
                Entity *pet = erGetEnt(e->petList[j]);
                if (pet && pet->petName && pet->pchar->attrCur.fHitPoints > 0.f && 
                    strcmp(e->pl->petNames[i]->petName,pet->petName) == 0) //found alive match
                {
                    found = true;
                    break;
                }
            }
            if (!found) { //this name is free to be used
                strcpy(e->pl->petNames[i]->DEPRECATED_pchPowerName, "");    //    clear out deprecated name if any
                return e->pl->petNames[i]->petName;
            }
            i++;
        }
    }
    // If we've gotten here, no petnames left for this power, return null
    return NULL;
}


static char *getFullPowerName(const BasePower *pow) {
    const char *pName = pow->pchName;
    const char *sName = pow->psetParent->pchName;
    const char *cName = pow->psetParent->pcatParent->pchName;
    char *buf = calloc(strlen(pName)+strlen(sName)+strlen(cName)+3,sizeof(char)); //3 = 2 .'s and a null
    sprintf(buf,"%s.%s.%s",cName,sName,pName);
    return buf;
}

static void setPetCostumeColors(Entity* pPet, ColorPair uiTint)
{
    if (uiTint.primary.a == 0 || uiTint.secondary.a == 0)
        return;

    {
        int i;
        Costume* mutable_costume = entGetMutableCostume(pPet);
        mutable_costume->appearance.colorSkin.integer = uiTint.primary.integer;
        for (i = 0; i < mutable_costume->appearance.iNumParts; ++i) {
            mutable_costume->parts[i]->color[0].integer = uiTint.primary.integer;
            mutable_costume->parts[i]->color[1].integer = uiTint.secondary.integer;
        }
    }

    pPet->ownedPowerCustomizationList = powerCustList_clone(pPet->powerCust);
}

/**********************************************************************func*
 * character_CreatePet
 *
 */
void character_CreatePet(Character *p, AttribMod *pmod)
{
    Entity *pPet;
    Entity *eCreator;
    const BasePower* creationPower;
    Power * ppow;
    F32 fLevel;

    assert(p!=NULL);
    assert(pmod!=NULL);
    assert(pmod->ptemplate!=NULL);

    if(pmod->ptemplate->pchEntityDef==NULL
        || pmod->ptemplate->pchPriorityListPassive==NULL
        || pmod->erCreated!=0)
    {
        return;
    }


    if((eCreator=erGetEnt(pmod->erSource)) == NULL)
        return;

    AI_LOG(AI_LOG_POWERS, (p->entParent, "Process :    Creating pet %s doing %s.\n",
        pmod->ptemplate->pchEntityDef, pmod->ptemplate->pchPriorityListPassive));

    if( ENTTYPE(eCreator) == ENTTYPE_PLAYER && !eCreator->initialPositionSet ) 
        return; // wait until I've been someplace before spawning pet

    if (eCreator->pchar->ppowCreatedMe)
    {
        creationPower = eCreator->pchar->ppowCreatedMe;
    }
    else
    {
        creationPower = pmod->ptemplate->ppowBase;
    }
    fLevel = pmod->fMagnitude;

    // make sure they are of the right level to have this pet here
    if( eCreator && ENTTYPE(eCreator) == ENTTYPE_PLAYER )
    {
        if(creationPower->eType != kPowerType_Boost)
        // boosts don't get checked for ownership in the same way as powers
        // if the boost's level is invalid the bonus attribute would never have been attached in the first place
        {
            ppow = character_OwnsPower(eCreator->pchar, creationPower);

            if( (!ppow && !creationPower->iNumCharges) || (ppow && !ppow->ppowBase->bIgnoreLevelBought && character_CalcCombatLevel(eCreator->pchar) + EXEMPLAR_GRACE_LEVELS < ppow->iLevelBought)) 
            {
                pmod->fDuration = 0.0; // Kill the mod
                return;
            }

            if( !ppow  ) // check for powers we used the final charge of
            {
                int i, found = 0;
                for( i = 0; i < eaSize(&eCreator->pchar->expiredPowers); i++ )
                {
                    if( eCreator->pchar->expiredPowers[i] == creationPower )
                        found = 1;
                }

                if(!found)
                {
                    pmod->fDuration = 0.0; // Kill the mod
                    return;
                }
            }

            //    happens during map transfer
            //    server needs to know the parent or else it won't kill the power properly when the toggle shuts off
            if (!pmod->parentPowerInstance)
            {
                attribmod_SetParentPowerInstance(pmod, ppow);
            }

            // We need to re-run the Application Requires in case we are now currently at the wrong Combat Level to have this pet out when map transfering
            // This currently doesn't support any of the evalFlags since we're not properly establishing which of those can be, so I've put in a devassert to trip if we have
            // a mod which has evalFlags and summons a canZone pet, which is currently unsupported by this code
            if(pmod->petFlags&PETFLAG_TRANSFER && p->entParent && pmod->ptemplate->ppchApplicationRequires &&
                combateval_Eval(p->entParent, p->entParent, pmod->parentPowerInstance, pmod->ptemplate->ppchApplicationRequires, pmod->parentPowerInstance->ppowBase->pchSourceFile)==0.0f)
            {
                devassertmsg(pmod->ptemplate->evalFlags == 0, "EntCreate of a CanZone pet doesn't currently support any special combateval operands");
                pmod->fDuration = 0.0; // Kill the mod
                return;
            }
        }

        if( pmod->ptemplate->fScale < 0  )
            fLevel = character_CalcCombatLevel(eCreator->pchar)+1;
        else
            fLevel = class_GetNamedTableValue(eCreator->pchar->pclass, pmod->ptemplate->pchTable, character_CalcCombatLevel(eCreator->pchar));
    }
    

    if( !p->entParent->erOwner && strstriConst( pmod->ptemplate->pchEntityDef, DOPPEL_NAME) )
         pPet=villainCreateDoppelganger( pmod->ptemplate->pchEntityDef + strlen(DOPPEL_NAME), 0, 0, VR_BOSS, 0, (int)fLevel, 0, 0, p->entParent, pmod->erSource, creationPower);
    else
        pPet=villainCreateByName(pmod->ptemplate->pchEntityDef, (int)fLevel, NULL, false, NULL, pmod->erSource, creationPower);

    if(pPet!=NULL && pPet->pchar != NULL)
    {
        Entity *eOwner;
        char* tmp = NULL;
        int added = 0;

        SET_ENTTYPE(pPet) = ENTTYPE_CRITTER;
        pPet->erCreator = pmod->erSource;

        if (!pmod->ptemplate->bDoNotTintCostume)
            setPetCostumeColors(pPet, pmod->uiTint);

        entSetMat3(pPet, ENTMAT(eCreator));

        if(eCreator->erOwner!=0 && !eCreator->petByScript)
        {
            pPet->erOwner = eCreator->erOwner;
        }
        else
        {
            pPet->erOwner = pmod->erSource;
        }

        // Set pet team, and note the last unmentored time of the owner/creator
        eOwner = erGetEnt(pPet->erOwner);
        if (eOwner && eOwner->pchar)
        {
            entSetOnTeam(pPet, eOwner->pchar->iAllyID); 
             if(pPet->pchar->iGangID == 0 || (g_ArchitectTaskForce && pPet->pchar->iGangID != entGangIDFromGangName("A_Gang_Never_To_Be_Used_Anywhere_Else") /*goddamn oil-slick*/ ))
                entSetOnGang(pPet, eOwner->pchar->iGangID);
            pPet->pchar->buddy.uTimeLastMentored = eOwner->pchar->buddy.uTimeLastMentored;
        }
        else
        {
            entSetOnTeam(pPet, eCreator->pchar->iAllyID);
            if(pPet->pchar->iGangID == 0 || g_ArchitectTaskForce)
                entSetOnGang(pPet, eCreator->pchar->iGangID);
            pPet->pchar->buddy.uTimeLastMentored = eCreator->pchar->buddy.uTimeLastMentored;
        }

        // Set the player pet name based on the pet names the player has set
        if (eOwner && ENTTYPE(eOwner) == ENTTYPE_PLAYER) {
            const char *entDefName = (pPet->villainDef && pPet->villainDef->name) ? pPet->villainDef->name : "none";
            tmp = getNewPetName(entDefName, eOwner);
            if (!tmp)
            {
                char *pname = getFullPowerName(pPet->pchar->ppowCreatedMe);
                tmp = deprecated_getNewPetNameByPower(pname, entDefName, eOwner);
                free(pname);
            }
        }

        if (tmp)
        { //use player pet name
            pPet->petName = strdup(tmp);
        }
        else 
        { //use default
            pPet->petName = strdup(pPet->name);
        }

        pPet->commandablePet = pPet->villainDef->petCommadability;
        pPet->petVisibility = pPet->villainDef->petVisibility;
        if( eOwner )
            pPet->petDismissable = (ENTTYPE(eOwner) == ENTTYPE_PLAYER);
        pPet->petByScript = eCreator->petByScript;
    
        // Slot all the pet's powers
        {
            int i, j;
            int iCntSets = eaSize(&pPet->pchar->ppPowerSets);
            for(i=0; i<iCntSets; i++)
            {
                PowerSet *pset = pPet->pchar->ppPowerSets[i];
                int iCntPows = eaSize(&pset->ppPowers);

                for(j=0; j<iCntPows; j++)
                {
                    character_PetSlotPower(pPet->pchar,pset->ppPowers[j]);
                }
            }
        }

        // Copy the creator's attrib mods
        if(eCreator && eCreator->pchar && pPet->pchar && pPet->villainDef)
        {
            AttribModList *pmodlist = &pPet->pchar->listMod;
            AttribModListIter iter;
            AttribMod *pmod = modlist_GetFirstMod(&iter, &eCreator->pchar->listMod);

            while(pmod!=NULL)
            {
                if(pPet->villainDef->copyCreatorMods && 
                            (pmod->ptemplate->offAspect == offsetof(CharacterAttribSet, pattrStrength)
                                || (pmod->ptemplate->offAspect == offsetof(CharacterAttribSet, pattrMod)
                                    && pmod->ptemplate->offAttrib == offsetof(CharacterAttributes, fToHit)))
                    || pmod->ptemplate->eTarget == kModTarget_CastersOwnerAndAllPets
                    || pmod->ptemplate->eTarget == kModTarget_FocusOwnerAndAllPets
                    || pmod->ptemplate->eTarget == kModTarget_AffectedsOwnerAndAllPets)
                {
                    // It's a strength mod, or current tohit copy it
                    AttribMod *pmodNew = attribmod_Create();

                    assert(pmodNew!=NULL); 

                    memcpy(pmodNew, pmod, sizeof(AttribMod));
                    pmodNew->next = 0;
                    pmodNew->prev = 0;
                    added = 1;
                    if(pmodNew->evalInfo)
                    {
                        pmodNew->evalInfo->refCount++;
                    }
                    modlist_AddMod(pmodlist, pmodNew);
                }
                pmod = modlist_GetNextMod(&iter);
            }

        }

         character_AccrueMods(pPet->pchar, 0.0, NULL);

        // They appear instantly (as opposed to fading in).
        pPet->fade = 0;
 
        // Set the initial position and give a tiny velocity to reduce
        // visual collisions.
        if(pmod->ptemplate->ppowBase->eEffectArea==kEffectArea_Location && !nearSameVec3(pmod->vecLocation, zerovec3))
        {
            entUpdatePosInstantaneous(pPet, pmod->vecLocation);
        }
        else
        {
            entUpdatePosInstantaneous(pPet, ENTPOS(p->entParent));
        }
        entGridUpdate(pPet, ENTTYPE(pPet));
        // This is a bit of defensive programming, but it's not expensive.
        // Some auto powers are marked as NearGround. However, when the Auto
        // powers were activated (within villainCreateByName) the pet's
        // location was 0,0,0 which might not be NearGround. So, now that's
        // I've placed the pet, try to activate all the powers again.
        // Design is supposed to make sure that AutoPowers are always valid
        // to go off, but this is a bit of safety just in case.
        character_ActivateAllAutoPowers(pPet->pchar);

        // Finally, set the priority list for the pet
        aiInit(pPet, NULL);
        aiBehaviorAddString(pPet, ENTAI(pPet)->base, pmod->ptemplate->pchPriorityListPassive);
        ENTAI(pPet)->doNotGoToSleep = 1; // This isn't persistent
        ENTAI(pPet)->useEnterable = 1;

        // This makes non-sleeping behavior persistent
        if ((eOwner && ENTTYPE(eOwner)==ENTTYPE_PLAYER) 
            || (eCreator && ENTTYPE(eCreator)==ENTTYPE_PLAYER))
        {
            aiBehaviorAddString(pPet,ENTAI(pPet)->base, "DoNotGoToSleep,UseEnterable");
        }

        // Remember the entity for later destruction
        pmod->erCreated = erGetRef(pPet);

        // Notify AI.
        {
            AIEventNotifyParams params;

            params.notifyType = AI_EVENT_NOTIFY_PET_CREATED;
            params.source = eCreator;
            params.target = pPet;

            aiEventNotify(&params);
        }

        // Add to encounter of parent
        EncounterEntAddPet(eCreator, pPet);
        PlayerAddPet(eCreator, pPet);

        if( (pmod->petFlags&PETFLAG_TRANSFER) )
            SET_ENTHIDE(pPet) = 1;
        else
            SETB( pPet->seq->state, STATE_SPAWNEDASPET );

        if( pmod->petFlags )
        {
            character_ApplyPetUpgrade(eCreator->pchar);

            if( pmod->petFlags&PETFLAG_TRANSFER )
            {
                entUpdatePosInstantaneous(pPet,ENTPOS(eCreator));
                entGridUpdate(pPet, ENTTYPE(pPet));
                aiBehaviorAddString(pPet, ENTAI(pPet)->base, "RunOutOfDoor");
                SET_ENTHIDE(pPet) = 1;
                pPet->draw_update = 1;
                pmod->petFlags &= ~PETFLAG_TRANSFER;
            }

        }
        LOG_ENT( eCreator, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Create:Pet Level %d %s doing %s ",
            (int)pmod->fMagnitude,
            pmod->ptemplate->pchEntityDef,
            pmod->ptemplate->pchPriorityListPassive);
    }
}

void character_BecomePet(Character *p, Entity *eNewCreator, AttribMod *pmod)
{
    assert(p!=NULL && p->entParent!=NULL);

    if(ENTTYPE(p->entParent)!=ENTTYPE_CRITTER)
        return;

    // Cleanup what you were before
    EncounterEntDetach(p->entParent);
    if(p->entParent->erCreator && p->entParent->erCreator != erGetRef(eNewCreator))
    {
        Entity *eOldCreator = erGetEnt(p->entParent->erCreator);
        PlayerRemovePet(eOldCreator, p->entParent);
        p->entParent->erCreator = 0;
        p->entParent->erOwner = 0;
    }

    SAFE_FREE(p->entParent->petName);

    // You've got that new parent smell
    if(eNewCreator!=NULL)
    {
        Entity *eNewOwner;

        p->entParent->erCreator = erGetRef(eNewCreator);
        eNewOwner = (eNewCreator->erOwner) ? erGetEnt(eNewCreator->erOwner) : eNewCreator;
        p->entParent->erOwner = erGetRef(eNewOwner);

        if(eNewOwner->pchar)
        {
            entSetOnTeam(p->entParent, eNewOwner->pchar->iAllyID);
            entSetOnGang(p->entParent, eNewOwner->pchar->iGangID);
            p->buddy.uTimeLastMentored = eNewOwner->pchar->buddy.uTimeLastMentored;
        }

        ENTAI(p->entParent)->doNotGoToSleep = 1; // This isn't persistent
        ENTAI(p->entParent)->useEnterable = 1;

        // This makes non-sleeping behavior persistent
        if ((eNewOwner && ENTTYPE(eNewOwner)==ENTTYPE_PLAYER) 
            || (eNewCreator && ENTTYPE(eNewCreator)==ENTTYPE_PLAYER))
        {
            aiBehaviorAddString(p->entParent,ENTAI(p->entParent)->base, "DoNotGoToSleep,UseEnterable");
        }

        EncounterEntAddPet(eNewCreator, p->entParent);
        PlayerAddPet(eNewCreator, p->entParent);

        p->entParent->commandablePet = p->entParent->villainDef->petCommadability;
        p->entParent->petVisibility = p->entParent->villainDef->petVisibility;
        p->entParent->petDismissable = 0;
        p->entParent->petByScript = 1;
    }

    if(pmod)
    {
        if (eNewCreator->pchar->ppowCreatedMe)
        {
            p->ppowCreatedMe = eNewCreator->pchar->ppowCreatedMe;
        }
        else
        {
            p->ppowCreatedMe = pmod->ptemplate->ppowBase;
        }
        // Change AI?
    }

    p->entParent->petinfo_update = 1;
}

void character_DestroyPet(Character *pCreator, AttribMod *pmod)
{
    Entity *ePet;

    assert(pCreator != NULL);
    assert(pmod != NULL);

    if (pmod->erCreated != 0)
    {
        if ((ePet = erGetEnt(pmod->erCreated)) != NULL)
        {
            int delay;

            LOG_ENT(pCreator->entParent, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "Die:Pet %s", ePet->name);
            if (ePet->seq)
                delay = ePet->seq->type->ticksToLingerAfterDeath;
            else
                delay = 0;

            PlayerRemovePet(pCreator->entParent, ePet);

            if (pmod->ptemplate->bVanishEntOnTimeout)
            {
                SET_ENTINFO(ePet).kill_me_please = 1;
            }
            else
            {
                dieNow(ePet, kDeathBy_HitPoints, false, delay);
            }
        }

        pmod->erCreated = 0;
    }
}


/**********************************************************************func*
 * character_DestroyAllFollowerPets
 * Destroy all pets created by character with self target
 */

void character_DestroyAllFollowerPets(Character *p)
{
    AttribModListIter iter;
    AttribMod *pmod = modlist_GetFirstMod(&iter, &p->listMod);
    while(pmod!=NULL)
    {
        if (pmod->ptemplate->offAttrib == kSpecialAttrib_EntCreate)
        {
            if(pmod->erCreated!=0)
            {
                Entity *pent = erGetEnt(pmod->erCreated);
                Entity *powner;
                if (!pent || pent->seq->type->notselectable) {
                    pmod = modlist_GetNextMod(&iter);
                    continue; //Skip fake pets                
                }
                powner = erGetEnt(pent->erOwner);
                if (!powner || powner->owner != p->entParent->owner) {
                    pmod = modlist_GetNextMod(&iter);
                    continue; //Skip others pets
                }
                character_DestroyPet(p, pmod);
            }

            // Mark this attribmod to be removed next time through and don't
            //   let it get turned on again.
            pmod->fDuration = 0;
            pmod->fTimer = 100; // abitrary, must be bigger than the lkely real tick time.
        }
        pmod = modlist_GetNextMod(&iter);
    }
}

/***********************************************************************
*
*/
int character_IsAFollowerPet(Character *p, Entity *pet)
{
    AttribModListIter iter;
    AttribMod *pmod = modlist_GetFirstMod(&iter, &p->listMod);
    while(pmod!=NULL)
    {
        if (pmod->ptemplate->offAttrib == kSpecialAttrib_EntCreate)
        {
            if(pmod->erCreated!=0)
            {
                Entity *pent = erGetEnt(pmod->erCreated);
                Entity *powner;


                if (!pent || pent->seq->type->notselectable) 
                {
                    pmod = modlist_GetNextMod(&iter);
                    continue; //Skip fake pets                
                }

                if( pent != pet )
                    continue; // not the pet we are interested in

                powner = erGetEnt(pent->erOwner);
                if (!powner || powner->owner != p->entParent->owner) 
                {
                    pmod = modlist_GetNextMod(&iter);
                    continue; //Skip others pets
                }

                return 1;
            }
        }
        pmod = modlist_GetNextMod(&iter);
    }

    return 0;
}

/**********************************************************************func*
* character_DestroyAllPets
* Destroy all pet creations targeted to character, even if cast by enemies
*/
void character_DestroyAllPets(Character *p)
{
    AttribModListIter iter;
    AttribMod *pmod = modlist_GetFirstMod(&iter, &p->listMod);
    while(pmod!=NULL)
    {
        if (pmod->ptemplate->offAttrib == kSpecialAttrib_EntCreate)
        {
            if(pmod->erCreated!=0)
            {
                character_DestroyPet(p, pmod);
            }

            // Mark this attribmod to be removed next time through and don't
            //   let it get turned on again.
            pmod->fDuration = 0;
            pmod->fTimer = 100; // abitrary, must be bigger than the lkely real tick time.
        }
        pmod = modlist_GetNextMod(&iter);
    }
}

void character_DestroyAllPetsEvenWithoutAttribMod(Character *p)
{
    int i;

    character_DestroyAllPets(p);
    //    Certain pets (soul extraction) are no longer on the attribmod list
    //    so they will not be killed by the destroy all pets call (this is intended behavior, since these pets are meant
    //    to stick around even after death. However they cannot persist for the level change and must be killed
    if (p && p->entParent)
    {
        Entity *e = p->entParent;
        for (i = (e->petList_size-1); i >= 0; --i)
        {
            Entity *pet = erGetEnt(e->petList[i]);
            if (pet && (pet->erCreator == erGetRef(e)) && !pet->petByScript)
            {
                character_DestroyPetByEnt(e, pet);
            }
        }
    }
}

/**********************************************************************func*
* character_DestroyAllPetsAtDeath
* Destroy all pet creations targeted to character, even if cast by enemies, unless marked KeepThroughDeath
*/
void character_DestroyAllPetsAtDeath(Character *p)
{
    AttribModListIter iter;
    AttribMod *pmod = modlist_GetFirstMod(&iter, &p->listMod);
    while(pmod!=NULL)
    {
        if (pmod->ptemplate->offAttrib == kSpecialAttrib_EntCreate && !pmod->ptemplate->bKeepThroughDeath)
        {
            if(pmod->erCreated!=0)
            {
                character_DestroyPet(p, pmod);
            }

            // Mark this attribmod to be removed next time through and don't
            //   let it get turned on again.
            pmod->fDuration = 0;
            pmod->fTimer = 100; // abitrary, must be bigger than the lkely real tick time.
        }
        pmod = modlist_GetNextMod(&iter);
    }
}


void character_UpdateAllPetAllyGang(Character *p)
{
    AttribModListIter iter;
    AttribMod *pmod = modlist_GetFirstMod(&iter, &p->listMod);
    while(pmod!=NULL)
    {
        if (pmod->ptemplate->offAttrib == kSpecialAttrib_EntCreate)
        {
            Entity *pent;
            if(pmod->erCreated && ((pent = erGetEnt(pmod->erCreated))!=NULL) )
            {
                AIVars* ai = ENTAI(pent);
                entSetOnTeam(pent, p->iAllyID );
                ai->originalAllyID = p->iAllyID;
                entSetOnGang(pent, p->iGangID );
            }
        }
        pmod = modlist_GetNextMod(&iter);
    }
}

void character_AllPetsRunOutOfDoor(Character *p, Vec3 position, MapTransferType xfer )
{
    AttribModListIter iter;
    Entity *pent;
    AttribMod *pmod = modlist_GetFirstMod(&iter, &p->listMod);
    while(pmod!=NULL)  
    {
        if (pmod->ptemplate->offAttrib == kSpecialAttrib_EntCreate)
        {
            if((pent = erGetEnt(pmod->erCreated))!=NULL)
            {
                if( !(xfer&XFER_STATIC) )
                {
                    if( pmod->petFlags&PETFLAG_TRANSFER )
                    {
                        entUpdatePosInstantaneous(pent,position);
                        entGridUpdate(pent, ENTTYPE(pent));
                        aiBehaviorAddString(pent, ENTAI(pent)->base, "RunOutOfDoor");
                        SET_ENTHIDE(pent) = 1;
                        pent->draw_update = 1;
                        pmod->petFlags &= ~PETFLAG_TRANSFER;
                    }
                }
                else
                {
                    pmod->petFlags&= ~(PETFLAG_TRANSFER); 
                }
            }
        }
        pmod = modlist_GetNextMod(&iter);
    }
}

void character_AllPetsRunIntoDoor(Character *p)
{
    AttribModListIter iter;
    Entity *pent;
    AttribMod *pmod = modlist_GetFirstMod(&iter, &p->listMod);
    while(pmod!=NULL)
    {
        if (pmod->ptemplate->offAttrib == kSpecialAttrib_EntCreate)
        {
            if((pent = erGetEnt(pmod->erCreated))!=NULL)
                aiBehaviorAddString(pent, ENTAI(pent)->base, "Clean,RunIntoDoor(Invincible),Invisible,DestroyMe");
        }
        pmod = modlist_GetNextMod(&iter);
    }
}

void character_PetsUpgraded(Character *p, int upgrade)
{
    AttribModListIter iter;
    Entity *pent;
    AttribMod *pmod = modlist_GetFirstMod(&iter, &p->listMod);
    while(pmod!=NULL)
    {
        if (pmod->ptemplate->offAttrib == kSpecialAttrib_EntCreate)
        {
            if((pent = erGetEnt(pmod->erCreated))!=NULL)
                pmod->petFlags |= upgrade;
        }
        pmod = modlist_GetNextMod(&iter);
    }
}

void character_ApplyPetUpgrade(Character *p)
{
    AttribModListIter iter;
    Entity *pent;
    AttribMod *pmod = modlist_GetFirstMod(&iter, &p->listMod);
    Power * upgrade1=0, *upgrade2=0;
    int i, j, k;

    // find the upgrade powers
    for( i = eaSize(&p->ppPowerSets)-1; i>=0; i-- )
    {
        for( j = eaSize(&p->ppPowerSets[i]->ppPowers)-1; j>=0; j-- )
        {
            if( p->ppPowerSets[i]->ppPowers[j] )
            {
                for( k = eaSize(&p->ppPowerSets[i]->ppPowers[j]->ppowBase->ppchAIGroups)-1; k>=0; k-- )
                {
                    if( stricmp( p->ppPowerSets[i]->ppPowers[j]->ppowBase->ppchAIGroups[k], "kMastermindUpgrade1") == 0)
                        upgrade1 = p->ppPowerSets[i]->ppPowers[j];
                    if( stricmp( p->ppPowerSets[i]->ppPowers[j]->ppowBase->ppchAIGroups[k], "kMastermindUpgrade2") == 0 )
                        upgrade2 = p->ppPowerSets[i]->ppPowers[j];
                }
            }
        }
    }

    // apply them to all pets
    while(pmod!=NULL)
    {
        if (pmod->ptemplate->offAttrib == kSpecialAttrib_EntCreate)
        {
            if((pent = erGetEnt(pmod->erCreated))!=NULL)
            {
                if(upgrade1 && pmod->petFlags&PETFLAG_UPGRADE1)
                {
                    if(upgrade1->ppowBase->bIgnoreLevelBought || character_CalcCombatLevel(p) + EXEMPLAR_GRACE_LEVELS >= upgrade1->iLevelBought)
                        character_ApplyPower(p,upgrade1,pmod->erCreated, p->vecLocationCurrent, 0, 1);
                }
                if(upgrade2 && pmod->petFlags&PETFLAG_UPGRADE2)
                {
                    if(upgrade2->ppowBase->bIgnoreLevelBought || character_CalcCombatLevel(p) + EXEMPLAR_GRACE_LEVELS >= upgrade2->iLevelBought)
                        character_ApplyPower(p,upgrade2,pmod->erCreated, p->vecLocationCurrent, 0, 1);
                }
            }
        }
        pmod = modlist_GetNextMod(&iter);
    }
}


/**********************************************************************func*
 * character_DestroyAllPendingPets
 *
 */
void character_DestroyAllPendingPets(Character *p)
{
    AttribModListIter iter;
    AttribMod *pmod = modlist_GetFirstMod(&iter, &p->listMod);
    while(pmod!=NULL)
    {
        if (pmod->ptemplate->offAttrib == kSpecialAttrib_EntCreate && pmod->erCreated == 0)
        {
            // Mark this attribmod to be removed next time through and don't
            //   let it get turned on again.
            pmod->fDuration = 0;
            pmod->fTimer = 100; // abitrary, must be bigger than the lkely real tick time.
        }
        pmod = modlist_GetNextMod(&iter);
    }
}

static int s_AllPetsDead(Character *pchar, const BasePower *bpow)
{
    AttribMod *pmod;
    AttribModListIter iter;
    pmod = modlist_GetFirstMod(&iter, &pchar->listMod); 
    while(pmod!=NULL)
    {
        if( pmod->fDuration > 0.0f && pmod->ptemplate->ppowBase == bpow && pmod->ptemplate->offAttrib == kSpecialAttrib_EntCreate )
        {
            return false;
        }
        pmod = modlist_GetNextMod(&iter);
    }

    return true;
}

/**********************************************************************func*
* character_PetClearParentMods
*
*/
void character_PetClearParentMods(Character *p)
{
    Entity *parent;
    AttribMod *pmod;
    AttribModListIter iter;

    if( !p || !p->entParent || !p->entParent->erOwner )
        return;

    parent = erGetEnt(p->entParent->erOwner);
    if(!parent)
        return;

    // Look for EntCreate mod
    pmod = modlist_GetFirstMod(&iter, &parent->pchar->listMod); 
    while(pmod!=NULL)
    {
        if( pmod->erCreated && pmod->ptemplate->offAttrib == kSpecialAttrib_EntCreate )
        {
            Entity * createdEnt = erGetEnt( pmod->erCreated );

            // now make sure the EntCreate mod is for this entity
            if( createdEnt == p->entParent ) 
            {
                // essentially cancel mod (we can't do it for real because we are already in mod loop)
                pmod->fDuration = 0.0f;

                if (pmod->ptemplate->ppowBase->eType == kPowerType_Toggle && parent->pchar)
                {
                    if (s_AllPetsDead(parent->pchar, pmod->ptemplate->ppowBase))
                    {
                        PowerListItem *ppowListItem;
                        PowerListIter ppowiter;

                        pmod->erCreated = 0;

                        for(ppowListItem = powlist_GetFirst(&parent->pchar->listTogglePowers, &ppowiter);
                            ppowListItem != NULL;
                            ppowListItem = powlist_GetNext(&ppowiter))
                        {
                            if (ppowListItem->ppow->ppowBase == pmod->ptemplate->ppowBase)
                            {
                                if (ppowListItem->ppow->bActive)
                                {
                                    character_ShutOffTogglePower(parent->pchar, ppowListItem->ppow);
                                    powlist_RemoveCurrent(&ppowiter);
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
        pmod = modlist_GetNextMod(&iter);
    }
}

/**********************************************************************func*
 * character_PetSlotPower
 * Slots the power based on the current slotting of the power that created the pet
 */
void character_PetSlotPower(Character *p, Power *ppow)
{
    Entity *eOwner;
    Power *ppowSrc;
    int iSrcExpLevel;

    if( !p || !p->entParent || !p->entParent->erOwner || !p->ppowCreatedMe)
        return;

    eOwner = erGetEnt(p->entParent->erOwner);
    if(!eOwner || !eOwner->pchar)
        return;

    ppowSrc = character_OwnsPower(eOwner->pchar, p->ppowCreatedMe);

    if(!ppowSrc) 
        return;

    iSrcExpLevel = character_CalcExperienceLevel(eOwner->pchar);
    character_SlotPowerFromSource(ppow, ppowSrc, p->iCombatLevel-iSrcExpLevel);

}

/* End of File */
