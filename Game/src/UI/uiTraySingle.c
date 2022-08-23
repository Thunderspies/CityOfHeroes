
#include "UI/uiTray.h"
#include "varutils.h"
#include "player/player.h"
#include <utilitieslib/components/HashFunctions.h>
#include "entity/entVarUpdate.h"
#include "clientcomm/clientcomm.h"
#include "UI/uiCursor.h"
#include "UI/uiChat.h"
#include "UI/uiWindows.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "player/playerState.h"
#include "UI/uiGame.h"
#include "UI/uiUtilGame.h"
#include "UI/uiContextMenu.h"
#include "win/input.h"
#include "UI/uiPet.h"
#include "win/win_init.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiDialog.h"
#include "entity/entclient.h"
#include "assert.h"
#include "UI/uiInput.h"
#include "cmdparse/cmdgame.h"
#include "language/langClientUtil.h"
#include "entity/PowerInfo.h"
//#include "entity/entity_power_client.h"           // for entity_ActivatePowerSend
#include "entity/character_base.h"
#include "entity/powers.h"
#include "gameComm/wdwbase.h"
#include "UI/uiConsole.h"
#include "gameComm/itemselect.h"             // for clect target
#include "graphics/camera.h"                 // for crazy define
#include "player/pmotion.h"                // for entMode
#include <utilitieslib/components/earray.h>
#include "UI/uiUtil.h"
#include "UI/uiToolTip.h"
#include "UI/uiInspiration.h"
#include "UI/uiInfo.h"
#include "UI/uiNet.h"
#include <utilitieslib/utils/timing.h>
#include "UI/uiOptions.h"
#include "UI/uiKeybind.h"
#include "UI/uiEditText.h"
#include "UI/uiWindows_init.h"
#include "entity/character_target.h"
#include "graphics/font.h"
#include "UI/uiTeam.h"
#include <utilitieslib/utils/utils.h>
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/strings_opt.h>
#include "entity/entPlayer.h"
#include "UI/uiReticle.h"
#include "clientcomm/clientcommreceive.h"
#include "entity/entity.h"
#include "gameComm/petCommon.h"
#include "entity/character_inventory.h"
#include "UI/uiPowerInventory.h"
#include "UI/uiTrade.h"
#include "UI/uiGift.h"
#include <utilitieslib/language/MessageStoreUtil.h>
//#include "entity/character_eval.h"
#include "UI/uiClipper.h"

void showNewTray(void)
{
    int i;
    for( i = WDW_TRAY_1; i <= WDW_TRAY_8; i++ )
    {
        if(!windowUp(i))
        {
            window_openClose(i);
            return;
        }
    }
}


static ContextMenu *gTraySingleContext;
static ContextMenu *gTrayServerContext;

static void cmTraySingleClose( void * wdw ){ window_openClose((int)wdw); }
// 1 x 10 (50 x 450)
static void cmTraySingleSize1( void * wdw ){ window_setDims( (int)wdw, -1, -1, 50*winDefs[(int)wdw].loc.sc, 450*winDefs[(int)wdw].loc.sc ); window_UpdateServer((int)wdw); }
// 2 x 5 (90 x 250)
static void cmTraySingleSize2( void * wdw ){ window_setDims( (int)wdw, -1, -1, 90*winDefs[(int)wdw].loc.sc, 250*winDefs[(int)wdw].loc.sc ); window_UpdateServer((int)wdw); }
// 3 x 4 (170 x 130)
static void cmTraySingleSize3( void * wdw ){ window_setDims( (int)wdw, -1, -1, 130*winDefs[(int)wdw].loc.sc, 170*winDefs[(int)wdw].loc.sc ); window_UpdateServer((int)wdw); }
// 4 x 3 (130 x 170)
static void cmTraySingleSize4( void * wdw ){ window_setDims( (int)wdw, -1, -1, 170*winDefs[(int)wdw].loc.sc, 130*winDefs[(int)wdw].loc.sc ); window_UpdateServer((int)wdw); }
// 5 x 2  (240 x 90)
static void cmTraySingleSize5( void * wdw ){ window_setDims( (int)wdw, -1, -1, 250*winDefs[(int)wdw].loc.sc, 90*winDefs[(int)wdw].loc.sc ); window_UpdateServer((int)wdw); }
// 10 x 1 (450 x 50)
static void cmTraySingleSize6( void * wdw ){ window_setDims( (int)wdw, -1, -1, 450*winDefs[(int)wdw].loc.sc, 50*winDefs[(int)wdw].loc.sc ); window_UpdateServer((int)wdw); }

static void traySingle_Init(void)
{
    gTraySingleContext = contextMenu_Create(NULL);
    contextMenu_addCode(gTraySingleContext, alwaysAvailable, 0, cmTraySingleClose, 0, "CMCloseTray", 0);
    contextMenu_addCode(gTraySingleContext, alwaysAvailable, 0, cmTraySingleSize1, 0, "CMTraySize1x12", 0);
    contextMenu_addCode(gTraySingleContext, alwaysAvailable, 0, cmTraySingleSize2, 0, "CMTraySize2x6", 0);
    contextMenu_addCode(gTraySingleContext, alwaysAvailable, 0, cmTraySingleSize3, 0, "CMTraySize3x4", 0);
    contextMenu_addCode(gTraySingleContext, alwaysAvailable, 0, cmTraySingleSize4, 0, "CMTraySize4x3", 0);
    contextMenu_addCode(gTraySingleContext, alwaysAvailable, 0, cmTraySingleSize5, 0, "CMTraySize6x2", 0);
    contextMenu_addCode(gTraySingleContext, alwaysAvailable, 0, cmTraySingleSize6, 0, "CMTraySize12x1", 0);
}

static int traySingle_getTrayNum(Entity *e, int win_id)
{
    int win_num = win_id - WDW_TRAY_1;

    // We can only store 8 indices in a 32 bit number so the Razer tray has its
    // own storage.
    if (win_id == WDW_TRAY_RAZER)
        return e->pl->tray->current_trays[kCurTrayType_Razer];

    return ((e->pl->tray->trayIndexes >> (4 * win_num)) & 0xf);
}

static void traySingle_changeTray( Entity *e, int win_id, int tray_num, int forward )
{
    int win_num = win_id-WDW_TRAY_1;

    tray_num = tray_changeIndex(kTrayCategory_PlayerControlled, tray_num, forward);

    if( win_id == WDW_TRAY_RAZER )
    {
        e->pl->tray->current_trays[kCurTrayType_Razer] = tray_num;
    }
    else
    {
        e->pl->tray->trayIndexes &= ~(0xf<<(4*win_num));
        e->pl->tray->trayIndexes |= (tray_num<<(4*win_num));
    }

    START_INPUT_PACKET(pak, CLIENTINP_RECEIVE_TRAYS);
    sendTray( pak, e );
    END_INPUT_PACKET

}

static void traySingle_drawArrows( Entity *e, int cur_tray, int win, float x, float y, float z, float wd, float ht, float scale, int color, int razer )
{
    char temp[8];
    static AtlasTex * power_ring;
    static AtlasTex * left_arrow;
    static AtlasTex * right_arrow;
    static int texBindInit=0;
    float xscale = 1.0f;
    int leftpos = -16;
    int rightpos = 11;

    if (razer)
    {
        xscale *= 1.75f;
        leftpos = -15;
        rightpos = 9;
    }

    if (!texBindInit) {
        texBindInit = 1;
        power_ring            = atlasLoadTexture( "tray_ring_power.tga" );
        left_arrow            = atlasLoadTexture( "teamup_arrow_contract.tga" );
        right_arrow           = atlasLoadTexture( "teamup_arrow_expand.tga" );
    }

    x -= 2*xscale;

    // display number
    itoa( cur_tray + 1, temp, 10 );
    font( &title_18 );
    font_color( 0xffffff88, 0xffffff88 );
    cprntEx( x, y, z, scale, scale, 3, temp );

    // left arrow
    if(  D_MOUSEHIT == drawGenericButtonEx( left_arrow, x + leftpos*xscale, y - (left_arrow->height)*scale/2, z, xscale, scale, (color & 0xffffff00) | 0x88, color, 1 ) )
        traySingle_changeTray( e, win, cur_tray, 0 );

    // right arrow
    if(  D_MOUSEHIT == drawGenericButtonEx( right_arrow, x + rightpos*xscale, y - (right_arrow->height*scale)/2, z, xscale, scale, (color & 0xffffff00) | 0x88, color, 1 ) )
        traySingle_changeTray( e, win, cur_tray, 1 );
}

static void traySingle_Fill( Entity *e, int tray, float x, float y, float z, float wd, float ht, float scale, int color, TrayBendType type, int razer )
{
    F32 xp, yp;
    int i;

    xp = x + 5*scale + (40/2)*scale;
    if( type == kBend_LR || type == kBend_UL )
        yp = y + ht - 5*scale - (40/2)*scale;
    else
        yp = y + 5*scale + (40/2)*scale;

    // Razer tray has the arrows at the end. Because the icon drawing starts by
    // moving to the right we need to adjust for that.
    if (razer)
        xp -= 40*scale;
    else
        traySingle_drawArrows( e, tray, gCurrentWindefIndex, xp, yp, z, wd, ht, scale, color, 0 );

    // draw the tray slots  
    for( i = 0; i < TRAY_SLOTS; i++ )  
    {
        if( type < 0 ) // square fill
        {
            xp += 40*scale;
            if( xp > x + wd - 20*scale )
            {
                xp = x + 5*scale + (40/2)*scale;
                yp += 40*scale;
            }
        }
        else if( type == kBend_UR ) // once we start wrapping reset to far right
        {
            xp += 40*scale;
            if( xp > x + wd - 20*scale )
            {
                xp = x + wd - 25*scale; 
                yp += 40*scale;
            }
        }
        else if(type == kBend_LL ) // down then right
        {
            yp += 40*scale;
            if( yp > y + ht - 20*scale )
            {
                yp = y + ht - 25*scale;
                xp += 40*scale;
            }
        }
        else if(type == kBend_UL ) // Up then Right
        {
            yp -= 40*scale;
            if( yp < y + 20*scale )
            {
                yp = y + 25*scale;
                xp += 40*scale;
            }
        }
        else if(type == kBend_LR ) // right then up
        {
            xp += 40*scale;
            if( xp > x + wd - 20*scale )
            {
                xp = x + wd - 25*scale; 
                yp -= 40*scale;
            }
        }

        trayslot_Display( e, i, tray, tray, xp, yp, z, scale, color, gCurrentWindefIndex );
    }

    // At the end of drawing the icons for the Razer tray we'll be positioned
    // over the last icon and need to move before we finally draw the arrows.
    if( razer )
    {
        xp += 40*1.5f*scale;
        if( xp > x + wd - 20*scale )
        {
            xp = x + wd - 25*scale; 
            yp -= 40*scale;
        }

        traySingle_drawArrows( e, tray, gCurrentWindefIndex, xp, yp, z, wd, ht, scale, color, 1 );
    }
}

// Deal with bendiness
void traySingle_getDims( int wdw, float * xout, float * yout, float *wdout, float *htout, int *typeout, CBox *box  )
{
    F32 x,y,wd,ht,scale,xp=0,yp=0,twd=0,tht=0,tail, bend_type=-1;
    int w,h;

    if( !window_getDims( wdw, &x, &y, 0, &wd, &ht, &scale, 0, 0 ) )
        return;

    windowClientSizeThisFrame( &w, &h );
    xp = x;
    yp = y;

    if (ht <= 50*scale + 1) // only straight trays can bend
    {
        if( x < 0 ) 
        {
            tail = MIN(10,(int)ceilf(ABS(x)/(40.f*scale)))*40*scale;

            xp = 0;
            twd = wd - tail;
            tht = ht + tail;
            if( y + ht/2 > h/2 )
            {
                yp = y - tail;
                bend_type = kBend_LL;
                BuildCBox( box, 0, yp, 50*scale, tail );
            }
            else
            {
                bend_type = kBend_UL;
                BuildCBox( box, 0, yp+50*scale, 50*scale, tail );
            }        
        }
        else if ( x > w-wd )
        {
            tail = MIN(10,(int)ceilf(ABS(x-(w-wd))/(40.f*scale)))*40*scale;

            xp = w - wd + tail;
            twd = wd - tail;
            tht = ht + tail;

            if( y + ht/2 > h/2 )
            {
                yp = y - tail;
                bend_type = kBend_LR;
                BuildCBox( box, w-50*scale, yp, 50*scale, tail );
            }
            else
            {
                bend_type = kBend_UR;
                BuildCBox( box, w-50*scale, yp+50*scale, 50*scale, tail );
            }
        }
    }

    if (wd <= 50*scale + 1) // only straight trays can bend
    {
        if( y < 0 )
        {
            tail = MIN(10,(int)ceilf(ABS(y)/(40.f*scale)))*40*scale;

            yp = 0;
            twd = wd + tail;
            tht = ht - tail;

            if( x + wd/2 > w/2 )
            {
                xp = x-tail;
                bend_type = kBend_UR;
                BuildCBox( box, x-tail, 0, tail, 50*scale );
            }
            else
            {
                bend_type = kBend_UL;
                BuildCBox( box, x+50*scale, 0, tail, 50*scale );
            }
        }
        else if ( y > h-ht )
        {
            tail = MIN(10,(int)ceilf(ABS(y-(h-ht))/(40.f*scale)))*40*scale;

            yp = h - ht + tail;
            twd = wd + tail;
            tht = ht - tail;

            if( x + wd/2 > w/2 )
            {
                xp = x-tail;
                bend_type = kBend_LR;
                BuildCBox( box, x-tail,h-50*scale, tail, 50*scale );
            }
            else
            {
                bend_type = kBend_LL; 
                BuildCBox( box, x+50*scale, h-50*scale, tail, 50*scale );
            }
        }
    }

    if(xout)
        *xout = xp;
    if(yout)
        *yout = yp;
    if(wdout)
        *wdout = twd;
    if(htout)
        *htout = tht;
    if(typeout)
        *typeout = bend_type;
}

static int trayWindowSingleEx(int razer)
{
    Entity  *e = playerPtr();
    float x, y, z, ht, wd, scale, xp, yp, twd, tht;
    float phx, phy, phz, phht, phwd, phscale;
    int wwd, wht;
    int color, bcolor, cur_tray, bend_type = -1;
    CBox box, box2 = {0};

     if( !window_getDims( gCurrentWindefIndex, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ) )
        return 0;

    if( razer && !optionGet( kUO_PopHelpDisable ) )
    {
        windowClientSizeThisFrame( &wwd, &wht );

        // If the Pop Help icons are against the right-hand edge of the screen
        // and the right hand edge of the Razer tray overlaps the Pop Help,
        // push back the Razer tray so its right hand edge is against the
        // left hand edge of the Pop Help. If the player moves the Pop Help
        // away from the edge, it's up to them to keep them from overlapping.
        window_getDims( WDW_POP_HELP, &phx, &phy, &phz, &phwd, &phht, &phscale, NULL, NULL );

        if( phx + ( phwd * phscale ) >= wwd && x + ( wd * scale ) > phx )
        {
            x = phx - ( wd * scale );
        }
    }

    if(!gTraySingleContext)
        traySingle_Init();

    cur_tray = traySingle_getTrayNum(e, gCurrentWindefIndex);
    traySingle_getDims(gCurrentWindefIndex, &xp, &yp, &twd, &tht, &bend_type, &box2);

    if( bend_type == -1 ) // square fill
    {
        drawFrame( PIX3, R22, x, y, z, wd, ht, scale, color, bcolor );
        traySingle_Fill( e, cur_tray, x, y, z, wd, ht, scale, color, -1, razer );
    }
    else
    {
        drawBendyTrayFrame( xp, yp, z, twd, tht, scale, color, bcolor, bend_type );
        traySingle_Fill( e, cur_tray, xp, yp, z, twd, tht, scale, color, bend_type, razer );
    }

    BuildCBox(&box, x, y, wd, ht);

    if( !razer && mouseClickHit(&box, MS_RIGHT) ||  mouseClickHit(&box2, MS_RIGHT) )
    {
        int ix, iy;
        inpMousePos(&ix, &iy);
        contextMenu_set(gTraySingleContext, ix, iy, (void*)gCurrentWindefIndex);
    }

    return 0;
}

int trayWindowSingleRazer(void)
{
    int retval = 0;

    if( game_state.razerMouseTray )
    {
        if( window_getMode( WDW_TRAY_RAZER ) != WINDOW_DISPLAYING &&
            window_getMode( WDW_TRAY_RAZER ) != WINDOW_GROWING )
        {
            window_setMode( WDW_TRAY_RAZER, WINDOW_GROWING );
        }

        retval = trayWindowSingleEx( 1 );
    }
    else
    {
        window_setMode( WDW_TRAY_RAZER, WINDOW_SHRINKING );
    }

    return retval;
}

int trayWindowSingle()
{
    return trayWindowSingleEx(0);
}
