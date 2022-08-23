/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/wininclude.h>

#include <utilitieslib/stdtypes.h>

#include "UI/uiGame.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiUtilGame.h"
#include "uiCombineSpec.h"
#include "uiPowers.h"
#include "UI/uiInput.h"
#include "UI/uiNet.h"
#include "UI/uiFx.h"
#include "UI/uiEditText.h"
#include "UI/uiScrollBar.h"
#include "UI/uiPowerInventory.h"
#include "UI/uiLevelSpec.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "cmdparse/cmdgame.h"

#include "player/player.h"
#include "entity/entVarUpdate.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/powers.h"
#include "entity/origins.h"
#include "entity/classes.h"
#include <utilitieslib/components/earray.h>
#include "win/win_init.h"
#include <utilitieslib/utils/timing.h>
#include "clientcomm/clientcomm.h"
#include "storyarc/contactCommon.h"
#include "graphics/ttFontUtil.h"
#include "sound/sound.h"
#include "graphics/textureatlas.h"
#include "entity/entity.h"
#include "UI/uiInfo.h"
#include "UI/uiPowerInfo.h"
#include <utilitieslib/utils/file.h>
#include "UI/uiClipper.h"

int gNumEnhancesLeft = 0;

//-------------------------------------------------------------

EnhancementAnim ** enhancementAnimQueue = 0;

#ifndef TEST_CLIENT

#define ENHANCE_MOVE_SPEED 10
#define ENHANCE_GROW_SPEED .1f
#define FRAME_WIDTH  320
#define FRAME_SPACE  (DEFAULT_SCRN_WD - 3*FRAME_WIDTH)/4
#define SL_FRAME_TOP 92
#define FRAME_HEIGHT 623
#define POOL_HEIGHT  423
//
//
int enhancementAnim_add( Power * pow )
{
    int i, inqueue = 0;
    EnhancementAnim *ea;

    if( !enhancementAnimQueue )
        eaCreate( &enhancementAnimQueue );

    if( !gNumEnhancesLeft )
        return 0;

    for( i = 0; i < eaSize(&enhancementAnimQueue); i++ )
    {
        if( pow == enhancementAnimQueue[i]->pow )
        {
            enhancementAnimQueue[i]->scale[enhancementAnimQueue[i]->count] = 0;
            enhancementAnimQueue[i]->count++;
            gNumEnhancesLeft--;
            return 1;
        }
    }

    ea = calloc( 1, sizeof(EnhancementAnim) );
    ea->expanding = 1;
    ea->pow = pow;
    ea->count++;
    gNumEnhancesLeft--;
    eaPush( &enhancementAnimQueue, ea );

    return 1;
}

//
//
void enhancementAnim_update(PowerSystem eSys)
{
    int i, j;
    float wd = SPEC_WD;

    if( !enhancementAnimQueue )
        return;

    for( i = 0; i < eaSize(&enhancementAnimQueue); i++ )
    {
        EnhancementAnim *ea = enhancementAnimQueue[i];

        if( ea->expanding )
        {
            if( ea->xoffset < wd*ea->count )
            {
                ea->xoffset += TIMESTEP*ENHANCE_MOVE_SPEED;
                if( ea->xoffset >= wd*ea->count )
                    ea->xoffset = wd*ea->count;
            }

            if( ea->xoffset > wd*ea->count )
            {
                ea->xoffset -= TIMESTEP*ENHANCE_MOVE_SPEED;
                if( ea->xoffset <= wd*ea->count )
                    ea->xoffset = wd*ea->count;
            }

            for( j = 0; j < ea->count; j++ )
            {
                if( ea->shrink[j] )
                {
                    ea->scale[j]  -= TIMESTEP*ENHANCE_GROW_SPEED;
                    if( ea->scale[j] < 0.f )
                    {
                        ea->scale[j] = 1.f;
                        ea->count--;
                        electric_removeById( ea->id[ea->count] );
                        gNumEnhancesLeft++;
                        ea->shrink[j] = 0;
                    }
                }
                else
                {
                    ea->scale[j]  += TIMESTEP*ENHANCE_GROW_SPEED;
                    if( ea->scale[j] > 1.f )
                        ea->scale[j] = 1.f;
                }
            }
        }
        else
        {
            electric_clearAll();

            if( ea->count > 0 )
            {
                if( ea->scale[ea->count-1] > 0 )
                {
                    ea->scale[ea->count-1] -= TIMESTEP*ENHANCE_GROW_SPEED;
                    ea->count--;
                    gNumEnhancesLeft++;
                }
            }
            else
            {
                ea->xoffset -= TIMESTEP*ENHANCE_MOVE_SPEED;
                if( ea->xoffset < 0 )
                {
                    // FIXEAR: Is this safe considering the loop index?
                    EnhancementAnim *tmp = eaRemove( &enhancementAnimQueue, i);
                    if( tmp )
                        free( tmp );
                }
            }
        }
    }
}

//
//
EnhancementAnim *enhancementAnim( Power *pow )
{
    int i;

    for( i = 0; i < eaSize(&enhancementAnimQueue); i++ )
    {
        if( pow == enhancementAnimQueue[i]->pow )
            return enhancementAnimQueue[i];
    }

    return NULL;
}

void enhancementAnim_clear()
{
    while(  eaSize(&enhancementAnimQueue) )
    {
        EnhancementAnim *tmp = eaRemove( &enhancementAnimQueue, 0);
        if( tmp )
            free( tmp );
    }
}

void drawPowerAndEnhancements( Character *pchar, Power * pow, int x, int y, int z, float scale, int available, int *offset, int selectable, float screenScaleX, float screenScaleY, CBox *baseClipBox)
{
    int i, hit, tx = 0;
    EnhancementAnim *ea;
    AtlasTex *icon = atlasLoadTexture(pow->ppowBase->pchIconName);
    float screenScale = MIN(screenScaleX, screenScaleY);
    if( stricmp( icon->name, "white" ) == 0 )
        icon = atlasLoadTexture( "Power_Missing.tga" );

    ea = enhancementAnim( pow );
    if( ea )
    {
        //enahncement_NewSlot( ea );
        for( i = 0; i < ea->count; i++ )
        {
            CBox box;
            drawMenuEnhancement( NULL, x*screenScaleX + (-2 + POWER_OFFSET)*screenScaleX + (SPEC_WD*(i+1))*scale*screenScale, y*screenScaleY + 20*scale*screenScale, z+100, screenScale*scale*.5f*ea->scale[i], kBoostState_Selected, 0, true, MENU_LEVEL_SPEC, true);
            BuildCBox( &box, x*screenScaleX + (-2 + POWER_OFFSET)*screenScaleX + (SPEC_WD*(i+1))*scale*screenScale - SPEC_WD/2, y*screenScaleY + 20*scale*screenScale - SPEC_WD/2, SPEC_WD*scale*screenScale, SPEC_WD*scale*screenScale );
            if( mouseClickHit( &box, MS_LEFT) )
            {
                ea->shrink[i] = !ea->shrink[i];
                collisions_off_for_rest_of_frame = 1;
            }
        }
        tx = ea->xoffset;
    }

       hit = drawCheckBar( (x+POWER_OFFSET)*screenScaleX, y*screenScaleY, z, scale*screenScale, POWER_WIDTH*screenScaleX, pow->ppowBase->pchDisplayName, 0, selectable, 0, 1, icon );

    if (hit == D_MOUSEOVER)
    { 
        g_pPowMouseover = pow;
        info_combineMenu( pow, 0, 0, 0, 0 );
    }

     if( hit == D_MOUSEHIT )
    {
         if( !gNumEnhancesLeft )
        {
            fadingText( DEFAULT_SCRN_WD/2,( SL_FRAME_TOP+FRAME_HEIGHT-5), z+250, 1.5f*scale*screenScale, CLR_WHITE, CLR_WHITE, 30, "NoSlots" );
        }
        else if( (ea
                    && (ea->count+pow->iNumBoostsBought >= MAX_BOOSTS_BOUGHT
                        || ea->count+eaSize(&pow->ppBoosts) >= pow->ppowBase->iMaxBoosts))
                || pow->iNumBoostsBought >= MAX_BOOSTS_BOUGHT
                || eaSize(&pow->ppBoosts) >= pow->ppowBase->iMaxBoosts
                || eaiSize(&pow->ppowBase->pBoostsAllowed)<=0)
        {
            fadingText( (x+POWER_OFFSET + POWER_WIDTH/2), (y+15), z+250, 1.5f*scale*screenScale, CLR_RED, CLR_RED, 30, "FullString" );
        }
        else
        {
            if( enhancementAnim_add( pow ) )
            {
                CBox clipBox;
                BuildCBox(&clipBox, baseClipBox->lx/screenScaleX, baseClipBox->ly/screenScaleY, baseClipBox->hx/screenScaleX, baseClipBox->hy/screenScaleY);
                clipperPushCBox(&clipBox);
                ea = enhancementAnim( pow );
                 ea->id[ea->count-1] = electric_init( x + (-2 + POWER_OFFSET) + (SPEC_WD*ea->count)*scale*screenScale/screenScaleX, y + 20*scale*screenScale/screenScaleY + *offset, z+190, MIN(1.f, .8*scale*screenScale), 0, -1, 1, offset );
                clipperPop();
                sndPlay( "powerselect2", SOUND_GAME );
            }
        }
    }

    for( i = 0; i < eaSize(&pow->ppBoosts); i++ )
    {
        // do some availability checks here
        int clickable = TRUE;

        if( !available )
            clickable = FALSE;

        drawMenuPowerEnhancement( pchar, pow->psetParent->idx, pow->idx, i, x + (-2 + POWER_OFFSET) + (tx+((i+1)*SPEC_WD))*scale*screenScale/screenScaleX,
                                    y + 20*scale*screenScale/screenScaleY, z+40, scale*.5f, CLR_WHITE, 0, screenScaleX, screenScaleY);
    }
}

//-----------------------------------------------------------------------------------------------------
// Main Loop //////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------
static int init = FALSE;

void specMenu()
{
    Entity *e = playerPtr();
    int i, color;
    bool bFakedLevel = false;
    static int s_ShowCombatNumbersHere = 0;
    static ScrollBar sb[3] = { 0 };
    CBox box[3];

    float primY = SL_FRAME_TOP + 50 - POWER_SPEC_HT - sb[0].offset, secY = SL_FRAME_TOP + 50 - POWER_SPEC_HT - sb[1].offset, poolY = SL_FRAME_TOP + 50 - POWER_SPEC_HT - sb[2].offset;
    int screenScalingDisabled = is_scrn_scaling_disabled();
    float screenScaleX = 1.f, screenScaleY = 1.f, screenScale;

    calc_scrn_scaling(&screenScaleX, &screenScaleY);
    screenScale = MIN(screenScaleX, screenScaleY);

    // HACK: Pretend that we're already a level higher to get the right options
    if(character_CalcExperienceLevel(e->pchar) > e->pchar->iLevel)
    {
        e->pchar->iLevel++;
        bFakedLevel = true;
    }

    drawBackground( NULL ); //atlasLoadTexture("_Enhancement_customizepower_MOCKUP.tga") );
    set_scrn_scaling_disabled(1);
    toggle_3d_game_modes( SHOW_NONE );
    drawMenuTitle( 10*screenScaleX, 30*screenScaleY, 20, "AddEnhancementSlot", screenScale, !init );

    if( !init )
    {
        init = TRUE;
        gNumEnhancesLeft = character_CountBuyBoostForLevel( e->pchar, e->pchar->iLevel);
        powerInfoSetLevel(e->pchar->iLevel);
        powerInfoSetClass(e->pchar->pclass);
    }

    // headers
    font( &gamebold_18 );
    drawMenuHeader( (DEFAULT_SCRN_WD - FRAME_WIDTH/2 - FRAME_SPACE)*screenScaleX, (SL_FRAME_TOP-20)*screenScaleY, 20, CLR_WHITE, SELECT_FROM_UISKIN( 0x88b6ffff, 0xaaaaaaff, CLR_WHITE ), "PoolTitle", screenScale  );

    // the frames
    drawMenuFrame( R12, FRAME_SPACE*screenScaleX, SL_FRAME_TOP*screenScaleY, 10, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // left
    BuildCBox( &box[0], FRAME_SPACE*screenScaleX, SL_FRAME_TOP*screenScaleY, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY );
    drawMenuFrame( R12, (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, SL_FRAME_TOP*screenScaleY, 10, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // middle
    BuildCBox( &box[1], (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, SL_FRAME_TOP*screenScaleY, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY );
    drawMenuFrame( R12, (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE)*screenScaleX, SL_FRAME_TOP*screenScaleY, 10, FRAME_WIDTH*screenScaleX, POOL_HEIGHT*screenScaleY, CLR_WHITE, 0x00000088, 0 ); // right
    BuildCBox( &box[2], (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE)*screenScaleX, SL_FRAME_TOP*screenScaleY, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY );

    font( &game_12 );
    if( !gNumEnhancesLeft )
    {
        font_color( CLR_GREEN, CLR_GREEN );
        color = CLR_GREEN;
    }
    else
    {
        font_color( CLR_RED, CLR_RED );
        color = CLR_RED;
    }

    drawFlatFrame( PIX3, R10, (DEFAULT_SCRN_WD/2 - 100)*screenScaleX, (SL_FRAME_TOP+FRAME_HEIGHT-20)*screenScaleY, 50, 200*screenScaleX, 30*screenScaleY, screenScale, color, CLR_BLACK ); // middle
    cprntEx( (DEFAULT_SCRN_WD/2)*screenScaleX, (SL_FRAME_TOP+FRAME_HEIGHT-5)*screenScaleY, 55, screenScale, screenScale, (CENTER_X|CENTER_Y), "EnhancmentsAllocated", gNumEnhancesLeft );

    // 2 buttons to pick the frame to show the combat numbers
    if( D_MOUSEHIT == drawStdButton( (FRAME_SPACE+FRAME_WIDTH/2)*screenScaleX, (SL_FRAME_TOP-22)*screenScaleY, 45, 120*screenScaleX, 30*screenScaleY, SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE ), (s_ShowCombatNumbersHere==1)?"HidePowerNumbers":"ShowCombatNumbersHere", screenScale, 0 ))
    {
        if(s_ShowCombatNumbersHere==1)
            s_ShowCombatNumbersHere = 0;
        else
            s_ShowCombatNumbersHere = 1;
        electric_clearAll();
    }
    if( D_MOUSEHIT == drawStdButton( (DEFAULT_SCRN_WD/2)*screenScaleX , (SL_FRAME_TOP-22)*screenScaleY, 45, 120*screenScaleX, 30*screenScaleY, SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE ), (s_ShowCombatNumbersHere==2)?"HidePowerNumbers":"ShowCombatNumbersHere", screenScale, 0 ))
    {
        if(s_ShowCombatNumbersHere==2)
            s_ShowCombatNumbersHere = 0;
        else
            s_ShowCombatNumbersHere = 2;
        electric_clearAll();
    }

    // The zeroth slot in power sets is for temporary powers, which don't have specs
    for( i = 1; i < eaSize(&e->pchar->ppPowerSets); i++ )
    {
        int j, top;
        CBox clipBox;
        if( e->pchar->ppPowerSets[i]->psetBase->bShowInManage )
        {
            if( e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Primary] )
            {
                BuildCBox(&clipBox, 0, (SL_FRAME_TOP+PIX4)*screenScaleY, 2000*screenScaleX, (FRAME_HEIGHT - PIX4*2 - 1)*screenScaleY );
                clipperPushCBox(&clipBox);
                if( s_ShowCombatNumbersHere == 1)
                    menuDisplayPowerInfo( FRAME_SPACE*screenScaleX, SL_FRAME_TOP*screenScaleY, 30, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY,  g_pPowMouseover?g_pPowMouseover->ppowBase:0, g_pPowMouseover, POWERINFO_BASE  );
                else
                {
                    primY += POWER_SPEC_HT;
                    top = primY;

                    setHeadingFontFromSkin(1);
                    cprnt( (FRAME_SPACE+FRAME_WIDTH/2)*screenScaleX, (top-23)*screenScaleY, 20, screenScale, screenScale, e->pchar->ppPowerSets[i]->psetBase->pchDisplayName );

                    for( j = 0; j < eaSize(&e->pchar->ppPowerSets[i]->ppPowers); j++ )
                    {
                        Power *pow = e->pchar->ppPowerSets[i]->ppPowers[j];
                        if( pow && pow->ppowBase->bShowInManage )
                        {
                            drawPowerAndEnhancements( e->pchar, pow, FRAME_SPACE, primY, 20, 1.f, FALSE, &sb[0].offset, TRUE, screenScaleX, screenScaleY, &clipBox );
                            primY += POWER_SPEC_HT;
                        }
                    }

                    // draw the small inner frame
                    drawFrame( PIX2, R6, (FRAME_SPACE + POWER_OFFSET)*screenScaleX, (top - 30)*screenScaleY, 5, (POWER_WIDTH - 10)*screenScaleX, (primY - top - 35)*screenScaleY, screenScale, 0xffffffaa, SELECT_FROM_UISKIN( 0x0000ff44, 0xff000044, 0x00000066 ) );
                }
                clipperPop();
            }
            else if( e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Secondary] )
            {
                BuildCBox(&clipBox, 0, (SL_FRAME_TOP+PIX4)*screenScaleY, 2000*screenScaleX, (FRAME_HEIGHT - PIX4*2 - 1)*screenScaleY );
                clipperPushCBox(&clipBox);
                if( s_ShowCombatNumbersHere == 2 )
                    menuDisplayPowerInfo( (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2)*screenScaleX, SL_FRAME_TOP*screenScaleY, 30, FRAME_WIDTH*screenScaleX, FRAME_HEIGHT*screenScaleY,  g_pPowMouseover?g_pPowMouseover->ppowBase:0, g_pPowMouseover, POWERINFO_BASE );
                else
                {
                    secY += POWER_SPEC_HT;
                    top = secY;
                    setHeadingFontFromSkin(1);
                    cprnt( (DEFAULT_SCRN_WD/2)*screenScaleX, (top-23)*screenScaleY, 20, screenScale, screenScale, e->pchar->ppPowerSets[i]->psetBase->pchDisplayName );

                    for( j = 0; j < eaSize(&e->pchar->ppPowerSets[i]->ppPowers); j++ )
                    {
                        Power *pow = e->pchar->ppPowerSets[i]->ppPowers[j];
                        if( pow && pow->ppowBase->bShowInManage )
                        {
                            drawPowerAndEnhancements( e->pchar, pow, DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2, secY, 20, 1.f, FALSE, &sb[1].offset, TRUE, screenScaleX, screenScaleY, &clipBox );
                            secY += POWER_SPEC_HT;
                        }
                    }

                    // draw the small inner frame
                    drawFrame( PIX2, R6, (DEFAULT_SCRN_WD/2 - FRAME_WIDTH/2 + POWER_OFFSET)*screenScaleX, (top - 30)*screenScaleY, 5, (POWER_WIDTH - 10)*screenScaleX, (secY - top - 35)*screenScaleY, screenScale, 0xffffffaa, SELECT_FROM_UISKIN( 0x0000ff44, 0xff000044, 0x00000066 ) );
                }
                clipperPop();
            }
             else
            {
                BuildCBox(&clipBox, 0, (SL_FRAME_TOP+PIX4)*screenScaleY, 2000*screenScaleX, (POOL_HEIGHT - PIX4*2 - 3)*screenScaleY );
                clipperPushCBox(&clipBox);
                poolY += POWER_SPEC_HT;
                top = poolY;
                setHeadingFontFromSkin(1);
                cprnt( (DEFAULT_SCRN_WD - FRAME_WIDTH/2 - FRAME_SPACE)*screenScaleX, (top-23)*screenScaleY, 20, screenScale, screenScale, e->pchar->ppPowerSets[i]->psetBase->pchDisplayName );

                for( j = 0; j < eaSize(&e->pchar->ppPowerSets[i]->ppPowers); j++ )
                {
                    Power *pow = e->pchar->ppPowerSets[i]->ppPowers[j];
                    if( pow && pow->ppowBase->bShowInManage )
                    {
                        drawPowerAndEnhancements( e->pchar, pow, DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE, poolY, 20, 1.f, FALSE, &sb[2].offset, TRUE, screenScaleX, screenScaleY, &clipBox);
                        poolY += POWER_SPEC_HT;
                    }
                }

                // draw the small inner frame
                 drawFrame( PIX2, R6, (DEFAULT_SCRN_WD - FRAME_WIDTH - FRAME_SPACE + POWER_OFFSET)*screenScaleX, (top - 30)*screenScaleY, 5, (POWER_WIDTH - 10)*screenScaleX, (poolY - top - 35)*screenScaleY, screenScale, 0xffffffaa, SELECT_FROM_UISKIN( 0x0000ff44, 0xff000044, 0x00000066 ) );
                clipperPop();
            }
        }
    }

    //wrappedText( 40, 650, 10, 940, "EnhancementExplanation", INFO_CHAT_TEXT );

    doScrollBarEx( &sb[0], (FRAME_HEIGHT-R12*2), (primY + sb[0].offset - POWER_SPEC_HT), (FRAME_SPACE + FRAME_WIDTH - PIX4/2), (SL_FRAME_TOP + R12), 20, &box[0], 0, screenScaleX, screenScaleY );
    doScrollBarEx( &sb[1], (FRAME_HEIGHT-R12*2), (secY  + sb[1].offset - POWER_SPEC_HT), ((DEFAULT_SCRN_WD + FRAME_WIDTH)/2 + PIX4/2), (SL_FRAME_TOP + R12), 20, &box[1], 0, screenScaleX, screenScaleY );
    doScrollBarEx( &sb[2], (POOL_HEIGHT-R12*2), (poolY + sb[2].offset - POWER_SPEC_HT), (DEFAULT_SCRN_WD - FRAME_SPACE + PIX4/2), (SL_FRAME_TOP + R12), 20, &box[2], 0, screenScaleX, screenScaleY );

    enhancementAnim_update(kPowerSystem_Powers);

    info_PowManagement(kPowInfo_SpecLevel, screenScaleX, screenScaleY );

    //if ( D_MOUSEHIT == drawStdButton( FRAME_SPACE + 50, 740, 10, 100, 40, CLR_DARK_RED, "CancelString", 2.f, 0) )
    if ( drawNextButton( 40*screenScaleX,  740*screenScaleY, 10, screenScale, 0, 0, 0, "CancelString" ))
    {
        enhancementAnim_clear();
        fadingText_clearAll();
        electric_clearAll();
        init = FALSE;
        s_ShowCombatNumbersHere = 0;
        g_pPowMouseover = 0;

        // Continue the dialog with the NPC
        START_INPUT_PACKET(pak, CLIENTINP_CONTACT_DIALOG_RESPONSE);
        pktSendBitsPack(pak, 1, CONTACTLINK_BYE);
        END_INPUT_PACKET

        start_menu( MENU_GAME );
    }

    if ( drawNextButton( 980*screenScaleX, 740*screenScaleY, 10, screenScale, 1, gNumEnhancesLeft>0, 0, "FinishString" ))
    {
        spec_sendAddSlot( 0 );

        enhancementAnim_clear();
        fadingText_clearAll();
        electric_clearAll();
        init = FALSE;
        s_ShowCombatNumbersHere = 0;
        g_pPowMouseover = 0;

        // Continue the dialog with the NPC
        START_INPUT_PACKET(pak, CLIENTINP_CONTACT_DIALOG_RESPONSE);
        pktSendBitsPack(pak, 1, CONTACTLINK_ENDTRAIN);
        END_INPUT_PACKET

        level_increaseLevel(); // Make the level increase permanent. Do this after everything else.

        //go back to game if we're done levelling
        // DGNOTE 10/1/2008 - not yet on this.  See the comment with the same DGNOTE tag in contactInteraction.c for reasons why.
        //if (!character_CanLevel(e->pchar))
        {
            start_menu( MENU_GAME );
        }
    }

    // HACK: stop pretending
    set_scrn_scaling_disabled(screenScalingDisabled);
    if(bFakedLevel)
    {
        e->pchar->iBuildLevels[e->pchar->iCurBuild] = --e->pchar->iLevel;
    }
}

void specMenu_ForceInit()
{
    init = FALSE;
}

#endif // TEST_CLIENT

void enhancementAnim_send( Packet *pak, int debug )
{
    Entity *e = playerPtr();
    int num = character_CountBuyBoostForLevel( e->pchar, character_GetLevel(e->pchar ) );
    int i, j, count = 0;

    srand( timerCpuTicks64() );
    pktSendBitsPack( pak, 1, num );
    pktSendBitsPack( pak, kPowerSystem_Numbits, 0 );

    if( debug )
    {
        int stuck_counter = 0;

        for( i = 0; i < num; i++ )
        {
            int valid = 0;
            
            while( !valid )
            {
                int iset, ipow;
                Power *ppow;

                stuck_counter++;

                if(stuck_counter>100000)
                {
                    for( /*the rest*/; i < num; i++ )
                    {
                        pktSendBitsPack( pak, 1, 0);
                        pktSendBitsPack( pak, 1, 0 );
                    }
                    break;
                }

                if (eaSize(&e->pchar->ppPowerSets)==0)
                    continue;
                iset = rand()%eaSize(&e->pchar->ppPowerSets);
                if (eaSize(&e->pchar->ppPowerSets[iset]->ppPowers)==0)
                    continue;
                ipow = rand()%eaSize(&e->pchar->ppPowerSets[iset]->ppPowers);
                ppow = e->pchar->ppPowerSets[iset]->ppPowers[ipow];
                if(!ppow) 
                    continue;

                if(character_BuyBoostSlot(e->pchar, ppow))
                {
                    printf("buying boost on slot set='%s' [%d] pow='%s' [%d]\n", e->pchar->ppPowerSets[iset]->psetBase->pchFullName, iset, ppow->ppowBase->pchName, ipow);
                    valid = 1;
                    pktSendBitsPack( pak, 1, iset );
                    pktSendBitsPack( pak, 1, ipow );
                }
            }
        }
    }
    else
    {
        for( i = 0; i < eaSize(&enhancementAnimQueue); i++ )
        {
            EnhancementAnim *ea = enhancementAnimQueue[i];
            for( j = 0; j < ea->count; j++ )
            {
                assert( count < num );
                character_BuyBoostSlot(e->pchar, ea->pow);
                pktSendBitsPack( pak, 1, ea->pow->psetParent->idx );
                pktSendBitsPack( pak, 1, ea->pow->idx );
                count++;
            }
        }

        for( /*the rest*/; count < num; count++ )
        {
            pktSendBitsPack( pak, 1, 0);
            pktSendBitsPack( pak, 1, 0 );
        }

        assert( count == num );
    }
}

/* End of File */
