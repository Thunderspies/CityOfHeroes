

#include "UI/uiUtil.h"
#include "UI/uiContextMenu.h"
#include "UI/uiTray.h"
#include "UI/uiTeam.h"
#include "UI/uiTarget.h"
#include "UI/uiCursor.h"
#include "UI/uiNet.h"
#include "UI/uiDialog.h"

#include "entity/character_base.h"
#include "entity/powers.h"
#include "entity/boost.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "player/player.h"
#include "UI/sprite/sprite_text.h"
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include <utilitieslib/components/earray.h>
#include "entity/character_inventory.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "bases/DetailRecipe.h"
#include "account/AccountData.h"
#include "UI/uiChat.h"

static ContextMenu *gGiftSubMenu = {0};

//------------------------------------------------------------------------------------------------
// Dealing with gifts before and after networking ////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------

void gift_SendAndRemove( int dbid, TrayObj *item )
{
    Entity *e = playerPtr();
    const BasePower *ppow;

    //first order of business, find the actual item and remove it (deletion repeated server-side)
     if( item->type == kTrayItemType_Inspiration )
    {
        ppow = e->pchar->aInspirations[item->iset][item->ipow];
        if (!ppow)
            return;

        if (!inspiration_IsTradeable(ppow, e->auth_name, ""))
        {
            addSystemChatMsg(textStd("GiftFailedUntradeable", textStd(ppow->pchDisplayName)), INFO_SVR_COM, 0);
            return;
        }

        character_RemoveInspiration(e->pchar, item->iset, item->ipow, NULL);
    }
    else if( item->type == kTrayItemType_SpecializationInventory )
    {
        if (!e->pchar->aBoosts[item->ispec])
            return;

        ppow = e->pchar->aBoosts[item->ispec]->ppowBase;
        if( !ppow )
            return;

        if (!detailrecipedict_IsBoostTradeable(ppow, e->pchar->aBoosts[item->ispec]->iLevel, e->auth_name, ""))
        {
            addSystemChatMsg(textStd("GiftFailedUntradeable", textStd(ppow->pchDisplayName)), INFO_SVR_COM, 0);
            return;
        }

        character_RemoveBoost(e->pchar, item->ispec, NULL);
    }
    else if( item->type == kTrayItemType_Power ) // we'll let power remove itself on server
    {
        float sc = 1.f;
        trayslot_Timer( e, item, &sc );

        // cannot trade active or recharging powers
        if( !trayslot_IsActive(e, item) && sc == 1.f )
        {
            if( EAINRANGE(item->iset, e->pchar->ppPowerSets) && e->pchar->ppPowerSets[item->iset] &&
                EAINRANGE(item->ipow, e->pchar->ppPowerSets[item->iset]->ppPowers) && e->pchar->ppPowerSets[item->iset]->ppPowers[item->ipow] )
            {
                ppow = e->pchar->ppPowerSets[item->iset]->ppPowers[item->ipow]->ppowBase;
                if( !ppow || !ppow->bTradeable )
                    return;
            }
        }
        else
            return;
    }
    else if(item->type == kTrayItemType_Salvage)
    {
        if( item->invIdx < 0 || 
            item->invIdx >= eaSize(&e->pchar->salvageInv) ||
            !character_CanAdjustSalvage(e->pchar, e->pchar->salvageInv[item->invIdx]->salvage, -1 ) ||
            (e && e->pchar && e->pchar->salvageInv && e->pchar->salvageInv[item->invIdx] && e->pchar->salvageInv[item->invIdx]->salvage &&
                (e->pchar->salvageInv[item->invIdx]->salvage->flags & SALVAGE_NOTRADE)))
        {
            return;
        }
    }
    else if(item->type == kTrayItemType_Recipe)
    {
        const DetailRecipe *pRec = detailrecipedict_RecipeFromId(item->invIdx);
        if ( pRec == NULL || !character_CanRecipeBeChanged(e->pchar, pRec, -1 ) || (pRec->flags & RECIPE_NO_TRADE) )
        {
            return;
        }
    }
    else
        return; // something we are not allowed to send

    // now send to mapserver
    gift_send( dbid, item->type,  item->iset, item->ipow, item->ispec, item->invIdx );
}



//------------------------------------------------------------------------------------------------
// Additions to Contex SubMenu ///////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------

static int gift_canSendToTeammate(void *data)
{
    int num = *((int*)data);
    Entity *e = playerPtr();

    if( !e->teamup || num >= e->teamup->members.count || e->teamup->members.ids[num] == e->db_id )
        return CM_HIDE;
    
    if( !entFromDbId( e->teamup->members.ids[num] ) )
        return CM_VISIBLE;

    return CM_AVAILABLE;
}

static char * gift_TeammateName(void *data)
{
    int num = *((int*)data);
    Entity *e = playerPtr();

    if( !e->teamup || num >= e->teamup->members.count )
        return "ErrorFilter";

    return e->teamup->members.names[num];
}

int teammateNum = -1;
TrayObj giftCopy;

void gift_ConfirmSendToTeammate(void *data)
{
    Entity *e = playerPtr();
    Entity *teammate = NULL;

    if( !e->teamup || teammateNum >= e->teamup->members.count )
        return;

    teammate = entFromDbId(e->teamup->members.ids[teammateNum]);

    gift_SendAndRemove( teammate->svr_idx, &giftCopy );
    teammateNum = -1;
}

void gift_DenySendToTeammate(void *data)
{
     teammateNum = -1;
}
 
static void gift_sendToTeammate( int num, void *data )
{
    TrayObj *item = (TrayObj *)data;
    Entity *e = playerPtr();
    Entity *teammate;

    if( !e->teamup || num >= e->teamup->members.count )
        return;
    teammate = entFromDbId(e->teamup->members.ids[num]);

    if ( item->type == kTrayItemType_SpecializationInventory && e && e->pchar)
    {
        Boost *pBoost = e->pchar->aBoosts[item->ispec];

        if (pBoost != NULL && pBoost->iNumCombines > 0 && teammateNum < 0)
        {
            teammateNum = num;
            memcpy(&giftCopy, item, sizeof(TrayObj));
            dialog( DIALOG_YES_NO, -1, -1, -1, -1, "ReallyTradeCombinedEnhancement", NULL, gift_ConfirmSendToTeammate, NULL, gift_DenySendToTeammate, 0, NULL, NULL,0,0,0, NULL );
            return;
        }
    }

    gift_SendAndRemove(teammate->svr_idx, item );
}


// this is hella lame, but I couldn't figure any better way to cram it into existing system
static void gift_sendToMemeber0(void *data)
{
    gift_sendToTeammate( 0, data );
}
static void gift_sendToMemeber1(void *data)
{
    gift_sendToTeammate( 1, data );
}
static void gift_sendToMemeber2(void *data)
{
    gift_sendToTeammate( 2, data );
}
static void gift_sendToMemeber3(void *data)
{
    gift_sendToTeammate( 3, data );
}
static void gift_sendToMemeber4(void *data)
{
    gift_sendToTeammate( 4, data );
}
static void gift_sendToMemeber5(void *data)
{
    gift_sendToTeammate( 5, data );
}
static void gift_sendToMemeber6(void *data)
{
    gift_sendToTeammate( 6, data );
}
static void gift_sendToMemeber7(void *data)
{
    gift_sendToTeammate( 7, data );
}

static int team_PlayerIsOnTeamAdapter(void)
{
    return team_PlayerIsOnTeam();
}

static const char * gift_TeammateNameAdapter(void* arg0)
{
    return gift_TeammateName((void *)arg0);
}

static void gift_initSubMenu()
{
    int i;
    static int teamlist[] = {0,1,2,3,4,5,6,7};
    static void (*code[])(void*) = { gift_sendToMemeber0, gift_sendToMemeber1, gift_sendToMemeber2, gift_sendToMemeber3,
                                gift_sendToMemeber4, gift_sendToMemeber5, gift_sendToMemeber6, gift_sendToMemeber7 };

    if( gGiftSubMenu )
        return;

    gGiftSubMenu = contextMenu_Create( team_PlayerIsOnTeamAdapter);

    for( i = 0; i < MAX_TEAM_MEMBERS; i++ )
    {
        contextMenu_addVariableTextCode( gGiftSubMenu, gift_canSendToTeammate, &teamlist[i], code[i], 0, gift_TeammateNameAdapter, &teamlist[i], 0 );
    }

}

//------------------------------------------------------------------------------------------------
// Additions to ContextMenu //////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------

static int gift_showSubMenu(void *data)
{
    Entity *e = playerPtr();
    TrayObj *item = (TrayObj*)data;

    // see if we have permission to trade
    if (!e->pl || !AccountCanTrade(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
        return CM_HIDE;

     if( e->teamup && e->teamup->members.count > 1 )
    {
        if( item->type == kTrayItemType_Power )
        {
            if( item->iset >= 0 && item->iset < eaSize(&e->pchar->ppPowerSets) &&
                item->ipow >= 0 && item->ipow < eaSize(&e->pchar->ppPowerSets[item->iset]->ppPowers) )
            {
                const BasePower *ppow;
                float sc;
                Power *ppower = e->pchar->ppPowerSets[item->iset]->ppPowers[item->ipow];
                if (!ppower)
                {
                    return CM_HIDE;
                }

                ppow = ppower->ppowBase;
                sc = 1.f;

                if( !ppow || !ppow->bTradeable )
                {
                    return CM_HIDE;
                }
            
                trayslot_Timer( e, item, &sc );
                if( trayslot_IsActive(e, item) || sc != 1.f )
                {
                    return CM_VISIBLE;
                }
            }
        }
        return CM_AVAILABLE;
    }

    return CM_HIDE;
}

static int gift_canSendToTarget(void *data)
{
    Entity *e = playerPtr();
    TrayObj *item = (TrayObj*)data;

    if(!current_target || ENTTYPE(current_target) != ENTTYPE_PLAYER || !e->pl || 
        !AccountCanTrade(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
        return CM_HIDE;

    if( item->type == kTrayItemType_Power )
    {
        if( EAINRANGE(item->iset, e->pchar->ppPowerSets) && e->pchar->ppPowerSets[item->iset] &&
            EAINRANGE(item->ipow, e->pchar->ppPowerSets[item->iset]->ppPowers) && e->pchar->ppPowerSets[item->iset]->ppPowers[item->ipow])
        {
            const BasePower *ppow = e->pchar->ppPowerSets[item->iset]->ppPowers[item->ipow]->ppowBase;
            float sc = 1.f;

            if( !ppow || !ppow->bTradeable )
                return CM_HIDE;

            trayslot_Timer( e, item, &sc );
            if( trayslot_IsActive(e, item) || sc != 1.f )
                return CM_VISIBLE;
        }
        else
        {
            return CM_HIDE;
        }
    }
    return CM_AVAILABLE;
}

static char *gift_TargetName(void *unused)
{
    Entity *e = playerPtr();

    if(current_target && ENTTYPE(current_target) == ENTTYPE_PLAYER)
        return textStd( "CMGive", current_target->name );
    else
        return NULL;
}

int targetNum = -1;
TrayObj giftTargetCopy;

void gift_ConfirmSendToTarget(void *data)
{
    TrayObj *item = (TrayObj*)data;

    gift_SendAndRemove( current_target->svr_idx, item );
    targetNum = -1;
}

void gift_DenySendToTarget(void *data)
{
    targetNum = -1;
}

static void gift_sendToTarget(void *data)
{ 
    Entity *e = playerPtr();
    TrayObj *item = (TrayObj*)data;

    if(!current_target || ENTTYPE(current_target) != ENTTYPE_PLAYER)
        return;

    // check for confirmation
    if ( item->type == kTrayItemType_SpecializationInventory && e && e->pchar)
    {
        Boost *pBoost = e->pchar->aBoosts[item->ispec];

        if (pBoost != NULL && pBoost->iNumCombines > 0 && targetNum < 0)
        {
            targetNum = current_target->svr_idx;
            memcpy(&giftTargetCopy, item, sizeof(TrayObj));
            dialog( DIALOG_YES_NO, -1, -1, -1, -1, "ReallyTradeCombinedEnhancement", NULL, gift_ConfirmSendToTarget, NULL, gift_DenySendToTarget, 0, NULL, NULL,0,0,0, data );
            return;
        }
    }

    gift_SendAndRemove( current_target->svr_idx, item );

}

static const char * gift_TargetNameAdapter(void* arg0)
{
    return gift_TargetName((void *)arg0);
}

void gift_addToContextMenu( ContextMenu * cm )
{
    gift_initSubMenu();

    contextMenu_addVariableTextCode( cm, gift_canSendToTarget, 0, gift_sendToTarget, 0, gift_TargetNameAdapter, 0, 0 );
    contextMenu_addCode( cm, gift_showSubMenu, 0, 0, 0, "CMGiveTo", gGiftSubMenu );
    
}