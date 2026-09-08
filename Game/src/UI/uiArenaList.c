

#include "gameComm/wdwbase.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiWindows.h"
#include "UI/uiComboBox.h"
#include "uiSlider.h"
#include "UI/uiScrollBar.h"
#include "uiListView.h"
#include "UI/uiClipper.h"
#include "UI/uiTabControl.h"
#include "UI/uiNet.h"
#include "uiHelp.h"
#include <utilitieslib/components/EString.h>
#include "UI/uiInput.h"
#include "UI/uiGame.h"
#include "formatter/smf_main.h"

#include "graphics/ttFontUtil.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "gameData/arenastruct.h"
#include "player/player.h"
#include <utilitieslib/components/earray.h>
#include "graphics/textureatlas.h"
#include "arena/arenagame.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/timing.h>
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "UI/uiToolTip.h"
#include <utilitieslib/utils/timing.h>
#include "entity/entity.h"
#include "auth/authUserData.h"
#include "clientcomm/dbclient.h"
#include <utilitieslib/language/MessageStoreUtil.h>

#include "UI/uiUtilGame.h" // for drawBox debugging

//--------------------------------------------------------------------------------------------------------------------------
// Globals and Defines /////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------------------

int filterColors[] = { 
    0xffffbbff,        //ARENA_FILTER_ELIGABLE
    0xbbffbbff,        //ARENA_FILTER_BEST_FIT
    0xffbbbbff,        //ARENA_FILTER_ONGOING
    0xaaaaaaff,        //ARENA_FILTER_COMPLETED,
    CLR_PARAGON,    //ARENA_FILTER_MY_EVENTS,
    0xffbbffff,        //ARENA_FILTER_INELIGABLE,
    0xbbffffff,        //ARENA_FILTER_PLAYER,
    0xffbbffff,        //ARENA_FILTER_ALL,
};

static UIListView* lvEvents[ARENA_TAB_TOTAL];
static UIListView* lvCurrent = 0;
static int lastSelection[ARENA_TAB_TOTAL] = {0};

static ArenaFilter arenaFilter = ARENA_FILTER_BEST_FIT;
static ComboBox comboFilter;

static char * eventNames[ARENA_NUM_TEAM_STYLES][ARENA_NUM_TEAM_TYPES] = {
    { "FreeformString",        "ArenaTeam",            "SupergroupRumble"    },
    { "Invalid",            "5TARString",            "Invalid"            },
    { "Invalid",            "Villain5TARString",    "Invalid"            },
    { "Invalid",            "Versus5TARString",        "Invalid"            },
    { "Invalid",            "7TARString",            "Invalid"            },
    { "Invalid",            "Villain7TARString",    "Invalid"            },
    { "Invalid",            "Versus7TARString",        "Invalid"            },
};

#define ARENA_MINE            "Scheduled"
#define ARENA_EVENT_TYPE    "EventType" 
#define ARENA_PLAYERS        "PlayersString"
#define ARENA_WEIGHT        "WeightString"
#define ARENA_CREATOR        "CreatorString"    
#define ARENA_FEE            "EntryFee"
#define ARENA_START            "StartTime"
#define ARENA_STATUS        "Status"


char * arenaGetEventName( int eventType, int teamType )
{
    return eventNames[eventType][teamType];
}

//--------------------------------------------------------------------------------------------------------------------------
// Listview management /////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------------------

static int arenaScheduledCompare( const ArenaEvent **a1, const ArenaEvent **a2 )
{
    ArenaEvent *event1 = *(ArenaEvent**)a1;
    ArenaEvent *event2 = *(ArenaEvent**)a2;

    return (event1->filter==ARENA_FILTER_MY_EVENTS)-(event2->filter==ARENA_FILTER_MY_EVENTS);
}

static int arenaEventCompare( const ArenaEvent **a1, const ArenaEvent **a2 )
{
    ArenaEvent *event1 = *(ArenaEvent**)a1;
    ArenaEvent *event2 = *(ArenaEvent**)a2;

    int tmp = event1->teamStyle - event2->teamStyle;

    if( !tmp )
    {
        int tmp2 = event1->teamType - event2->teamType;

        if( !tmp2 )
            return event1->sanctioned - event2->sanctioned;
        else
            return tmp2;
    }
    else
        return tmp;
}

static int arenaPlayerCompare( const ArenaEvent **a1, const ArenaEvent **a2 )
{
    ArenaEvent *event1 = *(ArenaEvent**)a1;
    ArenaEvent *event2 = *(ArenaEvent**)a2;

    return (event1->maxplayers-event2->maxplayers);
}

static int arenaWeightCompare( const ArenaEvent **a1, const ArenaEvent **a2 )
{
    ArenaEvent *event1 = *(ArenaEvent**)a1;
    ArenaEvent *event2 = *(ArenaEvent**)a2;

    return event1->weightClass - event2->weightClass;
}

static int arenaCreatorCompare( const ArenaEvent **a1, const ArenaEvent **a2 )
{
    ArenaEvent *event1 = *(ArenaEvent**)a1;
    ArenaEvent *event2 = *(ArenaEvent**)a2;

    if( event1->creatorname && event2->creatorname )
        return stricmp( event1->creatorname, event2->creatorname );
    else if ( event1->creatorname )
        return 1;
    else if ( event2->creatorname )
        return -1;
    else
        return event1->creatorid-event2->creatorid;
}

static int arenaFeeCompare( const ArenaEvent **a1, const ArenaEvent **a2 )
{
    ArenaEvent *event1 = *(ArenaEvent**)a1;
    ArenaEvent *event2 = *(ArenaEvent**)a2;

    return event1->entryfee-event2->entryfee;
}

static int arenaStartCompare( const ArenaEvent **a1, const ArenaEvent **a2 )
{
    ArenaEvent *event1 = *(ArenaEvent**)a1;
    ArenaEvent *event2 = *(ArenaEvent**)a2;

    return event1->start_time-event2->start_time;
}

static int arenaStatusCompare( const ArenaEvent **a1, const ArenaEvent **a2 )
{
    ArenaEvent *event1 = *(ArenaEvent**)a1;
    ArenaEvent *event2 = *(ArenaEvent**)a2;

    return event1->phase-event2->phase;
}



static UIBox lvEventsDisplayItem(UIListView* list, PointFloatXYZ rowOrigin, void* userSettings, void* itemData, int itemIndex);

static void arenaListInit()
{
    int i;
    for( i = 0; i < ARENA_TAB_TOTAL; i++ )
    {
        lvEvents[i] = uiLVCreate();

        uiLVHAddColumn(lvEvents[i]->header, ARENA_MINE, "", 10);
        uiLVBindCompareFunction(lvEvents[i], ARENA_MINE, arenaScheduledCompare );

        uiLVHAddColumn(lvEvents[i]->header, ARENA_EVENT_TYPE, ARENA_EVENT_TYPE, 100);
        uiLVBindCompareFunction(lvEvents[i], ARENA_EVENT_TYPE, arenaEventCompare );

        uiLVHAddColumn(lvEvents[i]->header, ARENA_CREATOR, ARENA_CREATOR, 90);
        uiLVBindCompareFunction(lvEvents[i], ARENA_CREATOR, arenaCreatorCompare );

        uiLVHAddColumn(lvEvents[i]->header, ARENA_PLAYERS, ARENA_PLAYERS, 30);
        uiLVBindCompareFunction(lvEvents[i], ARENA_PLAYERS, arenaPlayerCompare );

        uiLVHAddColumn(lvEvents[i]->header, ARENA_WEIGHT, ARENA_WEIGHT, 20);
        uiLVBindCompareFunction(lvEvents[i], ARENA_WEIGHT, arenaWeightCompare );

        uiLVHAddColumn(lvEvents[i]->header, ARENA_FEE, ARENA_FEE, 30);
        uiLVBindCompareFunction(lvEvents[i], ARENA_FEE, arenaFeeCompare );

        uiLVHAddColumn(lvEvents[i]->header, ARENA_START, ARENA_START, 70);
        uiLVBindCompareFunction(lvEvents[i], ARENA_START, arenaStartCompare );

        uiLVHAddColumn(lvEvents[i]->header, ARENA_STATUS, ARENA_STATUS, 50);
        uiLVBindCompareFunction(lvEvents[i], ARENA_STATUS, arenaStatusCompare );

        lvEvents[i]->displayItem = lvEventsDisplayItem;

        uiLVFinalizeSettings(lvEvents[i]);

        uiLVEnableMouseOver(lvEvents[i], 1);
        uiLVEnableSelection(lvEvents[i], 1);
    }
}

void arenaRebuildListView()
{
    int i, j;
    Entity * e = playerPtr();

    for( i = 0; i < ARENA_TAB_TOTAL; i++ )
    {
        // Clear the list.  This also kills the current selection.
        ArenaEventList* kiosk = getArenaKiosk();
        uiLVClear(lvEvents[i]);

        if(lvEvents[i] && kiosk->events)
        {
            // Iterate over all supergroup memebers.
            for(j = 0; j < eaSize(&kiosk->events); j++)
            {
                uiLVAddItem(lvEvents[i], kiosk->events[j]);
                // Reselect the previously selected item here.
                if(kiosk->events[j]->eventid == lastSelection[i])
                    uiLVSelectItem(lvEvents[i], j);
            }

            // Resort the list
            uiLVSortBySelectedColumn(lvEvents[i]);
        }
    }
}

static ToolTip arenaListPlayersToolTip;

static UIBox lvEventsDisplayItem(UIListView* list, PointFloatXYZ rowOrigin, void* userSettings, void* itemData, int itemIndex)
{
    ArenaEvent* item = (ArenaEvent*)itemData;
    UIBox box;
    PointFloatXYZ pen = rowOrigin;
    UIColumnHeaderIterator columnIterator;
    float sc = list->scale;

    box.origin.x = rowOrigin.x;
    box.origin.y = rowOrigin.y;

    uiCHIGetIterator(list->header, &columnIterator);

    rowOrigin.z+=1;

    // Determine the text color of the item.
    if(item == list->selectedItem)
    {
        font_color(CLR_SELECTED_ITEM, CLR_SELECTED_ITEM);
    }
    else
        font_color(filterColors[item->filter], filterColors[item->filter]);


    for(;uiCHIGetNextColumn(&columnIterator, list->scale);)
    {
        UIColumnHeader* column = columnIterator.column;
        UIBox clipBox;

        pen.x = rowOrigin.x + columnIterator.columnStartOffset*sc - PIX3*sc;
         clipBox.origin.x = pen.x-3*sc;
         clipBox.origin.y = pen.y;
        clipBox.height = 20*sc;

         if( columnIterator.columnIndex >= eaSize(&columnIterator.header->columns )-1 )
            clipBox.width = 1000*sc;
        else
             clipBox.width = (columnIterator.currentWidth+R10+PIX3*2)*sc;
        
         clipperPushRestrict( &clipBox );
        if( stricmp( column->name, ARENA_MINE ) == 0 )
        {
            if(item->filter == ARENA_FILTER_MY_EVENTS)
            {
                AtlasTex *star = atlasLoadTexture( "MissionPicker_icon_star.tga" );

                float scale = ((float)20/star->width)*sc;
                display_sprite( star, pen.x + (column->curWidth-20)/2, pen.y+PIX3*sc, rowOrigin.z, scale, scale, CLR_WHITE );
            }
        }
        if(stricmp(column->name, ARENA_EVENT_TYPE) == 0 )
        {
             if ( item->description[0] )
            {
                prnt(pen.x, pen.y+18*sc, rowOrigin.z, sc, sc, "%s", item->description );
            }
            else if( item->sanctioned )
            {
                prnt(pen.x, pen.y+18*sc, rowOrigin.z, sc, sc, textStd(eventNames[item->teamStyle][item->teamType]) );
            }
            else
            {
                prnt(pen.x, pen.y+18*sc, rowOrigin.z, sc, sc, textStd("CustomEvent", eventNames[item->teamStyle][item->teamType]) );
            }
        }
        if(stricmp(column->name, ARENA_PLAYERS) == 0 )
        {
            if (ASAP_EVENT(item) || item->start_time )
            {
                CBox box;
                BuildCBox(&box, pen.x + PIX3*sc + column->curWidth/2 - 15, pen.y+18*sc - 15, 30, 15);
                //drawBox(&box, 0, 0xff0000ff, 0);
                if (mouseCollision(&box))
                {
                    char *text, temp[20];
                    float temp_sc;

                    if (!arenaListPlayersToolTip.smf)
                    {
                        arenaListPlayersToolTip.smf = smfBlock_Create();
                    }
                    
                    estrCreate(&text);
                    estrConcatStaticCharArray(&text, "Current Queued Players: ");
                    itoa(item->numplayers, temp, 10);
                    estrConcatCharString(&text, temp);
                    estrConcatStaticCharArray(&text, "<br>Minimum Required Players: ");
                    itoa(item->minplayers, temp, 10);
                    estrConcatCharString(&text, temp);
                    estrConcatStaticCharArray(&text, "<br>Maximum Allowed Players: ");
                    itoa(item->maxplayers, temp, 10);
                    estrConcatCharString(&text, temp);
                    addToolTip(&arenaListPlayersToolTip);
                    temp_sc = sc;
                    forceDisplayToolTip(&arenaListPlayersToolTip, &box, text, MENU_GAME, WDW_ARENA_CREATE);
                    sc = temp_sc;
                    estrDestroy(&text);
                }

                cprnt(pen.x + PIX3*sc + column->curWidth/2, pen.y+18*sc, rowOrigin.z, sc, sc, "%i/%i+", item->numplayers, item->minplayers );
            }
            else
                cprnt(pen.x + PIX3*sc + column->curWidth/2, pen.y+18*sc, rowOrigin.z, sc, sc, "%i/%i", item->numplayers, item->maxplayers );
        }
        if(stricmp(column->name, ARENA_WEIGHT) == 0 )
        {
             if( !item->weightClass )
                cprnt(pen.x + PIX3*sc + column->curWidth/2, pen.y+18*sc, rowOrigin.z, sc, sc, "AnyString" );
            else
                cprnt(pen.x + PIX3*sc + column->curWidth/2, pen.y+18*sc, rowOrigin.z, sc, sc, "%i-%i", g_weightRanges[item->weightClass].minLevel+1, g_weightRanges[item->weightClass].maxLevel+1 );
        }
        if(stricmp(column->name, ARENA_CREATOR) == 0 )
        {
            if( item->creatorname[0] )
                cprntEx(pen.x, pen.y+18*sc, rowOrigin.z, sc, sc, NO_MSPRINT, item->creatorname );
            else
                prnt(pen.x, pen.y+18*sc, rowOrigin.z, sc, sc, "%i", item->creatorid );
        }
        if(stricmp(column->name, ARENA_FEE) == 0 )
        {
            cprnt(pen.x + PIX3*sc + column->curWidth/2, pen.y+18*sc, rowOrigin.z, sc, sc, "%i", item->entryfee );
        }
        if(stricmp(column->name, ARENA_START) == 0 )
        {
            if (ASAP_EVENT(item))
                prnt(pen.x, pen.y+18*sc, rowOrigin.z, sc, sc, "ASAPString" );
            else if (INSTANT_EVENT(item))
                prnt(pen.x, pen.y+18*sc, rowOrigin.z, sc, sc, "InstantString" );
            else if( item->start_time )
                prnt(pen.x, pen.y+18*sc, rowOrigin.z, sc, sc, "%s", timerMakeOffsetDateStringNoYearNoSecFromSecondsSince2000(item->start_time) );
            else
                prnt(pen.x, pen.y+18*sc, rowOrigin.z, sc, sc, "InstantString" );
        }
        if(stricmp(column->name, ARENA_STATUS) == 0 )
        {
            if( item->phase >= 0 && item->phase < ARRAY_SIZE(g_PhaseTranslationKeys) )
                prnt(pen.x, pen.y+18*sc, rowOrigin.z, sc, sc, g_PhaseTranslationKeys[item->phase] );
        }
        clipperPop( &clipBox );
    }

    return box;
}

//------------------------------------------------------------------------------------------------------
// Main Window /////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------
static uiTabControl * tc = 0;

int arenaListGetSelectedTab() {
    return uiTabControlGetSelectedIdx(tc);
}

int arenaListGetSelectedFilter() 
{
    return arenaFilter;
}

void arenaListSelectTab( int tab )
{
    uiTabControlSelect(tc, lvEvents[tab] );
    requestArenaKiosk(arenaListGetSelectedFilter(), arenaListGetSelectedTab());
}

static void arenaTabSelectedCallback(uiTabData * td) {
    requestArenaKiosk(arenaListGetSelectedFilter(), arenaListGetSelectedTab());
}

void requestCurrentKiosk()
{
    requestArenaKiosk(arenaListGetSelectedFilter(), uiTabControlGetSelectedIdx(tc));
}

#define BUTTON_OFFSET 25

int arenaListWindow(void)
{
    float x, y, z, wd, ht, sc, scale;
    int i, color, bcolor, fullDrawHeight, currentHeight;

    static ScrollBar sb = { WDW_ARENA_LIST };
    static int init = false;
    AtlasTex *round;

      if( !lvEvents[0] ) 
    {
        for( i = 0; i < ARENA_TAB_TOTAL; i++ )
            arenaListInit( lvEvents[i] );
    }

    if(!tc)
    {
        tc = uiTabControlCreate(TabType_Undraggable, arenaTabSelectedCallback, 0, 0, 0, 0);
        uiTabControlAdd(tc, textStd("AllTab"),        (void*)lvEvents[ARENA_TAB_ALL] );
        uiTabControlAdd(tc, textStd("SoloTab"),        (void*)lvEvents[ARENA_TAB_SOLO] );
        uiTabControlAdd(tc, textStd("ArenaTeamTab"),(void*)lvEvents[ARENA_TAB_TEAM] );
        uiTabControlAdd(tc, textStd("SgTab"),        (void*)lvEvents[ARENA_TAB_SG] );
        uiTabControlAdd(tc, textStd("TournyTab"),    (void*)lvEvents[ARENA_TAB_TOURNY] );
    }

    // Do everything common windows are supposed to do.
     if ( !window_getDims( WDW_ARENA_LIST, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ))
        return 0;


     if( !init )
    {
        comboboxTitle_init( &comboFilter, winDefs[WDW_ARENA_LIST].loc.wd - 175 - R10/2, 6, 20, 175, 20, 400, WDW_ARENA_LIST);
        comboboxTitle_add( &comboFilter, 0, NULL, "ShowBestFit",    "ShowBestFit",    ARENA_FILTER_BEST_FIT,    filterColors[ARENA_FILTER_BEST_FIT],0,0    );
        comboboxTitle_add( &comboFilter, 0, NULL, "ShowEligable",    "ShowEligable", ARENA_FILTER_ELIGABLE,    filterColors[ARENA_FILTER_ELIGABLE],0,0    );
        comboboxTitle_add( &comboFilter, 0, NULL, "ShowOngoing",    "ShowOngoing",    ARENA_FILTER_ONGOING,    filterColors[ARENA_FILTER_ONGOING],0,0    );
        comboboxTitle_add( &comboFilter, 0, NULL, "ShowComplete",    "ShowComplete",    ARENA_FILTER_COMPLETED,    filterColors[ARENA_FILTER_COMPLETED],0,0);
        comboboxTitle_add( &comboFilter, 0, NULL, "ShowScheduled",    "ShowScheduled",ARENA_FILTER_MY_EVENTS,    filterColors[ARENA_FILTER_MY_EVENTS],0,0);
        comboboxTitle_add( &comboFilter, 0, NULL, "ShowPlayer",        "ShowPlayer",    ARENA_FILTER_PLAYER,    filterColors[ARENA_FILTER_PLAYER],0,0);
        comboboxTitle_add( &comboFilter, 0, NULL, "ShowAll",        "ShowAll",        ARENA_FILTER_ALL,        filterColors[ARENA_FILTER_ALL],0,0        );
        combobox_setId(&comboFilter, arenaFilter, 1);
        init = true;
    }

    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );
       drawFrame( PIX3, R10, x, y+ht-BUTTON_OFFSET*2*sc, z, wd, BUTTON_OFFSET*2*sc, sc, color, 0 );
 
    lvCurrent = drawTabControl(tc, x+R10*sc, y, z+10, wd-(2*R10*sc), 0, sc, color, color, TabDirection_Horizontal);

    //display the list of events
     if(lvCurrent)
    {
        PointFloatXYZ pen;
        UIBox listViewDrawArea;
        ArenaEvent * event = NULL;
        int filter;

        pen.x = x;
        pen.y = y + 30*sc;
        pen.z = z;

        listViewDrawArea.x = x;
          listViewDrawArea.y = y+30*sc;
        listViewDrawArea.width = wd;
           listViewDrawArea.height = ht-(30+BUTTON_OFFSET*2)*sc;

        // How tall and wide is the list view supposed to be?
        uiLVSetDrawColor(lvCurrent, window_GetColor(WDW_ARENA_LIST));
        uiLVSetHeight(lvCurrent, listViewDrawArea.height);
        uiLVHSetWidth(lvCurrent->header, listViewDrawArea.width);
        uiLVSetScale( lvCurrent, sc );

        clipperPushRestrict(&listViewDrawArea);
        uiLVDisplay(lvCurrent, pen);
        clipperPop();
 
        // filter
        combobox_setId(&comboFilter, arenaFilter, 1);
        combobox_display( &comboFilter );

        if( comboFilter.changed )
        {
            arenaFilter = combobox_GetSelected(&comboFilter);
            requestArenaKiosk(arenaListGetSelectedFilter(), uiTabControlGetSelectedIdx(tc));
        }

        // Handle list view input.
        uiLVHandleInput(lvCurrent, pen);

        // scroll bar
         fullDrawHeight = uiLVGetFullDrawHeight(lvCurrent);
        currentHeight = uiLVGetHeight(lvCurrent);

        if(fullDrawHeight > currentHeight) 
        {
               doScrollBar(&sb, listViewDrawArea.height - 15*sc, fullDrawHeight, wd, (30+PIX3+R10)*sc, z+2, 0, &listViewDrawArea);
            lvCurrent->scrollOffset = sb.offset;
        }
        else
            lvCurrent->scrollOffset = 0;

        // Personal Stats            
           if( D_MOUSEHIT == drawStdButton( x + wd*9/40, y + ht - BUTTON_OFFSET*sc, z, 80*sc, 30*sc, color, "MyStats", 1.f, 0 ) )
        {
            cmdParse( "getarenastats" );
        }
 
        // Manage Gladiators  //To Do where is the best place for this to go? 
        if( D_MOUSEHIT == drawStdButton( x + wd*3/40, y + ht - BUTTON_OFFSET*sc, z, 80*sc, 30*sc, color, "ToGladiatorManager", 1.f, 0 ) )
        {
            //TO DO -- check on anti-spamming tricks
            if( !game_state.pickPets )
            {
                game_state.inPetsFromArenaList = 1;
                window_setMode( WDW_ARENA_LIST, WINDOW_SHRINKING );
                arena_ManagePets();
            }
            else
            {
                game_state.pickPets = PICKPET_WAITINGFORACKTOCLOSE;
                arena_CommitPetChanges();
            }
        }

        // refresh button
         if( D_MOUSEHIT == drawStdButton( x + wd*2/5, y + ht - BUTTON_OFFSET*sc, z, 120*sc, 30*sc, color, "RefreshString", 1.f, 0 ) )
        {
            requestArenaKiosk(arenaListGetSelectedFilter(), uiTabControlGetSelectedIdx(tc));
        }

        // create button
          if( D_MOUSEHIT == drawStdButton( x + wd*3/5, y + ht - BUTTON_OFFSET*sc, z, 120*sc, 30*sc, color, "CreateEvent", 1.f, 0 ) )
        {
            arenaSendCreateRequest();
        }


        if( lvCurrent->selectedItem )
            event = (ArenaEvent*)lvCurrent->selectedItem; 

        if( event )
        {
            filter = event->filter;
            lastSelection[arenaListGetSelectedTab()] = event->eventid;
        }
        else
            filter = ARENA_FILTER_INELIGABLE;

        round = atlasLoadTexture("MissionPicker_icon_round.tga");
        scale = ((float)30/round->width)*sc;
         cprntEx( x+wd+15*sc - (30+R10)*sc, y+ht+15*sc - (30+R10)*sc, z+1, 1.f, 1.f, CENTER_X|CENTER_Y, "?" );
        if( D_MOUSEHIT == drawGenericButton( round, x+wd - (30+R10)*sc, y+ht - (30+R10)*sc, z, scale, color, color, true ) )
        {
            helpSet(9);
        }

        // join/observe button
          switch( filter )
        {
            case ARENA_FILTER_ELIGABLE:
            case ARENA_FILTER_BEST_FIT:
            {
                 if( D_MOUSEHIT == drawStdButton( x + wd - wd/5, y + ht - BUTTON_OFFSET*sc, z, 120*sc, 30*sc, color, "JoinEvent", 1.f, 0 ) )
                {
                    arenaSendJoinRequest( event->eventid, event->uniqueid, false );
                    requestCurrentKiosk();
                }
            }break;
            case ARENA_FILTER_ONGOING:
            {
                if( event->tournamentType == ARENA_TOURNAMENT_SWISSDRAW )
                {
                    if( D_MOUSEHIT == drawStdButton( x + wd - wd/5, y + ht - BUTTON_OFFSET*sc, z, 120*sc, 30*sc, color, "ViewEventResults", 1.f, 0 ) )
                        arenaSendViewResultRequest( event->eventid, event->uniqueid );
                }
                else
                {
                    if( D_MOUSEHIT == drawStdButton( x + wd - wd/5, y + ht - BUTTON_OFFSET*sc, z, 120*sc, 30*sc, color, "ObserveEvent", 1.f, event->no_observe ) )
                        arenaSendObserveRequest( event->eventid, event->uniqueid  );
                }
            }break;
            case ARENA_FILTER_COMPLETED:
            {
                if( D_MOUSEHIT == drawStdButton( x + wd - wd/5, y + ht - BUTTON_OFFSET*sc, z, 120*sc, 30*sc, color, "ViewEventResults", 1.f, 0 ) )
                    arenaSendViewResultRequest( event->eventid, event->uniqueid );
            }break;
            case ARENA_FILTER_MY_EVENTS:
            {
                if( D_MOUSEHIT == drawStdButton( x + wd - wd/5, y + ht - BUTTON_OFFSET*sc, z, 120*sc, 30*sc, color, "DropEvent", 1.f, 0 ) )
                {
                    arenaSendPlayerDrop( event, playerPtr()->db_id );
                    requestCurrentKiosk();
                }
            }break;
            default:
            {
                drawStdButton( x + wd - wd/5, y + ht - BUTTON_OFFSET*sc, z, 110*sc, 30*sc, color, "JoinEvent", 1.f, 1 );
            }break;
         }
    }

    return 0;
}
