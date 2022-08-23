

#include "gameComm/wdwbase.h"
#include "UI/uiGame.h"
#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "bases/bases.h"
#include "bases/basedata.h"
#include "UI/uiBaseProps.h"
#include "UI/uiScrollBar.h"
#include "UI/uiTabControl.h"
#include <utilitieslib/language/MessageStoreUtil.h>

#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"

#include "formatter/smf_main.h"

#include <utilitieslib/components/earray.h>

// from uiBaseProps.c
extern TextAttribs gTextTitleAttr;
extern TextAttribs gTextDescAttr;

void openDetailLog( int detailID )
{
    window_openClose(WDW_BASE_LOG);
}

int baselogWindow(void)
{
    float x,y,z,wd,ht,sc, lineht, titleht=0;
    int i,color, bcolor, count = 0;
    static ScrollBar sb = { WDW_BASE_LOG };
    CBox box;

    if( !window_getDims( WDW_BASE_LOG, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ) )
        return 0;

    //draw frame
    drawFrame( PIX3, R10, x, y, z, wd, ht, sc,color, bcolor );

    if(window_getMode(WDW_BASE_LOG) != WINDOW_DISPLAYING )
         return 0;

    // text
     font(&game_12);
    font_color(CLR_WHITE, CLR_WHITE);
     set_scissor(true);
    titleht += 15*sc;
     scissor_dims(x+PIX3*sc, y+titleht+2*PIX3*sc, wd - 2*PIX3*sc, ht - titleht - 3*PIX3*sc );
    BuildCBox(&box,x+PIX3*sc, y+titleht+2*PIX3*sc, wd - 2*PIX3*sc, ht - titleht - 3*PIX3*sc );
    lineht = 15*sc;

    count = eaSize(&g_base.baselogMsgs);
    for( i = 0; i < count; i++ )
    {
        float ty = y+titleht+(i+1)*lineht - sb.offset;
        if( ty < 0 || ty > y+ht+titleht )
            continue;
        prnt( x + R10*sc, ty, z, sc, sc, g_base.baselogMsgs[i] );
    }

    set_scissor(false);

    //scrollbar
      doScrollBar( &sb, ht - titleht - 2*R10*sc, (count+1)*lineht, wd, titleht+R10*sc, z, &box, 0 );

    return 0;
}
