/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "entity/powers.h"
#include "entity/power_system.h"
#include "uiInvent.h"
#include "UI/uiTray.h"
#include "uiConceptInv.h"
#include "uiRecipeInv.h"
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
#include "uiWindows.h"
#include "UI/uiWindows.h"
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
#include "UI/uiUtilGame.h"
#include "entity/RewardSlot.h"
#include "entity/Invention.h"
#include "gameComm/trayCommon.h"
#include <utilitieslib/utils/strings_opt.h>
#include "graphics/ttFont.h"
#include <utilitieslib/language/MessageStoreUtil.h>

#define TAB_HT        20
#define START_ICON_WD 38
static U32 ICON_WD = START_ICON_WD;        // Size of icon
static U32 COL_WIDTH = START_ICON_WD;

typedef enum UiInventState
{
    kUiInventState_SelectingLevel,
    kUiInventState_Inventing,

    // ---
    kUiInventState_Count
} UiInventState;


typedef struct InventWindowState
{
    UiInventState state;
    Invention *pInvention;
    ContextMenu * rmenu;
    uiGrowBig growbig;
} InventWindowState;
static InventWindowState s_state;


static int s_dTrayslotDrawConcept( InventionSlot *slot, float xp, float yp, float zp, float sc, UIBox *bxIcon )
{
    int dSlotWidthRes = 1; // 1 by default
    AtlasTex * ic;
    int     clr = CLR_WHITE;
    bool    usable = true;
    CBox    box;

    static AtlasTex * power_ring;
    static AtlasTex * spin;
    static AtlasTex * autoexec;
    static AtlasTex * target;
    static AtlasTex * queued;
    static AtlasTex *power_back;
    static AtlasTex * ap;
    static texBindInit=0;

    if( !verify( slot ))
    {
        return dSlotWidthRes;
    }

    if (!texBindInit) {
        texBindInit = 1;
        power_ring            = atlasLoadTexture( "tray_ring_power.tga"             );
        spin                  = atlasLoadTexture( "tray_ring_endurancemeter.tga"    );
        autoexec              = atlasLoadTexture( "tray_ring_iconovr_autoexecute.tga" );
        target                = atlasLoadTexture( "tray_ring_iconovr_3Dcursor.tga"    );
        queued                = atlasLoadTexture( "tray_ring_iconovr_cued.tga"        );
        ap                    = atlasLoadTexture( "GenericPower_AutoOverlay.tga" );
        power_back            = atlasLoadTexture( "EnhncTray_RingHole.tga"          );
    }

//     if( stricmp( ic->name, "white" ) == 0)
//         ic = atlasLoadTexture( "Power_Missing.tga" );
    ic = power_ring;

    if( invention_SlotHardened(slot) )
    {
        usable = false;
        clr = 0xffffff66;
    }

    {
        F32 x_icon = xp;// - ic->width*sc/2;
        F32 y_icon = yp;//- ic->width*sc/2;
        F32 wd_icon = ic->width*sc;
        F32 z_back = zp + 1;
        F32 z_icon = zp + 2;

        BuildCBox( &box, x_icon, y_icon, wd_icon, wd_icon);

        if( mouseCollision(&box) && usable )
        {
            sc *= .9f;
            if( isDown(MS_LEFT) )
                sc *= .9f;
        }

        // ----------------------------------------
        // display

        {
            F32 sc_back = (F32)ic->width/power_back->width*sc;
            display_sprite(ic,x_icon,y_icon,z_back,sc,sc,clr);
            display_sprite(power_back,x_icon,y_icon,z_back,sc_back,sc_back,0xffffff66);

            if( eaSize(&slot->concepts) > 0 && verify(slot->concepts[0]))
            {
                ConceptItem *cpt = slot->concepts[0];
                bool bDrawTextCur = true;
                int clr;
                
                switch ( slot->state )
                {
                case kSlotState_Slotted:
                {
                    clr = CLR_GREYED;
                }
                break;
                case kSlotState_HardenSuccess:
                {
                    clr = CLR_WHITE;
                }
                break;
                case kSlotState_HardenFailure:
                {
                    clr = CLR_BLACK;
                    bDrawTextCur = false;
                }
                break;
                default:
                    verify(0 && "invalid switch value.");
                };
                
                dSlotWidthRes = uiconcept_DisplayItem( cpt, x_icon, 
                                                       y_icon, z_icon, 
                                                       ic->width, sc, 
                                                       clr, 
                                                       bDrawTextCur,
                                                       bxIcon);
            }
            else
            {
                if( bxIcon )
                {
                    bxIcon->x = x_icon;
                    bxIcon->y = y_icon;
                    bxIcon->width = wd_icon;
                    bxIcon->height = wd_icon;
                }
            }
        }
    }

    return dSlotWidthRes;
}

typedef struct TrayslotDisplayInfo
{
    bool pMouseOver;
    int dSlotWidth;
} TrayslotDisplayInfo;


static TrayslotDisplayInfo s_trayslot_Display( Character *pchar, InventionSlot *slot, int iSlot, int curItem, float xp, float yp, float zp, float scale, bool inSlotRange )
{
    static AtlasTex * power_ring; // remove
    static texBindInit=0;
    TrayslotDisplayInfo res = {0};
    char    temp[128];
    CBox box;
    AtlasTex * spr;
    UIBox bxConcept;
    
    if( !verify( slot ))
    {
        return res;
    }

    if (!texBindInit) {
        texBindInit = 1;
        power_ring            = atlasLoadTexture( "tray_ring_power.tga"             );
    }


    // --------------------
    // display the little numbers on the slots

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("tray_ring_number_");
    STR_COMBINE_CAT_D((iSlot+1)%10);
    STR_COMBINE_CAT(".tga");
    STR_COMBINE_END();
    spr = atlasLoadTexture( temp );
    display_sprite( spr, xp, yp + (power_ring->height - spr->height/2)*scale, zp + 3, scale, scale, CLR_WHITE );


    // --------------------
    // display

    res.dSlotWidth = s_dTrayslotDrawConcept(slot, xp, yp, zp, scale, &bxConcept);
    BuildCBox( &box, bxConcept.x, bxConcept.y, bxConcept.width, bxConcept.height );
    
    // highlight the slot to fill
    {
        
    }

    // --------------------
    // interaction

    if( slot->state == kSlotState_Empty )
    {
        int back_color = 0x00000088;
        float sc = 1.f;
        float holesc = .5f;

        // --------------------
        // dragging

        // todo: make sure next N slots are free
        // NOTE: can't drag n drop when getting info from server
        if ( cursor.dragging && character_InventingCanUpdate(pchar)) 
        {
            //OverlapCBoxCBox
            bool mouseCol = mouseCollision(&box);
            if( mouseCol || inSlotRange )
                sc *= .9;

            if( mouseCol )
            {
                res.pMouseOver = true; // over an empty
            }

            if( mouseCollision(&box) && !isDown(MS_LEFT) )
            {
                verify(character_InventingSlotConcept(pchar, iSlot, cursor.drag_obj.invIdx ));
                trayobj_stopDragging();
            }
        }
    }
    else // tray slot with concept in it
    {
//         if( ts->state == 1 )
//             traySlot_fireFlash( ts, xp, yp, zp );

        if ( cursor.dragging )
        {
            // dragged onto item alreay in tray
            if ( mouseCollision(&box) && !isDown(MS_LEFT) )
            {
                //trayobj_stopDragging();
            }
        }
        else if ( mouseLeftDrag( &box ) )
        {
            // what to do if dragging?
//             AtlasTex *ic = tray_GetIcon(ts, e);
//             trayobj_startDragging( ts, ic, NULL );  // or start dragging if and inventory is up
//             cursor.drag_obj.islot = i;          // tag the slot this power is comming from
//             cursor.drag_obj.itray = current_tray;
            
        }
        else //if( mouseCollision( &box ) )
        {
            if( slot->state == kSlotState_Slotted )
            {
                int color = CLR_GREYED;
                int overColor = CLR_WHITE;
                int txtColor = CLR_WHITE;
                F32 z_btn = zp + 50;
                UIBox prev = {0};
                F32 x_harden = bxConcept.x;// + bxConcept.width/2;
                F32 y_harden = bxConcept.y;// + bxConcept.height/3;
                F32 dx_cancel = 0.f;
                F32 dy_cancel = PIX3*scale;
                bool bCanUpdateInvention = character_InventingCanUpdate(pchar);
                
                if( D_MOUSEHIT == drawTextButton( "InventHardenConcept", x_harden, y_harden, z_btn, scale, color, overColor, txtColor, bCanUpdateInvention, &prev) )
                {
                    // harden this
                    verify(character_InventingHardenConcept(pchar, iSlot));
                }

                if( D_MOUSEHIT == drawTextButton( "InventUnslotConcept", prev.x + dx_cancel, prev.y + dy_cancel + prev.height, z_btn, scale, color, overColor, txtColor, bCanUpdateInvention, &prev) )
                {
                    verify(character_InventingUnSlotConcept( pchar, iSlot ));
                }

                // show the % chance to harden
                {
                    F32 fntHt = ttGetFontHeight(&game_9, scale, scale );
                    font(&game_9);
                    cprnt( prev.x + prev.width/2, prev.y + dy_cancel + prev.height + fntHt, z_btn + 100, scale, scale, "%.0f%%", slot->concepts[0]->def->slotSuccessChance );
                }
            }
        }

        // --------------------
        // context help

        if( mouseClickHit( &box, MS_RIGHT ) )
        {
            int x, y;
            CMAlign alignVert;
            CMAlign alignHoriz;

            rightClickCoords( &x, &y );
            alignHoriz = CM_LEFT;
            alignVert = CM_TOP;
        }
    }

    // --------------------
    // finally

    return res;

}


static void s_graphdraw(Invention *inv, F32 tx, F32 ty, F32 z, F32 wd_graph, F32 ht_graph, F32 sc, int clr_graph, int bclr_graph )
{
    if(verify(inv))
    {
        const BasePower *def = inv->recipe->recipe;
        F32 wd_bar_sep = PIX3*sc;
        F32 wd_bar_area = wd_graph/4.f - wd_bar_sep;
        UIBox box_graph = { tx, ty, wd_graph, ht_graph };
        F32 z_graph = z+1;
        const PowerVar **ppBars = NULL;
        int size;
        int i;
        int wdBorder = PIX2;

        drawFlatFrame(wdBorder, R10, box_graph.x, box_graph.y, 
            z_graph, box_graph.width, box_graph.height, 
            sc, clr_graph, bclr_graph );

        {
            F32 x_zero = tx + wdBorder*sc;
            F32 y_zero = ty + ht_graph/2;
            F32 wd_zero = wd_graph - wdBorder*sc*2;
            F32 ht_zero = 1.f;
            UIBox bxZero = {x_zero, y_zero, wd_zero, ht_zero};
            AtlasTex *txWhite = atlasLoadTexture("white.tga");
            clipperPushRestrict(&bxZero);
            if(txWhite)
            {
                F32 sc_zero = wd_zero/txWhite->width;
                display_sprite( txWhite, x_zero, y_zero, z_graph+1, sc_zero, sc_zero, CLR_WHITE );
            }
            clipperPop();
        }

        //get the unique bars
        for( i = eaSize(&def->ppVars) - 1; i >= 0; --i)
        {
            const PowerVar *var = def->ppVars[i];
            if(verify(var) && eaFind(&ppBars, var) < 0)
            {
                eaPushConst(&ppBars, var);
            }
        }

        //render the bars
        size = eaSize(&ppBars);
        for( i = eaSize(&ppBars) - 1; i >= 0; --i)
        {
            const PowerVar *var = ppBars[i];

            // get the starting value
            // if fMin > 0, choose min as start
            // if fMax < 0, choose it
            // else choose 0.
            F32 valStart = MIN(MAX(0.f, var->fMin),var->fMax);
            F32 actualVal = 0.f; 
            F32 potentialVal = 0.0f; // 0.f
            uiAttribModGroup *grp = uiAttribModGroup_GetByStr(var->pchName);

            if( !verify( var && var->pchName && grp ))
            {
                continue;
            }

            invention_ValsForVar(inv, var, &actualVal, &potentialVal);

            // render the bar
            {
                int potentialclr = CLR_WHITE;
                TTDrawContext *fnt = &game_9;
                F32 ht_desc = (24 + PIX3)*sc;
                F32 z_bar = z_graph + 1;
                F32 varmin = -5.f;
                F32 varmax = 5.f;

                UIBox box_bar = 
                {
                    box_graph.x + (wd_bar_sep + wd_bar_area)*(size - i - 1) + PIX3*sc,
                        box_graph.y + PIX3*sc,
                        wd_bar_area,
                        box_graph.height - ht_desc 
                };
                UIBox bxVarsActual = {0};

                // first draw the white background for the potential
                uiinvent_drawGraphBar(&box_bar,
                    var->fMin, var->fMax - var->fMin,
                    varmin, varmax,
                    z_bar,
                    grp->pogName, 0xffffff44);

                // draw the actual
                uiinvent_drawGraphBar(&box_bar,
                    valStart, actualVal - valStart,
                    varmin,varmax,
                    z_bar+1,
                    grp->pogName, CLR_WHITE );

                // draw the potential
                uiinvent_drawGraphBar(&box_bar,
                    actualVal, 
                    potentialVal - actualVal,
                    varmin,varmax,
                    z_bar+2,
                    grp->pogName, 0xffffff66 ); 



                // draw the text associated with it
                {
                    F32 x_txt = box_bar.x + box_bar.width/2;
                    F32 y_vars = box_graph.y + box_graph.height - ht_desc/2.f;
                    F32 y_txt = y_vars + ht_desc/2.f;
                    font(fnt);

                    font_color(0x00deffff, 0x00deffff);
                    cprnt(x_txt, y_vars, z_bar, sc, sc, "%.2f (%.2f)", actualVal, potentialVal );
                    cprnt(x_txt, y_txt, z_bar, sc, sc, grp->pchName);
                }
            }
        }

        // --------------------
        // finally

        eaDestroyConst(&ppBars);
    }
}


#define MAX_POWERUP_DISPLAY_COUNT 10

//------------------------------------------------------------
//  display the slot
//----------------------------------------------------------
static void s_displayEnhancement(Invention *inv, F32 tx, F32 ty, F32 z, F32 wd, F32 ht, F32 sc, int clr, int bclr)
{
    if(verify(inv))
    {
        const BasePower *def = inv->recipe->recipe;
        F32 wd_enh = 64.f;
        AtlasTex *icon = atlasLoadTexture( def->pchIconName );
        F32 x_enh = tx + wd - icon->width*sc - PIX3*sc;
        F32 y_enh = ty + PIX3*sc;
        F32 sc_enh = wd_enh/icon->width*sc;
        RewardSlot **ppPowerupSlots = NULL;
        RewardSlot **ppPotentialPowerupSlots = NULL;
        int i;
        int iSlotCur = 0;

        // gather the powerup cost
        for( i = 0; i < ARRAY_SIZE(inv->slots);++i )
        {
            InventionSlot *slot = inv->slots + i;
            const RewardSlot ***hAccum = NULL;
            int icpt;
            int dSlotsUsedMax = 0;

            // get the right accum
            if( invention_SlotHardened( slot ))
            {
                hAccum = &ppPowerupSlots;
            }
            else if( slot->state == kSlotState_Slotted )
            {
                hAccum = &ppPotentialPowerupSlots;
            }

            if( hAccum )
            {
                for( icpt = eaSize( &slot->concepts ) - 1; icpt >= 0; --icpt)
                {
                    const ConceptDef *def = slot->concepts[icpt] ? slot->concepts[icpt]->def : NULL;
                    if( verify( def ))
                    {
                        // if it is ever written...
                        //EArrayAppend(hAccum, &def->powerupCostSlots);
                        //int dstsize = eaSize( hAccum );
                        //int srcsize = eaSize(&def->powerupCostSlots);

                        //eaSetSize( hAccum, dstsize + srcsize);
                        //CopyStructs((*hAccum)+dstsize, def->powerupCostSlots, srcsize);
                        eaPushArrayConst(hAccum, &def->powerupCostSlots );
                    }
                }
            }
        }

        // frame
        drawFrame(PIX2, R4, tx, ty, z + 1, wd, ht, sc, clr, bclr );

        // draw the icon
        display_sprite( icon, tx, ty, z + 1, sc_enh, sc_enh, CLR_WHITE );

        // draw the powerup cost
        {

            // (loop for i from 0 to 9 
            //       for fpi = (/ pi 5.0)
            //       for ang = (+ (/ fpi 2) (- (/ pi 2))  (* fpi i)) do 
            //       (insert (format "{ %f, %f },\n" (cos ang) (sin ang))))
            static struct
            {
                F32 c;
                F32 s;
            } s_lookup[MAX_POWERUP_DISPLAY_COUNT] = 
            {
                { 0.309017, -0.951057 },
                { 0.809017, -0.587785 },
                { 1.000000, 0.000000 },
                { 0.809017, 0.587785 },
                { 0.309017, 0.951057 },
                { -0.309017, 0.951057 },
                { -0.809017, 0.587785 },
                { -1.000000, 0.000000 },
                { -0.809017, -0.587785 },
                { -0.309017, -0.951057 },
            };
            F32 wd_enh = icon->width*sc_enh;
            F32 x_ctr = tx + wd_enh/2.f;
            F32 y_ctr = ty + wd_enh/2.f;
            F32 radius = (wd_enh - 16*sc)/2;

            //int iPotential = eaSize(&ppPowerupSlots);
            //int size_potential = eaSize(&ppPotentialPowerupSlots);
            //eaSetSize(&ppPowerupSlots, iPotential + size_potential);
            //CopyStructs(ppPowerupSlots+iPotential, ppPotentialPowerupSlots, size_potential );
            int iPotential = eaPushArray(&ppPowerupSlots, &ppPotentialPowerupSlots);

            // draw the cost
            for( i = 0; i < eaSize(&ppPowerupSlots) 
                && verify(i < MAX_POWERUP_DISPLAY_COUNT); ++i)
            {
                int clr = CLR_WHITE;
                RewardSlot *r = ppPowerupSlots[i];
                F32 wd_pup = radius * .25f;
                F32 ht_pup = radius * .25f;
                F32 x_pup = x_ctr + s_lookup[i].c*radius - wd_pup/2;
                F32 y_pup = y_ctr - s_lookup[i].s*radius - ht_pup/2;

                if( i >= iPotential )
                {
                    clr = CLR_GREY;
                }
                else 
                {
                    switch ( r->type )
                    {
                    case kRewardItemType_Power:
                        {
                            clr = CLR_RED;
                        }
                        break;
                    case kRewardItemType_Salvage:
                        {
                            clr = CLR_YELLOW;
                        }
                        break;
                    case kRewardItemType_Concept:
                        {
                            clr = CLR_GREEN;
                        }
                        break;
                    case kRewardItemType_Proficiency:
                        {
                            clr = CLR_BLUE;
                        }
                        break;
                    default:
                        verify(0 && "invalid type");
                    };
                    STATIC_INFUNC_ASSERT(kRewardItemType_Count == 11);
                }

                // draw the item
                drawFrame(PIX2, R4, x_pup, y_pup, z + 1, wd_pup, ht_pup, sc, clr, clr );
            }

            eaDestroy(&ppPowerupSlots);
            eaDestroy(&ppPotentialPowerupSlots);
        }
    }
}



int inventWindow()
{
    float x, y, z, wd, ht, sc;
//     F32 view_y;
    F32 edge_off;
    int i,color, bcolor;
    static ScrollBar sb = { WDW_INVENT, 0};
    static bool initted = false;
    static bool bGotDimsPrev = false;
    Character *pchar;
    UIBox bxWindow;
    


     if(!window_getDims(WDW_INVENT, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
    {
        bGotDimsPrev = false;
        return 0;
    }
    if (!playerPtr())
        return 0;

    pchar = playerPtr()->pchar;

    // state to check always
    if( !character_IsInventing(pchar) )
    {
        s_state.state = kUiInventState_SelectingLevel;
        window_setMode( WDW_INVENT, WINDOW_SHRINKING );
    }

    if( !initted )
    {
        initted = true;
        
        // state
        ZeroStruct(&s_state);
        
        // context menu        
        s_state.rmenu = contextMenu_Create(NULL);
        
        // init the growbig
        uigrowbig_Init( &s_state.growbig, 1.1, 10 );        
    }    
    
    // --------------------
    // check for an inventory state change
    
    // ...

    // ----------------------------------------
    // post-init and state update
    
    edge_off = (R10+PIX3)*sc;    
    
    // window frame    
    drawFrame(PIX3, R10, x, y, z, wd, ht, sc, color, bcolor);
    
    // if not ready yet, return
      if(window_getMode(WDW_INVENT) != WINDOW_DISPLAYING)
        return 0;

    bxWindow.x = x;
    bxWindow.y = y;
    bxWindow.width = wd;
    bxWindow.height = ht;
    clipperPushRestrict(&bxWindow);
    if( character_IsInventing(pchar) )
    {
        Invention *inv = pchar->invention;
        const BasePower *def = inv->recipe->recipe;
        U32 scaledIconWd = ICON_WD * sc;
        F32 wd_border = PIX3*2*sc;
        F32 x_items = x+PIX3*sc*2;
        F32 ht_items = scaledIconWd + PIX3*4*sc;
        F32 y_items = y+ht-ht_items - wd_border;
        F32 wd_items = wd - wd_border*2 - 64*sc;
        F32 wd_btm = wd_items;
        F32 x_graph = x + PIX3*sc;
        F32 y_graph = y + PIX3*sc;
        F32 ht_graph = y_items - y - wd_border;
        F32 wd_graph = ht_graph * 1.61803399f; // tee-hee
        F32 wd_iconarea = wd - wd_graph - PIX3*3*sc;
        F32 ht_iconarea = ht_graph;
        F32 x_iconarea = x_graph + wd_graph + PIX3*sc; 
        F32 y_iconarea = y_graph;
        bool bFinishedInventing = false;
        
        void *mouseOver = NULL;
        
        if( pchar->invention->state == kInventionState_Started )
        {
            static ComboCheckboxElement **s_ppLevels = NULL;
            static ComboBox comboLvlSelect = {0};
            F32 dx_combo = PIX3*sc;
            F32 dy_combo = PIX3*sc;
            F32 x_btn = x + dx_combo;
            F32 y_btn = y + PIX3*sc;
            F32 z_btn = z + 1;
            int txtColor = CLR_WHITE;
            
            TTDrawContext *fnt = &game_12;
            int idLevelSelectTxt = -1;
            if( !s_ppLevels )
            {
                char *txtLevelSelect = "00.00 (00.00 to invent)";
                F32 wd_combo;
                F32 ht_combo;// = ttGetFontHeight(fnt, sc, sc);
                int numToShow = 10;
                int i;
                CBox bx = {0};

                str_dims(fnt,sc,sc,false,&bx,txtLevelSelect);
                wd_combo = bx.right - bx.left + 30*sc; // some fucking voodoo number
                ht_combo = bx.bottom - bx.top;

                // stick this in at the beginning
                //comboboxSharedElement_add( &s_ppLevels, NULL, txtLevelSelect, txtLevelSelect, idLevelSelectTxt, NULL );

                for( i = 0; i < MAX_PLAYER_SECURITY_LEVEL; ++i ) 
                {
                    char lvl[256];
                    F32 hardenAdj = invention_HardenChanceAdj( pchar->iLevel, i );
                    
                    sprintf(lvl,"%d (%s%.2f %s)",i+1, hardenAdj > 0.f ? "+" : "", hardenAdj, textStd("InventionToHardenAdj") ); //+1 for zero based levels
                    comboboxSharedElement_add( &s_ppLevels, NULL, lvl, lvl, i, NULL );
                }
                
                comboboxTitle_init( &comboLvlSelect, 0, 0, 0, wd_combo, ht_combo, numToShow*ht_combo, WDW_INVENT );
                comboLvlSelect.elements = s_ppLevels;
//                combobox_setChoiceCur(&comboLvlSelect, idLevelSelectTxt);
            }

            // --------------------
            // check if this is the first display. if so
            // set the current id to the player's level

            //if( !bGotDimsPrev )
            //{
            //    combobox_setChoiceCur(&comboLvlSelect, idLevelSelectTxt);
            //}

            // --------------------
            // always update location, and display

            combobox_setLoc(&comboLvlSelect, dx_combo, dy_combo, 10);
            combobox_display(&comboLvlSelect);


            {
                UIBox dimsBtnPrev = {0};
                bool bCanUpdateInvention = character_InventingCanUpdate(pchar);
                if( D_MOUSEHIT == drawTextButton( "InventSelectInventLevel", x_btn, y_btn + dy_combo + comboLvlSelect.ht*sc, z_btn, sc, color, bcolor, txtColor, bCanUpdateInvention, &dimsBtnPrev) )
                {
                    if( character_InventingSelectLevel( pchar, comboLvlSelect.elements[comboLvlSelect.currChoice]->id) )
                    {
                        s_state.state = kUiInventState_Inventing;
                        window_setMode( WDW_CONCEPTINV, WINDOW_GROWING );
                    }
                }

                if( D_MOUSEHIT == drawTextButton( "CancelString", x_btn, dimsBtnPrev.y + dimsBtnPrev.height + PIX3*sc, z_btn, sc, color, bcolor, txtColor, bCanUpdateInvention, &dimsBtnPrev))
                {
                    // just delete the invention
                    character_InventingCancel(pchar);
                    window_setMode( WDW_INVENT, WINDOW_SHRINKING );
                }
            }
        }
        else //if( s_state.state == kUiInventState_Inventing )
        {
            // --------------------
            // handle the items
            
            set_scissor(1);
            scissor_dims( x_items, y_items, wd_items, ht_items );
            {
                CBox box;
                F32 x_btm = x_items;
                U32 NUM_COLS = (wd_items - wd_border)/scaledIconWd;
                int size = ARRAY_SIZE(pchar->invention->slots);
                int iSlotCur = 0;
                BuildCBox( &box, x_items, y_items, wd_items, ht_items);
                
                drawFlatFrame(PIX3, R10, box.left, box.top, 
                              z + 1, wd_items, ht_items, 
                              sc, color, bcolor);
                
                // --------------------
                // draw the slots
                
                iSlotCur = 0;
                for( i = 0; iSlotCur < size; ++i ) 
                {
                    F32 col_off = PIX3*sc + (COL_WIDTH+PIX3)*iSlotCur*sc;
                    InventionSlot *slot = pchar->invention->slots + i;
                    bool inSlotRange = false;
                    TrayslotDisplayInfo cur;
                    
                    cur = s_trayslot_Display( pchar, slot, i, iSlotCur, box.left+col_off, y_items + PIX3*sc, z, sc, inSlotRange );
                    
                    iSlotCur += cur.dSlotWidth;
                    if( cur.pMouseOver )
                    {
                        mouseOver = slot;
                    }
                }
            }
            set_scissor(0);
            
            // --------------------
            // draw the finalize button
            
            {
                UIBox box = { 
                    x_items + wd_items,
                    y_items,
                    wd - wd_items - 2*wd_border,
                    ht_items
                };
                
                clipperPushRestrict(&box);
                {
                    int txtColor = CLR_WHITE;
                    F32 x_btn = box.x + PIX3*sc;
                    F32 y_btn = box.y + PIX3*sc;
                    F32 z_btn = z + 1;
                    UIBox dimsBtnPrev = {0};
                    bool bCanUpdateInvention = character_InventingCanUpdate(pchar);
                    drawFlatFrame(PIX3, R10, box.x, box.y, 
                                  z + 1, box.width, box.height, 
                                  sc, color, bcolor);
                    
                    if( D_MOUSEHIT == drawTextButton("InventFinalize", x_btn, y_btn, z_btn, sc, color, bcolor, txtColor, bCanUpdateInvention, &dimsBtnPrev ) )
                    {
                        bFinishedInventing = true;
                        invent_SendFinalize();
                    }
                    
                    if( D_MOUSEHIT == drawTextButton( "CancelString", x_btn, dimsBtnPrev.y + dimsBtnPrev.height + PIX3*sc, z_btn, sc, color, bcolor, txtColor, bCanUpdateInvention, &dimsBtnPrev))
                    {
                        
                        bFinishedInventing = true;
                        //invent_SendCancel();
                        character_InventingCancel(pchar);
                    }
                }
                clipperPop();
            }
            
            if( !bFinishedInventing )
            {
                // --------------------
                // draw the graph

                s_graphdraw(inv, x_graph, y_graph, z+1, wd_graph, ht_graph, sc, color, bcolor);

                // --------------------
                // draw the enhancement icon for the invention

                s_displayEnhancement(inv, x_iconarea, y_iconarea, z + 1, wd_iconarea, ht_iconarea, sc, color, bcolor);
            }

            // grow the current icon
            uigrowbig_Update(&s_state.growbig, mouseOver );        
            
            if( bFinishedInventing )
            {
                // ... todo, what happens here.
                s_state.state = kUiInventState_SelectingLevel;
                window_setMode( WDW_INVENT, WINDOW_SHRINKING );        
            }
        }
    }
    clipperPop();

    // --------------------
    // finally

    bGotDimsPrev = true;
    
     return 0;
}

