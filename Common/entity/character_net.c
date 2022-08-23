/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h> // MAX
#include <utilitieslib/network/netio.h>
#include <utilitieslib/components/earray.h>

#include "entity/powers.h"
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/utils.h> // INRANGE

#include "entity/character_base.h"
#include "entity/character_net.h"
#include "entity/character_level.h"
#include "entity/character_inventory.h"

#ifdef SERVER
#include "cmdparse/cmdserver.h"
#include "Reward.h"
#include "dbcomm/logcomm.h"
#include "dbcomm/dbcomm.h"
#include "gameSys/Turnstile.h"
#elif CLIENT
#include "UI/uiRecipeInventory.h"
#include "UI/uiIncarnate.h"
#include "UI/uiTray.h"
#include "clientcomm/clientcomm.h"
#include "comm_game.h"
#endif
#include "entity/entity.h"
#include "gameComm/VillainDef.h"
#include "entity/salvage.h"
#include "entity/Concept.h"


/**********************************************************************func*
 * basepower_Send
 *
 */
void basepower_Send(Packet *pak, const BasePower *ppow)
{
    pktSendBitsAuto(pak, ppow ? ppow->crcFullName : 0);
}

/**********************************************************************func*
 * basepower_Receive
 *
 */
const BasePower *basepower_Receive(Packet *pak, const PowerDictionary *pdict, Entity *receivingEntity)
{
    const BasePower *ppow;
    char *catName = 0;
    char *setName = 0;

    int crcFullName = pktGetBitsAuto(pak);
    if((ppow = powerdict_GetBasePowerByCRC(pdict, crcFullName))==NULL)
    {
#if CLIENT
        Packet *pak = pktCreateEx(&comm_link, CLIENT_BASEPOWER_NOT_FOUND);
        pktSendBitsAuto(pak, (receivingEntity ? receivingEntity->db_id : 0));
        pktSendBitsAuto(pak, (receivingEntity ? receivingEntity->id : 0));
        pktSendBitsAuto(pak, crcFullName);
        pktSend(&pak,&comm_link);

        lnkFlushAll();
#else
        LOG_ENT( receivingEntity, LOG_ERROR, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, 
            "BasePower:NotFound received <CRC value:%d>", crcFullName);
#endif
        //JE: I've fixed the code above this on the MapServer so that we can safely
        // continue here and it will disconnect the invalid client
        // ARM: This code is no longer a clean disconnect, it'll crash the client at least some of the time
        // Continue and ppow is NULL.
    }

    return ppow;
}

/**********************************************************************func*
 * SafeGetPowerByIdx
 *
 */
Power *SafeGetPowerByIdx(Character *p, int ibuild, int iset, int ipow)
{
    PowerSet *pset;

    assert(p != NULL);

    if (ibuild < 0 || ibuild >= MAX_BUILD_NUM
        || iset < 0 || iset >= eaSize(&p->ppBuildPowerSets[ibuild]))
    {
        // Bad power set index.
        return NULL;
    }

    pset = p->ppBuildPowerSets[ibuild][iset];
    if (ipow < 0 || ipow >= eaSize(&pset->ppPowers))
    {
        // Bad power index.
        return NULL;
    }

    return pset->ppPowers[ipow];
}

#define POWERUPSLOT_COUNT_PACKBITS 1
#define POWERUPSLOT_TYPE_PACKBITS 1
#define POWERUPSLOT_ID_PACKBITS 5

/**********************************************************************func*
* boost_Send
*
*/
void boost_Send(Packet *pak, Character *pchar, int idx)
{
    // boost slot index
    // 0
    //   empty slot
    // 1
    //   basepower
    //   level

    pktSendBitsPack(pak, 3, idx);
    if(pchar->aBoosts[idx]!=NULL)
    {
        int i;
        int numPowerupSlots;
        Boost *boost = pchar->aBoosts[idx];
        
        pktSendBits(pak, 1, 1);
        basepower_Send(pak, pchar->aBoosts[idx]->ppowBase);
        pktSendBitsPack(pak, 5, pchar->aBoosts[idx]->iLevel);
        pktSendBitsPack(pak, 2, pchar->aBoosts[idx]->iNumCombines);

        // send the invented vars
        for( i = 0; i < ARRAY_SIZE( boost->afVars ); ++i ) 
        {
            pktSendF32( pak, boost->afVars[i] );
        }

        // --------------------
        // send the powerups
        
        // count them
        numPowerupSlots = powerupslot_ValidCount( boost->aPowerupSlots, ARRAY_SIZE( boost->aPowerupSlots ) );

        // send them
        pktSendBitsPack( pak, POWERUPSLOT_COUNT_PACKBITS, numPowerupSlots );
        for( i = 0; i < numPowerupSlots; ++i ) 
        {
            PowerupSlot *slot = &boost->aPowerupSlots[i];
            int id = rewarditemtype_IdFromName( slot->reward.type, slot->reward.name );

            pktSendBitsPack( pak, POWERUPSLOT_TYPE_PACKBITS, slot->reward.type );
            pktSendBitsPack( pak, POWERUPSLOT_ID_PACKBITS, id );
            pktSendBitsPack( pak, 1, slot->filled );
        }
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }
}

/**********************************************************************func*
* character_SendBoosts
*
*/
void character_SendBoosts(Packet *pak, Character *pchar)
{
    int j;

    // count of inspirations
    //    inspiration

    pktSendBitsPack(pak, 5, CHAR_BOOST_MAX);
    for(j=0; j<CHAR_BOOST_MAX; j++)
    {
        boost_Send(pak, pchar, j);
    }
}


/**********************************************************************func*
* character_SendSalvages
*
*/
#define INV_COUNT_PACKBITS 8
#define SALVAGEINV_IDX_PACKBITS 1
#define INV_ITEMID_PACKBITS 16
#define INV_AMOUNT_PACKBITS 5

//------------------------------------------------------------
//  send a reverse engineer command to the server
//  -AB: created :2005 Feb 28 11:47 AM
//----------------------------------------------------------
void salvageinv_ReverseEngineer_Send(Packet *pak, int inv_idx )
{
    if(inv_idx > 0)
    {
        pktSendBitsPack( pak, SALVAGEINV_IDX_PACKBITS, inv_idx );
    }
    else
    {
        pktSendBitsPack( pak, SALVAGEINV_IDX_PACKBITS, kSalvageId_Invalid );
    }
}

#ifdef SERVER
//------------------------------------------------------------
// receive salvage
// see salvageinv_ReverseEngineer_Send
// -AB: created :2005 Feb 28 02:38 PM
//----------------------------------------------------------
void salvageinv_ReverseEngineer_Receive(Packet *pak, Character *pchar)
{
    int idx = pktGetBitsPack( pak, SALVAGEINV_IDX_PACKBITS );
    Entity *e = pchar->entParent;

    if( !verify(EAINRANGE( idx, pchar->salvageInv )) )
    {
        dbLog("cheater",NULL,"Player with authname %s tried to send index %d outside range of inventory array of size %d\n", e->auth_name, idx,  eaSize( &pchar->salvageInv ));
    }
    else if( !pchar->salvageInv[idx]->salvage )
    {
        dbLog("cheater",NULL,"Player with authname %s tried to reverse engineer null salvage index %d\n", e->auth_name, idx );
    }
    else
    {
        SalvageItem const *sal = pchar->salvageInv[idx]->salvage;
        character_ReverseEngineerSalvage( pchar, idx );
    }
}
#endif


// end salvages section
// ------------------------------------------------------------


// ------------------------------------------------------------
// Generic inventory

#define GENERICINV_TYPE_PACKBITS 5
#define GENERICINV_ID_PACKBITS 5
#define GENERICINV_IDX_PACKBITS 5
#define GENERICINV_AMOUNT_PACKBITS 5

//------------------------------------------------------------
//  Receive an item.
// See character_inventory_Receive as well which duplicates
// some of this functionality
// Net: always sends data
//----------------------------------------------------------
void conceptitem_Send(ConceptItem *p, Packet *pak)
{
    int i;
    int id = 0;
    F32 afVars[ARRAY_SIZE( p->afVars )] = {0} ; 
    
    if( verify( p && p->def && pak ))
    {
        id = p->def->id;
        CopyStructs(afVars, p->afVars, ARRAY_SIZE( p->afVars ));
    }
    
    // send the id
    pktSendBitsPack( pak, GENERICINV_ID_PACKBITS, id );
    
    // send the afVars
    for( i = 0; i < ARRAY_SIZE( afVars ); ++i ) 
    {
        // note: could be optimized to just the valid vars for the conceptdef
        pktSendF32(pak, afVars[i]);
    }
    
}

//------------------------------------------------------------
//  Receive an item.
// See character_inventory_Receive as well which duplicates
// some of this functionality
// Net: fully consumes data, even on error
//----------------------------------------------------------
ConceptItem* conceptitem_Receive(ConceptItem *resItem, Packet *pak)
{
    int id = pktGetBitsPack( pak, GENERICINV_ID_PACKBITS );
    F32 afVars[TYPE_ARRAY_SIZE( ConceptItem, afVars )];
    int i;

    // get the vars
    for( i = 0; i < ARRAY_SIZE(afVars); ++i ) 
    {
        afVars[i] = pktGetF32(pak);
    }

    resItem = conceptitem_Init( resItem, id, afVars );
    return resItem;
}

//------------------------------------------------------------
//  Send the generic inventory item
// param type: the type into the Character of the generic inventory item list
// Net: always sends bits. id of zero assumed to mean invalid
//----------------------------------------------------------
void character_inventory_Send(Character *p, Packet *pak, InventoryType type, int idx)
{
    int id = 0;
    int amount = 0;
    character_GetInvInfo(p, &id, &amount, NULL, type, idx);

    // always send the type, index, and amount
    pktSendBitsPack( pak, GENERICINV_TYPE_PACKBITS, type );
    pktSendBitsPack( pak, GENERICINV_IDX_PACKBITS, idx );
    pktSendBitsPack( pak, GENERICINV_AMOUNT_PACKBITS, amount );

    // --------------------
    // type specific extra data

    switch ( type )
    {
    case kInventoryType_Concept:
        {
            conceptitem_Send(  verify( EAINRANGE( idx, p->conceptInv) && p->conceptInv[idx] ) ? p->conceptInv[idx]->concept : NULL, pak );
        }
        break;
    default:
        // just send the id
        pktSendBitsPack( pak, GENERICINV_ID_PACKBITS, id); 
        break;
    };
}


//------------------------------------------------------------
//  receive inventory packets
// Net: fully consumes data on error
//----------------------------------------------------------
void character_inventory_Receive(Character *p, Packet *pak )
{
    // always get the type index and amount
    int type = pktGetBitsPack( pak, GENERICINV_TYPE_PACKBITS );
    int idx = pktGetBitsPack( pak, GENERICINV_IDX_PACKBITS );
    int amount = pktGetBitsPack( pak, GENERICINV_AMOUNT_PACKBITS );
    int id = pktGetBitsPack( pak, GENERICINV_ID_PACKBITS );

    switch ( type )
    {
    case kInventoryType_Concept:
    {
        F32 afVars[TYPE_ARRAY_SIZE( ConceptItem, afVars )];
        int i;

        // get the vars
        for( i = 0; i < ARRAY_SIZE(afVars); ++i ) 
        {
            afVars[i] = pktGetF32(pak);
        }

        character_SetConcept(p, id, afVars, idx, amount );
    }
    break;
    default:
    {
        character_SetInventory(p, type, id, idx, amount, __FUNCTION__ );
    }
    };
}

//------------------------------------------------------------
//  send inventory sizes
//----------------------------------------------------------
void character_SendInventorySizes(Character *pchar, Packet *pak )
{
    verify (pak);
    if( pchar && pchar->entParent && pchar->entParent->pl)  // only players have inventories we really care about
    {
        int type;
        int count = kInventoryType_Count;

        pktSendBitsPack( pak, 1, 1 );
        pktSendBitsPack( pak, 3, count );

        for( type = 0; type < count ; ++type )
        {
            int size = character_GetInvSize( pchar, type );
            int totalSize = character_GetInvTotalSize( pchar, type );

            // send the count
            pktSendBitsPack( pak, INV_COUNT_PACKBITS, totalSize );
            pktSendBitsPack( pak, INV_COUNT_PACKBITS, size );
        }

        // additional information for auction inventory
        pktSendBitsPack( pak, INV_COUNT_PACKBITS, character_GetAuctionInvTotalSize(pchar) );
        
        // additional information for auction boosts (enhancements)
        pktSendBitsPack( pak, INV_COUNT_PACKBITS, character_GetBoostInvTotalSize(pchar) );

    } else {
        pktSendBitsPack( pak, 1, 0 );
    }
}


//------------------------------------------------------------
//  send all the items in inventory
//----------------------------------------------------------
void character_SendInventories(Character *pchar, Packet *pak )
{
    if( verify( pak && pchar ))
    {
        int type;
        int count = kInventoryType_Count;

         pktSendBitsPack( pak, 3, count );
        
        for( type = 0; type < count ; ++type )
        {
            int i;
            int size = character_GetInvSize( pchar, type );
            int totalSize = character_GetInvTotalSize( pchar, type );

             // send the count
            pktSendBitsPack( pak, INV_COUNT_PACKBITS, totalSize );
             pktSendBitsPack( pak, INV_COUNT_PACKBITS, size );
            for( i = 0; i < size; ++i ) 
            {
                character_inventory_Send(pchar, pak, type, i);
            }
        }

        // additional information for auction inventory
        pktSendBitsPack( pak, INV_COUNT_PACKBITS, character_GetAuctionInvTotalSize(pchar) );

    }
}

//------------------------------------------------------------
//  receive inventory sizes
//----------------------------------------------------------
void character_ReceiveInventorySizes(Character *pchar, Packet *pak )
{
    if( verify( pak ))
    {
        int type;
        int valid = pktGetBitsPack( pak, 1 );

        if (valid)
        {
            int count = pktGetBitsPack( pak, 3 );

            for( type = 0; type < count; ++type ) 
            {
                int totalSize = pktGetBitsPack( pak, INV_COUNT_PACKBITS);
                int size = pktGetBitsPack( pak, INV_COUNT_PACKBITS);

                character_SetInvTotalSize(pchar, type, totalSize);
            }

            //! @todo pchar can be NULL.  How does this occur?  Should we prevent that from occurring?
            if (pchar)
            {
                pchar->auctionInvTotalSlots = pktGetBitsPack( pak, INV_COUNT_PACKBITS);
                pchar->iNumBoostSlots = pktGetBitsPack( pak, INV_COUNT_PACKBITS);
            } else {
                pktGetBitsPack( pak, INV_COUNT_PACKBITS );
                pktGetBitsPack( pak, INV_COUNT_PACKBITS );
            }
        }
    }
}

//------------------------------------------------------------
//  generic inventory receipt function.
//----------------------------------------------------------
void character_ReceiveInventories(Character *pchar, Packet *pak)
{
    if( verify( pak && pchar ))
    {
        int type;
         int count = pktGetBitsPack( pak, 3 );

        for( type = 0; type < count; ++type ) 
        {
            int totalSize = pktGetBitsPack( pak, INV_COUNT_PACKBITS);
            int size = pktGetBitsPack( pak, INV_COUNT_PACKBITS);
            int i;

            character_ClearInventory(pchar, type, "character_ReceiveInventories");

            character_SetInvTotalSize(pchar, type, totalSize);
            for( i = 0; i < size; ++i ) 
            {
                character_inventory_Receive(pchar, pak);
            }
        }

        pchar->auctionInvTotalSlots = pktGetBitsPack( pak, INV_COUNT_PACKBITS);
    }
}

// end Generic inventory
// ------------------------------------------------------------

void character_SendPowerBoost(Character *pchar, Packet *pak, Boost *pboost, int boostIdx)
{
    if(pboost!=NULL && pboost->ppowBase!=NULL)
    {
        int i;
        int numPowerupSlots;
        
        pktSendBits(pak, 1, 1); // iHasBoost
        basepower_Send(pak, pboost->ppowBase);
        pktSendBitsPack(pak, 5, pboost->iLevel);
        pktSendBitsPack(pak, 2, pboost->iNumCombines);
        
        // send the vars
        for( i = 0; i < ARRAY_SIZE( pboost->afVars ); ++i ) 
        {
            pktSendF32( pak, pboost->afVars[i] );
        }
        
        // --------------------
        // send the powerup slots
        
        // count the powerups
        numPowerupSlots = powerupslot_ValidCount( pboost->aPowerupSlots, ARRAY_SIZE( pboost->aPowerupSlots ) );
        
        // send them
        pktSendBitsPack( pak, POWERUPSLOT_COUNT_PACKBITS, numPowerupSlots );
        for( i = 0; i < numPowerupSlots; ++i ) 
        {
            PowerupSlot *slot = &pboost->aPowerupSlots[i];
            int id = rewarditemtype_IdFromName( slot->reward.type, slot->reward.name );
            pktSendBitsPack( pak, POWERUPSLOT_TYPE_PACKBITS, slot->reward.type );
            pktSendBitsPack( pak, POWERUPSLOT_ID_PACKBITS, id );
            pktSendBitsPack( pak, 1, slot->filled );
        }
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }
}

//------------------------------------------------------------
//  Helper for receiving a boost for a power
//----------------------------------------------------------
void character_ReceivePowerBoost(Character *pchar, Packet *pak, Power *ppow, int isGlobalBoost, int boostIdx)
{
    int iHasBoost = pktGetBits(pak, 1);
    
    if(iHasBoost)
    {
        const BasePower *ppowBase = basepower_Receive(pak, &g_PowerDictionary, (pchar ? pchar->entParent : 0));
        int iLevel = pktGetBitsPack(pak, 5);
        int iNumCombines = pktGetBitsPack(pak, 2);
        F32 afVars[TYPE_ARRAY_SIZE(Boost,afVars)];
        PowerupSlot aPowerupSlots[TYPE_ARRAY_SIZE(Boost,aPowerupSlots)];
        int numPowerupSlots;
        int i;
        
        // get the vars
        for( i = 0; i < ARRAY_SIZE( afVars ); ++i ) 
        {
            afVars[i] = pktGetF32(pak);
        }
        
        // get the powerup slots
        numPowerupSlots = pktGetBitsPack(pak, POWERUPSLOT_COUNT_PACKBITS);
        for( i = 0; i < numPowerupSlots; ++i ) 
        {
            int id;
            aPowerupSlots[i].reward.type = pktGetBitsPack(pak, POWERUPSLOT_TYPE_PACKBITS);
            id = pktGetBitsPack(pak, POWERUPSLOT_ID_PACKBITS);
            aPowerupSlots[i].reward.name = rewarditemtype_NameFromId( aPowerupSlots[i].reward.type, id );
            aPowerupSlots[i].filled = pktGetBitsPack(pak,1);
        }
        
        // if valid, set the boost
        if(ppow!=NULL && ppowBase!=NULL)
        {
            if (isGlobalBoost)
            {
                Boost *globalBoost = boost_Create(ppowBase, boostIdx, ppow, iLevel, iNumCombines, afVars, ARRAY_SIZE( afVars ), aPowerupSlots, numPowerupSlots);
                eaInsert(&ppow->ppGlobalBoosts, globalBoost, boostIdx);
                ppow->bBoostsCalcuated = false;
            }
            else
            {
                // Temporarily set pchar->iLevel to the correct value for this build, thios will make requires
                // statements for purple IO's work properly when build 0 is < 50 and build 1 is == 50 and has purples.
                int iLevelSave = pchar->iLevel;
                pchar->iLevel = pchar->iBuildLevels[pchar->iCurBuild];
                power_ReplaceBoost(ppow, boostIdx, ppowBase, iLevel, iNumCombines, afVars, ARRAY_SIZE( afVars ), aPowerupSlots, numPowerupSlots, NULL);
                pchar->iLevel = iLevelSave;
            }
        }
    }
}


/**********************************************************************func*
* character_SendPowers
*
* There's a fundamental flaw in this code.  We send powers from an array with NULLs in it.
* However, we don't send the NULLs, so the client ends up with an array without them.
* We then use the indices into the array for all sorts of uses, such as tray objects.
*/
void character_SendPowers(Packet *pak, Character *pchar)
{
    int j;
    int iSets;
    int hasMultiBuilds;
    int numBuildsToSend;
    int iBuild;
    int curBuild;
    int iFirstPowerset;

    // For non-player characters
    // 0 bit
    // num of power sets
    //   num of powers
    //     power
    //     level bought at
    //     num of boost slots
    //       0 (no boost in slot)
    //       1
    //         boost
    // For player characters
    // 1 bit
    // buildnum: 0 .. MAX_BUILD_NUM
    //   num of power sets -- skip the first g_numSharedPowersets for builds 1 and higher
    //     num of powers
    //       power
    //       level bought at
    //       num of boost slots
    //         0 (no boost in slot)
    //         1
    //           boost

    hasMultiBuilds = pchar->entParent && ENTTYPE(pchar->entParent) == ENTTYPE_PLAYER ? 1 : 0;
    pktSendBits(pak, 1, hasMultiBuilds);
    curBuild = character_GetActiveBuild(pchar->entParent);
    numBuildsToSend = hasMultiBuilds ? MAX_BUILD_NUM : 1;
    if (hasMultiBuilds)
    {
        pktSendBits(pak, 4, curBuild);
        for (iBuild = 0; iBuild < MAX_BUILD_NUM; iBuild++)
        {
            pktSendBits(pak, 6, pchar->iBuildLevels[iBuild]);
        }
    }
    for (iBuild = 0; iBuild < numBuildsToSend; iBuild++)
    {
        if (hasMultiBuilds)
        {
            pktSendBits(pak, 4, iBuild);
            character_SelectBuildLightweight(pchar->entParent, iBuild);
        }
        iFirstPowerset = iBuild == 0 ? 0 : g_numSharedPowersets;
        iSets = eaSize(&pchar->ppPowerSets);
        pktSendBitsPack(pak, 4, iSets);
        for (j = iFirstPowerset; j < iSets; j++)
        {
            int k;
            int iPows;
            int iRealCntPows=0;
            PowerSet *pset = pchar->ppPowerSets[j];

            iPows = eaSize(&pset->ppPowers);
            for(k=0; k<iPows; k++)
            {
                if(pset->ppPowers[k])
                    iRealCntPows++;
            }

            pktSendBitsPack(pak, 5, pset->iLevelBought);
            pktSendBitsPack(pak, 4, iRealCntPows);
            if(iRealCntPows==0)
            {
                // If this is an empty set, send the root power so the
                //   receiver can at least get the power set.
                basepower_Send(pak, pset->psetBase->ppPowers[0]);
            }
            else
            {
                for(k=0; k<iPows; k++)
                {
                    int i;
                    int iBoosts;
                    Power *ppow = pset->ppPowers[k];

                    if(ppow)
                    {
                        basepower_Send(pak, ppow->ppowOriginal);
                        pktSendBitsPack(pak, 5, ppow->iLevelBought);
                        pktSendF32(pak, ppow->fRangeLast);
                        pktSendBitsPack(pak, 3, ppow->iNumCharges);
                        pktSendF32(pak, ppow->fUsageTime);
                        pktSendF32(pak, ppow->fAvailableTime);
                        pktSendBitsPack(pak, 24, ppow->ulCreationTime);
                        pktSendBitsAuto(pak, ppow->bEnabled);
                        pktSendBitsAuto(pak, ppow->bDisabledManually);
                        pktSendBitsAuto(pak, ppow->iUniqueID);

                        // Now the boosts
                        iBoosts = eaSize(&ppow->ppBoosts);
                        pktSendBitsPack(pak, 4, iBoosts);
                        for(i=0; i<iBoosts; i++)
                        {
                            Boost *pboost = ppow->ppBoosts[i];
                            character_SendPowerBoost( pchar, pak, pboost, i );                        
                        }

                        iBoosts = eaSize(&ppow->ppGlobalBoosts);
                        pktSendBitsAuto(pak, iBoosts);
                        for(i=0; i<iBoosts; i++)
                        {
                            Boost *pboost = ppow->ppGlobalBoosts[i];
                            character_SendPowerBoost( pchar, pak, pboost, i );                        
                        }
                    }
                }
            }
        }
    }
    if (hasMultiBuilds)
    {
        character_SelectBuildLightweight(pchar->entParent, curBuild);
        pktSendBits(pak, 4, curBuild);
    }
}

/**********************************************************************func*
* boost_Receive
*
* NET: fully consumes data even on error
*/
void boost_Receive(Packet *pak, Character *pchar)
{
    // If pchar is NULL, data is still consumed safely.

    // inspiration slot index
    // 0
    //   empty slot
    // 1
    //   basepower

    int idx = pktGetBitsPack(pak, 3);

    if(pktGetBits(pak, 1))
    {
        const BasePower *ppowBase = basepower_Receive(pak, &g_PowerDictionary, (pchar ? pchar->entParent : 0));
        int iLevel = pktGetBitsPack(pak, 5);
        int iNumCombines = pktGetBitsPack(pak, 2);

        F32 afVars[TYPE_ARRAY_SIZE(Boost,afVars)];
        PowerupSlot aPowerupSlots[TYPE_ARRAY_SIZE(Boost,aPowerupSlots)];
        int numPowerupSlots;
        int i;

        // get the vars
        for( i = 0; i < ARRAY_SIZE( afVars ); ++i ) 
        {
            afVars[i] = pktGetF32(pak);
        }
        
        // get the powerup slots
        numPowerupSlots = pktGetBitsPack(pak, POWERUPSLOT_COUNT_PACKBITS);
        for( i = 0; i < numPowerupSlots; ++i ) 
        {
            int id;

            aPowerupSlots[i].reward.type = pktGetBitsPack(pak, POWERUPSLOT_TYPE_PACKBITS);
            id = pktGetBitsPack(pak, POWERUPSLOT_ID_PACKBITS);
            aPowerupSlots[i].reward.name = rewarditemtype_NameFromId( aPowerupSlots[i].reward.type, id );
            aPowerupSlots[i].filled = pktGetBitsPack(pak,1);
        }

        // if valid, set the boost
        if(pchar!=NULL && ppowBase!=NULL)
        {
            character_SetBoost(pchar, idx, ppowBase, iLevel, iNumCombines, 
                               afVars, ARRAY_SIZE(afVars),
                               aPowerupSlots, numPowerupSlots);
        }
        else
        {
            assert(0);
        }
    }
    else if(pchar!=NULL)
    {
        // Empty slot
        character_DestroyBoost(pchar, idx, NULL);
    }
}


/**********************************************************************func*
* character_ReceiveBoosts
*
* NET: fully consumes data even on error
*/
void character_ReceiveBoosts(Packet *pak, Character *pchar)
{
    int j;
    int iCnt;

    // count of inspirations
    //    inspiration

    iCnt = pktGetBitsPack(pak, 5);
    for(j=0; j<iCnt; j++)
    {
        if (pktEnd(pak)) // JE: if networking code is out of sync, this loop can go on for millions of iterations!
            break;
        boost_Receive(pak, pchar);
    }

#if CLIENT && !TEST_CLIENT
    recipeInventoryReparse();
    uiIncarnateReparse();
#endif
}


/**********************************************************************func*
* character_ReceivePowers
*
* NET: fully consumes data even on error
* There's a fundamental flaw in this code.  We receive powers into a clean array within
* their powersets.  However, on the server, the power array has NULLs in it.  We then
* use the indices into the array for all sorts of uses, such as tray objects.
*/
void character_ReceivePowers(Packet *pak, Character *pchar)
{
    // If pchar is NULL, data is still consumed safely.

    int iCntSets;
    int i;
    int j;
    int iset;
    int hasMultiBuilds;
    int iBuild;
    int stopHere;
    int startHere;
    int numBuilds;
    int curBuild;

    pchar->ppowStance = NULL; // just to be safe...

    hasMultiBuilds = pktGetBits(pak, 1);
    numBuilds = hasMultiBuilds ? MAX_BUILD_NUM : 1;
    assert(hasMultiBuilds == (pchar->entParent && ENTTYPE(pchar->entParent) == ENTTYPE_PLAYER ? 1 : 0));
    if (hasMultiBuilds)
    {
        curBuild = pktGetBits(pak, 4);
        stopHere = 0;
        for (iBuild = 0; iBuild < MAX_BUILD_NUM; iBuild++)
        {
            pchar->iBuildLevels[iBuild] = pktGetBits(pak, 6);
            for(iset = eaSize(&pchar->ppBuildPowerSets[iBuild]) - 1; iset >= stopHere; iset--)
            {
                powerset_Destroy(pchar->ppBuildPowerSets[iBuild][iset], NULL);
            }
            eaSetSize(&pchar->ppBuildPowerSets[iBuild], 0);
            stopHere = g_numSharedPowersets;
            if (iBuild == 0)
            {
                pchar->ppPowerSets = pchar->ppBuildPowerSets[0];
                pchar->iLevel = pchar->iBuildLevels[0];
                pchar->iCurBuild = 0;
                pchar->iActiveBuild = 0;

                character_GrantAutoIssueBuildSharedPowerSets(pchar);
            }
        }
    }
    else
    {
        for(iset = eaSize(&pchar->ppPowerSets)-1; iset>=0; iset--)
        {
            powerset_Destroy(pchar->ppPowerSets[iset], NULL);
        }
        eaSetSize(&pchar->ppPowerSets, 0);
    }
    // num of power sets
    //   num of powers
    //     power
    //     level bought at
    //     num of boost slots
    //       0 (no boost in slot)
    //       1
    //         boost
    for (iBuild = 0; iBuild < numBuilds; iBuild++)
    {
        if (hasMultiBuilds)
        {
            i = pktGetBits(pak, 4);
            assert(i == iBuild);
            character_SelectBuildLightweight(pchar->entParent, iBuild);
            if (iBuild >= 1)
            {
                for (j = 0; j < g_numSharedPowersets; j++)
                {
                    PowerSet *pset = pchar->ppBuildPowerSets[0][j];
                    int result = eaPush(&pchar->ppPowerSets, pset);
                    assert(result == j);
                    assert(pset->idx == j);
                }
            }
        }
        iCntSets = pktGetBitsPack(pak, 4);
        startHere = iBuild == 0 ? 0 : g_numSharedPowersets;
        for(i = startHere; i < iCntSets; i++)
        {
            int iSetLevelBought = pktGetBitsPack(pak, 5);
            int iCntPows = pktGetBitsPack(pak, 4);

            if(iCntPows==0)
            {
                // If this is an empty set, this power was sent just to get
                //   the power set.
                const BasePower *ppowBase = basepower_Receive(pak, &g_PowerDictionary, (pchar ? pchar->entParent : 0));
                if(pchar!=NULL && ppowBase!=NULL)
                {
                    PowerSet *pset;
                    // Make the power set
                    if((pset=character_BuyPowerSet(pchar, ppowBase->psetParent))!=NULL)
                    {
                        pset->iLevelBought = iSetLevelBought;
                    }
                }
            }
            else
            {
                for(j=0; j<iCntPows; j++)
                {
                    int k;
                    Power *ppow = NULL;

                    const BasePower *ppowBase = basepower_Receive(pak, &g_PowerDictionary, (pchar ? pchar->entParent : 0));
                    int iLevelBought = pktGetBitsPack(pak, 5);
                    float fRange = pktGetF32(pak);
                    int iNumCharges = pktGetBitsPack(pak,3);
                    float fUsageTime = pktGetF32(pak);
                    float fAvailableTime = pktGetF32(pak);
                    unsigned long ulCreationTime = pktGetBitsPack(pak,24);
                    int bEnabled = pktGetBitsAuto(pak);
                    int bDisabledManually = pktGetBitsAuto(pak);
                    int uniqueID = pktGetBitsAuto(pak);
                    int iCntBoosts = pktGetBitsPack(pak, 4);
                    int iCntGlobalBoosts;

                    if(pchar!=NULL && ppowBase!=NULL)
                    {
                        PowerSet *pset;
                        // Make the power set
                        if((pset=character_BuyPowerSet(pchar, ppowBase->psetParent))!=NULL)
                        {
                            pset->iLevelBought = iSetLevelBought;

                            // If they don't have the power yet or it's a temp power
                            //   buy the power.
                            if((ppow=character_OwnsPower(pchar, ppowBase))==NULL
                                || stricmp(pset->psetBase->pcatParent->pchName,"Temporary_Powers") == 0)
                            {
                                ppow = powerset_AddNewPower(pset, ppowBase, uniqueID, NULL);
                            }

                            if(ppow)
                            {
                                int iCntAllowed = CountForLevel(pchar->iLevel-iLevelBought, g_Schedules.aSchedules.piFreeBoostSlotsOnPower);
                                iCntAllowed = min(iCntAllowed, ppow->ppowBase->iMaxBoosts);
                                ppow->iNumBoostsBought = iCntBoosts-iCntAllowed;
                                if(ppow->iNumBoostsBought<0) ppow->iNumBoostsBought = 0; // Just for safety

                                ppow->iLevelBought = iLevelBought;
                                ppow->fRangeLast = fRange;
                                ppow->iNumCharges = iNumCharges;
                                ppow->fUsageTime = fUsageTime;
                                ppow->fAvailableTime = fAvailableTime;
                                ppow->ulCreationTime = ulCreationTime;
                                ppow->bEnabled = bEnabled;
                                ppow->bDisabledManually = bDisabledManually;
                                power_LevelFinalize(ppow);
                            }
                        }
                    }

                    // Get all the boosts
                    for(k=0; k<iCntBoosts; k++)
                    {
                        if(ppow!=NULL)
                        {
                            power_DestroyBoost(ppow, k, NULL);
                        }

                        character_ReceivePowerBoost( pchar, pak, ppow, 0, k );
                        
                        if (pktEnd(pak)) // JE: if networking code is out of sync, this loop can go on for millions of iterations!
                            break;
                    } // end for each boost

                    iCntGlobalBoosts = pktGetBitsAuto(pak);

                    if (ppow)
                    {
                        for (k = eaSize(&ppow->ppGlobalBoosts) - 1; k >= 0; k--)
                        {
                            eaRemoveAndDestroy(&ppow->ppGlobalBoosts, k, boost_Destroy);
                        }
                    }

                    // Get all the global boosts
                    for(k=0; k<iCntGlobalBoosts; k++)
                    {
                        character_ReceivePowerBoost( pchar, pak, ppow, 1, k );

                        if (pktEnd(pak)) // JE: if networking code is out of sync, this loop can go on for millions of iterations!
                            break;
                    } // end for each boost

                    if (pktEnd(pak)) // JE: if networking code is out of sync, this loop can go on for millions of iterations!
                        break;
                } // end for each power
            }
            if (pktEnd(pak)) // JE: if networking code is out of sync, this loop can go on for millions of iterations!
                break;
        } // end for each power set
        if (pktEnd(pak)) // JE: if networking code is out of sync, this loop can go on for millions of iterations!
            break;
    }
    if (hasMultiBuilds && !pktEnd(pak))
    {
        int curBuild = pktGetBits(pak, 4);
        character_SelectBuildLightweight(pchar->entParent, curBuild);
        pchar->iActiveBuild = curBuild;
    }
#if CLIENT && !TEST_CLIENT
    serverTray_Rebuild();
    recipeInventoryReparse();
    uiIncarnateReparse();
#endif

}

void powerRef_Send(PowerRef* ppowRef, Packet* pak)
{
    pktSendBitsAuto(pak, ppowRef->buildNum);
    pktSendBitsPack(pak, 2, ppowRef->powerSet);
    pktSendBitsPack(pak, 1, ppowRef->power);
}

void powerRef_Receive(PowerRef* ppowRef, Packet* pak)
{
    ppowRef->buildNum = pktGetBitsAuto(pak);
    ppowRef->powerSet = pktGetBitsPack(pak, 2);
    ppowRef->power = pktGetBitsPack(pak, 1);
}