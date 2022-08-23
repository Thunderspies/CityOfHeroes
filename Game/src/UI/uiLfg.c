
#include <utilitieslib/stdtypes.h>
#include "UI/uiLfg.h"
#include "UI/uiWindows.h"
#include <utilitieslib/components/earray.h>
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "string.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/textureatlas.h"
#include "UI/uiAutomap.h"
#include "UI/uiReticle.h"
#include "uiListView.h"
#include "UI/uiClipper.h"
#include "graphics/ttFontUtil.h"
#include "entity/entity.h"
#include "player/player.h"
#include "graphics/ttFont.h"
#include "UI/uiInput.h"
#include "UI/uiChat.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiComboBox.h"
#include "UIEdit.h"
#include "UI/uiCursor.h"
#include "UI/uiTarget.h"
#include "UI/uiUtil.h"
#include "UI/uiToolTip.h"
#include "win/input.h"
#include <utilitieslib/utils/utils.h>
#include "entity/classes.h"
#include "entity/origins.h"
#include "UI/uiGroupWindow.h"
#include "UI/uiGame.h"
#include "uiEmote.h"
#include "filter/profanity.h"
#include "UI/uiNet.h"
#include "entity/entPlayer.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/uiDialog.h"
#include "comm_game.h"
#include "UI/uiOptions.h"

static UIListView* lfgListView;
static UIEdit *editTxt;
static int lfgListRebuildRequired = 0;
static int isLfg = 1;

static UIListView * lfg_initListView();



#define LFG_FOOTER        40

static ComboBox comboArchetype    = {0};
static ComboBox comboOrigin        = {0};
static ComboBox comboMap        = {0};
static ComboBox comboLevel        = {0};
static ComboBox comboLfg        = {0};

//---------------------------------------------------------------------------------------
// List management
//---------------------------------------------------------------------------------------

typedef struct Lfg
{
    char name[32];
    char mapName[128];
    char comment[128];
    const char *originName;
    const char *className;

    AtlasTex * classIcon;
    AtlasTex * originIcon;

    int level;
    int anon;
    int lfg;
    int hidden;
    int dbid;
    int teamsize;
    int leader;
    int sameteam;
    int leaguesize;
    int leagueleader;
    int sameleague;
    int arena_map;
    int mission_map;
    int other_faction;
    int faction_same_map;
    int other_universe;
    int universe_same_map;

    ToolTip originTip;
    ToolTip classTip;
    ToolTip commentTip;
    ToolTip lfgTip;
    ToolTipParent parentTip;

} Lfg;

Lfg **lfgList = 0;
static int gResults = 0;
static int gTruncated = 0;


void lfg_setResults( int results, int actual )
{
    gResults = results;
    
    if( results > actual )
        gTruncated = 1;
    else
        gTruncated = 0;
}

void lfg_clearAll()
{
    lfgListRebuildRequired = 1;
    // FIXEAR: Optimize
    while( eaSize(&lfgList) )
    {
        Lfg * tmp = eaRemove(&lfgList, 0);
        if( tmp )
        {
            freeToolTip( &tmp->classTip );
            freeToolTip( &tmp->originTip );
            freeToolTip( &tmp->commentTip );
            freeToolTip( &tmp->lfgTip );
            memset( tmp, 0, sizeof(Lfg) );
            free( tmp );
        }
    }

    gResults = 0;
    gTruncated = 0;
}
 
void lfg_add( char * name, int archetype, int origin, int level, int lfg, int hidden, int teamsize, int leader, int sameteam, 
             int leaguesize, int leagueleader, int sameleague, char *mapName, int dbid, int arena_map, int mission_map, 
             int other_faction, int faction_same_map, int other_universe, int universe_same_map, char *comment )
{
    Lfg * tmp;

    if( !lfgList )
        eaCreate( &lfgList );

    if (archetype < 0 || archetype >= eaSize(&g_CharacterClasses.ppClasses))
        return;
    if (origin < 0 || origin >= eaSize(&g_CharacterOrigins.ppOrigins))
        return;


    tmp = calloc( 1, sizeof(Lfg) );

    strncpyt( tmp->name, name, 32 );
    strncpyt( tmp->mapName, mapName, 128 );
    strncpyt( tmp->comment, comment, 128 );

    tmp->classIcon = uiGetArchetypeIconByName( g_CharacterClasses.ppClasses[archetype]->pchName );
    tmp->originIcon = uiGetOriginTypeIconByName( g_CharacterOrigins.ppOrigins[origin]->pchName);
    tmp->className = g_CharacterClasses.ppClasses[archetype]->pchDisplayName;
    tmp->originName = g_CharacterOrigins.ppOrigins[origin]->pchDisplayName;

    tmp->level = level;
    tmp->lfg = lfg;
    tmp->hidden = hidden;
    tmp->dbid = dbid;
    tmp->teamsize = teamsize;
    tmp->leader = leader;
    tmp->sameteam = sameteam;
    tmp->leaguesize = leaguesize;
    tmp->leagueleader = leagueleader;
    tmp->sameleague = sameleague;
    tmp->arena_map = arena_map;
    tmp->mission_map = mission_map;
    tmp->other_faction = other_faction;
    tmp->faction_same_map = faction_same_map;
    tmp->other_universe = other_universe;
    tmp->universe_same_map = universe_same_map;

    eaPush(&lfgList, tmp);
}

//---------------------------------------------------------------------------------------
// Display code
//---------------------------------------------------------------------------------------

int lfgClassCompare(const Lfg** lfg1, const Lfg** lfg2)
{
    const Lfg* p1 = *lfg1;
    const Lfg* p2 = *lfg2;

    if( p1->classIcon == p2->classIcon )
        return stricmp( p1->name, p2->name );
    else
        return stricmp( p1->classIcon->name, p2->classIcon->name );
}

int lfgOriginCompare(const Lfg** lfg1, const Lfg** lfg2)
{
    const Lfg* p1 = *lfg1;
    const Lfg* p2 = *lfg2;

    if( p1->originIcon == p2->originIcon )
        return stricmp( p1->name, p2->name );
    else
        return stricmp( p1->originIcon->name, p2->originIcon->name );
}

int lfgLvlCompare(const Lfg** lfg1, const Lfg** lfg2)
{
    const Lfg* p1 = *lfg1;
    const Lfg* p2 = *lfg2;

    if(p1->level > p2->level)
        return -1;
    else if(p1->level < p2->level)
        return 1;
    else
        return stricmp( p1->name, p2->name );
}

int lfgLfgCompare(const Lfg** lfg1, const Lfg** lfg2)
{
    const Lfg* p1 = *lfg1;
    const Lfg* p2 = *lfg2;

    if(p1->lfg > p2->lfg)
        return -1;
    else if(p1->lfg < p2->lfg)
        return 1;
    else
        return stricmp( p1->name, p2->name );
}

int lfgMapCompare(const Lfg** lfg1, const Lfg** lfg2)
{
    const Lfg* p1 = *lfg1;
    const Lfg* p2 = *lfg2;

    return stricmp( p1->mapName, p2->mapName );
}

int lfgCommentCompare(const Lfg** lfg1, const Lfg** lfg2)
{
    const Lfg* p1 = *lfg1;
    const Lfg* p2 = *lfg2;

    return stricmp( p1->comment, p2->comment );
}


int lfgNameCompare(const Lfg** lfg1, const Lfg** lfg2)
{
    const Lfg* p1 = *lfg1;
    const Lfg* p2 = *lfg2;

    return stricmp( p1->name, p2->name );
}

#define ICON_SCALE .65f
static int showLFGmode = WDW_GROUP;
void lfg_setSearchMethod(int wdwType)
{
    showLFGmode = wdwType;
}

UIBox lfgListViewDisplayItem(UIListView* list, PointFloatXYZ rowOrigin, void* userSettings, Lfg* lfg, int itemIndex)
{
    UIBox box;
      PointFloatXYZ pen = rowOrigin;
    UIColumnHeaderIterator columnIterator;
    int selected = (lfg == list->selectedItem ? 1 : 0);
    int sameteam;
    int teamcolor;
    int teamleader;
    int busy = (lfg->arena_map || lfg->mission_map);
    int mustvisit = ((lfg->other_faction && lfg->faction_same_map) || (lfg->other_universe && lfg->universe_same_map));
    int nolfg = (lfg->lfg&LFG_NOGROUP);
    float x, y, z, wd, ht, sc;
    int color, back;
    
    switch (showLFGmode)
    {
    case WDW_GROUP:
        sameteam = (playerPtr()->teamup && lfg->sameteam && lfg->teamsize > 1);
        teamcolor = (lfg->teamsize > 1 && !lfg->lfg);
        teamleader = (lfg->teamsize > 1 && lfg->leader && lfg->teamsize < MAX_TEAM_MEMBERS);
        break;
    case WDW_LEAGUE:
        sameteam = (playerPtr()->league && lfg->sameleague && lfg->leaguesize > 1);
        teamcolor = (lfg->leaguesize > 1 && !lfg->lfg);
        teamleader = (lfg->leaguesize > 1 && lfg->leagueleader && lfg->leaguesize < MAX_LEAGUE_MEMBERS);
        break;
    }
    window_getDims( WDW_LFG, &x, &y, &z, &wd, &ht, &sc, &color, &back );

    box.origin.x = rowOrigin.x;
    box.origin.y = rowOrigin.y;

    uiCHIGetIterator(list->header, &columnIterator);

    rowOrigin.z+=1;

    BuildCBox( &lfg->parentTip.box, x, rowOrigin.y, wd, list->rowHeight*sc );

    if( sameteam )
        color = selected ? CLR_TXT_GREEN : 0x88aa88ff;
    else if( nolfg )
        color = selected ? CLR_RED : 0xaa8888ff;
    else if( busy )
        color = selected ? CLR_YELLOW : 0xaaaa00ff;
    else if( mustvisit )
        color = selected ? 0xff8800ff : 0xaa6600ff;
    else if( teamleader )
        color = selected ? 0xaa00ffff : 0x8800aaff;
    else if( teamcolor )
        color = selected ? 0xaaaaffff : 0x8888aaff;
    else
        color = selected ? CLR_WHITE : CLR_CONSTRUCT(99, 225, 252, 255);

    font_color(color, color);

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
        //if( columnIterator.columnIndex >= eaSize(&columnIterator.header->columns )-1 )
        //    clipBox.width = 1000*sc;
        //else
            clipBox.width = (columnIterator.currentWidth+5)*sc;

          BuildCBox( &cbox, pen.x, pen.y, (columnIterator.currentWidth+5)*sc, 20*sc );

        // Decide what to draw on on which column.
          if(stricmp(column->name, LFG_LFG_COLUMN) == 0)
        {
            SearchOption * so = lfg_getSearchOption( lfg->lfg );    
            AtlasTex * lfgIcon = atlasLoadTexture( so->texName );
            CBox color_box;

            BuildCBox( &color_box, rowOrigin.x, rowOrigin.y+1*sc, list->header->width, 22*sc );

            display_sprite( lfgIcon, pen.x + (columnIterator.currentWidth-lfgIcon->width*ICON_SCALE)*sc/2,
                pen.y + (26-lfgIcon->height*ICON_SCALE)*sc/2, pen.z, ICON_SCALE*sc, ICON_SCALE*sc, so->icon_color?so->icon_color:CLR_WHITE );

            drawBox( &color_box, pen.z-1, 0, (so->color&0xffffff00)|0x00000044 );
            if (pen.y <= y+ht - 30 && pen.y >= y+70) {
                addToolTip( &lfg->lfgTip );
                setToolTipEx( &lfg->lfgTip, &cbox, so->displayName, &lfg->parentTip, MENU_GAME, WDW_LFG, 1, 0 );
            }
        }
        else if(stricmp(column->name, LFG_CLASS_COLUMN) == 0)
        {
            display_sprite( lfg->classIcon, pen.x + (columnIterator.currentWidth-lfg->classIcon->width*ICON_SCALE)*sc/2,
                                   pen.y + (26-lfg->classIcon->height*ICON_SCALE)*sc/2, pen.z, ICON_SCALE*sc, ICON_SCALE*sc, CLR_WHITE );
            if (pen.y <= y+ht - 30 && pen.y >= y+70) {
                addToolTip( &lfg->classTip );
                setToolTipEx( &lfg->classTip, &cbox, lfg->className, &lfg->parentTip, MENU_GAME, WDW_LFG, 1, 0 );
            }
        }
        else if(stricmp(column->name, LFG_ORIGIN_COLUMN) == 0)
        {
            display_sprite( lfg->originIcon, pen.x + (columnIterator.currentWidth-lfg->originIcon->width*ICON_SCALE)*sc/2,
                 pen.y + (26-lfg->originIcon->height*ICON_SCALE)*sc/2, pen.z, ICON_SCALE*sc, ICON_SCALE*sc, CLR_WHITE );
            if (pen.y <= y+ht - 30 && pen.y >= y+70) {
                addToolTip( &lfg->originTip );
                setToolTipEx( &lfg->originTip, &cbox, lfg->originName, &lfg->parentTip, MENU_GAME, WDW_LFG, 1, 0 );
            }
        }
        else if(stricmp(column->name, LFG_LVL_COLUMN) == 0)
        {
              cprntEx( pen.x + columnIterator.currentWidth*sc/2, pen.y + 18*sc, pen.z, sc, sc, (CENTER_X), "%d", lfg->level+1 );
        }
         else if(stricmp(column->name, LFG_NAME_COLUMN) == 0)
        {
            clipperPushRestrict( &clipBox );
             cprntEx( pen.x, pen.y + 18*sc, pen.z, sc, sc, NO_MSPRINT, lfg->name );
            clipperPop();
        }
        else if(stricmp(column->name, LFG_MAP_COLUMN) == 0)
        {
            clipperPushRestrict( &clipBox );
            cprntEx( pen.x, pen.y + 18*sc, pen.z, sc, sc, 0, lfg->mapName );
            clipperPop();
        }
         else if( stricmp(column->name, LFG_COMMENT_COLUMN) == 0 )
        {
             if( !optionGet(kUO_AllowProfanity)  )
                ReplaceAnyWordProfane(lfg->comment);
             cbox.hx = x + wd - PIX3*sc;
             cprntEx( pen.x, pen.y + 18*sc, pen.z, sc, sc, 0, "DontTranslate", lfg->comment );
            if (pen.y <= y+ht - 30 && pen.y >= y+70) {
                addToolTip( &lfg->commentTip );
                setToolTipEx( &lfg->commentTip, &cbox, lfg->comment, &lfg->parentTip, MENU_GAME, WDW_LFG, 1, TT_NOTRANSLATE );
            }
        }
    }

     box.height = 0;
    box.width = list->header->width;

    // Returning the bounding box of the item does not do anything right now.
    // See uiListView.c for more details.
    return box;
}

int lfg_TargetIsInList(void)
{
    Entity *e = playerPtr();
    int i, id;

    id = targetGetDBID();
    if(!id)
        return FALSE;

    for( i = 0; i < eaSizeUnsafe(&lfgListView->items); i++ )
    {
        Lfg* item = lfgListView->items[i];
        if( id == item->dbid )
            return TRUE;
    }

    return FALSE;
}

static UIListView * lfg_initListView()
{
    UIListView * lfgListView;
    Wdw* lfgWindow = wdwGetWindow(WDW_FRIENDS);

    lfgListView = uiLVCreate();

    uiLVHAddColumn(lfgListView->header,  LFG_LFG_COLUMN, "", 10);
    uiLVBindCompareFunction(lfgListView, LFG_LFG_COLUMN, lfgLfgCompare);

    uiLVHAddColumn(lfgListView->header,  LFG_LVL_COLUMN, "", 10);
    uiLVBindCompareFunction(lfgListView, LFG_LVL_COLUMN, lfgLvlCompare);

    uiLVHAddColumn(lfgListView->header,  LFG_CLASS_COLUMN, "", 10);
    uiLVBindCompareFunction(lfgListView, LFG_CLASS_COLUMN, lfgClassCompare);

    uiLVHAddColumn(lfgListView->header,  LFG_ORIGIN_COLUMN, "", 10);
    uiLVBindCompareFunction(lfgListView, LFG_ORIGIN_COLUMN, lfgOriginCompare);

    uiLVHAddColumnEx(lfgListView->header, LFG_NAME_COLUMN, "NameString", 20, 270, 1);
    uiLVBindCompareFunction(lfgListView, LFG_NAME_COLUMN, lfgNameCompare);
    
    uiLVHAddColumnEx(lfgListView->header, LFG_MAP_COLUMN, "MapString", 20, 270, 1 );
    uiLVBindCompareFunction(lfgListView, LFG_MAP_COLUMN, lfgMapCompare);

    uiLVHAddColumn(lfgListView->header, LFG_COMMENT_COLUMN, "CommentString", 0);
    uiLVBindCompareFunction(lfgListView, LFG_COMMENT_COLUMN, lfgCommentCompare);

    uiLVFinalizeSettings(lfgListView);

    lfgListView->displayItem = lfgListViewDisplayItem;

    uiLVEnableMouseOver(lfgListView, 1);
    uiLVEnableSelection(lfgListView, 1);

    {
        // The window needs to be wide enough to fit the following.
        // - The list view header
        // - The window borders
        int minWindowWidth = (int)uiLVHGetMinDrawWidth(lfgListView->header) + 2*PIX3;

        // The window needs to be tall enough to fit the following:
        // - The list view, with header and sort arrow
        // - Two items in the list view
        // - The remove button
        // - The window borders
        int minWindowHeight = (int)uiLVGetMinDrawHeight(lfgListView) + 2*lfgListView->rowHeight + LFG_FOOTER + 2*PIX3;

        // Limit the window to the min header size.
        lfgWindow->min_wd = max(lfgWindow->min_wd, minWindowWidth);
        lfgWindow->min_ht = max(lfgWindow->min_ht, minWindowHeight);
    }

    return lfgListView;
}

void lfg_initComboboxes()
{
    int i;
    char tmp[4] = {0};

    comboboxClear(&comboArchetype);
     combocheckbox_init( &comboArchetype,    10,  10, 20, 100, 100, 20, 140, "Archetype...",    WDW_LFG, 0, 1 );
    comboboxClear(&comboOrigin);
    combocheckbox_init( &comboOrigin,        110, 10, 20, 100, 100, 20, 110, "Origin...",    WDW_LFG, 0, 1 );
    comboboxClear(&comboMap);
       combocheckbox_init( &comboMap,            210, 10, 20, 100, 170, 20, 400, "Map...",        WDW_LFG, 0, 1 );
    comboboxClear(&comboLevel);
      combocheckbox_init( &comboLevel,        310, 10, 20, 80, 100, 20, 200, "Level...",        WDW_LFG, 0, 1 );
    comboboxClear(&comboLfg);
      combocheckbox_init( &comboLfg,            210, 40,  7,  58, 200, 20, 140, "LF...",        WDW_LFG, 0, 1 );

    for( i = eaSize(&g_CharacterClasses.ppClasses)-1; i >= 0; i-- )
    {
        strncpyt( tmp, textStd(g_CharacterClasses.ppClasses[i]->pchDisplayName), 4 );
        if (stricmp(g_CharacterClasses.ppClasses[i]->pchName, "Class_Arachnos_Soldier") == 0) // Spider
        {
            strncpyt( tmp, textStd("ArachnosSpider"), 4 );
        }
        if (stricmp(g_CharacterClasses.ppClasses[i]->pchName, "Class_Arachnos_Widow") == 0)
        {
            strncpyt( tmp, textStd("ArachnosWidow"), 4 );
        }

        combocheckbox_add( &comboArchetype, 0, uiGetArchetypeIconByName( g_CharacterClasses.ppClasses[i]->pchName ), tmp, i );
    }

    for( i = eaSize(&g_CharacterOrigins.ppOrigins)-1; i >= 0; i-- )
    {
        strncpyt( tmp, textStd(g_CharacterOrigins.ppOrigins[i]->pchDisplayName), 4 );
        combocheckbox_add( &comboOrigin, 0, uiGetOriginTypeIconByName( g_CharacterOrigins.ppOrigins[i]->pchName), tmp, i );
    }
    // yeack
    addMapNames( &comboMap );
    addLFG( &comboLfg );

       for( i = 1; i <= 45; i += 5 )
    {
        char levels[32];
        sprintf( levels, "%d-%d", i, i+9 );
        combocheckbox_add( &comboLevel, 0, NULL, levels, 0 );
    }

}

void lfg_buildSearch(Packet *pak)
{
    int i, mask = 0;

    // name
    if( uiEditGetUTF8Text(editTxt) )
    {
        pktSendBits(pak,1,1);
        pktSendString(pak, uiEditGetUTF8Text(editTxt) );
    
    }
    else
        pktSendBits(pak,1,0);

    // archetype
    if( !comboArchetype.elements[0]->selected )
    {
        for( i = eaSize(&comboArchetype.elements)-1; i > 0; i-- )
        {
            if( comboArchetype.elements[i]->selected )
                pktSendBitsPack(pak,1,comboArchetype.elements[i]->id);
        }
        pktSendBitsPack(pak,1,-1);
    }
    else
        pktSendBitsPack(pak,1,-1);

    // origin
    if( !comboOrigin.elements[0]->selected )
    {
        for( i = eaSize(&comboOrigin.elements)-1; i > 0; i-- )
        {
            if( comboOrigin.elements[i]->selected )
                pktSendBitsPack(pak,1,comboOrigin.elements[i]->id);
        }
        pktSendBitsPack(pak,1,-1);
    }
    else
        pktSendBitsPack(pak,1,-1);

    // maps
     if( !comboMap.elements[0]->selected )
    {
        pktSendBits(pak,1,1);
        for( i = eaSize(&comboMap.elements)-1; i > 0; i-- )
        {
            if( comboMap.elements[i]->selected )
            {
                pktSendBits(pak,1,1);
                pktSendString(pak, comboMap.elements[i]->txt);
            }
        }
        pktSendBits(pak,1,0);
    }
    else
        pktSendBits(pak,1,0); // all

    //levels
    if( !comboLevel.elements[0]->selected )
    {
        int min = -1;
        int max = -1;

        for( i = eaSize(&comboLevel.elements)-1; i > 0; i-- )
        {
            if( comboLevel.elements[i]->selected )
            {
                char tmp[32];
                char *token;
                strncpyt( tmp, comboLevel.elements[i]->txt, 32 );

                token = strtok( tmp, "-" );
                if( min < 0 )
                    min = atoi(token);
                else
                    min = MIN( min, atoi(token) );
            
                token = strtok( NULL, "-" );
                if( max < 0 )
                    max = atoi(token);
                else
                    max = MAX( max, atoi(token) );
            }
        }

        if( min >= 0 && max >= 0 )
        {
            pktSendBits(pak,1,1);
            pktSendBitsPack(pak,1,min);
            pktSendBitsPack(pak,1,max);
        }
        else
            pktSendBits(pak,1,0);
    }
    else
        pktSendBits(pak,1,0);


    // lfg
    if( !comboLfg.elements[0]->selected )
    {
        for( i = eaSize(&comboLfg.elements)-1; i > 0; i-- )
        {
            if( comboLfg.elements[i]->selected )
                mask |= comboLfg.elements[i]->id;
        }
    }
    pktSendBitsPack(pak,1,mask);

}

static void lfgEditInputHandler(UIEdit* edit)
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
                    sendSearchRequest();
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
static void drawLeagueModeButton(float x, float y, float z, float wd, float height, float scale, int color)
{
    CBox box;
    int leagueStrWd = str_wd(&game_12,scale, scale, textStd("LeagueModeSearchString"));
    float txt_sc;
    font(&game_12);
    BuildCBox(&box, x, y, wd, height);
    if(showLFGmode == WDW_LEAGUE)
    {
        AtlasTex *sprite = atlasLoadTexture( "Context_checkbox_filled.tga" );
        display_sprite( sprite, x+3*scale, y+10*scale-sprite->height*scale/2, z+4, scale, scale, (color & 0xffffff00)|0xff  );

        sprite = atlasLoadTexture( "Context_checkbox_empty.tga" );
        display_sprite( sprite, x+3*scale, y+10*scale-sprite->height*scale/2, z+3, scale, scale, (color & 0xffffff00)|0xff );
    }
    else
    {
        AtlasTex *sprite = atlasLoadTexture( "Context_checkbox_empty.tga" );
        display_sprite( sprite, x+3*scale, y+10*scale-sprite->height*scale/2, z+3, scale, scale, color | 0xff );
    }
    if (mouseCollision(&box))
    {
        font_color(CLR_GREY, CLR_GREY);
        if (mouseClickHit(&box, MS_LEFT))
        {
            showLFGmode = (showLFGmode == WDW_LEAGUE) ? WDW_GROUP : WDW_LEAGUE;
        }
    }
    txt_sc  = (leagueStrWd > wd-(20*scale)) ? (wd-(20*scale))/leagueStrWd : 1.f;
    cprntEx( x+wd-((leagueStrWd+10)*txt_sc)/2 +5*scale, y+10*scale, z+10, txt_sc*scale, txt_sc*scale, CENTER_X | CENTER_Y, "LeagueModeSearchString" );
}
//---------------------------------------------------------------------------------------
// Main Window
//---------------------------------------------------------------------------------------

int lfgWindow()
{
    Entity *e = playerPtr();
    float x, y, z, wd, ht, scale;
    int color, back_color;
    static int init;

    PointFloatXYZ pen;
    UIBox windowDrawArea;
    UIBox listViewDrawArea;
    CBox box;

    static ScrollBar sb = {WDW_LFG,0};

     if( !window_getDims( WDW_LFG, &x, &y, &z, &wd, &ht, &scale, &color, &back_color) )
    {    
        init = false;
        uiEditReturnFocus(editTxt);
         if( lfgListView && lfgListView->selectedItem )
            lfgListView->selectedItem = NULL;
        return 0;
    }

       if( !init )
    {
         init = true;
        lfg_initComboboxes();

        if(editTxt)
            uiEditClear(editTxt);
        else
        {
            editTxt = uiEditCreateSimple(NULL, MAX_PLAYER_NAME_LEN(e), &game_12, CLR_WHITE, lfgEditInputHandler);
            uiEditSetClicks(editTxt, "type2", "mouseover");
            uiEditSetMinWidth(editTxt, 9000);
        }
    }

    drawFrame( PIX3, R10, x, y, z, wd, ht, scale, color, back_color );

    // Where in the window can we draw?
    windowDrawArea.x = x;
    windowDrawArea.y = y;
    windowDrawArea.width = wd;
    windowDrawArea.height = ht;
    uiBoxAlter(&windowDrawArea, UIBAT_SHRINK, UIBAD_ALL, PIX3*scale);
    

    listViewDrawArea = windowDrawArea;
       uiBoxAlter(&listViewDrawArea, UIBAT_SHRINK, UIBAD_TOP, 62*scale);
     uiBoxAlter(&listViewDrawArea, UIBAT_SHRINK, UIBAD_BOTTOM, 15*scale);

    // The list view will be drawn inside the listViewDrawArea and above the window frame.
    pen.x = listViewDrawArea.x;
    pen.y = listViewDrawArea.y;
     pen.z = z;                    // Draw on top of the frame

    // Make sure the list view is initialized.
     if(!lfgListView)
    {
        lfgListView = lfg_initListView();
    }

    // SEARCH CONTROLS HERE
    set_scissor(1);
    scissor_dims( x+PIX3*scale, y+PIX3*scale, wd-PIX3*2*scale, ht-PIX3*2*scale );
     font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );
    combobox_display( &comboOrigin );
      combobox_display( &comboArchetype );
    combobox_display( &comboLevel );
     combobox_display( &comboMap );
    combobox_display( &comboLfg );

    drawLeagueModeButton(x+390*scale, y+12*scale, z, 90*scale, 20*scale, scale, color);

    font_color(CLR_WHITE, CLR_WHITE);
       prnt( x+20*scale, y+58*scale, z+10, scale, scale, "NameColon" );
       uiEditEditSimple(editTxt, x+65*scale,y+43*scale, z+2, 145*scale, 20*scale, &game_12, INFO_HELP_TEXT, FALSE);

       BuildCBox( &box, x+10*scale, y+40*scale, 200*scale, 23*scale );
       drawFrame( PIX3, R10, x+10*scale, y+40*scale, z+1, 200*scale, 23*scale, scale, color, mouseCollision(&box)?0x222222ff:CLR_BLACK );

    if( mouseClickHit( &box, MS_LEFT) )
        uiEditTakeFocus(editTxt);
    if( !mouseCollision(&box) && mouseDown(MS_LEFT) )
        uiEditReturnFocus(editTxt);

       if(D_MOUSEHIT == drawStdButton(x + 430*scale, y + 51*scale, z, 100*scale, 26*scale, window_GetColor(WDW_LFG), "SearchString", scale, 0))
    {
        // build search string
        sendSearchRequest();
        if(lfgListView)
            lfgListView->selectedItem = 0;
    }

    // TELL INVITE REFRESH HERE
      if(D_MOUSEHIT == drawStdButton(x + 295*scale, y + 51*scale, z, 50*scale, 26*scale, window_GetColor(WDW_LFG), "TellString", scale, eaSize( &lfgList ) == 0 || !lfgListView || !lfgListView->selectedItem))
    {
        char buf[128];
        if(lfgListView && lfgListView->selectedItem)
        {
            sprintf( buf, "/tell %s, ", ((Lfg*)lfgListView->selectedItem)->name );
            setChatEditText( buf );
            uiChatStartChatting(buf, 0);
        }
    }

    if(D_MOUSEHIT == drawStdButton(x + 352*scale, y + 51*scale, z, 50*scale, 26*scale, window_GetColor(WDW_LFG), "InviteString", scale, eaSize( &lfgList ) == 0 || !lfgListView || !lfgListView->selectedItem))
    {
        char buf[128];
        if(lfgListView && lfgListView->selectedItem)
        {
            sprintf( buf, "%s %s", (showLFGmode == WDW_LEAGUE) ? "li" : "i", ((Lfg*)lfgListView->selectedItem)->name );
            cmdParse( buf );
        }
    }
    set_scissor(0);

      clipperPushRestrict(&windowDrawArea);

       if( eaSize( &lfgList ) > 0 )
    {
        font( &game_12 );
         font_color( 0xffffff88, 0xffffff88);

        if( gResults == 1 )
            cprnt( x + wd/2, y + ht - PIX3*2*scale, z, scale, scale, "OneMatchFound", gResults );
        else
        {
             if( gTruncated )
                cprnt( x + wd/2, y + ht - PIX3*2*scale, z, scale, scale, "MatchesFoundTrunc", gResults  );
            else
                cprnt( x + wd/2, y + ht - PIX3*2*scale, z, scale, scale, "MatchesFound", gResults );
        }
    }
    clipperPop();

    // Do nothing if the friendlist is empty.
    if( eaSize( &lfgList ) == 0 )
    {
        int fontHeight = ttGetFontHeight(font_grp, scale, scale);
          clipperPushRestrict(&windowDrawArea);
          font_color( CLR_WHITE, CLR_WHITE );
        cprntEx(x + wd/2, y+ht/2, z+2, scale, scale, (CENTER_X|CENTER_Y), "NoLfgString" );
        clipperPop();
        set_scissor( 0 );
        return 0;
    }

    if(!lfgListView)
        return 0;
    // Draw and handle all scroll bar functionaltiy if the friendlist is larger than the window.
    // This is seperate from the list view because the scroll bar is always outside the draw area
    // of the list view.
    if(lfgListView)
    {
        int fullDrawHeight = uiLVGetFullDrawHeight(lfgListView);
        int currentHeight = uiLVGetHeight(lfgListView);
        if(lfgListView)
        {
            if(fullDrawHeight > currentHeight)
            {
                  doScrollBar( &sb, currentHeight, fullDrawHeight, wd, R10+62*scale, z+2, 0, &listViewDrawArea );
                lfgListView->scrollOffset = sb.offset;
            }
            else
                lfgListView->scrollOffset = 0;
        }


    }

    // How tall and wide is the list view supposed to be?
    uiLVSetDrawColor(lfgListView, window_GetColor(WDW_LFG));
    uiLVSetHeight(lfgListView, listViewDrawArea.height);
    uiLVHFinalizeSettings(lfgListView->header);
     uiLVHSetWidth(lfgListView->header, MAX( wd - PIX3, listViewDrawArea.width));

     // Rebuild the entire list if required.
     if(lfgListView && lfgListRebuildRequired)
    {
        int i;

        uiLVClear(lfgListView);
        lfgListView->selectedItem = NULL;
        for(i = 0; i < eaSize(&lfgList); i++)
            uiLVAddItem(lfgListView, lfgList[i]);

        uiLVSortBySelectedColumn(lfgListView);

        lfgListRebuildRequired = 0;
    }

    // Disable mouse over on window resize.
    {
        Wdw* friendsWindow = wdwGetWindow(WDW_FRIENDS);
        if(friendsWindow->drag_mode)
            uiLVEnableMouseOver(lfgListView, 0);
        else
            uiLVEnableMouseOver(lfgListView, 1);
    }


    // Draw the draw the list and clip everything to inside the proper drawing area of the list view.
    clipperPushRestrict(&listViewDrawArea);
     uiLVSetScale(lfgListView, scale );
    uiLVDisplay(lfgListView, pen);
    clipperPop();

    // Handle list view input.
     uiLVHandleInput(lfgListView, pen);

    if(lfg_TargetIsInList())
    {
        int i;
         if(current_target && !lfgListView->newlySelectedItem)
        {
            for( i = 0; i < eaSizeUnsafe(&lfgListView->items); i++ )
            {
                Lfg *item = lfgListView->items[i];
                if( item->dbid == current_target->db_id )
                    lfgListView->selectedItem = item;
            }
        }
    }
    else if (!lfgListView->newlySelectedItem)
        lfgListView->selectedItem = 0;

       if(lfgListView->newlySelectedItem)
    {
        int index;
        Lfg *item;
        Entity * target;
        index = eaFind(&lfgListView->items, lfgListView->selectedItem);

         item = lfgListView->items[index];
        target = entFromDbId(item->dbid);

        if( item->dbid != e->db_id )
        {
              current_target = target;
             gSelectedHandle[0] = 0;    // since we don't know handle from the LFG info

            if(!current_target)
            {
                gSelectedDBID = item->dbid;
                strcpy(gSelectedName, item->name );
            }
            else
                gSelectedDBID = 0;

            if( item->dbid == playerPtr()->db_id )
                lfgListView->selectedItem = 0;

            if(index != -1 && uiMouseCollision(lfgListView->itemWindows[index]) && mouseRightClick())
            {    
                if(current_target)
                    contextMenuForTarget(current_target);
                else
                    contextMenuForOffMap();
            }
        }    
        lfgListView->newlySelectedItem = 0;
    }

        

    // Clip and draw the remove friend button
    listViewDrawArea.y += listViewDrawArea.height;
    listViewDrawArea.height = LFG_FOOTER*scale;

     return 0;
}


//---------------------------------------------------------------------------------------
// Setting Hide Options - Homeless
//---------------------------------------------------------------------------------------

void hideSetAll(  bool on )
{
    Entity * e = playerPtr();
    int i;
    for( i = 0; i < HIDE_COUNT; i++ )
    {
        if(on)
            e->pl->hidden |= (1<<i);
        else
            e->pl->hidden &= ~(1<<i);
    }
    cmdParsef( "hideset %i", e->pl->hidden );
}



void hideSetValue( int hide_type, bool on )
{
    Entity * e = playerPtr();
    if( on )
        cmdParsef( "hideset %i", e->pl->hidden | (1<<hide_type) );
    else
        cmdParsef( "hideset %i", e->pl->hidden & ~(1<<hide_type) );
}



static bool s_hide[HIDE_COUNT];
DialogCheckbox hideDCB[HIDE_COUNT] = 
{ 
    {"HideSearch", &s_hide[0]},
    {"HideSG", &s_hide[1]},
    {"HideFriends", &s_hide[2]},
    {"HideGFriends", &s_hide[3]},
    {"HideGChannels", &s_hide[4]},
    {"HideTells", &s_hide[5]},
    {"HideInvites", &s_hide[6]},
};

void hideSetSend( void * data )
{
    Entity * e = playerPtr();
    int i;

    for( i = 0; i < HIDE_COUNT; i++ )
    {
        if(s_hide[i])
            e->pl->hidden |= (1<<i);
        else
            e->pl->hidden &= ~(1<<i);
    }

    cmdParsef( "hideset %i", e->pl->hidden );
}

void hideShowDialog(int clear_choices)
{
    Entity * e = playerPtr();
    int i;

    if( clear_choices )
        memset( s_hide, 0, sizeof(s_hide) );
    else
    {
        for( i=0; i < HIDE_COUNT; i++ )
            s_hide[i] = e->pl->hidden&(1<<i);
    }
    dialog( DIALOG_ACCEPT_CANCEL, -1, -1, -1, -1, textStd("HideOptions"), NULL, hideSetSend, NULL, NULL, 0, NULL, hideDCB, HIDE_COUNT, 0, 0, 0 );
}
