/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "gameComm/wdwbase.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/utils.h>
#include "comm_game.h"
#include "filter/titles.h"
#include "player/player.h"
#include "graphics/truetype/ttFontDraw.h"
#include "entity/origins.h"
#include "entity/character_base.h"
#include "entity/entPlayer.h"
#include "clientcomm/clientcomm.h"
#include "storyarc/contactCommon.h"

#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiFocus.h"
#include "UI/uiScrollBar.h"
#include "UI/uiEditText.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiInput.h"
#include "UI/uiNet.h"
#include "entity/entVarUpdate.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"
#include "formatter/smf_main.h"
#include "graphics/ttFontUtil.h"
#include "entity/entity.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "uiColorPicker.h"
#include "UI/uiReticle.h"

static TextAttribs gTextAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.1f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
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
static bool s_bShowOrigin = false;
static bool s_bShowVeteran = false;

static int s_iBase;
static int s_iCommon;
static int s_iOrigin;
static int s_iColor1 = CLR_PARAGON;
static int s_iColor2 = CLR_PARAGON;
static int s_iCommon_display;
static int s_iVeteran_display;
static int s_iOrigin_display;

static TitleList *s_pBase;
static TitleList *s_pCommon;
static TitleList *s_pVeteran;
static TitleList *s_pOrigin;
static ColorPicker cp[2] = {0};

/**********************************************************************func*
 * selectableTextFlat
 *
 */
int selectableTextFlat(bool *pbSelected, float x, float y, float z, float wd, float ht, float sc,
        char *pch, bool bCenter, bool bSelected)
{
    CBox box;
    int colorFG = bSelected ? CLR_SELECTION_BACKGROUND : CLR_NORMAL_BACKGROUND;
    int colorBG = bSelected ? CLR_SELECTION_BACKGROUND : CLR_NORMAL_BACKGROUND;

    *pbSelected = false;

    wd = wd<=0 ? str_wd(font_grp, sc, sc, pch) : wd;
    ht = ht<=0 ? ttGetFontHeight(font_grp, sc, sc) : ht;

#define BLOAT 2

    wd = wd+sc*(PIX3*2+BLOAT*2);
    ht = ht+sc*(PIX3*2);

    BuildCBox(&box, x, y, wd, ht);
    if(mouseCollision(&box))
    {
        colorFG = CLR_SELECTION_BACKGROUND;
        colorBG = CLR_SELECTION_BACKGROUND;

        if(mouseDown(MS_LEFT))
        {
            *pbSelected = true;
        }

        drawFlatFrame(PIX3, R4, x, y, z, wd, ht, sc, colorFG, colorBG);
    }

    drawFlatFrame(PIX3, R4, x, y, z, wd, ht, sc, colorFG, colorBG);
    if(bCenter)
    {
        cprnt(x+wd/2, y+(ht-sc*4), z, sc, sc, pch);
    }
    else
    {
        prnt(x+sc*(PIX3+BLOAT), y+(ht-sc*4), z, sc, sc, pch);
    }

      return ht+1*sc;
}


/**********************************************************************func*
 * titleselectWindow
 *
 */
int titleselectWindow(void)
{
    static SMFBlock *s_block = 0;
    Entity *e = playerPtr();
    static ColorPicker cp[2] = {0};

    float x, y, yorig, xorig, z, wd, ht, sc, htHdr;
    float maxy = 0;
    int color, bcolor;
    int i, j;
    char buf[128];
    static ScrollBar sb;
    CBox box;

    if(!window_getDims(WDW_TITLE_SELECT, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor) || !s_pBase)
    {
        return 0;
    }

    if (!s_block)
    {
        s_block = smfBlock_Create();
    }

    drawFrame(PIX3, R10, x, y, z, wd, ht, sc, color, bcolor);

    if(window_getMode( WDW_TITLE_SELECT) == WINDOW_DISPLAYING && s_pBase && s_pCommon && s_pVeteran && s_pOrigin)
    {
        x+=sc*PIX3*2;
        y+=sc*PIX3*2;
        wd+=sc*(-PIX3*4);
        ht+=sc*(-PIX3*2);
        yorig = y;
        xorig = x;

        font(&game_9);
        font_color(CLR_WHITE, CLR_WHITE);

        gTextAttr.piScale = (int*)((int)(SMF_FONT_SCALE*sc));
        htHdr = smf_ParseAndDisplay(s_block, textStd("TitleSelectIntroSMF"), x, y, z, wd, ht, false, false, 
                                    &gTextAttr, NULL, 0, true);
        htHdr += 10*sc;

        y+=htHdr;

        set_scissor(1);
        scissor_dims(x, yorig + htHdr, wd, ht - htHdr - 85*sc);
        BuildCBox( &box, x, yorig + htHdr, wd, ht - htHdr - 85*sc);
        y -= sb.offset;

        if(!s_bShowOrigin)
            x+=55*sc;
        if(!s_bShowVeteran)
            x+=55*sc;

        // display chooselist instead of the actual titles when picking
        for(i=0; i<eaSize(&s_pBase->ppchChooseList); i++)
        {
            bool bClicked = false;

            y += selectableTextFlat(&bClicked, x, y, z, 85*sc, 0, sc,
                s_pBase->ppchChooseList[i],
                false, s_iBase==i);

            if(bClicked)
            {
                s_iBase = i;
            }
        }

        y = yorig+htHdr - sb.offset;

        // Color pickers here
        if(accountLoyaltyRewardHasNode(e->pl->loyaltyStatus, "ColoredTitles"))
        {
              prnt( x, y + 60*sc, z, sc, sc, "Color1String" );
             drawColorPicker( &cp[0], x, y + 65*sc, z, 95*sc, 50*sc, 0 );
            prnt( x, y + 145*sc, z, sc, sc, "Color2String" );
            drawColorPicker( &cp[1], x, y + 150*sc, z, 95*sc, 50*sc, 0 );
            s_iColor1 = cp[0].rgb_color;
            s_iColor2 = cp[1].rgb_color;
        }
        x+=100*sc;

        for(j = 0; j < eaiSize(&s_pCommon->piListGenders); j++)
        {
            if((s_pBase->piAdjectiveGenders && s_pCommon->piListGenders[j] == s_pBase->piAdjectiveGenders[s_iBase]) ||
               (!s_pBase->piAdjectiveGenders && s_pCommon->piListGenders[j] == s_pBase->piGenders[s_iBase]))
            {
                int size = eaSize(&s_pCommon->ppchChooseList) / eaiSize(&s_pCommon->piListGenders);
                for(i=0; i<size; i++)
                {
                    bool bClicked = false;
                    bool bCenter = (s_bShowVeteran && i == 0);
                    int  iWidth = (s_bShowVeteran && i == 0) ? 185*sc : 85*sc;

                    y += selectableTextFlat(&bClicked, x, y, z, iWidth, 0, sc,
                        s_pCommon->ppchChooseList[j*size+i],
                        bCenter, s_iCommon==i);

                    if(bClicked)
                        s_iCommon = i;
                    if(s_iCommon == i)
                        s_iCommon_display = j*size+i;
                }
            }
        }
        maxy = y - (yorig + htHdr - sb.offset);

        if(s_bShowVeteran)
        {
            y = yorig+htHdr - sb.offset;
            y += ttGetFontHeight(font_grp, sc, sc)+sc*(PIX3*2+1); // this is cheating, it knows how tall the selectableTextFlats are
            x = x+100*sc;
            for(j = 0; j < eaiSize(&s_pVeteran->piListGenders); j++)
            {
                if((s_pBase->piAdjectiveGenders && s_pVeteran->piListGenders[j] == s_pBase->piAdjectiveGenders[s_iBase]) ||
                (!s_pBase->piAdjectiveGenders && s_pVeteran->piListGenders[j] == s_pBase->piGenders[s_iBase]))
                {
                    int size = eaSize(&s_pVeteran->ppchChooseList) / eaiSize(&s_pVeteran->piListGenders);
                    for(i=0; i<size; i++)
                    {
                        bool bClicked = false;

                        y += selectableTextFlat(&bClicked, x, y, z, 85*sc, 0, sc,
                            s_pVeteran->ppchChooseList[j*size+i],
                            false, s_iCommon==i+eaSize(&s_pCommon->ppchTitles));

                        if(bClicked)
                            s_iCommon = i+eaSize(&s_pCommon->ppchTitles);
                        if(s_iCommon == i+eaSize(&s_pCommon->ppchTitles))
                            s_iVeteran_display = j*size+i;
                    }
                }
            }
            maxy = MAX(y - (yorig + htHdr - sb.offset), maxy);
        }

        if(s_bShowOrigin)
        {
            y = yorig+htHdr - sb.offset;
            x = x+100*sc;

            for(j = 0; j < eaiSize(&s_pOrigin->piListGenders); j++)
            {
                if((s_pBase->piAdjectiveGenders && s_pCommon->piListGenders[j] == s_pBase->piAdjectiveGenders[s_iBase]) ||
                    (!s_pBase->piAdjectiveGenders && s_pCommon->piListGenders[j] == s_pBase->piGenders[s_iBase]))
                {
                    int size = eaSize(&s_pOrigin->ppchChooseList) / eaiSize(&s_pOrigin->piListGenders);
                    for(i=0; i<size; i++)
                    {
                        bool bClicked = false;

                        y += selectableTextFlat(&bClicked, x, y, z, 85*sc, 0, sc,
                            s_pOrigin->ppchChooseList[j*size+i],
                            false, s_iOrigin==i);

                        if(bClicked)
                            s_iOrigin = i;
                        if(s_iOrigin == i)
                            s_iOrigin_display = j*size+i;
                    }
                }
            }
            maxy = MAX(y - (yorig + htHdr - sb.offset), maxy);
        }

        set_scissor(0);
        doScrollBar(&sb, ht - htHdr - 85*sc, maxy, xorig + wd + sc*(PIX3*2), yorig + htHdr, z, &box, 0);

        y = yorig+ht-60*sc;

        x = xorig+155*sc;

        buf[0] = '\0';
        strcat(buf,textStd("TitleSelectYourNameWill"));
        strcat(buf," ");
        strcat(buf,textStd("TitleSelectLookLikeThis"));
         font(&game_12);
        cprnt(x, y, z, sc, sc, buf);
        y+=45*sc;

         font(&entityNameText);
          font_color(s_iColor1, s_iColor2);

        if(s_iBase && s_pBase->piAttach[s_iBase] && !(s_iCommon>0 || s_iOrigin>0)) // VD - attach the article to the name only if it should be and there are no additional titles
        {
            cprntEx( x, y, z, sc*1.2, sc*1.2, NO_MSPRINT|CENTER_X, strInsert(e->name, e->name, textStd(s_pBase->ppchTitles[s_iBase])));
        }
        else
        {
             font_color(getReticleColor(e, 0, 1, 1), getReticleColor(e, 0, 0, 1));
            cprntEx( x, y, z, sc*1.2, sc*1.2, NO_MSPRINT|CENTER_X, e->name );
        }

        font_color(s_iColor1, s_iColor2);
        buf[0]='\0';
        if(s_iBase>0 && (!s_pBase->piAttach[s_iBase] || (s_iCommon>0 || s_iOrigin>0)))
        {
            strcat(buf, textStd(s_pBase->ppchTitles[s_iBase]));
        }

        if(s_iCommon>0)
        {
            if(buf[0]!='\0' && !s_pBase->piAttach[s_iBase]) strcat(buf, " ");
            if(s_iCommon < eaSize(&s_pCommon->ppchTitles))
                strcat(buf, textStd(s_pCommon->ppchTitles[s_iCommon_display]));
            else
                strcat(buf, textStd(s_pVeteran->ppchTitles[s_iVeteran_display]));
        }
        if(s_iOrigin>0)
        {
            if(buf[0]!='\0' && (s_iCommon || !s_pBase->piAttach[s_iBase])) strcat(buf, " ");
            strcat(buf, textStd(s_pOrigin->ppchTitles[s_iOrigin_display]));
        }
        if(buf[0]!='\0')
        {
            int width = str_wd(font_grp, sc*.75*1.2, sc*.75*1.2, buf);
            cprnt(x, y-14*sc*.75*1.2, z, sc*.75*1.2, sc*.75*1.2, buf);
        }

        x=xorig+wd+sc*(-50);
        y=yorig;

        if(D_MOUSEHIT==drawStdButton(x, y+ht+sc*(-55), z, 100*sc, 35*sc, CLR_GREEN, "TitleSelectSetTitle", 1.2f*sc, 0))
        {
            sendTitles(s_iBase, s_iCommon, s_iOrigin, s_iColor1, s_iColor2 );
            title_set(e, s_iBase, s_iCommon, s_iOrigin, s_iColor1, s_iColor2 );

            window_setMode(WDW_TITLE_SELECT, WINDOW_SHRINKING);
            window_setMode(WDW_CONTACT_DIALOG, WINDOW_GROWING);

            // Continue the dialog with the NPC
            START_INPUT_PACKET(pak, CLIENTINP_CONTACT_DIALOG_RESPONSE);
            pktSendBitsPack(pak, 1, CONTACTLINK_HELLO);
            END_INPUT_PACKET
        }
        else if(D_MOUSEHIT==drawStdButton(x, y+ht+sc*(-20), z, 80*sc, 20*sc, CLR_RED, "TitleSelectChooseLater", 0.75f*sc, 0))
        {
            window_setMode(WDW_TITLE_SELECT, WINDOW_SHRINKING);
            window_setMode(WDW_CONTACT_DIALOG, WINDOW_GROWING);

            // Continue the dialog with the NPC
            START_INPUT_PACKET(pak, CLIENTINP_CONTACT_DIALOG_RESPONSE);
            pktSendBitsPack(pak, 1, CONTACTLINK_HELLO);
            END_INPUT_PACKET
        }
    }

     return 0;
}

/**********************************************************************func*
 * titleselectOpen
 *
 */
void titleselectOpen(bool bOrigin, bool bVeteran)
{
    int i;
    Entity *e = playerPtr();
    TitleDictionary * pDict = &g_TitleDictionary;
    
    if( ENT_IS_IN_PRAETORIA(e) )
    {
        if( ENT_IS_VILLAIN(e) )
            pDict = &g_l_TitleDictionary;
        else
            pDict = &g_p_TitleDictionary;
    }
    else
    {
        if( ENT_IS_VILLAIN(e) )
            pDict = &g_v_TitleDictionary;
    }

    if(!pDict->ppOrigins)
        return;

    s_bShowOrigin = bOrigin;
    s_bShowVeteran = bVeteran;

    s_pBase = 0;
    s_pCommon = 0;
    s_pVeteran = 0;
    s_pOrigin = 0;

    if( e->pl->titleColors[0] && e->pl->titleColors[1])
    {
        pickerSetfromIntColor( &cp[0], e->pl->titleColors[0]);
        pickerSetfromIntColor( &cp[1], e->pl->titleColors[1]);
    }
    else
    {
        pickerSetfromIntColor( &cp[0], CLR_PARAGON);
        pickerSetfromIntColor( &cp[1], CLR_PARAGON);
    }

    for(i=0; i<eaSize(&pDict->ppOrigins); i++)
    {
        if(stricmp(pDict->ppOrigins[i]->pchOrigin, "Base")==0)
        {
            s_pBase = pDict->ppOrigins[i];
        }
        else if(stricmp(pDict->ppOrigins[i]->pchOrigin, "Common")==0)
        {
            s_pCommon = pDict->ppOrigins[i];
        }
        else if(stricmp(pDict->ppOrigins[i]->pchOrigin, "Veteran")==0)
        {
            s_pVeteran = pDict->ppOrigins[i];
        }
        else if(stricmp(pDict->ppOrigins[i]->pchOrigin, e->pchar->porigin->pchName)==0)
        {
            s_pOrigin = pDict->ppOrigins[i];
        }
    }

    s_iBase = 0;
    s_iCommon = 0;
    s_iOrigin = 0;

    for(i=0; i<eaSize(&s_pBase->ppchTitles); i++)
    {
        if(stricmp(e->pl->titleTheText, s_pBase->ppchTitles[i])==0)
        {
            s_iBase = i;
            break;
        }
    }

    for(i=0; i<eaSize(&s_pCommon->ppchTitles); i++)
    {
        if(stricmp(e->pl->titleCommon, s_pCommon->ppchTitles[i])==0)
        {
            s_iCommon = i;
            break;
        }
    }
    if(s_bShowVeteran && i >= eaSize(&s_pCommon->ppchTitles))
    {
        for(i=0; i<eaSize(&s_pVeteran->ppchTitles); i++)
        {
            if(stricmp(e->pl->titleCommon, s_pVeteran->ppchTitles[i])==0)
            {
                s_iCommon = i+eaSize(&s_pCommon->ppchTitles);
                break;
            }
        }
    }

    if(s_bShowOrigin)
    {
        for(i=0; i<eaSize(&s_pOrigin->ppchTitles); i++)
        {
            if(stricmp(e->pl->titleOrigin, s_pOrigin->ppchTitles[i])==0)
            {
                s_iOrigin = i;
                break;
            }
        }
    }

    window_setMode(WDW_TITLE_SELECT, WINDOW_DISPLAYING);
    window_setMode(WDW_CONTACT_DIALOG, WINDOW_SHRINKING);
}

/* End of File */
