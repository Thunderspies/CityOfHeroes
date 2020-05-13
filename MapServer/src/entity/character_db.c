/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>


#include <utilitieslib/utils/utils.h>
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "entGameActions.h"
#include "container/dbcontainerpack.h"

#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/character_eval.h"
#include "character_combat.h"
#include "entity/classes.h"
#include "entity/origins.h"
#include "entity/powers.h"
#include <utilitieslib/components/earray.h>
#include "dbcomm/dbcomm.h"
#include "entity/motion.h"

#include "character_db.h"
#include "entity/character_mods.h"
#include "dbghelper.h"
#include <utilitieslib/utils/error.h>
#include "cmdparse/cmdserver.h"
#include "gameComm/svr_chat.h"
#include "container/containerloadsave.h"
#include "comm_backend.h"
#include "gameSys/arenamap.h"
#include "dbcomm/staticMapInfo.h"
#include "character_pet.h"
#include "seq/seq.h"
#include "alignment_shift.h"
#include <utilitieslib/utils/log.h>
#include "dbcomm/logcomm.h"

#if (POWER_VAR_MAX_COUNT!=MAX_DB_POWER_VARS)
#error POWER_VAR_MAX_COUNT is not the same as MAX_DB_POWER_VARS!
#endif


/**********************************************************************func*
 * CharacterClassToAttr
 *
 */
const char *CharacterClassToAttr(intptr_t num)
{
    const CharacterClass *pclass = (const CharacterClass *)num;
    return pclass->pchName;
}

/**********************************************************************func*
 * AttrToCharacterClass
 *
 */
intptr_t AttrToCharacterClass(const char *pch)
{
    intptr_t iRet = (intptr_t)classes_GetPtrFromName(&g_CharacterClasses, pch);
    if(!iRet) iRet = (intptr_t)classes_GetPtrFromName(&g_VillainClasses, pch);

    return iRet;
}

/**********************************************************************func*
 * *CharacterOriginToAttr
 *
 */
const char *CharacterOriginToAttr(intptr_t num)
{
    const CharacterOrigin *porigin = (const CharacterOrigin *)num;
    return porigin->pchName;
}

/**********************************************************************func*
 * AttrToCharacterOrigins
 *
 */
intptr_t AttrToCharacterOrigin(const char *pch)
{
    intptr_t iRet = (intptr_t)origins_GetPtrFromName(&g_CharacterOrigins, pch);
    if(!iRet) iRet = (intptr_t)origins_GetPtrFromName(&g_VillainOrigins, pch);

    return iRet;
}

/**********************************************************************func*
 * dbpow_GetBasePower
 *
 */
static const BasePower *dbpow_GetBasePower(DBBasePower *pdbpow)
{
    const char *newSetName = power_GetReplacementPowerSetName(pdbpow->pchPowerSetName);
    const char *newPowerName = power_GetReplacementPowerName(pdbpow->pchPowerName);

    return powerdict_GetBasePowerByName(&g_PowerDictionary,
        pdbpow->pchCategoryName,
        newSetName ? newSetName : pdbpow->pchPowerSetName,
        newPowerName ? newPowerName : pdbpow->pchPowerName);
}

/**********************************************************************func*
 * dbpow_GetBasePowerError
 *
 */
char *dbpow_GetBasePowerError(DBBasePower *pdbpow)
{
    const char *newSetName = power_GetReplacementPowerSetName(pdbpow->pchPowerSetName);
    const char *newPowerName = power_GetReplacementPowerName(pdbpow->pchPowerName);

    return powerdict_GetBasePowerByNameError(&g_PowerDictionary,
        pdbpow->pchCategoryName,
        newSetName ? newSetName : pdbpow->pchPowerSetName,
        newPowerName ? newPowerName : pdbpow->pchPowerName);
}


/**********************************************************************func*
 * dbrewardslot_Fill
 *
 */
static void dbrewardslot_Fill(DBRewardSlot *p, RewardItemType type, char const *name)
{
    if( verify( p && name && rewarditemtype_Valid( type ) ))
    {
        p->type = type;
        p->id = rewarditemtype_IdFromName(type, name);
    }
}

/**********************************************************************func*
 * setDbPowerupSlots
 *
 * add the powerup slots to the db
 * aPowerupSlots: must have the same capacity as Power::aPowerupSlots
 * resMask: sets the bit in this if filled.
 *
 */
static void setDbPowerupSlots(DBRewardSlot *resSlots, int *resMask, PowerupSlot *aPowerupSlots, int size)
{
    int filled = 0;

    if( verify( resSlots && aPowerupSlots && resMask ))
    {
        int slot;

        // clear the passed array
        ZeroStructs( resSlots, TYPE_ARRAY_SIZE(Power, aPowerupSlots));

        // clear the mask
        *resMask = 0;

        // fill it
        filled = 0;
        for( slot = 0; slot < size ; ++slot )
        {
            PowerupSlot *pslot = &aPowerupSlots[slot];

            // get the reward
            if( powerupslot_Valid( pslot ))
            {
                // set the filled bit
                pslot->filled ? SETB(resMask, filled) : CLRB(resMask, filled);

                // fill the db slot
                dbrewardslot_Fill( &resSlots[filled], pslot->reward.type, pslot->reward.name );

                // increment
                filled++;
            }
        }
    }
}


STATIC_ASSERT(ARRAY_SIZE(((DBBoost*)0)->aPowerupSlots) == ARRAY_SIZE(((DBBoost*)0)->aPowerupSlots));

/**********************************************************************func*
 * setPowerupSlotsFromDB
 *
 */
static int setPowerupSlotsFromDB(PowerupSlot *resPowerupSlots, int mask, DBRewardSlot *aDbRewardSlots, int size)
{
    int filled = 0;

    if( verify( resPowerupSlots && aDbRewardSlots ))
    {
        int slot;
        for( slot = 0; slot < size ; ++slot )
        {
            if( dbrewardslot_Valid( &aDbRewardSlots[slot] ) )
            {
                PowerupSlot *pslot = &resPowerupSlots[filled];

                // set the filled
                pslot->filled = TSTB(&mask,slot) && 1;

                // set the reward
                pslot->reward.name = rewarditemtype_NameFromId( aDbRewardSlots[slot].type, aDbRewardSlots[slot].id );
                pslot->reward.type = aDbRewardSlots[slot].type;

                // increment if it is a valid powerup
                if( verify(pslot->reward.name) )
                {
                    ++filled;
                }
            }
        }
    }

    return filled;
}



/**********************************************************************func*
 * convertPowerBoosts
 *
 */
static int convertPowerBoosts(Power *ppow, DBBoost *pboost, int iID)
{
    int iCntBoosts = 0;
    int i;
    int iSize = eaSize(&ppow->ppBoosts);

    for(i=0; i<iSize; i++)
    {
        char *pchCat;
        char *pchSet;
        char *pchPow;

        if(ppow->ppBoosts[i]!=NULL && ppow->ppBoosts[i]->ppowBase!=NULL)
        {
            basepower_GetNames(ppow->ppBoosts[i]->ppowBase, &pchCat, &pchSet, &pchPow);

            pboost[iCntBoosts].dbpowBase.pchCategoryName = pchCat;
            pboost[iCntBoosts].dbpowBase.pchPowerSetName = pchSet;
            pboost[iCntBoosts].dbpowBase.pchPowerName = pchPow;
            pboost[iCntBoosts].iIdx = i;
            pboost[iCntBoosts].iLevel = ppow->ppBoosts[i]->iLevel;
            pboost[iCntBoosts].iNumCombines = ppow->ppBoosts[i]->iNumCombines;
            pboost[iCntBoosts].iID = iID;

            memcpy(pboost[iCntBoosts].afVars,
                ppow->ppBoosts[i]->ppowParent->afVars,
                sizeof(pboost[iCntBoosts].afVars));

            // copy the powerup slot
            setDbPowerupSlots( pboost[iCntBoosts].aPowerupSlots, &pboost[iCntBoosts].iSlottedPowerupsMask, ppow->ppBoosts[i]->aPowerupSlots, ARRAY_SIZE(ppow->ppBoosts[i]->aPowerupSlots));

            iCntBoosts++;
        }
    }

    return iCntBoosts;
}

/**********************************************************************func*
 * convertInventoryBoosts
 *
 */
static int convertInventoryBoosts(Character *p, DBBoost *pboost)
{
    int iCntBoosts = 0;
    int i;

    for(i=0; i<CHAR_BOOST_MAX; i++)
    {
        char *pchCat;
        char *pchSet;
        char *pchPow;

        // TODO: this is for the temporary definition of a boost

        if(p->aBoosts[i]!=NULL && p->aBoosts[i]->ppowBase!=NULL)
        {
            basepower_GetNames(p->aBoosts[i]->ppowBase, &pchCat, &pchSet, &pchPow);

            pboost[iCntBoosts].dbpowBase.pchCategoryName = pchCat;
            pboost[iCntBoosts].dbpowBase.pchPowerSetName = pchSet;
            pboost[iCntBoosts].dbpowBase.pchPowerName = pchPow;
            pboost[iCntBoosts].iIdx = i;
            pboost[iCntBoosts].iLevel = p->aBoosts[i]->iLevel;
            pboost[iCntBoosts].iNumCombines = p->aBoosts[i]->iNumCombines;
            pboost[iCntBoosts].iID = -1;

            memcpy(pboost[iCntBoosts].afVars,
                p->aBoosts[i]->afVars,
                sizeof(pboost[iCntBoosts].afVars));

            // copy the powerup slot
            setDbPowerupSlots(pboost[iCntBoosts].aPowerupSlots, &pboost[iCntBoosts].iSlottedPowerupsMask, p->aBoosts[i]->aPowerupSlots, ARRAY_SIZE(p->aBoosts[i]->aPowerupSlots));

            iCntBoosts++;
        }
    }

    return iCntBoosts;
}

/**********************************************************************func*
 * convertInspirations
 *
 */
static int convertInspirations(const BasePower *aInspirations[CHAR_INSP_MAX_COLS][CHAR_INSP_MAX_ROWS],
    DBPowers *pdb)
{
    int iCnt = 0;
    int i, j;

    for(i=0; i<CHAR_INSP_MAX_COLS && iCnt<MAX_DB_INSPIRATIONS; i++)
    {
        for(j=0; j<CHAR_INSP_MAX_ROWS && iCnt<MAX_DB_INSPIRATIONS; j++)
        {
            if(aInspirations[i][j]!=NULL)
            {
                char *pchCat;
                char *pchSet;
                char *pchPow;

                basepower_GetNames(aInspirations[i][j], &pchCat, &pchSet, &pchPow);

                pdb->inspirations[iCnt].iCol = i;
                pdb->inspirations[iCnt].iRow = j;
                pdb->inspirations[iCnt].dbpowBase.pchCategoryName = pchCat;
                pdb->inspirations[iCnt].dbpowBase.pchPowerSetName = pchSet;
                pdb->inspirations[iCnt].dbpowBase.pchPowerName = pchPow;

                iCnt++;
            }
        }
    }

    return iCnt;
}

/**********************************************************************func*
 * convertAttribMods
 *
 */
int convertAttribMods(Character *p, DBPowers *pdb)
{
    int iCnt = 0;
    AttribMod *pmod;
    AttribModListIter iter;

    pmod = modlist_GetFirstMod(&iter, &p->listMod);
    while(pmod!=NULL && iCnt<MAX_DB_ATTRIB_MODS)
    {
        Entity *eSrc = erGetEnt(pmod->erSource);
        int skip = 0;
        char *pchCat;
        char *pchSet;
        char *pchPow;

        // If this is a power which requires confirmation, don't
        //   save it. (This will cancel it.)
        if( !(0 == p->entParent->pl->uiDelayedInvokeID || pmod->uiInvokeID != p->entParent->pl->uiDelayedInvokeID) )
            skip = 1;

            
        // Don't save:
        //        If it's a taunt or placate attrib.
        //        confuse  (people leaving to Atlas Park to grief)
        if(    pmod->fMagnitude  && (pmod->ptemplate->offAttrib == offsetof(CharacterAttributes, fTaunt)
                                    || pmod->ptemplate->offAttrib == offsetof(CharacterAttributes, fPlacate)
                                    || pmod->ptemplate->offAttrib == offsetof(CharacterAttributes, fConfused)))
        {
            skip = 1;
        }

        // If its pet thats not commandable or dismissable
        if( pmod->ptemplate->offAttrib == kSpecialAttrib_EntCreate && pmod->erCreated)
        {
            const VillainDef* def = villainFindByName(pmod->ptemplate->pchEntityDef);
            if( !def || !def->canZone ) // check that its been explicitly allowed to zone
                skip = 1;
            else
            {
                Entity *pet = erGetEnt(pmod->erCreated);
                if( pet ) // If the map transfer took too long, then the pet may have been killed, so we are relying entirely on the canzone flag.
                {
                    if(pet->seq->type->notselectable || !(pet->commandablePet || pet->petDismissable)) // check that its really a pet
                        skip = 1;
                    else
                    {
                        Entity *powner = erGetEnt(pet->erOwner);
                        if (!powner || powner->owner != p->entParent->owner) // check that I'm really the owner
                            skip = 1;    
                    }
                }
            }
        }
        else if (eSrc == p->entParent
                && (pmod->ptemplate->ppowBase->eType == kPowerType_Auto || pmod->ptemplate->ppowBase->eType == kPowerType_Toggle)
                && pmod->ptemplate->eApplicationType == kModApplicationType_OnTick)
        {
            // If it's on ourselves and it's an auto or toggle power, (and not a pet power)
            //   don't bother. Autos and toggles will restart
            //   automatically.
            skip = 1;
        }

        if(!skip)
        {
            basepower_GetNames(pmod->ptemplate->ppowBase, &pchCat, &pchSet, &pchPow);
            pdb->attribmods[iCnt].dbpowBase.pchCategoryName = pchCat;
            pdb->attribmods[iCnt].dbpowBase.pchPowerSetName = pchSet;
            pdb->attribmods[iCnt].dbpowBase.pchPowerName = pchPow;
            pdb->attribmods[iCnt].iIdxTemplate = pmod->ptemplate->iIdx;

            pdb->attribmods[iCnt].fDuration = pmod->fDuration*100.0f;
            pdb->attribmods[iCnt].fMagnitude = pmod->fMagnitude*100.0f;
            pdb->attribmods[iCnt].fTimer = pmod->fTimer*100.0f;
            pdb->attribmods[iCnt].fChance = pmod->fChance*100.0f;
            pdb->attribmods[iCnt].UID = pmod->uiInvokeID;
            pdb->attribmods[iCnt].petFlags = pmod->petFlags;
            pdb->attribmods[iCnt].customFXToken = pmod->customFXToken;
            pdb->attribmods[iCnt].uiTint = pmod->uiTint;
            pdb->attribmods[iCnt].bSuppressedByStacking = pmod->bSuppressedByStacking;
            iCnt++;
        }

        pmod = modlist_GetNextMod(&iter);
    }

    return iCnt;
}

/**********************************************************************func*
 * GetRechargeTime
 *
 */
static U32 GetRechargeTime(Character *p, Power *ppow)
{
    PowerListItem *ppowListItem;
    PowerListIter iter;

    if(p->ppowCurrent == ppow
        && p->bCurrentPowerActivated
        && ppow->ppowBase->eType==kPowerType_Click)
    {
        return ppow->ppowBase->fRechargeTime + ppow->fTimer;
    }

    // Recharge powers recently used.
    for(ppowListItem = powlist_GetFirst(&p->listRechargingPowers, &iter);
        ppowListItem != NULL;
        ppowListItem = powlist_GetNext(&iter))
    {
        if(ppowListItem->ppow == ppow)
        {
            return ppowListItem->ppow->fTimer;
        }
    }

    return 0;
}

DBPowers s_dbpows;

/**********************************************************************func*
 * packageEntPowers
 *
 */
void packageEntPowers(Entity *e, StuffBuff *psb, StructDesc *desc)
{
    int i;
    int j;
    int k;
    int l;
    int iCurBuild;
    int iSizeBuilds;
    int iSizeSets;
    int iCntPowers = 0;
    int iCntBoosts = 0;
    int iCntInspirations = 0;
    int iCntAttribMods = 0;
    U32 ulNow = dbSecondsSince2000();

    memset(&s_dbpows, 0, sizeof(DBPowers));

    iCurBuild = character_GetActiveBuild(e);
    iSizeBuilds = e->pchar && ENTTYPE(e) == ENTTYPE_PLAYER ? MAX_BUILD_NUM : 1;
    for (l = 0; l < iSizeBuilds; l++)
    {
        character_SelectBuildLightweight(e, l);
        iSizeSets = eaSize(&e->pchar->ppPowerSets);
        for(j = l == 0 ? 0 : g_numSharedPowersets; j < iSizeSets; j++)
        {
            PowerSet *pset = e->pchar->ppPowerSets[j];
            int iSizePows = eaSize(&pset->ppPowers);
            for(k=0; k<iSizePows; k++)
            {
                Power *ppow = pset->ppPowers[k];
                if (!ppow)
                    continue;

                assert(iCntPowers<MAX_POWERS);

                ppow->ppowBase = ppow->ppowOriginal;
                
                //    all redirected powers should return to the original power, which should belong to the powerset the player owns.
                //    if this isn't true, server is saving a redirected power into the db. -EL
                devassertmsg(ppow->ppowBase->psetParent == pset->psetBase, "Redirected power being saved to the DB. Alert a programmer");

                if(!ppow->ppowBase->bDoNotSave && ppow->ppowBase->psetParent == pset->psetBase)
                {
                    //    Use the ppow base set and base category instead of the existing pset pointer since the power might have been redirected
                    s_dbpows.powers[iCntPowers].dbpowBase.pchCategoryName = ppow->psetParent->psetBase->pcatParent->pchName;
                    s_dbpows.powers[iCntPowers].dbpowBase.pchPowerSetName = ppow->psetParent->psetBase->pchName;
                    s_dbpows.powers[iCntPowers].dbpowBase.pchPowerName = ppow->ppowBase->pchName;
                    s_dbpows.powers[iCntPowers].iUniqueID = ppow->iUniqueID;

                    s_dbpows.powers[iCntPowers].iPowerSetLevelBought = ppow->psetParent->iLevelBought;
                    s_dbpows.powers[iCntPowers].iLevelBought = ppow->iLevelBought;
                    s_dbpows.powers[iCntPowers].iNumBoostsBought = ppow->iNumBoostsBought;
                    s_dbpows.powers[iCntPowers].iNumCharges = ppow->iNumCharges;
                    s_dbpows.powers[iCntPowers].fUsageTime = ppow->fUsageTime;
                    s_dbpows.powers[iCntPowers].fAvailableTime = ppow->fAvailableTime;
                    s_dbpows.powers[iCntPowers].ulCreationTime = ppow->ulCreationTime;
                    s_dbpows.powers[iCntPowers].ulRechargedAt = GetRechargeTime(e->pchar, ppow);
                    s_dbpows.powers[iCntPowers].iBuildNum = l;
                    s_dbpows.powers[iCntPowers].bDisabled = ppow->bDisabledManually;
                    s_dbpows.powers[iCntPowers].instanceBought = ppow->instanceBought;
                    if(s_dbpows.powers[iCntPowers].ulRechargedAt>0)
                    {
                        s_dbpows.powers[iCntPowers].ulRechargedAt+=ulNow;
                    }

                    s_dbpows.powers[iCntPowers].iID = iCntPowers+1; // can't be zero because of SQL.

                    memcpy(s_dbpows.powers[iCntPowers].afVars,
                        ppow->afVars,
                        sizeof(s_dbpows.powers[iCntPowers].afVars));

                    // copy the powerup slots
                    {
                        int numPowerupSlots = powerupslot_ValidCount( ppow->aPowerupSlots, ARRAY_SIZE(ppow->aPowerupSlots) );
                        setDbPowerupSlots( s_dbpows.powers[iCntPowers].aPowerupSlots,
                            &s_dbpows.powers[iCntPowers].iSlottedPowerupsMask,
                            ppow->aPowerupSlots, numPowerupSlots );
                    }

                    if(ppow->bActive && ppow->ppowBase->eType==kPowerType_Toggle )
                    {
                        s_dbpows.powers[iCntPowers].bActive = 1;
                    }
                    else
                    {
                        s_dbpows.powers[iCntPowers].bActive = 0;
                    }

                    if(ppow == e->pchar->ppowDefault)
                    {
                        s_dbpows.powers[iCntPowers].bActive |= 2;
                    }

                    iCntBoosts += convertPowerBoosts(ppow, &s_dbpows.boosts[iCntBoosts],
                        s_dbpows.powers[iCntPowers].iID);

                    assert(iCntBoosts<MAX_DB_BOOSTS);

                    iCntPowers++;
                }
            }
        }
    }

    character_SelectBuildLightweight(e, iCurBuild);

    iCntBoosts += convertInventoryBoosts(e->pchar, &s_dbpows.boosts[iCntBoosts]);
    assert(iCntBoosts<MAX_DB_BOOSTS);

    iCntInspirations = convertInspirations(e->pchar->aInspirations, &s_dbpows);
    iCntAttribMods = convertAttribMods(e->pchar, &s_dbpows);

    // This is an evil hack so we don't deal with lots of spurious
    // unused entries.
    assert(stricmp("Powers", desc->lineDescs[0].name)==0);
    assert(stricmp("Boosts", desc->lineDescs[1].name)==0);
    assert(stricmp("Inspirations", desc->lineDescs[2].name)==0);
    assert(stricmp("AttribMods", desc->lineDescs[3].name)==0);

    desc->lineDescs[0].size = iCntPowers;
    desc->lineDescs[1].size = iCntBoosts;
    desc->lineDescs[2].size = iCntInspirations;
    desc->lineDescs[3].size = iCntAttribMods;

    for(i=0;desc->lineDescs[i].type;i++)
    {
        structToLine(psb,(char*)&s_dbpows,0,desc,&desc->lineDescs[i],0);
    }

    // This is undoing an evil hack
    desc->lineDescs[0].size = MAX_POWERS;
    desc->lineDescs[1].size = MAX_DB_BOOSTS;
    desc->lineDescs[2].size = MAX_DB_INSPIRATIONS;
    desc->lineDescs[3].size = MAX_DB_ATTRIB_MODS;
}


/**********************************************************************func*
 * validateOriginalPowerSets
 *
 */
static bool validateOriginalPowerSets(Entity *e)
{
    bool retval = true;

    // It's okay for both to be empty - that means they'll be defaulted from
    // the listed powers. However, if either have something in them they must
    // BOTH be valid because they must have been filled out at some point in
    // the past.
    if (strlen(e->pl->primarySet) > 0 && strlen(e->pl->secondarySet) > 0)
    {
        if (!category_CheckForSet(e->pchar->pclass->pcat[kCategory_Primary],
            e->pl->primarySet))
        {
            retval = false;
        }

        if (!category_CheckForSet(e->pchar->pclass->pcat[kCategory_Secondary],
            e->pl->secondarySet))
        {
            retval = false;
        }
    }

    return retval;
}

/**********************************************************************func*
 * unpackEntPowers
 *
 */

bool unpackEntPowers(Entity *e, DBPowers *pdbpows, DB_RestoreAttrib_Types eRestoreAttrib, char *pchLogPrefix)
{
    bool bError = false;
    bool bHasAtLeastOnePower = false;
    int isPlayer = ENTTYPE(e) == ENTTYPE_PLAYER;
    int i = 0;
    U32 ulNow = dbSecondsSince2000();
    int realUnpack = (stricmp(pchLogPrefix,"Validate") != 0 && !server_state.charinfoserver);
    int bEnteredArena = OnArenaMap();
    const BasePower **toggleLocPowers = NULL;            //    don't auto activate location based powers. for now, only toggle pets really make sense for location toggles reactivating

    // Save important info from the character since character_Create nukes
    // it from orbit.
    // TODO: There's got to be a better way to do this.
    float fHitPoints                = e->pchar->attrCur.fHitPoints;
    float fMaxHitPoints                = e->pchar->attrMax.fHitPoints;
    float fEndurance                = e->pchar->attrCur.fEndurance;
    float fInsight                    = e->pchar->attrCur.fInsight;
    float fRage                        = e->pchar->attrCur.fRage;
    int iLevel                        = e->pchar->iLevel;
    int iCurBuild                    = e->pchar->iCurBuild;
    int iActiveBuild                = e->pchar->iActiveBuild;
    int iBuildChangeTime            = e->pchar->iBuildChangeTime;
    int iWisdomLevel                = e->pchar->iWisdomLevel;
    int iExperiencePoints            = e->pchar->iExperiencePoints;
    int iExperienceDebt                = e->pchar->iExperienceDebt;
    int iExperienceRest                = e->pchar->iExperienceRest;
    int iWisdomPoints                = e->pchar->iWisdomPoints;
    PlayerType playerTypeByLocation    = e->pchar->playerTypeByLocation;
    int iInfluencePoints            = e->pchar->iInfluencePoints;
    int iInfluenceApt                = e->pchar->iInfluenceApt;
    int recipeInvBonusSlots            = e->pchar->recipeInvBonusSlots;
    int recipeInvTotalSlots            = e->pchar->recipeInvTotalSlots;
    int salvageInvBonusSlots        = e->pchar->salvageInvBonusSlots;
    int salvageInvTotalSlots        = e->pchar->salvageInvTotalSlots;
    int auctionInvBonusSlots        = e->pchar->auctionInvBonusSlots;
    int auctionInvTotalSlots        = e->pchar->auctionInvTotalSlots;
    int storedSalvageInvBonusSlots    = e->pchar->storedSalvageInvBonusSlots;
    int storedSalvageInvTotalSlots    = e->pchar->storedSalvageInvTotalSlots;
    DetailInventoryItem **detailInv    = e->pchar->detailInv;
    int incarnateTimer0                = e->pchar->incarnateSlotTimesSlottable[0];
    int incarnateTimer1                = e->pchar->incarnateSlotTimesSlottable[1];
    int incarnateTimer2                = e->pchar->incarnateSlotTimesSlottable[2];
    int incarnateTimer3                = e->pchar->incarnateSlotTimesSlottable[3];
    int incarnateTimer4                = e->pchar->incarnateSlotTimesSlottable[4];
    int incarnateTimer5                = e->pchar->incarnateSlotTimesSlottable[5];
    int incarnateTimer6                = e->pchar->incarnateSlotTimesSlottable[6];
    int incarnateTimer7                = e->pchar->incarnateSlotTimesSlottable[7];
    int incarnateTimer8                = e->pchar->incarnateSlotTimesSlottable[8];
    int incarnateTimer9                = e->pchar->incarnateSlotTimesSlottable[9];
    int iBuildLevels[MAX_BUILD_NUM];
    memcpy(iBuildLevels, e->pchar->iBuildLevels, MAX_BUILD_NUM * sizeof(int));
    // TODO: ADD HERE (there's another one below, too)
    // You probably want to add to character_ReceiveRespec() as well

    if(e->pchar->pclass==NULL || e->pchar->porigin==NULL)
    {
        if(e->pchar->iLevel>0)
        {
            // Only warn of this when not creating a character
            LOG_ENT( e, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Unpack Invalid origin or class.");
            // Don't save them so we can do triage on them in the database.
            e->corrupted_dont_save = 1;
            e->pchar->entParent = NULL;
            bError = true;
        }
        return bError;
    }

    if (!validateOriginalPowerSets(e))
    {
        LOG_ENT( e, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Unpack Invalid original powersets.");
        e->pl->primarySet[0] = '\0';
        e->pl->secondarySet[0] = '\0';
    }

    character_CreatePartOne(e->pchar, e, e->pchar->porigin, e->pchar->pclass);

    // Restore nuked info.
    if (!bEnteredArena) // entering arena brings you to full health
    {
        e->pchar->attrCur.fEndurance        = fEndurance;
        e->pchar->attrCur.fHitPoints        = fHitPoints;
        e->pchar->attrCur.fInsight          = fInsight;
        e->pchar->attrCur.fRage             = fRage;
        e->pchar->attrMax.fHitPoints        = fMaxHitPoints ? fMaxHitPoints : e->pchar->attrMax.fHitPoints;
    }
    e->pchar->iLevel                             = iLevel;
    e->pchar->iCurBuild                            = iCurBuild;
    e->pchar->iActiveBuild                        = iActiveBuild;
    e->pchar->iBuildChangeTime                  = iBuildChangeTime;
    e->pchar->iWisdomLevel                       = iWisdomLevel;
    e->pchar->iExperiencePoints                  = iExperiencePoints;
    e->pchar->iExperienceDebt                    = iExperienceDebt;
    e->pchar->iExperienceRest                    = iExperienceRest;
    e->pchar->iWisdomPoints                      = iWisdomPoints;
    e->pchar->playerTypeByLocation                = playerTypeByLocation;
    e->pchar->iInfluencePoints                   = iInfluencePoints;
    e->pchar->iInfluenceApt                        = iInfluenceApt;

    e->pchar->recipeInvBonusSlots                = recipeInvBonusSlots;
    e->pchar->recipeInvTotalSlots                = recipeInvTotalSlots;
    e->pchar->salvageInvBonusSlots                = salvageInvBonusSlots;
    e->pchar->salvageInvTotalSlots                = salvageInvTotalSlots;
    e->pchar->auctionInvBonusSlots                = auctionInvBonusSlots;
    e->pchar->auctionInvTotalSlots                = auctionInvTotalSlots;
    e->pchar->storedSalvageInvBonusSlots        = storedSalvageInvBonusSlots;
    e->pchar->storedSalvageInvTotalSlots        = storedSalvageInvTotalSlots;

    e->pchar->detailInv                            = detailInv;

    e->pchar->incarnateSlotTimesSlottable[0]    = incarnateTimer0;
    e->pchar->incarnateSlotTimesSlottable[1]    = incarnateTimer1;
    e->pchar->incarnateSlotTimesSlottable[2]    = incarnateTimer2;
    e->pchar->incarnateSlotTimesSlottable[3]    = incarnateTimer3;
    e->pchar->incarnateSlotTimesSlottable[4]    = incarnateTimer4;
    e->pchar->incarnateSlotTimesSlottable[5]    = incarnateTimer5;
    e->pchar->incarnateSlotTimesSlottable[6]    = incarnateTimer6;
    e->pchar->incarnateSlotTimesSlottable[7]    = incarnateTimer7;
    e->pchar->incarnateSlotTimesSlottable[8]    = incarnateTimer8;
    e->pchar->incarnateSlotTimesSlottable[9]    = incarnateTimer9;

    memcpy(e->pchar->iBuildLevels, iBuildLevels, MAX_BUILD_NUM * sizeof(int));
    // Since we set iCurBuild above, make sure everything is now consistant
    e->pchar->iLevel                             = e->pchar->iBuildLevels[iCurBuild];
    e->pchar->ppPowerSets                        = e->pchar->ppBuildPowerSets[iCurBuild];

    // TODO: ADD HERE (there's another one above, too)

    // Set the update flag so that the new values get sent down
    e->general_update = 1;
    character_LevelFinalize(e->pchar, false);
    // Tag the buddy system last combat level, so that it knows we just zoned
    e->pchar->buddy.iLastCombatLevel = -1;

    // can't get hit points restored to correct value until after levels are set
    // ARM NOTE:  Is this necessary?
    //   If so, figure out how to get it to not grant auto-issue powers here, which is too soon.
//     if (bEnteredArena)
//     {
//         character_Reset(e->pchar, true);
//     }

    // ----------------------------------------
    // add in the original powersets first
    // This is going to cause incredible amounts of trouble for VEAT's.  It is perfectly legal for a VEAT to have different primaries and secondaries
    // in different builds.  Consider the case of a VEAT who has levelled their first build to 30, done the respec and selected a different primary and 
    // secondary.  Meanwhile, what becomes of their second build that is still at security level 15?
    if (strlen(e->pl->primarySet))
    {
        const BasePowerSet* pset;
        pset = powerdict_GetBasePowerSetByName(&g_PowerDictionary, e->pchar->pclass->pcat[kCategory_Primary]->pchName, e->pl->primarySet);
        for (i = 0; i < MAX_BUILD_NUM; i++)
        {
            character_SelectBuildLightweight(e, i);
            character_BuyPowerSet(e->pchar, pset);
        }
    }

    if (strlen(e->pl->secondarySet))
    {
        const BasePowerSet* pset;
        pset = powerdict_GetBasePowerSetByName(&g_PowerDictionary, e->pchar->pclass->pcat[kCategory_Secondary]->pchName, e->pl->secondarySet);
        for (i = 0; i < MAX_BUILD_NUM; i++)
        {
            character_SelectBuildLightweight(e, i);
            character_BuyPowerSet(e->pchar, pset);
        }
    }

    if (eRestoreAttrib != eRAT_ClearAll)
    {
        for (i=0; i < MAX_DB_ATTRIB_MODS && pdbpows->attribmods[i].dbpowBase.pchPowerName != NULL; i++)
        {
            const BasePower *ppowBase = dbpow_GetBasePower(&pdbpows->attribmods[i].dbpowBase);
            if (ppowBase)
            {
                if (pdbpows->attribmods[i].iIdxTemplate < eaSize(&ppowBase->ppTemplates))
                {
                    const AttribModTemplate *ptemplate = ppowBase->ppTemplates[pdbpows->attribmods[i].iIdxTemplate];

                    if (eRestoreAttrib == eRAT_ClearAbusiveBuffs && ppowBase->bAbusiveBuff)
                    {
                        //    abusive buffs
                        if ((ppowBase->eTargetType != kTargetType_Caster) &&
                            (ptemplate->offAttrib != kSpecialAttrib_EntCreate))
                        {
                            continue;
                        }
                        if (ppowBase->eType == kPowerType_Inspiration)
                        {
                            continue;
                        }
                    }

                    // Check for teleport attribmods - these make no sense in the "arriving" zone, so we
                    // don't include them in the list.  The actual bug that caused this was activating
                    // Lightning Rod while zoning, and getting an unwanted teleport right after arrival.
                    if (ptemplate->offAttrib != offsetof(CharacterAttributes, fTeleport) ||
                        (ptemplate->offAspect != offsetof(CharacterAttribSet, pattrMod) && 
                        ptemplate->offAspect != offsetof(CharacterAttribSet, pattrAbsolute)))
                    {
                        AttribMod *pmod = attribmod_Create();
                        assert(pmod != NULL);

                        pmod->ptemplate = ptemplate;
                        pmod->fx = power_GetFXFromToken(pmod->ptemplate->ppowBase, pdbpows->attribmods[i].customFXToken);
                        pmod->uiTint = pdbpows->attribmods[i].uiTint;
                        pmod->fDuration = pdbpows->attribmods[i].fDuration/100.0f;
                        pmod->fMagnitude = pdbpows->attribmods[i].fMagnitude/100.0f;
                        pmod->fTimer = pdbpows->attribmods[i].fTimer/100.0f;
                        pmod->fChance = pdbpows->attribmods[i].fChance/100.0f;
                        pmod->petFlags = pdbpows->attribmods[i].petFlags;
                        pmod->customFXToken = pdbpows->attribmods[i].customFXToken;
                        pmod->bSourceChecked = true;
                        pmod->uiInvokeID = pdbpows->attribmods[i].UID?pdbpows->attribmods[i].UID:nextInvokeId();
                        pmod->bSuppressedByStacking = pdbpows->attribmods[i].bSuppressedByStacking;
                        // Sources aren't tracked across server boundaries.
                        // This is actually pretty dangerous, 
                        // because kModApplicationType_OnActivate mods will be stored across
                        // mapmoves and will need to be stacked against.  Luckily, all of
                        // those are kModTarget_Self or kModTarget_SelfsOwnerAndAllPets,
                        // so we can fix it for those mod targets.

                        if( ptemplate->offAttrib == kSpecialAttrib_EntCreate )
                        {
                            pmod->erSource = erGetRef(e);
                            pmod->petFlags |= PETFLAG_TRANSFER;
                            if (ptemplate->ppowBase->eType == kPowerType_Toggle && 
                                (ptemplate->ppowBase->eTargetType == kTargetType_Location
                                    || ptemplate->ppowBase->eTargetType == kTargetType_Teleport
                                    || ptemplate->ppowBase->eTargetType == kTargetType_Position))
                            {
                                int j;
                                for (j = 0; j < eaSize(&toggleLocPowers); ++j)
                                {
                                    const BasePower *ppowBase = toggleLocPowers[j];
                                    if (ppowBase == ptemplate->ppowBase)
                                    {
                                        break;
                                    }
                                }

                                if (j == eaSize(&toggleLocPowers))
                                {
                                    eaPushConst(&toggleLocPowers, ptemplate->ppowBase);
                                }
                            }
                        }
                        else if (ptemplate->eTarget == kModTarget_Caster)
                        {
                            pmod->erSource = erGetRef(e);
                        }
                        else if (ptemplate->eTarget == kModTarget_CastersOwnerAndAllPets)
                        {
                            // ARM NOTE:  This isn't always right!
                            // But we currently never use kModTarget_SelfsOwnerAndAllPets
                            // on a pet's power, so it works... for now...  There's a designer
                            // error in CreateAndAttachNewAttribMod() that asserts this
                            // won't be a problem.
                            pmod->erSource = erGetRef(e);
                        }
                        else
                        {
                            pmod->erSource = 0;
                        }

                        modlist_AddMod(&e->pchar->listMod, pmod);
                        if (pmod->ptemplate && pmod->ptemplate->ppowBase && pmod->ptemplate->ppowBase->bShowBuffIcon)
                            e->team_buff_update = true;
                    }
                }
                else
                {
                    LOG_ENT( e, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Unpack AttribMod index is invalid. (Power definition changed?)");
                    bError = true;
                    // assert(!fileIsDevelopmentMode());
                }
            }
            else
            {
                LOG_ENT( e, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Unpack AttribMod power (%s) not in power dictionary.", dbpow_GetBasePowerError(&pdbpows->attribmods[i].dbpowBase));
                bError = true;
                // assert(!fileIsDevelopmentMode());
            }
        }
    }

    // ----------------------------------------
    // for each power from the db

    for(i=0; i<MAX_POWERS && pdbpows->powers[i].iID!=0; i++)
    {
        DBPower* thisDbPower = &pdbpows->powers[i];
        bHasAtLeastOnePower = true;
        if (thisDbPower->iBuildNum != 0)
        {
            // DGNOTE 9/18/2008
            // This should not be needed when this setup finally goes live.  If it's stopped tripping by the time this hits closed beta, the logic that sets this
            // and uses it can be removed.
            extern int g_sanityCheckBuildNumbers;
            
            if (g_sanityCheckBuildNumbers)
            {
                LOG_ENT( e, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Unpack Invalid iBuildNum detected (case 1): %d, resetting to zero.\n", thisDbPower->iBuildNum);
                thisDbPower->iBuildNum = 0;
            }
            if (thisDbPower->iBuildNum < 0 || thisDbPower->iBuildNum >= MAX_BUILD_NUM)
            {
                LOG_ENT( e, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Unpack Invalid iBuildNum detected (case 2): %d, resetting to zero.\n", thisDbPower->iBuildNum);
                thisDbPower->iBuildNum = 0;
            }
        }

        if (character_GetCurrentBuild(e) != thisDbPower->iBuildNum)
        {
            character_SelectBuildLightweight(e, thisDbPower->iBuildNum);
        }

        if(thisDbPower->dbpowBase.pchPowerName==NULL)
        {
            LOG_ENT( e, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Unpack Null power name.");
            bError = true;
        }
        else
        {
            const BasePower *ppowBase = dbpow_GetBasePower(&thisDbPower->dbpowBase);

            if(ppowBase!=NULL)
            {
                PowerSet *pset;
                Power *ppow;
                int j;

                // Fix the level bought
                if(ppowBase->bAutoIssue && !ppowBase->bAutoIssueSaveLevel)
                {
                    const BasePowerSet *psetBase = ppowBase->psetParent;
                    assert( eaiSize(&(psetBase->piAvailable)) == eaSize(&(psetBase->ppPowers)) );
                    for(j = eaiSize(&(psetBase->piAvailable))-1; j>=0; j--)
                    {
                        if(psetBase->ppPowers[j]==ppowBase)
                        {
                            thisDbPower->iPowerSetLevelBought = psetBase->piAvailable[j];
                            break;
                        }
                    }
                }

                // OK, make sure that its power set is added.
                if((pset=character_OwnsPowerSet(e->pchar, ppowBase->psetParent))==NULL)
                {
                    // DGNOTE 9/19/2008
                    // Right about here is where we'll fix VEAT's with mixed primaries and secondaries.

                    pset = character_AddNewPowerSet(e->pchar, ppowBase->psetParent);
                }

                if(!pset)
                {
                    LOG_ENT( e, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Unable to add power set for power (%s).", dbg_BasePowerStr(ppowBase));
                    bError = true;
                    continue;
                }

                pset->iLevelBought = (pset->psetBase && pset->psetBase->iForceLevelBought >= 0) ? pset->psetBase->iForceLevelBought : thisDbPower->iPowerSetLevelBought;

                // If they don't own the power yet or if it is part of the
                //    temporary power set, add it

                if((ppow = character_OwnsPower(e->pchar, ppowBase))==NULL
                    || stricmp(ppowBase->psetParent->pcatParent->pchName,"Temporary_Powers") == 0
                    || ppowBase->iNumAllowed > 1)
                {
                    // TODO: Add a proper iNumAllowed check here
                    ppow = powerset_AddNewPower(pset, ppowBase, thisDbPower->iUniqueID, NULL);
                }
                else if(stricmp(ppow->ppowBase->psetParent->pcatParent->pchName, "Inherent")!=0)
                {
                    LOG_ENT( e, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Unpack Duplicate power (%s).", dbg_BasePowerStr(ppowBase));
                    bError = true;
                    continue;
                }

                if(!ppow)
                {
                    LOG_ENT( e, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Unpack Unable to add power (%s).", dbg_BasePowerStr(ppowBase));
                    bError = true;
                    continue;
                }

                ppow->iLevelBought = (ppow->ppowBase && ppow->ppowBase->iForceLevelBought >= 0) ? ppow->ppowBase->iForceLevelBought : thisDbPower->iLevelBought;
                ppow->iNumBoostsBought = thisDbPower->iNumBoostsBought;
                ppow->iNumCharges = thisDbPower->iNumCharges;
                ppow->fUsageTime = thisDbPower->fUsageTime;
                ppow->fAvailableTime = thisDbPower->fAvailableTime;
                ppow->ulCreationTime = thisDbPower->ulCreationTime;
                ppow->instanceBought = thisDbPower->instanceBought;

                if (ppow->ppowBase->eType == kPowerType_Toggle 
                    && (ppow->ppowBase->eTargetType == kTargetType_Location
                        || ppow->ppowBase->eTargetType == kTargetType_Teleport
                        || ppow->ppowBase->eTargetType == kTargetType_Position))
                {
                    ppow->bActive = 0;    //    if any mods got transferred, then set it active. otherwise, reset it.
                    if (thisDbPower->bActive & 1)
                    {
                        int j;
                        for (j = 0; j < eaSize(&toggleLocPowers); ++j)
                        {
                            const BasePower *toggleLoc = toggleLocPowers[j];
                            if (toggleLoc == ppow->ppowBase)
                            {
                                ppow->bActive = 1;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    ppow->bActive = (thisDbPower->bActive & 1);
                }
                
                ppow->bDisabledManually = thisDbPower->bDisabled;

                if(thisDbPower->bActive & 2)
                {
                    if(ppow->ppowBase->eType==kPowerType_Click)
                    {
                        e->pchar->ppowDefault = ppow;
                    }
                }

                if (ppow->iNumBoostsBought > 0 && eaiSize(&ppow->ppowBase->pBoostsAllowed) == 0)
                {
                    LOG_ENT( e, LOG_POWERS, LOG_LEVEL_VERBOSE, 0,  "Unpack Unenhanceable power (%s) has slots. Autofixing.", dbg_BasePowerStr(ppowBase));
                    ppow->iNumBoostsBought = 0;
                    bError = true;
                }

                power_LevelFinalize(ppow); // sets number of boost slots, etc.

                // OK, loop over the boost list and assign boosts
                // to the power.
                for(j=0; pdbpows->boosts[j].dbpowBase.pchPowerName!=NULL; j++)
                {
                    if(pdbpows->boosts[j].iID == thisDbPower->iID)
                    {
                        const BasePower *ppowBase = dbpow_GetBasePower(&pdbpows->boosts[j].dbpowBase);

                        if(ppowBase!=NULL)
                        {
                            if(eaSize(&ppow->ppBoosts)<=pdbpows->boosts[j].iIdx)
                            {
                                // Not enough boost slots in power.
                                LOG_ENT( e, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Unpack Power (%s) doesn't have enough boost slots.", dbg_BasePowerStr(ppow->ppowBase));
                                bError = true;
                            }
                            else
                            {
                                DBBoost *boost = &pdbpows->boosts[j];
                                float afVars[MAX_DB_POWER_VARS];
                                PowerupSlot powerupSlots[ARRAY_SIZE(boost->aPowerupSlots)];
                                int numPowerupSlots;

                                // copy the float vals over
                                memcpy(afVars, boost->afVars, sizeof(boost->afVars));

                                // set the powerup slots
                                numPowerupSlots = setPowerupSlotsFromDB(powerupSlots, boost->iSlottedPowerupsMask, boost->aPowerupSlots, ARRAY_SIZE(boost->aPowerupSlots));

                                power_ReplaceBoost(ppow, pdbpows->boosts[j].iIdx,
                                    ppowBase,
                                    pdbpows->boosts[j].iLevel,
                                    pdbpows->boosts[j].iNumCombines,
                                    afVars, ARRAY_SIZE(afVars),
                                    powerupSlots, numPowerupSlots, NULL);

                                if(ppow->ppBoosts[pdbpows->boosts[j].iIdx]==NULL
                                    || !power_IsValidBoost(ppow, ppow->ppBoosts[pdbpows->boosts[j].iIdx]))
                                {
                                    StuffBuff sb;
                                    initStuffBuff(&sb, 100);

                                    addStringToStuffBuff(&sb, "Enhancement (%s)", dbg_BasePowerStr(ppowBase));
                                    LOG_ENT( e, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Unpack %s not allowed in power (%s).", sb.buff, dbg_BasePowerStr(ppow->ppowBase));
                                    bError = true;
                                    freeStuffBuff(&sb);
                                }
                            }
                        }
                        else
                        {
                            LOG_ENT( e, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Unpack Enhancement (%s) not in power dictionary.", dbpow_GetBasePowerError(&pdbpows->boosts[j].dbpowBase));
                            bError = true;
                        }
                    }
                }

                // If it's recharging, put it into the recharge list.
                if(thisDbPower->ulRechargedAt > ulNow && !bEnteredArena)
                {
                    power_ChangeRechargeTimer(ppow, thisDbPower->ulRechargedAt-ulNow);
                    powlist_Add(&e->pchar->listRechargingPowers, ppow,0);
                }
            }
            else
            {    
                LOG_ENT( e, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Unpack Power not in power dictionary (%s)", dbpow_GetBasePowerError(&thisDbPower->dbpowBase));
                bError = true;
            }
        }
    }

    character_SelectBuildLightweight(e, iActiveBuild);
    character_SelectBuild(e, iActiveBuild, 0);

    if(!bHasAtLeastOnePower && e->pchar->iLevel>0)
    {
        // Some terrible thing has happened to the character's powers.
        LOG_ENT( e, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Unpack Character has no powers!");

        // This will cause an error to get sent to the client saying that
        // the database is hosed.
        e->pchar->entParent = NULL;

        // Don't save them so we can do triage on them in the database.
        e->corrupted_dont_save = 1;
        eaDestroyConst(&toggleLocPowers);
        return true;
    }

    if(character_CanBuyPower(e->pchar) && e->pchar->iLevel>0)
    {
        // This is very odd, but we can try to heal them up.

        LOG_ENT( e, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Unpack Character doesn't have as many powers as it should. At level %d, it should have %d powers, not %d.", 
            e->pchar->iLevel + 1, 
            CountForLevel(e->pchar->iLevel, g_Schedules.aSchedules.piPower),
            character_CountPowersBought(e->pchar));
        bError = true;        
    }

    // If they didn't have the original primary and secondary specified, we
    // can imply them from their powers. They will have listed them by power
    // set, so the first primary and secondary we find in their own sets must
    // be the first ones they picked from and thus their original primary and
    // secondary at creation.
    i = 0;

    while ((strlen(e->pl->primarySet) == 0 || strlen(e->pl->secondarySet) == 0) &&
        i < eaSize(&(e->pchar->ppPowerSets)))
    {
        PowerSet* pset = e->pchar->ppPowerSets[i];

        if (pset->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Primary] &&
            strlen(e->pl->primarySet) == 0)
        {
            strncpy(e->pl->primarySet, pset->psetBase->pchName, MAX_NAME_LEN);
        }
        else if (pset->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Secondary] &&
            strlen(e->pl->secondarySet) == 0)
        {
            strncpy(e->pl->secondarySet, pset->psetBase->pchName, MAX_NAME_LEN);
        }

        i++;
    }

    // ----------------------------------------
    // for each inventory boost from the db

    for(i=0; (i<MAX_DB_BOOSTS) && (pdbpows->boosts[i].dbpowBase.pchPowerName!=NULL); i++)
    {
        if(pdbpows->boosts[i].iID == -1)
        {
            const BasePower *ppowBase = dbpow_GetBasePower(&pdbpows->boosts[i].dbpowBase);
            if(ppowBase)
            {
                DBBoost *boost = &pdbpows->boosts[i];
                float afVars[MAX_DB_POWER_VARS];
                PowerupSlot powerupSlots[ARRAY_SIZE(boost->aPowerupSlots)];
                int numPowerupSlots;

                // copy the float vals over
                memcpy(afVars, boost->afVars, sizeof(boost->afVars));

                // set the powerup slots
                numPowerupSlots = setPowerupSlotsFromDB( powerupSlots, boost->iSlottedPowerupsMask, boost->aPowerupSlots, ARRAY_SIZE( boost->aPowerupSlots ) );

                character_SetBoost(e->pchar, pdbpows->boosts[i].iIdx, ppowBase,
                                   pdbpows->boosts[i].iLevel,
                                   pdbpows->boosts[i].iNumCombines, afVars, ARRAY_SIZE( afVars ),
                                   powerupSlots, numPowerupSlots );
            }
            else
            {
                LOG_ENT( e, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Unpack Inventory enhancement (%s) not in power dictionary.", dbpow_GetBasePowerError(&pdbpows->boosts[i].dbpowBase));
                bError = true;
            }
        }
    }

    for(i=0; (i<MAX_DB_INSPIRATIONS) && (pdbpows->inspirations[i].dbpowBase.pchPowerName!=NULL); i++)
    {
        const BasePower *ppowBase = dbpow_GetBasePower(&pdbpows->inspirations[i].dbpowBase);
        if(ppowBase)
        {
            character_SetInspiration(e->pchar, ppowBase, pdbpows->inspirations[i].iCol, pdbpows->inspirations[i].iRow);
        }
        else
        {
            LOG_ENT( e, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Unpack Inspiration (%s) not in power dictionary.", dbpow_GetBasePowerError(&pdbpows->inspirations[i].dbpowBase));
            bError = true;
        }
    }

    eaDestroyConst(&toggleLocPowers);
    // Only accrue mods if we're doing a real unpack.
    if(stricmp(pchLogPrefix,"Validate") != 0 && !server_state.charinfoserver)
    {
        // validate powers
        character_checkPowersAndRemoveBadPowers(e->pchar);

        // set player on correct team in case we create pets
        if(ENT_IS_ON_RED_SIDE(e))
            entSetOnTeamVillain(e, 1);
        else
            entSetOnTeamHero(e, 1);

        character_AccrueMods(e->pchar, 0.0, NULL);
        setSurfaceModifiers(e, SURFPARAM_AIR, e->pchar->attrCur.fMovementControl, e->pchar->attrCur.fMovementFriction, 1.0f, 1.0f);
        setJumpHeight(e, e->pchar->attrCur.fJumpHeight);

        // Can't turn fly on yet, so this will allow it to kick on next tick
        e->pchar->attrCur.fFly = MIN(0.0,e->pchar->attrCur.fFly);
    }

    character_CreatePartTwo(e->pchar);

    return bError;
}


//------------------------------------------------------------
//  Checks if the passed slot is valid
//----------------------------------------------------------
bool  dbrewardslot_Valid(DBRewardSlot *slot)
{
    return slot && slot->id > 0 && rewarditemtype_Valid(slot->type);
}

/* End of File */

