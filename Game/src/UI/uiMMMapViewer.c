#include "UI/uiUtil.h"
#include "UI/uiGame.h"
#include "UI/uiInput.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiEditText.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "uiMMMapViewer.h"
#include "UI/uiWindows.h"
#include "UI/uiClipper.h"
#include "UI/uiToolTip.h"
#include "uiHelpButton.h"
#include "UI/sprite/sprite_text.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"

#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/Earray.h>
#include <utilitieslib/utils/mathutil.h>
#include "graphics/imageCapture.h"
#include "entity/entity.h"
#include <utilitieslib/components/estring.h>
#include "entity/motion.h"
#include "entity/entclient.h"
#include "entity/costume_client.h"
#include "win/win_init.h"
#include "seq/seqstate.h"
#include "entity/character_animfx_client.h"
#include "entity/character_base.h"
#include <utilitieslib/components/memorypool.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/uiAutomap.h"

static int s_showSpawns = 1, s_showItems = 1;

typedef enum pictBrowseState
{
    PICTBROWSE_IDLE,
    PICTBROWSE_MOVING,
} PictBrowseState;

typedef enum pictBrowseAction
{
    PICTBROWSE_NONE = 0,
    PICTBROWSE_HOVER = -2,
    PICTBROWSE_HIT,
} PictBrowseAction;

#define WINDOW_SIDES_BUFFER 5*PIX2


MP_DEFINE(MMMapViewer);
MMMapViewer *mmmapviewer_create()
{
    MMMapViewer *mv = NULL;

    // create the pool, arbitrary number
    MP_CREATE(MMMapViewer, 64);
    mv = MP_ALLOC( MMMapViewer );
    return mv;
}


void mmmapviewer_init( MMMapViewer *mv, float x, float y, float z, float wd, float ht, int wdw )
{
    assert(mv);
    memset(mv, 0, sizeof(MMMapViewer));
    mv->x = x;
    mv->y = y;
    mv->z = z;

    mv->maxWd = mv->wd = wd;
    mv->wdw = wdw;

    mv->maxHt = mv->ht = ht;
    mv->sc = 1;

    mv->state = PICTBROWSE_IDLE;
    mv->changed = TRUE;
    mv->currChoice = 0;
    mv->transitionProgress = 0;
}

// for dynamically populating a browser
void mmmapviewer_addPictures( MMMapViewer *mv, ArchitectMapHeader *map)
{
    assert(mv);

    mv->map = map;
}

// clear and free all of the elements
void mmmapviewer_clearPictures( MMMapViewer *mv)
{
    mv->map = NULL;
}

// clear out a pictureBrowser structure to be re-inited
void mmmapviewer_destroy(MMMapViewer *mv)
{
    mmmapviewer_clearPictures(mv);
    memset( mv, 0, sizeof(MMMapViewer) );
    MP_FREE(MMMapViewer, mv);
}

// sets a specific element to be selected
void mmmapviewer_setId( MMMapViewer *mv, int id)
{
    mv->currChoice = id;
    mv->changed = TRUE;
}

// set the location of a picture browser
void mmmapviewer_setLoc( MMMapViewer *mv, float x, float y, float z )
{
    assert(mv);
    mv->x = x;
    mv->y = y;
    mv->z = z;
}

int mmmapviewer_drawNavButton(float x, float y, float z, float wd, float ht, AtlasTex * icon, int *buttonColor)
{
    UIBox uibox;
    float subx = x, suby = y, subht = ht, subwd = wd;
    float subsc = 1;
    CBox cbox = {0};

    uibox.x = subx;
    uibox.y = suby;
    uibox.width = subwd;
    uibox.height = subht;

    uiBoxToCBox(&uibox, &cbox);

    if(icon)
    {
        subwd = MIN(icon->width, subwd - PIX2*2);
        subht = MIN(icon->height, subht - PIX2*2);
        subsc = MIN(subwd/icon->width, subht/icon->height);
        suby = y+(ht - icon->height*subsc)/2.0;
        subx = x+(wd - icon->width*subsc)/2.0;
    }

    if(icon)
        display_sprite(icon, subx, suby, z+2, subsc, subsc, 0xffffffff);

    //if hover
    if(mouseCollision( &cbox ) && buttonColor)
    {
        *buttonColor = 0xffffffaa;
        //drawFlatFrame(PIX3, R10, x, y, z+1, wd, ht, 1, 0x88888888, 0x88888888);
    }
    if(mouseDownHit(&cbox, MS_LEFT))
    {
        if(buttonColor)
            *buttonColor = 0xffffffff;
        //drawFlatFrame(PIX3, R10, x, y, z+1, wd, ht, 1, 0xffffff88, 0xffffff88);
        return 1;
    }
    return 0;
}


static void getCurrentImage(MMMapViewer *mv, int idx, ArchitectMapSubMap **floor, ArchitectMapSubMap **items, ArchitectMapSubMap **spawns)
{
    int levels;
    if(!mv || !floor || !items || !spawns || !mv->map)
        return;

    levels = eaSize(&mv->map->mapFloors);

    if(idx < levels)
    {
        *floor = mv->map->mapFloors[idx];
        if(idx < eaSize(&mv->map->mapItems))
            *items = mv->map->mapItems[idx];
        else
            *items = NULL;
        if(idx < eaSize(&mv->map->mapSpawns))
            *spawns = mv->map->mapSpawns[idx];
        else
            *spawns = NULL;
    }
    else
    {
        *floor = *spawns = *items = NULL;
    }
}

static void mmmapviewer_drawMap(MMMapViewer * mv, int floor, F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 stretch)
{
    ArchitectMapSubMap *map = 0;
    ArchitectMapSubMap *items = 0;
    ArchitectMapSubMap *spawns = 0;
    getCurrentImage(mv, floor, &map,  &items,  &spawns);

    automap_drawSubmapFloor(map, (s_showItems && items) ? items : NULL, ( s_showSpawns && spawns ) ? spawns : NULL, x, y, z+1, wd, ht, mv->sc, stretch);
};


#define MMMAP_VIEWER_SPEED .1
static void mmmapviewer_drawPictures(MMMapViewer * mv, float x, float y, float z, float wd, float ht, float scalex, float scaley)
{
    int element;
    float direction;
    int a, b;    //the element ids being displayed
    float tx;

    int numFloors = eaSize(&mv->map->mapFloors);
    if(!mv || !mv->map || !numFloors)
        return;

    element = mv->currChoice % numFloors;
    if(element == eaSize(&mv->map->mapFloors))
        return;    //this means we're empty.

    direction = element-mv->transitionProgress;
    if(direction < 0)
    {
        mv->transitionProgress -=MMMAP_VIEWER_SPEED*TIMESTEP;
        if(mv->transitionProgress <=element)
            mv->transitionProgress = element;
    }
    else if(direction > 0)
    {
        mv->transitionProgress +=MMMAP_VIEWER_SPEED*TIMESTEP;
        if(mv->transitionProgress >=element)
            mv->transitionProgress = element;
    }
    
    a = floor(mv->transitionProgress);
    b = a+1;
    direction = mv->transitionProgress - a;
    tx = x-(MIN(mv->wd, mv->ht)*direction*scalex);

    mmmapviewer_drawMap(mv, a, tx, y, z, wd, ht, scalex);
    if(direction)
    {
        tx += (MIN(mv->wd, mv->ht)*scalex);
        mmmapviewer_drawMap(mv, b, tx, y, z, wd, ht, scalex);
    }
}

// Draws the picture browser
#define A_BUTTON_HEIGHT PIX3*5
int mmmapviewer_display( MMMapViewer *mv )
{
    int color = CLR_WHITE;
    float tx, ty, tz, tht, twd, sc = 1.f, bwd, bwd2;
    //float subwd, subht;
    int col, back_color;
    UIBox uibox;
    int turning = 0;    //using in place of collisions_off_for_rest_of_frame so that the box will still show up
    int turnHover = 0;    //are we hovering over the turn buttons?
    static int isClicking = 0;//are we still mid click?
    int textColor = 0xFFFFFFFF;
    int centerColor, leftColor, rightColor;
    AtlasTex *arrowLeft = atlasLoadTexture( "chat_separator_arrow_left.tga" );
    AtlasTex *arrowRight = atlasLoadTexture( "chat_separator_arrow_right.tga" );
    F32 buttonWidth = arrowLeft->width*2+2*PIX2*2;
    static HelpButton *pHelp;
    F32 wdStretch = (mv->wd-R27*2)/((F32)MINIMAP_BASE_DIMENSION);
    F32 htStretch = mv->ht/((F32)MINIMAP_BASE_DIMENSION);
    F32 useStretch = MIN(wdStretch, htStretch);
    int nFloors;
    leftColor= rightColor =0x00000055;
    centerColor = 0x000000FF;
     

    if (!mv || mv->maxHt <= 0 || mv->maxWd <=0)
    { //We're trying to display an uninitialized picture browser
        return -1;
    }
    nFloors = mv->map?eaSize(&mv->map->mapFloors):0;


    // clear change notification
    mv->changed = false;


    // if its linked to a get offset coordinates
    if( mv->wdw )
    {
        float wx, wy, wz, wwd,wht;

        if( window_getMode( mv->wdw ) != WINDOW_DISPLAYING )
            return -1;

        window_getDims( mv->wdw, &wx, &wy, &wz, &wwd, &wht, &sc, &col, &back_color );
        tx = mv->x*sc + wx;
        ty = mv->y*sc + wy;
        tz = mv->z + wz;
        tht = mv->ht;
        twd = mv->wd;
    }
    else
    {
        tx = mv->x;
        ty = mv->y;
        tz = mv->z;
        tht = mv->ht;
        twd = mv->wd;
        col = back_color = -1;
        if( mv->sc )
            sc = mv->sc;
    }
    tx += WINDOW_SIDES_BUFFER;
    ty += WINDOW_SIDES_BUFFER;

       drawMMCheckBox( tx + 10*sc, ty - 25*sc, tz, sc, textStd("MMShowSpawns"), &s_showSpawns, CLR_MM_TEXT, CLR_MM_BUTTON, CLR_MM_TITLE_OPEN, CLR_MM_BUTTON_LIT, &bwd, 0 );
    drawMMCheckBox( tx + bwd + 20*sc, ty - 25*sc, tz, sc, textStd("MMShowItems"), &s_showItems, CLR_MM_TEXT, CLR_MM_BUTTON, CLR_MM_TITLE_OPEN, CLR_MM_BUTTON_LIT, &bwd2, 0 );
     helpButton( &pHelp, tx + bwd + bwd2 + 35*sc, ty - 20*sc, tz, sc, "MMShowSpawnKey", 0 );

    // get image pan interaction
    if(nFloors>1)
    {
        char *status;
        leftColor= rightColor = 0xffffff44;
        if(mv->currChoice >0 && mmmapviewer_drawNavButton(mv->x, ty, tz+2, buttonWidth, mv->ht, arrowLeft,&leftColor))
        {
            mmmapviewer_setId(mv, mv->currChoice-1);
        }
        if(mv->currChoice < nFloors-1 && mmmapviewer_drawNavButton(mv->x+mv->wd-buttonWidth, ty, tz+2, buttonWidth, mv->ht, arrowRight, &rightColor))
        {
            mmmapviewer_setId(mv, mv->currChoice+1);
        }

        font_color( CLR_WHITE, CLR_WHITE );
        font( &game_12 );

        estrCreate(&status);

        estrPrintCharString(&status, textStd("MMMFloorOf", mv->currChoice+1, nFloors));

        font_set(&game_12, 0, 0, 1, 1, textColor, textColor);
        cprntEx(mv->x+mv->wd/2, mv->y+mv->ht-PIX3, tz+5, sc, sc, CENTER_X, status );//mv->y+mv->ht-WINDOW_SIDES_BUFFER
        estrDestroy(&status);
    }


    //setup the uibox.
    if(nFloors)
    {
        F32 mapWidth = MINIMAP_BASE_DIMENSION*useStretch;
        F32 mapHeight = MINIMAP_BASE_DIMENSION*useStretch;

        tx = mv->x + (mv->wd-R27*2-mapWidth)*.5*sc-PIX3*2;//+R27*sc;
        ty = mv->y + (mv->ht-mapHeight)*.5*sc;
        uibox.x = tx;
        uibox.y =ty;
        uibox.width = sc*(mv->wd-R27*2)-PIX3*2;
        uibox.height = mv->ht;
        clipperPushRestrict(&uibox);
        
        mmmapviewer_drawPictures(mv, tx,ty,tz+2, uibox.width, uibox.height, useStretch*sc, useStretch*sc);


        clipperPop();
    }
     drawFlatThreeToneFrame(PIX3, R27, mv->x, mv->y, mv->z, mv->wd, mv->ht, sc, leftColor, centerColor, rightColor);

    return mv->y + mv->ht*useStretch;
}

// Moves the picture browser's selection left or right
void mmmapviewer_pan( MMMapViewer *pb, int pan)
{
    /*int id;
    if(pb->state== PICTBROWSE_MOVING)    //don't change if we're mid move
        return;

    id = pb->currChoice;
    if(pan == PICTBROWSE_LEFT)
    {
        if(pb->mmdata)
            pb->mmdata->rotation+= (TIMESTEP)*.1;
        id--;
    }
    else if(pan == PICTBROWSE_RIGHT)
    {
        if(pb->mmdata)
            pb->mmdata->rotation-= (TIMESTEP)*.1;
        id++;
    }
    //else
    //    id = id;    //do nothing.

    if(pb->elements)
    {
        id %= eaSize(&pb->elements);
        picturebrowser_setId(pb, id);
    }*/
}

char *mmmapviewer_getMapName(MMMapViewer *mv )
{
    return mv->mapname;
}
void mmmapviewer_setMapName(MMMapViewer *mv, char *mapname )
{
    estrPrintCharString(&mv->mapname, mapname);
}
