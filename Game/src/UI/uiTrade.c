
#include "UI/uiTrade.h"
#include "UI/uiNet.h"
#include "UI/uiChat.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiGame.h"
#include "UI/uiCursor.h"
#include "UI/uiWindows.h"
#include "UI/uiReticle.h"
#include "UI/uiScrollBar.h"
#include "UI/uiEditText.h"
#include "UI/uiTray.h"
#include "UI/uiInspiration.h"
#include "UI/uiEnhancement.h"
#include "UI/uiRecipeInventory.h"
#include "UI/uiInput.h"
#include "UI/uiInfo.h"
#include "UI/uiDialog.h"
#include "UI/uiContextMenu.h"
#include "uiCombineSpec.h"
#include "uiAmountSlider.h"
#include "UI/uiOptions.h"
#include "win/win_init.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/ttFontUtil.h"

#include "gameComm/wdwbase.h"
#include "entity/entity.h"
#include "entity/powers.h"
#include "cmdparse/cmdgame.h"
#include "entity/character_net.h"
#include <utilitieslib/components/earray.h>
#include "entity/costume.h"
#include "comm_game.h"
#include "entity/origins.h"
#include "entity/classes.h"
#include "player/player.h"
#include <utilitieslib/utils/strings_opt.h>
#include "entity/entVarUpdate.h"
#include "entity/character_base.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/mathutil.h>
#include "entity/character_inventory.h"
#include "uiSalvage.h"
#include "UIEdit.h"
#include "win/input.h"
#include <utilitieslib/language/MessageStoreUtil.h>

#define NUM_WD  11
#define NUM_HT  20

#define SPEC_SCALE      .5f

#define OFFER_WD        310

#define INV_HT          43
#define INV_BORDER      10

#define ICON_WD             32  // Size of inspiration icon
#define ICON_SPACE          5
#define MAX_TRADE_ROW       5

#define BUTTON_OFFSET       40
#define BUTTON_HT           44

#define TELEMENT_HT         38
#define TELEMENT_TXT_YOFF   40
#define TELEMENT_SPACE      9

enum
{
    NUM_NO_CHANGE,
    NUM_UP,
    NUM_DOWN,
};


//-------------------------------------------------------------------------------
// A M O U N T   S L I D E R   //////////////////////////////////////////////////
//-------------------------------------------------------------------------------
void recipeMarkForTrade( Entity *e,int idx, int count );
void salvMarkForTrade( Entity *e,int idx, int count );


void amountSliderDrawRecipeCallback(void *userData, float x, float y, float z, float sc, int clr)
{
    Entity *e = playerPtr();
    const DetailRecipe *recipe = detailrecipedict_RecipeFromId((int) userData);

    if ( recipe )
        uiRecipeDrawIcon(recipe, recipe->level, x-8*sc, y-5*sc, z, sc, 0, true, CLR_WHITE);
}

void amountSliderOKRecipeCallback(int amount, void *userData)
{
    const DetailRecipe *recipe = detailrecipedict_RecipeFromId((int) userData);
    if ( recipe )
    {
        recipeMarkForTrade( playerPtr(), (int) userData, amount );    
    }
}

void amountSliderDrawSalvageCallback(void *userData, float x, float y, float z, float sc, int clr)
{
    Entity *e = playerPtr();
    int idx = (int) userData;

    if ( e && e->pchar && e->pchar->salvageInv && e->pchar->salvageInv[idx]->salvage )
    {
        const SalvageItem *salvage = e->pchar->salvageInv[idx]->salvage;
        AtlasTex *icon = atlasLoadTexture(salvage->ui.pchIcon);
        if ( icon )
            display_sprite(icon, x, y, z, sc, sc, CLR_WHITE);
    }
}

void amountSliderOKSalvageCallback(int amount, void *userData)
{
    Entity *e = playerPtr();
    int idx = (int) userData;

    if ( e && e->pchar && e->pchar->salvageInv && e->pchar->salvageInv[idx]->salvage )
    {
        const SalvageItem *salvage = e->pchar->salvageInv[idx]->salvage;
        if ( salvage )
        {
            salvMarkForTrade( playerPtr(), idx, amount );    
        }
    }
}

//-------------------------------------------------------------------------------
// CONTEXT MENU /////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------

static int gDeleteNextClick = 0;
static TradeObj deleteMe = {0};

ContextMenu * gTradeContext = 0;

static void trade_ContextMenu( CBox *box, int type, const BasePower * ppow, int iLevel, const SalvageItem *sal, const DetailRecipe *pRec)
{
    static TradeObj to_tmp = {0};

    if( mouseCollision(box) && mouseRightClick() )
    {
        int rx, ry;
        int wd, ht;
        CMAlign alignVert;
        CMAlign alignHoriz;

        to_tmp.ppow = ppow;
        to_tmp.sal = sal;
        to_tmp.pRec = pRec;
        to_tmp.iLevel = iLevel;

        if( type == kTrayItemType_Inspiration || type == kTrayItemType_Power )
            to_tmp.iCol = 0;
        else if (type == kTrayItemType_Salvage)
            to_tmp.iCol = 2;
        else if (type == kTrayItemType_Recipe)
            to_tmp.iCol = 3;
        else 
            to_tmp.iCol = 1;

        windowClientSizeThisFrame( &wd, &ht );

        rx = (box->lx+box->hx)/2;
        alignHoriz = CM_CENTER;

        ry = box->ly<ht/2 ? box->hy : box->ly;
        alignVert = box->ly<ht/2 ? CM_TOP : CM_BOTTOM;

        contextMenu_setAlign( gTradeContext, rx, ry, alignHoriz, alignVert, &to_tmp );
    }
}

static const char* trade_PowerText( TradeObj *to )
{
    if (to->iCol == 2) 
        return to->sal->ui.pchDisplayName;
    else if (to->iCol == 3)
        return to->pRec->ui.pchDisplayName;
    else 
        return to->ppow->pchDisplayName;
}

static const char* trade_PowerInfo( TradeObj *to )
{
    if (to->iCol == 2) 
        return to->sal->ui.pchDisplayShortHelp;
    else if (to->iCol == 3)
        return to->pRec->ui.pchDisplayShortHelp;
    else
        return to->ppow->pchDisplayShortHelp;
}

static void trade_Info( TradeObj *to  )
{
    if (to->iCol == 3)
        info_Recipes(to->pRec);
    if (to->iCol == 2)
        info_Salvage(to->sal);
    if (to->iCol == 1)
        info_CombineSpec( to->ppow, 1, to->iLevel, to->iNumCombines );
    else
        info_Power( to->ppow, 0);
}

//-------------------------------------------------------------------------------
// INFLUENCE ////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------

static UIEdit *sInfEdit;
static char sInfText[12];

static void trade_drawInfluenceInventory( float x, float y, float z, float sc, float wd, float ht, int color )
{
    Entity *e = playerPtr();
    CBox box;
    int change = 0;
    float numwd;
    static int editing;
    int editSize = 11;
    int maxInf = 999999999;

    if(!sInfEdit)
    {
        sInfEdit = uiEditCreateSimple(NULL, editSize, &game_12, CLR_WHITE, uiEditCommaNumericInputHandler);
        uiEditSetClicks(sInfEdit, "type2", "mouseover");
    }
    if (!editing) {
        sprintf(sInfText,"%d",trade.aInfluence);
        uiEditSetUTF8Text(sInfEdit,sInfText);
        uiEditCommaNumericUpdate(sInfEdit);
    }
    numwd = (float)(str_wd(&game_12,sc,sc,"222222222")-1)/5/sc;
    font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );

    cprntEx( x, y + ht/2, z+1, sc, sc,CENTER_Y,"GiveInfluence" );
    BuildCBox( &box, x + (115-1.5*numwd+1)*sc, y+ht/2 - 9*sc, (6*numwd+2)*sc, 18*sc );

    if( !mouseCollision(&box) )
         drawFrame( PIX2, R6, box.left, box.top, z+1, box.right-box.left, box.bottom-box.top, sc, color, CLR_BLACK );
    else
        drawFrame( PIX2, R6, box.left, box.top, z+1, box.right-box.left, box.bottom-box.top, sc, color, 0x99999999);

    if( mouseClickHit( &box, MS_LEFT) )
    {
        uiEditTakeFocus(sInfEdit);
        uiEditSetUTF8Text(sInfEdit,"");
        editing = 1;
    }
    if( !mouseCollision(&box) && mouseDown(MS_LEFT) )
        uiEditReturnFocus(sInfEdit);
    
    uiEditSetFontScale(sInfEdit,sc);
    uiEditEditSimple(sInfEdit,box.left+numwd/2*sc, box.top, z+2, box.right-box.left-numwd*sc, box.bottom-box.top, &game_12, INFO_CHAT_TEXT, FALSE);
    if (editing) {
        int i;
        char *tmp = uiEditGetUTF8Text(sInfEdit);
        if (tmp) strcpy(sInfText,tmp);
        else strcpy(sInfText,"0");

        i = atoi_ignore_nonnumeric(sInfText);
        if (i < 0 || i > maxInf) {
            change = 1;
        } else {
            trade.aInfluence = i;
        }

        if (!uiEditHasFocus(sInfEdit))  
            editing = false; 
    }

    if( trade.aInfluence > e->pchar->iInfluencePoints )
        trade.aInfluence = e->pchar->iInfluencePoints;

    if ( change )
        sendTradeUpdate();

}

//-------------------------------------------------------------------------------
// ENHANCEMENTS /////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------

//
//
static void specRemoveFromTrade( int idx )
{
    int i;
    for( i = 0; i < trade.aNumSpec; i++ )
    {
        if( idx == trade.aSpec[i].iCol )
        {
            memcpy( &trade.aSpec[i], &trade.aSpec[i+1], sizeof(TradeObj)*(trade.aNumSpec-i));
            trade.aNumSpec--;
            sendTradeUpdate();
        }
    }
}

//
//

void specConfirmPutUpForTrade(void *data)
{
    Entity *e = playerPtr();
    int tradeIdx = (int) data;

    if( trade.aNumSpec < CHAR_BOOST_MAX )
    {
        trade.aSpec[trade.aNumSpec].iCol = tradeIdx;
        trade.aSpec[trade.aNumSpec].iLevel = e->pchar->aBoosts[tradeIdx]->iLevel;
        trade.aSpec[trade.aNumSpec].ppow = e->pchar->aBoosts[tradeIdx]->ppowBase;
        e->pchar->aBoosts[tradeIdx]->iNumCombines = 0;
        trade.aNumSpec++;
        sendTradeUpdate();
    }
}


//
void specMarkForTrade( Entity *e, int i )
{
    int idx;
    const BasePower *ppow = NULL;
    const DetailRecipe *recipe = NULL;

    for( idx = 0; idx < trade.aNumSpec; idx++)
    {
        if( trade.aSpec[idx].iCol == i )
            return;
    }

    if( !e->pchar->aBoosts[i] || !e->pchar->aBoosts[i]->ppowBase )
        return;

    ppow = e->pchar->aBoosts[i]->ppowBase;

    if (!detailrecipedict_IsBoostTradeable(ppow, e->pchar->aBoosts[i]->iLevel, e->auth_name, ""))
    {
        addSystemChatMsg( textStd("CannotTradeEnhancement"), INFO_USER_ERROR, 0 );
        return;
    }

    if (e->pchar->aBoosts[i]->iNumCombines > 0) {
        dialog( DIALOG_YES_NO, -1, -1, -1, -1, "ReallyTradeCombinedEnhancement", NULL, specConfirmPutUpForTrade, NULL, NULL, 0, NULL, NULL,0,0,0, (void *) i );
    } else {
        specConfirmPutUpForTrade((void *) i);
    }

}

//-------------------------------------------------------------------------------
// INSPIRATIONS //// ////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------

static void inspRemoveFromTrade( int i, int j )
{
    int idx;

    for( idx = 0; idx < trade.aNumInsp; idx++)
    {
        if( trade.aInsp[idx].iCol == i && trade.aInsp[idx].iRow == j )
        {
            memcpy(&trade.aInsp[idx], &trade.aInsp[idx+1], sizeof(TradeObj)*(trade.aNumInsp-idx) );
            trade.aNumInsp--;
            sendTradeUpdate();
        }
    }
}

//
int inspIsMarkedForTrade( Entity *e, int i, int j )
{
    int idx;

    // look for it in trade already
    for( idx = 0; idx < trade.aNumInsp; idx++)
    {
        if( trade.aInsp[idx].iCol == i && trade.aInsp[idx].iRow == j )
        {
            return 1;
        }
    }
    return 0;
}
//
//
void inspMarkForTrade( Entity *e, int i, int j )
{
    int idx;

    // look for it in trade already
    for( idx = 0; idx < trade.aNumInsp; idx++)
    {
        if( trade.aInsp[idx].iCol == i && trade.aInsp[idx].iRow == j )
        {
            return;
        }
    }

    if( !e->pchar->aInspirations[i][j] )
        return;

    if (!inspiration_IsTradeable(e->pchar->aInspirations[i][j], e->auth_name, ""))
    {
        addSystemChatMsg( textStd("CannotTradeInspiration"), INFO_USER_ERROR, 0 );
        return;
    }

    if( trade.aNumInsp < CHAR_INSP_MAX )
    {
        trade.aInsp[trade.aNumInsp].iCol = i;
        trade.aInsp[trade.aNumInsp].iRow = j;
        trade.aInsp[trade.aNumInsp].ppow = e->pchar->aInspirations[i][j];
        trade.aNumInsp++;
        sendTradeUpdate();
    }
}


void trade_setInspiration( const BasePower * pow, int i, int j )
{
    if( pow == NULL )
        inspRemoveFromTrade( i, j );
}

//-------------------------------------------------------------------------------
// SALVAGE //////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------

//
//
void salvRemoveFromTrade(int idx, int count)
{
    int i;
    for( i = 0; i < trade.aNumSalv; i++ )
    {
        if( idx == trade.aSalv[i].iCol)
        {
            if (count)
            {
                trade.aSalv[i].iNumCombines -= count;
            }
            if (!count || trade.aSalv[i].iNumCombines <= 0)
            {
                memcpy( &trade.aSalv[i], &trade.aSalv[i+1], sizeof(TradeObj)*(trade.aNumSalv-i));
                trade.aNumSalv--;
            }
            sendTradeUpdate();
        }
    }
}

//
//
void salvMarkForTrade( Entity *e,int idx, int count )
{
    int i;

    if( !count )
        count = e->pchar->salvageInv[idx]->amount;

    for( i = 0; i < trade.aNumSalv; i++)
    {
        if( trade.aSalv[i].iCol == idx) // is this the one?
        { 
            count += trade.aSalv[i].iNumCombines;

            if( count > (int)e->pchar->salvageInv[idx]->amount )
                count = e->pchar->salvageInv[idx]->amount;

            if (!character_CanAdjustSalvage(e->pchar,e->pchar->salvageInv[idx]->salvage,-count) )
            {
                return; //dont give more than you have. should never happen
            }

            trade.aSalv[i].iNumCombines = count;

            sendTradeUpdate();
            return;
        }
    }

    if( !e->pchar->salvageInv || !e->pchar->salvageInv[idx] )
        return;
    
    if (e->pchar->salvageInv[idx]->salvage && e->pchar->salvageInv[idx]->salvage->flags & SALVAGE_NOTRADE)
    {
        addSystemChatMsg(textStd("CannotTradeSalvage"), INFO_USER_ERROR, 0);
        return;
    }

    if( trade.aNumSalv < SALV_TRADE_MAX  )
    {
        if (!character_CanAdjustSalvage(e->pchar,e->pchar->salvageInv[idx]->salvage,-count))
            return;
        trade.aSalv[trade.aNumSalv].iCol = idx;
        trade.aSalv[trade.aNumSalv].iNumCombines = count;
        trade.aNumSalv++;
        sendTradeUpdate();
    }
}

void uiTrade_UpdateSalvageInventoryCount( Entity *e,int idx )
{
    int i;
    int playerCount = e->pchar->salvageInv[idx]->amount;

    for( i = 0; i < trade.aNumSalv; i++)
    {
        if( trade.aSalv[i].iCol == idx) // is this the recipe being modified?
        { 
            if (playerCount <= 0)
            {
                salvRemoveFromTrade(idx, 0);
            } else {
                if (trade.aSalv[i].iNumCombines > playerCount) 
                {
                    trade.aSalv[i].iNumCombines = playerCount;
                    sendTradeUpdate();
                }
            }
            break;
        }
    }
}

//-------------------------------------------------------------------------------
// RECIPES //////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------

//
//
void recipeRemoveFromTrade(int idx, int count)
{
    int i;

    for( i = 0; i < trade.aNumReci; i++ )
    {
        if( idx == trade.aReci[i].iCol)
        {
            if (count)
            {
                trade.aReci[i].iNumCombines -= count;
            }
            if (!count || trade.aReci[i].iNumCombines <= 0)
            {
                memcpy( &trade.aReci[i], &trade.aReci[i+1], sizeof(TradeObj)*(trade.aNumReci-i));
                trade.aNumReci--;
            }
            sendTradeUpdate();
        }
    }
}

//
//
void recipeMarkForTrade( Entity *e,int idx, int count )
{
    int i;
    const DetailRecipe *pRec = recipe_GetItemById(idx);
    int playerCount = character_RecipeCount(e->pchar, pRec);

    if( !count )
        count = playerCount;

    for( i = 0; i < trade.aNumReci; i++)
    {
        if( trade.aReci[i].iCol == idx) //increment if exists
        { 
            count += trade.aReci[i].iNumCombines;

            if( count > playerCount )
                count = playerCount;

            if (!character_CanRecipeBeChanged(e->pchar, pRec, -count) )
            {
                return; //dont give more than you have. should never happen
            }

            trade.aReci[i].iNumCombines = count;

            sendTradeUpdate();
            return;
        }
    }

    if( playerCount <= 0 )
        return;

    if (pRec && (pRec->flags & RECIPE_NO_TRADE) )
    {
        addSystemChatMsg(textStd("CannotTradeRecipe"), INFO_USER_ERROR, 0);
        return;
    }

    if( trade.aNumReci < SALV_TRADE_MAX  )
    {
        if (!character_CanRecipeBeChanged(e->pchar, pRec, -count))
            return;

        trade.aReci[trade.aNumReci].iCol = idx;
        trade.aReci[trade.aNumReci].iNumCombines = count;
        trade.aNumReci++;
        sendTradeUpdate();
    }
}

void uiTrade_UpdateRecipeInventoryCount( Entity *e, int idx, int id )
{
    int i;
    const DetailRecipe *pRec = recipe_GetItemById(id);
    int playerCount = character_RecipeCount(e->pchar, pRec);

    if (pRec == NULL)
        return;

    playerCount = character_RecipeCount(e->pchar, pRec);

    for( i = 0; i < trade.aNumReci; i++)
    {
        if( trade.aReci[i].iCol == id) // is this the recipe being modified?
        { 
            if (playerCount <= 0)
            {
                recipeRemoveFromTrade(id, 0);
            } else {
                if (trade.aReci[i].iNumCombines > playerCount)
                {
                    trade.aReci[i].iNumCombines = playerCount;
                    sendTradeUpdate();
                }
            }
            break;
        }
    }
}

//-------------------------------------------------------------------------------
// TEMP POWERS ////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------

bool powerIsMarkedForTrade( Power * pow )
{
    int i;
    for( i = 0; i < trade.aNumTpow; i++ )
    {
        if( pow->psetParent->idx == trade.aTpow[i].iCol && pow->idx == trade.aTpow[i].iRow )
            return true;
    }
    return false;
}

static void tpowRemoveFromTrade( int iset, int ipow )
{
    int idx;

    for( idx = 0; idx < trade.aNumTpow; idx++)
    {
        if( trade.aTpow[idx].iCol == iset && trade.aTpow[idx].iRow == ipow )
        {
            memcpy(&trade.aTpow[idx], &trade.aTpow[idx+1], sizeof(TradeObj)*(trade.aNumTpow-idx) );
            trade.aNumTpow--;
            sendTradeUpdate();
        }
    }
}

static void tpowMarkForTrade( Entity *e, int i, int j )
{
    int idx;
    Power* pow = NULL;

    if( i >= 0 && i < eaSize(&e->pchar->ppPowerSets) &&
        j >= 0 && j < eaSize(&e->pchar->ppPowerSets[i]->ppPowers) )
    {
        pow = e->pchar->ppPowerSets[i]->ppPowers[j];
    }

    if( !pow ) 
        return;

    if( !pow->ppowBase->bTradeable )
    {
        addSystemChatMsg(textStd("PowerNotTradeable",pow->ppowBase->pchDisplayName), INFO_USER_ERROR, 0);
        return;
    }

    // look for it in trade already
    for( idx = 0; idx < trade.aNumTpow; idx++)
    {
        if( trade.aTpow[idx].iCol == i && trade.aTpow[idx].iRow == j )
            return;
    }

    // force world targeting cursor off
    cursor.target_world = kWorldTarget_None;
    s_ptsActivated = NULL;

    if( trade.aNumTpow < SALV_TRADE_MAX )
    {
        trade.aTpow[trade.aNumTpow].iCol = i;
        trade.aTpow[trade.aNumTpow].iRow = j;
        trade.aTpow[trade.aNumTpow].ppow = pow->ppowBase;
        trade.aNumTpow++;
        sendTradeUpdate();
    }
}

//-------------------------------------------------------------------------------
// OFFER ////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------

//
//
static void trade_influenceElement( float x, float y, float z, float sc, float wd, float ht, int influence )
{
    float strwd;
    drawFadeBar( x, y, z, wd, ht, 0xffffff22 );

     font( &game_12 );
    font_color( CLR_YELLOW, CLR_YELLOW );
     cprntEx( x + ht/4, y + ht/2, z, sc, sc, CENTER_Y, "%s", itoa_with_commas_static(influence) );

    font_color( CLR_WHITE, CLR_WHITE );
    strwd = MAX( TELEMENT_TXT_YOFF*sc, str_wd(font_grp, sc, sc, "%i", influence )+10*sc );
    cprntEx( x + strwd, y + ht/2, z, sc, sc, CENTER_Y, "InfluenceString" );
}

//
//
static void trade_inspirationElement( const void * insp, float x, float y, float z, float sc, float wd, float ht, int partner )
{
    Entity      *e = playerPtr();
    const TradeObj    *to = 0;
    const BasePower   *ppow;
    AtlasTex* icon = NULL;
    CBox        box;

    drawFadeBar( x, y, z, wd, ht, 0xffffff22 );

    font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );

    if( partner )
        ppow = (const BasePower*)insp;
    else
    {
        TrayObj * actual;
        to = (const TradeObj*)insp;
        actual = inspiration_getCur( to->iCol, to->iRow );
         ppow = e->pchar->aInspirations[actual->iset][actual->ipow];
    }

    if( ppow )
    {
        icon = atlasLoadTexture( ppow->pchIconName );
        if(icon)
            display_sprite( icon, x + (ht - icon->width*sc)/2 + PIX3*2*sc, y + (ht - icon->height*sc)/2, z, sc, sc, CLR_WHITE );
        cprntEx( x + TELEMENT_TXT_YOFF*sc, y + ht/2, z,sc, sc, CENTER_Y, ppow->pchDisplayName );
    }

    BuildCBox( &box, x - TELEMENT_SPACE*sc, y - TELEMENT_SPACE*sc, wd + 2*TELEMENT_SPACE*sc, ht + 14*sc );

    if( ppow )
        trade_ContextMenu( &box, kTrayItemType_Inspiration, ppow, 0, 0, 0);

    if( !partner )
    {
        if( mouseClickHit( &box, MS_LEFT ))
            inspRemoveFromTrade( to->iCol, to->iRow );

        if( mouseCollision(&box) )
        {
            if( isDown(MS_LEFT) )
                drawFlatFrame( PIX2, R10, x-TELEMENT_SPACE*sc, y-TELEMENT_SPACE*sc, z-2, wd+2*TELEMENT_SPACE*sc, ht+2*TELEMENT_SPACE*sc, sc,
                                CLR_SELECTION_FOREGROUND, CLR_SELECTION_BACKGROUND );
            else
                drawFlatFrame( PIX2, R10, x-TELEMENT_SPACE*sc, y-TELEMENT_SPACE*sc, z-2, wd+2*TELEMENT_SPACE*sc, ht+2*TELEMENT_SPACE*sc, sc,
                                CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND );
        }

        if( mouseLeftDrag(&box) )
        {
            inspiration_drag( to->iCol, to->iRow, icon );
            inspRemoveFromTrade( to->iCol, to->iRow );
        }
    }
}

//
//
static void trade_enhancementElements( void * spec, float x, float y, float z, float sc, float wd, float ht, int partner )
{
    TradeObj    *to = 0;
    const Boost        *pboost;
    const BasePower   *ppow;
    CBox        box;
    int         iLevel;
    int         iNumCombines;
    AtlasTex* icon = NULL;
    Entity        *e = playerPtr();

    drawFadeBar( x, y, z, wd, ht, 0xffffff22 );

    if( partner )
    {
        pboost = (const Boost*)spec;
        ppow = pboost->ppowBase;
        iLevel = pboost->iLevel;
        iNumCombines = pboost->iNumCombines;
    }
    else
    {
        to = (TradeObj*)spec;
        pboost = e->pchar->aBoosts[to->iCol];

        if( pboost )
        {
            ppow = e->pchar->aBoosts[to->iCol]->ppowBase;
            iLevel = e->pchar->aBoosts[to->iCol]->iLevel;
            iNumCombines = e->pchar->aBoosts[to->iCol]->iNumCombines;
        }
        else
            ppow = NULL;
    }

    iNumCombines = 0;

    if( ppow )
    {

        if(pboost)
        {
            uiEnhancement *pEnh = uiEnhancementCreateFromBoost(pboost);
            if (pEnh)
                icon = uiEnhancementDraw(pEnh, x + ht/2 + PIX3*2*sc, y + ht/2, z-1, .5*sc, sc, MENU_GAME, WDW_TRADE, true);
            else
                icon = drawEnhancementOriginLevel( ppow, iLevel+1, iNumCombines, true, x + ht/2 + PIX3*2*sc, y + ht/2, z-1, .5*sc, sc, CLR_WHITE, false );
        } else {
            icon = drawEnhancementOriginLevel( ppow, iLevel+1, iNumCombines, true, x + ht/2 + PIX3*2*sc, y + ht/2, z-1, .5*sc, sc, CLR_WHITE, false );
        }
        font( &game_9 );
        font_color( CLR_WHITE, CLR_WHITE );
        cprntEx( x + TELEMENT_TXT_YOFF*sc, y + ht/2, z, sc, sc, CENTER_Y, ppow->pchDisplayName );
    }

    BuildCBox( &box, x - TELEMENT_SPACE*sc, y - TELEMENT_SPACE*sc, wd + 2*TELEMENT_SPACE*sc, ht + 14*sc );

    if( ppow )
        trade_ContextMenu( &box, kTrayItemType_SpecializationInventory, ppow, iLevel, 0, 0 );

    if( !partner )
    {
        if( mouseClickHit( &box, MS_LEFT ))
            specRemoveFromTrade( to->iCol );

        if( mouseCollision(&box) )
        {
            if( isDown(MS_LEFT) )
                drawFlatFrame( PIX2, R10, x-TELEMENT_SPACE*sc, y-TELEMENT_SPACE*sc, z-2, wd+2*TELEMENT_SPACE*sc, ht+2*TELEMENT_SPACE*sc, sc,
                                CLR_SELECTION_FOREGROUND, CLR_SELECTION_BACKGROUND );
            else
                drawFlatFrame( PIX2, R10, x-TELEMENT_SPACE*sc, y-TELEMENT_SPACE*sc, z-2, wd+2*TELEMENT_SPACE*sc, ht+2*TELEMENT_SPACE*sc, sc,
                                CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND );
        }

        if( mouseLeftDrag(&box) )
        {
            TrayObj tmp;
            buildSpecializationTrayObj( &tmp, 1, 0, 0, to->iCol);
            trayobj_startDragging( &tmp, atlasLoadTexture(ppow->pchIconName), icon );
            specRemoveFromTrade( to->iCol );
        }
    }
}

static void trade_salvageElement(void * salvage, float x, float y, float z, float sc, float wd, float ht, int partner )
{
    Entity      *e = playerPtr();
    TradeObj    *to = 0;
    AtlasTex* icon = NULL;
    CBox        box;
    const SalvageItem *sal;
    int count;

    drawFadeBar( x, y, z, wd, ht, 0xffffff22 );

    font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );

    if( partner )
    {
        SalvageTrade *st = (SalvageTrade *)salvage;
        sal = st->sal;
        count = st->count;
    }
    else
    {
        to = (TradeObj*)salvage;
        sal = e->pchar->salvageInv[to->iCol]->salvage;
        count = to->iNumCombines;
    }

    if( sal )
    {
        icon = atlasLoadTexture( sal->ui.pchIcon);
        if(icon)
            display_sprite( icon, x + (ht - icon->width*sc*0.75)/2 + PIX3*2*sc, y + (ht - icon->height*sc*0.75)/2, z, sc*0.75, sc*0.75, CLR_WHITE );

        font( &game_9 );
        font_color( CLR_WHITE, CLR_WHITE );
        cprntEx( x + TELEMENT_TXT_YOFF*sc, y + ht/2, z,sc, sc, CENTER_Y, sal->ui.pchDisplayName);

        font( &game_9 );
        font_color(0x00deffff, 0x00deffff);
        prnt( x + 0*sc, y + 0*sc, z, sc, sc, "%d", count );
    }

    BuildCBox( &box, x - TELEMENT_SPACE*sc, y - TELEMENT_SPACE*sc, wd + 2*TELEMENT_SPACE*sc, ht + 14*sc );

    if( sal )
        trade_ContextMenu( &box, kTrayItemType_Salvage, 0, 0, sal, 0 );

    if( !partner )
    {
        if( mouseClickHit( &box, MS_LEFT ))
            salvRemoveFromTrade( to->iCol,0);

        if( mouseCollision(&box) )
        {
            if( isDown(MS_LEFT) )
                drawFlatFrame( PIX2, R10, x-TELEMENT_SPACE*sc, y-TELEMENT_SPACE*sc, z-2, wd+2*TELEMENT_SPACE*sc, ht+2*TELEMENT_SPACE*sc, sc,
                CLR_SELECTION_FOREGROUND, CLR_SELECTION_BACKGROUND );
            else
                drawFlatFrame( PIX2, R10, x-TELEMENT_SPACE*sc, y-TELEMENT_SPACE*sc, z-2, wd+2*TELEMENT_SPACE*sc, ht+2*TELEMENT_SPACE*sc, sc,
                CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND );
        }

        if( mouseLeftDrag(&box) )
        {
            salvage_drag(to->iCol, icon,sc);
            salvRemoveFromTrade(to->iCol,0);
        }
    }
}

static void trade_recipeElement(void * recipe, float x, float y, float z, float sc, float wd, float ht, int partner )
{
    Entity      *e = playerPtr();
    TradeObj    *to = 0;
    AtlasTex* icon = NULL;
    CBox        box;
    const DetailRecipe *pRec;
    int count;

    drawFadeBar( x, y, z, wd, ht, 0xffffff22 );

    font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );

    if( partner ) {
        RecipeTrade *st = (RecipeTrade *) recipe;
        pRec = st->pRec;
        count = st->count;
    }
    else
    {
        to = (TradeObj *) recipe;
        pRec = recipe_GetItemById(to->iCol);
        count = to->iNumCombines;
    }

    if( pRec )
    {
        icon = atlasLoadTexture( pRec->ui.pchIcon);
        icon = uiRecipeDrawIcon(pRec, pRec->level, x + (ht - icon->width*sc*0.7)/2 - PIX3*sc, y + (ht - icon->height*sc*0.7)/2 - PIX3*3*sc, z, sc*0.7, WDW_TRADE, true, CLR_WHITE);  

        font( &game_9 );
        font_color( CLR_WHITE, CLR_WHITE );
        cprntEx( x + TELEMENT_TXT_YOFF*sc, y + ht/2, z,sc, sc, CENTER_Y, "%s %s", textStd("RecipeColon"), textStd(pRec->ui.pchDisplayName));

        font( &game_9 );
        font_color(0x00deffff, 0x00deffff);
        prnt( x + 0*sc, y + 0*sc, z, sc, sc, "%d", count );
    }

    BuildCBox( &box, x - TELEMENT_SPACE*sc, y - TELEMENT_SPACE*sc, wd + 2*TELEMENT_SPACE*sc, ht + 14*sc );

    if( pRec )
        trade_ContextMenu( &box, kTrayItemType_Recipe, 0, 0, 0, pRec );

    if( !partner )
    {
        if( mouseClickHit( &box, MS_LEFT ))
            recipeRemoveFromTrade( to->iCol,0);

        if( mouseCollision(&box) )
        {
            if( isDown(MS_LEFT) )
                drawFlatFrame( PIX2, R10, x-TELEMENT_SPACE*sc, y-TELEMENT_SPACE*sc, z-2, wd+2*TELEMENT_SPACE*sc, ht+2*TELEMENT_SPACE*sc, sc,
                CLR_SELECTION_FOREGROUND, CLR_SELECTION_BACKGROUND );
            else
                drawFlatFrame( PIX2, R10, x-TELEMENT_SPACE*sc, y-TELEMENT_SPACE*sc, z-2, wd+2*TELEMENT_SPACE*sc, ht+2*TELEMENT_SPACE*sc, sc,
                CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND );
        }

        if( mouseLeftDrag(&box) )
        {
            float cursorScale = optionGetf(kUO_CursorScale);
            recipe_drag(to->iCol, icon, NULL, cursorScale ? sc / cursorScale : sc);
            recipeRemoveFromTrade(to->iCol, 0);
        }
    }
}

//
//
static void trade_tempPowElement( void * tpow, float x, float y, float z, float sc, float wd, float ht, int partner )
{
    Entity      *e = playerPtr();
    TradeObj    *to = 0;
    const BasePower   *ppow;
    AtlasTex* icon = NULL;
    CBox        box;

    drawFadeBar( x, y, z, wd, ht, 0xffffff22 );

    font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );

    if( partner )
        ppow = ((Power*)tpow)->ppowBase;
    else
    {
        to = (TradeObj*)tpow;
        // warning! can temp power get used up while trading?
        ppow = e->pchar->ppPowerSets[to->iCol]->ppPowers[to->iRow]->ppowBase;
    }

    if( ppow )
    {
        icon = atlasLoadTexture( ppow->pchIconName );
        if(icon)
            display_sprite( icon, x + (ht - icon->width*sc)/2 + PIX3*2*sc, y + (ht - icon->height*sc)/2, z, sc, sc, CLR_WHITE );
        font( &game_9 );
        font_color( CLR_WHITE, CLR_WHITE );
        cprntEx( x + TELEMENT_TXT_YOFF*sc, y + ht/2, z,sc, sc, CENTER_Y, ppow->pchDisplayName );
    }

    BuildCBox( &box, x - TELEMENT_SPACE*sc, y - TELEMENT_SPACE*sc, wd + 2*TELEMENT_SPACE*sc, ht + 14*sc );

    if( ppow )
        trade_ContextMenu( &box, kTrayItemType_Power, ppow, 0, 0, 0);

    if( !partner )
    {
        if( mouseClickHit( &box, MS_LEFT ))
            tpowRemoveFromTrade( to->iCol, to->iRow );

        if( mouseCollision(&box) )
        {
            if( isDown(MS_LEFT) )
                drawFlatFrame( PIX2, R10, x-TELEMENT_SPACE*sc, y-TELEMENT_SPACE*sc, z-2, wd+2*TELEMENT_SPACE*sc, ht+2*TELEMENT_SPACE*sc, sc,
                CLR_SELECTION_FOREGROUND, CLR_SELECTION_BACKGROUND );
            else
                drawFlatFrame( PIX2, R10, x-TELEMENT_SPACE*sc, y-TELEMENT_SPACE*sc, z-2, wd+2*TELEMENT_SPACE*sc, ht+2*TELEMENT_SPACE*sc, sc,
                CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND );
        }

        if( mouseLeftDrag(&box) )
        {
            tray_dragPower( to->iCol, to->iRow, icon );
            tpowRemoveFromTrade( to->iCol, to->iRow );
        }
    }
}

//
//
static void trade_drawOffer( int partner, float x, float y, float z, float sc, float wd, float ht )
{
    static ScrollBar sb[2] = {0};
    int i, clr = 0x00000088, influence, inspNum, specNum, salvNum, tpowNum, reciNum;
    float ty = y - sb[partner].offset;
    CBox box;
    int itemCount = 0;

    if( partner )
    {
        influence = trade.bInfluence;
        inspNum = trade.bNumInsp;
        specNum = trade.bNumSpec;
        salvNum = trade.bNumSalv;
        tpowNum = trade.bNumTpow;
        reciNum = trade.bNumReci;
    }
    else
    {
        influence = trade.aInfluence;
        inspNum = trade.aNumInsp;
        specNum = trade.aNumSpec;
        salvNum = trade.aNumSalv;
        tpowNum = trade.aNumTpow;
        reciNum = trade.aNumReci;
    }

    BuildCBox( &box, x, y, wd, ht );
    set_scissor( FALSE );
    doScrollBar( &sb[partner], ht-16*sc, (1+inspNum+specNum+salvNum)*TELEMENT_HT*sc - 8*sc,  x + wd + 15*sc, y+0*sc, z, &box, 0  );
    set_scissor( TRUE );

    // influence
    trade_influenceElement( x, ty + TELEMENT_SPACE*sc, z, sc, wd, (TELEMENT_HT-2*TELEMENT_SPACE)*sc, influence );

    // enhancements
    for( i = 0; i < specNum; i++ )
    {
        if( partner )
            trade_enhancementElements( &trade.bSpec[i], x, ty + (TELEMENT_SPACE+(itemCount+1)*TELEMENT_HT)*sc, z, sc, wd, (TELEMENT_HT-2*TELEMENT_SPACE)*sc, partner );
        else
            trade_enhancementElements( &trade.aSpec[i], x, ty + (TELEMENT_SPACE+(itemCount+1)*TELEMENT_HT)*sc, z, sc, wd, (TELEMENT_HT-2*TELEMENT_SPACE)*sc, partner );

        itemCount++;
    }

    // inspirations
    for( i = 0; i < inspNum; i++ )
    {
        if( partner )
             trade_inspirationElement( trade.bInsp[i], x, ty + (TELEMENT_SPACE+(itemCount+1)*TELEMENT_HT)*sc, z, sc, wd, (TELEMENT_HT-2*TELEMENT_SPACE)*sc, partner );
        else
            trade_inspirationElement( &trade.aInsp[i], x, ty + (TELEMENT_SPACE+(itemCount+1)*TELEMENT_HT)*sc, z, sc, wd, (TELEMENT_HT-2*TELEMENT_SPACE)*sc, partner );

        itemCount++;
    }

    // Salvage
    for( i = 0; i < salvNum; i++ )
    {
        if( partner )
            trade_salvageElement( &trade.bSalv[i], x, ty + (TELEMENT_SPACE+(itemCount+1)*TELEMENT_HT)*sc, z, sc, wd, (TELEMENT_HT-2*TELEMENT_SPACE)*sc, partner );
        else
            trade_salvageElement( &trade.aSalv[i], x, ty + (TELEMENT_SPACE+(itemCount+1)*TELEMENT_HT)*sc, z, sc, wd, (TELEMENT_HT-2*TELEMENT_SPACE)*sc, partner );

        itemCount++;
    }

    // Temp powers
    for( i = 0; i < tpowNum; i++ )
    {
        if( partner )
            trade_tempPowElement( &trade.bTpow[i], x, ty + (TELEMENT_SPACE+(itemCount+1)*TELEMENT_HT)*sc, z, sc, wd, (TELEMENT_HT-2*TELEMENT_SPACE)*sc, partner );
        else
             trade_tempPowElement( &trade.aTpow[i], x, ty + (TELEMENT_SPACE+(itemCount+1)*TELEMENT_HT)*sc, z, sc, wd, (TELEMENT_HT-2*TELEMENT_SPACE)*sc, partner );

        itemCount++;
    }

    // Recipes
    for (i = 0; i < reciNum; i++)
    {
        if( partner )
            trade_recipeElement( &trade.bReci[i], x, ty + (TELEMENT_SPACE+(itemCount+1)*TELEMENT_HT)*sc, z, sc, wd, (TELEMENT_HT-2*TELEMENT_SPACE)*sc, partner );
        else
            trade_recipeElement( &trade.aReci[i], x, ty + (TELEMENT_SPACE+(itemCount+1)*TELEMENT_HT)*sc, z, sc, wd, (TELEMENT_HT-2*TELEMENT_SPACE)*sc, partner );

        itemCount++;
    }

    if (!specNum && !inspNum && !salvNum && !tpowNum && !reciNum && !partner)
    {
        font(&game_18);
        font_color(0xffffff22 ,0xffffff22 );
        cprntEx(x+wd/2,y+ht/2,z,sc*1.5,sc*1.5,CENTER_X|CENTER_Y,"DragItemsHere");
    }

    if( !partner && mouseCollision( &box ) )
        clr = 0xffffff11;

    drawFlatFrame( PIX3, R10, x- (6)*sc, y - 20*sc, z -5, wd+(12)*sc, ht + 30*sc, sc, 0, clr );

    if( !partner && cursor.dragging && mouseCollision(&box) && !isDown(MS_LEFT) )
    {
        if( cursor.drag_obj.type == kTrayItemType_Inspiration )
            inspMarkForTrade( playerPtr(), cursor.drag_obj.iset, cursor.drag_obj.ipow ) ;
        else if( cursor.drag_obj.type == kTrayItemType_SpecializationInventory )
            specMarkForTrade( playerPtr(), cursor.drag_obj.ispec ) ;
        else if( cursor.drag_obj.type == kTrayItemType_Salvage )
        {
            Entity *e = playerPtr();
            if (e && e->pchar)
            {
                const SalvageItem *salvage = playerPtr()->pchar->salvageInv[cursor.drag_obj.invIdx]->salvage;
                if ( salvage )
                {
                    if (salvage->flags & SALVAGE_NOTRADE)
                    {
                        addSystemChatMsg( textStd("CannotTradeSalvage"), INFO_USER_ERROR, 0 );
                    } else {
                        amountSliderWindowSetupAndDisplay(1, character_SalvageCount(playerPtr()->pchar, salvage), NULL, 
                                                            amountSliderDrawSalvageCallback, amountSliderOKSalvageCallback, 
                                                            (void *) cursor.drag_obj.invIdx, 1);
                    }
                }
            }
        }
        else if( cursor.drag_obj.type == kTrayItemType_Recipe )
        {
            const DetailRecipe *recipe = detailrecipedict_RecipeFromId(cursor.drag_obj.invIdx);
            if ( recipe )
            {
                if (recipe->flags & RECIPE_NO_TRADE)
                {
                    addSystemChatMsg( textStd("CannotTradeRecipe"), INFO_USER_ERROR, 0 );
                }
                else {
                    amountSliderWindowSetupAndDisplay(1, character_RecipeCount(playerPtr()->pchar, recipe), NULL, 
                                                    amountSliderDrawRecipeCallback, amountSliderOKRecipeCallback, 
                                                    (void *) cursor.drag_obj.invIdx, 0);
                }
            }
        }
        else if( cursor.drag_obj.type == kTrayItemType_Power )
        {
            float sc = 1.f;
            trayslot_Timer( playerPtr(), &cursor.drag_obj, &sc );
            
            // cannot trade active or recharging powers
            if( !trayslot_IsActive(playerPtr(), &cursor.drag_obj) && sc == 1.f )
                tpowMarkForTrade( playerPtr(), cursor.drag_obj.iset, cursor.drag_obj.ipow );
            else
                addSystemChatMsg(textStd("CannotTradeRechargeOrActivePow"), INFO_USER_ERROR, 0);
        }
        trayobj_stopDragging();
    }
}

//-------------------------------------------------------------------------------
// Drawing Functions ////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------

static void trade_drawHeader( float x, float y, float z, float sc, float wd, Entity * e )
{
    char *title;
    float txtWd, iconSc = .5f;
    AtlasTex *icon;

    font( &game_14 );
    font_color( 0x00ebffff, 0x00ebffff );

    if( e == playerPtr() )
        title = textStd( "MyOffer" );
    else if ( e )
        title = textStd( "PartnerOffer", e->name );
    else
    {
        title = textStd( "StatusDisconnected" );
        font_color( 0x888888ff, 0x888888ff );
    }

    // name
    cprnt( x + wd/2, y + 20*sc, z, sc, sc, title );
    txtWd = str_wd( font_grp, sc, sc, title );

    if( !e )
        return;

    // origin/class
    font( &game_9 );
    font_ital( 1 );
    cprnt( x + wd/2, y + 35*sc, z, sc, sc, "OriginClass", e->pchar->porigin->pchDisplayName, e->pchar->pclass->pchDisplayName );
    txtWd = MAX( txtWd, str_wd( font_grp, sc, sc, "OriginClass", e->pchar->porigin->pchDisplayName, e->pchar->pclass->pchDisplayName ) );
    font_ital( 0 );

    // origin icon
    icon = GetOriginIcon( e );
    display_sprite( icon, x + (wd - txtWd)/2 - (10+icon->width*iconSc)*sc, y + 15*sc, z, iconSc*sc, iconSc*sc, CLR_WHITE );

    // archetype icon
    icon = GetArchetypeIcon( e );
    display_sprite( icon, x + (wd + txtWd)/2 + 10*sc, y + 15*sc, z, iconSc*sc, iconSc*sc, CLR_WHITE );

}

static void trade_acceptButton( float x, float y, float z, float sc, float wd, float ht, int color )
{
    font( &game_18 );
    font_color( CLR_WHITE, CLR_WHITE );

    if( trade.aAccepted )
    {
        AtlasTex * arrow = atlasLoadTexture( "trade_readyarrow_L.tga" );

        drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, CLR_GREEN );
        display_sprite( arrow, x + wd - (arrow->width+5)*sc, y + (ht - arrow->height*sc)/2, z+1, sc, sc, CLR_WHITE );
        cprntEx( x + 120*sc, y + ht/2, z+1, sc, sc, CENTER_Y, "TradeOffered" );

        if( D_MOUSEHIT == drawStdButton( x + 50*sc, y + ht/2, z+1, 90*sc, ht - 10*sc, CLR_RED, "CancelString", 2.f*sc, 0 ) )
        {
            trade.aAccepted = 0;
            sendTradeUpdate();
        }
    }
    else
    {
        if( D_MOUSEHIT == drawStdButton( x + wd/2, y + ht/2, z, wd, ht, color, "TradeOffer", 2.f*sc, 0 ) )
        {
            trade.aAccepted = 1;
            sendTradeUpdate();
        }
    }
}

static void trade_partnerStatus( Entity *e, float x, float y, float z, float sc, float wd, float ht, int color )
{
    font( &game_18 );
    font_color( CLR_WHITE, CLR_WHITE );

    if( !e )
    {
        drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, CLR_RED );
        cprntEx( x + wd/2, y + ht/2, z+1, sc, sc, (CENTER_X|CENTER_Y), "NoPartner" );
    }
    else if( trade.bAccepted )
    {
        AtlasTex * arrow = atlasLoadTexture( "trade_readyarrow_R.tga" );

        drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, CLR_GREEN );
        display_sprite( arrow, x + 5*sc, y + (ht - arrow->height*sc)/2, z+1, sc, sc, CLR_WHITE );
        cprntEx( x + wd/2, y + ht/2, z+1, sc, sc, (CENTER_X|CENTER_Y), "AcceptedTrade" );
    }
    else
    {
        drawFrame( PIX3, R10, x, y, z+1, wd, ht, sc, color, CLR_RED );
        cprntEx( x + wd/2, y + ht/2, z+1, sc, sc, (CENTER_X|CENTER_Y), "ConsideringOffer" );
    }
}

//-----------------------------------------------------------------------------
// Master Function ////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------

#define BUT_WD 100
#define BUT_HT 20

int tradeWindow()
{
    float x, y, z, wd, ht, sc, oht;
    int color, bcolor;
    static int init = 0;
    int clr = 0x00000088;
    float ButAreaWd, ButAreaOff;

    Entity *e = playerPtr();
    Entity *partner = entFromDbId(trade.partnerDbid );

    if( !window_getDims( WDW_TRADE, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor) ) 
    {
        uiEditReturnFocus(sInfEdit);
        return 0;
    }
    oht = ht - (INV_HT+PIX3)*sc;
    // make trade window mutually exvlusive with store 
    // (would've made store window loser, but there is already som crazy logic 
    //  dealing with contacts I didn't want to break)
    if( window_getMode(WDW_STORE) != WINDOW_DOCKED  || 
        window_getMode(WDW_BASE_STORAGE) != WINDOW_DOCKED  )
    {
        sendTradeCancel( TRADE_FAIL_OTHER_LEFT );
        window_setMode( WDW_TRADE, WINDOW_DOCKED );
        return 0;
    }

    if( !partner )
        sendTradeCancel( TRADE_FAIL_OTHER_LEFT );

    if( !init )
    {
        gTradeContext = contextMenu_Create( NULL );
        contextMenu_addVariableTitle( gTradeContext, trade_PowerText, 0);
        contextMenu_addVariableText( gTradeContext, trade_PowerInfo, 0);
        contextMenu_addCode( gTradeContext, alwaysAvailable, 0, trade_Info, 0, "CMInfoString", 0  );
    }

    // first draw background
    drawFrame(PIX3,R10, x, y +oht-PIX3*sc, z, wd, ht-oht+PIX3*sc, sc, color, bcolor );
    drawFrame( PIX3, R10, x, y, z, wd, oht, sc, color, bcolor );

    
    // inventory items
    //-----------------------------------------------------------------------------------
    updateInspirationTray( e, e->pchar->iNumInspirationCols, e->pchar->iNumInspirationRows);

    trade_drawInfluenceInventory( x + PIX3*3*sc, y + oht, z, sc, wd-((PIX3+INV_BORDER)*2*sc), (ht-oht-(PIX3*3)*sc)/2, color );

    font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );
    cprntEx( x + wd/2 + 110*sc, y + oht + (PIX3*5)*sc, z, sc, sc, 0, "%s", itoa_with_commas_static(e->pchar->iInfluencePoints) );
    cprntEx( x + wd/2 + 20*sc, y + oht + (PIX3*5)*sc, z, sc, sc, 0,"MyInfluence" );

    if( D_MOUSEHIT == drawStdButton(x + wd - (100/2+PIX3)*sc, y + ht - (INV_HT/2+PIX3)*sc, z, 100*sc, 30*sc, CLR_DARK_RED, "CancelString", 1.4*sc, 0 ) )
    {
        sendTradeCancel( TRADE_FAIL_I_LEFT );
        window_setMode( WDW_TRADE, WINDOW_SHRINKING );
    }

    ButAreaOff = x + (BUT_WD/2 + PIX3)*sc;
    ButAreaWd = (OFFER_WD - BUT_WD - PIX3*2)*sc;

    if( D_MOUSEHIT == drawStdButton(ButAreaOff, y + ht - (BUT_HT/2+PIX3)*sc, z, BUT_WD*sc, BUT_HT*sc, color, "InspirationTab", 1.0*sc, 0 ) )
        window_openClose(WDW_INSPIRATION);

    if( D_MOUSEHIT == drawStdButton(ButAreaWd/2+ ButAreaOff, y + ht - (BUT_HT/2+PIX3)*sc, z, BUT_WD*sc, BUT_HT*sc, color, "ManageTab", 1.0*sc, 0 ) )
        window_openClose(WDW_ENHANCEMENT);

    if( D_MOUSEHIT == drawStdButton(1.0*ButAreaWd + ButAreaOff, y + ht - (BUT_HT/2+PIX3)*sc, z, BUT_WD*sc, BUT_HT*sc, color, "UiInventoryTypeSalvage", 1.0*sc, 0 ) )
        window_openClose(WDW_SALVAGE);

    if( D_MOUSEHIT == drawStdButton(1.5*ButAreaWd+ ButAreaOff, y + ht - (BUT_HT/2+PIX3)*sc, z, BUT_WD*sc, BUT_HT*sc, color, "UiInventoryTypeRecipe", 1.0*sc, 0 ) )
        window_openClose(WDW_RECIPEINVENTORY);

    // offers
    trade_drawHeader( x, y, z, sc, OFFER_WD*sc, e );
    trade_drawHeader( x + wd - OFFER_WD*sc, y, z, sc, OFFER_WD*sc, partner );

    set_scissor( TRUE );
    scissor_dims( x + PIX3*sc, y + (BUTTON_OFFSET+BUTTON_HT-PIX3)*sc, (OFFER_WD-2*PIX3)*sc, oht-(BUTTON_OFFSET+BUTTON_HT)*sc );
    trade_drawOffer( 0, x + 15*sc, y + (BUTTON_OFFSET+BUTTON_HT)*sc, z+1, sc, (OFFER_WD-30)*sc, oht-(BUTTON_OFFSET+BUTTON_HT)*sc );
    scissor_dims( x + wd + (PIX3-OFFER_WD)*sc, y + (BUTTON_OFFSET+BUTTON_HT-PIX3)*sc, (OFFER_WD-2*PIX3)*sc, oht-(BUTTON_OFFSET+BUTTON_HT)*sc );
    trade_drawOffer( 1, x + wd + (15-OFFER_WD)*sc, y + (BUTTON_OFFSET+BUTTON_HT)*sc, z+1, sc, (OFFER_WD-30)*sc, oht-(BUTTON_OFFSET+BUTTON_HT)*sc );

    set_scissor( FALSE );

    trade_acceptButton( x, y + BUTTON_OFFSET*sc, z+5, sc, OFFER_WD*sc, BUTTON_HT*sc, color );
    trade_partnerStatus( partner, x + wd - OFFER_WD*sc, y + BUTTON_OFFSET*sc, z+5, sc, OFFER_WD*sc, BUTTON_HT*sc, color );
    return 0;
}



