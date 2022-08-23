/***************************************************************************
*     Copyright (c) 2005-2006, Cryptic Studios
*     All Rights Reserved
*     Confidential Property of Cryptic Studios
***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/mathutil.h>

#include "entity/entity.h"
#include "entity/Supergroup.h"

#include "gameComm/wdwbase.h"
#include "render/sprite.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"

#include "player/player.h"
#include "cmdparse/cmdgame.h"
#include "graphics/camera.h"
#include "UI/uiBox.h"
#include "UI/uiUtil.h"
#include "UI/uiInput.h"
#include "UI/uiWindows.h"
#include "UI/uiClipper.h"
#include "UI/uiUtilGame.h"
#include "UI/uiTabControl.h"
#include "uiSlider.h"
#include "UI/uiScrollBar.h"
#include "UI/uiBaseInput.h"
#include "UI/uiBaseRoom.h"
#include "UI/uiBaseInventory.h"
#include "UI/uiToolTip.h"
#include "UI/uiGame.h"
#include "uiColorPicker.h"
#include "UI/uiNet.h"

#include "bases/bases.h"
#include "bases/basedata.h"
#include "bases/baseedit.h"
#include "bases/baseinv.h"
#include "bases/basesystems.h"
#include "bases/baseclientsend.h"

#include "formatter/smf_main.h"
#include "uiSMFView.h"

#include "graphics/ttFontUtil.h"
#include "graphics/groupThumbnail.h"
#include <utilitieslib/language/MessageStoreUtil.h>

TextAttribs gTextTitleAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.2f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_14,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

static TextAttribs gTextWarningAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xff2222ff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xff2222ff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};


char *roomdecor_desc[] = 
{
    "Floor_0_desc",
        "Floor_4_desc",
        "Floor_8_desc",
        "Ceiling_32_desc",
        "Ceiling_40_desc",
        "Ceiling_48_desc",
        "Wall_Lower_4_desc",
        "Wall_Lower_8_desc",
        "Wall_16_desc",
        "Wall_32_desc",
        "Wall_48_desc",
        "Wall_Upper_32_desc",
        "Wall_Upper_40_desc",
        "TrimLow_desc",
        "TrimHi_desc",
        "Doorway_desc",
}; 


void baseLookRoom(BaseRoom *room, int block[2]);

static int g_iSelectedTab;
static RoomDetail *s_pCurObject = NULL;
static int g_iCurObj = 0;

void SelectTab(void *data)
{
    g_iSelectedTab = (int)data;
}
#define LINE_HEIGHT 14
int basepropsItem(float x, float y, float z, float wd, float ht, float sc, int color, int bcolor)
{
    //int i;
    float xp[2], spr_sc, text_y, button_wd, text_wd;
    AtlasTex *spin[2], *ospin[2], *aspin[2];
    AtlasTex *item_pic;
    const Detail * detail;
    AtlasTex *engIcon = atlasLoadTexture("base_icon_power.tga");
    AtlasTex *conIcon = atlasLoadTexture("base_icon_control.tga");
    CBox box;

    static ToolTip powerTip, controlTip;

    //CBox box;
    int wy=0;
    int badcolor = 0xff4444ff;
    int goodcolor = 0x44ff44ff;
    int pulsecolor = 0, energycol, controlcol;
    static float pulsefloat = 0;
    float pulseper;

    pulsefloat += 0.15*TIMESTEP;
    pulseper = (sinf(pulsefloat)+1)/2;

     interp_rgba(pulseper,0xff4444ff,0x882222ff,&pulsecolor);
    interp_rgba(pulseper,0x8888ffff,0xff4444ff,&energycol);
    interp_rgba(pulseper,0xff66ffff,0xff4444ff,&controlcol);

    spin[0]  = atlasLoadTexture( "costume_button_spin_L_tintable.tga" );
    spin[1]  = atlasLoadTexture( "costume_button_spin_R_tintable.tga" );
    ospin[0] = atlasLoadTexture( "costume_button_spin_over_L_tintable.tga" );
    ospin[1] = atlasLoadTexture( "costume_button_spin_over_R_tintable.tga" );
    aspin[0] = atlasLoadTexture( "costume_button_spin_activate_L_grey.tga" );
    aspin[1] = atlasLoadTexture( "costume_button_spin_activate_R_grey.tga" );

    xp[0] = x+R10*sc;
    xp[1] = x + wd - (R10+spin[0]->width)*sc;
    wy += 15*sc;

    if( !g_refCurDetail.p || !g_refCurDetail.p->info || !g_refCurDetail.p->info->pchGroupName )
        return 0;

    addToolTip( &powerTip );
    addToolTip( &controlTip );

    // first build a header of the info we care about
    detail = g_refCurDetail.p->info;
    font(&game_12);
    font_color(CLR_WHITE,CLR_WHITE);

    text_y = y + ht - 16*sc;
    button_wd = MIN( 80, wd-(spin[0]->width+R10)*sc*2);
    
     if (g_refCurDetail.p->id)
    {    
        char * str;
        float cx;

         cx = x + (wd-button_wd)/4;

         if( detail->iEnergyConsume )
        {
             str = textStd("ItemEnergy",-detail->iEnergyConsume);
            text_wd = (engIcon->width*sc + str_wd(font_grp,sc,sc,str))/2;
            BuildCBox( &box, cx-text_wd/2, text_y-engIcon->height*sc/2, text_wd, engIcon->height*sc );

            if(g_refCurDetail.p->bPowered )
            {
                font_color(CLR_WHITE,CLR_WHITE);
                display_sprite( engIcon, cx+text_wd-engIcon->width*sc, text_y-engIcon->height*sc/2, z,sc,sc, 0x8888ffff );
                setToolTipEx( &powerTip, &box, textStd("ItemEnergyConsumeTip",detail->iEnergyConsume), NULL, MENU_GAME, WDW_BASE_PROPS, 1, TT_NOTRANSLATE );
            }
            else
            {
                font_color(pulsecolor,pulsecolor);
                display_sprite( engIcon, cx+text_wd-engIcon->width*sc, text_y-engIcon->height*sc/2, z,sc,sc, energycol );
                setToolTipEx( &powerTip, &box, textStd("ItemEnergyConsumeUnpoweredTip",detail->iEnergyConsume), NULL, MENU_GAME, WDW_BASE_PROPS, 1, TT_NOTRANSLATE);
            }

               cprntEx(cx-text_wd,text_y+2,z,sc,sc,CENTER_Y,str);
        }
        else if( detail->iEnergyProduce )
        {
            font_color(goodcolor,goodcolor);
             str = textStd("ItemEnergy",detail->iEnergyProduce);
            text_wd = (engIcon->width*sc + str_wd(font_grp,sc,sc,str))/2;
            cprntEx(cx-text_wd,text_y+2,z,sc,sc,CENTER_Y,str);
            display_sprite( engIcon, cx+text_wd-engIcon->width*sc, text_y-engIcon->height*sc/2, z,sc,sc, 0x8888ffff );
            BuildCBox( &box, cx-text_wd/2, text_y-engIcon->height*sc/2, text_wd, engIcon->height*sc );
            setToolTipEx( &powerTip, &box, textStd("ItemEnergyProduceTip",detail->iEnergyProduce), NULL, MENU_GAME, WDW_BASE_PROPS, 1, TT_NOTRANSLATE );
        }
        else
            clearToolTip( &powerTip );

         cx = x + wd - (wd-button_wd)/4;


         if( detail->iControlConsume )
         {
            str = textStd("ItemControl",-detail->iControlConsume);
            text_wd = engIcon->width*sc + str_wd(font_grp,sc,sc,str);
            BuildCBox( &box, cx-text_wd/2, text_y-engIcon->height*sc/2, text_wd, engIcon->height*sc );

            if(g_refCurDetail.p->bControlled )
            {
                font_color(CLR_WHITE,CLR_WHITE);
                display_sprite( conIcon, cx+text_wd/2-engIcon->width*sc, text_y-conIcon->height*sc/2, z,sc,sc, 0xff66ffff );
                setToolTipEx( &controlTip, &box, textStd("ItemControlConsumeTip",detail->iControlConsume), NULL, MENU_GAME, WDW_BASE_PROPS, 1, TT_NOTRANSLATE );
            }
            else
            {
                font_color(pulsecolor,pulsecolor);
                display_sprite( conIcon, cx+text_wd/2-engIcon->width*sc, text_y-conIcon->height*sc/2, z,sc,sc, controlcol );
                setToolTipEx( &controlTip, &box, textStd("ItemControlConsumeUncontrolledTip",detail->iControlConsume), NULL, MENU_GAME, WDW_BASE_PROPS, 1, TT_NOTRANSLATE );
             }
            
             cprntEx(cx-text_wd/2,text_y+2,z,sc,sc,CENTER_Y,str);
        }
        else if( detail->iControlProduce )
        {
            font_color(goodcolor,goodcolor);
            str = textStd("ItemControl",detail->iControlProduce);
            text_wd = engIcon->width*sc + str_wd(font_grp,sc,sc,str);
             cprntEx(cx-text_wd/2,text_y+2,z,sc,sc,CENTER_Y,str);
            display_sprite( conIcon, cx+text_wd/2-engIcon->width*sc, text_y-conIcon->height*sc/2, z,sc,sc, 0xff66ffff );
            BuildCBox( &box, cx-text_wd/2, text_y-engIcon->height*sc/2, text_wd, engIcon->height*sc );
            setToolTipEx( &controlTip, &box, textStd("ItemControlProduceTip",detail->iControlProduce), NULL, MENU_GAME, WDW_BASE_PROPS, 1, TT_NOTRANSLATE );
        }
        else
            clearToolTip( &controlTip );

         if(detail->iMaxAuxAllowed > 0)
        {    
            font_color(CLR_WHITE,CLR_WHITE);
            cprntEx(x+wd/2,y+wy,z,sc,sc,CENTER_X,"ItemCurMaxAux",g_refCurDetail.iNumAttachedAux,detail->iMaxAuxAllowed);
            //wy+=LINE_HEIGHT*sc;
        }
        else if(IS_AUXILIARY(detail->eFunction))
        {
            if(g_refCurDetail.p->bPowered && g_refCurDetail.p->bControlled && g_refCurDetail.p->iAuxAttachedID)            
            {            
                font_color(CLR_WHITE,CLR_WHITE);
                cprntEx(x+wd/2,y+wy,z,sc,sc,CENTER_X,"DetailAuxAttached");
            }
            else
            {
                font_color(pulsecolor,pulsecolor);
                cprntEx(x+wd/2,y+wy,z,sc,sc,CENTER_X,"DetailAuxUnattached");
            }
            //wy+=LINE_HEIGHT*sc;
        }
    }
    font_color(CLR_WHITE,CLR_WHITE);
    wy -= 15*sc;
    // Now draw the picture
    item_pic = groupThumbnailGet(detailGetThumbnailName(detail), true, detail->eSurface==kSurface_Ceiling, 0); 
    spr_sc = MIN( (wd-2*R10*sc)/item_pic->width, ((ht-wy)-4*R10*sc)/item_pic->height );
    display_sprite(item_pic, x + (wd-item_pic->width*spr_sc)/2, y + wy + TAB_HEIGHT, z, spr_sc, spr_sc, CLR_WHITE );


    // Sell Buttons
    if (baseedit_Mode() == kBaseEdit_Architect && !baseEdit_state.detail_dragging)
    {
        int cant_delete = (detail->bCannotDelete && game_state.beta_base != 2) || 
                            stricmp("Entrance", detail->pchCategory) == 0 ||
                            eaSize(&g_refCurDetail.p->storedEnhancement) ||
                            eaSize(&g_refCurDetail.p->storedInspiration) ||
                            eaSize(&g_refCurDetail.p->storedSalvage);

        if( D_MOUSEHIT == drawStdButton( x + wd/2, y + ht - 16*sc, z,button_wd, 20*sc,color, "SellString", sc, cant_delete))
        {
            baseedit_DeleteCurDetail();
            baseedit_SetCurDetail(NULL,NULL);
        }
    }
    else if (baseEdit_state.detail_dragging)
    {
        if( D_MOUSEHIT == drawStdButton( x + wd/2, y + ht - 16*sc, z, button_wd, 20*sc,color, "CancelString", sc, 0))
        {
            baseedit_CancelDrags();
            baseedit_CancelSelects();
        }
    }


    // Rotate buttons
/*    for( i = 0; i < 2; i++ )
    {
        build_cboxxy( spin[i], &box, sc, sc, xp[i], y + ht - (PIX3*2+spin[0]->height)*sc );

        if( mouseCollision(&box) )
        {
            if( isDown( MS_LEFT ))
            {
                display_sprite( aspin[i], xp[i], y + ht - (PIX3*2+spin[0]->height)*sc, z, sc, sc, color|0xff );
            }
            else
                display_sprite( ospin[i], xp[i], y + ht - (PIX3*2+spin[0]->height)*sc, z, sc, sc, color|0xff );

            if( mouseClickHit( &box, MS_LEFT ) )
                baseedit_RotateCurDetail( i );
        }
        else
            display_sprite( spin[i], xp[i], y + ht - (PIX3*2+spin[0]->height)*sc, z, sc, sc, color|0xff );
    }*/

    return 0;
}

extern int gShowHiddenDetails;

#define HR(w,h) addStringToStuffBuff(&sb, "<img border=0 width=1 height=%d color=#00000000><span align=center><img border=0 src=Headerbar_HorizDivider_L.tga height=1 width=20><img border=0 src=white.tga width=%i height=1><img border=0 src=Headerbar_HorizDivider_R.tga height=1 width=20></span><img border=0 width=1 height=%d color=#00000000>",h/2,w - 40,h/2)

char * getBaseDetailDescriptionString( const Detail * detail, const RoomDetail * room_detail, int width )
{
    static StuffBuff sb;
    int secondHR = 0;

    if( !detail )
        return NULL;

    if (!sb.buff) initStuffBuff(&sb,512);

    clearStuffBuff(&sb);
    addSingleStringToStuffBuff(&sb, detailGetDescription(detail,1) );
    addStringToStuffBuff(&sb, "<br>");
    if (width)
        HR(width,8);
    addStringToStuffBuff(&sb, "<linkhoverbg #578836><linkhover #a7ff6c><link #52ff7d><scale .80><table border=3>" );

    if( detail->pchCategory )
    {
        const DetailCategory *cat = basedata_GetDetailCategoryByName(detail->pchCategory);
        if (cat)
        {        
            addStringToStuffBuff(&sb, textStd("ItemCategoryTable"), cat->pchDisplayName);
            secondHR = 1;
        }
    }

    if( room_detail && room_detail->creator_name[0] )
    {
        addStringToStuffBuff(&sb, textStd("PlacedByTable"), room_detail->creator_name );
        secondHR = 1;
    }

    if( detail->iCostPrestige )
    {
        addStringToStuffBuff(&sb, textStd("ItemCostTable"), detail->iCostPrestige );
        secondHR = 1;
    }

    if( detail->iUpkeepPrestige )
    {
        addStringToStuffBuff(&sb, textStd("ItemUpkeepTable", detail->iUpkeepPrestige) );
        secondHR = 1;
    }

    if (detail->iEnergyProduce)
    {
        addStringToStuffBuff(&sb, textStd("ItemEnergyProduceTable", detail->iEnergyProduce) );
        secondHR = 1;
    }
    if (detail->iEnergyConsume)
    {
        addStringToStuffBuff(&sb, textStd("ItemEnergyConsumeTable", detail->iEnergyConsume) );
        secondHR = 1;
    }
    if (detail->iControlProduce)
    {
        addStringToStuffBuff(&sb, textStd("ItemControlProduceTable", detail->iControlProduce) );
        secondHR = 1;
    }
    if (detail->iControlConsume)
    {
        addStringToStuffBuff(&sb, textStd("ItemControlConsumeTable", detail->iControlConsume) );
        secondHR = 1;
    }    
    if(detail->iMaxAuxAllowed > 0)
    {
        int i,size = eaSize(&detail->ppchAuxAllowed);
        int found = 0;
        addStringToStuffBuff(&sb, textStd("ItemMaxAuxTable",detail->iMaxAuxAllowed));
        addStringToStuffBuff(&sb, "</table></scale>");
        if (width && secondHR)
            HR(width,12);
        addStringToStuffBuff(&sb, "<br>");
        addStringToStuffBuff(&sb, textStd("AllowedAux"));
        addStringToStuffBuff(&sb, textStd("<scale 0.8><table border=3><tr border=1><td border=0>"));
        for (i = 0; i < size; i++)
        {
            const Detail *aux = basedata_GetDetailByName(detail->ppchAuxAllowed[i]);
            if (!aux)
                continue;
            if (!gShowHiddenDetails && (detail->bObsolete || 
                (!baseDetailMeetsRequires(aux, playerPtr()) && aux->ppchRequires  )))
                continue;
            if (found)
                addStringToStuffBuff(&sb, ", ");
            found = 1;
            addStringToStuffBuff(&sb, textStd(aux->pchDisplayName));
        }
        if (!found)
        {
            addStringToStuffBuff(&sb, textStd("NoneAvailable"));
        }
        addStringToStuffBuff(&sb, textStd("</td></tr></table></scale>"));
    }
    else if(!IS_AUXILIARY(detail->eFunction))
    {
        addStringToStuffBuff(&sb, "</table></scale>");
        if (width && secondHR)
            HR(width,12);
        addStringToStuffBuff(&sb, "<br>");
        addStringToStuffBuff(&sb, textStd("ItemDoesNotAllowAux"));
    }
    else if(IS_AUXILIARY(detail->eFunction))
    {
        addStringToStuffBuff(&sb, "</table></scale>");
        if (width && secondHR)
            HR(width,12);
        addStringToStuffBuff(&sb, "<br>");
        if (room_detail && room_detail->iAuxAttachedID)
        {
            RoomDetail *attached = baseGetDetail(&g_base,room_detail->iAuxAttachedID,NULL);
            if (attached)
            {
                addStringToStuffBuff(&sb, textStd("DetailAttachedTo") );
                addStringToStuffBuff(&sb, "<scale 0.8><table border=3>");
                addStringToStuffBuff(&sb, textStd("DetailLink",attached->info->pchDisplayName,attached->id) );            
                addStringToStuffBuff(&sb, "</table></scale>");
            }            
        } 
        else
        {
            addStringToStuffBuff(&sb, textStd("DetailNeedsToAttach") );
        }
    }
    addStringToStuffBuff(&sb, "</linkhover></linkhoverbg></link>");
     return sb.buff;
}


char * getBaseDetailAuxString( const Detail * detail, RoomDetail * room_detail,int width)
{
    static StuffBuff sb;

    if( !detail )
        return NULL;

    if (!sb.buff) initStuffBuff(&sb,512);

    clearStuffBuff(&sb);
    addStringToStuffBuff(&sb,"<linkhoverbg #578836><linkhover #a7ff6c><link #52ff7d>");
    if(detail->iMaxAuxAllowed > 0)
    {
        int i,size, found = 0;
        BaseRoom *room;
        RoomDetail ** details;
        addStringToStuffBuff(&sb, textStd("ItemCurMaxAuxLong",g_refCurDetail.iNumAttachedAux,detail->iMaxAuxAllowed));
        room = baseGetRoom(&g_base,room_detail->parentRoom);
        if (room) {        
            details = findAttachedAuxiliary(room,room_detail);
            size = eaSize(&details);
            if (size > 0)
            {                
                HR(width,10);
                addStringToStuffBuff(&sb, "<br>");
                addStringToStuffBuff(&sb, textStd("AttachedAux"));
                addStringToStuffBuff(&sb, textStd("<scale 0.8><table border=3>"));
                for (i = 0; i < size; i++)
                {                
                    addStringToStuffBuff(&sb, textStd("DetailLink",details[i]->info->pchDisplayName,details[i]->id));
                }
                addStringToStuffBuff(&sb, textStd("</table></scale>"));
            }            
        }
        HR(width,10);
        addStringToStuffBuff(&sb, "<br>");
        addStringToStuffBuff(&sb, textStd("AllowedAux"));
        addStringToStuffBuff(&sb, textStd("<scale 0.8><table border=3><tr border=1><td border=0>"));
        size = eaSize(&detail->ppchAuxAllowed);
        for (i = 0; i < size; i++)
        {
            const Detail *aux = basedata_GetDetailByName(detail->ppchAuxAllowed[i]);
            if (!aux)
                continue;
            if (!gShowHiddenDetails && (detail->bObsolete || 
                (!baseDetailMeetsRequires(aux, playerPtr()) && aux->ppchRequires  )))
                continue;
            if (found)
                addStringToStuffBuff(&sb, ", ");
            found = 1;
            addStringToStuffBuff(&sb, textStd(aux->pchDisplayName));
        }
        if (!found)
        {
            addStringToStuffBuff(&sb, textStd("NoneAvailable"));
        }
        addStringToStuffBuff(&sb, textStd("</td></tr></table></scale>"));
    }
    else if(IS_AUXILIARY(detail->eFunction))
    {
        addStringToStuffBuff(&sb, textStd("ItemIsAuxiliary") );        
        if (room_detail && room_detail->iAuxAttachedID)
        {
            RoomDetail *attached = baseGetDetail(&g_base,room_detail->iAuxAttachedID,NULL);
            if (attached)
            {
                addStringToStuffBuff(&sb, textStd("DetailAttachedTo") );
                addStringToStuffBuff(&sb, textStd("<scale 0.8><table border=3>"));
                addStringToStuffBuff(&sb, textStd("DetailLink",attached->info->pchDisplayName,attached->id) );            
                addStringToStuffBuff(&sb, textStd("</table></scale>"));
            }            
        }
        addStringToStuffBuff(&sb, "<br>" );
    }

    addStringToStuffBuff(&sb, "</linkhover></linkhoverbg></link>");
    return sb.buff;
}

static int DetailCallback(char *pch)
{
    int id;
    RoomDetail *detail;
    BaseRoom *room;

    // Support cmd: navigation too
    if (!pch || !pch[0])
        return 0;
    id = atoi(pch);

    detail = baseGetDetail(&g_base,id,&room);
    if (!detail)
        return 0;
    baseedit_SetCurDetail(room,detail);

    return 0;
}

int basepropsInfo(float x, float y, float z, float wd, float ht, float sc, int color, int bcolor)
{
    RoomDetail * detail;
    static SMFBlock * smf_info = 0;
    static RoomDetail * last_detail;
    float text_ht;

    if( !g_refCurDetail.p || !g_refCurDetail.p->info )
        return 0;

    if (!smf_info)
    {
        smf_info = smfBlock_Create();
    }

    detail = g_refCurDetail.p;
    text_ht = smf_ParseAndDisplay( smf_info, getBaseDetailDescriptionString(g_refCurDetail.p->info, g_refCurDetail.p,wd-30*sc), 
                                    x + R10*sc, y, z, wd - R10*3*sc, 0, detail != last_detail, detail != last_detail, 
                                    &gTextAttr_White12, DetailCallback, 0, true);
    last_detail = detail;

    return text_ht + 10*sc;
}

int basepropsAux(float x, float y, float z, float wd, float ht, float sc, int color, int bcolor)
{
    RoomDetail * detail;
    static SMFBlock * smf_info = 0;
    static RoomDetail * last_detail;
    float text_ht;

    if( !g_refCurDetail.p || !g_refCurDetail.p->info )
        return 0;

    if (!smf_info)
    {
        smf_info = smfBlock_Create();
    }

    detail = g_refCurDetail.p;
    text_ht = smf_ParseAndDisplay( smf_info, getBaseDetailAuxString(g_refCurDetail.p->info, g_refCurDetail.p,wd-30*sc), 
                                    x + R10*sc, y, z, wd - R10*3*sc, 0, detail != last_detail, detail != last_detail, 
                                    &gTextAttr_White12, DetailCallback, 0, true);
    last_detail = detail;

    return text_ht + 10*sc;
}

#define            TB_FLAG_NONE            0
#define            TB_FLAG_CENTER            (1 << 0)

static RoomDetail *s_curDetail = NULL;
static int s_getDetailFromID = 0;
static int s_detailID;

int basepropsPermissions(float x, float y, float z, float wd, float ht, float sc, int color, int bcolor)
{
    RoomDetail *detail;
    int playerRank = 0;
    //float text_ht;
    int i;
    int column;
    int rank;
    char *title;
    Entity *e;
    Supergroup *sg;
    AtlasTex *filled = atlasLoadTexture("Context_checkbox_filled.tga");
    AtlasTex *empty  = atlasLoadTexture("Context_checkbox_empty.tga");
    AtlasTex *glow   = atlasLoadTexture("costume_button_link_glow.tga");
    static struct textBlock
    {
        char *text;
        float x, y, z, w, h;
        int flags;
    } textBlocks[] = 
    {
        // These are arranged so that rank within the SG indexes into this array properly
        { "@0",             10, 160, 0, 110, 20, TB_FLAG_NONE, },
        { "@1",             10, 135, 0, 110, 20, TB_FLAG_NONE, },
        { "@2",             10, 110, 0, 110, 20, TB_FLAG_NONE, },
        { "@3",             10,  85, 0, 110, 20, TB_FLAG_NONE, },
        { "@4",             10,  60, 0, 110, 20, TB_FLAG_NONE, },
        { "@5",             10,  35, 0, 110, 20, TB_FLAG_NONE, },
        { "PutString",    125,  10, 0,  40, 20, TB_FLAG_CENTER, },
        { "GetString",    175,  10, 0,  40, 20, TB_FLAG_CENTER, },
    };
    static SMFBlock *smf_info[ARRAY_SIZE(textBlocks)];

    e = playerPtr();
    if (e == NULL || e->supergroup == NULL)
    {
        return 0;
    }
    sg = e->supergroup;
    if (s_getDetailFromID)
    {
        detail = baseGetDetail(&g_base, s_detailID, NULL); 
    }
    else
    {
        if (s_curDetail == NULL)
        {
            return 0;
        }
        detail = s_curDetail;
    }
    if (detail->permissions == 0)
    {
        // No permissions set, just provide a sane init value.
        detail->permissions = STORAGE_BIN_DEFAULT_PERMISSIONS;
    }

    for(i = 0; i < eaSize(&e->supergroup->memberranks); i++)
    {
        if(e->supergroup->memberranks[i]->dbid == e->db_id)
        {
            playerRank = e->supergroup->memberranks[i]->rank;
            break;
        }
    }

    if (smf_info[0] == NULL)
    {
        for (i = 0; i < ARRAY_SIZE(textBlocks); i++)
        {
            smf_info[i] = smfBlock_Create();
            if (textBlocks[i].flags & TB_FLAG_CENTER)
            {
                smf_SetFlags(smf_info[i], 0, 0, 0, 0, 0, 0, 0, 0, SMAlignment_Center, 0, 0, 0);
            }
            title = textBlocks[i].text;
            if (title[0] == '@')
            {
                rank = atoi(&title[1]);
                title = sg->rankList[rank].name;
            }
            else
            {
                title = textStd(title);
            }
            smf_SetRawText(smf_info[i], title, false);
        }
    }
    for (i = 0; i < ARRAY_SIZE(textBlocks); i++)
    {
        smf_Display(smf_info[i],
                        textBlocks[i].x + x,
                        textBlocks[i].y + y,
                        textBlocks[i].z + z,
                        textBlocks[i].w,
                        textBlocks[i].h,
                        1, 1, &gTextAttr_White12, NULL);                  
    }

// Sanity check that the indexing we do inside these two loops to evaluate tx and ty will 
// access the correct members of the textBlocks array.  Access the first NUM_SG_RANKS_VISIBLE
// members to determine ty, as rank iterates, and then the last two to set the columns as
// column iterates.  This means that textBlocks *must* be correctly ordered entries for row 
// first, with two entries on the end for the two columns.

STATIC_INFUNC_ASSERT(ARRAY_SIZE(textBlocks) == NUM_SG_RANKS_VISIBLE + 2)

     for(rank = 0; rank < NUM_SG_RANKS_VISIBLE; rank++)
    {
        // column == 0 => Put, column == 1 => Get
        for(column = 0; column < 2; column++)
        {
            CBox cbox;
            float sc = 1.2f;
            // The first (NUM_SG_RANKS_WITH_VARIABLE_PERMISSIONS + 1) entries in textBlocks[] look after
            // The rank strings.  The last two handle the "put and get" strings.
            float tx = x + textBlocks[NUM_SG_RANKS_VISIBLE + column].x + 12;
            float ty = y + textBlocks[rank].y + 7;
            // Set up a one bit mask 
            int bit = 1 << (column * 16 + rank);
            int set = detail->permissions & bit ? 1 : 0;
            // Added a second check so that GM's can't accidentally change superleader permissions
            int allowed = ((rank < playerRank && srCanISetPermissions()) || playerRank == NUM_SG_RANKS - 2) && rank < NUM_SG_RANKS_WITH_VARIABLE_PERMISSIONS;
                
            BuildCBox(&cbox, tx - ABS(empty->width-glow->width*.5f)*sc/2, ty -  12, (glow->width*.5f*sc), 24 );

            if( mouseCollision(&cbox) && allowed )
            {
                display_sprite( glow, tx+(empty->width-glow->width*.5f)*sc/2, ty-glow->height*.5f*sc/2, z+1, .5f*sc, .5f*sc, CLR_WHITE );
                if( mouseClickHit(&cbox, MS_LEFT) )
                {
                    if (set)
                    {
                        detail->permissions &= ~bit;
                    }
                    else
                    {
                        detail->permissions |= bit;
                    }
                    set = !set;
                }
            }

            display_sprite( empty, tx, ty-empty->height*sc/2, z+1, sc, sc, allowed?CLR_WHITE:0xffffff88 );

            if (set)
            {
                display_sprite( filled, tx, ty-empty->height*sc/2, z+1, sc, sc, allowed?CLR_WHITE:0xffffff88 );
            }
        }
    }

    return 185;
}

typedef enum kBlockSegement
{
    HIGH_CEIL,
    LOW_CEIL,
    WALL,
    HIGH_FLOOR,
    LOW_FLOOR,
    SEGMENT_COUNT,
}kBlockSegement;

typedef enum kSegmentState
{
    BLOCK_EMPTY,
    BLOCK_FULL,
    POTENTIAL_FULL,
    POTENTIAL_EMPTY,
} kSegmentState;

#define TEXT_SPACE        60
#define BUTTON_SPACE    30

void basepropsBlock(float x, float y, float z, float wd, float ht, float sc, float color, float bcolor )
{
    float one_ft = (ht - (BUTTON_SPACE+R10*2)*sc)/48;
    float ty = y + R10*sc, twd = MIN( g_base.roomBlockSize*one_ft, .5f*(wd - 2*R10*sc));
    float tx = x + (wd - twd - R10*sc - TEXT_SPACE*sc)/2 + TEXT_SPACE*sc;

    CBox box[SEGMENT_COUNT];
    int state[SEGMENT_COUNT] = {0}, i, wall, door = false, actualHeights[2], setHeights[2];

    BaseBlock * block = g_refCurBlock.p;
    if( !block )
    {
        float floatHeights[2];
        if( !g_refCurRoom.p || !baseedit_IsDoorway( g_refCurRoom.p, g_refCurBlock.block, floatHeights ) )
            return;
        door = true;
        actualHeights[0] = floatHeights[0];
        actualHeights[1] = floatHeights[1];
    }
    else
    {
        actualHeights[0] = block->height[0];
        actualHeights[1] = block->height[1];
    }

    one_ft = MIN( one_ft, twd/g_base.roomBlockSize );
    ty += (ht-(BUTTON_SPACE+R10*2)*sc-48*one_ft)/2;

    BuildCBox( &box[HIGH_CEIL],    tx, ty,              twd, one_ft*7 );
    BuildCBox( &box[LOW_CEIL],    tx , ty+one_ft*8,  twd, one_ft*7 );
    BuildCBox( &box[WALL],        tx,    ty+one_ft*16, twd, one_ft*22 );
    BuildCBox( &box[HIGH_FLOOR],tx, ty+one_ft*39, twd, one_ft*4 );
    BuildCBox( &box[LOW_FLOOR],    tx, ty+one_ft*44, twd, one_ft*4 );

    wall = actualHeights[0]>=actualHeights[1];

    // determine current colors
    if( wall )
    {
        for( i = 0; i < SEGMENT_COUNT; i++ )
            state[i] = BLOCK_FULL;
    }
    else
    {
        if( actualHeights[0] == 4 )
            state[LOW_FLOOR] = BLOCK_FULL;
        if( actualHeights[0] == 8 )
            state[LOW_FLOOR] = state[HIGH_FLOOR] = BLOCK_FULL;
        if( actualHeights[1] == 40 )
            state[HIGH_CEIL] =  BLOCK_FULL;
        if( actualHeights[1] == 32 )
            state[HIGH_CEIL] = state[LOW_CEIL] = BLOCK_FULL;
    }

    // now have mouseover show predicted colors
    if( mouseCollision(&box[HIGH_CEIL]) )
    {
        setHeights[0] = actualHeights[0];
        setHeights[1] = 48;

        if(wall) // this is wall
        {
            setHeights[0] = 8;
            state[HIGH_CEIL] = state[LOW_CEIL] = state[WALL] = POTENTIAL_EMPTY;
        }
        else if( actualHeights[1] == 32 )
            state[HIGH_CEIL] = state[LOW_CEIL] = POTENTIAL_EMPTY;
        else if( actualHeights[1] == 40 ) 
            state[HIGH_CEIL] = POTENTIAL_EMPTY;
        else if( actualHeights[1] == 48 ) 
        {
            setHeights[1] = 40;
            state[HIGH_CEIL] = POTENTIAL_FULL;
        }

        if(mouseClickHit(&box[HIGH_CEIL], MS_LEFT))
            baseedit_SetCurBlockHeights(setHeights);
    }

    if( mouseCollision(&box[LOW_CEIL]) )
    {
        setHeights[0] = actualHeights[0];

        if(wall) // this is wall
        {
            setHeights[0] = 8;
            setHeights[1] = 40;
            state[LOW_CEIL] = state[WALL] = POTENTIAL_EMPTY;
        }
        else if( actualHeights[1] == 32 )
        {
            setHeights[1] = 40;
            state[LOW_CEIL] = POTENTIAL_EMPTY;
        }
        else if( actualHeights[1] == 40 ) 
        {
            setHeights[1] = 32;
            state[LOW_CEIL] = POTENTIAL_FULL;
        }
        else if( actualHeights[1] == 48 ) 
        {
            setHeights[1] = 32;
            state[LOW_CEIL] = state[HIGH_CEIL] = POTENTIAL_FULL;
        }

        if(mouseClickHit(&box[LOW_CEIL], MS_LEFT))
            baseedit_SetCurBlockHeights(setHeights);
    }

    if( mouseCollision(&box[WALL]) && !door )
    {
        setHeights[0] = setHeights[1] = 0;

        if(wall) // this is wall
        {
            setHeights[0] = 8;
            setHeights[1] = 32;
            state[WALL] = POTENTIAL_EMPTY;
        }
        else 
        {
            for( i = 0; i < SEGMENT_COUNT; i++ )
            {
                if( state[i] != BLOCK_FULL )
                    state[i] = POTENTIAL_FULL;
            }
        }

        if(mouseClickHit(&box[WALL], MS_LEFT))
            baseedit_SetCurBlockHeights(setHeights);
    }

    if( mouseCollision(&box[HIGH_FLOOR]) )
    {
        setHeights[1] = actualHeights[1];

        if(wall) // this is wall
        {
            setHeights[0] = 4;
            setHeights[1] = 32;
            state[HIGH_FLOOR] = state[WALL] = POTENTIAL_EMPTY;
        }
        else if( actualHeights[0] == 8 )
        {
            setHeights[0] = 4;
            state[HIGH_FLOOR] = POTENTIAL_EMPTY;
        }
        else if( actualHeights[0] == 4 ) 
        {
            setHeights[0] = 8;
            state[HIGH_FLOOR] = POTENTIAL_FULL;
        }
        else if( actualHeights[0] == 0 ) 
        {
            setHeights[0] = 8;
            state[HIGH_FLOOR] = state[LOW_FLOOR] = POTENTIAL_FULL;
        }

        if(mouseClickHit(&box[HIGH_FLOOR], MS_LEFT))
            baseedit_SetCurBlockHeights(setHeights);
    }

    if( mouseCollision(&box[LOW_FLOOR]) )
    {
        setHeights[1] = actualHeights[1];
        setHeights[0] = 0;

        if(wall) // this is wall
        {
            setHeights[1] = 32;
            state[LOW_FLOOR] = state[HIGH_FLOOR] = state[WALL] = POTENTIAL_EMPTY;
        }
        else if( actualHeights[0] == 8 )
            state[LOW_FLOOR] = state[HIGH_FLOOR] = POTENTIAL_EMPTY;
        else if( actualHeights[0] == 4 ) 
            state[LOW_FLOOR] = POTENTIAL_EMPTY;
        else if( actualHeights[0] == 0 ) 
        {
            setHeights[0] = 4;
            state[LOW_FLOOR] = POTENTIAL_FULL;
        }

        if(mouseClickHit(&box[LOW_FLOOR], MS_LEFT))
            baseedit_SetCurBlockHeights(setHeights);
    }

    font( &game_12 );
    font_color( 0xffffff88, 0xffffff88 );
    for( i = 0; i < SEGMENT_COUNT; i++ )
    {

        switch( state[i] )
        {
        case BLOCK_EMPTY:
            drawBox( &box[i], z, 0xffffff88, 0 );
            break;
        case BLOCK_FULL:
            drawBox( &box[i], z, 0xffffff88, 0xffffff88 );
            break;
        case POTENTIAL_FULL:
            drawBox( &box[i], z, 0xffffff88, 0xffffffcc );
            cprntEx( (box[i].lx+box[i].hx)/2, (box[i].ly+box[i].hy)/2+2*sc, z, sc, sc, CENTER_X|CENTER_Y, "AddString" );
            break;
        case POTENTIAL_EMPTY:
            drawBox( &box[i], z, 0xffffff88, 0xffffff44 );
            cprntEx( (box[i].lx+box[i].hx)/2, (box[i].ly+box[i].hy)/2+2*sc, z, sc, sc, CENTER_X|CENTER_Y, "RemoveString" );
            break;
        }
    }

    cprntEx( tx - TEXT_SPACE*sc, ty + one_ft*8, z, sc, sc, CENTER_Y, "CeilingString" );
    cprntEx( tx - TEXT_SPACE*sc, ty + one_ft*27, z, sc, sc, CENTER_Y, "WallString" );
    cprntEx( tx - TEXT_SPACE*sc, ty + one_ft*44, z, sc, sc, CENTER_Y, "FloorString" );

    {
        float button_wd = MIN( 120*sc, (wd - 3*R10*sc)/2);
        if( !door && D_MOUSEHIT == drawStdButton( x + R10*sc + button_wd/2, y + ht - (PIX3+BUTTON_SPACE)*sc/2, z, button_wd, 24*sc, color, "ApplyToRoom", sc, 0 ))
        {
            baseedit_SetRoomHeights(g_refCurRoom.p, actualHeights);
        }
        if( !door && D_MOUSEHIT == drawStdButton( x + wd - (R10*sc + button_wd/2), y + ht - (PIX3+BUTTON_SPACE)*sc/2, z, button_wd, 24*sc, color, "ApplyToBase", sc, 0 ))
        {
            baseedit_SetBaseHeights(actualHeights);
        }
    }
}

static SMFBlock *smf_block;
static void * last_block_item;

static float basepropsRoom(const RoomTemplate *room, float x, float y, float z, float wd, float ht, float sc, float color, float bcolor )
{
    bool bReparse = false;
    if( room != last_block_item )
    {
        bReparse = true;
        last_block_item = (void*)room;
    }

    if (!smf_block)
    {
        smf_block = smfBlock_Create();
    }

    return smf_ParseAndDisplay( smf_block, baseRoomGetDescriptionString(room, NULL,1,1,wd-30*sc), x + R10*sc, y, z, 
        wd - R10*sc*3, 0, bReparse, bReparse, &gTextAttr_White12, NULL, 0, true) + 10*sc;
}

static float basepropsPlot(const BasePlot * plot, float x, float y, float z, float wd, float ht, float sc, float color, float bcolor )
{
    bool bReparse = false;
    if( plot != last_block_item )
    {
        bReparse = true;
        last_block_item = (void*)plot;
    }

    if (!smf_block)
    {
        smf_block = smfBlock_Create();
    }

    return smf_ParseAndDisplay( smf_block, basePlotGetDescriptionString(plot,wd-30*sc), x + R10*sc, y, z, 
        wd - R10*sc*3, 0, bReparse, bReparse, &gTextAttr_White12, NULL, 0, true) + 10*sc;
}
#define DEFAULTHEIGHT 300
SMFBlock *smf_title;
SMFBlock *smf_desc;
typedef int(*tabWindowFunc)(float x, float y, float z, float wd, float ht, float sc, int color, int bcolor);

int basepropsWindow()
{
    float x, y, ty, z, wd, ht, sc, view_ht, func_ht;
    int color, bcolor;
    const Detail * item;
    static Detail * lastItem = NULL;
    const RoomTemplate * room = NULL;
    UIBox box;
    char *no_item[] = { "NoItem", "PleaseSelectItem" };
    char *room_section[] = { "RoomSectionString", "SetFloorCeiling" };
    char title[256], desc[512];

    static void *last_item = NULL;
    static ScrollBar sb = { WDW_BASE_PROPS };
    static uiTabControl * itemTabs = 0;
    static Color temp_color[2];

    tabWindowFunc func;

    // Do everything common windows are supposed to do.
    if (!window_getDims(WDW_BASE_PROPS, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
        return 0;

    if(window_getMode(WDW_BASE_PROPS) != WINDOW_DISPLAYING)
        return 0;

    if (!smf_title)
    {
        smf_title = smfBlock_Create();
    }

    if (!smf_desc)
    {
        smf_desc = smfBlock_Create();
    }

    item = g_refCurDetail.p?g_refCurDetail.p->info:NULL;

    if( baseEdit_state.room_dragging )
        room = g_pDragRoomTemplate;

     if( baseInventoryIsStyle() ) // Picking out styles, we are gonna hide the floor mover
    {
        if( g_iDecorIdx < 0 )
        {
             strncpyt(title,textStd("AllDecors"), ARRAY_SIZE(title));
            strncpyt(desc,textStd("AllDecorsDesc"), ARRAY_SIZE(desc));
        }
        else
        {
            strncpyt(title,textStd(roomdecor_names[g_iDecorIdx]), ARRAY_SIZE(title));
            strncpyt(desc,textStd(roomdecor_desc[g_iDecorIdx]), ARRAY_SIZE(desc));
        }
    }
    else if( room ) // we are dragging a new room around
    {
        strncpyt(title,textStd(room->pchDisplayName), ARRAY_SIZE(title));
        strncpyt(desc,textStd(room->pchDisplayShortHelp), ARRAY_SIZE(desc));
    }
    else if( item ) // an item is selected
    {
        strncpyt(title,textStd(item->pchDisplayName), ARRAY_SIZE(title));
        strncpyt(desc,detailGetDescription(item,0), ARRAY_SIZE(desc));
    }
    else if (baseedit_Mode() == kBaseEdit_Architect) // just architectin
    {
        strncpyt(title,textStd(room_section[0]), ARRAY_SIZE(title));
        strncpyt(desc,textStd(room_section[1]), ARRAY_SIZE(desc));
    }
    else if (baseedit_Mode() == kBaseEdit_Plot) // plot place mode
    {
        const BasePlot * plot = getBaseInventorySelectedItem();
        if(!plot || !plot->pchDisplayName)
            plot = g_base.plot;

        strncpyt(title,textStd(plot->pchDisplayName), ARRAY_SIZE(title));
        strncpyt(desc,textStd(plot->pchDisplayShortHelp), ARRAY_SIZE(desc));
    }
    else // fallback
    {
        strncpyt(title,textStd(no_item[0]), ARRAY_SIZE(title));
        strncpyt(desc,textStd(no_item[1]), ARRAY_SIZE(desc));
    }

    if( (room && room != last_item) || 
        (item && item != last_item) || 
        (baseInventoryIsStyle() && g_iDecorIdx != (int)last_item ) ||
        (!room && !item && !baseInventoryIsStyle() && last_item ) )
    {
        smf_ParseAndFormat(smf_title, title, 0, 0, 0, wd - R10*sc*2, 1000, false, true, false, &gTextTitleAttr, 0 );
        smf_ParseAndFormat(smf_desc, desc, 0, 0, 0, wd - R10*sc*2, 1000, false, true, true, &gTextAttr_White12, 0 );

        smf_SetFlags(smf_title, 0, 0, 0, 0, 0, 0, 0, 0, SMAlignment_Center, 0, 0, 0);
        smf_ParseAndFormat(smf_title, title, 0, 0, 0, wd - R10*sc*2, 1000, false, false, true, &gTextTitleAttr, 0 );
        if( baseInventoryIsStyle() )
            last_item = (void*)g_iDecorIdx;
        else if( room )    
            last_item = (void*)room;
        else
            last_item = (void*)item;

        temp_color[0] = CreateColorRGB( 128, 128, 128 );
        temp_color[1] = CreateColorRGB( 128, 128, 128 );
    } 

    gTextTitleAttr.piScale = (int*)((int)(SMF_FONT_SCALE*1.2f*sc));
    gTextAttr_White12.piScale = (int*)((int)(SMF_FONT_SCALE*sc));

    // Name
    ty = y + smf_ParseAndDisplay( smf_title, title, x + R10*sc, y + R10*sc, z, wd - R10*sc*2, 0, false, false, 
                                    &gTextTitleAttr, NULL, 0, true) + 15*sc;

    // Short Desc
    ty += smf_ParseAndDisplay( smf_desc, desc, x + R10*sc, ty, z, wd - R10*sc*2, 0, false, false, 
                                    &gTextAttr_White12, NULL, 0, true) + 5*sc;

    drawJoinedFrame2Panel( PIX3, R10, x, y, z, sc, wd, (ty-y), ht - (ty-y), color, bcolor, NULL, NULL );

     if( baseInventoryIsStyle() ) // Picking out styles, we are gonna hide the floor mover
    {
        const RoomTemplate * room = g_refCurRoom.p->info;
        static char *apchColors[] = { "Color1String", "Color2String" };
        static ColorPicker cp[2] = {0};
        DecorSwap    *swap;
        int i, top_ht;

          view_ht = ht - (ty-y)-PIX3*sc;
        top_ht = (ty-y);
        y = ty;
        uiBoxDefine( &box, x+PIX3*sc, ty, wd - 2*PIX3*sc, view_ht );
 
        clipperPush(&box);
        swap = decorSwapFind(g_refCurRoom.p,g_iDecorIdx);

        font( &game_12 );
        font_color(CLR_WHITE, CLR_WHITE);
        ty += 10*sc;

        // copied directly from shannons code
        for(i=0; i<2; i++)
        {
            Color *color;
            ty += 10*sc;
            prnt(x+5*sc, ty-sb.offset, z, sc, sc, apchColors[i]);

            if( swap && swap->tints[0].integer && swap->tints[1].integer )
                color = swap->tints;
            else
                color = temp_color;
            
            pickerSetfromColor( &cp[i], &color[i] );

            ty += 2*sc;
            if( drawColorPicker(&cp[i], x+R10*sc, ty-sb.offset, z, wd-R10*2*sc, 60*sc, baseedit_Mode() == kBaseEdit_AddPersonal) )
            {
                int c1, c2;

                 if( g_iDecorIdx < 0 )
                {
                    c1 = temp_color[0].integer = colorFlip(COLOR_CAST(cp[0].rgb_color)).integer;
                    c2 = temp_color[1].integer = colorFlip(COLOR_CAST(cp[1].rgb_color)).integer;
                }
                else if( i==0 )
                {
                    c1 = colorFlip(COLOR_CAST(cp[0].rgb_color)).integer;
                    c2 = color[1].integer;
                }
                else
                {
                    c1 = color[0].integer;
                    c2 = colorFlip(COLOR_CAST(cp[1].rgb_color)).integer;
                }

                baseedit_SetCurRoomTint(g_iDecorIdx, c1, c2);
            }
            ty += 65*sc;
        }

         if( D_MOUSEHIT == drawStdButton( x + wd/2, ty-sb.offset+12*sc, z, MIN(200*sc,wd-2*R10*sc), 20*sc, color, "ResetRoomString", 1.f, !g_refCurRoom.p ))
        {
            baseedit_SetCurRoomTint(-1, CreateColorRGB( 128, 128, 128 ).integer, CreateColorRGB( 128, 128, 128 ).integer );
        }
        ty += 25*sc;

        if( D_MOUSEHIT == drawStdButton( x + wd/2, ty-sb.offset+12*sc, z, MIN(200*sc,wd-2*R10*sc), 20*sc, color, "ApplyTintToAll", 1.f, !g_refCurRoom.p ))
        {
            baseedit_SetCurRoomTint(-1,colorFlip(COLOR_CAST(cp[0].rgb_color)).integer, colorFlip(COLOR_CAST(cp[1].rgb_color)).integer );
        }
        ty += 25*sc;

        if( D_MOUSEHIT == drawStdButton( x + wd/2, ty-sb.offset+12*sc, z, MIN(200*sc,wd-2*R10*sc), 20*sc, color, "ApplyRoomTintToBase", 1.f, !g_refCurRoom.p ))
        {
            baseClientSendApplyRoomTintToBase(g_refCurRoom.p);
        }
        ty += 25*sc;

         clipperPop();

           doScrollBar( &sb, view_ht-2*R10*sc, ty-y, x + wd, top_ht+R10*sc, z, 0, &box);
     }
    else if( room )
    {
        view_ht = ht - (ty-y)-PIX3*sc;
        uiBoxDefine( &box, x+PIX3*sc, ty, wd - 2*PIX3*sc, view_ht );

        clipperPush(&box);
        func_ht = basepropsRoom( room, x, ty, z, wd, ht - (ty-y), sc, color, bcolor );
        clipperPop();

        if( window_getMode(WDW_BASE_PROPS) == WINDOW_DISPLAYING && ht != ty + func_ht - y )
            window_setDims( WDW_BASE_PROPS, -1, -1, -1, ty + func_ht - y );
    }
    else if( item ) // do tabs and stuff if item is selected
    {
        static int had_aux = -1; // only show the aux tab if appropriate
        static int had_permissions = -1; // only show the permissions tab if appropriate
        int need_aux = g_refCurDetail.p && g_refCurDetail.p->info->ppchAuxAllowed;
        int need_permissions = g_refCurDetail.p && g_refCurDetail.p->info->bIsStorage;
        int old_permissions;
        // Tabs
        if (!itemTabs)
        {
            itemTabs = uiTabControlCreate(TabType_Undraggable, 0, 0, 0, 0, 0);
            uiTabControlAdd( itemTabs, "ObjectString", basepropsItem );
            uiTabControlAdd( itemTabs, "InfoString", basepropsInfo );
        }
        if (had_permissions != need_permissions)
        {
            if (need_permissions)
            {
                uiTabControlAdd(itemTabs, "PermissionsStr", basepropsPermissions);
                srRequestPermissionsSetBit();
            }
            else
            {
                uiTabControlRemoveByName(itemTabs, "PermissionsStr");
            }
            had_permissions = need_permissions;
        }
        if(had_aux != need_aux)
        {            
            if (need_aux && need_aux != had_aux)
                uiTabControlAdd( itemTabs, "AuxString", basepropsAux );
            else if (!need_aux && need_aux != had_aux)
                uiTabControlRemoveByName(itemTabs,"AuxString");
            had_aux = need_aux;
        }
        if (need_permissions)
        {
            old_permissions = g_refCurDetail.p->permissions;
        }

        func = drawTabControl(itemTabs, x+R10*sc, ty - PIX3*sc, z, wd - 2*R10*sc, TAB_HEIGHT, sc, color, color, TabDirection_Horizontal );
        ty += (TAB_HEIGHT+PIX3)*sc;
        view_ht = ht - (ty-y)-PIX3*sc;

        uiBoxDefine( &box, x+PIX3*sc, ty, wd - 2*PIX3*sc, view_ht );

        clipperPush(&box);
        s_curDetail = g_refCurDetail.p;
        func_ht = func( x, ty - sb.offset, z, wd, view_ht, sc, color, bcolor );
        clipperPop();

        // Need to check g_refCurDetail.p here since the func(...) call just above could have set it null if the item was sold
        if (need_permissions && g_refCurDetail.p != NULL && old_permissions != g_refCurDetail.p->permissions)
        {
            baseedit_SetDetailPermissions(g_refCurRoom.p, g_refCurDetail.p);
        }

        if (func_ht == 0)
        {
            func_ht = DEFAULTHEIGHT*sc - ty + y;
        }

        if( window_getMode(WDW_BASE_PROPS) == WINDOW_DISPLAYING && ht != ty + func_ht -y)
            window_setDims( WDW_BASE_PROPS, -1, -1, -1, ty + func_ht - y);
    }
    else if (baseedit_Mode() == kBaseEdit_Architect)
    {
        basepropsBlock( x, ty, z, wd, ht - (ty-y), sc, color, bcolor );
        if( window_getMode(WDW_BASE_PROPS) == WINDOW_DISPLAYING && ht != DEFAULTHEIGHT*sc )
            window_setDims( WDW_BASE_PROPS, -1, -1, -1, DEFAULTHEIGHT*sc);
    }
    else if ( baseedit_Mode() == kBaseEdit_Plot )
    {
        const BasePlot * plot = getBaseInventorySelectedItem();

        if(!plot)
            plot = g_base.plot;

        view_ht = ht - (ty-y)-PIX3*sc;
        uiBoxDefine( &box, x+PIX3*sc, ty, wd - 2*PIX3*sc, view_ht );

        clipperPush(&box);
        func_ht = basepropsPlot( plot, x, ty - sb.offset, z, wd, ht - (ty-y), sc, color, bcolor );
        clipperPop();

        if( window_getMode(WDW_BASE_PROPS) == WINDOW_DISPLAYING && ht != ty + func_ht - y )
            window_setDims( WDW_BASE_PROPS, -1, -1, -1, ty + func_ht - y );
    }

    return 0;
}

// Permissions window, a very scaled back version of the properties window

void openPermissions(RoomDetail *detail)
{
    s_detailID = detail->id;
    window_openClose(WDW_BASE_STORAGE_PERMISSIONS);
}

int basepermissionsWindow()
{
    float x, y, ty, z, wd, ht, sc, view_ht, func_ht;
    int color, bcolor;
    int old_permissions;
    static RoomDetail *lastDetail = NULL;
    UIBox box;
    char title[256], desc[512];
    RoomDetail *detail;

    // Do everything common windows are supposed to do.
    if (!window_getDims(WDW_BASE_STORAGE_PERMISSIONS, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
    {
        return 0;
    }

    if (window_getMode(WDW_BASE_STORAGE) != WINDOW_DISPLAYING)
    {
        window_setMode(WDW_BASE_STORAGE_PERMISSIONS, WINDOW_SHRINKING);
    }

    detail = baseGetDetail(&g_base, s_detailID, NULL); 

    wd = 250;
    ht = 250;

    if (window_getMode(WDW_BASE_STORAGE_PERMISSIONS) != WINDOW_DISPLAYING)
    {
        return 0;
    }

    if (smf_title == NULL)
    {
        smf_title = smfBlock_Create();
    }

    if (smf_desc == NULL)
    {
        smf_desc = smfBlock_Create();
    }

    strncpyt(title, textStd(detail->info->pchDisplayName), ARRAY_SIZE(title));
    strncpyt(desc, detailGetDescription(detail->info, 0), ARRAY_SIZE(desc));

    if(detail != lastDetail)
    {
        smf_ParseAndFormat(smf_title, title, 0, 0, 0, wd - R10*sc*2, 1000, false, true, false, &gTextTitleAttr, 0 );
        smf_ParseAndFormat(smf_desc, desc, 0, 0, 0, wd - R10*sc*2, 1000, false, true, true, &gTextAttr_White12, 0 );

        smf_SetFlags(smf_title, 0, 0, 0, 0, 0, 0, 0, 0, SMAlignment_Center, 0, 0, 0);
        smf_ParseAndFormat(smf_title, title, 0, 0, 0, wd - R10*sc*2, 1000, false, false, true, &gTextTitleAttr, 0 );
        lastDetail = detail;

        srRequestPermissionsSetBit();
    } 

    gTextTitleAttr.piScale = (int*)((int)(SMF_FONT_SCALE*1.2f*sc));
    gTextAttr_White12.piScale = (int*)((int)(SMF_FONT_SCALE*sc));

    // Name
    ty = y + smf_ParseAndDisplay( smf_title, title, x + R10*sc, y + R10*sc, z, wd - R10*sc*2, 0, false, false, 
                                    &gTextTitleAttr, NULL, 0, true) + 15*sc;

    // Short Desc
    ty += smf_ParseAndDisplay( smf_desc, desc, x + R10*sc, ty, z, wd - R10*sc*2, 0, false, false, 
                                    &gTextAttr_White12, NULL, 0, true) + 5*sc;

    drawJoinedFrame2Panel( PIX3, R10, x, y, z, sc, wd, (ty-y), ht - (ty-y), color, bcolor, NULL, NULL );

    old_permissions = detail->permissions;

    view_ht = ht - (ty-y)-PIX3*sc;

    uiBoxDefine( &box, x+PIX3*sc, ty, wd - 2*PIX3*sc, view_ht );

    clipperPush(&box);
    s_getDetailFromID = 1;
    func_ht = basepropsPermissions( x, ty, z, wd, view_ht, sc, color, bcolor );
    s_getDetailFromID = 0;
    clipperPop();

    if (old_permissions != detail->permissions)
    {
        BaseRoom *room = baseGetRoom(&g_base, detail->parentRoom);
        if (room)
        {
            baseedit_SetDetailPermissions(room, detail);
        }
    }

    if( window_getMode(WDW_BASE_STORAGE_PERMISSIONS) == WINDOW_DISPLAYING && ht != ty + func_ht - y)
    {
        window_setDims(WDW_BASE_STORAGE_PERMISSIONS, -1, -1, -1, ty + func_ht - y);
    }
    return 0;
}

/* End of File */
