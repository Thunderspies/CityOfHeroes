

#include <utilitieslib/components/earray.h>         // for StructGetNum
#include "player/player.h"
#include "entity/entity.h"         // for entity
#include "entity/powers.h"
#include "entity/entPlayer.h"
#include "entity/character_base.h" // for pchar
#include "entity/attrib_description.h"
#include <utilitieslib/utils/file.h>

#include "UI/sprite/sprite_font.h"    // for font definitions
#include "UI/sprite/sprite_text.h"    // for font functions
#include "UI/sprite/sprite_base.h"    // for sprites
#include "graphics/textureatlas.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "graphics/ttFontUtil.h" 

#include "UI/uiWindows.h"
#include "UI/uiInput.h"
#include "UI/uiUtilGame.h"     // for draw frame
#include "UI/uiUtilMenu.h"     //
#include "UI/uiUtil.h"         // for color definitions
#include "UI/uiNet.h"
#include "UI/uiContextMenu.h"
#include "UI/uiClipper.h"
#include "UI/uiCombatNumbers.h"



ContextMenu * s_CombatMonitorCM = 0;

#define LINE_HT 15

static void combatMonitorcm_openCombatNumbers( void * data )
{
    window_setMode(WDW_COMBATNUMBERS, WINDOW_GROWING);
    window_bringToFront(WDW_COMBATNUMBERS);
}

static void combatMonitorcm_stopDisplayAll( AttribDescription * pDesc )
{
    int i;
    Entity *e = playerPtr();

    for( i = 0; i < MAX_COMBAT_STATS; i++ )
        e->pl->combatMonitorStats[i].iKey = 0;
    sendCombatMonitorUpdate();
}

static void combatMonitorcm_stopDisplay( AttribDescription * pDesc )
{
    int i;
    Entity *e = playerPtr();

    for( i = 0; i < MAX_COMBAT_STATS; i++ )
    {
        AttribDescription *pDesc1 = attribDescriptionGet( e->pl->combatMonitorStats[i].iKey  );
        if(pDesc == pDesc1)
            e->pl->combatMonitorStats[i].iKey = 0;
    }
    sendCombatMonitorUpdate();
}

void combatNumbers_StopMonitor( char * pchName )
{
    AttribDescription *pDesc = attribDescriptionGetByName( pchName );
    if( pDesc )
        combatMonitorcm_stopDisplay(pDesc);
}

static char *combatMonitorcm_stopDisplayText( AttribDescription * pDesc )
{
    return textStd( "StopDisplayString", pDesc->pchDisplayName );
}

static void combatMonitorcm_MoveDown( AttribDescription * pDesc )
{
    int i;
    Entity *e = playerPtr();

    // assumes sorted list
    for( i = 0; i < MAX_COMBAT_STATS-1; i++ )
    {
        AttribDescription *pDesc1 = attribDescriptionGet( e->pl->combatMonitorStats[i].iKey  );
        AttribDescription *pDesc2 = attribDescriptionGet( e->pl->combatMonitorStats[i-1].iKey  );
        if( pDesc1 == pDesc && pDesc2 )
        {
            int tmp = e->pl->combatMonitorStats[i].iOrder;
            e->pl->combatMonitorStats[i].iOrder = e->pl->combatMonitorStats[i+1].iOrder;
            e->pl->combatMonitorStats[i+1].iOrder = tmp;
        }
    }
    sendCombatMonitorUpdate();
}

static void combatMonitorcm_MoveUp( AttribDescription * pDesc )
{
    int i;
    Entity *e = playerPtr();
    CombatMonitorStat * c1 = 0; 

    // assumes sorted list
    for( i = 1; i < MAX_COMBAT_STATS; i++ )
    {
        AttribDescription *pDesc1 = attribDescriptionGet( e->pl->combatMonitorStats[i].iKey  );
        AttribDescription *pDesc2 = attribDescriptionGet( e->pl->combatMonitorStats[i-1].iKey  );
        if( pDesc1 == pDesc && pDesc2 )
        {
            int tmp = e->pl->combatMonitorStats[i].iOrder;
            e->pl->combatMonitorStats[i].iOrder = e->pl->combatMonitorStats[i-1].iOrder;
            e->pl->combatMonitorStats[i-1].iOrder = tmp;
        }
    }
    sendCombatMonitorUpdate();
}

static void initCombatMonitorCM()
{
    if(s_CombatMonitorCM)
        return;

    s_CombatMonitorCM = contextMenu_Create(0);
    contextMenu_addTitle( s_CombatMonitorCM, "CombatMonitorString" );
    contextMenu_addCode( s_CombatMonitorCM, alwaysAvailable, 0, combatMonitorcm_openCombatNumbers, 0, "OpenCombatNumbersString", 0 );
    contextMenu_addVariableTextCode( s_CombatMonitorCM, alwaysAvailable, 0, combatMonitorcm_stopDisplay, 0, combatMonitorcm_stopDisplayText, 0, 0 );
    contextMenu_addCode( s_CombatMonitorCM, alwaysAvailable, 0, combatMonitorcm_MoveUp, 0, "MoveUpString", 0 );
    contextMenu_addCode( s_CombatMonitorCM, alwaysAvailable, 0, combatMonitorcm_MoveDown, 0, "MoveDownString", 0 );
    contextMenu_addDivider( s_CombatMonitorCM );
    contextMenu_addCode( s_CombatMonitorCM, alwaysAvailable, 0, combatMonitorcm_stopDisplayAll, 0, "StopDisplayAllString", 0 );
}


static int compareCombatStats( const CombatMonitorStat *s1, const CombatMonitorStat *s2 )
{
    if( s1->iKey && !s2->iKey ) 
        return -1;
    if( !s1->iKey && s2->iKey )
        return 1;
    else 
        return s1->iOrder-s2->iOrder;
}

static int col_wd;

static void displayCombatStat( F32 x, F32 y, F32 wd, F32 z, F32 sc, int key )
{
    AttribDescription *pDesc = attribDescriptionGet( key );

    if( pDesc )
    {
        CBox box;
        char * str;
        font(&game_12);
        font_color(CLR_WHITE,CLR_WHITE);
          cprntEx( x+5*sc, y+LINE_HT*sc, z, sc, sc, 0, pDesc->pchDisplayName );

        str = attribDesc_GetString( pDesc, pDesc->fVal,  g_pAttribTarget&&g_pAttribTarget->pchar?g_pAttribTarget->pchar:playerPtr()->pchar );
        cprntEx( x+col_wd, y+LINE_HT*sc, z, sc, sc, NO_MSPRINT, "%s", str );

        BuildCBox(&box, x, y, wd, LINE_HT*sc );
        if( mouseClickHit(&box,MS_RIGHT))
        {
            contextMenu_displayEx(s_CombatMonitorCM,pDesc);
        }
    }
}

int combatMonitorWindow(void)
{
    F32 x,y,z,wd,ht,sc;
    int i, count=0, color, bcolor;
    CBox box;
    Entity *e = playerPtr();

    if (!e)
        return 0;

    for(i=0;i<MAX_COMBAT_STATS;i++)
    {
        if(e->pl->combatMonitorStats[i].iKey)
            count++;
        else
            e->pl->combatMonitorStats[i].iOrder = 0;
    }

    initCombatMonitorCM();
    qsort(e->pl->combatMonitorStats, MAX_COMBAT_STATS, sizeof(CombatMonitorStat), compareCombatStats );

    if(!count)
        window_setMode(WDW_COMBATMONITOR,WINDOW_SHRINKING);
    else
    {
        F32 wd = 0, tmp;
        col_wd=0;
        window_setMode(WDW_COMBATMONITOR,WINDOW_GROWING);
        window_getDims(WDW_COMBATMONITOR, &x, &y, &z, &tmp, &ht, &sc, &color, &bcolor);

        for( i = 0; i < count; i++ )
        {
            AttribDescription *pDesc = attribDescriptionGet( e->pl->combatMonitorStats[i].iKey  );
            if( pDesc )
            {    
                F32 name_wd = str_wd(&game_12,sc,sc,pDesc->pchDisplayName);
                col_wd = MAX( col_wd, name_wd+10*sc );
            }
        }
        for( i = 0; i < count; i++ )
        {
            AttribDescription *pDesc = attribDescriptionGet( e->pl->combatMonitorStats[i].iKey  );
            if( pDesc )
            {    
                F32 num_wd = str_wd(&game_12,sc,sc, "%s", attribDesc_GetString( pDesc, pDesc->fVal,  g_pAttribTarget&&g_pAttribTarget->pchar?g_pAttribTarget->pchar:playerPtr()->pchar ));
                wd = MAX( wd, 5*sc+col_wd+num_wd);
            }
        }
        window_setDims(WDW_COMBATMONITOR, -1, -1, wd, (count*LINE_HT+4)*sc );
    }

    
    if( !window_getDims(WDW_COMBATMONITOR, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
        return 0;

    BuildCBox( &box, x, y, wd, ht );
    drawFrame( PIX2, R4, x, y, z, wd, ht, sc, color, bcolor );
    clipperPushCBox( &box );

    y+= 2*sc;

    for( i = 0; i < count; i++ )
        displayCombatStat( x, y + i*LINE_HT*sc, wd, z, sc, e->pl->combatMonitorStats[i].iKey );

    clipperPop();

    return 0;
}



