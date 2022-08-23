#include "uiTradeLogic.h"
#include "UI/uiCursor.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiWindows.h"
#include "UI/uiChat.h"
#include "gameComm/wdwbase.h"
#include "entity/character_net.h"
#include "arena/arenagame.h"
#include "UI/uiContextMenu.h"
#include "entity/salvage.h"
#include "player/player.h"
#include "account/AccountData.h"
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

enum
{
    TRADE_NONE,
    TRADE_READY,
    TRADE_ACTIVE,
};

Trade trade = {0};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

int canTradeWithTarget(void *foo)
{
    Entity *e = playerPtr();

    if( current_target && ENTTYPE(current_target) == ENTTYPE_PLAYER && e && e->pl && AccountCanTrade(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags) )
    {
        if( amOnArenaMap() )
            return CM_VISIBLE;
        else
            return CM_AVAILABLE;
    }
    else
        return CM_HIDE;
}


void tradeWithTarget(void *foo)
{
    if( current_target && ENTTYPE(current_target) == ENTTYPE_PLAYER )
    {
        char buf[256];
        sprintf( buf, "trade %s", current_target->name );
        cmdParse( buf );
    }
}

void trade_init( int partnerId )
{
    memset( &trade, 0, sizeof(Trade) );
    trade.partnerDbid = partnerId;

    window_setMode( WDW_TRADE, WINDOW_GROWING );
}

void trade_clear( int preserve )
{
    if( !preserve )
    {
        memset( &trade, 0, sizeof(Trade) );
    }
    else
    {
        memset( &trade.bSpec, 0, sizeof(BasePower*)*CHAR_BOOST_MAX );
        memset( (void*)&trade.bInsp, 0, sizeof(BasePower*)*CHAR_INSP_MAX );
    }
}

void trade_end()
{
    window_setMode( WDW_TRADE, WINDOW_SHRINKING );
    trade_clear( 0 );
}

void trade_cancel( char * reason )
{
    window_setMode( WDW_TRADE, WINDOW_SHRINKING );
    addSystemChatMsg( reason, INFO_USER_ERROR, 0 );
    trade_clear( 0 );
}

void trade_sendUpdate( Packet * pak )
{
    int i;

    pktSendBitsPack( pak, 1, trade.partnerDbid );
    pktSendBitsPack( pak, 1, trade.aAccepted );
    pktSendBitsPack( pak, 1, trade.aInfluence );

    pktSendBitsPack( pak, 1, trade.aNumSpec );
    pktSendBitsPack( pak, 1, trade.aNumInsp );
    pktSendBitsPack( pak, 1, trade.aNumSalv );
    pktSendBitsPack( pak, 1, trade.aNumTpow );
    pktSendBitsPack( pak, 1, trade.aNumReci );

    for( i = 0; i < trade.aNumSpec; i++ )
    {
        pktSendBitsPack(pak, 1, trade.aSpec[i].iCol);
    }


    for( i = 0; i < trade.aNumInsp; i++ )
    {
        pktSendBitsPack(pak, 1, trade.aInsp[i].iCol );
        pktSendBitsPack(pak, 1, trade.aInsp[i].iRow );
    }
    for( i = 0; i < trade.aNumSalv; i++ )
    {
        pktSendBitsPack(pak, 1, trade.aSalv[i].iCol );
        pktSendBitsPack(pak, 1, trade.aSalv[i].iRow );
        pktSendBitsPack(pak, 1, trade.aSalv[i].iNumCombines);
    }
    for( i = 0; i < trade.aNumReci; i++ )
    {
        pktSendBitsPack(pak, 1, trade.aReci[i].iCol );
        pktSendBitsPack(pak, 1, trade.aReci[i].iNumCombines);
    }
    for( i = 0; i < trade.aNumTpow; i++ )
    {
        pktSendBitsPack(pak, 1, trade.aTpow[i].iCol );
        pktSendBitsPack(pak, 1, trade.aTpow[i].iRow );
    }

}

void trade_receiveUpdate( Packet *pak )
{
    // read in data so we can compare for changes
    int dbid    = pktGetBitsPack( pak, 1 );
    int acceptA = pktGetBitsPack( pak, 1 );
    int acceptB = pktGetBitsPack( pak, 1 );
    int influenceA = pktGetBitsPack( pak, 1 );
    int influenceB = pktGetBitsPack( pak, 1 );

    int specCount = pktGetBitsPack( pak, 1 );
    int inspCount = pktGetBitsPack( pak, 1 );
    int salvCount = pktGetBitsPack( pak, 1 );
    int tpowCount = pktGetBitsPack( pak, 1 );
    int reciCount = pktGetBitsPack( pak, 1 );

    int i;

    // TODO check changes and play nice fx
    trade.aAccepted = acceptA;
    trade.bAccepted = acceptB;

    trade.aInfluence = influenceA;
    trade.bInfluence = influenceB;

    // TODO make sure stays within array
    trade.bNumSpec = specCount;
    trade.bNumInsp = inspCount;
    trade.bNumSalv = salvCount;
    trade.bNumTpow = tpowCount;
    trade.bNumReci = reciCount;


    trade_clear( 1 );
    for( i = 0; i < specCount; i++ )
    {
        // check for changes later
        trade.bSpec[i].ppowBase = basepower_Receive(pak, &g_PowerDictionary, 0);
        trade.bSpec[i].iLevel = pktGetBitsPack(pak, 5);
//        trade.bSpec[i].iNumCombines = pktGetBitsPack(pak, 2);
    }

    for( i = 0; i < inspCount; i++ )
    {
        trade.bInsp[i] =  basepower_Receive(pak, &g_PowerDictionary, 0);
    }

    for( i = 0; i < salvCount; i++ )
    {
        int salId = pktGetBitsPack(pak, 4);
        trade.bSalv[i].sal = salvage_GetItemById(salId);
        trade.bSalv[i].count = pktGetBitsPack(pak,1);
    }
    for( i = 0; i < reciCount; i++ )
    {
        int recId = pktGetBitsPack(pak, 6);
        trade.bReci[i].pRec = recipe_GetItemById(recId);
        trade.bReci[i].count = pktGetBitsPack(pak,1);
    }
    for( i = 0; i < tpowCount; i++ )
    {
        trade.bTpow[i].ppowBase = basepower_Receive(pak, &g_PowerDictionary, 0);
        trade.bTpow[i].iNumCharges = pktGetBitsPack(pak, 1);
        trade.bTpow[i].fUsageTime = pktGetF32(pak);
        trade.bTpow[i].fAvailableTime = pktGetF32(pak);
        trade.bTpow[i].ulCreationTime = pktGetBits(pak,32);
    }
}

