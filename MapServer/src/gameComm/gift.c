#include "entity/entity.h"
#include <utilitieslib/network/net_packetutil.h>
#include "svr/svr_base.h"
#include "entity/character_base.h"
#include "entity/boost.h"
#include <utilitieslib/components/earray.h>
#include "entity/powers.h"
#include "entity/entVarUpdate.h"
#include "gameComm/svr_chat.h"
#include "entity/entPlayer.h"
#include "language/langServerUtil.h"
#include "entity/teamup.h"
#include "dbcomm/dbnamecache.h"
#include "entity/entGameActions.h"
#include "entity/character_combat.h"
#include "gameComm/trayCommon.h"
#include <utilitieslib/utils/mathutil.h>
#include "gameSys/arenamap.h"
#include "entity/entserver.h"
#include "entity/character_target.h"
#include "entity/character_inventory.h"
#include "bases/DetailRecipe.h"
#include "dbcomm/logcomm.h"
#include "dbghelper.h"
#include "gameSys/arenamap.h"
#include "dbcomm/logcomm.h"

// something failed somewhere, pretend it never happened
//
static void gift_cancel( Entity *e, int type, int icol, int irow, int ispec )
{
    if( type == kTrayItemType_Inspiration )
        eaiPush(&e->inspirationStatusChange, icol*CHAR_INSP_MAX_ROWS+irow);
    else if( type == kTrayItemType_SpecializationInventory )
        eaiPush(&e->boostStatusChange, ispec);
    // powers and salvage we don't pre-update client so there is nothing to do
}  

// actually give player inspiration
//
static bool gift_grantInspiration( Entity *sender, Entity *recipient, const char * pchSet, const char * pchPow )
{
    const BasePower *ppow = powerdict_GetBasePowerByName(&g_PowerDictionary, "Inspirations", pchSet, pchPow);
    if(ppow!=NULL)
    {
        if( character_AddInspiration(recipient->pchar, ppow, "gift") >= 0 )
        {
            chatSendToPlayer( sender->db_id, localizedPrintf(sender,"GiftYouGave", ppow->pchDisplayName, recipient->name), INFO_SVR_COM, 0 );
            chatSendToPlayer( recipient->db_id, localizedPrintf(recipient,"GiftYouRecieved", ppow->pchDisplayName, sender->name), INFO_SVR_COM, 0 );
            serveFloater(recipient, recipient, "FloatFoundInspiration", 0.0f, kFloaterStyle_Attention, 0);
            return true;
        }
    }

    chatSendToPlayer( sender->db_id, localizedPrintf(sender,"GiftFailedUnknown"), INFO_USER_ERROR, 0 );
    return false;
}

// actually give player enhancement gift
//
static bool gift_grantEnhancement( Entity *sender, Entity *recipient, const char * pchSet, const char * pchPow, int level )
{
    const BasePower *ppow = powerdict_GetBasePowerByName(&g_PowerDictionary, "Boosts", pchSet, pchPow);
    if(ppow!=NULL)
    {
        if( character_AddBoost(recipient->pchar, ppow, level, 0, "gift") >= 0 )
        {
            chatSendToPlayer( sender->db_id, localizedPrintf(sender,"GiftYouGave", ppow->pchDisplayName, recipient->name), INFO_SVR_COM, 0 );
            chatSendToPlayer( recipient->db_id, localizedPrintf(recipient,"GiftYouRecieved", ppow->pchDisplayName, sender->name), INFO_SVR_COM, 0 );
            serveFloater(recipient, recipient, "FloatGiftEnhancement", 0.0f, kFloaterStyle_Attention, 0);
            return true;
        }
    }

    chatSendToPlayer( sender->db_id, localizedPrintf(sender,"GiftFailedUnknown"), INFO_USER_ERROR, 0 );
    return false;
}

// perform inspiration specific checks
//
static void gift_giveInspiration( Entity *e, Entity *recipient, int icol, int irow )
{
    const BasePower * ppow = e->pchar->aInspirations[icol][irow];

    if( !ppow )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"GiftFailedInspirationGone"), INFO_USER_ERROR, 0);
        gift_cancel( e, kTrayItemType_Inspiration, icol, irow, 0 );
        return;
    }
    if( character_InspirationInventoryFull(recipient->pchar) )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"GiftFailedRecipientFullInventory", recipient->name), INFO_USER_ERROR, 0);
        gift_cancel( e, kTrayItemType_Inspiration, icol, irow, 0 );
        return;
    }
    if( character_IsInspirationSlotInUse(e->pchar, icol, irow) )
    {
        gift_cancel( e, kTrayItemType_Inspiration, icol, irow, 0 );
        return;
    }

    if (!inspiration_IsTradeable(ppow, e->auth_name, ""))
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotTradeInspiration"), INFO_USER_ERROR, 0);
        gift_cancel(e, kTrayItemType_Inspiration, icol, irow, 0);
        return;
    }

    if( gift_grantInspiration( e, recipient, ppow->psetParent->pchName, ppow->pchName ) )
    {
        LOG_ENT( recipient, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[G]:Rcv:Insp %s from %s", dbg_BasePowerStr(ppow), e->name );
        character_RemoveInspiration( e->pchar, icol, irow, "gift" );
        character_CompactInspirations( e->pchar );
    }
    else
        gift_cancel( e, kTrayItemType_Inspiration, icol, irow, 0 );
}

// perform enhancement specific checks
//
static void gift_giveEnhancement( Entity *e, Entity *recipient, int ispec )
{
    Boost *pBoost = e->pchar->aBoosts[ispec];
    const BasePower *ppow;
    int iLevel;

    if( !pBoost || !pBoost->ppowBase )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"GiftFailedEnhancementGone"), INFO_USER_ERROR, 0);
        gift_cancel( e, kTrayItemType_SpecializationInventory, 0, 0, ispec );
        return;
    }

    ppow = pBoost->ppowBase;
    iLevel = pBoost->iLevel;

    if( character_BoostInventoryFull(recipient->pchar) )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"GiftFailedRecipientFullInventory", recipient->name), INFO_USER_ERROR, 0);
        gift_cancel( e, kTrayItemType_SpecializationInventory, 0, 0, ispec );
        return;
    }

    if (!detailrecipedict_IsBoostTradeable(ppow, iLevel, e->auth_name, ""))
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotTradeEnhancement"), INFO_USER_ERROR, 0);
        return;
    }

    if( gift_grantEnhancement( e, recipient, ppow->psetParent->pchName, ppow->pchName, iLevel ) )
    {
        character_RemoveBoost( e->pchar, ispec, "gift" );
        LOG_ENT( recipient, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[G]:Rcv:Enh %s from %s", dbg_BasePowerStr(ppow), e->name);
    }
    else
        gift_cancel( e, kTrayItemType_SpecializationInventory, 0, 0, ispec );
}

// perform power specific checks
//
static void gift_givePower( Entity *e, Entity *recipient, int iset, int ipow )
{
    Power * ppow = 0;

    if( iset >= 0 && iset < eaSize(&e->pchar->ppPowerSets) &&
        ipow >= 0 && ipow < eaSize(&e->pchar->ppPowerSets[iset]->ppPowers) )
    {
        ppow = e->pchar->ppPowerSets[iset]->ppPowers[ipow];
    }

    if( !ppow )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"GiftFailedInspirationGone"), INFO_USER_ERROR, 0);
        return;
    }

    if( !character_CanRemoveTemporaryPower( e->pchar, ppow ) )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"GiftFailedItemUse", ppow->ppowBase->pchDisplayName), INFO_USER_ERROR, 0);
        return;
    }

    if( !character_CanAddTemporaryPower( recipient->pchar, ppow) )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"GiftFailedRecipientCantUse", recipient->name, ppow->ppowBase->pchDisplayName), INFO_USER_ERROR, 0);
        return;
    }

    if( character_AddTemporaryPower( recipient->pchar, ppow ) )
    {
        chatSendToPlayer( e->db_id, localizedPrintf(e,"GiftYouGave", ppow->ppowBase->pchDisplayName, recipient->name), INFO_SVR_COM, 0 );
        chatSendToPlayer( recipient->db_id, localizedPrintf(recipient,"GiftYouRecieved", ppow->ppowBase->pchDisplayName, e->name), INFO_SVR_COM, 0 );
        serveFloater(recipient, recipient, "FloatGiftPower", 0.0f, kFloaterStyle_Attention, 0);
        LOG_ENT( recipient, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[G]:Rcv:Tpow %s from %s", dbg_BasePowerStr(ppow->ppowBase), e->name);
        character_RemovePower(e->pchar, ppow);
    }
    else
        chatSendToPlayer( e->db_id, localizedPrintf(e,"GiftFailedUnknown"), INFO_USER_ERROR, 0 );
}

// perform salvage specific checks
//
static void gift_giveSalvage( Entity *e, Entity *recipient, int idx )
{
    const SalvageItem * sal = NULL;

    if( idx >=0 && idx < eaSize(&e->pchar->salvageInv) )
        sal = e->pchar->salvageInv[idx]->salvage;

    if( !sal || !character_CanAdjustSalvage(e->pchar, sal, -1) )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"GiftFailedInspirationGone"), INFO_USER_ERROR, 0);
        return;
    }
    if( sal->flags & SALVAGE_NOTRADE )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotTradeSalvage"), INFO_USER_ERROR, 0);
        return;
    }
    if( !character_CanAdjustSalvage(recipient->pchar, sal, 1) )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"GiftFailedRecipientFullInventory", recipient->name), INFO_USER_ERROR, 0);
        return;
    }
    
    if( character_AdjustSalvage(recipient->pchar, sal, 1, "gift", false) >= 0 )
    {
        character_AdjustSalvage(e->pchar, sal, -1, "gift", false);
        chatSendToPlayer( e->db_id, localizedPrintf(e,"GiftYouGave", sal->ui.pchDisplayName, recipient->name), INFO_SVR_COM, 0 );
        chatSendToPlayer( recipient->db_id, localizedPrintf(recipient,"GiftYouRecieved", sal->ui.pchDisplayName, e->name), INFO_SVR_COM, 0 );
        serveFloater(recipient, recipient, "FloatGiftSalvage", 0.0f, kFloaterStyle_Attention, 0);
        LOG_ENT( recipient, LOG_REWARDS, LOG_LEVEL_IMPORTANT,  0, "[G]:Rcv:Sal %s from %s", localizedPrintf(e,sal->ui.pchDisplayName), e->name );
    }
    else
        chatSendToPlayer( e->db_id, localizedPrintf(e,"GiftFailedUnknown"), INFO_USER_ERROR, 0 );

}

// perform recipe specific checks
//
static void gift_giveRecipe( Entity *e, Entity *recipient, int idx )
{
    const DetailRecipe *pRec = detailrecipedict_RecipeFromId(idx);

    if( !pRec || !character_CanRecipeBeChanged(e->pchar, pRec, -1) )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"GiftFailedInspirationGone"), INFO_USER_ERROR, 0);
        return;
    }

    if( pRec->flags & RECIPE_NO_TRADE )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotTradeRecipe"), INFO_USER_ERROR, 0);
        return;
    }

    if( !character_CanRecipeBeChanged(recipient->pchar, pRec, 1) )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"GiftFailedRecipientFullInventory", recipient->name), INFO_USER_ERROR, 0);
        return;
    }

    if( character_AdjustRecipe(recipient->pchar, pRec, 1, "gift") >= 0 )
    {
        character_AdjustRecipe(e->pchar, pRec, -1, "gift");
        chatSendToPlayer( e->db_id, localizedPrintf(e,"GiftYouGave", pRec->ui.pchDisplayName, recipient->name), INFO_SVR_COM, 0 );
        chatSendToPlayer( recipient->db_id, localizedPrintf(recipient,"GiftYouRecieved", pRec->ui.pchDisplayName, e->name), INFO_SVR_COM, 0 );
        serveFloater(recipient, recipient, "FloatGiftRecipe", 0.0f, kFloaterStyle_Attention, 0);
        LOG_ENT( recipient, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[G]:Rcv:Rec %s from %s", localizedPrintf(e,pRec->ui.pchDisplayName), e->name );
    }
    else
        chatSendToPlayer( e->db_id, localizedPrintf(e,"GiftFailedUnknown"), INFO_USER_ERROR, 0 );

}


//
//
void gift_recieveSend( Entity *e, Packet * pak )
{
    //must consume data first
     int id = pktGetBitsPack( pak, 1 );
    int type = pktGetBitsPack( pak, 1 );
    Entity * recipient = entFromId( id );
    int icol, irow, ispec;

    if( type == kTrayItemType_Inspiration || type == kTrayItemType_Power)
    {
        icol = pktGetBitsPack( pak, 1 );
        irow = pktGetBitsPack( pak, 1 );
    }
    else if( type == kTrayItemType_SpecializationInventory || type == kTrayItemType_Salvage || type == kTrayItemType_Recipe)
    {
        ispec = pktGetBitsPack( pak, 1 );
    }

    // yummy, now check lots of factors
    if( !recipient ) //with the change, this should really never happen
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"GiftFailedRecipientGone", dbPlayerNameFromId(id)), INFO_USER_ERROR, 0);
        gift_cancel( e, type, icol, irow, ispec );
        return;
    }
 
    // if they are a pet we do different stuff
    if( ENTTYPE(recipient) != ENTTYPE_PLAYER )
    {
        if (stricmp( e->pchar->pclass->pchName, "Class_Mastermind") != 0 || !recipient->erOwner)  //not mastermind
        {
            gift_cancel(e,type,icol,irow,ispec);
            return;
        }
        if( type != kTrayItemType_Inspiration ||
            erGetEnt(recipient->erOwner) != e )    // not my pet
        {
            chatSendToPlayer( e->db_id, localizedPrintf(e,"PetCommandFailed"), INFO_USER_ERROR, 0 );
            gift_cancel( e, type, icol, irow,ispec );
            return;
        }
        else if (g_arenamap_event && g_arenamap_event->use_gladiators)
        {
            chatSendToPlayer( e->db_id, localizedPrintf(e,"PetCommandFailed"), INFO_USER_ERROR, 0 );
            gift_cancel( e, type, icol, irow,ispec );
            return;
        }
        else
        {
            character_ActivateInspiration( e->pchar, recipient->pchar, icol, irow );
        }
        return;
    }

     if( OnArenaMap() ) 
    {
        chatSendToPlayer( e->db_id, localizedPrintf(e,"GiftFailedArena", recipient->name ), INFO_USER_ERROR, 0 );
        gift_cancel( e, type, icol, irow,ispec );
        return;
    }
    if( (!team_IsMember(e, recipient->db_id) && recipient->pl->declineGifts) || 
        (recipient->pl->declineTeamGifts && team_IsMember(e, recipient->db_id)) ) 
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"GiftFailedRecipientDeclines", recipient->name), INFO_USER_ERROR, 0);
        gift_cancel( e, type, icol, irow,ispec );
        return;
    }
    if( e->owner == id ) 
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"GiftFailedYourself"), INFO_USER_ERROR, 0);
        gift_cancel( e, type, icol, irow,ispec );
        return;
    }
    if( isIgnored( recipient, e->db_id ) )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"GiftFailedIgnored", recipient->name), INFO_USER_ERROR, 0);
        gift_cancel( e, type, icol, irow,ispec );
        return;
    }
    if( distance3Squared(ENTPOS(e), ENTPOS(recipient)) > SQR(20) )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"GiftFailedTooFarAway", recipient->name ), INFO_USER_ERROR, 0);
        gift_cancel( e, type, icol, irow,ispec );
        return;
    }
    if( recipient->pl->isRespeccing )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"GiftFailedRespec", recipient->name ), INFO_USER_ERROR, 0);
        gift_cancel( e, type, icol, irow,ispec );
        return;
    }

    if( !AccountCanTrade(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags) )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotTradeNoPermission"), INFO_USER_ERROR, 0);
        gift_cancel( e, type, icol, irow,ispec );
        return;
    }

    if( !AccountCanTrade(ent_GetProductInventory( recipient ), recipient->pl->loyaltyPointsEarned, recipient->pl->account_inventory.accountStatusFlags) )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotTradeWithFreePlayer"), INFO_USER_ERROR, 0);
        gift_cancel( e, type, icol, irow,ispec );
        return;
    }


    // you're allowed to gift inspirations to anyone who cons friendly, ignoring alignment
    if (character_TargetIsFoe(e->pchar,recipient->pchar))
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CantGiftEnemy", recipient->name ), INFO_USER_ERROR, 0);
        gift_cancel( e, type, icol, irow,ispec );
        return;
    }
    if( type == kTrayItemType_Inspiration )
        gift_giveInspiration( e, recipient, icol, irow );
    else if( type == kTrayItemType_SpecializationInventory )
        gift_giveEnhancement( e, recipient, ispec );
    else if( type == kTrayItemType_Power )
        gift_givePower( e, recipient, icol, irow );
    else if( type == kTrayItemType_Salvage )
        gift_giveSalvage( e, recipient, ispec );
    else if (type == kTrayItemType_Recipe)
        gift_giveRecipe( e, recipient, ispec );

}

