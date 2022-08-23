/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "bases/DetailRecipe.h"
#include "uiSalvage.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiStoredSalvage.h"
#include "bases/basestorage.h"
#include "uiGrowBig.h"
#include "entity/salvage.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>

#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "player/badges.h"
#include "player/badges_client.h"

#include "gameComm/wdwbase.h"
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
#include <utilitieslib/components/StashTable.h>

#include <utilitieslib/language/MessageStore.h>
#include "entity/salvage.h"
#include "entity/character_base.h"
#include "entity/character_inventory.h"
#include "UI/uiComboBox.h"
#include "UI/uiContextMenu.h"
#include "UI/uiNet.h"
#include <utilitieslib/utils/error.h>
#include "UI/uiCursor.h"
#include "UI/uiTray.h"
#include "UI/uiTrade.h"
#include "entity/entVarUpdate.h"
#include "UI/uiChat.h"
#include "uiBaseStorage.h"
#include "UI/uiGift.h"
#include "bases/bases.h"
#include "UI/uiDialog.h"
#include "UI/uiOptions.h"
#include "sound/sound.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "uiAmountSlider.h"

#define STOREDSALVAGE_TITLE "PersonalInventionStorage"

static int storedsalvage_context_wdw = 0;

static bool s_bReparse = true;
void storedSalvageReparse() 
{
    s_bReparse = true;
}

typedef struct StoredSalvageWindowState
{
    bool init;
    SalvageWindowState salvage_wdw_state;
    SalvageInventoryItem **ppItems;
    uiGrowBig growbig;
    
    SalvageSortType sort;
    ComboBox sortbyCombo;

    BOOL has_anchor_pos;
    Vec3 anchor_pos;
} StoredSalvageWindowState;


static StoredSalvageWindowState s_storedsal_state = {0};

static SalvageInventoryItem** s_GetPlayerStoredSalvageInv()
{
    Character *pchar = playerPtr()->pchar;
    return pchar->storedSalvageInv;
}

static void s_fillFromInv(StoredSalvageWindowState *s, SalvageSortType sort)
{
    int i;
    Character *pchar = playerPtr()->pchar;
    SalvageInventoryItem **src = SalvageInventoryItem_GatherMatching(pchar->storedSalvageInv, "P1273912828");

    if (!src || !INRANGE0(sort, kSalvageSortType_Count))
        return;
    
    s->sort = sort;
    combobox_setId(&s->sortbyCombo, sort, TRUE );

    eaSetCapacity( &s->ppItems, eaSize(&src));
    eaSetSize(&s->ppItems,0);
    for( i = 0; i < eaSize(&src) ; ++i )
        if( salvageinventoryitem_Valid( src[i] ) )
            eaPush(&s->ppItems, src[i]);

    // first sort by name ascending, then the given sort
    stableSort( s->ppItems, eaSize(&s->ppItems), sizeof(*s->ppItems), (void*)kSalvageSortType_Name_Ascending, uiSalvageSort);
    stableSort( s->ppItems, eaSize(&s->ppItems), sizeof(*s->ppItems), (void*)sort, uiSalvageSort);
}

static void storedsalvagewindowstate_Init(StoredSalvageWindowState *state)
{
    ZeroStruct( state );
    state->salvage_wdw_state.GetSalvageInv = s_GetPlayerStoredSalvageInv;
    
    // ----------
    // items
    
    eaCreate(&state->ppItems);
    
    // --------------------
    // init the items
    
    s_fillFromInv(state,kSalvageSortType_Name_Ascending);

    // --------------------
    // init the sort-by combo

    uiSalvage_initSortCombo(WDW_STOREDSALVAGE, &state->sortbyCombo, false, true); // -type +rarity // TODO: this could be automated like salvagewindowstate_Init

    // ----------
    // init the growbig

    uigrowbig_Init( &state->growbig, 1.1, 10 );

    // ----------
    // done

    state->init = TRUE;
}

static float s_storedsalvage_flash;
void storedsalvageWindowFlash(void)
{
    s_storedsalvage_flash = .0001f;
}

#define TOP_HT        36
#define BOTTOM_HT    24

void storedsalvage_drag(int index, AtlasTex *icon, float sc) {
    TrayObj to;
    to.type = kTrayItemType_PersonalStorageSalvage;
    to.invIdx = index;
    to.scale = sc;
    trayobj_startDragging(&to,icon,NULL);
}

void storedsalvage_dragEx(SalvageInventoryItem *sal, AtlasTex *icon, float sc)
{
    Entity *e = playerPtr();
    int iInv = e ? eaFind(&e->pchar->storedSalvageInv,sal) : -1;

    // drag the concept
    if(iInv != -1)
        storedsalvage_drag(iInv,icon,sc);
}

void ShowStoredSalvage(bool has_anchor_pos, Vec3 anchor_pos)
{
    window_setMode(WDW_STOREDSALVAGE, WINDOW_GROWING);
    s_storedsal_state.has_anchor_pos = has_anchor_pos;
    if( has_anchor_pos )
        copyVec3(anchor_pos,s_storedsal_state.anchor_pos);
}

void storedSalvageDrawCallback(void *userData, float x, float y, float z, float sc, int clr)
{
    SalvageInventoryItem *sal = character_FindSalvageInv(playerPtr()->pchar, (char*)userData);
    if ( sal && sal->salvage )
        display_sprite(atlasLoadTexture(sal->salvage->ui.pchIcon), x, y, z, sc, sc, clr);
}

void storedsalvage_MoveFromPlayerInv(int amount, char *nameSalvage)
{
    Character *pchar = playerPtr()->pchar;
    if(pchar->storedSalvageInvCurrentCount >= character_GetInvTotalSize(pchar, kInventoryType_StoredSalvage))
    {
        addSystemChatMsg(textStd("BaseStorageFull",STOREDSALVAGE_TITLE),INFO_USER_ERROR,0);
        storedsalvageWindowFlash();
        if(!SAFE_MEMBER2(pchar,entParent,access_level))
            return;
    }
    storedsalvage_SendMoveFromPlayerInv(amount,nameSalvage);
}

void storedsalvage_MoveToPlayerInv(int amount, char *nameSalvage)
{
    Character *pchar = playerPtr()->pchar;
    if(pchar->salvageInvCurrentCount >= character_GetInvTotalSize(pchar, kInventoryType_Salvage))
    {
        addSystemChatMsg(textStd("InventionSalvageFull"),INFO_USER_ERROR,0);
        salvageWindowFlash();
        if(!SAFE_MEMBER2(pchar,entParent,access_level))
            return;
    }
    storedsalvage_SendMoveToPlayerInv(amount,nameSalvage);
}

int storedSalvageWindow()
{
    static bool storedsal_opened = FALSE;
    CBox boxItms = {0};    
    CBox boxBtm = {0};    
    float x, y, z, wd, ht, sc;
    int color, bcolor;
    static ScrollBar sb = { WDW_STOREDSALVAGE, 0};
    Character *pchar;
    int curTab = -1;

     if(!window_getDims(WDW_STOREDSALVAGE, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
    {
        if(storedsal_opened)
            s_storedsal_state.has_anchor_pos = 0;
        storedsal_opened = 0;
        storedSalvageReparse();
        return 0;
    }

    if(s_storedsal_state.has_anchor_pos && distance3Squared(ENTPOS(playerPtr()), s_storedsal_state.anchor_pos) > SQR(20))
    {
        drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );
        window_setMode(WDW_STOREDSALVAGE, WINDOW_SHRINKING);
        return 0;
    }

    pchar = playerPtr()->pchar;
    if( !s_storedsal_state.init )
    {
        // It's possible to arrive here after the open request has been made, in which case s_storedsal_state.has_anchor_pos and s_storedsal_state.anchor_pos
        // will already be valid.  So we save them across the init, to prevent them getting blown away.
        int saved_has_anchor_pos;
        Vec3 saved_anchor_pos;

        saved_has_anchor_pos = s_storedsal_state.has_anchor_pos;
        copyVec3(s_storedsal_state.anchor_pos, saved_anchor_pos);

        storedsalvagewindowstate_Init(&s_storedsal_state);

        s_storedsal_state.has_anchor_pos = saved_has_anchor_pos;
        copyVec3(saved_anchor_pos, s_storedsal_state.anchor_pos);
    }
    
    // --------------------
    // check for an inventory state change
    // When salvage inventory changes, s_bReparse will get set

    if( s_bReparse )
    {
        s_fillFromInv(&s_storedsal_state,s_storedsal_state.sort);
        s_bReparse = false;
    }

    // --------------------
    // window frame

    if( s_storedsalvage_flash )
    {
        int flash = sinf(s_storedsalvage_flash)*128;
        bcolor |= ((flash*2)<<24)|(flash<<16)|(flash<<8)|(flash);
        s_storedsalvage_flash += TIMESTEP/4;
        if( s_storedsalvage_flash > PI )
            s_storedsalvage_flash = 0.f;
    }

    drawFrame(PIX2, R10, x, y, z, wd, TOP_HT*sc, sc, color, bcolor);
    drawFrame(PIX3, R10, x, y+(TOP_HT-PIX2)*sc, z, wd, ht-(BOTTOM_HT+TOP_HT-PIX4)*sc, sc, color, bcolor);
    drawFrame(PIX2, R10, x, y+ht-BOTTOM_HT*sc, z, wd, BOTTOM_HT*sc, sc, color, bcolor);

    // if not ready yet, return
      if(window_getMode(WDW_STOREDSALVAGE) != WINDOW_DISPLAYING)
        return 0;

    storedsal_opened = TRUE;
    
    // ----------
    // display 

    // title
    font( &game_18 );
    font_color(CLR_WHITE, CLR_WHITE);
    prnt( x + 12*sc, y + 26*sc, z+1, sc, sc, textStd(STOREDSALVAGE_TITLE));

    // inventory size
    font( &game_12 );
    if (pchar->storedSalvageInvCurrentCount < character_GetInvTotalSize(pchar, kInventoryType_StoredSalvage))
        font_color(CLR_GREY, CLR_GREY);
    else
        font_color(CLR_RED, CLR_RED);
    prnt( x + wd - 60.f*sc, y + 24*sc, z+1, sc, sc, "%d/%d", pchar->storedSalvageInvCurrentCount, character_GetInvTotalSize(pchar, kInventoryType_StoredSalvage) );

    // items
    UISalvage_DisplayItems(WDW_STOREDSALVAGE, x,y+TOP_HT*sc,z,wd,ht-(TOP_HT+BOTTOM_HT+PIX2)*sc,sc,&sb, s_storedsal_state.ppItems, &s_storedsal_state.growbig, TRUE, &boxItms, &boxBtm, storedsalvage_dragEx);

    // empty message
    if(!pchar->storedSalvageInvCurrentCount)
    {
        int x = boxItms.left;
        int y = boxItms.top;
        F32 wd = CBoxWidth(&boxItms);
        F32 ht = CBoxHeight(&boxItms);
        
        font(&game_18);
        font_color(0xffffff22 ,0xffffff22 );
        cprntEx( x + wd/2, y + ht/2, z, 1.5f, 1.5f, CENTER_X|CENTER_Y, "DragSalvageHere" );
    }

    // sort combo
    // due to a feature of combobox, need to multiply by inverse scale
    combobox_setLoc(&s_storedsal_state.sortbyCombo, (wd - 164.f*sc)/sc, (ht - BOTTOM_HT*sc)/sc, -1);
    combobox_display(&s_storedsal_state.sortbyCombo);
    if( s_storedsal_state.sortbyCombo.newlySelectedItem )
    {
        SalvageSortType sort = uiSalvage_SortTypeFromCombo(&s_storedsal_state.sortbyCombo);
        if(sort != kSalvageSortType_Count )
            s_fillFromInv( &s_storedsal_state, sort);
        s_storedsal_state.sortbyCombo.newlySelectedItem = FALSE;
    }


    // --------------------
    // drag and drop interaction

    if ( cursor.dragging && mouseCollision(&boxItms) 
         && !isDown(MS_LEFT) && cursor.drag_obj.type == kTrayItemType_Salvage )
    {
        Character *pchar = playerPtr()->pchar;
        SalvageInventoryItem *sal = eaGet(&pchar->salvageInv,cursor.drag_obj.invIdx);
        const char *nameSalvage = (sal&&sal->salvage)?sal->salvage->pchName:NULL;
#define MOVE_STACK -1
        if(nameSalvage)
            amountSliderWindowSetupAndDisplay(1,sal->amount, NULL, storedSalvageDrawCallback, storedsalvage_MoveFromPlayerInv, cpp_const_cast(char*)(nameSalvage), 0);
        trayobj_stopDragging();
    }
    
    // -----
    // finally

    return 0;
}
