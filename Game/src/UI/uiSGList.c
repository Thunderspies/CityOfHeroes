/*\
 *
 *    uiSGList.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Shows a list of supergroups at the registration desk
 *
 */

#include "gameComm/wdwbase.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiWindows.h"
#include "UI/uiDialog.h"
#include "UI/uiComboBox.h"
#include "uiSlider.h"
#include "UI/uiScrollBar.h"
#include "UI/uiContextMenu.h"
#include "UI/uiChatUtil.h"
#include "UI/uiEditText.h"
#include "UI/uiInput.h"
#include "UI/uiReticle.h"
#include "UI/uiCursor.h"
#include "UI/uiFocus.h"
#include "UIEdit.h"
#include "UI/uiGame.h"
#include "UI/uiInfo.h"
#include "uiHelp.h"
#include "UI/uiNet.h"
#include "graphics/textureatlas.h"
#include "UI/uiSGList.h"
#include "graphics/ttFontUtil.h"
#include "graphics/truetype/ttFontDraw.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "UI/uiSuperRegistration.h"
#include "UI/uiClipper.h"
#include "UI/uiToolTip.h"
#include "uiEmote.h"
#include "UI/uiChat.h"
#include "UI/uiOptions.h"
#include "filter/profanity.h"
#include "filter/validate_name.h"

#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/utils/utils.h>
#include "clientcomm/clientcomm.h"
#include "entity/entVarUpdate.h"
#include "cmdparse/cmdgame.h"
#include "player/player.h"
#include "entity/entity.h"
#include "entity/Supergroup.h"
#include "win/input.h"

#include <utilitieslib/language/AppLocale.h>

#define SGLIST_BUTTON_HT        40
#define SGLIST_NAMECOL_WD        350
#define SGLIST_TROPHYCOL_WD        100
#define SGLIST_MEMBERCOL_WD        100
#define SGLIST_LEVELCOL_WD        100
#define SGLIST_TOTAL_WD            600

#define BUTTON_OFFSET            30

typedef struct SupergroupLine
{
    int        sgid;
    int        members;
    int        prestige;
    char    name[256];
    char    description[512];

    ToolTip commentTip;
    ToolTipParent parentTip;

} SupergroupLine;

MP_DEFINE(SupergroupLine);
SupergroupLine** gSupergroupLines = 0;

static UIListView* sgListView;

//-----------------------------------------------------------------------------------------------
// Networking
//-----------------------------------------------------------------------------------------------

static void supergroupListRequest(int search_by_prestige, char * name )
{
    if( name && name[0] )
    {
        strchrReplace( name, '\'', '\0' );
        if( name[0] )
            stripNonRegionChars(name, getCurrentLocale());
        else
            return;
    }

    START_INPUT_PACKET( pak, CLIENTINP_SUPERGROUP_LIST_REQUEST);
    pktSendBits(pak,1,search_by_prestige);
    pktSendString(pak, name);
    END_INPUT_PACKET
}

static void clearSupergroupLine(SupergroupLine *sgline)
{
    freeToolTip( &sgline->commentTip );
    MP_FREE(SupergroupLine, sgline);
}

void receiveSupergroupList(Packet *pak)
{
    int count, i;

    // alloc space
    count = pktGetBitsPack(pak, 1);
    if (!gSupergroupLines)
        eaCreate(&gSupergroupLines);
    eaClearEx( &gSupergroupLines, clearSupergroupLine);

    for (i = 0; i < count; i++)
    {
        SupergroupLine * sgline;
        MP_CREATE(SupergroupLine, 50);
        sgline = MP_ALLOC(SupergroupLine);
        
        sgline->sgid = pktGetBitsPack(pak, 1);
        strncpyt(sgline->name, unescapeString(pktGetString(pak)), ARRAY_SIZE(sgline->name));
        strncpyt(sgline->description, unescapeString(pktGetString(pak)), ARRAY_SIZE(sgline->description));
        sgline->prestige = pktGetBitsPack(pak, 1);

        eaPush(&gSupergroupLines, sgline);
    }

    // Rebuild the entire list if required.
    uiLVClear(sgListView);
    for(i = 0; i < eaSize(&gSupergroupLines); i++)
        uiLVAddItem(sgListView, gSupergroupLines[i]);

    uiLVSortBySelectedColumn(sgListView);

    winDefs[WDW_SUPERGROUP_LIST].loc.locked = 0;
    window_setMode(WDW_SUPERGROUP_LIST, WINDOW_GROWING);
}

//-----------------------------------------------------------------------------------------------
// Deal with listview
//-----------------------------------------------------------------------------------------------

#define SG_NAME_COL        "Name"
#define SG_PRESTIGE_COL    "Prestige"
#define SG_DESC_COL        "Desc"

static int sgNameCompare(const SupergroupLine** sg1, const SupergroupLine** sg2)
{
    return stricmp( (*sg1)->name, (*sg2)->name );        
}

static int sgPrestigeCompare(const SupergroupLine** sg1, const SupergroupLine** sg2)
{
    if(  (*sg2)->prestige != (*sg1)->prestige )
        return (*sg2)->prestige - (*sg1)->prestige;
    else
        return stricmp( (*sg1)->name, (*sg1)->name );
}


static int sgDescCompare(const SupergroupLine** sg1, const SupergroupLine** sg2)
{
    return stricmp( (*sg1)->description, (*sg1)->description );        
}

static UIBox sgListViewDisplayItem(UIListView* list, PointFloatXYZ rowOrigin, void* userSettings, SupergroupLine* sg, int itemIndex)
{
    UIBox box;
    PointFloatXYZ pen = rowOrigin;
    UIColumnHeaderIterator columnIterator;
    float x, y, z, wd, ht, sc;
    int color, back;

    window_getDims( WDW_SUPERGROUP_LIST, &x, &y, &z, &wd, &ht, &sc, &color, &back );

    box.origin.x = rowOrigin.x;
    box.origin.y = rowOrigin.y;

    uiCHIGetIterator(list->header, &columnIterator);
    rowOrigin.z+=1;

    BuildCBox( &sg->parentTip.box, x, rowOrigin.y, wd, list->rowHeight*sc );

    // Iterate through each of the columns.
    for(;uiCHIGetNextColumn(&columnIterator, list->scale);)
    {
        UIColumnHeader* column = columnIterator.column;
        float sc = list->scale;
        UIBox clipBox;
        CBox cbox;
        pen.x = rowOrigin.x + columnIterator.columnStartOffset*sc;

        clipBox.origin.x = pen.x;
        clipBox.origin.y = pen.y;
        clipBox.height = 20*sc;
        clipBox.width = (columnIterator.currentWidth+5)*sc;

        // Decide what to draw on on which column.
        if(stricmp(column->name, SG_NAME_COL) == 0)
        {
            clipperPushRestrict( &clipBox );
            cprntEx( pen.x, pen.y + 18*sc, pen.z, sc, sc, NO_MSPRINT, sg->name );
            clipperPop();
        }
        else if(stricmp(column->name, SG_PRESTIGE_COL) == 0)
        {
            clipperPushRestrict( &clipBox );
            cprnt( pen.x+columnIterator.currentWidth/2, pen.y + 18*sc, pen.z, sc, sc, "%i", sg->prestige );
            clipperPop();
        }
        else if(stricmp(column->name, SG_DESC_COL) == 0)
        {
             if( !optionGet(kUO_AllowProfanity)  )
                ReplaceAnyWordProfane(sg->description);
            cprntEx( pen.x, pen.y + 18*sc, pen.z, sc, sc, 0, "DontTranslate",sg->description );
            BuildCBox( &cbox, pen.x, pen.y,  wd - (pen.x-x), 20*sc );
             if (pen.y <= y+ht && pen.y >= y)
            {
                addToolTip( &sg->commentTip );
                setToolTipEx( &sg->commentTip, &cbox, (char*)unescapeString(sg->description), &sg->parentTip, MENU_GAME, WDW_SUPERGROUP_LIST, 1, TT_NOTRANSLATE );
            }
        }
    }

    box.height = 0;
    box.width = list->header->width;

    // Returning the bounding box of the item does not do anything right now.
    // See uiListView.c for more details.
    return box;
}

static UIListView * sglist_initSettings(void)
{
    UIListView * sgListView;
    Wdw* sgWindow = wdwGetWindow(WDW_SUPERGROUP_LIST);

    sgListView = uiLVCreate();

    uiLVHAddColumnEx(sgListView->header,  SG_NAME_COL, "NameString", 20, 200, 1);
    uiLVBindCompareFunction(sgListView, SG_NAME_COL, sgNameCompare);

    uiLVHAddColumnEx(sgListView->header,  SG_PRESTIGE_COL, "PrestigeString", 20, 100, 1);
    uiLVBindCompareFunction(sgListView, SG_PRESTIGE_COL, sgPrestigeCompare);

    uiLVHAddColumn(sgListView->header,  SG_DESC_COL, "DescriptionString", 10);
    uiLVBindCompareFunction(sgListView, SG_DESC_COL, sgDescCompare);

    uiLVFinalizeSettings(sgListView);
    uiLVEnableMouseOver(sgListView, 0);
    uiLVEnableSelection(sgListView, 0);
    uiLVSetClickable(sgListView,0);

    sgListView->displayItem = sgListViewDisplayItem;

    return sgListView;
}


//-----------------------------------------------------------------------------------------------
// Editing
//-----------------------------------------------------------------------------------------------

static UIEdit *editTxt;
static void sgEditInputHandler(UIEdit* edit)
{
    KeyInput* input;

    // Handle keyboard input.
    input = inpGetKeyBuf();

    while(input )
    {
        if(input->type == KIT_EditKey)
        {
            switch(input->key)
            {
            case INP_ESCAPE:
                {
                    uiEditClear(edit);
                    uiEditReturnFocus(edit);
                }
                break;
            case INP_NUMPADENTER: /* fall through */
            case INP_RETURN:
                {
                    static int last_req = 0;

                    if( timerSecondsSince2000() - last_req > 5 )
                    {
                        supergroupListRequest(0,uiEditGetUTF8Text(edit));
                        last_req = timerSecondsSince2000();
                    }
                    uiEditReturnFocus(edit);
                }
                break;
            default:
                uiEditDefaultKeyboardHandler(edit, input);
                break;
            }
        }
        else
            uiEditDefaultKeyboardHandler(edit, input);
        inpGetNextKey(&input);
    }

    uiEditDefaultMouseHandler(edit);
}

static char* itos(int i)
{
    static char buf[20];
    sprintf(buf, "%i", i);
    return buf;
}


//-----------------------------------------------------------------------------------------------
// Display
//-----------------------------------------------------------------------------------------------



int supergroupListWindow(void)
{
    float x, y, z, wd, ht, sc;
    int color, bcolor;
    CBox box;

    static bool init = false;
    static ScrollBar sgroupSb = { WDW_SUPERGROUP_LIST };
    UIBox windowDrawArea;
    PointFloatXYZ pen;

    // Do everything common windows are supposed to do.
       if ( !window_getDims( WDW_SUPERGROUP_LIST, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ))
    {
        init = false;
        uiEditReturnFocus(editTxt);
        return 0;
    }

    windowDrawArea.x = x;
    windowDrawArea.y = y;
    windowDrawArea.width = wd;
    windowDrawArea.height =  ht-(SGLIST_BUTTON_HT)*sc;
    uiBoxAlter(&windowDrawArea, UIBAT_SHRINK, UIBAD_ALL, PIX3*sc);

    pen.x = windowDrawArea.x;
    pen.y = windowDrawArea.y;
    pen.z = z;

     // Make sure the list view is initialized.
     if(!sgListView)
        sgListView = sglist_initSettings();

      if( !init ) // one time inits
    {
        init = true;
        if(editTxt)
            uiEditClear(editTxt);
        else
        {
            editTxt = uiEditCreateSimple(NULL, MAX_PLAYER_NAME_LEN(e), &game_12, CLR_WHITE, sgEditInputHandler);
            uiEditSetClicks(editTxt, "type2", "mouseover");
            uiEditSetMinWidth(editTxt, 9000);
        }
    }

    font_set( &game_12, 0, 0, 1, 1, CLR_WHITE, CLR_WHITE );

    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );// window 
     if( eaSize( &gSupergroupLines ) == 0 )
    {
        cprntEx(x + wd/2, y+ht/2, z+2, sc, sc, (CENTER_X|CENTER_Y), "NoLfgString" );
    }
    else
    {
        // Draw the list of supergroups
        // Draw and handle all scroll bar functionaltiy if the friendlist is larger than the window.
        // This is seperate from the list view because the scroll bar is always outside the draw area
        // of the list view.
        if(sgListView)
        {
            int fullDrawHeight = uiLVGetFullDrawHeight(sgListView);

            if(sgListView)
            {
                if(fullDrawHeight > windowDrawArea.height)
                {
                    doScrollBar( &sgroupSb, windowDrawArea.height, fullDrawHeight, wd,(R10+PIX3)*sc, z+2, 0, &windowDrawArea  );
                    sgListView->scrollOffset = sgroupSb.offset;
                }
                else
                    sgListView->scrollOffset = 0;
            }
        }

        uiLVSetDrawColor(sgListView, window_GetColor(WDW_SUPERGROUP_LIST));
        uiLVSetHeight(sgListView, windowDrawArea.height);
        uiLVHSetWidth(sgListView->header, windowDrawArea.width);
        uiLVHFinalizeSettings(sgListView->header);

        // How tall and wide is the list view supposed to be?
         clipperPushRestrict(&windowDrawArea);
        uiLVSetScale(sgListView, sc );
        uiLVDisplay(sgListView, pen);
        clipperPop();

        uiLVHandleInput(sgListView, pen);
    }

    // draw buttons
       if( D_MOUSEHIT == drawStdButton( x + wd/3-45*sc, y + ht - BUTTON_OFFSET*sc/2 - 2*PIX3, z, 200*sc, 25*sc, color, "TopPrestige", 1.f, 0 ) )
    {
        supergroupListRequest(1,NULL);
    }

    // Name search
    font_color(CLR_WHITE, CLR_WHITE);
     prnt( x + 2*wd/3 - 54*sc, y + ht - BUTTON_OFFSET*sc/2 - 2*PIX3 + 6*sc, z+10, sc, sc, "NameColon" );
       uiEditEditSimple(editTxt, x + 2*wd/3 - 10*sc, y + ht - BUTTON_OFFSET*sc/2 - 2*PIX3 - 7*sc, z+2, 160*sc, 25*sc, &game_12, INFO_HELP_TEXT, FALSE);

     BuildCBox( &box, x + 2*wd/3 - 60*sc,  y + ht - BUTTON_OFFSET*sc/2 - 2*PIX3*sc - 12*sc, 200*sc, 25*sc );
    drawFrame( PIX3, R10, x + 2*wd/3 - 60*sc,  y + ht - BUTTON_OFFSET*sc/2 - 2*PIX3*sc - 12*sc, z+1, 200*sc, 25*sc, sc, color, mouseCollision(&box)?0x222222ff:CLR_BLACK );

    if( mouseClickHit( &box, MS_LEFT) )
        uiEditTakeFocus(editTxt);
    if( (!mouseCollision(&box) && mouseDown(MS_LEFT)) )
        uiEditReturnFocus(editTxt);

    return 0;
}




