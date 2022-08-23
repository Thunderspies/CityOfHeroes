
#include "gameComm/wdwbase.h"
#include "bases/DetailRecipe.h"
#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiInput.h"
#include "uiCombineSpec.h"
#include "UI/uiRecipeInventory.h"
#include "UI/uiEnhancement.h"
#include "UI/uiScrollBar.h"
#include "UI/uiNet.h"
#include "UI/uiContextMenu.h"
#include "UI/uiInfo.h"
#include "UI/uiGame.h"
#include "UI/uiDialog.h"
#include "UI/uiOptions.h"
#include "uiSalvage.h"
#include "sound/sound.h"

#include "graphics/ttFont.h"
#include "graphics/ttFontUtil.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"

#include "entity/entclient.h"
#include "entity/entity.h"
#include "player/player.h"
#include "entity/character_base.h"
#include "entity/powers.h"
#include "gameData/store.h"
#include "assert.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/utils/timing.h>
#include "entity/character_inventory.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>

#define STORE_DIVIDE 4
#define STORE_FOOTER 50
#define STORE_HEADER 60

#define TITLE_INSET  10
#define ITEM_INSET   20
#define ITEM_HT      34
#define ITEM_SPACE   10

#define INV_PLAYER   1
#define INV_STORE    2

static int restoreContact = 0;

typedef struct storeItem
{
    int available;
    int selected;
    int inventory;
    int type;
    int i;
    int j;

    const StoreItem *psi;
    int price;

    const DetailRecipe *pRec;        // points to recipe being sold if this is a recipe, NULL otherwise
    const SalvageItem *pSal;        // points to salvage being sold if this is a salvage, NULL otherwise
} storeItem;

static storeItem gSelectedItem = {0};

static MultiStore s_MultiStore;

static ContextMenu *s_menuSell = 0;
static ContextMenu *s_menuBuy = 0;

int isStoreOpen()
{
    return (s_MultiStore.idNPC != 0);
}

// Unilaterally forget that we had a contact dialog open
void storeDisableRestoreContact()
{
    restoreContact = 0;
}

//
//
void menu_Info(void *data)
{
    Entity *e = playerPtr();

    if(gSelectedItem.psi)
    {
        if(gSelectedItem.type == kPowerType_Boost)
            info_CombineSpec(gSelectedItem.psi->ppowBase, 1, gSelectedItem.psi->power.level, 0);
        else
            info_Power(gSelectedItem.psi->ppowBase, 0);
    }
    else
    {
        if (gSelectedItem.inventory==INV_PLAYER)
        {
            const BasePower *ppowBase;
            if (gSelectedItem.type == kPowerType_Boost)
            {
                int iLevel = 0;

                if (e->pchar->aBoosts[gSelectedItem.j])
                {
                    ppowBase = e->pchar->aBoosts[gSelectedItem.j]->ppowBase;
                    iLevel = e->pchar->aBoosts[gSelectedItem.j]->iLevel;

                    info_CombineSpec(ppowBase, 1, iLevel, 0);
                }
            }
            else if (gSelectedItem.type == kPowerType_Inspiration)
            {
                ppowBase = e->pchar->aInspirations[gSelectedItem.i][gSelectedItem.j];
                if(ppowBase)
                {
                    info_Power(ppowBase, 0);
                }
            }
            else if (gSelectedItem.type == kPowerType_Count)
            {
                // used for salvage and recipes
                if (gSelectedItem.pSal)
                {
                    info_Salvage(gSelectedItem.pSal);
                }
                else if (gSelectedItem.pRec)
                {
                    // display recipe information here
                    info_Recipes(gSelectedItem.pRec);
                }
            }
        }
    }
}

void uiStore_DrawName(TTDrawContext *pfont, int cx, int cy, int cz, int width, const char *pch, float sc)
{
    int rgba[4];
    float tmpWd;
    char *pchTmp, *pchSplit;

    pchTmp = _alloca(strlen(textStd(pch))+1);
    strcpy(pchTmp, textStd(pch));

    tmpWd = str_wd_notranslate(pfont, sc, sc, pchTmp);
    pchSplit = pchTmp+strlen(pchTmp)-1;
    if (tmpWd > width)
    {
        while( pchSplit > pchTmp)
        {
            while(*pchSplit!=' ' && pchSplit > pchTmp)
            {
                pchSplit--;
            }

            if (pchSplit > pchTmp)
            {
                *pchSplit = 0;
                *pchSplit++;
            }

            // Not cprnt since we've already translated
            if (str_wd_notranslate(pfont, sc, sc, pchTmp) <= width)
            {
                determineTextColor(rgba);
                printBasic(font_grp, cx, cy-5*sc, cz, sc, sc, CENTER_Y | NO_MSPRINT, pchTmp, strlen(pchTmp), rgba);
                printBasic(font_grp, cx, cy+5*sc, cz, sc, sc, CENTER_Y | NO_MSPRINT, pchSplit, strlen(pchSplit), rgba);
                break;
            } else if ( pchSplit > pchTmp ) {
                *pchSplit--;
                *pchSplit = ' ';
                *pchSplit--;
            }
        }

        // fail case
        if (pchSplit <= pchTmp)
            cprntEx(cx, cy, cz, sc, sc, CENTER_Y | NO_MSPRINT, pchTmp);
    }
    else
    {
        cprntEx(cx, cy, cz, sc, sc, CENTER_Y | NO_MSPRINT, pchTmp);
    }
}

int menu_Available(void *data)
{
    if(gSelectedItem.available)
        return CM_AVAILABLE;

    return CM_VISIBLE;
}

int menu_SellStack(void *data)
{
    if(gSelectedItem.pRec != NULL || gSelectedItem.pSal != NULL)
        return CM_AVAILABLE;

    return CM_VISIBLE;
}


//------------------------------------------------------------
// helper to find the next inspiration in the pchar inspirations.
// -AB: created :2005 Feb 01 03:34 PM
//----------------------------------------------------------
static bool next_inspiration( int *res_i, int *res_j, Entity *e, int istart, int jstart)
{
    int i = istart, j = jstart - 1;
    assert(e && e->pchar );
     for(; i < e->pchar->iNumInspirationCols; i++ )
    {
        for(; j >= 0; j-- )
        {
            if( e->pchar->aInspirations[i][j] )
            {
                if( res_i ) *res_i = i;
                if( res_j ) *res_j = j;
                return true;
            }
        }
        j = e->pchar->iNumInspirationRows - 1;
    }

    return false;
}


//------------------------------------------------------------
// helper for iterating over the enhancements.
// -AB: created :2005 Feb 01 03:59 PM
//----------------------------------------------------------
static bool next_enhancement( int *res_i, int *res_j, Entity *e, int i, int j )
{
    assert(e && e->pchar);

    for(i = i + 1; i < CHAR_BOOST_MAX; i++ )
    {
        if( e->pchar->aBoosts[i] )
        {
            // set both to the same
            if( res_i ) *res_i = i;
            if( res_j ) *res_j = i;
            return true;            
        }
    }
    return false;
}


//------------------------------------------------------------
// find the previous enhancement, if any
// -AB: created :2005 Feb 01 04:18 PM
//----------------------------------------------------------
static bool prev_enhancement( int *res_i, int *res_j, Entity *e, int i, int j )
{
    assert(e && e->pchar);

    while( --i >= 0 )
    {
        if( e->pchar->aBoosts[i] )
        {
            // set both to the same
            if( res_i ) *res_i = i;
            if( res_j ) *res_j = i;
            return true;            
        }
    }
    return false;
}

//------------------------------------------------------------
// select an enhancement. used for when a player has sold
//  an enhancement.
// -AB: created :2005 Feb 01 04:19 PM
//----------------------------------------------------------
static bool select_enhancement( int *res_i, int *res_j, Entity *e, int i, int j )
{
    //try next first
    return next_enhancement( res_i, res_j, e, i, j ) || prev_enhancement( res_i, res_j, e, i, j );
}

static SalvageItem *s_DeleteMe = 0;

static void salvage_sell(void * data)
{
    if( s_DeleteMe )
    {
        int stack = data?1:0;
        
        if (stack)
            store_SendBuySalvage(&s_MultiStore, s_DeleteMe->salId, character_SalvageCount(playerPtr()->pchar, s_DeleteMe));
        else 
            store_SendBuySalvage(&s_MultiStore, s_DeleteMe->salId, 1);

        if (stack || character_SalvageCount(playerPtr()->pchar, gSelectedItem.pSal) < 2 )
        {
            const SalvageItem    *pSal = NULL;
            int                    id = 0;
            int                    count = character_GetInvSize(playerPtr()->pchar, kInventoryType_Salvage);
            int                    idx = gSelectedItem.i + 1;

            while (pSal == NULL && idx >= 0)
            {
                if (idx >= count)
                    idx = gSelectedItem.i - 1;

                character_GetInvInfo(playerPtr()->pchar, &id, NULL, NULL, kInventoryType_Salvage, idx);
                pSal = salvage_GetItemById(id);
                if (pSal)
                {
                    // select this recipe    
                    gSelectedItem.i = idx;
                    gSelectedItem.pSal = pSal;
                } 
                else if (idx > gSelectedItem.i)
                {
                    idx++;
                }
                else 
                {
                    idx--;
                }
            }
        }
    }
}

//------------------------------------------------------------
// called when an item is bought or sold
// -AB: refactored to select next item. :2005 Feb 01 03:13 PM
//----------------------------------------------------------
void menu_BuyOrSell(void *data)
{
    int stack = (int) data;

    if(gSelectedItem.inventory==INV_PLAYER)
    {
        if (gSelectedItem.psi != NULL)
        {
            store_SendBuyItem(&s_MultiStore, gSelectedItem.type, gSelectedItem.j, gSelectedItem.i);
        }
        else if (gSelectedItem.pSal) 
        {
            Entity * e = playerPtr();
            DetailRecipe **ppRecipes=0;

            if( !optionGet(kUO_HideUsefulSalvagePrompt) && character_IsSalvageUseful(e->pchar,gSelectedItem.pSal,&ppRecipes) )
            {
                int i;
                char * sal_string=0;
                estrConcatf(&sal_string, "%s<br><br>%s<br>", textStd("SellUsefulSalvage"), textStd("SalvageDropUseful") );
                for( i = 0; i < eaSize(&ppRecipes); i++ )
                    estrConcatf(&sal_string, "%s<br>", textStd(ppRecipes[i]->ui.pchDisplayName) );
                eaDestroy(&ppRecipes);

                dialogRemove(0,salvage_sell,0);
                s_DeleteMe = (SalvageItem*)gSelectedItem.pSal;
                dialog( DIALOG_YES_NO, -1, -1, -1, -1, sal_string, NULL, salvage_sell, NULL, NULL,0,sendOptions,deleteUsefulDCB,1,0,0, (void*)stack );
                estrDestroy(&sal_string);
            }
            else
            {
                if (stack)
                    store_SendBuySalvage(&s_MultiStore, gSelectedItem.pSal->salId, character_SalvageCount(playerPtr()->pchar, gSelectedItem.pSal));
                else 
                    store_SendBuySalvage(&s_MultiStore, gSelectedItem.pSal->salId, 1);
            }
        } 
        else if (gSelectedItem.pRec) 
        {
            if (stack)
                store_SendBuyRecipe(&s_MultiStore, gSelectedItem.pRec->id, character_RecipeCount(playerPtr()->pchar, gSelectedItem.pRec));
            else 
                store_SendBuyRecipe(&s_MultiStore, gSelectedItem.pRec->id, 1);
        }
        if( gSelectedItem.selected )
        {
            switch (gSelectedItem.type)
            {
                case kPowerType_Boost:
                    select_enhancement(&gSelectedItem.i, &gSelectedItem.j, playerPtr(), gSelectedItem.i, gSelectedItem.j);
                    break;
                case kPowerType_Inspiration:
                    next_inspiration(&gSelectedItem.i, &gSelectedItem.j, playerPtr(), gSelectedItem.i, gSelectedItem.j);
                    break;
                case kPowerType_Count:
                    // could be salvage or recipes
                    if (gSelectedItem.pSal )
                    {            
                        Entity * e = playerPtr();
                        DetailRecipe **ppRecipes=0;

                        if ( (stack || character_SalvageCount(playerPtr()->pchar, gSelectedItem.pSal) < 2) && !(!optionGet(kUO_HideUsefulSalvagePrompt) && character_IsSalvageUseful(e->pchar,gSelectedItem.pSal,&ppRecipes)))
                        {
                            const SalvageItem    *pSal = NULL;
                            int                    id = 0;
                            int                    count = character_GetInvSize(playerPtr()->pchar, kInventoryType_Salvage);
                            int                    idx = gSelectedItem.i + 1;

                            while (pSal == NULL && idx >= 0)
                            {
                                if (idx >= count)
                                    idx = gSelectedItem.i - 1;

                                character_GetInvInfo(playerPtr()->pchar, &id, NULL, NULL, kInventoryType_Salvage, idx);
                                pSal = salvage_GetItemById(id);
                                if (pSal)
                                {
                                    // select this recipe    
                                    gSelectedItem.i = idx;
                                    gSelectedItem.pSal = pSal;
                                } 
                                else if (idx > gSelectedItem.i)
                                {
                                    idx++;
                                }
                                else 
                                {
                                    idx--;
                                }
                            }
                        }
                    }     
                    else if (gSelectedItem.pRec)
                    {
                        const DetailRecipe    *pRec = NULL;
                        int                id = 0;
                        int                count = character_GetInvSize(playerPtr()->pchar, kInventoryType_Recipe);
                        int                idx = gSelectedItem.i + 1;
                    
                        if (stack || character_RecipeCount(playerPtr()->pchar,gSelectedItem.pRec) < 2)
                        {
                            while (pRec == NULL && idx >= 0)
                            {
                                if (idx >= count)
                                    idx = gSelectedItem.i - 1;

                                character_GetInvInfo(playerPtr()->pchar, &id, NULL, NULL, kInventoryType_Recipe, idx);
                                pRec = recipe_GetItemById(id);
                                if (pRec)
                                {
                                    // select this recipe    
                                    gSelectedItem.i = idx;
                                    gSelectedItem.pRec = pRec;
                                } 
                                else if (idx > gSelectedItem.i)
                                {
                                    idx++;
                                }
                                else 
                                {
                                    idx--;
                                }
                            }
                        }
                    }
                    break;
            }
        }
        
    }
    else
    {
        if (gSelectedItem.psi != NULL)
        {
            store_SendSellItem(&s_MultiStore, gSelectedItem.psi->pchName);
        }
    }
}



//
//
static void store_drawPower( const BasePower * ppow, float x, float y, float z, float sc, float wd, float ht, int i, int j, int price, int inventory, const StoreItem *psi, bool bAvailable, int type )
{ 
    AtlasTex     *icon;
    CBox        box;
    bool        bSelect = false;

    drawFadeBar( x+10*sc, y, z, wd-10*sc, ht, 0xffffff22 );

    font( &game_9 );

    if( ppow )
    {
        int color = CLR_WHITE;
        if(!bAvailable)
        {
            color = CLR_GREY;
        }

        icon = atlasLoadTexture( ppow->pchIconName );
        display_sprite( icon, x + (ht - icon->width*.75f*sc)/2 + PIX3*2*sc, y + (ht - icon->height*.75f*sc)/2, z,.75f*sc,.75f*sc, color );

        font_color( color, color );
        uiStore_DrawName(&game_9, x + ITEM_HT*sc, y + ht/2, z, wd - 125*sc, ppow->pchDisplayName, sc); 

        if(price>=0)
        {
            int swd = str_wd(&game_9, sc, sc, textStd("InfluenceWithCommas", price));
            cprntEx( x + wd  - swd - (ITEM_SPACE+PIX3)*sc, y + ht/2, z, sc, sc, CENTER_Y | NO_MSPRINT, textStd("InfluenceWithCommas", price) );
        }
        else if(inventory==INV_PLAYER)
        {
            cprntEx( x + wd - str_wd(font_grp, sc, sc, "CantSellHere") - (ITEM_SPACE+PIX3)*sc, y + ht/2, z, sc, sc, CENTER_Y, "CantSellHere");
        }
    }

    BuildCBox( &box, x - ITEM_SPACE*sc, y - ITEM_SPACE*sc, wd + 2*ITEM_SPACE*sc, ht + 14*sc );

    if( mouseCollision(&box) )
    {
        drawFlatFrame( PIX2, R10, x-ITEM_SPACE*sc, y-PIX3*sc, z-2, wd, ht+2*PIX3*sc, sc, CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND );
        if(mouseRightClick())
        {
            bSelect = true;
        }
    }

    if( bSelect || mouseClickHit( &box, MS_LEFT ))
    {
        gSelectedItem.available = bAvailable;
        gSelectedItem.type = type;
        gSelectedItem.i = i;
        gSelectedItem.j = j;
        gSelectedItem.selected = 1;
        gSelectedItem.inventory = inventory;
        gSelectedItem.psi = psi;
        gSelectedItem.price = price;
        gSelectedItem.pSal = NULL;
        gSelectedItem.pRec = NULL;

        if( bSelect )
        {
            if(inventory==INV_PLAYER)
                contextMenu_display(s_menuSell);
            else
                contextMenu_display(s_menuBuy);
        }
    }

    if( gSelectedItem.selected && gSelectedItem.inventory == inventory && gSelectedItem.type == type &&
        gSelectedItem.i == i && gSelectedItem.j == j )
    {
        drawFlatFrame( PIX2, R10, x-ITEM_SPACE, y-PIX3, z-2, wd, ht+2*PIX3, 1.f, CLR_SELECTION_FOREGROUND, CLR_SELECTION_BACKGROUND );
    }
}

//
//
static void store_drawEnhancement( const BasePower * ppow, float x, float y, float z, float sc, float wd, float ht, int i, int iLevel, int price, int inventory, const StoreItem *psi, bool bAvailable)
{
    AtlasTex *icon;
    CBox box;
    bool bSelect = false;

    drawFadeBar( x+10*sc, y, z, wd-10*sc, ht, 0xffffff22 );

    font( &game_9 );

    if( ppow )
    {
        int color = CLR_WHITE;
        uiEnhancement *pEnh = uiEnhancementCreate(ppow->pchFullName, iLevel);

        if(!bAvailable)
        {
            color = CLR_GREY;
        }
        uiEnhancementSetColor(pEnh, color);

         icon = uiEnhancementDraw( pEnh, x + ht/2 + PIX3*2*sc, y + ht/2, z, .5*sc, sc, MENU_GAME, WDW_STORE, true);
        //         icon = drawEnhancementOriginLevel( ppow, iLevel+1, 0, true, x + ht/2 + PIX3*2, y + ht/2, z, .5*sc, sc, color, false );
 
        font( &game_9 ); 
        font_color( color, color );
        uiStore_DrawName(&game_9, x + ITEM_HT*sc + PIX3*1*sc, y + ht/2, z, wd - 125*sc, ppow->pchDisplayName, sc); 

        uiEnhancementFree(&pEnh);

        if(price>=0)
        {
            int swd = str_wd(&game_9, sc, sc, textStd("InfluenceWithCommas", price));
            cprntEx( x + wd  - swd - (ITEM_SPACE+PIX3)*sc, y + ht/2, z, sc, sc, CENTER_Y | NO_MSPRINT, textStd("InfluenceWithCommas", price) );
        }
        else if(inventory==INV_PLAYER)
        {
            cprntEx( x + wd - str_wd(font_grp, sc, sc, "CantSellHere") - (ITEM_SPACE+PIX3)*sc, y + ht/2, z, sc, sc, CENTER_Y, "CantSellHere");
        }
    }

     BuildCBox( &box, x - ITEM_SPACE*sc, y - ITEM_SPACE*sc, wd + 2*ITEM_SPACE*sc, ht + 14*sc );

    if( mouseCollision(&box) )
    {
        drawFlatFrame( PIX2, R10, x-ITEM_SPACE*sc, y-PIX3*sc, z-2, wd, ht+2*PIX3*sc, sc, CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND );
        if(mouseRightClick())
        {
            bSelect = true;
        }
    }

    if( bSelect || mouseClickHit( &box, MS_LEFT ))
    {
        gSelectedItem.available = bAvailable;
        gSelectedItem.selected = 1;
        gSelectedItem.i = i;
        gSelectedItem.j = i;
        gSelectedItem.type = kPowerType_Boost;
        gSelectedItem.inventory = inventory;
        gSelectedItem.psi = psi;
        gSelectedItem.price = price;
        gSelectedItem.pSal = NULL;
        gSelectedItem.pRec = NULL;

        if( bSelect )
        {
            if(inventory==INV_PLAYER)
            {
                contextMenu_display(s_menuSell);
            }
            else
            {
                contextMenu_display(s_menuBuy);
            }
        }
    }

    if( gSelectedItem.selected && gSelectedItem.inventory == inventory &&
         gSelectedItem.type == kPowerType_Boost && gSelectedItem.i == i )
    {
        drawFlatFrame( PIX2, R10, x-ITEM_SPACE*sc, y-PIX3*sc, z-2, wd, ht+2*PIX3*sc, sc, CLR_SELECTION_FOREGROUND, CLR_SELECTION_BACKGROUND );
    }
}


//
//
static void store_drawRecipe( const DetailRecipe *pRec, float x, float y, float z, float sc, float wd, float ht, int price, int count, int idx)
{
    AtlasTex *icon;
    CBox box;
    bool bSelect = false;

    drawFadeBar( x+10*sc, y, z, wd-10*sc, ht, 0xffffff22 );

    font( &game_9 );

    if( pRec )
    {
        int color = CLR_WHITE;
        if(price <= 0)
        {
            color = CLR_GREY;
        }
        // --------------------
        // print the amount

        font( &game_9 );
        font_color(0x00deffff, 0x00deffff);
        prnt( x + -4*sc, y + 6*sc, z+1.0, sc, sc, "%d", count );

        icon = uiRecipeDrawIcon(pRec, pRec->level, x - ht/2 + PIX3*2*sc, y - ht/2 + PIX3*1*sc, z, sc*0.6f, WDW_STORE, true, CLR_WHITE); 

        font_color( color, color );
//        cprntEx( x + ITEM_HT*sc, y + ht/2, z, sc, sc, CENTER_Y, pRec->ui.pchDisplayName );
        uiStore_DrawName(&game_9, x + (ITEM_HT*1.5f)*sc, y + ht/2, z, wd - 175*sc, pRec->ui.pchDisplayName, sc);  


        if (price > 0)
        {
            int swd = str_wd(&game_9, sc, sc, textStd("InfluenceWithCommas", price));
            cprntEx( x + wd  - swd - (ITEM_SPACE+PIX3)*sc, y + ht/2, z, sc, sc, CENTER_Y | NO_MSPRINT, textStd("InfluenceWithCommas", price) );
        }
        else
        {
            cprntEx( x + wd - str_wd(font_grp, sc, sc, "CantSellHere") - (ITEM_SPACE+PIX3)*sc, y + ht/2, z, sc, sc, CENTER_Y, "CantSellHere");
        }
    }

     BuildCBox( &box, x - ITEM_SPACE*sc, y - ITEM_SPACE*sc, wd + 2*ITEM_SPACE*sc, ht + 14*sc );

    if( mouseCollision(&box) )
    {
        drawFlatFrame( PIX2, R10, x-ITEM_SPACE*sc, y-PIX3*sc, z-2, wd, ht+2*PIX3*sc, sc, CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND );
        if(mouseRightClick())
        {
            bSelect = true;
        }
    }

    if( bSelect || mouseClickHit( &box, MS_LEFT ))
    {
        if (price > 0)
            gSelectedItem.available = 1;
        else 
            gSelectedItem.available = 0;
        gSelectedItem.selected = 1;
        gSelectedItem.i = idx;
        gSelectedItem.j = 0;
        gSelectedItem.type = kPowerType_Count;
        gSelectedItem.inventory = INV_PLAYER;
        gSelectedItem.psi = NULL;
        gSelectedItem.price = price;
        gSelectedItem.pSal = NULL;
        gSelectedItem.pRec = pRec;

        if( bSelect )
        {
            contextMenu_display(s_menuSell);
        }
    }

    if( gSelectedItem.selected && gSelectedItem.pRec && gSelectedItem.pRec->id ==  pRec->id)
    {
        drawFlatFrame( PIX2, R10, x-ITEM_SPACE*sc, y-PIX3*sc, z-2, wd, ht+2*PIX3*sc, sc, CLR_SELECTION_FOREGROUND, CLR_SELECTION_BACKGROUND );
    }
}

//
//
void store_drawSalvage( const SalvageItem *pSal, float x, float y, float z, float sc, float wd, float ht, int price, int count, int idx)
{
    AtlasTex *icon;
    CBox box;
    bool bSelect = false;

    drawFadeBar( x+10*sc, y, z, wd-10*sc, ht, 0xffffff22 );

    font( &game_9 );

    if( pSal )
    {
        int color = CLR_WHITE;
        if(price <= 0)
        {
            color = CLR_GREY;
        }


        // --------------------
        // print the amount

        font( &game_9 );
        font_color(0x00deffff, 0x00deffff);
        prnt( x + -4*sc, y + 6*sc, z+1.0, sc, sc, "%d", count );

        // --------------------
        // draw the icon

        icon = atlasLoadTexture(pSal->ui.pchIcon);

        // draw the icon
        if( icon )
        {
            display_sprite( icon, x , y - PIX3*sc, z, sc*0.5, sc*0.5, color );
        }

        font_color( color, color );
        uiStore_DrawName(&game_9, x + ITEM_HT*sc, y + ht/2, z, wd - 125*sc, pSal->ui.pchDisplayName, sc); 


        if(price > 0)
        {
            int swd = str_wd(&game_9, sc, sc, textStd("InfluenceWithCommas", price));
            cprntEx( x + wd  - swd - (ITEM_SPACE+PIX3)*sc, y + ht/2, z, sc, sc, CENTER_Y | NO_MSPRINT, textStd("InfluenceWithCommas", price) );
        }
        else
        {
            cprntEx( x + wd - str_wd(font_grp, sc, sc, "CantSellHere") - (ITEM_SPACE+PIX3)*sc, y + ht/2, z, sc, sc, CENTER_Y, "CantSellHere");
        }
    }

    BuildCBox( &box, x - ITEM_SPACE*sc, y - ITEM_SPACE*sc, wd + 2*ITEM_SPACE*sc, ht + 14*sc );

    if( mouseCollision(&box) )
    {
        drawFlatFrame( PIX2, R10, x-ITEM_SPACE*sc, y-PIX3*sc, z-2, wd, ht+2*PIX3*sc, sc, CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND );
        if(mouseRightClick())
        {
            bSelect = true;
        }
    }

    if( bSelect || mouseClickHit( &box, MS_LEFT ))
    {
        
        if (price > 0)
            gSelectedItem.available = 1;
        else 
            gSelectedItem.available = 0;
        gSelectedItem.selected = 1;
        gSelectedItem.i = idx;
        gSelectedItem.j = 0;
        gSelectedItem.type = kPowerType_Count;
        gSelectedItem.inventory = INV_PLAYER;
        gSelectedItem.psi = NULL;
        gSelectedItem.price = price;
        gSelectedItem.pSal = pSal;
        gSelectedItem.pRec = NULL;

        if( bSelect )
        {
            contextMenu_display(s_menuSell);
        }
    }

    if( gSelectedItem.selected && gSelectedItem.pSal && gSelectedItem.pSal->salId ==  pSal->salId)
    {
        drawFlatFrame( PIX2, R10, x-ITEM_SPACE*sc, y-PIX3*sc, z-2, wd, ht+2*PIX3*sc, sc, CLR_SELECTION_FOREGROUND, CLR_SELECTION_BACKGROUND );
    }
}

int store_willBuyInventoryEnhancement( int idx )
{
    MultiStoreIter *piter = multistoreiter_Create(&s_MultiStore);
    const StoreItem * si;
    float f, retVal = 0;
    Entity *e = playerPtr();

    if ( !piter )
        return 0;

    si = multistoreiter_GetBuyItem(piter, e->pchar->aBoosts[idx]->ppowBase, e->pchar->aBoosts[idx]->iLevel, &f);

    if( si )
        retVal = MAX(ceil(si->iBuy*f),0);

    multistoreiter_Destroy(piter);
    return 0;
}

void store_SellEnhancement( int idx )
{
    MultiStoreIter *piter = multistoreiter_Create(&s_MultiStore);
    const StoreItem * si;
    float f, cost = 0;
    Entity *e = playerPtr();

    if ( !piter )
        return;

    si = multistoreiter_GetBuyItem(piter, e->pchar->aBoosts[idx]->ppowBase, e->pchar->aBoosts[idx]->iLevel, &f);
    multistoreiter_Destroy(piter);

    if( si )
        cost = MAX(ceil(si->iBuy*f),0);
    
    if( !si || cost < 0 )
        return;

    gSelectedItem.available = 1;
    gSelectedItem.selected = 1;
    gSelectedItem.i = idx;
    gSelectedItem.j = idx;
    gSelectedItem.type = kPowerType_Boost;
    gSelectedItem.inventory = INV_PLAYER;
    gSelectedItem.psi = si;
    gSelectedItem.pSal = NULL;
    gSelectedItem.pRec = NULL;
    menu_BuyOrSell(NULL);    
}

int store_drawPlayerInventory( float x, float y, float z, float sc, float wd, float ht, int numInsp, int numSpec, bool bInspFull, bool bSpecFull,float realy )
{
    Entity *e = playerPtr();
    int i, j;
    float ty = y;
    MultiStoreIter *piter;

    // first display the title
    piter = multistoreiter_Create(&s_MultiStore);
    if ( !piter )
        return 0;

    // then the enhancement inventory
     drawFadeBar( x, y, z, wd - TITLE_INSET*2*sc, (ITEM_HT-10)*sc, 0xffffff22 );
    font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );
    prnt( x + 10*sc, y+18*sc, z, sc, sc, "EnhancementTitle" );
    if(bSpecFull)
        font_color( CLR_RED, CLR_RED );
    prnt( x + wd - 60*sc, y+18*sc, z, sc, sc, "(%d/%d)", numSpec, e->pchar->iNumBoostSlots );

    for( i = -1; next_enhancement( &i, &i, e, i, i ); )
    {
        assert( e->pchar->aBoosts[i] );
        {
            float f, cost;
            const StoreItem * si = multistoreiter_GetBuyItem(piter, e->pchar->aBoosts[i]->ppowBase, e->pchar->aBoosts[i]->iLevel, &f);

            if( si )
                cost = ceil(si->iBuy*f);
            else
                cost = -1;

            y += ITEM_HT*sc;
            if (y < realy + ht && y + (ITEM_HT*sc) > realy)
            {
                store_drawEnhancement( e->pchar->aBoosts[i]->ppowBase, x + ITEM_INSET*sc, y, z+3, sc, wd - (ITEM_INSET+TITLE_INSET)*sc, (ITEM_HT-10)*sc, i, e->pchar->aBoosts[i]->iLevel, cost, INV_PLAYER, si, cost>=0 );
            }
        }
    }

    // now the inspiration inventory
       y += (ITEM_HT+10)*sc;

    drawFadeBar( x, y, z, wd - TITLE_INSET*2*sc, (ITEM_HT-10)*sc, 0xffffff22 );
    font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );
    prnt( x+10*sc, y+18*sc, z, sc, sc, "InspirationTitle" );
    if(bInspFull)
        font_color( CLR_RED, CLR_RED );
    prnt( x + wd - 60*sc, y+18*sc, z, sc, sc, "(%d/%d)", numInsp, e->pchar->iNumInspirationCols*e->pchar->iNumInspirationRows  );

    i = -1;
    j = 0;
    while( next_inspiration( &i, &j, e, i, j ) )
    {
        if( e->pchar->aInspirations[i][j] )
        {
            float f, cost;
            const StoreItem * si = multistoreiter_GetBuyItem(piter, e->pchar->aInspirations[i][j], 0, &f);
            
            if( si )
                cost = ceil(si->iBuy*f);
            else
                cost = -1;
            
            y += ITEM_HT*sc;
            if (y < realy + ht && y + (ITEM_HT*sc) > realy)
            {
                store_drawPower(  e->pchar->aInspirations[i][j], x + ITEM_INSET*sc, y, z+3, sc, wd - (ITEM_INSET+TITLE_INSET)*sc, (ITEM_HT-10)*sc, i, j, cost, INV_PLAYER, si, cost>=0, kPowerType_Inspiration );
            }
        }
    }
 
       y += (ITEM_HT+10)*sc;

    // Recipes 
    {
        const DetailRecipe    *pRec = NULL;
        int                cost = 0;
        int                count = character_GetInvSize(e->pchar, kInventoryType_Recipe);
        int                realCount = e->pchar->recipeInvCurrentCount;
        int                maxCount = character_GetInvTotalSize(e->pchar, kInventoryType_Recipe);

        // checking for outdated recipe 
        if (gSelectedItem.pRec)
        {
            if (character_RecipeCount(e->pchar, gSelectedItem.pRec) <= 0)
                gSelectedItem.selected = 0;
        }

        drawFadeBar( x, y, z, wd - TITLE_INSET*2*sc, (ITEM_HT-10)*sc, 0xffffff22 );
        font( &game_12 );
        font_color( CLR_WHITE, CLR_WHITE );
        prnt( x+10*sc, y+18*sc, z, sc, sc, "RecipeTitle" );
        if (count >= maxCount)
            font_color( CLR_RED, CLR_RED );
        prnt( x + wd - 60*sc, y+18*sc, z, sc, sc, "(%d/%d)", realCount, maxCount  );

        for (i = 0; i < count; i++)
        {
            char            *name;
            int                amount, id;

            character_GetInvInfo(e->pchar, &id, &amount, &name, kInventoryType_Recipe, i);
            pRec = recipe_GetItemById(id);
            if (pRec)
            {
                cost = pRec->SellToVendor * multistoreiter_GetBuyRecipeMult(piter);
                if (cost > 0)
                {
                    y += ITEM_HT*sc;
                    store_drawRecipe(  pRec, x + ITEM_INSET*sc, y, z+3, sc, wd - (ITEM_INSET+TITLE_INSET)*sc, (ITEM_HT-10)*sc, cost,
                                        character_RecipeCount(e->pchar, pRec), i);

                } else {
                    cost = -1;
                }
            }
        }
        y += (ITEM_HT+10)*sc;
    }

    // Salvage
    {
        int                    invCount = e->pchar->salvageInvCurrentCount;
        int                    count = character_GetInvSize(e->pchar, kInventoryType_Salvage); 
        int                    maxCount = character_GetInvTotalSize(e->pchar, kInventoryType_Salvage);
        int                    cost = 0;
        const SalvageItem    *pSal = NULL;

        // checking for outdated recipe 
        if (gSelectedItem.pSal)
        {
            if (character_SalvageCount(e->pchar, gSelectedItem.pSal) <= 0)
                gSelectedItem.selected = 0;
        }

        drawFadeBar( x, y, z, wd - TITLE_INSET*2*sc, (ITEM_HT-10)*sc, 0xffffff22 );
        font( &game_12 );
        font_color( CLR_WHITE, CLR_WHITE );
        prnt( x+10*sc, y+18*sc, z, sc, sc, "SalvageTitle" );
        if (count >= maxCount)
            font_color( CLR_RED, CLR_RED );
        prnt( x + wd - 60*sc, y+18*sc, z, sc, sc, "(%d/%d)", invCount, maxCount  );
        for (i = 0; i < count; i++)
        {
            char    *name;
            int        amount, id;

            character_GetInvInfo(e->pchar, &id, &amount, &name, kInventoryType_Salvage, i);
            pSal = salvage_GetItemById(id);

            if (pSal)
            {

                cost = pSal->sellAmount * multistoreiter_GetBuySalvageMult(piter);
                if (cost > 0) 
                {
                    y += ITEM_HT*sc;
                    
                    store_drawSalvage(  pSal, x + ITEM_INSET*sc, y, z+3, sc, wd - (ITEM_INSET+TITLE_INSET)*sc, (ITEM_HT-10)*sc, cost,
                                    character_SalvageCount(e->pchar, pSal), i);
                } else {
                    cost = -1;
                }
            }
        }
        y += (ITEM_HT+10)*sc;
    }

    multistoreiter_Destroy(piter);
    return y - ty;
}

int store_drawStoreInventory( float x, float y, float z, float sc, float wd, float ht, bool bInspFull, bool bSpecFull, float realy )
{
    MultiStoreIter *piter;
    const StoreItem *psi;
    float f;
    int ty, count = 0;
    bool bDrawTempPowers = false;

    ty = y;

    x += TITLE_INSET*sc;
    y += (STORE_HEADER+10)*sc;

    PERFINFO_AUTO_START("Inspirations", 1);

    // inspirations
    piter = multistoreiter_Create(&s_MultiStore);

    if ( !piter )
    {
        PERFINFO_AUTO_STOP();
        return 0;
    }

    psi = multistoreiter_First(piter, &f);

    drawFadeBar( x, y, z, wd - TITLE_INSET*2*sc, (ITEM_HT-10)*sc, 0xffffff22 );
    font( &game_12 );
     font_color( CLR_WHITE, CLR_WHITE );
    prnt( x + 10*sc, y+18*sc, z, sc, sc, "InspirationTitle" );

    while(psi)
    {
        if( psi->ppowBase->eType == kPowerType_Inspiration )
        {
            int cost = ceil(f*psi->iSell);
            bool bAvailable = cost>=0 && !bInspFull && playerPtr()->pchar->iInfluencePoints>=cost;
            y += ITEM_HT*sc;
            if (y < realy + ht && y + (ITEM_HT*sc) > realy)
            {
                store_drawPower( psi->ppowBase, x + ITEM_INSET*sc, y, z+3, sc, wd - (ITEM_INSET+TITLE_INSET)*sc, (ITEM_HT-10)*sc, count, 0, cost, INV_STORE, psi, bAvailable, kPowerType_Inspiration );
            }
        }
        else if( psi->ppowBase->eType != kPowerType_Boost)
        {
            bDrawTempPowers = true;
        }

        count++;
        psi = multistoreiter_Next(piter, &f);
    }

    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("boosts", 1);

    // enhancements
     y += (ITEM_HT+10)*sc;
    psi = multistoreiter_First(piter, &f);
    count = 0;

    drawFadeBar( x, y, z, wd - TITLE_INSET*2*sc, (ITEM_HT-10)*sc, 0xffffff22 );
    font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );
    prnt( x+10*sc, y+18*sc, z, sc, sc, "EnhancementTitle" );

    while(psi)
    {
        if( psi->ppowBase->eType == kPowerType_Boost)
        {
            int cost = ceil(f*psi->iSell);
            bool bAvailable = cost>=0 && !bSpecFull && playerPtr()->pchar->iInfluencePoints>=cost;
            y += ITEM_HT*sc;
            if (y < realy + ht && y + (ITEM_HT*sc) > realy)
            {
                store_drawEnhancement( psi->ppowBase, x + ITEM_INSET*sc, y, z+3, sc, wd - (ITEM_INSET+TITLE_INSET)*sc, (ITEM_HT-10)*sc, count, psi->power.level, cost, INV_STORE, psi, bAvailable );
            }
        }

        count++;
        psi = multistoreiter_Next(piter, &f);
    }
    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_START("temppowers", 1);
    // Temp Powers
    if(bDrawTempPowers)
    {
        y += (ITEM_HT+10)*sc;
        psi = multistoreiter_First(piter, &f);
        count = 0;

        drawFadeBar( x, y, z, wd - TITLE_INSET*2*sc, (ITEM_HT-10)*sc, 0xffffff22 );
        font( &game_12 );
        font_color( CLR_WHITE, CLR_WHITE );
        prnt( x+10*sc, y+18*sc, z, sc, sc, "TempPowerTitle" );

        while(psi)
        {
            if( psi->ppowBase->eType != kPowerType_Inspiration && psi->ppowBase->eType != kPowerType_Boost)
            {
                int cost = ceil(f*psi->iSell);
                 bool bAvailable = cost>=0 && playerPtr()->pchar->iInfluencePoints>=cost;
                y += ITEM_HT*sc;
                if (y < realy + ht && y + (ITEM_HT*sc) > realy)
                {
                    store_drawPower( psi->ppowBase, x + ITEM_INSET*sc, y, z+3, sc, wd - (ITEM_INSET+TITLE_INSET)*sc, (ITEM_HT-10)*sc, count, 0, cost, INV_STORE, psi, bAvailable, kPowerType_Click );
                }
            }

            count++;
            psi = multistoreiter_Next(piter, &f);
        }
     }
    PERFINFO_AUTO_STOP();

    multistoreiter_Destroy(piter);

    return y - ty;
}

void storeOpen(int id, int iCnt, char **ppchStores)
{
    s_MultiStore.idNPC = id;

    if(s_MultiStore.ppchStores)
    {
        int i;
        for(i=0; i<s_MultiStore.iCnt; i++)
        {
            free(s_MultiStore.ppchStores[i]);
        }
        free(s_MultiStore.ppchStores);
    }

    s_MultiStore.ppchStores = ppchStores;
    s_MultiStore.iCnt = iCnt;

    window_setMode(WDW_STORE, WINDOW_GROWING);

    // If the store is open, then you can't open a contact window.
    if(window_getMode(WDW_CONTACT_DIALOG)==WINDOW_DISPLAYING)
    {
         restoreContact = 1;
        window_setMode(WDW_CONTACT_DIALOG, WINDOW_SHRINKING);
    }
    else
        restoreContact = 0;
}

void storeClose(void)
{
    s_MultiStore.idNPC = 0;

    window_setMode(WDW_STORE, WINDOW_SHRINKING);
}


int storeWindow()
{
    Entity *e = playerPtr();
    float x, y, z, wd, ht, scale;
    int color, back_color, inv_ht, store_ht, grey;
    char * buttonText;
     int numInsp = 0, numSpec = 0;
    bool bSpecFull, bInspFull;
    int i, j;
    char *currency = NULL;

    static int init = 0;
    static ScrollBar inv_sb = {0};
    static ScrollBar store_sb = {0};
    CBox box;

    if(!init)
    {
        s_menuSell = contextMenu_Create(NULL);
        contextMenu_addCode(s_menuSell, alwaysAvailable, 0, menu_Info,      0, "CMInfoString", 0);
        contextMenu_addDivider(s_menuSell);
        contextMenu_addCode(s_menuSell, menu_Available,  0, menu_BuyOrSell, 0, "CMSellNow", 0);
        contextMenu_addCode(s_menuSell, menu_SellStack,  0, menu_BuyOrSell, (void *) 1, "CMSellAllNow", 0);

        s_menuBuy = contextMenu_Create(NULL);
        contextMenu_addCode(s_menuBuy, alwaysAvailable, 0, menu_Info,       0, "CMInfoString", 0);
        contextMenu_addDivider(s_menuBuy);
        contextMenu_addCode(s_menuBuy, menu_Available,  0, menu_BuyOrSell,  0, "CMBuyNow", 0);

        init = 1;
    }

    if( !window_getDims( WDW_STORE, &x, &y, &z, &wd, &ht, &scale, &color, &back_color ) )
    {
          gSelectedItem.selected = 0;
        gSelectedItem.inventory = 0;
        gSelectedItem.pRec = NULL;
        gSelectedItem.pSal = NULL;

        if( restoreContact  && window_getMode(WDW_STORE) == WINDOW_DOCKED )
        {
            restoreContact = 0;
            window_setMode(WDW_CONTACT_DIALOG, WINDOW_GROWING);
        }
        return 0;
    }

    drawFrame( PIX3, R10, x, y, z, wd, ht, scale, color, back_color );
    drawFrame( PIX3, R10, x, y, z+1, wd/2 - STORE_DIVIDE*scale, ht - STORE_FOOTER*scale, scale, color, 0 );
    drawFrame( PIX3, R10, x + wd/2 + STORE_DIVIDE*scale, y, z+1, wd/2 - STORE_DIVIDE*scale, ht - STORE_FOOTER*scale, scale, color, 0 );
    drawFrame( PIX3, R10, x, y + STORE_HEADER*scale, z+1, wd/2 - STORE_DIVIDE*scale, ht - (STORE_FOOTER+STORE_HEADER)*scale, scale, color, 0 );
    drawFrame( PIX3, R10, x + wd/2 + STORE_DIVIDE*scale, y + STORE_HEADER*scale, z+1, wd/2 - STORE_DIVIDE*scale, ht - (STORE_FOOTER+STORE_HEADER)*scale, scale, color, 0 );

    font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );
    cprntEx( x + (wd-2*STORE_DIVIDE*scale)/4, y + STORE_HEADER*scale/4, z, scale, scale, (CENTER_X | CENTER_Y), "MyInventory" );

    font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );
    cprntEx( x + 3*(wd-2*STORE_DIVIDE*scale)/4, y + STORE_HEADER*scale/2, z, scale, scale, (CENTER_X | CENTER_Y), "StoreString" );

    // Count how many we've got
    for( i = 0; i < e->pchar->iNumInspirationCols; i++ )
    {
        for( j = e->pchar->iNumInspirationRows-1; j >= 0; j-- )
        {
            if( e->pchar->aInspirations[i][j] )
                numInsp++;
        }
    }
    for( i = 0; i < CHAR_BOOST_MAX; i++ )
    {
        if( e->pchar->aBoosts[i] )
            numSpec++;
    }
    bInspFull = (numInsp == e->pchar->iNumInspirationCols*e->pchar->iNumInspirationRows);
    bSpecFull = (numSpec >= e->pchar->iNumBoostSlots);


    PERFINFO_AUTO_START("playerinv", 1);
    // player inventory
    set_scissor( 1 );
    scissor_dims( x+PIX3*scale, y+(PIX3+STORE_HEADER)*scale, wd/2 - (PIX3*2+STORE_DIVIDE)*scale, ht - (STORE_HEADER+STORE_FOOTER+2*PIX3)*scale );
    BuildCBox( &box, x+PIX3*scale, y+(PIX3+STORE_HEADER)*scale, wd/2 - (PIX3*2+STORE_DIVIDE)*scale, ht - (STORE_HEADER+STORE_FOOTER+2*PIX3)*scale );
    inv_ht = store_drawPlayerInventory( x+(TITLE_INSET+PIX3)*scale, y+(PIX3+STORE_HEADER+10)*scale - inv_sb.offset, z, scale, wd/2-(2*PIX3+STORE_DIVIDE)*scale, ht - (2*PIX3+STORE_FOOTER+STORE_HEADER)*scale, numInsp, numSpec, bInspFull, bSpecFull,y+(PIX3+STORE_HEADER+10)*scale  );
    set_scissor( 0 );
    PERFINFO_AUTO_STOP();

    if (window_getMode(WDW_STORE) == WINDOW_DISPLAYING)
        doScrollBar( &inv_sb, ht - (STORE_HEADER+STORE_FOOTER+2*PIX3+2*R10)*scale, inv_ht, x+wd/2 - STORE_DIVIDE*scale, y + (STORE_HEADER+R10)*scale, z, &box, 0 );

    PERFINFO_AUTO_START("storeinv", 1);
    // store inventory
    set_scissor( 1 );
    scissor_dims( x+wd/2+(STORE_DIVIDE+PIX3)*scale, y+(PIX3+STORE_HEADER)*scale, wd/2 - (PIX3*2+STORE_DIVIDE)*scale, ht - (STORE_HEADER+STORE_FOOTER+2*PIX3)*scale );
    BuildCBox( &box, x+wd/2+(STORE_DIVIDE+PIX3)*scale, y+(PIX3+STORE_HEADER)*scale, wd/2 - (PIX3*2+STORE_DIVIDE)*scale, ht - (STORE_HEADER+STORE_FOOTER+2*PIX3)*scale );
    store_ht = store_drawStoreInventory(x+wd/2+(STORE_DIVIDE+PIX3)*scale, y+PIX3*scale - store_sb.offset, z, scale, wd/2-(2*PIX3+STORE_DIVIDE)*scale, ht - (2*PIX3+STORE_FOOTER)*scale, bInspFull, bSpecFull, y );
    set_scissor( 0 );
    PERFINFO_AUTO_STOP();

    if (window_getMode(WDW_STORE) == WINDOW_DISPLAYING)
        doScrollBar( &store_sb, ht - (STORE_HEADER+STORE_FOOTER+2*PIX3+2*R10)*scale, store_ht, x+wd, y + (STORE_HEADER+R10)*scale, z, &box, 0);

    // influence
    font( &game_12 );
    if(gSelectedItem.selected
        && gSelectedItem.inventory==INV_STORE
        && gSelectedItem.price > e->pchar->iInfluencePoints)
    {
        font_color( CLR_RED, CLR_RED );
    }
    else
    {
        font_color( CLR_WHITE, CLR_WHITE );
    }
    switch(skinFromMapAndEnt(e))
    {
        case UISKIN_HEROES:
        default:
            currency = textStdEx(NO_PREPEND, "Influence");
            break;
        case UISKIN_VILLAINS:
            currency = textStdEx(NO_PREPEND, "V_Influence");
            break;
        case UISKIN_PRAETORIANS:
            currency = textStdEx(NO_PREPEND, "P_Influence");
            break;
    }
    drawFrame( PIX3, R10, x + R10*scale, y + STORE_HEADER*scale/2, z, wd/2 - (2*R10+STORE_DIVIDE)*scale, 2*(R10+PIX3)*scale, scale, color, 0 );
    cprntEx( x + R10*2*scale, y + (STORE_HEADER/2 + R10 + PIX3)*scale, z, scale, scale, CENTER_Y | NO_MSPRINT, currency );
    cprntEx( x + R10*2*scale + str_wd( font_grp, scale, scale, currency ), y + (STORE_HEADER/2 + R10 + PIX3)*scale, z, scale, scale, CENTER_Y, textStd("InfluenceWithCommas", e->pchar->iInfluencePoints) );

    if( gSelectedItem.inventory==INV_STORE )
        buttonText = textStd( "Buy" );
    else
        buttonText = textStd( "Sell" );


    grey = BUTTON_LOCKED;
    if(gSelectedItem.selected)
    {
        if(gSelectedItem.price >= 0
            && ((gSelectedItem.inventory==INV_PLAYER)
                || (gSelectedItem.price <= e->pchar->iInfluencePoints
                    && ((gSelectedItem.type==kPowerType_Inspiration && !bInspFull)
                        || (gSelectedItem.type==kPowerType_Boost && !bSpecFull)
                        || gSelectedItem.type==kPowerType_Click))))
        {
            grey = 0;
        }
    }


     if( D_MOUSEHIT == drawStdButton( x + wd/2, y + ht - STORE_FOOTER*scale/2, z, 100*scale, 40*scale, CLR_GREEN, buttonText, 1.3f*scale, grey ) )
    {
        menu_BuyOrSell(NULL);
        sndPlay( "happy1", SOUND_GAME );
    }

    return 0;
}
