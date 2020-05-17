/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/Earray.h>
#include <utilitieslib/utils/mathutil.h>

#include "entity/entity.h"

#include "gameComm/wdwbase.h"
#include <utilitieslib/utils/eval.h>

#include "render/sprite.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"
#include "graphics/ttFontUtil.h"

#include "player/player.h"
#include "cmdparse/cmdgame.h"
#include "graphics/camera.h"
#include "win/input.h"
#include "graphics/groupThumbnail.h"
#include "entity/character_base.h"
#include "entity/character_eval.h"
#include "entity/character_inventory.h"

#include "UI/uiBox.h"
#include "UI/uiUtil.h"
#include "UI/uiInfo.h"
#include "UI/uiInput.h"
#include "UI/uiClipper.h"
#include "UI/uiWindows.h"
#include "UI/uiUtilGame.h"
#include "UI/uiGame.h"
#include "UI/uiTabControl.h"
#include "uiSlider.h"
#include "UI/uiScrollBar.h"
#include "UI/uiContextMenu.h"
#include "UI/uibaseprops.h"
#include "UI/uiBaseRoom.h"
#include "UI/uiToolTip.h"
#include "bases/bases.h"
#include "bases/basedata.h"
#include "bases/baseedit.h"
#include "bases/baseinv.h"
#include "UI/uiBaseInventory.h"

#include "formatter/smf_main.h"
#include "uiSMFView.h"
#include "win/win_init.h"
#include "UI/uiBaseInput.h"
#include "language/langClientUtil.h"
#include "group/group.h"
#include "entity/Supergroup.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "bases/baselegal.h"
#include <utilitieslib/components/StringCache.h>

int gShowHiddenDetails = 0;

static uiTabControl *baseInventoryTabs;
static uiTabControl *baseRoomTabs;
static uiTabControl *basePlotTabs;
static uiTabControl *baseRoomStyleTabs;

static ContextMenu *gItemContext = 0;
static ContextMenu *gRoomContext = 0;
static ContextMenu *gPlotContext = 0;

typedef struct TabSave
{
    const RoomTemplate *room;
    char tabName[64];
}TabSave;

TabSave ** tabSaves = 0;

typedef struct SMFsave
{
    SMFBlock *smf;
    const char *str_ptr;
    int        ht;
}SMFsave;

SMFsave ** smfList = 0;

static int baseStatusMsgType = 0;
static char * baseStatusMsg = NULL;

void base_setStatusMsg( char * str, int type )
{
    if( !baseStatusMsg || type >= baseStatusMsgType  )
    {
        SAFE_FREE(baseStatusMsg);
        baseStatusMsg = strdup(str);
        baseStatusMsgType = type;
    }    
}

#define ITEM_FOOTER    30
#define ITEM_SHORT_FOOTER 12
#define ITEM_WD        120

static SMFsave * findOrAddSMF( const char * str_ptr, const char * text, float sc )
{
    int i, count;
    SMFsave * new_smf;

    if( !smfList )
        eaCreate(&smfList);

    // first look for exact string in list
    count = eaSize(&smfList);
    for( i = 0; i < count; i++ )
    {
        if( str_ptr == smfList[i]->str_ptr )
            return smfList[i];
    }

    // no dice, add it to list
    new_smf = calloc(1, sizeof(SMFsave));
    new_smf->str_ptr = str_ptr;
    new_smf->smf = smfBlock_Create();
    smf_ParseAndFormat(new_smf->smf, textStd(text), 0, 0, 0,(ITEM_WD-2*PIX3)*sc, 1000, false, true, false, &gTextAttr_White12, 0 );
    new_smf->smf->pBlock->pos.alignHoriz = SMAlignment_Center;
    new_smf->smf->dont_reparse_ever_again = true;
    new_smf->ht = smf_ParseAndFormat(new_smf->smf, textStd(text), 0, 0, 0, (ITEM_WD-2*PIX3)*sc, 1000, false, false, true, &gTextAttr_White12, 0 );
    eaPush( &smfList, new_smf );

    return new_smf;
}

static void * selected_item = 0;
static int selected_index = 0;
static RoomTemplate * selected_room = 0;

void *getBaseInventorySelectedItem()
{
    return selected_item; 
}
void setBaseInventorySelectedItem(void *item) {
    selected_item = item;
}
typedef enum DBIResult
{
    DBI_NOTHING,
    DBI_SELECT,
    DBI_SELECT_STICKY,
    DBI_SELL,
    DBI_PICK_UP,
    DBI_BUY,
} DBIResult;

typedef enum ItemTabType
{
    TABTYPE_DICT,
    TABTYPE_INVENTORY,
    TABTYPE_CUR_ROOM,
    TABTYPE_ROOM,
    TABTYPE_STYLE,
    TABTYPE_PERSONAL,
    TABTYPE_PLOT,
}ItemTabType;

#define BUTTON_HEIGHT 15

typedef struct DecorStyle
{
    char * name;
    char * displayName;
    char * def_name;
}DecorStyle;

static int playerFunds(void)
{
    return g_base.supergroup_id ? playerPtr()->supergroup->prestige : playerPtr()->pchar->iInfluencePoints;
}

static void drawBoxGrid( int width, int height, float x, float y, float z, float wd, float ht )
{
    int i, j;
    float seg_wd = MAX( 1.1f, MIN( ht, wd )/(MINMAX(MAX(width, height), 8, MAX(width,height))));

    for(i = 0; i < width; i++ )
    {
        for( j = 0; j < height; j++ )
        {
            CBox box;
            BuildCBox(&box, x+(wd - seg_wd*width)/2 + i*seg_wd, y + (ht - seg_wd*height)/2 + j*seg_wd, seg_wd-1, seg_wd-1 );
            drawBox( &box, z, 0, 0xffffff88 );
        }
    }
}

void drawRoomBoxes( const RoomTemplate * room, float x, float y, float z, float wd, float ht )
{
    drawBoxGrid( room->iWidth, room->iLength, x, y, z, wd, ht );
}

void drawPlotBox( const BasePlot * plot, float x, float y, float z, float wd, float ht )
{
    int i;
    static float max_wd, max_ht;
    float twd, tht;
    CBox box;

    if( !max_wd )
    {
        for( i = eaSize(&g_BasePlotDict.ppPlots)-1; i >= 0; i-- )
        {
            if( g_BasePlotDict.ppPlots[i]->iWidth > max_wd )
                max_wd = g_BasePlotDict.ppPlots[i]->iWidth;
            if( g_BasePlotDict.ppPlots[i]->iLength > max_ht )
                max_ht = g_BasePlotDict.ppPlots[i]->iLength;
        }
    }
    
    twd = ((float)plot->iWidth/max_wd)*wd;
    tht = ((float)plot->iLength/max_ht)*ht;
    BuildCBox( &box, x + (wd-twd)/2, y + (ht-tht)/2, twd, tht );
    drawBox( &box, z, 0, 0xffffff88 );
}

#define MAX_NEW_PER_FRAME 2
int num_loaded_images = 0;

static DBIResult displayBaseItem( void * item, float x, float y, float ht, float z, float sc, int color, ItemTabType flag, int hideImage, int index )
{
    AtlasTex *item_pic;
    SMFsave * smf;
    CBox box;
    DBIResult ret = DBI_NOTHING;

    float remaining_ht, pic_sc;
    bool bDim = false, bUnsellable = false;
    int iCost = 0, item_footer, cantAfford;
    const char *displayName, *displayStr, *costSecondary = 0;
    char costStr[128];
    
    z +=5;

    if( flag == TABTYPE_ROOM )
    {
        const RoomTemplate* room = cpp_reinterpret_cast(const RoomTemplate*)(item);
        item_footer = 4*sc;
        displayStr = displayName = room->pchDisplayName;
        msPrintf( menuMessages, SAFESTR(costStr), "ItemCost", room->iCostPrestige);
        iCost = room->iCostPrestige;
        smf = findOrAddSMF( displayStr, displayName, sc );

        // draw room boxes
        drawRoomBoxes( room, x, y, z, ITEM_WD*sc, ht-(2*R10)*sc - smf->ht );
    }
    else if (flag == TABTYPE_STYLE )
    {
        int ceiling = false;
        const DecorStyle* decor = cpp_reinterpret_cast(const DecorStyle*)(item);
        item_footer = 4*sc;
        displayStr = decor->name;
        displayName = decor->displayName; 
        *costStr = 0;
        if( strstri(decor->def_name, "ceil") )
            ceiling = true;

        if( stricmp(textStd(displayStr), displayStr) == 0 ) 
            smf = findOrAddSMF( displayStr, displayName, sc );
        else
            smf = findOrAddSMF( displayStr, displayStr, sc );
        
        // Picture
        if( num_loaded_images < MAX_NEW_PER_FRAME)
        {
            int load = 0;
            item_pic = groupThumbnailGet(decor->def_name, 0/*decor==selected_item*/, ceiling, &load);
            if( load )
                num_loaded_images++; 
            remaining_ht = ht - smf->ht - item_footer;
            pic_sc = MIN( ITEM_WD*sc/item_pic->width, remaining_ht/item_pic->height); // scale icon to fit in space left
            display_sprite( item_pic, x + (ITEM_WD*sc-item_pic->width*pic_sc)/2, y + (remaining_ht)/2 - item_pic->height*pic_sc/2, z+10, pic_sc, pic_sc, CLR_WHITE );
        }
    }    
    else if( flag == TABTYPE_PLOT )
    {
        const BasePlot* plot = cpp_reinterpret_cast(const BasePlot*)(item);
        item_footer = 4*sc;
        displayStr = displayName = plot->pchDisplayName;
        msPrintf( menuMessages, SAFESTR(costStr), "ItemCost", plot->iCostPrestige);
         iCost = plot->iCostPrestige;
        smf = findOrAddSMF( displayStr, displayName, sc );

        // draw room boxes
        drawPlotBox( plot, x+R10*sc, y+R10*sc, z, (ITEM_WD-2*R10)*sc, ht-(2*R10)*sc - smf->ht  );
    }
    else
    {
        const Detail* detail = cpp_reinterpret_cast(const Detail*)(item);
        item_footer = (flag?23:4)*sc;
        if( (detail->bCannotDelete && game_state.beta_base != 2) || stricmp("Entrance", detail->pchCategory)==0  )
            bUnsellable = true;

        displayStr = displayName = detail->pchDisplayName?detail->pchDisplayName:detail->pchName;
        msPrintf( menuMessages, SAFESTR(costStr), "ItemCost", detail->iCostPrestige);
        smf = findOrAddSMF( displayStr, displayName, sc );
        iCost = detail->iCostPrestige;

        if(num_loaded_images < MAX_NEW_PER_FRAME)
        {
            int load = 0;
            item_pic = groupThumbnailGet(detailGetThumbnailName(detail), selected_item == item && selected_index == index, detail->eSurface==kSurface_Ceiling, &load);
            if( load )
                num_loaded_images++; 
            remaining_ht = ht - smf->ht - item_footer - R10*sc;
            pic_sc = MIN( ITEM_WD*sc/item_pic->width, remaining_ht/item_pic->height); // scale icon to fit in space left
            display_sprite( item_pic, x + (ITEM_WD*sc-item_pic->width*pic_sc)/2, y + (remaining_ht)/2 - item_pic->height*pic_sc/2, z+10, pic_sc, pic_sc, CLR_WHITE );
        }
    }

    if( flag == TABTYPE_PLOT )
        cantAfford = !playerPtr()->supergroup || (iCost-g_base.plot->iCostPrestige > playerFunds());
    else
        cantAfford = !playerPtr()->supergroup || (iCost > playerFunds());

    if (playerPtr()->access_level >= BASE_FREE_ACCESS)
        cantAfford = false;

    if( cantAfford )
        drawFlatFrame( PIX3, R10,x+1, y, z+5, (ITEM_WD-2)*sc, ht, sc, 0x00000088, 0x00000088 ); // grey overlay

    z += 10;

    // Name
    gTextAttr_White12.piScale = (int*)((int)(SMF_FONT_SCALE*sc));
    smf_ParseAndDisplay( smf->smf, textStd(displayName), x, y + ht - item_footer - smf->ht - (*costStr?15*(costSecondary?2:1):0)*sc, z, 
                            (ITEM_WD-2*PIX3)*sc, 1000, false, false, &gTextAttr_White12, NULL, 0, true);

    // cost string
    font( &game_9 );
    if( cantAfford )
        font_color( CLR_RED, CLR_RED );
    else
        font_color( CLR_GREEN, CLR_GREEN );

    if( costSecondary )
        cprnt( x + ITEM_WD*sc/2, y + ht - item_footer - 10*sc, z, sc, sc, costSecondary);
    if( *costStr )
        cprnt( x + ITEM_WD*sc/2, y + ht - item_footer, z, sc, sc, costStr );

    // Action Buttons

    if( flag == TABTYPE_INVENTORY || flag == TABTYPE_CUR_ROOM )
    {
        if( flag == TABTYPE_INVENTORY ) // hack
        {
            if( D_MOUSEHIT == drawStdButton( x + (ITEM_WD*3/4)*sc, y + ht - (item_footer-12-BUTTON_HEIGHT/2)*sc, z, (ITEM_WD-20)/2*sc, 15*sc, color, "PlaceString", 0.75f, bDim ) )
            {
                ret = DBI_PICK_UP;
                // BASE_TODO: remove item from base and place in inventory
            }
        }
        else if( flag == TABTYPE_CUR_ROOM )
        {
            if( D_MOUSEHIT == drawStdButton( x + (ITEM_WD/2)*sc, y + ht - (PIX3*2 + BUTTON_HEIGHT/2)*sc, z, (ITEM_WD-20)*sc, 15*sc, color, "SellString", 0.75f, bUnsellable ) )
            {
                ret = DBI_SELL;
                // BASE_TODO: remove item from base and place in inventory
            }
        }
    }

    z -= 5;
    BuildCBox( &box, x, y, ITEM_WD*sc, ht );
    if( selected_item == item && selected_index == index)
    {
        if( iCost > playerFunds() && playerPtr()->access_level < BASE_FREE_ACCESS)
        {
            selected_item = NULL;
            selected_index = 0;
        }

        drawFlatFrame( PIX3, R10, x, y, z, ITEM_WD*sc, ht, sc, CLR_SELECTION_FOREGROUND, CLR_SELECTION_BACKGROUND );
    }

    if( mouseCollision( &box ) ) 
    {
        if( !cantAfford )
        {
            drawFlatFrame( PIX3, R10,x, y, z, ITEM_WD*sc, ht, sc, CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND );
            if( mouseDown( MS_LEFT ) )
            {
                selected_item = item;
                selected_index = index;
                if(!ret)
                    ret = DBI_SELECT;
            }
        }
        if( mouseRightClick( &box, MS_RIGHT ) )
        {    
            int x, y;
            inpMousePos( &x, &y );
            if( flag == TABTYPE_ROOM )
                contextMenu_set(gRoomContext, x, y, item);
            else if( flag == TABTYPE_PLOT )
                contextMenu_set(gPlotContext, x, y, item);
            else if( flag != TABTYPE_STYLE )
                contextMenu_set(gItemContext, x, y, item);
        }
    }

    return ret;
}


static const char* basecm_itemTitle( void * data )
{
    const Detail *item = (const Detail*)data; // total hack, different types of pointers come in here

    return item->pchDisplayName;
}

static const char* basecm_itemInfo( void * data )
{
    const Detail *item = (const Detail*)data;

    return detailGetDescription(item,0);
}

static const char* basecm_roomInfo( void * data )
{
    const RoomTemplate *room = (const RoomTemplate*)data;

    return room->pchDisplayShortHelp;
}

static const char* basecm_plotInfo( void * data )
{
    const BasePlot *plot = (const BasePlot*)data;

    return plot->pchDisplayShortHelp;
}

static void baseitem_Info( void * data )
{
    const Detail *item = (const Detail*)data;

    info_Item( item, groupThumbnailGet(detailGetThumbnailName(item), false, item->eSurface==kSurface_Ceiling,0) );
}

static void baseitem_RoomInfo( void * data )
{
    const RoomTemplate *room = (const RoomTemplate*)data;
    info_Room( room );
}

static void baseitem_PlotInfo( void * data )
{
    const BasePlot *plot = (const BasePlot*)data;
    info_Plot( plot );
}

enum
{
    kBaseInventory_AddRoom,
    kBaseInventory_Style,
    kBaseInventory_Item,
    kBaseInventory_CurRoom,
    kBaseInventory_Personal,
    kBaseInventory_Plot,
};


static int s_baseInventoryType = kBaseInventory_Item;

bool baseInventoryIsStyle(void)
{
    return (s_baseInventoryType == kBaseInventory_Style);
}

#define BUTTON_AREA_WD    150

static void baseDrawInstructionButtons( float x, float y, float z, float ht, float sc, int color, int bcolor )
{
    float wd = BUTTON_AREA_WD*sc;
    int mode = baseedit_Mode();
    Entity * e = playerPtr();
 
    if( !mode )
    {
        if( playerCanModifyBase(e, &g_base) )
        {
            // enter base edit mode button
            if( D_MOUSEHIT == drawStdButton( x + wd/2, y + ht/4, z, (BUTTON_AREA_WD-2*R10)*sc, 30*sc, color, "EditBase", sc, 0 ) )
            {
                s_baseInventoryType = kBaseInventory_AddRoom;
                baseEditToggle( kBaseEdit_Architect );
            }
        }

        // enter place personal item
        if( D_MOUSEHIT == drawStdButton( x + wd/2, y + ht/2, z, (BUTTON_AREA_WD-2*R10)*sc, 30*sc, color, "AddPersonalItem", sc, !playerCanPlacePersonal(playerPtr(), &g_base) || !character_CountInventory(e->pchar,kInventoryType_BaseDetail) ) )
        {
            s_baseInventoryType = kBaseInventory_Personal;
            baseEditToggle( kBaseEdit_AddPersonal );
        }

        // place plots
        if( playerCanModifyBase(e, &g_base) )
        {
            if( D_MOUSEHIT == drawStdButton( x + wd/2, y + ht*3/4, z, (BUTTON_AREA_WD-2*R10)*sc, 30*sc, color, "UpgradePlot", sc, 0 ) )
            {
                s_baseInventoryType = kBaseInventory_Plot;
                baseEditToggle( kBaseEdit_Plot );
            }
        }
    }
    else if( mode == kBaseEdit_Architect )
    {
        float button_ht = (ht - 8*PIX3*sc)/5;
        
        // create room
        if( D_MOUSEHIT == drawStdButton( x + wd/2, y + 2*PIX3*sc + button_ht/2, z, (BUTTON_AREA_WD-2*R10)*sc, button_ht, color, "CreateRoom", sc, 0 ) )
        {
            s_baseInventoryType = kBaseInventory_AddRoom;
            baseedit_SetCamDistAngle(400.f, RAD(80));
        }

        // pick style
        if( D_MOUSEHIT == drawStdButton( x + wd/2, y + 3*PIX3*sc + 3*button_ht/2, z, (BUTTON_AREA_WD-2*R10)*sc, button_ht, color, "PickStyle", sc, 0 ) )
        {
            s_baseInventoryType = kBaseInventory_Style;
            baseedit_SetCamDistAngle(20.f, RAD(5));
        }

        // place item
        if( D_MOUSEHIT == drawStdButton( x + wd/2, y + 4*PIX3*sc + 5*button_ht/2, z, (BUTTON_AREA_WD-2*R10)*sc, button_ht, color, "PlaceItem", sc, 0 ) )
        {
            s_baseInventoryType = kBaseInventory_Item;
            baseedit_SetCamDistAngle(80.f, RAD(45));
        }

        // Current Room
        if( D_MOUSEHIT == drawStdButton( x + wd/2, y + 5*PIX3*sc + 7*button_ht/2, z, (BUTTON_AREA_WD-2*R10)*sc, button_ht, color, "CurrentRoom", sc, 0 ) )
            s_baseInventoryType = kBaseInventory_CurRoom;
    
        // exit base
        if( D_MOUSEHIT == drawStdButton( x + wd/2, y + 6*PIX3*sc + 9*button_ht/2, z, (BUTTON_AREA_WD-8*R10)*sc, .75*button_ht, color, "ExitBase", sc, 0 ) )
        {
            baseedit_CancelDrags();
            baseedit_CancelSelects();
            baseedit_SetCamDistAngle(10.f, RAD(5));
            baseEditToggle( kBaseEdit_None );
        }
    }
    else
        // enter place personal item
        if( D_MOUSEHIT == drawStdButton( x + wd/2, y + ht/2, z, (BUTTON_AREA_WD-4*R10)*sc, 30*sc, color, "ExitBase", sc, 0 ) )
        {
            baseedit_CancelDrags();
            baseedit_CancelSelects();
            baseedit_SetCamDistAngle(10.f, RAD(5));
            baseEditToggle( kBaseEdit_None );
        }
}

// display the items in the current room
static int displayCurrentRoom( float x, float y, float z, float wd, float ht, float sc, int color, float sb_off )
{
    UIBox box;
    int i, item_count;
    
    uiBoxDefine( &box, x+PIX3*sc, y+(PIX3)*sc, wd - 2*PIX3*sc, ht-(2*PIX3)*sc );
    clipperPush(&box);

    item_count = eaSize(&g_refCurRoom.p->details);
    selected_item = 0;
    selected_index = 0;

    for( i = 0; i < item_count; i++ )
    {
        float tx = x + R10*sc + ITEM_WD*i*sc - sb_off;

        if( tx > x+wd || tx + ITEM_WD < x )
            continue;

        if( g_refCurRoom.p->details[i] == g_refCurDetail.p )
            selected_item = (void*)g_refCurRoom.p->details[i]->info;
        else
            selected_item = 0;

        switch(displayBaseItem( cpp_reinterpret_cast(void*)(g_refCurRoom.p->details[i]->info), tx, y + (PIX3)*sc, ht - (PIX3*3)*sc, z, sc, color, TABTYPE_CUR_ROOM, 0,0))
        {
        case DBI_SELECT:
            baseedit_SetCurDetail(g_refCurRoom.p, g_refCurRoom.p->details[i] );
            break;

        case DBI_SELL:
            if( g_refCurRoom.p->details[i] == g_refCurDetail.p  )
                baseedit_SetCurDetail(NULL,NULL);

            baseedit_DeleteDetail(g_refCurRoom.p, g_refCurRoom.p->details[i]);
            break;

        case DBI_PICK_UP:
            if( g_refCurRoom.p->details[i] == g_refCurDetail.p  )
                baseedit_SetCurDetail(NULL,NULL);
            baseinv_Add(g_refCurRoom.p->details[i]->info);
            baseedit_DeleteDetail(g_refCurRoom.p, g_refCurRoom.p->details[i]);
            break;
        }
    }
    clipperPop();

    base_setStatusMsg( textStd("CurrentRoomItems"), kBaseStatusMsg_Instruction );

    return item_count;
}

// display the items in the SG inventory
static int displayInventory( float x, float y, float z, float wd, float ht, float sc, int color, float sb_off )
{
    UIBox box;
    int i, item_count;
    int iDel = -1;

    uiBoxDefine( &box, x+PIX3*sc, y+(PIX3)*sc, wd - 2*PIX3*sc, ht-(2*PIX3)*sc );
    clipperPush(&box);
    item_count = eaSize(&g_ppDetailInv);

    for( i = 0; i < item_count; i++ )
    {
        switch(displayBaseItem( (void*)g_ppDetailInv[i]->info, x + R10*sc + ITEM_WD*i*sc - sb_off, y + (PIX3)*sc, ht - (PIX3*3)*sc, z, sc, color, TABTYPE_INVENTORY,0,0))
        {
        case DBI_SELL:
            iDel = i;
            break;

        case DBI_PICK_UP:
            baseedit_StartDetailDrag(g_refCurRoom.p, g_ppDetailInv[i], kDragType_Sticky);
            break;
        }
    }
    if(iDel>=0)
    {
        baseinv_RemoveIdx(iDel);
    }
    clipperPop();
    return item_count;
}

static int displayAddRoom( float x, float y, float z, float wd, float ht, float sc, int color, float sb_off )
{
    UIBox box;
    int i, count, item_count = 0;
    int room_count = eaSize(&g_base.rooms);

    RoomCategory * curTab = drawTabControl(baseRoomTabs, x+R10*sc, y, z, wd - 2*R10*sc, TAB_HEIGHT, sc, color, color, TabDirection_Horizontal );

    uiBoxDefine( &box, x+PIX3*sc, y+(PIX3+TAB_HEIGHT)*sc, wd - 2*PIX3*sc, ht-(2*PIX3+TAB_HEIGHT)*sc );
    clipperPush(&box);

    count = eaSize(&g_RoomTemplateDict.ppRooms);

    for( i = 0; i < count; i++ )
    {
        const RoomTemplate *room = g_RoomTemplateDict.ppRooms[i];

        float tx = x + R10*sc + ITEM_WD*item_count*sc - sb_off;

        if( room->pRoomCat == curTab && baseRoomMeetsRequires(room,playerPtr()) )
        {
            if( tx > x+wd || tx + ITEM_WD < x )
            {
                item_count++;
                continue;
            }

            switch(displayBaseItem((void*)room, tx, y + (PIX3+TAB_HEIGHT)*sc, ht - (PIX3*3+TAB_HEIGHT)*sc, z, sc, color, TABTYPE_ROOM,0,0))
            {
            case DBI_SELECT:
            case DBI_BUY:
                if( stricmp( room->pchName, "Doorway" ) == 0 )
                    baseedit_StartDoorDrag();
                else
                    baseedit_StartRoomDrag(room->iWidth, room->iLength, room);
                break;
            }
            item_count++;
        }
    }
    clipperPop();

    if( baseEdit_state.room_dragging )
        base_setStatusMsg( textStd("PlaceRoomString"), kBaseStatusMsg_Instruction );
    else
        base_setStatusMsg( textStd("PickRoomString"), kBaseStatusMsg_Instruction );

    return item_count;
}

static RoomDetail ImSuchAHack = {0};

static int displayItem( float x, float y, float z, float wd, float ht, float sc, int color, float sb_off )
{
    UIBox box;
    int i, item_count, display_count;
    int total_item_count = 0;
    int room_count = eaSize(&g_base.rooms);
    Detail** curTab_details = drawTabControl(baseInventoryTabs, x+R10*sc, y, z, wd - 2*R10*sc, TAB_HEIGHT, sc, color, color, TabDirection_Horizontal );
    char * tabName = uiTabControlGetTabName(baseInventoryTabs,uiTabControlGetSelectedIdx(baseInventoryTabs));
    static int max_allowed;
    if( !curTab_details )
        return 0;

    max_allowed += MAX_NEW_PER_FRAME;

    uiBoxDefine( &box, x+PIX3*sc, y+(PIX3+TAB_HEIGHT)*sc, wd - 2*PIX3*sc, ht-(2*PIX3+TAB_HEIGHT)*sc );
    clipperPush(&box);

    for( i = 0; i < room_count; i++ )
        total_item_count += eaSize(&g_base.rooms[i]->details);

    item_count = eaSize(&curTab_details);
    display_count = 0;
    for( i = 0; i < item_count; i++ )
    {
        float tx = x + R10*sc + ITEM_WD*display_count*sc - sb_off;

        if (!gShowHiddenDetails && (curTab_details[i]->bObsolete || !baseDetailMeetsRequires(curTab_details[i], playerPtr())))
            continue;

        if( tx > x+wd || tx + ITEM_WD < x )
        {
            display_count++;
            continue;
        }

        switch(displayBaseItem( (void*)curTab_details[i], tx, y + (PIX3+TAB_HEIGHT)*sc, ht - (PIX3*3+TAB_HEIGHT)*sc, z, sc, color, TABTYPE_DICT, i >= max_allowed,0))
        {
        case DBI_SELECT:
        case DBI_BUY:
        case DBI_SELECT_STICKY:
            {
                ImSuchAHack.info = curTab_details[i];
                ImSuchAHack.invID = 0;
                // TODO: Actually need to send the buy to the server, etc.
                baseedit_SetCurDetail(g_refCurRoom.p, &ImSuchAHack);
                baseedit_StartDetailDrag(g_refCurRoom.p, g_refCurDetail.p, kDragType_Sticky);
            }
            break;
        }
        display_count++;
    }
    clipperPop();

    if( baseEdit_state.detail_dragging )
        base_setStatusMsg( textStd("PlaceItemString"), kBaseStatusMsg_Instruction );
    else
    {
        base_setStatusMsg( textStd("PickItemString"), kBaseStatusMsg_Instruction );
        selected_item = NULL;
        selected_index = 0;
    }

    return display_count;
}

int g_iDecorIdx = 0;
void basepropsSetDecorIdx(int idx)
{
    if( s_baseInventoryType != kBaseInventory_Style || !baseRoomStyleTabs)
        return;

    if(idx>=0 && idx<ROOMDECOR_MAXTYPES)
        g_iDecorIdx = idx;

    uiTabControlSelectByIdx( baseRoomStyleTabs, g_iDecorIdx+1 );

}

static void baseSetCurDecorToSelectedTab( void * tabdata )
{
    char * decor_name = (char*)tabdata;
    g_iDecorIdx = baseFindDecorIdx( decor_name );

    if( g_iDecorIdx >= 0 )
        baseedit_SetCurDecor(g_iDecorIdx);
}

static int displayStyle( float x, float y, float z, float wd, float ht, float sc, int color, float sb_off )
{
    char *decor_name = drawTabControl(baseRoomStyleTabs, x+R10*sc, y, z, wd - 2*R10*sc, TAB_HEIGHT, sc, color, color, TabDirection_Horizontal );
    int i, display_count = 0,item_count = 0;
    DecorStyle tempstyle;
    UIBox box;

    uiBoxDefine( &box, x+PIX3*sc, y+(PIX3+TAB_HEIGHT)*sc, wd - 2*PIX3*sc, ht-(2*PIX3+TAB_HEIGHT)*sc );
    clipperPush( &box );

    g_iDecorIdx = baseFindDecorIdx( decor_name );

    if( g_iDecorIdx < 0 )
        item_count = eaSize(&baseReferenceLists[ROOMDECOR_ALL].names);
    else
        item_count = eaSize(&baseReferenceLists[g_iDecorIdx].names);
    
    for( i = 0; i < item_count; i++ )
    {
        int j;
        char tempstr[128];

        // Total hack to keep doorway out of list
        if( g_iDecorIdx >= 0 &&  strstri(baseReferenceLists[g_iDecorIdx].names[i], "door" ) )
            continue;

        selected_item = NULL;
        selected_index = 0;
        display_count++;

        // hack up a displayable name for now
        if( g_iDecorIdx < 0 )
        {
            tempstyle.displayName = tempstyle.name = baseReferenceLists[ROOMDECOR_ALL].names[i];

            for( j = 0; j < eaSize(&baseReferenceLists[ROOMDECOR_WALL_16].names); j++ )
            {
                if( strstri( baseReferenceLists[ROOMDECOR_WALL_16].names[j], tempstyle.name ) )
                {
                    tempstyle.def_name = baseReferenceLists[ROOMDECOR_WALL_16].names[j];
                    break;
                }
            }

            if( g_refCurRoom.p )
            {
                int all_match = true;
                for( j = 0; j < ROOMDECOR_MAXTYPES; j++ )
                {
                    if( g_refCurRoom.p->decor_defs[j] && g_refCurRoom.p->decor_defs[j]->count > 0 )
                    {
                        if(!strstriConst( g_refCurRoom.p->decor_defs[j]->entries->def->name, tempstyle.name ) )
                            all_match = false;
                    }
                }

                if( all_match )
                    selected_item = &tempstyle;
            }
        }
        else
        {
            char * endPtr;    
            tempstyle.name = baseReferenceLists[g_iDecorIdx].names[i];
            strncpyt( tempstr, baseReferenceLists[g_iDecorIdx].names[i]+6, 128); // plus 6 is skipping the "_base_"
            endPtr = strstri( tempstr, decor_name ) - 1; // minus one is _
            *endPtr = '\0';
            tempstyle.displayName = tempstr;

            tempstyle.def_name = baseReferenceLists[g_iDecorIdx].names[i];
     
            if( g_refCurRoom.p && g_iDecorIdx >= 0 && g_refCurRoom.p->decor_defs[g_iDecorIdx] && 
                g_refCurRoom.p->decor_defs[g_iDecorIdx]->count > 0 &&
                strstriConst( g_refCurRoom.p->decor_defs[g_iDecorIdx]->entries->def->name, baseReferenceLists[g_iDecorIdx].names[i] ) )
            {
                selected_item = &tempstyle;
            }
        }

        switch(displayBaseItem( &tempstyle, x + R10*sc +ITEM_WD*i*sc - sb_off, y + (PIX3+TAB_HEIGHT)*sc,
                                    ht - (PIX3*3+TAB_HEIGHT)*sc, z, sc, color, TABTYPE_STYLE,0,0))
        {
            case DBI_SELECT:
            case DBI_BUY:

                if( g_iDecorIdx < 0 )
                    baseedit_SetCurRoomGeometry(g_iDecorIdx, tempstyle.name);
                else
                    baseedit_SetCurRoomGeometry(g_iDecorIdx, tempstr);
                break;

            default:
                break;
        }

    }
    clipperPop();
 
    base_setStatusMsg( textStd("PickStyleHere"), kBaseStatusMsg_Instruction );

    return item_count;
}

static int displayPersonal( float x, float y, float z, float wd, float ht, float sc, int color, float sb_off )
{
    UIBox box;
    int i,j, count = 0;
    Entity * e = playerPtr();

    uiBoxDefine( &box, x+PIX3*sc, y+(PIX3)*sc, wd - 2*PIX3*sc, ht-(2*PIX3)*sc );
    clipperPush(&box);
    
    for( i = eaSize(&e->pchar->detailInv) - 1; i >= 0; i-- )
    {
        DetailInventoryItem *item = e->pchar->detailInv[i];
        if( !item || !item->item )
            continue;

        for (j = 0; j < (int)item->amount ; j++)
        {        
            switch(displayBaseItem( cpp_const_cast(Detail*)(e->pchar->detailInv[i]->item), x + R10*sc + ITEM_WD*count*sc - sb_off, 
                                    y + (PIX3)*sc, ht - (PIX3*3)*sc, z, sc, color, TABTYPE_DICT,0,j))
            {
                case DBI_SELECT:
                case DBI_BUY:
                case DBI_SELECT_STICKY:
                        ImSuchAHack.info = e->pchar->detailInv[i]->item;
                        ImSuchAHack.invID = e->pchar->detailInv[i]->item->id+1; //add 1 to make sure its detected
                        // TODO: Actually need to send the buy to the server, etc.
                        baseedit_SetCurDetail(g_refCurRoom.p, &ImSuchAHack);                        
                        baseedit_StartDetailDrag(g_refCurRoom.p, g_refCurDetail.p, kDragType_Sticky);
            }
            count++;
        }
    }

    clipperPop();
    if( baseEdit_state.detail_dragging )
        base_setStatusMsg( textStd("PlacePersonalItem"), kBaseStatusMsg_Instruction );
    else
    {
        base_setStatusMsg( textStd("PickPersonalItem"), kBaseStatusMsg_Instruction );
        selected_item = NULL;
        selected_index = 0;
    }    

    return count;
}

static int displayPlot( float x, float y, float z, float wd, float ht, float sc, int color, float sb_off )
{
    UIBox box;
    int i, item_count, actual_count = 0;

    char* curTab = drawTabControl(basePlotTabs, x+R10*sc, y, z, wd - 2*R10*sc, TAB_HEIGHT, sc, color, color, TabDirection_Horizontal );

    uiBoxDefine( &box, x+PIX3*sc, y+(PIX3+TAB_HEIGHT)*sc, wd - 2*PIX3*sc, ht-(2*PIX3+TAB_HEIGHT)*sc );
    clipperPush(&box);

    item_count = eaSize(&g_BasePlotDict.ppPlots);

    for( i = 0; i < item_count; i++ )
    {
        const BasePlot *plot = g_BasePlotDict.ppPlots[i];

        if( !curTab || stricmp(curTab, g_BasePlotDict.ppPlots[i]->pchDisplayTabName)!=0 ||
            !basePlotMeetsRequires(g_BasePlotDict.ppPlots[i],playerPtr()) )
            continue;

        switch(displayBaseItem((void*)plot, x + R10*sc + ITEM_WD*actual_count*sc - sb_off, y + (PIX3+TAB_HEIGHT)*sc, 
            ht - (PIX3*3+TAB_HEIGHT)*sc, z, sc, color, TABTYPE_PLOT,0,0))
        {
        case DBI_SELECT:
        case DBI_BUY:
                baseedit_StartPlotDrag( kDragType_Sticky, plot );
            break;
        }

        actual_count++;
    }

    clipperPop();

    if( baseEdit_state.plot_dragging )
        base_setStatusMsg( textStd("PlacePlotString"), kBaseStatusMsg_Instruction );
    else
        base_setStatusMsg( textStd("PickPlotString"), kBaseStatusMsg_Instruction );

    return actual_count;    
}

bool playerIsInBaseEntrance()
{
    int block[2];
    BaseRoom *pRoom = baseRoomLocFromPos(&g_base, ENTPOS(playerPtr()), block);
    if( pRoom )
    {
        const RoomTemplate * entrance = basedata_GetRoomTemplateByName( "Entrance" );
        if( pRoom->info == entrance )
            return true;
    }
    return false;
}

static int notranslate( void * data )
{
    return false;
}

int baseInventoryWindow(void)
{

    float x, y, z, wd, ht, sc;
    int i, j=0, w, h, color, bcolor, item_count = 0;
    CBox cbox;

    static MultiBar energy_bar, control_bar;
    static ToolTip  powerTip, controlTip;

    static const RoomTemplate * last_room = NULL;
    static int last_entered_room = -1;
    static ScrollBar sb = { WDW_BASE_INVENTORY };
    static int last_item_count = 0;
    static int last_inv_type = -1;
    static char lastTab[64];
    int editmode = baseedit_Mode();
    int count;

    AtlasTex *engIcon = atlasLoadTexture("base_icon_power.tga");
    AtlasTex *conIcon = atlasLoadTexture("base_icon_control.tga");

    num_loaded_images = 0;
    
    if( editmode )
    {
        windowClientSizeThisFrame(&w, &h);
        window_setDims( WDW_BASE_INVENTORY, 0, h, w, -1 );
    }
    else
    {
        window_setDims( WDW_BASE_INVENTORY, -1, -1, BUTTON_AREA_WD*winDefs[WDW_BASE_INVENTORY].loc.sc, -1 );

        if( playerCanModifyBase(playerPtr(), &g_base) || playerCanPlacePersonal(playerPtr(), &g_base) )
        {
            if(playerIsInBaseEntrance())
                window_setMode( WDW_BASE_INVENTORY, WINDOW_GROWING );
            else
            {
                window_setMode( WDW_BASE_INVENTORY, WINDOW_SHRINKING );
            }
        }
        else
            window_setMode( WDW_BASE_INVENTORY, WINDOW_DOCKED );
    }

    // Do everything common windows are supposed to do.
    if (!window_getDims(WDW_BASE_INVENTORY, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
        return 0;

    // frame around window
    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );
    BuildCBox( &cbox, x, y, wd, ht );

    baseDrawInstructionButtons(x, y, z, ht, sc, color, bcolor );

    if( !editmode )
        return 0;

    if( (editmode == kBaseEdit_Architect || editmode == kBaseEdit_Plot) && !playerCanModifyBase(playerPtr(), &g_base) )
        return 0;

    if( editmode == kBaseEdit_AddPersonal && !playerCanPlacePersonal(playerPtr(), &g_base) )
        return 0;

    // frame around button area
    x += BUTTON_AREA_WD*sc;
    wd -= BUTTON_AREA_WD*sc;
    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, 0 );

    if( ! g_refCurRoom.p || g_base.supergroup_id && !playerPtr()->supergroup )
        return 0;

    if (last_inv_type != s_baseInventoryType)
    {
        last_inv_type = s_baseInventoryType;
        baseedit_CancelDrags();
        baseedit_CancelSelects();

        if( !baseRoomTabs )
            baseRoomTabs = uiTabControlCreate(TabType_Undraggable, 0, 0, 0, 0, 0);

        uiTabControlRemoveAll( baseRoomTabs );
        count = eaSize(&g_RoomTemplateDict.ppRooms);
        for( i = 0; i < count; i++ )
        {
            if( stricmp( g_RoomTemplateDict.ppRooms[i]->pchRoomCat, "Entrance" ) != 0  &&
                !uiTabControlHasData(baseRoomTabs, g_RoomTemplateDict.ppRooms[i]->pRoomCat) && 
                (gShowHiddenDetails || baseRoomMeetsRequires(g_RoomTemplateDict.ppRooms[i], playerPtr())) )
            {
                uiTabControlAdd( baseRoomTabs, g_RoomTemplateDict.ppRooms[i]->pRoomCat->pchDisplayName, g_RoomTemplateDict.ppRooms[i]->pRoomCat );
            }
        }

        if( !basePlotTabs )
            basePlotTabs = uiTabControlCreate(TabType_Undraggable, 0, 0, 0, 0, 0);

        uiTabControlRemoveAll( basePlotTabs );
        count = eaSize(&g_BasePlotDict.ppPlots);
        for( i = 0; i < count; i++ )
        {
            if((gShowHiddenDetails || basePlotMeetsRequires(g_BasePlotDict.ppPlots[i], playerPtr())) )
            {
                uiTabControlRemoveByName(basePlotTabs,g_BasePlotDict.ppPlots[i]->pchDisplayTabName);
                uiTabControlAdd( basePlotTabs, g_BasePlotDict.ppPlots[i]->pchDisplayTabName, g_BasePlotDict.ppPlots[i]->pchDisplayTabName );
            }
        }

    }

    sb.horizontal = true;
    doScrollBar( &sb, wd - 2*R10*sc, last_item_count*ITEM_WD*sc, (BUTTON_AREA_WD+R10)*sc, ht, z, &cbox, 0 );

    // Tabs
    if( !baseInventoryTabs )
    {
        baseInventoryTabs = uiTabControlCreate(TabType_Undraggable, 0, 0, 0, 0, 0);
        baseRoomStyleTabs = uiTabControlCreate(TabType_Undraggable, baseSetCurDecorToSelectedTab, 0, 0, 0, 0);

        uiTabControlAdd( baseRoomStyleTabs,  "AllString", (uiTabData)allocAddString("All"));
        count = ARRAY_SIZE(roomdecor_names);
        for( i = 0; i < ROOMDECOR_MAXTYPES; i++ )
            uiTabControlAdd( baseRoomStyleTabs, roomdecor_names[i], roomdecor_names[i] );

        gItemContext = contextMenu_Create( NULL );
        contextMenu_addVariableTitle( gItemContext, basecm_itemTitle, 0);
        contextMenu_addVariableTextTrans( gItemContext, basecm_itemInfo, 0,notranslate,0);
        contextMenu_addCode( gItemContext, alwaysAvailable, 0, baseitem_Info, 0, "CMInfoString", 0  );

        gRoomContext = contextMenu_Create( NULL );
        contextMenu_addVariableTitle( gRoomContext, basecm_itemTitle, 0);
        contextMenu_addVariableText( gRoomContext, basecm_roomInfo, 0);
        contextMenu_addCode( gRoomContext, alwaysAvailable, 0, baseitem_RoomInfo, 0, "CMInfoString", 0  );

        gPlotContext = contextMenu_Create( NULL );
        contextMenu_addVariableTitle( gPlotContext, basecm_itemTitle, 0);
        contextMenu_addVariableText( gPlotContext, basecm_plotInfo, 0);
        contextMenu_addCode( gPlotContext, alwaysAvailable, 0, baseitem_PlotInfo, 0, "CMInfoString", 0  );

        eaCreate(&tabSaves);
    }

    if( editmode == kBaseEdit_Architect || editmode == kBaseEdit_Plot || editmode == kBaseEdit_AddPersonal )
    {
        font( &game_12 );
        font_color( CLR_WHITE, CLR_WHITE );
        prnt( x + (2*R10-BUTTON_AREA_WD)*sc, y - PIX3*sc, z, sc, sc, textStd(g_base.supergroup_id ? "SupergroupPrestige" : "InfluenceAmount", playerFunds()));
    }
    if( editmode == kBaseEdit_Plot )
        s_baseInventoryType = kBaseInventory_Plot;

    // If we've run into a new room, set it as current
    {
        int block[2];
        BaseRoom *pRoom = baseRoomLocFromPos(&g_base, ENTPOS(playerPtr()), block);
        BaseBlock *pBlock = NULL;
        if( pRoom )
            pBlock = roomGetBlock(pRoom, block);
        if (pRoom && pBlock && pRoom->id != last_entered_room && pRoom->id != g_refCurRoom.id)
        { //we've entered a new room        
            baseedit_SetCurRoom(pRoom, block);
        }
        if (pRoom && pBlock && pRoom->id != last_entered_room)
            last_entered_room = pRoom->id;
    }

    // Rebuild inventory tablist depending on room
    if( last_room != g_refCurRoom.p->info && g_refCurRoom.p->info )
    {
        const RoomTemplate *room = g_refCurRoom.p->info;
        int num_details = eaSize(&g_DetailDict.ppDetails);
        int num_room_cats = eaSize(&room->ppDetailCatLimits);
        int num_tab_saves = eaSize(&tabSaves);
        char * tabName = uiTabControlGetTabName(baseInventoryTabs,uiTabControlGetSelectedIdx(baseInventoryTabs));
        if (last_room)
        { //if we just changed from a room, remember the tab state
            for (i = 0; i < num_tab_saves; i++)
            {
                if (tabSaves[i]->room == last_room)
                {
                    strncpyt(tabSaves[i]->tabName,tabName,49);
                    break;
                }
            }
            if (i == num_tab_saves)
            {
                TabSave *temp = malloc(sizeof(TabSave));
                temp->room = last_room;
                strncpyt(temp->tabName,tabName,49);
                eaPush(&tabSaves,temp);
            }
            strncpyt(lastTab,tabName,49);
        }

        uiTabControlRemoveAll(baseInventoryTabs);

        for( i = 0; i < num_room_cats; i++ )
        {
            for( j = 0; j < num_details; j++ )
            {
                const Detail *detail = g_DetailDict.ppDetails[j];
                const Detail ** tab;
                
                if( !detail->pchDisplayTabName )
                    continue;

                tab = baseTabList_get(detail->pchDisplayTabName);

                if( stricmp(room->ppDetailCatLimits[i]->pchCatName, detail->pchCategory) == 0 &&
                    tab && !uiTabControlHasData(baseInventoryTabs, (uiTabData)tab)
                    && (gShowHiddenDetails || (!detail->bObsolete && baseDetailMeetsRequires(detail, playerPtr())))  )
                {
                    uiTabControlAdd(baseInventoryTabs, detail->pchDisplayTabName, (uiTabData)tab);
                }
            }
        }
        for (i = 0; i < num_tab_saves; i++)
        { //select the tab we had selected before
            if (tabSaves[i]->room == room)
            {
                uiTabControlSelectByName(baseInventoryTabs,tabSaves[i]->tabName);
                break;
            }
        }
        if (i == num_tab_saves)
        { //if we didnt find a match, use the last tab
            uiTabControlSelectByName(baseInventoryTabs,lastTab);
        }
    }

    if( window_getMode(WDW_BASE_INVENTORY) != WINDOW_DISPLAYING )
        return 0;

    switch( s_baseInventoryType )
    {
        case kBaseInventory_CurRoom: // current room
            item_count = displayCurrentRoom(x, y, z, wd, ht, sc, color, sb.offset );
        break;

        case kBaseInventory_AddRoom: // buy a room
            item_count = displayAddRoom(x, y, z, wd, ht, sc, color, sb.offset );
        break;

        case kBaseInventory_Item:
            item_count = displayItem(x, y, z, wd, ht, sc, color, sb.offset );
        break;

        case kBaseInventory_Style:
            item_count = displayStyle(x, y, z, wd, ht, sc, color, sb.offset );
        break;

        case kBaseInventory_Personal:
            item_count = displayPersonal(x, y, z, wd, ht, sc, color, sb.offset );
            break;

        case kBaseInventory_Plot:
            item_count = displayPlot(x, y, z, wd, ht, sc, color, sb.offset );
            break;

        default:
            break;
    }

    {
        int errcode;
        if (!baseIsLegal(&g_base,&errcode))
        {
            base_setStatusMsg(textStd(getBaseErr(errcode)),kBaseStatusMsg_PassiveError);
        }
    }

    if( baseStatusMsg )
    {
        int baseStatusMsgColor;
        float strwd;
        float tx, tsc = sc;
        set_scissor(true);
        scissor_dims(BUTTON_AREA_WD*sc, 0, wd-270*sc, 2000);
        font( &game_12 );

        if( baseStatusMsgType == kBaseStatusMsg_Instruction )
            baseStatusMsgColor = 0x44ff44ff;
        if( baseStatusMsgType == kBaseStatusMsg_Warning )
            baseStatusMsgColor = 0xffff44ff;
        if( baseStatusMsgType == kBaseStatusMsg_Error || baseStatusMsgType == kBaseStatusMsg_PassiveError)
            baseStatusMsgColor = 0xff4444ff;

        tx = x + (wd-BUTTON_AREA_WD*sc)/2;
        strwd = str_wd(font_grp, tsc, tsc,baseStatusMsg);

        font_color( baseStatusMsgColor, baseStatusMsgColor);

        if( strwd > wd-270*sc )
        {
            tsc = str_sc(font_grp, wd-270*tsc, baseStatusMsg );
            strwd = str_wd(font_grp, tsc, tsc,baseStatusMsg);
            tx = BUTTON_AREA_WD*sc + (wd-270*sc)/2;
        }
        else
        {
            int right;
            if( tx + strwd/2 > wd-270*sc )
            {    
                tx = wd-270*sc - strwd/2;
                right = true;
            }
            if( tx - strwd/2 < BUTTON_AREA_WD*sc )
            {
                tx = BUTTON_AREA_WD*sc + strwd/2;
                if( right )
                    tx = BUTTON_AREA_WD*sc + (wd-270*sc)/2;
            }
        }

        cprnt( tx, y - PIX3*sc, z, tsc, tsc, baseStatusMsg );
        SAFE_FREE(baseStatusMsg)
        baseStatusMsgType = 0;
        set_scissor(false);
    }

    // some handy to know text
    font( &game_18 );
    font_color( CLR_WHITE, CLR_WHITE );
    addToolTip( &powerTip );
    addToolTip( &controlTip );

    // energy/control bars
    font( &gamebold_12 );

    display_sprite( engIcon, x + wd - 270*sc, y-17*sc, z, (16.f)/engIcon->width, (16.f)/engIcon->width, 0x88888ffff );
    drawMultiBar( &energy_bar, g_base.iEnergyProUI, g_base.iEnergyConUI, x+wd-200*sc, y-17*sc, z, .7f*sc, 100*sc, color );
    BuildCBox( &cbox, x+wd-(270)*sc, y-20*sc, 130*sc, 20*sc );
    setToolTipEx( &powerTip, &cbox, textStd("EnergyNeededUsed", g_base.iEnergyConUI, g_base.iEnergyProUI ), NULL, MENU_GAME, WDW_BASE_ROOM,1,TT_NOTRANSLATE );

    display_sprite( conIcon, x + wd - 140*sc, y-17*sc, z, (16.f)/conIcon->width, (16.f)/conIcon->width, 0xff66ffff );
    drawMultiBar( &control_bar, g_base.iControlProUI, g_base.iControlConUI, x+wd-70*sc, y-17*sc, z, .7f*sc, 100*sc, color );
    BuildCBox( &cbox, x+wd-(140)*sc, y-20*sc, 120*sc, 20*sc );
    setToolTipEx( &controlTip, &cbox, textStd("ControlNeededUsed", g_base.iControlConUI, g_base.iControlProUI), NULL, MENU_GAME, WDW_BASE_ROOM,1,TT_NOTRANSLATE );

    last_item_count = item_count;
    last_room = g_refCurRoom.p->info;

    return 0;
}



/* End of File */
