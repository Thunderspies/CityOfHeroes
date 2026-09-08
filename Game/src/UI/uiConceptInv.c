/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "uiConceptInv.h"
#include "UI/uiCursor.h"
#include "uiGrowBig.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>
#include "entity/powers.h"
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

#include "formatter/smf_main.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "render/tex.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/mathutil.h>

#include <utilitieslib/language/MessageStore.h>
#include "entity/Concept.h"
#include "entity/character_base.h"
#include "entity/character_inventory.h"
#include "UI/uiComboBox.h"
#include "UI/uiContextMenu.h"
#include "UI/uiNet.h"
#include "entity/RewardSlot.h"
#include "entity/Invention.h"
#include "gameComm/trayCommon.h"
#include <utilitieslib/language/MessageStoreUtil.h>

// ------------------------------------------------------------
//

static uiAttribModGroup s_grps[] = 
{
    {"Var_Taunt_Duration","E_POG_TAUNT_DURATION"},
    {"Var_Stun_Duration","E_POG_STUN_DURATION"},
    {"Var_Slow_Movement","E_POG_SLOW_MOVEMENT"},
    {"Var_Sleep_Duration","E_POG_SLEEP_DURATION"},
    {"Var_Run_Speed","E_POG_RUN_SPEED"},
    {"Var_Recovery","E_POG_RECOVERY"},
    {"Var_Recharge_Time","E_POG_RECHARGE_TIME"},
    {"Var_Range_Increase","E_POG_RANGE_INCREASE"},
    {"Var_Radius","E_POG_RADIUS"},
    {"Var_Leap_Height","E_POG_LEAP_HEIGHT"},
    {"Var_Knockback_Distance","E_POG_KNOCKBACK_DISTANCE"},
    {"Var_Jump_Distance","E_POG_JUMP_DISTANCE"},
    {"Var_Interrupt_Times","E_POG_INTERRUPT_TIMES"},
    {"Var_Intagibility_Duration","E_POG_INTAGIBILITY_DURATION"},
    {"Var_Immobilization_Duration","E_POG_IMMOBILIZATION_DURATION"},
    {"Var_Hold_Duration","E_POG_HOLD_DURATION"},
    {"Var_Heal","E_POG_HEAL"},
    {"Var_Fly_Speed","E_POG_FLY_SPEED"},
    {"Var_Fear_Duration","E_POG_FEAR_DURATION"},
    {"Var_End_Drain","E_POG_END_DRAIN"},
    {"Var_End_Discount","E_POG_END_DISCOUNT"},
    {"Var_Debuff_To_Hit","E_POG_DEBUFF_TO_HIT"},
    {"Var_Debuff_Defense","E_POG_DEBUFF_DEFENSE"},
    {"Var_Debuff_Damage","E_POG_DEBUFF_DAMAGE"},
    {"Var_Damage_Resist","E_POG_DAMAGE_RESIST"},
    {"Var_Damage","E_POG_DAMAGE"},
    {"Var_Confusion_Duration","E_POG_CONFUSION_DURATION"},
    {"Var_Cone_Range","E_POG_CONE_RANGE"},
    {"Var_Buff_To_Hit","E_POG_BUFF_TO_HIT"},
    {"Var_Buff_Defense","E_POG_BUFF_DEFENSE"},
    {"Var_Buff_Damage","E_POG_BUFF_DAMAGE"},
    {"Var_Accuracy","E_POG_ACCURACY"},
};

uiAttribModGroup* uiAttribModGroup_GetByStr(char const *str)
{
    int i;
    for( i = 0; i < ARRAY_SIZE( s_grps ); ++i ) 
    {
        if( 0 == stricmp(str, s_grps[i].pchName) )
        {
            return s_grps+i;
        }
    }
    return NULL;
}

#define AM_NAME(CONCEPTDEF) CONCEPTDEF->attribMods[0]->name
#define INV_AMGRP(INV) (uiAttribModGroup_GetByStr(AM_NAME(INV->concept->def)))

// ------------------------------------------------------------
// 

typedef enum ConceptSortType
{
    kConceptSortType_Amount_Ascending,
    kConceptSortType_Amount_Descending,
    kConceptSortType_Name_Ascending,
    kConceptSortType_Name_Descending,
    kConceptSortType_Max_Ascending,
    kConceptSortType_Max_Descending,
    kConceptSortType_Type_Ascending, // i.e. Damage, etc. for the 'all' tab
    kConceptSortType_Type_Descending,
    kConceptSortType_Count
} ConceptSortType;

static bool sorttype_Valid(ConceptSortType t)
{
    return INRANGE0(t,kConceptSortType_Count);
}

static char *sorttype_Str(ConceptSortType t)
{
    static char *strs[] = 
        {
            "kSortType_Amount_Ascending",
            "kSortType_Amount_Descending",
            "kSortType_Name_Ascending",
            "kSortType_Name_Descending",
            "kSortType_Max_Ascending",
            "kSortType_Max_Descending",
            "kSortType_Type_Ascending", // i.e. Damage, etc. for the 'all' tab
            "kSortType_Type_Descending",
        };
    STATIC_INFUNC_ASSERT(ARRAY_SIZE(strs) == 8);
    return AINRANGE( t, strs ) ? strs[t] : "<invalid sorttype>";
}


// ------------------------------------------------------------
// 
typedef struct TabState
{
    ConceptSortType sortby;
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
        int comboVisibleMax = 0;
        // onetime init the combo box elements
        if( s_varElements == NULL )
        {

            for( i = 0; i < kConceptSortType_Count; ++i ) 
            {
                comboboxSharedElement_add( &s_allElements, NULL, sorttype_Str(i), sorttype_Str(i), i, NULL );
                // copy the all to the others except for type, which is NA to others
                if( i != kConceptSortType_Type_Descending && i != kConceptSortType_Type_Ascending )
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
        comboVisibleMax = eaSize(&srcComboElts);// MIN(eaSize(&srcComboElts), 4); -- our combo boxes don't scroll

        comboboxTitle_init( &res->sortbyCombo, 0,0,0, wd_combo, ht_combo, ht_combo*comboVisibleMax, WDW_CONCEPTINV );
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

static int s_tabCmp(const void* lhsData, const void* rhsData)
{
    void const * lhs = (void const *)lhsData;
    void const * rhs = (void const *)rhsData;
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
    ConceptInventoryItem const *lhs = *(ConceptInventoryItem**)item1;
    ConceptInventoryItem const *rhs = *(ConceptInventoryItem**)item2;
    ConceptSortType sortBy = (ConceptSortType)context;
    bool flip = false;
    
    if( verify(lhs && rhs && lhs->concept && rhs->concept && sorttype_Valid( sortBy )))
    {
        switch ( sortBy )
        {
        case kConceptSortType_Name_Descending:
            flip = true;
        case kConceptSortType_Name_Ascending:
        {
            res = verify(lhs->concept->def->ui.pchDisplayName && rhs->concept->def->ui.pchDisplayName) ? strcmp( lhs->concept->def->ui.pchDisplayName, rhs->concept->def->ui.pchDisplayName ) : lhs - rhs;
        }
        break;
        case kConceptSortType_Amount_Descending:
            flip = true;
        case kConceptSortType_Amount_Ascending:
        {
            res = lhs->amount - rhs->amount;
        }
        break;
        case kConceptSortType_Max_Descending:
            flip = true;
        case kConceptSortType_Max_Ascending:
            {
                // okay, these values are expressed as 92.34 i.e. 92.34%, 
                // so rounding is fine
                res = (lhs->concept->afVars[0] - rhs->concept->afVars[0])*100.f;
            }
        break;
        case kConceptSortType_Type_Descending:
            flip = true;
        case kConceptSortType_Type_Ascending:
            {
                const ConceptDef *defLhs = lhs->concept->def;
                const ConceptDef *defRhs = rhs->concept->def;
                if( verify( eaSize(&defLhs->attribMods) > 0 && eaSize(&defRhs->attribMods) > 0 ))
                {
                    uiAttribModGroup *grpLhs = uiAttribModGroup_GetByStr(defLhs->attribMods[0]->name);
                    uiAttribModGroup *grpRhs = uiAttribModGroup_GetByStr(defRhs->attribMods[0]->name);
                    if( verify( grpLhs && grpRhs ))
                    {
                        res = strcmp(grpLhs->pchName, grpRhs->pchName);
                    }
                }
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

static int s_canSlotSort(void const *item1, void const *item2, void const *context)
{
    int res = 0;
    ConceptInventoryItem const *lhs = *(ConceptInventoryItem**)item1;
    ConceptInventoryItem const *rhs = *(ConceptInventoryItem**)item2;
    Character *pchar = (Character*)context;

    bool bCAlhs = basepower_CanApplyConcept( pchar->invention->recipe->recipe, lhs->concept->def);
    bool bCArhs = basepower_CanApplyConcept( pchar->invention->recipe->recipe, rhs->concept->def);

    return bCArhs - bCAlhs;
}


// ------------------------------------------------------------
// 

typedef struct ConceptWindowState
{
    TabState **tabStates;
    ConceptInventoryItem **ppItems;
    struct InvSyncVars
    {
        int lastInvSize;
        bool isInventing;
    } invSync;
    uiAttribModGroup *curTab;
    ContextMenu * rmenu;
    uiGrowBig growbig;
    uiTabControl *tc;
} ConceptWindowState;
static ConceptWindowState s_state;

static TabState* s_TabState_FromAttrModGrp(ConceptWindowState *state, uiAttribModGroup *grp )
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


static void s_initTabState(TabState *tabstate, ConceptSortType sort)
{
    if( verify( tabstate && INRANGE0(sort, kConceptSortType_Count)))
    {
        Character *pchar = playerPtr()->pchar;
        int i;
        
        s_state.curTab = tabstate->amgroup;
        tabstate->sortby = sort;
        combobox_setId(&tabstate->sortbyCombo, sort, TRUE );

        eaSetSize( &s_state.ppItems, 0);

        for( i = 0; i < eaSize(&pchar->conceptInv); ++i ) 
        {
            ConceptInventoryItem *inv = pchar->conceptInv[i];
            if( s_state.curTab == TAB_ALL
                || tabstate->amgroup == INV_AMGRP(inv))
            {
                if( conceptinventoryitem_Valid( inv ))
                {
                    eaPush(&s_state.ppItems, inv);
                }
            }
        }

        // --------------------
        // now sort by type.

        // first sort by name ascending
        stableSort( s_state.ppItems, eaSize(&s_state.ppItems), sizeof(*s_state.ppItems), 
                    (void*)kConceptSortType_Name_Ascending, s_itemSort);

        // then the given sort
        stableSort( s_state.ppItems, eaSize(&s_state.ppItems), sizeof(*s_state.ppItems), 
                    (void*)tabstate->sortby, s_itemSort);

        // special third case, if inventing
        if(character_IsInventing( pchar ))
        {
            stableSort( s_state.ppItems, eaSize(&s_state.ppItems), sizeof(*s_state.ppItems), pchar, s_canSlotSort);
        }
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
static void conceptwindowstate_Sync(ConceptWindowState *state, Character *pchar, uiAttribModGroup *curTab)
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
        
        state->invSync.lastInvSize = eaSize(&pchar->conceptInv);
        state->curTab = curTab;
        
        // --------------------
        // gather the tabs, and fill current tab items

        // special case: the 'all' tab
        eaPush(&state->tabStates, tabstate_Create(TAB_ALL));

        for( i = eaSize( &pchar->conceptInv ) - 1; i >= 0; --i)
        {
            ConceptInventoryItem *inv = pchar->conceptInv[i];
            
            if( conceptinventoryitem_Valid(inv))
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
                    eaPush(&state->ppItems, inv );
                }
            }
        }
        
        // sort the tabs by name 
        // NOTE: the 'all' tab (which is first) is skipped, it is always at the beginning
        qsort(state->tabStates+1, eaSize(&state->tabStates) - 1, sizeof(*state->tabStates), s_tabCmp);
        
        // --------------------
        // init the ui
        
        // tab controls
        uiTabControlRemoveAll(state->tc);
        
        for( i = 0; i < eaSize(&state->tabStates); ++i ) 
        {
            TabState *s = state->tabStates[i];
            uiTabControlAdd(state->tc, s->amgroup ? s->amgroup->pchName : TAB_ALL_UINAME, s->amgroup );
        }

        
        // prime the tabcontrol by invoking the callback
        uiTabControlSelect( state->tc, curTab );
    }
}


#define TAB_HT        20
#define START_ICON_WD 48
#define ICON_WD_DRAGGING 32
#define ICON_WD START_ICON_WD
#define COL_WIDTH START_ICON_WD
#define ROW_HEIGHT START_ICON_WD

typedef struct ConceptDisplayInfo
{
    bool mouseOver;
    CBox box;
} ConceptDisplayInfo;


//------------------------------------------------------------
//  helper for displaying a concept as it appears in the 
// concept inventory window.
// NOTE: a little quirky since it helps DnD, doesn't display
// the amount in inventory, just the item itself.
//----------------------------------------------------------
int uiconcept_DisplayItem(ConceptItem *concept, F32 x, F32 y, F32 z, F32 usIconWd, F32 sc, int color, bool bDrawText, UIBox *dimsIconRes)
{
    int dSlotWidthRes = 0;
    UIBox dimsRes = {0};
    
    if( verify( concept && concept->def ))
    {
        F32 wd = usIconWd * sc;
        AtlasTex *icon = NULL;
        F32 ty = y;
        F32 tx = x;
        F32 z_icon = z + ARRAY_SIZE( s_state.growbig.items ) + 10;
        F32 z_amount = z_icon + 1;
        int i;
        bool canInteract = true;
        int curSlot;
        const ConceptDef *def = concept->def;
        dSlotWidthRes = def->slotsUsed;
        dimsRes.x = tx;
        dimsRes.y = ty;
        dimsRes.width = dSlotWidthRes * wd;
        dimsRes.height = wd;
        
        for( curSlot = 0; curSlot < def->slotsUsed; ++curSlot )
        {
            int zmod = ARRAY_SIZE(s_state.growbig.items);
            F32 overScale = uigrowbig_GetScale( &s_state.growbig, concept, &zmod );
            
            for( i = 0; i < eaSize(&concept->def->attribMods); ++i ) 
            {
                F32 clipMod = 7.f/12.f;
                uiAttribModGroup *grp = uiAttribModGroup_GetByStr(concept->def->attribMods[i]->name);
                
                if( verify( grp ))
                {
                    icon = atlasLoadTexture(grp->pogName);
                    
                    // draw the icon
                    if( verify(icon) )
                    {
                        // scale the icon to fit the area (by the growbig amnt too)                        
                        F32 sc_icon = ((F32)usIconWd)/icon->width*overScale*sc;
                        F32 ht_clip = usIconWd*i*clipMod*sc;
                        F32 x_icon = tx + wd*curSlot;
                        F32 y_icon = ty;
                        F32 y_off = ty + ht_clip;
                        UIBox box = 
                            {
                                x_icon, y_off, wd*overScale, wd*overScale - ht_clip
                            };
                        
                        clipperPushRestrict(&box);
                        {
                            display_sprite( icon, x_icon, y_icon, z_icon-zmod, sc_icon, sc_icon, color );
                        }
                        clipperPop();
                        
                        // --------------------
                        // print the values            
                        
                        if( curSlot == 0 && bDrawText )
                        {
                            if( i == 0 )
                            {
                                font( &game_9 );
                                font_color(0x00deffff, 0x00deffff);
                            }
                            else
                            {
                                font( &tinyfont );
                                font_color(0x00deffff, 0x00deffff);
                            }

                            {
                                F32 x_off = (wd*def->slotsUsed)*.5f;
                                F32 y_off = box.height/2.f;
                                cprnt( box.x + x_off, box.y  + y_off, z_icon - zmod + 1, sc, sc, "%.1f", concept->afVars[i] );
                            }
                        }
                        
                        // --------------------
                        // draw the bridge, if applicable
                        
                        // somehow show a bridge between the slot areas for multi slot
                        if( curSlot < def->slotsUsed - 1 )
                        {
                            F32 sc_bridge = sc_icon;
                            UIBox box_bridge = box;
                            F32 x_bridge = tx + + wd/2.f;
                            F32 y_bridge = ty;
                            box_bridge.x = x_bridge;
                            
                            clipperPushRestrict(&box_bridge);
                            {
                                display_sprite( icon, 
                                                x_bridge,//tx + box.width*(1 - sc_icon), 
                                                y_bridge,//ty + box.height*(1-sc_bridge)*.5f, 
                                                z_icon-zmod - 1, sc_bridge, sc_bridge, color );
                            }
                            clipperPop();
                        }
                    }
                }
            }
        }
    }

    // --------------------
    // finally

    if( dimsIconRes )
    {
        *dimsIconRes = dimsRes;
    }
    return dSlotWidthRes;
}

static void s_trayobj_displayCb(TrayObj *obj, F32 x, F32 y, F32 z, F32 sc)
{
    Character *pchar = playerPtr()->pchar;
    if( verify( obj 
                && EAINRANGE( obj->invIdx, pchar->conceptInv )
                && pchar->conceptInv[obj->invIdx]))
    {
        uiconcept_DisplayItem( pchar->conceptInv[obj->invIdx]->concept, x - ICON_WD*sc/2.f, y - ICON_WD*sc/2.f, z, ICON_WD_DRAGGING, sc, CLR_WHITE, false, NULL);
    }
}

static int s_iInvFromConceptInventoryItem(Character *pchar, ConceptInventoryItem const *itm)
{
    int res = -1;
    int i;
    if( verify( pchar ))
    {
        for( i = eaSize( &pchar->conceptInv ) - 1; i >= 0; --i)
        {
            if( itm == pchar->conceptInv[i] )
            {
                res = i;
                break;
            }
        }

    }
    // --------------------
    // finally

    return res;

}


static ConceptDisplayInfo concept_display(ConceptInventoryItem *itm, int itm_idx, Character *pchar, ConceptWindowState *state, float x, float y, float z, float sc )
{
    ConceptDisplayInfo res = {0}; 
    float wd = ICON_WD * sc;
    AtlasTex *icon = NULL;
    int color = CLR_WHITE;
    float ty, tx;
    F32 z_icon = z + ARRAY_SIZE( state->growbig.items ) + 10; // the max z an icon will occupy
    F32 z_amount = z_icon + 1;
    bool canInteract = true;
    const ConceptDef *def;
    UIBox dimsConcept = {0};

    if( !verify( itm && itm->concept && itm->concept->def && EAINRANGE( itm_idx, s_state.ppItems ) && s_state.ppItems[itm_idx] == itm ))
    {
        return res;
    }        

    def = itm->concept->def;
    
    ty = y + PIX3*2*sc;
    tx = x + PIX3*2*sc;
    
    // --------------------
    // if we're inventing, you can't interact with this concept unless its usable
    
    if(character_IsInventing( pchar ))
    {
        canInteract = basepower_CanApplyConcept( pchar->invention->recipe->recipe, itm->concept->def);
        // also change the color

        if( !canInteract)
        {
            color = DARK_GREY;
        }
    }
    
    // --------------------
    // draw the icon and amount

    uiconcept_DisplayItem(itm->concept, tx, ty, z, ICON_WD, sc, color, true, &dimsConcept);

    // --------------------
    // print the amount

    {
        F32 x_off = 4;
        F32 y_off = 15;
        font( &tinyfont );
        font_color(0x00deffff, 0x00deffff);
        prnt( tx + x_off*sc, ty + y_off*sc, z_amount, sc, sc, "%d", itm->amount );
    }

    // --------------------
    // handle interaction
    
    {
        CBox box;
        BuildCBox( &box, dimsConcept.x, dimsConcept.y, dimsConcept.width, dimsConcept.height); // always square
        res.box = box;
        
        if( mouseCollision(&box) )
        {
            // set the info text
            // @todo -AB: popover about this :2005 May 04 04:37 PM
            res.mouseOver = true;

            // --------------------
            // mouse interaction

            // set the context menu
            if( mouseClickHit( &box, MS_RIGHT ) )
            {
                int mx, my;
                CMAlign alignVert = CM_TOP;
                CMAlign alignHoriz = CM_LEFT;
                
                rightClickCoords(&mx,&my);
                contextMenu_setAlign( state->rmenu, mx, my, alignHoriz, alignVert, itm );
            }
            else if ( canInteract
                    && mouseLeftDrag( &box ) 
                      && verify(!cursor.dragging)) // just for my understanding
            {

                int iInv = s_iInvFromConceptInventoryItem( pchar, s_state.ppItems[itm_idx]);
                
                // drag the concept
                if(verify(EAINRANGE(iInv, pchar->conceptInv)))
                {
                    ZeroStruct( &cursor.drag_obj );
                    cursor.dragging = TRUE;
                    cursor.dragged_icon = NULL;
                    cursor.dragged_icon_scale = 1.0f;
                    cursor.drag_obj.type = kTrayItemType_ConceptInvItem;
                    cursor.drag_obj.invIdx = iInv;
                    cursor.drag_obj.displayCb = s_trayobj_displayCb;
                    cursor.drag_obj.z = z_icon + 1; //one above the max should work
                    cursor.drag_obj.scale = sc;
                }
            }
        }        
    }
    

    // --------------------
    // finally

    return res;
}


int conceptinvWindow()
{
    float x, y, z, wd, ht, sc;
//     F32 view_y;
    F32 edge_off;
    int i,color, bcolor;
    static ScrollBar sb = { WDW_CONCEPTINV, 0};
    static uiTabControl * tc = NULL;
    Character *pchar;

     if(!window_getDims(WDW_CONCEPTINV, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
        return 0;

    pchar = playerPtr()->pchar;

    if( !tc )
    {
        tc = uiTabControlCreate(TabType_Undraggable, s_tabSelectCb, 0, 0, 0, 0 );
        
        // state
        ZeroStruct(&s_state);
        eaCreate(&s_state.ppItems);
        s_state.invSync.lastInvSize = -1;
        s_state.tc = tc;

        // context menu        
        s_state.rmenu = contextMenu_Create(NULL);

        // init the growbig
        uigrowbig_Init( &s_state.growbig, 1.1, 10 );

        // init
        conceptwindowstate_Sync( &s_state, pchar, TAB_ALL );
    }

    // debugging for EnC, set this every frame.
    uiTabControl_SetSelectCb(tc, s_tabSelectCb);

    // --------------------
    // check for an inventory state change

    {
        int i;
        bool doSync = false;

        // inventory size change
        doSync = s_state.invSync.lastInvSize != eaSize(&pchar->conceptInv);
        
        // inventing state change
        doSync = doSync || (s_state.invSync.isInventing != character_IsInventing(pchar));
        
        // some item in inventory is no longer valid
        for( i = eaSize( &s_state.ppItems ) - 1; i >= 0 && !doSync; --i)
        {
            doSync = !conceptinventoryitem_Valid(s_state.ppItems[i]);
        }

        // init the tab
        if( doSync )
        {
            // sync the state
            conceptwindowstate_Sync( &s_state, pchar, s_state.curTab );
        }

        // update sync state
        s_state.invSync.isInventing = character_IsInventing(pchar);
    }

    // ----------------------------------------
    // post-init and state update

    edge_off = (R10+PIX3)*sc;


    // window frame    
    drawFrame(PIX3, R10, x, y, z, wd, ht, sc, color, bcolor);

    // if not ready yet, return
      if(window_getMode(WDW_CONCEPTINV) != WINDOW_DISPLAYING)
        return 0;

    // tabs
    drawTabControl(tc, x+edge_off*sc, y, z, wd - edge_off*2*sc, 15, sc, color, color, TabDirection_Horizontal);

    // --------------------
    // display window internals 
    {
        TabState *tabState = s_TabState_FromAttrModGrp(&s_state, uiTabControlGetSelectedData(tc));

        int size = eaSize( &s_state.ppItems );
        U32 scaledIconWd = ICON_WD * sc;
        struct MouseOverInfo
        { 
            ConceptItem *cpt;
            int row;
            CBox box;
        } mouseOver = {0};

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
        U32 NUM_COLS = (wd_items - wd_border)/scaledIconWd;
        int slotsUsedTotal = 0; // concepts can take more than one slot, so track
        
        
        // --------------------
        // handle the items
        
         set_scissor(1);
         scissor_dims( x_items, y_items, wd_items, ht_items );
        BuildCBox( &box, x_items, y_items, wd_items, ht_items);
        
        slotsUsedTotal = 0;
        for( i = 0; i < size; ++i ) 
        {
            ConceptInventoryItem *inv = s_state.ppItems[i];
            int slotsUsedFiller = slotsUsedTotal%NUM_COLS + inv->concept->def->slotsUsed > NUM_COLS ? NUM_COLS - slotsUsedTotal%NUM_COLS : 0;
            int row = (slotsUsedTotal+slotsUsedFiller)/NUM_COLS;
            int col = (slotsUsedTotal+slotsUsedFiller)%NUM_COLS;
            F32 col_off = COL_WIDTH*col*sc;
            F32 row_off = ROW_HEIGHT*row*sc;
            F32 y_item = y_items-sb.offset+row_off;
            F32 x_item = box.left+col_off;

            // if there is a wrap due to multi-slot concepts, pad the slots used amount
            slotsUsedTotal += slotsUsedFiller;

//            {
//                int bufferSlots = (int)NUM_COLS - col < inv->concept->def->slotsUsed ? (int)NUM_COLS - col : 0; 
//                slotsUsedTotal += bufferSlots;
//            }

            if( y_item + ROW_HEIGHT*sc > box.top && y_item < box.bottom )
            {
                ConceptDisplayInfo info = {0};
                // display the item
                info = concept_display( inv, i, pchar, &s_state, 
                    x_item, y_item, z, sc );

                if( info.mouseOver && inv && inv->concept)
                {
                    // special check: may be in collision with
                    // combo box, so if its open, don't show
                    if( !tabState->sortbyCombo.state )
                    {
                        mouseOver.cpt = inv->concept;
                        mouseOver.box = info.box;
                        mouseOver.row = row;
                    }
                }
            }
            slotsUsedTotal += inv->concept->def->slotsUsed;
        }
         set_scissor(0);
        
        // --------------------
        // info box

        if( mouseOver.cpt ) 
        {
            TTDrawContext *fnt = &game_9;
            F32 ht_fnt = str_height(fnt, sc, sc, false, "X");
            int cAttribMods = eaSize(&mouseOver.cpt->def->attribMods);
            F32 dht_inf = ht_fnt + PIX3*sc;
            F32 ht_frame = dht_inf*(cAttribMods+1) + PIX3*sc*2;
            F32 x_frame = mouseOver.box.lx;
            F32 y_frame;
            F32 dWdMax = 0.f;
            F32 wd_frame = 0.f;
            F32 z_frame = z + 50;


            // first loop through and get the max width
            for( i = 0; i < cAttribMods; ++i ) 
            {
                if(verify(mouseOver.cpt->def->attribMods[i]))
                {
                    const AttribModGroup *am = mouseOver.cpt->def->attribMods[i];
                    uiAttribModGroup *grp = uiAttribModGroup_GetByStr(am->name);
                    CBox box = {0};
                    if( verify( grp ))
                    {
                        char *grpName = textStd(grp->pchName);

                        // NOTE: keep in sync with prnt stmt below
                        str_dims(fnt,sc,sc,false,&box, "%s %.2f", grpName, mouseOver.cpt->afVars[i] );
                        dWdMax = MAX(dWdMax, box.right - box.left);
                    }
                }
            }

            // NOTE: keep in sync with prnt stmt below
            {
                ConceptItem *concept = mouseOver.cpt;
                F32 hardenAdj = concept->def->slotSuccessChance;
                str_dims(fnt,sc,sc,false,&box, "%s %.1f", textStd("InventionToHardenAdj"), hardenAdj );
                dWdMax = MAX(dWdMax, box.right - box.left);
            }

            wd_frame = dWdMax + R10*sc*2;

            if( mouseOver.box.ly - ht_frame > y + TAB_HEIGHT*sc )
            {
                y_frame = mouseOver.box.ly - ht_frame - PIX3*sc;
            }
            else
            {
                y_frame = mouseOver.box.ly + mouseOver.box.bottom - mouseOver.box.top;
            }

            // if the frame is outside the window, bring it in
            if( x_frame + wd_frame >= x + wd + PIX3*sc*2 )
            {
                x_frame = x + wd - PIX3*sc*2 - wd_frame;
            }

            // print the attribmods
            // NOTE: keep in sync with prnt stmt above
            {
                F32 x_inf = x_frame + R10*sc;
                F32 y_inf = y_frame + PIX3*sc;
                F32 z_inf = z_frame + 1;

                font(fnt);
                font_color(CLR_WHITE, CLR_WHITE);

                for( i = 0; i < cAttribMods; ++i ) 
                {
                    if(verify(mouseOver.cpt->def->attribMods[i]))
                    {
                        const AttribModGroup *am = mouseOver.cpt->def->attribMods[i];
                        uiAttribModGroup *grp = uiAttribModGroup_GetByStr(am->name);
                        CBox box = {0};
                        if( verify( grp ))
                        {
                            F32 y_itm = y_inf + (i+1)*dht_inf;
                            F32 dWdCur = 0.f;
                            char *grpName = textStd(grp->pchName);

                            str_dims(fnt,sc,sc,false,&box, "%s %.1f", grpName, mouseOver.cpt->afVars[i] );
                            prnt(x_inf, y_itm, z_inf+1, sc, sc, "%s %.1f", grpName, mouseOver.cpt->afVars[i] );

                            dWdCur = box.right - box.left;
                            dWdMax = MAX(dWdMax, dWdCur);
                        }
                    }
                }

                // always show
                // NOTE: uses 'i' from above
                // NOTE: keep in sync with prnt stmt above
                {
                    F32 y_itm = y_inf + (i+1)*dht_inf;
                    ConceptItem *concept = mouseOver.cpt;
                    F32 hardenAdj = concept->def->slotSuccessChance;
                    str_dims(fnt,sc,sc,false,&box, "%s %.2f %", textStd("InventionToHardenAdj"), hardenAdj );
                    prnt(x_inf, y_itm, z_inf+1, sc, sc, "%s %.2f", textStd("InventionToHardenAdj"), hardenAdj );

                    dWdMax = MAX(dWdMax, box.right - box.left);
                }
            }

            {
                int clr_inf = CLR_BLACK;
                int bclr_inf = CLR_BLACK;

                drawFlatFrame(PIX3, R10, x_frame, y_frame, z_frame, 
                    wd_frame, ht_frame, sc, clr_inf, bclr_inf );
            }
        }

        //scrollbar
        {
            F32 doc_bottom = y_items + (slotsUsedTotal+NUM_COLS-1)/NUM_COLS*scaledIconWd;
            doScrollBar( &sb, ht - edge_off*2 - (TAB_HT+20)*sc, doc_bottom - y, wd, (PIX3+R10+TAB_HT)*sc, z+5, 0, 0 );
        }
        
        // grow the current icon
        uigrowbig_Update(&s_state.growbig, mouseOver.cpt );
        
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
                if( INRANGE0( i, kConceptSortType_Count ))
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
