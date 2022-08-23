/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "uiInventory.h"
#include "UI/uiCursor.h"
#include "UI/uiInspiration.h"
#include "UI/uiInput.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiWindows.h"
#include "UI/uiTray.h"
#include "UI/uiCursor.h"
#include "UI/uiChat.h"
#include "UI/uiToolTip.h"
#include "UI/uiGame.h"
#include "UI/uiNet.h"
#include "UI/uiTrade.h"
#include "UI/uiContextMenu.h"
#include "UI/uiWindows_init.h"
#include "UI/uiGift.h"

#include "entity/character_base.h"
#include "gameComm/wdwbase.h"
#include "win/win_init.h"
#include "entity/powers.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "player/player.h"
#include "entity/entity.h"
#include "entity/entVarUpdate.h"
#include "clientcomm/clientcomm.h"
#include "cmdparse/cmdcommon.h"
#include "entity/entity_power_client.h"
#include "win/input.h"
#include "cmdparse/cmdcommon.h"
#include "cmdparse/cmdgame.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/mathutil.h>
#include "entity/Invention.h"

#include <utilitieslib/utils/strings_opt.h>


typedef enum UiInvType
{
    UiInvType_Salvage,
    UiInvType_Concept,
    UiInvType_Recipe,
    UiInvType_Count
} UiInvType;

static bool UiInvType_Valid(UiInvType type)
{
    return INRANGE0( type, UiInvType_Count );
};

static ToolTip toolTips[UiInvType_Count] = {0};

#define ICON_WD                32        // Size of inspiration icon
#define SPACER                4        // space between icons

static void drawInvSlot( UiInvType type, float x, float y, float z, float scale, float wd, float ht, int bcolor )
{
    AtlasTex *back = atlasLoadTexture( "Insp_ring_back.tga" );
       AtlasTex *border = atlasLoadTexture( "Insp_ring.tga" );
    CBox box;
    float sc = 1.f;
    
    if( !verify( UiInvType_Valid(type)))
    {
        return;
    }

    // --------------------
    // interaction

    // get the slot collision box
       BuildCBox( &box, x+(type*(ICON_WD+SPACER)-SPACER/2)*scale, y+ht-ICON_WD*scale, (ICON_WD+SPACER)*scale, ICON_WD*scale );
    
    // mouse is over inspiration
    if( mouseCollision( &box ) )
    {
        if( mouseLeftClick() )
        {
            // @todo -AB: open the window for this type :2005 Apr 25 03:02 PM
        }
        
        if( mouseRightClick() )
        {
            // yadda yadda
        }
    }

    // --------------------
    // display

       display_sprite( back, x + (type*(ICON_WD+SPACER) + ICON_WD/2*(1-sc))*scale, y + ht + (-(1)*(ICON_WD) + ICON_WD/2*(1-sc))*scale, z,
                      ((float)ICON_WD/back->width)*sc*scale, ((float)ICON_WD/back->height)*sc*scale, CLR_WHITE );
    display_sprite( border, x + (type*(ICON_WD+SPACER) + ICON_WD/2*(1-sc))*scale, y + ht + (-(1)*(ICON_WD) + ICON_WD/2*(1-sc))*scale, z,
        ((float)ICON_WD/border->width)*sc*scale, ((float)ICON_WD/border->height)*sc*scale, bcolor );
}

#define BUTTON_HEIGHT 20
#define BUTTON_WIDTH 100


int inventoryWindow()
{
    float x, y, z, wd, ht, sc;
    int i,color, bcolor;
    int res = 0;
    static int first = true;

    if( first )
    {
        for( i = 0; i < ARRAY_SIZE( toolTips ); ++i ) 
        {
            ZeroStruct(toolTips+i);
            addToolTip(toolTips+i);
        }
        
        
        first = false;
    }

     if(!window_getDims(WDW_INVENTORY, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
        return 0;

     drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );

      set_scissor( TRUE );
     scissor_dims( x + PIX3*sc, y + PIX3*sc, wd - PIX3*2*sc, ht - PIX3*2*sc );
    
    {
        F32 z_btn = z+1;
        int txtColor = CLR_BLUE;
        int cntBtns = UiInvType_Count;
        int dispx, dispy;
        Character *pchar = playerPtr()->pchar;
        struct InvInfo
        {
            char *str;
            int idWdw;
        } infos[UiInvType_Count+1] = 
            {
                { "UiInventoryTypeSalvage", WDW_SALVAGE },
                { "UiInventoryTypeConcept", WDW_CONCEPTINV },
                { "UiInventoryTypeRecipe", WDW_RECIPEINV },
                { "UiInventoryTypeInvent", WDW_INVENT }
            };
        STATIC_INFUNC_ASSERT(ARRAY_SIZE(infos) == UiInvType_Count+1);
        
        if( character_IsInventing(pchar) )
        {
            cntBtns++;
        }
        
        dispx = x + PIX3*sc;
        dispy = y + PIX3*sc;
        
        for( i = 0; i < cntBtns; ++i ) 
        {
            char *str = infos[i].str;
            if( verify(str) 
//                && D_MOUSEHIT == drawTextButton( str, prev.x, prev.y + prev.height  + PIX3*sc, z_btn, sc, color, overColor, txtColor, TRUE, &prev) )
                && D_MOUSEHIT == drawStdButton(dispx + BUTTON_WIDTH*sc/2,dispy + BUTTON_HEIGHT*sc/2,z_btn,BUTTON_WIDTH*sc,BUTTON_HEIGHT*sc,color,str,1.0,0))
            {
                // open/close
                window_openClose( infos[i].idWdw );
            }
            dispy += BUTTON_HEIGHT*sc;
        }

        window_setDims(WDW_INVENTORY, -1, -1, (BUTTON_WIDTH + PIX3*sc*2)*sc, (BUTTON_HEIGHT*cntBtns+PIX3*2)*sc );
    }
    

    set_scissor( FALSE );

    // --------------------
    // finally

    return res;
}
