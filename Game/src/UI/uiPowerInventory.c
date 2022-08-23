/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
// Abandon hope all ye who enter

#include <utilitieslib/assert/assert.h>

#include <utilitieslib/components/earray.h>         // for StructGetNum
#include <utilitieslib/utils/mathutil.h>
#include "sound/sound.h"
#include "player/player.h"
#include "entity/entity.h"         // for entity
#include "entity/powers.h"
#include "UI/uiFx.h"
#include "entity/origins.h"
#include "entity/character_base.h" // for pchar
#include "entity/character_level.h"
#include "cmdparse/cmdcommon.h"      // for TIMESTEP
#include "entity/attrib_names.h"
#include "bases/DetailRecipe.h"
#include "language/langClientUtil.h"

#include "UI/sprite/sprite_font.h"    // for font definitions
#include "UI/sprite/sprite_text.h"    // for font functions
#include "UI/sprite/sprite_base.h"    // for sprites
#include "graphics/textureatlas.h"

#include "UI/uiWindows.h"
#include "UI/uiInput.h"
#include "UI/uiInfo.h"
#include "UI/uiUtilGame.h"     // for draw frame
#include "UI/uiUtilMenu.h"     //
#include "UI/uiUtil.h"         // for color definitions
#include "UI/uiGame.h"         // for start_menu
#include "UI/uiTray.h"
#include "UI/uiChat.h"
#include "UI/uiCursor.h"
#include "UI/uiScrollBar.h"
#include "UI/uiNet.h"
#include "UI/uiToolTip.h"
#include "UI/uiContextMenu.h"
#include "UI/uiPet.h"
#include "UI/uiGift.h"

#include "UI/uiPowerInventory.h"
#include "clientcomm/clientcomm.h"
#include "gameComm/trayCommon.h"
#include "win/input.h"
#include "entity/entity_power_client.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "player/badges.h"
#include "entity/entPlayer.h"

//------------------------------------------------------------------------------------------------------------
// Definitions ///////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------------

ToolTip powerTip;

static ToolTipParent s_ttpPowerInventory    = {0};
static ContextMenu *gPowerListContext       = 0;
static ContextMenu *gSpecializationContext  = 0;

#define POWER_BAR_HT 22
#define SET_SPACER     5
#define SET_YSPACER     20

//------------------------------------------------------------------------------------------------------------
// Tray_obj //////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------------


// This function returns a pointer to the trayobj equipped with the power is it is in the current tray
//
// -AB: refactored for new cur tray functionality :2005 Jan 26 03:41 PM
TrayObj * power_getVisibleTrayObj( TrayObj * obj, int *tray, int *slot )
{
    Entity *e = playerPtr();
    int i, j;

    i = get_cur_tray( e );
    assert(e->pchar && e->pl);

    for( j = 0; j < TRAY_SLOTS; j++)
    {
        TrayObj *to = getTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, i, j, e->pchar->iCurBuild, false);
        if (!to)
            continue;

        if( obj->type == kTrayItemType_Power )
        {
            if( to->type == kTrayItemType_Power && obj->iset == to->iset && obj->ipow == to->ipow )
            {
                if( tray )    
                    *tray = i;
                if( slot )
                    *slot = j;
                return to;
            }
        }
        else if( obj->type == kTrayItemType_MacroHideName)
        {
            if( to->type == kTrayItemType_MacroHideName && stricmp(obj->shortName,to->shortName) == 0 && stricmp(obj->command,to->command) == 0 )
            {
                if ( tray )
                    *tray = i;
                if( slot )
                    *slot = j;
                return to;
            }
        }
    }
    return NULL;
}

// This function finds a pointer to the first empty slot in the visible tray
//
// -AB: refactored for new cur tray functionality :2005 Jan 26 03:41 PM
TrayObj * power_getEmptySlot( int *tray, int *num )
{
    Entity *e = playerPtr();
    int i, j;

    i = get_cur_tray( e );
    assert(e->pchar);
    
    for( j = 0; j < TRAY_SLOTS; j++)
    {
        int type = typeTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, i, j, e->pchar->iCurBuild);
        if( type == kTrayItemType_None )
        {
            *tray = i;
            *num = j;
            return getTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, i, j, e->pchar->iCurBuild, true);
        }
    }

    return NULL;
}

char *newbMasterMindDisplayStrings[3] = { "AttackString","FollowString","HeelString" };
char *newbMasterMindActions[3] = {"AttackString","FollowString","FollowString" };
char *newbMasterMindStances[3] = {"AggressiveString","AggressiveString","PassiveString"};
char *newbMasterMindHelpStrings[3] = { "AttackHelp","FollowHelp","HeelHelp" };
char *newbMasterMindIcons[3] = { "PetCommand_Action_Attack.tga","PetCommand_Action_Follow.tga","PetCommand_Stance_Passive.tga" };

BasePower newbMastermindMacros[3] = {0};
BasePower architectPowerMacro = {0};

void initnewbMasterMindMacros(void)
{
    int i;
    if( newbMastermindMacros[0].pchName )
        return;

    architectPowerMacro.pchName = strdup("ArchitectAccoladeDisplayName"); 
    architectPowerMacro.pchDisplayName = strdup("ArchitectAccoladeDisplayName");
    architectPowerMacro.pchDisplayHelp = strdup("Architect$$em MATablet");
    architectPowerMacro.pchDisplayShortHelp = strdup("ArchitectAccoladeShortHelp");
    architectPowerMacro.pchIconName = strdup("Architect_Accolade_Power.tga");

    for(i=0; i < ARRAY_SIZE(newbMastermindMacros); i++)
    {
        StuffBuff sb;

        // Construct the macros from parts so they work in non-english locales
        // The string created by initStuffBuff will stay around forever

        initStuffBuff(&sb,30);
        addStringToStuffBuff(&sb,"petcom_all ");
        addStringToStuffBuff(&sb,textStd(newbMasterMindActions[i]));
        addStringToStuffBuff(&sb," ");
        addStringToStuffBuff(&sb,textStd(newbMasterMindStances[i]));

        newbMastermindMacros[i].pchName = newbMasterMindDisplayStrings[i];
        newbMastermindMacros[i].pchDisplayName = newbMasterMindDisplayStrings[i];
        newbMastermindMacros[i].pchDisplayHelp = sb.buff;
        newbMastermindMacros[i].pchDisplayShortHelp = newbMasterMindHelpStrings[i];
        newbMastermindMacros[i].pchIconName = newbMasterMindIcons[i];
    }
}

char * masterMindHackGetHelpFromCommandName( char * cmd )
{
    int i;
    initnewbMasterMindMacros();

    for( i = 0; i < ARRAY_SIZE(newbMastermindMacros); i++ )
    {
        if( stricmp( cmd, newbMastermindMacros[i].pchDisplayHelp ) == 0)
        {
            return textStd(newbMastermindMacros[i].pchDisplayShortHelp);
        }
    }

    if( stricmp(cmd, architectPowerMacro.pchDisplayHelp ) == 0 )
            return textStd(architectPowerMacro.pchDisplayShortHelp);

    return cmd;
}

//------------------------------------------------------------------------------------------------------------
// Main functions /////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------------

// draws the small power and handles interaction with it
//
static void power_drawPower( const BasePower *pow, float x, float y, float z, float wd, float scale, int available, int i, int j )
{
    static AtlasTex * glow;
    static int texBindInit = 0;

    CBox box;
    Power *ppow = 0;

    TrayObj newObj = {0};
    static TrayObj to_tmp = {0};
    float glow_scale = 1.5f;
    float icon_scale = .5f;
    int color = available?CLR_WHITE:0xffffff44; // grey unavailable powers
    Entity *e = playerPtr();
    AtlasTex * icon = basepower_GetIcon(pow);
    bool bIsMacro = !pow->psetParent;
    bool bIsTempPower = bIsMacro?0:strcmp(pow->psetParent->pcatParent->pchName,"Temporary_Powers") == 0;

    assert(e->pchar);
    if( !texBindInit )
    {
        texBindInit = 1;
        glow = atlasLoadTexture( "healthbar_glow.tga" ); // this sprite seemed handy
    }

    // Temporary powers are special.
    if( bIsTempPower )
    {
        ppow = e->pchar->ppPowerSets[i]->ppPowers[j]; // temp power
    }
    else if( !bIsMacro )
    {
        ppow = character_OwnsPower( e->pchar, pow ); // the actual power pointer
    }

    if( ppow ) //make a try obj if we don't have one handy
        buildPowerTrayObj( &newObj, ppow->psetParent->idx, ppow->idx, ppow->ppowBase->eType == kPowerType_Auto );

     if( bIsMacro )
        buildMacroTrayObj( &newObj, pow->pchDisplayHelp, textStd(pow->pchName), pow->pchIconName, 1 );

    BuildCBox( &box, x, y, wd, (POWER_BAR_HT-1)*scale ); // collision box

    if (mouseCollision(&box))
    {
        addToolTip(&powerTip);
        setToolTip(&powerTip, &box, pow->pchDisplayShortHelp, &s_ttpPowerInventory, MENU_GAME, WDW_POWERLIST);
    }

    // a click was recieved, go to great lengths to drive icons around
    if( bIsMacro || (ppow && available) )
    {
        int autoPower = pow->eType == kPowerType_Auto;

        // glow behind power icon on mouseover
        if( mouseCollision( &box ) && !autoPower )
        {
            if( !isDown(MS_LEFT) )
                scale *= 1.1f;
            display_sprite( glow, x + (icon->width*icon_scale - glow->width*glow_scale)*scale/2, y + (icon->width*icon_scale - glow->width*glow_scale)*scale/2, z-1, glow_scale*scale, glow_scale*scale, CLR_WHITE );
            display_sprite( glow, x + (icon->width*icon_scale - glow->width*glow_scale)*scale/2, y + (icon->width*icon_scale - glow->width*glow_scale)*scale/2, z-1, glow_scale*scale, glow_scale*scale, CLR_WHITE );
        }

        // context menu
        if( mouseClickHit( &box, MS_RIGHT ) )
        {
            int x, y;
            rightClickCoords( &x, &y );
            if(bIsMacro)
                buildMacroTrayObj( &to_tmp, pow->pchDisplayHelp, textStd(pow->pchName), pow->pchIconName, 1 );
            else
                buildPowerTrayObj( &to_tmp, ppow->psetParent->idx, ppow->idx, 0 );
            contextMenu_set( gPowerListContext, x, y, &to_tmp );
        }

        // a click was recieved, go to great lengths to drive icons around
        if( mouseLeftDrag( &box ) )
        {
            if(bIsMacro)
            {
                trayobj_startDragging( &newObj, icon, NULL );
                cursor.drag_obj.islot = -1;         // tag the slot this power is coming from
                cursor.drag_obj.itray = -1;
                cursor.drag_obj.icat = -1;
            }
            else
                tray_dragPower( ppow->psetParent->idx, ppow->idx, icon );
        }
        else 
        if( mouseClickHit( &box, MS_LEFT ) && !autoPower )
        {
            if(inpLevel(INP_CONTROL)&& !bIsMacro) // if ctrl key is down set as auto-power
            {
                if( isMenu(MENU_GAME) )
                {
                    trayslot_findAndSetDefaultPower(e, ppow->ppowBase->pchName);
                }
            }
            else
            {
                int tray, num;
                float tx, ty, tz, twd, tht, tsc;
                int tclr, bcolor;
                TrayObj * ts = power_getVisibleTrayObj( &newObj, &tray, &num );

                if( ts ) // the power is in the tray, so remove it
                {
                    TrayObj empty = {0};

                    // if the tray is closed, move icon to status window
                    if ( !window_getDims( WDW_TRAY, &tx, &ty, &tz, &twd, &tht, &tsc, &tclr, &bcolor ) )
                    {
                        childButton_getCenter( wdwGetWindow(WDW_STAT_BARS), 1,  &tx, &ty );
                        tsc = 0;
                    }
                    else // otherwise get tray coordinates
                    {
                        AtlasTex * left_arrow = atlasLoadTexture( "tray_arrow_L.tga" );
                        tx += (left_arrow->width + 15 + num * BOX_SIDE + BOX_SIDE/2)*scale/1.1;
                        ty += (BOX_SIDE/2 + TRAY_YOFF)*scale/1.1;
                    }

                    clearTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, tray, num, e->pchar->iCurBuild);
                    //trayobj_copy( ts, &empty, TRUE ); //remove from tray and drive icon
                    movingIcon_add( icon, tx, ty, tsc, x + icon->width*icon_scale*scale/2, y + icon->height*icon_scale*scale/2, icon_scale*scale, z+50 );
                }
                else // the power is not in the tray, add it
                {
                    TrayObj *dest = power_getEmptySlot( &tray, &num ); // find a spot for it

                    // if tray is closed, move to status window
                    if ( !window_getDims( WDW_TRAY, &tx, &ty, &tz, &twd, &tht, &tsc, &tclr, &bcolor ) )
                    {
                        childButton_getCenter( wdwGetWindow(WDW_STAT_BARS), 1,  &tx, &ty );
                        tsc = 0;
                    }
                    else
                    {
                        AtlasTex * left_arrow = atlasLoadTexture( "tray_arrow_L.tga" );
                        tx += (left_arrow->width + 15 + num * BOX_SIDE + BOX_SIDE/2)*scale/1.1;
                        ty += (BOX_SIDE/2 + TRAY_YOFF)*scale/1.1;
                    }

                    if( dest )
                    {
                        movingIcon_add( icon, x + icon->width*icon_scale*scale/2, y + icon->height*icon_scale*scale/2, icon_scale*scale, tx, ty, tsc, z+50 );
                        trayobj_copy( dest, &newObj, TRUE );
                    }
                }
            }
        }

        // finally draw icon
        trayslot_drawIcon( &newObj, x + icon->width*icon_scale*scale/2, y + icon->height*icon_scale*scale/2, z, icon_scale*scale, 1 );

        if (ppow && !ppow->bEnabled)
        {
            color = CLR_RED;
        }
    }
    else
    {
        display_sprite( icon, x, y, z, icon_scale*scale, icon_scale*scale, color );
    }

    // print the name
    font( &game_9 );
    font_color( color, color );
    prnt( x + icon->width*icon_scale*scale + 5*scale, y + 12*scale, z, scale, scale, pow->pchDisplayName );

}

// draw a set of powers, frame included
//
static float power_drawSet( PowerSet *pset, float x, float y, float z, float wd, float scale, int color, int iset, float winY, float winHt )
{
    float height = 0;
    int i;
    Entity *e = playerPtr();

    set_scissor( TRUE );
    scissor_dims( x + PIX3, winY + PIX3, wd - PIX3*2, winHt - PIX3*2 );


    if( !pset ) // mastermind newb macros
    {
        for( i = 0; i < ARRAY_SIZE(newbMastermindMacros); i++ )
        {
            power_drawPower( &newbMastermindMacros[i], x + SET_SPACER*scale, y + SET_YSPACER*scale + height, z, wd - SET_SPACER*scale, scale, true, 9, i);
            height += POWER_BAR_HT*scale;
        }
    }
    else if( stricmp(pset->psetBase->pcatParent->pchName, "Temporary_Powers")==0 ) // show only powers player owns in the set
    {
        int idx;
        if( stricmp(pset->psetBase->pchName, "Accolades") == 0 && // glorious architect hack
            badge_GetIdxFromName( "ArchitectAccolade", &idx ) && BADGE_IS_OWNED( e->pl->aiBadges[idx] ) )
        {
            power_drawPower( &architectPowerMacro, x + SET_SPACER*scale, y + SET_YSPACER*scale + height, z, wd - SET_SPACER*scale, scale, true, 9, 0);
            height += POWER_BAR_HT*scale;
        }

        for( i = 0; i < eaSize(&pset->ppPowers); i++ )
        {
            if (power_ShowInInventory(pset->pcharParent, pset->ppPowers[i]))
            {
                power_drawPower( pset->ppPowers[i]->ppowBase, x + SET_SPACER*scale, y + SET_YSPACER*scale + height, z, wd - SET_SPACER*scale, scale, true, pset->idx, i);
                height += POWER_BAR_HT*scale;
            }
        }
    }
    else // show all powers in set, even if not owned.
    {
    
        for( i = 0; i < eaSize(&pset->psetBase->ppPowers); i++ )
        {
            if (basepower_ShowInInventory(pset->pcharParent, pset->psetBase->ppPowers[i]))
            {
                bool bShow = false;

                if(character_OwnsPower( e->pchar, pset->psetBase->ppPowers[i] )!=NULL ||
                    character_IsAllowedToHavePower( e->pchar, pset->psetBase->ppPowers[i] ) ||
                    ( baseset_IsSpecialization( pset->psetBase ) && character_WillBeAllowedToSpecializeInPowerSet( e->pchar, pset->psetBase ) ) )
                {
                    bShow = true;
                }

                 if(bShow)
                {
                    power_drawPower( pset->psetBase->ppPowers[i], x + SET_SPACER*scale, y + SET_YSPACER*scale + height, z, wd - SET_SPACER*scale, scale, (int)character_OwnsPower( e->pchar, pset->psetBase->ppPowers[i] ), iset, i );
                    height += POWER_BAR_HT*scale;
                }
            }
        }
    }

    // frame
    if( height > 0 )
    {
        scissor_dims( x, winY + PIX3*scale, wd, winHt - PIX3*2*scale );
        drawFrame( PIX3, R10, x, y + SET_YSPACER*scale/2, z, wd, height + SET_YSPACER*scale/2, scale, 0xffffff88, 0xffffff22 );

        // frame title
        font( &gamebold_9 );
        font_color( CLR_WHITE, CLR_BLUE );

        if( pset )    
            cprnt( x + wd/2, y+16*scale, z, scale, scale, pset->psetBase->pchDisplayName );
        else
            cprnt( x + wd/2, y+16*scale, z, scale, scale, "BasicPetCom" );

        set_scissor( FALSE );
        return height + SET_YSPACER*scale;
    }

    set_scissor( FALSE );
    return 0;
}


//------------------------------------------------------------------------------------------------------------
// Power Inventory Window ////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------------

int powersWindow()
{
    Entity *e = playerPtr();
    float x, y, z, wd, ht, scale;
    float primY, secY, poolY;
    static float primHt, secHt, poolHt;
    float max_ht;
    int i, j, catidx, color, bcolor;
    Category cat;
    static ScrollBar sb = {WDW_POWERLIST, 0 };
    static int init = 0;
    static int bSkill;
    int bAddedMasterMindPowers = 0;
    int bAddedArchitectPowers = 0;
    PowerSet tempSpecSet;
    float tht;

    if( !window_getDims( WDW_POWERLIST, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ) )
        return 0;

    // need to do this once
    if( !init )
    {
        gPowerListContext = contextMenu_Create(NULL);
        contextMenu_addVariableText( gPowerListContext, traycm_PowerText, 0);
        contextMenu_addVariableText( gPowerListContext, traycm_PowerInfo, 0);
        contextMenu_addVariableTextVisible( gPowerListContext, traycm_showExtra, 0, traycm_PowerExtra, 0 );
        contextMenu_addCode( gPowerListContext, alwaysAvailable, 0, traycm_Add,        0, "CMAddToTray", 0  );
        contextMenu_addCode( gPowerListContext, alwaysAvailable, 0, traycm_Remove,    0, "CMRemoveFromTray", 0  );
        contextMenu_addCode( gPowerListContext, alwaysAvailable, 0, traycm_Info,    0, "CMInfoString", 0  );
        contextMenu_addCode( gPowerListContext, traycm_DeletableTempPower, 0, traycm_DeletePower,    0, "CMPowerDelete", 0  );
        gift_addToContextMenu(gPowerListContext);
        initnewbMasterMindMacros();
        sb.wdw = WDW_POWERLIST;
        init = TRUE;
    }

    if(  eaSize(&e->pchar->ppPowerSets) > 10 )
        primY = secY = poolY = y - sb.offset;
    else
    {
        if( primHt > ht - 2*R10*scale )
            primY =  y - MIN( sb.offset, primHt-(ht - 2*R10*scale) );
        else
            primY = y;

        if( secHt > ht - 2*R10*scale )
            secY =  y - MIN( sb.offset, secHt-(ht - 2*R10*scale) );
        else
            secY = y;

        if( poolHt > ht - 2*R10*scale )
            poolY =  y - MIN( sb.offset, poolHt-(ht - 2*R10*scale) );
        else
            poolY = y;
    }

    drawFrame( PIX3, R10, x, y, z, wd, ht, scale, color, bcolor );
     BuildCBox(&s_ttpPowerInventory.box, x, y, wd, ht);

    primHt = secHt = poolHt = 0;
    for( i = 0; i < eaSize(&e->pchar->ppPowerSets); i++ )
    {
        if(powerset_ShowInInventory(e->pchar->ppPowerSets[i]))
        {
            if( e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Primary] )
            {
                tht = power_drawSet( e->pchar->ppPowerSets[i], x + 10*scale, primY, z, (wd-20*scale)/3 - 5*scale, scale, color, i, y, ht );
                primHt += tht;
                primY += tht;
            }
            else if( e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Secondary] )
            {
                tht = power_drawSet( e->pchar->ppPowerSets[i], x + wd/3 + 5*scale, secY, z, (wd-20*scale)/3 - 10*scale, scale, color, i, y, ht );
                secHt += tht;
                secY += tht;
            }
            else
            {
                tht = power_drawSet( e->pchar->ppPowerSets[i], x + wd - 10*scale - (wd-20*scale)/3, poolY, z, (wd-20*scale)/3 - 5*scale, scale, color, i, y, ht );
                 poolHt += tht;
                poolY += tht;

                // hack to add in pet commands after inherent powerset
                 if( strstriConst( e->pchar->ppPowerSets[i]->psetBase->pchName, "inherent" ) && playerIsMasterMind() && !bAddedMasterMindPowers )
                {
                    tht = power_drawSet( NULL, x + wd - 10*scale - (wd-20*scale)/3, poolY, z, (wd-20*scale)/3 - 5*scale, scale, color, i, y, ht );
                    poolHt += tht;
                    poolY += tht;
                    bAddedMasterMindPowers = true;
                }
            }
        }
    }

    // we show specialization sets even if the character doesn't have them
    // yet, so they have no powerset on them. as such we need a placeholder
    // power set. the index is ultimately used for storing tooltips, so we
    // set it off the end of the array so as not to conflict
    tempSpecSet.pcharParent = e->pchar;
    tempSpecSet.idx = eaSize( &e->pchar->ppPowerSets );
    tempSpecSet.ppPowers = NULL;
    tempSpecSet.iLevelBought = e->pchar->iLevel;

    // add in any specialization sets for the various categories
    for( catidx = 0; catidx < 2; catidx++ )
    {
        if (catidx == 0)
            cat = kCategory_Primary;
        if (catidx == 1)
            cat = kCategory_Secondary;

        for( i = 0; i < eaSize( &e->pchar->pclass->pcat[cat]->ppPowerSets ); i++ )
        {
            const BasePowerSet *psetBase = e->pchar->pclass->pcat[cat]->ppPowerSets[i];

            if (character_WillBeAllowedToSpecializeInPowerSet( e->pchar, psetBase ) )
            {
                bool found = false;

                for( j = 0; !found && j < eaSize( &e->pchar->ppPowerSets ); j++)
                {
                    if( psetBase == e->pchar->ppPowerSets[j]->psetBase )
                    {
                        found = true;
                    }
                }

                if( !found )
                {
                    tempSpecSet.psetBase = psetBase;

                    if( cat == kCategory_Primary )
                    {
                        tht = power_drawSet( &tempSpecSet, x + 10*scale, primY, z, (wd-20*scale)/3 - 5*scale, scale, color, tempSpecSet.idx, y, ht );
                        primHt += tht;
                        primY += tht;
                    }
                    else if( cat == kCategory_Secondary )
                    {
                        tht = power_drawSet( &tempSpecSet, x + wd/3 + 5*scale, secY, z, (wd-20*scale)/3 - 10*scale, scale, color, tempSpecSet.idx, y, ht );
                        secHt += tht;
                        secY += tht;
                    }
                }

                tempSpecSet.idx++;
            }
        }
    }

    max_ht = MAX( primHt, MAX(secHt,poolHt) );
     doScrollBar( &sb, ht - 2*R10*scale, max_ht-11*scale, wd - PIX3/2*scale, R10*scale, z, &s_ttpPowerInventory.box, 0 );

    return 0;
}

/* End of File */
