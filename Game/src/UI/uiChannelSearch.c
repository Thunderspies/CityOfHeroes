/*\
*
*    uiChannelsearch.h/c - Copyright 2004, 2005 Cryptic Studios
*        All Rights Reserved
*        Confidential property of Cryptic Studios
*
*    Shows a list of global chat channels and and allows the player to search them
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
#include "UI/uiChannelSearch.h"
#include "graphics/ttFontUtil.h"
#include "graphics/truetype/ttFontDraw.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "UI/uiClipper.h"
#include "UI/uiToolTip.h"
#include "uiEmote.h"
#include "filter/profanity.h"
#include "filter/validate_name.h"

#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/utils/utils.h>
#include "clientcomm/clientcomm.h"
#include "cmdparse/cmdgame.h"
#include "player/player.h"
#include "entity/entity.h"
#include "win/input.h"
#include "gameComm/chatClient.h"
#include "UI/uiChat.h"
#include "UI/uiChannel.h"
#include "UI/uiFriend.h"
#include "entity/character_base.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/uiOptions.h"

typedef struct ChannelLine
{
    char    name[MAX_CHANNELNAME];
    char    description[MAX_CHANNELDESC];
    int        members;
    int        online;

    ToolTip descriptionTip;
    ToolTipParent parentTip;

} ChannelLine;

MP_DEFINE(ChannelLine);
ChannelLine** gChannelLines = 0;

static UIListView* chanListView;
static int waitingUpdate = 0;
static char lastChannel[128];


static void clearChannelLine(ChannelLine *chanline)
{
    freeToolTip( &chanline->descriptionTip );
    MP_FREE(ChannelLine, chanline);
}

static void clearChannelLineAdapter(void* arg0)
{
    clearChannelLine((ChannelLine *)arg0);
}

static void clearChannelList(void)
{
    if (!gChannelLines)
        return;
    eaClearEx( &gChannelLines, clearChannelLineAdapter);
    uiLVClear(chanListView);
}

void receiveChannelData(char* name, char* description, int members, int online)
{    
    ChannelLine * chanline;

    if (!gChannelLines)
        eaCreate(&gChannelLines);

    if (waitingUpdate)
    {
        // If this is the first update in a stream, clear the list

        clearChannelList();
        waitingUpdate = 0;
    }

    if (!name || !name[0] || IsAnyWordProfane(name))
        return;

    // alloc space

    MP_CREATE(ChannelLine, 50);
    chanline = MP_ALLOC(ChannelLine);

    strncpyt(chanline->name, name, ARRAY_SIZE(chanline->name));
    strncpyt(chanline->description, description, ARRAY_SIZE(chanline->description));
    chanline->members = members;
    chanline->online = online;

    eaPush(&gChannelLines, chanline);

    uiLVAddItem(chanListView, chanline);

    uiLVSortBySelectedColumn(chanListView);

}

void chanRequestSearch(char* mode, char* value)
{
    static char lastMode[64];
    static char lastValue[512];
    static int last_req = 0;

    if( timerSecondsSince2000() - last_req <= 5 )
    {
        // Don't let the player spam the server

        return;
    }

    last_req = timerSecondsSince2000();


    waitingUpdate = 1;

    if (chanListView->selectedItem)
    {
        // Save the selected channel so it stays selected

        ChannelLine *line = chanListView->selectedItem;
        strcpy(lastChannel,line->name);
    }
    
    if (!value && !mode && lastMode[0])
    {
        // If we're passed in 0 for both, use the last value

        sendShardCmd("ChanFind","%s %s",lastMode, lastValue);
    }
    else
    {    
        if (!value)
            value = "";
        sendShardCmd("ChanFind","%s %s",mode, value);
        strncpyt(lastMode,mode,64);
        strncpyt(lastValue,value,512);
    }
}

static void requestDefault(void) 
{
    int i = 1;
    static StuffBuff sb;
    char rawname[128];
    char *trans;
    Entity *e = playerPtr();

    if (!e || !e->pchar)
        return;

    if (!sb.buff)
        initStuffBuff(&sb,1024);
    else
        clearStuffBuff(&sb);

    // First add the global defaults. They're pulled from menumessages.

    while (i < 10){        
        // Try to translate the key, and if it doesn't translate we know this
        // default channel doesn't exist.

        sprintf(rawname,"DefaultChannel%d",i);
        trans = textStd(rawname);
        if (strcmp(rawname,trans) == 0)
        {
            break;
        }
        addStringToStuffBuff(&sb,"%s,",trans);
        i++;
    }

    // Next add the channel for your shards

    i = 1;
    while (i < 10){        
        // Try to translate the key, and if it doesn't translate we know this
        // default channel doesn't exist.

        sprintf(rawname,"DefaultShardChannel%d",i);
        trans = textStd(rawname);
        if (strcmp(rawname,trans) == 0)
        {
            break;
        }
        addStringToStuffBuff(&sb,"%s",textStd(game_state.shard_name));
        addStringToStuffBuff(&sb,"%s,",trans);
        i++;
    }

    // Lastly add the archetype specific channel. Also pulled from MenuMessages

    sprintf(rawname,"Channel%s",e->pchar->pclass->pchName);
    addStringToStuffBuff(&sb, "%s,",textStd(rawname));

    // Drop the final ,
    sb.buff[strlen(sb.buff) - 1] = '\0';

    chanRequestSearch("list",sb.buff);
}


//-----------------------------------------------------------------------------------------------
// Deal with listview
//-----------------------------------------------------------------------------------------------


#define CHAN_NAME_COL        "Name"
#define CHAN_MEMBERS_COL    "Members"
#define CHAN_ONLINE_COL        "Online"
#define CHAN_DESC_COL        "Desc"

static int chanNameCompare(const ChannelLine** chan1, const ChannelLine** chan2)
{
    return stricmp( (*chan1)->name, (*chan2)->name );        
}

static int chanMembersCompare(const ChannelLine** chan1, const ChannelLine** chan2)
{
    if(  (*chan2)->members != (*chan1)->members )
        return (*chan2)->members - (*chan1)->members;
    else
        return stricmp( (*chan1)->name, (*chan2)->name );
}

static int chanOnlineCompare(const ChannelLine** chan1, const ChannelLine** chan2)
{
    if(  (*chan2)->online != (*chan1)->online )
        return (*chan2)->online - (*chan1)->online;
    else
        return stricmp( (*chan1)->name, (*chan2)->name );
}

static int chanDescCompare(const ChannelLine** chan1, const ChannelLine** chan2)
{
    return stricmp( (*chan1)->description, (*chan2)->description );        
}

static UIBox chanListViewDisplayItem(UIListView* list, PointFloatXYZ rowOrigin, void* userSettings, void* chanData, int itemIndex)
{
    ChannelLine* chan = (ChannelLine*)chanData;
    UIBox box;
    PointFloatXYZ pen = rowOrigin;
    UIColumnHeaderIterator columnIterator;
    float x, y, z, wd, ht, sc;
    int color, back;
    ChatChannel *channel = GetChannel(chan->name);

    if (channel)
        font_color( CLR_ONLINE_ITEM, CLR_ONLINE_ITEM );
    else
        font_color( CLR_WHITE, CLR_WHITE );

    window_getDims( WDW_CHANNEL_SEARCH, &x, &y, &z, &wd, &ht, &sc, &color, &back );

    box.origin.x = rowOrigin.x;
    box.origin.y = rowOrigin.y;

    uiCHIGetIterator(list->header, &columnIterator);
    rowOrigin.z+=1;

    BuildCBox( &chan->parentTip.box, x, rowOrigin.y, wd, list->rowHeight*sc );    

    if (!list->selectedItem && strcmp(chan->name,lastChannel) == 0)
    {
        uiLVSelectItem(list,itemIndex);
    }

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
        if(stricmp(column->name, CHAN_NAME_COL) == 0)
        {
            clipperPushRestrict( &clipBox );
            cprntEx( pen.x, pen.y + 18*sc, pen.z, sc, sc, NO_MSPRINT, chan->name );
            clipperPop();
        }
        else if(stricmp(column->name, CHAN_MEMBERS_COL) == 0)
        {
            clipperPushRestrict( &clipBox );
            cprnt( pen.x+columnIterator.currentWidth/2, pen.y + 18*sc, pen.z, sc, sc, "%i", chan->members );
            clipperPop();
        }
        else if(stricmp(column->name, CHAN_ONLINE_COL) == 0)
        {
            clipperPushRestrict( &clipBox );
            cprnt( pen.x+columnIterator.currentWidth/2, pen.y + 18*sc, pen.z, sc, sc, "%i", chan->online );
            clipperPop();
        }
        else if(stricmp(column->name, CHAN_DESC_COL) == 0)
        {
            if( !optionGet(kUO_AllowProfanity) )
                ReplaceAnyWordProfane(chan->description);
            cprntEx( pen.x, pen.y + 18*sc, pen.z, sc, sc, 0, "DontTranslate",chan->description );
            BuildCBox( &cbox, pen.x, pen.y,  wd - (pen.x-x), 20*sc );
            if (pen.y <= y+ht && pen.y >= y)
            {
                addToolTip( &chan->descriptionTip );
                setToolTipEx( &chan->descriptionTip, &cbox, chan->description, &chan->parentTip, MENU_GAME, WDW_CHANNEL_SEARCH, 1, TT_NOTRANSLATE );
            }
        }
    }

    box.height = 0;
    box.width = list->header->width;

    // Returning the bounding box of the item does not do anything right now.
    // See uiListView.c for more details.
    return box;
}

static UIListView * chanlist_initSettings(void)
{
    UIListView * chanListView;
    Wdw* chanWindow = wdwGetWindow(WDW_CHANNEL_SEARCH);

    chanListView = uiLVCreate();

    uiLVHAddColumnEx(chanListView->header,  CHAN_NAME_COL, "NameString", 20, 200, 1);
    uiLVBindCompareFunction(chanListView, CHAN_NAME_COL, chanNameCompare);

    uiLVHAddColumnEx(chanListView->header,  CHAN_MEMBERS_COL, "MembersString", 20, 100, 1);
    uiLVBindCompareFunction(chanListView, CHAN_MEMBERS_COL, chanMembersCompare);

    uiLVHAddColumnEx(chanListView->header,  CHAN_ONLINE_COL, "OnlineString", 20, 100, 1);
    uiLVBindCompareFunction(chanListView, CHAN_ONLINE_COL, chanOnlineCompare);

    uiLVHAddColumn(chanListView->header,  CHAN_DESC_COL, "DescriptionString", 10);
    uiLVBindCompareFunction(chanListView, CHAN_DESC_COL, chanDescCompare);

    uiLVFinalizeSettings(chanListView);
    uiLVEnableMouseOver(chanListView, 1);
    uiLVEnableSelection(chanListView, 1);
    uiLVSetClickable(chanListView,1);

    chanListView->displayItem = chanListViewDisplayItem;

    return chanListView;
}
//-----------------------------------------------------------------------------------------------
// Editing
//-----------------------------------------------------------------------------------------------

static UIEdit *editTxt;
static void chanEditInputHandler(UIEdit* edit)
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
                    chanRequestSearch("substring",uiEditGetUTF8Text(edit));
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

//-----------------------------------------------------------------------------------------------
// Display
//-----------------------------------------------------------------------------------------------

#define CHANLIST_BUTTON_HT        40
#define CHANLIST_HEADER_HT        56
#define CHANLIST_NAMECOL_WD        350
#define CHANLIST_TROPHYCOL_WD        100
#define CHANLIST_MEMBERCOL_WD        100
#define CHANLIST_LEVELCOL_WD        100
#define CHANLIST_TOTAL_WD            600

#define BUTTON_OFFSET            30
#define BUTTON_WD      95
#define BUTTON_SPACE   5

static char curchan[128];

static void confirmLeaveChannelDialog(void * data)
{
    if(curchan[0])
    {
        leaveChatChannel(curchan);
        curchan[0] = 0;
    }
}

static void setDescriptionDialog(void * data)
{
    if (curchan[0])
    {
        sendShardCmd("chandesc", "%s %s", curchan, dialogGetTextEntry());
        curchan[0] = 0;
    }
}

static void setMOTDDialog(void * data)
{
    if (curchan[0])
    {
        sendShardCmd("chanmotd", "%s %s", curchan, dialogGetTextEntry());
        curchan[0] = 0;
    }
}


int channelSearchWindow(void)
{
    float x, y, z, wd, ht, sc, hbottom;
    int color, bcolor;
    CBox box;

    static bool init = false;
    static ScrollBar chanroupSb = { WDW_CHANNEL_SEARCH };
    UIBox windowDrawArea;
    PointFloatXYZ pen;
    ChannelLine *line = NULL;

    if (!UsingChatServer())
    {
        window_setMode(WDW_CHANNEL_SEARCH,WINDOW_DOCKED);
    }

    // Do everything common windows are supposed to do.
    if ( !window_getDims( WDW_CHANNEL_SEARCH, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ))
    {
        init = false;
        uiEditReturnFocus(editTxt);
        return 0;
    }

    windowDrawArea.x = x;
    windowDrawArea.y = y + CHANLIST_HEADER_HT/2*sc;
    windowDrawArea.width = wd;
    windowDrawArea.height =  ht-(CHANLIST_HEADER_HT+6)*sc;
    uiBoxAlter(&windowDrawArea, UIBAT_SHRINK, UIBAD_ALL, PIX3*sc);

    pen.x = windowDrawArea.x;
    pen.y = windowDrawArea.y;
    pen.z = z;

    // Make sure the list view is initialized.
    if(!chanListView)
        chanListView = chanlist_initSettings();

    if( !init ) // one time inits
    {
        init = true;
        if(editTxt)
            uiEditClear(editTxt);
        else
        {
            editTxt = uiEditCreateSimple(NULL, MAX_CHANNELNAME, &game_12, CLR_WHITE, chanEditInputHandler);
            uiEditSetClicks(editTxt, "type2", "mouseover");
            uiEditSetMinWidth(editTxt, 9000);
        }
        requestDefault();
    }

    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );// window 
    if( eaSize( &gChannelLines ) == 0 )
    {
        font_color( CLR_WHITE, CLR_WHITE );
        cprntEx(x + wd/2, y+ht/2, z+2, sc, sc, (CENTER_X|CENTER_Y), "NoChannelString" );
        line = NULL;
    }
    else
    {
        // Draw the list of Channels
        // Draw and handle all scroll bar functionaltiy if the friendlist is larger than the window.
        // This is seperate from the list view because the scroll bar is always outside the draw area
        // of the list view.
        if(chanListView)
        {
            int fullDrawHeight = uiLVGetFullDrawHeight(chanListView);

            if(chanListView)
            {
                if(fullDrawHeight > windowDrawArea.height)
                {
                    doScrollBar( &chanroupSb, windowDrawArea.height - 10*sc, fullDrawHeight, wd,windowDrawArea.y - y+10*sc, z+2, 0, &windowDrawArea );
                    chanListView->scrollOffset = chanroupSb.offset;
                }
                else
                    chanListView->scrollOffset = 0;
            }
        }

        uiLVSetDrawColor(chanListView, window_GetColor(WDW_CHANNEL_SEARCH));
        uiLVSetHeight(chanListView, windowDrawArea.height);
        uiLVHSetWidth(chanListView->header, windowDrawArea.width);
        uiLVHFinalizeSettings(chanListView->header);

        // How tall and wide is the list view supposed to be?
        clipperPushRestrict(&windowDrawArea);
        uiLVSetScale(chanListView, sc );
        uiLVDisplay(chanListView, pen);
        clipperPop();

        line = chanListView->selectedItem;
        uiLVHandleInput(chanListView, pen);
        if (line != chanListView->selectedItem)
        {
            if (line = chanListView->selectedItem)
                selectChannelWindow(line->name);
        }
    }

    hbottom = (CHANLIST_HEADER_HT + 11)*sc;

    // Search buttons

    if( D_MOUSEHIT == drawStdButton( x + (BUTTON_WD/2 + BUTTON_SPACE*2)*sc, y + hbottom - BUTTON_OFFSET*sc/2 - 35*sc, z, BUTTON_WD*sc, 25*sc, color, "ChanMine", 1.f, 0 ) )
    {
        chanRequestSearch("memberof","");
    }

    if( D_MOUSEHIT == drawStdButton( x + (BUTTON_WD*3/2 + BUTTON_SPACE*3)*sc, y + hbottom - BUTTON_OFFSET*sc/2 - 35*sc, z, BUTTON_WD*sc, 25*sc, color, "ChanDefaults", 1.f, 0 ) )
    {
        requestDefault();
    }

    if( D_MOUSEHIT == drawStdButton( x + (BUTTON_WD*5/2 + BUTTON_SPACE*4)*sc, y + hbottom - BUTTON_OFFSET*sc/2 - 35*sc, z, BUTTON_WD*sc, 25*sc, color, "TopChannels", 1.f, 0 ) )
    {
        chanRequestSearch("substring","");
    }

    if( D_MOUSEHIT == drawStdButton( x + wd - (BUTTON_WD/2 + BUTTON_SPACE*2)*sc, y + hbottom - BUTTON_OFFSET*sc/2 - 35*sc, z, BUTTON_WD*sc, 25*sc, color, "SearchString", 1.f, 0 ) )
    {
        chanRequestSearch("substring",uiEditGetUTF8Text(editTxt));
    }

    // Name search
    font_color(CLR_WHITE, CLR_WHITE);
    prnt( x + wd - 300*sc, y + hbottom - BUTTON_OFFSET*sc/2 - 29*sc, z+10, sc, sc, "NameColon" );
    uiEditEditSimple(editTxt, x + wd - 255*sc, y + hbottom - BUTTON_OFFSET*sc/2 - 44*sc, z+2, 135*sc, 25*sc, &game_12, INFO_HELP_TEXT, FALSE);

    BuildCBox( &box, x + wd - 305*sc,  y + hbottom - BUTTON_OFFSET*sc/2 - 47*sc, 190*sc, 25*sc );
    drawFrame( PIX3, R10, x + wd  - 305*sc,  y + hbottom - BUTTON_OFFSET*sc/2 - 47*sc, z+1, 195*sc, 25*sc, sc, color, mouseCollision(&box)?0x222222ff:CLR_BLACK );

    if( mouseClickHit( &box, MS_LEFT) )
        uiEditTakeFocus(editTxt);
    if( (!mouseCollision(&box) && mouseDown(MS_LEFT)) )
        uiEditReturnFocus(editTxt);


    // Action Buttons

    hbottom = ht;

    if (line)
    {
        ChatChannel *chan = GetChannel(line->name);
        if (chan)
        {
            if( D_MOUSEHIT == drawStdButton( x + wd - (60 + BUTTON_SPACE*2)*sc, y + hbottom - BUTTON_OFFSET*sc/2 - 6*sc, z, 120*sc, 25*sc, color, "LeaveChannelButton", 1.f, 0 ) )
            {
                strcpy(curchan,line->name);
                dialogStd(DIALOG_YES_NO, textStd("ConfirmLeaveChannelDialog"), NULL, NULL, confirmLeaveChannelDialog, NULL, DLGFLAG_GAME_ONLY);                
            }
            if(isOperator(chan->me->flags))
            {            
                if( D_MOUSEHIT == drawStdButton( x + (60 + BUTTON_SPACE*2)*sc, y + hbottom - BUTTON_OFFSET*sc/2 - 6*sc, z, 120*sc, 25*sc, color, "ChanSetDescription", 1.f, 0 ) )
                {
                    strcpy(curchan,line->name);                    
                    dialogSetTextEntry( line->description );
                    dialog(DIALOG_OK_CANCEL_TEXT_ENTRY, -1, -1, -1, -1,
                        textStd("ChanDescriptionEdit", line->name), NULL, setDescriptionDialog, NULL, NULL,
                        DLGFLAG_GAME_ONLY|DLGFLAG_NO_TRANSLATE, NULL, NULL, 0, 0, MAX_CHANNELDESC, 0 );
                }
                if( D_MOUSEHIT == drawStdButton( x +  (180 + BUTTON_SPACE*3)*sc, y + hbottom - BUTTON_OFFSET*sc/2 - 6*sc, z, 120*sc, 25*sc, color, "ChanSetMOTD", 1.f, 0 ) )
                {
                    strcpy(curchan,line->name);                                        
                    dialog(DIALOG_OK_CANCEL_TEXT_ENTRY, -1, -1, -1, -1,
                        textStd("ChanMOTDEdit", line->name), NULL, setMOTDDialog, NULL, NULL,
                        DLGFLAG_GAME_ONLY|DLGFLAG_NO_TRANSLATE, NULL, NULL, 0, 0, MAX_MESSAGE,0 );
                }

                // MAKE PRIVATE Button
                if( chan->flags & CHANFLAGS_JOIN)
                {
                    if( D_MOUSEHIT == drawStdButton( x + (300 + BUTTON_SPACE*4)*sc, y + hbottom - BUTTON_OFFSET*sc/2 - 6*sc, z, 120*sc, 25*sc, color, "MakeChannelPrivate", 1.f, 0 ) )
                    {
                        changeChannelMode_Long(chan->name, "-join");
                    }
                }
                // MAKE PUBLIC Button
                else //if(gCurrentChannel->flags & CHANFLAGS_JOIN)
                {
                    if( D_MOUSEHIT == drawStdButton( x + (300 + BUTTON_SPACE*4)*sc, y + hbottom - BUTTON_OFFSET*sc/2 - 6*sc, z, 120*sc, 25*sc, color, "MakeChannelPublic", 1.f, 0 ) )
                    {
                        changeChannelMode_Long(chan->name, "+join");
                    }
                }
            }
        }
        else
        {        

            if( D_MOUSEHIT == drawStdButton( x + wd/2, y + hbottom-18*sc, z, 120*sc, 25*sc, color, "JoinChannel", 1.f, 0 ) )
            {
                joinChatChannel(line->name);
                ChatFilterAddPendingChannel(gActiveChatFilter, line->name, false);
            }
        }
    }

    return 0;
}
