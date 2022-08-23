/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "entity/character_workshop.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/MemoryPool.h>
#include "entity/character_base.h"
#include "bases/DetailRecipe.h"
#include <utilitieslib/network/netio.h>
#include "entity/entity.h"
#include "entity/character_inventory.h"

#if CLIENT
#include "UI/uiNet.h"
#include "UI/uiWindows.h"
#include "gameComm/wdwbase.h"
#endif // CLIENT

#if SERVER
#include "dbcomm/logcomm.h"
#include "bases/bases.h"
#include "bases/basedata.h"
#include "entity/entPlayer.h"
#include <utilitieslib/utils/mathutil.h>
#include "bases/baseserver.h"
#include "gameComm/svr_chat.h"
#include "language/langServerUtil.h"
#include "cmdparse/cmdserver.h"
#endif

/*
typedef struct character_WorkshopTypeMapLine
{
    char            *name;
    WorkshopType    type;
} character_WorkshopTypeMapLine;

static character_WorkshopTypeMapLine character_WorkshopTypeMap[] =
{
    { "Worktable_None",                    kWorkshopType_None },
    { "Worktable_Invention",            kWorkshopType_Invention_Basic },
    { "Worktable_Invention_Arcane",        kWorkshopType_Invention_Arcane },
    { "Worktable_Invention_Tech",        kWorkshopType_Invention_Tech }, 
};



int character_WorkshopWorkshopTypeByName(char *name)
{
    int i;

    for (i = 0; i < kWorkshopType_Count; i++)
    {
        if (stricmp(character_WorkshopTypeMap[i].name, name) == 0)
            return character_WorkshopTypeMap[i].type;

    }
    return kWorkshopType_None;
}

char *character_WorkshopWorkshopNameByType(int type)
{
    int i;

    for (i = 0; i < kWorkshopType_Count; i++)
    {
        if (character_WorkshopTypeMap[i].type == type)
            return character_WorkshopTypeMap[i].name;

    }
    return NULL;
}
*/

/**********************************************************************************************/
#if CLIENT
bool character_WorkshopSendDetailRecipeBuild(Character *p, char const *nameDetailRecipe, int iLevel, int bUseCoupon)
{
    bool res = false;

    if( verify( p ))
    {
        const DetailRecipe *r =  detailrecipedict_RecipeFromName(nameDetailRecipe);

        // --------------------
        // see if actually usable

        if( r && character_DetailRecipeCreatable( p, r, NULL, true, bUseCoupon, -1 ))
        {
            workshop_SendCreate( r, iLevel, bUseCoupon );
            res = true;
        }
    }
    return res;
}

bool character_WorkshopSendDetailRecipeBuy(Character *p, char const *nameDetailRecipe)
{
    bool res = false;

    if( verify( p ))
    {
        const DetailRecipe *r =  detailrecipedict_RecipeFromName(nameDetailRecipe);

        // --------------------
        // see if actually usable
        if( r && character_CanRecipeBeChanged( p, r, 1) )
        {
            workshop_SendBuy(r);
            res = true;
        } else {
            res = false;
        }
    }
    return res;
}

#endif // CLIENT
/**********************************************************************************************/


/**********************************************************************************************/
#if SERVER
void character_WorkshopReceiveDetailRecipeBuild(Character *p, Packet *pak)
{
    int failure = 0;
    RoomDetail *olddet;
    BaseRoom *room;
    EntPlayer *pl;
    const DetailRecipe *r = NULL;
    char *pchRecipe = NULL;
    int iLevel;
    int bUseCoupon;

    if( verify( pak ))
    {
        pchRecipe = pktGetString(pak);
        iLevel = pktGetBitsAuto(pak);
        bUseCoupon = pktGetBitsAuto(pak);

        r = detailrecipedict_RecipeFromName(pchRecipe);
        if(!r)
        {
            dbLog("cheater",NULL,"Player with authname %s tried to send invalid workshop recipe name (%s)\n", p->entParent->auth_name, pchRecipe);
            return;
        }
    }

    pl = p->entParent->pl;
    if (!pl->detailInteracting && pl->workshopInteracting[0] == 0)
    {
        failure = true;
    }
    else
    {
        // checks for base workshops
        if (pl->workshopInteracting[0] == 0)
        {
            olddet = baseGetDetail(&g_base,pl->detailInteracting,NULL);
            if (!olddet || olddet->info->eFunction != kDetailFunction_Workshop)
            {
                failure = true;
            }
            room = baseGetRoom(&g_base,olddet->parentRoom);
            if (!room)
            {
                failure = true;
            }
            if (!failure)
            {
                Vec3 loc;
                //Check to see if the detail is in range
                addVec3(room->pos,olddet->mat[3],loc);
                if (distance3Squared(loc, ENTPOS(p->entParent))>SQR(20))
                    failure = true;
            }
        }
    }


    if (failure && p->entParent->access_level < 9) //player can't actually workshop at this point
    {
        baseDeactivateDetail(p->entParent,kDetailFunction_Workshop);
    }
    else if (r)
    {
        if( r->flags & RECIPE_CERTIFICATON || r->flags & RECIPE_VOUCHER )  
        {
            
            if( character_DetailRecipeCreatable( p, r, 0, 1, 0, -1 ) )
            {
                // send the request to the account server first
                char * estr;
                estrCreate(&estr);
                estrPrintf(&estr, "account_certification_buy %i 1 %s", (r->flags&RECIPE_VOUCHER)!=0, r->pchName );
                serverParseClientEx( estr, p->entParent->client, ACCESS_INTERNAL );
                estrDestroy(&estr);

                if(r->pchDisplayAccountItemPurchase)
                    sendInfoBox(p->entParent, INFO_REWARD, r->pchDisplayAccountItemPurchase);
            }
        }
        else
        {
            int result = character_DetailRecipeCreate( p, r, iLevel, bUseCoupon );

            if (result == 0)
            { 
                // 0 means failed to reward (inventory full)
                chatSendToPlayer( p->entParent->db_id, localizedPrintf(p->entParent,"WorkshopFailInventory", r->ui.pchDisplayName), INFO_USER_ERROR, 0 );
                            
            } 
            else if (result == -1)
            {
                // -1 means failure to satisfy requirements, probably cheating
                dbLog("cheater",NULL,"Player with authname %s tried to create recipe %s without meeting requirements\n", p->entParent->auth_name,pchRecipe);
            }
        }
    }
}


/**********************************************************************func*
 * character_WorkshopTick
 *
 */
void character_WorkshopInterrupt(Entity *e)
{
    if(e && e->pl && strlen(e->pl->workshopInteracting) > 0)
    {
        e->pl->workshopInteracting[0] = 0;
        e->pl->workshopEntity = 0;
        sendActivateUpdate(e, false, 0, kDetailFunction_Workshop);
    }
}

#endif // SERVER

