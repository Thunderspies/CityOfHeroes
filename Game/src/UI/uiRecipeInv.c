/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "entity/powers.h"
#include "uiRecipeInv.h"
#include "uiConceptInv.h"
#include "UI/uiCursor.h"
#include "uiGrowBig.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>

#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "player/badges.h"
#include "player/badges_client.h"

#include "gameComm/wdwbase.h"
#include "UI/uiClipper.h"
#include "uiListView.h"
#include "UI/uiUtil.h"
#include "UI/uiWindows.h"
#include "UI/uiUtilGame.h"
#include "UI/uiScrollBar.h"
#include "UI/uiTabControl.h"
#include "UI/uiInput.h"
#include "UI/uiInfo.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "player/player.h"
#include "cmdparse/cmdgame.h"

#include "UI/uiBadges.h"
#include "player/badges_client.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "render/tex.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/mathutil.h>

#include <utilitieslib/language/MessageStore.h>
#include "entity/Concept.h"
#include "bases/DetailRecipe.h"
#include "entity/character_base.h"
#include "entity/character_inventory.h"
#include "UI/uiComboBox.h"
#include "UI/uiContextMenu.h"
#include "UI/uiNet.h"
#include "entity/RewardSlot.h"
#include "entity/Invention.h"
#include "gameComm/trayCommon.h"

// ------------------------------------------------------------
// 

// recipe->recipe->ppVars[0]->pchName
#define AM_NAME(RECIPE) RECIPE->pchName
#define INV_AMGRP(INV) (uiAttribModGroup_GetByStr(AM_NAME(INV->recipe)))


// ------------------------------------------------------------
// 

typedef enum RecipeSortType
{
    kRecipeSortType_Name_Ascending,
    kRecipeSortType_Name_Descending,
    kRecipeSortType_Max_Ascending,
    kRecipeSortType_Max_Descending,
    kRecipeSortType_Type_Ascending, // i.e. Damage, etc. for the 'all' tab
    kRecipeSortType_Type_Descending,
    kRecipeSortType_Count
} RecipeSortType;

static bool sorttype_Valid(RecipeSortType t)
{
    return INRANGE0(t,kRecipeSortType_Count);
}

static char *sorttype_Str(RecipeSortType t)
{
    static char *strs[] = 
        {
            "kSortType_Name_Ascending",
            "kSortType_Name_Descending",
            "kSortType_Max_Ascending",
            "kSortType_Max_Descending",
            "kSortType_Type_Ascending", // i.e. Damage, etc. for the 'all' tab
            "kSortType_Type_Descending",
        };
    STATIC_INFUNC_ASSERT(ARRAY_SIZE(strs) == 6);
    return AINRANGE( t, strs ) ? strs[t] : "<invalid sorttype>";
}


// ------------------------------------------------------------
// 
typedef struct TabState
{
    RecipeSortType sortby;
    ComboBox sortbyCombo;
    uiAttribModGroup *amgroup;
} TabState;

// a little hoaky, but the one with the NULL uiAttribModGroup is 'ALL'
#define TAB_ALL ((uiAttribModGroup*)NULL)
#define TAB_ALL_UINAME "AllTab"

MP_DEFINE(TabState);
static TabState* tabstate_Create(uiAttribModGroup *amgroup)
{
    TabState *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(TabState, 64);
    res = MP_ALLOC( TabState );
    if( verify( res ))
    {
        int i;
        static ComboCheckboxElement **s_varElements = NULL;
        static ComboCheckboxElement **s_allElements = NULL;
        F32 wd_combo = 164.f;
        F32 ht_combo = 20.f;
        ComboCheckboxElement **srcComboElts = NULL;
        // onetime init the combo box elements
        if( s_varElements == NULL )
        {

            for( i = 0; i < kRecipeSortType_Count; ++i ) 
            {
                comboboxSharedElement_add( &s_allElements, NULL, sorttype_Str(i), sorttype_Str(i), i, NULL );
                // copy the all to the others except for type, which is NA to others
                if( i != kRecipeSortType_Type_Descending && i != kRecipeSortType_Type_Ascending )
                {
                    eaPush(&s_varElements, s_allElements[i] );
                }
            }
        }

        // --------------------
        // init this

        res->amgroup = amgroup;

        // set the src
        srcComboElts = (amgroup == TAB_ALL) ? s_allElements : s_varElements;

        comboboxTitle_init( &res->sortbyCombo, 0,0,0, wd_combo, ht_combo, ht_combo*eaSize(&srcComboElts), WDW_RECIPEINV );
        res->sortbyCombo.elements = srcComboElts;
        res->sortbyCombo.reverse = TRUE;

    }
    // --------------------
    // finally, return

    return res;
}

static void tabstate_Destroy( TabState *item )
{
    comboboxClear(&item->sortbyCombo);
    MP_FREE(TabState, item);
}

static int s_tabCmp(void const *lhs, void const *rhs)
{
    int res = 0;
    if( verify( lhs && rhs ))
    {
        TabState *l = *(TabState**)lhs;
        TabState *r = *(TabState**)rhs;
        res = l && r && l->amgroup && r->amgroup ? stricmp(l->amgroup->pchName, r->amgroup->pchName) : l - r;
    }
    // --------------------
    // finally

    return res;
}

//------------------------------------------------------------
//  sort the items given the current tab state
//----------------------------------------------------------
static int s_itemSort(const void* item1, const void* item2, void const *context)
{
    int res = 0;
    RecipeInventoryItem const *lhs = *(RecipeInventoryItem**)item1;
    RecipeInventoryItem const *rhs = *(RecipeInventoryItem**)item2;
    RecipeSortType sortBy = (RecipeSortType)context;
    bool flip = false;
    
    if( verify(lhs && rhs && lhs->recipe && rhs->recipe && sorttype_Valid( sortBy )))
    {
        switch ( sortBy )
        {
        case kRecipeSortType_Name_Descending:
            flip = true;
        case kRecipeSortType_Name_Ascending:
        {
            res = verify(lhs->recipe->ui.pchDisplayName && rhs->recipe->ui.pchDisplayName) ? strcmp( lhs->recipe->ui.pchDisplayName, rhs->recipe->ui.pchDisplayName ) : lhs - rhs;
        }
        break;
        default:
            verify(0); // invalid state
        };        
    }

    // --------------------
    // finally

    return res*(flip?-1:1);
}

// ------------------------------------------------------------
// 

typedef struct RecipeWindowState
{
    TabState **tabStates;
    RecipeInventoryItem **ppItems;
    int lastInvSize;
    uiAttribModGroup *curTab;
    ContextMenu * rmenu;
    uiGrowBig growbig;
    uiTabControl *tc;
} RecipeWindowState;
static RecipeWindowState s_state;

static TabState* s_TabState_FromAttrModGrp(RecipeWindowState *state, uiAttribModGroup *grp )
{
    TabState *res = NULL;
    int i;
    for( i = eaSize(&state->tabStates) - 1; i >= 0 && !res; --i)
    {
        res = state->tabStates[i]->amgroup == grp ? state->tabStates[i] : NULL;
    }
    // --------------------
    // finally

    return res;
}


static void s_initTabState(TabState *tabstate, RecipeSortType sort)
{
    if( verify( tabstate && INRANGE0(sort, kRecipeSortType_Count)))
    {
        Character *pchar = playerPtr()->pchar;
        int i;
        
        s_state.curTab = tabstate->amgroup;
        tabstate->sortby = sort;
        combobox_setId(&tabstate->sortbyCombo, sort, TRUE );
        combobox_setChoiceCur(&tabstate->sortbyCombo, sort );

        eaSetSize( &s_state.ppItems, 0);

        for( i = 0; i < eaSize(&pchar->recipeInv); ++i ) 
        {
            RecipeInventoryItem *inv = pchar->recipeInv[i];
            if( s_state.curTab == TAB_ALL
                || tabstate->amgroup == INV_AMGRP(inv))
            {
                // if this is valid, add it.
                if( recipeinventoryitem_Valid(inv))
                {
                    eaPush(&s_state.ppItems, inv);
                }
            }
        }

        // --------------------
        // now sort by type.

        // first sort by name ascending
        stableSort( s_state.ppItems, eaSize(&s_state.ppItems), sizeof(*s_state.ppItems), 
                    (void*)kRecipeSortType_Name_Ascending, s_itemSort);

        // then the given sort
        stableSort( s_state.ppItems, eaSize(&s_state.ppItems), sizeof(*s_state.ppItems), 
                    (void*)tabstate->sortby, s_itemSort);
    }
}

static void s_tabSelectCb(uiAttribModGroup *data)
{
    int i;
    for( i = eaSize( &s_state.tabStates ) - 1; i >= 0; --i)
    {
        TabState *s = s_state.tabStates[i];
        if( verify(s) && s->amgroup == data )
        {
            s_initTabState( s, s->sortby );
            break;
        }
    }

    // just check that the tab was found
    verify(i >= 0);
}

//------------------------------------------------------------
//  Fills the state with the character's inventory
//----------------------------------------------------------
static void recipewindowstate_Sync(RecipeWindowState *state, Character *pchar, uiAttribModGroup *curTab)
{
    if( verify( state && state->tc && pchar ))
    {
        int i;
        
        // --------------------
        // cleanup
        
        for( i = eaSize( &state->tabStates ) - 1; i >= 0; --i)
        {
            tabstate_Destroy( state->tabStates[i] );
        }
        eaSetSize( &state->tabStates, 0 );
        eaSetSize( &state->ppItems, 0 );
        
        // ----------------------------------------
        // init
        
        state->lastInvSize = eaSize(&pchar->recipeInv);
        state->curTab = curTab;
        
        // --------------------
        // gather the tabs, and fill current tab items

        // special case: the 'all' tab
        eaPush(&state->tabStates, tabstate_Create(TAB_ALL));

        for( i = eaSize( &pchar->recipeInv ) - 1; i >= 0; --i)
        {
            RecipeInventoryItem *inv = pchar->recipeInv[i];
            
            if( recipeinventoryitem_Valid(inv) )
            {
                // @todo -AB: get the actual uiAttribMod :2005 May 04 05:04 PM
                uiAttribModGroup *grp = INV_AMGRP(inv);
                TabState *tab = NULL;
                int i_tab;

                // --------------------
                // try to find the tab for this item.

                // look for it
                for( i_tab = 0; i_tab < eaSize(&state->tabStates) && !tab; ++i_tab )
                {
                    if( grp == state->tabStates[i_tab]->amgroup )
                    {
                        tab = state->tabStates[i_tab];
                    }
                }
                
                // if not found, create and add the tab
                if( !tab )
                {
                    tab = tabstate_Create( grp );
                    eaPush(&state->tabStates, tab);
                }

                // --------------------
                // add this inv item to the tab if it fits

                if( verify( tab ) && (tab->amgroup == curTab || curTab == TAB_ALL ))
                {
                    eaPush(&s_state.ppItems, inv );
                }
            }
        }
        
        // sort the tabs by name
        eaQSort(state->tabStates, s_tabCmp);
        
        // --------------------
        // init the ui
        
        // tab controls
        uiTabControlRemoveAll(state->tc);
        
        for( i = 0; i < eaSize(&s_state.tabStates); ++i ) 
        {
            TabState *s = s_state.tabStates[i];
            uiTabControlAdd(state->tc, s->amgroup ? s->amgroup->pchName : TAB_ALL_UINAME, s->amgroup );
        }

        // prime the tabcontrol by invoking the callback
        s_tabSelectCb(curTab);
    }
}


#define TAB_HT        20
#define ICON_WD 48
#define GRAPH_WD 192
#define GRAPH_HT (48 + 48/2)
#define GRAPH_ITM_WD 42
#define ITEM_WD (ICON_WD + GRAPH_WD + 12)
#define ITEM_HT (GRAPH_HT + 2*PIX3)


//------------------------------------------------------------
//  draw a single graph bar
// param box_graph: describes the extent of the graph from max to min.
// param graph_varmax/min: the max and min in var space
// return: the screen dims of the drawn bar.
//----------------------------------------------------------
UIBox uiinvent_drawGraphBar( UIBox *box_bar, 
                             F32 start, F32 dVal,
                             F32 graph_varmin, F32 graph_varmax,
                             F32 z_icon,
                             char *icon_bar,
                             int color )
{
    UIBox res = {0};

    if( verify( box_bar && icon_bar ) )
    {
        AtlasTex *icon = atlasLoadTexture(icon_bar);
        
        if( verify(icon) )
        {
            // scale the icon to fit the area (by the growbig amnt too)
            // @todo -AB: use a log scale :2005 May 09 04:03 PM
            F32 fMin = MIN(start, start + dVal);
            F32 fMax = MAX(start, start + dVal);            
            F32 graph_range = graph_varmax - graph_varmin;
            F32 y_pct = (fMax - graph_varmin)/(graph_range);
            F32 ht_pct = fabs(dVal)/graph_range;
            F32 y_clip = box_bar->y + box_bar->height*(1.f - y_pct);
            F32 x_clip = box_bar->x;
            F32 ht_clip = ht_pct*box_bar->height;
            F32 wd_clip = box_bar->width;
            
            // todo: scale properly
            F32 sc_icon = 3*box_bar->width/(icon->width);

            UIBox box = 
                {
                    x_clip, y_clip, wd_clip, ht_clip
                };
                    
            clipperPushRestrict(&box);
            {
                display_sprite( icon, x_clip - (icon->width*sc_icon - wd_clip)/2, y_clip - (icon->height*sc_icon - ht_clip)/2, z_icon, sc_icon, sc_icon, color );
            }
            clipperPop();                    

            res = box;
        }
    }
    // --------------------
    // finally

    return res;
}



//------------------------------------------------------------
//  helper for displaying a concept as it appears in the 
// concept inventory window.
// NOTE: a little quirky since it helps DnD, doesn't display
// the amount in inventory, just the item itself.
//----------------------------------------------------------
static void s_RecipeItem_Display(RecipeInventoryItem *itm, U32 ixInvCurRecipe, F32 x, F32 y, F32 z, F32 sc)
{
    /* Removed due to changes in Invention system - VD 

    if( verify( itm && itm->recipe ))
    {
        DetailRecipe *recipe = itm->recipe;
        F32 wd = (ITEM_WD) * sc;
        F32 ht = (ITEM_HT) * sc;
        AtlasTex *icon = NULL;
        int color = DARK_GREY;
        int bgcolor = DARK_GREY;
        F32 ty = y + PIX3*2*sc;
        F32 tx = x + PIX3*2*sc;
        F32 z_icon = z + ARRAY_SIZE( s_state.growbig.items ) + 10;
        F32 z_amount = z_icon + 1;
        int i;
        bool canInteract = true;
        int zmod = ARRAY_SIZE(s_state.growbig.items);
        F32 overScale = uigrowbig_GetScale( &s_state.growbig, itm, &zmod );
        Character *pchar = playerPtr()->pchar;
        CBox boxFrame;
        BuildCBox( &boxFrame, tx, ty, wd, ht); // always square
        
        // --------------------
        // draw the frame

        // test for over for interaction
        if( mouseCollision(&boxFrame) )
        {
            color = CLR_GREY;
            //bgcolor = CLR_GREY;
        }


        drawFlatFrame( PIX2, R10, tx, ty, z, wd, ht, sc, color, bgcolor );

        // --------------------
        // draw the icon
        
        {
            F32 sc_icon;
            icon = atlasLoadTexture( itm->recipe->ui.pchIconName );
            sc_icon = ((F32)ICON_WD)/icon->width*overScale*sc;
            display_sprite( icon, tx, ty, z + 1, sc, sc, CLR_WHITE ); 
        }
        
        // --------------------
        // draw the graph

        {
            F32 y_graph = ty + PIX3*sc; 
            F32 x_graph = tx + (ITEM_WD - GRAPH_WD - PIX3)*sc; 
            int colorfg = CLR_MOUSEOVER_FOREGROUND;
            int colorbg = CLR_MOUSEOVER_BACKGROUND;
            F32 wd_graph = GRAPH_WD*sc;
            F32 ht_graph = GRAPH_HT*sc;
            TTDrawContext *fnt = &game_9;
            F32 htDesc = 0.f;
            int wdBorder = PIX2;

            {
                CBox dimsFnt = {0};
                str_dims(fnt, sc, sc, TRUE, &dimsFnt, "TestStr");
                htDesc = dimsFnt.bottom - dimsFnt.top + PIX3*sc;
            }

            drawFlatFrame(wdBorder, R10, x_graph, y_graph, z + 1, wd_graph, ht_graph, sc, colorfg, colorbg );

            // draw the zero line in the graph
            {
                F32 x_zero = x_graph + wdBorder*sc;
                F32 y_zero = y_graph + ht_graph/2;
                F32 wd_zero = wd_graph - wdBorder*sc*2;
                F32 ht_zero = 1.f;
                UIBox bxZero = {x_zero, y_zero, wd_zero, ht_zero};
                AtlasTex *txWhite = atlasLoadTexture("white.tga");
                clipperPushRestrict(&bxZero);
                if(txWhite)
                {
                    F32 sc_zero = wd_zero/txWhite->width;
                    display_sprite( txWhite, x_zero, y_zero, z_icon+1, sc_zero, sc_zero, CLR_WHITE );
                }
                clipperPop();
            }
            
            for( i = 0; i < eaSize(&itm->recipe->recipe->ppVars); ++i ) 
            {
                PowerVar *pv = itm->recipe->recipe->ppVars[i];
                uiAttribModGroup *grp = uiAttribModGroup_GetByStr(pv->pchName);
                
                if( verify( grp ))
                {
                    icon = atlasLoadTexture(grp->pogName);
                    
                    // draw the icon
                    if( verify(icon) ) 
                    {
                        F32 x_bar = x_graph + (ICON_WD*i)*sc + PIX3*sc*2;
                        F32 y_bar = y_graph + PIX3*sc*2;
                        F32 ht_bar = ht_graph - htDesc - PIX3*sc*3;
                        UIBox box_bar = 
                            {
                                x_bar,
                                y_bar,
                                GRAPH_ITM_WD*sc,
                                ht_bar
                            } ;
                                
                        UIBox bxDrawn = uiinvent_drawGraphBar( &box_bar, 
                                               pv->fMin, pv->fMax - pv->fMin,
                                               -5.f, 5.f,
                                               z_icon-zmod,
                                               grp->pogName,
                                               CLR_WHITE );
                        
                        // --------------------
                        // print the values            
                                                
                        // max and min and name
                        {
                            F32 x_off = bxDrawn.x + GRAPH_ITM_WD*.5f;
                            F32 dy_off = 0.f;
                            F32 htFnt;

                            {
                                CBox cbDims = {0};
                                str_dims(fnt, sc, sc, TRUE, &cbDims, "00.00" );
                                htFnt = cbDims.bottom - cbDims.top;
                            }

                            // in case they get too smushed
                            if( bxDrawn.height < htFnt)
                            {
                                dy_off = htFnt;// + PIX3*2*sc;
                            }
                            else
                            {
                                dy_off = htFnt/2;
                            }

                            font( &game_9 );
                            font_color(0x00deffff, 0x00deffff);
                            cprnt( x_off, bxDrawn.y + dy_off, z_icon - zmod + 1, sc, sc, "%.2f", pv->fMax );
                            cprnt( x_off, bxDrawn.y + bxDrawn.height + dy_off, z_icon - zmod + 1, sc, sc, "%.2f", pv->fMin );

                            // name
                            cprnt( x_off, y_graph + ht_graph - PIX3*sc, z_icon - zmod + 1, sc, sc, grp->pchName);
                        }                        

                    }
                }
            }
        }

        // --------------------
        // interaction

        // draw the 'invent' button
        if( !character_IsInventing( pchar ) )
        {
            UIBox bxBtn;
            
            // get the dims
            drawTextButton( NULL, 0, 0, 0, sc, 0, 0, 0, 0, &bxBtn );
            
            bxBtn.x = tx;
            bxBtn.y = ty + ht - bxBtn.height;
            
            if( D_MOUSEHIT == drawTextButton( "RecipeInvInvent", bxBtn.x, bxBtn.y, z+1, sc, color, bgcolor, CLR_WHITE, TRUE, &bxBtn ))
            {
                if(character_InventingStart( pchar, ixInvCurRecipe ))
                {
                    // open the invention window
                    window_setMode( WDW_INVENT, WINDOW_GROWING);
                }
            }
        }
    }
    */
}

// static void s_trayobj_displayCb(TrayObj *obj, F32 x, F32 y, F32 z, F32 sc)
// {
//     if( verify( obj 
//                 && EAINRANGE( obj->conceptInv.invIdx, s_state.ppItems )
//                 && s_state.ppItems[obj->conceptInv.invIdx] ))
//     {
//         s_RecipeItem_Display( s_state.ppItems[obj->conceptInv.invIdx], x, y, z, sc);
//     }
// }

static const char* recipe_display(RecipeInventoryItem *itm, int ixInvCurRecipe, Character *pchar, RecipeWindowState *state, float x, float y, float z, float sc )
{
    float wd = ICON_WD * sc;
    const char *res = NULL;
    AtlasTex *icon = NULL;
    int color = CLR_GREY;
    float ty, tx;
    F32 z_icon = z + ARRAY_SIZE( state->growbig.items ) + 10; // the max z an icon will occupy
    F32 z_amount = z_icon + 1;
    bool canInteract = true;

    if( !verify( itm && itm->recipe && EAINRANGE( ixInvCurRecipe, s_state.ppItems ) && s_state.ppItems[ixInvCurRecipe] == itm ))
    {
        return res;
    }        
    
    ty = y + PIX3*2*sc;
    tx = x + PIX3*2*sc;
    
    // --------------------
    // draw the icon

    s_RecipeItem_Display(itm, ixInvCurRecipe, x, y, z, sc);

    // --------------------
    // print the amount

    {
        F32 x_off = 4;
        F32 y_off = 15;
        font( &game_9 );
        font_color(0x00deffff, 0x00deffff);
        prnt( tx + x_off*sc, ty + y_off*sc, z_amount, sc, sc, "%d", itm->amount );
    }

    // --------------------
    // handle interaction
    
    {
        CBox box;
        BuildCBox( &box, tx, ty, wd, wd); // always square
        
        if( mouseCollision(&box) )
        {
            // set the info text
            // @todo -AB: popover about this :2005 May 04 04:37 PM
            res = itm->recipe->ui.pchDisplayShortHelp;
            
            // set the context menu
            if( mouseClickHit( &box, MS_RIGHT ) )
            {
                int mx, my;
                CMAlign alignVert = CM_TOP;
                CMAlign alignHoriz = CM_LEFT;
                
                rightClickCoords(&mx,&my);
                contextMenu_setAlign( state->rmenu, mx, my, alignHoriz, alignVert, itm);
            }
            else if ( mouseLeftDrag( &box ) 
                      && verify(!cursor.dragging)) // just for my understanding
            {
//                 // drag the concept
//                 ZeroStruct( &cursor.drag_obj );
//                 cursor.dragging = TRUE;
//                 cursor.drag_obj.conceptInv.invIdx = ixInvCurRecipe;
//                 cursor.drag_obj.conceptInv.displayCb = s_trayobj_displayCb;
//                 cursor.drag_obj.conceptInv.z = z_icon + 1; //one above the max should work
            }
        }        
    }
    

    // --------------------
    // finally

    return res;
}


int recipeinvWindow()
{
    float x, y, z, wd, ht, sc;
//     F32 view_y;
    F32 edge_off;
    int i,color, bcolor;
    static ScrollBar sb = { WDW_RECIPEINV, 0};
    static uiTabControl * tc = NULL;
    Character *pchar;

     if(!window_getDims(WDW_RECIPEINV, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
        return 0;

    pchar = playerPtr()->pchar;

    if( !tc )
    {
        tc = uiTabControlCreate(TabType_Undraggable, s_tabSelectCb, 0, 0, 0, 0 );
        
        // state
        ZeroStruct(&s_state);
        eaCreate(&s_state.ppItems);
        s_state.lastInvSize = -1;
        s_state.tc = tc;

        // context menu        
        s_state.rmenu = contextMenu_Create(NULL);

        // init the growbig
        uigrowbig_Init( &s_state.growbig, 1.1, 10 );

        // init
        recipewindowstate_Sync( &s_state, pchar, TAB_ALL );
    }

    // debugging for EnC, set this every frame.
    uiTabControl_SetSelectCb(tc, s_tabSelectCb);

    // --------------------
    // check for an inventory state change

    {
        int i;
        bool invalidFound = false;
        for( i = eaSize( &s_state.ppItems ) - 1; i >= 0 && !invalidFound; --i)
        {
            invalidFound = !recipeinventoryitem_Valid(s_state.ppItems[i]);
        }

        // init the tab
        if( invalidFound || s_state.lastInvSize != eaSize(&pchar->recipeInv))
        {
            // sync the state
            recipewindowstate_Sync( &s_state, pchar, s_state.curTab );
        }
    }

    // ----------------------------------------
    // post-init and state update

    edge_off = (R10+PIX3)*sc;


    // window frame    
    drawFrame(PIX3, R10, x, y, z, wd, ht, sc, color, bcolor);

    // if not ready yet, return
      if(window_getMode(WDW_RECIPEINV) != WINDOW_DISPLAYING)
        return 0;

    // tabs
    drawTabControl(tc, x+edge_off*sc, y, z, wd - edge_off*2*sc, 15, sc, color, color, TabDirection_Horizontal);

    // --------------------
    // display window internals 
    {
        TabState *tabState = s_TabState_FromAttrModGrp(&s_state, uiTabControlGetSelectedData(tc));

        int size = eaSize( &s_state.ppItems );
        U32 scaledItemWd = ITEM_WD*sc;
        const char *infoToDisplay = NULL;
        RecipeInventoryItem *mouseOverInv = NULL;
        CBox box;
        F32 ht_top = (PIX3+TAB_HT)*sc;
        F32 wd_border = PIX3*2*sc;
        F32 x_items = x+PIX3*sc;
        F32 y_items = y+ht_top;
        F32 ht_btm = ht_top;
        F32 ht_items = ht - ht_btm - ht_top;
        F32 wd_items = wd - wd_border*2;
        F32 wd_btm = wd_items;
        F32 x_btm = x_items;
        F32 y_btm = y_items + ht_items;
        
        // --------------------
        // handle the items
        
        set_scissor(1);
        scissor_dims( x_items, y_items, wd_items, ht_items );
        BuildCBox( &box, x_items, y_items, wd_items, ht_items);
        
        for( i = 0; i < size; ++i ) 
        {
            F32 row_off = ITEM_HT*i*sc;
            RecipeInventoryItem *inv = s_state.ppItems[i];
            const char *info = NULL;
            // display the item
            info = recipe_display( inv, i, pchar, &s_state, box.left, y_items-sb.offset+row_off, z, sc );
            
            if( info )
            {
                infoToDisplay = info;
                mouseOverInv = inv;
            }
        }
        set_scissor(0);
        
        //scrollbar
        {
            F32 doc_bottom = y_items + size*scaledItemWd;
            doScrollBar( &sb, ht - edge_off*2 - (TAB_HT+20)*sc, doc_bottom - y, wd, (PIX3+R10+TAB_HT)*sc, z+5, 0, 0 );
        }
        
        // grow the current icon
        uigrowbig_Update(&s_state.growbig, mouseOverInv );
        
        // --------------------
        // update the sort
        {
            // do to feature of combobox, need to multiply by inverse scale
            F32 x_adj = (wd_btm - 160.f*sc)/sc;
            F32 y_adj = (ht_items + ht_top - PIX2*sc)/sc;
            
            combobox_setLoc(&tabState->sortbyCombo, x_adj, y_adj, 10);
            combobox_display(&tabState->sortbyCombo);
            
            if( tabState->sortbyCombo.newlySelectedItem )
            {
                i = tabState->sortbyCombo.currChoice; 
                if( INRANGE0( i, kRecipeSortType_Count ))
                {
                    s_initTabState( tabState, i );
                }
                
                tabState->sortbyCombo.newlySelectedItem = FALSE;
            }
        }
    }

    // -----
    // finally

    return 0;
}
