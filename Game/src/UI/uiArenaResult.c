#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "uiListView.h"
#include "UI/uiScrollBar.h"
#include "UI/uiClipper.h"

#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "gameData/arenastruct.h"
#include "UI/uiArenaResult.h"
#include "UI/uiArena.h"
#include "UI/uiArenaList.h"
#include <utilitieslib/components/Earray.h>
#include "cmdparse/cmdgame.h"
#include "graphics/ttFontUtil.h"
#include "arena/ArenaGame.h"
#include "win/win_init.h"
#include <utilitieslib/utils/timing.h>
#include "graphics/textureatlas.h"
#include "UI/uiWindows_init.h"

//------------------------------------------------------------------------------------------------------
// List View Management ////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------
static UIListView* lvResult = 0;
static UIListView* lvMinimizedResult = 0;
static int total_width = -1, total_height = -1;

static ArenaTeamStyle teamStyle = 0;
static ArenaTeamType teamType = 0;
static ArenaVictoryType victoryType = 0;
static ArenaWeightClass weightClass = 0;
static char desc[ARENA_DESCRIPTION_LEN] = {0};

// Each column needs a compare function
static int arenaColumnCompareRank( const ArenaRankingTableEntry **a1, const ArenaRankingTableEntry **a2 )
{
    return (*a1)->rank-(*a2)->rank;
}

static int arenaColumnComparePts( const ArenaRankingTableEntry **a1, const ArenaRankingTableEntry **a2 )
{
    return ((*a1)->pts-(*a2)->pts)*10000;
}

static int arenaColumnComparePlayed( const ArenaRankingTableEntry **a1, const ArenaRankingTableEntry **a2 )
{
    return (*a1)->played-(*a2)->played;
}

static int arenaColumnCompareRating( const ArenaRankingTableEntry **a1, const ArenaRankingTableEntry **a2 )
{
    return ((*a1)->rating-(*a2)->rating)*10000;
}

static int arenaColumnCompareOppWinPct( const ArenaRankingTableEntry **a1, const ArenaRankingTableEntry **a2 )
{
    return ((*a1)->opponentAvgPts-(*a2)->opponentAvgPts)*10000;
}

static int arenaColumnCompareOppOppWinPct( const ArenaRankingTableEntry **a1, const ArenaRankingTableEntry **a2 )
{
    return ((*a1)->opp_oppAvgPts-(*a2)->opp_oppAvgPts)*10000;
}

static int arenaColumnCompareKills( const ArenaRankingTableEntry **a1, const ArenaRankingTableEntry **a2 )
{
    return (*a1)->kills-(*a2)->kills;
}

static int arenaColumnCompareSide( const ArenaRankingTableEntry **a1, const ArenaRankingTableEntry **a2 )
{
    return (*a1)->side-(*a2)->side;
}

static int arenaColumnComparePlayername( const ArenaRankingTableEntry **a1, const ArenaRankingTableEntry **a2 )
{
    return stricmp((*a1)->playername,(*a2)->playername);
}

static int arenaColumnCompareSEPts( const ArenaRankingTableEntry **a1, const ArenaRankingTableEntry **a2 )
{
    return ((*a1)->SEpts-(*a2)->SEpts)*10000;
}

static int arenaColumnCompareDeathTime(const ArenaRankingTableEntry **a1, const ArenaRankingTableEntry **a2 )
{
    return (*a1)->rank-(*a2)->rank;    // rank is the same whenever this gets displayed anyway
}

static UIBox lvResultsDisplayItem(UIListView* list, PointFloatXYZ rowOrigin, void* userSettings, ArenaRankingTableEntry* item, int itemIndex);

static void arenaResultInit(ArenaRankingTableEntry * arte)
{
    lvResult = uiLVCreate();
    lvMinimizedResult = uiLVCreate();

    if (arte==NULL || (int)arte->rank>=0) {
        uiLVHAddColumn(lvResult->header, ARENA_RESULT_RANK, ARENA_RESULT_RANK, 50);
        uiLVHAddColumn(lvMinimizedResult->header, ARENA_RESULT_RANK, ARENA_RESULT_RANK, 50);
        uiLVBindCompareFunction(lvResult, ARENA_RESULT_RANK, arenaColumnCompareRank );
        uiLVBindCompareFunction(lvMinimizedResult, ARENA_RESULT_RANK, arenaColumnCompareRank );
    }
    if (arte==NULL || arte->playername!=NULL) {
        uiLVHAddColumn(lvResult->header, ARENA_RESULT_PLAYERNAME, ARENA_RESULT_PLAYERNAME, 150);
        uiLVHAddColumn(lvMinimizedResult->header, ARENA_RESULT_PLAYERNAME, ARENA_RESULT_PLAYERNAME, 150);
        uiLVBindCompareFunction(lvResult, ARENA_RESULT_PLAYERNAME, arenaColumnComparePlayername );
        uiLVBindCompareFunction(lvMinimizedResult, ARENA_RESULT_PLAYERNAME, arenaColumnComparePlayername );
    }
    if (arte==NULL || (int)arte->played>=0) {
        uiLVHAddColumn(lvResult->header, ARENA_RESULT_PLAYED, ARENA_RESULT_PLAYED, 50);
        uiLVHAddColumn(lvMinimizedResult->header, ARENA_RESULT_PLAYED, ARENA_RESULT_PLAYED, 50);
        uiLVBindCompareFunction(lvResult, ARENA_RESULT_PLAYED, arenaColumnComparePlayed );
        uiLVBindCompareFunction(lvMinimizedResult, ARENA_RESULT_PLAYED, arenaColumnComparePlayed );
    }
    if (arte==NULL || arte->SEpts>=0) {
        uiLVHAddColumn(lvResult->header, ARENA_RESULT_SE_PTS, ARENA_RESULT_SE_PTS, 50);
        uiLVHAddColumn(lvMinimizedResult->header, ARENA_RESULT_SE_PTS, ARENA_RESULT_SE_PTS, 50);
        uiLVBindCompareFunction(lvResult, ARENA_RESULT_SE_PTS, arenaColumnCompareSEPts );
        uiLVBindCompareFunction(lvMinimizedResult, ARENA_RESULT_SE_PTS, arenaColumnCompareSEPts );
    }
    if (arte==NULL || arte->pts>=0) {
        uiLVHAddColumn(lvResult->header, ARENA_RESULT_PTS, ARENA_RESULT_PTS, 50);
        uiLVHAddColumn(lvMinimizedResult->header, ARENA_RESULT_PTS, ARENA_RESULT_PTS, 50);
        uiLVBindCompareFunction(lvResult, ARENA_RESULT_PTS, arenaColumnComparePts );
        uiLVBindCompareFunction(lvMinimizedResult, ARENA_RESULT_PTS, arenaColumnComparePts );
    }
    if (arte==NULL || arte->opponentAvgPts>=0) {
        uiLVHAddColumn(lvResult->header, ARENA_RESULT_OPP_AVG_PTS, ARENA_RESULT_OPP_AVG_PTS, 50);
        uiLVHAddColumn(lvMinimizedResult->header, ARENA_RESULT_OPP_AVG_PTS, ARENA_RESULT_OPP_AVG_PTS, 50);
        uiLVBindCompareFunction(lvResult, ARENA_RESULT_OPP_AVG_PTS, arenaColumnCompareOppWinPct );
        uiLVBindCompareFunction(lvMinimizedResult, ARENA_RESULT_OPP_AVG_PTS, arenaColumnCompareOppWinPct );
    }
    if (arte==NULL || arte->opp_oppAvgPts>=0) {
        uiLVHAddColumn(lvResult->header, ARENA_RESULT_OPP_OPP_AVG_PTS, ARENA_RESULT_OPP_OPP_AVG_PTS, 50);
        uiLVHAddColumn(lvMinimizedResult->header, ARENA_RESULT_OPP_OPP_AVG_PTS, ARENA_RESULT_OPP_OPP_AVG_PTS, 50);
        uiLVBindCompareFunction(lvResult, ARENA_RESULT_OPP_OPP_AVG_PTS, arenaColumnCompareOppOppWinPct );
        uiLVBindCompareFunction(lvMinimizedResult, ARENA_RESULT_OPP_OPP_AVG_PTS, arenaColumnCompareOppOppWinPct );
    }
    if (arte==NULL || (int)arte->kills>=0) {
        uiLVHAddColumn(lvResult->header, ARENA_RESULT_KILLS, ARENA_RESULT_KILLS, 50);
        uiLVHAddColumn(lvMinimizedResult->header, ARENA_RESULT_KILLS, ARENA_RESULT_KILLS, 50);
        uiLVBindCompareFunction(lvResult, ARENA_RESULT_KILLS, arenaColumnCompareKills );
        uiLVBindCompareFunction(lvMinimizedResult, ARENA_RESULT_KILLS, arenaColumnCompareKills );
    }
    if (arte==NULL || (int)arte->rating>=0) {
        uiLVHAddColumn(lvResult->header, ARENA_RESULT_RATING, ARENA_RESULT_RATING, 50);
        uiLVHAddColumn(lvMinimizedResult->header, ARENA_RESULT_RATING, ARENA_RESULT_RATING, 50);
        uiLVBindCompareFunction(lvResult, ARENA_RESULT_RATING, arenaColumnCompareRating);
        uiLVBindCompareFunction(lvMinimizedResult, ARENA_RESULT_RATING, arenaColumnCompareRating);
    }
    if (arte==NULL || (int)arte->deathtime>=0) {
        uiLVHAddColumn(lvResult->header, ARENA_RESULT_DEATHTIME, ARENA_RESULT_DEATHTIME, 50);
        uiLVHAddColumn(lvMinimizedResult->header, ARENA_RESULT_DEATHTIME, ARENA_RESULT_DEATHTIME, 50);
        uiLVBindCompareFunction(lvResult, ARENA_RESULT_DEATHTIME, arenaColumnCompareDeathTime);
        uiLVBindCompareFunction(lvMinimizedResult, ARENA_RESULT_DEATHTIME, arenaColumnCompareDeathTime);
    }
    lvResult->displayItem = lvResultsDisplayItem;
    lvMinimizedResult->displayItem = lvResultsDisplayItem;

    uiLVFinalizeSettings(lvResult);
    uiLVFinalizeSettings(lvMinimizedResult);

    uiLVEnableMouseOver(lvResult, 0);
    uiLVEnableSelection(lvResult, 0);
    uiLVEnableMouseOver(lvMinimizedResult, 0);
    uiLVEnableSelection(lvMinimizedResult, 0);
}

void arenaRebuildResultView(ArenaRankingTable * ranking)
{
    // Clear the list.  This also kills the current selection.
    uiLVClear(lvResult);
    uiLVDestroy(lvResult);
    uiLVClear(lvMinimizedResult);
    uiLVDestroy(lvMinimizedResult);

    strcpy( desc, ranking->desc );
    teamStyle = ranking->teamStyle;
    teamType = ranking->teamType;
    victoryType = ranking->victoryType;
    weightClass = ranking->weightClass;
    
    if (eaSize(&ranking->entries)==0)
        arenaResultInit(NULL);
    else
        arenaResultInit((ranking->entries)[0]);


    if(lvResult)
    {
        int i;
        for( i = 0; i<eaSize(&ranking->entries); i++ )
        {
            ArenaRankingTableEntry * ars=(ArenaRankingTableEntry*)calloc(1, sizeof(ArenaRankingTableEntry));
            memcpy(ars,ranking->entries[i],sizeof(ArenaRankingTableEntry));
            ars->playername=strdup(ranking->entries[i]->playername);
            // Add all items into the list
            uiLVAddItem(lvResult,ars);
        }

        // Resort the list
        uiLVSortBySelectedColumn(lvResult);
    }

    if (lvMinimizedResult)
    {
        ArenaRankingTableEntry **arsArray = 0;
        int i;
        for (i = 0; i < eaSize(&ranking->entries); i++)
        {
            ArenaRankingTableEntry *ars;

            while (eaSize(&arsArray) < ranking->entries[i]->side)
            {
                ars = (ArenaRankingTableEntry*) calloc(1, sizeof(ArenaRankingTableEntry));
                ars->rank = 0; // copy this out of entries
                ars->playername = strdup(getTeamString(eaSize(&arsArray) + 1));
                ars->dbid = 0; // do I care about this?
                ars->side = eaSize(&arsArray) + 1;
                ars->played = 0; // do I care about this?
                ars->SEpts = 0; // do I care about this?
                ars->pts = 0; // do I care about this?
                ars->opponentAvgPts = 0; // do I care about this?
                ars->opp_oppAvgPts = 0; // do I care about this?
                ars->rating = 0; // do I care about this?
                ars->kills = 0; // copy this out of entries
                ars->deathtime = -1; // copy this out of entries
                eaInsert(&arsArray, ars, eaSize(&arsArray));
            }

            ars = arsArray[ranking->entries[i]->side - 1];
            ars->rank = ranking->entries[i]->rank;
            ars->kills += ranking->entries[i]->kills;
            if (ranking->entries[i]->deathtime == 0)
            {
                ars->deathtime = 0;
            }
            else if (ars->deathtime != 0 && ranking->entries[i]->deathtime > ars->deathtime)
            {
                ars->deathtime = ranking->entries[i]->deathtime;
            }
        }

        for (i = 0; i < eaSize(&arsArray); i++)
        {
            uiLVAddItem(lvMinimizedResult, arsArray[i]);
        }

        // Resort the list
        uiLVSortBySelectedColumn(lvMinimizedResult);
    }
}


static UIBox lvResultsDisplayItem(UIListView* list, PointFloatXYZ rowOrigin, void* userSettings, ArenaRankingTableEntry * item, int itemIndex)
{
    UIBox box;
    PointFloatXYZ pen = rowOrigin;
    UIColumnHeaderIterator columnIterator;
    float sc = list->scale;

    font(&game_12);

     box.origin.x = rowOrigin.x;
    box.origin.y = rowOrigin.y;

    uiCHIGetIterator(list->header, &columnIterator);

     rowOrigin.z+=1;
    total_height += 20*sc;

    for(;uiCHIGetNextColumn(&columnIterator, list->scale);)
    {
        UIColumnHeader* column = columnIterator.column;
        UIBox clipBox;

        if(stricmp(column->name, ARENA_RESULT_RANK) == 0 )
        {
            if (item->rank<0) continue;
        } else 
        if(stricmp(column->name, ARENA_RESULT_PTS) == 0 )
        {
            if (item->pts<0) continue;
        } else 
        if(stricmp(column->name, ARENA_RESULT_RATING) == 0 )
        {
            if (item->rating<0) continue;
        } else 
        if(stricmp(column->name, ARENA_RESULT_PLAYED) == 0 )
        {
            if ((int)item->played<0) continue;
        } else 
        if(stricmp(column->name, ARENA_RESULT_OPP_AVG_PTS) == 0 )
        {
            if (item->opp_oppAvgPts<0) continue;
        } else 
        if(stricmp(column->name, ARENA_RESULT_OPP_OPP_AVG_PTS) == 0 )
        {
            if (item->opponentAvgPts<0) continue;
        }
        if(stricmp(column->name, ARENA_RESULT_SE_PTS) == 0 )
        {
            if (item->SEpts<0) continue;
        }
        if(stricmp(column->name, ARENA_RESULT_KILLS) == 0 )
        {
            if (item->kills<0) continue;
        }
        if(stricmp(column->name, ARENA_RESULT_PLAYERNAME) == 0 )
        {
            if (item->playername==NULL) continue;
        }
        if(stricmp(column->name, ARENA_RESULT_DEATHTIME) == 0)
        {
            if (item->deathtime<0) continue;
        }
        
        font_color(CLR_WHITE,CLR_WHITE);
        pen.x = rowOrigin.x + columnIterator.columnStartOffset*sc - PIX3*sc;
        clipBox.origin.x = pen.x;
        clipBox.origin.y = pen.y;
        clipBox.height = 20*sc;

        total_width = pen.x + columnIterator.currentWidth - rowOrigin.x + 2*R10*sc;

        clipBox.width = (columnIterator.currentWidth+5)*sc;

        if(stricmp(column->name, ARENA_RESULT_RANK) == 0 )
        {
            cprnt(pen.x+clipBox.width/2,pen.y+18*sc,rowOrigin.z,sc,sc,"%d",item->rank);
        } else 
        if(stricmp(column->name, ARENA_RESULT_PTS) == 0 )
        {
            cprnt(pen.x+clipBox.width/2,pen.y+18*sc,rowOrigin.z,sc,sc,"%.1f",item->pts);
        } else 
        if(stricmp(column->name, ARENA_RESULT_RATING) == 0 )
        {
            cprnt(pen.x+clipBox.width/2,pen.y+18*sc,rowOrigin.z,sc,sc,"%.2f",item->rating);
        } else 
        if(stricmp(column->name, ARENA_RESULT_PLAYED) == 0 )
        {
            cprnt(pen.x+clipBox.width/2,pen.y+18*sc,rowOrigin.z,sc,sc,"%d",item->played);
        } else 
        if(stricmp(column->name, ARENA_RESULT_OPP_AVG_PTS) == 0 )
        {
            cprnt(pen.x+clipBox.width/2,pen.y+18*sc,rowOrigin.z,sc,sc,"%3.2f",item->opponentAvgPts);
        } else 
        if(stricmp(column->name, ARENA_RESULT_OPP_OPP_AVG_PTS) == 0 )
        {
            cprnt(pen.x+clipBox.width/2,pen.y+18*sc,rowOrigin.z,sc,sc,"%3.2f",item->opp_oppAvgPts);
        }
        if(stricmp(column->name, ARENA_RESULT_SE_PTS) == 0 )
        {
            cprnt(pen.x+clipBox.width/2,pen.y+18*sc,rowOrigin.z,sc,sc,"%.1f",item->SEpts);
        }
/*        if(stricmp(column->name, ARENA_RESULT_SIDE) == 0 )
        {
            CBox box;
            BuildCBox( &box, 0, pen.y, 3000, 20*sc );
            drawBox( &box, rowOrigin.z, 0, getTeamColor(item->side) );
            prnt(pen.x,pen.y+18*sc,rowOrigin.z,sc,sc,"%s",getTeamString(item->side));
        }*/
        if(stricmp(column->name, ARENA_RESULT_PLAYERNAME) == 0 )
        {
            CBox box;
            BuildCBox( &box, 0, pen.y, 3000, 20*sc );
             drawBox( &box, rowOrigin.z, 0, getTeamColor(item->side) );
            cprnt(pen.x+clipBox.width/2,pen.y+18*sc,rowOrigin.z,sc,sc,"%s",item->playername);
        }
        if(stricmp(column->name, ARENA_RESULT_KILLS) == 0 )
        {
            cprnt(pen.x+clipBox.width/2,pen.y+18*sc,rowOrigin.z,sc,sc,"%d",item->kills);
        }
        if(stricmp(column->name, ARENA_RESULT_DEATHTIME) == 0)
        {
            if(item->deathtime == 0)
                cprnt(pen.x+clipBox.width/2,pen.y+18*sc,rowOrigin.z,sc,sc,"---");
            else
            {
                static char buf[20];
                timerMakeOffsetStringFromSeconds(buf, ABS_TO_SEC(item->deathtime));
                cprnt(pen.x+clipBox.width/2,pen.y+18*sc,rowOrigin.z,sc,sc,"%s",buf);
            }
        }
        

    }
    return box;
}

//------------------------------------------------------------------------------------------------------
// Main Window /////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------

static int arenaResultWindow_isMinimized = 0;

int arenaResultWindow()
{
    float x, y, z, wd, ht, sc;
    int color, back_color;
      int header_ht = 1, footer_ht = 0, textWd, total_text = 0;

    static ScrollBar sb = { WDW_ARENA_RESULT };

    font(&game_12);

    if( !lvResult )
        arenaResultInit(NULL);

    if( !window_getDims( WDW_ARENA_RESULT, &x, &y, &z, &wd, &ht, &sc, &color, &back_color) )
        return 0;

    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, back_color );
    drawFrame( PIX3, R10, x, y, z + 10, wd, ht, sc, color, 0 );

    // header information
    font(&game_14);
    font_color(CLR_WHITE, CLR_WHITE);
    
    if (!arenaResultWindow_isMinimized)
    {
        header_ht = 50;

        if( desc[0] )
        {
            total_text = str_wd(&game_12, sc, sc, desc) + (R10+PIX3)*sc;
            cprntEx( x + R10*sc, y + header_ht*sc/2, z, sc, sc, CENTER_Y, desc );
        }
        else
        {
            total_text = str_wd(&game_12, sc, sc, arenaGetEventName(teamStyle, teamType)) + (R10+PIX3)*sc;
            cprntEx( x + R10*sc, y + header_ht*sc/2, z, sc, sc, CENTER_Y, arenaGetEventName(teamStyle, teamType) );
        }

        font(&game_12);
        textWd = str_wd(&game_12, sc, sc, arenaGetWeightName(weightClass)) + (R10+PIX3)*sc;
        total_text += textWd;
        cprntEx( x + wd - textWd, y + header_ht*sc/3, z, sc, sc, CENTER_Y, arenaGetWeightName(weightClass) );
        textWd = str_wd(&game_12, sc, sc, arenaGetVictoryTypeName(victoryType)) + (R10+PIX3)*sc; 
        cprntEx( x + wd - textWd, y + header_ht*2*sc/3, z, sc, sc, CENTER_Y, arenaGetVictoryTypeName(victoryType) );
    }

    // minimize/maximize arrow
    {
        AtlasTex * arrow;
        float arrowY;

        if (!arenaResultWindow_isMinimized)
        {
            arrow = atlasLoadTexture( "Jelly_arrow_up.tga" );
            arrowY = y - (JELLY_HT + arrow->height)*sc/2;
        }
        else
        {
            arrow = atlasLoadTexture( "Jelly_arrow_down.tga" );
            arrowY = y - (JELLY_HT + arrow->height)*sc/2;
        }

        if (D_MOUSEHIT == drawGenericButton(arrow, x + wd - 60*sc, arrowY, z, sc, (color & 0xffffff00) | (0x88), (color & 0xffffff00) | (0xff), 1))
        {
            arenaResultWindow_isMinimized = !arenaResultWindow_isMinimized;
        }
    }

    // footer button to exit map
    if (game_state.mission_map && !arenaResultWindow_isMinimized)
    {
        footer_ht = 50;
        if( D_MOUSEHIT == drawStdButton( x + wd/2, y + ht - footer_ht/2, z, 100, footer_ht-10*sc, CLR_GREEN, "LeaveMap", 1.f, 0 ) )
        {
            cmdParse("requestexitmission 0");
        }
    }


    //display the list of events
    if (!arenaResultWindow_isMinimized)
    {
        if(lvResult)
        {
            PointFloatXYZ pen;
            UIBox listViewDrawArea;
            float fullDrawHeight, currentHeight;

            pen.x = x;
            pen.y = y+header_ht*sc;
            pen.z = z;

             total_height = header_ht*sc+footer_ht+46*sc;
            listViewDrawArea.x = x;
             listViewDrawArea.y = y+header_ht*sc;
            listViewDrawArea.width = wd;
            listViewDrawArea.height = ht-(PIX3+header_ht+footer_ht)*sc;

            // How tall and wide is the list view supposed to be?
            uiLVSetDrawColor(lvResult, window_GetColor(WDW_ARENA_RESULT));
            uiLVSetHeight(lvResult, listViewDrawArea.height);
            uiLVHSetWidth(lvResult->header, listViewDrawArea.width);
            uiLVSetScale(lvResult, sc );

            clipperPushRestrict(&listViewDrawArea);
            uiLVDisplay(lvResult, pen);
            clipperPop();

            // Handle list view input.
            uiLVHandleInput(lvResult, pen);
            uiLVSetClickable(lvResult,0);
            // scroll bar
              fullDrawHeight = uiLVGetFullDrawHeight(lvResult);
            currentHeight = uiLVGetHeight(lvResult) - uiLVGetMinDrawHeight(lvResult)/2;

            if(fullDrawHeight > currentHeight)
            {
                doScrollBar(&sb, currentHeight - R10 * sc, fullDrawHeight, wd, (PIX3+R10+header_ht)*sc, z+11, 0, &listViewDrawArea);
                lvResult->scrollOffset = sb.offset;
            }
            else
                lvResult->scrollOffset = 0;
        }
    }
    else
    {
        if(lvMinimizedResult)
        {
            PointFloatXYZ pen;
            UIBox listViewDrawArea;

            pen.x = x;
            pen.y = y+header_ht*sc;
            pen.z = z;

            total_height = header_ht*sc+footer_ht+46*sc;
            listViewDrawArea.x = x;
            listViewDrawArea.y = y+header_ht*sc;
            listViewDrawArea.width = wd;
            listViewDrawArea.height = ht-(PIX3+header_ht+footer_ht)*sc;

            // How tall and wide is the list view supposed to be?
            uiLVSetDrawColor(lvMinimizedResult, window_GetColor(WDW_ARENA_RESULT));
            uiLVSetHeight(lvMinimizedResult, listViewDrawArea.height);
            uiLVHSetWidth(lvMinimizedResult->header, listViewDrawArea.width);
            uiLVSetScale(lvMinimizedResult, sc );

            clipperPushRestrict(&listViewDrawArea);
            uiLVDisplay(lvMinimizedResult, pen);
            clipperPop();

            // Handle list view input.
            uiLVHandleInput(lvMinimizedResult, pen);
            uiLVSetClickable(lvMinimizedResult, 0);
            
            lvMinimizedResult->scrollOffset = 0;
        }
    }

    {
        int cww,cwh;
          windowClientSizeThisFrame(&cww,&cwh);
         if (total_height>cwh*3/4 || ht > cwh*3/4 )
            total_height=cwh*3/4;
        if(total_width < total_text)
            total_width = total_text;
         window_setDims( WDW_ARENA_RESULT, -1, -1, total_width, total_height);
    }
    return 0;
}
