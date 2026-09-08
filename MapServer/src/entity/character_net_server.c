/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/network/netio.h>
#include <utilitieslib/components/earray.h>

#include "entity/classes.h"
#include "entity/origins.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "character_combat.h"
#include "entity/character_inventory.h"
#include "entity/powers.h"
#include "entity/character_net.h"
#include "character_net_server.h"
#include "entity/entity.h" // Just to get at e->auth_name for printing cheater info.  Better solution is to make a cheaterLog function that takes an entity
#include "storyarc/contact.h" // For contact stuff
#include "storyarc/storyarcprivate.h" // For contact stuff
#include "entity/PowerInfo.h"    // for character respec
#include "dbcomm/dbcomm.h"  // for logging
#include "dbghelper.h" // for logging
#include "comm_game.h"
#include "entity/entPlayer.h"
#include "svr/svr_player.h"
#include "auth/authUserData.h"
#include "entity/costume.h"
#include "keybinds.h" // for testing keybind corruption
#include "entity/attrib_description.h"
#include "alignment_shift.h"
#include "dbcomm/logcomm.h"
#include "player/badges_server.h"
#include "accountservercomm.h"

/**********************************************************************func*
 * character_IsLeveling
 *
 */
bool character_IsLeveling(Character *pchar)
{
    if(pchar->entParent->pl->levelling)
    {
        return true;
    }

    return false;
}

/**********************************************************************func*
* character_IsValidAT
*
*/
bool character_IsValidAT(Entity *e, Character *pchar, CharacterClass *pclass, int bCanBePraetorian)
{
    // Epic ATs can only be Primal Earth characters.

//    if(isProductionMode())
    if (true)
    {
        if (class_MatchesSpecialRestriction(pclass, "Kheldian"))
        {
            if (bCanBePraetorian)
                return false;

            if (!authUserHasKheldian(e->pl->auth_user_data) && 
                (pclass->pchStoreRestrictions == NULL || 
                    !accountEval(ent_GetProductInventory( e ), e->pl->loyaltyStatus, e->pl->loyaltyPointsEarned, e->pl->auth_user_data, pclass->pchStoreRestrictions)))
                return false;

            return true;
        }

        if (class_MatchesSpecialRestriction(pclass, "ArachnosSoldier") ||    class_MatchesSpecialRestriction(pclass, "ArachnosWidow"))
        {
            if (bCanBePraetorian)
                return false;

            if (!authUserHasArachnosAccess(e->pl->auth_user_data) && 
                (pclass->pchStoreRestrictions == NULL || 
                !accountEval(ent_GetProductInventory( e ), e->pl->loyaltyStatus, e->pl->loyaltyPointsEarned, e->pl->auth_user_data, pclass->pchStoreRestrictions)))
                return false;

            return true;
        }


        if (pclass->pchStoreRestrictions != NULL && 
            !accountEval(ent_GetProductInventory( e ), e->pl->loyaltyStatus, e->pl->loyaltyPointsEarned, e->pl->auth_user_data, pclass->pchStoreRestrictions))
            return false;
        else
            return true;
    } else {
        return true;
    }
}

/**********************************************************************func*
 * character_ReceiveCreate
 *
 * Receives character creation data from client. The powers are verified
 * as being available and valid.
 *
 * NET: fully consumes data even on error
 * NET: returns false on bad data
 */
bool character_ReceiveCreate(Packet* pak, Character *pchar, Entity *e)
{
    bool bRet;

    bRet = character_ReceiveCreateClassOrigin(pak, pchar, e, NULL, NULL);
    bRet |= character_ReceiveCreatePowers(pak, pchar, e);
    character_CreatePartTwo(pchar);
    alignmentshift_updatePlayerTypeByLocation(e);

    // This forces a full redefine to be sent.
    if (pchar && pchar->entParent) {
        eaPush(&pchar->entParent->powerDefChange, (void *)-1);
    }

    return bRet;
}

bool character_ReceiveCreateClassOrigin(Packet* pak, Character *pchar, Entity *e, const CharacterClass *oldClass, const CharacterOrigin *oldOrigin)
{
    const CharacterOrigin *porigin;
    const CharacterClass *pclass;
    bool bRet = true;

    //
    // string ClassName
    // string OriginName
    // Powers
    // Inspirations
    //

    /* Basic Stuff ****************************************************/

    pclass = classes_GetPtrFromName(&g_CharacterClasses, pktGetString(pak));
    if (!pclass)
    {
        dbLog("cheater", e, "Player with authname %s tried to choose invalid class\n", e->auth_name);
        bRet = false;
    }
    porigin = origins_GetPtrFromName(&g_CharacterOrigins, pktGetString(pak));
    if (!porigin)
    {
        dbLog("cheater", e, "Player with authname %s tried to choose invalid origin\n", e->auth_name);
        bRet = false;
    }

    if(pclass != NULL && porigin != NULL)
    {
        // oldClass and oldOrigin will be non-NULL if and only if this is being called on behalf of a respec, that call comes from
        // the top of static bool respec_receiveInfo(...) over in character_respec.c.  When called for character creation they will
        // be NULL, that comes from bool character_ReceiveCreate(...) just above.
        if (oldClass != NULL && oldOrigin != NULL)
        {
            // In the case of a respec, the test becomes very simple: do the class and origin still match?  This is based on the fact that
            // you can't change your class and origin during a respec.
            if (pclass != oldClass || porigin != oldOrigin)
            {
                bRet = false;
                dbLog("cheater", e, "Authname %s tried to change class or origin during a respec\n", e->auth_name);
            }
        }
        else
        {
            //// We let people with accesslevel choose any origin for their
            //// characters.
            if(!e->access_level && !origin_IsAllowedForClass(porigin, pclass))
            {
                bRet = false;
                dbLog("cheater", e, "Authname %s tried to create character with an origin (%s) not allowed for the class (%s)\n", e->auth_name, porigin->pchName, pclass->pchName);
            }
        }
        // Create the shell of the character
        if( bRet )
        {
            character_CreatePartOne(pchar, e, porigin, pclass);
            dbLog("Character:Create", e, "Origin: %s, Archetype: %s", porigin->pchName, pclass->pchName);
        }
    }

    return bRet;
}

bool character_ReceiveCreatePowers(Packet* pak, Character *pchar, Entity *e)
{
    PowerSet *psetPrimary = NULL;
    PowerSet *psetSecondary = NULL;
    const BasePower *ppowBase;
    bool bRet = true;
    int uniqueID;

    /* Powers *********************************************************/

    ppowBase = basepower_Receive(pak, &g_PowerDictionary, (pchar ? pchar->entParent : 0));
    uniqueID = pktGetBitsAuto(pak);

    if(pchar && e->pl && ppowBase && ppowBase->psetParent && pchar->pclass
        && ppowBase->psetParent->pcatParent == pchar->pclass->pcat[kCategory_Primary])
    {
        if(ppowBase->psetParent->piAvailable[ppowBase->id.ipow] == 0)
        {
            Power *ppow;
            // Make the power set
            if ((psetPrimary = character_BuyPowerSet(pchar, ppowBase->psetParent)) != NULL)
            {
                if ((ppow = character_BuyPower(pchar, psetPrimary, ppowBase, uniqueID)) == NULL)
                {
                    bRet = false;
                }

                dbLog("Character:InitialPowerSet", pchar->entParent, "%s", ppowBase->psetParent->pchName);
                dbLog("Character:InitialPower", pchar->entParent, "%s", dbg_BasePowerStr(ppowBase));

            }
        } else {
            dbLog("cheater", pchar->entParent, "Authname %s tried to create character with unavailable power %s\n", e->auth_name, ppowBase->pchName);
            bRet = false;
        }
    } else {
        dbLog("cheater", (pchar ? pchar->entParent : 0), "Authname %s tried to create character with invalid power\n", e->auth_name);
        bRet = false;
    }

    // Yeah, it's just a kludgy copy of the above code.

    ppowBase = basepower_Receive(pak, &g_PowerDictionary, (pchar ? pchar->entParent : 0));
    uniqueID = pktGetBitsAuto(pak);

    if(pchar  && e->pl && ppowBase && ppowBase->psetParent && pchar->pclass
        && ppowBase->psetParent->pcatParent == pchar->pclass->pcat[kCategory_Secondary])
    {
        if(ppowBase->psetParent->piAvailable[ppowBase->id.ipow]==0)
        {
            Power *ppow;
            // Make the power set
            if ((psetSecondary = character_BuyPowerSet(pchar, ppowBase->psetParent)) != NULL)
            {
                if ((ppow = character_BuyPower(pchar, psetSecondary, ppowBase, uniqueID)) == NULL)
                {
                    bRet = false;
                }

                dbLog("Character:InitialPowerSet", pchar->entParent, "%s", ppowBase->psetParent->pchName);
                dbLog("Character:InitialPower", pchar->entParent, "%s", dbg_BasePowerStr(ppowBase));
            }
        } else {
            dbLog("cheater", pchar->entParent, "Authname %s tried to create character with unavailable power %s\n", e->auth_name, ppowBase->pchName);
            bRet = false;
        }
    } else {
        dbLog("cheater", (pchar ? pchar->entParent : 0), "Authname %s tried to create character with invalid power\n", e->auth_name);
        bRet = false;
    }

    // Now we know which sets they chose, store the names for future reference
    // in case they respec into specialization sets and don't pick any powers
    // from these original sets.
    if(bRet)
    {
        strncpy(e->pl->primarySet, psetPrimary->psetBase->pchName, MAX_NAME_LEN);
        strncpy(e->pl->secondarySet, psetSecondary->psetBase->pchName, MAX_NAME_LEN);
    }

    return bRet;
}

/**********************************************************************func*
 * inspiration_Send
 *
 */
void inspiration_Send(Packet *pak, Character *pchar, int iCol, int iRow)
{
    // inspiration column
    // inspiration row
    // 0
    //   empty slot
    // 1
    //   basepower

    // Although this is a two dimensional array, it's flattened for this.
    pktSendBitsPack(pak, 3, iCol);
    pktSendBitsPack(pak, 3, iRow);
    if(pchar->aInspirations[iCol][iRow]!=NULL)
    {
        pktSendBits(pak, 1, 1);
        basepower_Send(pak, pchar->aInspirations[iCol][iRow]);
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }
}

/**********************************************************************func*
 * character_SendInspirations
 *
 */
void character_SendInspirations(Packet *pak, Character *pchar)
{
    int i, j;

    // count of inspiration columns
    // count of inspiration
    //   inspiration[0][0]
    //     0
    //       empty slot
    //     1
    //       basepower
    //   inspiration[0][1]...

    pktSendBitsPack(pak, 3, pchar->iNumInspirationCols);
    pktSendBitsPack(pak, 3, pchar->iNumInspirationRows);
    for(i=0; i<pchar->iNumInspirationCols; i++)
    {
        for(j=0; j<pchar->iNumInspirationRows; j++)
        {
            if(pchar->aInspirations[i][j]!=NULL)
            {
                pktSendBits(pak, 1, 1);
                basepower_Send(pak, pchar->aInspirations[i][j]);
            }
            else
            {
                pktSendBits(pak, 1, 0);
            }
        }
    }
}

/**********************************************************************func*
 * character_Send
 *
 // -AB: added salvage :2005 Feb 23 02:47 PM
 */
void character_Send(Packet* pak, Character *pchar)
{
    /* Basic Stuff ****************************************************/

    pktSendString(pak, pchar->pclass->pchName);
    pktSendString(pak, pchar->porigin->pchName);
    pktSendBitsPack(pak, 5, pchar->iLevel);
    pktSendBitsAuto(pak, pchar->iCombatModShiftToSendToClient);
    pktSendBitsPack(pak, 4, pchar->iCurBuild);
    pktSendBitsAuto(pak, pchar->playerTypeByLocation);

    /* Powers *********************************************************/

    character_SendPowers(pak, pchar);

    /* Inspirations ***************************************************/

    character_SendInspirations(pak, pchar);

    /* Boosts ***************************************************/

    character_SendBoosts(pak, pchar);

    /* Inventory ***************************************************/

    character_SendInventories(pchar, pak);
}

/**********************************************************************func*
 * character_SendBuffs
 *
 */

static int ClearOldPowers(void* pData, StashElement elem)
{
    Character* p = (Character*)pData;
    U32 uiID = stashElementGetU32Key(elem);
    if(!stashIntFindInt(p->stNewBuffs, uiID, 0) )
    {
        p->entParent->team_buff_update = true;
        stashIntRemoveInt(p->stSentBuffs,uiID,NULL);
    }
    return 1;
}

void character_UpdateBuffStash(Character * p)
{
    // reset cleared buff list
    if (p->stSentBuffs)
    {
        devassert(p->stNewBuffs); // should be created  as a pair
        stashForEachElementEx(p->stSentBuffs, ClearOldPowers, p ); 
        stashTableClear(p->stNewBuffs);
    }
    else
    {
        // @todo
        // Apparently we can be asked to update before the iniial buff send
        // which creates stashes. Is that a problem?
        TODO();
    }
    p->update_buffs = 0;
}

// *************************************************************************
// - always send the powers active on the player.
// - for the attrib mods, only send the new ones.
// character_ReceiveBuffs
// *************************************************************************
void character_SendBuffs(Packet *pak, Entity * recvEnt, Character *p)
{
    int i;
    int iCntBuffs = 0;
    int dont_send, dont_send_numbers;
    int diffsOnly = eaiSize(&p->entParent->diffBuffs);

    if( !recvEnt->pchar->stSentBuffs )
        recvEnt->pchar->stSentBuffs = stashTableCreateInt(8);
     if( !recvEnt->pchar->stNewBuffs )
         recvEnt->pchar->stNewBuffs = stashTableCreateInt(8);

    p->update_buffs = 1;

    if( recvEnt == p->entParent )
    {
        dont_send = recvEnt->pl->buffSettings&(1<<(kBuff_NoSend));
        dont_send_numbers =recvEnt->pl->buffSettings&(1<<(kBuff_NoNumbers));
    }
    else if(p->entParent->erOwner) // buffs on a pet
    {
        dont_send = recvEnt->pl->buffSettings&(1<<(kBuff_NoSend+16));
        dont_send_numbers =recvEnt->pl->buffSettings&(1<<(kBuff_NoNumbers+16));
    }
    else //must be teammates
    {
        dont_send = recvEnt->pl->buffSettings&(1<<(kBuff_NoSend+8));
        dont_send_numbers =recvEnt->pl->buffSettings&(1<<(kBuff_NoNumbers+8));
    }

    if( dont_send )
    {
        pktSendBits(pak,1,1);
        return;
    }
    else
        pktSendBits(pak,1,0);

    // ----------------------------------------
    // send buff info to client
    // - send every power every tick
    // - send mods for new powers only
    if (diffsOnly)
    {
        iCntBuffs = eaiSize(&p->entParent->diffBuffs);
        pktSendBits(pak, 1, 1);
    }
    else
    {
        iCntBuffs = p->entParent->iCntBuffs;
        pktSendBits(pak, 1, 0);
    }
    pktSendBitsPack(pak, 4, iCntBuffs);
    
    while(iCntBuffs>0)
    {
        UniqueBuff *buff;
        iCntBuffs--;
        
        buff = &p->entParent->aBuffs[diffsOnly ? p->entParent->diffBuffs[iCntBuffs] : iCntBuffs];
        if (diffsOnly)
        {
            pktSendBits(pak, 1, buff->addedOrChangedThisTick == -1);
        }

        basepower_Send(pak,buff->ppow);


        if( buff->time_remaining )
        {
            pktSendBits( pak, 1, 1 );
            pktSendBits( pak, 1, (buff->time_remaining) < 10 ? 1 : 0);
        }
        else
        {
            pktSendBits( pak, 1, 0 );
        }
        pktSendBitsPack( pak, 20, (int)buff->uiID );

        stashIntAddInt(recvEnt->pchar->stNewBuffs, buff->uiID, buff->uiID, 1);

        if( dont_send_numbers || stashIntFindInt(recvEnt->pchar->stSentBuffs, buff->uiID,NULL) || !buff->mod_cnt )
        {
            //mods already sent
            pktSendBits(pak,1,0);
        }
        else
        {
            stashIntAddInt(recvEnt->pchar->stSentBuffs, buff->uiID, buff->uiID, 1);
            pktSendBits(pak,1,1);        
            pktSendBitsPack(pak,5,buff->mod_cnt);
            for(i=0;i<buff->mod_cnt;i++)
            {
                pktSendBitsPack(pak,10, (int)((buff->mod[i].fMag*100)));
                pktSendBitsPack(pak,10,buff->mod[i].iHash);
            }
        }
    }
}

/**********************************************************************func*
 * character_SendStatusEffects
 *
 */
void character_SendStatusEffects(Packet *pak, Character *p)
{
    U32 effects = 0;
    F32 fTest, fOrig;
    int i, numDesignerStatuses;
    static const CharacterClass *pclassStalker = NULL;
    static const CharacterClass *pclassArachnosWidow = NULL;
    static const CharacterClass *pclassArachnosSoldier = NULL;

    if(!pclassStalker)
    {
        pclassStalker = classes_GetPtrFromName(&g_CharacterClasses,"Class_Stalker");
        pclassArachnosWidow = classes_GetPtrFromName(&g_CharacterClasses,"Class_Arachnos_Widow");
        pclassArachnosSoldier = classes_GetPtrFromName(&g_CharacterClasses,"Class_Arachnos_Soldier");
    }
    // These are expected in a particular order on the client, and they're now checked for things
    //  like pet commands, so make sure to update them properly if you insert any
    effects |= (p->attrCur.fHeld>0.0)                << 0;
    effects |= (p->attrCur.fStunned>0.0)            << 1;
    effects |= (p->attrCur.fSleep>0.0)                << 2;
    effects |= (p->attrCur.fTerrorized>0.0)            << 3;
    effects |= (p->attrCur.fImmobilized>0.0)        << 4;
    effects |= (p->attrCur.fConfused>0.0)            << 5;
    effects |= (p->attrCur.fOnlyAffectsSelf>0.0)    << 6;
    effects |= (p->pmodTaunt!=0 && p->pmodTaunt->fMagnitude>0)            << 7;
    effects |= (p->attrCur.fMeter>=1.0 && (p->pclass == pclassStalker||p->pclass == pclassArachnosWidow || p->pclass == pclassArachnosSoldier) ) << 8;

    fTest = *(ATTRIB_GET_PTR(&p->attrMod, offsetof(CharacterAttributes, fPerceptionRadius)));
    fOrig = *(ATTRIB_GET_PTR(&g_attrModBase, offsetof(CharacterAttributes, fPerceptionRadius)));
    effects |= (fTest<fOrig) << 9;

    pktSendBits(pak, 10, effects);
    pktSendBitsAuto(pak, p->entParent->bitfieldVisionPhases);
    pktSendBitsAuto(pak, p->entParent->exclusiveVisionPhase);
    numDesignerStatuses = eaSize(&p->designerStatuses);
    pktSendBitsAuto(pak, numDesignerStatuses);
    for (i = 0; i < numDesignerStatuses; ++i)
    {
        pktSendString(pak, p->designerStatuses[i]);
    }
}

/**********************************************************************func*
 * power_ReceiveCombineBoosts
 *
 * NET: fully consumes data even on error
 * NET: returns false on bad data
 */
bool power_ReceiveCombineBoosts(Packet *pak, Character *pchar)
{
    Boost *pbA = 0, *pbB = 0;

    int isetA, ipowA, ispecA, isetB, ipowB, ispecB;

    // get the first boost
    if( pktGetBits( pak, 1 ) )
    {
        isetA  = pktGetBitsPack( pak, 1 );
        ipowA  = pktGetBitsPack( pak, 1 );
        ispecA = pktGetBitsPack( pak, 1 );

        // The zeroth slot in power sets is for temporary powers
        if( isetA >= 1 && isetA < eaSize(&pchar->ppPowerSets) && pchar->ppPowerSets[isetA] != NULL &&
            ipowA >= 0 && ipowA < eaSize(&pchar->ppPowerSets[isetA]->ppPowers) && pchar->ppPowerSets[isetA]->ppPowers[ipowA] != NULL &&
            ispecA >= 0 && ispecA < eaSize(&pchar->ppPowerSets[isetA]->ppPowers[ipowA]->ppBoosts ))
        {
            pbA = pchar->ppPowerSets[isetA]->ppPowers[ipowA]->ppBoosts[ispecA];
        }
    }
    else
    {
        ispecA = pktGetBitsPack( pak, 1 );

        if( ispecA >= 0 && ispecA < CHAR_BOOST_MAX )
        {
            pbA = pchar->aBoosts[ispecA];
        }
    }

    // get the second boost
    if( pktGetBits( pak, 1 ) )
    {
        isetB  = pktGetBitsPack( pak, 1 );
        ipowB  = pktGetBitsPack( pak, 1 );
        ispecB = pktGetBitsPack( pak, 1 );

        if( isetB >= 1 && isetB < eaSize(&pchar->ppPowerSets) && pchar->ppPowerSets[isetB] != NULL &&
            ipowB >= 0 && ipowB < eaSize(&pchar->ppPowerSets[isetB]->ppPowers) && pchar->ppPowerSets[isetB]->ppPowers[ipowB] != NULL &&
            ispecB >= 0 && ispecB < eaSize(&pchar->ppPowerSets[isetB]->ppPowers[ipowB]->ppBoosts ))
        {
            pbB = pchar->ppPowerSets[isetB]->ppPowers[ipowB]->ppBoosts[ispecB];
        }
    }
    else
    {
        ispecB = pktGetBitsPack( pak, 1 );

        if( ispecB >= 0 && ispecB < CHAR_BOOST_MAX )
        {
            pbB = pchar->aBoosts[ispecB];
        }
    }

    // We need both boosts, and one has to come from a power.
    if( !pbA || !pbB || ( !pbA->ppowParent && !pbB->ppowParent ) )
    {
        dbLog("CombineBoosts:Error",
            pchar->entParent,
            "%d.%d.%d pbA=0x%p %s.%d+%d, parent=0x%p  %d.%d.%d pbB=0x%p %s.%d+%d, parent=0x%p",
            isetA, ipowA, ispecA,
            pbA,
            pbA?dbg_BasePowerStr(pbA->ppowBase):"(null)", pbA?pbA->iLevel:0, pbA?pbA->iNumCombines:0,
            pbA ? pbA->ppowParent : 0,
            isetB, ipowB, ispecB,
            pbB,
            pbB?dbg_BasePowerStr(pbB->ppowBase):"(null)", pbB?pbB->iLevel:0, pbB?pbB->iNumCombines:0,
            pbB ? pbB->ppowParent : 0);

        return false;
    }

    // Both boosts must be from the same power set.
    if( !boost_IsValidCombination( pbA, pbB ) )
    {
        dbLog("CombineBoosts:Error",
            pchar->entParent,
            "Cannot combine boosts %s.%d+%d and %s.%d+%d",
            dbg_BasePowerStr(pbA->ppowBase), pbA->iLevel, pbA->iNumCombines,
            dbg_BasePowerStr(pbB->ppowBase), pbB->iLevel, pbB->iNumCombines);

        return false;
    }

    // Boosts must not be eachother
    if( pbA == pbB )
    {
        dbLog("CombineBoosts:Error",
            pchar->entParent,
            "Cannot combine boost with itself %s.%d",
            dbg_BasePowerStr(pbA->ppowBase), ispecA);

        return false;
    }


    dbLog("CombineBoosts", pchar->entParent, "Combining %s.%d+%d from %s %d and %s.%d+%d from %s %d",
        dbg_BasePowerStr(pbA->ppowBase),
        pbA->iLevel,
        pbA->iNumCombines,
        pbA->ppowParent ? dbg_BasePowerStr(pbA->ppowParent->ppowBase) : "inventory slot",
        ispecA,
        dbg_BasePowerStr(pbB->ppowBase),
        pbB->iLevel,
        pbB->iNumCombines,
        pbB->ppowParent ? dbg_BasePowerStr(pbB->ppowParent->ppowBase) : "inventory slot",
        ispecB);

    power_CombineBoosts( pchar, pbA, pbB );

    return true;
}

/**********************************************************************func*
* character_SendCombineResoponse
*
* NET: fully consumes data even on error
* NET: returns false on bad data
*/

void character_SendCombineResponse( Entity *e, int iSuccess, int iSlot )
{
    START_PACKET( pak, e, SERVER_COMBINE_RESPONSE )
        pktSendBitsPack( pak, 1, iSuccess );
        pktSendBitsPack( pak, 1, iSlot );
    END_PACKET
}

/**********************************************************************func*
* power_ReceiveBoosterBoost
*
* NET: fully consumes data even on error
* NET: returns false on bad data
*/
bool power_ReceiveBoosterBoost(Packet *pak, Character *pchar)
{
    Boost *pbA = 0;
    int isetA, ipowA, ispecA;
    SalvageInventoryItem *pSal = NULL;

    // get the boost
    isetA  = pktGetBitsPack( pak, 1 );
    ipowA  = pktGetBitsPack( pak, 1 );
    ispecA = pktGetBitsPack( pak, 1 );

    // The zeroth slot in power sets is for temporary powers
    if( isetA >= 1 && isetA < eaSize(&pchar->ppPowerSets) && pchar->ppPowerSets[isetA] != NULL &&
        ipowA >= 0 && ipowA < eaSize(&pchar->ppPowerSets[isetA]->ppPowers) && pchar->ppPowerSets[isetA]->ppPowers[ipowA] != NULL &&
        ispecA >= 0 && ispecA < eaSize(&pchar->ppPowerSets[isetA]->ppPowers[ipowA]->ppBoosts ))
    {
        pbA = pchar->ppPowerSets[isetA]->ppPowers[ipowA]->ppBoosts[ispecA];
    }

    // We need the boost and it has to come from a power.
    if( !pbA || !pbA->ppowParent )
    {
        dbLog("BoosterBoost:Error",
            pchar->entParent,
            "%d.%d.%d pbA=0x%p %s.%d+%d, parent=0x%p",
            isetA, ipowA, ispecA,
            pbA,
            pbA?dbg_BasePowerStr(pbA->ppowBase):"(null)", pbA?pbA->iLevel:0, pbA?pbA->iNumCombines:0,
            pbA ? pbA->ppowParent : 0);
        return false;
    }

    // Can this be boosted?
    if( !boost_IsBoostable( pchar->entParent, pbA ) )
    {
        dbLog("BoosterBoost:Error",
            pchar->entParent,
            "Cannot boost boost %s.%d+%d",
            dbg_BasePowerStr(pbA->ppowBase), pbA->iLevel, pbA->iNumCombines);

        return false;
    }

    // Do we have the required booster?
    pSal = character_FindSalvageInv(pchar, "S_EnhancementBooster");
    if (!pSal || pSal->amount <= 0)
    {
        dbLog("BoosterBoost:Error",
            pchar->entParent,
            "Player does not have booster to boost %s.%d+%d",
            dbg_BasePowerStr(pbA->ppowBase), pbA->iLevel, pbA->iNumCombines);
        return false;
    }


    dbLog("BoosterBoost", pchar->entParent, "Boosting %s.%d+%d from %s %d",
        dbg_BasePowerStr(pbA->ppowBase),
        pbA->iLevel,
        pbA->iNumCombines,
        pbA->ppowParent ? dbg_BasePowerStr(pbA->ppowParent->ppowBase) : "inventory slot",
        ispecA);

    power_BoosterBoost( pchar, pbA );

    return true;
}

/**********************************************************************func*
* character_SendBoosterResponse
*
* NET: fully consumes data even on error
* NET: returns false on bad data
*/

void character_SendBoosterResponse( Entity *e, int iSuccess, int iSlot )
{
    START_PACKET( pak, e, SERVER_BOOSTER_RESPONSE )
    pktSendBitsPack( pak, 1, iSuccess );
    pktSendBitsPack( pak, 1, iSlot );
    END_PACKET
}

/**********************************************************************func*
* power_ReceiveCatalystBoost
*
* NET: fully consumes data even on error
* NET: returns false on bad data
*/
bool power_ReceiveCatalystBoost(Packet *pak, Character *pchar)
{
    Boost *pbA = 0;
    int isetA, ipowA, ispecA;
    SalvageInventoryItem *pSal = NULL;

    // get the boost
    isetA  = pktGetBitsPack( pak, 1 );
    ipowA  = pktGetBitsPack( pak, 1 );
    ispecA = pktGetBitsPack( pak, 1 );

    // The zeroth slot in power sets is for temporary powers
    if( isetA >= 1 && isetA < eaSize(&pchar->ppPowerSets) && pchar->ppPowerSets[isetA] != NULL &&
        ipowA >= 0 && ipowA < eaSize(&pchar->ppPowerSets[isetA]->ppPowers) && pchar->ppPowerSets[isetA]->ppPowers[ipowA] != NULL &&
        ispecA >= 0 && ispecA < eaSize(&pchar->ppPowerSets[isetA]->ppPowers[ipowA]->ppBoosts ))
    {
        pbA = pchar->ppPowerSets[isetA]->ppPowers[ipowA]->ppBoosts[ispecA];
    }

    // We need the boost and it has to come from a power.
    if( !pbA || !pbA->ppowParent )
    {
        dbLog("CatalystBoost:Error",
            pchar->entParent,
            "%d.%d.%d pbA=0x%p %s.%d+%d, parent=0x%p",
            isetA, ipowA, ispecA,
            pbA,
            pbA?dbg_BasePowerStr(pbA->ppowBase):"(null)", pbA?pbA->iLevel:0, pbA?pbA->iNumCombines:0,
            pbA ? pbA->ppowParent : 0);
        return false;
    }

    // Can this be boosted?
    if( !boost_IsCatalyzable( pbA ) )
    {
        dbLog("CatalystBoost:Error",
            pchar->entParent,
            "Cannot catalyst boost %s.%d+%d",
            dbg_BasePowerStr(pbA->ppowBase), pbA->iLevel, pbA->iNumCombines);

        return false;
    }

    // Do we have the required booster?
    pSal = character_FindSalvageInv(pchar, "S_EnhancementCatalyst");
    if (!pSal || pSal->amount <= 0)
    {
        dbLog("CatalystBoost:Error",
            pchar->entParent,
            "Player does not have catalyst to boost %s.%d+%d",
            dbg_BasePowerStr(pbA->ppowBase), pbA->iLevel, pbA->iNumCombines);
        return false;
    }


    dbLog("CatalystBoost", pchar->entParent, "Catalyzing %s.%d+%d from %s %d",
        dbg_BasePowerStr(pbA->ppowBase),
        pbA->iLevel,
        pbA->iNumCombines,
        pbA->ppowParent ? dbg_BasePowerStr(pbA->ppowParent->ppowBase) : "inventory slot",
        ispecA);

    power_CatalyzeBoost( pchar, pbA );

    return true;
}

/**********************************************************************func*
* character_SendCatalystResponse
*
* NET: fully consumes data even on error
* NET: returns false on bad data
*/

void character_SendCatalystResponse( Entity *e, int iSuccess, int iSlot )
{
    START_PACKET( pak, e, SERVER_CATALYST_RESPONSE )
    pktSendBitsPack( pak, 1, iSuccess );
    pktSendBitsPack( pak, 1, iSlot );
    END_PACKET
}



/**********************************************************************func*
 * character_ReceiveSetBoost
 *
 * NET: fully consumes data even on error
 * NET: returns false on bad data
 * -AB: using charcter_SwapBoosts now  :2005 Mar 30 10:58 AM
 */
bool character_ReceiveSetBoost( Packet *pak, Character *pchar)
{
    int iFromIdx    = pktGetBitsPack( pak, 1 );
    int iToIdx      = pktGetBitsPack( pak, 1 );
    BasePower * toPower = 0;

    assert( pchar );

    if( iFromIdx >= CHAR_BOOST_MAX
        || iToIdx >= pchar->iNumBoostSlots
        || pchar->aBoosts[iFromIdx] == NULL)
        return false;

    // do the logging first
    if( pchar->aBoosts[iToIdx] )
    {
        LOG_ENT(pchar->entParent, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Character:BoostInventory Swapping %s.%d+%d at %d with %s.%d+%d at %d",
            dbg_BasePowerStr(pchar->aBoosts[iFromIdx]->ppowBase),
            pchar->aBoosts[iFromIdx]->iLevel,
            pchar->aBoosts[iFromIdx]->iNumCombines,
            iToIdx,
            dbg_BasePowerStr(pchar->aBoosts[iToIdx]->ppowBase),
            pchar->aBoosts[iToIdx]->iLevel,
            pchar->aBoosts[iToIdx]->iNumCombines,
            iFromIdx);
    }
    else
    {
        LOG_ENT(pchar->entParent, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Character:BoostInventory Moving %s.%d+%d from %d to %d",
            dbg_BasePowerStr(pchar->aBoosts[iFromIdx]->ppowBase),
            pchar->aBoosts[iFromIdx]->iLevel,
            pchar->aBoosts[iFromIdx]->iNumCombines,
            iFromIdx,
            iToIdx);
    }

    // swap the boosts
    character_SwapBoosts( pchar, iFromIdx, iToIdx );

    return true;
}

/**********************************************************************func*
 * character_ReceiveRemoveBoost
 *
 * NET: fully consumes data even on error
 * NET: returns false on bad data
 */
bool character_ReceiveRemoveBoost( Packet *pak, Character *pchar)
{
    int idx = pktGetBitsPack( pak, 1 );

    assert( pchar );

    if( idx < 0 || idx >= CHAR_BOOST_MAX )
        return false;

    if(pchar->aBoosts[idx])
    {
        LOG_ENT(pchar->entParent, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Character:BoostInventory Removing %s.%d+%d from %d",
            dbg_BasePowerStr(pchar->aBoosts[idx]->ppowBase),
            pchar->aBoosts[idx]->iLevel,
            pchar->aBoosts[idx]->iNumCombines,
            idx);
    }
    else
    {
        LOG_ENT(pchar->entParent, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Character:BoostInventory Removing from %d failed, no boost is there.", idx);
    }
    
    character_DestroyBoost( pchar, idx, "trash" );

    return true;
}
/**********************************************************************func*
* character_ReceiveRemoveBoostFromPower
*
* NET: fully consumes data even on error
* NET: returns false on bad data
*/
bool character_ReceiveRemoveBoostFromPower(Packet *pak, Character *pchar)
{
    int iset  = pktGetBitsPack( pak, 1 );
    int ipow  = pktGetBitsPack( pak, 1 );
    int ispec = pktGetBitsPack( pak, 1 );

    Power *ppow = NULL;

    assert( pchar );

    if( ispec<0
        || iset>=eaSize(&pchar->ppPowerSets)
        || ipow<0
        || ipow>=eaSize(&pchar->ppPowerSets[iset]->ppPowers))
    {
        return false;
    }

    ppow = pchar->ppPowerSets[iset]->ppPowers[ipow];

    if(ppow == NULL
        || ispec>=eaSize(&ppow->ppBoosts)
        || ppow->ppBoosts[ispec]==NULL)
    {
        return false;
    }

    LOG_ENT( pchar->entParent, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "DestroyBoost %s.%d+%d deleted from %s.%d",
        dbg_BasePowerStr(ppow->ppBoosts[ispec]->ppowBase),
        ppow->ppBoosts[ispec]->iLevel,
        ppow->ppBoosts[ispec]->iNumCombines,
        dbg_BasePowerStr(ppow->ppowBase),
        ispec);

    power_DestroyBoost( pchar->ppPowerSets[iset]->ppPowers[ipow], ispec, "trash" );

    return true;
}


/**********************************************************************func*
* character_ReceiveUnslotBoostFromPower
*
* NET: fully consumes data even on error
* NET: returns false on bad data
*/
bool character_ReceiveUnslotBoostFromPower(Packet *pak, Character *pchar)
{
    int iset  = pktGetBitsPack( pak, 1 );
    int ipow  = pktGetBitsPack( pak, 1 );
    int ispec = pktGetBitsPack( pak, 1 );
    int invspot = pktGetBitsPack( pak, 1 );
    const SalvageItem *item = salvage_GetItem("S_EnhancementUnslotter");
    int count = 0;

    Power *ppow = NULL;

    assert( pchar );

    if( ispec<0
        || iset>=eaSize(&pchar->ppPowerSets)
        || ipow<0
        || ipow>=eaSize(&pchar->ppPowerSets[iset]->ppPowers)
        || invspot<0
        || invspot>=pchar->iNumBoostSlots)
    {
        return false;
    }

    ppow = pchar->ppPowerSets[iset]->ppPowers[ipow];

    // verify power is good
    if(ppow == NULL
        || ispec>=eaSize(&ppow->ppBoosts)
        || ppow->ppBoosts[ispec]==NULL)
    {
        return false;
    }

    // verify that inventory spot is open
    if (pchar->aBoosts[invspot]!=NULL)
    {
        return false;
    }

    // verify player has unslotters
    if (item != NULL)
    {
        if (!character_CanSalvageBeChanged(pchar, item, -1))
            return false;
    } else {
        return false;
    }

    LOG_ENT( pchar->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "UnslotBoost %s.%d+%d deleted from %s.%d",
        dbg_BasePowerStr(ppow->ppBoosts[ispec]->ppowBase),
        ppow->ppBoosts[ispec]->iLevel,
        ppow->ppBoosts[ispec]->iNumCombines,
        dbg_BasePowerStr(ppow->ppowBase),
        ispec);

    // put boost in inventory
    character_SetBoost(pchar, invspot, ppow->ppBoosts[ispec]->ppowBase, ppow->ppBoosts[ispec]->iLevel, 
                        ppow->ppBoosts[ispec]->iNumCombines, NULL, 0, NULL, 0);

    // remove boost from power
    power_DestroyBoost( pchar->ppPowerSets[iset]->ppPowers[ipow], ispec, "unslot" );

    // remove unslotter
    character_AdjustSalvage(pchar, item, -1, "unslot", true);

    return true;
}


/**********************************************************************func*
 * power_ReceiveSetBoost
 *
 * NET: fully consumes data even on error
 * NET: returns false on bad data
 */
bool power_ReceiveSetBoost( Packet *pak, Character *pchar)
{
    int iLevel;
    int iset     = pktGetBitsPack( pak, 1 );
    int ipow     = pktGetBitsPack( pak, 1 );
    int iFromIdx = pktGetBitsPack( pak, 1 );
    int iToIdx   = pktGetBitsPack( pak, 1 );

    Power *ppow;

    assert( pchar );

    if( iset >= eaSize( &pchar->ppPowerSets ) || iset < 0  )
        return false;

    if( ipow >= eaSize( &pchar->ppPowerSets[iset]->ppPowers ) || ipow < 0 )
        return false;

    ppow = pchar->ppPowerSets[iset]->ppPowers[ipow];

    iLevel = character_CalcExperienceLevel(pchar);

    if( !ppow
        || iFromIdx < 0
        || iFromIdx >= pchar->iNumBoostSlots
        || iToIdx < 0
        || iToIdx >= eaSize(&ppow->ppBoosts)
        || pchar->aBoosts[iFromIdx]==NULL
        || !boost_IsValidToSlotAtLevel(pchar->aBoosts[iFromIdx], pchar->entParent, iLevel, pchar->entParent->erOwner)
        || !power_IsValidBoost(ppow, pchar->aBoosts[iFromIdx]) )
    {
        return false;
    }
    else
    {
        int iValidToIdx = power_ValidBoostSlotRequired(ppow, pchar->aBoosts[iFromIdx]);
        if(iValidToIdx>0 && iValidToIdx != iToIdx)
        {
            return false;
        }
    }

    if (!power_BoostCheckRequires(ppow, pchar->aBoosts[iFromIdx]))
        return false;

    if(ppow->ppBoosts[iToIdx])
    {
        LOG_ENT( pchar->entParent, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "ReplaceBoost Slotting %s.%d+%d to %s.%d replacing %s.%d+%d",
        dbg_BasePowerStr(pchar->aBoosts[iFromIdx]->ppowBase),
        pchar->aBoosts[iFromIdx]->iLevel,
        pchar->aBoosts[iFromIdx]->iNumCombines,
        dbg_BasePowerStr(ppow->ppowBase),
        iToIdx,
        dbg_BasePowerStr(ppow->ppBoosts[iToIdx]->ppowBase),
        ppow->ppBoosts[iToIdx]->iLevel,
        ppow->ppBoosts[iToIdx]->iNumCombines);
    }
    else
    {
        LOG_ENT( pchar->entParent, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "ReplaceBoost Slotting %s.%d+%d to %s.%d replacing nothing",
        dbg_BasePowerStr(pchar->aBoosts[iFromIdx]->ppowBase),
        pchar->aBoosts[iFromIdx]->iLevel,
        pchar->aBoosts[iFromIdx]->iNumCombines,
        dbg_BasePowerStr(ppow->ppowBase),
        iToIdx);
    }

    power_ReplaceBoostFromBoost(ppow, iToIdx, pchar->aBoosts[iFromIdx], "used" );

    eaPush(&pchar->entParent->powerDefChange, powerRef_CreateFromPower(pchar->iCurBuild, ppow));

    character_DestroyBoost(pchar, iFromIdx, NULL);

    return true;

}

// NET: fully consumes data even on error
// NET: returns false on bad data
bool character_ReceiveLevelAndBuyPower(Packet *pak, Character *pchar)
{
    const BasePower *ppowBase;
    int uniqueID;
    PowerSystem eSys;

    ppowBase = basepower_Receive(pak, &g_PowerDictionary, (pchar ? pchar->entParent : 0));
    uniqueID = pktGetBitsAuto(pak);
    eSys = pktGetBitsPack(pak, kPowerSystem_Numbits);

    // This forces a full redefine to be sent.
    pchar->entParent->level_update = 1;
    eaPush(&pchar->entParent->powerDefChange, (void *)-1);

    // If the definition of what you can get at a level has changed, then
    // you may not be able to level but still be able to buy a power.
    if(pchar->entParent->access_level>=ACCESS_DEBUG || character_IsLeveling(pchar))
    {
        bool bLeveled = false;

        if(character_CanLevel(pchar) && character_CountPowersBought(pchar) >= 2)
        {
            character_LevelUp(pchar);
            bLeveled = true;
        }

        if(ppowBase!=NULL && character_CanBuyPower(pchar) && character_IsAllowedToHavePower(pchar, ppowBase))
        {
            PowerSet *pset;

            if(character_OwnsPower(pchar, ppowBase)==NULL)
            {
                if((pset=character_OwnsPowerSet(pchar, ppowBase->psetParent))==NULL)
                {
                    bool bBoughtPower = false;

                    // The character doesn't own it the set yet.
                    if(baseset_IsSpecialization(ppowBase->psetParent)
                        && character_IsAllowedToHavePowerSetHypothetically(pchar, ppowBase->psetParent))
                    {
                        // Specialization sets are only supposed to be added to primaries or secondaries by design.
                        if(ppowBase->psetParent->pcatParent==pchar->pclass->pcat[kCategory_Primary]
                            || ppowBase->psetParent->pcatParent==pchar->pclass->pcat[kCategory_Secondary])
                        {
                            pset = character_BuyPowerSet(pchar, ppowBase->psetParent);
                            LOG_ENT( pchar->entParent, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "BuyPowerSet %s", ppowBase->psetParent->pchName);
                            bBoughtPower = true;
                        }
                    }

                    if(!bBoughtPower && character_CanBuyPoolPowerSet(pchar)
                        && character_IsAllowedToHavePowerSetHypothetically(pchar, ppowBase->psetParent))
                    {
                        // You can get new power sets only from the pool.
                        if(ppowBase->psetParent->pcatParent==pchar->pclass->pcat[kCategory_Pool])
                        {
                            pset = character_BuyPowerSet(pchar, ppowBase->psetParent);
                            LOG_ENT( pchar->entParent, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "BuyPowerSet %s", ppowBase->psetParent->pchName);
                            bBoughtPower = true;
                        }
                    }

                    if(!bBoughtPower && character_CanBuyEpicPowerSet(pchar)
                        && character_IsAllowedToHavePowerSetHypothetically(pchar, ppowBase->psetParent))
                    {
                        // You can get new power sets only from the epic pool.
                        if(ppowBase->psetParent->pcatParent==pchar->pclass->pcat[kCategory_Epic])
                        {
                            pset = character_BuyPowerSet(pchar, ppowBase->psetParent);
                            LOG_ENT( pchar->entParent, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "BuyPowerSet %s", ppowBase->psetParent->pchName);
                            bBoughtPower = true;
                        }
                    }

                    if(!bBoughtPower)
                    {
                        LOG_ENT( pchar->entParent, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "BuyPowerSet:Error Buying power set %s disallowed (owns %d, can get %d)", ppowBase->psetParent->pchName, character_CountPoolPowerSetsBought(pchar), CountForLevel(pchar->iLevel, g_Schedules.aSchedules.piPoolPowerSet));
                    }
                }

                if(pset!=NULL)
                {
                    // BuyPower is a command; it doesn't do any level checks.
                    if (character_BuyPower(pchar, pset, ppowBase, uniqueID) != NULL)
                    {
                        pchar->entParent->general_update = 1;
                        character_LevelFinalize(pchar, true);
                        alignmentshift_revokeAllBadTips(pchar->entParent);
                        if(ppowBase->eType==kPowerType_Auto)
                        {
                            character_ActivateAllAutoPowers(pchar);
                        }

                        LOG_ENT( pchar->entParent, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "BuyPower %s", dbg_BasePowerStr(ppowBase));

                        pchar->entParent->pl->levelling = 0;
                        svrSendEntListToDb( &pchar->entParent, 1 );
                        return true;
                    }
                    else
                    {
                        LOG_ENT( pchar->entParent, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "BuyPower:Error Unable to buy power %s", dbg_BasePowerStr(ppowBase));
                    }
                }
                else
                {
                    LOG_ENT( pchar->entParent, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "BuyPowerSet:Error Unable to buy power set %s", ppowBase->psetParent->pchName);
                }
            }
            else
            {
                LOG_ENT( pchar->entParent, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "BuyPower:Error Already has power %s", dbg_BasePowerStr(ppowBase));
            }
        }
        else
        {
            LOG_ENT( pchar->entParent, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "BuyPower:Error (%s level up) 0x%p==NULL || %s buy power || %s allowed to have it",
                bLeveled ? "did" : "did not",
                ppowBase,
                character_CanBuyPower(pchar) ? "cannot" : "can",
                character_IsAllowedToHavePower(pchar, ppowBase) ? "is" : "is not");
        }

        if(bLeveled)
            character_LevelDown(pchar);
    }
    else
    {
        const ContactDef *contactDef=NULL;
        if(pchar->entParent->storyInfo->interactTarget!=0)
            contactDef = ContactDefinition(pchar->entParent->storyInfo->interactTarget);

        LOG_ENT( pchar->entParent, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "BuyPower:Error Not leveling (target %d, %s, %s)",
            pchar->entParent->storyInfo->interactTarget,
            contactDef? contactDef->filename : "(unknown)",
            contactDef?(CONTACT_IS_TRAINER(contactDef)?"trainer":"NOT trainer"):"(unknown)");
    }

    pchar->entParent->pl->levelling = 0;
    return false;
}

/**********************************************************************func*
 * power_ReceiveLevelAndAssignBoostSlot
 *
 * NET: fully consumes data even on error
 * NET: returns false on bad data
 */
bool power_ReceiveLevelAndAssignBoostSlot(Packet *pak, Character *pchar)
{
    bool ret=true;
    assert(pchar);

    // This forces a full redefine to be sent.
    pchar->entParent->level_update = 1;
    eaPush(&pchar->entParent->powerDefChange, (void *)-1);

    if(pchar->entParent->access_level>=ACCESS_DEBUG || character_IsLeveling(pchar))
    {
        bool bLeveled = false;
        int iCnt = pktGetBitsPack(pak, 1);
        int i;
        PowerSystem eSys = pktGetBitsPack(pak, kPowerSystem_Numbits);
        

        if(character_CanLevel(pchar))
        {
            character_LevelUp(pchar);
            bLeveled = true;
        }

        for(i=0; i<iCnt; i++)
        {
            Power *ppow;
            int iset = pktGetBitsPack(pak, 1);
            int ipow = pktGetBitsPack(pak, 1);

            if(character_CanBuyBoost(pchar)
                && iset<eaSize(&pchar->ppPowerSets)
                && iset>=1 // The zeroth slot in power sets is for temporary powers
                && ipow<eaSize(&pchar->ppPowerSets[iset]->ppPowers)
                && ipow>=0
                && pchar->ppPowerSets[iset]->ppPowers[ipow]!=NULL)
            {
                ppow = pchar->ppPowerSets[iset]->ppPowers[ipow];

                // BuyBoostSlot is a command; it doesn't do any level checks.
                if(character_BuyBoostSlot(pchar, ppow))
                {
                    character_LevelFinalize(pchar, true);
                    alignmentshift_revokeAllBadTips(pchar->entParent);
                    dbLog("Character:AssignSlots", pchar->entParent, "Assigned slot to %s", dbg_BasePowerStr(ppow->ppowBase));
                }
                else
                {
                    dbLog("Character:AssignSlots:Error", pchar->entParent, "Unable to assign slot to %s", dbg_BasePowerStr(ppow->ppowBase));
                    ret = false;
                }
            }
            else
            {
                dbLog("Character:AssignSlots:Error", pchar->entParent, "Unable to assign slot. (%s && %d<%d, && %d>=1 && %d<%d && %d>=0 && 0x%p!=NULL)",
                    character_CanBuyBoost(pchar)?"can buy":"CANNOT buy",
                    iset, eaSize(&pchar->ppPowerSets),
                    iset,
                    ipow, (iset<eaSize(&pchar->ppPowerSets) && iset>=1) ? eaSize(&pchar->ppPowerSets[iset]->ppPowers) : -1,
                    ipow, (iset<eaSize(&pchar->ppPowerSets) && iset>=1 && ipow<eaSize(&pchar->ppPowerSets[iset]->ppPowers) && ipow>=0) ? pchar->ppPowerSets[iset]->ppPowers[ipow] : (void *)-1);

                ret = false;
            }
        }

        if (iCnt == 0)
        {
            dbLog("Character:AssignSlots:Error", pchar->entParent, "Character attempted to assign 0 slots at level up");
            ret = false;
        }

        if(!ret && bLeveled)
        {
            character_LevelDown(pchar);
        }
    }
    else
    {
        const ContactDef *contactDef=NULL;

        // Burn away the bits.
        int iCnt = pktGetBitsPack(pak, 1);
        while(iCnt>0)
        {
            pktGetBitsPack(pak, 1);
            pktGetBitsPack(pak, 1);
            iCnt--;
        }

        if(pchar->entParent->storyInfo->interactTarget!=0)
            contactDef = ContactDefinition(pchar->entParent->storyInfo->interactTarget);

        dbLog("Character:AssignSlots:Error", pchar->entParent, "Not leveling (target %d, %s, %s)",
            pchar->entParent->storyInfo->interactTarget,
            contactDef? contactDef->filename : "(unknown)",
            contactDef?(CONTACT_IS_TRAINER(contactDef)?"trainer":"NOT trainer"):"(unknown)");
    }

    pchar->entParent->pl->levelling = 0;
    if(ret)
        svrSendEntListToDb( &pchar->entParent, 1 );

    return ret;
}

/**********************************************************************func*
 * character_ReceiveLevel
 *
 * NET: fully consumes data even on error
 * NET: returns false on bad data
 */
bool character_ReceiveLevel(Packet *pak, Character *pchar)
{
    // This forces a full redefine to be sent.
    PowerSystem eSys = pktGetBits( pak, kPowerSystem_Numbits );
    pchar->entParent->level_update = 1;
    eaPush(&pchar->entParent->powerDefChange, (void *)-1);

    if(pchar->entParent->access_level>=ACCESS_DEBUG || character_IsLeveling(pchar))
    {
        // This command is only allowed if you can't buy a power or boost.
        if(!character_CanBuyPower(pchar) && !character_CanBuyBoost(pchar))
        {
            if(character_CanLevel(pchar))
            {
                dbLog("Character:OnlyLevel", pchar->entParent, "");
                character_LevelUp(pchar);
                character_LevelFinalize(pchar, true);
                alignmentshift_revokeAllBadTips(pchar->entParent);
                pchar->entParent->pl->levelling = 0;
                svrSendEntListToDb( &pchar->entParent, 1 );
                return true;
            }
            else
            {
                dbLog("Character:OnlyLevel:Error", pchar->entParent, "Attempted to level, but can't.");
            }
        }
        else
        {
            dbLog("Character:OnlyLevel:Error", pchar->entParent, "Attempted to only level, but should be buying something.");
        }
    }
    else
    {
        const ContactDef *contactDef=NULL;
        if(pchar->entParent->storyInfo->interactTarget!=0)
            contactDef = ContactDefinition(pchar->entParent->storyInfo->interactTarget);

        dbLog("Character:OnlyLevel:Error", pchar->entParent, "Not leveling (target %d, %s, %s)",
            pchar->entParent->storyInfo->interactTarget,
            contactDef? contactDef->filename : "(unknown)",
            contactDef?(CONTACT_IS_TRAINER(contactDef)?"trainer":"NOT trainer"):"(unknown)");
    }

    pchar->entParent->pl->levelling = 0;
    return false;
}


void character_toggleAuctionWindow(Entity *e, int windowUp, int npcId)
{
    // Either we're closing the window and so there's no owning NPC or
    // we're opening it and must have a valid NPC ID.
    verify((windowUp == 0 && npcId == 0) || (windowUp > 0 && npcId > 0));

    if (!accountLoyaltyRewardHasNode(e->pl->loyaltyStatus, "RemoteAuction"))
    {
        //    if they own the node, none of this stuff is relevant to them
        //    it sets the distance they can be from the npc before the window closes
        //    but if they have the badge, the distance is the map
        e->pl->at_auction = windowUp;
        e->pl->npcStore = npcId;
    }
    else if (e->pl->at_auction)
    {
        //    this is for the rare chance that they get awarded the badge while at the auction house
        e->pl->at_auction = e->pl->npcStore = 0;
    }

    //    if they own the node, the windowUp "or" check allows for the convenience of clicking 
    //    the contact isntead of typing the command (if they so choose)
    if (!accountLoyaltyRewardHasNode(e->pl->loyaltyStatus, "RemoteAuction") || windowUp)
    {
        START_PACKET( pak, e, SERVER_AUCTION_OPEN );
        pktSendBits( pak,  1, windowUp );
        END_PACKET 
    }

}


//------------------------------------------------------------
//  Repairs any user variable corruption
//----------------------------------------------------------
static int repairUICorruption(Entity *e, int justChecking)
{
    int changed=0;
    // Stick any checks for out of bounds ui variables here, as they pop up
    
    if (e->costume->appearance.fScale > 36 || e->costume->appearance.fScale < -36)
    {
        Costume* mutable_costume;

        if (justChecking)
            return 1;
        
        mutable_costume = entGetMutableCostume(e);
        mutable_costume->appearance.fScale = 0.0;

        dbLog("Corruption:Player", e, "Body scale was outside of acceptable range, reset");
        changed = 1;
    }

    if (e->pl->chat_font_size < 3 || e->pl->chat_font_size > 25)
    {
        if (justChecking)
        {
            return 1;
        }
        e->pl->chat_font_size = 12;
        dbLog("Corruption:Player", e, "Chat font size was outside of acceptable range, reset");
        changed = 1;
    }

    return changed;

}

static bool s_RepairRewardTokens(Entity *e, int justChecking)
{
    bool res = false;
    if( e && e->pl )
    {
        int i;
        for( i = eaSize( &e->pl->rewardTokens ) - 1; i >= 0; --i)
        {
            RewardToken *tok = e->pl->rewardTokens[i];
            if(!tok || !tok->reward )
            {
                res = true;
                if( justChecking )
                    break;
                else
                    eaRemove(&e->pl->rewardTokens,i);
            }
        }
    }
    return res;
}

static bool repairKeybindCorruption(Entity *e, int justChecking)
{
    int i, ret = 0;
    int *bad_indicies = NULL, last_idx = 0;

    for ( i = 0; i < BIND_MAX; ++i )
    {
        if ( e->pl->keybinds[i].key )
        {
            last_idx = i;
            if ( e->pl->keybinds[i].modifier >= 4 || e->pl->keybinds[i].modifier < 0 )
            {
                ret = 1;
                if ( justChecking )
                    return ret;
                else
                {
                    memset(&e->pl->keybinds[i], 0, sizeof(ServerKeyBind));
                    eaiPush(&bad_indicies, i);
                }
            }
        }
    }

    while ( i = eaiPop(&bad_indicies) )
    {
        swap(&e->pl->keybinds[i], &e->pl->keybinds[last_idx--], sizeof(ServerKeyBind));
    }
    eaiDestroy(&bad_indicies);
    return ret;
}

//------------------------------------------------------------
//  Repairs corruption on a character, returning 1 if any found
//----------------------------------------------------------
int player_RepairCorruption(Entity *e, int justChecking)
{
    int save_changes = 0;
    // Put in corruption repair here
    // If any repair occurs and we need to save the ent, it should return 1;
    // If justChecking is passed on, don't actually modify the ent, just return if need repair

    save_changes |= repairUICorruption(e,justChecking);
    save_changes |= s_RepairRewardTokens(e,justChecking);
    save_changes |= repairKeybindCorruption(e, justChecking);

    return save_changes;
}


/* End of File */
