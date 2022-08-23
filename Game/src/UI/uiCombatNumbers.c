


#include <utilitieslib/components/earray.h>         // for StructGetNum
#include "player/player.h"
#include "entity/entity.h"         // for entity
#include "entity/powers.h"
#include "entity/character_base.h" // for pchar
#include "entity/entPlayer.h"
#include "entity/attrib_description.h"

#include "UI/sprite/sprite_font.h"    // for font definitions
#include "UI/sprite/sprite_text.h"    // for font functions
#include "UI/sprite/sprite_base.h"    // for sprites
#include "graphics/textureatlas.h"

#include "UI/uiWindows.h"
#include "UI/uiInput.h"
#include "UI/uiUtilGame.h"     // for draw frame
#include "UI/uiUtilMenu.h"     //
#include "UI/uiUtil.h"         // for color definitions
#include "UI/uiGame.h"         // for start_menu
#include "UI/uiScrollBar.h"
#include "UI/uiNet.h"
#include "UI/uiToolTip.h"
#include "UI/uiContextMenu.h"
#include "UI/uiClipper.h"

#include <utilitieslib/language/MessageStoreUtil.h>
#include "graphics/ttFontUtil.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/language/AppLocale.h>
#include "cmdparse/cmdgame.h"

static ContextMenu * s_CombatNumberCM = 0;
static bool bHidePowers;
static ToolTipParent s_CombatNumberParent;

static int combatNumbersCM_checkHidePowers( void * data ){return (bHidePowers)?CM_CHECKED:CM_AVAILABLE;}
static void combatNumberCM_HidePowers(  void * data ){ bHidePowers = !bHidePowers;}

static int combatNumbersCM_checkMonitor( AttribDescription * pDesc )
{
    int i;
    Entity *e = playerPtr();

    if(!pDesc)
        return CM_HIDE;

    for( i = 0; i < MAX_COMBAT_STATS; i++ )
    {
        if(attribDescriptionGet(e->pl->combatMonitorStats[i].iKey) == pDesc )
            return CM_CHECKED;
    }
    return CM_AVAILABLE;
}

static void combatNumbersCM_Monitor( AttribDescription * pDesc )
{
    int i, iMaxOrd = 0;
    Entity *e = playerPtr();
    CombatMonitorStat * replace=0;

    for( i = 0; i < MAX_COMBAT_STATS; i++ )
    {
        if( attribDescriptionGet(e->pl->combatMonitorStats[i].iKey) == pDesc )
        {
            e->pl->combatMonitorStats[i].iKey = 0;
            return;
        }
        else
        {
            iMaxOrd = MAX( iMaxOrd, e->pl->combatMonitorStats[i].iOrder );
            if( !replace || !e->pl->combatMonitorStats[i].iKey )
                replace = &e->pl->combatMonitorStats[i];
            else if( replace->iOrder > e->pl->combatMonitorStats[i].iOrder )
                replace = &e->pl->combatMonitorStats[i];
        }
    }

    replace->iKey = (sizeof(CharacterAttributes)*pDesc->eType)+pDesc->offAttrib;
    replace->iOrder = iMaxOrd+1;
    sendCombatMonitorUpdate();
}

AttribDescription * attribDescriptionGetByName( char * pchName )
{
    int i,j;
    AttribDescription * pDesc;

    // make a pass looking only at display names
    for( i = eaSize(&g_AttribCategoryList.ppCategories)-1; i >= 0; i-- )
    {
        for( j = eaSize(&g_AttribCategoryList.ppCategories[i]->ppAttrib)-1; j>=0; j--)
        {
            pDesc = g_AttribCategoryList.ppCategories[i]->ppAttrib[j];
            if( strstri( textStd(pDesc->pchDisplayName), pchName) )
                return pDesc;
        }
    }

    // try internal names if that failed
    for( i = eaSize(&g_AttribCategoryList.ppCategories)-1; i >= 0; i-- )
    {
        for( j = eaSize(&g_AttribCategoryList.ppCategories[i]->ppAttrib)-1; j>=0; j--)
        {
            pDesc = g_AttribCategoryList.ppCategories[i]->ppAttrib[j];
            if( strstri( pDesc->pchName, pchName) )
                return pDesc;
        }
    }

    return NULL;
}

void combatNumbers_Monitor( char * pchName )
{
    AttribDescription *pDesc = attribDescriptionGetByName( pchName );
    if( pDesc )
        combatNumbersCM_Monitor(pDesc);
}

static char * combatNumbersCM_MonitorText( AttribDescription * pDesc )
{
    if(!pDesc)
        return NULL;

    return textStd( "cmMonitorAttrib", pDesc->pchDisplayName );
}

static void initCombatNumbersCM(void)
{
    if( s_CombatNumberCM )
        return;

    s_CombatNumberCM = contextMenu_Create(0);
    contextMenu_addTitle( s_CombatNumberCM, "CombatNumbersString" );
    contextMenu_addCheckBox( s_CombatNumberCM, combatNumbersCM_checkHidePowers, 0, combatNumberCM_HidePowers, 0, "cmHidePowerSources" );
    contextMenu_addVariableTextCheckBox( s_CombatNumberCM, combatNumbersCM_checkMonitor, 0, combatNumbersCM_Monitor, 0, combatNumbersCM_MonitorText, 0 );
}

char * attribDesc_GetString( AttribDescription *pDesc, F32 fVal, Character *pchar )
{
    static char buffer[128];

    // Apply base defense modification to all defense types
    if(pDesc->eType == kAttribType_Cur && pDesc->offAttrib >= (int)offsetof(CharacterAttributes, fDefenseType) && pDesc->offAttrib < (int)offsetof(CharacterAttributes, fDefense))
    {
        AttribDescription * pBase = attribDescriptionGet( offsetof(CharacterAttributes, fDefense) );
        fVal += pBase->fVal;
        if( fVal < 0 )
            pDesc->buffOrDebuff = 1;
    }

    if(pDesc->buffOrDebuff==3)
        font_color(CLR_PARAGON,CLR_PARAGON);
    else if(pDesc->buffOrDebuff==1)
    {
        if((pDesc->eStyle == kAttribStyle_Magnitude) || (pDesc->eStyle == kAttribStyle_InversePercent))
            font_color(CLR_GREEN,CLR_GREEN);
        else
            font_color(CLR_RED,CLR_RED);
    }
    else if(pDesc->buffOrDebuff==2)
    {
        if((pDesc->eStyle == kAttribStyle_Magnitude) || (pDesc->eStyle == kAttribStyle_InversePercent))
            font_color(CLR_RED,CLR_RED);
        else
            font_color(CLR_GREEN,CLR_GREEN);
    }

    switch(pDesc->eStyle)
    {
        xcase kAttribStyle_None: 
        {
            if(pDesc->eType == kAttribType_Cur && stricmp(pDesc->pchName, "HitPoints")==0 )
                sprintf_s( buffer, 128, "%.2f (%.1f%%)", fVal, 100*fVal/pchar->attrMax.fHitPoints );
            else if(pDesc->eType == kAttribType_Cur && stricmp(pDesc->pchName, "Absorb")==0 )
                sprintf_s( buffer, 128, "%.2f (%.1f%%)", fVal, 100*fVal/pchar->attrMax.fHitPoints ); // Max HP on purpose
            else if(pDesc->eType == kAttribType_Cur && stricmp(pDesc->pchName, "Endurance")==0 )
                sprintf_s( buffer, 128, "%.2f (%.1f%%)", fVal, 100*fVal/pchar->attrMax.fEndurance );
            else if(pDesc->eType == kAttribType_Special && stricmp(pDesc->pchName, "Influence")==0 )
                sprintf_s( buffer, 128, "%s", itoa_with_commas_static(pchar->iInfluencePoints));
            else
                sprintf_s( buffer, 128, "%.2f", fVal );
        }
        xcase kAttribStyle_Integer: sprintf_s( buffer, 128, "%i", (int)fVal );
        xcase kAttribStyle_Percent: sprintf_s( buffer, 128, "%.2f%%", fVal*100 );
        xcase kAttribStyle_InversePercent: sprintf_s( buffer, 128, "%.2f%%", fVal?(-100)*fVal:0.0 );
        xcase kAttribStyle_Magnitude: sprintf_s( buffer, 128, "%.1f",fVal?(-1)*fVal:0.0 );
        xcase kAttribStyle_PercentMinus100:    sprintf_s( buffer, 128, "%.2f%%", (fVal-1)*100 );
        xcase kAttribStyle_Multiply: sprintf_s( buffer, 128, "%.2fx", (fVal) );
        xcase kAttribStyle_Distance:
        {
            if( stricmp( pDesc->pchName, "JumpHeight" ) == 0 )
                fVal *= 4;
            if( !getCurrentLocale() )
                return textStd( "PrecisionFt", fVal);
            else
                return textStd( "PrecisionM", fVal*.3048f );
        }
        xcase kAttribStyle_PerSecond:
        {
            // Regen and Recovery are weird
            if( stricmp( pDesc->pchName, "Regeneration" ) == 0 )
            {
                fVal = 5.f*fVal/(3.f);
                return textStd( "HPPerSec", fVal, .01f*fVal*pchar->attrMax.fHitPoints );
            }
            else if( stricmp( pDesc->pchName, "Recovery" ) == 0 )
            {
                fVal = 6.67f*fVal/(4.f); 
                return textStd( "EndPerSec", fVal, .01f*fVal*pchar->attrMax.fEndurance );
            }
            else if( stricmp( pDesc->pchName, "EnduranceConsumption" ) == 0 )
            {
                return textStd( "EndPerSec", (fVal/pchar->attrMax.fEndurance)*100.f, fVal );
            }

        }
        xcase kAttribStyle_Speed:
        {
            if( !getCurrentLocale() )
                return textStd( "MilesPerHour", fVal*21*(3600.f/5280.f) );
            else
                return textStd( "KMPerHour", fVal*21*(3600.f/3280.84f) );
        }
        xcase kAttribStyle_ResistanceDuration:
        {
            return textStd( "PercentDurationString",  (1.f/(1.f+fVal))*100.f );
        }
        xcase kAttribStyle_EnduranceReduction:
        {
             sprintf_s( buffer, 128, "%.2f%%", (1-(1/fVal+0.0001f))*-100.f );
        }
    }

    return buffer;
}

char * attribDesc_GetModString( AttribDescription *pDesc, F32 fVal, Character *pchar )
{
    static char buffer[128]; 
    F32 *pf = ATTRIB_GET_PTR(pchar->pclass->ppattrBase[0],pDesc->offAttrib);

     switch(pDesc->eStyle)
    {
        xcase kAttribStyle_None: 
        {
            if(fVal < 0)
                font_color(CLR_RED,CLR_RED);
            sprintf_s( buffer, 128, "%+.2f", fVal );
        }
        xcase kAttribStyle_Percent:
        {
            if(fVal < 0)
                font_color(CLR_RED,CLR_RED);
            sprintf_s( buffer, 128, "%+.2f%%", fVal*100 );
        }
        xcase kAttribStyle_InversePercent:
        {
            if(fVal > 0)
                font_color(CLR_RED,CLR_RED);
            sprintf_s( buffer, 128, "%+.2f%%", fVal?(-100)*fVal:0.0 );
        }
        xcase kAttribStyle_Magnitude:
        {
            if(fVal > 0)
                font_color(CLR_RED,CLR_RED);
            sprintf_s( buffer, 128, "%+.1f", fVal?(-1)*fVal:0.0 );
        }
        xcase kAttribStyle_Multiply:
        {
            if(fVal < 0)
                font_color(CLR_RED,CLR_RED);
            sprintf_s( buffer, 128, "%+.2fx", fVal );
        }
        xcase kAttribStyle_Integer:
        {
            sprintf_s( buffer, 128, "%i", (int)fVal );
        }
        xcase kAttribStyle_PercentMinus100:
        {
             if( stricmp( pDesc->pchName, "DamageType[0]" ) == 0 )
            {
                if( fVal < 0 )
                    font_color(CLR_RED,CLR_RED);
                sprintf_s( buffer, 128, "%.2f%%", (fVal)*100 );
            }
            else
            {
                if((fVal) < 0)
                    font_color(CLR_RED,CLR_RED);
                sprintf_s( buffer, 128, "%+.2f%%", (fVal)*100 );
            }
        }
        xcase kAttribStyle_Distance:
        {
            if( stricmp( "PerceptionRadius", pDesc->pchName) == 0)
            {
                if((fVal) < 0)
                    font_color(CLR_RED,CLR_RED);
                if( !getCurrentLocale() )
                    return textStd( "PrecisionFtPlus", (fVal)*(*pf) );
                else
                    return textStd( "PrecisionMPlus", (fVal)*(*pf) *.3048f );
            }
            else if( stricmp( "JumpHeight", pDesc->pchName) == 0 )
            {
                if((fVal) < 0)
                    font_color(CLR_RED,CLR_RED);

                if( !getCurrentLocale() )
                    return textStd( "PrecisionFtPlus", (fVal)*4 );
                else
                    return textStd( "PrecisionMPlus", (fVal)*4*.3048f );                
            }
            else
            {
                if((fVal) < 0)
                    font_color(CLR_RED,CLR_RED);
                if( !getCurrentLocale() )
                    return textStd( "PrecisionFtPlus", fVal );
                else
                    return textStd( "PrecisionMPlus", fVal );                
            }
        }
        xcase kAttribStyle_PerSecond:
        {
            if((fVal) < 0)
                font_color(CLR_RED,CLR_RED);
            if( stricmp( pDesc->pchName, "Regeneration" ) == 0 )
            {
                // This .25 comes from the class table,  class_GetNamedTableValue TODO: send down the template?
                fVal = (5.f*(fVal)/(3.f))*(*pf);                    
                return textStd( "HPPerSec", fVal, .01f*(fVal)*pchar->attrMax.fHitPoints );
            }
            else if( stricmp( pDesc->pchName, "Recovery" ) == 0 )
            {
                fVal = 6.67f*(fVal)/(4.f)*(*pf); 
                return textStd( "EndPerSec", fVal, .01f*fVal*pchar->attrMax.fEndurance );
            }
        }
        xcase kAttribStyle_Speed:
        {
            if((fVal) < 0)
                font_color(CLR_RED,CLR_RED);
            if( !getCurrentLocale() )
                return textStd( "MilesPerHourPlus",  (fVal)*21*(3600.f/5280.f) );
            else
                return textStd( "KMPerHourPlus",  (fVal)*21*(3600.f/3280.84f) );
        }
        xcase kAttribStyle_ResistanceDuration:
        {
            return textStd( "PercentDurationStringPlus",  (1.f-(1.f/(1.f+fVal)))*-100.f );
        }
        xcase kAttribStyle_EnduranceReduction:
        {
             if( fVal > 0 )
                sprintf_s( buffer, 128, "%+.2f%%",  (1-(1/(1+fVal)+0.0001f))*-100.f );
            else
                sprintf_s( buffer, 128, "%+.2f%%",  0.f );
        }
    }

    return buffer;
}


F32 displayAllAttribs(F32 x, F32 y, F32 wd, F32 z, F32 sc, Character * pchar)
{
    int i, j, subsize, size = eaSize(&g_AttribCategoryList.ppCategories);
    F32 curY = y;
    F32 num_wd = 0;
    AttribDescription * pBaseDefense = attribDescriptionGet( offsetof(CharacterAttributes, fDefense) ); // this will be handy later

    font_color(CLR_WHITE,CLR_WHITE);
    font(&game_12);

    // get width
    for( i = 0; i < size; i++ )
    {
        const AttribCategory * pCat = g_AttribCategoryList.ppCategories[i];
        if(pCat->bOpen)
        {
            subsize = eaSize(&pCat->ppAttrib);
            for( j = 0; j < subsize; j++ )
            {
                const AttribDescription * pDesc = pCat->ppAttrib[j];
                int k, contribsize = eaSize(&pDesc->ppContributers);
                if( num_wd < x+25*sc+str_wd(&game_12,sc,sc,pDesc->pchDisplayName) )
                    num_wd = x+25*sc+str_wd(&game_12,sc,sc,pDesc->pchDisplayName);

                for(k = 0; k < contribsize && !bHidePowers; k++)
                {
                    AttribContributer * pContrib = pDesc->ppContributers[k];
                    if( num_wd < x+35*sc+str_wd(&game_12,sc,sc,pContrib->ppow->pchDisplayName) )
                        num_wd = x+35*sc+str_wd(&game_12,sc,sc,pContrib->ppow->pchDisplayName);
                }
            }
        }
    }

    for( i = 0; i < size; i++ )
    {
        const AttribCategory * pCat = g_AttribCategoryList.ppCategories[i];
        AtlasTex * pExpander = pCat->bOpen? atlasLoadTexture("map_zoom_minus.tga"): atlasLoadTexture("map_zoom_plus.tga");
        CBox box;
        int frameColor = CLR_NORMAL_BACKGROUND;
        char tipstr[256];

        subsize = eaSize(&pCat->ppAttrib);
        font_color(CLR_WHITE,CLR_WHITE);
        BuildCBox( &box, x, curY, wd, 20*sc );
        sprintf( tipstr, "%sToolTip", pCat->pchDisplayName );
        addToolTipEx(  x, curY, wd,20*sc, tipstr, &s_CombatNumberParent, MENU_GAME, WDW_COMBATNUMBERS, 0 );
        if( mouseCollision(&box) )
            frameColor = CLR_MOUSEOVER_BACKGROUND;
        if( mouseClickHit(&box,MS_LEFT) )
        {
            AttribCategory* pCat_mutable = cpp_const_cast(AttribCategory*)(pCat);
            pCat_mutable->bOpen = !pCat->bOpen;
        }
        if( mouseClickHit(&box,MS_RIGHT) )
            contextMenu_display(s_CombatNumberCM);
        
        drawFlatFrameBox(PIX2, R10, &box, z, frameColor, frameColor);
        display_sprite( pExpander, x + 10*sc, curY + 10*sc - pExpander->height*sc/2, z, sc, sc, 0xffffffff );
        cprntEx( x + 10*sc + pExpander->width*sc, curY+10*sc, z, sc, sc, CENTER_Y, pCat->pchDisplayName );
        curY += 24*sc;

        if(pCat->bOpen)
            curY += 15*sc;

        for( j = 0; j < subsize; j++ )
        {
            AttribDescription * pDesc = pCat->ppAttrib[j];
            int k, contribsize = eaSize(&pDesc->ppContributers);
            bool bDefenseType = pDesc->eType == kAttribType_Cur && pDesc->offAttrib >= (int)offsetof(CharacterAttributes, fDefenseType) && pDesc->offAttrib < (int)offsetof(CharacterAttributes, fDefense);

            if( num_wd < x+25*sc+str_wd(&game_12,sc,sc,pDesc->pchDisplayName) )
                num_wd = x+25*sc+str_wd(&game_12,sc,sc,pDesc->pchDisplayName);

            // is this a defense type? add its contributers too
            if(bDefenseType)
                contribsize++;

             if(pCat->bOpen && !pDesc->bHide )        
            {
                font_color(CLR_WHITE,CLR_WHITE);

                BuildCBox( &box, x, curY-15*sc, wd, ((bHidePowers?0:contribsize)+1)*15*sc );
                addToolTipEx( x, curY-15*sc, wd, ((bHidePowers?0:contribsize)+1)*15*sc, pDesc->pchToolTip, &s_CombatNumberParent, MENU_GAME, WDW_COMBATNUMBERS, 0 );
                if( mouseClickHit(&box,MS_RIGHT) )
                    contextMenu_displayEx(s_CombatNumberCM, pDesc);

                cprntEx( x+15*sc, curY, z, sc, sc, 0, pDesc->pchDisplayName );
                cprntEx( num_wd, curY, z, sc, sc, NO_MSPRINT, "%s", attribDesc_GetString( pDesc, pDesc->fVal, pchar ) );
        
                if(pDesc->bShowBase && contribsize)
                {
                    F32 *pf = ATTRIB_GET_PTR(pchar->pclass->ppattrBase[0],pDesc->offAttrib);

                    curY += 15*sc;
                    font_color(0xffffffaa,0xffffffaa);
                    cprntEx( x+25*sc, curY, z, sc, sc, 0, "%s", textStd("BaseString") );

                    font_color(CLR_WHITE,CLR_WHITE);
                    if( stricmp( pDesc->pchName, "Regeneration" ) == 0 || stricmp( pDesc->pchName, "PerceptionRadius" ) == 0  )
                        cprntEx( num_wd, curY, z, sc, sc, NO_MSPRINT, "%s", attribDesc_GetModString( pDesc, 1, pchar ) );
                    else
                        cprntEx( num_wd, curY, z, sc, sc, NO_MSPRINT, "%s", attribDesc_GetModString( pDesc, *pf, pchar ) );
                }

                for(k = 0; k < contribsize && !bHidePowers; k++)
                {
                    AttribContributer * pContrib = 0;
                    const char * pchDisplayName;
                    F32 fMag=0;

                     char *str;
                    if( k < eaSize(&pDesc->ppContributers) )
                    {
                        pContrib = pDesc->ppContributers[k];
                        pchDisplayName = pContrib->ppow->pchDisplayName; 
                        fMag = pContrib->fMag;
                    }
                    else
                    {
                        pchDisplayName = pBaseDefense->pchDisplayName;
                        fMag = pBaseDefense->fVal;
                    }

                    curY += 15*sc;
                    font_color(0xffffffaa,0xffffffaa);
                    cprntEx( x+25*sc, curY, z, sc, sc, NO_MSPRINT, "%s", textStd(pchDisplayName) );

                    if( num_wd < x+35*sc+str_wd(&game_12,sc,sc, "%s", textStd(pchDisplayName)) )
                        num_wd = x+35*sc+str_wd(&game_12,sc,sc, "%s", textStd(pchDisplayName));

                    font_color(CLR_GREEN,CLR_GREEN);
                    str = attribDesc_GetModString( pDesc,fMag, pchar );
                      cprntEx( num_wd, curY, z, sc, sc, NO_MSPRINT, "%s", str );
    
                    font_color(CLR_GREY,CLR_GREY);

                    if(pContrib)
                    {
                        switch( pContrib->eSrcType)
                        {
                            xcase kAttribSource_Self:        cprntEx( num_wd+5*sc+str_wd(&game_12,sc,sc, "%s", str), curY, z, sc, sc, 0, "%s", textStd("PowerFromTarget", "SelfString") );
                            xcase kAttribSource_Critter:    cprntEx( num_wd+5*sc+str_wd(&game_12,sc,sc, "%s", str), curY, z, sc, sc, 0, "%s", textStd("PowerFromTarget", pContrib->pchSrcDisplayName) );
                            xcase kAttribSource_Player:        cprntEx( num_wd+5*sc+str_wd(&game_12,sc,sc, "%s", str), curY, z, sc, sc, 0, "%s", textStd("PowerFromTargetNoTrans", pContrib->pchSrcDisplayName) );
                        }
                    }
                }

                curY += 20*sc;
            }
            else
            {
                clearToolTipEx(pDesc->pchToolTip);
            }
        }
    }

    return curY-y;
}


int combatNumbersWindow(void)
{
    F32 x,y,z,wd,ht,sc, height=0;
    int color,bcolor;
     Entity *e = playerPtr();
    static ScrollBar sb = {WDW_COMBATNUMBERS, 0 };
    UIBox box;
    static F32 timer;

    if(!window_getDims( WDW_COMBATNUMBERS, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ) )
        return 0;

    initCombatNumbersCM();

    drawFrame(PIX3, R10, x, y, z, wd, ht, sc, color, bcolor);
    BuildCBox(&s_CombatNumberParent.box, x, y, wd, ht );
    uiBoxDefine(&box, x+PIX3*sc, y+PIX3*sc, wd-2*PIX3*sc, ht-2*PIX3*sc);

    clipperPush( &box );

    if(g_pAttribTarget)
    {
        font_color(CLR_RED,CLR_RED);
        font(&game_12);
        cprntEx(x+10*sc, y+20*sc-sb.offset,z,sc,sc,NO_MSPRINT, "%s", textStd( "ViewingAttributes", g_pAttribTarget->name ));

        if(D_MOUSEHIT==drawStdButton(x+wd-90*sc, y + 14*sc - sb.offset, z, 90*sc, 20*sc, color, "ClearAttribTarget", 1.f, 0 ) )
            cmdParse("clearAttributeView");

        y+=20*sc;
        height +=20*sc;
    }
    
    height += displayAllAttribs(x+10*sc,y+10*sc-sb.offset,wd-20*sc,z,sc,(g_pAttribTarget&&g_pAttribTarget->pchar)?g_pAttribTarget->pchar:e->pchar );
    clipperPop();

    doScrollBar( &sb, ht - 2*(R10+PIX3)*sc, height, wd - PIX3/2*sc, R10*sc, z, 0, 0 );

    return 0;
}


