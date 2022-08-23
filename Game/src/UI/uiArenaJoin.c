
#include "win/win_init.h"
#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "player/playerState.h"
#include "player/player.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "formatter/smf_main.h"
#include "entity/character_base.h"
#include "UI/uiArena.h"
#include "UI/uiArenaList.h"
#include "UI/uiNet.h"
#include "entity/entity.h"
#include "gameData/arenastruct.h"
#include "arena/arenagame.h"
#include <utilitieslib/language/MessageStoreUtil.h>



int gArenaInvited = 0;
static bool gInArenaLobby = 0;

void enteredArenaLobby()
{
    gInArenaLobby = true;
}

void leftArenaLobby()
{
    gInArenaLobby = false;
}

bool inArenaLobby()
{
    return gInArenaLobby;
}

int arenaJoinWindow(void)
{
    float x, y, z, wd, ht, sc;
    int color, back_color;
    static SMFBlock *sm;
    Entity * e = playerPtr();

    if (!sm)
    {
        sm = smfBlock_Create();
    }

    if( !window_getDims( WDW_ARENA_JOIN, &x, &y, &z, &wd, &ht, &sc, &color, &back_color ) )
    {
        if( gArenaInvited )
            window_setMode( WDW_ARENA_JOIN, WINDOW_GROWING );
        return 0;
    }
    else if( gArenaInvited && inArenaLobby() && gArenaDetail.eventid )
    {
        window_setMode( WDW_ARENA_JOIN, WINDOW_SHRINKING );
        window_setMode( WDW_ARENA_CREATE, WINDOW_GROWING );
        gArenaInvited = false;
        return 0;
    }

    if( !gArenaInvited || window_getMode( WDW_ARENA_CREATE ) == WINDOW_DISPLAYING || amOnArenaMap() || !gArenaDetail.eventid )
    {
        window_setMode( WDW_ARENA_JOIN, WINDOW_SHRINKING );
        gArenaInvited = false;
        return 0;
    }

    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, back_color );
    set_scissor(1);
    scissor_dims( x+PIX3*sc, y+PIX3*sc, wd - PIX3*2*sc, ht-PIX3*2*sc );

    gTextAttr_Green.piScale = (int*)((int)(SMF_FONT_SCALE*sc));
    smf_ParseAndDisplay(sm, textStd("GetToArena"), x+PIX3*2*sc, y+PIX3*2*sc, z+1, wd-PIX3*4*sc, 0, 0, 0, 
                        &gTextAttr_Green, NULL, 0, true); 

      if(D_MOUSEHIT == drawStdButton(x + wd/2, y + ht - (PIX3+33)*sc, z+1, 140*sc, 20*sc, color, "OkString", sc, !gInArenaLobby))
    {
        requestCurrentKiosk();
    }

    if(D_MOUSEHIT == drawStdButton(x + wd/2, y + ht - (PIX3+13)*sc, z+1, 140*sc, 20*sc, CLR_RED, "QuitEvent", sc, 0))
    {
        // quit event
        arenaSendPlayerDrop( &gArenaDetail, e->db_id );
        gArenaInvited = false;
    }

    set_scissor(0);

    return 0;

}