/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/


/***************************************************************************

    THIS CODE HAS BE OBSOLETED! 

    It was an implementation of an old version of the invention system and has
    since been replaces with a new, simpler version.  This code will NOT work
    due to the repurposing of the recipe inventory system - Vince 4/19/06

 ***************************************************************************/

#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/network/net_packet.h>
#include "entity/entity.h"
#include "entity/character_base.h"
#include "entity/character_inventory.h"
#include "entity/character_net.h"
#include "entity/Concept.h"
#include "entity/Invention.h"
#include "bases/DetailRecipe.h"
#include "entity/boost.h"
#include "entity/powers.h"

#if SERVER 
#include "dbcomm/logcomm.h"
#include "Reward.h"
#endif

#if CLIENT // SERVER
#include "gameComm/wdwbase.h"
#include "UI/uiNet.h"
#include "UI/uiWindows.h"
#endif // CLIENT


char *inventionstate_Str(InventionState s)
{
    static char *ss[] = 
        {
            "None",
            "Starting",
            "Invent",
            "Updating",
            "Finalizing",
            "Finished",
            "kInventionState_Count",
        };
    return inventionstate_Valid( s ) ? ss[s] : "<invalid inventionstate>";
}

char *slotstate_Str(SlotState state)
{
    static char *ss[] = 
        {
            "Empty",
            "Slotted",
            "HardenSuccess",
            "HardenFailure",
            "Count",
        }
    ;
    return slotstate_Valid(state) ? ss[state] : "<invalid slotstate>";
}


static bool s_validateInventionState(Character const *p, Invention const *inv)
{
    return verify( p && inv
                   && (p->invention->state == kInventionState_Updating 
                   || p->invention->state == kInventionState_Started 
                   || INRANGE0( inv->boostInvIdx, CHAR_BOOST_MAX ) && p->aBoosts[inv->boostInvIdx]));
}


static bool s_Inventing(Character const *p)
{
    return p && p->invention && p->invention->state == kInventionState_Invent && s_validateInventionState(p, p->invention);
}

//------------------------------------------------------------
//  Helper for knowing when a player is in the process of 
// invention
//----------------------------------------------------------
bool character_IsInventing(Character const *p)
{
    return  verify(p) && p->invention && 
        (p->invention->state == kInventionState_Started
         || p->invention->state == kInventionState_Invent
         || p->invention->state == kInventionState_Updating
         || p->invention->state == kInventionState_Finalizing)
        && s_validateInventionState(p, p->invention);
}

bool invention_SlotHardened(InventionSlot *s)
{
    return s && s->state == kSlotState_HardenSuccess || s->state == kSlotState_HardenFailure;
}

void inventionslot_Clear(InventionSlot *s)
{
    if(s)
    {
        s->state = kSlotState_Empty;
        eaSetSize(&s->concepts,0);
    }
}

MP_DEFINE(Invention);
Invention* invention_Create()
{
    Invention *res = NULL;

    // create the pool, should only be one
    MP_CREATE(Invention, 1);
    res = MP_ALLOC( Invention );

    if( res )
    {
        res->boostInvIdx = -1;
    }


    // --------------------
    // finally, return

    return res;
}

void invention_Destroy( Invention *inv )
{
    int islot;
    for( islot = 0; islot < ARRAY_SIZE( inv->slots ) ; ++islot )
    {
        eaDestroy( &inv->slots[islot].concepts );
    }
    
    MP_FREE(Invention, inv);
}

static Invention* invention_Init(Character *p, InventionState state, RecipeItem *recipe, int boostInvIdx )
{
    Invention *res = NULL;
    if( verify(p) )
    {
        if( !p->invention )
        {
            p->invention = invention_Create();
        }

        if( verify( p->invention && inventionstate_Valid(state) ))
        {
            int islot;
            for( islot = 0; islot < ARRAY_SIZE( p->invention->slots ) ; ++islot )
            {
                inventionslot_Clear( &p->invention->slots[islot] );
            }

            p->invention->state = state;
            p->invention->recipe = recipe;
            p->invention->boostInvIdx = boostInvIdx;
        }
        res = p->invention;
    }
    return res;
}

static void invention_Clear(Character *p)
{
    if( verify( p && p->invention ))
    {
        invention_Init(p, kInventionState_None, NULL, -1 );
    }
}





// ------------------------------------------------------------
// invention net code

#define INVENTION_STATE_PACKBITS 1
#define INVENTION_RECIPEID_PACKBITS 1
#define INVENTION_BOOSTINVIDX_PACKBITS 1
#define INVENTION_NUMCONCEPTS_PACKBITS 1
#define INVENTION_CONCEPT_ID_PACKBITS 1
#define INVENTION_UPDATE_PACKBITS 1
#define INVENTION_RECIPEIDX_PACKBITS 1
#define INVENTION_STATUSCHANGE_PACKBITS 1
#define INVENTION_FILLED_PACKBITS 1

//------------------------------------------------------------
// Send an invention
//----------------------------------------------------------
void invention_Send(Character const *pchar, Packet *pak)
{
    int i;
    Invention const *invention = pchar->invention;
    F32 afVars[TYPE_ARRAY_SIZE(Boost,afVars)] = {0};
    PowerupSlot aPowerupSlots[TYPE_ARRAY_SIZE( Boost, aPowerupSlots )] = {0}; 
    InventionState stateInvention = kInventionState_None;
    int idRecipe = 0;
    int iBoostInv = -1;
    
    // --------------------
    // gather the minimum to send
    
    if( verify( invention ))
    {
        stateInvention = invention->state;
    }
    
    // ----------------------------------------
    // send the info
    
    pktSendBitsPack( pak, INVENTION_STATE_PACKBITS, stateInvention );
    
    // only continue if this is a valid invention state
    if( stateInvention != kInventionState_None )
    {
        // ----------------------------------------
        // send the info
        
        // if we're not inventing, no recipe
        idRecipe = character_IsInventing(pchar) ? invention->recipe->id : 0;
        pktSendBitsPack( pak, INVENTION_RECIPEID_PACKBITS, idRecipe );
        
        // --------------------
        // send boost info        
        
        iBoostInv = invention->boostInvIdx;
        
        if( verify(INRANGE0( invention->boostInvIdx, CHAR_BOOST_MAX )))
        {
            Boost *boost = pchar->aBoosts[invention->boostInvIdx];
            CopyStructs( afVars, boost->afVars, ARRAY_SIZE( afVars ));
            CopyStructs( aPowerupSlots, boost->aPowerupSlots, ARRAY_SIZE( aPowerupSlots ));
        }

        pktSendBitsPack( pak, INVENTION_BOOSTINVIDX_PACKBITS, iBoostInv );
        
        // vars
        for( i = 0; i < ARRAY_SIZE( afVars ); ++i ) 
        {
            pktSendF32(pak, afVars[i]);
        }
        
        // --------------------
        // powerup slots
        
        for( i = 0; i < ARRAY_SIZE( aPowerupSlots ); ++i ) 
        {
            PowerupSlot *slot = aPowerupSlots + i;
            
            rewardslot_Send(&slot->reward, pak);
            pktSendBitsPack( pak, INVENTION_FILLED_PACKBITS, slot->filled); 
        }
        
        // --------------------
        // send every slot's status
        // note: skipped accumulation here. just sends 0 if invention is NULL
        
        for( i = 0; i < ARRAY_SIZE( invention->slots ); ++i ) 
        {
            InventionSlot const *slot = invention ? &invention->slots[i] : NULL;
            int numConcepts = slot ? eaSize( &slot->concepts ) : 0;
            int cptidx;
            
            pktSendBitsPack( pak, INVENTION_STATE_PACKBITS, slot ? slot->state : kSlotState_Count);  
            pktSendBitsPack( pak, INVENTION_NUMCONCEPTS_PACKBITS, numConcepts);
            for( cptidx = 0; cptidx < numConcepts ; ++cptidx )
            {
                conceptitem_Send(slot->concepts[cptidx], pak);
            }
        }
    }
}

//------------------------------------------------------------
//  Receive an invention
// param resInvention: where the invention is stored to
// NOTE: it is up to the caller to check that the resulting fields
// are valid.
//----------------------------------------------------------
static void invention_Receive(Character *pchar, Packet *pak)
{
    /* 
    // set the invention
    pchar->invention = pchar->invention ? pchar->invention : invention_Create();
    invention_Clear(pchar);
    
    // always do this
    {
        int i;

        pchar->invention->state = pktGetBitsPack( pak, INVENTION_STATE_PACKBITS);

        // if the server says we're not inventing, bail out
        if( pchar->invention->state == kInventionState_None )
        {
            invention_Destroy( pchar->invention );
            pchar->invention = NULL;
        }
        else
        {
            int recipeId;
            F32 afVars[TYPE_ARRAY_SIZE(Boost,afVars)] = {0};
            PowerupSlot aPowerupSlots[TYPE_ARRAY_SIZE( Boost, aPowerupSlots )] = {0};

            // --------------------
            // recipe

            recipeId = pktGetBitsPack( pak, INVENTION_RECIPEID_PACKBITS);
            pchar->invention->recipe = recipe_GetItemById( recipeId );

            // --------------------
            // boost            

            // boost index
            pchar->invention->boostInvIdx = pktGetBitsPack( pak, INVENTION_BOOSTINVIDX_PACKBITS);

            // vars
            for( i = 0; i < ARRAY_SIZE( afVars ); ++i ) 
            {
                afVars[i] = pktGetF32(pak);
            }

            // powerup slots
            for( i = 0; i < ARRAY_SIZE( aPowerupSlots ); ++i ) 
            {
                PowerupSlot *slot = aPowerupSlots + i;
                rewardslot_Receive(&slot->reward, pak);
                slot->filled = pktGetBitsPack( pak, INVENTION_FILLED_PACKBITS); 
            }

            // copy the data back, if its valid
            if( verify( INRANGE0( pchar->invention->boostInvIdx, CHAR_BOOST_MAX )))
            {
                Boost *boost = pchar->aBoosts[pchar->invention->boostInvIdx];
                CopyStructs( boost->afVars, afVars, ARRAY_SIZE(afVars));
                CopyStructs( boost->aPowerupSlots, aPowerupSlots, ARRAY_SIZE(aPowerupSlots) );
            }

            // --------------------
            // get every slot's status

            for( i = 0; i < ARRAY_SIZE( pchar->invention->slots ); ++i ) 
            {
                InventionSlot *slot = &pchar->invention->slots[i];
                int numConcepts;
                int cptidx;

                slot->state = pktGetBitsPack( pak, INVENTION_STATE_PACKBITS); 

                // --------------------
                // get the concepts

                numConcepts = pktGetBitsPack( pak, INVENTION_NUMCONCEPTS_PACKBITS);

                // init concepts, if necessary
                if(!slot->concepts)
                {
                    eaCreate(&slot->concepts);
                }
                else
                {
                    // cleanup
                    int i;
                    for( i = eaSize(&slot->concepts) - 1; i >= numConcepts; --i )
                    {
                        conceptitem_Destroy(slot->concepts[i]);
                    }
                }
                eaSetSize(&slot->concepts, numConcepts);

                for( cptidx = 0; cptidx < numConcepts ; ++cptidx )
                {
                    // creates the item if NULL
                    slot->concepts[cptidx] = conceptitem_Receive( slot->concepts[cptidx], pak );
                }
            }
        }
    }
    */
}

#if SERVER
void entity_SendInventionUpdate( Entity *e, Packet *pak )
{
    bool statusChange = false;
    Invention *invention = NULL;
    
    if( e && e->pchar && e->pchar->invention)
    {
        invention = e->pchar->invention;
        statusChange = e->pchar->invention->statusChange;
    }

    pktSendBitsPack( pak, INVENTION_STATUSCHANGE_PACKBITS, statusChange); 

    if( statusChange )
    {
        invention_Send( e->pchar, pak );
        e->pchar->invention->statusChange = false;
    }
}

#elif CLIENT // SERVER
void entity_ReceiveInventionUpdate( Entity *e, Packet *p )
{
    bool statusChange = pktGetBitsPack( p, INVENTION_STATUSCHANGE_PACKBITS); 

    if( statusChange && verify(e->pchar) )
    {
        InventionState eStatePrev = kInventionState_None;
        
        if( verify(e->pchar) && e->pchar->invention )
        {
            eStatePrev = e->pchar->invention->state;
        }

        invention_Receive( e->pchar, p );

        if( e->pchar && character_IsInventing(e->pchar) )
        {
            // if we were just starting invention, show the window
            if( eStatePrev == kInventionState_Started )
            {
                window_setMode( WDW_INVENT, WINDOW_GROWING );
            }
        }
    }
}
#endif // CLIENT

#if CLIENT
//------------------------------------------------------------
//  Helper for when the invention is being updated on the
// server side and can't be modified locally.
//----------------------------------------------------------
bool character_InventingCanUpdate(Character *pchar)
{
    return character_IsInventing(pchar) 
        && pchar->invention->state != kInventionState_Updating 
        && pchar->invention->state != kInventionState_Finalizing;
}
#endif // CLIENT

#if CLIENT
//------------------------------------------------------------
//  Start the inventing process. this is really just a way 
// for the client to track state while the server is updating.
//----------------------------------------------------------
bool character_InventingStart(Character *p, U32 recipeinv_idx)
{
    bool res = false;
    /* Commented out - Recipes have been taken over by new invention system.

    int i;

    for(i = 0; i < CHAR_BOOST_MAX; i++)
    {
        if(p->aBoosts[i]==NULL)
        {
            break;
        }
    }

    if( !character_BoostInventoryFull(p) && i < CHAR_BOOST_MAX && // NULL slot found
        EAINRANGE( (int)recipeinv_idx, p->recipeInv ) &&
        verify( p && p->recipeInv[recipeinv_idx]->recipe &&
                   p->recipeInv[recipeinv_idx]->recipe->recipe &&
                   !s_Inventing(p)))
    {
        invention_Init( p, kInventionState_Started, p->recipeInv[recipeinv_idx]->recipe, -1 );
        res = p->invention != NULL;
    }
    */
    // --------------------
    // finally, return
    
    return res;
}
#endif

#if CLIENT
bool character_InventingSelectLevel(Character *pchar, U32 inventionlevel)
{
    bool res = false;
    /* Commented out - Recipes have been taken over by new invention system.

    int i;
    int invIdx = -1;
    
    if( character_InventingCanUpdate( pchar ))
    {
        // find the recipe being invented
        for( i = eaSize( &pchar->recipeInv ) - 1; i >= 0; --i)
        {
            if( pchar->recipeInv[i]->recipe == pchar->invention->recipe )
            {
                invIdx = i;
                break; // BREAK IN FLOW
            }
        }
        
        if( verify(INRANGE0( inventionlevel, MAX_PLAYER_SECURITY_LEVEL )
                   && EAINRANGE( invIdx, pchar->recipeInv)))
        {
            invent_SendSelectrecipe( invIdx, inventionlevel );
            pchar->invention->state = kInventionState_Updating;
            res = true;
        }
    }
    */

    // --------------------
    // finally

    return res;
}
#endif // CLIENT

#if CLIENT
bool character_InventingSlotConcept(Character *pchar, int iSlot, int iConceptInv )
{
    bool res = false;
    if( character_InventingCanUpdate(pchar) 
        && verify(AINRANGE( iSlot, pchar->invention->slots ) 
                  && EAINRANGE( iConceptInv, pchar->conceptInv )))
    {
        invent_SendSlotconcept( iSlot, iConceptInv );
        pchar->invention->state = kInventionState_Updating;
        res = true;
    }

    // --------------------
    // finally

    return res;
}
#endif // CLIENT

#if CLIENT
bool character_InventingHardenConcept(Character *pchar, int iSlot)
{
    bool res = false;
    if( character_InventingCanUpdate(pchar) )
    {
        invent_SendHardenslot( iSlot );
        pchar->invention->state = kInventionState_Updating;
        res = true;
    }

    // --------------------
    // finally

    return res;
}
#endif // CLIENT

#if CLIENT
bool character_InventingUnSlotConcept(Character *pchar, int iSlot)
{
    bool res = false;

    if( character_InventingCanUpdate(pchar) )
    {
        invent_SendUnSlot( iSlot );
        pchar->invention->state = kInventionState_Updating;
        res = true;
    }

    // --------------------
    // finally

    return res;
}
#endif // CLIENT
#if CLIENT
bool character_InventingCancel(Character *p)
{
    bool res = false;
    
    if( character_InventingCanUpdate(p) )
    {
        if( p->invention->state == kInventionState_Started )
        {
            // if starting, just delete.
            invention_Destroy(p->invention);
            p->invention = NULL;
        }
        else
        {
            // otherwise cancel
            invent_SendCancel();
            p->invention->state = kInventionState_Finalizing;
        }
        res = true;
    }
    // --------------------
    // finally

    return res;
}
#endif // CLIENT

#if SERVER

//------------------------------------------------------------
//  slot the passed concept in a hardened slot
// currently the only option is to clear this
// -AB: note that you cannot unslot a successful slot at the
// moment. to do this we need to back out of the boost effects
// (i.e. un-add to the vars) :2005 Apr 20 02:38 PM
//----------------------------------------------------------
static bool s_slotInHardened(Character *p, InventionSlot *slot, ConceptItem const *concept )
{
    bool res = false;
    
    if( verify( s_Inventing(p) && slot && slot->state == kSlotState_HardenFailure && concept ))
    {
        bool clearSlot = false;

        if( concept->def->hardenedAffectingRewards )
        {
            int j;
                
            // --------------------
            // see if any hardened-affecting rewards will help this
            
            for( j = eaSize(&concept->def->hardenedAffectingRewards) - 1; !clearSlot && j >= 0; --j)
            {
                const HardenedConceptAffectingRewardDef *def = concept->def->hardenedAffectingRewards[j];
                if(def->type == kHardenedAffectingRewardType_EmptySlot )
                {
                    clearSlot = true;
                }
            }
        }
        
        // --------------------
        // if the slot should be cleared
        
        if( clearSlot )
        {
            inventionslot_Clear(slot);
            p->invention->statusChange = true;
        }
    }
    // --------------------
    // finally

    return res;
}
#endif // SERVER

#if SERVER
//------------------------------------------------------------
//  Put a concept in a the hardening recipe in preparation for hardening
// returns true if the concept can be slotted, false otherwise
// @todo -AB: a big todo here, actually wire up rewards. :2005 Apr 07 04:33 PM
//----------------------------------------------------------
static bool character_SlotConcept(Character *p, U32 slotIdx, ConceptItem *concept)
{
    bool res = false;

    if( verify( s_Inventing(p)
                && AINRANGE( slotIdx, p->invention->slots )
                && concept ))
    {
        InventionSlot *slot = p->invention->slots + slotIdx;
        
        if( slot->state == kSlotState_Empty 
            && basepower_CanApplyConcept( p->invention->recipe->recipe, concept->def))
        {
            // if its empty, just add it
            eaPush( &slot->concepts, concept );
            slot->state = kSlotState_Slotted;
            res = true;
        }
        else if( slot->state == kSlotState_Slotted )
        {
            // if it is slotted, then see if this can affect slotted
            if( concept->def->slottedAffectingRewards )
            {
                // add it for now. later we may need to process it
                eaPush( &slot->concepts, concept );
                res = true;
            }
        }
        else if( slot->state == kSlotState_HardenFailure )
        {
            // if this is already hardened, affected it
            if( concept->def->hardenedAffectingRewards )
            {
                // in this case, just clear the concepts (all of them)
                res = s_slotInHardened(p, &p->invention->slots[slotIdx], concept );
            }
        }

        // update status
        p->invention->statusChange = true;
    }

    // --------------------
    // finally, return

    return res; 
}
#endif

#if SERVER
//------------------------------------------------------------
//  Helper for dispatching a hardening reward.
//----------------------------------------------------------
static s_HandleConceptHardeningReward(Character *p, int slotIdx, const ConceptHardeningRewardDef *rew)
{
    if( verify( p && p->invention && AINRANGE( slotIdx, p->invention->slots )))
    {
        Invention *inv = p->invention;
        InventionSlot *slot = &inv->slots[slotIdx];
        
        switch( rew->type )
        {
        case kConceptHardeningRewardType_Reward:
            rewardFindDefAndApplyToEnt( p->entParent, (const char**)eaFromPointerUnsafe((char*)rew->param0), VG_NONE, atoi(rew->param1), true, REWARDSOURCE_UNDEFINED, NULL);
            break;
        case kConceptHardeningRewardType_ChangeHardenedAttribs:
            break;
        case kConceptHardeningRewardType_ReducePowerupCost:
            // @todo -AB: not implemented yet :2005 Apr 14 09:48 AM
            break;
        xdefault:
            assertmsg(0,"invalid.");
        };
    }

}

#endif // SERVER

F32 invention_HardenChanceAdj(int charLevel, int hardenLevel)
{
    return (F32)(charLevel - hardenLevel);
}

#if SERVER
static bool character_SlotBoostForHardening(Character *p, int inventionlevel)
{
    bool res = false;
    
    if( verify( p && p->invention && p->invention->recipe ) )
    {
        // try and get a valid boost
        p->invention->boostInvIdx = character_AddBoost( p, p->invention->recipe->recipe, inventionlevel, 0, "Invention_c" );
        res = p->invention->boostInvIdx >= 0;

        // update status
        p->invention->statusChange = true;
    }
    return res;
}

#endif


#if SERVER
void character_InventReceiveRecipeSelection(Character *p, Packet *pak)
{
    int recipeInvIdx = -1;
    int inventionLevel = -1;

    // always get the net data
    if( verify( pak ))
    {
        recipeInvIdx = pktGetBitsPack( pak, INVENTION_RECIPEINVIDX_PACKBITS );
        inventionLevel = pktGetBitsPack( pak, INVENTION_INVENTIONLEVEL_PACKBITS); 
    }

    if( !EAINRANGE( recipeInvIdx, p->recipeInv ) 
        || !p->recipeInv[recipeInvIdx] )
    {
        dbLog("cheater",NULL,"Player with authname %s tried to send invalid recipe index %d\n", p->entParent->auth_name, recipeInvIdx);
    }
    else if( verify( p != NULL ))
    {
        /* Commented out - Recipes have been taken over by new invention system.

        Power *recipe = NULL;
        int boostidx = -1;

        // create the invention
        if( !p->invention )
        {
            p->invention = invention_Create();
        }
        invention_Clear(p);

        // get the recipe
        if( EAINRANGE( recipeInvIdx, p->recipeInv ) && p->recipeInv[recipeInvIdx] )
        {
            p->invention->recipe = p->recipeInv[recipeInvIdx]->recipe;
        }

        // slot the boost
        if( character_SlotBoostForHardening( p, inventionLevel ) )
        {
            p->invention->state = kInventionState_Invent;
        }
        else
        {
            p->invention->state = kInventionState_None;
        }

        */

        // either way, update
        p->invention->statusChange = true;
    }
}

#endif // SERVER

#if SERVER
void character_InventReceiveSlotConcept(Character *p, Packet *pak)
{
    int slotIdx = -1;
    int conceptInvIdx = -1;

    if( verify( pak ))
    {
        slotIdx = pktGetBitsPack( pak, INVENTION_SLOTIDX_PACKBITS); 
        conceptInvIdx = pktGetBitsPack( pak, INVENTION_CONCEPTINVIDX_PACKBITS);
    }

    if( !p->invention )
    {
        dbLog("cheater",NULL,"Player with authname %s tried to slot concept when not inventing\n", p->entParent->auth_name);
    }
    else if( !AINRANGE( slotIdx, p->invention->slots ) || !EAINRANGE( conceptInvIdx, p->conceptInv ) || !p->conceptInv[conceptInvIdx]->concept )
    {
        dbLog("cheater",NULL,"Player with authname %s tried to slot an invalid concept idx or slot %d %d\n", p->entParent->auth_name, slotIdx, conceptInvIdx);
    }
    else if( !s_Inventing(p) )
    {
        dbLog("cheater",NULL,"Player with authname %s tried to slot concept when not inventing \n", p->entParent->auth_name);
    }
    else
    {
        // looks good
        if( verify(character_SlotConcept(p, slotIdx, p->conceptInv[conceptInvIdx]->concept) ))
        {
        }
        else
        {
            dbLog("cheater",NULL,"Player with authname %s tried to slot a concept idx or slot %d %d that failed for unknown reason\n", p->entParent->auth_name, slotIdx, conceptInvIdx);
        }

        // @todo -AB: move stuff from inventory :2005 Apr 11 02:11 PM
    }
}
#endif // SERVER

#if SERVER
void character_InventReceiveUnSlot(Character *p, Packet *pak)
{
    int slotIdx = -1;

    if( verify( pak ))
    {
        slotIdx = pktGetBitsPack( pak, INVENTION_SLOTIDX_PACKBITS); 
    }
    
    if( !verify( character_IsInventing(p)) )
    {
        dbLog("cheater",NULL,"Player with authname %s tried to unslot when not inventing\n", p->entParent->auth_name);
    }
    else if( !verify(AINRANGE( slotIdx, p->invention->slots)))
    {
        dbLog("cheater",NULL,"Player with authname %s tried to unslot an invalid idx %d\n", p->entParent->auth_name, slotIdx);
    }
    else
    {
        InventionSlot *slot = p->invention->slots + slotIdx;
        if( !verify( slot->state == kSlotState_Slotted ))
        {
dbLog("cheater",NULL,"Player with authname %s tried to unslot an unslottable slot\n", p->entParent->auth_name);
        }
        else
        { 
            // @todo -AB: update inventory here. give back concept :2005 May 17 12:14 PM
            inventionslot_Clear(slot);
            p->invention->statusChange = true;
        }
    }
}
#endif // SERVER
#if SERVER
void character_InventReceiveHardenSlot(Character *p, Packet *pak)
{
    int slotIdx = -1;
    if( verify( pak ))
    {
        slotIdx = pktGetBitsPack( pak, INVENTION_SLOTIDX_PACKBITS); 
    }

    if( !s_Inventing(p) )
    {
        dbLog("cheater",NULL,"Player with authname %s tried to harden slot when not inventing\n",p->entParent->auth_name);
    }
    else
    {
        dbLog("cheater",NULL,"Player with authname %s tried to harden slot, but we don't support that.\n",p->entParent->auth_name);
    }
}
#endif // SERVER

#if SERVER
//------------------------------------------------------------
//  helper for when a potential enhancement is created (and inventing is finished)
//----------------------------------------------------------
static void s_PECreationReward(Character *p, const PECreationRewardDef *def)
{
    if( verify( def && p ))
    {
        switch ( def->type )
        {
        case kPECreationReward_ChangeHardenLevel:
        {
            int levelAdj = atoi(def->param);
            Boost *boost = p->aBoosts[p->invention->boostInvIdx];
            character_SetBoost(p, 
                               p->invention->boostInvIdx, 
                               boost->ppowBase, 
                               MAX(1,boost->iLevel - levelAdj),
                               boost->iNumCombines,
                               boost->afVars, ARRAY_SIZE( boost->afVars ),
                               boost->aPowerupSlots, ARRAY_SIZE(boost->aPowerupSlots));
        }
        break;
        case kPECreationReward_ChangeSellback:
        {
            // @todo -AB: need the db for this :2005 Apr 20 12:18 PM
        }
        break;
        case kPECreationReward_SpawnAmbush:
        {
            // @todo -AB: on hold :2005 Apr 20 12:18 PM
        }
        break;
        case kPECreationReward_WisdomMultiplier:
        {
            // @todo -AB: on hold :2005 Apr 20 12:18 PM
        }
        break;
        case kPECreationReward_PrefillPowerupCost:
        {
            Boost *boost = p->aBoosts[p->invention->boostInvIdx];
            int i;
            int numToFill = atoi(def->param);

            // fill the slots
            for( i = 0; i < ARRAY_SIZE( boost->aPowerupSlots ) && powerupslot_Valid( &boost->aPowerupSlots[i] ) && numToFill-- >= 0; ++i )
            {
                if( !boost->aPowerupSlots[i].filled )
                {
                    boost->aPowerupSlots[i].filled = true;                    
                }
            }

            // update the boost
            character_SetBoost(p, 
                               p->invention->boostInvIdx, 
                               boost->ppowBase, 
                               boost->iLevel,
                               boost->iNumCombines,
                               boost->afVars, ARRAY_SIZE( boost->afVars ),
                               boost->aPowerupSlots, ARRAY_SIZE(boost->aPowerupSlots));
        }
        break;
        default:
            assertmsg(0,"invalid.");
        };
    }
}
#endif // SERVER

#if SERVER
void character_InventReceiveFinalize(Character * p, Packet *pak)
{
    if( !s_Inventing(p))
    {
        dbLog("cheater",NULL,"Player with authname %s tried to finalize when not inventing\n", p->entParent->auth_name);
    }
    else
    {
        Invention *inv = p->invention;
        int slot;

        // --------------------
        // for all the slots, apply any pe creation rewards

        for( slot = 0; slot < ARRAY_SIZE( inv->slots ); ++slot )
        {
            InventionSlot *pslot = inv->slots + slot;

            if( pslot->state == kSlotState_HardenSuccess )
            {
                int i;
                for( i = 0; i < eaSize( &pslot->concepts ); ++i ) 
                {
                    int j;
                    const ConceptDef *def = pslot->concepts[i]->def;

                    for( j = 0; j < eaSize( &def->peCreationRewards ); ++j ) 
                    {
                        s_PECreationReward( p, def->peCreationRewards[j] );
                    }
                }
            }
        }

        p->invention->state = kInventionState_Finished;
        p->invention->statusChange = true;
    }
}
#endif // SERVER


#if SERVER
//------------------------------------------------------------
//  receive cancellation of invention.
//----------------------------------------------------------
void character_InventReceiveCancel(Character *p, Packet *pak)
{
    if( !s_Inventing(p))
    {
        dbLog("cheater",NULL,"Player with authname %s tried to finalize when not inventing\n", p->entParent->auth_name);
    }
    else
    {
        Invention *inv = p->invention;
        int slot;
        
        // --------------------
        // for all the slots, put back any un-hardened concpts
        
        for( slot = 0; slot < ARRAY_SIZE( inv->slots ); ++slot )
        {
            InventionSlot *pslot = inv->slots + slot;
            
            if( pslot->state == kSlotState_Slotted )
            {
                // @todo -AB: put back in inventory :2005 May 18 02:42 PM
            }
        }

        // actually remove the item from inventory
        if(verify(inv->boostInvIdx>=0))
        {
            character_RemoveBoost(p, inv->boostInvIdx, "Invention_c");
            inv->boostInvIdx = -1;
        }

        // clear the state
        invention_Clear(p);
        p->invention->statusChange = true;
    }
}
#endif // SERVER


//------------------------------------------------------------
//  checks if valid
//----------------------------------------------------------
bool inventionstate_Valid(InventionState state)
{
    return INRANGE( state, kInventionState_None, kInventionState_Count );
}

//------------------------------------------------------------
//  checks if valid
//----------------------------------------------------------
bool slotstate_Valid(SlotState state)
{
    return INRANGE( state, kSlotState_Empty, kSlotState_Count );
}

//------------------------------------------------------------
//  find the clamped actual and potential values for the 
// concepts slotted in this invention whose type is the passed
// powervar.
//----------------------------------------------------------
void invention_ValsForVar(Invention *inv, const PowerVar *var, F32 *resActual, F32 *resPotential)
{
    F32 actual = 0.f;
    F32 potential = 0.f;
    int i;
    
    if( verify( inv && var ))
    {    
        for( i = 0; i < ARRAY_SIZE(inv->slots);++i )
        {
            InventionSlot *slot = inv->slots + i;
            
            if( slot->state == kSlotState_HardenSuccess 
                || slot->state == kSlotState_Slotted )
            {
                // only var 0 contributes to value
                ConceptItem *cpt = eaSize(&slot->concepts) > 0 ? slot->concepts[0] : NULL;                        
                int i_am = conceptdef_GetAttribModGroupIdx( cpt ? cpt->def : NULL, var->pchName );
                if( i_am >= 0 )
                {
                    if( slot->state == kSlotState_HardenSuccess )
                    {
                        actual += cpt->afVars[i_am];
                    }
                    else if( slot->state == kSlotState_Slotted )
                    {
                        potential += cpt->afVars[i_am];
                    }
                }
            }
        }

        actual = CLAMPF32(actual, var->fMin, var->fMax);
        potential = CLAMPF32( actual+potential, var->fMin, var->fMax );
    }

    // --------------------
    // finally

    if( resActual )
    {
        *resActual = actual;
    }

    if( resPotential )
    {
        *resPotential = potential;
    }
}
