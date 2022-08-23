/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>

#include <utilitieslib/network/netio.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/error.h>

#include "entity/entity.h"
#include "player/player.h"
#include "entity/classes.h"
#include "entity/origins.h"
#include "entity/powers.h"

#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/character_net.h"
#include "entity/character_net_client.h"
#include "UI/sprite/sprite_text.h"

#include "UI/uiInspiration.h"
#include "UI/uiTrade.h"
#include "UI/uiBuff.h"
#include "UI/uiRecipeInventory.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/attrib_description.h"

/**********************************************************************func*
 * character_SendCreate
 *
 * Sends character origin, class, and power choices to server at
 * character creation.
 *
 */
void character_SendCreate(Packet* pak, Character *pchar)
{
    int iCnt = 0;
    int j;
    int iCntSets;

    //
    // string ClassName
    // string OriginName
    // Powers
    // Inspirations
    //

    /* Basic Stuff ****************************************************/

    pktSendString(pak, pchar->pclass->pchName);
    pktSendString(pak, pchar->porigin->pchName);

    /* Powers *********************************************************/

    iCntSets = eaSize(&pchar->ppPowerSets);
    for(j=1; j<iCntSets && iCnt<2; j++)
    {
        PowerSet *pset = pchar->ppPowerSets[j];

        if(pset->psetBase->pcatParent == pchar->pclass->pcat[kCategory_Primary]
                || pset->psetBase->pcatParent == pchar->pclass->pcat[kCategory_Secondary])
        {
            // At character create, you only get one power per set
            basepower_Send(pak, pset->ppPowers[0]->ppowBase);
            pktSendBitsAuto(pak, pset->ppPowers[0]->iUniqueID);
            iCnt++;
        }
    }
}

/**********************************************************************func*
 * inspiration_Receive
 *
 * NET: fully consumes data even on error
 */
void inspiration_Receive(Packet *pak, Character *pchar)
{
    int iCol;
    int iRow;

    // If pchar is NULL, data is still consumed safely.

    // inspiration slot index
    // 0
    //   empty slot
    // 1
    //   basepower

    iCol = pktGetBitsPack(pak, 3);
    iRow = pktGetBitsPack(pak, 3);

    inspirationNewData( iCol, iRow );

    if(pktGetBits(pak, 1))
    {
        const BasePower *ppowBase = basepower_Receive(pak, &g_PowerDictionary, (pchar ? pchar->entParent : 0));
        if(pchar!=NULL && ppowBase!=NULL)
        {
            character_SetInspiration( pchar, ppowBase, iCol, iRow);
            trade_setInspiration( ppowBase, iCol, iRow );
        }
        else
        {
            assert(0);
        }
    }
    else if(pchar!=NULL)
    {
        // Empty slot
        character_RemoveInspiration(pchar, iCol, iRow, NULL);
        trade_setInspiration( NULL, iCol, iRow );
    }


}

/**********************************************************************func*
 * character_ReceiveInspirations
 *
 * NET: fully consumes data even on error
 */
void character_ReceiveInspirations(Packet *pak, Character *pchar)
{
    int iCols;
    int iRows;
    int i;
    int j;

    // count of inspiration columns
    // count of inspiration rows
    //    inspiration[0][0]
    //    inspiration[0][1]...

    iCols = pktGetBitsPack(pak, 3);
    iRows = pktGetBitsPack(pak, 3);

    if(pchar!=NULL)
    {
        pchar->iNumInspirationCols = iCols;
        pchar->iNumInspirationRows = iRows;
    }

    for(i=0; i<iCols; i++)
    {
        for(j=0; j<iRows; j++)
        {
            if(pktGetBits(pak, 1))
            {
                const BasePower *ppowBase = basepower_Receive(pak, &g_PowerDictionary, (pchar ? pchar->entParent : 0));
                if(pchar!=NULL && ppowBase!=NULL)
                {
                    character_SetInspiration(pchar, ppowBase, i, j);
                }
            }
            else if(pchar!=NULL)
            {
                character_RemoveInspiration(pchar, i, j, NULL);
            }
        }
    }
#if CLIENT && !TEST_CLIENT
    recipeInventoryReparse();
#endif

}


/**********************************************************************func*
 * character_Receive
 *
 * Receives full character information from server. Used when entering a
 * map.
 *
 * NET: fully consumes data even on error
 */
void character_Receive(Packet* pak, Character *pchar, Entity *e)
{
    const CharacterOrigin *porigin;
    const CharacterClass *pclass;
    int iLevel;
    int iCombatModShift;
    int iCurBuild;
    PlayerType playerTypeByLocation;
    char *pch;

    // No validation is done here. The server is trusted.

    /* Basic Stuff ****************************************************/

    pch = pktGetString(pak);
    pclass = classes_GetPtrFromName(&g_CharacterClasses, pch);
    if(!pclass)
        pclass = classes_GetPtrFromName(&g_VillainClasses, pch);

    pch = pktGetString(pak);
    porigin = origins_GetPtrFromName(&g_CharacterOrigins, pch);
    if(!porigin)
        porigin = origins_GetPtrFromName(&g_VillainOrigins, pch);

    iLevel = pktGetBitsPack(pak, 5);
    iCombatModShift = pktGetBitsAuto(pak);
    iCurBuild = pktGetBitsPack(pak, 4);
    playerTypeByLocation = pktGetBitsAuto(pak);

    if(porigin!=NULL && pclass!=NULL)
    {
        character_CreatePartOne(pchar, e, porigin, pclass);
        pchar->iLevel = iLevel;
        pchar->iCombatModShift = iCombatModShift;
        pchar->iCurBuild = iCurBuild;
        pchar->iActiveBuild = iCurBuild;
        pchar->ppPowerSets = pchar->ppBuildPowerSets[iCurBuild];
        pchar->playerTypeByLocation = playerTypeByLocation;
    }
    else
    {
        pchar=NULL;
    }

    /* Powers ***************************************************/

    character_ReceivePowers(pak, pchar);

    /* Inspirations ***************************************************/

    character_ReceiveInspirations(pak, pchar);

    /* Boosts ***************************************************/

    character_ReceiveBoosts(pak, pchar);


    /* Inventory ***************************************************/

    character_ReceiveInventories( pchar, pak);

    character_CreatePartTwo(pchar);

    // --------------------
    // validation

    if(pchar==NULL)
    {
        FatalErrorf("%s", textStd("BadCharFromServer"));
    }

    return;
}

/**********************************************************************func*
 * character_ReceiveBuffs
 *
 * Receives current buff information from server.
 *
 * NET: fully consumes data even on error
 */
void character_ReceiveBuffs(Packet *pak, Entity *e, bool oo_data)
{
    int i,j;
    int iCntBuffs, not_getting_buffs;
    int onlyGettingDiff;
    not_getting_buffs = pktGetBits(pak,1);

    if(not_getting_buffs)
    {
        if (!oo_data)
        {
            markPowerBuff(e);
            clearMarkedPowerBuff(e);
        }
        return;
    }

    onlyGettingDiff = pktGetBits(pak, 1);
    iCntBuffs = pktGetBitsPack(pak, 4);
    if (!onlyGettingDiff)
    {
        markPowerBuff(e);
    }
    for(i=0; i<iCntBuffs; i++)
    {
        const BasePower *ppow;
        AttribDescription **ppDesc=0;
        F32 *ppfMag=0;
        int iBlink = 0;
        int uid = 0, mod_cnt = 0;
        int markForDelete = 0;

        if (pktEnd(pak)) // JE: if networking code is out of sync, this loop can go on for millions of iterations!
            break;

        if (onlyGettingDiff)
        {
            markForDelete = pktGetBits(pak, 1);
        }
        ppow = basepower_Receive(pak, &g_PowerDictionary, e);

        if( pktGetBits(pak,1) )
        {
            iBlink = pktGetBits( pak, 1 );        //    indicates that we should blink now
            iBlink |= 2;                        //    clients sets this since it is something that will eventually blink (and shouldn't start out faded)
        }
        uid = pktGetBitsPack( pak, 20 );

        if (pktGetBits(pak, 1))
        {
            mod_cnt = pktGetBitsPack( pak, 5 );
        }
        else
        {
            mod_cnt = 0;
        }
        for(j=0;j<mod_cnt;j++)
        {
            AttribDescription *pDesc = 0;
            F32 fMag = ((int)pktGetBitsPack(pak,10))/100.f;
            int iHash = pktGetBitsPack(pak,10);

            pDesc = attribDescriptionGet(iHash);
            if(pDesc)
            {
                eaPush(&ppDesc, pDesc);
                eafPush(&ppfMag, fMag);
            }
        }

        // NOTE: if no mods received, keep the existing mods on the power
        if (!oo_data && markForDelete)
        {
            markBuffForDeletion(e, uid);
        }
        else
        {
            if (!oo_data && ppow)
                addUpdatePowerBuff( e, ppow, iBlink, uid, ppDesc, ppfMag );
        }

        eaDestroy(&ppDesc);
    }

    if (!oo_data)
        clearMarkedPowerBuff(e);
}

// character_sendBuffRemove
// void character_ReceiveBuffRemove(Packet *pak, Entity *e, bool oo_data)
// {
//     int i, iCntBuffs = pktGetBitsPack(pak, 5);

//     for(i=0; i<iCntBuffs; i++)
//         markBuffForDeletion(e, pktGetBitsPack(pak,20) );

//     clearMarkedPowerBuff(e);
// }

/**********************************************************************func*
 * character_ReceiveStatusEffects
 *
 * Receives set of crowd control effects that are currently active
 *
 * NET: fully consumes data even on error
 */
void character_ReceiveStatusEffects(Packet *pak, Character *p, bool oo_data)
{
    U32 effects = pktGetBits(pak, 10);
    int visionPhases = pktGetBitsAuto(pak);
    int exclusiveVisionPhase = pktGetBitsAuto(pak);
    int count = pktGetBitsAuto(pak);
    int i;

    //! @todo p can be NULL.  Why does this occur?  Should we prevent it from occurring, elsewhere?
    if (p && !oo_data)
        eaClearEx(&p->designerStatuses, NULL);
    for (i = 0; i < count; ++i)
    {
        if (!oo_data && p && p->entParent)
        {
            char *status = strdup(pktGetString(pak));
            eaPush(&p->designerStatuses, status);
        }
        else
        {
            pktGetString(pak);        //    just consuming the packet here
        }
    }
    if (!oo_data && p && p->entParent)
    {
        p->entParent->ulStatusEffects = effects;
        p->entParent->bitfieldVisionPhases = visionPhases;
    }
}

void character_SendBuyPower(Packet *pak, Character *pchar, const Power *ppow, PowerSystem ps)
{
    // power
    basepower_Send(pak, ppow->ppowBase);
    pktSendBitsAuto(pak, ppow->iUniqueID);
    pktSendBitsPack(pak, kPowerSystem_Numbits, ps );
}


/**********************************************************************func*
 * power_SendBuyBoostSlot
 *
 */
void power_SendBuyBoostSlot(Packet *pak, Power *ppow)
{
    // power

    basepower_Send(pak, ppow->ppowBase);
}

/**********************************************************************func*
 * power_SendCombineBoosts
 *
 */
void power_SendCombineBoosts(Packet *pak, Boost * pbA, Boost *pbB )
{
    if( pbA->ppowParent )
    {
        pktSendBits( pak, 1, 1 );
        pktSendBitsPack( pak, 1, pbA->ppowParent->psetParent->idx );
        pktSendBitsPack( pak, 1, pbA->ppowParent->idx );
        pktSendBitsPack( pak, 1, pbA->idx );
    }
    else
    {
        pktSendBits( pak, 1, 0 );
        pktSendBitsPack( pak, 1, pbA->idx );
    }

    if( pbB->ppowParent )
    {
        pktSendBits( pak, 1, 1 );
        pktSendBitsPack( pak, 1, pbB->ppowParent->psetParent->idx );
        pktSendBitsPack( pak, 1, pbB->ppowParent->idx );
        pktSendBitsPack( pak, 1, pbB->idx );
    }
    else
    {
        pktSendBits( pak, 1, 0 );
        pktSendBitsPack( pak, 1, pbB->idx );
    }
}

/* End of File */
