
#include "svr/svr_base.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "cmdparse/cmdserver.h"
#include "entity/powers.h"
#include "bases/DetailRecipe.h"
#include "trading.h"
#include "entity/entPlayer.h"
#include "entity/character_base.h"
#include "entity/character_net.h"
#include "entity/entVarUpdate.h"
#include "language/langServerUtil.h"
#include "dbcomm/dbcomm.h"            // for dbPlayerIdFromName
#include "comm_game.h"
#include "player/parseClientInput.h"
#include <utilitieslib/utils/timing.h>
#include "svr_chat.h"
#include "svr/svr_player.h"
#include "dbghelper.h"
#include "entity/character_combat.h"
#include "dbcomm/dbnamecache.h"
#include "gameSys/arenamap.h"
#include "entity/character_inventory.h"
#include "entity/character_target.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include "auth/authUserData.h"
#include <utilitieslib/utils/log.h>
#include "dbcomm/logcomm.h"
//---------------------------------------------------------------------------------

enum
{
    TRADE_NONE,
    TRADE_READY,
    TRADE_WAITING_FOR_SYNC,
};

#define SALV_TRADE_MAX 40 
typedef struct TradeObj
{
    int iCol;
    int iRow;

    int iLevel;
    int iNumCombines;

    const BasePower * ppow;
    const SalvageItem *sal;
    const DetailRecipe *pRec;

} TradeObj;

typedef struct TradeData
{
    int state;
    int partnerDbid;
    int influence;
    int accepted;
    int lastModTime;

    int iSpecNum;
    int iInspNum;
    int iSalvNum;
    int iTpowNum;
    int iReciNum;

    TradeObj spec[CHAR_BOOST_MAX];
    TradeObj insp[CHAR_INSP_MAX];
    TradeObj salv[SALV_TRADE_MAX];
    TradeObj tpow[SALV_TRADE_MAX];
    TradeObj reci[SALV_TRADE_MAX];

} TradeData;

bool power_IsMarkedForTrade( Entity * e, Power * pow )
{
    int i;

    if( !e || !e->pl || !e->pl->trade || !pow )
        return 0;

    for( i = 0; i < e->pl->trade->iTpowNum; i++ )
    {
        if( e->pl->trade->tpow[i].iCol == pow->psetParent->idx 
            && e->pl->trade->tpow[i].iRow == pow->idx )
        {
            return 1;
        }
    }
    return 0;
}

//---------------------------------------------------------------------------------

void trade_offer( Entity*e, char* player_name )
{
    char *buf;

    int        id = dbPlayerIdFromName(player_name);
    Entity    *ent = entFromDbId( id );

    if( ent )
    {

        // initial player does not have permission to trade
        if (!AccountCanTrade(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
        {
            chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotTradeNoPermission"), INFO_USER_ERROR, 0);
            return;
        }

        // target player does not have permission to trade
        if (!AccountCanTrade(&ent->pl->account_inventory, ent->pl->loyaltyPointsEarned, ent->pl->account_inventory.accountStatusFlags))
        {
            chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotTradeWithFreePlayer"), INFO_USER_ERROR, 0);
            return;
        }

        // player has ignore trades set
        if (ent->pl->declineTradeInvite)
        {
            chatSendToPlayer(e->db_id, localizedPrintf(e,"PlayerIsAutoDecliningTrade"), INFO_USER_ERROR, 0);
            return;
        }
 
        // if player is already trading, forget it
          if( ent->pl->trade && ent->pl->trade->state )
        {
            chatSendToPlayer( e->db_id, localizedPrintf(e,"CouldNotActionPlayerReason", "InviteTradeString", player_name, "PlayerIsAlreadyTrading" ), INFO_USER_ERROR, 0 );
            return;
        }

        if( e->pl->trade && e->pl->trade->state )
        {
            chatSendToPlayer( ent->db_id, localizedPrintf(ent,"CouldNotActionPlayerReason", "InviteTradeString", e->name, "PlayerIsAlreadyTrading" ), INFO_USER_ERROR, 0 );
            return;
        }

        if( OnArenaMap() )
        {
            chatSendToPlayer( ent->db_id, localizedPrintf(ent,"CouldNotActionPlayerReason", "InviteTradeString", e->name, "PlayerIsInArena" ), INFO_USER_ERROR, 0 );
            return;
        }

        if( isIgnored( ent, e->db_id ) )
        {
            char *buf = localizedPrintf( e,"CouldNotActionPlayerReason", "InviteTradeString", player_name, "PlayerHasIgnoredYou" );
            chatSendToPlayer( e->db_id, buf , INFO_USER_ERROR, 0 );
            return;
        }

        if( character_TargetIsFoe(e->pchar,ent->pchar) )
        {
            chatSendToPlayer( e->db_id, localizedPrintf(e,"CouldNotActionPlayerReason", "InviteTradeString", player_name, "CantTradeEnemy" ), INFO_USER_ERROR, 0 );
            return;
        }

        // already being invited to another team
        if( ent->pl->inviter_dbid )
        {
            buf = localizedPrintf(e,"CouldNotActionPlayerReason", "InviteTradeString", player_name, "PlayerIsConsideringAnotherOffer" );
            chatSendToPlayer(e->db_id, buf, INFO_USER_ERROR, 0);
            return;
        }
        else if ( ent->pl->last_inviter_dbid == e->db_id && (dbSecondsSince2000()-ent->pl->last_invite_time) < BLOCK_INVITE_TIME  )
        {
            buf = localizedPrintf( ent, "CouldNotActionPlayerReason", "InviteString", player_name, "MustWaitLonger" );
            chatSendToPlayer(e->db_id, buf, INFO_USER_ERROR, 0);
            return;
        }

        if( e->db_id == id )
        {
            buf = localizedPrintf(e,"CannotTradeWithSelf");
            chatSendToPlayer(e->db_id, buf, INFO_USER_ERROR, 0);
            return;
        }

        if( distance3Squared(ENTPOS(e), ENTPOS(ent)) > SQR(20) )
        {
            buf = localizedPrintf(e,"CouldNotActionPlayerReason", "InviteTradeString", player_name, "PlayerIsTooFarAway" );
            chatSendToPlayer(e->db_id, buf, INFO_USER_ERROR, 0);
            return;
        }

        ent->pl->inviter_dbid = e->db_id; // remeber the inviter, so we can trap bad commands

        {
            char *buf = localizedPrintf( e,"TradeInviteSent" );
            chatSendToPlayer( e->db_id, buf , INFO_SVR_COM, 0 );
        }

        START_PACKET( pak, ent, SERVER_TRADE_OFFER )
            pktSendBits( pak, 32, e->db_id );
            pktSendString( pak, e->name );
        END_PACKET
    }
    else // counldn't find player
    {
        buf = localizedPrintf(e, "CouldNotActionPlayerReason", "InviteTradeString", player_name, "PlayerNotInZone" );
        chatSendToPlayer(e->db_id, buf, INFO_USER_ERROR, 0);
    }
}

//---------------------------------------------------------------------------------

// This function begines trading between to players
void trade_init( Entity *playerA, Entity *playerB )
{
    // first make sure they have valid trading structs
    if( !playerA->pl->trade )
        playerA->pl->trade = calloc(sizeof(TradeData), 1);

    if( !playerB->pl->trade )
        playerB->pl->trade = calloc(sizeof(TradeData), 1);

    // make sure they know who each other are
    playerA->pl->trade->partnerDbid = playerB->db_id;
    playerB->pl->trade->partnerDbid = playerA->db_id;

    // mark them as ready
    playerA->pl->trade->state = playerB->pl->trade->state = TRADE_READY;

    // send a back to both of them to bring up the trading window
    START_PACKET( pak, playerA, SERVER_TRADE_INIT )
        pktSendBitsPack( pak, 1, playerB->db_id );
    END_PACKET

    START_PACKET( pak, playerB, SERVER_TRADE_INIT )
        pktSendBitsPack( pak, 1, playerA->db_id );
    END_PACKET

}

// This will wipe out a trade structure
static void trade_clear( Entity * e )
{
    if( e->pl && e->pl->trade )
        free( e->pl->trade );

    e->pl->trade = NULL;
}

//---------------------------------------------------------------------------------

// if the trade is aborted for any reason it will pass through this function
void trade_cancel( Entity* e, TradeFail reason )
{
    Entity *partner;

    if( !e || !e->pl || !e->pl->trade ) // not much I can do in this case :/
        return;

    partner = entFromDbId( e->pl->trade->partnerDbid );

    // wipe out the trade for valid ents
    trade_clear( e );
    if( partner )
        trade_clear( partner );

    // send a message letting them know why the trade was canceled
    switch( reason )
    {
        case TRADE_FAIL_OTHER_LEFT: // the opposite entity was detected missing for some reason
            {
                START_PACKET( pak, e, SERVER_TRADE_CANCEL )
                    pktSendString( pak, localizedPrintf(e,"TradeFailPartnerLeft") );
                END_PACKET

                if( partner )
                {
                    START_PACKET( pak, partner, SERVER_TRADE_CANCEL )
                        pktSendString( pak, localizedPrintf(partner,"TradeFailPartnerLostConn", e->name ) );
                    END_PACKET
                }
            }break;

        case TRADE_FAIL_I_LEFT: // the entity that sent this message (e) left the trade
            {
                START_PACKET( pak, e, SERVER_TRADE_CANCEL )
                    pktSendString( pak, localizedPrintf(e,"TradeFailYouLeft") );
                END_PACKET

                if( partner )
                {
                    START_PACKET( pak, partner, SERVER_TRADE_CANCEL )
                        pktSendString( pak, localizedPrintf(partner,"TradeFailPartnerLeft") );
                    END_PACKET
                }
            }break;

        case TRADE_FAIL_CORRUPT: // something crazy happened, send a generic message
            {
                START_PACKET( pak, e, SERVER_TRADE_CANCEL )
                    pktSendString( pak, localizedPrintf(e,"TradeFailed") );
                END_PACKET

                    if( partner )
                    {
                        START_PACKET( pak, partner, SERVER_TRADE_CANCEL )
                            pktSendString( pak, localizedPrintf(partner,"TradeFailed") );
                        END_PACKET
                    }
            } break;

        case TRADE_FAIL_INVENTORY: // not enough inventory room for trading
            {
                START_PACKET( pak, e, SERVER_TRADE_CANCEL )
                    pktSendString( pak, localizedPrintf(e,"TradeFailedInventory") );
                END_PACKET

                    if( partner )
                    {
                        START_PACKET( pak, partner, SERVER_TRADE_CANCEL )
                            pktSendString( pak, localizedPrintf(partner,"TradeFailedInventory") );
                        END_PACKET
                    }
            } break;
        case TRADE_FAIL_DISTANCE: // partners got too far away
            {
                START_PACKET( pak, e, SERVER_TRADE_CANCEL )
                    pktSendString( pak, localizedPrintf(e,"TradeFailedDistance") );
                END_PACKET

                    if( partner )
                    {
                        START_PACKET( pak, partner, SERVER_TRADE_CANCEL )
                            pktSendString( pak, localizedPrintf(partner,"TradeFailedDistance") );
                        END_PACKET
                    }
            } break;
    }
}

//---------------------------------------------------------------------------------

static void trade_sendUpdate( Entity *e, Entity * partner )
{
    TradeData *tradeA = e->pl->trade;
    TradeData *tradeB = partner->pl->trade;
    int i;

    START_PACKET( pak, e, SERVER_TRADE_UPDATE )

    pktSendBitsPack( pak, 1, tradeA->partnerDbid  );
    pktSendBitsPack( pak, 1, tradeA->accepted );
    pktSendBitsPack( pak, 1, tradeB->accepted );
    pktSendBitsPack( pak, 1, tradeA->influence );
    pktSendBitsPack( pak, 1, tradeB->influence );
    pktSendBitsPack( pak, 1, tradeB->iSpecNum );
    pktSendBitsPack( pak, 1, tradeB->iInspNum );
    pktSendBitsPack( pak, 1, tradeB->iSalvNum );
    pktSendBitsPack( pak, 1, tradeB->iTpowNum );
    pktSendBitsPack( pak, 1, tradeB->iReciNum );

    for( i = 0; i < tradeB->iSpecNum; i++ )
    {
        // check for changes later
        basepower_Send(pak, tradeB->spec[i].ppow );
        pktSendBitsPack(pak, 5, tradeB->spec[i].iLevel);
//        pktSendBitsPack(pak, 2, tradeB->spec[i].iNumCombines);
    }

    for( i = 0; i < tradeB->iInspNum; i++ )
    {
        basepower_Send(pak, tradeB->insp[i].ppow );
    }

    for( i = 0; i < tradeB->iSalvNum; i++ )
    {
        pktSendBitsPack(pak,4,tradeB->salv[i].sal->salId);
        pktSendBitsPack(pak,1,tradeB->salv[i].iNumCombines);
    }

    for( i = 0; i < tradeB->iReciNum; i++ )
    {
        pktSendBitsPack(pak,6,tradeB->reci[i].pRec->id);
        pktSendBitsPack(pak,1,tradeB->reci[i].iNumCombines);
    }

    for( i = 0; i < tradeB->iTpowNum; i++ )
    {
        Power * pow = partner->pchar->ppPowerSets[tradeB->tpow[i].iCol]->ppPowers[tradeB->tpow[i].iRow];
        basepower_Send(pak, tradeB->tpow[i].ppow );
        pktSendBitsPack(pak, 1, tradeB->tpow[i].iNumCombines );
        pktSendF32(pak, pow->fUsageTime);
        pktSendF32(pak, pow->fAvailableTime);
        pktSendBits(pak, 32, pow->ulCreationTime );
    }

    END_PACKET
}

//---------------------------------------------------------------------------------
//
// check for duplicate inspirations in the trade

int trade_checkDuplicates( TradeData *trade, int inspCount, int specCount )
{
    int i, j;

    // check player inspirations
    for( i = 0; i < inspCount; i++ )
    {
        for( j = i+1; j < inspCount; j++ )
        {
            if( trade->insp[i].iCol == trade->insp[j].iCol &&
                trade->insp[i].iRow == trade->insp[j].iRow )
            {
                return TRUE;
            }
        }
    }

    //check player A specializations
    for( i = 0; i <specCount; i++ )
    {
        for( j = i+1; j < specCount; j++ )
        {
            if( trade->spec[i].iCol == trade->spec[j].iCol )
                return TRUE;
        }
    }

    return FALSE;
}

//---------------------------------------------------------------------------------
//
//
void trade_update( Entity *e, Packet *pak )
{
    // consume all data first
    //--------------------------------------------------
    Entity *partner;
    int i;

    int partnerID = pktGetBitsPack( pak, 1 );
    int accepted = pktGetBitsPack( pak, 1 );
    int influence = pktGetBitsPack( pak, 1 );

    int specCount = pktGetBitsPack( pak, 1 );
    int inspCount = pktGetBitsPack( pak, 1 );
    int salvCount = pktGetBitsPack( pak, 1 );
    int tpowCount = pktGetBitsPack( pak, 1 );
    int reciCount = pktGetBitsPack( pak, 1 );

    TradeObj spec[CHAR_BOOST_MAX];
    TradeObj insp[CHAR_INSP_MAX];
    TradeObj salv[SALV_TRADE_MAX];
    TradeObj tpow[SALV_TRADE_MAX];
    TradeObj reci[SALV_TRADE_MAX];

    for( i = 0; i < specCount; i++ )
        spec[i].iCol = pktGetBitsPack(pak, 1 );

    for( i = 0; i < inspCount; i++ )
    {
        insp[i].iCol = pktGetBitsPack(pak, 1 );
        insp[i].iRow = pktGetBitsPack(pak, 1 );
    }

    for( i = 0; i < salvCount; i++ )
    {
        salv[i].iCol = pktGetBitsPack(pak, 1 );
        salv[i].iRow = pktGetBitsPack(pak, 1 );
        salv[i].iNumCombines = pktGetBitsPack(pak,1);
    }

    for( i = 0; i < reciCount; i++ )
    {
        reci[i].iCol = pktGetBitsPack(pak, 1 );
        reci[i].iNumCombines = pktGetBitsPack(pak,1);
    }

    for( i = 0; i < tpowCount; i++ )
    {
        tpow[i].iCol = pktGetBitsPack(pak, 1 );
        tpow[i].iRow = pktGetBitsPack(pak, 1 );
    }

    // check critical factors
    //--------------------------------------------------
    partner = entFromDbId( partnerID );

    // if there is no entity, let the partner know...not sure how this could happen, but being redundantly careful
    if( (!e || !e->pl || !e->pl->trade ) )
    {
        if ( partner )
        {
            trade_cancel( partner, TRADE_FAIL_OTHER_LEFT );
            return;
        }
    }

    // partner is MIA, cancel trade
    if( !partner || !partner->pl || !partner->pl->trade )
    {
        trade_cancel( e, TRADE_FAIL_OTHER_LEFT );
        return;
    }

    // somehow there is disagreement about who the trading partners are, cancel trade
    if( e->pl->trade->partnerDbid != partnerID ||
        partner->pl->trade->partnerDbid != e->db_id )
    {
        trade_cancel( e, TRADE_FAIL_CORRUPT );
        trade_cancel( partner, TRADE_FAIL_CORRUPT );
        return;
    }

    // check influence
    //--------------------------------------------------
    e->pl->trade->accepted = accepted;

    // did influence change?
    if( e->pl->trade->influence != influence )
    {
        // reset accepts
        e->pl->trade->accepted = 0;
        partner->pl->trade->accepted = 0;

        if( influence < 0 )
        {
            trade_cancel( e, TRADE_FAIL_CORRUPT );
            trade_cancel( partner, TRADE_FAIL_CORRUPT );
            return;
        }

        if( influence > e->pchar->iInfluencePoints )
        {
            // Mark player as BAD
            e->pl->trade->influence = e->pchar->iInfluencePoints;
        }
        else
            e->pl->trade->influence = influence ;
    }

    // find specialization base powers and check for changes
    //--------------------------------------------------
    for( i = 0; i < specCount; i++ )
    {
        // make sure character actually has something in slot he wants to trade

        if( spec[i].iCol >= 0 && spec[i].iCol < CHAR_BOOST_MAX )
        {
            if (e->pchar->aBoosts[spec[i].iCol] && e->pchar->aBoosts[spec[i].iCol]->ppowBase )
            {
                const BasePower *ppow = e->pchar->aBoosts[spec[i].iCol]->ppowBase;
                if (!detailrecipedict_IsBoostTradeable(ppow, e->pchar->aBoosts[spec[i].iCol]->iLevel, e->auth_name, ""))
                {
                    //    enhancement isn't tradeable (because recipe isn't)
                    //    fall through to trade cancel
                }
                else
                {
                    e->pl->trade->spec[i].iCol = spec[i].iCol;

                    // check for a change
                    if( !e->pl->trade->spec[i].ppow
                        || e->pchar->aBoosts[spec[i].iCol]->ppowBase != e->pl->trade->spec[i].ppow
                        || e->pchar->aBoosts[spec[i].iCol]->iLevel != e->pl->trade->spec[i].iLevel
                        || e->pchar->aBoosts[spec[i].iCol]->iNumCombines != e->pl->trade->spec[i].iNumCombines)
                    {
                        // reset accepts
                        e->pl->trade->accepted = 0;
                        partner->pl->trade->accepted = 0;
                    }
 
                    e->pl->trade->spec[i].ppow = e->pchar->aBoosts[spec[i].iCol]->ppowBase;
                    e->pl->trade->spec[i].iLevel = e->pchar->aBoosts[spec[i].iCol]->iLevel;
                     e->pl->trade->spec[i].iNumCombines = e->pchar->aBoosts[spec[i].iCol]->iNumCombines;
                    continue;        //    everything is A-okay, so don't fall through to trade cancel
                }
            }
        }

        // something fishy going on, Mark player as BAD
        trade_cancel( e, TRADE_FAIL_CORRUPT );
        trade_cancel( partner, TRADE_FAIL_CORRUPT );
        return;
    }

    // find inspiration base powers and check for changes
    //--------------------------------------------------
     for( i = 0; i < inspCount; i++ )
    {
        // make sure character actually has something in slot he wants to trade
        if( insp[i].iCol >= 0 && insp[i].iCol < e->pchar->iNumInspirationCols &&
            insp[i].iRow >= 0 && insp[i].iRow < e->pchar->iNumInspirationRows &&
            e->pchar->aInspirations[insp[i].iCol][insp[i].iRow] &&
            inspiration_IsTradeable(e->pchar->aInspirations[insp[i].iCol][insp[i].iRow], e->auth_name, ""))
        {
            e->pl->trade->insp[i].iCol = insp[i].iCol;
            e->pl->trade->insp[i].iRow = insp[i].iRow;

            // check for a change
            if( !e->pl->trade->insp[i].ppow || e->pchar->aInspirations[insp[i].iCol][insp[i].iRow] != e->pl->trade->insp[i].ppow )
            {
                // reset accepts
                e->pl->trade->accepted = 0;
                partner->pl->trade->accepted = 0;
            }

            e->pl->trade->insp[i].ppow = e->pchar->aInspirations[insp[i].iCol][insp[i].iRow];
        }
        else
        {
            // something fishy going on, Mark player as BAD
            trade_cancel( e, TRADE_FAIL_CORRUPT );
            trade_cancel( partner, TRADE_FAIL_CORRUPT );
            return;
        }
    }

    // find temporary base powers and check for changes
    //--------------------------------------------------
    for( i = 0; i < tpowCount; i++ )
    {
        // make sure character actually has something in slot he wants to trade
        if( tpow[i].iCol >= 0 && tpow[i].iCol < eaSize(&e->pchar->ppPowerSets) &&
            tpow[i].iRow >= 0 && tpow[i].iRow < eaSize(&e->pchar->ppPowerSets[tpow[i].iCol]->ppPowers) )
        {
            e->pl->trade->tpow[i].iCol = tpow[i].iCol;
            e->pl->trade->tpow[i].iRow = tpow[i].iRow;

            // check for a change
            if( !e->pl->trade->tpow[i].ppow || 
                e->pchar->ppPowerSets[tpow[i].iCol]->ppPowers[tpow[i].iRow]->ppowBase != e->pl->trade->tpow[i].ppow )
            {
                // reset accepts
                e->pl->trade->accepted = 0;
                partner->pl->trade->accepted = 0;
            }

            e->pl->trade->tpow[i].ppow = e->pchar->ppPowerSets[tpow[i].iCol]->ppPowers[tpow[i].iRow]->ppowBase;
        }
        else
        {
            // something fishy going on, Mark player as BAD
            trade_cancel( e, TRADE_FAIL_CORRUPT );
            trade_cancel( partner, TRADE_FAIL_CORRUPT );
            return;
        }
    }

    // check salvage
    for( i = 0; i < salvCount; i++ )
    {
        // make sure character actually has something in slot he wants to trade
        // @todo -AB: fit with new flat salvage system :08/16/05
        if( e->pchar->salvageInv && 
            salv[i].iCol >= 0 && salv[i].iCol < eaSize(&e->pchar->salvageInv) &&
            e->pchar->salvageInv[salv[i].iCol] &&
            e->pchar->salvageInv[salv[i].iCol]->salvage &&
            !(e->pchar->salvageInv[salv[i].iCol]->salvage->flags & SALVAGE_NOTRADE))
        {
            e->pl->trade->salv[i].iCol = salv[i].iCol;
            e->pl->trade->salv[i].iRow = salv[i].iRow;

            // check for a change
            if( !e->pl->trade->salv[i].sal || 
                e->pchar->salvageInv[salv[i].iCol]->salvage != e->pl->trade->salv[i].sal ||
                e->pl->trade->salv[i].iNumCombines != salv[i].iNumCombines )
            {
                // reset accepts
                e->pl->trade->accepted = 0;
                partner->pl->trade->accepted = 0;
            }
        
            e->pl->trade->salv[i].iNumCombines = salv[i].iNumCombines;
            e->pl->trade->salv[i].sal = e->pchar->salvageInv[salv[i].iCol]->salvage;
        }
        else
        {
            // something fishy going on, Mark player as BAD
            trade_cancel( e, TRADE_FAIL_CORRUPT );
            trade_cancel( partner, TRADE_FAIL_CORRUPT );
            return;
        }
    }

    // check recipes
    for( i = 0; i < reciCount; i++ )
    {
        const DetailRecipe *pRec = recipe_GetItemById(reci[i].iCol);
        int recipeCount = character_RecipeCount(e->pchar, pRec);

        // make sure character actually has something in slot he wants to trade
        if( pRec && recipeCount > 0 && !(pRec->flags & RECIPE_NO_TRADE) )
        {
            e->pl->trade->reci[i].iCol = reci[i].iCol;

            // check for a change
            if( !e->pl->trade->reci[i].pRec || 
                pRec != e->pl->trade->reci[i].pRec ||
                e->pl->trade->reci[i].iNumCombines != reci[i].iNumCombines )
            {
                // reset accepts
                e->pl->trade->accepted = 0;
                partner->pl->trade->accepted = 0;
            }

            e->pl->trade->reci[i].iNumCombines = reci[i].iNumCombines;
            e->pl->trade->reci[i].pRec = pRec;
        }
        else
        {
            // something fishy going on, Mark player as BAD
            trade_cancel( e, TRADE_FAIL_CORRUPT );
            trade_cancel( partner, TRADE_FAIL_CORRUPT );
            return;
        }
    }



    if( trade_checkDuplicates( e->pl->trade, inspCount, specCount ) )
    {
        // something fishy going on, Mark player as BAD
        trade_cancel( e, TRADE_FAIL_CORRUPT );
        trade_cancel( partner, TRADE_FAIL_CORRUPT );
        return;
    }

    if( e->pl->trade->iSpecNum != specCount || 
        e->pl->trade->iInspNum != inspCount || 
        e->pl->trade->iSalvNum != salvCount ||
        e->pl->trade->iTpowNum != tpowCount ||
        e->pl->trade->iReciNum != reciCount)
    {
        // reset accepts
        e->pl->trade->accepted = 0;
        partner->pl->trade->accepted = 0;
    }
    e->pl->trade->iSpecNum = specCount;
    e->pl->trade->iInspNum = inspCount;
    e->pl->trade->iSalvNum = salvCount;
    e->pl->trade->iTpowNum = tpowCount;
    e->pl->trade->iReciNum = reciCount;

    // now send the updated data to both parties
    //--------------------------------------------------
    trade_sendUpdate( e, partner );
    trade_sendUpdate( partner, e );

    if( e->pl->trade->accepted && partner->pl->trade->accepted )
        e->pl->trade->state = partner->pl->trade->state = TRADE_WAITING_FOR_SYNC;

    // record the time
    e->pl->trade->lastModTime = timerSecondsSince2000();
}

static void trade_sucess( Entity *a, Entity *b )
{
    char *bufferGiveA = NULL;
    char *bufferGiveB = NULL;
    char *bufferReceiveA = NULL;
    char *bufferReceiveB = NULL;
    TradeData *ta = a->pl->trade;
    TradeData *tb = b->pl->trade;
    Entity *ent_array[2] = { a, b };
    int i;
    int len;

    estrCreate(&bufferGiveA);
    estrCreate(&bufferGiveB);
    estrCreate(&bufferReceiveA);
    estrCreate(&bufferReceiveB);

    ////////////////////////////////////////////////////////////////////////////
    // trade was successful, first deal with influence - Caps should be handled in trade_finalize
    ent_AdjInfluence(a, (int) (tb->influence - ta->influence), "trade");
    ent_AdjInfluence(b, (int) (ta->influence - tb->influence), "trade");

    if( ta->influence )
    {
        estrConcatCharString(&bufferGiveA, localizedPrintf( a,"InfluencePts", ta->influence) );
        estrConcatCharString(&bufferReceiveB, localizedPrintf( b,"InfluencePts", ta->influence) );
        LOG_ENT( b, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[TR]:Rcv:Inf %d (%d) from %s", ta->influence, b->pchar->iInfluencePoints, a->name );
    }
    if( tb->influence )
    {
        estrConcatCharString(&bufferGiveB, localizedPrintf( b,"InfluencePts", tb->influence) );
        estrConcatCharString(&bufferReceiveA, localizedPrintf( a,"InfluencePts", tb->influence) );
        LOG_ENT( a, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[TR]:Rcv:Inf %d (%d) from %s", tb->influence, a->pchar->iInfluencePoints, b->name);
    }

    ////////////////////////////////////////////////////////////////////////////
    //now specializations
    for( i = 0; i < ta->iSpecNum; i++ ) // remove traded specs
    {
        character_DestroyBoost( a->pchar, ta->spec[i].iCol, "trade" );
        estrConcatCharString(&bufferGiveA, localizedPrintf( a,"PowerComma", ta->spec[i].ppow->pchDisplayName ) );
    }
    for( i = 0; i < tb->iSpecNum; i++ ) // grant traded specs
    {
        character_AddBoost( a->pchar, tb->spec[i].ppow, tb->spec[i].iLevel, 0, "trade");
        estrConcatCharString(&bufferReceiveA, localizedPrintf( a,"PowerComma", tb->spec[i].ppow->pchDisplayName ) );
        LOG_ENT( a, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[TR]:Rcv:Enh %s %d", dbg_BasePowerStr(tb->spec[i].ppow), tb->spec[i].iLevel);
    }

    for( i = 0; i < tb->iSpecNum; i++ ) // remove traded specs
    {
        character_DestroyBoost( b->pchar, tb->spec[i].iCol, "trade" );
        estrConcatCharString(&bufferGiveB, localizedPrintf( b,"PowerComma", tb->spec[i].ppow->pchDisplayName ) );
    }
    for( i = 0; i < ta->iSpecNum; i++ ) // grant traded specs
    {
        character_AddBoost( b->pchar, ta->spec[i].ppow, ta->spec[i].iLevel, 0, "trade");
        estrConcatCharString(&bufferReceiveB, localizedPrintf( b,"PowerComma", ta->spec[i].ppow->pchDisplayName ) );
        LOG_ENT( b, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[TR]:Rcv:Enh %s %d", dbg_BasePowerStr(ta->spec[i].ppow), ta->spec[i].iLevel);
    }

    ////////////////////////////////////////////////////////////////////////////
    // now inspirations
    for( i = 0; i < ta->iInspNum; i++ ) // remove traded inspiration
    {
        character_RemoveInspiration( a->pchar, ta->insp[i].iCol, ta->insp[i].iRow, "trade" );
        estrConcatCharString(&bufferGiveA, localizedPrintf( a,"PowerComma", ta->insp[i].ppow->pchDisplayName ) );
    }
    for( i = 0; i < tb->iInspNum; i++ ) // grant traded inspiration
    {
        character_AddInspiration( a->pchar, tb->insp[i].ppow, "trade" );
        estrConcatCharString(&bufferReceiveA, localizedPrintf( a,"PowerComma", tb->insp[i].ppow->pchDisplayName ) );
        LOG_ENT( a, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[TR]:Rcv:Insp %s", dbg_BasePowerStr(tb->insp[i].ppow));
    }

    for( i = 0; i < tb->iInspNum; i++ ) // remove traded inspirations
    {
        character_RemoveInspiration( b->pchar, tb->insp[i].iCol, tb->insp[i].iRow, "trade" );
        estrConcatCharString(&bufferGiveB, localizedPrintf( b,"PowerComma", tb->insp[i].ppow->pchDisplayName ) );
    }
    for( i = 0; i < ta->iInspNum; i++ ) // grant traded inspiration
    {
        character_AddInspiration( b->pchar, ta->insp[i].ppow, "trade" );
        estrConcatCharString(&bufferReceiveB, localizedPrintf( b,"PowerComma", ta->insp[i].ppow->pchDisplayName ) );
        LOG_ENT( b, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[TR]:Rcv:Insp %s", dbg_BasePowerStr(ta->insp[i].ppow));
    }

    ////////////////////////////////////////////////////////////////////////////
    // Salvage
    for( i = 0; i < ta->iSalvNum; i++ ) // remove traded salvage
    {
        character_AdjustSalvage( a->pchar, ta->salv[i].sal, -1 * ta->salv[i].iNumCombines, "trade", false);
        estrConcatCharString(&bufferGiveA, localizedPrintf( a,"SalvageComma", ta->salv[i].iNumCombines, ta->salv[i].sal->ui.pchDisplayName ) );
    }
    for( i = 0; i < tb->iSalvNum; i++ ) // grant traded salvage
    {
        character_AdjustSalvage( a->pchar, tb->salv[i].sal, 1 * tb->salv[i].iNumCombines, "trade", false);
        estrConcatCharString(&bufferReceiveA, localizedPrintf( a,"SalvageComma", tb->salv[i].iNumCombines, tb->salv[i].sal->ui.pchDisplayName) );
        LOG_ENT( a, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[TR]:Rcv:Sal %d %s", tb->salv[i].iNumCombines, tb->salv[i].sal->pchName);
    }

    for( i = 0; i < tb->iSalvNum; i++ ) // remove traded salvage
    {
        character_AdjustSalvage( b->pchar, tb->salv[i].sal, -1 * tb->salv[i].iNumCombines, "trade", false);
        estrConcatCharString(&bufferGiveB, localizedPrintf( b,"SalvageComma", tb->salv[i].iNumCombines, tb->salv[i].sal->ui.pchDisplayName ) );
    }
    for( i = 0; i < ta->iSalvNum; i++ ) // grant traded salvage
    {
        character_AdjustSalvage( b->pchar, ta->salv[i].sal, 1 * ta->salv[i].iNumCombines, "trade", false);
        estrConcatCharString(&bufferReceiveB, localizedPrintf( b,"SalvageComma", ta->salv[i].iNumCombines, ta->salv[i].sal->ui.pchDisplayName ) );
        LOG_ENT( b, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[TR]:Rcv:Sal %d %s", ta->salv[i].iNumCombines, ta->salv[i].sal->pchName);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Recipes!!!!!!!!!!!!!
    for( i = 0; i < ta->iReciNum; i++ ) // remove traded recipes
    {
        character_AdjustRecipe( a->pchar, ta->reci[i].pRec, -1 * ta->reci[i].iNumCombines, "trade");
        estrConcatCharString(&bufferGiveA, localizedPrintf( a,"RecipeComma", ta->reci[i].iNumCombines, ta->reci[i].pRec->ui.pchDisplayName ) );
    }
    for( i = 0; i < tb->iReciNum; i++ ) // grant traded recipes
    {
        character_AdjustRecipe( a->pchar, tb->reci[i].pRec, 1 * tb->reci[i].iNumCombines, "trade");
        estrConcatCharString(&bufferReceiveA, localizedPrintf( a,"RecipeComma", tb->reci[i].iNumCombines, tb->reci[i].pRec->ui.pchDisplayName) );
        LOG_ENT( a, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[TR]:Rcv:Rec %d %s", tb->reci[i].iNumCombines, tb->reci[i].pRec->pchName);
    }

    for( i = 0; i < tb->iReciNum; i++ ) // remove traded recipes
    {
        character_AdjustRecipe( b->pchar, tb->reci[i].pRec, -1 * tb->reci[i].iNumCombines, "trade");
        estrConcatCharString(&bufferGiveB, localizedPrintf( b,"RecipeComma", tb->reci[i].iNumCombines, tb->reci[i].pRec->ui.pchDisplayName ) );
    }
    for( i = 0; i < ta->iReciNum; i++ ) // grant traded recipes
    {
        character_AdjustRecipe( b->pchar, ta->reci[i].pRec, 1 * ta->reci[i].iNumCombines, "trade");
        estrConcatCharString(&bufferReceiveB, localizedPrintf( b, "RecipeComma", ta->reci[i].iNumCombines, ta->reci[i].pRec->ui.pchDisplayName ) );
        LOG_ENT( b, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[TR]:Rcv:Rec %d %s", ta->reci[i].iNumCombines, ta->reci[i].pRec->pchName);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Temporary Powers (Need to grant powers first, in case the are deleted in remove step)
    for( i = 0; i < tb->iTpowNum; i++ ) // grant traded power
    {
        Power * pow = b->pchar->ppPowerSets[tb->tpow[i].iCol]->ppPowers[tb->tpow[i].iRow];
        character_AddTemporaryPower( a->pchar, pow );
        estrConcatCharString(&bufferReceiveA, localizedPrintf( a, "PowerComma", tb->tpow[i].ppow->pchDisplayName ) );
        LOG_ENT( a, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[TR]:Rcv:Tpow %s", dbg_BasePowerStr(tb->tpow[i].ppow));
    }
    for( i = 0; i < ta->iTpowNum; i++ ) // grant traded power
    {
        Power * pow = a->pchar->ppPowerSets[ta->tpow[i].iCol]->ppPowers[ta->tpow[i].iRow];
        character_AddTemporaryPower( b->pchar, pow );
        estrConcatCharString(&bufferReceiveB, localizedPrintf( b,"PowerComma", ta->tpow[i].ppow->pchDisplayName ) );
        LOG_ENT( b, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[TR]:Rcv:Tpow %s", dbg_BasePowerStr(ta->tpow[i].ppow));
    }

    for( i = 0; i < ta->iTpowNum; i++ ) // remove traded power
    {
        Power * pow = a->pchar->ppPowerSets[ta->tpow[i].iCol]->ppPowers[ta->tpow[i].iRow];
        character_RemovePower(a->pchar, pow);
        estrConcatCharString(&bufferGiveA, localizedPrintf( a,"PowerComma", ta->tpow[i].ppow->pchDisplayName ) );
    }

    for( i = 0; i < tb->iTpowNum; i++ ) // remove traded inspirations
    {
        Power * pow = b->pchar->ppPowerSets[tb->tpow[i].iCol]->ppPowers[tb->tpow[i].iRow];
        character_RemovePower(b->pchar, pow);
        estrConcatCharString(&bufferGiveB, localizedPrintf( b,"PowerComma", tb->tpow[i].ppow->pchDisplayName ) );
    }


    character_CompactInspirations(a->pchar);
    character_CompactInspirations(b->pchar);

    // ok trading part done so send them a chat msg explaining what happened
    if( (len = estrLength(&bufferGiveA)) == 0 )
        estrConcatCharString(&bufferGiveA, localizedPrintf(a,"NothingString") );
    else if (bufferGiveA[len-1] == ',')
        estrRemove(&bufferGiveA, len-1, 1); // get rid of very last comma

    if( (len = estrLength(&bufferGiveB)) == 0 )
        estrConcatCharString(&bufferGiveB, localizedPrintf(b,"NothingString") );
    else if (bufferGiveB[len-1] == ',')
        estrRemove(&bufferGiveB, len-1, 1); // get rid of very last comma

    if( (len = estrLength(&bufferReceiveA)) == 0 )
        estrConcatCharString(&bufferReceiveA, localizedPrintf(a,"NothingString") );
    else if (bufferReceiveA[len-1] == ',')
        estrRemove(&bufferReceiveA, len-1, 1); // get rid of very last comma

    if( (len = estrLength(&bufferReceiveB)) == 0 )
        estrConcatCharString(&bufferReceiveB, localizedPrintf(b,"NothingString") );
    else if (bufferReceiveB[len-1] == ',')
        estrRemove(&bufferReceiveB, len-1, 1); // get rid of very last comma

    START_PACKET( pak, a, SERVER_TRADE_SUCESS )
        pktSendString( pak, localizedPrintf(a,"TradeGaveRecieved", bufferGiveA, bufferReceiveA ) );
    END_PACKET

    START_PACKET( pak, b, SERVER_TRADE_SUCESS )
        pktSendString( pak, localizedPrintf(b,"TradeGaveRecieved", bufferGiveB, bufferReceiveB ) );
    END_PACKET

    // save character info to dbserver
    svrSendEntListToDb( ent_array, 2 );

    // lastly clean up
    trade_clear( a );
    trade_clear( b );
    estrDestroy(&bufferGiveA);
    estrDestroy(&bufferGiveB);
    estrDestroy(&bufferReceiveA);
    estrDestroy(&bufferReceiveB);

}

//
//
static void trade_fail( Entity *a, Entity *b )
{
    a->pl->trade->accepted = b->pl->trade->accepted = 0;
    a->pl->trade->state = b->pl->trade->state = TRADE_READY;
    trade_sendUpdate( a, b );
    trade_sendUpdate( b, a );
}

//
static void trade_salvagerecipe_cleanup(Character *a, Character *b)
{
    character_InventoryFree(a);
    character_InventoryFree(b);
    free(a);
    free(b);
}

static BOOL ent_validateSpecs(Entity *e)
{
    int i;
    TradeObj *spec;
    if(!e)
        return FALSE;
    spec = e->pl->trade->spec;
    for(i = 0; i < e->pl->trade->iSpecNum; ++i)
    {
        int j = spec[i].iCol;
        Boost *b;
        
        if(!INRANGE0(j,CHAR_BOOST_MAX))
            return FALSE;
        b = e->pchar->aBoosts[j];
        if(!b)
            return FALSE;
        
        if(b->ppowBase != spec[i].ppow
           || b->iLevel != spec[i].iLevel
           || b->iNumCombines != spec[i].iNumCombines)
            return FALSE;
    }
    return TRUE;
}


//
//
static void trade_finalize( Entity *a )
{
    // can't get here without checks to make sure entity and trade are valid
    //-------------------------------------------------------------------
    Entity *b = entFromDbId( a->pl->trade->partnerDbid );
     int i, j, eSpecRoom, partnerSpecRoom, eInspRoom, partnerInspRoom;
    TradeData *ta, *tb;
    int maxInf = 999999999;
    int max_inf_a = getEntMaxInf(a);
    int max_inf_b = getEntMaxInf(b);
    S64 infDelta = 0;

       if( !b ) 
    {
        trade_cancel( a, TRADE_FAIL_OTHER_LEFT );
        return;
    }

    // check for total craziness
    if ( !b->pl || !b->pl->trade || b->pl->trade->state != TRADE_WAITING_FOR_SYNC ||
          b->pl->trade->partnerDbid != a->db_id || b->db_id == a->db_id || b->pl->trade == a->pl->trade ||
          !AccountCanTrade(&a->pl->account_inventory, a->pl->loyaltyPointsEarned, a->pl->account_inventory.accountStatusFlags) || 
          !AccountCanTrade(&b->pl->account_inventory, b->pl->loyaltyPointsEarned, b->pl->account_inventory.accountStatusFlags))
    {
        trade_cancel( a, TRADE_FAIL_CORRUPT );
        trade_cancel( b, TRADE_FAIL_CORRUPT ); 
        return;
    }

    ta = a->pl->trade;
    tb = b->pl->trade;

    // now make sure they haven't spent their influence in the meantime
    //-------------------------------------------------------------------
    if( ta->influence > a->pchar->iInfluencePoints )
    {
        trade_cancel( a, TRADE_FAIL_CORRUPT );
        return;
    }

    if( tb->influence > b->pchar->iInfluencePoints )
    {
        trade_cancel( b, TRADE_FAIL_CORRUPT );
        return;
    }

    if( ta->influence > maxInf || ta->influence < 0 )
    {
        LOG_ENT( a, LOG_REWARDS, LOG_LEVEL_ALERT, 0, "[TR]:Cheat Tried to trade %i influence", ta->influence );
        trade_cancel( a, TRADE_FAIL_CORRUPT );
        return;
    }

    if( tb->influence > maxInf || tb->influence < 0 )
    {
        LOG_ENT( b, LOG_REWARDS, LOG_LEVEL_ALERT, 0, "[TR]:Cheat Tried to trade %i influence", tb->influence );
        trade_cancel( b, TRADE_FAIL_CORRUPT );
        return;
    }

    infDelta = (S64) (tb->influence - ta->influence);
    if (infDelta > 0 && !ent_canAddInfluence(a, (int) infDelta))
    {
        trade_fail( a, b );
        trade_cancel( a, TRADE_FAIL_INVENTORY );
        return;
    }

    infDelta = (S64) (ta->influence - tb->influence);
    if (infDelta > 0 && !ent_canAddInfluence(b, (int) infDelta))
    {
        trade_fail( a, b );
        trade_cancel( b, TRADE_FAIL_INVENTORY );
        return;
    }

    // now make sure both parties have enough room for specializations
    //-------------------------------------------------------------------
    eSpecRoom = ta->iSpecNum; // start out with the number or specs the person wants to give away
    eSpecRoom += character_BoostInventoryEmptySlots(a->pchar); // add on slots that are already empty

    // not enough room
    if( eSpecRoom < tb->iSpecNum )
    {
        trade_fail( a, b );
        trade_cancel( a, TRADE_FAIL_INVENTORY );
        return;
    }

    partnerSpecRoom = tb->iSpecNum; // start out with the number or specs the person wants to give away
    partnerSpecRoom += character_BoostInventoryEmptySlots(b->pchar); // add on slots that are already empty

    // not enough room
    if( partnerSpecRoom < ta->iSpecNum )
    {
        trade_fail( b, a );
        trade_cancel( b, TRADE_FAIL_INVENTORY );
        return;
    }

    // check that the source boost is valid
    if(!ent_validateSpecs(a))
    {
        trade_fail(a,b);
        trade_cancel(a,TRADE_FAIL_INVENTORY);
        return;
    }

    if(!ent_validateSpecs(b))
    {
        trade_fail(b,a);
        trade_cancel(b,TRADE_FAIL_INVENTORY);
        return;
    }

    // now make sure both parties have enough room for inspirations
    //-------------------------------------------------------------------
    eInspRoom = ta->iInspNum; // start out with the number or specs the person wants to give away
    for( i = 0; i < a->pchar->iNumInspirationCols; i++ ) // then count the remaining empty slots
    {
        for( j = 0; j < a->pchar->iNumInspirationRows; j++ )
        {
            if( !a->pchar->aInspirations[i][j] )
                eInspRoom++;
        }
    }

    // not enough rooom
    if( eInspRoom < tb->iInspNum )
    {
        trade_fail( a, b );
        trade_cancel( a, TRADE_FAIL_INVENTORY );
        return;
    }

    partnerInspRoom = tb->iInspNum; // start out with the number or specs the person wants to give away
    for( i = 0; i < b->pchar->iNumInspirationCols; i++ ) // then count the remaining empty slots
    {
        for( j = 0; j < b->pchar->iNumInspirationRows; j++ )
        {
            if( !b->pchar->aInspirations[i][j] )
                partnerInspRoom++;
        }
    }

    // not enough rooom
    if( partnerInspRoom < ta->iInspNum )
    {
        trade_fail( b, a );
        trade_cancel( b, TRADE_FAIL_INVENTORY );
        return;
    }

    // Make sure none of the inspirations are in use
    //-------------------------------------------------------------------
    for( i = 0; i < eInspRoom; i++ )
    {
        if( character_IsInspirationSlotInUse(a->pchar, ta->insp[i].iCol,  ta->insp[i].iRow ) )
        {
            trade_cancel( a, TRADE_FAIL_CORRUPT );
            return;
        }
    }

    for( i = 0; i < partnerInspRoom; i++ )
    {
        if( character_IsInspirationSlotInUse(b->pchar, tb->insp[i].iCol,  tb->insp[i].iRow ) )
        {
            trade_cancel( a, TRADE_FAIL_CORRUPT );
            return;
        }
    }

    // Make sure the inspirations are there
    //-------------------------------------------------------------------


    // copy characters to back ups to verify that their inventories can indeed be modified as expected
    {
        Character *pcharANew = calloc(1, sizeof(Character));
        Character *pcharBNew = calloc(1, sizeof(Character));

        memset(pcharANew, 0, sizeof(Character));
        memset(pcharBNew, 0, sizeof(Character));

        character_CopyInventory(a->pchar, pcharANew);
        character_CopyInventory(b->pchar, pcharBNew);

        // necessary for inventory computation (VIP check)
        pcharANew->entParent = a;
        pcharBNew->entParent = b;


        //////////////////////////////////////////////////////////////////////////
        // remove salvages
        for( i = 0; i < ta->iSalvNum; i++ ) // A salvage remove
        {
            if (ta->salv[i].sal->flags & SALVAGE_NOTRADE)
            {
                trade_fail( a, b );
                trade_cancel( a, TRADE_FAIL_CORRUPT );
                trade_salvagerecipe_cleanup(pcharANew, pcharBNew);
            }
            else if (!character_CanAdjustSalvage( pcharANew, ta->salv[i].sal, -1 * ta->salv[i].iNumCombines))
            {
                trade_fail( a, b );
                trade_cancel( a, TRADE_FAIL_INVENTORY );
                trade_salvagerecipe_cleanup(pcharANew, pcharBNew);
                return;
            } else {
                character_AdjustSalvage(pcharANew, ta->salv[i].sal, -1 * ta->salv[i].iNumCombines, NULL, true);
            }
        }
        for( i = 0; i < tb->iSalvNum; i++ ) // B salvage remove
        {
            if (tb->salv[i].sal->flags & SALVAGE_NOTRADE)
            {
                trade_fail( b, a );
                trade_cancel( b, TRADE_FAIL_CORRUPT );
                trade_salvagerecipe_cleanup(pcharANew, pcharBNew);
            }
            else if (!character_CanAdjustSalvage( pcharBNew, tb->salv[i].sal, -1 * tb->salv[i].iNumCombines))
            {
                trade_fail( b, a );
                trade_cancel( b, TRADE_FAIL_INVENTORY );
                trade_salvagerecipe_cleanup(pcharANew, pcharBNew);
                return;
            } else {
                character_AdjustSalvage(pcharBNew, tb->salv[i].sal, -1 * tb->salv[i].iNumCombines, NULL, true);
            }
        }

        // remove recipes
        for( i = 0; i < ta->iReciNum; i++ ) // A recipe remove
        {
            if (ta->reci[i].pRec->flags & RECIPE_NO_TRADE)
            {
                trade_fail( a, b );
                trade_cancel( a, TRADE_FAIL_CORRUPT );
                trade_salvagerecipe_cleanup(pcharANew, pcharBNew);
            }
            else if (!character_CanRecipeBeChanged( pcharANew, ta->reci[i].pRec, -1 * ta->reci[i].iNumCombines))
            {
                trade_fail( a, b );
                trade_cancel( a, TRADE_FAIL_INVENTORY );
                trade_salvagerecipe_cleanup(pcharANew, pcharBNew);
                return;
            } else {
                character_AdjustRecipe(pcharANew, ta->reci[i].pRec, -1 * ta->reci[i].iNumCombines, NULL);
            }
        }
        for( i = 0; i < tb->iReciNum; i++ ) // B recipe remove
        {
            if (tb->reci[i].pRec->flags & RECIPE_NO_TRADE)
            {
                trade_fail( b, a );
                trade_cancel( b, TRADE_FAIL_CORRUPT );
                trade_salvagerecipe_cleanup(pcharANew, pcharBNew);
            }
            else if (!character_CanRecipeBeChanged( pcharBNew, tb->reci[i].pRec, -1 * tb->reci[i].iNumCombines))
            {
                trade_fail( b, a );
                trade_cancel( b, TRADE_FAIL_INVENTORY );
                trade_salvagerecipe_cleanup(pcharANew, pcharBNew);
                return;
            } else {
                character_AdjustRecipe(pcharBNew, tb->reci[i].pRec, -1 * tb->reci[i].iNumCombines, NULL);
            }
        }

        //////////////////////////////////////////////////////////////////////////
        // add salvages
        for( i = 0; i < ta->iSalvNum; i++ ) // B adding
        {
            if (!character_CanAdjustSalvage( pcharBNew, ta->salv[i].sal, 1 * ta->salv[i].iNumCombines))
            {
                trade_fail( b, a );
                trade_cancel( b, TRADE_FAIL_INVENTORY );
                trade_salvagerecipe_cleanup(pcharANew, pcharBNew);
                return;
            } else {
                character_AdjustSalvage(pcharBNew, ta->salv[i].sal, 1 * ta->salv[i].iNumCombines, NULL, true);
            }
        }

        for( i = 0; i < tb->iSalvNum; i++ ) // A adding
        {
            if (!character_CanAdjustSalvage( pcharANew, tb->salv[i].sal, 1 * tb->salv[i].iNumCombines))
            {
                trade_fail( a, b );
                trade_cancel( a, TRADE_FAIL_INVENTORY );
                trade_salvagerecipe_cleanup(pcharANew, pcharBNew);
                return;
            } else {
                character_AdjustSalvage(pcharANew, tb->salv[i].sal, 1 * tb->salv[i].iNumCombines, NULL, true);
            }
        }

        // add recipes
        for( i = 0; i < ta->iReciNum; i++ ) // B adding recipes
        {
            if (!character_CanRecipeBeChanged( pcharBNew, ta->reci[i].pRec, 1 * ta->reci[i].iNumCombines))
            {
                trade_fail( b, a );
                trade_cancel( b, TRADE_FAIL_INVENTORY );
                trade_salvagerecipe_cleanup(pcharANew, pcharBNew);
                return;
            } else {
                character_AdjustRecipe(pcharBNew, ta->reci[i].pRec, -1 * ta->reci[i].iNumCombines, NULL);
            }
        }

        for( i = 0; i < tb->iReciNum; i++ ) // A adding recipes
        {
            if (!character_CanRecipeBeChanged( pcharANew, tb->reci[i].pRec, 1 * tb->reci[i].iNumCombines))
            {
                trade_fail( a, b );
                trade_cancel( a, TRADE_FAIL_INVENTORY );
                trade_salvagerecipe_cleanup(pcharANew, pcharBNew);
                return;
            } else {
                character_AdjustRecipe(pcharANew, tb->reci[i].pRec, 1 * tb->reci[i].iNumCombines, NULL);
            }
        }
        trade_salvagerecipe_cleanup(pcharANew, pcharBNew);
    }

    // Verify temp powers can be removed
    //-------------------------------------------------------------------
    for( i = 0; i < ta->iTpowNum; i++ )  // A
    {
        Power * pow = a->pchar->ppPowerSets[ta->tpow[i].iCol]->ppPowers[ta->tpow[i].iRow];
        if( !character_CanRemoveTemporaryPower(a->pchar, pow) )
        {
            trade_fail( a, b );
            trade_cancel( a, TRADE_FAIL_INVENTORY );
            return;
        }
        if( !character_CanAddTemporaryPower(b->pchar, pow) )
        {
            trade_fail( b, a );
            trade_cancel( b, TRADE_FAIL_INVENTORY );
            return;
        }
    }

    for( i = 0; i < tb->iTpowNum; i++ )  // B
    {
        Power * pow = b->pchar->ppPowerSets[tb->tpow[i].iCol]->ppPowers[tb->tpow[i].iRow];
        if( !character_CanRemoveTemporaryPower(b->pchar, pow) )
        {
            trade_fail( b, a );
            trade_cancel( b, TRADE_FAIL_INVENTORY );
            return;
        }
        if( !character_CanAddTemporaryPower(a->pchar, pow) )
        {
            trade_fail( a, b );
            trade_cancel( a, TRADE_FAIL_INVENTORY );
            return;
        }
    }

    // everything seems to be in order
    //-------------------------------------------------------------------
    trade_sucess( a, b );

}

void trade_CheckFinalize( Entity * e )
{
    if( e->pl )
    {
        if( e->pl->trade )
        {
            if( e->pl->trade->state == TRADE_WAITING_FOR_SYNC )
                trade_finalize( e );
        }
    }
}

void trade_tick( Entity *e )
{
    Entity *partner;

    if( !e->pl->trade )
        return;

    partner = entFromDbId( e->pl->trade->partnerDbid );

    if( !partner ) 
    {
        trade_cancel( e, TRADE_FAIL_OTHER_LEFT );
        return;
    }

    if( distance3Squared(ENTPOS(e), ENTPOS(partner)) > SQR(20) )
        trade_cancel( e, TRADE_FAIL_DISTANCE );
}
