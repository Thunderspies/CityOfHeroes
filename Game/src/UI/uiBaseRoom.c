/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/mathutil.h>

#include "entity/entity.h"

#include "gameComm/wdwbase.h"
#include "render/sprite.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"
#include "entity/Supergroup.h"
#include "bases/baseupkeep.h"

#include "player/player.h"
#include "cmdparse/cmdgame.h"
#include "graphics/camera.h"
#include "UI/uiBox.h"
#include "UI/uiUtil.h"
#include "UI/uiInput.h"
#include "UI/uiWindows.h"
#include "UI/uiUtilGame.h"
#include "UI/uiTabControl.h"
#include "uiSlider.h"
#include "UI/uiScrollBar.h"
#include "UI/uiComboBox.h"
#include "UI/uiClipper.h"
#include "uiColorPicker.h"
#include "UI/uiDialog.h"
#include "UI/uiToolTip.h"
#include "UI/uiGame.h"

#include "bases/bases.h"
#include "bases/basedata.h"
#include "bases/baseedit.h"
#include "bases/baselegal.h"
#include "bases/baseclientsend.h"
#include "UI/uiBaseRoom.h"

#include "formatter/smf_main.h"
#include "uiSMFView.h"
#include "group/group.h"
#include "seq/anim.h"
#include "render/tex.h"
#include <utilitieslib/language/MessageStoreUtil.h>

static TextAttribs gTextTitleAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_14,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

static TextAttribs gTextDeleteAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xff4444ff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xff4444ff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

#define HR(w,h) addStringToStuffBuff(&sb, "<img border=0 width=1 height=%d color=#00000000><span align=center><img border=0 src=Headerbar_HorizDivider_L.tga height=1 width=20><img border=0 src=white.tga width=%i height=1><img border=0 src=Headerbar_HorizDivider_R.tga height=1 width=20></span><img border=0 width=1 height=%d color=#00000000>",h/2,w - 40,h/2)

char * baseRoomGetDescriptionString( const RoomTemplate *room, BaseRoom * current_room, int showInfo, int showStats, int width)
{
    static StuffBuff sb;
    int i, bPlotRestricted = 0, cat_count, actual_count, base_count = 0, base_limit = 0;

     if(!sb.buff)
        initStuffBuff(&sb, 128);

      clearStuffBuff(&sb);

    // long desc
    if( showInfo )
    {
        addStringToStuffBuff( &sb, textStd( room->pchDisplayHelp ) );
        // first the cost and upkeep
          addStringToStuffBuff( &sb, "<br>" );
    }
    
    if( !current_room )    
    {
        if (width)
            HR(width,10);
        addStringToStuffBuff(&sb, "<scale 0.8><table border=3>");
        addStringToStuffBuff( &sb, textStd( "ItemCostTable", room->iCostPrestige ) );            
        if( room->iUpkeepPrestige ) 
        {
            addStringToStuffBuff( &sb, textStd( "ItemUpkeepTable", room->iUpkeepPrestige ) );
        }
        addStringToStuffBuff( &sb, textStd( "RoomDimensionsTable", room->iLength, room->iWidth ) );
        addStringToStuffBuff(&sb, "</table></scale>");
    }
    
    if( !showStats )
        return sb.buff;

    if (width && showInfo)
        HR(width,10);

    // category table
       addStringToStuffBuff( &sb, "<color white>" );
       addStringToStuffBuff( &sb, "<scale .80><table border=3><b><tr border=1><td width=70 align=left border=0>" );
       addStringToStuffBuff( &sb, textStd( "ItemCategory") );
    addStringToStuffBuff( &sb, "</td><td align=center width=65 border=0>" );
    addStringToStuffBuff( &sb, textStd( "AllowedItems") );
     addStringToStuffBuff( &sb, "</td>" );

     if( current_room )
    {
        addStringToStuffBuff( &sb, "<td align=center width=65 border=0>" );
        addStringToStuffBuff( &sb, textStd( "PlacedItems") );
        addStringToStuffBuff( &sb, "</td>" );
    }
     addStringToStuffBuff( &sb, "</b></tr>" );

      cat_count = eaSize(&room->ppDetailCatLimits);
      for( i = 0; i < cat_count; i++ )
    {
        if( current_room )
        {
            actual_count = detailCountInRoom( room->ppDetailCatLimits[i]->pchCatName, current_room);
            base_count = detailCountInBase( room->ppDetailCatLimits[i]->pchCatName, &g_base );
            base_limit = baseDetailLimit( &g_base, room->ppDetailCatLimits[i]->pchCatName );
 
            if( (room->ppDetailCatLimits[i]->iLimit && actual_count > room->ppDetailCatLimits[i]->iLimit) || (base_count > base_limit)  )
                addStringToStuffBuff( &sb, "<font color=#999999>" );
            else
                addStringToStuffBuff( &sb, "<font color=#44ff44>" );

            bPlotRestricted |= (base_count > base_limit);
        }
        else
            addStringToStuffBuff( &sb, "<font color=white>" );

        addStringToStuffBuff( &sb, "<tr border=1><td width=70 align=left border=0>" );
        addStringToStuffBuff( &sb, textStd( room->ppDetailCatLimits[i]->pCat->pchDisplayName ) );
        addStringToStuffBuff( &sb, "</td><td align=center width=65 border=0>" );
        if( room->ppDetailCatLimits[i]->iLimit )
            addStringToStuffBuff( &sb, "%i", room->ppDetailCatLimits[i]->iLimit );
        else
            addStringToStuffBuff( &sb, textStd( "UnlimitedStr" ) );
        addStringToStuffBuff( &sb, "</td>" );

         if( current_room )
        {
            addStringToStuffBuff( &sb, "<td align=center width=65 border=0>" );
            addStringToStuffBuff( &sb, "%i", actual_count );
            if( (base_count > base_limit) )
                addStringToStuffBuff( &sb, "*" );
            addStringToStuffBuff( &sb, "</td>" );
        }

        addStringToStuffBuff( &sb, "</font></tr>" );
    }

    if( bPlotRestricted )
    {
        addStringToStuffBuff( &sb, "</table></scale><br><br>" );
        addStringToStuffBuff( &sb, "<font color=#999999>" );
         addStringToStuffBuff( &sb, textStd( "PlotCountRestricting" ) );
        addStringToStuffBuff( &sb, "</font><br>" );
    }    
    else
    {
        addStringToStuffBuff( &sb, "</table></scale>" );
    }
    
    return sb.buff;
}


char * basePlotGetDescriptionString( const BasePlot *plot, int width)
{
    static StuffBuff sb;
    int i, cat_count, base_count = 0;

    if(!sb.buff)
        initStuffBuff(&sb, 128);

    clearStuffBuff(&sb);

    if(!plot)
    {
        addStringToStuffBuff( &sb, textStd( "PleaseSelectAPlot" ) );
        return sb.buff;
    }

    // long desc
    addStringToStuffBuff( &sb, textStd( plot->pchDisplayHelp ) );
    addStringToStuffBuff( &sb, "<br>" );
    if (width)
        HR(width,10);
    // first the cost and upkeep
    
    addStringToStuffBuff(&sb, "<scale 0.8><table border=3>");
    addStringToStuffBuff( &sb, textStd( "ItemCostTable", plot->iCostPrestige ) );
     if( plot->iUpkeepPrestige )
        addStringToStuffBuff( &sb, textStd( "ItemUpkeepTable", plot->iUpkeepPrestige ) );
    addStringToStuffBuff( &sb, textStd( "PlotSizeTable", plot->iLength, plot->iWidth ) );
    addStringToStuffBuff(&sb, "</table></scale>");
    if (width)
        HR(width,10);
    // category table
    addStringToStuffBuff( &sb, "<color white>" );
    addStringToStuffBuff( &sb, "<scale .80><table border=3><b><tr border=1><td width=70 align=left border=0>" );
    addStringToStuffBuff( &sb, textStd( "ItemCategory") );
    addStringToStuffBuff( &sb, "</td><td align=center width=65 border=0>" );
    addStringToStuffBuff( &sb, textStd( "AllowedItems") );
    addStringToStuffBuff( &sb, "</td>" );
    addStringToStuffBuff( &sb, "<td align=center width=65 border=0>" );
    addStringToStuffBuff( &sb, textStd( "PlacedItems") );
    addStringToStuffBuff( &sb, "</td>" );
    addStringToStuffBuff( &sb, "</b></tr>" );

    cat_count = eaSize(&plot->ppDetailCatLimits);
    for( i = 0; i < cat_count; i++ )
    {
        base_count = detailCountInBase( plot->ppDetailCatLimits[i]->pchCatName, &g_base );

        if( plot->ppDetailCatLimits[i]->iLimit && base_count > plot->ppDetailCatLimits[i]->iLimit  )
            addStringToStuffBuff( &sb, "<font color=#999999>" );
        else
            addStringToStuffBuff( &sb, "<font color=#44ff44>" );

        addStringToStuffBuff( &sb, "<tr border=1><td width=70 align=left border=0>" );
        addStringToStuffBuff( &sb, textStd( plot->ppDetailCatLimits[i]->pCat->pchDisplayName ) );
        addStringToStuffBuff( &sb, "</td><td align=center width=65 border=0>" );

        if( plot->ppDetailCatLimits[i]->iLimit )
            addStringToStuffBuff( &sb, "%i", plot->ppDetailCatLimits[i]->iLimit );
        else
            addStringToStuffBuff( &sb, textStd( "UnlimitedStr" ) );

        addStringToStuffBuff( &sb, "</td>" );
        addStringToStuffBuff( &sb, "<td align=center width=65 border=0>" );
        addStringToStuffBuff( &sb, "%i", base_count );
        addStringToStuffBuff( &sb, "</td>" );
        addStringToStuffBuff( &sb, "</font></tr>" );
    }
    addStringToStuffBuff( &sb, "</table></scale>" );

    return sb.buff;
}

static void baseRoomCycle( int forward )
{
    int i, n = eaSize(&g_base.rooms);
    BaseRoom *pRoom = NULL;
    BaseRoom *pRoomLast = NULL;
    int block[2];

    if( forward )
    {
        for(i=0; i<n; i++)
        {
            if(g_base.rooms[i]->id == g_refCurRoom.id)
            {
                i++;
                if(i<n)
                    pRoom = g_base.rooms[i];
                else
                    pRoom = g_base.rooms[0];
            }
        }
    }
    else
    {
        for(i=0; i<n; i++)
        {
            if(g_base.rooms[i]->id == g_refCurRoom.id)
                pRoom = pRoomLast;
            pRoomLast = g_base.rooms[i];
        }
        if(!pRoom)
            pRoom = g_base.rooms[eaSize(&g_base.rooms)-1];
    }

    block[0] = pRoom->size[0]/2;
    block[1] = pRoom->size[1]/2;
    baseedit_SetCurRoom(pRoom, block);
    baseedit_LookAtCurRoomBlock();
}

static int baseRoomDrawStats(float x, float y, float z, float wd, float ht, float sc, int color, int bcolor)
{
     float ty = y, allowed_item_wd = 0, column_wd = 70;
    const RoomTemplate *room;
    static const RoomTemplate * last_room;
    static SMFBlock *smf_room;
    bool bReparse = false;

    if (!smf_room)
    {
        smf_room = smfBlock_Create();
    }

      room = g_refCurRoom.p->info;  

    if( room != last_room )
    {
        bReparse = true;
        last_room = room;
    }

    ty += smf_ParseAndDisplay( smf_room, baseRoomGetDescriptionString(room, g_refCurRoom.p, 0, 1, wd-30*sc), x + R10*sc, y, z, 
        wd - R10*sc*2, 0, bReparse, bReparse, &gTextAttr_White12, NULL, 0, true) + 5*sc;
    if (baseedit_Mode() == kBaseEdit_AddPersonal)
    {
        ty += 5*sc;
    }
    else
    {
           if( D_MOUSEHIT == drawStdButton( x + wd/2, ty+5, z, MIN(180*sc,wd-2*R10*sc), 20*sc, color, "ApplyRoomStyleToBase", 1.f, !g_refCurRoom.p ))
            baseClientSendApplyStyleToBase(g_refCurRoom.p);

        ty += 35*sc;
    }

    return ty-y;
}

static int baseGetTotalPrestigeUpkeep()
{
    int i,j, upkeep = 0;
    Entity * e = playerPtr();

    if( !e || !e->supergroup || !g_base.plot )
        return 0;

    upkeep += g_base.plot->iUpkeepPrestige;

    for( i = eaSize(&g_base.rooms)-1; i>=0; i-- )
    {
        BaseRoom * room = g_base.rooms[i];
        upkeep += room->info->iUpkeepPrestige;

        for( j = eaSize(&room->details)-1; j>=0; j-- )
            upkeep += room->details[j]->info->iUpkeepPrestige;
    }

    return upkeep + baseupkeep_PctFromPrestige(e->supergroup->prestige)*e->supergroup->prestige;
}

static int baseRoomDrawInfo(float x, float y, float z, float wd, float ht, float sc, int color, int bcolor)
{
    float ty = y + 10*sc, allowed_item_wd = 0, column_wd = 70, text_sc;
    const RoomTemplate *room;
    static const RoomTemplate * last_room;
    static SMFBlock *smf_room;
    bool bReparse = false;
    int upkeep;

    if (!smf_room)
    {
        smf_room = smfBlock_Create();
    }

    room = g_refCurRoom.p->info;  

    if( room != last_room )
    {
        bReparse = true;
        last_room = room;
    }

    ty += smf_ParseAndDisplay( smf_room, baseRoomGetDescriptionString(room, g_refCurRoom.p, 1, 0, wd - 30*sc), x + R10*sc, ty, z, 
        wd - R10*sc*2, 0, bReparse, bReparse, &gTextAttr_White12, NULL, "uiBaseRoom.c::baseRoomDrawInfo", true) + 5*sc;

     font( &game_18 );
    font_color( CLR_WHITE, CLR_WHITE );
    upkeep = baseGetTotalPrestigeUpkeep();
    text_sc = MIN( sc, str_sc( &game_18, (wd-R10*2*sc)/2, "PlotSize", g_base.plot->iLength, g_base.plot->iWidth ));
    text_sc = MIN( text_sc, str_sc( &game_18, (wd-R10*2*sc)/2, "BaseUpkeep", upkeep ));

     prnt( x + R10*sc, ty, z, text_sc, text_sc, "PlotSize", g_base.plot->iLength, g_base.plot->iWidth );
    prnt( x + wd/2, ty, z, text_sc, text_sc, "BaseUpkeep", upkeep );

    ty += 30*sc;

    return ty-(y + 10*sc);
}

static int baseRoomDrawColor(float x, float y, float z, float wd, float ht, float sc, int color, int bcolor)
{
    const RoomTemplate * room = g_refCurRoom.p->info;
    static char *apchColors[] = { "LowColor", "MiddleColor", "HighColor" };
    int i;
    float ty = y;
    static ColorPicker cp[3] = {0};
    
    font( &game_12 );
     font_color(CLR_WHITE, CLR_WHITE);
    y += 10*sc;

    // copied directly from shannons code
     for(i=2; i>=0; i--)
    {
        y += 10*sc;
        prnt(x+5*sc, y, z, sc, sc, apchColors[i]);
           pickerSetfromColor( &cp[i], g_refCurRoom.p->lights[i] );
 
        y += 2*sc;
         if( drawColorPicker(&cp[i], x+R10*sc, y, z, wd-R10*2*sc, 60*sc, baseedit_Mode() == kBaseEdit_AddPersonal) )
        {
            extern void baseClientSendLight(BaseRoom *pRoom,int which,Color color);
             g_refCurRoom.p->lights[i]->r = (cp[i].rgb_color&0xff000000)>>24;
            g_refCurRoom.p->lights[i]->g = (cp[i].rgb_color&0x00ff0000)>>16;
            g_refCurRoom.p->lights[i]->b = (cp[i].rgb_color&0x0000ff00)>>8;
               baseClientSendLight(g_refCurRoom.p,i,*g_refCurRoom.p->lights[i]);
        }
         y += 65*sc;
    }
    if (baseedit_Mode() == kBaseEdit_AddPersonal)
    {
        y += 5*sc;
    }
    else
    {
           if( D_MOUSEHIT == drawStdButton( x + wd/2, y+12*sc, z, MIN(200*sc,wd-2*R10*sc), 20*sc, color, "ApplyLightingToBase", 1.f, !g_refCurRoom.p ))
            baseClientSendApplyLightingToBase(g_refCurRoom.p);
          y+= 35*sc;
    }

     return (y-ty)+10*sc;
}

static int baseRoomDrawDoor(float x, float y, float z, float wd, float ht, float sc, int color, int bcolor)
{
    float ty = y + 10*sc, allowed_item_wd = 0, column_wd = 70;
    static SMFBlock *smf_door;
    bool bReparse = false;

    if (!smf_door)
    {
        smf_door = smfBlock_Create();
    }

    ty += smf_ParseAndDisplay( smf_door, textStd("DoorLongDesc"), x + R10*sc, ty, z, 
        wd - R10*sc*2, 0, bReparse, bReparse, &gTextAttr_White12, NULL, 0, true) + 5*sc;

    return ty-y;
}


BaseRoom * delete_me;

static void dialogDeleteRoom(void*unused)
{
    if( delete_me )
        baseedit_DeleteRoom( delete_me ); 
}



//------------------------------------------------------------------------------------------------------------
// baseRoomWindow ////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------------
typedef int(*tabWindowFunc)(float x, float y, float z, float wd, float ht, float scale, int color, int bcolor);
static uiTabControl * roomTabs = 0;

void baseRoomSetStatTab()
{
    if( roomTabs )
        uiTabControlSelect(roomTabs, baseRoomDrawStats );
}

int baseRoomWindow(void)
{
    float x, y, ty, z, wd, ht, sc, text_ht, arrow_y, func_ht, view_ht, delete_ht;
    int color, bcolor;
    int bReparse = false, door = false, err_msg;
    UIBox box;
    
     const RoomTemplate * room, *entry_info = basedata_GetRoomTemplateByName( "Entrance" );
     AtlasTex * left_arrow = atlasLoadTexture("teamup_arrow_contract.tga");
    AtlasTex * right_arrow = atlasLoadTexture("teamup_arrow_expand.tga");

    static SMFView * title_view = 0;
    static SMFView * desc_view = 0;
    static SMFBlock *smf_del;

    static ScrollBar sb = { WDW_BASE_ROOM };

    static int last_error_msg = -1;
    char * del_error_msg[] = { "CantDeleteEntrance", "VitalRoomLink", "VitalDoorLink" };

    tabWindowFunc func;

    // Do everything common windows are supposed to do.
     if ( !window_getDims(WDW_BASE_ROOM, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
        return 0;

    if( !g_refCurRoom.p || !g_refCurRoom.p->info )
    {
        drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );
        return 0;
    }

    if (!smf_del)
    {
        smf_del = smfBlock_Create();
    }

    room = g_refCurRoom.p->info;

    if( g_refCurBlock.block[0] < 0 || g_refCurBlock.block[0] >= g_refCurRoom.p->size[0] ||
        g_refCurBlock.block[1] < 0 || g_refCurBlock.block[1] >= g_refCurRoom.p->size[1] )
    {
        door = true;
    }

    uiBoxDefine(&box, x+PIX3*sc, y+PIX3*sc, wd-2*PIX3*sc, ht-2*PIX3*sc );
    clipperPush(&box);
 
    //Top Pane------------------------------------------------------------------------------

    ty = y + PIX3*sc*2;

    // Room Name
    if(!title_view)
    {
        title_view = smfview_Create(0);
    }

       gTextTitleAttr.piScale = (int*)((int)(SMF_FONT_SCALE*sc*1.2));
    smfview_SetText(title_view, textStd(door?"DoorString":room->pchDisplayName) );
    smfview_SetSize(title_view, wd-2*(R10+PIX3)*sc, 1000);
     smfview_SetAttribs(title_view, &gTextTitleAttr);
    smfview_SetLocation(title_view, 0, 0, 0);
    smfview_SetBaseLocation(title_view, x+(PIX3+R10)*sc, ty, z, wd-2*(R10+PIX3)*sc, 0);
    text_ht = MAX(15, smfview_GetHeight(title_view)) + 10*sc;
    smfview_setAlignment( title_view, SMAlignment_Center );
    smfview_Draw(title_view);

    // Room-Switching Arrows
    arrow_y = y +PIX3*2*sc + text_ht/2 - left_arrow->height*sc/2;
    if( D_MOUSEHIT == drawGenericButton( left_arrow, x + PIX3*2*sc, arrow_y, z, sc, color, color, true ) )
        baseRoomCycle( 0 );
    if( D_MOUSEHIT == drawGenericButton( right_arrow, x + wd - (right_arrow->width+PIX3*2)*sc, arrow_y, z, sc, color, color, true ) )
        baseRoomCycle( 1 );

    // Short Desc
    if(!desc_view)
        desc_view = smfview_Create(0);

    gTextAttr_White12.piScale = (int*)((int)(SMF_FONT_SCALE*sc));
    smfview_SetText(desc_view, textStd(door?"DoorShortDesc":(room->pchDisplayShortHelp?room->pchDisplayShortHelp:"")));
    smfview_SetSize(desc_view, wd-2*(R10+PIX3), 1000);
    smfview_SetAttribs(desc_view, &gTextAttr_White12);
    smfview_SetLocation(desc_view, 0, 0, 0);
    smfview_SetBaseLocation(desc_view, x+(PIX3+R10)*sc, ty + text_ht, z, wd-2*(R10+PIX3)*sc, 0);
    smfview_Draw(desc_view);
    text_ht += MAX(15, smfview_GetHeight(desc_view))+15*sc;
    ty += text_ht;
  
    //Bottom Pane------------------------------------------------------------------------------

    // Tabs
    if( !roomTabs )
    {
        int room_count = eaSize(&g_RoomTemplateDict.ppRooms);
        roomTabs = uiTabControlCreate(TabType_Undraggable, 0, 0, 0, 0, 0);
        uiTabControlAdd( roomTabs, "StatsString", baseRoomDrawStats );
        uiTabControlAdd( roomTabs, "ColorString", baseRoomDrawColor );
        uiTabControlAdd( roomTabs, "InfoString", baseRoomDrawInfo );
    }

    if( !door )
    {
        func = drawTabControl(roomTabs, x+R10*sc, ty - 3*PIX3*sc, z, wd - 2*R10*sc, TAB_HEIGHT, sc, color, color, TabDirection_Horizontal );
        ty += (PIX3+TAB_HEIGHT)*sc;
    }
    else
    {
        func = baseRoomDrawDoor;
           ty += (PIX3+1)*sc;
    }

     ty -= R10*sc;
       view_ht = ht - ((ty-y)+(PIX3+1)*sc);
    
     uiBoxDefine( &box, x+PIX3*sc, ty, wd - 2*PIX3*sc, view_ht);
    if( window_getMode( WDW_BASE_ROOM ) != WINDOW_DISPLAYING )
    {
        clipperPop();
        return 0;
    }
    
       clipperPushRestrict(&box);
       func_ht = func( x, ty - sb.offset, z, wd, view_ht, sc, color, bcolor );
    clipperPop();

    // delete button or error text
    if(baseedit_Mode() == kBaseEdit_AddPersonal)
    {
        err_msg = -1;
        delete_ht = 10;
    }
       else if( door )
    {
        if( !roomDoorwayDeleteLegal(g_refCurRoom.p->id, g_refCurBlock.block ) )
        {
            err_msg = 2;
            delete_ht = 64;
            drawStdButton(x + wd/2, y+ht-52*sc, z, MIN(wd-R10*2*sc,150*sc), 20*sc, color, "DeleteDoor", 1.f, 1 );
        }
        else
        {
            err_msg=-1;
            delete_ht = 32;
            if(D_MOUSEHIT==drawStdButton(x + wd/2, y+ht-22*sc, z, MIN(wd-R10*2*sc,150*sc), 20*sc, color, "DeleteDoor", 1.f, 0 ))
                baseedit_DeleteCurDoorway();
        }

    }
    else
    {
        if(room==entry_info||eaSize(&g_base.rooms)<=1)
        {
            err_msg = 0;
            delete_ht = 81;
             drawStdButton(x + wd/2, y+ht-69*sc, z, MIN(wd-R10*2*sc,150*sc), 20*sc, color, "DeleteRoom", 1.f, 1);

             if(D_MOUSEHIT==drawStdButton(x + wd/2, y+ht-45*sc, z, MIN(wd-R10*2*sc,150*sc), 20*sc, color, "MoveRoom", 1.f, 0))
                baseedit_StartRoomMove( g_refCurRoom.p );
        }
        else if(!roomDeleteLegal(g_refCurRoom.p->id))
        {
            err_msg = 1;
            delete_ht = 89;
            drawStdButton(x + wd/2, y+ht-77*sc, z, MIN(wd-R10*2*sc,150*sc), 20*sc, color, "DeleteRoom", 1.f, 1);

             if(D_MOUSEHIT==drawStdButton(x + wd/2, y+ht-53*sc, z, MIN(wd-R10*2*sc,150*sc), 20*sc, color, "MoveRoom", 1.f, 0))
                baseedit_StartRoomMove( g_refCurRoom.p );
        }
        else
        {
            err_msg=-1;
            delete_ht = 57;
             if(D_MOUSEHIT==drawStdButton(x + wd/2, y+ht-47*sc, z, MIN(wd-R10*2*sc,150*sc), 20*sc, color, "DeleteRoom", 1.f, 0))
            {
                int i;
                BaseRoom *pRoom = g_refCurRoom.p;
                bool bWarning = false;
                bool bCantDelete = false;

                for (i = eaSize(&pRoom->details)-1; i >= 0; i--)
                {
                    RoomDetail *det = pRoom->details[i];
                    if (det->creator_name[0] || det->info->bObsolete || det->info->bWarnDelete)
                        bWarning = true;
                    if (det->info->bCannotDelete && game_state.beta_base != 2)
                        bCantDelete = true;
                }

                if (bWarning)
                {
                    dialogStd(DIALOG_OK,textStd("GenericRoomDeleteWarning"),NULL,NULL,NULL,NULL,0);
                }
                else if (bCantDelete)
                {
                    dialogStd(DIALOG_OK,textStd("CantDeleteAllDetails"),NULL,NULL,NULL,NULL,0);
                }
                else
                {                
                    delete_me = g_refCurRoom.p;
                    baseedit_CancelDrags();
                    dialogStd( DIALOG_YES_NO, textStd("dialogDeleteRoom"), NULL, NULL, dialogDeleteRoom, NULL, 0 );
                }
            }

            if(D_MOUSEHIT==drawStdButton(x + wd/2, y+ht-22*sc, z, MIN(wd-R10*2*sc,150*sc), 20*sc, color, "MoveRoom", 1.f, 0))
                baseedit_StartRoomMove( g_refCurRoom.p );
        }
    }

      if(err_msg>=0)
    {
         gTextDeleteAttr.piScale = (int*)((int)(SMF_FONT_SCALE*sc));
           smf_ParseAndDisplay( smf_del, textStd(del_error_msg[err_msg]), x + R10*sc, y+ht-(err_msg?40:28)*sc, z, wd - R10*sc*2, 0, 
            last_error_msg != err_msg, last_error_msg != err_msg, &gTextDeleteAttr, NULL, 0, true);
    }

     clipperPop();

       window_setDims( WDW_BASE_ROOM, -1, -1, -1, text_ht+func_ht+(delete_ht)*sc );
      drawJoinedFrame2Panel( PIX3, R10, x, y, z, sc, wd, text_ht, func_ht+(delete_ht)*sc, color, bcolor, NULL, NULL );        
    
    return 0;
}

/* End of File */
