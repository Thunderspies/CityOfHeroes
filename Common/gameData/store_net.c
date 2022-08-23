/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/network/netio.h>

#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "entity/powers.h"
#include "storyarc/storyarcprivate.h"
#include "storyarc/pnpc.h"
#include "entity/entVarUpdate.h"
#include "entity/entserver.h"
#include "comm_game.h"
#include "storyarc/contact.h"
#include "storyarc/pnpcCommon.h"
#include "store.h"

/**********************************************************************func*
 * store_SendOpenStore
 *
 */
bool store_SendOpenStore(Entity *e, int iOwner, int iCnt, const MultiStore *pmulti)
{
    int i;
    int iCntStores;

    if(!e || !pmulti)
        return false;

    START_PACKET(pak, e, SERVER_STORE_OPEN);
    pktSendBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX, iOwner);

    iCntStores = iCnt > pmulti->iCnt ? pmulti->iCnt : iCnt;

    pktSendBitsPack(pak, 2, iCntStores);
    for(i=0; i<iCntStores; i++)
    {
        pktSendString(pak, pmulti->ppchStores[i]);
    }
    END_PACKET

    return true;
}

/**********************************************************************func*
 * store_SendOpenStoreContact
 *
 */
bool store_SendOpenStoreContact(Entity *e, int iCnt, const MultiStore *pmulti)
{
    if(!e || !pmulti)
        return false;

    return store_SendOpenStore(e, 0, iCnt, pmulti);
}

/**********************************************************************func*
 * store_SendOpenStorePNPC
 *
 */
bool store_SendOpenStorePNPC(Entity *e, Entity *npc, const MultiStore *pmulti)
{
    if(!e || !pmulti)
        return false;

    return store_SendOpenStore(e, npc->owner, pmulti->iCnt, pmulti);
}

/**********************************************************************func*
 * store_ReceiveBuyItem
 *
 */
bool store_ReceiveBuyItem(Packet *pak, Entity *e)
{
    int type;
    int row = -1;
    int col;
    int iCntValid;
    int id = pktGetBitsPack(pak, 12);
    const MultiStore *pstore;
    StoryInfo *info = StoryInfoFromPlayer(e);

    type = pktGetBits(pak, 1);
    if(type==0)
    {
        type = kPowerType_Inspiration;
        row = pktGetBitsPack(pak, 3);
        col = pktGetBitsPack(pak, 3);
    }
    else if(type==1)
    {
        type = kPowerType_Boost;
        row = pktGetBitsPack(pak, 4);
        col = 0;
    }

    if(id>0 && id==e->pl->npcStore && entFromId(id))
    {
        Entity *npc = entFromId(id);

        if (npc->persistentNPCInfo && npc->persistentNPCInfo->pnpcdef)
        {
            pstore = &npc->persistentNPCInfo->pnpcdef->store;
            iCntValid = npc->persistentNPCInfo->pnpcdef->store.iCnt;
        } else {
            // script driven store
            pstore = ScriptDataGetStoreBeingUsed(e, npc);
            iCntValid = pstore->iCnt;
        }
    }
    else if(id==0 && info->interactTarget!=0)
    {
        StoryContactInfo* contactInfo;
        contactInfo = ContactGetInfo(info, info->interactTarget);
        if (!contactInfo)
            return false;

        iCntValid = contactInfo->contactRelationship;
        pstore = &contactInfo->contact->def->stores;
    }
    else
    {
        return false;
    }

    if(pstore && e && e->pchar)
    {
        multistore_BuyItem(pstore, iCntValid, e->pchar, type, row, col);
        return true;
    }

    return false;
}

/**********************************************************************func*
 * store_ReceiveSellItem
 *
 */

static bool receiveSellItem_NpcStore(Entity *e, int id, const MultiStore **pstore, int *iCntValid)
{
    Entity *npc = entFromId(id);
    if(npc)  //assuming e && id > 0 && pstore && iCntValid  removed check on MChock's suggestion
    {
        if (npc->persistentNPCInfo && npc->persistentNPCInfo->pnpcdef)
        {
            *pstore = &npc->persistentNPCInfo->pnpcdef->store;
            *iCntValid = npc->persistentNPCInfo->pnpcdef->store.iCnt;
        } else {
            // script driven store
            *pstore = ScriptDataGetStoreBeingUsed(e, npc);
            if(devassertmsg(*pstore, "Entity attempting to access store where either player or NPC merchant has no script data."))
            {
                *iCntValid = (*pstore)->iCnt;
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

static bool receiveSellItem_Contact(Entity *e, ContactHandle interactTarget, const MultiStore **pstore, int *iCntValid)
{
    //assuming e && pstore ** iCntValid.  Removed check on MChock's suggestion
    StoryContactInfo* contactInfo;
    StoryInfo *pInfo = StoryInfoFromPlayer(e);

    contactInfo = ContactGetInfo(pInfo, interactTarget);
    if (!contactInfo)
    {
        return false;
    }

    *iCntValid = contactInfo->contactRelationship;
    *pstore = &contactInfo->contact->def->stores;
    return true;
}

bool store_ReceiveSellItem(Packet *pak, Entity *e)
{
    MultiStore *pstore = NULL;
    int iCntValid;
    StoryInfo *info = e->storyInfo;                // store interact info is always stored in the player's story info

    int id = pktGetBitsPack(pak, 12);
    char *pchItem = pktGetString(pak);

    if(id>0 && id==e->pl->npcStore && entFromId(id))
    {
        if(!receiveSellItem_NpcStore(e, id, &pstore, &iCntValid))
            return false;
    }
    else if(id==0 && info->interactTarget!=0)
    {
        if(!receiveSellItem_Contact(e, info->interactTarget, &pstore, &iCntValid))
            return false;
    }

    if(pstore && e && e->pchar)
    {
        multistore_SellItemByName(pstore, iCntValid, e->pchar, pchItem);
        return true;
    }

    return false;
}

/**********************************************************************func*
* store_ReceiveBuySalvage
*
*/
bool store_ReceiveBuySalvage(Packet *pak, Entity *e)
{
    const MultiStore *pstore;
    int iCntValid;
    StoryInfo *info = StoryInfoFromPlayer(e);

    int id = pktGetBitsPack(pak, 12);
    int count = pktGetBitsPack(pak, 8);
    int salvageId = pktGetBitsPack(pak, 12);

    if(id>0 && id==e->pl->npcStore && entFromId(id))
    {
        Entity *npc = entFromId(id);
        if (npc->persistentNPCInfo && npc->persistentNPCInfo->pnpcdef)
        {
            pstore = &npc->persistentNPCInfo->pnpcdef->store;
            iCntValid = npc->persistentNPCInfo->pnpcdef->store.iCnt;
        } else {
            // script driven store
            pstore = ScriptDataGetStoreBeingUsed(e, npc);
            iCntValid = pstore->iCnt;
        }
    }
    else if(id==0 && info->interactTarget!=0)
    {
        StoryContactInfo* contactInfo;
        contactInfo = ContactGetInfo(info, info->interactTarget);
        if (!contactInfo)
            return false;

        iCntValid = contactInfo->contactRelationship;
        pstore = &contactInfo->contact->def->stores;
    }
    else
    {
        return false;
    }

    if(pstore && e && e->pchar)
    {
        multistore_BuySalvage(pstore, iCntValid, e->pchar, salvageId, count);
        return true;
    }

    return false;
}

/**********************************************************************func*
* store_ReceiveSellRecipe
*
*/
bool store_ReceiveBuyRecipe(Packet *pak, Entity *e)
{
    const MultiStore *pstore;
    int iCntValid;
    StoryInfo *info = StoryInfoFromPlayer(e);

    int id = pktGetBitsPack(pak, 12);
    int count = pktGetBitsPack(pak, 8);
    int recipeId = pktGetBitsPack(pak, 12);

    if(id>0 && id==e->pl->npcStore && entFromId(id))
    {
        Entity *npc = entFromId(id);
        if (npc->persistentNPCInfo && npc->persistentNPCInfo->pnpcdef)
        {
            pstore = &npc->persistentNPCInfo->pnpcdef->store;
            iCntValid = npc->persistentNPCInfo->pnpcdef->store.iCnt;
        } else {
            // script driven store
            pstore = ScriptDataGetStoreBeingUsed(e, npc);
            iCntValid = pstore->iCnt;
        }
    }
    else if(id==0 && info->interactTarget!=0)
    {
        StoryContactInfo* contactInfo;
        contactInfo = ContactGetInfo(info, info->interactTarget);
        if (!contactInfo)
            return false;

        iCntValid = contactInfo->contactRelationship;
        pstore = &contactInfo->contact->def->stores;
    }
    else
    {
        return false;
    }

    if(pstore && e && e->pchar)
    {
        multistore_BuyRecipe(pstore, iCntValid, e->pchar, recipeId, count);
        return true;
    }

    return false;
}


/* End of File */
