/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/utils/utils.h>
#include "UI/uiCursor.h"
#include "uiSalvage.h"
#include "uiGrowBig.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiWindows.h"
#include "UI/uiTray.h"
#include "UI/uiCursor.h"
#include "UI/uiToolTip.h"
#include "UI/uiGame.h"
#include "UI/uiNet.h"
#include "UI/uiContextMenu.h"
#include "UI/uiWindows_init.h"
#include "UI/uiScrollBar.h"
#include "UI/uiInput.h"
#include "bases/baseedit.h"
#include "bases/basestorage.h"
#include "UI/sprite/sprite_text.h"
#include "UI/uiChat.h"
#include "uiCombineSpec.h"
#include "UI/uiEnhancement.h"
#include "UI/uiInspiration.h"
#include "uiBaseLog.h"
#include "uiBaseStorage.h"
#include "UI/uibaseprops.h"

#include "entity/character_base.h"
#include "gameComm/wdwbase.h"
#include "win/win_init.h"
#include "entity/powers.h"
#include "player/player.h"
#include "entity/entity.h"
#include "entity/EntPlayer.h"
#include "entity/entVarUpdate.h"
#include "clientcomm/clientcomm.h"
#include "cmdparse/cmdcommon.h"
#include "entity/entity_power_client.h"
#include "win/input.h"
#include "cmdparse/cmdcommon.h"
#include "cmdparse/cmdgame.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/mathutil.h>
#include "entity/character_inventory.h"
#include "entity/character_workshop.h"
#include <utilitieslib/utils/file.h>
#include "bases/DetailRecipe.h"

#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/components/Earray.h>
#include "formatter/smf_main.h"
#include "graphics/groupThumbnail.h"
#include "bases/bases.h"
#include "bases/basedata.h"
#include "UI/uiTabControl.h"
#include "UI/uiComboBox.h"
#include <utilitieslib/components/StashTable.h>
#include "graphics/ttFontUtil.h"
#include "formatter/smf_main.h"
#include "entity/Supergroup.h"
#include <utilitieslib/language/MessageStoreUtil.h>

#define ITEM_FOOTER    30
#define ITEM_SHORT_FOOTER 12
        
#define ICON_WD 48
#define COL_WD 96*4
#define ROW_HT 78
#define SPACER_WD 10
#define BOTTOM_HT   24

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
    /* piFace        */  (int *)&game_9,
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

static TextAttribs gTextErr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
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

// from uiBaseProps.c
extern TextAttribs gTextTitleAttr;

typedef struct StoredSalvageSMF
{
    const char *nameItem;
    SMFBlock *smfName;
    SMFBlock *smfShortInfo;
    SMFBlock *smfInfo;
    F32 sc;
    int height;
} StoredSalvageSMF;

typedef struct BaseStorageState
{
    StoredSalvage **items;
    uiGrowBig growbig;
    int idDetail;
    StoredSalvageSMF **itemSMFs;
} BaseStorageState;

static char * err_msg;
static float err_timer;
static smf_err;
static SMFBlock smf_title;
static SMFBlock smf_desc;
static int forceReformat = false;

void baseStorage_ErrMessage( char * msg, float time )
{
    if( time > 0 && err_timer < 0 ) // error messages take priority
        return;

     if( err_msg && stricmp(err_msg, msg)!=0 )
    {
        free(err_msg);
        if( time < 0 ) // error messages are events
            addSystemChatMsg(msg, INFO_USER_ERROR, 0);
        err_msg=strdup(msg);
    }
    err_timer = time;

}

static BaseStorageState s_state = {0};

SalvageInventoryItem * getStoredSalvageFromName( const char * name )
{
    int i;
    RoomDetail * detail = baseGetDetail(&g_base, s_state.idDetail, NULL);

    if( !windowUp(WDW_BASE_STORAGE) || !detail || !eaSize(&detail->storedSalvage) )
        return 0;
 
    for( i = 0; i < eaSize(&detail->storedSalvage); i++ )
    {
        if(stricmp(name, detail->storedSalvage[i]->salvage->pchName) == 0 )
            return (SalvageInventoryItem*)detail->storedSalvage[i];
    }

    return 0;
}

MP_DEFINE(StoredSalvageSMF);
static StoredSalvageSMF* storedsalvagesmf_Create( const char *nameItem, F32 sc )
{
    StoredSalvageSMF *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(StoredSalvageSMF, 64);
    res = MP_ALLOC( StoredSalvageSMF );
    if( verify( res ))
    {
        res->nameItem = nameItem;
        res->smfName = smfBlock_Create();
        res->smfName->pBlock->pos.alignHoriz = SMAlignment_Left;
        res->smfShortInfo = smfBlock_Create();
        res->smfShortInfo->pBlock->pos.alignHoriz = SMAlignment_Left;
        res->smfInfo = smfBlock_Create();
        res->smfInfo->pBlock->pos.alignHoriz = SMAlignment_Left;
        res->sc = sc;
        res->height = -1;
        
    }
    return res;
}

static void storedsalvagesmf_Destroy( StoredSalvageSMF *hItem )
{
    if(hItem)
    {
        smfBlock_Destroy(hItem->smfName);
        smfBlock_Destroy(hItem->smfShortInfo);
        smfBlock_Destroy(hItem->smfInfo);
        MP_FREE(StoredSalvageSMF, hItem);
    }
}


static StoredSalvageSMF *findOrAddSMF(const char *nameItem, F32 sc)
{
    int i;
    StoredSalvageSMF *smf = NULL;
    
    for( i = eaSize( &s_state.itemSMFs) - 1; i >= 0; --i)
    {
        if( 0 == stricmp(nameItem, s_state.itemSMFs[i]->nameItem) )
            break;
    }
    if( i < 0 )
    {
        i = eaPush(&s_state.itemSMFs, storedsalvagesmf_Create(nameItem, sc));
    }
    smf = s_state.itemSMFs[i];
    
    if( smf->sc != sc )
    {
        smf->sc = sc;
    }
    return smf;
}

#define BOX_DY(box) ((box).hy-(box).ly)
#define BOX_DX(box) ((box).hx-(box).lx)
void prnt_dims(CBox *bxRes, float x, float y, float z, float xsc, float ysc, char  * str, int bDisplay)
{
    bxRes->lx = x;
    bxRes->ly = y;
    str_dims( font_get(), xsc, ysc, false, bxRes, str );
    if (bDisplay)
        prnt(x,y+BOX_DY(*bxRes),z,xsc,ysc,str);
}

static F32 storedenhancementsmf_Display( StoredSalvageSMF *smf, const char *name, const char *shortInfo, const char *info, const char *entFrom, F32 x, F32 y, F32 z, F32 dxMax, F32 sc, int rarity, int bDisplay )
{
    F32 resDy = 0;
    char *txtShortInfo = textStd(shortInfo);
    char *txtInfo = info;
    char *txtName = textStd(name);
    bool reformat = false;

    char buf[1024];
    char *color = "white";

    if (rarity >= 0)
    {
        switch (rarity)
        {
            case kSalvageRarity_Ubiquitous:
                color = "Gray";
                break;
            case kSalvageRarity_Common:
                color = "White";
                break;
            case kSalvageRarity_Uncommon:
                color = "#FFFF00";
                break;
            case kSalvageRarity_Rare:
                color = "#FF5500";
                break;
            case kSalvageRarity_VeryRare:
            case kSalvageRarity_Unique:
            case kSalvageRarity_Count:
                color = "Purple";
                break;
        }
        sprintf_s(buf, 1024, "<color %s>%s</color>", color, textStd(name));
        txtName = buf;
    }

    if (smf->smfName->iLastHeight == 0 || forceReformat) 
    { 
         gTextAttr_White12.piScale = (int*)((int)(SMF_FONT_SCALE*(sc*1.2f)));
        smf->smfName->iLastHeight =         smf_ParseAndFormat(smf->smfName, txtName, 0, 0, 0, dxMax, smf->smfName->iLastHeight, false, false, true, &gTextAttr_White12, 0 );
        gTextAttr.piScale = (int*)((int)(SMF_FONT_SCALE*(sc*1.0f)));
        smf->smfShortInfo->iLastHeight = smf_ParseAndFormat(smf->smfShortInfo, txtShortInfo, 0, 0, 0, dxMax, smf->smfShortInfo->iLastHeight, false, false, true, &gTextAttr, 0 );
        gTextAttr.piScale = (int*)((int)(SMF_FONT_SCALE*(sc*1.0f)));
        smf->smfInfo->iLastHeight =         smf_ParseAndFormat(smf->smfInfo, txtInfo, 0, 0, 0, dxMax, smf->smfInfo->iLastHeight, false, false, true, &gTextAttr, 0 );
        reformat = true;

        if(entFrom && *entFrom)
        {
            CBox bxName = {0};
            font( &game_9 );
            font_color(0x00deffff, 0x00deffff);
            prnt_dims( &bxName, x+PIX3*sc, y+resDy+(PIX3*sc), z, sc, sc, textStd("BaseStorageAddedBy", entFrom), false);
            resDy += BOX_DY(bxName) + PIX3*sc;
        }

        if (!bDisplay)
        {
            smf->height = (int) resDy + smf->smfName->iLastHeight + smf->smfShortInfo->iLastHeight + smf->smfInfo->iLastHeight;
            return smf->height + PIX3*sc;
        }
        resDy = 0.0f;
    }

    gTextAttr_White12.piScale = (int*)((int)(SMF_FONT_SCALE*(sc*1.2f)));
    smf_ParseAndDisplay( smf->smfName, txtName, x, y, z, dxMax, smf->smfName->iLastHeight, false, reformat, 
                            &gTextAttr_White12, NULL, 0, true);
    resDy += smf->smfName->iLastHeight;

    gTextAttr.piScale = (int*)((int)(SMF_FONT_SCALE*(sc*1.0f)));
      smf_ParseAndDisplay( smf->smfShortInfo, txtShortInfo, x, y+resDy+PIX3*sc, z, dxMax, smf->smfShortInfo->iLastHeight, false, reformat, 
                            &gTextAttr, NULL, 0, true);
     resDy += smf->smfShortInfo->iLastHeight;

    if(entFrom && *entFrom)
    {
        CBox bxName = {0};
        font( &game_9 );
        font_color(0x00deffff, 0x00deffff);
          prnt_dims( &bxName, x+PIX3*sc, y+resDy+(PIX3*sc), z, sc, sc, textStd("BaseStorageAddedBy", entFrom), true);
        resDy += BOX_DY(bxName) + PIX3*sc;
    }

    gTextAttr.piScale = (int*)((int)(SMF_FONT_SCALE*(sc*1.0f)));
      smf_ParseAndDisplay( smf->smfInfo, txtInfo, x, y+resDy+PIX3*sc, z, dxMax, smf->smfInfo->iLastHeight, false, reformat, 
                            &gTextAttr, NULL, 0, true);
     resDy += smf->smfInfo->iLastHeight;

    smf->height = (int) resDy +PIX3*sc;
    return smf->height;
}

static F32 storedsalvagesmf_Display( StoredSalvageSMF *smf, const char *name, const char *shortInfo, const char *info, const char *entFrom, F32 x, F32 y, F32 z, F32 dxMax, F32 sc, int rarity, int bDisplay )
{
    return storedenhancementsmf_Display(smf, name, shortInfo, textStd(info), entFrom, x, y, z, dxMax, sc, rarity, bDisplay);
}


void basestoragestate_Init(BaseStorageState *state)
{
    ZeroStruct( state );
    uigrowbig_Init( &state->growbig, 1.1, 10 );
}


static void storedSalvageDrag(SalvageInventoryItem *sal, AtlasTex *icon, float sc)
{
    if(verify( sal && icon ))
    {
        TrayObj to = {0};
        to.type = kTrayItemType_StoredSalvage;
        to.idDetail = s_state.idDetail;
        to.nameSalvage = sal->salvage->pchName;
        to.scale = sc;
        trayobj_startDragging(&to,icon,NULL);
    }
}

static void storedInspDrag(StoredInspiration *insp, F32 sc)
{
    if(verify( insp && insp->ppow ))
    {
        TrayObj to = {0};
        to.type = kTrayItemType_StoredInspiration;
        to.idDetail = s_state.idDetail;
        to.ppow = insp->ppow;
        to.scale = sc;
        trayobj_startDragging(&to,atlasLoadTexture(insp->ppow->pchIconName),NULL);
    }
}

static void storedEnhancementDrag(StoredEnhancement *itm, F32 sc, AtlasTex * icon)
{
    if(verify( itm && itm->enhancement && itm->enhancement[0] ))
    {
        TrayObj to = {0};
        to.type = kTrayItemType_StoredEnhancement;
        to.idDetail = s_state.idDetail;
        to.enhancement = itm->enhancement[0];
        to.scale = sc;
        trayobj_startDragging(&to,atlasLoadTexture(itm->enhancement[0]->ppowBase->pchIconName),icon);
    }
}

#define ICON_COL  80


static bool s_DisplayStoredSalvage(BaseStorageState *state, StoredSalvage *sal, float x, float y, float z, float sc, float wd, float *dy, int bDisplay, U32 permissions)
{
    float overScale, xsc, ht;
    int zmod;
    AtlasTex *icon;
    CBox box;
    Entity * e = playerPtr();

    StoredSalvageSMF *smf = sal && sal->salvage ? findOrAddSMF( sal->salvage->pchName, sc ) : NULL;
    const SalvageItem *itm = sal->salvage;

    if( !state || !sal || !sal->salvage || !smf )
        return 0;


    overScale = uigrowbig_GetScale( &state->growbig, sal, &zmod );
    icon = atlasLoadTexture( sal->salvage->ui.pchIcon );
    x += PIX3*sc;
    xsc = ((F32)ICON_WD)/icon->width;

    // display the salvage. get the dimensions
    if(verify(smf))
    {
        F32 dxMax = wd - (ICON_COL*sc+2*PIX3*sc); 
        ht = storedsalvagesmf_Display( smf, itm->ui.pchDisplayName, itm->ui.pchDisplayShortHelp, itm->ui.pchDisplayHelp, sal->nameEntFrom, x+(ICON_COL)*sc, y+(PIX3)*sc, z+1, dxMax, sc, itm->rarity, bDisplay )+R10*sc;
        *dy += ht;

        if (!bDisplay)
            return false;
    }

    // --------------------
    // print the amount
    font( &game_12 );
    font_color(0x00deffff, 0x00deffff);
    cprnt( x+R10, y + (R10+2*PIX3)*sc, z+2, sc, sc, "%d", sal->amount );

    display_sprite( icon, x + (ICON_COL-icon->width*xsc*overScale)*sc/2, y + ht/2 -icon->height*xsc*overScale*sc/2, z+1, sc*xsc*overScale, sc*xsc*overScale, CLR_WHITE );
     BuildCBox( &box, x, y, wd-(PIX3*4)*sc, ht );

     if(!cursor.dragging && mouseCollision(&box) && sgroup_hasStoragePermission(e, permissions, -1))
    {
        drawFlatFrameBox( PIX3, R10, &box, z, CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND );
        baseStorage_ErrMessage( textStd("ClickToTakeStack",itm->ui.pchDisplayName), 3.f );
    }

    if (mouseLeftDrag( &box ))
        storedSalvageDrag((SalvageInventoryItem*)sal, icon, sc);

     if(mouseClickHit(&box,MS_LEFT))
        salvageAddToStorageContainer((SalvageInventoryItem*)sal,-1);

    return mouseCollision(&box);
}

static float flash_timer = 0.f;

static void storageWindowFlash()
{
    flash_timer = .00001f;
}

// the goal icon width
//#define ICON_WD 96

static bool s_DisplayStoredInspiration(BaseStorageState *state, StoredInspiration *insp, float x, float y, float z, float sc, float wd, float *dy, int bDisplay, U32 permissions)
{
    AtlasTex *icon;
    CBox box;
    const BasePower *ppow = insp?insp->ppow:NULL;
    int zmod;
    float overScale, xsc, ht;
    StoredSalvageSMF *smf;
    char *pathInsp = (ppow) ? basepower_ToPath(ppow) : NULL;
    Entity * e = playerPtr();

    if( !state || !insp || !insp->ppow || !pathInsp )
        return false;

    smf = findOrAddSMF( pathInsp , sc );


    overScale = uigrowbig_GetScale( &state->growbig, insp, &zmod );
    icon = atlasLoadTexture( ppow->pchIconName );
      x += PIX3*sc;
    xsc = ((F32)ICON_WD)/icon->width;

    // to the right, put the description, etc.
    if(verify(smf))
    {
           F32 dxMax = wd - (ICON_COL*sc + 4*PIX3*sc);
        F32 height = storedsalvagesmf_Display( smf, ppow->pchDisplayName, ppow->pchDisplayShortHelp, ppow->pchDisplayHelp, insp->nameEntFrom, x+(ICON_COL)*sc, y+(PIX3)*sc, z+1, dxMax, sc, -1, bDisplay )+(R10*sc);
         ht = MAX(ROW_HT*sc, height);
          *dy += ht;

        if (!bDisplay)
            return false;
    }

    // --------------------
    // print the amount
    font( &game_12 );
       font_color(0x00deffff, 0x00deffff);
       cprnt( x+R10, y + (R10+2*PIX3)*sc, z+2, sc, sc, "%d", insp->amount );
 
       display_sprite( icon, x + (ICON_COL-icon->width*xsc*overScale)*sc/2, y + ht/2 -icon->height*xsc*overScale*sc/2, z+1, sc*xsc*overScale, sc*xsc*overScale, CLR_WHITE );
       BuildCBox( &box, x, y, wd-(PIX3*4)*sc, ht );

     if(!cursor.dragging && mouseCollision(&box) && sgroup_hasStoragePermission(e, permissions, -1))
    {
        baseStorage_ErrMessage( textStd("ClickToTake",ppow->pchDisplayName), 3.f );
        drawFlatFrameBox( PIX3, R10, &box, z, CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND );
    }

    if( mouseClickHit( &box, MS_RIGHT ) )
    {
        static TrayObj to_tmp;
        to_tmp.ppow = ppow;
        to_tmp.idDetail = s_state.idDetail;
        to_tmp.type = kTrayItemType_StoredInspiration;
        trayInfo_GenericContext( &box, &to_tmp );
    }

    if (mouseLeftDrag( &box ))
        storedInspDrag(insp, sc);
    if(mouseClickHit(&box,MS_LEFT))
    {
        window_setMode( WDW_INSPIRATION, WINDOW_GROWING );
        inspirationAddToStorageContainer(ppow, s_state.idDetail,-1,-1,-1);
    }

    return mouseCollision(&box);
}

#define ENH_WD 58.f
static bool s_DisplayStoredEnhancement(BaseStorageState *state, StoredEnhancement *itm, float x, float y, float z, float sc, float wd, float *dy, int bDisplay, U32 permissions)
{
    AtlasTex * icon = NULL;
    CBox box;
    const BasePower *ppow = itm&&itm->enhancement&&itm->enhancement[0]?itm->enhancement[0]->ppowBase:NULL;
    Boost *enh = itm&&itm->enhancement ? itm->enhancement[0] : NULL;
    int zmod;
    F32 overScale, xsc = 1.f, ht;
    StoredSalvageSMF *smf;
    char *pathBasePower = (ppow) ? basepower_ToPath(ppow) : NULL;
    Entity * e = playerPtr();
    StuffBuff sEnhHelp = {0};

    if( !state || !itm || !itm->enhancement || !pathBasePower || !enh )
        return false;

    smf = findOrAddSMF( pathBasePower , sc );


    // formatting help text
    initStuffBuff(&sEnhHelp, 256);
    uiEnhancementAddHelpText(enh, ppow, enh->iLevel, &sEnhHelp);


     overScale = uigrowbig_GetScale( &state->growbig, itm, &zmod );
    icon = atlasLoadTexture( ppow->pchIconName );
    x += PIX3*sc;
    xsc = ((F32)ICON_WD)/ENH_WD;

    // to the right, put the description, etc.
    if(verify(smf))
    {
           F32 dxMax = wd - (ICON_COL*sc+2*PIX3*sc); 
        ht = MAX(ROW_HT*sc,storedenhancementsmf_Display( smf, ppow->pchDisplayName, ppow->pchDisplayShortHelp, sEnhHelp.buff, itm->nameEntFrom, x+(ICON_COL)*sc, y+(PIX3)*sc, z+1, dxMax, sc, -1, bDisplay )+R10*sc);
          *dy += ht;

        if (!bDisplay)
            return false;
    }

    icon = drawEnhancementOriginLevel(ppow,enh->iLevel+1,enh->iNumCombines,false,x+(ICON_COL*sc)/2,y+ht/2,z,sc*xsc*overScale,sc*xsc*overScale, CLR_WHITE, false);
    // --------------------
    // print the amount
    font( &game_12 );
    font_color(0x00deffff, 0x00deffff);
    cprnt( x+R10, y + (R10+2*PIX3)*sc, z+2, sc, sc, "%d", itm->amount );

    display_sprite( icon, x + (ICON_COL-icon->width*xsc*overScale)*sc/2, y + ht/2 -icon->height*xsc*overScale*sc/2, z+1, sc*xsc*overScale, sc*xsc*overScale, CLR_WHITE );
    BuildCBox( &box, x, y, wd-(PIX3*4)*sc, ht );

     if(!cursor.dragging && mouseCollision(&box) && sgroup_hasStoragePermission(e, permissions, -1))
    {
        baseStorage_ErrMessage( textStd("ClickToTake",ppow->pchDisplayName), 3.f );
        drawFlatFrameBox( PIX3, R10, &box, z, CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND );
    }

    // set the context menu
    if( mouseClickHit( &box, MS_RIGHT ) )
    {
        static TrayObj to_tmp;
        to_tmp.enhancement = enh;
        to_tmp.idDetail = s_state.idDetail;
        to_tmp.type = kTrayItemType_StoredEnhancement;
        trayInfo_GenericContext(&box, &to_tmp);
    }

    if (mouseLeftDrag( &box )
        && verify(!cursor.dragging))
    {
        storedEnhancementDrag(itm, sc, icon);
    }
    if(mouseClickHit(&box,MS_LEFT))
    {
        window_setMode( WDW_ENHANCEMENT, WINDOW_GROWING );
        enhancementAddToStorageContainer(enh,s_state.idDetail,-1,-1);
    }

    freeStuffBuff(&sEnhHelp);

    return mouseCollision(&box);
}

void salvageAddToStorageContainer(const SalvageInventoryItem *sal, int count)
{
    Entity *e = playerPtr();
    RoomDetail * detail = baseGetDetail(&g_base, s_state.idDetail, NULL);
    int adding = count > 0;

    if( !SAFE_MEMBER(sal,salvage) || !detail || !SAFE_MEMBER(e,pchar) || !e->supergroup || e->supergroup_id != g_base.supergroup_id)
        return;

    // cap count at stack size
    if(count > ((int)sal->amount))
        count = sal->amount;
    if(count < -((int)sal->amount))
        count = -((int)sal->amount);

    // lower count (thus breaking stack) in attempt to fit
    while( count && 
            (count >= 0 ? !entity_CanAdjStoredSalvage( e, detail, (SalvageItem*)sal->salvage, count)
            : !character_CanAdjustSalvage(e->pchar, (SalvageItem*)sal->salvage, -count)))
    {
        if( count > 0 )
            count--;
        else
            count++;
    }

    if( count )
        basestorage_SendSalvageAdjust( detail->id, sal->salvage->pchName, count );
    else
    {
        if( adding ) // adding to storage
        {
            if( !sgroup_hasStoragePermission(e, detail->permissions, 1))
            {
                storageWindowFlash();
                baseStorage_ErrMessage(textStd("BaseStorageCantAdd"), -6);
                return;
            }

            if( roomdetail_MaxCount(detail) == roomdetail_CurrentCount(detail) )
                baseStorage_ErrMessage(textStd("BaseStorageFull", detail->info->pchDisplayName), -6);
            else
                baseStorage_ErrMessage(textStd("BaseStorageFail", sal->salvage->ui.pchDisplayName, detail->info->pchDisplayName), -6);

            storageWindowFlash();
        }
        else // taking from storage
        {
            if(!sgroup_hasStoragePermission(e, detail->permissions, -1))
            {
                storageWindowFlash();
                baseStorage_ErrMessage(textStd("BaseStorageCantTake"), -6);
                return;
            }

            if( character_SalvageIsFull( e->pchar, sal->salvage ) )
                baseStorage_ErrMessage(textStd("InventoryAddFromStorageFull", sal->salvage->ui.pchDisplayName), -6);
            else
                baseStorage_ErrMessage(textStd("InventoryAddFromStorageFailed", sal->salvage->ui.pchDisplayName), -6);

            salvageWindowFlash();
        }
    }
}

void inspirationAddToStorageContainer(const BasePower *ppow, int idDetail, int col, int row, int amount)
{
    Entity *e = playerPtr();
    RoomDetail * detail = baseGetDetail(&g_base, idDetail, NULL);
    char *pathInsp = ppow ? basepower_ToPath(ppow) : NULL;
    int adding = amount > 0;

    if( !ppow || !detail || !e->supergroup || e->supergroup_id != g_base.supergroup_id)
        return;

    if( entity_CanAdjStoredInspiration( e, detail, ppow, pathInsp, amount ))
        basestorage_SendInspirationAdjust( idDetail, col, row, pathInsp, amount);
    else
    {
        if( adding ) // adding to storage
        {
            if( !sgroup_hasStoragePermission(e, detail->permissions, 1))
            {
                storageWindowFlash();
                baseStorage_ErrMessage(textStd("BaseStorageCantAdd"), -6);
                return;
            }

            if( roomdetail_MaxCount(detail) == roomdetail_CurrentCount(detail) )
                baseStorage_ErrMessage(textStd("BaseStorageFull", detail->info->pchDisplayName), -6);
            else
                baseStorage_ErrMessage(textStd("BaseStorageFail", ppow->pchDisplayName, detail->info->pchDisplayName), -6);

            storageWindowFlash();
        }
        else // taking out of storage
        {
            if(!sgroup_hasStoragePermission(e, detail->permissions, -1))
            {
                storageWindowFlash();
                baseStorage_ErrMessage(textStd("BaseStorageCantTake"), -6);
                return;
            }

            if( character_InspirationInventoryFull(e->pchar) )
                baseStorage_ErrMessage(textStd("InspAddFromStorageFull", ppow->pchDisplayName), -6);
            else
                baseStorage_ErrMessage(textStd("InventoryAddFromStorageFailed", ppow->pchDisplayName), -6);
        
            inspirationWindowFlash();
        }
    }

}

void enhancementAddToStorageContainer(const Boost *enh, int idDetail, int iInv, int amount)
{
    Entity *e = playerPtr();
    RoomDetail * detail = baseGetDetail(&g_base, idDetail, NULL);
    char *pathBasePower = basepower_ToPath(enh->ppowBase);
    int adding = amount > 0;
    char * error_msg;

    strdup_alloca(pathBasePower,pathBasePower);

    if( !enh || !detail || !e->supergroup || e->supergroup_id != g_base.supergroup_id)
        return;
        
    if(entity_CanAdjStoredEnhancement( e, detail, enh->ppowBase, pathBasePower, enh->iLevel, enh->iNumCombines, amount ))
    {
        basestorage_SendEnhancementAdjust( idDetail, pathBasePower, enh->iLevel, enh->iNumCombines, iInv, amount);

    }
    else
    {
        if( adding ) // adding to storage
        {
            if( !sgroup_hasStoragePermission(e, detail->permissions, 1))
            {
                storageWindowFlash();
                baseStorage_ErrMessage( textStd("BaseStorageCantAdd"), -6.f );
                return;
            } 


            if( roomdetail_MaxCount(detail) == roomdetail_CurrentCount(detail) )
                error_msg = textStd("BaseStorageFull", detail->info->pchDisplayName);
            else if (enh->iNumCombines > 0)
                error_msg = textStd("BaseStorageCantAddCombined", detail->info->pchDisplayName);
            else if (!detailrecipedict_IsBoostTradeable(enh->ppowBase, enh->iLevel, e->auth_name, ""))
                error_msg = textStd("BaseStorageCantAddNonTrade", detail->info->pchDisplayName);
            else
                error_msg = textStd("BaseStorageFail", enh->ppowBase->pchDisplayName, detail->info->pchDisplayName);

            baseStorage_ErrMessage( error_msg, -6.f );
            storageWindowFlash();
        }
        else // taking out of storage
        {
            if(!sgroup_hasStoragePermission(e, detail->permissions, -1))
            {
                storageWindowFlash();
                baseStorage_ErrMessage( textStd("BaseStorageCantTake"), -6.f );
                return;
            }

            if( character_BoostInventoryFull(e->pchar) )
                error_msg = textStd("EnhAddFromStorageFull", enh->ppowBase->pchDisplayName);
            else
                error_msg = textStd("InventoryAddFromStorageFailed", enh->ppowBase->pchDisplayName);

            baseStorage_ErrMessage( error_msg, -6.f );
            enhancementWindowFlash();
        }
    }
}

RoomDetail *getStorageWindowDetail(void)
{
    return baseGetDetail(&g_base, s_state.idDetail, NULL);
}

static float showLogCenterX, showLogCenterY;
static float showPermCenterX, showPermCenterY;

int getShowLogCenter( int wdw, float * xp, float * yp )
{
    if( wdw == WDW_BASE_LOG && windowUp(WDW_BASE_STORAGE) )
    {
        *xp = showLogCenterX;
        *yp = showLogCenterY;
        return true;
    }
    return false;
}


void basestorageSetDetail(RoomDetail *detail)
{
    if(detail)
        s_state.idDetail = detail->id;
}

/////////////////////////////////////////////////////////////////

int basestorageSortSalvageCompare(const void *item1, const void *item2)
{
    StoredSalvage **sal1 = (StoredSalvage **) item1;
    StoredSalvage **sal2 = (StoredSalvage **) item2;

    if (!sal1)
        return 1;
        
    if (!sal2)
        return -1;

    return strcmp(textStd((*sal1)->salvage->ui.pchDisplayName), textStd((*sal2)->salvage->ui.pchDisplayName));
}

void basestorageSortSalvage(StoredSalvage **salItems)
{
    eaQSort(salItems, basestorageSortSalvageCompare);
}

int basestorageSortEnhancementsCompare(const void *item1, const void *item2)
{
    StoredEnhancement **enh1 = (StoredEnhancement **) item1;
    StoredEnhancement **enh2 = (StoredEnhancement **) item2;
    int retval = 0;

    if (!enh1)
        return 1;

    if (!enh2)
        return -1;

    retval = strcmp(textStd((*enh1)->enhancement[0]->ppowBase->pchDisplayName), textStd((*enh2)->enhancement[0]->ppowBase->pchDisplayName));
    if (retval == 0)
        retval = (*enh1)->enhancement[0]->iLevel - (*enh2)->enhancement[0]->iLevel;
    return retval;
}

void basestorageSortEnhancements(StoredEnhancement **salItems)
{
    eaQSort(salItems, basestorageSortEnhancementsCompare);
}


int basestorageSortInspirationsCompare(const void *item1, const void *item2)
{
    StoredInspiration **insp1 = (StoredInspiration **) item1;
    StoredInspiration **insp2 = (StoredInspiration **) item2;

    if (!insp1)
        return 1;

    if (!insp2)
        return -1;

    return strcmp(textStd((*insp1)->ppow->pchIconName), textStd((*insp2)->ppow->pchIconName));
}

void basestorageSortInspirations(StoredInspiration **salItems)
{
    eaQSort(salItems, basestorageSortInspirationsCompare);
}

/////////////////////////////////////////////////////////////////

int basestorageWindow()
{
    static ScrollBar sb = { WDW_BASE_STORAGE, 0 };
    static ComboBox hideBox = {0};
    static bool init = false;
    static bool once = false;
    static float widthLast = 0.0f;
    float x, y, z, wd, ht, sc;
    int color, bcolor;
    int i,size;
    float oldy;
    Entity *e = playerPtr();
    char *curTab;
    int editmode = baseedit_Mode();
    RoomDetail *detail = NULL;
    static SMFBlock smf_err;
    char count_str[9];
    int count_wd, button_wd = 100;
    static int lastUpdate = 0;


    if(!window_getDims(WDW_BASE_STORAGE, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
    {
        init = 1;
        lastUpdate = 0;
        return 0;
    }

    if (wd != widthLast)
    {
        forceReformat = true;
        widthLast = wd;
    }

    if( !once )
    {
        basestoragestate_Init(&s_state);
        once = true;
    }

    if (init || s_state.idDetail != e->pl->detailInteracting)
    {
        detail = baseGetDetail(&g_base, e->pl->detailInteracting, NULL);
        basestorageSetDetail(detail);        
        lastUpdate = 0;
        init = 0;
    }

       detail = baseGetDetail(&g_base, s_state.idDetail, NULL); 
    i = 0;
    oldy = 0.f;
    curTab = 0;
    size = 0;

    // if we're editing, get out
    if( editmode 
        || !detail 
        || !detail->info 
         || (detail->info->eFunction != kDetailFunction_StorageSalvage
        && detail->info->eFunction != kDetailFunction_StorageInspiration
        && detail->info->eFunction != kDetailFunction_StorageEnhancement))
    {
        // frame around window
        drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );
        window_setMode( WDW_BASE_STORAGE, WINDOW_SHRINKING );
        return 0;
    }

    // MinMax is safeguard against overflow in static buffer
     sprintf( count_str, "%i/%i", MINMAX(roomdetail_CurrentCount(detail),0,999), MINMAX(roomdetail_MaxCount(detail),0,999) );
     count_wd = MAX( button_wd*sc, str_wd(font_grp, sc, sc, count_str) ) + 2*R10*sc;

    // --------------------
    // display the items
    if( window_getMode(WDW_BASE_STORAGE) != WINDOW_DISPLAYING )
    {
        drawJoinedFrame2Panel( PIX3, R10, x, y, z, sc, wd, 40*sc, ht - 40*sc, color, bcolor, NULL, NULL );
    }
    else
    {        
        CBox box;
        void *mouseOverItem = NULL;
        void **items = NULL;
        StoredSalvage **salItems = detail->storedSalvage;
        StoredInspiration **inspItems = detail->storedInspiration;
        StoredEnhancement **enhItems = detail->storedEnhancement;
        int nItems = 0;
        float dy_title, ty, dy_item = 0;
        static float dy_err;
        int doSort = false;

        gTextTitleAttr.piScale = (int*)((int)(SMF_FONT_SCALE*1.2f*sc));
        gTextAttr_White12.piScale = (int*)((int)(SMF_FONT_SCALE*sc));
    
        // Name
        ty = y + smf_ParseAndDisplay( &smf_title, textStd(detail->info->pchDisplayName), x + R10*sc, y + R10*sc, z, wd - R10*sc*2 - count_wd, 0, false, false, 
                                        &gTextTitleAttr, NULL, 0, true) + 15*sc;
        
        // Short Desc
        ty += smf_ParseAndDisplay( &smf_desc, detailGetDescription(detail->info,false), x + R10*sc, ty, z, wd - R10*sc*2, 0, false, false, 
                                    &gTextAttr_White12, NULL, 0, true) + 5*sc;
        
        dy_title = MAX( 40*sc, ty-y); // force a minimum size

        if( flash_timer )
        {
            int flash = sinf(flash_timer)*128;
             bcolor |= ((flash*2)<<24)|(flash<<16)|(flash<<8)|(flash);
              flash_timer += TIMESTEP/4;
            if( flash_timer > PI )
                flash_timer = 0.f;
        }

        showLogCenterX = x + wd - button_wd/2*sc - R10*sc;
        showLogCenterY = y + dy_title - 15*sc;
        if( D_MOUSEHIT == drawStdButton( showLogCenterX, showLogCenterY, z, button_wd*sc, 20*sc, color, "ShowLog", 1.f, !detail ) )
            openDetailLog(detail->id);
        showPermCenterX = x + wd - (3 * button_wd)/2*sc - R10*5*sc;
        showPermCenterY = y + dy_title - 15*sc;
        if( D_MOUSEHIT == drawStdButton( showPermCenterX, showPermCenterY, z, button_wd*1.5*sc, 20*sc, color, "ShowPermissionsStr", 1.f, !detail ) )
        {
            openPermissions(detail);
        }

         font( &game_12 );
        if( roomdetail_MaxCount(detail) == roomdetail_CurrentCount(detail) )
            font_color(0xff0000ff,0xff0000ff);
        else
            font_color( CLR_GREY, CLR_GREY );
           cprntEx( x + wd - button_wd/2*sc - R10*sc, y + PIX3*sc + (dy_title-25*sc)/2, z, sc, sc, CENTER_X|CENTER_Y, count_str );    
 
        //if( err_msg && err_timer )
        {
            static char *last_err_msg;
            int color = CLR_WHITE;
            float txtSc = sc;

            if( err_timer < 0 )
                color = CLR_RED;

            if( err_timer && fabs(err_timer) < 3 )
                 color &= (0xffffff00 | (int)(((err_timer<0?-err_timer:err_timer)/3.f)*255.f) );
            
             gTextErr.piScale = (int*)((int)(SMF_FONT_SCALE*sc));
            gTextErr.piColor = (int*)color;

            if( err_timer == 0 ) 
            {
                 err_msg = strdup(textStd("ClickToTakeDefault"));
            }

            if( err_msg && (!last_err_msg || stricmp(err_msg, last_err_msg)!=0 ))
            {
                 smf_ParseAndFormat(&smf_err, err_msg, 0, 0, 0, wd - (R10+PIX3)*sc*2, 1000, false, true, false, &gTextErr, 0 );
                smf_err.pBlock->pos.alignHoriz = SMAlignment_Center;
                 dy_err = MAX( 26*sc, 5*sc + smf_ParseAndFormat(&smf_err, err_msg, 0, 0, 0, wd - (R10+PIX3)*sc*2, 1000, false, false, true, &gTextErr, 0 ) );

                if(last_err_msg)
                    free(last_err_msg);
                last_err_msg = strdup(err_msg);
            }

               smf_ParseAndDisplay( &smf_err, NULL, x + R10*sc, y+ht-dy_err+PIX3*sc, z+20, wd - (R10+PIX3)*sc*2, 0, false, false, 
                                    &gTextErr, NULL, 0, true);

             if( err_timer > 0 )
                err_timer = MAX( 0, err_timer-TIMESTEP/15);
             else
                err_timer = MIN( 0, err_timer+TIMESTEP/15);
        }

        drawJoinedFrame3Panel( PIX3, R10, x, y, z, sc, wd, dy_title, ht - dy_title - dy_err, dy_err, color, bcolor );

        // --------------------
        // handle the items
        set_scissor(1);
           scissor_dims( x+PIX3*sc, y+dy_title, wd-2*PIX3*sc, ht-dy_err-dy_title-PIX3*sc );
        BuildCBox( &box, x+PIX3*sc, y+dy_title, wd-2*PIX3*sc, ht-PIX3*sc);

        if (g_base.update_time >= lastUpdate)
        {
            doSort = true;
            lastUpdate = g_base.update_time;
        }

        switch ( detail->info->eFunction )
        {
            case kDetailFunction_StorageSalvage:
                nItems = eaSize(&salItems);
                items = salItems;
                if (doSort)
                {
                    basestorageSortSalvage(detail->storedSalvage);
                }
                break;
            case kDetailFunction_StorageInspiration:
                nItems = eaSize(&inspItems);
                items = inspItems;
                if (doSort)
                {
                    basestorageSortInspirations(detail->storedInspiration);
                }
                break;
            case kDetailFunction_StorageEnhancement:
                nItems = eaSize(&enhItems);
                items = enhItems;
                if (doSort)
                {
                    basestorageSortEnhancements(detail->storedEnhancement);
                }
                break;
            default:
                verify(0 && "invalid switch value.");
        };

          if( !nItems )
        {
            font(&game_18);
            font_color(0xffffff22 ,0xffffff22 );
             cprntEx( x + wd/2, y +dy_title + (ht-dy_title)/2, z, 1.5f, 1.5f, CENTER_X|CENTER_Y, "DragItemsHere" );
        }
        
        for( i = 0; i < nItems; ++i ) 
        {
            int iRow = i;
            bool over = false;
            void *item = NULL;
            float fHeight = 0.0f;

            ty = y + dy_title - sb.offset + dy_item;
            

            switch ( detail->info->eFunction )
            {
            case kDetailFunction_StorageSalvage:
                item = salItems[i];
                over = s_DisplayStoredSalvage( &s_state, item, box.left, ty, z, sc, BOX_DX(box), &fHeight, false, detail->permissions );
                break;
            case kDetailFunction_StorageInspiration:
                item = inspItems[i];
                over = s_DisplayStoredInspiration( &s_state, item, box.left, ty, z, sc, BOX_DX(box), &fHeight, false, detail->permissions );                
                break;
            case kDetailFunction_StorageEnhancement:
                item = enhItems[i];
                over = s_DisplayStoredEnhancement( &s_state, item, box.left, ty, z, sc, BOX_DX(box), &fHeight, false, detail->permissions );
                break;
            default:
                if( isDevelopmentMode() || e->access_level )
                    verify(0 && "Base detail for storage window has invalid switch value.");
            };

            if (-sb.offset + dy_item + fHeight < 0 )
            {
                dy_item += fHeight;
                continue;
            }
            else if (dy_title - sb.offset + dy_item > ht )
            {
                dy_item += fHeight;
                continue;
            }
            
            switch ( detail->info->eFunction )
            {
            case kDetailFunction_StorageSalvage:
                item = salItems[i];
                over = s_DisplayStoredSalvage( &s_state, item, box.left, ty, z, sc, BOX_DX(box), &dy_item, true, detail->permissions );
                break;
            case kDetailFunction_StorageInspiration:
                item = inspItems[i];
                 over = s_DisplayStoredInspiration( &s_state, item, box.left, ty, z, sc, BOX_DX(box), &dy_item, true, detail->permissions );                
                break;
            case kDetailFunction_StorageEnhancement:
                item = enhItems[i];
                over = s_DisplayStoredEnhancement( &s_state, item, box.left, ty, z, sc, BOX_DX(box), &dy_item, true, detail->permissions );
                break;
            default:
                if( isDevelopmentMode() || e->access_level )
                    verify(0 && "Base detail for storage window has invalid switch value.");
            };
 
            if( over && !cursor.dragging ) 
            {
                setCursor( "cursor_hand_open.tga", NULL, FALSE, 2, 2 );
                mouseOverItem = item;
            }
        }
        forceReformat = false; 
        set_scissor(0);
        
        //scrollbar
           doScrollBar( &sb, ht - dy_title - (PIX3+R10)*sc*2-dy_err, dy_item, wd, dy_title+(PIX3+R10)*sc, z+5, &box, 0 );
        
        // grow the current icon
         if(!cursor.dragging)    
            uigrowbig_Update(&s_state.growbig, mouseOverItem );
        
        // --------------------
        // DnD drag and drop interaction
        
        // drop salvage 
        if ( cursor.dragging )
        {
            TrayObj *obj = &cursor.drag_obj;

            switch ( cursor.drag_obj.type )
            {
            case kTrayItemType_Salvage:
                {
                    SalvageInventoryItem *itm = e->pchar->salvageInv[cursor.drag_obj.invIdx];
                    if(itm && itm->salvage && detail->info->eFunction == kDetailFunction_StorageSalvage )
                    {
                        if (itm->salvage->flags & SALVAGE_NOTRADE)
                        {
                            baseStorage_ErrMessage(textStd("InventoryAddFromStorageFailed", e->pchar->salvageInv[cursor.drag_obj.invIdx]->salvage->ui.pchDisplayName), -6);
                        }
                        else if(!entity_CanAdjStoredSalvage(e,detail,(SalvageItem*)e->pchar->salvageInv[cursor.drag_obj.invIdx]->salvage,1))
                        {
                            drawFrame(PIX3, R10, x, y+dy_title-PIX3*sc, z+15, wd, ht-dy_title-dy_err, sc, 0, 0x00000088 );
                            if( roomdetail_MaxCount(detail) == roomdetail_CurrentCount(detail) )
                                baseStorage_ErrMessage( textStd("ContainerFull"), -3 );
                            else
                                baseStorage_ErrMessage(textStd("InventoryAddFromStorageFailed", e->pchar->salvageInv[cursor.drag_obj.invIdx]->salvage->ui.pchDisplayName), -6);
                        }
                    }
                }
                break;
            case kTrayItemType_Inspiration:
                {
                    const BasePower *ppow = obj?e->pchar->aInspirations[obj->iset][obj->ipow]:NULL;
                    if( ppow && detail->info->eFunction == kDetailFunction_StorageInspiration)
                    {
                        if(!entity_CanAdjStoredInspiration(e, detail, ppow, basepower_ToPath(ppow), 1))
                        {
                               drawFrame(PIX3, R10, x, y+dy_title-PIX3*sc, z+15, wd, ht-dy_title-dy_err+PIX3*sc, sc, 0, 0x00000088 );
                            if( roomdetail_MaxCount(detail) == roomdetail_CurrentCount(detail) )
                                baseStorage_ErrMessage( textStd("ContainerFull"), -3 );
                            else
                                baseStorage_ErrMessage(textStd("InventoryAddFromStorageFailed", ppow->pchDisplayName), -6);

                        }
                    }
                }
                break;
            case kTrayItemType_SpecializationInventory: // enhancement
                {
                    Boost *boost = e->pchar->aBoosts[obj->ispec]?e->pchar->aBoosts[obj->ispec]:NULL;
                    const BasePower *ppow = boost?boost->ppowBase:NULL;
                    if( ppow && detail->info->eFunction == kDetailFunction_StorageEnhancement)
                    {
                        if(!entity_CanAdjStoredEnhancement(e, detail, ppow, basepower_ToPath(ppow),boost->iLevel,boost->iNumCombines,1))
                        {
                             drawFrame(PIX3, R10, x, y+dy_title-PIX3*sc, z+15, wd, ht-dy_title+PIX3*sc, sc, 0, 0x00000088 );
                            if( roomdetail_MaxCount(detail) == roomdetail_CurrentCount(detail) )
                                baseStorage_ErrMessage( textStd("ContainerFull"), -3 );
                            else
                                baseStorage_ErrMessage(textStd("InventoryAddFromStorageFailed", ppow->pchDisplayName), -6);
                        }
                    }
                }
                break;
            };
            
              if( mouseCollision(&box) && !isDown(MS_LEFT) && e )
            {
                Character *pchar = e->pchar;            
                switch ( cursor.drag_obj.type )
                {
                case kTrayItemType_Salvage:
                    {
                        SalvageInventoryItem *itm = pchar->salvageInv[cursor.drag_obj.invIdx];
                        if(itm && itm->salvage )
                        {
                            salvageAddToStorageContainer(itm, 10000);
                        }
                    }
                    break;
                case kTrayItemType_Inspiration:
                    {
                        const BasePower *ppow = obj?e->pchar->aInspirations[obj->iset][obj->ipow]:NULL;

                        if( ppow ) 
                        {
                            inspirationAddToStorageContainer(ppow,detail->id,obj->iset,obj->ipow,1);
                        }
                    }
                    break;
                case kTrayItemType_SpecializationInventory: // enhancement
                    {
                        Boost *boost = e->pchar->aBoosts[obj->ispec]?e->pchar->aBoosts[obj->ispec]:NULL;
                        const BasePower *ppow = boost?boost->ppowBase:NULL;

                        if( ppow )
                        {
                            enhancementAddToStorageContainer(boost,detail->id,obj->ispec,1);
                        }
                    }
                    break;
                };
                trayobj_stopDragging();
            }
        }
    }

    // ----------
    // finally

     return 0;
}



/* End of File */
