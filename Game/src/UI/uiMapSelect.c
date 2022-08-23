/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "gameComm/wdwbase.h"
#include "DoorStrings.h"
#include <utilitieslib/utils/utils.h>

#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiScrollBar.h"
#include "uiSMFView.h"
#include "clientcomm/clientcomm.h"
#include "entity/entVarUpdate.h"
#include "graphics/textureatlas.h"
#include "UI/sprite/sprite_base.h"
#include "UI/uiMapSelect.h"
#include <utilitieslib/utils/mathutil.h>
#include "player/player.h"
#include "entity/entPlayer.h"

#include "formatter/smf_main.h"
#include "entity/entity.h"
#include "entity/character_base.h"
#include "entity/Supergroup.h"
#include "entity/entPlayer.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/earray.h>
#include "UI/sprite/sprite_text.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "cmdparse/cmdgame.h"
#include "LWC.h"

static TextAttribs gTextAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)0,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

static SMFView *s_pview;
static StuffBuff s_sb; // the stuffbuff where the door text goes

// helper for tracking received mapselect updates
typedef struct MapUpdate
{
    int *ids; // ids received so far (ensures uniqueness)

    char *txtTop;
    char *txtUpdates; // complete text received
    char *txtBottom;
} MapUpdate;

// these strings keep track of updatable doors. any time a door receives an updated
// these three variables are catted together and put in the stuffbuff for display
static MapUpdate s_mapUpdate = {0};

static int    s_hasAnchorPos;
static Vec3 s_anchorPos;
static int    s_mapCountMax;
static int    s_mapCount;
static int*    s_mapIDs;
static int  s_monorail;
static int  s_base;
static int    s_background;
static char s_background_texture[200];

void ClearMapUpdate(void)
{
    estrClear(&s_mapUpdate.txtTop);
    eaiSetSize(&s_mapUpdate.ids, 0);
    estrClear(&s_mapUpdate.txtUpdates);
    estrClear(&s_mapUpdate.txtBottom);
}

void StartMapUpdate(char * title)
{
    if( !s_mapUpdate.txtTop )
        estrCreate(&s_mapUpdate.txtTop);
    if( !s_mapUpdate.txtBottom )
        estrCreate(&s_mapUpdate.txtBottom);
    
    estrPrintf(&s_mapUpdate.txtTop, "%s%s%s", mapSelectFormatTop1, title, mapSelectFormatTop3 );
    estrPrintf(&s_mapUpdate.txtBottom, 
               "<p><a href='cmd:enterdoor 0 0 0 -1'><tr><td align=center valign=center>"
               "%s"
               "</td></tr></a>"
               "%s", textStd("CancelString"), mapSelectFormatBottom);
}

/**********************************************************************func*
 * mapSelectInit
 *
 */
static void mapSelectInit(int monorail, int base, int background, char *background_texture)
{
    if(s_pview==NULL)
    {
        s_pview = smfview_Create(WDW_MAP_SELECT);

        // Sets the location within the window of the text region
        smfview_SetLocation(s_pview, PIX3, PIX3+R10, 0);
        smfview_SetText(s_pview, "");

        initStuffBuff(&s_sb, 1024);
    }
    s_monorail = monorail;
    s_base = base;
    s_background = background;
    if(s_monorail)
        sprintf(s_background_texture, "%s", background_texture);
    else
        s_background_texture[0] = '\0';
}

/**********************************************************************func*
 * mapSelectReceiveText
 *
 */
static void s_mapSelectReceiveHdr(Packet *pak, int *hasAnchorPos, Vec3 *anchorPos, int *monorail, int *base, int *background, char *background_texture)
{
    Vec3 anchorPosLocal;
    int tmpHasAnchorPos;
    int tmpMonorail;
    int tmpBase;
    
    if(!hasAnchorPos)
        hasAnchorPos = &tmpHasAnchorPos;
    if(!monorail)
        monorail = &tmpMonorail;
    if(!base)
        base =&tmpBase;
    
    if(!verify(pak))
    {
        return;
    }

    (*hasAnchorPos) = pktGetBits(pak, 1);

    if( (*hasAnchorPos) )
    {
        anchorPosLocal[0] = pktGetF32(pak);
        anchorPosLocal[1] = pktGetF32(pak);
        anchorPosLocal[2] = pktGetF32(pak);
    }

    if( (*hasAnchorPos) )
    {
        if( anchorPos )
        {
            (*anchorPos)[0] = anchorPosLocal[0];
            (*anchorPos)[1] = anchorPosLocal[1];
            (*anchorPos)[2] = anchorPosLocal[2];
        }
    }

    *monorail = pktGetBits(pak,1);
    if( *monorail )
    {
        *background = pktGetBits(pak,1);

        switch( *background )
        {
            case 0:
            default:
                background_texture[0] = '\0';
                break;
            case 1:
                sprintf(background_texture, "%s", pktGetString(pak));
                break;
        }
    }
    else
    {
        background_texture[0] = '\0';
    }

    *base = pktGetBits(pak,1);
}

void mapSelectReceiveText(Packet *pak)
{
    const char* text;
    int monorail, base, background;
    char background_texture[200];

    s_mapSelectReceiveHdr(pak, &s_hasAnchorPos, &s_anchorPos, &monorail, &base, &background, background_texture);
    text = pktGetString(pak);
    
    mapSelectInit(monorail,base,background,background_texture);
    mapSelectOpen();
    
    mapSelectSetText(text);
    
    // clear out the updatable strings. 
    ClearMapUpdate();
}


void mapSelectReceiveInit(Packet *pak)
{
    int monorail, base, background;
    char *title = NULL;
    char background_texture[200];

    ClearMapUpdate();
    
    // net
    s_mapSelectReceiveHdr(pak, &s_hasAnchorPos, &s_anchorPos, &monorail, &base, &background, background_texture);
    title = pktGetString(pak);

    StartMapUpdate(title);    
    mapSelectInit(monorail,base,background,background_texture);
    mapSelectOpen();    

    if( *s_mapUpdate.txtTop && *s_mapUpdate.txtBottom )
    {
        char *text;
        // combine the text to display the select initially
        estrCreate(&text);
        estrPrintf(&text,"%s%s", s_mapUpdate.txtTop, s_mapUpdate.txtBottom);
        mapSelectSetText(text);
        estrDestroy(&text);    
    }
}

void mapSelectReceiveUpdate(Packet *pak)
{
    int monorail, base, background;
    int idUpdate;
    char *title = NULL;
    char *txtUpdate = NULL;
    char background_texture[200];

    // net
    s_mapSelectReceiveHdr(pak, NULL, NULL, &monorail, &base, &background, background_texture);
    idUpdate = pktGetBitsAuto(pak);
    title = pktGetString(pak);

    mapSelectInit(monorail,base,background,background_texture);
    mapSelectOpen();    
    StartMapUpdate(title);    

    txtUpdate = pktGetString(pak);
    if(!verify(s_mapUpdate.txtTop && s_mapUpdate.txtBottom)
       || !verify( estrLength(&s_mapUpdate.txtTop) > 0 ))
    {
        return;
    }
    
    if( !s_mapUpdate.txtUpdates )
        estrCreate(&s_mapUpdate.txtUpdates);
    
    if( idUpdate == 0 || eaiFind( &s_mapUpdate.ids, idUpdate ) < 0 )
    {
        char *text = NULL;

        // add this entry
        estrConcatCharString(&s_mapUpdate.txtUpdates, txtUpdate);
        if( idUpdate )
            eaiPush(&s_mapUpdate.ids, idUpdate );
        
        // combine the text to display the select initially
        estrCreate(&text);
        estrPrintf(&text,"%s\n%s\n%s", s_mapUpdate.txtTop, s_mapUpdate.txtUpdates, s_mapUpdate.txtBottom);
        mapSelectSetText(text);
        estrDestroy(&text);
    }
}


/**********************************************************************func*
 * mapSelectReceiveClose
 *
 */
void mapSelectReceiveClose(Packet *pak)
{
    mapSelectClose();
}

/**********************************************************************func*
 * mapSelectOpen
 *
 */
void mapSelectOpen(void)
{
    window_setMode(WDW_MAP_SELECT, WINDOW_GROWING);
}

/**********************************************************************func*
 * mapSelectClose
 *
 */
void mapSelectClose(void)
{
    window_setMode(WDW_MAP_SELECT, WINDOW_SHRINKING);

    // clear out the updatable door. 
    ClearMapUpdate();
}

int isMapSelectOpen(void)
{
    return window_getMode(WDW_MAP_SELECT) == WINDOW_DISPLAYING;
}

/**********************************************************************func*
 * mapSelectSetText
 *
 */
void mapSelectSetText(const char *pch)
{

    clearStuffBuff(&s_sb);
    addSingleStringToStuffBuff(&s_sb, pch);

    smfview_Reparse(s_pview);
    smfview_SetText(s_pview, s_sb.buff);
}

/**********************************************************************func*
 * mapSelectWindow
 *
 */
int mapSelectWindow(void)
{
    float x, y, z, wd, ht, sc;
    int color, bcolor;
    AtlasTex *logo;
    Entity* player = playerPtr();
    float bottomgap;

    if(!window_getDims(WDW_MAP_SELECT, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
        return 0;
        
    if(player && s_hasAnchorPos && distance3Squared(ENTPOS(player), s_anchorPos) > SQR(20))
    {
        mapSelectClose();
    }

    // Matt H told me to make it opaque
    drawFrame(PIX3, R10, x, y, z, wd, ht, sc, color, bcolor|0xff);

    set_scissor(1);
    scissor_dims( x+PIX3*sc, y+PIX3*sc, wd - PIX3*2*sc, ht - PIX3*2*sc );
    if (s_monorail) 
    {
        if (s_background_texture[0])
        {
            logo = atlasLoadTexture(s_background_texture);

            display_sprite(logo, x + wd/2 - logo->width*sc/2, y+ht/2-logo->height*sc/2, z, sc, sc, 0xffffff22);
        }
    } 
    else if (s_base)
    {
        Supergroup* group = player->supergroup;
        if(group && group->emblem)
        {
            char emblemTexName[256];

            if (group->emblem[0] == '!')
            {
                emblemTexName[0] = 0;
            }
            else
            {
                strcpy(emblemTexName, "emblem_");
            }
            strcat(emblemTexName, group->emblem);
            strcat(emblemTexName, ".tga");

            logo = atlasLoadTexture(emblemTexName);
            sc *= 6.0f;
            display_sprite(logo, x + wd/2 - logo->width*sc/2, y+ht/2-logo->height*sc/2, z, sc, sc, 0xffffff22);
            sc /= 6.0f;
        }
    }

    set_scissor(0);

    bottomgap = PIX3*2+R10*2*sc;

    if (s_monorail)
        bottomgap += 10+30*sc;

    // Sets the size of the region within the window of the text region
    // In this case, it resizes with the window
    gTextAttr.piScale = (int*)((int)(SMF_FONT_SCALE*sc));
     smfview_SetSize(s_pview, wd-(PIX3*2+5)*sc, ht-bottomgap);
    smfview_Draw(s_pview);

    if (s_monorail)
    {
        if (D_MOUSEHIT == drawStdButton(x + wd/2, y + ht - 30*sc, z + 20, 100*sc, 30*sc, SELECT_FROM_UISKIN(CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE), "CancelString", sc, 0))
        {
            mapSelectClose();
        }
    }

    return 0;
}

/* End of File */

